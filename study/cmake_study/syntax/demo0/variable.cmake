message("--- 普通变量")
set(MyString1 "Text1")

# 变量名可以使用括号和引号还有空格
set([[My String2]] "Text2")
set("My String3" "Text3")

message(${MyString1})

# 包含空格的变量在使用的时候要加上转义 `\`
message("String2:" ${My\ String2})
message("String3:" ${My\ String3})

unset(MyString1)

message("--- 环境变量")

# //示例1：
set(ENV{CXX} "clang++")
unset(ENV{VERBOSE})

# //示例2：
set(ENV{CMAKE_PATH} "myown/path/example")

# 判断CMAKE_PATH环境变量是否定义
if(DEFINED ENV{CMAKE_PATH}) # 注意此处ENV前没有$符号
  message("CMAKE_PATH_1: $ENV{CMAKE_PATH}") # 注意此处ENV前有$符号
else()
  message("NOT DEFINED CMAKE_PATH VARIABLES")
endif()

# 示例3:
# 在配置期间打印myenv环境变量
message("generated with " $ENV{myenv})

# 在构建阶段过程中打印相同的变量
add_custom_target(EchoEnv ALL COMMAND echo "myenv in build is" $ENV{myenv})
