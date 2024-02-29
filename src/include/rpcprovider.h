#pragma once

#include "google/protobuf/service.h"

#include <google/protobuf/descriptor.h>
#include <memory>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <unordered_map>

namespace fst {

class RpcProvider
{
public:
    //框架提供给外部使用的，，可以发布rpc方法的函数接口
    void NotifyService(google::protobuf::Service *service);

    //启动rpc节点，开始提供rpc远程网络调用服务
    void Run();
private:
    void OnConnection(const muduo::net::TcpConnectionPtr&);
    void OnMessage(const muduo::net::TcpConnectionPtr&,
                            muduo::net::Buffer*,
                            muduo::Timestamp);
private:
    struct ServiceInfo{
        google::protobuf::Service  *s_service;  //服务对象
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> s_methodMap; //服务方法信息
    };

    //TcpServer
    std::unique_ptr<muduo::net::TcpServer> m_tcpServerPtr;
    //EventLoop
    muduo::net::EventLoop m_eventLoop;
    std::unordered_map<std::string, ServiceInfo> m_serviceMap;  //服务对象的所有信息


};

}