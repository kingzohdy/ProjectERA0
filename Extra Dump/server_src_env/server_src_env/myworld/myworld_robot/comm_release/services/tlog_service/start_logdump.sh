#!/bin/sh
usage()
{
      echo "Usage: "
      echo "  $0  logpath=xxx dumpstart=(sec,usec) dumpend=(sec,usec)"
      echo "      if no logpath, should set it in logdump.xml"
      echo "      if dumpstart=(0,0) only limit dumpend, if dumpend=(0,0) only limit dumpstart "
      echo " "
      echo "Example: "
      echo "  $0  dumpstart=(123433343,198759) dumpend=(123433643,443223)"
      exit 1
}

if [ $# -lt 2 ]; then
  echo "paramter list uncorrect"
  usage
fi

for i in "$@"
do
	j=`echo $i|sed -n -e "/^dumpstart/p" -e "/^dumpend/p" -e "/^logpath/p"`
	if [ -z "$j" ]
	then
		usage
	fi
done

../bin/logdump  --conf-file=logdump.xml --log-level=600 --log-file=../log/logdump --daemon $@ start

