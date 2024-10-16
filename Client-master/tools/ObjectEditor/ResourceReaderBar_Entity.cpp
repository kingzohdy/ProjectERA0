
#include "stdafx.h"
#include "ResourceReaderBar_Entity.h"
#include "ObjectTypeHelper.h"
#include "ResourceReader.h"
#include "EffectManager.h"

IMPLEMENT_DYNAMIC(CChunkDetailBarShowMod, CChunkDetailBar)
CChunkDetailBarShowMod::CChunkDetailBarShowMod()
{
}

CChunkDetailBarShowMod::~CChunkDetailBarShowMod()
{
}


BEGIN_MESSAGE_MAP(CChunkDetailBarShowMod, CChunkDetailBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ITEMACTIVATE, 1, OnItemActive)
END_MESSAGE_MAP()



// CChunkDetailBarShowMod message handlers


int CChunkDetailBarShowMod::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CChunkDetailBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	CCvtHelper cvt;

	m_wndList.Create(LVS_REPORT | WS_VISIBLE | LVS_NOCOLUMNHEADER , CRect(0, 0, 100, 100), this, 1);

	CString name = GetChunkTitle(m_pModDtl->m_strChunkName);

	LVGROUP   msw_grp;
	ZeroMemory(&msw_grp, sizeof(msw_grp));
	msw_grp.uAlign   =   0;     
	msw_grp.mask   =   LVGF_GROUPID   |   LVGF_HEADER   |   LVGF_STATE   ;     
	msw_grp.state   =   LVGS_NORMAL;     
	msw_grp.stateMask   =   LVGS_NORMAL;     
	msw_grp.pszHeader = cvt.GbToWide(name.GetBuffer());
	msw_grp.iGroupId   =   0;       
	msw_grp.cchHeader   =   wcslen(msw_grp.pszHeader);     
	msw_grp.cbSize   =   sizeof(LVGROUP);     


	m_wndList.InsertGroup(0, &msw_grp);

	m_wndList.InsertColumn(0, "", LVCFMT_LEFT, 170);
	m_wndList.InsertColumn(1, "", LVCFMT_LEFT, 30);

	LVITEM item;
	ZeroMemory(&item, sizeof(item));
	item.mask = LVIF_GROUPID;
	item.iGroupId = 0;


	//m_wndList.InsertItem(LVIF_TEXT, 0, "宽度", 0, 0, 0, 0);
	//m_wndList.SetItem(0, 1, LVIF_TEXT, cvt.IntToString(m_pTexDtl->m_iWidth),0, 0, 0, 0, 0);
	//item.iItem = 0;
	//m_wndList.SetItem(&item);

	//m_wndList.InsertItem(LVIF_TEXT, 1, "高度", 0, 0, 0, 0);
	//m_wndList.SetItem(1, 1, LVIF_TEXT, cvt.IntToString(m_pTexDtl->m_iHeight),0, 0, 0, 0, 0);
	//item.iItem = 1;
	//m_wndList.SetItem(&item);

	for(int i = 0; i < (int) m_pModDtl->m_MotionNames.size(); i++)
	{
		CString& motionName = m_pModDtl->m_MotionNames[i];
		m_wndList.InsertItem(LVIF_TEXT, i, motionName.GetBuffer(), 0, 0, 0, 0);
		//m_wndList.SetItem(i, 1, LVIF_TEXT, "双击加载并播放",0, 0, 0, 0, 0);
		item.iItem = i;
		m_wndList.SetItem(&item);
	}

	m_wndList.SetExtendedStyle(LVS_EX_FULLROWSELECT | m_wndList.GetExtendedStyle());
	m_wndList.EnableGroupView(TRUE);



	return 0;
}

void CChunkDetailBarShowMod::OnSize(UINT nType, int cx, int cy)
{
	CChunkDetailBar::OnSize(nType, cx, cy);


	CRect rt;
	GetClientRect((LPRECT)rt);
	m_wndList.SetWindowPos(NULL, 0, 0, rt.Width(), rt.Height(), SWP_NOMOVE | SWP_NOZORDER);
}

void CChunkDetailBarShowMod::OnItemActive(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW* pNM = (NMLISTVIEW*)pNMHDR;
	int iItem = pNM->iItem;
	if(iItem < 0 || iItem >= (int)m_pModDtl->m_MotionNames.size())
	{
		assert( 0 && "iItem < 0 || iItem >= m_pModDtl->m_MotionNames.size()");
		return;
	}

	CString strMotion = m_pModDtl->m_MotionNames[iItem];

	if(CEffectMgr::GetInstance()->m_strSmName != m_pModDtl->GetChunkName())
	{
		char* temp = new char[512];
		strcpy(temp, m_pModDtl->GetChunkName().GetBuffer());
		SendMainFrameMsg(EEM_REQ_LOAD_ACTOR, 0,(LPARAM)temp); //MainForm自行销毁buffer
	}

	CEffectMgr::GetInstance()->PlayMotion(strMotion.GetBuffer());

}