
#include "stdafx.h"
#include "BoundingEditBar.h"
#include "NotifyProperty.h"
#include "EffectManager.h"
#include "ObjectBounding.h"
#include "EffectUnitEditBar.h"

IMPLEMENT_DYNAMIC(CBoudingBar, CBCGPDockingControlBar)
CBoudingBar::CBoudingBar()
{
}

CBoudingBar::~CBoudingBar()
{
}


BEGIN_MESSAGE_MAP(CBoudingBar, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(EEN_PROP_UPDATE_VALUE, OnUpdatePropValue)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()



// CBoudingBar message handlers


int CBoudingBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;


	m_wndPropList.Create(WS_VISIBLE, CRect(0, 0, 100, 100), this, ID_BOUNDPROP);

	m_wndPropList.EnableHeaderCtrl(FALSE);
	// TODO:  Add your specialized creation code here
	CreateProp();
	return 0;
}

void CBoudingBar::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	CRect frameRect;
	GetClientRect(frameRect);

	m_wndPropList.SetWindowPos(NULL, 0, 0,
		frameRect.right - frameRect.left, frameRect.bottom - frameRect.top , SWP_NOZORDER);

	// TODO: Add your message handler code here
}


void CBoudingBar::CreateProp()
{

	m_wndPropList.RemoveAll();

	PropUpdate(CREATE_PROP);

	CBCGPProp*  pCenter = new CBCGPProp("中心点");
	pCenter->AddSubItem(m_pPropCenterX);
	pCenter->AddSubItem(m_pPropCenterY);
	pCenter->AddSubItem(m_pPropCenterZ);

	CBCGPProp*  pExtend = new CBCGPProp("大小");
	pExtend->AddSubItem(m_pPropExtendX);
	pExtend->AddSubItem(m_pPropExtendY);
	pExtend->AddSubItem(m_pPropExtendZ);

	m_pPropCenterX->SetNotifyTarget(GetSafeHwnd());
	m_pPropCenterY->SetNotifyTarget(GetSafeHwnd());
	m_pPropCenterZ->SetNotifyTarget(GetSafeHwnd());
	m_pPropExtendX->SetNotifyTarget(GetSafeHwnd());
	m_pPropExtendY->SetNotifyTarget(GetSafeHwnd());
	m_pPropExtendZ->SetNotifyTarget(GetSafeHwnd());

	m_wndPropList.AddProperty(pCenter);
	m_wndPropList.AddProperty(pExtend);
}

LRESULT CBoudingBar::OnUpdatePropValue(WPARAM wParam, LPARAM lParam)
{
	PROPNM* pnm = (PROPNM*)lParam;
	SaveCurrentProp();

	return 0;
}

void CBoudingBar::SaveCurrentProp()
{
	PropUpdate(PROP_TO_DATA);

	CEffectMgr::GetInstance()->m_BoundingInfo->m_Center = m_aabb.getCenter();
	CEffectMgr::GetInstance()->m_BoundingInfo->m_Extent = m_aabb.getExtension();
}

void CBoudingBar::UpdateList()
{
	Ogre::Vector3 center = CEffectMgr::GetInstance()->m_BoundingInfo->m_Center;
	Ogre::Vector3 exten = CEffectMgr::GetInstance()->m_BoundingInfo->m_Extent;
	m_aabb.setRange(center-exten, center+exten);
	PropUpdate(DATA_TO_PROP);

}

void CBoudingBar::PropUpdate(DATA_UPDATA_TYPE type)
{
	CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropCenterX, &m_aabb.m_MinPos.x, type, "X");
	CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropCenterY, &m_aabb.m_MinPos.y, type, "Y");
	CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropCenterZ, &m_aabb.m_MinPos.z, type, "Z");

	CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropExtendX, &m_aabb.m_MaxPos.x, type, "X");
	CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropExtendY, &m_aabb.m_MaxPos.y, type, "Y");
	CEffectEditBar::PropUpdate_float((CBCGPProp**)&m_pPropExtendZ, &m_aabb.m_MaxPos.z, type, "Z");

}
void CBoudingBar::OnSetFocus(CWnd* pOldWnd)
{
	//CBCGPDockingControlBar::OnSetFocus(pOldWnd);

	//if(CEffectMgr::GetInstance() != NULL)
	//{
	//	CEffectMgr::GetInstance()->m_BoundingInfo->m_bShowAABB = true;
	//}
	// TODO: Add your message handler code here
}

void CBoudingBar::OnKillFocus(CWnd* pNewWnd)
{
	//CBCGPDockingControlBar::OnKillFocus(pNewWnd);
	//if(CEffectMgr::GetInstance() != NULL)
	//{
	//	CEffectMgr::GetInstance()->m_BoundingInfo->m_bShowAABB = false;
	//}
	// TODO: Add your message handler code here
}

void CBoudingBar::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CBCGPDockingControlBar::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	if(CEffectMgr::GetInstance() != NULL)
	{
		CEffectMgr::GetInstance()->m_BoundingInfo->m_bShowAABB = (bShow == TRUE ? true : false);
	}
}
