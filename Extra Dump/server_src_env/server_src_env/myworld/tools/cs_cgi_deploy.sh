#!/bin/sh

DEPOLY_DIR=~/myworld/tools/cs_cgi_tmp

rm -rf ${DEPOLY_DIR}
rm -f ./cs_cgi.tgz

if [ ! -d ${DEPOLY_DIR} ];then
        mkdir -p ${DEPOLY_DIR}
fi

cp ~/myworld/doc/��Դ���ݿ��˵��.xls ${DEPOLY_DIR}/
cp ~/myworld/doc/��Ӫ��־���ݿ��˵��.xls ${DEPOLY_DIR}/
cp ~/myworld/doc/��Ӫ����/gmtools/�ͷ��������ھ򿪷�����.doc ${DEPOLY_DIR}/
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
