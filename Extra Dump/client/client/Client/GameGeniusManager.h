#pragma once
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "ItemDefine.h"

//tolua_begin
#pragma pack(1) 
struct tagGeniusType_ToLua
{
	unsigned char Type;                           
	int AttrIDs[MAX_GENIUS_SUBTYPE];    
};

struct tagGeniusCareerDef_ToLua
{
	int Career;                         
	tagGeniusType_ToLua Types[MAX_GENIUS_TYPE];         
};

struct tagGeniusUpLimit_ToLua
{
	unsigned int UpUseExp;                       
	int UpUseMoneyType;                 
	int UpUseMoney;                     
	unsigned short RoleLvlLimit;                    	/*   角色等级限制 */
	unsigned short ElseAttrID;                     
	unsigned short ElseAttrLvlLimit;               
};
struct tagGeniusAttrDef_ToLua
{
	unsigned char Career;                          	/*   职业 */
	unsigned char AttrID;                          	/*   属性ID */
	unsigned short AttrLvl;                         	/*   属性等级 */
	unsigned int IconID;                         
	char AttrDesc[MAX_GENIUS_DESC];      
	char Name[NAME_LEN];                 
	unsigned int AttrVar;                         	/*   当前等级对应的属性值 */
	tagGeniusUpLimit_ToLua UpLimit;                         	/*   升级的各种限制 */
	int Step;                            	/*   阶 */
	int Lvl;                             	/*   级 */
};

struct tagGeniusSubOne_ToLua
{
	unsigned char AttrID;                          	/*   属性ID */
	unsigned char Lvl;                             	/*   等级 */
};

struct tagGeniusOne_ToLua
{
	unsigned char Type;                            	/*   类型 */
	unsigned char Num;                            
	tagGeniusSubOne_ToLua SubOne[MAX_GENIUS_SUBTYPE];     
};

struct tagGeniusInfo_ToLua
{
	unsigned char Num;                            
	tagGeniusOne_ToLua One[MAX_GENIUS_TYPE];           
};

struct tagGeniusSeg_ToLua
{
	int From;                           
	int To;                             
	tagResult_ToLua Add[MAX_GENIUS_RESULT];         
};

struct tagGeniusAddDef_ToLua
{
	short Type;                            	/*   1-基础天赋 2-狂暴天赋 */
	short Career;                          	/*  Ver.1925  */
	tagGeniusSeg_ToLua Seg[MAX_GENIUS_SEG];            
};

struct tagLvlMdf_ToLua
{
	short LowerLimit;                     
	short UpperLimit;                    
	int Mul;                             	/*   修正 |万分率 */
};

struct tagGeniusMdfDef_ToLua
{
	int Type;                            	/*   天赋类型 */
	tagLvlMdf_ToLua Mdf[GENIUS_MDF_MAX];             	/*   消耗修正 */
};
#pragma pack()

#define MAX_GENIUS_SUBTYPE_TOLUA MAX_GENIUS_SUBTYPE
#define MAX_GENIUS_RESULT_TOLUA MAX_GENIUS_RESULT
#define MAX_GENIUS_SEG_TOLUA MAX_GENIUS_SEG
#define GENIUS_MDF_MAX_TOLUA GENIUS_MDF_MAX

enum tagGENIUS_SUBTYPE_TOLUA 
{
	GENIUS_SUBTYPE_HP_TOLUA = 1, 
	GENIUS_SUBTYPE_PHYATK_TOLUA = 2, 	/* 物理攻击 */
	GENIUS_SUBTYPE_ICEATK_TOLUA = 3, 	/* 魔法攻击 */
	GENIUS_SUBTYPE_PHYDEF_TOLUA = 4, 	/* 物理防御 */
	GENIUS_SUBTYPE_ICEDEF_TOLUA = 5, 	/* 魔法防御 */
	GENIUS_SUBTYPE_MINGZHONG_LVL_TOLUA = 6, 	/* 命中等级 */
	GENIUS_SUBTYPE_SHANBI_LVL_TOLUA = 7, 	/* 闪避等级 */
	GENIUS_SUBTYPE_BAOJI_LVL_TOLUA = 8, 	/* 暴击等级 */
	GENIUS_SUBTYPE_DIKANG_LVL_TOLUA = 9, 	/* 抵抗等级 */
	GENIUS_SUBTYPE_SH_ADD_DAM_TOLUA = 10, 	/* 对守护伤害加深 */
	GENIUS_SUBTYPE_ZF_ADD_DAM_TOLUA = 11, 	/* 对征服伤害加深 */
	GENIUS_SUBTYPE_SY_ADD_DAM_TOLUA = 12, 	/* 对圣佑伤害加深 */
	GENIUS_SUBTYPE_YL_ADD_DAM_TOLUA = 13, 	/* 对御雷伤害加深 */
	GENIUS_SUBTYPE_JN_ADD_DAM_TOLUA = 14, 	/* 对聚能伤害加深 */
	GENIUS_SUBTYPE_LH_ADD_DAM_TOLUA = 15, 	/* 对练魂伤害加深 */
	GENIUS_SUBTYPE_LS_ADD_DAM_TOLUA = 16, 	/* 对猎杀伤害加深 */
	GENIUS_SUBTYPE_HM_ADD_DAM_TOLUA = 17, 	/* 对毁灭伤害加深 */
	GENIUS_SUBTYPE_SH_DEC_DAM_TOLUA = 18, 	/* 守护对自己伤害减少 */
	GENIUS_SUBTYPE_ZF_DEC_DAM_TOLUA = 19, 	/* 征服对自己伤害减少 */
	GENIUS_SUBTYPE_SY_DEC_DAM_TOLUA = 20, 	/* 圣佑对自己伤害减少 */
	GENIUS_SUBTYPE_YL_DEC_DAM_TOLUA = 21, 	/* 御雷对自己伤害减少 */
	GENIUS_SUBTYPE_JN_DEC_DAM_TOLUA = 22, 	/* 聚能对自己伤害减少 */
	GENIUS_SUBTYPE_LH_DEC_DAM_TOLUA = 23, 	/* 练魂对自己伤害减少 */
	GENIUS_SUBTYPE_LS_DEC_DAM_TOLUA = 24, 	/* 猎杀对自己伤害减少 */
	GENIUS_SUBTYPE_HM_DEC_DAM_TOLUA = 25, 	/* 毁灭对自己伤害减少 */
};

//tolua_end

class GameGeniusManager//tolua_export
	:public NetMsgHandler, public Ogre::Singleton<GameGeniusManager>
{//tolua_export
public:
	GameGeniusManager();
	~GameGeniusManager(){};

public:
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

	//tolua_begin
	const tagGeniusAttrDef_ToLua* getGeniusAttrDef( int nCareer, int nAttrId, int nLv );
	const tagGeniusCareerDef_ToLua*	getGeniusCareerDef( int nCareer );
	const tagGeniusAddDef_ToLua* getGeniusAddDef( int nType, int nCareer );
	const tagGeniusMdfDef_ToLua* getGeniusMdfDef( int nType );

	void Reset();
	int getAttrLv( int nIdx, int nAttrId );
	int getGeniusType( int nIdx );
	int getAllGeniusValue();
	int getGeniusTypeValue( int nType );

	void LevelUp( int nType, int nAttrId, int nLv, int nFlag );
	//tolua_end

private:
	tagGeniusInfo_ToLua m_GeniusInfo;
};//tolua_export