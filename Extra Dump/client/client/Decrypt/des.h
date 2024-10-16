#ifndef _DES_H_
#define _DES_H_
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <algorithm>

enum EnCodeOp
{
	CLIENT_JIEMI = 0,
	CLIENT_JIAMI = 1,
};

// ��ȡ�ļ���md5У��
const char *file_checksum(const char *szFileName, int offset = 0);
// �����������Լ����Ϣ
void			UpdateSelfCompleteCheckInfo();
// �����������Լ��
bool			IsSelfComplete();
void DesDecryptFileFromBuffer( const char* pszSrcContent, const char* pszDestFile, EnCodeOp eOp );
char* getDecryptFileBuffer( const char* pszSrcFile, EnCodeOp nCurEncodeOp );
void DesDecryptFile( const char* pszSrcFile, const char* pszDestFile, EnCodeOp eOp );
bool Key_Go(char *Out, char *In, long datalen, const char *Key, int keylen, EnCodeOp Type);
#endif
