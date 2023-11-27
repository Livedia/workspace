#! /bin/bash

# shell 脚本运行有 4 种方式
# (1) ./run.sh
# (2) /bin/bash run.sh
# (3) source run.sh
# (4) . run.sh
# (1)和(2) 会调用子 shell 运行，运行完后还在当前目录，(3)和(4)是使用当前 shell 运行，运行完后会进入上一级目录

cd ..
ls
