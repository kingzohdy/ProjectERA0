/*
**  @file $RCSfile: tlog_event.h,v $
**  general description of this module
**  $Id: tlog_event.h,v 1.1.1.1 2011/01/04 11:02:51 niko Exp $
**  @author $Author: niko $
**  @date $Date: 2011/01/04 11:02:51 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TLOG_EVENT_H
#define TLOG_EVENT_H


//#include "pal/pal.h"
#include "tlog/tlog_buffer.h"
#include "tlog/tlog_loc_info.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct 
{
	const char* evt_category;
	int	evt_priority;
	const char* evt_msg;
	const char* evt_rendered_msg;
	tlog_buffer_t evt_buffer;
	struct timeval evt_timestamp;
	const tlog_loc_info_t* evt_loc;
	
	int evt_is_msg_binary;
	int evt_msg_len;
	int evt_rendered_msg_len;
	int evt_id;
	int evt_cls;
	int evt_type;
	int evt_version;
} tlog_event_t;

typedef tlog_event_t	TLOGEVENT;
typedef tlog_event_t	*LPTLOGEVENT;

#ifdef __cplusplus
}
#endif

#endif /* TLOG_EVENT_H */


