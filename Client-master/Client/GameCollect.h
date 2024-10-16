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
	// 采集
	void collect( int nMonId );

	//tolua_end
	//
	bool limitMachineCollectSimple(int resid);
	bool limitMachineCollectMore(int resid);
	bool limitMachineCollect(int resid);
	bool limitMachineCollect(GameActor* pMonster);
	unsigned int getMaxCollectDist( GameActor* pMonster );
	unsigned int getMaxCollectDist( int nResId );
	// 开始采集
	void startCollect( int nMonId );
	// 纯采集调用接口
	void purgeStartCollect(int nMonId);
	// 结束采集
	virtual void endCollect();

	virtual void update(unsigned int dtick);
private:
	int		m_nMonId;
	bool	m_bCath;
};//tolua_export
