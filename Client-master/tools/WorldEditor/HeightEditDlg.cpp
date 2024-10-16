// HeightEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "HeightEditDlg.h"
#include "TerrainEdit.h"

using namespace TE;
// CHeightEditDlg dialog

IMPLEMENT_DYNAMIC(CHeightEditDlg, CDialog)

CHeightEditDlg::CHeightEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHeightEditDlg::IDD, pParent)
{

}

CHeightEditDlg::~CHeightEditDlg()
{
}

void CHeightEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_HMAP_BRUSHSIZE, m_BrushSizeSlider);
	DDX_Control(pDX, IDC_SLIDER_HMAP_BRUSHSTRENGTH, m_BrushStrengthSlider);
	DDX_Control(pDX, IDC_CURVEWND, m_wndCurveWnd);
	DDX_Control(pDX, IDC_POINT_MODE, m_PointMode);
}


BEGIN_MESSAGE_MAP(CHeightEditDlg, CDialog)
	ON_WM_SETFOCUS()
	ON_EN_CHANGE(IDC_EDIT_HMAP_BRUSHSIZE, &CHeightEditDlg::OnEnChangeEditHmapBrushsize)
	ON_EN_CHANGE(IDC_EDIT_HMAP_BRUSHSTRENGTH, &CHeightEditDlg::OnEnChangeEditHmapBrushstrength)
	ON_BN_CLICKED(IDC_BUTTON_HMAP_SMOOTH, &CHeightEditDlg::OnBnClickedButtonHmapSmooth)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_HMAP_BRUSHSIZE, &CHeightEditDlg::OnNMReleasedcaptureSliderHmapBrushsize)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_HMAP_BRUSHSTRENGTH, &CHeightEditDlg::OnNMReleasedcaptureSliderHmapBrushstrength)
	ON_BN_CLICKED(IDC_RADIO_HMAPCURVE_SOFT, &CHeightEditDlg::OnBnClickedRadioHmapcurveSoft)
	ON_BN_CLICKED(IDC_RADIO_HMAPCURVE_HARD, &CHeightEditDlg::OnBnClickedRadioHmapcurveHard)
	ON_BN_CLICKED(IDC_RADIO_HMAPCURVE_SMOOTH, &CHeightEditDlg::OnBnClickedRadioHmapcurveSmooth)
	ON_BN_CLICKED(IDC_RADIO_HMAPCURVE_FLAT, &CHeightEditDlg::OnBnClickedRadioHmapcurveFlat)
	ON_BN_CLICKED(IDC_RADIO_HMAPCURVE_SLOPE, &CHeightEditDlg::OnBnClickedRadioHmapcurveSlope)
	ON_BN_CLICKED(IDC_RADIO_HMAPCURVE_DITCH, &CHeightEditDlg::OnBnClickedRadioHmapcurveDitch)
	ON_BN_CLICKED(IDC_RADIO_HMAPTRIM_NONE, &CHeightEditDlg::OnBnClickedRadioHmaptrimNone)
	ON_BN_CLICKED(IDC_RADIO_HMAPTRIM_TRIM, &CHeightEditDlg::OnBnClickedRadioHmaptrimTrim)
	ON_BN_CLICKED(IDC_RADIO_HMAPTRIM_RECOVER, &CHeightEditDlg::OnBnClickedRadioHmaptrimRecover)
	ON_BN_CLICKED(IDC_RADIO_HMAP_LIFT, &CHeightEditDlg::OnBnClickedRadioHmapLift)
	ON_BN_CLICKED(IDC_RADIO_HMAP_LOWER, &CHeightEditDlg::OnBnClickedRadioHmapLower)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_HMAP_BRUSHSIZE, &CHeightEditDlg::OnNMCustomdrawSliderHmapBrushsize)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_HMAP_BRUSHSTRENGTH, &CHeightEditDlg::OnNMCustomdrawSliderHmapBrushstrength)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_CBN_SELCHANGE(IDC_POINT_MODE, &CHeightEditDlg::OnCbnSelchangePointMode)
	ON_BN_CLICKED(IDC_RADIO_HMAPCURVE_CURVE, &CHeightEditDlg::OnBnClickedRadioHmapcurveCurve)
END_MESSAGE_MAP()


// CHeightEditDlg message handlers

void CHeightEditDlg::OnSetFocus(CWnd* pOldWnd)
{
	CDialog::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
	g_TerrainEdit.SetCurEditEvent( TE::CHANGE_HEIGHT_EVN );
}

BOOL CHeightEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_BrushSizeSlider.SetRange(1, 9);
	m_BrushSizeSlider.SetPos(4);
	m_BrushStrengthSlider.SetPageSize(1);
	SetDlgItemInt(IDC_EDIT_HMAP_BRUSHSIZE, 4);

	m_BrushStrengthSlider.SetRange(1, 1000);
	m_BrushStrengthSlider.SetPos(100);
	m_BrushStrengthSlider.SetPageSize(1);
	SetDlgItemInt(IDC_EDIT_HMAP_BRUSHSTRENGTH, 100);

	CheckDlgButton(IDC_RADIO_HMAPCURVE_SOFT, 1);
	CheckDlgButton(IDC_RADIO_HMAPTRIM_NONE, 1);
	CheckDlgButton(IDC_RADIO_HMAP_LIFT, 1);

	m_PointMode.AddString("线性");
	m_PointMode.AddString("三次曲线");
	m_PointMode.AddString("常量");
	m_PointMode.SelectString(0,"三次曲线");

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CHeightEditDlg::OnEnChangeEditHmapBrushsize()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	int s = GetDlgItemInt(IDC_EDIT_HMAP_BRUSHSIZE, NULL, FALSE);
	g_TerrainEdit.SetChangeHeightSize(s);
}

void CHeightEditDlg::OnEnChangeEditHmapBrushstrength()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	int s = GetDlgItemInt(IDC_EDIT_HMAP_BRUSHSTRENGTH, NULL, FALSE);
	g_TerrainEdit.SetHeightScale(s);
}

void CHeightEditDlg::OnBnClickedButtonHmapSmooth()
{
	// TODO: Add your control notification handler code here
	g_TerrainEdit.SmoothHeightMap( 129 ,129 );
	g_TerrainEdit.UpdataNormalMap();	
	g_TerrainEdit.UpdateHeightMapToAllTileHeight();	
}

void CHeightEditDlg::OnNMReleasedcaptureSliderHmapBrushsize(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int s = m_BrushSizeSlider.GetPos();
	SetDlgItemInt(IDC_EDIT_HMAP_BRUSHSIZE, s);
}

void CHeightEditDlg::OnNMReleasedcaptureSliderHmapBrushstrength(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int s = m_BrushStrengthSlider.GetPos();
	SetDlgItemInt(IDC_EDIT_HMAP_BRUSHSTRENGTH, s);
}

void CHeightEditDlg::OnBnClickedRadioHmapcurveSoft()
{
	// TODO: Add your control notification handler code here
	g_TerrainEdit.SetCurHeightMode(TE::SOFT_MODE);
}

void CHeightEditDlg::OnBnClickedRadioHmapcurveHard()
{
	// TODO: Add your control notification handler code here
	g_TerrainEdit.SetCurHeightMode(TE::HARD_MODE);
}

void CHeightEditDlg::OnBnClickedRadioHmapcurveSmooth()
{
	// TODO: Add your control notification handler code here
	g_TerrainEdit.SetCurHeightMode(TE::SMOOTH_MODE);
}

void CHeightEditDlg::OnBnClickedRadioHmapcurveFlat()
{
	// TODO: Add your control notification handler code here
	g_TerrainEdit.SetCurHeightMode(TE::LEVEL_OFF);
}

void CHeightEditDlg::OnBnClickedRadioHmapcurveSlope()
{
	// TODO: Add your control notification handler code here
	g_TerrainEdit.SetCurHeightMode(TE::RAMP_MODE);
}

void CHeightEditDlg::OnBnClickedRadioHmapcurveDitch()
{
	// TODO: Add your control notification handler code here
	g_TerrainEdit.SetCurHeightMode(TE::DITCH_MODE);
}

void CHeightEditDlg::OnBnClickedRadioHmaptrimNone()
{
	// TODO: Add your control notification handler code here
	g_TerrainEdit.SetReduceMode(TE::NO_REDUCE);
}

void CHeightEditDlg::OnBnClickedRadioHmaptrimTrim()
{
	// TODO: Add your control notification handler code here
	g_TerrainEdit.SetReduceMode(TE::BEGIN_REDUCE);
}

void CHeightEditDlg::OnBnClickedRadioHmaptrimRecover()
{
	// TODO: Add your control notification handler code here
	g_TerrainEdit.SetReduceMode(TE::REDUCE_RESUME);
}

void CHeightEditDlg::OnBnClickedRadioHmapLift()
{
	// TODO: Add your control notification handler code here
	g_TerrainEdit.SetModifyHeight(10);
	g_TerrainEdit.SetHeightAddMode(true);
}

void CHeightEditDlg::OnBnClickedRadioHmapLower()
{
	// TODO: Add your control notification handler code here
	g_TerrainEdit.SetModifyHeight(-10);
	g_TerrainEdit.SetHeightAddMode(false);
}

void CHeightEditDlg::OnNMCustomdrawSliderHmapBrushsize(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CHeightEditDlg::OnNMCustomdrawSliderHmapBrushstrength(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

int CHeightEditDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	this->m_wndCurveWnd.SubclassWindow(this->GetDlgItem(IDC_CURVEWND)
		->GetSafeHwnd());

	return 0;
}

void CHeightEditDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnRButtonDown(nFlags, point);
}

void CHeightEditDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	ClientToScreen(&point);
	m_wndCurveWnd.MyOnLButtonDblClk(nFlags,point);
	
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CHeightEditDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	ClientToScreen(&point);
	m_wndCurveWnd.MyOnLButtonDown(nFlags,point);

	CDialog::OnLButtonDown(nFlags, point);
}

void CHeightEditDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	ClientToScreen(&point);
	m_wndCurveWnd.MyOnMouseMove(nFlags,point);

	CDialog::OnMouseMove(nFlags, point);
}

void CHeightEditDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

LRESULT CHeightEditDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	switch (message)
	{
	case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_DELETE:
				{
				}
			}
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CHeightEditDlg::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	
	ClientToScreen(&point);
	m_wndCurveWnd.MyOnRButtonDblClk(nFlags,point);

	CDialog::OnRButtonDblClk(nFlags, point);
}

void CHeightEditDlg::OnCbnSelchangePointMode()
{
	// TODO: Add your control notification handler code here
	m_wndCurveWnd.SetDefAddPointMode((Ogre::InterpCurveMode)m_PointMode.GetCurSel());
	m_wndCurveWnd.SetSelectedPointMode((Ogre::InterpCurveMode)m_PointMode.GetCurSel());
}

void CHeightEditDlg::OnBnClickedRadioHmapcurveCurve()
{
	// TODO: Add your control notification handler code here
	g_TerrainEdit.SetCurHeightMode(TE::CURVE_MODE);
	g_TerrainEdit.SetHeightCurve(m_wndCurveWnd.GetInterpCurve());
}