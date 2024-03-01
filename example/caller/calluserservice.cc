#include <google/protobuf/service.h>
#include <iostream>
#include "mprpcapplication.h"
#include "user.pb.h"
#include "mprpcchannel.h"

int main(int argc,char **argv){
    fst::MprpcApplication::Init(argc,argv);

    fixbug::UserServiceRpc_Stub stub(new fst::MprpcChannel);
    fixbug::LoginRequest request;
    request.set_name("zhangsan");
    request.set_pwd("1234");

    fixbug::LoginResponse response;

    stub.Login(nullptr,&request,&response,nullptr);

    if(0 == response.result().errcode()){
        std::cout << "rpc login response success:" << response.success() << std::endl;
    }else{
        std::cout << "rpc login response error:" << response.result().errmsg() << std::endl;
    }
    return 0;
}