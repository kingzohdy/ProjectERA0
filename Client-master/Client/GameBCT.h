#pragma once

#include "GameEngineTypes.h"
#include "proto_comm.h"
#include "proto_cs.h"

#include <map>

//tolua_begin
struct BCTHistory
{
	int TopTime;                      	/*   ������ʷ��ʱ��߼�¼ */
	int TopTimeMs;                    	/*  Ver.1472 ������ʷ��ʱ��߼�¼|�ٷ��� */
	int NextTopUseTime;                  	/*   �²���ʷ��ʱ��߼�¼ */
	int NextTopUseTimeMs;                	/*  Ver.1472 �²���ʷ��ʱ��߼�¼|�ٷ��� */
	int SelfTime;                  		/*   �Լ���ʱ�ļ�¼ */
	int SelfTimeMs;                	/*  Ver.1472 �Լ���ʱ�ļ�¼|�ٷ��� */
	int SelfWhen;               		/*   ��¼ʱ�� */
	char TopName[NAME_LEN];             /*   ��߼�¼������ */
	unsigned int UpdateTime;			/*	 ����ʱ�� */

	BCTHistory()
	{
		memset(this, 0, sizeof(BCTHistory));
	}
};

struct BCTBoxItem
{
	int Num;
	int ItemID[BCT_ITEM_NUM];
	int ItemNum[BCT_ITEM_NUM];
};

struct BCTMonsterType
{
	int		Num;
	int		MonsterID[BCT_MON_TYPE_NUM];
	int		Level[BCT_MON_TYPE_NUM];
};

struct BCTBoxStatistic
{
	int id;
	int num;
};

struct BCTAwardItem
{
	int itemID;
	int itemNum;
};
//tolua_end

class GameBCT//tolua_export
	: public NetMsgHandler
{//tolua_export
protected:
	GameBCT();

public:
	~GameBCT();

private:
	void	setHistory(int layer, const BCTLAYERHISTORY& info, unsigned int updateTime = 0);

public:
	virtual int		HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	void			onEnterPworld( int nMapId );
	void			onLeavePworld( int nMapId );

public:
	static GameBCT & getSingleton();

public:
	//tolua_begin
	bool				isIn();
	const BCTHistory *	getHistory(int layer);
	void				updateHistory(int layer);
	const char *		getDesciption(int layer);
	int					getAwardExp(int layer);
	BCTBoxItem			getBoxItem(int layer);
	BCTMonsterType		getMonsterType(int layer);
	const char *		getMonsterName(int id);
	int					getMonsterIcon(int id);
	bool				hasBox(int layer);
	int					getBoxIcon(int layer);
	void				EnterPworld(int layer, bool useItem);
	void				ContinuePworld(int layer);
	void				RepeatPworld(int layer);
	void				ExitPworld();
	int					getNextBoxLayer(int layer);
	BCTBoxStatistic	*	getLayerItem(int layer);

	bool				IsAwardReceive(int layer);
	void				GetAward( int layer );
	BCTAwardItem		GetAwardItem( int layer, int idx );
	//tolua_end

private:
	int		m_nPworldID;
	std::map<int, BCTBoxStatistic>	m_boxStatistic;

	std::map<int, BCTHistory>	m_history;

	unsigned int	m_LayerAward[MAX_AWARD_LAYER];		//������ȡ��¼

};//tolua_export