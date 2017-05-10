#include <fstream>
#include <string>
#include <sstream>
#include "ConfigReader.h"
#include "Logger.h"

namespace tools{

bool ConfigReader::init(const char* file)
{
    bool bRet = false;

    std::ifstream confFile(file, std::ifstream::in);
    if ( confFile.is_open() )
    {
        LOG_INFO("Configuration file : %s loaded",file);
        _configMap.clear();

        std::string line;
        while( std::getline(confFile, line) )
        {
            std::istringstream iss(line);
            std::string key, value;

            iss >> key;
            iss >> value;
            _configMap.insert(std::make_pair(key, value));
            LOG_INFO("\tParameter %s : %s", key.c_str(), value.c_str());    
        }

        bRet = true;
        confFile.close();
    }
    else
    {
        LOG_ERROR("Cant load configuration file : %s", file);
    }

    return bRet;
}

int ConfigReader::getInt(const char* key) const
{
    return std::stoi(_configMap.at(key));
}

const std::string& ConfigReader::getStr(const char* key) const
{
    return _configMap.at(key);
}

float ConfigReader::getFloat(const char* key) const
{
    return std::stof(_configMap.at(key));
}

} //ns tools