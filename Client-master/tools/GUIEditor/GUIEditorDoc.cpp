// GUIEditorDoc.cpp : implementation of the CGUIEditorDoc class
//

#include "stdafx.h"
#include "GUIEditor.h"

#include "GUIEditorDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGUIEditorDoc

IMPLEMENT_DYNCREATE(CGUIEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CGUIEditorDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE_AS, &CGUIEditorDoc::OnFileSaveAs)
END_MESSAGE_MAP()


// CGUIEditorDoc construction/destruction

CGUIEditorDoc::CGUIEditorDoc()
{
	// TODO: add one-time construction code here

}

CGUIEditorDoc::~CGUIEditorDoc()
{
}

BOOL CGUIEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CGUIEditorDoc serialization

void CGUIEditorDoc::Serialize(CArchive& ar)
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


// CGUIEditorDoc diagnostics

#ifdef _DEBUG
void CGUIEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGUIEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGUIEditorDoc commands

void CGUIEditorDoc::OnFileSaveAs()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog fileSaveAsDlg( FALSE );
	//std::string xmlPostfix = ".xml";
	//保存toc文件
	fileSaveAsDlg.m_ofn.lpstrFilter	= _T("XML Files(*.xml)\0*.xml\0All Files(*.*)\0*.*\0");

	if( IDOK == fileSaveAsDlg.DoModal() )
	{
		CMainFrame * pMainframe = (CMainFrame *)AfxGetMainWnd();
		pMainframe->SaveXMLFile( fileSaveAsDlg.GetPathName() );
	}
}
