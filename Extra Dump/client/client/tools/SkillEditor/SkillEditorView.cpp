// SkillEditorView.cpp : implementation of the CSkillEditorView class
//

#include "stdafx.h"
#include "SkillEditor.h"

#include "SkillEditorDoc.h"
#include "SkillEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSkillEditorView

IMPLEMENT_DYNCREATE(CSkillEditorView, CView)

BEGIN_MESSAGE_MAP(CSkillEditorView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
END_MESSAGE_MAP()

// CSkillEditorView construction/destruction

CSkillEditorView::CSkillEditorView()
{
	// TODO: add construction code here

}

CSkillEditorView::~CSkillEditorView()
{
}

BOOL CSkillEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSkillEditorView drawing

void CSkillEditorView::OnDraw(CDC* /*pDC*/)
{
	CSkillEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


// CSkillEditorView printing

void CSkillEditorView::OnFilePrintPreview()
{
	BCGPPrintPreview (this);
}

BOOL CSkillEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSkillEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSkillEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CSkillEditorView diagnostics

#ifdef _DEBUG
void CSkillEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CSkillEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSkillEditorDoc* CSkillEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSkillEditorDoc)));
	return (CSkillEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CSkillEditorView message handlers
