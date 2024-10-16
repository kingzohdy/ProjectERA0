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
	unsigned short RoleLvlLimit;                    	/*   ��ɫ�ȼ����� */
	unsigned short ElseAttrID;                     
	unsigned short ElseAttrLvlLimit;               
};
struct tagGeniusAttrDef_ToLua
{
	unsigned char Career;                          	/*   ְҵ */
	unsigned char AttrID;                          	/*   ����ID */
	unsigned short AttrLvl;                         	/*   ���Եȼ� */
	unsigned int IconID;                         
	char AttrDesc[MAX_GENIUS_DESC];      
	char Name[NAME_LEN];                 
	unsigned int AttrVar;                         	/*   ��ǰ�ȼ���Ӧ������ֵ */
	tagGeniusUpLimit_ToLua UpLimit;                         	/*   �����ĸ������� */
	int Step;                            	/*   �� */
	int Lvl;                             	/*   �� */
};

struct tagGeniusSubOne_ToLua
{
	unsigned char AttrID;                          	/*   ����ID */
	unsigned char Lvl;                             	/*   �ȼ� */
};

struct tagGeniusOne_ToLua
{
	unsigned char Type;                            	/*   ���� */
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
	short Type;                            	/*   1-�����츳 2-���츳 */
	short Career;                          	/*  Ver.1925  */
	tagGeniusSeg_ToLua Seg[MAX_GENIUS_SEG];            
};

struct tagLvlMdf_ToLua
{
	short LowerLimit;                     
	short UpperLimit;                    
	int Mul;                             	/*   ���� |����� */
};

struct tagGeniusMdfDef_ToLua
{
	int Type;                            	/*   �츳���� */
	tagLvlMdf_ToLua Mdf[GENIUS_MDF_MAX];             	/*   �������� */
};
#pragma pack()

#define MAX_GENIUS_SUBTYPE_TOLUA MAX_GENIUS_SUBTYPE
#define MAX_GENIUS_RESULT_TOLUA MAX_GENIUS_RESULT
#define MAX_GENIUS_SEG_TOLUA MAX_GENIUS_SEG
#define GENIUS_MDF_MAX_TOLUA GENIUS_MDF_MAX

enum tagGENIUS_SUBTYPE_TOLUA 
{
	GENIUS_SUBTYPE_HP_TOLUA = 1, 
	GENIUS_SUBTYPE_PHYATK_TOLUA = 2, 	/* ������ */
	GENIUS_SUBTYPE_ICEATK_TOLUA = 3, 	/* ħ������ */
	GENIUS_SUBTYPE_PHYDEF_TOLUA = 4, 	/* ������� */
	GENIUS_SUBTYPE_ICEDEF_TOLUA = 5, 	/* ħ������ */
	GENIUS_SUBTYPE_MINGZHONG_LVL_TOLUA = 6, 	/* ���еȼ� */
	GENIUS_SUBTYPE_SHANBI_LVL_TOLUA = 7, 	/* ���ܵȼ� */
	GENIUS_SUBTYPE_BAOJI_LVL_TOLUA = 8, 	/* �����ȼ� */
	GENIUS_SUBTYPE_DIKANG_LVL_TOLUA = 9, 	/* �ֿ��ȼ� */
	GENIUS_SUBTYPE_SH_ADD_DAM_TOLUA = 10, 	/* ���ػ��˺����� */
	GENIUS_SUBTYPE_ZF_ADD_DAM_TOLUA = 11, 	/* �������˺����� */
	GENIUS_SUBTYPE_SY_ADD_DAM_TOLUA = 12, 	/* ��ʥ���˺����� */
	GENIUS_SUBTYPE_YL_ADD_DAM_TOLUA = 13, 	/* �������˺����� */
	GENIUS_SUBTYPE_JN_ADD_DAM_TOLUA = 14, 	/* �Ծ����˺����� */
	GENIUS_SUBTYPE_LH_ADD_DAM_TOLUA = 15, 	/* �������˺����� */
	GENIUS_SUBTYPE_LS_ADD_DAM_TOLUA = 16, 	/* ����ɱ�˺����� */
	GENIUS_SUBTYPE_HM_ADD_DAM_TOLUA = 17, 	/* �Ի����˺����� */
	GENIUS_SUBTYPE_SH_DEC_DAM_TOLUA = 18, 	/* �ػ����Լ��˺����� */
	GENIUS_SUBTYPE_ZF_DEC_DAM_TOLUA = 19, 	/* �������Լ��˺����� */
	GENIUS_SUBTYPE_SY_DEC_DAM_TOLUA = 20, 	/* ʥ�Ӷ��Լ��˺����� */
	GENIUS_SUBTYPE_YL_DEC_DAM_TOLUA = 21, 	/* ���׶��Լ��˺����� */
	GENIUS_SUBTYPE_JN_DEC_DAM_TOLUA = 22, 	/* ���ܶ��Լ��˺����� */
	GENIUS_SUBTYPE_LH_DEC_DAM_TOLUA = 23, 	/* ������Լ��˺����� */
	GENIUS_SUBTYPE_LS_DEC_DAM_TOLUA = 24, 	/* ��ɱ���Լ��˺����� */
	GENIUS_SUBTYPE_HM_DEC_DAM_TOLUA = 25, 	/* ������Լ��˺����� */
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