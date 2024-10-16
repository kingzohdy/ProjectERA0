
#include "GameStateAction.h"
#include "OgreTimer.h"
#include <Windows.h>
#include <assert.h>

// 状态表 0可切换, 1可切换但动作不变, 2切换成站立再切过去, 3不可切换
static const int STATE_MAP[MAX_IN_STATE][MAX_IN_STATE] =
{
//				站立	移动	坐下	死亡	冻结	技能集气	技能引导	技能循环攻击	怪物回归	采集	自动飞行	飞行	摆摊	假死	战斗准备	骑乘状态	变身状态	法术僵直状态	出场	技能施放	脱离战斗	使用临时机械	切换装备	背
/*站立*/		{ 0,	0,		0, 		0,		0,		0,			0,			0,				0,			0,		0,			0,		0,		0,			0,			0,			0,			0,				0,		0,			0,			0,				0,			0,	},			/*站立*/		
/*移动*/		{ 0,	0,		0,		0,		0,		0,			0,			0,				0,			0,		0,			2,		0,		0,			0,			2,			0,			0,				0,		0,			0,			0,				0,			0,	},			/*移动*/		
/*坐下*/		{ 0,	0,		3,		0,		3,		0,			0,			0,				0,			0,		0,			0,		0,		0,			1,			2,			0,			0,				0,		0,			1,			0,				0,			3,	},			/*坐下*/		
/*死亡*/		{ 3,	3,		3,		3,		3,		3,			3,			3,				3,			3,		3,			3,		3,		0,			3,			3,			3,			3,				3,		3,			1,			0,				0,			0,	},			/*死亡*/		
/*冻结*/		{ 3,	3,		3,		0,		0,		3,			3,			3,				3,			3,		3,			3,		3,		0,			3,			3,			0,			3,				0,		1,			0,			0,				0,			3,	},			/*冻结*/		
/*技能集气*/	{ 0,	0,		0, 		0,		0,		0,			0,			0,				0,			0,		0,			0,		0,		0,			0,			3,			0,			0,				0,		0,			1,			0,				1,			3,	},			/*技能集气*/	
/*技能引导*/	{ 0,	0,		0, 		0,		0,		0,			0,			0,				0,			0,		0,			0,		0,		0,			0,			3,			0,			0,				0,		0,			1,			0,				1,			0,	},			/*技能引导*/	
/*技能循环攻击*/{ 0,	0,		0, 		0,		0,		0,			0,			0,				0,			0,		0,			0,		0,		0,			0,			3,			0,			0,				0,		0,			0,			0,				0,			3,	},			/*技能循环攻击*/
/*怪物回归*/	{ 0,	0,		0, 		0,		0,		0,			0,			0,				0,			0,		0,			0,		0,		0,			0,			0,			0,			0,				0,		0,			0,			0,				0,			3,	},			/*怪物回归*/	
/*采集*/		{ 0,	2,		2, 		0,		0,		3,			3,			3,				0,			0,		3,			3,		3,		0,			1,			3,			0,			0,				0,		2,			1,			0,				1,			3,	},			/*采集*/		
/*自动飞行*/	{ 0,	3,		3,		3,		3,		3,			3,			3,				0,			3,		3,			3,		3,		0,			3,			3,			3,			3,				3,		3,			0,			0,				0,			3,	},			/*自动飞行*/	
/*飞行*/		{ 1,	1,		3, 		0,		0,		3,			3,			3,				0,			0,		0,			0,		2,		0,			3,			3,			0,			3,				0,		3,			0,			0,				0,			3,	},			/*飞行*/		
/*摆摊*/		{ 3,	3,		3,		3,		3,		3,			3,			3,				3,			3,		3,			3,		3,		0,			3,			3,			3,			3,				3,		3,			0,			0,				3,			3,	},			/*摆摊*/		
/*假死*/		{ 0,	0,		0,		0,		0,		0,			0,			0,				0,			0,		0,			0,		0,		1,			0,			0,			0,			0,				0,		0,			1,			0,				0,			0,	},			/*假死*/		
/*战斗准备*/	{ 0,	0,		1, 		0,		0,		1,			1,			1,				0,			0,		0,			0,		0,		0,			1,			0,			0,			0,				0,		0,			0,			1,				0,			0,	},			/*战斗准备*/	
/*骑乘状态*/	{ 1,	1,		3, 		0,		0,		3,			3,			3,				0,			1,		2,			2,		2,		0,			1,			0,			3,			0,				0,		0,			1,			0,				1,			3,	},			/*骑乘状态*/	
/*变身状态*/	{ 0,	0,		3, 		0,		0,		3,			3,			3,				0,			1,		3,			3,		3,		0,			1,			3,			3,			0,				0,		0,			0,			0,				3,			3,	},			/*变身状态*/	
/*法术僵直状态*/{ 0,	3,		3, 		0,		0,		3,			3,			3,				0,			0,		0,			0,		0,		0,			0,			0,			0,			0,				0,		3,			0,			0,				0,			0,	},			/*法术僵直状态*/
/*出场*/		{ 0,	0,		0, 		0,		0,		0,			0,			0,				0,			0,		0,			0,		0,		0,			0,			0,			0,			0,				0,		0,			0,			0,				0,			3,	},			/*出场*/		
/*技能施放*/	{ 0,	0,		0, 		0,		0,		0,			0,			0,				0,			0,		0,			0,		0,		0,			0,			0,			0,			0,				0,		0,			0,			0,				0,			3,	},			/*技能施放*/	
/*脱离战斗*/	{ 0,	0,		1, 		0,		0,		1,			1,			1,				0,			1,		1,			1,		1,		0,			1,			0,			1,			0,				0,		0,			1,			1,				0,			3,	},			/*脱离战斗*/	
/*使用临时机械*/{ 1,	1,		1, 		0,		0,		1,			1,			1,				0,			1,		3,			3,		3,		0,			1,			3,			3,			0,				0,		0,			1,			0,				1,			3,	},			/*使用临时机械*/
/*切换装备*/	{ 0,	0,		0,		0,		0,		0,			0,			0,				0,			0,		0,			0,		0,		0,			0,			0,			0,			0,				0,		0,			0,			0,				0,			1,	},			/*切换装备*/
/*背*/			{ 0,	0,		3,		0,		0,		3,			3,			3,				3,			3,		3,			3,		3,		0,			3,			3,			3,			3,				3,		3,			0,			3,				1,			3,	},			/*背*/
//				站立	移动	坐下	死亡	冻结	技能集气	技能引导	技能循环攻击	怪物回归	采集	自动飞行	飞行	摆摊	假死		战斗准备	骑乘状态	变身状态	法术僵直状态	出场	技能施放	脱离战斗	使用临时机械	切换装备	背

};

// 已处理的有GSACT_IN_FIGHT,GSACT_IN_RIDE,GSACT_IN_CHANGEEQUIP

int getState( int nCurState, int nNextState )
{
	return STATE_MAP[nCurState][nNextState];
}

GameState::GameState( int state ) : m_State(state)
{
	for( int i = 0; i < MAX_IN_STATE; i++ )
	{
		m_InState[i] = false;
	}
	m_PoseState = POSE_NONE;
}

void GameState::SetState( int state )
{
	m_State = state;
}

int GameState::GetState()
{ 
	return m_State;
}

int GameState::canChangeState( int state )
{
	int nState = getState( m_State, state );
	for( int i = MAX_ACT_STATE; i < MAX_IN_STATE; i++ )
	{
		if( m_InState[i] && getState( i, state ) > nState )
		{
			nState = getState( i, state );
		}
	}
	if (nState == 0)
	{
		return getState(m_State, state);
	}
	return nState;
}

void GameState::setPoseState( int state )
{
	m_PoseState = state;
}

int GameState::getPoseState()
{
	return m_PoseState;
}

void GameState::setInState( int state, bool flag )
{
	assert( state < MAX_IN_STATE );
	m_InState[state] = flag;
}

bool GameState::getInState( int state )
{
	assert( state < MAX_IN_STATE );
	return m_InState[state];
}

int GameState::getActionId( bool isLosePose )
{
	int ActionType	= ACTSTA_NORMAL;
	int SlotAction	= ACTSTA_NORMAL_RUN;
	switch( m_State )
	{
	case GSACT_STAND:
		{
			SlotAction = ACTSTA_NORMAL_STAND;
			// 若骑乘的时候进入战斗状态，是不能进入备战状态的
			if( getInState(GSACT_IN_FIGHT) && !getInState( GSACT_IN_RIDE ) )
			{
				SlotAction = ACTSTA_NORMAL_PRE_STAND;
			}
		}
		break;
	case GSACT_MOVE:	
		{
			SlotAction = ACTSTA_NORMAL_RUN;
		}
		break;
	case GSACT_DEAD:
		{
			SlotAction = ACTSTA_NORMAL_DEAD;
		}
		break;
	case GSACT_FLY:
		{
			SlotAction = ACTSTA_NORMAL_FLY;
		}
		break;
	case GSACT_AUTOFLY:
		{
			SlotAction = ACTSTA_NORMAL_RIDING_STAND;
		}
		break;
	case GSACT_SIT:
	case GSACT_STOP:
	case GSACT_COLLECT:
	case GSACT_PREPARE:
		return -1;
		break;
	case GSACT_ATTACK:
		{
			
		}
		break;
	case GSACT_COLLECTITEM:
		{
			ActionType = ACTSTA_READING;
			SlotAction = ACTSTA_NORMAL_STAND;
		}
		break;
	case GSACT_SHAMDEAD:
		{
			return 10704;
		}
		break;
	}
	
	if( isLosePose )
	{
		return getActId( POSE_NONE, ActionType, SlotAction );
	}
	
	return getActId( m_PoseState, ActionType, SlotAction );
}

const char*	GameState::getActionName()
{
	static char ActionName[256];
	sprintf( ActionName, "%d", getActionId() );
	return ActionName;
}

int GameState::getActionId( int actionId )
{
	int oldstate = m_State;
	int type = getSlotActionIdByShortId( actionId );

	// 循环攻击切换到攻击状态
	if( type == ACTSTA_ATTACKLOOP )
	{
		//m_State = GSACT_ATTACK;
	}
	
	// 吟唱或者集气切换到集气状态
	if( type == ACTSTA_READING )
	{
		m_State = GSACT_COLLECT;
	}

	int result = getActId(m_PoseState, actionId);
	m_State = oldstate;

	return result;
}

int GameState::getDeadActId()
{
	return getActId( m_PoseState, ACTSTA_NORMAL, ACTSTA_NORMAL_DEAD_BONE );
}

int GameState::getAttackFlyActId()
{
	return getActId( POSE_NONE, ACTSTA_NORMAL, ACTSTA_NORMAL_HIT_BACK );
}

int GameState::getWalkActId()
{
	return getActId( POSE_NONE, ACTSTA_NORMAL, ACTSTA_NORMAL_WALK );
}