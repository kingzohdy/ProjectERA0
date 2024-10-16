
#ifndef __GamePackageFile_H__
#define __GamePackageFile_H__
#include "OgreDataStream.h"
#include "OgreLocker.h"
#include <vector>
#include <stdio.h>

struct GamePackageHeader
{
	unsigned int id;
	int version;
	int numfiles;
	int keyoffset;
	int keylen;
	int dataoffset;
	int datalen;
};

struct GamePkgFileInfo
{
	unsigned int key1;
	unsigned int key2;
	unsigned int offset;
	unsigned int originsize;
	unsigned int packsize; //未压缩文件packsize == originsize
	unsigned int flags; //bit0: compress
};

//用于只读的package
class GameFilePackage
{
public:
	GameFilePackage() : m_FP(NULL){}

	bool open(const String &package_path, bool readonly);
	void close();

	size_t getNumFiles()
	{
		return m_FileTable.size();
	}

	const GamePkgFileInfo &getFileInfo(size_t i)
	{
		return m_FileTable[i];
	}

	DataStream *openFile(int file_number);
	unsigned int readFile(void *pbuf, unsigned int offset, unsigned int len);

	//packsize==0表示pdata数据是未压缩数据
	bool appendOrReplaceFile(uint key1, uint key2, uint flags, const void *pdata, uint originsize, uint packsize);
	bool appendOrReplaceFile(const char *filename, bool do_compress);

private:
	bool m_ReadOnly;
	FILE *m_FP;
	GamePackageHeader m_PkgDesc;
	std::vector<GamePkgFileInfo>m_FileTable;
	LockSection m_Mutex;
};

#endif