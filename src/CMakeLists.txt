aux_source_directory(./ SRC_LIST)
add_library(mprpc ${SRC_LIST})
target_include_directories(mprpc PUBLIC ./include)
#muduo_net 依赖 muduo_base
target_link_libraries(mprpc muduo_net muduo_base pthread zookeeper_mt)