/**
*
* @file     tormdbapi.h  
* @brief    TORM服务器与后台数据库管理系统之间交互API接口头文件
*	此接口是在TDR 的tdr_sql.h中定义接口的扩充，TORM服务器结合tdr_sql.h中的接口和本文件中定义
*   的接口完成与后台数据库管理系统之间交互
* 
* @author jackyai  
* @version 1.0
* @date 2007-11-23 
*
*
* All rights reserved.
*
*/

#ifndef TORMAPIDBAPI_H
#define TORMAPIDBAPI_H


#include "tormapi/torm_error.h"
#include "tormapi/torm_protocol.h"
#include "tormapi/torm_protocol_api.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct  tagTormDBObject
{
	int iVersion;		/**<操作数据的版本*/
	LPTDRMETA	pstMeta; /**<操作数据的元数据描述句柄*/
	LPTORMOPERINFO pstOperInfo; /**<操作数据*/
};
typedef struct  tagTormDBObject TORMDBOBJECT;
typedef struct  tagTormDBObject	*LPTORMDBOBJECT;


	/** @defgroup TORM_DBAPI TORM_对象关系映射
	* @{
	*/

/**根据数据操作信息生成SQL操作语句 
	*@param[out] a_pszNameBuff 保存操作数据所在数据库表名的缓冲区
	*@param[in] a_iNameBuffSize 缓冲区的最大可用空间	
	*@param[in] a_pstSplitInfo 保存操作数据的分表信息
	*@param[in] a_pstMeta	操作数据的元数据描述句柄
	*
	* @pre \e a_pszNameBuff 不能为NULL
	* @pre \e a_pstOperInfo 不能为NULL
	* @pre \e a_pstMeta 不能为NULL
	*
	*@retval 0   处理成功
	*@retval <0  处理失败，返回表示出错信息的错误代码：
*/
int torm_get_table_name(OUT char *a_pszNameBuff, IN int a_iNameBuffSize, IN LPTORMSUBPACK a_pstSplitInfo, IN LPTDRMETA a_pstMeta);


	
/**根据数据操作信息生成SQL操作语句 
*@param[in] a_hOrmHandler	ORM协议处理句柄
*@param[in] a_hDBHandler 保护DBMS处理句柄
*@param[in,out] a_pstSql 保存SQL语句的缓冲区结构指针
*	- 输入	a_pstSql.pszBuff 指定保存本地结构信息的缓冲区起始地址
*	- 输入  a_pstSql.iBuff	指定缓冲区的大小
*	- 输出  a_pstSql.iBuff  实际生成信息的总长度
*@param[in] a_iOPType DB操作的（Insert、Update、Delete和Select）ID,具体值的定义请参看tdr_define.h中TDRDBOPID定义
*@param[in,out] a_pstObject 操作数据相关信息结构
*	-	a_pstObject.pstMeta 操作数据的元数据描述句柄
*	-	a_pstObject.iVersion	 操作数据的版本，如果其值为0，则按最高版本处理
*	-	a_pstObject.pstOperInfo	操作数据信息
*	-	a_pstObject.pstMeta 其指针可能会不改写
*
* @pre \e a_hDBHandle 必须是通过tdr_open_dbhanlde打开的句柄
* @pre \e a_pstSql 不能为 NULL
* @pre \e a_pstObject 不能为NULL
* @pre \e a_pstMeta 不能为NULL
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码：
*/
int torm_operinfo2sql(IN TORMPROTOCOLHANDLE a_hOrmHandler, IN TDRDBHANDLE a_hDBHandler, INOUT LPTDRDATA a_pstSql, IN int a_iOPType, INOUT LPTORMDBOBJECT a_pstObject);


/** 根据元数据描述检索结果集的下一个数据，并按照指定剪裁版本将数据按网络消息格式打包到指定缓冲区中
*@param[in] a_hOrmHandler	ORM协议处理句柄
*@param[in] a_hDBResult 保存DB操作结果集的句柄
*@param[in,out] a_pstRecord 保存数据的缓冲区信息
*	- 输入	a_pstRecord.SubPack 指定此数据的缓冲区首地址
*	- 输入  a_pstRecord.SubPackLen	缓冲区最大可用字节数
*	- 输出  a_pstRecord.iBuff  此实际打包数据字节数
*@param[in] a_pstDBObj 操作数据结构体指针
*@param[in] a_bDoGetPrikey 指示是否取出主键信息
*
* @pre \e a_hDBResult 必须是通过tdr_query或tdr_query_quick获取的结果句柄
* @pre \e a_pstRecord 不能为NULL
* @pre \e a_pstDBObj 不能为NULL
*
*@retval 0   处理成功
*@retval >0  处理成功，但发生某些特殊情况：
*	- TDR_SUCWARN_TRUNCATE_DATE 保存数据时进行了数据截断
*@retval <0  处理失败，返回表示出错信息的错误代码：
*/
int  torm_fetch_row(IN TORMPROTOCOLHANDLE a_hOrmHandler, IN TDRDBRESULTHANDLE a_hDBResult,INOUT LPTORMSUBPACK a_pstRecord, 
					IN LPTORMDBOBJECT a_pstDBObj, IN TDRBOOLEAN a_bDoGetPrikey);


/**根据数据操作信息生成SQL操作语句 
*@param[in] a_hOrmHandler	ORM协议处理句柄
*@param[in,out] a_pstRecord 保存更新数据的缓冲区结构指针
*	- 输入	a_pstRecord.pszBuff 指定保存本地结构信息的缓冲区起始地址
*	- 输入  a_pstRecord.iBuff	指定缓冲区的大小
*	- 输出  a_pstRecord.iBuff  实际生成信息的总长度
*@param[in] a_pstOperInfo	操作数据信息
*@param[in] a_pstMeta 操作数据的元数据描述句柄
*@param[in] a_iVersion	 操作数据的版本，如果其值为0，则按最高版本处理
*
*
* @pre \e a_hOrmHandler 必须是通过tdr_open_dbhanlde打开的句柄
* @pre \e a_pstHost 不能为 NULL
* @pre \e a_pstObject 不能为NULL
* @pre \e a_pstMeta 不能为NULL
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码：
*/
int torm_update_record(IN TORMPROTOCOLHANDLE a_hOrmHandler, INOUT LPTDRDATA a_pstRecord, IN LPTORMOPERINFO a_pstOperInfo, IN LPTDRMETA a_pstMeta, IN int a_iVersion);


/**按照操作成员列表打包本地数据 
*@param[in] a_hOrmHandler	ORM协议处理句柄
*@param[in,out] a_pstOrmPack 保存打包数据的结构支持
*	- 输出  a_pstOrmPack.SubPackLen  实际生成信息的总长度
*@param[in] a_pstRecord 保存更新数据的缓冲区结构指针
*	- 输入	a_pstRecord.pszBuff 指定保存本地结构信息的缓冲区起始地址
*	- 输入  a_pstRecord.iBuff	指定缓冲区的大小
*@param[in] a_pstEntryList 结构中数据成员列表，如果列表为空，则表示取出所有成员的数据
*@param[in]	a_pstMeta 操作数据的元数据描述句柄
*@param[in]	a_iVersion	 操作数据的版本，如果其值为0，则按最高版本处理
*
* @pre \e a_hOrmHandler 必须是通过tdr_open_dbhanlde打开的句柄
* @pre \e a_pstHost 不能为 NULL
* @pre \e a_pstOrmPack 不能为NULL
* @pre \e a_pstMeta 不能为NULL
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码：
*/
int torm_pack_record(IN TORMPROTOCOLHANDLE a_hOrmHandler, INOUT LPTORMSUBPACK a_pstOrmPack, IN LPTDRDATA a_pstRecord, IN LPTORMENTRYNAMELIST a_pstEntryList, IN LPTDRMETA a_pstMeta, IN int a_iVersion);

/** 从本地数据中抽取主键信息进行打包
*@param[in]	a_pstMeta 操作数据的元数据描述句柄
*@param[in,out] a_pstPrimarykey 保存打包数据的结构支持
*	- 输入  a_pstPrimarykey.pszBuff 接收打包信息的缓冲区首地址
*	- 输入  a_pstPrimarykey.iBuff 接收打包信息的缓冲区最大长度
*	- 输出  a_pstPrimarykey.iBuff  实际生成信息的总长度
*@param[in] a_pstData 保存结构体数据缓冲区结构指针
*	- 输入	a_pstData.pszBuff 保存结构体数据的缓冲区起始地址
*	- 输入  a_pstData.iBuff	指定缓冲区的大小
*@param[in]	a_iVersion	 操作数据的版本，如果其值为0，则按最高版本处理
*
* @pre \e a_pstMeta 不能为NULL
* @pre \e a_pstPrimarykey 不能为 NULL
* @pre \e a_pstData 不能为NULL
*
*@retval 0   处理成功
*@retval <0  处理失败
*/
int torm_pack_primarykey(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstPrimarykey, IN LPTDRDATA a_pstData, int a_iVersion);

/** 从打包的主键信息中分析是否包含完整的数据表主键信息
*@param[in] a_hOrmHandler	ORM协议处理句柄
*@param[in]	a_pstMeta 操作数据的元数据描述句柄
*@param[in] a_pstPrimarykey 保存打包数据的结构支持
*	- 输入  a_pstPrimarykey.pszBuff 接收打包信息的缓冲区首地址
*	- 输入  a_pstPrimarykey.iBuff 接收打包信息的缓冲区最大长度
*
* @pre \e a_hOrmHandler 必须是通过tdr_open_dbhanlde打开的句柄
* @pre \e a_pstMeta 不能为NULL
* @pre \e a_pstPrimarykey 不能为 NULL
*
*@return 如果包含完整的主键信息，返回TDR_TRUE;否则返回TDR_FALSE
*/
TDRBOOLEAN torm_is_whole_primarykey(IN TORMPROTOCOLHANDLE a_hOrmHandler, IN LPTDRMETA a_pstMeta, IN LPTORMSUBPACK a_pstPrimarykey);



/** @}*/ /*defgroup TORM_DBAPI TORM_对象关系映射*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
