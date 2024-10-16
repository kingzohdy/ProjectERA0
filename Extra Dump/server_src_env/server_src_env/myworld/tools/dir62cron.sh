#!/bin/bash

if [ -f "/usr/local/mmog/cluster/bin/dirxml/dir62.xml" ];then
	scp -P36000 /usr/local/mmog/cluster/bin/dirxml/dir62.xml test@192.168.0.77:/usr/local/mmog/cluster/bin/dirxml/
fi

