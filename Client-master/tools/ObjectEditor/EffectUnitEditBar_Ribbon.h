
#pragma once
#include "EffectUnitEditBar.h"
#include "KeyframeEditBar.h"
#include "ResourceDropTarget.h"
#include "NormalSplitterWnd.h"

class CRibbonBar : public CEffectEditBar, public CKeyFrameObject
{
	DECLARE_DYNAMIC(CRibbonBar)

public:
	CRibbonBar();
	virtual ~CRibbonBar();

protected:
	DECLARE_MESSAGE_MAP()


protected:


	bool	m_bLockApp;
	CRibbon* m_pTarget;

	CBCGPPropList	m_wndPropList;
	CTreeCtrl		m_wndTree;
	CNormalSplitterWnd m_wndSplitter;
	CBCGPToolBar m_wndToolBar;
	HTREEITEM	m_hDesc;
	HTREEITEM	m_hKeyFrame;
	CResDropPropListTarget	m_dropTarget;

	CNotifiedProp* m_pPropOffsetX;
	CNotifiedProp* m_pPropOffsetY;
	CNotifiedProp* m_pPropOffsetZ;

	CNotifiedProp* m_pPropYaw;
	CNotifiedProp* m_pPropPitch;
	CNotifiedProp* m_pPropRoll;

	CNotifiedProp* m_pPropMaxSeg;
	CNotifiedProp* m_pPropLifeSpan;
	CNotifiedProp* m_pPropEmitteRate;
	CNotifiedProp* m_pPropBlendMode;
	CNotifiedProp* m_pPropTexture;
	CNotifiedProp* m_pPropBindBone;
	CNotifiedProp* m_pPropDistort;
	CNotifiedProp* m_pPropLerpSeq;
	CNotifiedProp*		m_pPropAddressMode;

	CNotifiedProp* m_pPropMaskUVAddressMode;
	CNotifiedProp* m_pPropMaskTexture;


	CBCGPColorProp* m_pPropColor;
	CNotifiedProp*		m_pPropAlpha;
	CNotifiedProp*		m_pPropAbove;
	CNotifiedProp*		m_pPropBelow;
	CNotifiedProp*		m_pPropTick;
	CNotifiedProp*		m_pPropUVRotate;
	CNotifiedProp*		m_pPropUScale;
	CNotifiedProp*		m_pPropVScale;
	CNotifiedProp*		m_pPropUOffset;
	CNotifiedProp*		m_pPropVOffset;

	CNotifiedProp*      m_pPropSeeMask;
	CNotifiedProp*		m_pPropMaskUVRotate;
	CNotifiedProp*		m_pPropMaskUScale;
	CNotifiedProp*		m_pPropMaskVScale;
	CNotifiedProp*		m_pPropMaskUOffset;
	CNotifiedProp*		m_pPropMaskVOffset;

	CNotifiedProp*		m_pPropEditTemplate;


public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	BOOL DestroyWindow();
	afx_msg void OnDestroy();
	afx_msg void OnNewFrame();
	afx_msg void OnDeleteFrame();
	afx_msg void OnMoveUp();
	afx_msg void OnMoveDown();
	afx_msg void OnApply();
	afx_msg void OnRemove();
	afx_msg void OnSave();
	afx_msg void OnVisible();
	afx_msg void OnUpdateVisible(CCmdUI* pUI);
	afx_msg void OnTreeSelChanged(NMHDR* pHdr, LRESULT* pResult);
	afx_msg LRESULT OnDropRes(WPARAM w, LPARAM l);
	afx_msg LRESULT OnUpdatePropValue(WPARAM wParam, LPARAM lParam);
	afx_msg LPARAM GetItemParam(HTREEITEM hItem);
	afx_msg void OnTreeDblClick(NMHDR* pHdr, LRESULT* pResult);
	afx_msg LRESULT OnClickPropBtn(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBtnLockApply();
	afx_msg void OnUpdateBtnLockApply(CCmdUI* pUI);
	afx_msg void OnClick();
	void UpdateTree();

	void ChangePropList();
	void SaveCurrentProp(HTREEITEM hOldItem);
	void UpdateList();

	void DescUpdate(DATA_UPDATA_TYPE type);
	void KeyFrameUpdate(DATA_UPDATA_TYPE type, CRibbon::RIB_FRAME* pItem);


	void SetEditTarget(CEffectUnit* pEffect);
	CEffectUnit* GetEditTarget();
	virtual CKeyFrameObject* GetKeyFramObj() { return static_cast<CKeyFrameObject*>(this); };

	HTREEITEM m_hLastSel;
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	void UpdateKeyFrameWnd();
	void ReleaseKeyFrameWnd();


	//KeyFrameObject
	virtual void CopyKeyFrame(int from, int to);
	virtual int AddKeyFrame(UINT tick);
	virtual void DeleteKeyFrame(int index);
	virtual void ActiveKeyFrame(int index);
	virtual void SetKeyFramePos(int index, UINT tick);
	virtual int GetKeyFrameCount();
	virtual UINT GetKeyFrameTick(int index);
	virtual CString GetName();
};


