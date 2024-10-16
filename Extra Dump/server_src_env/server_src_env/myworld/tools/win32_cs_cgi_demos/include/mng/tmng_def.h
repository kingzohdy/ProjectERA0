/*
 * @file   tmng_def.h
 *
 * @brief  TSF-G-MNG stuct define hearder file 
 *
 * @author Johnsun@localhost
 */

#ifndef		TSF_G_MNG_DEF_H
#define		TSF_G_MNG_DEF_H
#include    "mng/deploy_def.h"
#include    "mng/tmib.h"
#include    "mng/tmetabase.h"

#define     DEFAULT_COUNT           1000
#define     DEFAULT_SIZE            10000000
#define     KEY_SIZE                32
#define     MNG_PROCNAME_LEN        32
#define     CFG_DOMAIN              "CFG"
#define     MNG_DOMAIN              "MNG"
#define     RT_DOMAIN               "RUN"
#define     RDO_DOMAIN              "RDO"
#define     PROC_LIST_DOMAIN        "ProcList"
#define     DEFAULT_MIB_KEY         "22000"
#define     DEFAULT_METABASE_KEY    "22001"

#define     MNG_LIB                 "managing"
#define     DEPOLY_CFG              "Deploy"
#define     PROC_ATTR               "MNGPRocAttr"


/*
typedef   int (*TMNG_MSG_FUNC_PTR)(IN        unsigned int a_iRequestLen,
                                     IN        char* a_pRequest,
                                     INOUT     unsigned int* a_piResponseLen,
                                     OUT       char* a_pResponse);
*/

typedef struct tagTmngHandler
{   
    int         argc;
    char**      argv;
    int         iStaus;
    char        szProcName[MNG_PROCNAME_LEN];
    char        szProcID[MNG_PROCNAME_LEN];
    int         iProcID;
    int         iAgentID;
    int         iBus;               /* app to agent communicate*/

    char        szMibKey[KEY_SIZE];
    HTMIB       hMib;
    char        szMetabaseKey[KEY_SIZE];
    HTMBDESC    hDesc;

    LPMNGPROCATTR  pstProcAttr;
/*
    char    sMsgAddr[MaxMsgAddrLen+1] ;
    cwarn_info_map  WarnInfoMap;
    void    (*on_exit_ptr)(void) ;
    TMNG_MSG_FUNC_PTR  on_cfg_chg_ptr;
*/      
}   TMNGHANDLER;

typedef struct tagTmngHandler  *HTMNG;


#define     MNG_START_REPORT_REQ     0x01
#define     MNG_STATUS_REPORT_REQ    0x03
#define     MNG_EXIT_NOTIFY_REQ      0x05


#endif  /* TSF_G_MNG_DEF_H */
    
