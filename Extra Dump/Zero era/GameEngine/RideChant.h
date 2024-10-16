#pragma once

#include "IChant_Base.h"

#define RIDE_CHANT_TIME	3000	// ×øÆïÒ÷³ªÊ±¼ä

class RideChant : public IChant_Base
{
public:
	RideChant( GameActor* pOwner, unsigned int uStartTime,unsigned int rideId, int nLevel );
	virtual ~RideChant();
	virtual bool Update();
	virtual bool SendFinishRequest();
	virtual bool OnStart();
	virtual bool RequestBreak();
	virtual void OnBreak();

protected:
private:
	unsigned int	m_uRideID;
	int				m_nRideLv;
};