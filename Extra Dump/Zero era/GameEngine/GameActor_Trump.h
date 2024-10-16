#pragma once
#include "GameEngineTypes.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "GameDBData.h"
#include "GameActor_Pet.h"

#pragma pack(1)
//tolua_begin 

#define FAIRY_MAX_STAR_TOLUA FAIRY_MAX_STAR
#define DEC_EVOLVE_TIME_ITEMID_TOLUA DEC_EVOLVE_TIME_ITEMID
#define ZEROREELID_TOLUA ZEROREELID
struct tagTrumpId 
{
	int nHeight;
	int nLow;
};

struct tagRoleFairy_ToLua
{
	unsigned short FairyID;                        
	unsigned short FairyLevel;                     
	tagTrumpId WID;
	char Stat;                           
	unsigned int EvolveLeftTM;                   
	char GridIdx;                         	/*  Ver.136  */
	unsigned int Flag;                            	/*  Ver.150  */
	int Exp;                             	/*  Ver.191  */
	int StarLvl;                         	/*  Ver.539 �����Ǽ� */
	int TraitID;                         	/*  Ver.539 �����Ը�ID */
	int IsKeyin;                         	/*  Ver.539 �����Ƿ���������� 1�� 0�� */
};

struct tagFairySkill_ToLua
{
	unsigned char SkillType;                       	/*   0--����  1--���� */
	unsigned int SkillID;                        
	unsigned char SkillLevel;       
};

struct tagEvolveStuff_ToLua
{
	unsigned int ItemID;                         
	unsigned char ItemNum;                        
};

struct tagFairyDef_ToLua
{
	unsigned short FairyID;                         	/*   ����ID */
	unsigned short Level;                           	/*   ����ȼ� */
	char Name[RES_NAME_LEN];             
	unsigned int ModeID;                          	/*   ģ��ID */
	unsigned int BuffID;                          	/*   BuffID */
	char FairyDesc[SHORT_DESC_LEN];       	/*   �������� */
	tagFairySkill_ToLua FairySkillList[FAIRY_MAX_SKILL]; 	/*   �����б� */
	unsigned short EvolveTime;                      	/*   ����ʱ�� ��λ�� */
	unsigned char EvolveLevelLimit;                	/*   ��ɫ�����ȼ����� */
	unsigned int PhyAtk;                          	/*  Ver.133 �������� */
	unsigned int MgcAtk;                          	/*  Ver.133 ħ�������� */
	unsigned int BrkAtk;                          	/*  Ver.133 �ƻ��� */
	short BaseXp;                          	/*  Ver.183 ÿ10�����ӵ�xpֵ */
	unsigned int UseMoney;                        	/*  Ver.183 �������Ķ��ٽ�� */
	unsigned int UseExp;                          	/*  Ver.183 �������Ķ��پ��� */
	unsigned char GetXpRate;                       	/*  Ver.223 ��ɫɱ������ ��ȡxp�ĸ���  |�ٷ��� */
	int XpUpperLimit;                    	/*  Ver.240 xp���� */
	int FireAtk;                         	/*  Ver.426 �����Թ����� */
	int ThunderAtk;                      	/*  Ver.426 �����Թ����� */
	int LightAtk;                        	/*  Ver.426 �����Թ����� */
	int NightAtk;                        	/*  Ver.426 �����Թ����� */
	int WaterAtk;                        	/*  Ver.426 ˮ���Թ����� */
	unsigned char MaxStar;                         	/*  Ver.539 ������������Ǽ� */
	unsigned char TraitID;                         	/*  Ver.539 �Ը�ID */
	unsigned char PNum;                            	/*  Ver.539 �ܲ����������� */
	int PItems[FAIRY_MAX_PRODUCE];       	/*  Ver.539 ������������б� */
	int ZeroReelID;                      	/*  Ver.540 ���ֶθ�Ϊ:�������(��ӡ)ʱ��Ҫ���ߵ�����,IDд�� */
	int KeyinUseXp;                      	/*  Ver.805 �����ӡ���Ķ���Xp(ֵ) */
};

struct tagFairyStuffRate_ToLua
{
	unsigned char Star;                            	/*   ��Ӧ�Ǽ� */
	unsigned short Rate;                            	/*   �ɹ����� |����� */
};

struct tagFairyStarStuffDef_ToLua
{
	int ItemID;                         
	unsigned char MinStar;                        
	unsigned char MaxStar;                        
	tagFairyStuffRate_ToLua StuffRate[FAIRY_MAX_STAR];      
};

struct tagFairyStarStuff_ToLua
{
	int MainStuffID;                     	/*   ��������ƷID */
    int MainStuffNum;                    	/*   ������Ҫ��ô�������� */
    int Money;                           	/*   ��Ǯ */
};

struct tagFairyStarDef_ToLua
{
	short FairyID;                        
	short Star;                           
	unsigned char StarBackRate;                    	/*   �Ǽ����˸���|�ٷ��� */
	unsigned char PetExpMul;                       	/*   ��������Exp|�ٷֱ� */
	unsigned char AttrType;                        	/*   Ԫ���������� */
	int Atk;                             	/*   Ԫ���˺� */
	tagFairyStarStuff_ToLua StarUpStuff;                     	/*   Ʒ���������� */
	int StarSuccRate;                    	/*  Ver.805 �ɹ���|����� */
};


//tolua_end
#pragma pack()

class GameTrump//tolua_export 
	: public NetMsgHandler
{//tolua_export
public:
	GameTrump(void);
	~GameTrump(void);

	// ע��������Ϣ
	void initTrumpNetMessage();
	void releasePetNetMessage();
	// ����ӵ����
	void setOwner( GameActor* pOwner );

public:
	//tolua_begin 
	// ��������
	void trumpPre( int nHightId, int nLowId, int nGridIdx );
	// ��������
	void trumpEvolve( int nHightId, int nLowId, int nGridIdx );
	// ֹͣ����
	void trumpStopEvolve( int nHightId, int nLowId, int nGridIdx );
	// �����ٻ�
	void trumpBack( int nHightId, int nLowId, int nGridIdx );
	// ע�뾭��ֵ
	void trumpInfuseExp( int nHightId, int nLowId, int nGridIdx, int nIntoExp );
	//tolua_end

	// ��ʼ��������Ϣ
	void initTrumpPackage( const FAIRYPACKAGE& fairypackage );
	// s->c
	virtual int HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );

public:
	//tolua_begin 
	// ��ȡ�������������
	int getTrumpMaxNum(){ return m_nRoleMaxTrumpNum; }
	// ��ȡ��������
	int getTrumpNum(){ return m_nRoleTrumpNum; }

	// ��ȡ������Ϣ
	tagRoleFairy_ToLua* getRoleFairy( int nGridIdx );
	// ��ȡ��ǰ������������Ϣ
	tagRoleFairy_ToLua* getCampRoleFairy();
	// �Ƿ��о������
	bool isAnyFairyCamp();
	// ��ȡ������̬��Ϣ
	tagFairyDef_ToLua* getFairyDef( int nId, int nLv );
	// �����Ƿ��ڳ�������
	bool isPre();
	// �ж��Ƿ��ܹ��ٻس�������
	bool canBackTrump();
	// �ж��Ƿ��ܹ�������������
	bool canCampaignTrump();
	// �ж��Ƿ����ڸ�����������
	bool canCampInPworld( int nRealMapId );
	// �жϳ����״̬
	bool isFairyInState( int nFairyState, int nState );
	// ��ȡ���ڳ����ľ���ID
	int getCampFairyId()
	{
		return m_nCurCampFairyId;
	}

	int getCampFairyLv()
	{
		return m_nCurCampFairyLv;
	}
	const tagFairyStarStuffDef_ToLua*	getFairyStarStuffDef( int itemId );
	const tagFairyStarDef_ToLua*		getFairyStarDef( int nFairyId,int nStar );
	const tagPetTraitDef_ToLua*			getPetTraitDef( int nTraidId,int nStar );


	void requestOpStarUp( int nIdx,int nStuffID,int nStuffNum,int nMoney );
	void requestOpKeyIn(int nIndex);
	void requestKeyInItem( int nSrcList,int nSrcGrid,int nDestList,int nDestGrid );

	void requestDecEvolveTime( int nIdx );

	//����ǿ����Ҫ�ĵȼ�
	int getStrengthFairyLevel( int fairyID, int StarLv );

	//��ɫ�뿪��Ϸ��ʱ����վ�������
	void clearTrumpData();
	//tolua_end

private:
	void setTrumpAttr( const FAIRYDATA& fairydata );

private:
	tagRoleFairy_ToLua      m_RoleTrump[FAIRY_MAX_NUM];
	int                     m_nRoleMaxTrumpNum;
	int                     m_nRoleTrumpNum;
	bool                    m_bPre;
	GameActor*				m_pOwner;

public:
	int						m_nCurCampFairyId;
	int						m_nCurCampFairyLv;
};//tolua_export 

