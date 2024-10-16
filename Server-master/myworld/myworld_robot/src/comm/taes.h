/* crypto/aes/aes.h -*- mode:C; c-file-style: "eay" -*- */
/* ====================================================================
 * Copyright (c) 1998-2002 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.openssl.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    openssl-core@openssl.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.openssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 */

/* Author:hardway
   DATE:2009/08/05
   Suitable for X86(_64) Platform
 */

#ifndef HEADER_AES_H
#define HEADER_AES_H

#include <stdlib.h>

#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_AMD64) || defined(_M_X64))
# define SWAP(x) (_lrotl(x, 8) & 0x00ff00ff | _lrotr(x, 8) & 0xff00ff00)
# define GETU32(p) SWAP(*((u32 *)(p)))
# define PUTU32(ct, st) { *((u32 *)(ct)) = SWAP((st)); }
#else
# define GETU32(pt) (((u32)(pt)[0] << 24) ^ ((u32)(pt)[1] << 16) ^ ((u32)(pt)[2] <<  8) ^ ((u32)(pt)[3]))
# define PUTU32(ct, st) { (ct)[0] = (u8)((st) >> 24); (ct)[1] = (u8)((st) >> 16); (ct)[2] = (u8)((st) >>  8); (ct)[3] = (u8)(st); }
#endif

#define MAXKC   (256/32)
#define MAXKB   (256/8)
#define MAXNR   14


#define AES_ENCRYPT	1
#define AES_DECRYPT	0
#define AES_MAXNR 14
#define AES_BLOCK_SIZE 16
#define AES_KEY_BITS   128

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;


#if (defined(_WIN32) || defined(_WIN64)) && !defined(__MINGW32__)
typedef unsigned __int64 u64;
#define U64(C)	C##UI64
#elif defined(__arch64__)
typedef unsigned long u64;
#define U64(C)	C##UL
#else
typedef unsigned long long u64;
#define U64(C)	C##ULL
#endif


#ifdef  __cplusplus
extern "C" {
#endif

/* This should be a hidden type, but EVP requires that the size be known */
struct aes_key_st 
{

    unsigned int rd_key[4 *(AES_MAXNR + 1)];

    int rounds;
};

typedef struct aes_key_st AES_KEY;

struct aes_st
{
	AES_KEY stEncKey;
	AES_KEY stDecKey;
};

typedef struct aes_st AES;


static const unsigned char pIV[16] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};

/*设置加解密的密钥*/
/*0:成功
  <0:失败*/
int taes_setkey(AES *pAESCtx,const unsigned char *pKey);


/*密文是16字节的整数倍*/
/*密文格式:明文+paddding(var)+常量"xxxxx"(5bytes)+padding长度(1bytes)*/
int taes_encrypt(const char* pInBuf, int nInBufLen, AES *pAESCtx, char* pOutBuf, int *pOutBufLen);

/**/
int taes_decrypt(const char* pInBuf, int nInBufLen, AES *pAESCtx, char* pOutBuf, int *pOutBufLen);






#ifdef  __cplusplus
}
#endif

#endif /* !HEADER_AES_H */
