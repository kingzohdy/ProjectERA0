// NewColorKeyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "NewColorKeyDlg.h"


// CNewColorKeyDlg dialog

IMPLEMENT_DYNAMIC(CNewColorKeyDlg, CDialog)

CNewColorKeyDlg::CNewColorKeyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewColorKeyDlg::IDD, pParent)
{

}

CNewColorKeyDlg::~CNewColorKeyDlg()
{
}

void CNewColorKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CLRKEY, m_KeyValueList);
}


BEGIN_MESSAGE_MAP(CNewColorKeyDlg, CDialog)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_MESSAGE(EEN_LINE_UPDATE_DOT, OnLineUpdateDot)
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_CHECK_RANDOM_FRAME, &CNewColorKeyDlg::OnBnClickedCheckRandomFrame)
END_MESSAGE_MAP()


// CNewColorKeyDlg message handlers
BOOL CNewColorKeyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//m_colorMove.Create(3, EEN_LINE_UPDATE_DOT, CRect(10, 10, 435, 100), this);

	//m_colorMove.SetItemColor(0, m_color1);
	//m_colorMove.SetItemColor(1, m_color2);
	//m_colorMove.SetItemColor(2, m_color3);

	//m_colorMove.SetItemPos(1, m_fMidPoint);
	m_KeyValueList.InsertColumn(0, "控制点", LVCFMT_LEFT, 100);
	m_KeyValueList.InsertColumn(1, "位置", LVCFMT_LEFT, 100);

	UpdateCtrl();	// 颜色滑块
	RefreshList();
	UpdateInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CNewColorKeyDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_colorMove.OnDblClick(point);

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CNewColorKeyDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_colorMove.OnLBtnDown(point);
	CDialog::OnLButtonDown(nFlags, point);
}

void CNewColorKeyDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_colorMove.OnLBtnUp(point);
	CDialog::OnLButtonUp(nFlags, point);
}

void CNewColorKeyDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_colorMove.OnMouseMove(point);
	CDialog::OnMouseMove(nFlags, point);
}

void CNewColorKeyDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	m_colorMove.OnDrawCtrl(&dc);
}

COLORREF CNewColorKeyDlg::ColorFltToRef(Ogre::ColourValue flt)
{
	return RGB((int)(flt.r*255),(int)(flt.g*255),(int)(flt.b*255));
}

Ogre::ColourValue CNewColorKeyDlg::ColorRefToFlt(COLORREF ref)
{
	return Ogre::ColourValue(GetRValue(ref) / 255.0f, GetGValue(ref) / 255.0f, GetBValue(ref) / 255.0f, 1.0f);
}

LRESULT CNewColorKeyDlg::OnLineUpdateDot(WPARAM w, LPARAM l)
{
	CColorMoveCtrl* pCM = (CColorMoveCtrl*) l;
	switch(w)
	{
	case LM_DOTCHANGED:
		//m_fMidPoint = m_colorMove.GetItemPos(1);
		//m_color1 = m_colorMove.GetItemColor(0);
		//m_color2 = m_colorMove.GetItemColor(1);
		//m_color3 = m_colorMove.GetItemColor(2);
		//UpdateInfo();
		{
			for(int i = 0; i < (int) m_KeyValues.m_Array.size(); i++)
			{
				m_KeyValues.m_Array[i].tick = (Ogre::uint)(m_colorMove.GetItemPos(i) * 100);
				Ogre::ColourValue fData =ColorRefToFlt(m_colorMove.GetItemColor(i));
				m_KeyValues.m_Array[i].data = fData;
			}

			UpdateInfo();
		}
		break;
	case LM_DOTSPECIAL:
		if(m_KeyValues.m_Array.size() <= 3)
		{
			MessageBox("控制点不能少于三个");
			break;
		}
		if(m_colorMove.GetParamIndex() <= 0 || m_colorMove.GetParamIndex() >= (int)m_KeyValues.m_Array.size() - 1)
		{
			MessageBox("边缘的控制点不能删除");
			break;
		}

		m_KeyValues.m_Array.erase(m_KeyValues.m_Array.begin() + m_colorMove.GetParamIndex());
		UpdateCtrl();
		RefreshList();

		break;
	case LM_BLANKDBLCLICK:
		if(m_colorMove.GetParamIndex() <= 0 || m_colorMove.GetParamIndex() >= (int)m_KeyValues.m_Array.size())
		{
			MessageBox("插入点错误");
			break;
		}
		Ogre::KeyFrameArray<Ogre::ColourValue>::KEYFRAME_T kfInfo;
		kfInfo.tick = (Ogre::uint)(m_colorMove.GetParamPos() * 100);

		m_KeyValues.getValue(0, kfInfo.tick, kfInfo.data);


		m_KeyValues.m_Array.insert(m_KeyValues.m_Array.begin() + m_colorMove.GetParamIndex(), kfInfo);

		UpdateCtrl(); 
		RefreshList();
		break;
	}
	return 0;
}

void CNewColorKeyDlg::UpdateInfo()
{
	//CString info;
	//info.Format("中点: %.3f;  颜色变化(RGB): (%d, %d, %d) --> (%d, %d, %d)  --> (%d, %d, %d)",
	//	m_fMidPoint, 
	//	GetRValue(m_color1), GetGValue(m_color1), GetBValue(m_color1),
	//	GetRValue(m_color2), GetGValue(m_color2), GetBValue(m_color2),
	//	GetRValue(m_color3), GetGValue(m_color3), GetBValue(m_color3));

	//m_editInfo.SetWindowText(info.GetBuffer());
}

void CNewColorKeyDlg::RefreshList()
{
	m_KeyValueList.DeleteAllItems();
	for(int i = 0; i < (int) m_KeyValues.m_Array.size(); i++)
	{
		CString str;
		str.Format("控制点 %d", i);
		m_KeyValueList.InsertItem(LVIF_PARAM | LVIF_TEXT, i, str.GetBuffer(), 0, 0, 0, i);
	}

	if(m_KeyValues.m_Type == Ogre::INTERPOL_RANDOM) CheckDlgButton(IDC_CHECK_RANDOM_FRAME, 1);
	else CheckDlgButton(IDC_CHECK_RANDOM_FRAME, 0);
}

void CNewColorKeyDlg::UpdateCtrl()
{
	//m_lineMove.SetItemValue(0, ValueRealToVirtual(m_fValue1));
	//m_lineMove.SetItemValue(1, ValueRealToVirtual(m_fValue2));
	//m_lineMove.SetItemValue(2, ValueRealToVirtual(m_fValue3));

	//m_lineMove.SetItemPos(1, m_fMidPoint);


	m_colorMove.Create(m_KeyValues.m_Array.size(), EEN_LINE_UPDATE_DOT, CRect(10, 10, 435, 100), this);
	for(int i = 0; i < (int) m_KeyValues.m_Array.size(); i++)
	{
		m_colorMove.SetItemPos(i, m_KeyValues.m_Array[i].tick * 0.01f);
		m_colorMove.SetItemColor(i,ColorFltToRef( m_KeyValues.m_Array[i].data)); 
	}
	Invalidate(FALSE);

}

void CNewColorKeyDlg::OnBnClickedCheckRandomFrame()
{
	// TODO: Add your control notification handler code here
	if(IsDlgButtonChecked(IDC_CHECK_RANDOM_FRAME)) m_KeyValues.m_Type = Ogre::INTERPOL_RANDOM;
	else m_KeyValues.m_Type = Ogre::INTERPOL_LINEAR;
}
