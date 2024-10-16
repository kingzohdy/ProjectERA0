// SectionPointInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "SectionPointInfoDlg.h"


// CSectionPointInfoDlg dialog

IMPLEMENT_DYNAMIC(CSectionPointInfoDlg, CDialog)

CSectionPointInfoDlg::CSectionPointInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSectionPointInfoDlg::IDD, pParent)
	, m_fTexV(0)
	, m_fPosX(0)
	, m_fPosY(0)
{

}

CSectionPointInfoDlg::~CSectionPointInfoDlg()
{
}

void CSectionPointInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_UVOFFSET, m_fTexV);
	DDX_Text(pDX, IDC_EDIT_SECTION_X, m_fPosX);
	DDX_Text(pDX, IDC_EDIT_SECTION_Y, m_fPosY);
}


BEGIN_MESSAGE_MAP(CSectionPointInfoDlg, CDialog)
END_MESSAGE_MAP()


// CSectionPointInfoDlg message handlers
