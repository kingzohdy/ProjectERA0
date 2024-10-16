#!/bin/sh

#./create_log_sql.sh

if [ $# != 1 ];then
        echo "parameter errer!"
        echo "./roledata.sh num(num is worldID)"
        exit -1;
fi


worldid=world_$1

#mysql -ummog -pmmog world_5 <create_role_db.sql
mysql -ummog -pmmog $worldid << EOF
insert into RoleData_0 select * from RoleData_1;
insert into RoleData_0 select * from RoleData_2;
insert into RoleData_0 select * from RoleData_3;
insert into RoleData_0 select * from RoleData_4;
insert into RoleData_0 select * from RoleData_5;
insert into RoleData_0 select * from RoleData_6;

ALTER TABLE RoleData_0 RENAME TO RoleData;

drop table RoleData_1;
drop table RoleData_2;
drop table RoleData_3;
drop table RoleData_4;
drop table RoleData_5;
drop table RoleData_6;
EOF

