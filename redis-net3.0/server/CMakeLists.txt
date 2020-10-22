SET(EXECUTABLE_OUTPUT_PATH ./../bin)       #设置可执行文件的输出目录
SET(LIBRARY_OUTPUT_PATH ./../lib)             #设置库文件的输出目录
set(CMAKE_BUILD_TYPE "Debug")

add_compile_options(-std=gnu99)

#Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
add_definitions(-g)

project (server)


aux_source_directory(./../anet/src DIR_SRCS)
aux_source_directory(./../common/src COMMON_SRCS)

# 指定生成目标
add_executable(server  ${COMMON_SRCS} ${DIR_SRCS} main.c )

