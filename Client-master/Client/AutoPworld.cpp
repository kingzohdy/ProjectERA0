#include "AutoPworld.h"
#include "proto_cs.h"
#include "GameEngineTypes.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include "GameMsgPrint.h"
#include "GameDBData.h"

AutoPworldManager::AutoPworldManager(void)
{	
	INetworkManager::getSingleton().RegisterMsgHandler( AUTOPWORLD_SVR, this );
	resetPworldState();
}
AutoPworldManager::~AutoPworldManager(void)
{
}

void AutoPworldManager::resetPworldState()
{ 
	m_nPworldState = 0;
	m_nMemberNum = 0;
	m_uStartTime = 0;
	memset( &m_TeamInfo, 0, sizeof(AUTOPWORLDTEAMINFO));
}

void AutoPworldManager::requestPworldPortal( int nPworldID )
{
	tagCSPkgBody msgbody;
	msgbody.AutoPworldClt.Cmd = AUTOPWORLD_CLT_OP_ENTER;
	msgbody.AutoPworldClt.Data.Enter.PworldID = nPworldID;
	
	const tagPworldDef *pworldDef = DBDataManager::getSingleton().getPworldDef(nPworldID);
	if( pworldDef != NULL )
	{
		msgbody.AutoPworldClt.Data.Enter.DiffPworldID = pworldDef->DiffPworldID;
	}
	SendGameServerMsg( AUTOPWORLD_CLT, msgbody );
}

void AutoPworldManager::requestPortalFrame( int nType )
{
	tagCSPkgBody msgbody;
	msgbody.AutoPworldClt.Cmd = AUTOPWORLD_CLT_OP_UI;
	msgbody.AutoPworldClt.Data.UI.Invalid = nType;
	SendGameServerMsg( AUTOPWORLD_CLT, msgbody );
}

void AutoPworldManager::requestAgree( bool bAgree )
{
	tagCSPkgBody msgbody;
	memset(&msgbody,0,sizeof(msgbody));
	msgbody.AutoPworldClt.Cmd = AUTOPWORLD_CLT_OP_AGREE;
	msgbody.AutoPworldClt.Data.Agree.Agree = bAgree;
	SendGameServerMsg( AUTOPWORLD_CLT, msgbody );
}

int AutoPworldManager::HandleNetMsg( int msgid, const tagCSPkgBody& msgbody )
{
	if( msgid != AUTOPWORLD_SVR )
	{
		return 0;
	}

	const AUTOPWORLDSVR& autoPworldSvr = msgbody.AutoPworldSvr;
	switch( autoPworldSvr.Cmd )
	{
	case AUTOPWORLD_SVR_OP_ENTER:
		{
			const AUTOPWORLDSVRENTER& enter = autoPworldSvr.Data.EnterTransmit;
			m_nPworldState = enter.PworldID;
			m_nMemberNum = enter.TeamMemNum;
			m_uStartTime = enter.StartTM;
			memcpy( m_TeamInfo, enter.TeamInfo, sizeof(m_TeamInfo) );
			char buffer[256];
			sprintf( buffer, "ConfirmPworldPortal(%d,%d,%d)",enter.CancelFlag, enter.PworldID, enter.TeamMemNum );
			GameManager::getSingleton().getScriptVM()->callString( buffer );
		}
		break;
	case AUTOPWORLD_SVR_OP_UI:
		{
			m_nPworldState = autoPworldSvr.Data.UI.PworldID;
			char buffer[256];
			sprintf( buffer, "OpenPworldPortalFrame(%d, %d)", autoPworldSvr.Data.UI.PworldID, autoPworldSvr.Data.UI.EffectFlag );
			GameManager::getSingleton().getScriptVM()->callString( buffer );
		}
		break;
	default:
		break;
	}
	return 0;
}

int AutoPworldManager::getMemberNum()
{
	return m_nMemberNum;
}

unsigned int AutoPworldManager::getStartTime()
{
	return m_uStartTime;
}

tagAutoPworldTeamInfo_ToLua* AutoPworldManager::getMemberData( int idx )
{
	static_assert(sizeof(tagAutoPworldTeamInfo_ToLua) == sizeof(tagAutoPworldTeamInfo));
	if( idx < MAX_TEAM_MEMBER )
	{
		return (tagAutoPworldTeamInfo_ToLua*)&m_TeamInfo[idx];
	}
	return NULL;
}