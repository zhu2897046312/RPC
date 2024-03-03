#pragma once

#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

namespace fst
{
/**
* 1. rpc请求数据组装 -- 数据的序列化
* 2. 发送rpc请求 -- wait()
* 3. 接收rpc响应
* 4. 响应的反序列化
*/
class MprpcChannel : public google::protobuf::RpcChannel
{
public:
    void CallMethod(const google::protobuf::MethodDescriptor* method,
                          google::protobuf::RpcController* controller, const google::protobuf::Message* request,
                          google::protobuf::Message* response, google::protobuf::Closure* done);
};

}
