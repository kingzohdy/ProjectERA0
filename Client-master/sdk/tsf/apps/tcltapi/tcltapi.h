/*
**  @file $RCSfile: tcltapi.h,v $
**  general description of this module
**  $Id: tcltapi.h,v 1.1.1.1 2009/07/23 04:22:48 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2009/07/23 04:22:48 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TCLTAPI_H
#define TCLTAPI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "tdr/tdr.h"

#define TCLTAPI_ERR_NET			1
#define TCLTAPI_ERR_TDR			2
#define TCLTAPI_ERR_BUF			3


struct tagTCltapiHandle;
typedef struct tagTCltapiHandle 	TCLTAPIHANDLE;
typedef struct tagTCltapiHandle 	*HTCLTAPI;

int tcltapi_make_connect(const char* a_pszUri, int a_iTimeout);

int tcltapi_new(HTCLTAPI* a_phClt);

int tcltapi_free(HTCLTAPI* a_phClt);

int tcltapi_detach(HTCLTAPI a_hClt);

void tcltapi_attach(HTCLTAPI a_hClt, int a_s);

int tcltapi_open(const char* a_pszUri, int a_iTimeout, HTCLTAPI* a_phClt);

int tcltapi_set_pdu_meta(HTCLTAPI a_hClt, LPTDRMETA a_pstRecvMeta, LPTDRMETA a_pstSendMeta);

void tcltapi_set_version(HTCLTAPI a_hClt, int a_iSelfVersion, int a_iPeerVersion);

int tcltapi_send_buff(HTCLTAPI a_hClt, const char* a_pszBuff, int a_iBuff, int a_iTimeout);

int tcltapi_recv_buff(HTCLTAPI a_hClt, char* a_pszBuff, int a_iBuff, int a_iTimeout);

int tcltapi_send_msg(HTCLTAPI a_hClt, const char* a_pszMsg, int a_iMsg, int a_iTimeout);

int tcltapi_recv_msg(HTCLTAPI a_hClt, char* a_pszMsg, int* a_piMsg, int a_iTimeout);

int tcltapi_get_sock(HTCLTAPI a_hClt);

int tcltapi_set_recvbuff(HTCLTAPI a_hClt, int a_iSize);

int tcltapi_set_sendbuff(HTCLTAPI a_hClt, int a_iSize);

int tcltapi_get_err(HTCLTAPI a_hClt);
char *tcltapi_get_errstring(HTCLTAPI a_hClt);





#ifdef __cplusplus
}
#endif

#endif /* TCLTAPI_H */


