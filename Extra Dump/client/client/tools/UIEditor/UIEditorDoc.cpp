// UIEditorDoc.cpp : implementation of the CUIEditorDoc class
//

#include "stdafx.h"
#include "UIEditor.h"

#include "UIEditorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUIEditorDoc

IMPLEMENT_DYNCREATE(CUIEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CUIEditorDoc, CDocument)
END_MESSAGE_MAP()


// CUIEditorDoc construction/destruction

CUIEditorDoc::CUIEditorDoc()
{
	// TODO: add one-time construction code here

}

CUIEditorDoc::~CUIEditorDoc()
{
}

BOOL CUIEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CUIEditorDoc serialization

void CUIEditorDoc::Serialize(CArchive& ar)
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


// CUIEditorDoc diagnostics

#ifdef _DEBUG
void CUIEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CUIEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CUIEditorDoc commands
