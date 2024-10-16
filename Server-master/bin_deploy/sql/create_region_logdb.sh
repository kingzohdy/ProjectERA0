#!/bin/sh

#~/myworld/tools/create_dbtab_sql.sh;

if [ $# != 1 ];then
	echo "parameter errer!"
	echo "./create_roledb.sh RegionID"
	exit -1;
fi

Regionid=region_log_$1

#echo "$worldid"
#exit -1

mysql -ummog -pmmog << EOF
        create database ${Regionid};
        QUIT
EOF


#mysql -ummog -pmmog ${Regionid}<LogTable.sql
