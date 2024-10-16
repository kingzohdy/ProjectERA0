#pragma once
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "Item.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include <map>
#include <vector>

const int MAX_STUFF_ARMPROCE = 10;

//tolua_begin
enum
{
	STUFF_NAMAL			= 2000,				// 放入普通物品
	STUFF_LEVELGEM		= 2001,				// 放入升级要用的宝石
	STUFF_INLAYGEM		= 2002,				// 放入镶嵌要用的宝石
	STUFF_EQUIP			= 2003,				// 放入装备
	STUFF_INDENTITY		= 2004,				// 放入鉴定装备
	STUFF_NEEDHOLE		= 2005,				// 放入带孔装备
	STUFF_NEEDINLAYGEM	= 2006,				// 放入带镶嵌宝石装备
	STUFF_STARITEM		= 2007,				// 放入品质升级材料
	STUFF_FIGHTITEM		= 2008,				// 放入战斗力升级材料
	STUFF_LEVELITEM		= 2009,				// 放入等级升级材料
	STUFF_PICKGEM		= 2010,				// 装备上待摘取的宝石
	STUFF_CANLEVEL		= 2011,				// 放入可升级装备
	STUFF_SKILLFITTING	= 2012,				// 放入技能配件升级材料

	CLIENT_ARM_SAFE_ITEM_ID		= 5209001, 	/* 装备品质提升保险符 */
	CLIENT_FIGHT_SAFE_ITEM_ID	= 5219010,	/* 装备战斗力提升保险符 */
	CLIENT_GEM_SAFE_ITEM_ID		= 5219001 	/* 宝石镶嵌保险符 */
};

#pragma pack(1) 
struct tagGemAttrData_ToLua
{
	short GemRate;                         	/*   宝石作用概率 */
	unsigned int GemMoney;                        	/*   消耗金钱 */
};

struct tagGemDef_ToLua
{
	unsigned int ItemID;                         
	unsigned char GemLevel;                        	/*   宝石等级 */
	unsigned char GemEffectType;                   	/*   宝石影响类型 Bind Macrosgroup:GEM_EFFECT_TYPE,*/
	unsigned char GemType;                         	/*   镶嵌的宝石类型 Bind Macrosgroup:GEM_INLAY_TYPE,*/
	tagGemAttrData_ToLua GemAttr[MAX_GEM_ATTR_LEVEL];     	/*   宝石效果 */
	unsigned int LevelUpItemID;                   	/*  Ver.81 升级宝石所需道具 */
	unsigned int PickItemID;                      	/*  Ver.82 摘取宝石所需道具 */
	unsigned char GemColor;                        	/*  Ver.131 宝石颜色 */
	int GemMerit;                        	/*  Ver.946 宝石价值|用于解封装备 */
};

struct tagGemAttr_ToLua
{
	int AttrID;                         
	int Rate;                           
};
struct tagGemFengJie_ToLua
{
	int ID;                             
	int Min;                            
	int Max;                            
};
struct tagGemUse_ToLua
{
	int JinBi;                          
	int JinQuan;                        
	int ItemID;                         
};
struct tagGemNewDef_ToLua
{
	int ItemID;                         
	int Level;                          
	int IconID;                         
	tagGemAttr_ToLua Attr[MAX_GEM_ATTR];              	/*   属性列表，最多10个 */
	int UpID;                            	/*   升级为哪个宝石 */
	int UpRate;                          	/*   单个宝石升级概率|万分率 */
	int UpMin;                           	/*   最少需要几个宝石才能升级 */
	int GemMerit;                        	/*   用于解封消耗 */
	int Color;                           	/*   颜色 */
	tagGemFengJie_ToLua GemFengJie[MAX_GEM_FENGJIE];     	/*   宝石分解定义 */
	tagGemUse_ToLua ReSet;                           	/*   重置消耗，三种货币由玩家选择其一 */
	int PickID;                          	/*   摘取消耗 */
	short ArmLevel;                        	/*  Ver.1136 镶嵌装备最小等级 */
	int UseMoney;                        	/*  Ver.1144 消耗银币 */
	int InlayUseMoney;                   	/*  Ver.2198 镶嵌消耗货币 */
	int InlayUseType;                    	/*  Ver.2198 镶嵌消耗货币类型 */
};
struct tagGemLevelStarDef_ToLua
{
	short Level;                          
	short Star;                            	/*   0-7从零开始，0表示没有星级 */
	int PowerUp;                         	/*   能量上限,0表示不可以充能 */
	int GetPower;                        	/*   产生能量值,0表示没有能量值 */
};

struct tagGemAttrStarDef_ToLua
{
	short AttrID;                         
	char Level;                           	/*  Ver.1166  */
	char Star;  
	tagGemAttr_ToLua Attr[MAX_GEM_VAL];               	/*   数值列表，最多10个 */
};
struct tagUseDef_ToLua
{
	int UseID;                           	/*   取值为USE_ID */
	int UseJinbi;                        	/*   消耗金币 */
	int UseJinquan;                      	/*   消耗金券 */
	int UseMoney;                        	/*   消耗银币 */
};
struct tagArmIdxData_ToLua
{
	char ArmListType;                     	/*    Bind Macrosgroup:LIST_TYPE,*/
	unsigned short ArmGridIdx;                     
};
struct tagAddPowerStuff_ToLua
{
	short ItemNum;                        
	short ListType;                        	/*    Bind Macrosgroup:LIST_TYPE,*/
	int GridIdx;                        
	int ItemID;                         
};

struct tagGemAddPower_ToLua
{
	int OpType;                          	/*  Ver.1150 0-普通充能-vip充能*/
	int UseType;                         	/*  Ver.1150 0-金币-金券*/
	int ArmHoleIdx;                      	/*   装备上第几个宝石,如果是背包忽略此字段*/
	tagArmIdxData_ToLua ArmData;                        
	unsigned char StuffNum;                        	/*   材料数量*/
	tagAddPowerStuff_ToLua StuffData[GEM_ADD_POWER_LIST];   	/*   材料列表*/
};


struct tagRecipeGetItem_ToLua
{
    unsigned int ItemID;                         
    unsigned short Num;                            
    unsigned short Rate;                           
};

struct tagArmFenJieDef_ToLua
{
	short Level;                           	/*   等级 */
	short Important;                       	/*   品质 */
	tagRecipeGetItem_ToLua GetItem[MAX_ARMFENJIE_ITEM];     	/*   分解物品列表内容 */
	int UseMoney;                        	/*  Ver.1144 消耗银币 */
	tagRecipeGetItem_ToLua AppendItem[MAX_ARMFENJIE_APPEND]; 	/*  Ver.1227 附加物品列表内容 */
	short SparExp;                         	/*  Ver.1231 获取晶石经验 */
};

struct tagCSActionArmWash_ToLua
{
	int Succ;                            	/*   0 失败 1成功 */
	unsigned char AddAttrNum;                     
	tagAddAttrInst_ToLua AddAttr[MAX_ARM_ADD_ATTR];      
	unsigned char WashAttrNum;                    
	tagAddAttrInst_ToLua WashAddAttr[MAX_ARM_WASH_ADD_ATTR];
};

struct tagSparSorbData_ToLua
{
	tagArmIdxData_ToLua Src;                             	/*   源位置  */
	unsigned char StuffNum;                        	/*   材料数量 */
	tagAddPowerStuff_ToLua StuffData[MAX_SPAR_SORB_STUFF];  	/*   材料列表 */
};

struct tagSparDef_ToLua
{
	int ID;                              	/*   道具ID */
	char MaxLvl;                          	/*   此ID的晶石最高等级 */
	int UpExp[SPAR_MAX_LEVEL];           	/*   每个等级晶石升级需要经验 */
	int SparExp;                         	/*   此ID的晶石产生多少经验|与晶石等级无关 */
	int Step;                            	/*  Ver.1769 几阶晶石 */
};

struct tagArmLevelUPStuff_ToLua
{
	unsigned int ItemID;                         
	unsigned int Num;                            
};

struct tagArmLevelUPDef_ToLua
{
	int ItemID;                         
	tagArmLevelUPStuff_ToLua Stuff[MAX_ARM_LEVELUP_STUFF];   
	int MoneyType;                      
	int MoneyVal;                       
	int NextItemID; 
	char NextType;                        	/*  Ver.2225 具体进阶成啥套装，策划和客户端沟通 */
};

struct tagHighWashUseDef_ToLua
{
	int LockNum;                        
	int UseType;                        
	int UseVal;                         
};

struct tagBackLevelDef_ToLua
{
	int ItemID;                         
	int SubType;                        
	int Level;                          
	int NextItemID;                     
};

struct tagArmStepUp_ToLua
{
	tagArmIdxData_ToLua Src;                             	/*   升阶装备 */
	int ArmNum;                         
	tagArmIdxData_ToLua ArmStuffs[ARM_STEP_UP_STUFF];    	/*   消耗的装备  */
};

struct tagArmStepAttr_ToLua
{
	int Val;                             	/*   属性值 */
	int ItemNum;                        
	int MoneyNum;                       
	int ArmNum;                         
};

struct tagArmStepUpDef_ToLua
{
	short Lvl;                             	/*   等级段|等级/10,从0开始 */
	unsigned char Imp;                             	/*   品质 */
	unsigned char WPos;                            	/*   部位 */
	unsigned char MaxStep;                         	/*   最大可升阶层 */
	int ItemID;                          	/*   消耗道具ID */
	int MoneyID;                         	/*   消耗货币ID */
	int AttrID;                          	/*   附加属性ID */
	tagArmStepAttr_ToLua StepAttr[ARM_MAX_STEP];          	/*   阶层升级数据 */
	unsigned char UseArmImp;                       	/*  Ver.2164 消耗装备品质 */
};

struct tagArmGradeValDef_ToLua
{
	int Imp;                             	/*   品质 */
	int Val;                             	/*   档次值 */
};

struct tagResetRate_ToLua
{
	int Val;                            
	int Rate[ARM_RESET_MAX_RATE];       
};

struct tagArmReSetDef_ToLua
{
	int Lvl;                             	/*   等级 */
	tagResetRate_ToLua Reset[ARM_RESET_MAX_DIST];      
	int UseMoney;                       
};

struct tagArmBaseReSet_ToLua
{
	tagArmIdxData_ToLua Src;                             	/*   重置装备 */
	tagArmIdxData_ToLua UseItem;                         	/*   使用道具 */
	int ArmNum;                         
	tagArmIdxData_ToLua ArmStuffs[ARM_BASE_ATTR_RESET_MAX]; 	/*   消耗的装备  */
};

struct tagSkillActiveOne_ToLua
{
	int Rate;                           
	int UseItemID;                      
	int UseItemNum;                     
	int UseMoney;                       
};

struct tagArmSkillNumDef_ToLua
{
	short Lvl;                            
	unsigned char Imp;                            
	unsigned char WPos;                           
	tagSkillActiveOne_ToLua Active[MAX_ARM_SKILL_NUM];      
};

struct tagArmSkillAttrDef_ToLua
{
	int ID;                             
	char DefDesc[ARM_SKILL_MAX_DESC];    
	unsigned char Lvl;                            
	int SkillID;                        
	int EffectID;                       
	int BaseVal;                        
	int LvlMdf[MAX_LVL_GRADE];           	/*   等级段修正 */
	tagAddAttrWearPosLimit_ToLua WposMdf;   /*   部位修正 */
	int ImpMdf[MAX_IMP_GRADE];           	/*   品质修正 */
	short RandMdf[MAX_IMP_GRADE];          	/*   随机修正 */
	unsigned char MinLvlLimit;                    
	unsigned char MaxLvlLimit;                    
	int CareerLimit;                    
	int WposLimit;                      
	int ImpLimit;    
	char Type;                            	/*  Ver.2293  */
};
struct tagArmSkillUse_ToLua
{
	int ItemID;                         
	unsigned char ItemNum;                        
	unsigned char MoneyType;                      
	int MoneyNum;                       
};

struct tagArmSkillRecastDef_ToLua
{
	int Lvl;                            
	tagArmSkillUse_ToLua Active[MAX_ARM_SKILL_NUM];      
	int ResetItemID;                    
	unsigned char ResetItemNum;                   
	unsigned char ResetMoneyType;                 
	int ResetMoneyNum;                  
};
#pragma pack()

enum
{
	CLIENT_GEM_FAIL		= 0,
	CLIENT_GEM_SUCCESS	= 1,
	CLIENT_GEM_PERFECT	= 2,
};

enum
{
	CLIENT_GEM_EFFECT_STAR		= GEM_EFFECT_STAR,
	CLIENT_GEM_EFFECT_FIGHT		= GEM_EFFECT_FIGHT,
	CLIENT_GEM_EFFECT_LEVEL		= GEM_EFFECT_LEVEL,
	CLIENT_GEM_EFFECT_INLAY		= GEM_EFFECT_INLAY,
};

enum
{
	CLIENT_GEM_HOLE1_RANDOM = GEM_HOLE1_RANDOM, 	/* 第一个孔的概率 */
	CLIENT_GEM_HOLE2_RANDOM = GEM_HOLE2_RANDOM, 	/* 第二个孔的概率 */
	CLIENT_GEM_HOLE3_RANDOM = GEM_HOLE3_RANDOM, 	/* 第三个孔的概率 */
	CLIENT_GEM_HOLE4_RANDOM = GEM_HOLE4_RANDOM, 	/* 第四个孔的概率 */
};

const tagGemDef_ToLua* getGemDef( int nItemID );
const tagArmLevelUPDef_ToLua* getArmLevelUPDef( int nID );

int getArmMoveMoney( int nType );
int getArmResetMoney();

#define ARM_STAR_ITEM_TOLUA ARM_STAR_ITEM 	/* 日冕宝钻 装备升星和转移星级需要的宝石数量 */
#define ARM_UNBIND_ITEM_LOW_TOLUA ARM_UNBIND_ITEM_LOW         	/* 低级解绑符：可以解绑60级以下装备  */
#define ARM_UNBIND_ITEM_MID_TOLUA ARM_UNBIND_ITEM_MID		 	/* 中级解绑符：可以解绑61~90级装备 */
#define ARM_UNBIND_ITEM_HIGH_TOLUA ARM_UNBIND_ITEM_HIGH			/* 高级解绑符：可以解绑91级以上装备 */
#define ARM_GEM_PICK_ITEM_TOLUA	ARM_GEM_PICK_ITEM				//宝石摘取符 破碎宝石拆解器
#define ARM_BASE_ATTR_RESET_MAX_TOLUA ARM_BASE_ATTR_RESET_MAX   /* 基础属性重置消耗装备件数限制 */
#define ARM_RESET_MAX_DIST_TOLUA ARM_RESET_MAX_DIST				/* 属性重置的档次 */
#define ITEM_STAT_UP_FIRST_PERFECT_TOLUA ITEM_STAT_UP_FIRST_PERFECT /* 完美强化自动保存强化道具id */

enum tagCONSUME_TYPE_TOLUA 
{
	CONSUME_TYPE_BINDGODCOIN_TOLUA = CONSUME_TYPE_BINDGODCOIN, 	/* 金币消耗 */
	CONSUME_TYPE_GODCOIN_TOLUA = CONSUME_TYPE_GODCOIN, 	/* 金券消耗 */
};

enum tagVIPRECASTCONSUME_TOLUA 
{
	LEVEL40_TOLUA = LEVEL40, 	/* 小于等于40级装备消耗金币OR金券 */
	LEVEL60_TOLUA = LEVEL60, 	/* 小于等于60级装备消耗金币OR金券 */
	LEVEL80_TOLUA = LEVEL80, 	/* 小于等于80级装备消耗金币OR金券 */
	LEVEL100_TOLUA = LEVEL100, 	/* 小于等于100级装备消耗金币OR金券 */
	LEVEL120_TOLUA = LEVEL120, 	/* 小于等于120级装备消耗金币OR金券 */
};
enum 
{
	ONE_GEM_HECHENG_GODCOIN_TOLUA	= ONE_GEM_HECHENG_GODCOIN ,		/* 一级宝石合成符需要消耗的金币值 */
	TWO_GEM_HECHENG_GODCOIN_TOLUA	= TWO_GEM_HECHENG_GODCOIN  , 	/* 二级宝石合成符需要消耗的金币值 */
	THREE_GEM_HECHENG_GODCOIN_TOLUA = THREE_GEM_HECHENG_GODCOIN ,	/* 三级宝石合成符需要消耗的金币值 */
	FOUR_GEM_HECHENG_GODCOIN_TOLUA  = FOUR_GEM_HECHENG_GODCOIN , 	/* 四级宝石合成符需要消耗的金币值 */
	FIVE_GEM_HECHENG_GODCOIN_TOLUA  = FIVE_GEM_HECHENG_GODCOIN , 	/* 五级宝石合成符需要消耗的金币值 */
	SIX_GEM_HECHENG_GODCOIN_TOLUA   = SIX_GEM_HECHENG_GODCOIN , 	/* 六级宝石合成符需要消耗的金币值 */
	SEVEN_GEM_HECHENG_GODCOIN_TOLUA	= SEVEN_GEM_HECHENG_GODCOIN, 	/* 七级宝石合成符需要消耗的金币值 */
	EIGHT_GEM_HECHENG_GODCOIN_TOLUA		= EIGHT_GEM_HECHENG_GODCOIN,/* 八级宝石合成符需要消耗的金币值 */
	FIVE_GEM_HECHENG_GODCOIN_GJ_TOLUA	= FIVE_GEM_HECHENG_GODCOIN_GJ,	/* 五级宝石合成符需要消耗的金币值_高级 */
	SIX_GEM_HECHENG_GODCOIN_GJ_TOLUA	= SIX_GEM_HECHENG_GODCOIN_GJ,	/* 六级宝石合成符需要消耗的金币值_高级 */
	SEVEN_GEM_HECHENG_GODCOIN_GJ_TOLUA	= SEVEN_GEM_HECHENG_GODCOIN_GJ,	/* 七级宝石合成符需要消耗的金币值_高级 */
	EIGHT_GEM_HECHENG_GODCOIN_GJ_TOLUA	= EIGHT_GEM_HECHENG_GODCOIN_GJ,	/* 八级宝石合成符需要消耗的金币值_高级 */
};
 
//tolua_end

class GameArmProce//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<GameArmProce>
{//tolua_export
public:
	GameArmProce(void);
	~GameArmProce(void);

public:

	// c->s

	//tolua_begin
	// 装备品质提升
	void armAddStar( int nListType, int nGridIdx, int nSelectStar, int nBind, int nMust );
	//保存强化
	void armAddSave( int nArmListType, int nArmGridIdx, int nSelectStarIdx, int nRateIdx );
	// 装备等级提升
	void armAddLevel( int nListType, int nGridIdx );
	// 装备战斗力提升
	void armAddFight( int nListType, int nGridIdx, Item& safeItem );
	// 装备鉴定
	void armIdentifiability( int nListType, int nGridIdx );
	//装备鉴定（鉴定仪）
	void armIdentifyStart( int nSrcType,int nSrcGrid,int nDestType,int nDestGrid );
	// 宝石升级
	void gemLevelUp( Item& stuffItem,int nType );
	// 宝石开孔
	void openGemHole( int nHoleIdx, int nListType );
	void requireFengYinPlayerInfo( int nListType, int nGridIdx );
	void requireUnFengYin( int nListType, int nGridIdx );
	//宝石重置
	void resetGem( int nHoleIdx, int nUseMoneyType, int nListType, int nGirdId );
	//宝石保存重置
	void saveResetGem( int nListType, int nGridIdx );
	//宝石充能
	void chargeGem( tagGemAddPower_ToLua Info );
	tagGemAddPower_ToLua getChargeGemAddPowerInfo();
	int getChargePerPowerUseMoney();
	
	// 宝石镶嵌 nSafeFlag 保险副标志 0 不使用1使用
	void gemInlay( int nHoleIdx, Item& gemItem, int nListType/*, Item& safeItem*/ );
	// 宝石摘取
	void gemPick( int nHoleIdx, int nList,int nGrid,int nStuffID,int nStuffList,int nStuffGrid );
	// 是否在待装备列表中的物品
	bool isInStuffItem( int nListType, int nGrid );
	//装备绑定
	void bindArm( int nListType, int nGridIdx );

	void unBindArm( int nListType,int nGridIdx,int nStuffID,int nStuffList,int nStuffGridIdx );

	void moveArmStar( int nSrcList,int nSrcGrid,int nDestList,int nDestGrid,int nBind );

	//普通 重铸
	void normalRecastArm( int nItemId, int nListType, int nGridIndex, int nBind, int nMust );

	//高级 重铸
	void advanceRecastArm( int nType, int nListType, int nGridIndex );

	//批量 重铸
	void batchRecastArm( int nType, int nListType, int nGridIndex );

	void saveRecastArm( int nSelectIdx, int nListType, int nGridIdex );

	//洗练装备
	void requestArmWash( int stuffItemId, int armList, int armGrid, int nBind, int nLockFlag, int nSafeFlag, int nMust );
	void requestArmWashExtent( int armList, int armGrid, int idx );
	//保存洗练
	void saveWashArm( int armList, int armGrid );
	//晶石镶嵌
	bool requestSparInlay( int nSrcList, int nSrcGrid, int nDestList, int nDestGrid );
	//晶石吸收
	tagSparSorbData_ToLua* getSparSorbData();
	void requestSparSorb();

	void requestArmAttrMove( int nSrcList, int nSrcGrid, int nDstList, int nDstGrid, int nUseList, int nUseGrid, int nType );

	void requestArmAttrReset( int nList, int nGrid, int nItemID, int nBind, int nIdx );

	// 背饰进阶
	const	tagBackLevelDef_ToLua* getBackLevelDef( int id );
	void	requestBackLevelUp( int nBackList, int nBackGrid,tdr_ulonglong backWId, int nItemList, int nItemGrid,tdr_ulonglong itemWId );
	// 背饰改造
	int		getBackLevelID( int nType, int nLevel );
	void	requestBackChange( int nBackList, int nBackGrid, tdr_ulonglong backWId, int nItemList, int nItemGrid,tdr_ulonglong itemWId );
	//tolua_end

public:
	// s->c
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	
	//tolua_begin
	// 添加目标物品
	int addSrcItem( Item& item, int nType = STUFF_EQUIP );
	// 添加装备提升所需要材料或所需要提升(注意放宝石的nType为STUFF_GEM)
	int addStuff( Item& item, int nIndex, int nType = STUFF_NAMAL );
	// 删除装备提升所需要材料
	void delStuff( int nIndex );
	// 删除待提升装备
	void delSrcItem();
	// 清空栏位(注意:每次打开页面调用)
	void clear();
	
	// 获取目标物品
	Item& getSrcItem();
	// 材料是否为空
	bool isStuffItemEmpty();
	// 获取添加的材料
	Item& getStuffItem( int nIndex );
	// 获取所需宝石升级符ID
	int getGemUpdateId();
	// 获取宝石摘取所需物品ID
	int getGemPickUpUsedItem( int nGemId );
	// 获取宝石的升级概率
	int getGemSuccessRate( int index, int nGemId );
	// 获取宝石的升级花费
	unsigned int getGemCostMoney( int index, int nGemId );
	// 获取宝石的类型
	int getGemType( int nGemId );
	// 获取宝石的颜色
	int getGemColor( int nGemId );
	
	// 获取宝石定义
	const tagGemDef_ToLua* getGemDef( int nItemID );
	const tagGemNewDef_ToLua* getGemNewDef( int nItemID );
	const tagGemAttrStarDef_ToLua* getGemAttrStarDef( int nAttrID, int nLevel, int nStar );
	const tagGemLevelStarDef_ToLua* getGemLevelStarDef( int nLevel, int nStar );

	const tagUseDef_ToLua* getUseDef( int nID );
	const tagSparDef_ToLua* getSparDef( int id );

	// 是否是宝石
	bool isGem( int nItemID );

	const tagArmFenJieDef_ToLua* getArmFenJieDef( int nLevel, int nImportant );
	void requestContainerResolvePre( tdr_ulonglong WId, char ListType, unsigned short GirdId, int nType );
	//装备提炼
	void requestSparEpurate( tdr_ulonglong armWId, char armListType, unsigned short armGirdId, 
		tdr_ulonglong itemWId, char itemListType, unsigned short itemGirdId );
	void requestBreakContainerResolvePre();

	int getStarNeedStuffNum( int nStar );
	int getStarNeedMoney( int nStar );

	int getBindNeedMoney( int nStar );
	int getMoveStarNeedStuffNum( int nStar );
	int getUnBindNeedGold( int nStar );
	int getBackStar( int nStar );
	const tagGemDef_ToLua* getNextHeChengGemDef( const tagGemDef_ToLua* pCurGemDef );

	void stopContainerResolve();
	bool isResolvePre();

	int getArmRecastArea( int resultID, int mul );

	// 获取开启晶石格位的等级
	int getSparOpenLv( int nIdx );
	// 获取吸收每件装备消耗银币
	int getSparSorbCostMoney();
	// 高级洗炼
	const tagHighWashUseDef_ToLua* getHighWashDef( int nNum );
	// 装备升阶
	void requestArmStepUp( tagArmStepUp_ToLua Data );
	tagArmStepUp_ToLua getArmStepUpData();
	const tagArmStepUpDef_ToLua* getArmStepUpDef( int nLv, int nImp, int nPos );
	// 基础属性重置
	void requestArmBaseReset( tagArmBaseReSet_ToLua Data );
	void requestArmBaseResetSave( int nList, int nGrid );
	tagArmBaseReSet_ToLua getArmBaseResetData();
	const tagArmGradeValDef_ToLua* getArmGradeValDef( int nImp );
	const tagArmReSetDef_ToLua* getArmResetDef( int nLv );
	tagAddAttrInst_ToLua& getArmResetResult( int idx );
	// 装备技能
	const tagArmSkillNumDef_ToLua* getArmSkillNumDef( int nLv, int nImp, int nPos );
	const tagArmSkillAttrDef_ToLua* getArmSkillAttrDef( int nId );
	void requestArmSkillActive( int idx, int nArmList, int nArmGird, int nUseList, int nUseGrid );
	// 粉装升橙装
	void requestArmUpOrange( int nUseList, int nUseGrid, int nArmList, int nArmGird, int nNum, int nOption );
	//背饰幻化
	void requestUnRealBack( int nSrcList, int nSrcGrid, int nDesList, int nDesGrid );
	//装备技能激活
	void requestArmSkillActive( int srcGridIdx, int srcListType );
	//装备技能锁
	void requestArmSkillLock( int nIdx, int srcGridIdx, int srcListType );
	//装备技能重置
	void requestArmSkillReset( int stuffGridIdx, int stuffListType, int srcGridIdx, int srcListType );
	//装备改造需求
	const tagArmSkillRecastDef_ToLua* getArmSkillRecastDef( int nLv );
	//tolua_end

private:
	// 获取放入宝石的类型和等级
	void getGemTypeLv( int* nType, int* nLv );

private:
	Item	m_Stuff[MAX_STUFF_ARMPROCE];	// 装备提升使用材料
	Item	m_SrcItem;						// 要提升的物品或装备
	std::vector<tagGemDef*>		m_gemDef;
	std::vector<tagGemNewDef*>	m_gemNewDef;
	std::map<int, std::vector<tagGemDef*> >	m_gemTypePartion;
	bool	m_bCanOpenHole;
	tagGemAddPower_ToLua		m_GemAddPowerInfo;
	SPARSORBDATA				m_SparSorbData;

	// 晶石格子开启等级
	std::vector<int>			m_vSparAddLv;
	tagArmStepUp_ToLua			m_ArmStepUpData;
	tagArmBaseReSet_ToLua		m_ArmBaseResetData;
	tagAddAttrInst_ToLua		m_ArmBaseResetResult[MAX_ARM_ADD_ATTR];

};//tolua_export
