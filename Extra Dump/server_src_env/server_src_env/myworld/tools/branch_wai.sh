#!/bin/sh

if [ $# = 2 ];then
	ProjectName="myworld"
else
	echo "parameter errer!"
	exit -1
fi

cd /usr/local/test/myworld
cvs up

echo "start cvs tag tag_${2}_deploy_01 ..."
cvs tag tag_${2}_deploy_01
echo "start cvs rtag -b -r tag_${2}_deploy_01 tag_${2}_wai $ProjectName ..."
cvs rtag -b -r tag_${2}_deploy_01 tag_${2}_wai $ProjectName
	




