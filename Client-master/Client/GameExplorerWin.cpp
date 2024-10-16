//#include "GameExplorerWin.h"
//#include <Windows.h>
//#include <atlbase.h>
//CComModule _Module;
//#include <atlwin.h>
//#include "exdisp.h"
//
//LRESULT CALLBACK ExpWinProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
//{
//	switch( uiMsg )
//	{
//	case WM_ACTIVATE:
//		{
//			return MA_NOACTIVATE;
//		}
//		break;
//	case WM_NCLBUTTONDOWN:
//	case WM_LBUTTONDOWN:
//		break;
//	case WM_NCACTIVATE:
//	case WM_MOUSEACTIVATE:
//		break;
//	}
//	
//	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
//}
//
//GameExplorerWin::GameExplorerWin(void)
//{
//	
//}
//
//GameExplorerWin::~GameExplorerWin(void)
//{
//	CoUninitialize();
//}
//
//LRESULT onActive(UINT uMsg, WPARAM wParam, 
//				 LPARAM lParam, BOOL& bHandled)
//{
//		return MA_NOACTIVATE;
//}
//
//
//class MyWindow : public CAxWindow
//{
//public:
//	BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0)
//	{
//		if( uMsg == WM_ACTIVATE )
//		{
//			lResult = MA_NOACTIVATE;
//			return TRUE;
//		}else if( uMsg == WM_NCACTIVATE )
//		{
//			lResult = MA_NOACTIVATE;
//			return TRUE;
//		}else if( uMsg == WM_MOUSEACTIVATE )
//		{
//			lResult = MA_NOACTIVATE;
//			return TRUE;
//		}
//		return FALSE;
//	}
//};
//static MyWindow WinContainer;
//void GameExplorerWin::intitExplorerWin( void* hPreWin, void* pPreInstan, int w, int h )
//{
//	CoInitialize(NULL);
//	AtlAxWinInit();
//
//// 	WNDCLASS kWCChild;
//// 	kWCChild.style         = CS_HREDRAW | CS_VREDRAW;
//// 	kWCChild.lpfnWndProc   = ExpWinProc;
//// 	kWCChild.cbClsExtra    = 0;
//// 	kWCChild.cbWndExtra    = 0;
//// 	kWCChild.hInstance     = (HINSTANCE)pPreInstan;
//// 	kWCChild.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
//// 	kWCChild.hCursor       = LoadCursor(NULL, IDC_ARROW);
//// 	kWCChild.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
//// 	kWCChild.lpszClassName = "explorer";
//// 	kWCChild.lpszMenuName  = 0;
//// 	RegisterClass(&kWCChild);
//// 
//// 	m_hPreWnd	= hPreWin;
//// 
//// 	int x = (::GetSystemMetrics( SM_CXSCREEN) - w) / 2;
//// 	int y = (::GetSystemMetrics( SM_CYSCREEN ) - h) / 2;
//// 	HWND m_hWnd = CreateWindow( "explorer", 
//// 								"explorer", 
//// 								WS_POPUP | WS_EX_TOPMOST,
//// 								x,
//// 								y,
//// 								w,
//// 								h,
//// 								(HWND)hPreWin, 
//// 								0, 
//// 								(HINSTANCE)pPreInstan, 
//// 								NULL );
//// 	
//// 	ShowWindow(m_hWnd, SW_SHOWNA);
//// 	UpdateWindow(m_hWnd);
//
//	RECT rc;
//	IWebBrowser2* iWebBrowser;
//	VARIANT varMyURL;
//	
//	LPOLESTR pszName=OLESTR("shell.Explorer.2");
//	//WinContainer.
//	GetClientRect((HWND)hPreWin, &rc);
//
//	WinContainer.Create( (HWND)hPreWin, rc, 0, WS_POPUP,WS_EX_TOOLWINDOW );	
//	WinContainer.CreateControl(pszName);
//	WinContainer.QueryControl(__uuidof(IWebBrowser2),(void**)&iWebBrowser); 
//	VariantInit(&varMyURL);
//	varMyURL.vt = VT_BSTR; 
//	varMyURL.bstrVal = SysAllocString(L"http://192.168.0.100/my_view_page.php");
//	iWebBrowser->Navigate2(&varMyURL,0,0,0,0);
//	VariantClear(&varMyURL);
//	iWebBrowser->Release();
//	WinContainer.SetWindowPos(NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE);
//	//WinContainer.ShowWindow(SW_SHOWNA);
//}
//
//
//void GameExplorerWin::ReDraw()
//{
//// 	if ( bbbb)
//// 	{
//// 		WinContainer.Invalidate();
//// 	}
//}