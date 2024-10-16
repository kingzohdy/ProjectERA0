#!/bin/sh


time=`date +%m_%d`

bak_dir="/usr/local/mmog/resdb_bak"

#set `date`
#mysqldump -ummog -pmmog resdb >/usr/local/jerry/script_test/resdb$1$2$3.sql

/usr/local/bin/mysqldump -userver -ptulin\#1234  resdb >$bak_dir/resdb_$time.sql

scp -P 36000 $bak_dir/resdb_$time.sql niko@192.168.0.79:/usr/local/mmog/cvsbak/resdb_$time.sql
