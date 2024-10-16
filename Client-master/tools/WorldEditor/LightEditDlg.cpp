// LightEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "LightEditDlg.h"
#include "EditEvent_ObjectBrush.h"
#include "EditEventHandlerMgr.h"

#include "MapLights.h"
#include "TerrainEdit.h"

using namespace TE;
// CLightEditDlg dialog

extern Ogre::ColourValue GetColorFromButton(CBCGPColorButton &btn);
extern void SetColorToButton(CBCGPColorButton &btn, Ogre::ColourValue color);

static void SetEditControlFloat(CEdit &edit, float x)
{
	char buffer[256];
	sprintf(buffer, "%.1f", x);
	edit.SetWindowText(buffer);
}

static float GetEditControlFloat(CEdit &edit)
{
	char buffer[256];
	edit.GetWindowText(buffer, 255);
	return (float)atof(buffer);
}

static std::string GetEditControlStr(CEdit &edit)
{
	CString str;
	edit.GetWindowText(str);
	return std::string(str.GetBuffer());
}


IMPLEMENT_DYNAMIC(CLightEditDlg, CDialog)

CLightEditDlg::CLightEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLightEditDlg::IDD, pParent)
{

}

CLightEditDlg::~CLightEditDlg()
{
}

void CLightEditDlg::OnSelectLight(int sel)
{
	m_LightList.SetCurSel(sel);
	setLightSelectedInList(sel);
}

void CLightEditDlg::OnDeleteLight(int sel)
{
	m_LightList.DeleteString(sel);
	m_LightList.SetSel(-1);
}

void CLightEditDlg::UpdateLightList()
{
	m_LightList.ResetContent();

	for(size_t i=0 ; i<g_LightManager.m_LightInfoSet.size(); i++)
	{
		CString itemtext;
		LightInfo &info = g_LightManager.m_LightInfoSet[i];
		if(info.nType == 0) itemtext = "点光源：";
		else itemtext = "声音：";

		itemtext += info.name.c_str();
		m_LightList.AddString(itemtext);
	}
}

void CLightEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LIGHTS, m_LightList);
	DDX_Control(pDX, IDC_CHECK_RENDER_LIGHTS, m_RenderLights);
	DDX_Control(pDX, IDC_CHECK_RENDER_LIGHTMODELS, m_RenderLightModels);
	DDX_Control(pDX, IDC_CHECK_LIGHT_LOCKHEIGHT, m_LockHeight);
	DDX_Control(pDX, IDC_COMBO_EFFECT_EDITMODE, m_EffectEditMode);
	DDX_Control(pDX, IDC_BUTTON_EFFECT_COLOR, m_LightColorBtn);
	DDX_Control(pDX, IDC_EDIT_EFFECT_X, m_EffectPosX);
	DDX_Control(pDX, IDC_EDIT_EFFECT_Y, m_EffectPosY);
	DDX_Control(pDX, IDC_EDIT_EFFECT_Z, m_EffectPosZ);
	DDX_Control(pDX, IDC_EDIT_EFFECT_RANGE, m_EffectRange);
	DDX_Control(pDX, IDC_EDIT_SOUND_VOLUME, m_EffectVolume);
	DDX_Control(pDX, IDC_EDIT_EFFECT_NAME, m_EffectName);
	DDX_Control(pDX, IDC_EDIT_EFFECT_RESPATH, m_EffectResPath);
	DDX_Control(pDX, IDC_SOUNDTIME0, m_SoundTime0);
	DDX_Control(pDX, IDC_SOUNDTIME1, m_SoundTime1);
	DDX_Control(pDX, IDC_FULLRANGE, mSoundFullRange);
	DDX_Control(pDX, IDC_SOUNDISLOOP, m_CheckIsLoop);
}


BEGIN_MESSAGE_MAP(CLightEditDlg, CDialog)
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDC_BUTTON_LOADLIGHTS, &CLightEditDlg::OnBnClickedButtonLoadlights)
	ON_BN_CLICKED(IDC_CHECK_LIGHT_LOCKHEIGHT, &CLightEditDlg::OnBnClickedCheckLightLockheight)
	ON_BN_CLICKED(IDC_CHECK_RENDER_LIGHTMODELS, &CLightEditDlg::OnBnClickedCheckRenderLightmodels)
	ON_BN_CLICKED(IDC_CHECK_RENDER_LIGHTS, &CLightEditDlg::OnBnClickedCheckRenderLights)
	ON_CBN_SELCHANGE(IDC_COMBO_EFFECT_EDITMODE, &CLightEditDlg::OnCbnSelchangeComboEffectEditmode)
	ON_LBN_SELCHANGE(IDC_LIST_LIGHTS, &CLightEditDlg::OnLbnSelchangeListLights)
	ON_BN_CLICKED(IDC_BUTTON_EFFECT_PATH, &CLightEditDlg::OnBnClickedButtonEffectPath)
	ON_BN_CLICKED(IDC_BUTTON_EFFECT_COLOR, &CLightEditDlg::OnBnClickedButtonEffectColor)
	ON_EN_CHANGE(IDC_EDIT_EFFECT_RANGE, &CLightEditDlg::OnEnChangeEditEffectRange)
	ON_EN_CHANGE(IDC_EDIT_SOUND_VOLUME, &CLightEditDlg::OnEnChangeEditSoundVolume)
	ON_EN_CHANGE(IDC_EDIT_EFFECT_NAME, &CLightEditDlg::OnEnChangeEditEffectName)
	ON_EN_CHANGE(IDC_EDIT_EFFECT_RESPATH, &CLightEditDlg::OnEnChangeEditEffectRespath)
	ON_EN_CHANGE(IDC_EDIT_EFFECT_X, &CLightEditDlg::OnEnChangeEditEffectX)
	ON_EN_CHANGE(IDC_SOUNDTIME0, &CLightEditDlg::OnEnChangeSoundtime0)
	ON_EN_CHANGE(IDC_SOUNDTIME1, &CLightEditDlg::OnEnChangeSoundtime1)
	ON_EN_CHANGE(IDC_FULLRANGE, &CLightEditDlg::OnEnChangeFullrange)
	ON_BN_CLICKED(IDC_SOUNDISLOOP, &CLightEditDlg::OnBnClickedSoundisloop)
END_MESSAGE_MAP()


// CLightEditDlg message handlers

BOOL CLightEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_RenderLights.SetCheck(1);
	m_RenderLightModels.SetCheck(1);
	m_LockHeight.SetCheck(0);

	m_EffectEditMode.AddString("添加灯光");
	m_EffectEditMode.AddString("添加音效");
	m_EffectEditMode.SetCurSel(0);

	m_LightColorBtn.EnableOtherButton("其他颜色");
	m_LightColorBtn.SetColor(RGB(255, 255, 0));

	m_LightList.ResetContent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLightEditDlg::OnSetFocus(CWnd* pOldWnd)
{
	CDialog::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
	//g_TerrainEdit.SetCurEditEvent(TE::LIGHT_EDIT_EVN);
	g_TerrainEdit.SetCurEditEvent(TE::EDITEVENT_OBJECTBRUSH);
	ObjectBrushEditHandler *phandler = static_cast<ObjectBrushEditHandler *>(g_EventProcessFactory.GetEventProcess(EDITEVENT_OBJECTBRUSH));
	phandler->setEditObjectType(TE::EDITOBJ_LIGHT);
	phandler->setEditOP(TE::OBJEDIT_OP_ADD);
	UpdateLightList();
}

void CLightEditDlg::OnBnClickedButtonLoadlights()
{
	// TODO: Add your control notification handler code here
	g_TerrainEdit.LoadCurMapLightInfo();
}

void CLightEditDlg::OnBnClickedCheckLightLockheight()
{
	// TODO: Add your control notification handler code here
	int check = m_LockHeight.GetCheck();
	g_LightManager.SetLockHeight(check>0);
}

void CLightEditDlg::OnBnClickedCheckRenderLightmodels()
{
	// TODO: Add your control notification handler code here
	int check = m_RenderLightModels.GetCheck();
	if(check)
		g_LightManager.AttachLightModelToRoom();
	else 
		g_LightManager.DetachLightModelToRoom();
}

void CLightEditDlg::OnBnClickedCheckRenderLights()
{
	// TODO: Add your control notification handler code here
	int check = m_RenderLights.GetCheck();
	if(check) g_LightManager.AttachLightToRoom();
	else g_LightManager.DetachLightToRoom();
}

void CLightEditDlg::OnCbnSelchangeComboEffectEditmode()
{
	// TODO: Add your control notification handler code here
	int mode = m_EffectEditMode.GetCurSel();

	ObjectBrushEditHandler *phandler = static_cast<ObjectBrushEditHandler *>(g_EventProcessFactory.GetEventProcess(TE::EDITEVENT_OBJECTBRUSH));
	phandler->setEditOP(TE::OBJEDIT_OP_ADD);
	if(mode == 0) phandler->setEditObjectType(TE::EDITOBJ_LIGHT);
	else phandler->setEditObjectType(TE::EDITOBJ_SOUND);
}

void CLightEditDlg::setLightSelectedInList(int sel)
{
	if(sel < 0) return;

	LightInfo &lightinfo = g_LightManager.m_LightInfoSet[sel];
	m_EffectName.SetWindowText(lightinfo.name.c_str());
	m_EffectResPath.SetWindowText(lightinfo.respath.c_str());
	SetEditControlFloat(m_EffectPosX, lightinfo.pos.x);
	SetEditControlFloat(m_EffectPosY, lightinfo.pos.y);
	SetEditControlFloat(m_EffectPosZ, lightinfo.pos.z);
	SetEditControlFloat(m_EffectRange, lightinfo.fRange);
	SetEditControlFloat(m_EffectVolume, lightinfo.volume);
	SetColorToButton(m_LightColorBtn, lightinfo.color);
	SetEditControlFloat(m_SoundTime0, lightinfo.time0);
	SetEditControlFloat(m_SoundTime1, lightinfo.time1);
	SetEditControlFloat(mSoundFullRange, lightinfo.fSoundFullRange);
	m_CheckIsLoop.SetCheck(lightinfo.iIsLoop);
}

void CLightEditDlg::OnLbnSelchangeListLights()
{
	// TODO: Add your control notification handler code here
	int sel = m_LightList.GetCurSel();
	setLightSelectedInList(sel);

	g_TerrainEdit.SelectMapLight(sel);
	g_TerrainEdit.cameraOnSelectModel();
}

void CLightEditDlg::OnBnClickedButtonEffectPath()
{
	// TODO: Add your control notification handler code here
	if(g_TerrainEdit.m_SelResData.seltype != TE::SELECT_RES_SOUND) return;

	const char *path = g_TerrainEdit.m_SelResData.respath.c_str();
	m_EffectResPath.SetWindowText(path);
}

void CLightEditDlg::OnBnClickedButtonEffectColor()
{
	// TODO: Add your control notification handler code here
	int sel = m_LightList.GetCurSel();
	if(sel < 0) return;

	LightInfo &lightinfo = g_LightManager.getLightInfo(sel);
	lightinfo.color = GetColorFromButton(m_LightColorBtn);

	g_LightManager.updateNodeInfo(sel);
}

void CLightEditDlg::OnEnChangeEditEffectRange()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	int sel = m_LightList.GetCurSel();
	if(sel < 0) return;

	LightInfo &lightinfo = g_LightManager.getLightInfo(sel);
	lightinfo.fRange = GetEditControlFloat(m_EffectRange);

	g_LightManager.updateNodeInfo(sel);
}

void CLightEditDlg::OnEnChangeEditSoundVolume()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	int sel = m_LightList.GetCurSel();
	if(sel < 0) return;

	LightInfo &lightinfo = g_LightManager.getLightInfo(sel);
	lightinfo.volume = GetEditControlFloat(m_EffectVolume);
	g_LightManager.updateNodeInfo(sel);
}

void CLightEditDlg::OnEnChangeEditEffectName()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	int sel = m_LightList.GetCurSel();
	if(sel < 0) return;

	LightInfo &lightinfo = g_LightManager.getLightInfo(sel);
	lightinfo.name = GetEditControlStr(m_EffectName);

	CString itemtext;
	if(lightinfo.nType == 0) itemtext = "点光源：";
	else itemtext = "声音：";
	itemtext += lightinfo.name.c_str();
	m_LightList.InsertString(sel, itemtext);
	m_LightList.DeleteString(sel+1);

	m_LightList.SetCurSel(sel);
}

void CLightEditDlg::OnEnChangeEditEffectRespath()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	int sel = m_LightList.GetCurSel();
	if(sel < 0) return;

	LightInfo &lightinfo = g_LightManager.getLightInfo(sel);
	lightinfo.respath = GetEditControlStr(m_EffectResPath);

	g_LightManager.updateNodeInfo(sel);
}

void CLightEditDlg::OnEnChangeEditEffectX()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CLightEditDlg::OnEnChangeSoundtime0()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	int sel = m_LightList.GetCurSel();
	if(sel < 0) return;

	LightInfo &lightinfo = g_LightManager.getLightInfo(sel);
	lightinfo.time0 = GetEditControlFloat(m_SoundTime0);

	g_LightManager.updateNodeInfo(sel);
}

void CLightEditDlg::OnEnChangeSoundtime1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	int sel = m_LightList.GetCurSel();
	if(sel < 0) return;

	LightInfo &lightinfo = g_LightManager.getLightInfo(sel);
	lightinfo.time1 = GetEditControlFloat(m_SoundTime1);

	g_LightManager.updateNodeInfo(sel);
}

void CLightEditDlg::OnEnChangeFullrange()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	int sel = m_LightList.GetCurSel();
	if(sel < 0) return;

	LightInfo &lightinfo = g_LightManager.getLightInfo(sel);
	lightinfo.fSoundFullRange = GetEditControlFloat(mSoundFullRange);

	g_LightManager.updateNodeInfo(sel);
}

void CLightEditDlg::OnBnClickedSoundisloop()
{
	// TODO: Add your control notification handler code here

	int sel = m_LightList.GetCurSel();
	if(sel < 0) return;

	LightInfo &lightinfo = g_LightManager.getLightInfo(sel);
	lightinfo.iIsLoop = m_CheckIsLoop.GetCheck();

	g_LightManager.updateNodeInfo(sel);
}
