dir=$1
tgz=$2
webonly=0
t=`date +"%Y.%m.%d.%H.%M"`
if [ "$dir" = "" ]; then
	dir='./version_bi_service'
	mkdir $dir -p
	rm -rf $dir
	tgz='version_bi_service_'$t
fi
if [ "$dir" = "webonly" ];then
	webonly=1
	dir='./version_bi_service'
	mkdir $dir -p
	rm -rf $dir
	tgz='version_bi_service_webonly_'$t
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
	echo "create database bi_service;" > $dir/dep/bi_service.sql;
	echo "use bi_service;" >> $dir/dep/bi_service.sql;
	~/myworld/comm_release/tools/meta2tab --meta_file ~/myworld/cfg/db_meta/role_db_meta.tdr  --meta_name BiDBFailedOpLog -o ~/sql/BiDBFailedOpLog.sql
	cat ~/sql/BiDBFailedOpLog.sql >> $dir/dep/bi_service.sql;
}
cpfiles()
{
	cp -f ~/myworld/src/bi_service/bi_service $dir/bin
	cp -f ~/myworld/src/bi_service/bi_service.xml $dir/cfg/
	cp -f ~/myworld/src/bi_service/bi_service.wsdl $dir/cfg/
	cp -f ~/myworld/src/bi_service/start_bi_service.sh $dir/cfg/
	cp -f ~/myworld/src/bi_service/stop_bi_service.sh $dir/cfg/
	cp -f ~/myworld/src/bi_service/readme.txt $dir/cfg/
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

