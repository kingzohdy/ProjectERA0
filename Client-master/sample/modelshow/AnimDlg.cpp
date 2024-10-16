// AnimDlg.cpp : implementation file
//

#include "stdafx.h"
#include "modelshow.h"
#include "AnimDlg.h"


// CAnimDlg dialog

IMPLEMENT_DYNAMIC(CAnimDlg, CDialog)

CAnimDlg::CAnimDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnimDlg::IDD, pParent)
	, m_AnimId(0)
{

}

CAnimDlg::~CAnimDlg()
{
}

void CAnimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT1,m_AnimId);
}


BEGIN_MESSAGE_MAP(CAnimDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAnimDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAnimDlg message handlers

void CAnimDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_AnimId = GetDlgItemInt(IDC_EDIT1);
	OnOK();
}
