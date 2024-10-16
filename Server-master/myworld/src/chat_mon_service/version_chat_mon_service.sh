dir=$1
tgz=$2
webonly=0
t=`date +"%Y.%m.%d.%H.%M"`
if [ "$dir" = "" ]; then
	dir='./version_chat_mon_service'
	mkdir $dir -p
	rm -rf $dir
	tgz='version_chat_mon_service_'$t
fi
if [ "$dir" = "webonly" ];then
	webonly=1
	dir='./version_chat_mon_service'
	mkdir $dir -p
	rm -rf $dir
	tgz='version_chat_mon_service_webonly_'$t
fi
echo $dir

mkdir -p $dir/{cfg,bin,web,dep,log}

movedefault()
{
	cd $dir/cfg/;
	for f in *.xml
	do
		mv -v -T $f ${f}.default;
	done
	cd -;
}
generate_dbsql()
{
	echo "create database chat_mon_service;" > $dir/dep/chat_mon_service.sql;
	echo "use chat_mon_service;" >> $dir/dep/chat_mon_service.sql;
	~/myworld/comm_release/tools/meta2tab --meta_file ~/myworld/cfg/db_meta/role_db_meta.tdr  --meta_name LogRoleChatMon -o ~/sql/LogRoleChatMon.sql
	cat ~/sql/LogRoleChatMon.sql >> $dir/dep/chat_mon_service.sql;
}
cpfiles()
{
	cp -f ~/myworld/src/chat_mon_service/chat_mon_service $dir/bin
	cp -f ~/myworld/src/chat_mon_service/chat_mon_service.xml $dir/cfg/
	cp -f ~/myworld/src/chat_mon_service/chat_mon_service.wsdl $dir/cfg/
	cp -f ~/myworld/src/chat_mon_service/start_chat_mon_service.sh $dir/cfg/
	cp -f ~/myworld/src/chat_mon_service/stop_chat_mon_service.sh $dir/cfg/
	cp -f ~/myworld/src/chat_mon_service/readme.txt $dir/cfg/
	cp -f ~/myworld/src/cs_service/htpasswd* $dir/cfg/
	cp -f ~/myworld/cfg/db_meta/role_db_meta.tdr $dir/cfg/
	cp -f ~/myworld/dep/spawn-fcgi-1.6.3.tar.gz $dir/dep/
	generate_dbsql;
	movedefault;
}

cpfiles;

if [ 'tgz' != '' ]; then
	tgzdir=$dir;
	if [ $webonly = 1 ];then
		tgzdir=$dir/web
	fi
	tar zcvf ${tgz}.tgz $tgzdir
fi

