
#pragma once
#include <vector>
#include "ResourceDropTarget.h"

class CEditBar : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CEditBar)

public:
	CEditBar();
	virtual ~CEditBar();
	//CDefaultPage m_defaultPage;
	CBCGPToolBar m_wndToolBar;
	CBCGPListCtrl	m_wndEffectList;
	CResDropTarget m_dropTarget;
protected:
	DECLARE_MESSAGE_MAP()

public:
	void UpdateList(std::vector<CEffectUnit*>& m_effects);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	afx_msg void OnBtnNewEffect();
	afx_msg void OnBtnLoadEffect();
	afx_msg void OnInsertNorpar();
	afx_msg void OnInsertLight();
	afx_msg void OnInsertBillboard();
	afx_msg void OnInsertDecal();
	afx_msg void OnMnuShow();
	afx_msg void OnMnuHide();
	afx_msg void OnUpdateMnuShow(CCmdUI* pUI);
	afx_msg void OnUpdateMnuHide(CCmdUI* pUI);
	afx_msg void OnNMRclickEffectlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNmItemActive(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBtnApplyAll();
	afx_msg void OnMnuEditeffect();
	afx_msg LRESULT OnDropRes(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMnuInsertRibbon();
	afx_msg void OnMnuInsertBeam();
	afx_msg void OnMnuInsertSound();
	afx_msg void OnMnuSetIndependent();
	afx_msg void OnUpdateMnuSetIndependent(CCmdUI *pCmdUI);
	afx_msg void OnMnuSetIntegrate();
	afx_msg void OnUpdateMnuSetIntegrate(CCmdUI *pCmdUI);
	afx_msg void OnMnuInsertGeom();
	afx_msg void OnRefreshList();
	afx_msg void OnMnuInsertVirtual();
	afx_msg void OnBtnMoNewPost();
};


