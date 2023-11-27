# CMake 最低版本号要求
cmake_minimum_required(VERSION 2.8)

# 项目信息
project(Demo4)

# 加入一个配置头文件，用于处理 CMake 对源码的设置
configure_file(
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_SOURCE_DIR}/config.h"
)

# ${PROJECT_SOURCE_DIR} 表示当前项目源代码目录的绝对路径，即正在执行 CMakeLists.txt 文件的目录
# ${PROJECT_BINARY_DIR} 表示当前项目构建目录的绝对路径，即执行 cmake 命令的文件目录

# 是否使用自己的 MathFunctions 库
option(USE_MYMATH
  "Use provided math implementation" ON)

# 是否加入 MathFunctions 库
if(USE_MYMATH)
  include_directories("${PROJECT_SOURCE_DIR}/math")
  add_subdirectory(math)
  set(EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)
endif(USE_MYMATH)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

# 指定生成目标
add_executable(Demo ${DIR_SRCS})
target_link_libraries(Demo ${EXTRA_LIBS})
