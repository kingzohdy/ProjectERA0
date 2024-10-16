/*
**  @file $RCSfile: tlog.h,v $
**  general description of this module
**  $Id: tlog.h,v 1.1.1.1 2011/01/04 11:02:51 niko Exp $
**  @author $Author: niko $
**  @date $Date: 2011/01/04 11:02:51 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TLOG_H
#define TLOG_H

/* automatically include the correct library on windows */
#ifdef WIN32

# ifdef _DEBUG
#  define TSF4G_LIB_D    "_d"
# else
#  define TSF4G_LIB_D
# endif /* _DEBUG */


#if defined(TSF4G_LIB_D)
#pragma comment(lib, "libtlog"TSF4G_LIB_D".lib" )
#else
#pragma comment(lib, "libtlog.lib")
#endif

#endif/*#ifdef WIN32*/

#include "tdr/tdr.h"
#include "tlog/tlogerr.h"
#include "tlog/tlogfilter.h"
#include "tlog/tlogfile.h"
#include "tlog/tlognet.h"
#include "tlog/tlogvec.h"
#include "tlog/tlogany.h"
#include "tlog/tlog_category.h"
#include "tlog/tlogconf.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define TLOG_CHAR_DOT		'.'

struct tagTLogConfInst
{
	time_t tLastSync;
	int iCount;
	TLOGCATEGORYINST astCatInsts[TLOG_MAX_CATEGORY];
};

typedef struct tagTLogConfInst	TLOGCONFINST;
typedef struct tagTLogConfInst	*LPTLOGCONFINST;


struct tagTLogCtx
{
	TLOGCONF* pstConf;
	TLOGCONFINST stInst;
};

typedef struct tagTLogCtx		TLOGCTX;
typedef struct tagTLogCtx		*LPTLOGCTX;

const void* tlog_get_meta_data(void);

TLOGCTX* tlog_alloc(void);
int tlog_free(TLOGCTX** a_ppstCtx);

int tlog_init(TLOGCTX* a_pstCtx, TLOGCONF* a_pstConf);
int tlog_fini(TLOGCTX* a_pstCtx);

TLOGCATEGORYINST* tlog_get_category(TLOGCTX* a_pstCtx, const char* a_pszName);

#define tlog_category_is_priority_enabled(cat, priority) \
	( (cat) && ( NULL==(cat)->piPriorityHigh || TLOG_PRIORITY_NULL==*(cat)->piPriorityHigh || (priority)>=*(cat)->piPriorityHigh ) \
	&& ( NULL==(cat)->piPriorityLow || TLOG_PRIORITY_NULL==*(cat)->piPriorityLow || (priority)<=*(cat)->piPriorityLow ) )

#ifndef WIN32

#define tlog_log(cat, priority, id, cls, fmt, args...)\
	do{\
		if(tlog_category_is_priority_enabled(cat, priority))\
		{\
			TLOGEVENT stEvt;\
			const TLOGLOCINFO locinfo = TLOG_LOC_INFO_INITIALIZER(NULL);\
    		stEvt.evt_priority = priority; \
    		stEvt.evt_id = id; \
    		stEvt.evt_cls  = cls; \
		    stEvt.evt_is_msg_binary = 0; \
		    stEvt.evt_loc = &locinfo; \
			tlog_category_logv(cat, &stEvt, fmt, ##args);\
		}\
	}while(0)

#define tlog_fatal(cat, id, cls, fmt, args...)		tlog_log(cat, TLOG_PRIORITY_FATAL, id, cls, fmt, ##args)
#define tlog_alert(cat, id, cls, fmt, args...)		tlog_log(cat, TLOG_PRIORITY_ALERT, id, cls, fmt, ##args)
#define tlog_crit(cat, id, cls, fmt, args...)		tlog_log(cat, TLOG_PRIORITY_CRIT, id, cls, fmt, ##args)
#define tlog_error(cat, id, cls, fmt, args...)		tlog_log(cat, TLOG_PRIORITY_ERROR, id, cls, fmt, ##args)
#define tlog_warn(cat, id, cls, fmt, args...)		tlog_log(cat, TLOG_PRIORITY_WARN, id, cls, fmt, ##args)
#define tlog_notice(cat, id, cls, fmt, args...)		tlog_log(cat, TLOG_PRIORITY_NOTICE, id, cls, fmt, ##args)
#define tlog_info(cat, id, cls, fmt, args...)		tlog_log(cat, TLOG_PRIORITY_INFO, id, cls, fmt, ##args)
#define tlog_debug(cat, id, cls, fmt, args...)		tlog_log(cat, TLOG_PRIORITY_DEBUG, id, cls, fmt, ##args)
#define tlog_trace(cat, id, cls, fmt, args...)		tlog_log(cat, TLOG_PRIORITY_TRACE, id, cls, fmt, ##args)

#else /* WIN32 */

#if  _MSC_VER < 1400  /*vc2005*/
//# error "Please use a newer compiler that support __VA_ARGS__."

#else

	#define tlog_log(cat, priority, id, cls, fmt, ...)\
		do{\
			if(tlog_category_is_priority_enabled(cat, priority))\
			{\
				TLOGEVENT stEvt;\
				const TLOGLOCINFO locinfo = TLOG_LOC_INFO_INITIALIZER(NULL);\
    			stEvt.evt_priority = priority; \
    			stEvt.evt_id = id; \
    			stEvt.evt_cls  = cls; \
				stEvt.evt_is_msg_binary = 0; \
				stEvt.evt_loc = &locinfo; \
				tlog_category_logv(cat, &stEvt, fmt, __VA_ARGS__);\
			}\
		}while(0)

#define tlog_fatal(cat, id, cls, fmt, ...)		tlog_log(cat, TLOG_PRIORITY_FATAL, id, cls, fmt, __VA_ARGS__)
#define tlog_alert(cat, id, cls, fmt, ...)		tlog_log(cat, TLOG_PRIORITY_ALERT, id, cls, fmt, __VA_ARGS__)
#define tlog_crit(cat, id, cls, fmt, ...)		tlog_log(cat, TLOG_PRIORITY_CRIT, id, cls, fmt, __VA_ARGS__)
#define tlog_error(cat, id, cls, fmt, ...)		tlog_log(cat, TLOG_PRIORITY_ERROR, id, cls, fmt, __VA_ARGS__)
#define tlog_warn(cat, id, cls, fmt, ...)		tlog_log(cat, TLOG_PRIORITY_WARN, id, cls, fmt, __VA_ARGS__)
#define tlog_notice(cat, id, cls, fmt, ...)		tlog_log(cat, TLOG_PRIORITY_NOTICE, id, cls, fmt, __VA_ARGS__)
#define tlog_info(cat, id, cls, fmt, ...)		tlog_log(cat, TLOG_PRIORITY_INFO, id, cls, fmt, __VA_ARGS__)
#define tlog_debug(cat, id, cls, fmt, ...)		tlog_log(cat, TLOG_PRIORITY_DEBUG, id, cls, fmt, __VA_ARGS__)
#define tlog_trace(cat, id, cls, fmt, ...)		tlog_log(cat, TLOG_PRIORITY_TRACE, id, cls, fmt, __VA_ARGS__)

#endif  /* _MSC_VER < 1400  , vc2005*/

#endif /* WIN32 */



#define tlog_log_bin(cat, priority, id, cls, type, version, buff, bufflen)\
	do{\
		if(tlog_category_is_priority_enabled(cat, priority))\
		{\
			TLOGEVENT stEvt;\
			const TLOGLOCINFO locinfo = TLOG_LOC_INFO_INITIALIZER(NULL);\
    		stEvt.evt_priority = priority; \
    		stEvt.evt_id = id; \
    		stEvt.evt_cls  = cls; \
    		stEvt.evt_type = type; \
    		stEvt.evt_version = version; \
		    stEvt.evt_is_msg_binary = 1; \
		    stEvt.evt_msg = buff; \
		    stEvt.evt_msg_len = bufflen; \
		    stEvt.evt_loc = &locinfo; \
			tlog_category_log(cat, &stEvt);\
		}\
	}while(0)

#define tlog_fatal_bin(cat, id, cls, type, version, buff, len)		tlog_log_bin(cat, TLOG_PRIORITY_FATAL, id, cls, type, version, buff, len)
#define tlog_alert_bin(cat, id, cls, type, version, buff, len)		tlog_log_bin(cat, TLOG_PRIORITY_ALERT, id, cls, type, version, buff, len)
#define tlog_crit_bin(cat, id, cls, type, version, buff, len)		tlog_log_bin(cat, TLOG_PRIORITY_CRIT, id, cls, type, version, buff, len)
#define tlog_error_bin(cat, id, cls, type, version, buff, len)		tlog_log_bin(cat, TLOG_PRIORITY_ERROR, id, cls, type, version, buff, len)
#define tlog_warn_bin(cat, id, cls, type, version, buff, len)		tlog_log_bin(cat, TLOG_PRIORITY_WARN, id, cls, type, version, buff, len)
#define tlog_notice_bin(cat, id, cls, type, version, buff, len)		tlog_log_bin(cat, TLOG_PRIORITY_NOTICE, id, cls, type, version, buff, len)
#define tlog_info_bin(cat, id, cls, type, version, buff, len)		tlog_log_bin(cat, TLOG_PRIORITY_INFO, id, cls, type, version, buff, len)
#define tlog_debug_bin(cat, id, cls, type, version, buff, len)		tlog_log_bin(cat, TLOG_PRIORITY_DEBUG, id, cls, type, version, buff, len)
#define tlog_trace_bin(cat, id, cls, type, version, buff, len)		tlog_log_bin(cat, TLOG_PRIORITY_TRACE, id, cls, type, version, buff, len)

#define tlog_log_text(cat, priority, id, cls, buff, bufflen)\
	do{\
		if(tlog_category_is_priority_enabled(cat, priority))\
		{\
			TLOGEVENT stEvt;\
			const TLOGLOCINFO locinfo = TLOG_LOC_INFO_INITIALIZER(NULL);\
    		stEvt.evt_priority = priority; \
    		stEvt.evt_id = id; \
    		stEvt.evt_cls  = cls; \
		    stEvt.evt_is_msg_binary = 0; \
		    stEvt.evt_msg = buff; \
		    stEvt.evt_msg_len = bufflen; \
		    stEvt.evt_loc = &locinfo; \
			tlog_category_log(cat, &stEvt);\
		}\
	}while(0)

#define tlog_fatal_text(cat, id, cls, buff, len)		tlog_log_text(cat, TLOG_PRIORITY_FATAL, id, cls, buff, len)
#define tlog_alert_text(cat, id, cls, buff, len)		tlog_log_text(cat, TLOG_PRIORITY_ALERT, id, cls, buff, len)
#define tlog_crit_text(cat, id, cls, buff, len)			tlog_log_text(cat, TLOG_PRIORITY_CRIT, id, cls, buff, len)
#define tlog_error_text(cat, id, cls, buff, len)		tlog_log_text(cat, TLOG_PRIORITY_ERROR, id, cls, buff, len)
#define tlog_warn_text(cat, id, cls, buff, len)			tlog_log_text(cat, TLOG_PRIORITY_WARN, id, cls, buff, len)
#define tlog_notice_text(cat, id, cls, buff, len)		tlog_log_text(cat, TLOG_PRIORITY_NOTICE, id, cls, buff, len)
#define tlog_info_text(cat, id, cls, buff, len)			tlog_log_text(cat, TLOG_PRIORITY_INFO, id, cls, buff, len)
#define tlog_debug_text(cat, id, cls, buff, len)		tlog_log_text(cat, TLOG_PRIORITY_DEBUG, id, cls, buff, len)
#define tlog_trace_text(cat, id, cls, buff, len)		tlog_log_text(cat, TLOG_PRIORITY_TRACE, id, cls, buff, len)

#define tlog_log_str(cat, priority, id, cls, str)		tlog_log_text(cat, priority, id, cls, str, strlen(str))

#define tlog_fatal_str(cat, id, cls, str)			tlog_log_str(cat, TLOG_PRIORITY_FATAL, id, cls, str)
#define tlog_alert_str(cat, id, cls, str)			tlog_log_str(cat, TLOG_PRIORITY_ALERT, id, cls, str)
#define tlog_crit_str(cat, id, cls, str)			tlog_log_str(cat, TLOG_PRIORITY_CRIT, id, cls, str)
#define tlog_error_str(cat, id, cls, str)			tlog_log_str(cat, TLOG_PRIORITY_ERROR, id, cls, str)
#define tlog_warn_str(cat, id, cls, str)			tlog_log_str(cat, TLOG_PRIORITY_WARN, id, cls, str)
#define tlog_notice_str(cat, id, cls, str)			tlog_log_str(cat, TLOG_PRIORITY_NOTICE, id, cls, str)
#define tlog_info_str(cat, id, cls, str)			tlog_log_str(cat, TLOG_PRIORITY_INFO, id, cls, str)
#define tlog_debug_str(cat, id, cls, str)			tlog_log_str(cat, TLOG_PRIORITY_DEBUG, id, cls, str)
#define tlog_trace_str(cat, id, cls, str)			tlog_log_str(cat, TLOG_PRIORITY_TRACE, id, cls, str)

#define tlog_log_dr(a_cat, a_priority, a_id, a_cls, a_pmeta, a_buff, a_bufflen, a_version)\
	do{\
		if(tlog_category_is_priority_enabled(a_cat, a_priority))\
		{\
			TLOGEVENT stEvt;\
			const TLOGLOCINFO locinfo = TLOG_LOC_INFO_INITIALIZER(NULL);\
			char _szBuff[TLOG_BUFFER_SIZE_DEFAULT];					\
			int _iBuff;																			\
			TDRDATA _stHost;																\
			TDRDATA _stOut;																	\
			_iBuff = sizeof(_szBuff);												\
			if( a_cat->pstCat->iMaxMsgSize<= _iBuff ) 	\
			{																								\
				_stOut.pszBuff = &_szBuff[0];									\
			}																								\
			else																						\
			{																								\
				_iBuff = a_cat->pstCat->iMaxMsgSize;						\
				_stOut.pszBuff = (char *)alloca(_iBuff);				\
				if( !_stOut.pszBuff )													\
				{																							\
					_stOut.pszBuff = &_szBuff[0];								\
					_iBuff = sizeof(_szBuff);										\
				}																							\
			}																								\
			_stOut.iBuff = _iBuff;														\
			_stHost.iBuff = a_bufflen;												\
			_stHost.pszBuff = (char *)a_buff;									\
			tdr_sprintf(a_pmeta, &_stOut, &_stHost, a_version);					\
			if (_stOut.iBuff < (int)_iBuff)					\
			{															\
				_stOut.pszBuff[_stOut.iBuff] = '\0';							\
			}else													\
			{														\
				_stOut.pszBuff[_iBuff -1] = '\0';					\
			}														\
    	stEvt.evt_priority = a_priority; \
    	stEvt.evt_id = a_id; \
    	stEvt.evt_cls  = a_cls; \
			stEvt.evt_is_msg_binary = 0; \
			stEvt.evt_msg = &_stOut.pszBuff[0]; \
			stEvt.evt_msg_len = _stOut.iBuff; \
			stEvt.evt_loc = &locinfo; \
			tlog_category_log(a_cat, &stEvt);\
		}\
	}while(0)

#define tlog_fatal_dr(cat, id, cls, pmeta, buff, bufflen, version)		tlog_log_dr(cat, TLOG_PRIORITY_FATAL, id, cls,  pmeta, buff, bufflen, version)
#define tlog_alert_dr(cat, id, cls, pmeta, buff, bufflen, version)		tlog_log_dr(cat, TLOG_PRIORITY_ALERT,  id, cls, pmeta, buff, bufflen, version)
#define tlog_crit_dr(cat, id, cls, pmeta, buff, bufflen, version)		tlog_log_dr(cat, TLOG_PRIORITY_CRIT,  id, cls, pmeta, buff, bufflen, version)
#define tlog_error_dr(cat, id, cls, pmeta, buff, bufflen, version)		tlog_log_dr(cat, TLOG_PRIORITY_ERROR,  id, cls, pmeta, buff, bufflen, version)
#define tlog_warn_dr(cat, id, cls, pmeta, buff, bufflen, version)		tlog_log_dr(cat, TLOG_PRIORITY_WARN,  id, cls, pmeta, buff, bufflen, version)
#define tlog_notice_dr(cat, id, cls, pmeta, buff, bufflen, version)		tlog_log_dr(cat, TLOG_PRIORITY_NOTICE,  id, cls, pmeta, buff, bufflen, version)
#define tlog_info_dr(cat, id, cls, pmeta, buff, bufflen, version)		tlog_log_dr(cat, TLOG_PRIORITY_INFO,  id, cls, pmeta, buff, bufflen, version)
#define tlog_debug_dr(cat, id, cls, pmeta, buff, bufflen, version)		tlog_log_dr(cat, TLOG_PRIORITY_DEBUG,  id, cls, pmeta, buff, bufflen, version)
#define tlog_trace_dr(cat, id, cls, pmeta, buff, bufflen, version)		tlog_log_dr(cat, TLOG_PRIORITY_TRACE,  id, cls, pmeta, buff, bufflen, version)

#ifdef __cplusplus
}
#endif

#endif /* TLOG_H */


