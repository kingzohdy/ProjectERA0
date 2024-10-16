#!/bin/sh


su -l eric -c 'cd /data/shared/script; svn up' << EOF
enjoy eric
EOF

echo "Fetch script done."
