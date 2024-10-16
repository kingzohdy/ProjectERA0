#!/bin/sh

# Fisrt of all, you should set environment properly

export LD_LIBRARY_PATH=${HOME}/tsf4g/lib/:${LD_LIBRARY_PATH}
export PATH=.:${HOME}/tsf4g/tools/:${PATH}

TBUS_MGR=tbusmgr
RELAY_MGR=trelaymgr
TBUS_MGR_XML=./tbusmgr.xml
RELAY_MGR_XML=./trelaymgr.xml

help_info()
{
	echo "Usage: tbusconf [--addr | --relay] [ [--list | --write] [--version | -v] [--help | -h] ]"
}

addr_version()
{
	${TBUS_MGR} --version
}

addr_list()
{
	${TBUS_MGR} --conf-file ${TBUS_MGR_XML}
}

addr_write()
{
	${TBUS_MGR} --conf-file ${TBUS_MGR_XML} --write
}

relay_version()
{
	${RELAY_MGR} --version
}

relay_list()
{
	${RELAY_MGR} --conf-file ${RELAY_MGR_XML}
}

relay_write()
{
	${RELAY_MGR} --conf-file ${RELAY_MGR_XML} --write
}

#### main function ####
if test "$1" == ""; then
	addr_list

elif test "$1" == "--addr"; then

	if test "$2" == "--list"; then
		addr_list
	elif test "$2" == "--write"; then
		addr_write
	elif test "$2" == "--version" || test "$2" == "-v" ; then
		addr_version
	elif test "$2" == "--help" || test "$2" == "-h" ; then
		help_info
	else
		help_info
	fi

elif test "$1" == "--relay"; then

	if test "$2" == "--list"; then
		relay_list
	elif test "$2" == "--write"; then
		relay_write
	elif test "$2" == "--version" || test "$2" == "-v" ; then
		relay_version
	elif test "$2" == "--help" || test "$2" == "-h" ; then
		help_info
	else
		help_info
	fi

else
	help_info
fi

