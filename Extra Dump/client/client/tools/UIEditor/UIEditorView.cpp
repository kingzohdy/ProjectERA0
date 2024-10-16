// UIEditorView.cpp : implementation of the CUIEditorView class
//

#include "stdafx.h"
#include "UIEditor.h"

#include "UIEditorDoc.h"
#include "UIEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUIEditorView

IMPLEMENT_DYNCREATE(CUIEditorView, CView)

BEGIN_MESSAGE_MAP(CUIEditorView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
END_MESSAGE_MAP()

// CUIEditorView construction/destruction

CUIEditorView::CUIEditorView()
{
	// TODO: add construction code here

}

CUIEditorView::~CUIEditorView()
{
}

BOOL CUIEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CUIEditorView drawing

void CUIEditorView::OnDraw(CDC* /*pDC*/)
{
	CUIEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


// CUIEditorView printing

void CUIEditorView::OnFilePrintPreview()
{
	BCGPPrintPreview (this);
}

BOOL CUIEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CUIEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CUIEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CUIEditorView diagnostics

#ifdef _DEBUG
void CUIEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CUIEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUIEditorDoc* CUIEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUIEditorDoc)));
	return (CUIEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CUIEditorView message handlers
