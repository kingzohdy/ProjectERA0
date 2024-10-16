#!/bin/sh

PATH=../../../tools:${PATH}
export PATH

tmng  --create metabase
tmng  --create mib
#tmng  --loadmeta ../cfg/test_ormsvr_database.xml
tmng  --loadmeta ../cfg/tdr_database.xml
