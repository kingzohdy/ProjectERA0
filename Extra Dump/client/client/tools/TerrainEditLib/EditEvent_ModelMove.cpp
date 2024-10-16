#ifdef USE_OLDEDIT

#include "EditEvent_ModelMove.h"
#include "EditorModel.h"
#include "Coordinate.h"
#include "EditorCamera.h"
#include "Terrain.h"
#include "MapMark.h"

#include "OgreModel.h"
#include "OgreRay.h"

using namespace Ogre;

namespace TE
{
	extern  float g_fModelSignRad ;

	ModelMoveEditHandler::ModelMoveEditHandler(void)
	{	
		m_nProcessType = MOVE_OBJECT_EVN;
	}

	ModelMoveEditHandler::~ModelMoveEditHandler(void)
	{
	}

	bool ModelMoveEditHandler::handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event )
	{
		if (  pEditor->m_bLockModelEdit[ pEditor->m_EditOption.mModelType ])
			return true;

		switch( event->mouseEven )
		{
		case MouseLDown:
			{	
				if( !TestCurSelectModel( pEditor , event->dir , event->pos ) && pEditor->GetCoordinate()->getCoordSelect() == NO_AXES_SELECT )
				{
					ModelSelectEditHandler::handleEditEvent( pEditor, event );
					//SelectObjectAction(event);
					return true;
				}

				pEditor->AddSaveUndoData();
				pEditor->SaveCurData(0,0,0);
				if( pEditor->m_EditOption.pSelectModel )
				{
					pEditor->GetCoordinate()->bindObject ( pEditor->m_EditOption.pSelectModel->GetModelData ()->pModel  );
					g_Camera.BindObject ( pEditor->m_EditOption.pSelectModel );
				}; 		
				pEditor->saveCoordinatePos( event->mx , event->my );	
			}
			break;
		case MouseSewheel:
			{		
				g_Camera.WalkTerrain(event->mz*5.0f,&g_Terrain,2);	
			}
			break;
		case MouseMove:
			if( event->bMouseDown )
			{
				if( pEditor->m_select_coordinate == NO_AXES_SELECT )
				{
					MoveSelectModel( pEditor , event->dir , event->pos );
				}
				else
				{
					pEditor->MoveMoveAxes( pEditor->m_select_coordinate , event->mx , event->my , event->pos  , event->dir);
				}
			}
			else
			{
				MoveSelectModel( pEditor ,event->dir , event->pos , 1);	

				pEditor->m_select_coordinate = pEditor->GetCoordinate()->intersect( event->dir , event->pos , 0);
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

	bool   ModelMoveEditHandler::TestCurSelectModel( TerrainEditMgr* pEditor , Ogre::Vector3 &dir , Ogre::Vector3 &pos  )
	{	
		if( !pEditor->GetEditData() )
			return false;

		EditorModel* pModel;
		Ogre::Vector3   _pos;

		Ogre::Ray ray(pos, dir);
		if( pEditor->m_pCurSelectModel)
		{
			if( pEditor->m_pCurSelectModel->GetModelData ()->cType > TYPE_PLANT_MODEL )
				return false;

			pModel =  (EditorModel*)pEditor->m_pCurSelectModel->GetModelData ()->pModel ;

			if(pModel->GetShowModel()->intersectRay(INTERSECT_PICK, ray, NULL))
			{
				return true;
			}

			for( size_t i = 0 ; i < pEditor->m_CurSelectModelSet.size () ; i ++ )
			{
				if( pEditor->m_CurSelectModelSet[i]->GetModelData ()->cType> TYPE_PLANT_MODEL )
					return false;

				pModel =  (EditorModel*)pEditor->m_CurSelectModelSet[i]->GetModelData ()->pModel ;
				if(pModel->GetShowModel()->intersectRay(INTERSECT_PICK, ray, NULL)) return true;
			}

			return false;;
		}

		return false;
	}

	void  ModelMoveEditHandler::Undo( TerrainEditMgr* pEditor )
	{
		if( pEditor->m_SaveData.empty ())
			return ;

		if ( pEditor->m_nCurSaveDataIndex < 0 )
		{
			pEditor->m_nCurSaveDataIndex = 0;	
		} 
		else if ( pEditor->m_nCurSaveDataIndex >= (int)pEditor->m_SaveData.size ())
		{
			pEditor->m_nCurSaveDataIndex = pEditor->m_SaveData.size () - 1;	
		}

		for( unsigned int i = 0 ; i < pEditor->m_SaveData[pEditor->m_nCurSaveDataIndex].m_SaveSet .size (); i++)
		{
			UndoSaveData *pData = pEditor->m_SaveData[pEditor->m_nCurSaveDataIndex].m_SaveSet[i] ;

			for( size_t i = 0 ; i < pData->SaveSelectModelSet .size () ; i++ )	
			{
				pEditor->m_CurSelectModelSet[i]->GetModelData ()->pos = pData->SaveSelectModelSet[i].savePos ;
				pEditor->m_CurSelectModelSet[i]->GetModelData ()->dir = pData->SaveSelectModelSet[i].saveDir ;
				pEditor->m_CurSelectModelSet[i]->GetModelData ()->scale = pData->SaveSelectModelSet[i].saveScale ;
				pEditor->m_CurSelectModelSet[i]->ResetMat ();
			}

			pData->SaveSelectModelSet .clear () ;

			if( pEditor->m_pCurSelectModel != NULL )
			{					
				if( pData->pSaveModel )			
				{
					pEditor->m_pCurSelectModel = pData->pSaveModel ;

					pEditor->m_pCurSelectModel->GetModelData ()->pos = pData->savePos ;
					pEditor->m_pCurSelectModel->GetModelData ()->dir  = pData->saveDir ;
					pEditor->m_pCurSelectModel->GetModelData ()->scale  = pData->saveScale ;

					pEditor->m_pCurSelectModel->ResetMat ();
					pData->pSaveModel = NULL;
				}	
			}
		}

		pEditor->m_nCurSaveDataIndex --;
	};

	void  ModelMoveEditHandler::MoveSelectModel(  TerrainEditMgr* pEditor ,Ogre::Vector3 &dir,Ogre::Vector3 &pos,int mode)
	{
		Ogre::Vector3  pos0;

		if(pEditor->m_pSignSet)
		{
			pEditor->m_pSignSet->m_PosSet .clear ();
		}

		if(g_Terrain.TestAndRay(dir,pos,pos0))
		{
			pEditor->m_CurMousePos = pos0 ;
			if(pEditor->m_pSignSet)
			{
				pEditor->m_pSignSet->UpdateData ( pos0, g_fModelSignRad , 16 );
			}	
		}

		if( mode == 1 )
			return ;

		if( pEditor->m_pCurSelectModel == NULL)		
			return;

		Ogre::Vector3   dPos = pos0 - pEditor->m_pCurSelectModel->GetModelData ()->pos ;
		pEditor->m_pCurSelectModel->GetModelData ()->pos=pos0;
		pEditor->m_pCurSelectModel->ResetMat();

		for( size_t i = 0 ; i < pEditor->m_CurSelectModelSet.size(); i ++)
		{
			pEditor->m_CurSelectModelSet[i]->GetModelData()->pos += dPos;
			pEditor->m_CurSelectModelSet[i]->ResetMat();
		}
	}
}

#endif