// EditListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "EditListCtrl.h"


// CEditListCtrl

CEditListCtrl::CEditListCtrl()
{
	m_edit.m_hWnd = NULL;
}

CEditListCtrl::~CEditListCtrl()
{
	CComboBox* pComboBox = NULL;
	for(int i = 0; i < GetHeaderCtrl().GetItemCount(); ++i)
	{
		m_pComboBoxMap.Lookup(i , pComboBox);
		if(pComboBox)
			delete pComboBox;
	}
}

CComboBox* CEditListCtrl::GetComboBoxByColumn(int col)
{
	CComboBox* pComboBox = NULL;
	m_pComboBoxMap.Lookup(col , pComboBox);
	if(NULL == pComboBox)
	{
		pComboBox = new CListComboBox(col);
		pComboBox->Create(WS_CHILD | WS_VSCROLL|CBS_DROPDOWNLIST,CRect(0,0,0,0),this,0);
		pComboBox->ShowWindow(SW_HIDE);
		CFont tpFont;
		tpFont.CreateStockObject(DEFAULT_GUI_FONT);
		pComboBox->SetFont(&tpFont);
		tpFont.DeleteObject();
		m_pComboBoxMap[col] = pComboBox;
	}

	return pComboBox;
}

void CEditListCtrl::ShowEdit(BOOL bShow,int nItem,int nIndex,CRect rcCtrl)
{
	if(m_edit.m_hWnd == NULL)
	{
		m_edit.Create(ES_AUTOHSCROLL|WS_CHILD|ES_LEFT|ES_WANTRETURN|WS_BORDER,CRect(0,0,0,0),this,65535);
		m_edit.ShowWindow(SW_HIDE);

		CFont tpFont;
		tpFont.CreateStockObject(DEFAULT_GUI_FONT);
		m_edit.SetFont(&tpFont);
		tpFont.DeleteObject();	

		m_edit.m_pEditList = this;
	}
	if(bShow == TRUE)
	{
		CString strItem = CBCGPListCtrl::GetItemText(nItem,nIndex);
		m_edit.MoveWindow(rcCtrl);
		m_edit.ShowWindow(SW_SHOW);
		m_edit.SetWindowText(strItem);
		::SetFocus(m_edit.GetSafeHwnd());
		m_edit.SetSel(-1);  
		m_edit.SetCtrlData(MAKEWPARAM(nIndex,nItem));	
	}
	else
		m_edit.ShowWindow(SW_HIDE);
}
void CEditListCtrl::ShowComboBox(BOOL bShow,int nItem,int nIndex,CRect rcCtrl)
{
	CComboBox* pComboBox = m_pComboBoxMap[nIndex];

	if(bShow == TRUE)
	{
		CString strItem = CBCGPListCtrl::GetItemText(nItem,nIndex);
		pComboBox->MoveWindow(rcCtrl);
		pComboBox->ShowWindow(SW_SHOW);
		pComboBox->SetWindowText(strItem);
		::SetFocus(pComboBox->GetSafeHwnd());
	}
	else
		pComboBox->ShowWindow(SW_HIDE);
}
void CEditListCtrl::OnEditEnd(WPARAM wParam,LPARAM lParam)
{
	if(wParam == TRUE)
	{
		CString strText(_T(""));
		m_edit.GetWindowText(strText);
		DWORD dwData = m_edit.GetCtrlData();
		int nItem= dwData>>16;
		int nIndex = dwData&0x0000ffff;
		CBCGPListCtrl::SetItemText(nItem,nIndex,strText);
	}
	else
	{

	}

	if(lParam == FALSE)
		m_edit.ShowWindow(SW_HIDE);
}

BOOL CEditListCtrl::Key_Shift(int& nItem,int& nSub)
{
	int nItemCount = CBCGPListCtrl::GetItemCount();
	DWORD dwData = m_edit.GetCtrlData();
	nItem= dwData>>16;
	nSub = dwData&0x0000ffff;

	CHeaderCtrl* pHeader = CListCtrl::GetHeaderCtrl();
	if(pHeader == NULL)
		return FALSE;

	short sRet = GetKeyState(VK_SHIFT);
	int nSubcCount = pHeader->GetItemCount();
	sRet = sRet >>15;
	if(sRet == 0)
	{
		nSub += 1;
		if(nSub >= nSubcCount)
		{
			if(nItem == nItemCount-1)
			{
				nItem = 0;
				nSub  = 0;
			}
			else
			{
				nSub = 0;
				nItem += 1;
			}
		}
		if(nItem >= nItemCount)
			nItem = nItemCount-1;
		return FALSE;
	}
	else
	{
		nSub -= 1;
		if(nSub < 0)
		{

			nSub = nSubcCount -1;
			nItem --;
		}

		if(nItem < 0)
			nItem = nItemCount-1;
		return TRUE;

	}
	return FALSE;
}
BOOL CEditListCtrl::Key_Ctrl(int& nItem,int &nSub)
{
	short sRet = GetKeyState(VK_CONTROL);
	DWORD dwData = m_edit.GetCtrlData();
	nItem= dwData>>16;
	nSub = dwData&0x0000ffff;

	sRet = sRet >>15;
	int nItemCount = CBCGPListCtrl::GetItemCount();
	if(sRet == 0)
	{

	}
	else
	{
		nItem = nItem >=nItemCount-1? 0:nItem+=1;
		return TRUE;
	}

	return FALSE;
}


BEGIN_MESSAGE_MAP(CEditListCtrl, CBCGPListCtrl)
	ON_WM_PARENTNOTIFY()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CEditListCtrl message handlers



BOOL CEditListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_TAB && m_edit.m_hWnd!= NULL)
		{
			DWORD dwStyle = m_edit.GetStyle();
			if((dwStyle&WS_VISIBLE) == WS_VISIBLE)
			{
				OnEditEnd(TRUE,TRUE);
				CRect rcCtrl;		 
				int nItem;
				int nSub;
				if(FALSE == Key_Ctrl(nItem,nSub))
					Key_Shift(nItem,nSub);
				CBCGPListCtrl::GetSubItemRect(nItem,nSub,LVIR_LABEL,rcCtrl); 
				CPoint pt(rcCtrl.left+1,rcCtrl.top+1);
				OnLButtonDblClk(0,pt);
				POSITION pos = CBCGPListCtrl::GetFirstSelectedItemPosition();
				if (pos == NULL)
				{
				}
				else
				{
					while (pos)
					{
						int ntpItem = CBCGPListCtrl::GetNextSelectedItem(pos);
						CBCGPListCtrl::SetItemState(ntpItem,0,LVIS_SELECTED);
					}
				}
				CBCGPListCtrl::SetItemState(nItem,  LVIS_SELECTED,  LVIS_SELECTED);
				return TRUE;
			}
		}
	}

	return CBCGPListCtrl::PreTranslateMessage(pMsg);
}

void CEditListCtrl::OnParentNotify(UINT message, LPARAM lParam)
{
	CBCGPListCtrl::OnParentNotify(message, lParam);

	// TODO: Add your message handler code here
	CHeaderCtrl* pHeaderCtrl = CListCtrl::GetHeaderCtrl();
	if(pHeaderCtrl == NULL)
		return;
	CRect rcHeader;
	pHeaderCtrl->GetWindowRect(rcHeader);
	ScreenToClient(rcHeader);
	//The x coordinate is in the low-order word and the y coordinate is in the high-order word.
	CPoint pt;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);
	if(rcHeader.PtInRect(pt) && message == WM_LBUTTONDOWN)
	{
		if(m_edit.m_hWnd != NULL)
		{
			DWORD dwStyle = m_edit.GetStyle();
			if((dwStyle&WS_VISIBLE) == WS_VISIBLE)
			{
				m_edit.ShowWindow(SW_HIDE);
			}
		}	
	}
}

void CEditListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect rcCtrl;
	LVHITTESTINFO lvhti;
	lvhti.pt = point;
	int nItem = CBCGPListCtrl::SubItemHitTest(&lvhti);
	if(nItem == -1)
		return;
	int nSubItem = lvhti.iSubItem;
	CBCGPListCtrl::GetSubItemRect(nItem,nSubItem,LVIR_LABEL,rcCtrl);
	CComboBox* pComboBox = NULL;
	m_pComboBoxMap.Lookup(nSubItem , pComboBox);
	if(pComboBox)
	{
		rcCtrl.bottom += pComboBox->GetCount() * 20;
		ShowComboBox(TRUE,nItem,nSubItem,rcCtrl);
	}
	else ShowEdit(TRUE,nItem,nSubItem,rcCtrl);

	CBCGPListCtrl::OnLButtonDblClk(nFlags, point);
}

// Implementation of CListComboBox class
BEGIN_MESSAGE_MAP(CListComboBox, CComboBox)
	//{{AFX_MSG_MAP(CListComboBox)
	ON_CONTROL_REFLECT(CBN_CLOSEUP, CListComboBox::OnCloseUp)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS, CListComboBox::OnKillFocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CListComboBox::OnCloseUp() 
{
	CString strText;
	CListCtrl* pParent = (CListCtrl*)GetParent();
	ASSERT( pParent );

	int iCurSel = GetCurSel();
	if( iCurSel == CB_ERR )
		strText = m_strCurrText;
	else
		GetLBText( iCurSel, strText );

	POSITION pos = pParent->GetFirstSelectedItemPosition();
	if( pos && !strText.IsEmpty() )
		pParent->SetItemText( pParent->GetNextSelectedItem( pos ), m_iSubItem, strText );

	pParent->SetFocus();
	ShowWindow(SW_HIDE);
}

void CListComboBox::OnKillFocus() 
{
	ShowWindow( SW_HIDE );
	::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_USER_EDIT_END,0,0);
}

CListCtrlEdit::CListCtrlEdit()
{
	m_pEditList = NULL;
}

CListCtrlEdit::~CListCtrlEdit()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEdit, CEdit)
	//{{AFX_MSG_MAP(CListCtrlEdit)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEdit message handlers
void CListCtrlEdit::SetCtrlData(DWORD dwData)
{
	m_dwData = dwData;
}

DWORD CListCtrlEdit::GetCtrlData()
{
	return m_dwData;
}

void CListCtrlEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	CWnd* pParent = this->GetParent();
	::PostMessage(pParent->GetParent()->GetSafeHwnd(),WM_USER_EDIT_END,0,0);
	if( m_pEditList )
		m_pEditList->OnEditEnd ( m_bExchange, 0 );
}

BOOL CListCtrlEdit::PreTranslateMessage(MSG* pMsg) 
{

	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			CWnd* pParent = this->GetParent();
			if( m_pEditList )
				m_pEditList->OnEditEnd ( m_bExchange, 0 );
			::PostMessage(pParent->GetParent()->GetSafeHwnd(),WM_USER_EDIT_END,0,0);
		}
		else if(pMsg->wParam == VK_ESCAPE)
		{
			CWnd* pParent = this->GetParent();
			m_bExchange = FALSE;
			if( m_pEditList )
				m_pEditList->OnEditEnd ( m_bExchange, 0 );
			::PostMessage(pParent->GetParent()->GetSafeHwnd(),WM_USER_EDIT_END,0,0);
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}

void CListCtrlEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
	m_bExchange = TRUE;	
}