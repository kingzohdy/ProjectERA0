
#include ".\mscaleobjectprocess.h"
#include "Coordinate.h"
#include "EditorCamera.h"
#include "MTerrain.h"

using namespace TE;

MScaleObjectProcess::MScaleObjectProcess(void)
{	
	m_nProcessType = SCALE_OBJECT_EVN;
}

MScaleObjectProcess::~MScaleObjectProcess(void)
{
}


bool MScaleObjectProcess::Process( MTerrainEdit* pEditor , ActionEvent *event )
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
				ZoomSelectModel( pEditor  ,100*( (event->mx - _mdx)/6 ) );                              
				_mdx = event->mx;
			}
			else
			{
				MoveSelectModel( pEditor ,event->dir , event->pos , 1);						                    
				pEditor->m_select_coordinate = pEditor->GetCoordinate()->intersect( event->dir , event->pos , 0);
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
}

void  MScaleObjectProcess::ZoomSelectModel( MTerrainEdit* pEditor ,int  dz)
{
	if( pEditor->m_pCurSelectModel == NULL)
		return;

	//暂时不提供X，Y，Z缩放
    float fzoom = dz*0.0001f;

    pEditor->m_pCurSelectModel->GetModelData ()->scale += fzoom;
	if(pEditor->m_pCurSelectModel->GetModelData ()->scale <= 0)
         pEditor->m_pCurSelectModel->GetModelData ()->scale = 0.00004f;

    pEditor->m_pCurSelectModel->ResetMat();

	for( size_t i = 0 ; i < pEditor->m_CurSelectModelSet.size () ; i++)
	{ 
		pEditor->m_CurSelectModelSet[i]->GetModelData ()->scale += fzoom;
	
		if(pEditor->m_CurSelectModelSet[i]->GetModelData ()->scale <= 0)        
			pEditor->m_CurSelectModelSet[i]->GetModelData ()->scale = 0.00004f;

		pEditor->m_CurSelectModelSet[i]->ResetMat();
	}
};	