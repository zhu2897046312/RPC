
#include "rpcprovider.h"
#include "mprpcapplication.h"
#include <cstdint>
#include <cstdlib>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
#include <google/protobuf/stubs/callback.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpServer.h>
#include <functional>
#include <string>
#include <utility>
#include "rpcheader.pb.h"


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
    if(!conn->connected()){
        //连接断开了
        conn->shutdown();
    }
}
/**
* 记录serice_name method_name args_size
*/
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn,
                            muduo::net::Buffer* buffer,
                            muduo::Timestamp){
    //网络上接收远程的rpc调用请求字符流
    std::string recv_buf = buffer->retrieveAllAsString();

    //从字符流读取前4个字节的内容
    uint32_t header_size = 0;
    recv_buf.copy((char*)&header_size, 4,0);

    //根据header_size 读取数据头的原始字符流,反序列化数据,得到rpc请求的详细信息
    std::string rpc_header_str = recv_buf.substr(4,header_size);
    mprpc::RpcHeader rpcHeader;
    std::string service_name;
    std::string method_name;
    uint32_t args_size = 0;

    if (rpcHeader.ParseFromString(rpc_header_str)) {
        //反序列化成功
        service_name = rpcHeader.service_name();
        method_name = rpcHeader.method_name();
        args_size = rpcHeader.args_size();
    }else {
        std::cout << "rpc_header_str:" << rpc_header_str << "parse error!" << std::endl;
        return;
    }

    //获取rpc方法参数的字符流
    std::string args_str = recv_buf.substr(4 + header_size,args_size);

    std::cout << "==============================" << std::endl;
    std::cout << "rpc_header_str: " << rpc_header_str << std::endl;
    std::cout << "header_size: " << header_size << std::endl;
    std::cout << "service_name: " << service_name << std::endl;
    std::cout << "method_name: " << method_name << std::endl;
    std::cout << "args_str: " << method_name << std::endl;
    std::cout << "==============================" << std::endl;

    //获取service对象和method对象
    auto it = m_serviceMap.find(service_name);
    if(it == m_serviceMap.end()){
        std::cout << service_name << "is not exist!" << std::endl;
        return;
    }
    google::protobuf::Service* service = it->second.s_service;

    auto mit = it->second.s_methodMap.find(method_name);
    if(mit == it->second.s_methodMap.end()){
        std::cout << method_name << "is not exist!" << std::endl;
        return;
    }
    const google::protobuf::MethodDescriptor* method = mit->second;

    //生成rpc方法调用的请求request和响应response参数
    google::protobuf::Message*request = service->GetRequestPrototype(method).New();
    if(!request->ParseFromString(args_str)){
        std::cout << "requse parse error, content: " << args_str << std::endl;
        return;
    }
    google::protobuf::Message*response = service->GetResponsePrototype(method).New();

    //绑定ygclosure的回调函数
    auto done = google::protobuf::NewCallback<RpcProvider,
                                            const muduo::net::TcpConnectionPtr&,
                                            google::protobuf::Message*>
                                            (this,
                                            &RpcProvider::sendRpcResponse,
                                            conn,
                                            response);

    //在框架上根据远端rpc请求，调用当前rpc节点上发布的方法
    service->CallMethod(method,nullptr, request ,response, done);
}

void RpcProvider::sendRpcResponse(const muduo::net::TcpConnectionPtr& conn,google::protobuf::Message*  response){
    std::string response_str;
    if(response->SerializePartialToString(&response_str)){
        conn->send(response_str);
        conn->shutdown();
    }else{
        std::cout << "serialize sponse_str error !" << std::endl;
    }
    conn->shutdown();
}

}