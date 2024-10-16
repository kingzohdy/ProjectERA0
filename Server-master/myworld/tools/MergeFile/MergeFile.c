#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include "comm/shtable.h"

#define MAX_MERGE_LINE 30000

#define MAX_MERGE2_LINE 200000

struct tagMergeLine
{
	char szFileName[128];
	int   iLine;
	char szTrans[1024];
};
typedef struct tagMergeLine MergeLine;

struct tagMergeLine2
{
	char szFileName[128];
	int   iLine;
	int   iNum;
	char szTrans[2048];
};
typedef struct tagMergeLine2 MergeLine2;

struct tagMergeFile
{
	char szFileName[128];
	int iNum;
	MergeLine astLine;
};
typedef struct tagMergeFile MergeFile;

#define  IS_CHINESE_CHAR(lo,hi)		( (lo>0x80 && lo<0xff) && ((hi>0x3f && hi<0x7f)||(hi>0x7f && hi<0xff)))
#define  IS_CHINESE_PUNC(lo,hi)		( (lo>0xa0 && lo<0xb0) && (hi>0x3f && hi<0xff))

int is_merge_line(char *pszBuf, char *pszEscPre)
{
	int iFlag = 0;
	char *p;
	char *p1 = pszBuf;
	int iCount = 0;
	unsigned char cLoChar, cHiChar;

	if(pszEscPre)
	{
		p = strstr(pszBuf, pszEscPre);
		if (p)
		{
			while(p1 != p)
			{
				if (*p1 != ' ' && *p1 != '\t')
				{
					iFlag = 1;
					break;
				}
				p1++;
			}

			if (0 == iFlag)
			{
				return 0;
			}
		}
	}

	while ((cLoChar = pszBuf[iCount++]) != '\0')
	{
		cHiChar	= pszBuf[iCount];
					
		//if (IS_CHINESE_CHAR(cLoChar, cHiChar) )
		if ( (cLoChar>0x80 && cLoChar<0xff) && ((cHiChar>0x3f && cHiChar<0x7f)||(cHiChar>0x7f && cHiChar<0xff)))
		{
			return 1;
		}

		if ( IS_CHINESE_PUNC(cLoChar, cHiChar) ) // 中文标点符号
		{
			return 1;
		}

		if (cHiChar == '\0')
		{
			break;
		}
	}

	return 0;
}

int merge_file_one(FILE *file, char *pszFilePath, char *pszFileName, char *pszEscPre)
{
	FILE *file1;
	char szBuf[102400];
	char szBuf1[102400];
	int iLine = 0;

	if ((file1 = fopen(pszFilePath, "r")) == NULL)
	{
		printf("fopen %s fail\n", pszFileName);
		return -1;
	}

	while (fgets(szBuf, sizeof(szBuf), file1))
	{
		iLine++;
		if (is_merge_line(szBuf, pszEscPre))
		{
			if (szBuf[strlen(szBuf)-2] == '\r')
			{
				szBuf[strlen(szBuf)-2] = 0;
			}
			else
			{
				szBuf[strlen(szBuf)-1] = 0;
			}
			snprintf(szBuf1, sizeof(szBuf1), "%s,%d,\"%s\"\r\n", pszFileName, iLine, szBuf);
			fputs(szBuf1, file);
		}
	}

	fclose(file1);
	
	return 0;
}

int merge_file(char *pszDir, char *pszMergeFile, char *pszEscPre)
{
	FILE *file;
	DIR *dp = NULL;
	struct dirent *dirp;
	struct stat sbuf;
	char szFileName[256];

	if ((dp=opendir(pszDir)) == NULL)
	{
		printf("opendir error %d!\n", errno);
		return -1;
	}

	if ((file = fopen(pszMergeFile, "w")) == NULL)
	{
		printf("fopen error!\n");
		return -1;
	}

	fputs("File,Line,Txt\r\n" , file);
	
	while((dirp=readdir(dp)) != NULL)
	{
		if (0 == strcmp(dirp->d_name, ".") || 0 == strcmp(dirp->d_name, ".."))
		{
			continue;
		}

		snprintf(szFileName, sizeof(szFileName), "%s/%s",pszDir,dirp->d_name);

		if (stat(szFileName, &sbuf )< 0)
		{
			printf("can't open %s \n",szFileName);
			continue;
		}

		if (S_ISDIR(sbuf.st_mode))
		{
			continue;
		}

		merge_file_one(file, szFileName, dirp->d_name, pszEscPre);
	}

	fclose(file);
		
	return 0;
}

void mf_append(char *Out, int *pLen, char ch)
{
	Out[*pLen] = ch;
	*pLen += 1;
}

int mf_readstring(char *pOut, int *pLen, char **p)
{
	int iSucc = 0;
	
	if(**p != '\"')
	{
		return 0;
	}

	for(;;)
	{
		*p += 1;
		
		if(**p == '\0')
		{
			goto _out;
		}
		
		switch(**p)
		{
			case '\"':
				if(*(*p+1) == '\r' ||
					*(*p+1) == '\n' )
				{
					iSucc = 1;
					goto _out;
				}
				mf_append(pOut, pLen, '"');
				goto _out;
				break;

			case '\r':
			case '\n':
				iSucc = 1;
				goto _out;
				break;
			default:
				mf_append(pOut, pLen, **p);
				break;	
		}
	}

_out:

	return iSucc;
}

int get_merge_string(char **p, char *pOut, int *pLen)
{
	//如果整行被""括起来要去掉
	if(**p == '"')
	{
		*p += 1;
	}

	for(;;)
	{
		if(**p == '\0')
		{
			mf_append(pOut, pLen , '\0');
			goto _out;
		}
		
		switch(**p)
		{
			case '\r':
			case '\n':
				mf_append(pOut, pLen , '\0');
				goto _out;
				break;

			case '\"':
				if(mf_readstring2(pOut, pLen, p))
				{
					mf_append(pOut, pLen , '\0');
					goto _out;
				}
				break;
			default:
				mf_append(pOut, pLen, **p);
				break;	
		}

		*p += 1;
	}

_out:
	*p += 1;
	return 0;
}

int get_merge_file_name(char **p, char *pOut, int *pLen)
{	
	for(;;)
	{
		if(**p == '\0')
		{
			mf_append(pOut, pLen , '\0');
			goto _out;
		}
		
		switch(**p)
		{
			case '\r':
			case '\n':
				mf_append(pOut, pLen , '\0');
				goto _out;
				break;

			case ',':
				mf_append(pOut, pLen , '\0');
				goto _out;
				break;
			default:
				mf_append(pOut, pLen , **p);
				break;	
		}

		*p += 1;
	}

_out:
	
	*p += 1;
	return 0;
}

int get_merge_file_num(char **p, int *pOut)
{
	int iLen = 0;
	char szBuff[32];
	
	for(;;)
	{
		if(**p == '\0')
		{
			goto _out;
		}
		
		switch(**p)
		{
			case '\r':
			case '\n':
				mf_append(szBuff, &iLen , '\0');
				goto _out;
				break;

			case ',':
				mf_append(szBuff, &iLen , '\0');
				goto _out;
				break;
			default:
				mf_append(szBuff, &iLen , **p);
				break;	
		}

		*p += 1;
	}

_out:
	if(iLen > 0)
	{
		*pOut = atoi(szBuff);
	}
	*p += 1;
	return 0;
}


int get_mergeline(char *pszBuf, MergeLine *pstMergeLine)
{
	char *p;
	char *p1;

	p1 = pszBuf;
	
	memset(pstMergeLine, 0 , sizeof(pstMergeLine));

	p = strchr(p1, ',');
	if (p == NULL)
	{
		return -1;
	}

	if ((p - p1) >= sizeof(pstMergeLine->szFileName)) 
	{
		return -1;
	}

	*p = 0;
	strncpy(pstMergeLine->szFileName, p1, sizeof(pstMergeLine->szFileName));
	pstMergeLine->szFileName[sizeof(pstMergeLine->szFileName) - 1] = 0;

	p1 = p+1;

	p = strchr(p1, ',');
	if (p == NULL)
	{
		return -1;
	}

	*p = 0;
	pstMergeLine->iLine = atoi(p1);

	if (*(p+1) == '"')
	{
		p1 = p+2;
		p = strrchr(p1, '"');
		if (p == NULL)
		{
			return -1;
		}
		*p = 0;
	}
	else
	{
		int i;
		
		p1 = p+1;
		p = pszBuf + strlen(pszBuf);
		for (i=0; i<3; i++)
		{
			p = p - i;
			if (*p == '\r' || *p == '\n')
			{
				*p = 0;
			}
		}
	}

	strncpy(pstMergeLine->szTrans, p1, sizeof(pstMergeLine->szTrans));
	pstMergeLine->szTrans[sizeof(pstMergeLine->szTrans) - 1] = 0;
	
	return 0;
}

/*
int get_mergeline(char *pszBuf, MergeLine *pstMergeLine)
{
	int iQuatoFlag = 0;
	int iGet = 0;
	char *p1 = pszBuf;
	char szBuf[1024];
	int iPoint;
	
	memset(szBuf, 0, sizeof(szBuf));
	iPoint = 0;
	while(*p1 != '\0')
	{
		if ((*p1 == ',' || (*p1 == '\r' && *(p1+1) == '\n')) && iQuatoFlag == 0)
		{
			szBuf[iPoint] = 0;
			switch(iGet)
			{
				case 0:
					strncpy(pstMergeLine->szFileName, szBuf, sizeof(pstMergeLine->szFileName));
					pstMergeLine->szFileName[sizeof(pstMergeLine->szFileName) - 1] = 0
					break;
				case 1:
					pstMergeLine->iLine = atoi(szBuf);
					break;
				case 2:
					strncpy(pstMergeLine->szTrans, szBuf, sizeof(pstMergeLine->szTrans));
					pstMergeLine->szTrans[sizeof(pstMergeLine->szTrans) - 1] = 0;
					break;
				default:
					break;
			}
			
			iGet++;
			if (iGet >= 3)
			{
				break;
			}
			memset(szBuf, 0, sizeof(szBuf));
			iPoint = 0;
		}
		else if (*p1 == '\"')
		{
			if (iQuatoFlag == 0)
			{
				iQuatoFlag = 1;
			}
			else
			{
				iQuatoFlag = 0;
			}
		}
		else
		{
			szBuf[iPoint++] = *p1;
			if (iPoint > sizeof(szBuf) - 1)
			{
				printf("buf %s err\n", pszBuf);
				return -1;
			}
		}

		p1++;
	}

	if (iGet != 3)
	{
		printf("buf %s err\n", pszBuf);
		return -1;
	}
	
	return 0;
}
**/

static int mergeline_cmp( const void * pv1 , const void *pv2 )
{
	MergeLine *pMergeLine1 = (MergeLine *)pv1;
	MergeLine *pMergeLine2 = (MergeLine *)pv2;

	if (0 == strcmp(pMergeLine1->szFileName, pMergeLine2->szFileName) && 
		pMergeLine1->iLine == pMergeLine2->iLine)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

static unsigned int mergeline_hash(const void* pvData)
{	
	MergeLine *pMergeLine1 = (MergeLine *)pvData;
	
	return sht_get_code(pMergeLine1->szFileName);
}

static int mergeline2_cmp( const void * pv1 , const void *pv2 )
{
	MergeLine2 *pMergeLine1 = (MergeLine2 *)pv1;
	MergeLine2 *pMergeLine2 = (MergeLine2 *)pv2;

	if (0 == strcmp(pMergeLine1->szFileName, pMergeLine2->szFileName) && 
		pMergeLine1->iLine == pMergeLine2->iLine &&
		pMergeLine1->iNum == pMergeLine2->iNum)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

static unsigned int mergeline2_hash(const void* pvData)
{	
	MergeLine2 *pMergeLine1 = (MergeLine2 *)pvData;
	
	return sht_get_code(pMergeLine1->szFileName);
}

int unmerge_file_one(char *pszPath, char *pszFileName, LPSHTABLE pstMergeFileSht, char *pszDir1)
{
	FILE *file;
	FILE *file1;
	char szBuf[102400];
	char szNewPath[256];
	int iLine = 0;
	MergeLine stMergeLine, *pstMergeLine;

	if ((file = fopen(pszPath, "r")) == NULL)
	{
		printf("fopen error!\n");
		return -1;
	}

	snprintf(szNewPath, sizeof(szNewPath), "%s/%s", pszDir1, pszFileName);
	if ((file1 = fopen(szNewPath, "w")) == NULL)
	{
		printf("fopen error!\n");
		return -1;
	}

	while (fgets(szBuf, sizeof(szBuf), file))
	{
		iLine++;

		stMergeLine.iLine= iLine;
		strncpy(stMergeLine.szFileName, pszFileName, sizeof(stMergeLine.szFileName));
		
		pstMergeLine = sht_find(pstMergeFileSht, &stMergeLine,mergeline_cmp, mergeline_hash);
		if(!pstMergeLine)
		{
			fputs(szBuf, file1);
		}
		else
		{
			snprintf(szBuf, sizeof(szBuf), "%s\r\n", pstMergeLine->szTrans);
			fputs(szBuf, file1);
		}
	}

	fclose(file);
	fclose(file1);
	
	return 0;
}

int unmerge_file(char *pszDir, char *pszMergeFile, char *pszDir1)
{
	FILE *file;
	char szBuf[102400];
	char szFilePath[256];
	MergeLine stMergeLine;
	MergeLine *pstMergeLine;
	LPSHTABLE pstMergeFileSht;
	DIR *dp = NULL;
	struct dirent *dirp;
	struct stat sbuf;

	if ((file = fopen(pszMergeFile, "r")) == NULL)
	{
		printf("fopen error!\n");
		return -1;
	}

	pstMergeFileSht = sht_create(MAX_MERGE_LINE, MAX_MERGE_LINE, sizeof(MergeLine), NULL);
	if (NULL == pstMergeFileSht)
	{
		printf("sht_create error \n");
		return -1;
	}

	fgets(szBuf, sizeof(szBuf), file);
	
	while (fgets(szBuf, sizeof(szBuf), file))
	{
		if (0 > get_mergeline(szBuf, &stMergeLine))
		{
			printf("get_mergeline fail\n");
			continue;
		}

		pstMergeLine = sht_insert_unique(pstMergeFileSht, &stMergeLine, mergeline_cmp, mergeline_hash);
		if (NULL == pstMergeLine)
		{
			printf("file:%s,line:%d sht_insert_unique fail\n", stMergeLine.szFileName, stMergeLine.iLine);
		}
		else
		{
			memcpy(pstMergeLine, &stMergeLine, sizeof(*pstMergeLine));
		}
	}

	if ((dp=opendir(pszDir)) == NULL)
	{
		printf("opendir error %d!\n", errno);
		return -1;
	}

	while((dirp=readdir(dp)) != NULL)
	{
		if (0 == strcmp(dirp->d_name, ".") || 0 == strcmp(dirp->d_name, ".."))
		{
			continue;
		}

		snprintf(szFilePath, sizeof(szFilePath), "%s/%s",pszDir,dirp->d_name);

		if (stat(szFilePath, &sbuf )< 0)
		{
			printf("can't open %s \n",szFilePath);
			continue;
		}

		if (S_ISDIR(sbuf.st_mode))
		{
			continue;
		}

		unmerge_file_one(szFilePath, dirp->d_name, pstMergeFileSht, pszDir1);
	}

	sht_destroy(&pstMergeFileSht);
	fclose(file);
	
	
	return 0;
}

int mf_readstring2(char *pOut, int *pLen, char **p)
{
	int iSucc = 0;
	
	mf_append(pOut, pLen, '\"');
	for(;;)
	{
		*p += 1;
		
		if(**p == '\0')
		{
			return 0;
		}
		
		switch(**p)
		{
			case '\"':
				mf_append(pOut, pLen, '\"');
				*p += 1;
				if(**p == '\"')
				{
					mf_append(pOut, pLen, '\"');
					break;
				}
				goto _out;
				break;
			default:
				mf_append(pOut, pLen, **p);
				break;	
		}
	}

_out:
	
	return 0;
}

int get_merge2_string(char **p, char *pOut, int *pLen)
{
	int fEnd = 0;
	
	for(;;)
	{
		if(**p == '\0')
		{
			mf_append(pOut, pLen , '\0');
			fEnd = 1;
			goto _out;
		}
		
		switch(**p)
		{
			case '\r':
			case '\n':
				mf_append(pOut, pLen , '\0');
				goto _out;
				break;
			case ',':
				mf_append(pOut, pLen , '\0');
				goto _out;
				break;
			case '\"':
				mf_readstring2(pOut, pLen, p);
				mf_append(pOut, pLen , '\0');
				goto _out;
				break;
			default:
				mf_append(pOut, pLen, **p);
				break;	
		}

		*p += 1;
	}

_out:
	if(**p != '\0')
	{
		*p += 1;
		return 0;
	}
	else
	{
		return 1;
	}
}

void mf2_append(char *Out, char ch)
{
	char szBuf[2];
	szBuf[0] = ch;
	szBuf[1] = '\0';
	strcat(Out, szBuf);
}

char Read(FILE *file)
{
	return fgetc(file);
}

char Peek(FILE *file)
{
	char ch = fgetc(file);
	fseek(file, -1, SEEK_CUR);
	return ch;
}

int ReadRow(FILE *file, char *pStr, int *piRow)
{
	if(*piRow)
		strcat(pStr, ",");

	for(;;)
	{
		int ch = Peek(file);
		if(ch == EOF)
			return 0;

		switch(ch)
		{
		case ',': case '\n':
			goto out;

		case '\r':
			if(Read(file) != '\r') return 0;
			if(Peek(file) != '\n') return 0;
			goto out;

		default:
			mf2_append(pStr, Read(file));
			break;
		}
	}
out:
	*piRow += 1;
	return 1;
}

int ReadNullIfExist(FILE *file, char *pStr, int *piRow)
{
	if(Read(file) != ',')
		return 0;

	for(;;)
	{
		int ch = Peek(file);
		if(ch == EOF)
		{
			return 0;
		}
		
		switch(ch)
		{
		case '\"':
			goto out;

		case '\r':
			if(Read(file) != '\r') return 0;
			if(Peek(file) != '\n') return 0;
		case '\n':
		case ',':
			if(*piRow)
				strcat(pStr, ",");
			*piRow += 1;
			goto out;

		default:
			if(isspace(ch))
				goto out;
			else
				goto out;
			break;
		}
	}
	
out:
	return 1;
}

int ReadString(FILE *file, char *pStr, int *piRow)
{
	if(*piRow)
		mf2_append(pStr, ',');
	if(Read(file) != '\"')
		return 0;
	mf2_append(pStr, '\"');

	for(;;)
	{
		int ch = Peek(file);
		if(ch == EOF)
			return 0;

		switch(ch)
		{
		case '\"':
			if(Read(file) != '\"')
				return 0;
			if(Peek(file) != '\"')
				goto out;
			if(Read(file) != '\"')
				return 0;
			mf2_append(pStr, '\"');
			mf2_append(pStr, '\"');
			break;

		default:
			mf2_append(pStr, Read(file));
			break;
		}
	}
out:
	mf2_append(pStr, '\"');
	*piRow += 1;
	return 1;
}

int merge2_file_one_line(FILE *file, FILE *file1, char *pszFileName, int *piLine)
{
	int iLen = 0, iNum = 0;
	int iRow = 0;
	char ch;
	char *p;
	char szBuf[102400];
	char szRow[2048];
	char szBuf1[102400];
	int iEnd;

	szBuf[0] = '\0';
	
	for(;;)
	{
		ch= Peek(file1);
		if(ch == EOF)
		{
			return 0;
		}

		switch(ch)
		{
			case ' ':
				if(!ReadRow(file1, szBuf, &iRow))
					return 0;
				break;
			case '\"':
				if(!ReadString(file1, szBuf, &iRow))
					return 0;
				break;
			case ',':
				if(!ReadNullIfExist(file1, szBuf, &iRow))
					return 0;
				break;

			case '\r':
				if(Read(file1) != '\r') return 0;
				if(Read(file1) != '\n') return 0;
				goto out;

			case '\n':
				if(Read(file1) != '\n') return 0;
				goto out;

			default:
				if(isspace(ch))
					Read(file1);
				else if(!ReadRow(file1, szBuf, &iRow))
					return 0;
				break;
		}
	}
	
out:

	iNum = 1;
	p = szBuf;
	iEnd = 0;
	while(1)
	{
		iLen = 0;
		szRow[0] = 0;
		if(get_merge2_string(&p, szRow, &iLen))
		{
			iEnd = 1;
		}
		
		if(!szRow[0])
		{
			if (iEnd)
			{
				break;
			}
			
			iNum++;
			continue;
		}
			
		if (is_merge_line(szRow, NULL))
		{
			//if (szRow[strlen(szRow)-2] == '\r')
			//{
			//	szRow[strlen(szRow)-2] = 0;
			//}
			
			snprintf(szBuf1, sizeof(szBuf1), "%s,%d,%d,%s\r\n", pszFileName, *piLine, iNum, szRow);
			fputs(szBuf1, file);
		}

		if (iEnd)
		{
			break;
		}

		iNum++;
	}
	return 1;
}

int merge2_file_one(FILE *file, char *pszFilePath, char *pszFileName)
{
	FILE *file1;
	int iLine = 0;

	if ((file1 = fopen(pszFilePath, "r")) == NULL)
	{
		printf("fopen %s fail\n", pszFileName);
		return -1;
	}

	while(1)
	{
		iLine++;
		if(!merge2_file_one_line(file, file1, pszFileName, &iLine))
		{
			break;
		}
	}
	
	fclose(file1);
	
	return 0;
}

int merge2_file(char *pszDir, char *pszMergeFile)
{
	FILE *file;
	DIR *dp = NULL;
	struct dirent *dirp;
	struct stat sbuf;
	char szFileName[256];

	if ((dp=opendir(pszDir)) == NULL)
	{
		printf("opendir error %d!\n", errno);
		return -1;
	}

	if ((file = fopen(pszMergeFile, "w")) == NULL)
	{
		printf("fopen error!\n");
		return -1;
	}

	fputs("File,Line,Row,Txt\r\n" , file);
	
	while((dirp=readdir(dp)) != NULL)
	{
		if (0 == strcmp(dirp->d_name, ".") || 0 == strcmp(dirp->d_name, ".."))
		{
			continue;
		}

		snprintf(szFileName, sizeof(szFileName), "%s/%s",pszDir,dirp->d_name);

		if (stat(szFileName, &sbuf )< 0)
		{
			printf("can't open %s \n",szFileName);
			continue;
		}

		if (S_ISDIR(sbuf.st_mode))
		{
			continue;
		}

		merge2_file_one(file, szFileName, dirp->d_name);
	}

	fclose(file);
		
	return 0;
}

int get_mergeline2(char *pszBuf, MergeLine2 *pstMergeLine)
{
	int iLen;
	char *p = pszBuf;
	
	memset(pstMergeLine, 0 , sizeof(pstMergeLine));

	iLen = 0;
	get_merge_file_name(&p, pstMergeLine->szFileName, &iLen);

	iLen = 0;
	get_merge_file_num(&p, &pstMergeLine->iLine);

	iLen = 0;
	get_merge_file_num(&p, &pstMergeLine->iNum);

	iLen = 0;
	get_merge2_string(&p, pstMergeLine->szTrans, &iLen);
	
	return 0;
}

int unmerge2_file_one_line(FILE *file, FILE *file1, LPSHTABLE pstMergeFileSht, char *pszFileName, int *piLine)
{
	int iLen = 0, iNum = 0, fEnd;
	int iRow = 0;
	char ch;
	char *p;
	char szBuf[102400], szPutBuf[102400];
	char szRow[2048];
	char szBuf1[102400];
	MergeLine2 stMergeLine, *pstMergeLine;

	szBuf[0] = '\0';
	
	for(;;)
	{
		ch= Peek(file);
		if(ch == EOF)
		{
			return 0;
		}

		switch(ch)
		{
			case ' ':
				if(!ReadRow(file, szBuf, &iRow))
					return 0;
				break;
			case '\"':
				if(!ReadString(file, szBuf, &iRow))
					return 0;
				break;
			case ',':
				if(!ReadNullIfExist(file, szBuf, &iRow))
					return 0;
				break;

			case '\r':
				if(Read(file) != '\r') return 0;
				if(Read(file) != '\n') return 0;
				goto out;

			case '\n':
				if(Read(file) != '\n') return 0;
				goto out;

			default:
				if(isspace(ch))
					Read(file);
				else if(!ReadRow(file, szBuf, &iRow))
					return 0;
				break;
		}
	}
	
out:

	fEnd = 0;
	szPutBuf[0] = '\0'; 
	iNum = 0;
	p = szBuf;
	while(1)
	{
		iNum++;
		iLen = 0;
		szRow[0] = 0;

		fEnd = get_merge2_string(&p, szRow, &iLen);
		if(!szRow[0])
		{
			strcat(szPutBuf, ",");
			if(fEnd)
			{
				break;
			}
			continue;
		}

		stMergeLine.iLine= *piLine;
		stMergeLine.iNum = iNum;
		strncpy(stMergeLine.szFileName, pszFileName, sizeof(stMergeLine.szFileName));

		pstMergeLine = sht_find(pstMergeFileSht, &stMergeLine,mergeline2_cmp, mergeline2_hash);
		if(!pstMergeLine)
		{
			if(iNum != 1)
				strcat(szPutBuf, ",");
			strcat(szPutBuf, szRow);
			
		}
		else
		{
			if(iNum != 1)
				strcat(szPutBuf, ",");
			
			strcat(szPutBuf, pstMergeLine->szTrans);
		}

		if(fEnd)
		{
			break;
		}
	}

	snprintf(szBuf, sizeof(szBuf), "%s\r\n", szPutBuf);

	fputs(szBuf, file1);
	return 1;
}


int unmerge2_file_one(char *pszPath, char *pszFileName, LPSHTABLE pstMergeFileSht, char *pszDir1)
{
	FILE *file;
	FILE *file1;
	char szNewPath[256];
	int iLine = 0;

	if ((file = fopen(pszPath, "r")) == NULL)
	{
		printf("fopen error!\n");
		return -1;
	}

	snprintf(szNewPath, sizeof(szNewPath), "%s/%s", pszDir1, pszFileName);
	if ((file1 = fopen(szNewPath, "w")) == NULL)
	{
		printf("fopen error!\n");
		return -1;
	}

	while(1)
	{
		iLine++;
		if(!unmerge2_file_one_line(file, file1, pstMergeFileSht, pszFileName, &iLine))
		{
			break;
		}
	}	

	fclose(file);
	fclose(file1);
	
	return 0;
}

int unmerge2_file_sht_line(FILE *file, LPSHTABLE pstMergeFileSht)
{
	int iRow = 0;
	char ch;
	char szBuf[102400];
	MergeLine2 stMergeLine;
	MergeLine2 *pstMergeLine;

	szBuf[0] = '\0';
	for(;;)
	{
		ch= Peek(file);
		if(ch == EOF)
		{
			return 0;
		}

		switch(ch)
		{
			case ' ':
				if(!ReadRow(file, szBuf, &iRow))
					return 0;
				break;
			case '\"':
				if(!ReadString(file, szBuf, &iRow))
					return 0;
				break;
			case ',':
				if(!ReadNullIfExist(file, szBuf, &iRow))
					return 0;
				break;

			case '\r':
				if(Read(file) != '\r') return 0;
				if(Read(file) != '\n') return 0;
				goto out;

			case '\n':
				if(Read(file) != '\n') return 0;
				goto out;

			default:
				if(isspace(ch))
					Read(file);
				else if(!ReadRow(file, szBuf, &iRow))
					return 0;
				break;
		}
	}
	
out:

	get_mergeline2(szBuf, &stMergeLine);
	pstMergeLine = sht_insert_unique(pstMergeFileSht, &stMergeLine, mergeline2_cmp, mergeline2_hash);
	if (NULL == pstMergeLine)
	{
		printf("file:%s,line:%d Num:%d sht_insert_unique fail\n", stMergeLine.szFileName, stMergeLine.iLine, stMergeLine.iNum);
	}
	else
	{
		memcpy(pstMergeLine, &stMergeLine, sizeof(*pstMergeLine));
	}
	return 1;
}

int unmerge2_file(char *pszDir, char *pszMergeFile, char *pszDir1)
{
	FILE *file;
	char szBuf[102400];
	char szFilePath[256];
	LPSHTABLE pstMergeFileSht;
	DIR *dp = NULL;
	struct dirent *dirp;
	struct stat sbuf;

	if ((file = fopen(pszMergeFile, "r")) == NULL)
	{
		printf("fopen error!\n");
		return -1;
	}

	pstMergeFileSht = sht_create(MAX_MERGE2_LINE, MAX_MERGE2_LINE, sizeof(MergeLine2), NULL);
	if (NULL == pstMergeFileSht)
	{
		printf("sht_create error \n");
		return -1;
	}

	fgets(szBuf, sizeof(szBuf), file);
	
	while (1)
	{
		if(!unmerge2_file_sht_line(file, pstMergeFileSht))
		{
			break;
		}
	}

	if ((dp=opendir(pszDir)) == NULL)
	{
		printf("opendir error %d!\n", errno);
		return -1;
	}

	while((dirp=readdir(dp)) != NULL)
	{
		if (0 == strcmp(dirp->d_name, ".") || 0 == strcmp(dirp->d_name, ".."))
		{
			continue;
		}

		snprintf(szFilePath, sizeof(szFilePath), "%s/%s",pszDir,dirp->d_name);

		if (stat(szFilePath, &sbuf )< 0)
		{
			printf("can't open %s \n",szFilePath);
			continue;
		}

		if (S_ISDIR(sbuf.st_mode))
		{
			continue;
		}

		unmerge2_file_one(szFilePath, dirp->d_name, pstMergeFileSht, pszDir1);
	}

	sht_destroy(&pstMergeFileSht);
	fclose(file);
	
	
	return 0;
}

int main(int argc, char* argv[])
{
	int iMode;
		
	if (argc != 5)
	{
		printf("example:MergeFile 0|1|2|3 dir file esc_pre|dir1\n");
		return 0;
	}

	iMode = atoi(argv[1]);

	if(0 == iMode)
	{
		merge_file(argv[2], argv[3], argv[4]);
	}
	else if(1 == iMode)
	{
		unmerge_file(argv[2], argv[3], argv[4]);
	}
	else if(2 == iMode)
	{
		merge2_file(argv[2], argv[3]);
	}
	else if (3 == iMode)
	{
		unmerge2_file(argv[2], argv[3], argv[4]);
	}
	
	return 0;
}

