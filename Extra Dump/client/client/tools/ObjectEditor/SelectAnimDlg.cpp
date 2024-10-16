// SelectAnimDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "SelectAnimDlg.h"
#include "ObjectTypeHelper.h"
#include "EffectManager.h"

#include "OgreModel.h"
#include "OgreEntity.h"
#include "OgreEntityData.h"


// CSelectAnimDlg dialog

IMPLEMENT_DYNAMIC(CSelectAnimDlg, CDialog)

CSelectAnimDlg::CSelectAnimDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectAnimDlg::IDD, pParent)
	, m_fStart(0)
	, m_fEnd(0)
	, m_iSel(0)
	, m_bOverwrite(FALSE)
	, m_bNoStart(FALSE)
	, m_bNoEnd(FALSE)
	, m_iPriority(0)
{

}

CSelectAnimDlg::~CSelectAnimDlg()
{
}

void CSelectAnimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMO_ANSEL_ANIMS, m_cmbAnim);
	DDX_Text(pDX, IDC_EDIT_ANSEL_START, m_fStart);
	DDX_Text(pDX, IDC_EDIT_ANSEL_END, m_fEnd);
	DDX_Check(pDX, IDC_CHK_ANSEL_OVERWRITE, m_bOverwrite);
	DDX_Check(pDX, IDC_CHK_ANSEL_NOSTART, m_bNoStart);
	DDX_Check(pDX, IDC_CHK_ANSEL_NOEND, m_bNoEnd);
	DDX_Text(pDX, IDC_EDIT_ANSEL_PRIORITY, m_iPriority);
}


BEGIN_MESSAGE_MAP(CSelectAnimDlg, CDialog)
	ON_BN_CLICKED(IDC_CHK_ANSEL_NOSTART, OnBnClickedChkAnselNostart)
	ON_BN_CLICKED(IDC_CHK_ANSEL_NOEND, OnBnClickedChkAnselNoend)
END_MESSAGE_MAP()


// CSelectAnimDlg message handlers
BOOL CSelectAnimDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CCvtHelper cvt;
	Ogre::Model *pmodel = CEffectMgr::GetInstance()->GetModelActor()->getMainModel();
	if(pmodel)
	{
		std::vector<Ogre::SequenceDesc>seqarray;
		pmodel->getModelData()->getAllSequence(seqarray);
		for(size_t i=0; i<seqarray.size(); i++)
		{
			CString strSeqName = cvt.AnimIDToDesc(seqarray[i].id);
			m_cmbAnim.AddString(strSeqName.GetBuffer());
		}
		CString sel = cvt.AnimIDToDesc(m_iSel);
		m_cmbAnim.SetWindowText(sel);
	}
	OnBnClickedChkAnselNostart();
	OnBnClickedChkAnselNoend();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectAnimDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CString sel;
	CCvtHelper cvt;
	m_cmbAnim.GetWindowText(sel);
	//sscanf(sel.GetBuffer(), "%d", &m_iSel);
	m_iSel = cvt.DescToAnimID(sel);
	CDialog::OnOK();
}

void CSelectAnimDlg::OnBnClickedChkAnselNostart()
{
	if(BST_CHECKED == GetDlgItem(IDC_CHK_ANSEL_NOSTART)->SendMessage(BM_GETCHECK))
	{
		GetDlgItem(IDC_EDIT_ANSEL_START)->EnableWindow(FALSE);
	}
	else 
	{
		GetDlgItem(IDC_EDIT_ANSEL_START)->EnableWindow(TRUE);
	}
}

void CSelectAnimDlg::OnBnClickedChkAnselNoend()
{
	if(BST_CHECKED == GetDlgItem(IDC_CHK_ANSEL_NOEND)->SendMessage(BM_GETCHECK))
	{
		GetDlgItem(IDC_EDIT_ANSEL_END)->EnableWindow(FALSE);
	}
	else 
	{
		GetDlgItem(IDC_EDIT_ANSEL_END)->EnableWindow(TRUE);
	}
}