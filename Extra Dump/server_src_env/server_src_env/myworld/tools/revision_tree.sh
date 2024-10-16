#!/bin/bash

usage()
{
	echo "revision_tree.sh: 分支版本树"
	echo
	echo "Usage:"
	echo "    revision_tree.sh tag_patches"
	echo
	echo "example:"
	echo "    revision_tree.sh 0_0_22"
	exit 2
}

[[ $# != 1 ]] && usage

tag_version="tag_"${1}
echo "传入参数  "$tag_version
echo ""

cd /usr/local/test/myworld/

old_tag_version=`cvs log Makefile|egrep ${tag_version}`
echo $old_tag_version
if [ "$old_tag_version" == "" ];then
	echo "新的版本基线 暂时不处理 "$tag_version
	echo ""
	#echo "把工作目录的文件恢复到最新的版本"
	#cvs update -A 
	#echo ""
	#echo "从当前最新版本创建分支"
	#cvs tag -b $tag_version
	#echo ""
	exit 0
else
	echo $tag_version" 这个分支版本已经存在了，更新到这个基线版本"
	cvs update -r $tag_version
fi

cd /usr/local/test/myworld/src/
./svrauto.sh build  ${1}


