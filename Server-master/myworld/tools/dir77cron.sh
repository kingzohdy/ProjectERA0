#!/bin/bash

cd /usr/local/jerry/myworld/tools/worldlist
make clean;make >1
rm 1
./worldlist

cp -f dir.xml /usr/local/mmog/server_tools/htdocs/patchs





