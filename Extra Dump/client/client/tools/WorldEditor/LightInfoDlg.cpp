// LightInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "LightInfoDlg.h"
#include "TerrainEdit.h"
#include "MapLights.h"


using namespace TE;

// CLightInfoDlg dialog

IMPLEMENT_DYNAMIC(CLightInfoDlg, CDialog)

CLightInfoDlg::CLightInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLightInfoDlg::IDD, pParent)
{

}

CLightInfoDlg::~CLightInfoDlg()
{
}

void CLightInfoDlg::InitData()
{
	if( !g_TerrainEdit.m_EditOption.pSelectModel )
	{
		return;
	}

	LightInfo lightInfo;
	if( !g_LightManager.GetCurSelectLightInfo( lightInfo ) )
	{
		return;
	}

	SetDlgItemInt(IDC_EDIT_LIGHT_X, (int)lightInfo.pos.x);
	SetDlgItemInt(IDC_EDIT_LIGHT_Y, (int)lightInfo.pos.y);
	SetDlgItemInt(IDC_EDIT_LIGHT_Z, (int)lightInfo.pos.z);
	SetDlgItemInt(IDC_EDIT_LIGHT_RANGE, (int)lightInfo.fRange);

	int r = (int)(lightInfo.color.r*255);
	int g = (int)(lightInfo.color.g*255);
	int b = (int)(lightInfo.color.b*255);
	m_LightColorBtn.SetColor(RGB(r,g,b));
}

void CLightInfoDlg::UpdatePos()
{
	Ogre::Vector3 pos;
	pos.x = GetDlgItemInt(IDC_EDIT_LIGHT_X);
	pos.y = GetDlgItemInt(IDC_EDIT_LIGHT_Y);
	pos.z = GetDlgItemInt(IDC_EDIT_LIGHT_Z);

	g_LightManager.SetCurSelectLightPos(pos);
}

void CLightInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_LIGHT_COLOR, m_LightColorBtn);
	DDX_Control(pDX, IDC_COMBO_LIGHT_TYPE, m_LightTypeCombo);
}


BEGIN_MESSAGE_MAP(CLightInfoDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT_LIGHT_X, &CLightInfoDlg::OnEnChangeEditLightX)
	ON_EN_CHANGE(IDC_EDIT_LIGHT_Y, &CLightInfoDlg::OnEnChangeEditLightY)
	ON_EN_CHANGE(IDC_EDIT_LIGHT_Z, &CLightInfoDlg::OnEnChangeEditLightZ)
	ON_BN_CLICKED(IDC_BUTTON_LIGHT_COLOR, &CLightInfoDlg::OnBnClickedButtonLightColor)
	ON_EN_CHANGE(IDC_EDIT_LIGHT_RANGE, &CLightInfoDlg::OnEnChangeEditLightRange)
	ON_CBN_SELCHANGE(IDC_COMBO_LIGHT_TYPE, &CLightInfoDlg::OnCbnSelchangeComboLightType)
END_MESSAGE_MAP()


// CLightInfoDlg message handlers

void CLightInfoDlg::OnEnChangeEditLightX()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdatePos();
}

void CLightInfoDlg::OnEnChangeEditLightY()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdatePos();
}

void CLightInfoDlg::OnEnChangeEditLightZ()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdatePos();
}

void CLightInfoDlg::OnBnClickedButtonLightColor()
{
	// TODO: Add your control notification handler code here
	COLORREF c = m_LightColorBtn.GetColor();
	unsigned int r = GetRValue(c);
	unsigned int g = GetGValue(c);
	unsigned int b = GetBValue(c);

	Ogre::ColourValue color(r/255.0f, g/255.0f, b/255.0f, 1.0f);
	g_LightManager.SetCurSelectLightColor(color);
}

void CLightInfoDlg::OnEnChangeEditLightRange()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	float range = (float)GetDlgItemInt(IDC_EDIT_LIGHT_RANGE);
	g_LightManager.SetCurSelectLightRange(range);
}

void CLightInfoDlg::OnCbnSelchangeComboLightType()
{
	// TODO: Add your control notification handler code here
}

BOOL CLightInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_LightColorBtn.EnableOtherButton("其他颜色");
	m_LightTypeCombo.AddString("静态灯光");
	m_LightTypeCombo.AddString("动态灯光");

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
