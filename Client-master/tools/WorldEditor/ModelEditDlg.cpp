// ModelEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "ModelEditDlg.h"
#include "TerrainEdit.h"
#include "LinkTile.h"
#include "EditEvent_ObjectBrush.h"
#include "EditEventHandlerMgr.h"

using namespace TE;

namespace TE
{
	extern  bool  g_bUserWaterHeigh;
}

// CModelEditDlg dialog

IMPLEMENT_DYNAMIC(CModelEditDlg, CDialog)

CModelEditDlg::CModelEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModelEditDlg::IDD, pParent)
{

}

CModelEditDlg::~CModelEditDlg()
{
}

void CModelEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CModelEditDlg, CDialog)
	ON_COMMAND_RANGE(IDC_CHECK_BIG_VISIBLE, IDC_RADIO_PLANT_MODEL, &CModelEditDlg::OnBnClickedCommand)
	ON_COMMAND_RANGE(IDC_CHECK_CLIFF_DELTERRAIN, IDC_CHECK_CLIFF_AUTOLINK, &CModelEditDlg::OnOptionCheckCommand)
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDC_CHECK_LINKMESH_VISIBLE, &CModelEditDlg::OnBnClickedCheckLinkmeshVisible)
	ON_BN_CLICKED(IDC_CHECK_GRASS_VISIBLE, &CModelEditDlg::OnBnClickedCheckGrassVisible)
	ON_BN_CLICKED(IDC_CHECK_GLOBALMODEL_VISIBLE, &CModelEditDlg::OnBnClickedCheckGlobalmodelVisible)
	ON_BN_CLICKED(IDC_CHECK_GLOBALMODEL_LOCK, &CModelEditDlg::OnBnClickedCheckGlobalmodelLock)
	ON_BN_CLICKED(IDC_RADIO_GLOBAL_MODEL, &CModelEditDlg::OnBnClickedRadioGlobalModel)
	ON_BN_CLICKED(IDC_CHECK_MODEL_SNAPWATER, &CModelEditDlg::OnBnClickedCheckModelSnapwater)
	ON_BN_CLICKED(IDC_CHECK_BIG_LOCK, &CModelEditDlg::OnBnClickedCheckBigLock)
	ON_BN_CLICKED(IDC_CHECK_SMALL_LOCK, &CModelEditDlg::OnBnClickedCheckSmallLock)
	ON_BN_CLICKED(IDC_CHECK_DETAIL_LOCK, &CModelEditDlg::OnBnClickedCheckDetailLock)
	ON_BN_CLICKED(IDC_CHECK_PLANT_LOCK, &CModelEditDlg::OnBnClickedCheckPlantLock)
	ON_BN_CLICKED(IDC_CHECK_CLIFF_AUTOLINK, &CModelEditDlg::OnBnClickedCheckCliffAutolink)
	ON_BN_CLICKED(IDC_CHECK_MODEL_SNAPGRID, &CModelEditDlg::OnBnClickedCheckModelSnapgrid)
	ON_EN_CHANGE(IDC_EDIT_MODELAREA_SIZE, &CModelEditDlg::OnEnChangeEditModelareaSize)
	ON_BN_CLICKED(IDC_RADIO_BIG_MODEL, &CModelEditDlg::OnBnClickedRadioBigModel)
	ON_BN_CLICKED(IDC_BUTTON_MODELAREA_EDIT, &CModelEditDlg::OnBnClickedButtonModelareaEdit)
	ON_BN_CLICKED(IDC_RADIO_DETAIL_MODEL, &CModelEditDlg::OnBnClickedRadioDetailModel)
	ON_BN_CLICKED(IDC_RADIO_PLANT_MODEL, &CModelEditDlg::OnBnClickedRadioPlantModel)
END_MESSAGE_MAP()


// CModelEditDlg message handlers


void CModelEditDlg::OnBnClickedCommand(WPARAM wParam)
{
	// TODO: Add your control notification handler code here
	bool check = IsDlgButtonChecked(wParam)>0;

	if(wParam>=IDC_CHECK_BIG_VISIBLE && wParam<=IDC_CHECK_PLANT_VISIBLE)
	{
		g_TerrainEdit.SetTypeModelShow(wParam-IDC_CHECK_BIG_VISIBLE, check);
	}
	else if(wParam>=IDC_CHECK_BIG_LOCK && wParam<=IDC_CHECK_PLANT_LOCK)
	{
		g_TerrainEdit.m_bLockModelEdit[wParam-IDC_CHECK_BIG_LOCK] = check;
	}
	else if(wParam>=IDC_RADIO_BIG_MODEL && wParam<=IDC_RADIO_PLANT_MODEL)
	{
		if(check) g_TerrainEdit.SetCurSelectModelType((TE::MODEL_TYPE)(TE::TYPE_BIG_MODEL+wParam-IDC_RADIO_BIG_MODEL));
	}
}

void CModelEditDlg::OnOptionCheckCommand(WPARAM wParam)
{
	bool check = IsDlgButtonChecked(wParam)>0;
	if(wParam == IDC_CHECK_CLIFF_DELTERRAIN) g_TerrainEdit.SetAutoDelBlock(check);
	else if(wParam == IDC_CHECK_CLIFF_AUTOLINK) g_TerrainEdit.GetAutoLinkMesh()->SetLinkOder(check);
	else if(wParam == IDC_CHECK_MODEL_SNAPGRID) g_TerrainEdit.m_EditOption.bMoveGrid = check;
	else if(wParam == IDC_CHECK_MODEL_SNAPWATER) g_bUserWaterHeigh = check;
}

BOOL CModelEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	g_TerrainEdit.SetTypeModelShow(TYPE_BIG_MODEL, true);
	g_TerrainEdit.SetTypeModelShow(TYPE_SMALL_MODEL, true);
	g_TerrainEdit.SetTypeModelShow(TYPE_DETAIL_MODEL, true);
	g_TerrainEdit.SetTypeModelShow(TYPE_PLANT_MODEL, true);
	g_TerrainEdit.SetTypeModelShow(TYPE_PLANT_GRASS, true);
	g_TerrainEdit.SetTypeModelShow(TYPE_LINK_MODEL, true);
	g_TerrainEdit.SetTypeModelShow(TYPE_GLOBAL_MODEL, true);

	CheckDlgButton(IDC_CHECK_BIG_VISIBLE, 1);
	CheckDlgButton(IDC_CHECK_SMALL_VISIBLE, 1);
	CheckDlgButton(IDC_CHECK_DETAIL_VISIBLE, 1);
	CheckDlgButton(IDC_CHECK_PLANT_VISIBLE, 1);
	CheckDlgButton(IDC_CHECK_GRASS_VISIBLE, 1);
	CheckDlgButton(IDC_CHECK_LINKMESH_VISIBLE, 1);
	CheckDlgButton(IDC_CHECK_GLOBALMODEL_VISIBLE, 1);

	CheckDlgButton(IDC_RADIO_BIG_MODEL, 1);
	g_TerrainEdit.SetCurSelectModelType(TE::TYPE_BIG_MODEL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CModelEditDlg::OnSetFocus(CWnd* pOldWnd)
{
	CDialog::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
	g_TerrainEdit.SetCurEditEvent(TE::EDITEVENT_OBJECTBRUSH);

	ObjectBrushEditHandler *phandler = static_cast<ObjectBrushEditHandler *>(g_EventProcessFactory.GetEventProcess(EDITEVENT_OBJECTBRUSH));
	phandler->setEditObjectType(TE::EDITOBJ_MODEL);
	phandler->setEditOP(TE::OBJEDIT_OP_ADD);
}

void CModelEditDlg::OnBnClickedCheckLinkmeshVisible()
{
	// TODO: Add your control notification handler code here
	bool check = IsDlgButtonChecked(IDC_CHECK_LINKMESH_VISIBLE)>0;
	g_TerrainEdit.SetTypeModelShow(TYPE_LINK_MODEL, check);
}

void CModelEditDlg::OnBnClickedCheckGrassVisible()
{
	// TODO: Add your control notification handler code here
	bool check = IsDlgButtonChecked(IDC_CHECK_GRASS_VISIBLE)>0;
	g_TerrainEdit.SetTypeModelShow(TYPE_PLANT_GRASS, check);
}

void CModelEditDlg::OnBnClickedCheckGlobalmodelVisible()
{
	// TODO: Add your control notification handler code here
	bool check = IsDlgButtonChecked(IDC_CHECK_GLOBALMODEL_VISIBLE)>0;

	g_TerrainEdit.SetTypeModelShow(TYPE_GLOBAL_MODEL, check);
}

void CModelEditDlg::OnBnClickedCheckGlobalmodelLock()
{
	// TODO: Add your control notification handler code here
	bool check = IsDlgButtonChecked(IDC_CHECK_GLOBALMODEL_LOCK)>0;

	g_TerrainEdit.m_bLockModelEdit[TYPE_GLOBAL_MODEL] = check;
}

void CModelEditDlg::OnBnClickedRadioGlobalModel()
{
	// TODO: Add your control notification handler code here
	bool check = IsDlgButtonChecked(IDC_RADIO_GLOBAL_MODEL)>0;
	if(check)
		g_TerrainEdit.SetCurSelectModelType(TYPE_GLOBAL_MODEL);
}
void CModelEditDlg::OnBnClickedCheckModelSnapwater()
{
	// TODO: Add your control notification handler code here
}

void CModelEditDlg::OnBnClickedCheckBigLock()
{
	// TODO: Add your control notification handler code here
}

void CModelEditDlg::OnBnClickedCheckSmallLock()
{
	// TODO: Add your control notification handler code here
}

void CModelEditDlg::OnBnClickedCheckDetailLock()
{
	// TODO: Add your control notification handler code here
}

void CModelEditDlg::OnBnClickedCheckPlantLock()
{
	// TODO: Add your control notification handler code here
}

void CModelEditDlg::OnBnClickedCheckCliffAutolink()
{
	// TODO: Add your control notification handler code here
}

void CModelEditDlg::OnBnClickedCheckModelSnapgrid()
{
	// TODO: Add your control notification handler code here
}

void CModelEditDlg::OnEnChangeEditModelareaSize()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CModelEditDlg::OnBnClickedRadioBigModel()
{
	// TODO: Add your control notification handler code here
}

void CModelEditDlg::OnBnClickedButtonModelareaEdit()
{
	// TODO: Add your control notification handler code here
}
void CModelEditDlg::OnBnClickedRadioDetailModel()
{
	// TODO: Add your control notification handler code here
}

void CModelEditDlg::OnBnClickedRadioPlantModel()
{
	// TODO: Add your control notification handler code here
}
