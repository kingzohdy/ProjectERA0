// SkillEditorView.h : interface of the CSkillEditorView class
//


#pragma once


class CSkillEditorView : public CView
{
protected: // create from serialization only
	CSkillEditorView();
	DECLARE_DYNCREATE(CSkillEditorView)

// Attributes
public:
	CSkillEditorDoc* GetDocument() const;

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
	virtual ~CSkillEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SkillEditorView.cpp
inline CSkillEditorDoc* CSkillEditorView::GetDocument() const
   { return reinterpret_cast<CSkillEditorDoc*>(m_pDocument); }
#endif

