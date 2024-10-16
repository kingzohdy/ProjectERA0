# @file $RCSfile: Makefile,v $
#  general description of this module
#  $Id: Makefile,v 1.81 2013/08/05 09:32:47 tjw Exp $
#  @author $Author: tjw $
#  @date $Date: 2013/08/05 09:32:47 $
#  @version 1.0
#  @note Editor: Vim 6.1, Gcc 4.0.2, tab=4
#  @note Platform: Linux

include ../mmog.mk

.PHONY: all clean

all:no_name proto_cs role_db_meta zone torm_protocol proto_ss dir cs_svr cgi robot res op_log recv_report unite_svr tlog_db version_svr auth_svr bill_svr informer_svr service_svr region_gate aas_svr db_to_bin account_cs_svr mud_svr worldlist no_name_clear account_dyn active_code cdkey gamestat wid_check cs_service \
acc_service \
chat_mon_service \
bi_service \
genius \
sign_service \
#
no_name:
	rm -rf *_no_name.xml
	cp resdb_meta.xml resdb_meta_no_name.xml
	cp proto_cs.xml proto_cs_no_name.xml
	cp role_db_meta.xml role_db_meta_no_name.xml
	cp op_log_meta.xml op_log_meta_no_name.xml
	sed -i proto_cs_no_name.xml -e "s/<metalib name=\"proto_cs\"/<metalib name=\"\"/"
	sed -i role_db_meta_no_name.xml -e "s/<metalib name=\"role_db_meta\"/<metalib name=\"\"/"  
	sed -i op_log_meta_no_name.xml -e "s/<metalib name=\"op_log_meta\"/<metalib name=\"\"/"
	sed -i resdb_meta_no_name.xml -e "s/name=\"gameres\"/name=\"\"/"

no_name_clear:
	rm -rf *_no_name.xml
	rm -rf ../comm/*_no_name*

proto_cs:proto_comm.xml proto_cs.xml proto_comm.xml res
	$(GAME_COMM_TOOLS)/tdr -H -O ../comm/  --no_type_prefix   --no_lowercase_prefix proto_comm.xml resdb_meta_no_name.xml proto_cs.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ../comm/proto_cs.c proto_comm.xml resdb_meta_no_name.xml proto_cs.xml
	$(GAME_COMM_TOOLS)/tdr -B -o ../comm/proto_cs.tdr proto_comm.xml resdb_meta_no_name.xml proto_cs.xml
	rm -rf ../comm/proto_cs.meta
	cp -f ../comm/proto_cs.tdr ../../cfg/
	cp -f ../comm/proto_cs.h ../cs_cgi/
	cp -f ../comm/proto_comm.h ../cs_cgi/
	cp -f ../comm/proto_comm.h ../../tools/win32_cs_cgi_demos/cs_cgi_dll/
	cp -f ../comm/proto_cs.h ../../tools/win32_cs_cgi_demos/cs_cgi_dll/
	cp -f ../comm/proto_comm.h ../../myworld_robot/src/comm/
	cp -f ../comm/proto_cs.h ../../myworld_robot/src/comm/
	cp -f ../comm/proto_cs.tdr ~/myworld/tools/worldlist/

proto_ss:proto_comm.xml proto_ss.xml  proto_comm.xml proto_cs.xml
	$(GAME_COMM_TOOLS)/tdr -H -O ../comm/  --no_type_prefix   --no_lowercase_prefix proto_comm.xml resdb_meta_no_name.xml proto_cs_no_name.xml role_db_meta_no_name.xml op_log_meta_no_name.xml proto_ss.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ../comm/proto_ss.c proto_comm.xml resdb_meta_no_name.xml proto_cs_no_name.xml role_db_meta_no_name.xml op_log_meta_no_name.xml proto_ss.xml	
	$(GAME_COMM_TOOLS)/tdr -B -o ../comm/proto_ss.tdr proto_comm.xml resdb_meta_no_name.xml proto_cs_no_name.xml role_db_meta_no_name.xml  op_log_meta_no_name.xml proto_ss.xml
	cp -f ../comm/proto_ss.tdr ../../cfg/
	cp -f ../comm/proto_ss.tdr ../cs_cgi/
	cp -f ../comm/proto_ss.h ../cs_cgi/
	cp -f ../comm/proto_ss.h ../../tools/win32_cs_cgi_demos/cs_cgi_dll/
	cp -f ../comm/proto_ss.tdr ../../tools/win32_cs_cgi_demos/cs_cgi_dll/
	cp -f ../comm/proto_ss.tdr ../../tools/win32_cs_cgi_demos/cs_account_demo/
#	cp ../comm/op_log_meta.h ../../tools/win32_cs_cgi_demos/cs_cgi_dll/

role_db_meta:role_db_meta.xml tormsvr_conf_desc.xml proto_comm.xml
	$(GAME_COMM_TOOLS)/tdr -B -o $(MMOG_HOME)/cfg/db_meta/role_db_meta.tdr proto_comm.xml role_db_meta.xml
	$(GAME_COMM_TOOLS)/tdr -H -O ../zone/zone_svr/  --no_type_prefix   --no_lowercase_prefix proto_comm.xml role_db_meta.xml
	rm -rf ../zone/zone_svr/proto_comm.h
	$(GAME_COMM_TOOLS)/tdr --xml2h -O ../myworld_conf_create/ --no_type_prefix --no_lowercase_prefix  tormsvr_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr --xml2c -o ../myworld_conf_create/tormsvr_conf_desc.c tormsvr_conf_desc.xml
	cp -f ../zone/zone_svr/role_db_meta.h ../comm/
	cp -f ../zone/zone_svr/role_db_meta.h ../cs_cgi/
	cp -f ../zone/zone_svr/role_db_meta.h ../../tools/win32_cs_cgi_demos/cs_cgi_dll/

	
zone:zone_svr_conf_desc.xml zone_conn_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H -O ../zone/zone_svr/  --no_type_prefix   --no_lowercase_prefix zone_svr_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ../zone/zone_svr/zone_svr_conf_desc.c zone_svr_conf_desc.xml 
	$(GAME_COMM_TOOLS)/tdr -H -O ../zone/zone_conn/  --no_type_prefix   --no_lowercase_prefix zone_conn_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ../zone/zone_conn/zone_conn_conf_desc.c zone_conn_conf_desc.xml
	cp -f ../zone/zone_svr/zone_svr_conf_desc.h ../myworld_conf_create/
	cp -f ../zone/zone_svr/zone_svr_conf_desc.c ../myworld_conf_create/
	cp -f ../zone/zone_svr/zone_svr_conf_desc.h ../zone/zone_ctrl/
	cp -f ../zone/zone_svr/zone_svr_conf_desc.c ../zone/zone_ctrl/
	cp -f ../zone/zone_conn/zone_conn_conf_desc.h ../myworld_conf_create/
	cp -f ../zone/zone_conn/zone_conn_conf_desc.c ../myworld_conf_create/
	
	
	
torm_protocol:torm_protocol.xml
	$(GAME_COMM_TOOLS)/tdr -B -o $(MMOG_HOME)/cfg/db_meta/torm_protocol.tdr torm_protocol.xml
	
dir:dir_svr_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H -O ../dir_svr/  --no_type_prefix   --no_lowercase_prefix dir_svr_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ../dir_svr/dir_svr_conf_desc.c  dir_svr_conf_desc.xml
	cp -f ../dir_svr/dir_svr_conf_desc.h ../myworld_conf_create/
	cp -f ../dir_svr/dir_svr_conf_desc.c ../myworld_conf_create/

auth_svr:auth_svr_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H -O ../auth_svr/  --no_type_prefix   --no_lowercase_prefix auth_svr_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ../auth_svr/auth_svr_conf_desc.c auth_svr_conf_desc.xml
	cp -f ../auth_svr/auth_svr_conf_desc.h ../myworld_conf_create/
	cp -f ../auth_svr/auth_svr_conf_desc.c ../myworld_conf_create/

version_svr:proto_comm.xml version_svr_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H -O ../version_svr/  --no_type_prefix   --no_lowercase_prefix proto_comm.xml version_svr_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ../version_svr/version_svr_conf_desc.c proto_comm.xml version_svr_conf_desc.xml
	cp -f ../version_svr/version_svr_conf_desc.h ../myworld_conf_create/
	cp -f ../version_svr/version_svr_conf_desc.c ../myworld_conf_create/

cs_svr:cs_svr_conf_desc.xml
	#$(GAME_COMM_TOOLS)/tdr -H -O ../zone/cs_svr/  --no_type_prefix   --no_lowercase_prefix cs_svr_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H -O ../zone/cs_svr/ cs_svr_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ../zone/cs_svr/cs_svr_conf_desc.c  cs_svr_conf_desc.xml
	cp -f ../zone/cs_svr/cs_svr_conf_desc.h ../myworld_conf_create/
	cp -f ../zone/cs_svr/cs_svr_conf_desc.c ../myworld_conf_create/ 

cgi:proto_comm.xml cgi_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H --no_type_prefix   --no_lowercase_prefix proto_comm.xml cgi_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ./cgi_conf_desc.c proto_comm.xml cgi_conf_desc.xml
	cp -f cgi_conf_desc.c ../comm/
	cp -f cgi_conf_desc.h ../comm/
	cp -f cgi_conf_desc.c ../cs_cgi/
	cp -f cgi_conf_desc.h ../cs_cgi/
	cp -f cgi_conf_desc.h ../../tools/win32_cs_cgi_demos/cs_cgi_dll/
	cp -f cgi_conf_desc.c ../../tools/win32_cs_cgi_demos/cs_cgi_dll/
	cp -f cgi_conf_desc.c ../world_scan/
	cp -f cgi_conf_desc.h ../world_scan/

robot:robot_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H robot_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ./robot_conf_desc.c  robot_conf_desc.xml
	mv robot_conf_desc.h ~/myworld/tools/robot/robot_conf_desc.h
	mv robot_conf_desc.c ~/myworld/tools/robot/robot_conf_desc.c

recv_report:recv_report_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H recv_report_conf_desc.xml 
	$(GAME_COMM_TOOLS)/tdr -C -o ./recv_report_conf_desc.c recv_report_conf_desc.xml
	cp -f recv_report_conf_desc.h ../myworld_conf_create/
	cp -f recv_report_conf_desc.c ../myworld_conf_create/
	cp -f recv_report_conf_desc.h ../recv_report/
	cp -f recv_report_conf_desc.c ../recv_report/
unite_svr:unite_svr_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H unite_svr_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ./unite_svr_desc.c unite_svr_desc.xml
	cp -f unite_svr_desc.c ~/myworld/tools/unite_svr/unite_svr_desc.c
	cp -f unite_svr_desc.h ~/myworld/tools/unite_svr/unite_svr_desc.h
bill_svr:bill_svr_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H -O ../bill_svr/  --no_type_prefix   --no_lowercase_prefix bill_svr_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ../bill_svr/bill_svr_conf_desc.c bill_svr_conf_desc.xml
	cp -f ../bill_svr/bill_svr_conf_desc.h ../myworld_conf_create/
	cp -f ../bill_svr/bill_svr_conf_desc.c ../myworld_conf_create/
informer_svr:informer_svr_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H -O ../informer_svr/  --no_type_prefix   --no_lowercase_prefix informer_svr_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ../informer_svr/informer_svr_conf_desc.c informer_svr_conf_desc.xml
	cp -f ../informer_svr/informer_svr_conf_desc.h ../myworld_conf_create/
	cp -f ../informer_svr/informer_svr_conf_desc.c ../myworld_conf_create/
service_svr:service_svr_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H -O ../service_svr/  --no_type_prefix   --no_lowercase_prefix service_svr_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ../service_svr/service_svr_conf_desc.c service_svr_conf_desc.xml
	cp -f ../service_svr/service_svr_conf_desc.h ../myworld_conf_create/
	cp -f ../service_svr/service_svr_conf_desc.c ../myworld_conf_create/
cs_service:cs_service_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H -O ../cs_service/  --no_type_prefix   --no_lowercase_prefix cs_service_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ../cs_service/cs_service_conf_desc.c cs_service_conf_desc.xml
	cp -f ../cs_service/cs_service_conf_desc.h ../myworld_conf_create/
	cp -f ../cs_service/cs_service_conf_desc.c ../myworld_conf_create/
chat_mon_service:chat_mon_service_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H -O ../chat_mon_service/  --no_type_prefix   --no_lowercase_prefix chat_mon_service_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ../chat_mon_service/chat_mon_service_conf_desc.c chat_mon_service_conf_desc.xml
	cp -f ../chat_mon_service/chat_mon_service_conf_desc.h ../myworld_conf_create/
	cp -f ../chat_mon_service/chat_mon_service_conf_desc.c ../myworld_conf_create/
acc_service:acc_service_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H -O ../acc_service/  --no_type_prefix   --no_lowercase_prefix acc_service_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ../acc_service/acc_service_conf_desc.c acc_service_conf_desc.xml
	cp -f ../acc_service/acc_service_conf_desc.h ../myworld_conf_create/
	cp -f ../acc_service/acc_service_conf_desc.c ../myworld_conf_create/
bi_service:bi_service_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H -O ../bi_service/  --no_type_prefix   --no_lowercase_prefix bi_service_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ../bi_service/bi_service_conf_desc.c bi_service_conf_desc.xml
	cp -f ../bi_service/bi_service_conf_desc.h ../myworld_conf_create/
	cp -f ../bi_service/bi_service_conf_desc.c ../myworld_conf_create/
sign_service:sign_service_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H -O ../sign_service/  --no_type_prefix   --no_lowercase_prefix sign_service_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ../sign_service/sign_service_conf_desc.c sign_service_conf_desc.xml
	cp -f ../sign_service/sign_service_conf_desc.h ../myworld_conf_create/
	cp -f ../sign_service/sign_service_conf_desc.c ../myworld_conf_create/
region_gate:region_gate_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H -O ../region_gate/  --no_type_prefix   --no_lowercase_prefix region_gate_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ../region_gate/region_gate_conf_desc.c region_gate_conf_desc.xml
	cp -f ../region_gate/region_gate_conf_desc.h ../myworld_conf_create/
	cp -f ../region_gate/region_gate_conf_desc.c ../myworld_conf_create/
aas_svr:aas_svr_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H -O ../aas_svr/  --no_type_prefix   --no_lowercase_prefix aas_svr_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ../aas_svr/aas_svr_conf_desc.c aas_svr_conf_desc.xml
	cp -f ../aas_svr/aas_svr_conf_desc.h ../myworld_conf_create/
	cp -f ../aas_svr/aas_svr_conf_desc.c ../myworld_conf_create/
mud_svr:mud_svr_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H -O ../mud_svr/  --no_type_prefix   --no_lowercase_prefix mud_svr_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ../mud_svr/mud_svr_conf_desc.c mud_svr_conf_desc.xml
	cp -f ../mud_svr/mud_svr_conf_desc.h ../myworld_conf_create/
	cp -f ../mud_svr/mud_svr_conf_desc.c ../myworld_conf_create/
tlog_db:tlog_db_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H -O ../tlog_db/  --no_type_prefix   --no_lowercase_prefix tlog_db_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ../tlog_db/tlog_db_conf_desc.c tlog_db_conf_desc.xml
	cp -f ../tlog_db/tlog_db_conf_desc.h ../myworld_conf_create/
	cp -f ../tlog_db/tlog_db_conf_desc.c ../myworld_conf_create/
res:resdb_meta.xml
	$(GAME_COMM_TOOLS)/tdr -H -O ../comm/  --no_type_prefix   --no_lowercase_prefix proto_comm.xml resdb_meta.xml
	$(GAME_COMM_TOOLS)/tdr -B -o $(MMOG_HOME)/tools/resdb_meta.tdr proto_comm.xml resdb_meta.xml	
	cp -f  $(MMOG_HOME)/tools/resdb_meta.tdr ../../cfg/
	cp -f ~/myworld/src/comm/resdb_meta.h ../../myworld_robot/src/comm/ 

op_log:op_log_meta.xml op_log.xml proto_comm.xml
	$(GAME_COMM_TOOLS)/tdr -H -O ../comm/  --no_type_prefix   --no_lowercase_prefix proto_comm.xml  op_log_meta.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ../comm/op_log_meta.c proto_comm.xml op_log_meta.xml 
	$(GAME_COMM_TOOLS)/tdr -B -o ../comm/op_log_meta.tdr proto_comm.xml op_log_meta.xml
	cp -f op_log.xml ../../cfg/
	chmod +w ../../cfg/op_log.xml
	cp -f ../comm/op_log_meta.tdr ../../cfg/
	cp -f ../comm/op_log_meta.h ../../tools/win32_cs_cgi_demos/cs_cgi_dll/
db_to_bin:db_to_bin_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H db_to_bin_desc.xml  
	$(GAME_COMM_TOOLS)/tdr -C -o ./db_to_bin_desc.c db_to_bin_desc.xml
	cp -f db_to_bin_desc.c ~/myworld/tools/db_to_bin/db_to_bin_desc.c
	cp -f db_to_bin_desc.h ~/myworld/tools/db_to_bin/db_to_bin_desc.h
#scanf_role_snapshot:scanf_role_snapshot_desc.xml
#	$(GAME_COMM_TOOLS)/tdr -H scanf_role_snapshot_desc.xml
#	$(GAME_COMM_TOOLS)/tdr -C -o ./scanf_role_snapshot_desc.c scanf_role_snapshot_desc.xml
#	mv scanf_role_snapshot_desc.c ~/myworld/tools/scanf_role_snapshot/
#	mv scanf_role_snapshot_desc.h ~/myworld/tools/scanf_role_snapshot/

account_cs_svr:account_cs_svr_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H -O ../account_cs_svr/  --no_type_prefix   --no_lowercase_prefix account_cs_svr_conf_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ../account_cs_svr/account_cs_svr_conf_desc.c account_cs_svr_conf_desc.xml
	cp -f ../account_cs_svr/account_cs_svr_conf_desc.h ../myworld_conf_create/
	cp -f ../account_cs_svr/account_cs_svr_conf_desc.c ../myworld_conf_create/
worldlist:worldlist_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H worldlist_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ./worldlist_desc.c worldlist_desc.xml
	cp -f worldlist_desc.c ~/myworld/tools/worldlist/worldlist_desc.c
	cp -f worldlist_desc.h ~/myworld/tools/worldlist/worldlist_desc.h
account_dyn:account_dyn_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H account_dyn_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ./account_dyn_desc.c account_dyn_desc.xml
	cp -f account_dyn_desc.c account_dyn_desc.h ~/myworld/tools/account_dyn/

active_code:active_code_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H active_code_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ./active_code_desc.c active_code_desc.xml
	cp -f active_code_desc.* ~/myworld/tools/active_code/
cdkey:cdkey_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H cdkey_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ./cdkey_desc.c cdkey_desc.xml
	cp -f cdkey_desc.* ~/myworld/tools/cdkey/
gamestat:gamestat_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H gamestat_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ./gamestat_desc.c gamestat_desc.xml
	cp -f gamestat_desc.c gamestat_desc.h ~/myworld/tools/gamestat/
wid_check:wid_check_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H wid_check_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ./wid_check_desc.c wid_check_desc.xml
	cp -f wid_check_desc.c wid_check_desc.h ~/myworld/tools/wid_check/

genius:genius_rectify_desc.xml
	$(GAME_COMM_TOOLS)/tdr -H genius_rectify_desc.xml
	$(GAME_COMM_TOOLS)/tdr -C -o ./genius_rectify_desc.c genius_rectify_desc.xml
	cp -f genius_rectify_desc.* ~/myworld/tools/genuis_rectify/
clean:
	
