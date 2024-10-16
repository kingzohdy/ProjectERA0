#ifndef TMETABASE_H
#define TMETABASE_H
#include "pal/pal.h"
#include "tdr/tdr.h"

#define TMB_VERSION			0x01

#define TMB_IPC_TYPE			'2'

#define TMB_DFT_KEY			30000

struct tagTMetaBase
{
	int iVersion;
	int iSize;
	int iMaxLibs;
	int iCurLibs;
	int offs[1];
};

typedef struct tagTMetaBase		TMETABASE;
typedef struct tagTMetaBase		*LPTMETABASE;

struct tagTMBDesc
{
	int iLock;
	int iShm;
	LPTMETABASE pstBase;
	LPTDRMETALIB pstLib;
};

typedef struct tagTMBDesc		TMBDESC;
typedef struct tagTMBDesc		*LPTMBDESC;
typedef struct tagTMBDesc		*HTMBDESC;

#ifdef __cplusplus
extern "C"
{
#endif

TSF4G_API int tmb_create(OUT HTMBDESC* a_phDesc, IN const char* a_pszKey, IN int a_iMaxLibs, IN int a_iSize);
TSF4G_API int tmb_destroy(IN char* a_pszKey);

TSF4G_API int tmb_open(OUT HTMBDESC* a_phDesc, IN const char* a_pszKey, IN int a_fReadOnly);
TSF4G_API int tmb_close(INOUT HTMBDESC* a_phDesc);

TSF4G_API int tmb_lock(IN HTMBDESC a_hDesc);
TSF4G_API int tmb_unlock(IN HTMBDESC a_hDesc);

TSF4G_API int tmb_open_metalib(INOUT HTMBDESC a_hDesc, IN const char* a_pszLib, IN int a_iVersion);
TSF4G_API int tmb_dump(IN HTMBDESC a_hDesc, IN int a_fDetail, IN FILE* a_fp);

TSF4G_API int tmb_append(INOUT HTMBDESC a_hDesc, IN const char* a_pszPath);
TSF4G_API int tmb_meta_by_name(IN HTMBDESC a_hDesc, IN const char* a_pszName, INOUT LPTDRMETA* a_ppstMeta);
TSF4G_API int tmb_meta_by_id(IN HTMBDESC a_hDesc, IN int a_iID, INOUT LPTDRMETA* a_ppstMeta);

#ifdef __cplusplus
}
#endif

#endif /* TMETABASE_H */
