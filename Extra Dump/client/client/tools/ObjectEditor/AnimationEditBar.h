
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

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);//��ʼ��
	afx_msg void OnSize(UINT nType, int cx, int cy);	//���ڴ�С�����ı�

	afx_msg void OnBtnPlayanim();	// ���Ű�ť
	afx_msg void OnBtnStopanim();	// ֹͣ��ť
	afx_msg void OnBtnPauseanim();	// ��ͣ��ť
	afx_msg void OnOpenAnim();		// ��
	afx_msg void OnUpdateBtnPauseanim(CCmdUI* pUI);
	afx_msg void OnPlayMulity();	// ���Ź�ѡ����(����)
	afx_msg void OnPlayAdd();		// ���Ӳ��Ŷ���
	afx_msg void OnMoveDown();		// ����
	afx_msg void OnMoveUp();		// ����
	afx_msg void OnRemoveAnim();	// ��Ӧ�Ƴ���ť����
	afx_msg void OnItemChanged(NMHDR* pHdr, LRESULT* pResult);	//ѡ��Item�����ı�
	afx_msg void OnItemActived(NMHDR* pHdr, LRESULT* pResult);	//Item������
	afx_msg void OnItemDblClick(NMHDR* pHdr, LRESULT* pResult);	//˫��Item ����OnBtnPlayanim();
	void UpdateCurrentItem(int theItem = -1);					//���µ�ǰѡ�е�Item����
	void UpdateList();											//�����б��
	void SaveCurrentItem();										//���浱ǰItem����
	afx_msg void OnDestroy();
	afx_msg LRESULT OnPropEndEdit(WPARAM w, LPARAM l);
	afx_msg LRESULT OnUpdatePropValue(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDropRes(WPARAM wParam, LPARAM lParam);

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


