#include "GameNpc.h"
#include "GameMap.h"
#include "OgreEntity.h"
#include "OgreVector2.h"
#include "ShowActor.h"
#include "GameDBData.h"
#include "GameMainPlayer.h"
#include "GameActor_Tick.h"
#include <Windows.h>

using namespace Ogre;
static std::map<int, tagNpcQusetInfo> g_NpcQuestInfoMap;

GameNPC::GameNPC()
{
	m_lastXunluo = false;
}

bool isFixGetTask( int nQuestID )
{
	const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( nQuestID );
	if ( &taskDef == NULL )
	{
		return true;
	}

	return taskDef.TaskFlag != TASK_HIDE;
}

// 获取任务列表
void getQuestList( int npcId )
{
	if( &DBDataManager::getSingleton().getNpcInfoDef( npcId ) == NULL )
	{
		return;
	}

	std::map<int, tagNpcQusetInfo>::iterator itermap = g_NpcQuestInfoMap.find( npcId );
	if( itermap != g_NpcQuestInfoMap.end() )
	{
		return;
	}

	const tagNpcInfoDef& npcInfo = DBDataManager::getSingleton().getNpcInfoDef( npcId );
	tagNpcQusetInfo& oneNpcQuestInfo = g_NpcQuestInfoMap[npcId];
	for( int i = 0; i < MAX_NPC_TASK; i++ )
	{
		int nOneAcceptTaskID = npcInfo.AcceptTaskIDs[i];
		if( nOneAcceptTaskID != 0  )
		{
			if ( isFixGetTask( nOneAcceptTaskID ) )
			{
				oneNpcQuestInfo.m_HaveAcceptQuestId.push_back( nOneAcceptTaskID );
			}
			else
			{
				oneNpcQuestInfo.m_HaveAcceptDynamicQuestId.push_back( nOneAcceptTaskID );
			}
		}
		
		if( npcInfo.CommitTaskIDs[i] != 0 )
		{
			oneNpcQuestInfo.m_HaveFiniQuestId.push_back( npcInfo.CommitTaskIDs[i] );
		}
	}

	for( int i = 0; i < MAX_NPC_TASKCYCLE; i++ )
	{
		if( npcInfo.AptCycleIDs[i].TaskCycleID != 0 )
		{
			oneNpcQuestInfo.m_HaveAcceptRecycle.push_back( npcInfo.AptCycleIDs[i] );
		}
		if( npcInfo.CmtCycleIDs[i].TaskCycleID != 0 )
		{
			oneNpcQuestInfo.m_HaveFiniRecycle.push_back( npcInfo.CmtCycleIDs[i] );
		}
	}
	oneNpcQuestInfo.m_nQuestShow = -1;
}

tagNpcQusetInfo& getNpcQuestInfo( int npcId )
{
	return g_NpcQuestInfoMap[npcId];
}

void clearNpcQuestList()
{
	for ( std::map<int, tagNpcQusetInfo>::iterator iter = g_NpcQuestInfoMap.begin(), end = g_NpcQuestInfoMap.end();
			iter != end; ++iter )
	{
		tagNpcQusetInfo& oneNpcQuestInfo	= iter->second;
		oneNpcQuestInfo.m_nQuestShow		= -1;
	}
}

// @param[out]   未找到任务标志的NPC -2 无任务 -1 可领取(!) 0  可提交(?) 1 不可提交(?) 2 循环任务可领取(!) 3 不可接任务(!) 4  
int getNpcQuestShow( int npcId )
{
    std::map<int, tagNpcQusetInfo>::iterator npciter = g_NpcQuestInfoMap.find( npcId );
    if( npciter != g_NpcQuestInfoMap.end() )
    {
        return g_NpcQuestInfoMap[npcId].m_nQuestShow;
    }
    return -2;
}

int getMonsterRadius( int nResId )
{
	const tagMonsterDef* pMonsterDef = &DBDataManager::getSingleton().getMonsterDef( nResId );
	if( pMonsterDef == NULL )
	{
		return 0;
	}
	return	pMonsterDef->Radius;
}

int getMonsterSubtype( int nResId )
{
	const tagMonsterDef* pMonsterDef = &DBDataManager::getSingleton().getMonsterDef( nResId );
	if( pMonsterDef == NULL )
	{
		return 0;
	}
	return	pMonsterDef->SubType;
}

int getMonsterLevel( int nResId )
{
	const tagMonsterDef* pMonsterDef = &DBDataManager::getSingleton().getMonsterDef( nResId );
	if( pMonsterDef == NULL )
	{
		return 0;
	}
	return	pMonsterDef->Level;
}

int getMonsterNameColor( int nResId )
{
	MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer == NULL )
	{
		return MONSTER_GREEN;
	}

	int	nMainPlayerLv = pMainPlayer->getLv();
	const tagMonsterDef* pMonsterDef = &DBDataManager::getSingleton().getMonsterDef( nResId );
	if ( pMonsterDef == NULL )
	{
		return MONSTER_GREEN;
	}

	if ( nMainPlayerLv >= pMonsterDef->ColorLevel.GreenLevel )
	{
		return	MONSTER_GREEN;
	}

	if ( nMainPlayerLv >= pMonsterDef->ColorLevel.WhiteLevel )
	{
		return MONSTER_WHITE;
	}

	if ( nMainPlayerLv >= pMonsterDef->ColorLevel.RedLevel )
	{
		return MONSTER_YELLOW;
	}

	if ( nMainPlayerLv >= pMonsterDef->ColorLevel.BlackLevel )
	{
		return MONSTER_ORANGE;
	}

	return MONSTER_RED;
}

const char* getMonsterName( int nResId )
{
	if( nResId == 0 )
	{
		return "击杀怪物";
	}
	const tagMonsterDef* pMonsterDef = &DBDataManager::getSingleton().getMonsterDef( nResId );
	if( pMonsterDef == NULL )
	{
		return "";
	}
	return	pMonsterDef->Name;
}

int getMonsterIcon(int nResId)
{
	const tagMonsterDef* pMonsterDef = &DBDataManager::getSingleton().getMonsterDef( nResId );
	if( pMonsterDef == NULL )
	{
		return 0;
	}
	return	pMonsterDef->IconID;
}

int getMonsterMaxHP(int nResId)
{
	const tagMonsterDef* pMonsterDef = &DBDataManager::getSingleton().getMonsterDef( nResId );
	if( pMonsterDef == NULL )
	{
		return 0;
	}
	return	pMonsterDef->HP;
}

const char* getNpcDesc( int npcId )
{
	const tagNpcDisplayInfo* pNpcDisplayInfo = &DBDataManager::getSingleton().GetNpcDisplayInfo( npcId );
	if( pNpcDisplayInfo == NULL )
	{
		return "";
	}
	return pNpcDisplayInfo->npcdesc;
}

const char* GetNpcName( int nResid )
{
	return DBDataManager::getSingleton().GetNpcDisplayInfo( nResid ).name;
}

int getMonMachineMaxEnergy(int resid)
{
	return DBDataManager::getSingleton().getMonMachineDef(resid)->MaxEnergy;
}

void GameNPC::onBirth( int nBirthTime )
{
	if( nBirthTime == 0 )
	{
		return;
	}

	registerTickEvent( new GameActor_Tick_MonsterBirth( this, timeGetTime(), nBirthTime*1000 ) );
	
}

void GameNPC::CmdMoveBack( int to_x, int to_z )
{
	if (m_pCurMap == NULL)
	{
		return;
	}
	ChangeActState(GSACT_MOVEBACK);

	m_nMoveBackTime = timeGetTime();
	if( m_nSpeed == 0 ) return;

	Point2D start(to_x, to_z);
	Point2D scale(m_pCurMap->getMaskMap()->m_lMaskPixelWidth, m_pCurMap->getMaskMap()->m_lMaskPixelHeight);
	Point2D s_StartGrid = start / scale;
	if( !m_pCurMap->getMaskMap()->canWalk(s_StartGrid.x, s_StartGrid.y) ) return ;

	m_MoveEndPos.x = m_MoveEndPos.y = m_MoveEndPos.z = 0;

	int cur_x, cur_z;
	m_pMapMover->GetCurPos(cur_x, cur_z);

	GameMaskPath path;
	if( !m_pCurMap->FindPath(path, cur_x, cur_z, to_x, to_z, MASK_STOP | DYN_MASK_STOP, true ) )
	{
		return;
	}

	size_t npoint = path.getNumPoint();
	assert(npoint > 0);
	if( npoint - 1 > CS_MAX_MOVEPOINTS/2 )
	{
		m_MoveEndPos.x = to_x;
		m_MoveEndPos.y = to_z;
		path.limitPathPoint( CS_MAX_MOVEPOINTS/2 + 1 );
	}

	m_pMapMover->StartOnPath(&path);
}

bool GameNPC::UpdateMoveBack()
{
	float fTransparent = m_pShowActor->getCurEntity()->getTransparent();
	fTransparent -= 0.08f;
	if( timeGetTime() - m_nMoveBackTime >= 4000 )
	{
		return false;
	}
	return true;
}

void GameNPC::setAtkType()
{
	if( m_Type == GAT_MONSTER || m_Type == GAT_NPC )
	{
		const tagMonsterDef& MonsterDef = DBDataManager::getSingleton().getMonsterDef( m_ResID );
		m_nAtkType = MonsterDef.AtkType; 
	}

	m_nPlayFreeActTime	= timeGetTime();
	m_nNpcAngle			= getAngle();
	m_nQusetShow		= -1;
}

void GameNPC::setSubType()
{
	if( m_Type == GAT_MONSTER )
	{
		const tagMonsterDef& MonsterDef = DBDataManager::getSingleton().getMonsterDef( m_ResID );
		if( &MonsterDef != NULL )
		{
			m_nSubType = MonsterDef.SubType; 
		}
	}
}

void GameNPC::updateUI( int nChangeValue )
{
	GameActor::updateUI( nChangeValue );
// 	if( ActorManager::getSingleton().getMainPlayer() == NULL  ) 
// 	{
// 		return;
// 	}
// 	
// 	if( GetID() == ActorManager::getSingleton().getMainPlayer()->GetSelectTarget() )
// 	{
// 		GameMsgPrint::getSingleton().SendUIEvent( "UI_TARGET_ATTRIBUTE_CHG" );
// 	}
}

void GameNPC::Update(unsigned int dtick)
{
	GameActor::Update( dtick );
	if (getSubType() == MON_SUB_TYPE_XUNLUO)
	{
		bool inside = false;
		GameActor *pActor = ActorManager::getSingleton().getMainPlayer();
		if (pActor)
		{
			const tagMonsterDef &MonsterDef = DBDataManager::getSingleton().getMonsterDef(getResId());
			GameXYZ selfPos = GetPosition();
			GameXYZ pos = pActor->GetPosition();
			pos.y = selfPos.y;
			if (pos.subtract(selfPos).length() <= MonsterDef.Radius)
			{
				inside = true;
			}
		}
		if (inside)
		{
			if (!m_lastXunluo)
			{
				m_lastXunluo = true;
				tagCSPkgBody msgbody;
				msgbody.PrepareAtkInfo.PrepareAtkOp = NORMALATK_OP_AKEE;
				msgbody.PrepareAtkInfo.PrepareAtkData.NormalAtkByHit.AtkerID = GetID();
				msgbody.PrepareAtkInfo.PrepareAtkData.NormalAtkByHit.HitTarget = pActor->GetID();
				SendGameServerMsg(PREPAREATK_CLT, msgbody);
			}
		}
		else
		{
			m_lastXunluo = false;
		}
	}
	bool playFree = false;
	unsigned int now = timeGetTime();
	if (now - m_nPlayFreeActTime >= 10000 && getType() == GAT_NPC)
	{
		playFree = true;
	}
	else if (now - m_nPlayFreeActTime >= 5000 && getType() == GAT_MONSTER)
	{
		if (rand() % 100 >= 80)
		{
			playFree = true;
		}
	}
	if (playFree)
	{
		if (m_ActState.GetState() == GSACT_STAND && m_ActState.GetState() != GSACT_COLLECT && !m_ActState.getInState( GSACT_IN_FIGHT ) && !isInSkillTetanic())
		{
			if( getSubType() != MON_SUB_TYPE_MACHINE )
			{
				playAction( 10103, NULL, 0, -1 );
			}
		}
		m_nPlayFreeActTime = now;
	}
}

// 还原朝向
void GameNPC::reSetAngle()
{
	if( !m_bIsPatrol )
	{
		SetFaceTo( m_nNpcAngle );
	}
}

bool GameNPC::isPatrol()
{
	return m_bIsPatrol;
}