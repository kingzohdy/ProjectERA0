#!/bin/bash

# Fisrt of all, you should set PATH to tdr tools properly
PATH=$1:${PATH}
export PATH

tdr --xml2h global_var_svr_conf_desc.xml
tdr --xml2c  -o global_var_svr_conf_desc.c global_var_svr_conf_desc.xml
