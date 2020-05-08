SET(EXECUTABLE_OUTPUT_PATH ./../../bin)       #设置可执行文件的输出目录
SET(LIBRARY_OUTPUT_PATH ./../../lib)             #设置库文件的输出目录
set(CMAKE_BUILD_TYPE "Debug")



add_compile_options(-std=gnu99)

#Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息

project (Demo1)

aux_source_directory(./../net/src DIR_SRCS)

aux_source_directory(./../common COMM_SRCS)

# 指定生成目标
add_executable(server main.c ${DIR_SRCS} ${COMM_SRCS} )

