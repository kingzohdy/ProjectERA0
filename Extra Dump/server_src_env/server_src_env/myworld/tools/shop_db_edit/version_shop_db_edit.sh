dir=$1
tgz=$2
webonly=0
t=`date +"%Y.%m.%d.%H.%M"`
if [ "$dir" = "" ]; then
	dir='./version_shop_db_edit'
	mkdir $dir -p
	rm -rf $dir
	tgz='version_shop_db_edit_'$t
fi
if [ "$dir" = "gather" ];then
	echo gather
	rm -f phpmyadmin33.tgz
	cd /usr/local/nginx/html/;rm -f phpmyadmin33.tgz; tar zcvf ~/myworld/tools/shop_db_edit/phpmyadmin33.tgz phpmyadmin33;cd -;
	exit
#	webonly=1
#	dir='./version_shop_db_edit'
#	mkdir $dir -p
#	rm -rf $dir
#	tgz='version_shop_db_edit_webonly_'$t
fi

if [ "$dir" = "clean" ];then
	echo clean
	rm -f version_shop_db_edit*tgz
	exit
fi
echo $dir



mkdir -p $dir/{dep,cfg}

cd $dir; tar xfvz ~/myworld/tools/shop_db_edit/phpmyadmin33.tgz; cd -;

movedefault()
{
	cd $dir/cfg/;
	for f in *.wsdl
	do
		mv -v -T $f ${f}.default;
	done
	cd -;
}

cpfiles()
{
	cp -f ~/myworld/src/cs_service/cs_service.wsdl $dir/cfg/
	cp -f ~/myworld/tools/shop_db_edit/htpasswd* $dir/cfg/
	cp -f ~/myworld/tools/shop_db_edit/readme.txt $dir/cfg/
	cp -f $dir/phpmyadmin33/scripts/create_tables.sql $dir/dep/
	rm -f $dir/phpmyadmin33/config.inc.php
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

