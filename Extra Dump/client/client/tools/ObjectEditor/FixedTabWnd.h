
#pragma once

class CFixedTabWnd : public CBCGPTabWnd
{
	DECLARE_DYNAMIC(CFixedTabWnd)

public:
	CFixedTabWnd();
	virtual ~CFixedTabWnd();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);

protected:
	DECLARE_MESSAGE_MAP()
};


