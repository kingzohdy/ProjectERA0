
#pragma once
#include "ObjectEditorTypes.h"
#include "NormalSplitterWnd.h"

class CAnchorPane : public CWnd
{
	DECLARE_DYNAMIC(CAnchorPane)

public:
	CAnchorPane();
	virtual ~CAnchorPane();

	CBCGPToolBar	m_wndToolBar;
	CNormalSplitterWnd m_wndSplitter;
	CTreeCtrl		m_wndTree;
	CBCGPPropList	m_wndPropList;
	CAnchorSet*		m_pAnchor;

	CSplitterGridView* m_pGrid1;
	CSplitterGridView* m_pGrid2;


	CNotifiedProp* m_pPropId;
	CNotifiedProp* m_pPropBone;
	CNotifiedProp* m_pPropOffset;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void SetEditTarget(CAnchorSet* pAnchor);
	void UpdateTree();
	void UpdateCurrentItem();

	void UpdateAnchor();
	void UpdateEffects(int anchor, HTREEITEM hItem);

	void OnAchortreeSelchange(NMHDR* pHdr, LRESULT* pResult);

	void CreateAnchorProp(int anchorid);
	void UpdataAnchorProp(int anchorid);
};


