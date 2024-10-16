#pragma warning( disable : 4996 )

#include "GameDBData.h"
#include "OgreLog.h"
#include "OgreResourceManager.h"
#include "OgreDataStream.h"
#include "cs_net.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>

using namespace Ogre;

DBDataManager::DBDataManager() : m_pResDB(NULL)
{
	memset(&m_AllResData[0], 0, sizeof(m_AllResData));
	//DBDataManager::INVALDSTATUSDEF.StatusID = -1;
}

DBDataManager::~DBDataManager()
{
	delete m_pResDB;

	for(int i=0; i<DBRES_MAX_NUM; i++)
	{
		delete m_AllResData[i].pbuffer;
	}
}

bool DBDataManager::OfflineLoadData(const GameConfig &cfg )
{
	m_ResDBMode = cfg.resdb_mode;
	if(cfg.resdb_mode == RESDB_DB)
	{
		m_pResDB = CreateResDBFromRealDB("db/resdb_meta.tdr", cfg.resdb_server, cfg.resdb_database, cfg.resdb_user, cfg.resdb_passwd);
		if(m_pResDB == NULL)
		{
			LOG_SEVERE("open realdb %s failed", cfg.resdb_server);
			return false;
		}

		loadFromRealDB("NpcDisplayInfo",	sizeof(tagNpcDisplayInfo),		m_AllResData[DBRES_NPCDISPLAYINFO]);
		loadFromRealDB("ItemDef",			sizeof(tagItemDef),				m_AllResData[DBRES_ITEMDEF]);
		loadFromRealDB("ItemDisplayInfo",	sizeof(tagItemDisplayInfo),		m_AllResData[DBRES_ITEMDISPLAYINFO]);
		loadFromRealDB("LevelUP",			sizeof(tagLevelUP),				m_AllResData[DBRES_LEVELUP]);
		loadFromRealDB("ArmLevelHurtDef",	sizeof(ARMLEVELHURTDEF),		m_AllResData[DBRES_ARMLEVELHURTDEF]);
		loadFromRealDB("AddAttrDef",		sizeof(tagAddAttrDef),			m_AllResData[DBRES_ADDATTRDEF]);
		loadFromRealDB("ArmAddAttrDef",		sizeof(tagArmAddAttrDef),		m_AllResData[DBRES_ARMADDATTRDEF] );
		loadFromRealDB("SuitSet",			sizeof(tagSuitSet),				m_AllResData[DBRES_SUITSET] );
		loadFromRealDB("ArmGradeDef",		sizeof(tagArmGradeDef),			m_AllResData[DBRES_ARMGRADEDEF] );
		loadFromRealDB("AddAttrBaseDef",	sizeof(tagAddAttrBaseDef),		m_AllResData[DBRES_ADDATTRBASEDEF] );
		loadFromRealDB( "ArmRandDef",		sizeof(tagArmRandDef),			m_AllResData[DBRES_ARMRANDDEF]);
		loadFromRealDB( "NewAddAttrDef",	sizeof(tagNewAddAttrDef),		m_AllResData[DBRES_NEWADDATTRDEF]);
		loadFromRealDB( "AddAttrRandDef",	sizeof(tagAddAttrRandDef),		m_AllResData[DBRES_ADDATTRRANDDEF]);
		loadFromRealDB( "ItemResultDesc",	sizeof(tagItemResultDesc),		m_AllResData[DBRES_ITEMRESULTDESC] );
	}
	else
	{
		m_DBFileDir = cfg.resdb_dir;
		loadFromDBFile("NpcDisplayInfo.bin",	sizeof(tagNpcDisplayInfo),		m_AllResData[DBRES_NPCDISPLAYINFO]);
		loadFromDBFile("ItemDef.bin",			sizeof(tagItemDef),				m_AllResData[DBRES_ITEMDEF]);
		loadFromDBFile("ItemDisplayInfo.bin",	sizeof(tagItemDisplayInfo),		m_AllResData[DBRES_ITEMDISPLAYINFO]);
		loadFromDBFile("LevelUP.bin",			sizeof(tagLevelUP),				m_AllResData[DBRES_LEVELUP]);
		loadFromDBFile("ArmLevelHurtDef.bin",	sizeof(ARMLEVELHURTDEF),		m_AllResData[DBRES_ARMLEVELHURTDEF]);
		loadFromDBFile("AddAttrDef.bin",		sizeof(tagAddAttrDef),			m_AllResData[DBRES_ADDATTRDEF]);
		loadFromDBFile("ArmAddAttrDef.bin",		sizeof(tagArmAddAttrDef),		m_AllResData[DBRES_ARMADDATTRDEF] );
		loadFromDBFile("SuitSet.bin",			sizeof(tagSuitSet),				m_AllResData[DBRES_SUITSET] );
		loadFromDBFile("ArmGradeDef.bin",		sizeof(tagArmGradeDef),			m_AllResData[DBRES_ARMGRADEDEF] );
		loadFromDBFile("AddAttrBaseDef.bin",	sizeof(tagAddAttrBaseDef),		m_AllResData[DBRES_ADDATTRBASEDEF] );
		loadFromDBFile( "ArmRandDef.bin",		sizeof(tagArmRandDef),			m_AllResData[DBRES_ARMRANDDEF]);
		loadFromDBFile( "NewAddAttrDef.bin",	sizeof(tagNewAddAttrDef),		m_AllResData[DBRES_NEWADDATTRDEF]);
		loadFromDBFile( "AddAttrRandDef.bin",	sizeof(tagAddAttrRandDef),		m_AllResData[DBRES_ADDATTRRANDDEF]);
		loadFromDBFile( "ItemResultDesc.bin",	sizeof(tagItemResultDesc),		m_AllResData[DBRES_ITEMRESULTDESC] );
	}

	LOG_INFO("load db data succeeded");
	return true;
}

bool DBDataManager::LoadData(const GameConfig &cfg )
{
	m_ResDBMode = cfg.resdb_mode;
	if(cfg.resdb_mode == RESDB_DB)
	{
		m_pResDB = CreateResDBFromRealDB("db/resdb_meta.tdr", cfg.resdb_server, cfg.resdb_database, cfg.resdb_user, cfg.resdb_passwd);
		if(m_pResDB == NULL)
		{
			LOG_SEVERE("open realdb %s failed", cfg.resdb_server);
			return false;
		}

		loadFromRealDB("SkillDef",			sizeof(tagSkillDef),			m_AllResData[DBRES_SKILLDEF]);
		loadFromRealDB("MonsterDef",		sizeof(tagMonsterDef),			m_AllResData[DBRES_MONSTERDEF]);
		loadFromRealDB("NpcDisplayInfo",	sizeof(tagNpcDisplayInfo),		m_AllResData[DBRES_NPCDISPLAYINFO]);
		loadFromRealDB("MapDef",			sizeof(tagMapDef),				m_AllResData[DBRES_MAPDEF]);
		loadFromRealDB("StatusDef",			sizeof(tagStatusDef),			m_AllResData[DBRES_STATUSDEF]);
		loadFromRealDB("StatusOverlayDef",	sizeof(tagStatusOverlayDef),	m_AllResData[DBRES_STATUSOVERLAYDEF]);
		loadFromRealDB("ItemDef",			sizeof(tagItemDef),				m_AllResData[DBRES_ITEMDEF]);
		loadFromRealDB("ItemDisplayInfo",	sizeof(tagItemDisplayInfo),		m_AllResData[DBRES_ITEMDISPLAYINFO]);
		loadFromRealDB("LevelUP",			sizeof(tagLevelUP),				m_AllResData[DBRES_LEVELUP]);
		loadFromRealDB("MapNpc",			sizeof(tagMapNpc),				m_AllResData[DBRES_MAPNPC]);
		loadFromRealDB("ArmLevelHurtDef",	sizeof(ARMLEVELHURTDEF),		m_AllResData[DBRES_ARMLEVELHURTDEF]);
		loadFromRealDB("TaskDef",			sizeof(tagTaskDef),				m_AllResData[DBRES_TASKDEF]);
		loadFromRealDB("TaskRecycleDef",	sizeof(tagTaskRecycleDef),		m_AllResData[DBRES_TASKRECYCLEDEF]);
		loadFromRealDB("TaskLimitGroup",	sizeof(tagTaskLimitGroup),		m_AllResData[DBRES_TASKLIMITGROUP]);
		loadFromRealDB("NpcInfoDef",		sizeof(tagNpcInfoDef),			m_AllResData[DBRES_NPCINFO]);
		loadFromRealDB("AddAttrDef",		sizeof(tagAddAttrDef),			m_AllResData[DBRES_ADDATTRDEF]);
		loadFromRealDB("QiYueDef",			sizeof(tagQiYueDef),			m_AllResData[DBRES_QIYUEDEF]);
		loadFromRealDB("DiathesisDef",		sizeof(tagDiathesisDef),		m_AllResData[DBRES_DIATHESIS]);
		loadFromRealDB("GemDef",			sizeof(tagGemDef),				m_AllResData[DBRES_GEMDEF]);
		loadFromRealDB("SceneAreaDef",		sizeof(tagSceneAreaDef),		m_AllResData[DBRES_SCENEAREADEF]);
		loadFromRealDB("MonsterSpeechDef",	sizeof(tagMonsterSpeechDef),	m_AllResData[DBRES_MONSTERSPEECHDEF] );
		loadFromRealDB("ArmAddAttrDef",		sizeof(tagArmAddAttrDef),		m_AllResData[DBRES_ARMADDATTRDEF] );
		loadFromRealDB("SuitSet",			sizeof(tagSuitSet),				m_AllResData[DBRES_SUITSET] );
		loadFromRealDB("ArmGradeDef",		sizeof(tagArmGradeDef),			m_AllResData[DBRES_ARMGRADEDEF] );
		loadFromRealDB("WorldMapSearchDef",	sizeof(tagWorldMapSearchDef),	m_AllResData[DBRES_WORLDMAPSEARCHDEF] );
		loadFromRealDB("ClanLevelDef",		sizeof(tagClanLevelDef),		m_AllResData[DBRES_CLANLEVELDEF] );
		loadFromRealDB("ClanCityLevelDef",	sizeof(tagClanCityLevelDef),	m_AllResData[DBRES_CLANCITYLEVELDEF] );
		loadFromRealDB("ClanCityBuildingLevelDef",sizeof(tagClanCityBuildingLevelDef),
			m_AllResData[DBRES_CLANCITYBUILDINGLEVELDEF] );
        loadFromRealDB("DesignationDef",	sizeof(tagDesignationDef),		m_AllResData[DBRES_DESIGNATIONDEF] );
        loadFromRealDB("RespactPartyDef",	sizeof(tagRespactPartyDef),		m_AllResData[DBRES_RESPACTPARTYDEF] );
        loadFromRealDB("RespactPhaseDef",	sizeof(tagRespactPhaseDef),		m_AllResData[DBRES_RESPACTPHASEDEF] );
        loadFromRealDB("FairyDef",	        sizeof(tagFairyDef),		    m_AllResData[DBRES_FAIRYDEF] );
		loadFromRealDB("RideDef",	        sizeof(tagRideDef),				m_AllResData[DBRES_RIDEDEF] );
		loadFromRealDB("PworldDef",	        sizeof(tagPworldDef),			m_AllResData[DBRES_PWORLDDEF] );
		loadFromRealDB("PetDef",	        sizeof(tagPetDef),				m_AllResData[DBRES_PETDEF] );
		loadFromRealDB("PetSkillGridDef",	sizeof(tagPetSkillGridDef),		m_AllResData[DBRES_PETSKILLGRIDDEF] );
		loadFromRealDB("AddAttrBaseDef",	sizeof(tagAddAttrBaseDef),		m_AllResData[DBRES_ADDATTRBASEDEF] );
		loadFromRealDB( "PetStrengthenDef",	sizeof(tagPetStrengthenDef),	m_AllResData[DBRES_PETSTRENGTHENDEF] );
		loadFromRealDB( "PetLevelDef",		sizeof(tagPetLevelDef),			m_AllResData[DBRES_PETLEVELDEF] );
        loadFromRealDB( "PetTraitDef",		sizeof(tagPetTraitDef),			m_AllResData[DBRES_PETTRAITDEF] );
        loadFromRealDB( "OneDayGift",	    sizeof(tagOneDayGift),		    m_AllResData[DBRES_ONEDAYGIFTDEF] );
        loadFromRealDB( "LVGift",	        sizeof(tagLVGift),		        m_AllResData[DBRES_LVGIFTDEF] );
        loadFromRealDB( "FixedAddAttr",	    sizeof(tagFixedAddAttr),		m_AllResData[DBRES_FIXEDADDATTR] );
		loadFromRealDB( "BCTDef",			sizeof(tagBCTDef),				m_AllResData[DBRES_BCT] );
		loadFromRealDB( "RndAwdDef",		sizeof(tagRndAwdDef),			m_AllResData[DBRES_RNDAWD] );
		loadFromRealDB( "ActiveHelper",		sizeof(tagActiveHelper),		m_AllResData[DBRES_ACTIVEHELPER]);
		loadFromRealDB( "SecondarySkillDef",sizeof(tagSecondarySkillDef),	m_AllResData[DBRES_SECONDARYSKILL]);
		loadFromRealDB( "RecipeDef",		sizeof(tagRecipeDef),			m_AllResData[DBRES_RECIPE]);
		loadFromRealDB( "CangBaoTuDef",		sizeof(tagCangBaoTuDef),		m_AllResData[DBRES_CANGBAOTU]);
 		loadFromRealDB( "ClanTechDef",		sizeof(tagClanTechDef),			m_AllResData[DBRES_CLANTECHDEF]);
 		loadFromRealDB( "ClanProductDef",	sizeof(tagClanProductDef),		m_AllResData[DBRES_CLANPRODUCTDEF]);
		loadFromRealDB( "MonMachineDef",	sizeof(tagMonMachineDef),		m_AllResData[DBRES_MONMACHINEDEF]);
		loadFromRealDB( "ContOnlineGiftDef",sizeof(tagContOnlineGiftDef),	m_AllResData[DBRES_CONTONLINEGIFTDEF]);
		loadFromRealDB( "BattleDef",		sizeof(tagBattleDef),			m_AllResData[DBRES_BATTLEDEF]);
		loadFromRealDB( "BirthItemDef",		sizeof(tagBirthItemDef),		m_AllResData[DBRES_BIRTHITEMDEF]);
		loadFromRealDB( "ClanBuildingDisplayInfo",sizeof(tagClanBuildingDisplayInfo), m_AllResData[DBRES_CLAN_BUILDINGDISPLAYINFO]);
		loadFromRealDB( "FairyStarStuffDef",sizeof(tagFairyStarStuffDef),	m_AllResData[DBRES_FAIRYSTARSTUFFDEF]);
		loadFromRealDB( "FairyStarDef",		sizeof(tagFairyStarDef),		m_AllResData[DBRES_FAIRYSTARDEF]);
		loadFromRealDB( "OnlineGiftGroup",	sizeof(tagOnlineGiftGroup),		m_AllResData[DBRES_ONLINEGIFTGROUP]);
		loadFromRealDB( "ArmRandDef",		sizeof(tagArmRandDef),			m_AllResData[DBRES_ARMRANDDEF]);
		loadFromRealDB( "GameEventDef",		sizeof(tagGameEventDef),		m_AllResData[DBRES_GAMEEVENTDEF]);
		loadFromRealDB( "DiffPworldDef",	sizeof(tagDiffPworldDef),		m_AllResData[DBRES_DIFFPWORLDDEF]);
		loadFromRealDB( "WorldCityBuildDef",sizeof(tagWorldCityBuildDef),	m_AllResData[DBRES_WORLDCITYBUILDDEF]);
		loadFromRealDB( "CityProductDef",	sizeof(tagCityProductDef),		m_AllResData[DBRES_CITYPRODUCTDEF]);
		loadFromRealDB( "ArmFenJieDef",		sizeof(tagArmFenJieDef),		m_AllResData[DBRES_ARMFENJIDEF]);
// 		loadFromRealDB( "StarVipDef",		sizeof(tagStarVipDef),			m_AllResData[DBRES_STARVIPDEF]);
		loadFromRealDB( "StarStoneDef",		sizeof(tagStarStoneDef),		m_AllResData[DBRES_STARSTONEDEF]);
		loadFromRealDB( "StarFuncDef",		sizeof(tagStarFuncDef),			m_AllResData[DBRES_STARFUNCDEF]);
		loadFromRealDB( "MediaGiftDef",		sizeof(tagMediaGiftDef),		m_AllResData[DBRES_MEDIAGIFTDEF]);
		loadFromRealDB( "LtrGiftDef",		sizeof(tagLtrGiftDef),			m_AllResData[DBRES_LTRGIFTDEF]);
		loadFromRealDB( "GemNewDef",		sizeof(tagGemNewDef),			m_AllResData[DBRES_GEMNEWDEF]);
		loadFromRealDB( "GemLevelStarDef",	sizeof(tagGemLevelStarDef),		m_AllResData[DBEES_GEMLEVELSTARDEF]);
		loadFromRealDB( "GemAttrStarDef",	sizeof(tagGemAttrStarDef),		m_AllResData[DBRES_GEMATTRSTARDEF]);
		loadFromRealDB( "RuneAddAttrDef",	sizeof(tagRuneAddAttrDef),		m_AllResData[DBRES_RUNEADDATTRDEF]);
		loadFromRealDB( "UseDef",			sizeof(tagUseDef),				m_AllResData[DBRES_USEDEF]);
		loadFromRealDB( "ArmRecastDef",		sizeof(tagArmRecastDef),		m_AllResData[DBRES_ARMRECASTDEF] );
		loadFromRealDB( "ArmStarDef",		sizeof(tagArmStarDef),			m_AllResData[DBRES_ARMSTARDEF]);
		loadFromRealDB( "ArmHideAttrDef",	sizeof(tagArmHideAttrDef),		m_AllResData[DBRES_ARMHIDEATTRDEF]);
		loadFromRealDB( "DailyGoalAward",	sizeof(tagDailyGoalAward),		m_AllResData[DBRES_DAILYGOALAWARD]);
		loadFromRealDB( "UnrealDef",		sizeof(tagUnrealDef),			m_AllResData[DBRES_UNREALDEF]);
		loadFromRealDB( "NewAddAttrDef",	sizeof(tagNewAddAttrDef),		m_AllResData[DBRES_NEWADDATTRDEF]);
		loadFromRealDB( "AddAttrRandDef",	sizeof(tagAddAttrRandDef),		m_AllResData[DBRES_ADDATTRRANDDEF]);
		loadFromRealDB( "PetWashGeniusDef",	sizeof(tagPetWashGeniusDef),	m_AllResData[DBRES_PETWASHGENIUSDEF]);
		loadFromRealDB( "SparDef",			sizeof(tagSparDef),				m_AllResData[DBRES_SPARDEF]);
		loadFromRealDB( "AchievementCategoryDef",	sizeof(tagAchievementCategoryDef),m_AllResData[DBRES_ACHIEVEMENTCATEGORYDEF]);
		loadFromRealDB( "AchievementDef",	sizeof(tagAchievementDef),		m_AllResData[DBRES_ACHIEVEMENTDEF]);
		loadFromRealDB( "ItemResultDesc",	sizeof(tagItemResultDesc),		m_AllResData[DBRES_ITEMRESULTDESC] );
		loadFromRealDB( "MapTransDef",		sizeof(tagMapTransDef),			m_AllResData[DBRES_MAPTRANSDEF]);
		loadFromRealDB( "SJTaskDef",		sizeof(tagSJTaskDef),			m_AllResData[DBRES_SJTASKDEF]);
		loadFromRealDB( "ClanSpyAwardDropDef",	sizeof(tagClanSpyAwardDropDef),m_AllResData[DBRES_CLANSPYAWARDDROPDEF]);
		loadFromRealDB( "ClanSpyTransDef",	sizeof(tagClanSpyTransDef),		m_AllResData[DBRES_CLANSPYTRANSDEF]);
		loadFromRealDB( "SubJectDef",       sizeof(tagSubJectDef),          m_AllResData[DBRES_SUBJECTDEF]);
		loadFromRealDB( "StrongPointDef",	sizeof(tagStrongPointDef),		m_AllResData[DBRES_STRONGPOINTDEF]);
		loadFromRealDB( "ClanSkillDef",		sizeof(tagClanSkillDef),		m_AllResData[DBRES_CLANSKILLDEF]);
		loadFromRealDB( "BannerDef",		sizeof(tagBannerDef),			m_AllResData[DBRES_BANNERDEF]);
		loadFromRealDB( "ReviveDef",		sizeof(tagReviveDef),			m_AllResData[DBRES_REVIVEDEF]);
		loadFromRealDB( "ClanPayDef",		sizeof(tagClanPayDef),			m_AllResData[DBRES_CLANPAYDEF]);
		loadFromRealDB( "LevelExpDef",		sizeof(tagLevelExpDef),			m_AllResData[DBRES_LEVELEXPDEF]);
		loadFromRealDB( "LevelBuffExpDef",	sizeof(tagLevelBuffExpDef),		m_AllResData[DBRES_LEVELBUFFEXPDEF]);
		loadFromRealDB( "OfflineExpParameter",	sizeof(tagOfflineExpParameter),m_AllResData[DBRES_OFFLINEEXPPARAMETER]);
		loadFromRealDB( "RidePinZhiDef",	sizeof(tagRidePinZhiDef),		m_AllResData[DBRES_RIDEPINZHIDEF]);
		loadFromRealDB( "MapAwardOreDef",	sizeof(tagMapAwardOreDef),		m_AllResData[DBRES_MAPAWARDOREDEF] );
		loadFromRealDB( "RongluDef",		sizeof(tagRongluDef),			m_AllResData[DBRES_RONGLUDEF] );
		loadFromRealDB( "GeniusAttrDef",	sizeof(tagGeniusAttrDef),		m_AllResData[DBRES_GENIUSATTRDEF] );
		loadFromRealDB( "GeniusCareerDef",	sizeof(tagGeniusCareerDef),		m_AllResData[DBRES_GENIUSCAREERDEF]);
		loadFromRealDB( "GeniusAddDef",		sizeof(tagGeniusAddDef),		m_AllResData[DBRES_GENIUSADDDEF]);
		loadFromRealDB( "BaoXiangDef",		sizeof(tagBaoXiangDef),			m_AllResData[DBRES_BAOXIANGDEF]);
		loadFromRealDB( "ArmLevelUPDef",	sizeof(tagArmLevelUPDef),		m_AllResData[DBRES_ARMLEVELUPDEF] );
		loadFromRealDB( "WorldBossDef",		sizeof(tagWorldBossDef),		m_AllResData[DBRES_WORLDBOSSDEF]);
		loadFromRealDB( "LevelFeng",		sizeof(tagLevelFeng),			m_AllResData[DBRES_LEVELFENG] );
		loadFromRealDB( "WorldCityFightAwardDef",	sizeof(tagWorldCityFightAwardDef),	m_AllResData[DBRES_WORLDCITYFIGHTAWARDDEF]);
		loadFromRealDB( "BusyDef",			sizeof(tagBusyDef),				m_AllResData[DBRES_BUSYDEF] );
		loadFromRealDB( "BusyUseDef",		sizeof(tagBusyUseDef),			m_AllResData[DBRES_BUSYUSEDEF] );
		loadFromRealDB( "HighWashUseDef",	sizeof(tagHighWashUseDef),		m_AllResData[DBRES_HIGHWASHUSEDEF]);
		loadFromRealDB( "MapOreDef",		sizeof(tagMapOreDef),			m_AllResData[DBRES_MAPOREDEF]);
		loadFromRealDB( "PetGeniusDef",		sizeof(tagPetGeniusDef),		m_AllResData[DBRES_PETGENIUSDEF]);
		loadFromRealDB( "SpartaLevelDef",	sizeof(tagSpartaLevelDef),		m_AllResData[DBRES_SPARTALEVELDEF]);
		loadFromRealDB( "BackLevelDef",		sizeof(tagBackLevelDef),		m_AllResData[DBRES_BACKLEVELDEF]);
		
		loadFromRealDB( "NewRoleGift",		sizeof(tagNewRoleGift),			m_AllResData[DBRES_NEWROLEGIFTDEF]);
		loadFromRealDB( "ContCoverGift",	sizeof(tagContCoverGift),		m_AllResData[DBRES_CONCOVERGIFTDEF]);
		loadFromRealDB( "SchoolLevelDef",	sizeof(tagSchoolLevelDef),		m_AllResData[DBRES_SCHOOLLEVELDEF]);
		loadFromRealDB( "SchoolXinDeDef",	sizeof(tagSchoolXinDeDef),		m_AllResData[DBRES_SCHOOLXINDEDEF]);
		loadFromRealDB( "XuKongPosDef",		sizeof(tagXuKongPosDef),		m_AllResData[DBRES_XUKONGPOSDEF]);
		loadFromRealDB( "CardBoxDef",		sizeof(tagCardBoxDef),			m_AllResData[DBRES_CARDBOXDEF]);
		loadFromRealDB( "CardBoxGroupDef",	sizeof(tagCardBoxGroupDef),		m_AllResData[DBRES_CARDBOXGROUPDEF]);
		loadFromRealDB( "ArmBuChangDef",	sizeof(tagArmBuChangDef),		m_AllResData[DBRES_ARMBUCHANGDEF]);
		loadFromRealDB( "BuChangDef",		sizeof(tagBuChangDef),			m_AllResData[DBRES_BUCHANGDEF]);
		loadFromRealDB( "StoreGridDef",		sizeof(tagStoreGridDef),		m_AllResData[DBRES_STOREGRIDDEF]);
		loadFromRealDB( "ArmStepUpDef",		sizeof(tagArmStepUpDef),		m_AllResData[DBRES_ARMSTEPUPDEF]);
		loadFromRealDB( "RongYanJuShouDef",	sizeof(tagRongYanJuShouDef),	m_AllResData[DBRES_RONGYANJUSHOUDEF]);
		loadFromRealDB( "RongYanMonDynAttrDef",		sizeof(tagRongYanMonDynAttrDef),		m_AllResData[DBRES_RONGYANMONATTRDEF]);
		loadFromRealDB( "ArmGradeValDef",	sizeof(tagArmGradeValDef),		m_AllResData[DBRES_ARMGRADEVALDEF]);
		loadFromRealDB( "ArmReSetDef",		sizeof(tagArmReSetDef),			m_AllResData[DBRES_ARMRESETDEF]);
		loadFromRealDB( "GeniusMdfDef",		sizeof(tagGeniusMdfDef),		m_AllResData[DBRES_GENIUSMDFDEF]);
		loadFromRealDB( "ArmSkillNumDef",	sizeof(tagArmSkillNumDef),		m_AllResData[DBRES_ARMSKILLNUMDEF]);
		loadFromRealDB( "ArmSkillAttrDef",	sizeof(tagArmSkillAttrDef),		m_AllResData[DBRES_ARMSKILLATTRDEF]);
		loadFromRealDB( "PetMoneyDef",		sizeof(tagPetMoneyDef),			m_AllResData[DBRES_PETMONEYDEF]);
		loadFromRealDB( "PetImpUpDef",		sizeof(tagPetImpUpDef),			m_AllResData[DBRES_PETIMPUPDEF]);
		loadFromRealDB( "PetImpUpAttrDef",	sizeof(tagPetImpUpAttrDef),		m_AllResData[DBRES_PETIMPUPATTRDEF]);
		loadFromRealDB( "GemPagDef",		sizeof(tagGemPagDef),			m_AllResData[DBRES_GEMPAGDEF]);
		loadFromRealDB( "ArmTeshuDef",		sizeof(tagArmTeshuDef),			m_AllResData[DBRES_ARMTESHUDEF]);
		loadFromRealDB( "SpanStrongDef",	sizeof(tagSpanStrongDef),			m_AllResData[DBRES_SPANSTRONGDEF]);
		loadFromRealDB( "HomeLvlDef",		sizeof(tagHomeLvlDef),					m_AllResData[DBRES_HOMELVLDEF] );
		loadFromRealDB( "HomeMonLvlDef",		sizeof(tagHomeMonLvlDef),					m_AllResData[DBRES_HOMEMONLVLDEF] );
		loadFromRealDB( "HomeFarmDef",	sizeof(tagHomeFarmDef),				m_AllResData[DBRES_HOMEFARMDEF]);
		loadFromRealDB( "HomeMonSegDef",	sizeof(tagHomeMonSegDef),				m_AllResData[DBRES_HOMEMONSEGDEF]);
		loadFromRealDB( "ArmSkillRecastDef",	sizeof(tagArmSkillRecastDef),	m_AllResData[DBRES_ARMSKILLRECASTDEF]);

		/*
		m_pSkillTable		= m_pResDB->GetTable("SkillInfo");
		m_pSkillDefTable	= m_pResDB->GetTable("SkillDef");
		m_pTaskTable		= m_pResDB->GetTable("TaskDesc");
		m_pMonsterTable		= m_pResDB->GetTable("MonsterDef");
		m_pNpcDisplayTable	= m_pResDB->GetTable("NpcDisplayInfo");
		m_pMapDefTable      = m_pResDB->GetTable("MapDef");
		m_pMapMonsterTable  = m_pResDB->GetTable("MapMonster");
		m_pMapProcTable     = m_pResDB->GetTable("ProcEvent");
		m_pStatusTable		= m_pResDB->GetTable("StatusDef");
		m_pItemDefTable		= m_pResDB->GetTable("ItemDef");
		m_pProcEventTable	= m_pResDB->GetTable("ProcEvent");
		m_pLevelUpTable		= m_pResDB->GetTable("LevelUP");
		*/
	}
	else
	{
		m_DBFileDir = cfg.resdb_dir;

		loadFromDBFile( "SkillDef.bin",			sizeof(tagSkillDef),			m_AllResData[DBRES_SKILLDEF] );
		loadFromDBFile( "MonsterDef.bin",		sizeof(tagMonsterDef),			m_AllResData[DBRES_MONSTERDEF] );
		loadFromDBFile( "NpcDisplayInfo.bin",	sizeof(tagNpcDisplayInfo),		m_AllResData[DBRES_NPCDISPLAYINFO] );
		loadFromDBFile( "MapDef.bin",			sizeof(tagMapDef),				m_AllResData[DBRES_MAPDEF] );
		loadFromDBFile( "StatusDef.bin",		sizeof(tagStatusDef),			m_AllResData[DBRES_STATUSDEF] );
		loadFromDBFile("StatusOverlayDef.bin",	sizeof(tagStatusOverlayDef),	m_AllResData[DBRES_STATUSOVERLAYDEF]);
		loadFromDBFile( "ItemDef.bin",			sizeof(tagItemDef),				m_AllResData[DBRES_ITEMDEF] );
		loadFromDBFile( "ItemDisplayInfo.bin",	sizeof(tagItemDisplayInfo),		m_AllResData[DBRES_ITEMDISPLAYINFO] );
		loadFromDBFile( "LevelUP.bin",			sizeof(tagLevelUP),				m_AllResData[DBRES_LEVELUP] );
		loadFromDBFile( "MapNpc.bin",			sizeof(tagMapNpc),				m_AllResData[DBRES_MAPNPC] );
		loadFromDBFile( "ArmLevelHurtDef.bin",	sizeof(ARMLEVELHURTDEF),		m_AllResData[DBRES_ARMLEVELHURTDEF] );
		loadFromDBFile( "TaskDef.bin",			sizeof(tagTaskDef),				m_AllResData[DBRES_TASKDEF] );
		loadFromDBFile( "TaskRecycleDef.bin",	sizeof(tagTaskRecycleDef),		m_AllResData[DBRES_TASKRECYCLEDEF] );
		loadFromDBFile( "TaskLimitGroup.bin",	sizeof(tagTaskLimitGroup),		m_AllResData[DBRES_TASKLIMITGROUP] );
		loadFromDBFile( "NpcInfoDef.bin",		sizeof(tagNpcInfoDef),			m_AllResData[DBRES_NPCINFO] );
		loadFromDBFile( "AddAttrDef.bin",		sizeof(tagAddAttrDef),			m_AllResData[DBRES_ADDATTRDEF] );
		loadFromDBFile( "QiYueDef.bin",			sizeof(tagQiYueDef),			m_AllResData[DBRES_QIYUEDEF] );
		loadFromDBFile( "DiathesisDef.bin",		sizeof(tagDiathesisDef),		m_AllResData[DBRES_DIATHESIS] );
		loadFromDBFile( "GemDef.bin",			sizeof(tagGemDef),				m_AllResData[DBRES_GEMDEF] );
		loadFromDBFile( "SceneAreaDef.bin",		sizeof(tagSceneAreaDef),		m_AllResData[DBRES_SCENEAREADEF]);
		loadFromDBFile( "MonsterSpeechDef.bin",	sizeof(tagMonsterSpeechDef),	m_AllResData[DBRES_MONSTERSPEECHDEF] );
		loadFromDBFile( "ArmAddAttrDef.bin",	sizeof(tagArmAddAttrDef),		m_AllResData[DBRES_ARMADDATTRDEF] );
		loadFromDBFile( "SuitSet.bin",			sizeof(tagSuitSet),				m_AllResData[DBRES_SUITSET] );
		loadFromDBFile( "ArmGradeDef.bin",		sizeof(tagArmGradeDef),			m_AllResData[DBRES_ARMGRADEDEF] );
		loadFromDBFile( "WorldMapSearchDef.bin",sizeof(tagWorldMapSearchDef),	m_AllResData[DBRES_WORLDMAPSEARCHDEF] );
		loadFromDBFile( "ClanLevelDef.bin",		sizeof(tagClanLevelDef),		m_AllResData[DBRES_CLANLEVELDEF] );
		loadFromDBFile( "ClanCityLevelDef.bin",	sizeof(tagClanCityLevelDef),	m_AllResData[DBRES_CLANCITYLEVELDEF] );
		loadFromDBFile( "ClanCityBuildingLevelDef.bin",sizeof(tagClanCityBuildingLevelDef),
			m_AllResData[DBRES_CLANCITYBUILDINGLEVELDEF] );
        loadFromDBFile( "DesignationDef.bin",	sizeof(tagDesignationDef),		m_AllResData[DBRES_DESIGNATIONDEF] );
		loadFromDBFile( "RespactPartyDef.bin",	sizeof(tagRespactPartyDef),		m_AllResData[DBRES_RESPACTPARTYDEF] );
        loadFromDBFile( "RespactPhaseDef.bin",	sizeof(tagRespactPhaseDef),		m_AllResData[DBRES_RESPACTPHASEDEF] );
        loadFromDBFile( "FairyDef.bin",	        sizeof(tagFairyDef),	    	m_AllResData[DBRES_FAIRYDEF] );
		loadFromDBFile( "RideDef.bin",	        sizeof(tagRideDef),	    		m_AllResData[DBRES_RIDEDEF] );
		loadFromDBFile( "PworldDef.bin",		sizeof(tagPworldDef),			m_AllResData[DBRES_PWORLDDEF] );
		loadFromDBFile( "PetDef.bin",	        sizeof(tagPetDef),				m_AllResData[DBRES_PETDEF] );
		loadFromDBFile("PetSkillGridDef.bin",	sizeof(tagPetSkillGridDef),		m_AllResData[DBRES_PETSKILLGRIDDEF] );
		loadFromDBFile( "AddAttrBaseDef.bin",	sizeof(tagAddAttrBaseDef),		m_AllResData[DBRES_ADDATTRBASEDEF] );
		loadFromDBFile( "PetStrengthenDef.bin",	sizeof(tagPetStrengthenDef),	m_AllResData[DBRES_PETSTRENGTHENDEF] );
		loadFromDBFile( "PetLevelDef.bin",		sizeof(tagPetLevelDef),			m_AllResData[DBRES_PETLEVELDEF] );
		loadFromDBFile( "PetTraitDef.bin",		sizeof(tagPetTraitDef),			m_AllResData[DBRES_PETTRAITDEF] );
        loadFromDBFile( "OneDayGift.bin",	    sizeof(tagOneDayGift),		    m_AllResData[DBRES_ONEDAYGIFTDEF] );
        loadFromDBFile( "LVGift.bin",	        sizeof(tagLVGift),		        m_AllResData[DBRES_LVGIFTDEF] );
        loadFromDBFile( "FixedAddAttr.bin",	    sizeof(tagFixedAddAttr),		m_AllResData[DBRES_FIXEDADDATTR] );
		loadFromDBFile( "BCTDef.bin",			sizeof(tagBCTDef),				m_AllResData[DBRES_BCT] );
		loadFromDBFile( "RndAwdDef.bin",		sizeof(tagRndAwdDef),			m_AllResData[DBRES_RNDAWD] );
		loadFromDBFile( "ActiveHelper.bin",		sizeof(tagActiveHelper),		m_AllResData[DBRES_ACTIVEHELPER] );
		loadFromDBFile( "SecondarySkillDef.bin",sizeof(tagSecondarySkillDef),	m_AllResData[DBRES_SECONDARYSKILL]);
		loadFromDBFile( "RecipeDef.bin",		sizeof(tagRecipeDef),			m_AllResData[DBRES_RECIPE]);
		loadFromDBFile( "CangBaoTuDef.bin",		sizeof(tagCangBaoTuDef),		m_AllResData[DBRES_CANGBAOTU]);
		loadFromDBFile( "ClanTechDef.bin",		sizeof(tagClanTechDef),			m_AllResData[DBRES_CLANTECHDEF]);
		loadFromDBFile( "ClanProductDef.bin",	sizeof(tagClanProductDef),		m_AllResData[DBRES_CLANPRODUCTDEF]);
		loadFromDBFile( "MonMachineDef.bin",	sizeof(tagMonMachineDef),		m_AllResData[DBRES_MONMACHINEDEF]);
		loadFromDBFile( "ContOnlineGiftDef.bin",sizeof(tagContOnlineGiftDef),	m_AllResData[DBRES_CONTONLINEGIFTDEF]);
		loadFromDBFile( "BattleDef.bin",		sizeof(tagBattleDef),			m_AllResData[DBRES_BATTLEDEF]);
		loadFromDBFile( "BirthItemDef.bin",		sizeof(tagBirthItemDef),		m_AllResData[DBRES_BIRTHITEMDEF]);
		loadFromDBFile( "ClanBuildingDisplayInfo.bin",sizeof(tagClanBuildingDisplayInfo),m_AllResData[DBRES_CLAN_BUILDINGDISPLAYINFO]);
		loadFromDBFile( "FairyStarStuffDef.bin",sizeof(tagFairyStarStuffDef),	m_AllResData[DBRES_FAIRYSTARSTUFFDEF]);
		loadFromDBFile( "FairyStarDef.bin",		sizeof(tagFairyStarDef),		m_AllResData[DBRES_FAIRYSTARDEF]);
		loadFromDBFile( "OnlineGiftGroup.bin",	sizeof(tagOnlineGiftGroup),		m_AllResData[DBRES_ONLINEGIFTGROUP]);
		loadFromDBFile( "ArmRandDef.bin",		sizeof(tagArmRandDef),			m_AllResData[DBRES_ARMRANDDEF]);
		loadFromDBFile( "GameEventDef.bin",		sizeof(tagGameEventDef),		m_AllResData[DBRES_GAMEEVENTDEF]);
		loadFromDBFile( "DiffPworldDef.bin",	sizeof(tagDiffPworldDef),		m_AllResData[DBRES_DIFFPWORLDDEF]);
		loadFromDBFile( "WorldCityBuildDef.bin",sizeof(tagWorldCityBuildDef),	m_AllResData[DBRES_WORLDCITYBUILDDEF]);
		loadFromDBFile( "CityProductDef.bin",	sizeof(tagCityProductDef),		m_AllResData[DBRES_CITYPRODUCTDEF]);
		loadFromDBFile( "ArmFenJieDef.bin",		sizeof(tagArmFenJieDef),		m_AllResData[DBRES_ARMFENJIDEF]);
// 		loadFromDBFile( "StarVipDef.bin",		sizeof(tagStarVipDef),			m_AllResData[DBRES_STARVIPDEF]);
		loadFromDBFile( "StarStoneDef.bin",		sizeof(tagStarStoneDef),		m_AllResData[DBRES_STARSTONEDEF]);
		loadFromDBFile( "StarFuncDef.bin",		sizeof(tagStarFuncDef),			m_AllResData[DBRES_STARFUNCDEF]);
		loadFromDBFile( "MediaGiftDef.bin",		sizeof(tagMediaGiftDef),		m_AllResData[DBRES_MEDIAGIFTDEF]);
		loadFromDBFile( "LtrGiftDef.bin",		sizeof(tagLtrGiftDef),			m_AllResData[DBRES_LTRGIFTDEF]);
		loadFromDBFile( "GemNewDef.bin",		sizeof(tagGemNewDef),			m_AllResData[DBRES_GEMNEWDEF]);
		loadFromDBFile( "GemLevelStarDef.bin",	sizeof(tagGemLevelStarDef),		m_AllResData[DBEES_GEMLEVELSTARDEF]);
		loadFromDBFile( "GemAttrStarDef.bin",	sizeof(tagGemAttrStarDef),		m_AllResData[DBRES_GEMATTRSTARDEF]);
		loadFromDBFile( "RuneAddAttrDef.bin",	sizeof(tagRuneAddAttrDef),		m_AllResData[DBRES_RUNEADDATTRDEF]);
		loadFromDBFile( "UseDef.bin",			sizeof(tagUseDef),				m_AllResData[DBRES_USEDEF]);
		loadFromDBFile( "ArmRecastDef.bin",		sizeof(tagArmRecastDef),		m_AllResData[DBRES_ARMRECASTDEF]);
		loadFromDBFile( "ArmStarDef.bin",		sizeof(tagArmStarDef),			m_AllResData[DBRES_ARMSTARDEF]);
		loadFromDBFile( "ArmHideAttrDef.bin",	sizeof(tagArmHideAttrDef),		m_AllResData[DBRES_ARMHIDEATTRDEF]);
		loadFromDBFile( "DailyGoalAward.bin",	sizeof(tagDailyGoalAward),		m_AllResData[DBRES_DAILYGOALAWARD]);
		loadFromDBFile( "UnrealDef.bin",		sizeof(tagUnrealDef),			m_AllResData[DBRES_UNREALDEF]);
		loadFromDBFile( "NewAddAttrDef.bin",	sizeof(tagNewAddAttrDef),		m_AllResData[DBRES_NEWADDATTRDEF]);
		loadFromDBFile( "AddAttrRandDef.bin",	sizeof(tagAddAttrRandDef),		m_AllResData[DBRES_ADDATTRRANDDEF]);
		loadFromDBFile( "PetWashGeniusDef.bin",	sizeof(tagPetWashGeniusDef),	m_AllResData[DBRES_PETWASHGENIUSDEF]);
		loadFromDBFile( "SparDef.bin",			sizeof(tagSparDef),				m_AllResData[DBRES_SPARDEF]);
		loadFromDBFile( "AchievementCategoryDef.bin", sizeof(tagAchievementCategoryDef),m_AllResData[DBRES_ACHIEVEMENTCATEGORYDEF]);
		loadFromDBFile( "AchievementDef.bin",	sizeof(tagAchievementDef),		m_AllResData[DBRES_ACHIEVEMENTDEF]);
		loadFromDBFile( "ItemResultDesc.bin",	sizeof(tagItemResultDesc),		m_AllResData[DBRES_ITEMRESULTDESC] );
		loadFromDBFile( "MapTransDef.bin",		sizeof(tagMapTransDef),			m_AllResData[DBRES_MAPTRANSDEF]);
		loadFromDBFile( "SJTaskDef.bin",		sizeof(tagSJTaskDef),			m_AllResData[DBRES_SJTASKDEF]);
		loadFromDBFile( "ClanSpyAwardDropDef.bin",	sizeof(tagClanSpyAwardDropDef),m_AllResData[DBRES_CLANSPYAWARDDROPDEF]);
		loadFromDBFile( "ClanSpyTransDef.bin",	sizeof(tagClanSpyTransDef),		m_AllResData[DBRES_CLANSPYTRANSDEF]);
		loadFromDBFile( "SubJectDef.bin",		sizeof(tagSubJectDef),			m_AllResData[DBRES_SUBJECTDEF]);
		loadFromDBFile( "StrongPointDef.bin",	sizeof(tagStrongPointDef),		m_AllResData[DBRES_STRONGPOINTDEF]);
		loadFromDBFile( "ClanSkillDef.bin",			sizeof(tagClanSkillDef),		m_AllResData[DBRES_CLANSKILLDEF]);
		loadFromDBFile( "BannerDef.bin",		sizeof(tagBannerDef),			m_AllResData[DBRES_BANNERDEF]);
		loadFromDBFile( "ReviveDef.bin",		sizeof(tagReviveDef),			m_AllResData[DBRES_REVIVEDEF]);
		loadFromDBFile( "ClanPayDef.bin",		sizeof(tagClanPayDef),			m_AllResData[DBRES_CLANPAYDEF]);
		loadFromDBFile( "LevelExpDef.bin",		sizeof(tagLevelExpDef),			m_AllResData[DBRES_LEVELEXPDEF]);
		loadFromDBFile( "LevelBuffExpDef.bin",	sizeof(tagLevelBuffExpDef),		m_AllResData[DBRES_LEVELBUFFEXPDEF]);
		loadFromDBFile( "OfflineExpParameter.bin",	sizeof(tagOfflineExpParameter),m_AllResData[DBRES_OFFLINEEXPPARAMETER]);
		loadFromDBFile( "RidePinZhiDef.bin",	sizeof(tagRidePinZhiDef),		m_AllResData[DBRES_RIDEPINZHIDEF]);
		loadFromDBFile( "MapAwardOreDef.bin",	sizeof(tagMapAwardOreDef),		m_AllResData[DBRES_MAPAWARDOREDEF] );
		loadFromDBFile( "RongluDef.bin",		sizeof(tagRongluDef),			m_AllResData[DBRES_RONGLUDEF]);
		loadFromDBFile( "GeniusAttrDef.bin",	sizeof(tagGeniusAttrDef),		m_AllResData[DBRES_GENIUSATTRDEF] );
		loadFromDBFile( "GeniusCareerDef.bin",	sizeof(tagGeniusCareerDef),		m_AllResData[DBRES_GENIUSCAREERDEF]);
		loadFromDBFile(	"GeniusAddDef.bin",		sizeof(tagGeniusAddDef),		m_AllResData[DBRES_GENIUSADDDEF]);
		loadFromDBFile( "BaoXiangDef.bin",		sizeof(tagBaoXiangDef),			m_AllResData[DBRES_BAOXIANGDEF]);
		loadFromDBFile( "ArmLevelUPDef.bin",	sizeof(tagArmLevelUPDef),		m_AllResData[DBRES_ARMLEVELUPDEF] );
		loadFromDBFile( "WorldBossDef.bin",		sizeof(tagWorldBossDef),		m_AllResData[DBRES_WORLDBOSSDEF]);
		loadFromDBFile( "LevelFeng.bin",		sizeof(tagLevelFeng),			m_AllResData[DBRES_LEVELFENG] );
		loadFromDBFile( "WorldCityFightAwardDef.bin",	sizeof(tagWorldCityFightAwardDef),	m_AllResData[DBRES_WORLDCITYFIGHTAWARDDEF]);
		loadFromDBFile( "BusyDef.bin",			sizeof(tagBusyDef),				m_AllResData[DBRES_BUSYDEF]);
		loadFromDBFile( "BusyUseDef.bin",		sizeof(tagBusyUseDef),			m_AllResData[DBRES_BUSYUSEDEF]);
		loadFromDBFile( "HighWashUseDef.bin",	sizeof(tagHighWashUseDef),		m_AllResData[DBRES_HIGHWASHUSEDEF]);
		loadFromDBFile( "MapOreDef.bin",		sizeof(tagMapOreDef),			m_AllResData[DBRES_MAPOREDEF]);
		loadFromDBFile( "PetGeniusDef.bin",		sizeof(tagPetGeniusDef),		m_AllResData[DBRES_PETGENIUSDEF]);
		loadFromDBFile( "SpartaLevelDef.bin",	sizeof(tagSpartaLevelDef),		m_AllResData[DBRES_SPARTALEVELDEF]);
		loadFromDBFile( "BackLevelDef.bin",		sizeof(tagBackLevelDef),		m_AllResData[DBRES_BACKLEVELDEF]);
		loadFromDBFile( "SchoolLevelDef.bin",	sizeof(tagSchoolLevelDef),		m_AllResData[DBRES_SCHOOLLEVELDEF]);
		loadFromDBFile( "SchoolXinDeDef.bin",	sizeof(tagSchoolXinDeDef),		m_AllResData[DBRES_SCHOOLXINDEDEF]);
		loadFromDBFile( "XuKongPosDef.bin",		sizeof(tagXuKongPosDef),		m_AllResData[DBRES_XUKONGPOSDEF]);
		loadFromDBFile( "ContCoverGift.bin",	sizeof(tagContCoverGift),		m_AllResData[DBRES_CONCOVERGIFTDEF]);
		loadFromDBFile( "NewRoleGift.bin",		sizeof(tagNewRoleGift),			m_AllResData[DBRES_NEWROLEGIFTDEF]);
		loadFromDBFile( "CardBoxDef.bin",		sizeof(tagCardBoxDef),			m_AllResData[DBRES_CARDBOXDEF]);
		loadFromDBFile( "CardBoxGroupDef.bin",	sizeof(tagCardBoxGroupDef),		m_AllResData[DBRES_CARDBOXGROUPDEF]);
		loadFromDBFile( "ArmBuChangDef.bin",	sizeof(tagArmBuChangDef),		m_AllResData[DBRES_ARMBUCHANGDEF]);
		loadFromDBFile( "BuChangDef.bin",		sizeof(tagBuChangDef),			m_AllResData[DBRES_BUCHANGDEF]);
		loadFromDBFile( "StoreGridDef.bin",		sizeof(tagStoreGridDef),		m_AllResData[DBRES_STOREGRIDDEF]);
		loadFromDBFile( "ArmStepUpDef.bin",		sizeof(tagArmStepUpDef),		m_AllResData[DBRES_ARMSTEPUPDEF]);
		loadFromDBFile( "RongYanJuShouDef.bin",	sizeof(tagRongYanJuShouDef),	m_AllResData[DBRES_RONGYANJUSHOUDEF]);
		loadFromDBFile( "RongYanMonDynAttrDef.bin",		sizeof(tagRongYanMonDynAttrDef),		m_AllResData[DBRES_RONGYANMONATTRDEF]);
		loadFromDBFile( "ArmGradeValDef.bin",	sizeof(tagArmGradeValDef),		m_AllResData[DBRES_ARMGRADEVALDEF]);
		loadFromDBFile( "ArmReSetDef.bin",		sizeof(tagArmReSetDef),			m_AllResData[DBRES_ARMRESETDEF]);
		loadFromDBFile( "GeniusMdfDef.bin",		sizeof(tagGeniusMdfDef),		m_AllResData[DBRES_GENIUSMDFDEF]);
		loadFromDBFile( "ArmSkillNumDef.bin",	sizeof(tagArmSkillNumDef),		m_AllResData[DBRES_ARMSKILLNUMDEF]);
		loadFromDBFile( "ArmSkillAttrDef.bin",	sizeof(tagArmSkillAttrDef),		m_AllResData[DBRES_ARMSKILLATTRDEF]);
		loadFromDBFile( "PetMoneyDef.bin",		sizeof(tagPetMoneyDef),			m_AllResData[DBRES_PETMONEYDEF]);
		loadFromDBFile( "PetImpUpDef.bin",			sizeof(tagPetImpUpDef),			m_AllResData[DBRES_PETIMPUPDEF]);
		loadFromDBFile( "PetImpUpAttrDef.bin",		sizeof(tagPetImpUpAttrDef),		m_AllResData[DBRES_PETIMPUPATTRDEF]);
		loadFromDBFile( "GemPagDef.bin",		sizeof(tagGemPagDef),			m_AllResData[DBRES_GEMPAGDEF]);
		loadFromDBFile( "ArmTeshuDef.bin",		sizeof(tagArmTeshuDef),			m_AllResData[DBRES_ARMTESHUDEF]);
		loadFromDBFile( "SpanStrongDef.bin",		sizeof(tagSpanStrongDef),		m_AllResData[DBRES_SPANSTRONGDEF]);
		loadFromDBFile( "HomeLvlDef.bin",		sizeof(tagHomeLvlDef),					m_AllResData[DBRES_HOMELVLDEF] );
		loadFromDBFile( "HomeMonLvlDef.bin",		sizeof(tagHomeMonLvlDef),					m_AllResData[DBRES_HOMEMONLVLDEF] );
		loadFromDBFile( "HomeFarmDef.bin",	sizeof(tagHomeFarmDef),				m_AllResData[DBRES_HOMEFARMDEF]);
		loadFromDBFile( "HomeMonSegDef.bin",	sizeof(tagHomeMonSegDef),				m_AllResData[DBRES_HOMEMONSEGDEF]);
		loadFromDBFile( "ArmSkillRecastDef.bin",		sizeof(tagArmSkillRecastDef),	m_AllResData[DBRES_ARMSKILLRECASTDEF]);
	}

	LOG_INFO("load db data succeeded");
	return true;
}

void DBDataManager::reloadDB()
{
	loadFromRealDB( "SkillDef",			sizeof(tagSkillDef),			m_AllResData[DBRES_SKILLDEF] );
	loadFromRealDB( "MonsterDef",		sizeof(tagMonsterDef),			m_AllResData[DBRES_MONSTERDEF] );
	loadFromRealDB( "NpcDisplayInfo",	sizeof(tagNpcDisplayInfo),		m_AllResData[DBRES_NPCDISPLAYINFO] );
	loadFromRealDB( "MapDef",			sizeof(tagMapDef),				m_AllResData[DBRES_MAPDEF] );
	loadFromRealDB( "StatusDef",		sizeof(tagStatusDef),			m_AllResData[DBRES_STATUSDEF] );
	loadFromRealDB("StatusOverlayDef",	sizeof(tagStatusOverlayDef),	m_AllResData[DBRES_STATUSOVERLAYDEF]);
	loadFromRealDB( "ItemDef",			sizeof(tagItemDef),				m_AllResData[DBRES_ITEMDEF] );
	loadFromRealDB( "ItemDisplayInfo",	sizeof(tagItemDisplayInfo),		m_AllResData[DBRES_ITEMDISPLAYINFO] );
	loadFromRealDB( "LevelUP",			sizeof(tagLevelUP),				m_AllResData[DBRES_LEVELUP] );
	loadFromRealDB( "MapNpc",			sizeof(tagMapNpc),				m_AllResData[DBRES_MAPNPC] );
	loadFromRealDB( "ArmLevelHurtDef",	sizeof(ARMLEVELHURTDEF),		m_AllResData[DBRES_ARMLEVELHURTDEF] );
	loadFromRealDB( "WorldMapSearchDef",sizeof(tagWorldMapSearchDef),	m_AllResData[DBRES_WORLDMAPSEARCHDEF] );
	loadFromRealDB("ClanLevelDef",		sizeof(tagClanLevelDef),		m_AllResData[DBRES_CLANLEVELDEF] );
	loadFromRealDB("ClanCityLevelDef",	sizeof(tagClanCityLevelDef),	m_AllResData[DBRES_CLANCITYLEVELDEF] );
	loadFromRealDB("ClanCityBuildingLevelDef",sizeof(tagClanCityBuildingLevelDef),
		m_AllResData[DBRES_CLANCITYBUILDINGLEVELDEF] );
    loadFromRealDB("DesignationDef",	sizeof(tagDesignationDef),		m_AllResData[DBRES_DESIGNATIONDEF] );
    loadFromRealDB("RespactPartyDef",	sizeof(tagRespactPartyDef),		m_AllResData[DBRES_RESPACTPARTYDEF] );
    loadFromRealDB("RespactPhaseDef",	sizeof(tagRespactPhaseDef),		m_AllResData[DBRES_RESPACTPHASEDEF] );
    loadFromRealDB("FairyDef",	        sizeof(tagFairyDef),		    m_AllResData[DBRES_FAIRYDEF] );
	loadFromRealDB("RideDef",			sizeof(tagRideDef),				m_AllResData[DBRES_RIDEDEF] );
	loadFromRealDB("PworldDef",			sizeof(tagPworldDef),			m_AllResData[DBRES_PWORLDDEF] );
	loadFromRealDB("PetDef",	        sizeof(tagPetDef),				m_AllResData[DBRES_PETDEF] );
	loadFromRealDB("PetSkillGridDef",	sizeof(tagPetSkillGridDef),		m_AllResData[DBRES_PETSKILLGRIDDEF] );
	loadFromRealDB("AddAttrBaseDef",	sizeof(tagAddAttrBaseDef),		m_AllResData[DBRES_ADDATTRBASEDEF] );
	loadFromRealDB( "PetStrengthenDef",	sizeof(tagPetStrengthenDef),	m_AllResData[DBRES_PETSTRENGTHENDEF] );
	loadFromRealDB( "PetLevelDef",		sizeof(tagPetLevelDef),			m_AllResData[DBRES_PETLEVELDEF] );
	loadFromRealDB( "PetTraitDef",		sizeof(tagPetTraitDef),			m_AllResData[DBRES_PETTRAITDEF] );
    loadFromRealDB( "OneDayGift",	    sizeof(tagOneDayGift),		    m_AllResData[DBRES_ONEDAYGIFTDEF] );
    loadFromRealDB( "LVGift",	        sizeof(tagLVGift),		        m_AllResData[DBRES_LVGIFTDEF] );
    loadFromRealDB( "FixedAddAttr",	    sizeof(tagFixedAddAttr),		m_AllResData[DBRES_FIXEDADDATTR] );
	loadFromRealDB( "BCTDef",			sizeof(tagBCTDef),				m_AllResData[DBRES_BCT] );
	loadFromRealDB( "RndAwdDef",		sizeof(tagRndAwdDef),			m_AllResData[DBRES_RNDAWD] );
	loadFromRealDB( "ActiverHelper",	sizeof(tagActiveHelper),		m_AllResData[DBRES_ACTIVEHELPER] );
	loadFromRealDB( "SecondarySkillDef",sizeof(tagSecondarySkillDef),	m_AllResData[DBRES_SECONDARYSKILL]);
	loadFromRealDB( "RecipeDef",		sizeof(tagRecipeDef),			m_AllResData[DBRES_RECIPE]);
	loadFromRealDB( "CangBaoTuDef",		sizeof(tagCangBaoTuDef),		m_AllResData[DBRES_CANGBAOTU]);
	loadFromRealDB( "ClanTechDef",		sizeof(tagClanTechDef),			m_AllResData[DBRES_CLANTECHDEF]);
	loadFromRealDB( "ClanProductDef",	sizeof(tagClanProductDef),		m_AllResData[DBRES_CLANPRODUCTDEF]);
	loadFromRealDB( "MonMachineDef",	sizeof(tagMonMachineDef),		m_AllResData[DBRES_MONMACHINEDEF]);
	loadFromRealDB( "ContOnlineGiftDef",sizeof(tagContOnlineGiftDef),	m_AllResData[DBRES_CONTONLINEGIFTDEF]);
	loadFromRealDB( "BattleDef",		sizeof(tagBattleDef),			m_AllResData[DBRES_BATTLEDEF]);
	loadFromRealDB( "BirthItemDef",		sizeof(tagBirthItemDef),		m_AllResData[DBRES_BIRTHITEMDEF]);
	loadFromRealDB( "ClanBuildingDisplayInfo",sizeof(tagClanBuildingDisplayInfo), m_AllResData[DBRES_CLAN_BUILDINGDISPLAYINFO]);
	loadFromRealDB( "FairyStarStuffDef",sizeof(tagFairyStarStuffDef),	m_AllResData[DBRES_FAIRYSTARSTUFFDEF]);
	loadFromRealDB( "FairyStarDef",		sizeof(tagFairyStarDef),		m_AllResData[DBRES_FAIRYSTARDEF]);
	loadFromRealDB( "OnlineGiftGroup",	sizeof(tagOnlineAwardDef),		m_AllResData[DBRES_ONLINEGIFTGROUP]);
	loadFromRealDB( "ArmRandDef",		sizeof(tagArmRandDef),			m_AllResData[DBRES_ARMRANDDEF]);
	loadFromRealDB( "GameEventDef",		sizeof(tagGameEventDef),		m_AllResData[DBRES_GAMEEVENTDEF]);
	loadFromRealDB( "DiffPworldDef",	sizeof(tagDiffPworldDef),		m_AllResData[DBRES_DIFFPWORLDDEF]);
	loadFromRealDB( "WorldCityBuildDef",sizeof(tagWorldCityBuildDef),	m_AllResData[DBRES_WORLDCITYBUILDDEF]);
	loadFromRealDB( "CityProductDef",	sizeof(tagCityProductDef),		m_AllResData[DBRES_CITYPRODUCTDEF]);
	loadFromRealDB( "ArmFenJieDef",		sizeof(tagArmFenJieDef),		m_AllResData[DBRES_ARMFENJIDEF]);
// 	loadFromRealDB( "StarVipDef",		sizeof(tagStarVipDef),			m_AllResData[DBRES_STARVIPDEF]);
	loadFromRealDB( "StarStoneDef",		sizeof(tagStarStoneDef),		m_AllResData[DBRES_STARSTONEDEF]);
	loadFromRealDB( "StarFuncDef",		sizeof(tagStarFuncDef),			m_AllResData[DBRES_STARFUNCDEF]);
	loadFromRealDB( "MediaGiftDef",		sizeof(tagMediaGiftDef),		m_AllResData[DBRES_MEDIAGIFTDEF]);
	loadFromRealDB( "LtrGiftDef",		sizeof(tagLtrGiftDef),			m_AllResData[DBRES_LTRGIFTDEF]);
	loadFromRealDB( "GemNewDef",		sizeof(tagGemNewDef),			m_AllResData[DBRES_GEMNEWDEF]);
	loadFromRealDB( "GemLevelStarDef",	sizeof(tagGemLevelStarDef),		m_AllResData[DBEES_GEMLEVELSTARDEF]);
	loadFromRealDB( "GemAttrStarDef",	sizeof(tagGemAttrStarDef),		m_AllResData[DBRES_GEMATTRSTARDEF]);
	loadFromRealDB( "RuneAddAttrDef",	sizeof(tagRuneAddAttrDef),		m_AllResData[DBRES_RUNEADDATTRDEF]);
	loadFromRealDB( "UseDef",			sizeof(tagUseDef),				m_AllResData[DBRES_USEDEF]);
	loadFromRealDB( "ArmRecastDef",		sizeof(tagArmRecastDef),		m_AllResData[DBRES_ARMRECASTDEF]);
	loadFromRealDB( "ArmStarDef",		sizeof(tagArmStarDef),			m_AllResData[DBRES_ARMSTARDEF]);
	loadFromRealDB( "ArmHideAttrDef",	sizeof(tagArmHideAttrDef),		m_AllResData[DBRES_ARMHIDEATTRDEF]);
	loadFromRealDB( "DailyGoalAward",	sizeof(tagDailyGoalAward),		m_AllResData[DBRES_DAILYGOALAWARD]);
	loadFromRealDB( "UnrealDef",		sizeof(tagUnrealDef),			m_AllResData[DBRES_UNREALDEF]);
	loadFromRealDB( "NewAddAttrDef",	sizeof(tagNewAddAttrDef),		m_AllResData[DBRES_NEWADDATTRDEF]);
	loadFromRealDB( "AddAttrRandDef",	sizeof(tagAddAttrRandDef),		m_AllResData[DBRES_ADDATTRRANDDEF]);
	loadFromRealDB( "PetWashGeniusDef",	sizeof(tagPetWashGeniusDef),	m_AllResData[DBRES_PETWASHGENIUSDEF]);
	loadFromRealDB( "SparDef",			sizeof(tagSparDef),				m_AllResData[DBRES_SPARDEF]);
	loadFromRealDB( "AchievementCategoryDef", sizeof(tagAchievementCategoryDef), m_AllResData[DBRES_ACHIEVEMENTCATEGORYDEF]);
	loadFromRealDB( "AchievementDef",	sizeof(tagAchievementDef),		m_AllResData[DBRES_ACHIEVEMENTDEF]);
	loadFromRealDB( "ItemResultDesc",	sizeof(tagItemResultDesc),		m_AllResData[DBRES_ITEMRESULTDESC] );
	loadFromRealDB( "MapTransDef",		sizeof(tagMapTransDef),			m_AllResData[DBRES_MAPTRANSDEF]);
	loadFromRealDB( "SJTaskDef",		sizeof(tagSJTaskDef),			m_AllResData[DBRES_SJTASKDEF]);
	loadFromRealDB( "ClanSpyAwardDropDef",	sizeof(tagClanSpyAwardDropDef),m_AllResData[DBRES_CLANSPYAWARDDROPDEF]);
	loadFromRealDB( "ClanSpyTransDef",	sizeof(tagClanSpyTransDef),		m_AllResData[DBRES_CLANSPYTRANSDEF]);
	loadFromRealDB( "SubJectDef",		sizeof(tagSubJectDef),			m_AllResData[DBRES_SUBJECTDEF]);
	loadFromRealDB( "StrongPointDef",	sizeof(tagStrongPointDef),		m_AllResData[DBRES_STRONGPOINTDEF]);
	loadFromRealDB( "ClanSkillDef",		sizeof(tagClanSkillDef),		m_AllResData[DBRES_CLANSKILLDEF]);
	loadFromRealDB( "BannerDef",		sizeof(tagBannerDef),			m_AllResData[DBRES_BANNERDEF]);
	loadFromRealDB( "ReviveDef",		sizeof(tagReviveDef),			m_AllResData[DBRES_REVIVEDEF]);
	loadFromRealDB( "ClanPayDef",		sizeof(tagClanPayDef),			m_AllResData[DBRES_CLANPAYDEF]);
	loadFromRealDB( "LevelExpDef",		sizeof(tagLevelExpDef),			m_AllResData[DBRES_LEVELEXPDEF]);
	loadFromRealDB( "LevelBuffExpDef",	sizeof(tagLevelBuffExpDef),		m_AllResData[DBRES_LEVELBUFFEXPDEF]);
	loadFromRealDB( "OfflineExpParameter",	sizeof(tagOfflineExpParameter),m_AllResData[DBRES_OFFLINEEXPPARAMETER]);
	loadFromRealDB( "RidePinZhiDef",	sizeof(tagRidePinZhiDef),		m_AllResData[DBRES_RIDEPINZHIDEF]);
	loadFromRealDB( "MapAwardOreDef",	sizeof(tagMapAwardOreDef),		m_AllResData[DBRES_MAPAWARDOREDEF] );
	loadFromRealDB( "RongluDef",		sizeof(tagRongluDef),			m_AllResData[DBRES_RONGLUDEF] );
	loadFromRealDB( "GeniusAttrDef",	sizeof(tagGeniusAttrDef),		m_AllResData[DBRES_GENIUSATTRDEF] );
	loadFromRealDB( "GeniusCareerDef",	sizeof(tagGeniusCareerDef),		m_AllResData[DBRES_GENIUSCAREERDEF]);
	loadFromRealDB( "GeniusAddDef",		sizeof(tagGeniusAddDef),		m_AllResData[DBRES_GENIUSADDDEF]);
	loadFromRealDB( "BaoXiangDef",		sizeof(tagBaoXiangDef),			m_AllResData[DBRES_BAOXIANGDEF]);
	loadFromRealDB( "WorldBossDef",		sizeof(tagWorldBossDef),		m_AllResData[DBRES_WORLDBOSSDEF]);
	loadFromRealDB( "ArmLevelUPDef",	sizeof(tagArmLevelUPDef),		m_AllResData[DBRES_ARMLEVELUPDEF] );
	loadFromRealDB( "LevelFeng",		sizeof(tagLevelFeng),			m_AllResData[DBRES_LEVELFENG] );
	loadFromRealDB( "WorldCityFightAwardDef",	sizeof(tagWorldCityFightAwardDef),	m_AllResData[DBRES_WORLDCITYFIGHTAWARDDEF]);
	loadFromRealDB( "BusyDef",			sizeof(tagBusyDef),				m_AllResData[DBRES_BUSYDEF]);
	loadFromRealDB( "BusyUseDef",		sizeof(tagBusyUseDef),			m_AllResData[DBRES_BUSYUSEDEF]);
	loadFromRealDB( "HighWashUseDef",	sizeof(tagHighWashUseDef),		m_AllResData[DBRES_HIGHWASHUSEDEF]);
	loadFromRealDB( "MapOreDef",		sizeof(tagMapOreDef),			m_AllResData[DBRES_MAPOREDEF]);
	loadFromRealDB( "PetGeniusDef",		sizeof(tagPetGeniusDef),		m_AllResData[DBRES_PETGENIUSDEF]);
	loadFromRealDB( "SpartaLevelDef",	sizeof(tagSpartaLevelDef),		m_AllResData[DBRES_SPARTALEVELDEF]);
	loadFromRealDB( "BackLevelDef",		sizeof(tagBackLevelDef),		m_AllResData[DBRES_BACKLEVELDEF]);
	loadFromRealDB( "SchoolLevelDef",	sizeof(tagSchoolLevelDef),		m_AllResData[DBRES_SCHOOLLEVELDEF]);
	loadFromRealDB( "SchoolXinDeDef",	sizeof(tagSchoolXinDeDef),		m_AllResData[DBRES_SCHOOLXINDEDEF]);
	loadFromRealDB( "XuKongPosDef",		sizeof(tagXuKongPosDef),		m_AllResData[DBRES_XUKONGPOSDEF]);
	loadFromRealDB( "ContCoverGift",	sizeof(tagContCoverGift),		m_AllResData[DBRES_CONCOVERGIFTDEF]);
	loadFromRealDB( "NewRoleGift",		sizeof(tagNewRoleGift),			m_AllResData[DBRES_NEWROLEGIFTDEF]);
	loadFromRealDB( "CardBoxDef",		sizeof(tagCardBoxDef),			m_AllResData[DBRES_CARDBOXDEF]);
	loadFromRealDB( "CardBoxGroupDef",	sizeof(tagCardBoxGroupDef),		m_AllResData[DBRES_CARDBOXGROUPDEF]);
	loadFromRealDB( "ArmBuChangDef",	sizeof(tagArmBuChangDef),		m_AllResData[DBRES_ARMBUCHANGDEF]);
	loadFromRealDB( "BuChangDef",		sizeof(tagBuChangDef),			m_AllResData[DBRES_BUCHANGDEF]);
	loadFromRealDB( "StoreGridDef",		sizeof(tagStoreGridDef),		m_AllResData[DBRES_STOREGRIDDEF]);
	loadFromRealDB( "ArmStepUpDef",		sizeof(tagArmStepUpDef),		m_AllResData[DBRES_ARMSTEPUPDEF]);
	loadFromRealDB( "RongYanJuShouDef",	sizeof(tagRongYanJuShouDef),	m_AllResData[DBRES_RONGYANJUSHOUDEF]);
	loadFromRealDB( "RongYanMonDynAttrDef",		sizeof(tagRongYanMonDynAttrDef),		m_AllResData[DBRES_RONGYANMONATTRDEF]);
	loadFromRealDB( "ArmGradeValDef",	sizeof(tagArmGradeValDef),		m_AllResData[DBRES_ARMGRADEVALDEF]);
	loadFromRealDB( "ArmReSetDef",		sizeof(tagArmReSetDef),			m_AllResData[DBRES_ARMRESETDEF]);
	loadFromRealDB( "GeniusMdfDef",		sizeof(tagGeniusMdfDef),		m_AllResData[DBRES_GENIUSMDFDEF]);
	loadFromRealDB( "ArmSkillNumDef",	sizeof(tagArmSkillNumDef),		m_AllResData[DBRES_ARMSKILLNUMDEF]);
	loadFromRealDB( "ArmSkillAttrDef",	sizeof(tagArmSkillAttrDef),		m_AllResData[DBRES_ARMSKILLATTRDEF]);
	loadFromRealDB( "PetMoneyDef",		sizeof(tagPetMoneyDef),			m_AllResData[DBRES_PETMONEYDEF]);
	loadFromRealDB( "PetImpUpDef",		sizeof(tagPetImpUpDef),			m_AllResData[DBRES_PETIMPUPDEF]);
	loadFromRealDB( "PetImpUpAttrDef",	sizeof(tagPetImpUpAttrDef),		m_AllResData[DBRES_PETIMPUPATTRDEF]);
	loadFromRealDB( "GemPagDef",		sizeof(tagGemPagDef),			m_AllResData[DBRES_GEMPAGDEF]);
	loadFromRealDB( "ArmTeshuDef",		sizeof(tagArmTeshuDef),			m_AllResData[DBRES_ARMTESHUDEF]);
	loadFromRealDB( "SpanStrongDef",	sizeof(tagSpanStrongDef),			m_AllResData[DBRES_SPANSTRONGDEF]);
	loadFromRealDB( "HomeLvlDef",		sizeof(tagHomeLvlDef),					m_AllResData[DBRES_HOMELVLDEF] );
	loadFromRealDB( "HomeMonLvlDef",		sizeof(tagHomeMonLvlDef),					m_AllResData[DBRES_HOMEMONLVLDEF] );
	loadFromRealDB( "HomeFarmDef",	sizeof(tagHomeFarmDef),				m_AllResData[DBRES_HOMEFARMDEF]);
	loadFromRealDB( "HomeMonSegDef",	sizeof(tagHomeMonSegDef),				m_AllResData[DBRES_HOMEMONSEGDEF]);
	loadFromRealDB( "ArmSkillRecastDef",	sizeof(tagArmSkillRecastDef),	m_AllResData[DBRES_ARMSKILLRECASTDEF]);
}

int confuse_comp_func(const void * v1, const void *v2)
{
	return *((int *)v1) - *((int *)v2);
}

char *get_dbdata(void *pkey, DBDataTable &dbdata)
{
	char *p = (char *)bsearch(pkey, dbdata.pbuffer, dbdata.count, dbdata.unitsize, confuse_comp_func);
	return p;
}

bool DBDataManager::loadFromDBFile(const char *filename, int unitsize, DBDataTable &data)
{
	memset(&data, 0, sizeof(DBDataTable));

	char fullpath[256];
	sprintf(fullpath, "%s\\%s", m_DBFileDir.c_str(), filename);

	/*
	if(0 != _stat(fullpath, &stStat))
	{
		LOG_SEVERE("failed to open db file: %s", filename);
		return false;
	}

	int count = 0;
	char *pbuffer = new char[stStat.st_size];
	if(load_res_confuse_bin(fullpath, pbuffer, unitsize, stStat.st_size/unitsize, &count) < 0)
	{
		LOG_SEVERE("failed to parse db file: %s", filename);
		delete[] pbuffer;
		return false;
	}*/

	DataStream *fp = ResourceManager::getSingleton().openFileStream(fullpath, true);
	if(fp == NULL)
	{
		LOG_SEVERE("failed to open db file: %s", filename);
		return false;
	}

	int count = 0;
	size_t fsize = fp->size();
	char *pbuffer = new char[fsize];
	if(load_res_confuse_bin_mem((unsigned char *)fp->getMemoryImage(), (int)fsize, pbuffer, unitsize, (int)fsize/unitsize, &count) < 0)
	{
		LOG_SEVERE("failed to parse db file: %s", filename);
		delete fp;
		delete[] pbuffer;
		return false;
	}
	delete fp;

	if(count > 1)
	{
		qsort(pbuffer, count, unitsize, confuse_comp_func);
	}

	data.count = count;
	data.pbuffer = pbuffer;
	data.unitsize = unitsize;
	return true;
}

bool DBDataManager::loadFromRealDB(const char *tablename, int unitsize, DBDataTable &data)
{
	memset(&data, 0, sizeof(DBDataTable));

	ResTable *ptable = m_pResDB->GetTable(tablename);
	if(ptable == NULL)
	{
		LOG_SEVERE("failed to open db table:%s", tablename);
		return false;
	}

	assert(unitsize == ptable->GetRecordSize());
	int count = ptable->FindRecord(NULL, 0, NULL);
	if(count < 0)
	{
		LOG_SEVERE("failed to get recordsize: %s", tablename);
		return false;
	}

	char *pbuffer = new char[count*unitsize];
	ptable->FindRecord(pbuffer, count, NULL);

	if(count > 1)
	{
		qsort(pbuffer, count, unitsize, confuse_comp_func);
	}

	data.count		= count;
	data.pbuffer	= pbuffer;
	data.unitsize = unitsize;
	return true;
}

template<typename T>
const T &DBDataManager::GetInfo(int id, stdext::hash_map<int, T>&infotable, Ogre::ResTable *prestable)
{
	stdext::hash_map<int, T>::iterator iter = infotable.find(id);
	if(iter != infotable.end())
	{
		return iter->second;
	}

	T objs[10];
	T &obj = objs[0];

	*((int *)&obj) = id;
	size_t objsize = sizeof(T);
	objsize = sizeof(tagMonsterDef);
	int num = prestable->FindRecord(&obj, 1, &obj);
	if(num == 0)
	{
		memset(&obj, 0, sizeof(obj));
		assert(0);
	}

	infotable[id] = obj;
	return infotable.find(id)->second;
}

template<typename T>
const T &DBDataManager::GetInfo(int id, const void* pObj, stdext::hash_map<int, T>&infotable, Ogre::ResTable *prestable)
{
	stdext::hash_map<int, T>::iterator iter = infotable.find(id);
	if(iter != infotable.end())
	{
		return iter->second;
	}

	T objs[10];
	T &obj = objs[0];
	int num = prestable->FindRecord(&obj, 1, pObj);
	if(num == 0)
	{
		memset(&obj, 0, sizeof(obj));
		assert(0);
	}
	infotable[id] = obj;
	return infotable.find(id)->second;
}

static tagSkillDef nonSkillDef;
const tagSkillDef &DBDataManager::GetSkillDef(int id, int lv)
{
	tagSkillDef ID;
	ID.SkillID  = id;
	ID.Level	= lv;

	char *pskilldef = get_dbdata(&ID, m_AllResData[DBRES_SKILLDEF]);
	if(pskilldef == NULL)
	{
		nonSkillDef.SkillID = 0;
		return nonSkillDef;
		//LOG_INFO( "SkillDef不存在id=%d,lv=%d 的技能", id, lv);
		/*ID.SkillID = 1010;
		ID.Level = 1;
		pskilldef = get_dbdata(&ID, m_AllResData[DBRES_SKILLDEF]);*/
	}

	return *(tagSkillDef *)pskilldef;
}

const tagNpcDisplayInfo &DBDataManager::GetNpcDisplayInfo(int id)
{
	char *pnpc = get_dbdata(&id, m_AllResData[DBRES_NPCDISPLAYINFO]);
	if(pnpc == NULL)
	{
		LOG_INFO( "NPCDISPLAYINFO不存在id为 %d 的NPC", id );

		/*id = 200000;
		pnpc = get_dbdata(&id, m_AllResData[DBRES_NPCDISPLAYINFO]);*/
	}

	assert(pnpc != NULL);
	return *(tagNpcDisplayInfo *)pnpc;
}

const tagMonsterDef &DBDataManager::getMonsterDef(int id)
{
	return *(tagMonsterDef *)getMonsterDefPtr(id);
}

const tagMonsterDef* DBDataManager::getMonsterDefPtr( int id )
{
	char *pmonster = get_dbdata(&id, m_AllResData[DBRES_MONSTERDEF]);
	if(pmonster == NULL)
	{
		LOG_INFO( "MONSTERDEF不存在id为 %d 的怪物", id );
		return NULL;
	}
	return (const tagMonsterDef *)pmonster;

}

const tagMapDef &DBDataManager::getMapDef(int id)
{
	char *pmapdef = get_dbdata(&id, m_AllResData[DBRES_MAPDEF]);
	if(pmapdef == NULL)
	{
		LOG_INFO( "MAPDEF不存在id为 %d 的地图", id );

		id = 1000;
		pmapdef = get_dbdata(&id, m_AllResData[DBRES_MAPDEF]);
	}
	assert(pmapdef != NULL);
	return *(tagMapDef *)pmapdef;
}

const tagStatusDef& DBDataManager::getStatusDef(int id, int lv)
{
	tagStatusDef ID;
	ID.StatusID	= id;
	ID.Level	= lv;

	static tagStatusDef nonStatus;
	memset( &nonStatus, 0, sizeof(tagStatusDef) );
	char *pstatus = get_dbdata(&ID, m_AllResData[DBRES_STATUSDEF]);
	if(pstatus == NULL)
	{
		LOG_INFO("StatusDef不存在id=%d, lv=%d的状态", id, lv);
		return nonStatus;
		/*ID.StatusID = 1000;
		ID.Level = 1;
		pstatus = get_dbdata(&ID, m_AllResData[DBRES_STATUSDEF]);*/
	}

	//assert(pstatus != NULL);
	return *(tagStatusDef *)pstatus;
}

const tagStatusOverlayDef&	DBDataManager::getStatusOverlayDef(int id, int lv,int overlay)
{
	tagStatusOverlayDef ID;
	ID.StatusID	= id;
	ID.Level	= lv;
	ID.Overlay	= overlay;

	char *pstatusOverlay = get_dbdata(&ID, m_AllResData[DBRES_STATUSOVERLAYDEF]);
	if(pstatusOverlay == NULL)
	{
		LOG_INFO("StatusOverlayDef不存在id=%d, lv=%d, overlay=%d的状态", id, lv, overlay);
	}

	return *(tagStatusOverlayDef *)pstatusOverlay;
	
}

const tagItemDef &DBDataManager::getItemDef( unsigned int id )
{
	char *pitem = get_dbdata(&id, m_AllResData[DBRES_ITEMDEF]);
	if(pitem == NULL)
	{
		LOG_INFO( "ITEMDEF 不存在id为 %d 的物品", id );
		//assert( 0 );
		id = 1000;
		pitem = get_dbdata(&id, m_AllResData[DBRES_ITEMDEF]);
	}

	assert(pitem != NULL);
	return *(tagItemDef *)pitem;
}

const tagArmAddAttrDef &DBDataManager::getArmAddAttrDef( unsigned int id )
{
	char *pArmAddAttr = get_dbdata(&id, m_AllResData[DBRES_ARMADDATTRDEF]);
	if( pArmAddAttr == NULL ) 
	{
		LOG_INFO( "ARMADDATTRDEF 不存在id为 %d 的关键字属性", id );
	}

	assert( pArmAddAttr != NULL);
	return *(tagArmAddAttrDef *)pArmAddAttr;
}

const tagSuitSet &DBDataManager::getSuitSet( unsigned int id )
{
	char *pSuitSet = get_dbdata(&id, m_AllResData[DBRES_SUITSET]);
	if( pSuitSet == NULL ) 
	{
		LOG_INFO( "SUITSET 不存在id为 %d 的套装", id );
	}

	assert( pSuitSet != NULL);
	return *(tagSuitSet *)pSuitSet;
}

const tagLevelUP& DBDataManager::getLevelUP( int lv )
{
	char *plevel = get_dbdata(&lv, m_AllResData[DBRES_LEVELUP]);
	if(plevel == NULL)
	{
		LOG_INFO("LevelUP 不存在lv=%d的项", lv);

		/*lv = 1;
		plevel = get_dbdata(&lv, m_AllResData[DBRES_LEVELUP]);*/
	}

	assert(plevel != NULL);
	return *(tagLevelUP *)plevel;
}

const tagItemDisplayInfo &DBDataManager::getItemDisplayInfo(int id)
{
	char *pitem = get_dbdata(&id, m_AllResData[DBRES_ITEMDISPLAYINFO]);
	if(pitem == NULL)
	{
		LOG_INFO("ItemDisplayInfo 不存在id=%d", id);

		/*id = 1009900;
		pitem = get_dbdata(&id, m_AllResData[DBRES_ITEMDISPLAYINFO]);*/
	}

	assert(pitem != NULL);
	return *(tagItemDisplayInfo *)pitem;
}

const tagTaskDef& DBDataManager::getTaskDef(int id)
{
	char *ptask = get_dbdata(&id, m_AllResData[DBRES_TASKDEF]);
	
	if(ptask == NULL)
	{
		LOG_INFO( "TASKDEF 不存在id为 %d 的任务", id );

		id = 1050;
		ptask = get_dbdata(&id, m_AllResData[DBRES_TASKDEF]);
	}

	assert(ptask != NULL);
	return *(tagTaskDef *)ptask;
}

const tagTaskRecycleDef& DBDataManager::getTaskRecycleDef(int id)
{
	char *pctask = get_dbdata(&id, m_AllResData[DBRES_TASKRECYCLEDEF]);
	if(pctask == NULL)
	{
		LOG_INFO("TASKRECYCLEDEF 不存在id=%d", id);

		/*id = 1000;
		pctask = get_dbdata(&id, m_AllResData[DBRES_TASKRECYCLEDEF]);*/
	}

	assert(pctask != NULL);
	return *(tagTaskRecycleDef *)pctask;
}

const tagTaskLimitGroup& DBDataManager::getTaskLimitGroup(int id)
{
	char *pgtask = get_dbdata(&id, m_AllResData[DBRES_TASKLIMITGROUP]);
	if(pgtask == NULL)
	{
		LOG_INFO("TASKLIMITGROUP 不存在id=%d", id);

		/*id = 1000;
		pctask = get_dbdata(&id, m_AllResData[DBRES_TASKRECYCLEDEF]);*/
	}

	assert(pgtask != NULL);
	return *(tagTaskLimitGroup *)pgtask;
}

const tagNpcInfoDef& DBDataManager::getNpcInfoDef(int id)
{
	char *pnpc = get_dbdata(&id, m_AllResData[DBRES_NPCINFO]);
	if(pnpc == NULL)
	{
		/*LOG_INFO( "NPCINFO 不存在id为 %d 的Npc", id );

		id = 300001;
		pnpc = get_dbdata(&id, m_AllResData[DBRES_NPCINFO]);*/
	}

	//assert(pnpc != NULL);
	return *(tagNpcInfoDef *)pnpc;
}

const tagAddAttrDef& DBDataManager::getAddAttrDef(int id)
{
	char *pattr = get_dbdata(&id, m_AllResData[DBRES_ADDATTRDEF]);
	if(pattr == NULL)
	{
		LOG_INFO("ADDATTRDEF 不存在id=%d", id);

		//id = 1;
		//pattr = get_dbdata(&id, m_AllResData[DBRES_ADDATTRDEF]);
	}

	//assert(pattr != NULL);
	return *(tagAddAttrDef *)pattr;
}

const tagQiYueDef& DBDataManager::getQiYueDef(int id)
{
	char *pqiyue = get_dbdata(&id, m_AllResData[DBRES_QIYUEDEF]);
	if(pqiyue == NULL)
	{
		LOG_INFO("QIYUEDEF 不存在id=%d", id);

		//id = 1;
		//pqiyue = get_dbdata(&id, m_AllResData[DBRES_QIYUEDEF]);
	}

	//assert(pqiyue != NULL);
	return *(tagQiYueDef *)pqiyue;
}

const tagDiathesisDef& DBDataManager::getDiathesisDef(int id, int lv)
{
	tagDiathesisDef ID;
	ID.DiathesisID	= id;
	ID.Level		= lv;

	char *pdiathesis = get_dbdata(&ID, m_AllResData[DBRES_DIATHESIS]);
	if(pdiathesis == NULL)
	{
		LOG_INFO("DIATHESIS 不存在id=%d, lv=%d", id, lv);

		//ID.DiathesisID = 1;
		//ID.Level = 1;
		//pdiathesis = get_dbdata(&ID, m_AllResData[DBRES_DIATHESIS]);
	}

	//assert(pdiathesis != NULL);
	return *(tagDiathesisDef*)pdiathesis;
}

const tagGemDef& DBDataManager::getGemDef(int id)
{
	char *pgem = get_dbdata(&id, m_AllResData[DBRES_GEMDEF]);
	if(pgem == NULL)
	{
		LOG_INFO("GEMDEF 不存在id=%d", id);

		//id = 5060001;
		//pgem = get_dbdata(&id, m_AllResData[DBRES_GEMDEF]);
	}

	//assert(pgem != NULL);
	return *(tagGemDef*)pgem;
}

const tagGemNewDef& DBDataManager::getGemNewDef(int id)
{
	char *pgem = get_dbdata(&id, m_AllResData[DBRES_GEMNEWDEF]);
	if(pgem == NULL)
	{
		LOG_INFO("GEMNEWDEF 不存在id=%d", id);

		//id = 5060001;
		//pgem = get_dbdata(&id, m_AllResData[DBRES_GEMDEF]);
	}

	//assert(pgem != NULL);
	return *(tagGemNewDef*)pgem;
}

const tagArmGradeDef& DBDataManager::getArmGradeDef( int nArmType, int nArmLevel )
{
	tagArmGradeDef ID;
	ID.ArmType	= nArmType;
	ID.ArmLevel	= nArmLevel;

	char *pArmGrade = get_dbdata(&ID, m_AllResData[DBRES_ARMGRADEDEF]);
	if(pArmGrade == NULL)
	{
		LOG_INFO("ARMGRADEDEF 不存在nArmType=%d,nArmLevel=%d", nArmType,nArmLevel);
	}
	return *(tagArmGradeDef*)pArmGrade;
}

const tagArmStarDef& DBDataManager::getArmStarDef( int nStar )
{
	tagArmStarDef ID;
	ID.Star = nStar;

	char *pArmStar = get_dbdata( &ID, m_AllResData[DBRES_ARMSTARDEF] );
	if ( pArmStar == NULL )
	{
		LOG_INFO("ARMSTARDEF 不存在nStar=%d", nStar );
	}
	return *(tagArmStarDef*)pArmStar;
}
const tagArmHideAttrDef& DBDataManager::getArmHideAttrDef( int nCareer, int nArmStarLevel )
{
	tagArmHideAttrDef ID;
	ID.Career = nCareer;
	ID.ArmStarLevel = nArmStarLevel;

	char *pArmHideAttr = get_dbdata( &ID, m_AllResData[DBRES_ARMHIDEATTRDEF] );
	if ( pArmHideAttr == NULL )
	{
		LOG_INFO( "ARMHIDEATTRDEF 不存在nCareer=%d,nArmStarLevel=%d", nCareer, nArmStarLevel );
	}
	return *(tagArmHideAttrDef*)pArmHideAttr;
}

//const tagSceneAreaDef&	DBDataManager::getSceneAreaDef(int nMapId, int nListIndex )
//{
//	tagSceneAreaDef	ID;
//	ID.MapID		= nMapId;
//	ID.ListIndex	= nListIndex;
//	tagSceneAreaDef sceneArea = *(tagSceneAreaDef*)get_dbdata(&ID, m_AllResData[DBRES_SCENEAREADEF]); // TODO: DEL
//	return *(tagSceneAreaDef*)get_dbdata(&ID, m_AllResData[DBRES_SCENEAREADEF]);
//}

const tagMonsterSpeechDef* DBDataManager::getMonsterSpeechDef( int id )
{
	char* pMonsterSpeechDef = get_dbdata( &id, m_AllResData[DBRES_MONSTERSPEECHDEF] );
	if( pMonsterSpeechDef == NULL )
	{
		return NULL;
	}

	return (tagMonsterSpeechDef *)pMonsterSpeechDef;
}

const tagClanLevelDef*	DBDataManager::getClanLevelDef( int nLevel )
{
	char* pClanLevelDef = get_dbdata( &nLevel, m_AllResData[DBRES_CLANLEVELDEF] );
	if( pClanLevelDef == NULL )
	{
		LOG_INFO( "tagClanLevelDef不存在level=%d", nLevel );
		return NULL;
	}

	return (tagClanLevelDef *)pClanLevelDef;
}

const tagClanCityLevelDef*	DBDataManager::getClanCityLevelDef( int nLevel )
{
	char* pClanCityLevelDefDef = get_dbdata( &nLevel, m_AllResData[DBRES_CLANCITYLEVELDEF] );
	if( pClanCityLevelDefDef == NULL )
	{
		LOG_INFO( "tagClanCityLevelDef不存在level=%d", nLevel );
	}

	return (tagClanCityLevelDef *)pClanCityLevelDefDef;
}

const tagClanCityBuildingLevelDef*	DBDataManager::getClanCityBuildingLevelDef( int nType, int nLevel )
{
	tagClanCityBuildingLevelDef ID;
	ID.Type		= nType;
	ID.Level	= nLevel;

	char *pClanCityBuilding = get_dbdata(&ID, m_AllResData[DBRES_CLANCITYBUILDINGLEVELDEF]);
	if(pClanCityBuilding == NULL)
	{
		LOG_INFO("tagClanCityBuildingLevelDef 不存在nType=%d, nLevel=%d", nType, nLevel );
	}

	return (tagClanCityBuildingLevelDef*)pClanCityBuilding;
}

const tagDesignationDef& DBDataManager::getDesignationDef( int nId )
{
	char* pDesignationDef = get_dbdata( &nId, m_AllResData[DBRES_DESIGNATIONDEF] );
	if( pDesignationDef == NULL )
	{
		LOG_INFO("DesignationDef不存在id =%d的数据！", nId );
	}
	return *( ( tagDesignationDef* )pDesignationDef );
}

const tagRespactPartyDef& DBDataManager::getRespactPartyDef( int nId )
{
    char* pRespactPartyDef = get_dbdata( &nId, m_AllResData[DBRES_RESPACTPARTYDEF] );
    if( pRespactPartyDef == NULL )
    {
        LOG_INFO("pRespactPartyDef不存在id =%d的数据！", nId );
    }
    return *( ( tagRespactPartyDef* )pRespactPartyDef );
}

const tagRespactPhaseDef& DBDataManager::getRespactPhaseDef( int nId )
{
    char* pRespactPhaseDef = get_dbdata( &nId, m_AllResData[DBRES_RESPACTPHASEDEF] );
    if( pRespactPhaseDef == NULL )
    {
        LOG_INFO("pRespactPhaseDef不存在id =%d的数据！", nId );
    }
    return *( ( tagRespactPhaseDef* )pRespactPhaseDef );
}

const tagFairyDef* DBDataManager::getFairyDef( int nId, int nLv )
{
    tagFairyDef ID;
    ID.FairyID  = nId;
    ID.Level    = nLv;

    char *pFairyDef = get_dbdata(&ID, m_AllResData[DBRES_FAIRYDEF]);
    if(pFairyDef == NULL)
    {
        LOG_INFO("FAIRYDEF 不存在nId=%d, nLv=%d", nId, nLv );
    }

    return (tagFairyDef*)pFairyDef;
}

const tagRideDef* DBDataManager::getRideDef( int nId, int nLv )
{
	tagRideDef ID;
	ID.RideID	= nId;
	ID.Level    = nLv;

	char *pRideDef = get_dbdata(&ID, m_AllResData[DBRES_RIDEDEF]);
	if(pRideDef == NULL)
	{
		LOG_INFO("RIDEDEF 不存在nId=%d, nLv=%d", nId, nLv );
	}

	return ((tagRideDef*)pRideDef);
}

const tagPworldDef* DBDataManager::getPworldDef( int nId )
{
	char* pPworldDef = get_dbdata( &nId, m_AllResData[DBRES_PWORLDDEF] );
	if( pPworldDef == NULL )
	{
		LOG_INFO( "PWORLDDEF 不存在 Id= %d 的副本地图", nId );	
	}
	
	return (tagPworldDef*)pPworldDef;
}

const tagPetDef& DBDataManager::getPetDef( int nId )
{
	char* pPetDef = get_dbdata( &nId, m_AllResData[DBRES_PETDEF] );
	if( pPetDef == NULL )
	{
		LOG_INFO( "PetDef 不存在 Id= %d 的宠物", nId );	
	}

	return *((tagPetDef*)pPetDef);
}

const tagPetSkillGridDef& DBDataManager::getPetSkillGridDef( int nGridIndex )
{
	char* pPetSkillGridDef = get_dbdata( &nGridIndex, m_AllResData[DBRES_PETSKILLGRIDDEF] );
	if( pPetSkillGridDef == NULL )
	{
		LOG_INFO( "PetSkillGridDef 不存在 nGridIndex= %d 的宠物技能格定义", nGridIndex );	
	}

	return *((tagPetSkillGridDef*)pPetSkillGridDef);
}

const tagArmFenJieDef* DBDataManager::getArmFenJieDef( int nLevel,int nImportant )
{
	tagArmFenJieDef ID;
	ID.Level		=	nLevel;
	ID.Important	=	nImportant;

	char* pArmFenJieDef	=	get_dbdata( &ID, m_AllResData[DBRES_ARMFENJIDEF] );
	if ( pArmFenJieDef == NULL )
	{
		LOG_INFO( "ArmFenJieDef 不存在 nLevel= %d，nImportant= %d 的装备分解定义", nLevel, nImportant );
	}
	return (tagArmFenJieDef*)pArmFenJieDef;
}

const tagAddAttrBaseDef* DBDataManager::getAddAttrBaseDef( int nLv )
{
	char* pAddAttrBaseDef = get_dbdata( &nLv, m_AllResData[DBRES_ADDATTRBASEDEF] );
	if( pAddAttrBaseDef == NULL )
	{
		LOG_INFO( "AddAttrBaseDef 不存在 LV= %d 的装备基础关键字", nLv );	
	}

	return (tagAddAttrBaseDef*)pAddAttrBaseDef;
}

const tagPetStrengthenDef*	DBDataManager::getPetStrengthenDef( int nLv )
{
	char* pPetStrengthenDef = get_dbdata( &nLv, m_AllResData[DBRES_PETSTRENGTHENDEF] );
	if( pPetStrengthenDef == NULL )
	{
		LOG_INFO( "PetStrengthenDef 不存在 LV= %d 的宠物强化", nLv );	
	}

	return (const tagPetStrengthenDef*)pPetStrengthenDef;
}

const tagPetLevelDef* DBDataManager::getPetLevelDef( int nLv )
{
	char* pPetLevelDef = get_dbdata( &nLv, m_AllResData[DBRES_PETLEVELDEF] );
	if( pPetLevelDef == NULL )
	{
		LOG_INFO( "PetLevelDef 不存在 LV= %d 的宠物升级定义", nLv );	
	}

	return (const tagPetLevelDef*)pPetLevelDef;
}

const tagPetTraitDef* DBDataManager::getPetTraitDef( int nTraidId )
{
	char* pPetTraitDef = get_dbdata( &nTraidId, m_AllResData[DBRES_PETTRAITDEF] );
	if( pPetTraitDef == NULL )
	{
		LOG_INFO( "PetTraitDef 不存在 id= %d 的宠物性格定义", nTraidId );	
	}

	return (const tagPetTraitDef*)pPetTraitDef;
}

const tagPetTraitDef* DBDataManager::getPetTraitDef( int nTraidId,int nStar )
{
	tagPetTraitDef ID;
	ID.TraitID = nTraidId;
	ID.Star = nStar;
	char* pPetTraitDef = get_dbdata( &ID, m_AllResData[DBRES_PETTRAITDEF] );
	if( pPetTraitDef == NULL )
	{
		LOG_INFO( "PetTraitDef 不存在 id= %d,Star=%d 的宠物性格定义", nTraidId,nStar );
	}

	return (const tagPetTraitDef*)pPetTraitDef;
}

const tagFixedAddAttr* DBDataManager::getFixedAddAttr( int id )
{
    char* pFixedAddAttr = get_dbdata( &id, m_AllResData[DBRES_FIXEDADDATTR] );
    if( pFixedAddAttr == NULL )
    {
        LOG_INFO( "FixedAddAttr 不存在 id= %d 的定义", id );	
    }
    return (const tagFixedAddAttr*)pFixedAddAttr;
}

void DBDataManager::loadNpcDisplayInfo(std::vector<NPCDISPLAYINFO>&npcinfo)
{
	ResTable *pnpctable = m_pResDB->GetTable("NpcDisplayInfo");
	int num = pnpctable->FindRecord(NULL, 0, NULL);
	npcinfo.resize(num);
	if(num > 0) pnpctable->FindRecord(&npcinfo[0], num, NULL);
}

void DBDataManager::loadMapMonsters(int mapid, std::vector<MAPMONSTER>&monsters)
{
	ResTable *ptable = m_pResDB->GetTable("MapMonster");
	int num = ptable->FindRecord(NULL, 0, "MapID", mapid);
	monsters.resize(num);
	if(num > 0) ptable->FindRecord(&monsters[0], num, "MapID", mapid);
}

bool DBDataManager::saveMapMonsters(int mapid, const std::vector<MAPMONSTER>&monsters)
{
	size_t i;
	ResTable *ptable = m_pResDB->GetTable("MapMonster");

	std::vector<MAPMONSTER>old;
	loadMapMonsters(mapid, old);
	for(i=0; i<old.size(); i++)
	{
		if(ptable->DeleteRecord(&old[i]) < 0) return false;
	}

	for(i=0; i<monsters.size(); i++)
	{
		if(ptable->UpdateRecord(&monsters[i]) <= 0)
		{
			if(ptable->InsertRecord(&monsters[i]) < 0) return false;
		}
	}

	return true;
}

void DBDataManager::loadMapProcEvent(int mapid, std::vector<PROCEVENT>&procevents)
{
	ResTable *ptable = m_pResDB->GetTable("ProcEvent");

	int num = ptable->FindRecord(NULL, 0, "MapID", mapid);
	procevents.resize(num);
	if(num > 0) ptable->FindRecord(&procevents[0], num, "MapID", mapid);
}

bool DBDataManager::saveMapProcEvent(int mapid, const std::vector<PROCEVENT>&procevents)
{
	size_t i;

	ResTable *ptable = m_pResDB->GetTable("ProcEvent");

	std::vector<PROCEVENT>old;
	loadMapProcEvent(mapid, old);
	for(i=0; i<old.size(); i++)
	{
		if(ptable->DeleteRecord(&old[i]) < 0) return false;
	}

	for(i=0; i<procevents.size(); i++)
	{
		if(ptable->UpdateRecord(&procevents[i]) <= 0)
		{
			if(ptable->InsertRecord(&procevents[i]) < 0) return false;
		}
	}

	return true;
}

void DBDataManager::saveMapDef(const MAPDEF &mapdef)
{
	ResTable *ptable = m_pResDB->GetTable("MapDef");

	ptable->UpdateRecord(&mapdef);
}

void DBDataManager::loadMapNpc(std::vector<MAPNPC>&mapnpcinfo)
{
	if( m_ResDBMode == RESDB_DB && m_pResDB != NULL )
	{
		ResTable *pnpctable = m_pResDB->GetTable("MapNpc");
		int num = pnpctable->FindRecord(NULL, 0, NULL);
		mapnpcinfo.resize(num);
		if(num > 0) pnpctable->FindRecord(&mapnpcinfo[0], num, NULL);
	}
	else
	{
		int num = m_AllResData[DBRES_MAPNPC].count;
		mapnpcinfo.resize(num);
		for( int i = 0; i < num; i++ )
		{
			MAPNPC mapNpc = *( (MAPNPC*)( m_AllResData[DBRES_MAPNPC].pbuffer + i*sizeof(MAPNPC) )  );
			mapnpcinfo[i] = mapNpc;
		}
	}
}

void DBDataManager::loadSkillDef(std::vector<tagSkillDef>&skilldef)
{
	if( m_ResDBMode == RESDB_DB )
	{
		ResTable* pskilltable = m_pResDB->GetTable("SkillDef");
		int num = pskilltable->FindRecord(NULL, 0, NULL);
		skilldef.resize(num);
		if(num > 0) pskilltable->FindRecord(&skilldef[0], num, NULL);
	}
	else
	{
		int num = m_AllResData[DBRES_SKILLDEF].count;
		skilldef.resize(num);
		for( int i = 0; i < num; i++ )
		{
			tagSkillDef mapSkill = *( (tagSkillDef*)( m_AllResData[DBRES_SKILLDEF].pbuffer + i*sizeof(tagSkillDef) )  );
			skilldef[i] = mapSkill;
		}
	}
}

void DBDataManager::loadArmHurtDef( std::vector<ARMLEVELHURTDEF>&HurtDef )
{
	if( m_ResDBMode == RESDB_DB )
	{
		ResTable* pHurttable = m_pResDB->GetTable("ArmLevelHurtDef");
		int num = pHurttable->FindRecord(NULL, 0, NULL);
		HurtDef.resize(num);
		if(num > 0) pHurttable->FindRecord(&HurtDef[0], num, NULL);
	}
	else
	{
		int num = m_AllResData[DBRES_ARMLEVELHURTDEF].count;
		HurtDef.resize(num);
		for( int i = 0; i < num; i++ )
		{
			ARMLEVELHURTDEF mapSkill = *( (ARMLEVELHURTDEF*)( m_AllResData[DBRES_ARMLEVELHURTDEF].pbuffer + i*sizeof(ARMLEVELHURTDEF) ) );
			HurtDef[i] = mapSkill;
		}
	}
}

void DBDataManager::loadTaskDef( std::vector<tagTaskDef*>& taskdef )
{
	if( m_ResDBMode == RESDB_DB && m_pResDB == NULL )
	{
		return;
	}

	int num = m_AllResData[DBRES_TASKDEF].count;
	taskdef.resize( num );
	for ( int i = 0; i < num; ++i )
	{
		taskdef[i] = ( tagTaskDef* )( m_AllResData[DBRES_TASKDEF].pbuffer + i * sizeof( tagTaskDef ) );
	}
}

void DBDataManager::loadSceneAreaDef( std::vector<tagSceneAreaDef>&	sceneAreadef )
{
	if( m_ResDBMode == RESDB_DB && m_pResDB != NULL )
	{
		ResTable* pSceneAreaDeftable = m_pResDB->GetTable("SceneAreaDef");
		int num = pSceneAreaDeftable->FindRecord(NULL, 0, NULL);
		sceneAreadef.resize(num);
		if(num > 0) pSceneAreaDeftable->FindRecord(&sceneAreadef[0], num, NULL);
	}
	else
	{
		int num = m_AllResData[DBRES_SCENEAREADEF].count;
		sceneAreadef.resize(num);
		for( int i = 0; i < num; i++ )
		{
			tagSceneAreaDef mapSceneAreaDef = *( (tagSceneAreaDef*)( m_AllResData[DBRES_SCENEAREADEF].pbuffer +
												i * sizeof( tagSceneAreaDef) ) );
			sceneAreadef[i] = mapSceneAreaDef;
		}
	}
}

void DBDataManager::loadMapDef( std::vector<tagMapDef>&	mapVector )
{
	if ( m_ResDBMode == RESDB_DB && m_pResDB != NULL )
	{
		ResTable* pMapDeftable = m_pResDB->GetTable( "MapDef" );
		assert( pMapDeftable != NULL && "读取到空表!" );
		int	num = pMapDeftable->FindRecord( NULL, 0, NULL );
		assert( num >= 0 && "读取表失败!" );
		mapVector.resize( num );

		if ( num > 0 )	pMapDeftable->FindRecord( &(mapVector[0]), num, NULL );
	} 
	else
	{
		int	num = m_AllResData[DBRES_MAPDEF].count;
		mapVector.resize( num );
		for ( int i = 0; i < num; ++i )
		{
			tagMapDef mapDef = *( ( tagMapDef* )( m_AllResData[DBRES_MAPDEF].pbuffer + i * sizeof( MAPDEF) ) );
			mapVector[i] = mapDef;
		}
	}
}

void DBDataManager::loadWorldMapSearchDef( std::vector<tagWorldMapSearchDef>& mapSearchVec )
{
	if ( m_ResDBMode == RESDB_DB && m_pResDB != NULL )
	{
		ResTable* pMapSearchDeftable = m_pResDB->GetTable( "WorldMapSearchDef" );
		assert( pMapSearchDeftable != NULL );
		int	num = pMapSearchDeftable->FindRecord( NULL, 0, NULL );
		assert( num >= 0 );
		mapSearchVec.resize( num );

		if ( num > 0 )	pMapSearchDeftable->FindRecord( &(mapSearchVec[0]), num, NULL );
	} 
	else
	{
		// TODO:DBRES_MAPDEF这个枚举值如何定义
		int	num = m_AllResData[DBRES_WORLDMAPSEARCHDEF].count;
		mapSearchVec.resize( num );
		for ( int i = 0; i < num; ++i )
		{
			tagWorldMapSearchDef& mapSearchDef = *( ( tagWorldMapSearchDef* )( m_AllResData[DBRES_WORLDMAPSEARCHDEF].pbuffer + i * sizeof( WORLDMAPSEARCHDEF) ) );
			mapSearchVec[i] = mapSearchDef;
		}
	}
}

void DBDataManager::loadSuitSet( std::map<int,std::vector<int>>& mapSuitSet )
{
	std::vector<ITEMDEF> vectorItemDef;
	if( m_ResDBMode == RESDB_DB && m_pResDB != NULL )
	{
		ResTable* pItemDeftable = m_pResDB->GetTable( "ItemDef" );
		assert( pItemDeftable != NULL && "读到空的ItemDef表!" );
		int num = pItemDeftable->FindRecord( NULL, 0, NULL );
		assert( num >= 0 && "读取ItemDef失败!");
		vectorItemDef.resize( num );
		if( num > 0 )
		{
			pItemDeftable->FindRecord( &(vectorItemDef[0]), num, NULL );
		}
	}else{
		int num = m_AllResData[DBRES_ITEMDEF].count;
		vectorItemDef.resize( num );
		for( int i = 0; i < num; ++i )
		{
			ITEMDEF itemDef = *( ( ITEMDEF* )( m_AllResData[DBRES_ITEMDEF].pbuffer + i * sizeof( ITEMDEF ) ) );
			vectorItemDef[i] = itemDef;
		}
	}

	std::vector<ITEMDEF>::iterator iter = vectorItemDef.begin();
	while( iter != vectorItemDef.end() )
	{
		if ( iter->SuitSetID > 0 )
		{
			std::map<int,std::vector<int>>::iterator mapiter = mapSuitSet.find( iter->SuitSetID );
			if( mapiter != mapSuitSet.end() )
			{
				mapiter->second.push_back( iter->ItemID );
			}else{
				mapSuitSet[iter->SuitSetID].push_back( iter->ItemID ); 
			}
		}
		iter++;
	}
}

void DBDataManager::loadDesignation( std::map<std::string,std::vector<tagDesignationDef*>>& mapDesignationDef, std::vector<tagDesignationDef*>& vectDesignationDef )
{
	if( m_ResDBMode == RESDB_DB && m_pResDB == NULL )
	{
		return;
	}
	
    int num = m_AllResData[DBRES_DESIGNATIONDEF].count;
    vectDesignationDef.resize( num );
    for( int i = 0; i < num; ++i )
    {
        vectDesignationDef[i] = ( ( tagDesignationDef* )( m_AllResData[DBRES_DESIGNATIONDEF].pbuffer + i * sizeof( tagDesignationDef ) ) );
    }

	// 按类别归类数据
	std::vector<tagDesignationDef*>::iterator iter = vectDesignationDef.begin();
	while( iter != vectDesignationDef.end() )
	{
		assert( strcmp( (*iter)->CategoryName, "" ) != 0 );
		std::map<std::string,std::vector<tagDesignationDef*>>::iterator mapiter = mapDesignationDef.find( (*iter)->CategoryName );
		if( mapiter != mapDesignationDef.end() )
		{
			mapiter->second.push_back( *iter );
		}else{
			mapDesignationDef[(*iter)->CategoryName].push_back( *iter );
		}
		++iter;
	}
}

void DBDataManager::loadRespactphaseDef( std::vector<tagRespactPhaseDef*>& vectRespactPhaseDef )
{
	if( m_ResDBMode == RESDB_DB && m_pResDB == NULL )
	{
		return;
	}

    int num = m_AllResData[DBRES_RESPACTPHASEDEF].count;
    vectRespactPhaseDef.resize( num );
    for( int i = 0; i < num; ++i )
    {
        vectRespactPhaseDef[i] = ( tagRespactPhaseDef* )( m_AllResData[DBRES_RESPACTPHASEDEF].pbuffer + i * sizeof( tagRespactPhaseDef ) );
    }
}

void DBDataManager::loadRespactPartyDef( std::map<int,std::vector<tagRespactPartyDef*>>& mapRespactPartyDef )
{
	if( m_ResDBMode == RESDB_DB && m_pResDB == NULL )
	{
		return;
	}
    std::vector<tagRespactPartyDef*> vectRespactPartyDef;

    int num = m_AllResData[DBRES_RESPACTPARTYDEF].count;
    vectRespactPartyDef.resize( num );
    for( int i = 0; i < num; ++i )
    {
        vectRespactPartyDef[i] = ( tagRespactPartyDef* )( m_AllResData[DBRES_RESPACTPARTYDEF].pbuffer + i * sizeof( tagRespactPartyDef ) );
    } 
	std::sort( vectRespactPartyDef.begin(), vectRespactPartyDef.end() );
    // 按类别归类数据
    std::vector<tagRespactPartyDef*>::iterator iter = vectRespactPartyDef.begin();
    while( iter != vectRespactPartyDef.end() )
    {
        assert( strcmp( (*iter)->CategoryName, "" ) != 0 );
        bool bFound = false;
        std::map<int,std::vector<tagRespactPartyDef*>>::iterator mapiter = mapRespactPartyDef.begin();
        while( mapiter != mapRespactPartyDef.end() )
        {
            for( size_t i = 0; i < mapiter->second.size(); i++ )
            {
                if( strcmp( (*iter)->CategoryName, ( mapiter->second[i] )->CategoryName ) == 0 )
                {
                    mapiter->second.push_back( *iter );
                    bFound = true;
                    break;
                }
            }
            mapiter++;
        }
       
        if( !bFound )
        {
            mapRespactPartyDef[(*iter)->ID].push_back( *iter );
        }
        ++iter;
	}
}

void DBDataManager::loadCanFittingItem( std::map<int,std::vector<int>>& m_mapCanFittingItem )
{
	if( m_ResDBMode == RESDB_DB && m_pResDB == NULL )
	{
		return;
	}

	std::vector<tagItemDef*> vectItemDef;
	int num = m_AllResData[DBRES_ITEMDEF].count;
	vectItemDef.resize( num );
	for( int i = 0; i < num; ++i )
	{
		vectItemDef[i] = ( tagItemDef* )( m_AllResData[DBRES_ITEMDEF].pbuffer + i * sizeof( tagItemDef ) );
	} 
	
	std::vector<tagItemDef*>::iterator iter = vectItemDef.begin();
	while( iter != vectItemDef.end() )
	{
		int nIndex = -1;
		for( int i = 0; i < MAX_ITEM_RESULT; i++ )
		{
			if( (*iter)->Result[i].ResultID == RESULT_FITTINGS_INLAY )
			{
				nIndex = i;
			}
		}

		if( nIndex != -1 )
		{
			std::map<int,std::vector<int>>::iterator mapiter = m_mapCanFittingItem.find( (*iter)->Result[nIndex].ResultVal1 );
			if( mapiter != m_mapCanFittingItem.end() )
			{
				mapiter->second.push_back( (*iter)->ItemID );
			}else{
				m_mapCanFittingItem[(*iter)->Result[nIndex].ResultVal1].push_back( (*iter)->ItemID );
			}
		}
		iter++;
	}
}

void DBDataManager::loadDiathesisDef( std::vector<tagDiathesisDef>& diathesisdef )
{
	if ( m_ResDBMode == RESDB_DB && m_pResDB != NULL )
	{
		ResTable *pnpctable = m_pResDB->GetTable( "DiathesisDef" );
		int num = pnpctable->FindRecord( NULL, 0, NULL );
		diathesisdef.resize( num );
		if ( num > 0 ) 
		{
			pnpctable->FindRecord( &diathesisdef[0], num, NULL );
		}
	}
	else
	{
		int num = m_AllResData[DBRES_DIATHESIS].count;
		diathesisdef.resize( num );
		for ( int i = 0; i < num; i++ )
		{
			const tagDiathesisDef& diathess = *( ( tagDiathesisDef* )( m_AllResData[DBRES_DIATHESIS].pbuffer + 
													i * sizeof( tagDiathesisDef ) )  );
			diathesisdef[i] = diathess;
		}
	}
}

void DBDataManager::loadLVGiftDef( std::vector<tagLVGift*>& vectLVGiftDef )
{
    if( m_ResDBMode == RESDB_DB && m_pResDB == NULL )
    {
        return;
    }

    int num = m_AllResData[DBRES_LVGIFTDEF].count;
    vectLVGiftDef.resize( num );
    for( int i = 0; i < num; ++i )
    {
        vectLVGiftDef[i] = ( tagLVGift* )( m_AllResData[DBRES_LVGIFTDEF].pbuffer + i * sizeof( tagLVGift ) );
    }
}

const tagBCTDef* DBDataManager::getBCTDef(int id)
{
	char* pBCTDef = get_dbdata( &id, m_AllResData[DBRES_BCT] );
	if( pBCTDef == NULL )
	{
		LOG_INFO( "BCTDef 不存在 id= %d 的定义", id );	
	}
	return (const tagBCTDef*)pBCTDef;
}

const tagRndAwdDef* DBDataManager::getRndAwdDef(int id)
{
	char* pRndAwdDef = get_dbdata( &id, m_AllResData[DBRES_RNDAWD] );
	if( pRndAwdDef == NULL )
	{
		LOG_INFO( "RndAwdDef 不存在 id= %d 的定义", id );	
	}
	return (const tagRndAwdDef*)pRndAwdDef;
}

void DBDataManager::loadActiveHelper(std::vector<tagActiveHelper*>& activeHelper)
{
	if( m_ResDBMode == RESDB_DB && m_pResDB == NULL )
	{
		return;
	}
	int num = m_AllResData[DBRES_ACTIVEHELPER].count;
	activeHelper.resize(num);
	for( int i = 0; i < num; ++i )
	{
		activeHelper[i] = ( tagActiveHelper* )( m_AllResData[DBRES_ACTIVEHELPER].pbuffer + i * sizeof( tagActiveHelper ) );
	}

}

void DBDataManager::loadCangBaoTuDef(std::vector<tagCangBaoTuDef*>& vecCangBaoTuDef )
{
	if( m_ResDBMode == RESDB_DB && m_pResDB == NULL )
	{
		return;
	}
	int num = m_AllResData[DBRES_CANGBAOTU].count;
	vecCangBaoTuDef.resize(num);
	for( int i = 0; i < num; ++i )
	{
		vecCangBaoTuDef[i] = ( tagCangBaoTuDef* )( m_AllResData[DBRES_CANGBAOTU].pbuffer + i * sizeof( tagCangBaoTuDef ) );
	}
}

void DBDataManager::loadPetStrengthenDef(std::vector<tagPetStrengthenDef*>& vecPetStrengthenDef )
{
	if( m_ResDBMode == RESDB_DB && m_pResDB == NULL )
	{
		return;
	}
	int num = m_AllResData[DBRES_PETSTRENGTHENDEF].count;
	vecPetStrengthenDef.resize(num);
	for ( int i = 0; i < num; ++i )
	{
		vecPetStrengthenDef[i] = ( tagPetStrengthenDef* )( m_AllResData[DBRES_PETSTRENGTHENDEF].pbuffer + 
														i * sizeof( tagPetStrengthenDef ) );
	}
}

void DBDataManager::loadGemDef( std::vector<tagGemDef*>& vecGemDef )
{
	if( m_ResDBMode == RESDB_DB && m_pResDB == NULL )
	{
		return;
	}
	int num = m_AllResData[DBRES_GEMDEF].count;
	vecGemDef.resize(num);
	for( int i = 0; i < num; ++i )
	{
		vecGemDef[i] = ( tagGemDef* )( m_AllResData[DBRES_GEMDEF].pbuffer + i * sizeof( tagGemDef ) );
	}
}
void DBDataManager::loadGemNewDef( std::vector<tagGemNewDef*>& vecGemNewDef )
{
	if ( m_ResDBMode == RESDB_DB && m_pResDB == NULL )
	{
		return;
	}
	int num = m_AllResData[DBRES_GEMNEWDEF].count;
	vecGemNewDef.resize(num);
	for( int i = 0; i < num; ++i )
	{
		vecGemNewDef[i] = ( tagGemNewDef* )(m_AllResData[DBRES_GEMNEWDEF].pbuffer + i * sizeof( tagGemNewDef ) );
	}
}

const tagSecondarySkillDef* DBDataManager::getSecondarySkillDef(int nType,int nLevel)
{
	tagSecondarySkillDef id;
	id.SkillType = nType;
	id.Level = nLevel;
	char* pSecondarySkillDef = get_dbdata( &id,m_AllResData[DBRES_SECONDARYSKILL] );
	if( pSecondarySkillDef == NULL )
	{
		LOG_INFO( "SecondarySkillDef 不存在 SkillType=%d,Level=%d 的定义",nType, nLevel );
	}
	return (const tagSecondarySkillDef*)pSecondarySkillDef;
}

const tagRecipeDef* DBDataManager::getRecipeDef( int nType,int id )
{
	tagRecipeDef recipeId;
	recipeId.SkillType = nType;
	recipeId.RecipeID = id;
	char* pRecipeDef = get_dbdata( &recipeId,m_AllResData[DBRES_RECIPE] );
	if( pRecipeDef == NULL )
	{
		//LOG_INFO(" RecipeDef 不存在 type=%d,id=%d 的定义",nType,id);
	}
	return (const tagRecipeDef*) pRecipeDef;
}

const tagWorldMapSearchDef*	DBDataManager::getWorldMapSearchDef( int nFromMapID )
{
	char* pWorldMapSearchDef = get_dbdata( &nFromMapID, m_AllResData[DBRES_WORLDMAPSEARCHDEF] );
	if( pWorldMapSearchDef == NULL )
	{
		LOG_INFO( "WorldMapSearchDef 不存在 nFromMapID= %d 的定义", nFromMapID );	
	}
	return (const tagWorldMapSearchDef*)pWorldMapSearchDef;
}

const tagCangBaoTuDef* DBDataManager::getCangBaoTuDefDef( int nMapID, int nLevel, int nItemID )
{
	tagCangBaoTuDef id;
	id.MapID	= nMapID;
	id.Level	= nLevel;
	id.ItemID	= nItemID;
	char* pCangBaoTuDef = get_dbdata( &id,m_AllResData[DBRES_CANGBAOTU] );
	if( pCangBaoTuDef == NULL )
	{
		LOG_INFO( "CangBaoTuDef 不存在 mapID=%d, level = %d, itemID = %d 的定义", nMapID, nLevel, nItemID );
	}
	return (const tagCangBaoTuDef*)pCangBaoTuDef;
}

const tagClanTechDef* DBDataManager::getClanTechDef( int nCat,int nLevel )
{
	tagClanTechDef id;
	id.Category = nCat;
	id.Level = nLevel;
	char* pClanTechDef = get_dbdata(&id,m_AllResData[DBRES_CLANTECHDEF]);
	if( pClanTechDef == NULL )
	{
		//LOG_INFO("ClanTechDef 不存在Category=%d, Level=%d 的定义",nCat,nLevel);
	}
	return ( const tagClanTechDef* )pClanTechDef;
}

const tagClanProductDef* DBDataManager::getClanProductDef( int nID )
{
	char* pProductDef = get_dbdata(&nID,m_AllResData[DBRES_CLANPRODUCTDEF]);
	if( pProductDef == NULL )
	{
		LOG_INFO("ProductDef 不存在id=%d, 的定义",nID);
	}
	return ( const tagClanProductDef* ) pProductDef;
}

int DBDataManager::getClanProductDefNum()
{
	return m_AllResData[DBRES_CLANPRODUCTDEF].count;
}

const tagClanProductDef* DBDataManager::getClanProductDefByIndex(int nIndex)
{
	if( nIndex < m_AllResData[DBRES_CLANPRODUCTDEF].count )
	{
		return (tagClanProductDef*)(m_AllResData[DBRES_CLANPRODUCTDEF].pbuffer + nIndex*sizeof(tagClanProductDef));
	}	
	return NULL;
}
const tagMonMachineDef*	DBDataManager::getMonMachineDef( int nMonsterID )
{
	char* pMonMachineDef = get_dbdata( &nMonsterID,m_AllResData[DBRES_MONMACHINEDEF] );
	if( pMonMachineDef == NULL )
	{
		LOG_INFO("monmachinedef 中不存在MonsterID=%d的定义",nMonsterID);
	}
	return (const tagMonMachineDef*)pMonMachineDef;
}

const tagContOnlineGiftDef* DBDataManager::getContOnlineGiftDef(int nID)
{
	char* pDef = get_dbdata( &nID, m_AllResData[DBRES_CONTONLINEGIFTDEF] );
	if( pDef == NULL )
	{
		LOG_INFO( "ContOnlineGiftDef 不存在 GiftID= %d 的定义", nID );	
	}
	return (const tagContOnlineGiftDef*)pDef;
}

void DBDataManager::loadBattleDef( std::vector<tagBattleDef*>& vecBattleDef )
{
	if( m_ResDBMode == RESDB_DB && m_pResDB == NULL )
	{
		return;
	}
	int num = m_AllResData[DBRES_BATTLEDEF].count;
	vecBattleDef.resize(num);
	for( int i = 0; i < num; ++i )
	{
		vecBattleDef[i] = ( tagBattleDef* )( m_AllResData[DBRES_BATTLEDEF].pbuffer + i * sizeof( tagBattleDef ) );
	}
}

void DBDataManager::loadBirthItemDef( std::vector<tagBirthItemDef*>& vecBirthItemDef )
{
	if( m_ResDBMode == RESDB_DB && m_pResDB == NULL )
	{
		return;
	}
	int num = m_AllResData[DBRES_BIRTHITEMDEF].count;
	vecBirthItemDef.resize(num);
	for( int i = 0; i < num; ++i )
	{
		vecBirthItemDef[i] = ( tagBirthItemDef* )( m_AllResData[DBRES_BIRTHITEMDEF].pbuffer + i * sizeof( tagBirthItemDef ) );
	}
}

const tagClanBuildingDisplayInfo* DBDataManager::getBuildingDisplayInfo(int nID)
{
	char* pDef = get_dbdata( &nID,m_AllResData[DBRES_CLAN_BUILDINGDISPLAYINFO] );
	if( pDef == NULL )
	{
		LOG_INFO("ClanBuildingDisplayInfo 中不存在 id=%d的定义",nID);
	}
	return (const tagClanBuildingDisplayInfo*)pDef;
}

const tagFairyStarStuffDef* DBDataManager::getFairyStarStuffDef( int itemID )
{
	char* pDef = get_dbdata( &itemID,m_AllResData[DBRES_FAIRYSTARSTUFFDEF] );
	if( pDef == NULL )
	{
		LOG_INFO( "FairyStarStuffDef 中不存在 itemId=%d的定义",itemID );
	}
	return ( const tagFairyStarStuffDef* )pDef;
}

const tagFairyStarDef* DBDataManager::getFairyStarDef( int nFairyId,int nStar )
{
	tagFairyStarDef id;
	id.FairyID = nFairyId;
	id.Star = nStar;
	char* pDef = get_dbdata( &id,m_AllResData[DBRES_FAIRYSTARDEF] );
	if( pDef == NULL )
	{
		LOG_INFO( "FairyStarDef 中不存在 FairyID=%d,Star=%d的定义",nFairyId,nStar );
	}
	return ( const tagFairyStarDef* )pDef;
}

const tagOnlineGiftGroup* DBDataManager::getOnlineGiftGroup( int nID )
{
	char* pDef = get_dbdata( &nID,m_AllResData[DBRES_ONLINEGIFTGROUP] );
	if( pDef == NULL )
	{
		LOG_INFO("OnlineGiftGroup中不存在ID=%d的定义",nID);
	}
	return ( const tagOnlineGiftGroup* ) pDef;
}

const tagArmRandDef* DBDataManager::getArmRandDef( int nLevel,int nType )
{
	tagArmRandDef def;
	def.Level = nLevel;
	def.LevelType = nType;
	char* pDef = get_dbdata( &def,m_AllResData[DBRES_ARMRANDDEF] );
	if( pDef == NULL )
	{
		LOG_INFO("ArmRandDef中不存在Level=%d,LevelType=%d的定义",nLevel,nType);
	}
	return ( const tagArmRandDef* ) pDef;
}

const tagGameEventDef* DBDataManager::getGameEventDef(int eventId)
{
	tagGameEventDef def;
	def.EventID = eventId;
	char* pDef = get_dbdata( &def,m_AllResData[DBRES_GAMEEVENTDEF] );
	if( pDef == NULL )
	{
		LOG_INFO("GameEventDef中不存在EventID=%d的定义", eventId);
	}
	return (const tagGameEventDef*)pDef;
}

const tagBattleDef* DBDataManager::getBattleDef( int battleId )
{
	char* pDef = get_dbdata(&battleId,m_AllResData[DBRES_BATTLEDEF]);
	if( pDef == NULL )
	{
		LOG_INFO("BattleDef 中不存在BattleId=%d的定义",battleId);
	}
	return (const tagBattleDef*) pDef;
}

const tagDiffPworldDef* DBDataManager::getDiffPworldDef(int id)
{
	char* pDef = get_dbdata(&id, m_AllResData[DBRES_DIFFPWORLDDEF]);
	if( pDef == NULL )
	{
		LOG_INFO("DiffPworldDef 中不存在DiffPworldID=%d的定义", id);
	}
	return (const tagDiffPworldDef*)pDef;
}

const tagActiveHelper* DBDataManager::getActiveHelper(int id)
{
	char* pDef = get_dbdata(&id, m_AllResData[DBRES_ACTIVEHELPER]);
	if( pDef == NULL )
	{
		LOG_INFO("ActiveHelper 中不存在ActiveID=%d的定义", id);
	}
	return (const tagActiveHelper*)pDef;
}

const tagWorldCityBuildDef* DBDataManager::getWorldCityBuildDef(int id)
{
	char* pDef = get_dbdata(&id, m_AllResData[DBRES_WORLDCITYBUILDDEF]);
	if( pDef == NULL )
	{
		LOG_INFO("WorldCityBuildDef 中不存在ID=%d的定义", id);
	}
	return (const tagWorldCityBuildDef *)pDef;
}

const tagCityProductDef* DBDataManager::getCityProductDef(int id)
{
	char* pDef = get_dbdata(&id, m_AllResData[DBRES_CITYPRODUCTDEF]);
	return (const tagCityProductDef *)pDef;
}

void DBDataManager::loadRuneAddAttrDef(std::vector<tagRuneAddAttrDef>& verRuneAddAttrDef)
{
	if( m_ResDBMode == RESDB_DB )
	{
		ResTable* pskilltable = m_pResDB->GetTable("RuneAddAttrDef");
		int num = pskilltable->FindRecord(NULL, 0, NULL);
		verRuneAddAttrDef.resize(num);
		if (num > 0)
			pskilltable->FindRecord(&verRuneAddAttrDef[0], num, NULL);
	}
	else
	{
		int num = m_AllResData[DBRES_RUNEADDATTRDEF].count;
		verRuneAddAttrDef.resize(num);
		for( int i = 0; i < num; i++ )
		{
			verRuneAddAttrDef[i] = *((tagRuneAddAttrDef *)(m_AllResData[DBRES_RUNEADDATTRDEF].pbuffer + i * sizeof(tagRuneAddAttrDef)));
		}
	}
}

int DBDataManager::getResTableDefNum( unsigned int type )
{
	if( type > DBRES_MAX_NUM )
	{
		return 0;
	}
	return m_AllResData[type].count;
}
const tagGemLevelStarDef* DBDataManager::getGemLevelStarDef( int nLevel, int nStar )
{
	tagGemLevelStarDef ID;
	ID.Level = nLevel;
	ID.Star = nStar;

	char* pDef = get_dbdata( &ID, m_AllResData[DBEES_GEMLEVELSTARDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO("GemLevelStarDef 中不存在Level=%d,Star=%d的定义", nLevel, nStar);
		return NULL;
	}
	return ( const tagGemLevelStarDef* ) pDef;
}
const tagGemAttrStarDef* DBDataManager::getGemAttrStarDef( int nAttrID, int nLevel, int nStar )
{
	tagGemAttrStarDef ID;
	ID.AttrID = nAttrID;
	ID.Star = nStar;
	ID.Level = nLevel;

	char* pDef = get_dbdata( &ID,m_AllResData[DBRES_GEMATTRSTARDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO("GemAttrStarDef 中不存在AttrID=%d,Level=%d,Star=%d的定义", nAttrID, nLevel, nStar );
		return NULL;
	}
	return ( const tagGemAttrStarDef* ) pDef;
}

const tagRuneAddAttrDef* DBDataManager::getRuneAddAttrDef(int id)
{
	tagRuneAddAttrDef ID;
	ID.ID = id;
	char* pDef = get_dbdata( &ID, m_AllResData[DBRES_RUNEADDATTRDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO("RuneAddAttrDef 中不存在ID=%d的定义", id );
		return NULL;
	}
	return ( const tagRuneAddAttrDef* ) pDef;
}

const tagUseDef* DBDataManager::getUseDef( int nID )
{
	tagUseDef ID;
	ID.UseID = nID;
	char* pDef = get_dbdata( &ID, m_AllResData[DBRES_USEDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO("UseDef 中不存在UseID=%d的定义", nID );
		return NULL;
	}
	return ( const tagUseDef* ) pDef;
}

int DBDataManager::getOneDayGiftNum()
{
	return m_AllResData[DBRES_ONEDAYGIFTDEF].count;
}

int DBDataManager::getStarFuncNum()
{
	return m_AllResData[DBRES_STARFUNCDEF].count;
}

tagOneDayGift* DBDataManager::getOneDayGift( int idx )
{
	if( idx >= m_AllResData[DBRES_ONEDAYGIFTDEF].count )
	{
		return NULL;
	}
	return ( tagOneDayGift* )( m_AllResData[DBRES_ONEDAYGIFTDEF].pbuffer + idx * sizeof( tagOneDayGift ) );
}

const tagMediaGiftDef* DBDataManager::getMediaGiftDef( int id )
{
	char* pDef = get_dbdata(&id,m_AllResData[DBRES_MEDIAGIFTDEF]);
	if( pDef == NULL )
	{
		LOG_INFO("MediaGiftDef 中不存在ID=%d的定义", id);
	}
	return (const tagMediaGiftDef*) pDef;
}

const tagLtrGiftDef* DBDataManager::getLtrGiftDef( int id )
{
	char* pDef = get_dbdata(&id,m_AllResData[DBRES_LTRGIFTDEF]);
	if(pDef == NULL )
	{
		LOG_INFO("LtrGiftDef 中不存在ID=%d的定义", id);
	}
	return (const tagLtrGiftDef*) pDef;
}

const tagArmRecastDef* DBDataManager::getArmRecastDef( int nID )
{
	char* pDef = get_dbdata(&nID,m_AllResData[DBRES_ARMRECASTDEF]);
	if(pDef == NULL )
	{
		LOG_INFO("ArmRecastDef 中不存在ID=%d的定义", nID);
	}
	return (const tagArmRecastDef*) pDef;
}

const tagDailyGoalAward* DBDataManager::getDailyGoalAward( int nID )
{
	char* pDef = get_dbdata(&nID,m_AllResData[DBRES_DAILYGOALAWARD]);
	if(pDef == NULL )
	{
		//LOG_INFO("DailyActiveInfo 中不存在ID=%d的定义", nID);
		return NULL;
	}
	return (const tagDailyGoalAward*) pDef;
}

const tagUnrealDef* DBDataManager::getUnrealDef(int layer)
{
	char* pDef = get_dbdata(&layer, m_AllResData[DBRES_UNREALDEF]);
	if(pDef == NULL )
	{
		LOG_INFO("UnrealDef 中不存在Layer=%d的定义", layer);
	}
	return (const tagUnrealDef*)pDef;
}

const tagNewAddAttrDef* DBDataManager::getNewAddAttrDef(int id)
{
	char* pDef = get_dbdata( &id, m_AllResData[DBRES_NEWADDATTRDEF] );
	if( pDef == NULL )
	{
		LOG_INFO("NewAddAttrDef 中不存在ID=%d的定义", id);
	}
	return ( const tagNewAddAttrDef* )pDef;
}

const tagAddAttrRandDef* DBDataManager::getAddAttrRandDef(int level, int important)
{
	tagAddAttrRandDef id;
	id.ArmLevel = level;
	id.Important = important;
	char* pDef = get_dbdata( &id, m_AllResData[DBRES_ADDATTRRANDDEF] );
	if( pDef == NULL )
	{
		LOG_INFO("AddAttrRandDef 中不存在ID=%d的定义", id);
	}
	return ( const tagAddAttrRandDef* )pDef;
}

const tagPetWashGeniusDef* DBDataManager::getPetWashGeniusDef(int important,int pinZhi)
{
	tagPetWashGeniusDef geniusDef;
	geniusDef.Important = important;
	geniusDef.PinZhi = pinZhi;
	char* pDef = get_dbdata(&geniusDef, m_AllResData[DBRES_PETWASHGENIUSDEF]);
	if(pDef == NULL )
	{
		LOG_INFO("PetWashGeniusDef 中不存在Important=%d,pinZhi=%d的定义", important,pinZhi);
	}
	return (const tagPetWashGeniusDef*)pDef;
}

const tagSparDef* DBDataManager::getSparDef( int id )
{
	char* pDef = get_dbdata(&id, m_AllResData[DBRES_SPARDEF]);
	if( pDef == NULL )
	{
		LOG_INFO("SparDef 中不存在id=%d的定义", id);
	}
	return (const tagSparDef*)pDef;
}

void DBDataManager::loadLevelUp( std::vector<tagLevelUP*>& verLevelUpDef )
{
	if( m_ResDBMode == RESDB_DB && m_pResDB == NULL )
	{
		return;
	}
	int num = m_AllResData[DBRES_LEVELUP].count;
	verLevelUpDef.resize(num);
	for( int i = 0; i < num; ++i )
	{
		verLevelUpDef[i] = ( tagLevelUP* )( m_AllResData[DBRES_LEVELUP].pbuffer + i * sizeof( tagLevelUP ) );
	}
}
const tagAchievementCategoryDef* DBDataManager::getAchievementCategoryDef( int categoryId )
{
	char* pDef = get_dbdata( &categoryId, m_AllResData[DBRES_ACHIEVEMENTCATEGORYDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO("AchievementCategoryDef 中不存在categoryId=%d的定义", categoryId);
	}
	return (const tagAchievementCategoryDef*)pDef;
}
const tagAchievementDef* DBDataManager::getAchievementDef( int achievementId, int categoryId )
{
	tagAchievementDef ID;
	ID.AchievementID = achievementId;
	ID.CategoryID = categoryId;
	char* pDef = get_dbdata( &ID, m_AllResData[DBRES_ACHIEVEMENTDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO("AchievementDef 中不存在achievementId=%d,categoryId=%d的定义", achievementId, categoryId);
	}
	return (const tagAchievementDef*)pDef;
}

const tagItemResultDesc* DBDataManager::getItemResultDescDef( int id )
{
	char* pDef = get_dbdata( &id, m_AllResData[DBRES_ITEMRESULTDESC] );
	if ( pDef == NULL )
	{
		LOG_INFO("ItemResultDesc 中不存在ResultId=%d的定义", id);
	}
	return (const tagItemResultDesc*)pDef;
}

const tagSJTaskDef* DBDataManager::getSJTaskDef( int id )
{
	char* pDef = get_dbdata( &id, m_AllResData[DBRES_SJTASKDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO("SJTaskDef 中不存在Id=%d的定义", id);
	}
	return (const tagSJTaskDef*)pDef;
}

const tagClanSpyTransDef* DBDataManager::getClanSpyTransDef( int id )
{
	char* pDef = get_dbdata( &id, m_AllResData[DBRES_CLANSPYTRANSDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO("ClanSpyTransDef 中不存在Id=%d的定义", id);
	}
	return (const tagClanSpyTransDef*)pDef;
}

void DBDataManager::loadMapTransDef( std::vector<tagMapTransDef*>& verMapTransDef )
{
	if( m_ResDBMode == RESDB_DB && m_pResDB == NULL )
	{
		return;
	}
	int num = m_AllResData[DBRES_MAPTRANSDEF].count;
	verMapTransDef.resize(num);
	for( int i = 0; i < num; ++i )
	{
		verMapTransDef[i] = ( tagMapTransDef* )( m_AllResData[DBRES_MAPTRANSDEF].pbuffer + i * sizeof( tagMapTransDef ) );
	}
}

const tagSubJectDef* DBDataManager::getSubjectDef( int id )
{
	char *pSubjectDef = get_dbdata(&id, m_AllResData[DBRES_SUBJECTDEF]);
	if(pSubjectDef == NULL)
	{
		LOG_INFO( "SUBJECTDEF 不存在id为 %d 的物品", id );
	}
	return (const tagSubJectDef*)pSubjectDef;
}

const tagStrongPointDef* DBDataManager::getStrongPointDef( int mapId )
{
	char *pStrongPointDef = get_dbdata(&mapId, m_AllResData[DBRES_STRONGPOINTDEF]);
	if (pStrongPointDef == NULL)
	{
		LOG_INFO( "STRONGPOINTDEF 不存在mapId为 %d 的定义", mapId );
	}
	return (const tagStrongPointDef*)pStrongPointDef;
}
void DBDataManager::loadClanSkill( std::vector<tagClanSkillDef*>& vecClanSkillDef )
{
	if( m_ResDBMode == RESDB_DB && m_pResDB == NULL )
	{
		return;
	}
	int num = m_AllResData[DBRES_CLANSKILLDEF].count;
	vecClanSkillDef.resize(num);
	for( int i = 0; i < num; ++i )
	{
		vecClanSkillDef[i] = ( tagClanSkillDef* )( m_AllResData[DBRES_CLANSKILLDEF].pbuffer + i * sizeof( tagClanSkillDef ) );
	}
}
static tagClanSkillDef noClanSkillDef;
const tagClanSkillDef &DBDataManager::getClanSkillDef(int id, int lv)
{
	tagClanSkillDef ID;
	ID.SkillID	= id;
	ID.Level	= lv;

	char *pclanSkillDef = get_dbdata(&ID, m_AllResData[DBRES_CLANSKILLDEF]);
	if( pclanSkillDef == NULL )
	{
		noClanSkillDef.SkillID = 0;
		return noClanSkillDef;
	}

	return *(tagClanSkillDef *)pclanSkillDef;
}
const tagBannerDef* DBDataManager::getBannerDef( int nType, int subType )
{
	tagBannerDef ID;
	ID.Type = nType;
	ID.SubType = subType;

	char *pBannerDef = get_dbdata( &ID, m_AllResData[DBRES_BANNERDEF]);
	if ( pBannerDef == NULL )
	{
		LOG_INFO( "BANNERDEF 不存在Type=%d,SubType=%d 的定义", nType, subType );
	}
	return ( const tagBannerDef* )pBannerDef;
}
void DBDataManager::loadBannerDef( std::vector<tagBannerDef*>& vecBannerDef )
{
	if ( m_ResDBMode == RESDB_DB && m_pResDB == NULL )
	{
		return;
	}
	int num = m_AllResData[DBRES_BANNERDEF].count;
	vecBannerDef.resize(num);
	for ( int i = 0; i < num; ++i )
	{
		vecBannerDef[i] = ( tagBannerDef* )( m_AllResData[DBRES_BANNERDEF].pbuffer + i * sizeof( tagBannerDef ) );
	}
}

const tagReviveDef*	DBDataManager::getReviveDef( int nLevel )
{
	char* pReviveDef = get_dbdata( &nLevel, m_AllResData[DBRES_REVIVEDEF] );
	if( pReviveDef == NULL )
	{
		LOG_INFO( "pReviveDef不存在level=%d", nLevel );
		return NULL;
	}

	return (tagReviveDef *)pReviveDef;
}
const tagClanPayDef* DBDataManager::getClanPayDef( int nKeepNum )
{
	char* pClanPayDef = get_dbdata( &nKeepNum, m_AllResData[DBRES_CLANPAYDEF] );
	if( pClanPayDef == NULL )
	{
		LOG_INFO( "ClanPayDef不存在KeepTimes=%d", nKeepNum );
		return NULL;
	}

	return (tagClanPayDef *)pClanPayDef;
}

const tagLevelExpDef* DBDataManager::getLevelExpDef( int nLevel )
{
	char* pLevelExpDef = get_dbdata( &nLevel, m_AllResData[DBRES_LEVELEXPDEF] );
	if( pLevelExpDef == NULL )
	{
		LOG_INFO( "pLevelExpDef不存在level=%d", nLevel );
		return NULL;
	}
	
	return (tagLevelExpDef *)pLevelExpDef;
}

const tagLevelBuffExpDef* DBDataManager::getLevelBuffExpDef( int nLevel )
{
	char* pLevelBuffExpDef = get_dbdata( &nLevel, m_AllResData[DBRES_LEVELEXPDEF] );
	if( pLevelBuffExpDef == NULL )
	{
		LOG_INFO( "pLevelBuffExpDef不存在level=%d", nLevel );
		return NULL;
	}

	return (tagLevelBuffExpDef *)pLevelBuffExpDef;
}

const tagOfflineExpParameter* DBDataManager::getOfflineExpParameter( int nType )
{
	char* pOfflineExpParam = get_dbdata( &nType, m_AllResData[DBRES_OFFLINEEXPPARAMETER] );
	if( pOfflineExpParam == NULL )
	{
		LOG_INFO( "pOfflineExpParam不存在type=%d", nType );
		return NULL;
	}

	return (tagOfflineExpParameter *)pOfflineExpParam;
}
const tagRidePinZhiDef* DBDataManager::getRidePinZhiDef( int nLevel )
{
	char* pRidePinZhiDef = get_dbdata( &nLevel, m_AllResData[DBRES_RIDEPINZHIDEF] );
	if ( pRidePinZhiDef == NULL )
	{
		LOG_INFO( "RidePinZhiDef不存在Level=%d", nLevel);
		return NULL;
	}
	return (tagRidePinZhiDef*)pRidePinZhiDef;
}
const tagMapAwardOreDef* DBDataManager::getMapAwardOreDef( int nMapID )
{
	char* pMapAwardOreDef = get_dbdata( &nMapID, m_AllResData[DBRES_MAPAWARDOREDEF] );
	if ( pMapAwardOreDef == NULL )
	{
		LOG_INFO( "MapAwardOreDef不存在MapID=%d", nMapID);
		return NULL;
	}
	return (tagMapAwardOreDef*)pMapAwardOreDef;
}

const tagRongluDef* DBDataManager::getRongLuDef( int id )
{
	char* pRongLuDef = get_dbdata( &id, m_AllResData[DBRES_RONGLUDEF] );
	if ( pRongLuDef == NULL )
	{
		LOG_INFO( "pRongLuDef不存在id=%d", id );
		return NULL;
	}
	return (tagRongluDef*)pRongLuDef;
}

void DBDataManager::loadRongluDef( std::vector<tagRongluDef*>& vecRongluDef )
{
	if( m_ResDBMode == RESDB_DB && m_pResDB == NULL )
	{
		return;
	}
	int num = m_AllResData[DBRES_RONGLUDEF].count;
	vecRongluDef.resize(num);
	for( int i = 0; i < num; ++i )
	{
		vecRongluDef[i] = ( tagRongluDef* )( m_AllResData[DBRES_RONGLUDEF].pbuffer + i * sizeof( tagRongluDef ) );
	}
}
const tagGeniusAttrDef* DBDataManager::getGeniusAttrDef( int nCareer,int nAttrId, int nLv )
{
	tagGeniusAttrDef def;
	def.Career = nCareer;
	def.AttrID = nAttrId;
	def.AttrLvl = nLv;
	char* pDef = get_dbdata( &def, m_AllResData[DBRES_GENIUSATTRDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "GeniusAttrDef不存在Career=%d,AttrID=%d,AttrLvl=%d", nCareer, nAttrId, nLv);
		return NULL;
	}
	return (tagGeniusAttrDef*)pDef;
}

const tagGeniusCareerDef* DBDataManager::getGeniusCareerDef( int nCareer )
{
	char* pDef = get_dbdata( &nCareer, m_AllResData[DBRES_GENIUSCAREERDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "GeniusCareerDef不存在Career=%d", nCareer );
		return NULL;
	}
	return (tagGeniusCareerDef*)pDef;
}

const tagGeniusAddDef* DBDataManager::getGeniusAddDef( int nType, int nCareer )
{
	tagGeniusAddDef def;
	def.Type = nType;
	def.Career = nCareer;
	char* pDef = get_dbdata( &def, m_AllResData[DBRES_GENIUSADDDEF] );
	if ( pDef == NULL )
	{
		//LOG_INFO( "GeniusAddDef不存在Type=%d,Career=%d", nType, nCareer);
		return NULL;
	}
	return (tagGeniusAddDef*)pDef;
}

const tagPetGeniusDef* DBDataManager::getPetGeniusDef( int nGenius )
{
	char* pDef = get_dbdata( &nGenius, m_AllResData[DBRES_PETGENIUSDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "PetGeniusDef不存在id=%d的定义", nGenius );
		return NULL;
	}
	return ( tagPetGeniusDef* ) pDef;
}

const tagBaoXiangDef* DBDataManager::getBaoXiangDef( int id )
{
	char* pDef = get_dbdata( &id, m_AllResData[DBRES_BAOXIANGDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "BaoXiangDef不存在id=%d", id );
		return NULL;
	}
	return (tagBaoXiangDef*)pDef;
}

const tagArmLevelUPDef* DBDataManager::getArmLevelUPDef( int id )
{
	char* pDef = get_dbdata( &id, m_AllResData[DBRES_ARMLEVELUPDEF] );
	if ( pDef == NULL )
	{
		return NULL;
	}
	return (tagArmLevelUPDef*)pDef;
}

const tagWorldBossDef* DBDataManager::getWorldBossDef( int id )
{
	char* pDef = get_dbdata( &id, m_AllResData[DBRES_WORLDBOSSDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "WorldBossDef不存在id=%d", id );
		return NULL;
	}
	return (tagWorldBossDef*)pDef;
}

void DBDataManager::loadWorldBossDef(std::vector<tagWorldBossDef*>& vecWorldBossDef)
{
	if( m_ResDBMode == RESDB_DB && m_pResDB == NULL )
	{
		return;
	}
	int num = m_AllResData[DBRES_WORLDBOSSDEF].count;
	vecWorldBossDef.resize(num);
	for( int i = 0; i < num; ++i )
	{
		vecWorldBossDef[i] = ( tagWorldBossDef* )( m_AllResData[DBRES_WORLDBOSSDEF].pbuffer + i * sizeof( tagWorldBossDef ) );
	}
}

const tagLevelFeng* DBDataManager::getLevelFeng( int lv )
{
	char* pDef = get_dbdata( &lv, m_AllResData[DBRES_LEVELFENG] );
	if ( pDef == NULL )
	{
		LOG_INFO( "LevelFeng不存在id=%d", lv );
		return NULL;
	}
	return (tagLevelFeng*)pDef;

}

const tagWorldCityFightAwardDef* DBDataManager::getWorldCityFightAwardDef( int index )
{
	char* pDef = get_dbdata( &index, m_AllResData[DBRES_WORLDCITYFIGHTAWARDDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "WorldCityFightAwardDef不存在id=%d", index );
		return NULL;
	}
	return (tagWorldCityFightAwardDef*)pDef;
}

const tagBusyDef* DBDataManager::getBusyDef( int nId )
{
	char* busyDef = get_dbdata( &nId, m_AllResData[DBRES_BUSYDEF] );
	if ( busyDef == NULL )
	{
		LOG_INFO( "BusyDef不存在id=%d的定义", nId );
		return NULL;
	}
	return ( tagBusyDef* )busyDef;
}

const tagBusyUseDef* DBDataManager::getBusyUseDef( int nNum )
{
	char* pDef = get_dbdata( &nNum, m_AllResData[DBRES_BUSYUSEDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "BusyUseDef不存在id=%d的定义", nNum );
		return NULL;
	}
	return ( tagBusyUseDef* ) pDef;
}

const tagHighWashUseDef* DBDataManager::getHighWashUseDef( int nNum )
{
	char* pDef = get_dbdata( &nNum, m_AllResData[DBRES_HIGHWASHUSEDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "HighWashUseDef不存在num=%d的定义", nNum );
		return NULL;
	}
	return ( const tagHighWashUseDef* ) pDef;
}

const tagStarStoneDef* DBDataManager::getStarStoneDef( int id )
{
	char* pDef = get_dbdata( &id,m_AllResData[DBRES_STARSTONEDEF] );
	if( pDef == NULL )
	{
		LOG_INFO("StarStoneDef 中不存在ID=%d的定义", id);
		return NULL;
	}
	return (const tagStarStoneDef*) pDef;
}

const tagStarStoneDef* DBDataManager::getStarStoneDefByIdx( int nIdx )
{
	if( nIdx < m_AllResData[DBRES_STARSTONEDEF].count )
	{
		return ( const tagStarStoneDef* )( m_AllResData[DBRES_STARSTONEDEF].pbuffer + nIdx * sizeof( tagStarStoneDef ) );
	}
	return NULL;
}

const tagStarFuncDef* DBDataManager::getStarFuncDefByIdx( int nIdx )
{
	if ( nIdx < m_AllResData[DBRES_STARFUNCDEF].count )
	{
		return ( const tagStarFuncDef* )( m_AllResData[DBRES_STARFUNCDEF].pbuffer + nIdx * sizeof( tagStarFuncDef ) );
	}
	return NULL;
}

const tagStarFuncDef* DBDataManager::getStarFuncDef( int nID )
{
	char* pDef = get_dbdata( &nID, m_AllResData[DBRES_STARFUNCDEF] );
	if( pDef == NULL )
	{
		return NULL;
	}
	return (const tagStarFuncDef*) pDef;
}

const tagMapOreDef* DBDataManager::getMapOreDef( int id )
{
	char* pDef = get_dbdata( &id, m_AllResData[DBRES_MAPOREDEF] );
	if ( pDef == NULL )
	{
		return NULL;
	}
	return (const tagMapOreDef*) pDef;
}
void DBDataManager::loadSpartaLevelDef( std::vector<tagSpartaLevelDef*>& vecSpartaLevelDef )
{
	if ( m_ResDBMode == RESDB_DB && m_pResDB == NULL )
	{
		return;
	}
	int num = m_AllResData[DBRES_SPARTALEVELDEF].count;
	vecSpartaLevelDef.resize(num);
	for( int i = 0; i < num; ++i )
	{
		vecSpartaLevelDef[i] = ( tagSpartaLevelDef* )( m_AllResData[DBRES_SPARTALEVELDEF].pbuffer + i * sizeof( tagSpartaLevelDef ) );
	}
}
void DBDataManager::loadBackLevelDef( std::vector<tagBackLevelDef>& vecBackLevelDef )
{
	if( m_ResDBMode == RESDB_DB )
	{
		ResTable *pnpctable = m_pResDB->GetTable("BackLevelDef");
		int num = pnpctable->FindRecord(NULL, 0, NULL);
		vecBackLevelDef.resize(num);
		if(num > 0) pnpctable->FindRecord(&vecBackLevelDef[0], num, NULL);
	}
	else
	{
		int num = m_AllResData[DBRES_BACKLEVELDEF].count;
		vecBackLevelDef.resize(num);
		for( int i = 0; i < num; i++ )
		{
			vecBackLevelDef[i] = *((tagBackLevelDef *)(m_AllResData[DBRES_BACKLEVELDEF].pbuffer + i * sizeof(tagBackLevelDef)));
		}
	}
}
const tagBackLevelDef* DBDataManager::getBackLevelDef( int id )
{
	char* pDef = get_dbdata( &id,m_AllResData[DBRES_BACKLEVELDEF] );
	if( pDef == NULL )
	{
		LOG_INFO("BackLevelDef 中不存在ID=%d的定义", id);
		return NULL;
	}
	return (const tagBackLevelDef*) pDef;
}

const tagSchoolXinDeDef* DBDataManager::getSchoolXinDeDef( int nLevel )
{
	char* pDef = get_dbdata( &nLevel, m_AllResData[DBRES_SCHOOLXINDEDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO("SchoolXinDeDef 中不存在Level=%d的定义", nLevel);
		return NULL;
	}
	return ( const tagSchoolXinDeDef* )pDef;
}

const tagXuKongPosDef* DBDataManager::getXuKongPosDef( int nCamp, int nPosIdx )
{
	tagXuKongPosDef def;
	def.Camp	 = nCamp;
	def.PosIndex = nPosIdx;
	char* pDef = get_dbdata( &def, m_AllResData[DBRES_XUKONGPOSDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "XuKongPosDef不存在Camp=%d,PosIndex=%d", nCamp, nPosIdx );
		return NULL;
	}
	return (const tagXuKongPosDef*)pDef;
}

const tagContCoverGift* DBDataManager::getContCoverGiftDef( int nDays )
{
	char* pDef = get_dbdata( &nDays,m_AllResData[DBRES_CONCOVERGIFTDEF] );
	if( pDef == NULL )
	{
		LOG_INFO("ContCoverGift 中不存在Days=%d的定义", nDays);
		return NULL;
	}
	return (const tagContCoverGift*) pDef;
}

void DBDataManager::loadConCoverGift( std::vector<tagContCoverGift>& vecConCoverGift )
{
	if( m_ResDBMode == RESDB_DB )
	{
		ResTable *pnpctable = m_pResDB->GetTable("ContCoverGift");
		int num = pnpctable->FindRecord(NULL, 0, NULL);
		vecConCoverGift.resize(num);
		if(num > 0) pnpctable->FindRecord(&vecConCoverGift[0], num, NULL);
	}
	else
	{
		int num = m_AllResData[DBRES_CONCOVERGIFTDEF].count;
		vecConCoverGift.resize(num);
		for( int i = 0; i < num; i++ )
		{
			vecConCoverGift[i] = *((tagContCoverGift *)(m_AllResData[DBRES_CONCOVERGIFTDEF].pbuffer + i * sizeof(tagContCoverGift)));
		}
	}
}

const tagNewRoleGift* DBDataManager::getNewRoleGiftDef( int nID )
{
	char* pDef = get_dbdata( &nID,m_AllResData[DBRES_NEWROLEGIFTDEF] );
	if( pDef == NULL )
	{
		LOG_INFO("NewRoleGift 中不存在ID=%d的定义", nID );
		return NULL;
	}
	return (const tagNewRoleGift*) pDef;
}

const tagCardBoxDef* DBDataManager::getCardBoxDef( int id )
{
	char* pDef = get_dbdata( &id, m_AllResData[DBRES_CARDBOXDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "CardBoxDef不存在id=%d", id );
		return NULL;
	}
	return (const tagCardBoxDef*)pDef;
}

const tagCardBoxGroupDef* DBDataManager::getCardBoxGroupDef( int nGroupId )
{
	char* pDef = get_dbdata( &nGroupId, m_AllResData[DBRES_CARDBOXGROUPDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "CardBoxGroup不存在groupId=%d", nGroupId );
		return NULL;
	}
	return (const tagCardBoxGroupDef*)pDef;
}

const tagBuChangDef* DBDataManager::getBuChangDef( int nDay )
{
	char* pDef = get_dbdata( &nDay, m_AllResData[DBRES_BUCHANGDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "BuChangDef不存在Day=%d", nDay );
		return NULL;
	}
	return (const tagBuChangDef*)pDef;
}

const tagArmBuChangDef* DBDataManager::getArmBuChangDef( int nLvl, int nCareer )
{
	tagArmBuChangDef ID;
	ID.Lvl = nLvl;
	ID.Career = nCareer;
	char* pDef = get_dbdata( &ID, m_AllResData[DBRES_ARMBUCHANGDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "ArmBuChangDef不存在Lvl=%d,Career=%d", nLvl, nCareer );
		return NULL;
	}
	return (const tagArmBuChangDef*)pDef;
}

const tagStoreGridDef* DBDataManager::getStoreGridDef( int nIdx )
{
	char* pDef = get_dbdata( &nIdx, m_AllResData[DBRES_STOREGRIDDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "StoreGridDef不存在Idx=%d", nIdx );
		return NULL;
	}
	return (const tagStoreGridDef*)pDef;
}

const tagArmStepUpDef* DBDataManager::getArmStepUpDef( int nLv, int nImp, int nPos )
{
	tagArmStepUpDef ID;
	ID.Lvl = nLv;
	ID.Imp = nImp;
	ID.WPos = nPos;
	char* pDef = get_dbdata( &ID, m_AllResData[DBRES_ARMSTEPUPDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "ArmStepUpDef不存在Lvl=%d,Imp=%d,WPos=%d", nLv, nImp, nPos );
		return NULL;
	}
	return (const tagArmStepUpDef*)pDef;
}

const tagRongYanJuShouDef* DBDataManager::getRongYanJuShouDef( int nIdx )
{
	char* pDef = get_dbdata( &nIdx, m_AllResData[DBRES_RONGYANJUSHOUDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "RongYanJuShouDef不存在Idx=%d", nIdx );
		return NULL;
	}
	return (const tagRongYanJuShouDef*)pDef;
}

const tagRongYanMonDynAttrDef* DBDataManager::getRongYanMonAttrDef( int nLv )
{
	char* pDef = get_dbdata( &nLv, m_AllResData[DBRES_RONGYANMONATTRDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "RongYanMonDynAttrDef不存在Idx=%d", nLv );
		return NULL;
	}
	return (const tagRongYanMonDynAttrDef*)pDef;
}

const tagArmGradeValDef* DBDataManager::getArmGradeValDef( int nImp )
{
	char* pDef = get_dbdata( &nImp, m_AllResData[DBRES_ARMGRADEVALDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "ArmGradeValDef不存在Imp=%d", nImp );
		return NULL;
	}
	return (const tagArmGradeValDef*)pDef;
}

const tagArmReSetDef* DBDataManager::getArmResetDef( int nLv )
{
	char* pDef = get_dbdata( &nLv, m_AllResData[DBRES_ARMRESETDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "tagArmReSetDef不存在Lvl=%d", nLv );
		return NULL;
	}
	return (const tagArmReSetDef*)pDef;
}

const tagGeniusMdfDef* DBDataManager::getGeniusMdfDef( int nType )
{
	char* pDef = get_dbdata( &nType, m_AllResData[DBRES_GENIUSMDFDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "tagGeniusMdfDef不存在type=%d", nType );
		return NULL;
	}
	return (const tagGeniusMdfDef*)pDef;
}

const tagArmSkillNumDef* DBDataManager::getArmSkillNumDef( int nLv, int nImp, int nPos )
{
	tagArmSkillNumDef ID;
	ID.Lvl = nLv;
	ID.Imp = nImp;
	ID.WPos = nPos;
	char* pDef = get_dbdata( &ID, m_AllResData[DBRES_ARMSKILLNUMDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "tagArmSkillNumDef不存在Lvl=%d,Imp=%d,WPos=%d", nLv, nImp, nPos );
		return NULL;
	}
	return (const tagArmSkillNumDef*)pDef;
}
const tagArmSkillAttrDef* DBDataManager::getArmSkillAttrDef( int nID )
{
	char* pDef = get_dbdata( &nID, m_AllResData[DBRES_ARMSKILLATTRDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "tagArmSkillAttrDef不存在ID=%d", nID );
		return NULL;
	}
	return (const tagArmSkillAttrDef*)pDef;
}

const tagPetMoneyDef* DBDataManager::getPetMoneyDef( int nImp )
{
	char* pDef = get_dbdata( &nImp, m_AllResData[DBRES_PETMONEYDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "PetMoneyDef不存在Imp=%d", nImp );
		return NULL;
	}
	return (const tagPetMoneyDef*)pDef;
}

const tagPetImpUpDef* DBDataManager::getPetImpUpDef( int nImp )
{
	char* pDef = get_dbdata( &nImp, m_AllResData[DBRES_PETIMPUPDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "PetImpUpDef不存在Imp=%d", nImp );
		return NULL;
	}
	return (const tagPetImpUpDef*)pDef;
}
const tagPetImpUpAttrDef* DBDataManager::getPetImpUpAttrDef(int nImp,int nType )
{
	tagPetImpUpAttrDef ImpUpAttrDef;
	ImpUpAttrDef.Imp	= nImp;
	ImpUpAttrDef.Type	= nType;
	char* pDef = get_dbdata(&ImpUpAttrDef, m_AllResData[DBRES_PETIMPUPATTRDEF]);
	if(pDef == NULL )
	{
		LOG_INFO("PetImpUpAttrDef 中不存在Imp=%d,Type=%d的定义", nImp,nType);
	}
	return (const tagPetImpUpAttrDef*)pDef;
}
const tagGemPagDef* DBDataManager::getGemPagDef( int nId )
{
	char* pDef = get_dbdata( &nId, m_AllResData[DBRES_GEMPAGDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "GemPagDef不存在Id=%d", nId );
		return NULL;
	}
	return (const tagGemPagDef*)pDef;
}

const tagArmTeshuDef* DBDataManager::getArmTeshuDef( int nId )
{
	char* pDef = get_dbdata( &nId, m_AllResData[DBRES_ARMTESHUDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "ArmTeshuDef不存在Id=%d", nId );
		return NULL;
	}
	return (const tagArmTeshuDef*)pDef;
}
const tagSpanStrongDef*	DBDataManager::getSpanStrongDef( int nId )
{
	char* pDef = get_dbdata( &nId, m_AllResData[DBRES_SPANSTRONGDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "SpanStrongDef不存在Id=%d", nId );
		return NULL;
	}
	return (const tagSpanStrongDef*)pDef;
}

const tagHomeLvlDef* DBDataManager::getHomeLvlDef( int nLvl )
{
	char* pDef = get_dbdata( &nLvl, m_AllResData[DBRES_HOMELVLDEF] );
	if ( pDef == NULL ) {
		LOG_INFO( "HomeLvlDef不存在Lvl=%d", nLvl );
		return NULL;
	}
	return (const tagHomeLvlDef*)pDef;
}

const tagHomeMonLvlDef* DBDataManager::getHomeMonLvlDef( int nLvl )
{
	char* pDef = get_dbdata( &nLvl, m_AllResData[DBRES_HOMEMONLVLDEF] );
	if ( pDef == NULL ) {
		LOG_INFO( "HomeMonLvlDef不存在Lvl=%d", nLvl );
		return NULL;
	}
	return (const tagHomeMonLvlDef*)pDef;
}

const tagHomeFarmDef* DBDataManager::getHomeFarmDef( int nLvl )
{
	char* pDef = get_dbdata( &nLvl, m_AllResData[DBRES_HOMEFARMDEF] );
	if ( pDef == NULL ) {
		LOG_INFO( "HomeFarmDef不存在Lvl=%d", nLvl );
		return NULL;
	}
	return (const tagHomeFarmDef*)pDef;
}

const tagHomeMonSegDef* DBDataManager::getHomeMonSegDef( int nLvl )
{
	char* pDef = get_dbdata( &nLvl, m_AllResData[DBRES_HOMEMONSEGDEF] );
	if ( pDef == NULL ) {
		LOG_INFO( "HomeMonSegDef不存在Lvl=%d", nLvl );
		return NULL;
	}
	return (const tagHomeMonSegDef*)pDef;
}

const tagArmSkillRecastDef* DBDataManager::getArmSkillRecastDef( int nLv )
{
	char* pDef = get_dbdata( &nLv, m_AllResData[DBRES_ARMSKILLRECASTDEF] );
	if ( pDef == NULL )
	{
		LOG_INFO( "ArmSkillRecastDef不存在Lv=%d", nLv );
		return NULL;
	}
	return (const tagArmSkillRecastDef*)pDef;
}
