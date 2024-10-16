#!/bin/sh

#~/myworld/tools/create_dbtab_sql.sh;

if [ $# != 2 ];then
	echo "parameter errer!"
	echo "./create_roledb.sh RegionID WorldID"
	exit -1;
fi

worldid=world_$1_$2

#echo "$worldid"
#exit -1

mysql -ummog -pmmog << EOF
        create database ${worldid};
        QUIT
EOF


mysql -ummog -pmmog ${worldid}<MailEntry.sql;
mysql -ummog -pmmog ${worldid}<create_role_db.sql;
mysql -ummog -pmmog ${worldid}<create_clan.sql;
mysql -ummog -pmmog ${worldid}<create_global.sql;
mysql -ummog -pmmog ${worldid}<create_nameid_db.sql;
mysql -ummog -pmmog ${worldid}<create_relation.sql;
mysql -ummog -pmmog ${worldid}<create_shadow_role_db.sql;
mysql -ummog -pmmog ${worldid}<create_role_drop.sql;
mysql -ummog -pmmog ${worldid}<create_acc_share.sql;
mysql -ummog -pmmog ${worldid}<create_GMIPMacLimit.sql;
