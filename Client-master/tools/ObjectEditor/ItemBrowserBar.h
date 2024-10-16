
#pragma once
#include "GameResDBTypes.h"

class CItemViewerBar : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CItemViewerBar)

public:
	CItemViewerBar();
	virtual ~CItemViewerBar();

protected:
	CBCGPListCtrl	m_wndList;		//列表框
	CBCGPToolBar	m_wndToolBar;	//工具条
	CBCGPToolBar	m_wndSearchBar;	//工具条

	bool m_bReading;

	bool FitFilter_ItemType(ITEMDEF* pItem);
	bool FitFilter_ItemSlot(ITEMDEF* pItem);
	bool FitFilter_Quality(ITEMDEF* pItem);
	bool FitFilter_Name(ITEMDEF* pItem);

	void LoadItemFromDB();

	void LoadItemPiece(size_t start);
	void SetItemParam(int index, long param);
	long GetItemParam(int index);

	void SetReadingState(bool state);

protected:
	DECLARE_MESSAGE_MAP()
public:
	void ReloadItems();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnReadPiece(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBeginDrag(NMHDR* pHdr, LRESULT* pr);
	afx_msg void OnItemActive(NMHDR* pHdr, LRESULT* pr);

	afx_msg void OnItemType();
	afx_msg void OnItemPos();
	afx_msg void OnQuality();

	afx_msg void OnBtnNewItem();
	afx_msg void OnBtnEditItem();
	afx_msg void OnUpdateBtnEditItem(CCmdUI* pUI);
	afx_msg void OnBtnDeleteItem();
	afx_msg void OnUpdateBtnDeleteItem(CCmdUI* pUI);

	afx_msg void OnBtnText();
	afx_msg void OnBtnSearch();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
};


