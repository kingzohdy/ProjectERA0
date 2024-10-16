
#include "stdafx.h"
#include "KeyframeEditBar.h"
#include "NewKeyframeDlg.h"

// CKeyFrameWnd

IMPLEMENT_DYNAMIC(CKeyFrameWnd, CBCGPDockingControlBar)
CKeyFrameWnd::CKeyFrameWnd() : m_pKeyFrameObj(NULL)
{
}

CKeyFrameWnd::~CKeyFrameWnd()
{
}


BEGIN_MESSAGE_MAP(CKeyFrameWnd, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(TAN_POS_CHANGE, ID_KEYFRAMECTRL, OnPosChange)
	ON_NOTIFY(TAN_LAYER_ACTIVE, ID_KEYFRAMECTRL, OnLayerActive)
END_MESSAGE_MAP()



// CKeyFrameWnd message handlers


int CKeyFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndTimeAixes.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 100, 100), this, ID_KEYFRAMECTRL);
	m_wndTimeAixes.ShowWindow(SW_SHOW);
	m_wndTimeAixes.SetUnitValue(100.0f);
	return 0;
}

void CKeyFrameWnd::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	CRect clientRect;
	GetClientRect(clientRect);

	m_wndTimeAixes.SetWindowPos(NULL, clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height(), SWP_NOZORDER);
	// TODO: Add your message handler code here
}
void CKeyFrameWnd::OnLayerActive(NMHDR* pHdr, LRESULT* pResult)
{
	TANMHDR* pTanmhdr = (TANMHDR*)pHdr;
	if(pTanmhdr->layer < 0 || m_pKeyFrameObj == NULL)
		return;

	int block = m_wndTimeAixes.GetSelectedBox();
	if(block < 0)	return;
	CTimeLayer* pLayer = m_wndTimeAixes.GetLayer(0);
	int area = pLayer->IsFirstBlock(block);
	if(area < 0)
	{//ûѡ�У�׼���ڴ˿հ�λ����ӹؼ�֡
		UINT tick = (UINT)m_wndTimeAixes.GetBoxValue(block);
		CString str;
		str.Format("������ %d ���루%g �룩����ӹؼ�֡��������", tick, tick / 1000.0f);
		CNewKeyframeDlg dlg;
		dlg.m_strDesc = str;
		for(int i = 0; i < m_pKeyFrameObj->GetKeyFrameCount(); i++)
		{
			KFINFO ki;
			ki.tick = m_pKeyFrameObj->GetKeyFrameTick(i);
			dlg.m_kfInfo.push_back(ki);
		}
		if(IDOK == dlg.DoModal())
		{
			if(dlg.m_type == NKT_LERP)
			{
				m_pKeyFrameObj->AddKeyFrame(tick);
			}
			else if(dlg.m_type == NKT_COPY)
			{
				int pos = m_pKeyFrameObj->AddKeyFrame(tick);
				m_pKeyFrameObj->CopyKeyFrame(dlg.m_iCopyFrom, pos);
			}
			UpdateKeyFrame();
			m_pKeyFrameObj->ActiveKeyFrame(area);
		}

	}
	else
	{//ѡ����
		//UINT tick = (UINT)m_wndTimeAixes.GetBoxValue(block);
		//CString str;
		//str.Format("����ɾ��λ�� %d ���루%g �룩�Ĺؼ�֡%d��������", tick, tick / 1000.0f, area + 1);
		//if(IDYES == MessageBox(str.GetBuffer(), NULL, MB_YESNO))
		//{
		//	m_pKeyFrameObj->DeleteKeyFrame(area);
		//	UpdateKeyFrame();
		//}
		m_pKeyFrameObj->ActiveKeyFrame(area);
	}
	*pResult = 0;
}

void CKeyFrameWnd::OnPosChange(NMHDR* pHdr, LRESULT* pResult)
{
	TANMHDR* pTanmhdr = (TANMHDR*)pHdr;
	if(pTanmhdr->layer < 0 || m_pKeyFrameObj == NULL)
		return;
	UINT tick = (UINT)m_wndTimeAixes.GetLayerStartValue(0,  pTanmhdr->area);
	m_pKeyFrameObj->SetKeyFramePos(pTanmhdr->area, tick);
	*pResult = 0;
}

void CKeyFrameWnd::SetKeyFrameObj(CKeyFrameObject* pObj)
{
	m_pKeyFrameObj = pObj;
	UpdateKeyFrame();
}

void CKeyFrameWnd::UpdateKeyFrame()
{
	CTimeLayer layer;
	layer.m_blocks.clear();
	if(m_pKeyFrameObj == NULL)
		return;

	for(int i = 0; i < m_pKeyFrameObj->GetKeyFrameCount(); i++)
	{
		CTimeArea area;

		area.m_bAllowPos = (i == 0) ? false : true;
		area.m_bAllowSize = false;
		area.m_iLen = 1;
		area.m_iFirst = 0;

		layer.m_blocks.push_back(area);

	}
	layer.m_strName = m_pKeyFrameObj->GetName();

	m_wndTimeAixes.ClearLayers();
	m_wndTimeAixes.AddLayer(layer);

	for(int i = m_pKeyFrameObj->GetKeyFrameCount() - 1; i >= 0 ; i--)
	{
		int tick = m_pKeyFrameObj->GetKeyFrameTick(i);
		m_wndTimeAixes.SetLayerStartValue(0, (float)tick, i);
	}


}


BOOL CKeyFrameWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam != VK_DELETE)
			return CBCGPDockingControlBar::PreTranslateMessage(pMsg);
		int block = m_wndTimeAixes.GetSelectedBox();
		CTimeLayer* pLayer = m_wndTimeAixes.GetLayer(0);
		int area = pLayer->IsFirstBlock(block);
		if(area > 0)
		{//ѡ����
			UINT tick = (UINT)m_wndTimeAixes.GetBoxValue(block);
			CString str;
			str.Format("����ɾ��λ�� %d ���루%g �룩�Ĺؼ�֡%d��������", tick, tick / 1000.0f, area + 1);
			if(IDYES == MessageBox(str.GetBuffer(), NULL, MB_YESNO))
			{
				m_pKeyFrameObj->DeleteKeyFrame(area);
				UpdateKeyFrame();
			}
		}
	}
	return CBCGPDockingControlBar::PreTranslateMessage(pMsg);
}
