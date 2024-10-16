#include ".\mterraineditmsgprocess.h"

#include "EditorCamera.h"
#include "minputprocess.h"
#include "EditEventHandler.h"
#include "Coordinate.h"

using namespace TE;


extern  float  g_fFov ;
#define WM_MOUSEWHEEL                   0x020A

MTerrainEditMsgProcess   g_TerrainEditMsgProcess;

MTerrainEditMsgProcess::MTerrainEditMsgProcess(void)
{
}

MTerrainEditMsgProcess::~MTerrainEditMsgProcess(void)
{
}


void MTerrainEditMsgProcess::MsgProc( MTerrainEdit* pEditor, void *hwnd, unsigned int msg, unsigned int wParam, unsigned int lParam)
{
	if( pEditor->m_bEditDisable )
		return;

    if( pEditor->m_EditOption.mCurEditEvent == UNKOWN_ACTION )
		return;

    if( PLACE_PLYAER == pEditor->m_EditOption.mCurEditEvent )
    {
        pEditor->m_EditOption.pEventProcess->MsgProc(pEditor , hwnd , msg , wParam , lParam);

        return;
    }

	HWND hWnd = (HWND)hwnd;
	pEditor->m_hwndView = hWnd ;

	 static ActionEvent actionEvent;
     switch( msg )
	 {
		 case WM_LBUTTONDOWN:
			 {	
				 if(pEditor->GetCamera() )
				 {

					pEditor->GetCamera()->RefreshRay (hWnd);
					actionEvent.dir = pEditor->GetCamera()->m_Ray.m_Dir;
					actionEvent.pos = pEditor->GetCamera()->m_Ray.m_Origin;
					actionEvent.mouseEven = MouseLDown;
					actionEvent.bMouseDown = true;
					actionEvent.mx = LOWORD(lParam) ;
					actionEvent.my = HIWORD(lParam) ;
	
					pEditor->Process( &actionEvent ) ;
				 }
			 }
			 break;
		 case WM_MBUTTONDOWN:
			 {
				 //if( g_Camera.m_CameraType == 0 )
				 if(1)
				 {
					POINT ptCursor; 	   
					GetCursorPos( &ptCursor );
					pEditor->GetCamera()->SetMousePos( ptCursor.x  ,ptCursor.y );
					pEditor->GetCamera()->m_bMouseMoveCamera = true;

					 //return;
				 }

				 if( g_MInputProcess.GetUserInput()->bLAlt && g_MInputProcess.GetUserInput()->bLCtrl )
				{           
					pEditor->m_bAltCtrlMoveCamera = true;
					//POINT ptCursor; 	   
					//GetCursorPos( &ptCursor );
					int x = LOWORD(lParam); 
					int y = HIWORD(lParam);
					pEditor->m_AltCtrlMoveX = x ;
					pEditor->m_AltCtrlMoveY = y ;
				}
				else
				{
					actionEvent.mouseEven = MouseDown;
					pEditor->SetCameraType (2);
					POINT ptCursor; 	   
					GetCursorPos( &ptCursor );
					int x = LOWORD(lParam); 
					int y = HIWORD(lParam);
					pEditor->GetCamera()->SetMousePos( x ,y );
					pEditor->GetCamera()->SetMouseMoveInitPos(x  ,y );
					pEditor->GetCamera()->m_bMouseMoveCamera = true;
				}
			 }
			 break;
		 case WM_MBUTTONUP:
			 {
				 pEditor->m_bAltCtrlMoveCamera = false; 
				 pEditor->GetCamera()->m_bMouseMoveCamera = false;
				actionEvent.dir = pEditor->GetCamera()->m_Ray .m_Dir;
				actionEvent.pos = pEditor->GetCamera()->m_Ray.m_Origin;
				actionEvent.mouseEven = MouseRUp;

	
				pEditor->Process( &actionEvent ) ;		
				
				actionEvent.mouseEven = MouseUp;   

				if( g_Camera.m_CameraType != 0 )
				    pEditor->SetCameraType (1);
			 }
			 break;
		case WM_MOUSEWHEEL  :
			{	
				actionEvent.mouseEven = MouseSewheel;
				actionEvent.mz = (short)HIWORD(wParam);
	
				pEditor->Process( &actionEvent ) ;
			}
		case WM_RBUTTONUP:
			{   
				pEditor->GetCamera()->m_bMouseMoveCamera = false;
				actionEvent.dir = pEditor->GetCamera()->m_Ray.m_Dir;
				actionEvent.pos = pEditor->GetCamera()->m_Ray.m_Origin;
				actionEvent.mouseEven = MouseRUp;

				pEditor->Process( &actionEvent ) ;
			}
			break;
		case WM_RBUTTONDOWN:
			{ 			
				actionEvent.dir = pEditor->GetCamera()->m_Ray.m_Dir;
				actionEvent.pos = pEditor->GetCamera()->m_Ray.m_Origin;
				actionEvent.mouseEven = MouseRDown ;

				pEditor->Process( &actionEvent ) ;
			}
			break;
		case WM_LBUTTONUP:
			{
				if(pEditor->GetCamera())
				{
					pEditor->GetCamera()->RefreshRay (hWnd);
					actionEvent.dir = pEditor->GetCamera()->m_Ray.m_Dir;
					actionEvent.pos = pEditor->GetCamera()->m_Ray.m_Origin;
					actionEvent.mouseEven = MouseLUp;
					actionEvent.bMouseDown = false;
		           actionEvent.mx = LOWORD(lParam) ;
					actionEvent.my = HIWORD(lParam) ;

					pEditor->Process( &actionEvent ) ;
				}
			}
			break;	
		case WM_MOUSEMOVE:
			{
				if( pEditor->GetCamera() )
				{
					pEditor->GetCamera()->RefreshRay (hWnd); 
					actionEvent.mouseEven = MouseMove;
					actionEvent.dir = pEditor->GetCamera()->m_Ray.m_Dir;
					actionEvent.pos = pEditor->GetCamera()->m_Ray.m_Origin;
					actionEvent.mx = LOWORD(lParam) ;
					actionEvent.my = HIWORD(lParam) ;

					pEditor->Process( &actionEvent ) ;
				}
			}
			break;				
		case WM_KEYDOWN:
			{
				switch( wParam )
				{
					case 219://'{'
						{			
							pEditor->ChangePaintSize(-1);
						}
						break;
					case 221://'}'
						{
							pEditor->ChangePaintSize(1);
						}
						break;

					case VK_LEFT:
						pEditor->RoateModel(120);
						break;
					case VK_RIGHT:
						pEditor->RoateModel(-120);
						break;
					case  VK_DOWN:
						{
							pEditor->ControlModel( 0 );
						}
						break;
					case VK_DELETE:
						{
							pEditor->DelCurObject();
						}
						break;
					case 187://'=':
						{
							 g_fFov += 5 ;
						     pEditor->ChangePaintPower( 1 ); 
						}
						break;
					case 189://'-':
						{													
							g_fFov -= 5 ;
							pEditor->ChangePaintPower( -1 );                    
						}
						break;
					case VK_UP:
						{
							pEditor->ControlModel( 1 );
						}
						break;
					case 'W':
						if(ADD_OBJECT_EVN == pEditor->GetCurEditEvent() || 
							MOVE_OBJECT_EVN == pEditor->GetCurEditEvent() ||
							ROATE_OBJECT_EVN == pEditor->GetCurEditEvent() ||
							SELECT_OBJECT_EVN == pEditor->GetCurEditEvent()||
							SCALE_OBJECT_EVN == pEditor->GetCurEditEvent())
						{
						    pEditor->m_bPessQ = false;
							pEditor->SetCurEditEvent(MOVE_OBJECT_EVN);
							pEditor->GetCoordinate()->setCoordMode ( 0 );
						}
						break;
					case 'E':
						if(ADD_OBJECT_EVN == pEditor->GetCurEditEvent() || 
							MOVE_OBJECT_EVN == pEditor->GetCurEditEvent() ||
							ROATE_OBJECT_EVN == pEditor->GetCurEditEvent() ||
							SELECT_OBJECT_EVN == pEditor->GetCurEditEvent()||
							SCALE_OBJECT_EVN == pEditor->GetCurEditEvent())
						{
						    pEditor->SetCurModelAction( TE::MODEL_ROATE_Y );
							pEditor->SetCurEditEvent(ROATE_OBJECT_EVN);
							pEditor->GetCoordinate()->setCoordMode ( 1 );
						}
						break;
					case 'R':
						if(ADD_OBJECT_EVN == pEditor->GetCurEditEvent() || 
							MOVE_OBJECT_EVN == pEditor->GetCurEditEvent() ||
							ROATE_OBJECT_EVN == pEditor->GetCurEditEvent() ||
							SELECT_OBJECT_EVN == pEditor->GetCurEditEvent()||
							SCALE_OBJECT_EVN == pEditor->GetCurEditEvent())
						{
							pEditor->SetCurModelAction ( TE::MODEL_SCALE );
							pEditor->SetCurEditEvent(SCALE_OBJECT_EVN);
							pEditor->GetCoordinate()->setCoordMode ( 2 );
						}
						break;	
		
					case 'Q':
						if(ADD_OBJECT_EVN == pEditor->GetCurEditEvent() || 
							MOVE_OBJECT_EVN == pEditor->GetCurEditEvent() ||
							ROATE_OBJECT_EVN == pEditor->GetCurEditEvent() ||
							SELECT_OBJECT_EVN == pEditor->GetCurEditEvent()||
							SCALE_OBJECT_EVN == pEditor->GetCurEditEvent())
						{
							pEditor->m_bPessQ = true;
							pEditor->SetCurEditEvent(SELECT_OBJECT_EVN);
						}
						break;
					case 'Z':
						{
							pEditor->MoveCameraToModel();
						}
						break;
					case 'N':
						{
							pEditor->SetShowChunk(!pEditor->m_DrawOption.bDrawChunk  );
						}
						break;
					case 'M':
						{
							pEditor->SetShowLiquid(!pEditor->m_DrawOption.bDrawLiquid);
						}
						break;
					case VK_F2:
						{
							pEditor->SetShowQuadPath(!pEditor->m_DrawOption.bDrawQuad);
						}
						break;
					case VK_F3:
						{
							if(pEditor->m_DrawFillMode == DRAW_SOLID)
							{						
								pEditor->SetChunkDrawMode(DRAW_WIREFRAME);
							}
							else
							{
								pEditor->SetChunkDrawMode(DRAW_SOLID);
							}

						}
						break;
					case VK_F4:
						{
							pEditor->SetShowObjectBox( !pEditor->m_bShowObjectBox );
						}
						break;
					case VK_F5:
						{
							pEditor->SaveCurEditEvent();
							pEditor->SetCurEditEvent(PLACE_PLYAER);
						}
						break;
					case VK_F7:
						{
							pEditor->GetCamera()->SetOverView();
						}
                        break;
				}
			}
			break;
    }
};
