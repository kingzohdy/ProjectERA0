
#pragma once
#include "OgreVector2.h"


// CDrawPanel

struct VERTEX_INFO
{
	Ogre::Vector2	pos;
	float			texV;
	bool			selected;
};

struct LINE_INFO
{
	std::vector<int>	lineVertex;
};

class CDrawPanel : public CWnd
{
	DECLARE_DYNAMIC(CDrawPanel)

public:
	CDrawPanel();
	virtual ~CDrawPanel();

	float	m_fRatio;
	int		m_iDotRange;
	bool	m_bShowText;

	std::vector<VERTEX_INFO> m_Vertexes;
	std::vector<LINE_INFO>	 m_Lines;

	CPoint	PosRealToInt(Ogre::Vector2& pos);
	Ogre::Vector2 PosIntToReal(CPoint pos);

	int HitTest(CPoint pos);
	void DoDotPos(int iDot);

	int FindLineFromDot(int iDot);
	void DeleteLineDot(int iDot);

protected:
	DECLARE_MESSAGE_MAP()
public:
	void		PaintGrid(CDC* pDc);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


