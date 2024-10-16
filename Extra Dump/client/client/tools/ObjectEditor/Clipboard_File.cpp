
#include "stdafx.h"
#include "Clipboard_File.h"
#include <assert.h>

CFileChipboard::CFileChipboard()
{
}

CFileChipboard::~CFileChipboard(void)
{
}

//复制到剪贴板
void CFileChipboard::CopyToChipboard(std::vector<CString>& files)
{
	int iTotalLen = 0;
	for(int i = 0; i < (int)files.size(); i++)
	{
		iTotalLen += files[i].GetLength();
	}

	iTotalLen += (int)files.size(); // ending NULL of each string
	iTotalLen++;

	char* pShortNameBuf = new char[iTotalLen];
	ZeroMemory(pShortNameBuf, iTotalLen);
	int start = 0;
	for(int i = 0; i < (int)files.size(); i++)
	{
		int stringlen = files[i].GetLength();
		CopyMemory(pShortNameBuf + start, files[i].GetBuffer(), stringlen );
		start += stringlen + 1;
	}

	int iWideSize = ::MultiByteToWideChar(CP_ACP, 0, pShortNameBuf, iTotalLen, NULL, 0);
	char* pWideName = new char[iWideSize * 2];
	::MultiByteToWideChar(CP_ACP, 0, pShortNameBuf, iTotalLen, (LPWSTR)pWideName, iWideSize);

	DROPFILES df;
	df.fNC = 0;
	df.fWide = TRUE;
	df.pt.x = 0;
	df.pt.y = 0;
	df.pFiles = sizeof(df);

	int iBuffSize = sizeof(df) + iWideSize * 2;
	HANDLE hBuf = GlobalAlloc(GHND, iBuffSize);

	char* pBufPt = (char*) GlobalLock(hBuf);
	CopyMemory(pBufPt, &df, sizeof(df));
	CopyMemory(pBufPt + sizeof(df), pWideName, iWideSize * 2);

	GlobalUnlock(hBuf);

	OpenClipboard(NULL);

	SetClipboardData(CF_HDROP, hBuf);

	CloseClipboard();

	delete [] pWideName;
	delete [] pShortNameBuf;
}


//从剪贴版取文件名
void CFileChipboard::GetFromChipboard(std::vector<CString>& files)
{
	if( OpenClipboard(NULL) == FALSE)
		return;

	HANDLE hHandle = GetClipboardData(CF_HDROP);
	if(hHandle > 0)
	{
		SIZE_T size = GlobalSize(hHandle);
		LPVOID pData = GlobalLock(hHandle);

		DROPFILES df;
		CopyMemory(&df, pData, sizeof(df));

		SIZE_T namesize = size - sizeof(df);
		char* pNames = new char[namesize];
		assert(df.pFiles == sizeof(df));
		CopyMemory(pNames, (char*)pData + df.pFiles, namesize);

		if(df.fWide == TRUE)
		{
			char* pMultiNames = new char[namesize];
			::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)pNames,(int)( namesize / 2), pMultiNames, (int)namesize, NULL, NULL);
			delete [] pNames;
			pNames = pMultiNames;
			//namesize /= 2;
		}

		CString strTmp;
		int p = -1;
		do
		{
			int start = ++p;
			while(pNames[p] != 0) 
				++p;

			strTmp = pNames+start;
			if(strTmp != "")
				files.push_back(strTmp);
			else
				break;
		}
		while(p < (int)namesize);

		delete [] pNames;
		GlobalUnlock(hHandle);

	}

	CloseClipboard();


}