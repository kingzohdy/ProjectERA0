#pragma once


#include "mselectobjectprocess.h"


namespace TE
{

class MMoveObjectProcess
	:public MSelectObjectProcess
{
public:
	MMoveObjectProcess(void);
	virtual ~MMoveObjectProcess(void);
	
	virtual bool Process( MTerrainEdit* pEditor , ActionEvent *event );

	virtual void  Undo( MTerrainEdit* pEditor );

protected:
	bool   TestCurSelectModel( MTerrainEdit* pEditor , MVec3 &dir , MVec3 &pos  );
    void   MoveSelectModel(  MTerrainEdit* pEditor ,MVec3 &dir,MVec3 &pos,int mode = 0);
};

};