METALIB_FILES="proto_cs.xml proto_ss.xml proto_comm.xml resdb_meta.xml role_db_meta.xml"

fatal()
{
	echo -e "\033[0;31mFatal: ${1}\033[m"
	exit ${2}
}
 
metalib_version()
{
 	head ${1} | grep "^<metalib.*version=\"[0-9]*\".*>" | sed -e "s/.* version=\"//" -e "s/\".*$//"
 	[[ $? != 0 ]] && fatal "Get version failed." 1
}
 
metalib_version_list()
{
 	for i in ${METALIB_FILES}; do
 		metalib_version ${i}
 	done
}
 
max_version_one()
{
 	local ordered_version_list=$(metalib_version_list | sort -g -r)
 	for i in ${ordered_version_list}; do
 		echo ${i}
 		break
	done
}

update_version()
{
 	local will_replaced_line=$(head ${1} | grep "^<metalib.* version *= *\"[0-9]*\".*>" | sed -e "s/\r$//")
 	local new_version_line=$(echo ${will_replaced_line} | sed -e "s/ version=\"[0-9]*\"/ version=\"${2}\"/")

	sed -i ${1} -e "s/${will_replaced_line}/${new_version_line}/"
 	[[ $? != 0 ]] && fatal "Cannot modify version: ${1}." 1
 	echo -e "\033[0;32mFile: ${1} version is ${2} now. \033[m"
}
 
add_version()
{
 	local current_version=$(max_version_one)
 	if [[ -z ${current_version} ]]; then
 		fatal "Cannot get the version." 1
 	fi
 	local max_version=$(expr ${current_version} + 1)
 	echo -e "Last max version is:   \033[0;32m${current_version}\033[m"
 	echo -e "But version should be: \033[0;35m${max_version}\033[m"
 
 	update_version ${1} ${max_version}
}
 
try_update_version()
{
 	echo -e "\033[0;32mUpdate all metalib file first: \033[m"
 	cvs up
 	#[[ $? != 0 ]] && fatal "cvs update fail." 1
 	echo -e "\033[0;32mUpdate Done \033[m"
 	add_version ${1}

}
