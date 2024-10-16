#include "stdafx.h"
#include "OgrePackageFile.h"
#include "OgreStringUtil.h"
#include "../../sdk/zlib/zlib.h"
#include <stdio.h>

const int PACKAGE_VERSION = 100;

#pragma comment(lib, "..\\..\\sdk\\zlib\\zdll.lib")

static bool NewPackageFile(const char *path)
{
	FILE *fp = fopen(path, "wb" );
	if(fp == NULL)
	{
		return false;
	}

	PackageHeader pkgheader;
	pkgheader.id = 0x56789abc;
	pkgheader.version = PACKAGE_VERSION;
	pkgheader.numfiles = 0;
	pkgheader.keyoffset = sizeof(pkgheader);
	pkgheader.keylen = 0;
	pkgheader.dataoffset = sizeof(pkgheader);
	pkgheader.datalen = 0;

	if(fwrite(&pkgheader, sizeof(pkgheader), 1, fp) != 1)
	{
		//LOG_SEVERE("write pkgfile error");
		fclose(fp);
		return false;
	}

	fclose(fp);
	return true;
}

bool FilePackage::open( const String &packpath, bool readonly)
{
	m_ReadOnly = readonly;
	m_FP = fopen(packpath.c_str(), readonly?"rb":"r+b");
	if(m_FP == NULL)
	{
		if(readonly)
		{
			//LOG_SEVERE("open pkgfile error: %s", packpath.c_str());
			return false;
		}
		else
		{
			if(!NewPackageFile(packpath.c_str())) return false;
			m_FP = fopen(packpath.c_str(), "r+b");
			if(m_FP == NULL) return false;
		}
	} 

	if(1 != fread(&m_PkgDesc, sizeof(m_PkgDesc), 1, m_FP))
	{
		//LOG_SEVERE("read pkgfile error: %s", packpath.c_str());
		return false;
	}

	m_FileTable.resize(m_PkgDesc.numfiles);
	if(m_PkgDesc.numfiles > 0)
	{
		fseek(m_FP, m_PkgDesc.keyoffset, SEEK_SET);
		if(1!=fread(&m_FileTable[0], m_PkgDesc.numfiles*sizeof(PkgFileInfo), 1, m_FP))
		{
			//LOG_SEVERE("read pkgfile error: %s", packpath.c_str());
			return false;
		}
	}

	return true;
}

void FilePackage::close()
{
	if(!m_ReadOnly && m_FP!=NULL && m_PkgDesc.numfiles>0)
	{
		m_PkgDesc.keyoffset = m_PkgDesc.dataoffset + m_PkgDesc.datalen;
		m_PkgDesc.keylen = m_PkgDesc.numfiles*sizeof(PkgFileInfo);

		fseek( m_FP, 0, SEEK_SET );
		if(1 != fwrite(&m_PkgDesc, sizeof(m_PkgDesc), 1, m_FP))
		{
			//LOG_SEVERE("write pkgfile error");
		}

		fseek(m_FP, m_PkgDesc.keyoffset, SEEK_SET);
		if(1!=fwrite(&m_FileTable[0], m_PkgDesc.keylen, 1, m_FP))
		{
			//LOG_SEVERE("write pkgfile error");
		}
	}

	if(m_FP != NULL)
	{
		fclose(m_FP);
	}
}

class PkgFileStream : public DataStream
{
public:
	PkgFileStream(FilePackage *pkg, unsigned int pkgoffset, unsigned int filesize);
	~PkgFileStream();

	virtual size_t read(void* buf, size_t count);
	virtual size_t write(const void *buf, size_t count);
	virtual void skip(long count);
	virtual void seek( size_t pos );
	virtual size_t tell(void) const;
	virtual bool eof(void) const;
	virtual void close(void);
	virtual void *getMemoryImage();

private:
	FilePackage *m_pkg;
	unsigned int m_pkgOffset;
	unsigned int m_curPos;
	void *m_pMemImage;
};

PkgFileStream::PkgFileStream(FilePackage *pkg, unsigned int pkgoffset, unsigned int filesize) : m_pkg(pkg), m_pkgOffset(pkgoffset), m_curPos(0), m_pMemImage(NULL)
{
	mSize = filesize;
}

PkgFileStream::~PkgFileStream()
{
	close();
}

size_t PkgFileStream::read(void* buf, size_t count)
{
	return m_pkg->readFile(buf, m_curPos+m_pkgOffset, (unsigned int )count);
}

size_t PkgFileStream::write(const void *buf, size_t count)
{
	//assert(0);
	return 0;
}

void PkgFileStream::skip(long count)
{
	m_curPos += count;
}

void PkgFileStream::seek( size_t pos )
{
	m_curPos = (unsigned int)pos;
}

size_t PkgFileStream::tell(void) const
{
	return m_curPos;
}

bool PkgFileStream::eof(void) const
{
	return m_curPos>=mSize;
}

void PkgFileStream::close(void)
{
	if(m_pMemImage)
	{
		free(m_pMemImage);
		m_pMemImage = NULL;
	}
}

void *PkgFileStream::getMemoryImage()
{
	if(m_pMemImage == NULL)
	{
		m_pMemImage = malloc(mSize);
		m_pkg->readFile(m_pMemImage, m_pkgOffset, mSize);
	}

	return m_pMemImage;
}

DataStream *FilePackage::openFile(int file_number)
{
	if(file_number<0 || file_number>(int)m_FileTable.size())
	{
		//LOG_SEVERE("openFile wrong file_number:%d", file_number);
		return NULL;
	}

	const PkgFileInfo &info = m_FileTable[file_number];

	if((info.flags&1) == 0) //uncompressed file
	{
		PkgFileStream *pstream = new PkgFileStream(this, info.offset+m_PkgDesc.dataoffset, info.originsize);
		return pstream;
	}
	else
	{
		unsigned char *pbuf = new unsigned char[info.originsize];
		uchar *psrcbuf = new uchar[info.packsize];

		if(readFile(psrcbuf, info.offset, info.packsize) != info.packsize)
		{
			//LOG_SEVERE("read pkgfile error: %d,%d", info.offset, info.packsize);
			return NULL;
		}

		uLongf destlen = info.originsize;
		if(uncompress(pbuf, &destlen, psrcbuf, info.packsize) != Z_OK || destlen!=info.originsize)
		{
			//LOG_SEVERE("uncompress data error: %d, %d", destlen, info.originsize);
			return NULL;
		}

		delete[] psrcbuf;
		return new MemoryDataStream(pbuf, info.originsize, true);
	}
}

unsigned int FilePackage::readFile(void *pbuf, unsigned int offset, unsigned int len)
{
	LockFunctor lockfunc(&m_Mutex);

	fseek(m_FP, offset, SEEK_SET);
	return (unsigned int)fread(pbuf, 1, len, m_FP);
}

bool FilePackage::appendOrReplaceFile( uint key1, uint key2, uint flags, const void *pdata, uint originsize, uint packsize, const char* szFileName )
{
	LockFunctor lockfunc(&m_Mutex);

	PkgFileInfo info;
	info.key1	= key1;
	info.key2	= key2;
	info.flags	= flags;
	info.offset = m_PkgDesc.dataoffset + m_PkgDesc.datalen;
	strncpy( info.szPath, szFileName, strlen(szFileName)+1 );	

	fseek(m_FP, info.offset, SEEK_SET);

	size_t n;
	if((flags&1) && packsize==0)//如果需要压缩， 并且传入的是未压缩数据
	{
		uLongf destlen = compressBound(originsize);
		unsigned char *pcompressed = new unsigned char[destlen];
		if(Z_OK != compress(pcompressed, &destlen, (const Bytef *)pdata, originsize))
		{
			//LOG_SEVERE("compress error");
			delete pcompressed;
			return false;
		}

		n = fwrite(pcompressed, destlen, 1, m_FP);
		info.originsize = originsize;
		info.packsize = destlen;
		delete pcompressed;
	}
	else
	{
		n = fwrite(pdata, originsize, 1, m_FP);
		info.originsize = originsize;
		if(packsize == 0) info.packsize = originsize;
		else info.packsize = packsize;
	}

	if(n != 1)
	{
		//LOG_SEVERE("write pkgfile error: %d, %d", info.packsize, info.originsize);
		return false;
	}

	m_PkgDesc.datalen += info.packsize;

	size_t i;
	for(i=0; i<m_FileTable.size(); i++)
	{
		PkgFileInfo &info2 = m_FileTable[i];
		if(info2.key1==key1 && info2.key2==key2)
		{
			break;
		}
	}

	if(i == m_FileTable.size())
	{
		m_FileTable.resize(i+1);
		m_PkgDesc.numfiles++;
	}

	m_FileTable[i] = info;
	return true;
}

bool FilePackage::appendOrReplaceFile( const char* filename, bool do_compress, const char* szFilePath )
{
	FILE *fp = fopen(filename, "rb");
	if(fp == NULL)
	{
		return false;
	}

	fseek(fp, 0, SEEK_END);
	long len = ftell(fp);
	char *pbuf = new char[len];
	fseek(fp, 0, SEEK_SET);
	if(fread(pbuf, len, 1, fp) != 1)
	{
		fclose(fp);
		return false;
	}
	fclose(fp);
	
	string fileRealPath = filename;
	if( szFilePath != NULL )
	{
		// 绝对路径改相对路径
		fileRealPath = string( filename ).substr( strlen( szFilePath ) + 1 );
	}
	
	unsigned int key1 = StringUtil::hash(fileRealPath.c_str(), 1);
	unsigned int key2 = StringUtil::hash(fileRealPath.c_str(), 2);

	bool b = appendOrReplaceFile( key1, key2, do_compress?1:0, pbuf, len, 0, fileRealPath.c_str() );
	delete[] pbuf;

	return b;
}

/*
int main()
{
FilePackage pkg;
pkg.Open( "1.1.1.17.pkg" );

PkgFileInfo obj;
bool b = pkg.FindFirstFile( obj );
while( b )
{
b = pkg.FindNextFile( obj );
}

//bool GetFileData( void *pbuf, int originsize, int offset, int packsize );

pkg.Close();
return 0;
}
*/
