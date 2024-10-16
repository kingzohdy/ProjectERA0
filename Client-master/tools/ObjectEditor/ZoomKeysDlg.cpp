// ZoomKeysDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "ZoomKeysDlg.h"


// CZoomKeysDlg dialog

IMPLEMENT_DYNAMIC(CZoomKeysDlg, CDialog)

CZoomKeysDlg::CZoomKeysDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZoomKeysDlg::IDD, pParent)
	, m_fZoom(0)
{

}

CZoomKeysDlg::~CZoomKeysDlg()
{
}

void CZoomKeysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ZOOM, m_wndList);
	DDX_Control(pDX, IDC_SLIDER_ZOOM, m_sldZoom);
	DDX_Text(pDX, IDC_EDIT_ZOOM, m_fZoom);
}


BEGIN_MESSAGE_MAP(CZoomKeysDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT_ZOOM, OnEnChangeEditZoom)
	ON_WM_HSCROLL()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ZOOM, OnLvnItemchangedList)
	ON_BN_CLICKED(IDC_BTN_DELETE_COLORKEY, OnBnClickedBtnDelete)
END_MESSAGE_MAP()


// CZoomKeysDlg message handlers
BOOL CZoomKeysDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_sldZoom.SetRange(0, 340, TRUE);
	int pos =(int)(m_fZoom + 170);
	m_sldZoom.SetPos(pos);

	m_wndList.InsertColumn(0, "关键帧", LVCFMT_LEFT, 80);
	m_wndList.InsertColumn(1, "时间位置", LVCFMT_LEFT, 40);

	m_bNeedRefresh = false;

	UpdateList();
	UpdatePane();
	UpdateData(FALSE);

	return TRUE; 
}

void CZoomKeysDlg::OnOK()
{

	UpdateData();

	SaveCurKf();
	CDialog::OnOK();
}

void CZoomKeysDlg::OnEnChangeEditZoom()
{
	UpdateData();
	int pos =(int)(m_fZoom + 170);
	m_sldZoom.SetPos(pos);

	// TODO:  Add your control notification handler code here
}

void CZoomKeysDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int pos =  m_sldZoom.GetPos();
	m_fZoom = (float)(pos - 170);
	UpdateData(FALSE);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CZoomKeysDlg::UpdateList()
{
	m_wndList.DeleteAllItems();
	for(int i = 0; i < (int)m_zoomkeys.m_Array.size(); i++)
	{
		CString name;
		name.Format("关键帧 %d", i+1);
		CString desc;
		desc.Format("%g 秒", m_zoomkeys.m_Array[i].tick / 1000.0f);
		m_wndList.InsertItem(LVIF_TEXT, i, name.GetBuffer(), 0, 0, 0, 0);
		m_wndList.SetItem(i, 1, LVIF_TEXT, desc.GetBuffer(),0, 0, 0, 0, 0);
	}
}

void CZoomKeysDlg::UpdatePane()
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
		int pos =(int)( m_fZoom + 170);
		m_sldZoom.SetPos(pos);
	}
}

void CZoomKeysDlg::SetCurKf(int index)
{
	m_curkf = index;
	if(m_curkf >= 0 && m_curkf < (int)m_zoomkeys.m_Array.size())
	{
		m_fZoom = m_zoomkeys.m_Array[index].data;
	}

	UpdatePane();
}

int CZoomKeysDlg::GetCurKf()
{
	return m_curkf;
}

void CZoomKeysDlg::SaveCurKf()
{
	if(m_curkf >= 0 && m_curkf < (int)m_zoomkeys.m_Array.size())
	{
		m_zoomkeys.m_Array[m_curkf].data = m_fZoom;
	}
}

void CZoomKeysDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
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

void CZoomKeysDlg::OnBnClickedBtnDelete()
{
	if(m_curkf > 0 && m_curkf < (int) m_zoomkeys.m_Array.size())
	{
		m_zoomkeys.m_Array.erase(m_zoomkeys.m_Array.begin() + m_curkf);
		m_bNeedRefresh = true;

		if(m_curkf == 0) m_curkf++;
		if(m_curkf >= (int)m_zoomkeys.m_Array.size()) m_curkf--;
		if(m_zoomkeys.m_Array.size() == 0) m_curkf = -1;
		UpdateList();
		SetCurKf(m_curkf);
	}
}