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

// 获取文件的md5校验
const char *file_checksum(const char *szFileName, int offset = 0);
// 更新完整性自检查信息
void			UpdateSelfCompleteCheckInfo();
// 进行完整性自检查
bool			IsSelfComplete();
void DesDecryptFileFromBuffer( const char* pszSrcContent, const char* pszDestFile, EnCodeOp eOp );
char* getDecryptFileBuffer( const char* pszSrcFile, EnCodeOp nCurEncodeOp );
void DesDecryptFile( const char* pszSrcFile, const char* pszDestFile, EnCodeOp eOp );
bool Key_Go(char *Out, char *In, long datalen, const char *Key, int keylen, EnCodeOp Type);
#endif
