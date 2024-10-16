// SkillEditorDoc.cpp : implementation of the CSkillEditorDoc class
//

#include "stdafx.h"
#include "SkillEditor.h"

#include "SkillEditorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSkillEditorDoc

IMPLEMENT_DYNCREATE(CSkillEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CSkillEditorDoc, CDocument)
END_MESSAGE_MAP()


// CSkillEditorDoc construction/destruction

CSkillEditorDoc::CSkillEditorDoc()
{
	// TODO: add one-time construction code here

}

CSkillEditorDoc::~CSkillEditorDoc()
{
}

BOOL CSkillEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CSkillEditorDoc serialization

void CSkillEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CSkillEditorDoc diagnostics

#ifdef _DEBUG
void CSkillEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSkillEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSkillEditorDoc commands
