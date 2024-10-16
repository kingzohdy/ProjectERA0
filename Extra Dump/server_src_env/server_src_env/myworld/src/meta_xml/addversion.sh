#/bin/sh

. ~/myworld/src/meta_xml/libversion.sh

usage()
{
	echo "addversion.sh: Auto add xml metalib's version."
	echo
	echo "Usage:"
	echo "    addversion.sh XmlFileName"
	echo
	echo "XmlFileName must be one of \"${METALIB_FILES}\""
	exit 2
}
 

[[ $# != 1 ]] && usage

exist="no"
for i in $METALIB_FILES; do
	if [[ ${i} == ${1} ]]; then
		exist="yes"
		break
	fi
done

[[ ${exist} != "yes" ]] && fatal "File \"${1}\" is not in \"${METALIB_FILES}\"" 1
#rm -f  ${1}
try_update_version ${1}
sedit ${1}
exit 0
