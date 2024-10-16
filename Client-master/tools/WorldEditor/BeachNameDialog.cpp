// BeachNameDialog.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "BeachNameDialog.h"


// CBeachNameDialog dialog

IMPLEMENT_DYNAMIC(CBeachNameDialog, CDialog)

CBeachNameDialog::CBeachNameDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CBeachNameDialog::IDD, pParent)
	, m_BeachName(_T(""))
{

}

CBeachNameDialog::~CBeachNameDialog()
{
}

void CBeachNameDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BEACHNAME, m_BeachName);
}


BEGIN_MESSAGE_MAP(CBeachNameDialog, CDialog)
	ON_EN_CHANGE(IDC_EDIT_BEACHNAME, &CBeachNameDialog::OnEnChangeEditBeachname)
	ON_BN_CLICKED(IDC_BEACHNAMEOK, &CBeachNameDialog::OnBnClickedBeachnameok)
END_MESSAGE_MAP()


// CBeachNameDialog message handlers

void CBeachNameDialog::OnEnChangeEditBeachname()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CBeachNameDialog::OnBnClickedBeachnameok()
{
	// TODO: Add your control notification handler code here
	UpdateData(false);

	EndDialog(1);
}
