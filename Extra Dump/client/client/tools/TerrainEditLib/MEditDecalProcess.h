#pragma once


#include "EditEventHandler.h"

namespace TE
{

class MEditDecalProcess
		:public IEventProcess
{
public:
	MEditDecalProcess(void);
	~MEditDecalProcess(void);

	virtual bool Process( MTerrainEdit* pEditor , ActionEvent *event );

protected:
	bool      AddDecalPos(  MTerrainEdit* pEditor ,MVec3 &dir , MVec3 &pos  );

};

};
