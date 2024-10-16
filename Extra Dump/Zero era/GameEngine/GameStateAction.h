
#ifndef __GameStateAction_H__
#define __GameStateAction_H__

#include "GameEngineTypes.h"
#include "GameActionDefine.h"

enum
{
	STATE_CHANGE,			// 可切换
	STATE_UNACT_CHANGE,		// 可切换但动作不变
	STATE_STAND_CHANGE,		// 切换成站立再切过去
	STATE_UNCHANGE,			// 不可切换
};

// 基本状态
enum
{
	GSACT_STAND = 0,		// 站立
	GSACT_MOVE,				// 移动
	GSACT_SIT,				// 坐下
	GSACT_DEAD,				// 死亡
	GSACT_STOP,				// 冻结
	GSACT_COLLECT,			// 技能集气
	GSACT_PREPARE,			// 技能引导	
	GSACT_ATTACK,			// 技能循环攻击
	GSACT_MOVEBACK,			// 怪物回归
	GSACT_COLLECTITEM,		// 采集
	GSACT_AUTOFLY,			// 自动飞行
	GSACT_FLY,				// 飞行
	GSACT_STALL,			// 摆摊
	GSACT_SHAMDEAD,			// 假死

	MAX_ACT_STATE
};

// 二级并行状态
enum
{
	GSACT_IN_FIGHT = MAX_ACT_STATE,		// 战斗准备
	GSACT_IN_RIDE,						// 骑乘状态
	GSACT_IN_MORPH,						// 变身状态
	GSACT_IN_TETANIC,					// 法术僵直状态
	GSACT_IN_BIRTH,						// 出场
	
	GSACT_IN_CAST,						// 施法
	GSACT_IN_LEAVEFIGHT,				// 脱离战斗
	GSACT_IN_USEDMACHINE,				// 使用临时机械
	GSACT_IN_CHANGEEQUIP,				// 切换装备

	GSACT_IN_CARRY,						// 背

	MAX_IN_STATE
};

int getState( int nCurState, int nNextState );

class GameState
{
public:
	enum{ MAX_STATEDATA = 2 };

public:
	GameState( int state );
	
	// 设置状态
	void	SetState( int state );
	// 获取状态
	int		GetState();
	// 是否可以切换状态
	int		canChangeState( int state );
	// 

	void setInState( int state, bool flag );
	bool getInState( int state );
	
	// 设置姿态
	void setPoseState( int state );
	// 获取姿态
	int	 getPoseState();

	// 获取当前状态动作Id, 参数isLosePose是否忽略姿态启用空手姿态
	int				getActionId( bool isLosePose = false );

	const char*		getActionName();
	int				getActionId( int actionId );

	int				getDeadActId();
	int				getAttackFlyActId();
	int				getWalkActId();
private:
	int		m_State;						// 基本状态
	bool	m_InState[MAX_IN_STATE];		// 并行状态
	int		m_PoseState;					// 装备姿态

	unsigned int m_StateData[MAX_STATEDATA];
};

#endif