dir=$1
tgz=$2
webonly=0
t=`date +"%Y.%m.%d.%H.%M"`
if [ "$dir" = "" ]; then
	dir='./version_sign_service'
	mkdir $dir -p
	rm -rf $dir
	tgz='version_sign_service_'$t
fi
if [ "$dir" = "webonly" ];then
	webonly=1
	dir='./version_sign_service'
	mkdir $dir -p
	rm -rf $dir
	tgz='version_sign_service_webonly_'$t
fi
echo $dir

mkdir -p $dir/{cfg,bin,dep,log}

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
	echo "create database sign_service;" > $dir/dep/sign_service.sql;
	echo "use sign_service;" >> $dir/dep/sign_service.sql;
	~/myworld/comm_release/tools/meta2tab --meta_file ~/myworld/cfg/db_meta/role_db_meta.tdr  --meta_name SignDBFailedOpLog -o ~/sql/SignDBFailedOpLog.sql
	cat ~/sql/SignDBFailedOpLog.sql >> $dir/dep/sign_service.sql;
}
cpfiles()
{
	cp -f ~/myworld/src/sign_service/sign_service $dir/bin
	cp -f ~/myworld/src/sign_service/sign_service.xml $dir/cfg/
	cp -f ~/myworld/src/sign_service/sign_service.wsdl $dir/cfg/
	cp -f ~/myworld/src/sign_service/start_sign_service.sh $dir/cfg/
	cp -f ~/myworld/src/sign_service/stop_sign_service.sh $dir/cfg/
	cp -f ~/myworld/src/sign_service/readme.txt $dir/cfg/
	cp -f ~/myworld/src/cs_service/htpasswd* $dir/cfg/
	cp -f ~/myworld/cfg/db_meta/role_db_meta.tdr $dir/cfg/
	cp -f ~/myworld/dep/spawn-fcgi-1.6.3.tar.gz $dir/dep/
	#generate_dbsql;
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

