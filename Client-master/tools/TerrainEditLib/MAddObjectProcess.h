
#pragma once


#include "mmoveobjectprocess.h"

namespace TE
{

class MAddObjectProcess
	 :public MMoveObjectProcess
{
public:
	MAddObjectProcess(void);
	virtual ~MAddObjectProcess(void);

	virtual bool Process( MTerrainEdit* pEditor , ActionEvent *event );

public:
	void    AddObjectTitle( MTerrainEdit* pEditor );

	void    AddPlantModelToTitle( MTerrainEdit* pEditor,const char* filename,MVec3 pos,MVec3 dir,float fscale);
};


};