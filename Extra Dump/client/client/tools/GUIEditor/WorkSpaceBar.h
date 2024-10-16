// workspacebar.h : interface of the CWorkSpaceBar class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "xml_uimgr.h"

class XMLManager;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWorkSpaceBar : public CBCGPDockingControlBar
{
public:
	CWorkSpaceBar();

// Attributes
//protected:
	CTreeCtrl	m_wndTree;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkSpaceBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWorkSpaceBar();

// Generated message map functions
protected:
	//virtual void DoDataExchange(CDataExchange* pDX); 
	//{{AFX_MSG(CWorkSpaceBar)
	afx_msg void OnTvnSelchangedObjTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	bool TocFileOpen();

	

private:
	XMLManager *m_pXmlmgr;
	
	//std::vector <std::string> xmlFileSet;
	//std::vector <std::string>::iterator xmlFileIter;
public:
	std::string m_strCurObjName;
	std::string m_strLastObjName;
//	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

/////////////////////////////////////////////////////////////////////////////
