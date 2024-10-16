
#ifndef __GameDBData_H__
#define __GameDBData_H__

#include "OgreResDB.h"
#include "OgreSingleton.h"
#include "GameEngineTypes.h"
#include "GameResDBTypes.h"
#include <hash_map>
#include <map>

enum
{
	DBRES_SKILLDEF = 0,
	DBRES_MONSTERDEF,
	DBRES_NPCDISPLAYINFO,
	DBRES_MAPDEF,
	DBRES_STATUSDEF,
	DBRES_STATUSOVERLAYDEF,	
	DBRES_ITEMDEF,
	DBRES_ITEMDISPLAYINFO,
	DBRES_LEVELUP,
	DBRES_MAPNPC,
	DBRES_ARMLEVELHURTDEF,
	DBRES_TASKDEF,
	DBRES_TASKRECYCLEDEF,
	DBRES_TASKLIMITGROUP,
	DBRES_NPCINFO,
	DBRES_ADDATTRDEF,
	DBRES_QIYUEDEF,
	DBRES_DIATHESIS,
	DBRES_GEMDEF,
	DBRES_SCENEAREADEF,
	DBRES_MONSTERSPEECHDEF,
	DBRES_ARMADDATTRDEF,
	DBRES_SUITSET,
	DBRES_ARMGRADEDEF,
	DBRES_WORLDMAPSEARCHDEF,
	DBRES_CLANLEVELDEF,
	DBRES_CLANCITYBUILDINGLEVELDEF,
	DBRES_CLANCITYLEVELDEF,
	DBRES_DESIGNATIONDEF,
    DBRES_RESPACTPARTYDEF,
    DBRES_RESPACTPHASEDEF,
    DBRES_FAIRYDEF,
	DBRES_RIDEDEF,
	DBRES_PWORLDDEF,
	DBRES_PETDEF,
	DBRES_PETSKILLGRIDDEF,
	DBRES_ADDATTRBASEDEF,
	DBRES_PETSTRENGTHENDEF,
	DBRES_PETLEVELDEF,
    DBRES_PETTRAITDEF,
    DBRES_ONEDAYGIFTDEF,
    DBRES_LVGIFTDEF,
    DBRES_FIXEDADDATTR,
	DBRES_BCT,
	DBRES_RNDAWD,
	DBRES_ACTIVEHELPER,
	DBRES_SECONDARYSKILL,
	DBRES_RECIPE,
	DBRES_CANGBAOTU,
	DBRES_CLANTECHDEF,
	DBRES_CLANPRODUCTDEF,
	DBRES_MONMACHINEDEF,
	DBRES_CONTONLINEGIFTDEF,
	DBRES_BATTLEDEF,
	DBRES_CLAN_BUILDINGDISPLAYINFO,
	DBRES_FAIRYSTARSTUFFDEF,
	DBRES_FAIRYSTARDEF,
	DBRES_ONLINEGIFTGROUP,
	DBRES_ARMRANDDEF,
	DBRES_BIRTHITEMDEF,
	DBRES_GAMEEVENTDEF,
	DBRES_DIFFPWORLDDEF,
	DBRES_WORLDCITYBUILDDEF,
	DBRES_CITYPRODUCTDEF,
	DBRES_ARMFENJIDEF,
	DBRES_STARVIPDEF,
	DBRES_STARSTONEDEF,
	DBRES_STARFUNCDEF,
	DBRES_MEDIAGIFTDEF,
	DBRES_LTRGIFTDEF,
	DBRES_GEMNEWDEF,
	DBEES_GEMLEVELSTARDEF,
	DBRES_GEMATTRSTARDEF,
	DBRES_RUNEADDATTRDEF,
	DBRES_USEDEF,
	DBRES_ARMRECASTDEF,
	DBRES_ARMSTARDEF,
	DBRES_ARMHIDEATTRDEF,
	DBRES_DAILYGOALAWARD,
	DBRES_UNREALDEF,
	DBRES_NEWADDATTRDEF,
	DBRES_ADDATTRRANDDEF,
	DBRES_PETWASHGENIUSDEF,
	DBRES_SPARDEF,
	DBRES_ACHIEVEMENTCATEGORYDEF,
	DBRES_ACHIEVEMENTDEF,
	DBRES_ITEMRESULTDESC,
	DBRES_MAPTRANSDEF,
	DBRES_SJTASKDEF,
	DBRES_CLANSPYAWARDDROPDEF,
	DBRES_CLANSPYTRANSDEF,
	DBRES_SUBJECTDEF,
	DBRES_STRONGPOINTDEF,
	DBRES_CLANSKILLDEF,
	DBRES_BANNERDEF,
	DBRES_REVIVEDEF,
	DBRES_CLANPAYDEF,
	DBRES_LEVELEXPDEF,
	DBRES_LEVELBUFFEXPDEF,
	DBRES_OFFLINEEXPPARAMETER,
	DBRES_RIDEPINZHIDEF,
	DBRES_MAPAWARDOREDEF,
	DBRES_RONGLUDEF,
	DBRES_GENIUSCAREERDEF,
	DBRES_GENIUSATTRDEF,
	DBRES_GENIUSADDDEF,
	DBRES_BAOXIANGDEF,

	DBRES_ARMLEVELUPDEF,
	DBRES_WORLDBOSSDEF,
	DBRES_LEVELFENG,
	DBRES_WORLDCITYFIGHTAWARDDEF,
	DBRES_BUSYDEF,
	DBRES_BUSYUSEDEF,
	DBRES_HIGHWASHUSEDEF,
	DBRES_MAPOREDEF,
	DBRES_PETGENIUSDEF,
	DBRES_SPARTALEVELDEF,
	DBRES_BACKLEVELDEF,
	DBRES_SCHOOLLEVELDEF,
	DBRES_SCHOOLXINDEDEF,
	DBRES_XUKONGPOSDEF,
	DBRES_CONCOVERGIFTDEF,
	DBRES_NEWROLEGIFTDEF,
	DBRES_CARDBOXDEF,
	DBRES_CARDBOXGROUPDEF,
	DBRES_ARMBUCHANGDEF,
	DBRES_BUCHANGDEF,
	DBRES_STOREGRIDDEF,
	DBRES_ARMSTEPUPDEF,
	DBRES_RONGYANJUSHOUDEF,
	DBRES_RONGYANMONATTRDEF,
	DBRES_ARMGRADEVALDEF,
	DBRES_ARMRESETDEF,
	DBRES_GENIUSMDFDEF,
	DBRES_ARMSKILLNUMDEF,
	DBRES_ARMSKILLATTRDEF,
	DBRES_PETMONEYDEF,
	DBRES_PETIMPUPDEF,
	DBRES_PETIMPUPATTRDEF,
	DBRES_GEMPAGDEF,
	DBRES_ARMTESHUDEF,
	DBRES_SPANSTRONGDEF,
	DBRES_HOMELVLDEF,
	DBRES_HOMEMONLVLDEF,
	DBRES_HOMEFARMDEF,
	DBRES_HOMEMONSEGDEF,
	DBRES_ARMSKILLRECASTDEF,
	DBRES_MAX_NUM
};

struct DBDataTable
{
	char *pbuffer;
	int count;
	int unitsize;
};

class DBDataManager : public Ogre::Singleton<DBDataManager>
{
public:
	DBDataManager();
	~DBDataManager();

	bool LoadData(const GameConfig &cfg );
	bool OfflineLoadData(const GameConfig &cfg );
	void reloadDB();

	Ogre::ResDB *getDB(){ return m_pResDB; }

	const tagSkillDef&			GetSkillDef(int id, int lv);
	const tagNpcDisplayInfo&	GetNpcDisplayInfo(int id);
	const tagMonsterDef&		getMonsterDef(int id);
	const tagMonsterDef*		getMonsterDefPtr(int id);
	const tagMapDef&			getMapDef(int id);
	const tagStatusDef&			getStatusDef(int id, int lv);
	const tagStatusOverlayDef&	getStatusOverlayDef(int id, int lv,int overlay);
	const tagItemDef&			getItemDef( unsigned int id );
	const tagArmAddAttrDef&		getArmAddAttrDef( unsigned int id );
	const tagSuitSet &			getSuitSet( unsigned int id );
	const tagProcEvent&			getProcEvent( int id, int procId );
	const tagLevelUP&			getLevelUP( int lv );
	const tagItemDisplayInfo&	getItemDisplayInfo(int id);
	const tagTaskDef&			getTaskDef(int id);
	const tagTaskRecycleDef&	getTaskRecycleDef(int id);
	const tagTaskLimitGroup&	getTaskLimitGroup(int id);
	const tagNpcInfoDef&		getNpcInfoDef(int id);
	const tagAddAttrDef&		getAddAttrDef(int id);
	const tagQiYueDef&			getQiYueDef(int id);
	const tagDiathesisDef&		getDiathesisDef(int id, int lv);
	const tagGemDef&			getGemDef(int id);
	const tagGemNewDef&			getGemNewDef(int id);
	const tagArmGradeDef&		getArmGradeDef( int nArmType, int nArmLevel );
	const tagArmStarDef&		getArmStarDef( int nStar );
	const tagArmHideAttrDef&	getArmHideAttrDef( int nCareer, int nArmStarLevel );

	//const tagSceneAreaDef&	getSceneAreaDef(int nMapId, int nListIndex );
	const tagMonsterSpeechDef*	getMonsterSpeechDef( int id );
	const tagClanLevelDef*		getClanLevelDef( int nLevel );
	const tagClanCityLevelDef*	getClanCityLevelDef( int nLevel );
	const tagClanCityBuildingLevelDef*	getClanCityBuildingLevelDef( int nType, int nLevel );
	const tagDesignationDef& 	getDesignationDef( int nId );
    const tagRespactPartyDef&   getRespactPartyDef( int nId );
    const tagRespactPhaseDef&   getRespactPhaseDef( int nId );
    const tagFairyDef*          getFairyDef( int nId, int nLv );
	const tagRideDef*			getRideDef( int nId, int nLv );
	const tagPworldDef*			getPworldDef( int nId );
	const tagPetDef&			getPetDef( int nId );
	const tagPetSkillGridDef&	getPetSkillGridDef( int nGridIndex );
	const tagAddAttrBaseDef*	getAddAttrBaseDef( int nLv );
	const tagPetStrengthenDef*	getPetStrengthenDef( int nLv );
	const tagPetLevelDef*		getPetLevelDef( int nLv );
	const tagPetTraitDef*		getPetTraitDef( int nTraidId );
	const tagPetTraitDef*		getPetTraitDef( int nTraidId,int nStar );
	const tagFixedAddAttr*      getFixedAddAttr( int id );
	const tagBCTDef*			getBCTDef(int id);
	const tagRndAwdDef*			getRndAwdDef(int id);
	const tagActiveHelper*		getActiveHelper(int id);
	const tagSecondarySkillDef*	getSecondarySkillDef(int nType,int nLevel);
	const tagRecipeDef*			getRecipeDef(int nType,int id);
	const tagWorldMapSearchDef*	getWorldMapSearchDef( int nFromMapID );
	const tagCangBaoTuDef*		getCangBaoTuDefDef( int nMapID, int nLevel, int nItemID );
	const tagClanTechDef*		getClanTechDef( int nCat,int nLevel );
	const tagClanProductDef*	getClanProductDef( int nID );
	const tagClanProductDef*	getClanProductDefByIndex( int nIndex );
	const tagMonMachineDef*		getMonMachineDef( int nMonsterID );
	const tagClanBuildingDisplayInfo* getBuildingDisplayInfo( int nID );
	const tagFairyStarStuffDef*	getFairyStarStuffDef( int itemID );
	const tagFairyStarDef*		getFairyStarDef( int nFairyId,int nStar );
	const tagOnlineGiftGroup*	getOnlineGiftGroup( int nID );
	const tagArmRandDef*		getArmRandDef( int nLevel,int nType );
	const tagGameEventDef*		getGameEventDef(int eventId);
	const tagDiffPworldDef*		getDiffPworldDef(int id);
	const tagBattleDef*			getBattleDef( int battleId );
	const tagWorldCityBuildDef*	getWorldCityBuildDef(int id);
	const tagCityProductDef*	getCityProductDef(int id);
	const tagArmFenJieDef*		getArmFenJieDef( int nLevel, int nImportant );
	const tagMediaGiftDef*		getMediaGiftDef( int id );
	const tagLtrGiftDef*		getLtrGiftDef( int id );
	const tagGemLevelStarDef*	getGemLevelStarDef( int nLevel, int nStar );
	const tagGemAttrStarDef*	getGemAttrStarDef( int nAttrID, int nLevel, int nStar );
	const tagRuneAddAttrDef*	getRuneAddAttrDef(int id);
	const tagUseDef*			getUseDef( int nID );
	const tagArmRecastDef*		getArmRecastDef( int nID );
	const tagDailyGoalAward*	getDailyGoalAward( int nID );
	const tagUnrealDef*			getUnrealDef(int id);
	const tagNewAddAttrDef*		getNewAddAttrDef(int id);
	const tagAddAttrRandDef*	getAddAttrRandDef(int level, int important);
	const tagPetWashGeniusDef*	getPetWashGeniusDef(int important, int pinZhi);
	const tagSparDef*			getSparDef( int id );
	const tagAchievementCategoryDef* getAchievementCategoryDef( int categoryId );
	const tagAchievementDef*	getAchievementDef( int achievementId, int categoryId );
	const tagItemResultDesc*	getItemResultDescDef( int id );
	const tagSJTaskDef*			getSJTaskDef( int id );
	const tagClanSpyTransDef*	getClanSpyTransDef( int id );
	const tagSubJectDef*		getSubjectDef( int id );
	const tagStrongPointDef*	getStrongPointDef( int mapId );
	const tagClanSkillDef&		getClanSkillDef(int id, int lv);
	const tagBannerDef*			getBannerDef( int nType, int subType );
	const tagReviveDef*			getReviveDef( int nLevel );
	const tagClanPayDef*		getClanPayDef( int nKeepNum );
	const tagLevelExpDef*		getLevelExpDef( int nLevel );
	const tagLevelBuffExpDef*	getLevelBuffExpDef( int nLevel );
	const tagOfflineExpParameter*	getOfflineExpParameter( int nType );
	const tagRidePinZhiDef*		getRidePinZhiDef( int nLevel );
	const tagMapAwardOreDef*	getMapAwardOreDef( int nMapID );
	const tagRongluDef*			getRongLuDef( int id );
	const tagGeniusAttrDef*		getGeniusAttrDef( int nCareer,int nAttrId, int nLv );
	const tagGeniusCareerDef*	getGeniusCareerDef( int nCareer );
	const tagGeniusAddDef*		getGeniusAddDef( int nType, int nCareer );
	const tagBaoXiangDef*		getBaoXiangDef( int id );
	const tagArmLevelUPDef*		getArmLevelUPDef( int id );
	const tagWorldBossDef*		getWorldBossDef( int id );
	const tagLevelFeng*			getLevelFeng( int lv );
	const tagWorldCityFightAwardDef*	getWorldCityFightAwardDef( int index );
	const tagBusyDef*			getBusyDef( int nId );
	const tagBusyUseDef*		getBusyUseDef( int nNum );
	const tagHighWashUseDef*	getHighWashUseDef( int nNum );
	const tagStarStoneDef*		getStarStoneDef( int id );
	const tagStarStoneDef*		getStarStoneDefByIdx( int nIdx );
	const tagStarFuncDef*		getStarFuncDef( int nID );
	const tagMapOreDef*			getMapOreDef( int id );
	const tagStarFuncDef*		getStarFuncDefByIdx( int nIdx );
	const tagPetGeniusDef*		getPetGeniusDef( int nGenius );
	const tagBackLevelDef*		getBackLevelDef( int id );
	const tagSchoolXinDeDef*	getSchoolXinDeDef( int nLevel );
	const tagXuKongPosDef*		getXuKongPosDef( int nCamp, int nPosIdx );
	const tagContCoverGift*		getContCoverGiftDef( int nDays );
	const tagNewRoleGift*		getNewRoleGiftDef( int nID );
	const tagCardBoxDef*		getCardBoxDef( int id );
	const tagCardBoxGroupDef*	getCardBoxGroupDef( int nGroupId );
	const tagBuChangDef*		getBuChangDef( int nDay );
	const tagArmBuChangDef*		getArmBuChangDef( int nLvl, int nCareer );
	const tagStoreGridDef*		getStoreGridDef( int nIdx );
	const tagArmStepUpDef*		getArmStepUpDef( int nLv, int nImp, int nPos );
	const tagRongYanJuShouDef*  getRongYanJuShouDef( int nIdx );
	const tagRongYanMonDynAttrDef*  getRongYanMonAttrDef( int nLv );
	const tagArmGradeValDef*	getArmGradeValDef( int nImp );
	const tagArmReSetDef*		getArmResetDef( int nLv );
	const tagGeniusMdfDef*		getGeniusMdfDef( int nType );
	const tagArmSkillNumDef*	getArmSkillNumDef( int nLv, int nImp, int nPos );
	const tagArmSkillAttrDef*	getArmSkillAttrDef( int nID );
	const tagPetMoneyDef*		getPetMoneyDef( int nImp );
	const tagPetImpUpDef*		getPetImpUpDef( int nImp );
	const tagPetImpUpAttrDef*	getPetImpUpAttrDef(int nImp,int nType );
	const tagGemPagDef*			getGemPagDef( int nId );
	const tagArmTeshuDef*		getArmTeshuDef( int nId );
	const tagSpanStrongDef*		getSpanStrongDef( int nId );
	const tagHomeLvlDef*			getHomeLvlDef( int nLvl );
	const tagHomeMonLvlDef*	getHomeMonLvlDef( int nLvl );
	const tagHomeFarmDef*		getHomeFarmDef( int nLvl );
	const tagHomeMonSegDef*	getHomeMonSegDef( int nLvl );
	const tagArmSkillRecastDef* getArmSkillRecastDef( int nLv );


	void loadNpcDisplayInfo(std::vector<NPCDISPLAYINFO>& npcinfo);
	void loadMapMonsters(int mapid, std::vector<MAPMONSTER>& monsters);
	bool saveMapMonsters(int mapid, const std::vector<MAPMONSTER>& monsters);
	void loadMapProcEvent(int mapid, std::vector<PROCEVENT>& procevents);
	bool saveMapProcEvent(int mapid, const std::vector<PROCEVENT>& procevents);
	void saveMapDef(const MAPDEF& mapdef);
	void loadMapNpc(std::vector<MAPNPC>& mapnpcinfo);
	void loadSkillDef(std::vector<tagSkillDef>& skilldef);
	void loadArmHurtDef( std::vector<ARMLEVELHURTDEF>& HurtDef );
	void loadTaskDef( std::vector<tagTaskDef*>& taskdef );
	void loadSceneAreaDef( std::vector<tagSceneAreaDef>& sceneAreadef );
	void loadMapDef( std::vector<tagMapDef>& mapDef );
	void loadWorldMapSearchDef( std::vector<tagWorldMapSearchDef>& mapSearchVec  );
	void loadSuitSet( std::map<int,std::vector<int>>& mapSuitSet );
	void loadDesignation( std::map<std::string,std::vector<tagDesignationDef*>>& mapDesignationDef, std::vector<tagDesignationDef*>& vectDesignationDef );
    void loadRespactphaseDef( std::vector<tagRespactPhaseDef*>& vectRespactPhaseDef );
	void loadRespactPartyDef( std::map<int,std::vector<tagRespactPartyDef*>>& mapRespactPartyDef );
	void loadCanFittingItem( std::map<int,std::vector<int>>& m_mapCanFittingItem );
	void loadDiathesisDef( std::vector<tagDiathesisDef>& diathesisdef );
	//void loadOneDayGiftDef( std::vector<tagOneDayGift*>& vectOneDayGiftDef );
	void loadLVGiftDef( std::vector<tagLVGift*>& vectLVGiftDef );

	void loadActiveHelper(std::vector<tagActiveHelper*>& activeHelper);
	void loadCangBaoTuDef(std::vector<tagCangBaoTuDef*>& vecCangBaoTuDef );
	void loadPetStrengthenDef(std::vector<tagPetStrengthenDef*>& vecPetStrengthenDef );
	void loadGemDef( std::vector<tagGemDef*>& vecGemDef );
	void loadGemNewDef( std::vector<tagGemNewDef*>& vecGemNewDef );
	void loadBattleDef( std::vector<tagBattleDef*>& vecBattleDef );
	void loadBirthItemDef( std::vector<tagBirthItemDef*>& vecBirthItemDef );
	void loadRuneAddAttrDef(std::vector<tagRuneAddAttrDef>& verRuneAddAttrDef);
	void loadLevelUp( std::vector<tagLevelUP*>& verLevelUpDef );
	void loadMapTransDef( std::vector<tagMapTransDef*>& verMapTransDef );
	void loadClanSkill( std::vector<tagClanSkillDef*>& vecClanSkillDef );
	void loadBannerDef( std::vector<tagBannerDef*>& vecBannerDef );
	void loadRongluDef( std::vector<tagRongluDef*>& vecRongluDef );
	void loadWorldBossDef( std::vector<tagWorldBossDef*>& vecWorldBossDef );
	void loadSpartaLevelDef( std::vector<tagSpartaLevelDef*>& vecSpartaLevelDef );
	void loadBackLevelDef( std::vector<tagBackLevelDef>& vecBackLevelDef );
	void loadConCoverGift( std::vector<tagContCoverGift>& vecConCoverGift );
	const tagContOnlineGiftDef* getContOnlineGiftDef(int nID);
	int getClanProductDefNum();
	int getResTableDefNum( unsigned int type );
	int getOneDayGiftNum();
	int getStarFuncNum();
	tagOneDayGift* getOneDayGift( int idx );

protected:
	int m_ResDBMode;
	Ogre::ResDB *m_pResDB;
	Ogre::String m_DBFileDir;

	/*
	Ogre::ResTable *m_pSkillTable;
	Ogre::ResTable *m_pSkillDefTable;
	Ogre::ResTable *m_pTaskTable;
	Ogre::ResTable *m_pMonsterTable;
	Ogre::ResTable *m_pNpcDisplayTable;
	Ogre::ResTable *m_pMapDefTable;
	Ogre::ResTable *m_pProcEventTable;
	Ogre::ResTable *m_pMapMonsterTable;
	Ogre::ResTable *m_pMapProcTable;
	Ogre::ResTable *m_pFittingsTable;
	Ogre::ResTable *m_pStatusTable;
	Ogre::ResTable *m_pItemDefTable;
	Ogre::ResTable *m_pLevelUpTable;

	stdext::hash_map<int, tagSkillDef>m_SkillsDef;
	stdext::hash_map<int, tagNpcDisplayInfo>m_NpcDisplayInfo;
	stdext::hash_map<int, tagMonsterDef>m_Monsters;
	stdext::hash_map<int, tagMapDef>m_MapDefs;
	stdext::hash_map<int, tagFittingsDef>m_FittingsInfo;
	stdext::hash_map<int, tagStatusDef>m_StatusInfo;
	stdext::hash_map<int, tagItemDef>m_ItemDef;
	stdext::hash_map<int, tagItemDisplayInfo>m_ItemDisplayInfo;
	stdext::hash_map<int, tagProcEvent>m_ProcEvent;
	stdext::hash_map<int, tagLevelUP>m_LevelUp;
	*/

	DBDataTable m_AllResData[DBRES_MAX_NUM];

	template<class T>
	const T &GetInfo(int id, stdext::hash_map<int, T>&infomap, Ogre::ResTable *prestable);
	template<class T>
	const T &GetInfo(int id, const void* pObj, stdext::hash_map<int, T>&infomap, Ogre::ResTable *prestable);
	bool loadFromDBFile(const char *filename, int unitsize, DBDataTable &data);
	bool loadFromRealDB(const char *tablename, int unitsize, DBDataTable &data);
};


#endif
