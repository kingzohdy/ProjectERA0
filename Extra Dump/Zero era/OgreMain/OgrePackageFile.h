
#ifndef __PackageFile_H__
#define __PackageFile_H__

#include "OgrePrerequisites.h"
#include "OgreDataStream.h"
#include "OgreLocker.h"
#include <vector>
#include <stdio.h>

namespace Ogre
{
	class FilePackage;

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
		unsigned int packsize; //未压缩文件packsize == originsize
		unsigned int flags; //bit0: compress
	};

	class _OgreExport PackageDataStreamObject : public DataStreamObject
	{
	protected:
		FilePackage	*mPkg;
		size_t		mSize;
		size_t		mLength;
		void		*mPtr;

	public:
		PackageDataStreamObject(FilePackage *pkg, size_t size);
		virtual ~PackageDataStreamObject();

		virtual void	release();
		virtual void *	buffer();
		virtual void *	buffer(size_t len);
		virtual size_t	length();
		virtual size_t	size();
		virtual void	shift(size_t len);
	};

	//用于只读的package
	class _OgreExport FilePackage
	{
	public:
		FilePackage() : m_FP(NULL), m_MaxOriginSize(0), m_MaxPackSize(0), m_PackBuffer(NULL), m_BufferObjectCount(0){}
		~FilePackage() {close();}

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

		std::string getPackagePath ()
		{
			return m_PackagePath;
		}

		DataStream *openFile(int file_number);
		unsigned int readFile(void *pbuf, unsigned int offset, unsigned int len);

		//packsize==0表示pdata数据是未压缩数据
		bool appendOrReplaceFile(uint key1, uint key2, uint flags, const void *pdata, uint originsize, uint packsize);
		bool appendOrReplaceFile(const char *filename, bool do_compress);

		PackageDataStreamObject *newBufferObject();
		void freeBufferObject(PackageDataStreamObject *object);

	public:
		bool m_ReadOnly;
		FILE *m_FP;
		PackageHeader m_PkgDesc;
		std::vector<PkgFileInfo>m_FileTable;
		LockSection m_Mutex;
		std::string m_PackagePath;
		unsigned int m_MaxOriginSize;
		unsigned int m_MaxPackSize;
		void									*m_PackBuffer;
		std::vector<PackageDataStreamObject *>	m_OriginMemList;
		size_t									m_BufferObjectCount;

		LockSection m_PackMutex;
		LockSection m_MemListMutex;
	};
}

#endif