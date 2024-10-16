#include <math.h>
#include <Windows.h>
#include <assert.h>
#include "GameWheelLottery.h"
#include "GameDBData.h"
#include "proto_cs.h"
#include "NetMessage.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"

GameWheelLottery::GameWheelLottery()
{
	INetworkManager::getSingleton().RegisterMsgHandler( RAFFLE_SVR, this );
	INetworkManager::getSingleton().RegisterMsgHandler( GOLD_RAFFLE_SVR, this );
	m_resultIdx = -1;
	m_vipIndex = -1;
	m_vipResultIdx = -1;
	m_vipTimes = 0;
}

GameWheelLottery::~GameWheelLottery()
{
}

GameWheelLottery & GameWheelLottery::getSingleton()
{
	static GameWheelLottery singleton;
	return singleton;
}
int GameWheelLottery::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if( msgid == RAFFLE_SVR )
	{
// 		if( m_resultIdx < 0  )
// 		{
// 			return 0;
// 		}
		if( msgbody.CsRaffleSvr.Type == RND_AWD_TYPE_BCT )
		{
			m_resultIdx = msgbody.CsRaffleSvr.Idx;
			char buf[256];
			sprintf(buf,"SetGameWheelLotteryResult(%d)", m_resultIdx);
			GameManager::getSingleton().getScriptVM()->callString( buf );
		}else{
			m_vipResultIdx = msgbody.CsRaffleSvr.Idx;
			m_vipTimes ++;
			char buf[256];
			sprintf(buf,"SetVipWheelLotteryResult(%d)",m_vipResultIdx);
			GameManager::getSingleton().getScriptVM()->callString( buf );

		}		
	}else if( msgid == GOLD_RAFFLE_SVR )
	{
		m_vipTimes = msgbody.GoldRaffleSvr.RaffleNum;
	}
	return 0;
}

void GameWheelLottery::add(int time /*= 1*/)
{
	for (int i = 0; i < time; ++i)
	{
		int result = 0;
		for (int n = 1; ; ++n)
		{
			const tagRndAwdDef *rndAwdDef = DBDataManager::getSingleton().getRndAwdDef(n);
			if (rndAwdDef == NULL)
			{
				break;
			}
			if( rndAwdDef->Type != RND_AWD_TYPE_BCT )
			{
				continue;
			}
			if( result == 0 )
			{
				result = n;
			}
			if (rand() % n == 0)
			{
				result = n;
			}
		}
		m_lstIndex.push_back(result);
	}
}

void GameWheelLottery::reset()
{
	m_lstIndex.clear();
}

void GameWheelLottery::pop()
{
	m_lstIndex.pop_front();
}

int GameWheelLottery::top()
{
	return m_lstIndex.front();
}

int GameWheelLottery::remain()
{
	return (int)m_lstIndex.size();
}

RandomItemVector GameWheelLottery::getRandomItems(int idx)
{
	RandomItemVector items;
	const tagRndAwdDef*	rndAwdDef = DBDataManager::getSingleton().getRndAwdDef(idx);
	if (rndAwdDef)
	{
		for (int i = 0; i < RAND_ITEM_NUM; ++i)
		{
			items.Type[i] = rndAwdDef->RandIAwards[i].Type;
			items.ItemID[i] = rndAwdDef->RandIAwards[i].ItemID;
			items.Num[i] = rndAwdDef->RandIAwards[i].Num;
		}
	}
	else
	{
		for (int i = 0; i < RAND_ITEM_NUM; ++i)
		{
			items.Type[i] = 2;
			items.ItemID[i] = 0;
			items.Num[i] = 0;
		}
	}

	return items;
}

void GameWheelLottery::start()
{
	if (remain() > 0)
	{
		tagCSPkgBody msgbody;
		msgbody.CsRaffleClt.Idx = top();
		msgbody.CsRaffleClt.Type = RND_AWD_TYPE_BCT;
		msgbody.CsRaffleClt.StartFlag = 0;
		SendGameServerMsg(RAFFLE_CLT, msgbody);
		pop();
	}
}

void GameWheelLottery::get()
{
	if (m_resultIdx < 0)
	{
		return;
	}
	tagCSPkgBody msgbody;
	msgbody.CsRaffleClt.Idx = m_resultIdx;
	msgbody.CsRaffleClt.Type = RND_AWD_TYPE_BCT;
	msgbody.CsRaffleClt.StartFlag = 1;
	SendGameServerMsg(RAFFLE_CLT, msgbody);
	m_resultIdx = -1;
}

void GameWheelLottery::setResult(int result)
{
	m_resultIdx = result;
}

void GameWheelLottery::vipStart( int type )
{
	tagCSPkgBody msgbody;
	msgbody.CsRaffleClt.Idx = m_vipIndex;
	msgbody.CsRaffleClt.Type = type;
	msgbody.CsRaffleClt.StartFlag = 0;
	SendGameServerMsg(RAFFLE_CLT, msgbody);
}

void GameWheelLottery::vipGet( int type )
{
	if (m_vipResultIdx < 0)
	{
		return;
	}
	tagCSPkgBody msgbody;
	msgbody.CsRaffleClt.Idx = m_resultIdx;
	msgbody.CsRaffleClt.Type = type;
	msgbody.CsRaffleClt.StartFlag = 1;
	SendGameServerMsg(RAFFLE_CLT, msgbody);
	m_vipResultIdx = -1;

}

int GameWheelLottery::getVipTimes()
{
	return GOLD_RAFFLE_MAX_NUM_TOLUA - m_vipTimes;
}

void GameWheelLottery::clearData()
{
	m_lstIndex.clear();
	m_resultIdx = -1;
	m_vipIndex = - 1;
	m_vipResultIdx = -1;
	m_vipTimes = 0;
}

int GameWheelLottery::getVipIdx( int type )
{
	int result = 0;
	for (int n = 1; ; ++n)
	{
		const tagRndAwdDef *rndAwdDef = DBDataManager::getSingleton().getRndAwdDef(n);
		if (rndAwdDef == NULL)
		{
			break;
		}
		if( rndAwdDef->Type != type )
		{
			continue;
		}
		if( result == 0 )
		{
			result = n;
		}
		if (rand() % n == 0)
		{
			result = n;
		}
	}
	m_vipIndex = result;
	return m_vipIndex;
}