#pragma once


#include "mmoveobjectprocess.h"

namespace TE
{

class MLightEditProcess
	: public MMoveObjectProcess
{
public:
	MLightEditProcess(void);
	virtual ~MLightEditProcess(void);

	virtual bool Process( MTerrainEdit* pEditor , ActionEvent *event );

	virtual void  Undo( MTerrainEdit* pEditor );

private:
	bool SelectScreenLight( MTerrainEdit* pEditor ,MVec3 &dir,MVec3 &pos, int mode );	
	void  AddLight( MTerrainEdit* pEditor ,MVec3 &pos , MVec3 &dir);

private:
	//当前选择灯光节点的索引
	int                               m_nSelectLightNodeInfoIdx ;
};


}