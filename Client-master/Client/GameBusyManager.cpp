#include "GameBusyManager.h"
#include "ShowActor.h"

GameBusyManager::GameBusyManager() : m_nBeachTime(0),m_nUsedNum(0)
{
	INetworkManager::getSingleton().RegisterMsgHandler( BUSY_SVR, this );
}

GameBusyManager::~GameBusyManager()
{

}

int GameBusyManager::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if( msgid == BUSY_SVR )
	{
		const CSBUSYSVRDATA&  busyData = msgbody.BusySvr.Data;
		switch( msgbody.BusySvr.Cmd )
		{
		case BUSY_SVR_OP_DAY_NUM:
			{
				m_nUsedNum = busyData.Info.FiniNum;
			}
			break;
		case BUSY_SVR_OP_UI:
			{
				char szScripts[256];
				sprintf( szScripts, "OpenIsBusyFrame( %d,%d )" ,busyData.BusyUI.ID ,busyData.BusyUI.NpcID );
				GameManager::getSingleton().getScriptVM()->callString( szScripts );
			}
			break;
		case BUSY_SVR_OP_FINI_ACTION:
			{
				MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
				if( pMainplayer != NULL )
				{
					pMainplayer->getShowActor()->getCurEntity()->playMotion( "9506_16", false, 10000 );
				}
			}
			break;
		}
		
	}
	return 0;
}

void GameBusyManager::IsBusyTaskCommit( int nId ,int nItem ,int nFiniType )
{
	tagCSPkgBody msgbody;
	msgbody.BusyClt.Cmd					= BUSY_CLT_OP_FINI;
	msgbody.BusyClt.Data.Fini.ID		= nId;
	msgbody.BusyClt.Data.Fini.Type		= nFiniType;
	msgbody.BusyClt.Data.Fini.UseItem	= nItem;
	SendGameServerMsg( BUSY_CLT, msgbody );
}
const tagBusyDef_ToLua*  GameBusyManager::getBusyDef( int nId )
{
	static_assert( sizeof(tagBusyDef_ToLua) == sizeof(tagBusyDef) );
	return (const tagBusyDef_ToLua*)DBDataManager::getSingleton().getBusyDef(nId);
}
const tagBusyUseDef_ToLua*  GameBusyManager::getBusyUsedDef( int nNum )
{
	static_assert( sizeof(tagBusyUseDef_ToLua) == sizeof(tagBusyUseDef) );
	return (const tagBusyUseDef_ToLua*)DBDataManager::getSingleton().getBusyUseDef(nNum);
}
int GameBusyManager::GetBeachTime()
{
	return m_nBeachTime;
}

void GameBusyManager::SetBeachTime( int nTime )
{
	m_nBeachTime = nTime;
}

int GameBusyManager::GetUsedNum()
{
	return m_nUsedNum;
}
