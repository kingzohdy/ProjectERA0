/*
**  @file $RCSfile: tsemaphore.h,v $
**  general description of this module
**  $Id: tsemaphore.h,v 1.3 2007/06/01 08:39:55 steve Exp $
**  @author $Author: steve $
**  @date $Date: 2007/06/01 08:39:55 $
**  @version $Revision: 1.3 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TSEMAPHORE_H
#define TSEMAPHORE_H

#ifdef WIN32

#include "pal/tos.h"

#define SEM_FAILED -1
#define SEM_VALUE_MAX 0x7fffffff

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct 
{
	HANDLE sem;
	int lockedOrReferenced;
} sem_t;

int sem_init(sem_t *, int, unsigned);
int sem_close(sem_t *);
int sem_destroy(sem_t *);
int sem_getvalue(sem_t *, int *);
sem_t *sem_open(const char *, int, ...);
int sem_post(sem_t *);
int sem_timedwait(sem_t *, const struct timespec *);
int sem_trywait(sem_t *);
int sem_unlink(const char *);
int sem_wait(sem_t *);


#ifdef __cplusplus
}
#endif

#else /* WIN32 */
#include <semaphore.h>
#endif /* WIN32 */

#endif /* TSEMAPHORE_H */
