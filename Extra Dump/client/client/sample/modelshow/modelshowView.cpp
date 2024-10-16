// modelshowView.cpp : CmodelshowView ���ʵ��
//

#include "stdafx.h"
#include "modelshow.h"

#include "modelshowDoc.h"
#include "modelshowView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CmodelshowView

IMPLEMENT_DYNCREATE(CmodelshowView, CView)

BEGIN_MESSAGE_MAP(CmodelshowView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(ID_32771, &CmodelshowView::OnOpenFile)
	ON_COMMAND(ID_32773, &CmodelshowView::OnSetAnimtion)
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_32774, &CmodelshowView::OnReOpen)
	ON_COMMAND(ID_32775, &CmodelshowView::OnLoadBKModel)
	ON_COMMAND(ID_32776, &CmodelshowView::OnOpenENT)
END_MESSAGE_MAP()

// CmodelshowView ����/����

CmodelshowView::CmodelshowView()
{
	// TODO: �ڴ˴���ӹ������
	m_pShow = NULL;
	m_StrTitle = "ģ���ļ�(*.omod)|*.omod||";
	m_StrTitleofENT = "ģ���ļ�(*.ent)|*.ent||";
	m_isLBDown = false;
	m_OldPath = "e:\\myworld\\bin";
	m_filename = theApp.m_filename;
	m_BKFilename = "";
}

CmodelshowView::~CmodelshowView()
{
}

BOOL CmodelshowView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CmodelshowView ����

void CmodelshowView::OnDraw(CDC* /*pDC*/)
{
	CmodelshowDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CmodelshowView ��ӡ

BOOL CmodelshowView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CmodelshowView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CmodelshowView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CmodelshowView ���

#ifdef _DEBUG
void CmodelshowView::AssertValid() const
{
	CView::AssertValid();
}

void CmodelshowView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CmodelshowDoc* CmodelshowView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CmodelshowDoc)));
	return (CmodelshowDoc*)m_pDocument;
}
#endif //_DEBUG


// CmodelshowView ��Ϣ�������

int CmodelshowView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CmodelshowView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (m_pShow == NULL)
		return;
	static unsigned int lasttick = timeGetTime();
	unsigned int curtick = timeGetTime();

	m_pShow->UpdateData(curtick - lasttick);
	m_pShow->DoFrame();

	lasttick = curtick;
	CView::OnTimer(nIDEvent);
}

void CmodelshowView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_pShow = new CShow();
	m_pShow->InitRenderSys(m_hWnd);
	m_pShow->CreateData(theApp.m_filename);

	SetTimer(0,50,0);
	// TODO: Add your specialized code here and/or call the base class
}

void CmodelshowView::OnOpenFile()
{
	CFileDialog filedlg(TRUE, NULL, NULL, NULL, m_StrTitle,this);
	if (IDOK == filedlg.DoModal())
	{
		SetCurrentDirectory(m_OldPath);
		m_filename = filedlg.GetPathName();
		m_pShow->LoadModel(m_filename);
	}
}

void CmodelshowView::OnSetAnimtion()
{
	if (IDOK != m_AnimDlg.DoModal())
		return;
	UpdateData();
	m_pShow->SetAnimID(m_AnimDlg.GetAnimID());
	m_pShow->SetIsChangeAnim(true);
}

BOOL CmodelshowView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	m_pShow->SetModelSize( m_pShow->GetModelSize() + 0.001F*zDelta );
	if(m_pShow->GetModelSize() < MINSIZE)
		m_pShow->SetModelSize( MINSIZE );
	if (m_pShow->GetModelSize() > MAXSIZE)
		m_pShow->SetModelSize(MAXSIZE);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CmodelshowView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_isLBDown = true;
	m_Point = point;
	CView::OnLButtonDown(nFlags, point);
}

void CmodelshowView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_isLBDown = false;
	CView::OnLButtonUp(nFlags, point);
}

void CmodelshowView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if(!m_isLBDown)
		return;
	m_pShow->SetOffsetX( m_Point.x - point.x);
	m_pShow->SetOffsetY( m_Point.y - point.y);
	m_Point = point;
	CView::OnMouseMove(nFlags, point);
}

void CmodelshowView::OnDestroy()
{
	CView::OnDestroy();
	if (m_pShow)
	{
		delete m_pShow;
		m_pShow = NULL;
	}
}

void CmodelshowView::OnReOpen()
{
	if (m_filename =="")
		return;

	m_pShow->LoadModel(m_filename);
	/*m_filename.MakeLower();
	if (m_filename.Right(4) == "omod")
	{
		m_pShow->LoadModel(m_filename);
		return;
	}
	if (m_filename.Right(3) == "ent")
	{
		m_pShow->LoadEnt(m_filename);
	}*/
}

LRESULT CmodelshowView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			m_pShow->PreAnima();
			break;
		case VK_DOWN:
			m_pShow->NextAnima();
			break;
		case VK_LEFT:
			m_pShow->SetEyePos(Vector3(0,1000,-500));
			m_pShow->SetLookAt(Vector3(0,150,0));
			break;
		case VK_RIGHT:
			m_pShow->SetEyePos(Vector3(0,150,-1000));
			m_pShow->SetLookAt(Vector3(0,150,0));
			break;
		}
		break;
	}
	return CView::WindowProc(message, wParam, lParam);
}

void CmodelshowView::OnLoadBKModel()// ���ر���ģ��
{
	CFileDialog filedlg(TRUE, NULL, NULL, NULL, m_StrTitle,this);
	if (IDOK == filedlg.DoModal())
	{
		SetCurrentDirectory(m_OldPath);
		m_BKFilename = filedlg.GetPathName();
		m_pShow->LoadBKModel(m_BKFilename);
	}
}

void CmodelshowView::OnOpenENT()	// ��ENT�ļ�
{
	CFileDialog filedlg(TRUE, NULL, NULL, NULL, m_StrTitleofENT,this);
	if (IDOK == filedlg.DoModal())
	{
		SetCurrentDirectory(m_OldPath);
		m_filename = filedlg.GetPathName();
		m_pShow->LoadModel(m_filename);
	}
}
