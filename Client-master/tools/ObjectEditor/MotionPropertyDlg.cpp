// MotionPropertyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "MotionPropertyDlg.h"
#include "EffectManager.h"

#include "OgreModel.h"
#include "OgreAnimationData.h"
#include "OgreEntity.h"
#include <vector>


// CMotionPropertyDlg dialog

IMPLEMENT_DYNAMIC(CMotionPropertyDlg, CDialog)

CMotionPropertyDlg::CMotionPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMotionPropertyDlg::IDD, pParent)
	, m_strAnim(_T(""))
	, m_fTime(0)
	, m_strMotionName(_T(""))
	, m_bPublic(FALSE)
{

}

CMotionPropertyDlg::~CMotionPropertyDlg()
{
}

void CMotionPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_CMB_AN_MAINANIM, m_strAnim);
	DDX_Text(pDX, IDC_EDIT_AN_TIME, m_fTime);
	DDX_Text(pDX, IDC_EDIT_AN_NAME, m_strMotionName);
	DDX_Check(pDX, IDC_CHECK_PUBLIC, m_bPublic);
}


BEGIN_MESSAGE_MAP(CMotionPropertyDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CMotionPropertyDlg message handlers
BOOL CMotionPropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_CMB_AN_MAINANIM)->SendMessage(CB_ADDSTRING, 0, (LPARAM)"нч");

	Ogre::Model *pmodel = CEffectMgr::GetInstance()->GetModelActor()->getMainModel();
	if(pmodel)
	{
		std::vector<Ogre::SequenceDesc>seqarray;
		pmodel->getModelData()->getAllSequence(seqarray);
		for(size_t i=0; i<seqarray.size(); i++)
		{
			CString strSeqName;
			strSeqName.Format(_T("%d"), seqarray[i].id);
			GetDlgItem(IDC_CMB_AN_MAINANIM)->SendMessage(CB_ADDSTRING, 0, (LPARAM)strSeqName.GetBuffer());
		}
	}

	GetDlgItem(IDC_CMB_AN_MAINANIM)->EnableWindow(FALSE);

	if(m_loop == Ogre::ML_ONCE)
		GetDlgItem(IDC_RDO_ONCE)->SendMessage(BM_SETCHECK, BST_CHECKED, 0);
	else if(m_loop == Ogre::ML_ONCE_STOP)
		GetDlgItem(IDC_RDO_ONCESTOP)->SendMessage(BM_SETCHECK, BST_CHECKED, 0);
	else
		GetDlgItem(IDC_RDO_LOOP)->SendMessage(BM_SETCHECK, BST_CHECKED, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMotionPropertyDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if(GetDlgItem(IDC_RDO_ONCE)->SendMessage(BM_GETCHECK) == BST_CHECKED)
	{
		m_loop = Ogre::ML_ONCE;
	}
	else if(GetDlgItem(IDC_RDO_ONCESTOP)->SendMessage(BM_GETCHECK) == BST_CHECKED)
	{
		m_loop = Ogre::ML_ONCE_STOP;
	}
	else
	{
		m_loop = Ogre::ML_LOOP;
	}
	CDialog::OnOK();
}

void CMotionPropertyDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}