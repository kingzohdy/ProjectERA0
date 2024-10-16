#!/bin/bash
# //  Çåµµ½Å±¾

usage()
{
	echo "./clean_world_count.sh world_8"
	exit 2
}

[[ $# != 1 ]] && usage

world=${1}

mysql -ummog -pmmog  ${1} -N -e "delete from AccShare"
mysql -ummog -pmmog  ${1} -N -e "delete from Clan"
mysql -ummog -pmmog  ${1} -N -e "delete from Global"
mysql -ummog -pmmog  ${1} -N -e "delete from MailEntry"
mysql -ummog -pmmog  ${1} -N -e "delete from NameID"
mysql -ummog -pmmog  ${1} -N -e "delete from Relation"
mysql -ummog -pmmog  ${1} -N -e "delete from RoleData"
mysql -ummog -pmmog  ${1} -N -e "delete from ShadowAccData"
mysql -ummog -pmmog  ${1} -N -e "delete from TopRange"
mysql -ummog -pmmog  ${1} -N -e "delete from TradeBuy"
mysql -ummog -pmmog  ${1} -N -e "delete from TradeSell"
