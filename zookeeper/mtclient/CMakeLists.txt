SET(EXECUTABLE_OUTPUT_PATH ../bin)       #设置可执行文件的输出目录
SET(LIBRARY_OUTPUT_PATH ./lib)             #设置库文件的输出目录
set(CMAKE_BUILD_TYPE "Debug")

add_compile_options(-std=gnu99)

#Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
add_definitions(-g)
add_definitions(-DTHREAD)

link_directories(../lib)
include_directories(../common/zookeeper)

project (mtclient)

# 指定生成目标
add_executable(mtclient   client.c  zkcli.c)
target_link_libraries(mtclient pthread)
target_link_libraries(mtclient zookeeper_mt)
target_link_libraries(mtclient m)