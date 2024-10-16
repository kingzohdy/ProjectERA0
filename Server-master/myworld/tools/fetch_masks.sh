#!/bin/sh

su -l eric -c 'cd /data/shared/maps; svn up' << EOF
enjoy eric
EOF

rm -rf ${HOME}/myworld/cfg/res/*.msk
find /data/shared/maps -name *.msk | xargs -i cp {} ${HOME}/myworld/cfg/res/
echo "Fetch masks done."
