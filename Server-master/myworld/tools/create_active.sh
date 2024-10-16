#!/bin/sh

db_name=activecode

mysql -ummog -pmmog << EOF
        create database $db_name;
        QUIT
EOF

mysql -ummog -pmmog $db_name <Active.sql
mysql -ummog -pmmog $db_name <ActiveCodeGiftDef.sql

