/*****************************************************************************
// 项目名称	: Dirty Check Library
// 文件名	: tlib_dirty_check.c
// 创建时间	：
// 内容描述	：过滤敏感信息
// 文件版本  ：$Id: tlib_dirty_check.c,v 1.1 2009/10/09 08:16:04 kent Exp $
******************************************************************************/

#ifndef _TLIB_DIRTY_CHECK_C_
#define _TLIB_DIRTY_CHECK_C_

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>

#include "dir_mm.h"
#include "tlib_dirty_check.h"

static const key_t iDirtyShmKey=19331;
static const char *sDirtyGbFile ="dirty_gb.txt";
static const char *sDirtyEngFile="dirty_eng.txt";
static const char *sDirtySwFile ="dirty_sw.txt";

static char gsPath[255];

TLIB_DIRTY_CHN_STRUCT 	*pstChnIndexTab = NULL;
TLIB_DIRTY_CORE		    *pstDirtyCore = NULL;
TLIB_DIRTY_ENG_RECORD 	*pstDirtyList = NULL;


static char *trim(char *str)
{
      char *ibuf, *obuf;

      if (str)
      {
            for (ibuf = obuf = str; *ibuf; )
            {
                  while (*ibuf && (isspace (*ibuf)))
                        ibuf++;
                  if (*ibuf && (obuf != str))
                        *(obuf++) = ' ';
                  while (*ibuf && (!isspace (*ibuf)))
                        *(obuf++) = *(ibuf++);
            }
            *obuf = 0;
      }
      return (str);
}

static time_t GetFileModifyTime(char *pszFilePath,char *sErrMsg)
{
	struct stat 		stat_p;		
	 
	if ( -1 ==  stat (pszFilePath, &stat_p))
	{
		sprintf(sErrMsg,"stat %s error:%s",pszFilePath, strerror(errno));
		return -1;
	}
	return stat_p.st_mtime;
}
/************************************************************
// 函数名  : TLib_Get_ChnItem_From_File()
// 编写者  : 
// 参考资料:
// 功能    : 从数据文件中读取敏感词和其等级
// 输入参数: pFile -- 数据文件句柄 , pItem -- 敏感词
// 输出参数: 1 -- eof of the file, 0 -- continue , -1 -- error
// 备 注   : 数据文件格式为 <level>|<keyword>|<dirty string>
*************************************************************/
static int TLib_Get_ChnItem_From_File(FILE * pFile, Dirty_Chn_Item *pItem,char *sErrMsg) //int *piLevel, unsigned char * sDirtyWord, unsigned char * sKeyWord, char * sErrMsg)
{
	char		szLines[512], *ptr;
	int			i , iLength , iFlag;
	
	pItem->sDirtyStr[0]='\0';
	pItem->sKeyWord[0]='\0';
	pItem->iLevel = 0;
	
	do{
		if ( fgets(szLines,512,pFile) == NULL)
			return 1;
		trim(szLines);
	}while( szLines[0]=='#');
		
	if ((iLength=strlen(szLines))>50)
	{
		sprintf(sErrMsg,"Dirty Word Too Long");
		return -1;
	}
	if ((iLength=strlen(szLines))<8)
	{
		sprintf(sErrMsg,"Dirty Word Too Short");
		return -1;
	}
	
	ptr = szLines;
	iFlag = 0;
	for (i = 0 ; i < iLength ; i++)
	{
		if (szLines[i]!='|' && szLines[i]!=' ')
			continue;
		szLines[i] = 0;
		switch (iFlag)
		{
			case 0:
				pItem->iLevel = atoi(ptr);
				break;
			case 1:
				snprintf(pItem->sKeyWord,3,"%s",ptr);
				break;
			case 2:
				snprintf(pItem->sDirtyStr,C_MAX_WORD_LEN,"%s",ptr);
				break;
			default:
				sprintf(sErrMsg,"Parse Dirty Word Error");
				return -1;			
		}
		iFlag++;
		ptr = &(szLines[i+1]);
	}
	if (ptr!=&(szLines[i]) && iFlag==2)
	{
		snprintf(pItem->sDirtyStr,C_MAX_WORD_LEN,"%s",ptr);
		iFlag++;
	}
	if (iFlag!=3)
	{
		sprintf(sErrMsg,"Parse Dirty Word Error2:%d %s %s",iFlag,pItem->sKeyWord,pItem->sDirtyStr);
		return -1;
	}
	return 0;
}

/************************************************************
// 函数名： TLib_Init_Chn_Table()
// 编写者： 
// 参考资料：
// 功 能：  
// 输入参数: pItem -- 敏感词资料, iCount -- 该词应该放在第几格数组中
// 输出参数：ok: > 0  fail: -1
// 备 注：房间配置文件名为: roomX.ini ,如果不是room开头,.ini结尾,则
//       认为不是有效文件
*************************************************************/
int TLib_Init_Chn_Table (Dirty_Chn_Item *pItem, int iCount,char *sErrMsg)
{
	TLIB_DIRTY_CHN_RECORD    *pstDirtyRecord;
	unsigned char            *sSubstr;
	int                      iIndex,iOffset;
	
	assert(pstChnIndexTab!=NULL);
	if (iCount >= C_MAX_WORD_NUM)
	{
		sprintf(sErrMsg,"The number of the words is too large");	
		return -1;
	}

	pstDirtyRecord = pstDirtyCore->astChnDirtyRec ;
	
	strcpy(pstDirtyRecord[iCount].sDirtyStr,pItem->sDirtyStr); 
	strcpy(pstDirtyRecord[iCount].sKeyWord ,pItem->sKeyWord);
	pstDirtyRecord[iCount].iLevel = pItem->iLevel;
	sSubstr=strstr(pItem->sDirtyStr,pItem->sKeyWord);
	if (sSubstr==NULL){
        sprintf(sErrMsg,"Keyword(%s) in this file error",pItem->sKeyWord);
        return -1;	
	}
	pstDirtyRecord[iCount].iKeyOffset=(sSubstr-pItem->sDirtyStr); 
	pstDirtyRecord[iCount].iNextKey=-1;
	
#ifdef DEBUG 
    printf("Init_Chn_Table:KeyOffset:%d\n",pstDirtyRecord[iCount].iKeyOffset); 
    printf("Init_Chn_Table:iOffset  :%d\n",iCount);
#endif    
	 
	CAL_INDEX_OFFSET(iIndex,pItem->sKeyWord[0],pItem->sKeyWord[1])
	iOffset=pstChnIndexTab->iDirtyIndexTable[iIndex];
	
	if (iOffset==-1)
	{
		pstChnIndexTab->iDirtyIndexTable[iIndex]=iCount;
	}
	else 
	{ 
        while (iOffset!=-1)
        {
        	if (strcmp( pstDirtyRecord[iOffset].sDirtyStr,pItem->sDirtyStr) == 0)
        	{
        		memset( &(pstDirtyRecord[iCount]),0,sizeof(pstDirtyRecord[iCount]));
        		sprintf(sErrMsg,"Conflict word(%s)",pItem->sDirtyStr);
        		return -1;
        	}
        	iIndex  = iOffset;
        	iOffset = pstDirtyRecord[iOffset].iNextKey;
        }
        pstDirtyRecord[iIndex].iNextKey=iCount;
	}
	return 0; 
}

int TLib_Chn_Dirty_Word_Add(Dirty_Chn_Item *pItem , char *pszDirtyFilePath,char *sErrMsg)
{
	int		iCount ;
	struct stat stat_p;		
	//char 	sFile[50];
	FILE	*pFile;
	
 	if (pstDirtyCore->iUpdatingFlag)
 		return 0;

	if ( pstChnIndexTab == NULL)
	{
		printf("Not Init ?\n");
		return -3;
	}
	if ((pItem->sDirtyStr[0]=='\0') || (pItem->sKeyWord[0]=='\0') ) return -1;
	if ( pItem->iLevel > 6 && pItem->iLevel < 1 )		return -4;
	// iCount 实际是数据偏移量
	for ( iCount = 0 ; iCount < C_MAX_WORD_NUM ; iCount++)
	{
		if (pstDirtyCore->astChnDirtyRec[iCount].sKeyWord[0] == 0)
			break;
	}
 	pstDirtyCore->iUpdatingFlag = 1;
 	sleep(1);
	if (TLib_Init_Chn_Table(pItem, iCount, sErrMsg)<0)
	{
		sprintf(sErrMsg,"Fail to init dirty word table!");
		pstDirtyCore->iUpdatingFlag = 0;
		return -1;
	}
	pstDirtyCore->aiChnWordCount = pstDirtyCore->aiChnWordCount +1 ;

	//snprintf(sFile,50, "%s%s", gsPath, sDirtyGbFile);
	pFile =fopen(pszDirtyFilePath,"a+");

	if (pFile==NULL) 
	{
		sprintf(sErrMsg,"Fail to open file:%s",pszDirtyFilePath);
		pstDirtyCore->iUpdatingFlag = 0;
		return -2; 
	} 
	fprintf(pFile,"%d|%s|%s\n",pItem->iLevel,pItem->sKeyWord,pItem->sDirtyStr);
	fclose(pFile);

	if ( -1 ==  stat (pszDirtyFilePath, &stat_p))
	{
		printf(" Error occoured attempting to stat %s\n", pszDirtyFilePath);
		pstDirtyCore->iUpdatingFlag = 0;
		return -3;
	}
	pstDirtyCore->lChnUpdateTime =  stat_p.st_mtime;
	pstDirtyCore->iUpdatingFlag = 0;
	return 0;
}

int TLib_Chn_Dirty_Word_Remove (unsigned char * sDirtyWord, unsigned char *sKeyWord, char *sErrMsg)
{
	int	iCount = pstDirtyCore->aiChnWordCount ;
	int                      			 iIndex,iOffset , iOldOffset , iTmp;

	if ((sDirtyWord[0]=='\0') || (sKeyWord[0]=='\0') ) return -1;

	if ( pstChnIndexTab == NULL)
	{
		printf("Not Init ?\n");
		return -3;
	}
	CAL_INDEX_OFFSET(iIndex,sKeyWord[0],sKeyWord[1])
	//printf("%d %d\n",pstChnIndexTab->iDirtyIndexTable[iIndex],iIndex);
	iOffset=pstChnIndexTab->iDirtyIndexTable[iIndex];

	if ( iOffset == -1 )
		return -2;
	
       	iOldOffset=iOffset;
        while (iOffset!=-1)
        {
        	if ( strcmp( sDirtyWord , pstDirtyCore->astChnDirtyRec[iOffset].sDirtyStr) == 0)
        	{
	        	// 处理前一个元素的指针
	        	if ( iOffset == pstChnIndexTab->iDirtyIndexTable[iIndex] )
	        	{
	        		pstChnIndexTab->iDirtyIndexTable[iIndex] = pstDirtyCore->astChnDirtyRec[iOffset].iNextKey;
	        		// iOldOffset=pstDirtyRecord[iOffset].iNextKey;
	        	}
	        	else
	        	{
	        		pstDirtyCore->astChnDirtyRec[iOldOffset].iNextKey = pstDirtyCore->astChnDirtyRec[iOffset].iNextKey;
	        		// iOldOffset=iOffset;
	        	}
        		iTmp=pstDirtyCore->astChnDirtyRec[iOffset].iNextKey;
        		memset( &(pstDirtyCore->astChnDirtyRec[iOffset]) ,0,sizeof(pstDirtyCore->astChnDirtyRec[iOffset]));
        		iOffset = iTmp;
        		iCount --; 
        		continue;
        	}
        	iOldOffset=iOffset;
        	iOffset=pstDirtyCore->astChnDirtyRec[iOffset].iNextKey;
        }
	pstDirtyCore->aiChnWordCount = iCount;

	return 0;
}

/***********************************************************************************************
// 函数名： TLib_Chn_Dirty_Init()
// 编写者： 
// 参考资料：
// 功 能：  读取数据文件,并加入到Chn数据表中
// 输入参数: sChnFileName:中文脏话文件
// 输出参数：ok: 0  fail: -1
// 备 注：
*************************************************************************************************/	
static int TLib_Chn_Dirty_Init(char  *sChnFileName,  char  *sErrMsg)
{
	int					iCount=0, iRet;
	FILE				*pFile;
	time_t		 		lLastModifyTime;
	Dirty_Chn_Item		stItem;		
	 
	lLastModifyTime = GetFileModifyTime(sChnFileName,sErrMsg);
	if (lLastModifyTime == (time_t)-1)
	{
		printf("%s\n", sErrMsg);
		exit(0);
	}
	pstChnIndexTab = &(pstDirtyCore->stChnDirtyStruc);

	if (lLastModifyTime == pstDirtyCore->lChnUpdateTime )
	{
#ifdef DEBUG
		printf("File %s Hadn't Modify\n",sChnFileName);
#endif
		return 0;
	}

	//init shm 
	memset( pstDirtyCore->astChnDirtyRec,0,sizeof(pstDirtyCore->astChnDirtyRec));
	for (iCount=0;iCount<C_MAX_TABLE_LEN;iCount++)
	{
		pstChnIndexTab->iDirtyIndexTable[iCount]=-1;
	}

	//open file
	iCount=0;
	pFile =fopen(sChnFileName,"r");
	if (pFile==NULL) 
	{
		sprintf(sErrMsg,"Fail to open file %s:%s",sChnFileName,strerror(errno));
		return -1; 
	} 

	/* -------- 从文件里读脏话词语 -------------------------*/
	while ((iRet=TLib_Get_ChnItem_From_File(pFile,&stItem,sErrMsg))==0)
	{
#ifdef DEBUG
	printf("Dirty string:%s\n",sDirtyWord);
	printf("KeyWord string:%s\n",sKeyWord);
#endif
		
//		if ((sDirtyWord[0]=='\0') || (sKeyWord[0]=='\0') ) break;
		if ((stItem.iLevel >= LEVEL_SW) || (stItem.iLevel < 1) ) continue;
		if (TLib_Init_Chn_Table(&stItem,iCount,sErrMsg)<0)
		{
			fclose(pFile);
			return -1;
		}
		iCount++;
	}
	pstDirtyCore->aiChnWordCount = iCount;
	fclose(pFile);	
	if (iRet<0) 
		return -1;
	pstDirtyCore->lChnUpdateTime =  lLastModifyTime;
	return 0;
}

/***********************************************************************************************
// 函数名： TLib_Chn_Dirty_Check_Word()
// 编写者： 
// 参考资料：
// 功 能：  读取数据文件,并加入到Chn数据表中
// 输入参数: sDirtyStr:关键词,sCheckStr:待检测的句子,sReservBuff:临时存放区
// 输出参数：ok: 0  fail: -1
// 备 注：
*************************************************************************************************/	
int TLib_Chn_Dirty_Check_Word(int iKeyOff,unsigned char *sCheckStr,unsigned char *sReservBuff,unsigned char *sDirtyStr)
{
	int            iCount=0,iCheckCount=0;
	unsigned char  cHiChar,cLoChar; 
	
#ifdef DEBUG
	 printf("Check_Word:Checkstr:%s\n",sCheckStr);
	 printf("Check_Word:DirtyStr:%s\n",sDirtyStr);
	 printf("KeyOffset is:%d\n",iKeyOff);
	 sReservBuff[iKeyOff]='\0';
	 printf("ReservBuff is :%s\n",sReservBuff); 
#endif
	// 先比较临时存放区
	while ((iCount)<iKeyOff)
	{
		if (sReservBuff[iCount%C_MAX_WORD_LEN]!=sDirtyStr[iCount])
		    return 1;
		iCount++;    
	}
#ifdef DEBUG
	printf("Dirty String :%s\n",sDirtyStr+iCount); 
#endif 
	// 比较剩余句子
	while (sDirtyStr[iCount]!='\0')
	{
		if (sCheckStr[iCheckCount]=='\0')
		   return 1; 
		cLoChar=sCheckStr[iCheckCount++];   
	        cHiChar=sCheckStr[iCheckCount]; 
		if (!IS_CHINESE_CHAR(cLoChar,cHiChar)) /* 是否全角非汉字? */
			continue;
	        if ( IS_CHINESE_PUNC(cLoChar,cHiChar)) // 中文标点符号
	        {
			iCheckCount++;
	        	continue;
	        }
		iCheckCount++;
#ifdef DEBUG
       printf("the word is :%c%c\n",cLoChar,cHiChar);
       printf ("check word is:%c%c\n",sDirtyStr[iCount],sDirtyStr[iCount+1]);
#endif
	
		if (cLoChar!=sDirtyStr[iCount++])
	           return 1;
		if (cHiChar!=sDirtyStr[iCount++])
		   return 1; 		
	}
#ifdef DEBUG 
   printf ("Complete !\n");
#endif  
	return 0;
}
		    
/***********************************************************************************************
// 函数名： TLib_Chn_Dirty_Check()
// 编写者： 
// 参考资料：
// 功 能：  检查是否有中文脏字眼
// 输入参数: sDirtyStr:关键词,iLevel:欲检查的敏感字等级
// 输出参数：ok: 0  fail: -1
// 备 注：
*************************************************************************************************/	
int TLib_Chn_Dirty_Check(int iLevel,unsigned char *sCheckStr,char *sErrMsg)
{	
	unsigned char			sReservBuff[C_MAX_WORD_LEN],cLoChar,cHiChar;
	int						iIsDirty=0,iCount=0,iBufCount=0,iIndex,iOffset;
	int						iKeyOff;
	TLIB_DIRTY_CHN_RECORD	*pstDirtyRecord;

	assert(pstChnIndexTab!=NULL);
	
	if (sCheckStr == NULL)
	{
		sprintf(sErrMsg,"The checking string is null");
		return 0;
	}
	
	pstDirtyRecord =pstDirtyCore->astChnDirtyRec;
	
	/*   开始检查 字符串是否含有 ‘脏字眼’ */
	while ((!iIsDirty)&&(cLoChar=sCheckStr[iCount++])!='\0')
	{
		cHiChar=sCheckStr[iCount];
		if (!IS_CHINESE_CHAR(cLoChar,cHiChar))
			continue;
		if ( IS_CHINESE_PUNC(cLoChar,cHiChar)) // 中文标点符号
		{
			iCount++;
			continue;
		}
		iCount++;
		
		sReservBuff[(iBufCount++)%C_MAX_WORD_LEN]=cLoChar;
		sReservBuff[(iBufCount++)%C_MAX_WORD_LEN]=cHiChar;

		CAL_INDEX_OFFSET(iIndex,cLoChar,cHiChar)

		iOffset=pstChnIndexTab->iDirtyIndexTable[iIndex]; /*该字是否关键字*/

		while (iOffset!=-1)
		{
			iKeyOff=pstDirtyRecord[iOffset].iKeyOffset;
			if (pstDirtyRecord[iOffset].iLevel & iLevel )
			{
				// iBufCount : sReservBuf的最后一个字节偏移
				// iKeyOff     : 关键字在DirtyStr中的偏移
				// sCheckStr+iCount: 将剩下的句子传入检查
				// sReservBuff++(iBufCount-iKeyOff-2)%C_MAX_WORD_LEN : 将包含关键字的临时区间传入
				if (!TLib_Chn_Dirty_Check_Word(iKeyOff+2,sCheckStr+iCount,
										sReservBuff+(iBufCount-iKeyOff-2)%C_MAX_WORD_LEN,
										pstDirtyRecord[iOffset].sDirtyStr))
				{
					iIsDirty=pstDirtyRecord[iOffset].iLevel;
					break;
				}
			}
			iOffset=pstDirtyRecord[iOffset].iNextKey;  /* 下一个含有该关键字得短语*/	
		}       

	}


	return iIsDirty;
}

/**********************************************************************************************
            以下为英文脏话检查部分...
************************************************************************************************/
/***********************************************************************************************
// 函数名： TLib_Eng_Dirty_Init()
// 编写者： 
// 参考资料：
// 功 能：  初始化英文脏话共享内存区,读取数据文件,并加入到Eng数据表中
// 输入参数: sEngFileName:英文脏话文件
// 输出参数：ok: 0  fail: -1
// 备 注：
*************************************************************************************************/	
static int TLib_Eng_Dirty_Init (char  *sEngFileName,char  *sErrMsg)
{
	int				iCount=0,iLen,i;
	unsigned char	sDirtyWord[C_MAX_WORD_LEN+1]; 
	FILE			*pFile;
	time_t		 	lLastModifyTime;

	lLastModifyTime = GetFileModifyTime(sEngFileName,sErrMsg);
	if (lLastModifyTime == (time_t)-1)
	{
		printf("%s\n", sErrMsg);
		exit(0);
	}
	pstDirtyList= pstDirtyCore->astEngDirtyRec ;

	if (lLastModifyTime == pstDirtyCore->lEngUpdateTime )
	{
#ifdef DEBUG
		printf("File %s Hadn't Modify\n",sEngFileName);
#endif
		return 0;
	}

	/*先初始化 dirtystring list */
	memset(pstDirtyList,0,sizeof(pstDirtyList));
	for (iCount=0;iCount<E_MAX_WORD_NUM;iCount++)
	    pstDirtyList[iCount].sDirtyStr[0]='\0';
	
	pFile =fopen(sEngFileName,"r");
	if (pFile==NULL) {
		sprintf(sErrMsg,"Fail to open file:%s",sEngFileName);
		return -1; 
	} 
	iCount=0;        
	/* -------- 从文件里读脏话词语 -------------------------*/
	//while ((iRet=TLib_Get_ChnItem_From_File(pFile,&iLevel,sDirtyWord,sKeyWord,sErrMsg))==0)
	while(fgets(sDirtyWord,C_MAX_WORD_LEN,pFile) != NULL)
	{
		trim(sDirtyWord);
		if ( sDirtyWord[0] == '#' )
			continue;
#ifdef DEBUG 
    	printf("Dirty string:%s\n",sDirtyWord);
#endif
		iLen = strlen(sDirtyWord);
		for ( i = 0 ; i < iLen ; i++)
			if ( sDirtyWord[i] == '\n' || sDirtyWord[i] == '\r' || sDirtyWord[i] == ' ')
				sDirtyWord[i] = 0;
		
		strcpy(pstDirtyList[iCount++].sDirtyStr,sDirtyWord);
		if (iCount >= E_MAX_WORD_NUM)
		{
	        printf("The number of the eng words is too large");	
	        break;
		}
	}
	
	pstDirtyCore->aiEngWordCount = iCount;
	pstDirtyCore->lEngUpdateTime = lLastModifyTime;
	fclose(pFile);	

	return 0;
}

int TLib_Eng_Dirty_Check_Word(unsigned char  *sCheckStr,unsigned char  *sDirtyStr)
{
	int            iCount=0,iCheckCount=0;
	unsigned char  cHiChar,cLoChar,cCompChar; 
	
	#ifdef DEBUG 
	       printf("check string:%s\n",sCheckStr);
	       printf("dirty string:%s\n",sDirtyStr);
	#endif 
	while ((cCompChar=sDirtyStr[iCount])!='\0')
	{
		cLoChar=sCheckStr[iCheckCount++];
		if (cLoChar=='\0')
		   return 1;
		if (IS_DOUBLE_CHAR(cLoChar))
		{ 
			cHiChar=sCheckStr[iCheckCount++];
			if (!IS_ENGLISH_DOUBLE(cLoChar,cHiChar))
				continue;
			else {
				CONVERT_DOUBLE_TO_SINGLE(cLoChar,cLoChar,cHiChar)
			}    
		} 
		else if (IS_ENGLISH_CHAR(cLoChar)){
			CONVERT_CAPITAL_CHAR(cLoChar,cLoChar)
		}
		else continue;
		if (!EQUAL_ENGLISH_CHAR(cLoChar,cCompChar)){
	                #ifdef DEBUG 
	                   printf("first char:%c,second char :%c\n",cLoChar,cCompChar);
	                #endif 		
		        return 1; 
		}
		iCount++;   
	}
	return 0;
}
/**********************************************************************************************************************
Funciton    检查是否有英文脏字眼..
input       sCheckStr: 	//待检查的字符串 
output      sErrMsg	//错误信息	    
return:     0: 		//此字符串不带有‘脏字眼’ 		    
	       1: 		//此字符串带有‘脏字眼’ 		    
	       <0:         //Error
lastmodify  2001.08.06
written by  caocy
modify by  hunter
***********************************************************************************************************************/	
int TLib_Eng_Dirty_Check(unsigned char *sCheckStr,  char *sErrMsg)
{	
	unsigned char            cLoChar,cHiChar;
	int                      iIsDirty=0,iCount=0,iIndex=0;
	
	if (pstDirtyList == NULL)
	{
		sprintf(sErrMsg,"The ShareMemory hasn't beend Initiate");
		return -1;	
	}
	
	if (sCheckStr==NULL)
	{
		sprintf(sErrMsg,"The checking string is null");
		return -2;	
	}
		
	/*   开始检查 字符串是否含有 ‘脏字眼’ */
	while ((!iIsDirty)&&(cLoChar=sCheckStr[iCount++])!='\0')
	{
		if (IS_DOUBLE_CHAR(cLoChar)){ 
			cHiChar=sCheckStr[iCount++];
			if (!IS_ENGLISH_DOUBLE(cLoChar,cHiChar))
				continue;
			else {
				CONVERT_DOUBLE_TO_SINGLE(cLoChar,cLoChar,cHiChar)
			}    
	        } 
		else if (IS_ENGLISH_CHAR(cLoChar)){
			CONVERT_CAPITAL_CHAR(cLoChar,cLoChar)
		}
		else continue;
		iIndex=0; 
		while (pstDirtyList[iIndex++].sDirtyStr[0]!='\0')
		{
			if (EQUAL_ENGLISH_CHAR(cLoChar,pstDirtyList[iIndex-1].sDirtyStr[0]))
		        if (!TLib_Eng_Dirty_Check_Word(sCheckStr+iCount,pstDirtyList[iIndex-1].sDirtyStr+1)){
		            iIsDirty=1;
		            break;
		        }
		}       
	}
	return iIsDirty;
}

/**********************************************************************************************************************
Funciton    检查是否有中文单字脏字眼..
input       sCheckStr: 	//待检查的字符串 
output      sErrMsg	//错误信息	    
return:     0: 		//此字符串不带有‘脏字眼’ 		    
	    1: 		//此字符串带有‘脏字眼’ 		    
	    <0:         //Error
lastmodify  2000.07.17
written by  caocy
***********************************************************************************************************************/	
int TLib_Dirty_SwCheck(unsigned char *sCheckStr,char *sErrMsg)
{	
	unsigned char				sReservBuff[C_MAX_WORD_LEN],cLoChar,cHiChar;
	int						iIsDirty=0,iCount=0,iBufCount=0,iIndex,iOffset;
	TLIB_DIRTY_SW_RECORD *pstDirtySwRec;

	if (pstDirtyCore==NULL)
	{
		sprintf(sErrMsg,"The ShareMemory hasn't beend Initiate");
		return -1;	
	}

	if (sCheckStr==NULL)
	{
		sprintf(sErrMsg,"The checking string is null");
		return -2;	
	}
	
	pstDirtySwRec = pstDirtyCore->astSwDirtyRec ;
	
	/*   开始检查 字符串是否含有 ‘脏字眼’ */
	while ((!iIsDirty)&&(cLoChar=sCheckStr[iCount++])!='\0')
	{
	        cHiChar=sCheckStr[iCount];
		if (!IS_CHINESE_CHAR(cLoChar,cHiChar))
		   continue;
	        if ( IS_CHINESE_PUNC(cLoChar,cHiChar)) // 中文标点符号
	        {
	        	iCount++;
	        	continue;
	        }
		iCount++;
		
		sReservBuff[(iBufCount++)%C_MAX_WORD_LEN]=cLoChar;
		sReservBuff[(iBufCount++)%C_MAX_WORD_LEN]=cHiChar;

		CAL_INDEX_OFFSET2(iIndex,cLoChar,cHiChar)
//		iIndex = iIndex%C_MAX_TABLE_LEN;

		iOffset=pstDirtyCore->iDirtySwIndex[iIndex]; /*该字是否关键字*/

		if (iOffset!=-1)
		{
			//printf("check word: %c%c , %s\n",cLoChar , cHiChar ,pstDirtySwRec[iOffset].sDirtyWord);
			if ( cLoChar == pstDirtySwRec[iOffset].sDirtyWord[0] &&
			     cHiChar ==  pstDirtySwRec[iOffset].sDirtyWord[1] )
			{
				iIsDirty = 1;
				break;
			}
		}       
	}
	return iIsDirty;
}

/************************************************************
// 函数名  : TLib_SW_Dirty_Init()
// 编写者  : 
// 参考资料:
// 功能    : 初始化单字脏话共享内存区
// 输入参数: sSwFileName -- 单字敏感字文件
// 输出参数: 0 -- init ok , -1 -- error
// 备 注   : 
*************************************************************/
int TLib_SW_Dirty_Init (char  *sSwFileName,char  *sErrMsg)
{
	int					iCount=0,iLen,iIndex , i , iOffset;
	unsigned char		sDirtyWord[56]; 
	FILE				*pFile;
	TLIB_DIRTY_SW_RECORD *pstDirtySwRec;
	time_t		 		lLastModifyTime;

	lLastModifyTime = GetFileModifyTime(sSwFileName,sErrMsg);
	if (lLastModifyTime == (time_t)-1)
	{
		printf("%s\n", sErrMsg);
		exit(0);
	}
	pstDirtySwRec = pstDirtyCore->astSwDirtyRec ;

	if (lLastModifyTime == pstDirtyCore->lSwUpdateTime )
	{
#ifdef DEBUG
		printf("File %s Hadn't Modify\n",sSwFileName);
#endif
		return 0;
	}

	/*先初始化 dirtystring list */
	memset(pstDirtySwRec,0,sizeof(pstDirtySwRec));
	for (iCount=0;iCount<S_MAX_WORD_NUM;iCount++)
        pstDirtySwRec[iCount].sDirtyWord[0]='\0';
	
	//init shm 
	for (iCount=0;iCount<C_MAX_TABLE_LEN;iCount++)
	{
		pstDirtyCore->iDirtySwIndex[iCount]=-1;
	}
	
	pFile =fopen(sSwFileName,"r");
	if (pFile==NULL) 
	{
		sprintf(sErrMsg,"Fail to open file:%s",sSwFileName);
		return -1;
	} 
	iCount=0;        
	/* -------- 从文件里读脏话词语 -------------------------*/
	while( fgets(sDirtyWord,55,pFile) != NULL)
	{
		trim(sDirtyWord);
		if ( sDirtyWord[0] == '#' )
			continue;
		iLen = strlen(sDirtyWord);
		for ( i = 0 ; i < iLen ; i++)
			if ( sDirtyWord[i] == '\n' || sDirtyWord[i] == '\r' || sDirtyWord[i] == ' ')
				sDirtyWord[i] = 0;
		
		if (strlen(sDirtyWord) != 2 )
		{
			printf("wrong type word:%s\n",sDirtyWord);
			continue;
		}

		CAL_INDEX_OFFSET(iIndex,sDirtyWord[0],sDirtyWord[1])
		iOffset=pstDirtyCore->iDirtySwIndex[iIndex];

		if (iOffset==-1){
			pstDirtyCore->iDirtySwIndex[iIndex]=iCount;
		}
		else { 
			printf("conflict word :[%x] %s[%s]\n",iIndex,sDirtyWord,pstDirtySwRec[iOffset].sDirtyWord);
			continue;
		}

		strncpy(pstDirtySwRec[iCount++].sDirtyWord,sDirtyWord,3);
		if (iCount>=S_MAX_WORD_NUM)
		{
	        printf("The number of the sw words is too large");	
	        break;
		}
	}
	pstDirtyCore->aiSigWordCount = iCount;
	pstDirtyCore->lSwUpdateTime =  lLastModifyTime;
	
	fclose(pFile);	

	return 0;
}

/************************************************************
// 函数名  : TLib_Dirty_Init()
// 编写者  : 
// 参考资料:
// 功能    : 全局初始化函数
// 输入参数: sPath -- 数据文件所在目录
// 输出参数: 0 -- init ok , -1 -- error
// 备 注   : 
*************************************************************/
int TLib_Dirty_Init(char *sPath, char *sErrMsg)
{
	char 			sFile[50];
	int				iRetCode;
	int				iShmSize;
	unsigned char	*sShm;
		
	assert(sPath!=NULL);
	if (sPath[strlen(sPath)-1] != '/') strcat(sPath, "/");
	snprintf(gsPath,sizeof(gsPath),"%s",sPath);
	
	iShmSize=sizeof(TLIB_DIRTY_CORE);
	if (!(sShm = DirGetShm(iDirtyShmKey, iShmSize, 0666)))
	{
		if (!(sShm = DirGetShm(iDirtyShmKey, iShmSize, 0666 | IPC_CREAT))) 
		{
			sprintf(sErrMsg,"Fail to shmget. ShmKey is %d", iDirtyShmKey);
			return -1;
		}
	}
	pstDirtyCore = (TLIB_DIRTY_CORE *)sShm;

	snprintf(sFile ,sizeof(sFile) ,"%s%s", sPath, sDirtyGbFile);
	iRetCode = TLib_Chn_Dirty_Init(sFile, sErrMsg);
 	if (iRetCode<0)
	 	return iRetCode;

	snprintf(sFile,sizeof(sFile), "%s%s", sPath, sDirtyEngFile);
 	iRetCode = TLib_Eng_Dirty_Init(sFile, sErrMsg);
 	if (iRetCode<0)
	 	return iRetCode;

	snprintf(sFile,sizeof(sFile), "%s%s", sPath, sDirtySwFile);
 	iRetCode = TLib_SW_Dirty_Init(sFile, sErrMsg);
 	if (iRetCode<0)
	 	return iRetCode;

	printf("dirty shm size=%d\n", iShmSize);

	return 0;
}

/************************************************************
// 函数名  : TLib_Dirty_Check()
// 编写者  : 
// 参考资料:
// 功能    : 判断是否有dirtyWord
// 输入参数: sPath -- 数据文件所在目录
// 输出参数: 0 -- 没有敏感字 , -1 -- 有
// 备 注   : 
*************************************************************/
int TLib_Dirty_Check(int iLevel, char *sMsg,char *sErrMsg)
{
    int iRetCode;
    if (pstDirtyCore->iUpdatingFlag)
        return 0;

    if ((iLevel & LEVEL_ENG)==0)
    {
        iRetCode = TLib_Chn_Dirty_Check(iLevel,sMsg,sErrMsg);
        if (iRetCode)
            return -iRetCode;
        if (iLevel & LEVEL_SW)
        {
            if ((iRetCode = TLib_Dirty_SwCheck(sMsg,sErrMsg)))
                return -iRetCode;
        }
    }
    else
    {
    	iRetCode = TLib_Eng_Dirty_Check(sMsg,sErrMsg);
    	if (iRetCode) return -1;
    }

    return 0;
}


int TLib_Dirty_List_Word(int CharSet)
{
	TLIB_DIRTY_CHN_RECORD    *pstDirtyRecord;
	TLIB_DIRTY_ENG_RECORD	 *pstEngRecord;
	TLIB_DIRTY_SW_RECORD	 *pstSwRecord;
	int		i, j ;

	if ( pstChnIndexTab == NULL)
	{
		printf("Not Init ?\n");
		return -1;
	}
	printf("f:%d\n",pstDirtyCore->iUpdatingFlag);
	if ( CharSet ==1) // list chinese word
	{
		pstDirtyRecord= pstDirtyCore->astChnDirtyRec ;
		printf("Chn: %d\n",pstDirtyCore->aiChnWordCount);
		for( i = 0 , j = 0; i <C_MAX_WORD_NUM; i++)
		{
			if ( pstDirtyRecord[i].sKeyWord[0]==0 )
				continue;
			printf("[%03d][%s][%d][%d][%d] %s\t",i,pstDirtyRecord[i].sKeyWord,pstDirtyRecord[i].iKeyOffset,
						pstDirtyRecord[i].iNextKey , pstDirtyRecord[i].iLevel, pstDirtyRecord[i].sDirtyStr);
			j++;
			if ( j%3 == 2)
				printf("\n");
		}
	}
	else if (CharSet ==2 ) // list english word
	{
		pstEngRecord= pstDirtyCore->astEngDirtyRec ;
		printf("Eng: %d\n",pstDirtyCore->aiEngWordCount);
		for( i = 0 , j = 0; i <E_MAX_WORD_NUM; i++)
		{
			if ( pstEngRecord[i].sDirtyStr[0]==0 )
				continue;
			printf("[%03d] %s\t",i,pstEngRecord[i].sDirtyStr);
			j++;
			if ( j%3 == 2)
				printf("\n");
		}
	}
	else  // list single word
	{
		pstSwRecord= pstDirtyCore->astSwDirtyRec ;
		printf("Sig: %d\n",pstDirtyCore->aiSigWordCount);
		for( i = 0, j = 0; i <E_MAX_WORD_NUM; i++)
		{
			if ( pstSwRecord[i].sDirtyWord[0]==0 )
				continue;
			printf("[%03d] %s\t",i,pstSwRecord[i].sDirtyWord);
			j++;
			if ( j%3 == 2)
				printf("\n");
		}
	}
	printf("\n");
	return 0;
}

#endif
