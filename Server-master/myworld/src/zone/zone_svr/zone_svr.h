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
#define MAX_SUBJECT_TYPE_NUM 10 		// ���10������
#define MAX_SUBJECT_TYPE_COUNT 500 	//ÿ���������300��
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
	PLAYER_STATUS_BATTLE_LOGIN_ON = 2,   //ǰ���״̬����zone���޸�
	
	PLAYER_STATUS_BATTLE_TEAM_SYNC = 64, //���Ǹ���ֵ,�����γ�״̬�ı���
	PLAYER_STATUS_BATTLE_GOTO_ZONESVR = 128, //���˳�����ȥ����
	
}PLAYERSTATUSASSISTANT;

typedef enum tagZONEStatusMiscType
{
	ZONESTATUS_MISCTYPE = 0,
	ZONESTATUS_MISCTYPE_CHUANRAN_FLAG =1 , //λ��־1��ʾ�Ѿ����ٴ�Ⱦ��
	ZONESTATUS_MISCTYPE_DYN = 2 , //�ж�̬����

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
	 DIRTY_NORMAL,	//��ʱ����
	 DIRTY_IMPORTANT,  //��Ҫ��������
};

#define INI_VALUE -999	//��ʼ��һ��ֵ����Ϊ��ʶ

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

#define MAX_ARM_ON  3  //ͬʱ��װ������������

#define MAX_ARM_SKILL	6	//2��ɫװ�� ���Ӽ��ܵȼ�
#define MAX_ARM_ADD_SKILL	3	//2��ɫװ�� ���Ӽ���

#define MAX_GN_TOUCH_BUFF 2 //ÿ����ɫ�����츳  ʹ�ü��ܴ�������BUFF ����

#define MAX_STATUS_CHG_SKILL_LEVEL 1//״̬�ı似�ܵȼ��ļ��ܸ��� (״̬Ч��)

struct tagMidAttrList
{
	unsigned int uiHPMax;
	unsigned int uiMPMax;
	//unsigned short unNl;			// liliang
	unsigned int uiVimMax;
	unsigned int uiSpeMoveMax;   	// ����ƶ���	

	unsigned int uiFight; 			//ս����
	int iPetFight; 				//�����ṩ��ս����

	unsigned char bXianListAdd;
	unsigned char bShiListAdd;

	unsigned int uiSitRestoreHP;	// �����ظ���HP
	unsigned int uiSitRestoreMP;	// �����ظ���MP
	unsigned int uiSitAddExp;		// �������ӵ�EXP
	short iSitConsumeVigor;		// �������ĵ�Vigor
	unsigned int uiVigorMax;		// ������
	unsigned int uiDayExpMax;		// ÿ���ȡ��������
	unsigned int uiDayZhengyiMax;		// ÿ���ȡ���������
	
	unsigned short usDayLifeBrandMax;		// ÿ�����潱������

	unsigned int uiLevelUpAddMp;	// �ȼ���Ӧ����mp
	unsigned int uiLevelUpAddHp;	// �ȼ���Ӧ����hp

	unsigned int uiXpMax; 			// 	���xpֵ

	unsigned int uiKillAddHp;		// ��ɱ��ȡhp
	unsigned int uiKillAddMp;		// ��ɱ��ȡmp

	//����
	int iLiliangAdd;
	int iLiliangMul;

	//����
	int iZhiliAdd;
	int iZhiliMul;

	//����
	int iTiliAdd;
	int iTiliMul;

	//����
	int iJingshenAdd;
	int iJingshenMul;

	//����
	int iFangyuAdd;
	int iFangyuMul;

	//����
	int iMinJieAdd;
	int iMinJieMul;

	// һ������
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
	int iArmID; //���ĸ�װ��������
	int iArmDefPos;
	int iNeedAmm; //�Ƿ���Ҫ��ҩ
	int iAmmID;  // >0 װ���˶�Ӧ�ĵ�ҩ, 0 û��װ����Ӧ�ĵ�ҩ
	int iAmmPos; // ��ҩ���е�λ��������-1 ��ҩ����û�е�ҩ��
	int iArmType;
	*/

	//ħ��
	int unIceBaseArmAtk; 
	short nIceAtkChgMul;
	int iIceAtkChgAdd;
	int iIceDecDef;
	int iIceDecDefMul;
	int iIceMinAtk;
	int iIceMaxAtk;

	//����
	int unPhsicBaseArmAtk; 
	short nPhsicAtkChgMul;
	int iPhsicAtkChgAdd;
	int iPhsicDecDef;
	int iPhsicDecDefMul;
	int iPhsicMinAtk;
	int iPhsicMaxAtk;

	//������ʩ
	int unSiegeBaseArmAtk; 
	short nSiegeAtkChgMul;
	int iSiegeAtkChgAdd;
	int iSiegeDecDef;

	//��ʥ
	int unHolyBaseArmAtk; 
	short nHolyAtkChgMul;
	int iHolyAtkChgAdd;
	int iHolyDecDef;

	//��е
	int unMachineBaseArmAtk; 
	short nMachineAtkChgMul;
	int iMachineAtkChgAdd;
	int iMachineDecDef;

	//��
	int unFireBaseArmAtk; 
	short nFireAtkChgMul;
	int  iFireAtkChgAdd;
	int iFireDecDef;

	//��
	int unThunderBaseArmAtk; 
	short nThunderAtkChgMul;
	int    iThunderAtkChgAdd;
	int iThunderDecDef;

	//��
	int unLightBaseArmAtk; 
	short nLightAtkChgMul;
	int iLightAtkChgAdd;
	int iLightDecDef;

	//��
	int unNightBaseArmAtk; 
	short nNightAtkChgMul;
	int iNightAtkChgAdd;
	int iNightDecDef;

	//ˮ
	int unWaterBaseArmAtk; 
	short nWaterAtkChgMul;
	int iWaterAtkChgAdd;
	int iWaterDecDef;

	//����ϵ������
	int iAtkLevelMul;
	
	 //��ҩ
	unsigned short unBaseAmmAtk;

	//�����ȼ�����
	short nArmLevChgMul;

	//�ƻ�������
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
	
	//�����˺�����
	ATKERHURTCHG stAtkerHurtChg;
};
typedef struct tagAtkChg		ATKCHG;

struct tagDefChg
{
	//ħ��
	int unIceDefBaseAdd;
	short nIceDefChgMul;
	int iIceDefChgAdd;

	//����
	int unPhsicDefBaseAdd;
	short nPhsicDefChgMul;
	int iPhsicDefChgAdd;

	//������ʩ
	int unSiegeDefBaseAdd;
	short nSiegeDefChgMul;
	int iSiegeDefChgAdd;

	//��ʥ
	int unHolyDefBaseAdd;
	short nHolyDefChgMul;
	int iHolyDefChgAdd;

	//��е
	int unMachineDefBaseAdd;
	short nMachineDefChgMul;
	int iMachineDefChgAdd;

	//��
	int unFireDefBaseAdd;
	short nFireDefChgMul;
	int iFireDefChgAdd;

	//��
	int unThunderDefBaseAdd;
	short nThunderDefChgMul;
	int iThunderDefChgAdd;

	//��
	int unLightDefBaseAdd;
	short nLightDefChgMul;
	int iLightDefChgAdd;

	//��
	int unNightDefBaseAdd;
	short nNightDefChgMul;
	int iNightDefChgAdd;

	//ˮ
	int unWaterDefBaseAdd;
	short nWaterDefChgMul;
	int iWaterDefChgAdd;

	//����ϵ������
	int iDefLevelMul;

	//�����������
	int iIgnoreDef;	

	//����HP�˺�
	int iReflectHP;

	//����HP�˺�
	int iDefHP;

	// ���Է���ֵ
	int iAbsDef;

	// �츳�ӳ�
	int iGeniusMul; // �ٷֱ�
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

	short nAtkeeBreakPrepareMul; //�ֿ����������������������
	short nRes;
};
typedef struct tagFleeChg		FLEECHG;

struct tagHitRateChg
{
	short nAtkerHitRateMul;
	short nAtkerHitRateAdd;

	short nAtkeeHitRateMul;
	short nAtkeeHitRateAdd;
	
	short nHeavyHitAdd; //4		��������
	int nHeavyHitVal; //4		������ֵ

	int iHeavyHit; //4			������
	int iDefHeavyHit;    //4 		�����ֿ�

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

	short nSpdMoveRestore; 	/*�ƶ����ظ��ٶ�*/

	short nSpdMoveMaxBase; 		/*����ƶ���*/
	short nSPdMoveMaxMul;
	short nSPdMoveMaxAdd;

	short nSpeedMul; //�Գ�ʼ�ٶȵ�������
	double nSpeedAdd; //�Գ�ʼ�ٶȵ�������

	
	short nSpdFlyBase; 		/*�����ٶ�*/
	short nSPdFlyMul;
	short nSPdFlyAdd;

	short nSpdRideBase; 		/*�����ٶ�*/
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

	short nBaseXp; // xp�����ٶ�  �ָ�Ϊ����(FairyDef)
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

	//�츳 ����������Ѫ
	short nAtkHPMul;
	short nAtkHPAdd;
	short nAtkHpRate;//������Ѫ�ĸ���

	//�츳 ������Ѫ�ĸ���
	short nGeniusHealMPMul;
	unsigned short unGeniusHealMPAdd;
	short nGeniusAtkMPRate;

	//�츳 �����˺����ӵĸ���
	short nGeniusHealHurtMul;
	unsigned short unGeniusHealHurtAdd;
	short nGeniusAtkHurtRate;
	
	//�츳�Լ���cd��Ӱ��,���10�� �и���
	unsigned short 		  ushCDResultNum;
	SKILLRESULT astCDResult[MAX_CHG_SKILL_CD_NUM];

	//������������õ��б� Ĭ�϶��ǵ�0��������װ�������
	unsigned short unSecondarySkillType;
	unsigned short unRecipeProduceIdx;

	//�츳 ���������ܵ���ֵ��������
	short nGeniusCollectSkillAdd;
	//�츳 ���������ܵ�������ֵ�������� 
	//short nGeniusCollectSkillDec;
	//�츳 ���������ܶ�������޵�����
	short nGeniusCollectSkillMax;

	short nAtkeeHealHPMul;				//4������������
	unsigned short unAtkeeHealHPAdd;		//4������������
	short nReduceAtkeeHealHPMul;      //��������Ч��   ������

	int iAddMspd;
	int iKillerAddHP;
	int iKillerAddMP;
	int iNoFightRestoreHPMul;
	int iNoFightRestoreMPMul;

	int  iHpMulWanFengLv;	// �����hp����
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
	ATKEEHURTCHG stAtkeeHurtChg; //�ֿ��˺�����
	RESOURCEADDCHG stResourceAddChg;
	MONCHG stMonChg;
	ARMSKILL astArmSkill[MAX_ARM_SKILL];	
	ARMSKILL astAddSkill[MAX_ARM_ADD_SKILL];	
	STATUSCHGSKILL astStatusChgSkill[MAX_STATUS_CHG_SKILL_LEVEL];

	int iGnUseSkill;
	GNUSESKILLCHG astGnUseSkill[MAX_GN_TOUCH_BUFF]; // �츳   ����ʹ�ô�������BUFF

	int iBeachExpMul;//ɳ̲��������
	int iClanSkillExpMul;//���Ἴ�ܾ�������
	int iClanSkillVigorMul;//���Ἴ�ܴ�ֶ����û���ֵ��������
	int iClanSkillVigorVal;//���Ἴ�ܴ�ֶ����û���ֵֵ

	int iRoleLvl;
};
typedef struct tagDynAttrList			DYNATTRLIST;


typedef struct {
	short AtkerHurtMul;                    	/*   �츳�Ըû��ܵ��˺����� */
	int AtkerHurtAdd;                    		/*   �츳�Ըû��ܵ��˺����� */

	unsigned char HeavyHitRateMul;     /*   �Ա����ʵ����� */
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
	int iLast;//������ʱ��
	unsigned int iJumpNum; //״̬��Ծ�Ĵ���
	unsigned int iJumpedCount;//�Ѿ������Ŀ�ʼ����
	short nDefIdx;
	unsigned char bType;
	unsigned char bSubType;
	unsigned short unBindSkillID;
	unsigned char bBindSkillLevel;
	unsigned char bCount;			
	short nRelationDefIdx;
	unsigned short unOverlay; //������
	int iVal;
	int iApplyID;
	char cApplyRelation;//0����,1��ʾ�з�. ����ͨ��ʱ��
	time_t tLastStatusCheck;
	STATUSRESULT  ResultStepDecSpd;
	STATUSRESULT ResultStepDecHp;
	short nOverlayMul; //����ָ��
	unsigned int MiscType; //�ο� ZONESTATUSMISCTYPE ���λ����
	int iFeedbackMemID;	//������memid
	int iFeedbackStatusID; //������״̬id
	int iDynPara;//��̬����
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
	unsigned int iJiqiTime;//����ʱ��
	char cPilotEnd;
	char cAtkFlag; //atk ��hit ��ԣ���ֹ�������ܶ���෢hit
	char cFittingsIdx;
	//int iPrepareTimeMul;	//���һ�α�����������ʱ�İٷֱ�
	RESPOS stAtkPos;
	int iSkillCltSeq;
	int iAtkFlagBit;//λ����
	int iAtkLightningIdx;
}AtkInfo;

typedef struct {
		short shNum;
		short ashProc[MAX_DLG_OPT];
} DialogOpt;

#define MAX_DIALOG_COMMIT_NUM 10

typedef struct { 
	int iListType;		//4��λ��Ϣ
	int iGridIdx;
} DialogCommitInfo;

typedef struct { 
	int iItemID;		
	int iNum;
} DialogCommitIDInfo;


typedef struct {
		short shProc;
		int iNum;	//����ύ����
		DialogCommitInfo astGrids[MAX_DIALOG_COMMIT_NUM];// �ύ��Ʒ��λ����Ϣ		
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
#define MISC_FLAG_STATUS_FAIRYCAMP_TRACE 0x40000LL // �������׷��
#define MISC_FLAG_OWNED_TMP_ITEM_TRACE 0x80000LL   // ��ʱ����ӵ�����׷��
#define MISC_FLAG_SEND_SHOPDEF 0x100000LL	// ����󷢸��û��̳ǵ�����
#define MISC_FLAG_AUTO_PICK_ITEM 0x200000LL	// �Զ�ʰȡ��״̬λ
#define MISC_FLAG_LIFE_TIME_TRACE 0x400000LL	// ���ʱ��
#define MISC_FLAG_KILL_PLAYER_TRACE 0x800000LL	// ��ɱ���׷��
#define MISC_FLAG_SECONDARY_SKILL_ALL  0x1000000LL
#define MISC_FLAG_BEATK_TRACE     0x2000000LL
#define MISC_FLAG_CLANID_SNAPSHOT 0x4000000LL // ȫ������ID
#define MISC_FLAG_TMPITEM_PATROL_TRACE	0x8000000LL 	// �Զ�Ѳ�ߵ���ʱ��е�������
#define MISC_FLAG_SEND_LOCK_SAFE 0x10000000LL // ��ȫ����Ϣ
#define MISC_FLAG_LOGOUT_FOR_MUD 0x20000000LL // �˳���MUD
#define MISC_FLAG_EXP_NOLEVEL_LIMIT 0x40000000LL //���Եȼ���������
#define MISC_FLAG_IN_BEACH 		0x80000000LL //�ں�̲��
#define MISC_FLAG_LUCK 			0x100000000LL //�μ����콵�긣�
#define MISC_FLAG_MAP_BROADCAST 0x200000000LL //С��ͼ�ϱ�¶��
#define MISC_FLAG_USE_ITEM 		0x400000000LL	//ʹ�õ���
#define MISC_FLAG_ONLINE_TIME 	0x800000000LL	//��ɫһ������ʱ��
#define MISC_FLAG_SKILL 	    0x1000000000LL	//��ɫʹ�ü���
#define MISC_FLAG_SPY 	      	0x2000000000LL	//��ɫ�ڴ�̽
#define MISC_FLAG_ANTI_SPY 	    0x4000000000LL	//��ɫ���ػ�
#define MISC_FLAG_SPY_FINI 	    0x8000000000LL	//��ɫ��̽�������״̬
#define MISC_FLAG_ANTI_SPY_FINI 0x10000000000LL	//��ɫ�ػ��������״̬
#define MISC_FLAG_APEXON 0x20000000000LL	//����apex
#define MISC_FLAG_EXPNODAYLIMIT 0x40000000000LL  //����¼��ÿ�վ�����

#define MON_MISC_FLAG_STATUS_TRACE 0x01
#define MON_MISC_FLAG_DYN 0x02
#define MON_MISC_FLAG_IMPVIEW 0x04
#define MON_MISC_FLAG_MAPBROAD 0x08
#define MON_MISC_FLAG_RESTORE_HP 0x10
#define MON_MISC_FLAG_MAP_POS 0x20
#define MON_MISC_FLAG_IGNORE_PWORLD_OVER 0x40	//����������,Ҳ����ǿ�Ʋ���
#define MON_MISC_FLAG_PWORLD_WAVE 0x80	//�������ι�
#define MON_MISC_FLAG_PWORLD_TIMER 0x100 //������ʱ��
#define MON_MISC_FLAG_NO_DYNBAK 0x200 //���Զ�̬�赲��Ӱ��
#define MON_MISC_FLAG_MAP_SHOW 0x400 //��ǰ��ͼ����ʾ

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
	char cPackageIdx; //�����е���������
	int iDefIdx;	
	int iRaceLevelDefIdx;
}RolePetOnline;

typedef struct {
	int iNextLevelExp; //0 δ��ʼ��, -1 �����ټӾ�����

	short nExpMul; //�������ʵ�Ӱ��ľ���ֵ������
	unsigned short unExpAdd; //�������ʵ�Ӱ��ľ���ֵ������
} PetExpOnline;

typedef struct {
	short nPetPowerRestoreMul; //power ����������
	short nPetPowerRestoreAdd; //power ����������
	
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
	int iFlag;//��־λ
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
	short sBaseAttrMul; //�������Գ�����
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
	unsigned short unOverlay; //������
	short nOverlayMul; //����ָ��
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
	unsigned char bBackNum; //��
	PetFightShiOnline astFightShi[PET_MAX_FIGHT_SHI];
	PetBackOnline astBack[MAX_PET_PACKAGE];
	tdr_ulonglong llPetWidBackup;
}PetOnline;


typedef struct
{
	tdr_ulonglong ullExChgObj; 		// ���׶Է�ID
	int iInViteTime;					// ����ʱ����߱�����ʱ��
	int iMemID;

	char bExchgStat;

	//unsigned char bIsLock;			// �Ƿ�����������
	//unsigned char bIsExChg;			// �Ƿ���ȷ�Ͻ���

	EXCHGDATA stExchgData;
}ExchgOnline;

typedef struct
{
	tdr_ulonglong ullRoleID; // ���뷢���˵�ID
	CLANID stClanID; // �������Ĺ���ID
	time_t tTime; // ���뷢��ʱ��
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
	tdr_ulonglong ullStallOtherWID;		// ���������̯λʱ�����WID

	
	tdr_ulonglong ullStallWID;					// �Լ�̯λWID
	char szStallName[STALL_MAX_NAME_LEN];	// ̯λ����
	STALLDATA stStallData;					// �Լ�̯λ��Ϣ
	int tStallActiveTime;			// ��¼���ϴ�ɨ���ʱ��
}StallOnline;

#define FOLLOW_ANGEL_DIV 6

#define MAX_WEI_XING 24						// ���β�����
typedef struct
{
	unsigned char cWeiXingNum;
	int aiWeiXingMemID[MAX_WEI_XING];		// β�����memID
}WeiXingInfo;

// ��̬ѡ���б����
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
	unsigned short unPhyAtk;// ��������
	unsigned short unMgcAtk;// ����������
	unsigned short unBrkATk;// �ƻ���

	unsigned short unFireATk;		//4 �𹥻�
	unsigned short unThunderATk;	//4 �׹���
	unsigned short unLightATk;		//4 �⹥��
	unsigned short unNightATk;	//4 ������
	unsigned short unWaterATk;	//4 ˮ����
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
	tdr_ulonglong ullWID;	// ��������WID 
}FairyOnline;

typedef struct
{
	char cCampIdx;	// ��Ӫidx
	char cPloyType;	// 
	char fLeader;	
	int iCrystal;		// ˮ����
	int iOre;			// ��ʯ��
	int iKillNum;		// ɱ����
	int iContinueKill;	// ��ɱ��
	int iCurrKill;		// ��ǰɱ����
	int iDieNum;		// ��ɱ��(�����ֺ���)
	int iMaxLife;		// �������ʱ��
	int iCurrLife;		// ��ǰ����ʱ��
	int iDieSec;		// ����������
	int iLevel;
	int iAssistKill;		//2����
	int iTotalExp;
	int iAwardTime;
	int iEnergyStat;
	time_t tFightTime;	//����ʱ��
}WarFieldOL;

typedef struct
{
	int iQueueType;
	int iPworldID;
	int iPworldInstID;
	int iCamp;
	int iQueueIdx;			// �ȴ���������
	SSBATTLECAMPINFO stBattleCampInfo; //ս����Ӫ��Ϣ ���Ҫ��  battle_svr�鲻��waitqueue
}RolePworldQueue;

typedef struct
{
	int iQueueType;
	int iPworldID;
	int iPworldInstID;
	int iWaitQueueIdx;			// �ȴ���������
}ShiKongRoleQueueInfo;


//ʣ���Ѳ��·��,�����;��ƫ��,����·��Ӧ��Ҫ�����������
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

//#define SCENEOVERLAY_SAFE 1        	// ��ȫ��
//#define SCENEOVERLAY_TEST 2		// �д���

typedef struct {
	AUTOTEAMAPPLYINFO stApply;
	char AutoTeamID;
	char Stat; //0 single 1 created 2 pre
	unsigned char LimitTrigFlag; // 1 ��ʾ�Ѿ���ʱ�ſ�������	
	int bOPFlag; //�������
	time_t tStart; //��ʼ�ŵ�ʱ��
	time_t tAutoTeamValidTime;//�Զ���ӳɹ���,130�뵹��ʱ���������ʱ����,���Ҷ��鲻�ڸ�����
                                                //��������bOPFlag,��AutoTeamID, ������,����յ���TeamInfo����������
                                                //AutoTeamID <=0ʱ,����Զ����ͼ��,���������Խ��븱�������
	char TransAutoTeamID;
	int iPworldGrade;
	                                                
}AutoTeamInfo;

typedef struct
{
	int iStartLayer;
	int iUseItemEnterPworld;			//�Ƿ�ʹ�õ��߽��ĸ��� 0-�� 1-��
	int iRaffleNum;	
	int iRndDefIdx;					// rnd������
	int iItemIdx;						// �ڼ�����Ʒ
	int iBctFlag;						// ��¼ʱҪ�õ� ����
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
#define MAX_CHOOSE_NUM 2 //���ѡ���¼�����

typedef struct
{
	char cChooseBindType;	//ѡ��ȷ�Ͽ�ű�������
    int iChooseMemID;	//ѡ��ȷ�Ͽ�ű�������ID
    int aiChooseProc[MAX_CHOOSE_NUM];//ѡ���Ӧproc
    char szScript[RES_NAME_LEN];//�ű���
    int iChooseLevel;	//ѡ��ȷ�Ͽ�㼶
}ChooseInfo;

typedef struct
{
	int iMemID;				// �д�Է�MemID
	int iReqID;				// �д�����
	int iAcceptTm;			// �д�ͬ���ʱ��
	int iLeaveTm;			// �뿪�д������ʱ��
	RESPOS stTestPos;		// �д�����λ��
}QiecuoInfo;

typedef struct
{
	char bBHlingRate;	// ���鱳�������
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
	PET_FUSE_NO = 0, 		//2�����ں�״̬ 
	PET_FUSE_READY = 1, 	//2 �ں�״̬ 
	PET_FUSE_DEAL = 2, 		//2 ȷ���ں�״̬ 
};

typedef struct
{
	tdr_ulonglong ullPetWid; 			//1�Լ���Ҫ�ںϵĳ���
	tdr_ulonglong ullOtherPetWid; 		//1 �Է���Ҫ�ںϵĳ���
	int iMemID;						//1�Է���ɫmemid
	int iInViteTime;					//1����ʱ����߱�����ʱ��
	
	char bStat;						/*PET_FUSE_STAT*/
}PetFuseOnline;

#define MAX_PLAYER_BUB 5

enum tagMON_BUB_STAT 
{
	BUB_STAT_ATK = 0, 		//2����״̬
	BUB_STAT_FOLLOW = 1, 	//2 ����״̬
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

//�������������� 
typedef struct
{
	int iJumpNum; //��Ծ���� 
	RESPOS arrJumpedPos[MAX_LIGHTNING_JUMP_NUM];//�Ѿ�������pos
	int iMemID[MAX_LIGHTNING_JUMP_NUM];//�Ѿ�������pos
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

#define MAX_PLAYER_PROC_NUM 4 //������ͬʱ�ȵ�proc��

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
	int iEndTime; // ��Щ��ͼ������Ҵ���ʱ��
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
    int Map;                             	/*   ��ͼID */
    int X;                               	/*   ����x */
    int Y;                               	/*   ����y */
    int MapPos;                         	/*   map ʵ��λ�� */
	int OverTime;							//��ʱʱ����
}TeamSummonInfo;

typedef struct tagClanSummonInfo
{
    int Map;                             	/*   ��ͼID */
    int X;                               	/*   ����x */
    int Y;                               	/*   ����y */
    int MapPos;                         	/*   map ʵ��λ�� */
    int OverTime;				//��ʱʱ����
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
	SPAN_STAT_CONNECT = 1, 		//2(���߲Ŵ��ڵ�״̬) ������������״̬
	SPAN_STAT_HOLD_REQ = 2, 		//2��������״̬, ����Ԥռλ
	SPAN_STAT_HOLD = 3, 			//2ռλ״̬��������һ���յ�Player,ռס��һ������
	SPAN_STAT_UPDATE = 4, 			//2���ݻ�д״̬������ռλ�ɹ���
	SPAN_STAT_APPLY = 5, 			//2����״̬�����ˣ���Ϊ�˴���������߼���Ҫ
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
	int iGridIdx;                         	/*   λ�� */
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
	char cPatrolFlag; // 1��ʾ������·�� 2��ʾ·��Ҫ�ָ�
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
	int iVAnimate;//���Ӷ�̬����
	int aiVAnimate[MAX_VISIBLE_ANIMATE];
	int iVInanimate; // ������Ʒ����
	int aiVInanimate[MAX_VISIBLE_INANIMATE];
	char cDoSearch;
	char cCheckCltTimeFlag;
	unsigned char cDataDirty; //��ʶstRoleData���Ƿ�����Ҫ���ݱ仯
	unsigned char cCheckName;
	DYNATTRLIST stDyn;
	ZoneSkillList stSkillList;
	ZoneStatusList stStatusList;
	AtkInfo stAtkInfo;
	TestEnv stTestEnv;
	ScriptData stSData;
	int iDialogID;
	time_t tPackageArrange;	// �����
	time_t tPackageArrange1;// �ֿ�
	TeamOnline stTeamOnline;
	char cDialogStat;
	unsigned char bTrustPosCount;
	unsigned char bFirstAjustCount;
	CSLOCK stLock; //������Ϣ
	long long cMiscFlag; //һЩ������MISC_FLAG_PACKAGE_ALL
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
	ClanOnline stClanOnline; // �йع����������Ϣ
	char cUIType;
	unsigned char bQiYueArmIdx;
	unsigned char abFollowAngel[FOLLOW_ANGEL_DIV];
	unsigned char bDbBuddyRetry;
	char cDialogStatOld;
	char cSecenOverLay;		//�������򸲸�
	unsigned char cSecenFlag;
	time_t tTrustUpdateEnd;
	int iClanMemIdx;
	//FittingsOnline stFitOneline;
	StallOnline stStallOnline;
	int tLogoutBegin; // ���յ��˳�ָ���ʱ��
	unsigned char bWaitRoleLogout; // �ȴ��˳�
	unsigned char bConnLost; //conn�����Ƿ�Ͽ�
	unsigned char bLogoutPre;  //2�Ƿ������˳�ǰ�����߰˰˲���
	
	int iSharingTaskID; // �������� ID
	int tSharingTime; // ��ʼ�����ʱ��
	int iSharingOwner; // ������������
	//int iGrabSafeTm;	// ��������ʱ��
	
	time_t tRoleLogin;
	time_t tRoleLogout;
	int iCallNpcMemID;	// �ٻ�NPC �ڴ�ID
	WeiXingInfo stWeiXing;
	DynamicOptListInfo stDynamicOpt; // ��̬ѡ���б�
	RideOnInfo stRideOnInfo;		//������������Ϣ
	FairyOnline stFairyOnline;
	int ePkMode;						//1Pkģʽ
	time_t tPkEndTime;						//1 ����PK������ʱ��
	int iSkillAtkerID;						//״̬���������ͷż��ܵ���
	
	time_t tRoleChgMap;
	WarFieldOL stWarOL;
	RolePworldQueue stPworldQueue;

	AutoTeamInfo stAutoTeamInfo;
	BCTOnline stBctOnline;
	RecipePreOnline stRecipePre;

	tdr_ulonglong nExtendedResultBitMap; // ��ħװ��bitmap,���ڿ����ж���Щ���ϴ�
	                                    // �ŵ�װ���и�ħЧ��
	time_t tExtendedResultEnd[MAX_WEAR]; // ��¼��ħ����Ч������ȴʱ��

	int iPressedMonNum; // ��¼��Щ�����ѹ��������Player
	int aiPressedMon[PLAYER_MAX_PRESSED];

	MachineInfo stMachineInfo;
	CSPLAYERATTRVAL stBaseAttrList;
	int aiProcEvtIdx[MAX_PLAYER_PROC_NUM];
	int iProcEvtNum;//�������߼�������
	char cSafeLockFalg;

	ChooseInfo stChooseInfo;//��¼ѡ��ȷ�Ͽ���Ϣ
	QiecuoInfo stQiecuoInfo;	// �д���Ϣ

	time_t tAutoReviveTime;

	AtkBossInfo stAtkBossInfo;
	BHLingOL stBhlingOl;
	PetFuseOnline stPetFuseInfo;
	BubOnline stBubOnline;

	LightningAtkInfo stLightningAtkInfo[MAX_LIGHTNING_SKILL_NUM];
	char szAddFriendName[NAME_LEN];

	int fAasMsgNotify;			//1��Щ��������ʾ��Ҫ���ξ���
	int iLeaveStat;
	MOTIONINFO stMotionInfo; 	//2��������

	int iSelectTarget;				//1Ŀ��
	int iOldSelectTarget;

	ItemUseInfo stItemUseInfo;
	unsigned int ClanItemID;

	int eStatusAssistant;//������һЩ״̬
	int ZoneMemID;
	int iZoneConnectID;
	TFRAMEHEAD stFrameHead;
	PetStrengthenUIInfo stPetStrengthenUIInfo;
	int iUsedOfflineExp;//��þ����а��������߾���
	AutoMove stAutoMoveInfo;
	UnFengYin stUnFengYin;

	//���ֵܹ�Ŀ���ȴ���,��������ظ�����
	int iSkillMonID;//���ֵܹ�id
	unsigned char SkillMonTargetNum;                      
    	int SkillMonTargetIDs[MAX_SKILL_TARGET];  

	int iBeachMapSec;//��̲��ͼ����

	int iWorldCityBuffFlag;//ʱ�������սbuff��־,��ӦWORLD_CITY_BUFF_FLAG
	int iWorldCityProcEventFlag;//ʱ�������ս��ͼ�߼�����־,��ӦWORLD_CITY_PROC_EVENT_FLAG
	int iWorldCityBuildSerial;//ʱ�������ս��ͼ����ռ��仯���к�  

	time_t tDropTmpItemEnd;
	int      iDropTmpID;

	int iKillerObjType;//��������,ʬ����
	int iKillerMemID;//����id,ʬ����

	int iShopSendSeq; //�̳ǵ��ߵ��·���¼�������¼̫����Ҫ�ֿ���
	int iShopCurrPkg;
	int iOldChangeMapID;
	struct timeval stSkillMissTime;		//����ʱ��
	VIPBARINFO stVipBarInfo;//vip������Ϣ
	RECASTINFO stRecaseInfo;//�����ɫװ������δȷ���Ľ��
	HETIOL stHeti;
	char cClanPos;
	char fRealBiaoChe;
	char fShamDead;
	char fPetShouHu;

	int aiRuneColorCount[RUNE_YELLOW + 1];
	RUNEEXENABLEINFO RuneExEnableInfo[MAX_RUNE_EX];   	//���Ķ������Լ�����Ϣ

	ArmStarOL stArmStarOL;	//1װ��ǿ����ʱ��Ϣ
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
	int iVigorEx;//�����õĻ���
	int iAddWorldLevelExp;	//1���羭������ʾ��Ҫ 
	int iGameNotifyFlag;		//2�����¼���¼���
	CityFightOL stCityFightOL;
	int iDieNum;			//1�����ɱ���ļ��������е�ͼ��ɱ��������Һ�����
	time_t tCityCltCD;
	ClanSummonInfo stClanSummonInfo;
	char fBusyCheck;
	char bRongluReset;
	int iSpanGoldRate;
	ExtendedOL stExtendedOL;

	tdr_ulonglong iRoleID;
	char cSecondPwd[SECOND_PWD_LEN];//��ʱ�����������
	int iSecondPwdCmd;//  1 ���� ��¼   2���� ɾ��   

	CardBoxOL stCardBoxOL;
	ArmReSetOL stArmResetOL;

	int iPaoShangID;	//����id
	int iPaoShangNpcID;   //����npcid

	int iSmsCode;	//������֤��
	int iSmsTimeOut;//��ʱʱ��
	char szSmsMobile[NAME_LEN];                	/*   �ֻ����� */
	int iPetFgtTime;			// �������ʱ�䣬����ʵ�ֳ���ÿ��x������˼�Buff

	int iWillToSpouseStatus; //�Է�Ը���Ϊ���޲���0��ʾδѡ��1��ʾԸ�⣬2��ʾ��Ը��
	tdr_ulonglong ullLoveID;//��¼��˭��׵�id
	char cPursueName[SECOND_PWD_LEN];//��ʱ����������
	
	ShiKongRoleQueueInfo stShiKongQueueInfo;  // ʱ����Ԩ������Ϣ
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
	int iSpanMapCamp; //���ڿ��Ұ���ͼ����Ӫ
}PlayerSyncOnline;

char szWatchPlayerSyncOnline[MAX_PROXY_PLAYER_ONLINE_BUF_LEN - sizeof(PlayerSyncOnline)];


#define PLAYER_ONLINE_DATA 44304

#define  AAS_HOUR 3600
#define  AAS_HALF_HOUR 1800
#define  AAS_QUARTER_HOUR 900

//typedef enum tagRoleLogoutReason
//{
	//ROLE_LOGOUT_CLTREQ = 1, /*��������ǳ���ɫ*/
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
	GRAB_FLAG=1,		/*��ɫ���ű��*/	
};

// ��������ʱ��(����)
#define GRAB_SAFE_TIME 10 
enum ACC_SHARE_STATE
{
	ACC_SHARE_INIT, //��ʼ��״̬
	ACC_SHARE_GETING,	//��ȡdb������
	ACC_SHARE_FINAL,	//��ȡ����
	ACC_SHARE_FAIL,	//ʧ��
	ACC_SHARE_INS,	//����
};

typedef struct 
{
	int iStat;	//״̬,��ӦACC_SHARE_STATE
	int iDBQureyTime; //db�����ʱ��
	int iAccMaxRoleLevel; //�ʺ������Ľ�ɫ�ȼ�
	ACCSHARE stAccShare; //�ʺŹ���Ķ���
}AccShareData;

typedef struct
{
	int iMagic;
	PlayerOnline stOnline; //������ɫ���߱������ݣ�������Ұ�������
	char sRes1[PLAYER_ONLINE_DATA - sizeof(PlayerOnline)]; //Ԥ��һ���ռ䣬��Ӫʱ�仯
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
	int iAccSilenceEnd; // �ʺŽ��Խ���ʱ��
	int iCltVer;			//�ͻ��˰汾��
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
	int aiAreaDyn[MAX_MAP_AREA];	//�����ڴ������
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
	int iDynMskMonCount;//һ�������������ٸ���̬�赲����
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

#define PWORLD_MAPID_BASE 10000  //10000 �������������ͼ
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
	char szRoleName[ROLE_NAME_LEN]; // ����ӵ���߽�ɫ��
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

/* ��Ӫ����Ϣ */
typedef struct tagPworldCampInfo
{
    	int ZoneBusID;                       	/*   ����Ӫ�������ĸ�world��,��BusID */
    	char PworldCampID;                    	/*   ��������Ӫid,campnum���������,��0��ʼ */	
}PWORLDCAMPINFO;

typedef struct 
{
    int iMemID;
	int iDefID;
	RESPOS stPos;//����
	int iHP;//Ѫ��
	int iCamp;
	int iDieTime;//����ʱ��
	int iRangePos;//��������
}PworldCampMonInfo;

typedef struct 
{
    int iCampLv;
	int iPlayerNum;
	CLANID stClanID;//������Ӫ��Ӧ�Ĺ���gid
	int CampVal;                         	/*   ��Ӫ���� */
	int iSvrBusID;
	int iKillNum;//��ɱ����
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
	int iFraction;			//1����
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
	int iCurrLayer;	// ��ǰ��
	int iMonCount;	// ��������
	int iKillNum;		// �Ѿ��ɵ���
	int iMon;			// ˢ������
	int ID[UNREAL_MON_TYPE_COUNT];		// ˢ�ֵ�����
}UNREAL;


#define PWORLD_MON_ROUND_MAX 64 //���64 �ֲ��ιֻ�ʱ��

#define AWARD_FLAG_PLAYER 1 //������콱�����
#define AWARD_FLAG_CLAN 2 //�������콱�����
#define AWARD_FLAG_WARNING_TIP 4 //����ʾԤ����Ϣ
#define AWARD_FLAG_REFRESH_TIP 8 //����ʾˢ����Ϣ
#define AWARD_FLAG_EARLY 16 //ʱ������ǰ


typedef struct tagPWorldRoundMonInfo
{	
	int iTotalNum;	//��ˢ������
	int iCurrNum;			// ��ˢ������
	int iCountDown;	// ˢ�µ���ʱ��
	int iRefreshTime;	//��ʼˢ��ʱ��
	int iRepeatTime;	//�ظ�ˢʱ��
	int iCurrRepeat;	//��ǰ�ظ�ˢ����
	int iWaveDefID;		// ���ζ���ID
	int iAwardFlag;		//�������,1���� 2���� 4Ԥ����ʾ 8 ˢ����ʾ
}PWORLDROUNDMONINFO;


typedef struct tagPWorldRoundMon
{
	int iTotalRound;	// �ܹ�������
	int iCurrRound;	// ��ǰ������
	PWORLDROUNDMONINFO aRoundMonInfo[PWORLD_MON_ROUND_MAX];
}PWORLDROUNDMON;


struct tagZonePworldInst
{
	tdr_ulonglong ullPworldWID; //Ψһ��ʶ
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
	int iNextPworldInstID;	//����� ͬʱ�����������������
	int iWaitQueueIdx;
	int iDownTm;			//����ʱ	
	int iMonHpTotal;
	int iMapCurr;
	PworldCamp stPworldCamp;
	PworldBattleInfo stPworldBattleInfo;
	PworldAwardInfo stAwardInfo;
	char chLimitFlag;//0����ÿ�����ƴ���,1��ÿ�����ƴ���
	UNREAL stUnreal;
	int iNextUnrealInstID;
	PWORLDROUNDMON stTimerRound;//��ʱ����Ϣ
	PWORLDROUNDMON stWaveRound;//���ι���Ϣ
	int iStrongEnd;
	int iStrongFlag;
	int iClanSpyFlag;	
	PworldBoxInfo stPworldBoxInfo;
	int iAwardMul;//���������������,��ĸ100
	char cShareShuNiu;
	int iDianFengRound;//ĩ���۷�pk����
	time_t tYanChiTime;//ĩ���۷帱���ӳ�ʱ�䴦��
	int iHomeID;
	int iShiKongWaitQueueIdx;  // ʱ����Ԩ�ȴ���������
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
	AI_STAT_NOBIRTH = 0,  //δ����
	AI_STAT_IDLE, //ԭ�ؿ���
	AI_STAT_DEAD, //����
	AI_STAT_ATK, //����
	AI_STAT_MOVE, //�ƶ�
	AI_STAT_HOME, //��λ
	AI_STAT_TRACE, //׷��
	AI_STAT_RUNAWAY,//��������
	
}MONSTER_AI_STAT;

#define MAX_PRESS_ENTRY 16

typedef struct {
	int iTargetID;
	int iPress; //ѹ��ֵ
}PressEntry;

typedef struct {
	int iTargetCur; //��ǰĿ��
	int iOldTargetID;
	time_t tTargetSelect;

	int iMarkedID; //�������ID
	time_t tMarkedEnd; // ��ǽ���ʱ��
	
	int iDefyID; //������id
	time_t tDefyEnd; //�������ʱ��

	int PressNum; //ѹ�����е���Ч����
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
	int iDebuffNum;//������¼debuf�ĸ���
}TargetAni;
typedef struct{
	int iObj;
	TargetAni  astObj[MAX_SKILL_TARGET];//��debuf��Ŀ��
}DebuffObjList;


typedef struct {
	int iAtkeeID;
	int iSkillIdx;
	unsigned short unSkillID;
	unsigned char bSkillLevel;
	char cAtkStat; //CS_STAT_PREPARE, CS_STAT_ATK
	struct timeval stAtkStatEnd;	
	RESPOS stAtkPos;//���ܹ����ĵ�
	int iSkillSeq;
	unsigned short DirFace;//"�������򹥻���ĳ���ƫ��,����������
	struct timeval  stLastAtk;
	
	unsigned char bSkillType; //0, normal skill; 1 insert skill; 2 atkee skill
	unsigned char bMonSkillIdx;
	char cPilotEnd; //4				�����������
	char sRes[1];
	
	unsigned short unPilot; //4		��������
	unsigned char bNextSkillIdx; //4	������������, ��ʾ�����ͷ����弼���µĵڼ���������
	unsigned char bHeadSkillIdx;    //4���⼼����NormalSkill �е�λ��(Ŀǰֻ��NormalSkill �к�������)
	
	struct timeval stLastPilot;
	int fIsSpecificTarget; // �Ѿ�ָ����һ���ض�Ŀ��

	//�ƶ�Ŀ���
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
	 //MON_UNION_TYPE_ROUNDROLL_MON = 2,	 //���ش�������Բ��(����������)�ĽǶ�(����)
	 MON_UNION_TYPE_XUNLUO_MON = 3,	     //Ѳ�߹ֲ��е�����
	 MON_UNION_TYPE_FACTORY = 4,	     		//4���Ṥ��
	 MON_UNION_TYPE_RONGLU = 5,
	 MON_UNION_TYPE_LOSTTOTEM = 6,	   
	 MON_UNION_TYPE_WORD_PWD = 7,    //  ��������
};

typedef struct {
	int a; //���ٶ�
}XunLuoMon;

typedef struct {
	MONMACHINEINFO stMachineInfo; 	//4��е��Ϣ(�˿͡�����)
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
	int iPower;	// �ռ�������
	int iMaxPower;
	int MonIDs[RONGLU_KILL_MON_MAX];
}MonRonglu;

typedef struct {
	int iPower;	
	int iMaxPower;
}MonLostTotem;

typedef struct {
	int iLine;	 // WordPwdDef ��id
	int iIndex; // �ڼ���
}MonWordPwd;

typedef union {
	XunLuoMon stXunLuoMon;
	MachineData stMachine;
	MonFactoryData stFactory;
	MonRonglu stRonglu;
	MonLostTotem stLostTotem;
	MonWordPwd stWordPwd; //  ��������
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
	unsigned short unMinAtkSkillCool; //��λ100 ms
	unsigned short unTraceDist;
	unsigned short unRadius;
	ScriptIndex stScriptIdx;
	int cMonScriptFlag;
	char cType;// npc, monster
	char cRefresh;
	int iOwnerID; // ͼ�ڡ����׵ȵ�ӵ����
	int iOwnerPKMode; //ӵ���ߵ�PKģʽ
	unsigned long long ullOwnerApplyID; // ����id,�򹤻�id	
	int iCallMonID; //�ٻ����Ĺ���ID 
	int iPatrolID; //Ѳ��·��ID
	int iPatrolIdx;
	int iFormDefID;	// ����󣬼�¼���Ǹ�������������������ָ�ԭ����ˢ��
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
	char cOwnerIDType; // 1 ��ʾ����
	int iLinkedPrev; // ��һ�������ӵĹ���
	int iLinkedNext; // ��һ�������ӵĹ���
	int iGroupID; // ��id,0��ʾ����û����
	int iCampIdx;  //4��Ӫ��Ϣ(���ض�������أ���Ӫ��ϢĿǰֻ��ĳ��ս�������е���Ӫ)
	int iMaxHpbyCall;//�ٻ�ʱ����hp,�ο�

	//����Ҳ������һ����PK��Ϣ��ֻ���ܴ���һ��
	int iPKType;	//4
	tdr_ulonglong ullPKID;

	int nOwnerFastID;	//2���ٲ��ҹ���
	tdr_ulonglong ullBootyClanGID;		//ս��Ʒ�����ض����� 
	int iCityBuildDefID;	//2���н�������ID
	int iKillPlayerNum;	//2����BOSSɱ����Ҽ���

	time_t tLifeEnd;
	
	// һЩ������ͷ�����������
	// ˢ�ֵ�ʱ���������ͻ�Ķ�������
	int iCommType;
	MonCommUnion stCommUnion; 
	char cDartCarNotAtk;	// �����ڳ��ܷ񱻹��� 1-������2-����
	tdr_ulonglong WID;   
	tdr_ulonglong FormMonDropWID; //���ĸ��ֵ����,  �������
	int iFromMonDropLevel; // ���ĸ��ֵ���ģ��ĸ�����ĵȼ��� �������
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
	RESPOS stForcedPos; // ǿ���ƶ�Ŀ�ĵ�����
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
	time_t tFreezeEnd; //��Ӳʱ��

	char cFirstAtkerType; 
	int iFirstAtkerID; //������, ���ڹ���ս��Ʒ����
	tdr_ulonglong ullFirstAtkerWid;
	time_t tLastAtkTime; //��������ʱ��

	//��¼���ﱻ��������Ϣ���ڹ���ai Ѱ·
	int iAtkerID;
	RESPOS stAtkerPos;
	char  cAtkerInfoNew;

	unsigned char bPatrolPos; 
	unsigned char bPatrolTurn; //0 ��ת��1 ��ת
	unsigned char bPatrolTry; 
	unsigned char bDelayScript; //Ϊ����ű�Ƕ��
	char cMonScriptVar[MAX_MON_SCRIPT_VAR];

	struct timeval stAIBeAtkSKill;

	float fRoundRollAngle; // ���ش�������Բ��(����������)�ĽǶ�(����)
	
	int iTotemSkillID;
	int iTotemNext;
	time_t tLastPressUpdate;
	int iPlayerTotemNext;

	int tRunAwayStart; //AI_STAT_RUNAWAY״̬��ʼ��ʱ��,��λ��
	int iRunAwayTime;//����AI_STAT_RUNAWAY״̬�ĳ���ʱ�� ��λ��

	int iCombatTimeCount; // ս��ʱ�����
	int iSkillAtkerID;	
	
	int iOwnerMagic;		//1�̳���ֵ

	unsigned char abFollowAngel[FOLLOW_ANGEL_DIV];
	int iKillerObjType; //��������,ʬ����
	int iKillerMemID;	//����Memid,ʬ����
	int iWarningTime;	//���Ὠ���ܹ���ʱ��������������ʱ��
	int iWarningMul;			//����ɼ���Ѫ����ʾ
	int iFirstAtkerMemID;
	int iFollow;
	int astFollow[MAX_MON_FOLLOW];
	char cSecenOverLay;
	char cRonglu;
	char cBannerEffect;
	int iCallIceDefAdd;
	int iCallPhsicDefadd;

	int iRongYanTeamCapId;	 //�ӳ�id	���������ʹ��                          
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
	int iPatrolID;//�����·���Ļ����ֵ����0,��ʱ���߲�����
	int iPlayerMemID; //�����ĸ���ҵ���ʱ����, >0��ʱ������
	char OwnerName[ROLE_NAME_LEN_SAVE];   //������ҵ�����
	int iStatus; //ʰȡ����ʱ����״̬
	int iStatusLevel;//ʰȡ����ʱ���ĵȼ�
	int iLast;//ʰȡ����ʱ���ĳ�����ʱ��
}ZoneTmpItem;

typedef struct {
	ROLEGRID stGrid;
	tdr_ulonglong ullRoleID; 
	char bViewType;					//1�ɼ�������
	char bType;   					//1������ʾ����
	char bRoll;						//1Roll��״̬
	char bCollectTime;				//1��Ҫ�ɼ���ʱ��
	tdr_ulonglong ullTeamID;
	char szShowFlag;					//�ͻ��˱�ʾ���(0�ӳٲ���;1��������)
	int iFromID;
	tdr_ulonglong ullClanGID;
}ZoneBootyGrid;

typedef union {
		ZoneBootyGrid stBootyGrid;
		ZoneTmpItem stTmpItem;
} OneItem;

#define DROP_ITEM_NOSTAY 0
#define DROP_ITEM_STAY 1

#define TMP_ITEM_FLAG_MAP_REFRESH 0x1	//MapTmpItemDefˢ�µ���ʱ����
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
	time_t tPickLimitTime;		//1����ʱ�䣬����ʱ����ֻ��ӵ���߿���ʰȡ
	int iTmpItemFlag;	//���,pick��dropʱ��Ҫ��RoleTmpSys.ItemFlag����һ��,��ӦTMP_ITEM_FLAG_XXX
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
	int iAchievementID; //���ܹ����ĳɳ�֮·ID
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
	tdr_ulonglong ullPworldWID; //Ψһ��ʶ
}TeamPworldInfo;

typedef struct {

	int iMaxRoll;									//1��ǰ���Roll����									
	tdr_ulonglong ullWinRoleID; 					//1��ǰ���roll����˵�RoleID
	char szWinRoleName[ROLE_NAME_LEN];			//1��ɫ������ʾ��Ҫ�ã���ɫ�п���������
	
	int iWaitNum;
	int iWaitRoller[MAX_TEAM_MEMBER];				//1�ȴ�Roll��Ķ���(Roll���߷�����������ˣ���ֹ���Roll)
	
}TeamBootyRollInfo;

typedef struct {
	
	int iBootyID;								//1ս��ƷID
	time_t tEndTime;							//1Roll�㵹��ʱ��ʱ�䵽�˸�stBootyRollInfo ����Ȩ(�����˶�ûѡΪ0������Ϊ���˿�ʰȡ)	
	TeamBootyRollInfo stBootyRollInfo;
}TeamBootyInfo;

#define MAX_ROLL_TEAM_BOOTY  8

typedef struct {
	TEAMINFO stTeamInfo;
	int iTeamIdx;
	int iPworldInstID;
	tdr_ulonglong ullPworldWID;
	struct timeval stQiYueLastTv; // ��Լ��Ч��ʱ��
	int iQiYueIdx;
	int iPworldNum;
	TeamPworldInfo stPworldInfo[MAX_TEAM_PWORLD_INST];
	AutoTeamInfo stAutoTeamInfo;
	time_t tShadowAlive; // ������battle����Ч,��ʾshadow team info�Ĵ��ʱ��
	int iTeamMapLine;	//4��ͼ��·
	
	int iTeamBootyNum;
	TeamBootyInfo astTeamBooty[MAX_ROLL_TEAM_BOOTY];

	int eStatusAssistant; //�����״̬
	int BattleBusID; /* >0��ʾ��������,Ҫͬ��������ݵ�ָ����battle */
	int ZoneBusID;//�Լ�����busid
	tdr_ulonglong ullZoneTeamID;
	int iZoneTeamIdx;
	int iAutoPworldID;
	int iAutoPworldInstID;
	int iAutoPworldStart;

	//lzk  ����������
	int  iRongYanMonId;   //����id
	int iRongYanCurLevel;//��ǰ����	(�ǵø���)
	int iRongYanSelectMul; //ѡ��ı������� ֻ��ȡ1,2,3,4
	int iRongYanStatus;//1��ʾ���ڴ�֣�0��ʾû��
	int tRongYanBossSelectTimeEnd;//boss ѡ�� 3���Ӻ�ϵͳ�Զ�ѡ��
	int iRongYanBegin;//��¼������ʼ���  1��ʾ���ڽ��У� 0��ʾû��
	int iRongYanOldLevel;//������ٹֺ����粥����   ��¼����ֵ����һ��
	RESPOS stRongYanPos;//ˢ���ҹֵ�����
	ZONEIDX stRongYanZoneIdx;//��¼ˢ�����ҹֵ�zoneIdx   Ϊ����ˢ�ٹ�ʱ �������粥��  
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
	int iLevel;//������Ч�ĵȼ�
	int iNum;//id����
	int aiID[MAX_ACHIEVEMENT_TAIL_ID];//��ͬcond��AchievementID
	int iValid; //�Ƿ��ڴ����г���
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
	int iTotalMonNum;		// ��ˢ������
	int iMonPosDefStart;//��Ӧ����λ�ñ����Ŀ��ʼλ��
	int iMonPosDefCount;//��Ӧ����λ�ñ����Ŀ����
	int iClanAwardDefStart;	//��Ӧ���ά�������Ŀ��ʼλ��
	int iClanAwardDefCount;	//��Ӧ���ά�������Ŀ����
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
	int iPosStart;	//��ͬDefID��ʼλ��
	int iPosCount;	//��ӦDefID����
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
	int iPosStart;	//��ͬDefID��ʼλ��
	int iPosCount;	//��ӦDefID����
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


/* ��Ʒ�������Ժܶ�,��Ʒ�������ܴ洢�ڽ�ɫ����,��ʱ��hash�Ż����㷨 */
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
	CACHE_FLAG_CLEAN = 0,   // �����Ǹɾ��ġ�
	CACHE_FLAG_DIRTY = 1,   // ��������Ҫд�ء�
	CACHE_FLAG_IMM = 2,     // �����ݣ�����д�ء�
	CACHE_FLAG_INEXIST = 3, // ���ݲ������ˣ�������Ҫд�أ�����ɾ����
};

// �����е���������
#define MAX_CLAN_MACHINE 18

enum CLAN_MACHINE_PROPERY
{
	CLAN_MACHINE_MAX_HP, // ��������
	CLAN_MACHINE_DEF,    // ����
	CLAN_MACHINE_ATK,    // ����
	CLAN_MACHINE_CAP,    // ����
	CLAN_MACHINE_COLLECTING_SPEED, // �ɼ��ٶ�
	CLAN_MACHINE_MOVING_SPEED,     // �ƶ��ٶ�
	MAX_CLAN_MACHINE_PROPERTY,
};

typedef struct tagClanMachinePropertyEntry
{
	int iID; // �����еID
	int aiAdded[MAX_CLAN_MACHINE_PROPERTY]; // �����б�
}ClanMachinePropertyEntry;

#define INIT_CLAN_MACHINE_PROPERTY_ENTRY() \
	{ .iID = 0, .aiAdded = { 0, 0, 0, 0, 0, 0, }, }


// �����е�򹫻�Ƽ�������ԭ�����ӵ�����ֵ�Ļ���
typedef struct tagClanMachineProperyCache
{
	int iNum;
	ClanMachinePropertyEntry astProperties[MAX_CLAN_MACHINE];
}ClanMachineProperyCache;


typedef struct tagZoneClanLive
{
	CLANID stOtherSideClan; // �Է�����ID��
	int tActionTime;        // ����ʱ�䡣
}ZoneClanLive;

#define MAX_ROLL_CLAN_BOOTY  16
#define MAX_CLAN_ROLL_MEMBER	200

typedef struct {
	
	int iID;								//1ս��ƷID
	int iRoll;								//1�Ƿ�ROLL����
}ClanBootyWaiterInfo;

typedef struct {

	int iMaxRoll;									//1��ǰ���Roll����									
	tdr_ulonglong ullWinRoleID; 					//1��ǰ���roll����˵�RoleID
	char szWinRoleName[ROLE_NAME_LEN];			//1��ɫ������ʾ��Ҫ�ã���ɫ�п���������
	
	int iWaitNum;
	ClanBootyWaiterInfo astClanBootyWaiterInfo[MAX_CLAN_ROLL_MEMBER];		//1ROLL���������Ϣ
}ClanBootyRollInfo;

typedef struct {
	
	int iBootyID;								//1ս��ƷID
	time_t tEndTime;							//1Roll�㵹��ʱ��ʱ�䵽�˸�stBootyRollInfo ����Ȩ(�����˶�ûѡΪ0������Ϊ���˿�ʰȡ)	
	ClanBootyRollInfo stBootyRollInfo;
}ClanBootyInfo;

enum CLAN_BATTLE_STATUS
{
	CLAN_BATTLE_STATUS_CHECK, // ���׶�
	CLAN_BATTLE_STATUS_APPLY,    //������
	CLAN_BATTLE_STATUS_CAN_MATCH,    //��ƥ��	
	CLAN_BATTLE_STATUS_MATCH,    // ƥ����
	CLAN_BATTLE_STATUS_READY,    // �ɽ�ս��
};

typedef struct tagClanBattleApplyData
{
	int iBattleID; //������ս��id
	int iApplyStaus; //������״̬| CLAN_BATTLE_STATUS
	int iApplyTime;//������ʱ��
	int iApplyMemID;//���������memid
}ClanBattleApplyData;

typedef struct tagZoneClanInfo
{
	int iListVersion; //�б���Ϣ�汾
	int iDirty;
	int iTimerCount;
	
	// �����������ڱ������ߴ���Ķ���
	ZoneClanLive stInviteLive;
	ZoneClanLive stExitEnemyLive;

	int tLastSetBulliten; // ���һ���޸Ĺ����ʱ��]

	// ������е�ͼ/�������
	tdr_ulonglong ullCityPworldWID;
	int iCityPworldInstID;

	//ÿ�ջ����
	tdr_ulonglong ullEveryDayPworldWID;
	int iEveryDayPworldInstID;
	int iEveryDayPworldVal;
	int iMaxEveryDayPworldVal;
	int iBossLife;

	//�ػ���Ŧ����
	tdr_ulonglong ullMachinePworldWID;
	int iMachinePworldInstID;
	int iMachinePworldVal;

	// ���Ĺ������ʵ�ʱ���
	int tWorkingCostTime;

	// �Ƿ����ӳٴݻٹ���(������ɢ�Ƿ��ӳٴݻ�,������������ӳٴݻ�)
	int fWillDelayDestroy;

	// ��������������
	int iOnlineCount;

	// �����еӳ��
	int iMachineIDMap[MAX_CLAN_MACHINE_GRID_NUM];

	// �����е�򹫻�Ƽ�������ԭ�����ӵ�����ֵ�Ļ���
	ClanMachineProperyCache stMacProCache;

	// ������ͼ(�����ѯ�����ʾ������)�仯, ��Ҫ����֪ͨ
	int fWillViewNotify;

	CLANMAPPOS stClanMapPosInfo;

	// ����ս��֪ͨ��ʱ��
	int iWarNotify;

	int iClanTouchStatus;
	ClanBattleApplyData stBattleApplyData; //������ս������Ϣ
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


#define SURVIVE_SUPERGOD_NUM 20 //������鳬��ɱ����
#define SURVIVE_GOD_NUM 15 //�����������ɱ����
#define SURVIVE_MON_NUM 10 //�����������ɱ����
#define SURVIVE_MAN_NUM 5 //������������ܵ�ɱ����

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
	int iListVersion; //�б���Ϣ�汾�����������仯ʱ�޸�
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
//	int iInsertLock;// ������
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


// ��԰
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
//��԰end

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
	int iDirty;  // 1 ��ʱ���£� 2  ��������
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

// �ʼ�ϵͳ begin
#define ZONE_MAIL_DATA 2248
#define MAIL_MAX_TOTAL_NUM ((MAX_PLAYER*65/100)* (MAIL_MAX_NORMAL_NUM + MAIL_MAX_SYSTEM_NUM))

#define POSTOFFICE_OBJ_DATA 340000

#define MAIL_MAX_MAILBOX (MAX_PLAYER*125/100)
#define MAILBOX_BUCKET (MAIL_MAX_MAILBOX * 2)

// ����״̬����
#define MAILBOX_STATE_CONNECTED 1 // �����ӵ��������
#define MAILBOX_STATE_NOTIFIED  2 // �����Ѿ�֪ͨӵ���������ʼ�


/*
typedef struct tagListEntry
{
	int iIdx; // ��mempool�е�����
	int iNext; // ��һ�ڵ�/ͷ�ڵ�
	int iPrev; // ��һ�ڵ�/β�ڵ�
}ListEntry;
*/

typedef struct tagZoneMailInfo
{
	int iDirty; // �����־
	int iWriteBackTm; // ���ݿ��д����ʼ��ʱ��
	int iVersion; // �汾��
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
	char szOwner[ROLE_NAME_LEN]; // ����ӵ���߽�ɫ��
	size_t nNormalNum; // һ���ʼ�����
	size_t nSystemNum; // ϵͳ�ʼ�����
	int iMailList; // �ʼ�����
	int iActive; // ��Ծ��
	int iState; // ����״̬
}Mailbox;

typedef struct tagSubJectPlayer
{
	unsigned long long ullRoleID;
	int iMemID;
	int iInteg;	// ����
}SUBJECTPLAYER;

typedef struct tagSubJect
{
	char cStat;			// �״̬
	char cSubJectNum;	// �Ѿ��������������30��
	int iSubJectID;		// ��ǰ��ID
	int iSuccIdx;			// ��ȷ������������Ƶ�����
	time_t tLockEnd;		// �����������ʱ��
	time_t tResEnd;		// ���۴������ʱ��
	time_t tReqEnd;		// ���λ�������ʱ��
	time_t tStart;			// ���λ��ʼʱ��

	int iPlayer;			// �����Ϣ
	SUBJECTPLAYER astSJPlayer[MAX_PLAYER];
	int iAlready[SUBJECT_MAX_NUM];		// �Ѿ����������ID
}SUBJECT;

typedef struct tagWorldBossOne
{
	int iID;					// monid
	int iFreshTime;			// ����ˢ��ʱ��
	int iNextTime;			// �´�ˢ��ʱ��
	int iDayNum;				// ����ˢ�˼���
	int iStat;					// 1-����0-����
	int iTip;
	
}WORLDBOSSONE;

typedef struct tagWorldBoss
{
	int iBoss;
	WORLDBOSSONE stOne[MAX_WORLD_BOSS_DEF];
	
}WORLDBOSS;

typedef struct tagPostofficeInfo
{
	int iSendList; // �ʼ������б�
	int iSendNum; // �����͵��ʼ�����
	int iDeleteList; // �ʼ�ɾ���б�
	int iDeleteNum; // ��ɾ�����ʼ�����
}PostofficeInfo;

typedef struct tagPostoffice
{
	Mailbox astMailboxs[MAIL_MAX_MAILBOX]; // ����
	size_t nMailboxNum; // ����������
	PostofficeInfo stInfo;
}Postoffice;
char sWatchPostoffice[POSTOFFICE_OBJ_DATA - sizeof(Postoffice)];


typedef struct tagMailboxHash
{
	char szOwner[ROLE_NAME_LEN]; // ����ӵ���߽�ɫ��
	size_t nIdx; // ������Postoffice�е�����
}MailboxHash;

// �ʼ�ϵͳ end

typedef struct tagZoneScriptIndex
{
	char szScriptName[32];
	ScriptIndex stScriptIdx;
}ZoneScriptIndex;


// �������
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
	GAME_EVENT_FLAG_SPY = 1, //��̽�����
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
	short iPworldFlag; // ���Ϊ1     ����0
	short iQueuerNum;
	int iPworldID;
	int iQueuerType; //0Ϊ��� 1Ϊ���� PWORLD_APPLY_UNIT_TYPE
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
	int iQueuerType; //0Ϊ��� 1Ϊ���� PWORLD_APPLY_UNIT_TYPE
	PworldWaitQueuerInfo  stQueuerInfo[MAX_WAIT_QUEUER];

	int iCreateTime;
	int iLiftTime;					// ս����������ʱ��,�����¼һ��,����ÿ��tick��ȥ���
	char PworldCampID;                    	/*   ��������Ӫid,campnum��������� */
	int iPlayerMax;
	int iPlayerNum;
	int iPworldID;
       int iBattleBusID;                     	/*   ���busidҪ������ */
	tdr_ulonglong PworldWID;         /*   ������ʵ��ID */
	PworldCamp stPworldCamp;

}PworldWaitQueue;

typedef struct
{
	short iPworldFlag; // ���Ϊ1     ����0
	short iQueuerNum;
	int iPworldID;
	int iPworldInstID;
	CLANID stClanID;//����id
	int iQueueType;//PWORLD_CLAN_QUEUE iQueueIdx��Ӧ������ 0ΪApply 1ΪWait
	int iQueueIdx;//���е�����
	int iMatchStat;//ƥ��״̬ PWORLD_SVR_MATCH_STATUS
	int iBusID;//master ��busid
	int iSrcMaxLevel;//master������������ӡ�ȼ�
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


//ʱ����Ԩ���
#define MAX_SHIKONG_APPLY_QUEUE 1       // ���ٸ���������
#define MAX_SHIKONG_APPLY_QUEUE_MEM 200   // ÿ���������ж�����
#define MAX_SHIKONG_WAIT_QUEUE 150       // ���ٸ��ȴ�����
#define MAX_SHIKONG_WAIT_QUEUE_MEM 20   //ÿ���ȴ����ж�����
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
	short nLimitFlag; //�������
	unsigned char bCareer; //ְҵ
}AutoTeamSingle;

#define AUTO_TEAM_NEED_MEMBER (MAX_TEAM_MEMBER-2)

typedef struct tagAutoTeamCreated
{
	int iTeamID;
	short nLimitFlag; //�������
	unsigned char OpFlag; //��Ҫ����������ɴ���
	unsigned char nSingleNum;
	AutoTeamSingle  astSingle[AUTO_TEAM_NEED_MEMBER];
}AutoTeamCreated;

typedef struct tagAutoTeamPre
{
	time_t tTime;
	unsigned char OpFlag; //��Ҫ����������ɴ���
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
	char szRoleName[ROLE_NAME_LEN]; // ����ӵ���߽�ɫ��
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
	char szRoleName[ROLE_NAME_LEN]; // ����ӵ���߽�ɫ��
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

//���̼۸�����������
typedef struct {
	int iLeftItemID;    //����id
	int iUserBuyPrice;  //�����û�������Ҫ�ļ۸�
	int iBuyRemainNum;			//��ʣ��������
}PAOSHANGLEFTDATA;

//���̼۸�����ұ�����
typedef struct {
	int iRightItemID;    //����id
	int iUserSellPrice;  //�����û������ļ۸�
}PAOSHANGRIGHTDATA;

//����id��Ӧ�۸�����
typedef struct {
	int iPaoShangID;    //����id
	int iSellNum;  // npc���ĵ�������
	PAOSHANGLEFTDATA stPaoShangLeftData[MAX_PAOSHANG_NPC_SHELL_ITEM_NUM];
	int ItemTotalNum; //ȫ����������
	PAOSHANGRIGHTDATA stPaoShangRightData[MAX_PAOSHANG_DAOJU_NUM];
}PAOSHANGGLOBALDATA;

typedef struct {
	int PaoShangRefTime;  //����ˢ������ʱ��
	int iNpcNum;  //����npc����	
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

	/*----initzero ��������������ɹ����ڴ�ʱ����ʼ��Ϊ0*/
	int iInitRes;

	/*--�����Ƕ���data--*/
	//���ﶨ��
	char sMonsterDef[MAX_MONSTER_DEF][MONSTER_DEF_DATA];

	//����׶μ��ܶ���
	char sMonsterStateSkillDef[MAX_MONSTERSTATESKILL_DEF][MONSTERSTATESKILL_DEF_DATA];

	//��Ʒ����
	char sItemDef[MAX_ITEM_DEF][ITEM_DEF_DATA];

	//�������Զ���
	char sAddAttrDef[MAX_ADDATTR_DEF][ADDATTR_DEF_DATA];

	//��װ����
	char sSuitSetDef[MAX_SUITSET_DEF][SUITSET_DEF_DATA];

	//�����б���
	char sSellDef[MAX_SELL_DEF][SELL_DEF_DATA];

	//�����б���
	char sMapTransDef[MAX_MAP_TRANS_DEF][MAP_TRANS_DEF_DATA];

	//���ִ嶨��
	char sBirthMapDef[MAX_BIRTHMAP_DEF][BIRTHMAP_DEF_DATA];

	//�ж�״̬����
	char sActionStatusDef[RES_STATUS_MAX][ACTION_STATUS_DEF_DATA];

	//����Ч����
	char sLevelUpDef[MAX_LEVEL][LEVELUP_DEF_DATA];

	//�����ȼ��˺�������
	char sArmLevelHurtDef[MAX_ARM_LEVEL_HURT][ARM_LEVEL_HURT_DATA];

	//���ܶ���
	char sSkillDef[MAX_SKILL][SKILL_DATA];

	//���ʶ���
	char sDiathesisDef[MAX_DIATHESIS_DEF][DIATHESIS_DATA];

	//�������
	char sFittingsDef[MAX_SYS_FITTINGS][FITTINGS_DATA];

	//״̬����
	char sStatusDef[MAX_STATUS][STATUS_DATA];

	//״̬��ϵ����
	char    sOverlayTypeRelationDef[MAX_OVERLAYTYPE_RELATION][OVERLAYTYPE_RELATION_DATA];

	//״̬����ָ������
	char    sStatusOverlayDef[MAX_STATUS_OVERLAY][STATUS_OVERLAY_DATA];

	//���ﶯ̬������������
	char    sMonDynAttrDef[MAX_LEVEL][MON_DYN_ATTR_DATA];

	//Ѳ��·������
	char    sPatrolDef[MAX_PATROL][PATROL_DATA];

	//������
	char    sTaskDef[MAX_TASK][TASK_DATA];

	//�����鶨��
	char    sTaskCycleDef[MAX_TASK_CYCLE][TASK_CYCLE_DATA];

	//npc������Ϣ����
	char    sNpcInfoDef[MAX_NPC_INFO][NPC_INFO_DATA];
	
	//���ﶨ��
	char sPetDef[MAX_PET][PET_DATA];

	//����ȼ�����
	char sPetLevelDef[MAX_PETLEVEL][PETLEVEL_DATA];

	//�����Ը���
	char sPetTraitDef[MAX_PET_TRAIT_DEF][PET_TRAIT_DEF_DATA];

	//����ǿ������
	char sPetStrengthenDef[MAX_PET_STRENGTHEN_DEF][PET_STRENGTHEN_DEF_DATA];

	// ��Լ�����
	char sQiYueDef[MAX_QIYUE][QIYUE_DEF_DATA];

	// ��̬���ﾭ�����޶����
	char sMonDynExpLimitDef[MAX_MONDYNEXPLIMIT][MONDYNEXPLIMIT_DATA];

	// װ��������
	char sArmGradeDef[MAX_ARMGRADE_DEF][ARMGRADE_DATA];

	// װ�����ʱ�
	char sArmRandDef[MAX_ARMRAND_DEF][ARMRAND_DATA];

	// װ���������Ա�
	char sArmHideAttrDef[MAX_ARMHIDEATTR_DEF][ARMHIDEATTR_DATA];

	// װ���ֽ��
	char sArmFenJieDef[MAX_ARMFENJIE_DEF][ARMFENJIE_DATA];

	//�������߶����
	char sBirthItemDef[MAX_BIRTH_ITEM_DEF][BIRTH_ITEM_DEF_DATA];

	//���﷢�Զ����
	char sMonsterSpeechDef[MAX_MONSTER_SPEECH_DEF][MONSTER_SPEECH_DEF_DATA];

	//װ���ؼ��ֶ����
	char sArmAddAttrDef[MAX_ARM_ADDATTR_DEF][ARM_ADDATTR_DEF_DATA];

	//���߽��������
	char sOnlineAwardDef[MAX_ONLINE_AWARD_DEF][ONLINE_AWARD_DEF_DATA];

	// ���������鶨���
	char sTaskLimitGroup[MAX_TASK_LIMIT_GROUP][TASK_LIMIT_GROUP_DATA];
	
	// ������н������������
	char   sClanCityBuildingLevelDef[MAX_CLAN_CITY_BUILDING_LEVEL_DEF][CLAN_CITY_BUILDING_LEVEL_DEF_DATA];

	// ����������������
	char   sClanCityLevelDef[MAX_CLAN_CITY_LEVEL_DEF][CLAN_CITY_LEVEL_DEF_DATA];

	// �������������
	char   sClanLevelDef[MAX_CLAN_LEVEL_DEF][CLAN_LEVEL_DEF_DATA];

	// ������ɫս��Ʒ���������
	char   sMonsterColorDef[MAX_MONSTER_COLOR_DEF][MONSTER_COLOR_DEF_DATA];

	// ��Ϸ������
	char   sGameEventDef[MAX_GAME_EVENT_DEF][GAME_EVENT_DEF_DATA];

	// �ƺŶ����
	char   sDesignationDef[MAX_DESIGNATION_DEF][DESIGNATION_DEF_DATA];

	// �������������
	char   sRespactPartyDef[MAX_RESPACT_PARTY_DEF][RESPACT_PARTY_DEF_DATA];

	// �����׶ζ����
	char   sRespactPhaseDef[MAX_RESPACT_PHASE_DEF][RESPACT_PHASE_DEF_DATA];

	// ��˶����
	char   sRideDef[MAX_RIDE_DEF][RIDE_DEF_DATA];

	// ���鶨���
	char   sFairyDef[MAX_FAIRY_DEF][FAIRY_DEF_DATA];

	// �ؼ��ֻ�ֵ�����
	char   sAddAttrBaseDef[MAX_ADD_ATTR_BASE_DEF][ADD_ATTR_BASE_DEF_DATA];

	// �ؼ����漴�鶨���
	char   sAddAttrGroupDef[MAX_ADD_ATTR_GROUP_DEF][ADD_ATTR_GROUP_DEF_DATA];

	// �ȼ�������
	char sLVGift[MAX_GIFT_LV_DEF][GIFT_LV_DATA];

	//ý����������
	char sMediaGiftDef[MAX_GIFT_MEDIA_DEF][GIFT_MEDIA_DATA];

	//ý��齱������������
	char sLtrGiftDef[MAX_GIFT_LTR_DEF][GIFT_LTR_DATA];
	
	// ÿ�ս�����
	char sOneDayGift[MAX_GIFT_ONEDAY_DEF][GIFT_ONEDAY_DATA];

	// ������߱�(�齱)
	char sRndAwdDef[MAX_RAND_AWARD_DEF][RAND_AWARD_DATA];

	// �ײ���
	char sBCTDef[MAX_BCT_DEF][BCT_DEF_DATA];

	// �̶��ؼ��ֱ�
	char sFixedAddAttr[MAX_FIXED_ADD_ATTR][FIXED_ADD_ATTR_DATA];

	// ����ܶ����
	char sSecondarySkillDef[MAX_SECONDARY_SKILL_DEF][SECONDARY_SKILL_DEF_DATA];

	// ������䷽�����
	char sRecipeDef[MAX_RECIPE_DEF][RECIPE_DEF_DATA];

	//����ֶ����
	char sActiveHelp[MAX_ACTIVE_HELP][ACTIVE_HELP_DATA];

	//����ս��Ʒ�����
	char sMonBootyDef[MAX_MON_BOOTY_DEF][MON_BOOTY_DEF_DATA];

	//ս��Ʒ�鶨���
	char sMonBootyGroupDef[MAX_MON_BOOTY_GROUP_DEF][MON_BOOTY_GROUP_DEF_DATA];

	//�ر�ͼ�����
	char sCangBaoTuDef[MAX_CANGBAOTU_DEF][CANGBAOTU_DEF_DATA];

	//�����е�����
	char sMonMachineDef[MAX_MON_MACHINE_DEF][MON_MACHINE_DEF_DATA];

	//����Ƽ������
	char sClanTechDef[MAX_CLAN_TECH_DEF][CLAN_TECH_DEF_DATA];

	//�����Ʒ�����
	char sClanProductDef[MAX_CLAN_PRODUCT_DEF][CLAN_PRODUCT_DEF_DATA];

	//һ������ת�����ձ�
	char sAttrContrastDef[MAX_ATTR_CONTRAST_DEF][ATTR_CONTRAST_DEF_DATA];

	//�ȼ��������Զ����
	char sLevelAttrDef[MAX_LEVEL_ATTR_DEF][LEVEL_ATTR_DEF_DATA];

	//�����ͷ���
	char sDiePunishDef[MAX_DIE_PUNISH_DEF][DIE_PUNISH_DEF_DATA];

	//����ÿ�����߶�ö���
	char sContOnlineDef[MAX_CONT_ONLINE_DEF][CONT_ONLINE_DEF_DATA];

	//��ͨս�������
	char sBattleDef[MAX_BATTLE_DEF][BATTLE_DEF_DATA];

	// �����������ϱ�
	char sFairyStuffDef[MAX_FAIRY_STUFF_DEF][FAIRY_STUFF_DEF_DATA];

	// ����Ʒ�ʱ�
	char sFairyStarDef[MAX_FAIRY_STAR_DEF][FAIRY_STAR_DATA];

	//����ֽⶨ���
	char sPetResolveDef[MAX_PET_RESOLVE_DEF][PET_RESOLVE_DEF_DATA];

	//���＼�ܸ����
	char sPetSkillGridDef[MAX_PET_SKILL_GRID_DEF][PET_SKILL_GRID_DEF_DATA];

	//�����츳���ܶ����
	char sPetNativeSkillDef[MAX_PET_NATIVE_SKILL_DEF][PET_NATIVE_SKILL_DEF_DATA];

	//�����ں϶����
	char sPetFuseDef[MAX_PET_FUSE_DEF][PET_FUSE_DEF_DATA];
	//���߾��鹫ʽ��
	char sOfflineExpParameter[MAX_OFFLINE_EXP_PARAMETER][OFFLINE_EXP_PARAMETER_DATA];

	// ���߽�����
	char sOnlineGiftGroup[MAX_GIFT_GROUP_DEF][MAX_ONLINE_GROUP_DATA];

	//�ű����� ������Ʒ����
	char sWorldItemLimitDef[MAX_WORLD_ITEM_LIMIT_DEF][WORLD_ITEM_LIMIT_DEF_DATA];

	//����ս��Ʒ���������
	char sWorldBootyGroupLimitDef[MAX_WORLD_BOOTY_GROUP_LIMIT_DEF][WORLD_BOOTY_GROUP_LIMIT_DEF_DATA];

	//�Ѷȸ��������
	char sDiffPworldDef[MAX_DIFF_PWORLD_DEF][DIFF_PWORLD_DEF_DATA];

	//�Ѷȸ��������
	char sMonEventDef[MAX_MON_EVENT_DEF][MON_EVENT_DEF_DATA];

	//��������ս��Ʒ��
	char sPworldBootyGroupDef[MAX_PWORLD_BOOTY_GROUP_DEF][PWORLD_BOOTY_GROUP_DEF_DATA];

	//�������۽���ϵ��ս��Ʒ
	char sPworldAwardMulDef[MAX_PWORLD_AWARD_MUL_DEF][PWORLD_AWARD_MUL_DEF_DATA];

	//����������
	char sAtkerHurtMulDef[MAX_ATKER_HURT_MUL_DEF][ATKER_HURT_MUL_DEF_DATA];

	//�ǳؽ���
	char sWorldCityBuildDef[MAX_WORLD_CITY_BUILD_DEF][WORLD_CITY_BUILD_DEF_DATA];

	//ש�����
	char sStarStoneDef[MAX_STARSTONE_DEF][MAX_STARSTONE_DEF_DATA];

	//����ש/vip�Ĺ��ܶ����
	char sStarFuncDef[MAX_STARFUNC_DEF][MAX_STARFUNC_DEF_DATA];

	//�ǳؽ����Ʒ��
	char sCityProductDef[MAX_CITY_PRODUCT_DEF][CITY_PRODUCT_DEF_DATA];

	// ս������
	char sBattleGlory[MAX_BATTLE_GLORY_DEF][BATTLE_GLORY_DEF_DATA];

	// �µı�ʯ�����
	char sGemNewDef[MAX_GEM_NEW_DEF][GEM_NEW_DEF_DATA];
	
	// ��ʯ������ֵ���ձ�
	char sGemAttrStarDef[MAX_GEM_ATTR_STAR_DEF][GEM_ATTR_STAR_DEF_DATA];

	// ��ʯ�ȼ��Ǽ����ձ�
	char sGemLevelStarDef[MAX_GEM_LEVEL_STAR_DEF][GEM_LEVEL_STAR_DEF_DATA];

	//װ�����������
	char sArmRecastDef[MAX_ARM_RECAST_DEF][ARM_RECAST_DEF_DATA];

	// �������Ķ����
	char sUseDef[MAX_USE_DEF][USE_DEF_DATA];

	char sDailyGoalAward[MAX_DAILY_GOAL_AWARD][DAILY_GOAL_AWARD_DATA];

	// ���ĸ��Ӷ������Զ����
	char sRuneAddAttrDef[MAX_RUNE_ADDATTR_DEF][RUNE_ADDATTR_DEF_DATA];

	char sArmStarDef[MAX_ARM_STAR_DEF][ARM_STAR_DEF_DATA];

	// �þ�
	char sUnrealMapDef[MAX_UNREAL_MAP_DEF][UNREAL_MAP_DEF_SIZE];
	char sUnrealDef[MAX_UNREAL_DEF][UNREAL_DEF_SIZE];

	// ����ϴ������ض����
	char sPetWashGeniusDef[MAX_PET_WASH_GENIUS_DEF][PET_WASH_GENIUS_DEF_DATA];

	// �ؼ��ֿ�
	char sNewAddAttrDef[MAX_NEW_ADD_ATTR_DEF][NEW_ADD_ATTR_DEF_DATA];

	// �ؼ���������������
	char sAddAttrRandDef[MAX_ADD_ATTR_RAND_DEF][ADD_ATTR_RAND_DEF_DATA];


	// �ɳ�֮·���
	char sAchievementCategoryDef[MAX_ACHIEVEMENT_CATEGORY_DEF][ACHIEVEMENT_CATEGORY_DEF_DATA];

	// �ɳ�֮·
	char sAchievementDef[MAX_ACHIEVEMENT_DEF][ACHIEVEMENT_DEF_DATA];


	// �ؼ���ϴ�������
	char sAddAttrWashDef[MAX_ADD_ATTR_WASH_DEF][ADD_ATTR_WASH_DEF_DATA];

	// ��ʯ�����
	char sSparDef[MAX_SPAR_DEF][SPAR_DEF_SIZE];

	// װ�������Ǽ������
	char sItemStarRandDef[MAX_ITEM_STAR_RAND_DEF][ITEM_STAR_RAND_DEF_DATA];

	// �ͽ��������
	char sSJTaskDef[MAX_SJ_TASK_DEF][SJ_TASK_DEF_DATA];

	// ��̽�PK���������
	char sClanSpyAwardDropDef[MAX_CLAN_SPY_AWARD_DROP_DEF][CLAN_SPY_AWARD_DROP_DEF_DATA];

	// ��̽�����λ�ñ�
	char sClanSpyTransDef[MAX_CLAN_SPY_TRANS_DEF][CLAN_SPY_TRANS_DEF_DATA];

	// �����ڳ������
	char sDartCarDef[MAX_DART_CAR_DEF][DART_CAR_DEF_DATA];


	// �������ιֶ����
	char sPworldWaveMonDef[MAX_PWORLD_WAVE_MON_DEF][PWORLD_WAVE_MON_DEF_DATA];

	// �������ι�λ����Ϣ��
	char sPworldWaveMonPosDef[MAX_PWORLD_WAVE_MON_POS_DEF][PWORLD_WAVE_MON_POS_DEF_DATA];

	// �������ιֹ��ά����
	char sPworldWaveMonClanAwardDef[MAX_PWORLD_WAVE_MON_CLAN_AWARD_DEF][PWORLD_WAVE_MON_CLAN_AWARD_DEF_DATA];
	
	// ��ⶨ��
	char sSubJectDef[MAX_SUBJECT_DEF][SUBJECT_DEF_DATA];

	// ��Ŀ����
	char sSubJectTypeDef[MAX_SUBJECT_TYPE_DEF][SUBJECT_TYPE_DEF_DATA];

	//���ѽ��������
	char sGayDef[MAX_GAY_DEF][GAY_DEF_DATA];

	// ���а�ƺŶ���
	char sRangDsnDef[MAX_RANGDSN_DEF][RANGDSN_DEF_DATA];

	// ��Ŧ��������buff��Ӧ��
	char sPworldMachineBuffShopDef[MAX_PWORLD_MACHINE_BUFF_SHOP_DEF][PWORLD_MACHINE_BUFF_SHOP_DEF_DATA];

	// ��ҽ�����
	char sPlayerAwardDef[MAX_PLAYER_AWARD_DEF][PLAYER_AWARD_DEF_DATA];

	// ��ɫ
	char sCowFinalAwardDef[MAX_COW_FINAL_AWARD_DEF][COW_FINAL_AWARD_DEF_DATA];

	char sGmTrade[MAX_GM_TRADE][GM_TRADE_DATA];

	// �ݵ㶨���
	char sStrongPointDef[MAX_STRONG_POINT_DEF][STRONG_POINT_DEF_DATA];

	// ���Ἴ�ܱ�
	char sClanSkillDef[MAX_CLAN_SKILL_DEF][CLAN_SKILL_DEF_DATA];

	// �ݵ����Ӷ���
	char sBannerDef[MAX_BANNER_DEF][BANNER_DEF_DATA];

	// ������
	char sReviveDef[MAX_REVIVE_DEF][REVIVE_DEF_DATA];

	// ���Ṥ�ʱ�
	char sClanPayDef[MAX_CLAN_PAY_DEF][CLAN_PAY_DEF_DATA];

	// �ȼ����鲹�������
	char sLevelExpDef[MAX_LEVEL_EXP_DEF][LEVEL_EXP_DEF_DATA];

	// �ȼ���ÿ��BUFF���鲹�������
	char sLevelBuffExpDef[MAX_LEVEL_BUFF_EXP_DEF][LEVEL_BUFF_EXP_DEF_DATA];
	
	// ����Ʒ�ʶ����
	char sRidePinZhiDef[MAX_RIDE_PINZHI_DEF][RIDE_PINZHI_DEF_DATA];

	// ���ﰮ�ĵȼ������
	char sPetLoveLevelDef[MAX_PET_LOVE_LEVEL_DEF][PET_LOVE_LEVEL_DEF_DATA];

	// ռ���ͼ����������Դ�����
	char sMapAwardOreDef[MAX_MAP_AWARD_ORE_DEF][MAP_AWARD_ORE_DEF_DATA];

	// ��¯����
	char sRongluDef[MAX_RONGLU_DEF][RONGLU_DEF_DATA];

	// ְҵ�츳��
	char sGeniusCareerDef[MAX_GENIUS_CAREER_DEF][GENIUS_CAREER_DEF_DATA];

	// �츳���Ա�
	char sGeniusAttrDef[MAX_GENIUS_ATTR_DEF][GENIUS_ATTR_DEF_DATA];

	// �츳���Ա�
	char sBaoXiangDef[MAX_BAOXIANG_DEF][BAOXIANG_DEF_DATA];

	// ʱ���ս��������������
	char sWorldCityFightAwardDef[MAX_WORLD_CITY_FIGHT_AWARD_DEF][WORLD_CITY_FIGHT_AWARD_DEF_DATA];

	// ����boss�����
	char sWorldBossDef[MAX_WORLD_BOSS_DEF][WORLD_BOSS_DEF_DATA];

	//�ȼ���ӡ�����
	char sLevelFengDef[MAX_LEVEL_FENG_DEF][LEVEL_FENG_DEF_DATA];

	//װ������
	char sArmLevelUPDef[MAX_ARM_LEVELUP_DEF][ARM_LEVELUP_DEF_DATA];

	// �콵�긣��ͼ�����
	char sLuckyMapDef[MAX_LUCKY_MAP_DEF][LUCKY_MAP_DEF_DATA];

	// �Һ�æ����
	char sBusyDef[MAX_BUSY_DEF][BUSY_DEF_DATA];

	// �߼�ϴ�����Ľ�Ҷ���
	char sHighWashUseDef[MAX_HIGHWASH_USE_DEF][HIGHWASH_USE_DEF_DATA];

	char sBusyUseDef[MAX_BUSY_USE_DEF][BUSY_USE_DEF_DATA];

	char sItemIDDef[MAX_ITEM_ID_DEF][ITEM_ID_DEF_DATA];

	char sCareerHurtDef[MAX_CAREER_HURT_DEF][CAREER_HURT_DEF_DATA];

	// �츳�������Ա�
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

	//�������������
	char sHangHaiAwardDef[MAX_HANG_HAI_AWARD_DEF][HANG_HAI_AWARD_DEF_DATA];

	char sStoreGridDef[MAX_STORE_GRID_DEF][STORE_GRID_DEF_DATA];

	// װ�����׶���
	char sArmStepUpDef[MAX_ARM_STEP_UP_DEF][ARM_STEP_UP_DEF_DATA];

	// װ������ֵ����
	char sArmGradeValDef[MAX_ARM_VAL_DEF][ARM_VAL_DEF_DATA];
	// װ�������������ñ�
	char sArmReSetDef[MAX_ARM_RESET_DEF][ARM_RESET_DEF_DATA];

	//������޶����
	char sRongYanJuShouDef[MAX_RONGYANJUSHOU_DEF][RONGYANJUSHOU_DEF_DATA];

	//��������������Ա�
	char sRongYanMonDynAttrDef[MAX_RONGYANMONDYNATTR_DEF][RONGYANMONDYNATTR_DEF_DATA];

	// װ�����ܻ�ȡ��
	char sArmSkillNumDef[MAX_ARM_SKILL_NUM_DEF][ARM_SKILL_NUM_DEF_DATA];

	// װ��������ֵ��
	char sArmSkillAttrDef[MAX_ARM_SKILL_ATTR_DEF][ARM_SKILL_ATTR_DEF_DATA];
	

	// ���ά�������
	char sClanAwardDef[MAX_CLAN_AWARD_DEF][CLAN_AWARD_DEF_DATA];

	// �������ս�����
	char sClanWarAwardDef[MAX_CLAN_WAR_AWARD_DEF][CLAN_WAR_AWARD_DEF_DATA];

	char sGeniusMdfDef[MAX_GENIUS_MDF_DEF][GENIUS_MDF_DEF_DATA];

	//���̶����
	char sPaoShangDef[MAX_PAOSHANG_DEF][PAOSHANG_DEF_DATA];

	//���̵��߶����
	char sPaoShangItemDef[MAX_PAOSHANG_ITEM_DEF][PAOSHANG_ITEM_DEF_DATA];

	// 
	char sPetMoneyDef[MAX_PET_MONEY_DEF][PET_MONEY_DEF_DATA];


	char sPetImpUpAttrDef[MAX_PET_IMP_UP_ATTR_DEF][PET_IMP_UP_ATTR_DEF_DATA];

	char sPetImpUpDef[MAX_PET_IMP_UP_DEF][PET_IMP_UP_DEF_DATA];

	char sPetOpAwardDef[MAX_PET_OP_AWARD_DEF][PET_OP_AWARD_DEF_DATA];

	//��ʯ�������
	char sGemPagDef[MAX_GEM_PAG_DEF][GEM_PAG_DEF_DATA];

	// װ���������Զ���
	char sArmTeshuDef[MAX_ARM_TESHU_DEF][ARM_TESHU_DEF_DATA];

	//�ռ����Ҿ��޶���
	char sZJRongYanJuShouDef[MAX_ZJ_RONGYANJUSHOU_DEF][ZJ_RONGYANJUSHOU_DEF_DATA];

	// ս���ݵ�Def
	char sSpanStrongDef[MAX_SPAN_STRONG_DEF][SPAN_STRONG_DEF_SIZE];

	// ��������Def
	char sWordPwdDef[MAX_WORD_PWD_DEF][WORD_PWD_DEF_DATA];

	// ��԰���
	char sHomeLvlDef[MAX_HOME_LVL_DEF][HOME_LVL_DEF_DATA];
	char sHomeMonLvlDef[MAX_HOME_MON_LVL_DEF][HOME_MON_LVL_DEF_DATA];
	char sHomeMonSegDef[MAX_HOME_MON_SEG_DEF][HOME_MON_SEG_DEF_DATA];
	char sHomeFarmDef[MAX_HOME_FARM_DEF][HOME_FARM_DEF_DATA];
	char sHomeSoilDef[MAX_HOME_SOIL_DEF][HOME_SOIL_DEF_DATA];

	// װ�����ܸ���
	char sArmSkillRecastDef[MAX_SKILL_ARM_RECAST_DEF][SKILL_ARM_RECAST_DEF_DATA];

	/*------������Ҫ�����ʼ��-----------*/

	//��Ʒ��ȡ״̬
	int iShopGetStat;  //0 ��ʼ 1 ���ڻ�ȡ����

	//������Ʒ�ܶ���han���洢��,�ӿ�����ٶ�
	ShopMemory stShopMemory; 

	// ��Ʒ�����ƶ���
	//������Ʒ�ܶ���han���洢��,�ӿ�����ٶ�
	ShopLimitMemory stShopLimitDefMemory; 

	int iVipBarGetStat;  //0 ��ʼ 1 ���ڻ�ȡ���� 2��ȡ�ɹ�
	VipBarMemory stVipBarMemory; 
	int iVipBarLevelGetStat;  //0 ��ʼ 1 ���ڻ�ȡ���� 2��ȡ�ɹ�
	VipBarLevelMemory stVipBarLevelMemory; 
	int iVipBarNumGetStat;	//0 ��ʼ 1 ���ڻ�ȡ���� 2��ȡ�ɹ�
	VipBarNumMemory stVipBarNumMemory; 

	VipBarOnlineMemory stVipBarOnlineMemory; 
	DynActiveStatusMemory stDynActiveStatusMemory; 
	CondIdMapMemory stCondIdMapMemory; 


	//���۱���������
	LPSHTABLE pstTradeSellHash;
	TradeSellMemory stTradeSellMemory;
		
	//�󹺱���������
	LPSHTABLE pstTradeBuyHash;
	TradeBuyMemory stTradeBuyMemory;
	
	//��֤��ͼƬ
	char sGdCache[MAX_GD_CACHE][GD_DATA];

	char sItem[MEMPOOL_SIZE(MAX_ITEM, DROP_ITEM_DATA)];
	char sMonster[MEMPOOL_SIZE(MAX_MONSTER, MONSTER_DATA)];
	char sNpc[MEMPOOL_SIZE(MAX_NPC, NPC_DATA)];

	char sNameSht[SHT_SIZE(PLAYER_BUCKET, MAX_PLAYER, sizeof(NameMemID))];
	char sL2PSht[SHT_SIZE(PLAYER_BUCKET, MAX_PLAYER, sizeof(LoginName2MemID))];

	// Team��WID����
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

	// �����
	char sBulletinBoard[BULLETIN_BOARD_OBJ_DATA];

	//װ���ؼ��ֿ�������
	char sAddAttrFastList[ADD_ATTR_FAST_LIST_DATA];

	char sSJTaskFastList[SJ_TASK_LIST_DATA];

	char sSpartaData[SPARTA_QUEUE_DATA];

	//���а�
	time_t tLastRange;
	RangeMem stRange;

	//�Զ����
	AutoTeam stAutoTeam;

	SUBJECT stSubJect;
	WORLDBOSS stBoss;
	SVRWEATHER stWeather;

	SVRREGINFO stRegInfo;

	char sWatchSht[SHT_SIZE(WATCH_BUCKET, MAX_WATCH_ROLE, sizeof(WATCHONE))];

	char sStore[STORE_OBJ_DATA];
	char sStoreSht[SHT_SIZE( STORE_BUCKET, MAX_STORE_NUM, sizeof(StoreHash))];

	ArmSkillAttrFast stArmSkillFast;
	
	PAOSHANGINFO stPaoShangInfo;	   //��¼ˢ�����̵ĵ��߼۸�ˢ��ʱ���
	SpartaInfo stSpartaInfo;
	char sRegionGlobal[REGION_GLOBAL_DATA];
	char sHome[HOME_OBJ_DATA];
	int NewPeoRefTime;  //  ������ʾʱ��

	char sShiKongData[SHIKONG_QUEUE_DATA]; // ʱ����Ԩ
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
	int iPworldInstHead;			//�����ͬʱ�������������ͷ
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
	
	//ƽ�����綨��
	time_t tPworldDef;
	int iPworld;
	char sPworld[MAX_PWORLD_DEF][PWORLD_DATA];
	
	//��ͼ����
	time_t tMapDef;
	int iMapIndex;
	char sMapIdx[MAX_MAP_INDEX][MAP_INDEX_DATA];

	//��ͼ���ﶨ��
	time_t tMapMonsterDef;
	int iMapMonsterDef;
	char sMapMonsterDef[MAX_MAPMONSTER_DEF][MAPMONSTER_DEF_DATA];

	//��ͼnpc����
	time_t tMapNpcDef;
	int iMapNpcDef;
	char sMapNpcDef[MAX_MAPNPC_DEF][MAPNPC_DEF_DATA];

	//�¼�������, �е��˷�, Ӧ�ÿ��Բ���Ҫ
	time_t tProcDef;
	int iProcDef;
	char sProcDef[MAX_PROC_DEF][PROC_DEF_DATA];

	//��ʯ�ֲ����궨���
	time_t tMapOrePositionDef;
	int iMapOrePositionDef;
	char sMapOrePositionDef[MAX_MAP_ORE_POSITION_DEF][MAP_ORE_POSITION_DEF_DATA];

	//��ͼ��ʯ�����
	time_t tMapOreDef;
	int iMapOreDef;
	char sMapOreDef[MAX_MAP_ORE_DEF][MAP_ORE_DEF_DATA];

	//��ͼ��ʱ���߱�
	time_t tMapTmpItemDef;
	int iMapTmpItemDef;
	char sMapTmpItemDef[MAX_MAP_TMP_ITEM_DEF][MAP_TMP_ITEM_DEF_DATA];

	
	//��ͼ����
	int iArea;
	char sArea[MAX_AREA][AREA_DATA];

	//��ͼmsk
	int iMask;
	char sMask[MAX_MASK_DATA];

	//��ͼ��̬����
	char sAreaDyn[MEMPOOL_SIZE(MAX_AREA_DYN,AREA_DYN_DATA)];

	//��̬�赲
	char sDynMsk[MEMPOOL_SIZE(MAX_DYN_MSK,DYN_MSK_DATA)];

	int iMapInst;
	ZONEIDX astMapInstIdx[MAX_MAP_INST];
	char sMapInst[MEMPOOL_SIZE(MAX_MAP_INST, MAP_INST_DATA)];

	char sPworldInst[MEMPOOL_SIZE(MAX_PWORLD_INST, PWORLD_INST_DATA)];
}MapObj;

typedef int (* SearchFunc)(MapIndex *pstMapIndex, TMEMPOOL *pstAreaDynPool, ZONEMAPINST* pstMapInst, RESPOS *pstPos, SearchArea *pstSArea);

//��ʯ����
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
	GAME_FLAG_LUCK = 1,//�긣�������
	GAME_FLAG_IGNORE_REFER_LINE_MAP = 2,//chuange map ʱ��������Ķ��ߵ�ͼ,һ����Ч,���ͺ�Ҫ����
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
	LPSHTABLE pstShtL2P; // Login Name -> Player MemID ��ӳ���
	LPSHTABLE pstShtClan; 
	LPSHTABLE pstShtTrust;
	LPSHTABLE pstShtMailbox; // �����ϣ��
	LPSHTABLE pstShtEvent;
	LPSHTABLE pstShopHash;//��Ʒ��hash����
	LPSHTABLE pstShopLimitDefHash; //��Ʒ�����Ƶ�hash����
	LPSHTABLE pstShtRangeName;
	LPSHTABLE pstShtWID2Team; // WID -> Team idx ��ӳ���
	
	LPSHTABLE pstVipBarHash;
	LPSHTABLE pstVipBarLevelHash;
	LPSHTABLE pstVipBarNumHash;
	LPSHTABLE pstVipBarOnlineHash;
	LPSHTABLE pstDynActiveStatusHash;
	LPSHTABLE pstCondIdMapHash;
	LPSHTABLE pstWatchHash;

	LPSHTABLE pstShtStore;	// ���̹�ϣ 
	
	int iUinCacheShmID;
	int iUinCacheWashoutPos;
	ZoneObj *pstZoneObj;
	ClanObj *pstClanObj;
	GlobalObj *pstGlobalObj;
	StoreObj *pstStoreObj;
	Postoffice * pstPostoffice; // �ʾ�
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
	TMEMPOOL* pstMailPool; // �ʼ��ڴ��
	LPTDRMETALIB pstResLib;
	TDRDBHANDLE hResDBHandle;
	CmdMap astCmdMap[MAX_CS_CMD];
	char szRecvBuffer[MAX_MSG]; /*���������Ϣ������*/
	char szSendBuffer[MAX_MSG]; /*���淢����Ϣ������*/
	TFRAMEHEAD stFrameHeadTemp;
	unsigned short  unGidSeq;  //����ȫ��Ψһidʱ��������
	unsigned short   unWorldID; //����id��������õ���worldid���μ�entity.h
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
	BulletinBoard *pstBulletinBoard; // �����
	WorldEvent *pstWorldEvent;	//Ӱ��������¼���Ϣ
	WorldPworldQueue *pstPworldQueue;
	AddAttrFastList *pstAddAttrFastList;
	SJTaskFastList *pstSJTaskFastList;

	//unsigned char cBattleNum;
	//BATTLEINFO stBattleINfo[BATTLE_PWORLD_NUM];
	

	//��Ӫ��־
	TLOGCATEGORYINST* pstOpLogCat;	
	TLOGCONF stOpLogConf;	
	TLOGCTX stOpLogCtx;	
	LPTDRMETA pstLogicLogMeta;

	struct timeval stStart;
		
	time_t tCheckDB;
	int iTormSeq;
	int iLevelStandStart;

	int iMaxPworldExpMul;	//���������ϵ��֮�͡����㲹��ʱ�õ�

	tdr_ulonglong ullEventLogWID;

	GMLIMIT stGmLimt;
	
	char cClearShm;
	char cReloadMap;
	char cReladPworld;
	unsigned char ucRegionID;
	char cLimitPerm;

	unsigned char ucPlayerScriptFlag;//��ҽű��¼���־
	char cBeachClosed;//��̲�Ƿ�ر�
	int iWorldCityBuildSerial;//ʱ�������ս��ͼ����ռ��仯���к�  
	int iMiscFlag;//������,��ӦGMAE_FLAG
	int iLuckyTipCount;//�긣���ʾ����
	int iLastWorldTick;
	int iPetRndSkill;
	PetRndSkill astPetRndSkill[PET_RND_SKILL_MAX];
	UnrealInfo stUnrealInfo;
	int iRealItemDecNum;//ʵ�ʼ�����Ʒ����

	//SUBJECT stSubJect;
	SUBJECTLIST stSubList;
	int iClanCityInitStatusID;
	int iClanCityInitStatusLevel;
	int iClanCityInitStatusTime;
	int iWarningFlag;
	int iLuckyRandSum;
	int iSmsSN;//�������

	SpartaQueue *pstSpartaQueue;
	COROUTINERPC stCoroutineRPC;
	DBCOROUTINERPC stDBCoroutineRPC;
	DBMetaInfo stDBMetaInfo;

	int iShuNiuClanID;
	int iShopBuyTipTime;

	// ����ݵ�
	RegionGlobalInfo *pstRegionGlobalInfo; 
	char iSpanStrongMaxLvl; 
	char iSpanStrongExpMul; 
	int iSpanMaxBull;
	int iSpanMaxBindGold;
	// ����ݵ�

	ShiKongQueue *pstShiKongQueue; // ʱ����Ԩ
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

// �ñ���������
#define UNUSED( param ) ((void)(param))

// ��ȡһ������ĳ�Ա����
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


