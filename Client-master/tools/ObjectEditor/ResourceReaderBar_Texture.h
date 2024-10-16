
#pragma once
#include "ResourceReaderBar.h"

// CChunkDetailBarTex
class CChunkDetailTexture;
class CChunkDetailBarTex : public CChunkDetailBar
{
	DECLARE_DYNAMIC(CChunkDetailBarTex)

public:
	CChunkDetailBarTex();
	virtual ~CChunkDetailBarTex();

	CString m_strTexName;
	CChunkDetailTexture* m_pTexDtl;

	CListCtrl	m_wndList;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


