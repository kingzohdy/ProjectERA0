
#include "stdafx.h"
#include "Resource.h"
#include "SectionDrawPanel.h"
#include "SectionPointInfoDlg.h"

IMPLEMENT_DYNAMIC(CDrawPanel, CWnd)
CDrawPanel::CDrawPanel()
{
	m_fRatio = 200.0f / 100.0f;
	m_iDotRange = 3;
	m_bShowText = false;
}

CDrawPanel::~CDrawPanel()
{
}


CPoint	CDrawPanel::PosRealToInt(Ogre::Vector2& pos)
{
	CRect clientRect;
	GetClientRect(clientRect);

	CPoint ret;
	ret.x = clientRect.Width() / 2 + int(pos.x * m_fRatio);
	ret.y = clientRect.Height() / 2 - int(pos.y * m_fRatio);
	return ret;
}

Ogre::Vector2 CDrawPanel::PosIntToReal(CPoint pos)
{
	CRect clientRect;
	GetClientRect(clientRect);

	Ogre::Vector2 ret;
	ret.x = (pos.x - clientRect.Width() / 2) / m_fRatio;
	ret.y = (clientRect.Height() / 2 - pos.y) / m_fRatio;

	return ret;
}

int CDrawPanel::HitTest(CPoint pos)
{
	for(int i = 0; i < (int) m_Vertexes.size(); i++)
	{
		CPoint vetPos = PosRealToInt(m_Vertexes[i].pos);
		CRect vetRect(vetPos.x - m_iDotRange, vetPos.y - m_iDotRange, vetPos.x + m_iDotRange, vetPos.y + m_iDotRange);
		if(vetRect.PtInRect(pos) == TRUE)
			return i;
	}
	return -1;
}

int CDrawPanel::FindLineFromDot(int iDot)
{
	for(int i = 0; i < (int)m_Lines.size(); i++)
	{
		LINE_INFO& line = m_Lines[i];
		for(int k = 0; k < (int)line.lineVertex.size(); k++)
		{
			if(line.lineVertex[k] == iDot)
				return i;
		}
	}
	return -1;
}

BEGIN_MESSAGE_MAP(CDrawPanel, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CDrawPanel message handlers

void CDrawPanel::PaintGrid(CDC* pDc)
{
	CRect clientRect;
	GetClientRect(clientRect);
	CPen pen(PS_SOLID, 1, RGB(100, 100, 100));
	CPen* pOldPen = pDc->SelectObject(&pen);

	Ogre::Vector2 left = PosIntToReal(CPoint(0, clientRect.Height() / 2));
	Ogre::Vector2 top = PosIntToReal(CPoint(clientRect.Width() / 2, 0));
	Ogre::Vector2 right = PosIntToReal(CPoint(clientRect.Width(), clientRect.Height() / 2));
	Ogre::Vector2 bottom = PosIntToReal(CPoint(clientRect.Width() / 2, clientRect.Height()));

	float p = 50.0f;
	while(p < top.y)
	{
		CPoint start = PosRealToInt(Ogre::Vector2(left.x, p));
		CPoint end = PosRealToInt(Ogre::Vector2(right.x, p));
		pDc->MoveTo(start);
		pDc->LineTo(end);
		p += 50.0f;
	}

	p = -50.0f;
	while(p > bottom.y)
	{
		CPoint start = PosRealToInt(Ogre::Vector2(left.x, p));
		CPoint end = PosRealToInt(Ogre::Vector2(right.x, p));
		pDc->MoveTo(start);
		pDc->LineTo(end);
		p -= 50.0f;
	}

	p = 50.0f;
	while(p < right.x)
	{
		CPoint start = PosRealToInt(Ogre::Vector2(p, top.y));
		CPoint end = PosRealToInt(Ogre::Vector2(p, bottom.y));
		pDc->MoveTo(start);
		pDc->LineTo(end);
		p += 50.0f;
	}

	p = -50.0f;
	while(p > left.x)
	{
		CPoint start = PosRealToInt(Ogre::Vector2(p, top.y));
		CPoint end = PosRealToInt(Ogre::Vector2(p, bottom.y));
		pDc->MoveTo(start);
		pDc->LineTo(end);
		p -= 50.0f;
	}
	CPen hardPen (PS_SOLID, 1, RGB(200, 200, 200));
	pDc->SelectObject(&hardPen);
	CPoint start = PosRealToInt(Ogre::Vector2( left.x, 0));
	CPoint end = PosRealToInt(Ogre::Vector2(right.x, 0));
	pDc->MoveTo(start);
	pDc->LineTo(end);
	start = PosRealToInt(Ogre::Vector2(0, top.y));
	end = PosRealToInt(Ogre::Vector2(0, bottom.y));
	pDc->MoveTo(start);
	pDc->LineTo(end);

	pDc->SelectObject(pOldPen);

}

void CDrawPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting



	CBrush brGray(RGB(50, 50, 50));
	CPen penWhite(PS_SOLID , 1, RGB(255, 255, 255));
	CBrush brLightGray(RGB(200, 200, 200));
	CRect rect;
	GetClientRect(rect);
	CDC	memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	memDC.SelectObject(&bmp);
	memDC.FillRect(rect, &brGray);

	PaintGrid(&memDC);

	CFont font;
	font.CreatePointFont(90, _T("宋体"));
	CFont* poldfont = (CFont*)memDC.SelectObject(&font);
	COLORREF oldclr = memDC.GetTextColor();

	memDC.SetBkMode(TRANSPARENT);
	memDC.SetTextColor(RGB(255, 100, 100));

	memDC.SelectObject(&penWhite);

	for(int i = 0; i < (int)m_Lines.size(); i++)
	{
		LINE_INFO& currLine = m_Lines[i];

		for(int k = 0; k < (int)currLine.lineVertex.size(); k++)
		{
			int iIndex = currLine.lineVertex[k];

			CPoint pt = PosRealToInt(m_Vertexes[iIndex].pos);
			if(k == 0)
				memDC.MoveTo(pt);
			else
				memDC.LineTo(pt);			
		}
	}

	for(int i = 0; i < (int)m_Vertexes.size(); i++)
	{
		CPoint point =  PosRealToInt(m_Vertexes[i].pos);
		CRect dotRect(point.x - m_iDotRange, point.y - m_iDotRange, point.x + m_iDotRange, point.y + m_iDotRange);
		if(m_Vertexes[i].selected == false)
			memDC.SelectObject(&brGray);//dc.FillRect(dotRect, &brGray);
		else
			memDC.SelectObject(&brLightGray);
		memDC.Rectangle(dotRect);
		if(m_bShowText == true)
		{
			CRect textRect;
			textRect.left = point.x + 6;
			textRect.right = textRect.left + 200;
			textRect.top = point.y - 6;
			textRect.bottom = point.y + 6;
			CString strText;
			strText.Format("%.2f (%.2f, %.2f)", m_Vertexes[i].texV, m_Vertexes[i].pos.x, m_Vertexes[i].pos.y);
			memDC.DrawTextEx(strText , textRect, DT_WORD_ELLIPSIS | DT_LEFT |DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE, NULL);

		}
	}

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
}

void CDrawPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	for(int i = 0; i < (int)m_Vertexes.size(); i++)
	{
		m_Vertexes[i].selected = false;
	}
	int iHit = HitTest(point);


	if((GetAsyncKeyState(VK_CONTROL)&0xff00) != 0)
	{
		if(iHit < 0)
		{
			LINE_INFO li;
			VERTEX_INFO vi;
			vi.pos = PosIntToReal(point);
			vi.texV = 1.0f;
			vi.selected = false;

			int iVertex = (int) m_Vertexes.size();
			m_Vertexes.push_back(vi);
			li.lineVertex.push_back(iVertex);

			iHit = iVertex;
			m_Lines.push_back(li);

		}

		if(iHit >= 0)
		{
			int iLine = FindLineFromDot(iHit);
			if(iLine < 0 || iLine >= (int)m_Lines.size())
				return;
			int iCount = (int)m_Lines[iLine].lineVertex.size();

			//头
			if(iHit == m_Lines[iLine].lineVertex[0])
			{
				VERTEX_INFO vExtFrom = m_Vertexes[m_Lines[iLine].lineVertex[0]];
				VERTEX_INFO vertex;
				vertex.pos = vExtFrom.pos;
				vertex.texV = vExtFrom.texV;
				vertex.selected = true;
				m_Vertexes.push_back(vertex);
				int iIndex = (int)m_Vertexes.size() - 1;
				m_Lines[iLine].lineVertex.insert(m_Lines[iLine].lineVertex.begin(), iIndex);
				DoDotPos(iIndex);
				return;
			}
			//尾
			else if(iHit == m_Lines[iLine].lineVertex[iCount-1])
			{
				VERTEX_INFO vExtFrom = m_Vertexes[m_Lines[iLine].lineVertex[0]];
				VERTEX_INFO vertex;
				vertex.pos = vExtFrom.pos;
				vertex.texV = vExtFrom.texV;
				vertex.selected = true;
				m_Vertexes.push_back(vertex);
				int iIndex = (int)m_Vertexes.size() - 1;
				m_Lines[iLine].lineVertex.push_back(iIndex);
				DoDotPos(iIndex);
				return;
			}
			//中间
			for(int k = 1; k < (int)m_Lines[iLine].lineVertex.size() - 1; k++)
			{

				if(iHit == m_Lines[iLine].lineVertex[k])
				{
					VERTEX_INFO vExtFrom = m_Vertexes[m_Lines[iLine].lineVertex[k]];
					VERTEX_INFO vertex;
					vertex.pos = vExtFrom.pos;
					vertex.texV = vExtFrom.texV;
					vertex.selected = true;
					m_Vertexes.push_back(vertex);
					int iIndex = (int)m_Vertexes.size() - 1;
					m_Lines[iLine].lineVertex.insert(m_Lines[iLine].lineVertex.begin() + k, iIndex);
					DoDotPos(iIndex);
					return;
				}
			}


		}
	}
	else
	{
		if(iHit >= 0 && iHit <(int) m_Vertexes.size())
		{
			m_Vertexes[iHit].selected = true;
			DoDotPos(iHit);
			Invalidate(FALSE);
		}
	}


	CWnd::OnMButtonDown(nFlags, point);
}


void CDrawPanel::DoDotPos(int iDot)
{
	::MSG msg;
	CPoint pos;
	bool bExit = false;

	Ogre::Vector2 oldPos = m_Vertexes[iDot].pos;
	while(!bExit && ::GetMessage(&msg, NULL, 0, 0) )
	{
		TranslateMessage(&msg);
		switch(msg.message) 
		{
		case WM_MOUSEMOVE: 
			if(msg.hwnd == GetSafeHwnd())
			{
				//pos.x = LOWORD(msg.lParam);
				//pos.y = HIWORD(msg.lParam);
				POINT tmp;
				GetCursorPos(&tmp);

				//tmp.x = pos.x;
				//tmp.y = pos.y;
				ScreenToClient(&tmp);

				Ogre::Vector2 realPos = PosIntToReal(CPoint(tmp));

				m_Vertexes[iDot].pos = realPos;

				CPoint pt = PosRealToInt(realPos);

				Invalidate(FALSE);
			}
			break;
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
			bExit = true;
			break;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			m_Vertexes[iDot].pos = oldPos;
			bExit = true;
			Invalidate(FALSE);
			break;
		default:
			DispatchMessage(&msg);
			break;
		}
	}

	return ;
}

void CDrawPanel::DeleteLineDot(int iDot)
{
	if(iDot >= 0 && iDot < (int)m_Vertexes.size())
	{
		m_Vertexes.erase(m_Vertexes.begin() + iDot);
	}

	for(int i = 0; i < (int)m_Lines.size(); i++)
	{
		LINE_INFO& line = m_Lines[i];
		for(int k = 0; k < (int)line.lineVertex.size(); k++)
		{
			int iIndex = line.lineVertex[k];
			if(iIndex == iDot)
			{
				line.lineVertex[k] = -1;
			}
			else if(iIndex > iDot)
			{
				line.lineVertex[k]--;
			}
		}
	}

	for(int i = 0; i < (int)m_Lines.size(); i++)
	{
		LINE_INFO& line = m_Lines[i];
		bool bClear = false;
		while(bClear == false)
		{
			bClear = true;
			for(int k = 0; k < (int)line.lineVertex.size(); k++)
			{
				int iIndex = line.lineVertex[k];
				if(iIndex < 0)
				{
					bClear = false;
					line.lineVertex.erase(line.lineVertex.begin() + k);
					break;
				}
			}
		}
	}
}

void CDrawPanel::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if(nChar == VK_DELETE)
	{
		for(int i = 0; i < (int)m_Vertexes.size(); i++)
		{
			if(m_Vertexes[i].selected == true)
			{
				DeleteLineDot(i);
				break;
			}
		}
		bool bClear = false;
		while(bClear == false)
		{
			bClear = true;
			for(int i = 0; i < (int)m_Lines.size(); i++)
			{
				LINE_INFO& line = m_Lines[i];
				if(line.lineVertex.size() == 1)
				{
					bClear = false;
					DeleteLineDot(line.lineVertex[0]);
					break;
				}
			}
		}
		Invalidate(FALSE);
	}
	else if(nChar == 'T' || nChar == 't')
	{
		m_bShowText = !m_bShowText;
		Invalidate(FALSE);
	}
	//CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDrawPanel::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	int iHit = HitTest(point);
	if(iHit >= 0 && iHit < (int)m_Vertexes.size())
	{
		CSectionPointInfoDlg dlg;
		dlg.m_fPosX = m_Vertexes[iHit].pos.x;
		dlg.m_fPosY = m_Vertexes[iHit].pos.y;
		dlg.m_fTexV = m_Vertexes[iHit].texV;

		if(IDOK == dlg.DoModal())
		{
			m_Vertexes[iHit].pos.x = dlg.m_fPosX;
			m_Vertexes[iHit].pos.y = dlg.m_fPosY;
			m_Vertexes[iHit].texV = dlg.m_fTexV;
			Invalidate(FALSE);
		}
	}
	CWnd::OnLButtonDblClk(nFlags, point);
}
