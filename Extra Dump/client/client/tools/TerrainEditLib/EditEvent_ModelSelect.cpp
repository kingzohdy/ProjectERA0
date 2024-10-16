
#ifdef USE_OLDEDIT

#include "EditEvent_ModelSelect.h"
#include "Coordinate.h"
#include "EditorCamera.h"
#include "EditorModel.h"
#include "EditEventHandlerMgr.h"
#include "EditorInput.h"
#include "TerrainCull.h"

#include "OgreModel.h"
#include "OgreCamera.h"
#include "OgreRay.h"

using namespace Ogre;

namespace TE
{
	extern bool   TestIfLinkMesh(const char* modelName );

	Ogre::Vector3  ChangeScenePointToRay( int x , int y )
	{
		Ogre::Matrix4 mat = g_pCamera->getProjectMatrix();

		Ogre::Vector3 v;
		Ogre::Vector3 rayDir;

		v.x =  ( ( ( 2.0f * x ) / g_Camera.m_ScreenWidth  ) - 1 ) / mat._11;  
		v.y = -( ( ( 2.0f * y ) / g_Camera.m_ScreenHeight ) - 1 ) / mat._22;
		v.z =  1.0f;

		Ogre::Matrix4 matWorld,m,matView;
		matWorld.identity ();
		matView = g_pCamera->getViewMatrix();
		m = matWorld * matView;   
		m.inverse ();

		// Transform the screen space pick ray into 3D space    
		rayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;        
		rayDir.y= v.x*m._12 + v.y*m._22 + v.z*m._32;       
		rayDir.z = v.x*m._13 + v.y*m._23 + v.z*m._33;

		return  rayDir;
	}

	bool   TestNormal(Ogre::Vector3 &nor1 , Ogre::Vector3 &nor2,Ogre::Vector3 &nor3 , Ogre::Vector3 &nor4,Ogre::Vector3 testnor)
	{
		float result = DotProduct( nor1,testnor );

		float result1 = DotProduct( nor2 ,testnor );

		if( result < 0 && result1 >0 )
			return false;
		if( result > 0 && result1 < 0 )
			return false;

		result1 = DotProduct( nor3 , testnor );
		if( result < 0 && result1 >0 )
			return false;
		if( result > 0 && result1 < 0 )
			return false;

		result1 = DotProduct( nor4 , testnor );
		if( result < 0 && result1 >0 )
			return false;
		if( result > 0 && result1 < 0 )
			return false;

		return true;

	}

	ModelSelectEditHandler::ModelSelectEditHandler(void)
	{	
		m_nProcessType = SELECT_OBJECT_EVN;
	}

	ModelSelectEditHandler::~ModelSelectEditHandler(void)
	{
	}



	bool ModelSelectEditHandler::handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event )
	{
		if (  pEditor->m_bLockModelEdit[ pEditor->m_EditOption.mModelType ] )
		{
			switch( event->mouseEven )
			{		
			case MouseSewheel:
				{ 
					g_Camera.WalkTerrain(event->mz*5.0f,&g_Terrain,2);		
				}
				break;
			}
			return true;
		}

		switch( event->mouseEven )
		{
		case MouseLDown:
			{	
				if( pEditor->GetCoordinate()->getCoordSelect() != NO_AXES_SELECT )
					break;

				pEditor->m_bMouseMove = false;

				if( g_MInputProcess.GetUserInput()->bLCtrl )
				{
					if( AddSelectScreenObject( pEditor , event->dir ,event->pos ,1) )
					{
						pEditor->GetCoordinate()->bindObject( pEditor->m_EditOption.pSelectModel->GetModelData ()->pModel  );
						g_Camera.BindObject ( pEditor->m_EditOption.pSelectModel );
					}		
					pEditor->saveCoordinatePos( event->mx , event->my );	
				} 
				else if ( g_MInputProcess.GetUserInput()->bLAlt )
				{
					if( DelSelectScreenObject( pEditor , event->dir ,event->pos ,1) )
					{
						pEditor->GetCoordinate()->bindObject( pEditor->m_EditOption.pSelectModel->GetModelData ()->pModel  );
						g_Camera.BindObject ( pEditor->m_EditOption.pSelectModel );
					}		
					pEditor->saveCoordinatePos( event->mx , event->my );
				}
				else
				{
					if( pEditor->m_pCurSelectModel )
					{
						pEditor->m_pCurSelectModel->GetModelData ()->m_pWireBox ->SetColor (  0xffaaaaaa  ); 
					}
					for( size_t i = 0 ; i < pEditor->m_CurSelectModelSet.size () ; i++)
					{
						pEditor->m_CurSelectModelSet[i]->GetModelData ()->m_pWireBox ->SetColor ( 0xffaaaaaa   );  
					}
					pEditor->m_CurSelectModelSet.clear ();
					if( SelectScreenObject( pEditor , event->dir ,event->pos ,1) )
					{
						pEditor->GetCoordinate()->bindObject( pEditor->m_EditOption.pSelectModel->GetModelData ()->pModel  );
						g_Camera.BindObject ( pEditor->m_EditOption.pSelectModel );							

						if( !pEditor->m_bPessQ )
						{
							pEditor->m_EditOption.mCurEditEvent = MOVE_OBJECT_EVN ;
							pEditor->m_EditOption.pEventProcess = g_EventProcessFactory.GetEventProcess( MOVE_OBJECT_EVN );

							//更改事件后，没有鼠标按下状态，所以先保留现在位置
							pEditor->AddSaveUndoData();
							pEditor->SaveCurData(0,0,0);
						}		

						pEditor->saveCoordinatePos( event->mx , event->my );	
					}
					else
					{
						g_Camera.BindObject( NULL );
						g_Camera.m_bBeginMove = false;
						pEditor->m_pCurSelectModel = NULL ;
						pEditor->GetCoordinate()->bindObject( NULL );
					}		
				}
			}
			break;
		case MouseMove:
			pEditor->m_bMouseMove = true;
			pEditor->MoveSign(event->dir ,event->pos );

			if( event->bMouseDown )
			{       
				SelectRangeObject( pEditor , event->mx ,event->my , 1 );
			}
			break;
		case MouseLUp:
			{
				if ( SelectRangeObject( pEditor , event->mx ,event->my ) )
				{
					if( !pEditor->m_bPessQ )
					{
						pEditor->m_EditOption.mCurEditEvent = MOVE_OBJECT_EVN ;
						pEditor->m_EditOption.pEventProcess = g_EventProcessFactory.GetEventProcess( MOVE_OBJECT_EVN );
					}
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
			break;
		}
		return true;
	};


	bool   ModelSelectEditHandler::AddSelectScreenObject( TerrainEditMgr* pEditor ,Ogre::Vector3 &dir,Ogre::Vector3 &pos, int mode )
	{	
		if( !pEditor->GetEditData() )
			return false;

		EditorModel* pModel;
		Ray  ray(pos, dir);

		TerrainCull *pcull = pEditor->m_Cull;
		for( unsigned int i=0; i<pcull->m_ShowModelNum ; i++)
		{	
			if( pcull->m_ShowModel[i]->GetModelData ()->cType > TYPE_PLANT_MODEL )
				continue;

			//如果该类型被锁，则不被选中
			if( pEditor->m_bLockModelEdit [pcull->m_ShowModel[i]->GetModelData ()->cType] )
				continue;

			pModel = (EditorModel*)pcull->m_ShowModel[i]->GetModelData ()->pModel ;

			//地形的拼接，不被选中
			if( TestIfLinkMesh(pcull->m_ShowModel[i]->GetModelData ()->name) )
				continue;

			if(pModel->GetShowModel()->intersectRay(INTERSECT_PICK, ray, NULL))
			{
				if( pEditor->m_pCurSelectModel )
					pEditor->m_CurSelectModelSet.push_back ( pEditor->m_pCurSelectModel );	


				pEditor->m_pCurSelectModel = pcull->m_ShowModel[i];
				pEditor->m_pCurSelectModel->GetModelData ()->m_pWireBox ->SetColor ( 0xff00ff00 );  

				pEditor->m_EditOption.pSelectModel = pEditor->m_pCurSelectModel;
				return true;
			}
		}

		return false;
	}

	bool  ModelSelectEditHandler::SelectRangeObject( TerrainEditMgr* pEditor ,int dx, int dy , int mode)
	{

		if ( pEditor->GetEditData() == NULL )
			return false;

		POINT  pos[4];
		Ogre::Vector3  dir[4];

		//pos[0].x = pEditor->m_nSaveMouseX ;
		//pos[0].y = pEditor->m_nSaveMouseY ;

		pos[2].x = dx ;
		pos[2].y = dy ;

		if( pos[0].x == pos[2].x 
			|| pos[0].y == pos[2].y )
		{
			return false;
		}

		pos[1].x = pos[2].x ;
		pos[1].y = pos[0].y ;

		pos[3].x = pos[0].x ;
		pos[3].y = pos[2].y ;

		for( int i = 0 ;i < 4 ; i++)
		{
			dir[i] = ChangeScenePointToRay( pos[i].x , pos[i].y );
		}

		Ogre::Vector3  normal[4];

		normal[0] = CrossProduct( dir[0] , dir[1] );
		normal[1] = CrossProduct( dir[1] , dir[2] );
		normal[2] = CrossProduct( dir[2] , dir[3] );
		normal[3] = CrossProduct( dir[3] , dir[0] );

		for( int i = 0 ;i < 4 ; i++)
		{
			Normalize( normal[i] );
		}

		bool firstObject = true;

		for( size_t i = 0 ; i < pEditor->GetEditData()->modelData.modelSet.size () ; i ++)
		{
			TerrModel* pModel = pEditor->GetEditData()->modelData.modelSet[i];
			if(  pModel->GetModelData ()->cType < 3)
			{
				Ogre::Vector3  curPos = pModel->GetModelData ()->pos ;
				Ogre::Vector3 curDir = curPos - g_Camera.GetPosition ();
				Normalize(curDir);

				if( TestNormal( normal[0] , normal[1] , normal[2] ,normal[3] , curDir ) )
				{
					if(firstObject)
					{
						pEditor->m_pCurSelectModel = pModel ;
						pEditor->m_pCurSelectModel->GetModelData ()->m_pWireBox ->SetColor ( 0xff00ff00 );  

						if( mode == 0 )
						{
							pEditor->m_EditOption.pSelectModel = pEditor->m_pCurSelectModel;
							pEditor->GetCoordinate()->bindObject( pEditor->m_pCurSelectModel->GetModelData ()->pModel  );
							g_Camera.BindObject ( pEditor->m_EditOption.pSelectModel );
						}
						firstObject = false;
					}
					else
					{				 
						pModel->GetModelData ()->m_pWireBox ->SetColor ( 0xff00ff00); 

						if ( mode == 0 )
							pEditor->m_CurSelectModelSet.push_back ( pModel );	
					}

					pModel->SetTransparent( 0.5f );	 
				}
				else
				{	
					if( pModel ->GetModelData ()->m_pWireBox )
					{
						pModel ->GetModelData ()->m_pWireBox ->SetColor ( 0xffaaaaaa );  
					}
					pModel->SetTransparent( 1.0f );
				}
			}
		}

		return  !firstObject;
	}

	bool   ModelSelectEditHandler::DelSelectScreenObject( TerrainEditMgr* pEditor ,Ogre::Vector3 &dir,Ogre::Vector3 &pos, int mode )
	{
		EditorModel* pModel;

		if( !pEditor->m_pCurSelectModel )
			return false;

		Ray ray(pos, dir);

		std::vector <TerrModel*>::iterator  iter;

		for( iter = pEditor->m_CurSelectModelSet.begin () ; iter != pEditor->m_CurSelectModelSet.end() ; iter++ )
		{
			pModel = (EditorModel*)( (*iter)->GetModelData ()->pModel ) ;       

			if(pModel->GetShowModel()->intersectRay(INTERSECT_PICK, ray, NULL))
			{
				(*iter)->GetModelData ()->m_pWireBox ->SetColor (  0xffaaaaaa  );

				pEditor->m_CurSelectModelSet	.erase ( iter );
				return true;
			}
		}

		return false;
	}

	bool   ModelSelectEditHandler::SelectScreenObject( TerrainEditMgr* pEditor , Ogre::Vector3 &dir,Ogre::Vector3 &pos, int mode)
	{	
		if( !pEditor->GetEditData() )
			return false;

		EditorModel* pModel;

		Ray ray(pos, dir);
		bool bFind = false;

		for(unsigned int i = 0 ; i < pEditor->GetEditData()->modelData .modelSet .size () ; i++)
		{
			pModel = (EditorModel*)pEditor->GetEditData()->modelData .modelSet [i]->GetModelData ()->pModel ;
			if( pModel == NULL )
				continue;

			if( pEditor->GetEditData()->modelData .modelSet [i]->GetModelData ()->cType > TYPE_PLANT_MODEL)
				continue;

			//如果该物件被锁，则不进行测试
			if ( pEditor->m_bLockModelEdit [ pEditor->GetEditData()->modelData .modelSet[i]->GetModelData ()->cType  ] )
				continue;

			if(pModel->GetShowModel()->intersectRay(INTERSECT_PICK, ray, NULL) && !bFind)
			{
				//地形的拼接，不被选中
				//if( TestIfLinkMesh( pEditor->GetEditData()->modelData .modelSet [i]->GetModelData ()->name ) )
				//{
				// continue;
				//}

				pEditor->GetEditData()->modelData .modelSet [i]->GetModelData ()->m_pWireBox ->SetColor ( 0xff00ff00 );

				//pModel->m_pModel->SetTransparent(0.5f);
				if( mode == 1)
				{
					pEditor->m_pCurSelectModel  =  pEditor->GetEditData()->modelData .modelSet [i];
				}	
				pEditor->m_EditOption.pSelectModel = pEditor->GetEditData()->modelData .modelSet [i];

				bFind = true;
			}
			else
			{
				if( pEditor->GetEditData()->modelData .modelSet [i]->GetModelData ()->m_pWireBox )
					pEditor->GetEditData()->modelData .modelSet [i]->GetModelData ()->m_pWireBox ->SetColor ( 0xffaaaaaa );

				//	pModel->m_pModel->SetTransparent(1.0f);
			}

		}

		return bFind;
	}
}

#endif