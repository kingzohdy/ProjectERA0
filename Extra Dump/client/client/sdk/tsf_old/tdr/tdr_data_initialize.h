/**
*
* @file     tdr_data_initialize.h 
* @brief    TDR���ݳ�ʼ��/����
* 
* @author steve jackyai  
* @version 1.0
* @date 2007-06-05 
*
*
* Copyright (c)  2007, ��Ѷ�Ƽ����޹�˾���������з���
* All rights reserved.
*
*/
#ifndef TDR_DATA_INITIALIZE_H
#define TDR_DATA_INITIALIZE_H


#include "tdr/tdr_types.h"



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @defgroup TDR_DATA_INIT TDR_���ݳ�ʼ��
*  @{
*/

/** 
*@brief ���ݳ�ʼ��
*����Ԫ���ݵ�ȱʡֵԼ�����ڴ�ṹ����Ӧ��Ա����ʼֵ
*@param[in]	a_pstMeta Ԫ����������ָ�룬ͨ��tdr_get_meta_by_name��tdr_get_meta_by_id���Դ�Ԫ���ݿ��л�ȡ����ָ��
*@param[in]	a_pstHost �洢���ؽṹ���ݵĽṹ��ָ��
*	- ����	a_pstHost.pszBuff ָ�����汾�ؽṹ���ݵĻ�������ʼ��ַ
*	- ����  a_pstHost.iBuff	ָ���������Ĵ�С
*@param[in] a_iVersion	Ҫ���õİ汾��
*
*@note ���a_pstMeta������union���͵Ľṹ,�򲻽��д���ֱ�ӷ���
*@note ���a_iVersionΪ0 ����װ���õ���߰汾
*
* @pre \e a_pstHost.pszBuff�������б����ǰ���a_pstMeta����������
* @pre \e a_pstMeta ����Ϊ NULL
* @pre \e a_pstHost ����Ϊ NULL
* @pre \e a_pstHost.pszBuff ����Ϊ NULL
* @pre \e a_pstHost.iBuff �������0	
*
*@retval 0   ����ɹ�
*@retval >0  ����ɹ���������ĳЩ�������
*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ������	
*
*
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
int tdr_init(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost, IN int a_iVersion);


/** @}*/ /* TDR_DATA_INIT TDR���ݳ�ʼ��*/


#ifdef __cplusplus
}
#endif

#endif /* TDR_DATA_INITIALIZE_H */
