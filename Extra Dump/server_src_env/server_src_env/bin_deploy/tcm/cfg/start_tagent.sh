#!/bin/sh

#start_tagent.sh

echo "start tagent"
ulimit -c unlimited
ulimit -n 20400
cd /usr/local/mmog/tcm/cfg
../bin/tagent
