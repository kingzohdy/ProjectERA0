#include "GameCollect.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "GameQuest.h"
#include "SecondarySkillMgr.h"
#include "GameActor_MonsterMachine.h"
#include "WorldCity.h"
#include <Windows.h>

const int COLLECT_DIS = OP_DIST/4;

GameCollect::GameCollect(void):m_bCath(false)
{
	GameMsgPrint::getSingleton().registGameMsgHandler( this );
}

GameCollect::~GameCollect(void)
{
}

unsigned int GameCollect::getMaxCollectDist( GameActor* pMonster )
{
	if ( pMonster == NULL )
	{
		return 0;
	}

	return getMaxCollectDist( pMonster->getResId() );
}

unsigned int GameCollect::getMaxCollectDist( int nResId )
{
	const tagMonsterDef& monsterDef = DBDataManager::getSingleton().getMonsterDef( nResId );
	if ( &monsterDef == NULL )
	{
		return 0;
	}

	return monsterDef.Radius;
}

// 采集
void GameCollect::collect( int nMonId )
{
	GameActor* pNpc			= ActorManager::getSingleton().FindActor( nMonId );
	GameActor* pMainPlayer	= ActorManager::getSingleton().getMainPlayer();

	if( pNpc == NULL )			
	{
		return;
	}

	if( pMainPlayer == NULL )	
	{
		return;
	}

	if (pMainPlayer->isDead())
	{
		return;
	}

	if (!canCollect(pNpc))
	{
		return;
	}

	if (!limitMachineCollect(pNpc))
	{
		if (pMainPlayer->isOnMonsterMachine())
		{
			MonsterMachine* pMonsterMachine = pMainPlayer->getMonsterMachine();
			tagMonMachineDef_ToLua* monMachineDef = pMonsterMachine->getMonMachineDef();
			if (monMachineDef && monMachineDef->CollectType != 0)
			{
				GameMsgPrint::getSingleton().showMidTip(EMID_MESSAGE(MID_MACHINE_UNMATCH_COLLECT));
			}
			else
			{
				GameMsgPrint::getSingleton().showMidTip(EMID_MESSAGE(MID_MACHINE_UNMATCH_COLLECT_MORMAL));
			}
		}
		else
		{
			GameMsgPrint::getSingleton().showMidTip(EMID_MESSAGE(MID_MUST_MACHINE_COLLECT));
		}
		return;
	}

	if ( !GameQuest::getSingleton().canCollectQuest( pNpc->getResId() )  )
	{
		return;
	}

	m_nMonId = nMonId;
	((MainGamePlayer*)pMainPlayer)->m_nCollectMonId = nMonId;

	GameXYZ pos2	= pNpc->GetPosition();
	pos2.y			= 0;
	GameXYZ pos		= pMainPlayer->GetPosition();
	pos.y			= 0;
	unsigned int Dis= (unsigned int)pos2.subtract(pos).length();
	if( Dis <= getMaxCollectDist( pNpc ) )
	{
		startCollect( nMonId ); 
	}else{
		int dist = getMaxCollectDist( pNpc );
		pMainPlayer->CmdMoveTo( (int)Ogre::WorldPos::Fix2Flt(pNpc->GetPosition().x),
								(int)Ogre::WorldPos::Fix2Flt(pNpc->GetPosition().z), 
								max(dist * 3 / 4, dist - 10) );
		m_bCath  = true;
	}
}

// 开始采集
void GameCollect::startCollect( int nMonId )
{
	GameActor* pNpc = ActorManager::getSingleton().FindActor( nMonId );
	if (pNpc == NULL)
	{
		endCollect();
		m_bCath = false;
		return;
	}
	if ( !GameQuest::getSingleton().canCollectQuest( pNpc->getResId() ) )
	{
		endCollect();
		m_bCath = false;
		return;
	}
	if( !SecondarySkillMgr::getSingleton().checkRequireItem(pNpc->getResId()) )
	{
		endCollect();
		m_bCath = false;
		return;
	}
	purgeStartCollect(nMonId);
}

// 结束采集
void GameCollect::endCollect()
{
	tagCSPkgBody msgbody;
	msgbody.CollectClt.OpType = COLLECT_END;
	msgbody.CollectClt.CollectCltData.CollectEnd.MonID = m_nMonId;
	SendGameServerMsg( COLLECT_CLT, msgbody );
	GameMsgPrint::getSingleton().SendUIEvent("GE_BREAK_PRE_ACTION");
}

void GameCollect::update(unsigned int dtick)
{
	GameActor* pMainPlayer	= ActorManager::getSingleton().getMainPlayer();
	GameActor* pNpc			= ActorManager::getSingleton().FindActor( m_nMonId );
	
	if( pMainPlayer == NULL ) 
	{
		return;
	}

	if( pNpc == NULL ) 
	{
		return;
	}

	if( !m_bCath ) 
	{
		return;
	}

	if( pMainPlayer->getActState().GetState() != GSACT_MOVE )
	{
		if( pMainPlayer->getActState().GetState() == GSACT_STAND )
		{
			GameXYZ pos2 = pNpc->GetPosition();
			pos2.y		 = 0; 
			GameXYZ pos  = pMainPlayer->GetPosition();
			pos.y		 = 0;
			unsigned int Dis = (unsigned int)pos2.subtract(pos).length();
			if( Dis <= getMaxCollectDist( pNpc ) )
			{
				pMainPlayer->CmdMoveStop();
				startCollect( m_nMonId ); 
			}
		}else{
			m_bCath = false;
		}
	}
}

bool GameCollect::isInCollect()
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if( pMain == NULL )
	{
		return false;
	}
	return (pMain->getActState().GetState() == GSACT_COLLECTITEM );
}

bool GameCollect::canCollect( int id )
{
	GameActor* pMonster = ActorManager::getSingleton().FindActor( id );
	return canCollect( pMonster );
}

bool GameCollect::canCollect( GameActor* pMonster )
{
	if ( pMonster == NULL )
	{
		return false;
	}
	
	if ( pMonster->getType() != GAT_MONSTER || pMonster->getSubType() != MON_SUB_TYPE_COLLECT )
	{
		return false;
	}

	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMonster->getOwnerId() != 0)
	{
		if (pMainPlayer->GetID() != pMonster->getOwnerId() && (pMainPlayer->getTeamWID() == 0 || pMainPlayer->getTeamWID() != pMonster->getTeamWID()))
		{
			return false;
		}
	}

	if (!WorldCityManager::getSingleton().IsFighting() && WorldCityManager::getSingleton().IsCityBuildFight(pMonster))
	{
		return false;
	}
	
	const char* pszOwerName = pMonster->getOwnerName();
	if ( pszOwerName[0] != 0 )
	{
		if ( strcmp( pszOwerName, pMainPlayer->getName() ) != 0  )
		{
			return false;
		}
	}
	
	const tagMonsterDef& monsterDef = DBDataManager::getSingleton().getMonsterDef( pMonster->getResId() );
	if ( &monsterDef == NULL )
	{
		return false;
	}

	return true;
}

void GameCollect::purgeStartCollect(int nMonId)
{
	// 开始采集前先打断其它吟唱行为
	MainGamePlayer *player = ActorManager::getSingleton().getMainPlayer();
	if (player != NULL && player->getActState().GetState() != GSACT_COLLECTITEM)
	{
		player->breakPreAction();
	}

	tagCSPkgBody msgbody;
	msgbody.CollectClt.OpType = COLLECT_START;
	m_nMonId = nMonId;
	msgbody.CollectClt.CollectCltData.CollectStart.MonID = nMonId;
	SendGameServerMsg( COLLECT_CLT, msgbody );	

	m_bCath = false;
}

bool GameCollect::limitMachineCollectSimple(int resid)
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();

	if (pMainPlayer == NULL)
	{
		return false;
	}

	const tagMonsterDef &MonsterDef = DBDataManager::getSingleton().getMonsterDef(resid);
	if (pMainPlayer->isOnMonsterMachine())
	{
		MonsterMachine* pMonsterMachine = pMainPlayer->getMonsterMachine();
		tagMonMachineDef_ToLua* monMachineDef = pMonsterMachine->getMonMachineDef();
		if (monMachineDef == NULL)
		{
			return false;
		}
		if (MonsterDef.Collect.CollectType != monMachineDef->CollectType)
		{
			return false;
		}
	}
	return true;
}

bool GameCollect::limitMachineCollectMore(int resid)
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();

	if (pMainPlayer == NULL)
	{
		return false;
	}

	if (!pMainPlayer->isOnMonsterMachine())
	{
		const tagMonsterDef &MonsterDef = DBDataManager::getSingleton().getMonsterDef(resid);
// 		if (MonsterDef.Collect.CollectType != 0)
// 		{
// 			return false;
// 		}
		if (MonsterDef.Collect.CollectFlag != 0)
		{
			return false;
		}
		return true;
	}

	MonsterMachine* pMonsterMachine = pMainPlayer->getMonsterMachine();
	if (pMonsterMachine->getMonsterEnergy() <= 0)
	{
		return false;
	}
	tagMonMachineDef_ToLua* monMachineDef = pMonsterMachine->getMonMachineDef();
	for (size_t i = 0; i < MAX_MACHINE_LIMIT_ORE; ++i)
	{
		if (monMachineDef->LimitOreMonD[i] == resid)
		{
			return true;
		}
	}
	return false;
}

bool GameCollect::limitMachineCollect(int resid)
{
	return limitMachineCollectSimple(resid) && limitMachineCollectMore(resid);
}

bool GameCollect::limitMachineCollect(GameActor* pMonster)
{
	if (pMonster == NULL)
	{
		return false;
	}

	if (pMonster->getType() != GAT_MONSTER || pMonster->getSubType() != MON_SUB_TYPE_COLLECT)
	{
		return false;
	}

	return limitMachineCollect(pMonster->getResId());
}
