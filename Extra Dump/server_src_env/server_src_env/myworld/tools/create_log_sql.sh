#!/bin/sh
GAME_COMM_HOME=${HOME}/myworld/comm_release
GAME_COMM_TOOLS=${GAME_COMM_HOME}/tools

PATH=${PATH}:${GAME_COMM_TOOLS}
export PATH

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleLogin -o  LogRoleLogin.sql
cat LogRoleLogin.sql > LogTable.sql 
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleNew -o  LogRoleNew.sql
cat LogRoleNew.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleLogout -o  LogRoleLogout.sql
cat LogRoleLogout.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleShop -o LogRoleShop.sql
cat LogRoleShop.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleMail -o  LogRoleMail.sql
cat LogRoleMail.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleGetMailGoods -o  LogRoleGetMailGoods.sql
cat LogRoleGetMailGoods.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleExchg -o  LogRoleExchg.sql
cat LogRoleExchg.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleStall -o  LogRoleStall.sql
cat LogRoleStall.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleStallBuy -o  LogRoleStallBuy.sql
cat LogRoleStallBuy.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleArmMachining -o LogRoleArmMachining.sql
cat LogRoleArmMachining.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogMosterDie -o LogMosterDie.sql
cat LogMosterDie.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRolePickItem -o LogRolePickItem.sql
cat LogRolePickItem.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleTask -o  LogRoleTask.sql
cat LogRoleTask.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleChat -o  LogRoleChat.sql
cat LogRoleChat.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleClan -o  LogRoleClan.sql
cat LogRoleClan.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRolePet -o  LogRolePet.sql
cat LogRolePet.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleDel -o  LogRoleDel.sql
cat LogRoleDel.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleRespact -o  LogRoleRespact.sql
cat LogRoleRespact.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleDesignation -o  LogRoleDesignation.sql
cat LogRoleDesignation.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogPlayerDie -o  LogPlayerDie.sql
cat LogPlayerDie.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogPetAttrChg -o  LogPetAttrChg.sql
cat LogPetAttrChg.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleMapChg -o  LogRoleMapChg.sql
cat LogRoleMapChg.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleSkillChg -o  LogRoleSkillChg.sql
cat LogRoleSkillChg.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleDiathesisChg -o  LogRoleDiathesisChg.sql
cat LogRoleDiathesisChg.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleRideChg -o  LogRoleRideChg.sql
cat LogRoleRideChg.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleFairyChg  -o  LogRoleFairyChg.sql
cat LogRoleFairyChg.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleDropItem  -o  LogRoleDropItem.sql
cat LogRoleDropItem.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleGmCmd  -o  LogRoleGmCmd.sql
cat LogRoleGmCmd.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleSnapshot  -o  LogRoleSnapshot.sql
cat LogRoleSnapshot.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name OpBuyShoppingChg  -o  OpBuyShoppingChg.sql
cat OpBuyShoppingChg.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name OpRoleQiYueChg  -o  OpRoleQiYueChg.sql
cat OpRoleQiYueChg.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name OpTradeInfo  -o  OpTradeInfo.sql
cat OpTradeInfo.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name OpRoleLvUpSnapshot  -o  OpRoleLvUpSnapshot.sql
cat OpRoleLvUpSnapshot.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleEnterPworld  -o  LogRoleEnterPworld.sql
cat LogRoleEnterPworld.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogUseXp  -o  LogUseXp.sql
cat LogUseXp.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogGift  -o  LogGift.sql
cat LogGift.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogPworldTime  -o  LogPworldTime.sql
cat LogPworldTime.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleSecondarySkill  -o  LogRoleSecondarySkill.sql
cat LogRoleSecondarySkill.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogRoleSecSkillRecipe  -o  LogRoleSecSkillRecipe.sql
cat LogRoleSecSkillRecipe.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogAccLogin  -o  LogAccLogin.sql
cat LogAccLogin.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql

meta2tab --meta_file ../cfg/db_meta/role_db_meta.tdr --meta_name LogAccLogout  -o  LogAccLogout.sql
cat LogAccLogout.sql >> LogTable.sql
echo -e "\n" >> LogTable.sql


rm OpTradeInfo.sql OpRoleQiYueChg.sql OpBuyShoppingChg.sql LogRoleShop.sql LogRoleMail.sql LogRoleExchg.sql LogRoleStall.sql LogAccLogin.sql LogAccLogout.sql
rm LogRoleStallBuy.sql LogRoleArmMachining.sql LogMosterDie.sql LogRolePickItem.sql LogRoleTask.sql LogRoleChat.sql LogRoleClan.sql 
rm LogRolePet.sql LogRoleDel.sql LogRoleRespact.sql LogRoleDesignation.sql LogPlayerDie.sql LogPetAttrChg.sql LogRoleMapChg.sql 
rm LogRoleSkillChg.sql LogRoleDiathesisChg.sql LogRoleRideChg.sql LogRoleFairyChg.sql LogRoleDropItem.sql LogRoleLogin.sql LogRoleNew.sql 
rm LogRoleLogout.sql LogRoleGmCmd.sql LogRoleSnapshot.sql LogRoleGetMailGoods.sql OpRoleLvUpSnapshot.sql
rm LogRoleEnterPworld.sql LogUseXp.sql LogGift.sql LogPworldTime.sql LogRoleSecondarySkill.sql LogRoleSecSkillRecipe.sql

