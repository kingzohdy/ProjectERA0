
#include "stdafx.h"
#include "ResourceReaderBar.h"


// CChunkDetailBar

IMPLEMENT_DYNAMIC(CChunkDetailBar, CWnd)
CChunkDetailBar::CChunkDetailBar()
{
}

CChunkDetailBar::~CChunkDetailBar()
{
}


BEGIN_MESSAGE_MAP(CChunkDetailBar, CWnd)
END_MESSAGE_MAP()


CString CChunkDetailBar::GetChunkTitle(CString strChunk)
{
	int pos = strChunk.ReverseFind('.');
	if(pos < 0)
		return strChunk;
	else
		return strChunk.Right(strChunk.GetLength() - pos - 1);
}

// CChunkDetailBar message handlers

