
#pragma once
#include "ObjectEditorTypes.h"
#include "NormalSplitterWnd.h"

class CBoneEditPane : public CWnd
{
	DECLARE_DYNAMIC(CBoneEditPane)

public:
	CBoneEditPane();
	virtual ~CBoneEditPane();

	CBCGPToolBar	m_wndToolBar;
	CNormalSplitterWnd m_wndSplitter;
	CTreeCtrl		m_wndBoneTree;
	CBCGPPropList	m_wndPropList;
	CSkeleton*		m_pSkeleton;

	CSplitterGridView* m_pGrid1;
	CSplitterGridView* m_pGrid2;

	CNotifiedProp* m_pPropID;
	CNotifiedProp* m_pPropName;
	CNotifiedProp* m_pPropFatherID;
protected:
	HTREEITEM m_hBone;
	HTREEITEM m_hAnchor;
	BOOL m_bPropEdited;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBonetreeSelchange(NMHDR* pHdr, LRESULT* pResult);
	afx_msg void OnBtnApplyBone();
	afx_msg void OnBtnShowBone();
	afx_msg void OnBtnUpdateAnchor();
	afx_msg void OnUpdateBtnShowbone(CCmdUI* pUI);

	bool IsBone(HTREEITEM hItem);
	void UpdateEffects(int anchor, HTREEITEM hItem);
	void SetEditTarget(CSkeleton* pTarget);
	void UpdateTree();
	void UpdateCurrentItem();
	void SaveCurrentItem(HTREEITEM itemOld);
	HTREEITEM FindItem(HTREEITEM hParentItem, LPARAM id);
	afx_msg void OnDestroy();

	afx_msg LRESULT OnPropEndEdit(WPARAM w, LPARAM l);
};


