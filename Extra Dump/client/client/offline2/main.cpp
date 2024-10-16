//#include "../include/vld.h"
#include "OfflineManager.h"
#include <windows.h>
#include "OgreOSUtility.h"
#include "resource.h"
#include "FlickerManager.h"
#include <shobjidl.h>
#ifdef _DEBUG
#pragma comment(lib, "OgreMain_d.lib")
#pragma comment(lib, "GameEngine_d.lib")
#pragma comment(lib, "UILib_d.lib")
#pragma comment(lib, "luadll_d.lib")
#pragma comment(lib, "tolua++_d.lib")
#else
#pragma comment(lib, "OgreMain.lib")
#pragma comment(lib, "GameEngine.lib")
#pragma comment(lib, "UILib.lib")
#pragma comment(lib, "luadll.lib")
#pragma comment(lib, "tolua++.lib")
#endif
#pragma comment(lib, "Imm32.lib")
//#pragma comment(lib, "vld.lib")

HMENU hMenu,hMenufairy;

void ShowInTaskbar( HWND hWnd, BOOL bShow )
{
	// 在程序的起始处（例如 InitInstance 中）加入 ::CoInitialize( NULL );

	HRESULT hr;
	ITaskbarList* pTaskbarList;

	hr = CoCreateInstance( CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, 
		IID_ITaskbarList, ( void** )&pTaskbarList );

	pTaskbarList->HrInit( );

	if( bShow )
	{
		pTaskbarList->AddTab( hWnd );
	}
	else
	{
		pTaskbarList->DeleteTab( hWnd );
	}

	pTaskbarList->Release( );

	// 在程序的末尾（例如 ExitInstance 中）加入::CoUninitialize( );
}

LRESULT CALLBACK MyWinProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	OfflineManager *offline = OfflineManager::getSingletonPtr();
	if(offline)
	{			
		return offline->OnWindowMsg(hWnd, uiMsg, (unsigned int)wParam, (unsigned int)lParam);
	}
	else 
	{
		return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}
}

LRESULT CALLBACK ZeroWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	ShowInTaskbar( hWnd, false);
	switch (message)
	{
	case WM_LBUTTONDOWN:
		SystemParametersInfo(SPI_SETDRAGFULLWINDOWS,FALSE,NULL,0);
		SendMessage(hWnd, WM_SYSCOMMAND, 0xF012, 0);  
		break;
	case WM_ERASEBKGND:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_RBUTTONDOWN: 
		POINT point;
		GetCursorPos(&point);
		SetForegroundWindow(hWnd); // 修正当用户按下ESCAPE 键或者在菜单之外单击鼠标时菜单不会消失的情况
		TrackPopupMenu(hMenufairy, TPM_RIGHTBUTTON, point.x, point.y, 0, hWnd, NULL);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	::CoInitialize( NULL );
	WNDCLASSEX kWC;
	kWC.cbSize		  = sizeof(WNDCLASSEX);
	kWC.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	kWC.lpfnWndProc   = MyWinProc;
	kWC.cbClsExtra    = 0;
	kWC.cbWndExtra    = 0;
	kWC.hInstance     = hInstance;
	kWC.hIcon         = LoadIcon( kWC.hInstance, MAKEINTRESOURCE(IDI_MAIN));
	kWC.hCursor       = LoadCursor(NULL, IDC_ARROW);
	kWC.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	kWC.lpszClassName = "GameManagerWin";
	kWC.lpszMenuName  = 0;
	kWC.hIconSm		  = LoadIcon( kWC.hInstance, MAKEINTRESOURCE(IDI_MAIN));
	RegisterClassEx(&kWC);
	WNDCLASSEX kZeroWC;
	kZeroWC.cbSize		  = sizeof(WNDCLASSEX);
	kZeroWC.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	kZeroWC.lpfnWndProc   = ZeroWinProc;
	kZeroWC.cbClsExtra    = 0;
	kZeroWC.cbWndExtra    = 0;
	kZeroWC.hInstance     = hInstance;
	kZeroWC.hIcon         = LoadIcon( kZeroWC.hInstance, MAKEINTRESOURCE(IDI_MAIN));
	kZeroWC.hCursor       = LoadCursor(NULL, IDC_ARROW);
	kZeroWC.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	kZeroWC.lpszClassName = "ZeroWin";
	kZeroWC.lpszMenuName  = 0;
	kZeroWC.hIconSm		  = LoadIcon( kZeroWC.hInstance, MAKEINTRESOURCE(IDI_MAIN));
	RegisterClassEx(&kZeroWC);
	unsigned long style;
	style = WS_POPUP|WS_MINIMIZEBOX|WS_SYSMENU;
	int screenWidth		= GetSystemMetrics( SM_CXSCREEN );
	int screenHeight	= GetSystemMetrics( SM_CYSCREEN );
	HWND hWnd	 = CreateWindow(   "GameManagerWin" , 
									"Offline" , 
									style, 
									screenWidth - screenWidth /10 - LOGIN_WIN_WIDTH,
									screenHeight /10, 
									LOGIN_WIN_WIDTH,
									LOGIN_WIN_HEIGHT, 
									NULL, 
									NULL, 
									( HINSTANCE )hInstance, 
									NULL );
	style = WS_POPUP;
	//尺寸为0的窗口辅助托盘菜单自动消失问题
	HWND hwndZero = CreateWindow(	"GameManagerWin" , 
									"Zero" , 
									style, 
									0,
									0, 
									0,
									0, 
									NULL, 
									NULL, 
									( HINSTANCE )hInstance, 
									NULL );
	int nScreenWidth  = GetSystemMetrics( SM_CXSCREEN );
	int nScreenHeight = GetSystemMetrics( SM_CYSCREEN );
	HWND hwndfairy	  = CreateWindow(	"ZeroWin" , 
										"Zero" , 
										style, 
										1000,
										650, 
										160,
										160, 
										NULL, 
										NULL, 
										( HINSTANCE )hInstance, 
										NULL );
	hMenu=LoadMenu(( HINSTANCE )hInstance, MAKEINTRESOURCE(IDR_MENU1));
	hMenufairy=LoadMenu(( HINSTANCE )hInstance, MAKEINTRESOURCE(IDR_MENU2));
	hMenufairy = GetSubMenu(hMenufairy,0);
	//从客户端启动引力空间的标志布尔变量
	bool bdirect = false;
	if( strcmp( lpCmdLine,"") != 0 )
	{
		bdirect = true;
	}

	OfflineManager Offmgr;

	if (!Offmgr.init("client.cfg",hWnd,hInstance,bdirect,hMenu,(const char*)lpCmdLine))
	{
		MessageBox(NULL, "初始化游戏出错，请把游戏log文件给开发人员", "错误", MB_OK);
		return -1;
	}


	FlickerManager::getSingleton().setZerohwnd( hwndZero );
	//OfflineFairy::getSingleton().getFairyhwnd( hwndfairy );
	ShowWindow( hWnd, 1 );
	UpdateWindow( hWnd );
	SetWindowPos(hwndfairy, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
	while(true)
	{
		MSG kMsg;
		if (PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
		{
			if (kMsg.message == WM_QUIT) 
			{
					break;	
			}
			TranslateMessage(&kMsg);
			DispatchMessage(&kMsg);
		}
		else
		{
			Offmgr.update();
		}
	}
	Offmgr.Destroy();
	::CoUninitialize( );
	return 0;
}
