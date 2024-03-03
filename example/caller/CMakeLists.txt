
set(SRC calluserservice.cc ../user.pb.cc)
#set(SRC callfriendservice.cc ../friend.pb.cc)
add_executable(consumer ${SRC})

target_link_libraries(consumer mprpc protobuf)
