// SelectPostEffectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "SelectPostEffectDlg.h"
#include "ObjectTypeHelper.h"
#include "ColorKeysDlg.h"


// CSelectPostEffectDlg dialog

IMPLEMENT_DYNAMIC(CSelectPostEffectDlg, CDialog)

CSelectPostEffectDlg::CSelectPostEffectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectPostEffectDlg::IDD, pParent)
	, m_strPostEffect(_T(""))
	, m_fStart(0)
	, m_fEnd(0)
	, m_bNoStart(FALSE)
	, m_bNoEnd(FALSE)
{

}

CSelectPostEffectDlg::~CSelectPostEffectDlg()
{
}

void CSelectPostEffectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_CBString(pDX, IDC_COMBO_POSTTYPE, m_strPostEffect);
	DDX_Text(pDX, IDC_EDIT_POST_START, m_fStart);
	DDX_Text(pDX, IDC_EDIT_POST_END, m_fEnd);
	DDX_Check(pDX, IDC_CHK_POST_NOSTART, m_bNoStart);
	DDX_Check(pDX, IDC_CHK_POST_NOEND, m_bNoEnd);
}


BEGIN_MESSAGE_MAP(CSelectPostEffectDlg, CDialog)
	ON_BN_CLICKED(IDC_CHK_POST_NOSTART, OnBnClickedChkPostNostart)
	ON_BN_CLICKED(IDC_CHK_POST_NOEND, OnBnClickedChkPostNoend)
	ON_BN_CLICKED(IDC_BTN_POSTSETTING, OnBnClickedBtnPostsetting)
	ON_CBN_SELCHANGE(IDC_COMBO_POSTTYPE, OnCbnSelchangeCmoPostPostlist)
END_MESSAGE_MAP()


// CSelectPostEffectDlg message handlers
BOOL CSelectPostEffectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CCvtHelper cvt;
	std::vector<CString> effects;
	cvt.EnumPostEffect(effects);
	for(int i = 0; i < (int) effects.size(); i++)
		GetDlgItem(IDC_COMBO_POSTTYPE)->SendMessage(CB_ADDSTRING, 0, (LPARAM)(effects[i].GetBuffer()));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectPostEffectDlg::OnBnClickedChkPostNostart()
{
	if(BST_CHECKED == GetDlgItem(IDC_CHK_POST_NOSTART)->SendMessage(BM_GETCHECK))
	{
		GetDlgItem(IDC_EDIT_POST_START)->EnableWindow(FALSE);
	}
	else 
	{
		GetDlgItem(IDC_EDIT_POST_START)->EnableWindow(TRUE);
	}
}

void CSelectPostEffectDlg::OnBnClickedChkPostNoend()
{
	if(BST_CHECKED == GetDlgItem(IDC_CHK_POST_NOEND)->SendMessage(BM_GETCHECK))
	{
		GetDlgItem(IDC_EDIT_POST_END)->EnableWindow(FALSE);
	}
	else 
	{
		GetDlgItem(IDC_EDIT_POST_END)->EnableWindow(TRUE);
	}
}

void CSelectPostEffectDlg::OnBnClickedBtnPostsetting()
{
	CString effect;
	GetDlgItem(IDC_COMBO_POSTTYPE)->GetWindowText(effect);

	if(effect == "画面泛色")
	{
		CColorKeysDlg dlg;
		dlg.m_color = m_fsColor;
		dlg.m_colorkeys = m_fsColorKeys;
		if(IDOK == dlg.DoModal())
		{
			m_fsColor = dlg.m_color;
			m_fsColorKeys = dlg.m_colorkeys;
		}
	}/*
	else if(effect == "角色透明")
	{
		CTranspSettingDlg dlg;
		dlg.m_alphakeys = m_transpKeys;
		if(IDOK == dlg.DoModal())
		{
			m_transpKeys = dlg.m_alphakeys ;
		}
	}
	else if(effect == "自定义效果")
	{
		CCustomPostDlg dlg;
		if(IDOK == dlg.DoModal())
		{
			m_customPE = dlg.m_effectList;
		}
	}*/
}

void CSelectPostEffectDlg::OnCbnSelchangeCmoPostPostlist()
{
	CString effect;
	GetDlgItem(IDC_COMBO_POSTTYPE)->GetWindowText(effect);


	if(effect == "画面泛色" || effect == "角色透明" || effect == "自定义效果")
	{
		GetDlgItem(IDC_BTN_POSTSETTING)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_POSTSETTING)->EnableWindow(FALSE);
	}
}