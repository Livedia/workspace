#! /bin/bash

for FRUIT in apple banana pear; do
	echo "I like $FRUIT"
done


for TEST in `ls`
do
   ls -l $TEST
done



count=0
echo "Enter password:"
read PASSWORD
while [ "$PASSWORD" != "secret" -a $count -lt 10 ]; do
  count=$((count+1))
	echo "Sorry, password error, please try again, worng password times is: $count "
  if [ $count -eq 10 ];then
  echo "too many worng pasword, exit"
  exit
  fi
	read PASSWORD
done
