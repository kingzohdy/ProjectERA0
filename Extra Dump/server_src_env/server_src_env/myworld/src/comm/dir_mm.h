/***************************************************
// Project	: DirServer Core Lib
// Program 	: 
// FileName : dir_mm.h
// Purpose  : The Lib of DirServer, Memory Access ToolKits
****************************************************/
#ifndef _DIR_MM_H_
#define _DIR_MM_H_
#include <sys/types.h>	/* basic system data types */
#include <stdlib.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <sys/shm.h>


#ifdef  __cplusplus
extern "C" {
#endif

char* DirGetShm(int iKey, int iSize, int iFlag);


#ifdef  __cplusplus
}
#endif

#endif
