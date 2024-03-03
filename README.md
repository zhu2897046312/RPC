# RPC

#### 依赖库
```
1. protobuf

```

#### BUG
```
1.在zookeeperutil.cc的Start()中使用Logger会导致客户端在接收到响应后仍然不退出
```

#### 配置
```
1. key=value   --- key 与 value 中不能有空格
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


