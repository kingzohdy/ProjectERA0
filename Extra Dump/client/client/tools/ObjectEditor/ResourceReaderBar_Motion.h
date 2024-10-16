
#pragma once
#include "ResourceReaderBar.h"

class CChunkDetailBarMotion : public CChunkDetailBar
{
	DECLARE_DYNAMIC(CChunkDetailBarMotion)

public:
	CChunkDetailBarMotion();
	virtual ~CChunkDetailBarMotion();

	CBCGPButton m_btnPreview;
	CBCGPButton m_btnEdit;
	CBCGPButton m_btnAddToCurMotion;

	CString m_strChunkName;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnPreview();
	afx_msg void OnEditChunk();
	afx_msg void OnPaint();
	afx_msg void OnAddToMotion();

	CString m_strInfo;
	void ReadChunkInfo();
};


