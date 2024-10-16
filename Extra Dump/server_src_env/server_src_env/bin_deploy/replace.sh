#!/bin/sh


if [ $# != 3 ];then
	echo "parameter errer!"
	echo "use:./replace.sh dir old_text new_text"
	exit -1
fi

ReplaceAll=$(find $1/ -name \* |xargs grep "$2" |awk -F ':' '{print $1}')

#echo "$ReplaceAll"

for i in $ReplaceAll ;do
	sed $i -i -e "s/$2/$3/g"
done
