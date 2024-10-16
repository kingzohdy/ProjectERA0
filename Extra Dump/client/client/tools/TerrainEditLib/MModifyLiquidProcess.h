#pragma once


#include "mmoveobjectprocess.h"

namespace TE
{
class MModifyLiquidProcess
	 :public MMoveObjectProcess
{
public:
	MModifyLiquidProcess(void);
	virtual ~MModifyLiquidProcess(void);

	virtual bool Process( MTerrainEdit* pEditor , ActionEvent *event );

protected:
	bool  AddLiquid( MTerrainEdit* pEditor , MVec3 &dir, MVec3 &pos );
};

};