#include "patchFile.h"

BOOL CPatchFile::open(LPCTSTR szPatchName,size_t offset,size_t size)
{
	if (size < sizeof(m_packHeader) )
	{
		return FALSE;
	}
	m_startIndex = offset;

	m_pFile = _wfopen(szPatchName,_T("rb"));
	
	if (m_pFile == NULL )
	{
		return FALSE;
	}

	fseek(m_pFile,m_startIndex,SEEK_SET);
	if(1 != fread(&m_packHeader, sizeof(m_packHeader), 1, m_pFile))
	{
		return FALSE;
	}

	m_vFileInfo.resize(m_packHeader.numfiles);
	if(m_packHeader.numfiles > 0)
	{
		fseek(m_pFile,m_startIndex + m_packHeader.keyoffset, SEEK_SET);
		if(1!=fread(&m_vFileInfo[0], m_packHeader.numfiles*sizeof(PkgFileInfo), 1, m_pFile))
		{
			return FALSE;
		}
	}
	return TRUE;
}

// DataStream* CPatchFile::openFile(int file_number)
// {
// 	if(file_number<0 || file_number>(int)m_vFileInfo.size())
// 	{
// 		return NULL;
// 	}
// 	const PkgFileInfo &info = m_vFileInfo[file_number];

// 	if((info.flags&1) == 0) //uncompressed file
// 	{
// 		FileStreamDataStream *pstream = new FileStreamDataStream(m_szFileName,m_startIndex + info.offset+m_packHeader.dataoffset,info.originsize);
// 		return pstream;
// 	}
// 	else
// 	{
// 		unsigned char *pbuf = new unsigned char[info.originsize];
// 		uchar *psrcbuf = new uchar[info.packsize];
// 
// 		if(readFile(psrcbuf,m_startIndex + info.offset, info.packsize) != info.packsize)
// 		{
// 			return NULL;
// 		}
// 
// 		uLongf destlen = info.originsize;
// 		if(uncompress(pbuf, &destlen, psrcbuf, info.packsize) != Z_OK || destlen!=info.originsize)
// 		{
// 			return NULL;
// 		}
// 
// 		delete[] psrcbuf;
// 		return new MemoryDataStream(pbuf, info.originsize, true);
// 	}
// }

BOOL CPatchFile::read(size_t index,char* pbuf)
{
	if(index >= m_vFileInfo.size())
	{
		return FALSE;
	}

	const  PkgFileInfo& info = m_vFileInfo[index];
	if (m_pFile== NULL )
	{
		return FALSE;
	}
	fseek(m_pFile,m_startIndex + info.offset,SEEK_SET);
	if (fread(pbuf,1,info.packsize,m_pFile)!= info.packsize )
	{
		return FALSE;
	}
	return TRUE;
}

void CPatchFile::close()
{
	if(m_pFile != NULL)
	{
		fclose(m_pFile);
	}

}