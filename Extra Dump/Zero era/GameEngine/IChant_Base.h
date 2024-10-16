#pragma once

#include "GameEngineTypes.h"
#include "GameActor.h"
#include "ShowActor.h"
#include "GameMainPlayer.h"
#include "OgreScriptLuaVM.h"
#include "GameMsgPrint.h"
#include "ServerTimer.h"
#include "GameMsgPrint.h"

class IChant_Base
{
public:
	IChant_Base( GameActor* pOwner, unsigned int uStartTime,unsigned int uChantTime );
	virtual ~IChant_Base();

	bool OnUpdate();

	// ����Ƿ���Ҫ���
	virtual bool Update() = 0;
	// �������������Ϣ���������Ҫ������������ֱ�ӽ�����������
	virtual bool SendFinishRequest() = 0;
	// ��ʼ����
	virtual bool OnStart() = 0;
	// ����������������true ֱ�ӽ�����������,false�ȷ�����������Ϣ�ٽ�����������
	virtual bool RequestBreak() = 0;
	// �������
	virtual void OnBreak() = 0;
	// ������� ����true ������false �����´�����
	virtual bool OnFinish() ;
	
	void SetType( eChantType type )
	{
		m_Type = type;
	}
	eChantType GetType()
	{
		return m_Type;
	}
	GameActor* GetOwner()
	{
		return m_pOwner;
	}
	unsigned int GetStartTime()
	{
		return m_uStartTime;
	}
	unsigned int GetChantTime()
	{
		return m_uChantTime;
	}

private:
	unsigned int	m_uStartTime;	// ��ʼ����
	unsigned int	m_uChantTime;	// ����ʱ��
	eChantType		m_Type;
	GameActor*		m_pOwner;
public:
	bool			m_bRequest;
};