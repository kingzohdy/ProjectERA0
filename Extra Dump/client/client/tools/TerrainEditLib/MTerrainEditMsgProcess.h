
#pragma once


#include "mterrainedit.h"


namespace TE
{

class MTerrainEditMsgProcess
{
public:
	MTerrainEditMsgProcess(void);
	~MTerrainEditMsgProcess(void);

     void  MsgProc( TE::MTerrainEdit* pEditor, void *hWnd, unsigned int msg, unsigned int wParam, unsigned int lParam);
};

};


extern TE::MTerrainEditMsgProcess   g_TerrainEditMsgProcess;