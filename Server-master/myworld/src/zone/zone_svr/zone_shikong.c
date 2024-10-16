

#include "zone_shikong.h"
#include "zone_err.h"
#include "zone_clt.h"
#include "zone_map.h"
#include "zone_team.h"
#include "zone_player.h"
#include "zone_status.h"
#include "zone_span.h"

int g_EnterShiKong = 0;

/********* 时空深渊******绿野凶林**********/
/*等待队列与开启的时空副本一一对应*/
/************************************************/

//移除一个单位的排队队列
int shikong_queuers_remove_applyqueue(ShiKongApplyQueue *pstApplyQueue, int iQueuerNum, int *paiQueuer)
{
	int i = 0;
	int iNum = 0;

	if(iQueuerNum <= 0)
		return 0;
	
	for(i=0; i<pstApplyQueue->iApplyNum; i++)
	{
		if(pstApplyQueue->astMem[i].iMemID != paiQueuer[0])
		{
			continue;
		}

		if(iQueuerNum > 1 && i+iQueuerNum <= pstApplyQueue->iApplyNum)
		{
			iNum = iQueuerNum;
		}
		else
		{
			iNum = 1;
		}

		break;
	}

	if(i < pstApplyQueue->iApplyNum)
	{
		pstApplyQueue->iApplyNum -= iNum;
		if( i != pstApplyQueue->iApplyNum)
		{
			//还是要按顺序
			memmove(&pstApplyQueue->astMem[i], &pstApplyQueue->astMem[i+iNum], 
						sizeof(pstApplyQueue->astMem[0])*(pstApplyQueue->iApplyNum-i));
		}	
	}
	
	return 0;
}

//邀请进入UI
int shikong_invite_enter_notify(ZONESVRENV* pstEnv,Player *pstPlayer,CSINVITEENTER *pstInvite)
{
	CSPKG stPkg;
	CSSHIKONGSVR *pstShiKongSvr = &stPkg.Body.ShiKongSvr;
	CSINVITEENTER *pstInviteEnter = &pstShiKongSvr->Data.InviteEnter;

	pstShiKongSvr->Cmd = SHIKONG_SVR_OP_INVITE_ENTER;
	memcpy(pstInviteEnter,pstInvite,sizeof(*pstInviteEnter));
	
	Z_CSHEAD_INIT(&stPkg.Head, SHIKONG_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

//把一个单位的人员放入等待队列，并且从排队队列中清除
int shikong_queuers_inset_waitqueue(ZONESVRENV* pstEnv , ZONEPWORLD *pstPworld, 
									ZONEPWORLDINST *pstPworldInst,int iQueuerNum, int *paiQueuer)
{
	int i;
	Player *pstPlayer;
	CSINVITEENTER stInvite;
	ShiKongApplyQueue *pstApplyQueue = get_shikong_pworld_apply_queue(pstEnv, pstPworld->stPworldDef.PworldID);

	if (iQueuerNum <=0)
	{
		return 0;
	}

	if(!pstApplyQueue)
	{
		return -1;
	}
	else
	{
		//移除排队队列的一个单位
		shikong_queuers_remove_applyqueue(pstApplyQueue, iQueuerNum, paiQueuer);
	}

	i = iQueuerNum;
	while(i--)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, paiQueuer[i]);
		if(NULL == pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			continue;
		}	

		//放入等待队列
		if(0 == shikong_player2wait_queue(pstEnv, pstPworld, pstPworldInst->iPworldInstID, 
			pstPworldInst->iShiKongWaitQueueIdx, pstPlayer))
		{
			stInvite.PworldID = pstPworld->stPworldDef.PworldID;
			shikong_invite_enter_notify(pstEnv,pstPlayer,&stInvite);
		}
	}

	return 0;
}

//报名队列检查
int shikong_pworld_apply_queue_check(ZONESVRENV * pstEnv,ShiKongApplyQueue *pstApplyQueue)
{
	ZONEIDX stIdx;
	ZONEPWORLD *pstPworld;
	ZONEPWORLDINST *pstZonePworldInst;
	int i;
	int iQueuerNum = 0;
	int aQueuer[MAX_TEAM_MEMBER];
	int iUsePworldNum = 0;
	int iEnterWaitQueueIdx = 0;
	ShiKongWaitQueue *pstWaitQueue;
	ZONEPWORLD *pstZonePworld;
	ShiKongApplyQueue stApplyQueue;

	stIdx.iID = pstApplyQueue->iPworldID;
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(pstEnv->pstMapObj,&stIdx);
	if(!pstPworld)
	{
		return -1;
	}

	if(0 == is_shikong_pworld(pstPworld))
	{
		return -1;
	}

	stApplyQueue = *pstApplyQueue;
	
	for(i=0;i<stApplyQueue.iApplyNum;i++)
	{
		aQueuer[iQueuerNum++] = stApplyQueue.astMem[i].iMemID;

		//尽量一个队一起
		if(i+1 < stApplyQueue.iApplyNum && stApplyQueue.astMem[i].ullTeamWID > 0)
		{
			if(stApplyQueue.astMem[i].ullTeamWID == stApplyQueue.astMem[i+1].ullTeamWID)
			{
				continue;
			}
		}

		iUsePworldNum = shikong_get_wait_queue_use_num(pstEnv);
		if(iUsePworldNum < SHIKONG_PWORLD_OPEN_NUM)
		{
			// 12 人上限
			iEnterWaitQueueIdx = shikong_get_enter_pworld_idx(pstEnv,SHIKONG_PWORLD_MEM_NUM);
		}
		else
		{
			//18 人上限
			iEnterWaitQueueIdx = shikong_get_enter_pworld_idx(pstEnv,pstPworld->stPworldDef.PlayerNum);
		}

		if(iEnterWaitQueueIdx < 0)
		{
			//报名人数达到6人时，才会开启新副本
			if(pstApplyQueue->iApplyNum < MAX_TEAM_MEMBER)
			{
				return 0;
			}

			pstZonePworldInst = z_new_ploy_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstPworld, pstPworld->stPworldDef.Timeout*60,0);
			if(pstZonePworldInst == NULL)
			{
				return -1;
			}
		}
		else
		{
			pstWaitQueue = &pstEnv->pstShiKongQueue->astWaitQueue[iEnterWaitQueueIdx];
			pstZonePworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstWaitQueue->iPworldInstID);
			if(pstZonePworldInst == NULL)
			{
				return -1;
			}
	
			pstZonePworld = z_find_pworld(pstEnv->pstMapObj,&pstZonePworldInst->stPworldIdx);
			if(pstZonePworld == NULL)
			{
				return -1;
			}
		}	

		//把一个单位放进去
		shikong_queuers_inset_waitqueue(pstEnv, pstPworld, pstZonePworldInst,iQueuerNum, aQueuer);
			
		iQueuerNum = 0;
	}
	
	return 0;
}

int shikong_exit_queue_notify(ZONESVRENV * pstEnv,Player *pstPlayer,CSEXITSHIKONGQUEUE *pstExitQueue)
{
	CSPKG stPkg;
	CSSHIKONGSVR *pstShikongSvr = &stPkg.Body.ShiKongSvr;
	CSEXITSHIKONGQUEUE *pstExit = &pstShikongSvr->Data.ExitShiKongQueue;

	memcpy(pstExit,pstExitQueue,sizeof(*pstExit));

	pstShikongSvr->Cmd = SHIKONG_SVR_OP_EXIT_SHIKONG_QUEUE;
	
	Z_CSHEAD_INIT(&stPkg.Head, SHIKONG_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

//等待队列检查
int shikong_pworld_wait_queue_check(ZONESVRENV * pstEnv,ShiKongWaitQueue *pstWaitQueue)
{
	int i;
	Player *pstPlayer;
	int iMemNum = 0;
	ZONEIDX stIdx;
	ZONEPWORLD *pstZonePwolrd = NULL;
	CSEXITSHIKONGQUEUE stExitQueue;
	
	iMemNum = pstWaitQueue->iMemNum;
	if(iMemNum <= 0)
	{
		return 0;
	}

	stIdx.iID = pstWaitQueue->iPworldID;
	stIdx.iPos = -1;
	pstZonePwolrd = z_find_pworld(pstEnv->pstMapObj,&stIdx);
	if(!pstZonePwolrd)
	{
		return 0;
	}

	if(0 == is_shikong_pworld(pstZonePwolrd))
	{
		return -1;
	}

	for(i = iMemNum-1; i>=0; i--)
	{
		if (pstWaitQueue->astMem[i].tValidTime >=  pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			continue;
		}

		//超过倒计时不进入副本的踢出等待队列
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstWaitQueue->astMem[i].iMemID);
		if(pstPlayer)
		{
			shikong_player_online_remove_queue(pstEnv, pstPlayer);

			memset(&stExitQueue,0,sizeof(stExitQueue));
			stExitQueue.PworldID = pstWaitQueue->iPworldID;
			stExitQueue.type = SHIKONG_WAIT_QUEUE;
			shikong_exit_queue_notify(pstEnv,pstPlayer,&stExitQueue);	
			
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_SHIKONG_1,pstZonePwolrd->stPworldDef.Name);
		}
		
		pstWaitQueue->iMemNum--;

		if(i != pstWaitQueue->iMemNum)
		{
			memcpy(&pstWaitQueue->astMem[i], &pstWaitQueue->astMem[pstWaitQueue->iMemNum], 
				sizeof(pstWaitQueue->astMem[i]));
		}
	}
	
	return 0;
}

int shikong_tick(ZONESVRENV * pstEnv)
{
	static unsigned int iCount = 0;
	int i = 0;
	ShiKongQueue *pstShiKongQueue =  pstEnv->pstShiKongQueue;
	ShiKongWaitQueue  *pstWaitQueue = NULL;
	ShiKongApplyQueue *pstApplyQueue = NULL;

	//每2秒
	if(iCount % 2 == 0)
	{
		for(i=0;i<MAX_SHIKONG_APPLY_QUEUE;i++)
		{
			pstApplyQueue = &pstShiKongQueue->astApplyQueue[i];
			
			if(pstApplyQueue->iPworldID == 0 || pstApplyQueue->iApplyNum == 0)
			{
				continue;
			}

			shikong_pworld_apply_queue_check(pstEnv,pstApplyQueue);
		}

		for(i=0;i<MAX_SHIKONG_WAIT_QUEUE;i++)
		{
			pstWaitQueue = &pstShiKongQueue->astWaitQueue[i];
		
			if(pstWaitQueue->iPworldInstID == 0)
			{
				continue;
			}

			shikong_pworld_wait_queue_check(pstEnv,pstWaitQueue);
		}
	}

	iCount++;
	return 0;
}

//销毁副本的同时也需清掉等待队列
int shikong_clear_wait_queue(ZONESVRENV * pstEnv,int iWaitQueueIdx,int iPworldInstID)
{
	int i, iCount = 0;
	Player *pstPlayer;
	Player *apstPlayers[MAX_SHIKONG_WAIT_QUEUE_MEM];
	CSPKG stPkg;
	CSSHIKONGSVR *pstShikongSvr = &stPkg.Body.ShiKongSvr;
	CSEXITSHIKONGQUEUE *pstExit = &pstShikongSvr->Data.ExitShiKongQueue;
	ShiKongWaitQueue *pstWaitQueue = NULL;

	pstWaitQueue = 	get_shikong_pworld_wait_queue(pstEnv,iWaitQueueIdx,iPworldInstID);
	if(NULL == pstWaitQueue)
	{
		return -1;
	}
	
	for(i = 0; i < pstWaitQueue->iMemNum; i++)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv,pstWaitQueue->astMem[i].iMemID);
		if(NULL == pstPlayer || (pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN && pstPlayer->eStatus != PLAYER_STATUS_ROLE_NOTHING))
		{
			continue;
		}
		
		shikong_player_online_remove_queue(pstEnv, pstPlayer);
		
		apstPlayers[iCount++] = pstPlayer;
	}

	if(iCount > 0)
	{
		pstShikongSvr->Cmd = SHIKONG_SVR_OP_EXIT_SHIKONG_QUEUE;
		pstExit->PworldID = pstWaitQueue->iPworldID;
		pstExit->type = SHIKONG_WAIT_QUEUE;
		
		Z_CSHEAD_INIT(&stPkg.Head, SHIKONG_SVR);
		z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayers,
		                                iCount, &stPkg);
	}

	memset(pstWaitQueue, 0, sizeof(*pstWaitQueue));
	
	return 0;
}

//得到等待队列的使用个数，也就是得到时空副本的开启数量
int shikong_get_wait_queue_use_num(ZONESVRENV * pstEnv)
{
	int i = 0;
	int iCount = 0;
	ShiKongQueue *pstShiKongQueue = pstEnv->pstShiKongQueue;
	ShiKongWaitQueue *pstWaitQueue;
	
	for(i=0;i<MAX_SHIKONG_WAIT_QUEUE;i++)
	{
		pstWaitQueue = &pstShiKongQueue->astWaitQueue[i];
		
		if(pstWaitQueue->iPworldInstID != 0)
		{
			iCount++;
		}
	}

	return iCount;
}

//得到未使用的等待队伍索引
int shikong_get_wait_queue_free_idx(ZONESVRENV * pstEnv)
{
	int i = 0;
	ShiKongQueue *pstShiKongQueue = pstEnv->pstShiKongQueue;
	ShiKongWaitQueue *pstWaitQueue;
	
	for(i=0;i<MAX_SHIKONG_WAIT_QUEUE;i++)
	{
		pstWaitQueue = &pstShiKongQueue->astWaitQueue[i];
		
		if(pstWaitQueue->iPworldInstID == 0)
		{
			return i;
		}
	}
	
	return -1;
}

//得到未使用的报名队伍索引
int shikong_get_apply_queue_free_idx(ZONESVRENV * pstEnv)
{
	int i = 0;
	ShiKongQueue *pstShiKongQueue = pstEnv->pstShiKongQueue;
	ShiKongApplyQueue *pstApplyQueue;
	
	for(i=0;i<MAX_SHIKONG_APPLY_QUEUE;i++)
	{
		pstApplyQueue = &pstShiKongQueue->astApplyQueue[i];
		
		if(pstApplyQueue->iPworldID == 0)
		{
			return i;
		}
	}
	
	return -1;
}

//创建一条新的报名队列
ShiKongApplyQueue *new_apply_queue(ZONESVRENV * pstEnv,int iPworldID)
{
	int idx = 0;
	ShiKongQueue *pstShiKongQueue = pstEnv->pstShiKongQueue;
	idx = shikong_get_apply_queue_free_idx(pstEnv);

	if(idx < 0)
	{
		return NULL;
	}

	pstShiKongQueue->astApplyQueue[idx].iPworldID = iPworldID;
	pstShiKongQueue->astApplyQueue[idx].iApplyNum = 0;
	
	return &pstShiKongQueue->astApplyQueue[idx];
}

//得到能进入副本的索引，也就是能进入等待队列的索引
int shikong_get_enter_pworld_idx(ZONESVRENV * pstEnv,int iMemNum)
{
	int i = 0;
	ShiKongQueue *pstShiKongQueue = pstEnv->pstShiKongQueue;
	ShiKongWaitQueue *pstWaitQueue;
	ZONEPWORLDINST *pstZonePworldInst = NULL;
	ZONEPWORLD *pstZonePworld = NULL;
	int iCount = 0;
	int iEnterTime;
	
	for(i=0;i<MAX_SHIKONG_WAIT_QUEUE;i++)
	{
		pstWaitQueue = &pstShiKongQueue->astWaitQueue[i];
		
		if(pstWaitQueue->iPworldInstID == 0)
		{
			continue;
		}

		pstZonePworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstWaitQueue->iPworldInstID);
		if(pstZonePworldInst == NULL)
		{
			continue;
		}

		pstZonePworld = z_find_pworld(pstEnv->pstMapObj,&pstZonePworldInst->stPworldIdx);
		if(pstZonePworld == NULL)
		{
			continue;
		}
		
		//可进入的有效时间
		iEnterTime = get_battle_enter_validtime(pstZonePworld);
			
		if(!pstZonePworldInst->bOver 
			&& pstZonePworldInst->tStart + iEnterTime >= pstEnv->pstAppCtx->stCurr.tv_sec &&
			pstZonePworldInst->iPlayer + pstWaitQueue->iMemNum < iMemNum)
		{
			//还可以进人
			return i;
		}
		else
		{
			iCount++;
		}
	}

	//都满了
	if(iCount > 0)
	{
		return -1;
	}
	//一个副本都没
	else
	{
		return -2;
	}
}

//获得指定副本的报名队列
ShiKongApplyQueue *get_shikong_pworld_apply_queue(ZONESVRENV* pstEnv, int iPworldID)
{
	int i;
	ShiKongApplyQueue *pstApplyQueue;
	ShiKongQueue *pstShiKongQueue = pstEnv->pstShiKongQueue;

	//副本排队队列
	for(i = 0; i<MAX_SHIKONG_APPLY_QUEUE; i++)
	{
		pstApplyQueue = &pstShiKongQueue->astApplyQueue[i];

		if(pstApplyQueue->iPworldID == 0)
		{
			continue;
		}

		if(pstApplyQueue->iPworldID == iPworldID)
		{
			return pstApplyQueue;
		}
	}
	
	return NULL;
}

//角色的队列信息
ShiKongRoleQueueInfo *shikong_get_player_online_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID)
{
	if (pstPlayer->stOnline.stShiKongQueueInfo.iPworldID == iPworldID &&
		pstPlayer->stOnline.stShiKongQueueInfo.iQueueType > 0)
	{
		return &pstPlayer->stOnline.stShiKongQueueInfo;
	}

	return NULL;
}

//玩家转入等待队列
int shikong_player2wait_queue(ZONESVRENV *pstEnv, ZONEPWORLD *pstPworld, int iPworldInstID, 
									int iWaitQueueIdx, Player *pstPlayer)
{
	ShiKongWaitQueue *pstWaitQueue;
	ShiKongQueue *pstShiKongQueue = pstEnv->pstShiKongQueue;
	ShiKongRoleQueueInfo *pstRoleQueue;

	if (iWaitQueueIdx <0 ||iWaitQueueIdx >= MAX_SHIKONG_WAIT_QUEUE)
	{
		return -1;
	}

	pstWaitQueue = &pstShiKongQueue->astWaitQueue[iWaitQueueIdx];
	if (pstWaitQueue->iMemNum >= pstPworld->stPworldDef.PlayerNum)
	{
		return -1;
	}

	//自己身上有没有
	pstRoleQueue = shikong_get_player_online_queue(pstEnv, pstPlayer, pstPworld->stPworldDef.PworldID);
	if(NULL == pstRoleQueue || pstRoleQueue->iQueueType != SHIKONG_APPLY_QUEUE)
	{
		return -1;
	}

	pstRoleQueue->iQueueType = SHIKONG_WAIT_QUEUE;
	pstRoleQueue->iPworldInstID = iPworldInstID;
	pstRoleQueue->iWaitQueueIdx = iWaitQueueIdx;
	pstRoleQueue->iPworldID = pstPworld->stPworldDef.PworldID;

	memset(&pstWaitQueue->astMem[pstWaitQueue->iMemNum],0,sizeof(pstWaitQueue->astMem[pstWaitQueue->iMemNum]));
	pstWaitQueue->astMem[pstWaitQueue->iMemNum].iMemID = pstPlayer->iMemID;
	pstWaitQueue->astMem[pstWaitQueue->iMemNum].tValidTime = 
		pstEnv->pstAppCtx->stCurr.tv_sec + INVITE_ENTER_SHIKONG_PWORLD_TIME;
	
	pstWaitQueue->iMemNum++;
	return 0;
}

//点击进入时空深渊副本
int player_enter_shikong_pworld(ZONESVRENV *pstEnv,Player *pstPlayer,int iPworldID)
{
	ShiKongRoleQueueInfo *pstRoleQueue;
	ZONEPWORLDINST *pstPworldInst;
	ZONEPWORLD *pstPworld;
	ZONEIDX stIdx;

	pstRoleQueue = shikong_get_player_online_queue(pstEnv, pstPlayer, iPworldID);
	if(NULL == pstRoleQueue || pstRoleQueue->iQueueType != SHIKONG_WAIT_QUEUE)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_SHIKONG_3);
		return -1;
	}

	stIdx.iID = iPworldID;
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (NULL == pstPworld)
	{
		return -1;
	}

	if(pstPlayer->stOnline.State &	CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_MAP120);
		return -1;
	}

	if(pstPlayer->stOnline.State & CS_STAT_FIGHT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_SHIKONG_15);
		return -1;
	}
	
	if (pstPlayer->stOnline.stExchgOnline.bExchgStat > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SHIKONG_4);
		return -1;
	}

	if (pstPlayer->stOnline.State & CS_STAT_STALL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SHIKONG_5);
		return -1;
	}

	if(CS_STAT_RIDE_MON_MACHINE & pstPlayer->stOnline.State)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SHIKONG_6);
		return -1;
	}

	if (is_shikong_pworld(pstPworld))
	{			
		pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstRoleQueue->iPworldInstID);
		if(NULL == pstPworldInst)
		{
			return -1;
		}

		if (pstPworldInst->bOver)
		{
			return -1;
		}

		g_EnterShiKong = 1;
		if (0 > z_player_enter_pworld_by_inst(pstEnv, pstPlayer, pstPworld, pstPworldInst))
		{
			g_EnterShiKong = 0;
			return -1;
		}

		g_EnterShiKong = 0;
	}
	
	shikong_exit_pworld_queue(pstEnv, pstPlayer, iPworldID, 1);
	return 0;
}

//退出副本排队、等待队列
int shikong_exit_pworld_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID, int fNotify)
{
	CSEXITSHIKONGQUEUE stExitQueue;
	ShiKongRoleQueueInfo *pstRoleQueue;
	int iQueueType = 0;

	pstRoleQueue = shikong_get_player_online_queue(pstEnv, pstPlayer, iPworldID);
	if(NULL == pstRoleQueue)
	{
		return -1;
	}

	if(pstRoleQueue->iQueueType == SHIKONG_APPLY_QUEUE)
	{	
		ShiKongApplyQueue *pstApplyQueue = get_shikong_pworld_apply_queue(pstEnv,iPworldID);
	
		if(!pstApplyQueue)
		{
			goto end;
		}
		
		shikong_player_remove_apply_queue(pstEnv, pstPlayer,pstApplyQueue);
	}
	else if (pstRoleQueue->iQueueType == SHIKONG_WAIT_QUEUE)
	{
		shikong_player_remove_wait_queue(pstEnv, pstPlayer, pstRoleQueue->iWaitQueueIdx,pstRoleQueue->iPworldInstID);
	}

end:
	iQueueType = pstRoleQueue->iQueueType;
	shikong_player_online_remove_queue(pstEnv, pstPlayer);
	
	if(fNotify)
	{			
		stExitQueue.PworldID = iPworldID;
		stExitQueue.type = iQueueType;
		shikong_exit_queue_notify(pstEnv,pstPlayer,&stExitQueue);
	}

	return 0;
}

//从报名队列中剔除该玩家
int shikong_player_remove_apply_queue(ZONESVRENV* pstEnv, Player *pstPlayer,ShiKongApplyQueue *pstApplyQueue)
{
	int i;
	
	for(i = pstApplyQueue->iApplyNum-1; i>=0; i--)
	{
		if (pstApplyQueue->astMem[i].iMemID == pstPlayer->iMemID)
		{
			break;
		}
	}

	if(i >= 0)
	{
		pstApplyQueue->iApplyNum--;

		if(i != pstApplyQueue->iApplyNum)
		{
			//还是要按顺序
			memmove(&pstApplyQueue->astMem[i], &pstApplyQueue->astMem[i+1], 
				sizeof(pstApplyQueue->astMem[0])*(pstApplyQueue->iApplyNum-i));
		}
	}
	
	return 0;
}

//从等待队列中剔除该玩家
int shikong_player_remove_wait_queue(ZONESVRENV* pstEnv, Player *pstPlayer, int iWaitIdx,int iPworldInstID)
{
	ShiKongWaitQueue *pstWaitQueue = NULL;
	int i;

	pstWaitQueue = get_shikong_pworld_wait_queue(pstEnv, iWaitIdx, iPworldInstID);
	if(NULL == pstWaitQueue)
	{
		return -1;
	}

	for(i = pstWaitQueue->iMemNum-1; i>=0; i--)
	{	
		if(pstWaitQueue->astMem[i].iMemID == pstPlayer->iMemID)
		{
			break;
		}
	}

	if(i <0)
	{
		return -1;
	}

	pstWaitQueue->iMemNum--;

	if(i != pstWaitQueue->iMemNum)
	{
		memcpy(&pstWaitQueue->astMem[i], &pstWaitQueue->astMem[pstWaitQueue->iMemNum], 
			sizeof(pstWaitQueue->astMem[0]));
	}
	
	return 0;
}

//获得时空副本的等待队列
ShiKongWaitQueue *get_shikong_pworld_wait_queue(ZONESVRENV* pstEnv, int iWaitQueueIdx, int iPworldInstID)
{
	ShiKongWaitQueue *pstWaitQueue;
	ShiKongQueue *pstShiKongQueue = pstEnv->pstShiKongQueue;

	if (iWaitQueueIdx <0 ||iWaitQueueIdx >= MAX_SHIKONG_WAIT_QUEUE)
	{
		return NULL;
	}

	pstWaitQueue = &pstShiKongQueue->astWaitQueue[iWaitQueueIdx];
	if(pstWaitQueue->iPworldInstID != iPworldInstID)
	{
		return NULL;
	}

	return pstWaitQueue;
}

//离开队列角色信息同步
int shikong_player_online_remove_queue(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	pstPlayer->stOnline.stShiKongQueueInfo.iQueueType = 0;
	pstPlayer->stOnline.stShiKongQueueInfo.iWaitQueueIdx = 0;
	
	return 0;
}

//检查报名队伍
int shikong_check_apply_team(ZONESVRENV *pstEnv,Player *pstPlayer, ZONEPWORLD *pstZonePworld,
				Player** apstPlayer, int *piPlayer)
{
	int i;
	int iRet = 0;
	ZoneTeam *pstZoneTeam;
	TEAMMEMBER *pstCap;
	Player *pstTeamPlayer;
	SpanOL *pstSpanOnline;
	char szTempName[ROLE_NAME_LEN];

	*piPlayer = 0;
	 
	pstZoneTeam = player_team_get(pstEnv,pstPlayer);
	if (NULL == pstZoneTeam)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_TIP, ZONEERR_PWORLD122);
		return -1;
	}
	
	pstCap = player_team_get_captain(pstZoneTeam);
	if (!pstCap)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_TIP, ZONEERR_PWORLD123);
		return -1;
	}

	if (strcmp(pstCap->MemberName, pstPlayer->stRoleData.RoleName) != 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_TIP, ZONEERR_PWORLD124);
		return -1;
	}
	
	pstSpanOnline = &pstPlayer->stOnline.stSpanOnline;
	if(pstSpanOnline->stApply.iSpartaApply == SPAN_APPLY_ING)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHIKONG_13);
		return -1;
	}

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstTeamPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (pstTeamPlayer)
		{			
			STRNCPY(szTempName, pstTeamPlayer->stRoleData.RoleName, CCH(szTempName));
			battle_rolename_remove_prefix(szTempName);
	
			if (z_player_find_status(pstTeamPlayer, STATUS_ID_RUNAWAY) )
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, 
					ZONEERR_PWORLD126 ,szTempName);
				iRet = -1;
				continue;
			}
			
			if (pstTeamPlayer->stRoleData.Level > pstZonePworld->stPworldDef.MaxLv ||
				pstTeamPlayer->stRoleData.Level < pstZonePworld->stPworldDef.MinLv)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_TIP, ZONEERR_SHIKONG_7,szTempName);
				iRet = -1;
				continue;
			}

			if( pstTeamPlayer->eStatus == PLAYER_STATUS_ROLE_NOTHING)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_TIP, ZONEERR_PWORLD128,szTempName);
				iRet = -1;
				continue;
			}

			if( pstTeamPlayer->stRoleData.Map >= PWORLD_MAPID_BASE )
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_TIP, ZONEERR_PWORLD129, szTempName);
				iRet = -1;
				continue;
			}

			if (pstTeamPlayer->stRoleData.MiscInfo.PkInfo.PkVal >= PLAYER_PK_PUBLIC_ENEMY &&
				player_is_zuolao(pstTeamPlayer))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHIKONG_9,szTempName);
				iRet = -1;
				continue;
			}

			if (pstTeamPlayer->stRoleData.Map == SHZ_READY_MAP_ID || pstTeamPlayer->stRoleData.Map == ZHZ_READY_MAP_ID ||
				pstTeamPlayer->stRoleData.Map == JNZ_READY_MAP_ID || pstTeamPlayer->stRoleData.Map == LHS_READY_MAP_ID ||
				pstTeamPlayer->stRoleData.Map == LSZ_READY_MAP_ID || pstTeamPlayer->stRoleData.Map == HMZ_READY_MAP_ID ||
				pstTeamPlayer->stRoleData.Map == SYZ_READY_MAP_ID || pstTeamPlayer->stRoleData.Map == YLS_READY_MAP_ID ||
				(GET_PWORLDID(pstTeamPlayer->stRoleData.Map) == DIANFENG_PWORLD_ID))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHIKONG_10,szTempName);
				iRet = -1;
				continue;
			}

			pstSpanOnline = &pstTeamPlayer->stOnline.stSpanOnline;
			if(pstSpanOnline->stApply.iSpartaApply == SPAN_APPLY_ING)
			{
				z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHIKONG_8,
					szTempName);
				iRet = -1;
				continue;
			}

			if(0 > player_if_in_span_map(pstEnv,pstTeamPlayer))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHIKONG_14,szTempName);
				iRet = -1;
				continue;
			}
		
			apstPlayer[(*piPlayer) ++] = pstTeamPlayer;
		}
		else
		{
			STRNCPY(szTempName, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName, CCH(szTempName));
			battle_rolename_remove_prefix(szTempName);
			
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_TIP, ZONEERR_SHIKONG_14, szTempName);
			iRet = -1;
			continue;
		}
	}

	return iRet;
}

//检查报名个人
int shikong_check_apply_single(ZONESVRENV *pstEnv,Player *pstPlayer, ZONEPWORLD *pstZonePworld)
{
	if (pstPlayer->stRoleData.Level > pstZonePworld->stPworldDef.MaxLv ||
			pstPlayer->stRoleData.Level < pstZonePworld->stPworldDef.MinLv)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_TIP, ZONEERR_SHIKONG_11);
		return -1;
	}

	return 0;
}

int shikong_enter_apply_queue_notify(ZONESVRENV* pstEnv, Player *pstPlayer,CSENTERAPPLYQUEUE *pstEnterQueue)
{
	CSPKG stPkg;
	CSSHIKONGSVR *pstShiKongSvr = &stPkg.Body.ShiKongSvr;
	CSENTERAPPLYQUEUE *pstEnterApplyQueue = &pstShiKongSvr->Data.EnterApplyQueue;

	pstShiKongSvr->Cmd = SHIKONG_SVR_OP_ENTER_APPLY_QUEUE;
	memcpy(pstEnterApplyQueue,pstEnterQueue,sizeof(*pstEnterApplyQueue));
		
	Z_CSHEAD_INIT(&stPkg.Head, SHIKONG_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		
	return 0;
}

//报名成功放入报名队列
int shikong_players2apply_queue(ZONESVRENV* pstEnv, Player *pstPlayer, 
											Player ** ppstPlayer, int iPlayer,
											ZONEPWORLD *pstZonePworld)
{
	CSENTERAPPLYQUEUE stEnterQueue;
	int i;
	int iPworldID  = pstZonePworld->stPworldDef.PworldID;
	ShiKongApplyQueue *pstApplyQueue = get_shikong_pworld_apply_queue(pstEnv,iPworldID);
	if(!pstApplyQueue)
	{
		pstApplyQueue = new_apply_queue(pstEnv,iPworldID);
	}

	if(!pstApplyQueue)
	{
		return -1;
	}

	//保证一个队伍同时报名
	if(pstApplyQueue->iApplyNum + iPlayer > MAX_SHIKONG_APPLY_QUEUE_MEM)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_BUSY);
		return -1;
	}

	for(i=0;i<iPlayer;i++)
	{
		shikong_exit_pworld_queue(pstEnv, ppstPlayer[i], ppstPlayer[i]->stOnline.stShiKongQueueInfo.iPworldID, 1);

		memset(&ppstPlayer[i]->stOnline.stShiKongQueueInfo,0,sizeof(ppstPlayer[i]->stOnline.stShiKongQueueInfo));
		
		ppstPlayer[i]->stOnline.stShiKongQueueInfo.iPworldID = iPworldID;
		ppstPlayer[i]->stOnline.stShiKongQueueInfo.iQueueType = SHIKONG_APPLY_QUEUE;

		memset(&pstApplyQueue->astMem[pstApplyQueue->iApplyNum],0,sizeof(pstApplyQueue->astMem[pstApplyQueue->iApplyNum]));
		
		pstApplyQueue->astMem[pstApplyQueue->iApplyNum].iMemID = ppstPlayer[i]->iMemID;
		if(iPlayer > 1)
		{
			//有队伍
			pstApplyQueue->astMem[pstApplyQueue->iApplyNum].iTeamIdx = ppstPlayer[i]->stRoleData.MiscInfo.TeamIdx;
			pstApplyQueue->astMem[pstApplyQueue->iApplyNum].ullTeamWID = ppstPlayer[i]->stRoleData.MiscInfo.TeamID;
		}

		pstApplyQueue->iApplyNum++;
		
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, ppstPlayer[i], SYS_MIDDLE2, ZONEERR_SHIKONG_12);

		//弹出排队倒计时
		stEnterQueue.PworldID = iPworldID;
		shikong_enter_apply_queue_notify(pstEnv, ppstPlayer[i],&stEnterQueue);
	}
	
	return 0;
}

//从npc那报名
int player_apply_pworld_from_npc(ZONESVRENV* pstEnv, Player *pstPlayer,int iPworldID,int iMode)
{
	ZONEIDX stIdx;
	ZONEPWORLD *pstPworld;
	int iPlayer = 0;
	Player *apstPlayer[MAX_TEAM_MEMBER];
	
	stIdx.iID = iPworldID;
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(pstEnv->pstMapObj,&stIdx);
	if(!pstPworld)
	{
		return -1;
	}

	if(0 == is_shikong_pworld(pstPworld))
	{
		return -1;
	}

	if (z_check_enter_again(pstEnv,pstPlayer,pstPworld) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PWORLD114);
		return -1;
	}
	
	switch(iMode)
	{
		case SHIKONG_JOIN_SINGLE:
			if(0 > shikong_check_apply_single(pstEnv,pstPlayer, pstPworld))
			{
				return -1;
			}			

			apstPlayer[iPlayer++] = pstPlayer;
			break;
		case SHIKONG_JOIN_TEAM:
			if (0 > shikong_check_apply_team(pstEnv, pstPlayer, pstPworld, apstPlayer, &iPlayer))
			{
				return -1;
			}
			break;
		default:
			return -1;
			break;
	}

	if(0 > shikong_players2apply_queue(pstEnv, pstPlayer, apstPlayer, iPlayer,pstPworld))
	{
		return -1;
	}
	
	return 0;
}

int player_exit_apply_queue(ZONESVRENV* pstEnv, Player *pstPlayer, CSEXITAPPLYQUEUE *pstExitApplyQueue)
{
	if(0 == shikong_exit_pworld_queue(pstEnv, pstPlayer, pstExitApplyQueue->PworldID, 1))
	{
		ZONEPWORLD *pstZonePwolrd = NULL;
		ZONEIDX stIdx;
		stIdx.iID = pstExitApplyQueue->PworldID;
		stIdx.iPos = -1;
		pstZonePwolrd = z_find_pworld(pstEnv->pstMapObj,&stIdx);
		if(pstZonePwolrd)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SHIKONG_2,pstZonePwolrd->stPworldDef.Name);
		}
	}
	
	return 0;
}

int shikong_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
					TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSSHIKONGCLT *pstClt = &pstCsPkg->Body.ShiKongClt;
	
	if (INST_MODE_BATTLE_SVR != pstEnv->iInstMode)
	{
		return 0;
	}

	if(0 > player_if_in_span_map(pstEnv,pstPlayer))
	{
		return 0;
	}
	
	switch(pstClt->CltType)
	{
		case SHIKONG_CLT_EXIT_APPLY_QUEUE:
			return player_exit_apply_queue(pstEnv,pstPlayer,&pstClt->CltData.ExitApplyQueue);
			break;
		case SHIKONG_CLT_ENTER_PWORLD:
			return player_enter_shikong_pworld(pstEnv,pstPlayer,pstClt->CltData.EnterShikongPworld.PworldID);
			break;		
		default:
			return 0;
			break;
	}

	return 0;
}

int is_shikong_pworld(ZONEPWORLD *pstZonePworld)
{
	if(pstZonePworld->stPworldDef.Type != PWORLD_TYPE_PLOY)
	{
		return 0;
	}

	if(pstZonePworld->stPworldDef.PolyType != PWORLD_POLY_SHIKONG)
	{
		return 0;
	}
	
	return 1;
}