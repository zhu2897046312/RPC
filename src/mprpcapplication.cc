#include "mprpcapplication.h"
#include "mprpcconfig.h"

#include <cstdlib>
#include <unistd.h>
#include <iostream>

namespace fst {

MprcpConfig MprpcApplication::m_config;
bool MprpcApplication::is_init = false;

void ShowArgsHelp(){
    std::cout << "format: command -i <config_file>" << std::endl;
}

void MprpcApplication::Init(int argc, char **argv){
    //只需要初始化一次即可
    if (is_init) {
        return;
    }
    is_init = true;

    if(argc < 2){
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }

    int c = 0;
    std::string config_file;
    while((c = getopt(argc,argv,"i:")) != -1){
        switch (c) {
        case 'i':
            config_file = optarg;
            break;
        case '?':
            ShowArgsHelp();
            exit(EXIT_FAILURE);
            break;
        case ':':
            ShowArgsHelp();
            exit(EXIT_FAILURE);
            break;    
        default:
            break;
        }
    }

    //加载配置文件 rpcserver_ip = .. rpcserver_port = ... 
    //           zookeeper_ip = .. zookpper_port = ..
    m_config.LoadConfigFile(config_file.c_str());

    //std::cout << "rpcserverip:" << m_config.Load("rpcserverip") << std::endl; 
    //std::cout << "rpcserverport:" << m_config.Load("rpcserverport") << std::endl; 
    //std::cout << "zookeeperip:" << m_config.Load("zookeeperip") << std::endl; 
    //std::cout << "zookeeperport:" << m_config.Load("zookeeperport") << std::endl; 
}

MprpcApplication& MprpcApplication::GetInstance(){
    static MprpcApplication app;
        return app;
}

MprcpConfig& MprpcApplication::GetConfig(){
    return m_config;
}

}