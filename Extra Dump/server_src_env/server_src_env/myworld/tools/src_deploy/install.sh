#!/bin/sh

if [[ -z $1 ]]
then
	world="world"
else
	world=$1
fi		

cp -f ./res/bin/* /usr/local/mmog/$world/zone/cfg/res/
cp -f ./res/db_meta/* /usr/local/mmog/$world/zone/cfg/db_meta/
cp -f ./res/db_meta/* /usr/local/mmog/$world/db/cfg/db_meta/
cp -f ./res/msk/* /usr/local/mmog/$world/zone/cfg/res/
cp -f ./res/script/* /usr/local/mmog/$world/zone/cfg/script/
cp -f ./res/dirty/* /usr/local/mmog/$world/zone/cfg/dirty/
cp -f ./res/*.tdr /usr/local/mmog/$world/zone/cfg/
cp -f ./zone_svr /usr/local/mmog/$world/zone/bin/
cp -f ./res/mmogerr.xml /usr/local/mmog/$world/zone/cfg/res/

