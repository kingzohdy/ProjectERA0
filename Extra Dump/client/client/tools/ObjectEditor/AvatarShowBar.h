
#pragma once
#include "ItemDropTarget.h"
#include "ObjectEditorTypes.h"

#include "AvatarConfig.h"

class CAvatarShowBar : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CAvatarShowBar)

public:


	CAvatarShowBar();
	virtual ~CAvatarShowBar();

	CAvatarShowInfo*	GetEditTarget();
	void				SetEditTarget(CAvatarShowInfo* pInfo);
	void				CreateConfig();
	void				UpdateList();

	CBCGPProp*			CreateProp(Ogre::CVarInfo* pVarInfo);
	void				UpdateProp(CBCGPProp* pProp);

	void				SwitchPropState(BOOL bEnable);
protected:
	CBCGPPropList	m_wndPropList;
	CBCGPToolBar	m_wndToolbar;
	CItemDropPropListTarget m_dropTarget;

	CAvatarShowInfo* m_pAvatarShowInfo;

	CBCGPProp*		m_pPropMod;
	CBCGPProp*		m_pPropNormal;

	bool			m_bConfigCreated;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnLoadMod();
	afx_msg void OnSaveNPC();
	afx_msg void OnLoadNPC();
	afx_msg void OnBtnEnable();
	afx_msg void OnUpdateBtnEnable(CCmdUI* pUI);
	afx_msg LRESULT OnUpdatePropValue(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDropItem(WPARAM w, LPARAM l);
};


