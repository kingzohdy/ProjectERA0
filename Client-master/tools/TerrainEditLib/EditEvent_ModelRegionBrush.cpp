
#include "EditEvent_ModelRegionBrush.h"
#include "EditorCamera.h"
#include "MapMark.h"
#include "EditUndo.h"

namespace TE
{
	ModelRegionBrushEditHandler::ModelRegionBrushEditHandler(void)
	{
		m_nProcessType = ADD_MORE_OBJECT_EVN ;
	}

	ModelRegionBrushEditHandler::~ModelRegionBrushEditHandler(void)
	{
	}
	//处理过程; 参数( 编辑器指针 , 对应响应事件 ) ; 返回 ( 为真表示成功 )
	bool ModelRegionBrushEditHandler::handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event )
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
				g_UndoManager.AddSaveUndoData();
			}
			break;
		case MouseMove:
			{
				if( event->bMouseDown )
				{
					RefreshMoreModel( pEditor , event->dir , event->pos );
				}
				else
				{
					RefreshMoreModel( pEditor , event->dir , event->pos , 1);
				}
			}
			break;
		case MouseLUp:
			{
				g_UndoManager.AddSaveRedoData();
			}
			break;				 			
		case MouseSewheel:
			{ 
				g_Camera.WalkTerrain(event->mz*5.0f,&g_Terrain,2);		
			}
			break;
		}
		return true;
	};
	//刷新大量模型; 参数( 编辑器指针 , 射线方向, 射线位置 , 模式 )
	void  ModelRegionBrushEditHandler::RefreshMoreModel( TerrainEditMgr* pEditor ,Ogre::Vector3 &dir,Ogre::Vector3 &pos,int mode)
	{   
		Ogre::Vector3  pos0;
		if(g_Terrain.TestAndRay(dir,pos,pos0))
		{   		
			pEditor->m_pSignSet->m_PosSet.clear ();

			float _len =  pEditor->m_EditOption.nCurAddModelSize*TERRBLOCK_SIZE / 64.0f;

			pEditor->m_pSignSet->UpdateData ( pos0 , _len , 16 );

			if( mode == 1)
			{
				return;
			}
			//测试距离
			if( Distance ( pEditor->m_EditOption.nSaveAddPlantPos , pos0 ) >  _len ) 
			{	
				pEditor->m_EditOption.nSaveAddPlantPos = pos0;

				Ogre::Vector3  newpos,newdir(0,0,0);
				float  newscale;

				int dx ,dy;
				for( int i  = 0 ; i < pEditor->m_EditOption.nCurAddModelNum  ; i ++)
				{ 
					dx = rand() % (2*(int)_len) - (int)_len;
					dy = rand() % (2*(int)_len) - (int)_len ;

					newpos.x = pos0.x + dx ;
					newpos.z = pos0.z + dy ;

					newdir.x = ( rand()%628 ) / 100.0f;

					newscale =( rand()%50 ) / 100.0f + 1.0f;

					if( g_Terrain.GetHeight ( newpos.x ,newpos.z ,&newpos.y ) )
					{
						addModel(pEditor, pEditor->getSelectModel()->GetModelData()->name, newpos, newdir, newscale);
					}
				}
			}
		}
	}
}
