#!/bin/bash

# Fisrt of all, you should set PATH to tdr tools properly
PATH=$1:${PATH}
export PATH

tdr --xml2h  robot_conf_desc.xml
tdr --xml2c  -o robot_conf_desc.c robot_conf_desc.xml
