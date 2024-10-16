/*
**  @file $RCSfile: client.c,v $
**  general description of this module
**  $Id: client.c,v 1.33 2013/08/28 13:17:40 tjw Exp $
**  @author $Author: tjw $
**  @date $Date: 2013/08/28 13:17:40 $
**  @version $Revision: 1.33 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "pal/pal.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "cs_net.h"
#include "tlog/tlog.h"
#include "comm/tconvert.h"

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
	int iLastVersion;
	char szIP[48];
	unsigned short unPort;
	char szTdrPath[128];
	int i;
	char szWorldName[32];
	CSDIRWORLDLIST *pstWorldList;
	int iCltType;
	int iAuthFlag;
	char szWebSign[WEB_SIGN_HEX_LEN];//web签名字符串

	szWebSign[0] = 0;

	if (0 > init_tlog_conf())
	{
		printf("init tlog conf fail\n");
		return -1;
	}
	pstLogCat = tlog_get_category(&stTlogCtx, "texttrace");
	if (NULL == pstLogCat)
	{
		return -1;
	}

	szWorldName[0] = 0;
	strcpy(szWorldName, "world_5");
	iUin = 111111;
	strcpy(szRoleName, "tte1");
	if (argc == 1)
	{
	}
	else if (argc == 2)
	{
		STRNCPY(szWorldName, argv[1], sizeof(szWorldName));
	}
	else if (argc == 3)
	{
		STRNCPY(szWorldName, argv[1], sizeof(szWorldName));
		//iUin = atoi(argv[2]);
		szWebSign[0] = 0;
		
		STRNCPY(szWebSign,argv[2],
			sizeof(szWebSign));
	}
	else if (argc == 4)
	{
		STRNCPY(szWorldName, argv[1], sizeof(szWorldName));
		iUin = atoi(argv[2]);
		strcpy(szRoleName, argv[3]);
	}
	else
	{
		printf("arg error\n");
		return -1;
	}  
    
	//连接dir 获取目录信息
#ifdef WIN32
	STRNCPY(szTdrPath, "../../../comm/proto_cs.meta", sizeof(szTdrPath));
#else
	STRNCPY(szTdrPath, "../comm/proto_cs.meta", sizeof(szTdrPath));
#endif
       printf("szTdrPath:%s\n",szTdrPath);
	if (0 > net_clt_han_create(szTdrPath, "192.168.0.77", 6721, &pstNetCltHan))

	{
		printf("net_clt_han_create fail\n");
		return -1;
	}

	stReq.Head.Magic = PROTOCOL_MAGIC;
	stReq.Head.Cmd = DIR_REQ;
	stReq.Body.DirReq.LastVersion = 0;
	if (0 > net_clt_send(pstNetCltHan, &stReq, 10))
	{
		printf("net_clt_send fail\n");
		return -1;
	}
	if (0 >= net_clt_recv(pstNetCltHan, &stRes, 2000))
	{
		printf("net_clt_recv fail\n");
		return -1;
	}

	if (DIR_RES != stRes.Head.Cmd || DIR_SUCC != stRes.Body.DirRes.Result)
	{
		printf("dir res fail\n");
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
			}
		}
		if (0 == szIP[0])
		{
			printf("cant find %s world\n", szWorldName);
			return -1;
		}
	}
	printf("find %s world,ip:%s,port:%u\n", szWorldName,szIP,unPort);
	/*
	if (0 >= net_clt_recv(pstNetCltHan, &stRes, 2000))
	{
		printf("net_clt_recv fail, dir close conn\n");
	}
	*/

	net_clt_han_destroy(&pstNetCltHan);

	//再用latestversion 连接一次dir 获取目录信息
	/*
	if (0 > net_clt_han_create(szTdrPath, "192.168.0.77", 6722, &pstNetCltHan))
	{
		printf("net_clt_han_create fail\n");
		return -1;
	}
	stReq.Head.Magic = PROTOCOL_MAGIC;
	stReq.Head.Cmd = DIR_REQ;
	stReq.Body.DirReq.LastVersion = iLastVersion;
	if (0 > net_clt_send(pstNetCltHan, &stReq, 10))
	{
		printf("net_clt_send fail\n");
		return -1;
	}

	if (0 >= net_clt_recv(pstNetCltHan, &stRes, 2000))
	{
		printf("net_clt_recv fail\n");
		return -1;
	}

	if (DIR_RES != stRes.Head.Cmd || DIR_LATEST != stRes.Body.DirRes.Result)
	{
		printf("dir res fail\n");
		return -1;
	}

	if (0 >= net_clt_recv(pstNetCltHan, &stRes, 2000))
	{
		printf("net_clt_recv fail, dir close conn\n");
	}

	net_clt_han_destroy(&pstNetCltHan);
	*/
 
	 
	/*
	if (0 > net_clt_auth_han_create(szTdrPath, "192.168.0.77", 6729,
				szIP, NTOHS(unPort), "kent1919", "hello", 
				0, &pstNetCltAuthHan,NULL,NULL,NULL,NULL,NULL,0,0,NULL))
				*/
	iCltType = CLT_PC;
	iAuthFlag = 0;
	

	if (szWebSign[0])
	{
		iRet = 0;
		iCltType = CLT_MICRO;
		iAuthFlag = AUTH_FLAG_SIGN;
		
	}
	else
	{
		iRet = net_clt_auth_han_create(szTdrPath, "192.168.0.77", 6729,
					szIP, NTOHS(unPort), "0070073", "123456", 
					0, &pstNetCltAuthHan,NULL,NULL,NULL,NULL,NULL,0,AUTH_FLAG_NOX);
		if (iRet != 0)				
		{
			printf("net_clt_auth_han_create fail:%d \n",iRet);
			if (iRet != AUTH_REASON_NEED_PASSWD_CARD_ERROR && iRet != AUTH_IFM_NEED_REGIST_CODE_ERROR)
			{
				return -1;
			}
			else
			{
				NetCltAuthInfo stAuthInfo;
				get_clt_auth_han_info(&stAuthInfo);
				
				LPTDRMETALIB pLibc = meta_file_lib(szTdrPath);

				if (iRet == AUTH_IFM_NEED_REGIST_CODE_ERROR)
				{
				
					iRet = net_clt_auth_han_active_c((char*)pLibc, "192.168.0.77", 6729,
						szIP, NTOHS(unPort), "XLR2y3QpseGAiw", 
						&pstNetCltAuthHan,"",0,NULL);
					printf("net_clt_auth_han_create fail:%d \n",iRet);
				}
				else
				{			
					usleep(10000000);
					iRet = net_clt_auth_han_active(szTdrPath, "192.168.0.77", 6729,
					szIP, NTOHS(unPort), "", 
					&pstNetCltAuthHan,"",0,NULL);

					printf("net_clt_auth_han_create fail:%d \n",iRet);

					iRet = net_clt_auth_han_active(szTdrPath, "192.168.0.77", 6729,
						szIP, NTOHS(unPort), "", 
						&pstNetCltAuthHan,"",0,NULL);

					printf("net_clt_auth_han_create fail:%d \n",iRet);

					iRet = net_clt_auth_han_active(szTdrPath, "192.168.0.77", 6729,
						szIP, NTOHS(unPort), "", 
						&pstNetCltAuthHan,"",0,NULL);

					printf("net_clt_auth_han_create fail:%d \n",iRet);

					iRet = net_clt_auth_han_active(szTdrPath, "192.168.0.77", 6729,
						szIP, NTOHS(unPort), "", 
						&pstNetCltAuthHan,"",0,NULL);

					printf("net_clt_auth_han_create fail:%d \n",iRet);

					iRet = net_clt_auth_han_active(szTdrPath, "192.168.0.77", 6729,
						szIP, NTOHS(unPort), "", 
						&pstNetCltAuthHan,"",0,NULL);

					printf("net_clt_auth_han_create fail:%d \n",iRet);

					iRet = net_clt_auth_han_active(szTdrPath, "192.168.0.77", 6729,
						szIP, NTOHS(unPort), "", 
						&pstNetCltAuthHan,"",0,NULL);

					printf("net_clt_auth_han_create fail:%d \n",iRet);

					
				}
				
				return -1;
			}
		}
	}

	if (iCltType == CLT_MICRO && iAuthFlag == AUTH_FLAG_SIGN)
	{
		

		printf("%s\n",szWebSign);

/*
		net_clt_auth_han_create_web_sign_c(char * pstLibC,"192.168.0.58", 6727,
					inet_ntoa(*(struct in_addr *)&iIP), NTOHS(unPort),
					szWebSign, 
					0, &pstNetCltAuthHan,CLT_MICRO,AUTH_FLAG_PLUGIN_SIGN))

*/
		for (i = 0; i < 3; i++)
		{
			iRet = net_clt_auth_han_create_web_sign(szTdrPath, "192.168.0.77", 6729,
						szIP, NTOHS(unPort),
						szWebSign, 
						0, &pstNetCltAuthHan,CLT_MICRO,AUTH_FLAG_PLUGIN_SIGN);

			printf("net_clt_auth_han_create_web_sign fail:%d \n",iRet);
			if (0 != iRet )
			{
				char szMd5Hex[33];
				int iHexLen = sizeof(szMd5Hex);
				NetCltAuthInfo stNetCltAuthInfo;
				
				szMd5Hex[0] = 0;

				

				get_clt_auth_han_info(&stNetCltAuthInfo);
				
				Bin2Hex((unsigned char *)stNetCltAuthInfo.szMd5, sizeof(stNetCltAuthInfo.szMd5),szMd5Hex, &iHexLen);

				printf("auth_sign_info_assign md5:%s",  
													szMd5Hex);
				
				printf("net_clt_auth_han_create fail2\n");
				
				(void)(HEX_value);
			}
		}
		return -1;
	}

	//帐号登录响应
	if (0 >= net_clt_auth_recv(pstNetCltAuthHan, &stRes, 2000))
	{
		printf("net_clt_recv fail\n");
		iRet = -1;
	}
       printf("Result:%u,SvrProtoVer:%u\n",stRes.Body.AccLoginRes.Result,
	   	 stRes.Body.AccLoginRes.SvrProtoVer);
	if (0 == iRet)
	{
		if (stRes.Head.Cmd != ACC_LOGIN_RES)
		{
			printf("acc login fail\n");
			iRet = -1;
		}
		else if ( stRes.Body.AccLoginRes.Result != 0)
		{
			printf("acc login fail:%d\n",stRes.Body.AccLoginRes.Result);
			iRet = -1;
		}
	}



	if (0 == iRet) //获取大区角色列表
	{
		net_clt_acc_get_role(pstNetCltAuthHan->pstLib, "192.168.0.77", 6729,
			"kent1919", &stRes.Body.AccGetRoleSvr);
	}

	//角色列表
	if (0 == iRet && 0 >= net_clt_auth_recv(pstNetCltAuthHan, &stRes, 2000))
	{
		printf("net_clt_recv fail\n");
		iRet = -1;
	}

	if (0 == iRet)
	{
		if (ROLE_LIST != stRes.Head.Cmd)
		{
			printf("get role list fail\n");
			iRet = -1;
		}
		else if (stRes.Body.RoleList.RoleNum > 0)
		{
			ullRoleID = stRes.Body.RoleList.RoleList[0].RoleID;
		}
		else
		{
			//创建一个角色
			stReq.Head.Magic = PROTOCOL_MAGIC;
			stReq.Head.Cmd = ROLE_NEW_REQ;
			stReq.Body.RoleNewReq.Career = CAREER_MAGICOR;
			stReq.Body.RoleNewReq.Gender = GENDER_WOMAN;
			stReq.Body.RoleNewReq.Head = 1;
			stReq.Body.RoleNewReq.Face = 1;
			stReq.Body.RoleNewReq.Hair = 1;
			strcpy(stReq.Body.RoleNewReq.RoleName, szRoleName);
			printf("创建一个角色 rolename:%s\n",szRoleName);
			if (0 > net_clt_auth_send(pstNetCltAuthHan, &stReq, 10))
			{
				printf("net_clt_send fail\n");
				iRet = -1;
			}
			if (0 == iRet)
			{
				if (0 >= net_clt_auth_recv(pstNetCltAuthHan, &stRes, 2000))
				{
					printf("net_clt_recv fail\n");
					iRet = -1;
				}
			}
			if (0 == iRet)
			{

				if (ROLE_NEW_RES != stRes.Head.Cmd || 0 != stRes.Body.RoleNewRes.Result)
				{
					printf("new one role fail cmd:%u,result:%u\n",stRes.Head.Cmd,
						       stRes.Body.RoleNewRes.Result);
					iRet = -1;
				}
				{
					ullRoleID = stRes.Body.RoleNewRes.RoleInfo.RoleID;
				}
			}
		}
	}


	//角色登录
	if (0 == iRet)
	{
		stReq.Head.Magic = PROTOCOL_MAGIC;
		stReq.Head.Cmd = ROLE_LOGIN_REQ;
		stReq.Body.RoleLoginReq.RoleID = ullRoleID;
		if (0 > net_clt_auth_send(pstNetCltAuthHan, &stReq, 10))
		{
			printf("net_clt_send fail\n");
			iRet = -1;
		}
	}

	if (0 == iRet)
	{
		if (0 >= net_clt_auth_recv(pstNetCltAuthHan, &stRes, 2000))
		{
			printf("net_clt_recv fail\n");
			iRet = -1;
		}

		if (0 == iRet && SELECT_BIRTH_LOCATION_SVR == stRes.Head.Cmd )
		{
			if (0>= stRes.Body.SelectBirthLocS.BirthMapNum)
			{
				iRet = -1;
			}
			else
			{
				stReq.Head.Magic = PROTOCOL_MAGIC;
				stReq.Head.Cmd = SELECT_BIRTH_LOCATION_CLT;
				stReq.Body.SelectBirthLocC.MapID = stRes.Body.SelectBirthLocS.MapIDList[0];
				if (0 > net_clt_auth_send(pstNetCltAuthHan, &stReq, 10))
				{
					printf("net_clt_send fail\n");
					iRet = -1;
				}
				else
				{
					if (0 >= net_clt_auth_recv(pstNetCltAuthHan, &stRes, 2000))
					{
						printf("net_clt_recv fail\n");
						iRet = -1;
					}
				}
			}
		}
	}

	if (-1 == iRet)
	{
		return -1;
	}

	for (i=0; i<10; i++)
	{
		iRet = net_clt_auth_recv(pstNetCltAuthHan, &stRes,0);

		if (0 == iRet)
		{
			break;
		}
		else if (0 > iRet)
		{
			printf("msg err=%d\n", pstNetCltAuthHan->iErr);
			exit(-1);
		}

		iRecvMsg++;

		usleep(10000);
	}


	//角色退出
	if (0 == iRet)
	{
		stReq.Head.Magic = PROTOCOL_MAGIC;
		stReq.Head.Cmd = ROLE_LOGOUT_REQ;
		stReq.Body.RoleLogoutReq.RoleID = ullRoleID;
		if (0 > net_clt_auth_send(pstNetCltAuthHan, &stReq, 10))
		{
			printf("net_clt_send fail\n");
			iRet = -1;
		}
	}

	//帐号退出
	if (0 == iRet)
	{
		stReq.Head.Magic = PROTOCOL_MAGIC;
		stReq.Head.Cmd = ACC_LOGOUT_REQ;
		stReq.Body.AccLogoutReq.Uin = net_clt_auth_get_uin(pstNetCltAuthHan);
		if (0 > net_clt_auth_send(pstNetCltAuthHan, &stReq, 10))
		{
			printf("net_clt_send fail\n");
			iRet = -1;
		}
	}

	net_clt_han_auth_destroy(&pstNetCltAuthHan);

	return iRet;
}

