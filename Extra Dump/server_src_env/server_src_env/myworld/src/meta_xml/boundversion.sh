#!/bin/sh

. ~/myworld/src/meta_xml/libversion.sh

usage()
{
	echo "boundversion.sh: Bound all of xml metalib's version."
	echo
	echo "Usage:"
	echo "    boundversion.sh AddedVersion"
	echo
	echo "\"${METALIB_FILES}\" xml file will be bound version."
	exit 2
}

bound_version()
{
	local current_version=$(max_version_one)
 	if [[ -z ${current_version} ]]; then
 		fatal "Cannot get the version." 1
 	fi
	local bounded_version=$(expr ${current_version} + ${1})
 	for i in ${METALIB_FILES}; do
		update_version ${i} ${bounded_version}
 	done
}

[[ $# != 1 ]] && usage
bound_version ${1}
exit 0
