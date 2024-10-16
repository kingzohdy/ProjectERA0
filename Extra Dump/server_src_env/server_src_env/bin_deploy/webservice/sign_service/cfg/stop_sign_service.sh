#!/bin/sh

#../bin/sign_service --pid-file=../cfg/sign_service.pid stop
#pfile='../cfg/sign_service.pid';
for pfile in *.pid; do
	pid=`cat $pfile`;
	echo $pid;
	if [ '$pid' != '' ]; then
		kill -10 $pid;
	#	rm -r $pfile;
	fi
done



