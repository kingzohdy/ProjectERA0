
#pragma once

#include "mterrainedit.h"

namespace TE
{

class IEventProcess
{
public:
	IEventProcess();
	virtual  ~IEventProcess();
     
	virtual bool  Process( MTerrainEdit*  pEditor , ActionEvent* event ) = 0 ;

	virtual void  Undo( MTerrainEdit* ){};
	virtual void  Redo( MTerrainEdit* ){};

	virtual  void SaveData( MTerrainEdit* ){};
	virtual  void Clear(){};

	int           GetProcessType(){ return m_nProcessType ;}

	virtual  void  UpdateData( MTerrainEdit*  ){};
	virtual  void  MsgProc( MTerrainEdit* pEditor, void *hWnd, unsigned int msg, unsigned int wParam, unsigned int lParam){};
	virtual void  Init(MTerrainEdit*){};
	virtual void  Release(){};
    virtual void Update(MTerrainEdit* pEditor  , float tick) {}

protected:
	int                      m_nProcessType;


};

};