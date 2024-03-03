#pragma  once

#include "lockqueue.h"

#include <time.h>
#include <thread>
#include <unistd.h>
#include <fcntl.h>
#include <string>

#define LOG_INFO(logmsgformat, ...) \
        do{ \
            fst::Logger& logger = fst::Logger::GetInstance(); \
            logger.SetLogLevel(fst::INFO);   \
            char buf[1024] = {0};   \
            snprintf(buf, 1024, logmsgformat, ##__VA_ARGS__);   \
            logger.Log(buf);    \
        }while(0);

#define LOG_ERROR(logmsgformat, ...) \
        do{ \
            fst::Logger& logger = fst::Logger::GetInstance(); \
            logger.SetLogLevel(fst::ERROR);   \
            char buf[1024] = {0};   \
            snprintf(buf, 1024, logmsgformat, ##__VA_ARGS__);   \
            logger.Log(buf);    \
        }while(0);

#define LOG_DEBUG(logmsgformat, ...) \
        do{ \
            fst::Logger& logger = fst::Logger::GetInstance(); \
            logger.SetLogLevel(fst::DEBUG);   \
            char buf[1024] = {0};   \
            snprintf(buf, 1024, logmsgformat, ##__VA_ARGS__);   \
            logger.Log(buf);    \
        }while(0);

namespace fst {

enum LogLevel{
    DEBUG = 0,
    INFO = 1,
    ERROR = 2
};

class Logger
{
public:
    static Logger& GetInstance();
    void SetLogLevel(LogLevel level);
    void Log(std::string msg);
private:
    int m_logLevel;
    LockQueue<std::string> m_lockQueue;
    // std::list<pthread_t> m_threads;
    // int m_thread_numberr;
private:
    Logger();
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
};

}