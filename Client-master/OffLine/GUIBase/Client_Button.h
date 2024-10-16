#pragma once

#include <string>
#include "Client_Control.h"

class CClient_Button : public CClient_BitmapControl
{
public:
	enum STATE
	{
		ST_NORMAL = 0,
		ST_FOCUS,
		ST_DOWN,
		ST_DISABLE,

		//ST_COUNT,
	};

public:
	typedef	std::basic_string<TCHAR>	LPSTRING;
	STATE				m_State;
	LPSTRING			m_strCaption;
	bool				m_bDown;
	bool				m_bShow;
	bool				m_bChecked;

	HWND				m_hParentWnd;
	WPARAM				m_wParam;
	LPARAM				m_lParam;

	CClient_Image*		m_pHideImage;
	COLORREF			m_crColor;
	int					m_nFontWidth;

public:
	CClient_Button();

	void				Disable();
	void				Enable();
	bool				IsEnable( void );

	void				Show();
	void				Hide();
	bool				IsShown();

	void				SetNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);

	bool				IsPointIn( POINT pt );
	bool				IsPointIn( int nX, int nY );

	BOOL				OnMouseMove(int nX, int nY);
	BOOL				OnLButtonDown(int nX, int nY);
	BOOL				OnLButtonUp(int nX, int nY);
	BOOL				OnLButtonDblClk(int nX, int nY);

	void				OnPaint( HDC hDC = NULL );
	void				OnPaint_Mask( HDC hDC );

	BOOL				IsSelect( CPoint point );

	void				Rest(CPoint point);

	void                GetWindowRect( LPRECT rt );

	void				SetCaption( LPSTRING strCaption, COLORREF crColor = RGB( 255, 255, 0 ), 
		int nFontWidth = 12 );

	void				Checked();
	void				DisChecked();
	bool				IsChecked();
	void				SetTextColor( COLORREF crColor );
	void				SetFontWidth( int nFontWidth );

private:
	void DrawFont( HDC hDC, COLORREF crColor, LPSTRING strCaption, RECT& rect, int nFontWidth );
};