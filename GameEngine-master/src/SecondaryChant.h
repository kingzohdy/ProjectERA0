#pragma once

#include "IChant_Base.h"

class SecondaryChant : public IChant_Base
{
public:
	SecondaryChant( GameActor* pOwner, unsigned int uStartTime, unsigned int uChantTime, int nType, int nId );
	virtual ~SecondaryChant();
	virtual bool Update();
	virtual bool SendFinishRequest();
	virtual bool OnStart();
	virtual bool RequestBreak();
	virtual void OnBreak();

private:
	int					m_nType;
	int					m_nId;
};