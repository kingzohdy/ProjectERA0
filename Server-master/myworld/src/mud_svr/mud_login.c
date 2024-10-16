/*
#include "mud_svr.h"
#include "mud_player.h"
#include "mud_clt.h"

//static int init(Module *pstMod);
//static int tick(Module *pstMod);
//static int fini(Module *pstMod);

static int acc_login_req(Module *pstMod, MudPlayer *pstPlayer, const CSPKG *pstPkg);
static int acc_logout_req(Module *pstMod, MudPlayer *pstPlayer, const CSPKG *pstPkg);

static int login_perm_check(const CSACCLOGINREQ *pstReq);
static int login_pre_check(const CSACCLOGINREQ *pstReq);
static int login_to_online(const CSACCLOGINREQ *pstReq, MudPlayer **pstOut);
static void login_attach_player(const CSACCLOGINREQ *pstReq,
                                MudPlayer *pstPlayer);
static int login_respond(const CSACCLOGINREQ *pstReq, MudPlayer *pstPlayer);
static int kickoff_for_relogin(const MudUinCache *pstUin, MudPlayer *pstPlayer);
static int alloc_for_login(const CSACCLOGINREQ *pstReq, MudUinCache *pstUin,
                           MudPlayer **pstOut);

static CSHandlerEntry g_astLoginCSHandlers[] = {
	{ ACC_LOGIN_REQ,  -1, acc_login_req,  }, // 登录
	{ ACC_LOGOUT_REQ, -1, acc_logout_req, }, // 登出
	{ 0, 0, NULL },
};

Module gs_ModLogin = {
	.pszName = "login",
	.pfnInit = NULL,
	.pfnFini = NULL,
	.iIntervalTime = 0,
	.pfnTick = NULL,
	.astSSHandlers = NULL, // 不处理SS消息
	.astCSHandlers = g_astLoginCSHandlers, // CS消息处理器
	.astDBHandlers = NULL, // 不处理DB消息
	.pvData = NULL,
	.iTickCount = 0,
};

static inline const MudUinCache *fast_find_uin_info(int iUin)
{
	MudUinCache stKey;
	const MudUinCache *pstTarget = NULL;
	
	stKey.iUin = iUin;
	pstTarget = sht_find(this_env()->pstShtUinCache, &stKey, UinHashCmp,
	                     UinHashCode);
	return pstTarget;
}

static inline MudUinCache *fast_find_mutable_uin_info(int iUin)
{
	MudUinCache stKey;
	MudUinCache *pstTarget = NULL;
	
	stKey.iUin = iUin;
	pstTarget = sht_find(this_env()->pstShtUinCache, &stKey, UinHashCmp,
	                     UinHashCode);
	return pstTarget;
}

static inline const char* FH2Z(const TFRAMEHEAD *pstFrameHead)
{
	return inet_ntoa(*(struct in_addr *)&pstFrameHead->ExtraInfo.IPInfo.ip);
}

static inline unsigned short FH2S(const TFRAMEHEAD *pstFrameHead)
{
	return ntohs(pstFrameHead->ExtraInfo.IPInfo.port);
}




static int acc_login_req(Module *pstMod, MudPlayer *pstPlayer, const CSPKG *pstPkg)
{
	const CSACCLOGINREQ *pstReq = &pstPkg->Body.AccLoginReq;

	UNUSED(pstMod);
	UNUSED(pstPlayer);

	if(login_pre_check(pstReq) < 0)
	{
		return -1;
	}
	
	if(login_perm_check(pstReq) < 0)
	{
		return -1;
	}

	if(login_to_online(pstReq, &pstPlayer) < 0)
	{
		return -1;
	}

	login_attach_player(pstReq, pstPlayer);
	
	return login_respond(pstReq, pstPlayer);
}

static int acc_logout_req(Module *pstMod, MudPlayer *pstPlayer,
                          const CSPKG *pstPkg)
{
	UNUSED(pstMod);
	UNUSED(pstPlayer);
	UNUSED(pstPkg);

	return 0;
}

static int login_pre_check(const CSACCLOGINREQ *pstReq)
{
	const TFRAMECMDSTART *pstStart = &last_frame()->CmdData.Start;
	if(last_frame()->Cmd != TFRAMEHEAD_CMD_START)
	{
		return -1;
	}

	if(!this_env()->pstConf->NotUinAuth)
	{
		if(pstStart->AuthType != AUTH_JC ||
		   pstStart->AuthData.AuthJC.Uin != pstReq->Uin)
		{
			return -1;
		}
	}

	//server关闭过程中禁止登录
	//if (pstEnv->cShutdown)
	//{
	//	mud_player_acc_login_err(pstEnv, pstFrameHead, LOGIN_NOSERVICE);
	//	return -1;
	//}
	return 0;
}

static int login_perm_check(const CSACCLOGINREQ *pstReq)
{
	MUD_SVRENV* pstEnv = this_env();

	if((pstEnv->pstConf->MaxCltProtoVer &&
	    pstReq->CltProtoVer > pstEnv->pstConf->MaxCltProtoVer) ||
	   (pstEnv->pstConf->MinCltProtoVer &&
	    pstReq->CltProtoVer < pstEnv->pstConf->MinCltProtoVer))
	{	
		tlog_debug(
			pstEnv->pstLogCat, 0, 0,
			"client ip=%s port=%d proto ver %d",
			FH2Z(last_frame()),
			FH2S(last_frame()),
			pstReq->CltProtoVer);

		mud_player_acc_login_err(last_frame(), LOGIN_BADCLT);
		return -1;
	}

	if(black_white_check(pstEnv->pstBlackWhiteH,
	                     last_frame()->ExtraInfo.IPInfo.ip))
	{
		mud_player_acc_login_err(last_frame(), LOGIN_NOSERVICE);
		return -1;
	}

	tlog_debug(
		pstEnv->pstLogCat, pstReq->Uin, 0,
		"client uin=%d ip=%s port=%d ConnIdx=%d acc login",
		pstReq->Uin, 
		FH2Z(last_frame()),
		FH2S(last_frame()),
		last_frame()->ConnIdx);
	return 0;
}

static int kickoff_for_relogin(const MudUinCache *pstUin, MudPlayer *pstPlayer)
{
	char szMsg[DESC_LEN];

	snprintf(szMsg, CCH(szMsg), "有来自ip地址为%s的用户使用您的帐号登录", 
	         FH2Z(last_frame()));

	mud_player_kickoff_text_notify(pstPlayer, szMsg);
	mud_player_disconnect(pstPlayer);
				
	if(!(pstUin && pstUin->cGetRoleList))
	{
		mud_player_acc_login_err(last_frame(), LOGIN_SVRBUSY);
		return -1;
	}
	return 0;
}

static int alloc_for_login(const CSACCLOGINREQ *pstReq, MudUinCache *pstUin,
                           MudPlayer **pstOut)
{
	const TFRAMECMDSTART *pstStart = &last_frame()->CmdData.Start;

	if(this_env()->pstPlayerPool->iUsed >= MAX_MUD_PLAYER_SOFT &&
	   ((UIN_FLAG_GM & pstStart->AuthData.AuthJC.UinFlag) == 0))
	{
		mud_player_acc_login_err(last_frame(), LOGIN_SVRBUSY);
		return -1;
	}
		
	*pstOut = mud_player_alloc(pstReq->Uin, pstReq->AccountLoginName,
	                           pstUin);
	if(!(*pstOut))
	{
		mud_player_acc_login_err(last_frame(), LOGIN_SVRBUSY);
		tlog_error(this_env()->pstLogCat, pstReq->Uin, 0,
		           "recv Uin=%d acc login, but player_alloc NULL",
		           pstReq->Uin);
		return -1;
	}

	(*pstOut)->stRoleData.Uin = pstReq->Uin;
	return 0;
}

static int login_to_online(const CSACCLOGINREQ *pstReq, MudPlayer **pstOut)
{
	int iRet = 0;
	MudUinCache *pstUin = fast_find_mutable_uin_info(pstReq->Uin);
	MudPlayer *pstNewPlayer = NULL;

	if(pstUin &&
	   (pstUin->tLastLogin + this_env()->pstConf->LoginInterval >= CURR))
	{
		mud_player_acc_login_err(last_frame(), LOGIN_FREQUENCY);
		return -1;
	}

	pstNewPlayer = mud_player_get_by_uin(pstReq->Uin);
	if(pstNewPlayer)
	{
		iRet = kickoff_for_relogin(pstUin, pstNewPlayer);
	}
	else
	{
		iRet = alloc_for_login(pstReq, pstUin, &pstNewPlayer);
	}

	if(pstOut && pstNewPlayer)
	{
		*pstOut = pstNewPlayer;
	}
	return iRet;
}

static void login_attach_player(const CSACCLOGINREQ *pstReq, MudPlayer *pstPlayer)
{
	const TFRAMECMDSTART *pstStart = &last_frame()->CmdData.Start;

	// 挂机禁言应该也生效吧
	pstPlayer->iAccSilenceEnd = pstStart->AuthData.AuthJC.AccSilenceEnd;

	pstPlayer->unCltProVer = pstReq->CltProtoVer;	
	
	pstPlayer->eStatus = MUD_PLAYER_STATUS_ACC_LOGIN;
	pstPlayer->tActive = CURR;

	pstPlayer->stConnectInfo.chVer = last_frame()->Ver;
	pstPlayer->stConnectInfo.chCmd = TFRAMEHEAD_CMD_START;
	pstPlayer->stConnectInfo.chExtraType = last_frame()->ExtraType;
	pstPlayer->stConnectInfo.stExtraInfo = last_frame()->ExtraInfo;
	pstPlayer->stConnectInfo.chTimeStampType = last_frame()->TimeStampType;
	pstPlayer->stConnectInfo.stTimeStamp = last_frame()->TimeStamp;
	pstPlayer->stConnectInfo.iConnIdx = last_frame()->ConnIdx;
	pstPlayer->stConnectInfo.iID = pstPlayer->iIdx;
	pstPlayer->stOnline.bConnLost = 0;
}

static int login_respond(const CSACCLOGINREQ *pstReq, MudPlayer *pstPlayer)
{
	CSPKG *pstPkg = &this_env()->stCSRes;
	CSACCLOGINRES *pstRes = &this_env()->stCSRes.Body.AccLoginRes;
	const MudUinCache *pstUin = fast_find_uin_info(pstReq->Uin);
	
	Z_CSHEAD_INIT(&pstPkg->Head, ACC_LOGIN_RES);
	pstRes->Result = LOGIN_SUCC;
	pstRes->SvrProtoVer = TDR_METALIB_PROTO_CS_VERSION;

	if(z_cltmsg_send(pstPlayer, pstPkg) < 0)
	{
		mud_player_acc_login_err(last_frame(), LOGIN_SVRBUSY);

		tlog_error(this_env()->pstLogCat, pstReq->Uin, 0,
		           "recv Uin=%d acc login, but z_cltmsg_send fail",
		           pstReq->Uin);
		mud_player_free(pstPlayer);
		return -1;
	}
	
	pstPlayer->stConnectInfo.chCmd = TFRAMEHEAD_CMD_INPROC;

	if(pstUin && pstUin->cGetRoleList)
	{
		pstPlayer->eStatus = MUD_PLAYER_STATUS_ROLE_LIST;
	
		//return z_send_rolelist(pstEnv, pstPlayerNew, pstUinCache);
	}
	else
	{
		//send role list req
		//return z_db_role_list(pstAppCtx, pstEnv, pstPlayerNew);
	}
	return 0;
}
*/
