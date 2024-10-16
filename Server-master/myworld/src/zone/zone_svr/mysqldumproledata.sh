#!/bin/bash
# //  这是个极端优秀的脚本,需要大家共同来维护和添加相关的case
# //  伟大的人,做伟大的事,  信赖 潜力蛙  
# //  create date :  20110812
# //  auth        :  ouyang
# //  modify date :
# //  modify auth :

usage()
{
	echo "./mysqldumproledata.sh mysql_user mysql_passwd db_account table_account db_world table_world roleid Pass9Account"
	echo "./mysqldumproledata.sh mmog mmog account_3 world_8  18372447909670168119 ouyang"
	exit 2
}

[[ $# != 6 ]] && usage

mysql_user=${1}
mysql_passwd=${2}
db_account=${3}
db_world=${4}
roleid=${5}
Pass9Account=${6}

echo "mysqldump -u$mysql_user -p$mysql_passwd $db_account Account  -c -t --where \"Pass9Account='$Pass9Account'\" > $roleid.sql" |sh

echo "mysqldump -u$mysql_user -p$mysql_passwd $db_account AccountRole  -c -t --where \"RoleWID=$roleid\" >> $roleid.sql" |sh

echo "mysqldump -u$mysql_user -p$mysql_passwd $db_world RoleData  -c -t --where \"RoleID=$roleid\" >> $roleid.sql"|sh

echo "mysqldump -u$mysql_user -p$mysql_passwd $db_world NameID  -c -t --where \"GID=$roleid\" >> $roleid.sql"|sh

