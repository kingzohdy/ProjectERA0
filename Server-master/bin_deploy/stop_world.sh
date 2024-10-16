#! /bin/sh
cd /usr/local/mmog/world_${1}/zone/cfg;./stop_zone_svr.sh;sleep 2;./stop_zone_conn.sh
sleep 1
cd /usr/local/mmog/world_${1}/db/cfg;./stop_role_db.sh;./stop_name_id_db.sh
