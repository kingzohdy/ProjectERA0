
#include "ScreenCap.h"
#include "OgreResourceManager.h"
#pragma comment(lib, "Rpcrt4.lib")

// This function is used for monitor enumeration
BOOL CALLBACK EnumMonitorsProc(HMONITOR hMonitor, HDC /*hdcMonitor*/, LPRECT lprcMonitor, LPARAM dwData)
{	  
	CScreenCapture* psc = (CScreenCapture*)dwData;

	MONITORINFOEX mi;
	HDC hDC = NULL;  
	HDC hCompatDC = NULL;
	HBITMAP hBitmap = NULL;
	BITMAPINFO bmi;
	int nWidth = 0;
	int nHeight = 0;
	int nRowWidth = 0;
	char* pRowBits = NULL;

	// Get monitor rect size
	nWidth = lprcMonitor->right - lprcMonitor->left;
	nHeight = lprcMonitor->bottom - lprcMonitor->top;

	// Get monitor info
	mi.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(hMonitor, &mi);

	// Get the device context for this monitor
	hDC = CreateDC("DISPLAY", mi.szDevice, NULL, NULL); 
	if(hDC==NULL)
		goto cleanup;

	hCompatDC = CreateCompatibleDC(hDC);
	if(hCompatDC==NULL)
		goto cleanup;

	hBitmap = CreateCompatibleBitmap(hDC, nWidth, nHeight);
	if(hBitmap==NULL)
		goto cleanup;

	SelectObject(hCompatDC, hBitmap);

	int i;
	for(i=0; i<(int)psc->m_arcCapture.size(); i++)
	{
		RECT rc = psc->m_arcCapture[i];
		RECT rcIntersect;
		if(IntersectRect(&rcIntersect, lprcMonitor, &rc))
		{
			BOOL bBitBlt = BitBlt(hCompatDC, rc.left-lprcMonitor->left, rc.top-lprcMonitor->top, 
				rc.right - rc.left, rc.bottom - rc.top, hDC, rc.left-lprcMonitor->left, rc.top-lprcMonitor->top, SRCCOPY|CAPTUREBLT);
			if(!bBitBlt)
				goto cleanup;
		}
	}

	// Draw mouse cursor.
	if(PtInRect(lprcMonitor, psc->m_ptCursorPos))
	{						
		if(psc->m_CursorInfo.flags == CURSOR_SHOWING)
		{
			ICONINFO IconInfo;
			GetIconInfo((HICON)psc->m_CursorInfo.hCursor, &IconInfo);
			int x = psc->m_ptCursorPos.x - lprcMonitor->left - IconInfo.xHotspot;
			int y = psc->m_ptCursorPos.y - lprcMonitor->top  - IconInfo.yHotspot;
			DrawIcon(hCompatDC, x, y, (HICON)psc->m_CursorInfo.hCursor);
			DeleteObject(IconInfo.hbmMask);
			DeleteObject(IconInfo.hbmColor);
		}				
	}

	GUID* pguid = new GUID;
	unsigned char* pszUuid = 0;
	HRESULT hr = CoCreateGuid(pguid);
	if(SUCCEEDED(hr))
	{
		hr = UuidToString(pguid, &pszUuid);
		if(SUCCEEDED(hr) && pszUuid!=NULL)
		{
			memset(&bmi.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
			bmi.bmiHeader.biWidth = nWidth;
			bmi.bmiHeader.biHeight = nHeight;
			bmi.bmiHeader.biBitCount = 24;
			bmi.bmiHeader.biPlanes = 1;
			nRowWidth = nWidth*nHeight*3 + 10;
			pRowBits = new char[nRowWidth];
			char szPath[MAX_PATH];
			sprintf(szPath,"log/%s.jpg",(char*)pszUuid);
			int nFetched = GetDIBits(hCompatDC, hBitmap, i, nHeight, pRowBits, &bmi, DIB_RGB_COLORS);

			Ogre::ResourceManager::getSingleton().GenPngData(szPath,nWidth,nHeight,3,pRowBits);
			psc->m_out_file_list.push_back((char*)pszUuid);
			RpcStringFreeA(&pszUuid);
		}		
	}
	delete pguid;	

cleanup:
	// Clean up
	if(hDC)
		DeleteDC(hDC);

	if(hCompatDC)
		DeleteDC(hCompatDC);

	if(hBitmap)
		DeleteObject(hBitmap);

	if(pRowBits)
		delete [] pRowBits;

	// Next monitor
	return TRUE;
}

CScreenCapture::CScreenCapture()
{
}

CScreenCapture::~CScreenCapture()
{
}

bool CScreenCapture::CaptureScreenRect( std::vector<RECT> arcCapture, 
									   std::vector<std::string>& out_file_list)
{	
	// Init output variables
	out_file_list.clear();

	// Set internal variables
	m_arcCapture = arcCapture;
	m_out_file_list.clear();

	// Get cursor information
	GetCursorPos(&m_ptCursorPos);
	m_CursorInfo.cbSize = sizeof(CURSORINFO);
	GetCursorInfo(&m_CursorInfo);

	// Perform actual capture task inside of EnumMonitorsProc
	EnumDisplayMonitors(NULL, NULL, EnumMonitorsProc, (LPARAM)this);	

	// Return
	out_file_list = m_out_file_list;
	return TRUE;
}

// Gets rectangle of the virtual screen
void CScreenCapture::GetScreenRect(LPRECT rcScreen)
{
	int nWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int nHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	rcScreen->left = GetSystemMetrics(SM_XVIRTUALSCREEN);
	rcScreen->top = GetSystemMetrics(SM_YVIRTUALSCREEN);
	rcScreen->right = rcScreen->left + nWidth;
	rcScreen->bottom = rcScreen->top + nHeight;
}
