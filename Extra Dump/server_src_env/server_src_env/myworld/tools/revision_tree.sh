#!/bin/bash

usage()
{
	echo "revision_tree.sh: ��֧�汾��"
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
echo "�������  "$tag_version
echo ""

cd /usr/local/test/myworld/

old_tag_version=`cvs log Makefile|egrep ${tag_version}`
echo $old_tag_version
if [ "$old_tag_version" == "" ];then
	echo "�µİ汾���� ��ʱ������ "$tag_version
	echo ""
	#echo "�ѹ���Ŀ¼���ļ��ָ������µİ汾"
	#cvs update -A 
	#echo ""
	#echo "�ӵ�ǰ���°汾������֧"
	#cvs tag -b $tag_version
	#echo ""
	exit 0
else
	echo $tag_version" �����֧�汾�Ѿ������ˣ����µ�������߰汾"
	cvs update -r $tag_version
fi

cd /usr/local/test/myworld/src/
./svrauto.sh build  ${1}


