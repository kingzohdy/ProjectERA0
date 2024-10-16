#include "stdafx.h"
#include <assert.h>
#include "BZip2File.h"
#include "util/Utils.h"


CBZip2File::CBZip2File()
    : m_pBzFile(NULL), m_pFile(NULL), m_nErrno(BZ_OK)
{
}

CBZip2File::CBZip2File(const char *file_name, int offset)
{
    Open(file_name, offset);
}

CBZip2File::~CBZip2File()
{
    Close();
}

bool CBZip2File::Open(const char *file_name, int offset)
{
    m_pFile = fopen(file_name, "rb");
    if(m_pFile == NULL)
        return false;
    fseek(m_pFile, offset, SEEK_SET);

    m_pBzFile = BZ2_bzReadOpen(&m_nErrno, m_pFile, 0, 0, NULL, 0);
    if((m_pBzFile == NULL) || (m_nErrno != BZ_OK))
        return false;

    return false;
}

bool CBZip2File::Close()
{
    if(m_pFile)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }
    if(m_pBzFile)
    {
        BZ2_bzReadClose(&m_nErrno, m_pBzFile);
        m_pBzFile = NULL;
        return m_nErrno == BZ_OK;
    }
    return true;
}

bool CBZip2File::Read(char *buf, int& size)
{
    size = BZ2_bzRead(&m_nErrno, m_pBzFile, buf, size);
    if((m_nErrno != BZ_OK) && (m_nErrno != BZ_STREAM_END))
        return false;
    return true;
}
