/*
**  @file $RCSfile: tconnapi.h,v $
**  general description of this module
**  $Id: tconnapi.h,v 1.1.1.1 2011/01/05 10:59:28 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2011/01/05 10:59:28 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TCONNAPI_H
#define TCONNAPI_H

#include "tdr/tdr_types.h"
#include "proto_comm.h"
#include "proto_ss.h"

#ifndef IN
#define  IN
#endif

#ifndef OUT
#define  OUT
#endif

#ifndef INOUT
#define INOUT
#endif


extern LPTDRMETA g_pstConnapiFrameHead;

#define TCONNAPI_FRAMEHEAD_HTON(net, host, ver)		tdr_hton(g_pstConnapiFrameHead, net,host, ver)
#define TCONNAPI_FRAMEHEAD_NTOH(host,net, ver)		tdr_ntoh(g_pstConnapiFrameHead, host, net, ver)

#define TCONNAPI_MAX_BUFF		0x10000

#ifdef __cplusplus
extern "C"
{
#endif


/****** **********************************************************************
*Function description:this funcion used to init tbus and tdr meta info.should be called at first 
*parameter:
*   iKey:Bus share memory key.if set to 0,use 1688 by default
*return value:
*   <0:fail
*   =0:success
*/
int tconnapi_init(IN int iKey);


/****** **********************************************************************
*Function description:this funcion used to unwrap Frame Head of package
*parameter:
*   pszBuff:recv package of netorder
*   iBuff:recv package len
*   pstHead:frame head
*  piHeadLen:frame head net len
*return value:
*   <0:fail
*   =0:success
*/
int tconnapi_decode(IN const char* pszBuff,IN  int iBuff,OUT TFRAMEHEAD* pstHead,OUT int* piHeadLen);


/****** **********************************************************************
*Function description:this funcion used to wrap Frame Head of package
*parameter:
*   pszBuff: IN:package send  buffer
*               out:Frame Head of send  package
*   iBuff:IN:package send  buffer len 
*           OUT: net len of send package head
*   pstHead:frame head
*return value:
*   <0:fail
*   =0:success
*/
int tconnapi_encode(INOUT char* pszBuff,INOUT  int* piBuff, IN TFRAMEHEAD* pstHead);




/****** **********************************************************************
*Function description:this funcion used to fanalize tbus handle i
*/
void tconnapi_fini(void);


/****** **********************************************************************
*Function description:this funcion used to create send/recv handle and bind self bus ID
* parameter:
*     iProcID: BUS ID
*     piHandle:Handle
*return value:
*     <0:fail
*    =0:success
*/
int tconnapi_create(IN int iProcID, OUT int* piHandle);

/****** **********************************************************************
*Function description:this funcion used to free  send/recv handle
* parameter:
*     piHandle:Handle
*return value:
*/
void tconnapi_free(IN int* piHandle);

/****** **********************************************************************
*Function description:this funcion used to test whether the channel between local and peer is available
* parameter:
*     piHandle:Handle
*     iDst:dst bus id
*return value:
*    <0 fail
*     =0  success
*/
int tconnapi_connect(IN int iHandle,IN int iDst);

/****** **********************************************************************
*Function description:this funcion used to recv package from tbus
* parameter:
*     piHandle:Handle
*     piSrc:src bus id
*     pszBuff:IN: package body buffer
*                 OUT: package body msg
*     piBuff:IN:package body buffer len
*              OUT:package body msg net len
*    pstHead;package head info
*return value:
*    <0  recv  fail
*    =0   recv one package  
*/
int tconnapi_recv(IN int iHandle,IN int *piSrc,INOUT char* pszBuff,INOUT int* piBuff,OUT TFRAMEHEAD* pstHead);


/****** **********************************************************************
*Function description:this funcion used to send package to tbus
* parameter:
*     piHandle:Handle
*     iDst:dst bus id
*     pszBuff:package body
*     iBuff:package body net len
*    pstHead;package head info
*return value:
*    <0  send  fail
*     =0  success
*/
int tconnapi_send(IN int iHandle,IN  int iDst, IN char* pszBuff,IN  int iBuff,IN TFRAMEHEAD* pstHead);


/*private interface*/
int tconnapi_initialize(const char *a_pszGCIMKey, int a_iBusinessid);

#ifdef __cplusplus
}
#endif


#endif /* TCONNAPI_H */
