#pragma once

#include "google/protobuf/service.h"


namespace fst {

class RpcProvider
{
public:
    //框架提供给外部使用的，，可以发布rpc方法的函数接口
    void NotifyService(google::protobuf::Service *service);

    //启动rpc节点，开始提供rpc远程网络调用服务
    void Run();
};

}