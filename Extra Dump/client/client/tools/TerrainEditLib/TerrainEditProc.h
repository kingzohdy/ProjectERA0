
#pragma once


#include "TerrainEdit.h"


namespace TE
{

	// �༭��������
	/*
	���ݲ�ͬ�ļ��̣���������������Ӧ�Ĵ����¼�����TerrainEditMgr���д���
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