cmake_minimum_required(VERSION 2.8)

project(main)

# 添加子目录，将子目录中的 CMakeLists.txt 文件包含到当前构建中
add_subdirectory(add)

# 添加目标
add_executable(main src/main.cc)

# 链接静态库
target_link_libraries(main my_add)
