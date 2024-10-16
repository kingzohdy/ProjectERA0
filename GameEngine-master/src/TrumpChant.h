#pragma once

#include "IChant_Base.h"

#define TRUMP_CHANT_TIME	5000	// ¾«ÁéÒ÷³ªÊ±¼ä

class TrumpChant : public IChant_Base
{
public:
	TrumpChant( GameActor* pOwner, unsigned int uStartTime, int nHeighId, int nLowId, int nGridIdx );
	virtual ~TrumpChant();
	virtual bool Update();
	virtual bool SendFinishRequest();
	virtual bool OnStart();
	virtual bool RequestBreak();
	virtual void OnBreak();

private:
	int m_nHeighId;
	int m_nLowId;
	int m_nGridIdx;
};