// MailDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OffLine.h"
#include "MailDialog.h"


// CMailDialog dialog

IMPLEMENT_DYNAMIC(CMailDialog, CDialog)

CMailDialog::CMailDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMailDialog::IDD, pParent)
{

}

CMailDialog::~CMailDialog()
{
}

void CMailDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMailDialog, CDialog)
END_MESSAGE_MAP()


// CMailDialog message handlers
