// GUIEditorDoc.h : interface of the CGUIEditorDoc class
//


#pragma once


class CGUIEditorDoc : public CDocument
{
protected: // create from serialization only
	CGUIEditorDoc();
	DECLARE_DYNCREATE(CGUIEditorDoc)

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
	virtual ~CGUIEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileSaveAs();
};


