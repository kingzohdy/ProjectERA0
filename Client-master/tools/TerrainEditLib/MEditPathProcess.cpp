#include ".\meditpathprocess.h"

#include "EditorCamera.h"
#include "mterrain.h"

using namespace TE;

MEditPathProcess::MEditPathProcess(void)
{
	m_nProcessType = MODIFY_PATH ;
}

MEditPathProcess::~MEditPathProcess(void)
{
}

void MEditPathProcess::UpdateData( MTerrainEdit* pEditor )
{
	pEditor->GetPathSet()->UpdateSelectPathPos();
};

bool MEditPathProcess::Process( MTerrainEdit* pEditor , ActionEvent *event )
{
	switch( event->mouseEven )
	{
	case MouseLDown:
		AddPathPos( pEditor , event->dir , event->pos );
		break;
	case MouseMove:
		if(event->bMouseDown )
			MovePathPos( pEditor , event->dir , event->pos );
		break; 
	case MouseSewheel:
		{
			if( pEditor->m_EditOption.nCurCameraTpye == 0)
			{	
				g_Camera.WalkTerrain(event->mz*5.0f,&g_Terrain,2);		
			}
			else
			{
				g_Camera.WalkZ ( event->mz*5.0f );
			}
		}
		break;
		
	case MouseRDown :			
		{
			pEditor->m_bOpenModelInfoDlg = true;
		}
		break;
	}
	return true;
};


bool MEditPathProcess::TestPathModel(  MTerrainEdit* pEditor ,MVec3 &dir , MVec3 &pos  )
{
	return pEditor->GetPathSet()->TestPathModel( pEditor , dir , pos );
}

bool MEditPathProcess::AddPathPos(   MTerrainEdit* pEditor ,MVec3 &dir , MVec3 &pos  )
{
 	MVec3  pos0;

	if( pEditor->GetPathSet ()->TestPathModel( pEditor , dir , pos ) )
	{
		return true;
	}
	
	
	if( g_Terrain.TestAndRay (dir,pos,pos0) )
	{
		pos0.y += 20.0f;
		pEditor->GetPathSet()->AddCurPathPos( pos0 );
		
		return true;
	}
	
	return false;
};

bool MEditPathProcess::MovePathPos(  MTerrainEdit* pEditor , MVec3 &dir , MVec3 &pos )
{
 	MVec3  pos0;

	if( g_Terrain.TestAndRay (dir,pos,pos0))
	{
		pos0.y += 2.0f;
		pEditor->GetPathSet()->MoveCurPos ( pos0 );
	   	
		return true;
	}
	return false;
};