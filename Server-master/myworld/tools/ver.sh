#!/bin/sh

if [ $# = 1 ];then
        ProjectName="myworld"
else
        echo "parameter errer!"
        echo "./versh version"
        exit -1
fi

ver=$(echo $1 | awk -F '_' '{ print $3 }')
newrev=$(echo \#define REV    $ver)
oldrev=$(cat ~/myworld/src/comm/version.h |grep REV)
sed ~/myworld/src/comm/version.h -i -e "s/$oldrev/$newrev/g"

ver=$(echo $1 | awk -F '_' '{ print $2 }')
newrev=$(echo \#define MINOR $ver)
oldrev=$(cat ~/myworld/src/comm/version.h |grep MINOR)
sed ~/myworld/src/comm/version.h -i -e "s/$oldrev/$newrev/g"

ver=$(echo $1 | awk -F '_' '{ print $1 }')
newrev=$(echo \#define MAJOR $ver)
oldrev=$(cat ~/myworld/src/comm/version.h |grep MAJOR)
sed ~/myworld/src/comm/version.h -i -e "s/$oldrev/$newrev/g"

time=`date +20%y%m%d`
newrev=$(echo \#define BUILD $time)
oldrev=$(cat ~/myworld/src/comm/version.h |grep BUILD)
sed ~/myworld/src/comm/version.h -i -e "s/$oldrev/$newrev/g"


#for ((i=1; i <= 3; i++))
#do
#	mid="echo $1 | awk -F '.' '{ print \$$i}'"
#	eval $mid
	
#done


