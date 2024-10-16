/**
*
* @file     tbus_log.h  
* @brief    BUS日志处理相关接口
* @author jackyai  
* @version 1.0
* @date 2008-09-12 
*
*
* All rights reserved.
*
*/

#ifndef TBUS_LOG_H
#define TBUS_LOG_H

#include "tlog/tlog.h"
#include "tdr/tdr.h"
#include "tbus_kernel.h"

#ifdef __cplusplus
extern "C" {
#endif

extern LPTLOGCATEGORYINST	g_pstBusLogCat;
extern TBUSGLOBAL g_stBusGlobal;

#ifndef WIN32
#define tbus_log(priority, fmt, args...)  \
	do{\
	if(tlog_category_is_priority_enabled(g_pstBusLogCat, priority))\
{\
	TLOGEVENT stEvt;\
	const TLOGLOCINFO locinfo = TLOG_LOC_INFO_INITIALIZER(NULL);\
	stEvt.evt_priority = priority; \
	stEvt.evt_id = 0; \
	stEvt.evt_cls  = 0; \
	stEvt.evt_is_msg_binary = 0; \
	stEvt.evt_loc = &locinfo; \
	tlog_category_logv(g_pstBusLogCat, &stEvt, fmt, ##args);\
}\
	}while(0)
#else

#define tbus_log(priority, fmt, ...)  \
	do{\
		if(tlog_category_is_priority_enabled(g_pstBusLogCat, priority))\
		{\
			TLOGEVENT stEvt;\
			const TLOGLOCINFO locinfo = TLOG_LOC_INFO_INITIALIZER(NULL);\
			stEvt.evt_priority = priority; \
			stEvt.evt_id = 0; \
			stEvt.evt_cls  = 0; \
			stEvt.evt_is_msg_binary = 0; \
			stEvt.evt_loc = &locinfo; \
			tlog_category_logv(g_pstBusLogCat, &stEvt, fmt, __VA_ARGS__);\
		}\
	}while(0)
#endif/*#ifndef WIN32*/


#define tbus_log_data(a_priority, a_pstMeta, a_pData, a_iDataLen) \
do{																  \
	if(tlog_category_is_priority_enabled(g_pstBusLogCat, (a_priority)))\
	{																\
		char _szBuff[1024];											\
		TDRDATA _stHost;												\
		TDRDATA _stOut;												\
		assert(NULL != a_pstMeta);									\
		assert(NULL != a_pData);									\
		_stOut.iBuff = sizeof(_szBuff);								\
		_stOut.pszBuff = &_szBuff[0];									\
		_stHost.iBuff = a_iDataLen;									\
		_stHost.pszBuff = (char *)a_pData;									\
		tdr_sprintf(a_pstMeta, &_stOut, &_stHost, 0);					\
		if (0 < _stOut.iBuff)										\
		{															\
			if (_stOut.iBuff < (int)sizeof(_szBuff))					\
			{														\
				_szBuff[_stOut.iBuff] = '\0';							\
			}else													\
			{														\
				_szBuff[sizeof(_szBuff) -1] = '\0';					\
			}														\
			tbus_log((a_priority), "%s\n", _szBuff);		\
		}/*if (0 < stOut.iBuff)*/									\
	}																\
}while(0)
/** initialize log system for tbus
*@return 0: success, !0: failed
*/
int tbus_init_log();

#ifdef __cplusplus
}
#endif

#endif /*TBUS_LOG_H*/
