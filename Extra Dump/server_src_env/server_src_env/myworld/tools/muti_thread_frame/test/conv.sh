#!/bin/bash

# Fisrt of all, you should set PATH to tdr tools properly
PATH=$1:${PATH}
export PATH

tdr --xml2h  --no_type_prefix   --no_lowercase_prefix  test_conf_desc.xml
tdr --xml2c  -o test_conf_desc.c test_conf_desc.xml
