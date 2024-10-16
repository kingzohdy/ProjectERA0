
#pragma once

class CChunkDetailBar : public CWnd
{
	DECLARE_DYNAMIC(CChunkDetailBar)

public:
	CChunkDetailBar();
	virtual ~CChunkDetailBar();

	static CString GetChunkTitle(CString strChunk);

protected:



	DECLARE_MESSAGE_MAP()
};