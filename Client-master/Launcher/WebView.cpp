#include "WebView.h"

CWebView::CWebView(void)
{
}

CWebView::~CWebView(void)
{
}

void CWebView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();

	// TODO: This code navigates to a popular spot on the web.
	//  change the code to go where you'd like.
	Navigate2( _T("http://www.msdn.microsoft.com/vstudio/"), NULL,NULL );
}

