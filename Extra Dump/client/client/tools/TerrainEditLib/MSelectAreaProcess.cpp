#include ".\mselectareaprocess.h"

#include "EditorCamera.h"
#include "EnvRegion.h"

using namespace TE;

MSelectAreaProcess::MSelectAreaProcess(void)
{
	m_nProcessType = SELECT_AREA_EVEN ;
}

MSelectAreaProcess::~MSelectAreaProcess(void)
{
}


bool MSelectAreaProcess::Process( MTerrainEdit* pEditor , ActionEvent *event )
{
	switch( event->mouseEven )
	{
	case MouseLDown:
		{
			SelectArea( event->dir , event->pos );
		}
		break;
	case MouseMove:
		{	
            /// 只有在按鼠标左键确认选择区域时才算选中该区域，故不用此方式 by ashluo
			//pEditor->MoveSign(event->dir ,event->pos );
			//SelectArea( event->dir , event->pos );
		}
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

	case MouseRDown:
		{
            /// 取消从对话框载入区域文件的方式  by ashluo
            // pEditor->m_bOpenAreaInfoDlg = true;
		}
		break;
	}

	return true;
}

bool MSelectAreaProcess::SelectArea( MVec3 &dir , MVec3 &pos)
{
    return g_AreaSet.SelectArea(dir , pos);
}