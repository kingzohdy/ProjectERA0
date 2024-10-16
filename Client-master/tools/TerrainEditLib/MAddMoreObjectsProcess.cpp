
#include ".\maddmoreobjectsprocess.h"
#include "EditorCamera.h"
#include "MSign.h"

using namespace TE;

MAddMoreObjectsProcess::MAddMoreObjectsProcess(void)
{
	m_nProcessType = ADD_MORE_OBJECT_EVN ;
}

MAddMoreObjectsProcess::~MAddMoreObjectsProcess(void)
{
}
//处理过程; 参数( 编辑器指针 , 对应响应事件 ) ; 返回 ( 为真表示成功 )
bool MAddMoreObjectsProcess::Process( MTerrainEdit* pEditor , ActionEvent *event )
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
			pEditor->AddSaveUndoData();
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
			pEditor->AddSaveRedoData();
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
void  MAddMoreObjectsProcess::RefreshMoreModel( MTerrainEdit* pEditor ,MVec3 &dir,MVec3 &pos,int mode)
{   
	MVec3  pos0;
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

			MVec3  newpos,newdir(0,0,0);
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
					pEditor->AddPlantModelToTitle( pEditor->m_pCurSelectModel->GetModelData()->name , newpos , newdir ,newscale);
				}
			}
		}
	}
}