#include "ui_framemgr.h"
#include "ui_webbrowerframe.h"

#include <Windows.h>
#include <atlbase.h>
CComModule _Module;
#include <atlwin.h>
#include "exdisp.h"
#include <comdef.h>
// #include "comutil.h"

IMPLEMENT_UIOBJECT_CLONE(WebBrowerFrame)

WebBrowerFrame::WebBrowerFrame(): m_pWindow(NULL)
{
	CoInitialize(NULL);
	AtlAxWinInit();
}

WebBrowerFrame::~WebBrowerFrame()
{
	OGRE_DELETE(m_pWindow);
	CoUninitialize();
}

int WebBrowerFrame::OnInputMessage( const Ogre::InputEvent &event )
{
	return Frame::OnInputMessage( event );
}

void WebBrowerFrame::CopyMembers(WebBrowerFrame *pdest)
{
	Frame::CopyMembers( pdest );
}

void WebBrowerFrame::OpenSite(const char* szUrl)
{
	if( !m_bShow )
	{
		return;
	}
	if( m_pWindow == NULL )
	{		
		m_pWindow = new CAxWindow;
		RECT rc;
		LPOLESTR pszName=OLESTR("shell.Explorer.2"); 		
		rc.top = m_AbsRect.m_Top;
		rc.bottom = m_AbsRect.m_Bottom;
		rc.left = m_AbsRect.m_Left;
		rc.right = m_AbsRect.m_Right;
		((CAxWindow*)m_pWindow)->Create( (HWND)g_pFrameMgr->getWndHandle(), rc, 0, WS_POPUP );	
		((CAxWindow*)m_pWindow)->CreateControl(pszName);
		POINT point = {0,0};
		ClientToScreen((HWND)g_pFrameMgr->getWndHandle(),&point);
		((CAxWindow*)m_pWindow)->SetWindowPos(NULL,point.x + m_AbsRect.m_Left, point.y + m_AbsRect.m_Top,
			0,0,SWP_NOSIZE|SWP_NOACTIVATE);
	}

	IWebBrowser2* iWebBrowser;
	VARIANT varMyURL;
	((CAxWindow*)m_pWindow)->QueryControl(__uuidof(IWebBrowser2),(void**)&iWebBrowser); 
	VariantInit(&varMyURL);
	varMyURL.vt = VT_BSTR;

	_bstr_t bs1(szUrl);	
	varMyURL.bstrVal = SysAllocString( CComBSTR(szUrl) );
	iWebBrowser->Navigate2(&varMyURL,0,0,0,0);
	VariantClear(&varMyURL);
	iWebBrowser->Release();
	((CAxWindow*)m_pWindow)->ShowWindow(SW_SHOWNORMAL);
}

void WebBrowerFrame::ShowWindow( bool bShow )
{
	if( m_pWindow )
	{
		((CAxWindow*)m_pWindow)->ShowWindow(bShow);
	}
}

void WebBrowerFrame::UpdateSelf(float deltatime)
{
	if ( !m_bShow )
	{
		return;
	}
	if( m_pWindow && ((CAxWindow*)m_pWindow)->IsWindowVisible() )
	{
		POINT point = {0,0};
		ClientToScreen((HWND)g_pFrameMgr->getWndHandle(),&point);
		((CAxWindow*)m_pWindow)->SetWindowPos(NULL,point.x + m_AbsRect.m_Left, point.y + m_AbsRect.m_Top,
			m_AbsRect.getWidth(),m_AbsRect.getHeight(),SWP_NOACTIVATE);
	}
}
