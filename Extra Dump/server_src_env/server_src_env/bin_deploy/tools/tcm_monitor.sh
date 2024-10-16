#!/bin/sh

CMD="./sendEmail"
SEND="niko@wanyouyl.com"
RECV="niko@wanyouyl.com"
SMTP="10.129.96.21"
FILE="tcm_status.txt"

/usr/local/mmog/tools/tcm_monitor --conf-file=/usr/local/mmog/tcm/cfg/tcmconsole.xml start
if [[ ! -e ${FILE} ]]; then
	exit 0
fi

cat ${FILE} | grep -e "Connected(0)" -e "NOT-RUNNING" -e "timeout"
if [[ $? == "0" ]]; then
	${CMD} -f ${SEND} -t ${RECV} -s ${SMTP} -u "[TCM Alarm]" -m "TCM Alarm" -a ${FILE}
fi
