// ResBrowerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "ResBrowerDlg.h"
#include "TerrainEdit.h"

using namespace TE;
// CResBrowerDlg dialog

std::string CResBrowerDlg::m_SelectName; 

IMPLEMENT_DYNAMIC(CResBrowerDlg, CDialog)

CResBrowerDlg::CResBrowerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResBrowerDlg::IDD, pParent)
{
	m_bInited = false;
}

CResBrowerDlg::~CResBrowerDlg()
{
}

void CResBrowerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RES_PREVIEW, m_ResPreview);
	DDX_Control(pDX, IDC_RES_TREE, m_ResTree);
	DDX_Control(pDX, IDC_CHECK_DRAW_RADARMAP, m_DrawRadarMap);
	DDX_Control(pDX, IDC_BUTTON_PREVIEW_BGCOLOR, m_PreviewBGColor);
}

BEGIN_MESSAGE_MAP(CResBrowerDlg, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_RES_TREE, &CResBrowerDlg::OnTvnSelchangedResTree)
	ON_BN_CLICKED(IDC_CHECK_DRAW_RADARMAP, &CResBrowerDlg::OnBnClickedCheckDrawRadarmap)
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW_BGCOLOR, &CResBrowerDlg::OnBnClickedButtonPreviewBgcolor)
	ON_BN_CLICKED(IDC_RADIO_RES_ALL, &CResBrowerDlg::OnBnClickedRadioResAll)
	ON_BN_CLICKED(IDC_RADIO_RES_MODEL, &CResBrowerDlg::OnBnClickedRadioResModel)
	ON_BN_CLICKED(IDC_RADIO_RES_TILETEX, &CResBrowerDlg::OnBnClickedRadioResTiletex)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CResBrowerDlg message handlers

BOOL CResBrowerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CheckDlgButton(IDC_RADIO_RES_ALL, 1);
	InitFileTree(".");
	m_PreviewBGColor.SetColor(RGB(0,255,0));

	m_bInited = true;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CResBrowerDlg::InitFileTree(CString rootpath)
{
	m_ResTree.DeleteAllItems();
	AddFile(rootpath, NULL);

	if(rootpath == ".")	m_RootPath = "";
	else m_RootPath = rootpath + "\\";
}

void CResBrowerDlg::AddFile(CString StrPath, HTREEITEM faItem)
{
	CFileFind myfind;
	CString filename, dirname;
	BOOL donext; 
	HTREEITEM newitem;	

	dirname = StrPath+"\\*.*";
	donext = myfind.FindFile(dirname);
	while(donext)
	{  		
		donext = myfind.FindNextFile();
		if( myfind.GetFilePath().Find(".svn") != -1 )
		{
			continue;
		}
		if(myfind.IsDirectory() && !myfind.IsDots())
		{	
			dirname = myfind.GetFilePath();
			filename = myfind.GetFileTitle();

			newitem = m_ResTree.InsertItem(filename, faItem);

			AddFile(dirname, newitem);
		}

		if(!myfind.IsDirectory() && !myfind.IsDots())
		{
			filename = myfind.GetFileName();
			filename.MakeLower();

			char *pextens[] = {".omod", ".ent", ".otex", ".dds", ".jpg", ".tga", ".bmp", ".wav", ".mp3", ".wma"};

			for(size_t i=0; i<ARRAY_ELEMENTS(pextens); i++)
			{
				int index = filename.Find(pextens[i]);
				if(index != -1)
				{
					newitem = m_ResTree.InsertItem(filename, faItem);
					break;
				}
			}
		}
	}
	myfind.Close();
}

void CResBrowerDlg::OnTvnSelchangedResTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	UpdateData(true);

	unsigned long index;
	HTREEITEM selectItem = m_ResTree.GetSelectedItem();

	index = m_ResTree.GetItemData(selectItem);

	CString  node_name;
	node_name = m_ResTree.GetItemText(pNMTreeView->itemNew.hItem);

	int dotIndex = node_name.Find (".");

	if(dotIndex == -1)
	{
		*pResult = 0;
		return;
	}

	index = m_ResTree.GetIndent();
	CFileFind myfind;

	HTREEITEM parent_item = m_ResTree.GetParentItem(pNMTreeView->itemNew.hItem);

	CString parent_name;
	while(parent_item!=NULL)
	{
		parent_name = m_ResTree.GetItemText(parent_item);
		parent_item = m_ResTree.GetParentItem(parent_item);

		node_name = parent_name + "\\" + node_name;
	}

	node_name = m_RootPath + node_name;

	m_SelectName = std::string(node_name.GetBuffer());

	g_TerrainEdit.setCurSelectRes(node_name.GetBuffer());

	index = m_ResTree.GetItemData(pNMTreeView->itemNew.hItem);
	index = pNMTreeView->itemNew.lParam;

	*pResult = 0;
}

void CResBrowerDlg::OnBnClickedCheckDrawRadarmap()
{
	// TODO: Add your control notification handler code here
	g_TerrainEdit.m_PreviewOrRadar = m_DrawRadarMap.GetCheck()?0:1;
}

void CResBrowerDlg::OnBnClickedButtonPreviewBgcolor()
{
	// TODO: Add your control notification handler code here
	COLORREF c = m_PreviewBGColor.GetColor();
	unsigned int r = GetRValue(c);
	unsigned int g = GetGValue(c);
	unsigned int b = GetBValue(c);

	g_TerrainEdit.m_pResPreviewRU->setClearColor(RGB(b,g,r)|0xff000000);
}

void CResBrowerDlg::OnBnClickedRadioResAll()
{
	// TODO: Add your control notification handler code here
	InitFileTree(".");
}

void CResBrowerDlg::OnBnClickedRadioResModel()
{
	// TODO: Add your control notification handler code here
	InitFileTree("scene\\model");
}

void CResBrowerDlg::OnBnClickedRadioResTiletex()
{
	// TODO: Add your control notification handler code here
	InitFileTree("scene\\tileset");
}

void CResBrowerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	RECT rectDlg;
	GetClientRect(&rectDlg);

	float height = (rectDlg.bottom - rectDlg.top)-230;

	if (m_bInited)
	{
		RECT rectWindow; 
		m_ResTree.GetClientRect(&rectWindow);
		m_ResTree.MoveWindow(rectWindow.left,230,cx,height);
	}
}
