#! /bin/bash

echo print env:
# 打印当前环境变量，内容太多注释掉了
#printenv
# echo 空内容输出空行
echo
echo

# 定义本地变量(等号左右无空格)
a=10
b=20
echo a=$a b=${b}
# 算数代换，使用：$(()) 或者 $[]
a="$((a+2))"
b="$[b+3]"
echo after change: a="$a" b="${b}"

# 获取命令执行结果
files=`ls`
echo files are: $files
path=$(pwd)
echo "path are: $path"
result=$(ls -l)
   echo "result  $result"
echo "Today is $(date)"


# 定义环境变量
export ENV_A=30
export b

echo ENV_a=$ENV_A b=$b

echo print env:
#printenv
echo
echo

# 删除变量
unset a
unset ENV_A

echo after unset ENV_a=$ENV_A a=$a
