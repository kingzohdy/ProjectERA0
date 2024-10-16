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
	ATTACK_LOCK_NONE = 0,	// ������
	ATTACK_LOCK_OBJ,		// ����Ŀ��
	ATTACK_LOCK_DIR,		// ��������
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

enum//	�ķ����ܶ�
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
	MONSTER_UNSEEYOU	= 1,	// ���ᳯ�����NPC
	MONSTER_MACTION		= 2,	// ��ʱ��е����
	MONSTER_RIDE		= 4,	// ��ʱ��е���
	MONSTER_FUNCTION	= 8,	// ����
	MONSTER_QUEST		= 16,	// ����
	MONSTER_ACT			= 32,	// �
	MONSTER_TRAN		= 64,	// ����
	MONSTER_OTHER		= 128,	// ����
	MONSTER_PET			= 256,	//����
	MONSTER_SKILLNPC	= 512,	//����NPC
	MONSTER_SHOPNPC		= 1024,	//�̵�NPC
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
//��������
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
	unsigned char deadstate;                       	/*  Ver.74 ����״̬ ��Ҫ���ӹ���������ʧ�ļ��ַ�ʽ��������ʧ-�ɼ�Ʒ��0������ʧ��1�ӳ���ʧ */
	unsigned char normalrot;                       	/*  Ver.74 ����npc��Ҫ��һ����Ǳ�ʾ���������һ���������������㣩0������������1�������� */
	unsigned int userdata[24];                   
	unsigned int radius;                         
	unsigned int height;                         
	float size;                            	/*  Ver.159  */
	int rideact;                         	/*  Ver.103  */
	char visuals[RES_PATH_LEN];          
	char npcdesc[RES_NAME_LEN];           	/*  Ver.58 ��Ҫ������Ϣ */
	char effect[RES_EFFECT_LEN];          	/*  Ver.404 ���ֵ��� */
	int ZHigh;                           	/*  Ver.525 ��ͼ�ϵ�z�ᣬ��ʾģ�������ĸ߶� */
	unsigned int MiscType;                        	/*  Ver.532 λ�������� �ο�NPCDISPLAYINFO_MISC_TYPE Bind Macrosgroup:NPCDISPLAYINFO_MISC_TYPE,*/
	int PoseType;                        	/*  Ver.675 monster pose */
	int passengeract;                    	/*  Ver.809 �˿Ͷ��� */
	char socket[RES_PATH_LEN];            	/*  Ver.1085 ����ĵײ�ģ�� */
	int dieeffect;                       	/*  Ver.1262 ������Ч����ʹ��Ĭ�ϵ�������Ч��Ҫ����� */
	int birth;                           	/*  Ver.1490 ������Ч */
};

struct tagLevelUP_ToLua
{
	unsigned int Level;                          
	unsigned int LevelNextExp;                   
	unsigned int MoneyLimit;                     
	unsigned char LevelStand;                      	/*   �Ƿ��ǵȼ�ͣ���� */
	unsigned int ContributeExp;                   	/*  Ver.62 ������������ */
	unsigned int ContributeMoney;                 	/*  Ver.111 ����������Ǯ */
	unsigned short SitAddMp;                        	/*  Ver.83 ��������MP */
	unsigned short SitAddHp;                        	/*  Ver.83 ��������HP */
	int WarriorSkillList[MAX_LEVELUP_SKILL_NUM]; 	/*  Ver.84  */
	int MagicorSkillList[MAX_LEVELUP_SKILL_NUM]; 	/*  Ver.84  */
	int GunerSkillList[MAX_LEVELUP_SKILL_NUM]; 	/*  Ver.84  */
	int ChurchSkillList[MAX_LEVELUP_SKILL_NUM]; 	/*  Ver.84  */
	unsigned int DayExpLimit;                     	/*  Ver.85 ÿ����exp���� */
	unsigned short WarriorLiliang;                  	/*  Ver.128 ���� */
	unsigned short MagicorLiliang;                  	/*  Ver.128  */
	unsigned short GunerLiliang;                    	/*  Ver.128  */
	unsigned short ChurchLiliang;                   	/*  Ver.128  */
	unsigned short WarriorZhili;                    	/*  Ver.128 ���� */
	unsigned short MagicorZhili;                    	/*  Ver.128  */
	unsigned short GunerZhili;                      	/*  Ver.128  */
	unsigned short ChurchZhili;                     	/*  Ver.128  */
	unsigned short WarriorTili;                     	/*  Ver.128 ���� */
	unsigned short MagicorTili;                     	/*  Ver.128  */
	unsigned short GunerTili;                       	/*  Ver.128  */
	unsigned short ChurchTili;                      	/*  Ver.128  */
	unsigned short WarriorJingshen;                 	/*  Ver.128 ���� */
	unsigned short MagicorJingshen;                 	/*  Ver.128  */
	unsigned short GunerJingshen;                   	/*  Ver.128  */
	unsigned short ChurchJingshen;                  	/*  Ver.128  */
	unsigned short WarriorFangyu;                   	/*  Ver.128 ���� */
	unsigned short MagicorFangyu;                   	/*  Ver.128  */
	unsigned short GunerFangyu;                     	/*  Ver.128  */
	unsigned short ChurchFangyu;                    	/*  Ver.128  */
	unsigned short VimAdd;                          	/*  Ver.134  */
	unsigned short VimMax;                          	/*  Ver.134  */
	unsigned char AddRolePetGrid;                  	/*  Ver.144  */
	int MaxOfflineExp;                   	/*  Ver.567 ������߲������� */
	unsigned short WarriorAgile;                    	/*  Ver.426 ���� */
	unsigned short MagicorAgile;                    	/*  Ver.426  */
	unsigned short GunerAgile;                      	/*  Ver.426  */
	unsigned short ChurchAgile;                     	/*  Ver.426  */
	unsigned short AtkLevelMul;                     	/*  Ver.426 ������������ */
	unsigned short DefLevelMul;                     	/*  Ver.426 ������������ */
	int MonDefHeavyHit;                  	/*  Ver.426 ���ﱩ���ֿ� */
	unsigned short SwordWarriorLiliang;             	/*  Ver.431 ���� */
	unsigned short FireMagicorLiliang;              	/*  Ver.431  */
	unsigned short DoubleGunerLiliang;              	/*  Ver.431  */
	unsigned short ThunderChurchLiliang;            	/*  Ver.431  */
	unsigned short SwordWarriorZhili;               	/*  Ver.431 ���� */
	unsigned short FireMagicorZhili;                	/*  Ver.431  */
	unsigned short DoubleGunerZhili;                	/*  Ver.431  */
	unsigned short ThunderChurchZhili;              	/*  Ver.431  */
	unsigned short SwordWarriorTili;                	/*  Ver.431 ���� */
	unsigned short FireMagicorTili;                 	/*  Ver.431  */
	unsigned short DoubleGunerTili;                 	/*  Ver.431  */
	unsigned short ThunderChurchTili;               	/*  Ver.431  */
	unsigned short SwordWarriorJingshen;            	/*  Ver.431 ���� */
	unsigned short FireMagicorJingshen;             	/*  Ver.431  */
	unsigned short DoubleGunerJingshen;             	/*  Ver.431  */
	unsigned short ThunderChurchJingshen;           	/*  Ver.431  */
	unsigned short SwordWarriorFangyu;              	/*  Ver.431 ���� */
	unsigned short FireMagicorFangyu;               	/*  Ver.431  */
	unsigned short DoubleGunerFangyu;               	/*  Ver.431  */
	unsigned short ThunderChurchFangyu;             	/*  Ver.431  */
	unsigned short SwordWarriorAgile;               	/*  Ver.431 ���� */
	unsigned short FireMagicorAgile;                	/*  Ver.431  */
	unsigned short DoubleGunerAgile;                	/*  Ver.431  */
	unsigned short ThunderChurchAgile;              	/*  Ver.431  */
	int UIBaseFlee;                      	/*  Ver.470 �ο�����ֵ */
	int BattleWinExp;                    	/*  Ver.909 ս��ʤ������ */
	int BattleFailExp;                   	/*  Ver.909 ս��ʧ�ܾ��� */
	int MaxPworldOffExp;                 	/*  Ver.911 �����ÿ��δ�μӵĲ����������� */
	int MonLevelHit;                     	/*  Ver.1059 ����ȼ��ο������� */
	char AddSparPkg;                      	/*  Ver.1243 �Ƿ����Ӿ�ʯ�������� */
	int VigUp;                           	/*  Ver.1378 ��ɫ�ȼ��������� */
	int MaxStashMoney;                   	/*  Ver.1407 ��ɫЯ���������� */
	int MaxPackageMoney;                 	/*  Ver.1407 ��ɫЯ���������� */
	int DailyUseBull;                    	/*  Ver.1407 ��ɫÿ��ʹ�ð������� */
	int FzMod;                           	/*  Ver.1769 ������Ҫ�ķ���ϵ�� */
	int ExpLeiji;                        	/*  Ver.1811 �����ۻ�������0��ʾ�����ƣ�����59�� */
	unsigned int DayZhengyiLimit;           /*  Ver.2169 ÿ������������� */
	int BaseGenius;                      	/*  Ver.2172 ��ӡ�ȼ���Ӧ�Ļ����츳�� */
	unsigned short GreenGemLimit;           /*  Ver.2178 ÿ�������潱������ */
	int RemainTm;                        	/*  Ver.2391 ��λ��,�����õȼ�ͣ��������������ƽ����ض�ս�� */
};

struct tagAtkerHurtChg_ToLua
{
	short AtkerIceHurtMul;                 	/*   ħ���˺�����(������) */
	short AtkerPhsicHurtMul;               	/*   ���������˺�����(������) */
	short AtkerSiegeHurtMul;               	/*  Ver.419 ������ʩ�˺�����(������) */
	short AtkerHolyHurtMul;                	/*  Ver.419 ��ʥ�˺�����(������) */
	short AtkerMachineHurtMul;             	/*  Ver.419 ��е�˺�����(������) */
	short AtkerFireHurtMul;                	/*   �������˺�����(Ԫ������) */
	short AtkerThunderHurtMul;             	/*   �������˺�����(Ԫ������) */
	short AtkerLightHurtMul;               	/*   �������˺�����(Ԫ������) */
	short AtkerNightHurtMul;               	/*   �������˺�����(Ԫ������) */
	short AtkerWaterHurtMul;               	/*  Ver.419 ˮ�����˺�����(Ԫ������) */
	short AtkerMagicHurtMul;               	/*   ħ���˺����� */
	short AtkerHurtMul;                    	/*   �������˺����� */
	int AtkerPhsicHurtAdd;               	/*   ���������˺�����(������) */
	int AtkerIceHurtAdd;                 	/*   ħ���˺�����(������) */
	int AtkerSiegeHurtAdd;               	/*  Ver.419 ������ʩ�˺�����(������) */
	int AtkerHolyHurtAdd;                	/*  Ver.419 ��ʥ�˺�����(������) */
	int AtkerMachineHurtAdd;             	/*  Ver.419 ��е�˺�����(������) */
	int AtkerFireHurtAdd;                	/*   �������˺�����(Ԫ������) */
	int AtkerThunderHurtAdd;             	/*   �������˺�����(Ԫ������) */
	int AtkerLightHurtAdd;               	/*   �������˺�����(Ԫ������) */
	int AtkerNightHurtAdd;               	/*   �������˺�����(Ԫ������) */
	int AtkerWaterHurtAdd;               	/*  Ver.419 ˮ�����˺�����(Ԫ������) */
	int AtkerMagicHurtAdd;               	/*   ħ���˺����� */
	int AtkerHurtAdd;                    	/*   �������˺����� */
};

struct tagAtkeeHurtChg_ToLua
{
	short AtkeePhsicHurtMul;               	/*   �ֿ����������˺�����(������) */
	short AtkeeIceHurtMul;                 	/*   �ֿ��������˺�����(������) */
	int AtkeeSiegeHurtMul;               	/*  Ver.419 �ֿ�������ʩ�˺�����(������) */
	int AtkeeHolyHurtMul;                	/*  Ver.419 �ֿ���ʥ�˺�����(������) */
	int AtkeeMachineHurtMul;             	/*  Ver.419 �ֿ���е�˺�����(������) */
	short AtkeeFireHurtMul;                	/*   �ֿ��������˺�����(Ԫ������) */
	short AtkeeThunderHurtMul;             	/*   �ֿ��������˺�����(Ԫ������) */
	short AtkeeLightHurtMul;               	/*   �ֿ��������˺�����(Ԫ������) */
	short AtkeeNightHurtMul;               	/*   �ֿ��������˺�����(Ԫ������) */
	int AtkeeWaterHurtMul;               	/*  Ver.419 �ֿ�ˮ�����˺�����(Ԫ������) */
	short AtkeeMagicHurtMul;               	/*   �ֿ�ħ���˺����� */
	short AtkeeHurtMul;                    	/*   �ֿ��������˺����� */
	int AtkeePhsicHurtAdd;               	/*   �ֿ����������˺�����(������) */
	int AtkeeIceHurtAdd;                 	/*   �ֿ�ħ���˺�����(������) */
	int AtkeeSiegeHurtAdd;               	/*  Ver.419 �ֿ�������ʩ�˺�����(������) */
	int AtkeeHolyHurtAdd;                	/*  Ver.419 �ֿ���ʥ�˺�����(������) */
	int AtkeeMachineHurtAdd;             	/*  Ver.419 �ֿ���е�˺�����(������) */
	int AtkeeFireHurtAdd;                	/*   �ֿ��������˺�����(Ԫ������) */
	int AtkeeThunderHurtAdd;             	/*   �ֿ��������˺�����(Ԫ������) */
	int AtkeeLightHurtAdd;               	/*   �ֿ��������˺�����(Ԫ������) */
	int AtkeeNightHurtAdd;               	/*   �ֿ��������˺�����(Ԫ������) */
	int AtkeeWaterHurtAdd;               	/*  Ver.419 �ֿ�ˮ�����˺�����(Ԫ������) */
	int AtkeeMagicHurtAdd;               	/*   �ֿ�ħ���˺����� */
	int AtkeeHurtAdd;                    	/*   �ֿ��������˺����� */
};

struct tagPressInit_ToLua
{
	unsigned char PressInitType;                   	/*   ѹ����ֵ���� Bind Macrosgroup:PRESS_INIT_TYPE,*/
	unsigned char Val1;                            	/*   ְҵ��Ѫ���ٷֱ� */
	unsigned short Val2;                            	/*   ѹ����ֵ */
};

struct tagAINextSkill_ToLua
{
	unsigned short SkillID;                         	/*   ����ID */
	unsigned short SkillLevel;                      	/*   ���ܵȼ� */
};

struct tagAINormalSkill_ToLua
{
	unsigned short SkillID;                         	/*   ����ID */
	unsigned char SkillLevel;                      	/*   ���ܵȼ� */
	int ConditionID;                     	/*  Ver.501 ��������ID Bind Macrosgroup:INSERT_SKILL_CONDITION,*/
	int ConditionVal;                    	/*  Ver.501 ��������ֵ */
	unsigned char Rate;                            	/*   ���� */
	short Once;                            	/*  Ver.501 �Ƿ�ֻ����һ�� */
	tagAINextSkill_ToLua NextSkill[MAX_NEXT_SKILL];       	/*  Ver.367 �������� */
};

struct tagAIInsertSkill_ToLua
{
	unsigned short SkillID;                         	/*   ����ID */
	unsigned short SkillLevel;                      	/*   ���ܵȼ� */
	int ConditionID;                     	/*   ��������ID Bind Macrosgroup:INSERT_SKILL_CONDITION,*/
	int ConditionVal;                    	/*   ��������ֵ */
	short Rate;                            	/*   ���� */
	short Once;                            	/*   �Ƿ�ֻ����һ�� */
};

struct tagAIRefreshedSkill_ToLua
{
	unsigned short SkillID;                         	/*   ����ID */
	unsigned short SkillLevel;                      	/*   ���ܵȼ� */
	short Rate;                            	/*   ���� */
	char Self;                            	/*  Ver.354 �ǲ��Ƕ��Լ�ʹ�� */
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
	unsigned int Rate;                            	/*   ���� */
	unsigned char TeamBootyType;                   	/*   ���ʱ�ĵ��䷽ʽ Bind Macrosgroup:MON_BOOTY_TEAM_TYPE,*/
	unsigned short Num;                             	/*   ���� */
	unsigned char DirectPakage;                    	/*   �Ƿ�ֱ�ӷŽ����� */
	unsigned char MinLevel;                        	/*   ��ô���Ʒ����С�ȼ���0��ʾ������ */
	unsigned char MaxLevel;                        	/*   ��ô���Ʒ�����ȼ���0��ʾ������ */
	unsigned char Career;                          	/*   ְҵ��0��ʾ������ */
	unsigned short TaskID1;                        
	unsigned short TaskID2;                        
};

struct tagMonBooty_ToLua
{
	unsigned int Exp;                             	/*   ����ֵ */
	tagMonBootyMoney_ToLua Moneys[MAX_MONBOOTY_MONEY];      	/*   ���� */
	tagMonBootyItem_ToLua Items[MAX_MONBOOTY_ITEM];        	/*   ��Ʒ */
	unsigned int BootyPetEgg;                     	/*  Ver.88 ��ó��ﵰ */
	int BootyPetEggRate;                 	/*  Ver.88 ��ó��ﵰ��������� */
	tagRespactEntry_ToLua Respact;                         	/*  Ver.126 ��õ����� */
};

struct tagMonColorLevel_ToLua
{
	unsigned char BlackLevel;                      	/*   С������ȼ�Ϊ���� */
	unsigned char RedLevel;                        	/*   С������ȼ������ڵ���BlackLevelΪ���� */
	unsigned char WhiteLevel;                      	/*   С������ȼ������ڵ���RedLevelΪ���� */
	unsigned char GreenLevel;                      	/*   С������ȼ������ڵ���WhiteLevelΪ���������ڵ�������ȼ�Ϊ���� */
};

struct tagMonCollect_ToLua
{
	unsigned short TaskID[MON_COLLECT_TASK_NUM];   
	unsigned short CollectOnce;                     	/*   �Ƿ��ǵ��βɼ� */
	unsigned short CollectTime;                     	/*   �ɼ�һ����Ҫ�೤ʱ�䣬��λ�� */
	unsigned short CollectArmDur;                   	/*   �ɼ�һ�����Ĳɼ����ߵ��;� */
	unsigned int CollectSkillID;                  	/*   ��Ҫ�Ĳɼ����� */
	unsigned int CollectArm;                      	/*   ��Ҫ�Ĳɼ����� */
	unsigned int CollectItem;                     	/*  Ver.61 ��Ҫ�Ĳɼ���Ʒ */
	unsigned short CollectItemDur;                  	/*  Ver.61 �ɼ�һ��������Ʒ������ */
	unsigned short CollectSecondarySkillType;       	/*  Ver.299 �ɼ���Ҫ����� */
	unsigned short CollectRecipeID;                 	/*  Ver.299 �ɼ���Ҫ������䷽ */
	int NoDecHP;                         	/*  Ver.319 �ɼ���ɼ�����Ѫ��һֱ���Բɡ� */
	int CollectType;                     	/*  Ver.400 0 ��ͨ�ɼ��1 ��е�ſ��Բɼ�����Ʒ Bind Macrosgroup:MON_COLLECT_TYPE,*/
	unsigned int CollectPackgeItem[MAX_MON_COLLECT_PACKAGE_ITEM]; 	/*  Ver.455 ��Ҫ��������ĳ����Ʒ���ɲɼ� */
	int CollectFlag;                     	/*  Ver.1246 0 �˿��Բɼ���1 �˲����Բɼ��Ĳɼ��� */
};

struct tagAberrantList_ToLua
{
	unsigned int MonsterID;                      
	unsigned short Rate;                           
};

struct tagAddMonBootyGroup_ToLua
{
	int GroupID;                        
	int CollectTime;                     	/*   �ɼ�ʱ�� 0 ��ֱ��ʰȡ */
	int Flag;                            	/*   �������ǰ�λ��: 1 �ɼ������ʱ��ֻ��ɼ������� */
};

struct tagMonsterDef_ToLua
{
	unsigned int MonsterID;                      
	char Name[RES_NAME_LEN];             
	int IconID;                          	/*  Ver.6 ͼ��id */
	unsigned int HP;                             
	unsigned int IceAtk;                          	/*   �����Թ����� */
	unsigned int IceDef;                          	/*   �����Է����� */
	unsigned int FireAtk;                         	/*   �����Թ����� */
	unsigned int FireDef;                         	/*   �����Է����� */
	unsigned int ThunderAtk;                      	/*   �����Թ����� */
	unsigned int ThunderDef;                      	/*   �����Է����� */
	unsigned int LightAtk;                        	/*   �����Թ����� */
	unsigned int LightDef;                        	/*   �����Է����� */
	unsigned int NightAtk;                        	/*   �����Թ����� */
	unsigned int NightDef;                        	/*   �����Է����� */
	unsigned int PhsicAtk;                        	/*   �������Թ����� */
	unsigned int PhsicDef;                        	/*   �������Է����� */
	unsigned int WaterAtk;                        	/*  Ver.419 ˮ���Թ����� */
	unsigned int WaterDef;                        	/*  Ver.419 ˮ���Է����� */
	unsigned int SiegeAtk;                        	/*  Ver.419 ������ʩ������ */
	unsigned int SiegeDef;                        	/*  Ver.419 ������ʩ������ */
	unsigned int HolyAtk;                         	/*  Ver.419 ��ʥ������ */
	unsigned int HolyDef;                         	/*  Ver.419 ��ʥ������ */
	unsigned int MachineAtk;                      	/*  Ver.419 ��е������ */
	unsigned int MachineDef;                      	/*  Ver.419 ��е������ */
	tagAtkerHurtChg_ToLua AtkerHurtChg;                    	/*   �˺����� */
	tagAtkeeHurtChg_ToLua AtkeeHurtChg;                    	/*   �ֿ��˺����� */
	unsigned int Hit;                             	/*   ������ */
	unsigned int Flee;                            	/*   ������ */
	unsigned char Type;                            	/*   �������� Bind Macrosgroup:MON_TYPE,*/
	char BackMul;                         	/*   �������� */
	unsigned char NotRefresh;                      	/*  Ver.15 0 ��������ˢ��1 ��������ˢ */
	unsigned char DynAttr;                         	/*  Ver.15 1 ��̬���� 0 �Ƕ�̬���� */
	unsigned int MoveSpd;                         	/*   Ѳ��ʱ���ƶ��ٶ� */
	unsigned int FightMoveSpd;                    	/*  Ver.10 ս��ʱ���ƶ��ٶ� */
	unsigned int HomeMoveSpd;                     	/*  Ver.10 ��λʱ���ƶ��ٶ� */
	int RefreshTime;                     	/*   ���������������ʱ�䣬����ʬ�������ʱ��һֱ���ڣ���λ�� */
	short DeadAddResourceMul;              	/*   ���ﱻ��ɱʱ������Դ��������ָ�� */
	tagPressInit_ToLua PressInit[MAX_PRESS_INIT];       	/*  Ver.10 ѹ����ֵ */
	unsigned int PressHurtRate;                   	/*  Ver.10 �˺�ת�������������˺�ת��Ϊһ��ѹ����0��ʾ��ת�� */
	unsigned int PressSelectAdd;                  	/*  Ver.10 ѡ��Ŀ��ʱѹ������ֵ������ʵ��Ŀ�겻�л� */
	tagAINormalSkill_ToLua AINormalSkills[MAX_NORM_SKILL];  	/*  Ver.10 ��׼���ܣ��Եз�ʹ�� */
	tagAIInsertSkill_ToLua AIAtkeeSkills[MAX_ATKEE_SKILL];  	/*  Ver.10 ������ʱ�Ĳ��뼼�ܣ����Լ�ʹ�� */
	tagAIInsertSkill_ToLua AIAtkerSkills[MAX_ATKER_SKILL];  	/*  Ver.10 ����ʱ�Ĳ��뼼�ܣ��Եз�ʹ�� */
	unsigned int ViewDist;                        	/*  Ver.10 ������Ұ����λ���� */
	unsigned int TraceDist;                       	/*  Ver.10 ����׷�Ͼ��룬��λ���� */
	unsigned char MoveType;                        	/*  Ver.10 �����ƶ����� Bind Macrosgroup:MON_MOVE_TYPE,*/
	unsigned char AtkType;                         	/*  Ver.10 ���﹥������ Bind Macrosgroup:MON_ATK_TYPE,*/
	unsigned char RandSelect;                      	/*  Ver.10 ����л�����Ŀ���ʱ�� */
	unsigned char HeavyHitRate;                    	/*  Ver.10 ������ */
	unsigned int Radius;                          	/*  Ver.18  */
	unsigned int DynMask;                         	/*  Ver.331 ��̬�赲�����ߺ�ħ�������赲 */
	unsigned int DynMaskX;                        	/*  Ver.546 ��̬�赲���Թ������ĵ�ĳ���1/2,�����0,�õ���Radius */
	unsigned int DynMaskY;                        	/*  Ver.546 ��̬�赲���Թ������ĵ�Ŀ��1/2,�����0,�õ���Radius */
	unsigned int Height;                          	/*  Ver.18  */
	unsigned char Level;                           	/*  Ver.22 ����ȼ� */
	unsigned char BirthTime;                       	/*  Ver.25 �������ʱ��,��λ�� */
	unsigned short SubType;                         	/*  Ver.27 ���������� Bind Macrosgroup:MON_SUB_TYPE,*/
	tagMonBooty_ToLua Booty;                           	/*  Ver.22 ս��Ʒ */
	tagMonColorLevel_ToLua ColorLevel;                      	/*  Ver.22 ��������ɫ�ȼ���Ϣ */
	tagMonCollect_ToLua Collect;                         	/*  Ver.44 �ɼ� */
	unsigned short RefreshRadius;                   	/*  Ver.44 ����ˢ��ʱ������뾶 */
	unsigned char MonAttrType;                     	/*  Ver.64 ������������,������,����� Bind Macrosgroup:MON_ATTR_TYPE,*/
	tagAberrantList_ToLua Aberrants[MAX_MON_ABERRANT];     	/*  Ver.118 һ�����ʱ����ĳ�ֹ��� */
	int RunAwayHP;                       	/*  Ver.123 ���ܵ�Ѫ������ֵ */
	unsigned int RunAwayTime;                     	/*  Ver.123 ���������ܵ�ʱ��,��λ�� */
	int CallHelpHP;                      	/*  Ver.123 ���ȵ�Ѫ������ֵ */
	int CallHelpDist;                    	/*  Ver.125 �ɺ��ȵķ�Χ */
	unsigned int CallHelpMonsterID[MAX_CALL_MOSTER_NUM]; 	/*  Ver.123 ���ȹ�����æ�Ĺ���id�б� */
	int HoldDistanceRandom;              	/*  Ver.123 ���־��� Զ�̹����Ĺ�����һ���ļ��ʴ����ù������һ�ξ��룬�����ڼ��ܹ����ķ�Χ֮��,����� */
	int BootyXp;                         	/*  Ver.223 ������������ɫ���ӵ�xpֵ */
	int AtkMulByCaller;                  	/*  Ver.304 ͼ���ٻ����﹥���������� �����������+����������*����, ������ͬ */
	int BootyDropType;                   	/*  Ver.338 ս��Ʒ���䷽ʽ 0 ֱ�ӵ��� 1 һ��һ���������2 ��ʬ���й������ */
	tagAIRefreshedSkill_ToLua AIRefreshedSkill;                	/*  Ver.353 ����ˢ��ʱ�ͷŵļ��� */
	unsigned char BootyViewType;                   	/*  Ver.416 ����ɼ�����0 ����ɼ� 1 ȫ���ɼ� 2 �Լ��ɼ� Bind Macrosgroup:BOOTY_DROP_VIEW_TYPE,*/
	unsigned int MiscType;                        	/*  Ver.532 λ�������� �ο�MON_MISC_TYPE Bind Macrosgroup:MON_MISC_TYPE,*/
	char CollectActDesc[MON_COLLECT_DESC_LEN]; 	/*  Ver.598  */
	tagAddMonBootyGroup_ToLua BootyGroup[MAX_MON_BOOTY_GROUP_LIST]; 	/*  Ver.620 ����ս��Ʒ�б�ID */
	int ShowType;                        	/*  Ver.1124 ������ʾ���0 �� 1 ħ������2�������3 �ɲ���ĳ��� Bind Macrosgroup:MON_SHOW_TYPE,*/
	int BossType;                        	/*  Ver.1195 ��ʱ֧��1-10,��ʾ1-10��boss��ͼ */
	int Vig;                             	/*  Ver.1378 ��������ֵ */
	unsigned char DartCarType;                     	/*  Ver.1451 �����ڳ�����|DART_CAR_TYPE */
	short GetRes;                          	/*  Ver.1559 ÿ���ܻ�ȡ������Դ|�ݵ���Դ�� */
	int ClanBooty;                       	/*  Ver.1928 Ϊ1�򰴹������ս��Ʒ�Ĺ��� */
	int Zhengyi;                         	/*  Ver.2169 ɱ��������Ի�ö�������� */
	char IsJicheng;                       	/*  Ver.2312 �����е�Ļ���1-�̳��������� */
	int ChengshuID;                      	/*  Ver.2493 ��԰ֲ������ˢ�Ĳɼ���ID */
};

struct tagLevelFengExpDec_ToLua
{
	int ExpCount;                        	/*   ��������ֵʱ��������1��ʼ��1��ʾ�ӿ�ʼ��1�������˥�� */
	int ExpRate;                         	/*   �������,100��ʾ��˥����0��ʾȫ˥�� */
};

struct tagLevelFeng_ToLua
{
	int Level;                           	/*   ��ӡ�ĵȼ� */
	int LevelFengMax;                    	/*   �ȼ���ӡ���ʱ�䣬��λСʱ */
	int LevelFengMin;                    	/*   �ȼ���ӡ���ʱ�䣬��λСʱ */
	int LevelFengTimeDec;                	/*   ÿ�ﵽһ����ӡ�ȼ����ٵ�ʱ�䣬��λ�� */
	tagLevelFengExpDec_ToLua ExpRateDec[MAX_LEV_FENG_EXPDEC]; 	/*  Ver.1820 ��ӡ�ȼ�����˥�� */
};
#define MAX_LEV_FENG_EXPDEC_TOLUA MAX_LEV_FENG_EXPDEC

#define MAX_CATEGORY_ACHIEVEMENT_TOLUA 8
/* �ɳ�֮·����� */
struct tagAchievementCategoryDef_ToLua
{
	int CategoryID;                      	/*   ���id */
	int Icon;                            	/*   ͼ��ID */
	char Name[ROLE_NAME_LEN];             	/*   ���� */
	int Level;                           	/*   Ҫ��ȼ� */
	int NextLevel;                       	/*   �¸����ĵȼ� */
	int AchievementID[MAX_CATEGORY_ACHIEVEMENT_TOLUA]; 	/*   ����id */
};

/* �ɳ�֮·���� */
struct tagAchievementCond_ToLua
{
	char Des[ACHIEVEMENT_COND_DES_LEN];   	/*   ������������ */
	int Type;                            	/*   �������� */
	int Val1;                            	/*   ����ֵ1 */
	int Val2;                            	/*   ����ֵ2 */
	int Val3;                            	/*   ����ֵ3 */
};

/* �ɳ�֮·����� */
struct tagAchievementDef_ToLua
{
	int AchievementID;                   	/*   ���id */
	int CategoryID;                      	/*   �������id */
	int Icon;                            	/*   ͼ��ID */
	int IconRate;                        	/*   ͼ�����ű���(100Ϊ����) */
	char Name[64];             	/*   ���� */
	tagAchievementCond_ToLua Cond;                            	/*   ������� */
	tagResult_ToLua Result;                          	/*   �����Ч�� */
	char GuideDes[ACHIEVEMENT_GUIDE_DES_LEN]; 	/*   �������� */
};

#pragma pack()

#define FLY_ANIM_START_INDEX 100	//���ж��� ��ʼ����
#define FLY_ANIM_PATH_INDEX  200	//�������ж��� ��ʼ����

const tagNpcDisplayInfo_ToLua&	GetNpcDisplayInfo(int id);
const tagLevelUP_ToLua*		getLevelUP( int lv );
extern int getPlayerDisplayId( int gender, int race );
const tagLevelFeng_ToLua* getLevelFeng( int lv );
const tagMonsterDef_ToLua* getMonsterDef( int id );
struct tagPlayerPosInfo
{
	int m_nPosX;	// �����m��Ϊ��λ
	int m_nPosZ;	// �����m��Ϊ��λ
};

const char* GetModePathById(int nModelId);
const char *GetSocketPathById(int nModelId);

enum
{
	PET_FIGHT_ACTIVE = 0,	// ��������ģʽ
	PET_FIGHT_PASSIVE = 1,	// ���ﱻ��ģʽ
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
	int DynValueNum;                     	/*  Ver.1140 ��̬Ч��ֵ���� */
	int DynValue[MAX_FAKE_STATUS_DYN_VALUE]; 	/*  Ver.1140 ��̬Ч��ֵ */
};

enum
{
	CLIENT_BASE_MSPD = BASE_MSPD,
};
void logPrivateInfo( const char* pMessage );
//tolua_end

// �������λ��Ч��������
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
	// ������������
	virtual void Update(unsigned int dtick);
	virtual void UpdateMin(unsigned int dtick);
	void UpdateRotateState( unsigned int dtick );
	// �������ˢ��
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
	// ����ս��
	virtual void onFight(bool bForce = true);
	// �뿪ս��״̬
	virtual void onLeaveFight();
	// ɱ���¼�
	virtual void kill(int id){}
	// ���������¼�
	virtual void cast(int id, int lv){}
	virtual bool	canAutoTalkToNpc()
	{ 
		return false; 
	}

	virtual void	endAutoTalkToNpc(){}
	// �Ƿ���ʱװ
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
	// ���һ������·��(����:indexId·��ID,bFristFly�Ƿ��Ǹ����)
	void setAutoFly( int indexId, bool bFristFly = true );		

	void LoadRes(int resid);

	void onUpdateSkill(int skillid);

	// ���õ�ǰ���������ļ���
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
	// ���ý�ֱʱ��
	void setSkillTetanic( unsigned int time );
	bool isInSkillTetanic();
	// �л�վ��
	void onChangeStand();
	// �л��ƶ�
	void onChangeMove();
	// �л���ʬ��
	void onChangeBone();
	// ��ȡͷ���߶�(�������Ծʱ�ı�߶�)
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
	// �Ժ���Ϊ��λ
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
	
	// �Ժ���Ϊ��λ
	int	getPlayerX( )
	{
		return m_Pos.x;
	}
	// �Ժ���Ϊ��λ
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
	
	// �ж��Ƿ������״̬
	bool	isInRide()
	{
		return m_ActState.getInState( GSACT_IN_RIDE );
	}
	
	// �ж��Ƿ��ڱ���״̬
	bool	isInMorph()
	{
		return m_ActState.getInState( GSACT_IN_MORPH );
	}

	
	bool	isInScene();

	bool	isInMap()
	{
		return m_pCurMap != NULL;
	}

	//�Ƿ����⸱��Ҫ����ʾͷ��
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

	// ��ȡ�����Ϣ
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
	// ���Ŷ���
	void playAction( int actId, const char* szEffect );
	// ���NPC������ʾ���
	virtual int getNpcQuestShow();
	// ��ȡ���ﵱǰ��X��Z����
	tagPlayerPosInfo& getPlayerPostion();
	// ��ȡ����
	GamePet* getPet();
	GameQiYue& getQiYue();
    // ��ȡ����
    GameTrump* getTrump();
	// ��ȡ�ر�ͼ��Ϣ
	GameCBT& getCBTInfo();

	// ��ȡ��ǰ��������id
	int		getPrepareSkill()
	{
		return m_nPrepareSkill;
	}

	int				getPrepareTime();

	int				getTotalPrepareTime();

	// ȡ������ĳ�����
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

	// �Ƿ���ʾͷ��
	bool		isShowHead();
	// �Ƿ���ʾʱװ
	bool		isShowFashion();
	// �Ƿ���ʾ����
	bool		isShowBack();
	// �Ƿ���ʾ����
	bool		isShowFace();
	// �Ƿ���ʾ���·��޹�ϵ
	bool		isShowLove();
	void	setHitTargetID( int nTargetID );

	int		getHitTargetID(){ return m_nHitTargetID ;};
	
	//�Ƿ����ʰȡ
	bool	canPickUp();
	
	// -1 ��ʾû�п��
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

	//�����е�Ƿ�����������	param �����е
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

	// �ж��Ƿ�Ϊ�ɼ��� 2012.04.11
	//tolua_begin
	bool    isCollectMachine( );
	const char* getActorWorldName();
	//tolua_end
	void setActorWorldName( const char* szName );

	// ʹ����Ʒ����
	void startUseItemChant( unsigned int chantTime, int nItemID, int nTargetID, int nListType = -1, int nGridIdx = -1 );
	// �ٻ���������
	void startTrumpChant( int nHeighId, int nLowId, int nGridIdx );
	// �ٻ���������
	void startRideChant( int nRideId, int nLevel );
	// �ٻ���������
	void startPetChant( tdr_longlong wid );
	// ���������
	void startSecondaryChant( unsigned int uChantTime, int nType, int nId);
	//�ֽ�����
	void startResolveChant( int nListType, int nGridIdx, tdr_ulonglong Wid, int nType );
	//���ݵ�ǰ����״̬�ж��Ƿ���Կ�ʼ����
	bool canStartChant( int nType );

	void	setChantData( IChant_Base* pChant );
	void	requestBreakChant( int nType );
	void	finishChant( int nType );
	void	onBreakChant( int nType );
	bool	isInPre( int nType );
	// ��ϵ�ǰ������
	void	BreakCurChant();

	// �Ƿ������������ڳ�
	bool	isPolanCar();
	void	setDartCarType( unsigned char type );
	// ȡ������
	void	requestCancelShamDead();
	//����������ʾ
	void	setBannerInfo();
	//���������� ��·��Ч
	void	SetFootEffect( const GameXYZ &pos ,int nStar = 9 );
	// ���������� ��Ч
	void	setPerfectEffect();
	//����ʧ��ͼ��Ч��
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
	// ע���¼�����
	void	registerTickEvent( GameActor_Tick* pTick );
	// �鿴�¼��Ƿ�����
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
	int					m_nShowFlag;				// ͷ��ʱװ�Ƿ���ʾ+���·��޹�ϵ�Ƿ���ʾ
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
	// ����Ӱ
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

	GameXYZ						m_Pos;					// �����mm��Ϊ��λ
	Ogre::ShowActor*			m_pShowActor;
	GameMap*					m_pCurMap;				// ��ǰ��ͼ
	GameState					m_ActState;				// ����״̬
	RoleText					m_RoleText;				// ��ɫͷ��������Ϣ
	int							m_SelectTargetId;		// ѡ�ж���ʵ��Id
	GameActor_Tick_InFightTime* m_pFightTime;			// ����ս��
	
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
	unsigned int				m_skillTetanicTime;	// ������ֱ����ʱ��
	
	GameActor_Tick_MainPlayerFree*	m_pPlayerFreeTime;	//������м�ʱ
	std::list<IActorDeathAction *>	m_lstDeathAction;
	tagPetDiathesis_ToLua			m_petDiathesisInfo;

	int							m_nChatActionID;
	unsigned int				m_showWeaponTime;
	bool						m_isShowWeapon;
	bool						m_optimizeShow;
	bool						m_optimizeClear;
	bool						m_mustUpdateUI;

	float m_fJumpTime;
	bool m_bStartJump; // ������

	bool m_bStartJump1; // ����Ԥ���������ˣ���������

	int m_RollType;	//������Ʒ��roll�㷽ʽ
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
	int CollectSkill_SlotMax;            	/*   ���������ܲ۵�����,�ű�����,�츳Ӱ�� */
	int CollectSkill_SlotCurr;           	/*   ���������ܲ۵ĵ�ǰֵ */
	int CollectSkill_AddType;            	/*   ���������ܲ۵����ӷ�ʽ,1�������� 2�������� 3ս��ʱ����� */

	int m_CollectTime;

	int								m_nPetGenius;
	int								m_nPetGeniusId;
	int								m_nPetGenniusLv;
	bool							m_isHeti;
	int								m_nPetStrengthenLevel;

	GameActor_PKEffect				m_pkEffect;

	tagNetBarInfo_ToLua				m_NetBarInfo;

	unsigned char					m_DartCarType;		//���˳�����

	private:
	std::vector<UIObject*>			m_vecActorBindFrames;
	UIObject*							m_stickyFrame;
	IChant_Base*					m_pChantPart;		//����ģ��

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