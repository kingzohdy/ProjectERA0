// TileTexLayerInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "TileTexLayerInfoDlg.h"
#include "TerrainEdit.h"

using namespace TE;
// CTileTexLayerInfoDlg dialog

IMPLEMENT_DYNAMIC(CTileTexLayerInfoDlg, CDialog)

CTileTexLayerInfoDlg::CTileTexLayerInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTileTexLayerInfoDlg::IDD, pParent)
{

}

CTileTexLayerInfoDlg::~CTileTexLayerInfoDlg()
{
}

void CTileTexLayerInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_TEXREPEAT_LAYER1, m_TexRepeatSpin[0]);
	DDX_Control(pDX, IDC_SPIN_TEXREPEAT_LAYER2, m_TexRepeatSpin[1]);
	DDX_Control(pDX, IDC_SPIN_TEXREPEAT_LAYER3, m_TexRepeatSpin[2]);
	DDX_Control(pDX, IDC_SPIN_TEXREPEAT_LAYER4, m_TexRepeatSpin[3]);
	DDX_Control(pDX, IDC_EDIT_TEXNAME_LAYER1, m_TexNameLayer1);
	DDX_Control(pDX, IDC_EDIT_TEXNAME_LAYER2, m_TexNameLayer2);
	DDX_Control(pDX, IDC_EDIT_TEXNAME_LAYER3, m_TexNameLayer3);
	DDX_Control(pDX, IDC_EDIT_TEXNAME_LAYER4, m_TexNameLayer4);
	DDX_Control(pDX, IDC_EDIT_TEXNAME_SHADOW, m_ShadowName);
}


BEGIN_MESSAGE_MAP(CTileTexLayerInfoDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT_TEXREPEAT_LAYER1, &CTileTexLayerInfoDlg::OnEnChangeEditTexrepeatLayer1)
	ON_EN_CHANGE(IDC_EDIT_TEXREPEAT_LAYER2, &CTileTexLayerInfoDlg::OnEnChangeEditTexrepeatLayer2)
	ON_EN_CHANGE(IDC_EDIT_TEXREPEAT_LAYER3, &CTileTexLayerInfoDlg::OnEnChangeEditTexrepeatLayer3)
	ON_EN_CHANGE(IDC_EDIT_TEXREPEAT_LAYER4, &CTileTexLayerInfoDlg::OnEnChangeEditTexrepeatLayer4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TEXREPEAT_LAYER2, &CTileTexLayerInfoDlg::OnDeltaposSpinTexrepeatLayer2)
	ON_EN_CHANGE(IDC_EDIT_TEXNAME_SHADOW, &CTileTexLayerInfoDlg::OnEnChangeEditTexnameShadow)
END_MESSAGE_MAP()


// CTileTexLayerInfoDlg message handlers

void CTileTexLayerInfoDlg::InitData()
{
	for(int i=0; i<4; i++)
	{
		int repeat = g_TerrainEdit.GetCurTexRepeat(i);
		m_TexRepeatSpin[i].SetPos(repeat);
		m_BakTexRepeat[i] = repeat;
	}

	m_TexNameLayer1.SetWindowText(g_TerrainEdit.getTextureBrushPath(0));
	m_TexNameLayer2.SetWindowText(g_TerrainEdit.getTextureBrushPath(1));
	m_TexNameLayer3.SetWindowText(g_TerrainEdit.getTextureBrushPath(2));
	m_TexNameLayer4.SetWindowText(g_TerrainEdit.getTextureBrushPath(3));
	m_ShadowName.SetWindowText(g_TerrainEdit.GetShaowTexturePath());
}

BOOL CTileTexLayerInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for(int i=0; i<4; i++)
	{
		m_TexRepeatSpin[i].SetRange(1, 24);
	}
	// TODO:  Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTileTexLayerInfoDlg::OnEnChangeEditTexrepeatLayer1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	int repeat = GetDlgItemInt(IDC_EDIT_TEXREPEAT_LAYER1);
	g_TerrainEdit.SetCurTexRepeat(0, repeat);
}

void CTileTexLayerInfoDlg::OnEnChangeEditTexrepeatLayer2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	int repeat = GetDlgItemInt(IDC_EDIT_TEXREPEAT_LAYER2);
	g_TerrainEdit.SetCurTexRepeat(1, repeat);
}

void CTileTexLayerInfoDlg::OnEnChangeEditTexrepeatLayer3()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	int repeat = GetDlgItemInt(IDC_EDIT_TEXREPEAT_LAYER3);
	g_TerrainEdit.SetCurTexRepeat(2, repeat);
}

void CTileTexLayerInfoDlg::OnEnChangeEditTexrepeatLayer4()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	int repeat = GetDlgItemInt(IDC_EDIT_TEXREPEAT_LAYER4);
	g_TerrainEdit.SetCurTexRepeat(3, repeat);
}

void CTileTexLayerInfoDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	for(int i=0; i<4; i++)
	{
		g_TerrainEdit.m_EditOption.uUVRepeat[i] = m_BakTexRepeat[i];
		g_TerrainEdit.m_bShowTextureInfoDlg = false;
	}

	CDialog::OnCancel();
}

void CTileTexLayerInfoDlg::OnDeltaposSpinTexrepeatLayer2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CTileTexLayerInfoDlg::OnEnChangeEditTexnameShadow()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
