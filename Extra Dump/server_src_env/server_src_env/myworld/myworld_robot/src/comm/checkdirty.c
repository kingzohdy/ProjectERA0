#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>
#include "gbchar_process.h"
#include "tlib_dirty_check.h"

#define STX 0x2

typedef struct
{
	regex_t	stRegEx;
	int     used;
}REGINFO;


char sErrMsg[1024];
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
	/* һ */
	if (c1==0xd2 && c2==0xbb)
		return 1;
	return 0;
}

//�ףף�.jh18.�ãϣ�
static int splitec(char *psOrgStr, char **ppsChnStr,char **ppsEngStr)
{
	int 	iLength , i;
	int 	t = 0, u = 0 , v = 0;
	static char cflt[256];
	static char eflt[256];
/*	static char eflt_url[256]; */
	int		iContentChar;

	iLength = strlen(psOrgStr);
	if (iLength > 255)
		iLength = 255;
	
	iContentChar = 0;
	
	for (i = 0 ; i < iLength; i++)
	{
		if (isascii(*(psOrgStr + i)))
		{
			/* �����Ӣ���ַ����¼��eflt��*/
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
				/* ���ɼ��ַ�*/
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
				/* �����ȫ����ĸ��ת��Ϊ��� */
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
				/* �����ַ�����������Ҫ����ʶ */
				iContentChar |= CHN_SIGN;
				i++;
				continue;
			}
		}
		/* ����ʶ���ַ� */
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

void initDirtySystem()
{
/*	if (!initRegModule(&(stUrlRegInfo),"(http|www)+.+(com|net|org|biz|cn|tv|us|cc|cx|info)"))
	{
		printf("Can't Build RegModule1\n");
		exit(0);
	}
	if (!initRegModule(&(stUrl2RegInfo),"\\.+(com|net|org|biz|cn|tv|us|cc|cx|info)"))
	{
		printf("Can't Build RegModule2\n");
		exit(0);
	}
*/
	if (TLib_Dirty_Init("./", sErrMsg) != 0)
	{
		printf("%s\n",sErrMsg);
		exit(0);
	}
}

int main(int argc , char *argv[])
{
	char *psEngStr ,*psChnStr;
	char line[255];
	int  iRetCode;

		initDirtySystem();
	while(fgets(line,255,stdin) != NULL)
	{
		iRetCode = splitec(line,&psChnStr,&psEngStr);
/*
		if (iRetCode&PRO_CHAR)          {printf("��ֹ�û��Զ���Э��\n");continue;}
		if (iRetCode&UNKNOWN)			{printf("��GB�ַ�\n");continue;}
		if (iRetCode&SPE_SIGN)			{printf("����ı�����\n");continue;}
		if (iRetCode&URL_STR)			{printf("����URL�ַ���\n");continue;}
*/
		iRetCode = 0;
		if (psChnStr[0] != 0) // ƥ�����Ĺ����ֿ��� 1,2,4�����ȼ�
			iRetCode = TLib_Dirty_Check(1|2|4,psChnStr,sErrMsg);
		if (iRetCode == 0)
		{
			// ƥ��Ӣ�Ĺ����ֿ�
			if (TLib_Dirty_Check(LEVEL_ENG,psEngStr,sErrMsg)<0)
			   	iRetCode = 4;
		}
		if (iRetCode < 0 )
			iRetCode = -iRetCode;
		if (iRetCode & 4)
		{
			printf("ƥ��4���ؼ���");
		}
		else if (iRetCode & 1)
		{
			printf("ƥ��1���ؼ���");
		}
		else if (iRetCode & 2)
		{
			printf("ƥ��2���ؼ���");
		}
		else
		{
		    if (TLib_Dirty_Check(8|LEVEL_SW,psChnStr,sErrMsg ) < 0 )
				printf("ƥ�䵥�ֹؼ���");
		}
	}
		
	return 0;
}
