/*
**  @file $RCSfile: tuio.h,v $
**  general description of this module
**  $Id: tuio.h,v 1.3 2012/07/27 05:09:07 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2012/07/27 05:09:07 $
**  @version $Revision: 1.3 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TUIO_H

#include "pal/ttypes.h"

#define TUIO_H

#ifdef WIN32

struct iovec {
	void *iov_base;   /* Starting address */
	size_t iov_len;   /* Number of bytes */
};

ssize_t readv(int fd, const struct iovec *vector, int count);
ssize_t writev(int fd, const struct iovec *vector, int count);

#else /* WIN32 */
#endif /* WIN32	*/

#endif /* TUIO_H */
