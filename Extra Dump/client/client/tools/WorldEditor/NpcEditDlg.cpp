// NpcEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "NpcEditDlg.h"
#include "EditEvent_ObjectBrush.h"
#include "EditEventHandlerMgr.h"
#include "TerrainEdit.h"
#include "MapNpc.h"

#include "GameDBData.h"

using namespace TE;

// CNpcEditDlg dialog

IMPLEMENT_DYNAMIC(CNpcEditDlg, CBCGPDialog)

CNpcEditDlg::CNpcEditDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CNpcEditDlg::IDD, pParent)
{
	m_IDFilter[0] = 0;
	m_NameFilter[0] = 0;
}

CNpcEditDlg::~CNpcEditDlg()
{
}

void CNpcEditDlg::OnSelectNPC(int sel)
{
	m_NpcInstList.SetSelectionMark(sel);
}

void CNpcEditDlg::OnDeleteNPC(int sel)
{
	m_NpcInstList.DeleteItem(sel);
}

void CNpcEditDlg::UpdateNpcData()
{
	m_NpcInstList.DeleteAllItems();

	for(unsigned int i = 0 ; i < g_TerrainEdit.getNpcManager()->m_NpcSet.size(); i++)
	{
		CString str;		
		MapNpc*  pNpc = g_TerrainEdit.getNpcManager ()->m_NpcSet[i] ;

		str.Format ("%d(%d)", pNpc->m_nNPCID, i);

		m_NpcInstList.InsertItem (i, str);		
		m_NpcInstList.SetItemData (i, i);

		m_NpcInstList.SetItemText ( i , 1 , pNpc->m_strNPCName  );	
		str.Format ("%d,%d,%d", int(pNpc->GetPos().x), int(pNpc->GetPos().y), int(pNpc->GetPos().z));

		m_NpcInstList.SetItemText(i , 2 ,str);

		str.Format("%d", int(pNpc->GetDir().x));
		m_NpcInstList.SetItemText ( i , 3 ,str );
	}

	UpdateData( false );
};

void CNpcEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_NPC_RES, m_NpcResList);
	DDX_Control(pDX, IDC_LIST_NPC_INST, m_NpcInstList);
	DDX_Control(pDX, IDC_CHECK_DISPLAY_NPC, m_DispNpcCheck);
	DDX_Control(pDX, IDC_CHECK_EDIT_NPC, m_EditNpcCheck);
}


BEGIN_MESSAGE_MAP(CNpcEditDlg, CBCGPDialog)
	ON_WM_SETFOCUS()
	ON_NOTIFY(NM_CLICK, IDC_LIST_NPC_RES, &CNpcEditDlg::OnNMClickListNpcRes)
	ON_NOTIFY(NM_CLICK, IDC_LIST_NPC_INST, &CNpcEditDlg::OnNMClickListNpcInst)
	ON_BN_CLICKED(IDC_CHECK_DISPLAY_NPC, &CNpcEditDlg::OnBnClickedCheckDisplayNpc)
	ON_BN_CLICKED(IDC_CHECK_EDIT_NPC, &CNpcEditDlg::OnBnClickedCheckEditNpc)
	ON_BN_CLICKED(IDC_BUTTON_DEL_NPC, &CNpcEditDlg::OnBnClickedButtonDelNpc)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_NPC_RES, &CNpcEditDlg::OnLvnItemchangedListNpcRes)
	ON_BN_CLICKED(IDC_BUTTON_DEL_ALLNPC, &CNpcEditDlg::OnBnClickedButtonDelAllnpc)
	ON_BN_CLICKED(IDC_BUTTON_SAVENPC_TODB, &CNpcEditDlg::OnBnClickedButtonSavenpcTodb)
	ON_BN_CLICKED(IDC_BUTTON_COPYJINGYING_TODB, &CNpcEditDlg::OnBnClickedButtonCopyjingyingTodb)
	ON_BN_CLICKED(IDC_BUTTON_LOADNPC_FROMDB, &CNpcEditDlg::OnBnClickedButtonLoadnpcFromdb)
	ON_EN_CHANGE(IDC_EDIT_NPCID_FILTER, &CNpcEditDlg::OnEnChangeEditNpcidFilter)
	ON_EN_CHANGE(IDC_EDIT_NPCNAME_FILTER, &CNpcEditDlg::OnEnChangeEditNpcnameFilter)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_NPC_INST, &CNpcEditDlg::OnLvnItemchangedListNpcInst)
END_MESSAGE_MAP()


// CNpcEditDlg message handlers

void CNpcEditDlg::OnSetFocus(CWnd* pOldWnd)
{
	CBCGPDialog::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
	g_TerrainEdit.SetCurEditEvent(TE::EDITEVENT_OBJECTBRUSH);
	ObjectBrushEditHandler *phandler = static_cast<ObjectBrushEditHandler *>(g_EventProcessFactory.GetEventProcess(EDITEVENT_OBJECTBRUSH));
	phandler->setEditObjectType(TE::EDITOBJ_NPC);
	phandler->setEditOP(TE::OBJEDIT_OP_ADD);

	g_TerrainEdit.SetShowObjectBox(true);
}

void CNpcEditDlg::InitNpcResData()
{
	m_NpcResList.DeleteAllItems();

	std::vector<NPCDISPLAYINFO>npcinfo;
	DBDataManager::getSingleton().loadNpcDisplayInfo(npcinfo);
	int nrecord = (int)npcinfo.size();

	int count = 0;
	for(int i=0; i<nrecord; i++)
	{
		const NPCDISPLAYINFO &info = npcinfo[i];
		CString str;
		str.Format("%d", info.id);
		char *pstrbuf = str.GetBuffer();

		if(m_IDFilter[0] && strstr(pstrbuf, m_IDFilter)!=pstrbuf) continue;
		if(m_NameFilter[0] && strstr(info.name, m_NameFilter)==NULL) continue;

		m_NpcResList.InsertItem(count, str);
		m_NpcResList.SetItemData(count, i);

		m_NpcResList.SetItemText(count, 1, info.name);
		m_NpcResList.SetItemText(count, 2, info.model);

		count++;
	}

	m_NpcResList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	UpdateData(false);
}

BOOL CNpcEditDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	//TODO: Add extra initialization here

	m_NpcResList.InsertColumn (0, _T("#"), LVCFMT_LEFT, 100);
	m_NpcResList.InsertColumn ( 0 , "NPC_ID" , LVCFMT_LEFT , 70 );
	m_NpcResList.InsertColumn ( 1 , "NPC_NAME" , LVCFMT_LEFT , 70 );
	m_NpcResList.InsertColumn ( 2 , "NPC_MODEL_PATH" , LVCFMT_LEFT , 170 );

	m_NpcResList.EnableMarkSortedColumn();

	m_NpcInstList.InsertColumn (0, _T("#"), LVCFMT_LEFT, 100);
	m_NpcInstList.InsertColumn ( 0 , "NPC_ID" , LVCFMT_LEFT , 70 );
	m_NpcInstList.InsertColumn ( 1 , "NPC_NAME" , LVCFMT_LEFT , 70 );

	m_NpcInstList.InsertColumn ( 2 , "NPC_POS" , LVCFMT_LEFT , 120 );
	m_NpcInstList.InsertColumn ( 3 , "NPC_DIR" , LVCFMT_LEFT , 120 );
	m_NpcInstList.InsertColumn ( 4 , "NPC_SCRIPT" , LVCFMT_LEFT , 170 );

	m_NpcInstList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_NpcInstList.EnableMarkSortedColumn ();

	g_TerrainEdit.SetSelectNpcID ( -1 );

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CNpcEditDlg::OnNMClickListNpcRes(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int selectItem = m_NpcResList.GetNextItem   ( -1,   LVNI_SELECTED ) ;

	if( selectItem != -1 )
	{
		CString str;
		str = m_NpcResList.GetItemText(selectItem, 0);
		int id = atoi(str.GetBuffer());

		g_TerrainEdit.SetSelectNpcID(id);
		g_TerrainEdit.ShowNpc(id);

		g_TerrainEdit.setSelectModel(NULL);
	}
}

void CNpcEditDlg::OnNMClickListNpcInst(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int sel = m_NpcInstList.GetNextItem(-1, LVNI_SELECTED);
	if(sel >= 0)
	{
		g_TerrainEdit.selectMapNPC(sel);
		g_TerrainEdit.cameraOnSelectModel();
	}
}

void CNpcEditDlg::OnBnClickedCheckDisplayNpc()
{
	// TODO: Add your control notification handler code here
}

void CNpcEditDlg::OnBnClickedCheckEditNpc()
{
	// TODO: Add your control notification handler code here
}

void CNpcEditDlg::OnBnClickedButtonDelNpc()
{
	// TODO: Add your control notification handler code here
	g_TerrainEdit.DelCurObject();
}

void CNpcEditDlg::OnLvnItemchangedListNpcRes(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CNpcEditDlg::OnBnClickedButtonDelAllnpc()
{
	// TODO: Add your control notification handler code here
	g_TerrainEdit.getNpcManager()->deleteAllNpc();
	g_TerrainEdit.clearSelect();

	UpdateNpcData();
}

void CNpcEditDlg::OnBnClickedButtonSavenpcTodb()
{
	// TODO: Add your control notification handler code here
	int mapid = g_TerrainEdit.getMapInfo()->mapid;
	if(mapid <= 0)
	{
		AfxMessageBox("目前的地图ID不合法， 保存失败", MB_OK);
		return;
	}

	if(AfxMessageBox("将会用当前地图npc数据覆盖保存到数据库， 是否继续？", MB_OKCANCEL) == IDCANCEL)
	{
		return;
	}

	if(g_TerrainEdit.getNpcManager()->SaveDB(mapid, false))
	{
		AfxMessageBox("NPC成功导出到数据库", MB_OK);
	}
}

void CNpcEditDlg::OnBnClickedButtonLoadnpcFromdb()
{
	// TODO: Add your control notification handler code here
	int mapid = g_TerrainEdit.getMapInfo()->mapid;
	if(mapid <= 0)
	{
		AfxMessageBox("目前的地图ID不合法， 装载失败", MB_OK);
		return;
	}

	if(AfxMessageBox("将会读取数据库中npc数据并丢弃当前数据， 是否继续？", MB_OKCANCEL) == IDCANCEL)
	{
		return;
	}

	if(g_TerrainEdit.getNpcManager()->LoadDB(mapid))
	{
		AfxMessageBox("成功读取数据库NPC数据");
	}

	UpdateNpcData();
}

void CNpcEditDlg::OnBnClickedButtonCopyjingyingTodb()
{
	// TODO: Add your control notification handler code here
	int mapid = g_TerrainEdit.getMapInfo()->mapid;
	if(mapid <= 0)
	{
		AfxMessageBox("目前的地图ID不合法， 保存失败", MB_OK);
		return;
	}
	if(g_TerrainEdit.getNpcManager()->SaveDB(mapid, true))
	{
		AfxMessageBox("NPC成功导出到数据库", MB_OK);
	}
}

void CNpcEditDlg::OnEnChangeEditNpcidFilter()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBCGPDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	GetDlgItemText(IDC_EDIT_NPCID_FILTER, m_IDFilter, sizeof(m_IDFilter));

	InitNpcResData();
}

void CNpcEditDlg::OnEnChangeEditNpcnameFilter()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBCGPDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	GetDlgItemText(IDC_EDIT_NPCNAME_FILTER, m_NameFilter, sizeof(m_NameFilter));

	InitNpcResData();
}

void CNpcEditDlg::OnLvnItemchangedListNpcInst(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
