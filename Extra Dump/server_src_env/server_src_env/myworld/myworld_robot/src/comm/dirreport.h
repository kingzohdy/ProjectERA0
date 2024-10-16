#ifndef _DIR_REPORT_ 
#define _DIR_REPORT_


#include "proto_comm.h"
#include "proto_ss.h"


int init_report_interface(int *pHandle);
void destory_reprot_interface(int pHandle);
int record_data(int iworldID, int iOnlineNum);
int report_data(int iHandle,int iTimeOut);

#endif