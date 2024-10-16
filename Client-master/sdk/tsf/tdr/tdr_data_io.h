/**
*
* @file     tdr_data_io.h 
* @brief    TDR元数据库IO操作模块
* 
* @author steve jackyai  
* @version 1.0
* @date 2007-03-26 
*
*
* Copyright (c)  2007, 腾讯科技有限公司互动娱乐研发部
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



/**@defgroup  TDR_METALIB_IO TDR_从文件/文件流中保存/恢复元数据库
* @{
*/

/**从元数据描述二进制格式文件中加载元数据库,并校验其散列值是否和期望的散列值(NULL)一致
* @param[in,out] a_ppstLib 保存元数据库的指针的指针
* @param[in] a_pszBinFile 保存元数据库二进制格式的文件的文件名
* @param[in] a_pszHash 可视化的要校验的元数据描述库散列值字符串指针
*
* @note 元数据库的空间在函数内动态分配，需调用tdr_free_lib释放元数据库的空间
* @note 利用TDR的工具将元数据描述库转换成.h文件时，会将元数据库的散列值字符串作为宏定义写到.h文件中，此宏
* 定义的名字的命名规则为: "TDR_" + "元数据描述库名_" + "HASH"
* @note 如果a_pszHash参数为NULL，则不进行散列值校验，此时此接口和tdr_load_metalib接口的功能相同
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码:
*	- TDR_ERROR_FAILED_OPEN_FILE_TO_READ	打开文件读失败
*	- TDR_ERROR_INVALID_METALIB_FILE	元数据库文件的字节数为0，或读取的字节数和元数据描述库结构中记录的字节数不同
*	- TDR_ERROR_BUILD_VERSION_CONFLICT	TDR生成元数据库文件的工具的构建版本和加载工具不一致
*	- TDR_ERROR_DIFF_METALIB_HASH	元数据库的散列值和期望的散列值不一致
*
*@pre \e a_ppstLib 不能为NULL
*@pre \e a_pszBinFile 不能为 NULL
*@see tdr_free_lib
*/
TDR_API int tdr_load_verify_metalib(INOUT LPTDRMETALIB* a_ppstLib, IN const char* a_pszBinFile, IN const char *a_pszHash);


/**从元数据描述二进制格式文件中加载元数据库
* @param[in,out] a_ppstLib 保存元数据库的指针的指针
* @param[in] a_pszBinFile 保存元数据库二进制格式的文件的文件名
* @note 元数据库的空间在函数内动态分配，需调用tdr_free_lib释放元数据库的空间
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码:
*	- TDR_ERROR_FAILED_OPEN_FILE_TO_READ	打开文件读失败
*	- TDR_ERROR_INVALID_METALIB_FILE	元数据库文件的字节数为0，或读取的字节数和元数据描述库结构中记录的字节数不同
*	- TDR_ERROR_BUILD_VERSION_CONFLICT	TDR生成元数据库文件的工具的构建版本和加载工具不一致
*
*@pre \e a_ppstLib 不能为NULL
*@pre \e a_pszBinFile 不能为 NULL
*@see tdr_free_lib
*/
TDR_API int tdr_load_metalib(INOUT LPTDRMETALIB* a_ppstLib, IN const char* a_pszBinFile);

/**从元数据描述二进制格式中加载元数据库
* @param[in,out] a_ppstLib 保存元数据库的指针的指针
* @param[in] a_fpBin 保存元数据库的元数据描述二进制格式的文件句柄
* @note 元数据库的空间在函数内动态分配，需调用tdr_free_lib释放元数据库的空间
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码:
*	- TDR_ERROR_FAILED_OPEN_FILE_TO_READ	打开文件读失败
*	- TDR_ERROR_INVALID_METALIB_FILE	元数据库文件的字节数为0，或读取的字节数和元数据描述库结构中记录的字节数不同
*	- TDR_ERROR_BUILD_VERSION_CONFLICT	TDR生成元数据库文件的工具的构建版本和加载工具不一致
*
*@pre \e a_ppstLib 不能为NULL
*@pre \e a_fpBin 不能为 NULL
*@see tdr_free_lib
*/
TDR_API int tdr_load_metalib_fp(INOUT LPTDRMETALIB* a_ppstLib, IN FILE* a_fpBin);

/**将元数据库保存到二进制格式的文件中
* @param[in,out] a_pstLib 保存元数据库的指针
* @param[in] a_pszBinFile 保存元数据库的二进制格式文件的文件名
*
*@retval 0   处理成功
*@retval <0  处理失败，可能的错误代码如下:
*	- TDR_ERROR_INVALID_PARAM	参数无效
	- TDR_ERRIMPLE_FAILED_OPEN_FILE_TO_WRITE failed to open file to write
	- TDR_ERROR_FAILED_TO_WRITE_FILE	failed to write data to file
*
*@pre \e a_pstLib 不能为NULL
*@pre \e a_pszBinFile 不能为 NULL
*/
TDR_API int tdr_save_metalib(IN LPTDRMETALIB a_pstLib, IN const char* a_pszBinFile);

/**将元数据库保存到二进制格式的文件中
* @param[in,out] a_pstLib 保存元数据库的指针
* @param[in] a_fpBin 保存元数据库的二进制格式的文件句柄
*
@retval 0   处理成功
*@retval <0  处理失败，可能的错误代码如下:
*	- TDR_ERROR_INVALID_PARAM	参数无效
	- TDR_ERROR_FAILED_TO_WRITE_FILE	failed to write data to file
*
*@pre \e a_pstLib 不能为NULL
*@pre \e a_fpBin 不能为 NULL
*/
TDR_API int tdr_save_metalib_fp(IN LPTDRMETALIB a_pstLib, IN FILE* a_fpBin);

/**@} */ /*  文件/文件流中保存/恢复元数据库*/

/**@defgroup  TDR_METALIB_DUMP TDR_元数据库导出(调试时用)
* @{
*/

/**打印元数据库的内容
*@param[in] a_pstLib 要输出的元数据库
*@param[in] a_fp 输出文件句柄
@retval 0   处理成功
*@retval <0  处理失败，可能的错误代码如下:
*	- TDR_ERROR_INVALID_PARAM	参数无效
*
*@pre \e a_pstLib 不能为NULL
* @pre \e a_fp 不能为 NULL
*/
TDR_API void tdr_dump_metalib(IN LPTDRMETALIB a_pstLib, IN FILE* a_fp);

/**打印元数据库的内容
*将内容保存在a_pszFile指定的文件中
*@param[in] a_pstLib 要输出的元数据库
*@param[in] a_pszFile 输出文件名字串的指针
*@retval <0  处理失败，返回表示出错信息的错误代码
*@retval 0   处理成功
*@retval >0  处理成功，但发生某些特殊情况
*
*@pre \e a_pstLib 不能为NULL
* @pre \e a_pszFile 不能为 NULL
*/
TDR_API void tdr_dump_metalib_file(IN LPTDRMETALIB a_pstLib, IN const char* a_pszFile);

/**@} */ /*  TDR_METALIB_DUMP TDR_元数据库导出(调试时用)*/


/**@defgroup  TDR_DATA_IO TDR_数据以XML格式输入/输出
* @{
*/

/**设置被tdr接口处理的内存数据的字符集，如"GBK","UTF8"等
@param[in] a_pszEncoding 指定字符集信息的字符串缓冲区首地址
*@note 本接口是非线程安全的，如果在多线程环境调用此接口，调用者必须保证互斥调用本接口
*@note tdr缺省认为数据的字符集为"GBK"
*/
void tdr_set_encoding(IN const char *a_pszEncoding);

/**
*根据元数据描述将，可以把一个内存数据结构格式转化成一个符合特定规则的xml信息，并保存在指定缓冲区中
*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
*@param[in,out]	a_pstXml 存储XML格式信息的结构
*	- 输入	a_pstXml.pszBuff 指定保存XML信息的缓冲区起始地址
*	- 输入  a_pstXml.iBuff	指定缓冲区的大小
*	- 输出  a_pstXml.iBuff  实际转换后的信息的总长度
*@param[in]	a_pstHost 存储本地内存数据结构信息的结构体指针
*	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
*	- 输入  a_pstHost.iBuff	指定缓冲区的大小
*@param[in] a_iCutOffVersion	要剪裁的版本号
*@param[in] a_iXmlDataFormat XML数据文件的格式,目前支持的格式有：
*	- TDR_XML_DATA_FORMAT_LIST_MATA_NAME	
*	- TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME 
*	-TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME
*	- 这几种格式的区别请参见文件 TSF4G-DR-FAQ.doc 关于xml数据文件输入/输出部分
*
*
*@note 如果a_iCutOffVersion为0 ，则安装剪裁到最高版本
*@note 如果a_iXmlDataFormat指定除TDR_XML_DATA_FORMAT_LIST_MATA_NAME，TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME，
	TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME以外的值，则强制按照格式TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME进行处理
*note 关于xml数据文件的具体格式说明，请参考文档<TSF-G-DR-FAQ.doc> 
*中“Xml数据文件输入/输出篇” -> “Tdr支持什么格式的xml数据文件？”
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码：
*	- TDR_ERROR_INVALID_PARAM	参数无效
*		- TDR_ERROR_INVALID_CUTOFF_VERSION 指定剪裁版本比元数据基准版本要小
*		- TDR_ERROR_NO_HOSTBUFF_SPACE 本地存储缓冲区剩余空间不够
*		- TDR_ERROR_TOO_COMPLIEX_META 元数据描述复合数据类型嵌套层次超过32层
*		- TDR_ERROR_NET_UNSUPPORTED_TYPE    不支持的元数据类型
*		- TDR_ERROR_NET_INVALID_STRING_LEN string类型的元素其字符串长度超过了预定最大长度
*		- TDR_ERROR_NET_INVALID_VERSIONINDICATOR 元数据版本指示器的值比保存其值的元数据成员的版本要小
*
*
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*@pre \e a_pstMeta 不能为NULL
*@pre \e a_pstXml 不能为NULL
*@pre \e a_pstXml->pszBuff不能为NULL
*@pre \e a_pstXml->iBuff 必须大于0
*@pre \e a_pstHost 不能为NULL
*@pre \e a_pstHost->pszBuff不能为NULL
*@pre \e a_pstHost->iBuff 必须大于0
*/
TDR_API int tdr_output(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstXml, IN LPTDRDATA a_pstHost,
			   IN int a_iCutOffVersion, IN int a_iXmlDataFormat);

/**
*根据元数据描述将，可以把一个内存数据结构格式转化成一个符合特定规则的xml信息，并保存在指定文件中
*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
*@param[in]	a_fpXml 存储XML格式信息的文件句柄
*@param[in]	a_pstHost 存储本地内存数据结构信息的结构体指针
*	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
*	- 输入  a_pstHost.iBuff	指定缓冲区的大小
*@param[in] a_iCutOffVersion	要剪裁的版本号
*@param[in] a_iXmlDataFormat XML数据文件的格式,目前支持的格式有：
*	- TDR_XML_DATA_FORMAT_LIST_MATA_NAME	
*	- TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME 
*	-TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME
*	- 这几种格式的区别请参见文件 TSF4G-DR-FAQ.doc 关于xml数据文件输入/输出部分
*
*
*@note 如果a_iCutOffVersion为0 ，则安装剪裁到最高版本
*@note 如果a_iXmlDataFormat指定除TDR_XML_DATA_FORMAT_LIST_MATA_NAME，TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME，
TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME以外的值，则强制按照格式TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME进行处理
*
*note 关于xml数据文件的具体格式说明，请参考文档<TSF-G-DR-FAQ.doc> 
*中“Xml数据文件输入/输出篇” -> “Tdr支持什么格式的xml数据文件？”
* @pre \e a_pstMeta 不能为 NULL
* @pre \e a_fpXml 不能为 NULL
*@pre \e a_pstHost 不能为NULL
* @pre \e a_pstHost.pszBuff 不能为 NULL
* @pre \e a_pstHost.iBuff 必须大于0	
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码：
*	- TDR_ERROR_INVALID_PARAM	参数无效
*		- TDR_ERROR_INVALID_CUTOFF_VERSION 指定剪裁版本比元数据基准版本要小
*		- TDR_ERROR_NO_HOSTBUFF_SPACE 本地存储缓冲区剩余空间不够
*		- TDR_ERROR_TOO_COMPLIEX_META 元数据描述复合数据类型嵌套层次超过32层
*		- TDR_ERROR_NET_UNSUPPORTED_TYPE    不支持的元数据类型
*		- TDR_ERROR_NET_INVALID_STRING_LEN string类型的元素其字符串长度超过了预定最大长度
*		- TDR_ERROR_NET_INVALID_VERSIONINDICATOR 元数据版本指示器的值比保存其值的元数据成员的版本要小
*
*
* @see tdr_output
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_output_fp(IN LPTDRMETA a_pstMeta, IN FILE *a_fpXml, IN LPTDRDATA a_pstHost,
			   IN int a_iCutOffVersion, IN int a_iXmlDataFormat);

/**
*根据元数据描述将，可以把一个内存数据结构格式转化成一个符合特定规则的xml信息，并保存在指定文件中
*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
*@param[in]	a_szFile 存储XML格式信息的文件的文件名字符串指针
*@param[in]	a_pstHost 存储本地内存数据结构信息的结构体指针
*	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
*	- 输入  a_pstHost.iBuff	指定缓冲区的大小
*@param[in] a_iCutOffVersion	要剪裁的版本号
*@param[in] a_iXmlDataFormat XML数据文件的格式,目前支持的格式有：
*	- TDR_XML_DATA_FORMAT_LIST_MATA_NAME	
*	- TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME 
*	-TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME
*	- 这几种格式的区别请参见文件 TSF4G-DR-FAQ.doc 关于xml数据文件输入/输出部分
*
*
*@note 如果a_iCutOffVersion为0 ，则安装剪裁到最高版本
*@note 如果a_iXmlDataFormat指定除TDR_XML_DATA_FORMAT_LIST_MATA_NAME，TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME，
TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME以外的值，则强制按照格式TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME进行处理
*note 关于xml数据文件的具体格式说明，请参考文档<TSF-G-DR-FAQ.doc> 
*中“Xml数据文件输入/输出篇” -> “Tdr支持什么格式的xml数据文件？”
*
*@pre \e a_pstMeta 不能为NULL
*@pre \e a_szFile 不能为NULL
*@pre \e a_pstHost 不能为NULL
*@pre \e a_pstHost->pszBuff不能为NULL
*@pre \e a_pstHost->iBuff 不能为NULL
*
* @see tdr_output
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_output_file(IN LPTDRMETA a_pstMeta, IN const char *a_szFile, IN LPTDRDATA a_pstHost,
				  IN int a_iCutOffVersion, IN int a_iXmlDataFormat);

/**根据元数据描述将符合特定规则的xml信息转化成本地内存数据结构格式
*xml信息保存在指定缓冲区中
*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
*@param[in,out]	a_pstHost 存储本地内存数据结构信息的结构体指针
*	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
*	- 输入  a_pstHost.iBuff	指定缓冲区的大小
*	- 输出  a_pstHost.iBuff  实际生成的本地结构信息的总长度
*@param[in]	a_pstXml 存储XML格式信息的结构
*	- 输入	a_pstXml.pszBuff 指定保存XML信息的缓冲区起始地址
*	- 输入  a_pstXml.iBuff	指定缓冲区的大小
*@param[in] a_iCutOffVersion	要剪裁的版本号
*@param[in] a_iXmlDataFormat XML数据文件的格式,目前支持的格式有：
*	- TDR_XML_DATA_FORMAT_LIST_MATA_NAME	
*	- TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME 
*	-TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME
*	- 这几种格式的区别请参见文件 TSF4G-DR-FAQ.doc 关于xml数据文件输入/输出部分
*
*
*@note 如果a_iCutOffVersion为0 ，则安装剪裁到最高版本
*@note 如果a_iXmlDataFormat指定除TDR_XML_DATA_FORMAT_LIST_MATA_NAME，TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME，
TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME以外的值，则强制按照格式TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME进行处理
*note 关于xml数据文件的具体格式说明，请参考文档<TSF-G-DR-FAQ.doc> 
*中“Xml数据文件输入/输出篇” -> “Tdr支持什么格式的xml数据文件？”
*@note 本接口不支持versionindicator属性
*@note 本接口不支持 指针和引用 属性
*
* @pre \e a_pstMeta 不能为 NULL
* @pre \e a_pstXml 不能为 NULL
* @pre \e a_pstXml.pszBuff 不能为 NULL
* @pre \e a_pstXml.iBuff 必须大于0
* @pre \e a_pstXml 不能为 NULL
* @pre \e a_pstHost.pszBuff 不能为 NULL
* @pre \e a_pstHost.iBuff 必须大于0	
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码：
*	- TDR_ERROR_INVALID_PARAM	参数无效
*	-	TDR_ERROR_NO_XML_ROOT	没有合适的根节点，请确保XML元素树中包含以元数据名为根的节点
*
* @see  tdr_output
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_input(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost, IN LPTDRDATA a_pstXml, 
			   IN int a_iCutOffVersion, IN int a_iXmlDataFormat);

/**根据元数据描述将符合特定规则的xml信息转化成本地内存数据结构格式
*xml信息保存在指定文件中
*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
*@param[in,out]	a_pstHost 存储本地内存数据结构信息的结构体指针
*	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
*	- 输入  a_pstHost.iBuff	指定缓冲区的大小
*	- 输出  a_pstHost.iBuff  实际生成的本地结构信息的总长度
*@param[in]	a_fp 存储XML格式信息的文件句柄
*@param[in] a_iCutOffVersion	要剪裁的版本号
*@param[in] a_iXmlDataFormat XML数据文件的格式,目前支持的格式有：
*	- TDR_XML_DATA_FORMAT_LIST_MATA_NAME	
*	- TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME 
*	-TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME
*	- 这几种格式的区别请参见文件 TSF4G-DR-FAQ.doc 关于xml数据文件输入/输出部分
*
*
*@note 如果a_iCutOffVersion为0 ，则安装剪裁到最高版本
*@note 如果a_iXmlDataFormat指定除TDR_XML_DATA_FORMAT_LIST_MATA_NAME，TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME，
TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME以外的值，则强制按照格式TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME进行处理
*note 关于xml数据文件的具体格式说明，请参考文档<TSF-G-DR-FAQ.doc> 
*中“Xml数据文件输入/输出篇” -> “Tdr支持什么格式的xml数据文件？”
*
*@note 本接口不支持versionindicator属性，
*@note 本接口不支持 指针和引用 属性

* @pre \e a_pstMeta 不能为 NULL
* @pre \e a_fp 不能为 NULL
* @pre \e a_psHost 不能为NULL
* @pre \e a_pstHost.pszBuff 不能为 NULL
* @pre \e a_pstHost.iBuff 必须大于0	
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码：
*	-	TDR_ERROR_NO_XML_ROOT	没有合适的根节点，请确保XML元素树中包含以元数据名为根的节点
*
* @see  tdr_output
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_input_fp(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost, IN FILE *a_fp, 
			  IN int a_iCutOffVersion, IN int a_iXmlDataFormat);

/**根据元数据描述将符合特定规则的xml信息转化成本地内存数据结构格式
*xml信息保存在指定文件中
*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
*@param[in,out]	a_pstHost 存储本地内存数据结构信息的结构体指针
*	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
*	- 输入  a_pstHost.iBuff	指定缓冲区的大小
*	- 输出  a_pstHost.iBuff  实际生成的本地结构信息的总长度
*@param[in]	a_pszFile 存储XML格式信息的文件的文件名字符串指针
*@param[in] a_iCutOffVersion	要剪裁的版本号
*@param[in] a_iXmlDataFormat XML数据文件的格式,目前支持的格式有：
*	- TDR_XML_DATA_FORMAT_LIST_MATA_NAME	
*	- TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME 
*	-TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME
*	- 这几种格式的区别请参见文件 TSF4G-DR-FAQ.doc 关于xml数据文件输入/输出部分
*
*
*@note 如果a_iCutOffVersion为0 ，则安装剪裁到最高版本
*@note 如果a_iXmlDataFormat指定除TDR_XML_DATA_FORMAT_LIST_MATA_NAME，TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME，
TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME以外的值，则强制按照格式TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME进行处理
*note 关于xml数据文件的具体格式说明，请参考文档<TSF-G-DR-FAQ.doc> 
*中“Xml数据文件输入/输出篇” -> “Tdr支持什么格式的xml数据文件？”
*@note 本接口不支持versionindicator属性
*@note 本接口不支持 指针和引用 属性
*
* @pre \e a_pstMeta 不能为 NULL
* @pre \e a_pszFile 不能为 NULL
* @pre \e a_psHost 不能为NULL
* @pre \e a_pstHost.pszBuff 不能为 NULL
* @pre \e a_pstHost.iBuff 必须大于0	
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码：
*	- TDR_ERROR_INVALID_PARAM	参数无效
*	-	TDR_ERROR_NO_XML_ROOT	没有合适的根节点，请确保XML元素树中包含以元数据名为根的节点
*
* @see  tdr_output
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_input_file(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost, IN const char *a_pszFile, 
				   IN int a_iCutOffVersion, IN int a_iXmlDataFormat);
/**@}*/ /*defgroup  TDR_DATA_IO TDR_数据以XML格式输入/输出 */

/**@defgroup  TDR_PRINT_DATA TDR_数据打印
* @{
*/

/**根据元数据描述将本地内存数据结构以可视化的方式输出
*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
*@param[in]	a_fp 存储可视化数据信息的文件句柄
*@param[in]	a_pstHost 存储本地内存数据结构信息的结构体指针
*	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
*	- 输入  a_pstHost.iBuff	指定缓冲区的大小
*@param[in] a_iCutOffVersion	要剪裁的版本号
*@note 如果a_iCutOffVersion为0 ，则安装剪裁到最高版本
*
* @pre \e a_pstMeta 不能为 NULL
* @pre \e a_fp 不能为 NULL
* @pre \e a_psHost 不能为NULL
* @pre \e a_pstHost.pszBuff 不能为 NULL
* @pre \e a_pstHost.iBuff 必须大于0	
*
*@retval 0   处理成功
*@retval >0  处理成功，但发生某些特殊情况
*@retval <0  处理失败，返回表示出错信息的错误代码
*
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_fprintf(IN LPTDRMETA a_pstMeta, IN FILE  *a_fp, INOUT LPTDRDATA a_pstHost, IN int a_iCutOffVersion);

/**根据元数据描述将本地内存数据结构以可视化的方式输出到字符串缓冲区中
*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
*@param[in,out]	a_pstOut 存储可视化数据的结构体指针
*	- 输入	a_pstOut.pszBuff 指定保存存储可视化数据的信息的缓冲区起始地址
*	- 输入  a_pstOut.iBuff	指定缓冲区的大小
*	- 输出  a_pstOut.iBuff  实际生成的存储可视化数据的信息的总长度
*@param[in]	a_pstHost 存储本地内存数据结构信息的结构体指针
*	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
*	- 输入  a_pstHost.iBuff	指定缓冲区的大小
*@param[in] a_iCutOffVersion	要剪裁的版本号
*@note 如果a_iCutOffVersion为0 ，则安装剪裁到最高版本
*
* @pre \e a_pstMeta 不能为 NULL
* @pre \e a_pstHost 不能为 NULL
* @pre \e a_pstHost.pszBuff 不能为 NULL
* @pre \e a_pstHost.iBuff 必须大于0	
* @pre \e a_pstOut 不能为 NULL
* @pre \e a_pstOut.pszBuff 不能为 NULL
* @pre \e a_pstOut.iBuff 必须大于0
*
*@retval 0   处理成功
*@retval >0  处理成功，但发生某些特殊情况
*@retval <0  处理失败，返回表示出错信息的错误代码
*
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_sprintf(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstOut, IN LPTDRDATA a_pstHost, IN int a_iCutOffVersion);

/**@}*/ /* @defgroup  TDR_PRINT_DATA TDR_数据打印*/

#ifdef __cplusplus
}
#endif

#endif /* TDR_DATA_IO_H */
