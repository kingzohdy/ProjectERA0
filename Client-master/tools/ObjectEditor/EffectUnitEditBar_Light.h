
#pragma once
#include "EffectUnitEditBar.h"
#include "OgreRenderTypes.h"

class CLightBar : public CEffectEditBar
{
	DECLARE_DYNAMIC(CLightBar)

public:
	CLightBar();
	virtual ~CLightBar();

	virtual void SetEditTarget(CEffectUnit* pEffect);
	virtual CEffectUnit* GetEditTarget();

protected:
	bool			m_bLockApp;
	CBCGPToolBar		m_wndToolBar;
	CBCGPPropList		m_wndPropList;

	CNotifiedProp* m_pPropOffsetX;
	CNotifiedProp* m_pPropOffsetY;
	CNotifiedProp* m_pPropOffsetZ;

	CNotifiedProp*	m_pPropType;
	CNotifiedColorProp*	m_pPropColor;
	CNotifiedProp*	m_pPropAttenuation;
	CNotifiedProp*	m_pPropRange;
	CNotifiedProp*	m_pPropBindBone;

	CLight*		m_pTarget;
	DECLARE_MESSAGE_MAP()

protected:
	void CreateProp();
	void SaveCurrentProp();
	void UpdateList();

	void DataUpdate(DATA_UPDATA_TYPE type);

	void PropUpdate_lighttype(CBCGPProp** ppProp, Ogre::LightType* data, DATA_UPDATA_TYPE type, CString name);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateVisible(CCmdUI* pUI);
	afx_msg void OnVisible();
	afx_msg void OnApply();
	afx_msg void OnSave();
	afx_msg void OnRemove();
	afx_msg void OnBtnClick();
	afx_msg void OnBtnLockApply();
	afx_msg void OnUpdateBtnLockApply(CCmdUI* pUI);

	afx_msg LRESULT OnUpdatePropValue(WPARAM wParam, LPARAM lParam);
};


