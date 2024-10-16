#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "convert.h"

#ifndef _WIN32

#include <errno.h>
#include <iconv.h>

#else

#include <windows.h>

#endif

#define CP_GB2312 20936

int UTF8ToGB2312(char * pOut, size_t iBufSize, char * pText, size_t iLen)
{
#ifdef _WIN32
	int		iWcharLenth;
	int		iMultByteLenth;

	wchar_t	*pwTemp;

	pwTemp				= (wchar_t *)malloc(sizeof(wchar_t) * iLen + 16);
	if (!pwTemp)
	{
		return 0;
	}
	
	iWcharLenth		= MultiByteToWideChar(CP_UTF8, 0, pText, -1, pwTemp, iLen == 1 ? 2 : iLen);
	if (0 == iWcharLenth)
	{
		free (pwTemp);
		return 0;
	}
	
	iMultByteLenth	= WideCharToMultiByte(CP_GB2312, 0, pwTemp, iWcharLenth, pOut, iBufSize, NULL, NULL);
	if (0 == iMultByteLenth)
	{
		free (pwTemp);
		return 0;
	}

	free (pwTemp);
	return iMultByteLenth;

#else

	iconv_t	cd;
	size_t	rc;

	char	*inptr	=	pText; 
	char	*outptr	=	pOut;

	size_t	outsize	=	iBufSize;

	cd	=   iconv_open("GBK","UTF-8");
	if (cd == (iconv_t)-1)
	{
		return -1;
	}

	memset (pOut, 0x00, iBufSize);

	iconv(cd, NULL, NULL, NULL, NULL);
	rc	=	iconv(cd, &inptr,(size_t *)&iLen, &outptr, (size_t *)&outsize);
	if (rc == (size_t)(-1))
	{
		int saved_errno = errno;
		iconv_close(cd);
		errno = saved_errno;

		return -1;
	}

	iconv_close(cd);
	return outptr - pOut;

#endif

}

int GB2312ToUTF8(char * pOut, size_t iBufSize, char * pText, size_t iLen)
{
#ifdef _WIN32
	int			iWcharLenth;
	int			iMultByteLenth;
	wchar_t		*pwTemp;

	pwTemp				= (wchar_t *)malloc(sizeof(wchar_t) * iLen + 16);
	if (!pwTemp)
	{
		return 0;
	}

	iWcharLenth		= MultiByteToWideChar(CP_GB2312, MB_PRECOMPOSED, pText, -1, pwTemp, iLen == 1 ? 2 : iLen);
	if (0 == iWcharLenth) 
	{

#ifdef _DEBUG
		printf("%s:×ªÂë´íÎó2£¬´íÎóºÅ:%d\n", pwTemp, GetLastError());
#endif
		free (pwTemp);
		return 0;
	}
	
	iMultByteLenth	= WideCharToMultiByte(CP_UTF8, 0, pwTemp, iWcharLenth, pOut, iBufSize, NULL, NULL);
	if (0 == iMultByteLenth) 
	{
#ifdef _DEBUG
		printf("%s:×ªÂë´íÎó2£¬´íÎóºÅ:%d\n", pwTemp, GetLastError());
#endif

		free (pwTemp);
		return 0; 
	}

	free (pwTemp);
	return iMultByteLenth;

#else

	iconv_t	cd;
	size_t	rc;

	char	*inptr	=	pText; 
	char	*outptr	=	pOut;

	size_t	outsize	=	iBufSize;

	cd	=   iconv_open("UTF-8", "GBK");
	if (cd == (iconv_t)-1)
	{
		return -1;
	}

	memset (pOut, 0x00, iBufSize);

	iconv(cd, NULL, NULL, NULL, NULL);
	rc = iconv(cd, &inptr,(size_t *)&iLen, &outptr,(size_t *)&outsize);

	if (rc == (size_t)-1)
	{
		int saved_errno = errno;
		iconv_close(cd);
		errno = saved_errno;

		return -1;
	}

	iconv_close(cd);
	return outptr - pOut;
	
#endif
}

