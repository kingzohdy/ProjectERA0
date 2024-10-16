// TerrainLayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "TerrainLayerDlg.h"
#include "TerrainEdit.h"
#include "EditorManager.h"

using namespace TE;
// CTerrainLayerDlg dialog

IMPLEMENT_DYNAMIC(CTerrainLayerDlg, CDialog)

CTerrainLayerDlg::CTerrainLayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTerrainLayerDlg::IDD, pParent)
{

}

CTerrainLayerDlg::~CTerrainLayerDlg()
{
}

void CTerrainLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_TEXBRUSH_SIZE, m_BrushSizeSlider);
	DDX_Control(pDX, IDC_SLIDER_TEXBRUSH_STRENGTH, m_BrushStrengthSlider);
	DDX_Control(pDX, IDC_SLIDER_PLANT_SCALE, m_PlantScaleSlider);
}


BEGIN_MESSAGE_MAP(CTerrainLayerDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TEXSHAPE, &CTerrainLayerDlg::OnBnClickedButtonTexshape)
	ON_EN_CHANGE(IDC_EDIT_TEXBRUSH_SIZE, &CTerrainLayerDlg::OnEnChangeEditTexbrushSize)
	ON_EN_CHANGE(IDC_EDIT_TEXBRUSH_STRENGTH, &CTerrainLayerDlg::OnEnChangeEditTexbrushStrength)
	ON_EN_CHANGE(IDC_EDIT_PLANT_SCALE, &CTerrainLayerDlg::OnEnChangeEditPlantScale)
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDC_RADIO_LAYER1, &CTerrainLayerDlg::OnBnClickedRadioLayer1)
	ON_BN_CLICKED(IDC_RADIO_LAYER2, &CTerrainLayerDlg::OnBnClickedRadioLayer2)
	ON_BN_CLICKED(IDC_RADIO_LAYER3, &CTerrainLayerDlg::OnBnClickedRadioLayer3)
	ON_BN_CLICKED(IDC_RADIO_LAYER4, &CTerrainLayerDlg::OnBnClickedRadioLayer4)
	ON_BN_CLICKED(IDC_RADIO_LAYER5, &CTerrainLayerDlg::OnBnClickedRadioLayer5)
	ON_BN_CLICKED(IDC_RADIO_LAYER6, &CTerrainLayerDlg::OnBnClickedRadioLayer6)
	ON_BN_CLICKED(IDC_RADIO_LAYER7, &CTerrainLayerDlg::OnBnClickedRadioLayer7)
	ON_BN_CLICKED(IDC_RADIO_LAYER8, &CTerrainLayerDlg::OnBnClickedRadioLayer8)
	ON_BN_CLICKED(IDC_RADIO_LAYER9, &CTerrainLayerDlg::OnBnClickedRadioLayer9)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_TEXBRUSH_SIZE, &CTerrainLayerDlg::OnNMReleasedcaptureSliderTexbrushSize)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_TEXBRUSH_STRENGTH, &CTerrainLayerDlg::OnNMReleasedcaptureSliderTexbrushStrength)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_PLANT_SCALE, &CTerrainLayerDlg::OnNMReleasedcaptureSliderPlantScale)
	ON_BN_CLICKED(IDC_RADIO_PAINTSHAPE_CIRCLE, &CTerrainLayerDlg::OnBnClickedRadioPaintshapeCircle)
	ON_BN_CLICKED(IDC_RADIO_PAINTSHAPE_RECT, &CTerrainLayerDlg::OnBnClickedRadioPaintshapeRect)
	ON_BN_CLICKED(IDC_RADIO_PAINTSHAPE_RANDOMCIRCLE, &CTerrainLayerDlg::OnBnClickedRadioPaintshapeRandomcircle)
	ON_BN_CLICKED(IDC_RADIO_PAINTSHAPE_RANDOMRECT, &CTerrainLayerDlg::OnBnClickedRadioPaintshapeRandomrect)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_TEXBRUSH_SIZE, &CTerrainLayerDlg::OnNMCustomdrawSliderTexbrushSize)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_TEXBRUSH_STRENGTH, &CTerrainLayerDlg::OnNMCustomdrawSliderTexbrushStrength)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_PLANT_SCALE, &CTerrainLayerDlg::OnNMCustomdrawSliderPlantScale)
	ON_BN_CLICKED(IDC_RADIO_LAYER10, &CTerrainLayerDlg::OnBnClickedRadioLayer10)
	ON_BN_CLICKED(IDC_IMPORT_WALKBMP, &CTerrainLayerDlg::OnBnClickedImportWalkbmp)
END_MESSAGE_MAP()


// CTerrainLayerDlg message handlers

BOOL CTerrainLayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CheckDlgButton(IDC_RADIO_LAYER2, 1);
	CheckDlgButton(IDC_RADIO_PAINTSHAPE_CIRCLE, 1);

	m_BrushSizeSlider.SetRange(1, 32);
	m_BrushSizeSlider.SetPos(8);
	m_BrushSizeSlider.SetPageSize(1);
	SetDlgItemInt(IDC_EDIT_TEXBRUSH_SIZE, 8);

	m_BrushStrengthSlider.SetRange(1, 64);
	m_BrushStrengthSlider.SetPos(32);
	SetDlgItemInt(IDC_EDIT_TEXBRUSH_STRENGTH, 32);

	m_PlantScaleSlider.SetRange(1, 200);
	m_PlantScaleSlider.SetPos(100);
	SetDlgItemInt(IDC_EDIT_PLANT_SCALE, 100);

	SelectLayer(1);
	SelectPaintShape(TE::PAINT_CIRCLE);
	g_TerrainEdit.SetPaintVec(32);
	g_TerrainEdit.SetCurAlphaSize(8);
	g_TerrainEdit.m_EditOption.fPlantScale = 1.0f;


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTerrainLayerDlg::OnBnClickedButtonTexshape()
{
	// TODO: Add your control notification handler code here
}

void CTerrainLayerDlg::SelectPaintShape(int mode)
{
	g_TerrainEdit.SetCurPaintMode((TE::PAINT_MODE)mode);
	g_TerrainEdit.SetUsePaint(false);
	g_TerrainEdit.CreateAlphaPaint();
}

void CTerrainLayerDlg::SelectLayer(int sel)
{
	// TODO: Add your control notification handler code here
	if(sel < 6)
	{
		if(sel == 5) g_TerrainEdit.SetCurLayer(6); //Ö²±»²ã
		else g_TerrainEdit.SetCurLayer(sel); //»ìºÏ²ã + ÒõÓ°²ã
		g_TerrainEdit.ResetPaintTexture();
		g_TerrainEdit.m_DrawOption.bUserColorMap = false;
	}
	else
	{
		g_TerrainEdit.SetCurLayer(5); //Âß¼­ÑÕÉ«²ã
		g_TerrainEdit.ResetPaintTexture();

		int offset = sel-6;
		unsigned char color[3][3] = {{0,255,0}, {0, 0, 255}, {255, 0, 0}};

		g_TerrainEdit.SetColorMap(color[offset][0], color[offset][1], color[offset][2]);
		g_TerrainEdit.m_EditOption.gameInfo = (TE::PAINT_GAME_IFNO)(TE::WALK_LAYER+offset);

		g_TerrainEdit.UpateCurGameInfoToColorMap();
		g_TerrainEdit.m_DrawOption.bUserColorMap = true;
	}
}

void CTerrainLayerDlg::OnEnChangeEditTexbrushSize()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	int s = GetDlgItemInt(IDC_EDIT_TEXBRUSH_SIZE, NULL, FALSE);
	g_TerrainEdit.SetCurAlphaSize(s);
}

void CTerrainLayerDlg::OnEnChangeEditTexbrushStrength()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	int s = GetDlgItemInt(IDC_EDIT_TEXBRUSH_STRENGTH, NULL, FALSE);
	g_TerrainEdit.SetPaintVec(s);
	g_TerrainEdit.CreateAlphaPaint();
}

void CTerrainLayerDlg::OnEnChangeEditPlantScale()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	int s = GetDlgItemInt(IDC_EDIT_PLANT_SCALE, NULL, FALSE);
	g_TerrainEdit.m_EditOption.fPlantScale = s/100.0f ;
}

void CTerrainLayerDlg::OnSetFocus(CWnd* pOldWnd)
{
	CDialog::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
	g_TerrainEdit.SetCurEditEvent(TE::PANIT_EVN);
}

void CTerrainLayerDlg::OnBnClickedRadioLayer1()
{
	// TODO: Add your control notification handler code here
	SelectLayer(0);
}

void CTerrainLayerDlg::OnBnClickedRadioLayer2()
{
	// TODO: Add your control notification handler code here
	SelectLayer(1);
}

void CTerrainLayerDlg::OnBnClickedRadioLayer3()
{
	// TODO: Add your control notification handler code here
	SelectLayer(2);
}

void CTerrainLayerDlg::OnBnClickedRadioLayer4()
{
	// TODO: Add your control notification handler code here
	SelectLayer(3);
}

void CTerrainLayerDlg::OnBnClickedRadioLayer5()
{
	// TODO: Add your control notification handler code here
	SelectLayer(4);
}

void CTerrainLayerDlg::OnBnClickedRadioLayer6()
{
	// TODO: Add your control notification handler code here
	SelectLayer(5);
}

void CTerrainLayerDlg::OnBnClickedRadioLayer7()
{
	// TODO: Add your control notification handler code here
	SelectLayer(6);
}

void CTerrainLayerDlg::OnBnClickedRadioLayer8()
{
	// TODO: Add your control notification handler code here
	SelectLayer(7);
}

void CTerrainLayerDlg::OnBnClickedRadioLayer9()
{
	// TODO: Add your control notification handler code here
	SelectLayer(8);
}

void CTerrainLayerDlg::OnNMReleasedcaptureSliderTexbrushSize(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int pos = m_BrushSizeSlider.GetPos();
	SetDlgItemInt(IDC_EDIT_TEXBRUSH_SIZE, pos);
}

void CTerrainLayerDlg::OnNMReleasedcaptureSliderTexbrushStrength(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int pos = m_BrushStrengthSlider.GetPos();
	SetDlgItemInt(IDC_EDIT_TEXBRUSH_STRENGTH, pos);
}

void CTerrainLayerDlg::OnNMReleasedcaptureSliderPlantScale(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int pos = m_PlantScaleSlider.GetPos();
	SetDlgItemInt(IDC_EDIT_PLANT_SCALE, pos);
}

void CTerrainLayerDlg::OnBnClickedRadioPaintshapeCircle()
{
	// TODO: Add your control notification handler code here
	SelectPaintShape(TE::PAINT_CIRCLE);
}

void CTerrainLayerDlg::OnBnClickedRadioPaintshapeRect()
{
	// TODO: Add your control notification handler code here
	SelectPaintShape(TE::PAINT_QUAD);
}

void CTerrainLayerDlg::OnBnClickedRadioPaintshapeRandomcircle()
{
	// TODO: Add your control notification handler code here
	SelectPaintShape(TE::PAINT_RAND_CIRCLE);
}

void CTerrainLayerDlg::OnBnClickedRadioPaintshapeRandomrect()
{
	// TODO: Add your control notification handler code here
	SelectPaintShape(TE::PAINT_RAND_QUAD);
}

void CTerrainLayerDlg::OnNMCustomdrawSliderTexbrushSize(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CTerrainLayerDlg::OnNMCustomdrawSliderTexbrushStrength(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CTerrainLayerDlg::OnNMCustomdrawSliderPlantScale(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CTerrainLayerDlg::OnBnClickedRadioLayer10()
{
	// TODO: Add your control notification handler code here
	SelectLayer(9);
}

void CTerrainLayerDlg::OnBnClickedImportWalkbmp()
{
	// TODO: Add your control notification handler code here
	g_pApp->ImportMaskFile();
}
