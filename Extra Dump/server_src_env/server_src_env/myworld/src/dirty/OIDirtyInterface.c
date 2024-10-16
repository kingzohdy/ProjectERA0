#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _USESHM_

#include <errno.h>
#include <time.h>
#include <sys/shm.h>

#endif

#ifndef _WIN32
#include <unistd.h>
#endif

#define g_szChnName  		"chn.txt"
#define g_szEngName  		"eng.txt"
#define g_szRegName  		"reg.txt"
#define g_szPyyName			"pyy.txt"

#include "OIDirtyInterface.h"
#include "OIDirty.h"

DIRTY_CHECK * g_pstOIDirtyCheck		= NULL;
DIRTY_CHECK * g_pstOIDirtyCheckPinyin	= NULL;

/*
col1 关键字级别
col2 关键字
col3 关键字类型 1中文 2英文 3正则 4拼音
#1	注意：最多1000行，每行最多255字符
1	领奖|奖	1
2	妈|妈	1
1	money	2
10   fuck	2
2   shit	2
1	falun	4
1	[s|3]g[0-9a-zA-Z]{0,6}\.[0-9a-zA-Z]{2,6}\.	3
1	登[录|陆].*[礼|奖]	3
*/

static int divideDirtyFile(char *pszDirtyFile, char *pszChnFile, char *pszEngFile, char *pszRegFile, char *pszPyyFile) 
{
	char 	szBuf[1024] 	= {0};
	char 	szBbk[1024] 	= {0};
	char 	szFile[1024]	= {0};
	
	char *	pch		= 0;
	char *	idx		= 0;
	int 		type	= 0;
	int 		pid		= 0;
	
	FILE *	file		= 0;
	FILE *	file_Chn	= 0;
	FILE *	file_Eng	= 0;
	FILE *	file_Reg	= 0;
	FILE *	file_pyy	= 0;
	
	memset(szFile, 0x00, sizeof(szFile));

#ifndef _WIN32

	if ((idx = strrchr(pszDirtyFile, '/')))
	{
		memcpy(szFile, pszDirtyFile, idx-pszDirtyFile+1);
	}
	
	pid = getpid();
	
#else

	if ((idx = strrchr(pszDirtyFile, '\\')))
	{
		memcpy(szFile, pszDirtyFile, idx-pszDirtyFile+1);
	}
	
	pid = 1;
	
#endif

	sprintf(pszChnFile, "%s%s.%d", szFile, g_szChnName, pid);
	sprintf(pszEngFile, "%s%s.%d", szFile, g_szEngName, pid);
	sprintf(pszRegFile, "%s%s.%d", szFile, g_szRegName, pid);
	sprintf(pszPyyFile, "%s%s.%d", szFile, g_szPyyName, pid);
	
	file		= fopen(pszDirtyFile, "r");
	file_Chn	= fopen(pszChnFile,	"w");
	file_Eng	= fopen(pszEngFile,	"w");
	file_Reg	= fopen(pszRegFile,	"w");
	file_pyy	= fopen(pszPyyFile,	"w");
	
	if (!file || !file_Chn || !file_Eng || !file_Reg || !file_pyy) 
	{
		//fprintf (stderr, "%s %d:%s\n", __FILE__, __LINE__, strerror(errno));
		if (file)
		{
			fclose(file);
		}
		
		if (file_Chn)
		{
			fclose(file_Chn);
		}
		
		if (file_Eng)
		{
			fclose(file_Eng);
		}
		
		if (file_Reg)
		{
			fclose(file_Reg);
		}
		
		if (file_pyy)
		{
			fclose(file_pyy);
		}
		
		return -1;
	}
	
	while (!feof(file)) 
	{
		memset(szBuf,  0,  sizeof(szBuf));
		if (fgets((szBuf), sizeof((szBuf)), file)) 
		{
			strncpy(szBbk, szBuf, sizeof(szBbk));
			//memcpy(szBbk, szBuf, sizeof(szBbk));

			pch = strtok(szBuf, " \t\r\n");
			if(pch == NULL)
			{
				continue;
			}
			
			pch = strtok(NULL, " \t\r\n");
			if (pch == NULL)
			{
				continue;
			}
			
			pch = strtok(NULL, " \t\r\n");
			if (pch == NULL)
			{
				continue;
			}
			
			type = atoi(pch);
			switch(type) 
			{
				case 1:
					fprintf(file_Chn, "%s", szBbk);
					break;
				case 2:
					fprintf(file_Eng, "%s", szBbk);
					break;
				case 3:
					fprintf(file_Reg, "%s", szBbk);
					break;
				case 4:
					fprintf(file_pyy, "%s", szBbk);
					break;
				default:
				{
				}
			}
		}
	}
	
	fclose(file);
	fclose(file_Chn);
	fclose(file_Eng);
	fclose(file_Reg);
	fclose(file_pyy);
	
	return 0;
}


#ifdef _USESHM_

#define DIRTY_SHM_KEY		0xd123
#define DIRTY_SHM_MGC		0x321d
#define DIRTY_SHM_LEN		(sizeof(struct dirtyShmDesc) + sizeof(OI_DIRTY_CORE) * 2)

struct dirtyShmDesc
{
	int	iMagic;
	int	iCreate;
};

void * OIDirtyShmat(int iKey)
{
	int				iShmSize;
	int				iShmID;

	iShmSize		= DIRTY_SHM_LEN;
	iShmID			= shmget(iKey, iShmSize, IPC_CREAT | IPC_EXCL | 0666);

	if (iShmID < 0)
	{
		if( errno != EEXIST )
		{
			return NULL;
		}

		iShmID		= shmget(iKey, iShmSize, 0666);
		if (iShmID < 0)
		{
			iShmID	= shmget(iKey, 0, 0666);
			if( iShmID < 0 )
			{
				return NULL;
			}
			else
			{
				if (shmctl(iShmID, IPC_RMID, NULL))
				{
					return NULL;
				}

				iShmID	= shmget(iKey, iShmSize, IPC_CREAT|IPC_EXCL|0666);
				if( iShmID < 0 )
				{
					return NULL;
				}
			}
		}
	}
	
	return shmat(iShmID, NULL, 0);
}

#endif

//int InitializeDirtyCheck(char * pszChnKeyFile, char * pszEngKeyFile, char * pszRegFile, char * pszPinyinFile)
int InitializeDirtyCheck(char * pszDirtyFile, int iForce)
{
	int	iRet;
	char *pShm;	
	struct dirtyShmDesc *pstDesc;

	char pszChnKeyFile[128] = {0};
	char pszEngKeyFile[128] = {0};
	char pszRegFile[128] = {0};
	char pszPinyinFile[128] = {0};
	
	((void)iForce);
	((void)pstDesc);
	((void)pShm);

	//if ((pszChnKeyFile == NULL) && (pszEngKeyFile == NULL) && (pszRegFile == NULL) && (pszPinyinFile == NULL))
	if (pszDirtyFile == NULL)
	{
		g_pstOIDirtyCheck		    = NULL;
		g_pstOIDirtyCheckPinyin	= NULL;
		return -1;
	}

	if (g_pstOIDirtyCheck != NULL)
	{
		OI_Dirty_Fini( g_pstOIDirtyCheck );
		
		free(g_pstOIDirtyCheck);
		g_pstOIDirtyCheck = NULL;
	}

	if (g_pstOIDirtyCheckPinyin != NULL)
	{
		OI_Dirty_Fini( g_pstOIDirtyCheckPinyin );

		free(g_pstOIDirtyCheckPinyin);
		g_pstOIDirtyCheckPinyin = NULL;
	}
	
	g_pstOIDirtyCheck		    = malloc(sizeof(DIRTY_CHECK));
	g_pstOIDirtyCheckPinyin	= malloc(sizeof(DIRTY_CHECK));

	if ((g_pstOIDirtyCheck == NULL) || (g_pstOIDirtyCheckPinyin == NULL))
	{
		return -1;
	}

#ifdef _USESHM_
	
	pShm	= (char *)OIDirtyShmat(DIRTY_SHM_KEY);
	pstDesc = (struct dirtyShmDesc *)pShm;

	if (pShm == NULL)
	{
		return -1;
	}

	if (pstDesc->iMagic != DIRTY_SHM_MGC)
	{
		memset(pShm, 0x00, DIRTY_SHM_LEN);

		pstDesc->iMagic		= DIRTY_SHM_MGC;
		pstDesc->iCreate	= time(NULL);

		iForce				= 1;
	}

	OI_Dirty_Attach(g_pstOIDirtyCheck, pShm + sizeof(struct dirtyShmDesc));
	OI_Dirty_Attach(g_pstOIDirtyCheckPinyin, pShm + sizeof(struct dirtyShmDesc) + sizeof(OI_DIRTY_CORE));

	if (iForce)
	{
		iRet = divideDirtyFile(pszDirtyFile, pszChnKeyFile, pszEngKeyFile, pszRegFile, pszPinyinFile);
		if (iRet == -1)
		{
			return -1;
		}

		if (pszEngKeyFile != NULL)
		{
			iRet = OI_Dirty_Eng_GetFromFile(g_pstOIDirtyCheck, pszEngKeyFile);
			if(iRet < 0)
			{//load English keywords file failed
				return -2;
			}
		}

		if (pszChnKeyFile !=NULL)
		{
			iRet = OI_Dirty_Chn_GetFromFile(g_pstOIDirtyCheck, pszChnKeyFile);
			if (iRet < 0)
			{//load Chinese keywords failed
				return -3;
			}
		}

		if (pszRegFile != NULL)
		{
			iRet = OI_Dirty_Regular_GetFromFile(g_pstOIDirtyCheck, pszRegFile);
			if (iRet < 0)
			{//load regulation failed
				return -4;
			}
		}

		if (pszPinyinFile != NULL)
		{
			iRet = OI_Dirty_Regular_GetFromFile(g_pstOIDirtyCheckPinyin, pszPinyinFile);
			if (iRet < 0)
			{//load pinyin failed
				return -5;
			}
		}

		unlink(pszChnKeyFile);
		unlink(pszEngKeyFile);
		unlink(pszRegFile);
		unlink(pszPinyinFile);
	}
#else
	{
		iRet = divideDirtyFile(pszDirtyFile, pszChnKeyFile, pszEngKeyFile, pszRegFile, pszPinyinFile);
		if (iRet == -1)
		{
			return -1;
		}

		if (OI_Dirty_Init(g_pstOIDirtyCheck) != 0 || OI_Dirty_Init(g_pstOIDirtyCheckPinyin) != 0)
		{
			return -1;
		}

		if (pszEngKeyFile != NULL)
		{
			iRet = OI_Dirty_Eng_GetFromFile(g_pstOIDirtyCheck, pszEngKeyFile);
			if(iRet < 0)
			{//load English keywords file failed
				return -2;
			}
		}

		if (pszChnKeyFile !=NULL)
		{
			iRet = OI_Dirty_Chn_GetFromFile(g_pstOIDirtyCheck, pszChnKeyFile);
			if (iRet < 0)
			{//load Chinese keywords failed
				return -3;
			}
		}

		if (pszRegFile != NULL)
		{
			iRet = OI_Dirty_Regular_GetFromFile(g_pstOIDirtyCheck, pszRegFile);
			if (iRet < 0)
			{//load regulation failed
				return -4;
			}
		}

		if (pszPinyinFile != NULL)
		{
			iRet = OI_Dirty_Regular_GetFromFile(g_pstOIDirtyCheckPinyin, pszPinyinFile);
			if (iRet < 0)
			{//load pinyin failed
				return -5;
			}
		}
		
		unlink(pszChnKeyFile);
		unlink(pszEngKeyFile);
		unlink(pszRegFile);
		unlink(pszPinyinFile);
	}
#endif

#ifdef _TDIRTY_
	OI_Dirty_Report( g_pstOIDirtyCheck );
	OI_Dirty_Report( g_pstOIDirtyCheckPinyin );
#endif

	return 0;
}

int OIDirtyFini()
{
	if (g_pstOIDirtyCheck != NULL)
	{
		OI_Dirty_Fini(g_pstOIDirtyCheck);
		
		free(g_pstOIDirtyCheck);
		g_pstOIDirtyCheck = NULL;
	}

	if (g_pstOIDirtyCheckPinyin != NULL)
	{
		OI_Dirty_Fini(g_pstOIDirtyCheckPinyin);

		free(g_pstOIDirtyCheckPinyin);
		g_pstOIDirtyCheckPinyin = NULL;
	}

	return 0;
}

int OIDirtyCheck(int iDoorLevel, char * pszContent, int * piDirtyLevel)
{
	int iBegIdx	= 0;
	int iEndIdx	= 0;

	if (g_pstOIDirtyCheck == NULL)
	{//not initialized
		return -100;
	}

	return OI_Dirty_Check(g_pstOIDirtyCheck, iDoorLevel, pszContent, piDirtyLevel, 
		&iBegIdx, &iEndIdx);
}

int OIDirtyCheckChn(int iDoorLevel, char * pszContent, int * piDirtyLevel)
{
	int	iRet;
	int iBeginIdx	= 0;
	int iEndIdx		= 0;

	if (g_pstOIDirtyCheck == NULL)
	{//not initialized
		return -1;
	}

	iRet = OI_Dirty_Chn_Check(g_pstOIDirtyCheck, iDoorLevel, 
		(unsigned char *)pszContent, piDirtyLevel, &iBeginIdx, &iEndIdx, 1);
	if (iRet > 0)
	{// dirty Chinese
		return 1;
	}

	return 0;
}

int OIDirtyCheckEng(int iDoorLevel, char * pszContent, int * piDirtyLevel)
{
	int	iRet;
	int iBeginIdx	= 0;
	int iEndIdx		= 0;

	if (g_pstOIDirtyCheck == NULL)
	{//not initialized
		return -1;
	}

	iRet = OI_Dirty_Eng_Check(g_pstOIDirtyCheck, iDoorLevel, 
		(unsigned char *)pszContent, piDirtyLevel, &iBeginIdx, &iEndIdx, 1);
	if (iRet > 0)
	{// dirty English
		return 1;
	}

	return 0;
}

int OIDirtyCheckReg(int iDoorLevel, char * pszContent, int * piDirtyLevel, 
	int * piBeginIdx, int * piEndIdx)
{
	int	iRet;

	if (g_pstOIDirtyCheck == NULL)
	{//not initialized
		return -1;
	}

	iRet = OI_Dirty_Reg_Check(g_pstOIDirtyCheck, iDoorLevel, 
		(unsigned char *)pszContent, piDirtyLevel, piBeginIdx, piEndIdx, 1);
	if (iRet > 0)
	{// dirty regular
		return 1;
	}

	return 0;
}

int OIDirtyCheckPinyin(int iDoorLevel, char * pszContent, int * piDirtyLevel)
{
	int		iRet = 0;
	int		iBegIdx, iEndIdx;

	char	szSpellCode[1024]	= {0};
	char	szPinyinCode[1024]	= {0};

	iBegIdx	= 0;
	iEndIdx = 0;

	if (g_pstOIDirtyCheckPinyin == NULL)
	{//not initialized
		return -1;
	}

	iRet = MakeMapCode(pszContent, szSpellCode, szPinyinCode, sizeof(szSpellCode));
	if (iRet < 0)
	{// error make spell and pinyin code
		return -2;
	}
	
	iRet = OI_Dirty_Reg_Check(g_pstOIDirtyCheckPinyin, iDoorLevel, 
		(unsigned char *)szPinyinCode, piDirtyLevel, &iBegIdx, &iEndIdx, 1);
	if (iRet > 0)
	{//dirty pinyin
		return 1;
	}
	
	return 0;
}

int OIDirtyCheckEx(int iDoorLevel, char * pszContent, int * piDirtyLevel, 
	int * piBeginIdx, int * piEndIdx)
{
	if (g_pstOIDirtyCheck == NULL)
	{//not initialized
		return -100;
	}

	return OI_Dirty_Check(g_pstOIDirtyCheck, iDoorLevel, pszContent, piDirtyLevel, 
		piBeginIdx, piEndIdx);
}

int OIDirtyCheckChnEx(int iDoorLevel, char * pszContent, int * piDirtyLevel, 
	int * piBeginIdx, int * piEndIdx)
{
	int	iRet;

	if (g_pstOIDirtyCheck == NULL)
	{//not initialized
		return -1;
	}

	iRet = OI_Dirty_Chn_Check(g_pstOIDirtyCheck, iDoorLevel, 
		(unsigned char *)pszContent, piDirtyLevel, piBeginIdx, piEndIdx, 1);
	if (iRet > 0)
	{// dirty Chinese
		return 1;
	}

	return 0;
}

int OIDirtyCheckEngEx(int iDoorLevel, char * pszContent, int * piDirtyLevel, 
	int * piBeginIdx, int * piEndIdx)
{
	int	iRet;

	if (g_pstOIDirtyCheck == NULL)
	{//not initialized
		return -1;
	}

	iRet = OI_Dirty_Eng_Check(g_pstOIDirtyCheck, iDoorLevel, 
		(unsigned char *)pszContent, piDirtyLevel, piBeginIdx, piEndIdx, 1);
	if (iRet > 0)
	{// dirty English
		return 1;
	}

	return 0;
}

int OIDirtyReplace(int iDoorLevel, char * pszContent, int * piDirtyLevel)
{
	if (g_pstOIDirtyCheck == NULL)
	{//not initialized
		return -100;
	}

	return OI_Dirty_Rreplace(g_pstOIDirtyCheck, iDoorLevel, pszContent, 
		piDirtyLevel, 1);
}

int OIDirtyReplace2(int iDoorLevel, char * pszContent, int * piDirtyLevel)
{
	if (g_pstOIDirtyCheck == NULL)
	{//not initialized
		return -100;
	}

	return OI_Dirty_Rreplace(g_pstOIDirtyCheck, iDoorLevel, pszContent, 
		piDirtyLevel, 0);
}

int InitializeDirtyCheckEx(char * pszBuff,  int iBuff)
{
	int		iLevel;
	int		iType;
	
	char*	pch;
	char		szLine[1024];
	
	if (g_pstOIDirtyCheck != NULL)
	{
		OI_Dirty_Fini(g_pstOIDirtyCheck);
		
		free(g_pstOIDirtyCheck);
		g_pstOIDirtyCheck = NULL;
	}

	if (g_pstOIDirtyCheckPinyin != NULL)
	{
		OI_Dirty_Fini(g_pstOIDirtyCheckPinyin);

		free(g_pstOIDirtyCheckPinyin);
		g_pstOIDirtyCheckPinyin = NULL;
	}
	
	g_pstOIDirtyCheck		    = malloc(sizeof(DIRTY_CHECK));
	g_pstOIDirtyCheckPinyin	= malloc(sizeof(DIRTY_CHECK));

	if ((g_pstOIDirtyCheck == NULL) || (g_pstOIDirtyCheckPinyin == NULL))
	{
		return -1;
	}

	if (OI_Dirty_Init(g_pstOIDirtyCheck) != 0 || OI_Dirty_Init(g_pstOIDirtyCheckPinyin) != 0)
	{
		return -1;
	}

	pszBuff[iBuff] = 0;
	
	pch	= strtok (pszBuff,"\r\n");
	while (pch != NULL)
	{
		szLine[0]	= 0;
		iLevel	= -1;
		iType	= -1;
		
		sscanf(pch,"%d %s %d", &iLevel, szLine, &iType);
		if  (iType == 1)
		{
			OI_Dirty_Chn_GetFromLineBuff(g_pstOIDirtyCheck, pch);				
		}
		else if (iType == 2)
		{
			OI_Dirty_Eng_GetFromLineBuff(g_pstOIDirtyCheck, pch);				
		}
		else if (iType == 3)
		{
			OI_Dirty_Reg_GetFromLineBuff(g_pstOIDirtyCheck, pch);
		}
		else if (iType ==4)
		{
			OI_Dirty_Reg_GetFromLineBuff(g_pstOIDirtyCheckPinyin, pch);
		}
		
		pch = strtok (NULL, "\r\n");
	}

#ifdef _TDIRTY_
	OI_Dirty_Report(g_pstOIDirtyCheck);
	OI_Dirty_Report(g_pstOIDirtyCheckPinyin);
#endif

	return 0;
}

