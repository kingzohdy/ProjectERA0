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
	int		Level;                           	/*   ��ǰ�ȼ� > 0 ˵�������ѱ����� */
	char	Pending;                         	/*  Ver.463 �Ƿ��ڽ����� */
	char	Res1[3];                        
	int		BeginTime;                       	/*  Ver.463 ��ʼ�����ʱ�� */
	int		EndTime;                         	/*  Ver.463 ��ɽ����ʱ�� */
};

struct tagClanCity_ToLua
{
	unsigned char m_nLevel;                           	/*   ���еȼ� > 0 ˵�������ѱ����� */
	char m_szName[CLAN_MAX_CITY_NAME_LEN];    	/*   ����������� */
	tagClanCityBuilding_ToLua m_buildings[MAX_CLAN_BUILDING];    	/*   ���Ὠ�� */
};

struct	tagClanBuilding_ToLua
{
	int		m_nType;		// ��������(ҲΪID)
	int		m_nCurLevel;	// �����ȼ�
};

struct tagClanNpc_ToLua
{
	int				NpcID;                           	/*   NPC ID */
	int				NpcSaleIdx;                      	/*   NPC�۳���Ʒ�б����� */
	tagResPos_ToLua Pos;                             	/*   λ�� */
	int				Dir;                             	/*   ���� */
	char			DynamicScript[RES_NAME_LEN];     	/*   �ṩ��̬��Ϣ�Ľű� */
	int				PKMode;                          	/*  Ver.1104 pkģʽ|����ģʽ-1  */
	int				PatrolID;                        	/*  Ver.1106  */
	int Show;                            	/*  Ver.1201 �Ƿ���С��ͼ��ʾ */
};

struct tagClanCityBuildingLevelNeedItem_ToLua
{
	int ItemID;                          	/*   ��ƷID */
	unsigned int ItemNum;                         	/*   ��Ʒ���� */
};

struct tagClanMakeMaterial_ToLua
{
	int ID;                              	/*   ����DefID */
	char Category;                        	/*   ���:0�ֿ��е���Ʒ1:��ɫ���ϵ���Ʒ */
	int Num;                             	/*   ���� */
};

struct tagClanUpgradeRequired_ToLua
{
	int Money;                           	/*   �����ʽ� */
	int UsedTechPoint;                   	/*   ��ʹ�ÿƼ��� */
	int StableValue;                     	/*   ������ */
	tagClanMakeMaterial_ToLua Material[MAX_CLAN_MAKE_MATERIAL]; 	/*   ������Ʒ */
};


struct tagClanCityLevelDef_ToLua
{
	unsigned int Level;                           	/*   ���еȼ� */
	tagClanNpc_ToLua ExtraNpcs[CLAN_MAX_NPC_NUM];     	/*   ���ֵĶ���NPC */
	tagClanUpgradeRequired_ToLua Required;                        	/*   �������� */
	char EffectString[RES_NAME_LEN];      	/*  Ver.601 Ч���ַ��� */
};

struct tagClanCityBuildingLevelDef_ToLua
{
    short Type;                            	/*   �������� Bind Macrosgroup:CLAN_CONSTRUCTION_INDEX,*/
    short Level;                           	/*   �����ȼ� */
    int TimeCost;                        	/*   ������Ҫ��ʱ�� */
    tagClanUpgradeRequired_ToLua Required;                        	/*   �������� */
	int BarrierDefID;                    	/*  Ver.528 �赲�����DefID */
	int RequiredCityLv;                  	/*  Ver.533 ��Ҫ�Ĺ��������С�ȼ� */
	char EffectString[RES_NAME_LEN];      	/*  Ver.601 Ч���ַ��� */
};

//�߼���������

struct tagClanRunningProgress_ToLua
{
	char Category;                        	/*   ��� */
	char Res1[3];                        
	int ID;                              	/*   ��ƷDefID/��еMonsterID */
	int Value;                           	/*   ʣ����������/��Ƽ��ȼ� */
	int BeginTime;                       	/*   ��ʼ�����ʱ�� */
	int EndTime;                         	/*   ��������ʱ�� */
};

// ����TechLevel ����Ҫ�޸�
struct tagClanTech_ToLua
{
	int TechLevel[MAX_CLAN_TECH_CATEGORY]; 	/*   ����Ƽ��ĵȼ� */
	int TechResearchNum;                 	/*   Ŀǰ�����з��ĿƼ����� */
	tagClanRunningProgress_ToLua TechResearchProgress[MAX_CLAN_TECH_CATEGORY]; 	/*   ����Ƽ��ĵȼ� */
};

struct tagClanProduce_ToLua
{
	int MaxArmMakingNum;                 	/*   ������ͬʱ�������� */
	int ArmMakingNum;                    	/*   ����ͬʱ�������� */
	tagClanRunningProgress_ToLua ArmMakingProgress[MAX_ARM_MAKING_NUM]; 	/*   Ŀǰ�����еľ�����Ʒ/��е */
	int MaxCollectionMakingNum;          	/*   ���ɼ�ͬʱ�������� */
	int CollectionMakingNum;             	/*   �ɼ�ͬʱ�������� */
	tagClanRunningProgress_ToLua CollectionMakingProgress[MAX_COLLECTION_MAKING_NUM]; 	/*   Ŀǰ�����еĲɼ���е */
};

struct tagClanItemGrid_ToLua
{
	int Num;                             	/*   �ѵ��� */
};

struct tagClanMachineGrid_ToLua
{
	int HP;                              	/*   ��е��Ѫ�� */
	int Fuel;                            	/*   ��е������ */
	char Using;                           	/*   �Ƿ�ʹ���� */
	char Res2[3];                        
};

union tagClanStorageGridExtension_ToLua
{
	tagClanItemGrid_ToLua AsItemGrid;                      	/* CLAN_GRID_ITEM,  ������Ʒ�ĸ��� */
	tagClanMachineGrid_ToLua AsMachineGrid;                   	/* CLAN_GRID_MACHINE,  �����е�ĸ��� */
};

struct tagClanStorageGrid_ToLua
{
    int ID;                              	/*   ��Ʒ��ID */
    char Category;                        	/*   �������� Bind Macrosgroup:CLAN_GRID_CATEGORY,*/
    char Res1[3];                        
    char State;                           	/*   ״̬ Bind Macrosgroup:CLAN_STORAGE_GRID_STATE,*/
    char Res2[3];                        
    int Pos;                             	/*   �ڲֿ��е�λ�� */
    tagClanStorageGridExtension_ToLua Extension;                       	/*  Ver.431 ���Ӷ������� */
};

struct tagClanResourceInfo_ToLua
{
	int ResourceID;                      	/*   ��Դid,����Ʒid */
	int Num;                             	/*   ��Դ���� */
	char State;                           	/*  Ver.876 ״̬ Bind Macrosgroup:CLAN_STORAGE_GRID_STATE,*/
	int Pos;                             	/*  Ver.953 ���� */
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
	int ResourceNum;                     	/*   ��Դ�������� */
	tagClanResourceInfo_ToLua ResourceInfo[MAX_CLAN_RESOURCE]; 	/*   ��Դ��Ϣ */
	int ItemNum;                         	/*   ��Դ������Ʒ���� */
	tagClanResourceInfo_ToLua ItemInfo[MAX_CLAN_RESOURCE_GRID_NUM]; 	/*   ��Ʒ��Ϣ */
	int GetInfoNum;                      	/*  Ver.1633 ��ȡ��¼���� */
	tagGetResInfoOne_ToLua GetResInfos[MAX_GET_RES_INFO];   	/*  Ver.1633 ��ȡ��¼ */
};
/* ���Ἴ�� */
struct tagClanSkill_ToLua
{
	int SkillID;                         	/*   ����ID */
	int Level;                           	/*   ���ܵȼ� */
	int ValidLevel;                      	/*   ��Ч�ȼ� */
};

/* ���Ἴ�ܶ���� */
struct tagClanSkillDef_ToLua
{
	short SkillID;                         	/*   ����ID */
	short Level;                           	/*   ���ܵȼ� */
	int ResearchNeedClanMoney;           	/*   �о���Ҫ�ʽ��� */
	int ResearchNeedClanLevel;           	/*   �о���Ҫ�Ĺ���ȼ� */
	int LearnNeedContri;                 	/*   ѧϰ���������蹱�׶� */
	int AutoLearn;	                     	/*   �����Աѧϰ����|0���Ա�ֶ�ѧϰ 1��Ա����ѧϰ���Զ���Ч 2��Ա����ѧϰ��ûЧ�� */
	tagResult_ToLua Result[MAX_CLAN_SKILL_RESULT];  	/*   ����Ч�� */
	int IconID1;                         	/*  Ver.1566 ͼ��1 */
	int IconID2;                         	/*  Ver.1566 ͼ��2 */
	int IconID3;                         	/*  Ver.1566 ͼ��3 */
	char Name[NAME_LEN];                  	/*  Ver.1568 ���� */
	char SkillDesc[512];                  	/*  Ver.1569 ���� */
	int ReContri;                        	/*  Ver.2270 ���߳����˳�����ʱ�������ļ��ܹ��׶�ֵ */
};

/* ���Ἴ����Ϣ */
struct tagClanSkillInfo_ToLua
{
	int Num;                             	/*   ���� */
	tagClanSkill_ToLua ClanSkill[MAX_CLAN_SKILL_NUM];   	/*   ���Ἴ����Ϣ */
};

/* ��ҵĹ��Ἴ�� */
struct tagPlayerClanSkill_ToLua
{
	int SkillID;                         	/*   ����ID */
	int Level;                           	/*   ���ܵȼ� */
	int ClanSkillIdx;                    	/*   �����ڹ����Ѿ��о����ܵ�����λ�� */
};

/* ��ҵĹ��Ἴ����Ϣ */
struct tagPlayerClanSkillInfo_ToLua
{
	int Num;                             	/*   ���� */
	tagPlayerClanSkill_ToLua ClanSkill[MAX_CLAN_SKILL_NUM];   	/*   ���Ἴ����Ϣ */
};

/* ���������־��Ŀ */
struct tagClanDonateLogEntry_ToLua
{
	char MemberName[NAME_LEN];            	/*   ��ɫ�� */
	int DonateMoney;                     	/*   �������� */
	int ClanMoney;                       	/*   �������ʽ� */
	int MemberContri;                    	/*   ��Ա���׶� */
	int LogTime;                         	/*   ��¼ʱ�� */
};

/* ���������־��Ϣ */
struct tagClanDonateLogInfo_ToLua
{
	int BeginPos;                        	/*  Ver.1564 ��ʼλ�� */
	int Num;                             	/*   ���� */
	tagClanDonateLogEntry_ToLua Log[MAX_CLAN_DONATE];            	/*   ���Ἴ����Ϣ */
};

/* ����������Ϣ */
struct tagClanDartInfo_ToLua
{
	int StartTime;                      
	int IsOpen;                         
	int ClanBusinessExpMul;              	/*  Ver.2269 ���Ἴ��Ϊ�������ˣ�ÿ����һ�����ӹ������˵ľ���ӳɰٷֱ� */
};

struct tagClanAdvance_ToLua
{
	tagClanTech_ToLua Tech;                            		/*   ����Ƽ� */
	tagClanProduce_ToLua Produce;                         	/*   �������� */
	int UsedTechPoint;                   	/*  Ver.429 �Ѿ����ĵĿƼ����� */
	tagClanResource_ToLua Resource;                        	/*  Ver.870 ������Դ���� */
	tagClanSkillInfo_ToLua ClanSkillInfo;                   	/*  Ver.1562 ���Ἴ�� */
	tagClanDonateLogInfo_ToLua ClanDonateLogInfo;               	/*  Ver.1562 ���������־ */
	tagClanDartInfo_ToLua DartInfo;                        	/*  Ver.2137  */
};

struct tagClanGeneralResult_ToLua
{
	int ID;                              	/*   �Ƽ�Ч��ID */
	int Value1;                          	/*   Ч��ֵ1 */
	int Value2;                          	/*   Ч��ֵ2 */
	int Value3;                          	/*   Ч��ֵ3 */
	int Value4;                          	/*   Ч��ֵ4 */
};

struct tagClanTechDef_ToLua
{
	short Category;                        	/*   �Ƽ�������� Bind Macrosgroup:CLAN_TECH_CATEGORY,*/
	short Level;                           	/*   �ȼ� */
	char Name[RES_NAME_LEN];              	/*   ���� */
	char DescText[SHORT_DESC_LEN];        	/*   ������Ϣ */
	int Required;                        	/*   ��Ҫ���� */
	int NeedTechPointCost;               	/*   ����Ƽ�������� */
	int NeedMoneyCost;                   	/*   ���蹫���ʽ������ */
	int TimeCost;                        	/*   �з��˿Ƽ������ʱ��(��) */
	tagClanGeneralResult_ToLua Result[MAX_CLAN_TECH_RESULT];    	/*   ����Ƽ�Ч�� */
	char TypeName[RES_NAME_LEN];          	/*  Ver.417 ������ */
	int IconID;                          	/*  Ver.417 ͼ��ID */
	int NeedTech;                        	/*  Ver.870 ����Ƽ� */
	int NeedTechLevel;                   	/*  Ver.870 ����Ƽ��ĵȼ� */
};

struct tagClanMakeTechRequired_ToLua
{
	char TechCategory;                    	/*   ����Ƽ���� Bind Macrosgroup:CLAN_TECH_CATEGORY,*/
	char Level;                           	/*   ����Ƽ��ȼ� */
};

struct tagClanProductDef_ToLua
{
	int ID;                              	/*   ��ƷDefID/��е��MonsterID */
	char Category;                        	/*   ��� Bind Macrosgroup:CLAN_PRODUCT_CATEGORY,*/
	char Name[RES_NAME_LEN];              	/*   ���� */
	char DescText[SHORT_DESC_LEN];        	/*   ������Ϣ */
	int TimeCost;                        	/*   ����˻�е/��Ʒ�����ʱ��(��) */
	int NeedMoneyCost;                   	/*   ���蹫���ʽ������ */
	tagClanMakeMaterial_ToLua NeedMaterialCost[MAX_CLAN_MAKE_MATERIAL]; 	/*   ����˻�е����Ĳ��� */
	tagClanMakeTechRequired_ToLua TechRequired[MAX_CLAN_MAKE_TECH]; 	/*   ����˻�е����ĿƼ� */
	int MaxPile;                         	/*   �ɶѵ�����,ֻ����Ʒ��Ч */
	tagClanGeneralResult_ToLua Result[MAX_CLAN_ITEM_RESULT];    	/*   ������ƷЧ��,ֻ�е������Ʒ����Ʒ��ʱ�����Ч */
	char TypeName[RES_NAME_LEN];          	/*  Ver.420 ������ */
	int BuyPrice;                        	/*  Ver.449 �ڹ����̵������ļ۸�(�����ʽ���) */
	float ContributValue;                  	/*  Ver.541 ÿһ��λ����Դ�����ĸ��˹��׶� */
	int ResourceID;                      	/*  Ver.870 ��Ʒ��Ӧ����Դ��Ʒid */
	float ResourceRate;                    	/*  Ver.870 ��Ʒ��Ӧ����Դ���� */
	int ContributPrice;                  	/*  Ver.870 �һ�ʱ��Ҫ�Ĺ��׶� */
	int ProductTech;                     	/*  Ver.870 ����󹫻��ÿƼ����� */
	char RecDescText[SHORT_DESC_LEN];     	/*  Ver.913 ��Դ���Ķһ�ʱ������ */
};

enum tagCLAN_TECH_CATEGORY_TOLUA 
{
	CLAN_TECH_CATEGORY_NONE_TOLUA = 0, 
	CLAN_TECH_CATEGORY_COLLECTOR_ATTRIBUTES_TOLUA = 1, 	/* �ɿ���ǿ */
	CLAN_TECH_CATEGORY_COLLECTOR_MAKING_TOLUA = 2, 	/* �ɿ����� */
	CLAN_TECH_CATEGORY_COLLECTOR_ALTERATION_TOLUA = 3, 	/* �ɿ󳵸��� */
	CLAN_TECH_CATEGORY_MACHINE_MAKING_TOLUA = 4, 	/* ��е���� */
	CLAN_TECH_CATEGORY_TANK_MAKING_TOLUA = 5, 	/* ս������ */
	CLAN_TECH_CATEGORY_PET_TOLUA = 6, 	/* ���� */
	CLAN_TECH_CATEGORY_PET_ALTERATION_TOLUA = 7, 	/* ������� */
};

enum tagCLAN_GRID_CATEGORY_TOLUA 
{
	CLAN_GRID_ITEM_TOLUA = 0, 	/* ��Ʒ */
	CLAN_GRID_MACHINE_TOLUA = 1, 	/* ��е */
};

enum tagCLAN_STORAGE_GRID_STATE_TOLUA
{
	CLAN_GRID_LIMITED_TOLUA = CLAN_GRID_LIMITED, 	/* ����״̬ */
	CLAN_GRID_LOCKED_TOLUA = CLAN_GRID_LOCKED, 		/* ����״̬ */
	CLAN_GRID_OPEN_TOLUA = CLAN_GRID_OPEN, 			/* ����״̬ */
};

enum tagCLAN_CONSTRUCTION_INDEX_TOLUA 
{
	CLAN_IDX_TECH_CENTER_TOLUA = 0, 	/* �Ƽ����� */
	CLAN_IDX_COLLECTION_CENTER_TOLUA = 1, 	/* �ɼ����� */
	CLAN_IDX_FACTORY_TOLUA = 2, 	/* ���� */
	CLAN_IDX_FARM_TOLUA = 3, 	/* ���� */
	CLAN_IDX_WALL_TOLUA = 4, 	/* �Ƿ� */
	CLAN_IDX_CITY_TOLUA = 5, 	/* ���� */
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

// ���浱ǰ����/����������������
// ���浱ǰ����/����������������
class	GameClanCityInfo//tolua_export
{//tolua_export
public:
	//tolua_begin
	// ��������
	void upgradeClanBuilding( int nBuildingID );

	// �������
	void enableClanCity( const char* pszCityName );

	// ��������
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
