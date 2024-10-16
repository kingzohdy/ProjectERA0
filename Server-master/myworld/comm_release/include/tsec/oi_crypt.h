
#ifndef _OI_CRYPT_H
#define _OI_CRYPT_H

#define TEA_KEY_LEN	16
#define ENCRYPT_VERSION	110
#define C2C_ENCRYPT_VERSION	300

void OicqEncrypt(unsigned short shVersion, const char* pInBuf, int nInBufLen, const char* pKey, char* pOutBuf, int *pOutBufLen);
char OicqDecrypt(unsigned short shVersion, const char* pInBuf, int nInBufLen, const char* pKey, char* pOutBuf, int *pOutBufLen);
char OicqDecrypt3(unsigned long lUin, unsigned short shVersion, const char* pInBuf, int nInBufLen, const char* pKey, char* pOutBuf, int *pOutBufLen);
int MakeSigniture(short shVer, unsigned long lUin, short shFlag, const char* pKey, char* pOutBuf, int *pOutBufLen);
int MakeValueAddedSigniture(unsigned long lUin, char *sValueAddedFlag, char* pOutBuf);
int MakeValueAddedSignitureNew(short shVer, unsigned long lUin, char *sValueAddedFlag, const char* pKey, char* pOutBuf, int *pOutBufLen);

#endif
