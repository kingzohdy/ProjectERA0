
#pragma once


#include "mmoveobjectprocess.h"

namespace TE
{

class MScaleObjectProcess
		:public MMoveObjectProcess
{
public:
	MScaleObjectProcess(void);
	virtual ~MScaleObjectProcess(void);

	virtual bool Process( MTerrainEdit* pEditor , ActionEvent *event );

protected:
	void  ZoomSelectModel( MTerrainEdit* pEditor , int  dz);

};

};
