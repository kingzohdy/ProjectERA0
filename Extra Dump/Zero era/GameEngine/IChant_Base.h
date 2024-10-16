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

	// 检测是否需要打断
	virtual bool Update() = 0;
	// 发送吟唱完成消息，如果不需要连续吟唱，就直接结束吟唱动作
	virtual bool SendFinishRequest() = 0;
	// 开始吟唱
	virtual bool OnStart() = 0;
	// 请求打断吟唱，返回true 直接结束吟唱动作,false等服务器返回消息再结束吟唱动作
	virtual bool RequestBreak() = 0;
	// 打断吟唱
	virtual void OnBreak() = 0;
	// 完成吟唱 返回true 结束，false 继续下次吟唱
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
	unsigned int	m_uStartTime;	// 开始吟唱
	unsigned int	m_uChantTime;	// 吟唱时间
	eChantType		m_Type;
	GameActor*		m_pOwner;
public:
	bool			m_bRequest;
};