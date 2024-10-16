/** @file $RCSfile: tsec_macros.h,v $
  defined macros file for tsf4g-security module
  $Id: tsec_macros.h,v 1.1.1.1 2011/01/05 10:59:29 jerry Exp $
@author $Author: jerry $
@date $Date: 2011/01/05 10:59:29 $
@version 1.0
@note Editor: Vim 6.3, Gcc 4.0.2, tab=4
@note Platform: Linux
*/


#ifndef _TSEC_MACROS_H
#define _TSEC_MACROS_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <assert.h>

#ifndef _WIN32
#include <dlfcn.h>
#endif

#include "pal/pal.h"

#ifdef _WIN32
  #ifdef TSEC_DATA_EXPORTS
  #    define TSEC_DATA __declspec(dllexport)
  #else
  #    define TSEC_DATA extern __declspec(dllimport)
  #endif

  #ifdef TSEC_API_EXPORTS
  #    define TSEC_API __declspec(dllexport)
  #else
  #    define TSEC_API extern __declspec(dllimport)
  #endif
#else
  #define TSEC_DATA
  #define TSEC_API
#endif


#define TSEC_CONF_INFO "TsecConf"


/*@{ @name Size macros defined

  description of defined macro
*/
#define SIZE_4B   4
#define SIZE_8B   8
#define SIZE_16B  16
#define SIZE_32B  32
#define SIZE_64B  64
#define SIZE_128B 128
#define SIZE_256B 256
#define SIZE_512B 512
#define SIZE_1K   1024
#define SIZE_2K   2048
#define SIZE_4K   4096
#define SIZE_8K   8192
#define SIZE_16K  16384
#define SIZE_32K  32768
#define SIZE_64K  65536
/*@}*/

/*@{ @name Size macros defined

  description of defined macro
*/
#define TSEC_MAX_ARGS 16
/*@}*/

/*@{ @name tsec service type

  description of defined macro
*/
#define TSEC_SVC_CRYPTO  "crypto"
#define TSEC_SVC_HASH    "hash"
#define TSEC_SVC_ACRYPTO "acrypto"
#define TSEC_SVC_ENVP    "envelope"
#define TSEC_SVC_IP      "ip"

/*@}*/


/*@{ @name so function name defined

  description of defined macro
*/
#define TSEC_SO_INIT_SYM      "tsec_so_init"
#define TSEC_SO_INIT_EX_SYM   "tsec_so_init_ex"
#define TSEC_SO_PORC_SYM      "tsec_so_process"
#define TSEC_SO_PORC_EX_SYM   "tsec_so_process_ex"
#define TSEC_SO_DEPROC_SYM    "tsec_so_de_process"
#define TSEC_SO_DEPROC_EX_SYM "tsec_so_de_process_ex"
#define TSEC_SO_FINI_SYM      "tsec_so_fini"
#define TSEC_SO_FINI_EX_SYM   "tsec_so_fini_ex"
/*@}*/


/*@{ @name tsec service transaction type

  description of defined macro
*/
#define TSEC_TRANS_UNSET 0
#define TSEC_TRANS_SET   1
/*@}*/


/*@{ @name tsec service transaction type

  description of defined macro
*/
#define TSEC_CTRL_REQUIRED   "REQUIRED"   /**< no matter if the result is ok or failed, just indicated to invoke next function */
#define TSEC_CTRL_REQUISITE  "REQUISITE"  /**< if invoked successfully, continue invoked next function; otherwise, return immediately */
#define TSEC_CTRL_SUFFICIENT "SUFFICIENT" /**< if invoked successfully, just return; otherwise, continue invoke next function */
#define TSEC_CTRL_OPTIONAL   "OPTIONAL"   /**< skip any successful or failed invoked result */
/*@}*/


/*@{ @name tsec service reload operation type

  description of defined macro
*/
#define TSEC_SVC_LD_ALL    "LOAD_ALL"
#define TSEC_SVC_NOT_FOUNT -1
/*@}*/


/*@{ @name tsec configured file type

  description of defined macro
*/
#define TSEC_FILE_FAILED      -1
#define TSEC_FILE_NO_CHANGED   0
#define TSEC_FILE_TIME_CHANGED 1
#define TSEC_FILE_SIZE_CHANGED 2
#define TSEC_FILE_BOTH_CHANGED 3
/*@}*/


/*@{ @name Return code defined

  description of defined macro
*/
#define TSEC_OK            0
#define TSEC_FAILED       -1
#define TSEC_SYS_ERROR    -2
#define TSEC_ARG_ERROR    -3
#define TSEC_SVC_DISABLE  -4
#define TSEC_CTRL_UNKNOWN -5

#define HANDLER_NOT_FOUND -1
/*@}*/


/*@{ @name Service inited index defined

  description of defined macro
*/
#define SVC_NOT_INITED  0 /**< service not inited, function could not be invoked */
#define SVC_INITED      1 /**< service inited */
#define FUNC_NOT_INITED 0 /**< application function not inited */
#define FUNC_INITED     1 /**< application Functions inited */
/*@}*/

#endif /**< TSEC_MACROS_H */

