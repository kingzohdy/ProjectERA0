#pragma once
#include "MapViewInfoDlg.h"
#include "ResBrowerDlg.h"

// CInfoPanel

class CInfoPanel : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CInfoPanel)

public:
	CInfoPanel();
	virtual ~CInfoPanel();

public:
	CBCGPTabWnd	m_wndTabs;
	//CMapViewInfoDlg m_MapViewInfoDlg;
	CResBrowerDlg m_ResBrowerDlg;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


