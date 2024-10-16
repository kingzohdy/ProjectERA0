#if !defined(AFX_BCGPOUTLOOKCONTROLBAR_H__36D76712_E72A_496C_B227_A741E4A54169__INCLUDED_)
#define AFX_BCGPOUTLOOKCONTROLBAR_H__36D76712_E72A_496C_B227_A741E4A54169__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BCGPOutlookControlBar.h : header file
//

#include "BCGPBaseTabbedBar.h"
#include "BCGPOutlookBar.h"
/////////////////////////////////////////////////////////////////////////////
// CBCGPOutlookControlBar window

class BCGCBPRODLLEXPORT CBCGPOutlookControlBar : public CBCGPBaseTabbedBar
{
	DECLARE_DYNAMIC(CBCGPOutlookControlBar)
// Construction
public:
	CBCGPOutlookControlBar (BOOL bAutoDestroy = FALSE);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPOutlookControlBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBCGPOutlookControlBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPOutlookControlBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPOUTLOOKCONTROLBAR_H__36D76712_E72A_496C_B227_A741E4A54169__INCLUDED_)
