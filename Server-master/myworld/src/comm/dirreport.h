#ifndef _DIR_REPORT_ 
#define _DIR_REPORT_


#include "proto_comm.h"
#include "proto_cs.h"
#include "comm_misc.h"
#include "proto_ss.h"

int init_report_interface(unsigned long int *pHandle, int iSec);
void destory_reprot_interface(unsigned long int pHandle);
int record_data(int iRegionID, int iworldID, int iOnlineNum);
int record_tick();
int report_check(unsigned long int pHandle, int iNum, char *pBuff);
int report_get(unsigned long int pHandle, int *piNum, char *pBuff, int iMax);

#endif