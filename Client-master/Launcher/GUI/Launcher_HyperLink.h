#pragma once

#include <string>

class CClient_HyperLink : public CClient_Control
{
public:
	std::string			m_Text;
	SIZE				m_Size;
	
	HFONT				m_hFont;
	COLORREF			m_Color;
	COLORREF			m_MouseOnColor;
	COLORREF			m_NormalColor;

	HWND				m_hWnd;
	WPARAM				m_wParam;
	LPARAM				m_lParam;

public:
							CClient_HyperLink();

	void					SetText(CHAR* pText);
	void					SetNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void					SetFont(HFONT hFont);
	void					SetColor(COLORREF Color);
	void					SetMouseOnColor( COLORREF Color );
	void					SetNormalColor( COLORREF Color );

	SIZE*					Size(void);

	BOOL					OnMouseMove(int nX, int nY);
	BOOL					OnLButtonDown(int nX, int nY);
	BOOL					OnLButtonUp(int nX, int nY);

	void					OnPaint(HDC hDC = 0);
};