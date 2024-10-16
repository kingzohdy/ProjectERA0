#pragma once


#include "EditEventHandler.h"


namespace TE
{

class MEditAreaProcess
	:public IEventProcess
{
public:
	MEditAreaProcess(void);
	~MEditAreaProcess(void);

	virtual bool Process( MTerrainEdit* pEditor , ActionEvent *event );

protected:
	bool  AddAreaPos(  MVec3 &dir , MVec3 &pos  );
    bool  MoveAreaPos( MVec3 &dir , MVec3 &pos );
	void  DelAreaCurPos();
};

};