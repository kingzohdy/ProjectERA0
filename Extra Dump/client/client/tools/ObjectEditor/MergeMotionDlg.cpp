// MergeMotionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "MergeMotionDlg.h"
#include "EffectManager.h"
#include "ModelMotionInfo.h"


// CMergeMotionDlg dialog

IMPLEMENT_DYNAMIC(CMergeMotionDlg, CDialog)

CMergeMotionDlg::CMergeMotionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMergeMotionDlg::IDD, pParent)
	, m_bNoDelete(TRUE)
{

}

CMergeMotionDlg::~CMergeMotionDlg()
{
}

void CMergeMotionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MERGETO, m_cmbMotionList);
	DDX_Check(pDX, IDC_CHECK_NO_DELETE, m_bNoDelete);
}


BEGIN_MESSAGE_MAP(CMergeMotionDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CMergeMotionDlg message handlers
BOOL CMergeMotionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CModelMotionInfo* pMotionInfo = CEffectMgr::GetInstance()->m_modelMotion;
	for(int i = 0; i < (int)pMotionInfo->m_motions.size(); i++)
	{
		CModelMotionInfo::MOTION* pMotion = pMotionInfo->m_motions[i];
		m_cmbMotionList.AddString( pMotion->name.c_str());
	}

	CString strTitle;
	strTitle.Format("将选定特效 “%s” 合并到", m_strFromMotion.GetBuffer());
	GetDlgItem(IDC_STATIC_TITLE)->SetWindowText(strTitle.GetBuffer());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMergeMotionDlg::OnBnClickedOk()
{
	OnOK();
}

void CMergeMotionDlg::OnOK()
{
	CString strSel;
	m_cmbMotionList.GetWindowText(strSel);

	m_iToMotion = -1;
	strSel.MakeUpper();
	m_strFromMotion.MakeUpper();
	CModelMotionInfo* pMotionInfo = CEffectMgr::GetInstance()->m_modelMotion;
	for(int i = 0; i < (int)pMotionInfo->m_motions.size(); i++)
	{
		CModelMotionInfo::MOTION* pMotion = pMotionInfo->m_motions[i];
		CString strMotionName = pMotion->name.c_str();
		strMotionName.MakeUpper();
		if(strSel == strMotionName && strSel != m_strFromMotion)
		{
			m_iToMotion = i;
			break;
		}
	}

	if(m_iToMotion == -1)
	{
		MessageBox("选择的特效不存在，或者选择的特效是源特效");
	}
	else
	{
		CDialog::OnOK();
	}
}