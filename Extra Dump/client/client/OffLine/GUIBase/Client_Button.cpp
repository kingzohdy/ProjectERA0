#include "Client_Button.h"

const int ST_COUNT = 3;
const int DEFAULT_FONT_WIDTH = 12;

CClient_Button::CClient_Button()
{
	m_State			= ST_NORMAL;
	m_bDown			= false;
	m_hParentWnd	= NULL;
	m_pHideImage	= NULL;
	m_bShow			= false;
	m_bChecked		= false;
	m_crColor		= RGB( 255, 255, 0 );
	m_nFontWidth	= DEFAULT_FONT_WIDTH;
}

void CClient_Button::Disable( )
{	
	m_State = ST_DISABLE;
	// 重画
	OnPaint();
}

void CClient_Button::Enable()
{
	m_State = ST_NORMAL;
	// 重画
	OnPaint();
}

bool CClient_Button::IsEnable(void)
{
	return m_State == ST_NORMAL;
}

void CClient_Button::Show()
{
	m_bShow = true;
}
void CClient_Button::Hide()
{
	m_bShow = false;
}

bool CClient_Button::IsShown()
{
	return m_bShow;
}

void CClient_Button::SetNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	m_hParentWnd = hWnd;
	m_wParam = wParam;
	m_lParam = lParam;
}

bool CClient_Button::IsPointIn( POINT pt )
{
	SIZE* pSize = m_pImage->Size();
	RECT Rect = { m_Pos.x, m_Pos.y, m_Pos.x + pSize->cx, m_Pos.y + pSize->cy / ST_COUNT };

	return ( pt.x >= Rect.left && pt.x <= Rect.right && pt.y >= Rect.top && pt.y <= Rect.bottom );
}

bool CClient_Button::IsPointIn( int nX, int nY )
{
	SIZE* pSize = m_pImage->Size();
	RECT Rect = { m_Pos.x, m_Pos.y, m_Pos.x + pSize->cx, m_Pos.y + pSize->cy / ST_COUNT };

	return ( nX >= Rect.left && nX <= Rect.right && nY >= Rect.top && nY <= Rect.bottom );
}

BOOL CClient_Button::OnMouseMove( int nX, int nY )
{
	if(m_State == ST_DISABLE || !m_bShow )
	{
		return false;
	}

	STATE State = m_State;

	if ( IsPointIn( nX, nY )  )
	{
		if(m_bDown)
		{
			m_State = ST_DOWN;
		}
		else
		{
			m_State = ST_FOCUS;
		}
	}
	else
	{
		m_State = ST_NORMAL;
	}

	// 重画
	if(State != m_State)
	{
		OnPaint();
	}
	return m_State == ST_FOCUS || m_State == ST_DOWN;
}

BOOL CClient_Button::OnLButtonDown(int nX, int nY)
{
	if ( m_State == ST_DISABLE || !m_bShow )
	{
		return false;
	}

	STATE State = m_State;

	if ( IsPointIn( nX, nY ) )
	{
		m_bDown = true;
		m_State = ST_DOWN;
	}
	else
	{
		m_bDown = false;
		m_State = ST_NORMAL;
	}

	// 重画
	if ( State != m_State )
	{
		OnPaint();
	}
	return m_State == ST_DOWN;
}
BOOL CClient_Button::OnLButtonUp( int nX, int nY )
{
	if(m_State == ST_DISABLE || !m_bShow )
	{
		return false;
	}

	STATE State = m_State;

	if ( IsPointIn( nX, nY ) )
	{
		if( m_hParentWnd ) 
		{
			SendMessage( m_hParentWnd, WM_COMMAND, m_wParam, m_lParam );
		}
		m_State = ST_FOCUS;
	}
	else
	{
		m_State = ST_NORMAL;
	}
	m_bDown = false;

	// 重画
	if(State != m_State)
	{
		OnPaint();
	}
	return m_State == ST_FOCUS;
}

BOOL CClient_Button::OnLButtonDblClk(int nX, int nY)
{
	if(m_State == ST_DISABLE || !m_bShow )
	{
		return false;
	}

	STATE State = m_State;

	return IsPointIn( nX, nY );
}

void CClient_Button::OnPaint(HDC hDC)
{
	if ( !m_bShow )
	{
		return;
	}

	if(!hDC)
	{
		hDC = m_hDestDC;
	}

	if ( m_pHideImage != NULL )
	{
		OnPaint_Mask(hDC);
		return;
	}

	SIZE* pSize		= m_pImage->Size();
	DWORD dwHeight	= pSize->cy / ST_COUNT;

	RECT rect;
	rect.left	= m_Pos.x;
	rect.right	= m_Pos.x + pSize->cx;
	rect.top	= m_Pos.y;
	rect.bottom	= m_Pos.y + dwHeight;

	if ( m_pImage != NULL )
	{
		//BitBlt( hDC, m_Pos.x, m_Pos.y, pSize->cx, dwHeight, m_pImage->DC(), 0, m_State * dwHeight, SRCCOPY );
		TransparentBlt( hDC, m_Pos.x, m_Pos.y, pSize->cx, dwHeight, m_pImage->DC(), 0, m_State * dwHeight, 
			m_pImage->Size()->cx, dwHeight, RGB( 0xff, 0xff, 0xff ) );
	}

	DrawFont( hDC, m_crColor, m_strCaption, rect, m_nFontWidth );
}


void CClient_Button::OnPaint_Mask(HDC hDC)
{

	SIZE* pSize = m_pImage->Size();
	DWORD dwHeight = pSize->cy / ST_COUNT;

	RECT rect;
	rect.left	= m_Pos.x;
	rect.right	= m_Pos.x + pSize->cx;
	rect.top	= m_Pos.y;
	// TODO:
	rect.bottom	= m_Pos.y + dwHeight;

	if ( m_pImage != NULL )
	{
		//BitBlt(hDC, m_Pos.x, m_Pos.y, pSize->cx, dwHeight, m_pImage->DC(), 0, m_State * dwHeight, SRCPAINT );
		TransparentBlt( hDC, m_Pos.x, m_Pos.y, pSize->cx, dwHeight, m_pImage->DC(), 0, m_State * dwHeight, 
			m_pImage->Size()->cx, m_pImage->Size()->cy, RGB( 0xff, 0xff, 0xff ) );
	}

	if ( m_pHideImage != NULL )
	{
		//BitBlt( hDC, m_Pos.x, m_Pos.y, pSize->cx, dwHeight, m_pHideImage->DC(), 0, m_State * dwHeight, SRCAND );
		TransparentBlt( hDC, m_Pos.x, m_Pos.y, pSize->cx, dwHeight, m_pHideImage->DC(), 0, m_State * dwHeight, 
			m_pHideImage->Size()->cx, m_pHideImage->Size()->cy, RGB( 0xff, 0xff, 0xff ) );
		//BitBlt( hDC, m_Pos.x, m_Pos.y, pSize->cx, dwHeight, m_pHideImage->DC(), 0, 0, BLACKNESS );
	}

	//if ( m_pHideImage != NULL )
	//{
	//	BitBlt(hDC, m_Pos.x, m_Pos.y, pSize->cx, dwHeight, m_pHideImage->DC(), 0, m_State * dwHeight, SRCPAINT );
	//}

	//if ( m_pImage != NULL )
	//{
	//	BitBlt( hDC, m_Pos.x, m_Pos.y, pSize->cx, dwHeight, m_pImage->DC(), 0, 0, SRCAND );
	//	//BitBlt( hDC, m_Pos.x, m_Pos.y, pSize->cx, dwHeight, m_pHideImage->DC(), 0, 0, BLACKNESS );
	//}

	DrawFont( hDC, m_crColor, m_strCaption, rect, m_nFontWidth );
}

BOOL CClient_Button::IsSelect(CPoint point)
{
	SIZE* pSize = m_pImage->Size();
	CRect Rect = CRect( m_Pos.x, m_Pos.y, m_Pos.x + pSize->cx, m_Pos.y + pSize->cy / ST_COUNT );

	if ( Rect.PtInRect( point ) )
	{
		return true;
	}

	return false;
}

void CClient_Button::Rest( CPoint point )
{
	if ( m_State == ST_NORMAL || m_State == ST_DISABLE )
	{
		return;
	}

	// 若鼠标不在按钮上面
	if ( !IsPointIn( point ) )
	{
		m_bDown = false;
		m_State = ST_NORMAL;
		OnPaint( );
	}
}

void CClient_Button::GetWindowRect( LPRECT rt )
{
	rt->left	= m_Pos.x; 
	rt->top		= m_Pos.y;
	rt->right	= rt->left + m_pImage->Size()->cx;
	rt->bottom	= rt->top + (m_pImage->Size()->cy / ST_COUNT);
}

void CClient_Button::SetCaption( LPSTRING strCaption, COLORREF crColor, int nFontWidth )
{
	m_strCaption	= strCaption;
	m_crColor		= crColor;
	m_nFontWidth	= nFontWidth;
}

void CClient_Button::Checked()
{
	m_bChecked = true;
}

void CClient_Button::DisChecked()
{
	m_bChecked = false;
}

bool CClient_Button::IsChecked()
{
	return m_bChecked;
}

void CClient_Button::SetTextColor( COLORREF crColor )
{
	m_crColor = crColor;
}

void CClient_Button::SetFontWidth( int nFontWidth )
{
	m_nFontWidth = nFontWidth;
}

void CClient_Button::DrawFont( HDC hDC, COLORREF crColor, LPSTRING strCaption, RECT& rect, int nFontWidth )
{
	COLORREF crOld = ::SetTextColor( hDC, crColor );
	HFONT hNewFont = ::CreateFont( nFontWidth, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, GB2312_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT( "宋体" ) );
	int nOldMode = ::SetBkMode( hDC, TRANSPARENT );
	HGDIOBJ hOldFont = ::SelectObject( hDC, hNewFont );
	::DrawText( hDC, strCaption.c_str(), (int)strCaption.length(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS );

	// 恢复原来的字体
	::DeleteObject( SelectObject( hDC, hOldFont ) );
	// 恢复原来的背景模式
	::SetBkMode( hDC, nOldMode );
	// 恢复原来的颜色
	::SetTextColor( hDC, crOld );
}

