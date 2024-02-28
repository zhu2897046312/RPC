
cmake_minimum_required(VERSION 3.0)
project(RPC)

# 设置项目可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 设置编译头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/example)

# 设置库文件输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# 设置项目库文件搜索路径
link_directories(${PROJECT_SOURCE_DIR}/lib)

# 查找protobuf库
find_package(protobuf REQUIRED)
# 链接protobuf库
target_link_libraries(my_executable ${Protobuf_LIBRARIES})

#add_subdirectory(src)
add_subdirectory(example)