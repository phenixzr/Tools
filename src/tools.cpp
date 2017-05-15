#include <iostream>
#include <assert.h>

#include "Logger.h"
#include "ConfigReader.h"
#include "LuaBridgeWrapper.h"
#include "ObjectPool.h"
#include <SFML/Graphics.hpp>

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


    // test lua bridge
	const char* file1 = "Data/script1.lua"; 
	const char* file2 = "Data/script2.lua"; 
	const char* keyStr = "testString";
	const char* keyInt = "n1";
	const char* keyDouble = "d1";
	const char* keyFloat = "f1";

	std::cout <<"\n[LuaBridge TEST]\n"<<std::endl;
	tools::LuaBridgeWrapper reader(file1);
	std::cout << reader.getValue<std::string>(keyStr) <<std::endl;
	std::cout << reader.getValue<float>(keyFloat) <<std::endl;
	std::cout << reader.getValue<double>(keyDouble) <<std::endl;
	std::cout << reader.getValue<int>(keyInt) <<std::endl;

	reader.init(file2);
	std::cout << reader.getValue<std::string>(keyStr) <<std::endl;
	std::cout << reader.getValue<float>(keyFloat) <<std::endl;
	std::cout << reader.getValue<double>(keyDouble) <<std::endl;
	std::cout << reader.getValue<int>(keyInt) <<std::endl;

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


	sf::RenderWindow window(sf::VideoMode(300, 300), "SFML test!");
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
        window.draw(shape);
        window.display();
    }

    return bRet;
}