
#pragma once


#include "mmoveobjectprocess.h"


namespace TE
{

class MRoateObjectProcess
	:public MMoveObjectProcess
{
public:
	MRoateObjectProcess(void);
	virtual ~MRoateObjectProcess(void);

	virtual bool Process( MTerrainEdit* pEditor , ActionEvent *event );
protected:
	void rotateSelectModel(MTerrainEdit* pEditor, int dx, int dy, COORDINATE_SELECT axis);
	void  RoateYSelectModel ( MTerrainEdit* pEditor ,int dz );
};

};