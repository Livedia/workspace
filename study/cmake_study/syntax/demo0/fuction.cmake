# 定义了一个名为MyFunction的函数，参数为FirstArg
function(MyFunction FirstArg)
  # 变量 CMAKE_CURRENT_FUNCTION 为函数的官方变量，保存了当前的函数名
  message("Function: ${CMAKE_CURRENT_FUNCTION}")

  # 变量  CMAKE_CURRENT_FUNCTION_LIST_FILE 为函数的官方变量，保存了当前 cmake 文件的路径
  message("File: ${CMAKE_CURRENT_FUNCTION_LIST_FILE}")
  message("FirstArg: ${FirstArg}")
  set(FirstArg "new value")
  message("FirstArg again: ${FirstArg}")

  # 变量 ARGC 表示参数个数， ARGV 为参数列表
  message("ARGV0: ${ARGV0} ARGV1: ${ARGV1} ARGC: ${ARGC}")
endfunction()

set(FirstArg "first value")

# 调用函数，并传参个数比函数声明时的多了一个
MyFunction("Value1" "Value2")
message("FirstArg in global scope: ${FirstArg}")
