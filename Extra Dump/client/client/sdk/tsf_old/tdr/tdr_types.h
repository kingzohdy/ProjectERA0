/**
*
* @file     tdr_types.h
* @brief    DRʹ�õ���Ҫ���Ͷ���
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



#ifndef TDR_TYPES_H
#define TDR_TYPES_H

#include "tdr/tdr_define.h"

/** @defgroup TDR_TYPE TDR_��Ҫ���Ͷ���
 *  @{
 */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



#ifndef TDR_CUSTOM_C_TYPES
#define TDR_CUSTOM_C_TYPES

#if defined(WIN32) &&  _MSC_VER < 1300
	typedef __int64 tdr_longlong;
	typedef unsigned __int64 tdr_ulonglong;
#else
	typedef long long tdr_longlong;
	typedef unsigned long long tdr_ulonglong;
#endif  /*defined(WIN32) &&  _MSC_VER < 1300*/

	typedef unsigned short tdr_wchar_t;  /**<Wchar������������*/
	typedef unsigned int tdr_date_t;	/**<data������������*/
	typedef unsigned int tdr_time_t;	/**<time������������*/
	typedef tdr_ulonglong tdr_datetime_t; /**<datetime������������*/
	typedef unsigned long int tdr_ip_t;  /**<IPv4��������*/
#endif


/** a offset-alike type   */
typedef int TDRPTR;  

/** a index-alike type   */  
typedef int TDRIDX;     

/**a offset-alike type, describe the position in a meta: struct or union */
typedef int TDROFF;   





/**
 * This is the pointer type delcaration for TDR MetaLib.
 */
typedef struct tagTDRMetaLib	*LPTDRMETALIB;




/**
 * This is the type delcaration for TDR MetaLib param.
 */
typedef struct tagTDRLibParam	TDRLIBPARAM;

/**
 * This is the pointer type delcaration for TDR MetaLib param.
 */
typedef struct tagTDRLibParam	*LPTDRLIBPARAM;




/**
 * This is the pointer type delcaration for TDR Meta.
 */
typedef struct tagTDRMeta	*LPTDRMETA;



/**
 * This is the pointer type delcaration for TDR MetaEntry.
 */
typedef struct tagTDRMetaEntry	*LPTDRMETAENTRY;




/**
 * This is the type delcaration for TDR TDRData.
 */
typedef struct tagTDRData	TDRDATA;
/**
 * This is the pointer type delcaration for TDR TDRData.
 */
typedef struct tagTDRData	*LPTDRDATA;


/**
 * This is the pointer type delcaration for TDR Macro.
 */
typedef struct tagTDRMacro		*LPTDRMACRO;


/**
 * This is the pointer type info for c/c++ base date type info.
 */
typedef struct tagTDRCTypeInfo	*LPTDRCTYPEINFO;

/**����c/c++ ͷ�ļ�����ṹ����
*/
typedef struct tagTDRHppRule TDRHPPRULE;
typedef struct tagTDRHppRule *LPTDRHPPRULE;

/** �������ݿ����ϵͳ��Ϣ�ṹ������
*/
typedef struct tagTDRDBMS TDRDBMS;
typedef struct tagTDRDBMS *LPTDRDBMS;


/** ����DB������������
*/
typedef void * TDRDBHANDLE;
typedef TDRDBHANDLE * LPTDRDBHANDLE;

/** ����DB������������
*/
typedef void * TDRDBRESULTHANDLE;
typedef TDRDBRESULTHANDLE * LPTDRDBRESULTHANDLE;

/** DB�������ݶ��������
*/
typedef  struct tagDBObject  TDRDBOBJECT;
typedef  struct tagDBObject  *LPTDRDBOBJECT;


/**��ʼ��Ԫ���ݿ�Ľṹ��
*/
struct tagTDRLibParam
{
	int iID;        /**< Ԫ�����ID*/ 

	int iTagSetVersion; /**< Ԫ��������XML��ǩ���İ汾*/

	int iSize;		/**< Ԫ���ݵ��ڴ��С��tdr_init_lib�����ݴ����ݷ���ռ�*/
	int iMetaSize;      /**< �Զ�������������ռ�Ŀռ��С*/
	int iStrBufSize;    /**< �ַ�����������ռ�Ĵ�С*/

	int iMaxMacros;     /**< ��Ԫ���ݿ���������궨����*/
	int iMaxMetas;	/**< ��Ԫ���ݿ���������ṹ����*/

	int iRes;           /**< Ԥ��λ����δʹ��*/
	long  lVersion;     /**< Ԫ����İ汾*/

	char szName[TDR_NAME_LEN];  /**< Ԫ���ݿ�����*/

};

/**
* struct of TDR DRData
*/
struct tagTDRData
{
	char* pszBuff;          /**< ����DR���ݵĻ�����ָ��*/
	int iBuff;		/**< ����DR���ݵĻ�������С */
};



/**����c/c++ ͷ�ļ�����ṹ
*/
struct tagTDRHppRule
{
	int iRule;	/**<����ͷ�ļ��Ĺ���*/

	/*�û��Զ�������ǰ׺ iRule������TDR_HPPRULE_ADD_CUSTOM_PREFIX��־λʱ��������*/
	char szCustomNamePrefix[TDR_MAX_CUSTOM_NAME_PREFIX_LEN];  
};

/**���ݿ����ϵͳDBMS��Ϣ�ṹ
*/
struct tagTDRDBMS 
{
	int iMajorVersion;				/**<DBMS�����汾��*/
	int iMinVersion;				/**<DBMS�Ĵΰ汾��*/
	

	/**<��DBMS�������˽������ӵ���Ϣ����ͬDBMS�������ݿ��ܲ�ͬ������mysqlϵͳ�����ֶ�ָ��mysql��������������Ϣ*/
	int iReconnectOpt;	 /*�����Ϊ���Ӷ�ʧ����ִ��SQLʧ��ʱ�����ֶ�������ʾ�Ƿ��ؽ����Ӳ�����
						 ����ִ��SQL�����ֶ�ֵΪ����ֵʱ�ؽ����ӣ������ؽ�����*/
	char szDBMSName[TDR_NAME_LEN];   /**<DBMS�����ƣ��磺mysql,oracle,sqlserver��*/
	char szDBMSConnectionInfo[TDR_DBMS_CONNECTION_INFO_LEN]; 
	char szDBMSCurDatabaseName[TDR_DBMS_DATABASE_NAME_LEN]; /**<DBMS��ǰ���ݿ���*/
	char szDBMSUser[TDR_DBMS_USER_LEN];					/**<DBMS��ǰ���ݿ��û�����*/
	char szDBMSPassword[TDR_DBMS_PASSWORD_LEN];			/**<DBMS�û�����*/
};



struct tagDBObject
{
	LPTDRMETA pstMeta;    /**<���ݵ�Ԫ��������*/
	int iVersion;		/**<���ݵļ��ð汾*/
	int  iObjSize;     /**<�������ݵ��������Ĵ�С*/
	char *pszObj;     /**<�������ݵ��������׵�ַ*/
};

typedef enum tagTDRSortMethod TDRSORTMETHOD;
typedef enum tagTDRSortMethod *LPTDRSORTMETHOD;

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @}*/ // TDR_TYPE DR��Ҫ���Ͷ���
#endif /* TDR_TYPES_H */
