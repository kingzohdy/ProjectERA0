// WorldEditorView.cpp : CWorldEditorView ���ʵ��
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

// CWorldEditorView ����/����

CWorldEditorView::CWorldEditorView()
{
	// TODO: �ڴ˴���ӹ������

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
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	// TODO: �ڴ˴������Ϣ����������

	// ��ҪΪ������Ϣ������ CWnd::OnPaint()
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