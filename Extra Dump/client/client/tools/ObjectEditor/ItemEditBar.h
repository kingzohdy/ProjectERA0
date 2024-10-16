
#pragma once
#include "NormalSplitterWnd.h"
#include "ObjectEditorTypes.h"
#include "ResourceDropTarget.h"
#include <vector>

class CItemEditPane : public CWnd
{
	DECLARE_DYNAMIC(CItemEditPane)

public:
	CItemEditPane();
	virtual ~CItemEditPane();

	CBCGPToolBar	m_wndToolBar;
	CNormalSplitterWnd m_wndSplitter;
	CNormalSplitterWnd m_wndSplitter2;
	CNormalSplitterWnd m_wndSplitter3;

	CListCtrl		m_wndListType;
	//CListCtrl		m_wndListGeoset;
	CBCGPPropList	m_wndPropListMod;
	CBCGPPropList	m_wndPropListTex;

	CListCtrl		m_wndListItem;

	CResDropPropListTarget m_dropTarget;

	CSplitterGridView* m_pGrid1;
	CSplitterGridView* m_pGrid2;

	CSplitterGridView* m_pGrid21;
	CSplitterGridView* m_pGrid22;

	CSplitterGridView* m_pGrid221;
	CSplitterGridView* m_pGrid222;


	CNotifiedProp* m_pPropGeosetA;
	CNotifiedProp* m_pPropGeosetB;
	CNotifiedProp* m_pPropGeosetC;
	CNotifiedProp* m_pPropGeosetD;
	CNotifiedProp* m_pPropGeosetE;

	CNotifiedProp* m_pPropModel;
	CNotifiedProp* m_pPropModel2;
	CNotifiedProp* m_pPropModel3;
	CNotifiedProp* m_pPropSkin;
	CNotifiedProp* m_pPropSkin2;
	CNotifiedProp* m_pPropTexA;
	CNotifiedProp* m_pPropTexB;
	CNotifiedProp* m_pPropTexC;
	CNotifiedProp* m_pPropTexD;
	CNotifiedProp* m_pPropTexE;
	CNotifiedProp* m_pPropTexF;
	CNotifiedProp* m_pPropTexG;
	CNotifiedProp* m_pPropTexH;
	CNotifiedProp* m_pPropVisuals;


	CString m_strGeosetA;
	CString m_strGeosetB;
	CString m_strGeosetC;
	CString m_strGeosetD;
	CString m_strGeosetE;

	CString m_strModel;
	CString m_strModel2;
	CString m_strModel3;
	CString m_strSkin;
	CString m_strSkin2;
	CString m_strTexA;
	CString m_strTexB;
	CString m_strTexC;
	CString m_strTexD;
	CString m_strTexE;
	CString m_strTexF;
	CString m_strTexG;
	CString m_strTexH;
	
	CString m_strVisuals;

	int m_iLastCheck;
	int m_iCurrID;

	std::vector<CString> m_ognSkins;
	void CreateSplitter(CNormalSplitterWnd& wndSplitter, CWnd* pParent, CSplitterGridView* &pGrid1, CSplitterGridView* &pGrid2);
	int GetCheckedItem();
	bool AddProp(int iItemIndex, CNotifiedProp* pProp, CBCGPProp* pParent, CString strFieldName);
	bool AddTexProp(int iItemIndex, CNotifiedProp* pProp, CBCGPProp* pParent, CString strFieldName);

	bool IsFieldEnabled(CString strFieldName);

	void setModel(CBCGPProp* pProp, LPCTSTR strmodel, LPCTSTR filedName, int anchor_index);
	int GetAnchor(int mod);
	void CreateProp();
	void UpdateList();
	void UpdateSkins();
	void UpdateItemList();
	void AddSkinList(CBCGPProp* pProp);
	void SetTexture(CBCGPProp* pProp, LPCTSTR strTex, LPCTSTR filedName);

	void UpdateDispProp(DATA_UPDATA_TYPE type);
	bool IsOrginSkin(CString strSkinName);
	int  CreateNewItemDisplayID();

	void LoadItemById(int id);
	void WriteToDb(int itemId, const char* name, int quality);
	bool m_bUpdating;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnListItemChange(NMHDR* pHDR, LRESULT* pResult);
	afx_msg void OnGeoListItemChange(NMHDR* pHDR, LRESULT* pResult);
	afx_msg void OnEquipItemChange(NMHDR* pHDR, LRESULT* pResult);
	afx_msg LRESULT OnUpdatePropValue(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDropRes(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClickPropBtn(WPARAM w, LPARAM l);

	afx_msg void OnOverwriteItem();
	afx_msg void OnAddItem();
	afx_msg void OnDelItem();
	afx_msg void OnEquipItemActive(NMHDR* pHDR, LRESULT* pResult);
	afx_msg void OnBeginItemDrag(NMHDR* pHDR, LRESULT* pResult);
	afx_msg void OnClearEquip();
};


