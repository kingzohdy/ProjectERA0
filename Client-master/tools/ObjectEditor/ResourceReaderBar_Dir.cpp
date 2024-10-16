
#include "stdafx.h"
#include "ResourceReaderBar_Dir.h"

IMPLEMENT_DYNAMIC(CChunkDirBar, CWnd)
CChunkDirBar::CChunkDirBar()
{
}

CChunkDirBar::~CChunkDirBar()
{
}


BEGIN_MESSAGE_MAP(CChunkDirBar, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CChunkDirBar message handlers


void CChunkDirBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rt;
	GetClientRect((LPRECT)rt);

	CBrush br(RGB(0, 0, 0));
	dc.FillRect(rt, &br);

	CDC memdc;
	memdc.CreateCompatibleDC(&dc);

	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP_LOGO_ICON);
	memdc.SelectObject(&bmp);

	dc.BitBlt((rt.Width() - 64) / 2, (rt.Height() - 64) / 2
		, 64, 64, &memdc, 0, 0, SRCCOPY);
}
