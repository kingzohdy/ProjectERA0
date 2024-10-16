#include "EditEvent_MapRegionSelect.h"
#include "EditorCamera.h"
#include "EnvRegion.h"

namespace TE
{
	RegionSelectEditHandler::RegionSelectEditHandler(void)
	{
		m_nProcessType = SELECT_AREA_EVEN ;
	}

	RegionSelectEditHandler::~RegionSelectEditHandler(void)
	{
	}


	bool RegionSelectEditHandler::handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event )
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

	bool RegionSelectEditHandler::SelectArea( Ogre::Vector3 &dir , Ogre::Vector3 &pos)
	{
		return g_AreaSet.SelectArea(dir , pos);
	}
}
