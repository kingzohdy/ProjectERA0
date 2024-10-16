// workspace2.h : interface of the CWorkSpaceBar2 class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "xml_uimgr.h"
#include "ui_editmgr.h"
#include "OgreInputHandler.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMyProplist:public CBCGPPropList
{
public: 
	CMyProplist();
	~CMyProplist();

	//override
	//void OnPropertyChanged(CBCGPProp* pProp);

};

class CWorkSpaceBar2 : public CBCGPDockingControlBar
{
public:
	CWorkSpaceBar2();

// Attributes
protected:
	//CTreeCtrl	m_wndTree;
	CMyProplist	m_wndPropList;
	bool m_bWorkspaceON;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkSpaceBar2)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWorkSpaceBar2();

// Generated message map functions
protected:
	//{{AFX_MSG(CWorkSpaceBar2)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg LRESULT GetMsg(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void DisplayProp(UIObject *pUIObj);
	bool SavePropToXML(const char * path);
	//void OnFocusUIObject(const Ogre::InputEvent &event);
	bool HasCurObj();
	UIObject * GetCurrentObj();
	void WorkSpaceON(bool bOn);
	bool WorkSpaceIsON();

private:
	void SetCurrentObj(UIObject *pUIObj);

	UIObject *m_CurObj;
};

/////////////////////////////////////////////////////////////////////////////
