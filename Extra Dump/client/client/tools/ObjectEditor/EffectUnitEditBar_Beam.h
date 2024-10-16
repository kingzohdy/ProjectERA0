
#pragma once
#include "EffectUnitEditBar.h"
#include "ResourceDropTarget.h"
#include "KeyframeEditBar.h"
#include "NormalSplitterWnd.h"

#include "OgreBeamEmitterData.h"

class CBeamBar : public CEffectEditBar, public CKeyFrameObject
{
	DECLARE_DYNAMIC(CBeamBar)

public:
	CBeamBar();
	virtual ~CBeamBar();

	void SetEditTarget(CEffectUnit* pEffect);
	CEffectUnit* GetEditTarget();
	CBeam* m_pTarget;

	bool	m_bLockApp;

	CBCGPPropList m_wndPropList;
	CBCGPToolBar  m_wndToolBar;
	CResDropPropListTarget m_dropTarget;

	CTreeCtrl		m_wndTree;
	CNormalSplitterWnd m_wndSplitter;
	HTREEITEM	m_hDesc;
	HTREEITEM	m_hKeyFrame;
	HTREEITEM	m_hLastSel;

	CSplitterGridView* m_pGrid1;
	CSplitterGridView* m_pGrid2;



	CNotifiedProp* m_pPropBlendType;
	CNotifiedProp* m_pPropMaxParticles;

	CNotifiedProp* m_pPropHFPoints;
	CNotifiedProp* m_pPropHFRange1;
	CNotifiedProp* m_pPropHFRange2;

	CNotifiedProp* m_pPropLFPoints;
	CNotifiedProp* m_pPropWaveTypeY;
	CNotifiedProp* m_pPropLFYParam1;
	CNotifiedProp* m_pPropLFYParam2;
	CNotifiedProp* m_pPropLFYParam3;

	CNotifiedProp* m_pPropWaveTypeZ;
	CNotifiedProp* m_pPropLFZParam1;
	CNotifiedProp* m_pPropLFZParam2;
	CNotifiedProp* m_pPropLFZParam3;

	CNotifiedProp* m_pPropEmitOnce;
	CNotifiedProp* m_pPropDistort;
	CNotifiedProp* m_pPropRandomTile;
	CNotifiedProp* m_pPropFrameTick;

	CNotifiedProp* m_pPropAddressMode;
	CNotifiedProp* m_pPropUVRotate;
	CNotifiedProp* m_pPropUVScale;
	CNotifiedProp* m_pPropUSpeed;
	CNotifiedProp* m_pPropVSpeed;
	CNotifiedProp* m_pPropTexRows;
	CNotifiedProp* m_pPropTexCols;
	CNotifiedProp* m_pPropTexture;
	CNotifiedProp* m_pPropUOgnOff;
	CNotifiedProp* m_pPropVOgnOff;

	CNotifiedProp* m_pPropMaskAddressMode;
	CNotifiedProp* m_pPropMaskUVRotate;
	CNotifiedProp* m_pPropMaskUVScale;
	CNotifiedProp* m_pPropMaskUSpeed;
	CNotifiedProp* m_pPropMaskVSpeed;
	CNotifiedProp* m_pPropMaskTexRows;
	CNotifiedProp* m_pPropMaskTexCols;
	CNotifiedProp* m_pPropMaskTexture;
	CNotifiedProp* m_pPropMaskUOgnOff;
	CNotifiedProp* m_pPropMaskVOgnOff;

	CNotifiedProp* m_pPropColorKey;
	CNotifiedProp* m_pPropAlphaKey;
	CNotifiedProp* m_pPropWidthKey;

	CNotifiedProp* m_pPropBindBone;



	CNotifiedProp* m_pPropStartRangeX;
	CNotifiedProp* m_pPropStartRangeY;
	CNotifiedProp* m_pPropStartRangeZ;

	CNotifiedProp* m_pPropEndRangeX;
	CNotifiedProp* m_pPropEndRangeY;
	CNotifiedProp* m_pPropEndRangeZ;

	CNotifiedProp* m_pPropEndPosX;
	CNotifiedProp* m_pPropEndPosY;
	CNotifiedProp* m_pPropEndPosZ;

	CNotifiedProp* m_pPropSizeVar;
	CNotifiedProp* m_pPropLife;
	CNotifiedProp* m_pPropLifeVar;
	CNotifiedProp* m_pPropEmitRate;
	CNotifiedProp* m_pPropTick;

protected:
	DECLARE_MESSAGE_MAP()
public:
	void CreateDescProp();
	void CreateKeyFrameProp(CBeam::BEA_FRAME* pFrame);
	void CreateKeyFrameProp();

	void SaveCurrentProp(HTREEITEM hOldItem);
	void UpdateList();
	void ResetProp();
	void ClearProp();
	void ChangePropList();
	void UpdateTree();
	LPARAM GetItemParam(HTREEITEM hItem);


	void DescUpdate(DATA_UPDATA_TYPE type);

	void KeyFrameUpdate(DATA_UPDATA_TYPE type, CBeam::BEA_FRAME* pFrame);

	virtual CKeyFrameObject* GetKeyFramObj() { return static_cast<CKeyFrameObject*>(this); };

	void PropUpdate_wavetype(CBCGPProp** ppProp, Ogre::BEAM_WAVE_TYPE* data, DATA_UPDATA_TYPE type, CString name);

	virtual void SetPropValue(CString strFlag, _variant_t var);

	//KeyFrameObject
	virtual void CopyKeyFrame(int from, int to);
	virtual int AddKeyFrame(UINT tick);
	virtual void DeleteKeyFrame(int index);
	virtual void ActiveKeyFrame(int index);
	virtual void SetKeyFramePos(int index, UINT tick);
	virtual int GetKeyFrameCount();
	virtual UINT GetKeyFrameTick(int index);
	virtual CString GetName();

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnUpdateVisible(CCmdUI* pUI);
	afx_msg void OnVisible();
	afx_msg void OnApply();
	afx_msg void OnSave();
	afx_msg void OnRemove();

	afx_msg LRESULT OnUpdatePropValue(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDropRes(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClickPropBtn(WPARAM w, LPARAM l);
	afx_msg void OnTreeSelChanged(NMHDR* pHdr, LRESULT* pResult);
};