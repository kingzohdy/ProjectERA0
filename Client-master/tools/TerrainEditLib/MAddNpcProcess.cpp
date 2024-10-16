
#include ".\maddnpcprocess.h"

#include "EditorModel.h"
#include "mnpc.h"
#include "Coordinate.h"
#include "EditorCamera.h"
#include "MNPC.h"

#include "GameDBData.h"

using namespace TE;
using namespace Ogre;

MAddNpcProcess::MAddNpcProcess(void)
{
	m_nProcessType = ADD_NPC ;
}

MAddNpcProcess::~MAddNpcProcess(void)
{
}

//处理过程; 参数( 编辑器指针 , 响应事件消息 ) 返回( 为真表示成功 )
bool MAddNpcProcess::Process( MTerrainEdit* pEditor , ActionEvent *event )
{
	switch( event->mouseEven )
	{
	case MouseLDown:
		{
			if( !SelectScreenNPC( pEditor , event->dir ,event->pos ,1) )
			{
				AddNpc( pEditor , event->pos , event->dir  );

				pEditor->m_select_coordinate = pEditor->GetCoordinate()->intersect(event->dir , event->pos, 0);

				if( pEditor->m_select_coordinate == NO_AXES_SELECT )
				{
					MoveSelectModel( pEditor , event->dir , event->pos );
				}
			}
			else
			{
				pEditor->GetCoordinate()->bindObject ( pEditor->m_EditOption.pSelectModel->GetModelData ()->pModel  );
				g_Camera.BindObject(pEditor->m_EditOption.pSelectModel);

				pEditor->m_EditOption.nCurSelectNpcID = 0;
				pEditor->m_EditOption.CurSelectNpcIndex = pEditor->getNpcManager()->findNpcByModel(pEditor->m_EditOption.pSelectModel);
			}; 		
			pEditor->SetMoveAxes( event->mx , event->my );	

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
				pEditor->MoveMoveAxes( pEditor->m_select_coordinate , event->mx , event->my , event->pos  , event->dir );
			}
		}
		else
		{	
			pEditor->ClearObjectColor(0xffff0000);
			MoveSelectModel( pEditor , event->dir , event->pos , 1);	

			pEditor->m_select_coordinate = pEditor->GetCoordinate()->intersect(event->dir , event->pos , 0);
			SelectScreenNPC( pEditor , event->dir ,event->pos , NULL);
		}
		break;
	case MouseLUp:
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
//选择屏幕NPC ; 参数( 编辑器指针 , 射线方向, 射线位置 , 模式 ) 返回 ( 为真表示成功 )
bool   MAddNpcProcess::SelectScreenNPC( MTerrainEdit* pEditor ,MVec3 &dir,MVec3 &pos, int mode)	
{
	if( !pEditor->GetEditData() )
	{
		return false;
	}

	  for(unsigned int i = 0 ; i < pEditor->getNpcManager()->m_NpcSet .size () ; i++)
	   {
		   TerrModel* pModel = pEditor->getNpcManager()->m_NpcSet [i]->GetModel() ;
		   assert( pModel );

		   if ( pModel->TestIntersect( pos ,dir ) )
		   {	
			   if(  pEditor->m_EditOption.pSelectModel )				
			   {			
				   pEditor->m_EditOption.pSelectModel->GetModelData ()->pDrawbox ->SetColor (  0xffaaaaaa   );     	
			   }									

			   pEditor->getNpcManager()->m_NpcSet [i]->GetModel ()->GetModelData ()->pDrawbox ->SetColor ( 0xff00ff00 );

			   if( mode == 1)
			   {
				   pEditor->m_pCurSelectModel  = pEditor->getNpcManager()->m_NpcSet [i]->GetModel();
			   }	

			   pEditor->m_EditOption.pSelectModel = pEditor->getNpcManager()->m_NpcSet [i]->GetModel();
			   return true;
		   }
	   }
	   	
	   return false;
}
//增加NCP ; 参数( 编辑器指针, 射线位置, 射线方向 )
void  MAddNpcProcess::AddNpc( MTerrainEdit* pEditor ,MVec3 &pos , MVec3 &dir)
{
	if(pEditor->m_EditOption.nCurSelectNpcID <= 0)
	{
		return;
	}

	MVec3  pos0,dir0(0,0,0);

	if(g_Terrain.TestAndRay(dir,pos,pos0))
	{
		const NPCDISPLAYINFO *pinfo = &DBDataManager::getSingleton().GetNpcDisplayInfo(pEditor->m_EditOption.nCurSelectNpcID);
		MNPC *pNpc = new MNPC(pinfo->name, pinfo->model, pinfo->id, pos0, dir0, "");

		pEditor->getNpcManager()->AddNpc(pNpc);
	}
}
//增加具体NPC ; 参数( 编辑器指针 , NPC名,对应模型目录, NPCID ,NPC 位置, NPC 方向, 对应脚本 )
void      MAddNpcProcess::AddNPC( MTerrainEdit* pEditor ,const char* npcName , const char* npcModelPath ,  unsigned int npcID,
	          MVec3 npcPos, MVec3 npcDir ,const char* script )
{
	if( pEditor->GetEditData() == NULL )
	{
		return ;
	}

	MNPC *pNpc =  pEditor->getNpcManager()->FindNpc ( npcID ) ;

	if ( pNpc )
	{
		pNpc->SetPos ( npcPos );
	}
	else
	{
		pNpc = new MNPC( npcName , npcModelPath , npcID , npcPos ,npcDir, script );
		 pEditor->getNpcManager()->AddNpc ( pNpc );
	}
}
