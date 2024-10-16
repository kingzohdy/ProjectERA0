#include <errno.h>
#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#ifndef _WIN32

#include <unistd.h>
#include <stdint.h>
#else

#include <windows.h>

#endif

#include "base.h"

#ifndef _WIN32

//#if defined(__i386__) && !defined(_WIN64)

/*
#define uint2korr(A)    (*((unsigned short *) (A)))
#define uint4korr(A)    (*((unsigned long  *) (A)))

#define int2store(T,A)  *((unsigned short*) (T))= (uint16_t) (A)
#define int4store(T,A)  *((long *) (T))= (long) (A)*/

#define uint2korr(A)				(*((uint16_t *) (A)))
#define uint4korr(A)				(*((uint32_t *) (A)))

#define int2store(T,A)				*((uint16_t*) (T))= (uint16_t) (A)
#define int4store(T,A)				*((uint32_t *) (T))= (uint32_t) (A)

//#endif /* __i386__ */

/*
 获得Exchange区的Md5String信息
 */

int getMd5String(char *pShm, char szMd5String[])
{
	if (pShm == 0)
	{
		return -1;
	}
	
	memcpy(szMd5String, pShm, 16);
	return 0;
}

/*
 获得Exchange区的File文件路径
 */
int getMd5FilePath(char *pShm, char szMd5File[])
{
	unsigned short uLen=0;
	if (pShm == 0)
	{
		return -1;
	}
	
	pShm += 16;
	
	//get len
	uLen=uint2korr(pShm);
	pShm+=2;
	
	memcpy(szMd5File, pShm, uLen);
	szMd5File[uLen] = 0;
	
	return 0;
}

int setMd5String(char *pShm, char *pszMd5String)
{
	if (pShm == 0)
	{
		return -1;
	}
	
	memcpy(pShm, pszMd5String, 16);
	return 0;
}

int setMd5FilePath(char *pShm, char *pszMd5File)
{
	unsigned short uLen = 0;
	if (pShm == 0)
	{
		return -1;
	}
	
	pShm += 16;
	
	uLen = strlen(pszMd5File);
	int2store(pShm, uLen);
	pShm += 2;
	
	memcpy(pShm, pszMd5File, uLen);
	return 0;
}

/*
 检查文件合法性
 */
int validFile(const char *szFilePath)
{
	struct stat sbuf;
	memset(&sbuf, 0x00, sizeof(sbuf));
	
	if (stat(szFilePath, &sbuf) == -1) 
	{
		return -1;
	}

	return 0;
}

int copyFile(const char *szfrom, const char *szDest)
{
	unsigned char data [4096] = {0};
	
	int		bytes = 0;
	FILE *	from  = 0;
	FILE *	dest  = 0;
	
	struct stat srcbuf;
	struct stat detbuf;
	
	memset(&srcbuf, 0x00, sizeof(srcbuf));
	memset(&detbuf, 0x00, sizeof(detbuf));
	
	if (stat(szfrom, &srcbuf) < 0 || stat(szDest, &detbuf) < 0)
	{
		return -1;
	}
	
	if (srcbuf.st_ino == detbuf.st_ino && srcbuf.st_dev == detbuf.st_dev)
	{
		//same file
		return 0;
	}

	if (strcmp(szfrom, szDest) == 0)
	{
		return 0;
	}

	from	= fopen(szfrom, "r");
	dest	= fopen(szDest, "w");
	
	if (from == NULL || dest == NULL)
	{
		if (from)
		{
			fclose(from);
		}
		
		if (dest)
		{
			fclose(dest);
		}
		
		return -1;
	}
	
	while ((bytes = fread (data, 1, 4096, from)) != 0)
	{
		fwrite(data, 1, bytes, dest);
	}

	fclose(from);
	fclose(dest);
	
	return 0;
}
#else

int validFile(const char *szFilePath)
{

#ifndef INVALID_FILE_ATTRIBUTES
	#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif

	if ( GetFileAttributesA(szFilePath) != INVALID_FILE_ATTRIBUTES || GetLastError() != ERROR_FILE_NOT_FOUND )
	{
		return 0;
	}

	return -1;
}

#endif
