#pragma once
#include "OgreMemoryDelegation.h"
#include <stdlib.h>

const int LOOP_TICK = -1;

class GameActor_Tick : public Ogre::MemoryDelegation
{
public:
	GameActor_Tick( void* pOwner );
	GameActor_Tick( void* pOwner, unsigned int uStartTime/*开始时间*/, int	nPassTime/*所需时间*/ );
	virtual ~GameActor_Tick();
public:	
	enum
	{
		TICK_BEGIN,		// 开始
		TICK_PROC,		// 过程
		TICK_END,		// 结束
	};

	// 设置起始时间
	void setBeginTime( unsigned int uStartTime/*开始时间*/, int	nPassTime/*所需时间*/ );
	// 设置时间
	void setTime( unsigned int uStartTime/*开始时间*/, int	nPassTime/*所需时间*/ ); 
	// 获取经过时间比
	float getTimePer();
	// 刷新Tick
	virtual int updateTick( unsigned int uCurTime );

	// 开始事件
	virtual void onBeginEvent() = 0;
	// 过程事件
	virtual void onPassEvent() = 0;
	// 结束事件
	virtual void onEndEvent() = 0;
private:
	GameActor_Tick() {};

protected:
	void*		 m_pOwner;			// 所属对象
	unsigned int m_uBeginTime;		// 起始时间
	unsigned int m_uEndTime;		// 结束时间
	unsigned int m_uTime;			// 消耗的时间
	float		 m_fPass;			// 经过的时间百分比
	int			 m_nState;			// Tick状态
};

// 怪物出生
class GameActor_Tick_MonsterBirth : public GameActor_Tick
{
public:
	GameActor_Tick_MonsterBirth( void* pOwner, unsigned int uStartTime/*开始时间*/, int	nPassTime/*所需时间*/ )
		: GameActor_Tick( pOwner, uStartTime, nPassTime )
	{
		onBeginEvent();
	}

	// 开始事件
	virtual void onBeginEvent();
	// 过程事件
	virtual void onPassEvent() {};
	// 结束事件
	virtual void onEndEvent();
};

// 临时武器使用延迟
class GameActor_Tick_TmpEquip : public GameActor_Tick
{
public:
	GameActor_Tick_TmpEquip( void* pOwner, unsigned int uStartTime/*开始时间*/, int	nPassTime/*所需时间*/ )
		: GameActor_Tick( pOwner, uStartTime, nPassTime )
	{
	}

	// 开始事件
	virtual void onBeginEvent();
	// 过程事件
	virtual void onPassEvent() {};
	// 结束事件
	virtual void onEndEvent();
};

// 体型放大
class GameActor_Tick_ChgSize : public GameActor_Tick
{
protected:
	float			m_scale;
	float			m_lastScale;
public:
	GameActor_Tick_ChgSize(void* pOwner, unsigned int uStartTime/*开始时间*/, int	scale/*缩放比例*/, int passTime)
		: GameActor_Tick( pOwner, uStartTime, passTime)
	{
		m_scale = scale / 100.0f;
		m_lastScale = 1;
	}

	// 开始事件
	virtual void onBeginEvent(){}
	// 过程事件
	virtual void onPassEvent();
	// 结束事件
	virtual void onEndEvent();
};

// 死亡延迟
class GameActor_Tick_OnDead : public GameActor_Tick
{
public:
	GameActor_Tick_OnDead( void* pOwner, unsigned int uStartTime/*开始时间*/, int	nPassTime/*所需时间*/ )
		: GameActor_Tick( pOwner, uStartTime, nPassTime )
	{
	}

	// 开始事件
	virtual void onBeginEvent();
	// 过程事件
	virtual void onPassEvent();
	// 结束事件
	virtual void onEndEvent();
};

// 消失前闪烁
class GameActor_Tick_OnWink : public GameActor_Tick
{
public:
	GameActor_Tick_OnWink( void* pOwner, unsigned int uStartTime, int nPassTime, unsigned int uTotalLifeTime )
		: GameActor_Tick( pOwner, uStartTime, nPassTime ), m_uWinkTime( nPassTime ), m_uLifeTime( uTotalLifeTime )
	{
	}
	
	// 开始事件
	virtual void onBeginEvent();
	// 过程事件
	virtual void onPassEvent();
	// 结束事件
	virtual void onEndEvent();

private:
	unsigned int	m_uWinkTime;		// 闪烁的时间
	unsigned int	m_uLifeTime;		// 总生命时间

	struct tagKeepState
	{
		tagKeepState() : m_uKeepStartTime( 0 ), m_bFadeOut( true )
		{}

		unsigned int	m_uKeepStartTime;	// 保持某个状态的持续时间
		bool			m_bFadeOut;			// 是否是淡出
	};

	tagKeepState	m_keepState;
};

// 进入战斗状态
class GameActor_Tick_InFightTime : public GameActor_Tick
{
public:
	GameActor_Tick_InFightTime( void* pOwner, unsigned int uStartTime/*开始时间*/, int	nPassTime/*所需时间*/ )
		: GameActor_Tick( pOwner, uStartTime, nPassTime )
	{
	}

	// 开始事件
	virtual void onBeginEvent();
	// 过程事件
	virtual void onPassEvent() {};
	// 结束事件
	virtual void onEndEvent();
};

class GameActor_Tick_UnfixTmpArm : public GameActor_Tick
{
public:
	GameActor_Tick_UnfixTmpArm(void *pOwner) : GameActor_Tick(pOwner)
	{
		m_nTmpDelayTime = 0;
	}

	GameActor_Tick_UnfixTmpArm( void* pOwner, unsigned int uStartTime/*开始时间*/, int	nPassTime/*所需时间*/ )
		: GameActor_Tick( pOwner, uStartTime, nPassTime )
	{
		m_nTmpDelayTime = 0;
	}

	// 开始事件
	virtual void onBeginEvent(){};
	// 过程事件
	virtual void onPassEvent();
	// 结束事件
	virtual void onEndEvent();

private:
	int	m_nTmpDelayTime;	// 
};

class GameActor_Tick_ShowArm : public GameActor_Tick
{
public:
	GameActor_Tick_ShowArm(void *pOwner) : GameActor_Tick(pOwner)
	{
	}
	GameActor_Tick_ShowArm( void* pOwner, unsigned int uStartTime/*开始时间*/, int	nPassTime/*所需时间*/ )
		: GameActor_Tick( pOwner, uStartTime, nPassTime )
	{
	}
	// 开始事件
	virtual void onBeginEvent(){};
	// 过程事件
	virtual void onPassEvent(){};
	// 结束事件
	virtual void onEndEvent();
};

// 主角休闲动作播放
class GameActor_Tick_MainPlayerFree : public GameActor_Tick
{
public:
	GameActor_Tick_MainPlayerFree( void* pOwner, unsigned int uStartTime/*开始时间*/, int nPassTime/*所需时间*/ )
		: GameActor_Tick( pOwner, uStartTime, nPassTime )
	{
	}
	// 开始事件
	virtual void onBeginEvent();
	// 过程事件
	virtual void onPassEvent() ;
	// 结束事件
	virtual void onEndEvent() {};
};
