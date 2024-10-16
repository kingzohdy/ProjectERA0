#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>
#include "gbchar_process.h"
#include "tlib_dirty_check.h"

#define STX 0x2
#define MAX_DIRTY_STR		4096
#define inline 

typedef struct
{
	regex_t	stRegEx;
	int     used;
}REGINFO;


char sErrMsg[1024];
/*
REGINFO stUrlRegInfo;
REGINFO stUrl2RegInfo;

int initRegModule(REGINFO *pstRegInfo,const char *pszRegexMask)
{
	int err;
    err = regcomp(&(pstRegInfo->stRegEx),pszRegexMask,REG_EXTENDED);
    if (err) 
    	pstRegInfo->used = 0;
    else
    	pstRegInfo->used = 1;
    
    return pstRegInfo->used;
}

int isMatchRegMask(REGINFO *pstRegInfo,const char *pszMatchString)
{
	int 		err;
	regmatch_t 	subs[10];
	char 		errbuf[128];
	
	if (pstRegInfo->used==0)
		return 0;

    err = regexec(&(pstRegInfo->stRegEx),pszMatchString, 10, subs, 0);
    if (err == REG_NOMATCH)
        return 0;
    else if (err)
    {
        regerror(err,&(pstRegInfo->stRegEx),errbuf, sizeof(errbuf));
        printf("reg error:%s[%s]",pszMatchString,errbuf);
        return 0;
    }
	return 1;
}
*/

enum
{
	ENG_CHAR = 0x1,
	CHN_CHAR = 0x2,
	CHN_SIGN = 0x4,
	UNPRI	 = 0x8,
	UNKNOWN	 = 0x10,
	URL_STR  = 0x20,
	SPE_SIGN = 0x40,
	PRO_CHAR = 0x80,
};

inline int isSpecialChnChar(unsigned char c1,unsigned char c2)
{
	/* 一 */
	if (c1==0xd2 && c2==0xbb)
		return 1;
	return 0;
}

//ＷＷＷ.jh18.ＣＯＭ
static int splitec(const char *psOrgStr, char **ppsChnStr,char **ppsEngStr)
{
	int 	iLength , i;
	int 	t = 0, u = 0 , v = 0;
	static char cflt[MAX_DIRTY_STR];
	static char eflt[MAX_DIRTY_STR];
/*	static char eflt_url[MAX_DIRTY_STR]; */
	int		iContentChar;

	iLength = strlen(psOrgStr);
	if (iLength > MAX_DIRTY_STR - 1)
		iLength = MAX_DIRTY_STR - 1;
	
	iContentChar = 0;
	
	for (i = 0 ; i < iLength; i++)
	{
		if (isascii(*(psOrgStr + i)))
		{
			/* 如果是英文字符则记录在eflt里*/
			if (isalpha(*(psOrgStr + i)))
			{
				eflt[u++] 		= (char)tolower((int)*(psOrgStr + i));
/*				eflt_url[v++]	= eflt[u-1]; */
			}
/*			else if (isUrlChar(*(psOrgStr + i)))
				eflt_url[v++] 	= (char)tolower((int)*(psOrgStr + i)); */
			else if (!isprint(*(psOrgStr + i)))
			{
				if (*(psOrgStr + i) == STX)
	                iContentChar |= PRO_CHAR;
	            else
					iContentChar |= UNPRI;
			}
			continue;
		}
		if (ISDBCSLEAD(HZ_TGB,*(psOrgStr + i)))
		{
			if (ISDBCSNEXT(HZ_TGB,*(psOrgStr + i + 1)))
			{
				/* 不可见字符*/
				if (isUnPrintableChnChar(*(psOrgStr+i) ,*(psOrgStr+i+1)))
				{
					iContentChar |= UNPRI;
					i++;
					continue;
				}
				if (isSpecialChnChar(*(psOrgStr+i) ,*(psOrgStr+i+1)))
				{
					i++;
					continue;
				}
				cflt[t++] = *(psOrgStr + i);
				cflt[t++] = *(psOrgStr + i + 1);
				i++;
				continue;
			}
		}
		if (ISDBCSLEAD(HZ_TSIGN,*(psOrgStr + i)))
		{
			if (ISDBCSNEXT(HZ_TSIGN,*(psOrgStr + i + 1)) )
			{
				/* 如果是全角字母则转换为半角 */
				if (isGbAlpha(*(psOrgStr+i) ,*(psOrgStr+i+1)))
				{
					eflt[u++] 		= (char)tolower((int)convGbAlphaToEng(*(psOrgStr+i) ,*(psOrgStr+i+1)));
/*					eflt_url[v++]	= eflt[u-1]; */
					i++;
					continue;
				}
/*				if (isGbDigit(*(psOrgStr+i) ,*(psOrgStr+i+1)))
				{
					eflt_url[v++] = convGbAlphaToEng(*(psOrgStr+i) ,*(psOrgStr+i+1));
					i++;
					continue;
				}
				if (isGbUrlChar(*(psOrgStr+i) ,*(psOrgStr+i+1)))
				{
					eflt_url[v++] = convGbUrlCharToEng(*(psOrgStr+i) ,*(psOrgStr+i+1));
					i++;
					continue;
				}
*/
				/* 其余字符则跳过但是要做标识 */
				iContentChar |= CHN_SIGN;
				i++;
				continue;
			}
		}

		if (ISDBCSLEAD(HZ_GBK,*(psOrgStr + i)))
                {
                        if (ISDBCSNEXT(HZ_GBK,*(psOrgStr + i + 1)))
                        {
                                cflt[t++] = *(psOrgStr + i);
                                cflt[t++] = *(psOrgStr + i + 1);
                                i++;
                                continue;
                        }
                }

		/* 不可识别字符 */
		iContentChar |=UNKNOWN;
	}
	cflt[t] = 0;
	eflt[u] = 0;
/*	eflt_url[v] = 0; */
	
/*	if (v!=0)
	{
		if (isMatchRegMask(&(stUrlRegInfo),eflt_url))
			iContentChar |= URL_STR;
		else if (isMatchRegMask(&(stUrl2RegInfo),eflt_url))
			iContentChar |= URL_STR;
		else if (strstr(eflt_url,"http://") || strstr(eflt_url,"www."))
			iContentChar |= URL_STR;
	}
*/

	*ppsChnStr = cflt;
	*ppsEngStr = eflt;
	return iContentChar;
}

void InitDirtySystem(char *pszPath)
{
	if (TLib_Dirty_Init(pszPath, sErrMsg) != 0)
	{
		printf("%s\n",sErrMsg);
		exit(0);
	}
}

int CheckDirty(const char* pszText, int iLevel)
{
	char *psEngStr ,*psChnStr;
	int  iRetCode;

	iRetCode = splitec(pszText, &psChnStr, &psEngStr);
	iRetCode = 0;
	if (psChnStr[0] != 0) // 匹配中文过滤字库
		iRetCode = TLib_Dirty_Check(iLevel,psChnStr,sErrMsg);
	if (iRetCode == 0)
	{
			// 匹配英文过滤字库
		if (TLib_Dirty_Check(LEVEL_ENG,psEngStr,sErrMsg)<0)
		   	iRetCode = 4;
	}
	if (iRetCode < 0 )
		iRetCode = -iRetCode;
		
	return iRetCode;
}

int CheckCharacter ( char *pszStr )
{
    unsigned char *p ;

    for ( p = pszStr ; *p ; p ++ )
    {
        if ( ( *p >= '0' &&  *p <='9'  ) || (*p >= 'a' && *p <= 'z' ) || ( *p >= 'A' && *p <= 'Z' ) || ( *p == '_' ) )
           continue ;

       if ( *p >= 0x80 )
       {
           if ( !( *p >= 0xa0 && *p <= 0xa9 ) )
           {
               p ++ ;

			   if( *p && (*p<0x30 || *p=='\\') )
					return -1;

               continue ;
           }
       }

       return -1 ;
    }

    return 0 ;
}

int CheckCharacter2(char * pszStr) 
{
	unsigned char *p ;

	for(p = pszStr; *p; p++)
	{
		if( (*p >= '0' &&  *p <= '9') || (*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p == '_') )
		{
			continue ; 	
		}

		if(*p <0x20 && *p != 0x0a && *p != 0x0d) //allow '\n'
		{
			return(-1);
		}

		if(*p == 0x22 || *p == 0x27 || *p == 0x3d ||*p == '\\' ) // not ' and " , = 
		{
			return(-1);
		}
		
		if(*p > 0x80)
		{
			if(*(p+1) < 0x30)	//非法汉字或半个汉字
			{
				return(-1);
			}
			else	//合法汉字
			{
				p++;
			}
		}

	}

	return(0);
	
}



