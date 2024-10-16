/***************************************************
// Project	: DirServer Core Lib
// Program 	: 
// FileName : dir_mm.c
// Purpose  : The Lib of DirServer, Memory Access ToolKits
****************************************************/
#ifndef _DIR_MM_C_
#define _DIR_MM_C_
//#include "bbs.h"
#include "dir_mm.h"

char* DirGetShm(int iKey, int iSize, int iFlag)
{
	int iShmID;
	char* sShm;
	char sErrMsg[50];

	if ((iShmID = shmget(iKey, iSize, iFlag)) < 0) {
		sprintf(sErrMsg, "shmget %d %d", iKey, iSize);
		perror(sErrMsg);
		return NULL;
	}
	if ((sShm = shmat(iShmID, NULL ,0)) == (char *) -1) {
		perror("shmat");
		return NULL;
	}
	return sShm;
}
#endif
