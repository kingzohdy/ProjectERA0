// PetDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OffLine.h"
#include "PetDialog.h"


// CPetDialog dialog

IMPLEMENT_DYNAMIC(CPetDialog, CDialog)

CPetDialog::CPetDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPetDialog::IDD, pParent)
{

}

CPetDialog::~CPetDialog()
{
}

void CPetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PETSHOW, m_PetRenderWnd);
}


BEGIN_MESSAGE_MAP(CPetDialog, CDialog)
END_MESSAGE_MAP()


// CPetDialog message handlers
