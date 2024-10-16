
#pragma once


#include "mmoveobjectprocess.h"

namespace TE
{

class MAddNpcProcess
	:public MMoveObjectProcess
{
public:
	MAddNpcProcess(void);
	~MAddNpcProcess(void);

	virtual bool Process( MTerrainEdit* pEditor , ActionEvent *event );

protected:
	bool   SelectScreenNPC( MTerrainEdit* pEditor , MVec3 &dir,MVec3 &pos, int mode);
	void   AddNpc( MTerrainEdit* pEditor ,MVec3 &pos , MVec3 &dir);

public:
	void    AddNPC( MTerrainEdit* pEditor ,const char* npcName , const char* npcModelPath ,  unsigned int npcID,
	          MVec3 npcPos, MVec3 npcDir ,const char* script );
};

};