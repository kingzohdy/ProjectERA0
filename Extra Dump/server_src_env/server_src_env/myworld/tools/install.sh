#!/bin/sh
usage()
{
	echo "Example:./install.sh xxx.tgz [all | bin]"
	echo "paramter error"
}

if [ $# != 2 ];then
	usage
	exit -1;
fi

TGZ_NAME=$(echo "$1" |awk -F '.' '{print $2}')
if [ $TGZ_NAME != "tgz" ];then
	usage
        exit -1;
fi

if [ $2 != "all" -a $2 != "bin" ];then
	usage
        exit -1;
fi

if [ $2 = "all" ];then
Uid=$UID
if [ $Uid -ne 0 ];then
	echo "current user is not root"
	exit -1;
fi

echo "core-%e-%p-%t" > /proc/sys/kernel/core_pattern

ISEXIST="0"
USER_FIND=$(cat /etc/passwd |grep mmog |awk -F ':' '{print $1}')
for i in $USER_FIND ;do
	if [ $i = "mmog" ];then
		ISEXIST="1"
		break	
	fi
done

if [ ! -d "/usr/local/mmog" ];then
	mkdir /usr/local/mmog
fi

if [ $ISEXIST -ne "1" ];then
	useradd -d /usr/local/mmog -g 100 mmog
	#chown mmog:users mmog
	echo "create user"
fi

#FILE_FIND="0"
if [ -e "/etc/ssh/sshd_config" ]; then
	FILE_FIND=
	FILE_FIND=$(cat /etc/ssh/sshd_config |grep UsePrivilegeSeparation )
	if [[ ! -z $FILE_FIND ]];then
		sed /etc/ssh/sshd_config -i -e "s/$FILE_FIND/UsePrivilegeSeparation no/g"
		echo "$FILE_FIND"
	else 
		echo "UsePrivilegeSeparation no" >> /etc/ssh/sshd_config
	fi	
fi

if [ -e "/etc/security/limits.conf" ];then
	FILE_FIND=
	FILE_FIND=$(cat /etc/security/limits.conf |grep "hard nofile")
	#echo "$FILE_FIND"
	if [[ ! -z $FILE_FIND ]];then
		sed /etc/security/limits.conf -i -e "s/$FILE_FIND/* hard nofile 20400/g"
		echo "$FILE_FIND"
	else
		echo "* hard nofile 20400" >> /etc/security/limits.conf
	fi

	FILE_FIND=
        FILE_FIND=$(cat /etc/security/limits.conf |grep "soft nofile")
        if [[ ! -z $FILE_FIND ]];then
                sed /etc/security/limits.conf -i -e "s/$FILE_FIND/* soft nofile 20400/g"
                echo "$FILE_FIND"
        else
                echo "* soft nofile 20400" >> /etc/security/limits.conf
        fi	
fi


if [ -e "/etc/profile" ];then
	FILE_FIND=
	FILE_FIND=$(cat /etc/profile |grep "ulimit -c")
	if [[ -z $FILE_FIND ]];then
		echo "ulimit -c unlimited" >> /etc/profile
	else
	   	sed /etc/profile -i -e "s/$FILE_FIND/ulimit -c unlimited/g"
		echo "$FILE_FIND"
	fi	

	FILE_FIND=
        FILE_FIND=$(cat /etc/profile |grep "ulimit -n")
        if [[ -z $FILE_FIND ]];then
                echo "ulimit -n 20400" >> /etc/profile
	else
		sed /etc/profile -i -e "s/$FILE_FIND/ulimit -n 20400/g"
		echo "$FILE_FIND"
        fi
fi

if [ -e "/etc/ld.so.conf" ];then
	FILE_FIND=
	FILE_FIND=$(cat /etc/ld.so.conf |grep "/usr/local/mmog")
	if [[ -z $FILE_FIND ]];then
		echo "/usr/local/mmog" >> /etc/ld.so.conf
		echo "/usr/local/mmog"
		ldconfig
	fi
fi
fi

MMOG_DIR=/usr/local/mmog
CLUSTER=$MMOG_DIR/cluster
REGION=$MMOG_DIR/region
WORLD=$MMOG_DIR/world
TCM=$MMOG_DIR/tcm
TOOLS=$MMOG_DIR/tools
TBUS=$MMOG_DIR/tbusd
WEBSERVICE=$MMOG_DIR/webservice


AGENT_IP=$(cat ./tcm.cfg |grep "CENTER"|awk -F '=' '{print $2}')
echo "AGENT_IP=$AGENT_IP"

if [ -d "./bin_deploy" ];then
	rm -rf ./bin_deploy
fi
tar -zxvf $1
cd bin_deploy

chg_agent_ip()
{
#	AGENT_IP=$(cat ./tcm.cfg |grep "CENTER"|awk -F '=' '{print $2}')
	sed ./tcm/cfg/centerd.cfg -i -e "s/192.168.0.77/${AGENT_IP}/g"
        sed ./tcm/cfg/tagent.cfg -i -e "s/192.168.0.77/${AGENT_IP}/g"
	sed ./tcm/cfg/tcm_conn.xml -i -e "s/192.168.0.77/${AGENT_IP}/g"
	sed ./tcm/cfg/tcmconsole.xml -i -e "s/192.168.0.77/${AGENT_IP}/g"
}

install_default()
{
	cp -f ./libmysqlclient.so.15.0.0  ${MMOG_DIR}/
#--------------------------zone-----------------------------	
	cp -f ./zone/cfg/proto_cs.tdr ${WORLD}/zone/cfg/
	cp -f ./zone/cfg/resdb_meta.tdr ${WORLD}/zone/cfg/
	cp -f ./zone/cfg/zone_shopping.xml ${WORLD}/zone/cfg/	
	cp -f ./zone/cfg/db_meta/*.tdr ${WORLD}/zone/cfg/db_meta/
	cp -f ./zone/cfg/res/* ${WORLD}/zone/cfg/res/
	cp -f ./*.msk ${WORLD}/zone/cfg/res/
	cp -rf ./zone/script/* ${WORLD}/zone/cfg/script/
	cp -f ./zone/bin/* ${WORLD}/zone/bin/
	cp -f ./zone/cfg/dirty/* ${WORLD}/zone/cfg/dirty/
	cp -f ./zone/cfg/dirty/* ${WORLD}/mud/cfg/dirty/
        cp -f ./zone/cfg/dirty/* ${REGION}/cfg/dirty/
	chmod 777 -R ${WORLD}/zone/cfg/
#--------------------------roledb miscdb-----------------------------
	cp -f ./db/misc_db ${WORLD}/db/bin/
        cp -f ./db/role_db ${WORLD}/db/bin/
        cp -f ./db/db_meta/*.tdr ${WORLD}/db/cfg/db_meta/
        cp -f ./db/clear_mails.sh ${WORLD}/db/bin/
	cp -f ./db/mysql ${WORLD}/db/bin/
#------------------------cs_svr---------------------------
	cp -f ./cs_conn ${WORLD}/cs_svr/bin/
        cp -f ./cs_svr ${WORLD}/cs_svr/bin/
        cp -f ./zone/cfg/proto_ss.tdr ${WORLD}/cs_svr/cfg/
	cp -f ./zone/cfg/proto_ss.tdr ${CLUSTER}/cfg/
        cp -f ./zone/cfg/resdb_meta.tdr ${WORLD}/cs_svr/cfg/
        cp -f ./zone/cfg/db_meta/*.tdr ${WORLD}/cs_svr/cfg/db_meta/
#------------------------mud-------------------------
	cp -f ./mud/* ${WORLD}/mud/bin/
	cp -f ./zone/cfg/proto_ss.tdr ${WORLD}/mud/cfg/
	cp -f ./zone/cfg/proto_cs.tdr ${WORLD}/mud/cfg/
	cp -f ./zone/cfg/resdb_meta.tdr ${WORLD}/mud/cfg/
	cp -f ./zone/cfg/db_meta/*.tdr ${WORLD}/mud/cfg/db_meta/
	cp -f ./zone/cfg/res/* ${WORLD}/mud/cfg/res/
#------------------------webservice-------------------------
	cp -rf ./webservice/* ${WEBSERVICE}/
	chmod 777 -R ${WEBSERVICE}/cfg/
#------------------------tlog------------------------------
	cp -f ./logsvr/tlog_db ${WORLD}/logsvr/bin/
        cp -f ./logsvr/logdump ${WORLD}/logsvr/bin/
        cp -f ./logsvr/tlogd ${WORLD}/logsvr/bin/
        cp -f ./logsvr/cfg/* ${WORLD}/logsvr/cfg/
	cp -f ./logsvr/cfg/tconnddef.tdr ${WORLD}/logsvr/bin/
	
	cp -f ./logsvr/tlog_db ${REGION}/logsvr/bin/region_tlog_db
	cp -f ./logsvr/logdump ${REGION}/logsvr/bin/
	cp -f ./logsvr/tlogd ${REGION}/logsvr/bin/region_tlogd
	cp -f ./logsvr/cfg/* ${REGION}/logsvr/cfg/
	cp -f ./logsvr/cfg/tconnddef.tdr ${REGION}/logsvr/bin/
#------------------------cluster------------------------------------
        cp -f ./dir/dir* ${CLUSTER}/bin/
     
        cp -f ./dir/cfg/*.tdr ${CLUSTER}/cfg/
        
	cp -f ./zone/cfg/db_meta/* ${CLUSTER}/cfg/db_meta/
	cp -f ./version/version* ${CLUSTER}/bin/
	cp -f ./recv/recv_report ${CLUSTER}/bin/
#---------------------region-----------------------------
	cp -f ./zone/cfg/proto_cs.tdr ${REGION}/cfg/
	cp -f ./zone/cfg/proto_ss.tdr ${REGION}/cfg/
	cp -f ./zone/cfg/resdb_meta.tdr ${REGION}/cfg/
	cp -f ./zone/cfg/zone_shopping.xml ${REGION}/cfg/
	cp -f ./zone/cfg/db_meta/* ${REGION}/cfg/db_meta/
	cp -f ./zone/cfg/zone_shopping.xml ${REGION}/cfg/	
	cp -f ./zone/cfg/res/* ${REGION}/cfg/res/
	cp -f ./*.msk ${REGION}/cfg/res/
	cp -rf ./zone/script/* ${REGION}/cfg/script/
	chmod 777 -R ${REGION}/cfg
	cp -f ./tools/* ${TOOLS}/		
	cp -f ./auth/auth* ${REGION}/bin/
	cp -f ./version/account_cs* ${REGION}/bin/
	cp -f ./accdb/account_db ${REGION}/bin/
	cp -f ./informer/informer_svr ${REGION}/bin/
	cp -f ./bill/bill* ${REGION}/bin/
	cp -f ./aas/aas_svr ${REGION}/bin/
	cp -f ./region/region_gate ${REGION}/bin/
	cp -f ./region/region_db ${REGION}/bin/
	cp -f ./battle/battle_svr ${REGION}/bin/	
	cp -f ./service/service_svr ${REGION}/bin/
#----------------------resdb sql---------------------------
	cp -f ./sql/* 	${TOOLS}/
#----------------------tcm---------------------------------
	killall tagent
	cp -f ./tcm/bin/* ${TCM}/bin/
	cp -f ./tcm/cfg/*.sh 	${TCM}/cfg/
	cp -f ./tcm/lib/*       ${TCM}/lib/
}

install_all()
{
	chg_agent_ip
	install_default
	#------------------------apexsvr----------------------
	#cp -fR ./apexsvr/* ${MMOG_DIR}/apexsvr/
	chmod 777 -R ${MMOG_DIR}/apexsvr
	cp -f ./sql/* ${TOOLS}/
	cp -R ./tcm/* ${TCM}/
	cp -f ./*.sh ${TOOLS}/
	#cp -f ./tools/* ${TOOLS}/
	
	cp -f ./zone/cfg/fonts/* ${WORLD}/zone/cfg/fonts/
	cp -f ./zone/cfg/fonts/* ${WORLD}/mud/cfg/fonts/
	cp -f ./zone/cfg/fonts/* ${REGION}/cfg/fonts/
		
	cp -f ./zone/cfg/dirty/* ${WORLD}/zone/cfg/dirty/
	cp -f ./zone/cfg/dirty/* ${WORLD}/mud/cfg/dirty/
        cp -f ./zone/cfg/dirty/* ${REGION}/cfg/dirty/
        
        cp -f ./zone/cfg/black_white.cfg ${WORLD}/zone/cfg/
        cp -f ./zone/cfg/black_white.cfg ${WORLD}/mud/cfg/
        cp -f ./zone/cfg/black_white.cfg ${REGION}/cfg/
        
	cp -f ./tbus* ${TBUS}/
	
	cp -f ./informer_tool/libifm2_32.so /usr/local/mmog/
	#cp -f ./informer_tool/libcryptopp.so /usr/local/mmog/
	cp -f ./informer_tool/active_config.ini /usr/local/mmog/region/cfg/conf
	cp -f ./informer_tool/charge_config.ini /usr/local/mmog/region/cfg/conf
	cp -f ./informer_tool/member_config.ini /usr/local/mmog/region/cfg/conf
	cp -R ./informer_tool/key /usr/local/mmog/region/bin/
	cp -f ./dir/cfg/* ${CLUSTER}/cfg/
}

if [ $2 = "all" ];then
	if [ -d $CLUSTER ];then
        	rm -rf $CLUSTER
	fi
	if [ -d $REGION ];then
		rm -rf $REGION
	fi
	if [ -d $WORLD ];then
		rm -rf $WORLD
	fi
	if [ -d $TCM ];then
		rm -rf $TCM
	fi
	if [ -d $TOOLS ];then
		rm -rf $TOOLS
	fi
	if [ -d $TBUS ];then
		rm -rf $TBUS
	fi
	if [ -d $WEBSERVICE ];then
		rm -rf $WEBSERVICE
	fi

	mkdir -p ${CLUSTER}/{cfg/db_meta,bin/dirxml,log}
	mkdir -p ${REGION}/{cfg/{db_meta,res,script,fonts,dirty,conf},bin/key,log,logsvr/{bin,cfg,log,oplog,local}}
	mkdir -p ${TCM}/{bin,cfg,lib,log}
	mkdir -p ${WORLD}/{zone/{bin,cfg/{db_meta,dirty,fonts,res,script},log/oplog},logsvr/{cfg,bin,log},apexsvr,db/{cfg/db_meta,log,bin,},cs_svr/{cfg/db_meta,bin,log},mud/{cfg/{db_meta,res,dirty,fonts},bin,log/oplog}}
	mkdir -p $TOOLS
	mkdir -p $TBUS
	mkdir -p $WEBSERVICE/{cs_service/{bin,cfg,dep,log}}
#	mkdir -p $CS_INTERFACE

	ln -s ${REGION}/log ${REGION}/bin/log
	install_all
else
	if [ ! -d $CLUSTER ];then
		echo "please check target folder:/usr/local/mmog/cluster is exist!"
		exit -1
	fi

	if [ ! -d $REGION ];then
                echo "please check target folder:/usr/local/mmog/region  is exist!"
		exit -1
        fi

	if [ ! -d $WORLD ];then
		echo "please check target folder:/usr/local/mmog/world  is exist!"
                exit -1
	fi

	if [ ! -d $WEBSERVICE ];then
		echo "please check target folder:/usr/local/mmog/webservice  is exist!"
                exit -1
	fi
	
	rm -rf ./tools/*.ini
	rm -rf ./tools/*.xml
	rm -rf ./tools/*.txt

	install_default

fi


ipcs |awk '{if($6==0) printf "ipcrm shm %d\n",$2}' |sh ; ipcs

if [ $2 = "all" ];then
cd /usr/local/
chown -R mmog:users mmog

cd $MMOG_DIR
chown -R mmog:users $WORLD
chown -R mmog:users $TOOLS
chown -R mmog:users $TCM
chown -R mmog:users $CLUSTER
chown -R mmog:users $REGION
chown -R mmog:users $TBUS
chown -R mmog:users $WEBSERVICE
chown mmog:users ${MMOG_DIR}/libmysqlclient.so.15.0.0
chmod 770 ${MMOG_DIR}/libmysqlclient.so.15.0.0
chmod 777 /dev/shm/
ln -snf ${MMOG_DIR}/libmysqlclient.so.15.0.0 ${MMOG_DIR}/libmysqlclient.so.15
#rm -rf $MMOG_DIR/bin_pack/tempdir
su - mmog -c "cd /usr/local/mmog/tcm/cfg;./start_tagent.sh"
#chown -R mmog:users mmog

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:/usr/local/mmog"
ldconfig


/usr/local/mmog/tools/install_cron.sh

su mmog
fi

