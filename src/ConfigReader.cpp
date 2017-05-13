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
		size_t pos;
        while( std::getline(confFile, line) )
        {
			if( (pos=line.find("=")) != std::string::npos)
			{
				std::string key, value;

				key = line.substr(0, pos);
				value = line.substr(pos+1, line.length()-pos+1);

				_configMap.insert(std::make_pair(key, value));
				LOG_INFO("\tParameter %s : %s", key.c_str(), value.c_str());

			}
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