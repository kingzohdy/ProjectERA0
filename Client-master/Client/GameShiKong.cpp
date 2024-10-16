#include "GameShiKong.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"

GameShiKong::GameShiKong(void)
{
	INetworkManager::getSingleton().RegisterMsgHandler( SHIKONG_SVR, this );
}

GameShiKong::~GameShiKong(void)
{
}
int GameShiKong::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if ( msgid == SHIKONG_SVR )
	{
		const CSSHIKONGSVR& Msg = msgbody.ShiKongSvr;
		switch( Msg.Cmd )
		{	
		case SHIKONG_SVR_OP_ENTER_APPLY_QUEUE:
			{
				char buf[256];
				sprintf( buf, "ShiKongEnterApply(%d)",Msg.Data.EnterApplyQueue.PworldID );
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;
		case SHIKONG_SVR_OP_INVITE_ENTER:
			{
				char buf[256];
				sprintf( buf, "ShiKongInviteEnter(%d)",Msg.Data.InviteEnter.PworldID );
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;
		case SHIKONG_SVR_OP_EXIT_SHIKONG_QUEUE:
			{
				char buf[256];
				sprintf( buf, "ExitShiKongQueue(%d,%d)",Msg.Data.ExitShiKongQueue.PworldID,Msg.Data.ExitShiKongQueue.type );
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;
		default:
			break;
		}
	}
	return 0;
}
void GameShiKong::ExitApplyQueue( int nPworldID )
{
	tagCSPkgBody msgbody;
	msgbody.ShiKongClt.CltType = SHIKONG_CLT_EXIT_APPLY_QUEUE;
	msgbody.ShiKongClt.CltData.ExitApplyQueue.PworldID = nPworldID;
	SendGameServerMsg( SHIKONG_CLT, msgbody );
}
void GameShiKong::EnterPworld( int nPworldID )
{
	tagCSPkgBody msgbody;
	msgbody.ShiKongClt.CltType = SHIKONG_CLT_ENTER_PWORLD;
	msgbody.ShiKongClt.CltData.EnterShikongPworld.PworldID = nPworldID;
	SendGameServerMsg( SHIKONG_CLT, msgbody );
}