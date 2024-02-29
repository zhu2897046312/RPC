
#include "rpcprovider.h"
#include "mprpcapplication.h"
#include <cstdint>
#include <cstdlib>
#include <google/protobuf/descriptor.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpServer.h>
#include <functional>
#include <string>
#include <utility>


namespace fst {
/**
* serice_name --> service 描述
*                   --> service* 记录描述对象
* method_name --> method方法对象
*/
void RpcProvider::NotifyService(google::protobuf::Service *service){

    ServiceInfo service_info;

    //获取服务对象的描述信息
    const google::protobuf::ServiceDescriptor *pserviceDesc = service->GetDescriptor();
    //获取服务的名字
    std::string service_name = pserviceDesc->name();
    //获取服务对象servcice的方法的数量
    int methodCount = pserviceDesc->method_count();

    std::cout << "service_name: " << service_name << std::endl;

    for(int i = 0; i < methodCount; i++){
        //获取服务对象指定下标的服务方法的描述
        const google::protobuf::MethodDescriptor* pmethodDesc = pserviceDesc->method(i);
        std::string method_name = pmethodDesc->name();
        service_info.s_methodMap.insert(std::make_pair(method_name, pmethodDesc));

        std::cout << "method_name: " << method_name << std::endl;
    }

    service_info.s_service = service;
    m_serviceMap.insert(std::make_pair(service_name, service_info));
}

void RpcProvider::Run(){
    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    muduo::net::InetAddress address(ip,port);

    //创建TcpServer 对象
    muduo::net::TcpServer server(&m_eventLoop,address,"RpcProvider");
    // 绑定连接回调和消息读写回调方法
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection,this,std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage, this, 
                                std::placeholders::_1, 
                                std::placeholders::_2,
                                std::placeholders::_3));
    
    server.setThreadNum(4);

    std::cout << "RpcProvider start service at ip:" << ip <<" port:" << port << std::endl;

    //启动网络服务
    server.start();
    m_eventLoop.loop();
}

void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr& conn){

}

void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr&,
                            muduo::net::Buffer*,
                            muduo::Timestamp){
    
}

}