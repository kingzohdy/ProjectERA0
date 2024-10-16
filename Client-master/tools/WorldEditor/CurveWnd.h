#pragma once

#include "OgreInterpCurve.h"
#include "gdiplus.h"

using namespace std;
#pragma comment(lib,"gdiplus.lib")

// 曲线控制点
class CtrlPoint
{
public:
	CtrlPoint (float fInValue, float fOutValue, Ogre::InterpCurveMode InterpMode,
		int iParentWidth, int iParentHeight);
	~CtrlPoint();

	// 尝试选取
	bool TrySelect (CPoint point);
	void Select (bool bSelect);
	bool IsSelected ();

	void Draw (Gdiplus::Graphics &graphics);

public:
	Ogre::InterpCurvePoint<float> *m_pPoint;

protected:
	CtrlPoint ();
	int m_iParentWidth;
	int m_iParentHeight;
	int m_iRectHalfWidth;
	int m_iRectHalfHeight;
	Gdiplus::RectF m_rect;
	bool m_bSelected;
};

// 曲线编辑窗口
class CCurveWnd : public CWnd
{
	DECLARE_DYNAMIC(CCurveWnd)

public:
	CCurveWnd();
	virtual ~CCurveWnd();

	void SetXGridNum (int iNum);
	void SetYGridNum (int iNum);

	// 设置默认增加的关键点的模式
	/*
	在MyOnLButtonDblClk中会增加关键点，使用此函数函数控制输入关键点的模式。
	*/
	void SetDefAddPointMode (Ogre::InterpCurveMode InterpMode);

	// 增加点
	/*
	fInValue的值属于[0,1]
	fOutValue的值属于[0,1]
	一般情况下你无需使用此函数，由机制内部处理，在MyOnLButtonDblClk被调用
	*/
	void AddPoint (float fInValue, float fOutValue, Ogre::InterpCurveMode InterpMode);

	// 获得选择的CtrlPoint
	CtrlPoint *GetSelectedCtrlPoint ();

	// 改变点的模式
	void SetPointMode (CtrlPoint *pPoint, Ogre::InterpCurveMode mode);
	void SetSelectedPointMode (Ogre::InterpCurveMode mode);

	// 移除点
	void RemoveCtrlPoint (CtrlPoint *&pPoint);
	void RemoveSelectedCtrlPoint ();

	// 获得值
	float GetOutValue (float fInValue);

	// 获得曲线
	Ogre::InterpCurve<float> *GetInterpCurve ();

public:

	// 控件无法响应这些函数，你需要由父窗口调用 ////////////////

	// point必须是Sreen的坐标系，在内部会转换成本窗口坐标
	void MyOnLButtonDown (UINT nFlags, CPoint point);

	// point必须是Sreen的坐标系，在内部会转换成本窗口坐标
	void MyOnMouseMove(UINT nFlags, CPoint point);

	// point必须是Sreen的坐标系，在内部会转换成本窗口坐标
	void MyOnLButtonDblClk (UINT nFlags, CPoint point);

	// point必须是Sreen的坐标系，在内部会转换成本窗口坐标
	void MyOnRButtonDblClk(UINT nFlags, CPoint point);

protected:
	void MakePointToNormal (CPoint &point); // 将屏幕点变换到常用的模式
	bool PointInWnd (CPoint point); // 判断输入点是否在控件内，point必须在本窗口坐标系
	void Init();
	void InitDefaultPoint ();
	void DrawBackGrid (Gdiplus::Graphics &graphics);
	void DrawCurve (Gdiplus::Graphics &graphics);
	void DrawCtrlPoint (Gdiplus::Graphics &graphics);
	int m_iWidth;
	int m_iHeight;
	bool m_bInited;

	vector<CtrlPoint*> m_CtrlPointList;
	Ogre::InterpCurve<float> m_Curve;

	int m_iXValueNum;
	int m_iYValueNum;
	Gdiplus::PointF *m_paPointsX;	// 网格上下
	Gdiplus::PointF *m_paPointsY;	// 网格左右
	Gdiplus::PointF m_aPoints[100];	// 曲线

	Ogre::InterpCurveMode m_DefCurveMode;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};