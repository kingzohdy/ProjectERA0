
#pragma once
#include "OgreBounding.h"

class CBoudingBar : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CBoudingBar)

public:
	CBoudingBar();
	virtual ~CBoudingBar();

	CBCGPPropList m_wndPropList;

	CNotifiedProp* m_pPropCenterX;
	CNotifiedProp* m_pPropCenterY;
	CNotifiedProp* m_pPropCenterZ;

	CNotifiedProp* m_pPropExtendX;
	CNotifiedProp* m_pPropExtendY;
	CNotifiedProp* m_pPropExtendZ;

	void CreateProp();
	void UpdateList();
	void SaveCurrentProp();
	LRESULT OnUpdatePropValue(WPARAM wParam, LPARAM lParam);

	void PropUpdate(DATA_UPDATA_TYPE type);

	Ogre::BoxBound m_aabb;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};


