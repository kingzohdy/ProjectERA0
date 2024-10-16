#include "GamePoly.h"
#include "GameManager.h"
#include "GameMaxMap.h"
#include "OgreScriptLuaVM.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"

GamePoly::GamePoly()
{
	resetData();
	resetMachineWar();
	resetTank();
	INetworkManager::getSingleton().RegisterMsgHandler( POLY_PWORLD_SVR, this );
}

GamePoly::~GamePoly()
{

}

int GamePoly::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if(msgid == POLY_PWORLD_SVR )
	{
		if( msgbody.PolyPworldSvr.SvrType == POLY_SVR_TYPE_INFO )
		{
			switch( msgbody.PolyPworldSvr.SvrData.PolyPworldInfo.OpType )
			{
			case POLY_OP_DOUBLE_MONSTER:
				{
					m_sDoubleMonsterInfo = msgbody.PolyPworldSvr.SvrData.PolyPworldInfo.Data.DoubleMonster;
					tagCSDoubleMonsterEndInfo playsData = msgbody.PolyPworldSvr.SvrData.PolyPworldInfo.Data.DoubleMonster.MonsterEndInfo;
					for ( int i = 0; i < playsData.RoleNum; i++ )
					{
						GameActor* pActor = ActorManager::getSingleton().FindActor( playsData.RoleList[i].MemID );
						if ( pActor )
						{
							int nMonsterID = pActor->getMonsterMachine()->m_playerInfo.m_nMonsterMemID;
							GameActor* pMonster = ActorManager::getSingleton().FindActor( nMonsterID );
							if (pMonster)
							{
								pMonster->m_nCampIdx = playsData.RoleList[i].CampIdx;
							}
						}
					}
					GameManager::getSingleton().SendUIEvent( "GE_POLY_MONSTER_UPDATE" );
					if( msgbody.PolyPworldSvr.SvrData.PolyPworldInfo.End )
					{
						char buf[256];
						int selfCampIdx = getSelfCampIdx();
						if( selfCampIdx == msgbody.PolyPworldSvr.SvrData.PolyPworldInfo.Data.DoubleMonster.VCamp  && selfCampIdx != 0 )
						{
							sprintf( buf, "playUiEffect(0,0,1,[[effect\\entity\\shengli_16.ent]])" );
						}else{
							sprintf( buf, "playUiEffect(0,0,1,[[effect\\entity\\shibai_16.ent]])" );
						}
						GameManager::getSingleton().getScriptVM()->callString( buf );
						GameManager::getSingleton().getScriptVM()->callString( "LightAndShadowEnd()" );
					}
				}
				break;
			case POLY_OP_MACHINE_WAR:
				{
					m_MachineWarInfo = msgbody.PolyPworldSvr.SvrData.PolyPworldInfo.Data.MachineWar;
					MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
					if (pMain)
					{
						if ( pMain->m_nCampIdx != msgbody.PolyPworldSvr.SvrData.PolyPworldInfo.Data.MachineWar.SelfCamp )
						{
							GameManager::getSingleton().SendUIEvent("GE_POLY_MACHINE_WAR_SLEF_CAMP_CHANGE");
						}
						pMain->m_nCampIdx = msgbody.PolyPworldSvr.SvrData.PolyPworldInfo.Data.MachineWar.SelfCamp;
					}
					GameManager::getSingleton().SendUIEvent( "GE_POLY_MACHINE_WAR_UPDATE" );
					if ( msgbody.PolyPworldSvr.SvrData.PolyPworldInfo.End )
					{
						char buf[256];
						if ( msgbody.PolyPworldSvr.SvrData.PolyPworldInfo.Data.MachineWar.SelfCamp == msgbody.PolyPworldSvr.SvrData.PolyPworldInfo.Data.MachineWar.VCamp )
						{
							sprintf( buf, "MachineWarResult(%d,%d,%d)", 1, msgbody.PolyPworldSvr.SvrData.PolyPworldInfo.Data.MachineWar.EndExp, msgbody.PolyPworldSvr.SvrData.PolyPworldInfo.Data.MachineWar.ToTalExp );
						}
						else
						{
							sprintf( buf, "MachineWarResult(%d,%d,%d)", 0, msgbody.PolyPworldSvr.SvrData.PolyPworldInfo.Data.MachineWar.EndExp, msgbody.PolyPworldSvr.SvrData.PolyPworldInfo.Data.MachineWar.ToTalExp );
						}
						GameManager::getSingleton().getScriptVM()->callString( buf );
						if ( pMain )
						{
							pMain->m_nCampIdx = 0;
						}
					}
				}
				break;
			case POLY_OP_TANK:
				{
					m_TankInfo = msgbody.PolyPworldSvr.SvrData.PolyPworldInfo.Data.TankInfo;
					for ( int i = 0; i < m_TankInfo.RoleNum; i++ )
					{
						GameActor* pActor = ActorManager::getSingleton().FindActor( m_TankInfo.RoleList[i].MemID );
						if ( pActor )
						{
							int nMonsterID = pActor->getMonsterMachine()->m_playerInfo.m_nMonsterMemID;
							GameActor* pMonster = ActorManager::getSingleton().FindActor( nMonsterID );
							if (pMonster)
							{
								pMonster->m_nCampIdx = m_TankInfo.RoleList[i].CampIdx;
							}
						}
					}
					GameManager::getSingleton().SendUIEvent( "GE_POLY_TANK_UPDATE" );
					if( msgbody.PolyPworldSvr.SvrData.PolyPworldInfo.End )
					{
						char buf[256];
						int selfCampIdx = getTankSelfCampIdx();
						//平局也显示为胜利
						if( msgbody.PolyPworldSvr.SvrData.PolyPworldInfo.Data.TankInfo.VCamp == 0 )
						{
							sprintf( buf, "playUiEffect(0,0,1,[[effect\\entity\\shengli_16.ent]])" );
						}
						else if ( selfCampIdx == msgbody.PolyPworldSvr.SvrData.PolyPworldInfo.Data.TankInfo.VCamp  && selfCampIdx != 0 )
						{
							sprintf( buf, "playUiEffect(0,0,1,[[effect\\entity\\shengli_16.ent]])" );
						}else{
							sprintf( buf, "playUiEffect(0,0,1,[[effect\\entity\\shibai_16.ent]])" );
						}
						GameManager::getSingleton().getScriptVM()->callString( buf );
						GameManager::getSingleton().getScriptVM()->callString( "TankWarEnd()" );
					}
				}
				break;
			default:
				break;
			}			
		}
		else if ( msgbody.PolyPworldSvr.SvrType == POLY_SVR_TYPE_BOSS_POSITION )
		{
			m_MachineWarBossPos = msgbody.PolyPworldSvr.SvrData.BossPosition;
			GameManager::getSingleton().getScriptVM()->callString( "updateBossMachineWarPos()" );
		}
	}
	return 0;
}

void GamePoly::resetData()
{
	memset( &m_sDoubleMonsterInfo, 0, sizeof(CSDOUBLEMONSTERINFO) );
}
void GamePoly::resetMachineWar()
{
	memset( &m_MachineWarInfo, 0, sizeof(CSMACHINEWARINFO) );
}

const tagCSDoubleMonsterInfo_ToLua* GamePoly::getDoubleMonsterInfo()
{
	static_assert( sizeof(tagCSDoubleMonsterInfo_ToLua) == sizeof(tagCSDoubleMonsterInfo) );
	return (const tagCSDoubleMonsterInfo_ToLua*)(&m_sDoubleMonsterInfo);
}

const tagCSMachineWarInfo_ToLua* GamePoly::getMachineWarInfo()
{
	static_assert( sizeof(tagCSMachineWarInfo_ToLua) == sizeof(tagCSMachineWarInfo) );
	return (const tagCSMachineWarInfo_ToLua*)(&m_MachineWarInfo);
}

bool GamePoly::isInPolyPworld()
{
	int mapid = GameManager::getSingleton().getGameMapShower()->getRealMapId();
	if( mapid >= GAME_PWORLD_MAPID )
	{
		int nPworldId = mapid/100;
		const tagPworldDef* pPworldDef = DBDataManager::getSingleton().getPworldDef( nPworldId );
		if( pPworldDef != NULL && pPworldDef->PolyType == PWORLD_POLY_DOUBLE_MONSTER )
		{
			return true;
		}
	}
	return false;
}

bool GamePoly::isInPloyPworld_MachineWar()
{
	int mapid = GameManager::getSingleton().getGameMapShower()->getRealMapId();
	if( mapid >= GAME_PWORLD_MAPID )
	{
		int nPworldId = mapid/100;
		const tagPworldDef* pPworldDef = DBDataManager::getSingleton().getPworldDef( nPworldId );
		if( pPworldDef != NULL && pPworldDef->PolyType == PWORLD_POLY_MACHINE_WAR )
		{
			return true;
		}
	}
	return false;
}
int GamePoly::getMachineWarReadyTime()
{
	int mapid = GameManager::getSingleton().getGameMapShower()->getRealMapId();
	if( mapid >= GAME_PWORLD_MAPID )
	{
		int nPworldId = mapid/100;
		const tagPworldDef* pPworldDef = DBDataManager::getSingleton().getPworldDef( nPworldId );
		if( pPworldDef != NULL && pPworldDef->PolyType == PWORLD_POLY_MACHINE_WAR )
		{
			return pPworldDef->ReadyTime;
		}
	}
	return 0;
}
int GamePoly::getMachineWarTimeOut()
{
	int mapid = GameManager::getSingleton().getGameMapShower()->getRealMapId();
	if( mapid >= GAME_PWORLD_MAPID )
	{
		int nPworldId = mapid/100;
		const tagPworldDef* pPworldDef = DBDataManager::getSingleton().getPworldDef( nPworldId );
		if( pPworldDef != NULL && pPworldDef->PolyType == PWORLD_POLY_MACHINE_WAR )
		{
			return pPworldDef->Timeout;
		}
	}
	return 0;
}

int GamePoly::getSelfCampIdx()
{
	if ( m_sDoubleMonsterInfo.MonsterEndInfo.RoleNum != 0 )
	{
		for ( int i = 0; i < m_sDoubleMonsterInfo.MonsterEndInfo.RoleNum; i++ )
		{
			if ( strcmp( ActorManager::getSingleton().getMainPlayer()->getName(), m_sDoubleMonsterInfo.MonsterEndInfo.RoleList[i].RoleName) == 0 )
			{
				return m_sDoubleMonsterInfo.MonsterEndInfo.RoleList[i].CampIdx;
			}
		}
	}
	return 0;
}

int GamePoly::getTankSelfCampIdx()
{
	if ( m_TankInfo.RoleNum != 0 )
	{
		for ( int i = 0; i < m_TankInfo.RoleNum; i++ )
		{
			if ( strcmp( ActorManager::getSingleton().getMainPlayer()->getName(), m_TankInfo.RoleList[i].RoleName) == 0 )
			{
				return m_TankInfo.RoleList[i].CampIdx;
			}
		}
	}
	return 0;
}

int GamePoly::getMachineWarTitanPosX()
{
	return m_MachineWarBossPos.Titan.X;
}
int GamePoly::getMachineWarTitanPosY()
{
	return m_MachineWarBossPos.Titan.Y;
}
int GamePoly::getMachineWarMachinePosX()
{
	return m_MachineWarBossPos.Machine.X;
}
int GamePoly::getMachineWarMachinePosY()
{
	return m_MachineWarBossPos.Machine.Y;
}

void GamePoly::resetTank()
{
	memset( &m_TankInfo, 0, sizeof(CSTANKINFO) );
}

int GamePoly::getTankPlayerNum()
{
	return m_TankInfo.RoleNum;
}

const tagCSTankRoleInfo_ToLua* GamePoly::getTankPlayerByIdx( int idx )
{
	static_assert( sizeof(tagCSTankRoleInfo_ToLua) == sizeof(tagCSTankRoleInfo) );
	return (const tagCSTankRoleInfo_ToLua*)(&m_TankInfo.RoleList[idx]);
}

int GamePoly::getSelfCampInTankWar()
{
	if ( m_TankInfo.RoleNum != 0 )
	{
		for ( int i = 0; i < m_TankInfo.RoleNum; i++ )
		{
			if ( strcmp( ActorManager::getSingleton().getMainPlayer()->getName(), m_TankInfo.RoleList[i].RoleName) == 0 )
			{
				return m_TankInfo.RoleList[i].CampIdx;
			}
		}
	}
	return 0;
}