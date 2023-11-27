# 最低版本
cmake_minimum_required(VERSION 2.8)

# 项目名
project(ctest)

# 导入子目录
add_subdirectory(math)

# 添加目标文件
add_executable(demo src/main.cc)

# 链接到库
target_link_libraries(demo my_math)

#
#
# 启用测试
enable_testing()

# 测试程序是否成功运行
add_test(test_run demo 5 2)

# 测试帮助信息是否可以正常提示
add_test(test_usage demo)

# 使用正则匹配来校验输出
set_tests_properties(test_usage
  PROPERTIES PASS_REGULAR_EXPRESSION "Usage: .* base exponent")

# 测试 5 的平方
add_test(test_5_2 demo 5 2)
set_tests_properties(test_5_2
  PROPERTIES PASS_REGULAR_EXPRESSION "= 25")

# 测试 10 的 5 次方
add_test(test_10_5 demo 10 5)
set_tests_properties(test_10_5
  PROPERTIES PASS_REGULAR_EXPRESSION "= 100000")

# 测试 2 的 10 次方
add_test(test_2_10 demo 2 10)
set_tests_properties(test_2_10
  PROPERTIES PASS_REGULAR_EXPRESSION "= 1024")

# 使用宏来减少重复代码
# 定义一个宏，用来简化测试工作
macro(do_test arg1 arg2 result)
  add_test(test_${arg1}_${arg2} demo ${arg1} ${arg2})
  set_tests_properties(test_${arg1}_${arg2}
    PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endmacro(do_test)

# 使用该宏进行一系列的数据测试
do_test(6 2 "= 36")
do_test(10 3 "= 1000")
do_test(2 9 "= 512")
