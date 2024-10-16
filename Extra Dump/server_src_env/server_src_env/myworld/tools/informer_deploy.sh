#!/bin/sh

DEPOLY_DIR=~/myworld/tools/informer_deploy_tmp

rm -rf $DEPOLY_DIR
rm -f ./informer_deploy.tgz

if [ ! -d ${DEPOLY_DIR} ];then
	mkdir -p ${DEPOLY_DIR}/{dep,env/{tools,sql},myworld/{comm_release,src,tools}}
fi

cp -R  ~/myworld/comm_release/apps ${DEPOLY_DIR}/myworld/comm_release/
cp -R  ~/myworld/comm_release/include ${DEPOLY_DIR}/myworld/comm_release/
cp -R  ~/myworld/comm_release/lib ${DEPOLY_DIR}/myworld/comm_release/
cp -R  ~/myworld/comm_release/services ${DEPOLY_DIR}/myworld/comm_release/
cp -R  ~/myworld/comm_release/version ${DEPOLY_DIR}/myworld/comm_release/
cp -R ~/myworld/comm_release/tools/ ${DEPOLY_DIR}/myworld/comm_release/

cp -R ~/myworld/src/comm ${DEPOLY_DIR}/myworld/src/comm/
cp -R ~/myworld/src/dirty ${DEPOLY_DIR}/myworld/src/dirty/
cp -R ~/myworld/src/lib ${DEPOLY_DIR}/myworld/src/lib/
cp -R ~/myworld/src/auth_svr ${DEPOLY_DIR}/myworld/src/auth_svr/
cp  /usr/local/mysql/lib/mysql/*  ${DEPOLY_DIR}/myworld/src/lib/
cp -R ~/myworld/src/Makefile ${DEPOLY_DIR}/myworld/src/
mkdir -p ${DEPOLY_DIR}/myworld/src/zone/zone_svr/
cp  ~/myworld/src/zone/zone_svr/*.h  ${DEPOLY_DIR}/myworld/src/zone/zone_svr/
cp -R ~/myworld/Makefile ${DEPOLY_DIR}/myworld/
cp -R ~/myworld/src/mmog.mk ${DEPOLY_DIR}/myworld/src/
cp -R ~/myworld/src/client_ex ${DEPOLY_DIR}/myworld/src/client_ex/
cd ${DEPOLY_DIR}/myworld/src/
sed -i 's/myworld/\/myworld\/tools\/informer_deploy_tmp\/myworld/g' mmog.mk 
cd ~/myworld/tools/
cp -R ~/myworld/src/informer_svr/ ${DEPOLY_DIR}/myworld/src/informer_svr/ 


tar zcvf informer_deploy.tgz informer_deploy_tmp

rm -rf ${DEPOLY_DIR}