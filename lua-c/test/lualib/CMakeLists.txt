SET(LIBRARY_OUTPUT_PATH ./../)             #设置库文件的输出目录
set(CMAKE_BUILD_TYPE "Debug")

add_compile_options(-std=gnu99)

#Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
add_definitions(-g)



include_directories(./../3rd/lua/src)

aux_source_directory(./ DIR_SRCS)

add_library(my_math SHARED  ${DIR_SRCS} )

set_target_properties(my_math PROPERTIES PREFIX "")