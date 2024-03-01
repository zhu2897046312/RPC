
#include <cstdint>
#include <string>

#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

class UserService : public fixbug::UserServiceRpc   //rpc服务提供者
{
public:
    bool Login(std::string& name,std::string& pwd){
        std::cout << name << std::endl;
        std::cout << name << std::endl;
        return true;
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

    bool Register(uint32_t id , std::string& name, std::string& pwd){
        std::cout << id << std::endl;
        std::cout << name << std::endl;
        std::cout << name << std::endl;
        return true;
    }

    void Register(::google::protobuf::RpcController* controller,
                       const ::fixbug::REgisterRequst* request,
                       ::fixbug::REgisterResponse* response,
                       ::google::protobuf::Closure* done){
        uint32_t id = request->id();
        auto name = request->name();
        auto pwd = request->pwd();

        bool register_result = Register(id,name,pwd);

        fixbug::ResultCode* code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(register_result);

        done->Run();

    }
};

int main(int argc,char **argv){

    // 框架初始化操作
    fst::MprpcApplication::Init(argc,argv);

    //把Userervice对象发布到rpc节点上
    fst::RpcProvider provider;
    provider.NotifyService(new UserService());

    //启动一个rpc服务发布节点
    provider.Run();


    return 0;
}