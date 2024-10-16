
#include ".\mroateobjectprocess.h"
#include "Coordinate.h"
#include "EditorCamera.h"
#include "MTerrain.h"
#include "TerrainTile.h"

using namespace TE;

MRoateObjectProcess::MRoateObjectProcess(void)
{
	m_nProcessType = ROATE_OBJECT_EVN;
}

MRoateObjectProcess::~MRoateObjectProcess(void)
{
}

bool MRoateObjectProcess::Process( MTerrainEdit* pEditor , ActionEvent *event )
{
	if (  pEditor->m_bLockModelEdit[ pEditor->m_EditOption.mModelType ] )
		return true;

	static int _mdx,_mdy;
	switch( event->mouseEven )
	{		
	case MouseLDown:
		{	
			_mdx = event->mx ;
			_mdy = event->my ;
			 pEditor->AddSaveUndoData();	
			 pEditor->SaveCurData(0,0,0);
		}
		break;
	case MouseMove:
		{
			if( event->bMouseDown )
			{
				rotateSelectModel(pEditor, event->mx-_mdx, event->my-_mdy, pEditor->GetCoordinate()->getCoordSelect());
				_mdx = event->mx;
				_mdy = event->my;
			}
			else
			{
				MoveSelectModel( pEditor , event->dir , event->pos , 1);						                    
				pEditor->m_select_coordinate = pEditor->GetCoordinate()->intersect( event->dir , event->pos , 1);
			}
		}
		break;
	case MouseSewheel:
		{		
			g_Camera.WalkTerrain(event->mz*5.0f,&g_Terrain,2);	
		}
		break;

	case MouseRDown :			
		{
			pEditor->m_bOpenModelInfoDlg = true;
		}
	}
	return true;

};

void MRoateObjectProcess::rotateSelectModel(MTerrainEdit* pEditor, int dx, int dy, COORDINATE_SELECT axis)
{
	if( pEditor->m_pCurSelectModel == NULL)
		return;

	float scale = 0.2f;
	switch(axis)
	{
	case SELECT_X_AXES:	
		pEditor->m_pCurSelectModel->GetModelData()->dir.y -= dy*scale;
		break;
	case SELECT_Y_AXES:	
		pEditor->m_pCurSelectModel->GetModelData()->dir.x += dx*scale;
		break;
	case SELECT_Z_AXES:
		pEditor->m_pCurSelectModel->GetModelData()->dir.z += dx*scale;
		break;
	}

	pEditor->m_pCurSelectModel->ResetMat ();

	for( size_t i = 0 ; i < pEditor->m_CurSelectModelSet.size () ; i++)
	{ 
		switch( pEditor->GetCoordinate()->getCoordSelect())
		{
		case SELECT_X_AXES :	
			pEditor->m_CurSelectModelSet[i]->GetModelData ()->dir.y += dy*scale;
			break;
		case SELECT_Y_AXES :
			pEditor->m_CurSelectModelSet[i]->GetModelData ()->dir.x += dx*scale;
			break;
		case SELECT_Z_AXES:
			pEditor->m_CurSelectModelSet[i]->GetModelData ()->dir.z += dx*scale;
			break;
		}

		pEditor->m_CurSelectModelSet[i]->ResetMat();
	}
}

void  MRoateObjectProcess::RoateYSelectModel ( MTerrainEdit* pEditor ,int dz )
{
    if( pEditor->m_pCurSelectModel == NULL)
		return;

	float roate = dz*0.01f;
	switch( pEditor->GetCoordinate()->getCoordSelect())
	{
	case SELECT_X_AXES :	
		pEditor->m_pCurSelectModel->GetModelData ()->dir .y += roate;
		break;
	case SELECT_Y_AXES :	
	case NO_AXES_SELECT :
		pEditor->m_pCurSelectModel->GetModelData ()->dir .x += roate;
		break;
	case SELECT_Z_AXES:
		pEditor->m_pCurSelectModel->GetModelData ()->dir .z += roate;
		break;
	}

	pEditor->m_pCurSelectModel->ResetMat ();

	for( size_t i = 0 ; i < pEditor->m_CurSelectModelSet.size () ; i++)
	{ 
		switch( pEditor->GetCoordinate()->getCoordSelect())
		{
		case NO_AXES_SELECT :
			break;
		case SELECT_X_AXES :	
			pEditor->m_CurSelectModelSet[i]->GetModelData ()->dir .y += roate;
			break;
		case SELECT_Y_AXES :
			pEditor->m_CurSelectModelSet[i]->GetModelData ()->dir .x += roate;
			break;
		case SELECT_Z_AXES:
			pEditor->m_CurSelectModelSet[i]->GetModelData ()->dir .z += roate;
			break;
		}

		pEditor->m_CurSelectModelSet[i]->ResetMat();
	}

}