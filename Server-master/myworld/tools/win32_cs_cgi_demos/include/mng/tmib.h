/**************************************************************
 @file $RCSfile: tmib.h,v $
  tsf managing MIB define file
  $Id: tmib.h,v 1.1.1.1 2011/01/04 11:02:51 niko Exp $
 @author $Author: niko $
 @date $Date: 2011/01/04 11:02:51 $
 @version 1.0
 @note Editor: Vim 6.3, Gcc 4.0.2, tab=4
 @note Platform: Linux
 **************************************************************/

#ifndef TMNG_MIB_H
#define TMNG_MIB_H

#include "pal/pal.h"
#include "mng/tmetabase.h"


#define TMIB_VERSION        0x01

#define TMIB_IPC_TYPE       '1'
#define DR_NAME_LEN         32
#define DEFAULT_MIB_DATA_SIZE       1024*1024

struct tagMIBDataHead
{
    int     iPeriods;               /* report periods  (minutes) */
    time_t  tReportTime;            /* report timestamp */
    time_t  tUpdateTime;            /* update timestamp */
};

typedef struct tagMIBDataHead    TMIBHEAD;
typedef struct tagMIBDataHead    *LPTMIBHEAD;

#define     MIB_HEAD_SIZE       128


struct tagTMIBEntry
{
    char    szLib[DR_NAME_LEN];
    char    szMeta[DR_NAME_LEN];
    char    szDomain[DR_NAME_LEN];
    char    szName[DR_NAME_LEN];
    int     iProcID;
    int     iVersion;       
    int     iOff;                   /* iOff must be page aligned. */
    int     iSize;                  /* Use mprotect to do protect. */
    int     bExtern;                /* if  use external memory */
    int     iExShm;
    int     iExSize;
    int     bDelete;                /* this entry is deleted. */
    int     iPeriods;               /* report periods  (minutes) */
    time_t  tReportTime;            /* report timestamp */
    time_t  tUpdateTime;            /* update timestamp */
    int     bSystem;                /* this entry is added by system. */    
    int     iReserv1;
    int     iReserv2;
};

typedef struct tagTMIBEntry    TMIBENTRY;
typedef struct tagTMIBEntry    *LPTMIBENTRY;

struct tagTMIBData
{    
    char    szLib[DR_NAME_LEN];     /* input/output. */
    char    szMeta[DR_NAME_LEN];    /* input/output. */
    char    szDomain[DR_NAME_LEN];  /* input. */
    char    szName[DR_NAME_LEN];    /* input. */
    int     iProcID;                /* input. */
    int     iVersion;               
    int     iSize;                  /* input/output. */
    int     bExtern;                /* input/output. */
    int     iPeriods;      
    LPTMIBHEAD   pstHead;
    char*   pszData;                /* output. */
    int     iEntry;                 /* output, internal used. */
};

typedef struct tagTMIBData    TMIBDATA;
typedef struct tagTMIBData    *LPTMIBDATA;

struct tagTMIBInfo
{
    int     iVersion;
    int     iSize;
    int     iMaxDatas;
    int     iCurDatas;
    int     iExternSize;
    int     iHeadSize;            /* must be protected, page aligned. */
    int     iCheckSum;
    TMIBENTRY entries[1];
};

typedef struct tagTMIBInfo    TMIBINFO;
typedef struct tagTMIBInfo    *LPTMIBINFO;

struct tagTMIBDesc
{
    int     iLock;
    int     iShm;
    int     bCreate;
    TMIBINFO* pstInfo;
};

typedef struct tagTMIBDesc    TMIBDESC;
typedef struct tagTMIBDesc    *LPMIBDESC;
typedef struct tagTMIBDesc    *HTMIB;

#ifdef __cplusplus
extern "C"
{
#endif


/************************************************************************* 
  @brief create mib (rdo lib) handler
  @param a_phMib  -- mib handler
  @param a_pszKey -- mib key string (ipc key)
  @param a_iDatas -- mib data number
  @param a_iSize  -- mib memory size
  @retval 0  -- sucess
  @retval !0 -- fail
  @note  key format:  to be describe......
 *************************************************************************/
TSF4G_API int tmib_create(OUT HTMIB* a_phMib, IN const char* a_pszKey, IN int a_iDatas, IN int a_iSize);


/************************************************************************* 
  @brief destroy mib space
  @param a_pszKey -- mib key string
  @retval 0  -- sucess
  @retval !0 -- fail
  @note  key fromat : see also tmng_create_mib
 *************************************************************************/
TSF4G_API int tmib_destroy(IN const char* a_pszKey);


/************************************************************************* 
  @brief open a mib  handler ( have created already)
  @param a_phMib  -- mib handler
  @param a_pszKey -- mib key string (ipc key)
  @retval 0  -- sucess
  @retval !0 -- fail
  @note  key fromat : see also tmng_create_mib
 *************************************************************************/
TSF4G_API int tmib_open(OUT HTMIB *a_phMib, IN const char* a_pszKey);


/************************************************************************* 
  @brief close a mib  handler 
  @param a_phMib  -- mib handler
  @retval 0  -- sucess
  @retval !0 -- fail
  @note  
 *************************************************************************/
TSF4G_API int tmib_close(IN HTMIB *a_phMib);


/************************************************************************* 
  @brief lock a mib  handler 
  @param a_phMib  -- mib handler
  @retval 0  -- sucess
  @retval !0 -- fail
  @note  
 *************************************************************************/
/*
int tmib_lock_i(IN HTMIB a_hMib);
*/

/************************************************************************* 
  @brief unlock a mib  handler 
  @param a_phMib  -- mib handler
  @retval 0  -- sucess
  @retval !0 -- fail
  @note  
 *************************************************************************/
/*
int tmib_unlock_i(IN HTMIB a_hMib);
*/

/************************************************************************* 
  @brief  set extern size limit
  @param a_phMib    -- mib handler
  @param a_iExSize  -- extern size limit
  @retval 0  -- sucess
  @retval !0 -- fail
  @note  
 *************************************************************************/
TSF4G_API int tmib_set_extern_size(INOUT HTMIB a_hMib, IN int a_iExSize);

/************************************************************************* 
  @brief  set data update time stamp with unlocked
  @param a_phMib    -- mib handler
  @param a_pstData  -- data struct pointer
  @param a_tUpdTime --time stamp 
  @retval 0  -- sucess
  @retval !0 -- fail
  @note  
 *************************************************************************/
TSF4G_API int tmib_set_updtime_unlock(INOUT HTMIB a_hMib, INOUT TMIBDATA* a_pstData, IN time_t a_tUpdTime);


/************************************************************************* 
  @brief  set data update time stamp now time
  @param a_phMib    -- mib handler
  @param a_pstData  -- data struct pointer
  @retval 0  -- sucess
  @retval !0 -- not found
  @note  call function tmng_mib_set_updtime_unlock
 *************************************************************************/
TSF4G_API int tmib_set_updtime_now(INOUT HTMIB a_hMib, INOUT TMIBDATA* a_pstData);


/************************************************************************* 
  @brief  find data in mib with unlocked
  @param a_phMib    -- mib handler
  @param a_pstData  -- data struct pointer
  @param a_bIncludeDelete-- !0: include deleted data  0:not include
  @retval 0  -- sucess
  @retval !0 -- not found
  @note  
 *************************************************************************/
TSF4G_API int tmib_find_data_unlocked(IN HTMIB a_hMib, INOUT TMIBDATA* a_pstData, IN int a_bIncludeDelete);


/************************************************************************* 
  @brief  register data in mib with unlocked
  @param a_phMib    -- mib handler
  @param a_pstData  -- data struct pointer
  @retval 0  -- sucess
  @retval !0 -- fail
  @note  
 *************************************************************************/
TSF4G_API int tmib_register_data_unlocked(INOUT HTMIB a_hMib, INOUT TMIBDATA* a_pstData);


/************************************************************************* 
  @brief  check data control entry  with unlocked
  @param a_phMib    -- mib handler
  @param a_pstData  -- data struct pointer
  @retval 0  -- sucess
  @retval !0 -- fail
  @note  if data marked deleted , enable data to use
 *************************************************************************/
TSF4G_API int tmib_check_entry_unlocked(INOUT HTMIB a_hMib, IN TMIBDATA* a_pstData);


/************************************************************************* 
  @brief  register data into mib
  @param a_phMib    -- mib handler
  @param a_pstData  -- data struct pointer
  @retval 0  -- sucess
  @retval !0 -- fail
  @note  
 *************************************************************************/
TSF4G_API int tmib_register_data(INOUT  HTMIB a_hMib, INOUT TMIBDATA* a_pstData);


/************************************************************************* 
  @brief  register data into mib  by metalib name or(key string)
  @param a_phMib    -- mib handler
  @param a_hDesc    -- metalib name
  @param a_pstData  -- data struct pointer
  @retval 0  -- sucess
  @retval !0 -- fail
  @note  
 *************************************************************************/
TSF4G_API int tmib_register_data_by_mbhd(INOUT HTMIB a_hMib, IN HTMBDESC a_hDesc, INOUT TMIBDATA* a_pstData);



/************************************************************************* 
  @brief  register data into mib  by metalib
  @param a_phMib    -- mib handler
  @param a_hDesc    -- metalib handler
  @param a_pstData  -- data struct pointer
  @retval 0  -- sucess
  @retval !0 -- fail
  @note  
 *************************************************************************/
TSF4G_API int tmib_register_data_by_mbkey(INOUT HTMIB a_hMib, IN const char* a_pszBase, INOUT TMIBDATA* a_pstData);

/************************************************************************* 
  @brief  protect data read or write
  @param a_pstData  -- data struct pointer
  @param a_bReadOnly-- !0: read only  0: read or write
  @retval 0  -- sucess
  @retval !0 -- not found
  @note  
 *************************************************************************/
TSF4G_API int tmib_protect_data(IN TMIBDATA* a_pstData, IN int a_bReadOnly);


/************************************************************************* 
  @brief  unregister data in mib with unlocked
  @param a_phMib    -- mib handler
  @param a_pstData  -- data struct pointer
  @retval 0  -- sucess
  @retval !0 -- not found
  @note   mark entry delete field
*************************************************************************/
TSF4G_API int tmib_unregister_data_unlocked(INOUT HTMIB a_hMib, IN TMIBDATA* a_pstData);


/************************************************************************* 
  @brief  unregister data in mib
  @param a_phMib    -- mib handler
  @param a_pstData  -- data struct pointer
  @retval 0  -- sucess
  @retval !0 -- not found
  @note   lock and call tmng_mib_unregister_data_unlocked_i
*************************************************************************/
TSF4G_API int tmib_unregister_data(INOUT HTMIB a_hMib, IN TMIBDATA* a_pstData);


/************************************************************************* 
  @brief  unregister all data in domain with unlocked
  @param a_phMib    -- mib handler
  @param a_pszDomain-- domain
  @retval 0  -- sucess
  @retval !0 -- not found
  @note   mark entry delete field
*************************************************************************/
TSF4G_API int tmib_unregister_domain_unlocked(INOUT HTMIB a_hMib, IN const char* a_pszDomain);

/************************************************************************* 
  @brief  unregister all data in domain 
  @param a_phMib    -- mib handler
  @param a_pszDomain-- domain
  @retval 0  -- sucess
  @retval !0 -- not found
  @note   lock and call tmng_mib_unregister_domain_unlocked
*************************************************************************/
TSF4G_API int tmib_unregister_domain(INOUT HTMIB a_hMib, IN const char* a_pszDomain);

/************************************************************************* 
  @brief  get data from  mib 
  @param a_phMib    -- mib handler
  @param a_pstData  -- data struct pointer
  @param a_bReadOnly-- 0:read and write !0:readonly
  @retval 0  -- sucess
  @retval !0 -- fail
  @note  
 *************************************************************************/
TSF4G_API int tmib_get_data(IN HTMIB a_hMib, INOUT TMIBDATA* a_pstData, IN int a_bReadOnly);


/************************************************************************* 
  @brief  validate data from  mib 
  @param a_phMib    -- mib handler
  @param a_pstData  -- data struct pointer
  @param a_bReadOnly-- 0:read and write !0:readonly
  @retval 0  -- sucess
  @retval !0 -- fail
  @note  
 *************************************************************************/
TSF4G_API int tmib_validate_data(IN HTMIB a_hMib, INOUT TMIBDATA* a_pstData, IN int a_bReadOnly);


/************************************************************************* 
  @brief dump Mib head 
  @param a_hMib     -- mib handler
  @param a_fp       -- file pointer
  @retval 0  -- sucess
  @retval !0 -- fail
  @note  dump mib head data
 *************************************************************************/
TSF4G_API int tmib_dump_head(IN HTMIB a_hMib, IN FILE* a_fp);


/************************************************************************* 
  @brief use dr metalib dump data
  @param a_hDesc    -- metelib handler
  @param a_pstData  -- data struct pointer
  @param a_fp       -- file pointer
  @retval 0  -- sucess
  @retval !0 -- fail
  @note  
 *************************************************************************/
TSF4G_API int tmib_dump(IN HTMBDESC a_hDesc, IN TMIBDATA* a_pstData, IN FILE* a_fp);

/************************************************************************* 
  @brief use dr metalib dump data
  @param a_pszBase  -- metelib key string
  @param a_pstData  -- data struct pointer
  @param a_fp       -- file pointer
  @retval 0  -- sucess
  @retval !0 -- fail
  @note  one data
 *************************************************************************/
TSF4G_API int tmib_dump_once(IN const char* a_pszBase, IN TMIBDATA* a_pstData, IN FILE* a_fp);

/************************************************************************* 
  @brief use dr metalib dump all data
  @param a_hMib     -- mib handler
  @param a_pszBase  -- metelib key string
  @param a_fp       -- file pointer
  @retval 0  -- sucess
  @retval !0 -- fail
  @note  dump all data in mib
 *************************************************************************/
TSF4G_API int tmib_dump_all(IN HTMIB a_hMib, IN const char* a_pszBase, IN FILE* a_fp);


#ifdef __cplusplus
}
#endif

#endif /* TMNG_MIB_H  */
