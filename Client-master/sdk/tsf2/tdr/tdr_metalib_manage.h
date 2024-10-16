 /**
*
* @file     tdr_metalib_manage.h 
* @brief    Ԫ���ݿ�������ģ��
* 
* @author steve jackyai  
* @version 1.0
* @date 2007-04-02 
*
*
* Copyright (c)  2007, ��Ѷ�Ƽ����޹�˾���������з���
* All rights reserved.
*
*/

#ifndef TDR_METALIB_MANAGE_H
#define TDR_METALIB_MANAGE_H


#include "tdr/tdr_types.h"
#include "tdr/tdr_external.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

	
/** @defgroup TDR_META_MANAGE TDR_Ԫ����(meta)����
*  @{
*/


/** ����Ԫ��������Ԫ���ݿ��в��Ҵ�Ԫ����
*@param[in] a_pstLib Ԫ���ݿ�ָ��
*@param[in] a_pszName Ԫ�������ִ�ָ��
*
*@return ����ҵ��򷵻�Ԫ���ݵĳ�Ա�����򷵻�NULL
*
* @pre \e a_pstLib ����Ϊ NULL
* @pre \e a_pszName ����Ϊ NULL
*/
TDR_API LPTDRMETA tdr_get_meta_by_name(IN LPTDRMETALIB a_pstLib, IN const char* a_pszName);

/**����Ԫ���ݵ�ID��Ԫ���ݿ��в��Ҵ�Ԫ����
*@param[in] a_pstLib Ԫ���ݿ�ָ��
*@param[in] a_iID Ԫ���ݵ�ID
*
*@return ����ҵ��򷵻�Ԫ���ݵĳ�Ա�����򷵻�NULL
*
* @pre \e a_pstLib ����Ϊ NULL
*/
TDR_API LPTDRMETA tdr_get_meta_by_id(IN LPTDRMETALIB a_pstLib, IN int a_iID);


/** ��ȡԪ���������Ļ�׼�汾��
*@param[in] a_pstMeta Ԫ����������ָ��
*
*@return success: Ԫ���������Ļ�׼�汾�� failed: -1
*
* @pre \e a_pstMeta ����Ϊ NULL
*/
TDR_API int tdr_get_meta_based_version(IN LPTDRMETA a_pstMeta);



/**
*��ȡԪ���������ĵ�ǰ�汾��
*@param[in] a_pstMeta Ԫ����������ָ��
*
*@return success:Ԫ���������ĵ�ǰ�汾�� failed: -1
*
* @pre \e a_pstMeta ����Ϊ NULL
*/
TDR_API int tdr_get_meta_current_version(IN LPTDRMETA a_pstMeta);

/**
*��ȡԪ���������Ļ�����������ID
*@param[in] a_pstMeta Ԫ����������ָ��
*
*@return ��������ID: TDR_TYPE_UNION �� TDR_TYPE_STRUCT
*
* @pre \e a_pstMeta ����Ϊ NULL
*/
TDR_API int tdr_get_meta_type(IN LPTDRMETA a_pstMeta);

/**��ȡԪ��������������
*@param[in] a_pstMeta Ԫ����������ָ��
*
*@return Ԫ���������ַ�����ָ��
*
* @pre \e a_pstMeta ����Ϊ NULL
*/
TDR_API const char *tdr_get_meta_name(IN LPTDRMETA a_pstMeta);



/**
*��ȡ����Ԫ��������(meta)�Ĵ洢�ռ�
*@param[in] a_pstMeta Ԫ����������ָ��
*
*@return success: ����Ԫ��������(meta)�Ĵ洢���ֽ��� failed : -1
*
* @pre \e a_pstMeta ����Ϊ NULL
*/
TDR_API int tdr_get_meta_size(IN LPTDRMETA a_pstMeta);

/**
*��ȡ����Ԫ�������ͳ�ԱԪ�صĸ���
*@param[in] a_pstMeta Ԫ����������ָ��
*
*@return ���Idֵ
*
* @pre \e a_pstMeta ����Ϊ NULL
*/
TDR_API int tdr_get_entry_num(IN LPTDRMETA a_pstMeta);


/**���ݳ�Ա�ڽṹ���е�·��(Path)��Ϣ��ȡ�˳�Ա���ڽṹ���׵�ַ��ƫ��ֵ
*@param[in] a_pstMeta ��Ҫ��λ�Ľṹ��Ԫ�����������
*@param[in,out] a_ppstEntry �����Ӧ��Ա�����������ָ��
*@param[out] a_piHOff �����Ӧ��Ա��ƫ��ֵ
*@param[in] a_pszPath  ����˳�Ա·����Ϣ�Ļ�����
*
*return  �ɹ�����0�����򷵻ظ���
*
*@pre \e a_pszBuff ����ΪNULL
*@pre \e a_pstMeta ����ΪNULL
*@pre \e a_iBuff �������0
*@pre \e a_iOff ������ڵ���0
*@pre \e a_pszPath ����ΪNULL
*/
TDR_API int tdr_entry_path_to_off(IN LPTDRMETA a_pstMeta, INOUT LPTDRMETAENTRY *a_ppstEntry, OUT int *a_piHOff, IN const char *a_pszPath);


/**���ݳ�Ա�ڽṹ���е�ƫ�ƶ�λ�˳�Ա�ڽṹ�е�·����Ϣ
*@param[in] a_pstMeta ��Ҫ��λ�Ľṹ��Ԫ�����������
*@param[in] a_iOff  ��Ա��ƫ��
*
*@note ·����Ϣ�������ھ�̬�������У��ٴε��ñ��ӿ�ʱ���ϴε��û�ȡ��·����Ϣ������д
*
*return  �ɹ�����0�����򷵻ظ���
*
*@pre \e a_pstMeta ����ΪNULL
*@pre \e a_iOff ������ڵ���0
*/
TDR_API char *tdr_entry_off_to_path(IN LPTDRMETA a_pstMeta, IN int a_iOff);


/**��ѯ�˽ṹ���Ƿ�����������������Եĳ�Ա
*@param[in] a_pstMeta �ṹ���Ա��Ԫ�����������
*
*@pre \e a_pstMeta ����ΪNULL
*
*@return ������������Գ�Ա���򷵻ط���ֵ�����򷵻�0
*/
TDR_API int tdr_do_have_autoincrement_entry(IN LPTDRMETA a_pstMeta);

/** @}*/ /* TDR_META_MANAGE TDR_Ԫ����(meta)����*/


/** @defgroup TDR_ENTRY_MANAGE TDR_���ݳ�Ա(entry)����
*  @{
*/

/**��ȡԪ��������������
*@param[in] a_pstEntry Ԫ����������ָ��
*
*@return Ԫ���������ַ�����ָ��
*
* @pre \e a_pstEntry ����Ϊ NULL
*/
TDR_API const char *tdr_get_entry_name(IN LPTDRMETAENTRY a_pstEntry);


/**����Ԫ���ݳ�Ա����Ԫ���ݿ��в��Ҵ˳�Ա�ľ��
*@param[in] a_pstMeta ������Ԫ���ݵ�ָ��
*@param[in] a_pszName entry������
*
*@return �������ָ�����ֵĳ�Ա���򷵻ش˳�Ա�ľ�������򷵻�NULL
*
*@pre \e a_pstMeta ����ΪNULL
*@pre \e a_pszName ����Ϊ NULL
*/
TDR_API LPTDRMETAENTRY tdr_get_entryptr_by_name(IN LPTDRMETA a_pstMeta, IN const char* a_pszName);

/**����Ԫ���ݳ�Ա����Ԫ���ݿ��в��Ҵ˳�Ա�����ڸ�Ԫ���ݳ�Ա�����е��±���ʽ���أ�
*@param[out] a_piIdx ����˳�Ա������
*@param[in] a_pstMeta ������Ԫ���ݵ�ָ��
*@param[in] a_pszName Ԫ�������ִ�ָ��
*
*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ������
*@retval 0   ����ɹ�
*@retval >0  ����ɹ���������ĳЩ�������
*
* @pre \e a_piIdx ����Ϊ NULL
*@pre \e a_pstMeta ����ΪNULL
* @pre \e a_pszName ����Ϊ NULL
*/
TDR_API int tdr_get_entry_by_name(OUT int* a_piIdx, IN LPTDRMETA a_pstMeta, IN const char* a_pszName);

/**����Ԫ���ݳ�Ա��ID��Ԫ���ݿ��в��Ҵ˳�Ա�����ڸ�Ԫ���ݳ�Ա�����е��±���ʽ���أ�
*���Ԫ������union�ṹ�������a_iId��ĳ��Ա[minid, maxid]����֮�䣬��ѡ��˳�Ա������
*ѡ��ȱʡ��Ա��������ڵĻ���
*@param[out] a_piIdx ����˳�Ա������
*@param[in] a_pstMeta ������Ԫ���ݵ�ָ��
*@param[in] a_iId Ԫ���ݵ�ID
*
*@retval <0  ����ʧ�ܣ����ر�ʾ������Ϣ�Ĵ������
*@retval 0   ����ɹ�
*
* @pre \e a_piIdx ����Ϊ NULL
*@pre \e a_pstMeta ����ΪNULL
*/
TDR_API int tdr_get_entry_by_id(OUT int* a_piIdx, IN LPTDRMETA a_pstMeta, IN int a_iId);

/**����Ԫ���ݳ�Ա��Ԫ���ݽṹ�е�������ȡ�˳�Ա�����ľ��
*@param[in] a_pstMeta ������Ԫ���ݵ�ָ��
*@param[in] a_idxEntry entry������
*
*@return ���������ȷ,�򷵻ش˳�Ա�����ľ��,���򷵻�NULL
*
*@pre \e a_pstMeta ����ΪNULL
*@see tdr_get_meta_by_id
*@see tdr_get_entry_by_id
*/
TDR_API LPTDRMETAENTRY tdr_get_entry_by_index(IN LPTDRMETA a_pstMeta, IN int a_idxEntry);

/**��ȡentryԪ��id����ֵ
*@param[in] a_pstEntry entryԪ�������ṹ��ָ��
*
*@note ͨ������tdr_get_entry_by_index���Ի�ȡentryԪ�������ṹ��ָ��
*
*@return entryԪ��id����ֵ
*
*@pre \e a_pstEntry ����ΪNULL
*@see tdr_get_entry_by_index
*/
TDR_API int tdr_get_entry_id(IN LPTDRMETAENTRY a_pstEntry);

/**��ȡentryԪ��id����ֵ��Ӧ�ĺ궨�������
*@param[in] a_pstEntry entryԪ�������ṹ��ָ��
*@param[in] a_pstLib Ԫ�����������ָ��
*
*@note ͨ������tdr_get_entry_by_index���Ի�ȡentryԪ�������ṹ��ָ��
*
*@return ����˳�Ա��id����ֵΪ�궨��,�򷵻ش˺궨�������;���򷵻ؿմ�""
*
*@pre \e a_pstEntry ����ΪNULL
*@pre \e a_pstLib ����ΪNULL
*@see tdr_get_entry_by_index
*/
TDR_API const char *tdr_get_entry_id_name(IN LPTDRMETALIB a_pstLib, IN LPTDRMETAENTRY a_pstEntry);


/**��ȡ����entry��Ա�ı��ش洢�ռ��С
*@param[in] a_pstEntry entryԪ�������ṹ��ָ��
*
*@note ͨ������tdr_get_entry_by_index���Ի�ȡentryԪ�������ṹ��ָ��
*
*@return entryԪ�ص��������ı��ش洢�ռ��С
*
*@pre \e a_pstEntry ����ΪNULL
*@see tdr_get_entry_by_index
*/
TDR_API int tdr_get_entry_unitsize(IN LPTDRMETAENTRY a_pstEntry);

/**��ȡentry����������ID
*@param[in] a_pstEntry entryԪ�������ṹ��ָ��
*
*
*@return ���س�Ա����������ID������TDR֧�ֵ�����ID������μ�tdr_ctypes_info.h��"������������"
*
*/
TDR_API int tdr_get_entry_type(IN LPTDRMETAENTRY a_pstEntry);

/**��ȡentry�����������Ϣ
*@param[in] a_pstEntry entryԪ�������ṹ��ָ��
*
*
*@retval 0	��ʾ�˳�ԱΪ�ɱ䳤������
*@retval 1	��ʾ�˳�Ա��������
*@retval >1 ��ʾ�˳�ԱΪ�̶����ȵ�����
*
*/
TDR_API int tdr_get_entry_count(IN LPTDRMETAENTRY a_pstEntry);

/**��ȡentryԪ����ָmeta�ṹ������ָ��
* ���entryԪ��Ϊ������������ʱ,ͨ���˽ӿڿ��Ի�ȡ��entry�����������͵������ṹ��ָ��
*@param[in] a_pstLib	Ԫ�����������ָ��
*@param[in] a_pstEntry entryԪ�������ṹ��ָ��
*
*
*@return ���entryԪ��Ϊ������������ʱ,������ָ������������������ָ��;���򷵻�NULL
*
*/
TDR_API LPTDRMETA tdr_get_entry_type_meta(IN LPTDRMETALIB a_pstLib, IN LPTDRMETAENTRY a_pstEntry);

/**����Ԫ���ݳ�Ա����·����Ԫ���ݿ��в��Ҵ˳�Ա
*@param[in] a_pstMeta ������Ԫ���ݵ�ָ��
*@param[in] a_pszEntryPath entry�ڽṹ�е�����·����
*
*@return �������ָ�����ֵĳ�Ա���򷵻ش˳�Ա�ľ�������򷵻�NULL
*
*@pre \e a_pstMeta ����ΪNULL
*@pre \e a_pszEntryPath ����Ϊ NULL*/
TDR_API LPTDRMETAENTRY tdr_get_entry_by_path(IN LPTDRMETA a_pstMeta, IN const char* a_pszEntryPath);

/**��ȡ��Ա���Զ�������ֵ
*@param[in] a_pstEntry ��Ա��Ԫ�����������
*@param[in] a_pstLib	Ԫ�����������ָ��
*
*@return ����˳�Ա������customattr����,�򷵻ش����Ե��׵�ַ;���򷵻�""
*
*@pre \e a_pstEntry ����ΪNULL
*@pre \e a_pstLib ����ΪNULL
*/
TDR_API const char *tdr_get_entry_customattr(IN LPTDRMETALIB a_pstLib, IN LPTDRMETAENTRY a_pstEntry);


/** @}*/ /* TDR_ENTRY_MANAGE TDR_���ݳ�Ա(entry)����*/

/** @defgroup TDR_MACRO_MANAGE TDR_�궨��(macro)����
*  @{
*/		 
/** ���ݺ����������ֵ
*@param[out] a_piID ��ú궨�����ֵ
*@param[in] a_pstLib Ԫ���ݿ�
*@param[in] a_pszName ����Һ������
*
*@return �ɹ�����0�����򷵻ط�0
*
*@pre \e a_piID ����ΪNULL
*@pre \e a_pstLib ����ΪNULL
*@pre \e a_pszName ����ΪNULL
*/
TDR_API int tdr_get_macro_value(OUT int *a_piID, IN LPTDRMETALIB a_pstLib, IN const  char *a_pszName);



/** @}*/ /* TDR_META_MANAGE TDR_�궨��(macro)����*/

#ifdef __cplusplus
}
#endif


#endif /* TDR_METALIB_MANAGE_H */
