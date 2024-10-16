#include "sema.h"

#ifndef _WIN32

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>

/*
union semun {
	int val; 
	struct semid_ds *buf; 
	u_short *array; 
};

int semacquire(void *sem) {
    struct sembuf sb = { 0, -1, SEM_UNDO };

    if (sem && semop(*(int *)sem, &sb, 1) < 0) {
		return -1;
    }

    return 0;
}

int semrelease(void *sem) {
    struct sembuf sb = { 0, 1, SEM_UNDO };

    if (sem && semop(*(int *)sem, &sb, 1) < 0) {
		return -1;
    }

    return 0;
}

int semreset(void *sem, int val) {
    (void)sem;
    (void)val;

    return 0;
}

void semdestroy(void *sem) {
    union semun u;
    u.val = 1;

    semctl(*(int *)sem, 0, IPC_RMID, u);
}

void *semcreate(key_t key, int val) {
    int *semid = (int *)malloc(sizeof(int));
    union semun u;

    u.val = val;

    if (semid == NULL) {
		return NULL;
    }

    if ((*semid = semget(key, 1, 0664)) < 0) {
    	*semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0664);
    	if (*semid < 0)
    	{
			return NULL;
		}
    }

    if (semctl(*semid, 0, SETVAL, u) < 0) {
		return NULL;
    }

    return (void *)semid;
}
*/
#endif
