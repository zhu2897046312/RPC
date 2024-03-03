# RPC

# 依赖库
```
1. protobuf
2. zookeeper_mt
3. muduo
```

# BUG
```
在zookeeperutil.cc的Start()中使用Logger会导致客户端在接收到响应后仍然不退出
```

## 项目任务
```
任务一: 实现mprpc应用框架初始化
        到src/include/mprpcapplication.h src/include/mprpcconfig.h 实现相应方法

任务二: 实现RPC服务发布      --- RPCServer
        到 src/include/mprpcprovider.h 实现相应方法

任务三: 实现RPC服务Client
        到 src/include/mprpcchannel.h src/include/mprpccontroller.h实现相应方法

任务四: 实现RPC框架日志系统
        到 src/include/logger.h src/include/lockqueue.h 实现相应方法

任务五: 实现zookeeper注册中心
        到 src/include/zookeeperutil.h 实现相应方法
```

## 项目所需技术
```
1. muduo网络库
2. zookeeper分布式协调服务
3. protobuf格式库
```

### example 

```
1. 使用.proto文件注册rpc远程方法
2. 编译proto文件生成.pb.h \ .pb.cc文件
3. 要注册rpc远程方法的类，需要继承其服务类
4. 实现本地方法业务,重载其服务类中的方法
5. 重载该方法时的操作:
    (1)反序列化远程数据
    (2)调用本地方法
    (3)序列化响应数据包并发送回去
    (4)done->Run()
```


