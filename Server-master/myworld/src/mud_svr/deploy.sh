#!/bin/sh

mud_build()
{
	cd ~/myworld/src/mud_svr
	make clean all
	cd /usr/local/mmog/world_10/zone/cfg
	./stop_mud_svr.sh
	sleep 1;
	./stop_mud_conn.sh
	sleep 1;
	cp /usr/local/tjw/myworld/src/mud_svr/mud_svr ../bin -f
	./mud_rmshm.sh 
	./start_mud_svr.sh
	./start_mud_conn.sh
}
mud_gdb()
{
        # gdb zone_svr
        zonepid=` ps x |grep bin/mud_svr|grep -v grep |awk '{print $1}'`
	echo $zonepid
        gdb -pid $zonepid
}

case "$1" in
        "build")
                mud_build ;;
        "gdb")
                mud_gdb;;
	*)
		mud_build ;;
esac
