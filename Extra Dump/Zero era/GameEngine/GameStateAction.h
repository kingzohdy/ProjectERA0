
#ifndef __GameStateAction_H__
#define __GameStateAction_H__

#include "GameEngineTypes.h"
#include "GameActionDefine.h"

enum
{
	STATE_CHANGE,			// ���л�
	STATE_UNACT_CHANGE,		// ���л�����������
	STATE_STAND_CHANGE,		// �л���վ�����й�ȥ
	STATE_UNCHANGE,			// �����л�
};

// ����״̬
enum
{
	GSACT_STAND = 0,		// վ��
	GSACT_MOVE,				// �ƶ�
	GSACT_SIT,				// ����
	GSACT_DEAD,				// ����
	GSACT_STOP,				// ����
	GSACT_COLLECT,			// ���ܼ���
	GSACT_PREPARE,			// ��������	
	GSACT_ATTACK,			// ����ѭ������
	GSACT_MOVEBACK,			// ����ع�
	GSACT_COLLECTITEM,		// �ɼ�
	GSACT_AUTOFLY,			// �Զ�����
	GSACT_FLY,				// ����
	GSACT_STALL,			// ��̯
	GSACT_SHAMDEAD,			// ����

	MAX_ACT_STATE
};

// ��������״̬
enum
{
	GSACT_IN_FIGHT = MAX_ACT_STATE,		// ս��׼��
	GSACT_IN_RIDE,						// ���״̬
	GSACT_IN_MORPH,						// ����״̬
	GSACT_IN_TETANIC,					// ������ֱ״̬
	GSACT_IN_BIRTH,						// ����
	
	GSACT_IN_CAST,						// ʩ��
	GSACT_IN_LEAVEFIGHT,				// ����ս��
	GSACT_IN_USEDMACHINE,				// ʹ����ʱ��е
	GSACT_IN_CHANGEEQUIP,				// �л�װ��

	GSACT_IN_CARRY,						// ��

	MAX_IN_STATE
};

int getState( int nCurState, int nNextState );

class GameState
{
public:
	enum{ MAX_STATEDATA = 2 };

public:
	GameState( int state );
	
	// ����״̬
	void	SetState( int state );
	// ��ȡ״̬
	int		GetState();
	// �Ƿ�����л�״̬
	int		canChangeState( int state );
	// 

	void setInState( int state, bool flag );
	bool getInState( int state );
	
	// ������̬
	void setPoseState( int state );
	// ��ȡ��̬
	int	 getPoseState();

	// ��ȡ��ǰ״̬����Id, ����isLosePose�Ƿ������̬���ÿ�����̬
	int				getActionId( bool isLosePose = false );

	const char*		getActionName();
	int				getActionId( int actionId );

	int				getDeadActId();
	int				getAttackFlyActId();
	int				getWalkActId();
private:
	int		m_State;						// ����״̬
	bool	m_InState[MAX_IN_STATE];		// ����״̬
	int		m_PoseState;					// װ����̬

	unsigned int m_StateData[MAX_STATEDATA];
};

#endif