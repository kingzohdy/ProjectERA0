#!/bin/sh

#~/myworld/tools/create_dbtab_sql.sh;

#if [ $# != 1 ];then
#	echo "parameter errer!"
#	echo "./create_roledb.sh num(num is worldID)"
#	exit -1;
#fi

#worldid=world_$1

#echo "$worldid"
#exit -1

TableName="Online"

mysql -ummog -pmmog << EOF
        create database $TableName;
        QUIT
EOF


mysql -ummog -pmmog ${TableName}<create_online.sql;
#mysql -ummog -pmmog ${worldid}<create_role_db.sql;
#mysql -ummog -pmmog ${worldid}<create_clan.sql;
#mysql -ummog -pmmog ${worldid}<create_global.sql;
#mysql -ummog -pmmog ${worldid}<create_nameid_db.sql;
#mysql -ummog -pmmog ${worldid}<create_relation.sql;


