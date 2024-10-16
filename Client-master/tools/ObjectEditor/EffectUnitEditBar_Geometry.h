
#pragma once
#include "EffectUnitEditBar.h"
#include "KeyframeEditBar.h"
#include "NormalSplitterWnd.h"
#include "ResourceDropTarget.h"

#include "OgreParametricShapeData.h"

class CGeometry;
class CGeometryBar : public CEffectEditBar, public CKeyFrameObject
{
	DECLARE_DYNAMIC(CGeometryBar)

public:
	CGeometryBar();
	virtual ~CGeometryBar();

	CNotifiedProp* m_pPropOffsetX;
	CNotifiedProp* m_pPropOffsetY;
	CNotifiedProp* m_pPropOffsetZ;

	CNotifiedProp* m_pPropYaw;
	CNotifiedProp* m_pPropPitch;
	CNotifiedProp* m_pPropRoll;

	CNotifiedProp* m_pPropScaleX;
	CNotifiedProp* m_pPropScaleY;
	CNotifiedProp* m_pPropScaleZ;

	CNotifiedProp* m_pPropBlendMode;
	CNotifiedProp* m_pPropGeomType;

	CNotifiedProp* m_pPropTexCol;
	CNotifiedProp* m_pPropTexRow;
	CNotifiedProp* m_pPropFrameTime;

	CNotifiedProp* m_pPropUSeg;
	CNotifiedProp* m_pPropVSeg;

	CNotifiedProp* m_pPropTexture;
	CNotifiedProp* m_pPropBindBone;

	CNotifiedProp* m_pPropDistort;

	CNotifiedProp* m_pPropMaskUVAddressMode;
	CNotifiedProp* m_pPropMaskTexCols;
	CNotifiedProp* m_pPropMaskTexRows;
	CNotifiedProp* m_pPropMaskTexture;



	CNotifiedProp* m_pPropShereR;
	CNotifiedProp* m_pPropShereAngle;

	CNotifiedProp* m_pPropTursoLR;
	CNotifiedProp* m_pPropTursoNR;

	CNotifiedProp* m_pPropTubeH;
	CNotifiedProp* m_pPropTubeLR;
	CNotifiedProp* m_pPropTubeNR;

	CNotifiedProp* m_pPropColumnH;
	CNotifiedProp* m_pPropColumnDR;
	CNotifiedProp* m_pPropColumnUR;

	CNotifiedProp* m_pPropUOffset;
	CNotifiedProp* m_pPropVOffset;
	CNotifiedProp* m_pPropRound;

	CNotifiedProp* m_pPropUScale;
	CNotifiedProp* m_pPropVScale;
	CNotifiedProp* m_pPropUVRotate;

	CNotifiedColorProp* m_pPropColor;
	CNotifiedProp* m_pPropAlpha;
	CNotifiedProp* m_pPropTick;

	CNotifiedProp* m_pPropMaskUVRotate;
	CNotifiedProp* m_pPropMaskUScale;
	CNotifiedProp* m_pPropMaskVScale;
	CNotifiedProp* m_pPropMaskUOffset;
	CNotifiedProp* m_pPropMaskVOffset;



	CNotifiedProp* m_pPropShowGrid;
	CNotifiedProp* m_pPropAddressMode;

	bool		m_bLockApp;
	CGeometry*	m_pTarget;
	CBCGPPropList	m_wndPropList;
	CTreeCtrl		m_wndTree;
	CNormalSplitterWnd m_wndSplitter;
	CBCGPToolBar m_wndToolBar;
	HTREEITEM	m_hDesc;
	HTREEITEM	m_hKeyFrame;
	CResDropPropListTarget	m_dropTarget;

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

	afx_msg void OnBtnLockApply();
	afx_msg void OnUpdateBtnLockApply(CCmdUI* pUI);


	void UpdateTree();

	void ChangePropList();
	void SaveCurrentProp(HTREEITEM hOldItem);
	void UpdateList();

	void DescUpdate(DATA_UPDATA_TYPE type);
	void KeyFrameUpdate(DATA_UPDATA_TYPE type, CGeometry::GEO_FRAME* pItem);


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


	void PropUpdate_geomtype(CBCGPProp** ppProp, Ogre::PARAMSHAPE_TYPE* data, DATA_UPDATA_TYPE type, CString name);
protected:
	DECLARE_MESSAGE_MAP()
};



