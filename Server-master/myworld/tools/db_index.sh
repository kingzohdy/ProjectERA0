#!/bin/sh


SqlAll=$(cat $1 | grep "CREATE TABLE IF NOT EXISTS" | awk '{ print $6 }')


for i in $SqlAll ; do
	Count=0
	Count1=0
	SqlName=$i
	for j in $* ; do
		CreateIdx="create index"
		Count=`expr $Count + 1`
		if [ $Count != 1 ]; then		
			IndexName=${SqlName}"_Idx_$Count1"
			Count1=`expr $Count1 + 1`
			CreateIdx=${CreateIdx}" ${IndexName} on "$SqlName" (" 
			CreateIdxOut=${CreateIdx}" "$j");"
			#echo "$CreateIdxOut"
			echo "$CreateIdxOut" >> $1
		fi
	done

done





