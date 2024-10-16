#!/bin/sh

resdb_name=resdb

mysql -ummog -pmmog << EOF
        create database $resdb_name;
        QUIT
EOF

mysql -ummog -pmmog $resdb_name <resdb_data.sql
