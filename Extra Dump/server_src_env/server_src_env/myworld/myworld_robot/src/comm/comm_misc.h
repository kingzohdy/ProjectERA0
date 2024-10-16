#ifndef COMM_MISC_H
#define COMM_MISC_H

#include "pal/pal.h" 
#include "tdr/tdr.h"
#include "pal/pal.h"
#include "proto_comm.h"
#include "proto_ss.h"

struct tagSSHandle
{
	LPTDRMETALIB pstSSProtoLib;
	LPTDRMETA pstSSProtoPkgMeta;
};

typedef struct  tagSSHandle		SSHANDLE;
typedef struct  tagSSHandle		*LPSSHANDLE;


int set_rlimit(unsigned int uiMaxConn);

int ss_han_create(unsigned char *pszLib, LPSSHANDLE *ppSSHandle);
int ss_han_destroy(LPSSHANDLE *ppSSHandle);
int ss_unpack(SSHANDLE  *pstSSHandler, char *pszBuf, int iBuf, SSPKG *pstPkg, int iVersion);
int ss_pack(SSHANDLE  *pstSSHandler, TDRDATA *pstNet, SSPKG *pstPkg, int iVersion);


#endif /* COMM_MISC_H */
