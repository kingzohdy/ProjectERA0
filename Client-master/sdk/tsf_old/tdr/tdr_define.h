/**
*
* @file     tdr_define.h 
* @brief    TDR��������
* 
* @author steve jackyai  
* @version 1.0
* @date 2007-03-22 
*
*
* Copyright (c)  2007, ��Ѷ�Ƽ����޹�˾���������з���
* All rights reserved.
*
*/

#ifndef TDR_DEFINE_H
#define TDR_DEFINE_H


/** @name ������������/�������
 *  @{
 */
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
	        
/** @}*/ // ������������/�������

/**
*bool�ͱ���ȡֵ����
*/ 
typedef enum tagBoolean
{
	TDR_FALSE = 0,	    /**< false*/
	TDR_TRUE			/**< true*/ 
}TDRBOOLEAN;


/** @name Ԫ������������
 *  @{
 */
#define TDR_MAGIC		0x02D6      /**< Ԫ���ݶ������ļ��е�"ħ��"*/

        
#define	TDR_MAX_VERSION		0x7fffffff		/**< Ԫ���ݿ����汾��*/

#define TDR_NAME_LEN		32		/**< Ԫ�������ִ�����ֽ���*/
#define TDR_DESC_LEN		1024		/**< Ԫ��������������ֽ���*/
#define TDR_MACRO_LEN	64		/**< Ԫ���ݺ궨�����ִ�����ֽ���*/
#define TDR_CHNAME_LEN	512		/**< Ԫ����������������ֽ���*/
#define TDR_DEFAULT_VALUE_LEN	1024	/**< Ԫ����ȱʡֵ����ֽ���*/
/** @}*/ // Ԫ������������

#define TDR_MAX_FILES_IN_ONE_PARSE		128   /**< һ�ν��������XML�ļ���*/

#define TDR_DEFAULT_ALIGN_VALUE		1	/**<ȱʡ�ֽڶ���ֵ*/



/** @name ֧�ֵ�Ԫ����XML��ǩ���İ汾��Ϣ
 *  @{
 */
#define TDR_XML_TAGSET_VERSION_0             0      /**< ��0�汾*/
#define TDR_SUPPORTING_MIN_XMLTAGSET_VERSION	TDR_XML_TAGSET_VERSION_0       /**< ֧�ֵ����ٰ汾��*/
#define TDR_XML_TAGSET_VERSION_1             1      /**< ��1�汾*/
#define TDR_SUPPORTING_MAX_XMLTAGSET_VERSION	TDR_XML_TAGSET_VERSION_1      /**< ֧�ֵ����汾��*/
/** @}*/ // ֧�ֵ�Ԫ����XML��ǩ���İ汾��Ϣ


#define TDR_POINTER_UINT_SIZE 4         /**<ָ������洢���ֽ���*/



/** @name Ԫ��������c����ͷ�ļ�����(���������ͨ�����롯�ķ�ʽ���)
*  @{
*/
/**��Ա����������ȱʡ�����������ǰ׺�����û������ǰ׺���Ա������ĸ���Сд��
*����ĳ��Ա��DR�е���������ֵΪ"name",������Ϊ"string",����c����ͷ�ļ�������Ϊ:
*	szName	
*/
#define TDR_HPPRULE_DEFAULT   0x0000

#define TDR_HPPRULE_NO_TYPE_PREFIX	0x0001	/**<��Ա���������򣺲��������ǰ׺*/

/**��Ա��������������Զ���ǰ׺��������ĳ��Ա��DR�е���������ֵΪ"name",������Ϊ"string",�Զ���ǰ׺Ϊ"m_" 
*����c����ͷ�ļ�������Ϊ:	char*	m_Name	
*/
#define TDR_HPPRULE_ADD_CUSTOM_PREFIX 0x0002

#define TDR_HPPRULE_NO_LOWERCASE_PREFIX	0x0004	/**<��Ա���������򣺲�ǿ�ƽ���������ĸ���Сд*/

#define TDR_HPPRULE_NO_TYPE_DECLARE		0x0008 /**<ͷ�ļ��в��������ǰ׺*/

/** @ }*/

#define TDR_MAX_CUSTOM_NAME_PREFIX_LEN	9		/**<�û��Զ�������ǰ׺����󳤶�*/


/**@name IO����ֵ
*@ {*/
#define TDR_IO_NOLIMIT		0x00  /**<��ʾ��IO���/���û������*/
#define TDR_IO_NOINPUT		0x01    /**<��ʾ�˳�Ա��������*/
#define TDR_IO_NOOUTPUT		0x02    /**<��ʾ�˳�Ա�������*/
#define TDR_IO_NOIO		0x03    /**< ��ʾ�˳�Ա����������/���*/
#define TDR_IO_MIN_VALUE      TDR_IO_NOLIMIT
#define TDR_IO_MAX_VALUE TDR_IO_NOIO
/*@ }*/

/** @name �������XML��ʽ
*  @{
*/
#define TDR_IO_OLD_XML_VERSION		1		/**<�ɵ�XML����/�����ʽ*/
#define TDR_IO_NEW_XML_VERSION		2		/**<�µ�XML����/�����ʽ*/
/** @} */

/**@name ���򷽷�
*@ {*/
enum tagTDRSortMethod
{
	TDR_SORTMETHOD_NONE_SORT =	0x00,  /**< ������*/
	TDR_SORTMETHOD_ASC_SORT	=	0x01,    /**<��������*/
	TDR_SORTMETHOD_DSC_SORT =0x02,    /**<��������*/
};
/*@ }*/

/**@name DBMS��غ궨��
*@ {*/
#define TDR_DBMS_NAME_LEN					32	/**<DBMS������󳤶�*/
#define TDR_DBMS_CONNECTION_INFO_LEN		256  /**< DBMS������Ϣ����󳤶�*/
#define TDR_DBMS_DATABASE_NAME_LEN			64	/**<DBMS���ݿ�������󳤶�*/
#define TDR_DBMS_USER_LEN					256 /**<DBMS�û�������󳤶�*/
#define TDR_DBMS_PASSWORD_LEN				64  /**<DBMS�û�������󳤶�*/
/*@ }*/


/** @name DB����ID
*  @{
*/
typedef enum 
{
	TDR_DBOP_NONE = 0,		/**<�޲���*/
	TDR_DBOP_INSERT,		/**<�������*/	
	TDR_DBOP_UPDATE,		/**<���²���*/	
	TDR_DBOP_DELETE,		/**<ɾ������*/
	TDR_DBOP_SELECT,		/**<��ѯ����*/
}TDRDBOPID;

#endif /* TDR_DEFINE_H */
