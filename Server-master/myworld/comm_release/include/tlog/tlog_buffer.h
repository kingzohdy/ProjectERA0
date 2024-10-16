/*
**  @file $RCSfile: tlog_buffer.h,v $
**  general description of this module
**  $Id: tlog_buffer.h,v 1.4 2013/03/29 02:51:06 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2013/03/29 02:51:06 $
**  @version $Revision: 1.4 $
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

#define TLOG_BUFFER_SIZE_DEFAULT  102400

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


