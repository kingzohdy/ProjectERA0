#pragma once
#include "proto_comm.h"
#include "resdb_meta.h"
#pragma pack(1)
enum EARM_TYPE
{
	ARM_FIGHTER		= 10,		// սʿ����
	ARM_WIZARD		= 11,		// ��ʦ����
	ARM_GUN			= 12,		// ǹ��-ǹ
	ARM_CANNON		= 13,		// ǹ��-��
	ARM_TEMP		= 14,		// ��ʱ����
	ARM_CHURCH		= 15,		// ��ʦ����

	EQUIP_CLOSE		= 20,		// �·�
	EQUIP_HAT		= 21,		// ñ�� 
	EQUIP_SHOULDER	= 22,		// ����
	EQUIP_BACK		= 23,		// ����
	EQUIP_FACE		= 24,		// ����
	EQUIP_ACCOUTER	= 25,		// ��Ʒ
	EQUIP_FASHION	= 30,		// ʱװ

	BARDAIN_HEAR	= 31,		// ����
	BARDAIN_FACE	= 32,		// ����

	ITEM_TOOLS		= 50,
	ITEM_EXPEND		= 51,
	ITEM_STUFF		= 52,
	ITEM_TASK		= 53,
};

//tolua_begin

#define MAX_OTHER_MONEY_USE_CLIENT MAX_OTHER_MONEY_USE

struct tagItemData
{
	ROLEITEM			m_Item;	
	ROLEARM				m_Arm;
};

struct tagAddAttrInst_ToLua
{
	unsigned short AddAttrID;                       	/*   �ؼ��ֶ���ID */
	unsigned short Val;                            
};

struct tagExtendedResult_ToLua
{
	unsigned short ResultID;                       
	char Res1[2];
	short ResultVal1;
	char Res2[2];
	int ResultVal2;
	int ResultVal3;
	int ResultVal4;
};

struct tagItemMapInfo_ToLua
{
	int MapID;                           	/*   ��ͼID */
	int X;                               	/*   ����X */
	int Y;                               	/*   ����Y */
};

struct tagRoleItem_ToLua
{
	int DefID;                           	/*   ��ƷID */
	int DefPos;                          	/*   �������ӿ�����õ����� */
	unsigned short Num;                             	/*   ��Ʒ���� */
	short GridIdx;                         	/*   ������������,��0��ʼ */
	int InstFlag;                        	/*  Ver.81 ʵ����, ��ȫ���ȱ�־ */
	int ValidEnd;                        	/*  Ver.50 ���ĵ�����Ч����ʱ�� */
	tdr_ulonglong WID;                             	/*   ��ƷΨһ��ʶ, �뱣֤���ֶκ�֮ǰ���ֶ���RoleArm��ȫ��ͬ */
	int Flag;                            	/*  Ver.52 ���ڴ洢��Ҫ���ڵ����е����ݣ������Ե��߶��� */
	int InstVal1;                        	/*  Ver.108 �洢ʵ����Ϣ,�ӵ��߶���,�ɶ��ʹ�õĵ���UseMaxDur >0 ����InstVal1Ϊ��ǰʣ��ʹ�ô��� */
	int InstVal2;                        	/*  Ver.108 �洢ʵ����Ϣ,�ӵ��߶��� */
	int InstVal3;                        	/*  Ver.173 �洢ʵ����Ϣ,�ӵ��߶��� */
	char ItemMapNum;                      	/*  Ver.1424 ����ĵ�ͼλ����Ϣ���� */
	tagItemMapInfo_ToLua ItemMapInfo[MAX_SAVE_MAP_NUM];   	/*  Ver.1424 �����λ����Ϣ */
};
struct tagBaseAttrInfo_ToLua
{
	unsigned short ResultID;                       
	short Mul;                            
};

struct tagArmStarInfo_ToLua
{
	unsigned char ValIdx;                          	/*   ǿ������ֵ�ڱ���е����� */
};

struct tagArmSkillOne_ToLua
{
	char Idx;                   
	unsigned char Lock;                            	/*  Ver.2501  */         
	short ID;                             
};

struct tagArmSkillData_ToLua
{
	unsigned char Max;                            
	unsigned char Num;                            
	tagArmSkillOne_ToLua One[MAX_ARM_SKILL_NUM];         
};

struct tagRoleArm_ToLua
{
	int DefID;                           	/*   ��ƷID */
	int DefPos;                         
	unsigned short Num;                             	/*   ��Ʒ���� */
	short GridIdx;                         	/*   ������������,��0��ʼ */
	int InstFlag;                        	/*   ʵ����, ��ȫ���ȱ�־ */
	int ValidEnd;                        	/*  Ver.8 ��Ч����ʱ�䣬������װ */
	tdr_ulonglong WID;                             	/*   ��ƷΨһ��ʶ, �뱣֤���ֶκ�֮ǰ���ֶ���RoleItem��ȫ��ͬ */
	unsigned int Dur;                             	/*  Ver.8 װ����Ķ� */
	unsigned char StarLv;                          	/*  Ver.74 Ʒ��(��)��������: MAX_ARM_STAR_LEVEL */
	unsigned char FightLv;                         	/*  Ver.74 ս������������: MAX_ARM_FIGHT_LEVEL */
	unsigned char HoleNum;                         	/*  Ver.74 �׸�������: MAX_ARM_HOLE */
	HOLEGEM HoleGem[MAX_ARM_HOLE];           	/*  Ver.138 ���б�ʯ */	
	unsigned char AddAttrNum;                      	/*  Ver.137 �ؼ��ֵĸ�������: MAX_ARM_ADD_ATTR */
	tagAddAttrInst_ToLua AddAttr[MAX_ARM_ADD_ATTR];       	/*  Ver.137 װ���Ĺؼ��� */
	tagExtendedResult_ToLua ExtendedResult;                  	/*  Ver.321 ��ħЧ������ */
	int ExtendedFromDefID;               	/*  Ver.324 ��ʲô���߸�ħ */
	tagAddAttrInst_ToLua KeYinAddAttr;                    	/*  Ver.546 �����ӡ���� */
	int KeYinEnd;                        	/*  Ver.546 �����ӡ����ʱ�� */
	tdr_ulonglong FengYinID;                       	/*  Ver.942 ��˭��ӡ */
	unsigned char BaseAttrNum;                     	/*  Ver.1143 �����������Եĸ�������: MAX_ARM_BASE_ATTR */
	tagBaseAttrInfo_ToLua BaseAttr[MAX_ARM_BASE_ATTR];     	/*  Ver.1143 �������Ը���ֵ */
	tagArmStarInfo_ToLua StarInfo[MAX_ARM_STAR_LEVEL];    	/*  Ver.1183 ǿ����ֵ */
	unsigned char WashAttrNum;                     	/*  Ver.1226 �ؼ��ֵĸ�������: MAX_ARM_ADD_ATTR */
	tagAddAttrInst_ToLua WashAddAttr[MAX_ARM_WASH_ADD_ATTR]; 	/*  Ver.1226 ϴ���ؼ��� */
	int StrengthenCount;                 	/*  Ver.1671 ǿ������ */
	unsigned short FengyinFlag;                     	/*  Ver.1751 ��λ�ķ�ӡ��� */
	unsigned short WashNum;                         	/*  Ver.1954 ����ϴ��ʯ���� */
	unsigned char Step;                            	/*  Ver.2152 װ���� */
	tagArmSkillData_ToLua ArmSkill;                        	/*  Ver.2168 װ������ */
	unsigned char StepUse;                         	/*  Ver.2205  */
	int UnrealBackID;                    	/*  Ver.2428 �û�����ID */
	int UnrealBackTmEnd;                 	/*  Ver.2428 �û����� */
};

struct tagWearPre_ToLua
{
	unsigned char RoleLevelMin;                   
	unsigned char RoleLevelMax;                   
	unsigned char Career;                         
	unsigned char DiathesisID;                    
	unsigned char Gender;                         
	int GemOpenHoleMoney;                	/*  Ver.437 װ�����׵�Ǯ */
	int GemInlayMoney;                   	/*  Ver.437 װ����Ƕ��ʯ��Ǯ */
	short PKVal;                           	/*  Ver.970 pkֵҪ�� */
	char ClanLimit;                       	/*  Ver.1036 0-������ 1-ɳ�Ϳ��ϴ� 2-ɳ�Ϳ˳�Ա */
	unsigned short GeniusLimit;                     	/*  Ver.1910  */
	int SchoolValLimit;                  	/*  Ver.2095  */
};

struct tagResult_ToLua
{
	unsigned short ResultID;                        	/*    Bind Macrosgroup:RESULT,*/
	short ResultVal1;                     
	int ResultVal2;                     
	int ResultVal3;                     
	int ResultVal4;               
};

struct tagArmDur_ToLua
{
	unsigned int MaxDur;                          	/*   ����;ö� */
	unsigned int FightStatusDur;                  	/*   ս��״̬ÿ�������;� */
};

struct tagIdentifiableItem_ToLua
{
	unsigned int ItemID;                          	/*   ���������ƷID */
	unsigned short ItemRate;                        	/*   ���������Ʒ���� */
};

struct tagAddAttrList_ToLua
{
	unsigned int ID;                              	/*   װ���ؼ���ID */
	unsigned char Idx;                             	/*   װ���ؼ���Ч������ */
};

struct tagRespactEntry_ToLua
{
    unsigned int ID;                             
    unsigned int Value;                          
};

struct tagRndHole_ToLua
{
    unsigned char RndRed;                         
    unsigned char RndBlue;                        
    unsigned char RndGreen;                       
    unsigned char RndColor;                       
};

struct tagItemTmp_ToLua
{
	unsigned char Type;                            	/*    Bind Macrosgroup:ITEM_TMP_TYPE,*/
	unsigned char SkillLevel_1;                    	/*   ��ʱ���ܵȼ� */
	unsigned short SkillID_1;                       	/*   ��ʱ����ID */
	unsigned short SkillLevel_2;                    	/*   ��ʱ���ܵȼ� */
	unsigned short SkillID_2;                       	/*   ��ʱ����ID */
	unsigned short SkillDefaultCollectNum;          	/*   ��ʱ����ȱʡ������ */
	short SpeedMul;                        	/*   �ƶ��ٶȳ����� */
	short SpeedAdd;                        	/*   �ƶ��ٶȼ����� */
	unsigned short RefreshTime;                     	/*   ��ˢʱ�䣬��λ�룬0 ��ʾ����ˢ */
	unsigned short StayTime;                        	/*   ����ʱ�䣬��λ�룬0 ��ʾһֱ���� */
	unsigned short MachineEnerge;                   	/*   ��ʱ��е����ֵ */
	unsigned char EnergeDec;                       	/*   ��ʱ��е����ֵÿ����ʱ����ٵ��� */
	char HurtDefMul;                      	/*   �����˺������� */
	short HurtDefAdd;                      	/*   �����˺������� */
	int FleeAdd;                         	/*  Ver.161 ������������ */
};

struct tagMoneyUse_ToLua
{
	int ID;
	int Val;
};

struct tagItemDef_ToLua
{
	unsigned int ItemID;                          	/*   ����id */
	char Name[RES_NAME_LEN];              	/*   ������ */
	int IconID;                          	/*  Ver.6 ͼ��id */
	int ModeID;                          	/*  Ver.22 ģ������|��npcdisplayinfo�е����� */
	int ExchgItemID;                     	/*  Ver.96 �һ�����ID|���ֶ������Ϊ��,BuyPrice�ֶ�Ϊ�һ��������� */
	unsigned int SellPrice;                       	/*   �����۸�|�����̵�npcֵ����Ǯ */
	unsigned int BuyPrice;                        	/*   ����۸�|��npc����Ҫ����Ǯ */
	unsigned char MoneyType;                       	/*  Ver.34 �������� Bind Macrosgroup:MONEY_TYPE,*/
	unsigned char ItemType;                        	/*   ��������|����Ҫ��д Bind Macrosgroup:ITEM_TYPE,*/
	unsigned short MaxPile;                         	/*   �ɶѵ��������|0��1����ʾ���ɶѵ� */
	tagResult_ToLua Result[MAX_ITEM_RESULT];         	/*  Ver.8 ����Ч�� */
	tagArmDur_ToLua ArmDur;                          	/*  Ver.8 װ���;�|0��ʾû���;ö� */
	unsigned int ArmFight;                        	/*  Ver.8 װ��ս����|��ʱ�ϳ� */
	unsigned short SuitSetID;                       	/*  Ver.8 ������װ��ID */
	unsigned short ValidTime;                       	/*   ������Чʱ��|�ӻ�ÿ�ʼ����Чʱ�䣬��λСʱ */
	unsigned short NumType;                         	/*   ���߱��|����Ҫ��д��item id �ı��ǰ3λ����ϸ����Ź��� */
	unsigned char NoDrop;                          	/*   ��������|0 �ɶ��� 1 ���ɶ��� */
	unsigned char ArmBind;                         	/*   ������ Bind Macrosgroup:ITEM_BIND_TYPE,*/
	unsigned char ExchgLevel;                      	/*   ��ͨ�ȼ�|ί����Ʒ����ں��Ѽ���ͨ Bind Macrosgroup:BUDDY_GRADE,*/
	unsigned char Important;                       	/*   ��Ҫ�̶�|װ��ʹ�ô��ֶ���Ϊ��װ��װ�ȵ��ж�:1�� 2�� 3�� 4�� */
	short AmmAtk;                          	/*   ��ҩ����������|�ϳ� */
	unsigned char WeaponAmmNeed;                   	/*   �����赯ҩ����|�ϳ� */
	unsigned char WeaponAmmType;                   	/*   ������ҩ����|�ϳ�,�������ʹ��ʲô���͵ĵ�ҩ */
	unsigned char AmmType;                         	/*   ��ҩ����|�ϳ� */
	tagWearPre_ToLua WearPre;                         	/*  Ver.8 װ������ */
	unsigned char WearPosNum;                      	/*   װ������ */
	unsigned char WearPos1;                        	/*   װ����λ Bind Macrosgroup:WEAR_POS,*/
	unsigned char WearPos2;                        	/*   װ����λ Bind Macrosgroup:WEAR_POS,*/
	unsigned char WearPos3;                        	/*   װ����λ Bind Macrosgroup:WEAR_POS,*/
	char ItemDesc[DESC_LEN];              	/*   �������� */
	tagItemTmp_ToLua ItemTmp;                         	/*  Ver.26 ��ʱϵͳ */
	unsigned int RepairBull;                      	/*  Ver.34 װ���������|�󶨽�� */
	short DamageMul;                       	/*  Ver.35 �ƻ��������� */
	unsigned short DamageAdd;                       	/*  Ver.35 �ƻ��������� */
	int SelfCool;                        	/*  Ver.30 ��Ʒ������ȴ|��λ100ms */
	int SpeCool;                         	/*  Ver.30 ��Ʒָ����ȴ|��λ100ms */
	unsigned char SelfSpeCoolType;                 	/*  Ver.30 ����ָ����ȴ���� */
	unsigned char SpeCoolType;                     	/*  Ver.30 ָ����ȴ���� */
	unsigned char NoNpcSell;                       	/*  Ver.38 ������|0 ��������npc��1 ��������npc */
	unsigned char NoDeal;                          	/*  Ver.38 ���ܽ���|0 ��Ҽ�ɽ��ף�1 ���ɽ��� */
	unsigned char ItemScript;                      	/*  Ver.47 ���߽ű���־|����Ҫ��д */
	unsigned char MapItem;                         	/*  Ver.51 ��ͼ����|1 �뿪��ͼ�ض���ʧ */
	unsigned char ArmType;                         	/*  Ver.52 ����װ��|ʹ�ô��ֶ����Ʋ��ܴ�ͬ���ͣ�0��ʾ���˴���װ�� */
	unsigned char UseTarget;                       	/*  Ver.52 ����ʹ�ö������� Bind Macrosgroup:USE_TARGET_TYPE,*/
	short TrustContributeVim;              	/*  Ver.65 ί����Ʒ�������ľ��� */
	unsigned short GainExp;                         	/*  Ver.65 ��Ʒ������ָ�� */
	short TrustOrderVim;                   	/*  Ver.66 ί�йҵ����ľ��� */
	unsigned short ContributeBuddyVal;              	/*  Ver.67 ί�����ͻ���Ѻö� */
	unsigned int NextItemID;                      	/*  Ver.78 ��һ�ȼ���ƷID| 0���޷����� */
	unsigned int NextItemMoney;                   	/*  Ver.78 �����ȼ����Ľ�Ǯ */
	unsigned char NextItemLevel;                   	/*  Ver.78 ��һ�ȼ���Ʒ����|��д1-12,�����������ݶ�Ӧ������ʯ������Ϣ, */
	unsigned char IdentifiabilityType;             	/*  Ver.78 ��������|0������� Bind Macrosgroup:IDENTIFIABILITY_TYPE,*/
	unsigned int IdentifiabilityMoney;            	/*  Ver.78 �������Ľ�Ǯ */
	tagIdentifiableItem_ToLua IdentifiabilityItems[MAX_IDENTIFIABILITY_ITEM]; 	/*  Ver.78 ���������Ʒ�� */
	unsigned char StarLimit;                       	/*  Ver.78 װ��Ʒ����������|0��������Ʒ�� */
	tagAddAttrList_ToLua AddAttr[MAX_ITEM_ADD_ATTR];      	/*  Ver.104 װ���ؼ��� */
	int ItemLevel;                       	/*  Ver.107 ��Ʒ�ȼ� */
	unsigned char ItemStar;                        	/*  Ver.110 ��Ʒ��ʼ�� */
	unsigned short ItemUsePre;                      	/*  Ver.123 ��Ʒʹ�ö���ʱ�� */
	tagRespactEntry_ToLua NeedRespact;                     	/*  Ver.126 ��ȡ��������Ҫ�� */
	tagRndHole_ToLua Hole[MAX_ARM_HOLE];              	/*  Ver.135 װ�������|��һ���ʾ��ʼ����ף��ڶ����ʾƷ������3�������������6�������������9����� */
	unsigned char RandAddAttr;                     	/*  Ver.146 �Ƿ������ùؼ���| 0 �����, 1 �����ùؼ��� */
	unsigned int UseMaxDur;                       	/*  Ver.150 ��ʹ�ô��� */
	unsigned int MaxUseOneDay;                    	/*  Ver.169 ÿ���ʹ�ô��� */
	unsigned int UseTargetID;                     	/*  Ver.192 ��ƷҪ��ʹ��Ŀ��(NPC����MONSTER  ����ID) */
	char UseModel[RES_PATH_LEN];          	/*  Ver.217 ʹ����Ʒ��ʱģ�� */
	int UseActID;                        	/*  Ver.217 ʹ����Ʒ��ʱ����ID  */
	char BattleNoUse;                     	/*  Ver.276 �Ƿ���ս������ʹ��|0-��  1-����ʹ�� */
	char IsReStoreItem;                   	/*  Ver.289 �Ƿ�ָ������0-��  1-�� */
	int ExtendedType;                    	/*  Ver.320 ��ħ����|0���Ǹ�ħ��Ʒ 1װ��Ч�� 2�ؼ���Ч�� Bind Macrosgroup:EXTENDED_TYPE,*/
	int DropSelfView;                    	/*  Ver.338 ����ֻ�������߿ɼ� */
	int ModelIndex;                      	/*  Ver.355 ������Ϊ��ʾ����İ��� */
	int NoResolve;                       	/*  Ver.384 ���ɷֽ�ı�ǣ� 1 ���ɷֽ� */
	int DieDropFlag;                     	/*  Ver.442 ���������ǣ� 0 ��������ǲ����п��ܵ��䡣1 �����ض����� Bind Macrosgroup:DIE_DROP_TYPE,*/
	int AddAttrGroupID;                  	/*  Ver.462 �ؼ��������ID */
	int CtrlFlag;                        	/*  Ver.480 ��Ʒ��ǰ�λ��: 1 ս��״̬������ʹ����Ʒ */
	int ClanContri;                      	/*  Ver.701 ������Ҫ���ٹ��ṱ�׶�|��������ֶ�,��������ʧЧ */
	int MaxUseDist;                      	/*  Ver.809 ��Ʒ���ʹ�þ��� */
	unsigned int PoseID;                          	/*  Ver.984 ʹ����Ʒ��,ͨ������Э��ı������̬ */
	unsigned char BuyLimitFlag;                    	/*  Ver.1029 ��ҹ������Ʒ����|λ����|1-���������� */
	tagMoneyUse_ToLua MoneyUses[MAX_OTHER_MONEY_USE];  	/*  Ver.1061  */
	int NeedSpartaFight;                 	/*  Ver.2013 ������Ҫ������ս���� */
	int SkillNum;                        	/*  Ver.2313 װ���ض��������������� 0-��� */
	unsigned short TeshuID;                         	/*  Ver.2390 װ����������ID����ӦArmTeshuDef */
};

struct tagSuitAttr_ToLua
{
	unsigned int SuitAttrID;                      	/*   ��������ID */
	unsigned int MinSuitNum;                      	/*   ���ٺ�����װ��װ�������� */
};

struct tagSuitSet_ToLua
{
	unsigned int SuitSetID;                       	/*   ��װID */
	int SuitSetType;                     	/*  Ver.1192 ��װ��ɫ���� */
	char Name[RES_NAME_LEN];             
	tagSuitAttr_ToLua SuitAttr[MAX_SUIT_ATTR];        
};

struct tagStarAttr_ToLua
{
	unsigned int SuitAttrID;                      	/*  Ver.641 ��������ID */
};

struct tagAddAttrBaseDef_ToLua
{
	 int Level;                          
    unsigned int PhsicAtkBase;                    	/*   ������������ֵ */
    unsigned int IceAtkBase;                      	/*   ħ������������ֵ */
    unsigned int PhsicDefBase;                    	/*   �������������ֵ */
    unsigned int IceDefBase;                      	/*   ħ������������ֵ */
    unsigned int MaxHPBase;                       	/*   ���Ѫ������ֵ */
    unsigned int MaxMPBase;                       	/*   ���ħ������ֵ */
    unsigned int HitRateBase;                     	/*   ���л���ֵ */
    unsigned int FleeBase;                        	/*   ���ܻ���ֵ */
    unsigned int DefPhsicHurtBase;                	/*   �����Ի���ֵ */
    unsigned int DefIceHurtBase;                  	/*   ħ�����Ի���ֵ */
    unsigned int LiLiang;                         	/*  Ver.462 ���� */
    unsigned int Zhili;                           	/*  Ver.462 ���� */
    unsigned int TiZhi;                           	/*  Ver.462 ���� */
    unsigned int FangHu;                          	/*  Ver.462 ���� */
    unsigned int JingShen;                        	/*  Ver.462 ���� */
    unsigned int MinJie;                          	/*  Ver.462 ���� */
    unsigned int HeavyHit;                        	/*  Ver.462 �����ȼ� */
    unsigned int DefHeavyHit;                     	/*  Ver.462 �����ֿ� */
    int WaterAtk;                        	/*  Ver.462 ˮ���� */
    int WaterDef;                        	/*  Ver.462 ˮ���� */
    int FireAtk;                         	/*  Ver.462 �𹥻� */
    int FireDef;                         	/*  Ver.462 ����� */
    int ThunderAtk;                      	/*  Ver.462 �׹��� */
    int ThunderDef;                      	/*  Ver.462 �׷��� */
    int LightAtk;                        	/*  Ver.462 �⹥�� */
    int LightDef;                        	/*  Ver.462 ����� */
    int NightAtk;                        	/*  Ver.462 ������ */
	int NightDef;                        	/*  Ver.462 ������ */
	int IgnoreDef;                       	/*  Ver.853 �Ʒ��ȼ� */
	int ReflectHP;                       	/*  Ver.853 ����ȼ� */
	int DefHP;                           	/*  Ver.853 �ֿ��ȼ� */
};

struct tagFixedAddAttr_ToLua
{
    unsigned int ID;                              	        /*   ID */
    tagAddAttrList_ToLua AddAttrs[MAX_ITEM_ADD_ATTR];     	/*   �ؼ���ID������ */
};

struct tagArmHideAttrDef_ToLua
{
    unsigned short Career;                          	/*   ְҵ */
    unsigned short ArmStarLevel;                    	/*   װ���Ǽ� */
    tagResult_ToLua Result[MAX_ARM_HIDE_ATTR_RESULT]; 	/*  Ver.1189  */
};
struct tagStarMulInfo_ToLua
{
	int Mul;                            
	int Rate;                           
};

struct tagArmStarDef_ToLua
{
	int Star;                           
	tagStarMulInfo_ToLua StarInfo[MAX_STAR_MUL_LIST];    
	int PerfectRate;                     	/*  Ver.1672  */
	int PerfectNum;                      	/*  Ver.1672  */
	int MoveMoneyVal;                    	/*  Ver.1926  */
	int MustItemID;                      	/*  Ver.2115 �ض��������� */
};

struct tagArmGradeDef_ToLua
{
	short ArmType;                         	/*   �������� Bind Macrosgroup:ARM_TYPE,*/
	short ArmLevel;                        	/*  Ver.107 �����ȼ� */
	tagStarAttr_ToLua ArmStar[MAX_ARM_STAR_LEVEL];     	/*   װ��Ʒ��Ӱ���������� */
};

struct tagAttrResult_ToLua
{
	unsigned short ResultID;                        	/*    Bind Macrosgroup:RESULT,*/
	short ResultVal1;                     
	int ResultVal2;                     
	int ResultVal3;                     
	int ResultVal4;               
};

struct tagAddAttrDef_ToLua
{
	unsigned int AddAttrID;                       	/*   ��������ID */
	char AddAttrDesc[SHORT_DESC_LEN];     	/*  Ver.54 �ؼ������� */
	unsigned short EffectSkillID[MAX_ADDATTR_SKILL]; 	/*  Ver.69 Ӱ�켼��ID */
	tagAttrResult_ToLua Result[MAX_ADDATTR_RESULT]; 
};

struct tagArmResult_ToLua
{
	int ResultVal1;                     
	int ResultVal2;                     
	int ResultVal3;                     
	int ResultVal4;                     
};

struct tagArmAddAttrDef_ToLua
{
	unsigned int AddAttrID;                       	/*    Bind Macrosgroup:ARM_ADD_ATTR_RESULT,*/
	char AddAttrName[NAME_LEN];           	/*   �ؼ������� */
	tagArmResult_ToLua Result[MAX_ARM_ADD_ATTR_RESULT];
};

struct tagAddAttrRangeInfo_ToLua
{
	int MinMul;                          	/*   ��������ٷֱ�:��С */
	int MaxnMul;                         	/*   ��������ٷֱ�:��� */
	int MidMul;                          	/*  Ver.1954 ��������ٷֱ�:�� */
};

struct tagAddAttrWearPosLimit_ToLua
{
	int RightHand;                       	/*   ����0Ϊ�˲�λ�����ָùؼ��֣�>0ʱ�ؼ��ֵ���ֵ */
	int Head;                            	/*   ͷ */
	int Body;                            	/*   �·� */
	int LeftHand;                        	/*   ���� */
	int Face;                            	/*   �沿  */
	int Shoulder;                        	/*   �� */
	int Shoe;                            	/*   Ь�� */
	int Amulet;                          	/*   ����� */
	int Ring;                            	/*   ��ָ */
	int NeckLace;                        	/*   ���� */
	int Back;                            	/*  Ver.2086 ���� */
	int Badge;                           	/*  Ver.2086 ���� */
};

struct tagAddAttrCareerLimit_ToLua
{
	int None;                            	/*   ��ְҵ���� */
	int Warrior;                         	/*   սʿ */
	int Magicor;                         	/*   ��ʦ */
	int Guner;                           	/*   ǹ�� */
	int Church;                          	/*   ��ʦ */
	int SingleHandWarrior;               	/*   �ػ��� */
	int DoubleHandWarrior;               	/*   ������ */
	int SingleHandMagicor;               	/*   ������ */
	int DoubleHandMagicor;               	/*   ����ʦ */
	int SingleHandGuner;                 	/*   ��ɱ�� */
	int DoubleHandGuner;                 	/*   ������ */
	int SingleHandChurch;                	/*   ʥ���� */
	int DoubleHandChurch;                	/*   Ԧ��ʦ */
};

struct tagNewAddAttrDef_ToLua
{
	int ID;                             
	int AddAttrResultID;                
	char AddAttrDesc[DESC_LEN];          
	int Val1[MAX_ADD_ATTR_VAL_LEVEL];   
	int Val2;                           
	int Val3;                           
	int Val4;                           
	tagAddAttrRangeInfo_ToLua RangeList[MAX_ADD_ATTR_RAND_LEVEL];
	tagAddAttrWearPosLimit_ToLua WearPosLimit;                   
	tagAddAttrCareerLimit_ToLua CareerLimit;                    
	int MinLevel;                       
	int MaxLevel;                       
	int Important;
	float Score;                           	/*  Ver.1337 װ������ֵ */
	char NotSpar;                         	/*  Ver.1765 1-���ܲ�����ʯ */
	int Main;                            	/*  Ver.2164 �Ƿ�������| */
};

struct tagAddAttrRandList_ToLua
{
	int AddAttrNum;                      	/*   �ؼ������� */
	int Rate;                            	/*   ���ʰٷ��� */
};

struct tagAddAttrRandDef_ToLua
{
	short ArmLevel;                       
	short Important;                      
	tagAddAttrRandList_ToLua RandList[MAX_ADD_ATTR_RAND_LIST];
	int ValLevel;                        	/*   ��ֵ�ȼ���NewAddAttrDef->val1 �ڼ��� */
	int AddMulLevel;                     	/*   ������ֵ����ȼ�NewAddAttrDef->RangeList */
};

struct tagItemResultDesc_ToLua
{
	int ResultID;                       
	char ResultDesc[DESC_LEN];           
};

struct tagBaoXiangItem_ToLua
{
	int ItemID;                         
	int Num;                            
	int Rate;  
	int Bind;                            	/*  Ver.2001 0�ǰ󶨣�1�� */
	int Broadcast;                       	/*   ���Ϊ1ʱ��ô���Ʒȫ�������㲥 */
};

#define MAX_BAOXIANG_ITEM_TOLUA MAX_BAOXIANG_ITEM
#define MAX_BAOXIANG_DISPLAY_ITEM_TOLUA MAX_BAOXIANG_DISPLAY_ITEM

struct tagDisplayItem_ToLua
{
	int ItemID;                         
	int Num;                            
};
struct tagBaoXiangDef_ToLua
{
	int ID;                             
	tagBaoXiangItem_ToLua Items[MAX_BAOXIANG_ITEM];       
	int HideItemID;                      	/*   ����������ƷID */
	int HideItemNum;                     	/*   ����������Ʒ���� */
	int HideCount;                       	/*   ��Ҫȫ���翪�����ٸ��˱���ʱ����������� */
	int NeedItemID;                      	/*  Ver.2001 ������һ��ƷID */
	char ErrorTip[SHORT_DESC_LEN];        	/*  Ver.2001 ȱ����һ��Ʒʱ�Ĵ�����ʾ */
	tagDisplayItem_ToLua ShowItems[MAX_BAOXIANG_DISPLAY_ITEM]; 	/*  Ver.2001  */
};

enum
{
	CONTAINER_LOCK_STATE_NO,	//����û����
	CONTAINER_LOCK_STATE_LOCK,	//������ס
	CONTAINER_LOCK_STATE_OPEN,	//�����ѽ���
};
#pragma pack()
//tolua_end