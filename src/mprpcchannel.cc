#include "mprpcchannel.h"

#include "rpcheader.pb.h"
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "mprpcapplication.h"
#include "logger.h"
#include "zookeeperutil.h"
namespace fst
{
/**
* 序列化格式： header_size + service_name   method args_size + args
*/
void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                          google::protobuf::RpcController* controller, const google::protobuf::Message* request,
                          google::protobuf::Message* response, google::protobuf::Closure* done){
    auto sd = method->service();
    std::string service_name = sd->name();
    std::string method_name = method->name();

    //获取参数的序列化字符串长度 args_size
    uint32_t args_size = 0;
    std::string args_str;
	//【不同】SerializePartialToString --- SerializeToString
    if(request->SerializeToString(&args_str)){
        args_size = args_str.size();
    }else{
        //std::cout << "serialize request error!" << std::endl;
        controller->SetFailed("serialize request error!");
        return;
    }

    //定义rpc请求header
    mprpc::RpcHeader rpcHeader;
    rpcHeader.set_service_name(service_name);
    rpcHeader.set_method_name(method_name);
    rpcHeader.set_args_size(args_size);

    std::string rpc_header_str;
    uint32_t header_size;
	//不同  SerializePartialToString(  SerializeToString(
    if(rpcHeader.SerializeToString(&rpc_header_str)){
        header_size = rpc_header_str.size();
    }else{
        //std::cout << "serialize rpc_header_str error!" << std::endl;
        controller->SetFailed("serialize rpc_header_str error!");
        return;
    }

    //组织带发送的rpc请求字符串
    std::string send_rpc_str;
    send_rpc_str.insert(0, std::string((char*)&header_size, 4));
    send_rpc_str += rpc_header_str;
    send_rpc_str += args_str;

    std::cout << "==============================" << std::endl;
    std::cout << "rpc_header_str: " << rpc_header_str << std::endl;
    std::cout << "header_size: " << header_size << std::endl;
    std::cout << "service_name: " << service_name << std::endl;
    std::cout << "method_name: " << method_name << std::endl;
    std::cout << "args_str: " << args_str << std::endl;
    std::cout << "==============================" << std::endl;

    // std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    // uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    std::string ip;
    uint16_t port = 0;
    {
    ZkClient zk_client;
    zk_client.Start();
    std::string method_path = "/" + service_name + "/" + method_name;
    std::string host_data = zk_client.GetDate(method_path.c_str());
    std::cout << method_path << "---------" << host_data << std::endl;
    if(host_data == ""){
        controller->SetFailed(method_path + " is not exist! ");
        return;
    }
    int idx = host_data.find(":");
    if(-1 == idx){
        controller->SetFailed(method_path + " address is valid!");
        return;
    }
    ip = host_data.substr(0,idx);
    port = atoi(host_data.substr(idx + 1, host_data.size() - idx).c_str());
    }

    std::cout << "ip:" << ip << port << std::endl;
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == client_fd){
        std::cout << "create socket error ! errno: " << errno << std::endl;
        // LOG_ERROR("create socket error ! errno: %s", std::to_string(errno).c_str());
        controller->SetFailed("create socket error ! errno: " + std::to_string(errno));
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    //创建连接
    if(-1 == connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))){
        //std::cout << "connect error! errno: " << errno << std::endl;
        close(client_fd);
        controller->SetFailed("connect error! errno: " + std::to_string(errno));
        return;
    }
    //发送请求
    if(-1 == send(client_fd,send_rpc_str.c_str(), send_rpc_str.size(),0)){
        //std::cout << "send error ! error: " << errno << std::endl;
        controller->SetFailed("send error ! error: " + std::to_string(errno));
        close(client_fd);
        return;
    }

    char recv_buf[1024] = {0};
    int recv_size = 0;
    if(-1 == (recv_size = recv(client_fd,recv_buf,1024,0))){
        //std::cout << "recv error ! error: " << errno << std::endl;
        controller->SetFailed("recv error ! error: " + std::to_string(errno));
        close(client_fd);
        return;
    }

    //反序列化response  不同
    //std::string response_str(recv_buf,recv_size);
    if(!response->ParseFromArray(recv_buf,recv_size)){
        //std::cout << "parse error! response_str: " << response_str << std::endl;
        close(client_fd);
        char errtxt[2048] = {0};
        snprintf(errtxt, sizeof(errtxt),"parse error! response_str: %s", recv_buf);
        controller->SetFailed(errtxt);
        return;
    }
    //std::cout <<" mprpcchannel" <<response_str << std::endl;
    close(client_fd);
}

}
