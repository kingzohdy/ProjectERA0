#!/bin/sh

DEPOLY_DIR=~/myworld/tools/cs_cgi_tmp

rm -rf ${DEPOLY_DIR}
rm -f ./cs_cgi.tgz

if [ ! -d ${DEPOLY_DIR} ];then
        mkdir -p ${DEPOLY_DIR}
fi

cp ~/myworld/doc/资源数据库表说明.xls ${DEPOLY_DIR}/
cp ~/myworld/doc/运营日志数据库表说明.xls ${DEPOLY_DIR}/
cp ~/myworld/doc/运营开发/gmtools/客服及数据挖掘开发需求.doc ${DEPOLY_DIR}/
cp ~/myworld/src/cs_cgi/cs_cgi.h ${DEPOLY_DIR}/
cp ~/myworld/src/cs_cgi/proto_comm.h ${DEPOLY_DIR}/
cp ~/myworld/src/cs_cgi/proto_ss.h ${DEPOLY_DIR}/
cp ~/myworld/src/cs_cgi/proto_ss.tdr ${DEPOLY_DIR}/
cp ~/myworld/src/cs_cgi/cs_test.c ${DEPOLY_DIR}/
cp  ~/myworld/src/cs_cgi/cs_cgi.xml ${DEPOLY_DIR}/
cp ~/myworld/src/cs_cgi/*.a ${DEPOLY_DIR}/
cp ~/myworld/src/cs_cgi/cs_test.mk ${DEPOLY_DIR}/Makefile

tar zcvf cs_cgi.tgz cs_cgi_tmp
rm -rf ${DEPOLY_DIR}
