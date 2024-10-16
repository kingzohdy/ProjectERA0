
#pragma once
#include "NormalSplitterWnd.h"
#include "ResourceDropTarget.h"

class CAnimationEditPane : public CWnd
{
	DECLARE_DYNAMIC(CAnimationEditPane)
protected:
	DECLARE_MESSAGE_MAP()
public:
	CAnimationEditPane();
	virtual ~CAnimationEditPane();

protected:

	CBCGPToolBar	m_wndToolBar;
	CNormalSplitterWnd m_wndSplitter;
	CListCtrl		m_wndAnimList;
	CResDropTarget	m_dropTarget;
	CBCGPPropList	m_wndPropList;

	CSkeleton*	m_targetSkeleton;

	CNotifiedProp*	m_pPropAnimName;
	CNotifiedProp*	m_pPropLayer;
	//CBCGPProp*	m_pPropSeqs;
	//CBCGPProp*	m_pPropTriggers;

	CNotifiedProp*	m_pPropID;
	CNotifiedProp*	m_pPropTimeStart; 
	CNotifiedProp*	m_pPropTimeEnd;
	CBCGPProp*	m_pPropType;

	CSplitterGridView* m_pGrid1;
	CSplitterGridView* m_pGrid2;

	BOOL m_bPropEdited;
public:
	void SetEditTarget(CSkeleton* pSkeleton);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);//初始化
	afx_msg void OnSize(UINT nType, int cx, int cy);	//窗口大小发生改变

	afx_msg void OnBtnPlayanim();	// 播放按钮
	afx_msg void OnBtnStopanim();	// 停止按钮
	afx_msg void OnBtnPauseanim();	// 暂停按钮
	afx_msg void OnOpenAnim();		// 空
	afx_msg void OnUpdateBtnPauseanim(CCmdUI* pUI);
	afx_msg void OnPlayMulity();	// 播放勾选动画(报错)
	afx_msg void OnPlayAdd();		// 叠加播放动画
	afx_msg void OnMoveDown();		// 下移
	afx_msg void OnMoveUp();		// 上移
	afx_msg void OnRemoveAnim();	// 响应移除按钮命令
	afx_msg void OnItemChanged(NMHDR* pHdr, LRESULT* pResult);	//选择Item发生改变
	afx_msg void OnItemActived(NMHDR* pHdr, LRESULT* pResult);	//Item被激活
	afx_msg void OnItemDblClick(NMHDR* pHdr, LRESULT* pResult);	//双击Item 调用OnBtnPlayanim();
	void UpdateCurrentItem(int theItem = -1);					//更新当前选中的Item内容
	void UpdateList();											//更新列表框
	void SaveCurrentItem();										//保存当前Item内容
	afx_msg void OnDestroy();
	afx_msg LRESULT OnPropEndEdit(WPARAM w, LPARAM l);
	afx_msg LRESULT OnUpdatePropValue(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDropRes(WPARAM wParam, LPARAM lParam);

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


