#pragma once

#include "IChant_Base.h"

class UseItemChant : public IChant_Base
{
public:
	UseItemChant( GameActor* pOwner, unsigned int uStartTime, unsigned int uChantTime, int nItemID, int TargetID = 0, int ListType = -1, int GridIdx = -1 );
	virtual ~UseItemChant();
	virtual bool Update();
	virtual bool SendFinishRequest();
	virtual bool OnStart();
	virtual bool RequestBreak();
	virtual void OnBreak();
protected:
private:
	int		m_nItemID;
	int		m_nListType;
	int		m_nGridIdx;
	int		m_nTargetID;
};