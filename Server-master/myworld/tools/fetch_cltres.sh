#!/bin/sh


cd ~/myworld/tools/db_to_bin/
./db_to_bin

if [ $? != "0" ];then
        echo "db_to_bin error!"
        exit -1
fi

cd ~/myworld/tools/db_to_bin/clt_res/

smbclient //192.168.0.4/share_write -U jerry -N << EOF

cd ³ÌÐò¹²Ïí/cltres
prompt
mask *.bin
mput *
exit
EOF

