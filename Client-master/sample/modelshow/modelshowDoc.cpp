// modelshowDoc.cpp : CmodelshowDoc ���ʵ��
//

#include "stdafx.h"
#include "modelshow.h"

#include "modelshowDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CmodelshowDoc

IMPLEMENT_DYNCREATE(CmodelshowDoc, CDocument)

BEGIN_MESSAGE_MAP(CmodelshowDoc, CDocument)
END_MESSAGE_MAP()


// CmodelshowDoc ����/����

CmodelshowDoc::CmodelshowDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CmodelshowDoc::~CmodelshowDoc()
{
}

BOOL CmodelshowDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CmodelshowDoc ���л�

void CmodelshowDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CmodelshowDoc ���

#ifdef _DEBUG
void CmodelshowDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CmodelshowDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CmodelshowDoc ����
