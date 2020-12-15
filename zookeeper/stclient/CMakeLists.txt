SET(EXECUTABLE_OUTPUT_PATH ../bin)       #设置可执行文件的输出目录
SET(LIBRARY_OUTPUT_PATH ./lib)             #设置库文件的输出目录
set(CMAKE_BUILD_TYPE "Debug")

add_compile_options(-std=gnu99)

#Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
add_definitions(-g)


link_directories(../lib)
include_directories(../common/zookeeper)

project (stclient)

# 指定生成目标
add_executable(stclient   client.c  zkcli.c)
target_link_libraries(stclient zookeeper_st)
target_link_libraries(stclient m)