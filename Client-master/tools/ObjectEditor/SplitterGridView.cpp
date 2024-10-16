
#include "stdafx.h"
#include "SplitterGridView.h"


// CSplitterGridView

IMPLEMENT_DYNCREATE(CSplitterGridView, CWnd)

CSplitterGridView::CSplitterGridView() : m_pInnerCtrl(NULL)
{
}

CSplitterGridView::~CSplitterGridView()
{
}

BEGIN_MESSAGE_MAP(CSplitterGridView, CWnd)
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CSplitterGridView drawing

void CSplitterGridView::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
	//// TODO: add draw code here
	//pDC->TextOut(0, 0, CString(_T("a pane with no control")));
}


// CSplitterGridView diagnostics

#ifdef _DEBUG
void CSplitterGridView::AssertValid() const
{
	CWnd::AssertValid();
}

void CSplitterGridView::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);
}
#endif //_DEBUG


// CSplitterGridView message handlers

void CSplitterGridView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if(m_pInnerCtrl != NULL)
	{
		CRect rect;
		GetClientRect(rect);
		m_pInnerCtrl->SetWindowPos(NULL, 0, 0, 
			rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER);
	}
}

void CSplitterGridView::UpdateInnerCtrl()
{
	if(m_pInnerCtrl->GetSafeHwnd() && GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);
		m_pInnerCtrl->SetWindowPos(NULL, 0, 0, 
			rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER);
	}
}

BOOL CSplitterGridView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	GetParent()->SendMessage(WM_NOTIFY, wParam, lParam);
	return CWnd::OnNotify(wParam, lParam, pResult);
}

void CSplitterGridView::OnDestroy()
{
	CWnd::OnDestroy();
	delete this;
	// TODO: Add your message handler code here
}
