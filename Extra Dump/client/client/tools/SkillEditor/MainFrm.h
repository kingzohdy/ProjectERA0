// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "FightWorkSpace.h"
#include "CreatSkillWorkSpace.h"

#include "OutputBar.h"
#include "ChildView.h"
#include <Windows.h>
#include "Mmsystem.h"
#include "SkillValueCal.h"

class CMainFrame : public CBCGPFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();

	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	void DoFrame();

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CChildView				m_wndView;				// йсм╪

	CBCGPStatusBar			m_wndStatusBar;
	CBCGPMenuBar			m_wndMenuBar;
	CBCGPToolBar			m_wndToolBar;
	
	SkillValueCal			m_SkillValueCal;

	CBCGPToolBarImages	m_UserImages;
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	void OnToolsViewUserToolbar (UINT id);
	void OnUpdateToolsViewUserToolbar (CCmdUI* pCmdUI);
	afx_msg void OnAppLook(UINT id);
	afx_msg void OnUpdateAppLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()


	UINT	m_nAppLook;

public:
	CFightWorkSpace			m_wndFightWorkSpace;
	CCreatSkillWorkSpace	m_wndCreatSkillWorkSpace;
	COutputBar				m_wndOutput;
};


