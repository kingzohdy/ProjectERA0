// NewKeyframeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "NewKeyframeDlg.h"


// CNewKeyframeDlg dialog

IMPLEMENT_DYNAMIC(CNewKeyframeDlg, CDialog)

CNewKeyframeDlg::CNewKeyframeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewKeyframeDlg::IDD, pParent)
	, m_strDesc(_T(""))
{

}

CNewKeyframeDlg::~CNewKeyframeDlg()
{
}

void CNewKeyframeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DESC, m_strDesc);
	DDX_Control(pDX, IDC_LIST_KEYFRAME, m_list);
}


BEGIN_MESSAGE_MAP(CNewKeyframeDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_KEYFRAME_LERP, OnBnClickedRdoLerp)
	ON_BN_CLICKED(IDC_RADIO_KEYFRAME_COPY, OnBnClickedRdoCopy)
END_MESSAGE_MAP()


// CNewKeyframeDlg message handlers
void CNewKeyframeDlg::OnBnClickedRdoLerp()
{
	m_type = NKT_LERP;
	if(GetDlgItem(IDC_RADIO_KEYFRAME_LERP)->SendMessage(BM_GETCHECK) == BST_CHECKED)
	{
		m_list.EnableWindow(FALSE);
	}
	else if(GetDlgItem(IDC_RADIO_KEYFRAME_COPY)->SendMessage(BM_GETCHECK) == BST_CHECKED)
	{
		m_list.EnableWindow(TRUE);
	}
}

void CNewKeyframeDlg::OnBnClickedRdoCopy()
{
	m_type = NKT_COPY;
	if(GetDlgItem(IDC_RADIO_KEYFRAME_LERP)->SendMessage(BM_GETCHECK) == BST_CHECKED)
	{
		m_list.EnableWindow(FALSE);
	}
	else if(GetDlgItem(IDC_RADIO_KEYFRAME_COPY)->SendMessage(BM_GETCHECK) == BST_CHECKED)
	{
		m_list.EnableWindow(TRUE);
	}
}


BOOL CNewKeyframeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_list.DeleteAllItems();
	m_list.InsertColumn(0, "�ؼ�֡", LVCFMT_LEFT, 100, -1);
	m_list.InsertColumn(1, "ʱ��λ��", LVCFMT_LEFT, 100, -1);

	for(int i = 0; i < (int)m_kfInfo.size(); i++)
	{
		CString name;
		CString timepos;

		name.Format("�ؼ�֡ %d", i + 1);
		float f = m_kfInfo[i].tick / 1000.0f;
		if(i == 0) f = 0.0f;
		timepos.Format(" %g ��", f);
		m_list.InsertItem(LVIF_TEXT, i, name.GetBuffer(), 0, 0, 0, 0);
		m_list.SetItem(i, 1, LVIF_TEXT, timepos.GetBuffer(), 0, 0, 0, 0, 0);
	}
	GetDlgItem(IDC_RADIO_KEYFRAME_LERP)->SendMessage(BM_SETCHECK, BST_CHECKED, 0);
	OnBnClickedRdoLerp();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CNewKeyframeDlg::OnOK()
{
	if(m_type == NKT_COPY)
	{
		int sel = m_list.GetSelectionMark();
		if(sel < 0)
		{
			MessageBox("û��ѡ���ƵĹؼ�֡");
			return;
		}
		m_iCopyFrom = sel;
	}

	CDialog::OnOK();
}