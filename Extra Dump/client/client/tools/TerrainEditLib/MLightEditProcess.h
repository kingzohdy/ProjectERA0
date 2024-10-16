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
	//��ǰѡ��ƹ�ڵ������
	int                               m_nSelectLightNodeInfoIdx ;
};


}