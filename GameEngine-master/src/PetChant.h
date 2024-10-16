#pragma once

#include "IChant_Base.h"

#define PET_CHANT_TIME	5000	// ³èÎïÒ÷³ªÊ±¼ä

class PetChant : public IChant_Base
{
public:
	PetChant( GameActor* pOwner, unsigned int uStartTime, tdr_longlong wid );
	virtual ~PetChant();
	virtual bool Update();
	virtual bool SendFinishRequest();
	virtual bool OnStart();
	virtual bool RequestBreak();
	virtual void OnBreak();

private:
	tdr_longlong	m_Wid;
};