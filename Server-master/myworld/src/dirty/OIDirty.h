
#ifndef __OIDIRTY_H__
#define __OIDIRTY_H__

#ifdef __cplusplus
extern "C" 
{
#endif

#ifndef _WIN32
#include <regex.h>
#endif

#define	C_MAX_TABLE_LEN		256*256
#define	C_MAX_WORD_LEN		256  //�໰�ؼ�������ֽ���
#define	E_MAX_WORD_NUM		200
#define	C_MAX_WORD_NUM		5000

#define	IS_ENGLISH_CHAR(ch)					( (ch>='a'&&ch<='z') || (ch>='A'&&ch<='Z') )
#define	IS_DOUBLE_CHAR(ch)					( ch>=0xA0&&ch<0xFF )
#define	IS_ENGLISH_DOUBLE(lo,hi)			( (lo==0xA3) &&( (hi<=0xDA && hi>=0xC1) || (hi<=0xFA && hi>=0xE1) ))
#define	CONVERT_DOUBLE_TO_SINGLE(ch,lo,hi)	ch=(hi>=0xC1&&hi<=0xDA)?(hi-0x80):(hi-0xA0)
#define	CONVERT_CAPITAL_CHAR(ch,lo)			ch=(lo>='a'&&lo<='z')?lo-0x20:lo
#define	EQUAL_ENGLISH_CHAR(lo,hi)			( lo==hi || lo==(hi-0x20) )

#define	IS_DIGIT(ch)						( isdigit(ch) )
#define	IS_DIGIT_DOUBLE(lo,hi)				( (lo==0xA3) &&(hi>=0xB0 && hi<=0xB9) )
#define	CONVERT_DOUBLE_DIGIT_TO_SINGLE(ch,lo,hi)	ch=hi-0x80

//IS_CHINESE_CHAR������ϰ�����ȫ��Ӣ�ĺ�ȫ������
#define	IS_CHINESE_CHAR(lo,hi)				( (lo>0x80 && lo<0xff) && ( (hi>0x3f && hi<0x7f) || (hi>0x7f && hi<0xff) ) )
//IS_CHINESE_PUNC���ı�����
#define	IS_CHINESE_PUNC(lo,hi)				( (lo>0xa0 && lo<0xaa) && (hi>0x3f && hi<0xff))
#define	CAL_INDEX_OFFSET(offset,hi,lo)		offset=(hi-0x80)*256+lo;

/************************************************************************
  data structure
************************************************************************/
typedef struct _OI_DIRTY_CHN_INDEX 
{
     short   iDirtyIndexTable[C_MAX_TABLE_LEN];
} OI_DIRTY_CHN_INDEX;

typedef struct _OI_DIRTY_CHN_RECORD
{
     unsigned char 	sDirtyStr[C_MAX_WORD_LEN+1];	//�໰����
     unsigned char 	sKeyWord[3];					//�ؼ�����
	 unsigned char	bMix;							//�Ƿ���Ӣ���
     short			iKeyOffset;						//����ڹؼ�����sDirtystr�е�ƫ��
     short			iNextKey;						//��һ������ͬ�ؼ��ֵ��໰����
     int			iLevel;

     //char			sBase[20];
} OI_DIRTY_CHN_RECORD; //�ڴ�������

typedef struct _OI_DIRTY_ENG_RECORD 
{
	unsigned char	sDirtyStr[C_MAX_WORD_LEN+1];
	int				iLevel;
	//char			sBase[20];
} OI_DIRTY_ENG_RECORD;//�ڴ���Ӣ��

typedef struct _OI_DIRTY_REG_RECORD
{
	unsigned char	sDirtyStr[E_MAX_WORD_NUM+1];
	int				iLevel;
	//char			sBase[20];

#ifndef _WIN32
	regex_t stRegular;		                     //������������ʽ�ṹ��
#endif

} OI_DIRTY_REG_RECORD;//�ڴ���������ʽ

typedef struct _OI_DIRTY_SINGLE_WORD_RECORD 
{
	unsigned char sDirtyWord[3];
} OI_DIRTY_SW_RECORD;

typedef struct _OI_DIRTY_CORE
{
	int						iEnable ;            //�Ƿ���Ч  1 ��Ч
    //int					iReserve;	         //�汾��
	long					lChnUpdateTime;      //�ļ���ʱ��
	long					lEngUpdateTime;
	long					lSwUpdateTime;
	
	OI_DIRTY_CHN_INDEX	    stChnDirtyIndex;     //����
	int						aiChnWordCount;      //��������Ŀ
	int						aiEngWordCount;
	int						aiSigWordCount;
	int						aiRegWordCount;
	OI_DIRTY_CHN_RECORD	    astChnDirtyRec[C_MAX_WORD_NUM]; //���Ĺؼ��ּ�¼
	OI_DIRTY_ENG_RECORD	    astEngDirtyRec[E_MAX_WORD_NUM]; //Ӣ�Ĺؼ��ּ�¼
	OI_DIRTY_REG_RECORD	    astRegDirtyRec[E_MAX_WORD_NUM]; //������ʽ��¼
	short   				iDirtySwIndex[C_MAX_TABLE_LEN];
	OI_DIRTY_SW_RECORD      astSwDirtyRec[E_MAX_WORD_NUM];
	
	unsigned long           ulChnVer;            //��Ӣ�ĵİ汾��
	unsigned long           ulEngVer;
	char					sReserve[112];
} OI_DIRTY_CORE; //�ڴ�ṹ

#ifdef __cplusplus
}
#endif

typedef struct _DIRTY_CHECK
{
	OI_DIRTY_CHN_INDEX  * m_pstChnIndexTab;      //ָ���ڴ������ĵ�����
	OI_DIRTY_CORE		* m_pstDirtyCore;        //ָ��dirty���õ��ڴ�
	OI_DIRTY_ENG_RECORD * m_pstDirtyEngRec;      //ָ���ڴ���Ӣ������	
}DIRTY_CHECK;


int OI_Dirty_Init(DIRTY_CHECK * pstSelf);
int OI_Dirty_Fini(DIRTY_CHECK * pstSelf);

void OI_Dirty_Report(DIRTY_CHECK * pstSelf);
	
#ifdef _USESHM_
void OI_Dirty_Attach(DIRTY_CHECK * pstSelf, void * ptrAddr);
#endif

int OI_Dirty_Eng_GetFromLineBuff (DIRTY_CHECK * pstSelf, const char *pszBuff);
int OI_Dirty_Chn_GetFromLineBuff(DIRTY_CHECK * pstSelf, const char *pszBuff);
int OI_Dirty_Reg_GetFromLineBuff(DIRTY_CHECK * pstSelf, const char *pszBuff);
	
int OI_Dirty_Eng_GetFromFile	(DIRTY_CHECK * pstSelf, const char * pszEngFile);
int OI_Dirty_Chn_GetFromFile	(DIRTY_CHECK * pstSelf, const char * pszChnFile);
int OI_Dirty_Regular_GetFromFile(DIRTY_CHECK * pstSelf, const char * pszRegFile);

int OI_Dirty_Rreplace		(DIRTY_CHECK * pstSelf, int iLevel, char * pszContent, int * piDirtyLevel, int iCond);
int OI_Dirty_Check			(DIRTY_CHECK * pstSelf, int iLevel, char * pszContent, int * piDirtyLevel, int * piBeginIdx, int * piEndIdx);

int OI_Dirty_Chn_Check	(DIRTY_CHECK * pstSelf, int iLevel, unsigned char * sCheckStr, int * piDirtyLevel, int * piBeginIdx, int * piEndIdx, int iCond);
int OI_Dirty_Eng_Check	(DIRTY_CHECK * pstSelf, int iLevel, unsigned char * sCheckStr, int * piDirtyLevel, int * piBeginIdx, int * piEndIdx, int iCond);
int OI_Dirty_Reg_Check	(DIRTY_CHECK * pstSelf, int iLevel, unsigned char * sCheckStr, int * piDirtyLevel, int * piBeginIdx, int * piEndIdx, int iCond);
int OI_Dirty_Mix_Check	(DIRTY_CHECK * pstSelf, int iLevel, unsigned char * sCheckStr, int * piDirtyLevel, int iReplace, int iCond);


#endif /* __OIDIRTY_H__ */
