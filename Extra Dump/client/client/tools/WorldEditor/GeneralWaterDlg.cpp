// GeneralWaterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "GeneralWaterDlg.h"
#include "TerrainEdit.h"

using namespace TE;
// CGeneralWaterDlg dialog

IMPLEMENT_DYNAMIC(CGeneralWaterDlg, CDialog)

CGeneralWaterDlg::CGeneralWaterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGeneralWaterDlg::IDD, pParent)
	, m_WaterAlpha1(0)
	, m_WaterAlpha2(0)
	, m_WaterDepth1(0)
	, m_WaterDepth2(0)
{

}

CGeneralWaterDlg::~CGeneralWaterDlg()
{
}

void CGeneralWaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_WATERTEXTURE, m_BaseTexSelect);
	DDX_Control(pDX, IDC_SLIDER_WATERTEXREPEAT, m_TexRepeatSlider);
	DDX_Control(pDX, IDC_BTN_WATERCOLOR1, m_WaterColor1);
	DDX_Control(pDX, IDC_BTN_WATERCOLOR2, m_WaterColor2);
	DDX_Text(pDX, IDC_EDIT_WATERALPHA1, m_WaterAlpha1);
	DDV_MinMaxInt(pDX, m_WaterAlpha1, 0, 100);
	DDX_Text(pDX, IDC_EDIT_WATERALPHA2, m_WaterAlpha2);
	DDV_MinMaxInt(pDX, m_WaterAlpha2, 0, 100);
	DDX_Text(pDX, IDC_EDIT_WATERDEPTH1, m_WaterDepth1);
	DDX_Text(pDX, IDC_EDIT_WATERDEPTH2, m_WaterDepth2);
}


BEGIN_MESSAGE_MAP(CGeneralWaterDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT_WATERALPHA2, OnEnChangeEditWateralpha2)
	ON_BN_CLICKED(IDC_BTN_WATERCOLOR1, OnBnClickedBtnWatercolor1)
	ON_BN_CLICKED(IDC_BTN_WATERCOLOR2, OnBnClickedBtnWatercolor2)
	ON_CBN_SELCHANGE(IDC_COMBO_WATERTEXTURE, OnCbnSelchangeComboWatertexture)
	ON_EN_CHANGE(IDC_EDIT_WATERALPHA1, OnEnChangeEditWateralpha1)
	ON_EN_CHANGE(IDC_EDIT_WATERDEPTH1, OnEnChangeEditWaterdepth1)
	ON_EN_CHANGE(IDC_EDIT_WATERDEPTH2, OnEnChangeEditWaterdepth2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_WATERTEXREPEAT, OnNMReleasedcaptureSliderWatertexrepeat)
END_MESSAGE_MAP()

// CGeneralWaterDlg message handlers
static void ConvertColor(Ogre::ColourValue &dest, COLORREF rgb, int alpha)
{
	dest.r = GetRValue(rgb)/255.0f;
	dest.g = GetGValue(rgb)/255.0f;
	dest.b = GetBValue(rgb)/255.0f;
	dest.a = alpha/100.0f;
}

void CGeneralWaterDlg::UpdateToTerrainEdit()
{
	UpdateData();

	TE::EditOptionWater &water = g_TerrainEdit.m_EditOption.water;

	CString str;
	int index = m_BaseTexSelect.GetCurSel();
	if(index < 0)
		index = 0;
	m_BaseTexSelect.GetLBText(index, str);
	strcpy(water.animtex, str.GetString());

	ConvertColor(water.watercolor[0], m_WaterColor1.GetColor(), m_WaterAlpha1);
	ConvertColor(water.watercolor[1], m_WaterColor2.GetColor(), m_WaterAlpha2);

	water.waterdepth[0] = m_WaterDepth1;
	water.waterdepth[1] = m_WaterDepth2;
	water.ntexrepeat = (float)m_TexRepeatSlider.GetPos();
}

void CGeneralWaterDlg::OnEnChangeEditWateralpha2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateToTerrainEdit();
}

void CGeneralWaterDlg::OnBnClickedBtnWatercolor1()
{
	UpdateToTerrainEdit();
}

void CGeneralWaterDlg::OnBnClickedBtnWatercolor2()
{
	UpdateToTerrainEdit();
}

BOOL CGeneralWaterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_WaterColor1.SetColor(RGB(0, 25, 127));
	m_WaterColor2.SetColor(RGB(255, 25, 76));

	m_WaterAlpha1 = 20;
	m_WaterAlpha2 = 80;
	m_WaterDepth1 = 100.0f;
	m_WaterDepth2 = 500.0f;

	m_BaseTexSelect.AddString("slime");

	m_TexRepeatSlider.SetRange(1, 16);
	m_TexRepeatSlider.SetPos(1);

	UpdateData(FALSE);
	UpdateToTerrainEdit();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CGeneralWaterDlg::OnCbnSelchangeComboWatertexture()
{
	// TODO: Add your control notification handler code here
	UpdateToTerrainEdit();
}

void CGeneralWaterDlg::OnEnChangeEditWateralpha1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateToTerrainEdit();
}

void CGeneralWaterDlg::OnEnChangeEditWaterdepth1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateToTerrainEdit();
}

void CGeneralWaterDlg::OnEnChangeEditWaterdepth2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateToTerrainEdit();
}

void CGeneralWaterDlg::OnNMReleasedcaptureSliderWatertexrepeat(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	UpdateToTerrainEdit();

	*pResult = 0;
}