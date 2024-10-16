#pragma once

#include "IChant_Base.h"

#define RESOLVE_CHANT_TIME	1000	// �ֽ�����ʱ��

class ResolveChant : public IChant_Base
{
public:
	ResolveChant( GameActor* pOwner, unsigned int uStartTime, int nListType, int nGridIdx, unsigned long long Wid, int nType );
	virtual ~ResolveChant();
	virtual bool Update();
	virtual bool SendFinishRequest();
	virtual bool OnStart();
	virtual bool RequestBreak();
	virtual void OnBreak();

private:
	int					m_nListType;
	int					m_nGridIdx;
	unsigned long long	m_Wid;
	int					m_nType;//0����װ���ֽ⣬1������ʯ�ֽ�
};