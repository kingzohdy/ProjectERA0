#include "GamePetLand.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
GamePetLand::GamePetLand()
{
	INetworkManager::getSingleton().RegisterMsgHandler( GAY_SVR, this );
}

GamePetLand::~GamePetLand()
{

}

int GamePetLand::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if( msgid == GAY_SVR )
	{
		if ( msgbody.GaySvr.Type == GAY_SVR_INVITE )
		{
			msgbody.GaySvr.Data.Invite.RoleName;
			char szBuf[256];
			sprintf(szBuf,"OnReciveGayInvite( [===[%s]===] )",msgbody.GaySvr.Data.Invite.RoleName);
			GameManager::getSingleton().getScriptVM()->callString( szBuf );
		}
	}
	return 1;
}

void GamePetLand::replyGayInvite( const char* roleName, int Flag )
{
	tagCSPkgBody msgbody;
	if( Flag == 0 )
	{
		msgbody.GayClt.Type = GAY_CLT_REJECT;
		msgbody.GayClt.Data.Reject.Flag = 0;
		strcpy(msgbody.GayClt.Data.Reject.RoleName,roleName);
	}else{
		msgbody.GayClt.Type = GAY_CLT_ACCEPT;
		strcpy( msgbody.GayClt.Data.Accept.RoleName,roleName );
	}
	SendGameServerMsg( GAY_CLT, msgbody );
}

void GamePetLand::replyGayInviteTimeOut( const char* roleName )
{
	tagCSPkgBody msgbody;
	msgbody.GayClt.Type = GAY_CLT_REJECT;
	msgbody.GayClt.Data.Reject.Flag = 1;
	strcpy(msgbody.GayClt.Data.Reject.RoleName,roleName);
	SendGameServerMsg( GAY_CLT, msgbody );

}