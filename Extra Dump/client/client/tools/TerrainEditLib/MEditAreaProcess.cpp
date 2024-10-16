
#include ".\meditareaprocess.h"

#include "EditorCamera.h"
#include "EnvRegion.h"
#include "mterrain.h"
#include "MInputProcess.h"

using namespace TE;

MEditAreaProcess::MEditAreaProcess(void)
{	
	m_nProcessType = AREA_EIDT_EVEN ;
}

MEditAreaProcess::~MEditAreaProcess(void)
{
}


bool MEditAreaProcess::Process( MTerrainEdit* pEditor , ActionEvent *event )
{
	switch( event->mouseEven )
	{
	case MouseLUp:
		{
            if(g_MInputProcess.GetUserInput()->bLShift)
                DelAreaCurPos();
            else 
            {
                if(0xff != pEditor->m_EditOption.mSaveCurEditEvent)
                    AddAreaPos( event->dir , event->pos );
                else
                    pEditor->m_EditOption.mSaveCurEditEvent = (EIDT_EVENT)0;
            }
		}
		break;
	case MouseMove:
		{		
            pEditor->MoveSign(event->dir ,event->pos );

			if(event->bMouseDown )
            {
                MoveAreaPos( event->dir , event->pos );
                pEditor->m_EditOption.mSaveCurEditEvent = (EIDT_EVENT)0xff;     /// 临时占用，记录前一操作为移动区域点
            }
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
			//pEditor->m_bOpenAreaInfoDlg = true;
		}
		break;
	}
	return true;
};

bool MEditAreaProcess::AddAreaPos(  MVec3 &dir , MVec3 &pos  )
{
 	MVec3  pos0;

	if( g_Terrain.TestAndRay (dir,pos,pos0) )
	{	
		pos0.y += 2.0f;
		g_AreaSet.AddCurAreaPos( pos0 );
		return true;
	}
	return false;
};

void MEditAreaProcess::DelAreaCurPos()
{
     g_AreaSet.DelAreaCurPos();
};

bool MEditAreaProcess::MoveAreaPos( MVec3 &dir , MVec3 &pos )
{
 	MVec3  pos0;

	if( g_Terrain.TestAndRay (dir,pos,pos0))
	{
		pos0.y += 2.0f;
		g_AreaSet.MoveCurPos ( pos0 );
	   	
		return true;
	}
	return false;
};		
