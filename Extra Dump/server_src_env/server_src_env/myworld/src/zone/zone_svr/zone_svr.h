#ifndef ZONE_SVR_H
#define ZONE_SVR_H

#define PAL_NO_WIN_TYPES

#include "zone_svr_conf_desc.h"
#include "pal/pal.h"
#include "comm_misc.h"
#include "comm/shtable.h"
#include "cs_net.h"
#include "tlog/tlog.h"
#include "comm/tmempool.h"
#include "tapp/tapp.h"
#include "tbus/tbus.h"
#include "apps/tormapi/torm_protocol.h"
#include "apps/tormapi/torm_protocol_api.h"
#include "apps/tormapi/torm_error.h"
#include "entity.h"

#include "black_white.h"
#include "resdb_meta.h"
#include "og_script.h"
#include <math.h>

#define SAFELOCK_LEVEL 1

#define INIT_TASK_PACKAGE 32
#define INIT_SUNDRIES_PACKAGE 32
#define INIT_STASH_PACKAGE 36
#define INIT_STUFF_PACKAGE 0

#define INIT_ET_STASH_PKG 0
#define INIT_ET_SUND_PKG 2

#define INIT_SPAR_PKG 0

#ifdef _OPERATION_ENV
#define MAX_PLAYER 4560
#define MAX_PLAYER_SOFT 4540
#else
#define MAX_PLAYER 520
#define MAX_PLAYER_SOFT 500
#endif
#define PLAYER_BUCKET  MAX_PLAYER*5
#define MAX_SUBJECT_TYPE_NUM 10 		// 最多10个类型
#define MAX_SUBJECT_TYPE_COUNT 500 	//每种类型最多300个
#define SUBJECT_MAX_NUM 24

#define MAX_CAREER 4
//#define MAX_FENGYIN_ARM 1

#define PLAYER_SHM_MAGIC 0x63a875e4

/*if player struct change, increase PLAYER_SHM_VER*/
#define PLAYER_SHM_VER 0x5030

/*if zoneobj struct change, increase OBJ_SHM_VER*/
#define OBJ_SHM_VER 0x7032

/*if mapobj struct change, increase MAP_SHM_VER*/
#define MAP_SHM_VER 0x9024

char szWatchCSPkg[CS_MAX_BODY - sizeof(CSPKGBODY) + 1];
//char szWatchRoleData[CS_MAX_BODY - sizeof(ROLEDATA)];
//char szWatchPetPackage[MAX_PET_PACKAGE -MAX_PET_XIAN_PACKAGE-MAX_PET_SHI_PACKAGE];

char szWatchRelation1[MAX_BUDDY-MAX_BUDDY_FRIEND-MAX_BUDDY_MASTER-MAX_BUDDY_PRENTICE-MAX_BUDDY_SPOUSE-MAX_BUDDY_BROTHER];
char szWatchRelation2[RELATION_MAX-MAX_BUDDY-MAX_BLACK_NUM-MAX_ENEMY];


//#define RAND()  ((unsigned int)random())
#define INIT_DIST 100
#define MAX_TRY_POSITION		8
#define MAX_DROP_DIST			80
#define MIN_DROP_DIST			40
#define IDLE_SEARCH 30
#define MAX_PASS_POINT	15
#define VISIBLE_ANIMATE_VALVE MAX_VISIBLE_ANIMATE 
#define MAX_FUTURE_POINT	32
#define CLT_TIME_WAVE 200
#define CLT_TIME_AUTO_PATROL_WAVE 1
#define PI 3.14159265
#define MAX_MON_SCRIPT_VAR 8
#define MIN_BEATK_SKILL_PRE_MUL 10
#define MAX_RANDOM_RUN_DIST 3000

//static const char  BATTLE_NAME_SYMBOL = '$';
#define BATTLE_NAME_SYMBOL  			"$"
//#define SHORT_MAX 65535

#define DIST_TORRENT 100

#define BENCH_INTERVAL 15
#define SHUTDOWN_NOTIFY_TIMER 10

#define SCREEN_XGRID	3200
#define SCREEN_YGRID	3200
#define AREA_XGRID	6400
#define AREA_YGRID	6400

#define MAX_CS_CMD 1024
#define PLAYER_ACTIVE_TIMER 500
#define TEAM_ACTIVE_TIMER 3000
#define PWORLD_ACTIVE_TIMER 2000
#define MAPINST_ACTIVE_TIMER 2000
#define ITEM_ACTIVE_TIMER 2000
#define MONSTER_ACTIVE_TIMER 500
#define EVENT_ACTIVE_TIMER 500
#define EVENT_DEF_CHECK_TIMER 1
#define EVENT_TO_XML_TIMER 300

#define NPC_ACTIVE_TIMER 500

#define LOGOUT_CHECK_TIMER 20
#define AAS_ACTIVE_TIMER 500

#define MAX_MONSTER_DEF 6000 
#define MONSTER_DEF_DATA 1280
char szWatchMonsterDefData[MONSTER_DEF_DATA-sizeof(MONSTERDEF)];

#define MAX_HOME_LVL_DEF 3
#define HOME_LVL_DEF_DATA 512
char szWatchHomeLvlDefData[HOME_LVL_DEF_DATA-sizeof(HOMELVLDEF)];

#define MAX_HOME_MON_LVL_DEF 80
#define HOME_MON_LVL_DEF_DATA 16
char szWatchHomeMonLvlDefData[HOME_MON_LVL_DEF_DATA-sizeof(HOMEMONLVLDEF)];

#define MAX_HOME_MON_SEG_DEF 12
#define HOME_MON_SEG_DEF_DATA 1024
char szWatchHomeMonSegDefData[HOME_MON_SEG_DEF_DATA-sizeof(HOMEMONSEGDEF)];

#define MAX_HOME_FARM_DEF 60
#define HOME_FARM_DEF_DATA 16
char szWatchHomeFarmDefData[HOME_FARM_DEF_DATA-sizeof(HOMEFARMDEF)];

#define MAX_HOME_SOIL_DEF 20
#define HOME_SOIL_DEF_DATA 32
char szWatchHomeSoilDefData[HOME_SOIL_DEF_DATA-sizeof(HOMESOILDEF)];

#define MAX_SKILL_ARM_RECAST_DEF 10
#define SKILL_ARM_RECAST_DEF_DATA 64
char szWatchArmSkillRecastDefData[SKILL_ARM_RECAST_DEF_DATA-sizeof(ARMSKILLRECASTDEF)];

#define MAX_STORE_GRID_DEF 40 
#define STORE_GRID_DEF_DATA 32
char szWatchStoreGridDefData[STORE_GRID_DEF_DATA-sizeof(STOREGRIDDEF)];

#define MAX_ARM_STEP_UP_DEF 1024 
#define ARM_STEP_UP_DEF_DATA 256
char szWatchArmStepUpDefData[ARM_STEP_UP_DEF_DATA-sizeof(ARMSTEPUPDEF)];  

#define MAX_ARM_VAL_DEF 8 
#define ARM_VAL_DEF_DATA 16
char szWatchArmGradeValDefData[ARM_VAL_DEF_DATA-sizeof(ARMGRADEVALDEF)]; 

#define MAX_ARM_RESET_DEF 12
#define ARM_RESET_DEF_DATA 128
char szWatchArmReSetDefData[ARM_RESET_DEF_DATA-sizeof(ARMRESETDEF)];

#define MAX_BUCHANG_DEF 30 
#define BUCHANG_DEF_DATA 64
char szWatchBuChangDefData[BUCHANG_DEF_DATA-sizeof(BUCHANGDEF)];

#define MAX_ARM_BUCHANG_DEF 256 
#define ARM_BUCHANG_DEF_DATA 64
char szWatchArmBuChangDefData[ARM_BUCHANG_DEF_DATA-sizeof(ARMBUCHANGDEF)];

#define MAX_NEW_ROLE_GIFT_DEF 16
#define NEW_ROLE_GIFT_DEF_SIZE 128
char szWatchNewRoleGiftDefData[NEW_ROLE_GIFT_DEF_SIZE-sizeof(NEWROLEGIFT)];

#define MAX_HIGHWASH_USE_DEF 32
#define HIGHWASH_USE_DEF_DATA 32
char szWatchHighWashUseDefData[HIGHWASH_USE_DEF_DATA-sizeof(HIGHWASHUSEDEF)];

#define MAX_CONT_COVER_DEF 5
#define CONT_COVER_DEF_SIZE 256
char szWatchContCoverDefData[CONT_COVER_DEF_SIZE-sizeof(CONTCOVERGIFT)];

#define MAX_GENIUS_ADD_DEF 16
#define GENIUS_ADD_DEF_DATA 1024
char szWatchGeniusAddDefData[GENIUS_ADD_DEF_DATA-sizeof(GENIUSADDDEF)];

#define MAX_MONSTERSTATESKILL_DEF 100
#define MONSTERSTATESKILL_DEF_DATA 256
char szWatchMonsterStateSkillDefData[MONSTERSTATESKILL_DEF_DATA-sizeof(MONSTERSTATESKILLDEF)];


#define MAX_ITEM_DEF 10000
#define ITEM_DEF_DATA 1800
char szWatchItemDefData[ITEM_DEF_DATA-sizeof(ITEMDEF)];

#define MAX_SELL_DEF 128 
#define SELL_DEF_DATA 4096
char szWatchSellDefData[SELL_DEF_DATA-sizeof(NPCSELLLIST)];

#define MAX_MAP_TRANS_DEF 64
#define MAP_TRANS_DEF_DATA 512
char szWatchMapTransDefData[MAP_TRANS_DEF_DATA-sizeof(MAPTRANSDEF)];

#define MAX_MSG 256000*3

#define Z_SSHEAD_INIT(pstHead, unCmd) {(pstHead)->Cmd = unCmd;(pstHead)->Magic = PROTOCOL_MAGIC;}

#define RES_DIR "../cfg/res"

#define ITEM_DISPLAY_INFO_DATA 2048
#define ITEM_DISPLAY_INFO_MAX	1000

#define NPC_DISPLAY_INFO_DATA 1024
#define NPC_DISPLAY_INFO_MAX	1000

#define MON_LEAVE_MAP_TIME 3

extern int trl_comp_func(const void* pv1, const void* pv2);
extern int trl_comp_func_ll(const void* pv1, const void* pv2);

extern time_t g_tDefCheckTime;

typedef enum tagPlayerAccStatus
{
	PLAYER_STATUS_ACC_NOLOGIN = 0,
	PLAYER_STATUS_ACC_LOGIN_REQ =1 ,
	PLAYER_STATUS_ACC_LOGIN = 2 ,
	PLAYER_STATUS_ROLE_LIST_REQ =3,
	PLAYER_STATUS_ROLE_LIST = 4,
	PLAYER_STATUS_ROLE_LOGIN_REQ =5,
	PLAYER_STATUS_ROLE_LOGIN =6,
	PLAYER_STATUS_ROLE_LOGOUT_REQ =7,	
	PLAYER_STATUS_ROLE_LOGOUT =8,
	PLAYER_STATUS_ACC_LOGOUT_REQ =9,
	PLAYER_STATUS_ACC_LOGOUT =10,
	PLAYER_STATUS_ROLE_ON_BATTLE  =11,
	PLAYER_STATUS_ROLE_LOGOUT_BATTLE =12,
	PLAYER_STATUS_ROLE_NOTHING =13,
}PLAYERACCSTATUS;

typedef enum tagPlayerStatusAssistant
{
	PLAYER_STATUS_BATTLE_NOUSE = 0,
	PLAYER_STATUS_BATTLE_LOGIN_READY = 1,
	PLAYER_STATUS_BATTLE_LOGIN_ON = 2,   //前面的状态都是zone来修改
	
	PLAYER_STATUS_BATTLE_TEAM_SYNC = 64, //这是个伐值,用来形成状态的闭流
	PLAYER_STATUS_BATTLE_GOTO_ZONESVR = 128, //不退出队伍去本服
	
}PLAYERSTATUSASSISTANT;

typedef enum tagZONEStatusMiscType
{
	ZONESTATUS_MISCTYPE = 0,
	ZONESTATUS_MISCTYPE_CHUANRAN_FLAG =1 , //位标志1表示已经不再传染了
	ZONESTATUS_MISCTYPE_DYN = 2 , //有动态参数

}ZONESTATUSMISCTYPE;

typedef enum tagSvrInstMode
{
	INST_MODE_ZONE_SVR = 0,
	INST_MODE_BATTLE_SVR = 1,
}SvrInstMode;

typedef struct
{
	char chVer; 
	char chCmd;
    char chExtraType;                    
    char chTimeStampType;                
    int iID;                            
    int iConnIdx;                       
    TFRAMEHEADDATA stExtraInfo;                    
    TTIMESTAMPDATA stTimeStamp;                    
}ConnectInfo;

enum tagDirty
{
	 DIRTY_NO = 0,
	 DIRTY_NORMAL,	//定时更新
	 DIRTY_IMPORTANT,  //需要立即更新
};

#define INI_VALUE -999	//初始化一个值，作为标识

#define RECORD_TURN 50

#define VISIBLE_ANIMATE_NORM 56 
#define VISIBLE_ANIMATE_MUST 12
#define VISIBLE_ANIMATE_EMON 16
#define VISIBLE_ANIMATE_EPLAYER 16
#define VISIBLE_ANIMATE_FPLAYER 16
#define VISIBLE_ANIMATE_IMPMON 16
#define MAX_VISIBLE_ANIMATE	(VISIBLE_ANIMATE_NORM + VISIBLE_ANIMATE_MUST + VISIBLE_ANIMATE_EMON + VISIBLE_ANIMATE_EPLAYER + VISIBLE_ANIMATE_FPLAYER + VISIBLE_ANIMATE_IMPMON)

#define MAX_VISIBLE_INANIMATE_NORM 32
#define MAX_VISIBLE_INANIMATE_IMP 16
#define MAX_VISIBLE_INANIMATE	(MAX_VISIBLE_INANIMATE_NORM+MAX_VISIBLE_INANIMATE_IMP)

#define MAX_ARM_ON  3  //同时能装备的武器数量

#define MAX_ARM_SKILL	6	//2角色装备 增加技能等级
#define MAX_ARM_ADD_SKILL	3	//2角色装备 附加技能

#define MAX_GN_TOUCH_BUFF 2 //每个角色两个天赋  使用技能次数触发BUFF 数量

#define MAX_STATUS_CHG_SKILL_LEVEL 1//状态改变技能等级的技能个数 (状态效果)

struct tagMidAttrList
{
	unsigned int uiHPMax;
	unsigned int uiMPMax;
	//unsigned short unNl;			// liliang
	unsigned int uiVimMax;
	unsigned int uiSpeMoveMax;   	// 最大移动力	

	unsigned int uiFight; 			//战斗力
	int iPetFight; 				//宠物提供的战斗力

	unsigned char bXianListAdd;
	unsigned char bShiListAdd;

	unsigned int uiSitRestoreHP;	// 打坐回复的HP
	unsigned int uiSitRestoreMP;	// 打坐回复的MP
	unsigned int uiSitAddExp;		// 打坐增加的EXP
	short iSitConsumeVigor;		// 打坐消耗的Vigor
	unsigned int uiVigorMax;		// 最大活力
	unsigned int uiDayExpMax;		// 每天获取经验上限
	unsigned int uiDayZhengyiMax;		// 每天获取正义点上限
	
	unsigned short usDayLifeBrandMax;		// 每天生存奖章上限

	unsigned int uiLevelUpAddMp;	// 等级对应增加mp
	unsigned int uiLevelUpAddHp;	// 等级对应增加hp

	unsigned int uiXpMax; 			// 	最大xp值

	unsigned int uiKillAddHp;		// 击杀获取hp
	unsigned int uiKillAddMp;		// 击杀获取mp

	//力量
	int iLiliangAdd;
	int iLiliangMul;

	//智力
	int iZhiliAdd;
	int iZhiliMul;

	//体力
	int iTiliAdd;
	int iTiliMul;

	//精神
	int iJingshenAdd;
	int iJingshenMul;

	//防御
	int iFangyuAdd;
	int iFangyuMul;

	//闪避
	int iMinJieAdd;
	int iMinJieMul;

	// 一级属性
	unsigned int unLiliang;
	unsigned int unZhili;
	unsigned int unTili;
	unsigned int unJingshen;
	unsigned int unFangyu;
	unsigned int unMinJie;
	
	short nRes;
};
typedef struct tagMidAttrList		MIDATTRLIST;

struct tagAtkChg
{
	/*
	int iArmID; //是哪个装备的武器
	int iArmDefPos;
	int iNeedAmm; //是否需要弹药
	int iAmmID;  // >0 装备了对应的弹药, 0 没有装备对应的弹药
	int iAmmPos; // 弹药包中的位置索引，-1 弹药包里没有弹药了
	int iArmType;
	*/

	//魔法
	int unIceBaseArmAtk; 
	short nIceAtkChgMul;
	int iIceAtkChgAdd;
	int iIceDecDef;
	int iIceDecDefMul;
	int iIceMinAtk;
	int iIceMaxAtk;

	//物理
	int unPhsicBaseArmAtk; 
	short nPhsicAtkChgMul;
	int iPhsicAtkChgAdd;
	int iPhsicDecDef;
	int iPhsicDecDefMul;
	int iPhsicMinAtk;
	int iPhsicMaxAtk;

	//攻城设施
	int unSiegeBaseArmAtk; 
	short nSiegeAtkChgMul;
	int iSiegeAtkChgAdd;
	int iSiegeDecDef;

	//神圣
	int unHolyBaseArmAtk; 
	short nHolyAtkChgMul;
	int iHolyAtkChgAdd;
	int iHolyDecDef;

	//机械
	int unMachineBaseArmAtk; 
	short nMachineAtkChgMul;
	int iMachineAtkChgAdd;
	int iMachineDecDef;

	//火
	int unFireBaseArmAtk; 
	short nFireAtkChgMul;
	int  iFireAtkChgAdd;
	int iFireDecDef;

	//雷
	int unThunderBaseArmAtk; 
	short nThunderAtkChgMul;
	int    iThunderAtkChgAdd;
	int iThunderDecDef;

	//光
	int unLightBaseArmAtk; 
	short nLightAtkChgMul;
	int iLightAtkChgAdd;
	int iLightDecDef;

	//暗
	int unNightBaseArmAtk; 
	short nNightAtkChgMul;
	int iNightAtkChgAdd;
	int iNightDecDef;

	//水
	int unWaterBaseArmAtk; 
	short nWaterAtkChgMul;
	int iWaterAtkChgAdd;
	int iWaterDecDef;

	//攻击系数修正
	int iAtkLevelMul;
	
	 //弹药
	unsigned short unBaseAmmAtk;

	//武器等级修正
	short nArmLevChgMul;

	//破坏乘修正
	short nDamageMul;
	unsigned short unDamageAdd;

	int iAtkPetHurtMul;
	int iAtkNormalMonHurtMul;
	int iAtkJYMonHurtMul;
	int iAtkBossMonHurtMul;
	int iAtkRedHurtMul;

	int iAtkZSHurtMul;
	int iAtkFSHurtMul;
	int iAtkJSHurtMul;
	int iAtkQSHurtMul;

	int iDecZSHurtMul;
	int iDecFSHurtMul;
	int iDecJSHurtMul;
	int iDecQSHurtMul;

	int iAtkSHHurtMul;
	int iAtkZFHurtMul;
	int iAtkSYHurtMul;
	int iAtkYLHurtMul;
	int iAtkJNHurtMul;
	int iAtkLHHurtMul;
	int iAtkLSHurtMul;
	int iAtkHMHurtMul;

	int iDecSHHurtMul;
	int iDecZFHurtMul;
	int iDecSYHurtMul;
	int iDecYLHurtMul;
	int iDecJNHurtMul;
	int iDecLHHurtMul;
	int iDecLSHurtMul;
	int iDecHMHurtMul;
	
	//攻击伤害修正
	ATKERHURTCHG stAtkerHurtChg;
};
typedef struct tagAtkChg		ATKCHG;

struct tagDefChg
{
	//魔法
	int unIceDefBaseAdd;
	short nIceDefChgMul;
	int iIceDefChgAdd;

	//物理
	int unPhsicDefBaseAdd;
	short nPhsicDefChgMul;
	int iPhsicDefChgAdd;

	//攻城设施
	int unSiegeDefBaseAdd;
	short nSiegeDefChgMul;
	int iSiegeDefChgAdd;

	//神圣
	int unHolyDefBaseAdd;
	short nHolyDefChgMul;
	int iHolyDefChgAdd;

	//机械
	int unMachineDefBaseAdd;
	short nMachineDefChgMul;
	int iMachineDefChgAdd;

	//火
	int unFireDefBaseAdd;
	short nFireDefChgMul;
	int iFireDefChgAdd;

	//雷
	int unThunderDefBaseAdd;
	short nThunderDefChgMul;
	int iThunderDefChgAdd;

	//光
	int unLightDefBaseAdd;
	short nLightDefChgMul;
	int iLightDefChgAdd;

	//暗
	int unNightDefBaseAdd;
	short nNightDefChgMul;
	int iNightDefChgAdd;

	//水
	int unWaterDefBaseAdd;
	short nWaterDefChgMul;
	int iWaterDefChgAdd;

	//防御系数修正
	int iDefLevelMul;

	//忽视物防法防
	int iIgnoreDef;	

	//反射HP伤害
	int iReflectHP;

	//防御HP伤害
	int iDefHP;

	// 绝对防御值
	int iAbsDef;

	// 天赋加成
	int iGeniusMul; // 百分比
};
typedef struct tagDefChg		DEFCHG;

struct tagHitChg
{
	int unHitBaseAdd;
	short nHitChgMul;
	int iHitChgAdd;
};
typedef struct tagHitChg		HITCHG;

struct tagFleeChg
{
	int unFleeBaseAdd;
	short nFleeChgMul;
	int iFleeChgAdd;

	short nAtkeeBreakPrepareMul; //抵抗被攻击打断吟唱或集气修正
	short nRes;
};
typedef struct tagFleeChg		FLEECHG;

struct tagHitRateChg
{
	short nAtkerHitRateMul;
	short nAtkerHitRateAdd;

	short nAtkeeHitRateMul;
	short nAtkeeHitRateAdd;
	
	short nHeavyHitAdd; //4		爆击概率
	int nHeavyHitVal; //4		爆击数值

	int iHeavyHit; //4			爆击力
	int iDefHeavyHit;    //4 		爆击抵抗

	int iTongXingHeavyHit;
	int iYiXingHeavyHit;
	int iTongXingHitRate;
	int iYiXingHitRate;
};
typedef struct tagHitRateChg		HITRATECHG;

struct tagResourceAddChg
{
	unsigned short unMPAtkAdd;
	unsigned short unMPAtkMul;
	unsigned short unMPDeadAdd;
	unsigned short unMPDeadMul;

	unsigned short unCollectNumAtkAdd;
	unsigned short unCollectNumAtkMul;
	unsigned short unCollectNumDeadAdd;
	unsigned short unCollectNumDeadMul;
};
typedef struct tagResourceAddChg	RESOURCEADDCHG;

struct tagHealChg
{
	int iHPMaxBase;
	short nHPMaxMul;
	short nHPMaxAdd;

	int  iMPMaxBase;
	short nMPMaxMul;
	short nMPMaxAdd;

	short nLPMaxBase;
	short nLPMaxMul;
	short nLPMaxAdd;

	short nVimMaxBase;
	short nVimMaxMul;
	short nVimMaxAdd;

	short nSpdMoveRestore; 	/*移动力回复速度*/

	short nSpdMoveMaxBase; 		/*最大移动力*/
	short nSPdMoveMaxMul;
	short nSPdMoveMaxAdd;

	short nSpeedMul; //对初始速度的修正成
	double nSpeedAdd; //对初始速度的修正加

	
	short nSpdFlyBase; 		/*飞行速度*/
	short nSPdFlyMul;
	short nSPdFlyAdd;

	short nSpdRideBase; 		/*坐骑速度*/
	short nSPdRideMul;
	short nSPdRideAdd;

	short nBaseHealHPMul;
	short nHealHPMul;
	unsigned short unHealHPAdd;

	short nBaseHealMPMul;
	short nHealMPMul;
	unsigned short unHealMPAdd;

	unsigned short unArmHPAdd;
	unsigned short unArmMPAdd;

	unsigned short unNoFightRestoreHPAdd;
	unsigned short unNoFightRestoreMPAdd;

	unsigned short unFightRestoreHPAdd;
	unsigned short unFightRestoreMPAdd;

	short nSitHPMul;		
	short nSitMPMul;
	short nSitExpMul;
	short nSitVigorMul;

	short nBaseXp; // xp增长速度  现改为读表(FairyDef)
	short nXpAdd;
	short nXpMul;

	short nMaxXpAdd;

	short nArmExpMul;

	unsigned char ucBadStatusRate;
	unsigned char ucBadStatusDefRate;
	unsigned short unBadStatusDecTime;

	unsigned char ucTaskItemRate;
	unsigned char ucStuffItemRate;
	unsigned char ucEggRate;
	unsigned char ucBullRate;

	unsigned short unPetExpMul;
	unsigned short unSkillPonitMul;

	//天赋 攻击触发回血
	short nAtkHPMul;
	short nAtkHPAdd;
	short nAtkHpRate;//触发回血的概率

	//天赋 触发回血的概率
	short nGeniusHealMPMul;
	unsigned short unGeniusHealMPAdd;
	short nGeniusAtkMPRate;

	//天赋 触发伤害增加的概念
	short nGeniusHealHurtMul;
	unsigned short unGeniusHealHurtAdd;
	short nGeniusAtkHurtRate;
	
	//天赋对技能cd的影响,最多10个 有概率
	unsigned short 		  ushCDResultNum;
	SKILLRESULT astCDResult[MAX_CHG_SKILL_CD_NUM];

	//生活技能生产采用的列表， 默认都是第0个，根据装备会调整
	unsigned short unSecondarySkillType;
	unsigned short unRecipeProduceIdx;

	//天赋 积累数技能的数值增加修正
	short nGeniusCollectSkillAdd;
	//天赋 积累数技能的消耗数值减少修正 
	//short nGeniusCollectSkillDec;
	//天赋 积累数技能对最大上限的修正
	short nGeniusCollectSkillMax;

	short nAtkeeHealHPMul;				//4受治疗者修正
	unsigned short unAtkeeHealHPAdd;		//4受治疗者修正
	short nReduceAtkeeHealHPMul;      //降低治疗效果   负比例

	int iAddMspd;
	int iKillerAddHP;
	int iKillerAddMP;
	int iNoFightRestoreHPMul;
	int iNoFightRestoreMPMul;

	int  iHpMulWanFengLv;	// 万分率hp上限
};
typedef struct tagHealChg	HEALCHG;

struct tagMonChg
{
	short nFleeChgMul;
	unsigned short unFleeChgAdd;
	short nAtkeeHurtMul;
	unsigned short unAtkeeHurtAdd;

	short nAtkeeMagicDefMul;
	unsigned short nAtkeeMagicDefAdd;
	short nAtkeeIceDefMul;
	unsigned short nAtkeeIceDefAdd;
	short nAtkeePhsicDefMul;
	unsigned short nAtkeePhsicDefAdd;
};
typedef struct tagMonChg	MONCHG;

typedef struct {
	unsigned short unSkillID;
	unsigned short ucLevel;
}ARMSKILL;


typedef struct {
	unsigned short unSkillID;
	unsigned short ucLevel;
}STATUSCHGSKILL;


//MAX_GN_TOUCH_BUFF
typedef struct
{
	//short int iGnID; 
	
	short int iSkillID;
	short int iTouchNum;
	//short int iUseNum;
	short int iStatusID;
	short int iLevel;
	short int iTime;
}GNUSESKILLCHG;

struct tagDynAttrList
{
	MIDATTRLIST stMidAttrList;
	HEALCHG stHealChg;
	//ATKCHG astAtkChg[MAX_ARM_ON];
	ATKCHG stAtkChg;
	DEFCHG stDefChg;
	HITCHG stHitChg;
	FLEECHG stFleeChg;
	HITRATECHG stHitRateChg;
	ATKEEHURTCHG stAtkeeHurtChg; //抵抗伤害修正
	RESOURCEADDCHG stResourceAddChg;
	MONCHG stMonChg;
	ARMSKILL astArmSkill[MAX_ARM_SKILL];	
	ARMSKILL astAddSkill[MAX_ARM_ADD_SKILL];	
	STATUSCHGSKILL astStatusChgSkill[MAX_STATUS_CHG_SKILL_LEVEL];

	int iGnUseSkill;
	GNUSESKILLCHG astGnUseSkill[MAX_GN_TOUCH_BUFF]; // 天赋   技能使用次数触发BUFF

	int iBeachExpMul;//沙滩经验修正
	int iClanSkillExpMul;//公会技能经验修正
	int iClanSkillVigorMul;//公会技能打怪额外获得活力值概率修正
	int iClanSkillVigorVal;//公会技能打怪额外获得活力值值

	int iRoleLvl;
};
typedef struct tagDynAttrList			DYNATTRLIST;


typedef struct {
	short AtkerHurtMul;                    	/*   天赋对该机能的伤害修正 */
	int AtkerHurtAdd;                    		/*   天赋对该机能的伤害修正 */

	unsigned char HeavyHitRateMul;     /*   对暴击率的修正 */
}ZoneSkillMul;

typedef struct {
	int iSkillSeq;
	RESPOS RolePos;
   	RESPOS AtkPos;
	int iTargetNum;
	int aiTargetIDs[MAX_SKILL_TARGET];
}ZoneSkillAtkInfo;

typedef struct {
	CSSKILLDATA stSkillData;
	struct timeval stCoolEnd;
	short DefIdx;
	unsigned char bTotem;
	int iTotemHead;
	char cPetIdx;
	tdr_ulonglong ullPetWID;
	int iTagAniID;
	ZoneSkillMul stSkillMul;
	ZoneSkillAtkInfo stAtkInfo;
	int iRuneChgCoolEnd;
}ZoneSkill;

typedef struct {
	struct timeval stCommCoolEnd;
	short nSkillNum;
	short nSkillPos;
	ZoneSkill astZoneSkills[MAX_ROLE_SKILL];
}ZoneSkillList;


typedef struct {
	unsigned short unStatusID;
	unsigned char bLevel;
	unsigned char bBindSkillTrig;
	struct timeval stEnd;
	int iLast;//持续的时间
	unsigned int iJumpNum; //状态跳跃的次数
	unsigned int iJumpedCount;//已经跳过的开始计数
	short nDefIdx;
	unsigned char bType;
	unsigned char bSubType;
	unsigned short unBindSkillID;
	unsigned char bBindSkillLevel;
	unsigned char bCount;			
	short nRelationDefIdx;
	unsigned short unOverlay; //叠加数
	int iVal;
	int iApplyID;
	char cApplyRelation;//0好友,1表示敌方. 生命通道时用
	time_t tLastStatusCheck;
	STATUSRESULT  ResultStepDecSpd;
	STATUSRESULT ResultStepDecHp;
	short nOverlayMul; //叠加指数
	unsigned int MiscType; //参考 ZONESTATUSMISCTYPE 里的位定义
	int iFeedbackMemID;	//回馈的memid
	int iFeedbackStatusID; //回馈的状态id
	int iDynPara;//动态参数
	int iSeqID;
	short iByAtk;
}ZoneStatus;	

#define MAX_PAST_STATUS 10

typedef struct {
	unsigned char bGoodStatusNum;
	unsigned char bBadStatusNum;
	unsigned char bNoLimitStatusNum;
	unsigned char bRes;

	ZoneStatus astGoodStatus[MAX_GOOD_STATUS];
	ZoneStatus astBadStatus[MAX_BAD_STATUS];
	ZoneStatus astNoLimitStatus[MAX_NOLIMIT_STATUS];
}ZoneStatusList;


#define ATK_FLAG_BIT_NORMAL   0
#define ATK_FLAG_BIT_SVR_LIGHTNING   1
#define ATK_FLAG_BIT_SVR_ATK   2
#define ATK_FLAG_BIT_CLT_ATK   4

typedef struct {
	int iSkillSeq;
	unsigned short unSkillID;
	unsigned short unLevel;
	unsigned short unPilot;
	struct timeval stPrepare;
	struct timeval stLastPilot;
	struct timeval stPilotEnd;
	struct timeval  stAtkTime;
	unsigned int iJiqiTime;//集气时间
	char cPilotEnd;
	char cAtkFlag; //atk 与hit 配对，防止引导技能恶意多发hit
	char cFittingsIdx;
	//int iPrepareTimeMul;	//最近一次被攻击技能延时的百分比
	RESPOS stAtkPos;
	int iSkillCltSeq;
	int iAtkFlagBit;//位操作
	int iAtkLightningIdx;
}AtkInfo;

typedef struct {
		short shNum;
		short ashProc[MAX_DLG_OPT];
} DialogOpt;

#define MAX_DIALOG_COMMIT_NUM 10

typedef struct { 
	int iListType;		//4栏位信息
	int iGridIdx;
} DialogCommitInfo;

typedef struct { 
	int iItemID;		
	int iNum;
} DialogCommitIDInfo;


typedef struct {
		short shProc;
		int iNum;	//多格提交数量
		DialogCommitInfo astGrids[MAX_DIALOG_COMMIT_NUM];// 提交物品的位置信息		
} DialogCommitOpt;


typedef union {
		short shNext;
		DialogOpt stOpt;
		DialogCommitOpt stCommitOpt;
} ScriptData;

#define MISC_FLAG_PACKAGE_ALL 0x01LL
#define MISC_FLAG_TAG 0x02LL
#define MISC_FLAG_TRACE 0x04LL
#define MISC_FLAG_TASK_ALL 0x08LL
#define MISC_FLAG_CLAN_SNAP 0x10LL
#define MISC_FLAG_MAIL_FETCHED 0x20LL
#define MISC_FLAG_MAIL_SNAP 0x40LL
#define MISC_FLAG_WUSHUANG 0x80LL
#define MISC_FLAG_BULLETIN_SNAP 0x100LL
#define MISC_FLAG_STATUS_TRACE 0x200LL
#define MISC_FLAG_DESIGNATION_SNAP 0x400LL
#define MISC_FLAG_RESPACT_SNAP 0x800LL
#define MISC_FLAG_RESPACT_TRACE 0x1000LL
#define MISC_FLAG_HIT_TRACE 0x2000LL
#define MISC_FLAG_CONVOY_TASK_SNAP 0x4000LL
#define MISC_FLAG_CANNOT_ADDHP 0x8000LL
#define MISC_FLAG_CANNOT_ADDMP 0x10000LL
#define MISC_FLAG_STATUS_CHGMAP_CLEAN 0x20000LL
#define MISC_FLAG_STATUS_FAIRYCAMP_TRACE 0x40000LL // 精灵出征追踪
#define MISC_FLAG_OWNED_TMP_ITEM_TRACE 0x80000LL   // 临时道具拥有情况追踪
#define MISC_FLAG_SEND_SHOPDEF 0x100000LL	// 三秒后发给用户商城的数据
#define MISC_FLAG_AUTO_PICK_ITEM 0x200000LL	// 自动拾取的状态位
#define MISC_FLAG_LIFE_TIME_TRACE 0x400000LL	// 存活时间
#define MISC_FLAG_KILL_PLAYER_TRACE 0x800000LL	// 击杀玩家追踪
#define MISC_FLAG_SECONDARY_SKILL_ALL  0x1000000LL
#define MISC_FLAG_BEATK_TRACE     0x2000000LL
#define MISC_FLAG_CLANID_SNAPSHOT 0x4000000LL // 全部公会ID
#define MISC_FLAG_TMPITEM_PATROL_TRACE	0x8000000LL 	// 自动巡逻的临时机械任务跟踪
#define MISC_FLAG_SEND_LOCK_SAFE 0x10000000LL // 安全锁信息
#define MISC_FLAG_LOGOUT_FOR_MUD 0x20000000LL // 退出到MUD
#define MISC_FLAG_EXP_NOLEVEL_LIMIT 0x40000000LL //忽略等级经验限制
#define MISC_FLAG_IN_BEACH 		0x80000000LL //在海滩层
#define MISC_FLAG_LUCK 			0x100000000LL //参加了天降宏福活动
#define MISC_FLAG_MAP_BROADCAST 0x200000000LL //小地图上暴露了
#define MISC_FLAG_USE_ITEM 		0x400000000LL	//使用道具
#define MISC_FLAG_ONLINE_TIME 	0x800000000LL	//角色一天在线时间
#define MISC_FLAG_SKILL 	    0x1000000000LL	//角色使用技能
#define MISC_FLAG_SPY 	      	0x2000000000LL	//角色在刺探
#define MISC_FLAG_ANTI_SPY 	    0x4000000000LL	//角色在守护
#define MISC_FLAG_SPY_FINI 	    0x8000000000LL	//角色刺探任务完成状态
#define MISC_FLAG_ANTI_SPY_FINI 0x10000000000LL	//角色守护任务完成状态
#define MISC_FLAG_APEXON 0x20000000000LL	//开启apex
#define MISC_FLAG_EXPNODAYLIMIT 0x40000000000LL  //不记录在每日经验中

#define MON_MISC_FLAG_STATUS_TRACE 0x01
#define MON_MISC_FLAG_DYN 0x02
#define MON_MISC_FLAG_IMPVIEW 0x04
#define MON_MISC_FLAG_MAPBROAD 0x08
#define MON_MISC_FLAG_RESTORE_HP 0x10
#define MON_MISC_FLAG_MAP_POS 0x20
#define MON_MISC_FLAG_IGNORE_PWORLD_OVER 0x40	//副本结束了,也可以强势插入
#define MON_MISC_FLAG_PWORLD_WAVE 0x80	//副本波次怪
#define MON_MISC_FLAG_PWORLD_TIMER 0x100 //副本定时怪
#define MON_MISC_FLAG_NO_DYNBAK 0x200 //忽略动态阻挡的影响
#define MON_MISC_FLAG_MAP_SHOW 0x400 //当前地图上显示

#define FIGHT_STAT_DUR 6
#define GAOJI_SIT_CONSUME_VIGOR 20
#define SIT_SKILLID 7005

typedef struct {
	tdr_ulonglong ullRoleID;
	int iMemID;
	time_t tTime;
}TeamHandleEntry;

typedef struct {
	unsigned short unApplyNum;
	unsigned short unInviteNum;
	TeamHandleEntry astApplyEntry[MAX_APPLY_NUM];
	TeamHandleEntry astInviteEntry[MAX_INVITE_NUM];
}TeamOnline;

#define MAX_TRAP 5
#define MAX_TOTEM 2

#define BUDDY_GET_INIT 0
#define BUDDY_GET_GETTING 1
#define BUDDY_GET_FINI 2
#define BUDDY_GET_FAIL 3

typedef struct {
	tdr_ulonglong ullWID;
	unsigned short unDefID;
	unsigned char bPetType; //PET_TYPE_XIANZI, PET_TYPE_SHIWEI
	char cPackageIdx; //背包中的数组索引
	int iDefIdx;	
	int iRaceLevelDefIdx;
}RolePetOnline;

typedef struct {
	int iNextLevelExp; //0 未初始化, -1 不能再加经验了

	short nExpMul; //宠物素质等影响的经验值乘修正
	unsigned short unExpAdd; //宠物素质等影响的经验值加修正
} PetExpOnline;

typedef struct {
	short nPetPowerRestoreMul; //power 修正乘修正
	short nPetPowerRestoreAdd; //power 修正加修正
	
	int iPowerMaxBase;
	short nPowerMaxMul;
	short nPowerMaxAdd;
} PetPowerOnline;

#define RESULT_NATIVE_EXP_ADD_MAP 1
#define RESULT_NATIVE_MAX_LOVE_VAL_ADD_MAP 2
#define RESULT_NATIVE_ITEM_HP_ADD_MAP 4
#define RESULT_NATIVE_OWNER_DIE_EXP_MAP 8
#define RESULT_NATIVE_OWNER_DIE_MONEY_MAP 16
#define RESULT_NATIVE_XP_ADD_MAP 32
#define RESULT_NATIVE_REPAIR_COST_MAP 64
#define RESULT_NATIVE_LOVE_VAL_DIE_MAP 128

typedef struct
{
	int iFlag;//标志位
	short sExpAdd;
	short sMaxLoveValAdd;
	short sItemHpAdd;
	short sOwnerDieExp;
	short sOwnerDieMoney;
	short sXpAdd;
	short sRepairCost;
	short sLoveValDie;
}PETNATIVEINFO;

typedef struct
{
	short sBaseAttrMul; //基本属性乘修正
}PETLOVELEVELINFO;


typedef struct {
	ATKCHG stAtkChg;
	DEFCHG stDefChg;
	ATKEEHURTCHG stAtkeeChg;
	HITCHG stHitChg;
	FLEECHG stFleeChg;
	HITRATECHG stHitRateChg;
	PETNATIVEINFO stNativeInfo;
	PETLOVELEVELINFO stLoveLevelInfo;
}PetFightAttrOnline;

typedef struct {
	unsigned short SkillID;
	unsigned short SkillLevel;
	int iSkillDefIdx;
	int iSkillType;				
	struct timeval stCoolEnd;
}PetZoneSkill;

typedef struct {
	unsigned short unStatusID;
	short nDefIdx;
	unsigned char bLevel;
	char szRes[3];
	struct timeval stEnd;
	unsigned short unOverlay; //叠加数
	short nOverlayMul; //叠加指数
}PetZoneStatus;	

typedef struct {
	unsigned char bGoodStatusNum;
	char szRes[3];
	ZoneStatus astGoodStatus[MAX_PET_GOOD_STATUS];
}PetStatusInfo;	

typedef struct {
	int iYongQiAdd; 
	int iDecHurtMul; 
} PetTiShenOnline;

typedef struct {
	RolePetOnline stRolePetOnline;
	PetExpOnline stPetExpOnline;
	PetPowerOnline stPetPowerOnline;
	PetFightAttrOnline stPetFightAttrOnline;
	PetTiShenOnline stPetTiShenOnline;
	char szName[ROLE_NAME_LEN];
	char iXieDaiPos;
	unsigned char bSkillNum;
	char cTiShenFlag;
	PetZoneSkill astPetZoneSkills[MAX_PET_SKILL];
	int fNormalSkillHit;
	PetStatusInfo stStatusInfo;
}PetFightShiOnline;

typedef struct {
	RolePetOnline stRolePetOnline;
	PetPowerOnline stPetPowerOnline;
	PetFightAttrOnline stPetFightAttrOnline;
	PetTiShenOnline stPetTiShenOnline;
	PetExpOnline stPetExpOnline;
}PetBackOnline;

typedef struct {
	unsigned char bFightShiNum;
	unsigned char bBackNum; //后备
	PetFightShiOnline astFightShi[PET_MAX_FIGHT_SHI];
	PetBackOnline astBack[MAX_PET_PACKAGE];
	tdr_ulonglong llPetWidBackup;
}PetOnline;


typedef struct
{
	tdr_ulonglong ullExChgObj; 		// 交易对方ID
	int iInViteTime;					// 邀请时间或者被邀请时间
	int iMemID;

	char bExchgStat;

	//unsigned char bIsLock;			// 是否点击锁定交易
	//unsigned char bIsExChg;			// 是否点击确认交易

	EXCHGDATA stExchgData;
}ExchgOnline;

typedef struct
{
	tdr_ulonglong ullRoleID; // 邀请发起人的ID
	CLANID stClanID; // 邀请加入的公会ID
	time_t tTime; // 邀请发起时间
}ClanOnline;

typedef struct
{
	unsigned short unExpRate;
	unsigned short unPetExpRate;
	unsigned short unBullRate;
	unsigned short unSkillPointRate;
	unsigned short unTaskItemRate;
	unsigned short unStuffItemRate;
	
}FittingsOnline;

typedef struct
{
	tdr_ulonglong ullStallOtherWID;		// 打开其他玩家摊位时保存的WID

	
	tdr_ulonglong ullStallWID;					// 自己摊位WID
	char szStallName[STALL_MAX_NAME_LEN];	// 摊位名字
	STALLDATA stStallData;					// 自己摊位信息
	int tStallActiveTime;			// 记录下上次扫描的时间
}StallOnline;

#define FOLLOW_ANGEL_DIV 6

#define MAX_WEI_XING 24						// 最大尾行玩家
typedef struct
{
	unsigned char cWeiXingNum;
	int aiWeiXingMemID[MAX_WEI_XING];		// 尾行玩家memID
}WeiXingInfo;

// 动态选项列表相关
typedef struct
{
	int iContext;
	int iType;
	int iSelectedOpt;
	int aiOpts[MAX_DLG_OPT];
}DynamicOptListInfo;

typedef struct
{
	int iOwnerID;
	int iPassengerNum;
	int PassengerID[MAX_RIDE_PASSENGER];
}RideOnInfo;

typedef struct 
{
	tdr_ulonglong WID;
	unsigned char cGridIdx;
} FairyOneOnline;

typedef struct 
{
	unsigned short DiathID;
	unsigned char DiathLV;
	unsigned int CoolEnd;
} FairyDiath;

typedef struct 
{
	unsigned short unPhyAtk;// 物理攻击力
	unsigned short unMgcAtk;// 法术攻击力
	unsigned short unBrkATk;// 破坏力

	unsigned short unFireATk;		//4 火攻击
	unsigned short unThunderATk;	//4 雷攻击
	unsigned short unLightATk;		//4 光攻击
	unsigned short unNightATk;	//4 暗攻击
	unsigned short unWaterATk;	//4 水攻击
} FairyCampAttr;

typedef struct
{
	//unsigned char iNum;
	//FairyOneOnline FairyOneOnline[FAIRY_MAX_NUM];
	tdr_ulonglong ullCampWID;
	FairyCampAttr stFairyAttr;
	char iDiathNum;
	FairyDiath FairyDiathList[FAIRY_MAX_SKILL];
	unsigned int uiCampPreTime;
	tdr_ulonglong ullWID;	// 吟唱精灵WID 
}FairyOnline;

typedef struct
{
	char cCampIdx;	// 阵营idx
	char cPloyType;	// 
	char fLeader;	
	int iCrystal;		// 水晶数
	int iOre;			// 矿石数
	int iKillNum;		// 杀人数
	int iContinueKill;	// 连杀数
	int iCurrKill;		// 当前杀人数
	int iDieNum;		// 被杀数(包括怪和人)
	int iMaxLife;		// 最大生存时间
	int iCurrLife;		// 当前生存时间
	int iDieSec;		// 死亡计数器
	int iLevel;
	int iAssistKill;		//2助攻
	int iTotalExp;
	int iAwardTime;
	int iEnergyStat;
	time_t tFightTime;	//进入时间
}WarFieldOL;

typedef struct
{
	int iQueueType;
	int iPworldID;
	int iPworldInstID;
	int iCamp;
	int iQueueIdx;			// 等待队列索引
	SSBATTLECAMPINFO stBattleCampInfo; //战场阵营信息 这个要记  battle_svr查不到waitqueue
}RolePworldQueue;

typedef struct
{
	int iQueueType;
	int iPworldID;
	int iPworldInstID;
	int iWaitQueueIdx;			// 等待队列索引
}ShiKongRoleQueueInfo;


//剩余的巡逻路径,如果中途有偏移,最后的路径应该要把这里的续上
typedef struct
{
	unsigned char cFuturePoints;
	RESPOS stTurnPoints[RECORD_TURN];
}REMAINPATROL;

typedef struct
{
	char cMaster;
	char cCurrWeather;
	char cNextWeather;
	int iChgTime;	
}SVRWEATHER;



#define AUTO_TEAM_SINGLE_NUM 200
#define AUTO_TEAM_CREATED_NUM 200
#define AUTO_TEAM_PRE_NUM 300

//#define SCENEOVERLAY_SAFE 1        	// 安全区
//#define SCENEOVERLAY_TEST 2		// 切磋区

typedef struct {
	AUTOTEAMAPPLYINFO stApply;
	char AutoTeamID;
	char Stat; //0 single 1 created 2 pre
	unsigned char LimitTrigFlag; // 1 表示已经超时放宽了条件	
	int bOPFlag; //操作标记
	time_t tStart; //开始排的时间
	time_t tAutoTeamValidTime;//自动组队成功后,130秒倒计时。如果倒计时过了,并且队伍不在副本里
                                                //则会清理掉bOPFlag,和AutoTeamID, 组队玩家,如果收到到TeamInfo的数据里面
                                                //AutoTeamID <=0时,点击自动组队图标,不弹出可以进入副本的面板
	char TransAutoTeamID;
	int iPworldGrade;
	                                                
}AutoTeamInfo;

typedef struct
{
	int iStartLayer;
	int iUseItemEnterPworld;			//是否使用道具进的副本 0-否 1-是
	int iRaffleNum;	
	int iRndDefIdx;					// rnd表索引
	int iItemIdx;						// 第几个物品
	int iBctFlag;						// 登录时要用到 根据
	int iOldMap;
	int iJump;
}BCTOnline;

typedef struct
{
	unsigned short SkillType;
	unsigned short RecipeID;
	time_t tPreEndTime;
}RecipePreOnline;

typedef struct
{
	int iMachineMonID;
	tdr_ulonglong ullLockItemWID;
	time_t tLeavePreTime;
}MachineInfo;

#define PLAYER_MAX_PRESSED 16
#define MAX_CHOOSE_NUM 2 //最大选择事件数量

typedef struct
{
	char cChooseBindType;	//选择确认框脚本绑定类型
    int iChooseMemID;	//选择确认框脚本绑定类型ID
    int aiChooseProc[MAX_CHOOSE_NUM];//选择对应proc
    char szScript[RES_NAME_LEN];//脚本名
    int iChooseLevel;	//选择确认框层级
}ChooseInfo;

typedef struct
{
	int iMemID;				// 切磋对方MemID
	int iReqID;				// 切磋邀请
	int iAcceptTm;			// 切磋同意的时间
	int iLeaveTm;			// 离开切磋区域的时间
	RESPOS stTestPos;		// 切磋旗子位置
}QiecuoInfo;

typedef struct
{
	char bBHlingRate;	// 精灵背后灵概率
	char bFlag;
	char bSkillLvl;
	int iSkillID;
}BHLingOL;

typedef struct
{
	int iMemID;				
}AtkBossInfo;

enum tagPET_FUSE_STAT 
{
	PET_FUSE_NO = 0, 		//2不是融合状态 
	PET_FUSE_READY = 1, 	//2 融合状态 
	PET_FUSE_DEAL = 2, 		//2 确认融合状态 
};

typedef struct
{
	tdr_ulonglong ullPetWid; 			//1自己将要融合的宠物
	tdr_ulonglong ullOtherPetWid; 		//1 对方将要融合的宠物
	int iMemID;						//1对方角色memid
	int iInViteTime;					//1邀请时间或者被邀请时间
	
	char bStat;						/*PET_FUSE_STAT*/
}PetFuseOnline;

#define MAX_PLAYER_BUB 5

enum tagMON_BUB_STAT 
{
	BUB_STAT_ATK = 0, 		//2攻击状态
	BUB_STAT_FOLLOW = 1, 	//2 跟随状态
};

typedef struct
{
	int iStat;
	int iBubNum;
	int iBubID[MAX_PLAYER_BUB];
}BubOnline;

#define MAX_VISIBLE_IMP_MON 16

#define MAX_LIGHTNING_SKILL_NUM 2
#define MAX_LIGHTNING_JUMP_NUM  10

//闪电链攻击数据 
typedef struct
{
	int iJumpNum; //跳跃次数 
	RESPOS arrJumpedPos[MAX_LIGHTNING_JUMP_NUM];//已经调过的pos
	int iMemID[MAX_LIGHTNING_JUMP_NUM];//已经调过的pos
	unsigned short ushSkillID;
	int iAtkChgIdx;	
}LightningAtkInfo;

typedef struct
{
	time_t tPreTime;
	int iPreItemID;
	int iPreTargetID;
}ItemUseInfo;

typedef struct
{
	tdr_ulonglong ullPetWid;
	int iAddVal;
	char cReStrenLvl;
	int iBindNum;
}PetStrengthenUIInfo;

typedef struct
{
	tdr_ulonglong ullPetWid;
	int iGenius;
	int iPhsicAtkGenius;                  
    	int iIceAtkGenius;                    
    	int iHPGenius;                        
    	int iDefGenius;                    
	int iIceDefGenius;                       	
   	int iHitRateGenius;                   	
    	int iFleeGenius;                      
}PetWashGeniusInfo;

typedef struct tagAutoMove
{
	int iMapID;
	int iX;
	int iY;
	int iTmpItemID;
	int iStatusID;
}AutoMove;

typedef struct tagUnFengyin
{
	int iListType;
	int iGrid;
	char szRoleName[NAME_LEN];
}UnFengYin;

#define MAX_PLAYER_PROC_NUM 4 //玩家最大同时踩的proc数

struct tagZoneAni;


/*
#define MAX_ARM_RECAST_BATCH 10

typedef struct tagRecastItemsDetail
{
	int iBaseAttrNum;
	BASEATTRINFO astBaseInfo[MAX_ARM_BASE_ATTR];
}RecastItemsDetail;

typedef struct tagRecastInfo
{
	int WID;
	int iRecastNum;
	RecastItemsDetail astInfo[MAX_ARM_RECAST_BATCH];
	
}RecastInfo;
*/
typedef struct tagHetiOL
{
	tdr_ulonglong PetWID;
}HETIOL;

typedef struct tagArmStarOL
{
	tdr_ulonglong ArmWID;
	int iSelectStar;
	int iMulIdx;
}ArmStarOL;

typedef struct tagMapOL
{
	int iEndTime; // 有些地图会有玩家存在时间
}MapOL;

typedef struct tagUnrealOL
{
	int iEnterFlag;
}UnrealOL;

// 
typedef struct tagGemReSetOL
{
	char cHoleIdx;
	char cAttrIdx;
	char cValIdx;
	char cMul;
	tdr_ulonglong ullArmWID;
	
}GemReSetOL;

typedef struct tagTeamFollowOL
{
	char cTeamFollow;  
	
}TeamFollowOL;

typedef struct tagDieCleanOL
{
	char cDieClean;
	int iMemID; // 
	
}DieCleanOL;

typedef struct tagAddAttrWashOL
{
	tdr_ulonglong ullArmWID;
	int iAddAttrNum;
	ADDATTRINST stAddAttr[MAX_ARM_ADD_ATTR];
	int iWashAddAttrNum;
	ADDATTRINST stWashAddAttr[MAX_ARM_WASH_ADD_ATTR];
}AddAttrWashOL;

typedef struct tagTeamSummonInfo
{
    int Map;                             	/*   地图ID */
    int X;                               	/*   坐标x */
    int Y;                               	/*   坐标y */
    int MapPos;                         	/*   map 实例位置 */
	int OverTime;							//超时时间秒
}TeamSummonInfo;

typedef struct tagClanSummonInfo
{
    int Map;                             	/*   地图ID */
    int X;                               	/*   坐标x */
    int Y;                               	/*   坐标y */
    int MapPos;                         	/*   map 实例位置 */
    int OverTime;				//超时时间秒
}ClanSummonInfo;

/* Span Online */

typedef struct tagSpanChgInfo
{
	int iDstID;
	tdr_ulonglong ullRoleID;
	SSSPANDSTDATA stDstData;
}SpanChgInfo;

typedef struct tagSpanHoldInfo
{
	int iSrcID;
	tdr_ulonglong ullRoleID;
}SpanHoldInfo;

typedef struct tagSpanApplyInfo
{
	tdr_ulonglong ullRoleID;
}SpanApplyInfo;

typedef struct tagSpanConnectInfo
{
	int iDstID;
	tdr_ulonglong ullRoleID;
}SpanConnectInfo;

enum tagPLAYER_SPAN_STAT 
{
	SPAN_STAT_CONNECT = 1, 		//2(主线才存在的状态) 连接其他分线状态
	SPAN_STAT_HOLD_REQ = 2, 		//2切线请求状态, 申请预占位
	SPAN_STAT_HOLD = 3, 			//2占位状态，仅仅是一个空的Player,占住了一个名额
	SPAN_STAT_UPDATE = 4, 			//2数据回写状态，切线占位成功后
	SPAN_STAT_APPLY = 5, 			//2报名状态，假人，仅为了处理报名相关逻辑需要
};

typedef union {
	SpanConnectInfo stConnect;
	SpanChgInfo stHoldReq;
	SpanHoldInfo stHold;
	SpanChgInfo stUpdate;
	SpanApplyInfo stApply;
}SpanInfoUnion;

enum tagPLAYER_SPAN_APPLY_STAT
{
	SPAN_APPLY_NONE = 0,
	SPAN_APPLY_ING = 1,
};

typedef struct tagSpanApplyData
{
	int iSpartaApply;
	int iBattleApply;
}SpanApplyData;

typedef struct tagSpanOL
{
	int iStat;
	int iMasterBusID;
	SpanInfoUnion stInfo;
	SpanApplyData stApply;
}SpanOL;

typedef struct tagCityFightOL
{
	int iContinueKill;
}CityFightOL;

typedef struct tagExtendedOL
{
	tdr_ulonglong WID;  
	int ExtendedFromDefID;
	EXTENDEDRESULT ExtendedResult;               	
}ExtendedOL;

#define PLAYER_DAY_EXP_WARNING 1


typedef struct tagCardBoxSelectGrid
{
	int iSelect;
	int iGridIdx;                         	/*   位置 */
	int iGroupID;
	int iIdx;
	int iAwardIdx;
	CARDBOXITEM stItem;
}CardSelectGrid;

#define CARD_BOX_SELECT_NUM 5

typedef struct tagCardBoxOL
{
	int iStat;
	int iBoxID;
	int iBoxWashCount;
	int iBoxCostCount;
	int iBoxSelectCount;

	int iCardNum;
	CardSelectGrid astCardInfo[MAX_CARD_BOX_CARD_NUM];
}CardBoxOL;

typedef struct tagArmReSetOL
{
	int iAttrNum;
	ADDATTRINST AddAttr[MAX_ARM_ADD_ATTR]; 
	unsigned long long WID;
	
}ArmReSetOL;


typedef struct {
	int State;
	unsigned char bDir;
	unsigned char cMoving;
	unsigned char cCurrPoint;
	unsigned char cFuturePoints;
	unsigned char cCheckAdust;
	unsigned char cLastPath;
	unsigned char cLastPathPoints;
	unsigned char cWalkPath;
	RESPOS stTurnPoints[RECORD_TURN];
	RESPOS stLastSearchPos;
	REMAINPATROL  stRemainPatrol;
	char cPatrolFlag; // 1表示设置了路径 2表示路径要恢复
	struct timeval stLastSearchTv;
	struct timeval stLastMoveTv;
	struct timeval stLastCltMoveTime;
	struct timeval stLastCltTime;
	struct timeval stLastCltStopTime;
	struct timeval stRidePrepare;
	struct timeval stLastSpeed0;
	CSMSPDINFO stMspdInfo;
	long long llTimeErrAdjust;
	int iTimeAdjustNum;
	struct timeval stTimeAdjustCool;
	TTIMEVAL stConnectTime;
	int iMapPos;
	int iPosSeq;
	short iEmon;
	short iEplayer;
	short iFplayer;
	short iImpMon;
	int iVAnimate;//可视动态对象
	int aiVAnimate[MAX_VISIBLE_ANIMATE];
	int iVInanimate; // 可视物品对象
	int aiVInanimate[MAX_VISIBLE_INANIMATE];
	char cDoSearch;
	char cCheckCltTimeFlag;
	unsigned char cDataDirty; //标识stRoleData中是否有重要数据变化
	unsigned char cCheckName;
	DYNATTRLIST stDyn;
	ZoneSkillList stSkillList;
	ZoneStatusList stStatusList;
	AtkInfo stAtkInfo;
	TestEnv stTestEnv;
	ScriptData stSData;
	int iDialogID;
	time_t tPackageArrange;	// 杂物包
	time_t tPackageArrange1;// 仓库
	TeamOnline stTeamOnline;
	char cDialogStat;
	unsigned char bTrustPosCount;
	unsigned char bFirstAjustCount;
	CSLOCK stLock; //锁定信息
	long long cMiscFlag; //一些杂项标记MISC_FLAG_PACKAGE_ALL
	time_t tFightStatEnd;
	struct timeval stChatCommCool;
	time_t tChatWorldCoolEnd;
	time_t tRelationInitReq;
	time_t tCollectAward;
	int iCollectTarget;
	int iOldMap;
	int iCleanStatuFlag;
	RELATION stRelation;
	int iReviverID;
	int iReviverSkillID;
	int iReviverHP;
	int iReviverMoney;
	int iReviverExp;
	RESPOS stReviverPos;
	int iTotemHead;
	int RideItemID;
	int RideLevel;
	unsigned char bRelationUpdateFlag;
	unsigned char bTotemNum;
	unsigned char bGetBuddyStat;
	unsigned char bAddFriendReqNum;
	int aiAddFriendReqList[MAX_TEAM_MEMBER];
	int  iTmpEnemyNum;
	tdr_ulonglong ullTmpEnemy[MAX_SKILL_TARGET];
	int iSecCount;
	PetOnline stPetOnline;
	int iPetFightPreTime;
	int iHitTarget;
	int iAtkerTarget;
	ExchgOnline stExchgOnline;
	int iPworldID;
	ClanOnline stClanOnline; // 有关公会的在线信息
	char cUIType;
	unsigned char bQiYueArmIdx;
	unsigned char abFollowAngel[FOLLOW_ANGEL_DIV];
	unsigned char bDbBuddyRetry;
	char cDialogStatOld;
	char cSecenOverLay;		//场景区域覆盖
	unsigned char cSecenFlag;
	time_t tTrustUpdateEnd;
	int iClanMemIdx;
	//FittingsOnline stFitOneline;
	StallOnline stStallOnline;
	int tLogoutBegin; // 接收到退出指令的时间
	unsigned char bWaitRoleLogout; // 等待退出
	unsigned char bConnLost; //conn连接是否断开
	unsigned char bLogoutPre;  //2是否处理了退出前的七七八八操作
	
	int iSharingTaskID; // 共享任务 ID
	int tSharingTime; // 开始共享的时间
	int iSharingOwner; // 共享任务发起人
	//int iGrabSafeTm;	// 被顶保护时间
	
	time_t tRoleLogin;
	time_t tRoleLogout;
	int iCallNpcMemID;	// 召唤NPC 内存ID
	WeiXingInfo stWeiXing;
	DynamicOptListInfo stDynamicOpt; // 动态选项列表
	RideOnInfo stRideOnInfo;		//多人骑乘相关信息
	FairyOnline stFairyOnline;
	int ePkMode;						//1Pk模式
	time_t tPkEndTime;						//1 恶意PK灰名字时间
	int iSkillAtkerID;						//状态附带技能释放技能的人
	
	time_t tRoleChgMap;
	WarFieldOL stWarOL;
	RolePworldQueue stPworldQueue;

	AutoTeamInfo stAutoTeamInfo;
	BCTOnline stBctOnline;
	RecipePreOnline stRecipePre;

	tdr_ulonglong nExtendedResultBitMap; // 附魔装备bitmap,用于快速判断哪些身上穿
	                                    // 着的装备有附魔效果
	time_t tExtendedResultEnd[MAX_WEAR]; // 记录附魔特殊效果的冷却时间

	int iPressedMonNum; // 记录哪些怪物的压力表中有Player
	int aiPressedMon[PLAYER_MAX_PRESSED];

	MachineInfo stMachineInfo;
	CSPLAYERATTRVAL stBaseAttrList;
	int aiProcEvtIdx[MAX_PLAYER_PROC_NUM];
	int iProcEvtNum;//触发的逻辑区数量
	char cSafeLockFalg;

	ChooseInfo stChooseInfo;//记录选择确认框信息
	QiecuoInfo stQiecuoInfo;	// 切磋信息

	time_t tAutoReviveTime;

	AtkBossInfo stAtkBossInfo;
	BHLingOL stBhlingOl;
	PetFuseOnline stPetFuseInfo;
	BubOnline stBubOnline;

	LightningAtkInfo stLightningAtkInfo[MAX_LIGHTNING_SKILL_NUM];
	char szAddFriendName[NAME_LEN];

	int fAasMsgNotify;			//1有些防沉迷提示需要依次决定
	int iLeaveStat;
	MOTIONINFO stMotionInfo; 	//2动作表情

	int iSelectTarget;				//1目标
	int iOldSelectTarget;

	ItemUseInfo stItemUseInfo;
	unsigned int ClanItemID;

	int eStatusAssistant;//辅助的一些状态
	int ZoneMemID;
	int iZoneConnectID;
	TFRAMEHEAD stFrameHead;
	PetStrengthenUIInfo stPetStrengthenUIInfo;
	int iUsedOfflineExp;//获得经验中包含的离线经验
	AutoMove stAutoMoveInfo;
	UnFengYin stUnFengYin;

	//技能怪的目标先存下,避免外挂重复发包
	int iSkillMonID;//技能怪的id
	unsigned char SkillMonTargetNum;                      
    	int SkillMonTargetIDs[MAX_SKILL_TARGET];  

	int iBeachMapSec;//海滩地图秒数

	int iWorldCityBuffFlag;//时间城争霸战buff标志,对应WORLD_CITY_BUFF_FLAG
	int iWorldCityProcEventFlag;//时间城争霸战地图逻辑区标志,对应WORLD_CITY_PROC_EVENT_FLAG
	int iWorldCityBuildSerial;//时间城争霸战地图建筑占领变化序列号  

	time_t tDropTmpItemEnd;
	int      iDropTmpID;

	int iKillerObjType;//凶手类型,尸检用
	int iKillerMemID;//凶手id,尸检用

	int iShopSendSeq; //商城道具的下发记录，如果记录太多需要分开发
	int iShopCurrPkg;
	int iOldChangeMapID;
	struct timeval stSkillMissTime;		//闪避时间
	VIPBARINFO stVipBarInfo;//vip网吧信息
	RECASTINFO stRecaseInfo;//保存角色装备重铸未确定的结果
	HETIOL stHeti;
	char cClanPos;
	char fRealBiaoChe;
	char fShamDead;
	char fPetShouHu;

	int aiRuneColorCount[RUNE_YELLOW + 1];
	RUNEEXENABLEINFO RuneExEnableInfo[MAX_RUNE_EX];   	//符文额外属性激活信息

	ArmStarOL stArmStarOL;	//1装备强化临时信息
	MapOL stMapOL;
	PetWashGeniusInfo stPetWashGeniusInfo;
	GemReSetOL stGemReSetOL;
	UnrealOL stUnrealOL;

	char szAchievementFlag[MAX_ACHIEVEMENT/8+1];

	AddAttrWashOL stAddAttrWashOL;
	DieCleanOL stDieClean;
	//tdr_ulonglong llPetWidBackup;
	TeamFollowOL stTeamFollow;
	TeamSummonInfo stTeamSummonInfo;
	CSTRANSSELECT stTransSelect;
	SpanOL stSpanOnline;
	int iVigorEx;//额外获得的活力
	int iAddWorldLevelExp;	//1世界经验获得提示需要 
	int iGameNotifyFlag;		//2特殊事件记录标记
	CityFightOL stCityFightOL;
	int iDieNum;			//1被玩家杀死的计数器，切地图或杀死其他玩家后清零
	time_t tCityCltCD;
	ClanSummonInfo stClanSummonInfo;
	char fBusyCheck;
	char bRongluReset;
	int iSpanGoldRate;
	ExtendedOL stExtendedOL;

	tdr_ulonglong iRoleID;
	char cSecondPwd[SECOND_PWD_LEN];//暂时保存二级密码
	int iSecondPwdCmd;//  1 代表 登录   2代表 删除   

	CardBoxOL stCardBoxOL;
	ArmReSetOL stArmResetOL;

	int iPaoShangID;	//跑商id
	int iPaoShangNpcID;   //跑商npcid

	int iSmsCode;	//短信验证码
	int iSmsTimeOut;//超时时间
	char szSmsMobile[NAME_LEN];                	/*   手机号码 */
	int iPetFgtTime;			// 宠物出征时间，用来实现宠物每隔x秒给主人加Buff

	int iWillToSpouseStatus; //对方愿意成为夫妻不，0表示未选择，1表示愿意，2表示不愿意
	tdr_ulonglong ullLoveID;//记录向谁表白的id
	char cPursueName[SECOND_PWD_LEN];//暂时保存表白者名
	
	ShiKongRoleQueueInfo stShiKongQueueInfo;  // 时空深渊队列信息
}PlayerOnline;

typedef struct {
	PetOnline stPetOnline;
	HETIOL stHeti;
	FairyOnline stFairyOnline;
	DYNATTRLIST stDyn;
	long long ullMiscFlag;
	char cSafeLockFalg;
	int iMoveSeq; 
	int iSkillSeq; 
	int iSpanGoldRate; 
	int iSpanMapCamp; //属于跨服野外地图的阵营
}PlayerSyncOnline;

char szWatchPlayerSyncOnline[MAX_PROXY_PLAYER_ONLINE_BUF_LEN - sizeof(PlayerSyncOnline)];


#define PLAYER_ONLINE_DATA 44304

#define  AAS_HOUR 3600
#define  AAS_HALF_HOUR 1800
#define  AAS_QUARTER_HOUR 900

//typedef enum tagRoleLogoutReason
//{
	//ROLE_LOGOUT_CLTREQ = 1, /*玩家主动登出角色*/
	//ROLE_LOGOUT_KICK,
	//ROLE_LOGOUT_ACC,
	//ROLE_LOGOUT_CLOSE,
//}ROLELOGOUTREASON;

typedef struct {
	int iID;
	int iIndex;
	char cType;
	char sRes[3];
} IDIndex;

typedef struct {
	time_t tOnline;
	time_t tOffline;
	time_t tActive;
	time_t tNotify;
} AasTime;

char szWatchChatLink[MAX_SYS_LINK - sizeof(CHATLINK)];

#define PLAYER_ROLE_DATA 108240



enum PLAYER_MISC_FLAG
{
	GRAB_FLAG=1,		/*角色顶号标记*/	
};

// 被顶保护时间(分钟)
#define GRAB_SAFE_TIME 10 
enum ACC_SHARE_STATE
{
	ACC_SHARE_INIT, //初始化状态
	ACC_SHARE_GETING,	//获取db数据中
	ACC_SHARE_FINAL,	//获取结束
	ACC_SHARE_FAIL,	//失败
	ACC_SHARE_INS,	//插入
};

typedef struct 
{
	int iStat;	//状态,对应ACC_SHARE_STATE
	int iDBQureyTime; //db请求的时间
	int iAccMaxRoleLevel; //帐号中最大的角色等级
	ACCSHARE stAccShare; //帐号共享的东东
}AccShareData;

typedef struct
{
	int iMagic;
	PlayerOnline stOnline; //保留角色在线变量数据，例如视野行走相关
	char sRes1[PLAYER_ONLINE_DATA - sizeof(PlayerOnline)]; //预留一定空间，运营时变化
	int iMemID;
	int iBattleMemID;
	int UinFlag;
	int iUinStatus;
	int iCltType; //CLT_PC
	int GMLimitPerm;
	unsigned short unCltProVer;	/*the cs protocol version of client*/
	char eStatus;
	unsigned char ucRoleLogoutReason;
	int iUinCachePos;
	int iIdx; /*idx in player mempool*/
	ConnectInfo stConnectInfo;
	time_t tActive;
	char szAccount[ACCOUNT_LOGIN_NAME_LEN+5];
	char aMacAdd[MAC_ADDR_LEN];
	unsigned char bAasFlag;
	unsigned char bAasIncomeLimt;
	char sRes[2];
	AasTime stAasTime;
	int iAccSilenceEnd; // 帐号禁言结束时间
	int iCltVer;			//客户端版本号
	int iGrabTime;
	ROLEDATA stRoleData;
	char sRes2[PLAYER_ROLE_DATA - sizeof(ROLEDATA)];
	AccShareData stAccShareData;
	int iShowEquipScore;
}Player;

/*if player struct increase little, can restart zone_server without deleting shm*/
#define PLAYER_DATA PLAYER_ONLINE_DATA+PLAYER_ROLE_DATA+256
char szWatchPlayer[PLAYER_DATA - sizeof(Player)];

#define MAX_MAP_INDEX 256

#define MAX_AREA_PROC 16

typedef struct {
		int iTotal;
		int aiIndex[MAX_AREA_PROC];
} AreaProc;

typedef struct tagArea
{
	AreaProc stProc;
}Area;

#define MAX_AREA 10000
#define AREA_DATA 128
char szWatchArea[AREA_DATA - sizeof(Area)];

#define MAX_MASK_DATA 36000000

#ifdef _OPERATION_ENV
#define MAX_DYN_MASK_DATA 80000000
#else
#define MAX_DYN_MASK_DATA 20000000
#endif

#define MAX_PROC_DATA 10240000    

#define MAX_MAPMONSTER_DEF 16000
#define MAPMONSTER_DEF_DATA 32
char szWatchMonsterDef[MAPMONSTER_DEF_DATA-sizeof(MAPMONSTER)];

#define MAX_MAPNPC_DEF 2000
#define MAPNPC_DEF_DATA 128
char szWatchNpcDef[MAPNPC_DEF_DATA-sizeof(MAPNPC)];

#define MAX_PROC_DEF 10000
#define PROC_DEF_DATA 512
char szWatchProcDef[PROC_DEF_DATA-sizeof(PROCEVENT)-sizeof(ScriptIndex)];

#define MAX_AREA_ANIMATE	1000
#define MAX_AREA_INANIMATE      240	

struct tagAreaDyn
{
	int iMapInstID;
	short x;
	short y;
	int iAnimate;
	int aiAnimate[MAX_AREA_ANIMATE];
	int iInanimate;
	int aiInanimate[MAX_AREA_INANIMATE];
	int iDynMskIdx;
};
typedef struct tagAreaDyn		AREADYN;

#ifdef _OPERATION_ENV
#define MAX_AREA_DYN 40000
#else
#define MAX_AREA_DYN 10000
#endif

#define AREA_DYN_DATA 4980
char szWatchAreaDyn[AREA_DYN_DATA-sizeof(AREADYN)];

#define MAX_MAP_AREA 128
#define MAX_MAPINST_SCRI_VAR 16
#define MAX_DYN_REVIVE_POS 4
#define MAX_NEW_NPC 16
#define MAX_DYN_MSK_MON 200

struct tagMapOreInfo{
	int iOreID;
	int iOrePositionIdx;
};
typedef struct tagMapOreInfo	MapOreInfo;

struct tagZoneMapInst
{
	int iMapInstID;
	int iPworldInstID;
	int iMapPos;
	int iMapArea;
	int iAreaDyn;
	int aiAreaDyn[MAX_MAP_AREA];	//区域内存池索引
	int iMonsterCount;
	int iPlayerCount;
	int iIndex;
	int iRealIndex;
	short nEffectID;
	unsigned char bBroadcast; 
	unsigned char cInstScriptFlag;
	char szScriptVar[MAX_MAPINST_SCRI_VAR];
	ScriptIndex stScriptIdx;
	char  Res;
	int aiDynReviveCamp[BATTLE_DYN_REVIVEPOS_NUM];
	int iNewNpc;
	int aiNewNpc[MAX_NEW_NPC];
	int iDynMskMonCount;//一个区域里有两百个动态阻挡怪物
	int aiDynMskMonMemID[MAX_DYN_MSK_MON];
	
	int iOre;
	MapOreInfo astOres[MAX_MAP_ORE];
	time_t tRefreshOreTime[MAX_MAP_ORE_LIST];
	
};
typedef struct tagZoneMapInst	ZONEMAPINST;

#define MAX_MAP_INST 12000
#define MAP_INST_DATA 2000
char szWatchMapInst[MAP_INST_DATA-sizeof(ZONEMAPINST)];

struct tagZoneIdx
{
	int iID;                    
	int iPos;                   
};
typedef struct tagZoneIdx   ZONEIDX;

typedef struct {
	int iNum;
	AREADYN *pastArea[4];
} SearchArea;

#define PWORLD_MAPID_BASE 10000  //10000 以下是主世界地图
#define MAX_PWORLD_MAP 	32
#define MAX_PWORLD_INST 6000

typedef struct {
	tdr_ulonglong ullRoleID;
	int iID;
}PworldTypeSingle;

typedef struct {
	tdr_ulonglong ullTeamID;
	int iTeamIdx;
}PworldTypeTeam;

typedef struct {
	int iID;
	tdr_ulonglong ullGID;
}PworldTypeClan;

typedef struct
{
	int iUin;
	tdr_ulonglong ullRoleID;
	char szRoleName[ROLE_NAME_LEN]; // 邮箱拥有者角色名
	int iVal;
}PworldTypeSpartaTeamMem;

typedef struct {
	tdr_ulonglong ullTeamID;
	int iTeamVal;
	PworldTypeSpartaTeamMem astMem[MAX_SPARTA_TEAM_MEM];
}PworldTypeSpartaTeam;

typedef struct {
	int iTeam1Kill;
	int iTeam2Kill;
	PworldTypeSpartaTeam stTeam1;
	PworldTypeSpartaTeam stTeam2;
}PworldTypeSparta;

typedef union 
{
	PworldTypeSingle stSingle;
	PworldTypeTeam stTeam;
	PworldTypeClan stClan;
	PworldTypeSparta stSparta;
}PworldTypeData;

#define PWORLD_VAR 256

//#define MAX_PWORLD_MEMBER 300 
char szWatchPworldMember[MAX_PWORLD_MEMBER-CLAN_MAX_MEM];

//#define MAX_CAMP_NUM 3
#define MAX_LINKED_MON_SLOT 16

/* 阵营的信息 */
typedef struct tagPworldCampInfo
{
    	int ZoneBusID;                       	/*   此阵营的人是哪个world的,存BusID */
    	char PworldCampID;                    	/*   副本的阵营id,campnum分配过来的,从0开始 */	
}PWORLDCAMPINFO;

typedef struct 
{
    int iMemID;
	int iDefID;
	RESPOS stPos;//坐标
	int iHP;//血量
	int iCamp;
	int iDieTime;//死亡时间
	int iRangePos;//排名索引
}PworldCampMonInfo;

typedef struct 
{
    int iCampLv;
	int iPlayerNum;
	CLANID stClanID;//副本阵营对应的公会gid
	int CampVal;                         	/*   阵营气势 */
	int iSvrBusID;
	int iKillNum;//击杀总数
	PworldCampMonInfo stMapShowMon;
}PworldCampPlayerInfo;

typedef struct 
{
	int  iCampNum;	
    PworldCampPlayerInfo stCampInfo[MAX_CAMP_NUM];
}PworldCamp;

typedef struct 
{
	int iCampVal1;
	int iCampVal2;
	
}PworldXuKongInfo;

typedef struct 
{
	int iFraction;			//1积分
	PworldXuKongInfo stXuKongInfo;
}PworldBattleInfo;

typedef struct 
{
	char szOwner[ROLE_NAME_LEN];
	unsigned char iGridIdx;
	int iRate;
	ROLEGRID stGrid;
}PworldAwardCard;

enum tagPworldAwardState
{
	 PWORLD_AWARD_WAIT = 0,
	 PWORLD_AWARD_ASSIGN,	
	 PWORLD_AWARD_END,  
};

typedef struct 
{
	int iStat;
	int iVipStat;
	time_t tAwardTime;
	int iNormalNum;
	int iJingNum;
	int iBossNum;
	int iDieNum;
	
	int iCardNum;
	int iCardDecRate;
	PworldAwardCard AwardCards[MAX_PWORLD_AWARD_CARD];
	
	int iWaitSelectNum;
	int aiWaiter[MAX_TEAM_MEMBER];	

	int iVipCardNum;
	int iVipCardDecRate;
	PworldAwardCard VipAwardCards[MAX_PWORLD_VIP_AWARD_CARD];
	
	int iVipWaitSelectNum;
	int aiVipWaiter[MAX_TEAM_MEMBER];	

	tdr_ulonglong ullLogWID;
}PworldAwardInfo;

typedef struct 
{
	int iMemID;
	int iOpen;
	
}PworldBoxPlayerInfo;

typedef struct 
{
	int iPlayerNum;
	PworldBoxPlayerInfo astBoxPlayer[MAX_TEAM_MEMBER];
	
}PworldBoxInfo;

#define UNREAL_MON_TYPE_COUNT 20

typedef struct tagUnreal
{
	int iCurrLayer;	// 当前层
	int iMonCount;	// 怪物总数
	int iKillNum;		// 已经干掉的
	int iMon;			// 刷怪数量
	int ID[UNREAL_MON_TYPE_COUNT];		// 刷怪的种类
}UNREAL;


#define PWORLD_MON_ROUND_MAX 64 //最多64 轮波次怪或定时怪

#define AWARD_FLAG_PLAYER 1 //玩家已领奖励标记
#define AWARD_FLAG_CLAN 2 //公会已领奖励标记
#define AWARD_FLAG_WARNING_TIP 4 //已提示预警信息
#define AWARD_FLAG_REFRESH_TIP 8 //已提示刷怪信息
#define AWARD_FLAG_EARLY 16 //时间已提前


typedef struct tagPWorldRoundMonInfo
{	
	int iTotalNum;	//需刷怪总数
	int iCurrNum;			// 已刷怪数量
	int iCountDown;	// 刷新倒计时间
	int iRefreshTime;	//开始刷新时间
	int iRepeatTime;	//重复刷时间
	int iCurrRepeat;	//当前重复刷次数
	int iWaveDefID;		// 波次定义ID
	int iAwardFlag;		//奖励标记,1个人 2公会 4预警提示 8 刷新提示
}PWORLDROUNDMONINFO;


typedef struct tagPWorldRoundMon
{
	int iTotalRound;	// 总共多少轮
	int iCurrRound;	// 当前多少轮
	PWORLDROUNDMONINFO aRoundMonInfo[PWORLD_MON_ROUND_MAX];
}PWORLDROUNDMON;


struct tagZonePworldInst
{
	tdr_ulonglong ullPworldWID; //唯一标识
	int iPworldInstID;
	ZONEIDX stPworldIdx;
	time_t tStart;
	time_t tEnd;
	time_t tNoPlayer;
	int iPlayer;
	int aiPlayerID[MAX_PWORLD_MEMBER];
	int iMapInst;
	int iRealIndex;
	ZONEIDX astMapInstIdx[MAX_PWORLD_MAP];
	int iType;
	PworldTypeData stPworldTypeData;
	ScriptIndex stScriptIdx;
	unsigned char bDelayScript;
	unsigned char bPlayerMeanLevel;
	char cPworldScriptFlag;
	char sScriptVar[PWORLD_VAR];
	unsigned char bBroadCast;
	unsigned char bOver;
	unsigned char DynType;
	unsigned short TmpLevel;
	PWORLDSELECT stPworldselect;
	tdr_ulonglong ullClanGID;
	int iNextPworldInstID;	//活动副本 同时开启多个，链起来。
	int iWaitQueueIdx;
	int iDownTm;			//倒计时	
	int iMonHpTotal;
	int iMapCurr;
	PworldCamp stPworldCamp;
	PworldBattleInfo stPworldBattleInfo;
	PworldAwardInfo stAwardInfo;
	char chLimitFlag;//0不加每日限制次数,1加每日限制次数
	UNREAL stUnreal;
	int iNextUnrealInstID;
	PWORLDROUNDMON stTimerRound;//定时怪信息
	PWORLDROUNDMON stWaveRound;//波次怪信息
	int iStrongEnd;
	int iStrongFlag;
	int iClanSpyFlag;	
	PworldBoxInfo stPworldBoxInfo;
	int iAwardMul;//经验绑银奖励修正,分母100
	char cShareShuNiu;
	int iDianFengRound;//末世巅峰活动pk场次
	time_t tYanChiTime;//末世巅峰副本延迟时间处理
	int iHomeID;
	int iShiKongWaitQueueIdx;  // 时空深渊等待队列索引
};
typedef struct tagZonePworldInst	ZONEPWORLDINST;

#define PWORLD_INST_DATA 18000
char szWatchPworldInst[PWORLD_INST_DATA-sizeof(ZONEPWORLDINST)];

#define MAX_MONSTER_VPLAYER_NORM 56	
#define MAX_MONSTER_VPLAYER_IMP	8
#define MAX_MONSTER_VPLAYER	(MAX_MONSTER_VPLAYER_NORM+MAX_MONSTER_VPLAYER_IMP)

#ifdef _OPERATION_ENV
#define MAX_MONSTER 60000
#define MAX_NPC 26000
#else
#define MAX_MONSTER 20000
#define MAX_NPC 3000
#endif

#define MONSTER_DIE	0
#define MONSTER_ALIVE 1

#define PWORLD_MAPID	100
#define GET_PWORLDID(MapID)	\
	(((MapID) & 0xffff) < PWORLD_MAPID_BASE ? 0 : ( ((MapID) & 0xffff)/PWORLD_MAPID ))
#define MAKE_MAPINDEX(MapID, PworldInst)		( (MapID) | ((PworldInst)<<16) )
#define GET_MAPID(MapIndex)					( (MapIndex) & 0xffff )
#define GET_PWORLDINST(MapIndex)				( (MapIndex) >> 16 )

#define BASE_CITY_MAPID	1000

#define MEMPOOL_SIZE(max, unit) sizeof(TMEMPOOL) - 1 + max*TMEMPOOL_ROUND_UNIT(unit + sizeof(TMEMBLOCK) - 1)

typedef struct
{
	int iShmVersion;
	char sData[1];
}PlayerShm;

typedef struct tagZoneSvrEnv		ZONESVRENV;
typedef struct tagZoneSvrEnv		*LZONESVRENV;


typedef int (*CmdHandle)(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

typedef struct
{
	char cCtrlFlag;
	char cStateLimit;
	short nRes;
	CmdHandle pfnCmdHandle;
}CmdMap;

typedef struct
{
	unsigned short unSkillID;
	unsigned short nActiveNum;
}DeepSkillActiveInfo;	

typedef struct {
	unsigned char bGoodStatusNum;
	unsigned char bBadStatusNum;
	unsigned short unRes;
	
	ZoneStatus astGoodStatus[MAX_MON_GOOD_STATUS];
	ZoneStatus astBadStatus[MAX_BAD_STATUS];
}ZoneMonStatusList;

typedef enum 
{
	AI_STAT_NOBIRTH = 0,  //未出生
	AI_STAT_IDLE, //原地空闲
	AI_STAT_DEAD, //死亡
	AI_STAT_ATK, //攻击
	AI_STAT_MOVE, //移动
	AI_STAT_HOME, //归位
	AI_STAT_TRACE, //追敌
	AI_STAT_RUNAWAY,//怪物逃跑
	
}MONSTER_AI_STAT;

#define MAX_PRESS_ENTRY 16

typedef struct {
	int iTargetID;
	int iPress; //压力值
}PressEntry;

typedef struct {
	int iTargetCur; //当前目标
	int iOldTargetID;
	time_t tTargetSelect;

	int iMarkedID; //被标记者ID
	time_t tMarkedEnd; // 标记结束时间
	
	int iDefyID; //嘲讽者id
	time_t tDefyEnd; //嘲讽结束时间

	int PressNum; //压力表中的有效个数
	PressEntry astPressEntrys[MAX_PRESS_ENTRY];
}AtkTargetList;

struct tagMonster;
struct tagNpc;

typedef struct {
	Player *pstOwner;
	PetFightShiOnline *pstPetFightShi;
	ROLEPET *pstRolePet;
}ZonePet;

typedef union 
{
	Player* pstPlayer;
	struct tagMonster* pstMon;
	ZonePet stZonePet;
	struct tagNpc *pstNpc;
}ZoneAniData;

typedef struct {
	int iType;
	ZoneAniData  stObj;
}ZoneAni;


#define MAX_ATKEE_NUM 500 

typedef struct{
	int iObj;
	ZoneAni  astObj[MAX_ATKEE_NUM];
}ZoneObjList;


typedef struct{
	ZoneAni stObj;
	int iDebuffNum;//用来记录debuf的个数
}TargetAni;
typedef struct{
	int iObj;
	TargetAni  astObj[MAX_SKILL_TARGET];//有debuf的目标
}DebuffObjList;


typedef struct {
	int iAtkeeID;
	int iSkillIdx;
	unsigned short unSkillID;
	unsigned char bSkillLevel;
	char cAtkStat; //CS_STAT_PREPARE, CS_STAT_ATK
	struct timeval stAtkStatEnd;	
	RESPOS stAtkPos;//技能攻击的点
	int iSkillSeq;
	unsigned short DirFace;//"怪物面向攻击点的朝向偏移,用来做背击
	struct timeval  stLastAtk;
	
	unsigned char bSkillType; //0, normal skill; 1 insert skill; 2 atkee skill
	unsigned char bMonSkillIdx;
	char cPilotEnd; //4				引导结束标记
	char sRes[1];
	
	unsigned short unPilot; //4		引导次数
	unsigned char bNextSkillIdx; //4	后续技能索引, 标示即将释放主体技能下的第几个后续。
	unsigned char bHeadSkillIdx;    //4主题技能在NormalSkill 中的位置(目前只有NormalSkill 有后续技能)
	
	struct timeval stLastPilot;
	int fIsSpecificTarget; // 已经指定了一个特定目标

	//移动目标点
	RESPOS stMovePos;
	
}MonAtkInfo;

typedef struct {
	AINORMALSKILL stSkill;
	int iSkillIdx;
	struct timeval stCoolEnd;
	int iOnce;
}MonNormalSkill;

typedef struct {
	AIINSERTSKILL stSkill;
	int iSkillIdx;
	struct timeval stCoolEnd;
	int iOnce;
}MonInsertSkill;

#define MAX_TISHEN_MEM 5

typedef struct {
	int iTeamIdx;
	tdr_ulonglong ullTeamID;
}MonTeamInfo;

typedef struct {
	int iBctDefID;
	int iBctDefIdx;
	int iBctMonIdx;
}MonBctInfo;

enum tagMonCommUnionType
{
	 MON_UNION_TYPE_NONE = 0,
	 MON_UNION_TYPE_MACHINE = 1,	
	 //MON_UNION_TYPE_ROUNDROLL_MON = 2,	 //满地打滚相对于圆心(出生点坐标)的角度(弧度)
	 MON_UNION_TYPE_XUNLUO_MON = 3,	     //巡逻怪才有的数据
	 MON_UNION_TYPE_FACTORY = 4,	     		//4公会工厂
	 MON_UNION_TYPE_RONGLU = 5,
	 MON_UNION_TYPE_LOSTTOTEM = 6,	   
	 MON_UNION_TYPE_WORD_PWD = 7,    //  文字密码
};

typedef struct {
	int a; //加速度
}XunLuoMon;

typedef struct {
	MONMACHINEINFO stMachineInfo; 	//4机械信息(乘客、属性)
	int iNoIdle;
	int iDynAttrMul;
}MachineData;

typedef struct {
	tdr_ulonglong ullClanGID;
	int iClanID;
	time_t tBuildTime;
	time_t tNotify;
}MonFactoryData;

typedef struct {
	char OwnerName[NAME_LEN];
	int iPower;	// 收集能量数
	int iMaxPower;
	int MonIDs[RONGLU_KILL_MON_MAX];
}MonRonglu;

typedef struct {
	int iPower;	
	int iMaxPower;
}MonLostTotem;

typedef struct {
	int iLine;	 // WordPwdDef 的id
	int iIndex; // 第几个
}MonWordPwd;

typedef union {
	XunLuoMon stXunLuoMon;
	MachineData stMachine;
	MonFactoryData stFactory;
	MonRonglu stRonglu;
	MonLostTotem stLostTotem;
	MonWordPwd stWordPwd; //  文字密码
}MonCommUnion;

#define MAX_MON_FOLLOW 6

typedef struct tagMonster{
	int iID;
	int iDefIdx;
	int iDefPos;
	int iMIndex; 
	int iInitMap;
	int iInitMapPos;
	RESPOS stInitPos;
	MONDYNATTR stMonDynAttr; 
	unsigned short unViewDist;
	unsigned short unPressSelectAdd;
	MonNormalSkill astNormalSkill[MAX_NORM_SKILL];
	MonInsertSkill astAtkerInsertSkill[MAX_ATKER_SKILL];
	MonInsertSkill astAtkeeInsertSkill[MAX_ATKEE_SKILL];
	unsigned short unMinAtkSkillDist;
	unsigned short unMinAtkSkillCool; //单位100 ms
	unsigned short unTraceDist;
	unsigned short unRadius;
	ScriptIndex stScriptIdx;
	int cMonScriptFlag;
	char cType;// npc, monster
	char cRefresh;
	int iOwnerID; // 图腾、地雷等的拥有者
	int iOwnerPKMode; //拥有者的PK模式
	unsigned long long ullOwnerApplyID; // 队伍id,或工会id	
	int iCallMonID; //召唤它的怪物ID 
	int iPatrolID; //巡逻路径ID
	int iPatrolIdx;
	int iFormDefID;	// 变异后，记录从那个怪物变异过来。死亡则恢复原怪物刷新
	char szOwner[ROLE_NAME_LEN_SAVE];
	MonTeamInfo stTeamInfo;
	MonBctInfo stBctInfo;
	unsigned char bDirection;
	unsigned char bAtkType;
	unsigned char bMoveType;
	unsigned char bRandSelect;
	unsigned char bSubType;
	char cAtkInsert;
	int cMonMiscFlag; //MON_MISC_FLAG_MAPBOAD
	char cOwnerIDType; // 1 表示队伍
	int iLinkedPrev; // 上一个被链接的怪物
	int iLinkedNext; // 下一个被链接的怪物
	int iGroupID; // 组id,0表示怪物没有组
	int iCampIdx;  //4阵营信息(跟特定场景相关，阵营信息目前只是某个战场副本中的阵营)
	int iMaxHpbyCall;//召唤时给的hp,参考

	//怪物也具有人一样的PK信息，只可能存在一种
	int iPKType;	//4
	tdr_ulonglong ullPKID;

	int nOwnerFastID;	//2快速查找公会
	tdr_ulonglong ullBootyClanGID;		//战利品属于特定公会 
	int iCityBuildDefID;	//2城市建筑定义ID
	int iKillPlayerNum;	//2世界BOSS杀死玩家计数

	time_t tLifeEnd;
	
	// 一些公用体就放这里面来吧
	// 刷怪的时候，如果不冲突的都放里面
	int iCommType;
	MonCommUnion stCommUnion; 
	char cDartCarNotAtk;	// 商运镖车能否被攻击 1-不可以2-可以
	tdr_ulonglong WID;   
	tdr_ulonglong FormMonDropWID; //从哪个怪掉落的,  宝箱等用
	int iFromMonDropLevel; // 从哪个怪掉落的，哪个怪物的等级， 宝箱等用
	char cFirstBorn; //keep this var the last var of basic mon var
	
	//online monster var
	int iHP;
	int iMaxHP;

	short iEmon;
	short iEplayer;
	short iFplayer;
	short iImpMon;
	int iVPlayer;
	union {
	int aiVPlayer[MAX_MONSTER_VPLAYER];
	int aiVAnimate[MAX_VISIBLE_ANIMATE];
	};
	
	ZONEIDX stMap;
	
	RESPOS stCurrPos;
	RESPOS stNextPos;
	RESPOS stForcedPos; // 强制移动目的地坐标
	RESPOS stLastSearchPos;
		
	struct timeval stLastMoveTv;
	struct timeval stLastSearchTv;
	time_t tRefresh;
	time_t tBirthEnd;
	time_t tLeaveMap;
	
	char cStat;
	char cPatrolScript;
	char bFollowTry;
	char sRes;
	
	ZoneMonStatusList stStatusList;

	CSMSPDINFO stMspdInfo;
	char cMoving;
	char cDoSearch;

	unsigned char bAiStat;
	unsigned char bTraceTry;

	MonAtkInfo stMonAtkInfo;
	AtkTargetList  stAtkTargets;
	time_t tLastTrace;
	time_t tFreezeEnd; //僵硬时间

	char cFirstAtkerType; 
	int iFirstAtkerID; //优先者, 用于怪物战利品分配
	tdr_ulonglong ullFirstAtkerWid;
	time_t tLastAtkTime; //被攻击的时间

	//记录怪物被攻击的信息用于怪物ai 寻路
	int iAtkerID;
	RESPOS stAtkerPos;
	char  cAtkerInfoNew;

	unsigned char bPatrolPos; 
	unsigned char bPatrolTurn; //0 正转，1 反转
	unsigned char bPatrolTry; 
	unsigned char bDelayScript; //为避免脚本嵌套
	char cMonScriptVar[MAX_MON_SCRIPT_VAR];

	struct timeval stAIBeAtkSKill;

	float fRoundRollAngle; // 满地打滚相对于圆心(出生点坐标)的角度(弧度)
	
	int iTotemSkillID;
	int iTotemNext;
	time_t tLastPressUpdate;
	int iPlayerTotemNext;

	int tRunAwayStart; //AI_STAT_RUNAWAY状态开始的时间,单位秒
	int iRunAwayTime;//保持AI_STAT_RUNAWAY状态的持续时间 单位秒

	int iCombatTimeCount; // 战斗时间计数
	int iSkillAtkerID;	
	
	int iOwnerMagic;		//1继承数值

	unsigned char abFollowAngel[FOLLOW_ANGEL_DIV];
	int iKillerObjType; //凶手类型,尸检用
	int iKillerMemID;	//凶手Memid,尸检用
	int iWarningTime;	//公会建筑受攻击时，发出攻击警报时间
	int iWarningMul;			//公会采集场血量提示
	int iFirstAtkerMemID;
	int iFollow;
	int astFollow[MAX_MON_FOLLOW];
	char cSecenOverLay;
	char cRonglu;
	char cBannerEffect;
	int iCallIceDefAdd;
	int iCallPhsicDefadd;

	int iRongYanTeamCapId;	 //队长id	融焰巨兽中使用                          
} Monster;

#define MONSTER_DATA 3804
char szWatchMonster[MONSTER_DATA-sizeof(Monster)];


typedef struct tagNpc{
	Monster stMon; //mon must first var
	char sMonRes[MONSTER_DATA - sizeof(Monster)];

	char szScriptName[32];
	ScriptIndex stScriptIdx;
	short nTransListID;
	short nTransListIdx;
	short nSellListID;
	short nSellListIdx;

	int iNpcInfoIdx;

	char cScriptFlag;
	char cRes[3];

} Npc;

#define NPC_DATA 3864
char szWatchNpc[NPC_DATA-sizeof(Npc)];

#define ID_INDEX			700007

#define MIN_PWORLD_ITEM 50

#define MAX_ROLE_BOOTY 8

typedef struct {
	tdr_ulonglong ullRoleID; 
	unsigned char bNum;
	char sRes[3];
	ROLEGRID astGrid[MAX_ROLE_BOOTY];
}ZoneRoleBooty;

typedef struct {
	int iRoleNum;
	ZoneRoleBooty astRoleBooty[MAX_TEAM_MEMBER];
}ZoneBootyPak;

typedef struct {
	int iEnerge;
	int iSkill_1_collectnum;
	int iSkill_2_collectnum;	
	int iPatrolID;//如果有路径的话这个值大于0,临时道具才有用
	int iPlayerMemID; //属于哪个玩家的临时道具, >0的时候有用
	char OwnerName[ROLE_NAME_LEN_SAVE];   //所属玩家的名字
	int iStatus; //拾取道具时给的状态
	int iStatusLevel;//拾取道具时给的等级
	int iLast;//拾取道具时给的持续的时间
}ZoneTmpItem;

typedef struct {
	ROLEGRID stGrid;
	tdr_ulonglong ullRoleID; 
	char bViewType;					//1可见者类型
	char bType;   					//1掉落显示类型
	char bRoll;						//1Roll点状态
	char bCollectTime;				//1需要采集的时间
	tdr_ulonglong ullTeamID;
	char szShowFlag;					//客户端表示标记(0延迟播放;1立即播放)
	int iFromID;
	tdr_ulonglong ullClanGID;
}ZoneBootyGrid;

typedef union {
		ZoneBootyGrid stBootyGrid;
		ZoneTmpItem stTmpItem;
} OneItem;

#define DROP_ITEM_NOSTAY 0
#define DROP_ITEM_STAY 1

#define TMP_ITEM_FLAG_MAP_REFRESH 0x1	//MapTmpItemDef刷新的临时道具
//#define ITEM_RONGLU_DROP 0x2

typedef struct {
	char cType;
	char cStat;
	unsigned short unRefreshTime;
	OneItem stObj;
	int iDefIdx;
	int iID;
	ZONEIDX stMap;
	RESPOS stPos;
	int  iStayTime;
	struct timeval stDropTime;
	int iMIndex;
	tdr_ulonglong ullTeamID;
	int iFromID;
	unsigned char bDir;
	time_t tPickLimitTime;		//1限制时间，限制时间内只有拥有者可以拾取
	int iTmpItemFlag;	//标记,pick和drop时需要和RoleTmpSys.ItemFlag保持一致,对应TMP_ITEM_FLAG_XXX
	int iLeaveMapTime;
} DropItem;

typedef struct {
	char szName[ROLE_NAME_LEN];	
	int iID;
} NameMemID;

typedef struct {
	char szLoginName[ACCOUNT_LOGIN_NAME_LEN+5];
	int iID;
} LoginName2MemID;

typedef struct {
	tdr_ulonglong ullID;
	int iIdx;
} WID2Team;

#define TEAM_BUCKET (MAX_TEAM * 3)

#define MAX_ITEM 100000
#define DROP_ITEM_DATA 336
char sWatchItemData[DROP_ITEM_DATA-sizeof(DropItem)];

#define MAX_BIRTHMAP_DEF 8 
#define BIRTHMAP_DEF_DATA 48
char sWatchBirthMapData[BIRTHMAP_DEF_DATA-sizeof(BIRTHMAPDEF)];

#define ACTION_STATUS_DEF_DATA 176
char sWatchActionStatusData[ACTION_STATUS_DEF_DATA-sizeof(ACTIONSTATUS)];

#define LEVELUP_DEF_DATA 384
char sWatchLevelUpData[LEVELUP_DEF_DATA-sizeof(LEVELUP)];

#define MAX_ARM_LEVEL_HURT 3000
#define ARM_LEVEL_HURT_DATA 12
char sWatchArmLevel[ARM_LEVEL_HURT_DATA - sizeof(ARMLEVELHURTDEF)];

typedef struct {
	int iAchievementID; //技能关联的成长之路ID
} SkillDefTail;

#define MAX_SKILL 3500
#define SKILL_DATA 2400
char sWatchSkill[SKILL_DATA - sizeof(SKILLDEF) - sizeof(SkillDefTail) - 16];

#define MAX_DAILY_GOAL_AWARD 16
#define DAILY_GOAL_AWARD_DATA 64
char sWatchDailyGoalAwardData[DAILY_GOAL_AWARD_DATA - sizeof(DAILYGOALAWARD)];


#define SCRIPT_SYSVAR_SIZE 40960
#define SCRIPT_MINI_NUM 4000
#define SCRIPT_MIDDLE_NUM 800
#define SCRIPT_LARGE_NUM 400
#define SCRIPT_HUGE_NUM	100
#define SCRIPT_TOTAL_NUM	(SCRIPT_MINI_NUM+SCRIPT_MIDDLE_NUM+SCRIPT_LARGE_NUM+SCRIPT_HUGE_NUM)
#define SCRIPT_MINI	1
#define SCRIPT_MIDDLE	2
#define SCRIPT_LARGE	3
#define SCRIPT_HUGE	4
#define SCRIPT_MINI_SIZE	2048
#define SCRIPT_MIDDLE_SIZE	5120
#define SCRIPT_LARGE_SIZE	10240
#define SCRIPT_HUGE_SIZE	64000
#define DIALOG_STAT_BEGIN 0
#define DIALOG_STAT_ANSWER 1
#define DIALOG_STAT_NEXT 2
#define DIALOG_STAT_OPT 3
#define DIALOG_STAT_COMMIT_OPT 4

#define DIATHESIS_DATA 1280
char sWatchDiaThesis[DIATHESIS_DATA - sizeof(DIATHESISDEF)];
 
#define FITTINGS_DATA 1500
char sWatchFittings[FITTINGS_DATA - sizeof(FITTINGSDEF)];


typedef struct
{
	char sMap[MAX_STATUS_RESULT_VAL/8+1];
	char sMapIdx[MAX_STATUS_RESULT_VAL];
}StatusResultMap;

#define MAX_STATUS 2200
#define STATUS_DATA 2000
char sWatchStatus[STATUS_DATA - (sizeof(STATUSDEF)+sizeof(StatusResultMap))];

#define MAX_OVERLAYTYPE_RELATION 320 
#define OVERLAYTYPE_RELATION_DATA 16
char sWatchOverlayTypeRelation[OVERLAYTYPE_RELATION_DATA - sizeof(OVERLAYTYPERELATIONDEF)];

#define MAX_STATUS_OVERLAY 400
#define STATUS_OVERLAY_DATA 8
char sWatchStatusOverlay[STATUS_OVERLAY_DATA - sizeof(STATUSOVERLAYDEF)];

#define MAX_ADDATTR_DEF 1024
#define ADDATTR_DEF_DATA 450
char sWatchAddAttr[ADDATTR_DEF_DATA - sizeof(ADDATTRDEF)];

#define MAX_SUITSET_DEF   280
#define SUITSET_DEF_DATA   128
char sWatchSuit[SUITSET_DEF_DATA - sizeof(SUITSET)];

#define MAX_TEAM  MAX_PLAYER 
#define TEAM_DATA 5784

#define MAX_TEAM_PWORLD_INST 7

typedef struct {
	int iPworldID;
	int iPworldInstID;
	tdr_ulonglong ullPworldWID; //唯一标识
}TeamPworldInfo;

typedef struct {

	int iMaxRoll;									//1当前最大Roll点数									
	tdr_ulonglong ullWinRoleID; 					//1当前最大roll点的人的RoleID
	char szWinRoleName[ROLE_NAME_LEN];			//1角色名，提示需要用，角色有可能下线了
	
	int iWaitNum;
	int iWaitRoller[MAX_TEAM_MEMBER];				//1等待Roll点的队友(Roll或者放弃则清除此人，防止多次Roll)
	
}TeamBootyRollInfo;

typedef struct {
	
	int iBootyID;								//1战利品ID
	time_t tEndTime;							//1Roll点倒计时，时间到了给stBootyRollInfo 所有权(所有人都没选为0，则视为人人可拾取)	
	TeamBootyRollInfo stBootyRollInfo;
}TeamBootyInfo;

#define MAX_ROLL_TEAM_BOOTY  8

typedef struct {
	TEAMINFO stTeamInfo;
	int iTeamIdx;
	int iPworldInstID;
	tdr_ulonglong ullPworldWID;
	struct timeval stQiYueLastTv; // 契约生效的时间
	int iQiYueIdx;
	int iPworldNum;
	TeamPworldInfo stPworldInfo[MAX_TEAM_PWORLD_INST];
	AutoTeamInfo stAutoTeamInfo;
	time_t tShadowAlive; // 仅仅在battle中有效,表示shadow team info的存活时间
	int iTeamMapLine;	//4地图线路
	
	int iTeamBootyNum;
	TeamBootyInfo astTeamBooty[MAX_ROLL_TEAM_BOOTY];

	int eStatusAssistant; //队伍的状态
	int BattleBusID; /* >0表示队伍跨服了,要同步这个数据到指定的battle */
	int ZoneBusID;//自己服的busid
	tdr_ulonglong ullZoneTeamID;
	int iZoneTeamIdx;
	int iAutoPworldID;
	int iAutoPworldInstID;
	int iAutoPworldStart;

	//lzk  融焰巨兽相关
	int  iRongYanMonId;   //怪物id
	int iRongYanCurLevel;//当前层数	(记得更新)
	int iRongYanSelectMul; //选择的倍数索引 只能取1,2,3,4
	int iRongYanStatus;//1表示正在打怪，0表示没有
	int tRongYanBossSelectTimeEnd;//boss 选择 3分钟后系统自动选择
	int iRongYanBegin;//记录融焰活动开始与否  1表示正在进行， 0表示没有
	int iRongYanOldLevel;//给打完假怪后世界播报用   记录打真怪的最后一层
	RESPOS stRongYanPos;//刷熔岩怪的坐标
	ZONEIDX stRongYanZoneIdx;//记录刷出熔岩怪的zoneIdx   为了在刷假怪时 进行世界播报  
}ZoneTeam;

char sWatchTeamData[TEAM_DATA - sizeof(ZoneTeam)];

#define MON_DYN_ATTR_DATA 128
char sWatchMonDynAttrData[MON_DYN_ATTR_DATA - sizeof(MONDYNATTRDEF)];

#define MAX_PATROL 600
#define PATROL_DATA 384
char sWatchPatrolData[PATROL_DATA - sizeof(PATROLDEF)];

typedef struct { 
	char szStr[16];
	int iLen;
	char sData[MAX_GD_GIF];
}GdCache;

#define MAX_GD_CACHE 100
#define GD_DATA MAX_GD_GIF+32
char sWatchGdCache[GD_DATA - sizeof(GdCache)];

#define MAX_TASK 3000
#define TASK_DATA 5632
char sWatchTaskData[TASK_DATA-sizeof(TASKDEF)];

#define MAX_RAND_TASKID 400

typedef struct {	
	int iNum;
	int aiTaskID[MAX_RAND_TASKID];
}TaskCycleRandTaskID;

#define MAX_TASK_CYCLE 48
#define TASK_CYCLE_DATA 32000
char sWatchCycleData[TASK_CYCLE_DATA-sizeof(TASKRECYCLEDEF)-sizeof(TaskCycleRandTaskID)];

#define NPC_INFO_DATA 256
#define MAX_NPC_INFO 256

#define PET_DATA 640
#define MAX_PET 512
char sWatchPetData[PET_DATA - sizeof(PETDEF)];

#define PETLEVEL_DATA 64
#define MAX_PETLEVEL MAX_PET_LEVEL
char sWatchPetLevelData[PETLEVEL_DATA - sizeof(PETLEVELDEF)];

#define PET_TRAIT_DEF_DATA 384
#define MAX_PET_TRAIT_DEF 100
char sWatchPetTraitData[PET_TRAIT_DEF_DATA - sizeof(PETTRAITDEF)];

#define PET_STRENGTHEN_DEF_DATA 256
#define MAX_PET_STRENGTHEN_DEF 24
char sWatchPetStrengthenDefData[PET_STRENGTHEN_DEF_DATA - sizeof(PETSTRENGTHENDEF)];

#define QIYUE_DEF_DATA 1024
#define MAX_QIYUE  128
char sWatchQiYueDefData[QIYUE_DEF_DATA - sizeof(QIYUEDEF)];

#define MONDYNEXPLIMIT_DATA 64
#define MAX_MONDYNEXPLIMIT  MAX_LEVEL
char sWatchMonDynExpLimitData[MONDYNEXPLIMIT_DATA - sizeof(MONDYNEXPLIMITDEF)];

#define ARMGRADE_DATA 1024
#define MAX_ARMGRADE_DEF 1024
char sWatchArmGradeData[ARMGRADE_DATA - sizeof(ARMGRADEDEF)];

#define ARMRAND_DATA 64
#define MAX_ARMRAND_DEF 128
char sWatchArmRANDData[ARMRAND_DATA - sizeof(ARMRANDDEF)];

#define ARMHIDEATTR_DATA 512
#define MAX_ARMHIDEATTR_DEF 128
char sWatchArmHideAttrData[ARMHIDEATTR_DATA - sizeof(ARMHIDEATTRDEF)];

#define ARMFENJIE_DATA 128
#define MAX_ARMFENJIE_DEF 256
char sWatchArmFenJieData[ARMFENJIE_DATA - sizeof(ARMFENJIEDEF)];

#define BIRTH_ITEM_DEF_DATA 128
#define MAX_BIRTH_ITEM_DEF 8
char sWatchBirthItemData[BIRTH_ITEM_DEF_DATA - sizeof(BIRTHITEMDEF)];

#define MONSTER_SPEECH_DEF_DATA 3096 
#define MAX_MONSTER_SPEECH_DEF 512 
char sWatchMonsterSpeechData[MONSTER_SPEECH_DEF_DATA - sizeof(MONSTERSPEECHDEF)];

#define ARM_ADDATTR_DEF_DATA 576
#define MAX_ARM_ADDATTR_DEF 256
char sWatchArmAddAttrData[ARM_ADDATTR_DEF_DATA - sizeof(ARMADDATTRDEF)];

#define ONLINE_AWARD_DEF_DATA 8
#define MAX_ONLINE_AWARD_DEF 20
char sWatchOnlineAwardData[ONLINE_AWARD_DEF_DATA - sizeof(ONLINEAWARDDEF)];

#define  TASK_LIMIT_GROUP_DATA 16
#define  MAX_TASK_LIMIT_GROUP 32
char sWatchTaskLimitGroupData[TASK_LIMIT_GROUP_DATA - sizeof(TASKLIMITGROUP)];

#define MAX_CLAN_CITY_BUILDING_LEVEL_DEF 35
#define CLAN_CITY_BUILDING_LEVEL_DEF_DATA 300
char sWatchClanCityBuildingLevelDefData[CLAN_CITY_BUILDING_LEVEL_DEF_DATA - sizeof(CLANCITYBUILDINGLEVELDEF)];

#define MAX_CLAN_CITY_LEVEL_DEF 6
#define CLAN_CITY_LEVEL_DEF_DATA 2000
char sWatchClanCityLevelDefData[CLAN_CITY_LEVEL_DEF_DATA - sizeof(CLANCITYLEVELDEF)];

#define MAX_CLAN_LEVEL_DEF 6
#define CLAN_LEVEL_DEF_DATA 800
char sWatchClanLevelDefData[CLAN_LEVEL_DEF_DATA - sizeof(CLANLEVELDEF)];

#define MAX_MONSTER_COLOR_DEF 1
#define MONSTER_COLOR_DEF_DATA 128
char sWatchMonsterColorDefData[MONSTER_COLOR_DEF_DATA - sizeof(MONSTERCOLORDEF)];

#define MAX_GAME_EVENT_DEF 200
#define GAME_EVENT_DEF_DATA 128
char sWatchGameEventDefData[GAME_EVENT_DEF_DATA - sizeof(GAMEEVENTDEF)];

#define MAX_DESIGNATION_DEF 200
#define DESIGNATION_DEF_DATA 1024
char sWatchDesignationDefData[DESIGNATION_DEF_DATA - sizeof(DESIGNATIONDEF)];

#define MAX_RESPACT_PARTY_DEF 128
#define RESPACT_PARTY_DEF_DATA 800
char sWatchRespactPartyDefData[RESPACT_PARTY_DEF_DATA - sizeof(RESPACTPARTYDEF)];

#define MAX_RESPACT_PHASE_DEF 128
#define RESPACT_PHASE_DEF_DATA 800
char sWatchRespactPhaseDefData[RESPACT_PHASE_DEF_DATA - sizeof(RESPACTPHASEDEF)];

#define MAX_RIDE_DEF 4096
#define RIDE_DEF_DATA 512
char sWatchRideDefData[RIDE_DEF_DATA - sizeof(RIDEDEF)];

#define MAX_FAIRY_DEF 256
#define FAIRY_DEF_DATA 512

char sWatchFairyDefData[FAIRY_DEF_DATA - sizeof(FAIRYDEF)];

char sWatchNpcInfoData[NPC_INFO_DATA-sizeof(NPCINFODEF)];

#define MAX_ADD_ATTR_BASE_DEF 20
#define ADD_ATTR_BASE_DEF_DATA 128
char sWatchAddAttrBASEDefData[ADD_ATTR_BASE_DEF_DATA - sizeof(ADDATTRBASEDEF)];

#define MAX_ADD_ATTR_GROUP_DEF 3072
#define ADD_ATTR_GROUP_DEF_DATA 512
char sWatchAddAttrGroupDefData[ADD_ATTR_GROUP_DEF_DATA - sizeof(ADDATTRGROUPDEF)];

#define MAX_GIFT_LV_DEF 32
#define GIFT_LV_DATA 512
char sWatchGiftLvData[GIFT_LV_DATA - sizeof(LVGIFT)];

#define MAX_GIFT_MEDIA_DEF 32
#define GIFT_MEDIA_DATA 1600
char sWatchGiftMediaData[GIFT_MEDIA_DATA - sizeof(MEDIAGIFTDEF)];

#define MAX_GIFT_LTR_DEF 32
#define GIFT_LTR_DATA 1200
char sWatchGiftLtrData[GIFT_LTR_DATA - sizeof(LTRGIFTDEF)];

#define MAX_GIFT_ONEDAY_DEF 64
#define GIFT_ONEDAY_DATA 1024
char sWatchGiftOnedayData[GIFT_ONEDAY_DATA - sizeof(ONEDAYGIFT)];

#define MAX_FIXED_ADD_ATTR 128
#define FIXED_ADD_ATTR_DATA 80
char sWatchFixedAddAttrData[FIXED_ADD_ATTR_DATA - sizeof(FIXEDADDATTR)];

#define SHOP_DEF_DATA 320
#define MAX_BUCKET_SHOP_RECORD   MAX_SHOP_DEF_LINES+50
char sWatchShop[SHOP_DEF_DATA - sizeof(SHOPDEF)];

#define MAX_VIP_BAR_DEF_LINES	50000

#define VIP_BAR_DEF_DATA 64
#define MAX_BUCKET_VIP_BAR_RECORD   MAX_VIP_BAR_DEF_LINES+50
char sWatchVipBar[VIP_BAR_DEF_DATA - sizeof(VIPBARDEF)];

#define MAX_VIP_BAR_LEVEL_DEF_LINES	10

#define VIP_BAR_LEVEL_DEF_DATA 32
#define MAX_BUCKET_VIP_BAR_LEVEL_RECORD   MAX_VIP_BAR_LEVEL_DEF_LINES+50
char sWatchVipBarLevel[VIP_BAR_LEVEL_DEF_DATA - sizeof(VIPBARLEVELDEF)];

#define MAX_VIP_BAR_NUM_DEF_LINES	16

#define VIP_BAR_NUM_DEF_DATA 32
#define MAX_BUCKET_VIP_BAR_NUM_RECORD   MAX_VIP_BAR_NUM_DEF_LINES+50
char sWatchVipBarNum[VIP_BAR_NUM_DEF_DATA - sizeof(VIPBARNUMDEF)];

#define MAX_VIP_BAR_ONLINE_LINES	MAX_VIP_BAR_DEF_LINES

#define VIP_BAR_ONLINE_DATA 32
#define MAX_BUCKET_VIP_BAR_ONLINE_RECORD   MAX_VIP_BAR_ONLINE_LINES+50
char sWatchVipBarOnline[VIP_BAR_ONLINE_DATA - sizeof(VIPBARONLINE)];

#define MAX_DYN_ACTIVE_STATUS_LINES	256

#define DYN_ACTIVE_STATUS_DATA 256
#define MAX_BUCKET_DYN_ACTIVE_STATUS_RECORD   MAX_DYN_ACTIVE_STATUS_LINES+50
char sWatchDynActiveStatus[DYN_ACTIVE_STATUS_DATA - sizeof(DYNACTIVESTATUS)];

typedef struct
{
	int iAchievementID;
}CONDIDMAP;

#define MAX_COND_ID_MAP_LINES	256

#define COND_ID_MAP_DATA 12
#define MAX_BUCKET_COND_ID_MAP_RECORD   MAX_COND_ID_MAP_LINES+50
char sWatchCondIdMap[COND_ID_MAP_DATA - sizeof(CONDIDMAP)];


#define SHOPLIMIT_DEF_DATA 512
#define MAX_BUCKET_SHOP_LIMIT_RECORD   MAX_SHOP_DEF_LINES+50
char sWatchShopLimit[SHOPLIMIT_DEF_DATA - sizeof(SHOPLIMITDEF)];



#define MAX_RAND_AWARD_DEF 200
#define RAND_AWARD_DATA 512
char sWatchRandAwardData[RAND_AWARD_DATA - sizeof(RNDAWDDEF)];

#define MAX_BCT_DEF 100
#define BCT_DEF_DATA 768
char sWatchBCTDefData[BCT_DEF_DATA - sizeof(BCTDEF)];

#define MAX_SECONDARY_SKILL_DEF 512
#define SECONDARY_SKILL_DEF_DATA 128
char sWatchSecondarySkillDefData[SECONDARY_SKILL_DEF_DATA - sizeof(SECONDARYSKILLDEF)];

#define MAX_RECIPE_DEF 1024
#define RECIPE_DEF_DATA 768
char sWatchRecipeDefData[RECIPE_DEF_DATA - sizeof(RECIPEDEF)];

#define MAX_MON_BOOTY_GROUP_DEF 3072
#define MON_BOOTY_GROUP_DEF_DATA 512
char sWatchMonBootyGroupDefData[MON_BOOTY_GROUP_DEF_DATA - sizeof(MONBOOTYGROUPDEF)];

#define MAX_MON_BOOTY_DEF 5000 
#define MON_BOOTY_DEF_DATA 256
char sWatchMonBootyDefData[MON_BOOTY_DEF_DATA - sizeof(MONBOOTYDEF)];


#define MAX_CANGBAOTU_DEF 50
#define CANGBAOTU_DEF_DATA 128
char sWatchCangBaoTuDefData[CANGBAOTU_DEF_DATA - sizeof(CANGBAOTUDEF)];

#define MAX_MON_MACHINE_DEF 128
#define MON_MACHINE_DEF_DATA 512
char sWatchMonMachineDefData[MON_MACHINE_DEF_DATA - sizeof(MONMACHINEDEF)];

#define MAX_CLAN_TECH_DEF 64
#define CLAN_TECH_DEF_DATA 512
char sWatchClanTechDefData[CLAN_TECH_DEF_DATA - sizeof(CLANTECHDEF)];

#define MAX_CLAN_PRODUCT_DEF 64
#define CLAN_PRODUCT_DEF_DATA 1024 
char sWatchClanProductDefData[CLAN_PRODUCT_DEF_DATA - sizeof(CLANPRODUCTDEF)];

#define MAX_ATTR_CONTRAST_DEF 128
#define ATTR_CONTRAST_DEF_DATA 128
char sWatchAttrContrastDefData[ATTR_CONTRAST_DEF_DATA - sizeof(ATTRCONTRASTDEF)];

#define MAX_LEVEL_ATTR_DEF 120
#define LEVEL_ATTR_DEF_DATA 512
char sWatchLevelAttrDefData[LEVEL_ATTR_DEF_DATA - sizeof(LEVELATTRDEF)];

#define MAX_DIE_PUNISH_DEF 3
#define DIE_PUNISH_DEF_DATA 128
char sWatchDiePunishDefData[DIE_PUNISH_DEF_DATA - sizeof(DIEPUNISHDEF)];


#define MAX_CONT_ONLINE_DEF 5
#define CONT_ONLINE_DEF_DATA 512
char sWatchContOnlineDefData[CONT_ONLINE_DEF_DATA - sizeof(CONTONLINEGIFTDEF)];

#define MAX_BATTLE_DEF 7
#define BATTLE_DEF_DATA 3072
char sWatchBattleDefData[BATTLE_DEF_DATA - sizeof(BATTLEDEF)];

#define MAX_FAIRY_STAR_DEF 256
#define FAIRY_STAR_DATA 64
char sWatchFairyStarDefData[FAIRY_STAR_DATA - sizeof(FAIRYSTARDEF)];

#define MAX_FAIRY_STUFF_DEF 32
#define FAIRY_STUFF_DEF_DATA 64
char sWatchFairyStuffDefData[FAIRY_STUFF_DEF_DATA - sizeof(FAIRYSTARSTUFFDEF)];

#define MAX_PET_RESOLVE_DEF 32
#define PET_RESOLVE_DEF_DATA 64
char sWatchPetResolveDefData[PET_RESOLVE_DEF_DATA - sizeof(PETRESOLVEDEF)];

#define MAX_PET_SKILL_GRID_DEF 5
#define PET_SKILL_GRID_DEF_DATA 64
char sWatchPetSkillGridDefData[PET_SKILL_GRID_DEF_DATA - sizeof(PETSKILLGRIDDEF)];

#define MAX_PET_NATIVE_SKILL_DEF 64
#define PET_NATIVE_SKILL_DEF_DATA 32
char sWatchPetNativeSkillDefData[PET_NATIVE_SKILL_DEF_DATA - sizeof(PETNATIVESKILLDEF)];

#define MAX_PET_FUSE_DEF 16
#define PET_FUSE_DEF_DATA 32
char sWatchPetFuseDefData[PET_FUSE_DEF_DATA - sizeof(PETFUSEDEF)];

#define MAX_OFFLINE_EXP_PARAMETER 4
#define OFFLINE_EXP_PARAMETER_DATA 32
char sWatchOfflineExpParameterData[OFFLINE_EXP_PARAMETER_DATA - sizeof(OFFLINEEXPPARAMETER)];

#define MAX_MAP_TMP_ITEM_DEF 256
#define MAP_TMP_ITEM_DEF_DATA 32
char sWatchMapTmpItemDefData[MAP_TMP_ITEM_DEF_DATA - sizeof(MAPTMPITEMDEF)];

#define MAX_SUBJECT_TYPE_DEF 32
#define SUBJECT_TYPE_DEF_DATA 64
char sWatchSubJectTypeDefData[SUBJECT_TYPE_DEF_DATA - sizeof(SUBJECTTYPEDEF)];

#define MAX_SUBJECT_DEF 2048 
#define SUBJECT_DEF_DATA 1024
char sWatchSubJectDefData[SUBJECT_DEF_DATA - sizeof(SUBJECTDEF)];


//#define MAX_ONLINE_GROUP_DEF 4
#define MAX_ONLINE_GROUP_DATA 1500
char sWatchOnlineGroupData[MAX_ONLINE_GROUP_DATA - sizeof(ONLINEGIFTGROUP)];

#define MAX_WORLD_ITEM_LIMIT_DEF 1024
#define WORLD_ITEM_LIMIT_DEF_DATA 32
char sWatchWorldItemLimitDefData[WORLD_ITEM_LIMIT_DEF_DATA - sizeof(WORLDITEMLIMITDEF)];

#define MAX_WORLD_BOOTY_GROUP_LIMIT_DEF 128
#define WORLD_BOOTY_GROUP_LIMIT_DEF_DATA 32
char sWatchWorldBootyGroupLimitDefData[WORLD_BOOTY_GROUP_LIMIT_DEF_DATA - sizeof(WORLDBOOTYGROUPLIMITDEF)];

#define MAX_DIFF_PWORLD_DEF 32
#define DIFF_PWORLD_DEF_DATA 1664
char sWatchDiffPworldDefData[DIFF_PWORLD_DEF_DATA - sizeof(DIFFPWORLDDEF)];

#define MAX_MON_EVENT_DEF 128
#define MON_EVENT_DEF_DATA 64
char sWatchMonEventDefData[MON_EVENT_DEF_DATA - sizeof(MONEVENTDEF)];

#define MAX_PWORLD_BOOTY_GROUP_DEF 256
#define PWORLD_BOOTY_GROUP_DEF_DATA 512
char sWatchPworldBootyGroupDefData[PWORLD_BOOTY_GROUP_DEF_DATA - sizeof(PWORLDBOOTYGROUPDEF)];

#define MAX_PWORLD_AWARD_MUL_DEF 256 
#define PWORLD_AWARD_MUL_DEF_DATA 128
char sWatchPworldAwardMulDefData[PWORLD_AWARD_MUL_DEF_DATA - sizeof(PWORLDAWARDMULDEF)];

#define MAX_MAP_ORE_POSITION_DEF 1300
#define MAP_ORE_POSITION_DEF_DATA 64
char sWatchMapOrePositionDefData[MAP_ORE_POSITION_DEF_DATA - sizeof(MAPOREPOSITIONDEF)];

#define MAX_MAP_ORE_DEF 32
#define MAP_ORE_DEF_DATA 128
char sWatchMapOreDefData[MAP_ORE_DEF_DATA - sizeof(MAPOREDEF)];

#define MAX_ATKER_HURT_MUL_DEF 32
#define ATKER_HURT_MUL_DEF_DATA 128
char sWatchAtkerHurtMulDefData[ATKER_HURT_MUL_DEF_DATA - sizeof(ATKERHURTMULDEF)];

#define MAX_WORLD_CITY_BUILD_DEF 32
#define WORLD_CITY_BUILD_DEF_DATA 256
char sWatchWorldCityBuildDefData[WORLD_CITY_BUILD_DEF_DATA - sizeof(WORLDCITYBUILDDEF)];

#define MAX_STARSTONE_DEF 31
#define MAX_STARSTONE_DEF_DATA 2048
char sWatchStarStoneDefData[MAX_STARSTONE_DEF_DATA - sizeof(STARSTONEDEF)];

#define MAX_STARFUNC_DEF 256
#define MAX_STARFUNC_DEF_DATA 2048
char sWatchStarFuncDefData[MAX_STARFUNC_DEF_DATA - sizeof(STARFUNCDEF)];

#define MAX_CITY_PRODUCT_DEF 128
#define CITY_PRODUCT_DEF_DATA 3072
char sWatchCityProductDefData[CITY_PRODUCT_DEF_DATA - sizeof(CITYPRODUCTDEF)];

#define MAX_BATTLE_GLORY_DEF 64
#define BATTLE_GLORY_DEF_DATA 256
char sWatchBattleGloryDefData[CITY_PRODUCT_DEF_DATA - sizeof(BATTLEGLORYDEF)];

#define MAX_GEM_NEW_DEF 128
#define GEM_NEW_DEF_DATA 170
char sWatchGemNewDefData[GEM_NEW_DEF_DATA - sizeof(GEMNEWDEF)];

#define MAX_GEM_ATTR_STAR_DEF 5120
#define GEM_ATTR_STAR_DEF_DATA 128
char sWatchGemAttrStarDefData[GEM_ATTR_STAR_DEF_DATA-sizeof(GEMATTRSTARDEF)];

#define MAX_GEM_LEVEL_STAR_DEF 256
#define GEM_LEVEL_STAR_DEF_DATA 32
char sWatchGemLevelStarDefData[GEM_LEVEL_STAR_DEF_DATA-sizeof(GEMLEVELSTARDEF)];

#define MAX_ARM_RECAST_DEF 64
#define ARM_RECAST_DEF_DATA 128
char sWatchArmRecastDefData[ARM_RECAST_DEF_DATA-sizeof(ARMRECASTDEF)];

#define MAX_USE_DEF 50
#define USE_DEF_DATA 32
char sWatchUseDefData[USE_DEF_DATA-sizeof(USEDEF)];

#define MAX_RUNE_ADDATTR_DEF 128
#define RUNE_ADDATTR_DEF_DATA 2048
char sWatchRuneAddAttrDefData[RUNE_ADDATTR_DEF_DATA-sizeof(RUNEADDATTRDEF)];

#define MAX_ARM_STAR_DEF 12
#define ARM_STAR_DEF_DATA 512
char sWatchArmStarDefData[ARM_STAR_DEF_DATA-sizeof(ARMSTARDEF)];

#define MAX_UNREAL_MAP_DEF 10
#define UNREAL_MAP_DEF_SIZE 300
char sWatchUnrealMapDefData[UNREAL_MAP_DEF_SIZE-sizeof(UNREALMAPDEF)];

#define MAX_UNREAL_DEF 110
#define UNREAL_DEF_SIZE 512
char sWatchUnrealDefData[UNREAL_DEF_SIZE-sizeof(UNREALDEF)];

#define MAX_PET_WASH_GENIUS_DEF 100
#define PET_WASH_GENIUS_DEF_DATA 512
char sWatchPetWashGeniusData[PET_WASH_GENIUS_DEF_DATA-sizeof(PETWASHGENIUSDEF)];

#define MAX_NEW_ADD_ATTR_DEF 512
#define NEW_ADD_ATTR_DEF_DATA 2048
char sWatchNewAddAttrDefData[NEW_ADD_ATTR_DEF_DATA-sizeof(NEWADDATTRDEF)];

#define MAX_ADD_ATTR_RAND_DEF 128
#define ADD_ATTR_RAND_DEF_DATA 512
char sWatchAddAttrRandDefData[ADD_ATTR_RAND_DEF_DATA-sizeof(ADDATTRRANDDEF)];


#define MAX_ACHIEVEMENT_CATEGORY_DEF 256
#define ACHIEVEMENT_CATEGORY_DEF_DATA 1024
char sWatchAchievementCategoryDefData[ACHIEVEMENT_CATEGORY_DEF_DATA - sizeof(ACHIEVEMENTCATEGORYDEF)];

#define MAX_SJ_TASK_DEF 16
#define SJ_TASK_DEF_DATA 128
char sWatchSJTaskDefData[SJ_TASK_DEF_DATA-sizeof(SJTASKDEF)];

#define MAX_DART_CAR_DEF 10
#define DART_CAR_DEF_DATA 192
char sWatchDartCarDefData[DART_CAR_DEF_DATA-sizeof(DARTCARDEF)];

#define MAX_GAY_DEF 1
#define GAY_DEF_DATA 32
char sWatchGayDefData[GAY_DEF_DATA-sizeof(GAYDEF)];

#define MAX_RANGDSN_DEF 128
#define RANGDSN_DEF_DATA 32
char sWatchRangDsnDefData[MAX_RANGDSN_DEF-sizeof(RANGDSNDEF)];

#define MAX_REVIVE_DEF 120
#define REVIVE_DEF_DATA 128
char sWatchReviveDefData[REVIVE_DEF_DATA-sizeof(REVIVEDEF)];

#define MAX_ACHIEVEMENT_TAIL_ID 8

typedef struct 
{
	int iLevel;//属性起效的等级
	int iNum;//id数量
	int aiID[MAX_ACHIEVEMENT_TAIL_ID];//相同cond的AchievementID
	int iValid; //是否在大类中出现
}AchievementDefTail;


#define MAX_ACHIEVEMENT_DEF 64 
#define ACHIEVEMENT_DEF_DATA 2048 
char sWatchAchievementDefData[ACHIEVEMENT_DEF_DATA - sizeof(ACHIEVEMENTDEF) - sizeof(AchievementDefTail) - 16];



#define MAX_ADD_ATTR_WASH_DEF 8
#define ADD_ATTR_WASH_DEF_DATA 128
char sWatchAddAttrWashDefData[ADD_ATTR_WASH_DEF_DATA-sizeof(ADDATTRWASHDEF)];


#define MAX_SPAR_DEF 60
#define SPAR_DEF_SIZE 64
char sWatchSparDefData[SPAR_DEF_SIZE-sizeof(SPARDEF)];

#define MAX_ITEM_STAR_RAND_DEF 512
#define ITEM_STAR_RAND_DEF_DATA 64
char sWatchItemStarRandDefData[ITEM_STAR_RAND_DEF_DATA-sizeof(ITEMSTARRANDDEF)];

#define MAX_CLAN_SPY_AWARD_DROP_DEF 16
#define CLAN_SPY_AWARD_DROP_DEF_DATA 64
char sWatchClanSpyAwardDropDefData[CLAN_SPY_AWARD_DROP_DEF_DATA - sizeof(CLANSPYAWARDDROPDEF) ];

#define MAX_CLAN_SPY_TRANS_DEF 16
#define CLAN_SPY_TRANS_DEF_DATA 64
char sWatchClanSpyTransDefData[CLAN_SPY_TRANS_DEF_DATA - sizeof(CLANSPYTRANSDEF) ];

typedef struct 
{
	int iTotalMonNum;		// 需刷怪总数
	int iMonPosDefStart;//对应怪物位置表的条目开始位置
	int iMonPosDefCount;//对应怪物位置表的条目总数
	int iClanAwardDefStart;	//对应公会奖励表的条目开始位置
	int iClanAwardDefCount;	//对应公会奖励表的条目总数
}PworldWaveMonDefTail;

#define MAX_PWORLD_WAVE_MON_DEF 512
#define PWORLD_WAVE_MON_DEF_DATA 4096
char sWatchPworldWaveMonDefData[PWORLD_WAVE_MON_DEF_DATA - sizeof(PWORLDWAVEMONDEF) - sizeof(PworldWaveMonDefTail) - 16];

#define MAX_PWORLD_WAVE_MON_POS_DEF 40960
#define PWORLD_WAVE_MON_POS_DEF_DATA 256
char sWatchPworldWaveMonPosDefData[PWORLD_WAVE_MON_POS_DEF_DATA - sizeof(PWORLDWAVEMONPOSDEF) ];

#define MAX_PWORLD_WAVE_MON_CLAN_AWARD_DEF 128
#define PWORLD_WAVE_MON_CLAN_AWARD_DEF_DATA 64
char sWatchPworldWaveMonClanAwardDefData[PWORLD_WAVE_MON_CLAN_AWARD_DEF_DATA - sizeof(PWORLDWAVEMONCLANAWARDDEF) ];

#define MAX_PWORLD_MACHINE_BUFF_SHOP_DEF 128
#define PWORLD_MACHINE_BUFF_SHOP_DEF_DATA 128
char sWatchPworldMachineBuffShopDefData[PWORLD_MACHINE_BUFF_SHOP_DEF_DATA - sizeof(PWORLDMACHINEBUFFSHOPDEF) ];

typedef struct 
{
	int iPosStart;	//相同DefID开始位置
	int iPosCount;	//对应DefID数量
}PlayerAwardDefTail;

#define MAX_PLAYER_AWARD_DEF 1024
#define PLAYER_AWARD_DEF_DATA 128
char sWatchPlayerAwardDefData[PLAYER_AWARD_DEF_DATA - sizeof(PLAYERAWARDDEF) - sizeof(PlayerAwardDefTail) - 16 ];

#define MAX_COW_FINAL_AWARD_DEF 32
#define COW_FINAL_AWARD_DEF_DATA 512
char sWatchCowFinalAwardDefData[COW_FINAL_AWARD_DEF_DATA - sizeof(COWFINALAWARDDEF) ];

#define MAX_GM_TRADE 64
#define GM_TRADE_DATA 64
char sWatchGmTradeData[GM_TRADE_DATA - sizeof(GMTRADE) ];


#define MAX_STRONG_POINT_DEF 9
#define STRONG_POINT_DEF_DATA 1024
char sWatchStrongPointData[STRONG_POINT_DEF_DATA - sizeof(STRONGPOINTDEF) ];

#define MAX_CLAN_SKILL_DEF 1024
#define CLAN_SKILL_DEF_DATA 768
char sWatchClanSkillDefData[CLAN_SKILL_DEF_DATA - sizeof(CLANSKILLDEF) ];

#define MAX_BANNER_DEF 128
#define BANNER_DEF_DATA 128
char sWatchBannerDefData[BANNER_DEF_DATA - sizeof(BANNERDEF) ];

#define MAX_CLAN_PAY_DEF 256
#define CLAN_PAY_DEF_DATA 128
char sWatchClanPayDefData[CLAN_PAY_DEF_DATA - sizeof(CLANPAYDEF) ];

#define MAX_LEVEL_EXP_DEF 256
#define LEVEL_EXP_DEF_DATA 512
char sWatchLevelExpDefData[LEVEL_EXP_DEF_DATA - sizeof(LEVELEXPDEF) ];

#define MAX_LEVEL_BUFF_EXP_DEF 256
#define LEVEL_BUFF_EXP_DEF_DATA 128
char sWatchLevelBuffExpDefData[LEVEL_BUFF_EXP_DEF_DATA - sizeof(LEVELBUFFEXPDEF)];

#define MAX_RIDE_PINZHI_DEF 8
#define RIDE_PINZHI_DEF_DATA 128
char sWatchRidePinZhiDefData[RIDE_PINZHI_DEF_DATA - sizeof(RIDEPINZHIDEF)];

#define MAX_PET_LOVE_LEVEL_DEF 16
#define PET_LOVE_LEVEL_DEF_DATA 512
char sWatchPetLoveLevelDefData[PET_LOVE_LEVEL_DEF_DATA - sizeof(PETLOVELEVELDEF) ];

#define MAX_MAP_AWARD_ORE_DEF 16
#define MAP_AWARD_ORE_DEF_DATA 128
char sWatchMapAwardOreDefData[MAP_AWARD_ORE_DEF_DATA - sizeof(MAPAWARDOREDEF)];

#define MAX_RONGLU_DEF 16
#define RONGLU_DEF_DATA 1024
char sWatchRongluDefData[RONGLU_DEF_DATA - sizeof(RONGLUDEF)];


#define MAX_GENIUS_CAREER_DEF 8
#define GENIUS_CAREER_DEF_DATA 512
char sWatchGeniusCareerDefData[GENIUS_CAREER_DEF_DATA - sizeof(GENIUSCAREERDEF)];

#define MAX_GENIUS_ATTR_DEF 20352
#define GENIUS_ATTR_DEF_DATA 256
char sWatchGeniusAttrDefData[GENIUS_ATTR_DEF_DATA - sizeof(GENIUSATTRDEF)];

#define MAX_BAOXIANG_DEF 128
#define BAOXIANG_DEF_DATA 1024
char sWatchBaoXiangDefData[BAOXIANG_DEF_DATA - sizeof(BAOXIANGDEF)];

#define MAX_WORLD_CITY_FIGHT_AWARD_DEF 8
#define WORLD_CITY_FIGHT_AWARD_DEF_DATA 128
char sWatchWorldCityFightAwardDefData[WORLD_CITY_FIGHT_AWARD_DEF_DATA - sizeof(WORLDCITYFIGHTAWARDDEF)];

#define MAX_WORLD_BOSS_DEF MAX_GLOBAL_BOSS
#define WORLD_BOSS_DEF_DATA 1024 
char sWatchWorldBossDefData[WORLD_BOSS_DEF_DATA - sizeof(WORLDBOSSDEF)];

#define MAX_ARM_LEVELUP_DEF 512
#define ARM_LEVELUP_DEF_DATA 512
char sWatchArmLevelUPDefData[ARM_LEVELUP_DEF_DATA - sizeof(ARMLEVELUPDEF)];

#define MAX_LUCKY_MAP_DEF 128
#define LUCKY_MAP_DEF_DATA 128
char sWatchLuckyMapDefData[LUCKY_MAP_DEF_DATA - sizeof(LUCKYMAPDEF) ];


#define MAX_BUSY_DEF 256 
#define BUSY_DEF_DATA 256
char sWatchBusyDefData[BUSY_DEF_DATA - sizeof(BUSYDEF) ];

#define MAX_BUSY_USE_DEF 256
#define BUSY_USE_DEF_DATA 32
char sWatchBusyUseDefData[BUSY_USE_DEF_DATA - sizeof(BUSYUSEDEF) ];

#define MAX_ITEM_ID_DEF 32
#define ITEM_ID_DEF_DATA 8
char sWatchItemIDDefData[ITEM_ID_DEF_DATA - sizeof(ITEMIDDEF) ];

#define MAX_CAREER_HURT_DEF 16
#define CAREER_HURT_DEF_DATA 128
char sWatchCareerHurtDefData[CAREER_HURT_DEF_DATA - sizeof(CAREERHURTDEF) ];

#define MAX_PET_GENIUS_DEF 16
#define PET_GENIUS_DEF_DATA 32
char szWatchPetGeniusDefData[PET_GENIUS_DEF_DATA-sizeof(PETGENIUSDEF)];

#define MAX_WE_DEF 128
#define WE_DEF_DATA 32
char szWatchWeDefData[WE_DEF_DATA-sizeof(WEDEF)];

#define MAX_EXCHG_ITEM_DEF 10
#define EXCHG_ITEM_DEF_DATA 64
char szWatchExchgItemDefData[EXCHG_ITEM_DEF_DATA-sizeof(EXCHGITEMDEF)];

#define MAX_SPARTA_AWARD_DEF 24
#define SPARTA_AWARD_DEF_DATA 256
char szWatchSpartaAwardDefData[SPARTA_AWARD_DEF_DATA-sizeof(SPARTAAWARDDEF)];

#define MAX_SPARTA_LEVEL_DEF 32
#define SPARTA_LEVEL_DEF_DATA 64
char szWatchSpartaLevelDefData[SPARTA_LEVEL_DEF_DATA-sizeof(SPARTALEVELDEF)];

#define MAX_MAP_RAND_TRANS_DEF 64
#define MAP_RAND_TRANS_DATA 512
char szWatchMapRandTransmitDefData[MAP_RAND_TRANS_DATA-sizeof(MAPRANDTRANSMIT)];

#define MAX_SCHOOL_LEVEL_DEF 120
#define SCHOOL_LEVEL_DEF_DATA 512
char szWatchSchoolLevelDefData[SCHOOL_LEVEL_DEF_DATA-sizeof(SCHOOLLEVELDEF)];

#define MAX_SCHOOL_PWORLD_DEF 32
#define SCHOOL_PWORLD_DEF_DATA 64
char szWatchSchoolPworldDefData[SCHOOL_PWORLD_DEF_DATA-sizeof(SCHOOLPWORLDDEF)];

#define MAX_SCHOOL_XINDE_DEF 120
#define SCHOOL_XINDE_DEF_DATA 64
char szWatchSchoolXinDeDefData[SCHOOL_XINDE_DEF_DATA-sizeof(SCHOOLXINDEDEF)];

#define MAX_BACK_LEVEL_DEF 256
#define BACK_LEVEL_DEF_DATA 64
char szWatchBackLevelDefData[BACK_LEVEL_DEF_DATA-sizeof(BACKLEVELDEF)];

#define MAX_XUKONG_POS_DEF 64
#define XUKONG_POS_DEF_DATA 32
char szWatchXuKongPosDefData[XUKONG_POS_DEF_DATA -sizeof(XUKONGPOSDEF)];

#define MAX_CARD_BOX_DEF 64
#define CARD_BOX_DEF_DATA 512
char szWatchCardBoxDefData[CARD_BOX_DEF_DATA -sizeof(CARDBOXDEF)];

#define MAX_CARD_BOX_GROUP_DEF 64
#define CARD_BOX_GROUP_DEF_DATA 3072
char szWatchCardBoxGroupDefData[CARD_BOX_GROUP_DEF_DATA -sizeof(CARDBOXGROUPDEF)];

#define MAX_PWORLD_BOX_DEF 64
#define PWORLD_BOX_DEF_DATA 256
char szWatchPworldBoxDefData[PWORLD_BOX_DEF_DATA -sizeof(PWORLDBOXDEF)];

#define MAX_YANGHAIBANG_DEF 64
#define YANGHAIBANG_DEF_DATA 256
char szWatchYangHaiBangDefData[YANGHAIBANG_DEF_DATA -sizeof(YANGHAIBANGDEF)];

#define MAX_HANG_HAI_AWARD_DEF 32
#define HANG_HAI_AWARD_DEF_DATA 512
char sWatchHangHaiAwardDefData[HANG_HAI_AWARD_DEF_DATA - sizeof(HANGHAIAWARDDEF)];


typedef struct 
{
	int iPosStart;	//相同DefID开始位置
	int iPosCount;	//对应DefID数量
}ClanAwardDefTail;

#define MAX_CLAN_AWARD_DEF 128
#define CLAN_AWARD_DEF_DATA 128
char sWatchClanAwardDefData[CLAN_AWARD_DEF_DATA - sizeof(CLANAWARDDEF) - sizeof(ClanAwardDefTail)];

#define MAX_CLAN_WAR_AWARD_DEF 128
#define CLAN_WAR_AWARD_DEF_DATA 1024
char sWatchClanWarAwardDefData[CLAN_WAR_AWARD_DEF_DATA - sizeof(CLANWARAWARDDEF) ]; 

#define MAX_GENIUS_MDF_DEF 8
#define GENIUS_MDF_DEF_DATA 128
char sWatchGeniusMdfDefData[GENIUS_MDF_DEF_DATA - sizeof(GENIUSMDFDEF) ]; 

#define MAX_RONGYANJUSHOU_DEF 32
#define RONGYANJUSHOU_DEF_DATA 256
char szWatchRongYanJuShouDefData[RONGYANJUSHOU_DEF_DATA -sizeof(RONGYANJUSHOUDEF)];

#define MAX_RONGYANMONDYNATTR_DEF 128
#define RONGYANMONDYNATTR_DEF_DATA 256
char sWatchRongYanMonDynAttrDefData[RONGYANMONDYNATTR_DEF_DATA - sizeof(RONGYANMONDYNATTRDEF)];

#define MAX_ARM_SKILL_ATTR_DEF 256
#define ARM_SKILL_ATTR_DEF_DATA 384
char sWatchArmSkillAttrDefData[ARM_SKILL_ATTR_DEF_DATA - sizeof(ARMSKILLATTRDEF)];

#define MAX_ARM_SKILL_NUM_DEF 512
#define ARM_SKILL_NUM_DEF_DATA 64
char sWatchArmSkillNumDefData[ARM_SKILL_NUM_DEF_DATA - sizeof(ARMSKILLNUMDEF)];

#define MAX_PAOSHANG_DEF 32
#define PAOSHANG_DEF_DATA 64
char sWatchPaoShangDefData[PAOSHANG_DEF_DATA - sizeof(PAOSHANGDEF)];

#define MAX_PAOSHANG_ITEM_DEF 32
#define PAOSHANG_ITEM_DEF_DATA 32
char sWatchPaoShangItemDefData[PAOSHANG_ITEM_DEF_DATA - sizeof(PAOSHANGITEMDEF)];

#define MAX_PET_MONEY_DEF 13
#define PET_MONEY_DEF_DATA 64
char sWatchPetMoneyDefData[PET_MONEY_DEF_DATA - sizeof(PETMONEYDEF)];

#define MAX_PET_IMP_UP_ATTR_DEF 48
#define PET_IMP_UP_ATTR_DEF_DATA 128
char sWatchPetImpUpAttrDefData[PET_IMP_UP_ATTR_DEF_DATA - sizeof(PETIMPUPATTRDEF)];

#define MAX_PET_IMP_UP_DEF 13
#define PET_IMP_UP_DEF_DATA 64
char sWatchPetImpUpDefData[PET_IMP_UP_DEF_DATA - sizeof(PETIMPUPDEF)];

#define MAX_PET_OP_AWARD_DEF 32
#define PET_OP_AWARD_DEF_DATA 32
char sWatchPetOpAwardDefData[PET_OP_AWARD_DEF_DATA - sizeof(PETOPAWARDDEF)];

#define MAX_GEM_PAG_DEF 16 
#define GEM_PAG_DEF_DATA 1024
char szWatchGemPagDefData[GEM_PAG_DEF_DATA-sizeof(GEMPAGDEF)];

#define MAX_ARM_TESHU_DEF 64 
#define ARM_TESHU_DEF_DATA 256
char szWatchArmTeshuDefData[ARM_TESHU_DEF_DATA-sizeof(ARMTESHUDEF)];

#define MAX_ZJ_RONGYANJUSHOU_DEF 16 
#define ZJ_RONGYANJUSHOU_DEF_DATA 256
char szWatchZJRongYanJuShouDefData[ZJ_RONGYANJUSHOU_DEF_DATA -sizeof(ZJRONGYANJUSHOUDEF)];

#define MAX_SPAN_STRONG_DEF 13 
#define SPAN_STRONG_DEF_SIZE 512
char szWatchSpanStrongDefData[SPAN_STRONG_DEF_SIZE -sizeof(SPANSTRONGDEF)];

#define MAX_WORD_PWD_DEF 1012 
#define WORD_PWD_DEF_DATA 292
char szWatchWordPwdDefData[WORD_PWD_DEF_DATA -sizeof(WORDPWDDEF)];


/* 商品数量可以很多,商品索引不能存储在角色身上,暂时用hash优化下算法 */
typedef struct tagShopBucket
{
	unsigned int uiKey;
	char szData[SHOP_DEF_DATA];
}ShopBucket;
typedef struct tagShopMemory
{
	char sMem[SHT_SIZE(MAX_BUCKET_SHOP_RECORD, MAX_SHOP_DEF_LINES, sizeof(ShopBucket))];

}ShopMemory;

typedef struct tagShopLimitBucket
{
	unsigned int uiKey;
	char szData[SHOPLIMIT_DEF_DATA];
}ShopLimitBucket;
typedef struct tagShopLimitMemory
{
	char sMem[SHT_SIZE(MAX_BUCKET_SHOP_LIMIT_RECORD, MAX_SHOP_DEF_LINES, sizeof(ShopLimitBucket))];
}ShopLimitMemory;

typedef struct tagVipBarBucket
{
	unsigned int uiKey;
	char szData[VIP_BAR_DEF_DATA];
}VipBarBucket;

typedef struct tagVipBarMemory
{
	char sMem[SHT_SIZE(MAX_BUCKET_VIP_BAR_RECORD, MAX_VIP_BAR_DEF_LINES, sizeof(VipBarBucket))];
}VipBarMemory;

typedef struct tagVipBarLevelBucket
{
	unsigned int uiKey;
	char szData[VIP_BAR_LEVEL_DEF_DATA];
}VipBarLevelBucket;

typedef struct tagVipBarLevelMemory
{
	char sMem[SHT_SIZE(MAX_BUCKET_VIP_BAR_LEVEL_RECORD, MAX_VIP_BAR_LEVEL_DEF_LINES, sizeof(VipBarLevelBucket))];
}VipBarLevelMemory;

typedef struct tagVipBarNumBucket
{
	unsigned int uiKey;
	char szData[VIP_BAR_NUM_DEF_DATA];
}VipBarNumBucket;

typedef struct tagVipBarNumMemory
{
	char sMem[SHT_SIZE(MAX_BUCKET_VIP_BAR_NUM_RECORD, MAX_VIP_BAR_NUM_DEF_LINES, sizeof(VipBarNumBucket))];
}VipBarNumMemory;

typedef struct tagVipBarOnlineBucket
{
	unsigned int uiKey;
	char szData[VIP_BAR_ONLINE_DATA];
}VipBarOnlineBucket;

typedef struct tagVipBarOnlineMemory
{
	char sMem[SHT_SIZE(MAX_BUCKET_VIP_BAR_ONLINE_RECORD, MAX_VIP_BAR_ONLINE_LINES, sizeof(VipBarOnlineBucket))];
}VipBarOnlineMemory;

typedef struct tagDynActiveStatusBucket
{
	unsigned int uiKey;
	char szData[DYN_ACTIVE_STATUS_DATA];
}DynActiveStatusBucket;

typedef struct tagDynActiveStatusMemory
{
	char sMem[SHT_SIZE(MAX_BUCKET_DYN_ACTIVE_STATUS_RECORD, MAX_DYN_ACTIVE_STATUS_LINES, sizeof(DynActiveStatusBucket))];
}DynActiveStatusMemory;

typedef struct tagCondIdMapBucket
{
	unsigned int uiKey;
	char szData[COND_ID_MAP_DATA];
}CondIdMapBucket;

typedef struct tagCondIdMapMemory
{
	char sMem[SHT_SIZE(MAX_BUCKET_COND_ID_MAP_RECORD, MAX_COND_ID_MAP_LINES, sizeof(CondIdMapBucket))];
}CondIdMapMemory;



#define CLAN_OBJ_DATA 128000000
#define ZONE_CLAN_DATA 115000

enum tagCancheFlag
{
	CACHE_FLAG_CLEAN = 0,   // 数据是干净的。
	CACHE_FLAG_DIRTY = 1,   // 脏数据需要写回。
	CACHE_FLAG_IMM = 2,     // 脏数据，立即写回。
	CACHE_FLAG_INEXIST = 3, // 数据不存在了，不是需要写回，而是删除。
};

// 公会机械最大种类数
#define MAX_CLAN_MACHINE 18

enum CLAN_MACHINE_PROPERY
{
	CLAN_MACHINE_MAX_HP, // 生命上限
	CLAN_MACHINE_DEF,    // 防御
	CLAN_MACHINE_ATK,    // 攻击
	CLAN_MACHINE_CAP,    // 容量
	CLAN_MACHINE_COLLECTING_SPEED, // 采集速度
	CLAN_MACHINE_MOVING_SPEED,     // 移动速度
	MAX_CLAN_MACHINE_PROPERTY,
};

typedef struct tagClanMachinePropertyEntry
{
	int iID; // 公会机械ID
	int aiAdded[MAX_CLAN_MACHINE_PROPERTY]; // 属性列表
}ClanMachinePropertyEntry;

#define INIT_CLAN_MACHINE_PROPERTY_ENTRY() \
	{ .iID = 0, .aiAdded = { 0, 0, 0, 0, 0, 0, }, }


// 公会机械因公会科技或其他原因增加的属性值的缓存
typedef struct tagClanMachineProperyCache
{
	int iNum;
	ClanMachinePropertyEntry astProperties[MAX_CLAN_MACHINE];
}ClanMachineProperyCache;


typedef struct tagZoneClanLive
{
	CLANID stOtherSideClan; // 对方公会ID。
	int tActionTime;        // 发生时间。
}ZoneClanLive;

#define MAX_ROLL_CLAN_BOOTY  16
#define MAX_CLAN_ROLL_MEMBER	200

typedef struct {
	
	int iID;								//1战利品ID
	int iRoll;								//1是否ROLL过了
}ClanBootyWaiterInfo;

typedef struct {

	int iMaxRoll;									//1当前最大Roll点数									
	tdr_ulonglong ullWinRoleID; 					//1当前最大roll点的人的RoleID
	char szWinRoleName[ROLE_NAME_LEN];			//1角色名，提示需要用，角色有可能下线了
	
	int iWaitNum;
	ClanBootyWaiterInfo astClanBootyWaiterInfo[MAX_CLAN_ROLL_MEMBER];		//1ROLL点参与者信息
}ClanBootyRollInfo;

typedef struct {
	
	int iBootyID;								//1战利品ID
	time_t tEndTime;							//1Roll点倒计时，时间到了给stBootyRollInfo 所有权(所有人都没选为0，则视为人人可拾取)	
	ClanBootyRollInfo stBootyRollInfo;
}ClanBootyInfo;

enum CLAN_BATTLE_STATUS
{
	CLAN_BATTLE_STATUS_CHECK, // 检查阶段
	CLAN_BATTLE_STATUS_APPLY,    //报名中
	CLAN_BATTLE_STATUS_CAN_MATCH,    //可匹配	
	CLAN_BATTLE_STATUS_MATCH,    // 匹配中
	CLAN_BATTLE_STATUS_READY,    // 可进战场
};

typedef struct tagClanBattleApplyData
{
	int iBattleID; //报名的战场id
	int iApplyStaus; //报名的状态| CLAN_BATTLE_STATUS
	int iApplyTime;//报名的时间
	int iApplyMemID;//报名的玩家memid
}ClanBattleApplyData;

typedef struct tagZoneClanInfo
{
	int iListVersion; //列表信息版本
	int iDirty;
	int iTimerCount;
	
	// 以下数据用于必须在线处理的动作
	ZoneClanLive stInviteLive;
	ZoneClanLive stExitEnemyLive;

	int tLastSetBulliten; // 最后一次修改公告的时间]

	// 公会城市地图/副本相关
	tdr_ulonglong ullCityPworldWID;
	int iCityPworldInstID;

	//每日活动副本
	tdr_ulonglong ullEveryDayPworldWID;
	int iEveryDayPworldInstID;
	int iEveryDayPworldVal;
	int iMaxEveryDayPworldVal;
	int iBossLife;

	//守护枢纽副本
	tdr_ulonglong ullMachinePworldWID;
	int iMachinePworldInstID;
	int iMachinePworldVal;

	// 消耗公会物资的时间点
	int tWorkingCostTime;

	// 是否是延迟摧毁公会(主动解散是非延迟摧毁,其他情况都是延迟摧毁)
	int fWillDelayDestroy;

	// 在线人数计数器
	int iOnlineCount;

	// 公会机械映射
	int iMachineIDMap[MAX_CLAN_MACHINE_GRID_NUM];

	// 公会机械因公会科技或其他原因增加的属性值的缓存
	ClanMachineProperyCache stMacProCache;

	// 公会视图(公会查询面板显示的数据)变化, 需要发送通知
	int fWillViewNotify;

	CLANMAPPOS stClanMapPosInfo;

	// 发布战争通知的时间
	int iWarNotify;

	int iClanTouchStatus;
	ClanBattleApplyData stBattleApplyData; //公会跨服战申请信息
	int iClanBootyNum;
	ClanBootyInfo astClanBooty[MAX_ROLL_CLAN_BOOTY];
}ZoneClanInfo;

typedef struct tagZoneClan
{
	CLAN stClan;
	ZoneClanInfo stZoneClanInfo;
	char sRes[ZONE_CLAN_DATA - sizeof(CLAN) - sizeof(ZoneClanInfo)];
}ZoneClan;


typedef struct tagZoneStoreInfo
{
	int iDirty;
	unsigned int uiTimeCount;
}ZoneStoreInfo;


#define SURVIVE_SUPERGOD_NUM 20 //生存大考验超神杀人数
#define SURVIVE_GOD_NUM 15 //生存大考验如神杀人数
#define SURVIVE_MON_NUM 10 //生存大考验如妖杀人数
#define SURVIVE_MAN_NUM 5 //生存大考验无人能挡杀人数

#define GLOBAL_INIT 0
#define GLOBAL_REQ 1
#define GLOBAL_FINI 2

#define CLAN_STAT_INIT 0
#define CLAN_STAT_COUNT_REQ 1
#define CLAN_STAT_REQ 2
#define CLAN_STAT_FINI 3
#define CLAN_STAT_FAIL 4

#define CLAN_BUCKET CLAN_MAX_NUM_HARD*3
#define STORE_BUCKET MAX_STORE_NUM*3 

#define STORE_STAT_INIT 0
#define STORE_STAT_FINI 1
#define STORE_STAT_FAIL 2
#define STORE_STAT_REQ 3

#define HOME_STAT_INIT 0
#define HOME_STAT_FINI 1
#define HOME_STAT_FAIL 2
#define HOME_STAT_REQ 3

typedef struct tagClanObjInfo
{
	int iClanStat;
	time_t tReq;
	int iListVersion; //列表信息版本，数量发生变化时修改
	int iClanDBCount;
}ClanObjInfo;

typedef struct tagClanObj
{
	int iClanNum;
	ZoneClan astZoneClan[CLAN_MAX_NUM_HARD];
	ClanObjInfo stClanObjInfo;
}ClanObj;


#define STORE_OBJ_DATA 5200000
#define ZONE_STORE_DATA 10240

typedef struct tagZoneStore
{
	STORE stStore;
	ZoneStoreInfo stZoneStoreInfo;
	char sRes[ZONE_STORE_DATA - sizeof(STORE) - sizeof(ZoneStoreInfo)];
}ZoneStore;

typedef struct tagDBInsertBack
{
	int iID;
	tdr_ulonglong RoleID;
	char RoleName[NAME_LEN];
	char StoreName[NAME_LEN];
}DBInsertBack;

typedef struct tagDBHomeInsertBack
{
	int iID;
	tdr_ulonglong RoleID;
	char RoleName[NAME_LEN];
}DBHomeInsertBack;


typedef struct tagObjStoreInfo
{
		int iStat;
	int iDBCount;
	int iQsortTime;
//	int iInsertLock;// 插入锁
//	int iInsertTime;
	
}ObjStoreInfo;

typedef struct tagStoreObj
{
	int iStoreNum;
	ZoneStore astZoneStore[MAX_STORE_NUM];
	ObjStoreInfo stObjStoreInfo;
	
}StoreObj;

char sWatchStoreObj[STORE_OBJ_DATA - sizeof(StoreObj)];

char sWatchClanObj[CLAN_OBJ_DATA - sizeof(ClanObj)];


// 家园
#define HOME_OBJ_DATA 5300000
#define ZONE_HOME_DATA 5000
#define SVR_MAX_HOME 1000

typedef struct tagZoneHomeInfo
{
	int iDirty;
	unsigned int uiTimeCount;
}ZoneHomeInfo;

typedef struct tagZoneHome
{
	HOME stHome;
	ZoneHomeInfo stZoneHomeInfo;
	char sRes[ZONE_HOME_DATA - sizeof(HOME) - sizeof(ZoneHomeInfo)];
}ZoneHome;


typedef struct tagObjHomeInfo
{
	int iStat;
	int iDBCount;	
}ObjHomeInfo;

typedef struct tagHomeObj
{
	int iHomeNum;
	ZoneHome astZoneHome[SVR_MAX_HOME];
	ObjHomeInfo stObjHomeInfo;
	
}HomeObj;

char sWatchHomeObj[HOME_OBJ_DATA - sizeof(HomeObj)];
//家园end

typedef struct tagClanHash
{
	unsigned int uiClanId;
	tdr_ulonglong ullClanGID;
	int iIdx;
}ClanHash;

typedef struct tagStoreHash
{
	unsigned int uiID;
	tdr_ulonglong RoleID;
	int iIdx;
}StoreHash;


#define MAX_TRADESELL_BUCKET   MAX_TRADE_RECORD_NUM+100
typedef struct tagTradeSellMemory
{
	char sMem[SHT_SIZE(MAX_TRADESELL_BUCKET, MAX_TRADE_RECORD_NUM+100, sizeof(TRADESELL))];
}TradeSellMemory;
#define MAX_TRADEBUY_BUCKET   MAX_TRADE_RECORD_NUM+100
typedef struct tagTradeBuyMemory
{
	char sMem[SHT_SIZE(MAX_TRADEBUY_BUCKET, MAX_TRADE_RECORD_NUM+100, sizeof(TRADEBUY))];
}TradeBuyMemory;


#define TRUST_BUCKET MAX_PLAYER*3
#define MAX_TRUST_TASK 5

#define EVENT_BUCKET MAX_GAME_EVENT*3

#define WATCH_BUCKET MAX_WATCH_ROLE*3

//#define GLOBAL_DATA 51200
#define GLOBAL_DATA 800000


#define GLOBAL_STAT_INIT 0
#define GLOBAL_STAT_REQ 1
#define GLOBAL_STAT_FINI 2
#define GLOBAL_STAT_FAIL 3

#define GLOBAL_WORLDCITY_INIT 0
#define GLOBAL_WORLDCITY_FINI 1



typedef struct tagGlobalObjInfo
{
	int iGlobalStat;
	time_t tReq;
	int iDirty;  // 1 定时更新， 2  立即更新
	int iConnLimitsEnabled;
	int iWorldCityStat;
	int iStrongStat;
	int iBossStat;   
}GlobalObjInfo;



typedef struct tagGlobalObj
{
	GLOBAL stGlobal;
	GlobalObjInfo stGlobalObjInfo;
}GlobalObj;

char sWatchGlobalObj[GLOBAL_DATA - sizeof(GlobalObj)];


#define REGION_GLOBAL_STAT_INIT 0
#define REGION_GLOBAL_STAT_REQ 1
#define REGION_GLOBAL_STAT_FINI 2
#define REGION_GLOBAL_STAT_FAIL 3

struct tagRegionGlobalStat
{
	int iStat;
	int iDirty;
};
typedef struct tagRegionGlobalStat RegionGlobalStat;

struct tagRegionGlobalInfo
{
	REGIONGLOBAL stGlobal;
	RegionGlobalStat stStat;
};
typedef struct tagRegionGlobalInfo RegionGlobalInfo;

#define REGION_GLOBAL_DATA 40960
char sWatchRegionGlobalObj[REGION_GLOBAL_DATA - sizeof(RegionGlobalInfo)];

// 邮件系统 begin
#define ZONE_MAIL_DATA 2248
#define MAIL_MAX_TOTAL_NUM ((MAX_PLAYER*65/100)* (MAIL_MAX_NORMAL_NUM + MAIL_MAX_SYSTEM_NUM))

#define POSTOFFICE_OBJ_DATA 340000

#define MAIL_MAX_MAILBOX (MAX_PLAYER*125/100)
#define MAILBOX_BUCKET (MAIL_MAX_MAILBOX * 2)

// 邮箱状态定义
#define MAILBOX_STATE_CONNECTED 1 // 邮箱的拥有者在线
#define MAILBOX_STATE_NOTIFIED  2 // 邮箱已经通知拥有者有新邮件


/*
typedef struct tagListEntry
{
	int iIdx; // 在mempool中的索引
	int iNext; // 下一节点/头节点
	int iPrev; // 上一节点/尾节点
}ListEntry;
*/

typedef struct tagZoneMailInfo
{
	int iDirty; // 缓存标志
	int iWriteBackTm; // 数据库回写请求开始的时间
	int iVersion; // 版本号
}ZoneMailInfo;


typedef struct tagZoneMail
{
	ListEntry stEntry;
	ZoneMailInfo stInfo;
	MAIL stMail;
	char sRes[ZONE_MAIL_DATA - sizeof(ListEntry) - sizeof(ZoneMailInfo) - sizeof(MAIL)];
}ZoneMail;

typedef struct tagMailbox
{
	char szOwner[ROLE_NAME_LEN]; // 邮箱拥有者角色名
	size_t nNormalNum; // 一般邮件数量
	size_t nSystemNum; // 系统邮件数量
	int iMailList; // 邮件链表
	int iActive; // 活跃度
	int iState; // 邮箱状态
}Mailbox;

typedef struct tagSubJectPlayer
{
	unsigned long long ullRoleID;
	int iMemID;
	int iInteg;	// 积分
}SUBJECTPLAYER;

typedef struct tagSubJect
{
	char cStat;			// 活动状态
	char cSubJectNum;	// 已经出题数量，最多30题
	int iSubJectID;		// 当前题ID
	int iSuccIdx;			// 正确答案索引，避免频繁查表
	time_t tLockEnd;		// 本轮阅题结束时间
	time_t tResEnd;		// 本论答题结束时间
	time_t tReqEnd;		// 本次活动邀请结束时间
	time_t tStart;			// 本次活动开始时间

	int iPlayer;			// 玩家信息
	SUBJECTPLAYER astSJPlayer[MAX_PLAYER];
	int iAlready[SUBJECT_MAX_NUM];		// 已经出题的所有ID
}SUBJECT;

typedef struct tagWorldBossOne
{
	int iID;					// monid
	int iFreshTime;			// 本次刷新时间
	int iNextTime;			// 下次刷新时间
	int iDayNum;				// 今天刷了几次
	int iStat;					// 1-活着0-死亡
	int iTip;
	
}WORLDBOSSONE;

typedef struct tagWorldBoss
{
	int iBoss;
	WORLDBOSSONE stOne[MAX_WORLD_BOSS_DEF];
	
}WORLDBOSS;

typedef struct tagPostofficeInfo
{
	int iSendList; // 邮件发送列表
	int iSendNum; // 待发送的邮件数量
	int iDeleteList; // 邮件删除列表
	int iDeleteNum; // 待删除的邮件数量
}PostofficeInfo;

typedef struct tagPostoffice
{
	Mailbox astMailboxs[MAIL_MAX_MAILBOX]; // 邮箱
	size_t nMailboxNum; // 邮箱总数量
	PostofficeInfo stInfo;
}Postoffice;
char sWatchPostoffice[POSTOFFICE_OBJ_DATA - sizeof(Postoffice)];


typedef struct tagMailboxHash
{
	char szOwner[ROLE_NAME_LEN]; // 邮箱拥有者角色名
	size_t nIdx; // 邮箱在Postoffice中的索引
}MailboxHash;

// 邮件系统 end

typedef struct tagZoneScriptIndex
{
	char szScriptName[32];
	ScriptIndex stScriptIdx;
}ZoneScriptIndex;


// 公告相关
#define BULLETIN_BOARD_OBJ_DATA sizeof(BulletinBoard)

typedef struct tagZoneBulletin
{
	BULLETINENTRY stCore;
	int iActiveStart;
	int iActiveEnd;
	int iFlag;
}ZoneBulletin;

typedef struct tagBulletinBoard
{
	ZoneBulletin astBulletinArray[MAX_BULLETIN_NUM];
	int iNum;
}BulletinBoard;

typedef enum tagGameEventStatus
{
	GAME_EVENT_START= 0,
	GAME_EVENT_RUN,
	GAME_EVENT_END,
}GAMEEVENTSTATUS;

typedef enum tagGameEventFlag
{
	GAME_EVENT_FLAG_SPY = 1, //刺探活动开启
}GAMEEVENTFLAG;


typedef struct tagGameEventInst
{
	int iEventID;
	char bStat;
	char cScriptFlag;
	char EventScriptVar[MAX_EVENT_SCRIPT_VAR];
	char szScriptName[32];
	ScriptIndex stScriptIdx;
	time_t tEventStart;
	int iFlag; //GAME_EVENT_FLAG_SPY
	int iPworldInstID; 
	tdr_ulonglong iPworldWID; 
	int iLuckyRandID;
}GameEventInst;

#define MAX_EVENT_STATUS 8
#define MAX_EVENT_BULLETIN 8
#define MAX_EVENT_PWORLD 8
#define MAX_EVENT_QUEUE 8
#define MAX_EVENT_BATTLE 8
#define WORLD_EVENT_DATA sizeof(WorldEvent)

typedef struct tagEventStatus
{
	int iStatusID;
	int iLevel;
	int iTime;
	int EventIdx;
}EventStatus;

typedef struct tagEventBulletin
{
	int iBulletinID;
	int EventIdx;
}EventBulletin;

typedef struct tagEventPworld
{
	int iPworldID;
	int EventIdx;
}EventPworld;

typedef struct tagEventPworldQueue
{
	int iPworldID;
	int EventIdx;
}EventPworldQueue;

typedef struct tagEventBattle
{
	int iBattleD;
	int EventIdx;
}EventBattle;

typedef struct tagWorldEvent
{
	int iStatusNum;
	EventStatus stStatus[MAX_EVENT_STATUS];
	int iBulletinNum;
	EventBulletin stBulletin[MAX_EVENT_BULLETIN];
	int iPworldNum;
	EventPworld stPworld[MAX_EVENT_PWORLD];
	int iQueueNum;
	EventPworldQueue stPworldQueue[MAX_EVENT_QUEUE];
	int iBattleNum;
	EventBattle stBattle[MAX_EVENT_BATTLE];
}WorldEvent;

typedef struct tagNormalTrait
{
	int iTraitNum;
	int TraitID[MAX_PET_TRAIT_DEF];
}NormalTrait;
#define NORMAL_TRAIT_DATA sizeof(NormalTrait)

#define MAX_PWORLD_APPLY_QUEUE 100
#define MAX_PWORLD_WAIT_QUEUE 100
#define MAX_APPLY_QUEUER 100
#define MAX_WAIT_QUEUER 100
#define PWORLD_QUEUE_DATA sizeof(WorldPworldQueue)

typedef struct
{
	int iQueuerID;
	int iTeamIdx;
	tdr_ulonglong ullTeamWID;
	int iLevel;
	int iInvite;
	time_t tValidTime;
}QueueInfo;

#define MAX_PWORLD_CLAN_QUEUE 100
#define MAX_CLAN_QUEUER 100

typedef struct
{
	short iPworldFlag; // 跨服为1     否则0
	short iQueuerNum;
	int iPworldID;
	int iQueuerType; //0为玩家 1为公会 PWORLD_APPLY_UNIT_TYPE
	QueueInfo stQueuerInfo[MAX_APPLY_QUEUER];
}PworldApplyQueue;

typedef struct tagPworldWaitQueuerInfo
{
	//int iQueuerID;
	QueueInfo stRoleInfo;
	int iCamp;	
	time_t tValidTime;
}PworldWaitQueuerInfo;

typedef struct tagPworldWaitQueue
{
	int iPworldInstID;
	int iQueuerNum;
	int iQueuerType; //0为玩家 1为公会 PWORLD_APPLY_UNIT_TYPE
	PworldWaitQueuerInfo  stQueuerInfo[MAX_WAIT_QUEUER];

	int iCreateTime;
	int iLiftTime;					// 战场副本生存时间,这里记录一下,避免每次tick都去查表
	char PworldCampID;                    	/*   副本的阵营id,campnum分配过来的 */
	int iPlayerMax;
	int iPlayerNum;
	int iPworldID;
       int iBattleBusID;                     	/*   这个busid要记下来 */
	tdr_ulonglong PworldWID;         /*   副本的实例ID */
	PworldCamp stPworldCamp;

}PworldWaitQueue;

typedef struct
{
	short iPworldFlag; // 跨服为1     否则0
	short iQueuerNum;
	int iPworldID;
	int iPworldInstID;
	CLANID stClanID;//公会id
	int iQueueType;//PWORLD_CLAN_QUEUE iQueueIdx对应的类型 0为Apply 1为Wait
	int iQueueIdx;//队列的索引
	int iMatchStat;//匹配状态 PWORLD_SVR_MATCH_STATUS
	int iBusID;//master 的busid
	int iSrcMaxLevel;//master服务器的最大封印等级
	int iBattleID;
	int iCampIdx;
	QueueInfo stQueuerInfo[MAX_CLAN_QUEUER];
}PworldClanQueue;

typedef struct tagWorldPworldQueue
{
	PworldClanQueue stClanQueue[MAX_PWORLD_CLAN_QUEUE];
	PworldApplyQueue stApplyQueue[MAX_PWORLD_APPLY_QUEUE];
	PworldWaitQueue	stWaitQueue[MAX_PWORLD_WAIT_QUEUE];
}WorldPworldQueue;


//时空深渊相关
#define MAX_SHIKONG_APPLY_QUEUE 1       // 多少个报名队列
#define MAX_SHIKONG_APPLY_QUEUE_MEM 200   // 每个报名队列多少人
#define MAX_SHIKONG_WAIT_QUEUE 150       // 多少个等待队列
#define MAX_SHIKONG_WAIT_QUEUE_MEM 20   //每个等待队列多少人
#define SHIKONG_PWORLD_OPEN_NUM 10
#define SHIKONG_PWORLD_MEM_NUM 12

typedef struct
{
	time_t  tValidTime;
	int iMemID;
	int iTeamIdx;
	tdr_ulonglong ullTeamWID;
}ShiKongQueueMem;

typedef struct
{
	int iPworldInstID;
	int iPworldID;
	time_t  tStarTime;
	int iMemNum;
	ShiKongQueueMem astMem[MAX_SHIKONG_WAIT_QUEUE_MEM];
}ShiKongWaitQueue;

typedef struct
{	
	int iPworldID;
	int iApplyNum;
	ShiKongQueueMem astMem[MAX_SHIKONG_APPLY_QUEUE_MEM];
}ShiKongApplyQueue;

typedef struct
{
	ShiKongApplyQueue astApplyQueue[MAX_SHIKONG_APPLY_QUEUE];
	ShiKongWaitQueue astWaitQueue[MAX_SHIKONG_WAIT_QUEUE];
}ShiKongQueue;

#define SHIKONG_QUEUE_DATA sizeof(ShiKongQueue)


#define MAX_DIATHESIS_DEF 2500

typedef struct tagRangeEntry
{
	int iDirty;
	TOPRANGE stRange;
}RangeEntry;

typedef struct tagRangeOne
{
	int iNum;
	RangeEntry astRangeEntry[RANGE_ONE_NUM];
	int aiPageVersion[RANGE_MAX_PAGE];
}RangeOne;

typedef struct tagRangeOneMem
{
	int iMainType;
	int iSubType;
	RangeOne stLastRange;
	RangeOne stCurrRange;
}RangeOneMem;

#define RANGE_MAX 50

typedef struct tagRangeMem
{
	int iState;
	int iDBNum;
	int iDBRecvNum;
	int iRangeNum;
	RangeOneMem astRange[RANGE_MAX];
}RangeMem;

typedef struct tagAutoTeamSingle
{
	int iPlayerID;
	short nLimitFlag; //条件标记
	unsigned char bCareer; //职业
}AutoTeamSingle;

#define AUTO_TEAM_NEED_MEMBER (MAX_TEAM_MEMBER-2)

typedef struct tagAutoTeamCreated
{
	int iTeamID;
	short nLimitFlag; //条件标记
	unsigned char OpFlag; //主要用于锁定完成创建
	unsigned char nSingleNum;
	AutoTeamSingle  astSingle[AUTO_TEAM_NEED_MEMBER];
}AutoTeamCreated;

typedef struct tagAutoTeamPre
{
	time_t tTime;
	unsigned char OpFlag; //主要用于锁定完成创建
	unsigned char nSingleNum;
	AutoTeamSingle  astSingle[MAX_TEAM_MEMBER];
}AutoTeamPre;

typedef struct tagAutoTeamOne
{
	int iSingleNum;
	AutoTeamSingle astAutoTeamSingle[AUTO_TEAM_SINGLE_NUM];
	
	int iCreatedTeamNum;
	AutoTeamCreated astAutoTeamCreated[AUTO_TEAM_CREATED_NUM];

	int iPreTeamNum;
	AutoTeamPre astAutoTeamPre[AUTO_TEAM_PRE_NUM];
}AutoTeamOne;

typedef struct tagAutoTeam
{
	AutoTeamOne astAutoTeams[AUTO_TEAM_ID_MAX];
}AutoTeam;

#define MAX_ACTIVE_HELP 80
#define ACTIVE_HELP_DATA 2048

#define MAX_ONE_ADDATTR_LIST 100

typedef struct
{
	int iAddAttrNum;
	int iDefID[MAX_ONE_ADDATTR_LIST];
}OneAddAttrList;

typedef struct
{
	OneAddAttrList astRandList[MAX_ADD_ATTR_WEARPOS_LIST][MAX_ADD_ATTR_CAREER][MAX_ADD_ATTR_LEVEL_LIST][MAX_ADD_ATTR_IMPORTANT];
	OneAddAttrList astWashList[MAX_ADD_ATTR_WEARPOS_LIST][MAX_ADD_ATTR_CAREER][MAX_ADD_ATTR_LEVEL_LIST][MAX_ADD_ATTR_IMPORTANT];
}AddAttrFastList;


#define MAX_ONE_ARMATTR_LIST 30

typedef struct
{
	int iNum;
	int iDefID[MAX_ONE_ADDATTR_LIST];
}OneArmAttrList;

typedef struct
{
	OneArmAttrList astRandList[MAX_ARM_SKILL_NUM][MAX_ADD_ATTR_WEARPOS_LIST][MAX_ADD_ATTR_CAREER][MAX_ADD_ATTR_IMPORTANT];
}ArmSkillAttrFast;

#define ADD_ATTR_FAST_LIST_DATA sizeof(AddAttrFastList)


#define MAX_ONE_LIST_SJ_TASK 80

typedef struct
{
	int iNum;
	int iTaskID[MAX_ONE_LIST_SJ_TASK];
}OneSJTaskList;

typedef struct
{
	OneSJTaskList stOneList[MAX_SJ_TASK_LIST];
}SJTaskFastList;


#define SJ_TASK_LIST_DATA sizeof(SJTaskFastList)

/* Sparta Online */

//#define MAX_SPARTA_TEAM_MEM 3
#define MAX_SPARTA_APPLY_QUEUE_TEAM 1000
#define MAX_SPARTA_WAIT_QUEUE_TEAM 500

typedef struct
{
	int iUin;
	tdr_ulonglong ullRoleID;
	char szRoleName[ROLE_NAME_LEN]; // 邮箱拥有者角色名
	int iVal;
}SpartaQueueTeamMem;

typedef struct
{
	tdr_ulonglong ullTeamID;
	int iTeamVal;
	time_t tApplyTime;
	SpartaQueueTeamMem astMem[MAX_SPARTA_TEAM_MEM];
}SpartaQueueTeam;

typedef struct
{
	int iUin;
	tdr_ulonglong ullRoleID;
	char szRoleName[ROLE_NAME_LEN]; // 邮箱拥有者角色名
	int iVal;
	int iReady;
}SpartaWaitQueueTeamMem;

typedef struct
{
	tdr_ulonglong ullTeamID;
	int iTeamVal;
	SpartaWaitQueueTeamMem astMem[MAX_SPARTA_TEAM_MEM];
}SpartaWaitQueueTeam;

typedef struct
{
	time_t  tStarTime;
	SpartaWaitQueueTeam stSpartaTeam1;
	SpartaWaitQueueTeam stSpartaTeam2;
}SpartaWaitQueueInfo;

typedef struct
{
	int iApplyNum;
	SpartaQueueTeam astApplyQueue[MAX_SPARTA_APPLY_QUEUE_TEAM];

	int iWaitNum;
	SpartaWaitQueueInfo astWaitQueue[MAX_SPARTA_WAIT_QUEUE_TEAM];
}SpartaQueue;

#define SPARTA_QUEUE_DATA sizeof(SpartaQueue)

/* Sparta Online end */

#define SHOP_MSG_DATA 2056
char sWatchShopMsg[SHOP_MSG_DATA - sizeof(SHOPMSGDEF)];

#define MAX_LEVEL_FENG_DEF MAX_LEVEL
#define LEVEL_FENG_DEF_DATA 128 
char sWatchLevelFeng[LEVEL_FENG_DEF_DATA - sizeof(LEVELFENG)];

//跑商价格道具左边数据
typedef struct {
	int iLeftItemID;    //道具id
	int iUserBuyPrice;  //现在用户购买需要的价格
	int iBuyRemainNum;			//还剩多少数量
}PAOSHANGLEFTDATA;

//跑商价格道具右边数据
typedef struct {
	int iRightItemID;    //道具id
	int iUserSellPrice;  //现在用户卖出的价格
}PAOSHANGRIGHTDATA;

//跑商id对应价格数据
typedef struct {
	int iPaoShangID;    //跑商id
	int iSellNum;  // npc卖的道具总量
	PAOSHANGLEFTDATA stPaoShangLeftData[MAX_PAOSHANG_NPC_SHELL_ITEM_NUM];
	int ItemTotalNum; //全部道具总量
	PAOSHANGRIGHTDATA stPaoShangRightData[MAX_PAOSHANG_DAOJU_NUM];
}PAOSHANGGLOBALDATA;

typedef struct {
	int PaoShangRefTime;  //跑商刷新生成时间
	int iNpcNum;  //跑商npc数量	
	PAOSHANGGLOBALDATA stPaoShangGloData[MAX_PAOSHANG_NPC_NUM];
} PAOSHANGINFO;

typedef struct {
	int iNum;
	int iTime;
} SpartaInfo;

typedef struct tagZoneObj
{
	int iVersion;
	int iMemID;
	IDIndex stIndex[ID_INDEX];

	time_t tMonsterDef;
	int iMonsterDef;

	time_t tMonsterStateSkillDef;
	int iMonsterStateSkillDef;
	
	time_t tItemDef;
	int iItemDef;

	time_t tAddAttrDef;
	int iAddAttrDef;

	time_t tSuitSetDef;
	int iSuitSetDef;

	time_t tSellDef;
	int iSellDef;

	time_t tMapTransDef;
	int iMapTransDef;

	time_t tBirthMapDef;
	int iBirthMapDef;

	time_t tActionStatusDef;
	int iActionStatusDef;

	time_t tLevelUpDef;
	int iLevelUpDef;

	time_t tArmLevelHurtDef;
	int iArmLevelHurtDef;

	time_t tArmFenJieDef;
	int iArmFenJieDef;
	
	time_t tSkillDef;
	int iSkillDef;

	time_t tDiathesisDef;
	int iDiathesisDef;

	time_t tFittingsDef;
	int iFittingsDef;
	
	time_t tStatusDef;
	int iStatusDef;

	time_t tOverlayTypeRelationDef;
	int      iOverlayTypeRelationDef;

	time_t tStatusOverlayDef;
	int      iStatusOverlayDef;

	time_t tMonDynAttrDef;
	int      iMonDynAttrDef;

	time_t tPatrolDef;
	int      iPatrolDef;

	time_t tTaskDef;
	int      iTaskDef;

	time_t tTaskCycleDef;
	int      iTaskCycleDef;

	time_t tNpcInfoDef;
	int      iNpcInfoDef;

	time_t tPetDef;
	int iPetDef;

	time_t tPetLevelDef;
	int iPetLevelDef;

	time_t tPetTraitDef;
	int iPetTraitDef;

	time_t tPetStrengthenDef;
	int iPetStrengthenDef;

	time_t tQiYueDef;
	int iQiYueDef;

	time_t tMonDynExpLimitDef;
	int iMonDynExpLimitDef;

	time_t tArmGradeDef;
	int iArmGradeDef;

	time_t tArmRandDef;
	int iArmRandDef;

	time_t tArmHideAttrDef;
	int iArmHideAttrDef;

	time_t tBirthItemDef;
	int iBirthItemDef;

	time_t tMonsterSpeechDef;
	int iMonsterSpeechDef;

	time_t tArmAddAttrDef;
	int iArmAddAttrDef;

	time_t tOnlineAwardDef;
	int iOnlineAwardDef;

	time_t tTaskLimitGroup;
	int iTaskLimitGroup;

	time_t tClanCityBuildingLevelDef;
	int    iClanCityBuildingLevelDef;

	time_t tClanCityLevelDef;
	int    iClanCityLevelDef;

	time_t tClanLevelDef;
	int    iClanLevelDef;

	time_t tMonsterColorDef;
	int    iMonsterColorDef;

	time_t tGameEventDef;
	int    iGameEventDef;

	time_t tDesignationDef;
	int    iDesignationDef;

	time_t tRespactPartyDef;
	int    iRespactPartyDef;

	time_t tRespactPhaseDef;
	int    iRespactPhaseDef;

	time_t tRideDef;
	int    iRideDef;

	time_t tFairyDef;
	int    iFairyDef;

	time_t tAddAttrBaseDef;
	int    iAddAttrBaseDef;

	time_t tAddAttrGroupDef;
	int    iAddAttrGroupDef;

	time_t tLVGift;
	int iLVGift;

	time_t tMediaGiftDef;
	int iMediaGiftDef;

	time_t tLtrGiftDef;
	int iLtrGiftDef;
	
	time_t tOneDayGift;
	int iOneDayGift;

	time_t tRndAwdDef;
	int iRndAwdDef;

	time_t tBCTDef;
	int iBCTDef;

	time_t tFixedAddAttr;
	int iFixedAddAttr;

	time_t tSecondarySkillDef;
	int iSecondarySkillDef;

	time_t tRecipeDef;
	int iRecipeDef;

	time_t tActiveHelp;
	int iActiveHelp;

	time_t tMonBootyDef;
	int iMonBootyDef;

	time_t tMonBootyGroupDef;
	int iMonBootyGroupDef;

	time_t tCangBaoTuDef;
	int iCangBaoTuDef;

	time_t tMonMachineDef;
	int iMonMachineDef;

	time_t tClanTechDef;
	int iClanTechDef;

	time_t tClanProductDef;
	int iClanProductDef;

	time_t tAttrContrastDef;
	int iAttrContrastDef;

	time_t tLevelAttrDef;
	int iLevelAttrDef;

	time_t tDiePunishDef;
	int iDiePunishDef;

	time_t tContOnlineDef;
	int iContOnlineDef;

	time_t tBattleDef;
	int iBattleDef;

	time_t tFairyStuffDef;
	int iFairyStuffDef;

	time_t tFairyStarDef;
	int iFairyStarDef;

	time_t tVipBarDef;
	int iVipBarDef;

	time_t tPetResolveDef;
	int iPetResolveDef;

	time_t tPetNativeSkillDef;
	int iPetNativeSkillDef;

	time_t tPetSkillGridDef;
	int iPetSkillGridDef;

	time_t tPetFuseDef;
	int iPetFuseDef;

	time_t tOfflineExpParameter;
	int iOfflineExpParameter;

	time_t tOnlineGiftGroup;
	int iOnlineGiftGroup;

	time_t tWorldItemLimitDef;
	int iWorldItemLimitDef;
	
	time_t tWorldBootyGroupLimitDef;
	int iWorldBootyGroupLimitDef;
	
	time_t tDiffPworldDef;
	int iDiffPworldDef;

	time_t tMonEventDef;
	int iMonEventDef;

	time_t tPworldBootyGroupDef;
	int iPworldBootyGroupDef;

	time_t tPworldAwardMulDef;
	int iPworldAwardMulDef;

	time_t tAtkerHurtMulDef;
	int iAtkerHurtMulDef;
	
	time_t tWorldCityBuildDef;
	int iWorldCityBuildDef;

	time_t tStarStoneDef;
	int iStarStoneDef;

	time_t tStarFuncDef;
	int iStarFuncDef;

	time_t tCityProductDef;
	int iCityProductDef;

	time_t tBattleGlory;
	int iBattleGlory;

	time_t tGemNewDef;
	int iGemNewDef;

	time_t tGemAttrStarDef;
	int iGemAttrStarDef;

	time_t tGemLevelStarDef;
	int iGemLevelStarDef;

	time_t tArmRecastDef;
	int iArmReCastDef;

	time_t tUseDef;
	int iUseDef;

	time_t tDailyGoalAward;
	int iDailyGoalAward;

	time_t tRuneAddAttrDef;
	int iRuneAddAttrDef;

	time_t tArmStarDef;
	int iArmStarDef;

	time_t tPetWashGeniusDef;
	int iPetWashGeniusDef;

	time_t tUnrealMapDef;
	time_t tUnrealDef;
	int iUnrealMapDef;
	int iUnrealDef;

	time_t tNewAddAttrDef;
	int iNewAddAttrDef;

	time_t tAddAttrRandDef;
	int iAddAttrRandDef;



	time_t tAchievementCategoryDef;
	int iAchievementCategoryDef;

	time_t tAchievementDef;
	int iAchievementDef;


	time_t tAddAttrWashDef;
	int iAddAttrWashDef;

	time_t tSparDef;
	int iSparDef;

	time_t tItemStarRandDef;
	int iItemStarRandDef;

	time_t tSJTaskDef;
	int iSJTaskDef;

	time_t tClanSpyAwardDropDef;
	int iClanSpyAwardDropDef;

	time_t tClanSpyTransDef;
	int iClanSpyTransDef;

	time_t tDartCarDef;
	int iDartCarDef;

	time_t tPworldWaveMonDef;
	int iPworldWaveMonDef;

	time_t tPworldWaveMonPosDef;
	int iPworldWaveMonPosDef;

	time_t tPworldWaveMonClanAwardDef;
	int iPworldWaveMonClanAwardDef;

	time_t tGayDef;
	int iGayDef;

	time_t tPworldMachineBuffShopDef;
	int iPworldMachineBuffShopDef;

	time_t tPlayerAwardDef;
	int iPlayerAwardDef;

	time_t tCowFinalAwardDef;
	int iCowFinalAwardDef;

	time_t tGmTrade;
	int iGmTrade;

	time_t tClanSkillDef;
	int iClanSkillDef;

	time_t tClanPayDef;
	int iClanPayDef;

	time_t tPetLoveLevelDef;
	int iPetLoveLevelDef;

	time_t tLuckyMapDef;
	int iLuckyMapDef;

	time_t tClanAwardDef;
	int iClanAwardDef;

	time_t tClanWarAwardDef;
	int iClanWarAwardDef;
	
	int iGdCacheHead;
	int iGdCacheTail;

	time_t tSubJectDef;
	int iSubJectDef;

	time_t tSubJectTypeDef;
	int iSubJectTypeDef;

	time_t tRangDsnDef;
	int iRangDsnDef;

	time_t tStrongPointDef;
	int iStrongPointDef;

	time_t tBannerDef;
	int iBannerDef;

	time_t tReviveDef;
	int iReviveDef;

	time_t tLevelExpDef;
	int iLevelExpDef;

	time_t tLevelBuffExpDef;
	int iLevelBuffExpDef;

	time_t tRidePinZhiDef;
	int iRidePinZhiDef;

	time_t tMapAwardOreDef;
	int iMapAwardOreDef;

	time_t tRongluDef;
	int iRongluDef;

	time_t tGeniusCareerDef;
	int iGeniusCareerDef;

	time_t tGeniusAttrDef;
	int iGeniusAttrDef;

	time_t tBaoXiangDef;
	int iBaoXiangDef;

	time_t tWorldCityFightAwardDef;
	int iWorldCityFightAwardDef;

	time_t tWorldBossDef;
	int iWorldBossDef;

	time_t tLevelFengDef;
	int iLevelFengDef;

	time_t tArmLevelUPDef;
	int iArmLevelUPDef;

	time_t tBusyDef;
	int iBusyDef;

	time_t tBusyUseDef;
	int iBusyUseDef;

	time_t tItemIDDef;
	int iItemIDDef;

	time_t tCareerHurtDef;
	int iCareerHurtDef;

	time_t tGeniusAddDef;
	int iGeniusAddDef;

	time_t tHighWashUseDef;
	int iHighWashUseDef;

	time_t tPetGeniusDef;
	int iPetGeniusDef;

	time_t tWeDef;
	int iWeDef;

	time_t tExchgItemDef;
	int iExchgItemDef;

	time_t tSpartaAwardDef;
	int iSpartaAwardDef;

	time_t tSpartaLevelDef;
	int iSpartaLevelDef;

	time_t tMapRandTransmitDef;
	int iMapRandTransmitDef;

	time_t tSchoolLevelDef;
	int iSchoolLevelDef;

	time_t tSchoolPworldDef;
	int iSchoolPworldDef;

	time_t tSchoolXinDeDef;
	int iSchoolXinDeDef;

	time_t tBackLevelDef;
	int iBackLevelDef;

	time_t tXuKongPosDef;
	int iXuKongPosDef;

	time_t tNewRoleGiftDef;
	int iNewRoleGiftDef;

	time_t tContCoverGiftDef;
	int iContCoverGiftDef;

	time_t tCardBoxDef;
	int iCardBoxDef;

	time_t tCardBoxGroupDef;
	int iCardBoxGroupDef;

	time_t tArmBuChangDef;
	int iArmBuChangDef;

	time_t tBuChangDef;
	int iBuChangDef;

	time_t tPworldBoxDef;
	int iPworldBoxDef;

	time_t tYangHaiBangDef;
	int iYangHaiBangDef;

	time_t tHangHaiAwardDef;
	int iHangHaiAwardDef;

	time_t tStoreGridDef;
	int iStoreGridDef;

	time_t tArmStepUpDef;
	int iArmStepUpDef;

	time_t tArmGradeValDef;
	int iArmGradeValDef;

	time_t tArmReSetDef;
	int iArmReSetDef;

	time_t tRongYanJuShouDef;
	int iRongYanJuShouDef;

	time_t tRongYanMonDynAttrDef;
	int iRongYanMonDynAttrDef;

	time_t tArmSkillNumDef;
	int iArmSkillNumDef;

	time_t tArmSkillAttrDef;
	int iArmSkillAttrDef;

	time_t tGeniusMdfDef;
	int iGeniusMdfDef;

	time_t tPaoShangDef;
	int iPaoShangDef;

	time_t tPaoShangItemDef;
	int iPaoShangItemDef;

	time_t tPetMoneyDef;
	int iPetMoneyDef;

	time_t tPetImpUpAttrDef;
	int iPetImpUpAttrDef;

	time_t tPetImpUpDef;
	int iPetImpUpDef;

	time_t tPetOpAwardDef;
	int iPetOpAwardDef;

	time_t tGemPagDef;
	int iGemPagDef;

	time_t tArmTeshuDef;
	int iArmTeshuDef;

	time_t tZJRongYanJuShouDef;
	int iZJRongYanJuShouDef;

	int iSpanStrongDef;
	time_t tSpanStrongDef;

	time_t tWordPwdDef;
	int iWordPwdDef;

	time_t tHomeLvlDef;
	int iHomeLvlDef;

	time_t tHomeMonLvlDef;
	int iHomeMonLvlDef;

	time_t tHomeMonSegDef;
	int iHomeMonSegDef;

	time_t tHomeFarmDef;
	int iHomeFarmDef;

	time_t tHomeSoilDef;
	int iHomeSoilDef;

	time_t tArmSkillRecastDef;
	int  iArmSkillRecastDef;
	
	char sWorldEvent[WORLD_EVENT_DATA];
	char sPworldQueue[PWORLD_QUEUE_DATA];

	char sShopMsgDef[SHOP_MSG_DATA];

	/*----initzero 以上区域会在生成共享内存时被初始化为0*/
	int iInitRes;

	/*--下面是定义data--*/
	//怪物定义
	char sMonsterDef[MAX_MONSTER_DEF][MONSTER_DEF_DATA];

	//怪物阶段技能定义
	char sMonsterStateSkillDef[MAX_MONSTERSTATESKILL_DEF][MONSTERSTATESKILL_DEF_DATA];

	//物品定义
	char sItemDef[MAX_ITEM_DEF][ITEM_DEF_DATA];

	//附加属性定义
	char sAddAttrDef[MAX_ADDATTR_DEF][ADDATTR_DEF_DATA];

	//套装定义
	char sSuitSetDef[MAX_SUITSET_DEF][SUITSET_DEF_DATA];

	//售卖列表定义
	char sSellDef[MAX_SELL_DEF][SELL_DEF_DATA];

	//传送列表定义
	char sMapTransDef[MAX_MAP_TRANS_DEF][MAP_TRANS_DEF_DATA];

	//新手村定义
	char sBirthMapDef[MAX_BIRTHMAP_DEF][BIRTHMAP_DEF_DATA];

	//行动状态定义
	char sActionStatusDef[RES_STATUS_MAX][ACTION_STATUS_DEF_DATA];

	//升级效果表
	char sLevelUpDef[MAX_LEVEL][LEVELUP_DEF_DATA];

	//武器等级伤害修正表
	char sArmLevelHurtDef[MAX_ARM_LEVEL_HURT][ARM_LEVEL_HURT_DATA];

	//技能定义
	char sSkillDef[MAX_SKILL][SKILL_DATA];

	//素质定义
	char sDiathesisDef[MAX_DIATHESIS_DEF][DIATHESIS_DATA];

	//配件定义
	char sFittingsDef[MAX_SYS_FITTINGS][FITTINGS_DATA];

	//状态定义
	char sStatusDef[MAX_STATUS][STATUS_DATA];

	//状态关系定义
	char    sOverlayTypeRelationDef[MAX_OVERLAYTYPE_RELATION][OVERLAYTYPE_RELATION_DATA];

	//状态叠加指数定义
	char    sStatusOverlayDef[MAX_STATUS_OVERLAY][STATUS_OVERLAY_DATA];

	//怪物动态属性修正定义
	char    sMonDynAttrDef[MAX_LEVEL][MON_DYN_ATTR_DATA];

	//巡逻路径定义
	char    sPatrolDef[MAX_PATROL][PATROL_DATA];

	//任务定义
	char    sTaskDef[MAX_TASK][TASK_DATA];

	//任务环组定义
	char    sTaskCycleDef[MAX_TASK_CYCLE][TASK_CYCLE_DATA];

	//npc附加信息定义
	char    sNpcInfoDef[MAX_NPC_INFO][NPC_INFO_DATA];
	
	//宠物定义
	char sPetDef[MAX_PET][PET_DATA];

	//宠物等级定义
	char sPetLevelDef[MAX_PETLEVEL][PETLEVEL_DATA];

	//宠物性格定义
	char sPetTraitDef[MAX_PET_TRAIT_DEF][PET_TRAIT_DEF_DATA];

	//宠物强化定义
	char sPetStrengthenDef[MAX_PET_STRENGTHEN_DEF][PET_STRENGTHEN_DEF_DATA];

	// 契约定义表
	char sQiYueDef[MAX_QIYUE][QIYUE_DEF_DATA];

	// 动态怪物经验上限定义表
	char sMonDynExpLimitDef[MAX_MONDYNEXPLIMIT][MONDYNEXPLIMIT_DATA];

	// 装备提升表
	char sArmGradeDef[MAX_ARMGRADE_DEF][ARMGRADE_DATA];

	// 装备概率表
	char sArmRandDef[MAX_ARMRAND_DEF][ARMRAND_DATA];

	// 装备隐藏属性表
	char sArmHideAttrDef[MAX_ARMHIDEATTR_DEF][ARMHIDEATTR_DATA];

	// 装备分解表
	char sArmFenJieDef[MAX_ARMFENJIE_DEF][ARMFENJIE_DATA];

	//出生道具定义表
	char sBirthItemDef[MAX_BIRTH_ITEM_DEF][BIRTH_ITEM_DEF_DATA];

	//怪物发言定义表
	char sMonsterSpeechDef[MAX_MONSTER_SPEECH_DEF][MONSTER_SPEECH_DEF_DATA];

	//装备关键字定义表
	char sArmAddAttrDef[MAX_ARM_ADDATTR_DEF][ARM_ADDATTR_DEF_DATA];

	//在线奖励定义表
	char sOnlineAwardDef[MAX_ONLINE_AWARD_DEF][ONLINE_AWARD_DEF_DATA];

	// 任务限制组定义表
	char sTaskLimitGroup[MAX_TASK_LIMIT_GROUP][TASK_LIMIT_GROUP_DATA];
	
	// 公会城市建筑升级定义表
	char   sClanCityBuildingLevelDef[MAX_CLAN_CITY_BUILDING_LEVEL_DEF][CLAN_CITY_BUILDING_LEVEL_DEF_DATA];

	// 公会城市升级定义表
	char   sClanCityLevelDef[MAX_CLAN_CITY_LEVEL_DEF][CLAN_CITY_LEVEL_DEF_DATA];

	// 公会升级定义表
	char   sClanLevelDef[MAX_CLAN_LEVEL_DEF][CLAN_LEVEL_DEF_DATA];

	// 怪物颜色战利品修正定义表
	char   sMonsterColorDef[MAX_MONSTER_COLOR_DEF][MONSTER_COLOR_DEF_DATA];

	// 游戏活动定义表
	char   sGameEventDef[MAX_GAME_EVENT_DEF][GAME_EVENT_DEF_DATA];

	// 称号定义表
	char   sDesignationDef[MAX_DESIGNATION_DEF][DESIGNATION_DEF_DATA];

	// 声望势力定义表
	char   sRespactPartyDef[MAX_RESPACT_PARTY_DEF][RESPACT_PARTY_DEF_DATA];

	// 声望阶段定义表
	char   sRespactPhaseDef[MAX_RESPACT_PHASE_DEF][RESPACT_PHASE_DEF_DATA];

	// 骑乘定义表
	char   sRideDef[MAX_RIDE_DEF][RIDE_DEF_DATA];

	// 精灵定义表
	char   sFairyDef[MAX_FAIRY_DEF][FAIRY_DEF_DATA];

	// 关键字基值定义表
	char   sAddAttrBaseDef[MAX_ADD_ATTR_BASE_DEF][ADD_ATTR_BASE_DEF_DATA];

	// 关键字随即组定义表
	char   sAddAttrGroupDef[MAX_ADD_ATTR_GROUP_DEF][ADD_ATTR_GROUP_DEF_DATA];

	// 等级奖励表
	char sLVGift[MAX_GIFT_LV_DEF][GIFT_LV_DATA];

	//媒体礼包定义表
	char sMediaGiftDef[MAX_GIFT_MEDIA_DEF][GIFT_MEDIA_DATA];

	//媒体抽奖次数礼包定义表
	char sLtrGiftDef[MAX_GIFT_LTR_DEF][GIFT_LTR_DATA];
	
	// 每日奖励表
	char sOneDayGift[MAX_GIFT_ONEDAY_DEF][GIFT_ONEDAY_DATA];

	// 随机道具表(抽奖)
	char sRndAwdDef[MAX_RAND_AWARD_DEF][RAND_AWARD_DATA];

	// 白层塔
	char sBCTDef[MAX_BCT_DEF][BCT_DEF_DATA];

	// 固定关键字表
	char sFixedAddAttr[MAX_FIXED_ADD_ATTR][FIXED_ADD_ATTR_DATA];

	// 生活技能定义表
	char sSecondarySkillDef[MAX_SECONDARY_SKILL_DEF][SECONDARY_SKILL_DEF_DATA];

	// 生活技能配方定义表
	char sRecipeDef[MAX_RECIPE_DEF][RECIPE_DEF_DATA];

	//活动助手定义表
	char sActiveHelp[MAX_ACTIVE_HELP][ACTIVE_HELP_DATA];

	//怪物战利品定义表
	char sMonBootyDef[MAX_MON_BOOTY_DEF][MON_BOOTY_DEF_DATA];

	//战利品组定义表
	char sMonBootyGroupDef[MAX_MON_BOOTY_GROUP_DEF][MON_BOOTY_GROUP_DEF_DATA];

	//藏宝图定义表
	char sCangBaoTuDef[MAX_CANGBAOTU_DEF][CANGBAOTU_DEF_DATA];

	//怪物机械定义表
	char sMonMachineDef[MAX_MON_MACHINE_DEF][MON_MACHINE_DEF_DATA];

	//公会科技定义表
	char sClanTechDef[MAX_CLAN_TECH_DEF][CLAN_TECH_DEF_DATA];

	//公会产品定义表
	char sClanProductDef[MAX_CLAN_PRODUCT_DEF][CLAN_PRODUCT_DEF_DATA];

	//一级属性转换对照表
	char sAttrContrastDef[MAX_ATTR_CONTRAST_DEF][ATTR_CONTRAST_DEF_DATA];

	//等级提升属性定义表
	char sLevelAttrDef[MAX_LEVEL_ATTR_DEF][LEVEL_ATTR_DEF_DATA];

	//死亡惩罚表
	char sDiePunishDef[MAX_DIE_PUNISH_DEF][DIE_PUNISH_DEF_DATA];

	//连续每日在线多久定义
	char sContOnlineDef[MAX_CONT_ONLINE_DEF][CONT_ONLINE_DEF_DATA];

	//普通战场定义表
	char sBattleDef[MAX_BATTLE_DEF][BATTLE_DEF_DATA];

	// 精灵升级材料表
	char sFairyStuffDef[MAX_FAIRY_STUFF_DEF][FAIRY_STUFF_DEF_DATA];

	// 精灵品质表
	char sFairyStarDef[MAX_FAIRY_STAR_DEF][FAIRY_STAR_DATA];

	//宠物分解定义表
	char sPetResolveDef[MAX_PET_RESOLVE_DEF][PET_RESOLVE_DEF_DATA];

	//宠物技能格定义表
	char sPetSkillGridDef[MAX_PET_SKILL_GRID_DEF][PET_SKILL_GRID_DEF_DATA];

	//宠物天赋技能定义表
	char sPetNativeSkillDef[MAX_PET_NATIVE_SKILL_DEF][PET_NATIVE_SKILL_DEF_DATA];

	//宠物融合定义表
	char sPetFuseDef[MAX_PET_FUSE_DEF][PET_FUSE_DEF_DATA];
	//离线经验公式表
	char sOfflineExpParameter[MAX_OFFLINE_EXP_PARAMETER][OFFLINE_EXP_PARAMETER_DATA];

	// 在线奖励组
	char sOnlineGiftGroup[MAX_GIFT_GROUP_DEF][MAX_ONLINE_GROUP_DATA];

	//脚本怪物 产出物品限制
	char sWorldItemLimitDef[MAX_WORLD_ITEM_LIMIT_DEF][WORLD_ITEM_LIMIT_DEF_DATA];

	//怪物战利品组产出限制
	char sWorldBootyGroupLimitDef[MAX_WORLD_BOOTY_GROUP_LIMIT_DEF][WORLD_BOOTY_GROUP_LIMIT_DEF_DATA];

	//难度副本定义表
	char sDiffPworldDef[MAX_DIFF_PWORLD_DEF][DIFF_PWORLD_DEF_DATA];

	//难度副本定义表
	char sMonEventDef[MAX_MON_EVENT_DEF][MON_EVENT_DEF_DATA];

	//副本评价战利品库
	char sPworldBootyGroupDef[MAX_PWORLD_BOOTY_GROUP_DEF][PWORLD_BOOTY_GROUP_DEF_DATA];

	//副本评价奖励系数战利品
	char sPworldAwardMulDef[MAX_PWORLD_AWARD_MUL_DEF][PWORLD_AWARD_MUL_DEF_DATA];

	//攻防修正表
	char sAtkerHurtMulDef[MAX_ATKER_HURT_MUL_DEF][ATKER_HURT_MUL_DEF_DATA];

	//城池建筑
	char sWorldCityBuildDef[MAX_WORLD_CITY_BUILD_DEF][WORLD_CITY_BUILD_DEF_DATA];

	//砖定义表
	char sStarStoneDef[MAX_STARSTONE_DEF][MAX_STARSTONE_DEF_DATA];

	//各种砖/vip的功能定义表
	char sStarFuncDef[MAX_STARFUNC_DEF][MAX_STARFUNC_DEF_DATA];

	//城池建设产品表
	char sCityProductDef[MAX_CITY_PRODUCT_DEF][CITY_PRODUCT_DEF_DATA];

	// 战场奖励
	char sBattleGlory[MAX_BATTLE_GLORY_DEF][BATTLE_GLORY_DEF_DATA];

	// 新的宝石定义表
	char sGemNewDef[MAX_GEM_NEW_DEF][GEM_NEW_DEF_DATA];
	
	// 宝石属性数值对照表
	char sGemAttrStarDef[MAX_GEM_ATTR_STAR_DEF][GEM_ATTR_STAR_DEF_DATA];

	// 宝石等级星级对照表
	char sGemLevelStarDef[MAX_GEM_LEVEL_STAR_DEF][GEM_LEVEL_STAR_DEF_DATA];

	//装备重铸定义表
	char sArmRecastDef[MAX_ARM_RECAST_DEF][ARM_RECAST_DEF_DATA];

	// 各种消耗定义表
	char sUseDef[MAX_USE_DEF][USE_DEF_DATA];

	char sDailyGoalAward[MAX_DAILY_GOAL_AWARD][DAILY_GOAL_AWARD_DATA];

	// 符文附加额外属性定义表
	char sRuneAddAttrDef[MAX_RUNE_ADDATTR_DEF][RUNE_ADDATTR_DEF_DATA];

	char sArmStarDef[MAX_ARM_STAR_DEF][ARM_STAR_DEF_DATA];

	// 幻境
	char sUnrealMapDef[MAX_UNREAL_MAP_DEF][UNREAL_MAP_DEF_SIZE];
	char sUnrealDef[MAX_UNREAL_DEF][UNREAL_DEF_SIZE];

	// 宠物洗资质相关定义表
	char sPetWashGeniusDef[MAX_PET_WASH_GENIUS_DEF][PET_WASH_GENIUS_DEF_DATA];

	// 关键字库
	char sNewAddAttrDef[MAX_NEW_ADD_ATTR_DEF][NEW_ADD_ATTR_DEF_DATA];

	// 关键字随机数量定义表
	char sAddAttrRandDef[MAX_ADD_ATTR_RAND_DEF][ADD_ATTR_RAND_DEF_DATA];


	// 成长之路类别
	char sAchievementCategoryDef[MAX_ACHIEVEMENT_CATEGORY_DEF][ACHIEVEMENT_CATEGORY_DEF_DATA];

	// 成长之路
	char sAchievementDef[MAX_ACHIEVEMENT_DEF][ACHIEVEMENT_DEF_DATA];


	// 关键字洗炼定义表
	char sAddAttrWashDef[MAX_ADD_ATTR_WASH_DEF][ADD_ATTR_WASH_DEF_DATA];

	// 晶石定义表
	char sSparDef[MAX_SPAR_DEF][SPAR_DEF_SIZE];

	// 装备出生星级定义表
	char sItemStarRandDef[MAX_ITEM_STAR_RAND_DEF][ITEM_STAR_RAND_DEF_DATA];

	// 赏金任务定义表
	char sSJTaskDef[MAX_SJ_TASK_DEF][SJ_TASK_DEF_DATA];

	// 刺探活动PK奖励掉落表
	char sClanSpyAwardDropDef[MAX_CLAN_SPY_AWARD_DROP_DEF][CLAN_SPY_AWARD_DROP_DEF_DATA];

	// 刺探活动传送位置表
	char sClanSpyTransDef[MAX_CLAN_SPY_TRANS_DEF][CLAN_SPY_TRANS_DEF_DATA];

	// 商运镖车定义表
	char sDartCarDef[MAX_DART_CAR_DEF][DART_CAR_DEF_DATA];


	// 副本波次怪定义表
	char sPworldWaveMonDef[MAX_PWORLD_WAVE_MON_DEF][PWORLD_WAVE_MON_DEF_DATA];

	// 副本波次怪位置信息表
	char sPworldWaveMonPosDef[MAX_PWORLD_WAVE_MON_POS_DEF][PWORLD_WAVE_MON_POS_DEF_DATA];

	// 副本波次怪公会奖励表
	char sPworldWaveMonClanAwardDef[MAX_PWORLD_WAVE_MON_CLAN_AWARD_DEF][PWORLD_WAVE_MON_CLAN_AWARD_DEF_DATA];
	
	// 题库定义
	char sSubJectDef[MAX_SUBJECT_DEF][SUBJECT_DEF_DATA];

	// 题目定义
	char sSubJectTypeDef[MAX_SUBJECT_TYPE_DEF][SUBJECT_TYPE_DEF_DATA];

	//基友奖励定义表
	char sGayDef[MAX_GAY_DEF][GAY_DEF_DATA];

	// 排行榜称号定义
	char sRangDsnDef[MAX_RANGDSN_DEF][RANGDSN_DEF_DATA];

	// 枢纽副本购买buff对应表
	char sPworldMachineBuffShopDef[MAX_PWORLD_MACHINE_BUFF_SHOP_DEF][PWORLD_MACHINE_BUFF_SHOP_DEF_DATA];

	// 玩家奖励表
	char sPlayerAwardDef[MAX_PLAYER_AWARD_DEF][PLAYER_AWARD_DEF_DATA];

	// 角色
	char sCowFinalAwardDef[MAX_COW_FINAL_AWARD_DEF][COW_FINAL_AWARD_DEF_DATA];

	char sGmTrade[MAX_GM_TRADE][GM_TRADE_DATA];

	// 据点定义表
	char sStrongPointDef[MAX_STRONG_POINT_DEF][STRONG_POINT_DEF_DATA];

	// 公会技能表
	char sClanSkillDef[MAX_CLAN_SKILL_DEF][CLAN_SKILL_DEF_DATA];

	// 据点旗子定义
	char sBannerDef[MAX_BANNER_DEF][BANNER_DEF_DATA];

	// 复活定义表
	char sReviveDef[MAX_REVIVE_DEF][REVIVE_DEF_DATA];

	// 公会工资表
	char sClanPayDef[MAX_CLAN_PAY_DEF][CLAN_PAY_DEF_DATA];

	// 等级经验补偿定义表
	char sLevelExpDef[MAX_LEVEL_EXP_DEF][LEVEL_EXP_DEF_DATA];

	// 等级差每日BUFF经验补偿定义表
	char sLevelBuffExpDef[MAX_LEVEL_BUFF_EXP_DEF][LEVEL_BUFF_EXP_DEF_DATA];
	
	// 坐骑品质定义表
	char sRidePinZhiDef[MAX_RIDE_PINZHI_DEF][RIDE_PINZHI_DEF_DATA];

	// 宠物爱心等级定义表
	char sPetLoveLevelDef[MAX_PET_LOVE_LEVEL_DEF][PET_LOVE_LEVEL_DEF_DATA];

	// 占领地图奖励公会资源定义表
	char sMapAwardOreDef[MAX_MAP_AWARD_ORE_DEF][MAP_AWARD_ORE_DEF_DATA];

	// 熔炉定义
	char sRongluDef[MAX_RONGLU_DEF][RONGLU_DEF_DATA];

	// 职业天赋表
	char sGeniusCareerDef[MAX_GENIUS_CAREER_DEF][GENIUS_CAREER_DEF_DATA];

	// 天赋属性表
	char sGeniusAttrDef[MAX_GENIUS_ATTR_DEF][GENIUS_ATTR_DEF_DATA];

	// 天赋属性表
	char sBaoXiangDef[MAX_BAOXIANG_DEF][BAOXIANG_DEF_DATA];

	// 时间城战争公会排名奖励
	char sWorldCityFightAwardDef[MAX_WORLD_CITY_FIGHT_AWARD_DEF][WORLD_CITY_FIGHT_AWARD_DEF_DATA];

	// 世界boss定义表
	char sWorldBossDef[MAX_WORLD_BOSS_DEF][WORLD_BOSS_DEF_DATA];

	//等级封印定义表
	char sLevelFengDef[MAX_LEVEL_FENG_DEF][LEVEL_FENG_DEF_DATA];

	//装备升级
	char sArmLevelUPDef[MAX_ARM_LEVELUP_DEF][ARM_LEVELUP_DEF_DATA];

	// 天降宏福地图定义表
	char sLuckyMapDef[MAX_LUCKY_MAP_DEF][LUCKY_MAP_DEF_DATA];

	// 我很忙定义
	char sBusyDef[MAX_BUSY_DEF][BUSY_DEF_DATA];

	// 高级洗练消耗金币定义
	char sHighWashUseDef[MAX_HIGHWASH_USE_DEF][HIGHWASH_USE_DEF_DATA];

	char sBusyUseDef[MAX_BUSY_USE_DEF][BUSY_USE_DEF_DATA];

	char sItemIDDef[MAX_ITEM_ID_DEF][ITEM_ID_DEF_DATA];

	char sCareerHurtDef[MAX_CAREER_HURT_DEF][CAREER_HURT_DEF_DATA];

	// 天赋附加属性表
	char sGeniusAddDef[MAX_GENIUS_ADD_DEF][GENIUS_ADD_DEF_DATA];

	char sPetGeniusDef[MAX_PET_GENIUS_DEF][PET_GENIUS_DEF_DATA];

	char sWeDef[MAX_WE_DEF][WE_DEF_DATA];

	char sExchgItemDef[MAX_EXCHG_ITEM_DEF][EXCHG_ITEM_DEF_DATA];

	char sSpartaAwardDef[MAX_SPARTA_AWARD_DEF][SPARTA_AWARD_DEF_DATA];

	char sSpartaLevelDef[MAX_SPARTA_LEVEL_DEF][SPARTA_LEVEL_DEF_DATA];

	char sMapRandTransmitDef[MAX_MAP_RAND_TRANS_DEF][MAP_RAND_TRANS_DATA];

	char sSchoolLevelDef[MAX_SCHOOL_LEVEL_DEF][SCHOOL_LEVEL_DEF_DATA];

	char sSchoolPworldDef[MAX_SCHOOL_PWORLD_DEF][SCHOOL_PWORLD_DEF_DATA];

	char sSchoolXinDeDef[MAX_SCHOOL_XINDE_DEF][SCHOOL_XINDE_DEF_DATA];

	char sBackLevelDef[MAX_BACK_LEVEL_DEF][BACK_LEVEL_DEF_DATA];

	char sXuKongPosDef[MAX_XUKONG_POS_DEF][XUKONG_POS_DEF_DATA];

	char sNewRoleGiftDef[MAX_NEW_ROLE_GIFT_DEF][NEW_ROLE_GIFT_DEF_SIZE];

	char sContCoverGiftDef[MAX_CONT_COVER_DEF][CONT_COVER_DEF_SIZE];

	char sCardBoxDef[MAX_CARD_BOX_DEF][CARD_BOX_DEF_DATA];

	char sCardBoxGroupDef[MAX_CARD_BOX_GROUP_DEF][CARD_BOX_GROUP_DEF_DATA];

	char sArmBuChangDef[MAX_ARM_BUCHANG_DEF][ARM_BUCHANG_DEF_DATA];

	char sBuChangDef[MAX_BUCHANG_DEF][BUCHANG_DEF_DATA];

	char sPworldBoxDef[MAX_PWORLD_BOX_DEF][PWORLD_BOX_DEF_DATA];

	char sYangHaiBangDef[MAX_YANGHAIBANG_DEF][YANGHAIBANG_DEF_DATA];

	//航海奖励定义表
	char sHangHaiAwardDef[MAX_HANG_HAI_AWARD_DEF][HANG_HAI_AWARD_DEF_DATA];

	char sStoreGridDef[MAX_STORE_GRID_DEF][STORE_GRID_DEF_DATA];

	// 装备升阶定义
	char sArmStepUpDef[MAX_ARM_STEP_UP_DEF][ARM_STEP_UP_DEF_DATA];

	// 装备档次值定义
	char sArmGradeValDef[MAX_ARM_VAL_DEF][ARM_VAL_DEF_DATA];
	// 装备基础属性重置表
	char sArmReSetDef[MAX_ARM_RESET_DEF][ARM_RESET_DEF_DATA];

	//融焰巨兽定义表
	char sRongYanJuShouDef[MAX_RONGYANJUSHOU_DEF][RONGYANJUSHOU_DEF_DATA];

	//融焰巨兽修正属性表
	char sRongYanMonDynAttrDef[MAX_RONGYANMONDYNATTR_DEF][RONGYANMONDYNATTR_DEF_DATA];

	// 装备技能获取表
	char sArmSkillNumDef[MAX_ARM_SKILL_NUM_DEF][ARM_SKILL_NUM_DEF_DATA];

	// 装备技能数值表
	char sArmSkillAttrDef[MAX_ARM_SKILL_ATTR_DEF][ARM_SKILL_ATTR_DEF_DATA];
	

	// 公会奖励定义表
	char sClanAwardDef[MAX_CLAN_AWARD_DEF][CLAN_AWARD_DEF_DATA];

	// 跨服公会战定义表
	char sClanWarAwardDef[MAX_CLAN_WAR_AWARD_DEF][CLAN_WAR_AWARD_DEF_DATA];

	char sGeniusMdfDef[MAX_GENIUS_MDF_DEF][GENIUS_MDF_DEF_DATA];

	//跑商定义表
	char sPaoShangDef[MAX_PAOSHANG_DEF][PAOSHANG_DEF_DATA];

	//跑商道具定义表
	char sPaoShangItemDef[MAX_PAOSHANG_ITEM_DEF][PAOSHANG_ITEM_DEF_DATA];

	// 
	char sPetMoneyDef[MAX_PET_MONEY_DEF][PET_MONEY_DEF_DATA];


	char sPetImpUpAttrDef[MAX_PET_IMP_UP_ATTR_DEF][PET_IMP_UP_ATTR_DEF_DATA];

	char sPetImpUpDef[MAX_PET_IMP_UP_DEF][PET_IMP_UP_DEF_DATA];

	char sPetOpAwardDef[MAX_PET_OP_AWARD_DEF][PET_OP_AWARD_DEF_DATA];

	//宝石包定义表
	char sGemPagDef[MAX_GEM_PAG_DEF][GEM_PAG_DEF_DATA];

	// 装备特殊属性定义
	char sArmTeshuDef[MAX_ARM_TESHU_DEF][ARM_TESHU_DEF_DATA];

	//终极熔岩巨兽定义
	char sZJRongYanJuShouDef[MAX_ZJ_RONGYANJUSHOU_DEF][ZJ_RONGYANJUSHOU_DEF_DATA];

	// 战场据点Def
	char sSpanStrongDef[MAX_SPAN_STRONG_DEF][SPAN_STRONG_DEF_SIZE];

	// 文字密码Def
	char sWordPwdDef[MAX_WORD_PWD_DEF][WORD_PWD_DEF_DATA];

	// 家园相关
	char sHomeLvlDef[MAX_HOME_LVL_DEF][HOME_LVL_DEF_DATA];
	char sHomeMonLvlDef[MAX_HOME_MON_LVL_DEF][HOME_MON_LVL_DEF_DATA];
	char sHomeMonSegDef[MAX_HOME_MON_SEG_DEF][HOME_MON_SEG_DEF_DATA];
	char sHomeFarmDef[MAX_HOME_FARM_DEF][HOME_FARM_DEF_DATA];
	char sHomeSoilDef[MAX_HOME_SOIL_DEF][HOME_SOIL_DEF_DATA];

	// 装备技能改造
	char sArmSkillRecastDef[MAX_SKILL_ARM_RECAST_DEF][SKILL_ARM_RECAST_DEF_DATA];

	/*------以下需要特殊初始化-----------*/

	//商品获取状态
	int iShopGetStat;  //0 初始 1 正在获取数据

	//考虑商品很多用han来存储下,加快查找速度
	ShopMemory stShopMemory; 

	// 商品的限制定义
	//考虑商品很多用han来存储下,加快查找速度
	ShopLimitMemory stShopLimitDefMemory; 

	int iVipBarGetStat;  //0 初始 1 正在获取数据 2获取成功
	VipBarMemory stVipBarMemory; 
	int iVipBarLevelGetStat;  //0 初始 1 正在获取数据 2获取成功
	VipBarLevelMemory stVipBarLevelMemory; 
	int iVipBarNumGetStat;	//0 初始 1 正在获取数据 2获取成功
	VipBarNumMemory stVipBarNumMemory; 

	VipBarOnlineMemory stVipBarOnlineMemory; 
	DynActiveStatusMemory stDynActiveStatusMemory; 
	CondIdMapMemory stCondIdMapMemory; 


	//出售表的相关数据
	LPSHTABLE pstTradeSellHash;
	TradeSellMemory stTradeSellMemory;
		
	//求购表的相关数据
	LPSHTABLE pstTradeBuyHash;
	TradeBuyMemory stTradeBuyMemory;
	
	//验证码图片
	char sGdCache[MAX_GD_CACHE][GD_DATA];

	char sItem[MEMPOOL_SIZE(MAX_ITEM, DROP_ITEM_DATA)];
	char sMonster[MEMPOOL_SIZE(MAX_MONSTER, MONSTER_DATA)];
	char sNpc[MEMPOOL_SIZE(MAX_NPC, NPC_DATA)];

	char sNameSht[SHT_SIZE(PLAYER_BUCKET, MAX_PLAYER, sizeof(NameMemID))];
	char sL2PSht[SHT_SIZE(PLAYER_BUCKET, MAX_PLAYER, sizeof(LoginName2MemID))];

	// Team的WID索引
	char sWID2TeamSht[SHT_SIZE(TEAM_BUCKET, MAX_TEAM, sizeof(WID2Team))];

	char sScriptSysVar[SCRIPT_SYSVAR_SIZE];
	int iSIdxNum;
	ZoneScriptIndex astScriptIndex[SCRIPT_TOTAL_NUM];
	char sScriptMini[MEMPOOL_SIZE(SCRIPT_MINI_NUM, SCRIPT_MINI_SIZE)];
	char sScriptMiddle[MEMPOOL_SIZE(SCRIPT_MIDDLE_NUM, SCRIPT_MIDDLE_SIZE)];
	char sScriptLarge[MEMPOOL_SIZE(SCRIPT_LARGE_NUM, SCRIPT_LARGE_SIZE)];
	char sScriptHuge[MEMPOOL_SIZE(SCRIPT_HUGE_NUM, SCRIPT_HUGE_SIZE)];

	char sTeam[MEMPOOL_SIZE(MAX_TEAM, TEAM_DATA)];
	char sMail[MEMPOOL_SIZE(MAIL_MAX_TOTAL_NUM, ZONE_MAIL_DATA)];

	char sGlobal[GLOBAL_DATA];

	char sClan[CLAN_OBJ_DATA];
	char sClanSht[SHT_SIZE(CLAN_BUCKET, CLAN_MAX_NUM_HARD, sizeof(ClanHash))];
	
	char sTrustSht[SHT_SIZE(TRUST_BUCKET, MAX_PLAYER, sizeof(ROLETRUSTDATA))];

	char sPostoffice[POSTOFFICE_OBJ_DATA];
	char sMailboxSht[SHT_SIZE(MAILBOX_BUCKET, MAIL_MAX_MAILBOX, sizeof(MailboxHash))];
	char sEventSht[SHT_SIZE(EVENT_BUCKET, MAX_GAME_EVENT, sizeof(GameEventInst))];

	// 公告板
	char sBulletinBoard[BULLETIN_BOARD_OBJ_DATA];

	//装备关键字快速索引
	char sAddAttrFastList[ADD_ATTR_FAST_LIST_DATA];

	char sSJTaskFastList[SJ_TASK_LIST_DATA];

	char sSpartaData[SPARTA_QUEUE_DATA];

	//排行榜
	time_t tLastRange;
	RangeMem stRange;

	//自动组队
	AutoTeam stAutoTeam;

	SUBJECT stSubJect;
	WORLDBOSS stBoss;
	SVRWEATHER stWeather;

	SVRREGINFO stRegInfo;

	char sWatchSht[SHT_SIZE(WATCH_BUCKET, MAX_WATCH_ROLE, sizeof(WATCHONE))];

	char sStore[STORE_OBJ_DATA];
	char sStoreSht[SHT_SIZE( STORE_BUCKET, MAX_STORE_NUM, sizeof(StoreHash))];

	ArmSkillAttrFast stArmSkillFast;
	
	PAOSHANGINFO stPaoShangInfo;	   //记录刷新跑商的道具价格，刷新时间等
	SpartaInfo stSpartaInfo;
	char sRegionGlobal[REGION_GLOBAL_DATA];
	char sHome[HOME_OBJ_DATA];
	int NewPeoRefTime;  //  新人提示时间

	char sShiKongData[SHIKONG_QUEUE_DATA]; // 时空深渊
}ZoneObj;

typedef struct tagMapLine
{
        int iMapID[MAX_MAP_LINE];
}MapLine;

typedef struct tagMapIndex
{
	MAPDEF stMapDef;
	short iXAreaNum;
	short iYAreaNum;
	int iAreaOrigin;
	int iMaxMaskSize;
	int iMaskOrigin;
	RESPOS stEnd;
	int iMaxProcSize;
	int iNpcStart;
	int iNpcCount;
	int iMonsterStart;
	int iMonsterCount;
	time_t tMask;
	MapLine stMapLine;
	int iOrePositionStart;
	int iOrePositionCount;
	int iTmpItemStart;
	int iTmpItemCount;
	int iMapTransStart;
	int iMapTransCount;
}MapIndex;

struct tagZonePworld
{
	PWORLDDEF stPworldDef;
	int iArea;
	int iMap;
	ZONEIDX astMapIdx[MAX_PWORLD_MAP];
	int iPworldInstHead;			//活动副本同时开启多个，链表头
	int iPworldInstNum;	
};
typedef struct tagZonePworld	ZONEPWORLD;

#define MAX_PWORLD_DEF 256
#define PWORLD_DATA 1024
char sWatchPworldData[PWORLD_DATA - sizeof(ZONEPWORLD)];

#define MAP_INDEX_DATA 700 
char sWatchMapIndexData[MAP_INDEX_DATA - sizeof(MapIndex)];

#define MAX_PWORLD_INSTID	32768

#define MAX_DYN_MSK 16000
#define DYN_MSK_DATA 4096

typedef struct tagMapObj
{
	int iVersion;

	int iLastID;	/* the last id used for pworldinst. */
	
	//平行世界定义
	time_t tPworldDef;
	int iPworld;
	char sPworld[MAX_PWORLD_DEF][PWORLD_DATA];
	
	//地图定义
	time_t tMapDef;
	int iMapIndex;
	char sMapIdx[MAX_MAP_INDEX][MAP_INDEX_DATA];

	//地图怪物定义
	time_t tMapMonsterDef;
	int iMapMonsterDef;
	char sMapMonsterDef[MAX_MAPMONSTER_DEF][MAPMONSTER_DEF_DATA];

	//地图npc定义
	time_t tMapNpcDef;
	int iMapNpcDef;
	char sMapNpcDef[MAX_MAPNPC_DEF][MAPNPC_DEF_DATA];

	//事件区域定义, 有点浪费, 应该可以不需要
	time_t tProcDef;
	int iProcDef;
	char sProcDef[MAX_PROC_DEF][PROC_DEF_DATA];

	//矿石分布坐标定义表
	time_t tMapOrePositionDef;
	int iMapOrePositionDef;
	char sMapOrePositionDef[MAX_MAP_ORE_POSITION_DEF][MAP_ORE_POSITION_DEF_DATA];

	//地图矿石定义表
	time_t tMapOreDef;
	int iMapOreDef;
	char sMapOreDef[MAX_MAP_ORE_DEF][MAP_ORE_DEF_DATA];

	//地图临时道具表
	time_t tMapTmpItemDef;
	int iMapTmpItemDef;
	char sMapTmpItemDef[MAX_MAP_TMP_ITEM_DEF][MAP_TMP_ITEM_DEF_DATA];

	
	//地图区域
	int iArea;
	char sArea[MAX_AREA][AREA_DATA];

	//地图msk
	int iMask;
	char sMask[MAX_MASK_DATA];

	//地图动态区域
	char sAreaDyn[MEMPOOL_SIZE(MAX_AREA_DYN,AREA_DYN_DATA)];

	//动态阻挡
	char sDynMsk[MEMPOOL_SIZE(MAX_DYN_MSK,DYN_MSK_DATA)];

	int iMapInst;
	ZONEIDX astMapInstIdx[MAX_MAP_INST];
	char sMapInst[MEMPOOL_SIZE(MAX_MAP_INST, MAP_INST_DATA)];

	char sPworldInst[MEMPOOL_SIZE(MAX_PWORLD_INST, PWORLD_INST_DATA)];
}MapObj;

typedef int (* SearchFunc)(MapIndex *pstMapIndex, TMEMPOOL *pstAreaDynPool, ZONEMAPINST* pstMapInst, RESPOS *pstPos, SearchArea *pstSArea);

//宝石分类
typedef struct
{
	unsigned int BindItemID;
	unsigned int NormalItemID;                  
}GemAssort;

typedef struct
{
	tdr_ulonglong GID;
	int iIdx;
}RangeHashName;

#define MAX_GM_IP 256

typedef struct
{
	int iInitFlag; //0 not ready; 1 ready
	int iGMIP;
	GMIPMACLIMIT astGmIPMacLimit[MAX_GM_IP];
}GMLIMIT;

typedef enum tagGAME_FLAG
{
	GAME_FLAG_LUCK = 1,//宏福活动进行中
	GAME_FLAG_IGNORE_REFER_LINE_MAP = 2,//chuange map 时无视自身的多线地图,一次有效,传送后要重置
}SVR_FLAG;


#define PET_RND_SKILL_MAX 100
typedef struct
{
	int iID;
	int iLevel;
	int iGroupID;
	int iRate;
}PetRndSkill;

typedef struct tagunrealpworld
{
	int iNum;
	int iPInstIDHead;	
}UnrealPworld;

typedef struct tagUnrealInfo
{
	UNREALBOSSINFOS stBossInfo;
	UnrealPworld stPworld[UNREAL_MAX_LAYER];
}UnrealInfo;

#define SUBJECT_STAT_REQ 1
#define SUBJECT_STAT_START 2
#define SUBJECT_STAT_END 3





typedef struct tagSubJectType
{
	char cType;
	short cNum;
	int iID[MAX_SUBJECT_TYPE_COUNT];
	
}SUBJECTTYPE;

typedef struct tagSubJectList
{
	int iNum;
	SUBJECTTYPE stType[MAX_SUBJECT_TYPE_NUM];
	
}SUBJECTLIST;

#define ENV_DYN_MASK_WARNING 1
#define ENV_NEW_PWORLD_WARNING 2

enum DB_CO_DB_TYPE
{
	DB_CO_DB_TYPE_NONE = 0,
	DB_CO_DB_TYPE_MISC = 1,
	DB_CO_DB_TYPE_ROLE = 2,
	DB_CO_DB_TYPE_ACCOUNT = 3,
	DB_CO_DB_TYPE_REGION =4,
};

struct tagDBMetaInfo
{
	char szMetaName[MAX_DB_META_NAME_LEN];
	int iObjSize;
	char* pszObj;
	char szWhere[MAX_DB_SQL_LEN];
	int iUin;
	int iDoInsert;
	int iDBType; /*DB_CO_DB_TYPE*/
};

typedef struct tagDBMetaInfo DBMetaInfo;


struct tagZoneSvrEnv
{
	TAPPCTX *pstAppCtx;
	ZONE_SVRCONF *pstConf;
	ZONE_SVRRUN_STATUS *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;
	NETHANDLE *pstNetHan;
	TORMPROTOCOLHANDLE hTorm;
	int iRoleDBProtoVer;
	int iMiscDBProtoVer;
	int iAccountDBProtoVer;
	int iRegionDBProtoVer;
	int iCsSvrID;
	int iZoneConnID;
	int iZoneCtrlID;
	int iRoleDBID;
	int iMiscDBID;
	int iAccountDBID;
	int iRegionGateID;
	int iRegionDBID;
	int iMudSvrID;
	int iBattleSpartaID;
	int iBattlePveBattleID;
	int iBattlePvpID;	
	int iInstMode;
	unsigned long int iTconnapiHandle;
	int iLastSrc;
	TMEMPOOL* pstPlayerPool;
	PlayerShm *pstPlayerShm;
	int iPlayerShmID;
	LPSHTABLE pstShtUinCache; /*uin login frequency limit, uin role list*/
	LPSHTABLE pstShtNameMemID; 
	LPSHTABLE pstShtL2P; // Login Name -> Player MemID 的映射表
	LPSHTABLE pstShtClan; 
	LPSHTABLE pstShtTrust;
	LPSHTABLE pstShtMailbox; // 邮箱哈希表
	LPSHTABLE pstShtEvent;
	LPSHTABLE pstShopHash;//商品的hash定义
	LPSHTABLE pstShopLimitDefHash; //商品的限制的hash定义
	LPSHTABLE pstShtRangeName;
	LPSHTABLE pstShtWID2Team; // WID -> Team idx 的映射表
	
	LPSHTABLE pstVipBarHash;
	LPSHTABLE pstVipBarLevelHash;
	LPSHTABLE pstVipBarNumHash;
	LPSHTABLE pstVipBarOnlineHash;
	LPSHTABLE pstDynActiveStatusHash;
	LPSHTABLE pstCondIdMapHash;
	LPSHTABLE pstWatchHash;

	LPSHTABLE pstShtStore;	// 店铺哈希 
	
	int iUinCacheShmID;
	int iUinCacheWashoutPos;
	ZoneObj *pstZoneObj;
	ClanObj *pstClanObj;
	GlobalObj *pstGlobalObj;
	StoreObj *pstStoreObj;
	Postoffice * pstPostoffice; // 邮局
	HomeObj *pstHomeObj;
	int iZoneObjShmID;
	MapObj *pstMapObj;
	int iMapObjShmID;
	TMEMPOOL* pstAreaDynPool;
	TMEMPOOL* pstDynMskPool;
	TMEMPOOL* pstMapInstPool;
	TMEMPOOL* pstPworldInstPool;
	TMEMPOOL* pstMonsterPool;
	TMEMPOOL* pstNpcPool;
	TMEMPOOL* pstItemPool;
	TMEMPOOL* pstScriptMiniPool;
	TMEMPOOL* pstScriptMiddlePool;
	TMEMPOOL* pstScriptLargePool;
	TMEMPOOL* pstScriptHugePool;
	TMEMPOOL* pstTeamPool;
	TMEMPOOL* pstMailPool; // 邮件内存池
	LPTDRMETALIB pstResLib;
	TDRDBHANDLE hResDBHandle;
	CmdMap astCmdMap[MAX_CS_CMD];
	char szRecvBuffer[MAX_MSG]; /*保存接收消息缓冲区*/
	char szSendBuffer[MAX_MSG]; /*保存发送消息缓冲区*/
	TFRAMEHEAD stFrameHeadTemp;
	unsigned short  unGidSeq;  //生成全局唯一id时的自增量
	unsigned short   unWorldID; //根据id命名规则得到的worldid，参加entity.h
	SSHANDLE  *pstSSHan;
	int iDirSvrSock;
	int iDirBakSock;
	int iConnIP;
	unsigned short unPort;
	char cShutdown;
	unsigned char ucShutdownCount;
	time_t tShutdownStart;
	time_t tDirReport;
	CSPKG stCSReq;
	CSPKG stCSRes;
	SSPKG stSSReq;
	SSPKG stSSRes;
	TORMPKG stOrmPkg;
	BlackWhiteHandle *pstBlackWhiteH;
	int iUpdateCount;
	int iUpdatePos;
	int iBenchCount;
	int iBenchPos;
	int iOpRoleNumTm;
	SearchFunc astSearchFuns[64];
	GemAssort stGemAssort[MAX_GEM_LEVEL][MAX_GEM_INLAY];
	BulletinBoard *pstBulletinBoard; // 公告板
	WorldEvent *pstWorldEvent;	//影响世界的事件信息
	WorldPworldQueue *pstPworldQueue;
	AddAttrFastList *pstAddAttrFastList;
	SJTaskFastList *pstSJTaskFastList;

	//unsigned char cBattleNum;
	//BATTLEINFO stBattleINfo[BATTLE_PWORLD_NUM];
	

	//运营日志
	TLOGCATEGORYINST* pstOpLogCat;	
	TLOGCONF stOpLogConf;	
	TLOGCTX stOpLogCtx;	
	LPTDRMETA pstLogicLogMeta;

	struct timeval stStart;
		
	time_t tCheckDB;
	int iTormSeq;
	int iLevelStandStart;

	int iMaxPworldExpMul;	//活动副本补偿系数之和。计算补偿时用到

	tdr_ulonglong ullEventLogWID;

	GMLIMIT stGmLimt;
	
	char cClearShm;
	char cReloadMap;
	char cReladPworld;
	unsigned char ucRegionID;
	char cLimitPerm;

	unsigned char ucPlayerScriptFlag;//玩家脚本事件标志
	char cBeachClosed;//海滩是否关闭
	int iWorldCityBuildSerial;//时间城争霸战地图建筑占领变化序列号  
	int iMiscFlag;//杂项标记,对应GMAE_FLAG
	int iLuckyTipCount;//宏福活动提示计数
	int iLastWorldTick;
	int iPetRndSkill;
	PetRndSkill astPetRndSkill[PET_RND_SKILL_MAX];
	UnrealInfo stUnrealInfo;
	int iRealItemDecNum;//实际减少物品数量

	//SUBJECT stSubJect;
	SUBJECTLIST stSubList;
	int iClanCityInitStatusID;
	int iClanCityInitStatusLevel;
	int iClanCityInitStatusTime;
	int iWarningFlag;
	int iLuckyRandSum;
	int iSmsSN;//短信序号

	SpartaQueue *pstSpartaQueue;
	COROUTINERPC stCoroutineRPC;
	DBCOROUTINERPC stDBCoroutineRPC;
	DBMetaInfo stDBMetaInfo;

	int iShuNiuClanID;
	int iShopBuyTipTime;

	// 跨服据点
	RegionGlobalInfo *pstRegionGlobalInfo; 
	char iSpanStrongMaxLvl; 
	char iSpanStrongExpMul; 
	int iSpanMaxBull;
	int iSpanMaxBindGold;
	// 跨服据点

	ShiKongQueue *pstShiKongQueue; // 时空深渊
};

typedef struct tagUinCache 
{
	int  iUin;	 /*uin of player*/
	int  iIdx;	/*index of player in memory pool*/
	int  iOnline;
	time_t tLastLogin;
	char sRes[2];
	char cGetRoleList;
	unsigned char ucRoleNum;
	ROLELISTINFO astRoleListInfo[ROLE_MAX_NUM];
}UinCache;

typedef struct tagAttrChgVal
{
    short AttrID; 
	short nFlag; // 0 +, 1 -
    CSATTRIDVAL Value;                          
}AttrChgVal;

typedef struct tagPlayerChagAttrVal
{
    unsigned char AttrNum;                        
    AttrChgVal AttrList[MAX_ATTR_VAL];         
}PlayerChagAttrVal;

typedef struct
{
	BUDDYINFO stBuddyInfo;
	char szName[NAME_LEN];
}DBBuddyChg;

#define TV_ADD_MS(tv, tvnow, ms)		do									\
										{	               					\
											(tv).tv_sec = (tvnow).tv_sec + (ms)/1000; 		\
											(tv).tv_usec = (tvnow).tv_usec + ((ms) % 1000)*1000;\
											if( (tv).tv_usec>=1000000 )\
											{\
												(tv).tv_usec -=	1000000;\
												(tv).tv_sec++;\
											}\
											else if ((tv).tv_usec < 0)\
											{\
												(tv).tv_usec +=	1000000;\
												(tv).tv_sec--;\
											}\
										} while(0)

// 让编译器高兴
#define UNUSED( param ) ((void)(param))

// 获取一个数组的成员数量
#define CCH( arr ) (sizeof(arr)/sizeof(arr[0]))

#define DEFAULT_GOLD_RATE 1000

int UinHashCmp( const void * pv1 , const void *pv2 );
unsigned int UinHashCode(const void* pvData);
int z_washout_uincache(ZONESVRENV *pstEnv);
int z_free_player(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
int z_free_monster(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon);
int z_init_mapobj(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv);
int z_id_index(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iID, char cType);
int z_id_indextype(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iID, char *cType);
Player* z_id_player(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iID);
Npc* z_id_npc(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iID);
Monster* z_id_monster(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iID);
Monster* z_id_monster2(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iID);
DropItem* z_id_dropitem(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iID);
int z_id_clean(ZONESVRENV* pstEnv, int iID);
int z_get_mem_id(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iIndex, char cType);
Player * z_name_player(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, const char *pszName);
int NameMemIDHashCmp( const void * pv1 , const void *pv2 );
unsigned int NameMemIDHashCode(const void* pvData);
int z_load_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv);
int z_load_map(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iForce);
int init_item_scr(ZONESVRENV* pstEnv);
SUITSET *z_find_suitset(ZONESVRENV* pstEnv, int iSuitSetID);
ADDATTRDEF *z_find_addattr(ZONESVRENV* pstEnv, int iAddAttrID);
void *z_find_def(void* pBase, unsigned int uiKey, int iUnit, int iUnitNum, int *piIdx);
int z_reload(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv);

int z_global_res(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes);

//int z_gen_image(ZONESVRENV* pstEnv, void* buf, int* len, char *pszCode, int iCodeLen);

LEVELUP *z_get_levelup(ZONESVRENV* pstEnv, int iLevel);
int z_global_update_req( TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv );
int z_global_update_res( TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstTormPkg );
int z_msg_to_cs(ZONESVRENV* pstEnv, ASYNREQ *pstAsynReq);

void ss_init_head(SSPKGHEAD *pstHead, int iCmd);
int ss_sendto_cs_svr(ZONESVRENV *pstEnv, SSPKG *pstSSPkg);

int z_gen_bulletin_id();
int z_del_bulletin(ZONESVRENV* pstEnv, int iID);
int z_add_bulletin(ZONESVRENV* pstEnv, const SSBULLETIN *pstReq);
int z_ss_force_msg_req(ZONESVRENV* pstEnv, const FORCEMSGREQ*pstCtlReq);

int EventHashCmp( const void * pv1 , const void *pv2 );
unsigned int EventHashCode(const void* pvData);

int z_game_event_del(ZONESVRENV* pstEnv, GameEventInst *pstEvent); 

int mon_hp_action(ZONESVRENV* pstEnv, Monster *pstMon, int iOldHP, int iApplyID);

Player* z_loginname_player(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,
                           const char *pszAccount);

int L2PHashCmp(const void *pv1, const void *pv2);
unsigned int L2PHashCode(const void *pvData);

int init_task_scr(ZONESVRENV* pstEnv);

int z_clear_all_event(ZONESVRENV* pstEnv);

int z_game_event_role_login(ZONESVRENV* pstEnv, Player *pstPlayer);

int mon_active(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TMEMBLOCK *pstMemBlock, int iActivePos, int iRound);

static inline int is_battle_svr(ZONESVRENV *pstEnv)
{
	unsigned char ucEntity = 0;
	GET_FUNC_ENTITY(ucEntity, pstEnv->pstAppCtx->iId);
	return (ucEntity == FUNC_BATTLE_SVR);
}

int z_init_player_scr(ZONESVRENV* pstEnv);
int z_sync_mud_time(ZONESVRENV* pstEnv);

int is_battle_sparta_svr(ZONESVRENV* pstEnv);
int is_battle_pve_battle_svr(ZONESVRENV* pstEnv);
int is_battle_pvp_svr(ZONESVRENV* pstEnv);

ACTIVEHELPER *z_find_active_help_def(ZONESVRENV *pstEnv, int iActiveID, int *pIidx);

int z_gm_limit_res(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes);


int mon_map_pos(ZONESVRENV* pstEnv,Monster* pstMon,int iShowType);
DAILYGOALAWARD* z_find_daily_goal_award(ZONESVRENV *pstEnv, int iID);

DYNACTIVESTATUS * z_find_dyn_active_status(ZONESVRENV *pstEnv,unsigned int uiKey);
int hash_insert_dyn_active_status(ZONESVRENV *pstEnv,unsigned int uiKey,char* pszData,unsigned int iSize);
int dyn_active_status_init(ZONESVRENV *pstEnv);
int dyn_active_status_update(ZONESVRENV *pstEnv,Player* pstPlayer);
int dyn_active_status_update_all(ZONESVRENV *pstEnv);
int dyn_active_status_ch(ZONESVRENV *pstEnv,int iID,int iStatus);
int z_ss_coroutine_rpc_req(TAPPCTX * pstAppCtx, ZONESVRENV* pstEnv,COROUTINERPC *pstCoroutineRpc);

ACHIEVEMENTCATEGORYDEF* z_find_achievement_category_def(ZONESVRENV *pstEnv, int iID);
ACHIEVEMENTDEF* z_find_achievement_def(ZONESVRENV *pstEnv, int iID);
SkillDefTail* z_get_skill_tail(SKILLDEF *pstSkillDef);
AchievementDefTail* z_get_achievement_tail(ACHIEVEMENTDEF* pstDef);
CONDIDMAP * z_find_cond_id_map(ZONESVRENV *pstEnv,unsigned int uiKey);
int hash_insert_cond_id_map(ZONESVRENV *pstEnv,unsigned int uiKey,char* pszData,unsigned int iSize);
CLANSPYAWARDDROPDEF* z_find_clan_spy_award_drop_def(ZONESVRENV *pstEnv, int iID);
CLANSPYTRANSDEF* z_find_clan_spy_trans_def(ZONESVRENV *pstEnv, int iID);
PWORLDWAVEMONDEF* z_find_pworld_wave_mon_def(ZONESVRENV *pstEnv, int iID);
PWORLDWAVEMONPOSDEF* z_find_pworld_wave_mon_pos_def(ZONESVRENV *pstEnv, int iID);
PWORLDWAVEMONCLANAWARDDEF* z_find_pworld_wave_mon_clan_award_def(ZONESVRENV *pstEnv, int iID);
GameEventInst * z_get_game_event(ZONESVRENV* pstEnv,int iEventID);
PWORLDMACHINEBUFFSHOPDEF* z_find_pworld_machine_buff_shop_def(ZONESVRENV *pstEnv, int iID,int iLevel);
PLAYERAWARDDEF* z_find_player_award_def(ZONESVRENV *pstEnv, int iID);
COWFINALAWARDDEF* z_find_cow_final_award_def(ZONESVRENV *pstEnv, int iID);
PworldWaveMonDefTail* z_get_pworld_wave_mon_tail(PWORLDWAVEMONDEF* pstDef);
CLANSKILLDEF* z_find_clan_skill_def(ZONESVRENV *pstEnv, int iID,int iLevel);
PlayerAwardDefTail* z_get_player_award_tail(PLAYERAWARDDEF* pstDef);
int z_ss_cmd_req(ZONESVRENV* pstEnv, SSPKG *pstPkg);
CLANPAYDEF* z_find_clan_pay_def(ZONESVRENV *pstEnv, int iID);
int autoteam_check_active_running(ZONESVRENV* pstEnv, ACTIVEHELPER *pstActiveHelper);

PETLOVELEVELDEF* z_find_pet_love_level_def(ZONESVRENV *pstEnv, int iID);
int z_game_notify(ZONESVRENV* pstEnv, char *pstStr, int iLen);
LUCKYMAPDEF* z_find_lucky_map_def(ZONESVRENV *pstEnv, int iID);
int z_reload_self();
GAMEEVENTDEF *z_find_event_def(ZONESVRENV *pstEnv, int iEventID, int *pIidx);

int z_event_check_active_runing(ZONESVRENV* pstEnv, GAMEEVENTDEF *pstEventDef);

HANGHAIAWARDDEF* z_find_hang_hai_award_def(ZONESVRENV *pstEnv, int iID);
CLANAWARDDEF* z_find_clan_award_def(ZONESVRENV *pstEnv, int iID);
CLANWARAWARDDEF* z_find_clan_war_award_def(ZONESVRENV *pstEnv, int iID);

ClanAwardDefTail* z_get_clan_award_tail(CLANAWARDDEF* pstDef);

int z_wedding_tick(ZONESVRENV* pstEnv);


#endif /* ZONE_SVR_H */


