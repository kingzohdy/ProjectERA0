/**
*
* @file     tdr_net.h 
* @brief    TDR元数据网络交换消息编解码模块
* 
* @author steve jackyai  
* @version 1.0
* @date 2007-04-28 
*
*
* Copyright (c)  2007, 腾讯科技有限公司互动娱乐研发部
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

/** @defgroup TDR_NETMSG_PACKUNPACK TDR_网络交换消息编/解码
* @{
*/

/**
	*按照指定版本根据元数据描述将本地存储数据转换成网络交换消息
	*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
	*@param[in,out]	a_pstNet 存储网络信息结构体指针
	*	- 输入	a_pstNet.pszBuff 指定保存网络信息的缓冲区起始地址
	*	- 输入  a_pstNet.iBuff	指定缓冲区的大小
	*	- 输出  a_pstNet.iBuff  实际编码网络信息的总长度
	*@param[in,out]	a_pstHost 存储本地结构信息的结构体指针
	*	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
	*	- 输入  a_pstHost.iBuff	指定缓冲区的大小
	*	- 输出  a_pstHost.iBuff  实际编码本地结构信息的总长度
	*@param[in] a_iVersion	要剪裁的版本号
	*
	*@note 如果a_iVersion为0 ，则安装剪裁到最高版本
	*@note 如果处理失败，获取错误信息的方法:
	*	- 1.根据返回值，调用tdr_error_string()可以获取出错信息
	*	- 2.由于a_pstHost.iBuff得到了实际编码的信息的总长度，以这个长度为偏移值，调用tdr_entry_off_to_path可以获取
	*		出错时成员的路径信息，从而定位编码具体在哪个位置出错
	*
	* @pre \e a_pstMeta 不能为 NULL
	* @pre \e a_pstNet 不能为 NULL
	* @pre \e a_pstNet.pszBuff 不能为 NULL
	* @pre \e a_pstNet.iBuff 必须大于0
	* @pre \e a_pstHost.pszBuff 不能为 NULL
	* @pre \e a_pstHost.iBuff 必须大于0	
	*
	*@retval 0   处理成功
	*@retval >0  处理成功，但发生某些特殊情况
	*@retval <0  处理失败，返回表示出错信息的错误代码：
	*		- TDR_ERROR_INVALID_CUTOFF_VERSION 指定剪裁版本比元数据基准版本要小
	*		- TDR_ERR_NET_NO_NETBUFF_SPACE 保存网络信息的缓冲区剩余空间不够
	*		- TDR_ERROR_NO_HOSTBUFF_SPACE 本地存储缓冲区剩余空间不够
	*		- TDR_ERROR_INVALID_REFER_VALUE		此元素的refer属性值不正确，其值不能为负数且必须比count属性值小
	*		- TDR_ERROR_TOO_COMPLIEX_META 元数据描述复合数据类型嵌套层次超过32层
	*		- TDR_ERROR_NET_UNSUPPORTED_TYPE    不支持的元数据类型
	*		- TDR_ERROR_NET_INVALID_STRING_LEN string类型的元素其字符串长度超过了预定最大长度
	*		- TDR_ERROR_NET_INVALID_VERSIONINDICATOR 元数据版本指示器的值比保存其值的元数据成员的版本要小
	*
	*
	* @see	tdr_get_meta_by_name
	* @see	tdr_get_meta_by_id
	* @see	tdr_error_string
	* @see	tdr_entry_off_to_path
	*/
TDR_API int tdr_hton(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstNet, INOUT LPTDRDATA a_pstHost, IN int a_iVersion);

/**
*按照指定版本根据元数据描述将网络交换消息转换成本地存储数据
*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
*@param[in,out]	a_pstHost 存储本地结构信息的结构体指针
*	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
*	- 输入  a_pstHost.iBuff	指定缓冲区的大小
*	- 输出  a_pstHost.iBuff  实际解码本地结构信息的总长度
*@param[in,out]	a_pstNet 存储网络信息结构体指针
*	- 输入	a_pstNet.pszBuff 指定保存网络信息的缓冲区起始地址
*	- 输入  a_pstNet.iBuff	指定缓冲区的大小
*	- 输出  a_pstNet.iBuff  实际解码网络信息的总长度
*@param[in] a_iVersion	要剪裁的版本号
*
*@note 如果a_iVersion为0 ，则安装剪裁到最高版本
*@note 如果处理失败，获取错误信息的方法:
	*	- 根据返回值，调用tdr_error_string()可以获取出错信息
	*	- 由于a_pstHost.iBuff得到了实际解码的信息的总长度，以这个长度为偏移值，调用tdr_entry_off_to_path可以获取
	*		出错时成员的路径信息，从而定位解码具体在哪个位置出错
	*	- 调用tdr_fprintf/tdr_sprintf可以打印出已经解码的内容
*
* @pre \e a_pstMeta 不能为 NULL
* @pre \e a_pstNet 不能为 NULL
* @pre \e a_pstNet.pszBuff 不能为 NULL
* @pre \e a_pstNet.iBuff 必须大于0
* @pre \e a_pstHost.pszBuff 不能为 NULL
* @pre \e a_pstHost.iBuff 必须大于0	
*
*@retval 0   处理成功
*@retval >0  处理成功，但发生某些特殊情况
*@retval <0  处理失败，返回表示出错信息的错误代码：
*		- TDR_ERROR_INVALID_CUTOFF_VERSION 指定剪裁版本比元数据基准版本要小
*		- TDR_ERR_NET_NO_NETBUFF_SPACE 保存网络信息的缓冲区剩余空间不够
*		- TDR_ERROR_NO_HOSTBUFF_SPACE 本地存储缓冲区剩余空间不够
*		- TDR_ERROR_INVALID_REFER_VALUE		此元素的refer属性值不正确，其值不能为负数且必须比count属性值小
*		- TDR_ERROR_TOO_COMPLIEX_META 元数据描述复合数据类型嵌套层次超过32层
*		- TDR_ERROR_NET_UNSUPPORTED_TYPE    不支持的元数据类型
*		- TDR_ERROR_NET_INVALID_STRING_LEN string类型的元素其字符串长度超过了预定最大长度
*
*
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
* @see	tdr_error_string
* @see	tdr_entry_off_to_path
* @see  tdr_fprintf
*/
TDR_API int tdr_ntoh(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost, INOUT LPTDRDATA a_pstNet, IN int a_iVersion);

/** @} */ /*@defgroup TDR_网络交换消息编/解码*/



#ifdef __cplusplus
}
#endif

#endif /* TSF_G_TDR_NET_H */
