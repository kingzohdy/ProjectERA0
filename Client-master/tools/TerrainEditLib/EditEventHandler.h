
#pragma once
#include "TerrainEdit.h"

namespace TE
{
	class BaseEditHandler
	{
	public:
		BaseEditHandler();
		virtual  ~BaseEditHandler();

		virtual bool  handleEditEvent( TerrainEditMgr*  pEditor , ActionEvent* event ) = 0 ;

		virtual void  Undo( TerrainEditMgr* ){};
		virtual void  Redo( TerrainEditMgr* ){};

		virtual  void SaveData( TerrainEditMgr* ){};
		virtual  void Clear(){};

		int           GetProcessType(){ return m_nProcessType ;}

		virtual  void  UpdateData( TerrainEditMgr*  ){};
		virtual  void  MsgProc( TerrainEditMgr* pEditor, void *hWnd, unsigned int msg, unsigned int wParam, unsigned int lParam){};
		virtual void  Init(TerrainEditMgr*){};
		virtual void  Release(){};
		virtual void Update(TerrainEditMgr* pEditor  , float tick) {}

	protected:
		int                      m_nProcessType;
	};
}