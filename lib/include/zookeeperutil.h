#pragma once

#include <semaphore.h>
#include <string>
#include <zookeeper/zookeeper.h>

namespace fst {

class ZkClient
{
public:
    ZkClient();
    ~ZkClient();
    void Start();
    void Create(const char* path,const char* data,int datalen,int state = 0);
    //节点路径
    std::string GetDate(const char*path);
private:
    zhandle_t*m_zhandle;    //句柄
};

}