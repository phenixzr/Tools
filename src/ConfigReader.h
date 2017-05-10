#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#include <map>

#define theConfReader tools::ConfigReader::getInstance()
namespace tools
{
    class ConfigReader
    {
    public:

        static ConfigReader& getInstance()
        {
            static ConfigReader instance;
            return instance;
        }       

        ~ConfigReader(){}
        bool init(const char* file = "config.ini");
        int getInt(const char* key) const;
        const std::string& getStr(const char* key) const;
        float getFloat(const char* key) const;

    private:
        ConfigReader(){}
        ConfigReader(const ConfigReader&){}
        ConfigReader& operator=(const ConfigReader&){return *this;}

        std::map<std::string, std::string> _configMap;
    };
} //ns tools

#endif //CONFIG_READER_H