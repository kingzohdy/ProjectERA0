
#ifndef _OI_SHM_H
#define _OI_SHM_H

#include <sys/ipc.h>
#include <sys/shm.h>
#include "common.h"

#define MIN_SHM_KEY	8000
#define	CHANNEL_LOCAL_SHM_KEY	8020
#define	CHANNEL_STATUS_SHM_KEY	8025
#define	CHANNEL_REMOTE_SHM_KEY	8030
#define	CHANNEL_FINGER_SHM_KEY	8035
#define MPLIST_SHM_KEY		8040
#define SERVER_UPDATE_SHM_KEY	8100

char* GetShm(int iKey, int iSize, int iFlag);
int GetShm2(void **pstShm, int iShmID, int iSize, int iFlag);
int GetShm3(void **pstShm, int iShmID, int iSize, int iFlag);

int GetShmAttachByKey(int iKey, int iSize, int iFlag);
int GetShmAttach(int iShmID);

#endif
