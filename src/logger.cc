#include "logger.h"

#include <stdlib.h>
#include <iostream>


namespace fst {

Logger::Logger(){
    // m_thread_numberr = 1;

    std::thread writeLogTask([&](){
        while(true){
            time_t now = time(nullptr);
            auto nowtm = localtime(&now);

            char file_name[128];
            sprintf(file_name, "%d-%d-%d-log.txt", 
                nowtm->tm_year + 1900, nowtm->tm_mon + 1, nowtm->tm_mday);

            // FILE *pf = fopen(file_name, "a+");
            // if(pf == nullptr){
            //     std::cout << "logger file open error" << std::endl;
            //     exit(EXIT_FAILURE);
            // }
            // std::string msg = m_lockQueue.Pop();
            // char time_buf[128] = {0};
            // sprintf(time_buf, "%d:%d:%d ==> ", nowtm->tm_hour, nowtm->tm_min, nowtm->tm_sec);
            // msg.insert(0,time_buf);
            // msg.append("\n");
            // fputs(msg.c_str(),pf);
            // fclose(pf);

            int fd = open(file_name,  O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
            if(-1 == fd){
                perror("failed to create file");
                exit(EXIT_FAILURE);
            }

            std::string msg = m_lockQueue.Pop();
            char time_buf[128] = {0};
            sprintf(time_buf, "%d:%d:%d ==> [%s] ", 
                        nowtm->tm_hour, 
                        nowtm->tm_min, 
                        nowtm->tm_sec,
                        (m_logLevel == INFO ? "info" : "error"));
            msg.insert(0,time_buf);
            msg.append("\n");
            if(-1 == write(fd, msg.c_str(), msg.size())){
                perror("failed to write file");
                m_lockQueue.Push(msg);
                if(close(fd) == -1){
                    perror("failed to close file");
                    exit(EXIT_FAILURE);
                }
            }
            close(fd);
        }

    });

    writeLogTask.detach();
}

Logger& Logger::GetInstance(){
    static Logger logger;
    return logger;
}

void Logger::SetLogLevel(LogLevel level){
    m_logLevel = level;
}

void Logger::Log(std::string msg){
    m_lockQueue.Push(msg);
}

}