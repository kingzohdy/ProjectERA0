
#include "cs_cgi.h"
#include "cs_net.h"
extern unsigned char g_szMetalib_cgi[];

int init_sshandle(const char* szSSBinPath,int **ppSSProtoLib,int **ppSSProtoPkgMeta)
{
	SSHANDLE *pstSSHandle = NULL;
	
	pstSSHandle = (SSHANDLE *)malloc(sizeof(SSHANDLE));
	if (!pstSSHandle)
	{
		return -1;
	}

	if (0 > tdr_load_metalib(&pstSSHandle->pstSSProtoLib, szSSBinPath))
	{
		printf("load meta lib  fail\n");
		free(pstSSHandle);
		return -1;
	}

	pstSSHandle->pstSSProtoPkgMeta = tdr_get_meta_by_name(pstSSHandle->pstSSProtoLib, "SSPkg");
	if (NULL == pstSSHandle->pstSSProtoPkgMeta)
	{
		free(pstSSHandle);
		printf("tdr_get_meta_by_name fail\n");
		return -1;
	}
	else
	{
		*ppSSProtoLib = (int*)pstSSHandle->pstSSProtoLib;
		*ppSSProtoPkgMeta = (int*)pstSSHandle->pstSSProtoPkgMeta;
		return 0;
	}
	
}
int init_interface(const char* szSSBinPath, CGIENV **ppstCgiEnv)
{
	LPTDRMETALIB pstLib = NULL;
	LPTDRMETA pstMeta = NULL;
	TDRDATA stHost;
	CGICONF *pstConf;
	
	CGIENV *pstEnv = NULL;
	pstEnv = (CGIENV*)malloc(sizeof(CGIENV));
	if(pstEnv == NULL)
	{
		return -1;
	}

	pstConf = (CGICONF *)malloc(sizeof(CGICONF));
	if(pstConf == NULL)
	{
		free(pstEnv);
		return -1;
	}
	stHost.iBuff = sizeof(CGICONF);
	stHost.pszBuff = (char *)pstConf;
	
	pstLib = (LPTDRMETALIB)&g_szMetalib_cgi[0];
	pstMeta = tdr_get_meta_by_name(pstLib, "cgiconf");
	if(pstMeta == NULL)
	{
		free(pstEnv);
		free(pstConf);
		printf("tdr_get_meta_by_name fail\n");
		return -1;
	}
	if( tdr_input_file(pstMeta, &stHost, "cs_cgi.xml", 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME) < 0)
	{
		free(pstEnv);
		free(pstConf);
		printf("tdr_input_file fail\n");
		return -1;
	}
	pstEnv->pstConf = (int *)pstConf;
	
	if( init_sshandle(szSSBinPath,&pstEnv->pstSSProtoLib,&pstEnv->pstSSProtoPkgMeta) < 0)
	{
		free(pstEnv);
		free(pstConf);
		return -1;
	}

	*ppstCgiEnv = pstEnv;
	return 0;
}


void destory_interface(CGIENV *pstCgiEnv)
{
	if(pstCgiEnv == NULL)
	{
		return;
	}
	
	if(pstCgiEnv->pstSSProtoLib)
	{
		tdr_free_lib((LPTDRMETALIB*)&pstCgiEnv->pstSSProtoLib);
	}

	if(pstCgiEnv->pstConf)
	{
		free((CGICONF *)pstCgiEnv->pstConf);
		pstCgiEnv->pstConf = NULL;
	}
	
	free(pstCgiEnv);
	pstCgiEnv = NULL;
}

int recv_svr(SSHANDLE *pstSSHan, int iSock, SSCSCTRL* pOut, int iTimeOut)
{
	int iRecvLen;
	SSPKG stPak;
	char szBuff[sizeof(SSPKG)];
	
	if((iRecvLen = tnet_recvall(iSock, szBuff, sizeof(szBuff), iTimeOut)) < 0)
	{
		printf("tnet_recvall fail\n");
		return -1;
	}

	if(ss_unpack(pstSSHan, szBuff, iRecvLen,&stPak, 0) < 0)
	{
		return -1;
	}

	memcpy(pOut, &stPak.Body.CsCtrL, sizeof(SSCSCTRL));

	return 0;
}


int cs_interface_fun(CGIENV *pstCgiEnv, unsigned int iWolrdidx, SSCSCTRL *pIn,  SSCSCTRL* pOut, int iTimeOut)
{
	int i;
	SSPKG stPkg;
	SSPKG stPkgTmp;
	TDRDATA stNet;
	char szUri[128];
	SSHANDLE stSSHan;
	char sBuff[sizeof(SSPKG)];
	CGICONF *pstCgiConf = NULL;

	pstCgiConf = (CGICONF*)pstCgiEnv->pstConf;
	if(pstCgiConf == NULL)
	{
		return -1;
	}
	
	for(i=0; i<pstCgiConf->iCount; i++)
	{
		if(pstCgiConf->astWorldInfos[i].dwIdx == iWolrdidx)
		{
			snprintf(szUri, sizeof(szUri), "tcp://%s:%d", pstCgiConf->astWorldInfos[i].szIp, pstCgiConf->astWorldInfos[i].dwPort);
			break;
		}
	}

	if (i >= pstCgiConf->iCount)
	{
		return -1;
	}

	pstCgiEnv->iSock = tnet_connect(szUri, iTimeOut);
	if( -1 == pstCgiEnv->iSock )
	{
		return -1;
	}
	
	stPkg.Head.Cmd = CS_CTRL;
	stPkg.Head.Magic = PROTOCOL_MAGIC; 
	stPkg.Head.HeadLen = sizeof(stPkg.Head);
	memcpy(&stPkg.Body.CsCtrL, pIn, sizeof(SSCSCTRL));

	stSSHan.pstSSProtoLib = ((LPTDRMETALIB)pstCgiEnv->pstSSProtoLib);
	stSSHan.pstSSProtoPkgMeta = ((LPTDRMETA)pstCgiEnv->pstSSProtoPkgMeta);

	stNet.iBuff = sizeof(sBuff);
	stNet.pszBuff = sBuff;
	
	if(ss_pack(&stSSHan, &stNet, &stPkg, 0) < 0)
	{
		printf("ss_pack fail\n");
		return -1;
	}

	if(tnet_sendall(pstCgiEnv->iSock, stNet.pszBuff, stNet.iBuff, iTimeOut) < 0)
	{
		printf("tnet_sendall fail\n");
		return -1;
	}

	if(recv_svr(&stSSHan, pstCgiEnv->iSock, pOut, iTimeOut) < 0)
	{
		printf("recv_svr fail\n");
		return -1;
	}

	close(pstCgiEnv->iSock);
	
	return 0;
}
