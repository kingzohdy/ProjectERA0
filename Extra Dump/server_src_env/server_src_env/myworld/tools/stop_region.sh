#!/bin/sh

if [ $# = 1 ];then
        RegionID=$1
else
        echo "parameter errer!"
        echo "./start_region.sh num;the num is region id"
        exit -1
fi

cd /usr/local/mmog/region/cfg;

./stop_aas_svr_$1.0.102.1_66.sh;
./stop_bill_db_$1.0.104.1_66.sh;
./stop_bill_svr_$1.0.103.1_66.sh;
./stop_region_gate_$1.0.101.1_66.sh;

./stop_account_cs_conn_$1.0.90.1_66.sh;
./stop_account_cs_svr_$1.0.91.1_66.sh;
./stop_account_db_$1.0.95.1_66.sh;
#./stop_auth_bak_$1.0.71.1_66.sh;
#./stop_auth_bak_conn_$1.0.74.1_66.sh;
./stop_auth_conn_$1.0.73.1_66.sh;
#./stop_auth_slave_$1.0.72.1_66.sh;
#./stop_auth_slave_conn_$1.0.75.1_66.sh;
./stop_auth_svr_$1.0.70.1_66.sh;
./stop_informer_svr_$1.0.94.1_66.sh;
./stop_service_svr_$1.0.109.1_66.sh;

