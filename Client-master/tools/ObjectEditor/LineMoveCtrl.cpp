
#include "stdafx.h"
#include "LineMoveCtrl.h"

CLineMoveCtrl::CLineMoveCtrl(void)
{
	m_curDot = -1;
}

CLineMoveCtrl::~CLineMoveCtrl(void)
{
}


void CLineMoveCtrl::Create(int iDotCount, unsigned int notifyMsg,CRect rect, CWnd* pParent,  unsigned int flags)
{
	m_pParent = pParent;
	m_ctrlRect = rect;
	m_notifyMsg = notifyMsg;
	m_dots.clear();
	for(int i = 0; i < iDotCount; i++)
	{
		ItemDot dot;
		dot.value = 0.5;
		dot.pos =  i * 1.0f / (iDotCount - 1);
		dot.flags = 0;

		if((flags & LCFLAG_FIXPOS_ALL) != 0)
			dot.flags |= DOTFLAG_FIXPOS;

		if((flags & LCFLAG_FIXVALUE_ALL) != 0)
			dot.flags |= DOTFLAG_FIXVALUE;

		m_dots.push_back(dot);
	}

	if((flags & LCFLAG_FIXPOS_END) != 0)
	{
		m_dots[0].flags |= DOTFLAG_FIXPOS;
		m_dots[iDotCount - 1].flags |= DOTFLAG_FIXPOS;
	}

	if((flags & LCFLAG_FIXVALUE_END) != 0)
	{
		m_dots[0].flags |= DOTFLAG_FIXVALUE;
		m_dots[iDotCount - 1].flags |= DOTFLAG_FIXVALUE;
	}

	m_boundRect.top			= m_ctrlRect.top - 4;
	m_boundRect.bottom		= m_ctrlRect.bottom + 4;
	m_boundRect.left		= m_ctrlRect.left - 4;
	m_boundRect.right		= m_ctrlRect.right + 4;

}

CPoint CLineMoveCtrl::TransmitPoint(ItemDot& dot)
{
	int x = (int)(dot.pos * m_ctrlRect.Width());
	int y = (int)((1 - dot.value) * m_ctrlRect.Height());
	return CPoint(x + m_ctrlRect.left, y + m_ctrlRect.top);
}

CLineMoveCtrl::ItemDot CLineMoveCtrl::TransmitDot(CPoint& point)
{
	ItemDot ret;
	ret.pos = ((float)point.x - (float)m_ctrlRect.left) / (float)m_ctrlRect.Width();
	ret.value = 1.0f - ((float)point.y - (float)m_ctrlRect.top) / (float)m_ctrlRect.Height();

	return ret;
}

bool CLineMoveCtrl::HitTest(CPoint& point, ItemDot& dot)
{
	CPoint tp = TransmitPoint(dot);
	CRect rc(point.x - 4, point.y - 4, point.x + 4, point.y + 4);
	if(rc.PtInRect(tp) == TRUE)
		return true;

	return false;
}

void CLineMoveCtrl::TrimRect(CRect& rc)
{
	if(rc.left < m_ctrlRect.left)
		rc.left = m_ctrlRect.left;
	if(rc.top < m_ctrlRect.top)
		rc.top = m_ctrlRect.top;
	if(rc.right > m_ctrlRect.right)
		rc.right = m_ctrlRect.right;
	if(rc.bottom > m_ctrlRect.bottom)
		rc.bottom = m_ctrlRect.bottom;
}

void CLineMoveCtrl::OnDrawCtrl(CDC* pDc)
{
	DrawBorder(pDc);
	DrawBackground(pDc);
	DrawDots(pDc);

}

void CLineMoveCtrl::DrawBorder(CDC* pDc)
{
	CBrush bBlack(RGB(0, 0, 0));
	//pDc->FillRect(m_boundRect, &bBlack);
	//pDc->Draw3dRect(m_boundRect, 4, 0);
}

void CLineMoveCtrl::DrawBackground(CDC* pDc)
{
	CBrush bWhite(RGB(255, 255, 255));
	//pDc->FillRect(m_ctrlRect, &bWhite);
	pDc->FillRect(m_boundRect, &bWhite);
	pDc->Rectangle(m_ctrlRect);
}

void CLineMoveCtrl::DrawDots(CDC* pDc)
{
	CBrush bRed(RGB(255,0,0));
	CBrush bBlack(RGB(0,0,0));
	CBrush bWhite(RGB(255, 255, 255));


	pDc->MoveTo(TransmitPoint(m_dots[0]));
	for(int i = 1; i < (int) m_dots.size(); i++)
	{
		pDc->LineTo(TransmitPoint(m_dots[i]));
	}

	for(int i = 0; i < (int) m_dots.size(); i++)
	{
		CPoint tp = TransmitPoint(m_dots[i]);
		CRect rc(tp.x - 4, tp.y - 4, tp.x + 4, tp.y + 4);
		CRect rc2(tp.x - 3, tp.y - 3, tp.x + 3, tp.y + 3);

		//TrimRect(rc);
		//TrimRect(rc2);
		if(m_curDot == i)
		{
			pDc->FillRect(rc, &bBlack);
			pDc->FillRect(rc2, &bRed);
		}
		else
		{
			pDc->FillRect(rc, &bBlack);
			pDc->FillRect(rc2, &bWhite);

		}
	}
}


void CLineMoveCtrl::OnLBtnDown(CPoint point)
{
	for(int i = 0; i < (int) m_dots.size(); i++)
	{
		if(HitTest(point, m_dots[i]))
		{
			m_curDot = i;
			m_pParent->InvalidateRect(m_boundRect, TRUE);
			break;
		}
	}
}

void CLineMoveCtrl::OnLBtnUp(CPoint pt)
{
	m_curDot = -1;
	m_pParent->InvalidateRect(m_boundRect, TRUE);

	bool bHit = false;
	for(int i = 0; i < (int) m_dots.size(); i++)
	{
		if(HitTest(pt, m_dots[i]))
		{
			m_iParamIndex = i;
			bHit = true;
			break;
		}
	}

	if(bHit == true && (GetAsyncKeyState(VK_SHIFT)&0xff00)!=0 )
	{
		m_pParent->PostMessage(m_notifyMsg, LM_DOTSPECIAL, (LPARAM)this);
	}
}

void CLineMoveCtrl::OnMouseMove(CPoint point)
{
	if(m_curDot != -1)
	{
		ItemDot& dot = m_dots[m_curDot];
		ItemDot result = TransmitDot(point);
		//TRACE("y: %d, value %f\n",point.y, result.value);
		if(m_curDot > 0)
		{
			if(result.pos < m_dots[m_curDot - 1].pos)
				result.pos = m_dots[m_curDot - 1].pos;
		}
		if(m_curDot < (int)m_dots.size() - 1)
		{
			if(result.pos > m_dots[m_curDot + 1].pos)
				result.pos = m_dots[m_curDot + 1].pos;
		}

		if (result.value > 1.0f) 
		{
			result.value = 1.0f;
		}

		if (result.value < 0.0f) 
		{
			result.value = 0.0f;
		}
		if((dot.flags & DOTFLAG_FIXVALUE) == 0 && (GetAsyncKeyState(VK_CONTROL)&0xff00) == 0 )
		{
			dot.value = result.value;
		}
		if((dot.flags & DOTFLAG_FIXPOS) == 0 && (GetAsyncKeyState(VK_SHIFT)&0xff00) == 0 )
		{
			dot.pos = result.pos;
		}
		if(m_notifyMsg != NULL)
			m_pParent->PostMessage(m_notifyMsg, LM_DOTCHANGED, (LPARAM)this);
		m_pParent->InvalidateRect(m_boundRect, FALSE);
	}
}

void CLineMoveCtrl::OnLButtonDblClk(CPoint pt)
{
	if(m_boundRect.PtInRect(pt) == FALSE)
		return;

	ItemDot result = TransmitDot(pt);
	if(result.pos <= 0 || result.pos >= 1.0f)
		return;

	for(int i = 1; i < (int)m_dots.size() ; i++)
	{
		if(m_dots[i-1].pos < result.pos && m_dots[i].pos >= result.pos)
		{
			m_ParamDot = result;
			m_iParamIndex = i;
			m_pParent->PostMessage(m_notifyMsg, LM_BLANKDBLCLICK, (LPARAM)this);
			break;
		}
	}

}


void CLineMoveCtrl::SetItemPos(int index, float pos)
{
	if(index < 0 || index >= (int)m_dots.size())
		return;

	if((m_dots[index].flags & DOTFLAG_FIXPOS) == 0)
	{
		m_dots[index].pos = pos;
	}
	m_pParent->InvalidateRect(m_boundRect, TRUE);
}

void CLineMoveCtrl::SetItemValue(int index, float value)
{
	if(index < 0 || index >= (int)m_dots.size())
		return;
	if((m_dots[index].flags & DOTFLAG_FIXVALUE) == 0)
	{
		m_dots[index].value = value;
	}
	m_pParent->InvalidateRect(m_boundRect, TRUE);
}

float CLineMoveCtrl::GetItemPos(int index)
{
	if(index < 0 || index >= (int)m_dots.size())
		return -1.0f;
	return m_dots[index].pos;
}

float CLineMoveCtrl::GetItemValue(int index)
{
	if(index < 0 || index >= (int)m_dots.size())
		return -1.0f;
	return m_dots[index].value;
}

float CLineMoveCtrl::GetParamPos()
{
	return m_ParamDot.pos;
}

float CLineMoveCtrl::GetParamValue()
{
	return m_ParamDot.value;
}

int CLineMoveCtrl::GetParamIndex()
{
	return m_iParamIndex;
}