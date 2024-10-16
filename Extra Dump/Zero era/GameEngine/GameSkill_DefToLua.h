#pragma once
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"

//tolua_begin
#pragma pack(1)

struct tagSkillLearn_ToLua
{
	unsigned char LevelMin;                        	/*   等级要求 */
	unsigned char Career;                          	/*   职业要求 */
	unsigned short PreDiathesisID;                  	/*   前置素质要求 */
	unsigned int ItemID;                          	/*   道具要求 */
	unsigned short TaskID;                          	/*   任务完成要求 */
	unsigned short MoneyAttrID;                     	/*   需要的金钱种类 */
	unsigned int MoneyVal;                        	/*   需要的金钱数量 */
	unsigned char Mode;                            	/*   学习方式 */
	unsigned char PreDiathesisLevel;               	/*   前置素质等级要求 */
	unsigned int Exp;         
};

struct tagFittingList_ToLua
{
	unsigned short ID;                             
	unsigned short Group;                          
};
struct tagResRange_ToLua
{
	unsigned int RangeType;                       	/*    Bind Macrosgroup:SKILL_RANGE_TYPE,*/
	unsigned short RangeVal1;                       	/*   半径或宽 */
	unsigned short RangeVal2;                       	/*   角度或长 */
};

struct tagSkillResult_ToLua
{
	unsigned short SkillResultID;                   	/*    Bind Macrosgroup:RESULT,*/
	short SkillResultVal1;                
	int SkillResultVal2;                
	int SkillResultVal3;                
	int SkillResultVal4;                
};

struct tagSkillDef_ToLua
{
	unsigned short SkillID;                        
	unsigned short Level;                          
	char Name[RES_NAME_LEN];             
	int IconID1;                         	/*  Ver.6 图标id */
	int IconID2;                         	/*  Ver.6 图标id */
	int IconID3;                         	/*  Ver.6 图标id */
	ARMTYPE ArmType[MAX_SKILL_ARMTYPE];      	/*   技能的武器类型 */
	unsigned int SelfSpeCoolType;                 	/*   自身所属技能指定冷却类型 */
	unsigned char SpeCoolType1;                    	/*   触发指定技能冷却类型 */
	unsigned char SpeCoolType2;                   
	unsigned char SpeCoolType3;                   
	unsigned char SpeCoolType4;                   
	unsigned short SpeCoolVal1;                     	/*   触发指定技能冷却值，单位100ms */
	unsigned short SpeCoolVal2;                     	/*   触发指定技能冷却值，单位100ms */
	unsigned short SpeCoolVal3;                     	/*   触发指定技能冷却值，单位100ms */
	unsigned short SpeCoolVal4;                     	/*   触发指定技能冷却值，单位100ms */
	unsigned short CommCool;                        	/*   公共技能冷却值，单位100ms */
	unsigned short SelfCool;                        	/*   自我技能冷却值，单位100ms */
	unsigned short TargetType;                      	/*   技能对象类型 */
	unsigned short HelpSkillID;                     	/*   借助时对应的技能 */
	unsigned char HelpSkillLevel;                  	/*   借助时对应的技能等级 */
	unsigned char MovePrepare;                     	/*   移动是否可以直接中断吟唱或集气 */
	unsigned char MoveAttack;                      	/*   是否可以边移动边攻击 */
	unsigned char MainAttr;                        	/*   技能主属性 Bind Macrosgroup:SKILL_ATTR,*/
	unsigned int IceHurtMax;                     
	unsigned int IceHurtMin;                     
	short IceHurtMul;                     
	short IceHurtMulMax;                   	/*  Ver.127  */
	unsigned short FireHurtMax;                    
	unsigned short FireHurtMin;                    
	short FireHurtMul;                    
	short FireHurtMulMax;                  	/*  Ver.419  */
	unsigned short ThunderHurtMax;                 
	unsigned short ThunderHurtMin;                 
	short ThunderHurtMul;                 
	short ThunderHurtMulMax;               	/*  Ver.419  */
	unsigned short LightHurtMax;                   
	unsigned short LightHurtMin;                   
	short LightHurtMul;                   
	short LightHurtMulMax;                 	/*  Ver.419  */
	unsigned short NightHurtMax;                   
	unsigned short NightHurtMin;                   
	short NightHurtMul;                   
	short NightHurtMulMax;                 	/*  Ver.419  */
	unsigned int PhsicHurtMax;                   
	unsigned int PhsicHurtMin;                   
	short PhsicHurtMul;                   
	short PhsicHurtMulMax;                 	/*  Ver.127  */
	unsigned int WaterHurtMax;                    	/*  Ver.419  */
	unsigned int WaterHurtMin;                    	/*  Ver.419  */
	short WaterHurtMul;                    	/*  Ver.419  */
	short WaterHurtMulMax;                 	/*  Ver.419  */
	unsigned int SiegeHurtMax;                    	/*  Ver.419  */
	unsigned int SiegeHurtMin;                    	/*  Ver.419  */
	short SiegeHurtMul;                    	/*  Ver.419  */
	short SiegeHurtMulMax;                 	/*  Ver.419  */
	unsigned int HolyHurtMax;                     	/*  Ver.419  */
	unsigned int HolyHurtMin;                     	/*  Ver.419  */
	short HolyHurtMul;                     	/*  Ver.419  */
	short HolyHurtMulMax;                  	/*  Ver.419  */
	unsigned int MachineHurtMax;                  	/*  Ver.419  */
	unsigned int MachineHurtMin;                  	/*  Ver.419  */
	short MachineHurtMul;                  	/*  Ver.419  */
	short MachineHurtMulMax;               	/*  Ver.419  */
	unsigned short AttackActTime;                   	/*   攻击动作时间，此时间角色不能移动，单位100ms */
	unsigned char BindSkill;                       	/*  Ver.4 本技能是否是绑定技能，不需企划填写 Bind Macrosgroup:BINDSKILL_TYPE,*/
	unsigned char FightStat;                       	/*  Ver.8 技能对战斗状态的判断标志 Bind Macrosgroup:SKILL_FIGHT_STAT,*/
	tagResRange_ToLua MainRange;                       	/*   技能主范围 */
	tagResRange_ToLua SecondRange;                     	/*   技能副范围 */
	int Distance;                        	/*   距离或射程 */
	int CollectNumMax;                   	/*   积累数上限,给临时道具用 */
	int CollectNumUse;                   	/*   消耗积累数,给临时道具用 */
	int CollectSkillUse;                 	/*  Ver.311 积累数技能消耗,0表示不是积累数技能 */
	unsigned short SpdUse;                          	/*  Ver.49 消耗行动力 */
	unsigned short MPUse;                           	/*   消耗MP */
	unsigned short CollectNumAtkAdd;                	/*   攻击时增加积累数 */
	unsigned short CollectNumDeadAdd;               	/*   杀死时增加积累数 */
	unsigned short MPAtkAdd;                        	/*   攻击时增加MP */
	unsigned short MPDeadAdd;                       	/*   杀死时增加MP */
	unsigned char CollectNumAtkAddFlag;            	/*   本技能是否攻击时增加积累数 */
	unsigned char CollectNumDeadAddFlag;           	/*   本技能是否杀死时增加积累数 */
	unsigned short JiqiMul;                         	/*  Ver.568 集气的倍数Z 公式X（1 + z * y/t） */
	unsigned short JiqiTime;                        	/*  Ver.568 集气总时间t,单位秒 */
	unsigned short JiqiChgFlag;                     	/*  Ver.568 集气是否要转换成频数 */
	unsigned short ContinueNum;                     	/*   频数，例如机枪技能 */
	unsigned char HeavyHitRate;                    	/*   暴击率 */
	unsigned char TargetNum;                       	/*   范围攻击时同时伤害的对象个数 */
	unsigned short PrepareTime;                     	/*   技能准备时间，单位100ms */
	unsigned char PrepareType;                     	/*   技能准备是瞬发、吟唱、集气 Bind Macrosgroup:PREPARE_TYPE,*/
	unsigned char DeadTarget;                      	/*   是否可以对死忘目标使用 */
	tagSkillResult_ToLua SkillResult[MAX_SKILL_RESULT];  
	unsigned int HurtLimit;                       	/*   伤害上限 */
	unsigned short PilotTime;                       	/*   引导攻击的间隔，单位100ms */
	unsigned short PilotNum;                        	/*   引导攻击的次数 */
	unsigned char NoHurtSkill;                     	/*   是否是非伤害技能，例如借助、补血等 */
	unsigned char NoSkillStop;                     	/*   是否是要忽略法术阻挡，例如落雷、远程炮 */
	unsigned char TotemLimit;                      	/*  Ver.36 图腾数量上限 */
	unsigned char NoFightStatUse;                  	/*  Ver.40 非战斗状态下使用 Bind Macrosgroup:NO_FIGHT_USE,*/
	short PressRate;                       	/*  Ver.10 技能压力指数 */
	unsigned short NextSkill;                       	/*  Ver.10 后续技能 */
	unsigned short NextSkill2;                      	/*  Ver.10 后续技能 */
	unsigned short NextSkill3;                      	/*  Ver.10 后续技能 */
	char SkillDesc[DESC_LEN];            
	SKILLARMCHG SkillArmChg[MAX_SKILL_ARM_CHG];  	/*  Ver.37  */
	short AmmUse;                          	/*  Ver.12 技能所耗弹药 */
	unsigned char NotBootyFirstAtk;                	/*  Ver.23 0 会触发战利品优先者判断，1 不会触发战利品优先者判断 */
	unsigned char TargetRange;                     	/*  Ver.23 1 表示目标范围，技能是范围攻击但需要选择一个目标进行操作 Bind Macrosgroup:TARGET_RANGE,*/
	SKILLSTATUSCHG StatusChg;                       	/*  Ver.40 状态影响技能，例如精确射击 */
	tagSkillLearn_ToLua Learn;                           	/*  Ver.45 技能学习相关 */
	unsigned short FittingsDefault[MAX_SKILL_DEFAULT_FITTINGS]; 	/*  Ver.40 默认配件，获得技能时自动加上 */
	unsigned short TmpEnergeUse;                    	/*  Ver.26 技能消耗的临时能量 */
	unsigned short PetAddAge;                       	/*  Ver.55 使用此技能,仙子增加的寿命 */
	unsigned char SkillSwitch;                     	/*  Ver.50 是否是可开关技能 */
	unsigned char CollectNumAddNum;                	/*  Ver.52 表示技能每小时增加积累数的量，目前只对宠物技能有效 */
	unsigned char FightCollectNumAddNum;           	/*  Ver.52 表示战斗状态每3秒回复的积累数，目前只对宠物技能有效 */
	unsigned char SkillType;                       	/*  Ver.63 技能类型 Bind Macrosgroup:SKILL_TYPE,*/
	short MonMiniMul;                      	/*  Ver.60 微怪技能伤害修正 */
	short MonWeakMul;                      	/*  Ver.60 弱怪技能伤害修正 */
	short MonSoftMul;                      	/*  Ver.60 软怪技能伤害修正 */
	short MonNormalMul;                    	/*  Ver.60 普怪技能伤害修正 */
	short MonHardMul;                      	/*  Ver.60 硬怪技能伤害修正 */
	short MonXiMul;                        	/*  Ver.60 稀怪技能伤害修正 */
	short MonJinMul;                       	/*  Ver.60 精怪技能伤害修正 */
	short MonBossMul;                      	/*  Ver.60 BOSS 怪技能伤害修正 */
	short MonJiMul;                        	/*  Ver.60 极 怪技能伤害修正 */
	unsigned char LearnLevelLimit;                 	/*  Ver.70 技能面板可学等级限制 */
	unsigned char BeatBack;                        	/*  Ver.86 反击技能|1 反击状态附加技能或状态结束触发技能 */
	unsigned char WushuangFlag;                    	/*  Ver.92 无双标志,0--不是无双技 1--无限制无双技 2-副本无双技 */
	short UseXp;                           	/*  Ver.99 消耗xp值 */
	unsigned char IgnoreNoSkill;                   	/*  Ver.119 免疫无法使用技能的状态效果 */
	unsigned char LightningLinkNum;                	/*  Ver.576 闪电链的攻击次数,有这个值,就会跳跃攻击了 */
	TARGETSTATUSONE TargetStatus[MAX_TARGET_STATUS]; 	/*  Ver.121 目标状态影响 */
	short NoHurtPress;                     	/*  Ver.130 非伤害类技能产生的压力值  */
	unsigned char FairySkill;                      	/*  Ver.170 是否精灵技能0-否 1-是 */
	int AmmID;                           	/*  Ver.439 技能消耗的弹药ID */
	int RedRuneTransferedID;             	/*  Ver.551 猩红符文对应的技能ID */
	int BlueRuneTransferedID;            	/*  Ver.551 海洋符文对应的技能ID */
	int YellowRuneTransferedID;          	/*  Ver.551 翡翠符文对应的技能ID */
	char RuneDesc[SHORT_DESC_LEN];        	/*  Ver.582  */
	char NextLevelDesc[SHORT_DESC_LEN];   	/*  Ver.622  */
	int EnergyUse;                       	/*  Ver.626 怪物机械技能:使用技能消耗多少能量 */
	int AtkType;                         	/*  Ver.1105  Bind Macrosgroup:SKILL_ATK_TYPE,*/
	int UseItemID;                       	/*  Ver.1491 怪物机械技能:使用技能消耗一个物品 */
	int Auto;                            	/*  Ver.1597 是否适合自动施法 */
	int CtrlFlag;                        	/*  Ver.2529 位操作|SKILL_CTRL_FLAG */
};

/* 宠物技能升级概率|万分率 */
struct tagSkillUpRate_ToLua
{
	unsigned short Up;                             
	unsigned short Down;                           
	unsigned short NoChg;                          
};

struct tagDiathesisDef_ToLua
{
	unsigned short DiathesisID;                     	/*   素质ID，1到255 */
	unsigned short Level;                           	/*   素质等级 */
	char Name[RES_NAME_LEN];             
	int IconID;                          	/*  Ver.6 图标id */
	SKILLRESULT DiathesisResult[MAX_DIATHESIS_RESULT];
	unsigned int ArmType;                         	/*   素质生效的武器类型 */
	char DiathesisDesc[DESC_LEN];        
	tagSkillLearn_ToLua Learn;                           	/*  Ver.45 技能学习相关 */
	unsigned char LearnLevelLimit;                 	/*  Ver.70 素质面板可学等级限制 */
	unsigned short PreUsedGeniusPoint;              	/*  Ver.182 已近消耗的天赋点 */
	unsigned short UseGeniusPoint;                  	/*  Ver.182 升级本天赋需要消耗的天赋点 */
	unsigned short GeniusType;                      	/*  Ver.188 天赋的类型 是天赋Type必须大于1 */
	unsigned char PetDiathesisType;                	/*  Ver.141 宠物技能(实际是素质)类型 Bind Macrosgroup:PET_SKILL_TYPE,*/
	unsigned short SelfCool;                        	/*  Ver.160 自我技能冷却值，单位100ms */
	unsigned char PetDiathesisGradeType;           	/*  Ver.164 宠物技能品质，高级、低级 Bind Macrosgroup:PET_SKILL_GRADE_TYPE,*/
	int IconPosition;                    	/*  Ver.246 图标显示位置|0：无；1：显示到人物状态；2：显示到宠物状态； */
	unsigned short MutexDiathesisID;                	/*  Ver.297 互斥的天赋id,和互斥的天赋只能二者选其一 */
	unsigned char PetSkillType;                    	/*  Ver.1154 0-不是宠物技能 1-可领悟2-技能书学习 */
	unsigned short GroupSkill;                      	/*  Ver.1288 同一组对应技能ID,用于宠物学习,重置技能 */
	tagSkillUpRate_ToLua SkillUpRate;                     	/*  Ver.1538  */
	short ResetRate;                       	/*  Ver.1538  */
};

struct tagDiathesisData_toLua
{
	unsigned short DiathesisID;                    
	unsigned char DiathesisLevel;     
	unsigned char GeniusFlag;                      	/*  Ver.184 1表示这个是天赋 */            
	unsigned char Res;                            
};

struct tagDiathesisInfo_toLua
{
	unsigned int DiathesisNum;                   
	tagDiathesisData_toLua DiathesisList[MAX_DIATHESIS];   
	unsigned short UsedGeniusPoint;                 	/*  Ver.182 已经使用过总的天赋点 */
	unsigned short LearnGeniusPoint;                	/*  Ver.182 当前学到的天赋点 */
};

#pragma pack()

//tolua_end
