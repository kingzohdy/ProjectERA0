#!/bin/sh

#7381
TBUSMSGDIR=/usr/local/mmog/TbusWorkDir/busssiness_66/conf
if [ -d $TBUSMSGDIR ];then
	#7381
	TBUSMGR=
	TBUSMGR=$(ipcs -m |grep 0x00001cd5)
	if [[ -z $TBUSMGR ]];then
		/usr/local/mmog/tools/tbusmgr --conf-file=${TBUSMSGDIR}/tbusmgr.xml -W
	fi
	#7382
	TRELAYMGR=
	TRELAYMGR=$(ipcs -m |grep 0x00001cd6)
	if [[ -z $TRELAYMGR ]];then
		/usr/local/mmog/tools/trelaymgr --conf-file=${TBUSMSGDIR}/relaymgr.xml -W
	fi

	#tbusd
	TBUSD=
	TBUSD=$(ps -ef|grep tbusd |grep mmog/)
	if [[ -z $TBUSD ]];then
        	/usr/local/mmog/tools/start_tbusd.sh
	fi

fi


#tagent
TAGENT=
TAGENT=$(ps -ef|grep bin/tagent |grep ../bin)
if [[ -z $TAGENT ]];then
	/usr/local/mmog/tcm/cfg/start_tagent.sh
fi

#tbusd
#TBUSD=
#TBUSD=$(ps -ef|grep tbusd |grep mmog/)
#if [[ -z $TBUSD ]];then
#        /usr/local/mmog/tools/start_tbusd.sh
#fi


