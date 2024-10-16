/*
**  @file $RCSfile: tuio.h,v $
**  general description of this module
**  $Id: tuio.h,v 1.2 2007/12/13 03:38:10 steve Exp $
**  @author $Author: steve $
**  @date $Date: 2007/12/13 03:38:10 $
**  @version $Revision: 1.2 $
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
