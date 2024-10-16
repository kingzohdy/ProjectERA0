#pragma once

#include <string>

class CClient_Progress : public CClient_BitmapControl
{
public:
	BYTE				m_byPercent;
	CClient_Image*		m_pHideImage;

	// Alpha进度条使用
	int					m_iAlphaStart, m_iAlphaEnd;
	float				m_fCurrAlpha, m_f;
	CClient_Image*		m_pAlphaImage;
	BITMAP				m_bmData, m_bmMask, m_bmAlpha;
	BYTE				*m_pData, *m_pMask, *m_pAlpha;
	BYTE				*m_pTransData, *m_pTransData2;
	HWND				m_hWnd;
	std::string			m_strCaption;

public:
	CClient_Progress();
	virtual ~CClient_Progress();

	bool InitAlpha();
	void SetPercent(BYTE byPercent);
	BYTE Percent(void) const;
	void SetAlphaRange(int iStart, int iEnd);
	void SetHwnd( HWND hWnd );
	void SetInfo( const std::string& strCaption );
	std::string GetInfo() const;

	void Draw(void);
	void DrawMask(HDC hDC = 0);
	void DrawAlpha(HDC hDC = 0);
};