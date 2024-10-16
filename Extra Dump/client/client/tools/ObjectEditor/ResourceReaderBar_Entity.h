
#pragma once
#include "ResourceReaderBar.h"

// CChunkDetailBarShowMod
class CChunkDetailShowModel;
class CChunkDetailBarShowMod : public CChunkDetailBar
{
	DECLARE_DYNAMIC(CChunkDetailBarShowMod)

public:
	CChunkDetailBarShowMod();
	virtual ~CChunkDetailBarShowMod();

	CString m_strModName;
	CChunkDetailShowModel * m_pModDtl;

	CListCtrl	m_wndList;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnItemActive(NMHDR* pNMHDR, LRESULT* pResult);
};


