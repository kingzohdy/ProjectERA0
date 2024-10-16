#!/bin/sh
usage()
{
      echo "Usage: "
      echo "  $0  TappName"
      echo " "
      echo "Example: "
      echo "  $0  tqosproxy "
}

if [ $# -ne 1 ]; then
  echo "paramter list uncorrect"
  usage
  exit 1
fi

if  test "$1" == "--help" || test "$1" == "-h" ; then
	usage
	exit 0
fi	


rm -rf ${1}
mkdir ${1}

APP_BIG=`echo $1 | tr a-z A-Z`
	  
sed -e "s/TEMPLE_ORI/$1/g" -e "s/TEMPLE_BIG/$APP_BIG/g" ./temple/temple.c > ${1}/${1}.c
sed -e "s/TEMPLE_ORI/$1/g" -e "s/TEMPLE_BIG/$APP_BIG/g" ./temple/temple_conf_desc.xml > ${1}/${1}_conf_desc.xml
sed -e "s/TEMPLE_ORI/$1/g" -e "s/TEMPLE_BIG/$APP_BIG/g" ./temple/temple.xml > ${1}/${1}.xml
cp ./temple/Makefile ${1}/ 
sed -e "s/TEMPLE_ORI/$1/g" -e "s/TEMPLE_BIG/$APP_BIG/g" ./temple/conv.sh > ${1}/conv.sh
sed -e "s/TEMPLE_ORI/$1/g" -e "s/TEMPLE_BIG/$APP_BIG/g" ./temple/Makefile.real > ${1}/Makefile.real
sed -e "s/TEMPLE_ORI/$1/g" -e "s/TEMPLE_BIG/$APP_BIG/g" ./temple/version.h > ${1}/version.h
sed -e "s/TEMPLE_ORI/$1/g" -e "s/TEMPLE_BIG/$APP_BIG/g" ./temple/start_temple.sh > ${1}/start_${1}.sh
sed -e "s/TEMPLE_ORI/$1/g" -e "s/TEMPLE_BIG/$APP_BIG/g" ./temple/stop_temple.sh > ${1}/stop_${1}.sh
chmod +x ${1}/*.sh
