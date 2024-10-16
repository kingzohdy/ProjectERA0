#!/bin/sh

#~/myworld/tools/create_dbtab_sql.sh;

if [ $# != 2 ];then
	echo "parameter errer!"
	echo "./create_roledb.sh RegionID WorldID"
	exit -1;
fi

worldid=world_log_$1_$2

#echo "$worldid"
#exit -1

mysql -ummog -pmmog << EOF
        create database ${worldid};
        QUIT
EOF


#mysql -ummog -pmmog ${worldid}<LogTable.sql
