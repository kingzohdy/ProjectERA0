// EularEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EularEditDlg.h"


// CEularEditDlg dialog

IMPLEMENT_DYNAMIC(CEularEditDlg, CDialog)

CEularEditDlg::CEularEditDlg(CWnd* pParent /*=NULL*/)
: CDialog(CEularEditDlg::IDD, pParent)
, m_fRoll(0)
, m_fYaw(0)
, m_fPitch(0)
{

}

CEularEditDlg::~CEularEditDlg()
{
}

void CEularEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ROLL, m_fRoll);
	DDX_Text(pDX, IDC_EDIT_YAW, m_fYaw);
	DDX_Text(pDX, IDC_EDIT_PITCH, m_fPitch);
}


BEGIN_MESSAGE_MAP(CEularEditDlg, CDialog)
END_MESSAGE_MAP()


// CEularEditDlg message handlers
