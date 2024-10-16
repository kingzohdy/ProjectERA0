
#pragma once

class CExtListItem : public CObject
{
public:
	CExtListItem(int x, int y, COLORREF clrText, COLORREF clrBckg) :
	  m_x (x),
		  m_y (y),
		  m_clrText (clrText),
		  m_clrBckg (clrBckg)
	  {
	  }

	  const int	m_x;
	  const int	m_y;
	  COLORREF	m_clrText;
	  COLORREF	m_clrBckg;
};


// CListCtrlExt

class CListCtrlExt : public CBCGPListCtrl
{
	DECLARE_DYNAMIC(CListCtrlExt)

public:
	CListCtrlExt();
	virtual ~CListCtrlExt()
	{
		while (!m_ItemColors.IsEmpty ())
		{
			delete m_ItemColors.RemoveHead ();
		}
	}

	virtual COLORREF OnGetCellTextColor (int nRow, int nColum);
	virtual COLORREF OnGetCellBkColor (int nRow, int nColum);

	void SetCellColor(int nRow, int nColum, COLORREF textColor, COLORREF bkColor);

	CList<CExtListItem*, CExtListItem*>	m_ItemColors;

protected:
	DECLARE_MESSAGE_MAP()
};


