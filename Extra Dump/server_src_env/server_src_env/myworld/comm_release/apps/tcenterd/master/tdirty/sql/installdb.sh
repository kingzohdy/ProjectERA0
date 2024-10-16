#!/bin/sh

#installdb.sh

MYSQL=mysql

$MYSQL -uroot -e "drop database dirty"
$MYSQL -uroot -e "create database dirty"
$MYSQL -uroot dirty < dirty_db.sql
