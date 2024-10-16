
#pragma once
#include "NormalSplitterWnd.h"
#include "MotionCategoryInfo.h"
#include "MotionBrowserHelper.h"
#include "MotionDirHelper.h"

class CMotionBrowserBar : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CMotionBrowserBar)

public:
	CMotionBrowserBar();
	virtual ~CMotionBrowserBar();



	CEdit			m_wndInfo;
	CTreeCtrl		m_wndTree;
	CBCGPToolBar	m_wndToolBar;

	CNormalSplitterWnd	m_wndSplitter;
	CSplitterGridView*	m_pGrid1;
	CSplitterGridView*	m_pGrid2;

	CImageList		m_imgList;

	HTREEITEM	m_hCommon;			// 自有技能
	HTREEITEM	m_hPrivate;			// 公共技能
	HTREEITEM	m_hNotFinished;
	HTREEITEM	m_hMaterial;
	HTREEITEM	m_hWork;

	HTREEITEM	m_hCommonMod;
	HTREEITEM	m_hMaterialMod;
	HTREEITEM	m_hWorkMod;

	CString	GetSelectedCommonChunkFolder();


	void	UpdateTree();
	void	UpdateExistingCommonMotion();
	void	UpdateExistingCommonMotion2();

	void	UpdatePrivateMotion();
	void	GetFileNameParts(CString strFileName, CString& strTitle, CString& strExt);
	void	ResetCommonMotionIcon();

	void	OnMainClientCreated();
	void	OnMainActorLoaded();

	HTREEITEM			GetItemBasicParent(HTREEITEM hItem);
	MotionItemInfo*		GetSelectedMotionInfo();
	MotionItemInfo*		GetMotionInfo(HTREEITEM hItem);
	CMotionDirHelper*	GetSelectedMotionDirHelper();
	CMotionDirHelper*	GetMotionDirHelper(HTREEITEM hItem);


	CTaskInfo	m_privateTask;
	CTaskInfo	m_commonTask;

	CMotionBrowserHelper	m_CommonMotion;
	CMotionBrowserHelper	m_PrivateMotion;

	CMotionDirHelper		m_CommonDirMotion;
	CMotionDirHelper		m_MaterialDirMotion;
	CMotionDirHelper		m_WorkDirMotion;


	CMotionDirHelper		m_CommonDirShowMod;
	CMotionDirHelper		m_MaterialDirShowMod;
	CMotionDirHelper		m_WorkDirShowMod;


	CString m_strMotionPath;


protected:
	DECLARE_MESSAGE_MAP()
public:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	
	afx_msg void OnTreeRClick(NMHDR* pHdr, LRESULT* pResult);			// 弹出菜单
	afx_msg void OnTreeDBClick(NMHDR* pHdr, LRESULT* pResult);			// 双击节点
	afx_msg void OnTreeSelectChanged(NMHDR* pHdr, LRESULT* pResult);	// 节点选择改变

	afx_msg void OnTreeExpand(NMHDR* pHdr, LRESULT* pResult);			// 展开树形控件

	afx_msg void OnTreeBeginDrag(NMHDR* pHdr, LRESULT* pResult);		// 拖动item

	afx_msg void OnPreview();
	afx_msg void OnEdit();	
	afx_msg void OnDelete();
	afx_msg void OnCopyToPrivate();
	afx_msg void OnExplore();

	afx_msg void OnNewCommonMotion();
	afx_msg void OnNewPrivateMotion();

	afx_msg void OnImportCommonTask();
	afx_msg void OnImportPrivateTask();

	afx_msg void OnFolderCreateSub();
	afx_msg void OnFolderDelete();
	afx_msg void OnFolderRename();
	afx_msg void OnPruduct();
	afx_msg void OnMbLoadshowmod();
	afx_msg void OnDestroy();
};


