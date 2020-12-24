SET(EXECUTABLE_OUTPUT_PATH ./../bin)       #设置可执行文件的输出目录

set(CMAKE_BUILD_TYPE "Debug")

add_compile_options(-std=gnu99)

#Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
add_definitions(-g)


include_directories(../3rd/lua/src)
LINK_DIRECTORIES(../3rd/lua/src)

project (luaserver)


# 指定生成目标
add_executable(luaserver   main.c )
TARGET_LINK_LIBRARIES(luaserver lua )
TARGET_LINK_LIBRARIES(luaserver m )
TARGET_LINK_LIBRARIES(luaserver dl )
