/**
*
* @file     tdr_data_initialize.h 
* @brief    TDR数据初始化/正则化
* 
* @author steve jackyai  
* @version 1.0
* @date 2007-06-05 
*
*
* Copyright (c)  2007, 腾讯科技有限公司互动娱乐研发部
* All rights reserved.
*
*/
#ifndef TDR_DATA_INITIALIZE_H
#define TDR_DATA_INITIALIZE_H


#include "tdr/tdr_types.h"



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @defgroup TDR_DATA_INIT TDR_数据初始化
*  @{
*/

/** 
*@brief 数据初始化
*根据元数据的缺省值约束对内存结构的相应成员赋初始值
*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
*@param[in]	a_pstHost 存储本地结构数据的结构体指针
*	- 输入	a_pstHost.pszBuff 指定保存本地结构数据的缓冲区起始地址
*	- 输入  a_pstHost.iBuff	指定缓冲区的大小
*@param[in] a_iVersion	要剪裁的版本号
*
*@note 如果a_pstMeta描述的union类型的结构,则不进行处理直接返回
*@note 如果a_iVersion为0 ，则安装剪裁到最高版本
*
* @pre \e a_pstHost.pszBuff缓冲区中必须是按照a_pstMeta描述的数据
* @pre \e a_pstMeta 不能为 NULL
* @pre \e a_pstHost 不能为 NULL
* @pre \e a_pstHost.pszBuff 不能为 NULL
* @pre \e a_pstHost.iBuff 必须大于0	
*
*@retval 0   处理成功
*@retval >0  处理成功，但发生某些特殊情况
*@retval <0  处理失败，返回表示出错信息的错误代码	
*
*
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
int tdr_init(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost, IN int a_iVersion);


/** @}*/ /* TDR_DATA_INIT TDR数据初始化*/


#ifdef __cplusplus
}
#endif

#endif /* TDR_DATA_INITIALIZE_H */
