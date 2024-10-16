// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "WorkSpaceBar.h"
#include "WorkSpaceBar2.h"
#include "OutputBar.h"
#include "ChildView.h"
#include "ui_framemgr.h"
#include "ui_frame.h"
#include "ui_editui.h"


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
	void SaveXMLFile(const char * path);
// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members

	CChildView				m_wndView;

	CBCGPStatusBar			m_wndStatusBar;
	CBCGPMenuBar			m_wndMenuBar;
	CBCGPToolBar			m_wndToolBar;
	CWorkSpaceBar			m_wndWorkSpace;
	CWorkSpaceBar2			m_wndWorkSpace2;
	COutputBar				m_wndOutput;
	CBCGPDockingControlBar*	m_pwndUIPanel;

	CBCGPToolBarImages	m_UserImages;
// Generated message map functions
protected:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	void OnToolsViewUserToolbar (UINT id);
	void OnUpdateToolsViewUserToolbar (CCmdUI* pCmdUI);
	afx_msg void OnAppLook(UINT id);
	afx_msg void OnUpdateAppLook(CCmdUI* pCmdUI);
	afx_msg void OnXMLFileSaveAs();
	afx_msg LRESULT DisplaySelectedUI(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()


	UINT	m_nAppLook;

private:
	bool m_FileOpened;
public:
	afx_msg void OnFileOpen();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnUIGroup();

	BOOL CreateToolBox ();
};


