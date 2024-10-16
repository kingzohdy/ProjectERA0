
#pragma once
#include "EffectUnitEditBar.h"
#include "ResourceDropTarget.h"

#include "OgreEntityMotionElement.h"

class CUnknownObjectBar : public CEffectEditBar
{
	DECLARE_DYNAMIC(CUnknownObjectBar)

public:
	CUnknownObjectBar(void);
	virtual ~CUnknownObjectBar(void);
	virtual void SetEditTarget(CEffectUnit* pEffect);
	virtual CEffectUnit* GetEditTarget();
protected:
	bool			m_bLockApp;
	CBCGPToolBar		m_wndToolBar;
	CBCGPPropList		m_wndPropList;

	CNotifiedProp* m_pPropOffsetX;
	CNotifiedProp* m_pPropOffsetY;
	CNotifiedProp* m_pPropOffsetZ;

	CNotifiedProp* m_pPropYaw;
	CNotifiedProp* m_pPropPitch;
	CNotifiedProp* m_pPropRoll;


	CNotifiedProp* m_pPropScaleX;
	CNotifiedProp* m_pPropScaleY;
	CNotifiedProp* m_pPropScaleZ;

	CNotifiedProp*	m_pPropResPath;

	CNotifiedProp*	m_pPropAnimMode;
	CNotifiedProp*	m_pPropAnimName;

	CNotifiedProp*	m_pPropBindBone;

	CEffectUnit*		m_pTarget;

	CResDropPropListTarget m_dropTarget;
	DECLARE_MESSAGE_MAP()

protected:
	void CreateProp();
	void SaveCurrentProp();
	void UpdateList();

	void DataUpdate(DATA_UPDATA_TYPE type);

	void AddAnimNames();
	void AddAnimFromSkinActor(Ogre::ModelData* pActorSource, std::vector<CString>& names);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateVisible(CCmdUI* pUI);
	afx_msg void OnVisible();
	afx_msg void OnApply();
	afx_msg void OnRemove();
	afx_msg void OnBtnClick();
	afx_msg void OnBtnLockApply();
	afx_msg void OnUpdateBtnLockApply(CCmdUI* pUI);

	afx_msg LRESULT OnUpdatePropValue(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDropRes(WPARAM w, LPARAM l);

	void PropUpdate_animmode(CBCGPProp** ppProp, Ogre::BIND_ANIM_MODE* data, DATA_UPDATA_TYPE type, CString name);
};
