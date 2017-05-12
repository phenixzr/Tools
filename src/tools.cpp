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

    std::thread t1(dummyFunc,1);
    std::thread t2(dummyFunc,2);
    std::thread t3(dummyFunc,3);
    std::thread t4(dummyFunc,4);

	// test logger
    LOG_DEBUG("je logue depuis main !");
    std::this_thread::sleep_for (std::chrono::seconds(1));
    int x = 13;
    LOG_DEBUG("je logue une valeur ! %d", x);

	// test config reader
    float f = theConfReader.getFloat("angle");
    std::string str = theConfReader.getStr("title");
    int i = theConfReader.getInt("speed");
    

    LOG_DEBUG("je recupere la valeur angle  ! %f", f);
    LOG_DEBUG("je recupere la valeur title  ! %s", str.c_str());
    LOG_DEBUG("je recupere la valeur speed  ! %d", i);

    t1.join();t2.join();t3.join();t4.join();

    // test lua bridge
	const char* file = "Data/script.lua"; 
	const char* keyStr = "testString";
	const char* keyNbr = "number";

	lua_State* L = luaL_newstate();
	luaL_dofile(L, file);
	luaL_openlibs(L);
	lua_pcall(L, 0, 0, 0);

	luabridge::LuaRef s = luabridge::getGlobal(L, keyStr);
	luabridge::LuaRef n = luabridge::getGlobal(L, keyNbr);

	std::string luaString = s.cast<std::string>();
	int answer = n.cast<int>();

	LOG_DEBUG("voici une string LUA %s", luaString.c_str());
	LOG_DEBUG("voici un nombre LUA %d", answer);

	// test object pool
	tools::ObjectPool<DummyClass>* dummyObjPool = new tools::ObjectPool<DummyClass>(2,5);
	DummyClass* dobj = dummyObjPool->grab();
	dummyObjPool->grab();
	dummyObjPool->grab();
	dummyObjPool->grab();
	dummyObjPool->release(dobj);
	dobj = dummyObjPool->grab();
	delete dummyObjPool;

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