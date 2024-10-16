#ifndef _CS_CGI_H_
#define _CS_CGI_H_

#include "proto_comm.h"
#include "proto_ss.h"
#include "comm_misc.h"
#include "cgi_conf_desc.h"
//#include "tdr/tdr.h"
//#include "cs_net.h"

struct tagCgiEnv
{
	int *pstConf;
	int iSock;
	int  *pstSSProtoLib;
	int  *pstSSProtoPkgMeta;
};
typedef struct tagCgiEnv  CGIENV;

int init_interface(const char* szSSBinPath, CGIENV **ppstCgiEnv);
//int destory_sshandle(int **ppSSProtoLib,int **ppSSProtoPkgMeta);
void destory_interface(CGIENV *pstCgiEnv);
//int recv_svr(SSHANDLE *pstSSHan, int iSock, SSCSCTRL* pOut, int iTimeOut);
int cs_interface_fun(CGIENV *pstCgiEnv, unsigned int iWolrdidx, SSCSCTRL *pIn,  SSCSCTRL* pOut, int iTimeOut);
//int init_sshandle(const char* szSSBinPath,int **ppSSProtoLib,int **ppSSProtoPkgMeta);

#endif