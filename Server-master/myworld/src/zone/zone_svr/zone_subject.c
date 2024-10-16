

#include "zone_subject.h"
#include "zone_clt.h"
#include "zone_attr.h"
#include "resdb_meta.h"
#include "zone_shop.h"
#include "zone_err.h"
#include "zone_zh_name.h"
#include "zone_package.h"
#include "zone_mail.h"

#define SUBJECT_MIN_LEVEL 10

#define SUBJECT_MAX_AWARD_LIBAO_TOP  100  // 前100名获得礼包
#define SUBJECT_LIBAO_LEVEL  4        //分几层来奖励
static const int astAwardTop[] = {3,10,20,100};   // 名次分层
static const int astAwardNum[] = {5,3,2,1};       // 根据名次层次来奖励礼包个数


int subject_invite_player(ZONESVRENV* pstEnv, Player *pstPlayer, int iStartTime, char cSingle, int iInvite)
{
	CSPKG stPkg;
	SUBJECTSVR *pstSvr= &stPkg.Body.SubJectSvr;
	int i;
	Player *pstTmp;
	TMEMBLOCK* pstMemBlock;

	pstSvr->Cmd = SUBJECT_SVR_OP_INVITE;
	pstSvr->Data.Invite.Invite = iInvite;
	pstSvr->Data.Invite.StartTime = iStartTime;
	Z_CSHEAD_INIT(&stPkg.Head, SUBJECT_SVR);

	if (cSingle && pstPlayer)
	{
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);	
		return 0;
	}

	for (i=0; i<MAX_PLAYER; i++)
	{
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstTmp = (Player *)pstMemBlock->szData;
		if (	pstTmp && 
			(pstTmp->eStatus == PLAYER_STATUS_ROLE_LOGIN ||
			pstTmp->eStatus == PLAYER_STATUS_ROLE_NOTHING) &&
			pstTmp->stRoleData.Level >= SUBJECT_MIN_LEVEL)
		{
			z_cltmsg_send(pstEnv, pstTmp, &stPkg);	
		}
		
	}
	return 0;
}

int subject_get_theme_in(ZONESVRENV* pstEnv)
{
	SUBJECTDEF *pstSubDef;
	SUBJECT *pstSubJect = &pstEnv->pstZoneObj->stSubJect;

	if (pstSubJect->cSubJectNum < SUBJECT_MAX_NUM)
	{
		// 如果subject_get_theme出错,随机找一个题目
		pstSubDef =  subject_get_theme(pstEnv);
		if (!pstSubDef)
		{
			pstSubDef = (SUBJECTDEF *)&pstEnv->pstZoneObj->sSubJectDef[RAND1(pstEnv->pstZoneObj->iSubJectDef)];
		}
		
		pstSubJect->iAlready[(int)pstSubJect->cSubJectNum] = pstSubDef->ID;
		pstSubJect->cSubJectNum ++;
		
		pstSubJect->iSubJectID = pstSubDef->ID;
		pstSubJect->iSuccIdx = pstSubDef->Succ-1;
	}

	return 0;
}

int subject_info_notify(ZONESVRENV* pstEnv, Player *pstPlayer, SUBJECTTOP* pstTop, int iIdx)
{	
	CSPKG stPkg;
	SUBJECTSVR *pstSvr= &stPkg.Body.SubJectSvr;
	SUBJECTFLUSH *pstFlush = &pstSvr->Data.Flush;
	LEVELUP *pstLevelUp;
	char cTopInfo = 1;
	int iSucc;
	int iFail;
	SUBJECT *pstSubJect = &pstEnv->pstZoneObj->stSubJect;
	//SUBJECTLIST *pstSubList = &pstEnv->pstZoneObj->stSubList;

	memset(pstFlush, 0, sizeof(*pstFlush));

	// 活动没开始,肯定是没有题的
	if (pstSubJect->cStat == SUBJECT_STAT_START)
	{
		// 活动信息 
		pstFlush->Ploy.ID = pstSubJect->iSubJectID;
		pstFlush->Ploy.ResEnd = pstSubJect->tResEnd;
		pstFlush->Ploy.LockEnd = pstSubJect->tLockEnd;

		// 玩家下线上线要把本次答题信息发过去
		if (pstPlayer->stRoleData.MiscInfo.SubJectInfo.SubJectVar.SubJectIdx == pstSubJect->cSubJectNum)
		{
			pstFlush->Self.CurrIdx = pstPlayer->stRoleData.MiscInfo.SubJectInfo.SubJectVar.CurrIdx;
		}

		if (pstSubJect->cSubJectNum == 1)
			cTopInfo = 0;
	}
	else if (pstSubJect->cStat == SUBJECT_STAT_REQ)
	{
		pstFlush->Ploy.ReqEnd = pstSubJect->tReqEnd;
		cTopInfo = 0;
	}
	else if (pstSubJect->cStat == SUBJECT_STAT_END)
	{
		int  iTmp;
		unsigned int uiExp;
		
		iSucc = pstPlayer->stRoleData.MiscInfo.SubJectInfo.SubJectVar.Succ;
		iFail = pstPlayer->stRoleData.MiscInfo.SubJectInfo.SubJectVar.Fail;
		// 奖励信息
		pstLevelUp = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
		if (!pstLevelUp)
			return -1;

		uiExp =  pstLevelUp->ContributeExp;

		// 活动人气奖励	经验=M*6/1000
		pstFlush->Award.Add = uiExp * (6.0/1000); 	

		//完成情况奖励	答对一题经验=(M*15/1000)/24
		//						答错一题经验=(M*3/1000)/24
		pstFlush->Award.Base = (iSucc*(uiExp * (15.0/1000)/24)) + (iFail*(uiExp * (3.0/1000)/24));

		// 		竞赛排名奖励	奖励经验=基础经验+排名经验
		//		基础经验=M*36/10000
		//		排名经验=M*27/100000*if（排名<=20,(21-排名），0）
		iTmp = (iIdx>=20)?0:(uiExp*(27.0/100000)*(21-(iIdx+1)));
		pstFlush->Award.Top = uiExp*(36.0/10000) + iTmp;

		// 加经验 todo
		player_add_exp(pstEnv, pstPlayer, pstFlush->Award.Add+pstFlush->Award.Base+pstFlush->Award.Top, 0);
	}
	else
	{
		return -1;
	}
	
	// 自己的信息
	pstFlush->Self.Star = 3 -pstPlayer->stRoleData.MiscInfo.SubJectInfo.SubJectVar.Star;
	pstFlush->Self.Succ = pstPlayer->stRoleData.MiscInfo.SubJectInfo.SubJectVar.Succ;
	pstFlush->Self.Top = 0;
	pstFlush->Self.Integ = pstSubJect->astSJPlayer[iIdx].iInteg;

	// 排名信息
	if (cTopInfo && pstTop)
	{
		pstFlush->Top = *pstTop;
		pstFlush->Self.Top = iIdx + 1;
	}

	// 活动信息
	pstFlush->Ploy.Already = pstSubJect->cSubJectNum;
	pstFlush->Ploy.Odd = SUBJECT_MAX_NUM - pstSubJect->cSubJectNum;

	pstSvr->Cmd = SUBJECT_SVR_OP_FLUSH;
	Z_CSHEAD_INIT(&stPkg.Head, SUBJECT_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);	
	return 0;
}


int subject_get_top(ZONESVRENV* pstEnv, SUBJECTTOP *pstTop)
{
	int i;
	Player *pstPlayer;
	pstTop->Num = 0;
	SUBJECT *pstSubJect = &pstEnv->pstZoneObj->stSubJect;

	if (pstSubJect->cStat == SUBJECT_STAT_REQ)
	{
		return 0;
	}
	
	for (i=0; i<pstSubJect->iPlayer; i++)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx,  pstEnv, pstSubJect->astSJPlayer[i].iMemID);
		if (pstPlayer)
		{
			pstTop->Tops[pstTop->Num].Top = pstTop->Num+1;	 	// 排名从1开始
			pstTop->Tops[pstTop->Num].Integ = pstSubJect->astSJPlayer[i].iInteg;
			STRNCPY(pstTop->Tops[pstTop->Num].RoleName, pstPlayer->stRoleData.RoleName, 
					sizeof(pstTop->Tops[pstTop->Num].RoleName));
			pstTop->Num ++;
		}

		if (pstTop->Num >= SUBJECT_MAX_TOP)
				break;
	}
	return 0;
}

int subject_info_notify_by_world(ZONESVRENV* pstEnv)
{
	int i;
	Player *pstPlayer;
	SUBJECTTOP stTop;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	SUBJECT *pstSubJect = &pstEnv->pstZoneObj->stSubJect;

	pstSubJect->tLockEnd 	= iCurr + SUBJECT_LOCK_END;
	pstSubJect->tResEnd 	= iCurr + SUBJECT_RES_END;
	
	subject_get_top(pstEnv, &stTop);
	subject_get_theme_in(pstEnv);

	for (i=0; i<pstSubJect->iPlayer; i++)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstSubJect->astSJPlayer[i].iMemID);
		if (pstPlayer && 
			(pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN||
			pstPlayer->eStatus == PLAYER_STATUS_ROLE_NOTHING))
		{
			subject_info_notify(pstEnv, pstPlayer, &stTop, i);
		}
	}
	return 0;
}


int subject_player_login(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iStartTime = 0;
	SUBJECTINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.SubJectInfo;
	GameEventInst *pstInst;
	int iIdx;
	SUBJECTTOP stTop;
	SUBJECT *pstSubJect = &pstEnv->pstZoneObj->stSubJect;

	pstInst =  z_get_game_event(pstEnv,SUBJECT_EVENT_ID);
	if (!pstInst || pstInst->bStat != GAME_EVENT_RUN)
	{
		return 0;
	}

	if (pstPlayer->stRoleData.Level < SUBJECT_MIN_LEVEL)
	{
		return 0;
	}	
	
	// 自己主动退出的不邀请
	if (	pstInfo->ExitTime > 0 &&
		pstInfo->SubJectVar.StartTime == pstSubJect->tStart)
		return 0;
	

	if (pstSubJect->cStat == SUBJECT_STAT_REQ)
	{
		iStartTime = pstSubJect->tReqEnd;
	}
	else if (pstSubJect->cStat == SUBJECT_STAT_START)
	{
		iStartTime = 0;
	}
	else
	{
		return 0;
	}

	iIdx = subject_player_info_find(pstEnv, pstPlayer);
	if (iIdx < 0)
	{
		subject_invite_player(pstEnv, pstPlayer, iStartTime, 1, 0);
	}
	else
	{
		if (pstSubJect->cStat == SUBJECT_STAT_START)
		{
			subject_get_top(pstEnv, &stTop);
		}
		else
		{
			stTop.Num = 0;
		}
		subject_info_notify(pstEnv, pstPlayer, &stTop, iIdx);
	}
	return 0;
}

int subject_player_info_del(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iIdx;
	SUBJECT *pstSubJect = &pstEnv->pstZoneObj->stSubJect;

	iIdx =  subject_player_info_find(pstEnv, pstPlayer);
	if (iIdx < 0)
		return -1;

	pstSubJect->iPlayer --;
	if (pstSubJect->iPlayer != iIdx)
	{
		pstSubJect->astSJPlayer[iIdx] = pstSubJect->astSJPlayer[pstSubJect->iPlayer];
	}
	return 0;
}

int subject_player_logout(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	subject_player_info_del(pstEnv, pstPlayer);
	return 0;
}

static int subject_qsort_cmp(const void *p1, const void *p2)
{
	SUBJECTPLAYER* pstP1 = (SUBJECTPLAYER *)p1;
	SUBJECTPLAYER* pstP2 = (SUBJECTPLAYER *)p2;

	return pstP2->iInteg - pstP1->iInteg;
}

int subject_check_stat(ZONESVRENV* pstEnv,GameEventInst *pstEvent)
{
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	SUBJECT *pstSubJect = &pstEnv->pstZoneObj->stSubJect;

	if (pstEvent->iEventID != SUBJECT_EVENT_ID)
	{
		return 0;
	}

	switch(pstSubJect->cStat)
	{
	case 0:
		
		pstSubJect->cStat 		= SUBJECT_STAT_REQ;
		pstSubJect->tStart 	= iCurr;
		pstSubJect->tReqEnd 	= iCurr + 60;			
		subject_invite_player(pstEnv, NULL,  pstSubJect->tReqEnd, 0, 0);
		break;
	case SUBJECT_STAT_REQ:
		if (iCurr >= pstSubJect->tReqEnd)
		{
			pstSubJect->cStat 		= SUBJECT_STAT_START;
			subject_info_notify_by_world(pstEnv);
		}
		break;
	case SUBJECT_STAT_START:
		if (iCurr >= pstSubJect->tResEnd )
		{
			if (pstSubJect->cSubJectNum >= SUBJECT_MAX_NUM)
			{
				pstSubJect->cStat = SUBJECT_STAT_END;
			}
			
			// 排序
			qsort(&pstSubJect->astSJPlayer[0], pstSubJect->iPlayer, 
				sizeof(pstSubJect->astSJPlayer[0]), subject_qsort_cmp);
			
			subject_info_notify_by_world(pstEnv);
		}
		break;
	case SUBJECT_STAT_END:
		subject_get_top_award_libao(pstEnv);
		z_game_event_del (pstEnv, pstEvent);
		subject_invite_player(pstEnv, NULL,  0, 0, 1);
		shopping_add_bulletin(pstEnv->pstAppCtx,pstEnv,20,10, 
			LJY_SUBJECT01);
		memset(pstSubJect, 0, sizeof(*pstSubJect));
		break;
	default:
		return 0;
		break;
	}

	return 0;
}

int subject_player_info_find(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	SUBJECT *pstSubJect = &pstEnv->pstZoneObj->stSubJect;
	
	for (i=0; i<pstSubJect->iPlayer; i++)
	{
		if (pstSubJect->astSJPlayer[i].ullRoleID == 
						pstPlayer->stRoleData.RoleID)
		{
			return i;
		}
	}
	return -1;
}

SUBJECTDEF* subject_def_find(ZONESVRENV* pstEnv, int iID)
{
	SUBJECTDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.ID = iID;
	iIndex = bsearch_int(&stDef, pstObj->sSubJectDef, pstObj->iSubJectDef, SUBJECT_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (SUBJECTDEF *)&pstObj->sSubJectDef[iIndex];
	}
	return NULL;
}


int subject_player_is_join(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	SUBJECTINFO *pstSubInfo = &pstPlayer->stRoleData.MiscInfo.SubJectInfo;
	SUBJECT *pstSubJect = &pstEnv->pstZoneObj->stSubJect;
	int t =pstSubInfo->SubJectVar.StartTime;

	if (	pstSubJect->cStat != SUBJECT_STAT_REQ &&
		pstSubJect->cStat != SUBJECT_STAT_START)
		return 0;

	if ( t == pstSubJect->tStart && !IsSameDay(pstSubInfo->ExitTime, pstEnv->pstAppCtx->stCurr.tv_sec))
		return 1;

	return 0;
}

int subject_clt_join(ZONESVRENV* pstEnv, Player *pstPlayer, SUBJECTJOIN *pstJoin)
{
	int t;
	int iFirst = 1;
	SUBJECTINFO *pstSubInfo = &pstPlayer->stRoleData.MiscInfo.SubJectInfo;
	SUBJECTPLAYER *pstSubPlayer;
	int iIdx;
	SUBJECTTOP stTop;
	SUBJECT *pstSubJect = &pstEnv->pstZoneObj->stSubJect;
	
	if (	pstSubJect->cStat != SUBJECT_STAT_REQ &&
		pstSubJect->cStat != SUBJECT_STAT_START)
		return -1;

	if (subject_player_info_find(pstEnv, pstPlayer) >= 0)
		return -1;

	if (pstSubJect->iPlayer >= MAX_PLAYER)
		return -1;

	if (pstPlayer->stRoleData.Level < SUBJECT_MIN_LEVEL)
	{
		return -1;
	}

	t =pstSubInfo->SubJectVar.StartTime;
	pstSubPlayer = &pstSubJect->astSJPlayer[pstSubJect->iPlayer];
	pstSubPlayer->iInteg = 0;

	// 同一次活动中退出在参加要累计积分
	if (t > 0 && t == pstSubJect->tStart)
	{
		pstSubPlayer->iInteg = pstSubInfo->SubJectVar.SubInteg;
		iFirst = 0;	
	}
	
	pstSubPlayer->iMemID = pstPlayer->iMemID;
	pstSubPlayer->ullRoleID = pstPlayer->stRoleData.RoleID;
	pstSubJect->iPlayer ++;
	

	if (iFirst)
	{
		memset(pstSubInfo, 0, sizeof(*pstSubInfo));
		pstSubInfo->SubJectVar.StartTime = pstSubJect->tStart;

		iIdx = pstSubJect->iPlayer -1;
	}
	else
	{
		qsort(&pstSubJect->astSJPlayer[0], pstSubJect->iPlayer, 
			sizeof(pstSubJect->astSJPlayer[0]), subject_qsort_cmp);

		iIdx = subject_player_info_find(pstEnv, pstPlayer);
		if (iIdx < 0)
			return -1;
	}

	// 活动已经开始了,获取下前20名信息，否则不发
	if (pstSubJect->cStat == SUBJECT_STAT_START)
	{
		subject_get_top(pstEnv, &stTop);
	}
	else
	{
		stTop.Num = 0;
	}

	subject_info_notify(pstEnv, pstPlayer, &stTop, iIdx);
	return 0;
}

int subject_clt_exit(ZONESVRENV* pstEnv, Player *pstPlayer, SUBJECTEXIT *pstExit)
{
	SUBJECT *pstSubJect = &pstEnv->pstZoneObj->stSubJect;
	
	if (	pstSubJect->cStat != SUBJECT_STAT_REQ &&
		pstSubJect->cStat != SUBJECT_STAT_START)
		return -1;

	subject_player_info_del(pstEnv, pstPlayer);

	pstPlayer->stRoleData.MiscInfo.SubJectInfo.ExitTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	return 0;
}

int subject_clt_commit(ZONESVRENV* pstEnv, Player *pstPlayer, SUBJECTCOMMIT*pstCommit)
{
	int iIdx;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	int iSubJectTime;	// 答题时间
	int iInteg = 0;	// 积分
	SUBJECTINFO *pstSubInfo = &pstPlayer->stRoleData.MiscInfo.SubJectInfo;
	SUBJECT *pstSubJect = &pstEnv->pstZoneObj->stSubJect;
	
	if (pstSubJect->cStat != SUBJECT_STAT_START)
		return -1;

	if (	pstCommit->Idx != pstSubJect->cSubJectNum ||
		pstCommit->ID != pstSubJect->iSubJectID)
		return -1;

	// 此题已经答过了
	if (pstSubJect->cSubJectNum == pstSubInfo->SubJectVar.SubJectIdx)
		return -1;
	
	iIdx =  subject_player_info_find(pstEnv, pstPlayer);
	if (iIdx < 0)
		return -1;
	
	// 记一下第几题,不能重复回答
	pstSubInfo->SubJectVar.SubJectIdx = pstSubJect->cSubJectNum;
	
	if (pstCommit->IsStar == 1)
	{
		if (pstSubInfo->SubJectVar.Star >= 3)
			return -1;
		
		iSubJectTime = pstSubJect->tLockEnd +1;
		pstSubInfo->SubJectVar.Star++;
		pstSubInfo->SubJectVar.CurrIdx = pstSubJect->iSuccIdx +1;
	}
	else
	{
		// 记一下本次回答的答案，客户端上线下线要返回
		pstSubInfo->SubJectVar.CurrIdx = pstCommit->KeyIdx +1; 

		// 错了不给分
		if (pstCommit->KeyIdx != pstSubJect->iSuccIdx)
		{
			pstSubInfo->SubJectVar.Fail ++;
			return -1;
		}
		
		if (iCurr <= pstSubJect->tLockEnd)
		{
			iSubJectTime = pstSubJect->tLockEnd +1;
		}	
		else if (iCurr > pstSubJect->tResEnd)
		{
			iSubJectTime = pstSubJect->tResEnd;
		}
		else
		{
			iSubJectTime = iCurr;
		}	
	}

	iInteg = 11 - (iSubJectTime-pstSubJect->tLockEnd);
	if (iInteg <= 0)
		iInteg = 1;

	pstSubJect->astSJPlayer[iIdx].iInteg += iInteg;
	pstSubInfo->SubJectVar.Succ ++;
	pstSubInfo->SubJectVar.SubInteg = pstSubJect->astSJPlayer[iIdx].iInteg;
	return 0;
}

int subject_clt_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	SUBJECTCLT *pstClt = &pstCsPkg->Body.SubJectClt;

	if (INST_MODE_ZONE_SVR != pstEnv->iInstMode)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
		return 0;
	}
	
	switch(pstClt->Cmd)
	{
	case SUBJECT_CLT_OP_JOIN:
		return subject_clt_join(pstEnv, pstPlayer, &pstClt->Data.Join);
		break;
	case SUBJECT_CLT_OP_EXIT:
		return subject_clt_exit(pstEnv, pstPlayer, &pstClt->Data.Exit);
		break;
	case SUBJECT_CLT_OP_COMMIT:
		return subject_clt_commit(pstEnv, pstPlayer, &pstClt->Data.Commit);
		break;
	}
	return 0;
}

int subject_type_id(ZONESVRENV* pstEnv, int iID, int iType)
{
	int i;
	SUBJECTLIST *pstList = &pstEnv->stSubList;
	SUBJECTTYPE *pstType = NULL;

	for (i=0; i<pstList->iNum; i++)
	{
		if (pstList->stType[i].cType == iType)
		{
			pstType = &pstList->stType[i];
			break;
		}
	}

	if (i>=pstList->iNum )
	{
		if (i >= MAX_SUBJECT_TYPE_NUM)
		{
			return -1;
		}
		pstType = &pstList->stType[pstList->iNum];
		pstType->cType = iType;
		pstList->iNum ++;
	}

	if (!pstType)
		return -3;

	if (pstType->cNum >= MAX_SUBJECT_TYPE_COUNT)
	{
		return -2;
	}
	pstType->iID[(int)pstType->cNum] = iID;
	pstType->cNum ++;
	return 0;
}


// 获取题目
SUBJECTDEF* subject_get_theme(ZONESVRENV* pstEnv)
{
	SUBJECTTYPEDEF *pstTypeDef;
	SUBJECTTYPE *pstType = NULL;
	SUBJECTTYPE stType;
	//SUBJECTDEF *pstSubDef;
	int i,j;
	SUBJECT *pstSubJect = &pstEnv->pstZoneObj->stSubJect;
	int iAlReady = pstSubJect->cSubJectNum;


	pstTypeDef = subject_type_def_find( pstEnv, pstSubJect->cSubJectNum+1);
	if (!pstTypeDef)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "subject_get_theme error 1"); 
		return NULL;
	}
		

	for (i=0; i<pstEnv->stSubList.iNum; i++)
	{
		if (pstEnv->stSubList.stType[i].cType == pstTypeDef->Type)
		{
			pstType = &pstEnv->stSubList.stType[i];
			break;
		}
	}

	if (!pstType)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "subject_get_theme error 2  Type=%d, Num=%d",
					pstTypeDef->Type, pstSubJect->cSubJectNum+1); 
		return NULL;
	}

	stType = *pstType;
	for (i=stType.cNum-1; i>=0; i--)
	{
		for (j=0; j<iAlReady; j++)
		{
			if (stType.iID[i] == pstSubJect->iAlready[j])
			{
				stType.cNum--;
				if (stType.cNum != i)
				{
					stType.iID[i] = stType.iID[(int)stType.cNum];
				}
			}
		}
	}

	if (stType.cNum <= 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "subject_get_theme error 3 Type=%d", stType.cType); 
		return NULL;
	}

	return subject_def_find(pstEnv, stType.iID[RAND1(stType.cNum)]);
}

int subject_db_pre(ZONESVRENV* pstEnv)
{
	int i;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	SUBJECTDEF *pstDef;
	int iRet;

	memset(&pstEnv->stSubList, 0, sizeof(pstEnv->stSubList));
	for (i=0; i<pstObj->iSubJectDef; i++)
	{
		pstDef = (SUBJECTDEF*)&pstObj->sSubJectDef[i];
		if (pstDef->Succ < 1 || pstDef->Succ > 4)
		{
			printf("subject_db_pre:error 1\n");
			return -1;
		}

		if (pstDef->SubJect[0] == 0)
		{
			printf("subject_db_pre:error 2\n");
			return -1;
		}

		if (pstDef->Type <1 || pstDef->Type > MAX_SUBJECT_TYPE_NUM)
		{
			printf("subject_db_pre:error 3\n");
			return -1;
		}

		if (pstDef->SubJectKey[pstDef->Succ-1].Str[0] == 0)
		{
			printf("subject_db_pre:error 4\n");
			return -1;
		}

		iRet = subject_type_id(pstEnv,pstDef->ID, pstDef->Type);
		if (iRet < 0)
		{
			if (iRet == -1)
			{
				printf("subject_db_pre:error 5\n");
				return -1;
			}
			else if (iRet == -2)
			{
				printf("subject_db_pre:error 6\n");
				return -1;
			}
		}
	}
	return 0;
}

int subject_type_db_pre(ZONESVRENV* pstEnv)
{
	int i;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	SUBJECTTYPEDEF *pstDef;

	for (i=0; i<pstObj->iSubJectTypeDef; i++)
	{
		pstDef = (SUBJECTTYPEDEF*)&pstObj->sSubJectTypeDef[i];
		if (pstDef->Idx < 1 || pstDef->Idx > SUBJECT_MAX_NUM)
		{
			continue;
		}

		if (pstDef->Type < 0 || pstDef->Type > MAX_SUBJECT_TYPE_NUM)
		{
			printf("subject_type_db_pre:error \n");
			return -1;
		}
	}
	return 0;
}


SUBJECTTYPEDEF* subject_type_def_find(ZONESVRENV* pstEnv, int iIdx)
{
	SUBJECTTYPEDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Idx = iIdx;
	iIndex = bsearch_int(&stDef, pstObj->sSubJectTypeDef, pstObj->iSubJectTypeDef, SUBJECT_TYPE_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (SUBJECTTYPEDEF *)&pstObj->sSubJectTypeDef[iIndex];
	}
	return NULL;
}

int subject_get_top_award_libao(ZONESVRENV* pstEnv)
{
	int i;
	Player *pstPlayer;
	int Num = 0;
	SUBJECT *pstSubJect = &pstEnv->pstZoneObj->stSubJect;
	int j=0;
	int iAwardNum = 0;
	int iOdds = 0;
	ITEMDEF  *pstItemDef = NULL;
	char szText[256];
	ROLEGRID astRoleGrid[8];
	int k = 0;
	int iGridNum = 0;
	
	if (pstSubJect->cStat != SUBJECT_STAT_END)
	{
		return 0;
	}

	pstItemDef = z_find_itemdef(pstEnv, SUBJECT_LIBAO_ID);
	if(	!pstItemDef)
	{
		return 0;
	}

	// 排序
	qsort(&pstSubJect->astSJPlayer[0], pstSubJect->iPlayer, 
			sizeof(pstSubJect->astSJPlayer[0]), subject_qsort_cmp);

	for (i=0; i<pstSubJect->iPlayer; i++)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx,  pstEnv, pstSubJect->astSJPlayer[i].iMemID);
		if (pstPlayer)
		{
			Num ++;
			iAwardNum = 0;
	
			if(astAwardTop[j] < Num)
			{
				j++;
			}

			if(astAwardTop[j] >= Num)
			{
				if(j != SUBJECT_LIBAO_LEVEL-1)
				{
					iAwardNum = astAwardNum[j];
				}
				else
				{
					iOdds = RAND1(100);
					// 最底层名次的有10 % 概率获得
					if(iOdds < 10)
					{
						iAwardNum =astAwardNum[j];
					}
				}
			}
			
			if(iAwardNum > 0)
			{
				k = 0;
				iGridNum = 0;
				memset(&astRoleGrid,0,sizeof(ROLEGRID)*8);
				
				for(k=0;k<iAwardNum;k++)
				{
					if(0 > item_create(pstEnv->pstAppCtx, pstEnv, &astRoleGrid[iGridNum], SUBJECT_LIBAO_ID))
					{
						continue;
					}

					iGridNum++;
				}
				
				if(0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, astRoleGrid, iGridNum,0))
				{
					snprintf(szText, sizeof(szText), LJY_SUBJECT02, Num,iGridNum,pstItemDef->Name);
					
					player_sysmail_fast_send(pstEnv, pstPlayer->stRoleData.RoleName, 
								LJY_SUBJECT03, szText, 
								astRoleGrid, iGridNum, 0,LJY_MAIL2, 0);
					
					z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_SUBJECT_01);
				}
				else
				{
					package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, astRoleGrid, iGridNum, 0, IMPITEM_OPTYPE_SUBJECT);
				}

				if(Num == 1)
				{
					z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_SUBJECT_02,Num,iGridNum,pstItemDef->Name);
					z_sys_strf_broadcast(pstEnv, SYS_SHOW, ZONEERR_SUBJECT_03,pstPlayer->stRoleData.RoleName,iGridNum,pstItemDef->Name);
				}
			}
		}

		if (Num >= SUBJECT_MAX_AWARD_LIBAO_TOP)
				break;
	}
	
	return 0;
}

