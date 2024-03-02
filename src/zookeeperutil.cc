#include "zookeeperutil.h"

#include <cstdlib>
#include <iterator>
#include <semaphore.h>
#include <string>
#include <zookeeper/zookeeper.h>

#include "mprpcapplication.h"
#include "logger.h"
namespace fst {

//全局watcher观察器  zkserver给zkclient的通知
void global_watcher(zhandle_t*zh , int type,
                int state, const char* path, void *watcherCtx){
    if(type == ZOO_SESSION_EVENT){
        if(state == ZOO_CONNECTED_STATE){
            sem_t* sem = (sem_t*)zoo_get_context(zh);
            sem_post(sem);
        }
    }
}

ZkClient::ZkClient() : m_zhandle(nullptr){
}

ZkClient::~ZkClient(){
    if(m_zhandle != nullptr){
        zookeeper_close(m_zhandle);
    }
}
//连接zkserver
void ZkClient::Start(){
    std::string host = MprpcApplication::GetInstance().GetConfig().Load("zookeeperip");
    std::string port = MprpcApplication::GetInstance().GetConfig().Load("zookeeperport");
    std::string connstr = host + ":" + port;
    std::cout << __FILE__ << __FUNCTION__ << connstr << std::endl;

    /**
    多线程版本

    API调用线程
    网络I/O线程
    watcher回调线程
    */
    m_zhandle = zookeeper_init(connstr.c_str(), global_watcher,
             30000, nullptr, nullptr, 0);
    if (nullptr == m_zhandle) {
        LOG_ERROR("zookeeper_init error!");
        exit(EXIT_FAILURE);
    }

    sem_t sem;
    sem_init(&sem, 0, 0);       
    zoo_set_context(m_zhandle, &sem);
    sem_wait(&sem);         //阻塞等待响应

    LOG_INFO("zookeeper_init success!");
}

void ZkClient::Create(const char* path,const char* data,int datalen,int state){
    char path_buffer[128];
    int buffer_length = sizeof(path_buffer);
    int flag = 0;
    //先判断path表示的节点是否存在，存在即不重复创建
    flag = zoo_exists(m_zhandle, path, 0, nullptr);
    if(ZNONODE == flag){
        flag = zoo_create(m_zhandle, path, data, datalen, 
        &ZOO_OPEN_ACL_UNSAFE, state, path_buffer, buffer_length);
        if(ZOK == flag){
            LOG_INFO("znode create success... path: %s" ,path);
        }else {
            LOG_INFO("flage: %d" ,flag);
            LOG_INFO("znode create error... path: %s" ,path);
            exit(EXIT_FAILURE);
        }
    }
}

std::string ZkClient::GetDate(const char*path){
    char buffer[64];
    int buffer_length = sizeof(buffer);
    int flag = zoo_get(m_zhandle, path, 0, buffer, &buffer_length, nullptr);
    if(flag != ZOK){
        LOG_INFO("get znode error... path: %s", path);
        return "";
    }
    return std::string(buffer);
}

}