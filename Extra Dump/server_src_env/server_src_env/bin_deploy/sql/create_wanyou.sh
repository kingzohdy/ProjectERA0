#!/bin/sh

db_name=wanyou

mysql -ummog -pmmog << EOF
        create database $db_name;
        QUIT
EOF

mysql -ummog -pmmog $db_name <create_wanyou.sql

