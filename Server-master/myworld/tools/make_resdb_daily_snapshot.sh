#!/bin/bash

err="resdb_daily.err"

row_show_tables()
{
/usr/local/bin/mysql -u mmog -D ${1} -pmmog << EOF
SHOW TABLES;
EOF
}

cd ~/myworld/src/meta_xml
cvs up
make;

cd ~/myworld/src/resdb_daily
cvs up
make clean; make;
if [[ $? != "0" ]]; then
	echo "Make resdb_daily fail" >> ${err}
	exit 1
fi

output="/data/resdb_daily_snapshot/$(date +%m_%d)"
mkdir ${output}
if [[ $? != "0" ]]; then
	echo "Make dir: ${output} fail" >> ${err}
	exit 1
fi

table_names=$(row_show_tables resdb | sed -e "s/Tables_in_resdb//")
./resdb_daily ${output} ${table_names} 2&> ${err}
if [[ $? != "0" ]]; then
	echo "Make resdb snapshot fail" >> ${err}
	exit 1
fi

exit 0
