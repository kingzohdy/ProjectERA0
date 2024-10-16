#!/bin/sh
GAME_COMM_HOME=${HOME}/myworld/comm_release
GAME_COMM_TOOLS=${GAME_COMM_HOME}/tools

PATH=${PATH}:${GAME_COMM_TOOLS}
export PATH

meta2tab  --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name RoleData -o create_role_db.sql
./db_index.sh create_role_db.sql Uin LastLogin LastLogout

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name TradeSell -o  tmp.sql
./db_index.sh tmp.sql  Uin
echo "alter table TradeSell auto_increment=1000;" >>tmp.sql
echo "ALTER TABLE TradeSell  MODIFY COLUMN Pass9Account varchar(51) character set latin1 collate latin1_bin default NULL;" >> tmp.sql
cat tmp.sql >> create_role_db.sql
rm tmp.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name TradeBuy -o  tmp.sql
./db_index.sh tmp.sql  Uin
echo "alter table TradeBuy auto_increment=10000000;" >>tmp.sql
echo "ALTER TABLE TradeBuy MODIFY COLUMN Pass9Account varchar(51) character set latin1 collate latin1_bin default NULL;" >> tmp.sql
cat tmp.sql >> create_role_db.sql
rm tmp.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name Home -o tmp.sql
cat tmp.sql >> create_role_db.sql
rm tmp.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name ShopDef -o  tmp.sql
./db_index.sh tmp.sql  ShopID
cat tmp.sql >> create_role_db.sql
cat tmp.sql > ShopTable.sql
rm tmp.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name ShopLimitDef -o  tmp.sql
./db_index.sh tmp.sql  ShopID
cat tmp.sql >> create_role_db.sql
cat tmp.sql >> ShopTable.sql
rm tmp.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name ShopMsgDef -o  tmp.sql
./db_index.sh tmp.sql  seqID
cat tmp.sql >> create_role_db.sql
cat tmp.sql >> ShopTable.sql
rm tmp.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name VipBarDef -o  tmp.sql
./db_index.sh tmp.sql IP
cat tmp.sql >> ShopTable.sql
rm tmp.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name VipBarLevelDef -o  tmp.sql
cat tmp.sql >> ShopTable.sql
rm tmp.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name VipBarNumDef -o  tmp.sql
cat tmp.sql >> ShopTable.sql
rm tmp.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name RegionGlobal -o tmp.sql
cat tmp.sql >> ShopTable.sql
rm tmp.sql 

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name TopRange -o tmp.sql
./db_index.sh tmp.sql TopRange
cat tmp.sql >> create_role_db.sql
rm tmp.sql

meta2tab  --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name ShadowAccData -o create_shadow_role_db.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name AccShare -o create_acc_share.sql

meta2tab  --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name NameID -o create_nameid_db.sql

meta2tab  --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name Relation -o create_relation.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name Global -o create_global.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name GMIPMacLimit -o create_GMIPMacLimit.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name Clan -o create_clan.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name MailEntry -o MailEntry.sql
./db_index.sh MailEntry.sql Recv Time
 
meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name OnlineReCord -o  create_online.sql
meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name Active -o Active.sql
./db_index.sh Active.sql Pass9Account

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name Account -o  create_account_db.sql
./db_index.sh create_account_db.sql Uin Pass9Account LastLoginTime LastLogout
echo "ALTER TABLE Account MODIFY COLUMN Pass9Account varchar(51) character set latin1 collate latin1_bin default NULL;" >> create_account_db.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name AccountRole -o  create_account_role.sql
./db_index.sh create_account_role.sql RoleName Uin Pass9Account
echo "ALTER TABLE AccountRole MODIFY COLUMN Pass9Account varchar(51) character set latin1 collate latin1_bin default NULL;" >> create_account_role.sql
echo "ALTER TABLE AccountRole MODIFY COLUMN RoleName varchar(32) character set latin1 collate latin1_bin default NULL;" >> create_account_role.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name IPBlock -o  IpBlock.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name Bill -o  Bill.sql
./db_index.sh Bill.sql  Uin
cat Bill.sql > create_bill_db.sql
rm Bill.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name AddBillRecord -o  AddBillRecord.sql
./db_index.sh AddBillRecord.sql  Uin
cat AddBillRecord.sql >> create_bill_db.sql
rm AddBillRecord.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name DecBillRecord -o  DecBillRecord.sql
./db_index.sh DecBillRecord.sql  Uin
cat DecBillRecord.sql >> create_bill_db.sql
rm DecBillRecord.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name GodCoinConsumeRecord -o  GodCoinConsumeRecord.sql
./db_index.sh GodCoinConsumeRecord.sql  Uin
cat GodCoinConsumeRecord.sql >> create_bill_db.sql
rm GodCoinConsumeRecord.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name PropsDef -o  PropsDef.sql
cat PropsDef.sql >> create_bill_db.sql
rm PropsDef.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name PropsGift -o  PropsGift.sql
./db_index.sh PropsGift.sql Uin 
cat PropsGift.sql >> create_bill_db.sql
rm PropsGift.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name AccGift -o  AccGift.sql
./db_index.sh AccGift.sql  Uin RoleID
cat AccGift.sql >> create_bill_db.sql
#rm AccGift.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name CDKeyTypeDef -o CDKeyTypeDef.sql
cat CDKeyTypeDef.sql >> create_bill_db.sql
rm CDKeyTypeDef.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name CDKeyDef -o CDKeyDef.sql
./db_index.sh CDKeyDef.sql RoleID
cat CDKeyDef.sql >> create_bill_db.sql
rm CDKeyDef.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name CDKeyGet -o CDKeyGet.sql
./db_index.sh CDKeyGet.sql RoleID
cat  CDKeyGet.sql >> create_bill_db.sql
rm CDKeyGet.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name Store -o Store.sql
./db_index.sh Store.sql RoleID
cat  Store.sql >> create_role_db.sql
#rm Store.sql


#echo "成功生成建表语句，并保存在文件<create_bill_db.sql>中"

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name RoleDrop -o create_role_drop.sql

