
/********************************   ********************************************/
#ifndef _TLIB_DIRTY_CHECK_H_
#define _TLIB_DIRTY_CHECK_H_

#include <sys/ipc.h>
#include <sys/shm.h>


#ifdef  __cplusplus
extern "C" {
#endif

#define  C_MAX_WORD_LEN   32
#define  C_MAX_TABLE_LEN  256*256
#define  C_MAX_WORD_NUM   5000
#define  E_MAX_WORD_NUM   (C_MAX_WORD_NUM/5)
#define  S_MAX_WORD_NUM   (C_MAX_WORD_NUM/2)

#define  LEVEL_SW			0x8000
#define  LEVEL_ENG			0x4000


#define  IS_DOUBLE_CHAR(ch) 		( ch>=0xA0&&ch<0xFF)
#define  IS_ENGLISH_CHAR(ch)		( (ch>='a'&&ch<='z') || (ch>='A'&&ch<='Z') )
#define  IS_CHINESE_CHAR(lo,hi)		( (lo>0x80 && lo<0xff) && ((hi>0x3f && hi<0x7f)||(hi>0x7f && hi<0xff)))
#define  IS_CHINESE_PUNC(lo,hi)		( (lo>0xa0 && lo<0xb0) && (hi>0x3f && hi<0xff))
#define  IS_ENGLISH_DOUBLE(lo,hi)	( (lo==0xA3) &&( (hi<=0xDA && hi>=0xC1) || (hi<=0xFA && hi>=0xE1) ))
#define  CONVERT_DOUBLE_TO_SINGLE(ch,lo,hi)	ch=(hi>=0xC1&&hi<=0xDA)?(hi-0x80):(hi-0xA0);
#define  CONVERT_CAPITAL_CHAR(ch,lo)			 ch=(lo>='a'&&lo<='z')?lo-0x20:lo;
#define  EQUAL_ENGLISH_CHAR(lo,hi)  ( lo==hi || lo==(hi-0x20))


#define CHAR_KEYWORD_DELI '|'

#define CAL_INDEX_OFFSET(offset,hi,lo)	offset=(hi-0x80)*256+lo;
#define CAL_INDEX_OFFSET2(offset,hi,lo)	offset=(hi-0x80)*256+lo;

typedef struct _TLIB_DIRTY_CHN_RECORD
{
     unsigned char 	sDirtyStr[C_MAX_WORD_LEN+1]; /* 脏话短语 */
     unsigned char 	sKeyWord[3];     /* 关键字眼 */
     short			iKeyOffset;      /* 相对于关键字的偏移 */
     short			iNextKey;        /* 下一个有相同关键字的脏话短语 */ 
     short			iLevel;
}TLIB_DIRTY_CHN_RECORD;

typedef struct _TLIB_DIRTY_ENG_RECORD 
{
	unsigned char sDirtyStr[C_MAX_WORD_LEN+1];
}TLIB_DIRTY_ENG_RECORD;

typedef struct _TLIB_DIRTY_SINGLE_WORD_RECORD 
{
	unsigned char sDirtyWord[3];
}TLIB_DIRTY_SW_RECORD;

typedef struct _TLIB_DIRTY_CHN_STRUCT 
{
     short   iDirtyIndexTable[C_MAX_TABLE_LEN];
} TLIB_DIRTY_CHN_STRUCT;

typedef struct _TLIB_DIRTY_CORE
{
	int						iUpdatingFlag;
	time_t					lChnUpdateTime;
	time_t					lEngUpdateTime;
	time_t					lSwUpdateTime;
	TLIB_DIRTY_CHN_STRUCT	stChnDirtyStruc;
	int						aiChnWordCount;
	int						aiEngWordCount;
	int						aiSigWordCount;
	TLIB_DIRTY_CHN_RECORD	astChnDirtyRec[C_MAX_WORD_NUM];
	TLIB_DIRTY_ENG_RECORD	astEngDirtyRec[E_MAX_WORD_NUM];
	short   				iDirtySwIndex[C_MAX_TABLE_LEN];
	TLIB_DIRTY_SW_RECORD    astSwDirtyRec[S_MAX_WORD_NUM];
}TLIB_DIRTY_CORE;


typedef struct
{
     unsigned char 	sDirtyStr[C_MAX_WORD_LEN+1]; /* 脏话短语 */
     unsigned char 	sKeyWord[3];     /* 关键字眼 */
     unsigned short	iLevel;
}Dirty_Chn_Item;
typedef struct
{
	unsigned char sDirtyStr[C_MAX_WORD_LEN+1];
}Dirty_Eng_Item;

/***********************************************************************
  中文脏话格式:
  如:
  ...
  龟孙子|龟
  他妈的|妈        
  ...
  其中后为关键字(每句脏词眼都有个关键字，英文的例外)
  
  英文格式:
  ...
  Fuck you 
  shit
  bitch
  ...
  以换行符号为标隔
************************************************************************/
int TLib_Dirty_Check(int iLevel,char *sMsg,char *sErrMsg);
int TLib_Dirty_Init(char *sPath, char *sErrMsg);
int TLib_Dirty_List_Word(int CharSet);
int TLib_Chn_Dirty_Word_Add(Dirty_Chn_Item *pItem , char *pszDirtyFilePath,char *sErrMsg);

/* *************************************************************************
 

**************************************************************************/

#ifdef  __cplusplus
}
#endif

#endif

