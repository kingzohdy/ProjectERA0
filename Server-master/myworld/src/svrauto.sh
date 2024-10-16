#!/bin/ksh


ss=/usr/local/mmog/
world=
worldid=
usrworld=
CONF_FILE_PATH="/usr/local/mmog/deploy.conf"
TAG_VERSION=

world_build()
{
	if [[ ${TAG_VERSION} == "" || ${TAG_VERSION} == 0 ]]; then
		if [[ ${world} == "world_1" || ${world} == "world_20" ]]; then
			cd $HOME"/myworld/comm_release"
			cvs update
			cd $HOME"/myworld/src"
			cvs update
			cd $HOME"/myworld/tools/db_to_bin"
			cvs update
		else
			cd $HOME"/myworld/comm_release"
			cvs update -A 
			cd $HOME"/myworld/src"
			cvs update -A 
			cd $HOME"/myworld/tools/db_to_bin"
			cvs update -A
		fi
	else
		cd $HOME
		echo "cvs checkout -r $TAG_VERSION..."
		cvs checkout -r  "tag_"$TAG_VERSION myworld
	fi

	cd $HOME"/myworld/src"
	
	cp -f mmog_dev_env.mk mmog.mk
        make clean;make
        rm -rf mmog.mk
        cvs up mmog.mk

	cd $HOME"/myworld/tools/db_to_bin"
	make clean;make
	$ss"sworldctl.sh"  stop
	cd $HOME"/myworld/src"
        ./sdeploy.sh  $TAG_VERSION
	sleep 1
        $ss"sworldctl.sh" start
}

stop_world()
{
	$ss"sworldctl.sh" stop
}

start_world()
{	
        $ss"sworldctl.sh" start
}

world_gdb()
{
        # gdb zone_svr
        zonepid=` ps x |grep zone_svr|grep -v grep |awk '{print $1}'`
        gdb $ss$world"/zone/bin/zone_svr" $zonepid

}

zone_svr_build()
{
	cd $HOME"/myworld/src"
	cp -f mmog_dev_env.mk mmog.mk
	make clean;make
	rm -rf mmog.mk
	cvs up mmog.mk
	cd  $usrworld"/zone/cfg"
	cp -f $HOME"/myworld/src/zone/zone_svr/zone_svr"  ../bin/
	cp -f $HOME"/myworld/src/mud_svr/mud_svr" ../bin/
	./start_zone_svr.sh
	./start_mud_svr.sh
}

menu_hlp()
{
        echo "Usage:"
        echo "	./svrauto.sh (build|gdb|svr) "
        echo "Mean: "
        echo "	build)  stop_world && make src && deploy && start_world"
        echo "	gdb)    gdb zone_svr "
	echo "	svr)	stop_zone_svr & make -> cp zone_svr & start_zone_svr"
	echo "Example:"
	echo "	./svrauto.sh build"
	echo "	./svrauto.sh gdb"
#	echo "	./svrauto.sh svr"
	echo "world_1 Example: 不加版本号编译的是最新的程序"
	echo "world_1 Example: 加版本号编译的是该分支版本的程序"
	echo "	./svrauto.sh build | 0_0_22"
}

#main
	umask 0007

	
	#ename=$(awk -F: '{print $1 " " $3}' /etc/passwd | grep " $UID$" | awk '{print $1}')
        conf_line=$(grep -P "^$LOGNAME:\d+:.*" ${CONF_FILE_PATH})
	
        
	if [[ -z $conf_line ]]; then
                echo ""$ename" can not deploy!"
                exit 127
        fi

        i=0
        for column in $(echo $conf_line | sed -e "s/:/ /g")
        do
                conf[$i]=$column
                ((i+=1))
        done

	worldid=${conf[1]}
	world=world_${conf[1]}
	usrworld=$ss$world

	TAG_VERSION=$2

case "$1" in
	"build")
		world_build ;;
	"gdb")
		world_gdb;;
	"svr")
		zone_svr_build;;
	"stop") 
		stop_world ;;
	"start")
		start_world ;;
	*)
		menu_hlp;;
esac



