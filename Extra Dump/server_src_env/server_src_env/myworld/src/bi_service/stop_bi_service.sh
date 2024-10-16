#!/bin/sh

#../bin/bi_service --pid-file=../cfg/bi_service.pid stop
#pfile='../cfg/bi_service.pid';
for pfile in *.pid; do
	pid=`cat $pfile`;
	echo $pid;
	if [ '$pid' != '' ]; then
		kill -10 $pid;
		#rm -r $pfile;
	fi
done


