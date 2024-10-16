// SkillEditorDoc.h : interface of the CSkillEditorDoc class
//


#pragma once


class CSkillEditorDoc : public CDocument
{
protected: // create from serialization only
	CSkillEditorDoc();
	DECLARE_DYNCREATE(CSkillEditorDoc)

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
	virtual ~CSkillEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


