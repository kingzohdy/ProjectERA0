#pragma once
#include "OgreSingleton.h"
#include "GameEngineTypes.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "ItemDefine.h"


#define	 MAX_OPTIONBTN_NUM  7
//tolua_begin
enum 
{
	STAR_STONE_CTYSTAL	= 1,	//ˮ������
	STAR_STONE_JADE		= 2,	//������
	STAR_STONE_FIRE		= 3,	//�������
	STAR_STONE_SILVER	= 4,	//���ǻ���
	STAR_STONE_BLACK	= 5,	//�������
};

#pragma pack(1)
struct tagStarStoneDef_ToLua
{
	int ID;                              	/*   ��ID, ȡֵ1��31 */
	char Name[NAME_LEN];                  	/*   ������ */
	char StarDesc[SHORT_DESC_LEN];        	/*   ������ */
	int IconID;                          	/*   ��ͼ��id */
	int StarFuncList[MAX_STONE_FUNC];    	/*   ���Ӧ�Ĺ���id�б� */
	int OpenGodCoin[MAX_STONE_OPEN_CASE]; 	/*   ����ʱ�俪ͨ��ʽ 7�� 30�� 180�� */
};

struct tagStarFuncDef_ToLua
{
	int ID;                              	/*   �깦��id,���� �ӹ�����һ��Ҫ�ͳ���˵�� */
	char StarFuncDesc[SHORT_DESC_LEN];    	/*   �깦������ */
	char OpenLink[NAME_LEN];              	/*   �򿪡��鿴���������� */
	tagResult_ToLua StarResult;                     
};

enum tagSTAR_RESULT_TOLUA 
{
	STAR_RESULT_NONE_TOLUA = 0, 	/* ��ЧЧ�� */
	STAR_RESULT_PWORLD_CARD_TOLUA = 1, 	/* �������� */
	STAR_RESULT_STASH_TOLUA = 2, 	/* ����ֿ� */
	STAR_RESULT_FUZHU_TOLUA = 3, 	/* �������� */
	STAR_RESULT_CANGBAOTU_TOLUA = 4, 	/* �ر�ͼ����,val1��ʾ���ٴ� */
	STAR_RESULT_RONGLU_TOLUA = 5, 	/* ��¯����,val1��ʾ���ٴ� */
	STAR_RESULT_DADI_TOLUA = 6, 	/* ���֮�Ĵ���,val1��ʾ���ٴ� */
	STAR_RESULT_SHANGJIN_TOLUA = 7, 	/* �ͽ����,val1��ʾ���ٴ� */
	STAR_RESULT_SHANGSAN_TOLUA = 8, 	/* ���Ʒ���,val1��ʾ���ٴ� */
	STAR_RESULT_SHATAN_EXP_TOLUA = 9, 	/* ɳ̲����,val1 30��ʾ��ٷ�֮30 */
	STAR_RESULT_SHOP_TOLUA = 10, 	/* �����̵�,val1 NpcSellList�е�SellListID */
	STAR_RESULT_PKVAL_TOLUA = 11, 	/* ɱ��ֵ����,val1 100��ʾ2���ٶ�,200��ʾ3���ٶ� */
	STAR_RESULT_TRANS_TOLUA = 12, 	/* ���մ��� */
	STAR_RESULT_BUSY_TOLUA = 13, 	/* ����Һ�æ */
	STAR_RESULT_DEC_PKVAL_TOLUA = 14, 	/* ����ص�ʹ�ü�ɱ��ֵ���� */
	STAR_RESULT_STORE_TOLUA = 15, 	/* �������� */
};

#pragma pack()

struct tagAutoQuestData 
{
	int m_Num[MAX_OPTIONBTN_NUM];
	bool m_bChkBtn[MAX_OPTIONBTN_NUM];
};

struct tagAutoRecoveryData 
{
	int m_hpValue;
	int m_mpValue;
	int m_hpItemID;
	int m_mpItemID;
};
//tolua_end

class GameVipManager //tolua_export
	:public Ogre::Singleton<GameVipManager> , public NetMsgHandler
{//tolua_export
public:
	GameVipManager();
	~GameVipManager();

	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

	//tolua_begin
	const tagStarStoneDef_ToLua* getStarStoneDefByIdx( int nIdx );
	const tagStarStoneDef_ToLua* getStarStoneDef( int nID );
	const tagStarFuncDef_ToLua* getStarFuncDef( int nID );

	bool haveOpenVip();
	bool isEnableVip( int ID );
	int getVipEndTime( int nID );

	void requestOpenVip(  int nType , int nID );
	void openVipShop();

	int getCangBaoTuVipNum();
	int getBCTVipNum();
	int getShangShanNum();
	int getRongLuNum();
	int getShangJinNum();

	tagAutoQuestData* getAutoQuestOpt();
	void saveAutoQuestOpt();
	tagAutoRecoveryData* getAutoRecoveryOpt();
	void saveAutoRecoveryOpt();
	bool isOwnVipFunc( int nResultID );
	//tolua_end

	void reset();

private:
	void resetFuncData();
	int	 getStarFunResultVal1( int nResultID );

private:
	VIPDATA				m_vipData;
	bool				m_bRequestServer;
	std::vector<int>	m_vEnableFunc;
	tagAutoQuestData	m_autoQuestData;
	tagAutoRecoveryData m_autoRecoveryData;
};//tolua_export