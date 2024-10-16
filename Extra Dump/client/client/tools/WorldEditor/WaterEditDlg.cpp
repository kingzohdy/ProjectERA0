// WaterEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "WaterEditDlg.h"

#include "TerrainEdit.h"

using namespace TE;
// CWaterEditDlg dialog

IMPLEMENT_DYNAMIC(CWaterEditDlg, CDialog)

CWaterEditDlg::CWaterEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWaterEditDlg::IDD, pParent)
	,m_bInit(false)
	, m_UVSpeed(1.0f)
	, m_BeachWidth(100)
	, m_BeachHeight(5)
	, m_BeachTexture(_T(""))
	, m_uRepeat(10)
	, m_vRepeat(1)
	, m_fUVRange(1.0f)
	, m_maskTextureUse(_T(""))
{

}

CWaterEditDlg::~CWaterEditDlg()
{
}

void CWaterEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_WATERTYPE, m_WaterTypeSelect);
	DDX_Control(pDX, IDC_COMBO_WATERTEXTURE, m_AnimTexture);
	DDX_Control(pDX, IDC_EDIT_WATER_ENVMAP, m_EnvMapTexture);
	DDX_Control(pDX, IDC_SLIDER_WATER_SPEED, m_SpeedSlider);
	DDX_Control(pDX, IDC_SLIDER_WATER_UVREPEAT, m_UVRepeatSlider);
	DDX_Control(pDX, IDC_SLIDER_WATER_AMP, m_AmpSlider);
	DDX_Control(pDX, IDC_BTN_WATERCOLOR1, m_WaterColor1);
	DDX_Control(pDX, IDC_EDIT_WATERALPHA1, m_WaterAlpha1);
	DDX_Control(pDX, IDC_EDIT_WATERDEPTH1, m_WaterDepth1);
	DDX_Control(pDX, IDC_BTN_WATERCOLOR2, m_WaterColor2);
	DDX_Control(pDX, IDC_EDIT_WATERALPHA2, m_WaterAlpha2);
	DDX_Control(pDX, IDC_EDIT_WATERDEPTH2, m_WaterDepth2);
	DDX_Control(pDX, IDC_EDIT_WATERHEIGHT, m_WaterHeight);
	DDX_Control(pDX, IDC_CHECK_EDITBEACH, m_CheckBeach);
	DDX_Control(pDX, IDC_BEACHLIST, m_BeachList);
	DDX_Text(pDX, IDC_BUVSPEED, m_UVSpeed);
	DDX_Text(pDX, IDC_BEACHWIDTH, m_BeachWidth);
	DDX_Text(pDX, IDC_BEACHHEIGHT, m_BeachHeight);
	DDX_Text(pDX, IDC_BTNAME, m_BeachTexture);
	DDX_Text(pDX, IDC_UREPEAT, m_uRepeat);
	DDX_Text(pDX, IDC_VREPEAT, m_vRepeat);
	DDX_Text(pDX, IDC_BUVRANGE, m_fUVRange);
	DDX_Text(pDX, IDC_BEACHMASK, m_maskTextureUse);
}


BEGIN_MESSAGE_MAP(CWaterEditDlg, CDialog)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_COMBO_WATERTYPE, &CWaterEditDlg::OnCbnSelchangeComboWatertype)
	ON_EN_CHANGE(IDC_EDIT_WATERHEIGHT, &CWaterEditDlg::OnEnChangeEditWaterheight)
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_WATER_ENVMAP, &CWaterEditDlg::OnBnClickedButtonWaterEnvmap)
	ON_CBN_SELCHANGE(IDC_COMBO_WATERTEXTURE, &CWaterEditDlg::OnCbnSelchangeComboWatertexture)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_WATER_SPEED, &CWaterEditDlg::OnNMReleasedcaptureSliderWaterSpeed)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_WATER_UVREPEAT, &CWaterEditDlg::OnNMReleasedcaptureSliderWaterUvrepeat)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_WATER_AMP, &CWaterEditDlg::OnNMReleasedcaptureSliderWaterAmp)
	ON_BN_CLICKED(IDC_BTN_WATERCOLOR1, &CWaterEditDlg::OnBnClickedBtnWatercolor1)
	ON_EN_CHANGE(IDC_EDIT_WATERALPHA1, &CWaterEditDlg::OnEnChangeEditWateralpha1)
	ON_EN_CHANGE(IDC_EDIT_WATERDEPTH1, &CWaterEditDlg::OnEnChangeEditWaterdepth1)
	ON_BN_CLICKED(IDC_BTN_WATERCOLOR2, &CWaterEditDlg::OnBnClickedBtnWatercolor2)
	ON_EN_CHANGE(IDC_EDIT_WATERALPHA2, &CWaterEditDlg::OnEnChangeEditWateralpha2)
	ON_EN_CHANGE(IDC_EDIT_WATERDEPTH2, &CWaterEditDlg::OnEnChangeEditWaterdepth2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_WATER_SPEED, &CWaterEditDlg::OnNMCustomdrawSliderWaterSpeed)
	ON_BN_CLICKED(IDC_CHECK_EDITBEACH, &CWaterEditDlg::OnBnClickedCheckEditbeach)
	ON_BN_CLICKED(IDC_BEACH_ADD, &CWaterEditDlg::OnBnClickedBeachAdd)
	ON_BN_CLICKED(IDC_BEACHDELETE, &CWaterEditDlg::OnBnClickedBeachdelete)
	ON_BN_CLICKED(IDC_BEACH_COPY, &CWaterEditDlg::OnBnClickedBeachCopy)
	ON_LBN_SELCHANGE(IDC_BEACHLIST, &CWaterEditDlg::OnLbnSelchangeBeachlist)
	ON_EN_CHANGE(IDC_BUVSPEED, &CWaterEditDlg::OnEnChangeBuvspeed)
	ON_EN_CHANGE(IDC_BEACHWIDTH, &CWaterEditDlg::OnEnChangeBeachwidth)
	ON_EN_CHANGE(IDC_BEACHHEIGHT, &CWaterEditDlg::OnEnChangeBeachheight)
	ON_BN_CLICKED(IDC_BEACHTEXTURE_USE, &CWaterEditDlg::OnBnClickedBeachtextureUse)
	ON_BN_CLICKED(IDC_BT_DELETE, &CWaterEditDlg::OnBnClickedBtDelete)
	ON_EN_CHANGE(IDC_UREPEAT, &CWaterEditDlg::OnEnChangeUrepeat)
	ON_EN_CHANGE(IDC_VREPEAT, &CWaterEditDlg::OnEnChangeVrepeat)
	ON_EN_CHANGE(IDC_BUVRANGE, &CWaterEditDlg::OnEnChangeBuvrange)
	ON_BN_CLICKED(IDC_BEACHMASK_USE, &CWaterEditDlg::OnBnClickedBeachmaskUse)
	ON_BN_CLICKED(IDC_BM_DELETE, &CWaterEditDlg::OnBnClickedBmDelete)
	ON_EN_CHANGE(IDC_BEACHMASK, &CWaterEditDlg::OnEnChangeBeachmask)
	ON_EN_CHANGE(IDC_BTNAME, &CWaterEditDlg::OnEnChangeBtname)
	ON_BN_CLICKED(IDC_RENAME, &CWaterEditDlg::OnBnClickedRename)
END_MESSAGE_MAP()


// CWaterEditDlg message handlers

int CWaterEditDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	return 0;
}

BOOL CWaterEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_WaterTypeSelect.AddString("动画贴图水面");
	m_WaterTypeSelect.AddString("反射水面");

	m_WaterHeight.SetWindowText("0");

	m_AnimTexture.AddString("slime");
	m_AnimTexture.AddString("magma");

	m_EnvMapTexture.SetWindowText("");

	m_SpeedSlider.SetRange(0, 32);
	m_SpeedSlider.SetPos(16);

	m_UVRepeatSlider.SetRange(1, 32);
	m_UVRepeatSlider.SetPos(8);

	m_AmpSlider.SetRange(0, 32);
	m_AmpSlider.SetPos(1);

	m_WaterColor1.EnableOtherButton("其他颜色");
	m_WaterColor2.EnableOtherButton("其他颜色");

	m_WaterColor1.SetColor(RGB(0, 25, 127));
	m_WaterColor2.SetColor(RGB(255, 25, 76));

	m_WaterAlpha1.SetWindowText("20");
	m_WaterAlpha2.SetWindowText("80");

	m_WaterDepth1.SetWindowText("100");
	m_WaterDepth2.SetWindowText("500");

	m_bInit = true;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

static float GetEditInt(CEdit &edit)
{
	CString str;
	edit.GetWindowText(str);

	int v = 0;
	sscanf(str.GetBuffer(), "%d", &v);
	return (float)v;
}

static void SetEditInt(CEdit &edit, float v)
{
	char buffer[256];
	sprintf(buffer, "%d", int(v));
	edit.SetWindowText(buffer);
}

static void ConvertColor(Ogre::ColourValue &dest, COLORREF rgb, float alpha)
{
	dest.r = GetRValue(rgb)/255.0f;
	dest.g = GetGValue(rgb)/255.0f;
	dest.b = GetBValue(rgb)/255.0f;
	dest.a = alpha/100.0f;
}

void CWaterEditDlg::UpdateToTerrainEdit()
{
	if(!m_bInit) 
		return;

	TE::EditOptionWater &water = g_TerrainEdit.m_EditOption.water;

	water.watertype = m_WaterTypeSelect.GetCurSel();
	water.waterheight = GetEditInt(m_WaterHeight);

	CString str;
	int index = m_AnimTexture.GetCurSel();
	if(index < 0)
		water.animtex[0] = 0;
	else
	{
		m_AnimTexture.GetLBText(index, str);
		strcpy(water.animtex, str.GetBuffer());
	}

	m_EnvMapTexture.GetWindowText(str);
	strcpy(water.envtex, str.GetBuffer());

	ConvertColor(water.watercolor[0], m_WaterColor1.GetColor(), GetEditInt(m_WaterAlpha1));
	ConvertColor(water.watercolor[1], m_WaterColor2.GetColor(), GetEditInt(m_WaterAlpha2));

	water.waterdepth[0] = GetEditInt(m_WaterDepth1);
	water.waterdepth[1] = GetEditInt(m_WaterDepth2);

	water.ntexrepeat = (float)m_UVRepeatSlider.GetPos();
	water.speed = 0.25f*m_SpeedSlider.GetPos();
	water.amp = 0.25f*m_AmpSlider.GetPos();

	g_TerrainEdit.UpdateLiqduiOption(water);
}

extern void SetColorToButton(CBCGPColorButton &btn, Ogre::ColourValue color);

void CWaterEditDlg::UpdateWaterData()
{
	if(!g_TerrainEdit.GetUpdateWaterData())
	{
		return;
	}
	m_bInit = false;
	TE::EditOptionWater &water = g_TerrainEdit.m_EditOption.water;

	m_WaterTypeSelect.SetCurSel(water.watertype);
	SetEditInt(m_WaterHeight, water.waterheight);

	m_AnimTexture.SelectString(-1, water.animtex);
	m_EnvMapTexture.SetWindowText(water.envtex);

	SetColorToButton(m_WaterColor1, water.watercolor[0]);
	SetColorToButton(m_WaterColor2, water.watercolor[1]);

	SetEditInt(m_WaterAlpha1, water.watercolor[0].a*100);
	SetEditInt(m_WaterAlpha2, water.watercolor[1].a*100);

	SetEditInt(m_WaterDepth1, water.waterdepth[0]);
	SetEditInt(m_WaterDepth2, water.waterdepth[1]);

	m_UVRepeatSlider.SetPos(int(water.ntexrepeat));
	m_SpeedSlider.SetPos(int(water.speed*4));
	m_AmpSlider.SetPos(int(water.amp*4));

	g_TerrainEdit.SetUpdateWaterData(false);
	m_bInit = true;
}

void CWaterEditDlg::OnCbnSelchangeComboWatertype()
{
	// TODO: Add your control notification handler code here
	UpdateToTerrainEdit();
}

void CWaterEditDlg::OnEnChangeEditWaterheight()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateToTerrainEdit();
}

void CWaterEditDlg::OnSetFocus(CWnd* pOldWnd)
{
	CDialog::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
	g_TerrainEdit.SetCurEditEvent( TE::MODIFY_LIQUID );
}

void CWaterEditDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
}

void CWaterEditDlg::OnBnClickedButtonWaterEnvmap()
{
	// TODO: Add your control notification handler code here
	const char *path = g_TerrainEdit.m_SelResData.respath.c_str();
	assert(strlen(path)<128);
	m_EnvMapTexture.SetWindowText(path);

	UpdateToTerrainEdit();
}

void CWaterEditDlg::OnCbnSelchangeComboWatertexture()
{
	// TODO: Add your control notification handler code here
	UpdateToTerrainEdit();
}

void CWaterEditDlg::OnNMReleasedcaptureSliderWaterSpeed(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	UpdateToTerrainEdit();
}

void CWaterEditDlg::OnNMReleasedcaptureSliderWaterUvrepeat(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	UpdateToTerrainEdit();
}

void CWaterEditDlg::OnNMReleasedcaptureSliderWaterAmp(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	UpdateToTerrainEdit();
}

void CWaterEditDlg::OnBnClickedBtnWatercolor1()
{
	// TODO: Add your control notification handler code here
	UpdateToTerrainEdit();
}

void CWaterEditDlg::OnEnChangeEditWateralpha1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateToTerrainEdit();
}

void CWaterEditDlg::OnEnChangeEditWaterdepth1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateToTerrainEdit();
}

void CWaterEditDlg::OnBnClickedBtnWatercolor2()
{
	// TODO: Add your control notification handler code here
	UpdateToTerrainEdit();
}

void CWaterEditDlg::OnEnChangeEditWateralpha2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateToTerrainEdit();
}

void CWaterEditDlg::OnEnChangeEditWaterdepth2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateToTerrainEdit();
}

void CWaterEditDlg::OnNMCustomdrawSliderWaterSpeed(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CWaterEditDlg::OnBnClickedCheckEditbeach()
{
	// TODO: Add your control notification handler code here
	//if (m_CheckBeach.GetCheck())
	//	g_TerrainEdit.SetCurEditEvent(TE::MODIFY_BEACH);
	//else
	//	g_TerrainEdit.SetCurEditEvent(TE::MODIFY_LIQUID);
}

void CWaterEditDlg::OnBnClickedBeachAdd()
{
	// TODO: Add your control notification handler code here
	//Ogre::Beach *beach = new Ogre::Beach();

	//g_TerrainEdit.addBeach(beach);

	//int index = m_BeachList.AddString("NewBeach");
	//m_BeachList.SetItemData(index, (DWORD_PTR)beach);
}

void CWaterEditDlg::OnBnClickedBeachdelete()
{
	// TODO: Add your control notification handler code here

	//int index = m_BeachList.GetCurSel();
	//Ogre::Beach* pBeach = (Ogre::Beach*)m_BeachList.GetItemData(index);

	//g_TerrainEdit.removeBeach(pBeach);
}

void CWaterEditDlg::OnBnClickedBeachCopy()
{
	// TODO: Add your control notification handler code here
	int index = m_BeachList.GetCurSel();
	Ogre::Beach* pSelectedBeach = (Ogre::Beach*)m_BeachList.GetItemData(index);

	//if (pSelectedBeach)
	//{
	//	Ogre::Beach *pBeach = new Ogre::Beach;
	//	pBeach->m_RoadPoints
	//}
}

void CWaterEditDlg::OnLbnSelchangeBeachlist()
{
	// TODO: Add your control notification handler code here

	//int index = m_BeachList.GetCurSel();
	//Ogre::Beach* pBeach = (Ogre::Beach*)m_BeachList.GetItemData(index);	

	//g_TerrainEdit.setCurrentEditBeach(pBeach);
}

void CWaterEditDlg::OnEnChangeBuvspeed()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	//UpdateData();

	//Ogre::Beach *pBeach = g_TerrainEdit.getCurrentEditBeach();

	//if (pBeach)
	//{
	//	pBeach->setUVSpeed(m_UVSpeed);
	//}
}

void CWaterEditDlg::OnEnChangeBeachwidth()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	//UpdateData();

	//Ogre::Beach *pBeach = g_TerrainEdit.getCurrentEditBeach();

	//if (pBeach)
	//{
	//	pBeach->setWidth(m_BeachWidth);
	//}
}

void CWaterEditDlg::OnEnChangeBeachheight()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	//UpdateData();

	//Ogre::Beach *pBeach = g_TerrainEdit.getCurrentEditBeach();

	//if (pBeach)
	//{
	//	pBeach->setHeight(m_BeachHeight);
	//}
}

void CWaterEditDlg::OnBnClickedBeachtextureUse()
{
	// TODO: Add your control notification handler code here

	//if(g_TerrainEdit.m_SelResData.seltype != TE::SELECT_RES_TEXTURE) 
	//	return;

	//const char *path = g_TerrainEdit.m_SelResData.respath.c_str();

	//assert(strlen(path)<128);

	//m_BeachTexture = path;

	//UpdateData(false);

	//Ogre::Beach *pBeach = g_TerrainEdit.getCurrentEditBeach();

	//if (pBeach)
	//{
	//	pBeach->setTextureRes(path);
	//}
}

void CWaterEditDlg::OnBnClickedBtDelete()
{
	// TODO: Add your control notification handler code here
	//Ogre::Beach *pBeach = g_TerrainEdit.getCurrentEditBeach();

	//if (pBeach)
	//{
	//	pBeach->setTextureRes("");
	//}

	//m_BeachTexture = "";

	//UpdateData(false);
}

void CWaterEditDlg::OnEnChangeUrepeat()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	//UpdateData();

	//Ogre::Beach *pBeach = g_TerrainEdit.getCurrentEditBeach();

	//if (pBeach)
	//{
	//	pBeach->setUVXRepeat(m_uRepeat);
	//}
}

void CWaterEditDlg::OnEnChangeVrepeat()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	//UpdateData();

	//Ogre::Beach *pBeach = g_TerrainEdit.getCurrentEditBeach();

	//if (pBeach)
	//{
	//	pBeach->setUVYrepeat(m_vRepeat);
	//}
}

void CWaterEditDlg::OnEnChangeBuvrange()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	//UpdateData();

	//Ogre::Beach *pBeach = g_TerrainEdit.getCurrentEditBeach();

	//if (pBeach)
	//{
	//	pBeach->setVRange(m_fUVRange);
	//}
}

void CWaterEditDlg::OnBnClickedBeachmaskUse()
{
	// TODO: Add your control notification handler code here

	//if(g_TerrainEdit.m_SelResData.seltype != TE::SELECT_RES_TEXTURE) 
	//	return;

	//const char *path = g_TerrainEdit.m_SelResData.respath.c_str();

	//assert(strlen(path)<128);

	//m_maskTextureUse = path;

	//UpdateData(false);

	//Ogre::Beach *pBeach = g_TerrainEdit.getCurrentEditBeach();

	//if (pBeach)
	//{
	//	pBeach->setMaskTextureRes(path);
	//}
}

void CWaterEditDlg::OnBnClickedBmDelete()
{
	// TODO: Add your control notification handler code here
	//Ogre::Beach *pBeach = g_TerrainEdit.getCurrentEditBeach();

	//if (pBeach)
	//{
	//	pBeach->setMaskTextureRes("");
	//}

	//m_maskTextureUse = "";

	//UpdateData(false);
}

void CWaterEditDlg::OnEnChangeBeachmask()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CWaterEditDlg::OnEnChangeBtname()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CWaterEditDlg::OnBnClickedRename()
{
	// TODO: Add your control notification handler code here

	//if (1 == m_BeachNameDlg.DoModal())
	//{
	//	int selectIndex = m_BeachList.GetCurSel();
	//}
}
