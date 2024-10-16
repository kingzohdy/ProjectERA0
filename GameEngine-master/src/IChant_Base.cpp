#include "IChant_Base.h"
#include <Windows.h>

IChant_Base::IChant_Base( GameActor* pOwner, unsigned int uStartTime,unsigned int uChantTime )
	{
		m_pOwner = pOwner;
		m_uStartTime = uStartTime;
		m_uChantTime = uChantTime;
		m_bRequest = false;
	}


IChant_Base::~IChant_Base()
{

}

bool IChant_Base::OnUpdate()
{
	unsigned int uCurTick = timeGetTime();
	if( !m_bRequest && uCurTick > m_uStartTime + m_uChantTime )
	{
		return SendFinishRequest();
	}else{
		return Update();
	}
}

bool IChant_Base::OnFinish()
{
	return true;
}