#pragma once
#include "OgreSingleton.h"
#include "GameMsgPrint.h"

class GameCollect//tolua_export
	: public Ogre::Singleton<GameCollect>, IGameMsgPrint
{//tolua_export
public:
	GameCollect(void);
	~GameCollect(void);

public:
	//tolua_begin
	int getCollectingMonsterID()
	{
		return m_nMonId;
	}
	
	bool isInCollect();
	bool canCollect( int id );
	bool canCollect( GameActor* pMonster );
	// �ɼ�
	void collect( int nMonId );

	//tolua_end
	//
	bool limitMachineCollectSimple(int resid);
	bool limitMachineCollectMore(int resid);
	bool limitMachineCollect(int resid);
	bool limitMachineCollect(GameActor* pMonster);
	unsigned int getMaxCollectDist( GameActor* pMonster );
	unsigned int getMaxCollectDist( int nResId );
	// ��ʼ�ɼ�
	void startCollect( int nMonId );
	// ���ɼ����ýӿ�
	void purgeStartCollect(int nMonId);
	// �����ɼ�
	virtual void endCollect();

	virtual void update(unsigned int dtick);
private:
	int		m_nMonId;
	bool	m_bCath;
};//tolua_export
