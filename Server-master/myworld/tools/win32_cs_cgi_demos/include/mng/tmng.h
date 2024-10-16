/*
 * @file   tmng.h
 *
 * @brief  TSF-G-MNG main hearder file 
 *
 * @author Johnsun@localhost
 */

#ifndef		TSF_G_MNG_H
#define		TSF_G_MNG_H

#include    "pal/pal.h"
#include    "mng/tmng_def.h"
#include    "mng/mng_protocol.h"
#include    "mng/deploy_def.h"

/************************************************************************* 
  @brief  manager config module init
  @param a_phMng   manager handler
  @param a_szMibKey   
  @param a_szMetabaseKey  
  @retval 0  -- sucess
  @retval !0 -- error
  @note   
*************************************************************************/
TSF4G_API int  tmng_init_mem(INOUT HTMNG   *a_phMng, IN const char* a_szMibKey, IN const char* a_szMetabaseKey);

/************************************************************************* 
  @brief  manager config module fini
  @param a_phMng   manager handler
  @retval 0  -- sucess
  @retval !0 -- error
  @note   
*************************************************************************/
TSF4G_API int tmng_fini_mem(INOUT HTMNG   *a_phMng );


/************************************************************************* 
  @brief  manager config module init
  @param a_phMng   manager handler
  @param a_iProcId   bus define process id
  @param a_iAgentId  bus define agent id 
  @param a_szMibKey   
  @param a_szMetabaseKey  
  @retval 0  -- sucess
  @retval !0 -- error
  @note   
*************************************************************************/
TSF4G_API int tmng_init(  OUT HTMNG*      a_phMng, 
                IN  int         a_Argc,
                IN  const char* a_Argv[],
                IN  int         a_iProcID, 
                IN  const char* a_szMibKey, 
                IN  const char* a_szMetabaseKey);


/************************************************************************* 
  @brief  manager config module fini
  @param a_phMng   manager handler
  @retval 0  -- sucess
  @retval !0 -- error
  @note   
*************************************************************************/
TSF4G_API int tmng_fini(INOUT HTMNG   *a_phMng );

/************************************************************************* 
  @brief get configer  data point
  @param a_phMng   manager handler
  @param a_szCfgName    -- configer data name
  @param a_iVersion     -- data version
  @param a_ppCfgData    -- data point
  @retval 0  -- sucess
  @retval !0 -- error
  @note   configer data must be load in manager by another program
*************************************************************************/
TSF4G_API int tmng_get_cfg(IN  HTMNG       a_hMng,
                 IN  const char* a_szCfgName, 
                 IN  int         a_iVersion, 
                 OUT char**      a_ppCfgData );


TSF4G_API int tmng_send_alert(IN  HTMNG  a_hMng, const LPMNGALERT a_pstAlert);
    

TSF4G_API int tmng_get_rdo(IN  HTMNG       a_hMng,
                 IN  const char* a_szName, 
                 IN  int         a_iPeriods,
                 IN  int         a_iVersion, 
                 OUT char**      a_ppData ,
                 OUT char**      a_ppDefine);


#endif  /* TSF_G_MNG_H */

