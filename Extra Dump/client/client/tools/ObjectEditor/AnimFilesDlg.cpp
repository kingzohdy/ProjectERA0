// AnimFilesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "AnimFilesDlg.h"
#include "EffectManager.h"
#include "SkeletonInfo.h"


// CAnimFilesDlg dialog

IMPLEMENT_DYNAMIC(CAnimFilesDlg, CDialog)

CAnimFilesDlg::CAnimFilesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnimFilesDlg::IDD, pParent)
{

}

CAnimFilesDlg::~CAnimFilesDlg()
{
}

void CAnimFilesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ANIMFILES, m_wndList);
}


BEGIN_MESSAGE_MAP(CAnimFilesDlg, CDialog)
END_MESSAGE_MAP()


// CAnimFilesDlg message handlers
BOOL CAnimFilesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_wndList.InsertColumn(0, "引入的动画文件", LVCFMT_LEFT, 300);

	CSkeleton* pSkeleton = CEffectMgr::GetInstance()->m_skeleton;
	for(int i = 0; i < (int)pSkeleton->m_animFiles.size(); i++)
	{
		m_wndList.InsertItem(i, pSkeleton->m_animFiles[i].m_filename.c_str(), 0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAnimFilesDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	int sel = m_wndList.GetSelectionMark();
	if(sel < 0)
	{
		MessageBox("没有选择任何项");
	}
	else
	{
		m_strSel = m_wndList.GetItemText(sel, 0);
		CDialog::OnOK();
	}
}