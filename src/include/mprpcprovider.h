#pragma once

#include "google/protobuf/service.h"

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <memory>
#include <muduo/net/Callbacks.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <unordered_map>

namespace fst {

class MprpcProvider
{
public:
    //框架提供给外部使用的，，可以发布rpc方法的函数接口
    /**
    *   发布RPC服务
    *   1. 获取服务对象的描述信息 --- protobuf作为中间件 
    *       (1) 获取服务的名字
    *       (2) 获取服务对象servcice的方法的数量
    *   2. 获取服务对象指定下标的服务方法的描述
    *   3. 写入内存中
    */
    void NotifyService(google::protobuf::Service *service);

    //启动rpc节点，开始提供rpc远程网络调用服务
    /**
    *   启动RPC服务
    *   1. 获取RPC服务 address 
    *   2. 创建TCP连接          ---- muduo库
    *   3.把当前RPC节点要发布的服务全部注册到zk注册中心，让client可以发现服务
    *   4.启动网络服务
    */
    void Run();
private:
    void onConnection(const muduo::net::TcpConnectionPtr&);
    void onMessage(const muduo::net::TcpConnectionPtr&,
                            muduo::net::Buffer*,
                            muduo::Timestamp);
    //Closure回调
    void sendRpcResponse(const muduo::net::TcpConnectionPtr&,google::protobuf::Message*);
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
