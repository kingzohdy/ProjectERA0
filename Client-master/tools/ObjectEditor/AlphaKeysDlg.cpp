// AlphaKeysDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "AlphaKeysDlg.h"


// CAlphaKeysDlg dialog

IMPLEMENT_DYNAMIC(CAlphaKeysDlg, CDialog)

CAlphaKeysDlg::CAlphaKeysDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAlphaKeysDlg::IDD, pParent)
	, m_fAlpha(0)
{

}

CAlphaKeysDlg::~CAlphaKeysDlg()
{
}

void CAlphaKeysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COLORKEY, m_wndList);
	DDX_Control(pDX, IDC_SLIDER_COLORKEY_ALPHA, m_sldAlpha);
	DDX_Text(pDX, IDC_EDIT_COLORKEY_ALPHA, m_fAlpha);
}


BEGIN_MESSAGE_MAP(CAlphaKeysDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT_COLORKEY_ALPHA, OnEnChangeEditAlpha)
	ON_WM_HSCROLL()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_COLORKEY, OnLvnItemchangedList)
	ON_BN_CLICKED(IDC_BTN_DELETE_COLORKEY, OnBnClickedBtnDelete)
END_MESSAGE_MAP()


// CAlphaKeysDlg message handlers
BOOL CAlphaKeysDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_sldAlpha.SetRange(0, 100, TRUE);
	int pos =(int)( m_fAlpha * 100 );
	m_sldAlpha.SetPos(pos);

	m_wndList.InsertColumn(0, "关键帧", LVCFMT_LEFT, 80);
	m_wndList.InsertColumn(1, "时间位置", LVCFMT_LEFT, 40);

	m_bNeedRefresh = false;

	UpdateList();
	UpdatePane();
	UpdateData(FALSE);

	return TRUE; 
}

void CAlphaKeysDlg::OnOK()
{

	UpdateData();

	SaveCurKf();
	CDialog::OnOK();
}

void CAlphaKeysDlg::OnEnChangeEditAlpha()
{
	UpdateData();
	int pos =(int)( m_fAlpha * 100 );
	m_sldAlpha.SetPos(pos);

	// TODO:  Add your control notification handler code here
}

void CAlphaKeysDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int pos =  m_sldAlpha.GetPos();
	m_fAlpha = pos / 100.0f;
	UpdateData(FALSE);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CAlphaKeysDlg::UpdateList()
{
	m_wndList.DeleteAllItems();
	for(int i = 0; i < (int)m_alphakeys.m_Array.size(); i++)
	{
		CString name;
		name.Format("关键帧 %d", i+1);
		CString desc;
		desc.Format("%g 秒", m_alphakeys.m_Array[i].tick / 1000.0f);
		m_wndList.InsertItem(LVIF_TEXT, i, name.GetBuffer(), 0, 0, 0, 0);
		m_wndList.SetItem(i, 1, LVIF_TEXT, desc.GetBuffer(),0, 0, 0, 0, 0);
	}
}

void CAlphaKeysDlg::UpdatePane()
{
	if(GetSafeHwnd())
	{
		CString str;
		str.Format("当前编辑关键帧 %d", m_curkf + 1);
		GetDlgItem(IDC_CURKF )->SetWindowText(str.GetBuffer());

		CString str2;
		str2.Format("删除关键帧%d", m_curkf + 1);
		GetDlgItem(IDC_BTN_DELETE_COLORKEY )->SetWindowText(str2.GetBuffer());

		UpdateData(FALSE);
		int pos =(int)( m_fAlpha * 100 );
		m_sldAlpha.SetPos(pos);
	}
}

void CAlphaKeysDlg::SetCurKf(int index)
{
	m_curkf = index;
	if(m_curkf >= 0 && m_curkf < (int)m_alphakeys.m_Array.size())
	{
		m_fAlpha = m_alphakeys.m_Array[index].data;
	}

	UpdatePane();
}

int CAlphaKeysDlg::GetCurKf()
{
	return m_curkf;
}

void CAlphaKeysDlg::SaveCurKf()
{
	if(m_curkf >= 0 && m_curkf < (int)m_alphakeys.m_Array.size())
	{
		m_alphakeys.m_Array[m_curkf].data = m_fAlpha;
	}
}

void CAlphaKeysDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
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

void CAlphaKeysDlg::OnBnClickedBtnDelete()
{
	if(m_curkf > 0 && m_curkf < (int) m_alphakeys.m_Array.size())
	{
		m_alphakeys.m_Array.erase(m_alphakeys.m_Array.begin() + m_curkf);
		m_bNeedRefresh = true;

		if(m_curkf == 0) m_curkf++;
		if(m_curkf >= (int)m_alphakeys.m_Array.size()) m_curkf--;
		if(m_alphakeys.m_Array.size() == 0) m_curkf = -1;
		UpdateList();
		SetCurKf(m_curkf);
	}
}