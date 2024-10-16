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
            /// ֻ���ڰ�������ȷ��ѡ������ʱ����ѡ�и����򣬹ʲ��ô˷�ʽ by ashluo
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
            /// ȡ���ӶԻ������������ļ��ķ�ʽ  by ashluo
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