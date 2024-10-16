/**
*
* @file     tdr_types.h
* @brief    DR使用的主要类型定义
* 
* @author steve jackyai  
* @version 1.0
* @date 2007-03-22 
*
*
* All rights reserved.
*
*/



#ifndef TDR_TYPES_H
#define TDR_TYPES_H

#include "tdr/tdr_define.h"

/** @defgroup TDR_TYPE TDR_主要类型定义
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

	typedef unsigned short tdr_wchar_t;  /**<Wchar基本数据类型*/
	typedef unsigned int tdr_date_t;	/**<data基本数据类型*/
	typedef unsigned int tdr_time_t;	/**<time基本数据类型*/
	typedef tdr_ulonglong tdr_datetime_t; /**<datetime基本数据类型*/
	typedef unsigned long int tdr_ip_t;  /**<IPv4数据类型*/
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

/**生成c/c++ 头文件规则结构声明
*/
typedef struct tagTDRHppRule TDRHPPRULE;
typedef struct tagTDRHppRule *LPTDRHPPRULE;

/** 生成数据库管理系统信息结构的声明
*/
typedef struct tagTDRDBMS TDRDBMS;
typedef struct tagTDRDBMS *LPTDRDBMS;


/** 生成DB处理句柄的声明
*/
typedef void * TDRDBHANDLE;
typedef TDRDBHANDLE * LPTDRDBHANDLE;

/** 生成DB处理句柄的声明
*/
typedef void * TDRDBRESULTHANDLE;
typedef TDRDBRESULTHANDLE * LPTDRDBRESULTHANDLE;

/** DB处理数据对象的声明
*/
typedef  struct tagDBObject  TDRDBOBJECT;
typedef  struct tagDBObject  *LPTDRDBOBJECT;


/**初始化元数据库的结构体
*/
struct tagTDRLibParam
{
	int iID;        /**< 元数库的ID*/ 

	int iTagSetVersion; /**< 元数据描述XML标签集的版本*/

	int iSize;		/**< 元数据的内存大小，tdr_init_lib将根据此数据分配空间*/
	int iMacrosGroupSize;      /**< 自定义数据类型所占的空间大小*/
	int iMetaSize;      /**< 自定义数据类型所占的空间大小*/
	int iStrBufSize;    /**< 字符串缓冲区所占的大小*/

	int iMaxMacros;     /**< 此元数据库包含的最大宏定义数*/
	int iMaxMetas;	/**< 此元数据库包含的最大结构体数*/

	int iMaxMacrosGroupNum; /*最多能容纳的宏定义组数目, Add at TDR build Version: 0x00000008*/

	int iRes;           /**< 预留位，暂未使用*/
	long  lVersion;     /**< 元数库的版本*/

	char szName[TDR_NAME_LEN];  /**< 元数据库名字*/

};

/**
* struct of TDR DRData
*/
struct tagTDRData
{
	char* pszBuff;          /**< 保存DR数据的缓冲区指针*/
	int iBuff;		/**< 保存DR数据的缓冲区大小 */
};



/**生成c/c++ 头文件规则结构
*/
struct tagTDRHppRule
{
	int iRule;	/**<生成头文件的规则*/

	/*用户自定义名字前缀 iRule设置了TDR_HPPRULE_ADD_CUSTOM_PREFIX标志位时才有意义*/
	char szCustomNamePrefix[TDR_MAX_CUSTOM_NAME_PREFIX_LEN];  
};

/**数据库管理系统DBMS信息结构
*/
struct tagTDRDBMS 
{
	int iMajorVersion;				/**<DBMS的主版本号*/
	int iMinVersion;				/**<DBMS的次版本号*/
	

	/**<与DBMS服务器端建立连接的信息，不同DBMS设置内容可能不同。对于mysql系统，此字段指定mysql服务器的主机信息*/
	int iReconnectOpt;	 /*如果因为连接丢失引起执行SQL失败时，此字段用来表示是否重建连接并重新
						 尝试执行SQL。此字段值为非零值时重建连接；否则不重建连接*/
	char szDBMSName[TDR_NAME_LEN];   /**<DBMS的名称，如：mysql,oracle,sqlserver等*/
	char szDBMSConnectionInfo[TDR_DBMS_CONNECTION_INFO_LEN]; 
	char szDBMSCurDatabaseName[TDR_DBMS_DATABASE_NAME_LEN]; /**<DBMS当前数据库名*/
	char szDBMSUser[TDR_DBMS_USER_LEN];					/**<DBMS当前数据库用户名称*/
	char szDBMSPassword[TDR_DBMS_PASSWORD_LEN];			/**<DBMS用户密码*/
	char szDBMSEngine[TDR_DBMS_TABLE_ENGINE_LEN];		/**<DBMS数据库表的引擎,对于mysql可以是：ISAM,MyISAM, InnoDB*/
	char szDBMSCharset[TDR_DBMS_TABLE_CHARTSET_LEN];     /**<DBMS数据库表的字符集*/
	char szDBMSSock[TDR_DBMS_SOCK_FILE_LEN];		/*与dbms使用UNIX sock通信时所使用的文件*/
};



struct tagDBObject
{
	LPTDRMETA pstMeta;    /**<数据的元数据描述*/
	int iVersion;		/**<数据的剪裁版本*/
	int  iObjSize;     /**<保存数据的数据区的大小*/
	char *pszObj;     /**<保存数据的数据区首地址*/
};

typedef enum tagTDRSortMethod TDRSORTMETHOD;
typedef enum tagTDRSortMethod *LPTDRSORTMETHOD;




/*回调函数，在遍历结构成员时，如果此成员为struct/uion类型则回调此函数，获取此子成员处理句柄
@param[in] a_pHandle 此成员父结点的处理句柄
@param[in] a_pstChildEntry 此子成员的元数据描述句柄
@param[in] a_iTotal 此子成员的数组长度
@param[in] a_Index	如果此成员所在结构体为数组，则传入其数组当前索引值，否则传 -1
@param[out] a_ppChildHandle 获取此子成员处理句柄的指针
@note 根据子成员的a_pstChildEntry句柄调用tdr_get_entry_type_meta可以获得此子成员的类型
@return 0 成功 !0 失败
*/
typedef int (*PFN_ENTER_META)(IN void *a_pHandle, IN LPTDRMETAENTRY a_pstChildEntry, IN int a_iTotal, IN int a_Index,  OUT void ** a_ppHandle);


/*回调函数，在遍历结构成员时，如果此成员为简单数据类型,则回调此函数，
@param[in] a_pHandle 此成员父结点的处理句柄
@param[in] a_pstEntry 此子成员的元数据描述句柄
@param[in] a_iCount 此成员的数组长度
@param[in] a_pszHostStart 此成员本地存储起始地址
@param[in] a_iSize 此成员存储空间可用长度
@return 0 成功 !0 失败
*/
typedef int (*PFN_VISIT_SIMPLE_ENTRY)(IN void *a_pHandle, IN LPTDRMETAENTRY a_pstEntry, IN int a_iCount,
									  IN char *a_pszHostStart, IN int a_iSize);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @}*/ // TDR_TYPE DR主要类型定义
#endif /* TDR_TYPES_H */
