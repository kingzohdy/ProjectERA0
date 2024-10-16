// modelshowDoc.cpp : CmodelshowDoc 类的实现
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


// CmodelshowDoc 构造/析构

CmodelshowDoc::CmodelshowDoc()
{
	// TODO: 在此添加一次性构造代码

}

CmodelshowDoc::~CmodelshowDoc()
{
}

BOOL CmodelshowDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CmodelshowDoc 序列化

void CmodelshowDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CmodelshowDoc 诊断

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


// CmodelshowDoc 命令
