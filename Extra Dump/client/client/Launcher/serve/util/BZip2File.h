#pragma once

#include "bzlib.h"

class CBZip2File
{
public:
    CBZip2File();
    CBZip2File(const char *file_name, int offset = 0);
    ~CBZip2File();

    bool Open(const char *file_name, int offset = 0);
    bool Close();
    bool Read(char *buf, int& size);
    const int GetErrno() const { return m_nErrno; }

protected:
private:
    FILE*   m_pFile;
    BZFILE* m_pBzFile;
    int     m_nErrno;
};
