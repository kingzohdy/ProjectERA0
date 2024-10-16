#pragma once

class CClient_Image
{
public:
	HBITMAP				m_hBitmap;
	HDC					m_hDC;
	SIZE				m_Size;
	
public:
						CClient_Image();
	virtual				~CClient_Image();

	BOOL				Create(HDC hDestDC, CHAR* pFilename);
	BOOL				Create(HDC hDestDC, DWORD dwWidth, DWORD dwHeight);
	BOOL				Create(HDC hDestDC, HBITMAP hBitmap);
	void				Release(void);

	HDC					DC(void);
	SIZE*				Size(void);
	void				SetSize( int nWidth, int nHeight );

	void				Blt(HDC hDC, int nX, int nY);
	void				SBlt(HDC hDC, int nX, int nY, int nWidth, int nHeight);
	void				ABlt(HDC hDC, int nX, int nY, int nWidth, int nHeight, BYTE byAlpha);
	void				TBlt(HDC hDC, int nX, int nY, int nWidth, int nHeight, UINT uColor);
};