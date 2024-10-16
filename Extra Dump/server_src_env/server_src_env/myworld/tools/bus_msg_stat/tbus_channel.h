/** @file $RCSfile: tbus_channel.h,v $
  Channel function header file for tsf4g-tbus module
  $Id: tbus_channel.h,v 1.1 2013/01/17 08:45:57 kent Exp $
@author $Author: kent $
@date $Date: 2013/01/17 08:45:57 $
@version $Revision: 1.1 $
@note Editor: Vim 6.3, Gcc 4.0.2, tab=4
@note Platform: Linux
*/


#ifndef _TBUS_CHANNEL_H
#define _TBUS_CHANNEL_H

#include "tbus/tbus.h"
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
	int tbus_enable_addr (
		IN const TBUSADDR a_iAddr,
		IN  TBUSSHMCHANNELCNF *a_ptRoute,
		IN LPTBUSHANDLE a_pstBusHandle,
		IN LPTBUSSHMGCIMHEAD a_pstHead
		) ;


	/**
	@brief
	@retval 0 --
	@retval !0 --
	@note
	*/
	int tbus_attach_channel (
		IN const TBUSADDR a_iAddr,
		IN  TBUSSHMCHANNELCNF *a_ptRoute,
		INOUT TBUSCHANNEL *a_ptChannel,
		IN LPTBUSSHMGCIMHEAD a_pstHead
		) ;





int tbus_push_channel_pkgv(IN TBUSCHANNEL* a_pstChannel, IN LPTBUSHEAD a_pstHead, 
						   IN const struct iovec *a_ptVector, IN const int a_iVecCnt) ;

int	tbus_get_channel_pkgv(IN TBUSCHANNEL* a_pstChannel, OUT LPTBUSHEAD a_pstHead,
						  IN char *a_pszData, INOUT int *a_piLen);


int	tbus_peek_channel_pkgv(IN TBUSCHANNEL* a_pstChannel, OUT LPTBUSHEAD a_pstHead,
						   IN const char **a_ppszData, OUT int *a_piLen);


int tbus_delete_channel_headpkg(IN TBUSCHANNEL* a_pstChannel);

int tbus_moveto_next_pkg(IN IN LPTBUSHEAD a_pstHead, CHANNELHEAD *a_pstChlHead, IN unsigned int dwSize,
						 INOUT unsigned int *a_pdwHead, IN unsigned int dwTail);

/** 读取数据通道消息输入队列指定位置所在消息的序列号
@param[in] a_pstChannel tbus通道数据结构的指针
@param[in] a_dwPos 消息在队列中的起始位置
*/
int tbus_get_pkgseq_by_pos(IN TBUSCHANNEL* a_pstChannel, IN unsigned int a_dwPos);

#ifdef __cplusplus
}
#endif


#endif /**< _TBUS_CHANNEL_H */

