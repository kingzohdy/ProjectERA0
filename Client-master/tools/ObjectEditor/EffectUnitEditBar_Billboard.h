
#pragma once
#include "EffectUnitEditBar.h"
#include "KeyframeEditBar.h"
#include "ObjectEditorTypes.h"
#include "ResourceDropTarget.h"
#include "EffectUnit_Billboard.h"
#include "NormalSplitterWnd.h"

//class CBillboard;
class CBillboardBar : public CEffectEditBar, public CKeyFrameObject
{
	DECLARE_DYNAMIC(CBillboardBar)

public:
	CBillboardBar();
	virtual ~CBillboardBar();

	virtual void SetEditTarget(CEffectUnit* pEffect);
	virtual CEffectUnit* GetEditTarget();

	bool	m_bLockApp;

	CBillboard* m_pTarget;

	CBCGPToolBar	m_wndToolBar;
	//CBCGPPropList	m_wndPropList;
	CBCGPPropList	m_wndPropList;

	CNotifiedProp* m_pPropOffsetX;
	CNotifiedProp* m_pPropOffsetY;
	CNotifiedProp* m_pPropOffsetZ;

	CNotifiedProp* m_pPropYaw;
	CNotifiedProp* m_pPropPitch;
	CNotifiedProp* m_pPropRoll;

	CNotifiedProp*	m_pPropUVRotate;
	CNotifiedProp*	m_pPropUVScale;

	CNotifiedProp* m_pPropType;
	CNotifiedProp* m_pPropCenterX;
	CNotifiedProp* m_pPropCenterY;
	CNotifiedProp* m_pPropCenterZ;
	CNotifiedProp* m_pPropBlendType;
	CNotifiedProp* m_pPropPos;
	CNotifiedProp* m_pPropMidPoint;
	CNotifiedProp* m_pPropTexRows;
	CNotifiedProp* m_pPropTexCols;
	CNotifiedProp* m_pPropTimeSlot;

	CNotifiedProp* m_pPropAspectKey1;
	CNotifiedProp* m_pPropAspectKey2;
	CNotifiedProp* m_pPropAspectKey3;

	CNotifiedProp* m_pPropSizeKey1;
	CNotifiedProp* m_pPropSizeKey2;
	CNotifiedProp* m_pPropSizeKey3;

	CBCGPColorProp* m_pPropColorKey1;
	CBCGPColorProp* m_pPropColorKey2;
	CBCGPColorProp* m_pPropColorKey3;

	CNotifiedProp* m_pPropAlphaKey1;
	CNotifiedProp* m_pPropAlphaKey2;
	CNotifiedProp* m_pPropAlphaKey3;

	CNotifiedProp* m_pPropTexture;
	CNotifiedProp* m_pPropBindBone;

	CNotifiedProp* m_pPropColorBtn;
	CNotifiedProp* m_pPropAlphaBtn;
	CNotifiedProp* m_pPropSizeBtn;
	CNotifiedProp* m_pPropAspectBtn;
	CNotifiedProp* m_pPropAddressMode;;
	CNotifiedProp* m_pPropCenterOffsetMode;
	CNotifiedProp* m_pPropCenterOffsetX;
	CNotifiedProp* m_pPropCenterOffsetY;


	//CNotifiedProp* m_pPropScale;
	CNotifiedProp* m_pPropScaleX;
	CNotifiedProp* m_pPropScaleY;
	CNotifiedProp* m_pPropScaleZ;

	CNotifiedProp* m_pPropDistort;
	CNotifiedProp* m_pPropFrameType;
	CNotifiedProp* m_pPropXYDepend; // 104

	CNotifiedProp* m_pPropMaskUVAddressMode;
	CNotifiedProp* m_pPropMaskTexCols;
	CNotifiedProp* m_pPropMaskTexRows;
	CNotifiedProp* m_pPropMaskTexture;

	CResDropPropListTarget m_dropTarget;
	CTreeCtrl				m_wndTree;

	CNormalSplitterWnd m_wndSplitter;

	HTREEITEM	m_hDesc;	// 参数树
	HTREEITEM	m_hKeyFrame;// 关键帧树
	HTREEITEM	m_hLastSel;

	CNotifiedProp* m_pPropUVRot;
	CNotifiedProp* m_pPropUScale;
	CNotifiedProp* m_pPropVScale;
	CNotifiedProp* m_pPropUOffset;
	CNotifiedProp* m_pPropVOffset;
	CNotifiedProp* m_pPropSeqFrame;
	CNotifiedColorProp * m_pPropColor;
	CNotifiedProp * m_pPropAlpha;
	CNotifiedProp* m_pPropSize;
	CNotifiedProp* m_pPropAspect;

	// 104
	CNotifiedProp *m_pPropSizeX;
	CNotifiedProp *m_pPropSizeY;

	CNotifiedProp* m_pPropMaskUVRot;
	CNotifiedProp* m_pPropMaskUScale;
	CNotifiedProp* m_pPropMaskVScale;
	CNotifiedProp* m_pPropMaskUOffset;
	CNotifiedProp* m_pPropMaskVOffset;
	CNotifiedProp* m_pPropMaskSeqFrame;

	CNotifiedProp* m_pPropTick;


protected:
	DECLARE_MESSAGE_MAP()
protected:
	void ClearProps();
	void CreateDescProp();
	void CreateKeyFrameProp(CBillboard::BIL_FRAME* pFrameItem);
	void CreateProp();
	void SaveCurrentProp(HTREEITEM hOldItem);
	void UpdateList();
	void UpdateTree();
	void DescUpdate(DATA_UPDATA_TYPE type);
	void KeyFrameUpdate(DATA_UPDATA_TYPE type, CBillboard::BIL_FRAME* pItem);


	LPARAM GetItemParam(HTREEITEM hItem);

	//Digit Target
	virtual void Increase(int step);
	virtual void Decrease(int step);
public:
	afx_msg void OnTreeSelChanged(NMHDR* pHdr, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateVisible(CCmdUI* pUI);

	// 工具条按钮
	afx_msg void OnVisible();
	afx_msg void OnApply();
	afx_msg void OnSave();
	afx_msg void OnRemove();
	afx_msg void OnBtnLockApply();
	afx_msg void OnNewFrame();
	afx_msg void OnDeleteFrame();
	afx_msg void OnBtnClick();

	afx_msg void OnUpdateBtnLockApply(CCmdUI* pUI);
	afx_msg LRESULT OnUpdatePropValue(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDropRes(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClickPropBtn(WPARAM w, LPARAM l);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnDestroy();
	afx_msg void OnClose();

	void UpdateKeyFrameWnd();
	void ReleaseKeyFrameWnd();

	virtual CKeyFrameObject* GetKeyFramObj() { return static_cast<CKeyFrameObject*>(this); };

	virtual void CopyKeyFrame(int from, int to);
	virtual int AddKeyFrame(UINT tick);
	virtual void DeleteKeyFrame(int index);
	virtual void ActiveKeyFrame(int index);
	virtual void SetKeyFramePos(int index, UINT tick);
	virtual int GetKeyFrameCount();
	virtual UINT GetKeyFrameTick(int index);
	virtual CString GetName();


	void PropUpdate_frametype(CBCGPProp** ppProp, Ogre::SEQ_FRAME_TYPE* data, DATA_UPDATA_TYPE type, CString name);

};


