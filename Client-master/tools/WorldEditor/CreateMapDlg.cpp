// CreateMapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "CreateMapDlg.h"
#include "ControlsHelper.h"
#include "TerrainEdit.h"

extern std::string g_path;
using namespace TE;
// CCreateMapDlg dialog

IMPLEMENT_DYNAMIC(CCreateMapDlg, CDialog)

CCreateMapDlg::CCreateMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateMapDlg::IDD, pParent)
	, m_MapName(_T(""))
	, m_MapSizeX(0)
	, m_MapSizeZ(0)
	, m_MapID(0)
	, m_GridSizeX(0)
	, m_GridSizeZ(0)
	, m_MapCoordX(0)
	, m_MapCoordZ(0)
	, m_MapHeight(0)
	, m_MapMaxHeight(0)
	, m_MapTexRepeat(0)
	, m_DefaultTexture(_T(""))
{

}

CCreateMapDlg::~CCreateMapDlg()
{
}

void CCreateMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MAPNAME, m_MapName);
	DDX_Text(pDX, IDC_EDIT_MAP_SIZEX, m_MapSizeX);
	DDV_MinMaxInt(pDX, m_MapSizeX, 1, 128);
	DDX_Text(pDX, IDC_EDIT_MAP_SIZEZ, m_MapSizeZ);
	DDV_MinMaxInt(pDX, m_MapSizeZ, 1, 128);
	DDX_Text(pDX, IDC_EDIT_MAP_ID, m_MapID);
	DDV_MinMaxInt(pDX, m_MapID, 1, 100000000);
	DDX_Text(pDX, IDC_EDIT_MAP_GRIDSIZEX, m_GridSizeX);
	DDV_MinMaxInt(pDX, m_GridSizeX, 1, 32);
	DDX_Text(pDX, IDC_EDIT_MAP_GRIDSIZEZ, m_GridSizeZ);
	DDV_MinMaxInt(pDX, m_GridSizeZ, 1, 32);
	DDX_Text(pDX, IDC_EDIT_MAP_COORDX, m_MapCoordX);
	DDX_Text(pDX, IDC_EDIT_MAP_COORDZ, m_MapCoordZ);
	DDX_Text(pDX, IDC_EDIT_MAP_HEIGHT, m_MapHeight);
	DDX_Text(pDX, IDC_EDIT_MAP_MAXHEIGHT, m_MapMaxHeight);
	DDX_Text(pDX, IDC_EDIT_MAP_TEXREPEAT, m_MapTexRepeat);
	DDV_MinMaxInt(pDX, m_MapTexRepeat, 1, 128);
	DDX_Text(pDX, IDC_EDIT_DEFAULT_TEXTURE, m_DefaultTexture);
}


BEGIN_MESSAGE_MAP(CCreateMapDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT_TEXTURE, &CCreateMapDlg::OnBnClickedButtonDefaultTexture)
	ON_BN_CLICKED(IDOK, &CCreateMapDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCreateMapDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CCreateMapDlg message handlers

void CCreateMapDlg::OnBnClickedButtonDefaultTexture()
{
	/*
	// TODO: Add your control notification handler code here
	char initdir[MAX_PATH];
	char filetitle[MAX_PATH];
	char filepath[MAX_PATH];

	GetCurrentDirectory(MAX_PATH, initdir);
	filetitle[0] = filepath[0] = 0;

	// Display the OpenFileName dialog. Then, try to load the specified file
	OPENFILENAME ofn = { sizeof(OPENFILENAME), NULL, NULL,
		"Texture Files (.otex;.dds)\0*.otex;*.dds;\0Image Files (.dds;)\0*.dds;\0\0", 
		NULL, 0, 1, filepath, MAX_PATH, filetitle, MAX_PATH, 
		initdir, "Open Texture File",
		OFN_PATHMUSTEXIST, 0, 1, NULL, 0, NULL, NULL };

	if(TRUE == GetOpenFileName(&ofn))
	{
		SetCurrentDirectory(g_path.c_str());

		g_TerrainEdit.SetCurBaseTexture(filepath);
		m_DefaultTexture = filepath;

		UpdateData(FALSE);
	}*/
	const char *pfilter = "Texture Files (.otex;.dds)\0*.otex;*.dds;\0Image Files (.dds;)\0*.dds;\0\0";
	CString path = GetOpenFilePath(TRUE, "scene\\tileset", "", "", pfilter, TRUE);
	if(!path.IsEmpty())
	{
		g_TerrainEdit.SetCurBaseTexture(path.GetBuffer());
		m_DefaultTexture = path;

		UpdateData(FALSE);
	}
}

BOOL CCreateMapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_MapName = "空地图";
	m_MapID = 1001;
	m_MapSizeX = 16;
	m_MapSizeZ = 16;
	m_GridSizeX = 32;
	m_GridSizeZ = 32;
	m_MapCoordX = 0;
	m_MapCoordZ = 0;
	m_MapHeight = 100;
	m_MapMaxHeight = 100;
	m_MapTexRepeat = 12;
	m_DefaultTexture = "scene\\tileset\\tile\\cbcl\\cbcl_db001.dds";

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCreateMapDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	
	if(m_DefaultTexture.IsEmpty())
	{
		AfxMessageBox("需要选择缺省的地表贴图");
		return;
	}

	TE::GameMapInfo *pinfo = g_TerrainEdit.getMapInfo();

	pinfo->mapid = m_MapID;
	pinfo->ngridx = m_MapSizeX;
	pinfo->ngridz = m_MapSizeZ;
	pinfo->tilex = pinfo->tilez = 0;
	pinfo->texrepeat = m_MapTexRepeat;
	strcpy(pinfo->mapname, m_MapName.GetBuffer());

	g_TerrainEdit.CreateTitle(m_MapCoordX, m_MapCoordZ, TERRTILE_SIZE, m_MapSizeX, m_MapSizeZ, m_MapHeight, m_MapMaxHeight, 3, 3, m_DefaultTexture.GetBuffer(), m_MapTexRepeat, m_GridSizeX, m_GridSizeZ);

	CDialog::OnOK();
}

void CCreateMapDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	OnOK();
}

void CCreateMapDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
