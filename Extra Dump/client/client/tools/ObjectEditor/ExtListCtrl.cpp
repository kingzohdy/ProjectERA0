
#include "stdafx.h"
#include "ExtListCtrl.h"


// CListCtrlExt

IMPLEMENT_DYNAMIC(CListCtrlExt, CBCGPListCtrl)
CListCtrlExt::CListCtrlExt()
{
}


BEGIN_MESSAGE_MAP(CListCtrlExt, CBCGPListCtrl)
END_MESSAGE_MAP()


COLORREF CListCtrlExt::OnGetCellTextColor (int nRow, int nColumn)
{
	for (POSITION pos = m_ItemColors.GetHeadPosition (); pos != NULL;)
	{
		CExtListItem* pItem = m_ItemColors.GetNext (pos);

		ASSERT_VALID (pItem);

		if (pItem->m_x == nColumn && pItem->m_y == nRow)
		{
			if (pItem->m_clrText != -1)
			{
				return pItem->m_clrText;
			}

			break;
		}
	}

	return CBCGPListCtrl::OnGetCellTextColor (nRow, nColumn);
}

COLORREF CListCtrlExt::OnGetCellBkColor (int nRow, int nColumn)
{
	for (POSITION pos = m_ItemColors.GetHeadPosition (); pos != NULL;)
	{
		CExtListItem* pItem = m_ItemColors.GetNext (pos);
		ASSERT_VALID (pItem);

		if (pItem->m_x == nColumn && pItem->m_y == nRow)
		{
			if (pItem->m_clrBckg != -1)
			{
				return pItem->m_clrBckg;
			}

			break;
		}
	}

	return CBCGPListCtrl::OnGetCellBkColor (nRow, nColumn);
}



void CListCtrlExt::SetCellColor(int nRow, int nColum, COLORREF textColor, COLORREF bkColor)
{
	CExtListItem* pItem = NULL;

	for (POSITION pos = m_ItemColors.GetHeadPosition (); pos != NULL;)
	{
		CExtListItem* pListItem = m_ItemColors.GetNext (pos);
		ASSERT_VALID (pListItem);

		if (pListItem->m_x == nColum && pListItem->m_y == nRow)
		{
			pItem = pListItem;
			break;
		}
	}

	if (pItem == NULL)
	{
		pItem = new CExtListItem (nColum, nRow, textColor, bkColor);
		m_ItemColors.AddTail (pItem);
	}
	else
	{
		pItem->m_clrText = textColor;
		pItem->m_clrBckg = bkColor;
	}

	RedrawWindow ();
}


