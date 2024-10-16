#!/bin/sh

if [ $# = 2 ];then
	ProjectName="myworld"
else
	echo "parameter errer!"
	echo "./version.sh all|bin version"
	exit -1
fi
resdb_name=resdb_$2

CHECK=
CHECK=$(echo $2 |grep _)
if [ -z $CHECK ];then
	echo $2 is error!!!!!
	exit
fi

FINDVERLOCK=
FINDVERLOCK=$(find /data/cvs/cvsroot/myworld/src/comm/Locks/ -name version.h*)

if [ ! -z $FINDVERLOCK ];then
	echo "version.h is locking"
	exit
fi

cd ~/myworld/src/comm/
sedit version.h << EOF
        y
EOF
~/myworld/tools/ver.sh $2
cvs commit -m version.h

su -c 'rm /data/cvs/cvsroot/myworld/src/comm/Locks/version.h*'<< FFF
hello#sun
FFF
#强制更新到最新的版本
#运营中的版本就不会用这个打包
#运营中出现的问题，一般单发二进制
cd ~/myworld/
cvs update -A

cd ~/myworld/src/meta_xml/
./boundversion.sh 0

cd ~/myworld/src/
make clean;make


cd ~/myworld/c++/src
make clean;make

cd ~/myworld/tools/
make clean;make

cd ~/myworld/tools/export_item
./exportitem resdb localhost mmog mmog > imp_item.txt

cd ~/myworld/tools/db_to_bin
./db_to_bin
#if [ $? != "0" ];then
#	echo "db_to_bin error!"
#	exit -1
#fi

time=`date +%y_%m_%d_%H_%M`
bak_dir="/usr/local/mmog/code_bak"
bin_dir="/usr/local/mmog/bin_pack"
game_comm_dir=${HOME}/myworld/comm_release


/usr/local/bin/mysqldump -userver -ptulin#1234 --add-drop-database --add-drop-table --default-character-set=latin1  resdb >${bak_dir}/resdb_$2_$time.sql
tar zcvf $bak_dir/${ProjectName}_bak_$2_$time.tgz ~/$ProjectName
chmod 777 $bak_dir/${ProjectName}_bak_$2_$time.tgz

cd ~/$ProjectName

cvs tag tag_${2}_01
cvs rtag -b -r tag_${2}_01 tag_${2} $ProjectName

rm -rf ${bin_dir}/bin_deploy

mkdir -p ${bin_dir}/bin_deploy/{apexsvr,sql,tools/script_doc,informer_tool,resdb,db_unblob_interface/include,tcm/{bin,cfg,lib,log},dir/cfg,db/db_meta,zone/{bin,cfg,script,cfg/{db_meta,res,fonts,dirty}},logsvr/{cfg,res_bin},version,bill,auth,informer,aas,service,region,accdb,mud,battle,recv,webservice/{cs_service,bi_service,acc_service,chat_mon_service}/,web/{shop_db_edit}}


Generate_Conn()
{
	cp -f ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/zone/bin/
	cp -f ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/cs_conn
        cp -f ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/dir/dir_svr_conn
        cp -f ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/dir/dir_bak_conn
        cp -f ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/dir/dir_slave_conn
	cp -f ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/dir/dir_slave1_conn
	cp -f ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/auth/auth_conn
	cp -f ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/auth/auth_bak_conn
	cp -f ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/auth/auth_slave_conn
	cp -f ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/version/version_conn
	cp -f ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/version/version_bak_conn
	cp -f ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/version/version_slave_conn
	cp -f ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/version/version_slave1_conn
	cp -f ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/version/account_cs_conn
	cp -f ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/mud/mud_conn
}

Generate_Bin()
{
	cp -f ~/myworld/src/zone/zone_ctrl/zone_ctrl $bin_dir/bin_deploy/zone/bin/
        cp -f ~/myworld/src/zone/zone_svr/zone_svr $bin_dir/bin_deploy/zone/bin/
	cp -f ~/myworld/src/dir_svr/dir_svr $bin_dir/bin_deploy/dir/
        cp -f ~/myworld/src/dir_svr/dir_svr $bin_dir/bin_deploy/dir/dir_bak
        cp -f ~/myworld/src/dir_svr/dir_svr  $bin_dir/bin_deploy/dir/dir_slave
	cp -f ~/myworld/src/dir_svr/dir_svr  $bin_dir/bin_deploy/dir/dir_slave1
	cp -f ~/myworld/src/zone/cs_svr/cs_svr $bin_dir/bin_deploy/
	cp -f ${game_comm_dir}/apps/torm/bin/tormsvr $bin_dir/bin_deploy/db/role_db
        cp -f ${game_comm_dir}/apps/torm/bin/tormsvr $bin_dir/bin_deploy/db/misc_db
	cp -f /usr/local/bin/mysql $bin_dir/bin_deploy/db/
	cp -f ${game_comm_dir}/services/tlog_service/tlogd $bin_dir/bin_deploy/logsvr/
        cp -f ${game_comm_dir}/services/tlog_service/logdump $bin_dir/bin_deploy/logsvr/
	cp -f ${HOME}/myworld/src/tlog_db/tlog_db $bin_dir/bin_deploy/logsvr/
	cp -f ${HOME}/myworld/comm_release/apps/torm/bin/tormsvr $bin_dir/bin_deploy/accdb/account_db
        cp -f ${HOME}/myworld/comm_release/apps/torm/bin/tormsvr $bin_dir/bin_deploy/bill/bill_db
        cp -f ~/myworld/src/aas_svr/aas_svr $bin_dir/bin_deploy/aas/
        cp -f ~/myworld/src/auth_svr/auth_svr $bin_dir/bin_deploy/auth/auth_svr
	cp -f ~/myworld/src/auth_svr/auth_svr $bin_dir/bin_deploy/auth/auth_bak
	cp -f ~/myworld/src/auth_svr/auth_svr $bin_dir/bin_deploy/auth/auth_slave
        cp -f ~/myworld/src/informer_svr/informer_svr $bin_dir/bin_deploy/informer/
        cp -f ~/myworld/src/version_svr/version_svr $bin_dir/bin_deploy/version/version_svr
	cp -f ~/myworld/src/version_svr/version_svr $bin_dir/bin_deploy/version/version_bak
	cp -f ~/myworld/src/version_svr/version_svr $bin_dir/bin_deploy/version/version_slave
	cp -f ~/myworld/src/version_svr/version_svr $bin_dir/bin_deploy/version/version_slave1
	cp -f ~/myworld/src/account_cs_svr/account_cs_svr $bin_dir/bin_deploy/version/account_cs_svr
	cp -f ~/myworld/src/mud_svr/mud_svr $bin_dir/bin_deploy/mud/mud_svr
	cp -f ~/myworld/src/zone/zone_ctrl/zone_ctrl $bin_dir/bin_deploy/mud/mud_ctrl
	cp -f ~/myworld/tools/apex/apexproxy/AHThunkConfig.ini $bin_dir/bin_deploy/mud/
	cp -f ~/myworld/tools/apex/apexproxy/ApexProxyMain.so $bin_dir/bin_deploy/mud/
	cp -f ~/myworld/tools/apex/apexproxy/ApexProxy.so $bin_dir/bin_deploy/mud/
        cp -f ~/myworld/src/region_gate/region_gate  $bin_dir/bin_deploy/region/
        cp -f ~/myworld/src/bill_svr/bill_svr  $bin_dir/bin_deploy/bill/
        cp -f ~/myworld/src/service_svr/service_svr $bin_dir/bin_deploy/service/
        cp -fR ~/myworld/tools/apex/apexsvr/*  $bin_dir/bin_deploy/apexsvr/

	#cp -R ~/myworld/comm_release/services/agent_client $bin_dir/bin_deploy/
	#cp -R ~/myworld/comm_release/services/agent_server $bin_dir/bin_deploy/
	cp -f ~/myworld/src/recv_report/recv_report $bin_dir/bin_deploy/recv/recv_report

	cp -f ~/myworld/src/zone/zone_svr/zone_svr $bin_dir/bin_deploy/battle/battle_svr
	cp -f ${game_comm_dir}/apps/torm/bin/tormsvr $bin_dir/bin_deploy/region/region_db
}



Generate_Sql()
{
	cd ~/myworld/tools/
        ./create_dbtab_sql.sh
	./create_log_sql.sh
	./create_resdb_data.sh

#resdb_name=resdb_$2
mysql -userver  -ptulin#1234 << EOF
        	create database $resdb_name;
        	QUIT
EOF
	mysql -userver -ptulin#1234  $resdb_name <resdb_data.sql
	mysql -userver -ptulin#1234  resdb_deploy <resdb_data.sql

	#./resdb_grant.sh $resdb_name

        cp -f ~/myworld/tools/*.sql $bin_dir/bin_deploy/sql/
	cp -f ~/myworld/tools/install_cro* $bin_dir/bin_deploy/sql/
	cp  -f ~/myworld/tools/create_*.sh $bin_dir/bin_deploy/sql/
	rm -rf $bin_dir/bin_deploy/sql/create_dbtab_sql.sh
	rm -rf $bin_dir/bin_deploy/sql/create_log_sql.sh
	rm -rf $bin_dir/bin_deploy/sql/create_resdb_data.sh
	rm -rf $bin_dir/bin_deploy/sql/create_res_sql.sh
	cp -f ~/myworld/tools/resdb_data.sql $bin_dir/bin_deploy/resdb/
	./fetch_res_xls.sh
	cp -f /data/资源数据表/资源数据库表说明.xls $bin_dir/bin_deploy/resdb/
	cp -f /data/脚本指令说明/compiler.exe $bin_dir/bin_deploy/tools/script_doc/
	cp -f /data/脚本指令说明/cmd.ini $bin_dir/bin_deploy/tools/script_doc/
	cp -f /data/脚本指令说明/gamecmd.rtf $bin_dir/bin_deploy/tools/script_doc/
	cp -f /data/脚本指令说明/mchelp.cnt $bin_dir/bin_deploy/tools/script_doc/
	cp -f /data/脚本指令说明/MCHELP.HLP  $bin_dir/bin_deploy/tools/script_doc/
}


webservice()
{
	chmod 777 ~/myworld/src/cs_service/version_cs_service.sh
	~/myworld/src/cs_service/version_cs_service.sh $bin_dir/bin_deploy/webservice/cs_service/

	chmod 777 ~/myworld/src/acc_service/version_acc_service.sh
	~/myworld/src/acc_service/version_acc_service.sh $bin_dir/bin_deploy/webservice/acc_service/

	chmod 777 ~/myworld/src/bi_service/version_bi_service.sh
	~/myworld/src/bi_service/version_bi_service.sh $bin_dir/bin_deploy/webservice/bi_service/

	chmod 777 ~/myworld/src/chat_mon_service/version_chat_mon_service.sh
	~/myworld/src/chat_mon_service/version_chat_mon_service.sh $bin_dir/bin_deploy/webservice/chat_mon_service/

	chmod 777 ~/myworld/src/sign_service/version_sign_service.sh
	~/myworld/src/sign_service/version_sign_service.sh $bin_dir/bin_deploy/webservice/sign_service/
}

web()
{
	chmod 777 ~/myworld/tools/shop_db_edit/version_shop_db_edit.sh
	~/myworld/tools/shop_db_edit/version_shop_db_edit.sh $bin_dir/bin_deploy/web/shop_db_edit/
}

~/myworld/tools/fetch_script.sh 

cp -R ~/myworld/doc/运维文档 $bin_dir/bin_deploy/
cp -f ~/myworld/doc/服务器架构设计说明书.doc $bin_dir/bin_deploy/运维文档/
cp -f ~/myworld/doc/运营日志数据库表说明.xls $bin_dir/bin_deploy/运维文档/
cp -f ~/myworld/doc/帐号角色数据库表说明.xls $bin_dir/bin_deploy/运维文档/
cp -f ~/myworld/tools/start_cluster.sh $bin_dir/bin_deploy/
cp -f ~/myworld/tools/start_world.sh $bin_dir/bin_deploy/
cp -f ~/myworld/tools/stop_cluster.sh $bin_dir/bin_deploy/
cp -f ~/myworld/tools/stop_world.sh $bin_dir/bin_deploy/
cp -f ~/myworld/tools/replace.sh $bin_dir/bin_deploy/
#cp -f ~/myworld/tools/install_cro* $bin_dir/bin_deploy/
cp -f ~/myworld/comm_release/lib/libmysqlclient.so.15.0.0 $bin_dir/bin_deploy/

Generate_Sql

cd ~/$ProjectName
	
cp -rf /data/shared/script/* $bin_dir/bin_deploy/zone/script/
cp -f ~/myworld/cfg/db_meta/*.tdr $bin_dir/bin_deploy/zone/cfg/db_meta/
cp -f ~/myworld/cfg/proto_cs.tdr $bin_dir/bin_deploy/zone/cfg/
cp -f ~/myworld/cfg/proto_ss.tdr $bin_dir/bin_deploy/zone/cfg/
cp -f ~/myworld/cfg/zone_shopping.xml $bin_dir/bin_deploy/zone/cfg/
cp -f ~/myworld/cfg/resdb_meta.tdr $bin_dir/bin_deploy/zone/cfg/
cp -f ~/myworld/cfg/black_white.cfg $bin_dir/bin_deploy/zone/cfg/
rm -rf ~/myworld/cfg/res/*.bin
cp -f  ~/myworld/tools/db_to_bin/svr_res/* ~/myworld/cfg/res/
cp -f ~/myworld/cfg/res/* $bin_dir/bin_deploy/zone/cfg/res/
cp -f ~/myworld/cfg/fonts/* $bin_dir/bin_deploy/zone/cfg/fonts/
cp -f ~/myworld/cfg/dirty/*.txt $bin_dir/bin_deploy/zone/cfg/dirty/

cp -f ~/myworld/cfg/proto_ss.tdr $bin_dir/bin_deploy/

cp -rf ${game_comm_dir}/tools/*  $bin_dir/bin_deploy/tools/
cp -f ~/myworld/tools/tcm_monitor $bin_dir/bin_deploy/tools/
cp -f ~/myworld/tools/tcm_monitor.sh $bin_dir/bin_deploy/tools/
cp -f ~/myworld/tools/export_item/imp_item.txt $bin_dir/bin_deploy/tools/
cp -f ~/myworld/tools/rmshm.sh $bin_dir/bin_deploy/tools/
cp -f ~/myworld/c++/src/account_tool/account_tool $bin_dir/bin_deploy/tools/
cp -f ~/myworld/c++/src/account_tool/account_tool.ini $bin_dir/bin_deploy/tools/
cp -f /usr/local/mmog/tcm/TbusWorkDir/busssiness_66/conf/tbusmgr.xml  $bin_dir/bin_deploy/tools/
cp -f ~/myworld/cfg/tcm/mngsystem_tbus.xml $bin_dir/bin_deploy/tools/
cp -f ~/myworld/tools/db_mem_xml/db_mem_xml $bin_dir/bin_deploy/tools/
cp -f ~/myworld/tools/db_mem_xml/db_mem_xml.xml $bin_dir/bin_deploy/tools/
cp -f ~/myworld/cfg/db_meta/role_db_meta.tdr $bin_dir/bin_deploy/tools/
cp -rf ${game_comm_dir}/services/tbus_service/tbusd $bin_dir/bin_deploy/tools/
cp -f  ~/myworld/tools/unite_svr/unite_svr $bin_dir/bin_deploy/tools/
cp -f ~/myworld/tools/unite_svr/unite_svr.xml $bin_dir/bin_deploy/tools/
cp -f ~/myworld/tools/worldlist/worldlist $bin_dir/bin_deploy/tools/
cp -f ~/myworld/tools/worldlist/worldlist.xml $bin_dir/bin_deploy/tools/
cp -f ~/myworld/cfg/proto_cs.tdr $bin_dir/bin_deploy/tools/
cp -f ~/myworld/tools/account_dyn/account_dyn $bin_dir/bin_deploy/tools/
cp -f ~/myworld/tools/account_dyn/account_dyn.xml $bin_dir/bin_deploy/tools/
cp -f ~/myworld/tools/cdkey/cdkey $bin_dir/bin_deploy/tools/
cp -f ~/myworld/tools/cdkey/cdkey.xml $bin_dir/bin_deploy/tools/
cp -f ~/myworld/tools/gamestat/gamestat $bin_dir/bin_deploy/tools/
cp -f ~/myworld/tools/gamestat/gamestat.xml $bin_dir/bin_deploy/tools/
cp -f ~/myworld/tools/wid_check/wid_check $bin_dir/bin_deploy/tools/
cp -f ~/myworld/tools/wid_check/wid_check.xml $bin_dir/bin_deploy/tools/
cp -f ~/myworld/tools/active_code/active_code $bin_dir/bin_deploy/tools/
cp -f ~/myworld/tools/active_code/active_code.xml $bin_dir/bin_deploy/tools/
cp -f ~/myworld/tools/clean_world_log/cleanworldlog $bin_dir/bin_deploy/tools/
cp -f ~/myworld/src/world_scan/world_scan $bin_dir/bin_deploy/tools/
cp -f ~/myworld/src/cs_cgi/cs_cgi.xml $bin_dir/bin_deploy/tools/

cp -f ${game_comm_dir}/apps/tcm/bin/tcmconsole $bin_dir/bin_deploy/tcm/bin/
cp -f ${game_comm_dir}/apps/tcm/bin/tconnd  $bin_dir/bin_deploy/tcm/bin/tcm_conn
cp -f ${game_comm_dir}/apps/tcm/bin/tcmcenter $bin_dir/bin_deploy/tcm/bin/
cp -f ${game_comm_dir}/apps/tagent/bin/tagent $bin_dir/bin_deploy/tcm/bin/
cp -f ${game_comm_dir}/apps/tcenterd/bin/tcenterd $bin_dir/bin_deploy/tcm/bin/
cp -f ${game_comm_dir}/services/tlog_service/tconnddef.tdr $bin_dir/bin_deploy/tcm/cfg/
killall 10 tagent
cp -f ~/myworld/cfg/lib/mod_tdirty.so $bin_dir/bin_deploy/tcm/lib
cp -f ~/myworld/cfg/lib/mod_procmng.so.1.0.0 $bin_dir/bin_deploy/tcm/lib/
cp -f ~/myworld/cfg/lib/mod_tbusconfig.so.1.0.0 $bin_dir/bin_deploy/tcm/lib/

cp -f ~/myworld/cfg/world_list.cfg $bin_dir/bin_deploy/dir/cfg/
cp -f ~/myworld/cfg/black_white.cfg $bin_dir/bin_deploy/dir/cfg/
cp -f ~/myworld/cfg/proto_cs.tdr $bin_dir/bin_deploy/dir/cfg/

cp -f ~/myworld/cfg/db_meta/*.tdr $bin_dir/bin_deploy/db/db_meta/
cp -f ~/myworld/tools/clear_mails.sh $bin_dir/bin_deploy/db/
	
cp -f ${game_comm_dir}/services/tlog_service/*.sh  $bin_dir/bin_deploy/logsvr/cfg/
cp ${game_comm_dir}/services/tlog_service/*.xml  $bin_dir/bin_deploy/logsvr/cfg/
cp ${game_comm_dir}/services/tlog_service/*.tdr $bin_dir/bin_deploy/logsvr/cfg/
cp ~/myworld/cfg/op_log_meta.tdr $bin_dir/bin_deploy/logsvr/cfg/
cp ~/myworld/cfg/op_log.xml $bin_dir/bin_deploy/logsvr/cfg/

cp -f ~/myworld/tools/tcm.cfg $bin_dir/

~/myworld/tools/fetch_masks.sh
cp ~/myworld/cfg/res/*.msk $bin_dir/bin_deploy/
	
cp ~/myworld/comm_release/services/tbus_service/tbus* $bin_dir/bin_deploy/
	
Generate_Bin
Generate_Conn
webservice
web

if [ $1 = "all" ];then
	cp ~/myworld/cfg/tcm/* $bin_dir/bin_deploy/tcm/cfg/
	tgzname=${ProjectName}$2_bin_all
elif [ $1 = "bin" ];then
	tgzname=${ProjectName}$2_bin
fi

prefix="/usr/local/mmog/resdb_meta_version"

dest="${prefix}/resdb_meta_${2}.tdr"
cp ~/myworld/cfg/resdb_meta.tdr ${dest}

dest="${prefix}/resdb_meta_${2}.xml"
cp ~/myworld/src/meta_xml/resdb_meta.xml ${dest}

dest="${prefix}/proto_comm_${2}.xml"
cp ~/myworld/src/meta_xml/proto_comm.xml ${dest}

cd /usr/local/mmog/bin_pack

tar zcvf $tgzname.tgz bin_deploy
cp $tgzname.tgz ~/
cp -f ~/myworld/tools/install.sh ~/
cp -f ~/myworld/tools/install.sh $bin_dir/
rm -rf $bin_dir/bin_deploy

cd ~/myworld/src/meta_xml/
./boundversion.sh 10
cvs commit -m "Packed commit all meta xml files at $(date)"

