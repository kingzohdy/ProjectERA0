#include "GameDynPWorld.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"

GameDynPWorld::GameDynPWorld(void)
{
	INetworkManager::getSingleton().RegisterMsgHandler( DYN_PWORLD_SVR, this );
}

GameDynPWorld::~GameDynPWorld(void)
{
}

int GameDynPWorld::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if( msgid == DYN_PWORLD_SVR )
	{
		m_nPWorldId = msgbody.DynPworldSvr.PworldID;
		char buf[256];
		sprintf( buf, "openPWorldSelect(%d)", msgbody.DynPworldSvr.PworldID );
		GameManager::getSingleton().getScriptVM()->callString( buf );
	}

	return 0;
}

void GameDynPWorld::exitDynPWorld( int nMapDefID )
{
	tagCSPkgBody msgbody;
	msgbody.PworldClt.CltType = PWORLD_CLT_EXIT_PWORLD;
	msgbody.PworldClt.CltData.ExitPworld.PworldID = nMapDefID / 100;
	SendGameServerMsg(PWORLD_CLT, msgbody);
}

// 动态副本选择
void GameDynPWorld::selectDynPWorld( int nGrade, int nLevel, int nDifficulty )
{
	tagCSPkgBody msgbody;
	msgbody.DynPworldClt.PworldID = m_nPWorldId;
	msgbody.DynPworldClt.DynPworldSelect.PworldGrade = nGrade;
	msgbody.DynPworldClt.DynPworldSelect.PworldDifficulty = nDifficulty;
	msgbody.DynPworldClt.DynPworldSelect.LevelType = nLevel;
	SendGameServerMsg( DYN_PWORLD_CLT, msgbody );
}
