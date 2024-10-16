
#pragma once
#include <vector>

#define DOTFLAG_FIXPOS 2
#define DOTFLAG_FIXVALUE 4

#define LCFLAG_FIXPOS_END 1
#define LCFLAG_FIXPOS_ALL 2

#define LCFLAG_FIXVALUE_END 4
#define LCFLAG_FIXVALUE_ALL 8

#define LM_DOTCHANGED 1
#define LM_BLANKDBLCLICK 2
#define LM_DOTSPECIAL 3

class CLineMoveCtrl
{
public:
	CLineMoveCtrl(void);
	~CLineMoveCtrl(void);
	struct ItemDot
	{
		float pos;
		float value;
		unsigned int flags;
	};

	std::vector<ItemDot> m_dots;

	int m_curDot;
	//	int m_maxdots;
	CRect m_ctrlRect;

	CRect m_boundRect;
	ItemDot	m_ParamDot;
	int		m_iParamIndex;

	virtual void Create(int iDotCount, unsigned int notifyMsg,CRect rect, CWnd* pParent,  unsigned int flags = 0);
	CWnd* m_pParent;

	unsigned int m_notifyMsg;
public:
	virtual void SetItemPos(int index, float pos);
	virtual void SetItemValue(int index, float value);

	virtual float GetItemPos(int index);
	virtual float GetItemValue(int index);

	virtual float GetParamPos();
	virtual float GetParamValue();
	virtual int GetParamIndex();

	void OnLBtnDown(CPoint pt);
	void OnLBtnUp(CPoint pt);
	void OnMouseMove(CPoint pt);
	void OnLButtonDblClk(CPoint pt);

	virtual void OnDrawCtrl(CDC* pDc);

	virtual void DrawBorder(CDC* pDc);
	virtual void DrawBackground(CDC* pDc);
	virtual void DrawDots(CDC* pDc);

	CPoint TransmitPoint(ItemDot& dot);
	ItemDot TransmitDot(CPoint& point);
	bool HitTest(CPoint& point, ItemDot& dot);

	void TrimRect(CRect& rc);

};
