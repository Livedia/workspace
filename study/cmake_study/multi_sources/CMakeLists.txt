# 最低版本信息
cmake_minimum_required(VERSION 2.8)

# 项目名
project(my_add)

# 可以按照下面方式编写，如果文件夹下源文件太多，这边会写很多文件，采用下面变量的方法
# add_executable(add main.cc add.cc)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

# 目标文件  ${变量名} 来使用定义的变量
add_executable(add ${DIR_SRCS})
