#! /bin/bash

DEST_DIR="/usr/local/mmog"
LOCAL_DIR=$DEST_DIR
HOST_61_ADDR="192.168.0.61"
CONF_FILE_PATH="/usr/local/mmog/deploy.conf"

TAG_VERSION=
WORLD_ID=
MMOG_DIR=
REGION_DIR="${DEST_DIR}/region"
FAKE_MMOG_DIR=
DEPLOY_DIR=
FAKE_DEPLOY_DIR=
HOST_TOKEN=
CP_CMD=
R_RUN=
AGENT=
GAME_COMM_DIR="${HOME}/myworld/comm_release"
GAME_COMM_TOOLS_DIR="${GAME_COMM_DIR}/tools"
PROTECT_SCRIPTS="no"

usage()
{
	echo -e "Conf file : ${CONF_FILE_PATH} loaded"
	echo -e "Example: deploy to dir|tcm|tools "
	echo -e "  $0  cluster \n"
	echo -e "Example: deploy to /usr/local/mmog/world_{n} or remote machine "
	echo -e "  $0 \n"
}
deploy_world_res()
{
	CHECK_STRING=`echo $TAG_VERSION|egrep world`
	
	if [[ $WORLD_ID == "world_1" ]] ; then
		echo "svn up /data/shared/test/script"
		cd /data/shared/test/maps; 
		svn up
		find /data/shared/test/maps -name *.msk | xargs -i cp {} ${HOME}/myworld/cfg/res/
		echo "Fetch masks done."

		cd /data/shared/test/script; 
		svn up
		echo "Fetch script done."
	elif  [[ $WORLD_ID == "world_20" ]]; then
		echo "svn up /data/shared/wai/script"
		cd /data/shared/wai/maps; 
		svn up
		find /data/shared/wai/maps -name *.msk | xargs -i cp {} ${HOME}/myworld/cfg/res/
		echo "Fetch masks done."

		cd /data/shared/wai/script; 
		svn up
		echo "Fetch script done."	
	else
		echo "svn up /data/shared/script"
		~/myworld/tools/fetch_script.sh
		~/myworld/tools/fetch_masks.sh
	fi

	cd ~/myworld/tools/db_to_bin
	
	if [[ $WORLD_ID == "world_1" ]]; then
		./db_to_bin resdb_deploy
	elif  [[ $WORLD_ID == "world_20" ]]; then
		./db_to_bin resdb_wai	
	else
		./db_to_bin	
	fi
}
deploy_world_dir()
{
	if [[ -z ${HOST_TOKEN} ]]; then
		if [ ! -d ${DEPLOY_DIR} ];then
			mkdir -p ${DEPLOY_DIR}/{zone/{bin,cfg/{db_meta,res,script,fonts,dirty},log},logsvr/{bin,cfg,log,oplog,local},db/{bin,cfg/db_meta,log},cs_svr/{bin,cfg/db_meta,log}}
		fi
	else
		${R_RUN} "mkdir -p ${FAKE_DEPLOY_DIR}/{zone/{bin,cfg/{db_meta,res,script,fonts,dirty},log},logsvr/{bin,cfg,log,oplog,local},db/{bin,cfg/db_meta,log},cs_svr/{bin,cfg/db_meta,log}}"
		${R_RUN} "mkdir -p ${FAKE_MMOG_DIR}/tcm/cfg"
	fi

	rm -rf ${DEPLOY_DIR}/zone/bin/mud_svr	
	${CP_CMD} ./mud_svr/mud_svr ${DEPLOY_DIR}/zone/bin/mud_svr
	rm -rf ${DEPLOY_DIR}/zone/bin/mud_conn	
	${CP_CMD} ./zone/zone_conn/zone_conn ${DEPLOY_DIR}/zone/bin/mud_conn
	rm -rf ${DEPLOY_DIR}/zone/bin/zone_conn	
	${CP_CMD} ./zone/zone_conn/zone_conn ${DEPLOY_DIR}/zone/bin/
	rm -rf ${DEPLOY_DIR}/zone/bin/zone_svr
	${CP_CMD} ./zone/zone_svr/zone_svr ${DEPLOY_DIR}/zone/bin/
	rm -rf ${DEPLOY_DIR}/zone/bin/zone_ctrl
	${CP_CMD} ./zone/zone_ctrl/zone_ctrl ${DEPLOY_DIR}/zone/bin/
	${CP_CMD} ../cfg/db_meta/*.tdr ${DEPLOY_DIR}/zone/cfg/db_meta/
	${CP_CMD} ../cfg/proto_cs.tdr ${DEPLOY_DIR}/zone/cfg/
	#${CP_CMD} ../cfg/proto_ss.tdr ${DEPLOY_DIR}/cs_svr/cfg/
	${CP_CMD} ../cfg/resdb_meta.tdr ${DEPLOY_DIR}/zone/cfg/ 
	${CP_CMD} ../cfg/black_white.cfg ${DEPLOY_DIR}/zone/cfg/
	${CP_CMD} ../cfg/zone_shopping.xml ${DEPLOY_DIR}/zone/cfg/

	deploy_world_res
	
	cd ~/myworld/src

		if [[ -z ${HOST_TOKEN} ]]; then
			rm -rf ${DEPLOY_DIR}/zone/cfg/res/*
			cp -f ../cfg/res/*   ${DEPLOY_DIR}/zone/cfg/res/
			cp -f ~/myworld/tools/db_to_bin/svr_res/* ${DEPLOY_DIR}/zone/cfg/res/
			if [[ ${PROTECT_SCRIPTS} == "no" ]]; then
				rm -rf ${DEPLOY_DIR}/zone/cfg/script/*
				
    				if [[ $WORLD_ID == "world_1" ]]; then
     					cp -f /data/shared/test/script/*.mac ${DEPLOY_DIR}/zone/cfg/script/
					cp -f /data/shared/test/script/*.mxt ${DEPLOY_DIR}/zone/cfg/script/
     				elif  [[ $WORLD_ID == "world_20" ]]; then
     					cp -f /data/shared/wai/script/*.mac ${DEPLOY_DIR}/zone/cfg/script/
					 cp -f /data/shared/wai/script/*.mxt ${DEPLOY_DIR}/zone/cfg/script/	
    				elif  [[ $WORLD_ID == "world_8" || $WORLD_ID == "world_9" || $WORLD_ID == "world_2" || $WORLD_ID == "world_4" ]]; then
    					cp -f /data/shared/script/*.mac ${DEPLOY_DIR}/zone/cfg/script/
                                        cp -f /data/shared/script/*.mxt ${DEPLOY_DIR}/zone/cfg/script/
    				else
					echo "get world_3 script ......"
    					cp /usr/local/mmog/world_3/zone/cfg/script/*.mac ${DEPLOY_DIR}/zone/cfg/script/
					cp /usr/local/mmog/world_3/zone/cfg/script/*.mxt ${DEPLOY_DIR}/zone/cfg/script/
    				fi
    				
				chmod 777 ${DEPLOY_DIR}/zone/cfg/script/*
			fi
		else
			${R_RUN} "rm -rf ${FAKE_DEPLOY_DIR}/zone/cfg/res/*"
			${CP_CMD} -f ../cfg/res/* ${DEPLOY_DIR}/zone/cfg/res/
			cp -f ~/myworld/tools/db_to_bin/svr_res/* ${DEPLOY_DIR}/zone/cfg/res/
			
			if [[ ${PROTECT_SCRIPTS} == "no" ]]; then
				${R_RUN} "rm -rf ${FAKE_DEPLOY_DIR}/zone/cfg/script/*"
			
   				if [[ $WORLD_ID == "world_1" ]]; then
      					cp -f /data/shared/test/script/*.mac ${DEPLOY_DIR}/zone/cfg/script/
					cp -f /data/shared/test/script/*.mxt ${DEPLOY_DIR}/zone/cfg/script/
      				elif  [[ $WORLD_ID == "world_20" ]]; then
     					cp -f /data/shared/wai/script/*.mac ${DEPLOY_DIR}/zone/cfg/script/
					cp -f /data/shared/wai/script/*.mxt ${DEPLOY_DIR}/zone/cfg/script/	
   				elif  [[ $WORLD_ID == "world_8" || $WORLD_ID == "world_9" || $WORLD_ID == "world_2" || $WORLD_ID == "world_4" ]]; then
   					cp -f /data/shared/script/*.mac ${DEPLOY_DIR}/zone/cfg/script/
                                        cp -f /data/shared/script/*.mxt ${DEPLOY_DIR}/zone/cfg/script/
				else	
					echo "get world_3 script ......"
    					cp /usr/local/mmog/world_3/zone/cfg/script/*.mac ${DEPLOY_DIR}/zone/cfg/script/
                                        cp /usr/local/mmog/world_3/zone/cfg/script/*.mxt ${DEPLOY_DIR}/zone/cfg/script/
				fi
				
				${R_RUN} "chmod 777 ${FAKE_DEPLOY_DIR}/zone/cfg/script/*"
			fi
		fi

	${CP_CMD}  ../cfg/fonts/* ${DEPLOY_DIR}/zone/cfg/fonts/
	${CP_CMD}  ../cfg/dirty/*.txt ${DEPLOY_DIR}/zone/cfg/dirty/
	${CP_CMD} ./benchmark/gd/testgd  ${DEPLOY_DIR}/zone/cfg/
	
	${CP_CMD} ${GAME_COMM_DIR}/apps/torm/bin/tormsvr ${DEPLOY_DIR}/db/bin/role_db
	${CP_CMD} ${GAME_COMM_DIR}/apps/torm/bin/tormsvr ${DEPLOY_DIR}/db/bin/misc_db
	${CP_CMD} ../cfg/db_meta/*.tdr ${DEPLOY_DIR}/db/cfg/db_meta/
	${CP_CMD} ../tools/clear_mails.sh ${DEPLOY_DIR}/db/bin/

	${CP_CMD} ${GAME_COMM_DIR}/services/tlog_service/tlogd ${DEPLOY_DIR}/logsvr/bin/
	${CP_CMD} ${GAME_COMM_DIR}/services/tlog_service/logdump ${DEPLOY_DIR}/logsvr/bin/
	${CP_CMD} ${GAME_COMM_DIR}/services/tlog_service/*.sh  ${DEPLOY_DIR}/logsvr/cfg/
	${CP_CMD} ${GAME_COMM_DIR}/services/tlog_service/*.xml  ${DEPLOY_DIR}/logsvr/cfg/
	${CP_CMD} ${GAME_COMM_DIR}/services/tlog_service/*.tdr ${DEPLOY_DIR}/logsvr/cfg/
	${CP_CMD} ${HOME}/myworld/src/tlog_db/tlog_db ${DEPLOY_DIR}/logsvr/bin/
	${CP_CMD} ../cfg/op_log_meta.tdr ${DEPLOY_DIR}/logsvr/cfg/
	${CP_CMD} ../cfg/op_log.xml ${DEPLOY_DIR}/logsvr/cfg/

	${CP_CMD} ./zone/cs_svr/cs_svr ${DEPLOY_DIR}/cs_svr/bin/
	${CP_CMD} ./zone/zone_conn/zone_conn ${DEPLOY_DIR}/cs_svr/bin/cs_conn
	${CP_CMD} ../cfg/proto_ss.tdr ${DEPLOY_DIR}/cs_svr/cfg/
	${CP_CMD} ./myworld_conf_create/myworld_config.so ${MMOG_DIR}/tcm/cfg/
	${CP_CMD} ../cfg/db_meta/*.tdr ${DEPLOY_DIR}/cs_svr/cfg/db_meta/
	chmod 777 ${DEPLOY_DIR}/zone/cfg/zone_shopping.xml
	
}

deploy_world_init()
{
	local ename
	local conf_line
	local i

	ename=$(awk -F: '{print $1 " " $3}' /etc/passwd | grep " $UID$" | awk '{print $1}')
	conf_line=$(grep -P "^$ename:\d+:.*" ${CONF_FILE_PATH})

	if [[ -z $conf_line ]]; then
		echo ""$ename" can not deploy!"
		exit 127
	fi

	i=0
	for column in $(echo $conf_line | sed -e "s/:/ /g")
	do
		conf[$i]=$column
		((i+=1))
	done

	if [[ ${conf[3]} == "LOCAL" ]]; then
		MMOG_DIR=${DEST_DIR}
		CP_CMD="cp -v "
	elif [[ ${conf[3]} == "HOST_61" ]]; then
		HOST_TOKEN="${conf[2]}@${HOST_61_ADDR}"
		MMOG_DIR="${HOST_TOKEN}:${DEST_DIR}"
		CP_CMD="scp -P 22"
		R_RUN="ssh -p 22 ${HOST_TOKEN}"
	fi

	if [[ ${conf[1]} == "3" ]]; then
		echo -e "\033[0;31m!!This world's scripts is protected!!\033[m"
		PROTECT_SCRIPTS="yes"
	fi

	FAKE_MMOG_DIR=${DEST_DIR}
	FAKE_DEPLOY_DIR="${DEST_DIR}/world_${conf[1]}"
	DEPLOY_DIR="${MMOG_DIR}/world_${conf[1]}"
	WORLD_ID="world_${conf[1]}"
	AGENT=${conf[2]}

#	echo "ename:${conf[0]}, worldno:${conf[1]}, dest:${conf[2]}"
#	echo "mmog_dir: ${MMOG_DIR}"
#	echo "fake_deploy_dir: ${FAKE_DEPLOY_DIR}"
#	echo "deploy_dir: ${DEPLOY_DIR}"
#	echo "agent: ${AGENT}"
#	echo "cp_cmd: ${CP_CMD}"
#	echo "host_token: ${HOST_TOKEN}"
#	echo "r_run: ${R_RUN}"
}

deploy_cluster_init()
{
	MMOG_DIR=${DEST_DIR}
}

deploy_cluster_dir()
{
	if [ ! -d ${MMOG_DIR} ];then
		mkdir -p ${MMOG_DIR}/{dir/{bin,cfg,log},tcm/{bin,cfg,lib,log},tools}
	fi
	
	cp -rf ${GAME_COMM_TOOLS_DIR}/*  ${MMOG_DIR}/tools/
	cp -f /usr/local/mmog/tcm/TbusWorkDir/busssiness_66/conf/tbusmgr.xml ${MMOG_DIR}/tools/
	cp ../cfg/tcm/mngsystem_tbus.xml ${MMOG_DIR}/tools/
	cp ../tools/syn*  ${MMOG_DIR}/tools/
	cp ../tools/mysqlbak.sh ${MMOG_DIR}/tools/
	
	cp ./dir_svr/dir_svr  ${MMOG_DIR}/dir/bin/
	cp ./dir_svr/dir_svr ${MMOG_DIR}/dir/bin/dir_bak
	cp ./dir_svr/dir_svr ${MMOG_DIR}/dir/bin/dir_slave
	cp ./zone/zone_conn/zone_conn ${MMOG_DIR}/dir/bin/dir_svr_conn
	cp ./zone/zone_conn/zone_conn ${MMOG_DIR}/dir/bin/dir_bak_conn
	cp ./zone/zone_conn/zone_conn ${MMOG_DIR}/dir/bin/dir_slave_conn
	#cp ./myworld_conf_create/myworld_config.so ${MMOG_DIR}/tcm/cfg/
	
	cp ../cfg/world_list.cfg ${MMOG_DIR}/dir/cfg/
	cp ../cfg/black_white.cfg ${MMOG_DIR}/dir/cfg/
	cp ../cfg/proto_cs.tdr ${MMOG_DIR}/dir/cfg/
	
	cp ${GAME_COMM_DIR}/apps/tcm/bin/tcmconsole ${MMOG_DIR}/tcm/bin/
	cp ${GAME_COMM_DIR}/apps/tcm/bin/tconnd  ${MMOG_DIR}/tcm/bin/tcm_conn
	cp ${GAME_COMM_DIR}/apps/tcm/bin/tcmcenter ${MMOG_DIR}/tcm/bin/
	cp ${GAME_COMM_DIR}/apps/tagent/bin/tagent ${MMOG_DIR}/tcm/bin/
	cp ${GAME_COMM_DIR}/apps/tcenterd/bin/tcenterd ${MMOG_DIR}/tcm/bin/
	cp ../cfg/tcm/* ${MMOG_DIR}/tcm/cfg/
	cp ./myworld_conf_create/myworld_config.so ${MMOG_DIR}/tcm/cfg/
	cp ${GAME_COMM_DIR}/services/tlog_service/tconnddef.tdr ${MMOG_DIR}/tcm/cfg/
	
	
	# tagent is the last 
	killall 10 tagent
	cp ../cfg/lib/mod_tdirty.so ${MMOG_DIR}/tcm/lib/
	cp ../cfg/lib/mod_procmng.so.1.0.0 ${MMOG_DIR}/tcm/lib/
	cp ../cfg/lib/mod_tbusconfig.so.1.0.0 ${MMOG_DIR}/tcm/lib/
	cd ${MMOG_DIR}/tcm/cfg
}

deploy_region_dir()
{
        if [ ! -d ${REGION_DIR} ];then
                mkdir -p ${REGION_DIR}/{bin,cfg/{db_meta,res,script,fonts,dirty},log,logsvr/{bin,cfg,log,oplog,local}}
        fi

        #cp ./region_gate/region_gate ${REGION_DIR}/bin
        #cp ./bill_svr/bill_svr ${REGION_DIR}/bin
        #cp ./aas_svr/aas_svr ${REGION_DIR}/bin

        rm -rf ${REGION_DIR}/bin/battle_svr
        cp ./zone/zone_svr/zone_svr ${REGION_DIR}/bin/battle_svr
        cp ../cfg/db_meta/*.tdr ${REGION_DIR}/cfg/db_meta/
        cp ../cfg/proto_cs.tdr ${REGION_DIR}/cfg/
        cp ../cfg/resdb_meta.tdr ${REGION_DIR}/cfg/
        cp ../cfg/zone_shopping.xml ${REGION_DIR}/cfg/

        deploy_world_res

	cd ~/myworld/src

        rm -rf ${REGION_DIR}/cfg/res/*
        cp -f ../cfg/res/*   ${REGION_DIR}/cfg/res/
        cp -f ~/myworld/tools/db_to_bin/svr_res/* ${REGION_DIR}/cfg/res/
        rm -rf ${REGION_DIR}/cfg/script/*
        cp -rf /data/shared/script/* ${REGION_DIR}/cfg/script/
        chmod 777 ${REGION_DIR}/cfg/script/*

	cp ${GAME_COMM_DIR}/apps/torm/bin/tormsvr ${REGION_DIR}/bin/region_db

	cp ${GAME_COMM_DIR}/services/tlog_service/tlogd ${REGION_DIR}/logsvr/bin/region_tlogd
	cp ${GAME_COMM_DIR}/services/tlog_service/logdump ${REGION_DIR}/logsvr/bin/
	cp ${GAME_COMM_DIR}/services/tlog_service/*.sh  ${REGION_DIR}/logsvr/cfg/
	cp ${GAME_COMM_DIR}/services/tlog_service/*.xml  ${REGION_DIR}/logsvr/cfg/
	cp ${GAME_COMM_DIR}/services/tlog_service/*.tdr ${REGION_DIR}/logsvr/cfg/
	cp ${HOME}/myworld/src/tlog_db/tlog_db ${REGION_DIR}/logsvr/bin/region_tlog_db
	cp ../cfg/op_log.xml ${REGION_DIR}/logsvr/cfg/
	cp ../cfg/op_log_meta.tdr ${REGION_DIR}/logsvr/cfg/


}

deploy_battle()
{
su -l test -c 'cd /usr/local/mmog/region/cfg/; ./stop_battle_svr_3.0.105.1_66.sh;./stop_battle_svr_3.0.105.2_66.sh;' << EOF
test#1234
EOF
	cd ~/myworld/src
	cp -f ./zone/zone_svr/zone_svr ${REGION_DIR}/bin/battle_svr
	cp -f ../cfg/db_meta/*.tdr ${REGION_DIR}/cfg/db_meta/
	cp -f ../cfg/proto_cs.tdr ${REGION_DIR}/cfg/
        cp -f ../cfg/proto_ss.tdr ${REGION_DIR}/cfg/
	cp -f ../cfg/resdb_meta.tdr ${REGION_DIR}/cfg/
	cp -f ../cfg/res/*   ${REGION_DIR}/cfg/res/
	cp -f ~/myworld/tools/db_to_bin/svr_res/* ${REGION_DIR}/cfg/res/
	cp -rf /data/shared/script/* ${REGION_DIR}/cfg/script/	

	sleep 1
su -l test -c 'cd /usr/local/mmog/region/cfg/; ./start_battle_svr_3.0.105.1_66.sh;./start_battle_svr_3.0.105.2_66.sh' << EOF
test#1234
EOF
	echo "-------battle ok --------"
}


umask 0007
if [[ ${1} == "cluster" ]]; then
	echo -e "\033[0;32m---------deploy to cluster----------\033[m"
	deploy_cluster_init
	deploy_cluster_dir
	echo -e "\033[0;32m-------------deploy end-------------\033[m"
elif [[ ${1} == "region" ]]; then
	echo -e "\033[0;32---------deploy to region-----------\033[m"
	deploy_region_dir
	echo -e "\033[0;32m-------------deploy end-------------\033[m"
elif [[ ${1} == "battle" ]]; then
	echo -e "\033[0;32---------deploy to battle-----------\033[m"
	deploy_battle
	echo -e "\033[0;32m-------------deploy end-------------\033[m"	
elif [[ ${1} == "--help" ]]; then
	usage
else
	echo -e "\033[0;32m-----------deploy to world----------\033[m"
	TAG_VERSION=${1}
        echo "version $TAG_VERSION"
	deploy_world_init
	deploy_world_dir
	echo -e "\033[0;32m-------------deploy end-------------\033[m"
fi

