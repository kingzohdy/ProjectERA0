#if defined(WIN32)
#include <winsock2.h>
#pragma warning(disable:4127)
#pragma warning(disable:4996)
#pragma warning(disable:4244)
#pragma warning(disable:4115)
#pragma warning(disable:4819)
#endif
#include "tdr/tdr.h"
#include "pal/pal.h"
#include "pal/tnet.h"
#include "cs_cgi.h"
#include "mysql/mysql.h"
//#include "comm_misc.h"
#include "cgi_conf_desc.h"

#if defined(WIN32)
#	define INLINE __inline
#else
#	define INLINE inline
#endif

struct tagSSContext
{
	LPTDRMETALIB pstSSProtoLib;
	LPTDRMETA pstSSProtoPkgMeta;
};

typedef struct  tagSSContext		SSCONTEXT;
typedef struct  tagSSContext		*LPSSCONTEXT;

static INLINE int cgi_ss_unpack(SSCONTEXT *pstSSHandler, char *pszBuf, int iBuf,
                                SSPKG *pstPkg, int iVersion)
{
	TDRDATA stHost;
	TDRDATA stNet;

	stNet.pszBuff = pszBuf;
	stNet.iBuff = iBuf;
	stHost.pszBuff = (char *)pstPkg;
	stHost.iBuff = sizeof(SSPKG);
	return tdr_ntoh(pstSSHandler->pstSSProtoPkgMeta, &stHost, &stNet, iVersion);
}

static INLINE int cgi_ss_pack(SSCONTEXT *pstSSHandler, TDRDATA *pstNet,
                              SSPKG *pstPkg, int iVersion)
{
	TDRDATA stHost;

	stHost.iBuff = sizeof(SSPKG);
	stHost.pszBuff = (char *)pstPkg;
	pstPkg->Head.Ver = TDR_METALIB_PROTO_SS_VERSION;
	return tdr_hton(pstSSHandler->pstSSProtoPkgMeta,  pstNet, &stHost, iVersion);
}




struct tagCgiEnv
{
	int *pstConf;
	//int iSock;
	int  *pstSSProtoLib;
	int  *pstSSProtoPkgMeta;
};
typedef struct tagCgiEnv  CGIENV;


extern unsigned char g_szMetalib_cgi[];

static int interface_fun_in(int *pHandle, const char * pszUri, SSCSCTRL *pIn, 
	                          SSCSCTRL* pOut, int iTimeOut);

static int cs_interface_fun(int *pHandle, int iRegionid, unsigned int iWolrdidx, 
	                        SSCSCTRL *pIn,  SSCSCTRL* pOut, int iTimeOut);

static int acc_interface_fun(int *pHandle, int iRegionid, SSCSCTRL *pIn,
                             SSCSCTRL* pOut, int iTimeOut);

static FIND_ACCOUNT_HANDLE create_find_account_handle(int *pHandle,
                                                      const char *pszPattern,
                                                      int iTimeOut, int iByWhat);

static int shut_acc_mouth(int *pHandle, int iRegionid, int iWolrdID, 
                          const char *pszLoginName, int fEnable,
                          int iSilenceTime, int iTimeOut);

static int shut_all_mouth_by_loginname(int *pHandle, int iRegionID,
                                       const char *pszLoginName, int fEnable,
                                       int iSilenceTime, int iTimeOut);

static int kick_acc_off(int *pHandle, int iRegionid, int iWolrdID,
                        const char *pszLoginName, int iBlockTime, int iTimeOut);

static int kick_off_all_by_loginname(int *pHandle, int iRegionID,
                                     const char *pszLoginName,
                                     int iFreezeTime, int iTimeOut);

static void account_id_copy_from(const ACCFINDROLELISTRES *pstFrom,
                                 CSAccountIDRes *pstTo);

static int get_account_role_by(
	int *pHandle,
	int iRegionID,
	int iWorldID,
	int iByWhat,
	const char *pszPattern,
	int iIndex,
	ACCFINDROLELISTRES *pstOut,
	int iTimeOut);

static FIND_ACCOUNT_ROLE_HANDLE create_find_account_role_handle(
	int *pHandle,
	int iRegionID,
	int iWorldID,
	const char *pszPattern,
	int iTimeOut,
	int iByWhat);

static int get_account_by(
	int *pHandle,
	int iRegionID,
	int iByWhat,
	const char *pszPattern,
	int iIndex,
	ACCFINDRES *pstOut,
	int iTimeOut);

static FIND_CLAN_HANDLE create_find_clan_handle(
	int *pHandle,
	int iRegionID,
	int iWorldID,
	const char *pszPattern,
	int iTimeOut);

static int get_clan_list(
	int *pHandle,
	int iRegionID,
	int iWorldID,
	const char *pszClanNamePattern,
	int iIndex,
	ZONEFINDCLANRES *pstOut,
	int iTimeOut);

enum FIND_ACC_BYTHEWAY
{
	BY_ROLE_NAME,
	BY_LOGIN_NAME,
};

struct _FindAccountContext
{
	int *pHandle;
	int iTimeOut;
	char szPattern[128];
	int aiReginID[MAX_REGIONNUM];
	int iMax;
	int iIndex;
	int iQueryIndex;
	int iPrevCount;
	int iByWhat;
};

struct _FindAccountRoleContext
{
	int *pHandle;
	int iTimeOut;
	SSFINDACCOUNTROLELISTKEY stKey;
	int iRegionID;
	int iWorldID;
	int iPrevCount;
	int iQueryIndex;
	int iByWhat;
};

struct _LimitedFindAccountContext
{
	int *pHandle;
	int iTimeOut;
	char szPattern[ACCOUNT_LOGIN_NAME_LEN + 1];
	int iRegionID;
	int iPrevCount;
	int iQueryIndex;
};

struct _FindMaillistContext
{
	int *pHandle;
	int iTimeOut;
	char szRoleName[ROLE_NAME_LEN + 1];
	int iRegionID;
	int iWorldID;
	int iPrevCount;
	int iQueryIndex;
};

struct _FindClanContext
{
	int *pHandle;
	int iTimeOut;
	char szClanNamePattern[NAME_LEN];
	int iRegionID;
	int iWorldID;
	int iPrevCount;
	int iQueryIndex;
	int iByWhat;
};

static int cs_gen_bulletin_id()
{
	static int iN = 0;
	int iCurr = time(NULL);
	int iRet = 0;

	iN = (iN + 1) % 10807;
	srand(iCurr);
	iRet = (((rand() * iN) & 0x00ffff00) << 8) | ((iCurr + iN) & 0x0000ffff);

	return iRet;
}


int init_analy_interface(const char* szMetaPath, int **ppHandle)
{
	if (!ppHandle)
	{
		return -1;
	}

	if (0 > tdr_load_metalib((LPTDRMETALIB*)ppHandle, szMetaPath))
	{
		printf("load meta lib  fail\n");
		return -1;
	}

	return 0;
	
}

int analy_interface(int *pHandle, char *pszBlobMetaName,char *pszBlobBuff
								,int iBlobBuff, char *pszXml, int* piXml)
{
	TDRDATA stData;
	int iVersion = 0;
	LPTDRMETA pstMeta = NULL;
	TDRDATA stXml;
	TDRDATA stHost;
	int iRet = 0;

	if(!pHandle || !pszBlobMetaName || !pszBlobBuff || !pszXml || (unsigned int)iBlobBuff < 2*sizeof(long) || *piXml < 1)
	{
		return CS_ERR_ARGS;
	}
	
	iVersion = ntohl(*((long*)pszBlobBuff));
	pszBlobBuff += sizeof(long);

	iBlobBuff = ntohl(*((long*)pszBlobBuff));
	pszBlobBuff += sizeof(long);

	stData.iBuff = iBlobBuff;
	stData.pszBuff = pszBlobBuff;

	stXml.iBuff = *piXml;
	stXml.pszBuff = pszXml;

	pstMeta = tdr_get_meta_by_name((LPTDRMETALIB) pHandle, pszBlobMetaName);
	if (NULL == pstMeta)
	{
		return CS_ERR_ARGS;
	}
	
	stHost.iBuff = tdr_get_meta_size(pstMeta);
	stHost.pszBuff = NULL;
	stHost.pszBuff = malloc(stHost.iBuff);
	if (NULL == stHost.pszBuff)
	{
		return CS_ERR_ARGS;
	}	
		
	tdr_ntoh(pstMeta, &stHost, &stData, iVersion);
	iRet = tdr_output(pstMeta, &stXml, &stHost,iVersion, TDR_IO_NEW_XML_VERSION);
	
	*piXml = stXml.iBuff;

	if (stHost.pszBuff)
	{
		free(stHost.pszBuff);
		stHost.pszBuff = NULL;
	}
	return iRet;
}

int fini_analy_interface(int **ppHandle)
{
	if (!ppHandle)
	{
		return -1;
	}
	*ppHandle = NULL;
	return 0;
}


static int init_sshandle(const char* szSSBinPath,int **ppSSProtoLib,int **ppSSProtoPkgMeta)
{
	SSCONTEXT *pstSSHandle = NULL;
	
	pstSSHandle = malloc(sizeof(SSCONTEXT));
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

int init_interface(const char* szSSBinPath, int  **ppHandle,const char* szXmlPath)
{
	LPTDRMETALIB pstLib = NULL;
	LPTDRMETA pstMeta = NULL;
	TDRDATA stHost;
	CGICONF *pstConf;
	CGIENV *pstEnv = NULL;

	tsocket_init(2, 0);

	pstEnv = (CGIENV*)malloc(sizeof(CGIENV));
	if(pstEnv == NULL)
	{
		return CS_ERR_SYS;
	}

	pstConf = (CGICONF *)malloc(sizeof(CGICONF));
	if(pstConf == NULL)
	{
		free(pstEnv);
		return CS_ERR_SYS;
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
		return CS_ERR_SYS;
	}
	if( tdr_input_file(pstMeta, &stHost, szXmlPath, 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME) < 0)
	{
		free(pstEnv);
		free(pstConf);
		printf("tdr_input_file fail\n");
		return CS_ERR_SYS;
	}
	pstEnv->pstConf = (int *)pstConf;
	
	if( init_sshandle(szSSBinPath,&pstEnv->pstSSProtoLib,&pstEnv->pstSSProtoPkgMeta) < 0)
	{
		free(pstEnv);
		free(pstConf);
		return CS_ERR_SYS;
	}
	*ppHandle = (int *)pstEnv;
	return 0;
}

int cs_get_regionworld_info(int *pHandle, REGIONWORLDINFO *pstRegionWorldInfo)
{
	CGICONF *pstCgiConf = NULL;
	CGIENV *pstCgiEnv = (CGIENV*)(pHandle);

	if (!pHandle || !pstRegionWorldInfo)
		return CS_ERR_ARGS;
	
	pstCgiConf = (CGICONF*)pstCgiEnv->pstConf;
	if(pstCgiConf == NULL)
	{
		return CS_ERR_ARGS;
	}

	pstRegionWorldInfo->AccountCount = pstCgiConf->AccountCount;
	pstRegionWorldInfo->WorldCount = pstCgiConf->WorldCount;
	memcpy(pstRegionWorldInfo->AccountInfos, pstCgiConf->AccountInfos, sizeof(pstRegionWorldInfo->AccountInfos));
	memcpy(pstRegionWorldInfo->WorldInfos, pstCgiConf->WorldInfos, sizeof(pstRegionWorldInfo->WorldInfos));
	
	return 0;
}

void destory_interface(int  *pHandle)
{
	CGIENV *pstCgiEnv = NULL;

	if(pHandle == NULL)
	{
		return;
	}

	pstCgiEnv = (CGIENV*)(pHandle);
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

	tsocket_fini();
}

static int recv_svr(SSCONTEXT *pstSSHan, int iSock, SSCSCTRL* pOut, int iTimeOut)
{
	int iRecvLen;
	SSPKG *pstPak = (SSPKG*)malloc(sizeof(SSPKG));
	char *pszBuff = (char*)malloc(sizeof(SSPKG));
	ERRRES *pstErrRes;
	int iRet;

	if(!pstPak || !pszBuff)
	{
		iRet =  CS_ERR_SYS;
		goto final;
	}
	
	if((iRecvLen = tnet_recvall(iSock, pszBuff, sizeof(SSPKG), iTimeOut)) <= 0)
	{
		printf("tnet_recvall fail\n");
		if (iRecvLen == 0)
		{
			iRet =  CS_ERR_TIME_OUT;
			goto final;
		}
		else if (iRecvLen < 0)
		{
			iRet =  CS_ERR_NET;
			goto final;
		}
	}

	if(cgi_ss_unpack(pstSSHan, pszBuff, iRecvLen,pstPak, 0) < 0)
	{
		iRet =  CS_ERR_TDR;
		goto final;
	}


	if (pstPak->Body.CsCtrL.Cmd == ERR_RES)
	{
		pstErrRes = &pstPak->Body.CsCtrL.CsCmd.ErrRes;
		if (pstErrRes->ErrReason == ERR_SYS)
		{
			iRet =  CS_ERR_SYS;
			goto final;
		}
		else if (pstErrRes->ErrReason == ERR_ONLINE)
		{	
			iRet =  CS_ERR_ONLINE;
			goto final;
		}
		else if (pstErrRes->ErrReason == ERR_OUTLINE)
		{
			iRet =  CS_ERR_OUTLINE;
			goto final;
		}
		else if (pstErrRes->ErrReason == ERR_TDR)
		{
			iRet =  CS_ERR_TDR;
			goto final;
		}
		else if(pstErrRes->ErrReason == ERR_NO)
		{
			iRet =  CS_ERR_SUCCEEDED;
		}
		else if (pstErrRes->ErrReason == ERR_DUP)
		{
			iRet =  CS_ERR_DUP;
			goto final;
		}
		else if (pstErrRes->ErrReason == ERR_ARGS)
		{
			iRet =  CS_ERR_ARGS;
			goto final;
		}
		else
		{
			iRet =  CS_ERR_NOT_FOUND;
			goto final;
		}
		
	}
	

	memcpy(pOut, &pstPak->Body.CsCtrL, sizeof(SSCSCTRL));
	iRet = 0;
	
final:

	if(pstPak)
		free(pstPak);
	if(pszBuff)
		free(pszBuff);
	return iRet;
}

int cs_print_data(int *pHandle, const char *pszMetaName, const void *pvData,
	                int iDataSize)
{
	TDRDATA stHost;
	SSCONTEXT stSSHan;
	CGIENV *pstCgiEnv = (CGIENV*)(pHandle);

	assert(pHandle);

	stSSHan.pstSSProtoLib = ((LPTDRMETALIB)pstCgiEnv->pstSSProtoLib);
	stSSHan.pstSSProtoPkgMeta = ((LPTDRMETA)pstCgiEnv->pstSSProtoPkgMeta);

	stHost.pszBuff = (char *)pvData;
	stHost.iBuff = iDataSize;

	return tdr_fprintf(tdr_get_meta_by_name(stSSHan.pstSSProtoLib, pszMetaName),
		                 stdout, &stHost, 0);
}

int cs_sprintf(
	int *handle,
	const char *meta_name,
	const void *data,
	int data_size,
	char *buf,
	int buf_size)
{
	TDRDATA host_data, net_data;
	SSCONTEXT ss_handle;
	CGIENV *pstCgiEnv = (CGIENV*)(handle);

	assert(handle);

	ss_handle.pstSSProtoLib = ((LPTDRMETALIB)pstCgiEnv->pstSSProtoLib);
	ss_handle.pstSSProtoPkgMeta = ((LPTDRMETA)pstCgiEnv->pstSSProtoPkgMeta);

	host_data.pszBuff = (char *)data;
	host_data.iBuff = data_size;

	net_data.pszBuff = (char *)buf;
	net_data.iBuff = buf_size;

	return tdr_sprintf(tdr_get_meta_by_name(ss_handle.pstSSProtoLib, meta_name),
	                   &net_data, &host_data, 0);
}

int cs_sprint_xml(
	int *handle,
	const char *meta_name,
	const void *data,
	int data_size,
	char *buf,
	int buf_size)
{
	TDRDATA host_data, net_data;
	SSCONTEXT ss_handle;
	CGIENV *pstCgiEnv = (CGIENV*)(handle);

	assert(handle);

	ss_handle.pstSSProtoLib = ((LPTDRMETALIB)pstCgiEnv->pstSSProtoLib);
	ss_handle.pstSSProtoPkgMeta = ((LPTDRMETA)pstCgiEnv->pstSSProtoPkgMeta);

	host_data.pszBuff = (char *)data;
	host_data.iBuff = data_size;

	net_data.pszBuff = (char *)buf;
	net_data.iBuff = buf_size;

	return tdr_output(tdr_get_meta_by_name(ss_handle.pstSSProtoLib, meta_name),
	                   &net_data, &host_data,0, TDR_IO_NEW_XML_VERSION);

	//return tdr_sprintf(tdr_get_meta_by_name(ss_handle.pstSSProtoLib, meta_name),
	  //                 &net_data, &host_data, 0);
}


/*static int recv_svr_roleinfo(SSCONTEXT *pstSSHan, int iSock, ZONEGETROLEDETAILRES* pstOut, int iTimeOut)
{
	int iRecvLen;
	SSPKG stPak;
	char szBuff[sizeof(SSPKG)];
	
	if((iRecvLen = tnet_recvall(iSock, szBuff, sizeof(szBuff), iTimeOut)) < 0)
	{
		printf("tnet_recvall fail\n");
		return CS_ERR_NET;
	}

	if(cgi_ss_unpack(pstSSHan, szBuff, iRecvLen,&stPak, 0) < 0)
	{
		return CS_ERR_TDR;
	}

	memcpy(pstOut, &stPak.Body.CsCtrL.CsCmd.GetRoleDetailRes, sizeof(ZONEGETROLEDETAILRES));
	return 0;
}*/

static int acc_interface_fun(int *pHandle, int iRegionID, SSCSCTRL *pIn,
                             SSCSCTRL *pOut, int iTimeOut)
{
	int i = 0;
	char szUri[128];
	CGICONF *pstCgiConf = NULL;
	CGIENV *pstCgiEnv = (CGIENV*)(pHandle);

	assert(pHandle != NULL);
	pstCgiConf = (CGICONF*)pstCgiEnv->pstConf;
	if(pstCgiConf == NULL)
	{
		return CS_ERR_ARGS;
	}

	for(i = 0; i < pstCgiConf->AccountCount; i++)
	{
		if((int)pstCgiConf->AccountInfos[i].RegionID == iRegionID)
		{
			snprintf(szUri, sizeof(szUri), "tcp://%s:%d", 
				     pstCgiConf->AccountInfos[i].Ip, 
				     pstCgiConf->AccountInfos[i].Port);
			break;
		}
	}

	if (i >= pstCgiConf->AccountCount)
	{
		return CS_ERR_ARGS;
	}

	return interface_fun_in(pHandle, szUri, pIn, pOut, iTimeOut);
}

static int cs_interface_fun(int *pHandle, int iRegionID, unsigned int iWolrdID,
	                        SSCSCTRL *pIn, SSCSCTRL* pOut, int iTimeOut)
{
	int i;
	char szUri[128];
	CGICONF *pstCgiConf = NULL;
	CGIENV *pstCgiEnv = (CGIENV*)(pHandle);


	assert(pHandle != NULL);
	pstCgiConf = (CGICONF*)pstCgiEnv->pstConf;
	if(pstCgiConf == NULL)
	{
		return CS_ERR_ARGS;
	}

	for(i=0; i<pstCgiConf->WorldCount; i++)
	{
		if(pstCgiConf->WorldInfos[i].WorldID == iWolrdID &&
		   (int)pstCgiConf->WorldInfos[i].RegionID == iRegionID)
		{
			snprintf(szUri, sizeof(szUri), "tcp://%s:%d", 
				       pstCgiConf->WorldInfos[i].Ip,
				       pstCgiConf->WorldInfos[i].Port);
			break;
		}
	}

	if (i >= pstCgiConf->WorldCount)
	{
		return CS_ERR_ARGS;
	}

	return interface_fun_in(pHandle, szUri, pIn, pOut, iTimeOut);
}


int cs_get_role_info(int *pHandle, int iRegionID, int iWolrdID, char *pszRoleName,
						ZONEGETROLEDETAILRES *pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}
	
	stIn.Cmd = GET_ROLE_DETAIL_REQ;
	stIn.CsCmd.GetRoleDetailReq.RoleID = 0;
	stIn.CsCmd.GetRoleDetailReq.Uin = 0;
	STRNCPY(stIn.CsCmd.GetRoleDetailReq.RoleName, pszRoleName,
		      sizeof(stIn.CsCmd.GetRoleDetailReq.RoleName));

	iErr = cs_interface_fun(pHandle, iRegionID, iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}
	*pstOut = stOut.CsCmd.GetRoleDetailRes;

final:
	return iErr;
}


int cs_get_clan_info(int *pHandle, int iRegionID, int iWolrdID,
                     char *pszClanName, ZONEGETCLANINFORES *pstOut,
                     int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszClanName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}
	
	stIn.Cmd = GET_CLAN_INFO_REQ;
	stIn.CsCmd.GetClanInfoReq.ClanID = 0;
	STRNCPY(stIn.CsCmd.GetClanInfoReq.ClanName,
	        pszClanName,sizeof(stIn.CsCmd.GetClanInfoReq.ClanName));

	iErr = cs_interface_fun(pHandle, iRegionID, iWolrdID, &stIn,  &stOut,
	                        iTimeOut);
	if(iErr)
	{
		goto final;
	}
	*pstOut = stOut.CsCmd.GetClanInfoRes;

final:
	return iErr;
	
}

int cs_get_clan_all_info(int *pHandle, int iRegionID, int iWolrdID,
                         const char *pszClanName, ZONEGETCLANALLINFORES *pstOut,
                         int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pszClanName || !*pszClanName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}
	
	stIn.Cmd = GET_CLAN_ALL_INFO_REQ;
	STRNCPY(stIn.CsCmd.GetClanAllInfoReq.ClanName,
	        pszClanName,sizeof(stIn.CsCmd.GetClanAllInfoReq.ClanName));

	iErr = cs_interface_fun(pHandle, iRegionID, iWolrdID, &stIn, &stOut,
	                        iTimeOut);
	if(iErr)
	{
		goto final;
	}
	*pstOut = stOut.CsCmd.GetClanAllInfoRes;

final:
	return iErr;
}

int cs_del_clan_member(int *pHandle, int iRegionID, int iWolrdID,
                       const char *pszClanName, const char *pszRoleName,
                       int iTimeOut)
{
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pszClanName || !*pszClanName || !pszRoleName ||
	   !*pszRoleName)
	{
		return CS_ERR_ARGS;
	}
	
	stIn.Cmd = DEL_CLAN_MEMBER_REQ;
	STRNCPY(stIn.CsCmd.DelClanMemberReq.ClanName, pszClanName,
	        sizeof(stIn.CsCmd.DelClanMemberReq.ClanName));
	STRNCPY(stIn.CsCmd.DelClanMemberReq.RoleName, pszRoleName,
	        sizeof(stIn.CsCmd.DelClanMemberReq.RoleName));

	return cs_interface_fun(pHandle, iRegionID, iWolrdID, &stIn, &stOut,
	                        iTimeOut);
}

int cs_disband_clan(int *pHandle, int iRegionID, int iWolrdID,
                    const char *pszClanName, int iTimeOut)
{
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pszClanName || !*pszClanName)
	{
		return CS_ERR_ARGS;
	}
	
	stIn.Cmd = DISBAND_CLAN_REQ;
	STRNCPY(stIn.CsCmd.DisbandClanReq.ClanName, pszClanName,
	        sizeof(stIn.CsCmd.DisbandClanReq.ClanName));

	return cs_interface_fun(pHandle, iRegionID, iWolrdID, &stIn, &stOut,
	                        iTimeOut);
}

int cs_set_clan_base_attr(int *pHandle, int iRegionID, int iWorldID
                              , const char *pszClanName, int iMoney, int iStable
                              , int iActive, int iTech,int iTimeout)
{
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pszClanName || iActive < 0 || iTech < 0 || iStable < 0 || iMoney < 0 )
	{
		return CS_ERR_ARGS;
	}

	stIn.Cmd = SET_CLAN_BASE_ATTR_REQ;
	STRNCPY(stIn.CsCmd.SetClanBaseAttrReq.ClanName,pszClanName,sizeof(stIn.CsCmd.SetClanBaseAttrReq.ClanName));
	stIn.CsCmd.SetClanBaseAttrReq.Active = iActive;
	stIn.CsCmd.SetClanBaseAttrReq.Money = iMoney;
	stIn.CsCmd.SetClanBaseAttrReq.Stable = iStable;
	stIn.CsCmd.SetClanBaseAttrReq.Tech = iTech;

	return cs_interface_fun(pHandle, iRegionID, iWorldID, &stIn, &stOut,
	                        iTimeout);
}

int cs_clear_clan_diplomatism(int *pHandle, int iRegionID, int iWorldID,
                              const char *pszClanName, int iTimeOut)
{
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pszClanName || !*pszClanName)
	{
		return CS_ERR_ARGS;
	}

	stIn.Cmd = CLEAR_CLAN_DIPLOMATISM_REQ;
	STRNCPY(stIn.CsCmd.ClearClanDiplomatismReq.ClanName, pszClanName,
	        sizeof(stIn.CsCmd.ClearClanDiplomatismReq.ClanName));

	return cs_interface_fun(pHandle, iRegionID, iWorldID, &stIn, &stOut,
	                        iTimeOut);
}

int cs_get_friend_info(int *pHandle, int iRegionID, int iWolrdID,
                       char *pszRoleName, ZONEGETFIRENDRES *pstOut,
                       int iTimeOut)
{
	
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}
	
	stIn.Cmd = GET_ROLE_FRIEND_REQ;
	STRNCPY(stIn.CsCmd.FriendReq.RoleName, pszRoleName,sizeof(stIn.CsCmd.FriendReq.RoleName));

	iErr = cs_interface_fun(pHandle, iRegionID, iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}
	*pstOut = stOut.CsCmd.FirendRes;

final:
	return iErr;
	
}

int cs_get_safelock(int *pHandle, int iRegionid, int iWolrdID,
                   const char *pszRoleName, GETSAFELOCKRES *pstOut,int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = GET_SAFELOCK_REQ;
	STRNCPY(stIn.CsCmd.GetSafeLockReq.RoleName, pszRoleName,sizeof(stIn.CsCmd.GetSafeLockReq.RoleName));
	iErr =  cs_interface_fun(pHandle, iRegionid, iWolrdID, &stIn,  &stOut, iTimeOut);
	if (iErr)
	{
		goto final;
	}

	*pstOut = stOut.CsCmd.GetSafeLockRes;

final:
	return iErr;
}

int cs_get_acc_safelock(int *pHandle, int iRegionid,const char *pszLoginName, GETACCSAFELOCKRES *pstOut,int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pstOut || !pszLoginName)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = GET_ACC_SAFELOCK_REQ;
	STRNCPY(stIn.CsCmd.GetAccSafeLockReq.AccName, pszLoginName,
	        sizeof(stIn.CsCmd.GetAccSafeLockReq.AccName));

	iErr = acc_interface_fun(pHandle, iRegionid, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	*pstOut = stOut.CsCmd.GetAccSafeLockRes;

final:
	return iErr;
	
}

int cs_set_acc_safelock(int *pHandle, int iRegionid,const char *pszLoginName, const char* pszPassPwd,
							SETACCSAFELOCKRES *pstOut,int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle  || !pszLoginName ||!pszPassPwd)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	// 密码最少6位
	if (strlen(pszPassPwd) < 6)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = SET_ACC_SAFELOCK_REQ;
	STRNCPY(stIn.CsCmd.SetAccSafeLockReq.AccName, pszLoginName,
	        sizeof(stIn.CsCmd.SetAccSafeLockReq.AccName));
	STRNCPY(stIn.CsCmd.SetAccSafeLockReq.SafeLockPassWd, pszPassPwd,
	        sizeof(stIn.CsCmd.SetAccSafeLockReq.SafeLockPassWd));
	
	iErr = acc_interface_fun(pHandle, iRegionid, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if (pstOut)
		*pstOut = stOut.CsCmd.SetAccSafeLockRes;

final:
	return iErr;
	
}


int cs_set_safelock(int *pHandle, int iRegionid, int iWolrdID,
                   const char *pszRoleName, const char* pszPassWD, SETSAFELOCKRES *pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pszPassWD)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = SET_SAFELOCK_REQ;
	STRNCPY(stIn.CsCmd.SetSafeLockReq.RoleName, pszRoleName,sizeof(stIn.CsCmd.SetSafeLockReq.RoleName));
	STRNCPY(stIn.CsCmd.SetSafeLockReq.SafeLockPassWd, pszPassWD,sizeof(stIn.CsCmd.SetSafeLockReq.SafeLockPassWd));
	iErr =  cs_interface_fun(pHandle, iRegionid, iWolrdID, &stIn,  &stOut, iTimeOut);
	if (iErr)
	{
		goto final;
	}

	if (pstOut)
	{
		*pstOut = stOut.CsCmd.SetSafeLockRes;
	}

final:
	return iErr;
}


int cs_set_second_pwd(int *pHandle, int iRegionid, int iWolrdID,
                   const char *pszRoleName, const char* pszPassWD, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	//if (!pHandle || !pszRoleName || !pszPassWD)
	if (!pHandle || !pszRoleName)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = SET_SECOND_PWD_REQ;
	STRNCPY(stIn.CsCmd.SetSecondPwdReq.RoleName, pszRoleName,sizeof(stIn.CsCmd.SetSecondPwdReq.RoleName));
	STRNCPY(stIn.CsCmd.SetSecondPwdReq.SecondPassWd, pszPassWD,sizeof(stIn.CsCmd.SetSecondPwdReq.SecondPassWd));
	iErr =  cs_interface_fun(pHandle, iRegionid, iWolrdID, &stIn,  &stOut, iTimeOut);
	if (iErr)
	{
		goto final;
	}

final:
	return iErr;
}


int cs_get_black_info(int *pHandle, int iRegionID, int iWolrdID, char *pszRoleName,
						ZONEGETBLACKRES *pstOut, int iTimeOut)
{
	
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}
	
	stIn.Cmd = GET_ROLE_BLACK_REQ;
	STRNCPY(stIn.CsCmd.BlackReq.RoleName, pszRoleName,sizeof(stIn.CsCmd.BlackReq.RoleName));

	iErr = cs_interface_fun(pHandle, iRegionID, iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}
	*pstOut = stOut.CsCmd.BlackRes;

final:
	return iErr;
	
}

int cs_get_item_info(int *pHandle, int iRegionid, int iWolrdID,
	                   char *pszRoleName,ZONEGETROLEITEMLISTRES *pstOut,
	                   int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = GET_ROLE_ITEMLIST_REQ;
	stIn.CsCmd.GetRoleItemListReq.Uin = 0;
	stIn.CsCmd.GetRoleItemListReq.RoleID = 0;
	STRNCPY(stIn.CsCmd.GetRoleItemListReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.GetRoleItemListReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}
	*pstOut = stOut.CsCmd.GetRoleItemListRes;

final:
	return iErr;
}

int cs_get_skill_info(int *pHandle, int iRegionid, int iWolrdID, 
	                    const char *pszRoleName, ZONEGETROLESKILLINFORES *pstOut,
	                    int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = GET_ROLE_SKILLINFO_REQ;
	stIn.CsCmd.GetRoleSkillInfoReq.RoleID = 0;
	stIn.CsCmd.GetRoleSkillInfoReq.Uin = 0;
	STRNCPY(stIn.CsCmd.GetRoleSkillInfoReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.GetRoleSkillInfoReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}
	*pstOut = stOut.CsCmd.GetRoleSkillInfoRes;

final:
	return iErr;
}

/*int cs_get_fittings_info(int *pHandle, int iRegionid, int iWolrdID, 
	                       long long llWid, const char *pszRoleName,int iUin, 
	                       ZONEGETROLEFITTINGSRES*pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = GET_ROLE_FITTING_REQ;
	stIn.CsCmd.GetRoleFittingsReq.RoleID = llWid;
	stIn.CsCmd.GetRoleFittingsReq.Uin = iUin;
	STRNCPY(stIn.CsCmd.GetRoleFittingsReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.GetRoleFittingsReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}
	*pstOut = stOut.CsCmd.GetRoleFittingsRes;

final:
	return iErr;
}
*/
int cs_block_role(int *pHandle, int iRegionid, int iWolrdID, 
	                const char *pszRoleName, int iBlockTime, 
	                ZONEBLOCKPLAYERRES *pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pszRoleName || iBlockTime == 0)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = BLOCKPLAYER_REQ;
	stIn.CsCmd.BlockPlayerReq.RoleID = 0;
	stIn.CsCmd.BlockPlayerReq.Uin = 0;
	stIn.CsCmd.BlockPlayerReq.Time = iBlockTime;
	stIn.CsCmd.BlockPlayerReq.Type = 0;
	stIn.CsCmd.BlockPlayerReq.IsTemp = 0;
	STRNCPY(stIn.CsCmd.BlockPlayerReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.BlockPlayerReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(pstOut)
	{
		memcpy(pstOut, &stOut.CsCmd.BlockPlayerRes, sizeof(*pstOut));
	}

final:
	return iErr;
}

int cs_set_role_info(int *pHandle, int iRegionid,  int iWolrdID,
	                   ZONEGETROLEDETAILRES *pstSSRoleDetail,
	                   ZONESETROLEDETAILRES *pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pstSSRoleDetail)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.CsCmd.SetRoleDetailReq.Uin 			= 	0;
	stIn.CsCmd.SetRoleDetailReq.MapID 			= 	pstSSRoleDetail->MapID;
	stIn.CsCmd.SetRoleDetailReq.RoleID 			= 	0;
	stIn.CsCmd.SetRoleDetailReq.X 				= 	pstSSRoleDetail->X;
	stIn.CsCmd.SetRoleDetailReq.Y 				= 	pstSSRoleDetail->Y;
	stIn.CsCmd.SetRoleDetailReq.Perm 			= 	pstSSRoleDetail->Perm;
	stIn.CsCmd.SetRoleDetailReq.BolckTime 		= 	pstSSRoleDetail->Block;
	stIn.CsCmd.SetRoleDetailReq.SilenceTime 		= 	pstSSRoleDetail->Silence;
	stIn.CsCmd.SetRoleDetailReq.RoleDetailInfo 	= 	pstSSRoleDetail->RoleDetailInfo;
	STRNCPY(stIn.CsCmd.SetRoleDetailReq.RoleName, pstSSRoleDetail->RoleName, 
		      sizeof(stIn.CsCmd.SetRoleDetailReq.RoleName));
	stIn.CsCmd.SetRoleDetailReq.Level			=     pstSSRoleDetail->Level;
	stIn.Cmd = SET_ROLE_DETAIL_REQ;

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(strcmp(pstSSRoleDetail->RoleName, stOut.CsCmd.SetRoleDetailRes.RoleName) != 0)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}
	
	if(pstOut)
	{
		STRNCPY(pstOut->RoleName, stOut.CsCmd.SetRoleDetailRes.RoleName,
			      sizeof(pstOut->RoleName));
	}

final:
	return iErr;
}

int cs_set_skill_info(int *pHandle, int iRegionid,  int iWolrdID,
                      const char *pszRoleName, unsigned int iSkillID,
                      unsigned int iSkillLevel, int iMode,
                      ZONESETROLESKILLINFORES *pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pszRoleName || !pHandle )
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	if(iMode < ROLE_SKILL_DEL || iMode > ROLE_SKILL_ADD)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	if(iSkillLevel > 12)
	{
		//iErr = CS_ERR_ARGS;
		//goto final;
	}

	memset (&stIn, 0, sizeof(stIn));
	stIn.CsCmd.SetRoleSkillInfoReq.RoleID = 0;
	stIn.CsCmd.SetRoleSkillInfoReq.Type = iMode;
	stIn.CsCmd.SetRoleSkillInfoReq.Uin = 0;
	stIn.CsCmd.SetRoleSkillInfoReq.ZoneSkillData.SkillLevel = iSkillLevel;
	stIn.CsCmd.SetRoleSkillInfoReq.ZoneSkillData.SkillID = iSkillID;
	STRNCPY(stIn.CsCmd.SetRoleSkillInfoReq.RoleName, 
		      pszRoleName, sizeof(stIn.CsCmd.SetRoleSkillInfoReq.RoleName));
	stIn.Cmd = SET_ROLE_SKILLINFO_REQ;

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(strcmp(pszRoleName, stOut.CsCmd.SetRoleDetailRes.RoleName) != 0)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

	if(pstOut)
	{
		STRNCPY(pstOut->RoleName, pszRoleName, sizeof(pstOut->RoleName));	
	}

final:
	return iErr;
}

/*int cs_set_fitting_info(int *pHandle, int iRegionid, int iWolrdID, 
	                      long long llWid, const char *pszRoleName, int iUin,
	                      unsigned int iFittingID, unsigned int iFittingLevel,
	                      int iMode, ZONESETROLEFITTINGSRES *pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pszRoleName || !pHandle )
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	if(iMode < ROLE_FITTING_DEL || iMode > ROLE_FITTING_ADD)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	memset (&stIn, 0, sizeof(stIn));
	stIn.CsCmd.SetRoleFittingsReq.RoleID = llWid;
	stIn.CsCmd.SetRoleFittingsReq.Uin = iUin;
	stIn.CsCmd.SetRoleFittingsReq.Type = iMode;
	stIn.CsCmd.SetRoleFittingsReq.ZoneFittingData.FittingsID = iFittingID;
	stIn.CsCmd.SetRoleFittingsReq.ZoneFittingData.FittingsLevel = iFittingLevel;
	STRNCPY(stIn.CsCmd.SetRoleFittingsReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.SetRoleFittingsReq.RoleName));
	stIn.Cmd = SET_ROLE_FITTING_REQ;

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(strcmp(pszRoleName, stOut.CsCmd.SetRoleFittingsRes.RoleName) != 0)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

	if(pstOut)
	{
		STRNCPY(pstOut->RoleName, pszRoleName, sizeof(pstOut->RoleName));
	}

final:
	return iErr;
}*/

int cs_del_task_by_id(int *pHandle, int iRegionid, int iWolrdID,
					            const char *pszRoleName, unsigned int uiTaskID,
					            ZONEDELROLETASKRES *pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pszRoleName || !pHandle ||uiTaskID <= 0)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	memset(&stIn, 0, sizeof(stIn));

	stIn.Cmd = DEL_ROLE_TASK_REQ;
	stIn.CsCmd.DelRoleTaskReq.RoleID = 0;
	stIn.CsCmd.DelRoleTaskReq.Uin = 0;
	stIn.CsCmd.DelRoleTaskReq.TaskID = uiTaskID;
	STRNCPY(stIn.CsCmd.DelRoleTaskReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.DelRoleTaskReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(strcmp(pszRoleName, stOut.CsCmd.DelRoleTaskRes.RoleName) != 0)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

	if(pstOut)
	{
		STRNCPY(pstOut->RoleName, pszRoleName, sizeof(pstOut->RoleName));
	}

final:
	return iErr;
}

int cs_add_fairy_by_id(int *pHandle, int iRegionid, int iWolrdID,
					            const char *pszRoleName, unsigned int FairyID,
					            ZONESETROLEFAIRYRES*pstOut, int iTimeOut)
{
	SSCSCTRL stIn;
	SSCSCTRL stOut;
	int iErr;

	if (!pHandle || !pszRoleName )
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	memset(&stIn, 0, sizeof(stIn));

	stIn.Cmd = MODIFY_ROLE_FAIRY_REQ;
	stIn.CsCmd.SetRoleFairyReq.FairyID = FairyID;
	stIn.CsCmd.SetRoleFairyReq.FairyLV = 1;
	stIn.CsCmd.SetRoleFairyReq.OpType = ROLE_FAIRY_ADD;
	stIn.CsCmd.SetRoleFairyReq.RoleID = 0;
	stIn.CsCmd.SetRoleFairyReq.Uin = 0;
	STRNCPY(stIn.CsCmd.SetRoleFairyReq.RoleName, pszRoleName, sizeof(stIn.CsCmd.SetRoleFairyReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if (pstOut)
	{
		pstOut->OpType = ROLE_FAIRY_ADD;
		STRNCPY(pstOut->RoleName, stOut.CsCmd.SetRoleFairyRes.RoleName, sizeof(pstOut->RoleName));
	}
	
final:
	return iErr;	
}

int cs_del_fairy_by_id(int *pHandle, int iRegionid, int iWolrdID,
					            const char *pszRoleName, unsigned int FairyID,
					            ZONESETROLEFAIRYRES*pstOut, int iTimeOut)
{
	SSCSCTRL stIn;
	SSCSCTRL stOut;
	int iErr;

	if (!pHandle || !pszRoleName )
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	memset(&stIn, 0, sizeof(stIn));

	stIn.Cmd = MODIFY_ROLE_FAIRY_REQ;
	stIn.CsCmd.SetRoleFairyReq.FairyID = FairyID;
	stIn.CsCmd.SetRoleFairyReq.FairyLV = 1;
	stIn.CsCmd.SetRoleFairyReq.OpType = ROLE_FAIRY_DEL;
	stIn.CsCmd.SetRoleFairyReq.RoleID = 0;
	stIn.CsCmd.SetRoleFairyReq.Uin = 0;
	STRNCPY(stIn.CsCmd.SetRoleFairyReq.RoleName, pszRoleName, sizeof(stIn.CsCmd.SetRoleFairyReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if (pstOut)
	{
		pstOut->OpType = ROLE_FAIRY_DEL;
		STRNCPY(pstOut->RoleName, stOut.CsCmd.SetRoleFairyRes.RoleName, sizeof(pstOut->RoleName));
	}
	
final:
	return iErr;	
}

int cs_modify_fairy_by_id(int *pHandle, int iRegionid, int iWolrdID,
					            const char *pszRoleName,unsigned int FairyID,unsigned int FairyLv,
					            ZONESETROLEFAIRYRES*pstOut, int iTimeOut)
{
	SSCSCTRL stIn;
	SSCSCTRL stOut;
	int iErr;

	if (!pHandle || !pszRoleName )
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	memset(&stIn, 0, sizeof(stIn));

	stIn.Cmd = MODIFY_ROLE_FAIRY_REQ;
	stIn.CsCmd.SetRoleFairyReq.FairyID = FairyID;
	stIn.CsCmd.SetRoleFairyReq.FairyLV = FairyLv;
	stIn.CsCmd.SetRoleFairyReq.OpType = ROLE_FAIRY_MODIFY;
	stIn.CsCmd.SetRoleFairyReq.RoleID = 0;
	stIn.CsCmd.SetRoleFairyReq.Uin = 0;
	STRNCPY(stIn.CsCmd.SetRoleFairyReq.RoleName, pszRoleName, sizeof(stIn.CsCmd.SetRoleFairyReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if (pstOut)
	{
		pstOut->OpType = ROLE_FAIRY_MODIFY;
		STRNCPY(pstOut->RoleName, stOut.CsCmd.SetRoleFairyRes.RoleName, sizeof(pstOut->RoleName));
	}
	
final:
	return iErr;	
}

int cs_set_item_info(int *pHandle, int iRegionid,  int iWolrdID,
					           const char *pszRoleName, int iListType,
					           const ROLEGRID *pstGrid, int iMode,
					           ZONESETROLEITEMLISTRES *pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pszRoleName || !pHandle  || !pstGrid)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	if (iMode < ROLE_ITEM_DEL || iMode > ROLE_ITEM_ADD)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	memset (&stIn, 0, sizeof(stIn));
	stIn.CsCmd.SetRoleItemListReq.Uin = 0;
	stIn.CsCmd.SetRoleItemListReq.RoleID = 0;
	stIn.CsCmd.SetRoleItemListReq.OPType = iMode;
	stIn.CsCmd.SetRoleItemListReq.PackType = iListType;
	memcpy(&stIn.CsCmd.SetRoleItemListReq.ZoneRoleGrid,pstGrid, sizeof(ROLEGRID));
	STRNCPY(stIn.CsCmd.SetRoleItemListReq.RoleName, pszRoleName,
		      sizeof(stIn.CsCmd.SetRoleItemListReq.RoleName));
	stIn.Cmd = SET_ROLE_ITEMLIST_REQ;

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(strcmp(pszRoleName, stOut.CsCmd.SetRoleItemListRes.RoleName) != 0)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

	if (pstOut)
	{
		STRNCPY(pstOut->RoleName, pszRoleName, sizeof(pstOut->RoleName));
	}
	
final:
	return iErr;
}

int cs_get_role_list(int *pHandle, int iRegionid, int iWolrdID, int iUin,
	                   ZONEROLELISTRES *pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;
	
	if(!pHandle || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}
	
	memset (&stIn, 0, sizeof(stIn));
	stIn.CsCmd.RoleListReq.RegionID = iRegionid;
	stIn.CsCmd.RoleListReq.Uin = iUin;
	stIn.CsCmd.RoleListReq.WorldID = iWolrdID;
	stIn.Cmd = ROLE_LIST_REQ;

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(stOut.CsCmd.RoleListRes.Uin != iUin)
	{
		iErr = CS_ERR_NOT_FOUND;
		goto final;
	}

	
	memcpy(pstOut, &stOut.CsCmd.RoleListRes, sizeof(*pstOut));
	

	return 0;
	

final:
	return iErr;
}

int cs_set_diathsis_info(int *pHandle, int iRegionid,  int iWolrdID,
	                       const char *pszRoleName,
	                       unsigned int iDiathsisID, unsigned int iDiathsisLevel,
	                       int iMode, ZONESETROLEDIATHESISRES *pstOut, 
	                       int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pszRoleName || !pHandle)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	if(iMode < ROLE_DIATHESIS_DEL || iMode > ROLE_DIATHESIS_ADD)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	memset (&stIn, 0, sizeof(stIn));
	stIn.CsCmd.SetRoleDiathesisReq.RoleID = 0;
	stIn.CsCmd.SetRoleDiathesisReq.Uin = 0;
	stIn.CsCmd.SetRoleDiathesisReq.OpType = iMode;
	stIn.CsCmd.SetRoleDiathesisReq.ZoneDiathesisData.DiathesisID = iDiathsisID;
	stIn.CsCmd.SetRoleDiathesisReq.ZoneDiathesisData.DiathesisLevel = iDiathsisLevel;
	STRNCPY(stIn.CsCmd.SetRoleDiathesisReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.SetRoleDiathesisReq.RoleName));
	stIn.Cmd = SET_ROLE_DIATHESIS_REQ;

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(strcmp(pszRoleName, stOut.CsCmd.SetRoleDiathesisRes.RoleName) != 0)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

	if(pstOut)
	{
		STRNCPY(pstOut->RoleName, pszRoleName, sizeof(pstOut->RoleName));
	}

	return 0;

final:
	return iErr;
}

int cs_silence_role(int *pHandle, int iRegionid, int iWolrdID, 
	                  const char *pszRoleName,int iSilenceTime, 
	                  ZONESILENCEROLERES *pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pszRoleName )
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = SILENCE_ROLE_REQ;
	stIn.CsCmd.SilenceRoleReq.RoleID = 0;
	stIn.CsCmd.SilenceRoleReq.Time = iSilenceTime;
	stIn.CsCmd.SilenceRoleReq.Uin = 0;
	stIn.CsCmd.SilenceRoleReq.IsTemp = 0;
	STRNCPY(stIn.CsCmd.SilenceRoleReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.SilenceRoleReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(pstOut)
	{
		memcpy(pstOut, &stOut.CsCmd.SilenceRoleRes, sizeof(*pstOut));
	}

	return 0;

final:
	return iErr;
}

int cs_unsilence_role(int *pHandle, int iRegionid, int iWolrdID, 
                    const char *pszRoleName,ZONESILENCEROLERES *pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pszRoleName)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = SILENCE_ROLE_REQ;
	stIn.CsCmd.SilenceRoleReq.RoleID = 0;
	stIn.CsCmd.SilenceRoleReq.Time = 0;
	stIn.CsCmd.SilenceRoleReq.Uin = 0;
	stIn.CsCmd.SilenceRoleReq.IsTemp = 0;
	STRNCPY(stIn.CsCmd.SilenceRoleReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.SilenceRoleReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(pstOut)
	{
		memcpy(pstOut, &stOut.CsCmd.SilenceRoleRes, sizeof(*pstOut));
	}

	return 0;

final:
	return iErr;
}

int cs_unblock_role(int *pHandle, int iRegionid, int iWolrdID, 
	                  const char *pszRoleName, ZONEBLOCKPLAYERRES *pstOut,
	                  int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pszRoleName)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = BLOCKPLAYER_REQ;
	stIn.CsCmd.BlockPlayerReq.RoleID = 0;
	stIn.CsCmd.BlockPlayerReq.Uin = 0;
	stIn.CsCmd.BlockPlayerReq.Type = 1;
	stIn.CsCmd.BlockPlayerReq.IsTemp = 0;
	STRNCPY(stIn.CsCmd.BlockPlayerReq.RoleName, pszRoleName,
		      sizeof(stIn.CsCmd.BlockPlayerReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if (pstOut)
	{
		memcpy(pstOut, &stOut.CsCmd.BlockPlayerRes, sizeof(*pstOut));
	}

	return 0;
	
final:
	return iErr;
}


int cs_role_kick_off(int *pHandle, int iRegionid, int iWolrdID, 
	                  const char *pszRoleName, ZONEROLELOGOUTRES *pstOut,
	                  int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pszRoleName)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = SET_ROLE_LOGOUT_REQ;
	
	STRNCPY(stIn.CsCmd.KickOffRoleReq.RoleName, pszRoleName,
		      sizeof(stIn.CsCmd.KickOffRoleReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if (pstOut)
	{
		memcpy(pstOut, &stOut.CsCmd.KickOffRoleRes, sizeof(*pstOut));
	}

	return 0;
	
final:
	return iErr;
}


int cs_del_role(int *pHandle, int iRegionid, int iWolrdID, 
	                  const char *pszRoleName, ZONEDELROLERES *pstOut,
	                  int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pszRoleName)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = DEL_ROLE_REQ;
	stIn.CsCmd.DelRoleReq.Uin = 0;
	stIn.CsCmd.DelRoleReq.RoleID = 0;
	STRNCPY(stIn.CsCmd.DelRoleReq.RoleName, pszRoleName,
		      sizeof(stIn.CsCmd.DelRoleReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if (pstOut)
	{
		memcpy(pstOut, &stOut.CsCmd.DelRoleRes, sizeof(*pstOut));
	}

	return 0;
	
final:
	return iErr;
}

int cs_get_life_skill(int *pHandle, int iRegionid, int iWolrdID, 
	                  const char *pszRoleName, GETLIFTSKILLRES*pstOut,
	                  int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pszRoleName)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = GET_LIFT_SKILL_REQ;
	STRNCPY(stIn.CsCmd.GetLiftSkillReq.RoleName, pszRoleName,
		      			sizeof(stIn.CsCmd.GetLiftSkillReq.RoleName));
	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if (iErr)
		goto final;

	if (pstOut)
		memcpy(pstOut, &stOut.CsCmd.GetLiftSkillRes, sizeof(*pstOut));
	return 0;

final:
	return iErr;
}

int cs_set_life_skill(int *pHandle, int iRegionid, int iWolrdID, SETLIFTSKILLREQ *pstReq,SETLIFTSKILLRES *pstOut,int iTimeOut)
{
	
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pstReq)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	if (pstReq->RoleName[0] == 0)
	{
		iErr = CS_ERR_ARGS;
		goto final;	
	}

	if (pstReq->SkillOpType != SETLIFTSKILL_OP_DEL &&
		pstReq->SkillOpType != SETLIFTSKILL_OP_MOD)
	{
		iErr = CS_ERR_ARGS;
		goto final;	
	}

	stIn.Cmd = SET_LIFT_SKILL_REQ;
	stIn.CsCmd.SetLiftSkillReq = *pstReq;

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if (iErr)
		goto final;

	if (pstOut)
		memcpy(pstOut, &stOut.CsCmd.SetLiftSkillRes, sizeof(*pstOut));
	return 0;

final:
	return iErr;
}

int cs_force_msg_req(int *pHandle, int iRegionid, int iWolrdID, 
						FORCEMSGREQ *pstIn, FORCEMSGRES*pstOut,int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle ||!pstIn)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = FORCE_MSG_REQ;
	stIn.CsCmd.ForceMsgReq = *pstIn;
	
	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if (iErr)
		goto final;

	if (pstOut)
		pstOut->Succ = stOut.CsCmd.ForceMsgRes.Succ;
	iErr = 0;
final:
	return iErr;
}

int cs_set_role_perm(int *pHandle, int iRegionid, int iWolrdID, 
	                  const char *pszRoleName, int iPerm,ZONEDELROLERES *pstOut,
	                  int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pszRoleName ||iPerm < 0)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = SET_ROLE_PERM_REQ;
	stIn.CsCmd.SetPermReq.Perm = iPerm;
	STRNCPY(stIn.CsCmd.SetPermReq.RoleName, pszRoleName,
		      sizeof(stIn.CsCmd.SetPermReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if (pstOut)
	{
		memcpy(pstOut, &stOut.CsCmd.DelRoleRes, sizeof(*pstOut));
	}

	return 0;
	
final:
	return iErr;
}

int cs_get_diathesis_info(int *pHandle, int iRegionid, int iWolrdID, 
	                        const char *pszRoleName, 
	                        ZONEGETROLEDIATHESISRES *pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}
	
	stIn.Cmd = GET_ROLE_DIATHESIS_REQ;
	stIn.CsCmd.GetRoleDiathesisReq.RoleID = 0;
	stIn.CsCmd.GetRoleDiathesisReq.Uin = 0;
	STRNCPY(stIn.CsCmd.GetRoleDiathesisReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.GetRoleDiathesisReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}
	*pstOut = stOut.CsCmd.GetRoleDiathesisRes;

	return 0;

final:
	return iErr;
}

int cs_get_task_info(int *pHandle, int iRegionid, int iWolrdID, 
	                   const char *pszRoleName,ZONEGETROLETASKRES *pstOut,
	                   int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = GET_ROLE_TASK_REQ;
	stIn.CsCmd.GetRoleTaskReq.RoleID = 0;
	stIn.CsCmd.GetRoleTaskReq.Uin = 0;
	STRNCPY(stIn.CsCmd.GetRoleTaskReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.GetRoleTaskReq.RoleName));
	
	iErr = cs_interface_fun(pHandle, iRegionid, iWolrdID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	*pstOut = stOut.CsCmd.GetRoleTaskRes;

	return 0;

final:
	return iErr;
}

int cs_chg_role_name(int *pHandle, int iRegionid, int iWolrdID,const char *pszRoleName, 
	                   const char *pszChgRoleName, ZONECHGROLENAMERES*pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;
	ZONEGETROLEDETAILRES stRoleInfoOut;

	if(!pHandle || !pszRoleName || !pszChgRoleName)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}
	
	if (strcmp(pszRoleName, pszChgRoleName) == 0)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}


	if (cs_get_role_info(pHandle, iRegionid, iWolrdID,
	                     (char *)pszRoleName, &stRoleInfoOut, 1500) < 0)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

	stIn.Cmd = CHG_ROLE_NAME_REQ;
	stIn.CsCmd.ChgRoleNameReq.RoleID = stRoleInfoOut.RoleID;
	stIn.CsCmd.ChgRoleNameReq.Uin = stRoleInfoOut.Uin;
	STRNCPY(stIn.CsCmd.ChgRoleNameReq.OldRoleName, pszRoleName, 
				sizeof(stIn.CsCmd.ChgRoleNameReq.OldRoleName));

	STRNCPY(stIn.CsCmd.ChgRoleNameReq.ChgRoleName, pszChgRoleName, 
				sizeof(stIn.CsCmd.ChgRoleNameReq.ChgRoleName));

	iErr = cs_interface_fun(pHandle, iRegionid, iWolrdID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(strcmp(stOut.CsCmd.ChgRoleNameRes.ChgRoleName, pszChgRoleName) != 0)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

	if(pstOut)
	{
		STRNCPY(pstOut->ChgRoleName, pszChgRoleName, sizeof(pstOut->ChgRoleName));
	}

	return 0;

final:
	return iErr;
}

int cs_chg_role_pos(int *pHandle, int iRegionid, int iWolrdID, 
	                   const char *pszRoleName,  int iMap,int x,int y,
	                   ZONECHGROLEPOSRES *pstOut,int iTimeOut)
{
	SSCSCTRL stIn;
	SSCSCTRL stOut;
	int iErr = 0;

	if(!pHandle || !pszRoleName)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}
	
	stIn.Cmd = CHG_ROLE_POS_REQ;
	stIn.CsCmd.ChgRolePosReq.RoleID = 0;
	stIn.CsCmd.ChgRolePosReq.Map = iMap;
	stIn.CsCmd.ChgRolePosReq.X = x;
	stIn.CsCmd.ChgRolePosReq.Y= y;
	stIn.CsCmd.ChgRolePosReq.Uin = 0;
	STRNCPY(stIn.CsCmd.ChgRolePosReq.RoleName, pszRoleName, 
				sizeof(stIn.CsCmd.ChgRolePosReq.RoleName));

	iErr = cs_interface_fun(pHandle, iRegionid, iWolrdID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if (strcmp(stOut.CsCmd.ChgRolePosRes.RoleName, pszRoleName) != 0)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}
	
	if (pstOut)
	{
		strncpy(pstOut->RoleName, pszRoleName, sizeof(pstOut->RoleName));
	}
	return 0;
	
final:
	return iErr;
}


int cs_get_account_info(int *pHandle, int iRegionID, const char *pszLoginName,
                        SSACCINFO *pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = GET_ACCOUNT_INFO_REQ;
	STRNCPY(stIn.CsCmd.GetAccInfoReq.AccIDInfo.LoginName, pszLoginName,
	        sizeof(stIn.CsCmd.GetAccInfoReq.AccIDInfo.LoginName));

	iErr = acc_interface_fun(pHandle, iRegionID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(!stOut.CsCmd.GetAccInfoRes.Found)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}
	*pstOut = stOut.CsCmd.GetAccInfoRes.AccountInfo;

final:
	return iErr;
}

static  int cs_ip_block_by_world(int *pHandle,  int iRegionID,SSCSCTRL *pstIn, int iTimeOut)
{
	CGICONF *pstCgiConf = NULL;
	CGIENV *pstCgiEnv = (CGIENV*)(pHandle);
	int i;

	pstCgiConf = (CGICONF*)pstCgiEnv->pstConf;
	if(pstCgiConf == NULL)
	{
		return -1;
	}

	for(i=0; i<pstCgiConf->WorldCount; i++)
	{
		if((int)pstCgiConf->WorldInfos[i].RegionID == iRegionID)
		{
			cs_interface_fun(pHandle,iRegionID,pstCgiConf->WorldInfos[i].WorldID, pstIn,  NULL, iTimeOut);
		}
	}
	
	return 0;
}
int cs_ip_block_by_region(int *pHandle, int iRegionID,const char * pszIP, const char* pszMask, int iSec,int iOpType,int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;
	int iIP;
	int iMask;

	if (!pHandle || !pszIP )
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	iIP = (int)inet_addr(pszIP);
	if (iIP <= 0)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}
	
	if (pszMask)
	{
		iMask = (int)inet_addr(pszMask);
	}
	else
	{
		iMask = (int)inet_addr("255.255.255.255");
	}

	stIn.Cmd = IP_LIMIT_REQ;
	stIn.CsCmd.IPLimitReq.Op = iOpType;
	stIn.CsCmd.IPLimitReq.IP = iIP;
	stIn.CsCmd.IPLimitReq.Mask = iMask;
	stIn.CsCmd.IPLimitReq.Sec = iSec;

	iErr = acc_interface_fun(pHandle, iRegionID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	// 若是添加ip限制  等authsvr成功返回    去zone_svr找人踢
	if  ((iOpType == OP_ADD || iOpType == OP_MODIFY) && iSec !=0 &&!iErr)
	{
		cs_ip_block_by_world(pHandle,  iRegionID,&stIn, 1000);
	}
	
final:
	return iErr;
}

int cs_get_ip_block_list_by_region(int *pHandle, int iRegionID, const char * pszIP, ZONEGETIPLIMITRES *pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle  || !pstOut ||iRegionID <= 0)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = GET_IP_LIMIT_REQ;

	if (pszIP)
	{
		stIn.CsCmd.GetIPLimitReq.Res = (int)inet_addr(pszIP);
	}
	else
	{
		stIn.CsCmd.GetIPLimitReq.Res = 0;
	}
	

	iErr = acc_interface_fun(pHandle, iRegionID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	memcpy (pstOut, &stOut.CsCmd.GetIPLimitRes, sizeof(*pstOut));

final:
	return iErr;
	
}


static int cs_set_account_uin_to_world(int *pHandle,  int iRegionID,SSCSCTRL *pstIn, int iTimeOut)
{
	CGICONF *pstCgiConf = NULL;
	CGIENV *pstCgiEnv = (CGIENV*)(pHandle);
	int i;

	pstCgiConf = (CGICONF*)pstCgiEnv->pstConf;
	if(pstCgiConf == NULL)
	{
		return -1;
	}

	for(i=0; i<pstCgiConf->WorldCount; i++)
	{
		if((int)pstCgiConf->WorldInfos[i].RegionID == iRegionID)
		{
			cs_interface_fun(pHandle,iRegionID,pstCgiConf->WorldInfos[i].WorldID, pstIn,  NULL, iTimeOut);
		}
	}
	return 0;
}

int cs_set_account_uin_flag(int *pHandle, int iRegionID,
                            const char *pszLoginName, unsigned int nUinFlag,
                            int nType,
                            int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = SET_ACCOUNT_UIN_FLAG_REQ;
	STRNCPY(stIn.CsCmd.SetAccountUinFlagReq.AccIDInfo.LoginName, pszLoginName,
	        sizeof(stIn.CsCmd.SetAccountUinFlagReq.AccIDInfo.LoginName));
	stIn.CsCmd.SetAccountUinFlagReq.UinFlag = nUinFlag;
	stIn.CsCmd.SetAccountUinFlagReq.Type = nType;

	iErr = acc_interface_fun(pHandle, iRegionID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(!stOut.CsCmd.SetAccountUinFlagRes.Succeeded)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

	// 更新zone_svr
	cs_set_account_uin_to_world(pHandle, iRegionID,&stIn, 1000);

final:
	return iErr;
}

static FIND_ACCOUNT_HANDLE create_find_account_handle(int *pHandle,
                                                      const char *pszPattern,
                                                      int iTimeOut,
                                                      int iByWhat)
{
	int i = 0;
	FIND_ACCOUNT_HANDLE pstFind = NULL;
	CGIENV *pstCgiEnv = (CGIENV*)(pHandle);
	const CGICONF *pstCgiConf = !pstCgiEnv
	                          ? NULL : (const CGICONF*)pstCgiEnv->pstConf;

	if(pstCgiConf == NULL)
	{
		return NULL;
	}

	// 创建句柄
	pstFind = calloc(1, sizeof(struct _FindAccountContext));
	if(!pstFind)
	{
		return NULL;
	}

	for(i = 0; i < pstCgiConf->AccountCount; i++)
	{
		pstFind->aiReginID[pstFind->iMax++] =
			(int)pstCgiConf->AccountInfos[i].RegionID;
	}
	STRNCPY(pstFind->szPattern, pszPattern, sizeof(pstFind->szPattern));
	pstFind->iTimeOut = iTimeOut;
	pstFind->pHandle = pHandle;
	pstFind->iByWhat = iByWhat;

	return pstFind;
}
	

int cs_find_first_account_by_loginname(int *pHandle, const char *pszLoginName,
                                       FIND_ACCOUNT_HANDLE *pFind,
                                       ACCFINDRES *pstOut, int iTimeOut)
{
	int iErr = 0, iSavedRegionID = 0;
	FIND_ACCOUNT_HANDLE pstFind = NULL;

	pstFind = create_find_account_handle(pHandle, pszLoginName, iTimeOut,
	                                     BY_LOGIN_NAME);
	if(!pstFind)
	{
		iErr = CS_ERR_SYS;
		goto error;
	}

	// 获取第一次
	for(;;)
	{
		if(pstFind->iIndex >= pstFind->iMax)
		{
			iErr = CS_ERR_NOT_FOUND;
			goto error;
		}

		iSavedRegionID = pstFind->aiReginID[pstFind->iIndex];
		pstFind->iQueryIndex = 0;
		iErr = get_account_by(
			pstFind->pHandle,
			iSavedRegionID,
			FIND_ACCOUNT_BY_LOGINNAME,
			pstFind->szPattern,
			pstFind->iQueryIndex++,
			pstOut,
			pstFind->iTimeOut);
		if(iErr)
		{
			pstFind->iPrevCount = 0;
			pstFind->iIndex++;
			continue;
		}
	
		pstFind->iPrevCount = pstOut->ResNum;
		if(pstOut->ResNum > 0)
		{
			break; // get it!
		}
		else
		{
			//没错误也没找到
			pstFind->iIndex++;
		}
	}

	pstOut->ReginID = iSavedRegionID;

	*pFind = pstFind; // 保存句柄
	return 0;

error:
	cs_close_handle(pstFind);
	bzero(pstOut, sizeof(*pstOut));
	return iErr;
}

int cs_find_first_account_by_rolename(int *pHandle, const char *pszRoleName,
                                      FIND_ACCOUNT_HANDLE *pFind,
                                      ACCFINDRES *pstOut, int iTimeOut)
{
	int iErr = 0, iSavedRegionID = 0;
	FIND_ACCOUNT_HANDLE pstFind = NULL;

	pstFind = create_find_account_handle(pHandle, pszRoleName, iTimeOut,
	                                     BY_ROLE_NAME);
	if(!pstFind)
	{
		iErr = CS_ERR_SYS;
		goto error;
	}

	// 获取第一次
	for(;;)
	{
		if(pstFind->iIndex >= pstFind->iMax)
		{
			iErr = CS_ERR_NOT_FOUND;
			goto error;
		}

		iSavedRegionID = pstFind->aiReginID[pstFind->iIndex];
		pstFind->iQueryIndex = 0;
		iErr = get_account_by(
			pstFind->pHandle,
			iSavedRegionID,
			FIND_ACCOUNT_BY_ROLENAME,
			pstFind->szPattern,
			pstFind->iQueryIndex++,
			pstOut,
			pstFind->iTimeOut);
		if(iErr)
		{
			pstFind->iIndex++;
			pstFind->iPrevCount = 0;
			continue;
		}
	
		pstFind->iPrevCount = pstOut->ResNum;
		if(pstOut->ResNum > 0)
		{
			
			break; // get it!
		}
		else
		{
			pstFind->iIndex++;
		}
	}

	pstOut->ReginID = iSavedRegionID;

	*pFind = pstFind; // 保存句柄
	return 0;

error:
	cs_close_handle(pstFind);
	bzero(pstOut, sizeof(*pstOut));
	return iErr;
}

int cs_find_next_account(FIND_ACCOUNT_HANDLE pstFind, ACCFINDRES *pstOut)
{
	int iErr = 0, iSavedRegionID = pstFind->aiReginID[pstFind->iIndex];

	for(;;)
	{
		if(pstFind->iIndex >= pstFind->iMax &&
			pstFind->iPrevCount < ACC_MAX_RES)
		{
			iErr = CS_ERR_NOT_FOUND;
			goto error;
		}

		if(pstFind->iPrevCount < ACC_MAX_RES)
		{
			pstFind->iIndex++;
			iSavedRegionID = pstFind->aiReginID[pstFind->iIndex];
			pstFind->iQueryIndex = 0;
		}
		else
		{
			iSavedRegionID = pstFind->aiReginID[pstFind->iIndex];
		}

		switch(pstFind->iByWhat)
		{
		case BY_LOGIN_NAME:
			iErr = get_account_by(
				pstFind->pHandle,
				iSavedRegionID,
				FIND_ACCOUNT_BY_LOGINNAME,
				pstFind->szPattern,
				pstFind->iQueryIndex++,
				pstOut,
				pstFind->iTimeOut);
			break;
		case BY_ROLE_NAME:
			iErr = get_account_by(
				pstFind->pHandle,
				iSavedRegionID,
				FIND_ACCOUNT_BY_ROLENAME,
				pstFind->szPattern,
				pstFind->iQueryIndex++,
				pstOut,
				pstFind->iTimeOut);
			break;
		default:
			iErr = CS_ERR_ARGS;
			goto error;
			break;
		}
		if(iErr)
		{
			pstFind->iPrevCount = 0;
			continue;
		}

		pstFind->iPrevCount = pstOut->ResNum;
		if(pstOut->ResNum > 0)
		{
			break; // get it!
		}
	}

	pstOut->ReginID = iSavedRegionID;
	return 0;

error:
	bzero(pstOut, sizeof(*pstOut));
	return iErr;
}

static int get_account_by(
	int *pHandle,
	int iRegionID,
	int iByWhat,
	const char *pszPattern,
	int iIndex,
	ACCFINDRES *pstOut,
	int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pszPattern || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = FIND_ACCOUNT_REQ;

	stIn.CsCmd.FindAccountReq.ByWhat = iByWhat;
	switch(stIn.CsCmd.FindAccountReq.ByWhat)
	{
	case FIND_ACCOUNT_BY_LOGINNAME:
		STRNCPY(stIn.CsCmd.FindAccountReq.Key.LoginNamePattern,
		        pszPattern,
		        sizeof(stIn.CsCmd.FindAccountReq.Key.LoginNamePattern));
		break;
	case FIND_ACCOUNT_BY_ROLENAME:
		STRNCPY(stIn.CsCmd.FindAccountReq.Key.RoleNamePattern,
		        pszPattern,
		        sizeof(stIn.CsCmd.FindAccountReq.Key.RoleNamePattern));
		break;
	default:
		iErr = CS_ERR_ARGS;
		goto final;
		break;
	}
	stIn.CsCmd.FindAccountReq.Index = iIndex;

	iErr = acc_interface_fun(pHandle, iRegionID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}
	{
		int i = stOut.CsCmd.FindAccountRes.ResNum;
		pstOut->ResNum = stOut.CsCmd.FindAccountRes.ResNum;
		while(i--)
		{
			pstOut->AccountLiteArray[i] = 
				stOut.CsCmd.FindAccountRes.AccountLiteArray[i];
		}
	}

final:
	return iErr;
}

static int shut_acc_mouth(int *pHandle, int iRegionid, int iWolrdID, 
                          const char *pszLoginName, int fEnable,
                          int iSilenceTime, int iTimeOut)
{
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle)
	{
		return CS_ERR_ARGS;
	}	

	stIn.Cmd = SILENCE_ACCOUNT_REQ;
	stIn.CsCmd.SilenceAccountReq.Enable = fEnable;
	stIn.CsCmd.SilenceAccountReq.SilenceTime = iSilenceTime;
	STRNCPY(stIn.CsCmd.SilenceAccountReq.LoginName, pszLoginName,
	        sizeof(stIn.CsCmd.SilenceAccountReq.LoginName));

	return cs_interface_fun(pHandle, iRegionid, iWolrdID, &stIn, &stOut,
	                        iTimeOut);
}

static int shut_all_mouth_by_loginname(int *pHandle, int iRegionID,
                                       const char *pszLoginName, int fEnable,
                                       int iSilenceTime, int iTimeOut)
{
	int i = 0;
	CGIENV *pstCgiEnv = (CGIENV*)(pHandle);
	const CGICONF *pstCgiConf = !pstCgiEnv
	                          ? NULL : (const CGICONF*)pstCgiEnv->pstConf;

	if(pstCgiConf == NULL)
	{
		return CS_ERR_ARGS;
	}

	for(i = 0; i < pstCgiConf->WorldCount; i++)
	{
		int iErr = 0;
		const WORLDINFO *pstDst = pstCgiConf->WorldInfos + i;

		if((int)pstDst->RegionID != iRegionID)
		{
			continue;
		}
		iErr = shut_acc_mouth(pHandle, iRegionID, pstDst->WorldID, pszLoginName,
		                      fEnable, iSilenceTime, iTimeOut);
		if(iErr)
		{
			//return iErr;
			continue;
		}
	}

	return 0;
}

int cs_set_account_silence(int *pHandle, int iRegionID, const char *pszLoginName, 
	                       int iEnable, int iSilenceTime, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pszLoginName || !pszLoginName[0])
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	// 把在线的玩家禁言了先
	iErr = shut_all_mouth_by_loginname(pHandle, iRegionID, pszLoginName,
	                                   iEnable, iSilenceTime, iTimeOut);
	if(iErr)
	{
		goto final;
	}


	stIn.Cmd = SET_ACCOUNT_SILENCE_REQ;
	stIn.CsCmd.SetAccSilenceReq.Enable = iEnable;
	stIn.CsCmd.SetAccSilenceReq.SilenceTime = iSilenceTime;
	STRNCPY(stIn.CsCmd.SetAccSilenceReq.AccIDInfo.LoginName, pszLoginName,
	        sizeof(stIn.CsCmd.SetAccSilenceReq.AccIDInfo.LoginName));

	iErr = acc_interface_fun(pHandle, iRegionID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(!stOut.CsCmd.SetAccSilenceRes.Succeeded)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

final:
	return iErr;
}


static int kick_acc_off(int *pHandle, int iRegionid, int iWolrdID,
                        const char *pszLoginName, int iBlockTime, int iTimeOut)
{
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle)
	{
		return CS_ERR_ARGS;
	}

	stIn.Cmd = BLOCK_ACCOUNT_REQ;
	STRNCPY(stIn.CsCmd.BlockAccountReq.LoginName, pszLoginName,
	        sizeof(stIn.CsCmd.BlockAccountReq.LoginName));
	stIn.CsCmd.BlockAccountReq.BlockTime = iBlockTime;
	
	return cs_interface_fun(pHandle, iRegionid, iWolrdID, &stIn, &stOut,
	                        iTimeOut);
}

static int kick_off_all_by_loginname(int *pHandle, int iRegionID,
                                     const char *pszLoginName,
                                     int iFreezeTime, int iTimeOut)
{
	int i = 0;
	CGIENV *pstCgiEnv = (CGIENV*)(pHandle);
	const CGICONF *pstCgiConf = !pstCgiEnv
	                          ? NULL : (const CGICONF*)pstCgiEnv->pstConf;

	if(pstCgiConf == NULL)
	{
		return CS_ERR_ARGS;
	}

	for(i = 0; i < pstCgiConf->WorldCount; i++)
	{
		int iErr = 0;
		const WORLDINFO *pstDst = pstCgiConf->WorldInfos + i;

		if((int)pstDst->RegionID != iRegionID)
		{
			continue;
		}
		iErr = kick_acc_off(pHandle, pstDst->RegionID, pstDst->WorldID,
		                    pszLoginName, iFreezeTime, iTimeOut);
		if(iErr == CS_ERR_ARGS || iErr == CS_ERR_TIME_OUT)
		{
			printf("the cs_svr of World_%d may not be started\n", pstDst->WorldID);
			continue;
		}
		else if(iErr)
		{
			return iErr;
		}
	}

	return 0;
}

int cs_kick_off_by_loginname(int *pHandle, int iRegionID,
                             const char *pszLoginName, int iTimeOut)
{
	return kick_off_all_by_loginname(pHandle, iRegionID, pszLoginName, 1,
	                                 iTimeOut);
}

int cs_set_account_freeze(int *pHandle, int iRegionID, const char *pszLoginName,
                          int iEnable, int iFreezeTime,char* pszReason, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pszLoginName || !pszLoginName[0])
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	// 把在线的玩家踢下线先
	iErr = kick_off_all_by_loginname(pHandle, iRegionID, pszLoginName,
	                                 iFreezeTime, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	stIn.Cmd = SET_ACCOUNT_FREEZE_REQ;
	stIn.CsCmd.SetAccFreezeReq.Enable = iEnable;
	stIn.CsCmd.SetAccFreezeReq.FreezeTime = iFreezeTime;
	STRNCPY(stIn.CsCmd.SetAccFreezeReq.AccIDInfo.LoginName,
	        pszLoginName, sizeof(stIn.CsCmd.SetAccFreezeReq.AccIDInfo.LoginName));
	STRNCPY(stIn.CsCmd.SetAccFreezeReq.FreezeReason,pszReason,
			sizeof(stIn.CsCmd.SetAccFreezeReq.FreezeReason));

	iErr = acc_interface_fun(pHandle, iRegionID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(!stOut.CsCmd.SetAccFreezeRes.Succeeded)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

	if(!stOut.CsCmd.SetAccFreezeRes.Succeeded ||
	   !iEnable )
	{
		goto final;
	}

final:
	return iErr;
}


int cs_restore_deleted_role(int *pHandle, int iRegionID, int iWorldID, 
	                        long long llWID, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = RESTORE_ROLE_REQ;
	stIn.CsCmd.RestoreRoleReq.RoleID = llWID;

	iErr = cs_interface_fun(pHandle, iRegionID, iWorldID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(!stOut.CsCmd.RestoreRoleRes.Succeeded)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

	acc_interface_fun(pHandle, iRegionID, &stIn, &stOut, iTimeOut);

final:
	return iErr;
}

/*
int cs_get_role_maillist(int *pHandle, int iRegionID, int iWorldID, 
	                       const char *pszRoleName, ZONEGETMAILLISTRES *pstOut, 
	                       int iTimeOut)
{
	int i, iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;
	
	if(!pHandle || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = GET_ROLE_MAIL_LIST_REQ;
	STRNCPY(stIn.CsCmd.GetRoleMailListReq.RoleName, 
		      pszRoleName,
		      sizeof(stIn.CsCmd.GetRoleMailListReq.RoleName));

	iErr = cs_interface_fun(pHandle, iRegionID, iWorldID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	pstOut->ResNum = stOut.CsCmd.GetRoleMailListRes.ResNum;
	i = stOut.CsCmd.GetRoleMailListRes.ResNum;
	while(i--)
	{
		pstOut->MailHeads[i] = stOut.CsCmd.GetRoleMailListRes.MailHeads[i];
	}

final:
	return iErr;
}
*/

static FIND_MAILLIST_HANDLE create_find_maillist_handle(
	int *pHandle,
	int iRegionID,
	int iWorldID,
	const char *pszRoleName,
	int iTimeOut)
{
	int i = 0;
	FIND_MAILLIST_HANDLE pstContext = NULL;
	CGIENV *pstCgiEnv = (CGIENV*)(pHandle);
	const CGICONF *pstCgiConf = !pstCgiEnv ?
	                            NULL : (const CGICONF*)pstCgiEnv->pstConf;

	if(pstCgiConf == NULL)
	{
		goto fatal;
	}

	for(i = 0; i < (int)pstCgiConf->WorldCount; i++)
	{
		if((int)pstCgiConf->WorldInfos[i].RegionID == iRegionID &&
		   (int)pstCgiConf->WorldInfos[i].WorldID == iWorldID)
		{
			break;
		}
	}
	if(i >= pstCgiConf->WorldCount)
	{
		goto fatal;
	}
	
	pstContext = calloc(1, sizeof(struct _FindMaillistContext));
	if(!pstContext)
	{
		goto fatal;
	}

	strncpy(pstContext->szRoleName, pszRoleName, sizeof(pstContext->szRoleName));
	pstContext->iTimeOut = iTimeOut;
	pstContext->iRegionID = iRegionID;
	pstContext->iWorldID = iWorldID;
	pstContext->pHandle = pHandle;
	return pstContext;
	
fatal:
	if(pstContext)
	{
		free(pstContext);
	}
	return NULL;
}

static int get_role_maillist(int *pHandle, int iRegionID, int iWorldID,
                             const char *pszRoleName, int iIndex,
                             ZONEGETMAILLISTRES *pstOut, int iTimeOut)
{
	int i, iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;
	
	if(!pHandle || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = GET_ROLE_MAIL_LIST_REQ;
	stIn.CsCmd.GetRoleMailListReq.Index = iIndex;
	STRNCPY(stIn.CsCmd.GetRoleMailListReq.RoleName, pszRoleName,
		    sizeof(stIn.CsCmd.GetRoleMailListReq.RoleName));

	iErr = cs_interface_fun(pHandle, iRegionID, iWorldID, &stIn, &stOut,
	                        iTimeOut);
	if(iErr)
	{
		goto final;
	}

	pstOut->ResNum = stOut.CsCmd.GetRoleMailListRes.ResNum;
	i = stOut.CsCmd.GetRoleMailListRes.ResNum;
	while(i--)
	{
		pstOut->MailHeads[i] = stOut.CsCmd.GetRoleMailListRes.MailHeads[i];
	}

final:
	return iErr;
}

int cs_find_first_maillist(int *pHandle, int iRegionID, int iWorldID,
                           const char *pszRoleName, FIND_MAILLIST_HANDLE *pFind,
                           ZONEGETMAILLISTRES *pstOut, int iTimeOut)
{
	int iErr = 0;
	FIND_MAILLIST_HANDLE pstFind = NULL;

	pstFind = create_find_maillist_handle(pHandle, iRegionID, iWorldID,
	                                      pszRoleName, iTimeOut);
	if(!pstFind)
	{
		iErr = CS_ERR_SYS;
		goto error;
	}

	// 获取第一次
	pstFind->iQueryIndex = 0;
	iErr = get_role_maillist(
		pstFind->pHandle,
		pstFind->iRegionID,
		pstFind->iWorldID,
		pstFind->szRoleName,
		pstFind->iQueryIndex++,
		pstOut,
		pstFind->iTimeOut);
	if(iErr)
	{
		goto error;
	}
	
	pstFind->iPrevCount = pstOut->ResNum;
	if(!pstOut->ResNum)
	{
		iErr = CS_ERR_NOT_FOUND;
		goto error;
	}

	*pFind = pstFind; // 保存句柄
	return 0;

error:
	cs_close_handle(pstFind);
	bzero(pstOut, sizeof(*pstOut));
	return iErr;
}


int cs_find_next_maillist(FIND_MAILLIST_HANDLE pstFind,
                          ZONEGETMAILLISTRES *pstOut)
{
	int iErr = 0;

	if(pstFind->iPrevCount < MAIL_MAX_NUM)
	{
		iErr = CS_ERR_NOT_FOUND;
		goto error;
	}

	iErr = get_role_maillist(
		pstFind->pHandle,
		pstFind->iRegionID,
		pstFind->iWorldID,
		pstFind->szRoleName,
		pstFind->iQueryIndex++,
		pstOut,
		pstFind->iTimeOut);
	if(iErr)
	{
		goto error;
	}

	pstFind->iPrevCount = pstOut->ResNum;
	if(!pstOut->ResNum)
	{
		iErr = CS_ERR_NOT_FOUND;
		goto error;
	}
	return 0;

error:
	bzero(pstOut, sizeof(*pstOut));
	return iErr;
}

int cs_get_role_maildata(int *pHandle, int iRegionID, int iWorldID, 
	                       const char *pszRoleName, long long llMailID,
	                       MAIL *pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = GET_ROLE_MAIL_REQ;
	STRNCPY(stIn.CsCmd.GetRoleMailReq.RoleName, 
		      pszRoleName,
		      sizeof(stIn.CsCmd.GetRoleMailReq.RoleName));
	stIn.CsCmd.GetRoleMailReq.MailID = llMailID;

	iErr = cs_interface_fun(pHandle, iRegionID, iWorldID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(!stOut.CsCmd.GetRoleMailRes.Found)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}
	*pstOut = stOut.CsCmd.GetRoleMailRes.MailData;

final:
	return iErr;
}

int cs_del_role_mail(int *pHandle, int iRegionID, int iWorldID, 
                     const char *pszRoleName, long long llMailID, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pszRoleName)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = DEL_ROLE_MAIL_REQ;
	STRNCPY(stIn.CsCmd.DelMailReq.RoleName,
		      pszRoleName, 
		      sizeof(stIn.CsCmd.DelMailReq.RoleName));
	stIn.CsCmd.DelMailReq.MailID = llMailID;

	iErr = cs_interface_fun(pHandle, iRegionID, iWorldID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(!stIn.CsCmd.DelMailRes.Succeeded)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

final:
	return iErr;
}


int cs_send_sys_mail(int *pHandle, int iRegionID, int iWorldID, 
	                   const char *pszTo, const char *pszSend, 
	                   const char *pszTitle, const char *pszText, 
	                   const ROLEGRID *astItems, int iItemNum,
	                   unsigned int nMoney, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;
	ZONESENDMAILREQ *pstReq = &stIn.CsCmd.SendMailReq;

	if(!pHandle || !pszTo || !pszTo[0] || !pszSend || !pszSend[0] ||
		 iItemNum > MAIL_MAX_ITEM)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}
	
	stIn.Cmd = SEND_MAIL_REQ;
	pstReq->Money = nMoney;
	STRNCPY(pstReq->To, pszTo, sizeof(pstReq->To));
	STRNCPY(pstReq->Send, pszSend, sizeof(pstReq->Send));
	STRNCPY(pstReq->Title, (!pszTitle ? "无标题" : pszTitle), sizeof(pstReq->Title));
	STRNCPY(pstReq->Text, (!pszText ? "" : pszText), sizeof(pstReq->Text));
	if(!astItems || !iItemNum)
	{
		pstReq->ItemNum = 0;
		bzero(pstReq->Items, sizeof(pstReq->Items));
	}
	else
	{
		pstReq->ItemNum = iItemNum;
		memcpy(pstReq->Items, astItems, sizeof(pstReq->Items[0]) * iItemNum);
	}

	iErr = cs_interface_fun(pHandle, iRegionID, iWorldID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(!stOut.CsCmd.SendMailRes.Succeeded)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

final:
	return iErr;
}

/*
int cs_get_account_rolelist(int *pHandle, int iRegionID,
                            const char *pszLoginName, ACCGETROLELISTRES *pstOut,
                            int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = GET_ACCOUNT_ROLELIST_REQ;
	stIn.CsCmd.GetAccRoleListReq.Index = 0;
	STRNCPY(stIn.CsCmd.GetAccRoleListReq.AccIDInfo.LoginName, pszLoginName,
	        sizeof(stIn.CsCmd.GetAccRoleListReq.AccIDInfo.LoginName));

	iErr = acc_interface_fun(pHandle, iRegionID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	pstOut->ResNum = stOut.CsCmd.GetAccRoleListRes.ResNum;
	if(pstOut->ResNum > 0)
	{
		memcpy(pstOut->RoleArray, stOut.CsCmd.GetAccRoleListRes.RoleArray,
		       sizeof(pstOut->RoleArray[0]) * pstOut->ResNum);
	}

final:
	return iErr;
}
*/

int cs_mov_role_item(int *pHandle, const SSROLESTORAGEPATH *pstSrc,
                     const SSROLESTORAGEPATH *pstDst, int iListType,
                     const ROLEGRID *pstGrid, int iTimeOut)
{
	int iErr = 0;
	ZONESETROLEITEMLISTRES res;

	iErr = cs_set_item_info(pHandle, pstSrc->RegionID, pstSrc->WorldID, 
		                       pstSrc->RoleName, iListType, pstGrid,
		                      ROLE_ITEM_DEL, &res, iTimeOut);
	if(iErr)
	{
		return iErr;
	}

	return cs_set_item_info(pHandle, pstDst->RegionID, pstDst->WorldID, 
		                       pstDst->RoleName,iListType, pstGrid,
		                      ROLE_ITEM_ADD, &res, iTimeOut);
}

int cs_copy_role(int *pHandle, int iRegionID, int iWorldID, long long llDstID,
                 const char *pszDstName, long long llSrcID,
                 const char *pszSrcName, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pszDstName || !pszDstName[0] ||
	   !pszSrcName || !pszSrcName[0])
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = COPY_ROLE_REQ;
	stIn.CsCmd.CopyRoleReq.SrcRoleID = llSrcID;
	STRNCPY(stIn.CsCmd.CopyRoleReq.SrcRoleName, pszSrcName, 
		    sizeof(stIn.CsCmd.CopyRoleReq.SrcRoleName));
	stIn.CsCmd.CopyRoleReq.DstRoleID = llDstID;
	STRNCPY(stIn.CsCmd.CopyRoleReq.DstRoleName, pszDstName, 
		    sizeof(stIn.CsCmd.CopyRoleReq.DstRoleName));

	iErr = cs_interface_fun(pHandle, iRegionID, iWorldID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(stOut.CsCmd.CopyRoleRes.Succeeded < 0)
	{
		iErr = stOut.CsCmd.CopyRoleRes.Succeeded;
		goto final;
	}

final:
	return iErr;
}

int cs_get_bulletin_list(int *pHandle, int iRegionID, int iWorldID,
	                       ZONEGETBULLETINLISTRES *pstOut, int iTimeOut)
{
	int iErr = 0, i = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = GET_BULLETIN_LIST_REQ;

	iErr = cs_interface_fun(pHandle, iRegionID, iWorldID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	pstOut->Num = stOut.CsCmd.GetBulletinListRes.Num;
	if(stOut.CsCmd.GetBulletinListRes.Num > 0)
	{
		i = stOut.CsCmd.GetBulletinListRes.Num;
		while(i--)
		{
			pstOut->BulletinArray[i] = stOut.CsCmd.GetBulletinListRes.BulletinArray[i];
		}
	}

final:
	return iErr;
}

int cs_add_bulletin(int *pHandle, int iRegionID, int iWorldID, 
                    const char *pszText, int iRollInterval, int iActiveTime, int tActiveTimeStart,
                    int *piID, int iTimeOut)
{
	int iErr = 0, iID = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;
	int tTime;
	int tNow ;

	if(!pHandle || !pszText || !pszText[0])
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	if(!iActiveTime || iActiveTime < iRollInterval)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	tTime = tActiveTimeStart;

	tNow = time(NULL);
	
	if (tTime < tNow)
	{
		tTime = tNow;
	}



	iID = cs_gen_bulletin_id();
	
	memset(&stIn.CsCmd.AddBulletinReq,0,
		sizeof(stIn.CsCmd.AddBulletinReq.Bulletin));
	
	stIn.Cmd = ADD_BULLETIN_REQ;
	
	stIn.CsCmd.AddBulletinReq.Bulletin.ActiveTime = iActiveTime;
	stIn.CsCmd.AddBulletinReq.Bulletin.ActiveTimeStart = tTime;
	stIn.CsCmd.AddBulletinReq.Bulletin.Core.RollInterval = iRollInterval;
	stIn.CsCmd.AddBulletinReq.Bulletin.Core.ID = iID;
	STRNCPY(stIn.CsCmd.AddBulletinReq.Bulletin.Core.Text, pszText,
		      sizeof(stIn.CsCmd.AddBulletinReq.Bulletin.Core.Text));

	iErr = cs_interface_fun(pHandle, iRegionID, iWorldID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(!stOut.CsCmd.AddBulletinRes.Succeeded)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

	if(piID)
	{
		*piID = iID;
	}

final:
	return iErr;
}


int cs_add_bulletin_to_all(int *pHandle, const char *pszText, int iRollInterval,
                           int iActiveTime, int tActiveTimeStart, int *piID, int iTimeOut)
{
	int iErr = 0, i = 0;
	CGICONF *pstCgiConf = NULL;
	CGIENV *pstCgiEnv = (CGIENV*)(pHandle);

	pstCgiConf = (CGICONF*)pstCgiEnv->pstConf;
	if(!pHandle || !pszText || !pszText[0])
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	if(!iActiveTime || iActiveTime < iRollInterval)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	i = pstCgiConf->WorldCount;
	while(i--)
	{
		cs_add_bulletin(pHandle, pstCgiConf->WorldInfos[i].RegionID,
		                pstCgiConf->WorldInfos[i].WorldID, 
			            pszText, iRollInterval, iActiveTime, tActiveTimeStart,piID, iTimeOut);
	}

final:
	return iErr;
}


int cs_del_bulletin(int *pHandle, int iRegionID, int iWorldID, int iID, 
	                  int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = DEL_BULLETIN_REQ;
	stIn.CsCmd.DelBulletinReq.ID = iID;

	iErr = cs_interface_fun(pHandle, iRegionID, iWorldID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(!stOut.CsCmd.DelBulletinRes.Succeeded)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

final:
	return iErr;
}

int cs_del_bulletin_from_all(int *pHandle, int iID, int iTimeOut)
{
	int i = 0;
	CGICONF *pstCgiConf = NULL;
	CGIENV *pstCgiEnv = (CGIENV*)(pHandle);

	if(!pHandle)
	{
		return CS_ERR_ARGS;
	}
	
	pstCgiConf = (CGICONF*)pstCgiEnv->pstConf;
	i = pstCgiConf->WorldCount;
	while(i--)
	{
		cs_del_bulletin(pHandle, pstCgiConf->WorldInfos[i].RegionID,
		                pstCgiConf->WorldInfos[i].WorldID, iID, iTimeOut);
	}
	return 0;
}


#if 0
int cs_create_role_item(int *pHandle, int iRegionID, int iWorldID, 
                        const ZONECREATEITEMREQ *pstReq,
                        ZONECREATEITEMRES *pstOut, int iTimeOut)
{
	int i = 0, iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pstOut || !pHandle || pstReq->Num > ROLE_GRID_MAX_RES)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = CREATE_ITEM_REQ;
	stIn.CsCmd.CreateRoleItemReq.Num = pstReq->Num;
	i = pstReq->Num;
	while(i--)
	{
		stIn.CsCmd.CreateRoleItemReq.ItemIdents[i] = pstReq->ItemIdents[i];
	}

	iErr = cs_interface_fun(pHandle, iRegionID, iWorldID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(!stOut.CsCmd.CreateRoleItemRes.Num)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

	pstOut->Num = stOut.CsCmd.CreateRoleItemRes.Num;
	i = stOut.CsCmd.CreateRoleItemRes.Num;
	while(i--)
	{
		pstOut->RoleGrids[i] = stOut.CsCmd.CreateRoleItemRes.RoleGrids[i];
	}

final:
	return iErr;
}
#endif

int cs_create_role_item(int *pHandle,  const ZONECREATEITEMREQ *pstReq,
                        				ZONECREATEITEMRES *pstOut)
{
	int i = 0;
	const SSITEMIDENT *pstCurr = NULL;
	ROLEGRID *pstDst = NULL;
	
	((void)pHandle);

	if(!pstOut)
	{
		return CS_ERR_ARGS;
	}
	
	i = pstReq->Num;
	pstOut->Num = pstReq->Num;
	pstDst = pstOut->RoleGrids;
	pstCurr = pstReq->ItemIdents;
	while(i--)
	{
		if(!pstCurr->Num)
		{
			continue;
		}
		memset(pstDst, 0, sizeof(*pstDst));
		
		if(pstCurr->DefID >= 1000000 && pstCurr->DefID <= 4999999)
		{
			// arm
			pstDst->Type = GRID_ITEM_ARM;
			pstDst->GridData.RoleArm.DefID = pstCurr->DefID;
			pstDst->GridData.RoleArm.DefPos = 0;
			pstDst->GridData.RoleArm.Dur = 2000 * 100;
			pstDst->GridData.RoleArm.FightLv = 0;
			pstDst->GridData.RoleArm.GridIdx = 0;
			pstDst->GridData.RoleArm.HoleNum = 0;
			pstDst->GridData.RoleArm.InstFlag = 0;
			pstDst->GridData.RoleArm.Num = 1;
			pstDst->GridData.RoleArm.StarLv = 0;
			pstDst->GridData.RoleArm.ValidEnd = 0;
			pstDst->GridData.RoleArm.WID = 0ULL;
		}
		else
		{
			// item
			pstDst->Type = GRID_ITEM_ITEM;
			pstDst->GridData.RoleItem.DefID = pstCurr->DefID;
			pstDst->GridData.RoleItem.DefPos = 0;
			pstDst->GridData.RoleItem.Flag = 0;
			pstDst->GridData.RoleItem.GridIdx = 0;
			pstDst->GridData.RoleItem.InstFlag = 0;
			pstDst->GridData.RoleItem.InstVal1 = 0;
			pstDst->GridData.RoleItem.InstVal2 = 0;
			pstDst->GridData.RoleItem.Num = pstCurr->Num;
			pstDst->GridData.RoleItem.ValidEnd = 0;
			pstDst->GridData.RoleItem.WID = 0;
		}
		
		pstCurr++;
		pstDst++;
	}

	return 0;
}

int cs_get_pet_info(int *pHandle, int iRegionid, int iWolrdID,
	                    const char *pszRoleName,ZONEGETROLEPETINFORES *pstOut,
	                    int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = GET_ROLE_PET_REQ;
	stIn.CsCmd.GetRolePetInfoReq.RoleID = 0;
	stIn.CsCmd.GetRolePetInfoReq.Uin = 0;
	STRNCPY(stIn.CsCmd.GetRolePetInfoReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.GetRolePetInfoReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}
	*pstOut = stOut.CsCmd.GetRolePetInfoRes;

final:
	return iErr;
}

int cs_shop_cgi_req(int *pHandle, int iRegionid, int iWolrdID,SHOPCGIREQ *pstIn, SHOPCGIRES *pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pstIn || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = CTL_SHOP_CGI_REQ;
	stIn.CsCmd.ShopCgiReq = *pstIn;
	
	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}
	*pstOut = stOut.CsCmd.ShopCgiRes;
	
final:
	return iErr;
}

int cs_set_pet_info(int *pHandle, int iRegionid,  int iWolrdID,
                      const char *pszRoleName, ROLEPET *pstRolePet, int iMode,
                      ZONESETROLEPETINFORES *pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pszRoleName || !pHandle )
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	if(iMode < ROLE_PET_DEL || iMode > ROLE_PET_ADD)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	memset (&stIn, 0, sizeof(stIn));
	stIn.CsCmd.SetRolePetInfoReq.RoleID = 0;
	stIn.CsCmd.SetRolePetInfoReq.Type = iMode;
	stIn.CsCmd.SetRolePetInfoReq.Uin = 0;
	STRNCPY(stIn.CsCmd.SetRolePetInfoReq.RoleName, 
		      pszRoleName, sizeof(stIn.CsCmd.SetRolePetInfoReq.RoleName));
	
	memcpy(&stIn.CsCmd.SetRolePetInfoReq.ZoneRolePetData,pstRolePet, sizeof(ROLEPET));
	
	stIn.Cmd = SET_ROLE_PET_REQ;

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(strcmp(pszRoleName, stOut.CsCmd.SetRolePetInfoRes.RoleName) != 0)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

	if(pstOut)
	{
		STRNCPY(pstOut->RoleName, pszRoleName, sizeof(pstOut->RoleName));	
	}

final:
	return iErr;
}

int cs_role_hatch_pet(int *pHandle, int iRegionid, int iWolrdID,
			     	const char *pszRoleName, int iPetID, int iPetLevel, int iPetGenius, 
			     	int iStrengthenLevel, ZONEROLEHATCHPETRES *pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = ROLE_HATCH_PET_REQ;
	stIn.CsCmd.RoleHatchPetReq.PetInfo.PetID = iPetID;
	stIn.CsCmd.RoleHatchPetReq.PetInfo.PetLevel = iPetLevel;
	stIn.CsCmd.RoleHatchPetReq.PetInfo.PetGenius = iPetGenius;
	stIn.CsCmd.RoleHatchPetReq.PetInfo.StrengthenLevel = iStrengthenLevel;
	
	STRNCPY(stIn.CsCmd.RoleHatchPetReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.RoleHatchPetReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(pstOut)
	{
		*pstOut = stOut.CsCmd.RoleHatchPetRes;
	}

final:
	return iErr;
}


int cs_role_drop_restore(int *pHandle, int iRegionid, int iWolrdID,
			     	int iDropType, const ROLEDROPRESTOREINFO *pstRestoreInfo, const char *pszRoleName, 
			     	ROLEDROPRESTORERES *pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = ROLE_DROP_RESTORE_REQ;
	stIn.CsCmd.RoleDropRestoreReq.Type = iDropType;
	stIn.CsCmd.RoleDropRestoreReq.RestoreInfo= *pstRestoreInfo;
	STRNCPY(stIn.CsCmd.RoleDropRestoreReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.RoleDropRestoreReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}
	
	if(pstOut)
	{
		*pstOut = stOut.CsCmd.RoleDropRestoreRes;
	}
final:
	return iErr;
}


int cs_get_role_drop_info(int *pHandle, int iRegionid, int iWolrdID,
	                    const char *pszRoleName, ZONEGETROLEDROPRES *pstOut,
	                    int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = GET_ROLE_DROP_REQ;
	STRNCPY(stIn.CsCmd.GetRoleDropReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.GetRoleDropReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}
	*pstOut = stOut.CsCmd.GetRoleDropRes;

final:
	return iErr;
}

int cs_set_package_info(int *pHandle, int iRegionid, int iWolrdID,
	                    const char *pszRoleName, int iListType, int iNum, ZONESETROLEPACKAGERES *pstOut,
	                    int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = SET_ROLE_PACKAGE_REQ;
	stIn.CsCmd.SetRolePackageReq.ListType = iListType;
	stIn.CsCmd.SetRolePackageReq.Num = iNum;
	STRNCPY(stIn.CsCmd.SetRolePackageReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.SetRolePackageReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(pstOut)
	{
		*pstOut = stOut.CsCmd.SetRolePackageRes;
	}
	
final:
	return iErr;
}

int cs_get_package_info(int *pHandle, int iRegionid, int iWolrdID,
	                    const char *pszRoleName, int iListType, ZONEGETROLEPACKAGERES *pstOut,
	                    int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = GET_ROLE_PACKAGE_REQ;
	stIn.CsCmd.GetRolePackageReq.ListType = iListType;
	STRNCPY(stIn.CsCmd.GetRolePackageReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.GetRolePackageReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}
	*pstOut = stOut.CsCmd.GetRolePackageRes;

final:
	return iErr;
}

int cs_get_ride_info(int *pHandle, int iRegionid, int iWolrdID,
	                    const char *pszRoleName, ZONEGETROLERIDEINFORES *pstOut,
	                    int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = GET_ROLE_RIDE_REQ;
	stIn.CsCmd.GetRoleRideInfoReq.RoleID = 0;
	stIn.CsCmd.GetRoleRideInfoReq.Uin = 0;
	STRNCPY(stIn.CsCmd.GetRoleRideInfoReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.GetRoleRideInfoReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}
	*pstOut = stOut.CsCmd.GetRoleRideInfoRes;

final:
	return iErr;
}

int cs_set_ride_info(int *pHandle, int iRegionid,  int iWolrdID,
                      const char *pszRoleName, unsigned int iRideID,
                      unsigned int iRideLevel, int iEndTime, int iMode,
                      ZONESETROLERIDEINFORES *pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pszRoleName || !pHandle )
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	if(iMode < ROLE_RIDE_DEL || iMode > ROLE_RIDE_ADD)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	if(iEndTime < 0)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	memset (&stIn, 0, sizeof(stIn));
	stIn.CsCmd.SetRoleRideInfoReq.RoleID = 0;
	stIn.CsCmd.SetRoleRideInfoReq.Type = iMode;
	stIn.CsCmd.SetRoleRideInfoReq.Uin = 0;
	stIn.CsCmd.SetRoleRideInfoReq.ZoneRoleRideData.ItemID = iRideID;
	stIn.CsCmd.SetRoleRideInfoReq.ZoneRoleRideData.Level = iRideLevel;
	stIn.CsCmd.SetRoleRideInfoReq.ZoneRoleRideData.Endtime = iEndTime;
	STRNCPY(stIn.CsCmd.SetRoleRideInfoReq.RoleName, 
		      pszRoleName, sizeof(stIn.CsCmd.SetRoleRideInfoReq.RoleName));
	stIn.Cmd = SET_ROLE_RIDE_REQ;

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(strcmp(pszRoleName, stOut.CsCmd.SetRoleRideInfoRes.RoleName) != 0)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

	if(pstOut)
	{
		STRNCPY(pstOut->RoleName, pszRoleName, sizeof(pstOut->RoleName));	
	}

final:
	return iErr;
}

int cs_get_role_qiyue_list(int *pHandle, int iRegionid, int iWolrdID,
	                    const char *pszRoleName, GETROLEQIYUELISTRES *pstOut,
	                    int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = GET_ROLE_QIYUELIST_REQ;
	stIn.CsCmd.GetRoleQiYueListReq.RoleID = 0;
	stIn.CsCmd.GetRoleQiYueListReq.Uin = 0;
	STRNCPY(stIn.CsCmd.GetRoleQiYueListReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.GetRoleQiYueListReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}
	*pstOut = stOut.CsCmd.GetRoleQiYueListRes;

final:
	return iErr;
}

int cs_set_role_qiyue(int *pHandle, int iRegionid, int iWolrdID, 
	                    const char *pszRoleName,unsigned int iQiYueID,
	                    SETROLEQIYUERES *pstOut,
	                    int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = SET_ROLE_QIYUE_REQ;
	stIn.CsCmd.SetRoleQiYueReq.RoleID = 0;
	stIn.CsCmd.SetRoleQiYueReq.Uin = 0;
	STRNCPY(stIn.CsCmd.SetRoleQiYueReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.SetRoleQiYueReq.RoleName));
	stIn.CsCmd.SetRoleQiYueReq.QiYueID = iQiYueID;
	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}
	*pstOut = stOut.CsCmd.SetRoleQiYueRes;

final:
	return iErr;
}

int cs_add_role_qiyue(int *pHandle, int iRegionid, int iWolrdID, 
	                    const char *pszRoleName, unsigned int iQiYueID,
	                    ADDROLEQIYUERES *pstOut,
	                    int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = ADD_ROLE_QIYUE_REQ;
	stIn.CsCmd.AddRoleQiYueReq.RoleID = 0;
	stIn.CsCmd.AddRoleQiYueReq.Uin = 0;
	STRNCPY(stIn.CsCmd.AddRoleQiYueReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.AddRoleQiYueReq.RoleName));
	stIn.CsCmd.AddRoleQiYueReq.QiYueID = iQiYueID;
	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}
	*pstOut = stOut.CsCmd.AddRoleQiYueRes;

final:
	return iErr;
}

int cs_del_role_qiyue(int *pHandle, int iRegionid, int iWolrdID, 
	                    const char *pszRoleName, unsigned int iQiYueID,
	                    DELROLEQIYUERES *pstOut,
	                    int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = DEL_ROLE_QIYUE_REQ;
	stIn.CsCmd.DelRoleQiYueReq.RoleID = 0;
	stIn.CsCmd.DelRoleQiYueReq.Uin = 0;
	STRNCPY(stIn.CsCmd.DelRoleQiYueReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.DelRoleQiYueReq.RoleName));
	stIn.CsCmd.DelRoleQiYueReq.QiYueID = iQiYueID;
	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}
	*pstOut = stOut.CsCmd.DelRoleQiYueRes;

final:
	return iErr;
}


static int interface_fun_in(int *pHandle, const char * pszUri, SSCSCTRL *pIn, 
	                          SSCSCTRL *pOut, int iTimeOut)
{
	int iErr = 0;
	SSPKG *pstPkg = (SSPKG *)malloc(sizeof(SSPKG));
	TDRDATA stNet;
	SSCONTEXT stSSHan;
	char *psBuff = (char *)malloc(sizeof(SSPKG));
	CGIENV *pstCgiEnv = (CGIENV*)(pHandle);

	int iSock = tnet_connect(pszUri, iTimeOut);
	if(iSock < 0 || !pstPkg ||!psBuff)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

	assert(pHandle != NULL);

	pstPkg->Head.Cmd = CS_CTRL;
	pstPkg->Head.Magic = PROTOCOL_MAGIC; 
	pstPkg->Head.HeadLen = sizeof(pstPkg->Head);
	memcpy(&pstPkg->Body.CsCtrL, pIn, sizeof(SSCSCTRL));

	stSSHan.pstSSProtoLib = ((LPTDRMETALIB)pstCgiEnv->pstSSProtoLib);
	stSSHan.pstSSProtoPkgMeta = ((LPTDRMETA)pstCgiEnv->pstSSProtoPkgMeta);

	stNet.iBuff = sizeof(SSPKG);
	stNet.pszBuff = psBuff;
	
	if(cgi_ss_pack(&stSSHan, &stNet, pstPkg, 0) < 0)
	{
		printf("cgi_ss_pack fail\n");
		iErr = CS_ERR_TDR;
		goto final;
	}
	
	if(tnet_sendall(iSock, stNet.pszBuff, stNet.iBuff, 
		            iTimeOut < 0 ? 0 : iTimeOut) < 0)
	{
		printf("tnet_sendall fail\n");
		iErr = CS_ERR_NET;
		goto final;
	}

	if (pOut)
	{
		iErr = recv_svr(&stSSHan, iSock, pOut, iTimeOut);
		if(iErr )
		{
			goto final;
		}
	}
	
final:
	tnet_close(iSock);
	if(pstPkg)
		free(pstPkg);
	if(psBuff)
		free(psBuff);
	return iErr;
}



int cs_add_designation(int *pHandle, int iRegionID, int iWolrdID,
                       const char *pszRoleName, int iDesignationID,
                       int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = ADD_ROLE_DESIGNATION_REQ;
	STRNCPY(stIn.CsCmd.AddRoleDesignationReq.RoleName, pszRoleName, 
		    sizeof(stIn.CsCmd.AddRoleDesignationReq.RoleName));
	stIn.CsCmd.AddRoleDesignationReq.DesignationID = iDesignationID;
	iErr = cs_interface_fun(pHandle, iRegionID, iWolrdID, &stIn, &stOut,
	                        iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(!stOut.CsCmd.AddRoleDesignationRes.Succeeded)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

final:
	return iErr;
}
                       
int cs_del_designation(int *pHandle, int iRegionID, int iWolrdID,
                       const char *pszRoleName, int iDesignationID,
                       int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = DEL_ROLE_DESIGNATION_REQ;
	STRNCPY(stIn.CsCmd.DelRoleDesignationReq.RoleName, pszRoleName, 
		    sizeof(stIn.CsCmd.DelRoleDesignationReq.RoleName));
	stIn.CsCmd.DelRoleDesignationReq.DesignationID = iDesignationID;
	iErr = cs_interface_fun(pHandle, iRegionID, iWolrdID, &stIn, &stOut,
	                        iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(!stOut.CsCmd.DelRoleDesignationRes.Succeeded)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

final:
	return iErr;
}
                       
int cs_get_designation_list(int *pHandle, int iRegionID, int iWolrdID,
                            const char *pszRoleName, ROLEDESIGNATIONS *pstOut,
                            int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = GET_ROLE_DESIGNATION_LIST_REQ;
	STRNCPY(stIn.CsCmd.GetRoleDesignationListReq.RoleName, pszRoleName, 
		    sizeof(stIn.CsCmd.GetRoleDesignationListReq.RoleName));
	iErr = cs_interface_fun(pHandle, iRegionID, iWolrdID, &stIn, &stOut,
	                        iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(!stOut.CsCmd.GetRoleDesignationListRes.Succeeded)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

	*pstOut = stOut.CsCmd.GetRoleDesignationListRes.Designations;

final:
	return iErr;
}

int cs_set_respact(int *pHandle, int iRegionID, int iWolrdID,
                   const char *pszRoleName, int iRespactPartyID,
                   int iRespactValue, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = SET_ROLE_RESPACT_REQ;
	STRNCPY(stIn.CsCmd.SetRoleRespactReq.RoleName, pszRoleName, 
		    sizeof(stIn.CsCmd.SetRoleRespactReq.RoleName));
	stIn.CsCmd.SetRoleRespactReq.RespactPartyID = iRespactPartyID;
	stIn.CsCmd.SetRoleRespactReq.RespactValue = iRespactValue;
	iErr = cs_interface_fun(pHandle, iRegionID, iWolrdID, &stIn, &stOut,
	                        iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(!stOut.CsCmd.SetRoleRespactRes.Succeeded)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

final:
	return iErr;
}

int cs_del_respact(int *pHandle, int iRegionID, int iWolrdID,
                   const char *pszRoleName, int iRespactPartyID, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = DEL_ROLE_RESPACT_REQ;
	STRNCPY(stIn.CsCmd.DelRoleRespactReq.RoleName, pszRoleName, 
		    sizeof(stIn.CsCmd.DelRoleRespactReq.RoleName));
	stIn.CsCmd.DelRoleRespactReq.RespactPartyID = iRespactPartyID;

	iErr = cs_interface_fun(pHandle, iRegionID, iWolrdID, &stIn, &stOut,
	                        iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(!stOut.CsCmd.DelRoleRespactRes.Succeeded)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

final:
	return iErr;
}

int cs_respact_list(int *pHandle, int iRegionID, int iWolrdID,
                    const char *pszRoleName, RESPACTINFO *pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = GET_ROLE_RESPACT_LIST_REQ;
	STRNCPY(stIn.CsCmd.GetRoleRespactListReq.RoleName, pszRoleName, 
		    sizeof(stIn.CsCmd.GetRoleRespactListReq.RoleName));
	iErr = cs_interface_fun(pHandle, iRegionID, iWolrdID, &stIn, &stOut,
	                        iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if(!stOut.CsCmd.GetRoleRespactListRes.Succeeded)
	{
		iErr = CS_ERR_SYS;
		goto final;
	}

	*pstOut = stOut.CsCmd.GetRoleRespactListRes.RespactInfo;

final:
	return iErr;
}

static FIND_ACCOUNT_ROLE_HANDLE create_find_account_role_handle(
	int *pHandle,
	int iRegionID,
	int iWorldID,
	const char *pszPattern,
	int iTimeOut,
	int iByWhat)
{
	int i = 0;
	FIND_ACCOUNT_ROLE_HANDLE pstContext = NULL;
	CGIENV *pstCgiEnv = (CGIENV*)(pHandle);
	const CGICONF *pstCgiConf = !pstCgiEnv
	                          ? NULL : (const CGICONF*)pstCgiEnv->pstConf;

	if(pstCgiConf == NULL)
	{
		goto fatal;
	}

	if (0 == iRegionID && 0 == iWorldID)
	{
		//不做判断
	}
	else
	{
		for(i = 0; i < (int)pstCgiConf->AccountCount; i++)
		{
			if((int)pstCgiConf->AccountInfos[i].RegionID == iRegionID)
			{
				break;
			}
		}
		if(i >= pstCgiConf->AccountCount)
		{
			goto fatal;
		}
	}
	
	pstContext = calloc(1, sizeof(struct _FindAccountRoleContext));
	if(!pstContext)
	{
		goto fatal;
	}
	
	pstContext->iByWhat = iByWhat;
	pstContext->iTimeOut = iTimeOut;
	pstContext->iRegionID = iRegionID;
	pstContext->iWorldID = iWorldID;
	pstContext->pHandle = pHandle;
	switch(pstContext->iByWhat)
	{
	case BY_ROLE_NAME:
		STRNCPY(pstContext->stKey.RoleNamePattern, pszPattern,
		        sizeof(pstContext->stKey.RoleNamePattern));
		break;

	case BY_LOGIN_NAME:
		STRNCPY(pstContext->stKey.LoginNamePattern, pszPattern,
		        sizeof(pstContext->stKey.LoginNamePattern));
		break;

	default:
		goto fatal;
		break;
	}
	return pstContext;
	
fatal:
	if(pstContext)
	{
		free(pstContext);
	}
	return NULL;
}

static int get_account_role_by(
	int *pHandle,
	int iRegionID,
	int iWorldID,
	int iByWhat,
	const char *pszPattern,
	int iIndex,
	ACCFINDROLELISTRES *pstOut,
	int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pszPattern || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = FIND_ACCOUNT_ROLELIST_REQ;
	stIn.CsCmd.FindAccountRoleListReq.Index = iIndex;
	stIn.CsCmd.FindAccountRoleListReq.ByWhat = iByWhat;
	stIn.CsCmd.FindAccountRoleListReq.ServerID = iWorldID;
	switch(stIn.CsCmd.FindAccountRoleListReq.ByWhat)
	{
	case FIND_ACCOUNT_ROLELIST_BY_LOGINNAME:
		STRNCPY(stIn.CsCmd.FindAccountRoleListReq.Key.LoginNamePattern,
			    pszPattern,
			    sizeof(stIn.CsCmd.FindAccountRoleListReq.Key.LoginNamePattern));
		break;
	case FIND_ACCOUNT_ROLELIST_BY_ROLENAME:
		STRNCPY(stIn.CsCmd.FindAccountRoleListReq.Key.RoleNamePattern,
		        pszPattern,
		        sizeof(stIn.CsCmd.FindAccountRoleListReq.Key.RoleNamePattern));
		break;
	default:
		return CS_ERR_ARGS;
		break;
	}

	iErr = acc_interface_fun(pHandle, iRegionID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}
	
	*pstOut = stOut.CsCmd.FindAccountRoleListRes;

final:
	return iErr;
}

int cs_find_first_role_by_loginname(
	int *pHandle,
	int iRegionID,
	int iWorldID,
	const char *pszLoginNamePattern,
	FIND_ACCOUNT_ROLE_HANDLE *pFind,
	ACCFINDROLELISTRES *pstOut,
	int iTimeOut)
{
	int iErr = 0;
	FIND_ACCOUNT_ROLE_HANDLE pstContext = create_find_account_role_handle(
		pHandle,
		iRegionID,
		iWorldID,
		pszLoginNamePattern,
		iTimeOut,
		BY_LOGIN_NAME);
	if(!pstContext)
	{
		iErr = CS_ERR_SYS;
		goto error;
	}
	
	iErr = get_account_role_by(
		pstContext->pHandle,
		pstContext->iRegionID,
		pstContext->iWorldID,
		FIND_ACCOUNT_ROLELIST_BY_LOGINNAME,
		pstContext->stKey.LoginNamePattern,
		pstContext->iQueryIndex++,
		pstOut,
		pstContext->iTimeOut);
	if(iErr)
	{
		goto error;
	}
	if(!pstOut->ResNum)
	{
		iErr = CS_ERR_NOT_FOUND;
		goto error;
	}
	
	pstContext->iPrevCount = pstOut->ResNum;
	*pFind = pstContext;
	return 0;
	
error:
	cs_close_handle(pstContext);
	bzero(pstOut, sizeof(*pstOut));
	return iErr;
}

int cs_find_first_role_by_rolename(
	int *pHandle,
	int iRegionID,
	int iWorldID,
	const char *pszRoleNamePattern,
	FIND_ACCOUNT_ROLE_HANDLE *pFind,
	ACCFINDROLELISTRES *pstOut,
	int iTimeOut)
{
	int iErr = 0;
	FIND_ACCOUNT_ROLE_HANDLE pstContext = create_find_account_role_handle(
		pHandle,
		iRegionID,
		iWorldID,
		pszRoleNamePattern,
		iTimeOut,
		BY_ROLE_NAME);
	
	if(!pstContext)
	{
		iErr = CS_ERR_SYS;
		goto error;
	}
	
	iErr = get_account_role_by(
		pstContext->pHandle,
		pstContext->iRegionID,
		pstContext->iWorldID,
		FIND_ACCOUNT_ROLELIST_BY_ROLENAME,
		pstContext->stKey.RoleNamePattern,
		pstContext->iQueryIndex++,
		pstOut,
		pstContext->iTimeOut);
	if(iErr)
	{
		goto error;
	}
	if(!pstOut->ResNum)
	{
		iErr = CS_ERR_NOT_FOUND;
		goto error;
	}
	
	pstContext->iPrevCount = pstOut->ResNum;
	*pFind = pstContext;
	return 0;
	
error:
	cs_close_handle(pstContext);
	bzero(pstOut, sizeof(*pstOut));
	return iErr;
}

int cs_find_next_role(FIND_ACCOUNT_ROLE_HANDLE pstContext,
                      ACCFINDROLELISTRES *pstOut)
{
	int iErr = 0;
	
	if(pstContext->iPrevCount < ROLE_MAX_RES)
	{
		iErr = CS_ERR_NOT_FOUND;
		goto error;
	}
	
	switch(pstContext->iByWhat)
	{
	case BY_LOGIN_NAME:
		iErr = get_account_role_by(
			pstContext->pHandle,
			pstContext->iRegionID,
			pstContext->iWorldID,
			FIND_ACCOUNT_ROLELIST_BY_LOGINNAME,
			pstContext->stKey.LoginNamePattern,
			pstContext->iQueryIndex++,
			pstOut,
			pstContext->iTimeOut);
		break;
		
	case BY_ROLE_NAME:
		iErr = get_account_role_by(
			pstContext->pHandle,
			pstContext->iRegionID,
			pstContext->iWorldID,
			FIND_ACCOUNT_ROLELIST_BY_ROLENAME,
			pstContext->stKey.RoleNamePattern,
			pstContext->iQueryIndex++,
			pstOut,
			pstContext->iTimeOut);
		break;
		
	default:
		iErr = CS_ERR_ARGS;
		goto error;
		break;
	}
	if(iErr)
	{
		goto error;
	}
	if(!pstOut->ResNum)
	{
		iErr = CS_ERR_NOT_FOUND;
		goto error;
	}
	
	pstContext->iPrevCount = pstOut->ResNum;
	return 0;
error:
	bzero(pstOut, sizeof(*pstOut));
	return iErr;
}

int cs_find_all_first_role_by_rolename(int *pHandle
                              , char *pszRoleNamePattern, FIND_ACCOUNT_ROLE_HANDLE *pFind, ACCFINDROLELISTRES *pstOut
                              , int iTimeOut)
{
	CGICONF *pstCgiConf = NULL;
	CGIENV *pstCgiEnv = (CGIENV*)(pHandle);
	int iRegionID = 0;
	int iWolrdID = 0;
	int iErr = 0;
	FIND_ACCOUNT_ROLE_HANDLE pstContext = NULL;

	if(!pHandle || !pszRoleNamePattern || !pFind || !pstOut)
	{
		return CS_ERR_ARGS;
	}

	pstCgiConf = (CGICONF*)pstCgiEnv->pstConf;
	if(pstCgiConf == NULL)
	{
		return CS_ERR_ARGS;
	}

	if (pstCgiConf->WorldCount < 1)
	{
		return CS_ERR_ARGS;
	}

	iWolrdID = pstCgiConf->WorldInfos[0].WorldID;
	iRegionID = (int)pstCgiConf->WorldInfos[0].RegionID;

	pstContext = create_find_account_role_handle(
		pHandle,
		iRegionID,
		iWolrdID,
		pszRoleNamePattern,
		iTimeOut,
		BY_ROLE_NAME);

	if(!pstContext)
	{
		return CS_ERR_ARGS;
	}

	iErr = get_account_role_by(
		pstContext->pHandle,
		pstContext->iRegionID,
		pstContext->iWorldID,
		FIND_ACCOUNT_ROLELIST_BY_ROLENAME,
		pstContext->stKey.RoleNamePattern,
		pstContext->iQueryIndex++,
		pstOut,
		pstContext->iTimeOut);
	if(iErr)
	{
		goto error;
	}
	
	pstContext->iPrevCount = ROLE_MAX_RES;
	
	*pFind = pstContext;
	return 0;
	
error:
	cs_close_handle(pstContext);
	bzero(pstOut, sizeof(*pstOut));
	return iErr;
}

int cs_find_all_next_role(int *pHandle 
                              , FIND_ACCOUNT_ROLE_HANDLE hFind , ACCFINDROLELISTRES *pstOut)
{
	int iErr = 0;
	CGICONF *pstCgiConf = NULL;
	CGIENV *pstCgiEnv = (CGIENV*)(pHandle);

	if(!pHandle || !hFind || !pstOut)
	{
		return CS_ERR_ARGS;
	}

	pstCgiConf = (CGICONF*)pstCgiEnv->pstConf;
	if(pstCgiConf == NULL)
	{
		return CS_ERR_ARGS;
	}

	iErr = get_account_role_by(
		hFind->pHandle,
		hFind->iRegionID,
		hFind->iWorldID,
		FIND_ACCOUNT_ROLELIST_BY_ROLENAME,
		hFind->stKey.RoleNamePattern,
		hFind->iQueryIndex++,
		pstOut,
		hFind->iTimeOut);
	
	//if(iErr)
	//{
		//return iErr;
	//}
	
	while(!pstOut->ResNum)
	{
		//找下一个WORLD
		int i = 0;	
		
		for(i=0; i<pstCgiConf->WorldCount; i++)
		{
			if((int)pstCgiConf->WorldInfos[i].WorldID == hFind->iWorldID &&
			   (int)pstCgiConf->WorldInfos[i].RegionID == hFind->iRegionID)
			{
				if (i+1 < pstCgiConf->WorldCount)
				{
					hFind->iWorldID = pstCgiConf->WorldInfos[i+1].WorldID;
					hFind->iRegionID = pstCgiConf->WorldInfos[i+1].RegionID;
				}
				else
				{
					hFind->iWorldID = 0;
					hFind->iRegionID = 0;
				}

				break;
			}
		}

		if (i >= pstCgiConf->WorldCount || 
			(0 == hFind->iWorldID&& 0 == hFind->iRegionID))
		{
			return CS_ERR_NOT_FOUND;
		}

		hFind->iQueryIndex = 0;

		iErr = get_account_role_by(
			hFind->pHandle,
			hFind->iRegionID,
			hFind->iWorldID,
			FIND_ACCOUNT_ROLELIST_BY_ROLENAME,
			hFind->stKey.RoleNamePattern,
			hFind->iQueryIndex++,
			pstOut,
			hFind->iTimeOut);
		
		if(iErr)
		{
			hFind->iPrevCount = 0;
			//return iErr;
		}		
	}
	
	hFind->iPrevCount = ROLE_MAX_RES;

	return 0;
}


static void account_id_copy_from(const ACCFINDROLELISTRES *pstFrom,
                                 CSAccountIDRes *pstTo)
{
	int i = 0;
	pstTo->iResNum = 0;
	for(i = 0; i < pstFrom->ResNum; i++)
	{
		pstTo->astAccountIDs[i].iUin = pstFrom->RoleArray[i].Uin;
		strncpy(pstTo->astAccountIDs[i].szPass9Account,
		        pstFrom->RoleArray[i].Pass9Account,
		        sizeof(pstTo->astAccountIDs[i].szPass9Account));
		pstTo->iResNum++;
	}
}

int cs_find_first_account_id_by_rolename(
	int *pHandle,
	int iRegionID,
	int iWorldID,
	const char *pszRoleNamePattern,
	FIND_ACCOUNT_ROLE_HANDLE *pFind,
	CSAccountIDRes *pstOut,
	int iTimeOut)
{
	ACCFINDROLELISTRES stRes;
	int iErr = cs_find_first_role_by_rolename(
		pHandle,
		iRegionID,
		iWorldID,
		pszRoleNamePattern,
		pFind,
		&stRes,
		iTimeOut);
	if(iErr)
	{
		return iErr;
	}
	account_id_copy_from(&stRes, pstOut);
	return 0;
}
	
int cs_find_next_account_id(FIND_ACCOUNT_ROLE_HANDLE hFind,
                            CSAccountIDRes *pstOut)
{
	ACCFINDROLELISTRES stRes;
	int iErr = cs_find_next_role(hFind, &stRes);
	if(iErr)
	{
		return iErr;
	}
	account_id_copy_from(&stRes, pstOut);
	return 0;
}

static LIMITED_FIND_ACCOUNT_HANDLE create_limited_find_account_handle(
	int *pHandle,
	int iRegionID,
	const char *pszLoginNamePattern,
	int iTimeOut)
{
	int i = 0;
	LIMITED_FIND_ACCOUNT_HANDLE pstContext = NULL;
	CGIENV *pstCgiEnv = (CGIENV*)(pHandle);
	const CGICONF *pstCgiConf = !pstCgiEnv
	                          ? NULL : (const CGICONF*)pstCgiEnv->pstConf;

	if(pstCgiConf == NULL)
	{
		goto fatal;
	}

	for(i = 0; i < (int)pstCgiConf->AccountCount; i++)
	{
		if((int)pstCgiConf->AccountInfos[i].RegionID == iRegionID)
		{
			break;
		}
	}
	if(i >= pstCgiConf->AccountCount)
	{
		goto fatal;
	}
	
	pstContext = calloc(1, sizeof(struct _LimitedFindAccountContext));
	if(!pstContext)
	{
		goto fatal;
	}
	
	pstContext->iTimeOut = iTimeOut;
	pstContext->iRegionID = iRegionID;
	pstContext->pHandle = pHandle;
	STRNCPY(pstContext->szPattern, pszLoginNamePattern,
	        sizeof(pstContext->szPattern));
	return pstContext;
	
fatal:
	if(pstContext)
	{
		free(pstContext);
	}
	return NULL;
}

int cs_limited_find_first_account_by_loginname(
	int *pHandle,
	int iRegionID,
	const char *pszLoginNamePattern,
	LIMITED_FIND_ACCOUNT_HANDLE *pFind,
	ACCFINDRES *pstOut,
	int iTimeOut)
{
	int iErr = 0;
	LIMITED_FIND_ACCOUNT_HANDLE pstContext = create_limited_find_account_handle(
		pHandle,
		iRegionID,
		pszLoginNamePattern,
		iTimeOut);
	if(!pstContext)
	{
		iErr = CS_ERR_SYS;
		goto error;
	}

	iErr = get_account_by(
		pstContext->pHandle,
		pstContext->iRegionID,
		FIND_ACCOUNT_BY_LOGINNAME,
		pstContext->szPattern,
		pstContext->iQueryIndex++,
		pstOut,
		pstContext->iTimeOut);
	if(iErr)
	{
		goto error;
	}
	if(!pstOut->ResNum)
	{
		iErr = CS_ERR_NOT_FOUND;
		goto error;
	}
	
	pstContext->iPrevCount = pstOut->ResNum;
	*pFind = pstContext;
	pstOut->ReginID = pstContext->iRegionID;
	return 0;
	
error:
	cs_close_handle(pstContext);
	bzero(pstOut, sizeof(*pstOut));
	return iErr;
}

int cs_limited_find_next_account(LIMITED_FIND_ACCOUNT_HANDLE pstContext,
                                 ACCFINDRES *pstOut)
{
	int iErr = 0;
	
	if(pstContext->iPrevCount < ACC_MAX_RES)
	{
		iErr = CS_ERR_NOT_FOUND;
		goto error;
	}
	
	iErr = get_account_by(
		pstContext->pHandle,
		pstContext->iRegionID,
		FIND_ACCOUNT_BY_LOGINNAME,
		pstContext->szPattern,
		pstContext->iQueryIndex++,
		pstOut,
		pstContext->iTimeOut);
	if(iErr)
	{
		goto error;
	}
	if(!pstOut->ResNum)
	{
		iErr = CS_ERR_NOT_FOUND;
		goto error;
	}
	
	pstContext->iPrevCount = pstOut->ResNum;
	pstOut->ReginID = pstContext->iRegionID;
	return 0;

error:
	bzero(pstOut, sizeof(*pstOut));
	return iErr;
}

void cs_close_handle(void *hHandle)
{
	if(hHandle)
	{
		free(hHandle);
	}
}

int cs_query_bill_point(int *pHandle,
	int iRegionID,
	const char *pszLoginNamePattern,
	BILLRECORD *pstOut,
	int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd =CS_QUERY_BILL_REQ;// GET_ACCOUNT_INFO_REQ;
	STRNCPY(stIn.CsCmd.QeuryBillRecordReq.Pass9Account, pszLoginNamePattern,
	        sizeof(stIn.CsCmd.QeuryBillRecordReq.Pass9Account));

	iErr = acc_interface_fun(pHandle, iRegionID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	*pstOut = stOut.CsCmd.QeuryBillRecordRes;

final:
	return iErr;
}

int cs_cash_props_gift_bill(int *pHandle,
	int iRegionID,
	const char *pszLoginName,
	int iUin,
	int iGroupID,
	int iAddCash,
	CASHPROPSGIFTINFORES *pstOut,
	int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if(!pHandle || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd =CS_CASH_PROPS_BILL_REQ;
	STRNCPY(stIn.CsCmd.CashPropsGiftInfoReq.Pass9Account, pszLoginName,
	        sizeof(stIn.CsCmd.CashPropsGiftInfoReq.Pass9Account));
	stIn.CsCmd.CashPropsGiftInfoReq.Uin = iUin;
	stIn.CsCmd.CashPropsGiftInfoReq.GroupID = iGroupID;	
	stIn.CsCmd.CashPropsGiftInfoReq.AddCash = iAddCash;

	iErr = acc_interface_fun(pHandle, iRegionID, &stIn, &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	*pstOut = stOut.CsCmd.CashPropsGiftInfoRes;

final:
	return iErr;
}

int cs_get_package_item_info(int *pHandle, int iRegionid, int iWolrdID,
	                    const char *pszRoleName, int iListType, ZONEGETPACKAGEITEMINFORES*pstOut,
	                    int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = GET_ROLE_PACKAGE_ITEM_REQ;
	stIn.CsCmd.GetPackageItemInfoReq.ListType = iListType;
	STRNCPY(stIn.CsCmd.GetPackageItemInfoReq.RoleName, pszRoleName, 
		      sizeof(stIn.CsCmd.GetPackageItemInfoReq.RoleName));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}
	*pstOut = stOut.CsCmd.GetPackageItemInfoRes;

final:
	return iErr;
}




int cs_role_back_record(int *pHandle, int iRegionid, int iWolrdID,const char *pszRoleName,const char *pszDate, 
							ZONEROLEBACKRES *pstOut, int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	if (!pHandle || !pszRoleName || !pszDate)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = ROLE_BACK_RECORD_REQ;
	strncpy(stIn.CsCmd.RoleBackReq.RoleName, pszRoleName, sizeof(stIn.CsCmd.RoleBackReq.RoleName));
	strncpy(stIn.CsCmd.RoleBackReq.BackData, pszDate, sizeof(stIn.CsCmd.RoleBackReq.BackData));

	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}
	if (pstOut)
		*pstOut = stOut.CsCmd.RoleBackRes;

final:
	return iErr;
}
/*static int back_record_get_date(const char *pszDate, char* szYear, char *szMonth)
{
	//2011-07-01 15:32:08
	const char *p = pszDate;
	strncpy(szYear,p+2,2);
	szYear[2] = 0;
	strncpy(szMonth,p+5,2);
	szMonth[2] = 0;
	return 0;
}

// 回档
int cs_role_back_record(int *pHandle, int iRegionid, int iWolrdID,const char *pszRoleName,const char *pszDate, 
							ZONEROLEBACKRES *pstOut, int iTimeOut)
{
	if (!pHandle || !pszRoleName)
		return -1;
	
	CGIENV *pstCgiEnv = (CGIENV*)(pHandle);
	CGICONF *pstConf = (CGICONF *)pstCgiEnv->pstConf;
	if (!pstConf)
		return -1;
	
	TDRDBMS stDBMS;
	TDRDBHANDLE hResDBHandle;
	char szErrorMsg[1024];
	TDRDBRESULTHANDLE hDBResult = 0;
	char szSql[10240];
	int iNumRows;
	TDRDATA stSql;
	LPTDRMETALIB pstResLib;
	LOGROLESNAPSHOT *pstSanpshot, stSanpshot;
	int iErr;
	char szYear[16],szMonth[16];
	
	memset(&stDBMS, 0, sizeof(stDBMS));
	STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
	STRNCPY(stDBMS.szDBMSCurDatabaseName, pstConf->LogDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
	STRNCPY(stDBMS.szDBMSConnectionInfo, pstConf->LogDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
	STRNCPY(stDBMS.szDBMSUser, pstConf->LogDBUser, sizeof(stDBMS.szDBMSUser));
	STRNCPY(stDBMS.szDBMSPassword, pstConf->LogDBPasswd, sizeof(stDBMS.szDBMSPassword));
	stDBMS.iReconnectOpt = 1;	
	
	if (0 > tdr_open_dbhanlde(&hResDBHandle, &stDBMS, &szErrorMsg[0]))
	{
		//printf("tdr_open_dbhanlde fail: %s", szErrorMsg);
		return CS_ERR_SYS;
	}

	if (0 > tdr_load_metalib(&pstResLib, "../../cfg/db_meta/role_db_meta.tdr") && 
		0 > tdr_load_metalib(&pstResLib, "role_db_meta.tdr"))
	{
		return CS_ERR_SYS;
	}

	stSql.iBuff = sizeof(szSql);
	stSql.pszBuff = &szSql[0];

	TDRDBOBJECT stDBObj;
	memset (&stDBObj, 0, sizeof(stDBObj));
	stDBObj.pstMeta = tdr_get_meta_by_name(pstResLib, "LogRoleSnapshot");
	if (!stDBObj.pstMeta)
	{
		return CS_ERR_SYS;
	}	
	stDBObj.iVersion = tdr_get_meta_current_version(stDBObj.pstMeta);
	stDBObj.iObjSize = sizeof(stSanpshot);
	stDBObj.pszObj = (char*)&stSanpshot;

	back_record_get_date(pszDate, szYear, szMonth);
	memset(szSql, 0, sizeof(szSql));
	snprintf(szSql, sizeof(szSql),
			"select * from LogRoleSnapshot_%s%s where RoleName='%s' AND LogTime='%s'",
			szYear,szMonth,pszRoleName,pszDate);
	stSql.iBuff = strlen(szSql);
	stSql.pszBuff = &szSql[0];
	if(tdr_query(&hDBResult, hResDBHandle, &stSql) != 0)
	{
		return CS_ERR_SYS;
	}

	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows != 1)
	{
		return CS_ERR_SYS;
	}

	int iRet = tdr_fetch_row(hDBResult, &stDBObj);
	if (iRet != 0)
		return CS_ERR_SYS;

	pstSanpshot = (LOGROLESNAPSHOT *)stDBObj.pszObj;
	
	tdr_free_dbresult(&hResDBHandle);
	
	SSCSCTRL stIn;
	SSCSCTRL stOut;

	stIn.Cmd = ROLE_BACK_RECORD_REQ;
	strncpy(stIn.CsCmd.RoleBackReq.RoleName, pszRoleName, sizeof(stIn.CsCmd.RoleBackReq.RoleName));
	stIn.CsCmd.RoleBackReq.RoleData = pstSanpshot->LogData.LogRoleData;
	
	iErr = cs_interface_fun(pHandle,iRegionid,iWolrdID, &stIn,  &stOut, iTimeOut);
	if(iErr)
	{
		goto final;
	}

	if (pstOut)
	{
		*pstOut = stOut.CsCmd.RoleBackRes;
	}
final:
		
	return iErr;
}*/

static FIND_CLAN_HANDLE create_find_clan_handle(
	int *pHandle,
	int iRegionID,
	int iWorldID,
	const char *pszPattern,
	int iTimeOut)
{
	FIND_CLAN_HANDLE pstContext = calloc(1, sizeof(struct _FindClanContext));

	if (!pstContext)
	{
		return NULL;
	}

	pstContext->pHandle = pHandle;
	pstContext->iRegionID = iRegionID;
	pstContext->iWorldID = iWorldID;
	STRNCPY(pstContext->szClanNamePattern, pszPattern,
	        sizeof(pstContext->szClanNamePattern));
	pstContext->iTimeOut = iTimeOut;

	return pstContext;
}

static int get_clan_list(
	int *pHandle,
	int iRegionID,
	int iWorldID,
	const char *pszClanNamePattern,
	int iIndex,
	ZONEFINDCLANRES *pstOut,
	int iTimeOut)
{
	int i, iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;
	
	if(!pHandle || !pstOut)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = FIND_CLAN_REQ;
	stIn.CsCmd.FindClanReq.Index = iIndex;
	STRNCPY(stIn.CsCmd.FindClanReq.Pattern, pszClanNamePattern,
		    sizeof(stIn.CsCmd.FindClanReq.Pattern));

	iErr = cs_interface_fun(pHandle, iRegionID, iWorldID, &stIn, &stOut,
	                        iTimeOut);
	if(iErr)
	{
		goto final;
	}

	pstOut->ResNum = stOut.CsCmd.FindClanRes.ResNum;
	i = stOut.CsCmd.FindClanRes.ResNum;
	while(i--)
	{
		pstOut->ClanArray[i] = stOut.CsCmd.FindClanRes.ClanArray[i];
	}

final:
	return iErr;
}

int cs_find_first_clan_profile_by_name(
	int *pHandle,
	int iRegionID,
	int iWorldID,
	const char *pszClanNamePattern,
	FIND_CLAN_HANDLE *pFind,
	ZONEFINDCLANRES *pstOut,
	int iTimeOut)
{
	int iErr = 0;
	FIND_CLAN_HANDLE pstFind = create_find_clan_handle(
		pHandle,
		iRegionID,
		iWorldID,
		pszClanNamePattern,
		iTimeOut);
	if(!pstFind)
	{
		iErr = CS_ERR_SYS;
		goto error;
	}

	pstFind->iQueryIndex = 0;
	iErr = get_clan_list(
		pstFind->pHandle,
		pstFind->iRegionID,
		pstFind->iWorldID,
		pstFind->szClanNamePattern,
		pstFind->iQueryIndex++,
		pstOut,
		pstFind->iTimeOut);
	if(iErr)
	{
		goto error;
	}

	pstFind->iPrevCount = pstOut->ResNum;
	if(!pstOut->ResNum)
	{
		iErr = CS_ERR_NOT_FOUND;
		goto error;
	}

	*pFind = pstFind;
	return 0;

error:
	cs_close_handle(pstFind);
	bzero(pstOut, sizeof(*pstOut));
	return iErr;
}

int cs_find_next_clan_profile(FIND_CLAN_HANDLE pstFind, ZONEFINDCLANRES *pstOut)
{
	int iErr = 0;

	if(pstFind->iPrevCount < CLAN_MAX_RES)
	{
		iErr = CS_ERR_NOT_FOUND;
		goto error;
	}

	iErr = get_clan_list(
		pstFind->pHandle,
		pstFind->iRegionID,
		pstFind->iWorldID,
		pstFind->szClanNamePattern,
		pstFind->iQueryIndex++,
		pstOut,
		pstFind->iTimeOut);
	if(iErr)
	{
		goto error;
	}

	pstFind->iPrevCount = pstOut->ResNum;
	if(!pstOut->ResNum)
	{
		iErr = CS_ERR_NOT_FOUND;
		goto error;
	}
	return 0;

error:
	bzero(pstOut, sizeof(*pstOut));
	return iErr;
}

int cs_reload_server(int *pHandle, int iRegionID, int iWorldID
                              , ZONERELOADSERVERRES *pstOut,int iTimeOut)
{
	SSCSCTRL stIn;
	SSCSCTRL stOut;
	int iRet = 0;

	if(!pHandle || !pstOut)
	{
		return CS_ERR_ARGS;
	}

	stIn.Cmd = RELOAD_SERVER_REQ;
	
	stIn.CsCmd.ReloadServerReq.Res = 0;

	

	iRet = cs_interface_fun(pHandle, iRegionID, iWorldID, &stIn, &stOut,
	                        iTimeOut);
	*pstOut = stOut.CsCmd.ReloadServerRes;

	return iRet;
}

int cs_exchange_clan_leader(
	int *pHandle,
	int iRegionID,
	int iWorldID,
	const char *pszClanName,
	const char *pszRoleName,
	int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;
	
	if(!pHandle)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = EXCHANGE_CLAN_LEADER_REQ;
	STRNCPY(stIn.CsCmd.ExchangeClanLeaderReq.ClanName, pszClanName,
		    sizeof(stIn.CsCmd.ExchangeClanLeaderReq.ClanName));
	STRNCPY(stIn.CsCmd.ExchangeClanLeaderReq.NewLeaderRoleName, pszRoleName,
	        sizeof(stIn.CsCmd.ExchangeClanLeaderReq.NewLeaderRoleName));

	iErr = cs_interface_fun(pHandle, iRegionID, iWorldID, &stIn, &stOut,
	                        iTimeOut);
final:
	return iErr;
}

int cs_del_clan_diplomatism(
	int *pHandle,
	int iRegionID,
	int iWorldID,
	const char *pszClanName,
	const char *pszOtherClanName,
	int iTimeOut)
{
	int iErr = 0;
	SSCSCTRL stIn;
	SSCSCTRL stOut;
	
	if(!pHandle)
	{
		iErr = CS_ERR_ARGS;
		goto final;
	}

	stIn.Cmd = DEL_CLAN_DIPLOMATISM_REQ;
	STRNCPY(stIn.CsCmd.DelClanDiplomatismReq.ClanName, pszClanName,
	        sizeof(stIn.CsCmd.DelClanDiplomatismReq.ClanName));
	STRNCPY(stIn.CsCmd.DelClanDiplomatismReq.TargetName, pszOtherClanName,
	        sizeof(stIn.CsCmd.DelClanDiplomatismReq.TargetName));

	iErr = cs_interface_fun(pHandle, iRegionID, iWorldID, &stIn, &stOut,
	                        iTimeOut);
final:
	return iErr;
}
