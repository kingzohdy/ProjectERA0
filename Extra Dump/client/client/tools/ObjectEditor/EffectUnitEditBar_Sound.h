
#pragma once
#include "EffectUnitEditBar.h"
#include "ExtPropList.h"

#include "OgreSoundData.h"

class CSoundBar : public CEffectEditBar
{
	DECLARE_DYNAMIC(CSoundBar)

public:
	CSoundBar();
	virtual ~CSoundBar();
	virtual void SetEditTarget(CEffectUnit* pEffect);
	virtual CEffectUnit* GetEditTarget();


	CBCGPToolBar m_wndToolBar;
	CExtPropList m_wndPropList;

	CNotifiedProp* m_pPropOffsetX;
	CNotifiedProp* m_pPropOffsetY;
	CNotifiedProp* m_pPropOffsetZ;

	CNotifiedProp* m_pPropFileName;
	CNotifiedProp* m_pPropMinDst;
	CNotifiedProp* m_pPropMaxDst;
	CNotifiedProp* m_pPropVolume;
	CNotifiedProp* m_pPropVel;
	CNotifiedProp* m_pPropLoop;
	CNotifiedProp* m_pPropPlayMode;

	CNotifiedProp* m_pPropBindBone;
	//float		fMinDistance; 
	//float		fMaxDistance;
	//float		fVolume;
	//Vector3		vPos;
	//Vector3		vVel;
	//bool		bLoop;


	CSound* m_pTarget;
	bool m_bLockApp;
	void CreateProp();
	void SaveCurrentProp();
	void UpdateList();
	void DataUpdate(DATA_UPDATA_TYPE type);
	void PropUpdate_file(CBCGPProp** ppProp, CString* data, DATA_UPDATA_TYPE type, CString name);
	void PropUpdate_soundplaymode(CBCGPProp** ppProp, Ogre::SOUND_PLAY_MODE* data, DATA_UPDATA_TYPE type, CString name);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnUpdateVisible(CCmdUI* pUI);
	afx_msg void OnVisible();
	afx_msg void OnApply();
	afx_msg void OnSave();
	afx_msg void OnRemove();
	afx_msg void OnBtnLockApply();
	afx_msg void OnUpdateBtnLockApply(CCmdUI* pUI);
	afx_msg void OnBtnClick();
	afx_msg LRESULT OnUpdatePropValue(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClickPropBtn(WPARAM w, LPARAM l);
};


