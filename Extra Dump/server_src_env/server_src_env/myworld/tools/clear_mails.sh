#!/bin/sh

PATH=$PATH:./

CONF_FILE=
for i in $(ls ../cfg/); do
	CONF_FILE=$(echo ${i} | grep "^role_db[[:graph:]]*.xml$")
	if [[ -n ${CONF_FILE} ]]; then
		break
	fi
done
if [[ -z ${CONF_FILE} ]]; then
	echo "Can not find any conf file!"
	exit
fi
CONF_FILE="../cfg/${CONF_FILE}"
declare -r MAIL_EXIST_SECS=5184000 # 5184000 seconds == 60 days
declare -r ROLE_DEL_EXIST_SECS=15552000 # 180 days

check_env()
{
	echo -n "* Check configure file ... "
	if [[ -e $CONF_FILE ]]
	then
		echo "Ok"
	else
		echo "Fail"
		exit -1
	fi

	echo -n "* Check mysql command ... "
	if [[ -e $(which mysql) ]]
	then
		echo "Ok"
	else
		echo "Fail"
		exit -1
	fi

	echo -n "* Check mktemp command ... "
	if [[ -e $(which mktemp) ]]
	then
		echo "Ok"
	else
		echo "Fail"
		exit -1
	fi

	return 0
}

get_conf_entry()
{
	local entry

	entry=$(sed $CONF_FILE -n -e "/<$1>.*<\/$1>/p" | sed -e "s/^.*<$1>//" | sed -e "s/<\/$1>//")

	if [[ entry == "" ]]
	then
		echo "no \"$1\" entry in configure file"
		exit -1
	fi

	echo $entry
}

exec_sql_script()
{
	local user
	local passwd
	local host
	local sock
	local dbname

	echo "* [`date -R`] Now deleting unusable mails ..."

	user=$(get_conf_entry "DBMSUser")
	passwd=$(get_conf_entry "DBMSPassword")
	host=$(get_conf_entry "DBMSConnectionInfo")
	sock=$(get_conf_entry "DBMSSock")
	dbname=$(get_conf_entry "DBMSCurDatabaseName")

	mysql -vvv --host=$host --user=$user --password=$passwd $dbname < $1

	if [[ $? != "0" ]]
	then
		echo "* [`date -R`] Deleting failed!"
		exit -1
	fi

	echo "* [`date -R`] Cleanup!"
}

generate_sql_file()
{
	local endpoint
	local sqlfile
	local endpoint1

	endpoint=$(expr $(date +%s) - $MAIL_EXIST_SECS)
	endpoint1=$(expr $(date +%s) - $ROLE_DEL_EXIST_SECS)
	sqlfile=$(mktemp)

	echo "DELETE FROM MailEntry WHERE Time < $endpoint;" >> $sqlfile
	echo "DELETE FROM MailEntry WHERE Recv IN ( SELECT NameID.Name FROM NameID WHERE NameID.NameType = 65 );" >> $sqlfile
	echo "DELETE FROM MailEntry WHERE Recv NOT IN ( SELECT NameID.Name FROM NameID );" >> $sqlfile
	
	echo "DELETE FROM RoleData WHERE Level < 40 and DelTime > 0 and DelTime < $endpoint1 ;" >> $sqlfile
	echo "DELETE FROM NameID WHERE NameType = 65 and Name NOT IN ( SELECT RoleData.RoleName FROM RoleData );" >> $sqlfile

	echo $sqlfile
}




# Scrip entry :
if [[ $1 == "-d" ]]
then
	if [[ -z $2 ]]
	then
		nohup /bin/sh $0 &
	else
		nohup /bin/sh $0 >> $2 &
	fi
	exit
fi

check_env

#exec_sql_script clear.sql
sqlfile=$(generate_sql_file)
exec_sql_script $sqlfile

rm -f sqlfile
exit

