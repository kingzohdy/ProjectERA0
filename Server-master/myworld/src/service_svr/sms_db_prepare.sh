#!/bin/sh

#account_3
srcdbname=${1}
#$newtbl
newtbl=${2}

#大区account数据库
srcdbip='192.168.0.77'
srcuser='mmog'
srcpwd='mmog'

#wanyou网站数据库,需要写权限
dstdbip='192.168.0.77'
dstuser='server'
dstpwd='tulin#1234'

if [ "$srcdbname" != "remove" ]; then
	#mysql -u$dstuser -p$dstpwd -h$dstdbip --force wanyou -e "DROP TABLE IF EXISTS $newtbl;"
#else
	mysqldump -u$srcuser -p$srcuser -h$srcdbip --insert-ignore $srcdbname Account --where "UinFlag & 256 and LastLoginTime=0" > ./sms_prepare.sql
	sed -i s/'Pass9Account'/'AccName'/g ./sms_prepare.sql
	sed -i s/'FreezeReason'/'Mobile'/g ./sms_prepare.sql
	sed -i s/'FreezeStartTime'/'Send'/g ./sms_prepare.sql
	sed -i s/'Account'/$newtbl/g ./sms_prepare.sql
	mysql -u$dstuser -p$dstpwd -h$dstdbip --force wanyou < ./sms_prepare.sql
	mysql -u$dstuser -p$dstpwd -h$dstdbip --force wanyou -e "update $newtbl a,ljy_member_profile b set a.Mobile=b.mobile where a.AccName=b.username; "
	mysql -u$dstuser -p$dstpwd -h$dstdbip --force wanyou -e "update $newtbl  set Send=1 where Mobile=''; "
	mysql -u$dstuser -p$dstpwd -h$dstdbip --force wanyou -e "flush table $newtbl  ; "
fi

