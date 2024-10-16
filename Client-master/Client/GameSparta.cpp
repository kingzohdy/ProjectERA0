#include "GameSparta.h"
#include "GameManager.h"
#include "GameMaxMap.h"
#include "OgreScriptLuaVM.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "GameDBData.h"
#include "GameLoginManager.h"
#include "GameTeamManager.h"

GameSparta::GameSparta()
{
	INetworkManager::getSingleton().RegisterMsgHandler( SPARTA_SVR, this );
}

GameSparta::~GameSparta()
{

}

int GameSparta::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if( msgid == SPARTA_SVR )
	{
		if ( msgbody.SpartaSvr.Type == SPARTA_SVR_READY_UI )
		{
			for ( int i = 0; i < MAX_SPARTA_ROLE; i++ )
			{
				m_SpartaReadyRoleInfo[i].Ready = msgbody.SpartaSvr.Data.ReadyUI.RoleInfo[i].Ready;
				sprintf( m_SpartaReadyRoleInfo[i].RoleName, msgbody.SpartaSvr.Data.ReadyUI.RoleInfo[i].RoleName );
			}
			GameManager::getSingleton().getScriptVM()->callString( "UpdateSpartaGetReady()" );
		}
		else if ( msgbody.SpartaSvr.Type == SPARTA_SVR_APPLY_INFO )
		{
			char buf[256];
			sprintf( buf, "SpartaApplyResult(%d)", msgbody.SpartaSvr.Data.Apply.Succ );
			GameManager::getSingleton().getScriptVM()->callString( buf );
		}
		else if ( msgbody.SpartaSvr.Type == SPARTA_SVR_EXIT_QUEUE )
		{
			char buf[256];
			sprintf( buf, "SpartaApplyEixtApplyResult(%d)", msgbody.SpartaSvr.Data.ExitQueue );
			GameManager::getSingleton().getScriptVM()->callString( buf );
		}
		else if ( msgbody.SpartaSvr.Type == SPARTA_SVR_FIGHT_INFO )
		{
			m_SpartaFightInfo = msgbody.SpartaSvr.Data.FightInfo;
			MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
			for ( int i = 0; i < MAX_SPARTA_ROLE; i++ )
			{
				if ( strcmp( msgbody.SpartaSvr.Data.FightInfo.EndInfo.RoleInfo[i].RoleName, pMain->getName() ) == 0 )
				{
					if (pMain)
					{
						pMain->m_nCampIdx = msgbody.SpartaSvr.Data.FightInfo.EndInfo.RoleInfo[i].Camp;
					}
				}
			}
			if ( msgbody.SpartaSvr.Data.FightInfo.End == 1 )
			{
				char buf[256];
				int selfCampIdx = GetSelfCampIdx();
				//胜利
				if ( msgbody.SpartaSvr.Data.FightInfo.VCamp == selfCampIdx )

				{
					//完胜
					if ( GetEnemyTeamKill() == 0 )
					{
						sprintf( buf, "playUiEffect(0,0,1,[[effect\\entity\\JJCwansheng.ent]])" );
					}
					else//胜利
					{
						sprintf( buf, "playUiEffect(0,0,1,[[effect\\entity\\JJCshengli.ent]])" );
					}
				}
				//平局
				else if ( msgbody.SpartaSvr.Data.FightInfo.VCamp ==  0 )
				{
					if ( GetSelfTeamKill() == GetEnemyTeamKill() )
					{
						sprintf( buf, "playUiEffect(0,0,1,[[effect\\entity\\JJCping.ent]])" );
					}
				}
				//失败
				else
				{
					sprintf( buf, "playUiEffect(0,0,1,[[effect\\entity\\JJCshibai.ent]])" );
				}
				GameManager::getSingleton().getScriptVM()->callString( buf );
				GameManager::getSingleton().getScriptVM()->callString( "SpartaEnd()" );
				if ( pMain )
				{
					pMain->m_nCampIdx = 0;
				}
			}
		}
		else if ( msgbody.SpartaSvr.Type == SPARTA_SVR_ROLE_SPARTA_INFO )
		{
			MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
			pMainplayer->setSpartaDefeat( msgbody.SpartaSvr.Data.SprataInfo.Defeat );
			pMainplayer->setSpartaFightVal( msgbody.SpartaSvr.Data.SprataInfo.FightVal );
			pMainplayer->setSpartaRunAway( msgbody.SpartaSvr.Data.SprataInfo.RunAway );
			pMainplayer->setSpartaVictory( msgbody.SpartaSvr.Data.SprataInfo.Victory );
		}
	}
	return 0;
}

void GameSparta::objEnterView(GameActor *actor)
{
	if ( !IsInSpartaPworld() )
	{
		return;
	}
	if( actor->getServerId() != -1 )
	{
		actor->setActorWorldName( LoginManager::getSingleton().getWorldNameById(actor->getServerId()) );
	}
}

void GameSparta::SpartaGetReady()
{
	tagCSPkgBody msgbody;
	msgbody.SpartaClt.Type = SPARTA_CLT_READY;
	msgbody.SpartaClt.Data.Ready.Ready = 1;
	SendGameServerMsg( SPARTA_CLT, msgbody );
}

void GameSparta::SpartaApply()
{
	tagCSPkgBody msgbody;
	msgbody.SpartaClt.Type = SPARTA_CLT_APPLY;
	msgbody.SpartaClt.Data.Apply = 1;
	SendGameServerMsg( SPARTA_CLT, msgbody );
}

void GameSparta::SpartaExitApply()
{
	tagCSPkgBody msgbody;
	msgbody.SpartaClt.Type = SPARTA_CLT_EXIT_APPLY;
	msgbody.SpartaClt.Data.Exit = 1;
	SendGameServerMsg( SPARTA_CLT, msgbody );
}

int GameSparta::IsSpartaRoleReady( int idx )
{
	if ( idx < MAX_SPARTA_ROLE && idx >= 0 )
	{
		return m_SpartaReadyRoleInfo[idx].Ready;
	}
	return 0;
}

const char* GameSparta::GetSpartaRoleName( int idx )
{
	if ( idx < MAX_SPARTA_ROLE && idx >= 0 )
	{
		return m_SpartaReadyRoleInfo[idx].RoleName;
	}
	return "";
}

int GameSparta::GetSelfCampIdx()
{
	for ( int i = 0; i < MAX_SPARTA_ROLE; i++ )
	{
		if ( strcmp( ActorManager::getSingleton().getMainPlayer()->getName(), m_SpartaFightInfo.EndInfo.RoleInfo[i].RoleName ) == 0 )
		{
			return m_SpartaFightInfo.EndInfo.RoleInfo[i].Camp;
		}
	}
	return 0;
}

tagCSSpartaFightRoleInfo_ToLua GameSparta::GetSpartaEndInfo( int idx )
{
	assert( sizeof( tagCSSpartaFightRoleInfo_ToLua ) == sizeof( tagCSSpartaFightRoleInfo ) );
	tagCSSpartaFightRoleInfo_ToLua spartaFightRoleInfo;
	spartaFightRoleInfo.Camp = m_SpartaFightInfo.EndInfo.RoleInfo[idx].Camp;
	spartaFightRoleInfo.SpartaGlory = m_SpartaFightInfo.EndInfo.RoleInfo[idx].SpartaGlory;
	spartaFightRoleInfo.Leave = m_SpartaFightInfo.EndInfo.RoleInfo[idx].Leave;
	spartaFightRoleInfo.FightVal = m_SpartaFightInfo.EndInfo.RoleInfo[idx].FightVal;
	spartaFightRoleInfo.ChgFightVal = m_SpartaFightInfo.EndInfo.RoleInfo[idx].ChgFightVal;
	sprintf( spartaFightRoleInfo.RoleName, m_SpartaFightInfo.EndInfo.RoleInfo[idx].RoleName );
	return spartaFightRoleInfo;
}

tagCSSpartaRoleSpartaInfo_ToLua GameSparta::GetSpartaRoleSaprtaInfo()
{
	assert( sizeof( tagCSSpartaRoleSpartaInfo_ToLua ) == sizeof( tagCSSpartaRoleSpartaInfo ) );
	tagCSSpartaRoleSpartaInfo_ToLua spartaRoleSpartaInfo;
	spartaRoleSpartaInfo.Defeat = m_SpartaRoleSpartaInfo.Defeat;
	spartaRoleSpartaInfo.FightVal = m_SpartaRoleSpartaInfo.FightVal;
	spartaRoleSpartaInfo.RunAway = m_SpartaRoleSpartaInfo.RunAway;
	spartaRoleSpartaInfo.Victory = m_SpartaRoleSpartaInfo.Victory;
	return spartaRoleSpartaInfo;
}

int GameSparta::GetSpartaVCamp()
{
	return m_SpartaFightInfo.VCamp;
}

int GameSparta::GetSelfTeamKill()
{
	const tagTeamInfo_ToLua* teamInfo = GameTeamManager::getSingleton().getTeamInfo();
	if ( m_SpartaFightInfo.Team1ID == teamInfo->TeamID )
	{
		return m_SpartaFightInfo.Team1Kill;
	}
	else
	{
		return m_SpartaFightInfo.Team2Kill;
	}
}
int GameSparta::GetEnemyTeamKill()
{
	const tagTeamInfo_ToLua* teamInfo = GameTeamManager::getSingleton().getTeamInfo();
	if ( m_SpartaFightInfo.Team1ID == teamInfo->TeamID )
	{
		return m_SpartaFightInfo.Team2Kill;
	}
	else
	{
		return m_SpartaFightInfo.Team1Kill;
	}
}
bool GameSparta::IsInSpartaPworld()
{
	int mapid = GameManager::getSingleton().getGameMapShower()->getRealMapId();
	if( mapid >= GAME_PWORLD_MAPID )
	{
		int nPworldId = mapid/100;
		const tagPworldDef* pPworldDef = DBDataManager::getSingleton().getPworldDef( nPworldId );
		if( pPworldDef != NULL && pPworldDef->PolyType == PWORLD_POLY_SPARTA )
		{
			return true;
		}
	}
	return false;
}
