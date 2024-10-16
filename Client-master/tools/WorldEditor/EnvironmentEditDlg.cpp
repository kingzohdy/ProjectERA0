// EnvironmentEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "EnvironmentEditDlg.h"
#include "TerrainEdit.h"
#include "Terrain.h"
#include "EnvRegion.h"
#include "ogrebloom.h"

using namespace Ogre;
using namespace TE;
// CEnvironmentEditDlg dialog

IMPLEMENT_DYNAMIC(CEnvironmentEditDlg, CDialog)

ColourValue GetColorFromButton(CBCGPColorButton &btn)
{
	COLORREF c = btn.GetColor();

	return ColourValue(GetRValue(c)/255.0f, GetGValue(c)/255.0f, GetBValue(c)/255.0f, 1.0f);
}

void SetColorToButton(CBCGPColorButton &btn, ColourValue color)
{
	int r = int(color.r*255);
	int g = int(color.g*255);
	int b = int(color.b*255);

	btn.SetColor(RGB(r,g,b));
}

CEnvironmentEditDlg::CEnvironmentEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEnvironmentEditDlg::IDD, pParent)
{
	m_bInit = false;
}

CEnvironmentEditDlg::~CEnvironmentEditDlg()
{
}

void CEnvironmentEditDlg::GetDataFromUI()
{
	if(!m_bInit) return;
	m_bInit = false;

	MapArea *parea = g_AreaSet.GetCurSelectArea();
	Ogre::AmbientInfo &info = *parea->GetAmbientInfo();

	int angle1 = m_DirHorizonSlider.GetPos();
	int angle2 = m_DirVerticalSlider.GetPos();
	info.dirlight_angle = Vector2((float)angle1, (float)angle2);

	info.dirlight_color = GetColorFromButton(m_DiffuseColor);
	info.specular_color = GetColorFromButton(m_SpecularColor);
	info.ambient_color = GetColorFromButton(m_AmbientColorBtn);
	info.specular_power = 10.0f;

	info.toplight_color = GetColorFromButton(m_TopLightColor);

	info.distfog_color = GetColorFromButton(m_DistFogColor);
	info.distfog_enable = m_DistFogEnable.GetCheck()!=0;
	info.distfog_near = m_DistFogNear.GetPos();
	info.distfog_far = m_DistFogFar.GetPos()+m_DistFogNear.GetPos();

	info.heightfog_color = GetColorFromButton(m_HeightFogColor);
	info.heightfog_enable = m_HeightFogEnable.GetCheck()!=0;
	info.heightfog_near = m_HeightFogNear.GetPos();
	info.heightfog_far = m_HeightFogFar.GetPos()+m_HeightFogNear.GetPos();
	info.shaow			=	m_ShadowEnable.GetCheck()!=0;
	info.shadowdensity = m_Shadowdensity.GetPos()/100.0f;

	if(m_ToplightShadowType.GetCurSel() == 0) info.toplight_shadowbits = 0xffffffff;
	else info.toplight_shadowbits = 2;
	
	info.bloom_gray = m_BloomExposure.GetPos()/1000.0f;
	info.bloom_high = m_Bloomamp.GetPos()/1000.0f;
	info.bloom_blur = m_Bloomwidth.GetPos()/1000.0f;

	CString str;
	m_MusicVolEdit.GetWindowText(str);
	info.musicvol = atof(str.GetBuffer());
	m_BGSoundVolEdit.GetWindowText(str);
	info.bgsoundvol = atof(str.GetBuffer());

	GetDlgItemText(IDC_EDIT_SKYMODEL, info.skyfile, sizeof(info.skyfile));
	info.skyfile[sizeof(info.skyfile)-1] = 0;
	GetDlgItemText(IDC_EDIT_BGMUSIC, info.musicfile, sizeof(info.musicfile));
	info.musicfile[sizeof(info.musicfile)-1] = 0;
	GetDlgItemText(IDC_EDIT_BGSOUND, info.bgsoundfile, sizeof(info.bgsoundfile));
	info.bgsoundfile[sizeof(info.bgsoundfile)-1] = 0;
	GetDlgItemText(IDC_EDIT_CAMERAEFFECT, info.effectfile, sizeof(info.effectfile));
	info.effectfile[sizeof(info.effectfile)-1] = 0;

	g_Terrain.m_pAmbientMgr->setCurAmbient(info, true);
	m_bInit = true;
}

void CEnvironmentEditDlg::SetDataToUI()
{
	if(!m_bInit) return;
	m_bInit = false;

	TE::MapArea *parea = g_AreaSet.GetCurSelectArea();
	Ogre::AmbientInfo &info = *parea->GetAmbientInfo();

	m_DirHorizonSlider.SetPos((int)info.dirlight_angle.x);
	m_DirVerticalSlider.SetPos((int)info.dirlight_angle.y);
	SetDlgItemInt(IDC_EDIT_DIR_HORIZON, (int)info.dirlight_angle.x);
	SetDlgItemInt(IDC_EDIT_DIR_VERTICAL, (int)info.dirlight_angle.y);

	SetColorToButton(m_DiffuseColor, info.dirlight_color);
	SetColorToButton(m_SpecularColor, info.specular_color);
	SetColorToButton(m_AmbientColorBtn, info.ambient_color);
	SetColorToButton(m_TopLightColor, info.toplight_color);

	SetColorToButton(m_DistFogColor, info.distfog_color);
	m_DistFogEnable.SetCheck(info.distfog_enable);
	m_DistFogNear.SetPos((int)info.distfog_near);
	m_DistFogFar.SetPos(int(info.distfog_far-info.distfog_near));

	SetColorToButton(m_HeightFogColor, info.heightfog_color);
	m_HeightFogEnable.SetCheck(info.heightfog_enable);
	m_HeightFogNear.SetPos((int)info.heightfog_near);
	m_HeightFogFar.SetPos(int(info.heightfog_far-info.heightfog_near));

	m_ShadowEnable.SetCheck(info.shaow);
	m_Shadowdensity.SetPos(int(info.shadowdensity*100));

	if(info.toplight_shadowbits == 2) m_ToplightShadowType.SetCurSel(1);
	else m_ToplightShadowType.SetCurSel(0);

	m_BloomExposure.SetPos(int(info.bloom_gray*1000));
	m_Bloomamp.SetPos(int(info.bloom_high*1000));
	m_Bloomwidth.SetPos(int(info.bloom_blur*1000));

	char buffer[256];
	sprintf(buffer, "%.2f", info.musicvol);
	m_MusicVolEdit.SetWindowText(buffer);
	sprintf(buffer, "%.2f", info.bgsoundvol);
	m_BGSoundVolEdit.SetWindowText(buffer);

	SetDlgItemText(IDC_EDIT_SKYMODEL, info.skyfile);
	SetDlgItemText(IDC_EDIT_BGMUSIC, info.musicfile);
	SetDlgItemText(IDC_EDIT_BGSOUND, info.bgsoundfile);
	SetDlgItemText(IDC_EDIT_CAMERAEFFECT, info.effectfile);

	g_Terrain.m_pAmbientMgr->setCurAmbient(info, true);
	m_bInit = true;
}

void CEnvironmentEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_DIFFUSE_COLOR, m_DiffuseColor);
	DDX_Control(pDX, IDC_BTN_SPECULAR_COLOR, m_SpecularColor);
	DDX_Control(pDX, IDC_BTN_AMBIENT_COLOR, m_AmbientColorBtn);
	DDX_Control(pDX, IDC_CHECK_DISTFOG_ENABLE, m_DistFogEnable);
	DDX_Control(pDX, IDC_SLIDER_DISTFOG_NEAR, m_DistFogNear);
	DDX_Control(pDX, IDC_SLIDER_DISTFOG_FAR, m_DistFogFar);
	DDX_Control(pDX, IDC_BTN_DISTFOG_COLOR, m_DistFogColor);
	DDX_Control(pDX, IDC_CHECK_HEIGHTFOG_ENABLE, m_HeightFogEnable);
	DDX_Control(pDX, IDC_SLIDER_HEIGHTFOG_FAR, m_HeightFogFar);
	DDX_Control(pDX, IDC_SLIDER_HEIGHTFOG_NEAR, m_HeightFogNear);
	DDX_Control(pDX, IDC_BTN_HEIGHTFOG_COLOR, m_HeightFogColor);
	DDX_Control(pDX, IDC_CHECK_BIG_LOCK, m_ShadowEnable);
	DDX_Control(pDX, IDC_SLIDER_DIR_HORIZON, m_DirHorizonSlider);
	DDX_Control(pDX, IDC_SLIDER_DIR_VERTICAL, m_DirVerticalSlider);
	DDX_Control(pDX, IDC_SLIDER_SHADOWDENSITY, m_Shadowdensity);
	DDX_Control(pDX, IDC_SLIDER_BLOOM_EXPOSURE, m_BloomExposure);
	DDX_Control(pDX, IDC_SLIDER_BLOOM_AMP, m_Bloomamp);
	DDX_Control(pDX, IDC_SLIDER_BLOOM_WIDTH, m_Bloomwidth);

	DDX_Control(pDX, IDC_BTN_TOPLIGHT_COLOR, m_TopLightColor);
	DDX_Control(pDX, IDC_COMBO_TOPLIGHT_SHADOW, m_ToplightShadowType);
	DDX_Control(pDX, IDC_EDIT_MUSICVOLUME, m_MusicVolEdit);
	DDX_Control(pDX, IDC_EDIT_BGSOUNDVOL, m_BGSoundVolEdit);
}


BEGIN_MESSAGE_MAP(CEnvironmentEditDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_AMBIENT_COLOR, &CEnvironmentEditDlg::OnBnClickedBtnAmbientColor)
	ON_BN_CLICKED(IDC_BTN_DIFFUSE_COLOR, &CEnvironmentEditDlg::OnBnClickedBtnDiffuseColor)
	ON_BN_CLICKED(IDC_BTN_SPECULAR_COLOR, &CEnvironmentEditDlg::OnBnClickedBtnSpecularColor)
	ON_BN_CLICKED(IDC_CHECK_DISTFOG_ENABLE, &CEnvironmentEditDlg::OnBnClickedCheckDistfogEnable)
	ON_BN_CLICKED(IDC_CHECK_HEIGHTFOG_ENABLE, &CEnvironmentEditDlg::OnBnClickedCheckHeightfogEnable)
	ON_BN_CLICKED(IDC_BTN_DISTFOG_COLOR, &CEnvironmentEditDlg::OnBnClickedBtnDistfogColor)
	ON_BN_CLICKED(IDC_CHECK1, &CEnvironmentEditDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK_BIG_LOCK, &CEnvironmentEditDlg::OnBnClickedCheckBigLock)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_DIR_HORIZON, &CEnvironmentEditDlg::OnNMReleasedcaptureSliderDirHorizon)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_DIR_VERTICAL, &CEnvironmentEditDlg::OnNMReleasedcaptureSliderDirVertical)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_DISTFOG_NEAR, &CEnvironmentEditDlg::OnNMReleasedcaptureSliderDistfogNear)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_DISTFOG_FAR, &CEnvironmentEditDlg::OnNMReleasedcaptureSliderDistfogFar)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_HEIGHTFOG_FAR, &CEnvironmentEditDlg::OnNMReleasedcaptureSliderHeightfogFar)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_HEIGHTFOG_NEAR, &CEnvironmentEditDlg::OnNMReleasedcaptureSliderHeightfogNear)
	ON_BN_CLICKED(IDC_BTN_HEIGHTFOG_COLOR, &CEnvironmentEditDlg::OnBnClickedBtnHeightfogColor)
	ON_BN_CLICKED(IDC_BTN_SKYMODEL, &CEnvironmentEditDlg::OnBnClickedBtnSkymodel)
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDC_BTN_BGMUSIC, &CEnvironmentEditDlg::OnBnClickedBtnBgmusic)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SHADOWDENSITY, &CEnvironmentEditDlg::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_BLOOM_EXPOSURE, &CEnvironmentEditDlg::OnNMCustomdrawSlider3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_BLOOM_AMP, &CEnvironmentEditDlg::OnNMCustomdrawSlider4)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_BLOOM_WIDTH, &CEnvironmentEditDlg::OnNMCustomdrawSlider5)
	ON_BN_CLICKED(IDC_BTN_CAMERAEFFECT, &CEnvironmentEditDlg::OnBnClickedBtnCameraeffect)
	ON_BN_CLICKED(IDC_BTN_TOPLIGHT_COLOR, &CEnvironmentEditDlg::OnBnClickedBtnToplightColor)
	ON_CBN_SELCHANGE(IDC_COMBO_TOPLIGHT_SHADOW, &CEnvironmentEditDlg::OnCbnSelchangeComboToplightShadow)
	ON_EN_CHANGE(IDC_EDIT_MUSICVOLUME, &CEnvironmentEditDlg::OnEnChangeEditMusicvolume)
	ON_BN_CLICKED(IDC_BTN_BGSOUND, &CEnvironmentEditDlg::OnBnClickedBtnBgsound)
	ON_EN_CHANGE(IDC_EDIT_BGSOUNDVOL, &CEnvironmentEditDlg::OnEnChangeEditBgsoundvol)
	ON_BN_CLICKED(IDC_BTN_SKYMODEL_CLEAR, &CEnvironmentEditDlg::OnBnClickedBtnSkymodelClear)
	ON_BN_CLICKED(IDC_BTN_BGMUSIC_CLEAR, &CEnvironmentEditDlg::OnBnClickedBtnBgmusicClear)
	ON_BN_CLICKED(IDC_BTN_BGSOUND_CLEAR, &CEnvironmentEditDlg::OnBnClickedBtnBgsoundClear)
	ON_BN_CLICKED(IDC_BTN_CAMERAEFFECT_CLEAR, &CEnvironmentEditDlg::OnBnClickedBtnCameraeffectClear)
	ON_BN_CLICKED(IDC_BTN_SAVE_AMBIENT, &CEnvironmentEditDlg::OnBnClickedBtnSaveAmbient)
	ON_BN_CLICKED(IDC_BTN_LOADAMBIENT, &CEnvironmentEditDlg::OnBnClickedBtnLoadambient)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_HEIGHTFOG_FAR, &CEnvironmentEditDlg::OnNMCustomdrawSliderHeightfogFar)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_GRASS, &CEnvironmentEditDlg::OnNMCustomdrawSliderGrass)
END_MESSAGE_MAP()


// CEnvironmentEditDlg message handlers

BOOL CEnvironmentEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_DirHorizonSlider.SetRange(0, 360);
	m_DirHorizonSlider.SetPos(0);
	m_DirHorizonSlider.SetPageSize(10);
	SetDlgItemInt(IDC_EDIT_DIR_HORIZON, 0);

	m_DirVerticalSlider.SetRange(0, 90);
	m_DirVerticalSlider.SetPos(0);
	m_DirVerticalSlider.SetPageSize(10);
	SetDlgItemInt(IDC_EDIT_DIR_VERTICAL, 0);

	m_DiffuseColor.EnableOtherButton("其他颜色");
	m_SpecularColor.EnableOtherButton("其他颜色");
	m_AmbientColorBtn.EnableOtherButton("其他颜色");
	m_TopLightColor.EnableOtherButton("其他颜色");
	m_DistFogColor.EnableOtherButton("其他颜色");
	m_HeightFogColor.EnableOtherButton("其他颜色");
	m_DistFogNear.SetRange(0, 5000);
	m_DistFogFar.SetRange(100, 50000);
	m_HeightFogNear.SetRange(-1000, 10000);
	m_HeightFogFar.SetRange(100, 10000);
	m_Shadowdensity.SetRange(0,100);
	m_Shadowdensity.SetPos(100);

	m_BloomExposure.SetRange(0,1000);
	m_BloomExposure.SetPos(300);
	m_Bloomamp.SetRange(0,2000);
	m_Bloomamp.SetPos(1000);
	m_Bloomwidth.SetRange(1000,3000);
	m_Bloomwidth.SetPos(1000);

	m_ToplightShadowType.AddString("全投影");
	m_ToplightShadowType.AddString("角色投影");

	m_bInit = true;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEnvironmentEditDlg::OnBnClickedBtnAmbientColor()
{
	// TODO: Add your control notification handler code here
	GetDataFromUI();
}

void CEnvironmentEditDlg::OnBnClickedBtnDiffuseColor()
{
	// TODO: Add your control notification handler code here
	GetDataFromUI();
}

void CEnvironmentEditDlg::OnBnClickedBtnSpecularColor()
{
	// TODO: Add your control notification handler code here
	GetDataFromUI();
}

void CEnvironmentEditDlg::OnBnClickedCheckDistfogEnable()
{
	// TODO: Add your control notification handler code here
	GetDataFromUI();
}

void CEnvironmentEditDlg::OnBnClickedCheckHeightfogEnable()
{
	// TODO: Add your control notification handler code here
	GetDataFromUI();
}

void CEnvironmentEditDlg::OnBnClickedBtnDistfogColor()
{
	// TODO: Add your control notification handler code here
	GetDataFromUI();
}

void CEnvironmentEditDlg::OnBnClickedCheck1()
{

}

void CEnvironmentEditDlg::OnBnClickedCheckBigLock()
{
	GetDataFromUI();
}

void CEnvironmentEditDlg::OnNMReleasedcaptureSliderDirHorizon(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int pos = m_DirHorizonSlider.GetPos();
	SetDlgItemInt(IDC_EDIT_DIR_HORIZON, pos);

	GetDataFromUI();
}

void CEnvironmentEditDlg::OnNMReleasedcaptureSliderDirVertical(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int pos = m_DirVerticalSlider.GetPos();
	SetDlgItemInt(IDC_EDIT_DIR_VERTICAL, pos);

	GetDataFromUI();
}

void CEnvironmentEditDlg::OnNMReleasedcaptureSliderDistfogNear(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	SetDlgItemInt(IDC_EDIT_DISTFOG_NEAR, m_DistFogNear.GetPos());

	GetDataFromUI();
}

void CEnvironmentEditDlg::OnNMReleasedcaptureSliderDistfogFar(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	SetDlgItemInt(IDC_EDIT_DISTFOG_FAR, m_DistFogFar.GetPos());

	GetDataFromUI();
}

void CEnvironmentEditDlg::OnNMReleasedcaptureSliderHeightfogFar(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	SetDlgItemInt(IDC_EDIT_HEIGHTFOG_FAR, m_HeightFogFar.GetPos());

	GetDataFromUI();
}

void CEnvironmentEditDlg::OnNMReleasedcaptureSliderHeightfogNear(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	SetDlgItemInt(IDC_EDIT_HEIGHTFOG_NEAR, m_HeightFogNear.GetPos());

	GetDataFromUI();
}

void CEnvironmentEditDlg::OnBnClickedBtnHeightfogColor()
{
	// TODO: Add your control notification handler code here
	GetDataFromUI();
}

void CEnvironmentEditDlg::OnBnClickedBtnSkymodel()
{
	// TODO: Add your control notification handler code here
	if(g_TerrainEdit.m_SelResData.seltype!=TE::SELECT_RES_MODEL) return;

	const char *path = g_TerrainEdit.m_SelResData.respath.c_str();
	assert(strlen(path)<128);
	SetDlgItemText(IDC_EDIT_SKYMODEL, path);

	GetDataFromUI();
}

void CEnvironmentEditDlg::OnSetFocus(CWnd* pOldWnd)
{
	CDialog::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
}

void CEnvironmentEditDlg::OnBnClickedBtnBgmusic()
{
	// TODO: Add your control notification handler code here
	if(g_TerrainEdit.m_SelResData.seltype != TE::SELECT_RES_SOUND) return;

	const char *path = g_TerrainEdit.m_SelResData.respath.c_str();
	assert(strlen(path)<128);
	SetDlgItemText(IDC_EDIT_BGMUSIC, path);

	GetDataFromUI();
}

void CEnvironmentEditDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	// TODO: Add your control notification handler code here
	*pResult = 0;
	GetDataFromUI();
}

void CEnvironmentEditDlg::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	GetDataFromUI();
}

void CEnvironmentEditDlg::OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	GetDataFromUI();
}

void CEnvironmentEditDlg::OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	GetDataFromUI();
}

void CEnvironmentEditDlg::OnBnClickedBtnCameraeffect()
{
	// TODO: Add your control notification handler code here
	if(g_TerrainEdit.m_SelResData.seltype != TE::SELECT_RES_MODEL) return;

	const char *path = g_TerrainEdit.m_SelResData.respath.c_str();
	assert(strlen(path)<128);
	SetDlgItemText(IDC_EDIT_CAMERAEFFECT, path);

	GetDataFromUI();
}

void CEnvironmentEditDlg::OnBnClickedBtnToplightColor()
{
	// TODO: Add your control notification handler code here
	GetDataFromUI();
}

void CEnvironmentEditDlg::OnCbnSelchangeComboToplightShadow()
{
	// TODO: Add your control notification handler code here
	GetDataFromUI();
}

void CEnvironmentEditDlg::OnEnChangeEditMusicvolume()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	GetDataFromUI();
}

void CEnvironmentEditDlg::OnBnClickedBtnBgsound()
{
	// TODO: Add your control notification handler code here
	if(g_TerrainEdit.m_SelResData.seltype != TE::SELECT_RES_SOUND) return;

	const char *path = g_TerrainEdit.m_SelResData.respath.c_str();
	assert(strlen(path)<128);
	SetDlgItemText(IDC_EDIT_BGSOUND, path);

	GetDataFromUI();
}

void CEnvironmentEditDlg::OnEnChangeEditBgsoundvol()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	GetDataFromUI();
}

void CEnvironmentEditDlg::OnBnClickedBtnSkymodelClear()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_SKYMODEL, "");
	GetDataFromUI();
}

void CEnvironmentEditDlg::OnBnClickedBtnBgmusicClear()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_BGMUSIC, "");
	GetDataFromUI();
}

void CEnvironmentEditDlg::OnBnClickedBtnBgsoundClear()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_BGSOUND, "");
	GetDataFromUI();
}

void CEnvironmentEditDlg::OnBnClickedBtnCameraeffectClear()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_CAMERAEFFECT, "");
	GetDataFromUI();
}

void CEnvironmentEditDlg::OnBnClickedBtnSaveAmbient()
{
	// TODO: Add your control notification handler code here
}

void CEnvironmentEditDlg::OnBnClickedBtnLoadambient()
{
	// TODO: Add your control notification handler code here
}

void CEnvironmentEditDlg::OnNMCustomdrawSliderHeightfogFar(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CEnvironmentEditDlg::OnNMCustomdrawSliderGrass(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
