#pragma once
#include "OgreMemoryDelegation.h"
#include <stdlib.h>

const int LOOP_TICK = -1;

class GameActor_Tick : public Ogre::MemoryDelegation
{
public:
	GameActor_Tick( void* pOwner );
	GameActor_Tick( void* pOwner, unsigned int uStartTime/*��ʼʱ��*/, int	nPassTime/*����ʱ��*/ );
	virtual ~GameActor_Tick();
public:	
	enum
	{
		TICK_BEGIN,		// ��ʼ
		TICK_PROC,		// ����
		TICK_END,		// ����
	};

	// ������ʼʱ��
	void setBeginTime( unsigned int uStartTime/*��ʼʱ��*/, int	nPassTime/*����ʱ��*/ );
	// ����ʱ��
	void setTime( unsigned int uStartTime/*��ʼʱ��*/, int	nPassTime/*����ʱ��*/ ); 
	// ��ȡ����ʱ���
	float getTimePer();
	// ˢ��Tick
	virtual int updateTick( unsigned int uCurTime );

	// ��ʼ�¼�
	virtual void onBeginEvent() = 0;
	// �����¼�
	virtual void onPassEvent() = 0;
	// �����¼�
	virtual void onEndEvent() = 0;
private:
	GameActor_Tick() {};

protected:
	void*		 m_pOwner;			// ��������
	unsigned int m_uBeginTime;		// ��ʼʱ��
	unsigned int m_uEndTime;		// ����ʱ��
	unsigned int m_uTime;			// ���ĵ�ʱ��
	float		 m_fPass;			// ������ʱ��ٷֱ�
	int			 m_nState;			// Tick״̬
};

// �������
class GameActor_Tick_MonsterBirth : public GameActor_Tick
{
public:
	GameActor_Tick_MonsterBirth( void* pOwner, unsigned int uStartTime/*��ʼʱ��*/, int	nPassTime/*����ʱ��*/ )
		: GameActor_Tick( pOwner, uStartTime, nPassTime )
	{
		onBeginEvent();
	}

	// ��ʼ�¼�
	virtual void onBeginEvent();
	// �����¼�
	virtual void onPassEvent() {};
	// �����¼�
	virtual void onEndEvent();
};

// ��ʱ����ʹ���ӳ�
class GameActor_Tick_TmpEquip : public GameActor_Tick
{
public:
	GameActor_Tick_TmpEquip( void* pOwner, unsigned int uStartTime/*��ʼʱ��*/, int	nPassTime/*����ʱ��*/ )
		: GameActor_Tick( pOwner, uStartTime, nPassTime )
	{
	}

	// ��ʼ�¼�
	virtual void onBeginEvent();
	// �����¼�
	virtual void onPassEvent() {};
	// �����¼�
	virtual void onEndEvent();
};

// ���ͷŴ�
class GameActor_Tick_ChgSize : public GameActor_Tick
{
protected:
	float			m_scale;
	float			m_lastScale;
public:
	GameActor_Tick_ChgSize(void* pOwner, unsigned int uStartTime/*��ʼʱ��*/, int	scale/*���ű���*/, int passTime)
		: GameActor_Tick( pOwner, uStartTime, passTime)
	{
		m_scale = scale / 100.0f;
		m_lastScale = 1;
	}

	// ��ʼ�¼�
	virtual void onBeginEvent(){}
	// �����¼�
	virtual void onPassEvent();
	// �����¼�
	virtual void onEndEvent();
};

// �����ӳ�
class GameActor_Tick_OnDead : public GameActor_Tick
{
public:
	GameActor_Tick_OnDead( void* pOwner, unsigned int uStartTime/*��ʼʱ��*/, int	nPassTime/*����ʱ��*/ )
		: GameActor_Tick( pOwner, uStartTime, nPassTime )
	{
	}

	// ��ʼ�¼�
	virtual void onBeginEvent();
	// �����¼�
	virtual void onPassEvent();
	// �����¼�
	virtual void onEndEvent();
};

// ��ʧǰ��˸
class GameActor_Tick_OnWink : public GameActor_Tick
{
public:
	GameActor_Tick_OnWink( void* pOwner, unsigned int uStartTime, int nPassTime, unsigned int uTotalLifeTime )
		: GameActor_Tick( pOwner, uStartTime, nPassTime ), m_uWinkTime( nPassTime ), m_uLifeTime( uTotalLifeTime )
	{
	}
	
	// ��ʼ�¼�
	virtual void onBeginEvent();
	// �����¼�
	virtual void onPassEvent();
	// �����¼�
	virtual void onEndEvent();

private:
	unsigned int	m_uWinkTime;		// ��˸��ʱ��
	unsigned int	m_uLifeTime;		// ������ʱ��

	struct tagKeepState
	{
		tagKeepState() : m_uKeepStartTime( 0 ), m_bFadeOut( true )
		{}

		unsigned int	m_uKeepStartTime;	// ����ĳ��״̬�ĳ���ʱ��
		bool			m_bFadeOut;			// �Ƿ��ǵ���
	};

	tagKeepState	m_keepState;
};

// ����ս��״̬
class GameActor_Tick_InFightTime : public GameActor_Tick
{
public:
	GameActor_Tick_InFightTime( void* pOwner, unsigned int uStartTime/*��ʼʱ��*/, int	nPassTime/*����ʱ��*/ )
		: GameActor_Tick( pOwner, uStartTime, nPassTime )
	{
	}

	// ��ʼ�¼�
	virtual void onBeginEvent();
	// �����¼�
	virtual void onPassEvent() {};
	// �����¼�
	virtual void onEndEvent();
};

class GameActor_Tick_UnfixTmpArm : public GameActor_Tick
{
public:
	GameActor_Tick_UnfixTmpArm(void *pOwner) : GameActor_Tick(pOwner)
	{
		m_nTmpDelayTime = 0;
	}

	GameActor_Tick_UnfixTmpArm( void* pOwner, unsigned int uStartTime/*��ʼʱ��*/, int	nPassTime/*����ʱ��*/ )
		: GameActor_Tick( pOwner, uStartTime, nPassTime )
	{
		m_nTmpDelayTime = 0;
	}

	// ��ʼ�¼�
	virtual void onBeginEvent(){};
	// �����¼�
	virtual void onPassEvent();
	// �����¼�
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
	GameActor_Tick_ShowArm( void* pOwner, unsigned int uStartTime/*��ʼʱ��*/, int	nPassTime/*����ʱ��*/ )
		: GameActor_Tick( pOwner, uStartTime, nPassTime )
	{
	}
	// ��ʼ�¼�
	virtual void onBeginEvent(){};
	// �����¼�
	virtual void onPassEvent(){};
	// �����¼�
	virtual void onEndEvent();
};

// �������ж�������
class GameActor_Tick_MainPlayerFree : public GameActor_Tick
{
public:
	GameActor_Tick_MainPlayerFree( void* pOwner, unsigned int uStartTime/*��ʼʱ��*/, int nPassTime/*����ʱ��*/ )
		: GameActor_Tick( pOwner, uStartTime, nPassTime )
	{
	}
	// ��ʼ�¼�
	virtual void onBeginEvent();
	// �����¼�
	virtual void onPassEvent() ;
	// �����¼�
	virtual void onEndEvent() {};
};
