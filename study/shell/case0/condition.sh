#! /bin/bash

if [ -f sample.sh ]
then
	echo 'sample.sh is a file'
else
	echo 'sample.sh is NOT a file'
fi

if :
then
	echo 'always true'
fi



echo "Is it morning? Please answer yes or no."
read YES_OR_NO
if [ "$YES_OR_NO" = "yes" ]; then
        echo "Good morning!"
elif [ "$YES_OR_NO" = "no" ]; then
        echo "Good afternoon!"
else
        echo "Sorry, $YES_OR_NO not recognized. Enter yes or no."
fi

# case
echo "Is it morning? Please answer yes or no."
read YES_OR_NO
case "$YES_OR_NO" in
yes|y|Yes|YES)
        echo "Good morning!"
        ;;
[nN]*)
        echo "Good afternoon!"
        ;;
*)
        echo "Sorry, $YES_OR_NO not recognized. Enter yes or no."
esac
