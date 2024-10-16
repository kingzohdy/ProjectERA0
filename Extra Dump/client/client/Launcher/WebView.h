#pragma once

#include <afxhtml.h>
#include <afxext.h>
class CWebView : public CHtmlView
{
public:
	CWebView(void);
	~CWebView(void);
	void OnInitialUpdate();
	DECLARE_MESSAGE_MAP();
	DECLARE_DYNAMIC(CWeb);
};
