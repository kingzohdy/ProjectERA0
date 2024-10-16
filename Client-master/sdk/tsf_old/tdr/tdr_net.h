/**
*
* @file     tdr_net.h 
* @brief    TDRԪ�������罻����Ϣ�����ģ��
* 
* @author steve jackyai  
* @version 1.0
* @date 2007-04-28 
*
*
* Copyright (c)  2007, ��Ѷ�Ƽ����޹�˾���������з���
* All rights reserved.
*
*/



#ifndef TSF_G_TDR_NET_H
#define TSF_G_TDR_NET_H


#include "tdr/tdr_types.h"
#include "tdr/tdr_external.h"



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @defgroup TDR_NETMSG_PACKUNPACK TDR_���罻����Ϣ��/����
* @{
*/

/**
	*����ָ���汾����Ԫ�������������ش洢����ת�������罻����Ϣ
	*@param[in]	a_pstMeta Ԫ����������ָ�룬ͨ��tdr_get_meta_by_name��tdr_get_meta_by_id���Դ�Ԫ���ݿ��л�ȡ����ָ��
	*@param[in,out]	a_pstNet �洢������Ϣ�ṹ��ָ��
	*	- ����	a_pstNet.pszBuff ָ������������Ϣ�Ļ�������ʼ��ַ
	*	- ����  a_pstNet.iBuff	ָ���������Ĵ�С
	*	- ���  a_pstNet.iBuff  ʵ�ʱ���������Ϣ���ܳ���
	*@param[in,out]	a_pstHost �洢���ؽṹ��Ϣ�Ľṹ��ָ��
	*	- ����	a_pstHost.pszBuff ָ�����汾�ؽṹ��Ϣ�Ļ�������ʼ��ַ
	*	- ����  a_pstHost.iBuff	ָ���������Ĵ�С
	*	- ���  a_pstHost.iBuff  ʵ�ʱ��뱾�ؽṹ��Ϣ���ܳ���
	*@param[in] a_iVersion	Ҫ���õİ汾��
	*
	*@note ���a_iVersionΪ0 ����װ���õ���߰汾
	*@note �������ʧ�ܣ���ȡ������Ϣ�ķ���:
	*	- 1.���ݷ���ֵ������tdr_error_string()���Ի�ȡ������Ϣ
	*	- 2.����a_pstHost.iBuff�õ���ʵ�ʱ������Ϣ���ܳ��ȣ����������Ϊƫ��ֵ������tdr_entry_off_to_path���Ի�ȡ
	*		����ʱ��Ա��·����Ϣ���Ӷ���λ����������ĸ�λ�ó���
	*
	* @pre \e a_pstMeta ����Ϊ NULL
	* @pre \e a_pstNet ����Ϊ NULL
	* @pre \e a_pstNet.pszBuff ����Ϊ NULL
	* @pre \e a_pstNet.iBuff �������0
	* @pre \e a_pstHost.pszBuff ����Ϊ NULL
	* @pre \e a_pstHost.iBuff �������0	
	*
	*@retval 0   ����ɹ�
	*@retval >0  ����ɹ���������ĳЩ�������
	*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ�����룺
	*		- TDR_ERROR_INVALID_CUTOFF_VERSION ָ�����ð汾��Ԫ���ݻ�׼�汾ҪС
	*		- TDR_ERR_NET_NO_NETBUFF_SPACE ����������Ϣ�Ļ�����ʣ��ռ䲻��
	*		- TDR_ERROR_NO_HOSTBUFF_SPACE ���ش洢������ʣ��ռ䲻��
	*		- TDR_ERROR_INVALID_REFER_VALUE		��Ԫ�ص�refer����ֵ����ȷ����ֵ����Ϊ�����ұ����count����ֵС
	*		- TDR_ERROR_TOO_COMPLIEX_META Ԫ��������������������Ƕ�ײ�γ���32��
	*		- TDR_ERROR_NET_UNSUPPORTED_TYPE    ��֧�ֵ�Ԫ��������
	*		- TDR_ERROR_NET_INVALID_STRING_LEN string���͵�Ԫ�����ַ������ȳ�����Ԥ����󳤶�
	*		- TDR_ERROR_NET_INVALID_VERSIONINDICATOR Ԫ���ݰ汾ָʾ����ֵ�ȱ�����ֵ��Ԫ���ݳ�Ա�İ汾ҪС
	*
	*
	* @see	tdr_get_meta_by_name
	* @see	tdr_get_meta_by_id
	* @see	tdr_error_string
	* @see	tdr_entry_off_to_path
	*/
TDR_API int tdr_hton(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstNet, INOUT LPTDRDATA a_pstHost, IN int a_iVersion);

/**
*����ָ���汾����Ԫ�������������罻����Ϣת���ɱ��ش洢����
*@param[in]	a_pstMeta Ԫ����������ָ�룬ͨ��tdr_get_meta_by_name��tdr_get_meta_by_id���Դ�Ԫ���ݿ��л�ȡ����ָ��
*@param[in,out]	a_pstHost �洢���ؽṹ��Ϣ�Ľṹ��ָ��
*	- ����	a_pstHost.pszBuff ָ�����汾�ؽṹ��Ϣ�Ļ�������ʼ��ַ
*	- ����  a_pstHost.iBuff	ָ���������Ĵ�С
*	- ���  a_pstHost.iBuff  ʵ�ʽ��뱾�ؽṹ��Ϣ���ܳ���
*@param[in,out]	a_pstNet �洢������Ϣ�ṹ��ָ��
*	- ����	a_pstNet.pszBuff ָ������������Ϣ�Ļ�������ʼ��ַ
*	- ����  a_pstNet.iBuff	ָ���������Ĵ�С
*	- ���  a_pstNet.iBuff  ʵ�ʽ���������Ϣ���ܳ���
*@param[in] a_iVersion	Ҫ���õİ汾��
*
*@note ���a_iVersionΪ0 ����װ���õ���߰汾
*@note �������ʧ�ܣ���ȡ������Ϣ�ķ���:
	*	- ���ݷ���ֵ������tdr_error_string()���Ի�ȡ������Ϣ
	*	- ����a_pstHost.iBuff�õ���ʵ�ʽ������Ϣ���ܳ��ȣ����������Ϊƫ��ֵ������tdr_entry_off_to_path���Ի�ȡ
	*		����ʱ��Ա��·����Ϣ���Ӷ���λ����������ĸ�λ�ó���
	*	- ����tdr_fprintf/tdr_sprintf���Դ�ӡ���Ѿ����������
*
* @pre \e a_pstMeta ����Ϊ NULL
* @pre \e a_pstNet ����Ϊ NULL
* @pre \e a_pstNet.pszBuff ����Ϊ NULL
* @pre \e a_pstNet.iBuff �������0
* @pre \e a_pstHost.pszBuff ����Ϊ NULL
* @pre \e a_pstHost.iBuff �������0	
*
*@retval 0   ����ɹ�
*@retval >0  ����ɹ���������ĳЩ�������
*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ�����룺
*		- TDR_ERROR_INVALID_CUTOFF_VERSION ָ�����ð汾��Ԫ���ݻ�׼�汾ҪС
*		- TDR_ERR_NET_NO_NETBUFF_SPACE ����������Ϣ�Ļ�����ʣ��ռ䲻��
*		- TDR_ERROR_NO_HOSTBUFF_SPACE ���ش洢������ʣ��ռ䲻��
*		- TDR_ERROR_INVALID_REFER_VALUE		��Ԫ�ص�refer����ֵ����ȷ����ֵ����Ϊ�����ұ����count����ֵС
*		- TDR_ERROR_TOO_COMPLIEX_META Ԫ��������������������Ƕ�ײ�γ���32��
*		- TDR_ERROR_NET_UNSUPPORTED_TYPE    ��֧�ֵ�Ԫ��������
*		- TDR_ERROR_NET_INVALID_STRING_LEN string���͵�Ԫ�����ַ������ȳ�����Ԥ����󳤶�
*
*
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
* @see	tdr_error_string
* @see	tdr_entry_off_to_path
* @see  tdr_fprintf
*/
TDR_API int tdr_ntoh(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost, INOUT LPTDRDATA a_pstNet, IN int a_iVersion);

/** @} */ /*@defgroup TDR_���罻����Ϣ��/����*/



#ifdef __cplusplus
}
#endif

#endif /* TSF_G_TDR_NET_H */
