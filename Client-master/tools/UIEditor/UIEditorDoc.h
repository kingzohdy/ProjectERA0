// UIEditorDoc.h : interface of the CUIEditorDoc class
//


#pragma once


class CUIEditorDoc : public CDocument
{
protected: // create from serialization only
	CUIEditorDoc();
	DECLARE_DYNCREATE(CUIEditorDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CUIEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


