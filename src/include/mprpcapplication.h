#pragma once

#include "mprpcconfig.h"
namespace fst {

//使用单例模式 负责框架的初始化操作
class MprpcApplication
{
public:
    static void Init(int argc,char **argv);
    static MprpcApplication& GetInstance();
    static MprcpConfig& GetConfig();
private:
    static MprcpConfig m_config;
    static bool is_init;
private:
    MprpcApplication(){}
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(MprpcApplication&&) = delete;
};

}