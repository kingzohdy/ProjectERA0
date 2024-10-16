#!/bin/sh

if [ $# != 1 ];then
        echo "parameter errer!"
        echo "./create_regiondb.sh num(num is regionID)"
        exit -1;
fi

BillName="bill_$1"
AccountName="account_$1"
RegionName="region_$1"

mysql -ummog -pmmog << EOF
        create database $RegionName;
        QUIT
EOF

mysql -ummog -pmmog ${RegionName} <ShopTable.sql;

mysql -ummog -pmmog << EOF
        create database $BillName;
        QUIT
EOF

mysql -ummog -pmmog ${BillName}<create_bill_db.sql;
#mysql -ummog -pmmog ${BillName}<AccGift.sql;

mysql -ummog -pmmog << EOF
        create database $AccountName;
        QUIT
EOF

mysql -ummog -pmmog ${AccountName}<create_account_db.sql;
mysql -ummog -pmmog ${AccountName}<create_account_role.sql;
mysql -ummog -pmmog ${AccountName}<IpBlock.sql
