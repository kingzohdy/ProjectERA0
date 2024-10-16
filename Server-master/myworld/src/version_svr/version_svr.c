/*
**  @file RCSfile
**  general description of this module
**  Id
**  @author Author
**  @date Date
**  @version Revision
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "tapp/tapp.h"
#include "tdr/tdr.h"
#include "tlog/tlog.h"
#include "proto_comm.h"
#include "version_svr_conf_desc.h"
#include "version.h"
#include "cs_net.h"
#include "entity.h"
#include "tconnapi.h"
#include "proto_cs.h"
#include "tbus/tbus.h"
#include "md5.h"

#define LOOP_NUM 100
#define CS_MAX_MSG CS_MAX_BODY+sizeof(CSPKGHEAD)+sizeof(TFRAMEHEAD)
#define UNUSED( var ) ((void)var)

struct tagversion_svrEnv
{
	VERSION_SVRCONF *pstConf;
	VERSION_SVRRUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;
	unsigned long int iTconnapiHandle;
	int iConnID;
	NETHANDLE *pstCSHan; 
	time_t tLastModify;
	char szRecvBuffer[CS_MAX_MSG];
	char szSendBuffer[CS_MAX_MSG];
	unsigned int iVersionNew;//最新的版本
	unsigned int iVersionMin; //最老的版本
};

typedef struct tagversion_svrEnv		VERSION_SVRENV;
typedef struct tagversion_svrEnv		*LVERSION_SVRENV;


static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_version_svr[];
extern unsigned char g_szMetalib_proto_cs[];

VERSION_SVRENV gs_stEnv;
static VERSION_SVRRUN_STATUS gs_stStat;
int g_iNow;

int version_svr_reload(TAPPCTX *pstAppCtx, VERSION_SVRENV *pstEnv);

int UpPackCmp(const void *p1, const void *p2)
{
	unsigned int iVer1;
	unsigned int iVer2;
	UPDATAPACK *pstPack1 = (UPDATAPACK *)p1;
	UPDATAPACK *pstPack2 = (UPDATAPACK *)p2;

	iVer1 = version_to_uint(pstPack1->Version_From);
	iVer2 = version_to_uint(pstPack2->Version_From);

	if (iVer1 > iVer2)
	{
		return 1;
	}
	else if (iVer1 == iVer2)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int UpJumpCmp(const void *p1, const void *p2)
{
	unsigned int iVer1;
	unsigned int iVer2;
	UPJUMPPACK *pstPack1 = (UPJUMPPACK *)p1;
	UPJUMPPACK *pstPack2 = (UPJUMPPACK *)p2;

	iVer1 = version_to_uint(pstPack1->Version_From);
	iVer2 = version_to_uint(pstPack2->Version_From);

	if (iVer1 > iVer2)
	{
		return 1;
	}
	else if (iVer1 == iVer2)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int get_new_version(VERSION_SVRCONF *pstConf, unsigned int *piVersion)
{
	unsigned int iVersionNew = 0;
	unsigned int iTmp = 0;
	int i = 0;
		
	for (i=0; i<(int)(pstConf->Count); i++)
	{
		iTmp = version_to_uint(pstConf->UpPack[i].Version_To);
		if (iTmp == (unsigned int)(-1))
		{
			printf("version %s Version_To err\n", pstConf->UpPack[i].Version_Name);
			return -1;
		}

		if (iTmp > iVersionNew)
		{
			iVersionNew = iTmp;
		}
	}

	*piVersion = iVersionNew;

	return 0;
}

int get_min_version(VERSION_SVRCONF *pstConf, unsigned int *piVersion)
{
	unsigned int iMinVer = 0xffffffff;
	unsigned int iTmp;
	int i;

	for (i=0; i<(int)pstConf->Count; i++)
	{
		iTmp = version_to_uint(pstConf->UpPack[0].Version_From);
		if (iTmp == (unsigned int)(-1))
		{
			printf("version %s Version_From err\n", pstConf->UpPack[i].Version_Name);
			return -1;
		}

		if (iTmp < iMinVer)
		{
			iMinVer = iTmp;
		}
	}

	for (i=0; i<(int)pstConf->JumpCount; i++)
	{
		iTmp = version_to_uint(pstConf->JumpPack[i].Version_From);
		if (iTmp == (unsigned int)(-1))
		{
			printf("version %s Version_From err\n", pstConf->JumpPack[i].Version_Name);
			return -1;
		}

		if (iTmp < iMinVer)
		{
			iMinVer = iTmp;
		}

		//顺便检测一下
		iTmp = version_to_uint(pstConf->JumpPack[i].Version_Jump);
		if (iTmp == (unsigned int)(-1))
		{
			printf("version %s Version_Jump err\n", pstConf->JumpPack[i].Version_Name);
			return -1;
		}

		iTmp = version_to_uint(pstConf->JumpPack[i].JumpMaxVer);
		if (iTmp == (unsigned int)(-1))
		{
			printf("version %s JumpMaxVer err\n", pstConf->JumpPack[i].Version_Name);
			return -1;
		}
	}

	if (iMinVer == 0xffffffff)
	{
		*piVersion = 0;
	}
	else
	{
		*piVersion = iMinVer;
	}

	return 0;
}

int create_version_xml(TAPPCTX *pstAppCtx, VERSION_SVRENV *pstEnv)
{
	VERSION_SVRCONF *pstConf;
	VERSION_XML stVersion_xml;
	VERSION_SVRCONF stTmpConf;
	LPTDRMETA pstXmlMeta;
	LPTDRMETALIB pstLib;
	TDRDATA stData;

	pstConf = pstEnv->pstConf;

	if (pstConf->XmlPath[0] == 0)
	{
		return 0;
	}

	memcpy(&stTmpConf, pstConf, sizeof(stTmpConf));
	memset(&stVersion_xml, 0, sizeof(stVersion_xml));

	stVersion_xml.NewVersion = pstEnv->iVersionNew;
	stVersion_xml.MinVersion = pstEnv->iVersionMin;

	if (stTmpConf.Count > 1)
	{
		qsort(stTmpConf.UpPack, stTmpConf.Count, sizeof(stTmpConf.UpPack[0]), UpPackCmp);
	}

	if (stTmpConf.JumpCount > 1)
	{
		qsort(stTmpConf.JumpPack, stTmpConf.JumpCount, sizeof(stTmpConf.JumpPack[0]), UpJumpCmp);
	}

	if (stTmpConf.Count > MAX_XML_VERSION_NUM)
	{
		stVersion_xml.Count = MAX_XML_VERSION_NUM;
		memcpy(&stVersion_xml.UpPack[0], 
			&stTmpConf.UpPack[stTmpConf.Count-MAX_XML_VERSION_NUM],
			stVersion_xml.Count*(sizeof(stVersion_xml.UpPack[0])));
	}
	else
	{
		stVersion_xml.Count = stTmpConf.Count;
		memcpy(&stVersion_xml.UpPack[0], 
			&stTmpConf.UpPack[0],
			stVersion_xml.Count*(sizeof(stVersion_xml.UpPack[0])));
	}

	if (stTmpConf.JumpCount > MAX_XML_JUMP_NUM)
	{
		stVersion_xml.JumpCount = MAX_XML_JUMP_NUM;
		memcpy(&stVersion_xml.JumpPack[0], 
			&stTmpConf.JumpPack[stTmpConf.JumpCount-MAX_XML_JUMP_NUM],
			stVersion_xml.JumpCount*(sizeof(stVersion_xml.JumpPack[0])));
	}
	else
	{
		stVersion_xml.JumpCount = stTmpConf.JumpCount;
		memcpy(&stVersion_xml.JumpPack[0], 
			&stTmpConf.JumpPack[0],
			stVersion_xml.JumpCount*(sizeof(stVersion_xml.JumpPack[0])));
	}

	pstLib = (LPTDRMETALIB)g_szMetalib_proto_cs;
	pstXmlMeta = tdr_get_meta_by_name(pstLib, "version_xml");
	if (NULL == pstXmlMeta)
	{
		return -1;
	}

	stData.iBuff = sizeof(stVersion_xml);
	stData.pszBuff = (char *)&stVersion_xml;
	tdr_output_file(pstXmlMeta, pstConf->XmlPath, &stData, 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	
	return 0;
}

int version_svr_init(TAPPCTX *pstAppCtx, VERSION_SVRENV *pstEnv)
{
	unsigned char ucEntity; 

	pstEnv->iVersionNew = 0;
	if (NULL == pstAppCtx->stConfData.pszBuff || 0 == pstAppCtx->stConfData.iMeta 
		|| NULL == pstAppCtx->pszConfFile )
	{
		return -1;
	}
	pstEnv->pstConf = (VERSION_SVRCONF *)pstAppCtx->stConfData.pszBuff;

	pstEnv->pstRunStat = (VERSION_SVRRUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
	if (NULL == pstEnv->pstRunStat)
	{
		pstEnv->pstRunStat = &gs_stStat;
	}

	if (0 > tapp_get_category(NULL, (void *)(&pstEnv->pstLogCat)))	
	{		
		printf("tapp_get_category run fail\n");
		return -1;	
	}	

	if (0 > net_han_create_c(g_szMetalib_proto_cs , &pstEnv->pstCSHan))
	{
		return -1;
	}

	if (0 > tconnapi_initialize(pstAppCtx->pszGCIMKey, pstAppCtx->iBusinessID))
	{
		printf("tbus gcim key %s init fail\n", pstAppCtx->pszGCIMKey) ;
		return -1;
	}
	
	if (0 > tconnapi_create(pstAppCtx->iId, &pstEnv->iTconnapiHandle))
	{
		printf("tbus id %s init fail\n", pstAppCtx->pszId);
		return -1;
	}


	GET_FUNC_ENTITY(ucEntity, pstAppCtx->iId);

	switch(ucEntity)
	{
	case FUNC_VERSION_SVR:
		GET_CLUSTER_ENTITY_ID(pstEnv->iConnID, pstAppCtx->iId, FUNC_VERSION_CONN);
		break;
	case FUNC_VERSION_BAK:
		GET_CLUSTER_ENTITY_ID(pstEnv->iConnID, pstAppCtx->iId, FUNC_VERSION_BAK_CONN);
		break;	
	case FUNC_VERSION_SLAVE:
		GET_CLUSTER_ENTITY_ID(pstEnv->iConnID, pstAppCtx->iId, FUNC_VERSION_SLAVE_CONN);
		break;
	case FUNC_VERSION_SLAVE1:
		GET_CLUSTER_ENTITY_ID(pstEnv->iConnID, pstAppCtx->iId, FUNC_VERSION_SLAVE1_CONN);
		break;	
	default:
		return -1;
		break;
	}

	if (0 > get_new_version(pstEnv->pstConf, &pstEnv->iVersionNew))
	{
		return -1;
	}
	
	if (0 > get_min_version(pstEnv->pstConf, &pstEnv->iVersionMin))
	{
		return -1;
	}
	
	create_version_xml(pstAppCtx, pstEnv);
	
	g_iNow = 0;
	
	printf("version_svr start\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "version_svr start");

	tlog_info_dr(pstEnv->pstLogCat, 0, 0, (LPTDRMETA)pstAppCtx->stConfData.iMeta, pstAppCtx->stConfData.pszBuff, 
				pstAppCtx->stConfData.iLen, 0);	

	return 0;
}


int version_svr_discon(VERSION_SVRENV *pstEnv, TFRAMEHEAD *pstFrameHead)
{
	TDRDATA stNet;

	stNet.pszBuff = &pstEnv->szSendBuffer[0];	
	stNet.iBuff = 0;
	
	pstFrameHead->Cmd = TFRAMEHEAD_CMD_STOP;
	pstFrameHead->TimeStampType = 0;
	pstFrameHead->ID = -1;
	
	return tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iConnID, stNet.pszBuff, stNet.iBuff, pstFrameHead);
}

/*int version_svr_updata_info(VERSION_SVRENV *pstEnv, CSPKG *pstCSPkgReq,TFRAMEHEAD *pstFrameHead, int iCltProtoVer)
{
	TDRDATA stNet;
	int i;
	VERSION_SVRCONF *pstConf = pstEnv->pstConf;
	unsigned int iVersion;
	CSPKG stPkg;
	CSVERSIONSVR *pstVersionSvr = &stPkg.Body.VersionSvr;
	unsigned int iLast = pstCSPkgReq->Body.VersionClt.VersionNum;

	pstVersionSvr->IsUp = -1;
	
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = &pstEnv->szSendBuffer[0];

	iVersion = version_to_uint(pstConf->MinVersion_Num);
	if (iVersion <= 0)
	{
		return -1;
	}

	//  低于最低版本
	if (iLast < iVersion)
	{
		pstVersionSvr->IsUp = VERSION_OLD;
	}

	if (pstVersionSvr->IsUp != VERSION_OLD)
	{
		for (i=0; i<(int)(pstConf->Count); i++)
		{
			iVersion = version_to_uint(pstConf->UpPack[i].Version_From);
			if (iVersion <= 0)
			{
				continue;
			}

			// 要更新的版本
			if(iVersion == iLast)
			{
				pstVersionSvr->IsUp = VERSION_UPDATA;
				pstVersionSvr->VersionFrom = iVersion;
				pstVersionSvr->VersionTo = version_to_uint(pstConf->UpPack[i].Version_To);
				pstVersionSvr->UpSize = pstConf->UpPack[i].Version_Size;
				STRNCPY(pstVersionSvr->UpName, pstConf->UpPack[i].Version_Name, sizeof(pstVersionSvr->UpName));
				STRNCPY(pstVersionSvr->MD5, pstConf->UpPack[i].MD5, sizeof(pstVersionSvr->MD5));
				STRNCPY(pstVersionSvr->UpAddr, pstConf->UpPack[i].UpAddr, sizeof(pstVersionSvr->UpAddr));
				pstVersionSvr->VersionNew = pstEnv->iVersionNew;//　把最新的版本下发下
				break;
			}
		}

		if (i >= (int)(pstConf->Count))
		{
			pstVersionSvr->IsUp = VERSION_NOT_UPDATA;
		}
	}
	
	stPkg.Head.Cmd = VERSION_SVR;
	stPkg.Head.Magic = PROTOCOL_MAGIC;
	stPkg.Head.Ver = TDR_METALIB_PROTO_CS_VERSION;
	
	if (0 > net_cs_pack(pstEnv->pstCSHan, &stNet, &stPkg, iCltProtoVer))
	{
		return -2;
	}

	pstFrameHead->Cmd = TFRAMEHEAD_CMD_STOP;
	pstFrameHead->TimeStampType = 0;
	pstFrameHead->ID = -1;
	
	return tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iConnID, stNet.pszBuff, stNet.iBuff, pstFrameHead);
}*/


int version_svr_to_clt(VERSION_SVRENV *pstEnv, CSPKG *pstCSPkgReq,TFRAMEHEAD *pstFrameHead, int iCltProtoVer)
{
	TDRDATA stNet;
	int i;
	VERSION_SVRCONF *pstConf = pstEnv->pstConf;
	unsigned int iVersion;
	CSPKG stPkg;
	CSVERSIONSVR *pstVersionSvr = &stPkg.Body.VersionSvr;
	unsigned int iLast = pstCSPkgReq->Body.VersionClt.VersionNum;
	char cJumpFind = 0;

	memset (pstVersionSvr, 0, sizeof(*pstVersionSvr));
	pstVersionSvr->IsUp = -1;
	pstVersionSvr->VersionNew = pstEnv->iVersionNew;
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = &pstEnv->szSendBuffer[0];
	
	if (0 == pstEnv->iVersionMin || 0 == pstEnv->iVersionNew)
	{
		pstVersionSvr->IsUp = VERSION_NOT_UPDATA;
	}
	else if (iLast < pstEnv->iVersionMin)
	{
		pstVersionSvr->IsUp = VERSION_OLD;
	}
	else
	{
		// 优先查找跳包  
		for (i=0; i<(int)(pstConf->JumpCount); i++)
		{
			if (	iLast >= version_to_uint(pstConf->JumpPack[i].Version_From) &&
				iLast <= version_to_uint(pstConf->JumpPack[i].JumpMaxVer))
			{
				cJumpFind = 1;

				pstVersionSvr->IsUp = VERSION_UPDATA;
				pstVersionSvr->VersionFrom = version_to_uint(pstConf->JumpPack[i].Version_From);
				pstVersionSvr->VersionTo = version_to_uint(pstConf->JumpPack[i].Version_Jump);
				pstVersionSvr->UpSize = pstConf->JumpPack[i].Version_Size;
				STRNCPY(pstVersionSvr->UpName, pstConf->JumpPack[i].Version_Name, sizeof(pstVersionSvr->UpName));
				STRNCPY(pstVersionSvr->MD5, pstConf->JumpPack[i].MD5, sizeof(pstVersionSvr->MD5));
				STRNCPY(pstVersionSvr->UpAddr, pstConf->JumpPack[i].UpAddr, sizeof(pstVersionSvr->UpAddr));
				break;
			}
		}

		// 没有找到跳包  继续找单个的包
		if (0 == cJumpFind)
		{
			for (i=0; i<(int)(pstConf->Count); i++)
			{
				iVersion = version_to_uint(pstConf->UpPack[i].Version_From);
				if (iVersion == (unsigned int)(-1))
				{
					continue;
				}

				// 要更新的版本
				if(iVersion == iLast)
				{
					pstVersionSvr->IsUp = VERSION_UPDATA;
					pstVersionSvr->VersionFrom = iVersion;
					pstVersionSvr->VersionTo = version_to_uint(pstConf->UpPack[i].Version_To);
					pstVersionSvr->UpSize = pstConf->UpPack[i].Version_Size;
					STRNCPY(pstVersionSvr->UpName, pstConf->UpPack[i].Version_Name, sizeof(pstVersionSvr->UpName));
					STRNCPY(pstVersionSvr->MD5, pstConf->UpPack[i].MD5, sizeof(pstVersionSvr->MD5));
					STRNCPY(pstVersionSvr->UpAddr, pstConf->UpPack[i].UpAddr, sizeof(pstVersionSvr->UpAddr));
					break;
				}
			}

			if (i >= (int)(pstConf->Count))
			{
				pstVersionSvr->IsUp = VERSION_NOT_UPDATA;
			}	
		}
	}
	
	stPkg.Head.Cmd = VERSION_SVR;
	stPkg.Head.Magic = PROTOCOL_MAGIC;
	stPkg.Head.Ver = TDR_METALIB_PROTO_CS_VERSION;
	
	if (0 > net_cs_pack(pstEnv->pstCSHan, &stNet, &stPkg, iCltProtoVer))
	{
		return -2;
	}

	pstFrameHead->Cmd = TFRAMEHEAD_CMD_STOP;
	pstFrameHead->TimeStampType = 0;
	pstFrameHead->ID = -1;

	return tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iConnID, stNet.pszBuff, stNet.iBuff, pstFrameHead);

	return 0;
}

int version_svr_msg_send(VERSION_SVRENV *pstEnv, CSPKG *pstCSPkgReq,TFRAMEHEAD *pstFrameHead, int iCltProtoVer)
{
	if (pstCSPkgReq->Head.Cmd == VERSION_CLT)
	{
		//return version_svr_updata_info(pstEnv, pstCSPkgReq,pstFrameHead, iCltProtoVer);
		return version_svr_to_clt(pstEnv, pstCSPkgReq,pstFrameHead, iCltProtoVer);
	}
	else
	{
		return -1;
	}
}

int version_svr_msg(TAPPCTX* pstAppCtx, VERSION_SVRENV* pstEnv, char *pszMsg, int iMsgLen, TFRAMEHEAD *pstFrameHead)
{
	CSPKG stCsPkg;
	int iRet;
	int iCltProtoVer;
	
	if (TFRAMEHEAD_CMD_START != pstFrameHead->Cmd)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "pstFrameHead->Cmd=%d", pstFrameHead->Cmd);
		pstEnv->pstRunStat->BadCltMsg++;

		version_svr_discon(pstEnv, pstFrameHead);
		return -1;
	}

	
	if (0 > net_cs_unpack(pstEnv->pstCSHan, pszMsg, iMsgLen, &stCsPkg, 0))
	{
		pstEnv->pstRunStat->BadCltMsg++;
		tlog_error(pstEnv->pstLogCat, 0, 0, "net_cs_unpack fail");

		version_svr_discon(pstEnv, pstFrameHead);
		return -1;
	}

	if (stCsPkg.Head.Cmd != VERSION_CLT)
	{
		version_svr_discon(pstEnv, pstFrameHead);
		return -1;
	}

	iCltProtoVer = stCsPkg.Head.Ver;

	pstEnv->pstRunStat->RecvCltMsg++;
	
	iRet = version_svr_msg_send(pstEnv, &stCsPkg, pstFrameHead, iCltProtoVer);
	if (0 > iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "dir_cltmsg_send fail %d", iRet);
		pstEnv->pstRunStat->SendCltMsgFail++;
	}

	return iRet;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}


/*
int get_min_version(VERSION_SVRENV *pstEnv)
{
	VERSION_SVRCONF *pstConf = pstEnv->pstConf;
	unsigned int iMinVer;
	unsigned int iVer;
	int iPos = 0;
	
	int i;

	if (pstConf->Count <= 0)
	{
		return -1;
	}

	iMinVer = version_to_uint(pstConf->UpPack[0].Version_From);
	for (i=1; i<(int)(pstConf->Count); i++)
	{
		iVer = version_to_uint(pstConf->UpPack[i].Version_From);
		if (iMinVer > iVer)
		{
			iMinVer = iVer;
			iPos = i;
		}
	}

	return iPos;
}*/

/*int write_to_conf(VERSION_SVRENV *pstEnv, char *pszFileName)
{
	char *pszTmp,*pszTmp1;
	int iLen;
	char szPathName[256];
	char szMd5[24];
	char szFrom[32];
	struct stat sbuf;
	UPDATAPACK *pstUp;
	int iPos,iFlag;
	VERSION_SVRCONF *pstConf = pstEnv->pstConf;
	unsigned int iVersionNew = 0;
	
	memset(&sbuf, 0x00, sizeof(sbuf));
	snprintf(szPathName, sizeof(szPathName),"%s/%s", pstConf->VersionDataPath, pszFileName);

	if (stat(szPathName, &sbuf) == -1) 
		return -1;

	pszTmp = strstr(pszFileName, "_");
	if (!pszTmp)
		return -1;

	iLen = pszTmp - pszFileName;
	memcpy(szFrom, pszFileName, iLen);
	szFrom[iLen] = 0;

	if (pstConf->Count >= MAX_UP_VERSION_NUM)
	{
		iPos = get_min_version(pstEnv);
		if (iPos < 0)
		{
			return 0;
		}
		if (version_to_uint(pstConf->UpPack[iPos].Version_From) < version_to_uint(szFrom))
		{
			pstUp = 	&pstConf->UpPack[iPos];
		}
		else
		{
			return 0;
		}

		// 覆盖
		iFlag = 0;
	}
	else
	{
		pstUp = &pstConf->UpPack[pstConf->Count];
		// 增加
		iFlag = 1;
	};

	STRNCPY(pstUp->Version_From, szFrom, sizeof(pstUp->Version_From));
	pszTmp ++;
	pszTmp1 = pszTmp;

	pszTmp = strstr(pszTmp, "exe");
	if (!pszTmp)
	{
		return -1;
	}

	pszTmp --;

	iLen = pszTmp - pszTmp1;
	memcpy(pstUp->Version_To, pszTmp1, iLen);
	pstUp->Version_To[iLen] = 0;
	pstUp->Version_Size = sbuf.st_size;
	STRNCPY(pstUp->UpAddr, pszFileName, sizeof(pstUp->UpAddr));
	Md5File (szMd5, szPathName);
	Md5HexString(szMd5, pstUp->MD5);

	iVersionNew = version_to_uint(pstUp->Version_To);
	if( iVersionNew > pstEnv->iVersionNew )
	{
		pstEnv->iVersionNew  = iVersionNew;
	}
	
	if (iFlag)
	{
		pstConf->Count++;
	}
	return 0;
}*/

/*int load_data_path(VERSION_SVRENV *pstEnv)
{
	DIR *dp = NULL;
	struct dirent *dirp;
	VERSION_SVRCONF *pstConf = pstEnv->pstConf;
	char *pszExe;
	int iPos;
	struct stat sbuf;
	
	if (pstConf->VersionDataPath[0] == 0)
	{
		return -1;
	}

	if ((dp = opendir(pstConf->VersionDataPath)) == NULL)
	{
		return -1;
	}

	// 目录是否发生变化
	if (stat(pstConf->VersionDataPath,&sbuf) < 0)
	{
		goto closedir;
	}
	if (sbuf.st_mtime == pstEnv->tLastModify)
	{
		goto closedir;
	}
	pstEnv->tLastModify = sbuf.st_mtime;
	pstConf->Count = 0;
	while((dirp = readdir(dp)) != NULL)
	{
		if ((pszExe = strstr(dirp->d_name, "exe")) != NULL)
		{
			if (pstEnv->pstConf->Count >= MAX_UP_VERSION_NUM)
			{
				// 清除最小版本
				iPos = get_min_version(pstEnv);
				pstConf->Count --;
				if (iPos != (int)pstConf->Count)
				{
					memcpy(&pstConf->UpPack[iPos], &pstConf->UpPack[pstConf->Count], sizeof(UPDATAPACK));
				}
			}
			
			write_to_conf(pstEnv, dirp->d_name);
		}
	}

	// 最小版本
	iPos = get_min_version(pstEnv);
	if (iPos >= 0)
	{
		STRNCPY(pstConf->MinVersion_Num, pstConf->UpPack[iPos].Version_From, sizeof(pstConf->MinVersion_Num));
	}

closedir:
	if (dp)
	{
		closedir(dp);
	}
	return 0;
}*/

int version_svr_proc(TAPPCTX *pstAppCtx, VERSION_SVRENV *pstEnv)
{
	int iBusy=0;
	int iLoopNum = 0;
	TBUSADDR iMsgDst ;
	TBUSADDR iMsgSrc ;
	int iMsgLen;
	int iRet = 0;
	char *pszMsg;
	TFRAMEHEAD stFrameHead;
	int iFrameHeadLen ;

	//先收client的请求
	while (LOOP_NUM > iLoopNum)
	{
		iLoopNum++;

		
		
		iMsgDst = pstAppCtx->iId;
		iMsgSrc = TBUS_ADDR_ALL;	// 接收发到tbus的所有来源的数据
		iMsgLen = sizeof(pstEnv->szRecvBuffer);
		pszMsg = &pstEnv->szRecvBuffer[0];
		if ( TBUS_SUCCESS != tbus_recv(pstAppCtx->iBus, &iMsgSrc, &iMsgDst, 
			(void *)pszMsg, (size_t *)&iMsgLen, 0) )
		{
			//no msg
			break;
		}
		iBusy = 1;

		iFrameHeadLen = sizeof(stFrameHead);
		iRet = tconnapi_decode(pszMsg, iMsgLen, &stFrameHead, &iFrameHeadLen);
		if (0 != iRet)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tconnapi_decode failed, iRet0x%x, Msglen:%d", iRet, iMsgLen);
			continue;
		}

		pszMsg += iFrameHeadLen;
		iMsgLen -= iFrameHeadLen;
		iRet = version_svr_msg(pstAppCtx, pstEnv, pszMsg, iMsgLen, &stFrameHead);
	}

	if (iBusy)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int version_svr_reload(TAPPCTX *pstAppCtx, VERSION_SVRENV *pstEnv)
{
	unsigned int iVersion;
	VERSION_SVRCONF   *pstPreConf;
	
	pstPreConf = (VERSION_SVRCONF   *)pstAppCtx->stConfPrepareData.pszBuff;

	if (0 > get_new_version(pstPreConf , &iVersion))
	{
		printf("version_svr reload fail\n");
		tlog_info(pstEnv->pstLogCat, 0, 0, "version_svr reload fail");
		return -1;
	}

	if (0 > get_min_version(pstPreConf , &iVersion))
	{
		printf("version_svr reload fail\n");
		tlog_info(pstEnv->pstLogCat, 0, 0, "version_svr reload fail");
		return -1;
	}

	memcpy(pstEnv->pstConf, pstPreConf, sizeof(VERSION_SVRCONF));

	get_new_version(pstEnv->pstConf, &pstEnv->iVersionNew);
	get_min_version(pstEnv->pstConf, &pstEnv->iVersionMin);
	create_version_xml(pstAppCtx, pstEnv);
	
	printf("version_svr reload\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "version_svr reload");

	//load_data_path(pstEnv);

	return 0;
}

int version_svr_fini(TAPPCTX *pstAppCtx, VERSION_SVRENV *pstEnv)
{
	
	printf("version_svr finish\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "version_svr finish");
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int version_svr_tick(TAPPCTX *pstAppCtx, VERSION_SVRENV *pstEnv)
{
	//if (pstAppCtx->stCurr.tv_sec - g_iNow > 10)
	//{
	//	load_data_path(pstEnv);
	//	g_iNow = pstAppCtx->stCurr.tv_sec;
	//}
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
}

int version_svr_stop(TAPPCTX *pstAppCtx, VERSION_SVRENV *pstEnv)
{
	int iReady=0;
	
	iReady = 1;
	if (iReady)
	{
		return -1;
	}
	else
	{
		return 0;
	}
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
}

int main(int argc, char* argv[])
{
	int iRet;
	void* pvArg	=	&gs_stEnv;

	memset(&gs_stAppCtx, 0, sizeof(gs_stAppCtx));
	memset(&gs_stEnv, 0, sizeof(gs_stEnv));

	gs_stAppCtx.argc	=	argc;
	gs_stAppCtx.argv	=	argv;

	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)version_svr_init;
	
	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_version_svr;
	gs_stAppCtx.stConfData.pszMetaName = "version_svrconf";
	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);
		
	iRet	=	tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		printf("Error: app Initialization failed.\n");
		return iRet;
	}

	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)version_svr_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)version_svr_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)version_svr_tick;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)version_svr_reload;
	gs_stAppCtx.pfnStop  =   (PFNTAPPFUNC)version_svr_stop;
	
	iRet	=	tapp_def_mainloop(&gs_stAppCtx, pvArg);

	tapp_def_fini(&gs_stAppCtx, pvArg);
	
	return iRet;
}


