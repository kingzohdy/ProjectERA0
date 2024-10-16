#pragma once


// CChildView

class CChildView : public CWnd
{
	DECLARE_DYNAMIC(CChildView)

public:
	CChildView();
	virtual ~CChildView();
	
	void OnInit();
protected:
	DECLARE_MESSAGE_MAP()
	
public:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	
};

