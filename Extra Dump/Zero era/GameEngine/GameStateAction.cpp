
#include "GameStateAction.h"
#include "OgreTimer.h"
#include <Windows.h>
#include <assert.h>

// ״̬�� 0���л�, 1���л�����������, 2�л���վ�����й�ȥ, 3�����л�
static const int STATE_MAP[MAX_IN_STATE][MAX_IN_STATE] =
{
//				վ��	�ƶ�	����	����	����	���ܼ���	��������	����ѭ������	����ع�	�ɼ�	�Զ�����	����	��̯	����	ս��׼��	���״̬	����״̬	������ֱ״̬	����	����ʩ��	����ս��	ʹ����ʱ��е	�л�װ��	��
/*վ��*/		{ 0,	0,		0, 		0,		0,		0,			0,			0,				0,			0,		0,			0,		0,		0,			0,			0,			0,			0,				0,		0,			0,			0,				0,			0,	},			/*վ��*/		
/*�ƶ�*/		{ 0,	0,		0,		0,		0,		0,			0,			0,				0,			0,		0,			2,		0,		0,			0,			2,			0,			0,				0,		0,			0,			0,				0,			0,	},			/*�ƶ�*/		
/*����*/		{ 0,	0,		3,		0,		3,		0,			0,			0,				0,			0,		0,			0,		0,		0,			1,			2,			0,			0,				0,		0,			1,			0,				0,			3,	},			/*����*/		
/*����*/		{ 3,	3,		3,		3,		3,		3,			3,			3,				3,			3,		3,			3,		3,		0,			3,			3,			3,			3,				3,		3,			1,			0,				0,			0,	},			/*����*/		
/*����*/		{ 3,	3,		3,		0,		0,		3,			3,			3,				3,			3,		3,			3,		3,		0,			3,			3,			0,			3,				0,		1,			0,			0,				0,			3,	},			/*����*/		
/*���ܼ���*/	{ 0,	0,		0, 		0,		0,		0,			0,			0,				0,			0,		0,			0,		0,		0,			0,			3,			0,			0,				0,		0,			1,			0,				1,			3,	},			/*���ܼ���*/	
/*��������*/	{ 0,	0,		0, 		0,		0,		0,			0,			0,				0,			0,		0,			0,		0,		0,			0,			3,			0,			0,				0,		0,			1,			0,				1,			0,	},			/*��������*/	
/*����ѭ������*/{ 0,	0,		0, 		0,		0,		0,			0,			0,				0,			0,		0,			0,		0,		0,			0,			3,			0,			0,				0,		0,			0,			0,				0,			3,	},			/*����ѭ������*/
/*����ع�*/	{ 0,	0,		0, 		0,		0,		0,			0,			0,				0,			0,		0,			0,		0,		0,			0,			0,			0,			0,				0,		0,			0,			0,				0,			3,	},			/*����ع�*/	
/*�ɼ�*/		{ 0,	2,		2, 		0,		0,		3,			3,			3,				0,			0,		3,			3,		3,		0,			1,			3,			0,			0,				0,		2,			1,			0,				1,			3,	},			/*�ɼ�*/		
/*�Զ�����*/	{ 0,	3,		3,		3,		3,		3,			3,			3,				0,			3,		3,			3,		3,		0,			3,			3,			3,			3,				3,		3,			0,			0,				0,			3,	},			/*�Զ�����*/	
/*����*/		{ 1,	1,		3, 		0,		0,		3,			3,			3,				0,			0,		0,			0,		2,		0,			3,			3,			0,			3,				0,		3,			0,			0,				0,			3,	},			/*����*/		
/*��̯*/		{ 3,	3,		3,		3,		3,		3,			3,			3,				3,			3,		3,			3,		3,		0,			3,			3,			3,			3,				3,		3,			0,			0,				3,			3,	},			/*��̯*/		
/*����*/		{ 0,	0,		0,		0,		0,		0,			0,			0,				0,			0,		0,			0,		0,		1,			0,			0,			0,			0,				0,		0,			1,			0,				0,			0,	},			/*����*/		
/*ս��׼��*/	{ 0,	0,		1, 		0,		0,		1,			1,			1,				0,			0,		0,			0,		0,		0,			1,			0,			0,			0,				0,		0,			0,			1,				0,			0,	},			/*ս��׼��*/	
/*���״̬*/	{ 1,	1,		3, 		0,		0,		3,			3,			3,				0,			1,		2,			2,		2,		0,			1,			0,			3,			0,				0,		0,			1,			0,				1,			3,	},			/*���״̬*/	
/*����״̬*/	{ 0,	0,		3, 		0,		0,		3,			3,			3,				0,			1,		3,			3,		3,		0,			1,			3,			3,			0,				0,		0,			0,			0,				3,			3,	},			/*����״̬*/	
/*������ֱ״̬*/{ 0,	3,		3, 		0,		0,		3,			3,			3,				0,			0,		0,			0,		0,		0,			0,			0,			0,			0,				0,		3,			0,			0,				0,			0,	},			/*������ֱ״̬*/
/*����*/		{ 0,	0,		0, 		0,		0,		0,			0,			0,				0,			0,		0,			0,		0,		0,			0,			0,			0,			0,				0,		0,			0,			0,				0,			3,	},			/*����*/		
/*����ʩ��*/	{ 0,	0,		0, 		0,		0,		0,			0,			0,				0,			0,		0,			0,		0,		0,			0,			0,			0,			0,				0,		0,			0,			0,				0,			3,	},			/*����ʩ��*/	
/*����ս��*/	{ 0,	0,		1, 		0,		0,		1,			1,			1,				0,			1,		1,			1,		1,		0,			1,			0,			1,			0,				0,		0,			1,			1,				0,			3,	},			/*����ս��*/	
/*ʹ����ʱ��е*/{ 1,	1,		1, 		0,		0,		1,			1,			1,				0,			1,		3,			3,		3,		0,			1,			3,			3,			0,				0,		0,			1,			0,				1,			3,	},			/*ʹ����ʱ��е*/
/*�л�װ��*/	{ 0,	0,		0,		0,		0,		0,			0,			0,				0,			0,		0,			0,		0,		0,			0,			0,			0,			0,				0,		0,			0,			0,				0,			1,	},			/*�л�װ��*/
/*��*/			{ 0,	0,		3,		0,		0,		3,			3,			3,				3,			3,		3,			3,		3,		0,			3,			3,			3,			3,				3,		3,			0,			3,				1,			3,	},			/*��*/
//				վ��	�ƶ�	����	����	����	���ܼ���	��������	����ѭ������	����ع�	�ɼ�	�Զ�����	����	��̯	����		ս��׼��	���״̬	����״̬	������ֱ״̬	����	����ʩ��	����ս��	ʹ����ʱ��е	�л�װ��	��

};

// �Ѵ������GSACT_IN_FIGHT,GSACT_IN_RIDE,GSACT_IN_CHANGEEQUIP

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
			// ����˵�ʱ�����ս��״̬���ǲ��ܽ��뱸ս״̬��
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

	// ѭ�������л�������״̬
	if( type == ACTSTA_ATTACKLOOP )
	{
		//m_State = GSACT_ATTACK;
	}
	
	// �������߼����л�������״̬
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