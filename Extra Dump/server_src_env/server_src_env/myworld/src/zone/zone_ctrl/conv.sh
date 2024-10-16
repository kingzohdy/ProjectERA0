#!/bin/bash

# Fisrt of all, you should set PATH to tdr tools properly
PATH=$1:${PATH}
export PATH

tdr --xml2h  zone_ctrl_conf_desc.xml
tdr --xml2c  -o zone_ctrl_conf_desc.c zone_ctrl_conf_desc.xml
