#!/bin/sh
rm -f ../bin/sign_service_log*.xml
ARGF='../cfg/args.txt'

startservice()
{
	ID=$1
	BID=''
	if [ "$ID" != "" ]; then
		BID="--business-id="$ID
		ID="_"$ID;
	fi
	`../bin/sign_service $BID --conf-path=../cfg --log-level=600 --log-path=../log --print-spawn-args=$ARGF start > /dev/null`
	ARGS=`cat $ARGF`
	echo $ARGS
	spawn-fcgi $ARGS -- ../bin/sign_service $BID --conf-path=../cfg --log-level=700 --log-path=../log --daemon --pid-file=../cfg/sign_service${ID}.pid start

}
`../bin/sign_service --conf-path=../cfg --log-level=600 --log-path=../log --print-spawn-num=$ARGF start > /dev/null`
NUM=`cat $ARGF`
if [ $NUM -le 1 ]; then
	startservice
	exit	
fi
generateconf()
{
	ID=$1
	BID=''
	if [ '$ID' == '' ]; then
		return
	fi

	BID="--business-id="$ID
	ID="_"$ID;
	CFG='../cfg/sign_service'$ID'.xml';
	if [ -f $CFG ]; then
		ORG=`stat -c %Y ../cfg/sign_service.xml`;
		SPAWN=`stat -c %Y $CFG`;
		if [ $ORG -lt $SPAWN ]; then
			return
		fi
	fi


	`../bin/sign_service $BID --conf-file=../cfg/sign_service.xml --log-level=600 --log-path=../log --generate-spawn-conf=$CFG start > /dev/null`
	echo "generate "$CFG

}
CID=1
while [ $CID -le $NUM ] ;do
	generateconf $CID
	startservice $CID;
	CID=$(($CID+1));
done


