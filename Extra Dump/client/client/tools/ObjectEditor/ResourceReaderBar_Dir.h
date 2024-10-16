
#pragma once
#include "ResourceReaderBar.h"

class CChunkDirBar : public CChunkDetailBar
{
	DECLARE_DYNAMIC(CChunkDirBar)

public:
	CChunkDirBar();
	virtual ~CChunkDirBar();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


