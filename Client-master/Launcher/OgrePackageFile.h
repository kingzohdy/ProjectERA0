
#ifndef __PackageFile_H__
#define __PackageFile_H__

#include "OgreDataStream.h"
#include "OgreLocker.h"
#include <vector>
#include <stdio.h>


struct PackageHeader
{
	unsigned int id;
	int version;
	int numfiles;
	int keyoffset;
	int keylen;
	int dataoffset;
	int datalen;
};

struct PkgFileInfo
{
	unsigned int key1;
	unsigned int key2;
	unsigned int offset;
	unsigned int originsize;
	unsigned int packsize; //δѹ���ļ�packsize == originsize
	unsigned int flags; //bit0: compress
	char szPath[MAX_PATH];
};

//����ֻ����package
class FilePackage
{
public:
	FilePackage() : m_FP(NULL){}

	bool open(const String &package_path, bool readonly);
	void close();

	size_t getNumFiles()
	{
		return m_FileTable.size();
	}

	const PkgFileInfo &getFileInfo(size_t i)
	{
		return m_FileTable[i];
	}

	DataStream *openFile(int file_number);
	unsigned int readFile(void *pbuf, unsigned int offset, unsigned int len);

	//packsize==0��ʾpdata������δѹ������
	bool appendOrReplaceFile( uint key1, uint key2, uint flags, const void *pdata, uint originsize, uint packsize, const char* szFileName );
	bool appendOrReplaceFile( const char *filename, bool do_compress, const char* szFilePath = NULL );

private:
	bool m_ReadOnly;
	FILE *m_FP;
	PackageHeader m_PkgDesc;
	std::vector<PkgFileInfo>m_FileTable;
	LockSection m_Mutex;
};


#endif