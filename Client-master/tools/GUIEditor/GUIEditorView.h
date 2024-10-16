// GUIEditorView.h : interface of the CGUIEditorView class
//


#pragma once


class CGUIEditorView : public CView
{
protected: // create from serialization only
	CGUIEditorView();
	DECLARE_DYNCREATE(CGUIEditorView)

// Attributes
public:
	CGUIEditorDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGUIEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileNew();
};

#ifndef _DEBUG  // debug version in GUIEditorView.cpp
inline CGUIEditorDoc* CGUIEditorView::GetDocument() const
   { return reinterpret_cast<CGUIEditorDoc*>(m_pDocument); }
#endif

