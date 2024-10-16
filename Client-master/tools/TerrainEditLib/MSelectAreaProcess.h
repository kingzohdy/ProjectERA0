
#pragma once


#include "EditEventHandler.h"


namespace TE
{

class MSelectAreaProcess
	:public IEventProcess
{
public:
	MSelectAreaProcess(void);
	~MSelectAreaProcess(void);


	virtual bool Process( MTerrainEdit* pEditor , ActionEvent *event );

protected:
	bool    SelectArea( MVec3 &dir , MVec3 &pos);
};

};