/*
**  @file $RCSfile: tapp.h,v $
**  general description of this module
**  $Id: tapp.h,v 1.1.1.1 2011/01/05 10:59:29 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2011/01/05 10:59:29 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TAPP_H
#define TAPP_H

/* automatically include the correct library on windows */
#ifdef WIN32

# ifdef _DEBUG
#  define TSF4G_LIB_D    "_d"
# else
#  define TSF4G_LIB_D
# endif /* _DEBUG */


#if defined(TSF4G_LIB_D)
#pragma comment(lib, "libtapp"TSF4G_LIB_D".lib" )
#else
#pragma comment(lib, "libtapp.lib")
#endif

#endif/*#ifdef WIN32*/


#include "pal/pal.h"

#define TAPP_EXIT_QUIT		0x01
#define TAPP_EXIT_STOP		0x02

#define TAPP_DEF_TIMER		50

#ifdef  WIN32
#define TAPP_DEF_BUSDIR		"C:\\Temp"
#else /* WIN32 */
#define TAPP_DEF_BUSDIR		"/usr/local/bus"
#endif /* WIN32 */

#define TAPP_USEBUS_NONE	0
#define TAPP_USEBUS_TBUS	1
#define TAPP_USEBUS_OBUS	2

#define TAPP_EPOLL_WAIT		10

#ifdef __cplusplus
extern "C" 
{
#endif 

/* version pattern: major.minor.build */

#define TAPP_MAKE_VERSION(major, minor, rev, build)	(((unsigned long long)(major))<<56 |((unsigned long long) (minor))<<40 | ((unsigned long long)(rev))<<32 | (build) )
#define TAPP_GET_MAJOR(ver)						((ver)>>56 & 0xff)
#define TAPP_GET_MINOR(ver)						((ver)>>40 & 0xffff)
#define TAPP_GET_REV(ver)						((ver)>>32 & 0xff)
#define TAPP_GET_BUILD(ver)						((ver) & 0xffffffff)

#define TAPP_REFRESH_TBUS_CONFIGRE_DEFAULT_TIMER	60	/*the default timer used by tapp to refresh tbus configure*/

struct tagTappData
{
	char* pszBuff;
	int iLen;
	char* pszMetaName;
	int iMeta;
};

typedef struct tagTappData						TAPPDATA;
typedef struct tagTappData						*LPTAPPDATA;

struct tagTappOption
{
	int iRefeshTbusTimer;	/*timer for refresh tbus channel configure*/
	int iConfigFileFormat;	/*xml File format*/
};
typedef struct tagTappOption	TAPPOPTION;
typedef struct tagTappOption	*LPTAPPOPTION;

struct tagTAPPCTX
{
	int argc;
	char** argv;

	unsigned long long uiVersion;
	int iTimer;		/* the tick timer period. */

	int iWait;		/* the seconds to wait the previous to exit. */
	int iEpollWait;	/* the milliseconds to wait the epoll-wait timeout. */
	int iIdleCount;
	int iIdleSleep;

	int iTickCount;
	int iUseBus;  /* this member can use three value: TAPP_USEBUS_NONE, TAPP_USEBUS_OBUS, TAPP_USEBUS_TBUS */
					/* for value TAPP_USEBUS_NONE, iBus is nonsense. */
					/* for value TAPP_USEBUS_OBUS, iBus is really a pointer to BusInterface*. */
					/* for value TAPP_USEBUS_TBUS, iBus is really handle of tbus. */
	char *pszGCIMKey;   /*Shm key for tbus GCIM*/
	int  iBusinessID;		/*Bussiness id assigned by tagent*/
	struct timeval stLastTick;
	struct timeval stCurr;				

	const char* pszApp;
	const char* pszId;
	const char* pszPidFile;
	const char* pszConfFile;

	const char* pszBusDir;
	const char* pszTdrFile;
	const char* pszLogFile;
	const char* pszLogLevel;

	const char *pszLogConfFile;/*tlog cfg file path*/
	const char *pszRundataTimerConf; /*run data timer cfg file path*/

	const char* pszMibKey;
	const char* pszMBaseKey;

	int (*pfnReload)(struct tagTAPPCTX *pstCtx, void* pvArg);
	int (*pfnProc)(struct tagTAPPCTX *pstCtx, void* pvArg);

	int (*pfnTick)(struct tagTAPPCTX *pstCtx, void* pvArg);
	int (*pfnInit)(struct tagTAPPCTX *pstCtx, void* pvArg);
	int (*pfnFini)(struct tagTAPPCTX *pstCtx, void* pvArg);
	int (*pfnQuit)(struct tagTAPPCTX *pstCtx, void* pvArg); /* when quit signal is received, this func while be called, if -1 is returned, the mainloop will break. */
	int (*pfnStop)(struct tagTAPPCTX *pstCtx, void* pvArg); /* when quit signal is received, this func while be called, if -1 is returned, the mainloop will break. */
	int (*pfnIdle)(struct tagTAPPCTX *pstCtx, void* pvArg);
	int (*pfnUsage)(int argc, char* argv[]);

	int (*pfnGenerate)(struct tagTAPPCTX *pstCtx, void* pvArg); /*  this func while be called,to generate templet of configure file. */
	
	int iNoLoadConf;
	int iNeedFreeLib;
	int iLib;	/* if used, and non-zero, can be cast to TDRMETALIB* pointer. */
	int iBus;

	int iUseMib;
	int iMBase;
	int iMib;
	int iId;

	time_t tLogConfMod;
	time_t tConfMod;
	time_t tRunTimerConfMod;

	TAPPDATA stBaseData; /*base run time data*/
	TAPPDATA stConfData; /*conf data*/
	TAPPDATA stConfPrepareData; /*when reload, can use this for prepare load conf */
	TAPPDATA stConfBackupData; /*after reload , old conf data in here*/
	TAPPDATA stRunDataStatus; /*run time data for status val*/
	TAPPDATA stRunDataCumu; /*run time data for cumulate val*/
	TAPPDATA stRunDataTimer; /*run time data timer*/
	TAPPDATA stLogConfData; /*log conf data*/
	TAPPDATA stLogConfPrepareData; /*when reload, can use this for prepare load log conf */
	TAPPDATA stLogConfBackupData; /*after reload , old log conf data in here*/
	TAPPOPTION stOption;	/*the option by command line*/ 
};

typedef struct tagTAPPCTX 		TAPPCTX;
typedef struct tagTAPPCTX 		*LPTAPPCTX;


typedef int (*PFNTAPPFUNC)(struct tagTAPPCTX*, void*);

/*********************************************************************
*	@brief  judge whether recieve exit request.
*	@param void no param is needed.
*	@return whether need to exit the process.
*	@retval 0 no exit request received.
*	@retval 1 TAPP_EXIT_QUIT need exit, just quit. 
*	@retval 2 TAPP_EXIT_STOP need exit, a full stop required. 
*********************************************************************/
TSF4G_API int tapp_is_exit(void);


/*********************************************************************
*	@brief  exit the mainloop.
*	@param void no param is needed.
*	@return whether need to exit the process.
*	@retval 0 no exit request received.
*********************************************************************/
TSF4G_API void tapp_exit_mainloop(void);


TSF4G_API int tapp_def_init(TAPPCTX* pstCtx, void* pvArg);
TSF4G_API int tapp_def_fini(TAPPCTX* pstCtx, void* pvArg);
TSF4G_API int tapp_def_reload(TAPPCTX* pstCtx, void* pvArg);
TSF4G_API int tapp_def_mainloop(TAPPCTX* pstCtx, void* pvArg);

TSF4G_API int tapp_get_category(const char* pszName, int* piCatInst);


#ifdef __cplusplus
}
#endif 

#endif /* TAPP_H */

