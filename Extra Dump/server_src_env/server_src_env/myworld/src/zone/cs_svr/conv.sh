#!/bin/bash

# Fisrt of all, you should set PATH to tdr tools properly
PATH=$1:${PATH}
export PATH

tdr --xml2h  cs_svr_conf_desc.xml
tdr --xml2c  -o cs_svr_conf_desc.c cs_svr_conf_desc.xml
