// SelectSkillDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "SelectSkillDlg.h"
#include "DBManager.h"

#include "OgreResDB.h"
#include "GameDBData.h"


// CSelectSkillDlg dialog

IMPLEMENT_DYNAMIC(CSelectSkillDlg, CDialog)

CSelectSkillDlg::CSelectSkillDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectSkillDlg::IDD, pParent)
{

}

CSelectSkillDlg::~CSelectSkillDlg()
{
}

void CSelectSkillDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SELECTSKILL, m_wndList);
}


BEGIN_MESSAGE_MAP(CSelectSkillDlg, CDialog)
END_MESSAGE_MAP()


// CSelectSkillDlg message handlers
BOOL CSelectSkillDlg::OnInitDialog()
{/*
	CDialog::OnInitDialog();

	m_wndList.InsertColumn(0, "技能ID", LVCFMT_LEFT, 100);
	m_wndList.InsertColumn(1, "技能名称", LVCFMT_LEFT, 100);
	m_wndList.SetExtendedStyle(m_wndList.GetExtendedStyle() | LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	Ogre::ResTable* pTable = CDataBase::GetInstance()->GetTable("SkillInfo");
	if(pTable == NULL)
	{
		MessageBox("无法打开数据库");
	}
	else
	{
		int skillCount = pTable->FindRecord(NULL, 0, NULL);
		tagSkillInfo* pSkills = new tagSkillInfo[skillCount];
		pTable->FindRecord(pSkills, skillCount, NULL);

		for(int i = 0; i < skillCount; i++)
		{
			CString strID;
			strID.Format("%d", pSkills[i].id);
			m_wndList.InsertItem(LVIF_TEXT |LVIF_PARAM, i, strID.GetBuffer(), 0, 0, 0, (LPARAM)pSkills[i].id); 
			m_wndList.SetItemText(i, 1, pSkills[i].name);
		}
		delete pSkills;
	}*/

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectSkillDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	int iSel = m_wndList.GetSelectionMark();
	if(iSel < 0)
	{
		MessageBox("没有选择技能");
		return;
	}

	LVITEM item;
	item.iItem = iSel;
	item.mask = LVIF_PARAM;
	m_wndList.GetItem(&item);
	m_iSelId = (unsigned int)item.lParam;

	CDialog::OnOK();
}