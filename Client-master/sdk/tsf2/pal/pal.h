/*
**  @file $RCSfile: pal.h,v $
**  general description of this module
**  $Id: pal.h,v 1.2 2008/11/12 04:05:35 jackyai Exp $
**  @author $Author: jackyai $
**  @date $Date: 2008/11/12 04:05:35 $
**  @version $Revision: 1.2 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef PAL_H
#define PAL_H

/* automatically include the correct library on windows */
#ifdef WIN32

# ifdef _DEBUG
#  define TSF4G_LIB_D    "_d"
# else
#  define TSF4G_LIB_D
# endif /* _DEBUG */


#if defined(TSF4G_LIB_D)
#pragma comment(lib, "libpal"TSF4G_LIB_D".lib" )
#else
#pragma comment(lib, "libpal.lib")
#endif

#endif/*#ifdef WIN32*/

#include "pal/tapi_external.h"
#include "pal/tos.h"
#include "pal/ttypes.h"
#include "pal/tstdio.h"
#include "pal/tstdlib.h"
#include "pal/tstring.h"
#include "pal/trules.h"
#include "pal/terrno.h"

#include <stddef.h>
#include <assert.h>
#include <stdarg.h>

#include "pal/tuio.h"
#include "pal/tdlfcn.h"
#include "pal/getopt.h"
#include "pal/ttime.h"
#include "pal/tfile.h"
#include "pal/tlock.h"
#include "pal/tmutex.h"
#include "pal/tipc.h"
#include "pal/tshm.h"
#include "pal/tsem.h"
#include "pal/tmsgque.h"
#include "pal/tmmap.h"
#include "pal/tsocket.h"
#include "pal/tinet.h"
#include "pal/tnet.h"
#include "pal/tpoll.h"
#include "pal/tfork.h"
#include "pal/tthread.h"
#include "pal/tdirent.h"
#include "pal/tsemaphore.h"
#include "pal/tsignal.h"
#include "pal/texec.h"
#include "pal/twait.h"
#include "pal/pcreposix.h"

#endif /* PAL_H */
