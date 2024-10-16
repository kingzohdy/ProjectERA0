
#pragma once

#include <Windows.h>
#include <string>

// 结构
struct SplashStruct
{
	HWND hParent;
	HWND hSplashWnd;
	HINSTANCE SplashInst;
	WNDCLASS Splashwcl;
	char *szSplashCls;
	HBITMAP hSplashBmp;
	BITMAP SplashBmp;
	HBITMAP m_hSplashBackBmp;
	BITMAP m_SplashBackBmp;
	UINT IDB_BITMAP;
	int SplashTime;
	std::string m_strSplash;
	int m_nSplashPercent;
};

// 回调
HRESULT CALLBACK SplashWndProc (HWND hWnd, UINT Msg, WPARAM wParam, 
								LPARAM lParam);

// 注册
bool RegisterSplashClass ();


// 创建
bool CreateScreenSplash ();

// 隐藏
void DestoryScreenSplash ();

//更新进度
void UpdateScreenSplash(const char *TempCls,int nPercent);

extern SplashStruct SplStruct;
extern LPCRITICAL_SECTION lprogress;