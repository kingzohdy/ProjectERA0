// LauncherFlashDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Launcher.h"
#include "LauncherFlashDialog.h"
#include "utils.h"
#include <MsHTML.h>
#include <mshtmhst.h>


// CLauncherFlashDialog dialog

IMPLEMENT_DYNAMIC(CLauncherFlashDialog, CDialog)

CLauncherFlashDialog::CLauncherFlashDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLauncherFlashDialog::IDD, pParent)
{

}

CLauncherFlashDialog::~CLauncherFlashDialog()
{
}

void CLauncherFlashDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_web);
}


BEGIN_MESSAGE_MAP(CLauncherFlashDialog, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CLauncherFlashDialog message handlers

BOOL CLauncherFlashDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	ModifyStyleEx( 0, WS_EX_LAYERED );
	SetLayeredWindowAttributes( RGB(255,0,255), 255, LWA_ALPHA | LWA_COLORKEY );

	RECT rc = {0,0,0,0};

	m_web.SetWindowPos( NULL, 0,0, 0,0, SWP_NOZORDER );
	m_web.Navigate( "about:blank", NULL, NULL, NULL, NULL );

	return TRUE;
}

BOOL CLauncherFlashDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	CBrush	brush( RGB(255,0,255) );
	CRect	rc;
	GetClientRect( rc );
	pDC->FillRect( rc, &brush );
	return FALSE;
}

void CLauncherFlashDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
}
BEGIN_EVENTSINK_MAP(CLauncherFlashDialog, CDialog)
	ON_EVENT(CLauncherFlashDialog, IDC_EXPLORER1, 259, CLauncherFlashDialog::DocumentCompleteExplorer1, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

class DocHostUIHandler : public IDocHostUIHandler 
{ 
public: 
	DocHostUIHandler()
		: refcount(1)
	{
	}
	STDMETHOD_(ULONG, AddRef)()
	{
		::InterlockedIncrement(&refcount);
		return refcount;
	}
	STDMETHOD_(ULONG, Release)()
	{
		if(refcount)
			::InterlockedDecrement(&refcount);
		return refcount;
	}
	STDMETHOD(QueryInterface)(REFIID iid, LPVOID* ppvObj)
	{
		if(iid == IID_IUnknown)
		{
			*ppvObj = (IUnknown*) this;
			AddRef();
		}
		else if(iid == IID_IDocHostUIHandler)
		{
			*ppvObj = (IDocHostUIHandler*) this;
			AddRef();
		}
		else if(iid == IID_IDispatch)
		{
			*ppvObj = (IDispatch*) this;
			AddRef();
		}
		else
		{
			return E_NOINTERFACE;
		}
		return S_OK;
	}
	STDMETHOD(ShowContextMenu)(DWORD, LPPOINT, LPUNKNOWN, LPDISPATCH)
	{
		return S_OK;
	}
	STDMETHOD(GetHostInfo)(DOCHOSTUIINFO* pInfo)
	{
		pInfo->cbSize = sizeof(DOCHOSTUIINFO);   
		pInfo->dwFlags = DOCHOSTUIFLAG_DIALOG |    
			DOCHOSTUIFLAG_THEME  |    
			DOCHOSTUIFLAG_NO3DBORDER |    
			DOCHOSTUIFLAG_SCROLL_NO;
		pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
		return S_OK;
	}
	STDMETHOD(ShowUI)(DWORD, LPOLEINPLACEACTIVEOBJECT,
		LPOLECOMMANDTARGET, LPOLEINPLACEFRAME, LPOLEINPLACEUIWINDOW)
	{
		return S_OK;
	}
	STDMETHOD(HideUI)(void)
	{
		return S_OK;
	}
	STDMETHOD(UpdateUI)(void)
	{
		return S_OK;
	}
	STDMETHOD(EnableModeless)(BOOL)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnDocWindowActivate)(BOOL)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(OnFrameWindowActivate)(BOOL)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(ResizeBorder)(LPCRECT, LPOLEINPLACEUIWINDOW, BOOL)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(TranslateAccelerator)(LPMSG, const GUID*, DWORD)
	{
		return S_OK;
	}
	STDMETHOD(GetOptionKeyPath)(OLECHAR **, DWORD)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(GetDropTarget)(LPDROPTARGET, LPDROPTARGET*)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(GetExternal)(LPDISPATCH*)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(TranslateUrl)(DWORD, OLECHAR*, OLECHAR **)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(FilterDataObject)(LPDATAOBJECT , LPDATAOBJECT*)
	{
		return E_NOTIMPL;
	}
private:
	long refcount;
};
static DocHostUIHandler ms_DocHostUIHandler;

void CLauncherFlashDialog::DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL)
{
	// TODO: Add your message handler code here

	CString str( URL->bstrVal );
	if( str != "about:blank" )
	{
		m_web.SetWindowPos( NULL, 0,0, 388,75	, SWP_NOZORDER );
	}else{
		IHTMLDocument2* pDoc;
		IHTMLElement* pEl;
		IHTMLBodyElement * pBodyEl;	
		if (SUCCEEDED(m_web.get_Document()->QueryInterface(IID_IHTMLDocument2, (void**)&pDoc)))
		{
			ICustomDoc *pCustomDoc;
			if (SUCCEEDED(pDoc->QueryInterface(IID_ICustomDoc, (void **) &pCustomDoc)))
			{
				pCustomDoc->SetUIHandler(&ms_DocHostUIHandler);
			}
			if (SUCCEEDED(pDoc->get_body(&pEl)))
			{
				if (SUCCEEDED(pEl->QueryInterface(IID_IHTMLBodyElement, (void**)&pBodyEl)))
				{
					pBodyEl->put_scroll(BSTR(L"no"));
					pBodyEl->Release();
				}
				pBodyEl->Release();
			}
			pDoc->Release();
			string url = "http://www.ljy0.com/new_launcher_2013_5/showTab.html";
			m_web.Navigate(url.c_str(), NULL, NULL, NULL, NULL);
		}
	}
}
