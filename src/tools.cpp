#include <iostream>

#include "Logger.h"
#include "ConfigReader.h"
#include "ObjectPool.h"
#include <Lua\selene.h>
#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/SFMLOrthogonalLayer.hpp>
#include <rapidjson/document.h>
#include <rapidjson/filestream.h>

void dummyFunc(int a)
{
    switch (a)
    {
    case 1 :
        LOG_INFO("je logue une info !");
        break;
    case 2 :
        LOG_WARN("je logue un warning !");
        break;
    case 3 :
        LOG_ERROR("je logue une erreur !");
        break;
    default :
        LOG_INFO("je logue par defaut !");
        break;
    }
}

struct DummyClass
{
	DummyClass() { clear(); }
	void clear() { _nbOpe = 0; _way = 'D'; _id.resize(0); }
	~DummyClass(){std::cout<<"Dest called \n";}
	int _nbOpe;
	char _way;
	std::string _id;
};

int main(int argc, char** argv)
{
    int bRet = 0;
    tools::Logger::getInstance().init();
    theConfReader.init("Data/config.ini");

#pragma region TEST_LOGGER_+_CONF_READER
	std::thread t1(dummyFunc,1);
	std::thread t2(dummyFunc,2);
	std::thread t3(dummyFunc,3);
	std::thread t4(dummyFunc,4);

	// test logger
	LOG_DEBUG("je logue depuis main !");
	std::this_thread::sleep_for (std::chrono::seconds(1));
	int x = 13;
	LOG_DEBUG("je logue une valeur ! %d", x);

#pragma region TEST_CONF_READER
	// test config reader
	float f = theConfReader.getFloat("angle");
	std::string str = theConfReader.getStr("title");
	int i = theConfReader.getInt("speed");

	std::cout <<"[ConfReader TEST]\n"<<std::endl;
	std::cout<<"angle = "<<f<<std::endl;
	std::cout<<"title = "<<str<<std::endl;
	std::cout<<"speed = "<<i<<std::endl;  
#pragma endregion


	LOG_DEBUG("je recupere la valeur angle  ! %f", f);
	LOG_DEBUG("je recupere la valeur title  ! %s", str.c_str());
	LOG_DEBUG("je recupere la valeur speed  ! %d", i);

	t1.join();t2.join();t3.join();t4.join();  
#pragma endregion

#pragma region TEST_OBJPOOL
	tools::ObjectPool<DummyClass>* dummyObjPool = new tools::ObjectPool<DummyClass>(2,5);
	DummyClass* dobj = dummyObjPool->grab();
	dummyObjPool->grab();
	dummyObjPool->grab();
	dummyObjPool->grab();
	dummyObjPool->release(dobj);
	dobj = dummyObjPool->grab();
	delete dummyObjPool;  
#pragma endregion

#pragma region TEST_LUA
	sel::State sel;
	int lval1 = 12, lval2 = 4, res = 0;
	sel.Load("Data/script1.lua");
	res = sel["add"](lval1, lval2);
	std::cout << "calling lua function : add " << lval1 << " + " << lval2 << " = " << res << std::endl;;
	std::cout << "reading values from LUA " << static_cast<int>( sel["n1"] )<< std::endl;
	std::cout << "reading values from LUA " << static_cast<double>( sel["d1"] ) << std::endl;
	std::string lstr = sel["testString"];
	std::cout << "reading values from LUA " << lstr << std::endl;
    std::cout << "reading values from LUA " << static_cast<double>( sel["f1"] ) << std::endl;
#pragma endregion

	FILE* jsFile = NULL;
	assert(fopen_s(&jsFile, "Data/file.json", "r") == 0);
	rapidjson::FileStream isw(jsFile);
	rapidjson::Document doc;
		
	if (!doc.ParseStream<0>(isw).HasParseError())
	{
		printf("\nAccess values in document:\n");
		assert(doc.IsObject());
		assert(doc.HasMember("hello"));
		assert(doc["hello"].IsString());
		printf("hello = %s\n", doc["hello"].GetString());

		const rapidjson::Value& a = doc["textures"];
		assert(a.IsArray());
		for (rapidjson::SizeType i = 0; i < a.Size(); i++)
		{
			const rapidjson::GenericValue <rapidjson::UTF8<>>& text = a[i];

			printf("a[%d] = file = %s, {x=%d,y=%d}, visible=%d\n"
				, i
				, text["file"].GetString()
				, text["x"].GetInt()
				, text["y"].GetInt()
				, text["visible"].GetBool());
		}
		fclose(jsFile);
	}

	
	

#pragma region TEST_SFML
	
	tmx::Map map;
	map.load("Data/dummy.tmx");
	MapLayer layerZero(map, 0);
	MapLayer layerOne(map, 1);	

	sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML test!");
	sf::CircleShape shape(150.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(layerZero);
		window.draw(layerOne);
		window.draw(shape);
		window.display();
	}
#pragma endregion

    return bRet;
}