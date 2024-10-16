
#include "SMailer.h"
#include "WinSockHelper.h"

#include "GameManager.h"
#include <Windows.h>
// #include "winexception.h"
#include "Splash.h"
#include "resource.h"
#include "OgreOSUtility.h"
#include "crashrpt/include/CrashRpt.h"
#include "winexception.h"
#include "OgreLog.h"
#include <vld.h>

#ifdef _DEBUG
#pragma comment(lib, "OgreMain_d.lib")
#pragma comment(lib, "GameEngine_d.lib")
#pragma comment(lib, "UILib_d.lib")
#pragma comment(lib, "luadll_d.lib")
#pragma comment(lib, "tolua++_d.lib")
#pragma comment(lib, "CrashRpt1300d.lib")
#else
#pragma comment(lib, "OgreMain.lib")
#pragma comment(lib, "GameEngine.lib")
#pragma comment(lib, "UILib.lib")
#pragma comment(lib, "luadll.lib")
#pragma comment(lib, "tolua++.lib")
#pragma comment(lib, "CrashRpt1300.lib")
#endif

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Imm32.lib")
#pragma comment(lib, "atl")

#define SPLASHBOTTOM 375   //图片进度条地部的起始像素 
#define SPLASHLENGTH 140   //图片进度条的长度
#define SPLASHFONHEIGHT 23 //字体中心离窗体地面的高度
#define ALLPERCENT 100
bool bOnDestory = false;
SplashStruct SplStruct;
LPCRITICAL_SECTION lprogress;
HFONT hSplashfont;

// 回调
HRESULT CALLBACK SplashWndProc (HWND hWnd, UINT Msg, WPARAM wParam, 
								LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	HBITMAP hBmp,hBackBmp;
	BITMAP Bmp,BackBmp;
	short int ID_SPLASHTIME;
	char* szSplashCls;
	HINSTANCE hInst;
	static int nLoad=SPLASHBOTTOM;  //进度加载高度
	HFONT hFont,hFontOld;
	HBITMAP hMaskBMP;
	std::string strTemp;

	hInst   =SplStruct.SplashInst;
	szSplashCls  =SplStruct.szSplashCls;
	ID_SPLASHTIME =SplStruct.SplashTime;
	hBmp   =SplStruct.hSplashBmp;
	Bmp    =SplStruct.SplashBmp;
	hBackBmp=SplStruct.m_hSplashBackBmp;
	BackBmp=SplStruct.m_SplashBackBmp;
	hFont=hSplashfont;

	switch (Msg)
	{
	case WM_PAINT://处理闪屏图像的显示
		EnterCriticalSection(lprogress);
		if ( nLoad >= SplStruct.m_nSplashPercent) 
		{
			nLoad--;
		}
		strTemp = SplStruct.m_strSplash;
		LeaveCriticalSection(lprogress);//获取显示的文字即进度刷新的高度

		hDC = BeginPaint(hWnd, &ps);
		RECT rect;
		GetClientRect(hWnd, &rect);
		HDC hImageDC,hImageBackDC;
		hImageDC = CreateCompatibleDC(hDC);
		if (hImageDC == NULL)
			return false;
		hImageBackDC = CreateCompatibleDC(hDC);
		if (hImageBackDC == NULL)
			return false;

		static HBITMAP hOldBitmap,hOldBackBitmap;
		hOldBitmap = (HBITMAP)SelectObject(hImageDC, hBmp);	
		hOldBackBitmap = (HBITMAP)SelectObject(hImageBackDC, hBackBmp);
		SetBkMode(hImageBackDC,TRANSPARENT);
		SetTextColor(hImageBackDC,RGB(255,255,255));
		SetTextAlign(hImageBackDC,TA_CENTER);                      //设定子体显示模式即颜色
		hFontOld = (HFONT)SelectObject(hImageBackDC, hFont);       //选择字体

		BitBlt(hImageBackDC, 0, nLoad, BackBmp.bmWidth, BackBmp.bmHeight, hImageDC, 0, nLoad, SRCCOPY);
		TextOut(hImageBackDC,BackBmp.bmWidth/2,BackBmp.bmHeight-SPLASHFONHEIGHT,strTemp.c_str(),strTemp.size());
		BitBlt(hDC, 0, 0, BackBmp.bmWidth, BackBmp.bmHeight, hImageBackDC, 0, 0, SRCCOPY);//模拟双缓冲

		SelectObject(hImageBackDC, hFontOld);
		SelectObject(hImageDC, hOldBitmap);
		SelectObject(hImageBackDC, hOldBackBitmap);
		EndPaint(hWnd, &ps);
		break;
	default:
		return (int)DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return 0;

}

// 注册
bool RegisterSplashClass ()
{
	HINSTANCE hInst;
	WNDCLASS wcl;
	char*  szSplashCls;
	hInst = SplStruct.SplashInst;
	//wcl  = SplStruct.Splashwcl;
	szSplashCls = SplStruct.szSplashCls;
	wcl.style = CS_HREDRAW | CS_VREDRAW;
	wcl.lpfnWndProc =(WNDPROC)SplashWndProc; 
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hInstance = hInst;
	wcl.hIcon  = NULL;
	wcl.hCursor  = NULL;
	wcl.hbrBackground = NULL;
	wcl.lpszMenuName = NULL;
	wcl.lpszClassName = szSplashCls;
	if (RegisterClass(&wcl) == 0)
		return false;
	
	return true;
}

// 创建
bool CreateScreenSplash ()
{
	HINSTANCE SplashInst;
	HWND hParenWnd;
	char* szSplashCls;
	HBITMAP hSplashBmp;
	BITMAP SplashBmp;
	int xScreen,yScreen;
	szSplashCls=SplStruct.szSplashCls;
	SplashInst =SplStruct.SplashInst; 
	hParenWnd =SplStruct.hParent;
	xScreen  = GetSystemMetrics(SM_CXFULLSCREEN);
	yScreen  = GetSystemMetrics(SM_CYFULLSCREEN);
	SplStruct.m_nSplashPercent=SPLASHBOTTOM;
	SplStruct.m_strSplash="初始化开始...";
	
	hSplashBmp =SplStruct.hSplashBmp;
	GetObject(hSplashBmp, sizeof(SplashBmp), &SplashBmp);
	SplStruct.SplashBmp =SplashBmp;
	hSplashBmp =SplStruct.m_hSplashBackBmp;
	GetObject(hSplashBmp, sizeof(SplashBmp), &SplashBmp);
	SplStruct.m_SplashBackBmp =SplashBmp;
	
	hSplashfont = CreateFont(
		12,      //字体的逻辑高度
		0,       //逻辑平均字符宽度
		0,           //与水平线的角度
		0,           //基线方位角度
		FW_NORMAL,  //字形：常规
		FALSE,       //字形：斜体
		FALSE,       //字形：下划线
		FALSE,       //字形：粗体
		GB2312_CHARSET,          //字符集
		OUT_DEFAULT_PRECIS,      //输出精度
		CLIP_DEFAULT_PRECIS,     //剪截精度
		PROOF_QUALITY,           //输出品质
		FIXED_PITCH | FF_MODERN, //倾斜度
		"宋体"                   //字体
		); 
	
	const char* pszWindowName = NULL;
	if ( GetACP() == 936 )
	{
		pszWindowName = "零纪元";
	}
	else
	{
		pszWindowName = "Era Zero";
	}
	SplStruct.hSplashWnd=CreateWindowEx(
		WS_EX_APPWINDOW,
		szSplashCls,
		pszWindowName,
		WS_POPUP | WS_VISIBLE,
		(xScreen - SplStruct.SplashBmp.bmWidth)/2,
		(yScreen - SplStruct.SplashBmp.bmHeight)/2,
		SplStruct.SplashBmp.bmWidth, 
		SplStruct.SplashBmp.bmHeight,
		hParenWnd,
		NULL,
		SplashInst,
		NULL);
	if(SplStruct.hSplashWnd==NULL)
		return false;

	//SetWindowPos(SplStruct.hSplashWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);

	UpdateWindow(SplStruct.hSplashWnd);
	ShowWindow(SplStruct.hSplashWnd, 1);
	return true;
}

//更新
void UpdateScreenSplash(const char *TempCls,int nPercent)
{
	float fTemp;
	EnterCriticalSection(lprogress);
	SplStruct.m_strSplash=TempCls;
	fTemp=static_cast<float>(nPercent);
	fTemp=SPLASHBOTTOM-fTemp*SPLASHLENGTH/ALLPERCENT;
	SplStruct.m_nSplashPercent=static_cast<int>(fTemp);
	LeaveCriticalSection(lprogress);
}

// 隐藏
void DestoryScreenSplash ()
{
	//处理闪屏图像的隐藏
	HWND  hSplashWnd;
	HINSTANCE SplashInst;
	char* szSplashCls;
	HBITMAP hSplashBmp;
	SplashInst = SplStruct.SplashInst;
	hSplashWnd = SplStruct.hSplashWnd;
	szSplashCls = SplStruct.szSplashCls;
	hSplashBmp = SplStruct.hSplashBmp;

	if (IsWindow(SplStruct.hSplashWnd))
	{
		KillTimer(SplStruct.hSplashWnd,1);
		DestroyWindow(SplStruct.hSplashWnd);
		DeleteObject(SplStruct.hSplashBmp);
		DeleteObject(SplStruct.m_hSplashBackBmp);
		SplStruct.hSplashWnd=NULL;
		UnregisterClass(SplStruct.szSplashCls, SplashInst);
		DeleteCriticalSection(lprogress);
	}
}

LRESULT CALLBACK MyWinProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (bOnDestory)
		return DefWindowProc(hWnd, uiMsg, wParam, lParam);

	GameManager *pgame = GameManager::getSingletonPtr();
	if(pgame)
	{			return pgame->OnWindowMsg(hWnd, uiMsg, (unsigned int)wParam, (unsigned int)lParam);
	}
	else return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

bool g_bGameCreated = false;

DWORD WINAPI splashWorkFunction( void* data )
{
	SplStruct.SplashInst = 0;
	SplStruct.szSplashCls = "2012Splash";
	SplStruct.hSplashBmp = (HBITMAP) LoadImage(NULL, "resource\\DengLuTu.bmp", IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	SplStruct.m_hSplashBackBmp = (HBITMAP) LoadImage(NULL, "resource\\DengLuTuBeiJing.bmp", IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	// begin splash
	SplStruct.SplashTime = 0;
	SplStruct.hParent = 0;
	RegisterSplashClass();
	CreateScreenSplash();

	while(!g_bGameCreated)
	{
		::Sleep(65);
		//::SendMessage(SplStruct.hSplashWnd, WM_PAINT, 0, 0);
		InvalidateRect(SplStruct.hSplashWnd,NULL,true);
		UpdateWindow(SplStruct.hSplashWnd);
	}

	ShowWindow((HWND)SplStruct.hSplashWnd, SW_HIDE);
	DestoryScreenSplash();

	::Sleep(200);

	return 0;
}

BOOL WINAPI CrashCallback(LPVOID lpvState)
{
	EXCEPTION_POINTERS* pException = (EXCEPTION_POINTERS*)lpvState;
	CWinException e( pException );
	GameManager::getSingleton().PrintRunningInfo();
	MessageBox(NULL, "程序异常，错误信息已经发送给开发人员，会在第一时间处理解决", "错误", MB_OK);
// 	Ogre::LogRelease();
	return TRUE;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if( strcmp( lpCmdLine,"launcher") != 0 && strstr(lpCmdLine,"directmud") == NULL)
	{
		::ShellExecute( NULL, "open", "Launcher.exe", NULL, NULL, SW_SHOWNORMAL );
		return -1;
	}
	
	char szVer[32];
	unsigned int uiVer = getLocalVersion();
	sprintf(szVer,"%d.%d.%d",(uiVer>>24)&0xFF ,(uiVer>>16)&0xFF, uiVer&0xFFFF);
	CR_INSTALL_INFO info;
	memset(&info, 0, sizeof(CR_INSTALL_INFO));
	info.cb = sizeof(CR_INSTALL_INFO);			// Size of the structure
	info.pszAppName = "Client";					// App name
	info.pszAppVersion = szVer;					// App version
	info.pszErrorReportSaveDir = "log/";
 	info.pfnCrashCallback = CrashCallback;

	info.dwFlags |= CR_INST_NO_GUI;
	info.dwFlags |= CR_INST_ALL_POSSIBLE_HANDLERS;

	// Install crash handlers
#ifndef _DEBUG
	int nInstResult = crInstall(&info);
#endif

#ifndef BUILD_DEPLOY
	crAddScreenshot2(CR_AS_VIRTUAL_SCREEN|CR_AS_USE_JPEG_FORMAT, 100);
	crAddFile2("Client.exe", NULL, "Client.exe", CR_AF_TAKE_ORIGINAL_FILE);
	crAddFile2("Client.pdb", NULL, "Client.pdb", CR_AF_TAKE_ORIGINAL_FILE);
	crAddFile2("OgreMain.dll", NULL, "OgreMain.dll", CR_AF_TAKE_ORIGINAL_FILE);
	crAddFile2("OgreMain.pdb", NULL, "OgreMain.pdb", CR_AF_TAKE_ORIGINAL_FILE);
	crAddFile2("RenderSystem_Direct3D9.dll", NULL, "RenderSystem_Direct3D9.dll", CR_AF_TAKE_ORIGINAL_FILE );
	crAddFile2("RenderSystem_Direct3D9.pdb", NULL, "RenderSystem_Direct3D9.pdb", CR_AF_TAKE_ORIGINAL_FILE );
#else
	crAddScreenshot2(CR_AS_VIRTUAL_SCREEN|CR_AS_USE_JPEG_FORMAT, 5);
#endif
	crAddFile2("log\\all.log", NULL, "all log", CR_AF_TAKE_ORIGINAL_FILE);
	crAddFile2("log\\path.log", NULL, "path log", CR_AF_TAKE_ORIGINAL_FILE);

#ifdef _DEBUG
 	if(AllocConsole())
 	{
 		SetConsoleTitle("Debug Window");
 		freopen("CONOUT$","w", stdout);
 		freopen("CONIN$", "r", stdin);
 	}
#endif

	srand((unsigned)timeGetTime());

	/*SplStruct.SplashInst = 0;
	SplStruct.szSplashCls = "2012Splash";
	SplStruct.hSplashBmp = (HBITMAP) LoadImage(NULL, "resource\\登陆图.bmp", IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	SplStruct.hProgressbarBmp = (HBITMAP) LoadImage(NULL, "resource\\ProgressFore.bmp", IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	RegisterSplashClass();*/

	// begin splash
	//SplStruct.SplashTime = 0;
	//SplStruct.hParent = 0;
	//CreateScreenSplash();
	lprogress= new CRITICAL_SECTION;
	InitializeCriticalSection(lprogress);
	// begin Splash
	DWORD threadid;
	void * splashThread = (void*)CreateThread(NULL, 0, splashWorkFunction, 0, 0, &threadid);
	// end Splash

	WNDCLASSEX kWC;
	kWC.cbSize		  = sizeof(WNDCLASSEX);
	kWC.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	kWC.lpfnWndProc   = MyWinProc;
	kWC.cbClsExtra    = 0;
	kWC.cbWndExtra    = 0;
	kWC.hInstance     = hInstance;
	kWC.hIcon         = LoadIcon(NULL, MAKEINTRESOURCE(IDI_CLIENT));
	kWC.hCursor       = LoadCursor(NULL, IDC_ARROW);
	kWC.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	kWC.lpszClassName = "GameManagerWin";
	kWC.lpszMenuName  = 0;
	kWC.hIconSm		 = LoadIcon(kWC.hInstance, MAKEINTRESOURCE(IDI_CLIENT));
	RegisterClassEx(&kWC);

	char curdir[256];
	::GetCurrentDirectory(255, curdir);
	GameManager game;
	if (!game.checkInstanceNum())
	{
		MessageBox(NULL, "同时运行客户端数量已达最大数量", "错误", MB_OK);
		return -1;
	}
	if (strstr(lpCmdLine,"directmud") != NULL)
	{
		game.setFromMud(true);
	}
	if (!game.Create("client.cfg", hInstance,(const char*)lpCmdLine))
	{
		MessageBox(NULL, "初始化游戏出错，请重新安装客户端。", "错误", MB_OK);
		return -1;
	}

	// 游戏创建完毕，销毁闪屏，现实主窗口
	g_bGameCreated = true;
	//ShowWindow((HWND)SplStruct.hSplashWnd, SW_HIDE);
	//DestoryScreenSplash();
//	CloseHandle(splashThread);

//	UINT flags = SWP_NOREDRAW|SWP_SHOWWINDOW;
//	RECT rect;
//	GetWindowRect((HWND)game.m_hWnd, &rect);
//	SetWindowPos((HWND)game.m_hWnd, HWND_TOPMOST, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, flags);

	ShowWindow((HWND)game.m_hWnd, SW_SHOW);
	UpdateWindow((HWND)game.m_hWnd);
	SetForegroundWindow( (HWND)game.m_hWnd );
	::SetWindowPos( (HWND)game.m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW );
	::SetWindowPos( (HWND)game.m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW );

	game.SetCreated(true);
	while(true)
	{
		MSG kMsg;
		if (PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
		{
			if (kMsg.message == WM_QUIT) break;

			TranslateMessage(&kMsg);
			DispatchMessage(&kMsg);
		}
		else
		{
			game.DoFrame();
		}
	}
	
	bOnDestory = true;
	game.Destroy();

#ifndef _DEBUG
	crUninstall();
#endif

	return 0;
}
