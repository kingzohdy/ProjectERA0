#pragma once


#include "EditEventHandler.h"
#include "mmoveobjectprocess.h"

namespace TE
{

class MEditPathProcess
		:public MMoveObjectProcess
{
public:
	MEditPathProcess(void);
	virtual ~MEditPathProcess(void);

	virtual bool Process( MTerrainEdit* pEditor , ActionEvent *event );
	virtual void UpdateData( MTerrainEdit* pEditor );

protected:
	bool AddPathPos(  MTerrainEdit* pEditor , MVec3 &dir , MVec3 &pos  );
	bool MovePathPos( MTerrainEdit* pEditor ,  MVec3 &dir , MVec3 &pos );

	bool TestPathModel( MTerrainEdit* pEditor ,  MVec3 &dir , MVec3 &pos );
};

};
