// ColorKeysDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "ColorKeysDlg.h"


// CColorKeysDlg dialog
#define IDC_COLORBAR 100

IMPLEMENT_DYNAMIC(CColorKeysDlg, CDialog)

CColorKeysDlg::CColorKeysDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColorKeysDlg::IDD, pParent)
	, m_fAlpha(0)
{

}

CColorKeysDlg::~CColorKeysDlg()
{
}

void CColorKeysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_COLORKEY, m_wndColorFrame);
	DDX_Control(pDX, IDC_SLIDER_COLORKEY_ALPHA, m_sldAlpha);
	DDX_Text(pDX, IDC_EDIT_COLORKEY_ALPHA, m_fAlpha);
	DDX_Control(pDX, IDC_LIST_COLORKEY, m_wndList);
}


BEGIN_MESSAGE_MAP(CColorKeysDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT_COLORKEY_ALPHA, OnEnChangeEditAlpha)
	ON_WM_HSCROLL()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_COLORKEY, OnLvnItemchangedList)
	ON_BN_CLICKED(IDC_BTN_DELETE_COLORKEY, OnBnClickedBtnDelete)
END_MESSAGE_MAP()


// CColorKeysDlg message handlers
BOOL CColorKeysDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rectColorBar;
	m_wndColorFrame.GetClientRect (&rectColorBar);
	m_wndColorFrame.MapWindowPoints (this, &rectColorBar);

	m_wndColor.SetHorzMargin (0);
	m_wndColor.SetVertMargin (0);
	m_wndColor.EnableOtherButton (_T("更多颜色..."));

	m_wndColor.CreateControl (this, rectColorBar, IDC_COLORBAR, 7 /* columns */);
	COLORREF clr = RGB((int)(m_color.r * 255), (int)(m_color.g * 255), (int)(m_color.b * 255));
	m_wndColor.SetColor ( clr );

	m_sldAlpha.SetRange(0, 100, TRUE);
	int pos =(int)( m_fAlpha * 100 );
	m_sldAlpha.SetPos(pos);
	m_fAlpha = m_color.a;

	m_wndList.InsertColumn(0, "关键帧", LVCFMT_LEFT, 80);
	m_wndList.InsertColumn(1, "时间位置", LVCFMT_LEFT, 40);

	m_bNeedRefresh = false;

	UpdateList();
	UpdatePane();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CColorKeysDlg::OnOK()
{
	UpdateData();

	SaveCurKf();
	CDialog::OnOK();
}

void CColorKeysDlg::OnTbMove(NMHDR *pNMHDR, LRESULT *pResult)
{


	*pResult = 0;
}

void CColorKeysDlg::OnEnChangeEditAlpha()
{
	UpdateData();
	int pos =(int)( m_fAlpha * 100 );
	m_sldAlpha.SetPos(pos);

}

void CColorKeysDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int pos =  m_sldAlpha.GetPos();
	m_fAlpha = pos / 100.0f;
	UpdateData(FALSE);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CColorKeysDlg::UpdateList()
{
	m_wndList.DeleteAllItems();
	for(int i = 0; i < (int)m_colorkeys.m_Array.size(); i++)
	{
		CString name;
		name.Format("关键帧 %d", i+1);
		CString desc;
		desc.Format("%g 秒", m_colorkeys.m_Array[i].tick / 1000.0f);
		m_wndList.InsertItem(LVIF_TEXT, i, name.GetBuffer(), 0, 0, 0, 0);
		m_wndList.SetItem(i, 1, LVIF_TEXT, desc.GetBuffer(),0, 0, 0, 0, 0);
	}
}

void CColorKeysDlg::UpdatePane()
{
	if(GetSafeHwnd())
	{
		CString str;
		str.Format("当前编辑关键帧 %d", m_curkf + 1);
		GetDlgItem(IDC_CURKF )->SetWindowText(str.GetBuffer());

		CString str2;
		str2.Format("删除关键帧%d", m_curkf + 1);
		GetDlgItem(IDC_BTN_DELETE_COLORKEY)->SetWindowText(str2.GetBuffer());

		COLORREF clr = RGB((int)(m_color.r * 255), (int)(m_color.g * 255), (int)(m_color.b * 255));

		m_wndColor.SetColor(clr);
		m_fAlpha = m_color.a;
		UpdateData(FALSE);
		int pos =(int)( m_fAlpha * 100 );
		m_sldAlpha.SetPos(pos);
	}
}

void CColorKeysDlg::SetCurKf(int index)
{
	m_curkf = index;
	if(m_curkf >= 0 && m_curkf < (int)m_colorkeys.m_Array.size())
	{
		m_color.r = m_colorkeys.m_Array[index].data.x;
		m_color.g = m_colorkeys.m_Array[index].data.y;
		m_color.b = m_colorkeys.m_Array[index].data.z;
		m_color.a = m_colorkeys.m_Array[index].data.w;

		m_fAlpha = m_color.a;
	}

	UpdatePane();
}

int CColorKeysDlg::GetCurKf()
{
	return m_curkf;
}

void CColorKeysDlg::SaveCurKf()
{
	if(m_curkf >= 0 && m_curkf < (int)m_colorkeys.m_Array.size())
	{
		COLORREF clr = m_wndColor.GetColor();
		m_color.r = GetRValue(clr) / 255.0f;
		m_color.g = GetGValue(clr) / 255.0f;
		m_color.b = GetBValue(clr) / 255.0f;

		m_color.a = m_fAlpha;

		m_colorkeys.m_Array[m_curkf].data.x = m_color.r;
		m_colorkeys.m_Array[m_curkf].data.y = m_color.g;
		m_colorkeys.m_Array[m_curkf].data.z = m_color.b;
		m_colorkeys.m_Array[m_curkf].data.w = m_color.a;

	}
}

void CColorKeysDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if((pNMLV->uNewState & LVIS_SELECTED) == 0 && (pNMLV->uOldState & LVIS_SELECTED) != 0)
	{//被取消选中
		SaveCurKf();
	}


	if((pNMLV->uNewState & LVIS_SELECTED) != 0 && (pNMLV->uOldState & LVIS_SELECTED) == 0)
	{//被选中
		SetCurKf(pNMLV->iItem);
	}
	*pResult = 0;
}

void CColorKeysDlg::OnBnClickedBtnDelete()
{
	if(m_curkf > 0 && m_curkf < (int) m_colorkeys.m_Array.size())
	{
		m_colorkeys.m_Array.erase(m_colorkeys.m_Array.begin() + m_curkf);
		m_bNeedRefresh = true;

		if(m_curkf == 0) m_curkf++;
		if(m_curkf >= (int)m_colorkeys.m_Array.size()) m_curkf--;
		if(m_colorkeys.m_Array.size() == 0) m_curkf = -1;
		UpdateList();
		SetCurKf(m_curkf);
	}
}