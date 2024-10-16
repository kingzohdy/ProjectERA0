/**
*
* @file     tdr_data_io.h 
* @brief    TDRԪ���ݿ�IO����ģ��
* 
* @author steve jackyai  
* @version 1.0
* @date 2007-03-26 
*
*
* Copyright (c)  2007, ��Ѷ�Ƽ����޹�˾���������з���
* All rights reserved.
*
*/


#ifndef TDR_DATA_IO_H
#define TDR_DATA_IO_H

#include <stdio.h>
#include "tdr/tdr_types.h"
#include "tdr/tdr_define.h"
#include "tdr/tdr_external.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



/**@defgroup  TDR_METALIB_IO TDR_���ļ�/�ļ����б���/�ָ�Ԫ���ݿ�
* @{
*/

/**��Ԫ�������������Ƹ�ʽ�ļ��м���Ԫ���ݿ�,��У����ɢ��ֵ�Ƿ��������ɢ��ֵ(NULL)һ��
* @param[in,out] a_ppstLib ����Ԫ���ݿ��ָ���ָ��
* @param[in] a_pszBinFile ����Ԫ���ݿ�����Ƹ�ʽ���ļ����ļ���
* @param[in] a_pszHash ���ӻ���ҪУ���Ԫ����������ɢ��ֵ�ַ���ָ��
*
* @note Ԫ���ݿ�Ŀռ��ں����ڶ�̬���䣬�����tdr_free_lib�ͷ�Ԫ���ݿ�Ŀռ�
* @note ����TDR�Ĺ��߽�Ԫ����������ת����.h�ļ�ʱ���ὫԪ���ݿ��ɢ��ֵ�ַ�����Ϊ�궨��д��.h�ļ��У��˺�
* ��������ֵ���������Ϊ: "TDR_" + "Ԫ������������_" + "HASH"
* @note ���a_pszHash����ΪNULL���򲻽���ɢ��ֵУ�飬��ʱ�˽ӿں�tdr_load_metalib�ӿڵĹ�����ͬ
*
*@retval 0   ����ɹ�
*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ������:
*	- TDR_ERROR_FAILED_OPEN_FILE_TO_READ	���ļ���ʧ��
*	- TDR_ERROR_INVALID_METALIB_FILE	Ԫ���ݿ��ļ����ֽ���Ϊ0�����ȡ���ֽ�����Ԫ����������ṹ�м�¼���ֽ�����ͬ
*	- TDR_ERROR_BUILD_VERSION_CONFLICT	TDR����Ԫ���ݿ��ļ��Ĺ��ߵĹ����汾�ͼ��ع��߲�һ��
*	- TDR_ERROR_DIFF_METALIB_HASH	Ԫ���ݿ��ɢ��ֵ��������ɢ��ֵ��һ��
*
*@pre \e a_ppstLib ����ΪNULL
*@pre \e a_pszBinFile ����Ϊ NULL
*@see tdr_free_lib
*/
TDR_API int tdr_load_verify_metalib(INOUT LPTDRMETALIB* a_ppstLib, IN const char* a_pszBinFile, IN const char *a_pszHash);


/**��Ԫ�������������Ƹ�ʽ�ļ��м���Ԫ���ݿ�
* @param[in,out] a_ppstLib ����Ԫ���ݿ��ָ���ָ��
* @param[in] a_pszBinFile ����Ԫ���ݿ�����Ƹ�ʽ���ļ����ļ���
* @note Ԫ���ݿ�Ŀռ��ں����ڶ�̬���䣬�����tdr_free_lib�ͷ�Ԫ���ݿ�Ŀռ�
*
*@retval 0   ����ɹ�
*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ������:
*	- TDR_ERROR_FAILED_OPEN_FILE_TO_READ	���ļ���ʧ��
*	- TDR_ERROR_INVALID_METALIB_FILE	Ԫ���ݿ��ļ����ֽ���Ϊ0�����ȡ���ֽ�����Ԫ����������ṹ�м�¼���ֽ�����ͬ
*	- TDR_ERROR_BUILD_VERSION_CONFLICT	TDR����Ԫ���ݿ��ļ��Ĺ��ߵĹ����汾�ͼ��ع��߲�һ��
*
*@pre \e a_ppstLib ����ΪNULL
*@pre \e a_pszBinFile ����Ϊ NULL
*@see tdr_free_lib
*/
TDR_API int tdr_load_metalib(INOUT LPTDRMETALIB* a_ppstLib, IN const char* a_pszBinFile);

/**��Ԫ�������������Ƹ�ʽ�м���Ԫ���ݿ�
* @param[in,out] a_ppstLib ����Ԫ���ݿ��ָ���ָ��
* @param[in] a_fpBin ����Ԫ���ݿ��Ԫ�������������Ƹ�ʽ���ļ����
* @note Ԫ���ݿ�Ŀռ��ں����ڶ�̬���䣬�����tdr_free_lib�ͷ�Ԫ���ݿ�Ŀռ�
*
*@retval 0   ����ɹ�
*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ������:
*	- TDR_ERROR_FAILED_OPEN_FILE_TO_READ	���ļ���ʧ��
*	- TDR_ERROR_INVALID_METALIB_FILE	Ԫ���ݿ��ļ����ֽ���Ϊ0�����ȡ���ֽ�����Ԫ����������ṹ�м�¼���ֽ�����ͬ
*	- TDR_ERROR_BUILD_VERSION_CONFLICT	TDR����Ԫ���ݿ��ļ��Ĺ��ߵĹ����汾�ͼ��ع��߲�һ��
*
*@pre \e a_ppstLib ����ΪNULL
*@pre \e a_fpBin ����Ϊ NULL
*@see tdr_free_lib
*/
TDR_API int tdr_load_metalib_fp(INOUT LPTDRMETALIB* a_ppstLib, IN FILE* a_fpBin);

/**��Ԫ���ݿⱣ�浽�����Ƹ�ʽ���ļ���
* @param[in,out] a_pstLib ����Ԫ���ݿ��ָ��
* @param[in] a_pszBinFile ����Ԫ���ݿ�Ķ����Ƹ�ʽ�ļ����ļ���
*
*@retval 0   ����ɹ�
*@retval <0  ����ʧ�ܣ����ܵĴ����������:
*	- TDR_ERROR_INVALID_PARAM	������Ч
	- TDR_ERRIMPLE_FAILED_OPEN_FILE_TO_WRITE failed to open file to write
	- TDR_ERROR_FAILED_TO_WRITE_FILE	failed to write data to file
*
*@pre \e a_pstLib ����ΪNULL
*@pre \e a_pszBinFile ����Ϊ NULL
*/
TDR_API int tdr_save_metalib(IN LPTDRMETALIB a_pstLib, IN const char* a_pszBinFile);

/**��Ԫ���ݿⱣ�浽�����Ƹ�ʽ���ļ���
* @param[in,out] a_pstLib ����Ԫ���ݿ��ָ��
* @param[in] a_fpBin ����Ԫ���ݿ�Ķ����Ƹ�ʽ���ļ����
*
@retval 0   ����ɹ�
*@retval <0  ����ʧ�ܣ����ܵĴ����������:
*	- TDR_ERROR_INVALID_PARAM	������Ч
	- TDR_ERROR_FAILED_TO_WRITE_FILE	failed to write data to file
*
*@pre \e a_pstLib ����ΪNULL
*@pre \e a_fpBin ����Ϊ NULL
*/
TDR_API int tdr_save_metalib_fp(IN LPTDRMETALIB a_pstLib, IN FILE* a_fpBin);

/**@} */ /*  �ļ�/�ļ����б���/�ָ�Ԫ���ݿ�*/

/**@defgroup  TDR_METALIB_DUMP TDR_Ԫ���ݿ⵼��(����ʱ��)
* @{
*/

/**��ӡԪ���ݿ������
*@param[in] a_pstLib Ҫ�����Ԫ���ݿ�
*@param[in] a_fp ����ļ����
@retval 0   ����ɹ�
*@retval <0  ����ʧ�ܣ����ܵĴ����������:
*	- TDR_ERROR_INVALID_PARAM	������Ч
*
*@pre \e a_pstLib ����ΪNULL
* @pre \e a_fp ����Ϊ NULL
*/
TDR_API void tdr_dump_metalib(IN LPTDRMETALIB a_pstLib, IN FILE* a_fp);

/**��ӡԪ���ݿ������
*�����ݱ�����a_pszFileָ�����ļ���
*@param[in] a_pstLib Ҫ�����Ԫ���ݿ�
*@param[in] a_pszFile ����ļ����ִ���ָ��
*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ������
*@retval 0   ����ɹ�
*@retval >0  ����ɹ���������ĳЩ�������
*
*@pre \e a_pstLib ����ΪNULL
* @pre \e a_pszFile ����Ϊ NULL
*/
TDR_API void tdr_dump_metalib_file(IN LPTDRMETALIB a_pstLib, IN const char* a_pszFile);

/**@} */ /*  TDR_METALIB_DUMP TDR_Ԫ���ݿ⵼��(����ʱ��)*/


/**@defgroup  TDR_DATA_IO TDR_������XML��ʽ����/���
* @{
*/

/**���ñ�tdr�ӿڴ�����ڴ����ݵ��ַ�������"GBK","UTF8"��
@param[in] a_pszEncoding ָ���ַ�����Ϣ���ַ����������׵�ַ
*@note ���ӿ��Ƿ��̰߳�ȫ�ģ�����ڶ��̻߳������ô˽ӿڣ������߱��뱣֤������ñ��ӿ�
*@note tdrȱʡ��Ϊ���ݵ��ַ���Ϊ"GBK"
*/
void tdr_set_encoding(IN const char *a_pszEncoding);

/**
*����Ԫ���������������԰�һ���ڴ����ݽṹ��ʽת����һ�������ض������xml��Ϣ����������ָ����������
*@param[in]	a_pstMeta Ԫ����������ָ�룬ͨ��tdr_get_meta_by_name��tdr_get_meta_by_id���Դ�Ԫ���ݿ��л�ȡ����ָ��
*@param[in,out]	a_pstXml �洢XML��ʽ��Ϣ�Ľṹ
*	- ����	a_pstXml.pszBuff ָ������XML��Ϣ�Ļ�������ʼ��ַ
*	- ����  a_pstXml.iBuff	ָ���������Ĵ�С
*	- ���  a_pstXml.iBuff  ʵ��ת�������Ϣ���ܳ���
*@param[in]	a_pstHost �洢�����ڴ����ݽṹ��Ϣ�Ľṹ��ָ��
*	- ����	a_pstHost.pszBuff ָ�����汾�ؽṹ��Ϣ�Ļ�������ʼ��ַ
*	- ����  a_pstHost.iBuff	ָ���������Ĵ�С
*@param[in] a_iCutOffVersion	Ҫ���õİ汾��
*@param[in] a_iXmlDataFormat XML�����ļ��ĸ�ʽ,Ŀǰ֧�ֵĸ�ʽ�У�
*	- TDR_XML_DATA_FORMAT_LIST_MATA_NAME	
*	- TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME 
*	-TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME
*	- �⼸�ָ�ʽ��������μ��ļ� TSF4G-DR-FAQ.doc ����xml�����ļ�����/�������
*
*
*@note ���a_iCutOffVersionΪ0 ����װ���õ���߰汾
*@note ���a_iXmlDataFormatָ����TDR_XML_DATA_FORMAT_LIST_MATA_NAME��TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME��
	TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME�����ֵ����ǿ�ư��ո�ʽTDR_XML_DATA_FORMAT_LIST_ENTRY_NAME���д���
*note ����xml�����ļ��ľ����ʽ˵������ο��ĵ�<TSF-G-DR-FAQ.doc> 
*�С�Xml�����ļ�����/���ƪ�� -> ��Tdr֧��ʲô��ʽ��xml�����ļ�����
*
*@retval 0   ����ɹ�
*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ�����룺
*	- TDR_ERROR_INVALID_PARAM	������Ч
*		- TDR_ERROR_INVALID_CUTOFF_VERSION ָ�����ð汾��Ԫ���ݻ�׼�汾ҪС
*		- TDR_ERROR_NO_HOSTBUFF_SPACE ���ش洢������ʣ��ռ䲻��
*		- TDR_ERROR_TOO_COMPLIEX_META Ԫ��������������������Ƕ�ײ�γ���32��
*		- TDR_ERROR_NET_UNSUPPORTED_TYPE    ��֧�ֵ�Ԫ��������
*		- TDR_ERROR_NET_INVALID_STRING_LEN string���͵�Ԫ�����ַ������ȳ�����Ԥ����󳤶�
*		- TDR_ERROR_NET_INVALID_VERSIONINDICATOR Ԫ���ݰ汾ָʾ����ֵ�ȱ�����ֵ��Ԫ���ݳ�Ա�İ汾ҪС
*
*
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*@pre \e a_pstMeta ����ΪNULL
*@pre \e a_pstXml ����ΪNULL
*@pre \e a_pstXml->pszBuff����ΪNULL
*@pre \e a_pstXml->iBuff �������0
*@pre \e a_pstHost ����ΪNULL
*@pre \e a_pstHost->pszBuff����ΪNULL
*@pre \e a_pstHost->iBuff �������0
*/
TDR_API int tdr_output(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstXml, IN LPTDRDATA a_pstHost,
			   IN int a_iCutOffVersion, IN int a_iXmlDataFormat);

/**
*����Ԫ���������������԰�һ���ڴ����ݽṹ��ʽת����һ�������ض������xml��Ϣ����������ָ���ļ���
*@param[in]	a_pstMeta Ԫ����������ָ�룬ͨ��tdr_get_meta_by_name��tdr_get_meta_by_id���Դ�Ԫ���ݿ��л�ȡ����ָ��
*@param[in]	a_fpXml �洢XML��ʽ��Ϣ���ļ����
*@param[in]	a_pstHost �洢�����ڴ����ݽṹ��Ϣ�Ľṹ��ָ��
*	- ����	a_pstHost.pszBuff ָ�����汾�ؽṹ��Ϣ�Ļ�������ʼ��ַ
*	- ����  a_pstHost.iBuff	ָ���������Ĵ�С
*@param[in] a_iCutOffVersion	Ҫ���õİ汾��
*@param[in] a_iXmlDataFormat XML�����ļ��ĸ�ʽ,Ŀǰ֧�ֵĸ�ʽ�У�
*	- TDR_XML_DATA_FORMAT_LIST_MATA_NAME	
*	- TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME 
*	-TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME
*	- �⼸�ָ�ʽ��������μ��ļ� TSF4G-DR-FAQ.doc ����xml�����ļ�����/�������
*
*
*@note ���a_iCutOffVersionΪ0 ����װ���õ���߰汾
*@note ���a_iXmlDataFormatָ����TDR_XML_DATA_FORMAT_LIST_MATA_NAME��TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME��
TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME�����ֵ����ǿ�ư��ո�ʽTDR_XML_DATA_FORMAT_LIST_ENTRY_NAME���д���
*
*note ����xml�����ļ��ľ����ʽ˵������ο��ĵ�<TSF-G-DR-FAQ.doc> 
*�С�Xml�����ļ�����/���ƪ�� -> ��Tdr֧��ʲô��ʽ��xml�����ļ�����
* @pre \e a_pstMeta ����Ϊ NULL
* @pre \e a_fpXml ����Ϊ NULL
*@pre \e a_pstHost ����ΪNULL
* @pre \e a_pstHost.pszBuff ����Ϊ NULL
* @pre \e a_pstHost.iBuff �������0	
*
*@retval 0   ����ɹ�
*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ�����룺
*	- TDR_ERROR_INVALID_PARAM	������Ч
*		- TDR_ERROR_INVALID_CUTOFF_VERSION ָ�����ð汾��Ԫ���ݻ�׼�汾ҪС
*		- TDR_ERROR_NO_HOSTBUFF_SPACE ���ش洢������ʣ��ռ䲻��
*		- TDR_ERROR_TOO_COMPLIEX_META Ԫ��������������������Ƕ�ײ�γ���32��
*		- TDR_ERROR_NET_UNSUPPORTED_TYPE    ��֧�ֵ�Ԫ��������
*		- TDR_ERROR_NET_INVALID_STRING_LEN string���͵�Ԫ�����ַ������ȳ�����Ԥ����󳤶�
*		- TDR_ERROR_NET_INVALID_VERSIONINDICATOR Ԫ���ݰ汾ָʾ����ֵ�ȱ�����ֵ��Ԫ���ݳ�Ա�İ汾ҪС
*
*
* @see tdr_output
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_output_fp(IN LPTDRMETA a_pstMeta, IN FILE *a_fpXml, IN LPTDRDATA a_pstHost,
			   IN int a_iCutOffVersion, IN int a_iXmlDataFormat);

/**
*����Ԫ���������������԰�һ���ڴ����ݽṹ��ʽת����һ�������ض������xml��Ϣ����������ָ���ļ���
*@param[in]	a_pstMeta Ԫ����������ָ�룬ͨ��tdr_get_meta_by_name��tdr_get_meta_by_id���Դ�Ԫ���ݿ��л�ȡ����ָ��
*@param[in]	a_szFile �洢XML��ʽ��Ϣ���ļ����ļ����ַ���ָ��
*@param[in]	a_pstHost �洢�����ڴ����ݽṹ��Ϣ�Ľṹ��ָ��
*	- ����	a_pstHost.pszBuff ָ�����汾�ؽṹ��Ϣ�Ļ�������ʼ��ַ
*	- ����  a_pstHost.iBuff	ָ���������Ĵ�С
*@param[in] a_iCutOffVersion	Ҫ���õİ汾��
*@param[in] a_iXmlDataFormat XML�����ļ��ĸ�ʽ,Ŀǰ֧�ֵĸ�ʽ�У�
*	- TDR_XML_DATA_FORMAT_LIST_MATA_NAME	
*	- TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME 
*	-TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME
*	- �⼸�ָ�ʽ��������μ��ļ� TSF4G-DR-FAQ.doc ����xml�����ļ�����/�������
*
*
*@note ���a_iCutOffVersionΪ0 ����װ���õ���߰汾
*@note ���a_iXmlDataFormatָ����TDR_XML_DATA_FORMAT_LIST_MATA_NAME��TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME��
TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME�����ֵ����ǿ�ư��ո�ʽTDR_XML_DATA_FORMAT_LIST_ENTRY_NAME���д���
*note ����xml�����ļ��ľ����ʽ˵������ο��ĵ�<TSF-G-DR-FAQ.doc> 
*�С�Xml�����ļ�����/���ƪ�� -> ��Tdr֧��ʲô��ʽ��xml�����ļ�����
*
*@pre \e a_pstMeta ����ΪNULL
*@pre \e a_szFile ����ΪNULL
*@pre \e a_pstHost ����ΪNULL
*@pre \e a_pstHost->pszBuff����ΪNULL
*@pre \e a_pstHost->iBuff ����ΪNULL
*
* @see tdr_output
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_output_file(IN LPTDRMETA a_pstMeta, IN const char *a_szFile, IN LPTDRDATA a_pstHost,
				  IN int a_iCutOffVersion, IN int a_iXmlDataFormat);

/**����Ԫ���������������ض������xml��Ϣת���ɱ����ڴ����ݽṹ��ʽ
*xml��Ϣ������ָ����������
*@param[in]	a_pstMeta Ԫ����������ָ�룬ͨ��tdr_get_meta_by_name��tdr_get_meta_by_id���Դ�Ԫ���ݿ��л�ȡ����ָ��
*@param[in,out]	a_pstHost �洢�����ڴ����ݽṹ��Ϣ�Ľṹ��ָ��
*	- ����	a_pstHost.pszBuff ָ�����汾�ؽṹ��Ϣ�Ļ�������ʼ��ַ
*	- ����  a_pstHost.iBuff	ָ���������Ĵ�С
*	- ���  a_pstHost.iBuff  ʵ�����ɵı��ؽṹ��Ϣ���ܳ���
*@param[in]	a_pstXml �洢XML��ʽ��Ϣ�Ľṹ
*	- ����	a_pstXml.pszBuff ָ������XML��Ϣ�Ļ�������ʼ��ַ
*	- ����  a_pstXml.iBuff	ָ���������Ĵ�С
*@param[in] a_iCutOffVersion	Ҫ���õİ汾��
*@param[in] a_iXmlDataFormat XML�����ļ��ĸ�ʽ,Ŀǰ֧�ֵĸ�ʽ�У�
*	- TDR_XML_DATA_FORMAT_LIST_MATA_NAME	
*	- TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME 
*	-TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME
*	- �⼸�ָ�ʽ��������μ��ļ� TSF4G-DR-FAQ.doc ����xml�����ļ�����/�������
*
*
*@note ���a_iCutOffVersionΪ0 ����װ���õ���߰汾
*@note ���a_iXmlDataFormatָ����TDR_XML_DATA_FORMAT_LIST_MATA_NAME��TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME��
TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME�����ֵ����ǿ�ư��ո�ʽTDR_XML_DATA_FORMAT_LIST_ENTRY_NAME���д���
*note ����xml�����ļ��ľ����ʽ˵������ο��ĵ�<TSF-G-DR-FAQ.doc> 
*�С�Xml�����ļ�����/���ƪ�� -> ��Tdr֧��ʲô��ʽ��xml�����ļ�����
*@note ���ӿڲ�֧��versionindicator����
*@note ���ӿڲ�֧�� ָ������� ����
*
* @pre \e a_pstMeta ����Ϊ NULL
* @pre \e a_pstXml ����Ϊ NULL
* @pre \e a_pstXml.pszBuff ����Ϊ NULL
* @pre \e a_pstXml.iBuff �������0
* @pre \e a_pstXml ����Ϊ NULL
* @pre \e a_pstHost.pszBuff ����Ϊ NULL
* @pre \e a_pstHost.iBuff �������0	
*
*@retval 0   ����ɹ�
*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ�����룺
*	- TDR_ERROR_INVALID_PARAM	������Ч
*	-	TDR_ERROR_NO_XML_ROOT	û�к��ʵĸ��ڵ㣬��ȷ��XMLԪ�����а�����Ԫ������Ϊ���Ľڵ�
*
* @see  tdr_output
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_input(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost, IN LPTDRDATA a_pstXml, 
			   IN int a_iCutOffVersion, IN int a_iXmlDataFormat);

/**����Ԫ���������������ض������xml��Ϣת���ɱ����ڴ����ݽṹ��ʽ
*xml��Ϣ������ָ���ļ���
*@param[in]	a_pstMeta Ԫ����������ָ�룬ͨ��tdr_get_meta_by_name��tdr_get_meta_by_id���Դ�Ԫ���ݿ��л�ȡ����ָ��
*@param[in,out]	a_pstHost �洢�����ڴ����ݽṹ��Ϣ�Ľṹ��ָ��
*	- ����	a_pstHost.pszBuff ָ�����汾�ؽṹ��Ϣ�Ļ�������ʼ��ַ
*	- ����  a_pstHost.iBuff	ָ���������Ĵ�С
*	- ���  a_pstHost.iBuff  ʵ�����ɵı��ؽṹ��Ϣ���ܳ���
*@param[in]	a_fp �洢XML��ʽ��Ϣ���ļ����
*@param[in] a_iCutOffVersion	Ҫ���õİ汾��
*@param[in] a_iXmlDataFormat XML�����ļ��ĸ�ʽ,Ŀǰ֧�ֵĸ�ʽ�У�
*	- TDR_XML_DATA_FORMAT_LIST_MATA_NAME	
*	- TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME 
*	-TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME
*	- �⼸�ָ�ʽ��������μ��ļ� TSF4G-DR-FAQ.doc ����xml�����ļ�����/�������
*
*
*@note ���a_iCutOffVersionΪ0 ����װ���õ���߰汾
*@note ���a_iXmlDataFormatָ����TDR_XML_DATA_FORMAT_LIST_MATA_NAME��TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME��
TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME�����ֵ����ǿ�ư��ո�ʽTDR_XML_DATA_FORMAT_LIST_ENTRY_NAME���д���
*note ����xml�����ļ��ľ����ʽ˵������ο��ĵ�<TSF-G-DR-FAQ.doc> 
*�С�Xml�����ļ�����/���ƪ�� -> ��Tdr֧��ʲô��ʽ��xml�����ļ�����
*
*@note ���ӿڲ�֧��versionindicator���ԣ�
*@note ���ӿڲ�֧�� ָ������� ����

* @pre \e a_pstMeta ����Ϊ NULL
* @pre \e a_fp ����Ϊ NULL
* @pre \e a_psHost ����ΪNULL
* @pre \e a_pstHost.pszBuff ����Ϊ NULL
* @pre \e a_pstHost.iBuff �������0	
*
*@retval 0   ����ɹ�
*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ�����룺
*	-	TDR_ERROR_NO_XML_ROOT	û�к��ʵĸ��ڵ㣬��ȷ��XMLԪ�����а�����Ԫ������Ϊ���Ľڵ�
*
* @see  tdr_output
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_input_fp(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost, IN FILE *a_fp, 
			  IN int a_iCutOffVersion, IN int a_iXmlDataFormat);

/**����Ԫ���������������ض������xml��Ϣת���ɱ����ڴ����ݽṹ��ʽ
*xml��Ϣ������ָ���ļ���
*@param[in]	a_pstMeta Ԫ����������ָ�룬ͨ��tdr_get_meta_by_name��tdr_get_meta_by_id���Դ�Ԫ���ݿ��л�ȡ����ָ��
*@param[in,out]	a_pstHost �洢�����ڴ����ݽṹ��Ϣ�Ľṹ��ָ��
*	- ����	a_pstHost.pszBuff ָ�����汾�ؽṹ��Ϣ�Ļ�������ʼ��ַ
*	- ����  a_pstHost.iBuff	ָ���������Ĵ�С
*	- ���  a_pstHost.iBuff  ʵ�����ɵı��ؽṹ��Ϣ���ܳ���
*@param[in]	a_pszFile �洢XML��ʽ��Ϣ���ļ����ļ����ַ���ָ��
*@param[in] a_iCutOffVersion	Ҫ���õİ汾��
*@param[in] a_iXmlDataFormat XML�����ļ��ĸ�ʽ,Ŀǰ֧�ֵĸ�ʽ�У�
*	- TDR_XML_DATA_FORMAT_LIST_MATA_NAME	
*	- TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME 
*	-TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME
*	- �⼸�ָ�ʽ��������μ��ļ� TSF4G-DR-FAQ.doc ����xml�����ļ�����/�������
*
*
*@note ���a_iCutOffVersionΪ0 ����װ���õ���߰汾
*@note ���a_iXmlDataFormatָ����TDR_XML_DATA_FORMAT_LIST_MATA_NAME��TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME��
TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME�����ֵ����ǿ�ư��ո�ʽTDR_XML_DATA_FORMAT_LIST_ENTRY_NAME���д���
*note ����xml�����ļ��ľ����ʽ˵������ο��ĵ�<TSF-G-DR-FAQ.doc> 
*�С�Xml�����ļ�����/���ƪ�� -> ��Tdr֧��ʲô��ʽ��xml�����ļ�����
*@note ���ӿڲ�֧��versionindicator����
*@note ���ӿڲ�֧�� ָ������� ����
*
* @pre \e a_pstMeta ����Ϊ NULL
* @pre \e a_pszFile ����Ϊ NULL
* @pre \e a_psHost ����ΪNULL
* @pre \e a_pstHost.pszBuff ����Ϊ NULL
* @pre \e a_pstHost.iBuff �������0	
*
*@retval 0   ����ɹ�
*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ�����룺
*	- TDR_ERROR_INVALID_PARAM	������Ч
*	-	TDR_ERROR_NO_XML_ROOT	û�к��ʵĸ��ڵ㣬��ȷ��XMLԪ�����а�����Ԫ������Ϊ���Ľڵ�
*
* @see  tdr_output
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_input_file(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost, IN const char *a_pszFile, 
				   IN int a_iCutOffVersion, IN int a_iXmlDataFormat);
/**@}*/ /*defgroup  TDR_DATA_IO TDR_������XML��ʽ����/��� */

/**@defgroup  TDR_PRINT_DATA TDR_���ݴ�ӡ
* @{
*/

/**����Ԫ���������������ڴ����ݽṹ�Կ��ӻ��ķ�ʽ���
*@param[in]	a_pstMeta Ԫ����������ָ�룬ͨ��tdr_get_meta_by_name��tdr_get_meta_by_id���Դ�Ԫ���ݿ��л�ȡ����ָ��
*@param[in]	a_fp �洢���ӻ�������Ϣ���ļ����
*@param[in]	a_pstHost �洢�����ڴ����ݽṹ��Ϣ�Ľṹ��ָ��
*	- ����	a_pstHost.pszBuff ָ�����汾�ؽṹ��Ϣ�Ļ�������ʼ��ַ
*	- ����  a_pstHost.iBuff	ָ���������Ĵ�С
*@param[in] a_iCutOffVersion	Ҫ���õİ汾��
*@note ���a_iCutOffVersionΪ0 ����װ���õ���߰汾
*
* @pre \e a_pstMeta ����Ϊ NULL
* @pre \e a_fp ����Ϊ NULL
* @pre \e a_psHost ����ΪNULL
* @pre \e a_pstHost.pszBuff ����Ϊ NULL
* @pre \e a_pstHost.iBuff �������0	
*
*@retval 0   ����ɹ�
*@retval >0  ����ɹ���������ĳЩ�������
*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ������
*
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_fprintf(IN LPTDRMETA a_pstMeta, IN FILE  *a_fp, INOUT LPTDRDATA a_pstHost, IN int a_iCutOffVersion);

/**����Ԫ���������������ڴ����ݽṹ�Կ��ӻ��ķ�ʽ������ַ�����������
*@param[in]	a_pstMeta Ԫ����������ָ�룬ͨ��tdr_get_meta_by_name��tdr_get_meta_by_id���Դ�Ԫ���ݿ��л�ȡ����ָ��
*@param[in,out]	a_pstOut �洢���ӻ����ݵĽṹ��ָ��
*	- ����	a_pstOut.pszBuff ָ������洢���ӻ����ݵ���Ϣ�Ļ�������ʼ��ַ
*	- ����  a_pstOut.iBuff	ָ���������Ĵ�С
*	- ���  a_pstOut.iBuff  ʵ�����ɵĴ洢���ӻ����ݵ���Ϣ���ܳ���
*@param[in]	a_pstHost �洢�����ڴ����ݽṹ��Ϣ�Ľṹ��ָ��
*	- ����	a_pstHost.pszBuff ָ�����汾�ؽṹ��Ϣ�Ļ�������ʼ��ַ
*	- ����  a_pstHost.iBuff	ָ���������Ĵ�С
*@param[in] a_iCutOffVersion	Ҫ���õİ汾��
*@note ���a_iCutOffVersionΪ0 ����װ���õ���߰汾
*
* @pre \e a_pstMeta ����Ϊ NULL
* @pre \e a_pstHost ����Ϊ NULL
* @pre \e a_pstHost.pszBuff ����Ϊ NULL
* @pre \e a_pstHost.iBuff �������0	
* @pre \e a_pstOut ����Ϊ NULL
* @pre \e a_pstOut.pszBuff ����Ϊ NULL
* @pre \e a_pstOut.iBuff �������0
*
*@retval 0   ����ɹ�
*@retval >0  ����ɹ���������ĳЩ�������
*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ������
*
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_sprintf(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstOut, IN LPTDRDATA a_pstHost, IN int a_iCutOffVersion);

/**@}*/ /* @defgroup  TDR_PRINT_DATA TDR_���ݴ�ӡ*/

#ifdef __cplusplus
}
#endif

#endif /* TDR_DATA_IO_H */
