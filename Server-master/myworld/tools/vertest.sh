#!/bin/sh

if [ $# = 2 ];then
	ProjectName="myworld"
else
	echo "parameter errer!"
	echo "./version.sh all|bin version"
	exit -1
fi
#resdb_name=resdb_$2

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

#cd ~/myworld/src/comm/
#sedit version.h << EOF
#        y
#EOF
#~/myworld/tools/ver.sh $2
#cvs commit -m version.h

#su -c 'rm /data/cvs/cvsroot/myworld/src/comm/Locks/version.h*'<< FFF
#hello#sun
#FFF

cd ~/myworld/src/
cvs up
make clean;make

cd ~/myworld/tools/
./fetch_cltres.sh

if [ $? != "0" ];then
	echo "db_to_bin error!"
	exit -1
fi

time=`date +%y_%m_%d_%H_%M`
bak_dir="/usr/local/mmog/code_bak"
bin_dir="/usr/local/mmog/bin_pack"
game_comm_dir=${HOME}/myworld/comm_release


#/usr/local/bin/mysqldump -uusrwrite -pusrwrite --add-drop-database --add-drop-table --default-character-set=latin1  resdb >${bak_dir}/resdb_$2_$time.sql
#tar zcvf $bak_dir/${ProjectName}_bak_$2_$time.tgz ~/$ProjectName

cd ~/$ProjectName

#cvs tag tag_$time
#cvs rtag -b -r tag_$time tag_${time}_01 $ProjectName

rm -rf ${bin_dir}/bin_deploy
rm -rf ${bin_dir}/cs_interface

mkdir -p ${bin_dir}/bin_deploy/{sql,tools,informer_tool,resdb,tcm/{bin,cfg,lib,log},dir/cfg,db/db_meta,zone/{bin,cfg,script,cfg/{db_meta,res,fonts,dirty}},logsvr/{cfg,res_bin},version,bill,auth,informer,aas,region,accdb,mud,battle,recv/}
mkdir -p ${bin_dir}/cs_interface/{windows,linux,doc}

Generate_Conn()
{
	cp ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/zone/bin/
	cp ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/cs_conn
        cp ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/dir/dir_svr_conn
        cp ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/dir/dir_bak_conn
        cp ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/dir/dir_slave_conn
	cp ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/dir/dir_slave1_conn
	cp ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/auth/auth_conn
	cp ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/auth/auth_bak_conn
	cp ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/auth/auth_slave_conn
	cp ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/version/version_conn
	cp ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/version/version_bak_conn
	cp ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/version/version_slave_conn
	cp ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/version/version_slave1_conn
	cp ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/version/account_cs_conn
	cp ~/myworld/src/zone/zone_conn/zone_conn $bin_dir/bin_deploy/mud/mud_conn
}

Generate_Bin()
{
	cp ~/myworld/src/zone/zone_ctrl/zone_ctrl $bin_dir/bin_deploy/zone/bin/
        cp ~/myworld/src/zone/zone_svr/zone_svr $bin_dir/bin_deploy/zone/bin/
	cp ~/myworld/src/dir_svr/dir_svr $bin_dir/bin_deploy/dir/
        cp ~/myworld/src/dir_svr/dir_svr $bin_dir/bin_deploy/dir/dir_bak
        cp ~/myworld/src/dir_svr/dir_svr  $bin_dir/bin_deploy/dir/dir_slave
	cp ~/myworld/src/dir_svr/dir_svr  $bin_dir/bin_deploy/dir/dir_slave1
	cp ~/myworld/src/zone/cs_svr/cs_svr $bin_dir/bin_deploy/
	cp ${game_comm_dir}/apps/torm/bin/tormsvr $bin_dir/bin_deploy/db/role_db
        cp ${game_comm_dir}/apps/torm/bin/tormsvr $bin_dir/bin_deploy/db/misc_db
	cp ${game_comm_dir}/services/tlog_service/tlogd $bin_dir/bin_deploy/logsvr/
        cp ${game_comm_dir}/services/tlog_service/logdump $bin_dir/bin_deploy/logsvr/
	cp ${HOME}/myworld/src/tlog_db/tlog_db $bin_dir/bin_deploy/logsvr/
	cp ${HOME}/myworld/comm_release/apps/torm/bin/tormsvr $bin_dir/bin_deploy/accdb/account_db
        cp ${HOME}/myworld/comm_release/apps/torm/bin/tormsvr $bin_dir/bin_deploy/bill/bill_db
        cp ~/myworld/src/aas_svr/aas_svr $bin_dir/bin_deploy/aas/
        cp ~/myworld/src/auth_svr/auth_svr $bin_dir/bin_deploy/auth/auth_svr
	cp ~/myworld/src/auth_svr/auth_svr $bin_dir/bin_deploy/auth/auth_bak
	cp ~/myworld/src/auth_svr/auth_svr $bin_dir/bin_deploy/auth/auth_slave
        cp ~/myworld/src/informer_svr/informer_svr $bin_dir/bin_deploy/informer/
        cp ~/myworld/src/version_svr/version_svr $bin_dir/bin_deploy/version/version_svr
	cp ~/myworld/src/version_svr/version_svr $bin_dir/bin_deploy/version/version_bak
	cp ~/myworld/src/version_svr/version_svr $bin_dir/bin_deploy/version/version_slave
	cp ~/myworld/src/version_svr/version_svr $bin_dir/bin_deploy/version/version_slave1
	cp ~/myworld/src/account_cs_svr/account_cs_svr $bin_dir/bin_deploy/version/account_cs_svr
	cp ~/myworld/src/mud_svr/mud_svr $bin_dir/bin_deploy/mud/mud_svr
        cp ~/myworld/src/region_gate/region_gate  $bin_dir/bin_deploy/region/
        cp ~/myworld/src/bill_svr/bill_svr  $bin_dir/bin_deploy/bill/

#	cp -R ~/myworld/comm_release/services/agent_client $bin_dir/bin_deploy/
#	cp -R ~/myworld/comm_release/services/agent_server $bin_dir/bin_deploy/
	cp ~/myworld/src/recv_report/recv_report $bin_dir/bin_deploy/recv/recv_report

	cp ~/myworld/src/zone/zone_svr/zone_svr $bin_dir/bin_deploy/battle/battle_svr
	cp ${game_comm_dir}/apps/torm/bin/tormsvr $bin_dir/bin_deploy/db/region_db
}

Generate_Sql()
{
	cd ~/myworld/tools/
        ./create_dbtab_sql.sh
	./create_log_sql.sh
	./create_resdb_data.sh

	#resdb_name=resdb_$2
#mysql -uversion -pversion << EOF
#        create database $resdb_name;
#        QUIT
#EOF
#	mysql -uversion -pversion $resdb_name <resdb_data.sql


        cp ~/myworld/tools/*.sql $bin_dir/bin_deploy/sql/
	cp  ~/myworld/tools/create_*.sh $bin_dir/bin_deploy/sql/
	rm -rf $bin_dir/bin_deploy/sql/create_dbtab_sql.sh
	rm -rf $bin_dir/bin_deploy/sql/create_log_sql.sh
	rm -rf $bin_dir/bin_deploy/sql/create_resdb_data.sh
	rm -rf $bin_dir/bin_deploy/sql/create_res_sql.sh
	cp ~/myworld/tools/resdb_data.sql $bin_dir/bin_deploy/resdb/
	./fetch_res_xls.sh
	cp /data/资源数据表/资源数据库表说明.xls $bin_dir/bin_deploy/resdb/		
}

Generate_Cs_interface()
{
	cp -R ~/myworld/tools/win32_cs_cgi_demos $bin_dir/cs_interface/windows/
	rm -rf $bin_dir/cs_interface/windows/win32_cs_cgi_demos/cs_cgi_dll_demo/Debug
	rm -rf $bin_dir/cs_interface/windows/win32_cs_cgi_demos/cs_cgi_dll_demo/Release
	cd ~/myworld/src/cs_cgi/
	make clean;make
	cp -f ~/myworld/src/cs_cgi/*.tdr  $bin_dir/cs_interface/linux/
	cp -f ~/myworld/src/cs_cgi/*.xml  $bin_dir/cs_interface/linux/
	cp -f ~/myworld/src/cs_cgi/*.a  $bin_dir/cs_interface/linux/
	cp -f ~/myworld/src/cs_cgi/*.c  $bin_dir/cs_interface/linux/
	cp -f ~/myworld/src/cs_cgi/*.h  $bin_dir/cs_interface/linux/
	cp -f ~/myworld/src/comm/cs_cgi.c $bin_dir/cs_interface/windows/win32_cs_cgi_demos/cs_cgi_dll/
	cp -f ~/myworld/src/comm/cs_cgi.h $bin_dir/cs_interface/windows/win32_cs_cgi_demos/cs_cgi_dll/
	cp -f ~/myworld/src/cs_cgi/cs_cgi.xml $bin_dir/cs_interface/windows/win32_cs_cgi_demos/cs_account_demo/
	cp ~/myworld/src/cs_cgi/Makefile_version $bin_dir/cs_interface/linux/Makefile
	cp -f ~/myworld/doc/运营开发/gmtools/客服及数据挖掘开发需求.doc $bin_dir/cs_interface/doc/
	cp -f ~/myworld/doc/运营日志数据库表说明.xls $bin_dir/cs_interface/doc/
}

informer()
{
	cp -f /usr/lib/libifm2_32.so $bin_dir/bin_deploy/informer_tool/
 cp -f ~/myworld/src/informer_svr/active_config.ini $bin_dir/bin_deploy/informer_tool/
 cp -f ~/myworld/src/informer_svr/charge_config.ini $bin_dir/bin_deploy/informer_tool/
 cp -f ~/myworld/src/informer_svr/member_config.ini $bin_dir/bin_deploy/informer_tool/
 cp -R ~/myworld/src/informer_svr/key $bin_dir/bin_deploy/informer_tool/
	#cp -f /usr/lib/libcryptopp.so $bin_dir/bin_deploy/informer_tool/
	#cp -R ~/myworld/ext/agent_server $bin_dir/bin_deploy/informer_tool/
	#cp -R ~/myworld/ext/agent_client $bin_dir/bin_deploy/informer_tool/
	#cp -R ~/myworld/ext/agent_charge $bin_dir/bin_deploy/informer_tool/
	

	#chmod -R 777 /dev/shm/
}

~/myworld/tools/fetch_script.sh 

cp -R ~/myworld/doc/运维文档 $bin_dir/bin_deploy/
cp ~/myworld/doc/服务器架构设计说明书.doc $bin_dir/bin_deploy/运维文档/
cp ~/myworld/tools/start_cluster.sh $bin_dir/bin_deploy/
cp ~/myworld/tools/start_world.sh $bin_dir/bin_deploy/
cp ~/myworld/tools/stop_cluster.sh $bin_dir/bin_deploy/
cp ~/myworld/tools/stop_world.sh $bin_dir/bin_deploy/
cp ~/myworld/tools/replace.sh $bin_dir/bin_deploy/
cp ~/myworld/comm_release/lib/libmysqlclient.so.15.0.0 $bin_dir/bin_deploy/

Generate_Sql

cd ~/$ProjectName
	
cp -f /data/shared/script/* $bin_dir/bin_deploy/zone/script/
cp ~/myworld/cfg/db_meta/*.tdr $bin_dir/bin_deploy/zone/cfg/db_meta/
cp ~/myworld/cfg/proto_cs.tdr $bin_dir/bin_deploy/zone/cfg/
cp ~/myworld/cfg/proto_ss.tdr $bin_dir/bin_deploy/zone/cfg/
cp ~/myworld/cfg/zone_shopping.xml $bin_dir/bin_deploy/zone/cfg/
cp ~/myworld/cfg/resdb_meta.tdr $bin_dir/bin_deploy/zone/cfg/
cp ~/myworld/cfg/black_white.cfg $bin_dir/bin_deploy/zone/cfg/
cp -f  ~/myworld/tools/db_to_bin/svr_res/* ~/myworld/cfg/res/
cp ~/myworld/cfg/res/* $bin_dir/bin_deploy/zone/cfg/res/
cp ~/myworld/cfg/fonts/* $bin_dir/bin_deploy/zone/cfg/fonts/
cp ~/myworld/cfg/dirty/* $bin_dir/bin_deploy/zone/cfg/dirty/

cp ~/myworld/cfg/proto_ss.tdr $bin_dir/bin_deploy/

cp -rf ${game_comm_dir}/tools/*  $bin_dir/bin_deploy/tools/
cp -f ~/myworld/tools/rmshm.sh $bin_dir/bin_deploy/tools/
cp -f ~/myworld/c++/src/account_tool/account_tool $bin_dir/bin_deploy/tools/
cp -f ~/myworld/c++/src/account_tool/account_tool.ini $bin_dir/bin_deploy/tools/
cp -f /usr/local/mmog/tcm/TbusWorkDir/busssiness_66/conf/tbusmgr.xml  $bin_dir/bin_deploy/tools/
cp ~/myworld/cfg/tcm/mngsystem_tbus.xml $bin_dir/bin_deploy/tools/
cp ~/myworld/tools/db_mem_xml/db_mem_xml $bin_dir/bin_deploy/tools/
cp ~/myworld/tools/db_mem_xml/db_mem_xml.xml $bin_dir/bin_deploy/tools/
cp ~/myworld/cfg/db_meta/role_db_meta.tdr $bin_dir/bin_deploy/tools/
cp -rf ${game_comm_dir}/services/tbus_service/tbusd $bin_dir/bin_deploy/tools/

cp ${game_comm_dir}/apps/tcm/bin/tcmconsole $bin_dir/bin_deploy/tcm/bin/
cp ${game_comm_dir}/apps/tcm/bin/tconnd  $bin_dir/bin_deploy/tcm/bin/tcm_conn
cp ${game_comm_dir}/apps/tcm/bin/tcmcenter $bin_dir/bin_deploy/tcm/bin/
cp ${game_comm_dir}/apps/tagent/bin/tagent $bin_dir/bin_deploy/tcm/bin/
cp ${game_comm_dir}/apps/tcenterd/bin/tcenterd $bin_dir/bin_deploy/tcm/bin/
cp ${game_comm_dir}/services/tlog_service/tconnddef.tdr $bin_dir/bin_deploy/tcm/cfg/
killall 10 tagent
cp ~/myworld/cfg/lib/mod_tdirty.so $bin_dir/bin_deploy/tcm/lib
cp ~/myworld/cfg/lib/mod_procmng.so.1.0.0 $bin_dir/bin_deploy/tcm/lib/
cp ~/myworld/cfg/lib/mod_tbusconfig.so.1.0.0 $bin_dir/bin_deploy/tcm/lib/

cp ~/myworld/cfg/world_list.cfg $bin_dir/bin_deploy/dir/cfg/
cp ~/myworld/cfg/black_white.cfg $bin_dir/bin_deploy/dir/cfg/
cp ~/myworld/cfg/proto_cs.tdr $bin_dir/bin_deploy/dir/cfg/

cp ~/myworld/cfg/db_meta/*.tdr $bin_dir/bin_deploy/db/db_meta/
cp ~/myworld/tools/clear_mails.sh $bin_dir/bin_deploy/db/
	
cp ${game_comm_dir}/services/tlog_service/*.sh  $bin_dir/bin_deploy/logsvr/cfg/
cp ${game_comm_dir}/services/tlog_service/*.xml  $bin_dir/bin_deploy/logsvr/cfg/
cp ${game_comm_dir}/services/tlog_service/*.tdr $bin_dir/bin_deploy/logsvr/cfg/
cp ~/myworld/cfg/op_log_meta.tdr $bin_dir/bin_deploy/logsvr/cfg/
cp ~/myworld/cfg/op_log.xml $bin_dir/bin_deploy/logsvr/cfg/

cp -R ~/myworld/tools/tcm.cfg $bin_dir/

~/myworld/tools/fetch_masks.sh
cp ~/myworld/cfg/res/*.msk $bin_dir/bin_deploy/
	
cp ~/myworld/comm_release/services/tbus_service/tbus* $bin_dir/bin_deploy/
	
Generate_Bin
Generate_Conn
Generate_Cs_interface
informer

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
cp -f ~/myworld/tools/install.sh $bin_dir/
rm -rf $bin_dir/bin_deploy

tgzname=cs_interface${2}_bin
tar zcvf $tgzname.tgz cs_interface
cp $tgzname.tgz ~/
rm -rf $bin_dir/cs_interface



