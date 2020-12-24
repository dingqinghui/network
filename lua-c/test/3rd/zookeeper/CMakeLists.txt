SET(LIBRARY_OUTPUT_PATH ./../../)             #设置库文件的输出目录
set(CMAKE_BUILD_TYPE "Debug")

add_compile_options(-std=gnu99)

#Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
add_definitions(-g)


#库目录
link_directories(./)
#文件目录
include_directories(./)
include_directories(./../lua/src)

#加入编译的文件目录
aux_source_directory(./ DIR_SRCS)
#生成库
add_library(zookeeper SHARED  ${DIR_SRCS} )
#连接库
target_link_libraries(zookeeper zookeeper_st)
target_link_libraries(zookeeper m)
#设置目标生成属性
set_target_properties(zookeeper PROPERTIES PREFIX "")