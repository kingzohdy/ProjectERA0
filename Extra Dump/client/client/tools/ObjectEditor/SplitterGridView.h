
#pragma once

class CSplitterGridView : public CWnd
{
	DECLARE_DYNCREATE(CSplitterGridView)

public:
	CSplitterGridView();           // protected constructor used by dynamic creation
	virtual ~CSplitterGridView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


	void SetInnerCtrl(CWnd* pCtrl) { m_pInnerCtrl = pCtrl; };
	CWnd* GetInnerCtrl() { return m_pInnerCtrl; };
	void UpdateInnerCtrl();

protected:

	CWnd* m_pInnerCtrl;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnDestroy();
};


