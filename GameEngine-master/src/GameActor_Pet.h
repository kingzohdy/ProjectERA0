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
	CLIENT_PET_STAT_FIGHT = 1, 		/* 出征 */
	CLIENT_PET_STAT_SEEFIGHT = 2, 	/* 见习 */
	CLIENT_PET_STAT_DEAD = 4, 		/* 死亡 */
	CLIENT_PET_STAT_TEMP = 8, 		/* 临时宠物 */
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
	PET_OP_FIGHT,		// 召唤操作
	PET_OP_BACK,		// 召回操作
};

#define PET_HETI_SKILL_ID	6010
#define PET_STEP_UP_USE_ID_TOLUA	PET_STEP_UP_USE_ID				/* 宠物进阶消耗道具ID */
#define USE_ID_PET_STEP_UP_TOLUA	USE_ID_PET_STEP_UP				/* 宠物进阶消耗银币 */
#define USE_ID_PET_STEP_UP_TOLUA	USE_ID_PET_STEP_UP				/* 宠物进阶消耗银币 */

struct tagPetNativeSkill_ToLua 
{
	unsigned short SkillID;                         	/*   技能ID */
	unsigned short Rate;                            	/*   获得技能的概率 */
};

struct tagPetGenius_ToLua
{
	unsigned short PhsicAtkGenius;                  	/*   物理攻击资质 */
	unsigned short IceAtkGenius;                    	/*   魔法攻击资质 */
	unsigned short HPGenius;                        	/*   气血资质 */
	unsigned short DefGenius;                       	/*   防御资质 */
	unsigned short HitRateGenius;                   	/*   命中资质 */
	unsigned short FleeGenius;                      	/*   闪避资质 */
	unsigned short IceDefGenius;                    	/*  Ver.561 魔法防御资质 */
};

struct tagPetHeti_ToLua
{
	int UseLoyalty;                      	/*   合体消耗忠诚度|百分比 */
    int MinLoyalty;                      	/*   最少忠诚度要求|百分比 */
	int MinLevel;                        	/*   合体最少等级要求 */
};

struct tagPetDef_ToLua 
{
	unsigned int PetID;                          
	char Name[NAME_LEN];                 
	unsigned int IconID;                          	/*  Ver.152 头像 */
	unsigned int MouldID;                        
	char PetDesc[SHORT_DESC_LEN];         	/*   宠物描述 */
	unsigned char Type;                            	/*   宠物类型 Bind Macrosgroup:PET_TYPE,*/
	unsigned char PetAttrType;                     	/*  Ver.144 用于显示是哪类特长宠物， 血宠，物攻 宠等 */
	unsigned char Breed;                           	/*   是否可以繁殖 0 不可以 1可以 */
	unsigned char BornSkillGrid;                   	/*  Ver.145 出生活的0则不增加, 否则为指定的类型 */
	unsigned char FightLevel;                      	/*   出征等级限制 */
	unsigned char TiShen;                          	/*   替身率 */
	unsigned short NormalSkill;                     	/*   普通攻击技能 */
	tagPetNativeSkill_ToLua NativeSkill;                     	/*   天生技能 */
	tagPetGenius_ToLua Genius[MAX_PET_GENIUS];          	/*   资质d c b a s */
	unsigned short NormalWashGenius[MAX_PET_GENIUS]; 	/*   低级洗点和出生资质随即概率 */
	unsigned short HighWashGenius[MAX_PET_GENIUS];  	/*   高级洗点 */
	short MaxFleeMul;                      	/*   闪避道具修正最大上限 */
	short MaxPhsicAtkMul;                  	/*   物理攻击道具修正最大上限 */
	short MaxIceAtkMul;                    	/*   魔法攻击道具修正最大上限 */
	short MaxHPMul;                        	/*   气血道具修正最大上限 */
	short MaxPhsicDefMul;                  	/*   物理防御道具修正最大上限?*/
	short MaxIceDefMul;                    	/*   魔法防御道具修正最大上限 */
	short MaxHitRateMul;                   	/*   命中道具修正最大上限 */
	unsigned char HeavyHitRate;                    	/*  Ver.149 暴击率 */
	short MaxHeavyHitRateMul;              	/*  Ver.157 暴击率道具修正最大上限 */
	unsigned int BornTrait;                       	/*  Ver.217 出生指定性格 */
	unsigned int BabyPetID;                       	/*  Ver.556 下一代宠物 */
	tagPetHeti_ToLua Heti;                            	/*  Ver.1154 合体数据 */
	int TejiID;                          	/*  Ver.1155 宠物对应特殊技能ID */
	int Important;                       	/*  Ver.1216 宠物重要程度编号 */
	int FailRate;                        	/*  Ver.2294 捕捉基础失败概率:万分率 */
};

struct tagPetTraitDef_ToLua 
{
	unsigned int TraitID;                        
    unsigned char TraitType;                       	/*    Bind Macrosgroup:PET_TRAIT_TYPE,*/
    char Name[NAME_LEN];                 
    short PhsicAtkMul;                     	/*   物理攻击乘修正 */
    short IceAtkMul;                       	/*   魔法攻击乘修正 */
    short HPMul;                           	/*   气血乘修正 */
    short PhsicDefMul;                     	/*   物理防御乘修正 */
    short IceDefMul;                       	/*   魔法防御乘修正 */
    short HitRateMul;                      	/*   命中乘修正 */
    short FleeMul;                         	/*   闪避乘修正 */
    short HeavyHitRateMul;                 	/*   暴击率乘修正 */
	char PetTraitDesc[SHORT_DESC_LEN];    	/*   宠物性格描述 */
	short TiShenMul;                       	/*  Ver.153 替身乘修正 */
};

struct tagPetLevelDef_ToLua 
{
	int Level;                          
	unsigned int LevelUpExp;                      	/*   升级所需经验 */
	short PhsicAtk;                        	/*   物理攻击基值 */
	short IceAtk;                          	/*   魔法攻击乘基值 */
	short HP;                              	/*   气血乘基值 */
	short PhsicDef;                        	/*   物理防御乘基值 */
	short IceDef;                          	/*   魔法防御乘基值 */
	short HitRate;                         	/*   命中基值 */
	short Flee;                            	/*   闪避基值 */
	unsigned int MonMiniExpLimit;                
	unsigned int MonWeakExpLimit;                
	unsigned int MonSoftExpLimit;                
	unsigned int MonNormalExpLimit;              
	unsigned int MonHardExpLimit;                
	unsigned int MonXiExpLimit;                  
	unsigned int MonJinExpLimit;                 
	unsigned int MonBossExpLimit;                
	unsigned int MonJiExpLimit;                  
	unsigned int HurtMul;                         	/*  Ver.144 破坏力 */
	unsigned char AddSkillGrid;                    	/*  Ver.145 此级别增加的技能栏位类型，0则不增加, 1 随即获得一个 */
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
	tagPetGeniusNamesDef_ToLua PetGeniusName[MAX_PET_GENIUS]; 	/*  Ver.388 资质d c b a s */
	int UseItemID;                       	/*  Ver.802 需要的材料 */
	int UseItemNum;                      	/*  Ver.802 需要的材料数量 */
	int LevelRate[MAX_PET_STRENGTHEN_RATE_LIST]; 	/*  Ver.802 强化概率列表万分率，对应增加1%-5%资质 */
	int PerfectRate;                     	/*  Ver.1673  */
	int PerfectNum;                      	/*  Ver.1673  */
	int MoveMoneyType;                   	/*  Ver.1961  */
	int MoveMoneyVal;                    	/*  Ver.1961  */
};

struct tagPetFightAttr_ToLua
{
	 unsigned short PhsicAtk;                        	/*   物理攻击 */
    unsigned short IceAtk;                          	/*   魔法攻击 */
    unsigned short PhsicDef;                        	/*   物理防御 */
    unsigned short IceDef;                          	/*   魔法防御 */
    unsigned short HitRate;                         	/*   命中 */
    unsigned short Flee;                            	/*   闪避 */
	short HeavyHitRate;                    	/*   暴击率 */
	short YongQi;                          	/*  Ver.163 勇气值 */
	short DecHurtMul;                      	/*  Ver.1614 减伤比率 */
};

struct tagPetSkillGrid_ToLua
{
	unsigned short SkillID;                         	/*   技能ID */
	unsigned short SkillLevel;                      	/*   技能等级 */
	unsigned short GridType;                        	/*   格子类型 */
};

struct tagPetSkill_ToLua
{
	unsigned char SkillGridNum;                    	/*   技能格子数量 */
	unsigned char AddSkillGridNum;                 	/*   记录使用道具增加的技能格子 */
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
	unsigned short TraitID;                         	/*  Ver.141 宠物性格 */
	char Name[PET_NAME_LEN_CLT];          	/*  Ver.141  */
	tagPetId_ToLua WID;                             	/*  Ver.141 宠物唯一标识 */
	unsigned int Exp;                             	/*  Ver.141 宠物经验值 */
	unsigned int MaxHP;                           	/*  Ver.141 最大HP */
	unsigned int HP;                              	/*  Ver.141 HP */
	int PetDefIdx;                       	/*  Ver.141 定义表格索引 */
	unsigned short LoveVal;                         	/*  Ver.141 忠诚度 */
	unsigned char Level;                          
	unsigned char PetGenius;                       	/*  Ver.141 资质 Bind Macrosgroup:PET_GENIUS_TYPE,*/
	unsigned char BreedNum;                        	/*  Ver.141 繁殖的次数 */
	unsigned char Hierarchy;                       	/*  Ver.141 第几代 */
	unsigned char StrengthenLevel;                 	/*  Ver.141 强化等级 */
	unsigned short PhsicAtkGenius;                  	/*  Ver.141 物理攻击资质上限 */
	unsigned short IceAtkGenius;                    	/*  Ver.141 魔法攻击资质上限 */
	unsigned short HPGenius;                        	/*  Ver.141 气血资质上限 */
	unsigned short DefGenius;                       	/*  Ver.141 防御资质上限 */
	unsigned short HitRateGenius;                   	/*  Ver.141 命中资质上限 */
	unsigned short FleeGenius;                      	/*  Ver.141 闪避资质上限 */
	char StrengthenInfo[MAX_PET_STRENGTHEN_LEVEL]; 	/*  Ver.141 强化信息 */
	unsigned short NextStrenLvl;                    	/*  Ver.141 下一次能强化的等级|0--不能强化了 */
	tagPetSkill_ToLua PetSkills;                       	/*  Ver.141  */
	unsigned char StatFlag;                        	/*  Ver.141 状态 */
	unsigned char Gender;                          	/*  Ver.141 性别 Bind Macrosgroup:GENDER_TYPE,*/
	char Res;                             	/*  Ver.141  */
	tagPetFightAttr_ToLua FightAttr;                       	/*  Ver.148  */
	short HeavyHitRateMul;                 	/*  Ver.157 暴击率修正 */
	char Passive;                         	/*  Ver.247 模式：0：主动模式；1：被动模式 */
	tagPetSkillGrid_ToLua NativeSkill;                     	/*  Ver.557  */
	unsigned short IceDefGenius;                    	/*  Ver.560 魔法防御资质上限 */
	int MiscFlag;                        	/*  Ver.1156 宠物杂项标记|使用PET_MISC_FLAG宏 */
	unsigned char LockNum;                         	/*   锁住技能格子数 */
	char LockIdxs[MAX_LOCK_SKILL_GRID];   	/*   锁住技能格子数组 */
	int StrengthenCount;                 	/*  Ver.1673 强化次数记录 */
	unsigned char LoveLevel;                       	/*  Ver.1747 宠物爱心等级 */
	unsigned char Imp;                             	/*  Ver.2289 成长等级 */
	short WenBao;                          	/*  Ver.2289 温饱值 */
	int ImpExp;                          	/*  Ver.2289 成长经验 */
};

struct tagPetSkillGridDef_ToLua
{
	int SkillGrid;                       	/*   技能格子数 */
	int Money;                           	/*   开启当前格子所需金钱 */
	int GridRate[MAX_PET_SKILL_GRID];    	/*   格子领悟技能概率可覆盖 */
	int TransferRate;                    	/*  Ver.556 传递给二代宠物技能格子里的书的概率 */
};

struct tagCSPetFuseInviteInfo_ToLua
{
	unsigned int MemID;                           	/*   目标宠物id */
	char RoleName[NAME_LEN];             
	tagRolePet_ToLua Pet;                             	/*   宠物 */
};

struct tagCSPetFuseUI_ToLua
{
	char RoleName[NAME_LEN];             
	tagRolePet_ToLua Pet;                             	/*   宠物 */
};

struct tagCSPetFuseBaby_ToLua
{
	unsigned int		MemID;                           	/*   所有者 */
	tagRolePet_ToLua	Pet;                             	/*   宠物 */
	unsigned int		OtherMemID;                      	/*   所有者 */
	tagRolePet_ToLua	OtherPet;                        	/*   宠物 */
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
	int ExpUp;                           	/*   潜力上限 */
	int UseExp;                          	/*   寄养每3分钟扣除经验 */
	int TouDecExp;                       	/*   被偷减少的潜力 */
	int AddExp;                          	/*   寄养每3分钟增加的潜力 */
	int UseMoney;                        	/*  Ver.2290 每小时消耗银币 */
	int UseContri;                       	/*  Ver.2290 每小时消耗公会贡献 */
	int UpUseMoney;                      	/*  Ver.2300 升级消耗银币 */
	int UpUseItem;                       	/*  Ver.2300 升级消耗道具 */
	int UpUseItemNum;                    	/*  Ver.2300 升级消耗道具数量 */
};

struct tagPetImpUpAttrDef_ToLua
{
	short Imp;                            
	short Type;                           
	tagPetGenius_ToLua Genius[MAX_PET_GENIUS];          	/*   资质d c b a s */
};
#pragma pack()

/* PET_TYPE*/
enum 
{
	CLIENT_PET_TYPE_NORMAL		= PET_TYPE_NORMAL, 		/* 普通宠物 */
	CLIENT_PET_TYPE_ABERRANCE	= PET_TYPE_ABERRANCE, 	/* 异型宠物 */
};

enum
{
	CLIENT_PET_MAX_FIGHT_SHI		= PET_MAX_FIGHT_SHI,
	CLIENT_MAX_PET_STRENGTHEN_ITEM	= MAX_PET_STRENGTHEN_ITEM,
	CLIENT_MAX_PET_STRENGTHEN_LEVEL	= MAX_PET_STRENGTHEN_LEVEL,
};

enum 
{
	CLIENT_PET_GENIUS_D = PET_GENIUS_D, 	/* 普通资质 */
	CLIENT_PET_GENIUS_C = PET_GENIUS_C, 	/* 良好资质 */
	CLIENT_PET_GENIUS_B = PET_GENIUS_B, 	/* 优秀资质 */
	CLIENT_PET_GENIUS_A = PET_GENIUS_A, 	/* 杰出资质 */
	CLIENT_PET_GENIUS_S = PET_GENIUS_S, 	/* 完美资质 */
};

enum
{
	CLIENT_PET_GENIUS_D_WASH_MONEY	= PET_GENIUS_D_WASH_MONEY,	/* 普通幻化是消耗的钱 */
	CLIENT_PET_GENIUS_C_WASH_MONEY	= PET_GENIUS_C_WASH_MONEY,	/* 良好幻化是消耗的钱 */
	CLIENT_PET_GENIUS_B_WASH_MONEY	= PET_GENIUS_B_WASH_MONEY,	/* 优秀幻化是消耗的钱 */
	CLIENT_PET_GENIUS_A_WASH_MONEY	= PET_GENIUS_A_WASH_MONEY,	/* 杰出幻化是消耗的钱 */
	CLIENT_PET_GENIUS_S_WASH_MONEY	= PET_GENIUS_S_WASH_MONEY,	/* 完美幻化是消耗的钱 */
};
enum
{
	CLIENT_PET_GENIUS_D_UNBIND_MONEY	= PET_GENIUS_D_UNBIND_MONEY,	/* 解绑资质为普通的宠物消耗的银币值 */
	CLIENT_PET_GENIUS_C_UNBIND_MONEY	= PET_GENIUS_C_UNBIND_MONEY,	/* 解绑资质为良好的宠物消耗的银币值 */
	CLIENT_PET_GENIUS_B_UNBIND_MONEY	= PET_GENIUS_B_UNBIND_MONEY,	/* 解绑资质为优秀的宠物消耗的银币值 */
	CLIENT_PET_GENIUS_A_UNBIND_MONEY	= PET_GENIUS_A_UNBIND_MONEY,	/* 解绑资质为杰出的宠物消耗的银币值 */
	CLIENT_PET_GENIUS_S_UNBIND_MONEY	= PET_GENIUS_S_UNBIND_MONEY,	/* 解绑资质为完美的宠物消耗的银币值 */
};

enum
{
	CLIENT_MIN_RONG_HE_LV = 50,
};

enum
{
	CLIENT_PET_FUSE_FAIL	= 0,	// 融合失败
	CLIENT_PET_FUSE_SUCCESS	= 1,	// 融合成功
};

enum
{
	CLIENT_MAX_RONG_HE_DIST = 10, // 以米为单位
	CLIENT_MAX_RONG_HE_TIME = 5 * 60,
	CLIENT_RESTREN_ITEMID	= RESTREN_ITEMID,
};

// 获取宠物静态信息
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
	// 准备出征
	void petPre( int nHightId, int nLowId );
	// 召回
	void petBackUp( int nHightId, int nLowId );
	// 放生
	void petFreedom( int nHightId, int nLowId );
	// 放生
	void petFreedom( tdr_ulonglong petWID );
	// 设置宠物出征战斗模式
	void setPetFightMode( int nMode,int nHightId, int nLowId );
	
	// 宠物技能学习
	void petSkillLearn( int nHightId, int nLowId, int nGridIdx, Item& item );
	// 宠物技能升级
	void petSkillLevelUp( int nHightId, int nLowId, int nSkillId, int nSkillLv, int nGridIdx, Item& item );
	// 宠物洗技能格子
	void petWashSkillGrid( int nHightId, int nLowId, Item& item, int nGridIdx );
	bool isPetStrthenItem( int nItemID );
	// 宠物强化
	bool petStrengthen( int nHightId, int nLowId, Item& stuffItem, Item* safeItem, int num, int nStrthenLevel );
	// 宠物星级重新强化
	void petResetStrengthen( int nHightId, int nLowId, int nStrthenLevel );
	// 保存宠物强化数据
	void petSaveStrengthen( int nHightId, int nLowId, int nVal, int nStrthenLevel );
	// 观察他人宠物
	void petWatch( int nMemId );
	// 是否可以修改宠物昵称
	bool canChangePetName( int nHightId, int nLowId );
	// 修改宠物名
	void petChangeName( int nHightId, int nLowId, const char* szName );
	//设置侍卫实体数据
	void setPetActor( int nXieDaiPos, GameActor* pActor );
	// 获得侍卫实体
	GameActor* getPetActor( int nXieDaiPos );
	// 获取处于出征状态的宠物实体
	GameActor* getPetActor( tdr_ulonglong WID );
	// 召唤回所有的宠物
	void callBackAllPets();
	// 开启宠物技能格子
	void addPetSkillGrid( int nHeightId, int nLowId );
	// 添加要放生的宠物
	void addFreedomPet( int nHeightId, int nLowId );
	// 宠物是否正在放生中
	bool isPetInFreedom( int nHeightId, int nLowId );
	// 获得某个强化等级的提升值
	int	getStrthenVal( tagRolePet_ToLua* pOnePetInfo, int nIndex );
	// 锁定宠物技能
	void LockPetSkill(int high, int low, int idx, bool usecoin);
	void UnLockPetSkill(int high, int low, int idx);
	// 重置宠物技能
	void ResetPetSkill(int high, int low);
	// 升级技能
	void LevelUpPetSkill(int high, int low, int idx);
	// 宠物幻化
	void ResetPetGrow(int high, int low, int lock, bool usecoin);
	void ResetPetGrow(int high, int low);
	void ResetPetGrowSave(int high, int low);
	void petFreeHeti(int high, int low);
	void loveLevelUp( int high, int low );
	//tolua_end

	// S->C
	// 注册网络消息
	void initPetNetMessage();
	void releasePetNetMessage();
	// 网络协议处理
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	// 初始化宠物栏位
	void initPetPackage( tagPetPackage* petInfo );
	// 宠物栏位数变化
	void setPetPackageGridInfo( int nMaxNum );

private:
	// 设置宠物属性
	void setPetAttr( ROLEPET& petAttr, CSPETATTRCHG* data );

public:
	// 设置拥有者
	void setOwner( GameActor* pOwner );
	// TODO:
	
	// 设置侍卫模型出征
	void setShiModle( int nOwnerId, int nXieDaiPos, int nPetId, const char* szPetName, 
		int nPower, int nMaxPower, int nLevel, tdr_ulonglong PetWID, int nOpCode, 
		int nPetFightMode = -1, int nDiathesisNum = 0, 
		const tagCSPetDiathesisView* pPetDiathesises = NULL,
		int nGoodNum = 0, const CSROLESTATUS* pGoodList = NULL, int nGeniusType = -1, 
		int nStrengthenLevel = -1, int nBattleFlag = 0 );
	// TODO: 新重构好的接口验证下功能再替换老接口
	// 改变宠物出征模式或者名字
	void changePetNameOrFightMode( const CSPETVIEW& fightPetServeData );
	// 收回宠物
	void callBackPet( const CSPETVIEW& fightPetServeData );
	// 重新显示侍卫模型
	void reShowModle();
	// 设置宠物位置
	void setPetPostion( GameActor* pPetActor );
	void setPetPostion( int nXieDaiPos );
	// 设置宠物合体
	void setPetHeti(tdr_ulonglong WID,int nPetGenius, bool heti, int nBattleFlag = 0 );
	void onPetHeti(GameActor* pPetActor, bool heti);
	void onPetHeti(bool heti);

	// 设置宠物移动位置
	void setMoveToPoint( int to_x, int to_z );
	// 宠物移动
	void moveTo( int nXiDaiPos, int to_x, int to_z );
	// 刷新宠物移动速度
	void updatePetMoveSpe();
	//
	void FreedomPetOnLeaveMap();
	// 刷新放生宠物动画
	void UpdateFreedomPetAnim( unsigned int dtick );
	void UpdateFreedomPetPopRich( FollowCamera *pcamera );

	void SetGeniusInfo( GameActor* pFightPet, int GeniusId, int GeniusLv );

	//tolua_begin
	// 获取玩家身上的宠物数量
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

	// 删除宠物的时候更新索引信息
	void updatePetIndexDelPet( tdr_ulonglong delPetWid );
	// 获取宠物信息
	tagRolePet_ToLua* getPetInfo( int nIndex );
	// 获取宠物信息
	tagRolePet_ToLua* getPetInfo( tdr_ulonglong WID );
	// 获取宠物信息
	tagRolePet_ToLua* getPetInfo( int nHeightId, int nLowId );
	// 通过实体获取宠物信息
	tagRolePet_ToLua* getPetInfo(GameActor *pActor);
	// 获取宠物在服务器上对应索引值
	int	getPetIndex( tagRolePet_ToLua* pPetInfo );
	// 获取被观察宠物信息
	tagRolePet_ToLua* getWatchPetInfo();
	// 获取宠物静态信息
	tagPetDef_ToLua* getPetDef( int nId );
	// 判断宠物的状态
	bool isPetInState( int nIndex, int nState );
	// 判断宠物的状态
	bool isPetInState( int nHeightId, int nLowId, int nState );
	// 判断宠物的状态
	bool isPetInState( tagPetId_ToLua& WID, int nState );
	// 判断宠物的状态
	bool isPetInState( tdr_ulonglong WID, int nState );
	// 判断宠物的是否使用金币
	bool isPetResetSkillLockUseCoin(tagRolePet_ToLua *info);
	// 获取宠物锁了哪些技能格
	int getPetResetSkillLockNum(tagRolePet_ToLua *info);
	int getPetResetSkillLockIdx(tagRolePet_ToLua *info, int idx);
	// 获取宠物技能重置的消耗
	int getPetResetSkillCostMoney();
	int getPetResetSkillCostMoney(int num, bool usecoin);
	// 判断宠物技能能否升级
	bool isPetSkillCanLevelUp(int id, int level);
	// 获取宠物技能升级的消耗
	int getPetSkillLevelUpCostItemCount(int id, int level);
	// 获取宠物幻化的消耗
	int getPetGrowResetCostMoney(int high, int low);
	int getPetGrowResetCostMoney(int high, int low, int locknum, bool usecoin);
	// 获取宠物当前的资质值
	PetGrowValue getPetGrowValue(tagRolePet_ToLua *info);
	PetGrowValue getPetGrowValue(int high, int low);
	// 获取宠物资质的范围和极限
	PetGrowRange getPetGrowResetRange(int id,int nImp);
	PetGrowRange getPetGrowResetRange(tagRolePet_ToLua *info);
	PetGrowRange getPetBlinkMinRange( int nGrade, int nPetID, int nImp );
	PetGrowRange getPetGrowResetLimit(int id, int nImp);
	PetGrowRange getPetGrowResetLimit(tagRolePet_ToLua *info);
	//  判断是否有空余栏位存放宠物
	bool isPetFull();
	// 判断是否能够出征宠物
	bool canCampaignPet(int nHightId, int nLowId );
	// 查看是否处于吟唱状态
	bool isPre();
	// 获取宠物合体
	int getPetHeti();
	// 获取出征宠物实体
	GameActor* getFightPet( int nIndex )
	{
		return m_pShiActor[nIndex];
	}
	//根据宠物资质设置特效
	void setPetHetiEntity( GameActor* pOwner,int nPetGenius );
	//设置战宠特效
	void setBattlePetEntity( GameActor* pOwner,int nPetGenius );
	static bool isPetStrenthenItem( int nStrengthenLvel, int nItemID );

	// 邀请融合
	void inviteFuse( int nDestPetMemID );
	// 同意融合
	void acceptFuseInvite( int nDestPlayerMemID );
	// 拒绝融合
	void rejectFuse( int nDestPlayerMemID );
	// 取消融合
	void cancelFuse( int nDestPlayerMemID );
	// 同意融合
	void agreeFuse( int nDestPlayerMemID );
	// 进行吞噬
	void startEatPet( int nSrcHightId, int nSrcLowId ,int nDesHightId, int nDesLowId );
	// 进行转星
	void startMoveStar( int nSrcHightId, int nSrcLowId ,int nDesHightId, int nDesLowId );

	const tagCSPetFuseInviteInfo_ToLua& getPetFuseInviteInfo();
	const tagCSPetFuseUI_ToLua&			getRongHePlayerInfo();
	tagPetId_ToLua& getSelfRongHePetWID();
	void			clearSelfRongHePetInfo();
	tagCSPetFuseBaby_ToLua& getSuccessFuseInfo();
	//tolua_end

private:
	// 收到宠物信息变化回复
	void	OnGetPetInfoChg( const ROLEPET& newPetInfo );
	void	OnGetNewPet( const ROLEPET& newPetInfo );
	void	OnUpdatePetInfo( ROLEPET& oldPetInfo, const ROLEPET& newPetInfo );
	// 收到宠物强化回复
	void	OnGetPetStrengthenInfo( const CSPETSVRSTRENGTHEN& petStrengthenInfo );
	// 保存融合的宠物WID
	void	SaveRongHePetInfo();
	// 收到邀请融合
	void	OnGetFuseInvite( const CSPETFUSEINVITEINFO& petFuseInviteRes );

	// 收到拒绝融合
	void	OnGetRejectFuse( const CSPETFUSEREJECT& rejectFuseRes );

	// 收到取消融合
	void	OnGetCancelFuse( const CSPETFUSECANCEL& cancelFuseRes );
	
	// 收到同意融合
	void	OnGetDealFuse( const CSPETFUSEDEAL& dealFuseRes );
	
	// 收到对方同意融合邀请的消息
	void	OnGetOtherAcceptInviteRes( const CSPETFUSEUI& fuseUIRes );

	// 收到标志融合是否成功
	void	OnGetFuseResultRes( const CSPETFUSESUCC& fuseResultRes );
	// 设置出征宠物的属性
	void setFightPetAttr( const CSPETVIEW& fightPetServeData );
	// 设置出征宠物的战斗模式
	void setFightPetFightMode( GameActor* pFightPet );
	// 宠物移到某个位置
	void clientSelfPetMove( GameActor* pFightPet );
	void generateRandomMovePoint( int& nDestX, int& nDestY, int nCurPosX, int nCurPosY, int nRangeIndex );
	bool tryMove( GameActor* pFightPet, int nDestX, int nDestY, int nCurX, int nCurY );
	// 强化属性提升值是否变化
	bool isStrthenValChg( const char* pszLeftVals, const char* pszRighVals );
	int  getChgStrthenLevel( const char* pszLeftVals, const char* pszRighVals );
	// 是否是重置强化星级
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
	GameActor*		m_pShiActor[PET_MAX_FIGHT_SHI];			// 侍卫模型
	unsigned int	m_lastFree;

	tagCSPetFuseInviteInfo_ToLua	m_fuseInviteInfo;
	tagCSPetFuseUI_ToLua			m_rongHePlayerInfo;
	tagPetId_ToLua					m_selfRongHePetWID;
	static tagCSPetFuseBaby_ToLua	m_successRongHeInfo;

	std::map<GameActor*, tagFreedomPetInfo>		m_freedomPets;
	static std::vector<std::string>				m_vecPetFreedomText;
};//tolua_export