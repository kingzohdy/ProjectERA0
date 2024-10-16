
#include "stdafx.h"
#include "ResourceReaderBar_Texture.h"
#include "ResourceReader.h"
#include "ObjectTypeHelper.h"

IMPLEMENT_DYNAMIC(CChunkDetailBarTex, CChunkDetailBar)
CChunkDetailBarTex::CChunkDetailBarTex()
{
}

CChunkDetailBarTex::~CChunkDetailBarTex()
{
}


BEGIN_MESSAGE_MAP(CChunkDetailBarTex, CChunkDetailBar)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CChunkDetailBarTex message handlers


void CChunkDetailBarTex::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rt;
	GetClientRect((LPRECT)rt);

	CBrush br(RGB(255, 200, 200));
	dc.FillRect(rt, &br);

	CString str = "Texture : ";
	str += m_pTexDtl->m_strChunkName;
	dc.SetBkMode(TRANSPARENT);
	dc.DrawTextEx(str , CRect(0, 0, rt.Width(), rt.Height())
		, DT_WORD_ELLIPSIS | DT_LEFT |DT_END_ELLIPSIS | DT_TOP | DT_SINGLELINE, NULL);
}

int CChunkDetailBarTex::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CChunkDetailBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	CCvtHelper cvt;

	m_wndList.Create(LVS_REPORT | WS_VISIBLE | LVS_NOCOLUMNHEADER , CRect(0, 0, 100, 100), this, 1);

	CString name = GetChunkTitle(m_pTexDtl->m_strChunkName);

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

	m_wndList.InsertColumn(0, "", LVCFMT_LEFT, 100);
	m_wndList.InsertColumn(1, "", LVCFMT_LEFT, 100);

	LVITEM item;
	ZeroMemory(&item, sizeof(item));
	item.mask = LVIF_GROUPID;
	item.iGroupId = 0;


	m_wndList.InsertItem(LVIF_TEXT, 0, "¿í¶È", 0, 0, 0, 0);
	m_wndList.SetItem(0, 1, LVIF_TEXT, cvt.IntToString(m_pTexDtl->m_iWidth),0, 0, 0, 0, 0);
	item.iItem = 0;
	m_wndList.SetItem(&item);

	m_wndList.InsertItem(LVIF_TEXT, 1, "¸ß¶È", 0, 0, 0, 0);
	m_wndList.SetItem(1, 1, LVIF_TEXT, cvt.IntToString(m_pTexDtl->m_iHeight),0, 0, 0, 0, 0);
	item.iItem = 1;
	m_wndList.SetItem(&item);

	m_wndList.SetExtendedStyle(LVS_EX_FULLROWSELECT | m_wndList.GetExtendedStyle());
	m_wndList.EnableGroupView(TRUE);



	return 0;
}

void CChunkDetailBarTex::OnSize(UINT nType, int cx, int cy)
{
	CChunkDetailBar::OnSize(nType, cx, cy);

	CRect rt;
	GetClientRect((LPRECT)rt);
	m_wndList.SetWindowPos(NULL, 0, 0, rt.Width(), rt.Height(), SWP_NOMOVE | SWP_NOZORDER);
}
