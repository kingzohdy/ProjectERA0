/*
**  @file $RCSfile: tlog_buffer.h,v $
**  general description of this module
**  $Id: tlog_buffer.h,v 1.1.1.1 2011/01/05 10:59:29 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2011/01/05 10:59:29 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TLOG_BUFFER_H
#define TLOG_BUFFER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define TLOG_BUFFER_SIZE_DEFAULT  4096

typedef struct
{
    size_t buf_size;
    size_t buf_maxsize;
    char*  buf_data;
} tlog_buffer_t;

typedef  tlog_buffer_t 		TLOGBUFFER;
typedef  tlog_buffer_t 		*LPTLOGBUFFER;

#ifdef __cplusplus
}
#endif

#endif /* TLOG_BUFFER_H */


