#! /bin/sh
cd /usr/local/mmog/world_${1}/zone/cfg;
./stop_zone_svr.sh;
sleep 2
./zone_rmshm.sh;
sleep 1 
./stop_zone_conn.sh;
cd /usr/local/mmog/world_${1}/db/cfg
./stop_role_db.sh;./stop_name_id_db.sh;
sleep 1
./start_role_db.sh;./start_name_id_db.sh
cd /usr/local/mmog/world_${1}/zone/cfg
./start_zone_svr.sh;./start_zone_conn.sh
