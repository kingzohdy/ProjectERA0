// AreaEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "AreaEditDlg.h"
#include "TerrainEdit.h"
#include "EnvRegion.h"
#include "EnvironmentEditDlg.h"
#include "ProcEditDlg.h"
#include "Terrain.h"
#include "EditorCamera.h"

namespace TE
{
	extern std::string g_path;
}

using namespace TE;

// CAreaEditDlg dialog

IMPLEMENT_DYNAMIC(CAreaEditDlg, CDialog)

CAreaEditDlg::CAreaEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAreaEditDlg::IDD, pParent)
{

}

CAreaEditDlg::~CAreaEditDlg()
{
}

void CAreaEditDlg::UpdateAreaData()
{
	if( !g_TerrainEdit.GetUpdateAreaData () )
		return;

	m_AreaList.DeleteAllItems ();

	for(unsigned int i=0 ; i<g_AreaSet.GetAreaSize() ; i++)
	{
		CString str;		
		MapArea *parea = g_AreaSet.GetArea(i);

		str.Format("%d", parea->GetID());

		m_AreaList.InsertItem (i, str);		
		m_AreaList.SetItemData (i, i);

		if(parea->GetType()==0) m_AreaList.SetItemText(i, 1, "环境");
		else if(parea->GetType()==1) m_AreaList.SetItemText(i, 1, "逻辑");
		else m_AreaList.SetItemText(i, 1, "错误");

		m_AreaList.SetItemText(i, 2, parea->GetName() .c_str ()  );	
		m_AreaList.SetItemText(i, 3, parea->GetLua() .c_str () );
	}

	UpdateData(false);

	SelectArea(g_AreaSet.GetCurSelectID());

	g_TerrainEdit.SetUpdateAreaData(false);
};

void CAreaEditDlg::SelectArea(int i)
{
	//change color
	g_AreaSet.SetCurSelectID(i);
	m_pEnvEditDlg->SetDataToUI();
	m_pProcEditDlg->SetDataToUI();

	g_AreaSet.ClearColor(RED_COLOR);
	MapArea *parea = g_AreaSet.GetArea(i);
	if(i>0 && parea->GetPath()->GetPosNum()>0)
	{
		parea->SetColor(GREEN_COLOR);
		Ogre::Vector3 center = parea->GetAABBox().getCenter();
		g_Camera.setTarget(center.x, center.y, center.z);
	}
}

void CAreaEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_DISPLAYAREA, m_DisplayAreaCheck);
	DDX_Control(pDX, IDC_LIST_AREA, m_AreaList);
}

void CAreaEditDlg::addArea(int type)
{
	if(g_AreaSet.GetAreaSize() == 0)
		return;

	CString id;

	int nID =  g_AreaSet.newAreaID();
	unsigned int index = m_AreaList.GetItemCount();
	id.Format ( "%d" , nID);

	m_AreaList.InsertItem(index, id);	
	m_AreaList.SetItemData(index, index);

	if(type==0) m_AreaList.SetItemText(index, 1, "环境");
	else if(type==1) m_AreaList.SetItemText(index, 1, "逻辑");
	else m_AreaList.SetItemText(index, 1, "错误");

	CString name;
	name.Format("区域_%d", nID);
	m_AreaList.SetItemText(index, 2, name);
	m_AreaList.SetItemText(index, 3, "");

	g_TerrainEdit.SetCurEditEvent(TE::AREA_EIDT_EVEN);
	g_TerrainEdit.AddArea(nID, type, name.GetBuffer(), "");
}


BEGIN_MESSAGE_MAP(CAreaEditDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADDAREA, &CAreaEditDlg::OnBnClickedButtonAddarea)
	ON_BN_CLICKED(IDC_BUTTON_DELAREA, &CAreaEditDlg::OnBnClickedButtonDelarea)
	ON_BN_CLICKED(IDC_BUTTON_LOADAREA, &CAreaEditDlg::OnBnClickedButtonLoadarea)
	ON_BN_CLICKED(IDC_BUTTON_SAVEAREA, &CAreaEditDlg::OnBnClickedButtonSavearea)
	ON_BN_CLICKED(IDC_BUTTON_SELECTAREA, &CAreaEditDlg::OnBnClickedButtonSelectarea)
	ON_BN_CLICKED(IDC_CHECK_DISPLAYAREA, &CAreaEditDlg::OnBnClickedCheckDisplayarea)
	ON_NOTIFY(NM_CLICK, IDC_LIST_AREA, &CAreaEditDlg::OnNMClickListArea)
	ON_BN_CLICKED(IDC_BUTTON_ADDAREA_PROC, &CAreaEditDlg::OnBnClickedButtonAddareaProc)
	ON_BN_CLICKED(IDC_BUTTON_SAVEPROC_TODB, &CAreaEditDlg::OnBnClickedButtonSaveprocTodb)
	ON_BN_CLICKED(IDC_BUTTON_COPYAREA, &CAreaEditDlg::OnBnClickedButtonCopyarea)
	ON_BN_CLICKED(IDC_BUTTON_PASTEAREA, &CAreaEditDlg::OnBnClickedButtonPastearea)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_AREA, &CAreaEditDlg::OnLvnItemchangedListArea)
	ON_NOTIFY(HDN_ITEMCHANGED, 0, &CAreaEditDlg::OnHdnItemchangedListArea)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_AREA, &CAreaEditDlg::OnLvnEndlabeleditListArea)
	ON_BN_CLICKED(IDC_BUTTON_LOADPROC_FROMDB, &CAreaEditDlg::OnBnClickedButtonLoadprocFromdb)
END_MESSAGE_MAP()


// CAreaEditDlg message handlers

void CAreaEditDlg::OnBnClickedButtonAddarea()
{
	// TODO: Add your control notification handler code here
	addArea(0);
}

void CAreaEditDlg::OnBnClickedButtonDelarea()
{
	// TODO: Add your control notification handler code here
	g_AreaSet.DelCurArea();
	g_TerrainEdit.SetUpdateAreaData(true);
}

void CAreaEditDlg::OnBnClickedButtonLoadarea()
{
	// TODO: Add your control notification handler code here
	if(g_AreaSet.GetAreaSize() == 0)
		return;

	char strAreaName[MAX_PATH] ;
	char strAreaNamePath[MAX_PATH] ; 
	strAreaName[0] = 0;
	strAreaNamePath[0]=0;

	memset( strAreaNamePath , 0 , MAX_PATH);

	OPENFILENAME ofn = { sizeof(OPENFILENAME), NULL, NULL,
		_T("Area Files (.area)\0*.area\0\0"), 
		NULL, 0, 1, strAreaNamePath, MAX_PATH, strAreaName, MAX_PATH, 
		NULL, _T("Open Area File"), 
		OFN_FILEMUSTEXIST, 0, 1, NULL, 0, NULL, NULL };

	if( TRUE == GetOpenFileName( &ofn ) )
	{
		SetCurrentDirectory(g_path.c_str ());
		g_TerrainEdit.LoadAreaFile ( strAreaNamePath );
		return ;
	}     
}

void CAreaEditDlg::OnBnClickedButtonSavearea()
{
	// TODO: Add your control notification handler code here
	if(g_AreaSet.GetAreaSize() == 0)
		return;

//!!!	g_MapInfoManager.SaveMapTrap();

	g_TerrainEdit.SaveAreaFile (NULL);  /// DON'T Care the filename
}

void CAreaEditDlg::OnBnClickedButtonSelectarea()
{
	// TODO: Add your control notification handler code here
	if(g_AreaSet.GetAreaSize() == 0)
		return;

	g_TerrainEdit.SetCurEditEvent(TE::SELECT_AREA_EVEN);
}

void CAreaEditDlg::OnBnClickedCheckDisplayarea()
{
	// TODO: Add your control notification handler code here
	g_AreaSet.SetShowArea(m_DisplayAreaCheck.GetCheck()>0);
}

void CAreaEditDlg::OnNMClickListArea(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int selectItem = m_AreaList.GetNextItem(-1, LVNI_SELECTED);
	if(selectItem == -1) return;
	if(g_AreaSet.GetCurSelectID() == selectItem) return;

	SelectArea(selectItem);
}

BOOL CAreaEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	unsigned int nRows = 10;
	m_AreaList.InsertColumn(0, _T("#"), LVCFMT_LEFT, nRows );
	m_AreaList.InsertColumn( 0 , "ID" , LVCFMT_LEFT , 32);
	m_AreaList.InsertColumn(1, "类型", LVCFMT_LEFT, 32);
	m_AreaList.InsertColumn (2 , "名称" , LVCFMT_LEFT , 60 );
	m_AreaList.InsertColumn (3 , "脚本" , LVCFMT_LEFT , 120 );

	m_AreaList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_AreaList.EnableMarkSortedColumn();

	m_DisplayAreaCheck.SetCheck(1);
	/// 添加一个缺省的全局区域，表示整个场景的环境
	m_AreaList.InsertItem( 0 ,"0");
	m_AreaList.SetItemData(0 , 0);
	m_AreaList.SetItemText(0, 1, "环境");
	m_AreaList.SetItemText(0, 2, "全局区域");
	m_AreaList.SetItemText(0 , 3, "");

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAreaEditDlg::OnBnClickedButtonAddareaProc()
{
	// TODO: Add your control notification handler code here
	addArea(1);
}

void CAreaEditDlg::OnBnClickedButtonSaveprocTodb()
{
	// TODO: Add your control notification handler code here
	int mapid = g_TerrainEdit.getMapInfo()->mapid;
	if(mapid <= 0)
	{
		AfxMessageBox("目前的地图ID不合法， 保存失败", MB_OK);
		return;
	}
	g_AreaSet.saveProcAreaData(mapid);

	AfxMessageBox("逻辑区域成功导出到数据库", MB_OK);
}

void CAreaEditDlg::OnBnClickedButtonCopyarea()
{
	// TODO: Add your control notification handler code here
	g_AreaSet.copyFromCurArea();	
}

void CAreaEditDlg::OnBnClickedButtonPastearea()
{
	// TODO: Add your control notification handler code here
	g_AreaSet.pasteToCurArea();

	m_pEnvEditDlg->SetDataToUI();
	m_pProcEditDlg->SetDataToUI();
}

void CAreaEditDlg::OnLvnItemchangedListArea(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	/*
	MapArea *parea = g_AreaSet.GetArea(pNMLV->iItem);

	char buffer[256];
	if(pNMLV->iSubItem == 2)
	{
		m_AreaList.GetItemText(pNMLV->iItem, pNMLV->iSubItem, buffer, 256);
		parea->SetName(buffer);
	}
	else if(pNMLV->iSubItem == 3)
	{
		m_AreaList.GetItemText(pNMLV->iItem, pNMLV->iSubItem, buffer, 256);
		parea->SetLuaName(buffer);
	}*/
}

void CAreaEditDlg::OnHdnItemchangedListArea(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CAreaEditDlg::OnLvnEndlabeleditListArea(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	/*

	MapArea *parea = g_AreaSet.GetArea(pDispInfo->item.iItem);

	char buffer[256];
	if(pDispInfo->item.iSubItem == 2)
	{
		parea->SetName(pDispInfo->item.pszText);
	}
	else if(pDispInfo->item.iSubItem == 3)
	{
		parea->SetLuaName(pDispInfo->item.pszText);
	}*/
}

void CAreaEditDlg::OnBnClickedButtonLoadprocFromdb()
{
	// TODO: Add your control notification handler code here

	int mapid = g_TerrainEdit.getMapInfo()->mapid;
	if(mapid <= 0)
	{
		AfxMessageBox("目前的地图ID不合法， 装载失败", MB_OK);
		return;
	}

	if(AfxMessageBox("将会读取数据库中逻辑区域信息， 是否继续？", MB_OKCANCEL) == IDCANCEL)
	{
		return;
	}

	g_AreaSet.loadProcAreaData(mapid);

	g_TerrainEdit.SetUpdateAreaData(true);

	UpdateAreaData();

	AfxMessageBox("从数据库导入逻辑区域成功!", MB_OK);
}
