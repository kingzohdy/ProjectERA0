#include "stdafx.h"
	
CClient_Image::CClient_Image()
{
	m_hDC		= 0;
	m_hBitmap	= NULL;
	m_Size.cx	= 0;
	m_Size.cy	= 0;
}
CClient_Image::~CClient_Image()
{
	Release();
}

BOOL CClient_Image::Create(HDC hDestDC, CHAR* pFilename)
{
	DeleteObject(m_hBitmap);

	m_hBitmap = (HBITMAP)LoadImage(
		AfxGetApp()->m_hInstance, 
		pFilename,
		IMAGE_BITMAP,
		0,
		0,
		0);

    if(!m_hBitmap)
    {
	    m_hBitmap = (HBITMAP)LoadImage(
		    0, 
		    pFilename,
		    IMAGE_BITMAP,
		    0,
		    0,
		    LR_LOADFROMFILE); 
    }

	if(!m_hBitmap)
	{
		LPTSTR lpBuffer = NULL;
		::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM, 
			NULL, ::GetLastError(), LANG_NEUTRAL, ( LPTSTR )&lpBuffer, 0, NULL );
		if ( lpBuffer != NULL )
		{
			::OutputDebugString( lpBuffer );
		}
		return false;
	}
	
	if(!m_hDC)
	{
		m_hDC = CreateCompatibleDC(hDestDC);
	}
	SelectObject(m_hDC, m_hBitmap);

	BITMAP bm;   
	GetObject(m_hBitmap, sizeof(BITMAP), &bm);
	m_Size.cx = bm.bmWidth;
	m_Size.cy = bm.bmHeight;

	return true;
}
BOOL CClient_Image::Create(HDC hDestDC, DWORD dwWidth, DWORD dwHeight)
{
	DeleteObject(m_hBitmap);

	m_hBitmap = CreateCompatibleBitmap(hDestDC, dwWidth, dwHeight);
	if(!m_hBitmap)
	{
		return false;
	}

	if(!m_hDC)
	{
		m_hDC = CreateCompatibleDC(hDestDC);
	}
	SelectObject(m_hDC, m_hBitmap);

	m_Size.cx = dwWidth;
	m_Size.cy = dwHeight;

	return true;
}
BOOL CClient_Image::Create(HDC hDestDC, HBITMAP hBitmap)
{
	DeleteObject(m_hBitmap);

	m_hBitmap = hBitmap;

	if(!m_hDC)
	{
		m_hDC = CreateCompatibleDC(hDestDC);
	}
	SelectObject(m_hDC, m_hBitmap);

	BITMAP bm;   
	GetObject(m_hBitmap, sizeof(BITMAP), &bm);
	m_Size.cx = bm.bmWidth;
	m_Size.cy = bm.bmHeight;

	return true;
}
void CClient_Image::Release(void)
{
	DeleteDC(m_hDC);
	DeleteObject(m_hBitmap);
}

HDC CClient_Image::DC(void)
{
	return m_hDC;
}
SIZE* CClient_Image::Size(void)
{
	return &m_Size;
}

void CClient_Image::SetSize( int nWidth, int nHeight )
{
	m_Size.cx = nWidth;
	m_Size.cy = nHeight;	
}

void CClient_Image::Blt(HDC hDC, int nX, int nY)
{
	//BitBlt(hDC, nX, nY, m_Size.cx, m_Size.cy, m_hDC, 0, 0, SRCCOPY);
	TransparentBlt( hDC, nX, nY, m_Size.cx, m_Size.cy, m_hDC, 0, 0, 
		m_Size.cx, m_Size.cy, RGB( 0xff, 0xff, 0xff ) );
}
void CClient_Image::SBlt(HDC hDC, int nX, int nY, int nWidth, int nHeight)
{
	StretchBlt(hDC, nX, nY, nWidth, nHeight, m_hDC, 0, 0, m_Size.cx, m_Size.cy, SRCCOPY);
}
void CClient_Image::ABlt(HDC hDC, int nX, int nY, int nWidth, int nHeight, BYTE byAlpha)
{
	BLENDFUNCTION Blend;
	Blend.BlendOp = AC_SRC_OVER;
	Blend.BlendFlags = 0;
	Blend.SourceConstantAlpha = byAlpha;
	Blend.AlphaFormat = 0;
	AlphaBlend(hDC, nX, nY, nWidth, nHeight, m_hDC, 0, 0, m_Size.cx, m_Size.cy, Blend);
}
void CClient_Image::TBlt(HDC hDC, int nX, int nY, int nWidth, int nHeight, UINT uColor)
{
	TransparentBlt(hDC, nX, nY, nWidth, nHeight, m_hDC, 0, 0, m_Size.cx, m_Size.cy, uColor);
}
