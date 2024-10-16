#pragma once

#include "OgreInterpCurve.h"
#include "gdiplus.h"

using namespace std;
#pragma comment(lib,"gdiplus.lib")

// ���߿��Ƶ�
class CtrlPoint
{
public:
	CtrlPoint (float fInValue, float fOutValue, Ogre::InterpCurveMode InterpMode,
		int iParentWidth, int iParentHeight);
	~CtrlPoint();

	// ����ѡȡ
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

// ���߱༭����
class CCurveWnd : public CWnd
{
	DECLARE_DYNAMIC(CCurveWnd)

public:
	CCurveWnd();
	virtual ~CCurveWnd();

	void SetXGridNum (int iNum);
	void SetYGridNum (int iNum);

	// ����Ĭ�����ӵĹؼ����ģʽ
	/*
	��MyOnLButtonDblClk�л����ӹؼ��㣬ʹ�ô˺���������������ؼ����ģʽ��
	*/
	void SetDefAddPointMode (Ogre::InterpCurveMode InterpMode);

	// ���ӵ�
	/*
	fInValue��ֵ����[0,1]
	fOutValue��ֵ����[0,1]
	һ�������������ʹ�ô˺������ɻ����ڲ�������MyOnLButtonDblClk������
	*/
	void AddPoint (float fInValue, float fOutValue, Ogre::InterpCurveMode InterpMode);

	// ���ѡ���CtrlPoint
	CtrlPoint *GetSelectedCtrlPoint ();

	// �ı���ģʽ
	void SetPointMode (CtrlPoint *pPoint, Ogre::InterpCurveMode mode);
	void SetSelectedPointMode (Ogre::InterpCurveMode mode);

	// �Ƴ���
	void RemoveCtrlPoint (CtrlPoint *&pPoint);
	void RemoveSelectedCtrlPoint ();

	// ���ֵ
	float GetOutValue (float fInValue);

	// �������
	Ogre::InterpCurve<float> *GetInterpCurve ();

public:

	// �ؼ��޷���Ӧ��Щ����������Ҫ�ɸ����ڵ��� ////////////////

	// point������Sreen������ϵ�����ڲ���ת���ɱ���������
	void MyOnLButtonDown (UINT nFlags, CPoint point);

	// point������Sreen������ϵ�����ڲ���ת���ɱ���������
	void MyOnMouseMove(UINT nFlags, CPoint point);

	// point������Sreen������ϵ�����ڲ���ת���ɱ���������
	void MyOnLButtonDblClk (UINT nFlags, CPoint point);

	// point������Sreen������ϵ�����ڲ���ת���ɱ���������
	void MyOnRButtonDblClk(UINT nFlags, CPoint point);

protected:
	void MakePointToNormal (CPoint &point); // ����Ļ��任�����õ�ģʽ
	bool PointInWnd (CPoint point); // �ж�������Ƿ��ڿؼ��ڣ�point�����ڱ���������ϵ
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
	Gdiplus::PointF *m_paPointsX;	// ��������
	Gdiplus::PointF *m_paPointsY;	// ��������
	Gdiplus::PointF m_aPoints[100];	// ����

	Ogre::InterpCurveMode m_DefCurveMode;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};