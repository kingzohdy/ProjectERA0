
#pragma once
#include "LineMoveCtrl.h"

class CColorMoveCtrl : public CLineMoveCtrl
{
public:
	CColorMoveCtrl(void);
	~CColorMoveCtrl(void);

	struct ColorDot
	{
		float pos;
		COLORREF color;
	};
	void SetItemColor(int index, COLORREF clr);
	virtual void SetItemPos(int index, float pos);
	//virtual void OnDrawCtrl(CDC* pDc);

	virtual void DrawBackground(CDC* pDc);
	virtual void Create(int iDotCount, unsigned int notifyMsg, CRect rect,  CWnd* pParent);
	void OnDblClick(CPoint point);
	std::vector<ColorDot> m_colorDots;

	COLORREF GetItemColor(int index);
};
