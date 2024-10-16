
#include "stdafx.h"
#include "FixedTabWnd.h"


IMPLEMENT_DYNAMIC(CFixedTabWnd, CBCGPTabWnd)
CFixedTabWnd::CFixedTabWnd()
{
}

CFixedTabWnd::~CFixedTabWnd()
{
}


BEGIN_MESSAGE_MAP(CFixedTabWnd, CBCGPTabWnd)
END_MESSAGE_MAP()


DROPEFFECT CFixedTabWnd::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	return DROPEFFECT_COPY;
}

