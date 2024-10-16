
#include "EditEvent_LightBrush.h"

#include "MapLights.h"
#include "EditorModel.h"
#include "Terrain.h"
#include "Coordinate.h"
#include "EditorCamera.h"
#include "EditorMessageQue.h"

using namespace Ogre;

namespace TE
{
	LightEditHandler::LightEditHandler(void)
	{
		m_nProcessType = LIGHT_EDIT_EVN ;
	}

	LightEditHandler::~LightEditHandler(void)
	{
	}

	//选择屏幕灯光;参数( 编辑器指针 , 射线方向 , 射线位置, 模式 ) ; 返回 ( 为真表示成功 )
	bool LightEditHandler::SelectScreenLight( TerrainEditMgr* pEditor ,Ogre::Vector3 &dir,Ogre::Vector3 &pos, int mode )	
	{
		if( !pEditor->GetEditData() )
		{
			return false;
		}

		EditorModel* pModel = NULL ;
		SceneNode *pNode  = NULL ;

		for(unsigned int i = 0 ; i < g_LightManager.m_LightSet .size () ; i++)
		{
			TerrModel* pTileModel = g_LightManager.m_LightSet[i].pModel ;
			assert( pTileModel );

			if( pTileModel->TestIntersect( pos , dir ) )			
			{							
				if(  pEditor->m_EditOption.pSelectModel )				
				{
					pEditor->m_EditOption.pSelectModel->GetModelData ()->m_pWireBox ->SetColor(0xffaaaaaa);
				}									
				pTileModel->GetModelData ()->m_pWireBox ->SetColor(0xff00ff00);
				if( mode == 1)
				{
					pEditor->m_pCurSelectModel  = pTileModel ;
				}	
				pEditor->m_EditOption.pSelectModel = pTileModel ;

				g_LightManager.setCurLightObject(i);
				return true;
			}
		}
		/*
		if( mode == 1)
		{
		pEditor->m_pCurSelectModel  = NULL ;
		}	
		pEditor->m_EditOption.pSelectModel = NULL  ;

		m_nSelectLightNodeInfoIdx = -1 ;*/

		return false;	   
	}
	//增加灯光; 参数( 编辑器指针 ,  射线位置 ,射线方向 )
	void  LightEditHandler::AddLight( TerrainEditMgr* pEditor, Ogre::Vector3 &pos , Ogre::Vector3 &dir)
	{
		Ogre::Vector3  pos0,dir0(0,0,0);

		if(g_Terrain.TestAndRay(dir,pos,pos0))
		{
			LightInfo  lightInfo;
			lightInfo.color = Ogre::ColourValue(1.0f, 1.0f, 0.0f, 1.0f);
			lightInfo.volume = 1.0f;

			if(g_LightManager.m_CurEditMode == MapLightsManager::EDIT_ADDLIGHT)
			{
				lightInfo.nType = 0;
				lightInfo.fRange = 500.0f;
				lightInfo.pos = pos0;// + Vector3(0, 200.0f, 0);
			}
			else
			{
				lightInfo.nType = 1;
				lightInfo.fRange = 5000.0f;
				lightInfo.respath = "sounds\\default.wav";
				lightInfo.pos = pos0;
			}

			LightNodeInfo pNodeInfo = g_LightManager.AddLight(lightInfo);
		}
	}

	//灯光处理过程; 参数 ( 编辑器指针, 对应消息事件 ) ; 返回 ( 为真表示成功 )
	bool LightEditHandler::handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event )
	{
		switch( event->mouseEven )
		{
		case MouseLDown:
			{
				if(!SelectScreenLight( pEditor , event->dir ,event->pos ,1))
				{
					pEditor->m_select_coordinate = pEditor->GetCoordinate()->intersect(event->dir , event->pos, 0);

					if(pEditor->m_select_coordinate==NO_AXES_SELECT && (g_LightManager.m_CurEditMode==MapLightsManager::EDIT_ADDLIGHT || g_LightManager.m_CurEditMode==MapLightsManager::EDIT_ADDSOUND))
					{
						AddLight(pEditor, event->pos, event->dir);
					}
					else
					{/*
						
						if( pEditor->m_select_coordinate == NO_AXES_SELECT)
						{
							MoveSelectModel(pEditor, event->dir, event->dir);
						}*/
					}
				}
				else
				{				
					pEditor->GetCoordinate()->bindObject(pEditor->m_EditOption.pSelectModel->GetModelData()->pModel);
					g_Camera.BindObject(pEditor->m_EditOption.pSelectModel);
					int sel = g_LightManager.findLightBySelectModel(pEditor->m_EditOption.pSelectModel);
					g_LightManager.setCurLightObject(sel);
					g_EditorMsgQue.postMessage(EDMSG_SELECTLIGHT);
				}; 		
				pEditor->SetMoveAxes(event->mx , event->my);
			}
			break;

		case MouseMove:
			{
				if( event->bMouseDown )
				{			
					//如果高度被锁住，则只在XZ表面移动
					if( g_LightManager.m_bLockHeight )
					{
						pEditor->m_select_coordinate = SELECT_XZ_AXES;
					}
					//如果没有选中坐标,则物体随地表移动
					if( pEditor->m_select_coordinate == NO_AXES_SELECT)
					{
						MoveSelectModel(pEditor, event->dir, event->pos);
					}
					else
					{
						pEditor->MoveMoveAxes(pEditor->m_select_coordinate , event->mx , event->my , event->pos  , event->dir , 1);
					}

					g_LightManager.getCurLightInfo().pos = pEditor->m_pCurSelectModel->GetModelData()->pos;
					g_LightManager.setCurLightInfoToNode();
				}
				else
				{
					pEditor->ClearObjectColor(0xffff0000);
					MoveSelectModel( pEditor , event->dir , event->pos , 1);	

					pEditor->m_select_coordinate = pEditor->GetCoordinate()->intersect( event->dir , event->pos , 0);
					SelectScreenLight( pEditor , event->dir ,event->pos , NULL);
				}
			}
			break;
		case MouseLUp:
			{
			}
			break;
		case MouseSewheel:
			{	
				g_Camera.WalkTerrain(event->mz*5.0f,&g_Terrain,2);	
			}
			break;

		case MouseRDown :			
			{
				pEditor->m_bOpenLightInfoDlg = true;
			}
			break;
		}

		return true;
	};
	//后退操作
	void  LightEditHandler::Undo( TerrainEditMgr* pEditor )
	{

	}
}
