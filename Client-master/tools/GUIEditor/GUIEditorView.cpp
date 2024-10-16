// GUIEditorView.cpp : implementation of the CGUIEditorView class
//

#include "stdafx.h"
#include "GUIEditor.h"

#include "GUIEditorDoc.h"
#include "GUIEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGUIEditorView

IMPLEMENT_DYNCREATE(CGUIEditorView, CView)

BEGIN_MESSAGE_MAP(CGUIEditorView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_NEW, &CGUIEditorView::OnFileNew)
END_MESSAGE_MAP()

// CGUIEditorView construction/destruction

CGUIEditorView::CGUIEditorView()
{
	// TODO: add construction code here

}

CGUIEditorView::~CGUIEditorView()
{
}

BOOL CGUIEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGUIEditorView drawing

void CGUIEditorView::OnDraw(CDC* /*pDC*/)
{
	CGUIEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


// CGUIEditorView printing

void CGUIEditorView::OnFilePrintPreview()
{
	BCGPPrintPreview (this);
}

BOOL CGUIEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGUIEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGUIEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGUIEditorView diagnostics

#ifdef _DEBUG
void CGUIEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CGUIEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGUIEditorDoc* CGUIEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGUIEditorDoc)));
	return (CGUIEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CGUIEditorView message handlers

void CGUIEditorView::OnFileNew()
{
	// TODO: 在此添加命令处理程序代码
	HDC hdc;
	hdc = ::GetDC(m_hWnd);

}
