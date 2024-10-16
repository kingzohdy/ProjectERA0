#!/bin/sh

#../bin/cs_service --pid-file=../cfg/cs_service.pid stop
pfile='../cfg/cs_service.pid';
pid=`cat $pfile`;
echo $pid;
if [ '$pid' != '' ]; then
	kill -10 $pid;
	#rm $pfile;
fi

