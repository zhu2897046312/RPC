#include <google/protobuf/service.h>
#include <iostream>
#include "mprpcapplication.h"
#include "mprpccontroller.h"
#include "user.pb.h"
#include "mprpcchannel.h"

int main(int argc,char **argv){
    fst::MprpcApplication::Init(argc,argv);

    fixbug::UserServiceRpc_Stub stub(new fst::MprpcChannel);
    fixbug::LoginRequest request;
    request.set_name("zhangsan");
    request.set_pwd("1234");

    fixbug::LoginResponse response;
    fst::MprpcConTroller controller;
    stub.Login(&controller,&request,&response,nullptr);

    if(controller.Failed()){
        std::cout << controller.ErrorText() << std::endl;
    }else{
        if(0 == response.result().errcode()){
            std::cout << "rpc login response success:" << response.success() << std::endl;
        }else{
            std::cout << "rpc login response error:" << response.result().errmsg() << std::endl;
        }
    }

    fixbug::REgisterRequst req;
    req.set_id(200);
    req.set_name("dfjal");
    req.set_pwd("dkfhja");
    fixbug::REgisterResponse rsp;
    stub.Register(&controller,&req,&rsp,nullptr);
    if(controller.Failed()){
        std::cout << controller.ErrorText() << std::endl;
    }else{
        if(0 == rsp.result().errcode()){
            std::cout << "rpc Register response success:" << rsp.success() << std::endl;
        }else{
            std::cout << "rpc Register response error:" << rsp.result().errmsg() << std::endl;
        }
    }
    return 0;
}