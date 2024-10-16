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
	CLIENT_SECONDRAY_SKILL_TYPE_MINING = 1, 	/* �ɿ� */
	CLIENT_SECONDRAY_SKILL_TYPE_HERBALISM = 2, 	/* ��ҩ */
	CLIENT_SECONDRAY_SKILL_TYPE_RESOLVE = 3, 	/* �ֽ� */
	CLIENT_SECONDRAY_SKILL_TYPE_BLACKSMITHING = 4, 	/*  ���� */
	CLIENT_SECONDRAY_SKILL_TYPE_ALCHEMY = 5, 	/* ���� */
	CLIENT_SECONDRAY_SKILL_TYPE_ENCHANT = 6, 	/* ��ħ */
	CLIENT_SECONDRAY_SKILL_TYPE_ENGINEERING = 7, 	/* ����ѧ(�����Ƽ�) */
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
	tagRecipeGetItem_toLua GetItem[MAX_RECIPE_GET_ITEM];    	/*   ��������Ʒ */
};

struct tagRecipeDef_toLua
{
	unsigned short SkillType;                       	/*   ��������: �ɿ��ҩ�ȵ� Bind Macrosgroup:SECONDARY_SKILL_TYPE,*/
	unsigned short RecipeID;                        	/*   ���ܵȼ�,10��һ�Σ�����11  ����1�� */
	char NameDesc[RES_NAME_LEN];          				/*   �䷽���� */
	char RecipeDesc[SHORT_DESC_LEN];      				/*   �䷽���� */
	int RecipelLevel;                    				/*   �䷽�ȼ�,�������������ȵ��ж� */
	unsigned short LearnType;                       	/*   ѧϰ��ʽ:0 ��� 1 ���� Bind Macrosgroup:LEARN_RECIPE_TYPE,*/
	unsigned short LearnSkillLevel;                 	/*   ѧϰ���ܵȼ�Ҫ�� */
	int LearnMoneyType;                  				/*   ѧϰ���ܽ�ǮҪ�� */
	int LearnMoneyVal;                   				/*   ѧϰ���ܽ�ǮҪ�� */
	tagRecipeStuff_toLua Stuff[MAX_RECIPE_STUFF];         	/*   ��Ҫ�Ĳ��� */
	int ConsumeVim;                      				/*   ���ĵľ��� */
	int MachiningTime;                   				/*   ����ʱ�䵥λ�� */
	int CoolDownTime;                    				/*   ��ȴʱ�䵥λ�� */
	int RecipeType1;                     				/*   ɸѡ���� */
	int RecipeType2;                     				/*   ɸѡ���� */
	int IconID;                          	/*  Ver.309 �䷽��ͼ�� */
	tagRecipeProduce_toLua ProduceList[MAX_RECIPE_GET_ITEM_LIST]; 	/*  Ver.310 ��������Ʒ */
	int CostMoneyType;                   	/*  Ver.1636 �䷽�����ǮҪ�� */
	int CostMoneyVal;                    	/*  Ver.1636 �䷽���켼�ܽ�ǮҪ�� */
};


struct tagSecondarySkillDef_toLua
{
	unsigned short SkillType;                       	/*   ��������:�ɿ��ҩ�ȵ� Bind Macrosgroup:SECONDARY_SKILL_TYPE,*/
	unsigned short Level;                           	/*   ���ܵȼ���10��һ�Σ�����11  ����1�� */
	char NameDesc[RES_NAME_LEN];          	/*   ������ϰ�������� */
	int LevelUpVal;                      	/*   �������������� */
	int AddVal;                          	/*  Ver.306 �ɼ�������ÿ�����ӵ������� */
	unsigned short RecipeID[MAX_AUTO_LEARN_RECIPE]; 	/*  Ver.371 ѧϰ���ܺ��Զ���õ��䷽ */
};

struct tagRecipeCoolDownInfo_toLua
{
	int RecipeID;
	int CoolDownTime;
};

struct tagSecondarySkillInfo_toLua
{
	unsigned short SkillType;                       	/*   ��������: �ɿ��ҩ�ȵ� Bind Macrosgroup:SECONDARY_SKILL_TYPE,*/
	unsigned short SkillLevel;                      	/*   ���ܵȼ� */
	unsigned int SkillVal;                        	/*   ���������� */
	char RecipeInfo[MAX_SECONDARY_SKILL_RECIPE];
	int CoolDownNum;                    
	tagRecipeCoolDownInfo_toLua CoolDownInfo[MAX_SECONDARY_SKILL_COOLDOWN_RECIPE];
};

struct tagSecondarySkillMiscInfo_toLua
{
	int SkillNum;                       
	tagSecondarySkillInfo_toLua SkillInfo[MAX_SECONDARY_SKILL];  	/*   �������Ϣ */
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
	//ѧϰ�䷽
	void requestLearnRecipt(int nType,int id);
	//��ʼ��������
	void requestReciptPre(int nType,int id);
	//�����������
	void requestBreakReciptPre();

	void requestUseSkillPre(int nType,int id);
	//�ֽ�
	void requestReciptResolve(int nList,int gridIdx);
	//ɾ�������
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

	//�Բɼ�������Ҫ���ж�
	bool checkRequireItem(int nresId);

	int getNonCommSecondaySkillNum();

	bool hasSecondarySkill(int nType,int nSkillLevel);

	bool canLearnNextLevel( int nType,int nSkillLevel );

private:
	tagSecondarySkillMiscInfo	m_SecondaryData;
	tagExtendedResult_ToLua		m_ExtendResult;

};//tolua_export