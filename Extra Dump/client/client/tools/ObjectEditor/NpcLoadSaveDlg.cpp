// NpcLoadSaveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "NpcLoadSaveDlg.h"
#include "NPCManager.h"
#include "ObjectTypeHelper.h"

// CNpcLoadSaveDlg dialog

IMPLEMENT_DYNAMIC(CNpcLoadSaveDlg, CDialog)

CNpcLoadSaveDlg::CNpcLoadSaveDlg(CWnd* pParent /*=NULL*/, bool load)
	: CDialog(CNpcLoadSaveDlg::IDD, pParent)
	, m_NpcID(0)
	, m_NpcName(_T(""))
	, m_NpcDesc("")
	, m_NpcScale(1.0f)
	, m_NpcXparent(1.0f)
	, m_isLoad(load)
	, m_NpcHeight(200)
	, m_NpcRadius(40)
{

}

CNpcLoadSaveDlg::~CNpcLoadSaveDlg()
{
}

void CNpcLoadSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_NPCLIST, m_NpcList);
	DDX_Text(pDX, IDC_EDIT_NPCID, m_NpcID);
	DDX_Text(pDX, IDC_EDIT_NPCNAME, m_NpcName);
	DDX_Text(pDX, IDC_EDIT_NPCDESC, m_NpcDesc);
	DDX_Text(pDX, IDC_EDIT_NPCSCALE, m_NpcScale);
	DDX_Text(pDX, IDC_EDIT_NPCTRANS, m_NpcXparent);
}


BEGIN_MESSAGE_MAP(CNpcLoadSaveDlg, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_NPCLIST, &CNpcLoadSaveDlg::OnLvnItemchangedListNpclist)
END_MESSAGE_MAP()


// CNpcLoadSaveDlg message handlers

BOOL CNpcLoadSaveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_NpcList.SetExtendedStyle(m_NpcList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_NpcList.InsertColumn(0, _T("NPC ID"), LVCFMT_LEFT, 50);
	m_NpcList.InsertColumn(1, _T("NPC 名称"), LVCFMT_LEFT, 120);
	m_NpcList.InsertColumn(2, "放缩", LVCFMT_LEFT, 40);
	m_NpcList.InsertColumn(3, "透明", LVCFMT_LEFT, 40);
	m_NpcList.InsertColumn(4, _T("模型地址"), LVCFMT_LEFT, 260);

	loadNpcList();

	if(m_isLoad) SetWindowText("装载NPC");
	else SetWindowText("保存NPC");

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CNpcLoadSaveDlg::loadNpcList()
{
	size_t count = CNPCOperator::GetNpcCount();
	m_NpcList.DeleteAllItems();
	CCvtHelper cvt;
	NPCDISPLAYINFO* pNpcs = new NPCDISPLAYINFO[count];
	CNPCOperator::GetAllNpc(pNpcs, count);

	for(size_t i = 0; i < count; i++)
	{
		NPCDISPLAYINFO& npc = pNpcs[i];
		int index = m_NpcList.InsertItem(LVIF_TEXT|LVIF_PARAM, i, cvt.IntToString((int)npc.id), 0, 0, 0, npc.id);
		m_NpcList.SetItem(index, 1, LVIF_TEXT, npc.name, 0, 0, NULL, 0);
		m_NpcList.SetItem(index, 2, LVIF_TEXT, cvt.FloatToString(npc.scale), 0, 0, NULL, 0);
		m_NpcList.SetItem(index, 3, LVIF_TEXT, cvt.FloatToString(npc.alpha), 0, 0, NULL, 0);
		m_NpcList.SetItem(index, 4, LVIF_TEXT, npc.model, 0, 0, NULL, 0);
	}
}

void CNpcLoadSaveDlg::OnLvnItemchangedListNpclist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	GetDlgItem(IDC_EDIT_NPCID)->SetWindowText(m_NpcList.GetItemText(pNMLV->iItem, 0).GetBuffer());
	GetDlgItem(IDC_EDIT_NPCNAME)->SetWindowText(m_NpcList.GetItemText(pNMLV->iItem, 1).GetBuffer());
	GetDlgItem(IDC_EDIT_NPCSCALE)->SetWindowText(m_NpcList.GetItemText(pNMLV->iItem, 2).GetBuffer());
	GetDlgItem(IDC_EDIT_NPCTRANS)->SetWindowText(m_NpcList.GetItemText(pNMLV->iItem, 3).GetBuffer());

	UpdateData();

	*pResult = 0;
}
