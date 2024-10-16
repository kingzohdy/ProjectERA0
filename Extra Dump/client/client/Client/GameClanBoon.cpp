#include "GameManager.h"
#include "GameClanBoon.h"

GameClanBoon::GameClanBoon()
{
	memset( &m_ClanBoonInfo, 0, sizeof(tagClanBoon_ToLua) );
	INetworkManager::getSingleton().RegisterMsgHandler( BOON_SVR, this );
}

int GameClanBoon::HandleNetMsg( int msgid, const tagCSPkgBody& msgbody )
{
	if ( msgid == BOON_SVR )
	{
		switch( msgbody.BoonSvr.Cmd )
		{
		case BOON_SVR_OP_ALL:
			{
				static_assert( sizeof(tagClanBoon_ToLua) == sizeof(tagClanBoon) );
				memcpy( &m_ClanBoonInfo, &msgbody.BoonSvr.Data.All.ClanBoon, sizeof(tagClanBoon) );
				GameManager::getSingleton().SendUIEvent("GE_CLAN_BOON_ALL");
			}
			break;
		case BOON_SVR_OP_CHG:
			{
				static_assert( sizeof(tagBoonCond_ToLua) == sizeof(tagBoonCond) );
				memcpy( &m_ClanBoonInfo.Cond, &msgbody.BoonSvr.Data.Chg.Cond, sizeof(tagBoonCond) );
				GameManager::getSingleton().SendUIEvent("GE_CLAN_BOON_UPDATE");
			}
			break;
		case BOON_SVR_OP_ASSIGN:
			{
				static_assert( sizeof(tagBoonAssign_ToLua) == sizeof(tagBoonAssign) );
				memcpy( &m_ClanBoonInfo.Assign, &msgbody.BoonSvr.Data.Assign.Assign, sizeof(tagBoonAssign) );
				GameManager::getSingleton().SendUIEvent("GE_CLAN_BOON_UPDATE");
			}
			break;
		default:
			break;
		}
	}
	return 0;
}

void GameClanBoon::requestClanBoonAssign()
{
	tagCSPkgBody msgbody;
	msgbody.BoonClt.Cmd = BOON_CLT_OP_ASSIGN;
	msgbody.BoonClt.Data.Assign.Office = m_AssignInfo.Office;
	for ( int i = 0; i < BOON_GET_MAX; ++i )
	{
		strcpy( msgbody.BoonClt.Data.Assign.AssignBindGlod[i].Name, m_AssignInfo.AssignBindGlod[i].Name );
		msgbody.BoonClt.Data.Assign.AssignBindGlod[i].Pos = m_AssignInfo.AssignBindGlod[i].Pos;
		msgbody.BoonClt.Data.Assign.AssignBindGlod[i].Money = m_AssignInfo.AssignBindGlod[i].Money;
	}
	SendGameServerMsg( BOON_CLT, msgbody );
}

void GameClanBoon::requestClanBoonGet( int money, int flag )
{
	tagCSPkgBody msgbody;
	msgbody.BoonClt.Cmd = BOON_CLT_OP_GET;
	msgbody.BoonClt.Data.Get.Money = money;
	msgbody.BoonClt.Data.Get.Flag = flag;
	SendGameServerMsg( BOON_CLT, msgbody );
}

void GameClanBoon::resetAssignData()
{
	memset( &m_AssignInfo, 0, sizeof(tagBoonAssign) );
}

void GameClanBoon::setBoonAssignDataByIndex( int idx, char * szName, int pos, int money )
{
	assert( idx < BOON_GET_MAX );
	strcpy( m_AssignInfo.AssignBindGlod[idx].Name, szName );
	m_AssignInfo.AssignBindGlod[idx].Pos = pos;
	m_AssignInfo.AssignBindGlod[idx].Money = money;
}

int GameClanBoon::getBoonDataIndexByPos( int pos )
{
	for ( int i = 0; i < BOON_GET_MAX; ++i )
	{
		if ( m_ClanBoonInfo.Assign.AssignBindGlod[i].Pos == pos )
		{
			return i;
		}
	}
	return 0;
}