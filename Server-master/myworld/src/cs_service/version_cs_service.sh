dir=$1
tgz=$2
webonly=0
t=`date +"%Y.%m.%d.%H.%M"`
if [ "$dir" = "" ]; then
	dir='./version_cs_service'
	mkdir $dir -p
	rm -rf $dir
	tgz='version_cs_service_'$t
fi
if [ "$dir" = "webonly" ];then
	webonly=1
	dir='./version_cs_service'
	mkdir $dir -p
	rm -rf $dir
	tgz='version_cs_service_webonly_'$t
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
	cd $dir/web/gmtools/cfg;
	for f in *.php *.wsdl
	do
		mv -v -T $f default.${f};
	done
	cd -;
}

cpfiles()
{
	~/myworld/tools/fetch_gmtools.sh 
	rsync -rv --exclude=_notes --exclude=.svn /data/gmtools $dir/web/
	rsync -rv --exclude=_notes --exclude=.svn /data/gmtools/dep $dir/web/
	rm -rfv $dir/web/gmtools/dep
	cp -f ~/myworld/src/cs_service/cs_service $dir/bin
	cp -f ~/myworld/src/cs_cgi/cs_cgi.xml $dir/cfg/
	cp -f ~/myworld/src/cs_service/cs_service.xml $dir/cfg/
	cp -f ~/myworld/src/cs_service/cs_service.wsdl $dir/cfg/
	cp -f ~/myworld/src/cs_service/start_cs_service.sh $dir/cfg/
	cp -f ~/myworld/src/cs_service/stop_cs_service.sh $dir/cfg/
	cp -f ~/myworld/src/cs_service/readme.txt $dir/cfg/
	cp -f ~/myworld/src/cs_service/htpasswd* $dir/cfg/
	cp -f ~/myworld/cfg/proto_ss.tdr $dir/cfg/
	cp -f ~/myworld/dep/spawn-fcgi-1.6.3.tar.gz $dir/dep/
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
