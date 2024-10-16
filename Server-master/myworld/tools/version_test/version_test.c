


#include "pal/pal.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "cs_net.h"
#include "tlog/tlog.h"

int main(int argc, char *argv[])
{
	CSPKG stIn, stOut;
	NETCLTHANDLE *pstNetCltHan = NULL; 
	int i;

	
	if (argc != 2)
	{
		return -1;
	}

	if (strcmp(argv[1], "1") == 0)
	{
		net_clt_han_create("../../src/comm/proto_cs.meta", "192.168.0.77", 6724, &pstNetCltHan);

		stIn.Head.Cmd = VERSION_CLT;
		stIn.Body.VersionClt.VersionNum = 1;
	}
	else if(strcmp(argv[1], "2") == 0)
	{
		net_clt_han_create("../../src/comm/proto_cs.meta", "192.168.0.77", 6722, &pstNetCltHan);
		stIn.Head.Cmd = DIR_REQ;
		stIn.Body.DirReq.LastVersion = 0;
	}
	
	if (!pstNetCltHan)
	{
		return -1;
	}

	stIn.Head.Magic = PROTOCOL_MAGIC; 
	stIn.Head.HeadLen = sizeof(stIn.Head);
	stIn.Head.Ver = TDR_METALIB_PROTO_CS_VERSION;

	if (0 > net_clt_send(pstNetCltHan, &stIn, 10))
	{
		printf("net_clt_send fail\n");
		return -1;
	}

	if (0 >= net_clt_recv(pstNetCltHan, &stOut, 2000))
	{
		printf("net_clt_recv fail\n");
		return -1;
	}
	else 
	{
		printf("asdjfkljhsdklfhsd\n");
		//printf("IsUp=%d from=%d to=%d\n", stOut.Body.VersionSvr.IsUp, stOut.Body.VersionSvr.VersionFrom, stOut.Body.VersionSvr.VersionTo);
		
	}
		

	net_clt_han_destroy(&pstNetCltHan);
	
	return 0;
}
