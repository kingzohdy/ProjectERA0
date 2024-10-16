
#include "stdafx.h"
#include "ColorMoveCtrl.h"

CColorMoveCtrl::CColorMoveCtrl(void)
{
}

CColorMoveCtrl::~CColorMoveCtrl(void)
{
}

void CColorMoveCtrl::Create(int iDotCount, unsigned int notifyMsg, CRect rect, CWnd* pParent)
{
	CLineMoveCtrl::Create(iDotCount, notifyMsg,rect, pParent,  LCFLAG_FIXPOS_END | LCFLAG_FIXVALUE_ALL);
	m_colorDots.clear();
	for(int i = 0; i < iDotCount; i++)
	{
		ColorDot cd;
		cd.pos = m_dots[i].pos;
		cd.color = RGB(255, 255, 255);
		m_colorDots.push_back(cd);
	}
}

void CColorMoveCtrl::OnDblClick(CPoint point)
{
	bool bHit = false;
	for(int i = 0; i < (int) m_dots.size(); i++)
	{
		if(HitTest(point, m_dots[i]))
		{
			CColorDialog colorDlg;
			if(IDOK == colorDlg.DoModal())
			{
				m_colorDots[i].color = colorDlg.GetColor();
				if(m_notifyMsg != NULL)
					m_pParent->PostMessage(m_notifyMsg, LM_DOTCHANGED, (LPARAM)this);
				m_pParent->InvalidateRect(m_boundRect, TRUE);
			}

			bHit = true;
			break;
		}
	}
	if(bHit == false)
	{
		CLineMoveCtrl::OnLButtonDblClk(point);
	}
}

void CColorMoveCtrl::DrawBackground(CDC* pDc)
{
	int lastx = m_ctrlRect.left;
	for(int i = 0; i < (int) m_dots.size(); i++)
	{
		ColorDot& cd = m_colorDots[i];
		CPoint pt = TransmitPoint(m_dots[i]);

		COLORREF oldColor = RGB(255, 255, 255);
		COLORREF newColor = m_colorDots[i].color;
		if(i != 0) 
			oldColor = m_colorDots[i - 1].color;

		for(int x = lastx; x < pt.x; x++)
		{
			unsigned char r = (unsigned char) (GetRValue(oldColor) 
				+ (GetRValue(newColor) - GetRValue(oldColor)) * (x - lastx) / (float)(pt.x - lastx));
			unsigned char g = (unsigned char) (GetGValue(oldColor) 
				+ (GetGValue(newColor) - GetGValue(oldColor)) * (x - lastx) / (float)(pt.x - lastx));
			unsigned char b = (unsigned char) (GetBValue(oldColor) 
				+ (GetBValue(newColor) - GetBValue(oldColor)) * (x - lastx) / (float)(pt.x - lastx));

			COLORREF curClr = RGB( r, g, b);
			CPen pen(PS_SOLID, 1, curClr);

			CPen* pOldPen = pDc->SelectObject(&pen);

			pDc->MoveTo(x, m_ctrlRect.top);
			pDc->LineTo(x, m_ctrlRect.bottom);
			pDc->SelectObject(pOldPen);
		}
		lastx = pt.x;
	}
}

void CColorMoveCtrl::SetItemPos(int index, float pos)
{
	if(index < 0 || index >= (int)m_dots.size())
		return;
	CLineMoveCtrl::SetItemPos(index, pos);

	m_colorDots[index].pos = m_dots[index].pos;
}


void CColorMoveCtrl::SetItemColor(int index, COLORREF clr)
{
	if(index < 0 || index >= (int)m_dots.size())
		return;

	m_colorDots[index].color = clr;

	m_pParent->InvalidateRect(m_boundRect, TRUE);
}

COLORREF CColorMoveCtrl::GetItemColor(int index)
{
	if(index < 0 || index >= (int)m_dots.size())
		return RGB(255, 255, 255);

	return m_colorDots[index].color;
}