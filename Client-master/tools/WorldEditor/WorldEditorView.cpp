// WorldEditorView.cpp : CWorldEditorView 类的实现
//

#include "stdafx.h"
#include "WorldEditor.h"

#include "WorldEditorView.h"
#include "TerrainEdit.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace TE;
// CWorldEditorView

BEGIN_MESSAGE_MAP(CWorldEditorView, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CWorldEditorView 构造/析构

CWorldEditorView::CWorldEditorView()
{
	// TODO: 在此处添加构造代码

}

CWorldEditorView::~CWorldEditorView()
{
}

BOOL CWorldEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CWorldEditorView::OnPaint() 
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	// TODO: 在此处添加消息处理程序代码

	// 不要为绘制消息而调用 CWnd::OnPaint()
}

LRESULT CWorldEditorView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	g_TerrainEdit.MsgProc(GetSafeHwnd(), message, wParam, lParam);

	if (WM_MOUSEMOVE == message)
	{
	//	((CMainFrame*)GetParent())->m_ModelInfoDlg.InitData();
	}

	switch( message )
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDOWN:
		{
			SetFocus ();
		}
		break;
	}

	return CWnd::WindowProc(message, wParam, lParam);
}