
#pragma once


#include "TerrainEdit.h"


namespace TE
{

	// 编辑器处理类
	/*
	根据不同的键盘，鼠标操作，产生相应的处理事件交给TerrainEditMgr进行处理。
	*/
	class TerrainEditProc
	{
	public:
		TerrainEditProc(void);
		~TerrainEditProc(void);

		void  MsgProc( TE::TerrainEditMgr* pEditor, void *hWnd, unsigned int msg, unsigned int wParam, unsigned int lParam);
	};

	extern TerrainEditProc   g_TerrainEditMsgProcess;
}