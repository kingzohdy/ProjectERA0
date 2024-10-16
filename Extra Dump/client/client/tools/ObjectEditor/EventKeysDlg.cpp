// EventKeysDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "EventKeysDlg.h"


// CEventKeysDlg dialog

IMPLEMENT_DYNAMIC(CEventKeysDlg, CDialog)

CEventKeysDlg::CEventKeysDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEventKeysDlg::IDD, pParent)
{

}

CEventKeysDlg::~CEventKeysDlg()
{
}

void CEventKeysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EVENTKEYS, m_wndList);
	DDX_Control(pDX, IDC_EDIT_EVENTKEY, m_editNew);
}


BEGIN_MESSAGE_MAP(CEventKeysDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADDEVENT, OnBnClickedBtnAddevent)
	ON_BN_CLICKED(IDC_BTN_DELEVENT, OnBnClickedBtnDelevent)
END_MESSAGE_MAP()


// CEventKeysDlg message handlers
BOOL CEventKeysDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_wndList.InsertColumn(0, "ÊÂ¼þÃû³Æ",  LVCFMT_LEFT, 200, -1);

	RefreshList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEventKeysDlg::OnBnClickedBtnAddevent()
{
	CString strEvent;
	m_editNew.GetWindowText(strEvent);
	bool bExist = false;
	for(int i = 0; i < (int) m_EventNames.size(); i++)
	{
		if(m_EventNames[i] == strEvent)
		{
			bExist = true;
			break;
		}
	}

	if(bExist == false)
	{
		m_EventNames.push_back(strEvent);
		RefreshList();
	}
}

void CEventKeysDlg::OnBnClickedBtnDelevent()
{
	int iSel = m_wndList.GetSelectionMark();
	if(iSel >= 0 && iSel < (int)m_EventNames.size())
	{
		m_EventNames.erase(m_EventNames.begin() + iSel);
		RefreshList();
	}
}


void CEventKeysDlg::RefreshList()
{
	m_wndList.DeleteAllItems();
	for(int i = 0; i < (int) m_EventNames.size(); i++)
	{
		m_wndList.InsertItem(LVIF_TEXT, i, m_EventNames[i].GetBuffer(), 0, 0, 0, 0);
	}
}