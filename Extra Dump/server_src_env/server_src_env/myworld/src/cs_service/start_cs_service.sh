#!/bin/sh
rm -f ../bin/cs_service_log.xml
ARGF='../cfg/args.txt'
`../bin/cs_service --conf-path=../cfg --log-level=700 --log-path=../log --print-spawn-args=$ARGF start`
ARGS=`cat $ARGF`
echo $ARGS
spawn-fcgi $ARGS -- ../bin/cs_service  --conf-path=../cfg --log-level=700 --log-path=../log --daemon --pid-file=../cfg/cs_service.pid start
