#ifndef TRESLOADER_H
#define TRESLOADER_H

#include <stdio.h>
#include <stdarg.h>
#include "ResConv.h"


#define RL_MAX_PATH_LEN		256

#define RL_LOADMODE_XMLV1	1		/*��ȡǶ�׽ṹ������������ʶ��xml��Դ�ļ�*/
#define RL_LOADMODE_XMLV2	2		/*��ȡǶ�׽ṹ���Գ�Ա����ʶ��xml��Դ�ļ�*/
#define RL_LOADMODE_BIN		3		/*��ȡ���������ݸ�ʽ����Դ�ļ�*/

#ifndef RES_ID_ARRAY
#define RES_ID_ARRAY	1
#endif

/** @name TRC_LOADR����ID
*  @{
*/
#define RL_ERROR_OPEN_FILE_FAILED		-1		/*���ļ���ʧ��*/
#define RL_ERROR_READ_FILE_FAILED		-2		/*���ļ���ʧ��*/
#define RL_ERROR_CHECK_FAILE_FAILED		-3		/*������Դ�ļ�ʧ�ܣ�������Դ�ļ��İ汾��ħ���뱾�ӿڶ���Ĳ�һ��*/
#define RL_ERROR_NO_MEMORY				-4		/*�����ڴ�ʧ��*/
#define RL_ERROR_UNIT_SIZE_TOO_SMALE		-5		/*ָ���ĵ�����Դ�洢�ռ��Ԥ�ڵ�ҪС*/
#define RL_ERROR_BUFFER_SIZE_TO_SMALL	-6		/*������Դ���������ռ䲻��*/
#define RL_ERROR_DIFF_HASH 	-7		/*��Դ�ļ��м�¼��DRԪ���ݿ�hashֵ�뵱ǰָ����hashֵ��ͬ��˵����Դ�ṹ������Ѿ����*/
#define RL_ERROR_READ_XML_FILE_FAILED -8	/*��ȡxml�ļ�ʧ��*/
#define RL_ERROR_FAILED_TO_GET_HEAD_META -9	/*��ȡTResHeadԪ�����������ʧ��*/
#define RL_ERROR_FAILED_TO_READ_XRESHEAD -10	/*��xml�ж�ȡTResHeadʧ��*/
#define RL_ERROR_INVALID_PARAM -11	/*����ӿڵĲ�����Ч*/
/**   @}*/

/** @name TRC_LOADR��Դ���ر�ʶλ
*  @{
*/
#define RL_FLAG_INGORE_DIFF_HASH		0x00000001		/*������Դ�ļ���Ԫ�����������hashֵ�뵱ǰhashֵ�Ĳ�ͬ������������Դ*/
/**   @}*/




/*��ʾ�ò���ֻ���������*/
#ifndef IN
#define IN  
#endif


/*��ʾ�ò���ֻ���������*/
#ifndef OUT
#define OUT
#endif


/*��ʾ�ò���������������������������*/
#ifndef INOUT
#define INOUT
#endif

/* automatically include the correct library on windows */
#ifdef WIN32

	# ifdef _DEBUG
		#  define LIB_RESLOADER_D    "_d"
	# else
		#  define LIB_RESLOADER_D
	# endif /* _DEBUG */

	# if defined(LIB_RESLOADER_D)
		//# pragma comment( lib, "libresloader"  LIB_RESLOADER_D ".lib" )
	# else
		//# pragma comment( lib, "libresloader.lib" )
	# endif/*# if defined(LIB_RESLOADER_D)*/
#endif /*#ifdef WIN32*/


/*Ϊ�����Ͻӿڶ�����Ľӿ�ת��*/
#define rl_load	trl_load
#define rl_unload trl_unload
#define rl_specail_load	trl_specail_load
#define rl_xload_init	trl_xload_init
#define rl_xload_with_reshead	trl_xload_with_reshead
#define rl_xload	trl_xload
#define rl_find	trl_find
#define rl_find_ll	trl_find_ll
#define rl_find_n	trl_find_n

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup TRC_LOADR_VFILE �����ļ�ϵͳ֧�ֽӿ�
*  @{
*/

/*file stream open function*/
typedef FILE* (*LPFOPEN)( const char *path, const char *mode ); 

/*close a file stream*/
typedef int (*LPFCLOSE)(FILE *fp);	

	/*binary stream input function*/
typedef	size_t (*LPFREAD)(void *ptr, size_t size, size_t nmemb, FILE *stream); 	 

	/*function sets the file position indicator for the stream pointed to by stream*/
typedef int (*LPFSEEK)(FILE *stream, long offset, int whence);

	/*obtains the current value of the file position indicator for the stream pointed to by stream*/
typedef long (*LPFTELL)(FILE *stream);	


/*�ļ�ϵͳIOAPI�ص�������open,fread,fwrite���ļ�ϵͳio�ӿ�������*/
struct tagTFileIOInterface
{
	/*file stream open function*/
	LPFOPEN pfopen; 

	/*close a file stream*/
	LPFCLOSE pfclose;	

	/*binary stream input function*/
	LPFREAD pfread; 	 

	/*function sets the file position indicator for the stream pointed to by stream*/
	LPFSEEK pfseek;

	/*obtains the current value of the file position indicator for the stream pointed to by stream*/
	LPFTELL pftell;	
};
typedef struct tagTFileIOInterface TFILEIOINTERFACE;
typedef struct tagTFileIOInterface *LPTFILEIOINTERFACE;


/**�����ļ�ϵͳI/O�����ӿ�
@param[in] a_pstInterface �����ļ�ϵͳIO������ؽӿڵ����ݽṹָ��
*@retval 0 �ɹ�
*@retval <0	ʧ��
@pre \e a_pstInterface ����ΪNULL
@pre \e a_pstInterface��ָ�ṹ���и��ӿ�ָ�벻��ΪNULL
@note ���ӿ��Ƿ��̰߳�ȫ�ģ�����ڶ��̻߳������ñ��ӿڣ������߱��뻥����ñ��ӿ�
@note ֻ��ʹ�������ļ�ϵͳ�����ڼ�����Դ֮ǰ����һ�Σ���ָ��������ļ�ϵͳio�ӿڡ�
@note ��������ñ��ӿڣ���Դ���ؿ��ʹ��C���ṩ���ļ�io�����ӿ�:fopen,fread,fseek��
*/
int trl_set_fileio_interface(LPTFILEIOINTERFACE a_pstInterface);
/**  @}*/ /** @defgroup TRC_LOADR_VFILE �����ļ�ϵͳ֧�ֽӿ�*/


/** @defgroup TRC_LOADR_BIN �����Ƹ�ʽ����Դ�ļ����ؽӿ�
*  @{
*/




/** ����Դ�ļ��м�����Դ���������У��������Ŀռ��ں����ڷ���
*@param[in,out] ppvBuff �������ݵ�������ָ���ָ��
*@param[out] piBuff	��ȡ���������ֽ���
*@param[out] piUnit	��ȡ������Դ��Ϣ�ṹ����ڻ������еĴ洢�ռ��С
*@param[in] pszFilePath	��Դ�ļ���
*@param[in] pszMetalibHash ����Դ�ļ�����Դ�ṹ��ӦDRԪ�����������hashֵ
*@param[in] iFlag	��Դ���ؿ��Ʊ�ʶλ���˲�������Ϊ0�����б�ʶλ��λ'��'ֵ:
*	-	RL_FLAG_INGORE_DIFF_HASH	����Ԫ����������hash�Ĳ�ͬ
*
*note	������Դ��Ϣ�Ļ������������trl_unload�����ͷ�
*note Ŀǰ��Դ�ļ���ͷ����¼��������Դ�ļ�ʱ����Դ�ṹ���ӦӦDR
*	Ԫ���ݿ��hashֵ��pszMetalibHash��������Ӧ�õ�ǰʹ�õ���Դ�ṹDR Ԫ���ݿ�
*	��hashֵ��ͨ�����hashֵ�Ƿ���ͬ�������ж���Դ�ļ�����Դ�ṹ�İ汾
*	�뵱ǰ�ṹ�İ汾�Ƿ���ͬ�������Դ�ṹ�汾��ͬ���������Դ
*	ʧ�ܣ������ܱ�������Դ�ṹ���������ش������Դ
*note	���pszMetalibHash������ΪNULL, �򲻼��Ԫ���ݿ��hashֵ����ʱ���ܼ��
*	��Դ�ļ��нṹ���뵱ǰʹ�õ���Դ�ṹ�Ƿ�һ��
*note 	��ǰ��Դ�ṹDR Ԫ��������hashֵ�Ļ�ȡ;����:
*	-	ͨ��ʹ��md5sum�������õ�ǰ������DRԪ���ݿ��ļ�����
*	-	����tdr���ߣ�����Դ�ṹ����ת����.hͷ�ļ�ʱ���Ὣ����Դ�ṹ
*		DRԪ���ݿ��hashֵд��ͷ�ļ��У�
*note ���ʹ�������ļ����洢�洢��Դ�ļ�������ñ��ӿ�֮ǰ�������trl_set_fileio_interface����
*	�����ļ�IO�����ӿ�
*@see trl_set_fileio_interface
*
*@retval ����  ��ȡ����Դ����
*@retval 0	�ļ���û����Դ
*@retval ������ ������Դʧ�ܣ����ܷ���ֵ�ʹ���ԭ��
*	�� RL_ERROR_OPEN_FILE_FAILED ���ļ���ʧ��
*	-  RL_ERROR_READ_FILE_FAILED	���ļ���ʧ��
*	- RL_ERROR_CHECK_FAILE_FAILED		������Դ�ļ�ʧ�ܣ�������Դ�ļ��İ汾��ħ���뱾�ӿڶ���Ĳ�һ��
*	- RL_ERROR_NO_MEMORY				�����ڴ�ʧ��
*	- RL_ERROR_DIFF_HASH	��Դ�ṹԪ����������hash��ͬ
*/
int trl_load(INOUT char** ppvBuff, OUT int *piBuff, OUT int *piUnit, IN const char* pszFilePath, 
	IN const char *pszMetalibHash, IN int iFlag);


/* �ͷ���Դ��ռ�Ļ������ռ�
*@param[in,out] ppvBuff �������ݵĻ�����ָ���ָ��
*
*/
void trl_unload(INOUT char** ppvBuff);


/** �Ӷ�������Դ�ļ��ж�ȡ����Դ��Ϣͷ��
*@param[in] a_pszFilePath	��Դ�ļ���
*@param[in] a_pstResHead ������Դ��Ϣͷ����Ϣ�Ľṹ��ָ��
*
*note ���ʹ�������ļ����洢�洢��Դ�ļ�������ñ��ӿ�֮ǰ�������trl_set_fileio_interface����
*	�����ļ�IO�����ӿ�
*@see trl_set_fileio_interface
*retval 0	�ɹ�
*@retval ����  ʧ��,���ܷ���ֵ�ʹ���ԭ��
*	�� RL_ERROR_OPEN_FILE_FAILED ���ļ���ʧ��
*	-  RL_ERROR_READ_FILE_FAILED	���ļ���ʧ��
*	- RL_ERROR_CHECK_FAILE_FAILED		������Դ�ļ�ʧ�ܣ�������Դ�ļ��İ汾��ħ���뱾�ӿڶ���Ĳ�һ��
*/
int trl_read_reshead(IN const char* a_pszFilePath, IN LPTRESHEAD a_pstResHead);



/** ָ��������Դ���ݵĻ����С��ָ��������Դ���ݵ��ڴ��С,����Դ�ļ��н���Դ��С�������У��������Ŀռ��ں��������
*@param[in] pszBuff �������ݵĻ������׵�ַ
*@param[in] iBuff	�������Ŀ����ֽ���
*@param[in] iUnit	ָ��������Դ���ݽṹ������ڴ��еĴ洢�ռ䣬ͨ��������������߿���Ϊÿ����Դ�����ʵ����Ҫ�洢�ռ����Ŀռ䡣
����˲�����ֵΪ0���򵥸���Դ��Ϣ�ṹ��Ĵ洢�ռ�Ϊʵ������ռ�
*@param[in] pszFilePath	��Դ�ļ���
*@param[in] pszMetalibHash ����Դ�ļ�����Դ�ṹ��ӦDRԪ�����������hashֵ
*@param[in] iFlag	��Դ���ؿ��Ʊ�ʶλ���˲�������Ϊ0�����б�ʶλ��λ'��'ֵ:
*	-	RL_FLAG_INGORE_DIFF_HASH	����Ԫ����������hash�Ĳ�ͬ
*
*note	������Դ��Ϣ�Ļ������������trl_unload�����ͷ�
*note Ŀǰ��Դ�ļ���ͷ����¼��������Դ�ļ�ʱ����Դ�ṹ���ӦӦDR
*	Ԫ���ݿ��hashֵ��pszMetalibHash��������Ӧ�õ�ǰʹ�õ���Դ�ṹDR Ԫ���ݿ�
*	��hashֵ��ͨ�����hashֵ�Ƿ���ͬ�������ж���Դ�ļ�����Դ�ṹ�İ汾
*	�뵱ǰ�ṹ�İ汾�Ƿ���ͬ�������Դ�ṹ�汾��ͬ���������Դ
*	ʧ�ܣ������ܱ�������Դ�ṹ���������ش������Դ
*note	���pszMetalibHash������ΪNULL, �򲻼��Ԫ���ݿ��hashֵ����ʱ���ܼ��
*	��Դ�ļ��нṹ���뵱ǰʹ�õ���Դ�ṹ�Ƿ�һ��
*note 	��ǰ��Դ�ṹDR Ԫ��������hashֵ�Ļ�ȡ;����:
*	-	ͨ��ʹ��md5sum�������õ�ǰ������DRԪ���ݿ��ļ�����
*	-	����tdr���ߣ�����Դ�ṹ����ת����.hͷ�ļ�ʱ���Ὣ����Դ�ṹ
*		DRԪ���ݿ��hashֵд��ͷ�ļ��У�
*
*note ���ʹ�������ļ����洢�洢��Դ�ļ�������ñ��ӿ�֮ǰ�������trl_set_fileio_interface����
*	�����ļ�IO�����ӿ�
*@see trl_set_fileio_interface
*@retval ����  ��ȡ����Դ����
*@retval 0	�ļ���û����Դ
*@retval ������ ������Դʧ�ܣ����ܷ���ֵ�ʹ���ԭ��
*	�� RL_ERROR_OPEN_FILE_FAILED ���ļ���ʧ��
*	-  RL_ERROR_READ_FILE_FAILED	���ļ���ʧ��
*	- RL_ERROR_CHECK_FAILE_FAILED		������Դ�ļ�ʧ�ܣ�������Դ�ļ��İ汾��ħ���뱾�ӿڶ���Ĳ�һ��
*	- RL_ERROR_NO_MEMORY				�����ڴ�ʧ��
*	- RL_ERROR_DIFF_HASH	��Դ�ṹԪ����������hash��ͬ
*/
int trl_specail_load(IN char* pszBuff, IN int iBuff, IN int iUnit, IN const char* pszFilePath, 
					IN const char *pszMetalibHash, IN int iFlag);

/** @} */  /*TRC_LOADR_BIN ��������Դ�ļ����ؽӿ�*/




/** @defgroup TRC_LOADR_XML XML��ʽ����Դ�ļ����ؽӿ�
*  @{
*/

/** ��ʼ��������Դ�Ľӿ�
*@param[in] pszResDir	��Դ�ļ�����Ŀ¼
*@param[in] pszTDRFile ������Դ�ṹ���Ԫ���ݿ��ļ��ļ���
*@param[in] cLoadMode	��Դ�ļ��ĸ�ʽ,��ȡֵΪ��
*	-	 RL_LOADMODE_XMLV1	1		��ȡǶ�׽ṹ������������ʶ��xml��Դ�ļ�
*	-	 RL_LOADMODE_XMLV2	2		��ȡǶ�׽ṹ���Գ�Ա����ʶ��xml��Դ�ļ�
*	-	 RL_LOADMODE_BIN		3	��ȡ���������ݸ�ʽ����Դ�ļ�
*@return �ɹ�����0��ʧ�ܷ��ظ���
*@note ���ӿڷ��̰߳�ȫ,�ڶ��̻߳���,Ӧ�������л������
*@note Ŀǰ��֧��ʹ�������ļ�ϵͳ���洢xml�ļ���ʽ����Դ�ļ�
*/
int trl_xload_init(IN const char *pszResDir, IN const char *pszTDRFile, IN char cLoadMode);

/** �ͷż�����Դ�ӿڵ���Դ
*/
void trl_xload_fini();

/** ��XML��ʽ����Դ�ļ��ж�ȡ����Դ��Ϣͷ��
*@param[in] a_pszFilePath	��Դ�ļ���
*@param[in] a_pstResHead ������Դ��Ϣͷ����Ϣ�Ľṹ��ָ��
*
*retval 0	�ɹ�
*@retval ����  ʧ��,���ܷ���ֵ�ʹ���ԭ��
*	- RL_ERROR_CHECK_FAILE_FAILED		������Դ�ļ�ʧ�ܣ�������Դ�ļ��İ汾��ħ���뱾�ӿڶ���Ĳ�һ��
*	- RL_ERROR_FAILED_TO_GET_HEAD_META 	��ȡTResHeadԪ�����������ʧ��
*	- RL_ERROR_FAILED_TO_READ_XRESHEAD 	��xml�ж�ȡTResHeadʧ��
*/
int trl_xread_reshead(IN const char* a_pszFilePath, IN LPTRESHEAD a_pstResHead);


/** ָ��������Դ���ݵĻ����С��ָ��������Դ���ݵ��ڴ��С,����Դ�ļ��н���Դ��С�������У��������Ŀռ��ں��������
* �����Դ�ļ�Ϊxml�ļ�������ļ��б��������������Դͷ��
*@param[in] pszMetaName ��Դ�ṹ���Ԫ���������ַ�����ָ��
*@param[in] pszBuff �������ݵĻ������׵�ַ
*@param[in] iBuff	�������Ŀ����ֽ���
*@param[in] iUnit	������Դ��Ϣ�ṹ����ڻ������еĴ洢�ռ䣬ͨ��������������߿���Ϊÿ����Դ�����ʵ�ʴ洢�ռ����Ŀռ䡣
*@param[in] pszFilePath	��Դ�ļ���
*@param[in] iFlag	��Դ���ؿ��Ʊ�ʶλ���˲�������Ϊ0�����б�ʶλ��λ'��'ֵ:
*	-	RL_FLAG_INGORE_DIFF_HASH	����Ԫ����������hash�Ĳ�ͬ
*@note ʹ�ô˽ӿ�֮ǰ���������trl_xload_init���г�ʼ��
*  @code
	�����Դ�ļ���xml�ļ������xml�е���Դ���ݱ��������¸�ʽ��
	<ResExperience_Tab>
		<ResHead version="2147483647">
			<Magic>1397052237 </Magic>
			<Version>2 </Version>
			<Unit>12 </Unit>
			<Count>100 </Count>
			<MetalibHash>b97379b24b0aa935f87b6658190325a5 </MetalibHash>
		</ResHead>
		<ResExperience version="2147483647">
			<Level>1 </Level>
			<Value>10 </Value>
			<Total>0 </Total>
		</ResExperience>
		..........
	</Res_ResExperience>
*  @endcode
*@retval ����  ��ȡ����Դ����
*@retval 0	�ļ���û����Դ
*@retval ������ ������Դʧ��
*
*@see trl_xload_init
*/
int trl_xload_with_reshead(IN const char *pszMetaName, IN char* pszBuff, IN int iBuff, IN int iUnit, 
						  IN const char* pszFilePath, IN int iFlag);



/** ָ��������Դ���ݵĻ����С��ָ��������Դ���ݵ��ڴ��С,����Դ�ļ��н���Դ��С�������У��������Ŀռ��ں��������
*@param[in] pszMetaName ��Դ�ṹ���Ԫ���������ַ�����ָ��
*@param[in] pszBuff �������ݵĻ������׵�ַ
*@param[in] iBuff	�������Ŀ����ֽ���
*@param[in] iUnit	ָ��������Դ���ݽṹ������ڴ��еĴ洢�ռ䣬ͨ��������������߿���Ϊÿ����Դ�����ʵ����Ҫ�洢�ռ����Ŀռ䡣
����˲�����ֵΪ0���򵥸���Դ��Ϣ�ṹ��Ĵ洢�ռ�Ϊʵ������ռ�
*@param[in] pszFilePath	��Դ�ļ���
*@param[in] iFlag	��Դ���ؿ��Ʊ�ʶλ���˲�������Ϊ0�����б�ʶλ��λ'��'ֵ:
*	-	RL_FLAG_INGORE_DIFF_HASH	����Ԫ����������hash�Ĳ�ͬ
*
*@note ʹ�ô˽ӿ�֮ǰ���������trl_xload_init���г�ʼ��
*
*  @code
	�����Դ�ļ���xml�ļ������xml�е���Դ���ݱ��������¸�ʽ��
	<ResExperienceTab>
		<Total>100</Total>		
		<ResExperience version="2147483647">
			<Level>1 </Level>
			<Value>10 </Value>
			<Total>0 </Total>
		</ResExperience>
		..........
	</ResExperienceTab>
*  @endcode
*@retval ����  ��ȡ����Դ����
*@retval 0	�ļ���û����Դ
*@retval ������ ������Դʧ��
*
*@see trl_xload_init
*/
int trl_xload(IN const char *pszMetaName, IN char *pszBuff, IN int iBuff, IN int iUnit, IN const char* pszFilePath, IN int iFlag);

/** @} */  /*TRC_LOADR_XML XML��ʽ����Դ�ļ����ؽӿ� */



/** @defgroup TRC_RES_FIND ��Դ����
*  @{
*/

/** ������Դ��Ϣ�Ĺؼ�����Ϣ������Դ�ļ������ض���Դ��Ϣ
*@param[in] pszBuff ������Դ���ݵĻ������׵�ַ
*@param[in] iCount	�˻������б������Դ��Ϣ�ṹ��ĸ���
*@param[in] iUnit	������Դ��Ϣ�ṹ����ڻ������еĴ洢�ռ�
*@param[in] iKey	���ҹؼ���
*@return ����ɹ��ҵ����ش���Դ��Ϣ���׵�ַ�����򷵻�NULL
*
*@pre \e ����Դ��Ϣ�ṹ�ĵ�һ����Ա��Ϊ���ҹؼ��֣������ͱ������������͡�
*/
char* trl_find(char* pszBuff, int iCount, int iUnit, int iKey);

/** ������Դ��Ϣ�Ĺؼ�����Ϣ������Դ�ļ������ض���Դ��Ϣ
*@param[in] pszBuff ������Դ���ݵĻ������׵�ַ
*@param[in] iCount	�˻������б������Դ��Ϣ�ṹ��ĸ���
*@param[in] iUnit	������Դ��Ϣ�ṹ����ڻ������еĴ洢�ռ�
*@param[in] llKey	���ҹؼ���
*@return ����ɹ��ҵ����ش���Դ��Ϣ���׵�ַ�����򷵻�NULL
*
*@pre \e ����Դ��Ϣ�ṹ�ĵ�һ����Ա��Ϊ���ҹؼ��֣������ͱ����ǳ��������͡�
*/
char* trl_find_ll(char* pszBuff, int iCount, int iUnit, tdr_longlong llKey);


/** ������Դ��Ϣ�Ĺؼ�����Ϣ������Դ�ļ������ض���Դ��Ϣ
*@param[in] pszBuff ������Դ���ݵĻ������׵�ַ
*@param[in] iCount	�˻������б������Դ��Ϣ�ṹ��ĸ���
*@param[in] iUnit	������Դ��Ϣ�ṹ����ڻ������еĴ洢�ռ�
*@param[in] nKey	���ҹؼ���
*@return ����ɹ��ҵ����ش���Դ��Ϣ���׵�ַ�����򷵻�NULL
*
*@pre \e ����Դ��Ϣ�ṹ�ĵ�һ����Ա��Ϊ���ҹؼ��֣������ͱ����Ƕ��������͡�
*/
char* trl_find_n(char* pszBuff, int iCount, int iUnit, short nKey);

/**  @}*/
#ifdef __cplusplus
}
#endif

#endif /* TRESLOADER_H */
