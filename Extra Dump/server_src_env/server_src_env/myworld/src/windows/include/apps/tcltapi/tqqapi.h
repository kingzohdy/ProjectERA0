/*
**  @file $RCSfile: tqqapi.h,v $
**  general description of this module
**  $Id: tqqapi.h,v 1.1.1.1 2009/07/23 04:22:48 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2009/07/23 04:22:48 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TQQAPI_H
#define TQQAPI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "pal/pal.h"
#include "tdr/tdr.h"

#include "apps/tcltapi/tqqdef.h"
#include "apps/tcltapi/tpdudef.h"


#ifndef IN
#define  IN
#endif

#ifndef OUT
#define  OUT
#endif

#ifndef INOUT
#define INOUT
#endif

#define TQQE_ERROR			       -1
#define TQQE_CONN			       -2
#define TQQE_DECRYPT			-3
#define TQQE_PASSWD			-4


typedef enum
{
         TQQAPI_ERR_NETWORK_FAILURE          =1,
         TQQAPI_ERR_TDR_NTOH_HEAD             =2,
         TQQAPI_ERR_NOT_ENOUGHBUF             =3,
         TQQAPI_ERR_DECRYPTION_HEAD          =4,
         TQQAPI_ERR_TDR_NTOH_BODY              =5,
         TQQAPI_ERR_TDR_HTON_HEAD             =6,
         TQQAPI_ERR_TDR_HTON_BODY             =7,
         TQQAPI_ERR_RECV_TIMEOUT                =8,
         TQQAPI_ERR_DECRYPTION_BODY          =9,
         TQQAPI_ERR_INVALID_CMD                  =10,
         TQQAPI_ERR_TDR_HTON_USERIDENT   =11,
         TQQAPI_ERR_BUILD_AUTH                    =12,
         TQQAPI_ERR_BUILD_SYNACK                 =13,
         TQQAPI_ERR_COUNT
}TQQAPI_ERR;






struct tagTQQApiHandle;

typedef struct tagTQQApiHandle		TQQAPIHANDLE;
typedef struct tagTQQApiHandle		*HTQQAPI;
extern LPTDRMETA g_pstqqapiPduHead;

#define TQQAPI_PDUHEAD_HTON(net, host, ver)		tdr_hton(g_pstqqapiPduHead, net,host, ver)
#define TQQAPI_PDUHEAD_NTOH(host,net, ver)		tdr_ntoh(g_pstqqapiPduHead, host, net, ver)


/****** For tenio client use **********************************************************************
*Function description: If use decode and encode funtion,this fuction should be called first 
*parameter:
*return value:
*   <0:fail
*   =0:success
*/
int  tqqapi_create();

/***************************************************************************
*Function description: decode pduhead    
*Parameter:
*IN         pszBuff:Recv package
*IN         iBuff:Recv package net len
*OUT      pstHead:package head info
*OUT      piHeadLen: package head net len
*return value: 
*         = 0 for success
*         <0 failed
*/
int tqqapi_decode(IN const char* pszBuff,IN int iBuff,OUT TPDUHEAD* pstHead,OUT int* piHeadLen);


/***************************************************************************
*Function description:encode msg     
*parameter:
*INOUT        pszBuff:IN-Package Send  buffer 
*                             OUT-package head
*INOUT       iBuff:IN-package send buffer len
*                        OUT-package head net len
*IN           pstHead:package head info
*return value: 
*           =0 for success
*           <0 failed
*/
int tqqapi_encode(INOUT char* pszBuff,INOUT int* piBuff, IN TPDUHEAD* pstHead);
// tenio client use end




//public

/********For Game Client use*******************************************************************
*Function description: this fuction used connect server and assign a handle      
*Function parameter: 
* IN   a_pszUri:IP and port address
* IN   a_iTimeout:connecton time out.unit:millisecond.-1 means always wait until connected
* OUT a_phQQClt:connection handle
*return value: socket file descriptor
*       >=0 success
*       =-1  failed
****************************************************************************/
int tqqapi_open(IN const char* a_pszUri,IN int a_iTimeout,OUT HTQQAPI* a_phQQClt);


/***************************************************************************
*Function description: this fuction used to  allocate a connect instance and return the handle   
*Function parameter: 
*OUT    a_phQQClti:connection handle
*return value: 
*             =0 for success
*             <0 failed
****************************************************************************/
int tqqapi_new(OUT HTQQAPI* a_phQQClt);


/***************************************************************************
*Function description: attach connection socket for handle    
*Function parameter: 
* IN            a_s:connection socket file descriptor 
* IN           a_hQQClt:connection handle
****************************************************************************/
void tqqapi_attach(IN HTQQAPI a_hQQClt,IN int a_s);


/***************************************************************************
*Function description: this fuction used to allocate send buffer and recv buffer for connection handle
*Function parameter: 
* IN   a_hQQClt:connection handle
* IN   a_pstSendMeta:Send Meta Of Msg
* IN   a_pstRecvMeta:Receive Mete Of Msg
*return value: =0 for success
*                    <0 failed
****************************************************************************/
int tqqapi_set_pdu_meta(IN HTQQAPI a_hQQClt,IN LPTDRMETA a_pstSendMeta,IN  LPTDRMETA a_pstRecvMeta);


/***************************************************************************
*Function description: this funcion used to set gamekey of connection handle
*Function parameter: 
* IN   a_hQQClt:handle
* IN   pszGameKey:GameKey:16 byte
****************************************************************************/
void tqqapi_set_gamekey(IN HTQQAPI a_hQQClt,IN  const char* pszGameKey);


/***************************************************************************
*Function description: this funcion used to get gamekey of connection handle
*Function parameter: 
* IN        a_hQQClti:connection handle
* OUT  pszGameKey:GameKey:16 byte
****************************************************************************/
int   tqqapi_get_gamekey(IN  HTQQAPI a_hQQClt, OUT char* pszGameKey);




/***************************************************************************
*Function description: this fuction used to set tdr  version of send/recv msg meta      
*Function parameter: 
*IN     a_hQQClt:API Handle
*IN     a_iSelfVersion:local version
*IN     a_iPeerVersion:peer version
*return value: 
****************************************************************************/
void tqqapi_set_version(IN HTQQAPI a_hQQClt,IN  int a_iSelfVersion,IN int a_iPeerVersion);



/***************************************************************************
*Function description: this fuction used to set identity info,should be call for relay    
*Function parameter: 
*IN    a_hQQClt:API Handle
*IN    a_iPos:index of  conenction instance
*IN    a_pszIdentity:16bytes indentiry
****************************************************************************/
void tqqapi_set_identity(IN HTQQAPI a_hQQClt,IN  int a_iPos,IN  const char * a_pszIdentity);

/***************************************************************************
*Function description: this fuction used to get identity info,should be call for relay    
*Function parameter: 
*IN       a_hQQClt:API Handle
*OUT    a_iPos:index of  conenction instance
*OUT    a_pszIdentity:16bytes indentiry
****************************************************************************/
void tqqapi_get_identity(IN HTQQAPI a_hQQClt,OUT int *a_iPos,OUT char * a_pszIdentity);


/***************************************************************************
*Function description: this fuction used to seed msg     
*Function parameter: 
*IN         a_hQQClt:connection handle
*IN         a_pstHead: null means default package head 
*IN         a_pszMsg: package body(LocalHost) 
*IN          a_iMsg:package body len
*IN          a_iTimeout:send timeout,this  value can be small (Generally set to 10 can do,if network crowed,prolong to 100 or more) unit:millisecond
*return value:
*            0:for success
*           <0:for fail
*                use tqqapi_get_err
****************************************************************************/
int tqqapi_send_msg(IN HTQQAPI a_hQQClt, IN TPDUHEAD* a_pstHead,IN const char* a_pszMsg,IN int a_iMsg,IN int a_iTimeout);



/***************************************************************************
*Function description: this fuction used to receive msg     
*Function parameter: 
*IN      a_hQQClt:connection handle
*OUT    a_pstHead:Package head,
*INOUT    a_pszMsg:IN:package body recv buffer:
*                            OUT:package body buffer(local host)
*INOUT    a_iMsg:IN:package body recv buffer len:
*                        OUT:package body len
*IN          a_iTimeout:recv timeout (Generally set to 100 can do,if network crowed,prolong to 500 or more) unit:millisecond
*return value:
*             1:receive one package
*             0:receive NULL
*            <0:for fail
****************************************************************************/
int tqqapi_recv_msg(IN HTQQAPI a_hQQClt,OUT TPDUHEAD* a_pstHead,INOUT char* a_pszMsg,INOUT int* a_piMsg,IN int a_iTimeout);



/***************************************************************************
*Function description: this fuction used to seed buffer     
*Function parameter: 
*IN         a_hQQClt:connection handle
*IN         a_pstHead: null means default package head 
*IN         a_pszBuffer: package body(packed:net order) 
*IN          iBufferLen:package body len
*IN          a_iTimeout:send timeout,this  value can be small (Generally set to 10 can do,if network crowed,prolong to 100 or more) unit:millisecond
*return value:
*            0:for success
*           <0:for fail
*                use tqqapi_get_err
****************************************************************************/
int tqqapi_send_buffer(IN HTQQAPI a_hQQClt, IN TPDUHEAD* a_pstHead,IN const char* a_pszBuffer,IN int iBufferLen,IN int a_iTimeout);



/***************************************************************************
*Function description: this fuction used to recv  buffer     
*Function parameter: 
*IN      a_hQQClt:connection handle
*OUT    a_pstHead:Package head,
*INOUT    a_pszMsg:IN:package body recv buffer:
*                            OUT:package body buffer(unpacked:net oder)
*INOUT    a_iMsg:IN:package body recv buffer len:
*                        OUT:package body len
*IN          a_iTimeout:recv timeout (Generally set to 100 can do,if network crowed,prolong to 500 or more) unit:millisecond
*return value:
*             1:receive one package
*             0:receive NULL
*            <0:for fail
****************************************************************************/
int tqqapi_recv_buffer(IN HTQQAPI a_hQQClt, IN TPDUHEAD* a_pstHead,IN const char* a_pszBuffer,INOUT int * piBufferLen,IN int a_iTimeout);

/***************************************************************************
*Function description: this fuction used to get Hande Error Code     
*Function parameter: 
*IN          a_hQQClt:handle
*return value:
*           error code
****************************************************************************/
int tqqapi_get_err(IN HTQQAPI a_hQQClt);


/***************************************************************************
*Function description: this fuction used to get Hande Error string     
*Function parameter: 
*IN          a_hQQClt:handle
*return value:
*           error string 
****************************************************************************/
const char * tqqapi_get_errstring(IN HTQQAPI a_hQQClt);

/***************************************************************************
*Function description: this fuction must be called  to fanalize Hande 
*Function parameter: 
*IN          a_hQQClt:handle
*return value:
*           0:for suceess
*          <0:for fail
****************************************************************************/
int tqqapi_free(IN HTQQAPI* a_phQQClt);


/***************************************************************************
*Function description: this fuction used to create initial connection to tconnd,which means signature verify needed
*Fuciont: used by client for test number debug    
*Function parameter: 
*IN          a_phQQClt:Connection Handle
*IN          uin:QQ number
*IN          pszSvrkey:test server key,16byte
*IN          iAuthType:Sig type (set to 3 default),refer tpdudef.h for detail
*IN          a_iTimeOut:connection create time out value. Generally set to 500,if network crowed.prolong to longer. unit/milli second,
*return value:
*             0:for success
*            <0:for fail
****************************************************************************/
int tqqapi_create_initial_connection(IN HTQQAPI a_phQQClt,IN long uin, IN const char *pszSvrkey,IN int iAuthType,IN int a_iTimeOut);





/***************************************************************************
*Function description: this fuction used to create relay connection to tconnd,which means has been verified
*Funciont:used by client 
*Function parameter: 
*IN          a_phQQClt:Connection Handle
*IN          uin:QQ number
*IN          nPos:connetion idx in tconnd
*IN          szConnectKey:16 byte connectkey
*IN          szIdentity:16 byte identity
*IN          iRelayType:relay type(set to 1 default ),refer tpdudef.h for detail
*IN          a_iTimeOut:connection create time out value. Generally set to 500,if network crowed.prolong to longer. unit/milli second,
*return value:
*             0:for success
*            <0:for fail
****************************************************************************/
int tqqapi_create_relay_connection(IN HTQQAPI a_phQQClt,IN long uin,IN int nPos,IN const char * szConnectKey,IN const char *szIdentity,IN int a_iTimeOut,IN int iRelayType);




/***************************************************************************
*Function description: this fuction used to get socket fd
*Funciont:used by client 
*Function parameter: 
*IN    a_phQQClt:Handle
*return value: socket fd
*             
****************************************************************************/
int tqqapi_get_sock(IN HTQQAPI a_phQQClt);

/***************************************************************************
*Function description: this fuction used to set socket recv buff
*Funciont:used by client 
*Function parameter: 
*IN    a_phQQClt:Handle
*IN    a_iSize:recv buff size
*return value:
*             0:for success
*            <0:for fail          
****************************************************************************/
int tqqapi_set_recvbuff(IN HTQQAPI a_phQQClt, IN int a_iSize);

/***************************************************************************
*Function description: this fuction used to set socket send buff
*Funciont:used by client 
*Function parameter: 
*IN    a_phQQClt:Handle
*IN    a_iSize:send buff size
*return value:
*             0:for success
*            <0:for fail          
****************************************************************************/
int tqqapi_set_sendbuff(IN HTQQAPI a_phQQClt,IN  int a_iSize);


/***************************************************************************
*Function description: this fuction used to judge wheather the connection  can be resumed due to temporary network failure
*Function parameter: 
*IN    a_phQQClt:Handle
*return value:
*             0:success:relay enabled
*            <0:fail:relay disabled         
****************************************************************************/
int tqqapi_IsRelay_Enable(IN HTQQAPI a_phQQClt);




//for client use end

//public







//private Interface  for internal use
void              tqqapi_init_base(TPDUBASE* a_pstBase);
const char*   tqqapi_get_meta_data(void);
char *           tqqapi_randstr(char* pszBuff, int iLen);
int                tqqapi_detach(HTQQAPI a_hQQClt);
int                tqqapi_extract_identinfo(HTQQAPI a_hQQClt, TPDUHEAD* a_pstHead, TPDUIDENTINFO* pstIdentInfo);
int                tqqapi_extract_Sessionkey(HTQQAPI a_hQQClt, TPDUHEAD* a_pstHead,char *pszSessionkey);
int                tqqapi_extract_syninfo(HTQQAPI a_hQQClt,TPDUHEAD* a_pstHead);
int                tqqapi_make_authinfo(TQQAUTHINFO* pstAuthInfo, TQQGAMESIG* pstGameSig, TQQSIGFORS2* pstSigForS2, long uin, char* pszSvrSKey);
int                tqqapi_extract_authinfo(TQQGAMESIG* pstGameSig, TQQSIGFORS2* pstSigForS2, long uin, TQQAUTHINFO* pstAuthInfo,char* pszSvrSKey);
int                tqqapi_make_QQUnify_Authinfo(TQQUNIFIEDAUTHINFO *pstUnifyAuthInfo,long uin ,char* pszSvrSKey,char *pszSessionkey,unsigned int uCltIP);
int                tqqapi_extract_QQUnify_Authinfo(TQQUNIFIEDAUTHINFO *pstUnifyAuthInfo,char* pszSvrSKey);


//private  interface obsolete
int tqqapi_get_siginfo_by_socket(TQQSIGINFO* pstSigInfo, TQQUSERTOKEN* pstToken, int s, char* pszErr, int iErrLen, int iTimeout);
int tqqapi_get_siginfo_by_uri(TQQSIGINFO* pstSigInfo, TQQUSERTOKEN* pstToken, const char* pszUri, char* pszErr, int iErrLen, int iTimeout);
int tqqapi_get_siginfo(TQQSIGINFO* pstSigInfo, TQQUSERTOKEN* pstToken, char* pszErr, int iErrLen, int iTimeout);
int tqqapi_build_auth_msg(TPDUHEAD * a_pstHead, long uin, const char *pszSvrkey,int iAuthType,char *pszGameKey,unsigned int uCltIP);
int tqqapi_build_relay_msg(TPDUHEAD* a_pstHead,long uin,int nPos,const char * szConnectKey,const char *szIdentity,int iRelayType); 




int tqqapi_write_msg(IN HTQQAPI a_hQQClt, IN TPDUHEAD* a_pstHead,IN const char* a_pszMsg,IN int a_iMsg);

int tqqapi_read_msg(IN HTQQAPI a_hQQClt,OUT TPDUHEAD* a_pstHead,INOUT char* a_pszMsg,INOUT int* a_piMsg);

//private




#ifdef __cplusplus
}
#endif

#endif /* TQQAPI_H */


