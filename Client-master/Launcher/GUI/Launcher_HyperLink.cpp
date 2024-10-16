#include "stdafx.h"

CClient_HyperLink::CClient_HyperLink()
{
	m_hWnd = 0;
}

void CClient_HyperLink::SetText(CHAR* pText)
{
	m_Text = pText;

	SelectObject(m_hDestDC, m_hFont);
	GetTextExtentPoint32(
		m_hDestDC,
		pText,
		(int)strlen(pText),
		&m_Size);
}
void CClient_HyperLink::SetNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	m_hWnd = hWnd;
	m_wParam = wParam;
	m_lParam = lParam;
}
void CClient_HyperLink::SetFont(HFONT hFont)
{
	m_hFont = hFont;
}
void CClient_HyperLink::SetColor(COLORREF Color)
{
	m_Color = Color;
}

void CClient_HyperLink::SetMouseOnColor( COLORREF Color )
{
	m_MouseOnColor = Color;
}

void CClient_HyperLink::SetNormalColor( COLORREF Color )
{
	m_NormalColor = Color;
}

SIZE* CClient_HyperLink::Size(void)
{
	return &m_Size;
}

BOOL CClient_HyperLink::OnMouseMove(int nX, int nY)
{
	POINT Point = {nX, nY};
	RECT Rect = {m_Pos.x, m_Pos.y, m_Pos.x + m_Size.cx, m_Pos.y + m_Size.cy};

	if(PtInRect(&Rect, Point))
	{
		SetCursor(LoadCursor(0, IDC_HAND));
		m_Color = m_MouseOnColor;
		return true;
	}
	else
	{
		m_Color = m_NormalColor;
	}

	return false;
}
BOOL CClient_HyperLink::OnLButtonDown(int nX, int nY)
{
	POINT Point = {nX, nY};
	RECT Rect = {m_Pos.x, m_Pos.y, m_Pos.x + m_Size.cx, m_Pos.y + m_Size.cy};

	if(PtInRect(&Rect, Point))
	{
		SetCursor(LoadCursor(0, IDC_HAND));

		if(m_hWnd)
		{
			PostMessage(m_hWnd, WM_COMMAND, m_wParam, m_lParam);
		}
		return true;
	}
	return false;
}
BOOL CClient_HyperLink::OnLButtonUp(int nX, int nY)
{
	POINT Point = {nX, nY};
	RECT Rect = {m_Pos.x, m_Pos.y, m_Pos.x + m_Size.cx, m_Pos.y + m_Size.cy};

	if(PtInRect(&Rect, Point))
	{
		SetCursor(LoadCursor(0, IDC_HAND));
		return true;
	}
	return false;
}

void CClient_HyperLink::OnPaint(HDC hDC)
{
	if(!hDC)
	{
		hDC = m_hDestDC;
	}

	::SelectObject(hDC, m_hFont);
	::SetTextColor(hDC, m_Color);
	::TextOut(hDC, m_Pos.x, m_Pos.y, m_Text.c_str(), (int)m_Text.size());
}
