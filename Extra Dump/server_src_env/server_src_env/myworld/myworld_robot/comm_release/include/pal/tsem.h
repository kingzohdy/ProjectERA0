/*
**  @file $RCSfile: tsem.h,v $
**  general description of this module
**  $Id: tsem.h,v 1.1.1.1 2011/01/05 10:59:29 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2011/01/05 10:59:29 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/


#ifndef TSEM_H
#define TSEM_H

#ifdef WIN32
#else /* WIN32 */
#include <sys/ipc.h>
#include <sys/sem.h>
#endif /* WIN32 */


#include "pal/tipc.h"
#include "pal/ttime.h"

#ifdef __cplusplus
extern "C"
{
#endif  /* __cplusplus */

#ifdef WIN32
	/* Flags for `semop'.  */
	#define SEM_UNDO    0x1000      /* undo the operation on exit */

	/* Commands for `semctl'.  */
	#define GETPID      11      /* get sempid */
	#define GETVAL      12      /* get semval */
	#define GETALL      13      /* get all semval's */
	#define GETNCNT     14      /* get semncnt */
	#define GETZCNT     15      /* get semzcnt */
	#define SETVAL      16      /* set semval */
	#define SETALL      17      /* set all semval's */

	/* * ipcs ctl cmds */
	#define SEM_STAT 	18
	#define SEM_INFO 	19

	struct semid_ds
	{
		struct ipc_perm sem_perm;  /* Ownership and permissions */
		time_t sem_otime; 		   /* Last semop time */
		time_t sem_ctime;		   /* Last change time */
		unsigned short sem_nsems;  /* No. of semaphores in set */
	};

	struct  seminfo
	{
		int semmap;
		int semmni;
		int semmns;
		int semmnu;
		int semmsl;
		int semopm;
		int semume;
		int semusz;
		int semvmx;
		int semaem;
	};

	#define SEMMNI  128             /* <= IPCMNI  max # of semaphore identifiers */
	#define SEMMSL  250             /* <= 8 000 max num of semaphores per id */
	#define SEMMNS  (SEMMNI*SEMMSL) /* <= INT_MAX max # of semaphores in system */
	#define SEMOPM  32              /* <= 1 000 max num of ops per semop call */
	#define SEMVMX  32767           /* <= 32767 semaphore maximum value */
	#define SEMAEM  SEMVMX          /* adjust on exit max value */

	/* unused */
	#define SEMUME  SEMOPM          /* max num of undo entries per process */
	#define SEMMNU  SEMMNS          /* num of undo structures system wide */
	#define SEMMAP  SEMMNS          /* # of entries in semaphore map */
	#define SEMUSZ  20              /* sizeof struct sem_undo */

	int semget(key_t key, int nsems, int semflg);

	int semctl(int semid, int semnum, int cmd, ...);

	int semop(int semid, struct sembuf *sops, unsigned nsops);

	int semtimedop(int semid, struct sembuf *sops, unsigned nsops, struct timespec *timeout);

#else /* WIN32 */
#endif	/* WIN32 */

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
	/* union semun is defined by including <sys/sem.h> */
#else
	/* according to X/OPEN we have to define it ourselves */
	union semun 
	{
		int val;                  /* value for SETVAL */
		struct semid_ds *buf;     /* buffer for IPC_STAT, IPC_SET */
		unsigned short *array;    /* array for GETALL, SETALL */
					  			  /* Linux specific part: */
		struct seminfo *__buf;    /* buffer for IPC_INFO */
	};
#endif


#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif	/* TSEM_H */
