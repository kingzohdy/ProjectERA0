#!/bin/sh

#export VISUAL='ed'

INSTALL_CRON=
INSTALL_CRON=$(crontab -u mmog -l |grep install_cronttab.sh)
if [[  -z $INSTALL_CRON ]];then
	#echo "Cron is not exist!!"
	crontab -u mmog /dev/stdin << EOF
* * * * * /usr/local/mmog/tools/install_crontab.sh 
EOF
fi
#export VISUAL='vim'
