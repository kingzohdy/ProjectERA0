
#include "stdafx.h"
#include "ResourceReaderBar_PreviewModel.h"
#include "ResourceReader.h"
#include "EffectManager.h"

CChunkDetailBarPreviewModel::CChunkDetailBarPreviewModel(void)
{

}

CChunkDetailBarPreviewModel::~CChunkDetailBarPreviewModel(void)
{
}


IMPLEMENT_DYNAMIC(CChunkDetailBarPreviewModel, CChunkDetailBar);


BEGIN_MESSAGE_MAP(CChunkDetailBarPreviewModel, CChunkDetailBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

#define RENDER_EVENT 1

int CChunkDetailBarPreviewModel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CChunkDetailBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CRect rc;
	GetClientRect(rc);
	m_wndRenderStatic.Create(rc, this, 1);

	m_wndRenderStatic.ShowWindow(SW_SHOW);

	m_wndRenderStatic.StartRender();
	CEffectMgr::GetInstance()->AddFrameJob(&m_wndRenderStatic);

	//MessageBox("m_wndRenderStatic.StartRender();成功");
	m_wndRenderStatic.SetModel(m_pDetail->GetPreviewObject());
	m_wndRenderStatic.m_bDrawOnce = false;
	//MessageBox("m_wndRenderStatic.SetModel(m_pDetail->m_pNode);成功");
	m_wndRenderStatic.m_bSelfDraw = false;
	m_bFirst = true;
	//::SetTimer(GetSafeHwnd(), RENDER_EVENT, 30, NULL);

	return 0;
}

void CChunkDetailBarPreviewModel::OnSize(UINT nType, int cx, int cy)
{
	CChunkDetailBar::OnSize(nType, cx, cy);

	CRect rc;
	GetClientRect(rc);

	m_wndRenderStatic.SetWindowPos(NULL, 0, 0, rc.Width(), rc.Height(), SWP_NOZORDER);
}

void CChunkDetailBarPreviewModel::OnDestroy()
{	
	m_wndRenderStatic.ShutRender();
	CEffectMgr::GetInstance()->RemoveFrameJob(&m_wndRenderStatic);
	CChunkDetailBar::OnDestroy();

	// TODO: Add your message handler code here
}

void CChunkDetailBarPreviewModel::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case RENDER_EVENT:
		{

			if(m_bFirst)
			{
				//m_bFirst = false;
				//m_wndRenderStatic.DoFrame(true);
				//MessageBox("第一次m_wndRenderStatic.DoFrame();成功");
			}
			else
			{
				//m_wndRenderStatic.DoFrame();
			}
			//m_wndRenderStatic.DoFrame();


		}
		break;
	default:
		break;
	}
	CChunkDetailBar::OnTimer(nIDEvent);
}
