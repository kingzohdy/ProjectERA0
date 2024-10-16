#!/bin/sh

# <  ---  -lt
# <= ---  -le
# >  ---  -gt
# >= ---  -ge
# =  ---  -eq
# != ---  -ne

if [ $# -lt 1 ];then
	echo "error"
	exit -1;
fi

if [ $# = 2 ];then
	if [ $2 != "tui" ];then
		echo "error"	
		exit -1;
	fi
fi

pid=$(ps -ef|grep zone_svr |grep $1.61.1|awk '{print $2}');
echo "Zone_svr PID=$pid"


currusr=$(export | grep USER |awk -F '"' '{print $2}')
echo "Curr USR=$currusr"
currpusr=$(ps -ef|grep zone_svr |grep $1.61.1|awk '{print $1}')

if [ $currusr != $currpusr ];then
	echo "world_$1 USR is $currpusr"
	exit -1
fi

isgdb=$(ps -ef|grep gdb|grep $pid|awk '{print $10}')0

if [ $isgdb = ${pid}0 ];then
	echo "world_$1 is GDB"
	exit -1
else
	if [ $2 = "tui" ];then
	gdb -tui /usr/local/mmog/world_$1/zone/bin/zone_svr $pid
	else
	gdb  /usr/local/mmog/world_$1/zone/bin/zone_svr $pid
	fi
fi

