#include "stdafx.h"
#include "util/utils.h"

#define	SAFE_DELETE(p) { if (p) { delete [] p; p = NULL; } }

CClient_Progress::CClient_Progress()
{
	m_byPercent		= 0;
	m_pHideImage	= NULL;

	m_iAlphaStart	= 0;
	m_iAlphaEnd		= 100;
	m_fCurrAlpha	= 0.0f;
	m_pAlphaImage	= NULL;
	m_pData			= NULL;
	m_pMask			= NULL;
	m_pAlpha		= NULL;
	m_pTransData	= NULL;
	m_pTransData2	= NULL;
}

CClient_Progress::~CClient_Progress()
{
	SAFE_DELETE(m_pData);
	SAFE_DELETE(m_pMask);
	SAFE_DELETE(m_pAlpha);
	SAFE_DELETE(m_pTransData);
	SAFE_DELETE(m_pTransData2);
}

void CClient_Progress::SetAlphaRange(int iStart, int iEnd)
{
	m_iAlphaStart = iStart;
	m_iAlphaEnd	  = iEnd;
}

void CClient_Progress::SetPercent(BYTE byPercent)
{
	if ( byPercent > 100 )
	{
		byPercent = 100;
	}

	m_byPercent  = byPercent;
	m_fCurrAlpha = m_iAlphaStart + float(m_iAlphaEnd - m_iAlphaStart) * byPercent / 100;
	
	if ( m_pAlphaImage != NULL )
	{
		DrawAlpha();
	}
	else if( m_pHideImage|| m_pImage )
	{
		DrawMask();
	}
}

BYTE CClient_Progress::Percent( ) const
{
	return m_byPercent;
}

void CClient_Progress::SetHwnd( HWND hWnd )
{
	m_hWnd = hWnd;
}

void CClient_Progress::SetInfo( const std::string& strCaption )
{
	m_strCaption = strCaption;
}

std::string CClient_Progress::GetInfo() const
{
	return m_strCaption;
}

void CClient_Progress::Draw(void)
{
	SIZE* pSize = m_pImage->Size();
	
	DWORD dwFinishWidth = pSize->cx * m_byPercent / 100;
	DWORD dwHeight = pSize->cy / 2;

	BitBlt(
		m_hDestDC, 
		m_Pos.x,
		m_Pos.y,
		dwFinishWidth, 
		dwHeight, 
		m_pImage->DC(), 
		0, 
		dwHeight, 
		SRCCOPY);
	
	BitBlt(
		m_hDestDC, 
		m_Pos.x + dwFinishWidth, 
		m_Pos.y, 
		pSize->cx - dwFinishWidth, 
		dwHeight,
		m_pImage->DC(), 
		dwFinishWidth, 
		0, 
		SRCCOPY);
}

void CClient_Progress::DrawMask(HDC hDC)
{
	if ( !hDC )
	{
		hDC = m_hDestDC;
	}

	SIZE* pSize = m_pImage->Size();

	DWORD dwFinishWidth = pSize->cx * m_byPercent / 100;
	DWORD dwHeight = pSize->cy;

	if( m_pImage )
	{
		TransparentBlt(
			hDC, 
			m_Pos.x,
			m_Pos.y,
			dwFinishWidth, 
			dwHeight, 
			m_pImage->DC(), 
			0,
			0,
			dwFinishWidth,
			dwHeight,
			RGB(255,255,255) );
	}
	if( m_pHideImage )
	{
		BitBlt(
			hDC, 
			m_Pos.x,
			m_Pos.y,
			dwFinishWidth, 
			dwHeight, 
			m_pHideImage->DC(), 
			0, 
			0, 
			SRCAND);
	}
}

bool CClient_Progress::InitAlpha()
{
	if ( NULL == m_pImage->m_hBitmap ||
		(m_pHideImage != NULL && NULL == m_pHideImage->m_hBitmap) ||
		( m_pAlphaImage != NULL && NULL == m_pAlphaImage->m_hBitmap ) )
	{
		return false;
	}

	if ( m_pImage != NULL )
	{
		::GetObject(m_pImage->m_hBitmap, sizeof(BITMAP), &m_bmData);
	}
	
	if ( m_pHideImage != NULL )
	{
		::GetObject(m_pHideImage->m_hBitmap, sizeof(BITMAP), &m_bmMask);
	}
	
	if ( m_pAlphaImage != NULL )
	{
		::GetObject(m_pAlphaImage->m_hBitmap, sizeof(BITMAP), &m_bmAlpha);
	}
	
	if (memcmp(&m_bmData, &m_bmMask, sizeof(m_bmData) - 4) != 0)
		return false;
	if (memcmp(&m_bmData, &m_bmAlpha, sizeof(m_bmData) - 4) != 0)
		return false;

	SAFE_DELETE(m_pData);
	SAFE_DELETE(m_pMask);
	SAFE_DELETE(m_pAlpha);

	DWORD ld = m_bmData.bmHeight * m_bmData.bmWidthBytes;
	m_pData = new BYTE [ld];
	::GetBitmapBits(m_pImage->m_hBitmap, ld, m_pData);
	
	m_pMask = new BYTE [ld];
	::GetBitmapBits(m_pHideImage->m_hBitmap, ld, m_pMask);

	m_pAlpha = new BYTE [ld];
	::GetBitmapBits(m_pAlphaImage->m_hBitmap, ld, m_pAlpha);
	
	m_pTransData	= new BYTE [ld];
	m_pTransData2	= new BYTE [ld];

	return true;
}

void CClient_Progress::DrawAlpha(HDC hDC)
{
	if (NULL == m_pData || NULL == m_pMask || NULL == m_pAlpha)
	{
		return;
	}

	memset(m_pTransData, 0xff, m_bmData.bmHeight * m_bmData.bmWidthBytes);
	memset(m_pTransData2, 0x00, m_bmData.bmHeight * m_bmData.bmWidthBytes);
	int bpp = m_bmData.bmBitsPixel / 8;
	for (int y = 0; y < m_bmData.bmHeight; y++)
	{
		for (int x = 0; x < m_bmData.bmWidth; x++)
		{
			long offset = y * m_bmData.bmWidthBytes + x * bpp;
			float alpha = m_pAlpha[offset] / 255.0f * 100;
			if (alpha > m_fCurrAlpha)
			{
				continue;
			}


			switch(bpp)
			{
			case 4:		// bit32
			case 3:		// bit24
				if (m_pMask[offset + 0] > 240)
				{
					m_pTransData[offset + 0] = m_pData[offset + 0];
					m_pTransData[offset + 1] = m_pData[offset + 1];
					m_pTransData[offset + 2] = m_pData[offset + 2];

					m_pTransData2[offset + 0] = m_pMask[offset + 0];
					m_pTransData2[offset + 1] = m_pMask[offset + 1];
					m_pTransData2[offset + 2] = m_pMask[offset + 2];
				}
				break;
			case 2:		// bit16
				break;
			default:
				break;
			}
		}
	}

	::SetBitmapBits(m_pImage->m_hBitmap, m_bmData.bmHeight * m_bmData.bmWidthBytes, m_pTransData);
	//::SetBitmapBits(m_pHideImage->m_hBitmap, m_bmData.bmHeight * m_bmData.bmWidthBytes, m_pTransData2);


	if(!hDC)
	{
		hDC = m_hDestDC;
	}

	BitBlt(
		hDC, 
		m_Pos.x,
		m_Pos.y,
		m_bmData.bmWidth, 
		m_bmData.bmHeight, 
		m_pHideImage->DC(), 
		0, 
		0, 
		SRCPAINT);

	BitBlt(
		hDC, 
		m_Pos.x,
		m_Pos.y,
		m_bmData.bmWidth, 
		m_bmData.bmHeight, 
		m_pImage->DC(), 
		0, 
		0,
		SRCAND);
}
