#pragma once

#include <unordered_map>
#include <string>


namespace fst
{

//框架配置文件类
class MprcpConfig
{
public:
    //加载配置文件
    void LoadConfigFile(const char* config_file);

    //查询配置项信息
    std::string Load(const std::string& key);
    
private:
    //去掉字符串前后的空格
    void trim(std::string& str);

private:
    std::unordered_map<std::string, std::string> m_configMap;
};


}