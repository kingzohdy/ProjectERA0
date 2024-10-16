
#pragma once

#include <Windows.h>
#include <string>

// �ṹ
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

// �ص�
HRESULT CALLBACK SplashWndProc (HWND hWnd, UINT Msg, WPARAM wParam, 
								LPARAM lParam);

// ע��
bool RegisterSplashClass ();


// ����
bool CreateScreenSplash ();

// ����
void DestoryScreenSplash ();

//���½���
void UpdateScreenSplash(const char *TempCls,int nPercent);

extern SplashStruct SplStruct;
extern LPCRITICAL_SECTION lprogress;