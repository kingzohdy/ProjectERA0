#!/bin/sh

hlp()
{
	echo "parameter errer!"
        echo "./meta2tab.sh TableName [Alter Vesion]"
        exit -1;
}


#main

TableName=$1
Version=

if [ $# == 0 ];then
	hlp;
	exit -1;
fi

if [ $# == 2 ];then
	Version="--alter_table=$2";
fi

~/myworld/comm_release/tools/meta2tab  --charset latin1 --meta_file ~/myworld/tools/resdb_meta.tdr --meta_name ${TableName} ${Version} -o 1.sql
