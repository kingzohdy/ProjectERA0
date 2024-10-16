
#include "stdafx.h"
#include "NormalSplitterWnd.h"

CNormalSplitterWnd::CNormalSplitterWnd(void)
{
}

CNormalSplitterWnd::~CNormalSplitterWnd(void)
{
}


enum HitTestvalue 
{ 
	noHit = 0,//��ʾû��ѡ���κζ��� 
	vSplitterBox = 1, 
	hSplitterBox = 2, 
	bothSplitterBox = 3, 
	vSplitterBar1 = 101,//������������ˮƽ�ָ��� 
	vSplitterBar15 = 115, 
	hSplitterBar1 = 201,//����ֱ����ĸ����ָ��� 
	hSplitterBar15 = 215, 
	splitterIntersection1 = 301,//������������ 
	splitterIntersection225 = 525 
}; 

CWnd* CNormalSplitterWnd::GetActivePane(int* pRow, int* pCol) 
{ 
	ASSERT_VALID(this); 
	//��õ�ǰ�Ļ�ý���Ĵ��� 
	//����ע�ʹ������ԭ�еĴ������Ҫ���֡� 
	// CWnd* pView = NULL; 
	//CFrameWnd* pFrameWnd = GetParentFrame(); 
	//ASSERT_VALID(pFrameWnd); 
	//pView = pFrameWnd->GetActiveView(); 
	//if (pView == NULL) 
	// pView = GetFocus(); 
	CWnd* pView = GetFocus(); 
	if (pView != NULL && !IsChildPane(pView, pRow, pCol)) 
		pView = NULL; 
	return pView; 
} 

void CNormalSplitterWnd::SetActivePane( int row, int col, CWnd* pWnd) 
{ 
	CWnd* pPane = pWnd == NULL ? GetPane(row, col) : pWnd; 
	//�����ע�ʹ������ԭ�д������Ҫ���֡� 
	//FrameWnd* pFrameWnd = GetParentFrame(); 
	//ASSERT_VALID(pFrameWnd); 
	//pFrameWnd->SetActiveView((CView*)pPane); 
	pPane->SetFocus();//�޸ĺ����� 
} 

void CNormalSplitterWnd::StartTracking(int ht) 
{ 
	ASSERT_VALID(this); 
	if (ht == noHit) 
		return; 
	// GetHitRect will restrict ��������m_rectLimit�������� as appropriate 

	GetInsideRect(m_rectLimit); 
	if (ht >= splitterIntersection1 && ht <= splitterIntersection225) 

	{ 
		// split two directions (two tracking rectangles) 

		int row = (ht - splitterIntersection1) / 15; 

		int col = (ht - splitterIntersection1) % 15; 

		GetHitRect(row + vSplitterBar1, m_rectTracker); 

		int yTrackOffset = m_ptTrackOffset.y; 
		m_bTracking2 = TRUE; 
		GetHitRect(col + hSplitterBar1, m_rectTracker2); 

		m_ptTrackOffset.y = yTrackOffset; 
	} 
	else if (ht == bothSplitterBox) 
	{ 
		// hit on splitter boxes (for keyboard) 
		GetHitRect(vSplitterBox, m_rectTracker); 
		int yTrackOffset = m_ptTrackOffset.y; 
		m_bTracking2 = TRUE; 
		GetHitRect(hSplitterBox, m_rectTracker2); 
		m_ptTrackOffset.y = yTrackOffset; // center it 
		m_rectTracker.OffsetRect(0, m_rectLimit.Height()/2); m_rectTracker2.OffsetRect(m_rectLimit.Width()/2, 
			0); 
	} 
	else 
	{ 
		// only hit one bar 
		GetHitRect(ht, m_rectTracker); 
	} 

	//�����ע�͵Ľ��ӳ�����ɾȥ�� 
	//CView* pView = (CView*)GetActivePane(); 
	//if (pView != NULL && pView->IsKindOf(RUNTIME_CLASS(CView))) 
	//{ 
	// ASSERT_VALID(pView); 
	// CFrameWnd* pFrameWnd = GetParentFrame(); 
	//ASSERT_VALID(pFrameWnd); 
	//pView->OnActivateFrame(WA_INACTIVE, pFrameWnd); 
	// } 
	// steal focus and capture 
	SetCapture(); 
	SetFocus(); 
	// make sure no updates are pending 
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW); 

	// set tracking state and appropriate cursor 
	m_bTracking = TRUE; 
	OnInvertTracker(m_rectTracker); 
	if (m_bTracking2) 
		OnInvertTracker(m_rectTracker2); 
	m_htTrack = ht; 
	SetSplitCursor(ht); 
} 

BOOL CNormalSplitterWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{ 
	if (CWnd::OnCommand(wParam, lParam)) 
		return TRUE; 
	//����������ԭ�������� 
	//return GetParentFrame()->SendMessage(WM_COMMAND, wParam, lParam); 

	return (BOOL)GetParent()->SendMessage(WM_COMMAND, wParam, lParam); 

} 
BOOL CNormalSplitterWnd::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult ) 
{ 
	//if (CWnd::OnNotify(wParam, lParam, pResult)) 
	//	return TRUE; 
	//����������Դ�������� 
	//*pResult = GetParentFrame()->SendMessage(WM_NOTIFY, wParam, lParam); 
	*pResult = GetParent()->SendMessage(WM_NOTIFY, wParam, lParam); 
	return TRUE; 
} 

BOOL CNormalSplitterWnd::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{ 
	// The code line below is necessary if using CxSplitterWnd in a regular dll 
	// AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	return CWnd::OnWndMsg(message, wParam, lParam, pResult); 

} 
