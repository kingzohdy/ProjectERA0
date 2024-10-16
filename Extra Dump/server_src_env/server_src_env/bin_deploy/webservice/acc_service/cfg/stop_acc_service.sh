#!/bin/sh

#../bin/acc_service --pid-file=../cfg/acc_service.pid stop
#pfile='../cfg/acc_service.pid';
for pfile in *.pid; do
	pid=`cat $pfile`;
	echo $pid;
	if [ '$pid' != '' ]; then
		kill -10 $pid;
	#	rm -r $pfile;
	fi
done


