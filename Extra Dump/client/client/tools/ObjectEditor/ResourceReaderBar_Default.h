
#pragma once
#include "ResourceReaderBar.h"

// CChunkDetailBarDefault
class CChunkDetail;

class CChunkDetailBarDefault : public CChunkDetailBar
{
	DECLARE_DYNAMIC(CChunkDetailBarDefault)

public:
	CChunkDetailBarDefault();
	virtual ~CChunkDetailBarDefault();
	CString m_strChunkName;

	CChunkDetail* m_pChunkDtl;

	CListCtrl	m_wndList;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


