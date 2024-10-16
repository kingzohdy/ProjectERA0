
#pragma once


#include "mmoveobjectprocess.h"

namespace TE
{

class MAddMoreObjectsProcess
		 :public MMoveObjectProcess
{
public:
	MAddMoreObjectsProcess(void);
	virtual  ~MAddMoreObjectsProcess(void);

	virtual bool Process( MTerrainEdit* pEditor , ActionEvent *event );

protected:
	void  RefreshMoreModel( MTerrainEdit* pEditor ,MVec3 &dir,MVec3 &pos,int mode = 0);
};

};