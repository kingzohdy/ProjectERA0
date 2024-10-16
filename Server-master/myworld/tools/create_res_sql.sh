#!/bin/sh
COMM_TOOLS=${HOME}/myworld/comm_release/tools

PATH=${PATH}:${COMM_TOOLS}
export PATH

meta2tab  --charset latin1 --meta_file resdb_meta.tdr --meta_name MapDef -o MapDef.sql
cat MapDef.sql > ResTable.sql
echo -e "\n" >> ResTable.sql
meta2tab  --charset latin1 --meta_file resdb_meta.tdr --meta_name ProcEvent -o ProcEvent.sql
cat ProcEvent.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql
meta2tab  --charset latin1 --meta_file resdb_meta.tdr --meta_name MonsterDef -o MonsterDef.sql
cat MonsterDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql
meta2tab  --charset latin1 --meta_file resdb_meta.tdr --meta_name ItemDef -o ItemDef.sql
cat ItemDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql
meta2tab  --charset latin1 --meta_file resdb_meta.tdr --meta_name NpcSellList -o NpcSellList.sql
cat NpcSellList.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql
meta2tab  --charset latin1 --meta_file resdb_meta.tdr --meta_name TransList -o TransList.sql 
cat TransList.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql
meta2tab  --charset latin1 --meta_file resdb_meta.tdr --meta_name MapNpc -o MapNpc.sql 
cat MapNpc.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql
meta2tab  --charset latin1 --meta_file resdb_meta.tdr --meta_name MapMonster -o MapMonster.sql 
cat MapMonster.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql
meta2tab  --charset latin1 --meta_file resdb_meta.tdr --meta_name PworldDef -o PworldDef.sql 
cat PworldDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab  --charset latin1 --meta_file resdb_meta.tdr --meta_name BirthMapDef -o BirthMapDef.sql
cat BirthMapDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab  --charset latin1 --meta_file resdb_meta.tdr --meta_name ActionStatus -o ActionStatus.sql
cat ActionStatus.sql >>  ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab  --charset latin1 --meta_file resdb_meta.tdr --meta_name LevelUP -o LevelUP.sql
cat LevelUP.sql >>  ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab  --charset latin1 --meta_file resdb_meta.tdr --meta_name ItemDisplayInfo  -o ItemDisplayInfo.sql
cat ItemDisplayInfo.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab  --charset latin1 --meta_file resdb_meta.tdr --meta_name NpcDisplayInfo  -o NpcDisplayInfo.sql
cat NpcDisplayInfo.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab  --charset latin1 --meta_file resdb_meta.tdr --meta_name SkillDef -o SkillDef.sql
cat SkillDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab  --charset latin1 --meta_file resdb_meta.tdr --meta_name ArmLevelHurtDef -o ArmLevelHurtDef.sql
cat ArmLevelHurtDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab  --charset latin1 --meta_file resdb_meta.tdr --meta_name DiathesisDef -o DiathesisDef.sql
cat DiathesisDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name FittingsDef -o FittingsDef.sql
cat FittingsDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name StatusDef -o StatusDef.sql
cat StatusDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name StatusOverlayDef -o StatusOverlayDef.sql
cat StatusOverlayDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name AddAttrDef -o AddAttrDef.sql
cat AddAttrDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name SuitSet -o SuitSet.sql
cat SuitSet.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name MonDynAttrDef -o MonDynAttrDef.sql
cat MonDynAttrDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name PatrolDef -o PatrolDef.sql
cat PatrolDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name TaskDef -o TaskDef.sql
cat TaskDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name TaskRecycleDef -o TaskRecycleDef.sql
cat TaskRecycleDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name NpcInfoDef -o NpcInfoDef.sql
cat NpcInfoDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name QiYueDef -o QiYueDef.sql
cat QiYueDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name MonDynExpLimitDef -o MonDynExpLimitDef.sql
cat MonDynExpLimitDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name SceneAreaDef -o SceneAreaDef.sql
cat SceneAreaDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name ArmGradeDef -o ArmGradeDef.sql
cat ArmGradeDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name ArmRandDef -o ArmRandDef.sql
cat ArmRandDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name ArmHideAttrDef -o ArmHideAttrDef.sql
cat ArmHideAttrDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql


meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name GemDef -o GemDef.sql
cat GemDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name BirthItemDef -o BirthItemDef.sql
cat BirthItemDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name MonsterSpeechDef -o MonsterSpeechDef.sql
cat MonsterSpeechDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name BootyGroupDef -o BootyGroupDef.sql
cat BootyGroupDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name ArmAddAttrDef -o ArmAddAttrDef.sql
cat ArmAddAttrDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name WorldMapSearchDef -o WorldMapSearchDef.sql
cat WorldMapSearchDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name OnlineAwardDef -o OnlineAwardDef.sql
cat OnlineAwardDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name TaskLimitGroup -o TaskLimitGroup.sql
cat TaskLimitGroup.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name ClanCityBuildingLevelDef -o ClanCityBuildingLevelDef.sql
cat ClanCityBuildingLevelDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name ClanCityLevelDef -o ClanCityLevelDef.sql
cat ClanCityLevelDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name ClanLevelDef -o ClanLevelDef.sql
cat ClanLevelDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name MonsterColorDef -o MonsterColorDef.sql
cat MonsterColorDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name GameEventDef -o GameEventDef.sql
cat GameEventDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name DesignationDef -o DesignationDef.sql
cat DesignationDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name RespactPartyDef -o RespactPartyDef.sql
cat RespactPartyDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name RespactPhaseDef -o RespactPhaseDef.sql
cat RespactPhaseDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name RideDef -o RideDef.sql
cat RideDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name FairyDef -o FairyDef.sql
cat FairyDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name PetDef -o PetDef.sql
cat PetDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name PetLevelDef -o PetLevelDef.sql
cat PetLevelDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name PetTraitDef -o PetTraitDef.sql
cat PetTraitDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name PetStrengthenDef -o PetStrengthenDef.sql
cat PetStrengthenDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name OverlayTypeRelationDef  -o OverlayTypeRelationDef.sql
cat OverlayTypeRelationDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql 

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name AddAttrBaseDef -o AddAttrBaseDef.sql
cat AddAttrBaseDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name AddAttrGroupDef -o AddAttrGroupDef.sql
cat AddAttrGroupDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name WorldBootyDef -o WorldBootyDef.sql
cat WorldBootyDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name DiffBootyDef -o DiffBootyDef.sql
cat DiffBootyDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name TempleDef -o TempleDef.sql
cat TempleDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name OneDayGift -o OneDayGift.sql
cat OneDayGift.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name LVGift -o LVGift.sql
cat LVGift.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name FixedAddAttr -o FixedAddAttr.sql
cat FixedAddAttr.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name BCTDef -o BCTDef.sql
cat BCTDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql


meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name RndAwdDef -o RndAwdDef.sql
cat RndAwdDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name ActiveHelper -o ActiveHelper.sql
cat ActiveHelper.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name CangBaoTuDef -o CangBaoTuDef.sql
cat CangBaoTuDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name MonMachineDef -o MonMachineDef.sql
cat MonMachine.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name ClanTechDef -o ClanTechDef.sql
cat ClanTechDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name ClanProductDef -o ClanProductDef.sql
cat ClanProductDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name ItemDropInfoDef -o ItemDropInfoDef.sql
cat ItemDropInfoDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name ContOnlineGiftDef -o ContOnlineGiftDef.sql
cat ContOnlineGiftDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name MonsterStateSkillDef -o MonsterStateSkillDef.sql
cat MonsterStateSkillDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name ClanBuildingDisplayInfo -o ClanBuildingDisplayInfo.sql
cat ClanBuildingDisplayInfo.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name FairyStarStuffDef -o FairyStarStuffDef.sql
cat FairyStarStuffDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name FairyStarDef -o FairyStarDef.sql
cat FairyStarDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name OnlineGiftGroup -o OnlineGiftGroup.sql
cat OnlineGiftGroup.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name StarStoneDef -o StarStoneDef.sql
cat StarStoneDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name StarFuncDef -o StarFuncDef.sql
cat StarFuncDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name ArmFenJieDef -o ArmFenJieDef.sql
cat ArmFenJieDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name BattleGloryDef -o BattleGloryDef.sql
cat BattleGloryDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name MediaGiftDef -o MediaGiftDef.sql
cat MediaGiftDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name LtrGiftDef -o LtrGiftDef.sql
cat LtrGiftDef.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name GmTrade -o GmTrade.sql
cat GmTrade.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

meta2tab --charset latin1 --meta_file resdb_meta.tdr --meta_name LevelFeng -o LevelFeng.sql
cat LevelFeng.sql >> ResTable.sql
echo -e "\n" >> ResTable.sql

rm -rf StarStoneDef.sql StarFuncDef.sql  LevelFeng.sql GmTrade.sql ArmFenJieDef.sql ItemDropInfoDef.sql ActiveHelper.sql OverlayTypeRelationDef.sql  NpcInfoDef.sql TaskDef.sql PatrolDef.sql MapDef.sql ProcEvent.sql MonsterDef.sql ItemDef.sql NpcSellList.sql TransList.sql MapNpc.sql MapMonster.sql  PworldDef.sql  BirthMapDef.sql  ActionStatus.sql LevelUP.sql  ItemDisplayInfo.sql NpcDisplayInfo.sql SkillDef.sql ArmLevelHurtDef.sql DiathesisDef.sql FittingsDef.sql StatusDef.sql StatusOverlayDef.sql AddAttrDef.sql SuitSet.sql MonDynAttrDef.sql QiYueDef.sql MonDynExpLimitDef.sql SceneAreaDef.sql ArmGradeDef.sql GemDef.sql BirthItemDef.sql BootyGroupDef.sql MonsterSpeechDef.sql ArmAddAttrDef.sql WorldMapSearchDef.sql OnlineAwardDef.sql TaskLimitGroup.sql ClanCityBuildingLevelDef.sql ClanCityLevelDef.sql ClanLevelDef.sql MonsterColorDef.sql GameEventDef.sql DesignationDef.sql RespactPartyDef.sql RespactPhaseDef.sql RideDef.sql FairyDef.sql PetDef.sql PetLevel.sql PetTraitDef.sql PetStrengthenDef.sql AddAttrBaseDef.sql AddAttrGroupDef.sql LVGift.sql OneDayGift.sql FixedAddAttr.sql RndAwdDef.sql BCTDef.sql CangBaoTuDef.sql ClanTechDef.sql ClanProductDef.sql ContOnlineGiftDef.sql FairyStarStuffDef.sql FairyStarDef.sql OnlineGiftGroup ArmRandDef.sql ArmHideAttrDef.sql
