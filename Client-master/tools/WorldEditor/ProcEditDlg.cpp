// ProcEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "ProcEditDlg.h"
#include "MapPlayerBrower.h"
#include "EditEventHandlerMgr.h"

#include "EnvRegion.h"

using namespace TE;
// CProcEditDlg dialog

IMPLEMENT_DYNAMIC(CProcEditDlg, CDialog)

CProcEditDlg::CProcEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcEditDlg::IDD, pParent)
	, m_LinkMapID(0)
	, m_LinkPosX(0)
	, m_LinkPosY(0)
	, m_LinkDir(0)
	, m_BackPosX(0)
	, m_BackPosY(0)
	, m_BackDir(0)
	, m_PreviewActorDir(0)
	, m_PreviewCameraDir(0)
	, m_LinkCameraDir(0)
	, m_PreviewActorX(0)
	, m_PreviewActorY(0)
{
	m_bInit = false;
}

CProcEditDlg::~CProcEditDlg()
{
}

void CProcEditDlg::GetDataFromUI()
{
	if(!m_bInit) return;
	m_bInit = false;

	UpdateData();

	TE::MapArea *parea = g_AreaSet.GetCurSelectArea();
	parea->getProcInfo().link_mapid = m_LinkMapID;
	parea->getProcInfo().link_mappos.x = m_LinkPosX;
	parea->getProcInfo().link_mappos.y = m_LinkPosY;
	parea->getProcInfo().link_mapdir = m_LinkDir;
	parea->getProcInfo().link_mapcamdir = m_LinkCameraDir;
	parea->getProcInfo().fail_mappos.x = m_BackPosX;
	parea->getProcInfo().fail_mappos.y = m_BackPosY;
	parea->getProcInfo().fail_mapdir = m_BackDir;

	m_LinkScript.GetWindowText(parea->getProcInfo().script, 256);

	m_bInit = true;
}

void CProcEditDlg::SetDataToUI()
{
	if(!m_bInit) return;
	m_bInit = false;

	TE::MapArea *parea = g_AreaSet.GetCurSelectArea();
	m_LinkMapID = parea->getProcInfo().link_mapid;
	m_LinkPosX = parea->getProcInfo().link_mappos.x;
	m_LinkPosY = parea->getProcInfo().link_mappos.y;
	m_LinkDir = parea->getProcInfo().link_mapdir;
	m_LinkCameraDir = parea->getProcInfo().link_mapcamdir;
	m_BackPosX = parea->getProcInfo().fail_mappos.x;
	m_BackPosY = parea->getProcInfo().fail_mappos.y;
	m_BackDir = parea->getProcInfo().fail_mapdir;

	m_LinkScript.SetWindowText(parea->getProcInfo().script);

	UpdateData(FALSE);

	m_bInit = true;
}

void CProcEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LINK_MAPID, m_LinkMapID);
	DDX_Text(pDX, IDC_EDIT_LINK_POSX, m_LinkPosX);
	DDX_Text(pDX, IDC_EDIT_LINK_POSY, m_LinkPosY);
	DDX_Text(pDX, IDC_EDIT_LINK_DIR, m_LinkDir);
	DDX_Text(pDX, IDC_EDIT_LINK_CAMERADIR, m_LinkCameraDir);
	DDX_Text(pDX, IDC_EDIT_BACK_POSX, m_BackPosX);
	DDX_Text(pDX, IDC_EDIT_BACK_POSY, m_BackPosY);
	DDX_Text(pDX, IDC_EDIT_BACK_DIR, m_BackDir);
	DDX_Text(pDX, IDC_EDIT_PREVIEW_ACTORDIR, m_PreviewActorDir);
	DDX_Text(pDX, IDC_EDIT_PREVIEW_CAMERADIR, m_PreviewCameraDir);
	DDX_Text(pDX, IDC_EDIT_PREVIEW_ACTORX, m_PreviewActorX);
	DDX_Text(pDX, IDC_EDIT_PREVIEW_ACTORY, m_PreviewActorY);
	DDX_Control(pDX, IDC_EDIT_LINK_SCRIPT, m_LinkScript);
}


BEGIN_MESSAGE_MAP(CProcEditDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT_LINK_MAPID, &CProcEditDlg::OnEnChangeEditLinkMapid)
	ON_EN_CHANGE(IDC_EDIT_LINK_POSX, &CProcEditDlg::OnEnChangeEditLinkPosx)
	ON_EN_CHANGE(IDC_EDIT_LINK_POSY, &CProcEditDlg::OnEnChangeEditLinkPosy)
	ON_EN_CHANGE(IDC_EDIT_LINK_DIR, &CProcEditDlg::OnEnChangeEditLinkDir)
	ON_EN_CHANGE(IDC_EDIT_BACK_POSX, &CProcEditDlg::OnEnChangeEditBackPosx)
	ON_EN_CHANGE(IDC_EDIT_BACK_POSY, &CProcEditDlg::OnEnChangeEditBackPosy)
	ON_EN_CHANGE(IDC_EDIT_BACK_DIR, &CProcEditDlg::OnEnChangeEditBackDir)
	ON_BN_CLICKED(IDC_BUTTON_GET_PREVIEWDIR, &CProcEditDlg::OnBnClickedButtonGetPreviewdir)
	ON_EN_CHANGE(IDC_EDIT_LINK_SCRIPT, &CProcEditDlg::OnEnChangeEditLinkScript)
END_MESSAGE_MAP()


// CProcEditDlg message handlers

void CProcEditDlg::OnEnChangeEditLinkMapid()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	GetDataFromUI();
}

void CProcEditDlg::OnEnChangeEditLinkPosx()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	GetDataFromUI();
}

void CProcEditDlg::OnEnChangeEditLinkPosy()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	GetDataFromUI();
}

void CProcEditDlg::OnEnChangeEditLinkDir()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	GetDataFromUI();
}

void CProcEditDlg::OnEnChangeEditBackPosx()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	GetDataFromUI();
}

void CProcEditDlg::OnEnChangeEditBackPosy()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	GetDataFromUI();
}

void CProcEditDlg::OnEnChangeEditBackDir()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	GetDataFromUI();
}

void CProcEditDlg::OnBnClickedButtonGetPreviewdir()
{
	// TODO: Add your control notification handler code here
	MapPlayerBrower *player = static_cast<MapPlayerBrower *>(g_EventProcessFactory.GetEventProcess(PLACE_PLYAER));
	m_PreviewActorDir = player->getPlayerDir();
	m_PreviewCameraDir = player->getCameraDir();
	m_PreviewActorX = (int)player->getCurPos().x;
	m_PreviewActorY = (int)player->getCurPos().z;

	UpdateData(FALSE);
}

void CProcEditDlg::OnEnChangeEditLinkScript()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	GetDataFromUI();
}

BOOL CProcEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_bInit = true;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}