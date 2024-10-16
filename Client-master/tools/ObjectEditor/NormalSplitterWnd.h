
#pragma once

class CNormalSplitterWnd : public CBCGPSplitterWnd
{
public:
	CNormalSplitterWnd(void);
	~CNormalSplitterWnd(void);

	virtual void StartTracking(int ht); 
	virtual CWnd* GetActivePane(int* pRow = NULL, int* pCol  = NULL); 
	virtual void SetActivePane( int row, int col, CWnd* pWnd  = NULL ); 
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam); 
	virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult ); 
	virtual BOOL OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult ); 

};
