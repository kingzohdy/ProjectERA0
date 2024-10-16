#!/bin/bash
cd ~/myworld/src/zone/zone_svr/
mkdir -p ./tmp1/
#cvs update >tmp.txt
#cat cmp.txt|egrep U |awk -F " " '{print $2}' >tmp1.txt
#cat tmp1.txt |awk -F "" '{print "cvs log "$0}' >tmp5.txt
while read filename;do
echo $filename|sh|egrep revision|head |awk -F " " '{print $2}'|egrep -v r|head -n2 >tmp3.txt
while read ver; do  printf $ver;printf " ";i=$((i+1));done <tmp3.txt >tmp2.txt
cat tmp2.txt |awk -F " " '{printf "cvs diff -r"$1" -r"$2}' >tmp3.txt
echo $filename|awk -F " " '{printf " "$3;printf " "}'>>tmp3.txt; 
cat tmp3.txt |awk -F " " '{print $0 ">> ./tmp1/" $5"_"$3}' |sh 
done < tmp5.txt
#rm tmp*txt

