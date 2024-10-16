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
#include "robot_conf_desc.h"
#include "version.h"
#include "cs_net.h"
#include <math.h>


#define TIMEOUT_TIME 10000
#define MAX_LOGIN_NUM 3000
#define HEARTBEAT_REQ_INTERVAL 30000
#define ADD_MP_INTERVAL 10000
#define SHOW_LOGIN_INFO_INTERVAL 10000
#define ARM_ID 1109901

#define FLAG_GET_GM_NUM 1

typedef unsigned int uint;
typedef unsigned long ulong;

typedef enum tagPlayerAccStatus
{
	PLAYER_STATUS_ACC_NOLOGIN = 0,
	PLAYER_STATUS_ACC_LOGIN_REQ,
	PLAYER_STATUS_ACC_LOGIN,
	PLAYER_STATUS_ROLE_LIST_REQ,
	PLAYER_STATUS_ROLE_LIST,
	PLAYER_STATUS_NEW_ROLE_REQ,
	PLAYER_STATUS_ROLE_LOGIN_REQ,
	PLAYER_STATUS_ROLE_LOGIN,
	PLAYER_STATUS_ROLE_LOGOUT_REQ,	
	PLAYER_STATUS_ROLE_LOGOUT,
	PLAYER_STATUS_ACC_LOGOUT_REQ,
	PLAYER_STATUS_ACC_LOGOUT,
	
}PLAYERACCSTATUS;

typedef enum tagisdead
{
	ROLE_LIVE = 0,
	ROLE_DEAD,
}ISDEAD;

typedef enum tagconnect
{
	CONNECT = 0,
	READYDISCONN,
	DISCONNECT,
}CONNECTSTATUS;

typedef struct tagtestplayer
{
	int iRoleID;
	int iRoleStatus;
	
	NETCLTAUTHHANDLE *pNetCltHan;
	int iRecvFailFlag;									// 接收失败标记
	struct timeval stRecvFailTime;						// 记录接收消息失败的时间

	struct timeval stLoginReqTime;					// 记录登录请求时间
	struct timeval stTimeDiff;							// 收到登录回复时间与登录请求时间的差值
	struct timeval stHeartBeatReqTime;				// 记录本次发送心跳包时间

	CSPOINT stCurrPos;
	int iDir;
	int iMoveSeq;
	int iSkillID;
	int iIsDead;
	int iRoleMemID;
	
	struct timeval stSendMovetime;	
	struct timeval stSendChatTime;
	struct timeval stSendSkillTime;
	struct timeval stAddMpTime;

	int iIsAreadyArm;
	int iLoginOnce;
	int iCareer;
	int iGmCmdNum;
	int iFlag;
	int iAreadyGmNum;

	int iAutoTeamNum;	// 统计自动组队apply的人数
	int iRandArm;
}TESTPLAYER;

struct tagrobotEnv
{
	ROBOTCONF *pstConf;
	ROBOTRUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;
	
	char szIP[48];
	uint unPort;
	int iLoginNum;								// 将要登录的人数
	TESTPLAYER pTestPlayer[MAX_LOGIN_NUM];		// 最大登录人数 3000
	//int iAlreadyNum;								// 已经登录的人数

	char szTdrPath[64];

	CSPKG stReq;
	CSPKG stRes;
	
	int iMoveEndInx;
	int iChatEndInx;
	int iSkillEndInx;

	/*统计信息*/
	//int iMoveNum;
	//int iChatNum;
	//int iSKillNum;

	int iConnNum;
	struct timeval stShowInfoTime;

	int iConnectStatus;

	char szAuthIP[48];                        
    unsigned short AuthPort;      
	int iGmCmdTime;
	LPTDRMETALIB pLibc;
	
};

typedef struct tagrobotEnv		ROBOTENV;
typedef struct tagrobotEnv		*LROBOTENV;


static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_robot[];

ROBOTENV gs_stEnv;
static ROBOTRUN_STATUS gs_stStat;


int get_dir_info(ROBOTENV *pstEnv)
{
	int i;
	NETCLTHANDLE *pstNetCltHan;
	CSPKG stReq;
	CSPKG stRes;
	CSDIRWORLDLIST *pstWorldList ;
	pstEnv->szIP[0] = 0;

#ifdef WIN32
       //STRNCPY(pstEnv->szTdrPath, "../../../../../src/comm/proto_cs.meta", sizeof(pstEnv->szTdrPath));
	STRNCPY(pstEnv->szTdrPath, "../../../../src/comm/proto_cs.meta", sizeof(pstEnv->szTdrPath));
#else
       STRNCPY(pstEnv->szTdrPath, "../../../myworld/src/comm/proto_cs.meta", sizeof(pstEnv->szTdrPath));
#endif
	//STRNCPY(pstEnv->szTdrPath, "./proto_cs.meta", sizeof(pstEnv->szTdrPath));
	if (0 > net_clt_han_create(pstEnv->szTdrPath, pstEnv->pstConf->stRoleLogin.szDirIP, pstEnv->pstConf->stRoleLogin.iDirPort, &pstNetCltHan))
	{
		 STRNCPY(pstEnv->szTdrPath, "./proto_cs.meta", sizeof(pstEnv->szTdrPath));
		if (0 > net_clt_han_create(pstEnv->szTdrPath, pstEnv->pstConf->stRoleLogin.szDirIP, pstEnv->pstConf->stRoleLogin.iDirPort, &pstNetCltHan))
		{
			printf("net_clt_han_create fail\n");
			return -1;
		}
	}

	pstEnv->pLibc = meta_file_lib(pstEnv->szTdrPath);
	if (pstEnv->pLibc  == NULL)
	{
		printf("meta_file_lib is fail!");
		return -1;
	}

	stReq.Head.Magic = PROTOCOL_MAGIC;
	stReq.Head.Cmd = DIR_REQ;
	stReq.Body.DirReq.LastVersion = 0;
	if (0 > net_clt_send(pstNetCltHan, &stReq, 10000))		
	{
		printf("net_clt_send fail\n");
		return -1;
	}

	if (0 >= net_clt_recv(pstNetCltHan, &stRes, 2000))
	{
		printf("net_clt_auth_recv fail\n");
		return -1;
	}

	if (DIR_RES != stRes.Head.Cmd || DIR_SUCC != stRes.Body.DirRes.Result)
	{
		printf("dir res fail\n");
		return -1;
	}

	pstWorldList = &stRes.Body.DirRes.DirInfo.WorldList;
	for(i = 0; i< pstWorldList->WorldNum; i++)
	{
		if(0 == strcmp(pstWorldList->ListInfoList[i].WorldName,pstEnv->pstConf->stRoleLogin.szWorldName))
		{
			//pstEnv->iIP = pstWorldList->ListInfoList[i].IP;
			STRNCPY(pstEnv->szIP, pstWorldList->ListInfoList[i].IP, sizeof(pstEnv->szIP));
			pstEnv->unPort = pstWorldList->ListInfoList[i].Port;

			//pstEnv->AuthIP = pstWorldList->ListInfoList[i].AuthIP;  
			STRNCPY(pstEnv->szAuthIP, pstWorldList->ListInfoList[i].AuthIP, sizeof(pstEnv->szAuthIP));
    			pstEnv->AuthPort = pstWorldList->ListInfoList[i].AuthPort;          
			break;
		}
	}
	
	if(0 == pstEnv->szIP[0])
	{
		printf("find world fail\n");
		return -1;
	}
	
	net_clt_han_destroy(&pstNetCltHan);
	return 0;
}

int robot_init(TAPPCTX *pstAppCtx, ROBOTENV *pstEnv)
{
	int i;
	int iRoleID;
	srand( (unsigned)time( NULL ) );
	memset (pstEnv, 0, sizeof(*pstEnv));

	if (NULL == pstAppCtx->stConfData.pszBuff || 0 == pstAppCtx->stConfData.iMeta 
		|| NULL == pstAppCtx->pszConfFile )
	{
		printf("init configfile fail\n");
		return -1;
	}

	pstEnv->pstConf = (ROBOTCONF *)pstAppCtx->stConfData.pszBuff;
	pstEnv->pstRunStat = (ROBOTRUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
	if (NULL == pstEnv->pstRunStat)
	{
		pstEnv->pstRunStat = &gs_stStat;
	}
	
	if (0 > tapp_get_category(NULL, (void *)(&pstEnv->pstLogCat)))	
	{		
		printf("tapp_get_category run fail\n");
		return -1;	
	}	

	if(get_dir_info(pstEnv) < 0)
	{
		printf("get_dirinfo fail\n");
		return -1;
	}

	pstEnv->iLoginNum = pstEnv->pstConf->stRoleLogin.dwLoginNum;
	iRoleID = pstEnv->pstConf->stRoleLogin.dwLoginID;
	for(i = 0; i< pstEnv->iLoginNum; i++)
	{
		pstEnv->pTestPlayer[i].iRoleID = iRoleID;
		pstEnv->pTestPlayer[i].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
		pstEnv->pTestPlayer[i].iRecvFailFlag = 0;	
		pstEnv->pTestPlayer[i].iSkillID = 0;

		pstEnv->pTestPlayer[i].iLoginOnce = 0;
		pstEnv->pTestPlayer[i].iGmCmdNum = 0;
		pstEnv->pTestPlayer[i].iFlag = 0;
		pstEnv->pTestPlayer[i].iIsAreadyArm = 1;
		pstEnv->pTestPlayer[i].iAreadyGmNum = 0;
		pstEnv->pTestPlayer[i].iAutoTeamNum = 0;
		pstEnv->pTestPlayer[i].iRandArm = 0;
		

		if (pstEnv->pTestPlayer[i].iCareer == 0)
		{
			if (i<pstEnv->pstConf->stRoleLogin.iFsNum)
			{
				pstEnv->pTestPlayer[i].iCareer = CAREER_MAGICOR;
			}
			else if(i<pstEnv->pstConf->stRoleLogin.iFsNum+pstEnv->pstConf->stRoleLogin.iJsNum)
			{
				pstEnv->pTestPlayer[i].iCareer = CAREER_CHURCH;
			}
			else if (i<pstEnv->pstConf->stRoleLogin.iFsNum+pstEnv->pstConf->stRoleLogin.iJsNum+pstEnv->pstConf->stRoleLogin.iQsNum)
			{
				pstEnv->pTestPlayer[i].iCareer = CAREER_GUNER;
			}
			else
			{
				pstEnv->pTestPlayer[i].iCareer = CAREER_WARRIOR;
			}
		}
		
		TV_CLONE(pstEnv->pTestPlayer[i].stHeartBeatReqTime, pstAppCtx->stCurr);
		iRoleID ++;
	}

	pstEnv->iConnectStatus = CONNECT;
	pstEnv->iMoveEndInx = pstEnv->pstConf->stRoleMove.dwMoveNum;
	pstEnv->iChatEndInx = pstEnv->pstConf->stRoleChat.dwChatNum + pstEnv->iMoveEndInx;
	pstEnv->iSkillEndInx = pstEnv->pstConf->stRoleSkill.dwSkillNum + pstEnv->iChatEndInx;

	if( pstEnv->iSkillEndInx > pstEnv->iLoginNum)
	{
		printf("config file is errer\n");
		return -1;
	}

	pstEnv->iConnNum = gs_stAppCtx.iTimer/pstEnv->pstConf->stRoleLogin.dwInterval;
	if(pstEnv->iConnNum <= 0)
	{
		pstEnv->iConnNum = 1;
	}
	TV_CLONE(pstEnv->stShowInfoTime, pstAppCtx->stCurr);
	
	printf("robot start\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "robot start");

	return 0;
}

// 计算两个timeval时间值之差     转换为毫秒；
ulong get_time_diff_to_ms(struct timeval stCurrTime, struct timeval stPreTime)
{
	ulong ulTmpMsTime;
	struct timeval stTmpTvDiff;
	TV_DIFF(stTmpTvDiff, stCurrTime, stPreTime);
	TV_TO_MS(ulTmpMsTime, stTmpTvDiff);
	return ulTmpMsTime;
}

void calculate_login_info(ROBOTENV *pstEnv)
{
	int i;
	int iAlreadyNum = 0;
	int iMoveNum = 0;
	int iChatNum = 0;
	int iSKillNum = 0;
	for(i = 0; i < pstEnv->iLoginNum; i++)
	{
		if(pstEnv->pTestPlayer[i].iRoleStatus == PLAYER_STATUS_ROLE_LOGIN)
		{
			iAlreadyNum ++;
			if(i < pstEnv->iMoveEndInx)
			{
				iMoveNum++;
			}
			else if(i < pstEnv->iChatEndInx)
			{
				iChatNum++;
			}
			else if(i < pstEnv->iSkillEndInx)
			{
				iSKillNum++;
			}
			else
			{
				// other
			}
			
		}
	}
	printf("already login number:%d,move number:%d,chat number:%d,skill number:%d\n",iAlreadyNum,iMoveNum,iChatNum,iSKillNum);
}

int scanf_role(TAPPCTX *pstAppCtx, ROBOTENV *pstEnv)
{
	int i;
	int iRet = -1;
	int iRet1 ;
	int iConnIndex = 0; 	// 连接计数器
	int iDisConnInx = 0;	// 断开连接计数器
	
	for(i = 0; i < pstEnv->iLoginNum; i++)
	{	
		if(pstEnv->iConnectStatus == CONNECT)
		{
			if(iConnIndex >= pstEnv->iConnNum)
			{
				break;
			}
			
			if(pstEnv->pTestPlayer[i].iRoleStatus == PLAYER_STATUS_ACC_NOLOGIN)
			{
				char szAccount[128];
				//char szAuthIp[64];		
				//char *pszZoneConnIp;	

				//memcpy(szAuthIp, inet_ntoa(*(struct in_addr *)&pstEnv->AuthIP), sizeof(szAuthIp));
				//pszZoneConnIp = inet_ntoa(*(struct in_addr *)&pstEnv->iIP);
				
				sprintf(szAccount, "test%d%d", pstEnv->pTestPlayer[i].iRoleID,pstEnv->pstConf->stRoleLogin.dwLoginID);

				iRet1 = net_clt_auth_han_create_c((char*)pstEnv->pLibc, pstEnv->szAuthIP,NTOHS(pstEnv->AuthPort), 
												pstEnv->szIP, NTOHS(pstEnv->unPort),
												szAccount, "hello", 0, &pstEnv->pTestPlayer[i].pNetCltHan,
												NULL,NULL,NULL,NULL,NULL,0,0);
				if (0 != iRet1)
				{
					pstEnv->pTestPlayer[i].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
					printf("net_clt_auth_han_create_c fail\n");
					//free(pstEnv->pLibc);
					return iRet1;
				}
				else
				{
					pstEnv->pTestPlayer[i].pNetCltHan->pstLib = (char*)(pstEnv->pLibc);
					pstEnv->pTestPlayer[i].iRoleStatus = PLAYER_STATUS_ACC_LOGIN_REQ;
					iConnIndex++;
					iRet = 0;
				}

				

	

				/*if (0 > net_clt_auth_han_create(pstEnv->szTdrPath, szAuthIp,NTOHS(pstEnv->AuthPort), pszZoneConnIp, NTOHS(pstEnv->unPort),
					szAccount, "hello", 0, &pstEnv->pTestPlayer[i].pNetCltHan,
					NULL,NULL,NULL,NULL,NULL,0,0))
				{
					pstEnv->pTestPlayer[i].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
				}
				else
				{
					pstEnv->pTestPlayer[i].iRoleStatus = PLAYER_STATUS_ACC_LOGIN_REQ;
					iConnIndex++;
					iRet = 0;
				}*/
			}
		}
		else if(pstEnv->iConnectStatus == READYDISCONN)
		{
			if(pstEnv->pTestPlayer[i].iRoleStatus != PLAYER_STATUS_ACC_NOLOGIN )
			{
				net_clt_han_auth_destroy(&pstEnv->pTestPlayer[i].pNetCltHan);
				pstEnv->pTestPlayer[i].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
				iDisConnInx ++;
				if(iDisConnInx >= pstEnv->iConnNum)
				{
					break;
				}
			}
		}
	}

	if(pstEnv->iConnectStatus == READYDISCONN && iDisConnInx == 0)
	{
		pstEnv->iConnectStatus = DISCONNECT;
	}

	for(i = 0; i < pstEnv->iLoginNum; i++)
	{
		/*
		if(get_time_diff_to_ms(pstAppCtx->stCurr, pstEnv->pTestPlayer[i].stAddMpTime) >= ADD_MP_INTERVAL
			&& pstEnv->pTestPlayer[i].iRoleStatus == PLAYER_STATUS_ROLE_LOGIN)
		{
			pstEnv->stReq.Head.Magic = PROTOCOL_MAGIC;
			pstEnv->stReq.Head.Cmd = CHAT_REQ;

			pstEnv->stReq.Body.ChatReq.LinkLen = 0;
			pstEnv->stReq.Body.ChatReq.Type = CHAT_SAY;
			pstEnv->stReq.Body.ChatReq.ToName[0] = 0;
			sprintf(pstEnv->stReq.Body.ChatReq.Content,"%s","//addattr mp=300 hp=500");
						
			net_clt_auth_send(pstEnv->pTestPlayer[i].pNetCltHan, &pstEnv->stReq, 0);
			TV_CLONE(pstEnv->pTestPlayer[i].stAddMpTime, pstAppCtx->stCurr);
			iRet = 0;
		}*/
		
		if(pstEnv->pTestPlayer[i].iIsDead ==  ROLE_DEAD
			&& pstEnv->pTestPlayer[i].iRoleStatus == PLAYER_STATUS_ROLE_LOGIN)
		{
			pstEnv->stReq.Head.Magic = PROTOCOL_MAGIC;
			pstEnv->stReq.Head.Cmd = REVIVE;
			pstEnv->stReq.Body.Revive.ReviveType = REVIVE_NORMAL;
			if(net_clt_auth_send(pstEnv->pTestPlayer[i].pNetCltHan, &pstEnv->stReq, 10000) < 0)
			{
				pstEnv->pTestPlayer[i].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
				net_clt_han_auth_destroy(&pstEnv->pTestPlayer[i].pNetCltHan);
			}
			else
			{
				iRet = 0;
			}
		}
	}
		
	if(get_time_diff_to_ms(pstAppCtx->stCurr, pstEnv->stShowInfoTime) >= SHOW_LOGIN_INFO_INTERVAL)
	{
		calculate_login_info(pstEnv);
		TV_CLONE(pstEnv->stShowInfoTime, pstAppCtx->stCurr);
	}
	return iRet;
}



int recv_acc_login_res(TAPPCTX *pstAppCtx, ROBOTENV *pstEnv, int index)
{
	int iRet;
	iRet = net_clt_auth_recv(pstEnv->pTestPlayer[index].pNetCltHan, &pstEnv->stRes, 2000);
	if(iRet < 0)			// 收包失败
	{
		pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
		net_clt_han_auth_destroy(&pstEnv->pTestPlayer[index].pNetCltHan);
		return 0;
	}
	else if (0 == iRet)		// 没收到包 需要计算超时
	{
		if(pstEnv->pTestPlayer[index].iRecvFailFlag== 0)
		{
			TV_CLONE(pstEnv->pTestPlayer[index].stRecvFailTime, pstAppCtx->stCurr);
			pstEnv->pTestPlayer[index].iRecvFailFlag = 1;
		}
		else
		{
			if(get_time_diff_to_ms(pstAppCtx->stCurr, pstEnv->pTestPlayer[index].stRecvFailTime)> TIMEOUT_TIME)
			{
				pstEnv->pTestPlayer[index].iRecvFailFlag = 0;
				pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
				net_clt_han_auth_destroy(&pstEnv->pTestPlayer[index].pNetCltHan);
			}
		}
		return -1;
	}
	else					// recv succeed
	{
		if (pstEnv->stRes.Head.Cmd != ACC_LOGIN_RES || pstEnv->stRes.Body.AccLoginRes.Result != 0 )
		{
			pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
			net_clt_han_auth_destroy(&pstEnv->pTestPlayer[index].pNetCltHan);
		}
		else
		{
			pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ACC_LOGIN;
		}
		pstEnv->pTestPlayer[index].iRecvFailFlag = 0;
		return 0;
	}
}

int recv_role_list_res(TAPPCTX *pstAppCtx, ROBOTENV *pstEnv, int index)
{
	int iRet;
	iRet = net_clt_auth_recv(pstEnv->pTestPlayer[index].pNetCltHan, &pstEnv->stRes, 2000); // 接收角色列表
	if(iRet < 0)
	{
		pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
		net_clt_han_auth_destroy(&pstEnv->pTestPlayer[index].pNetCltHan);
		return 0;
	}
	else if (0 == iRet)
	{
		if(pstEnv->pTestPlayer[index].iRecvFailFlag == 0)
		{
			TV_CLONE(pstEnv->pTestPlayer[index].stRecvFailTime, pstAppCtx->stCurr);
			pstEnv->pTestPlayer[index].iRecvFailFlag = 1;
		}
		else
		{	
			if(get_time_diff_to_ms(pstAppCtx->stCurr, pstEnv->pTestPlayer[index].stRecvFailTime) > TIMEOUT_TIME)
			{
				pstEnv->pTestPlayer[index].iRecvFailFlag = 0;
				pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
				net_clt_han_auth_destroy(&pstEnv->pTestPlayer[index].pNetCltHan);
			}
		}
		return -1;
	}
	else
	{
		pstEnv->pTestPlayer[index].iRecvFailFlag = 0;
	
		if (ROLE_LIST != pstEnv->stRes.Head.Cmd)
		{
			pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
			net_clt_han_auth_destroy(&pstEnv->pTestPlayer[index].pNetCltHan);
		}
		else if (pstEnv->stRes.Body.RoleList.RoleNum > 0)
		{
			pstEnv->stReq.Head.Magic = PROTOCOL_MAGIC;
			pstEnv->stReq.Head.Cmd = ROLE_LOGIN_REQ;
			pstEnv->stReq.Body.RoleLoginReq.RoleID = pstEnv->stRes.Body.RoleList.RoleList[0].RoleID;
			if (0 > net_clt_auth_send(pstEnv->pTestPlayer[index].pNetCltHan, &pstEnv->stReq, 10000))
			{
				pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
				net_clt_han_auth_destroy(&pstEnv->pTestPlayer[index].pNetCltHan);
			}
			else
			{
				TV_CLONE(pstEnv->pTestPlayer[index].stLoginReqTime, pstAppCtx->stCurr);
				pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ROLE_LOGIN_REQ;
			}

			pstEnv->pTestPlayer[index].iCareer = pstEnv->stRes.Body.RoleList.RoleList[0].Career;
		}
		else
		{
			pstEnv->stReq.Head.Magic = PROTOCOL_MAGIC;
			pstEnv->stReq.Head.Cmd = ROLE_NEW_REQ;
			pstEnv->stReq.Body.RoleNewReq.Career = pstEnv->pTestPlayer[index].iCareer;
			//pstEnv->stReq.Body.RoleNewReq.Gender = GENDER_WOMAN;
			//pstEnv->stReq.Body.RoleNewReq.Career = (rand()%4) + 1;
			pstEnv->stReq.Body.RoleNewReq.Gender = (rand()%2) + 1;
			pstEnv->stReq.Body.RoleNewReq.Head = 0;
			pstEnv->stReq.Body.RoleNewReq.Face = 0;
			pstEnv->stReq.Body.RoleNewReq.Hair = 0;
			sprintf(pstEnv->stReq.Body.RoleNewReq.RoleName,"%s%d%d","T",
					pstEnv->pTestPlayer[index].iRoleID,pstEnv->pstConf->stRoleLogin.dwLoginID);
			
			if (0 > net_clt_auth_send(pstEnv->pTestPlayer[index].pNetCltHan, &pstEnv->stReq, 10000))
			{
				pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
				net_clt_han_auth_destroy(&pstEnv->pTestPlayer[index].pNetCltHan);
			}
			else
			{
				pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_NEW_ROLE_REQ;
			}
		}
		return 0;
	}
}

int recv_new_role_res(TAPPCTX *pstAppCtx, ROBOTENV *pstEnv, int index)
{
	int iRet;
	iRet = net_clt_auth_recv(pstEnv->pTestPlayer[index].pNetCltHan, &pstEnv->stRes, 2000);// 接收新建角色回复
	if(iRet < 0)
	{
		pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
		net_clt_han_auth_destroy(&pstEnv->pTestPlayer[index].pNetCltHan);
		return 0;
	}
	else if (0 == iRet)
	{
		if(pstEnv->pTestPlayer[index].iRecvFailFlag == 0)
		{
			TV_CLONE(pstEnv->pTestPlayer[index].stRecvFailTime , pstAppCtx->stCurr);
			pstEnv->pTestPlayer[index].iRecvFailFlag = 1;
		}
		else
		{
			if(get_time_diff_to_ms(pstAppCtx->stCurr, pstEnv->pTestPlayer[index].stRecvFailTime) > TIMEOUT_TIME)
			{
				pstEnv->pTestPlayer[index].iRecvFailFlag = 0;
				pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
				net_clt_han_auth_destroy(&pstEnv->pTestPlayer[index].pNetCltHan);
			}
		}
		return -1;
	}
	else
	{
		pstEnv->pTestPlayer[index].iRecvFailFlag = 0;
		if (ROLE_NEW_RES != pstEnv->stRes.Head.Cmd || 0 != pstEnv->stRes.Body.RoleNewRes.Result)
		{
			pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
			net_clt_han_auth_destroy(&pstEnv->pTestPlayer[index].pNetCltHan);
		}
		else
		{
			pstEnv->stReq.Head.Magic = PROTOCOL_MAGIC;
			pstEnv->stReq.Head.Cmd = ROLE_LOGIN_REQ;
			pstEnv->stReq.Body.RoleLoginReq.RoleID = pstEnv->stRes.Body.RoleNewRes.RoleInfo.RoleID;
			if (0 > net_clt_auth_send(pstEnv->pTestPlayer[index].pNetCltHan, &pstEnv->stReq, 10000))
			{
				pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
				net_clt_han_auth_destroy(&pstEnv->pTestPlayer[index].pNetCltHan);
			}
			else
			{
				TV_CLONE(pstEnv->pTestPlayer[index].stLoginReqTime, pstAppCtx->stCurr);
				pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ROLE_LOGIN_REQ;
			}
		}
		return 0;
	}
}

int recv_role_login_res(TAPPCTX *pstAppCtx, ROBOTENV *pstEnv, int index)
{
	int iRet;
	iRet = net_clt_auth_recv(pstEnv->pTestPlayer[index].pNetCltHan, &pstEnv->stRes, 2000);
	if(iRet < 0)
	{
		pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
		net_clt_han_auth_destroy(&pstEnv->pTestPlayer[index].pNetCltHan);
		printf("%d\n", iRet);
		return -1;
	}
	else if (0 == iRet)
	{
		if(pstEnv->pTestPlayer[index].iRecvFailFlag == 0)
		{
			TV_CLONE(pstEnv->pTestPlayer[index].stRecvFailTime , pstAppCtx->stCurr);
			pstEnv->pTestPlayer[index].iRecvFailFlag = 1;
		}
		else
		{
			if(get_time_diff_to_ms(pstAppCtx->stCurr, pstEnv->pTestPlayer[index].stRecvFailTime) > TIMEOUT_TIME)
			{
				pstEnv->pTestPlayer[index].iRecvFailFlag = 0;
				pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
				net_clt_han_auth_destroy(&pstEnv->pTestPlayer[index].pNetCltHan);
			}
		}
		return -1;
	}
	else
	{
		if ( SELECT_BIRTH_LOCATION_SVR == pstEnv->stRes.Head.Cmd )
		{
			if (0>= pstEnv->stRes.Body.SelectBirthLocS.BirthMapNum)
			{
				pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
				net_clt_han_auth_destroy(&pstEnv->pTestPlayer[index].pNetCltHan);

				return 0;
			}
			else
			{
				pstEnv->stReq.Head.Magic = PROTOCOL_MAGIC;
				pstEnv->stReq.Head.Cmd = SELECT_BIRTH_LOCATION_CLT;
				//pstEnv->stReq.Body.SelectBirthLocC.MapID = 2000;
				pstEnv->stReq.Body.SelectBirthLocC.MapID = pstEnv->stRes.Body.SelectBirthLocS.MapIDList[0];
				if (0 > net_clt_auth_send(pstEnv->pTestPlayer[index].pNetCltHan, &pstEnv->stReq, 10000))
				{
					pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
					net_clt_han_auth_destroy(&pstEnv->pTestPlayer[index].pNetCltHan);

					return 0;
				}
			}
		}
		else if(ROLE_LOGIN_RES == pstEnv->stRes.Head.Cmd && 0 == pstEnv->stRes.Body.RoleLoginRes.Result)
		{
			int i;
			int iArmNum;
			int iAttrNum;
			int iHp;
			struct timeval stResTime;
			int iSkillnum = pstEnv->stRes.Body.RoleLoginRes.Enter.RoleDetail.SkillList.SkillNum;
			
			stResTime.tv_sec = pstEnv->stRes.Body.RoleLoginRes.BenchTime.BenchTimeSec;
			stResTime.tv_usec = pstEnv->stRes.Body.RoleLoginRes.BenchTime.BenchTimeUSec;
			TV_DIFF(pstEnv->pTestPlayer[index].stTimeDiff, stResTime, pstEnv->pTestPlayer[index].stLoginReqTime);
			
			pstEnv->pTestPlayer[index].stCurrPos.x = pstEnv->stRes.Body.RoleLoginRes.Enter.EnterMap.x;// 保存登录时坐标
			pstEnv->pTestPlayer[index].stCurrPos.y = pstEnv->stRes.Body.RoleLoginRes.Enter.EnterMap.y;
			pstEnv->pTestPlayer[index].iMoveSeq = pstEnv->stRes.Body.RoleLoginRes.Enter.EnterMap.InitMoveSeq;// 保存MoveSeq	
			
			iAttrNum = pstEnv->stRes.Body.RoleLoginRes.Enter.RoleDetail.PlayerAttrVal.AttrNum;
			for(i=0; i< iAttrNum; i++)
			{
				if(pstEnv->stRes.Body.RoleLoginRes.Enter.RoleDetail.PlayerAttrVal.AttrList[i].AttrID == ATTR_ID_HP)
				{
					iHp = pstEnv->stRes.Body.RoleLoginRes.Enter.RoleDetail.PlayerAttrVal.AttrList[i].Value.HP;
					break;
				}
			}
			pstEnv->pTestPlayer[index].iIsDead = iHp > 0 ? ROLE_LIVE : ROLE_DEAD;
		
			pstEnv->pTestPlayer[index].iRoleMemID = pstEnv->stRes.Body.RoleLoginRes.RoleMemID;
			for(i = 0; i < iSkillnum; i++)
			{
				if(pstEnv->pstConf->stRoleSkill.iSkillID == pstEnv->stRes.Body.RoleLoginRes.Enter.RoleDetail.SkillList.Skills[i].SkillID)
				{
					pstEnv->pTestPlayer[index].iSkillID = pstEnv->pstConf->stRoleSkill.iSkillID;
					break;
				}
			}
			if(pstEnv->pTestPlayer[index].iSkillID != pstEnv->pstConf->stRoleSkill.iSkillID)
			{
				pstEnv->stReq.Head.Magic = PROTOCOL_MAGIC;
				pstEnv->stReq.Head.Cmd = CHAT_REQ;
				pstEnv->stReq.Body.ChatReq.LinkLen = 0;
				pstEnv->stReq.Body.ChatReq.Type = CHAT_SAY;
				pstEnv->stReq.Body.ChatReq.ToName[0] = 0;
				sprintf(pstEnv->stReq.Body.ChatReq.Content,"%s %d %d","//updateskill",pstEnv->pstConf->stRoleSkill.iSkillID,1);
								
				if(0 > net_clt_auth_send(pstEnv->pTestPlayer[index].pNetCltHan, &pstEnv->stReq, 10000))
				{
					pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
					net_clt_han_auth_destroy(&pstEnv->pTestPlayer[index].pNetCltHan);

					return 0;
				}
			}
			
			/*
			      pstEnv->stReq.Head.Magic = PROTOCOL_MAGIC;
				pstEnv->stReq.Head.Cmd = CHAT_REQ;
				pstEnv->stReq.Body.ChatReq.LinkLen = 0;
				pstEnv->stReq.Body.ChatReq.Type = CHAT_SAY;
				pstEnv->stReq.Body.ChatReq.ToName[0] = 0;
				sprintf(pstEnv->stReq.Body.ChatReq.Content,"//pworld 501 0");
				net_clt_auth_send(pstEnv->pTestPlayer[index].pNetCltHan, &pstEnv->stReq, 0);
				*/

			TV_CLONE(pstEnv->pTestPlayer[index].stSendMovetime, pstAppCtx->stCurr);
			TV_CLONE(pstEnv->pTestPlayer[index].stSendChatTime, pstAppCtx->stCurr);
			TV_CLONE(pstEnv->pTestPlayer[index].stSendSkillTime, pstAppCtx->stCurr);
			TV_CLONE(pstEnv->pTestPlayer[index].stAddMpTime, pstAppCtx->stCurr);

			pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ROLE_LOGIN;
			pstEnv->iGmCmdTime = pstAppCtx->stCurr.tv_sec;

			iArmNum = pstEnv->stRes.Body.RoleLoginRes.Enter.RoleDetail.Wear.ArmNum;
			if(iArmNum > 0)
			{
				for(i=0; i<iArmNum; i++)
				{
					if(pstEnv->stRes.Body.RoleLoginRes.Enter.RoleDetail.Wear.RoleArms[i].DefID == ARM_ID)// 已经装备法杖
					{
						pstEnv->pTestPlayer[index].iIsAreadyArm = 1;
						break;
					}
				}
			}
			
		}
		/*
		else
		{
			pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
			net_clt_han_auth_destroy(&pstEnv->pTestPlayer[index].pNetCltHan);
		}*/
		return 0;
	}
}

int send_heartbeat_req(TAPPCTX *pstAppCtx, ROBOTENV *pstEnv, int index)
{
	if(get_time_diff_to_ms(pstAppCtx->stCurr, pstEnv->pTestPlayer[index].stHeartBeatReqTime)  >= HEARTBEAT_REQ_INTERVAL)
	{
		CSTIME stSvrTime;
		stSvrTime.BenchTimeSec = pstAppCtx->stCurr.tv_sec + pstEnv->pTestPlayer[index].stTimeDiff.tv_sec;
		stSvrTime.BenchTimeUSec = pstAppCtx->stCurr.tv_usec + pstEnv->pTestPlayer[index].stTimeDiff.tv_usec;
		if(stSvrTime.BenchTimeUSec >= 1000000)
		{
			stSvrTime.BenchTimeSec ++;
			stSvrTime.BenchTimeUSec -=1000000;
		}
						
		pstEnv->stReq.Head.Magic = PROTOCOL_MAGIC;
		pstEnv->stReq.Head.Cmd = HEARTBEAT_REQ;
		pstEnv->stReq.Body.HeartBeatReq.SvrTime = stSvrTime;
		
		if (0 > net_clt_auth_send(pstEnv->pTestPlayer[index].pNetCltHan, &pstEnv->stReq, 0))
		{
			/*
			pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
			net_clt_han_auth_destroy(&pstEnv->pTestPlayer[index].pNetCltHan);*/

			pstEnv->pTestPlayer[index].stHeartBeatReqTime = pstAppCtx->stCurr;
			return -1;
		}
		else
		{
			pstEnv->pTestPlayer[index].stHeartBeatReqTime = pstAppCtx->stCurr;
			return 0;
		}			
	}
	return -1;
}


int send_move_clt(TAPPCTX *pstAppCtx, ROBOTENV *pstEnv, int index)
{
	if(get_time_diff_to_ms(pstAppCtx->stCurr, pstEnv->pTestPlayer[index].stSendMovetime) 
		>= pstEnv->pstConf->stRoleMove.dwMoveInterval
		&& pstEnv->pTestPlayer[index].iIsDead == ROLE_LIVE)
	{
		int ix;
		int iy;
		CSTIME stSvrTime;
		int iMoveDistance = pstEnv->pstConf->stRoleMove.dwMoveDistance;
		int iDir  = rand()%360;
		ix = (int)(cosf(iDir)* iMoveDistance);
		iy = (int)(sinf(iDir) * iMoveDistance);
		
		
		stSvrTime.BenchTimeSec = pstAppCtx->stCurr.tv_sec + pstEnv->pTestPlayer[index].stTimeDiff.tv_sec;
		stSvrTime.BenchTimeUSec = pstAppCtx->stCurr.tv_usec + pstEnv->pTestPlayer[index].stTimeDiff.tv_usec;
		if(stSvrTime.BenchTimeUSec >= 1000*1000)
		{
			stSvrTime.BenchTimeSec ++;
			stSvrTime.BenchTimeUSec -=1000*1000;
		}
		pstEnv->stReq.Body.MoveReq.SvrTime = stSvrTime;
		
		pstEnv->stReq.Head.Magic = PROTOCOL_MAGIC;
		pstEnv->stReq.Head.Cmd = MOVE_CLT;
		pstEnv->stReq.Body.MoveReq.x = pstEnv->pTestPlayer[index].stCurrPos.x;
		pstEnv->stReq.Body.MoveReq.y = pstEnv->pTestPlayer[index].stCurrPos.y;
		pstEnv->stReq.Body.MoveReq.MoveType = CS_MOVE_WALK;
		pstEnv->stReq.Body.MoveReq.Data.Walk.Total = 1;
		pstEnv->stReq.Body.MoveReq.Data.Walk.Points[0].x = pstEnv->pTestPlayer[index].stCurrPos.x + ix;
		pstEnv->stReq.Body.MoveReq.Data.Walk.Points[0].y = pstEnv->pTestPlayer[index].stCurrPos.y + iy;

		pstEnv->pTestPlayer[index].iMoveSeq ++;
		pstEnv->stReq.Body.MoveReq.MoveSeq = pstEnv->pTestPlayer[index].iMoveSeq;

		if(0 == net_clt_auth_send(pstEnv->pTestPlayer[index].pNetCltHan, &pstEnv->stReq, 0)) 
		{
			TV_CLONE(pstEnv->pTestPlayer[index].stSendMovetime, pstAppCtx->stCurr);
			pstEnv->pTestPlayer[index].stCurrPos = pstEnv->stReq.Body.MoveReq.Data.Walk.Points[0];
			return 0;
		}
		else
		{
			pstEnv->pTestPlayer[index].stSendMovetime.tv_sec = pstAppCtx->stCurr.tv_sec + 10;
		}
	}
	return -1;
}


int robot_say(ROBOTENV *pstEnv,int iIdx,char *pszSay)
{
	pstEnv->stReq.Head.Magic = PROTOCOL_MAGIC;
	pstEnv->stReq.Head.Cmd = CHAT_REQ;
	pstEnv->stReq.Body.ChatReq.LinkLen = 0;
	pstEnv->stReq.Body.ChatReq.Type = CHAT_SAY;
	pstEnv->stReq.Body.ChatReq.ToName[0] = 0;
	sprintf(pstEnv->stReq.Body.ChatReq.Content,"%s",pszSay);
			
	net_clt_auth_send(pstEnv->pTestPlayer[iIdx].pNetCltHan, &pstEnv->stReq, 0);
	return 0;
}

int login_once_chat(TAPPCTX *pstAppCtx, ROBOTENV *pstEnv, int index)
{
	int i = pstEnv->pTestPlayer[index].iAreadyGmNum;
	int iCount = 0;
	int iLoopGmNum=1;// 一次执行的GmCmd数量  

	//char szCDKey[64]={0};
	
	for (; i<pstEnv->pstConf->iCommCount; i++)
	{
		if (	pstEnv->pstConf->astCommGmCmd[i].szStrChat[0] != 0)
		{
			/*if (strcmp("//accgift",pstEnv->pstConf->astCommGmCmd[i].szStrChat) == 0)
			{
				snprintf(szCDKey,sizeof(szCDKey),"//accgift %d",(index+1)%501);
				robot_say(pstEnv,index,szCDKey);
				printf("%s\n", szCDKey);
			}
			else
			{
				robot_say(pstEnv,index,pstEnv->pstConf->astCommGmCmd[i].szStrChat);
			}*/
			
			robot_say(pstEnv,index,pstEnv->pstConf->astCommGmCmd[i].szStrChat);
			iCount++;
			pstEnv->pTestPlayer[index].iAreadyGmNum++;
			if (iCount%iLoopGmNum==0)
			{
				return 0;
			}
		}
	}

	i = pstEnv->pTestPlayer[index].iAreadyGmNum-pstEnv->pstConf->iCommCount;
	if (i<0)
	{
		pstEnv->pTestPlayer[index].iLoginOnce = 1;
		return 0;
	}
	
	if (pstEnv->pTestPlayer[index].iCareer == CAREER_GUNER)
	{
		for (; i<pstEnv->pstConf->iQsCount; i++)
		{
			if (pstEnv->pstConf->astQsGmCmd[i].szStrChat[0] != 0)
			{
				robot_say(pstEnv,index,pstEnv->pstConf->astQsGmCmd[i].szStrChat);
				iCount++;
				pstEnv->pTestPlayer[index].iAreadyGmNum++;
				if (iCount%iLoopGmNum==0)
				{
					return 0;
				}
			}
		}
	}
	else if (pstEnv->pTestPlayer[index].iCareer == CAREER_WARRIOR)
	{
		for (; i<pstEnv->pstConf->iZsCount; i++)
		{
			if (pstEnv->pstConf->astZsGmCmd[i].szStrChat[0] != 0)
			{
				robot_say(pstEnv,index,pstEnv->pstConf->astZsGmCmd[i].szStrChat);
				iCount++;
				pstEnv->pTestPlayer[index].iAreadyGmNum++;
				if (iCount%iLoopGmNum==0)
				{
					return 0;
				}
			}
		}
	}
	else if(pstEnv->pTestPlayer[index].iCareer == CAREER_CHURCH)
	{
		for (; i<pstEnv->pstConf->iJsCount; i++)
		{
			if (pstEnv->pstConf->astJsGmCmd[i].szStrChat[0] != 0)
			{
				robot_say(pstEnv,index,pstEnv->pstConf->astJsGmCmd[i].szStrChat);
				iCount++;
				pstEnv->pTestPlayer[index].iAreadyGmNum++;
				if (iCount%iLoopGmNum==0)
				{
					return 0;
				}
			}
		}
	}
	else if (pstEnv->pTestPlayer[index].iCareer == CAREER_MAGICOR)
	{
		for (; i<pstEnv->pstConf->iFsCount; i++)
		{
			if (pstEnv->pstConf->astFsGmCmd[i].szStrChat[0] != 0)
			{
				robot_say(pstEnv,index,pstEnv->pstConf->astFsGmCmd[i].szStrChat);
				iCount++;
				pstEnv->pTestPlayer[index].iAreadyGmNum++;
				if (iCount%iLoopGmNum==0)
				{
					return 0;
				}
			}
		}
	}

	pstEnv->pTestPlayer[index].iLoginOnce = 1;
	return 0;
}


int send_chat_req(TAPPCTX *pstAppCtx, ROBOTENV *pstEnv, int index)
{
	if(get_time_diff_to_ms(pstAppCtx->stCurr, pstEnv->pTestPlayer[index].stSendChatTime) 
		>= pstEnv->pstConf->stRoleChat.dwChatInterval
		&& pstEnv->pTestPlayer[index].iIsDead == ROLE_LIVE)
	{
		pstEnv->stReq.Head.Magic = PROTOCOL_MAGIC;
		pstEnv->stReq.Head.Cmd = CHAT_REQ;
		pstEnv->stReq.Body.ChatReq.LinkLen = 0;
		pstEnv->stReq.Body.ChatReq.Type = CHAT_SAY;
		pstEnv->stReq.Body.ChatReq.ToName[0] = 0;
		sprintf(pstEnv->stReq.Body.ChatReq.Content,"%s",pstEnv->pstConf->stRoleChat.szStrChat);
				
		if(0 == net_clt_auth_send(pstEnv->pTestPlayer[index].pNetCltHan, &pstEnv->stReq, 0))
		{
			TV_CLONE(pstEnv->pTestPlayer[index].stSendChatTime, pstAppCtx->stCurr);
			return 0;
		}
		else
		{
			pstEnv->pTestPlayer[index].stSendChatTime.tv_sec = pstAppCtx->stCurr.tv_sec + 10;
		}
	}
	return -1;
}


int recv_server_msg(TAPPCTX *pstAppCtx, ROBOTENV *pstEnv, int index)
{
	int iRet;
	int i;
	
	iRet = net_clt_auth_recv(pstEnv->pTestPlayer[index].pNetCltHan, &pstEnv->stRes, 0);
	if(iRet < 0 )
	{
		pstEnv->pTestPlayer[index].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
		net_clt_han_auth_destroy(&pstEnv->pTestPlayer[index].pNetCltHan);
		return 0;
	}
	else if(0 == iRet)
	{
		return -1;
	}
	else
	{
		if(MOVE_STOP == pstEnv->stRes.Head.Cmd)
		{
			struct timeval stMoveStoptime;
			MS_TO_TV(stMoveStoptime, pstEnv->stRes.Body.MoveStop.BenchTimeMs);
			
			pstEnv->pTestPlayer[index].stCurrPos.x = pstEnv->stRes.Body.MoveStop.x;
			pstEnv->pTestPlayer[index].stCurrPos.y = pstEnv->stRes.Body.MoveStop.y;
			pstEnv->pTestPlayer[index].iMoveSeq = pstEnv->stRes.Body.MoveStop.MoveSeq;
			pstEnv->pTestPlayer[index].stTimeDiff.tv_sec += stMoveStoptime.tv_sec;
			pstEnv->pTestPlayer[index].stTimeDiff.tv_usec += stMoveStoptime.tv_usec;
			if(pstEnv->pTestPlayer[index].stTimeDiff.tv_usec >= 1000*1000)
			{
				pstEnv->pTestPlayer[index].stTimeDiff.tv_sec ++;
				pstEnv->pTestPlayer[index].stTimeDiff.tv_usec -=1000*1000;
			}
			if (pstEnv->pTestPlayer[index].stTimeDiff.tv_usec < 0)
			{
				pstEnv->pTestPlayer[index].stTimeDiff.tv_sec --;
				pstEnv->pTestPlayer[index].stTimeDiff.tv_usec = 1000*1000 + pstEnv->pTestPlayer[index].stTimeDiff.tv_usec;
			}
		}
		else if(ACTION_RES == pstEnv->stRes.Head.Cmd)
		{
			if(pstEnv->stRes.Body.ActionRes.ActionID == ACTION_HIT)
			{
				for(i=0; i<pstEnv->stRes.Body.ActionRes.Data.Hit.TotalHit;i++)
				{
					if(pstEnv->stRes.Body.ActionRes.Data.Hit.Hits[i].ID == pstEnv->pTestPlayer[index].iRoleMemID 
						&& pstEnv->stRes.Body.ActionRes.Data.Hit.Hits[i].DeadFlag == 1)
					{
						pstEnv->pTestPlayer[index].iIsDead = ROLE_DEAD;
						pstEnv->pTestPlayer[index].stCurrPos.x = pstEnv->stRes.Body.ActionRes.Data.Dead.x;
						pstEnv->pTestPlayer[index].stCurrPos.y = pstEnv->stRes.Body.ActionRes.Data.Dead.y;
						break;
					}
				}
			}
			else if( pstEnv->stRes.Body.ActionRes.ActionID  == ACTION_REVIVE )
			{
				if(pstEnv->stRes.Body.ActionRes.ID == pstEnv->pTestPlayer[index].iRoleMemID )
				{
					pstEnv->pTestPlayer[index].iIsDead = ROLE_LIVE;
				}
			}
		}
		else if (ENTER_MAP == pstEnv->stRes.Head.Cmd)
		{	
			CSENTERMAP *pstEnterMap = &pstEnv->stRes.Body.EnterMap;
			pstEnv->pTestPlayer[index].stCurrPos.x = pstEnv->stRes.Body.EnterMap.x;
			pstEnv->pTestPlayer[index].stCurrPos.y = pstEnv->stRes.Body.EnterMap.y;
			pstEnv->pTestPlayer[index].iMoveSeq = pstEnv->stRes.Body.EnterMap.InitMoveSeq;	
		}
		else if (AUTOTEAM_SVR == pstEnv->stRes.Head.Cmd)
		{
			if (pstEnv->stRes.Body.AutoTeamSvr.AutoTeamSvrOp == AUTOTEAM_SVR_OP_PRE_ADD)
			{
				pstEnv->pTestPlayer[index].iAutoTeamNum += 
									pstEnv->stRes.Body.AutoTeamSvr.AutoTeamSvrData.PreAdd.Num;
			}
		}
		return 0;
	}
}


int send_skill_atk(TAPPCTX *pstAppCtx, ROBOTENV *pstEnv, int index)
{
	if(get_time_diff_to_ms(pstAppCtx->stCurr, pstEnv->pTestPlayer[index].stSendSkillTime) 
		>= pstEnv->pstConf->stRoleSkill.dwSkillInterval
		&& pstEnv->pTestPlayer[index].iIsDead == ROLE_LIVE)
	{
		pstEnv->stReq.Head.Magic = PROTOCOL_MAGIC;
		pstEnv->stReq.Head.Cmd = CHAT_REQ;
		pstEnv->stReq.Body.ChatReq.LinkLen = 0;
		pstEnv->stReq.Body.ChatReq.Type = CHAT_SAY;
		pstEnv->stReq.Body.ChatReq.ToName[0] = 0;
		sprintf(pstEnv->stReq.Body.ChatReq.Content,"%s %d %d","//skillatk",pstEnv->pstConf->stRoleSkill.iSkillID, 1);
				
		if(0 == net_clt_auth_send(pstEnv->pTestPlayer[index].pNetCltHan, &pstEnv->stReq, 0))
		{
			TV_CLONE(pstEnv->pTestPlayer[index].stSendSkillTime, pstAppCtx->stCurr);
			return 0;
		}
		else
		{
			pstEnv->pTestPlayer[index].stSendSkillTime.tv_sec = pstAppCtx->stCurr.tv_sec + 10;
		}
	}
	return -1;
}

int send_adjust_clt(TAPPCTX *pstAppCtx, ROBOTENV *pstEnv, int index)
{
	/*
	if(pstEnv->pTestPlayer[index].iIsAreadyArm == 0)
	{
		pstEnv->stReq.Head.Magic = PROTOCOL_MAGIC;
		pstEnv->stReq.Head.Cmd = ADJUST_CLT;
		pstEnv->stReq.Body.AdjustC.Type = ADJUST_TYPE_WEAR;
		pstEnv->stReq.Body.AdjustC.AdjustData.Wear.ArmDefID = ARM_ID;			// 法杖
		pstEnv->stReq.Body.AdjustC.AdjustData.Wear.GridIdx = 1;					// 第几个格子
		pstEnv->stReq.Body.AdjustC.AdjustData.Wear.ListType = LIST_TYPE_SUNDRIES;
		pstEnv->stReq.Body.AdjustC.AdjustData.Wear.WearPos = WEAR_POS_RIGHTHAND;

		if(0 == net_clt_auth_send(pstEnv->pTestPlayer[index].pNetCltHan, &pstEnv->stReq, 10))
		{
			pstEnv->pTestPlayer[index].iIsAreadyArm = 1;
			return 0;
		}
	}
	return -1;*/

	pstEnv->pTestPlayer[index].iIsAreadyArm = 1;
	return 0;
}



#if 0
	    CAREER_WARRIOR_SINGLEHAND = 17, 	/* 守护者 */
    CAREER_WARRIOR_DOUBLEHAND = 33, 	/* 征服者 */
    CAREER_MAGICOR_SINGLEHAND = 18, 	/* 聚能者 */
    CAREER_MAGICOR_DOUBLEHAND = 34, 	/* 炼魂师 */
    CAREER_GUNER_SINGLEHAND = 19, 	/* 猎杀者 */
    CAREER_GUNER_DOUBLEHAND = 35, 	/* 毁灭者 */
    CAREER_CHURCH_SINGLEHAND = 20, 	/* 圣佑者 */
    CAREER_CHURCH_DOUBLEHAND = 36, 	/* 驭雷师 */

#endif

int robot_player_get_gmcmd_num(ROBOTENV *pstEnv, int iIdx)
{
	if (pstEnv->pTestPlayer[iIdx].iCareer == CAREER_GUNER ||
		pstEnv->pTestPlayer[iIdx].iCareer == CAREER_GUNER_SINGLEHAND ||
		pstEnv->pTestPlayer[iIdx].iCareer == CAREER_GUNER_DOUBLEHAND)
	{
		pstEnv->pTestPlayer[iIdx].iGmCmdNum =pstEnv->pstConf->iQsCount+pstEnv->pstConf->iCommCount;
	}
	else if (pstEnv->pTestPlayer[iIdx].iCareer == CAREER_MAGICOR||
		pstEnv->pTestPlayer[iIdx].iCareer == CAREER_MAGICOR_SINGLEHAND||
		pstEnv->pTestPlayer[iIdx].iCareer == CAREER_MAGICOR_DOUBLEHAND)
	{
		pstEnv->pTestPlayer[iIdx].iGmCmdNum =pstEnv->pstConf->iFsCount+pstEnv->pstConf->iCommCount;
	}
	else if(pstEnv->pTestPlayer[iIdx].iCareer == CAREER_WARRIOR||
		pstEnv->pTestPlayer[iIdx].iCareer == CAREER_WARRIOR_SINGLEHAND||
		pstEnv->pTestPlayer[iIdx].iCareer == CAREER_WARRIOR_DOUBLEHAND)
	{
		pstEnv->pTestPlayer[iIdx].iGmCmdNum =pstEnv->pstConf->iZsCount+pstEnv->pstConf->iCommCount;
	}
	else if(pstEnv->pTestPlayer[iIdx].iCareer == CAREER_CHURCH||
		pstEnv->pTestPlayer[iIdx].iCareer == CAREER_CHURCH_SINGLEHAND||
		pstEnv->pTestPlayer[iIdx].iCareer == CAREER_CHURCH_DOUBLEHAND)
	{
		pstEnv->pTestPlayer[iIdx].iGmCmdNum =pstEnv->pstConf->iJsCount+pstEnv->pstConf->iCommCount;
	}
	else
	{
		printf("robot_player_get_gmcmd_num error!\n");
	}
	return 0;
}


int robot_autoteam_agree(TAPPCTX *pstAppCtx, ROBOTENV *pstEnv, int index)
{
	robot_say(pstEnv,index,"//ateamagree");
	return 0;
}

int robot_wear_rand_arm(TAPPCTX *pstAppCtx, ROBOTENV *pstEnv, int index)
{
	TESTPLAYER *pstPlayer = &pstEnv->pTestPlayer[index];
	int iRand = 0;
	int i;
	char szGm[64];
	
	
	if (pstPlayer->iCareer == CAREER_MAGICOR)
	{
		if (pstEnv->pstConf->iFsArmNum <= 0)
		{
			return 0;
		}
		iRand = rand()%pstEnv->pstConf->iFsArmNum;
		for (i=0; i<2; i++)
		{
			if (pstEnv->pstConf->astFsArm[iRand].iArmID<= 0)
			{
				return 0;
			}
			snprintf(szGm, sizeof(szGm),"//wear %d", pstEnv->pstConf->astFsArm[iRand++].iArmID);
			robot_say(pstEnv,index,szGm);
			iRand  %=pstEnv->pstConf->iFsArmNum;
		}
	}
	else if (pstPlayer->iCareer == CAREER_CHURCH)
	{
		if (pstEnv->pstConf->iJsArmNum <= 0)
		{
			return 0;
		}
		iRand = rand()%pstEnv->pstConf->iJsArmNum;
		for (i=0; i<2; i++)
		{
			if (pstEnv->pstConf->astJsArm[iRand].iArmID <= 0)
			{
				return 0;
			}
			snprintf(szGm, sizeof(szGm),"//wear %d", pstEnv->pstConf->astJsArm[iRand++].iArmID);
			robot_say(pstEnv,index,szGm);
			
			iRand  %=pstEnv->pstConf->iFsArmNum;
		}
	}
	else if(pstPlayer->iCareer == CAREER_GUNER)
	{
		if (pstEnv->pstConf->iQsArmNum <= 0)
		{
			return 0;
		}
		iRand = rand()%pstEnv->pstConf->iQsArmNum;
		for (i=0; i<2; i++)
		{
			if (pstEnv->pstConf->astQsArm[iRand].iArmID <= 0)
			{
				return 0;
			}
			snprintf(szGm, sizeof(szGm),"//wear %d", pstEnv->pstConf->astQsArm[iRand++].iArmID);
			robot_say(pstEnv,index,szGm);
			iRand  %=pstEnv->pstConf->iQsArmNum;
		}
	}
	else if(pstPlayer->iCareer == CAREER_WARRIOR)
	{
		if (pstEnv->pstConf->iZsArmNum <= 0)
		{
			return 0;
		}
		iRand = rand()%pstEnv->pstConf->iZsArmNum;
		for (i=0; i<2; i++)
		{
			if (pstEnv->pstConf->astZsArm[iRand].iArmID <= 0)
			{
				return 0;
			}
			snprintf(szGm, sizeof(szGm),"//wear %d", pstEnv->pstConf->astZsArm[iRand++].iArmID);
			robot_say(pstEnv,index,szGm);
			iRand  %=pstEnv->pstConf->iZsArmNum;
		}
	}
	else
	{
		return -1;
	}

	

	return 0;
}

int robot_proc(TAPPCTX *pstAppCtx, ROBOTENV *pstEnv)
{
	int iBusy=0;
	int i;
	int iLoginNum = pstEnv->iLoginNum;

	if(pstEnv->iConnectStatus != CONNECT)
	{
		return -1;
	}

	for(i = 0; i<iLoginNum; i++)
	{
		if (pstEnv->pTestPlayer[i].pNetCltHan == NULL)
		{
			pstEnv->pTestPlayer[i].iRoleStatus = PLAYER_STATUS_ACC_NOLOGIN;
		}

		switch (pstEnv->pTestPlayer[i].iRoleStatus)
		{
		case PLAYER_STATUS_ACC_NOLOGIN:

			break;
		case PLAYER_STATUS_ACC_LOGIN_REQ:
			if(recv_acc_login_res(pstAppCtx, pstEnv,i) == 0)
			{
				iBusy = 1;
			}
			break;
		case PLAYER_STATUS_ACC_LOGIN:
			if(recv_role_list_res(pstAppCtx, pstEnv,i) == 0)
			{
				iBusy = 1;
			}
			break;
		case PLAYER_STATUS_NEW_ROLE_REQ:
			if(recv_new_role_res(pstAppCtx, pstEnv,i) == 0)
			{
				iBusy = 1;
			}
			break;

		case PLAYER_STATUS_ROLE_LOGIN_REQ:
			if(recv_role_login_res(pstAppCtx, pstEnv,i) == 0)
			{
				iBusy = 1;
			}
			break;
		case PLAYER_STATUS_ROLE_LOGIN:

			if (!(pstEnv->pTestPlayer[i].iFlag &FLAG_GET_GM_NUM))
			{
				robot_player_get_gmcmd_num(pstEnv, i);
				pstEnv->pTestPlayer[i].iFlag |=FLAG_GET_GM_NUM;
			}

			if (pstAppCtx->stCurr.tv_sec - pstEnv->iGmCmdTime > 10 )
			{
				if (pstEnv->pTestPlayer[i].iLoginOnce == 0)
				{
					// 登陆后执行一次
					login_once_chat(pstAppCtx, pstEnv, i);
				}

				if (pstEnv->pTestPlayer[i].iRandArm == 0)
				{
					robot_wear_rand_arm(pstAppCtx, pstEnv, i);
					pstEnv->pTestPlayer[i].iRandArm= 1;
				}
				//pstEnv->iGmCmdTime = pstAppCtx->stCurr.tv_sec;
			}

			if (pstEnv->pTestPlayer[i].iAutoTeamNum >= 6)
			{
				// 开搞
				robot_autoteam_agree(pstAppCtx, pstEnv, i);
				pstEnv->pTestPlayer[i].iAutoTeamNum = 0;
			}
			
			
			if(send_adjust_clt(pstAppCtx, pstEnv,i ) == 0)
			{
				iBusy = 1;
			}
			
			if(send_heartbeat_req(pstAppCtx, pstEnv,i) == 0)
			{
				iBusy = 1;
			}

			if(i < pstEnv->iMoveEndInx)
			{
				if(send_move_clt(pstAppCtx, pstEnv,i) == 0)
				{
					iBusy = 1;
				}
			}
			else if( i < pstEnv->iChatEndInx)
			{
				if(send_chat_req(pstAppCtx, pstEnv,i) == 0)
				{
					iBusy = 1;
				}
			}
			
			else if(i < pstEnv->iSkillEndInx)
			{
				if(send_skill_atk(pstAppCtx, pstEnv,i) == 0)
				{
					iBusy = 1;
				}
			}
			else
			{
				// other
			}
			
			if(recv_server_msg(pstAppCtx, pstEnv,i) == 0)
			{
				iBusy = 1;
			}

			break;
			
		default:
			break;
		}
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

int robot_reload(TAPPCTX *pstAppCtx, ROBOTENV *pstEnv)
{
	ROBOTCONF   *pstPreConf;
	
	pstPreConf = (ROBOTCONF   *)pstAppCtx->stConfPrepareData.pszBuff;
	
	printf("robot reload\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "robot reload");

	return 0;
}

int robot_fini(TAPPCTX *pstAppCtx, ROBOTENV *pstEnv)
{
	printf("robot finish\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "robot finish");
	return 0;
}

int robot_tick(TAPPCTX *pstAppCtx, ROBOTENV *pstEnv)
{
	return scanf_role(pstAppCtx, pstEnv);
}

int robot_stop(TAPPCTX *pstAppCtx, ROBOTENV *pstEnv)
{
	int iReady=0;

	if (pstEnv->iConnectStatus == CONNECT)
	{
		pstEnv->iConnectStatus = READYDISCONN;
	}
	else if (pstEnv->iConnectStatus == DISCONNECT)
	{
		iReady = 1;
	}
	
	if (iReady)
	{
		if (pstEnv->pLibc)
		{
			free(pstEnv->pLibc);
		}
		return -1;
	}
	else
	{
		return 0;
	}
}

int main(int argc, char* argv[])
{
	int iRet;
	void* pvArg	=	&gs_stEnv;

	memset(&gs_stAppCtx, 0, sizeof(gs_stAppCtx));
	memset(&gs_stEnv, 0, sizeof(gs_stEnv));

	gs_stAppCtx.argc	=	argc;
	gs_stAppCtx.argv	=	argv;

	gs_stAppCtx.iTimer = 2000;
	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)robot_init;
	
	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_robot;
	gs_stAppCtx.stConfData.pszMetaName = "robotconf";
	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);

	iRet	=	tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		printf("Error: app Initialization fail.\n");
		return iRet;
	}

	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)robot_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)robot_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)robot_tick;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)robot_reload;
	gs_stAppCtx.pfnStop  =   (PFNTAPPFUNC)robot_stop;

	
	iRet	=	tapp_def_mainloop(&gs_stAppCtx, pvArg);

	tapp_def_fini(&gs_stAppCtx, pvArg);
	
	return iRet;
}




