#pragma once
#include "GameEngineTypes.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "GameDBData.h"

#pragma pack(1)
//tolua_begin
enum  
{
	CLIENT_PET_STAT_FIGHT = 1, 		/* ���� */
	CLIENT_PET_STAT_SEEFIGHT = 2, 	/* ��ϰ */
	CLIENT_PET_STAT_DEAD = 4, 		/* ���� */
	CLIENT_PET_STAT_TEMP = 8, 		/* ��ʱ���� */
};

enum
{
	CLIENT_MAX_PET_GENIUS = MAX_PET_GENIUS,
};

enum
{
	MAX_PET_SKILLS = MAX_PET_SKILL_GRID,
};

enum
{
	PET_SKILL_RESET_ITEM	= PET_SKILL_RESET_ID,
	PET_SKILL_LEVELUP_ITEM	= PET_SKILL_UP_ITEMID,
	PET_GROW_RESET_ITEM		= 5233000,
	PET_UNBIND_ITEM_ID_TOLUA = PET_UNBIND_ITEM_ID,
};

enum
{
	PET_RESET_LOCK_GENIUS = PET_WASH_LOCK_GENIUS,
	PET_RESET_LOCK_ATK_GENIUS = PET_WASH_LOCK_ATK_GENIUS,
	PET_RESET_LOCK_PHSIC_DEF_GENIUS = PET_WASH_LOCK_PHSIC_DEF_GENIUS,
	PET_RESET_LOCK_ICE_DEF_GENIUS = PET_WASH_LOCK_ICE_DEF_GENIUS,
	PET_RESET_LOCK_HP_GENIUS = PET_WASH_LOCK_HP_GENIUS,
	PET_RESET_LOCK_HITRATE_GENIUS = PET_WASH_LOCK_HITRATE_GENIUS,
	PET_RESET_LOCK_FLEE_GENIUS = PET_WASH_LOCK_FLEE_GENIUS,
};

enum
{
	PET_OP_FIGHT,		// �ٻ�����
	PET_OP_BACK,		// �ٻز���
};

#define PET_HETI_SKILL_ID	6010
#define PET_STEP_UP_USE_ID_TOLUA	PET_STEP_UP_USE_ID				/* ����������ĵ���ID */
#define USE_ID_PET_STEP_UP_TOLUA	USE_ID_PET_STEP_UP				/* ��������������� */
#define USE_ID_PET_STEP_UP_TOLUA	USE_ID_PET_STEP_UP				/* ��������������� */

struct tagPetNativeSkill_ToLua 
{
	unsigned short SkillID;                         	/*   ����ID */
	unsigned short Rate;                            	/*   ��ü��ܵĸ��� */
};

struct tagPetGenius_ToLua
{
	unsigned short PhsicAtkGenius;                  	/*   ���������� */
	unsigned short IceAtkGenius;                    	/*   ħ���������� */
	unsigned short HPGenius;                        	/*   ��Ѫ���� */
	unsigned short DefGenius;                       	/*   �������� */
	unsigned short HitRateGenius;                   	/*   �������� */
	unsigned short FleeGenius;                      	/*   �������� */
	unsigned short IceDefGenius;                    	/*  Ver.561 ħ���������� */
};

struct tagPetHeti_ToLua
{
	int UseLoyalty;                      	/*   ���������ҳ϶�|�ٷֱ� */
    int MinLoyalty;                      	/*   �����ҳ϶�Ҫ��|�ٷֱ� */
	int MinLevel;                        	/*   �������ٵȼ�Ҫ�� */
};

struct tagPetDef_ToLua 
{
	unsigned int PetID;                          
	char Name[NAME_LEN];                 
	unsigned int IconID;                          	/*  Ver.152 ͷ�� */
	unsigned int MouldID;                        
	char PetDesc[SHORT_DESC_LEN];         	/*   �������� */
	unsigned char Type;                            	/*   �������� Bind Macrosgroup:PET_TYPE,*/
	unsigned char PetAttrType;                     	/*  Ver.144 ������ʾ�������س���� Ѫ�裬�﹥ ��� */
	unsigned char Breed;                           	/*   �Ƿ���Է�ֳ 0 ������ 1���� */
	unsigned char BornSkillGrid;                   	/*  Ver.145 �������0������, ����Ϊָ�������� */
	unsigned char FightLevel;                      	/*   �����ȼ����� */
	unsigned char TiShen;                          	/*   ������ */
	unsigned short NormalSkill;                     	/*   ��ͨ�������� */
	tagPetNativeSkill_ToLua NativeSkill;                     	/*   �������� */
	tagPetGenius_ToLua Genius[MAX_PET_GENIUS];          	/*   ����d c b a s */
	unsigned short NormalWashGenius[MAX_PET_GENIUS]; 	/*   �ͼ�ϴ��ͳ��������漴���� */
	unsigned short HighWashGenius[MAX_PET_GENIUS];  	/*   �߼�ϴ�� */
	short MaxFleeMul;                      	/*   ���ܵ�������������� */
	short MaxPhsicAtkMul;                  	/*   ��������������������� */
	short MaxIceAtkMul;                    	/*   ħ��������������������� */
	short MaxHPMul;                        	/*   ��Ѫ��������������� */
	short MaxPhsicDefMul;                  	/*   ����������������������?*/
	short MaxIceDefMul;                    	/*   ħ��������������������� */
	short MaxHitRateMul;                   	/*   ���е�������������� */
	unsigned char HeavyHitRate;                    	/*  Ver.149 ������ */
	short MaxHeavyHitRateMul;              	/*  Ver.157 �����ʵ�������������� */
	unsigned int BornTrait;                       	/*  Ver.217 ����ָ���Ը� */
	unsigned int BabyPetID;                       	/*  Ver.556 ��һ������ */
	tagPetHeti_ToLua Heti;                            	/*  Ver.1154 �������� */
	int TejiID;                          	/*  Ver.1155 �����Ӧ���⼼��ID */
	int Important;                       	/*  Ver.1216 ������Ҫ�̶ȱ�� */
	int FailRate;                        	/*  Ver.2294 ��׽����ʧ�ܸ���:����� */
};

struct tagPetTraitDef_ToLua 
{
	unsigned int TraitID;                        
    unsigned char TraitType;                       	/*    Bind Macrosgroup:PET_TRAIT_TYPE,*/
    char Name[NAME_LEN];                 
    short PhsicAtkMul;                     	/*   ������������ */
    short IceAtkMul;                       	/*   ħ������������ */
    short HPMul;                           	/*   ��Ѫ������ */
    short PhsicDefMul;                     	/*   ������������� */
    short IceDefMul;                       	/*   ħ������������ */
    short HitRateMul;                      	/*   ���г����� */
    short FleeMul;                         	/*   ���ܳ����� */
    short HeavyHitRateMul;                 	/*   �����ʳ����� */
	char PetTraitDesc[SHORT_DESC_LEN];    	/*   �����Ը����� */
	short TiShenMul;                       	/*  Ver.153 ��������� */
};

struct tagPetLevelDef_ToLua 
{
	int Level;                          
	unsigned int LevelUpExp;                      	/*   �������辭�� */
	short PhsicAtk;                        	/*   ��������ֵ */
	short IceAtk;                          	/*   ħ�������˻�ֵ */
	short HP;                              	/*   ��Ѫ�˻�ֵ */
	short PhsicDef;                        	/*   ��������˻�ֵ */
	short IceDef;                          	/*   ħ�������˻�ֵ */
	short HitRate;                         	/*   ���л�ֵ */
	short Flee;                            	/*   ���ܻ�ֵ */
	unsigned int MonMiniExpLimit;                
	unsigned int MonWeakExpLimit;                
	unsigned int MonSoftExpLimit;                
	unsigned int MonNormalExpLimit;              
	unsigned int MonHardExpLimit;                
	unsigned int MonXiExpLimit;                  
	unsigned int MonJinExpLimit;                 
	unsigned int MonBossExpLimit;                
	unsigned int MonJiExpLimit;                  
	unsigned int HurtMul;                         	/*  Ver.144 �ƻ��� */
	unsigned char AddSkillGrid;                    	/*  Ver.145 �˼������ӵļ�����λ���ͣ�0������, 1 �漴���һ�� */
};

struct tagPetGeniusNamesDef_ToLua
{
	char name[NAME_LEN];
};

struct tagPetStrengthenDef_ToLua
{
	int Level;                          
	int MoneyType;                      
	int MoneyVal;                       
	tagPetGeniusNamesDef_ToLua PetGeniusName[MAX_PET_GENIUS]; 	/*  Ver.388 ����d c b a s */
	int UseItemID;                       	/*  Ver.802 ��Ҫ�Ĳ��� */
	int UseItemNum;                      	/*  Ver.802 ��Ҫ�Ĳ������� */
	int LevelRate[MAX_PET_STRENGTHEN_RATE_LIST]; 	/*  Ver.802 ǿ�������б�����ʣ���Ӧ����1%-5%���� */
	int PerfectRate;                     	/*  Ver.1673  */
	int PerfectNum;                      	/*  Ver.1673  */
	int MoveMoneyType;                   	/*  Ver.1961  */
	int MoveMoneyVal;                    	/*  Ver.1961  */
};

struct tagPetFightAttr_ToLua
{
	 unsigned short PhsicAtk;                        	/*   ������ */
    unsigned short IceAtk;                          	/*   ħ������ */
    unsigned short PhsicDef;                        	/*   ������� */
    unsigned short IceDef;                          	/*   ħ������ */
    unsigned short HitRate;                         	/*   ���� */
    unsigned short Flee;                            	/*   ���� */
	short HeavyHitRate;                    	/*   ������ */
	short YongQi;                          	/*  Ver.163 ����ֵ */
	short DecHurtMul;                      	/*  Ver.1614 ���˱��� */
};

struct tagPetSkillGrid_ToLua
{
	unsigned short SkillID;                         	/*   ����ID */
	unsigned short SkillLevel;                      	/*   ���ܵȼ� */
	unsigned short GridType;                        	/*   �������� */
};

struct tagPetSkill_ToLua
{
	unsigned char SkillGridNum;                    	/*   ���ܸ������� */
	unsigned char AddSkillGridNum;                 	/*   ��¼ʹ�õ������ӵļ��ܸ��� */
	char Res[2];                         
	tagPetSkillGrid_ToLua SkillGrid[MAX_PET_SKILL_GRID];  
};

struct tagPetId_ToLua
{
	int nHeight;
	int nLow;
};

struct tagRolePet_ToLua
{
	unsigned short PetID;                          
	unsigned short TraitID;                         	/*  Ver.141 �����Ը� */
	char Name[PET_NAME_LEN_CLT];          	/*  Ver.141  */
	tagPetId_ToLua WID;                             	/*  Ver.141 ����Ψһ��ʶ */
	unsigned int Exp;                             	/*  Ver.141 ���ﾭ��ֵ */
	unsigned int MaxHP;                           	/*  Ver.141 ���HP */
	unsigned int HP;                              	/*  Ver.141 HP */
	int PetDefIdx;                       	/*  Ver.141 ���������� */
	unsigned short LoveVal;                         	/*  Ver.141 �ҳ϶� */
	unsigned char Level;                          
	unsigned char PetGenius;                       	/*  Ver.141 ���� Bind Macrosgroup:PET_GENIUS_TYPE,*/
	unsigned char BreedNum;                        	/*  Ver.141 ��ֳ�Ĵ��� */
	unsigned char Hierarchy;                       	/*  Ver.141 �ڼ��� */
	unsigned char StrengthenLevel;                 	/*  Ver.141 ǿ���ȼ� */
	unsigned short PhsicAtkGenius;                  	/*  Ver.141 �������������� */
	unsigned short IceAtkGenius;                    	/*  Ver.141 ħ�������������� */
	unsigned short HPGenius;                        	/*  Ver.141 ��Ѫ�������� */
	unsigned short DefGenius;                       	/*  Ver.141 ������������ */
	unsigned short HitRateGenius;                   	/*  Ver.141 ������������ */
	unsigned short FleeGenius;                      	/*  Ver.141 ������������ */
	char StrengthenInfo[MAX_PET_STRENGTHEN_LEVEL]; 	/*  Ver.141 ǿ����Ϣ */
	unsigned short NextStrenLvl;                    	/*  Ver.141 ��һ����ǿ���ĵȼ�|0--����ǿ���� */
	tagPetSkill_ToLua PetSkills;                       	/*  Ver.141  */
	unsigned char StatFlag;                        	/*  Ver.141 ״̬ */
	unsigned char Gender;                          	/*  Ver.141 �Ա� Bind Macrosgroup:GENDER_TYPE,*/
	char Res;                             	/*  Ver.141  */
	tagPetFightAttr_ToLua FightAttr;                       	/*  Ver.148  */
	short HeavyHitRateMul;                 	/*  Ver.157 ���������� */
	char Passive;                         	/*  Ver.247 ģʽ��0������ģʽ��1������ģʽ */
	tagPetSkillGrid_ToLua NativeSkill;                     	/*  Ver.557  */
	unsigned short IceDefGenius;                    	/*  Ver.560 ħ�������������� */
	int MiscFlag;                        	/*  Ver.1156 ����������|ʹ��PET_MISC_FLAG�� */
	unsigned char LockNum;                         	/*   ��ס���ܸ����� */
	char LockIdxs[MAX_LOCK_SKILL_GRID];   	/*   ��ס���ܸ������� */
	int StrengthenCount;                 	/*  Ver.1673 ǿ��������¼ */
	unsigned char LoveLevel;                       	/*  Ver.1747 ���ﰮ�ĵȼ� */
	unsigned char Imp;                             	/*  Ver.2289 �ɳ��ȼ� */
	short WenBao;                          	/*  Ver.2289 �±�ֵ */
	int ImpExp;                          	/*  Ver.2289 �ɳ����� */
};

struct tagPetSkillGridDef_ToLua
{
	int SkillGrid;                       	/*   ���ܸ����� */
	int Money;                           	/*   ������ǰ���������Ǯ */
	int GridRate[MAX_PET_SKILL_GRID];    	/*   ���������ܸ��ʿɸ��� */
	int TransferRate;                    	/*  Ver.556 ���ݸ��������＼�ܸ��������ĸ��� */
};

struct tagCSPetFuseInviteInfo_ToLua
{
	unsigned int MemID;                           	/*   Ŀ�����id */
	char RoleName[NAME_LEN];             
	tagRolePet_ToLua Pet;                             	/*   ���� */
};

struct tagCSPetFuseUI_ToLua
{
	char RoleName[NAME_LEN];             
	tagRolePet_ToLua Pet;                             	/*   ���� */
};

struct tagCSPetFuseBaby_ToLua
{
	unsigned int		MemID;                           	/*   ������ */
	tagRolePet_ToLua	Pet;                             	/*   ���� */
	unsigned int		OtherMemID;                      	/*   ������ */
	tagRolePet_ToLua	OtherPet;                        	/*   ���� */
};
struct tagPetGeniusDef_ToLua
{
	int PinZhi;                         
	int EatMoneyType;                   
	int EatMoneyVal;   
};
struct tagPetMoneyDef_ToLua
{
	int Imp;                            
	int Money[MAX_PET_GENIUS];          
	int Count;                          
};

struct tagPetImpUpDef_ToLua
{
	int Imp;                            
	int ExpUp;                           	/*   Ǳ������ */
	int UseExp;                          	/*   ����ÿ3���ӿ۳����� */
	int TouDecExp;                       	/*   ��͵���ٵ�Ǳ�� */
	int AddExp;                          	/*   ����ÿ3�������ӵ�Ǳ�� */
	int UseMoney;                        	/*  Ver.2290 ÿСʱ�������� */
	int UseContri;                       	/*  Ver.2290 ÿСʱ���Ĺ��ṱ�� */
	int UpUseMoney;                      	/*  Ver.2300 ������������ */
	int UpUseItem;                       	/*  Ver.2300 �������ĵ��� */
	int UpUseItemNum;                    	/*  Ver.2300 �������ĵ������� */
};

struct tagPetImpUpAttrDef_ToLua
{
	short Imp;                            
	short Type;                           
	tagPetGenius_ToLua Genius[MAX_PET_GENIUS];          	/*   ����d c b a s */
};
#pragma pack()

/* PET_TYPE*/
enum 
{
	CLIENT_PET_TYPE_NORMAL		= PET_TYPE_NORMAL, 		/* ��ͨ���� */
	CLIENT_PET_TYPE_ABERRANCE	= PET_TYPE_ABERRANCE, 	/* ���ͳ��� */
};

enum
{
	CLIENT_PET_MAX_FIGHT_SHI		= PET_MAX_FIGHT_SHI,
	CLIENT_MAX_PET_STRENGTHEN_ITEM	= MAX_PET_STRENGTHEN_ITEM,
	CLIENT_MAX_PET_STRENGTHEN_LEVEL	= MAX_PET_STRENGTHEN_LEVEL,
};

enum 
{
	CLIENT_PET_GENIUS_D = PET_GENIUS_D, 	/* ��ͨ���� */
	CLIENT_PET_GENIUS_C = PET_GENIUS_C, 	/* �������� */
	CLIENT_PET_GENIUS_B = PET_GENIUS_B, 	/* �������� */
	CLIENT_PET_GENIUS_A = PET_GENIUS_A, 	/* �ܳ����� */
	CLIENT_PET_GENIUS_S = PET_GENIUS_S, 	/* �������� */
};

enum
{
	CLIENT_PET_GENIUS_D_WASH_MONEY	= PET_GENIUS_D_WASH_MONEY,	/* ��ͨ�û������ĵ�Ǯ */
	CLIENT_PET_GENIUS_C_WASH_MONEY	= PET_GENIUS_C_WASH_MONEY,	/* ���ûû������ĵ�Ǯ */
	CLIENT_PET_GENIUS_B_WASH_MONEY	= PET_GENIUS_B_WASH_MONEY,	/* ����û������ĵ�Ǯ */
	CLIENT_PET_GENIUS_A_WASH_MONEY	= PET_GENIUS_A_WASH_MONEY,	/* �ܳ��û������ĵ�Ǯ */
	CLIENT_PET_GENIUS_S_WASH_MONEY	= PET_GENIUS_S_WASH_MONEY,	/* �����û������ĵ�Ǯ */
};
enum
{
	CLIENT_PET_GENIUS_D_UNBIND_MONEY	= PET_GENIUS_D_UNBIND_MONEY,	/* �������Ϊ��ͨ�ĳ������ĵ�����ֵ */
	CLIENT_PET_GENIUS_C_UNBIND_MONEY	= PET_GENIUS_C_UNBIND_MONEY,	/* �������Ϊ���õĳ������ĵ�����ֵ */
	CLIENT_PET_GENIUS_B_UNBIND_MONEY	= PET_GENIUS_B_UNBIND_MONEY,	/* �������Ϊ����ĳ������ĵ�����ֵ */
	CLIENT_PET_GENIUS_A_UNBIND_MONEY	= PET_GENIUS_A_UNBIND_MONEY,	/* �������Ϊ�ܳ��ĳ������ĵ�����ֵ */
	CLIENT_PET_GENIUS_S_UNBIND_MONEY	= PET_GENIUS_S_UNBIND_MONEY,	/* �������Ϊ�����ĳ������ĵ�����ֵ */
};

enum
{
	CLIENT_MIN_RONG_HE_LV = 50,
};

enum
{
	CLIENT_PET_FUSE_FAIL	= 0,	// �ں�ʧ��
	CLIENT_PET_FUSE_SUCCESS	= 1,	// �ںϳɹ�
};

enum
{
	CLIENT_MAX_RONG_HE_DIST = 10, // ����Ϊ��λ
	CLIENT_MAX_RONG_HE_TIME = 5 * 60,
	CLIENT_RESTREN_ITEMID	= RESTREN_ITEMID,
};

// ��ȡ���ﾲ̬��Ϣ
tagPetSkillGridDef_ToLua* getPetSkillGridDef( int nGridIndex );
tagPetDef_ToLua* getPetDef( int nId );
const tagPetGeniusDef_ToLua* getPetGeniusDef( int nGenius );
const tagPetStrengthenDef_ToLua* getPetStrengthenDef( int nLv );
bool isMaxStrengthenLevel( int nLv );
int getPetStrengthenCost( int nLv );
const tagPetLevelDef_ToLua* getPetLevelDef( int nLv );
const tagPetTraitDef_ToLua* getPetTraitDef( int nTraidId );
const tagPetMoneyDef_ToLua* getPetMoneyDef( int nImp );
const tagPetImpUpDef_ToLua* getPetImpUpDef( int nImp );
const tagPetImpUpAttrDef_ToLua* getPetImpUpAttrDef(int nImp,int nType );

struct PetGrowValue
{
	int Attack;
	int HP;
	int Defend;
	int Resist;
	int Exact;
	int Dodge;
};

struct PetGrowRange 
{
	PetGrowValue Min;
	PetGrowValue Max;
};
//tolua_end

enum
{
	MIN_DIST = 150,
};

struct tagFreedomPetInfo
{
	unsigned int	m_uStartFreeTime;
	float			m_falpha;
};

typedef std::map<tdr_ulonglong, ROLEPET>			ROLEPETMAP;
typedef std::map<tdr_ulonglong, ROLEPET>::iterator	ROLEPETMAP_ITER;

typedef std::map<tdr_ulonglong, int>				ROLEPETINDEXMAP;
typedef std::map<tdr_ulonglong, int>::iterator		ROLEPETINDEXMAP_ITER;
class GamePet//tolua_export 
	: public NetMsgHandler
{//tolua_export
public:
	static void loadPetConfig();
	GamePet(void);
	~GamePet(void);

public:
	// C->S

	void breakPetPreMutexAction();
	//tolua_begin
	// ׼������
	void petPre( int nHightId, int nLowId );
	// �ٻ�
	void petBackUp( int nHightId, int nLowId );
	// ����
	void petFreedom( int nHightId, int nLowId );
	// ����
	void petFreedom( tdr_ulonglong petWID );
	// ���ó������ս��ģʽ
	void setPetFightMode( int nMode,int nHightId, int nLowId );
	
	// ���＼��ѧϰ
	void petSkillLearn( int nHightId, int nLowId, int nGridIdx, Item& item );
	// ���＼������
	void petSkillLevelUp( int nHightId, int nLowId, int nSkillId, int nSkillLv, int nGridIdx, Item& item );
	// ����ϴ���ܸ���
	void petWashSkillGrid( int nHightId, int nLowId, Item& item, int nGridIdx );
	bool isPetStrthenItem( int nItemID );
	// ����ǿ��
	bool petStrengthen( int nHightId, int nLowId, Item& stuffItem, Item* safeItem, int num, int nStrthenLevel );
	// �����Ǽ�����ǿ��
	void petResetStrengthen( int nHightId, int nLowId, int nStrthenLevel );
	// �������ǿ������
	void petSaveStrengthen( int nHightId, int nLowId, int nVal, int nStrthenLevel );
	// �۲����˳���
	void petWatch( int nMemId );
	// �Ƿ�����޸ĳ����ǳ�
	bool canChangePetName( int nHightId, int nLowId );
	// �޸ĳ�����
	void petChangeName( int nHightId, int nLowId, const char* szName );
	//��������ʵ������
	void setPetActor( int nXieDaiPos, GameActor* pActor );
	// �������ʵ��
	GameActor* getPetActor( int nXieDaiPos );
	// ��ȡ���ڳ���״̬�ĳ���ʵ��
	GameActor* getPetActor( tdr_ulonglong WID );
	// �ٻ������еĳ���
	void callBackAllPets();
	// �������＼�ܸ���
	void addPetSkillGrid( int nHeightId, int nLowId );
	// ���Ҫ�����ĳ���
	void addFreedomPet( int nHeightId, int nLowId );
	// �����Ƿ����ڷ�����
	bool isPetInFreedom( int nHeightId, int nLowId );
	// ���ĳ��ǿ���ȼ�������ֵ
	int	getStrthenVal( tagRolePet_ToLua* pOnePetInfo, int nIndex );
	// �������＼��
	void LockPetSkill(int high, int low, int idx, bool usecoin);
	void UnLockPetSkill(int high, int low, int idx);
	// ���ó��＼��
	void ResetPetSkill(int high, int low);
	// ��������
	void LevelUpPetSkill(int high, int low, int idx);
	// ����û�
	void ResetPetGrow(int high, int low, int lock, bool usecoin);
	void ResetPetGrow(int high, int low);
	void ResetPetGrowSave(int high, int low);
	void petFreeHeti(int high, int low);
	void loveLevelUp( int high, int low );
	//tolua_end

	// S->C
	// ע��������Ϣ
	void initPetNetMessage();
	void releasePetNetMessage();
	// ����Э�鴦��
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	// ��ʼ��������λ
	void initPetPackage( tagPetPackage* petInfo );
	// ������λ���仯
	void setPetPackageGridInfo( int nMaxNum );

private:
	// ���ó�������
	void setPetAttr( ROLEPET& petAttr, CSPETATTRCHG* data );

public:
	// ����ӵ����
	void setOwner( GameActor* pOwner );
	// TODO:
	
	// ��������ģ�ͳ���
	void setShiModle( int nOwnerId, int nXieDaiPos, int nPetId, const char* szPetName, 
		int nPower, int nMaxPower, int nLevel, tdr_ulonglong PetWID, int nOpCode, 
		int nPetFightMode = -1, int nDiathesisNum = 0, 
		const tagCSPetDiathesisView* pPetDiathesises = NULL,
		int nGoodNum = 0, const CSROLESTATUS* pGoodList = NULL, int nGeniusType = -1, 
		int nStrengthenLevel = -1, int nBattleFlag = 0 );
	// TODO: ���ع��õĽӿ���֤�¹������滻�Ͻӿ�
	// �ı�������ģʽ��������
	void changePetNameOrFightMode( const CSPETVIEW& fightPetServeData );
	// �ջس���
	void callBackPet( const CSPETVIEW& fightPetServeData );
	// ������ʾ����ģ��
	void reShowModle();
	// ���ó���λ��
	void setPetPostion( GameActor* pPetActor );
	void setPetPostion( int nXieDaiPos );
	// ���ó������
	void setPetHeti(tdr_ulonglong WID,int nPetGenius, bool heti, int nBattleFlag = 0 );
	void onPetHeti(GameActor* pPetActor, bool heti);
	void onPetHeti(bool heti);

	// ���ó����ƶ�λ��
	void setMoveToPoint( int to_x, int to_z );
	// �����ƶ�
	void moveTo( int nXiDaiPos, int to_x, int to_z );
	// ˢ�³����ƶ��ٶ�
	void updatePetMoveSpe();
	//
	void FreedomPetOnLeaveMap();
	// ˢ�·������ﶯ��
	void UpdateFreedomPetAnim( unsigned int dtick );
	void UpdateFreedomPetPopRich( FollowCamera *pcamera );

	void SetGeniusInfo( GameActor* pFightPet, int GeniusId, int GeniusLv );

	//tolua_begin
	// ��ȡ������ϵĳ�������
	int getPetNum()
	{ 
		return (int)m_mapRolePet.size(); 
	}
	
	int getPetMaxNum()
	{ 
		return m_nRoleShiMaxNum; 
	}
	int		getBattlePetNum();
	bool	isExtraBattlePet( tagRolePet_ToLua * info );
	bool	isExtraClanJiYangPet( tagRolePet_ToLua * info );

	void	petStartStepUp( int nHighID, int nLowID );
	void	petStartImpUp( int nHighID, int nLowID );

	void	petGiftBookUse( int nGrid );

	void	petStartUnBind( int nHighID, int nLowID );

	// ɾ�������ʱ�����������Ϣ
	void updatePetIndexDelPet( tdr_ulonglong delPetWid );
	// ��ȡ������Ϣ
	tagRolePet_ToLua* getPetInfo( int nIndex );
	// ��ȡ������Ϣ
	tagRolePet_ToLua* getPetInfo( tdr_ulonglong WID );
	// ��ȡ������Ϣ
	tagRolePet_ToLua* getPetInfo( int nHeightId, int nLowId );
	// ͨ��ʵ���ȡ������Ϣ
	tagRolePet_ToLua* getPetInfo(GameActor *pActor);
	// ��ȡ�����ڷ������϶�Ӧ����ֵ
	int	getPetIndex( tagRolePet_ToLua* pPetInfo );
	// ��ȡ���۲������Ϣ
	tagRolePet_ToLua* getWatchPetInfo();
	// ��ȡ���ﾲ̬��Ϣ
	tagPetDef_ToLua* getPetDef( int nId );
	// �жϳ����״̬
	bool isPetInState( int nIndex, int nState );
	// �жϳ����״̬
	bool isPetInState( int nHeightId, int nLowId, int nState );
	// �жϳ����״̬
	bool isPetInState( tagPetId_ToLua& WID, int nState );
	// �жϳ����״̬
	bool isPetInState( tdr_ulonglong WID, int nState );
	// �жϳ�����Ƿ�ʹ�ý��
	bool isPetResetSkillLockUseCoin(tagRolePet_ToLua *info);
	// ��ȡ����������Щ���ܸ�
	int getPetResetSkillLockNum(tagRolePet_ToLua *info);
	int getPetResetSkillLockIdx(tagRolePet_ToLua *info, int idx);
	// ��ȡ���＼�����õ�����
	int getPetResetSkillCostMoney();
	int getPetResetSkillCostMoney(int num, bool usecoin);
	// �жϳ��＼���ܷ�����
	bool isPetSkillCanLevelUp(int id, int level);
	// ��ȡ���＼������������
	int getPetSkillLevelUpCostItemCount(int id, int level);
	// ��ȡ����û�������
	int getPetGrowResetCostMoney(int high, int low);
	int getPetGrowResetCostMoney(int high, int low, int locknum, bool usecoin);
	// ��ȡ���ﵱǰ������ֵ
	PetGrowValue getPetGrowValue(tagRolePet_ToLua *info);
	PetGrowValue getPetGrowValue(int high, int low);
	// ��ȡ�������ʵķ�Χ�ͼ���
	PetGrowRange getPetGrowResetRange(int id,int nImp);
	PetGrowRange getPetGrowResetRange(tagRolePet_ToLua *info);
	PetGrowRange getPetBlinkMinRange( int nGrade, int nPetID, int nImp );
	PetGrowRange getPetGrowResetLimit(int id, int nImp);
	PetGrowRange getPetGrowResetLimit(tagRolePet_ToLua *info);
	//  �ж��Ƿ��п�����λ��ų���
	bool isPetFull();
	// �ж��Ƿ��ܹ���������
	bool canCampaignPet(int nHightId, int nLowId );
	// �鿴�Ƿ�������״̬
	bool isPre();
	// ��ȡ�������
	int getPetHeti();
	// ��ȡ��������ʵ��
	GameActor* getFightPet( int nIndex )
	{
		return m_pShiActor[nIndex];
	}
	//���ݳ�������������Ч
	void setPetHetiEntity( GameActor* pOwner,int nPetGenius );
	//����ս����Ч
	void setBattlePetEntity( GameActor* pOwner,int nPetGenius );
	static bool isPetStrenthenItem( int nStrengthenLvel, int nItemID );

	// �����ں�
	void inviteFuse( int nDestPetMemID );
	// ͬ���ں�
	void acceptFuseInvite( int nDestPlayerMemID );
	// �ܾ��ں�
	void rejectFuse( int nDestPlayerMemID );
	// ȡ���ں�
	void cancelFuse( int nDestPlayerMemID );
	// ͬ���ں�
	void agreeFuse( int nDestPlayerMemID );
	// ��������
	void startEatPet( int nSrcHightId, int nSrcLowId ,int nDesHightId, int nDesLowId );
	// ����ת��
	void startMoveStar( int nSrcHightId, int nSrcLowId ,int nDesHightId, int nDesLowId );

	const tagCSPetFuseInviteInfo_ToLua& getPetFuseInviteInfo();
	const tagCSPetFuseUI_ToLua&			getRongHePlayerInfo();
	tagPetId_ToLua& getSelfRongHePetWID();
	void			clearSelfRongHePetInfo();
	tagCSPetFuseBaby_ToLua& getSuccessFuseInfo();
	//tolua_end

private:
	// �յ�������Ϣ�仯�ظ�
	void	OnGetPetInfoChg( const ROLEPET& newPetInfo );
	void	OnGetNewPet( const ROLEPET& newPetInfo );
	void	OnUpdatePetInfo( ROLEPET& oldPetInfo, const ROLEPET& newPetInfo );
	// �յ�����ǿ���ظ�
	void	OnGetPetStrengthenInfo( const CSPETSVRSTRENGTHEN& petStrengthenInfo );
	// �����ںϵĳ���WID
	void	SaveRongHePetInfo();
	// �յ������ں�
	void	OnGetFuseInvite( const CSPETFUSEINVITEINFO& petFuseInviteRes );

	// �յ��ܾ��ں�
	void	OnGetRejectFuse( const CSPETFUSEREJECT& rejectFuseRes );

	// �յ�ȡ���ں�
	void	OnGetCancelFuse( const CSPETFUSECANCEL& cancelFuseRes );
	
	// �յ�ͬ���ں�
	void	OnGetDealFuse( const CSPETFUSEDEAL& dealFuseRes );
	
	// �յ��Է�ͬ���ں��������Ϣ
	void	OnGetOtherAcceptInviteRes( const CSPETFUSEUI& fuseUIRes );

	// �յ���־�ں��Ƿ�ɹ�
	void	OnGetFuseResultRes( const CSPETFUSESUCC& fuseResultRes );
	// ���ó������������
	void setFightPetAttr( const CSPETVIEW& fightPetServeData );
	// ���ó��������ս��ģʽ
	void setFightPetFightMode( GameActor* pFightPet );
	// �����Ƶ�ĳ��λ��
	void clientSelfPetMove( GameActor* pFightPet );
	void generateRandomMovePoint( int& nDestX, int& nDestY, int nCurPosX, int nCurPosY, int nRangeIndex );
	bool tryMove( GameActor* pFightPet, int nDestX, int nDestY, int nCurX, int nCurY );
	// ǿ����������ֵ�Ƿ�仯
	bool isStrthenValChg( const char* pszLeftVals, const char* pszRighVals );
	int  getChgStrthenLevel( const char* pszLeftVals, const char* pszRighVals );
	// �Ƿ�������ǿ���Ǽ�
	bool isResetStrthenLevel( int nNewCanStrthenLevel, int nOldCanStrthenLevel );
	const tagUseDef *	getPetGrowResetCostMoneyUseDef(int high, int low);

private:
	ROLEPETMAP			m_mapRolePet;
	ROLEPET				m_WatchRolePet;
	ROLEPETINDEXMAP		m_mapRolePetIndex;
	int					m_nRoleShiMaxNum;
	std::string			m_strWatchPetOwer;
	
	GameXYZ			m_movePoint;
	GameActor*		m_pOwner;
	GameActor*		m_pShiActor[PET_MAX_FIGHT_SHI];			// ����ģ��
	unsigned int	m_lastFree;

	tagCSPetFuseInviteInfo_ToLua	m_fuseInviteInfo;
	tagCSPetFuseUI_ToLua			m_rongHePlayerInfo;
	tagPetId_ToLua					m_selfRongHePetWID;
	static tagCSPetFuseBaby_ToLua	m_successRongHeInfo;

	std::map<GameActor*, tagFreedomPetInfo>		m_freedomPets;
	static std::vector<std::string>				m_vecPetFreedomText;
};//tolua_export