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

// 是否缺少文件
bool isAnyFileNotExist();
// 检验是否有写入完整性信息，若没有，则写入
bool HasMD5Writed();
// 更新完整性自检查信息
void UpdateSelfCompleteCheckInfo();
// 进行完整性自检查
bool IsSelfComplete();
void DesDecryptFileFromBuffer( const char* pszSrcContent, const char* pszDestFile, EnCodeOp eOp );
void DesDecryptFile( const char* pszSrcFile, const char* pszDestFile, EnCodeOp eOp );
char* getDecryptFileBuffer( const char* pszSrcFile, EnCodeOp nCurEncodeOp );
bool Key_Go(char *Out, char *In, long datalen, const char *Key, int keylen, bool Type);
#endif
