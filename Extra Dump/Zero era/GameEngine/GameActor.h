#pragma once
#include "GameDBData.h"
#include "GameEngineTypes.h"
#include "GameStateAction.h"
#include "GameActor_Attr.h"
#include "GameBufStatus.h"
#include "NetMessage.h"
#include "OgreWorldPos.h"
#include "Equip.h"
#include "Container.h"
#include "AvatarEntity.h"
#include "GameActor_RoleText.h"
#include "OgreSceneRenderer.h"
#include "GameActor_Move.h"
#include "GameActor_Rider.h"
#include "GameActor_Pet.h"
#include "GameActorManager.h"
#include "GameQiYue.h"
#include "GameActor_Trump.h"
#include "GameCBT.h"
#include <string>
#include <map>
#include <vector>
#include "GameActor_Carry.h"
#include "GameActor_MonsterMachine.h"
#include "OgreSmallDecal.h"

class SpecialMove;
class GameActor_Tick_InFightTime;
class GameActor_Tick_MainPlayerFree;
class IChant_Base;
enum
{
	ATTACK_LOCK_NONE = 0,	// 无锁定
	ATTACK_LOCK_OBJ,		// 锁定目标
	ATTACK_LOCK_DIR,		// 锁定朝向
};

enum
{
	DEAD_STATE_DECAY,
	DEAD_STATE_DECENCY,
	DEAD_STATE_FADE,
	DEAD_STATE_SOUL,
};

enum
{
	CLAN_OTHER,
	CLAN_ENEMY,
	CLAN_SAME,
	CLAN_ALLY,
};

enum//	四方向跑动
{
	RUN_NONE,

	RUN_BEFORE,
	RUN_BACK,
	RUN_LEFT,
	RUN_RIGHT,
};

struct tagAttackLock
{
	int m_nAttackLockType;
	int m_nLockValue;
};

enum{
	BACK_CURPOST	= 0,
	BACK_LINE,
	BACK_SIN,
	BACK_CIRCLE,
	BACK_PARABOLA,

	FLY_HIGHT		= 8000,
	FLY_TIME		= 1500,
};

enum{
	MONSTER_UNSEEYOU	= 1,	// 不会朝向你的NPC
	MONSTER_MACTION		= 2,	// 临时器械变身
	MONSTER_RIDE		= 4,	// 临时器械骑乘
	MONSTER_FUNCTION	= 8,	// 功能
	MONSTER_QUEST		= 16,	// 任务
	MONSTER_ACT			= 32,	// 活动
	MONSTER_TRAN		= 64,	// 传送
	MONSTER_OTHER		= 128,	// 其他
	MONSTER_PET			= 256,	//宠物
	MONSTER_SKILLNPC	= 512,	//技能NPC
	MONSTER_SHOPNPC		= 1024,	//商店NPC
};

const std::string EffectPath[] = 
{
	"effect\\entity\\9615_16_1.ent",
	"effect\\entity\\9615_16_3.ent",
	"effect\\entity\\9615_16_2.ent",
};

enum
{
	EFFECT_STATE_START = 0,
	EFFECT_STATE_LIVE,
	EFFECT_STATE_END,
	EFFECT_STATE_MAX
};
//完美九星
#define  PERFECT_STAR_NINE  9

class GameActor_PKEffect
{
public:
	Ogre::Entity*	m_pEntity;
	int	m_nX;
	int m_nZ;
	int m_nState;
	int m_nVictory;
	GameActor*	m_pOwner;
	GameActor_PKEffect()
	{
		m_pEntity = NULL;
		m_nX = 0;
		m_nZ = 0;
		m_nState = 0;
		m_nVictory = -1;
		m_pOwner = NULL;
	}
	~GameActor_PKEffect();
	void	onUpdate(unsigned int dtick);
	void	setState(int nState);
	void	setPos(int x,int z);
	void	setVictory(int v){m_nVictory = v;}
};


//tolua_begin
#pragma pack( 1 )
struct tagNpcDisplayInfo_ToLua
{
	unsigned int id;                             
	char name[RES_NAME_LEN];             
	char model[RES_PATH_LEN];            
	float scale;                          
	float alpha;                          
	char skin1[RES_PATH_LEN];            
	char skin2[RES_PATH_LEN];            
	char skin3[RES_PATH_LEN];            
	unsigned int sound;                          
	int race;                           
	char gender;                         
	unsigned char deadstate;                       	/*  Ver.74 死亡状态 需要增加怪物死亡消失的几种方式（立即消失-采集品）0立即消失，1延迟消失 */
	unsigned char normalrot;                       	/*  Ver.74 怪物npc需要有一个标记表示它会跟坐骑一样随地形起伏（鳄鱼）0不随地形起伏，1随地形起伏 */
	unsigned int userdata[24];                   
	unsigned int radius;                         
	unsigned int height;                         
	float size;                            	/*  Ver.159  */
	int rideact;                         	/*  Ver.103  */
	char visuals[RES_PATH_LEN];          
	char npcdesc[RES_NAME_LEN];           	/*  Ver.58 简要功能信息 */
	char effect[RES_EFFECT_LEN];          	/*  Ver.404 表现调整 */
	int ZHigh;                           	/*  Ver.525 地图上的z轴，表示模型离地面的高度 */
	unsigned int MiscType;                        	/*  Ver.532 位操作类型 参考NPCDISPLAYINFO_MISC_TYPE Bind Macrosgroup:NPCDISPLAYINFO_MISC_TYPE,*/
	int PoseType;                        	/*  Ver.675 monster pose */
	int passengeract;                    	/*  Ver.809 乘客动作 */
	char socket[RES_PATH_LEN];            	/*  Ver.1085 分离的底层模型 */
	int dieeffect;                       	/*  Ver.1262 死亡特效，不使用默认的死亡特效需要填这个 */
	int birth;                           	/*  Ver.1490 出生特效 */
};

struct tagLevelUP_ToLua
{
	unsigned int Level;                          
	unsigned int LevelNextExp;                   
	unsigned int MoneyLimit;                     
	unsigned char LevelStand;                      	/*   是否是等级停留点 */
	unsigned int ContributeExp;                   	/*  Ver.62 馈赠奖励经验 */
	unsigned int ContributeMoney;                 	/*  Ver.111 馈赠奖励金钱 */
	unsigned short SitAddMp;                        	/*  Ver.83 打坐增加MP */
	unsigned short SitAddHp;                        	/*  Ver.83 打坐增加HP */
	int WarriorSkillList[MAX_LEVELUP_SKILL_NUM]; 	/*  Ver.84  */
	int MagicorSkillList[MAX_LEVELUP_SKILL_NUM]; 	/*  Ver.84  */
	int GunerSkillList[MAX_LEVELUP_SKILL_NUM]; 	/*  Ver.84  */
	int ChurchSkillList[MAX_LEVELUP_SKILL_NUM]; 	/*  Ver.84  */
	unsigned int DayExpLimit;                     	/*  Ver.85 每天获得exp上限 */
	unsigned short WarriorLiliang;                  	/*  Ver.128 力量 */
	unsigned short MagicorLiliang;                  	/*  Ver.128  */
	unsigned short GunerLiliang;                    	/*  Ver.128  */
	unsigned short ChurchLiliang;                   	/*  Ver.128  */
	unsigned short WarriorZhili;                    	/*  Ver.128 智力 */
	unsigned short MagicorZhili;                    	/*  Ver.128  */
	unsigned short GunerZhili;                      	/*  Ver.128  */
	unsigned short ChurchZhili;                     	/*  Ver.128  */
	unsigned short WarriorTili;                     	/*  Ver.128 体力 */
	unsigned short MagicorTili;                     	/*  Ver.128  */
	unsigned short GunerTili;                       	/*  Ver.128  */
	unsigned short ChurchTili;                      	/*  Ver.128  */
	unsigned short WarriorJingshen;                 	/*  Ver.128 精神 */
	unsigned short MagicorJingshen;                 	/*  Ver.128  */
	unsigned short GunerJingshen;                   	/*  Ver.128  */
	unsigned short ChurchJingshen;                  	/*  Ver.128  */
	unsigned short WarriorFangyu;                   	/*  Ver.128 防御 */
	unsigned short MagicorFangyu;                   	/*  Ver.128  */
	unsigned short GunerFangyu;                     	/*  Ver.128  */
	unsigned short ChurchFangyu;                    	/*  Ver.128  */
	unsigned short VimAdd;                          	/*  Ver.134  */
	unsigned short VimMax;                          	/*  Ver.134  */
	unsigned char AddRolePetGrid;                  	/*  Ver.144  */
	int MaxOfflineExp;                   	/*  Ver.567 最大离线补偿经验 */
	unsigned short WarriorAgile;                    	/*  Ver.426 敏捷 */
	unsigned short MagicorAgile;                    	/*  Ver.426  */
	unsigned short GunerAgile;                      	/*  Ver.426  */
	unsigned short ChurchAgile;                     	/*  Ver.426  */
	unsigned short AtkLevelMul;                     	/*  Ver.426 攻击力乘修正 */
	unsigned short DefLevelMul;                     	/*  Ver.426 防御力乘修正 */
	int MonDefHeavyHit;                  	/*  Ver.426 怪物暴击抵抗 */
	unsigned short SwordWarriorLiliang;             	/*  Ver.431 力量 */
	unsigned short FireMagicorLiliang;              	/*  Ver.431  */
	unsigned short DoubleGunerLiliang;              	/*  Ver.431  */
	unsigned short ThunderChurchLiliang;            	/*  Ver.431  */
	unsigned short SwordWarriorZhili;               	/*  Ver.431 智力 */
	unsigned short FireMagicorZhili;                	/*  Ver.431  */
	unsigned short DoubleGunerZhili;                	/*  Ver.431  */
	unsigned short ThunderChurchZhili;              	/*  Ver.431  */
	unsigned short SwordWarriorTili;                	/*  Ver.431 体力 */
	unsigned short FireMagicorTili;                 	/*  Ver.431  */
	unsigned short DoubleGunerTili;                 	/*  Ver.431  */
	unsigned short ThunderChurchTili;               	/*  Ver.431  */
	unsigned short SwordWarriorJingshen;            	/*  Ver.431 精神 */
	unsigned short FireMagicorJingshen;             	/*  Ver.431  */
	unsigned short DoubleGunerJingshen;             	/*  Ver.431  */
	unsigned short ThunderChurchJingshen;           	/*  Ver.431  */
	unsigned short SwordWarriorFangyu;              	/*  Ver.431 防御 */
	unsigned short FireMagicorFangyu;               	/*  Ver.431  */
	unsigned short DoubleGunerFangyu;               	/*  Ver.431  */
	unsigned short ThunderChurchFangyu;             	/*  Ver.431  */
	unsigned short SwordWarriorAgile;               	/*  Ver.431 敏捷 */
	unsigned short FireMagicorAgile;                	/*  Ver.431  */
	unsigned short DoubleGunerAgile;                	/*  Ver.431  */
	unsigned short ThunderChurchAgile;              	/*  Ver.431  */
	int UIBaseFlee;                      	/*  Ver.470 参考命中值 */
	int BattleWinExp;                    	/*  Ver.909 战场胜利经验 */
	int BattleFailExp;                   	/*  Ver.909 战场失败经验 */
	int MaxPworldOffExp;                 	/*  Ver.911 活动副本每日未参加的补偿经验上限 */
	int MonLevelHit;                     	/*  Ver.1059 怪物等级参考闪避力 */
	char AddSparPkg;                      	/*  Ver.1243 是否增加晶石背包格子 */
	int VigUp;                           	/*  Ver.1378 角色等级活力上限 */
	int MaxStashMoney;                   	/*  Ver.1407 角色携带银币上限 */
	int MaxPackageMoney;                 	/*  Ver.1407 角色携带银币上限 */
	int DailyUseBull;                    	/*  Ver.1407 角色每日使用绑银上限 */
	int FzMod;                           	/*  Ver.1769 升级需要的方舟系数 */
	int ExpLeiji;                        	/*  Ver.1811 经验累积倍数，0表示不限制，例如59级 */
	unsigned int DayZhengyiLimit;           /*  Ver.2169 每天获得正义点上限 */
	int BaseGenius;                      	/*  Ver.2172 封印等级对应的基础天赋点 */
	unsigned short GreenGemLimit;           /*  Ver.2178 每天获得生存奖章上限 */
	int RemainTm;                        	/*  Ver.2391 单位天,超过该等级停留的最大天数限制进入特定战场 */
};

struct tagAtkerHurtChg_ToLua
{
	short AtkerIceHurtMul;                 	/*   魔法伤害修正(主属性) */
	short AtkerPhsicHurtMul;               	/*   物理属性伤害修正(主属性) */
	short AtkerSiegeHurtMul;               	/*  Ver.419 攻城设施伤害修正(主属性) */
	short AtkerHolyHurtMul;                	/*  Ver.419 神圣伤害修正(主属性) */
	short AtkerMachineHurtMul;             	/*  Ver.419 机械伤害修正(主属性) */
	short AtkerFireHurtMul;                	/*   火属性伤害修正(元素属性) */
	short AtkerThunderHurtMul;             	/*   雷属性伤害修正(元素属性) */
	short AtkerLightHurtMul;               	/*   光属性伤害修正(元素属性) */
	short AtkerNightHurtMul;               	/*   暗属性伤害修正(元素属性) */
	short AtkerWaterHurtMul;               	/*  Ver.419 水属性伤害修正(元素属性) */
	short AtkerMagicHurtMul;               	/*   魔法伤害修正 */
	short AtkerHurtMul;                    	/*   主属性伤害修正 */
	int AtkerPhsicHurtAdd;               	/*   物理属性伤害修正(主属性) */
	int AtkerIceHurtAdd;                 	/*   魔法伤害修正(主属性) */
	int AtkerSiegeHurtAdd;               	/*  Ver.419 攻城设施伤害修正(主属性) */
	int AtkerHolyHurtAdd;                	/*  Ver.419 神圣伤害修正(主属性) */
	int AtkerMachineHurtAdd;             	/*  Ver.419 机械伤害修正(主属性) */
	int AtkerFireHurtAdd;                	/*   火属性伤害修正(元素属性) */
	int AtkerThunderHurtAdd;             	/*   雷属性伤害修正(元素属性) */
	int AtkerLightHurtAdd;               	/*   光属性伤害修正(元素属性) */
	int AtkerNightHurtAdd;               	/*   暗属性伤害修正(元素属性) */
	int AtkerWaterHurtAdd;               	/*  Ver.419 水属性伤害修正(元素属性) */
	int AtkerMagicHurtAdd;               	/*   魔法伤害修正 */
	int AtkerHurtAdd;                    	/*   主属性伤害修正 */
};

struct tagAtkeeHurtChg_ToLua
{
	short AtkeePhsicHurtMul;               	/*   抵抗物理属性伤害修正(主属性) */
	short AtkeeIceHurtMul;                 	/*   抵抗冰属性伤害修正(主属性) */
	int AtkeeSiegeHurtMul;               	/*  Ver.419 抵抗攻城设施伤害修正(主属性) */
	int AtkeeHolyHurtMul;                	/*  Ver.419 抵抗神圣伤害修正(主属性) */
	int AtkeeMachineHurtMul;             	/*  Ver.419 抵抗机械伤害修正(主属性) */
	short AtkeeFireHurtMul;                	/*   抵抗火属性伤害修正(元素属性) */
	short AtkeeThunderHurtMul;             	/*   抵抗雷属性伤害修正(元素属性) */
	short AtkeeLightHurtMul;               	/*   抵抗光属性伤害修正(元素属性) */
	short AtkeeNightHurtMul;               	/*   抵抗暗属性伤害修正(元素属性) */
	int AtkeeWaterHurtMul;               	/*  Ver.419 抵抗水属性伤害修正(元素属性) */
	short AtkeeMagicHurtMul;               	/*   抵抗魔法伤害修正 */
	short AtkeeHurtMul;                    	/*   抵抗主属性伤害修正 */
	int AtkeePhsicHurtAdd;               	/*   抵抗物理属性伤害修正(主属性) */
	int AtkeeIceHurtAdd;                 	/*   抵抗魔法伤害修正(主属性) */
	int AtkeeSiegeHurtAdd;               	/*  Ver.419 抵抗攻城设施伤害修正(主属性) */
	int AtkeeHolyHurtAdd;                	/*  Ver.419 抵抗神圣伤害修正(主属性) */
	int AtkeeMachineHurtAdd;             	/*  Ver.419 抵抗机械伤害修正(主属性) */
	int AtkeeFireHurtAdd;                	/*   抵抗火属性伤害修正(元素属性) */
	int AtkeeThunderHurtAdd;             	/*   抵抗雷属性伤害修正(元素属性) */
	int AtkeeLightHurtAdd;               	/*   抵抗光属性伤害修正(元素属性) */
	int AtkeeNightHurtAdd;               	/*   抵抗暗属性伤害修正(元素属性) */
	int AtkeeWaterHurtAdd;               	/*  Ver.419 抵抗水属性伤害修正(元素属性) */
	int AtkeeMagicHurtAdd;               	/*   抵抗魔法伤害修正 */
	int AtkeeHurtAdd;                    	/*   抵抗主属性伤害修正 */
};

struct tagPressInit_ToLua
{
	unsigned char PressInitType;                   	/*   压力初值类型 Bind Macrosgroup:PRESS_INIT_TYPE,*/
	unsigned char Val1;                            	/*   职业或血量百分比 */
	unsigned short Val2;                            	/*   压力初值 */
};

struct tagAINextSkill_ToLua
{
	unsigned short SkillID;                         	/*   技能ID */
	unsigned short SkillLevel;                      	/*   技能等级 */
};

struct tagAINormalSkill_ToLua
{
	unsigned short SkillID;                         	/*   技能ID */
	unsigned char SkillLevel;                      	/*   技能等级 */
	int ConditionID;                     	/*  Ver.501 插入条件ID Bind Macrosgroup:INSERT_SKILL_CONDITION,*/
	int ConditionVal;                    	/*  Ver.501 插入条件值 */
	unsigned char Rate;                            	/*   概率 */
	short Once;                            	/*  Ver.501 是否只触发一次 */
	tagAINextSkill_ToLua NextSkill[MAX_NEXT_SKILL];       	/*  Ver.367 后续技能 */
};

struct tagAIInsertSkill_ToLua
{
	unsigned short SkillID;                         	/*   技能ID */
	unsigned short SkillLevel;                      	/*   技能等级 */
	int ConditionID;                     	/*   插入条件ID Bind Macrosgroup:INSERT_SKILL_CONDITION,*/
	int ConditionVal;                    	/*   插入条件值 */
	short Rate;                            	/*   概率 */
	short Once;                            	/*   是否只触发一次 */
};

struct tagAIRefreshedSkill_ToLua
{
	unsigned short SkillID;                         	/*   技能ID */
	unsigned short SkillLevel;                      	/*   技能等级 */
	short Rate;                            	/*   概率 */
	char Self;                            	/*  Ver.354 是不是对自己使用 */
};

struct tagMonBootyMoney_ToLua
{
	unsigned short MoneyAttrID;                    
	unsigned short Rate;                           
	unsigned int MoneyVal;                       
};

struct tagMonBootyItem_ToLua
{
	unsigned int ItemID;                         
	unsigned int Rate;                            	/*   概率 */
	unsigned char TeamBootyType;                   	/*   组队时的掉落方式 Bind Macrosgroup:MON_BOOTY_TEAM_TYPE,*/
	unsigned short Num;                             	/*   数量 */
	unsigned char DirectPakage;                    	/*   是否直接放进背包 */
	unsigned char MinLevel;                        	/*   获得此物品的最小等级，0表示不限制 */
	unsigned char MaxLevel;                        	/*   获得此物品的最大等级，0表示不限制 */
	unsigned char Career;                          	/*   职业，0表示不限制 */
	unsigned short TaskID1;                        
	unsigned short TaskID2;                        
};

struct tagMonBooty_ToLua
{
	unsigned int Exp;                             	/*   经验值 */
	tagMonBootyMoney_ToLua Moneys[MAX_MONBOOTY_MONEY];      	/*   货币 */
	tagMonBootyItem_ToLua Items[MAX_MONBOOTY_ITEM];        	/*   物品 */
	unsigned int BootyPetEgg;                     	/*  Ver.88 获得宠物蛋 */
	int BootyPetEggRate;                 	/*  Ver.88 获得宠物蛋概率万分率 */
	tagRespactEntry_ToLua Respact;                         	/*  Ver.126 获得的声望 */
};

struct tagMonColorLevel_ToLua
{
	unsigned char BlackLevel;                      	/*   小于这个等级为黑名 */
	unsigned char RedLevel;                        	/*   小于这个等级，大于等于BlackLevel为红名 */
	unsigned char WhiteLevel;                      	/*   小于这个等级，大于等于RedLevel为白名 */
	unsigned char GreenLevel;                      	/*   小于这个等级，大于等于WhiteLevel为绿名，大于等于这个等级为灰名 */
};

struct tagMonCollect_ToLua
{
	unsigned short TaskID[MON_COLLECT_TASK_NUM];   
	unsigned short CollectOnce;                     	/*   是否是单次采集 */
	unsigned short CollectTime;                     	/*   采集一次需要多长时间，单位秒 */
	unsigned short CollectArmDur;                   	/*   采集一次消耗采集工具的耐久 */
	unsigned int CollectSkillID;                  	/*   需要的采集技能 */
	unsigned int CollectArm;                      	/*   需要的采集工具 */
	unsigned int CollectItem;                     	/*  Ver.61 需要的采集物品 */
	unsigned short CollectItemDur;                  	/*  Ver.61 采集一次消耗物品的数量 */
	unsigned short CollectSecondarySkillType;       	/*  Ver.299 采集需要生活技能 */
	unsigned short CollectRecipeID;                 	/*  Ver.299 采集需要生活技能配方 */
	int NoDecHP;                         	/*  Ver.319 采集物采集不减血，一直可以采。 */
	int CollectType;                     	/*  Ver.400 0 普通采集物，1 机械才可以采集的物品 Bind Macrosgroup:MON_COLLECT_TYPE,*/
	unsigned int CollectPackgeItem[MAX_MON_COLLECT_PACKAGE_ITEM]; 	/*  Ver.455 需要背包中有某个物品方可采集 */
	int CollectFlag;                     	/*  Ver.1246 0 人可以采集，1 人不可以采集的采集物 */
};

struct tagAberrantList_ToLua
{
	unsigned int MonsterID;                      
	unsigned short Rate;                           
};

struct tagAddMonBootyGroup_ToLua
{
	int GroupID;                        
	int CollectTime;                     	/*   采集时间 0 则直接拾取 */
	int Flag;                            	/*   掉落包标记按位或: 1 采集分配的时候只归采集者所有 */
};

struct tagMonsterDef_ToLua
{
	unsigned int MonsterID;                      
	char Name[RES_NAME_LEN];             
	int IconID;                          	/*  Ver.6 图标id */
	unsigned int HP;                             
	unsigned int IceAtk;                          	/*   冰属性攻击力 */
	unsigned int IceDef;                          	/*   冰属性防御力 */
	unsigned int FireAtk;                         	/*   火属性攻击力 */
	unsigned int FireDef;                         	/*   火属性防御力 */
	unsigned int ThunderAtk;                      	/*   雷属性攻击力 */
	unsigned int ThunderDef;                      	/*   雷属性防御力 */
	unsigned int LightAtk;                        	/*   光属性攻击力 */
	unsigned int LightDef;                        	/*   光属性防御力 */
	unsigned int NightAtk;                        	/*   暗属性攻击力 */
	unsigned int NightDef;                        	/*   暗属性防御力 */
	unsigned int PhsicAtk;                        	/*   物理属性攻击力 */
	unsigned int PhsicDef;                        	/*   物理属性防御力 */
	unsigned int WaterAtk;                        	/*  Ver.419 水属性攻击力 */
	unsigned int WaterDef;                        	/*  Ver.419 水属性防御力 */
	unsigned int SiegeAtk;                        	/*  Ver.419 攻城设施攻击力 */
	unsigned int SiegeDef;                        	/*  Ver.419 攻城设施防御力 */
	unsigned int HolyAtk;                         	/*  Ver.419 神圣攻击力 */
	unsigned int HolyDef;                         	/*  Ver.419 神圣防御力 */
	unsigned int MachineAtk;                      	/*  Ver.419 机械攻击力 */
	unsigned int MachineDef;                      	/*  Ver.419 机械防御力 */
	tagAtkerHurtChg_ToLua AtkerHurtChg;                    	/*   伤害修正 */
	tagAtkeeHurtChg_ToLua AtkeeHurtChg;                    	/*   抵抗伤害修正 */
	unsigned int Hit;                             	/*   命中力 */
	unsigned int Flee;                            	/*   闪避力 */
	unsigned char Type;                            	/*   怪物类型 Bind Macrosgroup:MON_TYPE,*/
	char BackMul;                         	/*   震退修正 */
	unsigned char NotRefresh;                      	/*  Ver.15 0 死亡会重刷，1 死亡不重刷 */
	unsigned char DynAttr;                         	/*  Ver.15 1 动态属性 0 非动态属性 */
	unsigned int MoveSpd;                         	/*   巡逻时的移动速度 */
	unsigned int FightMoveSpd;                    	/*  Ver.10 战斗时的移动速度 */
	unsigned int HomeMoveSpd;                     	/*  Ver.10 归位时的移动速度 */
	int RefreshTime;                     	/*   怪物死亡后的重生时间，怪物尸体在这段时间一直存在，单位秒 */
	short DeadAddResourceMul;              	/*   怪物被击杀时增加资源数的修正指数 */
	tagPressInit_ToLua PressInit[MAX_PRESS_INIT];       	/*  Ver.10 压力初值 */
	unsigned int PressHurtRate;                   	/*  Ver.10 伤害转换比例，多少伤害转化为一点压力，0表示不转换 */
	unsigned int PressSelectAdd;                  	/*  Ver.10 选中目标时压力增加值，用于实现目标不切换 */
	tagAINormalSkill_ToLua AINormalSkills[MAX_NORM_SKILL];  	/*  Ver.10 标准技能，对敌方使用 */
	tagAIInsertSkill_ToLua AIAtkeeSkills[MAX_ATKEE_SKILL];  	/*  Ver.10 被攻击时的插入技能，对自己使用 */
	tagAIInsertSkill_ToLua AIAtkerSkills[MAX_ATKER_SKILL];  	/*  Ver.10 攻击时的插入技能，对敌方使用 */
	unsigned int ViewDist;                        	/*  Ver.10 怪物视野，单位厘米 */
	unsigned int TraceDist;                       	/*  Ver.10 怪物追赶距离，单位厘米 */
	unsigned char MoveType;                        	/*  Ver.10 怪物移动类型 Bind Macrosgroup:MON_MOVE_TYPE,*/
	unsigned char AtkType;                         	/*  Ver.10 怪物攻击类型 Bind Macrosgroup:MON_ATK_TYPE,*/
	unsigned char RandSelect;                      	/*  Ver.10 随机切换攻击目标的时间 */
	unsigned char HeavyHitRate;                    	/*  Ver.10 爆击率 */
	unsigned int Radius;                          	/*  Ver.18  */
	unsigned int DynMask;                         	/*  Ver.331 动态阻挡，行走和魔法都被阻挡 */
	unsigned int DynMaskX;                        	/*  Ver.546 动态阻挡，以怪物中心点的长度1/2,如果是0,用的是Radius */
	unsigned int DynMaskY;                        	/*  Ver.546 动态阻挡，以怪物中心点的宽度1/2,如果是0,用的是Radius */
	unsigned int Height;                          	/*  Ver.18  */
	unsigned char Level;                           	/*  Ver.22 怪物等级 */
	unsigned char BirthTime;                       	/*  Ver.25 怪物出生时间,单位秒 */
	unsigned short SubType;                         	/*  Ver.27 怪物子类型 Bind Macrosgroup:MON_SUB_TYPE,*/
	tagMonBooty_ToLua Booty;                           	/*  Ver.22 战利品 */
	tagMonColorLevel_ToLua ColorLevel;                      	/*  Ver.22 怪物名颜色等级信息 */
	tagMonCollect_ToLua Collect;                         	/*  Ver.44 采集 */
	unsigned short RefreshRadius;                   	/*  Ver.44 怪物刷新时的随机半径 */
	unsigned char MonAttrType;                     	/*  Ver.64 怪物属性类型,变异人,生物等 Bind Macrosgroup:MON_ATTR_TYPE,*/
	tagAberrantList_ToLua Aberrants[MAX_MON_ABERRANT];     	/*  Ver.118 一定几率变异成某种怪物 */
	int RunAwayHP;                       	/*  Ver.123 逃跑的血量触发值 */
	unsigned int RunAwayTime;                     	/*  Ver.123 反方向逃跑的时间,单位秒 */
	int CallHelpHP;                      	/*  Ver.123 呼救的血量触发值 */
	int CallHelpDist;                    	/*  Ver.125 可呼救的范围 */
	unsigned int CallHelpMonsterID[MAX_CALL_MOSTER_NUM]; 	/*  Ver.123 呼救过来帮忙的怪物id列表 */
	int HoldDistanceRandom;              	/*  Ver.123 保持距离 远程攻击的怪物有一定的几率触发该怪物后退一段距离，但还在技能攻击的范围之内,万分率 */
	int BootyXp;                         	/*  Ver.223 怪物死亡给角色增加的xp值 */
	int AtkMulByCaller;                  	/*  Ver.304 图腾召唤怪物攻击力的修正 怪物的物理攻击+人物物理攻击*修正, 法术相同 */
	int BootyDropType;                   	/*  Ver.338 战利品掉落方式 0 直接掉落 1 一个一个掉落出来2 从尸体中滚落出来 */
	tagAIRefreshedSkill_ToLua AIRefreshedSkill;                	/*  Ver.353 怪物刷新时释放的技能 */
	unsigned char BootyViewType;                   	/*  Ver.416 掉落可见类型0 队伍可见 1 全部可见 2 自己可见 Bind Macrosgroup:BOOTY_DROP_VIEW_TYPE,*/
	unsigned int MiscType;                        	/*  Ver.532 位操作类型 参考MON_MISC_TYPE Bind Macrosgroup:MON_MISC_TYPE,*/
	char CollectActDesc[MON_COLLECT_DESC_LEN]; 	/*  Ver.598  */
	tagAddMonBootyGroup_ToLua BootyGroup[MAX_MON_BOOTY_GROUP_LIST]; 	/*  Ver.620 怪物战利品列表ID */
	int ShowType;                        	/*  Ver.1124 怪物显示标记0 无 1 魔法怪物2物理怪物3 可捕获的宠物 Bind Macrosgroup:MON_SHOW_TYPE,*/
	int BossType;                        	/*  Ver.1195 暂时支持1-10,表示1-10层boss地图 */
	int Vig;                             	/*  Ver.1378 活力基础值 */
	unsigned char DartCarType;                     	/*  Ver.1451 商运镖车类型|DART_CAR_TYPE */
	short GetRes;                          	/*  Ver.1559 每秒能获取多少资源|据点资源怪 */
	int ClanBooty;                       	/*  Ver.1928 为1则按公会分配战利品的怪物 */
	int Zhengyi;                         	/*  Ver.2169 杀死怪物可以获得多少正义点 */
	char IsJicheng;                       	/*  Ver.2312 怪物机械的话，1-继承任务属性 */
	int ChengshuID;                      	/*  Ver.2493 家园植物成熟后刷的采集怪ID */
};

struct tagLevelFengExpDec_ToLua
{
	int ExpCount;                        	/*   几倍经验值时调整，从1开始，1表示从开始第1条经验就衰减 */
	int ExpRate;                         	/*   经验比率,100表示不衰减，0表示全衰减 */
};

struct tagLevelFeng_ToLua
{
	int Level;                           	/*   封印的等级 */
	int LevelFengMax;                    	/*   等级封印最大时间，单位小时 */
	int LevelFengMin;                    	/*   等级封印最大时间，单位小时 */
	int LevelFengTimeDec;                	/*   每达到一个封印等级减少的时间，单位秒 */
	tagLevelFengExpDec_ToLua ExpRateDec[MAX_LEV_FENG_EXPDEC]; 	/*  Ver.1820 封印等级经验衰减 */
};
#define MAX_LEV_FENG_EXPDEC_TOLUA MAX_LEV_FENG_EXPDEC

#define MAX_CATEGORY_ACHIEVEMENT_TOLUA 8
/* 成长之路分类表 */
struct tagAchievementCategoryDef_ToLua
{
	int CategoryID;                      	/*   类别id */
	int Icon;                            	/*   图标ID */
	char Name[ROLE_NAME_LEN];             	/*   名称 */
	int Level;                           	/*   要求等级 */
	int NextLevel;                       	/*   下个类别的等级 */
	int AchievementID[MAX_CATEGORY_ACHIEVEMENT_TOLUA]; 	/*   子项id */
};

/* 成长之路条件 */
struct tagAchievementCond_ToLua
{
	char Des[ACHIEVEMENT_COND_DES_LEN];   	/*   条件描述名称 */
	int Type;                            	/*   条件类型 */
	int Val1;                            	/*   条件值1 */
	int Val2;                            	/*   条件值2 */
	int Val3;                            	/*   条件值3 */
};

/* 成长之路定义表 */
struct tagAchievementDef_ToLua
{
	int AchievementID;                   	/*   类别id */
	int CategoryID;                      	/*   所属类别id */
	int Icon;                            	/*   图标ID */
	int IconRate;                        	/*   图标缩放比例(100为基数) */
	char Name[64];             	/*   名称 */
	tagAchievementCond_ToLua Cond;                            	/*   达成条件 */
	tagResult_ToLua Result;                          	/*   激活后效果 */
	char GuideDes[ACHIEVEMENT_GUIDE_DES_LEN]; 	/*   攻略描述 */
};

#pragma pack()

#define FLY_ANIM_START_INDEX 100	//飞行动画 开始索引
#define FLY_ANIM_PATH_INDEX  200	//多条飞行动画 开始索引

const tagNpcDisplayInfo_ToLua&	GetNpcDisplayInfo(int id);
const tagLevelUP_ToLua*		getLevelUP( int lv );
extern int getPlayerDisplayId( int gender, int race );
const tagLevelFeng_ToLua* getLevelFeng( int lv );
const tagMonsterDef_ToLua* getMonsterDef( int id );
struct tagPlayerPosInfo
{
	int m_nPosX;	// 这个以m作为单位
	int m_nPosZ;	// 这个以m作为单位
};

const char* GetModePathById(int nModelId);
const char *GetSocketPathById(int nModelId);

enum
{
	PET_FIGHT_ACTIVE = 0,	// 宠物主动模式
	PET_FIGHT_PASSIVE = 1,	// 宠物被动模式
};

struct tagPetDiathesisData_ToTua
{
	int m_nID;                             
	int m_nLevel;                          
};

struct tagPetDiathesis_ToLua
{
	int							m_nFightMode;
	tagPetDiathesisData_ToTua	m_diathesisData[MAX_PET_SKILL_GRID];
	int							m_nNum;
};

struct tagNetBarInfo_ToLua
{
	int DynValueNum;                     	/*  Ver.1140 动态效果值数量 */
	int DynValue[MAX_FAKE_STATUS_DYN_VALUE]; 	/*  Ver.1140 动态效果值 */
};

enum
{
	CLIENT_BASE_MSPD = BASE_MSPD,
};
void logPrivateInfo( const char* pMessage );
//tolua_end

// 技能造成位移效果的数据
struct tagBackInfo
{
	unsigned int startTime;
	int mapid;
	int x;
	int z;
	int backTime;
	int type;
	int height;
	int speed;
	bool passive;
	int actId;
	std::string effect;

	union
	{
		struct
		{
			int x;
			int y;
			bool inverted;
		} circle;
	} extra;
};

enum
{
	PET_WINK_TIME = 10000,
};

class IActorDeathAction;
class UIObject;

class GameActor : public RoleAttr//tolua_export
{//tolua_export
public:
	GameActor();
	virtual ~GameActor();
	
	
	virtual void setPkValue( int nPkValue );
	virtual void setDesignationID( int nDesignationID );
	virtual	void setRelationType( int nRelationType );
	virtual void setObjCamp( int nObjCamp );
	virtual void setHP( int hp );
	virtual void setMaxHP(int maxhp);
	// 对象心跳处理
	virtual void Update(unsigned int dtick);
	virtual void UpdateMin(unsigned int dtick);
	void UpdateRotateState( unsigned int dtick );
	// 对象界面刷新
	virtual void updateUI( int nChangeValue );
	virtual void actualUpdateUI();
	virtual bool UpdateMoveBack(){ return true; };
	virtual void OnEnterMap(GameMap *pmap);
	virtual void OnLeaveMap();
	virtual void OnReachPathTarget();
	virtual void OnPathMove(GameMaskPath *ppath);
	virtual void onTurn(int x, int z);
	virtual void onStop(int x, int z, int state = 0 );
	virtual void OnRevive();
	virtual void onHorse( int nHorseId,bool bBind=true );
	// 进入战斗
	virtual void onFight(bool bForce = true);
	// 离开战斗状态
	virtual void onLeaveFight();
	// 杀人事件
	virtual void kill(int id){}
	// 攻击击中事件
	virtual void cast(int id, int lv){}
	virtual bool	canAutoTalkToNpc()
	{ 
		return false; 
	}

	virtual void	endAutoTalkToNpc(){}
	// 是否有时装
	virtual bool hasFashion();
	virtual int CmdMoveTo(int x, int z, int dis = 0 )
	{
		return 1;
	}
	virtual int ContinueMove()
	{
		return 1;
	}
	void	hideWeapon(unsigned int time);
	void	hideWeapon(bool b);
	virtual void CmdMoveBack( int x, int z ){};
	virtual void CmdMovePause();
	virtual void CmdMoveStop();
	virtual void circularMoveAngle( int from_x, int from_z, int to_x, int to_z );

	virtual void doBack( int skillid, int sourceid, int x, int z, int BackTime = FLY_TIME, 
							int type = BACK_LINE, int nHeight = FLY_HIGHT, int speed = 2000, 
								const std::string &extra = "", int delay = 0, bool passive = true, int actId = 0, const char *atkEff = "" );
	virtual void stopBack();
	// 添加一条飞行路径(参数:indexId路径ID,bFristFly是否是刚起飞)
	void setAutoFly( int indexId, bool bFristFly = true );		

	void LoadRes(int resid);

	void onUpdateSkill(int skillid);

	// 设置当前正在吟唱的技能
	virtual void setPrepareSkill(int id = 0, int lv = 0, int index = 0, unsigned int time = 0);
	virtual void setPrepareSkill(int id, int lv, int index, unsigned int time, unsigned int total);
	virtual void setPilotSkill(int id = 0);
	virtual int getPilotSkill();

	void addPopRich( const char* pszPopName );
	void addActorBindFrame( UIObject* pFrame );
	void setStickyFrame( UIObject* pFrame ) {
		m_stickyFrame = pFrame;
	}
	UIObject* getStickyFrame() {
		return m_stickyFrame;
	}
	void SetID(int id)
	{
		m_ID = id;
		m_BufStatus.setOwnerId(id);
	}
	void SetPositionTM( int x, int y, int z );
	virtual void setSubType() {};
	virtual void setAtkType() {};
	
	void ChangeActState(int state,bool bRevive = false);
	bool CanChangeActState(int state);
	GameState& getActState()
	{
		return m_ActState;
	}
	// 设置僵直时间
	void setSkillTetanic( unsigned int time );
	bool isInSkillTetanic();
	// 切换站立
	void onChangeStand();
	// 切换移动
	void onChangeMove();
	// 切换到尸体
	void onChangeBone();
	// 获取头顶高度(玩家在跳跃时改变高度)
	int  getOverHead();

	void setInState( int state, bool flag );
	bool getInState( int state );

	void lockFace(int type, int value, int skillId);
	virtual void unLockFace();
	void SetFaceTo(int x, int z, bool immediately = false);
	void SetFaceTo(int from_x, int from_z, int to_x, int to_z, bool immediately = false);
	void SetFaceTo(int angle, bool immediately = false);

	void setDeathAction(IActorDeathAction *action);
	
	void setCurSkillInfo( int skillId, const char* curEffect )
	{
		m_curSkillAction	= skillId;
		m_curSkillEffect[0] = curEffect;
	}

	void setPetDiathesis( int nNum, const CSPETDIATHESISVIEW* pDiathesisDatas );
	void setPetDiathesis( int nMAXNum, const PETSKILLGRID* pDiathesisGrides );
	
	//tolua_begin
	virtual void setType( int type );
	virtual void setSubType( int nSubType );
	virtual void breakBTypePre() {}
	void setTeamColor(int r, int g, int b);
	bool hasOtherBooty();
	virtual void SetPosition(int x, int z, int height = 0 );
	// 以毫米为单位
	GameXYZ GetPosition()
	{
		return m_Pos; 
	}
	void hide(bool b);
	bool isShow()
	{
		return m_isShow;
	}
	void SetShow(bool tshow)
	{
		m_isShow=tshow;
	}
	bool needAddButtonName();
	void setCollectInfo( int nRace, bool bEnable )
	{
		m_actorCollectInfo.m_nRace			= nRace;
		m_actorCollectInfo.m_bEnableCollect	= bEnable;
	}

	void enableCollect( bool bEnable )
	{
		m_actorCollectInfo.m_bEnableCollect	= bEnable;
	}

	bool canCollect()
	{
		return m_actorCollectInfo.m_bEnableCollect;
	}

	void setPetFightMode( int nFightMode );

	int getPetFightMode( )
	{
		return m_petDiathesisInfo.m_nFightMode;
	}
	
	int  GetCatchPet();
	
	// 以毫米为单位
	int	getPlayerX( )
	{
		return m_Pos.x;
	}
	// 以毫米为单位
	int getPlayerZ( )
	{
		return m_Pos.z;
	}

	int getMapId();

	int getMapRealId();

	bool	isDead()
	{
		return m_ActState.GetState() == GSACT_DEAD;
	}
	
	bool	isInStall()
	{
		return m_ActState.GetState() == GSACT_STALL;
	}
	
	bool	isInFly()
	{
		return m_ActState.GetState() == GSACT_FLY || m_ActState.GetState() == GSACT_AUTOFLY;
	}

	bool	isInMove()
	{
		return getActState().GetState() == GSACT_MOVE;
	}

	bool	isInFight()
	{
		 return m_ActState.getInState( GSACT_IN_FIGHT );
	}
	bool	isInStand()
	{
		return m_ActState.GetState() == GSACT_STAND ;
	}
	
	// 判断是否在骑乘状态
	bool	isInRide()
	{
		return m_ActState.getInState( GSACT_IN_RIDE );
	}
	
	// 判断是否处于变身状态
	bool	isInMorph()
	{
		return m_ActState.getInState( GSACT_IN_MORPH );
	}

	
	bool	isInScene();

	bool	isInMap()
	{
		return m_pCurMap != NULL;
	}

	//是否特殊副本要求不显示头顶
	bool IsInSpecialPworldNotShowTitle();
	
	void RawSetSelectTarget( int nTargetId );
	bool canSelectTarget( GameActor* pTarget);
	void SetSelectTarget( int nTargetId );
	void UpdateTargetDrawTitle( int nTargetID );
	const char* GetModePathById(int nModelId);
	int getIconId();
	int GetTargetID();

	int GetID()
	{
		return m_ID;
	}

	int GetMorphResID()
	{
		return m_nMorphResID;
	}
	
	int getAtkType()
	{
		return m_nAtkType;
	}

	int getSubType()
	{
		return m_nSubType;
	}

	int getResId()
	{
		return m_ResID;
	}

	int getRealId()
	{
		if (m_RealID > 0)
		{
			return m_RealID;
		}
		return getResId();
	}

	void setRealId(int realid)
	{
		m_RealID = realid;
	}

	int GetSelectTarget()
	{
		return m_SelectTargetId;
	}

	int getAngle() 
	{ 
		return m_nAngle; 
	}
	
	int getSpeed()
	{
		return m_nSpeed;
	}
	
	void BuildTitle()
	{
		m_RoleText.BuildTitle();
	}

	GameBufStatus* getBufStatus()
	{ 
		return &m_BufStatus; 
	} 

	// 获取骑乘信息
	Rider*			getRider() 
	{ 
		return &m_Rider; 
	}

	SpecialMove* getSpecialMove()
	{
		return m_pSpecialMove;	
	}

	void playMotion( const char* szEffect, bool bResetPlay = false );
	void stopMotion( const char* szEffect );

	void playMotion(const char* szEffect, int motionclass);
	bool isMotionPlaying(const char* szEffect, int motionclass);
	// 播放动作
	void playAction( int actId, const char* szEffect );
	// 获得NPC任务显示标记
	virtual int getNpcQuestShow();
	// 获取人物当前的X和Z坐标
	tagPlayerPosInfo& getPlayerPostion();
	// 获取宠物
	GamePet* getPet();
	GameQiYue& getQiYue();
    // 获取法宝
    GameTrump* getTrump();
	// 获取藏宝图信息
	GameCBT& getCBTInfo();

	// 获取当前吟唱技能id
	int		getPrepareSkill()
	{
		return m_nPrepareSkill;
	}

	int				getPrepareTime();

	int				getTotalPrepareTime();

	// 取消跟随某个玩家
	virtual void cancelWeiXing( ) {}
	
	tagPetDiathesis_ToLua& getPetDiathesis()
	{
		return m_petDiathesisInfo;
	}

	void startGray();
	void stopGray();

	int getCollectSlotMax()
	{
		return CollectSkill_SlotMax;
	}

	int getCollectSlotCurr()
	{
		return CollectSkill_SlotCurr;
	}

	int getCollectSlotType()
	{
		return CollectSkill_AddType;
	}
	bool canPlayChatAction();

	void playChatAction(int actionID,bool bReset);

	MonsterMachine* getMonsterMachine(){return &m_MonsterMachine;}
	
	bool isOnMonsterMachine();

	bool isInCarry();

	bool	isInSafeOverlay();
	int		getPetGenius();
	int		getPetGeniusId();
	int		getPetGeniusLv();
	bool	isRollItem();
	bool	isNeedShowSafeAreaBuf();
	int		getCurActionId();
	void	addPopWindow( const char* szText );
	bool	isAFK();

	// 是否显示头盔
	bool		isShowHead();
	// 是否显示时装
	bool		isShowFashion();
	// 是否显示背饰
	bool		isShowBack();
	// 是否显示面饰
	bool		isShowFace();
	// 是否显示情侣夫妻关系
	bool		isShowLove();
	void	setHitTargetID( int nTargetID );

	int		getHitTargetID(){ return m_nHitTargetID ;};
	
	//是否可以拾取
	bool	canPickUp();
	
	// -1 表示没有跨服
	int		getServerId();

	tagNetBarInfo_ToLua* getNetBarInfo(){return &m_NetBarInfo;};

	bool isPickItem(){return m_IsPickItem;}
	tagAchievementDef_ToLua* getAchievementDef( int achievementId, int categoryId );
	tagAchievementCategoryDef_ToLua* getAchievementCategoryDef( int categoryId );

	void setTextureByID( int id,const char* szPath );
	int  getWanMeiStarNum();
	//tolua_end
	
	Ogre::ShowActor *getShowActor()
	{
		return m_pShowActor;
	}

	GameMap *getCurMap()
	{
		return m_pCurMap;
	}
	
	inline RoleText& getRoleText() 
	{ 
		return m_RoleText; 
	}

	void setSpeed(int s);
	
	bool canPlayEffect();
	void playAction(const char *pname);
	void playAction( int seqid, const char **pmotions = NULL, int num_motion = 0, int motion_class = 0 );
	bool hasAnim(int seq);
	
	void onMorph( int resId );
	void OnStall(bool bStall);
	void onRestore( int resId );
	virtual void onAFK(bool b);
	void playAFK(bool b);
	void loadFairyEffect();
	unsigned int	getClanColor();
	void			setClanColor(  unsigned int nClanColor );
	void			resetClan();
	void			setClanAlly();
	void			setClanSame();
	void			setClanEnemy();
	bool			isClanSame();
	bool			isClanAlly();

	Ogre::AvatarEntity* getAvatar();

	virtual void endCollect() {};
	
	bool isJumping();
	virtual void jump();
	virtual void sitdown();
	virtual void situp();

	void	setRollType(int nType);

	//怪物机械是否属于这个玩家	param 怪物机械
	bool	isRideMonster( GameActor* pActor );

	void	optimizeClear(bool show = false);
	bool	isOptimizeClear() { return m_optimizeClear; }
	void	optimizeDisplay(bool show = false);
	bool	isOptimizeDisplay() { return m_optimizeShow; }
	
	void	setBuildTime(unsigned int remain);

	void	setMonsterLiveTime(int nTime){ m_nLiveTime = nTime; };
	int		getMonsterLiveTime(){ return m_nLiveTime; }

	void	setBaseWarResMon( bool bIsResMon ){ m_IsBaseWarResMon = bIsResMon; };
	bool	IsBaseWarResMon(){ return m_IsBaseWarResMon; };
	void	setBaseWarClanWID( tdr_ulonglong clanWID ){ m_BaseWarClanWID = clanWID; };
	tdr_ulonglong getBaseWarClanWID(){ return m_BaseWarClanWID; };
	bool	IsCurMapHasStrongPoint( int mapID );
	tdr_ulonglong getCurMapStrongOwnerWID( int mapID );
	
	//tolua_begin
	int		getMonsterPhysicType();
	//tolua_end

	// 判断是否为采集车 2012.04.11
	//tolua_begin
	bool    isCollectMachine( );
	const char* getActorWorldName();
	//tolua_end
	void setActorWorldName( const char* szName );

	// 使用物品吟唱
	void startUseItemChant( unsigned int chantTime, int nItemID, int nTargetID, int nListType = -1, int nGridIdx = -1 );
	// 召唤精灵吟唱
	void startTrumpChant( int nHeighId, int nLowId, int nGridIdx );
	// 召唤坐骑吟唱
	void startRideChant( int nRideId, int nLevel );
	// 召唤宠物吟唱
	void startPetChant( tdr_longlong wid );
	// 生活技能吟唱
	void startSecondaryChant( unsigned int uChantTime, int nType, int nId);
	//分解吟唱
	void startResolveChant( int nListType, int nGridIdx, tdr_ulonglong Wid, int nType );
	//根据当前吟唱状态判断是否可以开始吟唱
	bool canStartChant( int nType );

	void	setChantData( IChant_Base* pChant );
	void	requestBreakChant( int nType );
	void	finishChant( int nType );
	void	onBreakChant( int nType );
	bool	isInPre( int nType );
	// 打断当前的吟唱
	void	BreakCurChant();

	// 是否是破烂商运镖车
	bool	isPolanCar();
	void	setDartCarType( unsigned char type );
	// 取消假死
	void	requestCancelShamDead();
	//设置旗子显示
	void	setBannerInfo();
	//设置完美星 走路特效
	void	SetFootEffect( const GameXYZ &pos ,int nStar = 9 );
	// 设置完美星 特效
	void	setPerfectEffect();
	//设置失落图腾效果
	void	setLostTotemEffect();
	void	setVipFlag( int nFlag );
	int		getVipFlag();
	int		getXuKongFlag() { return m_nXuKongFlag; }
	void	setXuKongFlag( int nFlag )
	{
		m_nXuKongFlag = nFlag;
	}
	void	setXuKongEffect();
	bool	canActWingFloat();
public:
	// 注册事件函数
	void	registerTickEvent( GameActor_Tick* pTick );
	// 查看事件是否处理完
	bool	haveTickEvent( GameActor_Tick* pTick );

public:
	struct tagCollectInfo
	{
		tagCollectInfo()
		{
			m_nRace = 0;
			m_bEnableCollect = false;
		}

		int		m_nRace;
		bool	m_bEnableCollect;
	};
	int					m_nShowFlag;				// 头像时装是否显示+情侣夫妻关系是否显示
	tagCollectInfo				m_actorCollectInfo;
	GameBufStatus				m_BufStatus;
	tagAttackLock				m_AttackLock;
	Equip						m_Equip;
	std::string					m_preSkillEffect;
	GameMapMover*				m_pMapMover;
	Rider						m_Rider;
	MonsterMachine				m_MonsterMachine;
	GamePet						m_Pet;
	GameQiYue					m_QiYue;
    GameTrump                   m_Trump;
	GameCBT						m_CangBaoTuInfo;
	
	int							m_curSkillAction;
	int							m_nCurFairyEffect;
	const char*					m_curSkillEffect[4];
	std::map<int, tagBackInfo>	m_BackInfoList;
	unsigned int				m_leastNoCrimeTime;
	unsigned int				m_lastChangeNameColorTime;
	bool						m_isLastChangeNameColor;

	int							m_nHitTargetID;
	tagCSActionMonBootyFirst	m_BootyInfo;
	tagViewBanner				m_ViewBanner;
	// 简单阴影
	Ogre::SmallDecal			*m_pSmallDecal;

	unsigned int				m_lastOptimizeTime;
	bool						m_IsPickItem;

	bool						m_IsBaseWarResMon;
	tdr_ulonglong				m_BaseWarClanWID;
	
	int							m_nPerfectStar;
protected:
	int m_ID;
	int m_ResID;
	int	m_nMorphResID;
	int m_RealID;

	bool m_isShow;

	GameXYZ						m_Pos;					// 这个以mm作为单位
	Ogre::ShowActor*			m_pShowActor;
	GameMap*					m_pCurMap;				// 当前地图
	GameState					m_ActState;				// 动作状态
	RoleText					m_RoleText;				// 角色头顶文字信息
	int							m_SelectTargetId;		// 选中对象实体Id
	GameActor_Tick_InFightTime* m_pFightTime;			// 进入战斗
	
	GameXYZ						m_MoveEndPos;
	int							m_MoveEndDis;
	SpecialMove*				m_pSpecialMove;
	Ogre::Entity*				m_ReviveEff;
	int							m_nAngle;
	int							m_nFinalAngle;
	int							m_nSpeed;
	int							m_curRunAngle;		
	int							m_nAtkType;
	int							m_nSubType;

	unsigned int				m_tmEquipStartUsed;
	int							m_tmEquipUsedTime;

	tagPlayerPosInfo			m_PlayerPosInfo;

	std::list<GameActor_Tick*>	m_TickList;
	bool						m_bWaterPlay;
	unsigned int				m_uLastFightTime;

	int							m_nOverHead;
	unsigned int				m_nLastJumpTime;
	bool						m_isJumping;

	unsigned int				m_uPrepareTime;
	int							m_nTotalPrepareTime;
	int							m_nPrepareSkill;
	int							m_nPilotSkill;
	unsigned int				m_skillTetanicTime;	// 法术僵直结束时间
	
	GameActor_Tick_MainPlayerFree*	m_pPlayerFreeTime;	//玩家休闲计时
	std::list<IActorDeathAction *>	m_lstDeathAction;
	tagPetDiathesis_ToLua			m_petDiathesisInfo;

	int							m_nChatActionID;
	unsigned int				m_showWeaponTime;
	bool						m_isShowWeapon;
	bool						m_optimizeShow;
	bool						m_optimizeClear;
	bool						m_mustUpdateUI;

	float m_fJumpTime;
	bool m_bStartJump; // 决定跳

	bool m_bStartJump1; // 跳的预备动作过了，跳了起来

	int m_RollType;	//掉落物品的roll点方式
	bool	m_isAFK;
	int								m_clanRelation;
	unsigned int					m_clanColor;

	int								m_nTargetCatchPetID;
	int								m_nCatchPetNum;

	unsigned int					m_BuildFinishTime;

	int								m_nLiveTime;
public:
	bool					m_flagDead;
	GameActor_Carry*				m_pCarry;
	int CollectSkill_SlotMax;            	/*   积累数技能槽的上限,脚本给定,天赋影响 */
	int CollectSkill_SlotCurr;           	/*   积累数技能槽的当前值 */
	int CollectSkill_AddType;            	/*   积累数技能槽的增加方式,1被击积累 2攻击积累 3战斗时间积累 */

	int m_CollectTime;

	int								m_nPetGenius;
	int								m_nPetGeniusId;
	int								m_nPetGenniusLv;
	bool							m_isHeti;
	int								m_nPetStrengthenLevel;

	GameActor_PKEffect				m_pkEffect;

	tagNetBarInfo_ToLua				m_NetBarInfo;

	unsigned char					m_DartCarType;		//商运车类型

	private:
	std::vector<UIObject*>			m_vecActorBindFrames;
	UIObject*							m_stickyFrame;
	IChant_Base*					m_pChantPart;		//吟唱模块

	char							m_szActorWorldName[WORLD_NAME_LEN];

	int								m_nVipFlag;
	int								m_nXuKongFlag;
	//static bool canAttackPVP();
};//tolua_export

class GamePlayer : public GameActor//tolua_export
{//tolua_export
};//tolua_export


class IActorDeathAction : public Ogre::MemoryDelegation
{
public:
	virtual ~IActorDeathAction(){};
	virtual void doAction(GameActor *) = NULL;
};