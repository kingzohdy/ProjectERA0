/*
**  @file $RCSfile: client_press.c,v $
**  general description of this module
**  $Id: client_press.c,v 1.9 2013/02/25 09:46:30 tjw Exp $
**  @author $Author: tjw $
**  @date $Date: 2013/02/25 09:46:30 $
**  @version $Revision: 1.9 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "pal/pal.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "cs_net.h"
#include "tlog/tlog.h"
// test9 
int iRecvMsg = 0; 
NETCLTHANDLE *pstNetCltHan;
LPNETCLTAUTHHANDLE pstNetCltAuthHan;
TLOGCTX stTlogCtx;
TLOGCONF stTlogConf;
TLOGCATEGORYINST* pstLogCat;
  
int recv_svr_msg(CSPKG *pstPkg, int iTimeout)
{
	int iRet;

	iRet = net_clt_recv(pstNetCltHan, pstPkg, iTimeout);

	return iRet;
}
      
int init_tlog_conf()
{
	TLOGCATEGORY *pstCat;

	memset(&stTlogConf, 0, sizeof(stTlogConf));
	stTlogConf.iCount	=	1;
	stTlogConf.iPriorityHigh  =	TLOG_PRIORITY_NULL;
	stTlogConf.iPriorityLow  =  TLOG_PRIORITY_NOTSET;
	stTlogConf.iDelayInit	=	0;
	stTlogConf.iSuppressError	=	0;

	pstCat	=	stTlogConf.astCategoryList;
	STRNCPY(pstCat->szName, "texttrace", sizeof(pstCat->szName));
	STRNCPY(pstCat->szFormat, "[%d.%u][(%f:%l) (%F)] %m%n", sizeof(pstCat->szFormat));
	pstCat->iPriorityHigh  =	 TLOG_PRIORITY_NULL;
	pstCat->iPriorityLow  =  TLOG_PRIORITY_TRACE;
	pstCat->iLevelDispatch =  0;
	pstCat->iMustSucc	=	 1;
	pstCat->iMaxMsgSize	=	 0;
	pstCat->stDevice.iType	=	TLOG_DEV_FILE;
	pstCat->stDevice.stDevice.stFile.szPattern[sizeof(pstCat->stDevice.stDevice.stFile.szPattern)-1] = 0;
	snprintf(pstCat->stDevice.stDevice.stFile.szPattern, sizeof(pstCat->stDevice.stDevice.stFile.szPattern)-1, "%s.log", "client");
	pstCat->stDevice.stDevice.stFile.iSyncTime	=	0;
	pstCat->stDevice.stDevice.stFile.iPrecision =	1;
	pstCat->stDevice.stDevice.stFile.iSizeLimit =	100*1024*1024;
	pstCat->stDevice.stDevice.stFile.iMaxRotate =	2;

	return tlog_init(&stTlogCtx, &stTlogConf);
}

int main ( int argc, char ** argv )
{
	CSPKG stReq;
	CSPKG stRes;
	int iRet = 0;
	tdr_ulonglong ullRoleID = 0;
	int iUin;
	char szRoleName[32];
	char szRolePasswd[32];
	char szActiveCode[256];
	int iLastVersion;
	char szIP[48];
	unsigned short unPort;
	char szTdrPath[128];
	int i;
	char szWorldName[32];
	CSDIRWORLDLIST *pstWorldList;
	char szDirIp[32];
	int   iDirPort = 0;
	char szCertID[128];
	char szRealName[128];
	char eMail[128];
	char Mobile[128];

	memset(szDirIp,0,sizeof(szDirIp));
	memset(szWorldName,0,sizeof(szWorldName));
	memset(szRoleName,0,sizeof(szRoleName));
	memset(szTdrPath,0,sizeof(szTdrPath));
	memset(szRolePasswd,0,sizeof(szRolePasswd));
	memset(szActiveCode,0,sizeof(szActiveCode));
	memset(szCertID,0,sizeof(szCertID));
	memset(szRealName,0,sizeof(szRealName));
	memset(eMail,0,sizeof(eMail));
	memset(Mobile,0,sizeof(Mobile));
	

	if (argc < 11 )
	{
		printf("usage: ./client_press dir_ip dir_port world_id user_name passwd active_code CertID RealName eMail Mobile\n");
		printf("ex: ./client_press 192.168.0.77 6721 world_8 ouyang 11 testactivecode 11111 jiangping ssd@163.com 13266742752\n");
		return -1;
	}

	STRNCPY(szDirIp, argv[1], sizeof(szDirIp));
	iDirPort =  atoi(argv[2]);
	STRNCPY(szWorldName, argv[3], sizeof(szWorldName));
	STRNCPY(szRoleName, argv[4],sizeof(szRoleName));
	STRNCPY(szRolePasswd, argv[5],sizeof(szRolePasswd));
	STRNCPY(szActiveCode, argv[6],sizeof(szActiveCode));
	STRNCPY(szCertID, argv[7],sizeof(szCertID));
	STRNCPY(szRealName, argv[8],sizeof(szRealName));
	STRNCPY(eMail, argv[9],sizeof(eMail));
	STRNCPY(Mobile, argv[10],sizeof(Mobile));
	
	STRNCPY(szTdrPath, "../comm/proto_cs.meta", sizeof(szTdrPath));
     
	if (0 > net_clt_han_create(szTdrPath, szDirIp, iDirPort, &pstNetCltHan))

	{
		printf("error! net_clt_han_create fail\n");
		return -1;
	}

	stReq.Head.Magic = PROTOCOL_MAGIC;
	stReq.Head.Cmd = DIR_REQ;
	stReq.Body.DirReq.LastVersion = 0;
	if (0 > net_clt_send(pstNetCltHan, &stReq, 10))
	{
		printf("error! net_clt_send fail\n");
		return -1;
	}
	if (0 >= net_clt_recv(pstNetCltHan, &stRes, 10000))
	{
		printf("net_clt_recv fail\n");
		return -1;
	}

	if (DIR_RES != stRes.Head.Cmd || DIR_SUCC != stRes.Body.DirRes.Result)
	{
		printf("error! dir res fail\n");
		return -1;
	}

	iLastVersion = stRes.Body.DirRes.DirInfo.WorldList.LastVersion;

	pstWorldList = &stRes.Body.DirRes.DirInfo.WorldList;
	if (0 == szWorldName[0])
	{
		STRNCPY(szIP, pstWorldList->ListInfoList[0].IP, sizeof(szIP));
		unPort = pstWorldList->ListInfoList[0].Port;
	}
	else
	{
		szIP[0] = 0;
		for (i=0; i<pstWorldList->WorldNum; i++)
		{
			if (0 == strcasecmp(pstWorldList->ListInfoList[i].WorldName, szWorldName))
			{
				STRNCPY(szIP, pstWorldList->ListInfoList[i].IP, sizeof(szIP));
				unPort = pstWorldList->ListInfoList[i].Port;
				break;
			}
		}
		if (0 == szIP[0])
		{
			printf("error! fail cant find %s world\n", szWorldName);
			return -1;
		}
	}
	/*printf("find %s world,ip:%s,port:%u,AuthIP:%s,AuthPort:%d\n", szWorldName,
		inet_ntoa(*(struct in_addr *)&iIP),ntohs(unPort),
		inet_ntoa(*(struct in_addr *)&pstWorldList->ListInfoList[i].AuthIP),
		ntohs(pstWorldList->ListInfoList[i].AuthPort) );*/

	net_clt_han_destroy(&pstNetCltHan);

	iRet =  net_clt_auth_han_create(szTdrPath, inet_ntoa(*(struct in_addr *)&pstWorldList->ListInfoList[i].AuthIP), 
								NTOHS(pstWorldList->ListInfoList[i].AuthPort),
								szIP, NTOHS(unPort), szRoleName, szRolePasswd, 
								0, &pstNetCltAuthHan,NULL,szCertID,szRealName,eMail,Mobile,0,0);
	if(iRet != AUTH_IFM_NEED_REGIST_CODE_ERROR )
	{
		if( iRet == 0)
		{
			printf("error! account had exsit iRet=%d szRoleName=%s\n",iRet,szRoleName);
		}
		else
		{
			printf("error! net_clt_auth_han_create fail iRet=%d szRoleName=%s\n",iRet,szRoleName);
		}
		return -1;
	}
	if( iRet == AUTH_IFM_NEED_REGIST_CODE_ERROR )
	{
		/*printf("find %s world,ip:%s,port:%u,AuthIP:%s,AuthPort:%d\n", szWorldName,
		inet_ntoa(*(struct in_addr *)&iIP),ntohs(unPort),
		inet_ntoa(*(struct in_addr *)&pstWorldList->ListInfoList[i].AuthIP),
		ntohs(pstWorldList->ListInfoList[i].AuthPort) );*/
		
		iRet= net_clt_auth_han_active(szTdrPath,inet_ntoa(*(struct in_addr *)&pstWorldList->ListInfoList[i].AuthIP), 
								NTOHS(pstWorldList->ListInfoList[i].AuthPort),
								szIP, NTOHS(unPort),
								szActiveCode,&pstNetCltAuthHan,NULL,0,NULL);
		if(iRet !=0 )
		{
			printf("error! net_clt_auth_han_create fail iRet=%d szRoleName=%s\n",iRet,szRoleName);
		}
		else
		{
			printf("OK! Active succ iRet=%d szRoleName=%s\n",iRet,szRoleName);
		}

	}
	if(pstNetCltAuthHan)
	net_clt_han_auth_destroy(&pstNetCltAuthHan);

	return iRet;
}

