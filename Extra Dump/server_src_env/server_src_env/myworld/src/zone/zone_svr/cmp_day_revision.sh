#!/bin/bash
# //使用方法，加个crontab 就可以了 
# * */1 * * *  /usr/local/ouyang/myworld/src/zone/zone_svr/cmpverdiff.sh
#
#


cd ~/myworld/src/zone/zone_svr/
mkdir -p ./tmp/
cvs update >cmp.txt
cat cmp.txt|egrep U |awk -F " " '{print $2}' >tmp1.txt
cat tmp1.txt |awk -F "" '{print "cvs log "$0}' >tmp5.txt
today=`date "+%Y-%m-%d"`
yesterday==`date --date="1   day   ago" "+%Y-%m-%d"`
while read filename;do
echo $filename|sh|egrep -A1 revision|egrep -v "\-\-"|egrep -v total |egrep -v description|egrep -B1 $today| head -n2 >tmp6.txt
echo $filename|sh|egrep -A1 revision|egrep -v "\-\-"|egrep -v total |egrep -v description|egrep -A3 $today|egrep -v $today|egrep -B1 date >tmp7.txt
cat tmp6.txt|egrep revision|head |awk -F " " '{print $2}'|egrep -v r>tmp3.txt
cat tmp7.txt|egrep revision|head |awk -F " " '{print $2}'|egrep -v r >>tmp3.txt
while read ver; do  printf $ver;printf " ";i=$((i+1));done <tmp3.txt >tmp2.txt
cat tmp2.txt |awk -F " " '{printf "cvs diff -r"$1" -r"$2}' >tmp3.txt
echo $filename|awk -F " " '{printf " "$3;printf " "}'>>tmp3.txt; 
cat tmp3.txt |awk -F " " '{print $0 ">> ./tmp/" $5"_"$3}' |sh 
done < tmp5.txt
rm tmp*txt

