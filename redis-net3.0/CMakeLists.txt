

add_compile_options(-std=gnu99)

#Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)

add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(./anet/src)



