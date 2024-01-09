#! /bin/bash

# 最上面的是 shebang 指定解释器路径
echo "Study shell"

shval="shval"
# 当前 shell 调用脚本：
# . xxx.sh   # 试了一下没运行成功
# sourch xxx.sh

# 创建一个新的子 shell 来执行脚本
# ./xxx.sh
# sh xxx.sh
# bash xxx.sh


# 特殊字符含义
# $#   传递到脚本的参数个数
# $*   以一个单字符串显示所有向脚本传递的参数。 如"$*"用「"」括起来的情况、以"$1 $2 … $n"的形式输出所有参数，1 个。
# $@   与$*相同，但是使用时加引号，并在引号中返回每个参数。 如"$@"用「"」括起来的情况、以"$1" "$2" … "$n" 的形式输出所有参数，n 个。
# $$   脚本运行的当前进程ID号
# $?   显示最后命令的退出状态。0表示没有错误，其他任何值表明有错误。
# $-   显示Shell使用的当前选项，与[set命令](https://www.runoob.com/linux/linux-comm-set.html)功能相同。
# $n   n 表示数字，表示启动脚本时后面的参数 $1 第一个参数 $2 第二个参数...

# 判断语句
# 使用 test
# -e 表示判断文件是否存在
test -e shell.md && echo "shell.md is exist!" || echo "shell.md not exist!"

# test 常用参数含义：
# -e 文件名是否存在
# -f 文件名是否存在且为文件类型
# -d 文件名是否存在且为目录

# -eq 相等
# -ne 不等
# -gt 左大于右
# -lt 左小于右
# -ge 左大于等于右
# -le 左小于等于右

# -z 判断其后字符串是否为 0，为 0 返回 TRUE
# -n 判断其后字符串是否非 0，非 0 返回 TRUE

# -a 后面加其它判断，表示与 and
# -r 后面加其它判断，表示或 or
# ！ 取反

# 使用判断符号 [ ]
# 使用中括号和 test 基本一样，中括号两边都要有空格，注意空格呀！

# if [  ]

if test $# -eq 0 ; then
  echo "no param!"
else
  echo "have param, param size is: $#"
fi

if [ $# -gt 0 ]; then
  if [ $1 == 1 ] ; then
    echo "param is $1"
  elif [ $1 == 2 ]; then
    echo "param is not 1, is: $1"
  else
    echo "param is not 1 and 2"
  fi
else
  echo "param is 0"
fi


# case

case $1 in
  "1")
      echo "case is 1"
      ;;
  "2")
      echo "case is 2"
      ;;
  "test")
      echo "case is test"
      ;;
esac


# 循环

# while 循环
while_max=5
while [ $while_max -gt 0 ]
do
  echo "while loop max is $while_max"
  while_max=$(($while_max-1))
done

# until 循环
until_max=5
until [ $until_max -lt 0 ]
do
  echo "until loop max is $until_max"
  until_max=$(($until_max-1))
done

# for 循环
for it in 1 2 3 4 5
do
  echo "for loop it is:$it"
done

vars="6 7 8 9"
for it in $vars
do
  echo "for loop it is:$it"
done

for((i=0;i<=10;i=i+1))
do
  echo "for((i=0;i<=10;i=i+1)) i=$i"
done



# function 表示函数，也可以不写 function 标识
function myname()
{
  read -p "Please input your first name: " firstname # 提示使用者输入 -p 表示打印提示信息
  read -p "Please input your last name: " lastname
  echo -e "\n Your full name is: ${firstname} ${lastname}"  # echo -e 表示可以打印转义字符，\n 换行可以显示出来了
}
# 直接使用函数名进行函数调用
# myname
# 函数，也可以不写 function 标识，但是一定要有（）
add()
{
  # 进行计算的时候用 $((计算式子)) 来计算结果，注意只支持整数的运算
  echo "$1 + $2 =  $(($1+$2))"

  a=10
  echo "before a=$a"
  a=$(($a+10))
  echo "after a=$a"
}
# 函数调用实参直接写在后面就行了，函数内部使用 $1 $2 ... 来使用传入的参数
add 1 2

# 涉及到小数的计算需要借助其它指令来实现
add_folat()
{
  a=1.234
  b=2.345

  # echo "use $((formela)): $a + $b =$(($a + $b))"  # 会报错
  echo "$a+$b = `echo "$a+$b" | bc` "
  echo "$a+$b" | bc
}

add_folat

echo "End of study shell"
