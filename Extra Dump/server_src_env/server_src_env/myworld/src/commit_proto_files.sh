#!/bin/sh

proxy_do()
{
su -l eric -c "${1}" << EOF
enjoy eric
EOF
}

cd ~/myworld/src/meta_xml
cvs up
make
cd ~/myworld/src/comm
make clean && make


PREFIX="${HOME}/myworld/"
DEST="/data/shared/proto"
ename=$(awk -F: '{print $1 " " $3}' /etc/passwd | grep " $UID$" | awk '{print $1}')
if [[ ${ename} == "test" ]]; then
        DEST="/data/shared/deploy/proto"
fi

proxy_do "cd ${DEST}; svn up"
proxy_do "cd ${PREFIX}/src/comm; cp -f proto_comm.h proto_cs.h resdb_meta.h proto_cs.meta ${DEST}/"
proxy_do "cp -f ${PREFIX}/cfg/resdb_meta.tdr ${DEST}/"

proxy_do "cd ${DEST}; svn add * 2>/dev/null; svn commit -m \"Commit by ${USER}\""

