#! /bin/bash

add(){
  echo " "$1" + "$2" = $(($1+$2)) "
}

add 2 3



# 下面这个脚本可以`一次创建多个目录`，各目录通过命令行参数传入，脚本逐个测试各目录是否存在，如果目录不存在，首先打印信息然后试着创建该目录。
function is_directory() {
	DIR_NAME=$1
	if [ ! -d $DIR_NAME ]; then
		return 1
	else
		return 0
	fi
}

for DIR in "$@"; do
	if is_directory "$DIR"
	then :
	else
		echo "$DIR doesn't exist. Creating it now..."
    mkdir $DIR > /dev/null 2>&1
    if [ $? -ne 0 ]; then
      echo "Can't create directory $DIR"
      exit 1
    fi
  fi
done
