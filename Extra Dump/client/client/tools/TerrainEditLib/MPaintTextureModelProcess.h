
#pragma once


#include "EditEventHandler.h"

namespace TE
{


class MPaintTextureModelProcess
			:public IEventProcess
{
public:
	MPaintTextureModelProcess(void);
	~MPaintTextureModelProcess(void);

	virtual bool Process( MTerrainEdit* pEditor , ActionEvent *event );

	virtual void Undo( MTerrainEdit* pEditor );

protected:
	void  SetAutoLinkMeshSign(  MTerrainEdit* pEditor ,MVec3 &dir ,MVec3 &pos);
};


};
