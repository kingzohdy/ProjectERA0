/**
*
* @file     tbus_dyemsg.h 
* @brief    tbus染色相关接口
* 
* @author jackyai  
* @version 1.0
* @date 2008-05-16 
*
*
* All rights reserved.
*
*/

#ifndef TBUS_DYEMSG_H
#define TBUS_DYEMSG_H

#include "tbus/tbus.h"
#include "tbus_head.h"


/** 根据染色标志位和前一个消息对消息进行染色
*/
int tbus_dye_pkg(IN LPTBUSHEAD a_pstHead, IN LPTBUSHEAD a_pstPreHead, int a_iFlag);

/**
@brief 对染色的消息进行记录
@param
@note
*/
void tbus_log_dyedpkg(IN LPTBUSHEAD a_pstHead, IN const char *a_pszOperation) ;

#endif
