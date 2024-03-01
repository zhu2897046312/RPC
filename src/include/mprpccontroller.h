#pragma once

#include <google/protobuf/service.h>
#include <google/protobuf/stubs/callback.h>
#include <string>

namespace fst {

class MprpcConTroller : public google::protobuf::RpcController
{
public:
    MprpcConTroller();

    void Reset();
    bool Failed() const;
    std::string ErrorText() const;
    void SetFailed(const std::string& reason);

    void StartCancel(){}
    bool IsCanceled() const { return true; }
    void NotifyOnCancel(google::protobuf::Closure* callback){}

private:
    bool m_failed;  //RPC方法执行过程中的状态
    std::string m_errText; //RPC方法执行过程中的错误信息
};

}