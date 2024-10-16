// DirectionEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "DirectionEditDlg.h"
#include "EditorManager.h"

#include "OgreGameScene.h"


#define ID_RENDERTIMER	10
// CDirectionEditDlg dialog

IMPLEMENT_DYNAMIC(CDirectionEditDlg, CDialog)

CDirectionEditDlg::CDirectionEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDirectionEditDlg::IDD, pParent)
	, m_fHrot(0)
	, m_fVrot(0)
	, m_fx(0)
	, m_fy(0)
	, m_fz(0)
	, m_fAcclen(0)
{

}

CDirectionEditDlg::~CDirectionEditDlg()
{
}

void CDirectionEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1, m_wndRenderStatic);
	DDX_Text(pDX, IDC_EDIT_HROT, m_fHrot);
	DDX_Text(pDX, IDC_EDIT_VROT, m_fVrot);
	DDX_Text(pDX, IDC_EDIT_X, m_fx);
	DDX_Text(pDX, IDC_EDIT_Y, m_fy);
	DDX_Text(pDX, IDC_EDIT_Z, m_fz);
	DDX_Text(pDX, IDC_EDIT_ACCLEN, m_fAcclen);
	DDX_Control(pDX, IDC_SLD_HROT, m_sldHrot);
	DDX_Control(pDX, IDC_SLD_VROT, m_sldVrot);
	DDX_Control(pDX, IDC_SLD_ACCLEN, m_sldAcclen);
}


BEGIN_MESSAGE_MAP(CDirectionEditDlg, CDialog)
	ON_NOTIFY(TB_THUMBTRACK, IDC_SLD_HROT, OnSldHrotTrace)
	ON_NOTIFY(TB_THUMBTRACK, IDC_SLD_VROT, OnSldVrotTrace)
	ON_NOTIFY(TB_THUMBTRACK, IDC_SLD_ACCLEN, OnSldAcclenTrace)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_EN_KILLFOCUS(IDC_EDIT_HROT, OnEnKillfocusEditHrot)
	ON_EN_KILLFOCUS(IDC_EDIT_VROT, OnEnKillfocusEditVrot)
END_MESSAGE_MAP()


// CDirectionEditDlg message handlers
BOOL CDirectionEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_sldHrot.SetRange(0, 719);
	m_sldVrot.SetRange(0, 719);

	m_wndRenderStatic.Init();
	m_wndRenderStatic.StartRender();
	//m_wndRenderStatic.SetSelfDraw(true);
	//m_wndRenderStatic.SetModel("mmrun");
	m_pRotCoord = new CRotCood();
	m_pRotCoord->attachToScene(m_wndRenderStatic.m_pPreviewRoom);
	m_wndRenderStatic.SetCallBackHandler(this);
	m_wndRenderStatic.SetAutoCamera(false);
	SetTimer(ID_RENDERTIMER, 30, NULL);

	m_sldHrot.SetPos(int(m_fHrot * (float)m_sldHrot.GetRangeMax() / 359.5f));
	m_sldVrot.SetPos(int(m_fVrot * (float)m_sldVrot.GetRangeMax() / 359.5f));

	m_pRotCoord->m_fVrot = m_fVrot;
	m_pRotCoord->m_fHrot = m_fHrot;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDirectionEditDlg::OnSldHrotTrace(NMHDR* pNm, LRESULT* pResult)
{
	//m_fHrot = m_sldHrot.GetPos() / (float) m_sldHrot.GetRangeMax();
	//UpdateData(FALSE);
}

void CDirectionEditDlg::OnSldVrotTrace(NMHDR* pNm, LRESULT* pResult)
{
}

void CDirectionEditDlg::OnSldAcclenTrace(NMHDR* pNm, LRESULT* pResult)
{
}
void CDirectionEditDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CDirectionEditDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;
	if(pSlider == &m_sldHrot)
	{
		m_fHrot = 359.5f * m_sldHrot.GetPos() / (float) m_sldHrot.GetRangeMax();
	}
	else if(pSlider == &m_sldVrot)
	{
		m_fVrot = 359.5f * m_sldVrot.GetPos() / (float) m_sldVrot.GetRangeMax();
	}
	else if(pSlider == &m_sldAcclen)
	{
	}

	UpdateDirByRot();
	//UpdateData(FALSE);
	m_pRotCoord->m_fHrot = m_fHrot;
	m_pRotCoord->m_fVrot = m_fVrot;
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDirectionEditDlg::UpdateDirByRot()
{
	m_fx = Ogre::Cos(m_fHrot) * Ogre::Cos(m_fVrot);
	m_fy = Ogre::Sin(m_fVrot);
	m_fz = Ogre::Sin(m_fHrot) * Ogre::Cos(m_fVrot);

	if(m_fx > -0.000001 && m_fx < 0.000001)
		m_fx = 0.0f;
	if(m_fy > -0.000001 && m_fy < 0.000001)
		m_fy = 0.0f;
	if(m_fz > -0.000001 && m_fz < 0.000001)
		m_fz = 0.0f;

	UpdateData(FALSE);
	GetDlgItem(IDC_SLD_HROT)->Invalidate(FALSE);

}

void CDirectionEditDlg::UpdateRotByDir()
{
	m_fHrot = Ogre::Rad2Deg( atan(m_fz / m_fx));
	m_fVrot = Ogre::Rad2Deg( atan(m_fy / Ogre::Sqrt(m_fx * m_fx + m_fz * m_fz)));

	if(m_fx == 0 && m_fz != 0)
	{
		m_fHrot = m_fz > 0 ? 90.0f : 270.0f;
	}
	if(m_fx == 0 && m_fz == 0)
	{
		m_fHrot = m_fz > 0 ? 90.0f : 270.0f;
		m_fVrot = m_fy > 0 ? 90.0f : 270.0f;
	}
}

void CDirectionEditDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	//delete(m_pRotCoord);
	m_wndRenderStatic.ShutRender();
	CDialog::OnOK();
}

void CDirectionEditDlg::OnCancel()
{
	//delete(m_pRotCoord);
	m_wndRenderStatic.ShutRender();
	//m_wndRenderStatic.SetSelfDraw(true);
	CDialog::OnCancel();
}

void CDirectionEditDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}


void CDirectionEditDlg::OnCallbackUpdate(float dtime)
{
	//if(m_iAnchor > 0)
	//m_pCoord->SetPosition(m_wndRenderStatic.m_pModel->GetMainNode()->GetAnchorWorldMatrix(m_iAnchor).GetRow3(3));
}

void CDirectionEditDlg::OnCallbackDraw()
{
}

void CDirectionEditDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	long lTick = ::GetTickCount();
	CEnginAgent::GetInstance()->Update(lTick);
	m_wndRenderStatic.DoFrame();
	__super::OnTimer(nIDEvent);
}

void CDirectionEditDlg::OnEnKillfocusEditHrot()
{
	UpdateData(TRUE);
	UpdateDirByRot();
	m_pRotCoord->m_fHrot = m_fHrot;

	m_sldHrot.SetPos(int(m_fHrot * (float)m_sldHrot.GetRangeMax() / 359.5f));
}

void CDirectionEditDlg::OnEnKillfocusEditVrot()
{
	UpdateData(TRUE);
	UpdateDirByRot();
	m_pRotCoord->m_fVrot = m_fVrot;

	m_sldVrot.SetPos(int(m_fVrot * (float)m_sldVrot.GetRangeMax() / 359.5f));
}