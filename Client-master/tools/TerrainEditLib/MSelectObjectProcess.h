
#pragma once


#include "EditEventHandler.h"


namespace TE
{

class MSelectObjectProcess
	:public IEventProcess
{
public:
	MSelectObjectProcess(void);
	virtual ~MSelectObjectProcess(void);

	virtual bool Process( MTerrainEdit* pEditor , ActionEvent *event );
protected:
	bool    AddSelectScreenObject( MTerrainEdit* pEditor ,MVec3 &dir,MVec3 &pos, int mode  );
    bool    SelectRangeObject(  MTerrainEdit* pEditor ,int dx, int dy , int mode = 0);

	bool    DelSelectScreenObject( MTerrainEdit* pEditor , MVec3 &dir,MVec3 &pos, int mode );
    bool    SelectScreenObject( MTerrainEdit* pEditor ,MVec3 &dir,MVec3 &pos, int mode = 0);

};


};
