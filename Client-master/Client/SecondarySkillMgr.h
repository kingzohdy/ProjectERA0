#pragma once

#include "GameEngineTypes.h"
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "GameDefineTolua.h"
#include "ItemDefine.h"

#pragma pack(1)

//tolua_begin

#define MAX_RECIPE_STUFF 4
#define MAX_RECIPE_GET_ITEM 4

#define MAX_RECIPE_NUM 1024
enum
{
	LEARN_TYPE_PANEL = 0,
	LEARN_TYPE_QUEST = 1,
};

enum tagSECONDARY_SKILL_TYPE_toLua 
{
	CLIENT_SECONDRAY_SKILL_TYPE_MINING = 1, 	/* 采矿 */
	CLIENT_SECONDRAY_SKILL_TYPE_HERBALISM = 2, 	/* 采药 */
	CLIENT_SECONDRAY_SKILL_TYPE_RESOLVE = 3, 	/* 分解 */
	CLIENT_SECONDRAY_SKILL_TYPE_BLACKSMITHING = 4, 	/*  锻造 */
	CLIENT_SECONDRAY_SKILL_TYPE_ALCHEMY = 5, 	/* 炼金 */
	CLIENT_SECONDRAY_SKILL_TYPE_ENCHANT = 6, 	/* 附魔 */
	CLIENT_SECONDRAY_SKILL_TYPE_ENGINEERING = 7, 	/* 工程学(蒸汽科技) */
};

struct tagRecipeStuff_toLua
{
	unsigned int ItemID;                         
	unsigned int Num;                            
};

struct tagRecipeGetItem_toLua
{
	unsigned int ItemID;                         
	unsigned short Num;                            
	unsigned short Rate;                           
};

struct tagRecipeProduce_toLua
{
	tagRecipeGetItem_toLua GetItem[MAX_RECIPE_GET_ITEM];    	/*   产出的物品 */
};

struct tagRecipeDef_toLua
{
	unsigned short SkillType;                       	/*   技能类型: 采矿采药等等 Bind Macrosgroup:SECONDARY_SKILL_TYPE,*/
	unsigned short RecipeID;                        	/*   技能等级,10级一段，例如11  熟练1级 */
	char NameDesc[RES_NAME_LEN];          				/*   配方名字 */
	char RecipeDesc[SHORT_DESC_LEN];      				/*   配方描述 */
	int RecipelLevel;                    				/*   配方等级,用于增加熟练度的判断 */
	unsigned short LearnType;                       	/*   学习方式:0 面板 1 道具 Bind Macrosgroup:LEARN_RECIPE_TYPE,*/
	unsigned short LearnSkillLevel;                 	/*   学习技能等级要求 */
	int LearnMoneyType;                  				/*   学习技能金钱要求 */
	int LearnMoneyVal;                   				/*   学习技能金钱要求 */
	tagRecipeStuff_toLua Stuff[MAX_RECIPE_STUFF];         	/*   需要的材料 */
	int ConsumeVim;                      				/*   消耗的精力 */
	int MachiningTime;                   				/*   制造时间单位秒 */
	int CoolDownTime;                    				/*   冷却时间单位秒 */
	int RecipeType1;                     				/*   筛选分类 */
	int RecipeType2;                     				/*   筛选分类 */
	int IconID;                          	/*  Ver.309 配方的图标 */
	tagRecipeProduce_toLua ProduceList[MAX_RECIPE_GET_ITEM_LIST]; 	/*  Ver.310 产出的物品 */
	int CostMoneyType;                   	/*  Ver.1636 配方制造金钱要求 */
	int CostMoneyVal;                    	/*  Ver.1636 配方制造技能金钱要求 */
};


struct tagSecondarySkillDef_toLua
{
	unsigned short SkillType;                       	/*   技能类型:采矿采药等等 Bind Macrosgroup:SECONDARY_SKILL_TYPE,*/
	unsigned short Level;                           	/*   技能等级，10级一段，例如11  熟练1级 */
	char NameDesc[RES_NAME_LEN];          	/*   描述见习、熟练等 */
	int LevelUpVal;                      	/*   升级所需熟练度 */
	int AddVal;                          	/*  Ver.306 采集、生产每次增加的熟练度 */
	unsigned short RecipeID[MAX_AUTO_LEARN_RECIPE]; 	/*  Ver.371 学习技能后自动获得的配方 */
};

struct tagRecipeCoolDownInfo_toLua
{
	int RecipeID;
	int CoolDownTime;
};

struct tagSecondarySkillInfo_toLua
{
	unsigned short SkillType;                       	/*   技能类型: 采矿采药等等 Bind Macrosgroup:SECONDARY_SKILL_TYPE,*/
	unsigned short SkillLevel;                      	/*   技能等级 */
	unsigned int SkillVal;                        	/*   技能熟练度 */
	char RecipeInfo[MAX_SECONDARY_SKILL_RECIPE];
	int CoolDownNum;                    
	tagRecipeCoolDownInfo_toLua CoolDownInfo[MAX_SECONDARY_SKILL_COOLDOWN_RECIPE];
};

struct tagSecondarySkillMiscInfo_toLua
{
	int SkillNum;                       
	tagSecondarySkillInfo_toLua SkillInfo[MAX_SECONDARY_SKILL];  	/*   生活技能信息 */
};

//tolua_end
#pragma pack()


class SecondarySkillMgr //tolua_export
	:public NetMsgHandler,public Ogre::Singleton<SecondarySkillMgr>
{//tolua_export
public:
	SecondarySkillMgr();
	~SecondarySkillMgr();

	//////////////////////////////////////////////////////////////////////////
	virtual int	HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );

	//tolua_begin
	//学习配方
	void requestLearnRecipt(int nType,int id);
	//开始制造吟唱
	void requestReciptPre(int nType,int id);
	//打断制造吟唱
	void requestBreakReciptPre();

	void requestUseSkillPre(int nType,int id);
	//分解
	void requestReciptResolve(int nList,int gridIdx);
	//删除生活技能
	void requestDelSkill(int nSkillType);

	bool isLearnedRecipe( int nType,int id );

	int	getSecondarySkillNum();
	
	tagSecondarySkillInfo_toLua* getSecondarySkillInfo( int index );

	tagSecondarySkillInfo_toLua* getSecondarySkillInfoByType( int type );

	tagSecondarySkillDef_toLua* getSecondarySkillData(int nType,int nLevel);

	tagRecipeDef_toLua*	getRecipeDef(int nType,int id);

	void clearSecondarySkillData();

	bool startCollect(int nType,int nRecipeID);

	bool canLearnSkill(int nType,int nRecipeId);

	void endCollect();

	bool isLearnedSecondarySkill( int nType );

	bool isInSeondarySkillPre();

	bool isLearnedSecondarySkill(int nType,int nSkillLevel);

	bool HasRecipeType1( int SkillType,int nType );
	bool LinkHasRecipeType1( int nLinkId,int nType );
	bool HasRecipeType2( int SkillType,int nType );

	void setSecondarySkillLink( int skillType );

	bool isLinkLearnedRecipe(int linkId,int recipeID);

	void requestUseEnchant( int nSrcList,int nSrcGrid ,int nDestList,int nDestGrid );

	tagRecipeCoolDownInfo_toLua* getSkillCoolInfo( int skillType, int recipeId );

	tagExtendedResult_ToLua*		getExtendResult();
	void saveExtendResult( int nFromID, int nDestList,int nDestGrid );

	//tolua_end
	
	bool canCollect(int nresId);

	bool checkCollect(int nresId);

	void learnRecipe(int nType,int nRecipeID);

	//对采集道具需要的判断
	bool checkRequireItem(int nresId);

	int getNonCommSecondaySkillNum();

	bool hasSecondarySkill(int nType,int nSkillLevel);

	bool canLearnNextLevel( int nType,int nSkillLevel );

private:
	tagSecondarySkillMiscInfo	m_SecondaryData;
	tagExtendedResult_ToLua		m_ExtendResult;

};//tolua_export