SET(EXECUTABLE_OUTPUT_PATH ./bin)       #设置可执行文件的输出目录

set(CMAKE_BUILD_TYPE "Debug")

add_compile_options(-std=gnu99)

#Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
add_definitions(-g)


project (test)


# 指定生成目标
add_executable(test   main.c ../net_daemon.c )

