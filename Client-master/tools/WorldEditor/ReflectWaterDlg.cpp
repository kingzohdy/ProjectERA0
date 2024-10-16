// ReflectWaterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "ReflectWaterDlg.h"
#include "TerrainEdit.h"
#include "Terrain.h"
#include "Ogrereflect.h"

using namespace TE;
// CReflectWaterDlg dialog

IMPLEMENT_DYNAMIC(CReflectWaterDlg, CDialog)

CReflectWaterDlg::CReflectWaterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReflectWaterDlg::IDD, pParent)
	, m_WaterAlpha1(0)
	, m_WaterAlpha2(0)
	, m_WaterDepth1(0)
	, m_WaterDepth2(0)
{

}

CReflectWaterDlg::~CReflectWaterDlg()
{
}

BOOL CReflectWaterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_WaterColor1.SetColor(RGB(0, 25, 127));
	m_WaterColor2.SetColor(RGB(255, 25, 76));

	m_WaterAlpha1 = 20;
	m_WaterAlpha2 = 80;

	m_WaterDepth1 = 100.0f;
	m_WaterDepth2 = 500.0f;

	m_SpeedSlider.SetRange(0, 32);
	m_SpeedSlider.SetPos(16);

	m_TexRepeatSlider.SetRange(1, 32);
	m_TexRepeatSlider.SetPos(8);

	m_AmpSlider.SetRange(0, 32);
	m_AmpSlider.SetPos(1);

	m_AlphaSlider.SetRange(0, 255);
	m_AlphaSlider.SetPos(125);
	
	UpdateData(FALSE);
	//UpdateToTerrainEdit();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void ConvertColor(Ogre::ColourValue &dest, COLORREF rgb, int alpha)
{
	dest.r = GetRValue(rgb)/255.0f;
	dest.g = GetGValue(rgb)/255.0f;
	dest.b = GetBValue(rgb)/255.0f;
	dest.a = alpha/100.0f;
}

void InvertConvertColor(Ogre::ColourValue dest, COLORREF& rgb)
{
	DWORD r = dest.r*255;
	DWORD g = dest.g*255;
	DWORD b = dest.b*255;
	rgb = r + g<< 8 + b<<16;
}

void CReflectWaterDlg::UpdateToTerrainEdit()
{
	TE::EditOptionWater &water = g_TerrainEdit.m_EditOption.water;

	UpdateData(TRUE);
	water.ntexrepeat = (float)m_TexRepeatSlider.GetPos();
	water.speed		 =	0.25f*m_SpeedSlider.GetPos();
	//water.amp		 =	0.25f*m_AmpSlider.GetPos();
	//临时代码
	//这个地方透明度和最大强度共用
	WORD* ampalpha = (WORD*)&water.amp;
	ampalpha[0] = m_AmpSlider.GetPos();
	ampalpha[1]	= m_AlphaSlider.GetPos();

	ConvertColor(water.watercolor[0], m_WaterColor1.GetColor(), m_WaterAlpha1);
	ConvertColor(water.watercolor[1], m_WaterColor2.GetColor(), m_WaterAlpha2);

	water.waterdepth[0] = m_WaterDepth1;
	water.waterdepth[1] = m_WaterDepth2;
	
	g_TerrainEdit.UpdateLiqduiOption(water);
	//if(g_Terrain.m_pReflect)
	//	g_Terrain.m_pReflect->m_Height = g_TerrainEdit.m_EditOption.water.waterheight;

}

void CReflectWaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER2, m_TexRepeatSlider);
	DDX_Control(pDX, IDC_SLIDER1, m_SpeedSlider);
	DDX_Control(pDX, IDC_SLIDER3, m_AmpSlider);
	DDX_Control(pDX, IDC_SLIDER4, m_AlphaSlider);

	DDX_Control(pDX, IDC_BTN_REFLECTWATERCOLOR1, m_WaterColor1);
	DDX_Control(pDX, IDC_BTN_REFLECTWATERCOLOR2, m_WaterColor2);
	DDX_Text(pDX, IDC_EDIT_REFLECTWATERALPHA1, m_WaterAlpha1);
	DDV_MinMaxInt(pDX, m_WaterAlpha1, 0, 100);
	DDX_Text(pDX, IDC_EDIT_REFLECTWATERALPHA2, m_WaterAlpha2);
	DDV_MinMaxInt(pDX, m_WaterAlpha2, 0, 100);
	DDX_Text(pDX, IDC_EDIT_REFLECTWATERDEPTH1, m_WaterDepth1);
	DDX_Text(pDX, IDC_EDIT_REFLECTWATERDEPTH2, m_WaterDepth2);

}

void CReflectWaterDlg::SetDataToUI()
{
	size_t nspeed = g_TerrainEdit.m_EditOption.water.speed*4;
	m_SpeedSlider.SetPos(nspeed);

	size_t ntexrepeat = g_TerrainEdit.m_EditOption.water.ntexrepeat;
	m_TexRepeatSlider.SetPos(ntexrepeat);

	WORD* ampalpha = (WORD*)&g_TerrainEdit.m_EditOption.water.amp;

	size_t namp		= ampalpha[0];
	m_AmpSlider.SetPos(namp);

	size_t nalpha	= ampalpha[1];
	m_AlphaSlider.SetPos(nalpha);

	COLORREF color0,color1;
	InvertConvertColor(g_TerrainEdit.m_EditOption.water.watercolor[0],color0);
	InvertConvertColor(g_TerrainEdit.m_EditOption.water.watercolor[1],color1);
	m_WaterColor1.SetColor(color0);
	m_WaterColor2.SetColor(color1);

	m_WaterAlpha1 = g_TerrainEdit.m_EditOption.water.watercolor[0].a*100;
	m_WaterAlpha2 = g_TerrainEdit.m_EditOption.water.watercolor[1].a*100;

	m_WaterDepth1 = g_TerrainEdit.m_EditOption.water.waterdepth[0];
	m_WaterDepth1 = g_TerrainEdit.m_EditOption.water.waterdepth[1];



}

BEGIN_MESSAGE_MAP(CReflectWaterDlg, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CReflectWaterDlg::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CReflectWaterDlg::OnNMCustomdrawSlider2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, &CReflectWaterDlg::OnNMCustomdrawSlider3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER4, &CReflectWaterDlg::OnNMCustomdrawSlider4)
	ON_BN_CLICKED(IDC_BTN_REFLECTWATERCOLOR1, &CReflectWaterDlg::OnBnClickedBtnReflectwatercolor1)
	ON_EN_CHANGE(IDC_EDIT_REFLECTWATERALPHA1, &CReflectWaterDlg::OnEnChangeEditReflectwateralpha1)
	ON_EN_CHANGE(IDC_EDIT_REFLECTWATERDEPTH1, &CReflectWaterDlg::OnEnChangeEditReflectwaterdepth1)
	ON_BN_CLICKED(IDC_BTN_REFLECTWATERCOLOR2, &CReflectWaterDlg::OnBnClickedBtnReflectwatercolor2)
	ON_EN_CHANGE(IDC_EDIT_REFLECTWATERALPHA2, &CReflectWaterDlg::OnEnChangeEditReflectwateralpha2)
	ON_EN_CHANGE(IDC_EDIT_REFLECTWATERDEPTH2, &CReflectWaterDlg::OnEnChangeEditReflectwaterdepth2)
END_MESSAGE_MAP()


// CReflectWaterDlg message handlers

void CReflectWaterDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateToTerrainEdit();
	*pResult = 0;
}

void CReflectWaterDlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateToTerrainEdit();
	*pResult = 0;
}

void CReflectWaterDlg::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateToTerrainEdit();
	*pResult = 0;
}

void CReflectWaterDlg::OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateToTerrainEdit();
	*pResult = 0;
}

void CReflectWaterDlg::OnBnClickedBtnReflectwatercolor1()
{
	UpdateToTerrainEdit();
}

void CReflectWaterDlg::OnEnChangeEditReflectwateralpha1()
{
	UpdateToTerrainEdit();
}

void CReflectWaterDlg::OnEnChangeEditReflectwaterdepth1()
{
	UpdateToTerrainEdit();
}

void CReflectWaterDlg::OnBnClickedBtnReflectwatercolor2()
{
	UpdateToTerrainEdit();
}

void CReflectWaterDlg::OnEnChangeEditReflectwateralpha2()
{
	UpdateToTerrainEdit();
}

void CReflectWaterDlg::OnEnChangeEditReflectwaterdepth2()
{
	UpdateToTerrainEdit();
}
