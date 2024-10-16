#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "OIDirty.h"

void TrimStr(char * pszString);

static int OI_Dirty_Mixed					(unsigned char * sCheckStr);
static int OI_Drity_Chn_Add_Item		(DIRTY_CHECK * pstSelf, unsigned char * sDirtyWord, unsigned char * sKeyWord, int iCount, int iLevel);
static int OI_Dirty_Chn_Check_Word	(int iKeyOff, unsigned char  * sCheckStr, unsigned char  * sReservBuff, unsigned char * sDirtyStr, int * iRealCount);
static int OI_Dirty_Eng_Check_Word	(unsigned char * sCheckStr, unsigned char * sDirtyStr, int * iRealCount);

#ifdef _WIN32
int strncasecmp(const char *s1, const char *s2, size_t n)
{
	if (n == 0)
	{
		return 0;
	}

	while (n-- != 0 && tolower(*s1) == tolower(*s2))
	{
		if (n == 0 || *s1 == '\0' || *s2 == '\0')
		{
			break;
		}

		s1++;
		s2++;
	}

	return tolower(*(unsigned char *) s1) - tolower(*(unsigned char *) s2);
}
#endif


#ifdef _USESHM_
void OI_Dirty_Attach(DIRTY_CHECK * pstSelf, void * ptrAddr)
{
	pstSelf->m_pstDirtyCore		= (OI_DIRTY_CORE *)ptrAddr;
	pstSelf->m_pstChnIndexTab	= &(pstSelf->m_pstDirtyCore->stChnDirtyIndex);
	pstSelf->m_pstDirtyEngRec  = pstSelf->m_pstDirtyCore->astEngDirtyRec;
}
#endif

int OI_Dirty_Init(DIRTY_CHECK * pstSelf)
{
	int	i;
	
	pstSelf->m_pstDirtyCore = (OI_DIRTY_CORE *)malloc(sizeof(OI_DIRTY_CORE));
	if (pstSelf->m_pstDirtyCore == NULL)
	{
		return -1;
	}

	memset((void *)pstSelf->m_pstDirtyCore, 0, sizeof(OI_DIRTY_CORE));
	pstSelf->m_pstChnIndexTab	= &(pstSelf->m_pstDirtyCore->stChnDirtyIndex);
	pstSelf->m_pstDirtyEngRec	= pstSelf->m_pstDirtyCore->astEngDirtyRec;
	
	pstSelf->m_pstDirtyCore->aiRegWordCount	= 0;
	pstSelf->m_pstDirtyCore->aiEngWordCount	= 0;
	pstSelf->m_pstDirtyCore->aiChnWordCount	= 0;
	
	pstSelf->m_pstDirtyCore->iEnable 			= 0;
	memset(pstSelf->m_pstDirtyCore->astEngDirtyRec,	0x0,	sizeof(pstSelf->m_pstDirtyCore->astEngDirtyRec));
	memset(pstSelf->m_pstDirtyCore->astChnDirtyRec,	0x0,	sizeof(pstSelf->m_pstDirtyCore->astChnDirtyRec));
	memset(pstSelf->m_pstDirtyCore->astRegDirtyRec,	0x0,	sizeof(pstSelf->m_pstDirtyCore->astRegDirtyRec));

	for (i = 0; i < C_MAX_TABLE_LEN; i++)
	{
		pstSelf->m_pstChnIndexTab->iDirtyIndexTable[i] = -1;
	}
	
	return 0;
}

int OI_Dirty_Fini(DIRTY_CHECK * pstSelf)
{
	int i = 0;

#ifndef _WIN32
	if (pstSelf->m_pstDirtyCore != NULL)
	{
		//正则要释放 否则泄露内存
		for(i = 0; i < E_MAX_WORD_NUM && i < pstSelf->m_pstDirtyCore->aiRegWordCount; i++)
		{
			regfree( &(pstSelf->m_pstDirtyCore->astRegDirtyRec[i].stRegular) );
		}
	}
#endif

	if (pstSelf->m_pstDirtyCore != NULL)
	{

#ifndef _USESHM_
		free (pstSelf->m_pstDirtyCore);
		pstSelf->m_pstDirtyCore = NULL;
#endif
	}

	return 0;
}

void OI_Dirty_Report(DIRTY_CHECK * pstSelf)
{
	int	i;
	
	printf ("Eng Words Count:%d\n", pstSelf->m_pstDirtyCore->aiEngWordCount);
	for (i = 0; i < pstSelf->m_pstDirtyCore->aiEngWordCount; i++)
	{
		printf ("Word:%s\tLevel:%d\n", pstSelf->m_pstDirtyCore->astEngDirtyRec[i].sDirtyStr, pstSelf->m_pstDirtyCore->astEngDirtyRec[i].iLevel);
	}

	printf ("Chn Words Count:%d\n", pstSelf->m_pstDirtyCore->aiChnWordCount);
	for (i = 0; i < pstSelf->m_pstDirtyCore->aiChnWordCount; i++)
	{
		printf ("Word:%s|%s\tLevel:%d\n", pstSelf->m_pstDirtyCore->astChnDirtyRec[i].sDirtyStr, pstSelf->m_pstDirtyCore->astChnDirtyRec[i].sKeyWord, pstSelf->m_pstDirtyCore->astChnDirtyRec[i].iLevel);
	}

	printf ("Reg Words Count:%d\n", pstSelf->m_pstDirtyCore->aiRegWordCount);
	for (i = 0; i < pstSelf->m_pstDirtyCore->aiRegWordCount; i++)
	{
		printf ("Word:%s\tLevel:%d\n", pstSelf->m_pstDirtyCore->astRegDirtyRec[i].sDirtyStr, pstSelf->m_pstDirtyCore->astRegDirtyRec[i].iLevel);
	}
}

int OI_Dirty_Eng_GetFromLineBuff (DIRTY_CHECK * pstSelf, const char* pszBuff)
{
	int			iCount;
	char 		szLineBuf[1024];
	
	if (!pszBuff)
	{
		return -1;
	}

	if (pstSelf->m_pstDirtyCore->aiEngWordCount >= E_MAX_WORD_NUM)
	{
		return -1;
	}
	
	strncpy(szLineBuf, pszBuff, sizeof(szLineBuf));
	TrimStr(szLineBuf);
	
	if(szLineBuf[0] == '#')
	{
		return -1;
	}
	
	iCount		= pstSelf->m_pstDirtyCore->aiEngWordCount;
	sscanf(szLineBuf,"%d %s", &(pstSelf->m_pstDirtyCore->astEngDirtyRec[iCount].iLevel), pstSelf->m_pstDirtyCore->astEngDirtyRec[iCount].sDirtyStr);
	
	pstSelf->m_pstDirtyCore->astEngDirtyRec[iCount].sDirtyStr[C_MAX_WORD_LEN] = 0;
	pstSelf->m_pstDirtyCore->aiEngWordCount ++;
	pstSelf->m_pstDirtyCore->iEnable = 1;
	
	return 0;
}

int OI_Dirty_Chn_GetFromLineBuff (DIRTY_CHECK * pstSelf, const char* pszBuff)
{
	int			iRet;
	int			iLevel;
	
	char			szLineBuf[1024];
	char* 		pToken;
	
	unsigned char	szDirtyStr[C_MAX_WORD_LEN+1];
	unsigned char	szKeyWord[3];

	if (!pszBuff)
	{
		return -1;
	}

	if (pstSelf->m_pstDirtyCore->aiChnWordCount >= C_MAX_WORD_NUM)
	{
		return-1;
	}
	
	strncpy(szLineBuf, pszBuff, sizeof(szLineBuf));
	TrimStr(szLineBuf);
	
	if(szLineBuf[0] == '#')
	{
		return -1;
	}
	
	sscanf(szLineBuf,"%d %s", &iLevel, szDirtyStr);
	szDirtyStr[C_MAX_WORD_LEN]	= 0;
	
	pToken		= (char *)strchr((const char *)szDirtyStr, '|');
	if (!pToken)
	{
		szKeyWord[0] = 0;
	}
	else
	{
		*pToken	= 0;

		
		strncpy((char*)szKeyWord, pToken+1, 3);
		szKeyWord[2] = 0;
	}
	
	iRet 			= OI_Drity_Chn_Add_Item(pstSelf, szDirtyStr, szKeyWord, pstSelf->m_pstDirtyCore->aiChnWordCount, iLevel);
	if ( iRet == 0 )
	{
		pstSelf->m_pstDirtyCore->aiChnWordCount ++;
		pstSelf->m_pstDirtyCore->iEnable = 1;
	}
	
	return 0;
}

int OI_Dirty_Reg_GetFromLineBuff(DIRTY_CHECK * pstSelf, const char * pszBuff)
{
	int			iCount;
	int			iReads;
	
	char			szLineBuf[1024];

	if (!pszBuff)
	{
		return -1;
	}

	if (pstSelf->m_pstDirtyCore->aiRegWordCount >= E_MAX_WORD_NUM)
	{
		return-1;
	}
	
	strncpy(szLineBuf, pszBuff, sizeof(szLineBuf));
	TrimStr(szLineBuf);
	
	if(szLineBuf[0] == '#')
	{
		return -1;
	}
	
	iCount		= pstSelf->m_pstDirtyCore->aiRegWordCount;
	iReads		= sscanf(szLineBuf,"%d %s",&(pstSelf->m_pstDirtyCore->astRegDirtyRec[iCount].iLevel), pstSelf->m_pstDirtyCore->astRegDirtyRec[iCount].sDirtyStr);
	if (iReads < 2)
	{
		return -2;
	}
	
	pstSelf->m_pstDirtyCore->astRegDirtyRec[iCount].sDirtyStr[E_MAX_WORD_NUM] = 0;

#ifndef _WIN32
		char	errbuf[1024];
		int	err	= regcomp(&(pstSelf->m_pstDirtyCore->astRegDirtyRec[iCount].stRegular), (const char*)pstSelf->m_pstDirtyCore->astRegDirtyRec[iCount].sDirtyStr, REG_EXTENDED);
		
		if (err)
		{
			regerror(err, &(pstSelf->m_pstDirtyCore->astRegDirtyRec[iCount].stRegular), errbuf, sizeof(errbuf));
			return -1;
		}

		pstSelf->m_pstDirtyCore->aiRegWordCount ++;
		pstSelf->m_pstDirtyCore->iEnable = 1;
		
#endif

	return 0;
}

int OI_Dirty_Eng_GetFromFile(DIRTY_CHECK * pstSelf, const char * pszEngFile)
{
	FILE* 	fpEngFile = NULL;
	
	char		szLineBuf[1024];
	int		iCount;
	
	if(!pszEngFile)
	{
		return -1;
	}

	fpEngFile = fopen(pszEngFile, "r");
	if(!fpEngFile)
	{
		return -2;
	}

	pstSelf->m_pstDirtyCore->iEnable = 0;
	pstSelf->m_pstDirtyCore->aiEngWordCount = 0;
	memset(pstSelf->m_pstDirtyCore->astEngDirtyRec, 0, sizeof(pstSelf->m_pstDirtyCore->astEngDirtyRec));

	iCount = 0;
	for(;;)
	{
		if(feof(fpEngFile))
		{
			break;
		}

		memset(szLineBuf, 0x0, sizeof(szLineBuf));
		fgets((char *)szLineBuf, sizeof(szLineBuf)-1, fpEngFile);
		TrimStr(szLineBuf);

		if (strlen(szLineBuf) == 0)
		{
			continue;
		}
		
		if(szLineBuf[0] == '#')
		{
			continue;
		}

		sscanf(szLineBuf,"%d %s",&(pstSelf->m_pstDirtyCore->astEngDirtyRec[iCount].iLevel), pstSelf->m_pstDirtyCore->astEngDirtyRec[iCount].sDirtyStr);
		pstSelf->m_pstDirtyCore->astEngDirtyRec[iCount].sDirtyStr[C_MAX_WORD_LEN] = 0;
		
		iCount++;
		if (iCount >= E_MAX_WORD_NUM)
		{
			break;
		}
	}
	
	fclose(fpEngFile);
	
	pstSelf->m_pstDirtyCore->aiEngWordCount	= iCount;
	pstSelf->m_pstDirtyCore->iEnable			= 1;

	return 0;
}

int OI_Dirty_Chn_GetFromFile(DIRTY_CHECK * pstSelf, const char * pszChnFile)
{
	FILE *			fpChnFile;

	char				szLineBuf[1024];
	char *			pNextToken;
	
	unsigned char		szDirtyStr[C_MAX_WORD_LEN+1];
	unsigned char		szKeyWord[3];


	int				iLevel	= 0;
	int				iRet	= 0;

	int				iCount;

	if (!pszChnFile)
	{
		return -1;
	}

	fpChnFile = fopen(pszChnFile, "r");
	if (!fpChnFile)
	{
		return -2;
	}

	pstSelf->m_pstDirtyCore->iEnable			= 0;
	pstSelf->m_pstDirtyCore->aiChnWordCount	= 0;

	memset(pstSelf->m_pstDirtyCore->astChnDirtyRec, 0x0, sizeof(pstSelf->m_pstDirtyCore->astChnDirtyRec));

	for (iCount = 0; iCount < C_MAX_TABLE_LEN; iCount++)
	{
		pstSelf->m_pstChnIndexTab->iDirtyIndexTable[iCount] = -1;
	}

	iCount	= 0;

	for(;;)
	{
		if ( feof(fpChnFile) )
		{
			break;
		}

		memset(szLineBuf, 0x0, sizeof(szLineBuf));
		fgets((char *)szLineBuf, sizeof(szLineBuf)-1, fpChnFile);
		TrimStr(szLineBuf);

		if(szLineBuf[0] == '#')
		{
			continue;
		}

		pNextToken = strtok(szLineBuf, " \t\r\n");
		if (!pNextToken)
		{
			continue;
		}

		iLevel		=	atoi(pNextToken);
		pNextToken	=	strtok(NULL, "|\r\n");

		if (!pNextToken)
		{
			continue;
		}

		strncpy ((char*)szDirtyStr, (const char *)pNextToken, C_MAX_WORD_LEN);		
		szDirtyStr[C_MAX_WORD_LEN]	=	0;
		TrimStr((char*)szDirtyStr);
		
		pNextToken	= strtok(NULL, "\r\n");
		if (pNextToken)
		{
			strncpy((char*)szKeyWord, (const char *)pNextToken, 3);
			szKeyWord[2] = 0;
		}

		iRet = OI_Drity_Chn_Add_Item(pstSelf, szDirtyStr,szKeyWord,iCount,iLevel);
		if ( iRet == 0 )
		{
			iCount++;
		}

		if (iCount >= C_MAX_WORD_NUM)
		{
			break;
		}
	}

	fclose(fpChnFile);
	
	pstSelf->m_pstDirtyCore->aiChnWordCount	= iCount;
	pstSelf->m_pstDirtyCore->iEnable			= 1;

	return 0;
}

int OI_Dirty_Regular_GetFromFile(DIRTY_CHECK * pstSelf, const char * pszRegFile)
{
	FILE * 	fpEngFile 	= NULL;
	int		iCount 		= 0;
	
	char		szLineBuf[1024];
	
	if(!pszRegFile)
	{
		return -1;
	}
	
	fpEngFile = fopen(pszRegFile, "r");
	if(!fpEngFile)
	{
		return -2;
	}

	pstSelf->m_pstDirtyCore->iEnable = 0;
	pstSelf->m_pstDirtyCore->aiRegWordCount = 0;
	memset(pstSelf->m_pstDirtyCore->astRegDirtyRec, 0, sizeof(pstSelf->m_pstDirtyCore->astRegDirtyRec));

	iCount = 0;
	for(;;)
	{
		if(feof(fpEngFile))
		{
			break;
		}

		memset(szLineBuf, 0x0, sizeof(szLineBuf));
		fgets((char *)szLineBuf, sizeof(szLineBuf)-1, fpEngFile);
		TrimStr(szLineBuf);
		if(szLineBuf[0] == '#')
		{
			continue;
		}
		
		int reads = sscanf(szLineBuf,"%d %s",&(pstSelf->m_pstDirtyCore->astRegDirtyRec[iCount].iLevel), pstSelf->m_pstDirtyCore->astRegDirtyRec[iCount].sDirtyStr);
		if (reads < 2)
		{
			continue;
		}
		pstSelf->m_pstDirtyCore->astRegDirtyRec[iCount].sDirtyStr[E_MAX_WORD_NUM] = 0;
		
#ifndef _WIN32
		char	errbuf[1024];
		int	err = regcomp(&(pstSelf->m_pstDirtyCore->astRegDirtyRec[iCount].stRegular), (const char*)pstSelf->m_pstDirtyCore->astRegDirtyRec[iCount].sDirtyStr, REG_EXTENDED);
		
		if (err)
		{
			regerror(err, &(pstSelf->m_pstDirtyCore->astRegDirtyRec[iCount].stRegular), errbuf, sizeof(errbuf));
			continue;
		}
#endif
		
		iCount++;
		if (iCount >=E_MAX_WORD_NUM)
		{
			break;
		}
	}

	fclose(fpEngFile);
	pstSelf->m_pstDirtyCore->aiRegWordCount = iCount;
	pstSelf->m_pstDirtyCore->iEnable = 1;

	return 0;
}

int OI_Dirty_Mixed(/*DIRTY_CHECK * pstSelf, */unsigned char * sCheckStr)
{
	int				iCount = 0;
	unsigned char	cLoChar, cHiChar;

	while( (cLoChar = sCheckStr[iCount ++]) != '\0' )
	{
		if ( (char)cLoChar < 0 )
		{
			cHiChar	=	sCheckStr[iCount++];

			if (cHiChar == '\0') 
			{
				break;
			}

			if ( IS_DIGIT_DOUBLE(cLoChar, cHiChar) ) 
			{
				return 1;
			}
			else if ( IS_ENGLISH_DOUBLE(cLoChar, cHiChar) ) 
			{
				return 1;
			}
			else if ( IS_CHINESE_CHAR(cLoChar, cHiChar) )
			{
				continue;
			}
		}

		return 1;
	}

	return 0;
}

int OI_Drity_Chn_Add_Item(DIRTY_CHECK * pstSelf, unsigned char * sDirtyWord, unsigned char * sKeyWord, int iCount, int iLevel)
{
	OI_DIRTY_CHN_RECORD*	pstDirtyRecord;
	unsigned char*			sSubstr;
	int                    			iIndex, iOffset;
	
	if ((iCount >=C_MAX_WORD_NUM) || (iCount<0))
	{
		return -1;
	}

	if (pstSelf->m_pstChnIndexTab == NULL)
	{
		return -2;
	}

	pstDirtyRecord	= pstSelf->m_pstDirtyCore->astChnDirtyRec;
	strncpy((char *)(pstDirtyRecord[iCount].sDirtyStr), (const char *)sDirtyWord, C_MAX_WORD_LEN+1);
	pstDirtyRecord[iCount].sDirtyStr[C_MAX_WORD_LEN] = 0;

	strncpy((char *)(pstDirtyRecord[iCount].sKeyWord), (const char *)sKeyWord, 3);
	pstDirtyRecord[iCount].sKeyWord[2]	= 0;

	pstDirtyRecord[iCount].iLevel		= iLevel;
	pstDirtyRecord[iCount].bMix			= OI_Dirty_Mixed(sDirtyWord);

#if	0
	if (pstDirtyRecord[iCount].bMix)
	//if (1)
	{
		printf ("'%s'\n", sDirtyWord);
	}
#endif
	sSubstr			= (unsigned char *)strstr((const char *)sDirtyWord, (const char *)sKeyWord);
	if (sSubstr == NULL)
	{
		memset( &(pstDirtyRecord[iCount]), 0, sizeof(pstDirtyRecord[iCount]));
		return -3;	
	}

	pstDirtyRecord[iCount].iKeyOffset	= (short)(sSubstr - sDirtyWord); 
	pstDirtyRecord[iCount].iNextKey		= -1;
	 
	CAL_INDEX_OFFSET(iIndex,sKeyWord[0],sKeyWord[1]);
	iOffset = pstSelf->m_pstChnIndexTab->iDirtyIndexTable[iIndex];
	
	if (iOffset == -1)
	{
		pstSelf->m_pstChnIndexTab->iDirtyIndexTable[iIndex] = iCount;
	}
	else 
	{
		while (iOffset != -1)
		{
			if (strcmp( (const char *)(pstDirtyRecord[iOffset].sDirtyStr), (const char *)sDirtyWord) == 0 )
			{
	        		memset( &(pstDirtyRecord[iCount]), 0x0, sizeof(pstDirtyRecord[iCount]));
	        		return -4;
	       	}
			
	       	iIndex=iOffset;
	       	iOffset=pstDirtyRecord[iOffset].iNextKey;
	    }

	    pstDirtyRecord[iIndex].iNextKey=iCount;
	}

	return 0;
}

int OI_Dirty_Rreplace(DIRTY_CHECK * pstSelf, int iLevel, char * pszContent, int * piDirtyLevel, int iCond)
{
	int	i, iRet	= 0;
	int	iBegIdx	= 0;
	int	iEndIdx	= 0;
	
	if ((pstSelf->m_pstDirtyCore == NULL) || (pstSelf->m_pstChnIndexTab == NULL) || (pstSelf->m_pstDirtyEngRec == NULL))
	{
		return -1;	
	}

	if (!pstSelf->m_pstDirtyCore->iEnable) 
	{
		return -2;
	}

#ifdef _TDIRTY_
	printf ("filter by NT:%s\n", pszContent);
#endif

	do
	{
		iRet	=	OI_Dirty_Chn_Check(pstSelf, iLevel, (unsigned char *)pszContent, piDirtyLevel, &iBegIdx, &iEndIdx, iCond);
		if (iRet < 0)
		{
			return iRet;
		}
		else if (iRet == 0)
		{
			break;
		}

		for (i = iBegIdx; i < iEndIdx; i++)
		{
			pszContent[i] = '*';
		}

	} while (iRet > 0);

#ifdef _TDIRTY_
	printf ("filter by CN:%s\n", pszContent);
#endif

	do
	{
		iRet	=	OI_Dirty_Eng_Check(pstSelf, iLevel, (unsigned char *)pszContent, piDirtyLevel, &iBegIdx, &iEndIdx, iCond);
		if (iRet < 0)
		{
			return iRet;
		}
		else if (iRet == 0)
		{
			break;
		}

		for (i = iBegIdx; i < iEndIdx; i++)
		{
			pszContent[i] = '*';
		}

	} while (iRet > 0);

#ifdef _TDIRTY_
	printf ("filter by EN:%s\n", pszContent);
#endif

	do
	{
		iRet	=	OI_Dirty_Mix_Check(pstSelf, iLevel, (unsigned char *)pszContent, piDirtyLevel, 1, iCond);
		if (iRet < 0)
		{
			return iRet;
		}
		else if (iRet == 0)
		{
			break;
		}

	} while (iRet > 0);

#ifdef _TDIRTY_
	printf ("filter by Mx:%s\n", pszContent);
#endif

	return 0;
}

int OI_Dirty_Check(DIRTY_CHECK * pstSelf, int iLevel, char * pszContent, int * piDirtyLevel, int * piBeginIdx, int * piEndIdx)
{
	int iRet = 0;
	
	*piBeginIdx	= 0;
	*piEndIdx		= 0;

	if ((pstSelf->m_pstDirtyCore==NULL) || (pstSelf->m_pstChnIndexTab==NULL) || (pstSelf->m_pstDirtyEngRec == NULL))
	{
		return -1;	
	}

	if(!pstSelf->m_pstDirtyCore->iEnable) 
	{
		return -2;
	}

	iRet = OI_Dirty_Chn_Check(pstSelf, iLevel, (unsigned char *)pszContent, piDirtyLevel, piBeginIdx, piEndIdx, 1);
 	if (iRet > 0)
	{
 		return 1;
	}

	iRet = OI_Dirty_Eng_Check(pstSelf, iLevel, (unsigned char *)pszContent, piDirtyLevel, piBeginIdx, piEndIdx, 1);
 	if (iRet >  0)
	{
 		return 2;	
	}
	
	iRet = OI_Dirty_Mix_Check(pstSelf, iLevel, (unsigned char *)pszContent, piDirtyLevel, 0, 1);
	if(iRet > 0) 
	{
		return iRet;
	}
	
	return 0;
}

int OI_Dirty_Chn_Check(DIRTY_CHECK * pstSelf, int iLevel, unsigned char * sCheckStr, int * piDirtyLevel, int * piBeginIdx, int * piEndIdx, int iCond)
{
	unsigned char		sReservBuff[C_MAX_WORD_LEN], cLoChar, cHiChar;
	unsigned char		cIdxLoChar, cIdxHiChar, cTempIdxLoChar, cTempIdxHiChar;

	int				iRet = 0;
	int				iIsDirty=0, iCount=0, iBufCount=0, iIndex, iOffset;
	int				iKeyOff;
	int				iTempMidIdx, iTempBeginIdx, iTempEndIdx;
	int				iReadCount = 0;
	int				fResult;
	
	OI_DIRTY_CHN_RECORD	* pstDirtyRecord;

	*piBeginIdx		= 0;
	*piEndIdx			= 0;
	iCount			  = 0;
	iTempMidIdx		= 0;
	iTempBeginIdx	= 0;
	iTempEndIdx		= 0;

	if (sCheckStr == NULL)
	{
		return -1;	
	}
	
	if(!pstSelf->m_pstDirtyCore->iEnable) 
	{
		return -2;
	}

	pstDirtyRecord = pstSelf->m_pstDirtyCore->astChnDirtyRec;
	//   开始检查 字符串是否含有 ‘脏字眼
	while ((!iIsDirty) && ((cLoChar = sCheckStr[iCount++]) != '\0'))
	{
		cHiChar	= sCheckStr[iCount];
		if ( !IS_CHINESE_CHAR(cLoChar, cHiChar) )
		{
			continue;
		}

		if ( IS_CHINESE_PUNC(cLoChar, cHiChar) ) // 中文标点符号
		{
			iCount++;
			continue;
		}

		iCount	++;
		iTempMidIdx = iCount - 2;

		sReservBuff[(iBufCount++) % C_MAX_WORD_LEN] = cLoChar;
		sReservBuff[(iBufCount++) % C_MAX_WORD_LEN] = cHiChar;

		CAL_INDEX_OFFSET(iIndex, cLoChar, cHiChar);

		iOffset = pstSelf->m_pstChnIndexTab->iDirtyIndexTable[iIndex]; //该字是否关键字
		while ((iOffset != -1) && (iOffset >= 0) && ( iOffset < C_MAX_WORD_NUM))
		{
			iKeyOff = pstDirtyRecord[iOffset].iKeyOffset;
			if ((pstDirtyRecord[iOffset].sKeyWord[0] != cLoChar) || (pstDirtyRecord[iOffset].sKeyWord[1] != cHiChar))
			{
				//关键字不匹配可能发生在loadmem的时候
				break;
			}

			if (iCond < 0)
			{
				fResult	= (pstDirtyRecord[iOffset].iLevel < iLevel) ? 1 : 0;
			}
			else if (iCond == 0)
			{
				fResult	= (pstDirtyRecord[iOffset].iLevel == iLevel ) ? 1 : 0;
			}
			else
			{
				fResult	= (pstDirtyRecord[iOffset].iLevel >= iLevel ) ? 1 : 0;
			}
			
			//if (pstDirtyRecord[iOffset].iLevel >= iLevel)
			if (fResult)
			{
				// iBufCount : sReservBuf的最后一个字节偏移
				// iKeyOff     : 关键字在DirtyStr中的偏移
				// sCheckStr+iCount: 将剩下的句子传入检查
				// sReservBuff+(iBufCount-iKeyOff-2)%C_MAX_WORD_LEN : 将包含关键字的临时区间传入
				iRet = OI_Dirty_Chn_Check_Word(iKeyOff+2, sCheckStr+iCount,  //关键字后面段
										sReservBuff+(iBufCount-iKeyOff-2)%C_MAX_WORD_LEN,  //关键字前面一段
										//??norman 如果iBufCount-iKeyOff-2%C_MAX_WORD_LEN 接近C_MAX_WORD_LEN 可能会造成函数比较前段时访问sRescerBuff +len 越界
										pstDirtyRecord[iOffset].sDirtyStr, &iReadCount);

				if (iRet == 0)//脏话
				{
					iIsDirty = 1;
					*piDirtyLevel = pstDirtyRecord[iOffset].iLevel;

					//dirty index
					*piBeginIdx		= *piEndIdx = iTempMidIdx;
					iTempBeginIdx	= iTempEndIdx = iKeyOff;

					while (iTempBeginIdx > 1)
					{
						if ((*piBeginIdx <= 1) || (iTempBeginIdx <= 1))
						{
							break;
						}

						cIdxHiChar		= sCheckStr[*piBeginIdx - 1];
						cIdxLoChar		= sCheckStr[*piBeginIdx - 2];
						cTempIdxHiChar	= pstDirtyRecord[iOffset].sDirtyStr[iTempBeginIdx - 1];
						cTempIdxLoChar	= pstDirtyRecord[iOffset].sDirtyStr[iTempBeginIdx - 2];

						if ((cIdxLoChar == cTempIdxLoChar) && (cIdxHiChar == cTempIdxHiChar))
						{
							*piBeginIdx		-= 2;
							iTempBeginIdx	-= 2;
						}
						else if ((cIdxLoChar != cTempIdxLoChar) && (cIdxHiChar == cTempIdxHiChar))
						{
							*piBeginIdx		-= 1;
						}
						else if ((cIdxLoChar == cTempIdxHiChar) && (cIdxHiChar != cTempIdxHiChar))
						{
							*piBeginIdx		-= 1;
						}
						else
						{
							*piBeginIdx		-= 2;
						}
					}
					
					//Sean Add
					*piEndIdx			= iCount + iReadCount;
					//End
					
					break;
				}
			}

			iOffset = pstDirtyRecord[iOffset].iNextKey;  // 下一个含有该关键字得短语
		}
	}
	
	return iIsDirty;
}

int OI_Dirty_Chn_Check_Word(/*DIRTY_CHECK * pstSelf, */int iKeyOff, unsigned char * sCheckStr, unsigned char * sReservBuff, unsigned char * sDirtyStr, int * piRealCount)
{
	int            iCount=0, iCheckCount=0;
	unsigned char  cHiChar, cLoChar; 
	
	// 先比较临时存放区前面一段
	while (iCount < iKeyOff)
	{
		if (sReservBuff[iCount%C_MAX_WORD_LEN] != sDirtyStr[iCount])
		{
		    return 1;
		}

		iCount++;
	}

	// 比较剩余句子
	while (sDirtyStr[iCount] != '\0')
	{
		if (sCheckStr[iCheckCount] == '\0')
		{
			return 2;
		}

		cLoChar	= sCheckStr[iCheckCount++];   
	    cHiChar = sCheckStr[iCheckCount];

		if ( !IS_CHINESE_CHAR(cLoChar,cHiChar) ) // 是否全角非汉字
		{
			continue;
		}

	    if ( IS_CHINESE_PUNC(cLoChar,cHiChar) ) // 中文标点符号
	    {
			iCheckCount++;
	        continue;
	    }

		iCheckCount++;

		if (cLoChar!=sDirtyStr[iCount++])
		{
			return 3;
		}

		if (cHiChar!=sDirtyStr[iCount++])
		{
		   return 3;
		}
	}

	*piRealCount	=	iCheckCount;

	return 0;
}

int OI_Dirty_Eng_Check(DIRTY_CHECK * pstSelf, int iLevel,unsigned char * sCheckStr,int * piDirtyLevel, int * piBeginIdx, int * piEndIdx, int iCond)
{
	int iRet		= 0;

	unsigned char	cLoChar, cHiChar;
	//unsigned char	cIdxLoChar, cIdxHiChar;

	int			iIsDirty=0, iCount=0, iIndex=0;
	int			iTempBeginIdx, iTempEndIdx;

	int			fResult;
	
	*piBeginIdx	  = 0;
	*piEndIdx		  = 0;
	iTempBeginIdx = 0;
	iTempEndIdx	  = 0;

	if (sCheckStr == NULL)
	{
		return -1;	
	}

	if(!pstSelf->m_pstDirtyCore->iEnable) 
	{
		return -2;
	}

	//   开始检查 字符串是否含有 ‘脏字眼’ //
	while ((!iIsDirty) && ( (cLoChar = sCheckStr[iCount++]) != '\0'))
	{
		if ( IS_DOUBLE_CHAR(cLoChar) )
		{
			cHiChar = sCheckStr[iCount++];
			if(cHiChar == '\0')
			{
				break;
			}

			if (!IS_ENGLISH_DOUBLE(cLoChar, cHiChar))
			{
				continue;
			}
			else 
			{
				CONVERT_DOUBLE_TO_SINGLE(cLoChar, cLoChar, cHiChar);
				iTempBeginIdx = iCount - 2;
			}
		}
		else if (IS_ENGLISH_CHAR(cLoChar))
		{
			CONVERT_CAPITAL_CHAR(cLoChar, cLoChar);
			iTempBeginIdx = iCount - 1;
		}
		else
		{
			continue;
		}

		iIndex = 0;
		while (pstSelf->m_pstDirtyEngRec[iIndex ++].sDirtyStr[0] != '\0')
		{
			if (iCond < 0)
			{
				fResult	= (pstSelf->m_pstDirtyEngRec[iIndex-1].iLevel < iLevel) ? 1 : 0;
			}
			else if (iCond == 0)
			{
				fResult	= (pstSelf->m_pstDirtyEngRec[iIndex-1].iLevel == iLevel) ? 1 : 0;
			}
			else
			{
				fResult	= (pstSelf->m_pstDirtyEngRec[iIndex-1].iLevel >= iLevel) ? 1 : 0;
			}
			
			//字符cLochar是否为脏话的第一个字符
			//if ((pstSelf->m_pstDirtyEngRec[iIndex-1].iLevel >= iLevel) && EQUAL_ENGLISH_CHAR(cLoChar, pstSelf->m_pstDirtyEngRec[iIndex-1].sDirtyStr[0]))
			if (fResult && EQUAL_ENGLISH_CHAR(cLoChar, pstSelf->m_pstDirtyEngRec[iIndex-1].sDirtyStr[0]))
			{
				//比较后面串是否和脏话后面的相同
				iRet = OI_Dirty_Eng_Check_Word(sCheckStr+iCount, pstSelf->m_pstDirtyEngRec[iIndex-1].sDirtyStr+1, &iTempEndIdx);
				if (iRet == 0)
				{
					iIsDirty = 1;
					*piDirtyLevel	= pstSelf->m_pstDirtyEngRec[iIndex-1].iLevel;

					//Sean Add
					*piBeginIdx	= iTempBeginIdx;
					*piEndIdx		= iTempEndIdx + iCount;
					//End
					break;
				}
			}
		}       
	}

	return iIsDirty;
}

int OI_Dirty_Eng_Check_Word(/*DIRTY_CHECK * pstSelf, */unsigned char * sCheckStr, unsigned char * sDirtyStr, int * piRealCount)
{
	int				iCount=0, iCheckCount = 0;
	unsigned char		cHiChar, cLoChar, cCompChar; 

	while ((cCompChar = sDirtyStr[iCount]) != '\0')
	{
		cLoChar		= sCheckStr[iCheckCount ++];
		if (cLoChar == '\0')
		{
			return 1;
		}
		
		if (IS_DOUBLE_CHAR(cLoChar))
		{
			cHiChar	= sCheckStr[iCheckCount ++];
			if (!IS_ENGLISH_DOUBLE(cLoChar, cHiChar))
			{
				continue;
			}
			else 
			{
				CONVERT_DOUBLE_TO_SINGLE(cLoChar, cLoChar, cHiChar);
			}    
		}
		else if (IS_ENGLISH_CHAR(cLoChar))
		{
			CONVERT_CAPITAL_CHAR(cLoChar, cLoChar);
		}
		else
		{
			continue;
		}

		if (!EQUAL_ENGLISH_CHAR(cLoChar, cCompChar))
		{
			return 2;
		}

		iCount++;
	}

	//Sean add, for Replace
	
	*piRealCount	=	iCheckCount;
	//End
	return 0;
}

int OI_Dirty_Reg_Check(DIRTY_CHECK * pstSelf, int iLevel, unsigned char * sCheckStr, int * piDirtyLevel, int * piBeginIdx, int * piEndIdx, int iCond)
{
#define MAX_SUB_NUM 1
	int	iIsDirty = 0;
	int	fResult;
	size_t		len;
	regmatch_t	subs[MAX_SUB_NUM];
	char			errbuf[128];
	int			err, i;
	OI_DIRTY_REG_RECORD * pstRegRecord = NULL;
	
	*piBeginIdx	= 0;
	*piEndIdx		= 0;

	if (sCheckStr == NULL)
	{//input error
		return -1;	
	}

	if(!pstSelf->m_pstDirtyCore->iEnable) 
	{//Dirty DisEnable
		return -2;
	}

#ifndef _WIN32


	pstRegRecord = pstSelf->m_pstDirtyCore->astRegDirtyRec;
	for (i = 0; i < pstSelf->m_pstDirtyCore->aiRegWordCount; i++)
	{
		err = regexec(&pstRegRecord[i].stRegular, (const char *)sCheckStr, (size_t)MAX_SUB_NUM, subs, 0);
		if(err == REG_NOMATCH)   
		{
			continue;
		}   
		else if(err)
		{   
			len = regerror(err, &pstRegRecord[i].stRegular, errbuf, sizeof(errbuf));
			continue;  
		}

		if (iCond < 0)
		{
			fResult	= (pstRegRecord[i].iLevel < iLevel) ? 1 : 0;
		}
		else if (iCond == 0)
		{
			fResult	= (pstRegRecord[i].iLevel >= iLevel) ? 1 : 0;
		}
		else
		{
			fResult	= (pstRegRecord[i].iLevel >= iLevel) ? 1 : 0;
		}
		
		//if (pstRegRecord[i].iLevel >= iLevel)
		if (fResult)
		{
			iIsDirty	= 1;
			*piDirtyLevel = pstRegRecord[i].iLevel;

			*piBeginIdx	= subs[0].rm_so;
			*piEndIdx		= subs[0].rm_eo;
		}

		break;
	}
#else

	(void)iLevel;
	*piDirtyLevel	= 0;

#endif

	return iIsDirty;
}

int OI_Dirty_Mix_Check(DIRTY_CHECK * pstSelf, int iLevel, unsigned char * sCheckStr, int * piDirtyLevel, int iReplace, int iCond)
{
	unsigned char				cLoChar, cHiChar;
	unsigned char *			sReservBuff;
	
	size_t					iStrLen = 0;

	int						iIsDirty = 0, iCount = 0, iBufCount = 0, iIndex, iOffset;
	int						iKeyOff;
	int						fResult;
	
	OI_DIRTY_CHN_RECORD*	pstDirtyRecord;
	unsigned char				abyRsvBuf[4096];

	if ((sCheckStr == NULL) || ( strlen((const char *)sCheckStr) == 0 ))
	{//input error
		return -1;
	}

	if (!pstSelf->m_pstDirtyCore->iEnable) 
	{//Dirty DisEnable
		return -2;
	}

	if (strlen((const char *)sCheckStr) + 10 >= sizeof(abyRsvBuf))
	{//too long
		return -3;
	}

	sReservBuff = &abyRsvBuf[0];
	while( (cLoChar = sCheckStr[iCount++]) != '\0' )
	{
		if (IS_DOUBLE_CHAR(cLoChar))
		{ 
			cHiChar=sCheckStr[iCount++];
			if(cHiChar == '\0') 
			{
				break;
			}

			if( IS_DIGIT_DOUBLE(cLoChar, cHiChar) ) 
			{
				CONVERT_DOUBLE_DIGIT_TO_SINGLE(sReservBuff[iBufCount++], cLoChar, cHiChar);
			}
			else if (IS_ENGLISH_DOUBLE(cLoChar, cHiChar) ) 
			{
				CONVERT_DOUBLE_TO_SINGLE(sReservBuff[iBufCount++], cLoChar, cHiChar);
			}
			//else if (IS_CHINESE_CHAR(cLoChar, cHiChar))
			else
			{
				//if( !IS_CHINESE_PUNC(cLoChar,cHiChar) ) 
				{ 
					sReservBuff[iBufCount++]=cLoChar;
					sReservBuff[iBufCount++]=cHiChar;
				}
			}
		}
	   /*
  	   else if ( IS_ENGLISH_CHAR(cLoChar) )
	   {
		   CONVERT_CAPITAL_CHAR(sReservBuff[iBufCount++],cLoChar);
	   }
	   else if( IS_DIGIT(cLoChar) )
	   {
		   sReservBuff[iBufCount++] = cLoChar;
	   }
	   */
	   else
	   {
		   sReservBuff[iBufCount++] = cLoChar;
	   }
	}

	sReservBuff[iBufCount] = 0;
	
	iCount		= 0;
	iIsDirty	= 0;

	pstDirtyRecord = pstSelf->m_pstDirtyCore->astChnDirtyRec;
	while( (!iIsDirty) && ( (cLoChar = sReservBuff[iCount++]) != '\0') )
	{
		if( IS_ENGLISH_CHAR(cLoChar) || IS_DIGIT(cLoChar) )
		{
			iIndex = 0;
			while (pstSelf->m_pstDirtyEngRec[iIndex++].sDirtyStr[0] != '\0')
			{
				if (iCond < 0)
				{
					fResult	= (pstSelf->m_pstDirtyEngRec[iIndex-1].iLevel < iLevel) ? 1 : 0;
				}
				else if (iCond == 0)
				{
					fResult	= (pstSelf->m_pstDirtyEngRec[iIndex-1].iLevel == iLevel) ? 1 : 0;
				}
				else
				{
					fResult	= (pstSelf->m_pstDirtyEngRec[iIndex-1].iLevel >= iLevel) ? 1 : 0;
				}
				
				//if ((pstSelf->m_pstDirtyEngRec[iIndex-1].iLevel >= iLevel) && EQUAL_ENGLISH_CHAR(cLoChar, pstSelf->m_pstDirtyEngRec[iIndex-1].sDirtyStr[0]))
				if (fResult && EQUAL_ENGLISH_CHAR(cLoChar, pstSelf->m_pstDirtyEngRec[iIndex-1].sDirtyStr[0]))
				{
					iStrLen = strlen((const char *)(pstSelf->m_pstDirtyEngRec[iIndex - 1].sDirtyStr));
					if ( strncasecmp((const char *)(sReservBuff + iCount - 1), (const char *)(pstSelf->m_pstDirtyEngRec[iIndex - 1].sDirtyStr), iStrLen) == 0 )
					{
						iIsDirty = 4;
						*piDirtyLevel = pstSelf->m_pstDirtyEngRec[iIndex-1].iLevel;

						if (iReplace)
						{
							memset(sReservBuff + iCount - 1, '*', iStrLen);
						}

						break;
					}
				}
			}//while
		}//if
	   else if (IS_DOUBLE_CHAR(cLoChar))	
	   {
			cHiChar = sReservBuff[iCount++]; 
			if(cHiChar == '\0')
			{
				break;
			}

			if (IS_CHINESE_CHAR(cLoChar,cHiChar))
			{
				CAL_INDEX_OFFSET(iIndex, cLoChar, cHiChar);
				iOffset = pstSelf->m_pstChnIndexTab->iDirtyIndexTable[iIndex]; //该字是否关键字//

				while ((iOffset != -1) && (iOffset >= 0) && (iOffset < C_MAX_WORD_NUM))
				{
					if (!pstDirtyRecord[iOffset].bMix)
					{
						//是中文但不是Mix
						iOffset = pstDirtyRecord[iOffset].iNextKey;  // 下一个含有该关键字得短语//
						continue;
					}

					iKeyOff = pstDirtyRecord[iOffset].iKeyOffset;
					if ((pstDirtyRecord[iOffset].sKeyWord[0] != cLoChar) || (pstDirtyRecord[iOffset].sKeyWord[1] != cHiChar))
					{
						//关键字不匹配可能发生在loadmem的时候
						break;
					}
					
					if (iCond < 0)
					{
						fResult	= (pstDirtyRecord[iOffset].iLevel < iLevel) ? 1 : 0;
					}
					else if (iCond == 0)
					{
						fResult	= (pstDirtyRecord[iOffset].iLevel == iLevel) ? 1 : 0;
					}
					else
					{
						fResult	= (pstDirtyRecord[iOffset].iLevel >= iLevel) ? 1 : 0;
					}
					
					//if (pstDirtyRecord[iOffset].iLevel >= iLevel)
					if (fResult)
					{
						if ((iCount - 2) >= pstDirtyRecord[iOffset].iKeyOffset)
						{
							iStrLen	= strlen((const char *)(pstDirtyRecord[iOffset].sDirtyStr));

							if (strncasecmp((const char *)(sReservBuff + (iCount - 2 - pstDirtyRecord[iOffset].iKeyOffset)), 
								(const char *)(pstDirtyRecord[iOffset].sDirtyStr), iStrLen) == 0)
							{
								iIsDirty = 5;
								*piDirtyLevel = pstDirtyRecord[iOffset].iLevel;

								if (iReplace)
								{
									memset(sReservBuff + (iCount - 2 - pstDirtyRecord[iOffset].iKeyOffset), '*', iStrLen);
								}

								break;
							}
						}
					}

					iOffset = pstDirtyRecord[iOffset].iNextKey;  // 下一个含有该关键字得短语//
				}
			}// if chinese 
		}//if double 
	}//end while

	if (iReplace && iIsDirty)
	{
		memcpy(sCheckStr, sReservBuff, iBufCount+1);
	}

	sReservBuff	= NULL;
	return iIsDirty;
}

void TrimStr(char * pszString)
{
	char * pb;
    char * pe;
    size_t iTempLength;

    if(pszString == NULL)
    {
        return;
    }

    iTempLength = strlen(pszString);
    if(iTempLength == 0)
    {
        return;
    }

    pb = pszString;
    while ( ( (*pb == ' ') || (*pb == '\t') || (*pb == '\n') || (*pb == '\r') ) && (*pb != 0))
    {
        pb++;
    }

    pe = &pszString[iTempLength - 1];
    while ( (pe >= pb) && ( (*pe == ' ') || (*pe == '\t') || (*pe == '\n') || (*pe == '\r') ) )
    {
        pe--;
    }
    *(pe + 1) = '\0';

    strcpy( pszString, pb );

    return;
}

