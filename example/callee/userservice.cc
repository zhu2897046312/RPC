#include <iostream>
#include <string>

#include "user.pb.h"

class UserService : public fixbug::UserServiceRpc   //rpc服务提供者
{
public:
    bool Login(std::string& name,std::string& pwd){
        return false;
    }
    /**
    * 1. caller -==> Login(LoginRequest) ==> muduo ==> callee
    * 2. callee ===> Login(LoginResponse) ==> 交到下面的方法 
    */
    void Login(::google::protobuf::RpcController* controller,
                       const ::fixbug::LoginRequest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done)
    {
        //框架给业务上报了请求参数LoginRequest，应用获取相应的书记作本地业务
        std::string name = request->name();
        std::string pwd = request->pwd();

        //作本地业务
        bool login_result = Login(name,pwd);  

        //把相应写入 包括错误码 、 错误信息 、 返回值
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(login_result);

        //执行回调
        done->Run();

    }
};

int main(){
    return 0;
}