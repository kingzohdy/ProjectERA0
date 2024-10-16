// ModifyEffectNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "ModifyEffectNameDlg.h"


// CModifyEffectNameDlg dialog

IMPLEMENT_DYNAMIC(CModifyEffectNameDlg, CBCGPDialog)

CModifyEffectNameDlg::CModifyEffectNameDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CModifyEffectNameDlg::IDD, pParent)
	, m_strText(_T(""))
{
	m_strTitle = " ‰»Î";
	m_strDesc = "«Î ‰»Î";
}

CModifyEffectNameDlg::~CModifyEffectNameDlg()
{
}

void CModifyEffectNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_EFFECTNAME, m_strText);
	DDX_Control(pDX, IDC_EDIT_DESC, m_editDesc);
}


BEGIN_MESSAGE_MAP(CModifyEffectNameDlg, CBCGPDialog)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CModifyEffectNameDlg message handlers
void CModifyEffectNameDlg::SetTitle(CString str)
{
	m_strTitle = str;
}
void CModifyEffectNameDlg::SetDesc(CString str) 
{
	m_strDesc = str;
}


int CModifyEffectNameDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CModifyEffectNameDlg::OnBnClickedOk()
{
	UpdateData();
	OnOK();
}

BOOL CModifyEffectNameDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	SetWindowText(m_strTitle);
	m_editDesc.SetWindowText( m_strDesc.GetBuffer());
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}