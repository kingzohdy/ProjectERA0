// NewFloatKeyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectEditor.h"
#include "NewFloatKeyDlg.h"


// CNewFloatKeyDlg dialog

IMPLEMENT_DYNAMIC(CNewFloatKeyDlg, CDialog)

CNewFloatKeyDlg::CNewFloatKeyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewFloatKeyDlg::IDD, pParent)
	, m_fCurrPos(0)
	, m_fCurrValue(0)
{
	m_Mode = FKD_LINEAR;
}

CNewFloatKeyDlg::~CNewFloatKeyDlg()
{
}

void CNewFloatKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FLT_POS, m_fCurrPos);
	DDX_Text(pDX, IDC_EDIT_FLT_VALUE, m_fCurrValue);
	DDX_Control(pDX, IDC_LIST_POS, m_KeyValueList);
}


BEGIN_MESSAGE_MAP(CNewFloatKeyDlg, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()

	ON_MESSAGE(EEN_LINE_UPDATE_DOT, OnLineUpdateDot)

	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_POS, OnLvnItemchangedListPos)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_LIST_POS, OnLvnItemchangingListPos)
	ON_EN_CHANGE(IDC_EDIT_MIDDLE, OnEnChangeEditMiddle)
END_MESSAGE_MAP()


// CNewFloatKeyDlg message handlers
BOOL CNewFloatKeyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lineMove.Create(3, EEN_LINE_UPDATE_DOT, CRect(10, 10, 435, 100), this, LCFLAG_FIXPOS_END);

	m_KeyValueList.InsertColumn(0, "控制点", LVCFMT_LEFT, 100);
	m_KeyValueList.InsertColumn(1, "位置", LVCFMT_LEFT, 100);
	UpdateCtrl();

	CString title;
	title.Format("%s调整", m_strValueName.GetBuffer());
	SetWindowText(title.GetBuffer());
	UpdateInfo();
	RefreshList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CNewFloatKeyDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_lineMove.OnLBtnDown(point);
	CDialog::OnLButtonDown(nFlags, point);
}

void CNewFloatKeyDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_lineMove.OnLBtnUp(point);
	CDialog::OnLButtonUp(nFlags, point);
}

void CNewFloatKeyDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_lineMove.OnMouseMove(point);
	CDialog::OnMouseMove(nFlags, point);
}

void CNewFloatKeyDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_lineMove.OnLButtonDblClk(point);
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CNewFloatKeyDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	m_lineMove.OnDrawCtrl(&dc);
}

LRESULT CNewFloatKeyDlg::OnLineUpdateDot(WPARAM w, LPARAM l)
{
	CLineMoveCtrl* pCM = (CLineMoveCtrl*) l;
	switch(w)
	{
	case LM_DOTCHANGED:
		//m_fMidPoint = m_lineMove.GetItemPos(1);
		//m_fValue1 = ValueVirtualToReal( m_lineMove.GetItemValue(0));
		//m_fValue2 = ValueVirtualToReal(m_lineMove.GetItemValue(1));
		//m_fValue3 = ValueVirtualToReal(m_lineMove.GetItemValue(2));
		{
			for(int i = 0; i < (int) m_KeyValues.m_Array.size(); i++)
			{
				m_KeyValues.m_Array[i].tick = (Ogre::uint)(m_lineMove.GetItemPos(i) * 100);
				float fData =ValueVirtualToReal(m_lineMove.GetItemValue(i));
				m_KeyValues.m_Array[i].data = fData;
			}

			UpdateInfo();

			int iSel = m_KeyValueList.GetSelectionMark();
			if(iSel >= 0 && iSel < (int)m_KeyValues.m_Array.size())
			{
				m_fCurrPos = m_KeyValues.m_Array[iSel].tick * 0.01f;
				m_fCurrValue =m_KeyValues.m_Array[iSel].data;
				UpdateData(FALSE);
			}
		}
		break;
	case LM_DOTSPECIAL:
		if(m_KeyValues.m_Array.size() <= 3)
		{
			MessageBox("控制点不能少于三个");
			break;
		}
		if(m_lineMove.GetParamIndex() <= 0 || m_lineMove.GetParamIndex() >= (int)m_KeyValues.m_Array.size() - 1)
		{
			MessageBox("边缘的控制点不能删除");
			break;
		}

		m_KeyValues.m_Array.erase(m_KeyValues.m_Array.begin() + m_lineMove.GetParamIndex());
		UpdateCtrl();
		RefreshList();

		break;
	case LM_BLANKDBLCLICK:
		if(m_lineMove.GetParamIndex() <= 0 || m_lineMove.GetParamIndex() >= (int)m_KeyValues.m_Array.size())
		{
			MessageBox("插入点错误");
			break;
		}
		Ogre::KeyFrameArray<float>::KEYFRAME_T kfInfo;
		kfInfo.data = ValueVirtualToReal(m_lineMove.GetParamValue());
		kfInfo.tick = (Ogre::uint)(m_lineMove.GetParamPos() * 100);

		m_KeyValues.m_Array.insert(m_KeyValues.m_Array.begin() + m_lineMove.GetParamIndex(), kfInfo);

		UpdateCtrl(); 
		RefreshList();
		break;
	}
	return 0;
}

void CNewFloatKeyDlg::UpdateInfo()
{
	CString info;
	//info.Format("中点: %.3f;  %s变化: %.3f --> %.3f --> %.3f",
	//	m_fMidPoint, m_strValueName.GetBuffer(), m_fValue1, m_fValue2, m_fValue3);
	//
	//info.Format("中点: %.3f;  %s变化: ",
	//m_fMidPoint, m_strValueName.GetBuffer());

	//m_editInfo.SetWindowText(info.GetBuffer());
	UpdateData(FALSE);
}

void CNewFloatKeyDlg::UpdateCtrl()
{
	//m_lineMove.SetItemValue(0, ValueRealToVirtual(m_fValue1));
	//m_lineMove.SetItemValue(1, ValueRealToVirtual(m_fValue2));
	//m_lineMove.SetItemValue(2, ValueRealToVirtual(m_fValue3));

	//m_lineMove.SetItemPos(1, m_fMidPoint);



	m_lineMove.Create(m_KeyValues.m_Array.size(), EEN_LINE_UPDATE_DOT, CRect(10, 10, 435, 100), this, LCFLAG_FIXPOS_END);
	for(int i = 0; i < (int) m_KeyValues.m_Array.size(); i++)
	{
		assert( i < m_lineMove.m_dots.size());
		m_lineMove.SetItemPos(i, m_KeyValues.m_Array[i].tick * 0.01f);
		m_lineMove.SetItemValue(i,ValueRealToVirtual( m_KeyValues.m_Array[i].data)); 
	}
	Invalidate(FALSE);

}

void CNewFloatKeyDlg::RefreshList()
{
	m_KeyValueList.DeleteAllItems();
	for(int i = 0; i < (int) m_KeyValues.m_Array.size(); i++)
	{
		CString str;
		str.Format("控制点 %d", i);
		m_KeyValueList.InsertItem(LVIF_PARAM | LVIF_TEXT, i, str.GetBuffer(), 0, 0, 0, i);
	}
}

float CNewFloatKeyDlg::ValueRealToVirtual(float realvalue)
{
	if(m_Mode == FKD_LINEAR)
		return (realvalue - m_fMin) / (m_fMax - m_fMin);
	else //FKD_LOG;
		return ::logf(realvalue / m_fMin) / ::logf( m_fMax / m_fMin);
}

float CNewFloatKeyDlg::ValueVirtualToReal(float virtualvalue)
{
	if(m_Mode == FKD_LINEAR)
		return virtualvalue * (m_fMax - m_fMin) + m_fMin;
	else //FKD_LOG;
		return Ogre::Pow(m_fMax / m_fMin, virtualvalue) * m_fMin;
}

void CNewFloatKeyDlg::OnEnKillfocusEditStart()
{
	UpdateData();
	UpdateCtrl();
}

void CNewFloatKeyDlg::OnEnKillfocusEditMiddle()
{
	UpdateData();
	UpdateCtrl();
}

void CNewFloatKeyDlg::OnEnKillfocusEditFkinfo()
{
	UpdateData();
	UpdateCtrl();
}



void CNewFloatKeyDlg::OnLvnItemchangedListPos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if((pNMLV->uOldState & LVIS_SELECTED) != 0 && (pNMLV->uNewState & LVIS_SELECTED) == 0)
	{
		UpdateData();
		int iIndex = pNMLV->iItem;
		m_KeyValues.m_Array[iIndex].data = m_fCurrValue;

		if(iIndex > 0 && m_fCurrPos < m_KeyValues.m_Array[iIndex-1].tick * 0.01f)
			m_fCurrPos = m_KeyValues.m_Array[iIndex-1].tick * 0.01f;

		if(iIndex < m_KeyValues.m_Array.size() - 1 && m_fCurrPos > m_KeyValues.m_Array[iIndex+1].tick * 0.01f)
			m_fCurrPos = m_KeyValues.m_Array[iIndex+1].tick * 0.01f;

		m_KeyValues.m_Array[iIndex].tick = (Ogre::uint)(m_fCurrPos * 100);
		UpdateCtrl();
	}

	if((pNMLV->uOldState & LVIS_SELECTED) == 0 && (pNMLV->uNewState & LVIS_SELECTED) != 0)
	{
		int iSel = pNMLV->iItem;
		if(iSel >= 0 && iSel < (int)m_KeyValues.m_Array.size())
		{
			m_fCurrPos = m_KeyValues.m_Array[iSel].tick * 0.01f;
			m_fCurrValue =m_KeyValues.m_Array[iSel].data;
			UpdateData(FALSE);
		}
	}

	*pResult = 0;
}

void CNewFloatKeyDlg::OnLvnItemchangingListPos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CNewFloatKeyDlg::OnEnChangeEditMiddle()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CNewFloatKeyDlg::OnOK()
{
	UpdateData();
	int iIndex = m_KeyValueList.GetSelectionMark();

	if(iIndex >= 0)
	{
		m_KeyValues.m_Array[iIndex].data = m_fCurrValue;

		if(iIndex > 0 && m_fCurrPos < m_KeyValues.m_Array[iIndex-1].tick * 0.01f)
			m_fCurrPos = m_KeyValues.m_Array[iIndex-1].tick * 0.01f;

		if(iIndex < m_KeyValues.m_Array.size() - 1 && m_fCurrPos > m_KeyValues.m_Array[iIndex+1].tick * 0.01f)
			m_fCurrPos = m_KeyValues.m_Array[iIndex+1].tick * 0.01f;

		m_KeyValues.m_Array[iIndex].tick = (Ogre::uint)(m_fCurrPos * 100);
	}
	//UpdateCtrl();

	CDialog::OnOK();
}