#pragma once
#include "proto_comm.h"
#include "resdb_meta.h"
#pragma pack(1)
enum EARM_TYPE
{
	ARM_FIGHTER		= 10,		// 战士武器
	ARM_WIZARD		= 11,		// 法师武器
	ARM_GUN			= 12,		// 枪手-枪
	ARM_CANNON		= 13,		// 枪手-炮
	ARM_TEMP		= 14,		// 临时武器
	ARM_CHURCH		= 15,		// 牧师武器

	EQUIP_CLOSE		= 20,		// 衣服
	EQUIP_HAT		= 21,		// 帽子 
	EQUIP_SHOULDER	= 22,		// 肩饰
	EQUIP_BACK		= 23,		// 背饰
	EQUIP_FACE		= 24,		// 面饰
	EQUIP_ACCOUTER	= 25,		// 饰品
	EQUIP_FASHION	= 30,		// 时装

	BARDAIN_HEAR	= 31,		// 发型
	BARDAIN_FACE	= 32,		// 脸谱

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
	unsigned short AddAttrID;                       	/*   关键字定义ID */
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
	int MapID;                           	/*   地图ID */
	int X;                               	/*   坐标X */
	int Y;                               	/*   坐标Y */
};

struct tagRoleItem_ToLua
{
	int DefID;                           	/*   物品ID */
	int DefPos;                          	/*   服务器加快查找用的索引 */
	unsigned short Num;                             	/*   物品数量 */
	short GridIdx;                         	/*   背包格子索引,从0开始 */
	int InstFlag;                        	/*  Ver.81 实例绑定, 安全锁等标志 */
	int ValidEnd;                        	/*  Ver.50 消耗道具有效结束时间 */
	tdr_ulonglong WID;                             	/*   物品唯一标识, 请保证此字段和之前的字段与RoleArm完全相同 */
	int Flag;                            	/*  Ver.52 用于存储需要放在道具中的数据，具体试道具而定 */
	int InstVal1;                        	/*  Ver.108 存储实例信息,视道具而定,可多次使用的道具UseMaxDur >0 ，则InstVal1为当前剩余使用次数 */
	int InstVal2;                        	/*  Ver.108 存储实例信息,视道具而定 */
	int InstVal3;                        	/*  Ver.173 存储实例信息,视道具而定 */
	char ItemMapNum;                      	/*  Ver.1424 保存的地图位置信息数量 */
	tagItemMapInfo_ToLua ItemMapInfo[MAX_SAVE_MAP_NUM];   	/*  Ver.1424 保存的位置信息 */
};
struct tagBaseAttrInfo_ToLua
{
	unsigned short ResultID;                       
	short Mul;                            
};

struct tagArmStarInfo_ToLua
{
	unsigned char ValIdx;                          	/*   强化比率值在表格中的索引 */
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
	int DefID;                           	/*   物品ID */
	int DefPos;                         
	unsigned short Num;                             	/*   物品数量 */
	short GridIdx;                         	/*   背包格子索引,从0开始 */
	int InstFlag;                        	/*   实例绑定, 安全锁等标志 */
	int ValidEnd;                        	/*  Ver.8 有效结束时间，用于罩装 */
	tdr_ulonglong WID;                             	/*   物品唯一标识, 请保证此字段和之前的字段与RoleItem完全相同 */
	unsigned int Dur;                             	/*  Ver.8 装备损耗度 */
	unsigned char StarLv;                          	/*  Ver.74 品质(星)级别上限: MAX_ARM_STAR_LEVEL */
	unsigned char FightLv;                         	/*  Ver.74 战斗力级别上限: MAX_ARM_FIGHT_LEVEL */
	unsigned char HoleNum;                         	/*  Ver.74 孔个数上限: MAX_ARM_HOLE */
	HOLEGEM HoleGem[MAX_ARM_HOLE];           	/*  Ver.138 孔中宝石 */	
	unsigned char AddAttrNum;                      	/*  Ver.137 关键字的个数上限: MAX_ARM_ADD_ATTR */
	tagAddAttrInst_ToLua AddAttr[MAX_ARM_ADD_ATTR];       	/*  Ver.137 装备的关键字 */
	tagExtendedResult_ToLua ExtendedResult;                  	/*  Ver.321 附魔效果属性 */
	int ExtendedFromDefID;               	/*  Ver.324 被什么道具附魔 */
	tagAddAttrInst_ToLua KeYinAddAttr;                    	/*  Ver.546 精灵刻印属性 */
	int KeYinEnd;                        	/*  Ver.546 精灵刻印到期时间 */
	tdr_ulonglong FengYinID;                       	/*  Ver.942 被谁封印 */
	unsigned char BaseAttrNum;                     	/*  Ver.1143 基础浮动属性的个数上限: MAX_ARM_BASE_ATTR */
	tagBaseAttrInfo_ToLua BaseAttr[MAX_ARM_BASE_ATTR];     	/*  Ver.1143 基础属性浮动值 */
	tagArmStarInfo_ToLua StarInfo[MAX_ARM_STAR_LEVEL];    	/*  Ver.1183 强化数值 */
	unsigned char WashAttrNum;                     	/*  Ver.1226 关键字的个数上限: MAX_ARM_ADD_ATTR */
	tagAddAttrInst_ToLua WashAddAttr[MAX_ARM_WASH_ADD_ATTR]; 	/*  Ver.1226 洗练关键字 */
	int StrengthenCount;                 	/*  Ver.1671 强化次数 */
	unsigned short FengyinFlag;                     	/*  Ver.1751 按位的封印标记 */
	unsigned short WashNum;                         	/*  Ver.1954 消耗洗练石个数 */
	unsigned char Step;                            	/*  Ver.2152 装备阶 */
	tagArmSkillData_ToLua ArmSkill;                        	/*  Ver.2168 装备技能 */
	unsigned char StepUse;                         	/*  Ver.2205  */
	int UnrealBackID;                    	/*  Ver.2428 幻化背饰ID */
	int UnrealBackTmEnd;                 	/*  Ver.2428 幻化期限 */
};

struct tagWearPre_ToLua
{
	unsigned char RoleLevelMin;                   
	unsigned char RoleLevelMax;                   
	unsigned char Career;                         
	unsigned char DiathesisID;                    
	unsigned char Gender;                         
	int GemOpenHoleMoney;                	/*  Ver.437 装备开孔的钱 */
	int GemInlayMoney;                   	/*  Ver.437 装备镶嵌宝石的钱 */
	short PKVal;                           	/*  Ver.970 pk值要求 */
	char ClanLimit;                       	/*  Ver.1036 0-无限制 1-沙巴克老大 2-沙巴克成员 */
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
	unsigned int MaxDur;                          	/*   最大耐久度 */
	unsigned int FightStatusDur;                  	/*   战斗状态每秒消耗耐久 */
};

struct tagIdentifiableItem_ToLua
{
	unsigned int ItemID;                          	/*   鉴定获得物品ID */
	unsigned short ItemRate;                        	/*   鉴定获得物品概率 */
};

struct tagAddAttrList_ToLua
{
	unsigned int ID;                              	/*   装备关键字ID */
	unsigned char Idx;                             	/*   装备关键字效果索引 */
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
	unsigned char SkillLevel_1;                    	/*   临时技能等级 */
	unsigned short SkillID_1;                       	/*   临时技能ID */
	unsigned short SkillLevel_2;                    	/*   临时技能等级 */
	unsigned short SkillID_2;                       	/*   临时技能ID */
	unsigned short SkillDefaultCollectNum;          	/*   临时技能缺省积累数 */
	short SpeedMul;                        	/*   移动速度乘修正 */
	short SpeedAdd;                        	/*   移动速度加修正 */
	unsigned short RefreshTime;                     	/*   重刷时间，单位秒，0 表示不重刷 */
	unsigned short StayTime;                        	/*   存在时间，单位秒，0 表示一直存在 */
	unsigned short MachineEnerge;                   	/*   临时机械能量值 */
	unsigned char EnergeDec;                       	/*   临时机械能量值每秒随时间减少的量 */
	char HurtDefMul;                      	/*   最终伤害乘修正 */
	short HurtDefAdd;                      	/*   最终伤害加修正 */
	int FleeAdd;                         	/*  Ver.161 闪避力加修正 */
};

struct tagMoneyUse_ToLua
{
	int ID;
	int Val;
};

struct tagItemDef_ToLua
{
	unsigned int ItemID;                          	/*   道具id */
	char Name[RES_NAME_LEN];              	/*   道具名 */
	int IconID;                          	/*  Ver.6 图标id */
	int ModeID;                          	/*  Ver.22 模型索引|在npcdisplayinfo中的索引 */
	int ExchgItemID;                     	/*  Ver.96 兑换道具ID|此字段如果不为零,BuyPrice字段为兑换道具数量 */
	unsigned int SellPrice;                       	/*   售卖价格|卖给商店npc值多少钱 */
	unsigned int BuyPrice;                        	/*   购买价格|从npc买需要多少钱 */
	unsigned char MoneyType;                       	/*  Ver.34 货币类型 Bind Macrosgroup:MONEY_TYPE,*/
	unsigned char ItemType;                        	/*   道具类型|不需要填写 Bind Macrosgroup:ITEM_TYPE,*/
	unsigned short MaxPile;                         	/*   可堆叠最大上限|0和1都表示不可堆叠 */
	tagResult_ToLua Result[MAX_ITEM_RESULT];         	/*  Ver.8 道具效果 */
	tagArmDur_ToLua ArmDur;                          	/*  Ver.8 装备耐久|0表示没有耐久度 */
	unsigned int ArmFight;                        	/*  Ver.8 装备战斗力|暂时废除 */
	unsigned short SuitSetID;                       	/*  Ver.8 所属套装的ID */
	unsigned short ValidTime;                       	/*   道具有效时间|从获得开始的有效时间，单位小时 */
	unsigned short NumType;                         	/*   道具编号|不需要填写，item id 的编号前3位，详细见编号规则 */
	unsigned char NoDrop;                          	/*   丢弃属性|0 可丢弃 1 不可丢弃 */
	unsigned char ArmBind;                         	/*   绑定属性 Bind Macrosgroup:ITEM_BIND_TYPE,*/
	unsigned char ExchgLevel;                      	/*   流通等级|委托物品如何在好友间流通 Bind Macrosgroup:BUDDY_GRADE,*/
	unsigned char Important;                       	/*   重要程度|装备使用此字段作为绿装蓝装等的判断:1绿 2蓝 3紫 4橙 */
	short AmmAtk;                          	/*   弹药攻击力修正|废除 */
	unsigned char WeaponAmmNeed;                   	/*   武器需弹药类型|废除 */
	unsigned char WeaponAmmType;                   	/*   武器弹药类型|废除,这个武器使用什么类型的弹药 */
	unsigned char AmmType;                         	/*   弹药类型|废除 */
	tagWearPre_ToLua WearPre;                         	/*  Ver.8 装备条件 */
	unsigned char WearPosNum;                      	/*   装备坑数 */
	unsigned char WearPos1;                        	/*   装备坑位 Bind Macrosgroup:WEAR_POS,*/
	unsigned char WearPos2;                        	/*   装备坑位 Bind Macrosgroup:WEAR_POS,*/
	unsigned char WearPos3;                        	/*   装备坑位 Bind Macrosgroup:WEAR_POS,*/
	char ItemDesc[DESC_LEN];              	/*   道具描述 */
	tagItemTmp_ToLua ItemTmp;                         	/*  Ver.26 临时系统 */
	unsigned int RepairBull;                      	/*  Ver.34 装备修理费用|绑定金币 */
	short DamageMul;                       	/*  Ver.35 破坏力乘修正 */
	unsigned short DamageAdd;                       	/*  Ver.35 破坏力加修正 */
	int SelfCool;                        	/*  Ver.30 物品自我冷却|单位100ms */
	int SpeCool;                         	/*  Ver.30 物品指定冷却|单位100ms */
	unsigned char SelfSpeCoolType;                 	/*  Ver.30 自身指定冷却类型 */
	unsigned char SpeCoolType;                     	/*  Ver.30 指定冷却类型 */
	unsigned char NoNpcSell;                       	/*  Ver.38 不能卖|0 可以卖给npc，1 可以卖给npc */
	unsigned char NoDeal;                          	/*  Ver.38 不能交易|0 玩家间可交易，1 不可交易 */
	unsigned char ItemScript;                      	/*  Ver.47 道具脚本标志|不需要填写 */
	unsigned char MapItem;                         	/*  Ver.51 地图道具|1 离开地图必定消失 */
	unsigned char ArmType;                         	/*  Ver.52 宠物装备|使用此字段限制不能穿同类型，0表示是人穿的装备 */
	unsigned char UseTarget;                       	/*  Ver.52 道具使用对象类型 Bind Macrosgroup:USE_TARGET_TYPE,*/
	short TrustContributeVim;              	/*  Ver.65 委托物品赠送消耗精力 */
	unsigned short GainExp;                         	/*  Ver.65 物品经验获得指数 */
	short TrustOrderVim;                   	/*  Ver.66 委托挂单消耗精力 */
	unsigned short ContributeBuddyVal;              	/*  Ver.67 委托赠送获得友好度 */
	unsigned int NextItemID;                      	/*  Ver.78 下一等级物品ID| 0则无法升级 */
	unsigned int NextItemMoney;                   	/*  Ver.78 升级等级消耗金钱 */
	unsigned char NextItemLevel;                   	/*  Ver.78 下一等级物品级别|填写1-12,升级概率依据对应升级宝石概率信息, */
	unsigned char IdentifiabilityType;             	/*  Ver.78 鉴定类型|0无需鉴定 Bind Macrosgroup:IDENTIFIABILITY_TYPE,*/
	unsigned int IdentifiabilityMoney;            	/*  Ver.78 鉴定消耗金钱 */
	tagIdentifiableItem_ToLua IdentifiabilityItems[MAX_IDENTIFIABILITY_ITEM]; 	/*  Ver.78 鉴定获得物品组 */
	unsigned char StarLimit;                       	/*  Ver.78 装备品质升级上限|0不可升级品质 */
	tagAddAttrList_ToLua AddAttr[MAX_ITEM_ADD_ATTR];      	/*  Ver.104 装备关键字 */
	int ItemLevel;                       	/*  Ver.107 物品等级 */
	unsigned char ItemStar;                        	/*  Ver.110 物品初始星 */
	unsigned short ItemUsePre;                      	/*  Ver.123 物品使用读条时间 */
	tagRespactEntry_ToLua NeedRespact;                     	/*  Ver.126 获取道具声望要求 */
	tagRndHole_ToLua Hole[MAX_ARM_HOLE];              	/*  Ver.135 装备随机孔|第一组表示初始随机孔，第二组表示品质升到3级随机，第三组6级随机，第四组9级随机 */
	unsigned char RandAddAttr;                     	/*  Ver.146 是否随机获得关键字| 0 不随机, 1 随机获得关键字 */
	unsigned int UseMaxDur;                       	/*  Ver.150 可使用次数 */
	unsigned int MaxUseOneDay;                    	/*  Ver.169 每天可使用次数 */
	unsigned int UseTargetID;                     	/*  Ver.192 物品要求使用目标(NPC或者MONSTER  定义ID) */
	char UseModel[RES_PATH_LEN];          	/*  Ver.217 使用物品临时模型 */
	int UseActID;                        	/*  Ver.217 使用物品临时动作ID  */
	char BattleNoUse;                     	/*  Ver.276 是否跨服战场不能使用|0-否  1-不能使用 */
	char IsReStoreItem;                   	/*  Ver.289 是否恢复类道具0-否  1-是 */
	int ExtendedType;                    	/*  Ver.320 附魔类型|0不是附魔物品 1装备效果 2关键字效果 Bind Macrosgroup:EXTENDED_TYPE,*/
	int DropSelfView;                    	/*  Ver.338 掉落只有所有者可见 */
	int ModelIndex;                      	/*  Ver.355 用来作为显示掉落的包裹 */
	int NoResolve;                       	/*  Ver.384 不可分解的标记， 1 不可分解 */
	int DieDropFlag;                     	/*  Ver.442 死亡掉落标记， 0 死亡如果是不绑定有可能掉落。1 死亡必定掉落 Bind Macrosgroup:DIE_DROP_TYPE,*/
	int AddAttrGroupID;                  	/*  Ver.462 关键字随机组ID */
	int CtrlFlag;                        	/*  Ver.480 物品标记按位或: 1 战斗状态不可以使用物品 */
	int ClanContri;                      	/*  Ver.701 购买需要多少公会贡献度|填了这个字段,其他货币失效 */
	int MaxUseDist;                      	/*  Ver.809 物品最大使用距离 */
	unsigned int PoseID;                          	/*  Ver.984 使用物品后,通过具体协议改变玩家姿态 */
	unsigned char BuyLimitFlag;                    	/*  Ver.1029 玩家购买此物品限制|位操作|1-黑名不能买 */
	tagMoneyUse_ToLua MoneyUses[MAX_OTHER_MONEY_USE];  	/*  Ver.1061  */
	int NeedSpartaFight;                 	/*  Ver.2013 购买需要竞技场战斗力 */
	int SkillNum;                        	/*  Ver.2313 装备必定出几条技能属性 0-随机 */
	unsigned short TeshuID;                         	/*  Ver.2390 装备特殊属性ID，对应ArmTeshuDef */
};

struct tagSuitAttr_ToLua
{
	unsigned int SuitAttrID;                      	/*   附加属性ID */
	unsigned int MinSuitNum;                      	/*   最少含有套装中装备的数量 */
};

struct tagSuitSet_ToLua
{
	unsigned int SuitSetID;                       	/*   套装ID */
	int SuitSetType;                     	/*  Ver.1192 套装颜色类型 */
	char Name[RES_NAME_LEN];             
	tagSuitAttr_ToLua SuitAttr[MAX_SUIT_ATTR];        
};

struct tagStarAttr_ToLua
{
	unsigned int SuitAttrID;                      	/*  Ver.641 附加属性ID */
};

struct tagAddAttrBaseDef_ToLua
{
	 int Level;                          
    unsigned int PhsicAtkBase;                    	/*   物理攻击力基础值 */
    unsigned int IceAtkBase;                      	/*   魔法攻击力基础值 */
    unsigned int PhsicDefBase;                    	/*   物理防御力基础值 */
    unsigned int IceDefBase;                      	/*   魔法防御力基础值 */
    unsigned int MaxHPBase;                       	/*   最大血量基础值 */
    unsigned int MaxMPBase;                       	/*   最大魔法基础值 */
    unsigned int HitRateBase;                     	/*   命中基础值 */
    unsigned int FleeBase;                        	/*   闪避基础值 */
    unsigned int DefPhsicHurtBase;                	/*   物理抗性基础值 */
    unsigned int DefIceHurtBase;                  	/*   魔法抗性基础值 */
    unsigned int LiLiang;                         	/*  Ver.462 力量 */
    unsigned int Zhili;                           	/*  Ver.462 智力 */
    unsigned int TiZhi;                           	/*  Ver.462 体质 */
    unsigned int FangHu;                          	/*  Ver.462 防护 */
    unsigned int JingShen;                        	/*  Ver.462 精神 */
    unsigned int MinJie;                          	/*  Ver.462 敏捷 */
    unsigned int HeavyHit;                        	/*  Ver.462 暴击等级 */
    unsigned int DefHeavyHit;                     	/*  Ver.462 暴击抵抗 */
    int WaterAtk;                        	/*  Ver.462 水攻击 */
    int WaterDef;                        	/*  Ver.462 水防御 */
    int FireAtk;                         	/*  Ver.462 火攻击 */
    int FireDef;                         	/*  Ver.462 火防御 */
    int ThunderAtk;                      	/*  Ver.462 雷攻击 */
    int ThunderDef;                      	/*  Ver.462 雷防御 */
    int LightAtk;                        	/*  Ver.462 光攻击 */
    int LightDef;                        	/*  Ver.462 光防御 */
    int NightAtk;                        	/*  Ver.462 暗攻击 */
	int NightDef;                        	/*  Ver.462 暗防御 */
	int IgnoreDef;                       	/*  Ver.853 破防等级 */
	int ReflectHP;                       	/*  Ver.853 反射等级 */
	int DefHP;                           	/*  Ver.853 抵抗等级 */
};

struct tagFixedAddAttr_ToLua
{
    unsigned int ID;                              	        /*   ID */
    tagAddAttrList_ToLua AddAttrs[MAX_ITEM_ADD_ATTR];     	/*   关键字ID和索引 */
};

struct tagArmHideAttrDef_ToLua
{
    unsigned short Career;                          	/*   职业 */
    unsigned short ArmStarLevel;                    	/*   装备星级 */
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
	int MustItemID;                      	/*  Ver.2115 必定完美道具 */
};

struct tagArmGradeDef_ToLua
{
	short ArmType;                         	/*   武器类型 Bind Macrosgroup:ARM_TYPE,*/
	short ArmLevel;                        	/*  Ver.107 武器等级 */
	tagStarAttr_ToLua ArmStar[MAX_ARM_STAR_LEVEL];     	/*   装备品质影响属性修正 */
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
	unsigned int AddAttrID;                       	/*   附加属性ID */
	char AddAttrDesc[SHORT_DESC_LEN];     	/*  Ver.54 关键字描述 */
	unsigned short EffectSkillID[MAX_ADDATTR_SKILL]; 	/*  Ver.69 影响技能ID */
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
	char AddAttrName[NAME_LEN];           	/*   关键字名称 */
	tagArmResult_ToLua Result[MAX_ARM_ADD_ATTR_RESULT];
};

struct tagAddAttrRangeInfo_ToLua
{
	int MinMul;                          	/*   幅度区间百分比:最小 */
	int MaxnMul;                         	/*   幅度区间百分比:最大 */
	int MidMul;                          	/*  Ver.1954 幅度区间百分比:中 */
};

struct tagAddAttrWearPosLimit_ToLua
{
	int RightHand;                       	/*   右手0为此部位不出现该关键字，>0时关键字的数值 */
	int Head;                            	/*   头 */
	int Body;                            	/*   衣服 */
	int LeftHand;                        	/*   左手 */
	int Face;                            	/*   面部  */
	int Shoulder;                        	/*   肩 */
	int Shoe;                            	/*   鞋子 */
	int Amulet;                          	/*   护身符 */
	int Ring;                            	/*   戒指 */
	int NeckLace;                        	/*   项链 */
	int Back;                            	/*  Ver.2086 被饰 */
	int Badge;                           	/*  Ver.2086 徽章 */
};

struct tagAddAttrCareerLimit_ToLua
{
	int None;                            	/*   无职业限制 */
	int Warrior;                         	/*   战士 */
	int Magicor;                         	/*   法师 */
	int Guner;                           	/*   枪手 */
	int Church;                          	/*   牧师 */
	int SingleHandWarrior;               	/*   守护者 */
	int DoubleHandWarrior;               	/*   征服者 */
	int SingleHandMagicor;               	/*   聚能者 */
	int DoubleHandMagicor;               	/*   炼魂师 */
	int SingleHandGuner;                 	/*   猎杀者 */
	int DoubleHandGuner;                 	/*   毁灭者 */
	int SingleHandChurch;                	/*   圣佑者 */
	int DoubleHandChurch;                	/*   驭雷师 */
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
	float Score;                           	/*  Ver.1337 装备评分值 */
	char NotSpar;                         	/*  Ver.1765 1-不能产生晶石 */
	int Main;                            	/*  Ver.2164 是否主属性| */
};

struct tagAddAttrRandList_ToLua
{
	int AddAttrNum;                      	/*   关键字数量 */
	int Rate;                            	/*   概率百分率 */
};

struct tagAddAttrRandDef_ToLua
{
	short ArmLevel;                       
	short Important;                      
	tagAddAttrRandList_ToLua RandList[MAX_ADD_ATTR_RAND_LIST];
	int ValLevel;                        	/*   数值等级，NewAddAttrDef->val1 第几个 */
	int AddMulLevel;                     	/*   附加数值区间等级NewAddAttrDef->RangeList */
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
	int Bind;                            	/*  Ver.2001 0非绑定，1绑定 */
	int Broadcast;                       	/*   标记为1时获得此物品全服务器广播 */
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
	int HideItemID;                      	/*   特殊条件物品ID */
	int HideItemNum;                     	/*   特殊条件物品数量 */
	int HideCount;                       	/*   需要全世界开启多少个此宝箱时触发这个奖励 */
	int NeedItemID;                      	/*  Ver.2001 所需另一物品ID */
	char ErrorTip[SHORT_DESC_LEN];        	/*  Ver.2001 缺少另一物品时的错误提示 */
	tagDisplayItem_ToLua ShowItems[MAX_BAOXIANG_DISPLAY_ITEM]; 	/*  Ver.2001  */
};

enum
{
	CONTAINER_LOCK_STATE_NO,	//背包没有锁
	CONTAINER_LOCK_STATE_LOCK,	//背包锁住
	CONTAINER_LOCK_STATE_OPEN,	//背包已解锁
};
#pragma pack()
//tolua_end