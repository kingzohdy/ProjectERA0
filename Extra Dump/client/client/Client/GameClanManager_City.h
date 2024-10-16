#pragma once
#include "GameEngineTypes.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "ItemDefine.h"
#include "GameDefineTolua.h"

#pragma pack(1)
//tolua_begin

#define MAX_CLAN_MAKE_MATERIAL_TOLUA MAX_CLAN_MAKE_MATERIAL
struct tagClanCityBuilding_ToLua
{
	int		Level;                           	/*   当前等级 > 0 说明建筑已被激活 */
	char	Pending;                         	/*  Ver.463 是否在建造中 */
	char	Res1[3];                        
	int		BeginTime;                       	/*  Ver.463 开始建造的时间 */
	int		EndTime;                         	/*  Ver.463 完成建造的时间 */
};

struct tagClanCity_ToLua
{
	unsigned char m_nLevel;                           	/*   城市等级 > 0 说明城市已被激活 */
	char m_szName[CLAN_MAX_CITY_NAME_LEN];    	/*   公会城市名称 */
	tagClanCityBuilding_ToLua m_buildings[MAX_CLAN_BUILDING];    	/*   公会建筑 */
};

struct	tagClanBuilding_ToLua
{
	int		m_nType;		// 建筑类型(也为ID)
	int		m_nCurLevel;	// 建筑等级
};

struct tagClanNpc_ToLua
{
	int				NpcID;                           	/*   NPC ID */
	int				NpcSaleIdx;                      	/*   NPC售出物品列表索引 */
	tagResPos_ToLua Pos;                             	/*   位置 */
	int				Dir;                             	/*   朝向 */
	char			DynamicScript[RES_NAME_LEN];     	/*   提供动态信息的脚本 */
	int				PKMode;                          	/*  Ver.1104 pk模式|公会模式-1  */
	int				PatrolID;                        	/*  Ver.1106  */
	int Show;                            	/*  Ver.1201 是否在小地图显示 */
};

struct tagClanCityBuildingLevelNeedItem_ToLua
{
	int ItemID;                          	/*   物品ID */
	unsigned int ItemNum;                         	/*   物品数量 */
};

struct tagClanMakeMaterial_ToLua
{
	int ID;                              	/*   材料DefID */
	char Category;                        	/*   类别:0仓库中的物品1:角色身上的物品 */
	int Num;                             	/*   数量 */
};

struct tagClanUpgradeRequired_ToLua
{
	int Money;                           	/*   公会资金 */
	int UsedTechPoint;                   	/*   已使用科技点 */
	int StableValue;                     	/*   安定度 */
	tagClanMakeMaterial_ToLua Material[MAX_CLAN_MAKE_MATERIAL]; 	/*   公会物品 */
};


struct tagClanCityLevelDef_ToLua
{
	unsigned int Level;                           	/*   城市等级 */
	tagClanNpc_ToLua ExtraNpcs[CLAN_MAX_NPC_NUM];     	/*   出现的额外NPC */
	tagClanUpgradeRequired_ToLua Required;                        	/*   升级条件 */
	char EffectString[RES_NAME_LEN];      	/*  Ver.601 效果字符串 */
};

struct tagClanCityBuildingLevelDef_ToLua
{
    short Type;                            	/*   建筑类型 Bind Macrosgroup:CLAN_CONSTRUCTION_INDEX,*/
    short Level;                           	/*   建筑等级 */
    int TimeCost;                        	/*   建设需要的时间 */
    tagClanUpgradeRequired_ToLua Required;                        	/*   升级条件 */
	int BarrierDefID;                    	/*  Ver.528 阻挡怪物的DefID */
	int RequiredCityLv;                  	/*  Ver.533 需要的公会城市最小等级 */
	char EffectString[RES_NAME_LEN];      	/*  Ver.601 效果字符串 */
};

//高级公会数据

struct tagClanRunningProgress_ToLua
{
	char Category;                        	/*   类别 */
	char Res1[3];                        
	int ID;                              	/*   物品DefID/机械MonsterID */
	int Value;                           	/*   剩余制造数量/或科技等级 */
	int BeginTime;                       	/*   开始制造的时间 */
	int EndTime;                         	/*   完成制造的时间 */
};

// 这里TechLevel 不需要修改
struct tagClanTech_ToLua
{
	int TechLevel[MAX_CLAN_TECH_CATEGORY]; 	/*   各项科技的等级 */
	int TechResearchNum;                 	/*   目前正在研发的科技数量 */
	tagClanRunningProgress_ToLua TechResearchProgress[MAX_CLAN_TECH_CATEGORY]; 	/*   各项科技的等级 */
};

struct tagClanProduce_ToLua
{
	int MaxArmMakingNum;                 	/*   最大军事同时制造数量 */
	int ArmMakingNum;                    	/*   军事同时制造数量 */
	tagClanRunningProgress_ToLua ArmMakingProgress[MAX_ARM_MAKING_NUM]; 	/*   目前制造中的军事物品/机械 */
	int MaxCollectionMakingNum;          	/*   最大采集同时制造数量 */
	int CollectionMakingNum;             	/*   采集同时制造数量 */
	tagClanRunningProgress_ToLua CollectionMakingProgress[MAX_COLLECTION_MAKING_NUM]; 	/*   目前制造中的采集机械 */
};

struct tagClanItemGrid_ToLua
{
	int Num;                             	/*   堆叠数 */
};

struct tagClanMachineGrid_ToLua
{
	int HP;                              	/*   机械的血量 */
	int Fuel;                            	/*   机械的油量 */
	char Using;                           	/*   是否使用中 */
	char Res2[3];                        
};

union tagClanStorageGridExtension_ToLua
{
	tagClanItemGrid_ToLua AsItemGrid;                      	/* CLAN_GRID_ITEM,  保存物品的格子 */
	tagClanMachineGrid_ToLua AsMachineGrid;                   	/* CLAN_GRID_MACHINE,  保存机械的格子 */
};

struct tagClanStorageGrid_ToLua
{
    int ID;                              	/*   物品的ID */
    char Category;                        	/*   格子类型 Bind Macrosgroup:CLAN_GRID_CATEGORY,*/
    char Res1[3];                        
    char State;                           	/*   状态 Bind Macrosgroup:CLAN_STORAGE_GRID_STATE,*/
    char Res2[3];                        
    int Pos;                             	/*   在仓库中的位置 */
    tagClanStorageGridExtension_ToLua Extension;                       	/*  Ver.431 格子额外数据 */
};

struct tagClanResourceInfo_ToLua
{
	int ResourceID;                      	/*   资源id,或物品id */
	int Num;                             	/*   资源数量 */
	char State;                           	/*  Ver.876 状态 Bind Macrosgroup:CLAN_STORAGE_GRID_STATE,*/
	int Pos;                             	/*  Ver.953 索引 */
};
struct tagGetResInfoOne_ToLua
{
	int Position;                       
	int GetNum;                         
	int ItemID;                         
	int UseContri;                      
	int GetTime;                        
	char Name[ROLE_NAME_LEN];            
};

struct tagClanResource_ToLua
{
	int ResourceNum;                     	/*   资源类型数量 */
	tagClanResourceInfo_ToLua ResourceInfo[MAX_CLAN_RESOURCE]; 	/*   资源信息 */
	int ItemNum;                         	/*   资源中心物品数量 */
	tagClanResourceInfo_ToLua ItemInfo[MAX_CLAN_RESOURCE_GRID_NUM]; 	/*   物品信息 */
	int GetInfoNum;                      	/*  Ver.1633 领取记录数量 */
	tagGetResInfoOne_ToLua GetResInfos[MAX_GET_RES_INFO];   	/*  Ver.1633 领取记录 */
};
/* 公会技能 */
struct tagClanSkill_ToLua
{
	int SkillID;                         	/*   技能ID */
	int Level;                           	/*   技能等级 */
	int ValidLevel;                      	/*   有效等级 */
};

/* 公会技能定义表 */
struct tagClanSkillDef_ToLua
{
	short SkillID;                         	/*   技能ID */
	short Level;                           	/*   技能等级 */
	int ResearchNeedClanMoney;           	/*   研究需要资金数 */
	int ResearchNeedClanLevel;           	/*   研究需要的公会等级 */
	int LearnNeedContri;                 	/*   学习或升级所需贡献度 */
	int AutoLearn;	                     	/*   公会成员学习类型|0需成员手动学习 1成员不能学习但自动有效 2成员不能学习且没效果 */
	tagResult_ToLua Result[MAX_CLAN_SKILL_RESULT];  	/*   技能效果 */
	int IconID1;                         	/*  Ver.1566 图标1 */
	int IconID2;                         	/*  Ver.1566 图标2 */
	int IconID3;                         	/*  Ver.1566 图标3 */
	char Name[NAME_LEN];                  	/*  Ver.1568 名称 */
	char SkillDesc[512];                  	/*  Ver.1569 描述 */
	int ReContri;                        	/*  Ver.2270 被踢出或退出公会时，返还的技能贡献度值 */
};

/* 公会技能信息 */
struct tagClanSkillInfo_ToLua
{
	int Num;                             	/*   数量 */
	tagClanSkill_ToLua ClanSkill[MAX_CLAN_SKILL_NUM];   	/*   公会技能信息 */
};

/* 玩家的公会技能 */
struct tagPlayerClanSkill_ToLua
{
	int SkillID;                         	/*   技能ID */
	int Level;                           	/*   技能等级 */
	int ClanSkillIdx;                    	/*   技能在公会已经研究技能的索引位置 */
};

/* 玩家的公会技能信息 */
struct tagPlayerClanSkillInfo_ToLua
{
	int Num;                             	/*   数量 */
	tagPlayerClanSkill_ToLua ClanSkill[MAX_CLAN_SKILL_NUM];   	/*   公会技能信息 */
};

/* 公会捐献日志条目 */
struct tagClanDonateLogEntry_ToLua
{
	char MemberName[NAME_LEN];            	/*   角色名 */
	int DonateMoney;                     	/*   捐献数量 */
	int ClanMoney;                       	/*   公会获得资金 */
	int MemberContri;                    	/*   成员贡献度 */
	int LogTime;                         	/*   记录时间 */
};

/* 公会捐献日志信息 */
struct tagClanDonateLogInfo_ToLua
{
	int BeginPos;                        	/*  Ver.1564 起始位置 */
	int Num;                             	/*   数量 */
	tagClanDonateLogEntry_ToLua Log[MAX_CLAN_DONATE];            	/*   公会技能信息 */
};

/* 公会运镖信息 */
struct tagClanDartInfo_ToLua
{
	int StartTime;                      
	int IsOpen;                         
	int ClanBusinessExpMul;              	/*  Ver.2269 公会技能为公会商运，每提升一级增加公会商运的经验加成百分比 */
};

struct tagClanAdvance_ToLua
{
	tagClanTech_ToLua Tech;                            		/*   公会科技 */
	tagClanProduce_ToLua Produce;                         	/*   公会制造 */
	int UsedTechPoint;                   	/*  Ver.429 已经消耗的科技点数 */
	tagClanResource_ToLua Resource;                        	/*  Ver.870 公会资源中心 */
	tagClanSkillInfo_ToLua ClanSkillInfo;                   	/*  Ver.1562 公会技能 */
	tagClanDonateLogInfo_ToLua ClanDonateLogInfo;               	/*  Ver.1562 公会捐献日志 */
	tagClanDartInfo_ToLua DartInfo;                        	/*  Ver.2137  */
};

struct tagClanGeneralResult_ToLua
{
	int ID;                              	/*   科技效果ID */
	int Value1;                          	/*   效果值1 */
	int Value2;                          	/*   效果值2 */
	int Value3;                          	/*   效果值3 */
	int Value4;                          	/*   效果值4 */
};

struct tagClanTechDef_ToLua
{
	short Category;                        	/*   科技所属类别 Bind Macrosgroup:CLAN_TECH_CATEGORY,*/
	short Level;                           	/*   等级 */
	char Name[RES_NAME_LEN];              	/*   名字 */
	char DescText[SHORT_DESC_LEN];        	/*   描述信息 */
	int Required;                        	/*   必要条件 */
	int NeedTechPointCost;               	/*   所需科技点的消耗 */
	int NeedMoneyCost;                   	/*   所需公会资金的消耗 */
	int TimeCost;                        	/*   研发此科技所需的时间(秒) */
	tagClanGeneralResult_ToLua Result[MAX_CLAN_TECH_RESULT];    	/*   公会科技效果 */
	char TypeName[RES_NAME_LEN];          	/*  Ver.417 大类名 */
	int IconID;                          	/*  Ver.417 图标ID */
	int NeedTech;                        	/*  Ver.870 所需科技 */
	int NeedTechLevel;                   	/*  Ver.870 所需科技的等级 */
};

struct tagClanMakeTechRequired_ToLua
{
	char TechCategory;                    	/*   所需科技类别 Bind Macrosgroup:CLAN_TECH_CATEGORY,*/
	char Level;                           	/*   所需科技等级 */
};

struct tagClanProductDef_ToLua
{
	int ID;                              	/*   物品DefID/机械的MonsterID */
	char Category;                        	/*   类别 Bind Macrosgroup:CLAN_PRODUCT_CATEGORY,*/
	char Name[RES_NAME_LEN];              	/*   名字 */
	char DescText[SHORT_DESC_LEN];        	/*   描述信息 */
	int TimeCost;                        	/*   制造此机械/物品所需的时间(秒) */
	int NeedMoneyCost;                   	/*   所需公会资金的消耗 */
	tagClanMakeMaterial_ToLua NeedMaterialCost[MAX_CLAN_MAKE_MATERIAL]; 	/*   制造此机械所需的材料 */
	tagClanMakeTechRequired_ToLua TechRequired[MAX_CLAN_MAKE_TECH]; 	/*   制造此机械所需的科技 */
	int MaxPile;                         	/*   可堆叠数量,只对物品有效 */
	tagClanGeneralResult_ToLua Result[MAX_CLAN_ITEM_RESULT];    	/*   公会物品效果,只有当这个产品是物品的时候才有效 */
	char TypeName[RES_NAME_LEN];          	/*  Ver.420 类型名 */
	int BuyPrice;                        	/*  Ver.449 在公会商店卖出的价格(公会资金数) */
	float ContributValue;                  	/*  Ver.541 每一单位的资源奖励的个人贡献度 */
	int ResourceID;                      	/*  Ver.870 物品对应的资源物品id */
	float ResourceRate;                    	/*  Ver.870 物品对应的资源比率 */
	int ContributPrice;                  	/*  Ver.870 兑换时需要的贡献度 */
	int ProductTech;                     	/*  Ver.870 制造后公会获得科技点数 */
	char RecDescText[SHORT_DESC_LEN];     	/*  Ver.913 资源中心兑换时的描述 */
};

enum tagCLAN_TECH_CATEGORY_TOLUA 
{
	CLAN_TECH_CATEGORY_NONE_TOLUA = 0, 
	CLAN_TECH_CATEGORY_COLLECTOR_ATTRIBUTES_TOLUA = 1, 	/* 采矿车增强 */
	CLAN_TECH_CATEGORY_COLLECTOR_MAKING_TOLUA = 2, 	/* 采矿车制造 */
	CLAN_TECH_CATEGORY_COLLECTOR_ALTERATION_TOLUA = 3, 	/* 采矿车改造 */
	CLAN_TECH_CATEGORY_MACHINE_MAKING_TOLUA = 4, 	/* 机械制造 */
	CLAN_TECH_CATEGORY_TANK_MAKING_TOLUA = 5, 	/* 战车制造 */
	CLAN_TECH_CATEGORY_PET_TOLUA = 6, 	/* 宠物 */
	CLAN_TECH_CATEGORY_PET_ALTERATION_TOLUA = 7, 	/* 宠物改造 */
};

enum tagCLAN_GRID_CATEGORY_TOLUA 
{
	CLAN_GRID_ITEM_TOLUA = 0, 	/* 物品 */
	CLAN_GRID_MACHINE_TOLUA = 1, 	/* 机械 */
};

enum tagCLAN_STORAGE_GRID_STATE_TOLUA
{
	CLAN_GRID_LIMITED_TOLUA = CLAN_GRID_LIMITED, 	/* 限制状态 */
	CLAN_GRID_LOCKED_TOLUA = CLAN_GRID_LOCKED, 		/* 锁定状态 */
	CLAN_GRID_OPEN_TOLUA = CLAN_GRID_OPEN, 			/* 开放状态 */
};

enum tagCLAN_CONSTRUCTION_INDEX_TOLUA 
{
	CLAN_IDX_TECH_CENTER_TOLUA = 0, 	/* 科技中心 */
	CLAN_IDX_COLLECTION_CENTER_TOLUA = 1, 	/* 采集中心 */
	CLAN_IDX_FACTORY_TOLUA = 2, 	/* 工厂 */
	CLAN_IDX_FARM_TOLUA = 3, 	/* 兽栏 */
	CLAN_IDX_WALL_TOLUA = 4, 	/* 城防 */
	CLAN_IDX_CITY_TOLUA = 5, 	/* 城市 */
};


#pragma pack()

const tagClanCityLevelDef_ToLua*			getClanCityLevelDef( int nLevel );
const tagClanCityBuildingLevelDef_ToLua*	getClanCityBuildingLevelDef( int nType, int nLevel );

struct tagClanBuildingEntity_ToLua
{
	char Model[RES_PATH_LEN];            
	int X;                              
	int Y;                              
	float Scale;                          
	int Pitch;                          
	int Yaw;                            
	int Roll;                           
	char DescText[SHORT_DESC_LEN];        	/*  Ver.537  */
};

struct tagClanBuildingDisplayInfo_ToLua
{
	int ID;                             
	tagClanBuildingEntity_ToLua Entities[MAX_CLAN_BUILDING_ENTITIES];
};

const tagClanBuildingDisplayInfo_ToLua*		getBuildingDisplayInfo( int nIndex );

//tolua_end

// 保存当前购买/升级城市所需数据
// 保存当前购买/升级建筑所需数据
class	GameClanCityInfo//tolua_export
{//tolua_export
public:
	//tolua_begin
	// 升级建筑
	void upgradeClanBuilding( int nBuildingID );

	// 申请城市
	void enableClanCity( const char* pszCityName );

	// 升级城市
	void upgradeClanCity( const char* pszCityName );

	tagClanCity_ToLua&	getCityInfo()
	{
		return m_clanCityInfo;
	}

	tagClanBuilding_ToLua&	getSelectBuilding()
	{
		return m_curSelectBuilding;
	}

	void	setSelectBuilding( int nType );
	//tolua_end


	void setCityInfo( const tagClanCity& cityInfo );
public:
	tagClanCity_ToLua			m_clanCityInfo;
	tagClanBuilding_ToLua		m_curSelectBuilding;
};//tolua_export
