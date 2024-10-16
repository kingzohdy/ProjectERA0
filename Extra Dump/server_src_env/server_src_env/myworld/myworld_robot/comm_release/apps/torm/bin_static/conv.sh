#!/bin/sh
PATH=../../../tools:${PATH}
export PATH

#tdr --xml2dr -o ../cfg/test_ormsvr_database.tdr ../cfg/test_ormsvr_database.xml
tdr --xml2dr -o ../cfg/tdr_database.tdr ../cfg/tdr_database.xml
