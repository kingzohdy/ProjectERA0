// modelshowView.h : CmodelshowView 类的接口
//


#pragma once
#include "Show.h"
#include "AnimDlg.h"
#include <Mmsystem.h>
#pragma comment(lib, "winmm.lib")
#define MINSIZE 0.1F
#define MAXSIZE 10.0F

class CmodelshowView : public CView
{
private:
	CShow		*m_pShow;	
	CString		m_StrTitle;
	CString		m_StrTitleofENT;
	CAnimDlg	m_AnimDlg;
	bool		m_isLBDown;
	POINT		m_Point;
	char		*m_OldPath;
	CString     m_filename;
	CString		m_BKFilename;
protected: // 仅从序列化创建
	CmodelshowView();
	DECLARE_DYNCREATE(CmodelshowView)

// 属性
public:
	CmodelshowDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CmodelshowView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnInitialUpdate();
	afx_msg void OnOpenFile();
	afx_msg void OnSetAnimtion();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnReOpen();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLoadBKModel();
	afx_msg void OnOpenENT();
};

#ifndef _DEBUG  // modelshowView.cpp 中的调试版本
inline CmodelshowDoc* CmodelshowView::GetDocument() const
   { return reinterpret_cast<CmodelshowDoc*>(m_pDocument); }

#endif

