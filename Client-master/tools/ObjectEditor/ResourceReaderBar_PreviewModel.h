
#pragma once
#include "ResourceReaderBar.h"
#include "RenderStaticView.h"

class CChunkDetailPreviewModel;

class CChunkDetailBarPreviewModel :	public CChunkDetailBar
{

	DECLARE_DYNAMIC(CChunkDetailBarPreviewModel)
public:
	CChunkDetailBarPreviewModel(void);
	virtual ~CChunkDetailBarPreviewModel(void);

	CChunkDetailPreviewModel*	m_pDetail;
	CRenderStatic				m_wndRenderStatic;
	CRenderStaticHandler		m_handler;
	bool						m_bFirst;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
};
