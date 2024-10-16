#pragma once


#include "EditEventHandler.h"

namespace TE
{

class MUnknowProcess
	 :public IEventProcess
{
public:
	MUnknowProcess(void);
	~MUnknowProcess(void);

	virtual bool Process( MTerrainEdit* pEditor , ActionEvent *event );
};

};