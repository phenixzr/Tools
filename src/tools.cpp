#include <iostream>
#include "Logger.h"
#include "ConfigReader.h"
#include "LuaBridgeWrapper.h"

void dummy(int a)
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

int main(int argc, char** argv)
{
    int bRet = 0;
    tools::Logger::getInstance().init();
    theConfReader.init();

    std::thread t1(dummy,1);
    std::thread t2(dummy,2);
    std::thread t3(dummy,3);
    std::thread t4(dummy,4);

    LOG_DEBUG("je logue depuis main !");
    std::this_thread::sleep_for (std::chrono::seconds(1));
    int x = 13;
    LOG_DEBUG("je logue une valeur ! %d", x);

    float f = theConfReader.getFloat("angle");
    std::string str = theConfReader.getStr("title");
    int i = theConfReader.getInt("speed");
    
    LOG_DEBUG("je recupere la valeur angle  ! %f", f);
    LOG_DEBUG("je recupere la valeur title  ! %s", str.c_str());
    LOG_DEBUG("je recupere la valeur speed  ! %d", i);

    t1.join();t2.join();t3.join();t4.join();

    // lancer les testU

    return bRet;
}