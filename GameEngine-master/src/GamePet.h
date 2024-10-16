#pragma once
#include "GameEngineTypes.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "GameDBData.h"

const int PET_MAX_FIGHT_SHIS = 3;

#pragma pack(1)

//tolua_begin
enum  
{
	CLIENT_PET_STAT_FIGHT = 1, 		/* 出征 */
	CLIENT_PET_STAT_SEEFIGHT = 2, 	/* 见习 */
	CLIENT_PET_STAT_DEAD = 4, 		/* 死亡 */
};

struct tagPetId
{
	int nHeight;
	int nLow;
};

struct tagRoleArm_ToLua
{
	int DefID;                          
	int DefPos;                         
	unsigned short Num;                            
	short GridIdx;                        
	tdr_ulonglong WID;                             	/*   物品唯一标识, 请保证此字段和之前的字段与RoleItem完全相同 */
	unsigned int Dur;                             	/*  Ver.8 装备损耗度 */
	int ValidEnd;                        	/*  Ver.8 有效结束时间，用于罩装 */
};

struct tagPetArm_ToLua
{
	unsigned char Num;                            
	unsigned char MaxNum;                         
	unsigned char AddedCount;                      	/*   额外装备栏计数 */
	tagRoleArm_ToLua Arms[MAX_PET_ARM];              
};

struct tagPetSkillOne_ToLua
{
	unsigned short SkillID;                        
	unsigned char SkillLevel;                     
	unsigned char Res;                            
	unsigned int SkillCollectNum;     
};

struct tagPetDiathesisOne_ToLua
{
	unsigned short DiathesisID;                    
	unsigned char Level;                          
	unsigned char Res;                            
};

struct tagPetSkill_ToLua
{
	unsigned char SkillNum;                       
	unsigned char MaxNum;                         
	unsigned char DiathesisNum;                   
	unsigned char Res;                            
	tagPetSkillOne_ToLua Skills[MAX_PET_SKILL];          
	tagPetDiathesisOne_ToLua Diathesis[MAX_PET_DIATHESIS];   
};

struct tagPetAttr_ToLua
{
	unsigned char Num;                            
	unsigned char ForgetNum;                      
	unsigned short AddAttrID[MAX_PET_ATTR];        
	unsigned short AddAttrIDForget[MAX_PET_ATTR_FORGET_SAVE];
};

struct tagRolePet_ToLua
{
	unsigned short PetID;                          
	char Name[ROLE_NAME_LEN_CLT];        
	tagPetId WID;									/*   宠物唯一标识 */
	unsigned int Exp;                             	/*   宠物经验值 */
	unsigned int SkillPoint;                      	/*   宠物剩余技能点 */
	unsigned int SkillPointTotal;                 	/*   宠物总共获得的技能点 */
	unsigned int MaxPower;                        	/*   仙子的最大MP，侍卫的最大HP */
	unsigned int Power;                           	/*   仙子的MP，侍卫的HP */
	unsigned char Level;                          
	unsigned char Hierarchy;                       	/*   第几代 */
	unsigned char GenEggNum;                       	/*   生蛋的次数 */
	unsigned char ZhuanShen;                       	/*   几转的宠物 */
	unsigned int QianLi;                          	/*   潜力 */
	unsigned int NengLi;                          	/*   能力 */
	unsigned int PingZhi;                         	/*   品质 */
	unsigned int QianZhi;                         	/*   潜质 */
	unsigned int Age;                             	/*   年龄 */
	tagPetArm_ToLua PetArms;                        
	tagPetSkill_ToLua PetSkills;                      
	tagPetAttr_ToLua PetAttrs;                      /*   宠物关键字属性 */
	int PetDefIdx;                      
	char GridIdx;                         			/*   栏位格子的索引 */
	unsigned char StatFlag;                        	/*   状态 */
	unsigned char AddSkillGrid;                    	/*  Ver.55 额外技能栏数量 */
};

struct tagPetDef_ToLua
{
	unsigned int ID;                             
	unsigned int MouldID;							  	/*  Ver.53  */
	char Name[NAME_LEN];                 
	unsigned char Type;								  	/*   宠物的形 Bind Macrosgroup:PET_TYPE,*/
	unsigned char Race;								  	/*   宠物的族 */
	unsigned char SubRace;							  	/*   宠物的子类 */
	unsigned char MaxLevel;							  	/*   等级上限 */
	unsigned char BornLevel;						 	/*   出生时的初始等级 */
	unsigned char MaxAge;							 	/*   最大寿命 */
	unsigned short MaxGenEgg;                       	/*   最多下多少次蛋 */
	PETGENEGG GenEggs[MAX_PET_GENEGG_ID];				/*   可以生的蛋的列表 */
	unsigned short IceAtk;                          	/*   冰属性攻击力 */
	unsigned short IceDef;                          	/*   冰属性防御力 */
	unsigned short FireAtk;                         	/*   火属性攻击力 */
	unsigned short FireDef;                         	/*   火属性防御力 */
	unsigned short ThunderAtk;                      	/*   雷属性攻击力 */
	unsigned short ThunderDef;                      	/*   雷属性防御力 */
	unsigned short LightAtk;                        	/*   光属性攻击力 */
	unsigned short LightDef;                        	/*   光属性防御力 */
	unsigned short NightAtk;                        	/*   暗属性攻击力 */
	unsigned short NightDef;                        	/*   暗属性防御力 */
	unsigned short PhsicAtk;                        	/*   物理属性攻击力 */
	unsigned short PhsicDef;						   	/*   物理属性防御力 */
	ATKERHURTCHG AtkerHurtChg;						  	/*   伤害修正 */
	ATKEEHURTCHG AtkeeHurtChg;							/*   抵抗伤害修正 */
	unsigned short Hit;                             	/*   命中力 */
	unsigned short Flee;                            	/*   闪避力 */
	short TiShen;									 	/*   侍卫替身的概率 */
	unsigned short SubRaceLevelStart;               	/*   企划不需要填写 */
	unsigned short SubRaceLevelEnd;                 	/*   企划不需要填写 */
	char PetDesc[SHORT_DESC_LEN];						/*  Ver.53 宠物描述 */
	unsigned short BasicSkills[MAX_PET_BASIC_SKILL]; 	/*   宠物基本技能表，面板可学习 */
	unsigned short BasicDiathesis[MAX_PET_BASIC_DIATHESIS]; 	/*   宠物基本素质表，面板可学习 */
	unsigned int HideSkills[MAX_PET_HIDE_SKILL];  		/*   宠物隐藏技能表 */
	unsigned short BornSkills;                      	/*  Ver.56 宠物出生技能 */
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
	tagAttrResult_ToLua Result[MAX_ADDATTR_RESULT];     
};

#pragma pack()
//tolua_end

class GamePet//tolua_export 
	: public NetMsgHandler
{//tolua_export
public:
	GamePet(void);
	~GamePet(void);

public:
	// C->S
	
	//tolua_begin
	// 涅槃 
	void petNirvana( int nHightId, int nLowId  );
	// 下蛋
	void petEgg( int nHightId, int nLowId );
	// 准备出征
	void petPre( int nHightId, int nLowId );
	// 出征
	void petCampaign( int nHightId, int nLowId );
	// 见习
	void petNoviciate( int nHightId, int nLowId );
	// 后备
	void petInSupport( int nHightId, int nLowId );
	// 转生
	void petRelive( int nHightId, int nLowId );
	// 技能学习( type = 0 技能，1 素质)
	void petSkillUp( int nType, int nId, int nLv, int nHightId, int nLowId );
	// 修改宠物名
	void petChangeName( int nHightId, int nLowId, const char* szName );
	// 放生侍卫
	void petFreedomShi( int nIndex );
	// 放生仙子
	void petFreedomXian( int nIndex );
	// 宠物穿装备
	void petWearArm( int nArmId, int nListType, int nGridInex, int nHightId, int nLowId );
	// 宠物脱装备
	void petUnWearArm( int nArmId, int nListType, int nGridInex, int nHightId, int nLowId );
	//tolua_end

	// S->C

	// 注册网络消息
	void initPetNetMessage();
	// 宠物包裹变化
	void petPackageChange( PETPACKAGECHG* pPackageChange );
	// 网络协议处理
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

public:
	// 设置拥有者
	void setOwner( GameActor* pOwner );

	//tolua_begin
	// 初始化宠物栏位
	void initPetPackage( tagPetPackage* petInfo );
	// 获取仙子数量
	int getXianNum();
	// 获取侍卫数量
	int getShiNum();
	// 获取仙子数量
	int getXianMaxNum();
	// 获取侍卫数量
	int getShiMaxNum();
	// 获取宠物当前等级最大经验( type 0 仙子 1宠物 )
	int getPetMaxExp( int nType, int nlv );
	// 获取宠物战斗力
	int getPetFightVal( int id );

	// 获取宠物关键字信息
	tagAddAttrDef_ToLua* getAddAttrDef( int id );
	// 获取宠物信息( type 0 仙子 1宠物 )
	tagRolePet_ToLua* getPetInfo( int nType, int nIndex );	
	// 获取宠物静态信息
	tagPetDef_ToLua* getPetDefInfo( int id );
	// 获得侍卫实体
	GameActor* getPetActor( int nXieDaiPos );
	//tolua_end

	// 设置侍卫模型出征
	void setShiModle( int nOwnerId, int nXieDaiPos, int nPetId, const char* szPetName, int nPower, int nMaxPower );
	// 重新显示侍卫模型
	void reShowModle();
	// 设置宠物位置
	void setPetPostion( int nXieDaiPos );

	// 宠物移动
	void moveTo( int nXiDaiPos, int to_x, int to_z, int dis );
	// 刷新宠物移动熟读
	void updatePetMoveSpe();
private:
	// 设置宠物属性
	void setPetAttr( ROLEPET& petAttr, CSPETATTRCHG* data );
	
private:
	GameActor*	m_pOwner;
	int			m_nRoleXianNum;
	int			m_nRoleXianMaxNum;
	int			m_nRoleShiNum;
	int			m_nRoleShiMaxNum;
	ROLEPET		m_RoleXianPets[MAX_PET_XIAN_PACKAGE];
	ROLEPET		m_RoleShiPets[MAX_PET_SHI_PACKAGE];

	GameActor*	m_pShiActor[PET_MAX_FIGHT_SHIS];			// 侍卫模型
};//tolua_export
