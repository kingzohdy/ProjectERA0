/** @file $RCSfile: tbus_misc.h,v $
  Misc functions header file for tsf4g-tbus relay module
  $Id: tbus_misc.h,v 1.1 2013/01/17 08:45:57 kent Exp $
@author $Author: kent $
@date $Date: 2013/01/17 08:45:57 $
@version $Revision: 1.1 $
@note Editor: Vim 6.3, Gcc 4.0.2, tab=4
@note Platform: Linux
*/



#ifndef _TBUS_MISC_H
#define _TBUS_MISC_H


#include "tbus_head.h"
#include "tbus_kernel.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */





/**
  @brief
  @retval 0 --
  @retval !0 --
  @note
*/
char *tbus_conv_hexstr ( IN const unsigned char *a_pszStr, IN const int a_iStrLen ) ;



/**
  @brief
  @retval 0 --
  @retval !0 --

  @note
*/
unsigned short tbus_head_checksum ( IN const TBUSHEAD *a_ptHead ) ;


/**
  @brief Show Hex val of a string, if a_iFlag = 1, show printable character as char
  @retval 0 --
  @retval !0 --

  @note
*/
void tbus_print_hexstr ( IN const unsigned char *a_pszStr, IN const int a_iLen, IN const int a_iFlag ) ;

LPTBUSHANDLE tbus_get_handle(IN int a_iHandle);

/**
@brief try to attach share memory via specified key and size. If no old share memory exists, this function
would create it automatically
*@param[in,out] a_ppvShm  pointer to saving shm address
*@param[out] a_piCreated  指明此共享内存是否是刚被创建的，其值为非零值表示此共享内存是刚被创建的，否则是之前
*@	已经存在的
@retval 0 -- attach exist share memory ok
@retval 1 -- create and attach share memory ok
@retval <0 -- failed
@note
*/
int tbus_auto_get_shm (INOUT void **a_ppvShm, OUT int *a_piCreated, IN const int a_iShmKey, IN  int *a_piSize, IN  int a_iFlag ) ;

int tbus_init_headmeta();

void tbus_trim_str( char *a_strInput );

int IsChannelInGCIM(CHANNELHEAD * a_pstHead, TBUSSHMGCIM *a_pstGCIM);



#ifdef __cplusplus
}
#endif


#endif /**< _TBUS_MISC_H */

