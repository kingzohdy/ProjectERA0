
#pragma once
#include "stdafx.h"
#include "OgrePackageFile.h"

//���������������ݲ���
class CPatchFile
{
public:
	CPatchFile():m_pFile(NULL)
	{
		memset(&m_packHeader,0,sizeof(m_packHeader));
	};	

public:	
	BOOL open( LPCTSTR szPatchName,size_t offset,size_t size);
	void close();

	size_t getNumFiles()
	{
		return m_vFileInfo.size();
	}

	const PkgFileInfo &getFileInfo(size_t i)
	{
		return m_vFileInfo[i];
	}

	unsigned int readFile(void *pbuf, unsigned int offset, unsigned int len);

	//��ȡ���ݣ�pbuf�Ĵ�СӦ����packsize
	BOOL read(size_t index,char* pbuf);

private:
	size_t m_startIndex;
	FILE* m_pFile;
	PackageHeader m_packHeader;
	std::vector<PkgFileInfo>m_vFileInfo;
};