/** @file $RCSfile: tsec.h,v $
  header file for tsf4g-security module
  $Id: tsec.h,v 1.1.1.1 2011/01/04 11:02:51 niko Exp $
@author $Author: niko $
@date $Date: 2011/01/04 11:02:51 $
@version 1.0
@note Editor: Vim 6.3, Gcc 4.0.2, tab=4
@note Platform: Linux
*/

#ifndef _TSEC_H
#define _TSEC_H

#include "sec/tsec_macros.h"
#include "sec/tsec_encrypt.h"
#include "sec/tsec_hash.h"
#include "sec/tsec_acrypto.h"
#include "sec/tsec_ip.h"

#include "sec/tsec_conf_desc.h"


typedef int (*TSEC_SO_INIT) (
  IN const char *argsbuf
) ;

typedef int (*TSEC_SO_INIT_EX) (
  IN int argc,
  IN char** argv,
  INOUT void* extbuf,
  INOUT ssize_t *extbufLen
);

typedef int (*TSEC_SO_PROC) (
  IN const char *argsbuf,
  IN const ssize_t argsLen,
  IN const void *data,
  IN const ssize_t dataLen,
  INOUT void *buffer,
  INOUT ssize_t *bufLen
) ;

typedef int (*TSEC_SO_PROC_EX) (
  IN const char *argsbuf,
  IN const ssize_t argsLen,
  IN const void *data,
  IN const ssize_t dataLen,
  INOUT void *buffer,
  INOUT ssize_t *bufLen,
  INOUT void *extbuf,
  INOUT ssize_t *extbufLen
) ;

typedef int (*TSEC_SO_DEPROC) (
  IN const char *argsbuf,
  IN const ssize_t argsLen,
  IN const void *data,
  IN const ssize_t dataLen,
  INOUT void *buffer,
  INOUT ssize_t *bufLen
) ;

typedef int (*TSEC_SO_DEPROC_EX) (
  IN const char *argsbuf,
  IN const ssize_t argsLen,
  IN const void *data,
  IN const ssize_t dataLen,
  INOUT void *buffer,
  INOUT ssize_t *bufLen,
  INOUT void *extbuf,
  INOUT ssize_t *extbufLen
) ;

typedef int (*TSEC_SO_FINI) () ;

typedef int (*TSEC_SO_FINI_EX) (
  INOUT void *extbuf,
  INOUT ssize_t *extbufLen
) ;


/**
  key and its value at value buffer
*/
typedef struct st_tsec_val {
  char sName[SIZE_32B] ;
  char sValue[SIZE_128B] ;
} TSECVAL ;

typedef struct st_tsec_args {
  char sArgs[SIZE_1K] ;
  int iArgsCnt ;
  TSECVAL stArgs[TSEC_MAX_ARGS] ; /**< 16 args max */
} TSECARGS ;


typedef struct st_tsec_soitem {
  char sSvcName[SIZE_32B] ;
  char sMType[SIZE_32B] ;
  char sCtrFlag[SIZE_32B] ;
  char sMPath[SIZE_256B] ;
  TSECARGS stArgFields ;
} TSECSOITEM ;


/**
  global setttings struct
*/
typedef struct st_tsec_global {
  char sInitFile[SIZE_512B] ;

  TSECCONF stTsecConf ;
  struct stat stFileStat ; 

  int iTotalCnt ;
  TSECSOITEM *pstItems ;
} TSECGLOBAL ;


/**
  single function pointer
*/
typedef struct st_tsec_sofun {
  unsigned short iRun ;

  TSEC_SO_INIT so_init ;
  TSEC_SO_INIT_EX so_init_ex ;

  TSEC_SO_PROC so_proc ;
  TSEC_SO_PROC_EX so_proc_ex ;

  TSEC_SO_DEPROC so_deproc ;
  TSEC_SO_DEPROC_EX so_deproc_ex ;

  TSEC_SO_FINI so_fini ;
  TSEC_SO_FINI_EX so_fini_ex ;

  TSECSOITEM stItem ; /**< so item info struct */
} TSECSOFUN ;


/**
  tsec single API
*/
typedef struct st_tsec_svc {

  int iEnable ; /**< service inited */

  int iTrans ; /**< multi function execution index */

  char sSvc[SIZE_256B] ; /**< service name */

  int iCryptoCnts ; /**< crypto so counts */
  TSECSOFUN *pstCrypto ; /**< all symmetrical crypto functions */

  int iHashCnts ; /**< hash so counts */
  TSECSOFUN *pstHash ; /**< all hash functions */

  int iAsyCnts ; /**< asymmetrical crypto so counts */
  TSECSOFUN *pstACrypto ; /**< all asymmetrical crypto functions */

  int iEnvpCnts ; /**< envelope functions so counts */
  TSECSOFUN *pstEnvp ; /**< all envelope functions */

  int iIPCnts ; /**< ip verify functions so counts */
  TSECSOFUN *pstIP ; /**< all ip verify functions */

} TSECSVC ;


/**
  tsec API set
*/
typedef struct st_tsec_set {
  int iInited ; /**< tsec module if inited: 0 -- no, 1 -- yes */
  int iSvcCnt ;  /**< how much svc contains, identified by service name */
  TSECSVC *ptTsecAPI ;
} TSECSET ;


/**
  global parameters
*/
extern TSECSET s_stTsec ;
extern TSECGLOBAL mstTsecIni ;


/**
  @brief Global inited function
  @retval 0 -- service inited ok
  @retval !0 -- service inited failed
  @param[in] a_pszIniFile -- global settings file
  @note This function should be invoked before using any tsec functions
*/
int tsec_init ( IN const char *a_pszIniFile ) ;


/**
  @brief Global destructed function
  @retval 0 -- service destructed ok
  @retval !0 -- service destructed failed
  @note This function should be invoked before application process exits
*/
int tsec_fini ( ) ;


/**
  @brief Get operation handler via specified module name
  @param[in] a_pszModule -- service name
  @param[in,out] a_piHandler -- return service handler
  @retval 0 -- get handler ok
  @retval !0 -- get handler failed
  @note
*/
int tsec_open_handler ( IN const char *a_pszModule, INOUT int *a_piHandler ) ;


/**
  @brief Disable operation handler
  @param[in,out] a_piHandler -- return handler
  @retval 0 -- close handler ok
  @retval !0 -- close handler failed
  @note
*/
int tsec_close_handler ( INOUT int *a_piHandler ) ;


/**
  @brief reload a service function set
  @param[in] a_pszSvcName -- service name
  @retval 0 -- reload ok
  @retval !0 -- reload failed
  @note iif a_pszSvcName is set to "ALL", this function will reload all modules
*/
int tsec_svc_reload ( IN const char *a_pszSvcName ) ;


/**
  @brief check if function configured file modified
  @retval TSEC_FILE_NO_CHANGED -- ok, file no changed
  @retval TSEC_FILE_FAILED -- read file failed
  @retval TSEC_FILE_TIME_CHANGED -- file modification time changed
  @retval TSEC_FILE_SIZE_CHANGED -- file size changed
  @retval TSEC_FILE_BOTH_CHANGED -- file modification time and size changed
  @note
*/
int tsec_is_config_changed ( ) ;


/**
  @brief Encrypted function: DES, 3DES, AES, IDEA, Blowfish
  @param[in] a_iHandler -- service handle, this valude is obtained from tsec_open_handler()
  @param[in] a_pszKeys -- crypto key
  @param[in] a_pvData -- source data need to be enctypted
  @param[in] a_iDataLen -- source data len
  @param[in,out] a_pvBuffer -- enctypted data
  @param[in,out] a_piLen -- in: enctypted data buffer length; out: number of bytes encrypted returned
  @retval 0 -- encrypted ok
  @retval !0 -- encrypted failed
  @note
*/
int tsec_encrypt (
  IN const int a_iHandler,
  IN const char *a_pszKeys,
  IN const void *a_pvData,
  IN const ssize_t a_iDataLen,
  INOUT void *a_pvBuffer,
  INOUT ssize_t *a_piLen
) ;


/**
  @brief Descrypted function: DES, 3DES, AES, IDEA, Blowfish
  @param[in] a_iHandler -- service handle, this valude is obtained from tsec_open_handler()
  @param[in] a_pszKeys -- crypto key
  @param[in] a_pvData -- source data need to be descrypted, mostly, the data is
  @param[in] a_iDataLen -- source data len
  @param[in,out] a_pvBuffer -- descrypted data
  @param[in,out] a_piLen -- in: descrypted data buffer length; out: number of bytes descrypted returned
  @retval 0 -- descrypted ok
  @retval !0 -- descrypted failed
  @note
*/
int tsec_descrypt (
  IN const int a_iHandler,
  IN const char *a_pszKeys,
  IN const void *a_pvData,
  IN const ssize_t a_iDataLen,
  INOUT void *a_pvBuffer,
  INOUT ssize_t *a_piLen
) ;


/**
  @brief Hash function: MD2, MD5, SHA, SHA1
  @param[in] a_iHandler -- service handle, this valude is obtained from tsec_open_handler()
  @param[in] a_pvData -- source data need to be hashed
  @param[in] a_iDataLen -- source data len
  @param[in,out] a_pvBuffer -- hash value data
  @param[in,out] a_piLen -- in: data buffer length; out: hash data bytes
  @retval 0 -- hash ok
  @retval !0 -- hash failed
  @note
*/
int tsec_hash (
  IN const int a_iHandler,
  IN const void *a_pvData,
  IN const ssize_t a_iDataLen,
  INOUT void *a_pvBuffer,
  INOUT ssize_t *a_piLen
) ;


/**
  @brief asymmetrical crypto function: RSA
  @param[in] a_iHandler -- service handle, this valude is obtained from tsec_open_handler()
  @param[in] a_pszKeys -- crypto key
  @param[in] a_pvData -- source data need to be encrypted, mostly, the data is
  @param[in] a_iDataLen -- source data len
  @param[in,out] a_pvBuffer -- encrypted data
  @param[in,out] a_piLen -- in: encrypted data buffer length; out: number of bytes encrypted returned
  @retval 0 -- encrypted ok
  @retval !0 -- encrypted failed
  @note
*/
int tsec_aencrypt (
  IN const int a_iHandler,
  IN const char *a_pszKeys,
  IN const void *a_pvData,
  IN const ssize_t a_iDataLen,
  INOUT void *a_pvBuffer,
  INOUT ssize_t *a_piLen
) ;


/**
  @brief asymmetrical crypto function: RSA
  @param[in] a_iHandler -- service handle, this valude is obtained from tsec_open_handler()
  @param[in] a_pszKeys -- crypto key
  @param[in] a_pvData -- source data need to be descrypted, mostly, the data is
  @param[in] a_iDataLen -- source data len
  @param[in,out] a_pvBuffer -- descrypted data
  @param[in,out] a_piLen -- in: descrypted data buffer length; out: number of bytes descrypted returned
  @retval 0 -- descrypted ok
  @retval !0 -- descrypted failed
  @note
*/
int tsec_adescrypt (
  IN const int a_iHandler,
  IN const char *a_pszKeys,
  IN const void *a_pvData,
  IN const ssize_t a_iDataLen,
  INOUT void *a_pvBuffer,
  INOUT ssize_t *a_piLen
) ;


/**
  @brief ip connection counter register function
  @param[in] a_iHandler -- service handle, this valude is obtained from tsec_open_handler()
  @param[in] a_pszSrcIP -- source ip, format "xxx.xxx.xxx.xxx", could not be NULL, if no value, just set it empty string
  @retval 0 -- invoked ok
  @retval !0 -- invoked failed
  @note
*/
int tsec_ip_reg (
  IN const int a_iHandler,
  IN const char *a_pszSrcIP
) ;


/**
  @brief ip connection counter unregister function
  @param[in] a_iHandler -- service handle, this valude is obtained from tsec_open_handler()
  @param[in] a_pszSrcIP -- source ip, format "xxx.xxx.xxx.xxx", could not be NULL, if no value, just set it empty string
  @retval 0 -- invoked ok
  @retval !0 -- invoked failed
  @note
*/
int tsec_ip_unreg (
  IN const int a_iHandler,
  IN const char *a_pszSrcIP
) ;


/**
  @brief ip connection conunter reset function
  @param[in] a_iHandler -- service handle, this valude is obtained from tsec_open_handler()
  @param[in] a_pszSrcIP -- source ip, format "xxx.xxx.xxx.xxx", could not be NULL, if no value, just set it empty string
  @retval 0 -- invoked ok
  @retval !0 -- invoked failed
  @note
*/
int tsec_ip_reset (
  IN const int a_iHandler,
  IN const char *a_pszSrcIP
) ;


/**
  @brief source ip verfify function
  @param[in] a_iHandler -- service handle, this valude is obtained from tsec_open_handler()
  @param[in] a_pszSrcIP -- source ip, format "xxx.xxx.xxx.xxx", could not be NULL, if no value, just set it empty string
  @retval 0 -- invoked ok
  @retval !0 -- invoked failed
  @note
*/
int tsec_ip_srcip_verify (
  IN const int a_iHandler,
  IN const char *a_pszSrcIP
) ;


/**
  @brief source ip --> destination ip verfify function
  @param[in] a_iHandler -- service handle, this valude is obtained from tsec_open_handler()
  @param[in] a_pszSrcIP -- source ip, format "xxx.xxx.xxx.xxx", could not be NULL, if no value, just set it empty string
  @param[in] a_pszDstIP -- destination ip, format "xxx.xxx.xxx.xxx", could not be NULL, if no value, just set it empty string 
  @retval 0 -- invoked ok
  @retval !0 -- invoked failed
  @note
*/
int tsec_ip_dstip_verify (
  IN const int a_iHandler,
  IN const char *a_pszSrcIP,
  IN const char *a_pszDstIP
) ;

#endif /**< _TSEC_H */

