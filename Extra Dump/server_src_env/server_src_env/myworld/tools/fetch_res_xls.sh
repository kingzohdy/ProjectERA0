#!/bin/sh

su -l eric -c 'cd /data/资源数据表; svn up' <<EOF
enjoy eric
EOF

su -l eric -c 'cd /data/脚本指令说明; svn up' <<EOF
enjoy eric
EOF


echo "Fetch 资源数据表 done."

