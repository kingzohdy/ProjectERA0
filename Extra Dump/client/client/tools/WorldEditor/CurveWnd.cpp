// CurveWnd.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "CurveWnd.h"

using namespace Gdiplus;
using namespace Ogre;

CtrlPoint::CtrlPoint (float fInValue, float fOutValue, InterpCurveMode InterpMode,
					  int iParentWidth, int iParentHeight)
{
	m_iRectHalfWidth = 3;
	m_iRectHalfHeight = 3;
	m_iParentWidth = iParentWidth;
	m_iParentHeight = iParentHeight;
	m_bSelected = false;

	m_pPoint = new InterpCurvePoint<float>;
	m_pPoint->fInVal = fInValue;
	m_pPoint->TOutVal = fOutValue;
	m_pPoint->TArriveTangent = 0.2f;
	m_pPoint->TLeaveTangent = 0.2f;
	m_pPoint->InterpMode = InterpMode;

	int iXPos = m_pPoint->fInVal * (float)m_iParentWidth;
	int iYPos = m_pPoint->TOutVal * (float)m_iParentHeight;

	m_rect = Gdiplus::RectF(
		iXPos-m_iRectHalfWidth,
		iYPos-m_iRectHalfWidth,
		m_iRectHalfWidth*2,
		m_iRectHalfWidth*2);
}

CtrlPoint::~CtrlPoint()
{
	if (m_pPoint)
		delete m_pPoint;

	m_pPoint = NULL;
}

bool CtrlPoint::TrySelect (CPoint point)
{
	float fX = (float)point.x;
	float fY = (float)point.y;

	if (m_rect.Contains(fX,fY))
	{
		Select(true);

		return true;
	}
	else
	{
		Select(false);

		return false;
	}

	return false;
}

void CtrlPoint::Select (bool bSelect)
{
	m_bSelected = bSelect;
}

bool CtrlPoint::IsSelected ()
{
	return m_bSelected;
}

void CtrlPoint::Draw (Gdiplus::Graphics &graphics)
{
	GraphicsPath path;

	int iXPos = m_pPoint->fInVal * (float)m_iParentWidth;
	int iYPos = m_pPoint->TOutVal * (float)m_iParentHeight;

	m_rect = Gdiplus::RectF( iXPos-m_iRectHalfWidth,
		iYPos-m_iRectHalfWidth,
		m_iRectHalfWidth*2,
		m_iRectHalfWidth*2 );

	path.AddRectangle(m_rect);

	if (m_bSelected)
	{
		graphics.DrawPath(&Pen(Color(255,0,0)),&path);
	}
	else
	{
		graphics.DrawPath(&Pen(Color(0,255,0)),&path);
	}
}

// CCurveWnd

IMPLEMENT_DYNAMIC(CCurveWnd, CWnd)

CCurveWnd::CCurveWnd()
{
	m_bInited		= false;
	m_paPointsX		= 0;
	m_paPointsY		= 0;
	m_iXValueNum	= 11;
	m_iYValueNum	= 11;
	m_DefCurveMode	= CIM_Curve;
}

CCurveWnd::~CCurveWnd()
{
	if (m_paPointsX)
		delete m_paPointsX;
	m_paPointsX = 0;

	if (m_paPointsY)
		delete m_paPointsY;
	m_paPointsY = 0;

	for (int i=0; i<(int)m_CtrlPointList.size(); i++)
	{
		delete m_CtrlPointList[i];
	}

	m_CtrlPointList.clear();
}

BEGIN_MESSAGE_MAP(CCurveWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CCurveWnd message handlers
void CCurveWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages

	if (!m_bInited)
	{
		Init();

		AddPoint(0.0f,1.0f,CIM_Curve);
		AddPoint(1.0f,0.0f,CIM_Curve);

		m_bInited = true;

		this->Invalidate();
	}

	// 创建内存DC
	CDC memdc;
	memdc.CreateCompatibleDC(&dc);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc,m_iWidth,m_iHeight);
	memdc.SelectObject(&bmp);
	bmp.DeleteObject();

	// 创建GDI+画图对象
	Graphics graphics(memdc);

	Matrix mat;
	mat.Scale(1.0f,-1.0f);
	mat.Translate(0.0f,-(float)m_iHeight);
	graphics.SetTransform(&mat);

	graphics.Clear(Color(100,100,100));

	DrawBackGrid(graphics);
	DrawCurve(graphics);
	DrawCtrlPoint(graphics);

	dc.BitBlt(0,0,m_iWidth,m_iHeight,&memdc,0,0,SRCCOPY);

	memdc.DeleteDC();
}

void CCurveWnd::SetXGridNum (int iNum)
{
	m_iXValueNum = iNum + 1;

	if (m_paPointsX)
	{
		delete []m_paPointsX;
	}

	m_paPointsX = new PointF[m_iXValueNum*2];

	float fXStep = (float)m_iWidth/(float)(m_iXValueNum-1);

	int k=0;
	for (int i=0; i<m_iXValueNum; i++)
	{
		if ((k%2) == 0)
		{
			m_paPointsX[i*2].X = (float)i*fXStep;
			m_paPointsX[i*2].Y = 0;
			m_paPointsX[i*2+1].X = (float)i*fXStep;
			m_paPointsX[i*2+1].Y = (float)m_iHeight;
		}
		else
		{
			m_paPointsX[i*2+1].X = (float)i*fXStep;
			m_paPointsX[i*2+1].Y = 0;
			m_paPointsX[i*2].X = (float)i*fXStep;
			m_paPointsX[i*2].Y = (float)m_iHeight;
		}

		k++;
	}
}

void CCurveWnd::SetYGridNum (int iNum)
{
	m_iYValueNum = iNum + 1;

	if (m_paPointsY)
	{
		delete []m_paPointsY;
	}

	m_paPointsY = new PointF[m_iYValueNum*2];

	float fYStep = (float)m_iHeight/(float)(m_iYValueNum-1);
	
	int k=0;
	for (int i=0; i<m_iYValueNum; i++)
	{
		if ((k%2) == 0)
		{
			m_paPointsY[i*2].X = 0;
			m_paPointsY[i*2].Y = (float)i*fYStep;
			m_paPointsY[i*2+1].X = (float)m_iWidth;
			m_paPointsY[i*2+1].Y = (float)i*fYStep;
		}
		else
		{
			m_paPointsY[i*2+1].X = 0;
			m_paPointsY[i*2+1].Y = (float)i*fYStep;
			m_paPointsY[i*2].X = (float)m_iWidth;
			m_paPointsY[i*2].Y = (float)i*fYStep;
		}

		k++;
	}
}

void CCurveWnd::SetDefAddPointMode (InterpCurveMode InterpMode)
{
	m_DefCurveMode = InterpMode;
}

void CCurveWnd::AddPoint (float fInValue, float fOutValue, InterpCurveMode InterpMode)
{
	CtrlPoint *pNewPoint = new CtrlPoint(fInValue,fOutValue,InterpMode,m_iWidth,m_iHeight);

	pNewPoint->Select(true);
	m_Curve.InsertPoint(pNewPoint->m_pPoint);

	m_CtrlPointList.push_back(pNewPoint);
}

CtrlPoint *CCurveWnd::GetSelectedCtrlPoint ()
{
	for (int i=0; i<(int)m_CtrlPointList.size(); i++)
	{
		if (m_CtrlPointList[i]->IsSelected())
			return m_CtrlPointList[i];
	}

	return NULL;
}

void CCurveWnd::SetPointMode (CtrlPoint *pPoint, InterpCurveMode mode)
{
	pPoint->m_pPoint->InterpMode = mode;

	this->Invalidate();
}

void CCurveWnd::SetSelectedPointMode (InterpCurveMode mode)
{
	CtrlPoint *pCtrlPoint = GetSelectedCtrlPoint();

	if (pCtrlPoint)
	{
		SetPointMode(pCtrlPoint,mode);
	}
}

void CCurveWnd::RemoveCtrlPoint (CtrlPoint *&pPoint)
{
	m_Curve.RemovePoint(pPoint->m_pPoint);

	vector<CtrlPoint*>::iterator it = m_CtrlPointList.begin();
	vector<CtrlPoint*>::iterator it_end = m_CtrlPointList.end();

	for (it; it!=it_end; it++)
	{
		if ((*it) == pPoint)
		{
			m_CtrlPointList.erase(it);

			delete pPoint;
			pPoint = NULL;

			return;
		}
	}
}

void CCurveWnd::RemoveSelectedCtrlPoint ()
{
	CtrlPoint *pCtrlPoint = GetSelectedCtrlPoint();

	if (pCtrlPoint)
	{
		RemoveCtrlPoint(pCtrlPoint);
	}
}

float CCurveWnd::GetOutValue (float fInValue)
{
	float fDefaultValue = 0.0f;
	float fValue = 0.0f;
	fValue = m_Curve.Evaluate(fInValue,fDefaultValue,0);
	return fValue;
}

InterpCurve<float> *CCurveWnd::GetInterpCurve ()
{
	return &m_Curve;
}

void CCurveWnd::MyOnLButtonDown (UINT nFlags, CPoint point)
{
	ScreenToClient(&point);
	if (!PointInWnd(point))
		return;
	MakePointToNormal(point);

	// 现将所有设为不选择
	for (int i=0; i<(int)m_CtrlPointList.size(); i++)
	{
		m_CtrlPointList[i]->Select(false);
	}

	this->Invalidate();

	// 确保只有一个控制点会被选中，即使控制点位置重合
	for (int i=0; i<(int)m_CtrlPointList.size(); i++)
	{
		if (m_CtrlPointList[i]->TrySelect(point))
		{
			// 设置剩下的不被选择
			if (i != ((int)m_CtrlPointList.size()-1))
			{
				int j=i+1;
				for (j;j<(int)m_CtrlPointList.size(); j++)
				{
					m_CtrlPointList[j]->Select(false);
				}
			}

			this->Invalidate();

			return;
		}
	}
}

void CCurveWnd::MyOnMouseMove(UINT nFlags, CPoint point)
{
	ScreenToClient(&point);
	if (!PointInWnd(point))
		return;
	MakePointToNormal(point);

	if (nFlags == MK_LBUTTON)
	{
		CtrlPoint *pCtrlPoint = GetSelectedCtrlPoint();

		if (pCtrlPoint)
		{
			float fInValue = (float)point.x / (float)m_iWidth;
			float fOutValue = (float)point.y / (float)m_iHeight;

			pCtrlPoint->m_pPoint->fInVal = fInValue;
			pCtrlPoint->m_pPoint->TOutVal = fOutValue;
			
			this->Invalidate();
		}

		m_Curve.Update ();
	}
}

void CCurveWnd::MyOnLButtonDblClk (UINT nFlags, CPoint point)
{
	ScreenToClient(&point);
	if (!PointInWnd(point))
		return;
	MakePointToNormal(point);

	float fInValue = (float)point.x / (float)m_iWidth;
	float fOutValue = (float)point.y / (float)m_iHeight;
	AddPoint(fInValue,fOutValue,m_DefCurveMode);

	this->Invalidate();
}

void CCurveWnd::MyOnRButtonDblClk(UINT nFlags, CPoint point)
{
	ScreenToClient(&point);
	if (!PointInWnd(point))
		return;

	RemoveSelectedCtrlPoint();

	this->Invalidate();
}

void CCurveWnd::MakePointToNormal (CPoint &point)
{
	point.y = m_iHeight-point.y;
}

bool CCurveWnd::PointInWnd (CPoint point)
{
	if (point.x<m_iWidth && point.y<m_iHeight &&
		point.x>0 && point.y>0)
	{
		return true;
	}
	else
		return false;
}

void CCurveWnd::Init()
{
	CRect rc;
	GetClientRect(&rc);
	m_iWidth = rc.Width();
	m_iHeight = rc.Height();

	SetXGridNum(m_iXValueNum-1);
	SetYGridNum(m_iYValueNum-1);
}

void CCurveWnd::DrawBackGrid (Gdiplus::Graphics &graphics)
{
	GraphicsPath pathX;
	GraphicsPath pathY;

	pathX.AddLines(m_paPointsX,m_iXValueNum*2);
	pathY.AddLines(m_paPointsY,m_iYValueNum*2);

	graphics.DrawPath(&Pen(Color(150,150,150)),&pathX);
	graphics.DrawPath(&Pen(Color(150,150,150)),&pathY);
}

void CCurveWnd::DrawCurve (Graphics &graphics)
{
	GraphicsPath path;

	if (m_Curve.GetNumPoints() == 0)
		return;
	else
	{
		float fDiff = m_Curve.GetMaxInValue() - m_Curve.GetMinInValue();

		for (int i=0; i<100; i++)
		{
			float fDelta = (float)i / 100.0f;

			float fInValue = m_Curve.GetMinInValue() + fDiff*fDelta;

			m_aPoints[i].X = fInValue*(float)m_iWidth;

			float fDefOutValue = 0.0f;
			float fOutValue = m_Curve.Evaluate(fInValue,fDefOutValue,0);
			m_aPoints[i].Y = fOutValue * (float)m_iHeight;
		}
	}

	path.AddLines(m_aPoints,100);
	graphics.DrawPath(&Pen(Color(255,0,0)),&path);
}

void CCurveWnd::DrawCtrlPoint (Graphics &graphics)
{
	for (int i=0; i<(int)m_CtrlPointList.size(); i++)
	{
		m_CtrlPointList[i]->Draw(graphics);
	}
}

void CCurveWnd::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnClose();
}

int CCurveWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}
