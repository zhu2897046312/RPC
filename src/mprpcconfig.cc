#include "mprpcconfig.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>


namespace fst
{

//解析、加载配置文件

/**
* 1.注释 #
* 2.正确的配置项
* 3.去点多余的空格
*/
void MprcpConfig::LoadConfigFile(const char* config_file){
    FILE *pf = fopen(config_file, "r");
    if(nullptr == pf){
        std::cout << config_file << "is note exist!" << std::endl;
        exit(EXIT_FAILURE);
    }
    while(!feof(pf)){
        char buf[512] = {0};
        fgets(buf,512,pf);

        //去掉字符串前面的空格
        std::string src_buf(buf);
        int index = src_buf.find_first_not_of(' ');
        if(index != -1){
            //前面有
            src_buf = src_buf.substr(index,src_buf.size()-index);
        }
        //去掉字符串后面的空格
        index = src_buf.find_last_not_of(' ');
        if (-1 != index) {
            //后面有
            src_buf = src_buf.substr(0,index + 1);
        }

        // #
        if('#' == src_buf[0] || src_buf.empty()) 
            continue;

        // 解析配置项


        index  = src_buf.find('=');
        if (-1 == index) 
            continue;
        std::string key;
        std::string value;
        key = src_buf.substr(0,index);
        value = src_buf.substr(index + 1,src_buf.size() - index);
        //去掉 \n
        index = value.find('\n');
        value = value.substr(0,index);
        m_configMap.insert(std::make_pair(key, value));
    }
}


//查询配置项信息
std::string MprcpConfig::Load(const std::string& key){
    auto it = m_configMap.find(key);

    if (m_configMap.end() == it) 
        return "";
    
    return it->second;
}


}