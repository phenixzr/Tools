#ifndef LOGGER_H
#define LOGGER_H

#include <stdarg.h>
#include <mutex>
#include <thread>
#include <chrono>
#include <ctime>

#define LOG_ERROR(msg, ...) tools::Logger::getInstance().write("ERROR   ", msg, __VA_ARGS__)
#define	LOG_WARN(msg, ...)  tools::Logger::getInstance().write("WARNING ", msg, __VA_ARGS__)
#define	LOG_INFO(msg, ...)  tools::Logger::getInstance().write("INFO    ", msg, __VA_ARGS__)
#ifdef NDEBUG
  #define LOG_DEBUG(msg, ...) 
#else
  #define LOG_DEBUG(msg, ...) tools::Logger::getInstance().write("DEBUG   ", msg, __VA_ARGS__)
#endif

namespace tools
{
    class Logger
    {
    public:

        bool init(const char *version = "0.0.0.0", const char *fileName = "log.txt")
        {	
            bool bRet = false;
            if ( _logFile == NULL )
            {
                if ( fopen_s(&_logFile, fileName, "w") == 0 )
                {
                    fprintf(_logFile, "Software Version : %s\n\n\n", version);
                    bRet = true;
                }
            }
            return bRet;
        }

        ~Logger()
        {
            if ( _logFile != NULL ) 
            {
                fprintf(_logFile,"\n\n---- End of file ----\n");
                fclose(_logFile);
                _logFile = NULL;
            }
        }
    
        static Logger& getInstance()
        {
            static Logger instance;
            return instance;
        }
        
        void write(const char* level, const char* msg, ...)
        {
            std::lock_guard<std::mutex> locker(_mtx);
            va_list args;
            va_start(args, msg);
                printCurrentDate();
                fprintf(_logFile, "[%s]  ", level);
                fprintf(_logFile, "%-10x    ", std::hash<std::thread::id>()(std::this_thread::get_id()));
                vfprintf(_logFile, msg, args);
                fprintf(_logFile, "\n");
            va_end(args);
            fflush(_logFile);
        }

    private:
        Logger(): _logFile(NULL){};
        Logger(const Logger&){_logFile=NULL;};
        Logger& operator=(const Logger&){return *this;};

        void printCurrentDate()
        {
            char dateFormat[30];
            std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
            std::chrono::system_clock::duration tp = now.time_since_epoch();
            tp -= std::chrono::duration_cast<std::chrono::seconds>(tp);
            time_t tt = std::chrono::system_clock::to_time_t(now);
            tm t;
            gmtime_s(&t, &tt);

            int len = sprintf_s(dateFormat
                    , "%04u-%02u-%02u %02u:%02u:%02u.%03u"
                    , t.tm_year + 1900
                    , t.tm_mon + 1
                    , t.tm_mday
                    , t.tm_hour
                    , t.tm_min
                    , t.tm_sec
                    , std::chrono::duration_cast<std::chrono::milliseconds>(tp).count());

            fprintf(_logFile, "%.*s  ", len, dateFormat);
        }

        FILE* _logFile;
        std::mutex _mtx;
    };
} //ns tools

#endif //LOGGER_H