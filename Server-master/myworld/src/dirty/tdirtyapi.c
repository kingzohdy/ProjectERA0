/*
**  @file $RCSfile: tdirtyapi.c,v $
**  general description of this module
**  $Id: tdirtyapi.c,v 1.3 2013/01/29 11:12:46 tjw Exp $
**  @author $Author: tjw $
**  @date $Date: 2013/01/29 11:12:46 $
**  @version $Revision: 1.3 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include <errno.h>
#include <stdio.h>

#include <string.h>
#include <stdlib.h>

#include "tdirtyapi.h"
#include "OIDirtyInterface.h"

#include "base.h"
#include "md5.h"
#include "sema.h"
#include "convert.h"

#ifndef _WIN32
#include "pal/pal.h"
#include "taa/tagentapi.h"
#else
#include <windows.h>
#endif

#define DIRTYLIBKEY			0xccddeeff
#define TMPVOCABULARY		TOS_TMP_DIR"vocabulary.%d.txt"

#define ENGINE_TYPE_GBK		0
#define ENGINE_TYPE_UTF		1
#define CONVERTERROR		-200

#ifndef _WIN32
static char *g_pShm=0;					//共享内存
static void *g_asem=0;					//信号量
static LPEXCHANGEMNG g_pstExcMng = 0;	//Exchange Block句柄
#endif

static char g_szDirtyFile[1024]	= {0};	//脏字文件起始地址
static char g_szDirtyMd5File[1024]	= {0};	//脏字文件起始地址
static char g_szMd5Version[16]	= {0};	//md5String版本
static char g_szVocabulary[256]	= {0};	//临时词汇表

static int	g_iAutoUpdate		= 1;	//自动更新默认关闭
static int	g_iDirtyEngine		= ENGINE_TYPE_GBK;	//调用脏字检索engine

struct tagSpecialChar
{
	char		cLen;
	char		spec[7];
};

typedef struct tagSpecialChar		SPECIALCHAR;
typedef struct tagSpecialChar		*LPSPECIALCHAR;

struct tagSCFHandle
{
	int iCount;
	SPECIALCHAR astSpecChar[256];		
};

typedef struct tagSCFHandle	SCFHANDLE;
typedef struct tagSCFHandle	*LPSCFHANDLE;

static int initExchange(unsigned int uiBussid)
{

#ifndef _WIN32

	int 	iRet = 0;
	iRet 		= agent_api_init(&g_pstExcMng);
	if (iRet)
	{
		return -1;
	}
	
	iRet 		= agent_api_register(g_pstExcMng, ID_APPID_DIRTY, uiBussid);
	if (iRet == -1)
	{
		return -1;
	}
	else if (iRet == -2)
	{
		//logging:no more space 	
	}
	
	g_pShm = agent_api_get_freebufptr(g_pstExcMng, ID_APPID_DIRTY, uiBussid);
	
	return g_pShm == NULL ? -1 : 0;
#else

	return -1;
#endif

}

static int updateExchange(char szMd5String[])
{

#ifndef _WIN32

	char *pShm = g_pShm;
	if (pShm == 0)
	{
		return -1;
	}
	
	memcpy(pShm, szMd5String, 16);
	return 0;

#else

	return -1;

#endif
}

static int reloadDirtyCheck(char *pszDirtyFile, int iForce)
{
	int	iRet = 0;
	/*
	char szChnFile[1024] = {0};
	char szEngFile[1024] = {0};
	char szRegFile[1024] = {0};
	char szPyyFile[1024] = {0};
	*/

	if (pszDirtyFile == NULL || validFile(pszDirtyFile) == -1)
	{
		return -1;
	}
	/*
	iRet = divideDirtyFile(pszDirtyFile, szChnFile, szEngFile, szRegFile, szPyyFile);
	if (iRet == -1)
	{
		//some error skip
	}
	*/
	//iRet = InitializeDirtyCheck(szChnFile, szEngFile, szRegFile, szPyyFile);
	iRet = InitializeDirtyCheck(pszDirtyFile, iForce);

	if (iRet == 0)
	{
		Md5File(g_szMd5Version, pszDirtyFile);
	}
	/*
	unlink(szChnFile);
	unlink(szEngFile);
	unlink(szRegFile);
	unlink(szPyyFile);
	*/

	return iRet;
}

/***********************************************
Exchange Struct
Md5String(16字节)+Stringlen(2字节)+String(变长)
这个地方要处理一下防止 lib不停的刷新换版本
***********************************************/

static int checkExchange(void)
{

#ifndef _WIN32
	int		iRet		= 0;
	char *	pShm		= g_pShm;
	
	char 	szMd5String	[16] = {0};
	char 	szMd5File	[1024] = {0};
	//char	szMd5DirtyFile[16] = {0};
	
	/*
	 pShm有没有attach上
	 */
	if (pShm == 0)
	{
		return -1;
	}
	
	/*
	 get md5String
	 */
	getMd5String(pShm, szMd5String);
	getMd5FilePath(pShm, szMd5File);
	
	if (memcmp(szMd5String, g_szMd5Version, 16))
	//if (memcmp(szMd5DirtyFile, g_szMd5Version, 16))
	{
		//检查临时文件是否存在
		if (validFile(szMd5File) == -1)
		{
			return -2;
		}
		
		//文件存在
		iRet = reloadDirtyCheck(szMd5File, 1);
		if (iRet == 0)
		{
			//加载成功开始复制文件
			//copy /tmp/dirtiword.306cc3a6efbfd51735e07a31907e5b98
			copyFile(szMd5File, g_szDirtyFile);
		}
		else
		{
			//重新加载原来的文件
			reloadDirtyCheck(g_szDirtyFile, 1);
		}

		return 1;
	}
	
	return 0;

#else
	return -1;

#endif
}

int tMakeMapCode(char * pszCharacter, char * pszSpellCode, char * pszPinyinCode, int iMaxCodeLenth)
{
	return MakeMapCode(pszCharacter, pszSpellCode, pszPinyinCode, iMaxCodeLenth);
}

int tSetDirtyMd5File(char *pszDirtyMd5File)
{
	snprintf(g_szDirtyMd5File, sizeof(g_szDirtyMd5File), pszDirtyMd5File);
	return 0;
}


int tInitializeDirtyCheck(char *pszDirtyFile, unsigned int bussid)
{

#ifndef _WIN32
	int		iRet	= 0;
	char *	pShm	= 0;
	FILE *	fp		= 0;

	char	szMd5DirtyFile[16] = {0};
	char	szMd5AgentShm [16] = {0};
	char	szFileAgentShm[1024] = {0};

	if (pszDirtyFile == NULL)
	{
		/* 传入空参数自动更新设置打开 */
		g_iAutoUpdate	=	1;

		sprintf (g_szVocabulary, TMPVOCABULARY, bussid);
		pszDirtyFile	= g_szVocabulary;
		
		fp = fopen(pszDirtyFile, "r");
		if (fp)
		{
			fclose(fp);
		}
		else
		{
			printf ("vocabulary(%s) is unreadable\n", pszDirtyFile);
			return -1;
		}
	}
	
	if (g_pShm || g_asem)
	{
		return 0;
	}

	// 第一次调用tInitializeDirtyCheck初始化Exchange
	g_asem = semcreate(DIRTYLIBKEY, 1);
	if (g_asem == 0)
	{
		//fprintf (stderr, "Error:%s %d:Init semaphore Fail:%s\n", __FILE__, __LINE__, strerror(errno));
		exit(-1);
	}
	
	//lock()
	semacquire(g_asem);
	if (initExchange(bussid))
	{
		//fprintf (stderr, "Error:%s %d:Init ShareMemory Fail\n", __FILE__, __LINE__);

		//unlock
		semrelease(g_asem);
		exit(-1);
	}
	
	//复制路径
	snprintf(g_szDirtyFile, sizeof(g_szDirtyFile), "%s", pszDirtyFile);
	if (g_szDirtyMd5File[0])
	{
		setMd5FilePath	(pShm, g_szDirtyMd5File);
		g_szDirtyMd5File[0] = 0;
	}

	//Exchange 初始化完毕立刻检查Exchange区
	pShm = g_pShm;
	getMd5String	(pShm, szMd5AgentShm);
	getMd5FilePath	(pShm, szFileAgentShm);

	Md5File(szMd5DirtyFile, pszDirtyFile);
	//查看Exchange的数据有没有更新
	if (memcmp(szMd5DirtyFile, szMd5AgentShm, 16) && g_iAutoUpdate == 1)
	{
		//如果版本信息不一致并且自动更新打开
		iRet = reloadDirtyCheck(szFileAgentShm, 1);
		if (iRet == 0)
		{
			//加载Exchange区的临时文件成功
			//复制文件
			copyFile(szFileAgentShm, pszDirtyFile);
		}
		else
		{
			//加载Agent的dirtyword失败了恢复当前的pszDirtyFile
			iRet = reloadDirtyCheck(pszDirtyFile, 1);
			if (iRet == 0)
			{
				//把当前的版本信息写入Exchange
				updateExchange(szMd5DirtyFile);
			}
		}
	}
	else
	{
		iRet = reloadDirtyCheck(pszDirtyFile, 0);
	}
	
	//unlock
	semrelease(g_asem);
	return iRet;
	
#else

	sprintf(g_szDirtyFile,"%s", pszDirtyFile);
	return reloadDirtyCheck(g_szDirtyFile, 1);
	
#endif
	
}

int tDirtyCheck(int iDoorLevel, char * pszContent, int * piDirtyLevel)
{
	if (g_iAutoUpdate)
	{
		tDirtyUpdateCheck();
	}
	
	return OIDirtyCheck(iDoorLevel, pszContent, piDirtyLevel);
}

int tDirtyCheckChn(int iDoorLevel, char * pszContent, int * piDirtyLevel)
{
	if (g_iAutoUpdate)
	{
		tDirtyUpdateCheck();
	}
	
	return OIDirtyCheckChn(iDoorLevel, pszContent, piDirtyLevel);
}

int tDirtyCheckEng(int iDoorLevel, char * pszContent, int * piDirtyLevel)
{
	if (g_iAutoUpdate)
	{
		tDirtyUpdateCheck();
	}
	
	return OIDirtyCheckEng(iDoorLevel, pszContent, piDirtyLevel);
}

int tDirtyCheckReg(int iDoorLevel, char * pszContent, int * piDirtyLevel)
{
	int iBegIdx, iEndIdx;

	iBegIdx = 0;
	iEndIdx	= 0;

	if (g_iAutoUpdate)
	{
		tDirtyUpdateCheck();
	}
	
	return OIDirtyCheckReg(iDoorLevel, pszContent, piDirtyLevel, &iBegIdx, &iEndIdx);
}

int tDirtyCheckPinyin(int iDoorLevel, char * pszContent, int * piDirtyLevel)
{
	if (g_iAutoUpdate)
	{
		tDirtyUpdateCheck();
	}
	
	return OIDirtyCheckPinyin(iDoorLevel, pszContent, piDirtyLevel);
}

int tDirtyCheckEx(int iDoorLevel, char * pszContent, int * piDirtyLevel, 
	int * piBeginIdx, int * piEndIdx)
{
	if (g_iAutoUpdate)
	{
		tDirtyUpdateCheck();
	}
	
	return OIDirtyCheckEx(iDoorLevel, pszContent, piDirtyLevel, piBeginIdx, piEndIdx);
}

int tDirtyCheckChnEx(int iDoorLevel, char * pszContent, int * piDirtyLevel, 
	int * piBeginIdx, int * piEndIdx)
{
	if (g_iAutoUpdate)
	{
		tDirtyUpdateCheck();
	}
	
	return OIDirtyCheckChnEx(iDoorLevel, pszContent, piDirtyLevel, piBeginIdx, piEndIdx);
}

int	tDirtyCheckEngEx(int iDoorLevel, char * pszContent, int * piDirtyLevel, 
	int * piBeginIdx, int * piEndIdx)
{
	if (g_iAutoUpdate)
	{
		tDirtyUpdateCheck();
	}
	
	return OIDirtyCheckEngEx(iDoorLevel, pszContent, piDirtyLevel, piBeginIdx, piEndIdx);
}

int	tDirtyReplace(int iDoorLevel, char * pszContent, int * piDirtyLevel)
{
	if (g_iAutoUpdate)
	{
		tDirtyUpdateCheck();
	}

	return OIDirtyReplace(iDoorLevel, pszContent, piDirtyLevel);
}

int	tDirtyReplace2(int iDoorLevel, char * pszContent, int * piDirtyLevel)
{
	if (g_iAutoUpdate)
	{
		tDirtyUpdateCheck();
	}

	return OIDirtyReplace2(iDoorLevel, pszContent, piDirtyLevel);
}

const char *tGetDirtywordVersion()
{
	return (const char *)g_szMd5Version;
}

void tSetDirtywordVersion(char* pszDirtyFile)
{
	Md5File(g_szMd5Version, pszDirtyFile);
}


void tDirtySetAutoUpdate(int iAutoUpdate)
{
	g_iAutoUpdate = iAutoUpdate;
}

int tDirtyUpdateCheck()
{
	return checkExchange();
}

int tDirtyFini()
{
	return OIDirtyFini();
}

int utf8DirtyCheck(int iDoorLevel, char * pszUTF8Content, int * piDirtyLevel)
{
	int		iRet = 0;
	char	pGbk[1024 * 4] = {0};

	if (g_iDirtyEngine == ENGINE_TYPE_GBK)
	{
		iRet	=	UTF8ToGB2312(pGbk, sizeof(pGbk), pszUTF8Content, strlen(pszUTF8Content));
	#ifdef _WIN32
		if (iRet == 0)
		{
			return -1;
		}
	#else
		if (iRet == -1)
		{
			return -1;
		}
	#endif

		iRet	=	tDirtyCheck(iDoorLevel, pGbk, piDirtyLevel);
	}

	return iRet;
}

int utf8DirtyReplace(int iDoorLevel, char * pszUTF8Content, int * piDirtyLevel)
{
	int		iRet;
	int		iUTF8;
	int		iGBK;

	char	pGbk[1024 * 4] = {0};

	if (g_iDirtyEngine == ENGINE_TYPE_GBK)
	{
		iUTF8	=	strlen(pszUTF8Content);
		iGBK	=	UTF8ToGB2312(pGbk, sizeof(pGbk), pszUTF8Content, iUTF8);
		
	#ifdef _WIN32
		if (iGBK == 0)
		{
			return CONVERTERROR;
		}
	#else
		if (iGBK == -1)
		{
			return CONVERTERROR;
		}
	#endif

		iRet	=	tDirtyReplace(iDoorLevel, pGbk, piDirtyLevel);
		if (iRet < 0)
		{
			return iRet;
		}

		iUTF8	=	GB2312ToUTF8(pszUTF8Content, iUTF8, pGbk, iGBK);

	#ifdef _WIN32
		if (iUTF8 == 0)
		{
			return CONVERTERROR;
		}
	#else
		if (iUTF8 == -1)
		{
			return CONVERTERROR;
		}
	#endif
	}

	return 0;
}

size_t utf8_strlen(const char *s)
{
	size_t i,len;
	size_t count;

	unsigned char * us;
	
	count	= 0;
	len		= strlen(s);
	us		= (unsigned char *)s;
	
	for(i = 0; i < len; count ++)
	{
		if (us[i] < 128)
		{
			i ++;
		}
		else if (us[i] < 224)
		{
			i += 2;
		}
		else if (us[i] < 240)
		{
			i += 3;
		}
		else if (us[i] < 248)
		{
			i += 4;
		}
		else if (us[i] < 252)
		{
			i += 5;
		}
		else if (us[i] < 255)
		{
			i += 6;
		}
	}
	
	return count;
}

const char* utf8_strstr(const char *s1, const char *s2, size_t n)
{
	size_t	i,j;
	size_t	len;
	size_t	count;

	unsigned char * us1;
	unsigned char * us2;
	
	count	= 0;
	len		= strlen(s1);

	us1		= (unsigned char *)s1;
	us2		= (unsigned char *)s2;
	
	for(i = 0; i < len; count ++)
	{
		j = i;
		
		if (us1[i] < 128)
		{
			i += 1;
		}
		else if (us1[i] < 224)
		{
			i += 2;
		}
		else if (us1[i] < 240)
		{
			i += 3;
		}
		else if (us1[i] < 248)
		{
			i += 4;
		}
		else if (us1[i] < 252)
		{
			i += 5;
		}
		else if (us1[i] < 255)
		{
			i += 6;
		}
		
		if ((i - j) == n)
		{
			if (memcmp(&us1[j], us2, n) == 0)
			{
				return &s1[j];
			}
		}
	}
	
	return NULL;
}

int utf8SpecialCharFilterInit(unsigned long int* piHandle, const char * pszFileName)
{
	int		i;
	int		iRet;
	
	char*	p;
	char		buffer[4096];
	char		gb2312[4096];
	
	FILE*	fp;
	SCFHANDLE* pstHandle;
	
	pstHandle	=	(SCFHANDLE*) calloc(1, (int) sizeof(*pstHandle));
	if (!pstHandle)
	{
		return -1;
	}
	
	fp 			=	fopen(pszFileName, "r");
	if (!fp)
	{
		return -1;
	}

	i = 0;
	while (!feof(fp)) 
	{
		memset(gb2312,  0,  sizeof(gb2312));
		if (fgets((gb2312), sizeof((gb2312)), fp)) 
		{
			if ((p = strstr(gb2312, "\r")))
			{
				*p	= 0;
			}
			else if ((p = strstr(gb2312, "\n")))
			{
				*p	= 0;
			}
			
			iRet	= GB2312ToUTF8(buffer, sizeof(buffer), gb2312, strlen(gb2312));
#ifdef _WIN32
			if (iRet == 0)
			{
				printf ("%d\n", ERROR_INSUFFICIENT_BUFFER);
				if (GetLastError() == ERROR_INVALID_FLAGS)
				{
					printf ("%s\n", gb2312);
					strncpy(buffer, gb2312, sizeof(buffer));
				}
				else
				{
					continue;
				}
			}
#else
			if (iRet == -1)
			{
				continue;
			}
					
#endif
			pstHandle->astSpecChar[i].cLen = strlen(buffer);
			if (pstHandle->astSpecChar[i].cLen == 0)
			{
				pstHandle->astSpecChar[i].cLen		= 1;
				pstHandle->astSpecChar[i].spec[0]	= '\r';

				i ++;

				pstHandle->astSpecChar[i].cLen		= 1;
				pstHandle->astSpecChar[i].spec[0]	= '\n';
			}
			else
			{
				strncpy(pstHandle->astSpecChar[i].spec, buffer, 7);
			}
			
			i++;
		}
	}
	
	fclose(fp);
	pstHandle->iCount = i;

	if( piHandle )
	{
		*piHandle	=	(unsigned long int)pstHandle;
	}
	
	return 0;
}

int utf8SpecialCharFilterScan(unsigned long int iHandle, const char *pszString)
{
	int i;
	SCFHANDLE*	pstHandle;
	
	pstHandle	=	(SCFHANDLE*) iHandle;

	for (i = 0; i < pstHandle->iCount; i++)
	{
		if (utf8_strstr(pszString, pstHandle->astSpecChar[i].spec, pstHandle->astSpecChar[i].cLen))
		{
			return 1;
		}
		
	}
	
	return 0;
}

void utf8SpecialCharFilterFini(unsigned long int* piHandle)
{
	if( !piHandle )
	{
		return;
	}
	
	if( *piHandle )
	{
		free( (void*)*piHandle );
	}
	
	*piHandle	=	0;

	return;
}

int tInitializeDirtyCheckEx(char * pszBuff,  int iBuff)
{
	return InitializeDirtyCheckEx(pszBuff, iBuff);
}

