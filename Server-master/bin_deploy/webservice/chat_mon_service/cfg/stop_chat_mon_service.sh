#!/bin/sh

#../bin/chat_mon_service --pid-file=../cfg/chat_mon_service.pid stop
#pfile='../cfg/chat_mon_service.pid';
for pfile in *.pid; do
	pid=`cat $pfile`;
	echo $pid;
	if [ '$pid' != '' ]; then
		kill -10 $pid;
	#	rm -r $pfile;
	fi
done



