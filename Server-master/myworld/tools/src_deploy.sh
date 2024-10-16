#!/bin/sh

if [ $# != 1 ];then
        echo "parameter errer!"
        echo "./src_deploy.sh 0_0_23"
        exit -1
fi

time=`date +%y_%m_%d_%H_%M`

DEPOLY_DIR=~/myworld/tools/src_deploy_tmp

rm -rf $DEPOLY_DIR
rm -f ./src_depoly.tgz

if [ ! -d ${DEPOLY_DIR} ];then
	mkdir -p ${DEPOLY_DIR}/{dep,res/{resdb,bin,script,msk,dirty,db_meta},myworld/{comm_release,src,tools,robot}}
fi

cd ~/myworld/src/zone/zone_svr/
make clean;make

cp -R ~/myworld/doc/源代码包说明.doc ${DEPOLY_DIR}/
cp -R ~/myworld/doc/运维文档/重要进程配置文件.doc ${DEPOLY_DIR}/ 
cp -R ~/myworld/dep/* ${DEPOLY_DIR}/dep/
cp ~/myworld/src/zone/zone_svr/zone_svr ${DEPOLY_DIR}/
cp ~/myworld/cfg/res/mmogerr.xml ${DEPOLY_DIR}/res/

#cp -R /usr/local/mmog/start_cluster.sh ${DEPOLY_DIR}/env/
#cp -R /usr/local/mmog/stop_cluster.sh ${DEPOLY_DIR}/env/
#cp -R /usr/local/mmog/start_region.sh ${DEPOLY_DIR}/env/
#cp -R /usr/local/mmog/stop_region.sh ${DEPOLY_DIR}/env/
#cp -R /usr/local/mmog/start_world.sh ${DEPOLY_DIR}/env/
#cp -R /usr/local/mmog/stop_world.sh ${DEPOLY_DIR}/env/
#cp -R /usr/local/mmog/tools/tbusmgr ${DEPOLY_DIR}/env/tools/
#cp -R ~/myworld/tools/db_mem_xml/db_mem_xml ${DEPOLY_DIR}/env/tools/
#cp -R ~/myworld/cfg/db_meta/role_db_meta.tdr ${DEPOLY_DIR}/env/tools/
#cp -R ~/myworld/tools/db_mem_xml/db_mem_xml.xml ${DEPOLY_DIR}/env/tools/
#cp -R /usr/local/mmog/tcm/TbusWorkDir/busssiness_66/conf/tbusmgr.xml ${DEPOLY_DIR}/env/tools/

#echo "start copy env"
#rm -rf /usr/local/mmog/world_5/db/log/*
#rm -rf /usr/local/mmog/world_5/zone/log/*
#rm -rf /usr/local/mmog/world_5/logsvr/log/*
#rm -rf /usr/local/mmog/world_5/logsvr/local/*
#rm -rf /usr/local/mmog/world_5/logsvr/oplog/*
#cp -R /usr/local/mmog/world_5  ${DEPOLY_DIR}/env/
#cp -f ./src_deploy/zone_svr.xml ${DEPOLY_DIR}/env/world_5/zone/cfg/
#rm -rf /usr/local/mmog/region/log/*
#cp -R /usr/local/mmog/region ${DEPOLY_DIR}/env/
#rm -rf /usr/local/mmog/cluster/log/*
#cp -R /usr/local/mmog/cluster ${DEPOLY_DIR}/env/
#cp -f ./src_deploy/start_misc_db.sh ${DEPOLY_DIR}/env/world_5/db/cfg/
#cp -f ./src_deploy/start_role_db.sh ${DEPOLY_DIR}/env/world_5/db/cfg/
#cp -f ./src_deploy/start_zone_conn.sh ${DEPOLY_DIR}/env/world_5/zone/cfg/
#cp -f ./src_deploy/start_zone_svr.sh ${DEPOLY_DIR}/env/world_5/zone/cfg/

echo "start db to bin"
cd ~/myworld/tools/db_to_bin
./db_to_bin 

~/myworld/tools/fetch_script.sh

cp ./svr_res/* ${DEPOLY_DIR}/res/bin/
cd ~/myworld/tools/
./create_resdb_data.sh
cp ~/myworld/tools/resdb_data.sql ${DEPOLY_DIR}/res/resdb/
~/myworld/tools/fetch_res_xls.sh
cp /data/资源数据表/资源数据库表说明.xls ${DEPOLY_DIR}/res/resdb/
cp ~/myworld/cfg/db_meta/*  ${DEPOLY_DIR}/res/db_meta/
./fetch_masks.sh
cp ~/myworld/cfg/res/*.msk ${DEPOLY_DIR}/res/msk/
#cp /usr/local/mmog/script_deploy/* ${DEPOLY_DIR}/res/script/
cp /data/shared/script/*  ${DEPOLY_DIR}/res/script/
cp  ~/myworld/cfg/dirty/* ${DEPOLY_DIR}/res/dirty/
cp ~/myworld/cfg/resdb_meta.tdr ${DEPOLY_DIR}/res/


rm -f ~/myworld/tools/*.sql
cd ~/myworld/tools/
#~/myworld/tools/create_dbtab_sql.sh
#cp ~/myworld/tools/*.sql ${DEPOLY_DIR}/env/sql/
#cp ~/myworld/tools/create_account.sh ${DEPOLY_DIR}/env/sql/
#cp ~/myworld/tools/create_logdb.sh  ${DEPOLY_DIR}/env/sql/
#cp ~/myworld/tools/create_online.sh  ${DEPOLY_DIR}/env/sql/
#cp ~/myworld/tools/create_roledb.sh  ${DEPOLY_DIR}/env/sql/
cp ./src_deploy/install.sh ${DEPOLY_DIR}/
chmod +x ${DEPOLY_DIR}/install.sh

cp -R  ~/myworld/comm_release/apps ${DEPOLY_DIR}/myworld/comm_release/
cp -R  ~/myworld/comm_release/include ${DEPOLY_DIR}/myworld/comm_release/
cp -R  ~/myworld/comm_release/lib ${DEPOLY_DIR}/myworld/comm_release/
cp -R  ~/myworld/comm_release/services ${DEPOLY_DIR}/myworld/comm_release/
cp -R  ~/myworld/comm_release/version ${DEPOLY_DIR}/myworld/comm_release/
cp -R ~/myworld/comm_release/tools/ ${DEPOLY_DIR}/myworld/comm_release/

cd ~/myworld/src/zone
make clean;

cd ~/myworld/tools/

cp -R ~/myworld/src/benchmark ${DEPOLY_DIR}/myworld/src/
cp -R ~/myworld/src/client_ex ${DEPOLY_DIR}/myworld/src/
cp -R ~/myworld/src/comm ${DEPOLY_DIR}/myworld/src/
rm -rf ${DEPOLY_DIR}/myworld/src/confuse.c
rm -rf ${DEPOLY_DIR}/myworld/src/dirreport.c
rm -rf ${DEPOLY_DIR}/myworld/src/mapmask.c
rm -rf ${DEPOLY_DIR}/myworld/src/og_script.c
rm -rf ${DEPOLY_DIR}/myworld/src/poly.c
rm -rf ${DEPOLY_DIR}/myworld/src/ptgenimg.c
rm -rf ${DEPOLY_DIR}/myworld/src/tlib_dirty_check.c
rm -rf ${DEPOLY_DIR}/myworld/src/sig_key.c
cp -R ~/myworld/src/lib ${DEPOLY_DIR}/myworld/src/
cp -R ~/myworld/src/meta_xml ${DEPOLY_DIR}/myworld/src/
cp -R ./src_deploy/mmog.mk ${DEPOLY_DIR}/myworld/src/
cp -R ~/myworld/src/zone ${DEPOLY_DIR}/myworld/src/
cp -f ~/myworld/tools/src_deploy/Makefile_src ${DEPOLY_DIR}/myworld/src/Makefile
cp -f ~/myworld/tools/src_deploy/Makefile_xml ${DEPOLY_DIR}/myworld/src/meta_xml/Makefile
cp -f ~/myworld/tools/src_deploy/Makefile_zone ${DEPOLY_DIR}/myworld/src/zone/Makefile
cp -f ~/myworld/tools/src_deploy/Makefile_zone_svr ${DEPOLY_DIR}/myworld/src/zone/zone_svr/Makefile
cp -f ~/myworld/src/auth_svr/auth_svr.h ${DEPOLY_DIR}/myworld/src//zone/zone_svr/

rm -f ${DEPOLY_DIR}/myworld/src/zone/zone_svr/zone_move_in.c

cp -R ~/myworld/tools/db_to_bin ${DEPOLY_DIR}/myworld/tools/
cp -f ~/myworld/myworld/cfg/resdb_meta.tdr ${DEPOLY_DIR}/myworld/tools/db_to_bin/
cp ./src_deploy/db_to_bin.xml ${DEPOLY_DIR}/myworld/tools/db_to_bin/

cp -f ~/myworld/src/comm/version.h ~/myworld/myworld_robot/src/comm/
cp -f ~/myworld/src/comm/cs_net.h ~/myworld/myworld_robot/src/comm/
cp -f ~/myworld/src/comm/cs_net.c ~/myworld/myworld_robot/src/comm/
cp -f ~/myworld/src/comm/proto_comm.h ~/myworld/myworld_robot/src/comm/
cp -f ~/myworld/src/comm/proto_cs.h ~/myworld/myworld_robot/src/comm/
cp -f ~/myworld/src/comm/resdb_meta.h ~/myworld/myworld_robot/src/comm/
cp -f ~/myworld/src/comm/proto_cs.meta ~/myworld/myworld_robot/src/comm/
cp -f ~/myworld/src/comm/id_rsa_pub.c ~/myworld/myworld_robot/src/comm/
cp -f ~/myworld/src/comm/id_rsa.c ~/myworld/myworld_robot/src/comm/
cp -f ~/myworld/src/comm/grav_rsa_types.h ~/myworld/myworld_robot/src/comm/
cp -f ~/myworld/tools/robot/robot_conf_desc.h ~/myworld/myworld_robot/tools/robot/
cp -f ~/myworld/tools/robot/robot_conf_desc.c ~/myworld/myworld_robot/tools/robot/
cp -f ~/myworld/tools/robot/robot.xml ~/myworld/myworld_robot/tools/robot/
cp -f ~/myworld/tools/robot/robot.xml ~/myworld/myworld_robot/tools/robot/win32/robot/
cp -f ~/myworld/tools/robot/robot.c ~/myworld/myworld_robot/tools/robot/
cp -f -R ~/myworld/myworld_robot ${DEPOLY_DIR}/myworld/robot

tar zcvf src_deploy_$1_${time}.tgz src_deploy_tmp 

rm -rf ${DEPOLY_DIR}

mv src_deploy_$1_${time}.tgz ~/

cp ~/src_deploy_$1_${time}.tgz /usr/local/mmog/code_bak/

