/*
**  @file $RCSfile: zone_task.c,v $
**  general description of this module
**  $Id: zone_task.c,v 1.622 2014/06/25 18:57:34 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2014/06/25 18:57:34 $
**  @version $Revision: 1.622 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "zone_task.h"
#include "zone_clt.h"
#include "zone_team.h"
#include "zone_package.h"
#include "zone_status.h"
#include "zone_npc.h"
#include "zone_player.h"
#include "zone_attr.h"
#include "comm_misc.h"
#include "zone_script.h"
#include "zone_move.h"
#include "zone_relation.h"
#include "zone_mail.h"
#include "zone_svr.h"
#include "zone_oplog.h"
#include "zone_pet.h"
#include "zone_clan.h"
#include "zone_respact.h"
#include "zone_designation.h"
#include "zone_secondaryskill.h"
#include "zone_range.h"
#include "zone_combat.h"
#include "zone_clan_advance.h"
#include "zone_shangjin.h"
#include "zone_gay.h"
#include "zone_misc.h"
#include "zone_ronglu.h"
#include "zone_vip.h"
#include "zone_err.h"
#include "zone_zh_name.h"
#include "zone_arm.h"

extern ScriptEnv stSEnv;
static  int g_siCycleID = 0;
static  int g_siTaskCommit = 0;
static  int g_siTaskDrop = 0;
extern int g_iOptype;
extern int g_iOpsubtype;

//#define SJ_MAX_SUIPIAN_WEEK 50

TASKRUNONE *player_get_dart_car_task(ZONESVRENV *pstEnv, Player *pstPlayer);
int taskcycle_add_run(ZONESVRENV *pstEnv, Player *pstPlayer, TASKRECYCLEDEF *pstTaskCycleDef, int iTaskID);
int trust_award_order_update(ZONESVRENV* pstEnv, Player *pstPlayer, unsigned int uiTaskID);
int task_auto_pop(ZONESVRENV *pstEnv, Player *pstPlayer, int iTaskID, int iNpcID);
int taskcycle_new(ZONESVRENV *pstEnv, Player *pstPlayer, TASKRECYCLEDEF *pstTaskCycleDef);

#define THROW_ERROR(err) iRet = - err; \
	goto error
	
int TrustHashCmp( const void * pv1 , const void *pv2 )
{
	ROLETRUSTDATA *pstTrustHash1 = (ROLETRUSTDATA *)pv1;
	ROLETRUSTDATA *pstTrustHash2 = (ROLETRUSTDATA *)pv2;

	return (pstTrustHash1->RoleID ==  pstTrustHash2->RoleID) ? 0 : 1;
}

unsigned int TrustHashCode(const void* pvData)
{
	ROLETRUSTDATA *pstTrustHash1 = (ROLETRUSTDATA*)pvData;
	
	return pstTrustHash1->RoleID;
}

TASKRUNONE *task_run_find(Player *pstPlayer, int iTaskID)
{
	int i;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i=0; i<pstTaskRun->TaskRunNum; i++)
	{
		if (pstTaskRun->TaskRuns[i].TaskID == iTaskID)
		{
			return &pstTaskRun->TaskRuns[i];
		}
	}
	
	return NULL;
}

TASKRECYCLEONE *taskcycle_run_find(Player *pstPlayer, int iTaskCycleID)
{
	int i;
	TASKRECYCLE *pstTaskCycleRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRecycle;

	for (i=0; i<pstTaskCycleRun->TaskRecycleNum; i++)
	{
		if (pstTaskCycleRun->TaskRecycles[i].TaskRecycleID == iTaskCycleID)
		{
			return &pstTaskCycleRun->TaskRecycles[i];
		}
	}
	
	return NULL;
}

int taskcycle_run_del(Player *pstPlayer, int iTaskCycleID)
{
	int i;
	TASKRECYCLE *pstTaskCycleRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRecycle;

	for (i=pstTaskCycleRun->TaskRecycleNum-1; i>=0; i--)
	{
		if (pstTaskCycleRun->TaskRecycles[i].TaskRecycleID == iTaskCycleID)
		{
			pstTaskCycleRun->TaskRecycleNum--;
			if (i != pstTaskCycleRun->TaskRecycleNum)
			{
				pstTaskCycleRun->TaskRecycles[i] = pstTaskCycleRun->TaskRecycles[pstTaskCycleRun->TaskRecycleNum];
			}
		}
	}

	return -1;
}

TASKDEF *task_def_find(ZONESVRENV *pstEnv, int iTaskID, int *piTaskIdx)
{
	return (TASKDEF *)z_find_def(pstEnv->pstZoneObj->sTaskDef, iTaskID, TASK_DATA, 
									pstEnv->pstZoneObj->iTaskDef, piTaskIdx);
}

TASKLIMITGROUP *task_limit_group_def_find(ZONESVRENV *pstEnv, int iGroupID, int *piTaskIdx)
{
	return (TASKLIMITGROUP *)z_find_def(pstEnv->pstZoneObj->sTaskLimitGroup, iGroupID, TASK_LIMIT_GROUP_DATA, 
									pstEnv->pstZoneObj->iTaskLimitGroup, piTaskIdx);
}

TASKRECYCLEDEF *taskcycle_def_find(ZONESVRENV *pstEnv, int iTaskCycleID, int *piTaskCycleIdx)
{
	return (TASKRECYCLEDEF *)z_find_def(pstEnv->pstZoneObj->sTaskCycleDef, iTaskCycleID, TASK_CYCLE_DATA, 
									pstEnv->pstZoneObj->iTaskCycleDef, piTaskCycleIdx);
}

//效率有点低，以后可优化为二分查找
int task_find_fini(Player *pstPlayer, int iTaskID)
{
	int i;
	
	TASKFINI *pstTaskFini = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskFini;

	for (i=pstTaskFini->TaskNum-1; i>=0; i--)
	{
		if (pstTaskFini->TaskIDs[i] == iTaskID)
		{
			return 0;
		}
	}

	return -1;
}

int task_add_fini(Player *pstPlayer, int iTaskID)
{
	TASKFINI *pstTaskFini = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskFini;
	
	if (0 == task_find_fini(pstPlayer, iTaskID))
	{
		return 0;
	}

	if (pstTaskFini->TaskNum >= MAX_TASK_FINI)
	{
		memmove(&pstTaskFini->TaskIDs[0], &pstTaskFini->TaskIDs[1], (MAX_TASK_FINI-1)*sizeof(pstTaskFini->TaskIDs[0]));

		pstTaskFini->TaskIDs[MAX_TASK_FINI-1] = iTaskID;
	}
	else
	{
		pstTaskFini->TaskIDs[pstTaskFini->TaskNum] = iTaskID;
		pstTaskFini->TaskNum++;
	}

	return 0;
}

GROUPONEDAYINFO *group_find_oneday(Player *pstPlayer, int iGroupID)
{
	int i;
	GROUPONEDAY *pstOneDay = &pstPlayer->stRoleData.MiscInfo.TaskInfo.GroupOneDay;
	GROUPONEDAYINFO *pstOneDayInfo;

	for (i=0; i<pstOneDay->GroupNum; i++)
	{
		pstOneDayInfo = &pstOneDay->GroupOneDayInfo[i];
		if (pstOneDayInfo->GroupID == iGroupID)
		{
			return pstOneDayInfo;
		}
	}
	return NULL;
}

TASKGROUPWEEKINFO* group_find_week(Player *pstPlayer, int iGroupID)
{
	int i;
	GROUPWEEK*pstWeek = &pstPlayer->stRoleData.MiscInfo.TaskInfo.GroupWeek;

	for (i=0; i<pstWeek->GroupNum; i++)
	{
		if (pstWeek->GroupWeekInfo[i].GroupID == iGroupID)
		{
			return &pstWeek->GroupWeekInfo[i];
		}
	}
	return NULL;
}

TASKONEDAYINFO *task_find_oneday(Player *pstPlayer, int iTaskID)
{
	int i;
	TASKONEDAYINFO *pstTaskOneDayInfo;
	TASKONEDAY *pstTaskOneDay = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskOneDay;
	
	for (i=0; i<pstTaskOneDay->TaskNum; i++)
	{
		pstTaskOneDayInfo = &pstTaskOneDay->TaskOneDayInfo[i];
		if (pstTaskOneDayInfo->TaskID == iTaskID)
		{
			return pstTaskOneDayInfo;
		}
	}

	return NULL;
}

int group_add_oneday(ZONESVRENV *pstEnv, Player *pstPlayer, int iGroupID)
{
	int i;
	
	TASKLIMITGROUP *pstLimitGroup = task_limit_group_def_find(pstEnv,iGroupID, NULL);
	if (!pstLimitGroup)
		return 0;

	if (pstLimitGroup->AddFlag == 0)
	{
		GROUPONEDAY *pstGroupOneDay;
		GROUPONEDAYINFO *pstGroupOneDayInfo;
	
		pstGroupOneDay = &pstPlayer->stRoleData.MiscInfo.TaskInfo.GroupOneDay;

		//清理一下
		for (i=pstGroupOneDay->GroupNum -1; i>=0; i--)
		{
			if (0 == IsSameDay(pstGroupOneDay->GroupOneDayInfo[i].Time, pstEnv->pstAppCtx->stCurr.tv_sec))
			{
				pstGroupOneDay->GroupNum --;
				if (i != pstGroupOneDay->GroupNum)
				{
					pstGroupOneDay->GroupOneDayInfo[i] = pstGroupOneDay->GroupOneDayInfo[pstGroupOneDay->GroupNum];
				}
			}
		}
		
		for (i=0; i<pstGroupOneDay->GroupNum; i++)
		{
			pstGroupOneDayInfo = &pstGroupOneDay->GroupOneDayInfo[i];
			if (pstGroupOneDayInfo->GroupID == iGroupID)
			{
				pstGroupOneDayInfo->OneDayNum ++;
				return 0;
			}
		}

		if (pstGroupOneDay->GroupNum >= MAX_GROUP_ONEDAY)
		{
			memmove(&pstGroupOneDay->GroupOneDayInfo[0],&pstGroupOneDay->GroupOneDayInfo[1],
						(MAX_GROUP_ONEDAY-1)*sizeof(pstGroupOneDay->GroupOneDayInfo[0]));

			pstGroupOneDayInfo = &pstGroupOneDay->GroupOneDayInfo[MAX_GROUP_ONEDAY-1];
		}
		else
		{
			pstGroupOneDayInfo = &pstGroupOneDay->GroupOneDayInfo[pstGroupOneDay->GroupNum];
			pstGroupOneDay->GroupNum ++;
		}

		pstGroupOneDayInfo->GroupID = iGroupID;
		pstGroupOneDayInfo->OneDayNum = 1;
		pstGroupOneDayInfo->Time = pstEnv->pstAppCtx->stCurr.tv_sec;
	}
	else
	{
		TASKGROUPWEEKINFO *pstWeekInfo;
		GROUPWEEK *pstGroupWeek = &pstPlayer->stRoleData.MiscInfo.TaskInfo.GroupWeek;

		for (i=pstGroupWeek->GroupNum-1; i>=0; i--)
		{
			if (0 == IsSameWeek(pstGroupWeek->GroupWeekInfo[i].Time, pstEnv->pstAppCtx->stCurr.tv_sec))
			{
				pstGroupWeek->GroupNum--;
				if (i != pstGroupWeek->GroupNum)
				{
					pstGroupWeek->GroupWeekInfo[i] = pstGroupWeek->GroupWeekInfo[pstGroupWeek->GroupNum];
				}
			}
		}

		for (i=0; i<pstGroupWeek->GroupNum; i++)
		{
			if (pstGroupWeek->GroupWeekInfo[i].GroupID == iGroupID)
			{
				pstGroupWeek->GroupWeekInfo[i].WeekNum++;
				return 0;
			}
		}

		if (pstGroupWeek->GroupNum >= MAX_GROUP_WEEK)
		{
			memmove(&pstGroupWeek->GroupWeekInfo[0],&pstGroupWeek->GroupWeekInfo[1],
						(MAX_GROUP_WEEK-1)*sizeof(pstGroupWeek->GroupWeekInfo[0]));

			pstWeekInfo = &pstGroupWeek->GroupWeekInfo[MAX_GROUP_WEEK-1];
		}
		else
		{
			pstWeekInfo  = &pstGroupWeek->GroupWeekInfo[pstGroupWeek->GroupNum];
			pstGroupWeek->GroupNum ++;
		}

		pstWeekInfo->GroupID = iGroupID;
		pstWeekInfo->Time = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstWeekInfo->WeekNum = 1;
	}

	return 0;
}

int task_add_oneday(ZONESVRENV *pstEnv, Player *pstPlayer, int iTaskID)
{
	int i;
	TASKONEDAYINFO *pstTaskOneDayInfo;
	TASKONEDAY *pstTaskOneDay = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskOneDay;

	// 清理任务信息
	for (i=pstTaskOneDay->TaskNum-1; i>=0; i--)
	{
		if (0 == IsSameDay(pstTaskOneDay->TaskOneDayInfo[i].Time, pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			pstTaskOneDay->TaskNum--;
			if (i != pstTaskOneDay->TaskNum)
			{
				pstTaskOneDay->TaskOneDayInfo[i] = pstTaskOneDay->TaskOneDayInfo[pstTaskOneDay->TaskNum];
			}
		}
	}

	for (i=0; i<pstTaskOneDay->TaskNum; i++)
	{
		pstTaskOneDayInfo = &pstTaskOneDay->TaskOneDayInfo[i];
		if (pstTaskOneDayInfo->TaskID == iTaskID)
		{
			pstTaskOneDayInfo->OneDayNum++;
			return pstTaskOneDayInfo->OneDayNum;
		}
	}

	if (pstTaskOneDay->TaskNum >= MAX_TASK_ONEDAY)
	{
		memmove(&pstTaskOneDay->TaskOneDayInfo[0], &pstTaskOneDay->TaskOneDayInfo[1], 
					(MAX_TASK_ONEDAY-1)*sizeof(pstTaskOneDay->TaskOneDayInfo[0]));

		pstTaskOneDayInfo = &pstTaskOneDay->TaskOneDayInfo[MAX_TASK_ONEDAY-1];
	}
	else
	{
		pstTaskOneDayInfo = &pstTaskOneDay->TaskOneDayInfo[pstTaskOneDay->TaskNum];
		
		pstTaskOneDay->TaskNum++;
	}

	pstTaskOneDayInfo->TaskID = iTaskID;
	pstTaskOneDayInfo->OneDayNum = 1;
	pstTaskOneDayInfo->Time = pstEnv->pstAppCtx->stCurr.tv_sec;
	
	return pstTaskOneDayInfo->OneDayNum;
}

TASKCYCLEONEDAYINFO *taskcycle_find_oneday(Player *pstPlayer, int iTaskCycleID)
{
	int i;
	TASKCYCLEONEDAYINFO *pstTaskCycleOneDayInfo;
	TASKCYCLEONEDAY *pstTaskCycleOneDay = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskCycleOneDay;

	for (i=0; i<pstTaskCycleOneDay->TaskCycleNum; i++)
	{
		pstTaskCycleOneDayInfo = &pstTaskCycleOneDay->TaskCycleOneDayInfo[i];

		if (pstTaskCycleOneDayInfo->TaskCycleID == iTaskCycleID)
		{
			return pstTaskCycleOneDayInfo;
		}
	}

	return NULL;
}

TASKCYCLEWEEKINFO *taskcyc_find_week(Player *pstPlayer, int iTaskCycleID)
{
	int i;
	TASKCYCLEWEEK *pstWeek = &pstPlayer->stRoleData.MiscInfo.TaskInfo.CycleWeek;

	for (i=0; i<pstWeek->CycleNum; i++)
	{
		if (pstWeek->CycleWeekInfo[i].CycleID == iTaskCycleID)
		{
			return &pstWeek->CycleWeekInfo[i];
		}
	}
	
	return NULL;
}


int taskcycle_add_oneday(ZONESVRENV* pstEnv, Player *pstPlayer, int iTaskCycleID)
{
	int i;
	TASKCYCLEONEDAYINFO *pstTaskCycleOneDayInfo;
	TASKCYCLEONEDAY *pstTaskCycleOneDay = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskCycleOneDay;
	TASKCYCLEWEEK *pstWeek = &pstPlayer->stRoleData.MiscInfo.TaskInfo.CycleWeek;

	TASKRECYCLEDEF * pstTaskRecycleDef;
	TASKCYCLEWEEKINFO *pstWeekInfo;
	
	pstTaskRecycleDef = taskcycle_def_find(pstEnv, iTaskCycleID, NULL);
	if(!pstTaskRecycleDef)
	{
		return 0;
	}

	if (pstTaskRecycleDef->OneDayNum > 0)
	{
		for (i=pstTaskCycleOneDay->TaskCycleNum-1; i>=0; i--)
		{
			if (0 == IsSameDay(pstTaskCycleOneDay->TaskCycleOneDayInfo[i].Time, pstEnv->pstAppCtx->stCurr.tv_sec))
			{
				pstTaskCycleOneDay->TaskCycleNum--;
				if (i != pstTaskCycleOneDay->TaskCycleNum)
				{
					pstTaskCycleOneDay->TaskCycleOneDayInfo[i] = pstTaskCycleOneDay->TaskCycleOneDayInfo[pstTaskCycleOneDay->TaskCycleNum];
				}
			}
		}
		
		for (i=0; i<pstTaskCycleOneDay->TaskCycleNum; i++)
		{
			pstTaskCycleOneDayInfo = &pstTaskCycleOneDay->TaskCycleOneDayInfo[i];

			if (pstTaskCycleOneDayInfo->TaskCycleID == iTaskCycleID)
			{
				pstTaskCycleOneDayInfo->OneDayNum++;
				goto week;
			}
		}

		if (pstTaskCycleOneDay->TaskCycleNum >= MAX_TASKCYCLE_ONEDAY)
		{
			memmove(&pstTaskCycleOneDay->TaskCycleOneDayInfo[0], &pstTaskCycleOneDay->TaskCycleOneDayInfo[1],
						(MAX_TASKCYCLE_ONEDAY-1)*sizeof(pstTaskCycleOneDay->TaskCycleOneDayInfo[0]));

			pstTaskCycleOneDayInfo = &pstTaskCycleOneDay->TaskCycleOneDayInfo[MAX_TASKCYCLE_ONEDAY-1];
		}
		else
		{
			pstTaskCycleOneDayInfo = &pstTaskCycleOneDay->TaskCycleOneDayInfo[pstTaskCycleOneDay->TaskCycleNum];

			pstTaskCycleOneDay->TaskCycleNum++;
		}

		pstTaskCycleOneDayInfo->TaskCycleID = iTaskCycleID;
		pstTaskCycleOneDayInfo->OneDayNum=1;
		pstTaskCycleOneDayInfo->Time = pstEnv->pstAppCtx->stCurr.tv_sec;
	}
	
// 借用此函数记录每周环组信息
week:
	if (pstTaskRecycleDef->WeekNum > 0)
	{
		for (i=pstWeek->CycleNum-1; i>=0; i--)
		{
			if (0==IsSameWeek(pstWeek->CycleWeekInfo[i].Time, pstEnv->pstAppCtx->stCurr.tv_sec))
			{
				pstWeek->CycleNum --;
				if (i != pstWeek->CycleNum)
				{
					pstWeek->CycleWeekInfo[i] = pstWeek->CycleWeekInfo[pstWeek->CycleNum];
				}
			}
		}
		
		for (i=0; i<pstWeek->CycleNum; i++)
		{
			if (pstWeek->CycleWeekInfo[i].CycleID == iTaskCycleID)
			{
				pstWeek->CycleWeekInfo[i].WeekNum ++;
				return 0;
			}
		}

		if (pstWeek->CycleNum >= MAX_TASKCYCLE_WEEK)
		{
			memmove(&pstWeek->CycleWeekInfo[0], &pstWeek->CycleWeekInfo[1],
				(MAX_TASKCYCLE_WEEK-1)*sizeof(pstWeek->CycleWeekInfo[0]));
			pstWeekInfo = &pstWeek->CycleWeekInfo[MAX_TASKCYCLE_WEEK-1];
		}
		else
		{
			pstWeekInfo = &pstWeek->CycleWeekInfo[pstWeek->CycleNum];
			pstWeek->CycleNum ++;
		}
		pstWeekInfo->CycleID = iTaskCycleID;
		pstWeekInfo->Time = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstWeekInfo->WeekNum = 1;
	}
	
	return 0;
}

void z_clear_convoy_info(Player *pstPlayer)
{
	ROLEMISCINFO *pstMisc = &pstPlayer->stRoleData.MiscInfo;

	pstMisc->ConvoyInfo.TaskID = 0;
	pstMisc->ConvoyInfo.ConvoyMonID = 0;

	return ;
}

int is_convoy_task(TASKDEF *pstTaskDef)
{
	if(pstTaskDef->Type == TASK_TYPE_SINGLE_CONVOY || pstTaskDef->Type == TASK_TYPE_TEAM_CONVOY)
	{
		return 1;
	}
	
	return 0;
}

int z_convoy_task_check(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	ROLEMISCINFO *pstMisc = &pstPlayer->stRoleData.MiscInfo;
	TASKRUNONE *pstTaskRunOne; 
	TASKDEF *pstTaskDef;
	Monster *pstMon;

	pstTaskRunOne = task_run_find(pstPlayer, pstMisc->ConvoyInfo.TaskID);
	if(NULL == pstTaskRunOne || pstTaskRunOne->TaskStat == TASK_STAT_FAIL)
	{
		return 0;
	}

	pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
	if(NULL == pstTaskDef)
	{
		return 0;;
	}

	if(pstTaskDef->Type == TASK_TYPE_TEAM_CONVOY)
	{
		ZoneTeam *pstTeam = player_team_get(pstEnv, pstPlayer);

		if(NULL == pstTeam)
		{
			goto _error;
		}
	}

	pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstMisc->ConvoyInfo.ConvoyMonID);
	if(NULL == pstMon)
	{
		goto _error;
	}

	if(pstMon->stMap.iID != pstPlayer->stRoleData.Map ||
			z_distance(&pstMon->stCurrPos, &pstPlayer->stRoleData.Pos) > MAX_CONVOY_TASK_DIST)
	{
		goto _error;
	}

	return 0;

_error:

	z_clear_convoy_info(pstPlayer);
	
	pstTaskRunOne->TaskStat = TASK_STAT_FAIL;
	task_chg(stSEnv.pstEnv, pstPlayer, pstTaskRunOne);

	return 0;
}

int z_convoy_task_fail(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;
	TASKDEF *pstTaskDef;

	for (i=0; i<pstTaskRun->TaskRunNum; i++)
	{
		pstTaskDef = task_def_find(pstEnv, pstTaskRun->TaskRuns[i].TaskID, &pstTaskRun->TaskRuns[i].TaskIdx);
		if(NULL == pstTaskDef)
		{
			continue;
		}

		if((pstTaskDef->Type == TASK_TYPE_TEAM_CONVOY || pstTaskDef->Type == TASK_TYPE_SINGLE_CONVOY) && pstTaskRun->TaskRuns[i].TaskStat != TASK_STAT_FAIL)
		{
			z_clear_convoy_info(pstPlayer);
			
			pstTaskRun->TaskRuns[i].TaskStat = TASK_STAT_FAIL;
			task_chg(stSEnv.pstEnv, pstPlayer, &pstTaskRun->TaskRuns[i]);
		}
	}
	
	return 0;
}

int z_team_convoy_task_fail(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;
	TASKDEF *pstTaskDef;

	for (i=0; i<pstTaskRun->TaskRunNum; i++)
	{
		pstTaskDef = task_def_find(pstEnv, pstTaskRun->TaskRuns[i].TaskID, &pstTaskRun->TaskRuns[i].TaskIdx);
		if(NULL == pstTaskDef)
		{
			continue;
		}

		if(pstTaskDef->Type == TASK_TYPE_TEAM_CONVOY && pstTaskRun->TaskRuns[i].TaskStat != TASK_STAT_FAIL)
		{
			z_clear_convoy_info(pstPlayer);
			
			pstTaskRun->TaskRuns[i].TaskStat = TASK_STAT_FAIL;
			task_chg(stSEnv.pstEnv, pstPlayer, &pstTaskRun->TaskRuns[i]);
		}
	}
	
	return 0;
}
	
int task_accept_check_matchclient(ZONESVRENV *pstEnv, Player *pstPlayer, TASKDEF *pstTaskDef, int fTeamNotify)
{
	int i;
	TASKGETLIMIT *pstTaskLimit = &pstTaskDef->TaskGetLimit;
	ZoneTeam *pstTeam = player_team_get(pstEnv, pstPlayer);
	int iRet = -1;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;
	int iLevelFailed = 0;

	// 检查公会任务
	if (pstTaskDef->Type == TASK_TYPE_CLAN)
	{
		int iRet1 = player_check_clan(pstEnv, pstPlayer);
		if (iRet1 < 0)
		{
			THROW_ERROR(TAE_CLAN_LIMIT); 
		}
	}

	if(pstTaskDef->ClanLevelLimit > 0)
	{
		ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);

		if(!pstZoneClan)
		{
			THROW_ERROR(TAE_CLAN_LEVEL_LIMIT); 
		}

		if(pstZoneClan->stClan.ClanProf.Level < pstTaskDef->ClanLevelLimit)
		{
			THROW_ERROR(TAE_CLAN_LEVEL_LIMIT); 
		}
	}
	
	if (pstTaskDef->LimitGroupID > 0 && pstTaskDef->IsGroupMutex > 0)
	{
		int iRun = 0;
		for (i=0; i<pstTaskRun->TaskRunNum; i++)
		{
			TASKDEF *pstTask = task_def_find(pstEnv,
			                                 pstTaskRun->TaskRuns[i].TaskID, 
				                             &pstTaskRun->TaskRuns[i].TaskIdx);

			if(NULL == pstTask || pstTaskDef->LimitGroupID != pstTask->LimitGroupID)
			{
				continue;
			}

			if(pstTask->IsGroupMutex <= 0)
			{
				continue;
			}
			
			iRun++;
			if(iRun >= pstTaskDef->IsGroupMutex)
			{
				THROW_ERROR(TAE_MUTEX);
			}
		}
	}

	if(task_run_find(pstPlayer, pstTaskDef->ID))
	{
		THROW_ERROR(TAE_REPEATED);
	}

	if ( (CAREER_NON !=pstTaskLimit->Career   && (pstPlayer->stRoleData.Career&0x0F) != (pstTaskLimit->Career&0x0F)) ||
		 (CAREER_NON != (pstTaskLimit->Career&0xF0) && (pstPlayer->stRoleData.Career&0xF0) != (pstTaskLimit->Career&0xF0)) )
	{
		//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, "你的职业不符合任务要求");
		THROW_ERROR(TAE_CAREER_ERROR);
	}

	if (pstTaskLimit->Gender != GENDER_NO && pstTaskLimit->Gender != pstPlayer->stRoleData.Gender)
	{
		//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, "你的性别不符合任务要求");
		THROW_ERROR(TAE_GENDER_ERROR);
	}

	// 检查声望需求
	if(player_respact_value_check(pstEnv, pstPlayer,
	                              &pstTaskLimit->NeedRespact) < 0)
	{
		THROW_ERROR(TAE_NOT_ENOUGH_RESPACT);
	}

	if (pstTaskLimit->PreTaskID > 0)
	{
		if (0 > task_find_fini(pstPlayer, pstTaskLimit->PreTaskID))
		{
			/*
			TASKDEF *pstPreTaskDef;
			char szMsg[256];
			
			pstPreTaskDef = task_def_find(pstEnv, pstTaskLimit->PreTaskID, NULL);
			if (pstPreTaskDef)
			{
				sprintf(szMsg, "本任务需要先完成前置任务%s ", pstPreTaskDef->Name);
			}
			else
			{
				sprintf(szMsg, "本任务需要先完成前置任务");
			}
			
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, szMsg);*/
			THROW_ERROR(TAE_NEED_PRETASK);
		}
	}

	if (0 == pstTaskDef->Redo)
	{
		if (0 == task_find_fini(pstPlayer, pstTaskDef->ID))
		{
			//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, "本任务你已经做过了，不能再接");
			THROW_ERROR(TAE_TASK_REDO);
		}
	}
	else if (pstTaskLimit->NumOneDay > 0)
	{
		TASKONEDAYINFO *pstTaskOneDay;

		pstTaskOneDay = task_find_oneday(pstPlayer, pstTaskDef->ID);
		if (pstTaskOneDay && pstTaskOneDay->OneDayNum >= pstTaskDef->TaskGetLimit.NumOneDay)
		{
			if(fTeamNotify && pstTeam)
			{
				z_team_sys_strf(pstEnv,  pstTeam, SYS_MIDDLE2, 
					            ZONEERR_TEAM307, pstPlayer->stRoleData.RoleName);
			}
			THROW_ERROR(TAE_FINI_PER_DAY);
		}
	}
	else if (pstTaskDef->LimitGroupID  > 0)
	{
		GROUPONEDAYINFO *pstGroupOneDayInfo = group_find_oneday(pstPlayer,pstTaskDef->LimitGroupID);
		TASKGROUPWEEKINFO*pstGroupWeekInfo = group_find_week(pstPlayer, pstTaskDef->LimitGroupID);
		
		if (pstGroupWeekInfo)
		{
			int iVip = 0;
			RESULT *pstVipResult ;
			TASKLIMITGROUP *pstLimitGroup = task_limit_group_def_find(pstEnv,pstTaskDef->LimitGroupID, NULL);

			if (	pstTaskDef->Type == TASK_TYPE_SJ_MON || 
				pstTaskDef->Type == TASK_TYPE_SJ_ITEM ||
				pstTaskDef->Type == TASK_TYPE_SJ_BOSS)
			{
				pstVipResult = star_get_result(pstEnv, pstPlayer, STAR_RESULT_SHANGJIN);
				if (pstVipResult)
				{
					iVip = pstVipResult->ResultVal1;
				}
			}

			if (pstLimitGroup && pstGroupWeekInfo->WeekNum >= pstLimitGroup->GroupOneDay+iVip)
			{
				if(fTeamNotify && pstTeam)
				{
					z_team_sys_strf(pstEnv,  pstTeam, SYS_MIDDLE2, 
						            ZONEERR_TEAM318, pstPlayer->stRoleData.RoleName);
				}
				THROW_ERROR(TAE_LIMIT_GROUP);
			}
		}
		else if (pstGroupOneDayInfo)
		{
			int iVip = 0;
			RESULT *pstVipResult ;
			TASKLIMITGROUP *pstLimitGroup = task_limit_group_def_find(pstEnv,pstTaskDef->LimitGroupID, NULL);

			if (pstTaskDef->Type == TASK_TYPE_RONGLU)
			{
				pstVipResult = star_get_result(pstEnv, pstPlayer, STAR_RESULT_RONGLU);
				if (pstVipResult)
				{
					iVip = pstVipResult->ResultVal1;
				}
			}
			else if (pstTaskDef->ID >= 3351 && pstTaskDef->ID <= 3361) //赏善罚恶
			{
				pstVipResult = star_get_result(pstEnv, pstPlayer, STAR_RESULT_SHANGSAN);
				if (pstVipResult)
				{
					iVip = pstVipResult->ResultVal1;
				}
			}
			
			if (pstLimitGroup && pstGroupOneDayInfo->OneDayNum >= pstLimitGroup->GroupOneDay+iVip)
			{
				if(fTeamNotify && pstTeam)
				{
					z_team_sys_strf(pstEnv,  pstTeam, SYS_MIDDLE2, 
						            ZONEERR_TEAM307, pstPlayer->stRoleData.RoleName);
				}
				THROW_ERROR(TAE_LIMIT_GROUP);
			}
		}
	}

	if (pstTaskLimit->SecondarySkillType > 0)
	{
		SECONDARYSKILLDEF *pstSecSkillDef;
		SECONDARYSKILLINFO *pstSkillInfo = get_player_secondary_skill_info(pstPlayer, pstTaskLimit->SecondarySkillType);

		if(NULL == pstSkillInfo)
		{
			THROW_ERROR(TAE_SECONDARYSKILL_LIMIT);
		}

		if(pstTaskLimit->SecondarySkillLevel > 0)
		{
			if (pstSkillInfo->SkillLevel != pstTaskLimit->SecondarySkillLevel)
			{
				THROW_ERROR(TAE_SECONDARYSKILL_LIMIT);
			}
			
			pstSecSkillDef = z_find_secondary_skill_def(pstEnv, pstSkillInfo->SkillType, pstSkillInfo->SkillLevel);
			if(NULL == pstSecSkillDef || pstSkillInfo->SkillVal < (unsigned int) pstSecSkillDef->LevelUpVal)
			{
				THROW_ERROR(TAE_SECONDARYSKILL_LIMIT);
			}
		}
		
	}

	if(pstTaskDef->TaskAward.SecondarySkillType > 0)
	{
		if (0 >player_learn_secondary_skill_check(pstEnv, pstPlayer, pstTaskDef->TaskAward.SecondarySkillType,
											pstTaskDef->TaskAward.SecondarySkillLevel))
		{
			THROW_ERROR(TAE_SECONDARYSKILL_LIMIT); 
		}
	}

	if (pstTaskDef->Type == TASK_TYPE_DAILY_GOAL)
	{
		iLevelFailed = 
			(pstTaskLimit->GetLevelMin != 0 && pstPlayer->stRoleData.MiscInfo.DailyActive.MinLevel < pstTaskLimit->GetLevelMin) || 
			(pstTaskLimit->GetLevelMax != 0 && pstPlayer->stRoleData.MiscInfo.DailyActive.MinLevel > pstTaskLimit->GetLevelMax);
	}
	else
	{
		iLevelFailed = (pstTaskLimit->GetLevelMin != 0 && pstPlayer->stRoleData.Level < pstTaskLimit->GetLevelMin) || 
			(pstTaskLimit->GetLevelMax != 0 && pstPlayer->stRoleData.Level > pstTaskLimit->GetLevelMax);
	}
	
	if (iLevelFailed)
	{

		if(fTeamNotify && pstTeam)
		{
			z_team_sys_strf(pstEnv,  pstTeam, SYS_MIDDLE2, 
				            ZONEERR_TEAM308, pstPlayer->stRoleData.RoleName);
		}
		
		if (pstTaskLimit->GetLevelMin != 0 && 
			pstPlayer->stRoleData.Level < pstTaskLimit->GetLevelMin &&
			(pstPlayer->stRoleData.Level + 4) >= pstTaskLimit->GetLevelMin)  //差一级企划要求特殊显示
		{
			//iRet = -2;
			//goto _error;

			THROW_ERROR(TAE_LEVEL_ONE);
		}
		else
		{
			THROW_ERROR(TAE_LEVEL_LIMIT);
		}
	}

	iRet = 0;

final:
	return iRet;

error:
	
	goto final;
	return 0;
}

int taskcycle_check_mutex(ZONESVRENV *pstEnv, Player *pstPlayer, TASKRECYCLEDEF *pstAtpCycleDef)
{
	int i;
	TASKRECYCLE *pstTaskRecycle = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRecycle;
	TASKRECYCLEDEF *pstTaskCycleDef;
	
	if (pstAtpCycleDef->IsGroupMutex > 0)
	{
		for (i=0; i<pstTaskRecycle->TaskRecycleNum; i++)
		{
			pstTaskCycleDef = taskcycle_def_find(pstEnv, pstTaskRecycle->TaskRecycles[i].TaskRecycleID,NULL);

			if(	pstTaskCycleDef &&
			   	pstTaskCycleDef->IsGroupMutex > 0 &&
			   	pstTaskCycleDef->GroupID == pstAtpCycleDef->GroupID&&
			   	pstTaskCycleDef->ID != pstAtpCycleDef->ID)
			{
				return -1;
			}
		}	
	}
	return 0;
}

int taskcycle_check_group_oneday(ZONESVRENV *pstEnv, Player *pstPlayer, TASKRECYCLEDEF *pstAtpCycleDef)
{
	int i;
	TASKCYCLEONEDAY *pstTaskCycleOneDay = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskCycleOneDay;
	TASKRECYCLEDEF *pstTaskCycleDef;
	int iGroupOnedayNum = 0;

	if (pstAtpCycleDef->GroupLimit <= 0 )
		return 0;

	for (i=0; i<pstTaskCycleOneDay->TaskCycleNum; i++)
	{
		pstTaskCycleDef = taskcycle_def_find(pstEnv, pstTaskCycleOneDay->TaskCycleOneDayInfo[i].TaskCycleID,NULL);
		if (	pstTaskCycleDef && 
			pstTaskCycleDef->GroupID == pstAtpCycleDef->GroupID &&
			pstTaskCycleDef->GroupLimit == pstAtpCycleDef->GroupLimit )
		{
			iGroupOnedayNum += pstTaskCycleOneDay->TaskCycleOneDayInfo[i].OneDayNum;
		}
	}

	if (iGroupOnedayNum >= pstAtpCycleDef->GroupLimit)
	{
		return -1;
	}
	return 0;
}

int taskcycle_accept_check(ZONESVRENV *pstEnv, Player *pstPlayer, TASKRECYCLEDEF *pstTaskCycleDef,
								int fDoNotNotify)
{
	TASKCYCLEONEDAYINFO *pstTaskCycleOneDay;
	TASKRECYCLEONE *pstTaskCycleOne;
	TASKRECYCLEAWARD *pstTaskCycleAward;
	LEVELUP *pstLevelUP = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	int iCycleIdx = 0;
	TASKCYCLEWEEKINFO *pstWeek;
	//TASKCYCLEWEEKINFO *taskcyc_find_week(Player *pstPlayer, int iTaskCycleID)
		
	pstTaskCycleOneDay = taskcycle_find_oneday(pstPlayer, pstTaskCycleDef->ID);
	pstWeek = taskcyc_find_week(pstPlayer, pstTaskCycleDef->ID);

	if (pstTaskCycleDef->CycType == CYC_TYPE_CLAN)
	{
		if (player_check_clan(pstEnv, pstPlayer) < 0)
		{
			return -1;
		}	
	}

	if(pstTaskCycleDef->CtrlFlag & CYC_CTRL_FLAG_GAY)
	{
		if(!is_gay_gong(pstEnv, pstPlayer)) 
		{
			if (!fDoNotNotify)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK201);
			}
			goto _error;
		}
	}
	if (pstTaskCycleDef->PreTaskID > 0)
	{
		if (0 > task_find_fini(pstPlayer, pstTaskCycleDef->PreTaskID))
		{
			if (!fDoNotNotify)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK301);
			}	
			goto _error;
		}
	}
	
	if (	taskcycle_check_mutex(pstEnv, pstPlayer, pstTaskCycleDef) < 0)
	{
		if (!fDoNotNotify)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK202);
		}
		goto _error;
	}

	if (taskcycle_check_group_oneday(pstEnv, pstPlayer, pstTaskCycleDef) < 0)
	{
		if (!fDoNotNotify)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK203);
		}
		goto _error;
	}

	if (pstTaskCycleDef->MinLevel > 0 && pstPlayer->stRoleData.Level < pstTaskCycleDef->MinLevel)
	{
		if (!fDoNotNotify)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK204, pstTaskCycleDef->MinLevel);
		}
		goto _error;
	}

	if (pstTaskCycleDef->MaxLevel > 0 && pstPlayer->stRoleData.Level > pstTaskCycleDef->MaxLevel)
	{
		if (!fDoNotNotify)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK205, pstTaskCycleDef->MaxLevel);
		}
		goto _error;
	}

	if (pstTaskCycleOneDay && (pstTaskCycleDef->OneDayNum > 0 && pstTaskCycleOneDay->OneDayNum >= pstTaskCycleDef->OneDayNum))
	{
		if (!fDoNotNotify)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK206);
		}
		goto _error;
	}

	if (pstWeek && (pstTaskCycleDef->WeekNum > 0 && pstWeek->WeekNum >= pstTaskCycleDef->WeekNum))
	{
		if (fDoNotNotify)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, 
				ZONEERR_TASK801,
				pstTaskCycleDef->WeekNum);
		}
		goto _error;
	}

	//奖励检查
	pstTaskCycleOne = taskcycle_run_find(pstPlayer, pstTaskCycleDef->ID);
	if (pstTaskCycleOne)
	{
		iCycleIdx = pstTaskCycleOne->TaskRecycleIdx;

		if (0 > iCycleIdx || iCycleIdx >= pstTaskCycleDef->CycleNum)
		{
			goto _error;
		}

		if (NULL == pstLevelUP)
		{
			goto _error;
		}

		pstTaskCycleAward = &pstTaskCycleDef->TaskRecycles[iCycleIdx].TaskRecycleAward;
		if (pstTaskCycleAward->Money.Type > 0)
		{
			if (0 > package_add_money_test(pstEnv, pstPlayer, pstTaskCycleAward->Money.Type,
											(pstLevelUP->ContributeMoney/100.0)*pstTaskCycleAward->Money.MoneyRate))
			{
				char szBuff[32];
				
				if (money_type_to_string(pstTaskCycleAward->Money.Type, szBuff, sizeof(szBuff)) < 0 )
				{
					strcpy(szBuff, "金钱");
				}
				
				if (!fDoNotNotify)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK207, szBuff);
				}
				
				goto _error;
			}
		}
	}

	return 0;

_error:
		
	return -1;
}

TASKRECYCLEONE *taskcycle_run_find_bytask(Player *pstPlayer, int iTaskID)
{
	int i;
	TASKRECYCLE *pstTaskCycleRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRecycle;

	for (i=0; i<pstTaskCycleRun->TaskRecycleNum; i++)
	{
		if (pstTaskCycleRun->TaskRecycles[i].TaskID == iTaskID)
		{
			return &pstTaskCycleRun->TaskRecycles[i];
		}
	}
	
	return NULL;
}

int taskcycle_rand_check(ZONESVRENV *pstEnv, Player *pstPlayer, int iCycleID, TASKDEF *pstTaskDef)
{
	int iRet = -1;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;
	TASKGETLIMIT *pstTaskLimit = &pstTaskDef->TaskGetLimit;

	if (CS_STAT_DEAD & pstPlayer->stOnline.State)
	{
		goto error;
	}

	pstTaskRunOne = task_run_find(pstPlayer, pstTaskDef->ID);
	if (pstTaskRunOne)
	{
		goto error;
	}
	else if (g_siTaskCommit == 0 && pstTaskRun->TaskRunNum >= MAX_TASK_RUN)
	{
		goto error;
	}

	if (pstTaskLimit->GetTeamRelation != TASK_TYPE_NONE)
	{
		if (NULL == player_team_get(pstEnv, pstPlayer))
		{
			goto error;
		}
	}

	if (pstTaskDef->TaskAward.Money.Type > 0)
	{
		if (0 > package_add_money_test(pstEnv, pstPlayer, pstTaskDef->TaskAward.Money.Type,
										pstTaskDef->TaskAward.Money.Val))
		{
			goto error;
		}
	}
	
	if (0 > task_accept_check_matchclient(pstEnv, pstPlayer, pstTaskDef, 0))
	{
		goto error;
	}

	if (iCycleID > 0) //环组任务
	{
		TASKRECYCLEDEF *pstTaskCycleDef;
		TASKRECYCLEONE *pstTaskCycleOne;
		
		pstTaskCycleDef = taskcycle_def_find(pstEnv, iCycleID, NULL);
		if (NULL == pstTaskCycleDef)
		{
			goto error;
		}

		if (0 > taskcycle_accept_check(pstEnv, pstPlayer, pstTaskCycleDef, 1))
		{
			goto error;
		}

		pstTaskCycleOne = taskcycle_run_find(pstPlayer, pstTaskCycleDef->ID);
		if (NULL == pstTaskCycleOne)
		{
			iRet = -2;
			goto error;
		}
	}
	
	iRet = 0;
	
final:
	return iRet;

error:
	goto final;
	return 0;
}

int player_check_clan(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);

	// 无工会
	if (!pstZoneClan /*|| pstZoneClan->stZoneClanInfo.ullCityPworldWID != pstPlayer->stRoleData.MiscInfo.PworldWID*/)
	{
		//z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "你不是公会成员");
		return -1;
	}

	if (0 == g_siTaskDrop)
	{
		// 不是本公会成员
		if (	pstPlayer->stRoleData.Map > PWORLD_MAPID_BASE &&
			pstPlayer->stRoleData.MiscInfo.PworldWID > 0 &&
			pstZoneClan->stZoneClanInfo.ullCityPworldWID != pstPlayer->stRoleData.MiscInfo.PworldWID &&
			pstZoneClan->stZoneClanInfo.ullEveryDayPworldWID != pstPlayer->stRoleData.MiscInfo.PworldWID &&
			pstZoneClan->stZoneClanInfo.ullMachinePworldWID != pstPlayer->stRoleData.MiscInfo.PworldWID)
		{
			//z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "你不是本公会成员");
			return -2;
		}
	}
	
	return 0;
}


int task_accept_check(ZONESVRENV *pstEnv, Player *pstPlayer, TASKDEF *pstTaskDef,
	                    int fDoNotNotify, int fTeamNotify)
{
	int i;
	int iRet = -TAE_INTERNAL_ERROR;
	TASKRUNONE *pstTaskRunOne = NULL;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;
	TASKGETLIMIT *pstTaskLimit = &pstTaskDef->TaskGetLimit;
	ZoneTeam *pstTeam = player_team_get(pstEnv, pstPlayer);

	if (CS_STAT_DEAD & pstPlayer->stOnline.State)
	{
		THROW_ERROR(TAE_DEAD);
	}

	// 检查组任务是否互斥
	if (pstTaskDef->IsGroupMutex > 0)
	{
		for (i=0; i<pstTaskRun->TaskRunNum; i++)
		{
			TASKDEF *pstTask = task_def_find(pstEnv,
			                                 pstTaskRun->TaskRuns[i].TaskID, 
				                             &pstTaskRun->TaskRuns[i].TaskIdx);

			if(pstTask &&
			   pstTask->IsGroupMutex >0 &&
			   pstTaskDef->LimitGroupID == pstTask->LimitGroupID)
			{
				if(!fDoNotNotify)
				{
					if(fTeamNotify && pstTeam)
					{
						z_team_sys_strf(pstEnv,  pstTeam, SYS_MIDDLE2, 
							ZONEERR_TEAM309, pstPlayer->stRoleData.RoleName);
					}
					else
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK235);
					}
				}
				THROW_ERROR(TAE_MUTEX);
			}
		}
	}

	if(pstPlayer->stOnline.fBusyCheck == 0)
	{
		//检查给予任务物品
		if (pstTaskDef->TaskItem > 0)
		{
			ROLEGRID stRoleGrid;
				
			if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid,
			                    pstTaskDef->TaskItem))
			{
				THROW_ERROR(TAE_INTERNAL_ERROR);
			}

			if (pstTaskDef->TaskItemNum > 1)
			{
				stRoleGrid.GridData.RoleItem.Num = pstTaskDef->TaskItemNum;
			}
				
			if (0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer,
			                         &stRoleGrid, 1,0))
			{
				if(!fDoNotNotify)
				{
					if(fTeamNotify && pstTeam)
					{
						z_team_sys_strf(pstEnv,  pstTeam, SYS_MIDDLE2, 
							ZONEERR_TEAM310, pstPlayer->stRoleData.RoleName);
					}
					else
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK236);
					}
				}
				THROW_ERROR(TAE_PACKAGE_FULL);
			}
		}
	}

	// 金钱检查
	if (pstTaskDef->TaskAward.Money.Type > 0)
	{
		if (0 > package_add_money_test(pstEnv,
		                               pstPlayer,
		                               pstTaskDef->TaskAward.Money.Type,
		                               pstTaskDef->TaskAward.Money.Val))
		{
			if(!fDoNotNotify)
			{
				if(fTeamNotify && pstTeam)
				{
					z_team_sys_strf(pstEnv,  pstTeam, SYS_MIDDLE2, 
						ZONEERR_TEAM311, pstPlayer->stRoleData.RoleName);
				}
				else
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK237);
				}
			}
			THROW_ERROR(TAE_MONEY_FULL);
		}
	}

	// 组队检查
	if (pstTaskLimit->GetTeamRelation != TASK_TYPE_NONE)
	{
		if (NULL == player_team_get(pstEnv, pstPlayer))
		{
			if(!fDoNotNotify)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK238);
			}
			THROW_ERROR(TAE_NEED_TEAM);
		}
	}

	
	//检查变身需求
	if (pstTaskDef->TaskFlag & TASK_BIANSHEN )
	{
		// 下马接任务
		/*if (pstPlayer->stOnline.State &(CS_STAT_RIDE_ON|CS_STAT_RIDE_PREPARE))
		{
			z_ride_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		}*/
		
		if (!is_can_bianshen(pstEnv,pstPlayer,0,1))
		{
			if(!fDoNotNotify)
			{
				if(fTeamNotify && pstTeam)
				{
					z_team_sys_strf(pstEnv,  pstTeam, SYS_MIDDLE2, 
						ZONEERR_TEAM312, pstPlayer->stRoleData.RoleName);
				}
				else
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK239);
				}
			}
			THROW_ERROR(TAE_BIANSHEN_LIMIT);
		}
	}

	// 玩家自身属性检查
	iRet = task_accept_check_matchclient(pstEnv, pstPlayer, pstTaskDef,
	                                     fTeamNotify);
	if(iRet < 0)
	{
		goto error;
	}

	if(pstTaskDef->TaskRecycleIDs[0] > 0) //环组任务
	{
		TASKRECYCLEDEF *pstTaskCycleDef = NULL;
		TASKRECYCLEONE *pstTaskCycleOne = NULL;

		pstTaskCycleOne = taskcycle_run_find_bytask(pstPlayer, pstTaskDef->ID);
		if (NULL == pstTaskCycleOne)
		{
			THROW_ERROR(TAE_NO_CYCLE);
		}
		
		pstTaskCycleDef = taskcycle_def_find(pstEnv, pstTaskCycleOne->TaskRecycleID,
		                                     NULL);
		if (NULL == pstTaskCycleDef)
		{
			THROW_ERROR(TAE_INTERNAL_ERROR);
		}

		if (0 > taskcycle_accept_check(pstEnv, pstPlayer, pstTaskCycleDef,
		                               fDoNotNotify))
		{
			THROW_ERROR(TAE_CYCLE_ERROR);
		}
	}

	// 检查重复接任务/任务上限
	pstTaskRunOne = task_run_find(pstPlayer, pstTaskDef->ID);
	if (pstTaskRunOne)
	{
		if(!fDoNotNotify)
		{
			if(fTeamNotify && pstTeam)
			{
				z_team_sys_strf(pstEnv,  pstTeam, SYS_MIDDLE2, 
					ZONEERR_TEAM313, pstPlayer->stRoleData.RoleName);
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK240);
			}
		}
		THROW_ERROR(TAE_REPEATED);
	}
	else if (pstTaskRun->TaskRunNum >= MAX_TASK_RUN)
	{
		if(!fDoNotNotify)
		{
			if(fTeamNotify && pstTeam)
			{
				z_team_sys_strf(pstEnv,  pstTeam, SYS_MIDDLE2, 
					ZONEERR_TEAM314, pstPlayer->stRoleData.RoleName);
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK241);
			}
		}
		THROW_ERROR(TAE_TASK_FULL);
	}

	// 消耗金钱检查
	if (pstTaskDef->ConsumeMoney.Type > 0)
	{
		if (0 > package_dec_money_test(pstEnv,
		                               pstPlayer,
		                               pstTaskDef->ConsumeMoney.Type,
		                               pstTaskDef->ConsumeMoney.Val))
		{
			if(!fDoNotNotify)
			{
				if(fTeamNotify && pstTeam)
				{
					z_team_sys_strf(pstEnv,  pstTeam, SYS_MIDDLE2, 
						ZONEERR_TEAM315, pstPlayer->stRoleData.RoleName);
				}
				else
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK242);
				}
			}
			THROW_ERROR(TAE_NOT_ENOUGH_MONEY);
		}
	}

	iRet = 0;
	
final:
	return iRet;

error:
	
	goto final;
	return 0;
}

int task_trace_init(Player *pstPlayer, TASKRUNONE *pstTaskRunOne, TASKDEF *pstTaskDef)
{
	int i;
	TASKMONTRACEONE *pstTaskMonTraceOne;
	TASKITEMTRACEONE *pstTaskItemTraceOne;

	pstTaskRunOne->TaskMonTrace.TaskMonTraceNum = 0;
	for (i=0; i<MAX_TASK_MON; i++)
	{
		if ((pstTaskDef->KillMon[i].MonID1 == 0 && pstTaskDef->SpecialTask[0].FiniObjType != TASK_OBJ_LVRANGE_MONSTER) ||
			  pstTaskDef->KillMon[i].Num == 0)
		{
			break;
		}

		pstTaskMonTraceOne = &pstTaskRunOne->TaskMonTrace.TaskKillMons[pstTaskRunOne->TaskMonTrace.TaskMonTraceNum++];
		pstTaskMonTraceOne->MonID = pstTaskDef->KillMon[i].MonID1;
		pstTaskMonTraceOne->MaxNum = pstTaskDef->KillMon[i].Num;
		pstTaskMonTraceOne->Num = 0;
	}

	pstTaskRunOne->TaskItemTrace.TaskItemTraceNum = 0;
	for (i=0; i<MAX_TASK_ITEM; i++)
	{
		if(pstTaskDef->GetItem[i].ItemID == 0)
		{
			break;
		}

		pstTaskItemTraceOne = &pstTaskRunOne->TaskItemTrace.TaskGetItems[pstTaskRunOne->TaskItemTrace.TaskItemTraceNum++];
		pstTaskItemTraceOne->ItemID = pstTaskDef->GetItem[i].ItemID;
		pstTaskItemTraceOne->MaxNum = pstTaskDef->GetItem[i].Num;
	}

	if(pstTaskDef->SJID > 0)
	{
		sj_task_trace_init(pstPlayer, pstTaskRunOne, pstTaskDef);
	}
	
	return 0;
}

TASKRUNONE *task_add_taskrun(ZONESVRENV *pstEnv, Player *pstPlayer, TASKDEF *pstTaskDef)
{
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	if (pstTaskRun->TaskRunNum >= MAX_TASK_RUN)
	{
		return NULL;
	}

	if (task_run_find(pstPlayer, pstTaskDef->ID))
	{
		return NULL;
	}

	pstTaskRunOne = &pstTaskRun->TaskRuns[pstTaskRun->TaskRunNum];
	memset(pstTaskRunOne, 0, sizeof(*pstTaskRunOne));
	pstTaskRunOne->TaskID = pstTaskDef->ID;
	pstTaskRunOne->TaskStat = TASK_STAT_RUN;
	pstTaskRunOne->TaskIdx = pstTaskDef - (TASKDEF *)pstEnv->pstZoneObj->sTaskDef;
	pstTaskRunOne->TaskGetTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	if (pstTaskDef->TaskRecycleIDs[0] > 0)
	{
		TASKRECYCLEONE * pstTaskCycleOne = taskcycle_run_find_bytask(pstPlayer, pstTaskDef->ID);
		if (pstTaskCycleOne)
		{
			pstTaskRunOne->CycleID = pstTaskCycleOne->TaskRecycleID;
		}
	}
	
	// 初始化通用进度
	pstTaskRunOne->TaskGenericProgress.Progress = 0;
	pstTaskRunOne->TaskGenericProgress.Max = pstTaskDef->MaxGenericProgress;
	task_trace_init(pstPlayer, pstTaskRunOne, pstTaskDef);

	pstTaskRun->TaskRunNum++;

	return pstTaskRunOne;
}

void task_one_csinfo(ZONESVRENV *pstEnv, Player *pstPlayer, CSTASKRUNONE *pstCSTaskRunOne, TASKRUNONE *pstTaskRunOne)
{
	TASKDEF *pstTaskDef;
		
	pstCSTaskRunOne->TaskID = pstTaskRunOne->TaskID;
	pstCSTaskRunOne->TaskStat = pstTaskRunOne->TaskStat;
	memcpy(&pstCSTaskRunOne->TaskMonTrace, &pstTaskRunOne->TaskMonTrace, sizeof(pstCSTaskRunOne->TaskMonTrace));
	memcpy(&pstCSTaskRunOne->TaskItemTrace, &pstTaskRunOne->TaskItemTrace, sizeof(pstCSTaskRunOne->TaskItemTrace));
	// 填充通用进度
	pstCSTaskRunOne->TaskGenericProgress = pstTaskRunOne->TaskGenericProgress;
	pstCSTaskRunOne->TaskCycleIdx = 0;
	pstCSTaskRunOne->CycleID = g_siCycleID;
	pstCSTaskRunOne->SJTaskStar = pstTaskRunOne->SJTaskStar;

	pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
	if (pstTaskDef)
	{
		if ( (int)(pstTaskRunOne->TaskGetTime + pstTaskDef->TaskTimer) > pstEnv->pstAppCtx->stCurr.tv_sec)
			pstCSTaskRunOne->TaskEndTime =pstTaskRunOne->TaskGetTime + pstTaskDef->TaskTimer - pstEnv->pstAppCtx->stCurr.tv_sec;
		else
		{
			pstCSTaskRunOne->TaskEndTime = 0;
		}
		
		if ( pstTaskRunOne->CycleID > 0)
		{
			TASKRECYCLEONE *pstTaskCycleOne;

			pstTaskCycleOne = taskcycle_run_find(pstPlayer, pstTaskRunOne->CycleID);
			if (pstTaskCycleOne)
			{
				pstCSTaskRunOne->TaskCycleIdx = pstTaskCycleOne->TaskRecycleIdx;
				pstCSTaskRunOne->CycleID = pstTaskCycleOne->TaskRecycleID;
			}
		}
	}
	
	return;
}

int gm_task_accept(ZONESVRENV *pstEnv, Player *pstPlayer, int iTaskID)
{
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
				
	pstTaskDef = task_def_find(pstEnv, iTaskID, NULL);
	if (NULL == pstTaskDef)
	{
		return -1;
	}

	pstTaskRunOne = task_add_taskrun(pstEnv, pstPlayer, pstTaskDef);
	if (NULL == pstTaskRunOne)
	{
		return -1;
	}

	task_chg(pstEnv, pstPlayer, pstTaskRunOne);

	return 0;
}


int gm_task_add(ZONESVRENV *pstEnv, Player *pstPlayer, int iTaskID)
{
	int k;
	int iRet; 
	TASKDEF *pstTaskDef;
	TASKRECYCLEDEF *pstTaskCycleDef = NULL;
	TASKRECYCLEONE *pstTaskCycleOne;
	int iCyc = 0;
	

	pstTaskDef = task_def_find(pstEnv, iTaskID, NULL);
	if (!pstTaskDef)
	{
		return -1;
	}

	iRet = task_accept_check(pstEnv, pstPlayer, pstTaskDef, 0, 0);

	if (0 > iRet && -2 != iRet)
	{
		return -1;
	}

	pstTaskCycleOne = NULL;
	for (k=0; k<MAX_TASK_TASKCYCLE; k++)
	{
		if (0 == pstTaskDef->TaskRecycleIDs[0] )
		{
			break;
		}

		iCyc = 1;
		pstTaskCycleOne = taskcycle_run_find(pstPlayer, pstTaskDef->TaskRecycleIDs[k]);
		if (pstTaskCycleOne)
		{
			break;
		}
	}

	if (iCyc)
	{
		if (NULL == pstTaskCycleOne)
		{
			//z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "请先接一个环组任务再使用gm指令指定任务吧");
			return -1;
		}
		else
		{
			pstTaskCycleDef = taskcycle_def_find(pstEnv, pstTaskCycleOne->TaskRecycleID, NULL);
			if (NULL == pstTaskCycleDef)
			{
				return -1;
			}

			pstTaskCycleOne->TaskID = iTaskID;
		}
	}
		
	if (0 > task_accept(pstEnv, pstPlayer, iTaskID))
	{
		return -1;
	}

	return 0;
}


static int task_get_montrace_kill_num(Player *pstPlayer, int iMonID,int iTaskID)
{
	int iNum = 0;
	int i = 0;
	int j = 0;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;
	
	for (i=0; i<pstTaskRun->TaskRunNum; i++)
	{
		if (pstTaskRun->TaskRuns[i].TaskID != iTaskID)
		{
			continue;
		}
		
		for (j=0; j<pstTaskRun->TaskRuns[i].TaskMonTrace.TaskMonTraceNum; j++)
		{
			if ((int)pstTaskRun->TaskRuns[i].TaskMonTrace.TaskKillMons[j].MonID == iMonID)
			{
				return pstTaskRun->TaskRuns[i].TaskMonTrace.TaskKillMons[j].Num;
			}
		}
	}
	return iNum;
}

int task_login_check_trace(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;
	TASKDEF *pstTaskDef;
	int i;
	int j;
	TASKMONTRACE stTrace;

	for (i=0; i<pstTaskRun->TaskRunNum; i++)
	{
		stTrace.TaskMonTraceNum = 0;
		pstTaskDef = task_def_find(pstEnv, pstTaskRun->TaskRuns[i].TaskID, NULL);
		if (!pstTaskDef)
			continue;

		if(pstTaskDef->Type == TASK_TYPE_SJ_MON ||
			pstTaskDef->Type == TASK_TYPE_SJ_ITEM)
		{
			continue;
		}

		for (j=0; j<MAX_TASK_MON; j++)
		{
			if ((pstTaskDef->KillMon[j].MonID1 == 0 && pstTaskDef->SpecialTask[0].FiniObjType != TASK_OBJ_LVRANGE_MONSTER) || 
				pstTaskDef->KillMon[j].Num == 0)
			{
				break;
			}	

			stTrace.TaskKillMons[stTrace.TaskMonTraceNum].MonID = pstTaskDef->KillMon[j].MonID1;
			stTrace.TaskKillMons[stTrace.TaskMonTraceNum].MaxNum = pstTaskDef->KillMon[j].Num;
			stTrace.TaskKillMons[stTrace.TaskMonTraceNum].Num = 
					task_get_montrace_kill_num(pstPlayer, pstTaskDef->KillMon[j].MonID1,pstTaskDef->ID);
			stTrace.TaskMonTraceNum ++;
		}

		pstTaskRun->TaskRuns[i].TaskMonTrace = stTrace;
	}
	
	return 0;
}

int task_accept(ZONESVRENV *pstEnv, Player *pstPlayer, int iTaskID)
{	
	int iRet;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRECYCLEONE *pstTaskCycleOne = NULL;
	int iDropInterVal = TASK_CYCLE_DROP_COOL;
	TASKRECYCLEDEF *pstTaskCycleDef;
	char szTip[32];
	int iOldTaskStat;
					
	pstTaskDef = task_def_find(pstEnv, iTaskID, NULL);
	if (NULL == pstTaskDef)
	{
		return -1;
	}

	if (pstTaskDef->Type == TASK_TYPE_DART_CAR )
	{
		if (	player_is_dartcar_limit(pstPlayer) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK243);
			return -1;
		}
	}

	if (pstTaskDef->Type == TASK_TYPE_RONGLU)
	{
		if (package_get_freegrid_num(pstEnv->pstAppCtx, pstEnv, pstPlayer, LIST_TYPE_SUNDRIES) <=0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK244);
			return -1;
		}
	}
	

	if (pstTaskDef->TaskRecycleIDs[0] > 0)
	{
		pstTaskCycleOne = taskcycle_run_find_bytask(pstPlayer, pstTaskDef->ID);
		if (NULL == pstTaskCycleOne)
		{
			return -1;
		}
		else
		{
			if (pstTaskCycleOne->TaskRecycleIdxDropTime != 0 && 
				pstEnv->pstAppCtx->stCurr.tv_sec - pstTaskCycleOne->TaskRecycleIdxDropTime < iDropInterVal)
			{
				sec_trans_char(iDropInterVal, szTip, sizeof(szTip));
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK245,szTip);
				return -1;
			}
		}
		
		pstTaskCycleDef = taskcycle_def_find(pstEnv, pstTaskCycleOne->TaskRecycleID, NULL);
		if(!pstTaskCycleDef)
		{
			return -1;
		}

		if (pstTaskCycleDef->DropInterVal > 0)
		{
			iDropInterVal = pstTaskCycleDef->DropInterVal;
		}
	}

	if (0 > task_accept_check(pstEnv, pstPlayer, pstTaskDef, 0, 0))
	{
		return -1;
	}

	pstTaskRunOne = task_add_taskrun(pstEnv, pstPlayer, pstTaskDef);
	if (NULL == pstTaskRunOne)
	{
		return -1;
	}

	//扣除金钱
	if (pstTaskDef->ConsumeMoney.Type > 0)
	{
		if (0 > (iRet = package_dec_money(pstEnv,
							pstPlayer,
							pstTaskDef->ConsumeMoney.Type,
							pstTaskDef->ConsumeMoney.Val)))
		{
			return -1;
		}

		if (pstTaskDef->ConsumeMoney.Type == ATTR_ID_MONEY)
		{
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_TASK, pstTaskDef->ID, 0, pstTaskDef->ConsumeMoney.Val, "任务");
		}
		else if (pstTaskDef->ConsumeMoney.Type == ATTR_ID_BULL)
		{
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_TASK, pstTaskDef->ID, pstTaskDef->ConsumeMoney.Val-iRet,iRet,  "任务");
		}
	}

	//给予任务物品
	if (pstTaskDef->TaskItem > 0)
	{
		ROLEGRID stRoleGrid;
			
		if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstTaskDef->TaskItem))
		{
			return -1;
		}

		if (pstTaskDef->TaskItemNum > 1)
		{
			stRoleGrid.GridData.RoleItem.Num = pstTaskDef->TaskItemNum;
		}
		
		package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0,0);
	}

	// 检查通过了  ，变身任务 下马，在应用状态之前
	if (pstTaskDef->TaskFlag & TASK_BIANSHEN )
	{
		if (pstPlayer->stOnline.State &(CS_STAT_RIDE_ON|CS_STAT_RIDE_PREPARE))
		{
			z_ride_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		}
	}
	
	//给予任务Buff
	if (pstTaskDef->TaskBuff.TaskBuffID > 0)
	{
		z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstTaskDef->TaskBuff.TaskBuffID, 
								pstTaskDef->TaskBuff.TaskBuffLevel, pstTaskDef->TaskBuff.TaskBuffTime, 1, 0);
	}

	iOldTaskStat = pstTaskRunOne->TaskStat;
	//任务脚本
	if ((pstTaskDef->TaskScript & TASK_SCRIPT_PROC_ACCEPT_MAP))
	{	
		ScriptIndex *pstScriptInd;
		char szScript[32];	
	
		stSEnv.pstMonster = NULL;
		stSEnv.pstNpc = NULL;
		stSEnv.pstPlayer = pstPlayer;
		stSEnv.pstMapinst = NULL;
		stSEnv.pstKiller = NULL;
		
		snprintf(szScript, sizeof(szScript), "task%d.mac", pstTaskDef->ID);				
		pstScriptInd = get_script_ind(szScript);

		if ( !pstScriptInd )
		{
			printf("get_script_ind fail\n");
			tlog_error(pstEnv->pstLogCat, 0, 0, "get_script_ind fail");
			return -1;
		}
		
		z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, szScript, pstScriptInd, TASK_SCRIPT_PROC_ACCEPT);
	}

	if(iOldTaskStat == pstTaskRunOne->TaskStat)
	{
		task_chg(pstEnv, pstPlayer, pstTaskRunOne);
	}

	if (pstTaskCycleOne)
	{
		pstTaskCycleOne->Stat = TASKCYCLE_STAT_RUN;
	}

	// 让脚本刷吧
	//if (pstTaskDef->Type == TASK_TYPE_DART_CAR)
	//{
	//	new_mon_dart_car_by_task(pstEnv,pstPlayer, 0, pstTaskDef->TaskTimer);
	//}

	task_check_trace(pstEnv, pstPlayer);

	task_special_fini(pstEnv, pstPlayer, pstTaskDef, pstTaskRunOne);

	/*
	if (pstTaskCycleOne)
	{
		TASKRECYCLEDEF *pstTaskCycleDef;
	
		pstTaskCycleDef = taskcycle_def_find(pstEnv, pstTaskCycleOne->TaskRecycleID, NULL);
		if (pstTaskCycleDef)
		{
			if(pstTaskCycleDef->CycType != CYC_TYPE_PLOY)
			{
				snprintf(szMsg, sizeof(szMsg), "接受%s 环组第%d 环", pstTaskCycleDef->Name, pstTaskCycleOne->TaskRecycleIdx+1);
				z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, szMsg);
			}
		}
	}
	*/
	
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_TASK246,pstTaskDef->Name);

	//刺探任务
	if (pstTaskDef->ID == CLAN_SPY_TASK_ID)
	{
		pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_SPY;
		pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_SPY_FINI;
		pstPlayer->stRoleData.MiscInfo.DailyInfo.SpyInfo.TargetClanID = 0;
		player_action_view_flag_chg(pstEnv,pstPlayer);
	}

	if (pstTaskDef->ID == CLAN_ANTI_SPY_TASK_ID)
	{
		pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_ANTI_SPY;
		pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_ANTI_SPY_FINI;
		player_action_view_flag_chg(pstEnv,pstPlayer);
	}

	if (pstTaskDef->Type == TASK_TYPE_RONGLU)
	{
		ronglu_one_chg(pstEnv, pstPlayer,pstPlayer->stRoleData.MiscInfo.RongluInfo.ItemID, iTaskID);
	}
	else if (pstTaskDef->Type == TASK_TYPE_DART_CAR)
	{
		pstPlayer->stRoleData.MiscInfo.DartCarInfo.AddRate = 0;
		if (clan_is_open_dart_by_role(pstEnv, pstPlayer))
		{
			pstPlayer->stRoleData.MiscInfo.DartCarInfo.AddRate = 10;
		}
	}

	//接取跑商任务
	if (pstTaskDef->Type == TASK_TYPE_PAOSHANG)
	{
		player_paoshang_clean(pstEnv,pstPlayer);
		pstPlayer->stRoleData.MiscInfo.PaoShang.YinPiao = TASK_PAOSHANG_YINPIAO;
		int j;
		for (j=0; j<MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType == 0)
			{
				break;
			}
			
			if(pstTaskDef->SpecialTask[j].FiniObjType == TASK_OBJ_PAOSHANG)
			{
				pstPlayer->stRoleData.MiscInfo.PaoShang.JiaoPiao = pstTaskDef->SpecialTask[j].FiniObjVal;
				break;
			}
		}
	}
	
	//任务日志相关流水号
	z_oplog_event_begin(pstEnv);
	// OP LOG
	z_role_task_oplog( pstEnv, pstPlayer, (unsigned short)iTaskID, 
			OP_TASK_ACCEPT,NULL,0,0,0,0, pstTaskDef->ConsumeMoney.Val);

	//成长之路
	{
		ACHIEVEMENTCOND stCond;

		memset(&stCond,0,sizeof(stCond));
		
		stCond.Type = COND_TASK_ACCEPT;
		stCond.Val1 = iTaskID;

		z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
	}

	return 0;
}

int taskcycle_rand_task(ZONESVRENV *pstEnv, Player *pstPlayer, TASKRECYCLEDEF *pstTaskCycleDef)
{
	int i, iRet;
	TASKDEF *pstTaskDef;
	TaskCycleRandTaskID stTmp;
	TaskCycleRandTaskID *pstTaskCycleRandID = (TaskCycleRandTaskID *)((char *)pstTaskCycleDef + sizeof(*pstTaskCycleDef));

	stTmp.iNum = 0;

	for (i=0; i<pstTaskCycleRandID->iNum; i++)
	{
		pstTaskDef = task_def_find(pstEnv, pstTaskCycleRandID->aiTaskID[i], NULL);
			
		if (NULL == pstTaskDef)
		{
			continue;
		}

		iRet = taskcycle_rand_check(pstEnv, pstPlayer, pstTaskCycleDef->ID, pstTaskDef);
		if (0 > iRet  && -2 != iRet)
		{
			continue;
		}

		stTmp.aiTaskID[stTmp.iNum++] = pstTaskDef->ID;
	}
	
	if (0 >= stTmp.iNum)
	{
		return -1;
	}

	return stTmp.aiTaskID[(int)RAND1(stTmp.iNum)];
}

int taskrecycle_get_task(ZONESVRENV *pstEnv, Player *pstPlayer, TASKRECYCLEDEF *pstTaskCycleDef, int iCycleIdx, TASKRECYCLEONE *pstTaskCycleOne)
{
	int iTaskID;
	TASKRECYCLEENTRY *pstTaskCycleEntry;
	int i;
	int iHideTask = 0;

	if (0 > iCycleIdx || iCycleIdx >= pstTaskCycleDef->CycleNum)
	{
		return -1;
	}
	
	pstTaskCycleEntry = &pstTaskCycleDef->TaskRecycles[iCycleIdx];
	if (pstTaskCycleEntry->Rate > 0)
	{
		if (RAND1(10000) >= pstTaskCycleEntry->Rate)
		{
			//没有命中，就随机
			return -1;
		}
		else
		{
			//最后一个任务
			if(iCycleIdx + 1 == pstTaskCycleDef->CycleNum && pstTaskCycleEntry->Rate != 10000)
			{
				// 指定隐藏任务随机
				if (pstTaskCycleDef->HideAccType == HIDE_ACC_TYPE_ONE_RAND)
				{
					for (i=0; i<MAX_HIDE_TASK_IN_CYC; i++)
					{
						if (pstTaskCycleDef->HideTaskLists[i] <= 0)
						{
							break;
						}
						iHideTask++;
					}	

					if (iHideTask > 0)
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_TASK247, pstTaskCycleDef->Name);
						return pstTaskCycleDef->HideTaskLists[(int)RAND1(iHideTask)];
					}
					else
					{
						return -1;
					}
				}
				z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_TASK247, pstTaskCycleDef->Name);
			}
		}
	}

	if (0 == pstTaskCycleEntry->TaskID)
	{
		iTaskID = taskcycle_rand_task(pstEnv, pstPlayer, pstTaskCycleDef);
	}
	else
	{
		iTaskID = pstTaskCycleEntry->TaskID;
	}

	return iTaskID;
	UNUSED(pstTaskCycleOne);	//add by paraunused.pl
}

int taskcycle_add_run(ZONESVRENV *pstEnv, Player *pstPlayer, TASKRECYCLEDEF *pstTaskCycleDef, int iTaskID)
{
	int i;
	int iIndex;
	TASKRECYCLEONE *pstTaskCycleOne = NULL;
	TASKRECYCLE *pstTaskCycle = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRecycle;
	
	if (taskcycle_run_find(pstPlayer, pstTaskCycleDef->ID))
	{
		return -1;
	}

	if (pstTaskCycle->TaskRecycleNum >= MAX_TASK_RUN)
	{
		iIndex = -1;
		for (i=0; i<pstTaskCycle->TaskRecycleNum; i++)
		{
			if (pstTaskCycle->TaskRecycles[i].Stat == TASKCYCLE_STAT_INIT)
			{
				if (-1 == iIndex)
				{
					iIndex = i;
				}
				else
				{
					if (pstTaskCycle->TaskRecycles[i].GetTime < pstTaskCycle->TaskRecycles[iIndex].GetTime)
					{
						iIndex = i;
					}
				}
			}
		}

		if (-1 == iIndex)
		{
			return -1;
		}
		else
		{
			pstTaskCycleOne = &pstTaskCycle->TaskRecycles[iIndex];
		}
	}
	else
	{
		pstTaskCycleOne = &pstTaskCycle->TaskRecycles[pstTaskCycle->TaskRecycleNum++];
	}

	if (NULL == pstTaskCycleOne)
	{
		if (pstTaskCycle->TaskRecycleNum >= MAX_TASK_RUN)
		{
			return -1;
		}
		else
		{
			pstTaskCycleOne = &pstTaskCycle->TaskRecycles[pstTaskCycle->TaskRecycleNum++];
		}
	}
	
	memset(pstTaskCycleOne, 0, sizeof(*pstTaskCycleOne));
	pstTaskCycleOne->TaskRecycleID = pstTaskCycleDef->ID;
	pstTaskCycleOne->TaskID = iTaskID;
	pstTaskCycleOne->GetTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstTaskCycleOne->Stat = TASKCYCLE_STAT_INIT;

	return 0;
}

int task_dialog(ZONESVRENV *pstEnv, Player *pstPlayer, int iNPCID, int iTaskID, int iDialogType)
{
	TASKDEF *pstTaskDef;
	CSPKG stPkg;
	CSTASKSVR *pstTaskSvr = &stPkg.Body.TaskSvr;
	Npc *pstNpc;

	pstTaskSvr->OpType = TASK_DIALOG;
	pstTaskSvr->TaskSvrData.TaskDialog.NpcID = iNPCID;
	pstTaskSvr->TaskSvrData.TaskDialog.TaskID = iTaskID;
	pstTaskSvr->TaskSvrData.TaskDialog.Type = iDialogType;
	pstTaskSvr->TaskSvrData.TaskDialog.TaskCycleIdx = 0;
	pstTaskSvr->TaskSvrData.TaskDialog.CycleID = 0;

	pstTaskDef = task_def_find(pstEnv, iTaskID, NULL);
	if (pstTaskDef && pstTaskDef->TaskRecycleIDs[0] > 0)
	{
		TASKRECYCLEONE *pstTaskCycleOne;

		pstTaskCycleOne = taskcycle_run_find_bytask(pstPlayer, pstTaskDef->ID);
		if (pstTaskCycleOne)
		{
			pstTaskSvr->TaskSvrData.TaskDialog.TaskCycleIdx = pstTaskCycleOne->TaskRecycleIdx;
			pstTaskSvr->TaskSvrData.TaskDialog.CycleID = pstTaskCycleOne->TaskRecycleID;
		}
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, TASK_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, iNPCID);
	if(pstNpc)
	{
		pstNpc->stMon.tFreezeEnd = pstEnv->pstAppCtx->stCurr.tv_sec + 30;
		if (pstNpc->stMon.cMoving)
		{
			z_mon_stop(pstEnv, &pstNpc->stMon);
		}
	}

	return 0;
}

int task_run_del(ZONESVRENV* pstEnv, Player *pstPlayer, int iTaskID)
{
	int i;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i=pstTaskRun->TaskRunNum-1; i>=0; i--)
	{
		if (pstTaskRun->TaskRuns[i].TaskID == iTaskID)
		{
			pstTaskRun->TaskRunNum--;
			if (i != pstTaskRun->TaskRunNum)
			{
				pstTaskRun->TaskRuns[i] = pstTaskRun->TaskRuns[pstTaskRun->TaskRunNum];
			}

			task_check_trace(pstEnv, pstPlayer);
			return 0;
		}
	}

	return -1;
}
int task_team_share_clear(ZONESVRENV *pstEnv, Player *pstPlayer, ZoneTeam *pstTeam, int iTaskID)
{
	TEAMMEMBER *pstMember;
	Player *pstTmpPlayer;
	int i;
	
	for (i=pstTeam->stTeamInfo.MemberNum-1; i>=0; i--)
	{
		pstMember = &pstTeam->stTeamInfo.TeamMember[i];
		if (pstMember->OffLineTime > 0)
		{
			continue;
		}
		
		pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstMember->MemberName);
		if(NULL == pstTmpPlayer || pstTmpPlayer == pstPlayer)
		{
			continue;
		}

		//搞无效
		if(pstTmpPlayer->stOnline.iSharingTaskID == iTaskID)
		{
			pstTmpPlayer->stOnline.tSharingTime = pstEnv->pstAppCtx->stCurr.tv_sec - TASK_SHARING_ACCEPT_TIME;
		}
	}
	
	return 0;
}


// 商运镖车时间到了，设置任务失败
int task_fail_by_dartcar_die(ZONESVRENV *pstEnv, Monster *pstMon)
{
	Player *pstPlayer;
//	int i;
//	TASKINFO *pstTaskInfo;
//	TASKDEF *pstTaskDef;
	TASKRUNONE *pstOne;

	if (pstMon->cDartCarNotAtk > 0)
	{
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, 
								this_machine(pstEnv,pstMon)->stMachineInfo.OwnerName);
		if (!pstPlayer)
			return 0;

		pstOne = player_get_dart_car_task(pstEnv, pstPlayer);
		if (pstOne)
		{
			pstOne->TaskStat = TASK_STAT_FAIL;
			task_chg(pstEnv, pstPlayer, pstOne);
		}

	}
	return 0;
}


int task_clear_player_dart_info(Player *pstPlayer)
{
	//pstPlayer->stRoleData.MiscInfo.DartCarInfo.AwardRate = 0;
	pstPlayer->stRoleData.MiscInfo.DartCarInfo.MonMemID = 0;
	return 0;
}

TASKRUNONE *player_get_dart_car_task(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	
	TASKINFO *pstTaskInfo = &pstPlayer->stRoleData.MiscInfo.TaskInfo;
	int i;
	TASKDEF *pstTaskDef;

	for (i=0; i<pstTaskInfo->TaskRun.TaskRunNum; i++)
	{
		pstTaskDef = task_def_find(pstEnv, pstTaskInfo->TaskRun.TaskRuns[i].TaskID, NULL);
		if (!pstTaskDef)
			continue;
		
		if (pstTaskDef->Type == TASK_TYPE_DART_CAR)
		{
			return &pstTaskInfo->TaskRun.TaskRuns[i];
		}
	}

	return NULL;
}


// 商运镖车任务结束(放弃，提交等)，清理数据
int task_end_dart_car_clear(ZONESVRENV *pstEnv, Player *pstPlayer, TASKDEF *pstTaskDef)
{
	Monster *pstMon;

	if (pstPlayer->stRoleData.MiscInfo.DartCarInfo.MonMemID <= 0)
		return 0;
	
	if (pstTaskDef->Type == TASK_TYPE_DART_CAR)
	{
		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, 
						pstPlayer->stRoleData.MiscInfo.DartCarInfo.MonMemID);
		if(pstMon)
		{
			z_ride_machine_clear(pstEnv, pstMon);
			z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);
			z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);
		}
		task_clear_player_dart_info(pstPlayer);
	}

	return 0;
}


// 镖车只给这一个道具奖励
int task_dart_car_item_award(ZONESVRENV *pstEnv, Player *pstPlayer, TASKDEF *pstTaskDef, 
								ROLEGRID *pstGrid, int *piGrid)
{
	int iNum = pstPlayer->stRoleData.MiscInfo.DartCarInfo.AwardNum;

	if (iNum <= 0)
		return 0;
	
	if (pstTaskDef->Type == TASK_TYPE_DART_CAR)
	{
		if (0 > item_create(pstEnv->pstAppCtx, pstEnv, pstGrid, 5060030))
		{
			return -1;
		}
		
		pstGrid->GridData.RoleItem.Num = iNum;
		*piGrid = 1;
	}
	return 0;
}

int task_login_check_dart_car(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	Monster *pstMon;
	TASKRUNONE *pstOne = player_get_dart_car_task(pstEnv, pstPlayer);

	if (pstOne && pstOne->TaskStat == TASK_STAT_RUN)
	{
		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv,pstPlayer->stRoleData.MiscInfo.DartCarInfo.MonMemID);
		if (!pstMon || !is_mon_biaoche(pstMon))
		{
			pstOne->TaskStat = TASK_STAT_FAIL;
			task_clear_player_dart_info(pstPlayer);
			return 0;
		}

		if(this_machine(pstEnv, pstMon)->stMachineInfo.OwnerType != MACHINE_OWNER_SELF ||
			this_machine(pstEnv, pstMon)->stMachineInfo.OwnerID != pstPlayer->stRoleData.RoleID)
		{
			pstOne->TaskStat = TASK_STAT_FAIL;
			task_clear_player_dart_info(pstPlayer);
			return 0;
		}
	}
	return 0;
}

int task_drop_convoy_clear(ZONESVRENV *pstEnv, Player *pstPlayer, TASKDEF *pstTaskDef)
{
	ZoneTeam *pstTeam;
	Player *pstTmpPlayer;
	TEAMMEMBER *pstMember;
	TASKRUNONE *pstTaskRun;
	int i;
	
	if(!is_convoy_task(pstTaskDef))
	{
		return 0;
	}

	if(pstTaskDef->Type == TASK_TYPE_SINGLE_CONVOY)
	{
		Monster *pstMon;

		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, 
						pstPlayer->stRoleData.MiscInfo.ConvoyInfo.ConvoyMonID);
		if(pstMon)
		{
			z_mon_destroy(pstEnv, pstMon);
		}
	}

	//清理运镖信息
	z_clear_convoy_info(pstPlayer);

	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(NULL == pstTeam)
	{
		return 0;
	}

	//队长放弃才检查
	pstTmpPlayer = player_team_get_captain_player(pstEnv, pstTeam);
	if(NULL == pstTmpPlayer || pstTmpPlayer != pstPlayer)
	{
		return 0;
	}

	for (i=pstTeam->stTeamInfo.MemberNum-1; i>=0; i--)
	{
		pstMember = &pstTeam->stTeamInfo.TeamMember[i];
		if (pstMember->OffLineTime > 0)
		{
			continue;
		}
		
		pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstMember->MemberName);
		if(NULL == pstTmpPlayer || pstTmpPlayer == pstPlayer)
		{
			continue;
		}

		//有一个其他队员接取了任务就可以进行下去。
		pstTaskRun = task_run_find(pstTmpPlayer, pstTaskDef->ID);
		if(pstTaskRun && pstTaskRun->TaskStat != TASK_STAT_FAIL)
		{
			return 0;
		}
	}

	task_team_share_clear(pstEnv, pstPlayer, pstTeam, pstTaskDef->ID);
	
	return 0;
}


int task_drop_check(ZONESVRENV *pstEnv, Player *pstPlayer,TASKDEF *pstTaskDef)
{
	int iRet;
	
	if (pstTaskDef->TaskItem > 0)
	{
		int iItemNum = 1;
		
		if (pstTaskDef->TaskItemNum > 1)
		{
			iItemNum = pstTaskDef->TaskItemNum;
		}
		
		iRet =  package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstTaskDef->TaskItem, iItemNum, -1, NULL,PACKAGE_DEC_NONE, 0);

		if (0 == pstTaskDef->TaskItemDropNoFlag && 0 > iRet)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_TASK208);
			return -1;
		}
	}

	if (pstTaskDef->TaskBuff.TaskBuffID > 0)
	{
		if (0 == z_player_del_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstTaskDef->TaskBuff.TaskBuffID, 1, 0))
		{
			z_player_send_status(pstEnv->pstAppCtx, pstEnv, pstPlayer);
			z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		}
	}

	return 0;
}

int task_drop(ZONESVRENV *pstEnv, Player *pstPlayer, int iTaskID)
{
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	int i;
	int iNotify = 0;

	if(pstEnv->iInstMode != INST_MODE_ZONE_SVR)
	{
		//跨服野外可以丢弃任务
		if(0 > player_if_in_span_map(pstEnv,pstPlayer))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_BATTLE_SVR_OP);
			return -1;
		}
	}

	if (iTaskID == BIRTH_PLAYER_TASK)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_TASK209);
		return -1;
	}
	
	pstTaskRunOne = task_run_find(pstPlayer, iTaskID);
	if (NULL == pstTaskRunOne)
	{
		return -1;
	}

	pstTaskDef = task_def_find(pstEnv, iTaskID, &pstTaskRunOne->TaskIdx);
	if (NULL == pstTaskDef)
	{
		return -1;
	}

	if (task_drop_check(pstEnv, pstPlayer,pstTaskDef) < 0)
	{
		return -1;
	}

	//放弃也算作完成任务组
	if (pstTaskDef->LimitGroupID > 0)
	{
		TASKLIMITGROUP *pstLimitGroup = task_limit_group_def_find(pstEnv,pstTaskDef->LimitGroupID, NULL);
		if(pstLimitGroup && pstLimitGroup->DropFlag)
		{
			group_add_oneday(pstEnv, pstPlayer, pstTaskDef->LimitGroupID);
			iNotify = 1;
		}
	}

	pstTaskRunOne->TaskStat = TASK_STAT_DROP;
	task_chg(pstEnv, pstPlayer, pstTaskRunOne);

	//任务脚本
	if ((pstTaskDef->TaskScript & TASK_SCRIPT_PROC_DROP_MAP))
	{	
		ScriptIndex *pstScriptInd;
		char szScript[32];	
	
		stSEnv.pstMonster = NULL;
		stSEnv.pstNpc = NULL;
		stSEnv.pstPlayer = pstPlayer;
		stSEnv.pstMapinst = NULL;
		stSEnv.pstKiller = NULL;
		
		snprintf(szScript, sizeof(szScript), "task%d.mac", pstTaskDef->ID);				
		pstScriptInd = get_script_ind(szScript);

		if ( !pstScriptInd )
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "task_drop_check:get_script_ind fail(TaskID=%d)",pstTaskDef->ID);
			return -1;
		}
		
		z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, szScript, pstScriptInd, TASK_SCRIPT_PROC_DROP);
	}

	task_run_del(pstEnv, pstPlayer, iTaskID);	
	if (pstTaskDef->TaskRecycleIDs[0] > 0)
	{
		TASKRECYCLEONE *pstCycleOne = NULL;
		TASKRECYCLEDEF *pstTaskRecycle = NULL;
		int iNewTaskID;

		pstCycleOne = taskcycle_run_find_bytask(pstPlayer, pstTaskDef->ID);
		if (pstCycleOne)
		{
			pstCycleOne->TaskRecycleIdxDropTime = pstEnv->pstAppCtx->stCurr.tv_sec;
			pstCycleOne->TaskID = 0;

			pstTaskRecycle = taskcycle_def_find(pstEnv, pstCycleOne->TaskRecycleID, NULL);
			if (pstTaskRecycle)
			{
				g_siTaskDrop = 1;
				if ( 0 > (iNewTaskID = taskrecycle_get_task(pstEnv, pstPlayer, pstTaskRecycle, pstCycleOne->TaskRecycleIdx, pstCycleOne)))
				{	
					if (pstTaskRecycle->OneDayNum > 0 || pstTaskRecycle->WeekNum >0)
					{
						taskcycle_add_oneday(pstEnv, pstPlayer, pstTaskRecycle->ID);
						iNotify = 1;
					}
					
					taskcycle_run_del(pstPlayer, pstTaskRecycle->ID);

					//taskcycle_fini_trace(pstEnv, pstPlayer, pstTaskRecycle->ID);
					pstCycleOne = NULL;
				}
				else
				{
					pstCycleOne->TaskID = iNewTaskID;
					pstCycleOne->Stat = TASKCYCLE_STAT_INIT;
				}
				g_siTaskDrop = 0;

				if (	pstCycleOne && !pstTaskRecycle->OtherDayFlag &&
					!IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstCycleOne->GetTime))
				{
					pstCycleOne->TaskID = 0;
					pstCycleOne->TaskRecycleIdx = 0;
				}

				if (pstCycleOne && pstTaskRecycle->DropFlag > 0)
				{
					pstCycleOne->TaskID = 0;
					pstCycleOne->TaskRecycleIdx = 0;

					if((pstTaskRecycle->OneDayNum > 0 &&
						IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstCycleOne->GetTime)) ||
						pstTaskRecycle->WeekNum >0)
					{
						taskcycle_add_oneday(pstEnv, pstPlayer, pstCycleOne->TaskRecycleID);
						iNotify = 1;
					}
				}
			}
			else
			{
				taskcycle_run_del(pstPlayer, pstCycleOne->TaskRecycleID);
			}

			
		}
	}

	task_drop_convoy_clear(pstEnv, pstPlayer, pstTaskDef);
	task_end_dart_car_clear(pstEnv, pstPlayer, pstTaskDef);
	ronglu_task_fini(pstEnv, pstPlayer, pstTaskDef);

	//跑商放弃任务时，需要清除银票、身上的贸易商品、已兑换的本金上限
	if (pstTaskDef->Type == TASK_TYPE_PAOSHANG)
	{
		player_paoshang_clean(pstEnv,pstPlayer);
		player_paoshang_login_check(pstEnv, pstPlayer);
	}
	
	for(i=0; i<MAX_TASK_CLEAR_ITEM; i++)
	{
		if(pstTaskDef->TaskClearItemID[i] > 0)
		{
			int iNum;
			ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
			
			iNum = z_get_pak_item_num(pstEnv, pstPak, pstTaskDef->TaskClearItemID[i], -1,0,PACKAGE_DEC_NONE);
			if(iNum)
			{
				package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstTaskDef->TaskClearItemID[i], iNum, -1, NULL,PACKAGE_DEC_NONE, 0);
			}
		}
	}

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_TASK210, pstTaskDef->Name);

	//任务日志相关流水号
	z_oplog_event_begin(pstEnv);
	z_role_task_oplog( pstEnv, pstPlayer, (unsigned short)iTaskID, OP_TASK_DROP,NULL,0,0,0,0,0);

	if (iNotify)
	{
		task_info_all(pstEnv, pstPlayer);
	}

	return 0;
}

int task_option(ZONESVRENV *pstEnv, Player *pstPlayer, int iNPCID, int iTaskID)
{
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;

	pstTaskRunOne = task_run_find(pstPlayer, iTaskID);
	if (NULL == pstTaskRunOne)
	{
		return task_dialog(pstEnv, pstPlayer, iNPCID, iTaskID, TASK_DIALOG_ACCEPT);
	}

	pstTaskDef = task_def_find(pstEnv, iTaskID, NULL);
	if(pstTaskDef)
	{
		task_check_fini(pstEnv, pstPlayer, pstTaskRunOne, pstTaskDef);
	}

	switch(pstTaskRunOne->TaskStat)
	{
		case TASK_STAT_RUN:
			task_dialog(pstEnv, pstPlayer, iNPCID, iTaskID,TASK_DIALOG_NOFINI_TIP);
			break;
		case TASK_STAT_FAIL:
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_TASK211);
			break;
		case TASK_STAT_FINI:
			task_dialog(pstEnv, pstPlayer, iNPCID, iTaskID, TASK_DIALOG_AWARD);
			break;
		default:
			return -1;
			break;
	}

	return 0;
}

int task_award_mail(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEGRID *pstGrid, int iGridNum)
{
	char szTitle[MAIL_MAX_TITLE_LEN];
	char szText[SHORT_DESC_LEN];

	STRNCPY(szTitle, LJY_MAIL50, sizeof(szTitle));
	STRNCPY(szText,  LJY_MAIL51, sizeof(szText));
	
	return player_sysmail_fast_send(pstEnv, pstPlayer->stRoleData.RoleName, szTitle, szText, pstGrid, iGridNum, 0,NULL, 0);
}

int task_award_resource(ZONESVRENV* pstEnv, Player *pstPlayer, int iCycleID, TASKDEF *pstTaskDef, ROLEGRID *pstRoleGrid, int iRoleGrid,
							int iMoney, int iMoneyType, int iExp, int iContri,int iLastCyc,int iCycClanMoney)
{
	PlayerChagAttrVal stChgVal;
	AttrChgVal *pstVal;
	TASKAWARD *pstTaskAwardDef = &pstTaskDef->TaskAward;
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
	TASKRECYCLEDEF *pstTaskCycleDef = NULL;
	TASKRECYCLEONE *pstTaskCycleOne;
	unsigned int uiOldMoney = pstPlayer->stRoleData.RoleDetail.Money;
	int iClanBuildingValue;
	int iClanMoney;
	int iClanAnding;
	int iTech;
	int i;
	
	if (iRoleGrid > 0)
	{
		if (0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRoleGrid, iRoleGrid,0))
		{
			if (0 > task_award_mail(pstEnv, pstPlayer, pstRoleGrid, iRoleGrid))
			{
				;
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_TASK212);
			}
		}
		else
		{
			package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRoleGrid, iRoleGrid,0, IMPITEM_OPTYPE_TASK);
		}
	}

	if (pstTaskAwardDef->Buff.TaskBuffID > 0)
	{
		z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstTaskAwardDef->Buff.TaskBuffID, 
							pstTaskAwardDef->Buff.TaskBuffLevel, pstTaskAwardDef->Buff.TaskBuffTime, 1, 0);
	}

	stChgVal.AttrNum = 0;

	if (iExp > 0)
	{
		tdr_longlong ullRealExp = iExp;

		player_world_level_exp(pstEnv, pstPlayer, &ullRealExp);
		
		pstVal = &stChgVal.AttrList[stChgVal.AttrNum++];
		pstVal->AttrID = ATTR_ID_EXP;
		pstVal->nFlag = 0;
		pstVal->Value.Exp = ullRealExp;
	}

	if (iMoneyType != 0 && iMoney > 0)
	{
		if (package_add_money_test(pstEnv, pstPlayer, iMoneyType, iMoney) == 0)
		{
			pstVal = &stChgVal.AttrList[stChgVal.AttrNum++];
			pstVal->AttrID = iMoneyType;
			pstVal->nFlag = 0;

			if (pstVal->AttrID == ATTR_ID_MONEY || pstVal->AttrID == ATTR_ID_BULL 
				|| (pstVal->AttrID >= ATTR_ID_BINDGODCOIN && pstVal->AttrID <= ATTR_ID_VIPPOINT) ||
				pstVal->AttrID == ATTR_ID_LIFT_BRAND||
				pstVal->AttrID == ATTR_ID_AOSHAN_BRAND ||
				pstVal->AttrID == ATTR_ID_CLAN_WAR_BRAND||
				pstVal->AttrID == ATTR_ID_BATTLE_GLORY || pstVal->AttrID == ATTR_ID_ZHENGYI)
			{
				pstVal->Value.Money = iMoney;

				if (pstVal->AttrID == ATTR_ID_BINDGODCOIN)
				{
					z_bindgold_create_oplog(pstEnv, pstPlayer, BINDGOLD_OPTYPE_TASK, pstTaskDef->ID,
									iMoney, "任务奖励");
				}
				else if (pstVal->AttrID == ATTR_ID_MONEY)
				{
					z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_TASK, pstTaskDef->ID, 0, iMoney, "任务奖励");
				}
				else if (pstVal->AttrID == ATTR_ID_BULL)
				{
					z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_TASK, pstTaskDef->ID,  iMoney,0, "任务奖励");
				}
			}
			else
			{
				//pstVal->Value.BlueGem = iMoney*(1-pstPlayer->bAasIncomeLimt/100.0);
			}
		}
	}

	if (stChgVal.AttrNum > 0)
	{
		z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);

		if (pstPlayer->stRoleData.RoleDetail.Money != uiOldMoney)
		{
			range_single_set(pstEnv, pstPlayer, RANGE_MAIN_SINGLE_SUB_MONEY, 
							pstPlayer->stRoleData.RoleDetail.Money+pstPlayer->stRoleData.Package.StashPackage.Money);
		}
	}

	// 环次任务给的个人贡献
	if (iContri > 0 && iCycleID > 0 && pstZoneClan)
	{
		g_iOptype = OP_OPTYPE_TASK;
		g_iOpsubtype = pstTaskDef->ID;
		clan_resource_inc(pstEnv, pstZoneClan, pstPlayer, CLAN_OWNED_CONTRIBUT,iContri);
		g_iOptype = 0;
		g_iOpsubtype = 0;
	}

	if(pstTaskAwardDef->Respact.ID) // 完成任务增加声望
	{
		player_respact_inc(pstEnv, pstPlayer, pstTaskAwardDef->Respact.ID,
		                   pstTaskAwardDef->Respact.Value, 0);
	}

	if(pstTaskAwardDef->Designation) // 完成任务奖励一个称号
	{
		player_designation_add(pstEnv, pstPlayer, pstTaskAwardDef->Designation);
	}

	// 最后一环
	pstTaskCycleDef = taskcycle_def_find(pstEnv, iCycleID, NULL);
	if (iCycleID > 0)
	{
		//基友奖励
		player_gay_award(pstEnv, pstPlayer, pstTaskCycleDef);
		
		pstTaskCycleOne = taskcycle_run_find(pstPlayer, iCycleID);
		if (pstTaskCycleDef && pstTaskCycleOne == NULL)
		{
			if(pstTaskCycleDef->FnshRespact.ID) // 完成任务增加声望
			{
				player_respact_inc(pstEnv, pstPlayer, pstTaskCycleDef->FnshRespact.ID,
				                   pstTaskCycleDef->FnshRespact.Value, 0);
			}

			if(pstTaskCycleDef->FnshDesignation) // 完成任务奖励一个称号
			{
				player_designation_add(pstEnv, pstPlayer, pstTaskCycleDef->FnshDesignation);
			}
		}
	}
	
	if(pstZoneClan)
	{
		int iCycNum = 1;
		TASKCYCLEONEDAYINFO *pstCycOneDayInfo;
		iClanBuildingValue = pstTaskAwardDef->ClanBuildingValue;
		iClanMoney = pstTaskAwardDef->ClanMoney;
		iClanAnding = pstTaskAwardDef->ClanAnDing;
		iTech = pstTaskAwardDef->ClanTechPoint;
		
		pstCycOneDayInfo =  taskcycle_find_oneday(pstPlayer, iCycleID);
		if (pstCycOneDayInfo)
		{
			// 如果是最后一环的话 ,在Task_commit中OneDayNum已经加一了
			if (iLastCyc)
				iCycNum = pstCycOneDayInfo->OneDayNum;
			else
				iCycNum = pstCycOneDayInfo->OneDayNum+1;
		}

		if (pstTaskCycleDef)
		{
			iClanMoney += iCycClanMoney;
		
			for (i=0; i<MAX_CYC_AWD_RATE; i++)
			{
				if (	iCycNum >= pstTaskCycleDef->AwdRate[i].StartCyc &&
					iCycNum <= pstTaskCycleDef->AwdRate[i].EndCyc)
				{
					iClanBuildingValue = iClanBuildingValue*(pstTaskCycleDef->AwdRate[i].TaskAwdRate/100.0);
					iClanMoney = iClanMoney*(pstTaskCycleDef->AwdRate[i].TaskAwdRate/100.0);
					iClanAnding = iClanAnding*(pstTaskCycleDef->AwdRate[i].TaskAwdRate/100.0);
					iTech = iTech*(pstTaskCycleDef->AwdRate[i].TaskAwdRate/100.0);
					break;
				}
			}
		}

		g_iOptype = OP_OPTYPE_TASK;
		g_iOpsubtype = pstTaskDef->ID;
		if(iClanMoney > 0)
		{
			clan_resource_inc(pstEnv, pstZoneClan, pstPlayer, CLAN_MONEY,
			                  iClanMoney);
		}

		// 环次任务只给环次任务表里面的贡献度
		if(pstTaskAwardDef->ClanOwnedContribute > 0 && iCycleID == 0)
		{
			clan_resource_inc(pstEnv, pstZoneClan, pstPlayer, CLAN_OWNED_CONTRIBUT,
			                  pstTaskAwardDef->ClanOwnedContribute);
		}

		// 公会安定度
		if (pstTaskAwardDef->ClanAnDing > 0)
		{
			clan_resource_inc(pstEnv, pstZoneClan, pstPlayer, CLAN_STABLE_VALUE,
			                  iClanAnding);
		}

		// 公会科技点
		if (pstTaskAwardDef->ClanTechPoint > 0)
		{
			clan_resource_inc(pstEnv, pstZoneClan, pstPlayer, CLAN_TECH_POINT,
			                  iTech);
		}

		//各种资源
		for (i = 0; i < MAX_TASK_CLAN_RESOURCE; i++)
		{
			int iID = pstTaskAwardDef->ClanResource[i].ResourceID;
			int iNum = pstTaskAwardDef->ClanResource[i].Num;
			int iRealNum = 0;

			if (iID <= 0 || iNum <= 0)
			{
				break;
			}
			if (clan_resource_add_entries(pstEnv,pstZoneClan, iID,iNum,&iRealNum) == 0)
			{

				OPCLANDATA stData;
				CLANPRODUCTDEF* pstProductDef = clan_product_def(pstEnv, iID);
				
				stData.ClanResourceGather.ResouceID = iID;
				stData.ClanResourceGather.ResouceNum = iRealNum;
				stData.ClanResourceGather.TaskID = pstTaskDef->ID;

				z_role_clan_oplog(pstEnv, pstPlayer->stRoleData.RoleName, clan_id_warp(&pstZoneClan->stClan),
				OP_CLAN_RESOURCE_TASK,&stData);
				if (pstProductDef)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_CHAT, ZONEERR_TASK248, pstProductDef->Name,iNum);
				}

			}
		}

		g_iOptype = 0;
		g_iOpsubtype = 0;
	}

	//学习生活技能
	if(pstTaskAwardDef->SecondarySkillType > 0)
	{
		//奖励技能等级
		if(pstTaskAwardDef->SecondarySkillLevel > 0)
		{
			player_learn_secondary_skill(pstEnv, pstPlayer, pstTaskAwardDef->SecondarySkillType, pstTaskAwardDef->SecondarySkillLevel);
		}

		//奖励熟练度
		if(pstTaskAwardDef->SecondarySkillVal > 0)
		{
			SECONDARYSKILLINFO *pstSkillInfo = get_player_secondary_skill_info(pstPlayer, pstTaskAwardDef->SecondarySkillType);
			if(pstSkillInfo)
			{
				z_add_secondary_skill_val(pstEnv, pstPlayer, pstSkillInfo, pstTaskAwardDef->SecondarySkillVal);
			}
		}
	}

	return 0;
}

int gm_task_commit(ZONESVRENV* pstEnv, Player *pstPlayer, TASKRUNONE *pstTaskRunOne, TASKDEF *pstTaskDef)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	int iCycleID = pstTaskRunOne->CycleID;

	if (pstTaskDef->TaskGetLimit.NumOneDay > 0)
	{
		task_add_oneday(pstEnv, pstPlayer, pstTaskDef->ID);
	}

	if (0 == pstTaskDef->Redo || pstTaskDef->Flag)
	{
		task_add_fini(pstPlayer, pstTaskDef->ID);
	}

	if (pstTaskDef->LimitGroupID > 0)
	{
		group_add_oneday(pstEnv, pstPlayer, pstTaskDef->LimitGroupID);
	}

	if (iCycleID > 0)
	{
		int iTaskID;
		TASKRECYCLEONE *pstTaskCycleOne;
		TASKRECYCLEDEF *pstTaskRecycle;
		
		pstTaskCycleOne = taskcycle_run_find(pstPlayer, iCycleID);	
		if (NULL == pstTaskCycleOne)
		{
			return -1;
		}
		else
		{
			pstTaskRecycle = taskcycle_def_find(pstEnv, iCycleID, NULL);
			if (NULL == pstTaskRecycle)
			{
				return -1;
			}

			pstTaskCycleOne->TaskRecycleIdx++;
			g_siTaskCommit = 1;
			if (0 > (iTaskID = taskrecycle_get_task(pstEnv, pstPlayer, pstTaskRecycle, pstTaskCycleOne->TaskRecycleIdx, pstTaskCycleOne)))
			{
				if ((pstTaskRecycle->OneDayNum > 0||pstTaskRecycle->WeekNum >0) && iTaskID != -2)
				{
					taskcycle_add_oneday(pstEnv, pstPlayer, iCycleID);
				}
				
				taskcycle_run_del(pstPlayer, iCycleID);

				taskcycle_fini_trace(pstEnv, pstPlayer, pstTaskRecycle->ID);
			}
			else
			{
				pstTaskCycleOne->TaskID = iTaskID;
				pstTaskCycleOne->Stat = TASKCYCLE_STAT_INIT;
			}
			g_siTaskCommit = 0;
		}
	}

	g_siCycleID = iCycleID;
	pstTaskRunOne->TaskStat = TASK_STAT_COMMIT;
	task_chg(pstEnv, pstPlayer, pstTaskRunOne);
	g_siCycleID = 0;

	task_run_del(pstEnv, pstPlayer, pstTaskDef->ID);

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_TASK;
	pstActionRes->Data.Task.TaskID = pstTaskDef->ID;
	pstActionRes->Data.Task.TaskStat = TASK_STAT_COMMIT;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);

	return 0;
}

int task_commit(ZONESVRENV* pstEnv, Player *pstPlayer, TASKRUNONE *pstTaskRunOne, TASKDEF *pstTaskDef)
{
	int i;
	int iTaskID;
	char szTips[256];
	TASKRECYCLEONE *pstTaskCycleOne;
	TASKRECYCLEDEF *pstTaskRecycle;
	int iRet1 = 0;
	int iCycleID = 0;
	int iOriTaskStat = 0;
	
	CSPKG stPkg;
	int iRet;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	
	ZoneTeam *pstTeam = NULL;

	if (pstTaskRunOne)
	{
	 	iCycleID = pstTaskRunOne->CycleID;
	}

       if (pstTaskDef->TaskItem > 0)
	{
		int iItemNum = 1;

		if (pstTaskDef->TaskItemNum > 1)
		{
			iItemNum = pstTaskDef->TaskItemNum;
		}
		
		iRet =  package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstTaskDef->TaskItem, iItemNum, -1, NULL,PACKAGE_DEC_NONE, 0);

		if (0 == pstTaskDef->TaskItemCommitNoFlag && 0 > iRet)
		{
			ITEMDEF *pstItemDef;

			pstItemDef = z_find_itemdef(pstEnv, pstTaskDef->TaskItem);
			if (pstItemDef)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_TASK213, pstItemDef->Name);
			}
			
			return -1;
		}
	}
	
	if (pstTaskDef->TaskBuff.TaskBuffID > 0)
	{
		z_player_del_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstTaskDef->TaskBuff.TaskBuffID, 1, 0);

	}

	if(pstTaskRunOne)
	{
		for (i=0; i<pstTaskRunOne->TaskItemTrace.TaskItemTraceNum; i++)
		{
			if (pstTaskRunOne->TaskItemTrace.TaskGetItems[i].ItemID== 0)
			{
				break;
			}

			if (0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
					pstTaskRunOne->TaskItemTrace.TaskGetItems[i].ItemID, 
					pstTaskRunOne->TaskItemTrace.TaskGetItems[i].MaxNum, 
					-1, NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_TASK))
			{
				return -1;
			}
		}	
	}

	szTips[0] = 0;
	if (iCycleID > 0)
	{
		pstTaskCycleOne = taskcycle_run_find(pstPlayer, iCycleID);	
		if (NULL == pstTaskCycleOne)
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "taskcycle_run_find fail when task commit");
			return 0;
		}
		else
		{
			pstTaskRecycle = taskcycle_def_find(pstEnv, iCycleID, NULL);
			if (NULL == pstTaskRecycle)
			{
				return 0;
			}

			/*
			if (pstTaskRecycle && pstTaskCycleOne)
			{
				if(pstTaskRecycle->CycType != CYC_TYPE_PLOY)
				{
					snprintf(szTips, sizeof(szTips), "%s环组第%d 环", pstTaskRecycle->Name, pstTaskCycleOne->TaskRecycleIdx+1);
				}
			}
			*/

			// 清理隔天的环组
			if (	!pstTaskRecycle->OtherDayFlag &&
				!IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstTaskCycleOne->GetTime))
			{
				pstTaskCycleOne->TaskID = 0;
				pstTaskCycleOne->TaskRecycleIdx = 0;
			}
			else
			{
				pstTaskCycleOne->TaskRecycleIdx++;
				g_siTaskCommit = 1;
				if (0 > (iTaskID = taskrecycle_get_task(pstEnv, pstPlayer, pstTaskRecycle, pstTaskCycleOne->TaskRecycleIdx, pstTaskCycleOne)))
				{
					if ((pstTaskRecycle->OneDayNum > 0||pstTaskRecycle->WeekNum >0) && iTaskID != -2)
					{
						taskcycle_add_oneday(pstEnv, pstPlayer, iCycleID);
					}
					
					taskcycle_run_del(pstPlayer, iCycleID);

					taskcycle_fini_trace(pstEnv, pstPlayer, pstTaskRecycle->ID);
					iRet1 = 1;// 整个环完成标记 award中要用

					//临时加个日志看下
					if(pstTaskCycleOne->TaskRecycleIdx < pstTaskRecycle->CycleNum && 
						pstTaskCycleOne->TaskRecycleIdx < 10)
					{
						tlog_info(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, 
							"taskcycle=%d, taskid=%d, idx=%d, ret=%d", 
							iCycleID, pstTaskCycleOne->TaskID, pstTaskCycleOne->TaskRecycleIdx, iTaskID);
					}
					
				}
				else
				{
					pstTaskCycleOne->TaskID = iTaskID;
					pstTaskCycleOne->Stat = TASKCYCLE_STAT_INIT;
				}

				g_siTaskCommit = 0;
			}
		}
	}

	for(i=0; i<MAX_TASK_CLEAR_ITEM; i++)
	{
		if(pstTaskDef->TaskClearItemID[i] > 0)
		{
			int iNum;
			ROLEPACKAGE *pstPak = &pstPlayer->stRoleData.Package;
			
			iNum = z_get_pak_item_num(pstEnv, pstPak, pstTaskDef->TaskClearItemID[i], -1,0,PACKAGE_DEC_NONE);
			if(iNum)
			{
				package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstTaskDef->TaskClearItemID[i], iNum, -1, NULL,PACKAGE_DEC_NONE, 0);
			}
		}
	}

	if (pstTaskRunOne)
	{
		iOriTaskStat = pstTaskRunOne->TaskStat;
		g_siCycleID = iCycleID;
		pstTaskRunOne->TaskStat = TASK_STAT_COMMIT;
		task_chg(pstEnv, pstPlayer, pstTaskRunOne);
		g_siCycleID = 0;

		task_run_del(pstEnv, pstPlayer, pstTaskDef->ID);	
	}

	if (iOriTaskStat != TASK_STAT_DROP)
	{
		task_fini_trace(pstEnv, pstPlayer, pstTaskDef->ID);
	}

	if (pstTaskDef->TaskGetLimit.NumOneDay > 0)
	{
		int iRet;
		
		iRet = task_add_oneday(pstEnv, pstPlayer, pstTaskDef->ID);

		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_TASK214, pstTaskDef->Name, iRet);
	       z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_TASK215,pstTaskDef->Name);
	}
	else
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_TASK215,pstTaskDef->Name);
		z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_TASK215,pstTaskDef->Name);
	}
	
	if (pstTaskDef->LimitGroupID > 0)
	{
		group_add_oneday(pstEnv, pstPlayer, pstTaskDef->LimitGroupID);
	}

	if (0 == pstTaskDef->Redo || pstTaskDef->Flag)
	{
		task_add_fini(pstPlayer, pstTaskDef->ID);
	}

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_TASK;
	pstActionRes->Data.Task.TaskID = pstTaskDef->ID;
	pstActionRes->Data.Task.TaskStat = TASK_STAT_COMMIT;

	task_end_dart_car_clear(pstEnv, pstPlayer, pstTaskDef);

	
	//刺探任务
	if (pstTaskDef->ID == CLAN_SPY_TASK_ID)
	{
		pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_SPY;
		player_action_view_flag_chg(pstEnv,pstPlayer);
	}

	if (pstTaskDef->ID == CLAN_ANTI_SPY_TASK_ID)
	{
		pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_ANTI_SPY;
		player_action_view_flag_chg(pstEnv,pstPlayer);
	}
	
	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(NULL != pstTeam)
	{
		if(0 != pstTaskDef->BuddyVal)
		{
			//提交任务时，相互为好友并且组队加亲密度
			team_relation_buddyval_change(pstEnv, pstTeam, pstTaskDef->BuddyVal);
		}
	}

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);

	
	return iRet1;
}

int trust_check_fini(ZONESVRENV *pstEnv, Player *pstPlayer, TASKDEF *pstTaskDef, unsigned char  ucFlag)
{
	int i = 0;
	TASKGETLIMIT *pstTaskLimit = &pstTaskDef->TaskGetLimit;

	if ( (CAREER_NON != pstTaskLimit->Career && (pstPlayer->stRoleData.Career&0x0F) != (pstTaskLimit->Career&0x0F)) || 
		 (CAREER_NON != (pstTaskLimit->Career&0xF0) && (pstPlayer->stRoleData.Career&0xF0) != (pstTaskLimit->Career&0xF0)) )
	{
		//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "委托要求职业不符");
		return 0;
	}

	if ((pstTaskLimit->GetLevelMin != 0 && pstPlayer->stRoleData.Level < pstTaskLimit->GetLevelMin) || 
		(pstTaskLimit->GetLevelMax != 0 && pstPlayer->stRoleData.Level > pstTaskLimit->GetLevelMax) )
	{
		//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "委托等级不符");
		return 0;
	}

	if (pstTaskLimit->Gender != GENDER_NO && pstTaskLimit->Gender != pstPlayer->stRoleData.Gender)
	{
		return 0;
	}	
	
	if (pstTaskDef->Redo && pstTaskLimit->NumOneDay > 0)
	{
		TASKONEDAYINFO *pstTaskOneDay;

		pstTaskOneDay = task_find_oneday(pstPlayer, pstTaskDef->ID);
		if (pstTaskOneDay && pstTaskOneDay->OneDayNum >= pstTaskDef->TaskGetLimit.NumOneDay)
		{
			//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "此委托任务每天完成的次数已达到上限 ");
			return 0;
		}
	}
	
	if (0 == pstTaskDef->Redo)
	{
		if (0 == task_find_fini(pstPlayer, pstTaskDef->ID))
		{
			return 0;
		}
	}
	
	if (ucFlag)
	{
		for (i=0; i<MAX_TASK_ITEM; i++)
		{
			if (pstTaskDef->GetItem[i].ItemID == 0 || pstTaskDef->GetItem[i].Num == 0)
			{
				break;
			}

			if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstTaskDef->GetItem[i].ItemID, pstTaskDef->GetItem[i].Num, -1, NULL,PACKAGE_DEC_NONE))
			{
				return 0;
			}		
		}
	}
		
	return 1;
}

int npc_task_accept_check(ZONESVRENV* pstEnv, Player *pstPlayer, Npc *pstNpc)
{
	int i;
	NPCINFODEF *pstNpcInfoDef;
	TASKDEF *pstTaskDef;
	
	pstNpcInfoDef = npcinfo_def_find(pstEnv,pstNpc->stMon.iDefIdx , &pstNpc->iNpcInfoIdx);
	if (NULL == pstNpcInfoDef)
	{
		return 0;
	}

	for (i=0; i<(int)(sizeof(pstNpcInfoDef->AcceptTaskIDs)/sizeof(pstNpcInfoDef->AcceptTaskIDs[0])); i++)
	{
		if (0 == pstNpcInfoDef->AcceptTaskIDs[i])
		{
			break;
		}

		pstTaskDef = task_def_find(pstEnv, pstNpcInfoDef->AcceptTaskIDs[i], NULL);
		if (NULL == pstTaskDef)
		{
			continue;
		}
		
		if (0 == task_accept_check_matchclient(pstEnv, pstPlayer, pstTaskDef, 0))
		{
			return 1;
		}
	}

	return 0;
}

int taskrecycle_check_npc(ZONESVRENV* pstEnv, Player *pstPlayer, int iNPCID, int iTaskRecycleID, int iFlag)
{
	int i;
	Npc *pstNpc;
	NPCINFODEF *pstNpcInfoDef;
	TASKRECYCLEONE *pstTaskCycleOne; 
	int iIdx;
	
	if (iNPCID != pstPlayer->stOnline.iDialogID)
	{
		return -1;
	}

	pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, iNPCID);
	if (NULL == pstNpc)
	{
		return -1;
	}

	if (0 > z_check_op_dist(pstEnv, pstPlayer, pstNpc->stMon.stMap.iID, &pstNpc->stMon.stCurrPos, OP_DIST+pstNpc->stMon.unRadius))
	{
		return -1;
	}

	pstNpcInfoDef = npcinfo_def_find(pstEnv,pstNpc->stMon.iDefIdx , &pstNpc->iNpcInfoIdx);
	if (NULL == pstNpcInfoDef)
	{
		return -1;
	}

	pstTaskCycleOne = taskcycle_run_find(pstPlayer, iTaskRecycleID);
	if (0 == iFlag) //接任务
	{
		if (pstTaskCycleOne)
		{
			iIdx = pstTaskCycleOne->TaskRecycleIdx +1;
		}
		else
		{
			iIdx = 1;
		}
		
		for (i=0; i<(int)(sizeof(pstNpcInfoDef->AptCycleIDs)/sizeof(pstNpcInfoDef->AptCycleIDs[0])); i++)
		{
			if (pstNpcInfoDef->AptCycleIDs[i].TaskCycleID == 0)
			{
				break;
			}

			if (	pstNpcInfoDef->AptCycleIDs[i].TaskCycleID == iTaskRecycleID &&
				iIdx >= pstNpcInfoDef->AptCycleIDs[i].From &&
				iIdx <= pstNpcInfoDef->AptCycleIDs[i].To)
			{
				return 0;
			}
		}
	}
	else //交任务
	{
		if (pstTaskCycleOne)
		{
			iIdx = pstTaskCycleOne->TaskRecycleIdx+1;
		}
		else
		{
			return -1;
		}

		for (i=0; i<(int)(sizeof(pstNpcInfoDef->CmtCycleIDs)/sizeof(pstNpcInfoDef->CmtCycleIDs[0])); i++)
		{
			if (pstNpcInfoDef->CmtCycleIDs[i].TaskCycleID == 0)
			{
				break;
			}

			if (	pstNpcInfoDef->CmtCycleIDs[i].TaskCycleID == iTaskRecycleID &&
				iIdx >= pstNpcInfoDef->CmtCycleIDs[i].From &&
				iIdx <= pstNpcInfoDef->CmtCycleIDs[i].To)
			{
				return 0;
			}
		}
	}

	return -1;
}

int task_check_npc(ZONESVRENV* pstEnv, Player *pstPlayer, int iNPCID, int iTaskID, int iFlag)
{
	int i,j;
	Npc *pstNpc;
	NPCINFODEF *pstNpcInfoDef;
	TASKDEF *pstTaskDef;

	TASKRECYCLEONE *pstTaskCycleOne; 
	int iIdx;
	
	if (iNPCID != pstPlayer->stOnline.iDialogID)
	{
		return -1;
	}

	pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, iNPCID);
	if (NULL == pstNpc)
	{
		return -1;
	}

	if (0 > z_check_op_dist(pstEnv, pstPlayer, pstNpc->stMon.stMap.iID, &pstNpc->stMon.stCurrPos, OP_DIST+pstNpc->stMon.unRadius))
	{
		return -1;
	}

	pstNpcInfoDef = npcinfo_def_find(pstEnv,pstNpc->stMon.iDefIdx , &pstNpc->iNpcInfoIdx);
	if (NULL == pstNpcInfoDef)
	{
		return -1;
	}

	pstTaskDef = task_def_find(pstEnv, iTaskID, NULL);
	if (NULL == pstTaskDef)
	{
		return -1;
	}

	pstTaskCycleOne = taskcycle_run_find_bytask(pstPlayer, pstTaskDef->ID);
	
	if (0 == iFlag) //接任务
	{
		if (pstTaskDef->TaskRecycleIDs[0] > 0)
		{
			if (pstTaskCycleOne)
			{
				iIdx = pstTaskCycleOne->TaskRecycleIdx +1;
			}
			else
			{
				iIdx = 1;
			}
			
			for (i=0; i<(int)(sizeof(pstNpcInfoDef->AptCycleIDs)/sizeof(pstNpcInfoDef->AptCycleIDs[0])); i++)
			{
				if (pstNpcInfoDef->AptCycleIDs[i].TaskCycleID == 0)
				{
					break;
				}

				for (j=0; j<MAX_TASK_TASKCYCLE; j++)
				{
					if (0 == pstTaskDef->TaskRecycleIDs[j])
					{
						break;
					}

					if (	pstNpcInfoDef->AptCycleIDs[i].TaskCycleID == pstTaskDef->TaskRecycleIDs[j] &&
						iIdx >= pstNpcInfoDef->AptCycleIDs[i].From &&
						iIdx <= pstNpcInfoDef->AptCycleIDs[i].To)
					{
						return 0;
					}
				}
			}
		}
		else
		{
			for (i=0; i<(int)(sizeof(pstNpcInfoDef->AcceptTaskIDs)/sizeof(pstNpcInfoDef->AcceptTaskIDs[0])); i++)
			{
				if (0 == pstNpcInfoDef->AcceptTaskIDs[i])
				{
					break;
				}
				
				if (pstNpcInfoDef->AcceptTaskIDs[i] == iTaskID)
				{
					return 0;
				}
			}
		}
	}
	else //交任务
	{
		if (pstTaskDef->TaskRecycleIDs[0] > 0)
		{
			if (pstTaskCycleOne)
			{
				iIdx = pstTaskCycleOne->TaskRecycleIdx+1;
			}
			else
			{
				return -1;
			}

			for (i=0; i<(int)(sizeof(pstNpcInfoDef->CmtCycleIDs)/sizeof(pstNpcInfoDef->CmtCycleIDs[0])); i++)
			{
				if (pstNpcInfoDef->CmtCycleIDs[i].TaskCycleID == 0)
				{
					break;
				}

				for (j=0; j<MAX_TASK_TASKCYCLE; j++)
				{
					if (0 == pstTaskDef->TaskRecycleIDs[j])
					{
						break;
					}

					if (	pstNpcInfoDef->CmtCycleIDs[i].TaskCycleID == pstTaskDef->TaskRecycleIDs[j] &&
						iIdx >= pstNpcInfoDef->CmtCycleIDs[i].From &&
						iIdx <= pstNpcInfoDef->CmtCycleIDs[i].To)
					{
						return 0;
					}
				}
			}
			
		}
		else
		{
			for (i=0; i<(int)(sizeof(pstNpcInfoDef->CommitTaskIDs)/sizeof(pstNpcInfoDef->CommitTaskIDs[0])); i++)
			{
				if (0 == pstNpcInfoDef->CommitTaskIDs[i])
				{
					break;
				}
				
				if (pstNpcInfoDef->CommitTaskIDs[i] == iTaskID)
				{
					return 0;
				}
			}

			if(pstNpc->stMon.iDefIdx == pstTaskDef->CommitNpcID)
			{
				return 0;
			}

		}
	}

	return -1;
}

int task_dialog_again(ZONESVRENV* pstEnv, Player *pstPlayer, Npc *pstNpc)
{
	int i;
	NPCINFODEF *pstNpcInfoDef;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	int iFlag = 0;

	pstNpcInfoDef = npcinfo_def_find(pstEnv,pstNpc->stMon.iDefIdx , &pstNpc->iNpcInfoIdx);
	if (NULL == pstNpcInfoDef)
	{
		return -1;
	}

	for (i=0; i<MAX_NPC_TASK; i++)
	{
		if (0 == pstNpcInfoDef->AcceptTaskIDs[i])
		{
			break;
		}

		pstTaskDef = task_def_find(pstEnv, pstNpcInfoDef->AcceptTaskIDs[i], NULL);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		if (0 == task_accept_check(pstEnv, pstPlayer, pstTaskDef, 1, 0))
		{
			iFlag = 1;
			break;
		}
	}

	for (i=0; i<MAX_NPC_TASK; i++)
	{
		if (0 == pstNpcInfoDef->CommitTaskIDs[i])
		{
			break;
		}

		pstTaskDef = task_def_find(pstEnv, pstNpcInfoDef->CommitTaskIDs[i], NULL);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		pstTaskRunOne = task_run_find(pstPlayer, pstNpcInfoDef->CommitTaskIDs[i]);
		if (NULL == pstTaskRunOne)
		{
			continue;
		}

		if (1 == task_check_fini(pstEnv, pstPlayer, pstTaskRunOne, pstTaskDef))
		{
			iFlag = 1;
			break;
		}
	}

	if (iFlag)
	{
		CSPKG stPkg;
		CSDLGC *pstCSDlgC = &stPkg.Body.DlgC;

		memset(pstCSDlgC, 0, sizeof(*pstCSDlgC));

		pstCSDlgC->NpcID = pstNpc->stMon.iID;
		pstCSDlgC->Type = DLG_TYPE_INPUT;
		pstCSDlgC->RolePos = pstPlayer->stRoleData.Pos;
		pstCSDlgC->SvrTime.BenchTimeSec = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstCSDlgC->SvrTime.BenchTimeUSec = pstEnv->pstAppCtx->stCurr.tv_usec;

		pstPlayer->stOnline.cDialogStat = DIALOG_STAT_BEGIN;
		
		player_dialog(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, &stPkg);
	}
	
	return 0;
}


int task_award_spar(ROLEITEM *pstItem)
{
	if (pstItem->DefID != 5213540)
		return 0;

	pstItem->Flag = 1004;							// 关键字ID
	pstItem->InstVal1= 1;								// 等级
	pstItem->InstVal2 = 90;						
	pstItem->InstVal3 = 0;							// 经验
	return 0;
}

int task_award_cacl_dratcar_yajin(ZONESVRENV* pstEnv, Player *pstPlayer,TASKDEF *pstTaskDef)
{
//	int iMoney = 0;
	Monster *pstMon;

	if (pstTaskDef->Type != TASK_TYPE_DART_CAR)
		return 0;

	pstMon = z_id_monster(pstEnv->pstAppCtx,pstEnv, pstPlayer->stRoleData.MiscInfo.DartCarInfo.MonMemID);
	if (!pstMon)
		return 0;

	if (pstMon->cDartCarNotAtk == 2)
	{
		return pstTaskDef->ConsumeMoney.Val;
	}

	return 0;
}


int task_award(ZONESVRENV* pstEnv, Player *pstPlayer, CSTASKAWARD *pstTaskAward)
{
	int i;
	TASKDEF *pstTaskDef;
	TASKRECYCLEDEF *pstTaskCycleDef;
	TASKRUNONE *pstTaskRunOne = NULL;
	TASKAWARD *pstTaskAwardDef;
	LEVELUP *pstLevelUP;
	TASKRECYCLEAWARD *pstTaskRecycleAwardDef;
	TASKRECYCLEONE *pstTaskCycleOne;
	ROLEGRID astRoleGrid[MAX_TASK_AWARD_ITEM*2];
	int iRoleGrid = 0;
	int iMoney = 0;
	int iMoneyType = 0;
	tdr_longlong iExp = 0;
	int iLastCycItemWard = 0;// 是否最后一环
	 TASKCYCLEONEDAYINFO *pstCycOneDayInfo;
	 int iCycNum = 1;
	 int iContri = 0;
	 int iCycleID = 0;
	 int iRet;
	 int iClanMoney = 0;
	 int iSuipian = 0;

	if(pstPlayer->bAasIncomeLimt > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
						ZONEERR_TASK216);
		return -1;
	}	
	
	pstTaskDef = task_def_find(pstEnv, pstTaskAward->TaskID, NULL);
	if (NULL == pstTaskDef)
	{
		return -1;
	}

	if(pstTaskDef->ID == 1085 ||
		pstTaskDef->ID == 1090  ||
		pstTaskDef->ID == 1093)
	{
		if(is_player_ride_on_machine(pstPlayer))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK249);
			return -1;
		}
	}

	if (0 == pstTaskDef->AutoFini && 
		TASK_TYPE_DAILY_GOAL != pstTaskDef->Type &&
		0 > task_check_npc(pstEnv, pstPlayer, pstTaskAward->NpcID, pstTaskAward->TaskID, 1))
	{
		return -1;
	}
	
	pstTaskRunOne = task_run_find(pstPlayer, pstTaskAward->TaskID);
	if (NULL == pstTaskRunOne)
	{
		return -1;
	}
	
	if (0 == task_check_fini(pstEnv, pstPlayer, pstTaskRunOne, pstTaskDef))
	{
		return -1;
	}

	pstLevelUP = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	if (NULL == pstLevelUP)
	{
		return -1;
	}

	iCycleID = pstTaskRunOne->CycleID;
	if (iCycleID > 0)
	{
		pstTaskCycleDef = taskcycle_def_find(pstEnv, iCycleID, NULL);
		if (NULL == pstTaskCycleDef)
		{
			return -1;
		}

		pstTaskCycleOne = taskcycle_run_find(pstPlayer, iCycleID);
		if (NULL == pstTaskCycleOne ||0 > pstTaskCycleOne->TaskRecycleIdx || pstTaskCycleOne->TaskRecycleIdx >=  pstTaskCycleDef->CycleNum)
		{
			return -1;
		}

		pstTaskRecycleAwardDef = &pstTaskCycleDef->TaskRecycles[pstTaskCycleOne->TaskRecycleIdx].TaskRecycleAward;

		iMoneyType = pstTaskRecycleAwardDef->Money.Type;

		iContri = pstTaskRecycleAwardDef->Contri;
		if(pstTaskCycleDef->NoDynAward)
		{
			iMoney = pstTaskRecycleAwardDef->Money.MoneyRate;
			iExp = pstTaskRecycleAwardDef->Exp.ExpRate;
		}
		else
		{
			iMoney = pstTaskRecycleAwardDef->Money.MoneyRate*(pstLevelUP->ContributeMoney/10000.0);
			iExp = pstTaskRecycleAwardDef->Exp.ExpRate*(pstLevelUP->ContributeExp/10000.0);
		}

		iClanMoney += pstTaskRecycleAwardDef->ClanMoney;
		
		pstCycOneDayInfo =  taskcycle_find_oneday(pstPlayer, iCycleID);
		if (pstCycOneDayInfo)
		{
			iCycNum = pstCycOneDayInfo->OneDayNum+1;
		}
	
		for (i=0; i<MAX_CYC_AWD_RATE; i++)
		{
			if (	iCycNum >= pstTaskCycleDef->AwdRate[i].StartCyc &&
				iCycNum <= pstTaskCycleDef->AwdRate[i].EndCyc)
			{
				iExp = iExp*(pstTaskCycleDef->AwdRate[i].CycAwdRate/100.0);
				iMoney = iMoney*(pstTaskCycleDef->AwdRate[i].CycAwdRate/100.0);
				iContri = iContri*(pstTaskCycleDef->AwdRate[i].CycAwdRate/100.0);
				break;
			}	
		}
		
		// 最后一环
		if (pstTaskCycleDef->FnshAwdCycIdx == pstTaskCycleOne->TaskRecycleIdx + 1)
		{
			iExp += pstTaskCycleDef->FnshAwd.Exp.ExpRate;

			// 策划说金钱类型肯定会一致 ,所以不判断金钱类型,以最后一环为准
			if (iMoney > 0)
			{
				iMoney += pstTaskCycleDef->FnshAwd.Money.MoneyRate;
			}
			else
			{
				iMoneyType = pstTaskCycleDef->FnshAwd.Money.Type;
				iMoney = pstTaskCycleDef->FnshAwd.Money.MoneyRate;
			}

			iClanMoney += pstTaskCycleDef->FnshAwd.ClanMoney;
				
			if (!((pstTaskCycleDef->CtrlFlag & CYC_CTRL_FLAG_LAST) &&
				 taskcycle_find_oneday(pstPlayer, pstTaskCycleDef->ID)))
			{
				iLastCycItemWard = 1;
			}
		}

		if (iExp > 0)
		{
			int iRet;
			iRet = z_player_addexp_test(pstEnv,pstPlayer, &iExp);
			
			if (iRet < 0 && (0 == pstTaskDef->AutoFini || 2 == pstTaskDef->AutoFini) && iRet != -3)
			{
				return -1;
			}
		}
		
		if (pstTaskRecycleAwardDef->ItemAwardType == TASK_ITEM_AWARD_ONE)
		{
			if (pstTaskAward->SelectItemID == 0)
			{
				return -1;
			}

			for (i=0; i<(int)(sizeof(pstTaskRecycleAwardDef->Items)/sizeof(pstTaskRecycleAwardDef->Items[0])); i++)
			{
				if (0 == pstTaskRecycleAwardDef->Items[i].ID)
				{
					break;
				}

				if (pstTaskAward->SelectItemID == pstTaskRecycleAwardDef->Items[i].ID)
				{
					//if (iRoleGrid >= (int)(sizeof(astRoleGrid)/sizeof(astRoleGrid[0])) )
					if (iRoleGrid >= MAX_TASK_AWARD_ITEM)
					{
						break;
					}
					
					if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &astRoleGrid[iRoleGrid], pstTaskAward->SelectItemID))
					{
						return -1;
					}

					if(pstTaskCycleDef->CtrlFlag & CYC_CTRL_FLAG_ALL_BIND)
					{
						astRoleGrid[iRoleGrid].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
					}

					if (GRID_ITEM_ITEM == astRoleGrid[iRoleGrid].Type)
					{
						astRoleGrid[iRoleGrid].GridData.RoleItem.Num = pstTaskRecycleAwardDef->Items[i].Num;
						task_award_spar(&astRoleGrid[iRoleGrid].GridData.RoleItem);
					}
					
					iRoleGrid++;
					break;
				}
			}
		}
		else if (pstTaskRecycleAwardDef->ItemAwardType == TASK_ITEM_AWARD_ALL)
		{
			for (i=0; i<(int)(sizeof(pstTaskRecycleAwardDef->Items)/sizeof(pstTaskRecycleAwardDef->Items[0])); i++)
			{
				if (iRoleGrid >= MAX_TASK_AWARD_ITEM )
				{
					break;
				}
				
				if (0 == pstTaskRecycleAwardDef->Items[i].ID)
				{
					break;
				}

				if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &astRoleGrid[iRoleGrid], pstTaskRecycleAwardDef->Items[i].ID))
				{
					return -1;
				}

				if(pstTaskCycleDef->CtrlFlag & CYC_CTRL_FLAG_ALL_BIND)
				{
					astRoleGrid[iRoleGrid].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
				}


				if (GRID_ITEM_ITEM == astRoleGrid[iRoleGrid].Type)
				{
					astRoleGrid[iRoleGrid].GridData.RoleItem.Num = pstTaskRecycleAwardDef->Items[i].Num;
					task_award_spar(&astRoleGrid[iRoleGrid].GridData.RoleItem);
				}
				
				iRoleGrid++;
			}
		}
		else if (pstTaskRecycleAwardDef->ItemAwardType == TASK_ITEM_AWARD_RAND)
		{
			int iTmp = 0;
			int iRand = RAND1(100);
			
			for (i=0; i<(int)(sizeof(pstTaskRecycleAwardDef->Items)/sizeof(pstTaskRecycleAwardDef->Items[0])); i++)
			{
				if (iRoleGrid >= MAX_TASK_AWARD_ITEM)
				{
					break;
				}
				
				if (0 == pstTaskRecycleAwardDef->Items[i].ID)
				{
					break;
				}

				iTmp += pstTaskRecycleAwardDef->Items[i].ItemRate;
				if (iRand < iTmp)
				{
					if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &astRoleGrid[iRoleGrid], pstTaskRecycleAwardDef->Items[i].ID))
					{
						return -1;
					}

					if(pstTaskCycleDef->CtrlFlag & CYC_CTRL_FLAG_ALL_BIND)
					{
						astRoleGrid[iRoleGrid].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
					}
					
					if (GRID_ITEM_ITEM == astRoleGrid[iRoleGrid].Type)
					{
						astRoleGrid[iRoleGrid].GridData.RoleItem.Num = pstTaskRecycleAwardDef->Items[i].Num;
						task_award_spar(&astRoleGrid[iRoleGrid].GridData.RoleItem);
					}
					
					iRoleGrid++;
					break;
				}
			}
		}

		// 最终奖励类型为TASK_ITEM_AWARD_ALL
		if (iLastCycItemWard)
		{
			for (i=0; i<MAX_RECYCLE_TASK_AWARD_ITEM; i++)
			{
				if (iRoleGrid >= MAX_TASK_AWARD_ITEM*2)
				{
					break;
				}
				
				if (0 == pstTaskCycleDef->FnshAwd.Items[i].ID)
				{
					break;
				}

				if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &astRoleGrid[iRoleGrid], pstTaskCycleDef->FnshAwd.Items[i].ID))
				{
					return -1;
				}

				if(pstTaskCycleDef->CtrlFlag & CYC_CTRL_FLAG_FNSH_BIND)
				{
					astRoleGrid[iRoleGrid].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
				}

				if (GRID_ITEM_ITEM == astRoleGrid[iRoleGrid].Type)
				{
					astRoleGrid[iRoleGrid].GridData.RoleItem.Num = pstTaskCycleDef->FnshAwd.Items[i].Num;
					task_award_spar(&astRoleGrid[iRoleGrid].GridData.RoleItem);
				}
				iRoleGrid++;
			}
		}
	}
	else
	{
		pstTaskAwardDef = &pstTaskDef->TaskAward;
		iMoneyType = pstTaskAwardDef->Money.Type;

		if(	pstTaskDef->Type == TASK_TYPE_DYN ||
			TASK_TYPE_DAILY_GOAL == pstTaskDef->Type ||
			pstTaskDef->Type == TASK_TYPE_DART_CAR ||
			pstTaskDef->Type == TASK_TYPE_BATTLE)
		{
			iMoney = (double )pstTaskAwardDef->Money.Val*pstLevelUP->ContributeMoney/10000.0;
			iExp = (double )pstTaskAwardDef->Exp*pstLevelUP->ContributeExp/10000.0;
		}
		else if(pstTaskDef->SJID > 0)
		{
			SJTASKDEF *pstSJTaskDef;
	
			pstSJTaskDef = z_sj_task_def(pstEnv, pstTaskDef->SJID);
			if(!pstSJTaskDef)
			{
				return -1;
			}
			
			iMoney = pstSJTaskDef->StarRate[pstTaskRunOne->SJTaskStar].MoneyMul;
			iExp = pstSJTaskDef->StarRate[pstTaskRunOne->SJTaskStar].ExpMul;
		}
		else
		{
			iMoney = pstTaskAwardDef->Money.Val;
			iExp = pstTaskAwardDef->Exp;
		}

		if (iExp > 0)
		{
			int iRet;

			if (	pstTaskDef->Type == TASK_TYPE_DART_CAR &&
				pstPlayer->stRoleData.MiscInfo.DartCarInfo.AwardRate > 0)
			{
				
				ZoneClan *pstZoneClan = NULL;
				CLANDARTINFO *pstDartInfo = NULL;

				iExp = iExp*(pstPlayer->stRoleData.MiscInfo.DartCarInfo.AwardRate/100.0);
				
				if (pstPlayer->stRoleData.MiscInfo.DartCarInfo.AddRate > 0)
				{
					//lzk    公会技能为公会商运的经验乘修正
					pstZoneClan = player_get_clan(pstEnv, pstPlayer);
					if (pstZoneClan)
					{				
						 pstDartInfo = &pstZoneClan->stClan.ClanAdvance.DartInfo;
						if(pstDartInfo->ClanBusinessExpMul >0)
						{
							iExp += iExp *(pstDartInfo->ClanBusinessExpMul/1000.0);
						}
					}
					
					//原来的
					//iExp += iExp*0.1f;
				}
			}
			
			iRet = z_player_addexp_test(pstEnv,pstPlayer, &iExp);
			//if (iRet < 0 && !pstTaskDef->AutoFini && iRet != -3)
			
			if (iRet < 0 && (0 == pstTaskDef->AutoFini || 2 == pstTaskDef->AutoFini) && iRet != -3)
			{
				return -1;
			}
		}
		
		if (pstTaskAwardDef->ItemAwardType == TASK_ITEM_AWARD_ONE)
		{
			if (pstTaskAward->SelectItemID == 0)
			{
				return -1;
			}
			
			for (i=0; i<MAX_TASK_AWARD_ITEM; i++)
			{
				if (0 == pstTaskAwardDef->Items[i].ID)
				{
					break;
				}

				if (iRoleGrid >= (int)(sizeof(astRoleGrid)/sizeof(astRoleGrid[0])) )
				{
					break;
				}

				if ( (CAREER_NON  != pstTaskAwardDef->Items[i].Career && (pstTaskAwardDef->Items[i].Career&0x0F) != (pstPlayer->stRoleData.Career&0x0F)) ||
					 (CAREER_NON  != (pstTaskAwardDef->Items[i].Career&0xF0) && (pstTaskAwardDef->Items[i].Career&0xF0) != (pstPlayer->stRoleData.Career&0xF0)) )
				{
					continue;
				}
				
				if (pstTaskAward->SelectItemID == pstTaskAwardDef->Items[i].ID)
				{
					if(task_award_item_create(pstEnv, &pstTaskAwardDef->Items[i],
					                          &astRoleGrid[iRoleGrid]) < 0)
					{
						return -1;
					}

					if (GRID_ITEM_ITEM == astRoleGrid[iRoleGrid].Type)
					{
						astRoleGrid[iRoleGrid].GridData.RoleItem.Num = pstTaskAwardDef->Items[i].Num;
						task_award_spar(&astRoleGrid[iRoleGrid].GridData.RoleItem);
					}
					
					iRoleGrid++;
					break;
				}
			}
		}
		else if (pstTaskAwardDef->ItemAwardType == TASK_ITEM_AWARD_ALL)
		{
			for (i=0; i<MAX_TASK_AWARD_ITEM; i++)
			{
				if (0 == pstTaskAwardDef->Items[i].ID)
				{
					break;
				}
				
				if ( (CAREER_NON  != pstTaskAwardDef->Items[i].Career && (pstTaskAwardDef->Items[i].Career&0x0F) != (pstPlayer->stRoleData.Career&0x0F)) ||
					 (CAREER_NON  != (pstTaskAwardDef->Items[i].Career&0xF0) && (pstTaskAwardDef->Items[i].Career&0xF0) != (pstPlayer->stRoleData.Career&0xF0)) )
				{
					continue;
				}				

				if (iRoleGrid >= (int)(sizeof(astRoleGrid)/sizeof(astRoleGrid[0])) )
				{
					break;
				}
			
				if(task_award_item_create(pstEnv, &pstTaskAwardDef->Items[i],
				                          &astRoleGrid[iRoleGrid]) < 0)
				{
					return -1;
				}

				if (GRID_ITEM_ITEM == astRoleGrid[iRoleGrid].Type)
				{
					astRoleGrid[iRoleGrid].GridData.RoleItem.Num = pstTaskAwardDef->Items[i].Num;
					task_award_spar(&astRoleGrid[iRoleGrid].GridData.RoleItem);
				}
				
				iRoleGrid++;
			}
		}
		else if (pstTaskAwardDef->ItemAwardType == TASK_ITEM_AWARD_SHANGJIN)
		{
			if(pstTaskDef->SJID <= 0)
			{
				return -1;
			}
				
			for (i=0; i<MAX_TASK_AWARD_ITEM; i++)
			{
				if (0 == pstTaskAwardDef->Items[i].ID)
				{
					break;
				}

				if (iRoleGrid >= (int)(sizeof(astRoleGrid)/sizeof(astRoleGrid[0])) )
				{
					break;
				}

				if (pstPlayer->stRoleData.MiscInfo.SJTaskInfo.GetItemNum >= SJ_MAX_SUIPIAN_WEEK)
				{
					break;
				}

				if (pstTaskAwardDef->Items[i].Career != pstTaskRunOne->SJTaskStar+1)
				{
					continue;
				}
				
				if(task_award_item_create(pstEnv, &pstTaskAwardDef->Items[i],
				                          &astRoleGrid[iRoleGrid]) < 0)
				{
					return -1;
				}

				if (GRID_ITEM_ITEM == astRoleGrid[iRoleGrid].Type)
				{
					astRoleGrid[iRoleGrid].GridData.RoleItem.Num = pstTaskAwardDef->Items[i].Num;
					task_award_spar(&astRoleGrid[iRoleGrid].GridData.RoleItem);
				}
				
				iRoleGrid++;
				iSuipian = 1; 		//赏金令碎片
				break;
			}
		}
	}

	 task_dart_car_item_award(pstEnv, pstPlayer, pstTaskDef, &astRoleGrid[0], &iRoleGrid);
	
	if (iRoleGrid > 0)
	{
		if (0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, astRoleGrid, iRoleGrid,0))
		{
			if (0 == pstTaskDef->AutoFini || 2 == pstTaskDef->AutoFini)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
								ZONEERR_TASK217);
				return -1;
			}
		}
	}
	
	if(iMoney > 0)
	{
		if (0 > package_add_money_test(pstEnv, pstPlayer, iMoneyType, iMoney) &&
			iMoneyType == ATTR_ID_MONEY)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
								ZONEERR_NOMORE_MONEY3);
			return -1;
		}
	}
	
	// 商运镖车退还押金
	iMoney += task_award_cacl_dratcar_yajin(pstEnv, pstPlayer,pstTaskDef);
	task_end_dart_car_clear(pstEnv, pstPlayer, pstTaskDef);

	//清理运镖信息
	if(is_convoy_task(pstTaskDef))
	{
		z_clear_convoy_info(pstPlayer);
	}

	//跑商完成任务时，需要清除银票、身上的贸易商品、已兑换的本金上限
	if (pstTaskDef->Type == TASK_TYPE_PAOSHANG)
	{
		player_paoshang_clean(pstEnv,pstPlayer);
		player_paoshang_login_check(pstEnv, pstPlayer);
	}
	
	iRet  = task_commit(pstEnv, pstPlayer, pstTaskRunOne, pstTaskDef);
	if (iRet >= 0)
	{
		Npc *pstNpc = NULL;

		//任务日志相关流水号
		z_oplog_event_begin(pstEnv);
		
		z_role_task_oplog(pstEnv, pstPlayer, (unsigned short)pstTaskAward->TaskID,
	                  OP_TASK_FINAL,astRoleGrid, iRoleGrid, iMoney, iMoneyType, iExp, 0);

		if (iRoleGrid > 0)
		{
			z_role_task_item_award(pstEnv, pstPlayer, pstTaskAward->TaskID, astRoleGrid, iRoleGrid);
		}
		
			
		// 赏金令碎片限制
		if (iSuipian)
		{
			pstPlayer->stRoleData.MiscInfo.SJTaskInfo.GetItemNum ++;
			player_sj_item_info(pstEnv, pstPlayer);
		}
		task_award_resource(pstEnv, pstPlayer, iCycleID, pstTaskDef, astRoleGrid, 
							iRoleGrid, iMoney, iMoneyType, iExp,iContri,iRet,iClanMoney);

		if (pstTaskDef->Type == TASK_TYPE_DAILY_GOAL)
		{
			pstPlayer->stRoleData.MiscInfo.DailyActive.TaskNum++;
			player_daily_goal_info(pstEnv,pstPlayer);
		}
		

		//任务脚本
		if ((pstTaskDef->TaskScript & TASK_SCRIPT_PROC_AWARD_MAP))
		{	
			ScriptIndex *pstScriptInd;
			char szScript[32];	
		
			stSEnv.pstMonster = NULL;
			stSEnv.pstNpc = NULL;
			stSEnv.pstPlayer = pstPlayer;
			stSEnv.pstMapinst = NULL;
			stSEnv.pstKiller = NULL;
			
			snprintf(szScript, sizeof(szScript), "task%d.mac", pstTaskDef->ID);				
			pstScriptInd = get_script_ind(szScript);

			if ( !pstScriptInd )
			{
				printf("get_script_ind fail\n");
				tlog_error(pstEnv->pstLogCat, 0, 0, "get_script_ind fail");
				return -1;
			}
			
			z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, szScript, pstScriptInd, TASK_SCRIPT_PROC_AWARD);
		}
		
		// 完成任务 回调脚本 
		pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstTaskAward->NpcID);
		if (pstNpc)
		{
			if (NPC_SCRIPT_PROC_AWARD_TASK_MAP & pstNpc->cScriptFlag)
			{
				pstPlayer->stOnline.iDialogID = pstNpc->stMon.iID;
				
				stSEnv.pstMonster = NULL;
				stSEnv.pstNpc = pstNpc;
				stSEnv.pstPlayer = pstPlayer;
				stSEnv.pstPworldInst = NULL;
				stSEnv.iTaskCurr = pstTaskAward->TaskID;
				stSEnv.pstMapinst = NULL;
				stSEnv.pstKiller = NULL;

				z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstNpc->szScriptName, &pstNpc->stScriptIdx, NPC_SCRIPT_PROC_AWARD_TASK);
			}

			task_dialog_again(pstEnv, pstPlayer, pstNpc);
		}

		if (1 == pstTaskDef->AutoFini)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_TASK218, pstTaskDef->Name);
		}

		//成长之路
		{
			ACHIEVEMENTCOND stCond;

			memset(&stCond,0,sizeof(stCond));
			
			stCond.Type = COND_TASK_DONE;
			stCond.Val1 = pstTaskDef->ID;

			z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
		}

	}

	return 0;
}

int taskcycle_new(ZONESVRENV *pstEnv, Player *pstPlayer, TASKRECYCLEDEF *pstTaskCycleDef)
{
	int iTaskID;
	TASKDEF *pstTaskDef;
	//TASKRECYCLEONE *pstTaskCycleOne;
		
	if (0 > taskcycle_accept_check(pstEnv, pstPlayer, pstTaskCycleDef, 0))	
	{
		return -1;
	}
	
	iTaskID = taskrecycle_get_task(pstEnv, pstPlayer, pstTaskCycleDef, 0, NULL);
	if (0 > iTaskID)
	{
		return -1;
	}

	pstTaskDef = task_def_find(pstEnv, iTaskID, NULL);
	if (NULL == pstTaskDef)
	{
		return -1;
	}
	
	if (0 > taskcycle_add_run(pstEnv, pstPlayer, pstTaskCycleDef, iTaskID))
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "taskcycle_add_run fail");
		return -1;
	}

	return iTaskID;
}


int sec_trans_char(int iSec, char *pszTime, int iLen)
{
	if (iSec > 60)
	{
		if (iSec%60)
		{
			snprintf(pszTime, iLen, "%d%s%d%s",iSec/60,LJY_MINUTE,iSec%60,LJY_SECOND);
		}
		else
		{
			snprintf(pszTime, iLen, "%d%s",iSec/60,LJY_MINUTE);
		}
	}
	else
	{
		snprintf(pszTime, iLen, "%d%s",iSec, LJY_SECOND);
	}
	pszTime[iLen-1]=0;
	return 0;
}

int taskcycle_option(ZONESVRENV *pstEnv, Player *pstPlayer, int iNPCID, int iTaskCycleID)
{
	int iTaskID = 0;
	TASKDEF *pstTaskDef;
	TASKRECYCLEDEF *pstTaskCycleDef;
	TASKRECYCLEONE *pstTaskCycleOne;
	TASKRUNONE *pstTaskRunOne = NULL;
	int iDropInterVal = TASK_CYCLE_DROP_COOL;
	char szTip[32];

	pstTaskCycleDef = taskcycle_def_find(pstEnv, iTaskCycleID, NULL);
	if (NULL == pstTaskCycleDef)
	{
		return -1;
	}

	if (pstTaskCycleDef->DropInterVal > 0)
	{
		iDropInterVal = pstTaskCycleDef->DropInterVal;
	}

	pstTaskCycleOne = taskcycle_run_find(pstPlayer, iTaskCycleID);	
	if (pstTaskCycleOne)
	{
		if (pstTaskCycleOne->TaskRecycleIdxDropTime != 0 && 
			pstEnv->pstAppCtx->stCurr.tv_sec - pstTaskCycleOne->TaskRecycleIdxDropTime < iDropInterVal)
		{
			sec_trans_char(iDropInterVal, szTip, sizeof(szTip));
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK245,szTip);
			return -1;
		}
		
		iTaskID = pstTaskCycleOne->TaskID;
		pstTaskDef = task_def_find(pstEnv, iTaskID, NULL);
		pstTaskRunOne = task_run_find(pstPlayer, iTaskID);
		if (pstTaskDef == NULL || (NULL == pstTaskRunOne && 0 > task_accept_check_matchclient(pstEnv, pstPlayer, pstTaskDef, 0)))	
		{
			if (pstTaskRunOne)
			{
				pstTaskRunOne->TaskStat = TASK_STAT_COMMIT;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				
				task_run_del(pstEnv, pstPlayer, iTaskID);
			}
			
			pstTaskCycleOne = NULL; 
			taskcycle_run_del(pstPlayer, iTaskCycleID);
		}
	}

	if (pstTaskCycleOne == NULL)
	{
		iTaskID = taskcycle_new(pstEnv, pstPlayer, pstTaskCycleDef);
		if (iTaskID < 0 )
		{
			return -1;
		}
	}

	task_option(pstEnv, pstPlayer, iNPCID, iTaskID);
	
	return 0;	
}

int taskcycle_opt_str(ZONESVRENV *pstEnv, Npc *pstNpc, Player *pstPlayer, char *pszOptStr, int iOptStr, int iTaskCycleID)
{
	int iTaskID;
	char szBuff[32];
	TASKDEF *pstTaskDef;
	TASKRECYCLEDEF *pstTaskCycleDef;
	TASKRECYCLEONE *pstTaskCycleOne;
	TASKRUNONE *pstTaskRunOne = NULL;
	int iLen  = 0;
	char szBuff1[64];

	pstTaskCycleDef = taskcycle_def_find(pstEnv, iTaskCycleID, NULL);
	if (NULL == pstTaskCycleDef)
	{
		return -1;
	}

	szBuff[0] = 0;
	switch(pstTaskCycleDef->CycType)
	{
		case CYC_TYPE_CLAN:
			STRNCPY(szBuff, LJY_TASK1,sizeof(szBuff));
			break;
		case CYC_TYPE_PLOY:
			STRNCPY(szBuff, LJY_TASK2,sizeof(szBuff));
			break;
		default:
			STRNCPY(szBuff, LJY_TASK3,sizeof(szBuff));
			break;
	}

	pstTaskCycleOne = taskcycle_run_find(pstPlayer, iTaskCycleID);	
	if (pstTaskCycleOne && (pstTaskRunOne = task_run_find(pstPlayer, pstTaskCycleOne->TaskID)))
	{
		
		iTaskID = pstTaskCycleOne->TaskID;
		pstTaskDef = task_def_find(pstEnv, iTaskID, NULL);
		if (NULL == pstTaskDef)
		{
			return -1;
		}

		if (0 > taskrecycle_check_npc(pstEnv, pstPlayer, pstNpc->stMon.iID, iTaskCycleID, 1))
		{
			return -1;
		}
		
		if (pstTaskRunOne && task_check_fini(pstEnv, pstPlayer, pstTaskRunOne, pstTaskDef))
		{
			//if(pstTaskCycleDef->CycType == CYC_TYPE_PLOY)
			{
				snprintf(pszOptStr, iOptStr, "(%s)[%s] %s ", LJY_TASK4, szBuff, pstTaskCycleDef->Name);
			}
			/*
			else
			{
				snprintf(pszOptStr, iOptStr, "(完成)[%s] %s 第%d环", szBuff, pstTaskCycleDef->Name, pstTaskCycleOne->TaskRecycleIdx+1);
			}
			*/
		}
		else
		{
			//if(pstTaskCycleDef->CycType == CYC_TYPE_PLOY)
			{
				snprintf(pszOptStr, iOptStr, "(%s)[%s] %s ", LJY_TASK5, szBuff, pstTaskCycleDef->Name);
			}
			/*
			else
			{
				snprintf(pszOptStr, iOptStr, "(未完成)[%s] %s 第%d环", szBuff, pstTaskCycleDef->Name, pstTaskCycleOne->TaskRecycleIdx+1);
			}
			*/
		}
	}
	else
	{
		if (0 > taskrecycle_check_npc(pstEnv, pstPlayer, pstNpc->stMon.iID, iTaskCycleID, 0))
		{
			return -1;
		}

		if (0 > taskcycle_accept_check(pstEnv, pstPlayer, pstTaskCycleDef, 1))	
		{
			return -1;
		}
		
		if (NULL == pstTaskCycleOne)
		{
			iTaskID = taskrecycle_get_task(pstEnv, pstPlayer, pstTaskCycleDef, 0, NULL);
			if (0 > iTaskID)
			{
				return -1;
			}

			//if(pstTaskCycleDef->CycType == CYC_TYPE_PLOY)
			{
				snprintf(pszOptStr, iOptStr, "#(%s)[%s] %s", LJY_TASK6, szBuff,pstTaskCycleDef->Name);
			}
			/*
			else
			{
				snprintf(pszOptStr, iOptStr, "#(可接)[%s] %s 第1环", szBuff,pstTaskCycleDef->Name);
			}
			*/
		}
		else
		{
			//if(pstTaskCycleDef->CycType == CYC_TYPE_PLOY)
			{
				snprintf(pszOptStr, iOptStr, "#(%s)[%s] %s ", LJY_TASK6, szBuff,pstTaskCycleDef->Name);
			}
			/*
			else
			{
				snprintf(pszOptStr, iOptStr, "#(可接)[%s] %s 第%d环", szBuff,pstTaskCycleDef->Name, pstTaskCycleOne->TaskRecycleIdx+1);
			}
			*/
		}	
	}


	iLen  = strlen(pszOptStr);
	//char szBuff1[64];
	szBuff1[0] = 0;
	if (pstTaskCycleDef->MinLevel > 0)
	{
		if (pstTaskCycleDef->MaxLevel > 0)
		{
			snprintf(szBuff1, sizeof(szBuff1), LJY_TASK7, pstTaskCycleDef->MinLevel,pstTaskCycleDef->MaxLevel);
		}
		else
		{
			snprintf(szBuff1, sizeof(szBuff1), LJY_TASK8, pstTaskCycleDef->MinLevel);
		}
	}
	else
	{
		if (pstTaskCycleDef->MaxLevel > 0)
		{
			snprintf(szBuff1, sizeof(szBuff1), LJY_TASK9, pstTaskCycleDef->MaxLevel);
		}
	}


	if (szBuff1[0] != 0 && iLen < iOptStr)
	{
		snprintf(&pszOptStr[iLen], iOptStr-iLen, "%s", szBuff1);
	}
	
	pszOptStr[iOptStr - 1] = 0;
	
	return 0;	
}


int task_fini_clear_dart_car(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	Monster *pstMon;

	if (pstPlayer->stRoleData.MiscInfo.DartCarInfo.MonMemID <= 0)
	{
		return 0;
	}

	pstMon = z_id_monster(pstEnv->pstAppCtx,  pstEnv, pstPlayer->stRoleData.MiscInfo.DartCarInfo.MonMemID);
	if (pstMon)
	{
		z_ride_machine_clear(pstEnv, pstMon);
		//z_ride_machine_clear(pstEnv, pstMon);
		z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);
		z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);
	}

	pstPlayer->stRoleData.MiscInfo.DartCarInfo.MonMemID = 0;
	return 0;
}

int task_check_fini(ZONESVRENV *pstEnv, Player *pstPlayer, TASKRUNONE *pstTaskRunOne, TASKDEF *pstTaskDef)
{
	int i ;
	int iOldStat = pstTaskRunOne->TaskStat;

	if (pstTaskRunOne->TaskStat != TASK_STAT_RUN && pstTaskRunOne->TaskStat != TASK_STAT_FINI)
	{
		return 0;
	}

	if (pstTaskRunOne->TaskItemTrace.TaskItemTraceNum > 0 || pstTaskRunOne->TaskMonTrace.TaskMonTraceNum > 0)
	{
		pstTaskRunOne->TaskStat = TASK_STAT_RUN;
	}
	else if (pstTaskRunOne->TaskStat == TASK_STAT_FINI)
	{
		task_end_dart_car_clear(pstEnv, pstPlayer, pstTaskDef);
		return 1;
	}
	else if(!pstTaskDef->FiniTaskArea.MapID)
	{
		return 0;
	}

	if ((pstTaskDef->TaskFlag & TASK_SPY_TARGET) && 
		0 == pstPlayer->stRoleData.MiscInfo.DailyInfo.SpyInfo.TargetClanID)
	{
		return 0;
	}

	for (i=0; i<pstTaskRunOne->TaskItemTrace.TaskItemTraceNum; i++)
	{
		if (pstTaskRunOne->TaskItemTrace.TaskGetItems[i].ItemID == 0)
		{
			break;
		}

		if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
				pstTaskRunOne->TaskItemTrace.TaskGetItems[i].ItemID, 
				pstTaskRunOne->TaskItemTrace.TaskGetItems[i].MaxNum, 
				-1, NULL,PACKAGE_DEC_NONE))
		{
			return 0;
		}		
	}

	for (i=0; i<pstTaskRunOne->TaskMonTrace.TaskMonTraceNum; i++)
	{
		if (pstTaskRunOne->TaskMonTrace.TaskKillMons[i].Num < pstTaskRunOne->TaskMonTrace.TaskKillMons[i].MaxNum)
		{
			return 0;
		}
	}

	if (pstTaskDef->FiniTaskArea.MapID && (pstTaskRunOne->TaskStat != TASK_STAT_FINI))
	{
		MapIndex* pstMapIdx;
		unsigned int uiMap = GET_MAPID(pstPlayer->stRoleData.Map);

		pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, uiMap);
		if (pstMapIdx && pstMapIdx->stMapDef.ReferMapID > 0 && pstMapIdx->stMapDef.MapLine > 0)
		{
			uiMap = pstMapIdx->stMapDef.ReferMapID;
		}
		
		if (uiMap != pstTaskDef->FiniTaskArea.MapID ||
			pstTaskDef->FiniTaskArea.Radius < z_distance(&pstPlayer->stRoleData.Pos, &pstTaskDef->FiniTaskArea.Pos))
		{
			return 0;
		}
	}

	pstTaskRunOne->TaskStat = TASK_STAT_FINI;
	task_end_dart_car_clear(pstEnv, pstPlayer, pstTaskDef);

	//任务脚本
	if (iOldStat != TASK_STAT_FINI && (pstTaskDef->TaskScript & TASK_SCRIPT_PROC_FINI_MAP))
	{	
		ScriptIndex *pstScriptInd;
		char szScript[32];	
	
		stSEnv.pstMonster = NULL;
		stSEnv.pstNpc = NULL;
		stSEnv.pstPlayer = pstPlayer;
		stSEnv.pstMapinst = NULL;
		stSEnv.pstKiller = NULL;
		
		snprintf(szScript, sizeof(szScript), "task%d.mac", pstTaskDef->ID);				
		pstScriptInd = get_script_ind(szScript);

		if ( !pstScriptInd )
		{
			printf("get_script_ind fail\n");
			tlog_error(pstEnv->pstLogCat, 0, 0, "get_script_ind fail");
			return -1;
		}
		
		z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, szScript, pstScriptInd, TASK_SCRIPT_PROC_FINI);
	}

	return 1;
}


int task_auto_pop(ZONESVRENV *pstEnv, Player *pstPlayer, int iTaskID, int iNpcID)
{
	TASKDEF *pstTaskDef;
	Npc *pstNpc;

	pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, iNpcID);
	if (NULL == pstNpc)
	{
		return -1;
	}

	pstTaskDef = task_def_find(pstEnv, iTaskID, NULL);
	if (!pstTaskDef )
	{
		return -1;
	}

	if (task_check_npc(pstEnv, pstPlayer, iNpcID, iTaskID, 0) < 0)
	{
		return -1;
	}

	if (task_accept_check_matchclient(pstEnv, pstPlayer, pstTaskDef, 0) < 0)
	{
		return -1;
	}

	return task_dialog(pstEnv, pstPlayer, pstNpc->stMon.iID, iTaskID, TASK_DIALOG_ACCEPT);
	
}

int task_fini(ZONESVRENV *pstEnv, Player *pstPlayer, int iTaskID, int iNotify)
{
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;

	pstTaskRunOne = task_run_find(pstPlayer, iTaskID);
	if (NULL == pstTaskRunOne)
	{
		return -1;
	}

	pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
	if (NULL == pstTaskDef)
	{
		return -1;
	}

	if (0 == task_check_fini(pstEnv, pstPlayer, pstTaskRunOne, pstTaskDef))
	{
		return -1;
	}

	if (1 == pstTaskDef->AutoFini)
	{
		CSTASKAWARD stTaskAward;

		stTaskAward.NpcID = 0;
		stTaskAward.SelectItemID = 0;
		stTaskAward.TaskID = pstTaskDef->ID;
		task_award(pstEnv, pstPlayer, &stTaskAward);
	}
	else	if (iNotify || pstTaskDef->FiniTaskArea.MapID > 0)
	{
		task_chg(pstEnv, pstPlayer, pstTaskRunOne);
	}

	// 设置任务完成删除熔炉，清空角色数据
	ronglu_task_fini(pstEnv, pstPlayer, pstTaskDef);

	if (pstTaskDef->Type == TASK_TYPE_PAOSHANG)
	{
		player_paoshang_login_check(pstEnv, pstPlayer);
	}
	
	return 0;
}

void task_def_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	TASKINFO *pstRoleTaskInfo = &pstPlayer->stRoleData.MiscInfo.TaskInfo;
	TASKRUN *pstTaskRun = &pstRoleTaskInfo->TaskRun;
	TASKDEF *pstTaskDef;
	int i;
	
	for (i=pstTaskRun->TaskRunNum-1; i>=0; i--)
	{
		pstTaskDef = task_def_find(pstEnv, pstTaskRun->TaskRuns[i].TaskID, &pstTaskRun->TaskRuns[i].TaskIdx);
		if(NULL == pstTaskDef)
		{
			pstTaskRun->TaskRunNum--;
			if(i != pstTaskRun->TaskRunNum)
			{
				memmove(&pstTaskRun->TaskRuns[i], &pstTaskRun->TaskRuns[i+1], 
							(pstTaskRun->TaskRunNum-i)*sizeof(pstTaskRun->TaskRuns[0]));
			}
		}
		else
		{
			int iFailed = 0;
			//运镖
			if((pstTaskDef->Type == TASK_TYPE_TEAM_CONVOY || pstTaskDef->Type == TASK_TYPE_SINGLE_CONVOY) && pstTaskRun->TaskRuns[i].TaskStat != TASK_STAT_FAIL)
			{
				z_clear_convoy_info(pstPlayer);
				
				pstTaskRun->TaskRuns[i].TaskStat = TASK_STAT_FAIL;
				iFailed = 1;
			}
			//离线则任务失败
			else if (pstTaskRun->TaskRuns[i].TaskStat == TASK_STAT_RUN)
			{
				 if(pstTaskDef->TaskFlag & TASK_OFF_LINE_FAIL)
				 {
					pstTaskRun->TaskRuns[i].TaskStat = TASK_STAT_FAIL;
					iFailed = 1;
				 }
			}
			
			if(iFailed && (pstTaskDef->TaskScript & TASK_SCRIPT_PROC_FAIL_MAP))
			{	
				ScriptIndex *pstScriptInd;
				char szScript[32];	
			
				stSEnv.pstMonster = NULL;
				stSEnv.pstNpc = NULL;
				stSEnv.pstPlayer = pstPlayer;
				stSEnv.pstMapinst = NULL;
				stSEnv.pstKiller = NULL;
				
				snprintf(szScript, sizeof(szScript), "task%d.mac", pstTaskDef->ID);				
				pstScriptInd = get_script_ind(szScript);

				if ( !pstScriptInd )
				{
					printf("get_script_ind fail\n");
					tlog_error(pstEnv->pstLogCat, 0, 0, "get_script_ind fail");
				}
				
				z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, szScript, pstScriptInd, TASK_SCRIPT_PROC_FAIL);
			}
		}
	}
	
	return ;
}


int task_info_all(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSTASKSVR *pstTaskSvr = &stPkg.Body.TaskSvr;
	CSTASKINFO *pstTaskInfo = &pstTaskSvr->TaskSvrData.GetTaskInfo;

	TASKINFO *pstRoleTaskInfo = &pstPlayer->stRoleData.MiscInfo.TaskInfo;
	TASKRUN *pstTaskRun = &pstRoleTaskInfo->TaskRun;
	TASKRECYCLE *pstTaskRecycle =  &pstRoleTaskInfo->TaskRecycle;
	int i;
	
	pstTaskSvr->OpType = TASK_INFO;
	pstTaskInfo->TaskRunNum = pstTaskRun->TaskRunNum;
	for (i=0; i<pstTaskRun->TaskRunNum; i++)
	{
		task_one_csinfo(pstEnv, pstPlayer, &pstTaskInfo->TaskRuns[i], &pstTaskRun->TaskRuns[i]);
	}

	pstTaskInfo->TaskFiniNum = pstRoleTaskInfo->TaskFini.TaskNum;
	memcpy(pstTaskInfo->TaskFinis, pstRoleTaskInfo->TaskFini.TaskIDs, 
			pstTaskInfo->TaskFiniNum*sizeof(pstTaskInfo->TaskFinis[0]));

	pstTaskInfo->TaskOneDayNum = pstRoleTaskInfo->TaskOneDay.TaskNum;
	memcpy(pstTaskInfo->TaskOneDayInfo, pstRoleTaskInfo->TaskOneDay.TaskOneDayInfo, 
			pstTaskInfo->TaskOneDayNum*sizeof(pstTaskInfo->TaskOneDayInfo[0]));

	memcpy(&pstTaskInfo->TaskCycleOneDay, &pstRoleTaskInfo->TaskCycleOneDay, sizeof(pstTaskInfo->TaskCycleOneDay));

	memcpy(&pstTaskInfo->GroupOneDay, &pstRoleTaskInfo->GroupOneDay, sizeof(pstTaskInfo->GroupOneDay));

	pstTaskInfo->CycleWeek = pstRoleTaskInfo->CycleWeek;
	pstTaskInfo->GroupWeek = pstRoleTaskInfo->GroupWeek;

	//环组信息
	for (i=0; i<pstTaskRecycle->TaskRecycleNum; i++)
	{
		pstTaskInfo->TaskRecycleInfo[i].TaskRecycleID = pstTaskRecycle->TaskRecycles[i].TaskRecycleID;
		pstTaskInfo->TaskRecycleInfo[i].TaskID = pstTaskRecycle->TaskRecycles[i].TaskID;
		pstTaskInfo->TaskRecycleInfo[i].TaskCycleIdx = pstTaskRecycle->TaskRecycles[i].TaskRecycleIdx;
		pstTaskInfo->TaskRecycleInfo[i].TaskRecycleGetTime = pstTaskRecycle->TaskRecycles[i].GetTime;
	}
	pstTaskInfo->TaskRecycleNum = pstTaskRecycle->TaskRecycleNum;

	memcpy(&pstTaskInfo->HideTaskInfo,&pstRoleTaskInfo->HideTaskInfo,sizeof(pstTaskInfo->HideTaskInfo));

	Z_CSHEAD_INIT(&stPkg.Head, TASK_SVR);

	return z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
}

int task_info_get(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	TASKINFO *pstRoleTaskInfo = &pstPlayer->stRoleData.MiscInfo.TaskInfo;
	TASKONEDAY *pstTaskOneDay = &pstRoleTaskInfo->TaskOneDay;

	if (MISC_FLAG_TASK_ALL & pstPlayer->stOnline.cMiscFlag)
	{
		return -1;
	}

	//检查无效的任务
	task_def_check(pstEnv, pstPlayer);

	for (i=pstTaskOneDay->TaskNum-1; i>=0; i--)
	{
		if (0 == IsSameDay(pstTaskOneDay->TaskOneDayInfo[i].Time, pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			pstTaskOneDay->TaskNum--;
			if (i != pstTaskOneDay->TaskNum)
			{
				pstTaskOneDay->TaskOneDayInfo[i] = pstTaskOneDay->TaskOneDayInfo[pstTaskOneDay->TaskNum];
			}
		}
	}

	if (task_info_all(pstEnv, pstPlayer) == 0)
	{
		ronglu_player_login(pstEnv, pstPlayer);
		pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_TASK_ALL;
	}
	return 0;
}

int trust_open_face(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i = 0;
	CSPKG stPkg;
	CSTRUSTSVR *pstTrustSvr = &stPkg.Body.TrustSvr;
	CSTRUSTVERSIONLIST *pstTrusVertList = &pstTrustSvr->TrustSvrData.TrustVerList;
	ROLETRUSTDATA stTrustHash, *pstTrustHash = NULL;
	BUDDY *pstBuddy = &pstPlayer->stOnline.stRelation.Buddy;

	//频繁请求限制
	if (pstPlayer->stOnline.tTrustUpdateEnd < pstEnv->pstAppCtx->stCurr.tv_sec)
	{	
		pstTrusVertList->RoleNum = 0;
		
		for (i = 0; i< pstBuddy->BuddyNum; i++)
		{
			stTrustHash.RoleID = pstBuddy->BuddyInfos[i].RoleID;
			pstTrustHash = sht_find(pstEnv->pstShtTrust, &stTrustHash, TrustHashCmp, TrustHashCode);
			if (pstTrustHash)
			{
				pstTrusVertList->RoleVersionList[pstTrusVertList->RoleNum].RoleID = pstTrustHash->RoleID;
				pstTrusVertList->RoleVersionList[pstTrusVertList->RoleNum].RoleTrustVersion= pstTrustHash->RoleTrustVersion;
				pstTrusVertList->RoleNum++;
			}
		}

		pstPlayer->stOnline.tTrustUpdateEnd = pstEnv->pstAppCtx->stCurr.tv_sec + TRUST_OPEN_LIMIT;

		pstTrustSvr->TrustSvrType = TRUST_VER_LIST;
		Z_CSHEAD_INIT(&stPkg.Head, TRUST_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
		
	return 0;
}

int trust_role_vercmp(ZONESVRENV* pstEnv, Player *pstPlayer, CSTRUSTROLELIST *pstTrustRoleList )
{
	int i = 0;
	CSPKG stPkg;
	CSTRUSTSVR *pstTrustSvr = &stPkg.Body.TrustSvr;
	CSTRUSTORDERLIST *pstTrustOrderList = &pstTrustSvr->TrustSvrData.TrustOrderList;
	ROLETRUSTDATA  stTrustHash, *pstTrustHash = NULL;
	
	pstTrustOrderList->RoleNum = 0;
	
	for (i = 0; i< pstTrustRoleList->RoleNum; i++)
	{
		stTrustHash.RoleID = pstTrustRoleList->RoleID[i];
		pstTrustHash = sht_find(pstEnv->pstShtTrust, &stTrustHash, TrustHashCmp, TrustHashCode);
	       if (pstTrustHash && pstTrustHash->RoleID == stTrustHash.RoleID)
		{
			memcpy(&pstTrustOrderList->RoleTrust[pstTrustOrderList->RoleNum], pstTrustHash, sizeof(pstTrustOrderList->RoleTrust[0]));
			pstTrustOrderList->RoleNum++;
		}
		else
		{
			pstTrustOrderList->RoleTrust[pstTrustOrderList->RoleNum].RoleID = pstTrustRoleList->RoleID[i];
			pstTrustOrderList->RoleTrust[pstTrustOrderList->RoleNum].RoleTrustVersion = -1;
			pstTrustOrderList->RoleTrust[pstTrustOrderList->RoleNum].TrustNum = 0;
			pstTrustOrderList->RoleNum++;
		}
		
	}

	pstTrustSvr->TrustSvrType = TRUST_ORDER_LIST;
	Z_CSHEAD_INIT(&stPkg.Head, TRUST_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

TRUSTTASK*  find_trust_taskid(ROLETRUSTDATA *pstRoleTrust, unsigned int uiTaskID)
{
	int iNum = 0;
	
	for (iNum = 0; iNum< pstRoleTrust->TrustNum; iNum++)
	{
		if ( pstRoleTrust->TrustTaskData[iNum].TaskID == uiTaskID)
		{
			return &pstRoleTrust->TrustTaskData[iNum];
		}
	}
	
	return NULL;
}

int get_taskiteam_idx(TASKDEF *pstTaskDef, unsigned int uiItemID)
{
	int i = 0;

	for(i = 0; i <MAX_TASK_ITEM; i++)
	{
		if (pstTaskDef->GetItem[i].ItemID == 0 || pstTaskDef->GetItem[i].Num == 0)
		{
			break;
		}
		
		if ( pstTaskDef->GetItem[i].ItemID == uiItemID)
		{
			return i;
		}
	}
	
	return -1; 
}

void trust_onerole_update(ZONESVRENV* pstEnv, Player *pstPlayer, ROLETRUSTDATA *pstTrustHash, tdr_ulonglong ullRoleID)
{
	CSPKG stPkg;
	CSTRUSTSVR *pstTrustSvr = &stPkg.Body.TrustSvr;
	CSTRUSTORDERLIST  *pstTrustOrderList = &pstTrustSvr->TrustSvrData.TrustOrderList;

	pstTrustOrderList->RoleNum = 0;

	if (pstTrustHash)
	{
		memcpy(&pstTrustOrderList->RoleTrust[pstTrustOrderList->RoleNum], pstTrustHash, sizeof(pstTrustOrderList->RoleTrust[0]));	
	}
	else if (ullRoleID)
	{
		pstTrustOrderList->RoleTrust[pstTrustOrderList->RoleNum].RoleID = ullRoleID;
		pstTrustOrderList->RoleTrust[pstTrustOrderList->RoleNum].RoleTrustVersion = -1;
		pstTrustOrderList->RoleTrust[pstTrustOrderList->RoleNum].TrustNum = 0;
	}

	pstTrustOrderList->RoleNum++;
	
	pstTrustSvr->TrustSvrType = TRUST_ORDER_LIST;
	Z_CSHEAD_INIT(&stPkg.Head, TRUST_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return ;
}
	
int trust_contribute_check(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstTrustPlayer, ROLETRUSTDATA *pstTrustHash, TRUSTTASK *pstTrustTask, TASKDEF *pstTaskDef, CSTRUSTCONTRIBUTE *pstTrustContribute)
{
	int i = 0, iVim = 0;
	int iTrustItemIdx;
	ITEMDEF *pstItemDef = NULL;
	UNUSED(pstTrustPlayer);

	if (pstTrustContribute->ItemNum ==0 || pstTrustContribute->ItemNum > MAX_TASK_ITEM)
	{
		return -1;
	}
	
	for( i = 0; i<  pstTrustContribute->ItemNum; i++)
	{
		pstItemDef = z_find_itemdef(pstEnv, pstTrustContribute->TrustItem[i].ItemID);
		if (!pstItemDef)
		{
			return -1;
		}
	
		//委托赠送道具
		iTrustItemIdx = get_taskiteam_idx(pstTaskDef, pstTrustContribute->TrustItem[i].ItemID);
		if (iTrustItemIdx < 0 || pstTrustTask->TrustItem[iTrustItemIdx].ItemID != pstTrustContribute->TrustItem[i].ItemID)
		{
			return -1;
		}

		if (pstTrustTask->TrustItem[iTrustItemIdx].Num >= pstTaskDef->GetItem[iTrustItemIdx].Num)
		{
			trust_onerole_update( pstEnv, pstPlayer, pstTrustHash, 0);
			return -1;
		}

		//实际赠送数量
		if ( (pstTrustContribute->TrustItem[i].Num +pstTrustTask->TrustItem[iTrustItemIdx].Num) >pstTaskDef->GetItem[iTrustItemIdx].Num )
		{
			pstTrustContribute->TrustItem[i].Num = pstTaskDef->GetItem[iTrustItemIdx].Num - pstTrustTask->TrustItem[iTrustItemIdx].Num;
		}

		if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstTrustContribute->TrustItem[i].ItemID, pstTrustContribute->TrustItem[i].Num, -1, NULL,PACKAGE_DEC_NONE))
		{
			//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "缺少赠送物品");
			return -1;
		}

		iVim += pstItemDef->TrustContributeVim *pstTrustContribute->TrustItem[i].Num;
	}

	//精力值检查
	if ( pstPlayer->stRoleData.RoleDetail.VIMCurr < iVim)
	{
		//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "赠送需要精力不足");
		return -1;
	}	

	return 0;
}

void trust_order_remove_update(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstOtherPlayer, ROLETRUSTDATA *pstTrustHash, TRUSTTASK *pstTrustTask)
{
	CSPKG stPkg;
	ROLETRUSTDATA stTrustHash;
	CSTRUSTSVR *pstTrustSvr = &stPkg.Body.TrustSvr;
	CSTRUSTORDERLIST *pstTrustOrderList = &pstTrustSvr->TrustSvrData.TrustOrderList;

	pstTrustOrderList->RoleNum = 0;
	
	if (pstTrustHash->TrustNum ==1 )
	{
		
		pstTrustOrderList->RoleTrust[pstTrustOrderList->RoleNum].RoleID = pstTrustHash->RoleID;
		pstTrustOrderList->RoleTrust[pstTrustOrderList->RoleNum].RoleTrustVersion = -1;
		pstTrustOrderList->RoleTrust[pstTrustOrderList->RoleNum].TrustNum = 0;
		pstTrustOrderList->RoleNum++;

		stTrustHash.RoleID = pstTrustHash->RoleID;
		sht_remove(pstEnv->pstShtTrust,&stTrustHash,TrustHashCmp, TrustHashCode);
	}
	else
	{
		pstTrustHash->RoleTrustVersion++;
		pstTrustHash->TrustNum--;
		
		if (pstTrustTask->TaskID != pstTrustHash->TrustTaskData[pstTrustHash->TrustNum].TaskID)
		{
			memmove(pstTrustTask, pstTrustTask+1, (pstTrustHash->TrustNum-(pstTrustTask-pstTrustHash->TrustTaskData))*sizeof(*pstTrustTask));
		}
		
		memcpy(&pstTrustOrderList->RoleTrust[pstTrustOrderList->RoleNum], pstTrustHash, sizeof(pstTrustOrderList->RoleTrust[0]));
		pstTrustOrderList->RoleNum++;
	}

	pstTrustSvr->TrustSvrType = TRUST_ORDER_LIST;
	Z_CSHEAD_INIT(&stPkg.Head, TRUST_SVR);
	
	if (pstOtherPlayer)
	{
		z_cltmsg_send(pstEnv, pstOtherPlayer, &stPkg);
	}
	
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return ;
}

int trust_order_check_fini(ZONESVRENV* pstEnv, Player *pstTrustPlayer, Player *pstOtherPlayer, ROLETRUSTDATA *pstTrustHash, TRUSTTASK *pstTrustTask)
{
	int i = 0;
	TASKDEF *pstTaskDef = NULL;
	
	pstTaskDef = task_def_find(pstEnv, pstTrustTask->TaskID, NULL);
	if (!pstTaskDef)
	{
		return 0;
	}

	for (i = 0; i < MAX_TASK_ITEM; i++)
	{
		if (pstTaskDef->GetItem[i].ItemID == 0 || pstTaskDef->GetItem[i].Num == 0)
		{
			break;
		}
		
		if (pstTaskDef->GetItem[i].Num != pstTrustTask->TrustItem[i].Num )
		{
			return 0;
		}
	}

	trust_order_remove_update(pstEnv, pstTrustPlayer, pstOtherPlayer, pstTrustHash, pstTrustTask);
	
	return 1;
}

void trust_attr_chg( PlayerChagAttrVal *pstChgVal,short nAttrID, short nFlag, unsigned int uiValue)
{
	AttrChgVal *pstVal;
	
	switch(nAttrID)
	{
		case ATTR_ID_EXP:
			pstVal = &pstChgVal->AttrList[pstChgVal->AttrNum++];
			pstVal->AttrID = nAttrID;
			pstVal->nFlag = nFlag;
			pstVal->Value.Exp = uiValue;
			break;
			
		case ATTR_ID_VIM:
			pstVal = &pstChgVal->AttrList[pstChgVal->AttrNum++];
			pstVal->AttrID = nAttrID;
			pstVal->nFlag = nFlag;
			pstVal->Value.Vim = uiValue;
			break;
			
		default:
			break;
	}
	
	return ;
}

int trust_buddyval_check(unsigned short unBuddyVal, unsigned char ucBuddyGrade)
{
	if ( ucBuddyGrade == 0)
	{
		return 0;
	}
	
	switch(ucBuddyGrade)
	{
		case BUDDY_GRADE_S:
			if (unBuddyVal < BUDDY_VALUE_S)
			{
				return -1;
			}
			break;

		case BUDDY_GRADE_A:
			if (unBuddyVal < BUDDY_VALUE_A)
			{
				return -1;
			}
			break;

		case BUDDY_GRADE_B:
			if (unBuddyVal < BUDDY_VALUE_B)
			{
				return -1;
			}
			break;

		case BUDDY_GRADE_C:
			if (unBuddyVal < BUDDY_VALUE_C)
			{
				return -1;
			}
			break;

		case BUDDY_GRADE_D:
			if (unBuddyVal < BUDDY_VALUE_D)
			{
				return -1;
			}
			break;

		default:
			return -1;
			break;
	}
	
	return 0;
}

int trust_send_mail(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstTrustPlayer, CSTRUSTCONTRIBUTE *pstTrustContribute)
{
	int i =0;
	MailFastSend stSendMail;

	bzero(&stSendMail, sizeof(stSendMail));
	
	STRNCPY(stSendMail.szTo, pstTrustPlayer->stRoleData.RoleName, CCH(stSendMail.szTo));
	STRNCPY(stSendMail.szTitle, "委托赠送", CCH(stSendMail.szTitle));

	for (i = 0; i< pstTrustContribute->ItemNum; i++)
	{
		stSendMail.astItems[stSendMail.iItemNum].cListType = -1;
		stSendMail.astItems[stSendMail.iItemNum].iDefID = pstTrustContribute->TrustItem[i].ItemID;
		stSendMail.astItems[stSendMail.iItemNum].iNum= pstTrustContribute->TrustItem[i].Num;
		stSendMail.iItemNum++;
	}

	/* 应该用player_sysmail_fast_send
	if (0 >player_mail_fast_send(pstEnv, pstPlayer, &stSendMail))
	{
		return -1;
	}*/

	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}

int trust_contribute(ZONESVRENV* pstEnv, Player *pstPlayer, CSTRUSTCONTRIBUTE *pstTrustContribute)
{
	int i = 0;
	char szMsg[SHORT_DESC_LEN], szContributeMSg[SHORT_DESC_LEN];
	int iTrustItemIdx = -1, iBuddyVal = 0, iVim = 0;
	tdr_longlong iPlayerExp = 0, iTrustExp = 0;
	Player *pstTrustPlayer = NULL ;
	PlayerChagAttrVal stChgVal;
	TASKDEF *pstTaskDef = NULL;
	ITEMDEF *pstItemDef = NULL;
	TRUSTTASK *pstTrustTask = NULL;
	LEVELUP *pstLevelUp = NULL,  *pstTrustLevelUp = NULL;
	ROLETRUSTDATA stTrustHash, *pstTrustHash = NULL;
	ROLEBUDDY *pstRoleBuddy = &pstPlayer->stRoleData.Buddy;
	ROLEBUDDYMEMBER *pstRoleBuddyMember = NULL, *pstTrustBuddyMember = NULL;	
	CSPKG stPkg;
	CSTRUSTSVR *pstTrustSvr = &stPkg.Body.TrustSvr;
	int iRet;
	
	bzero(szContributeMSg, sizeof(szContributeMSg));

	pstTaskDef = task_def_find(pstEnv, pstTrustContribute->TaskID, NULL);
	if (!pstTaskDef)
	{
		return -1;
	}

	stTrustHash.RoleID = pstTrustContribute->RoleID;
	pstTrustHash = sht_find(pstEnv->pstShtTrust, &stTrustHash, TrustHashCmp, TrustHashCode);
	if (NULL == pstTrustHash || pstTrustHash->RoleID != pstTrustContribute->RoleID)
	{
		trust_onerole_update( pstEnv, pstPlayer, NULL, pstTrustContribute->RoleID);
		//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "此好友无委托挂单");
		return -1;
	}

	pstTrustTask = find_trust_taskid(pstTrustHash, pstTrustContribute->TaskID);
	if (!pstTrustTask)
	{
		trust_onerole_update( pstEnv, pstPlayer, pstTrustHash, 0);
		//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "好友无此挂单");
		return -1;
	}

	pstRoleBuddyMember = relation_buddy_online_find(pstRoleBuddy,  pstTrustContribute->RoleID);
	if (!pstRoleBuddyMember)
	{
		//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "此委托单不是你的好友发布");
		return -1;
	}

	pstTrustPlayer = z_name_player( pstEnv->pstAppCtx, pstEnv, pstRoleBuddyMember->RoleName );
	if (!pstTrustPlayer)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "trust role %s not online", pstRoleBuddyMember->RoleName);
		return -1;
	}

	pstTrustBuddyMember = relation_buddy_online_find(&pstTrustPlayer->stRoleData.Buddy,  pstPlayer->stRoleData.RoleID);

	pstTrustLevelUp = z_get_levelup(pstEnv, pstTrustPlayer->stRoleData.Level);
	if (!pstTrustLevelUp)
	{
		return -1;
	}
	
	pstLevelUp = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	if (!pstLevelUp)
	{
		return -1;
	}

	if (0 > trust_contribute_check(pstEnv, pstPlayer, pstTrustPlayer, pstTrustHash, pstTrustTask, pstTaskDef, pstTrustContribute))
	{
		return -1;
	}
	
	for (i = 0; i < pstTrustContribute->ItemNum; i++)
	{
		pstItemDef = z_find_itemdef(pstEnv, pstTrustContribute->TrustItem[i].ItemID);
		if (!pstItemDef)
		{
			return -1;
		}

		if ( 0 > trust_buddyval_check(pstRoleBuddyMember->BuddyVal, pstItemDef ->ExchgLevel))
		{
				//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "好友度未达到流通要求");
				return -1;
		}

		iVim += pstTrustContribute->TrustItem[i].Num*pstItemDef->TrustContributeVim;
		iBuddyVal += pstItemDef->ContributeBuddyVal*pstTrustContribute->TrustItem[i].Num;
		iTrustExp += pstTrustLevelUp->ContributeExp*pstTrustContribute->TrustItem[i].Num*pstItemDef->GainExp/10000.0;
		iPlayerExp += pstLevelUp->ContributeExp*pstTrustContribute->TrustItem[i].Num*pstItemDef->GainExp/10000.0;

		snprintf(szMsg,sizeof(szMsg),"%s %d 个", pstItemDef->Name, pstTrustContribute->TrustItem[i].Num);
		strcat(szContributeMSg, szMsg);
	}

	iRet = z_player_addexp_test(pstEnv,pstPlayer, &iPlayerExp);
	if (iRet < 0)
	{
		/*
		if(iRet == -2)
		{
			snprintf(szMsg, sizeof(szMsg), "%s %s", pstPlayer->stRoleData.RoleName, 
					"您当前存储的经验值已经达到上限,请提升等级,否则无法继续获取经验");
		}
		else
		{
			snprintf(szMsg, sizeof(szMsg), "%s %s", 
					pstPlayer->stRoleData.RoleName, "每天经验获取达到上限");
		}

		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, szMsg);*/
		return -1;
	}

	iRet = z_player_addexp_test(pstEnv,pstTrustPlayer, &iTrustExp);
	if (iRet < 0)
	{
		/*
		if(iRet == -2)
		{
			snprintf(szMsg, sizeof(szMsg), "%s %s", pstTrustPlayer->stRoleData.RoleName, 
					"您当前存储的经验值已经达到上限,请提升等级,否则无法继续获取经验");
		}
		else
		{
			snprintf(szMsg, sizeof(szMsg), "%s %s", 
					pstTrustPlayer->stRoleData.RoleName, "每天经验获取达到上限");
		}

		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, szMsg);*/
		return -1;
	}

	if ( 0 > trust_send_mail(pstEnv, pstPlayer, pstTrustPlayer, pstTrustContribute))
	{
		return -1;
	}
	
	//赠送方友好度
	//relation_buddyval_change(pstEnv, pstPlayer, pstRoleBuddyMember, iBuddyVal);

	//挂单方友好度
	if (pstTrustBuddyMember)
	{
		//relation_buddyval_change(pstEnv, pstTrustPlayer, pstTrustBuddyMember, iBuddyVal);
	}
	
	//挂单方获得经验
	stChgVal.AttrNum = 0;
	trust_attr_chg(&stChgVal, ATTR_ID_EXP, 0, iTrustExp);
	z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstTrustPlayer, &stChgVal, 1);
	
	//赠送方获得经验
	stChgVal.AttrNum = 0;
	trust_attr_chg(&stChgVal, ATTR_ID_EXP, 0, iPlayerExp);

	//赠送方 减少精力
	trust_attr_chg(&stChgVal, ATTR_ID_VIM, 1, iVim);
	z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);

	for( i = 0; i<  pstTrustContribute->ItemNum; i++)
	{
		iTrustItemIdx = get_taskiteam_idx(pstTaskDef, pstTrustContribute->TrustItem[i].ItemID);
		if (iTrustItemIdx < 0 || pstTrustTask->TrustItem[iTrustItemIdx].ItemID != pstTrustContribute->TrustItem[i].ItemID)
		{
			snprintf(szMsg, sizeof(szMsg), "trust contribute Item %d iTrustItemIdx %d TrustItem %d error", pstTrustContribute->TrustItem[i].ItemID, iTrustItemIdx, pstTrustTask->TrustItem[iTrustItemIdx].ItemID);
			tlog_error(pstEnv->pstLogCat, 0, 0, "trust contribute   ItemIdx error");
			return -1;
		}
		
		pstTrustTask->TrustItem[iTrustItemIdx].Num += pstTrustContribute->TrustItem[i].Num;
	}
	
	if(0 == trust_order_check_fini(pstEnv, pstTrustPlayer, pstPlayer, pstTrustHash, pstTrustTask))
	{
		trust_onerole_update(pstEnv, pstPlayer, pstTrustHash, 0);
		trust_onerole_update(pstEnv, pstTrustPlayer, pstTrustHash, 0);
	}
	else
	{
		sprintf(szMsg, "委托%s 挂单已完成", pstTaskDef->Name);
		//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstTrustPlayer, SYS_MIDDLE2, szMsg);
	}
	
	pstTrustSvr->TrustSvrType = TRUST_CONTRIBUTE_MSG;
	Z_CSHEAD_INIT(&stPkg.Head, TRUST_SVR);
	
	//挂单方消息通知
	sprintf(&pstTrustSvr->TrustSvrData.TrustContributeMsg[0], "%s 成功将%s 赠送给你", pstPlayer->stRoleData.RoleName, szContributeMSg);
	z_cltmsg_send(pstEnv, pstTrustPlayer, &stPkg);

	//赠送方消息通知
	sprintf(&pstTrustSvr->TrustSvrData.TrustContributeMsg[0], "你已成功将%s  赠送给%s ", szContributeMSg, pstTrustPlayer->stRoleData.RoleName);	
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int trust_cancel_order(ZONESVRENV* pstEnv, Player *pstPlayer, unsigned int  uiTaskID)
{
	TRUSTTASK *pstTrustTask = NULL;
	ROLETRUSTDATA stTrustHash, *pstTrustHash = NULL;

	stTrustHash.RoleID = pstPlayer->stRoleData.RoleID;

	pstTrustHash = sht_find(pstEnv->pstShtTrust, &stTrustHash, TrustHashCmp, TrustHashCode);
	if (NULL == pstTrustHash || pstTrustHash->RoleID != pstPlayer->stRoleData.RoleID)
	{
		//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "委托任务未曾挂单");
		return -1;
	}

	
	pstTrustTask = find_trust_taskid(pstTrustHash, uiTaskID);
	if (!pstTrustTask)
	{
		//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "委托任务未曾挂单");
		return -1;
	}
	else
	{
		trust_order_remove_update(pstEnv, pstPlayer, NULL, pstTrustHash, pstTrustTask);
	}

	return 0;	
}

int trust_order_check(ZONESVRENV* pstEnv, Player *pstPlayer, TASKDEF *pstTaskDef)
{
	int i = 0, iVim = 0;
	ITEMDEF *pstItemDef = NULL;
	
	if (0 == trust_check_fini(pstEnv, pstPlayer, pstTaskDef, 0))
	{
		return -1;
	}

	//精力消耗check
	for (i =0; i< MAX_TASK_ITEM; i++)
	{
		if (pstTaskDef->GetItem[i].ItemID == 0 || pstTaskDef->GetItem[i].Num == 0)
		{
			break;
		}
		
		pstItemDef = z_find_itemdef(pstEnv, pstTaskDef->GetItem[i].ItemID);
		if (!pstItemDef)
		{
			return -1;
		}
		iVim += pstItemDef->TrustOrderVim*pstTaskDef->GetItem[i].Num;	
	}

	if (iVim > pstPlayer->stRoleData.RoleDetail.VIMCurr)
	{
		//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "精力值不足无法挂单");
		return -1;
	}
	
	return 0;
}

int trust_order(ZONESVRENV* pstEnv, Player *pstPlayer, unsigned int  uiTaskID)
{
	int i = 0, iVim = 0;
	PlayerChagAttrVal stChgVal;
	ITEMDEF *pstItemDef = NULL;	
	ROLETRUSTDATA stTrustHash, *pstTrustHash = NULL;
	TASKDEF *pstTaskDef = NULL;
	TRUSTTASK *pstTrustTask  = NULL;

	pstTaskDef = task_def_find(pstEnv, uiTaskID, NULL);
	if (!pstTaskDef)
	{
		return -1;
	}

	if (0 > trust_order_check(pstEnv, pstPlayer, pstTaskDef))
	{
		return -1;
	}
	
	stTrustHash.RoleID = pstPlayer->stRoleData.RoleID;
	
	pstTrustHash = sht_find(pstEnv->pstShtTrust, &stTrustHash, TrustHashCmp, TrustHashCode);
	if (NULL == pstTrustHash )
	{
		pstTrustHash = sht_insert_unique( pstEnv->pstShtTrust, &stTrustHash, TrustHashCmp, TrustHashCode );
		if(NULL == pstTrustHash)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "trust hash error");
			return -1;
		}
		memset(pstTrustHash, 0, sizeof(*pstTrustHash));
		pstTrustHash->RoleID = pstPlayer->stRoleData.RoleID;
	}
	else
	{
		if (pstTrustHash->TrustNum >= MAX_TRUST_ORDER)
		{
			//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "委托挂单到最大限制");
			return -1;
		}

		for (i = 0; i< pstTrustHash->TrustNum; i++)
		{
			if (pstTrustHash->TrustTaskData[i].TaskID == uiTaskID)
			{
				//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "此委托已挂单");
				return -1;
			}
		}
	}

	//精力消耗
	for (i =0; i< MAX_TASK_ITEM; i++)
	{
		if (pstTaskDef->GetItem[i].ItemID == 0 || pstTaskDef->GetItem[i].Num == 0)
		{
			break;
		}
		
		pstItemDef = z_find_itemdef(pstEnv, pstTaskDef->GetItem[i].ItemID);
		if (!pstItemDef)
		{
			return -1;
		}
		iVim += pstItemDef->TrustOrderVim*pstTaskDef->GetItem[i].Num;	
	}

	stChgVal.AttrNum = 0;
	trust_attr_chg(&stChgVal, ATTR_ID_VIM, 1, iVim);
	z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);

	
	pstTrustHash->RoleTrustVersion++;
	pstTrustTask = &pstTrustHash->TrustTaskData[pstTrustHash->TrustNum];
	pstTrustTask->TaskID = uiTaskID;
	pstTrustTask->ItemNum = 0;
	
	while(pstTaskDef->GetItem[pstTrustTask->ItemNum].ItemID)
	{
		pstTrustTask->TrustItem[pstTrustTask->ItemNum].ItemID = pstTaskDef->GetItem[pstTrustTask->ItemNum].ItemID;
		pstTrustTask->TrustItem[pstTrustTask->ItemNum].Num = 0;
		pstTrustTask->ItemNum++;

		if (pstTrustTask->ItemNum == MAX_TASK_ITEM)
		{
			break;
		}
	}
	pstTrustHash->TrustNum++;

	trust_onerole_update(pstEnv, pstPlayer, pstTrustHash, 0);
	
	return 0;
}

int trust_award_order_update(ZONESVRENV* pstEnv, Player *pstPlayer, unsigned int uiTaskID)
{
	TRUSTTASK *pstTrustTask = NULL;
	ROLETRUSTDATA stTrustHash, *pstTrustHash = NULL;
	
	stTrustHash.RoleID = pstPlayer->stRoleData.RoleID;
	pstTrustHash = sht_find(pstEnv->pstShtTrust, &stTrustHash, TrustHashCmp, TrustHashCode);
	if (pstTrustHash)
	{
		pstTrustTask = find_trust_taskid(pstTrustHash, uiTaskID);
		if (pstTrustTask)
		{
			trust_order_remove_update(pstEnv, pstPlayer,  NULL, pstTrustHash, pstTrustTask);
		}
	}

	return 0;	
}

int player_trust(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSTRUSTCLT *pstTrustClt = &pstCsPkg->Body.TrustClt;
	CSTRUSTROLELIST *pstTrustRoleList =  &pstTrustClt->TrustCltData.TrustRoleList;
	CSTRUSTCONTRIBUTE *pstTrustContribute = &pstTrustClt->TrustCltData.TrustContribute;
	UNUSED(pstAppCtx);
	UNUSED(pstFrameHead);
	
	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,"战场不允许操作委托");
		return -1;	
	}
	switch(pstTrustClt->TrustCltType)
	{
		case TRUST_OPEN_FACE:
			return trust_open_face(pstEnv, pstPlayer);
			break;

		case TRUST_ROLE_ORDER:
			return trust_role_vercmp( pstEnv, pstPlayer, pstTrustRoleList);
			break;

		case TRUST_CONTRIBUTE:
			return trust_contribute( pstEnv, pstPlayer, pstTrustContribute);
			break;

		case TRUST_ORDER:
			return trust_order(pstEnv, pstPlayer, pstTrustClt->TrustCltData.OrderTrustID);
			break;

		case TRUST_CANCEL_ORDER:
			return trust_cancel_order( pstEnv, pstPlayer, pstTrustClt->TrustCltData.CancelTrustID);
			break;

		default:
			return -1;
			break;
	}

	return 0;	
}

int task_share_check(ZONESVRENV *pstEnv, TASKDEF *pstTaskDef, Player *pstPlayer,
                     Player *pstAcceptor, int fCheckDist)
{
	int iRet = -1;

	if(!pstAcceptor || pstAcceptor == pstPlayer)
	{
		iRet = -1;
		goto final;
	}
	
	if(fCheckDist &&
	   z_check_op_dist(pstEnv, pstPlayer, pstAcceptor->stRoleData.Map,
		               &pstAcceptor->stRoleData.Pos,
		               TASK_SHARING_ACCEPT_DIST) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK219, pstAcceptor->stRoleData.RoleName);
		goto disallow;
	}

	if(pstAcceptor->stOnline.iSharingTaskID &&
	   pstAcceptor->stOnline.tSharingTime - pstEnv->pstAppCtx->stCurr.tv_sec 
	   < TASK_SHARING_ACCEPT_TIME)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK220, pstAcceptor->stRoleData.RoleName);
		goto disallow;
	}

	iRet = task_accept_check(pstEnv, pstAcceptor, pstTaskDef, 1, 0);
	if(iRet < 0)
	{
		//char szAcceptorMsg[CLT_MSG_LEN];
		switch(-iRet)
		{
		case TAE_LEVEL_LIMIT:
		case TAE_LEVEL_ONE:
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK221, pstAcceptor->stRoleData.RoleName);
			break;

		case TAE_REPEATED:
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK222, pstAcceptor->stRoleData.RoleName);
			break;

		case TAE_FINI_PER_DAY:
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK223, pstAcceptor->stRoleData.RoleName);
			break;

		case TAE_CLAN_LIMIT:
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK224, pstAcceptor->stRoleData.RoleName);
			break;

		case TAE_NEED_PRETASK:
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK225, pstAcceptor->stRoleData.RoleName);
			break;
		case TAE_BIANSHEN_LIMIT:
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK226, pstAcceptor->stRoleData.RoleName);
			break;
		case TAE_TASK_REDO:
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK227, pstAcceptor->stRoleData.RoleName);
			break;

		default:
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK228, pstAcceptor->stRoleData.RoleName);
			break;
		}

		//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstAcceptor, SYS_MIDDLE2,
		//               szAcceptorMsg);
		goto disallow;
	}
	/*
	if(task_accept_check(pstEnv, pstAcceptor, pstTaskDef, 1, 0) < 0)
	{
		goto disallow;
	}
	*/
	
	pstAcceptor->stOnline.iSharingTaskID = pstTaskDef->ID;
	pstAcceptor->stOnline.tSharingTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstAcceptor->stOnline.iSharingOwner = pstPlayer->iIdx;
	iRet = 0;

final:
	return iRet;
	
disallow:
	
	goto final;
	return 0;
}

int player_task_share(ZONESVRENV *pstEnv, Player *pstPlayer, int iNpcID,
                      int iTaskID)
{
	int i, iCount = 0;
	int fNoPworld = 0;
	const TEAMMEMBER *pstMember = NULL;
	Player *apstAcceptors[MAX_TEAM_MEMBER];
	TASKDEF *pstTaskDef = task_def_find(pstEnv, iTaskID, NULL);
	TASKRUNONE *pstRunOne = task_run_find(pstPlayer, iTaskID);
	ZoneTeam *pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	Player *pstAcceptor;
	
	if (!pstTaskDef || !pstRunOne)
	{
		return -1;
	}
	
	if(pstEnv->iInstMode == INST_MODE_ZONE_SVR &&
	   (pstPlayer->stOnline.eStatusAssistant&PLAYER_STATUS_BATTLE_TEAM_SYNC) )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
		               ZONEERR_TASK229);
		return -1;
	}
	
	if(pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
		               ZONEERR_TASK230);
		return -1;
	}

	if(!pstTaskDef->CanShare)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
		               ZONEERR_TASK231);
		return -1;
	}

	if(pstRunOne->TaskStat == TASK_STAT_FAIL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
		               ZONEERR_TASK232);
		return -1;
	}

	if(pstRunOne->TaskStat == TASK_STAT_FINI)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
		               ZONEERR_TASK233);
		return -1;
	}

	if (!pstZoneTeam)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK234);
		return -1;
	}

	fNoPworld = 0;
	//不在副本里要检查距离
	if(pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
	{
		fNoPworld = 1;
	}

	pstMember = pstZoneTeam->stTeamInfo.TeamMember;
	//i = pstZoneTeam->stTeamInfo.MemberNum;

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstMember = &pstZoneTeam->stTeamInfo.TeamMember[i];
		pstAcceptor =  z_name_player(pstEnv->pstAppCtx, pstEnv,
		                                    			pstMember->MemberName);
		if (!pstAcceptor)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
			           ZONEERR_TASK250, pstMember->MemberName);
			continue;
		}

		if (pstMember->Follow > 0)
		{
			if (task_accept(pstEnv, pstAcceptor,pstTaskDef->ID) < 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,ZONEERR_TASK251, 
								pstAcceptor->stRoleData.RoleName);
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,ZONEERR_TASK252, 
								pstAcceptor->stRoleData.RoleName);
			}
		}
		else
		{
			if(task_share_check(pstEnv, pstTaskDef, pstPlayer, pstAcceptor,fNoPworld) == 0)
			{
				apstAcceptors[iCount++] = pstAcceptor;
			}
		}
	}

	/*while(i--)
	{
		Player *pstAcceptor = !pstMember->OffLineTime ?
		                      z_name_player(pstEnv->pstAppCtx, pstEnv,
		                                    pstMember->MemberName) : NULL;
		if (!pstAcceptor)
		{
			z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2,
			           "%s不在游戏中，无法共享任务", pstMember->MemberName);
			pstMember++;
			continue;
		}
		pstMember++;

		//不在一个地图,也就是不在一个副本
		if(pstAcceptor->stRoleData.Map != pstPlayer->stRoleData.Map)
		{
			continue;
		}
		
		if(task_share_check(pstEnv, pstTaskDef, pstPlayer, pstAcceptor,
		                    fNoPworld) == 0)
		{
			if ()
			{

			}
			apstAcceptors[iCount++] = pstAcceptor;
		}
	}*/
	
	if(iCount > 0)
	{
		CSPKG stPkg;
		CSTASKSVR *pstTaskSvr = &stPkg.Body.TaskSvr;
	
		pstTaskSvr->OpType = TASK_DIALOG;
		pstTaskSvr->TaskSvrData.TaskDialog.NpcID = iNpcID;
		pstTaskSvr->TaskSvrData.TaskDialog.TaskID = iTaskID;
		pstTaskSvr->TaskSvrData.TaskDialog.Type = TASK_DIALOG_ACCEPT;
		pstTaskSvr->TaskSvrData.TaskDialog.CycleID = 0;
	
		Z_CSHEAD_INIT(&stPkg.Head, TASK_SVR);
		return z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv,
		                                apstAcceptors, iCount, &stPkg);
	}
	return 0;
}

int player_task_accept(ZONESVRENV *pstEnv, Player *pstPlayer,
                       CSTASKACCEPT *pstTaskAccept)
{
	TASKDEF *pstDef = task_def_find(pstEnv, pstTaskAccept->TaskID, NULL);

	if (!pstDef)
	{
		return -1;
	}
	
	// 接受的是共享任务
	if((unsigned)(pstPlayer->stOnline.iSharingTaskID) == pstTaskAccept->TaskID)
	{
		
		int iTime = pstPlayer->stOnline.tSharingTime;
		Player *pstOwner = tmempool_get(pstEnv->pstPlayerPool,
		                                pstPlayer->stOnline.iSharingOwner);
		
		pstPlayer->stOnline.tSharingTime = 0;
		pstPlayer->stOnline.iSharingTaskID = 0;
		
		if(pstEnv->pstAppCtx->stCurr.tv_sec - iTime >= TASK_SHARING_ACCEPT_TIME)
		{
			// 过了有效时间
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK253);
			return -1;
		}


		if(is_convoy_task(pstDef))
		{
			if(NULL == pstOwner ||pstOwner->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK253);
				return -1;
			}
		}

		// 接受共享任务
		if(task_accept(pstEnv, pstPlayer, pstTaskAccept->TaskID) < 0)
		{
			return -1;
		}

		if(pstOwner)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK254,
			           pstOwner->stRoleData.RoleName, pstDef->Name);
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOwner, SYS_MIDDLE2, ZONEERR_TASK255,
			           pstPlayer->stRoleData.RoleName, pstDef->Name);
		}
		return 0;
	}


	// 接受的是非共享任务,并且不是今日目标任务
	if(pstDef->Type != TASK_TYPE_DAILY_GOAL && 
		0 > task_check_npc(pstEnv, pstPlayer, pstTaskAccept->NpcID,
	                      pstTaskAccept->TaskID, 0))
	{
		return -1;
	}

	if(0 == task_accept(pstEnv, pstPlayer, pstTaskAccept->TaskID))
	{
		Npc *pstNpc = NULL;

		pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstTaskAccept->NpcID);
		if(pstNpc)
		{
			if(NPC_SCRIPT_PROC_ACCEPT_TASK_MAP & pstNpc->cScriptFlag)
			{
				pstPlayer->stOnline.iDialogID = pstNpc->stMon.iID;
				
				stSEnv.pstMonster = NULL;
				stSEnv.pstNpc = pstNpc;
				stSEnv.pstPlayer = pstPlayer;
				stSEnv.pstPworldInst = NULL;
				stSEnv.iTaskCurr = pstTaskAccept->TaskID;
				stSEnv.pstMapinst = NULL;
				stSEnv.pstKiller = NULL;

				z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer,
				              pstNpc->szScriptName, &pstNpc->stScriptIdx,
				              NPC_SCRIPT_PROC_ACCEPT_TASK);
			}
			task_dialog_again(pstEnv, pstPlayer, pstNpc);
		}
	}
	return 0;
}

static int player_task_refuse(ZONESVRENV *pstEnv, Player *pstPlayer,
                              int iTaskID)
{
	Player *pstOwner = NULL;

	if(iTaskID != pstPlayer->stOnline.iSharingTaskID /*||
	   pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.tSharingTime >=
	   TASK_SHARING_ACCEPT_TIME*/)
	{
		return -1;
	}

	pstPlayer->stOnline.iSharingTaskID = 0;
	pstPlayer->stOnline.tSharingTime = 0;
	pstPlayer->stOnline.iSharingOwner = 0;

	pstOwner = tmempool_get(pstEnv->pstPlayerPool,
	                        pstPlayer->stOnline.iSharingOwner);
	if(pstOwner)
	{
		TASKDEF *pstDef = task_def_find(pstEnv, iTaskID, NULL);
		if(!pstDef)
		{
			return -1;
		}
		
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOwner, SYS_MIDDLE2, ZONEERR_TASK256,
		           pstPlayer->stRoleData.RoleName, pstDef->Name);
	}
	return 0;
}

int player_task_recycle_item_open(ZONESVRENV* pstEnv, Player *pstPlayer, CSTASKITEMRECYCLEOPEN *pstCSOpen)
{
	int iPos, iTaskID;
	ITEMDEF *pstItemDef;
	ROLEITEM *pstRoleItem;
	RESULT *pstResult;
	TASKRUNONE *pstTaskRunOne;
	TASKDEF *pstTaskDef;
	TASKRECYCLEDEF * pstTaskRecycleDef;
	TASKRECYCLEONE *pstTaskRecycleOne;
	CSPKG stPkg;
	CSTASKSVR *pstCSTaskSvr = &stPkg.Body.TaskSvr;
	
	if (0 > package_get_item(pstEnv, pstPlayer, pstCSOpen->ListType, 
							pstCSOpen->GridIdx, &pstItemDef, &pstRoleItem, &iPos))
	{
		return -1;
	}

	pstResult = z_item_get_result(pstItemDef, RESULT_ITEM_TASK_RECYCLE);
	if(!pstResult)
	{
		return -1;
	}

	pstTaskRecycleDef = taskcycle_def_find(pstEnv, pstResult->ResultVal1, NULL);
	if(!pstTaskRecycleDef)
	{
		return -1;
	}

	if (0 > taskcycle_accept_check(pstEnv, pstPlayer, pstTaskRecycleDef, 1))	
	{
		return -1;
	}

	pstTaskRecycleOne = taskcycle_run_find(pstPlayer, pstResult->ResultVal1);	
	if (pstTaskRecycleOne)
	{
		iTaskID = pstTaskRecycleOne->TaskID;
		pstTaskDef = task_def_find(pstEnv, iTaskID, NULL);
		pstTaskRunOne = task_run_find(pstPlayer, iTaskID);
		if (pstTaskDef == NULL || (NULL == pstTaskRunOne && 0 > task_accept_check_matchclient(pstEnv, pstPlayer, pstTaskDef, 0)))	
		{
			if (pstTaskRunOne)
			{
				pstTaskRunOne->TaskStat = TASK_STAT_COMMIT;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				
				task_run_del(pstEnv, pstPlayer, iTaskID);
			}
			
			pstTaskRecycleOne = NULL; 
			taskcycle_run_del(pstPlayer, pstResult->ResultVal1);
		}
	}

	if (pstTaskRecycleOne == NULL)
	{
		iTaskID = taskcycle_new(pstEnv, pstPlayer, pstTaskRecycleDef);
		if (iTaskID < 0 )
		{
			return -1;
		}
	}
	else
	{
		iTaskID = pstTaskRecycleOne->TaskID;
		pstTaskRunOne = task_run_find(pstPlayer, iTaskID);
		if (pstTaskRunOne)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK802, pstTaskRecycleDef->Name);
			return -1;
		}
		
		if (0 == pstResult->ResultVal2 && pstTaskRecycleOne->TaskRecycleIdx != 0)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK803);
			return -1;
		}
	}
	
	pstCSTaskSvr->OpType = TASK_ITEM_RECYCLE_UI;
	pstCSTaskSvr->TaskSvrData.ItemRecycleUI.TaskRecycleID = pstTaskRecycleDef->ID;
	pstCSTaskSvr->TaskSvrData.ItemRecycleUI.TaskID = iTaskID;
	pstCSTaskSvr->TaskSvrData.ItemRecycleUI.ListType = pstCSOpen->ListType;
	pstCSTaskSvr->TaskSvrData.ItemRecycleUI.GridIdx = pstCSOpen->GridIdx;
	Z_CSHEAD_INIT(&stPkg.Head, TASK_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_task_recycle_item_accept(ZONESVRENV* pstEnv, Player *pstPlayer, CSTASKITEMRECYCLEACCEPT *pstCSAccept)
{
	int iPos;
	ITEMDEF *pstItemDef;
	ROLEITEM *pstRoleItem;
	RESULT *pstResult;
	TASKRECYCLEONE *pstTaskRecycleOne;
	TASKRECYCLEDEF * pstTaskRecycleDef;
	TASKRUNONE *pstTaskRunOne;
	
	if (0 > package_get_item(pstEnv, pstPlayer, pstCSAccept->ListType, 
							pstCSAccept->GridIdx, &pstItemDef, &pstRoleItem, &iPos))
	{
		return -1;
	}

	pstResult = z_item_get_result(pstItemDef, RESULT_ITEM_TASK_RECYCLE);
	if(!pstResult)
	{
		return -1;
	}

	pstTaskRecycleDef = taskcycle_def_find(pstEnv, pstResult->ResultVal1, NULL);
	if(!pstTaskRecycleDef)
	{
		return -1;
	}

	pstTaskRecycleOne = taskcycle_run_find(pstPlayer, pstResult->ResultVal1);	
	if (!pstTaskRecycleOne || pstTaskRecycleOne->TaskID <= 0)
	{
		return -1;
	}

	pstTaskRunOne = task_run_find(pstPlayer, pstTaskRecycleOne->TaskID);
	if (pstTaskRunOne)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TASK802, pstTaskRecycleDef->Name);
		return -1;
	}

	if(pstCSAccept->TaskID != pstTaskRecycleOne->TaskID)
	{
		return -1;
	}

	if(0 > task_accept(pstEnv, pstPlayer, pstTaskRecycleOne->TaskID))
	{
		return -1;
	}

	if (0 == pstResult->ResultVal2)
	{
		if (0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItemDef->ItemID, 1, 
			pstCSAccept->ListType, &iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_TASK_RECYCLE))
		{
			return -1;
		}
	}
	
	return 0;
}

int player_task(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSTASKCLT *pstTaskClt = &pstCsPkg->Body.TaskClt;
	CSTASKAWARD *pstTaskAward = &pstTaskClt->TaskCltData.GetTaskAward;
	CSTASKACCEPT *pstTaskAccept = &pstTaskClt->TaskCltData.TaskAccept;
	CSTASKDROP *pstTaskDrop = &pstTaskClt->TaskCltData.TaskDrop;
	CSTASKFINI *pstTaskFini = &pstTaskClt->TaskCltData.TaskFini;
	TASKDEF *pstTaskDef;
	
	UNUSED(pstAppCtx);
	UNUSED(pstFrameHead);

	switch (pstTaskClt->OpType)
	{
	case TASK_INFO:
		//避免下行包量集中，服务器安排下发
		//task_info_get(pstEnv, pstPlayer);
		break;
		
	case TASK_ACCEPT:
		player_task_accept(pstEnv, pstPlayer, pstTaskAccept);
		break;
	
	case TASK_AWARD:
		if(CS_STAT_DEAD & pstPlayer->stOnline.State)
		{
			return -1;
		}
		pstTaskDef = task_def_find(pstEnv, pstTaskAward->TaskID, NULL);
		if (NULL == pstTaskDef)
		{
			return -1;
		}

		if (pstTaskDef->Type == TASK_TYPE_DART_CAR)
		{
			pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_EXP_NOLEVEL_LIMIT;
		}
		
		task_award(pstEnv, pstPlayer, pstTaskAward);
		
		if (pstTaskDef->Type == TASK_TYPE_DART_CAR)
		{
			pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_EXP_NOLEVEL_LIMIT;
		}
		break;

	case TASK_DROP:
		task_drop(pstEnv, pstPlayer, pstTaskDrop->TaskID);
		break;

	case TASK_FINI:
		task_fini(pstEnv, pstPlayer, pstTaskFini->TaskID, 0);
		break;

	case TASK_SHARE:
		player_task_share(pstEnv, pstPlayer, 0,
		                  pstTaskClt->TaskCltData.TaskShare.TaskID);
		break;
	case TASK_REFUSE:
		player_task_refuse(pstEnv, pstPlayer,
		                   pstTaskClt->TaskCltData.TaskRefuse.TaskID);
		break;
	case TASK_ITEM_RECYCLE_OPEN:
		player_task_recycle_item_open(pstEnv, pstPlayer, &pstTaskClt->TaskCltData.ItemRecycleOpen);
		break;
	case TASK_ITEM_RECYCLE_ACCEPT:
		player_task_recycle_item_accept(pstEnv, pstPlayer, &pstTaskClt->TaskCltData.ItemRecycleAccept);
		break;
	default:
		return -1;
		break;
	
	}
	
	return 0;
}

int task_chg(ZONESVRENV *pstEnv, Player *pstPlayer, TASKRUNONE *pstTaskRunOne)
{
	CSPKG stPkg;
	CSTASKSVR *pstTaskSvr = &stPkg.Body.TaskSvr;

	//任务失败回调脚本
	if(pstTaskRunOne->TaskStat == TASK_STAT_FAIL)
	{
		TASKDEF  *pstTaskDef;

		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if(pstTaskDef && (pstTaskDef->TaskScript & TASK_SCRIPT_PROC_FAIL_MAP))
		{	
			ScriptIndex *pstScriptInd;
			char szScript[32];	
		
			stSEnv.pstMonster = NULL;
			stSEnv.pstNpc = NULL;
			stSEnv.pstPlayer = pstPlayer;
			stSEnv.pstMapinst = NULL;
			stSEnv.pstKiller = NULL;
			
			snprintf(szScript, sizeof(szScript), "task%d.mac", pstTaskDef->ID);				
			pstScriptInd = get_script_ind(szScript);

			if ( !pstScriptInd )
			{
				printf("get_script_ind fail\n");
				tlog_error(pstEnv->pstLogCat, 0, 0, "get_script_ind fail");
				return -1;
			}
			
			z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, szScript, pstScriptInd, TASK_SCRIPT_PROC_FAIL);
		}
		
		if (pstTaskDef && pstTaskDef->TaskBuff.TaskBuffID > 0)
		{
			z_player_del_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstTaskDef->TaskBuff.TaskBuffID, 1, 0);
		}

		if (pstTaskDef && pstTaskDef->TaskItem > 0 && pstTaskDef->TaskItemNum > 0)
		{
			package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer,  pstTaskDef->TaskItem, 
									pstTaskDef->TaskItemNum, -1, NULL,PACKAGE_DEC_NONE, 0);
		}

		//任务日志相关流水号
		z_oplog_event_begin(pstEnv);
		z_role_task_oplog( pstEnv, pstPlayer, pstTaskRunOne->TaskID, 
				OP_TASK_FAILED,NULL,0,0,0,0, 0);
	}

	//未发送任务INFO信息前，不通知客户端任务的变化。
	if(0 == (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_TASK_ALL))
	{
		return 0;
	}

	if (pstTaskRunOne->TaskStat == TASK_STAT_FINI)
	{
		//刺探任务
		if (pstTaskRunOne->TaskID == CLAN_SPY_TASK_ID)
		{
			pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_SPY_FINI;
		}

		if (pstTaskRunOne->TaskID == CLAN_ANTI_SPY_TASK_ID)
		{
			pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_ANTI_SPY_FINI;
		}
	}
	else if (pstTaskRunOne->TaskStat == TASK_STAT_DROP ||
				pstTaskRunOne->TaskStat == TASK_STAT_FAIL)
	{
		//刺探任务
		if (pstTaskRunOne->TaskID == CLAN_SPY_TASK_ID)
		{
			pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_SPY_FINI;
			pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_SPY;
			
			player_action_view_flag_chg(pstEnv,pstPlayer);
		}

		if (pstTaskRunOne->TaskID == CLAN_ANTI_SPY_TASK_ID)
		{
			pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_ANTI_SPY_FINI;
			pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_ANTI_SPY;

			player_action_view_flag_chg(pstEnv,pstPlayer);
		}
	}
	
	pstTaskSvr->OpType = TASK_CHG;

	task_one_csinfo(pstEnv, pstPlayer, &pstTaskSvr->TaskSvrData.GetTaskChg, pstTaskRunOne);
	
	Z_CSHEAD_INIT(&stPkg.Head, TASK_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int task_tick(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i;
	TASKDEF *pstTaskDef;
	TASKINFO *pstTaskInfo = &pstPlayer->stRoleData.MiscInfo.TaskInfo;
	TASKRUN *pstTaskRun = &pstTaskInfo->TaskRun;
//	TASKRECYCLE *pstTaskCycle = &pstTaskInfo->TaskRecycle;
//	TASKONEDAY *pstTaskOneDay = &pstTaskInfo->TaskOneDay;
//	TASKCYCLEONEDAY *pstTaskCycleOneDay = &pstTaskInfo->TaskCycleOneDay;
//	GROUPONEDAY *pstGroupOneDay = &pstTaskInfo->GroupOneDay;

	for (i=pstTaskRun->TaskRunNum-1; i>=0; i--)
	{
		if (pstTaskRun->TaskRuns[i].TaskStat == TASK_STAT_FAIL)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRun->TaskRuns[i].TaskID, &pstTaskRun->TaskRuns[i].TaskIdx);
		if(NULL == pstTaskDef)
		{
			continue;
		}

		if(pstTaskRun->TaskRuns[i].TaskStat == TASK_STAT_FINI)
		{
			//道具追踪，防止丢弃物品，状态未改变
			if(pstTaskRun->TaskRuns[i].TaskItemTrace.TaskItemTraceNum > 0)
			{
				task_check_fini(pstEnv, pstPlayer, &pstTaskRun->TaskRuns[i], pstTaskDef);
				if(pstTaskRun->TaskRuns[i].TaskStat != TASK_STAT_FINI)
				{
					task_chg(pstEnv, pstPlayer, &pstTaskRun->TaskRuns[i]);
					continue;
				}	
			}
		}
		
		//任务脚本
		if ((pstTaskDef->TaskScript & TASK_SCRIPT_PROC_TIMER_MAP) &&  pstTaskRun->TaskRuns[i].TaskStat == TASK_STAT_RUN)
		{	
			ScriptIndex *pstScriptInd;
			char szScript[32];	

			if (task_check_fini(pstEnv, pstPlayer, &pstTaskRun->TaskRuns[i], pstTaskDef))
			{
				if(pstTaskRun->TaskRuns[i].TaskStat != TASK_STAT_RUN)
				{
					task_chg(pstEnv, pstPlayer, &pstTaskRun->TaskRuns[i]);
				}
				continue;
			}

			stSEnv.pstMonster = NULL;
			stSEnv.pstNpc = NULL;
			stSEnv.pstPlayer = pstPlayer;
			stSEnv.pstMapinst = NULL;
			stSEnv.pstKiller = NULL;
			
			snprintf(szScript, sizeof(szScript), "task%d.mac", pstTaskDef->ID);				
			pstScriptInd = get_script_ind(szScript);

			if ( !pstScriptInd )
			{
				printf("get_script_ind fail\n");
				tlog_error(pstEnv->pstLogCat, 0, 0, "get_script_ind fail");
				return -1;
			}
			
			z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, szScript, pstScriptInd, TASK_SCRIPT_PROC_TIMER);
		}

		//定时脚本
		if (pstTaskDef->TaskScript & TASK_SCRIPT_PROC_TIMER2_MAP)
		{	
			ScriptIndex *pstScriptInd;
			char szScript[32];	

			stSEnv.pstMonster = NULL;
			stSEnv.pstNpc = NULL;
			stSEnv.pstPlayer = pstPlayer;
			stSEnv.pstMapinst = NULL;
			stSEnv.pstKiller = NULL;
			
			snprintf(szScript, sizeof(szScript), "task%d.mac", pstTaskDef->ID);				
			pstScriptInd = get_script_ind(szScript);

			if ( !pstScriptInd )
			{
				printf("get_script_ind fail\n");
				tlog_error(pstEnv->pstLogCat, 0, 0, "get_script_ind fail");
				return -1;
			}
			
			z_exec_script(pstEnv->pstAppCtx, pstEnv, pstPlayer, szScript, pstScriptInd, TASK_SCRIPT_PROC_TIMER2);
		}
		
		if (pstTaskDef->TaskTimer > 0 && (int)(pstTaskRun->TaskRuns[i].TaskGetTime+pstTaskDef->TaskTimer) < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			if (task_check_fini(pstEnv, pstPlayer, &pstTaskRun->TaskRuns[i], pstTaskDef))
			{
				continue;
			}
			
			pstTaskRun->TaskRuns[i].TaskStat = TASK_STAT_FAIL;
			task_chg(pstEnv, pstPlayer, &pstTaskRun->TaskRuns[i]);
		}
	}

	/*
	for (i=pstTaskCycle->TaskRecycleNum-1; i>=0; i--)
	{
		if (pstTaskCycle->TaskRecycles[i].Stat == TASKCYCLE_STAT_INIT && ((int)(pstTaskCycle->TaskRecycles[i].GetTime+21600) < pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			pstTaskCycle->TaskRecycleNum--;
			if (i != pstTaskCycle->TaskRecycleNum)
			{
				pstTaskCycle->TaskRecycles[i] = pstTaskCycle->TaskRecycles[pstTaskCycle->TaskRecycleNum];
			}
		}
	}
	
	
	for (i=pstTaskOneDay->TaskNum-1; i>=0; i--)
	{
		if (0 == IsSameDay(pstTaskOneDay->TaskOneDayInfo[i].Time, pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			pstTaskOneDay->TaskNum--;
			if (i != pstTaskOneDay->TaskNum)
			{
				pstTaskOneDay->TaskOneDayInfo[i] = pstTaskOneDay->TaskOneDayInfo[pstTaskOneDay->TaskNum];
			}
		}
	}

	for (i=pstTaskCycleOneDay->TaskCycleNum-1; i>=0; i--)
	{
		if (0 == IsSameDay(pstTaskCycleOneDay->TaskCycleOneDayInfo[i].Time, pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			pstTaskCycleOneDay->TaskCycleNum--;
			if (i != pstTaskCycleOneDay->TaskCycleNum)
			{
				pstTaskCycleOneDay->TaskCycleOneDayInfo[i] = pstTaskCycleOneDay->TaskCycleOneDayInfo[pstTaskCycleOneDay->TaskCycleNum];
			}
		}
	}

	for (i=pstGroupOneDay->GroupNum -1; i>=0; i--)
	{
		if (0 == IsSameDay(pstGroupOneDay->GroupOneDayInfo[i].Time, pstEnv->pstAppCtx->stCurr.tv_sec))
		{
			pstGroupOneDay->GroupNum --;
			if (i != pstGroupOneDay->GroupNum)
			{
				pstGroupOneDay->GroupOneDayInfo[i] = pstGroupOneDay->GroupOneDayInfo[pstGroupOneDay->GroupNum];
			}
		}
	}*/

	return 0;
}

int task_check_trace(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i,j;
	TASKMONTRACE *pstTaskMonTrace;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_TRACE;
	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_RESPACT_TRACE;
	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_HIT_TRACE;
	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_STATUS_FAIRYCAMP_TRACE;
	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_OWNED_TMP_ITEM_TRACE;
	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_LIFE_TIME_TRACE;
	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_KILL_PLAYER_TRACE;
	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_BEATK_TRACE;
	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_TMPITEM_PATROL_TRACE;
	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_USE_ITEM;
	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_ONLINE_TIME;
	
	for (i=0; i<pstTaskRun->TaskRunNum; i++)
	{
		const TASKDEF *pstDef = task_def_find(pstEnv,
		                                      pstTaskRun->TaskRuns[i].TaskID,
		                                      &pstTaskRun->TaskRuns[i].TaskIdx);

		pstTaskMonTrace = &pstTaskRun->TaskRuns[i].TaskMonTrace;
		
		for (j=0; j<pstTaskMonTrace->TaskMonTraceNum; j++)
		{
			if (pstTaskMonTrace->TaskKillMons[j].Num < pstTaskMonTrace->TaskKillMons[j].MaxNum)
			{
				pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_TRACE;
			}
		}

		//if (pstTaskRun->TaskRuns[i].TaskStat != TASK_STAT_RUN)
		//{
		//	continue;
		//}

		if(!pstDef)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "task : %d is not exist now.",
			           pstTaskRun->TaskRuns[i].TaskID);
			continue;
		}

		for(j = 0; j < (int)CCH(pstDef->SpecialTask); j++)
		{
			if (0 == pstDef->SpecialTask[j].FiniObjType)
			{
				break;
			}

			//挪下来，有个特殊的任务完成了也要改回未完成。
			if (pstTaskRun->TaskRuns[i].TaskStat != TASK_STAT_RUN && 
				pstDef->SpecialTask[j].FiniObjType != TASK_OBJ_OWNED_TMP_ITEM)
			{
				break;
			}
				
	
			switch(pstDef->SpecialTask[j].FiniObjType)
			{
			case TASK_OBJ_RESPACT:
				pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_RESPACT_TRACE;
				break;
			case TASK_OBJ_SPECIFY_STRIKE:
				pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_HIT_TRACE;
				break;
			case TASK_OBJ_FAIRY_CAMP:
				pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_STATUS_FAIRYCAMP_TRACE;
				break;
			case TASK_OBJ_OWNED_TMP_ITEM:
				pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_OWNED_TMP_ITEM_TRACE;
				break;
			case TASK_OBJ_PLAYER_LIFE_TIME:
				pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_LIFE_TIME_TRACE;
				break;
			case TASK_OBJ_KILL_PLAYER:
				pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_KILL_PLAYER_TRACE;
				break;
			case TASK_OBJ_BEATK_PLAYER_FAIL:
				pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_BEATK_TRACE;
				break;
			case TASK_OBJ_TMPITEM_PATROL_END:
				pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_TMPITEM_PATROL_TRACE;
				break;
			case TASK_OBJ_USE_ITEM:
				pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_USE_ITEM;
				break;
			case TASK_OBJ_ONLINE_TIME:
				pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_ONLINE_TIME;
				break;
			}
		}
	}

	return 0;
}

int task_mon_in_trace(ZONESVRENV *pstEnv, Player *pstPlayer, Monster *pstMon,
                      const TASKDEF *pstTaskDef,
                      const TASKKILLMON *pstTaskKillMon)
{
	int fPrefix = 0;
	unsigned i, iMonID = pstMon->iDefIdx;

	// 检测特殊杀怪的任务
	for(i = 0; i < CCH(pstTaskDef->SpecialTask); i++)
	{
		switch(pstTaskDef->SpecialTask[i].FiniObjType)
		{
		case NONE_TASK_OBJ:
			goto out_range;
		case TASK_OBJ_LVRANGE_MONSTER:
			{
				int iDist = 0;
				iDist = (int)pstPlayer->stRoleData.Level -
				        (int)pstMon->stMonDynAttr.Level;
				if(iDist < 0)
				{
					iDist = -iDist;
				}
				if((unsigned)iDist > pstTaskDef->SpecialTask[i].FiniObjVal)
				{
					fPrefix = 1;
					goto out_range;
				}
				return 1; // 只要等级相差在指定范围内，就直接计数
			}
			break;
		case TASK_OBJ_SIGN_KILL:
			{
				if(!z_mon_find_status(pstMon, pstTaskDef->SpecialTask[i].FiniObjVal))
				{
					fPrefix = 1;
					goto out_range;
				}
			}
			break;
		case TASK_OBJ_LONGDAN_XIEZHU:
			{
				int j, iValidNum = 0;
				Player *pstTmpPlayer;
				ZoneTeam *pstTeam = player_team_get(pstEnv, pstPlayer);
				if(!pstTeam)
				{
					fPrefix = 1;
					goto out_range;
				}
					
				if(GET_MAPID(pstPlayer->stRoleData.Map) != 21100)
				{
					fPrefix = 1;
					goto out_range;
				}

				for (j=0; j<pstTeam->stTeamInfo.MemberNum; j++)
				{
					pstTmpPlayer = z_name_player(pstEnv->pstAppCtx,pstEnv, 
										pstTeam->stTeamInfo.TeamMember[j].MemberName);
					if (!pstTmpPlayer || pstTmpPlayer == pstPlayer)
					{
						continue;
					}

					if(pstTmpPlayer->stRoleData.Map != pstPlayer->stRoleData.Map)
					{
						continue;
					}
						

					if (pstTmpPlayer->stRoleData.Level < pstTaskDef->SpecialTask[i].FiniObjVal)
					{
						iValidNum++;
					}
				}

				if(iValidNum < 2)
				{
					fPrefix = 1;
					goto out_range;
				}
			}
			break;
		default:
			break;
		}
	}

out_range:
	if(fPrefix)
	{
		return 0;
	}

	if (iMonID == pstTaskKillMon->MonID1 || iMonID == pstTaskKillMon->MonID2 ||
		iMonID == pstTaskKillMon->MonID3 || iMonID == pstTaskKillMon->MonID4 ||
		iMonID == pstTaskKillMon->MonID5)
	{
		return 1;
	}

	return 0;
}

int task_trace_mon(ZONESVRENV *pstEnv, Player *pstPlayer, Monster *pstMon)
{
	int iFlag = 0;
	int i,j;
	TASKMONTRACE *pstTaskMonTrace;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	//int iMonID = pstMon->iDefIdx;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i=pstTaskRun->TaskRunNum-1; i>=0; i--)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if (TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}
			
		pstTaskMonTrace = &pstTaskRunOne->TaskMonTrace;
		for (j=0; j<pstTaskMonTrace->TaskMonTraceNum; j++)
		{
			if ((pstTaskMonTrace->TaskKillMons[j].Num < pstTaskMonTrace->TaskKillMons[j].MaxNum) &&
				task_mon_in_trace(pstEnv, pstPlayer, pstMon, pstTaskDef, &pstTaskDef->KillMon[j]))
			{
				iFlag = 1;

				pstTaskRunOne->TaskMonTrace.TaskKillMons[j].Num++;
				
				task_check_fini(pstEnv, pstPlayer, pstTaskRunOne, pstTaskDef);
				
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
			}
		}
	}

	if (iFlag)
	{
		task_check_trace(pstEnv, pstPlayer);
	}

	return 0;
}

/*
int task_trace_mon(ZONESVRENV *pstEnv, Player *pstPlayer, int iMonID)
{
	int iFlag = 0;
	int i,j;
	TASKMONTRACE *pstTaskMonTrace;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i=pstTaskRun->TaskRunNum-1; i>=0; i--)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if (TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}
			
		pstTaskMonTrace = &pstTaskRunOne->TaskMonTrace;
		for (j=0; j<pstTaskMonTrace->TaskMonTraceNum; j++)
		{
			if ((pstTaskMonTrace->TaskKillMons[j].Num < pstTaskMonTrace->TaskKillMons[j].MaxNum) &&
				task_mon_in_trace(iMonID, &pstTaskDef->KillMon[j]))
			{
				iFlag = 1;

				//task_check_fini(pstEnv, pstPlayer, pstTaskRunOne, pstTaskDef);
				pstTaskRunOne->TaskMonTrace.TaskKillMons[j].Num++;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
			}
		}
	}

	if (iFlag)
	{
		task_check_trace(pstEnv, pstPlayer);
	}

	return 0;
}
*/

int task_type_to_string(unsigned char bTaskType, char* pszBuff, int iSize)
{
	switch(bTaskType)
	{
		case TASK_TYPE_NEW:
			STRNCPY(pszBuff, LJY_TASK11,iSize);
			break;
		case TASK_TYPE_MAIN:
			STRNCPY(pszBuff, LJY_TASK12,iSize);
			break;
		case TASK_TYPE_BRANCH:
			STRNCPY(pszBuff, LJY_TASK13,iSize);
			break;
		case TASK_TYPE_BUSSINESS:
			STRNCPY(pszBuff, LJY_TASK14,iSize);
			break;
		case TASK_TYPE_ACT:
			STRNCPY(pszBuff, LJY_TASK15,iSize);
			break;
		case TASK_TYPE_REP:
			STRNCPY(pszBuff, LJY_TASK16,iSize);
			break;
		case TASK_TYPE_GOLD:
			STRNCPY(pszBuff, LJY_TASK17,iSize);
			break;
		case TASK_TYPE_PWORLD:
			STRNCPY(pszBuff, LJY_TASK18,iSize);
			break;	
		case TASK_TYPE_DISCOVER:
			STRNCPY(pszBuff, LJY_TASK19,iSize);
			break;	
		case TASK_TYPE_SINGLE_CONVOY:
			STRNCPY(pszBuff, LJY_TASK20,iSize);
			break;
		case TASK_TYPE_TEAM_CONVOY:
			STRNCPY(pszBuff, LJY_TASK21,iSize);
			break;
		case TASK_TYPE_CLAN:
			STRNCPY(pszBuff, LJY_TASK22,iSize);
			break;
		case TASK_TYPE_DYN:
			STRNCPY(pszBuff, LJY_TASK23,iSize);
			break;
		case TASK_TYPE_CAREER:
			STRNCPY(pszBuff, LJY_TASK24,iSize);
			break;
		case TASK_TYPE_RIDE:
			STRNCPY(pszBuff, LJY_TASK25,iSize);
			break;
		case TASK_TYPE_FAIRY:
			STRNCPY(pszBuff, LJY_TASK26,iSize);
			break;
		case TASK_TYPE_SECONDARYSKILL:
			STRNCPY(pszBuff, LJY_TASK27,iSize);
			break;
		case TASK_TYPE_GROUP:
			STRNCPY(pszBuff, LJY_TASK28,iSize);
			break;
		case TASK_TYPE_DART_CAR:
			STRNCPY(pszBuff, LJY_TASK29,iSize);
			break;
		case TASK_TYPE_SJ_BOSS:
		case TASK_TYPE_SJ_ITEM:
		case TASK_TYPE_SJ_MON:
			STRNCPY(pszBuff, LJY_TASK30,iSize);
			break;
		case TASK_TYPE_DESIGNATION:
			STRNCPY(pszBuff, LJY_TASK31,iSize);
			break;
		case TASK_TYPE_RONGLU:
			STRNCPY(pszBuff, LJY_TASK32,iSize);
			break;
		case TASK_TYPE_XIEZHU:
			STRNCPY(pszBuff, LJY_TASK33,iSize);
			break;
		case TASK_TYPE_BATTLE:
			STRNCPY(pszBuff, LJY_TASK34,iSize);
			break;
		case TASK_TYPE_PAOSHANG:
			STRNCPY(pszBuff, LJY_TASK36,iSize);
			break;
		default:
			return -1;
			break;
	}

	return 0;
}

int player_trust_order_list( ZONESVRENV* pstEnv, Player *pstPlayer )
{
	ROLETRUSTDATA stTrustHash, * pstTrustHash = NULL;
	
	stTrustHash.RoleID = pstPlayer->stRoleData.RoleID;
	pstTrustHash = sht_find(pstEnv->pstShtTrust, &stTrustHash, TrustHashCmp, TrustHashCode);
	if ( !pstTrustHash )
	{
		return -1;
	}

	trust_onerole_update( pstEnv, pstPlayer, pstTrustHash, 0ULL );
	return 0;
}

int task_opt_str(ZONESVRENV *pstEnv, Npc *pstNpc, Player *pstPlayer, char *pszOptStr, int iOptStr, int iTaskID)
{
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	char szBuff[32];
	int iRet;
	
	pszOptStr[0] = 0;

	pstTaskDef = task_def_find(pstEnv, iTaskID, NULL);
	if (NULL == pstTaskDef)
	{
		return -1;
	}

	if (task_type_to_string(pstTaskDef->Type, szBuff, sizeof(szBuff)) < 0 )
	{
		strcpy(szBuff, LJY_TASK10);
	}

	pstTaskRunOne = task_run_find(pstPlayer, iTaskID);
	if (NULL == pstTaskRunOne) //接任务
	{
		if (0 > task_check_npc(pstEnv, pstPlayer, pstNpc->stMon.iID, iTaskID, 0))
		{
			return -1;
		}

		iRet = task_accept_check_matchclient(pstEnv, pstPlayer, pstTaskDef, 0);
		// task_accept_check(pstEnv, pstPlayer, pstTaskDef, 0, 0);
		
		if (0 > iRet)
		{
			if(-TAE_LEVEL_ONE == iRet)
			{
				/*
				if(npc_task_accept_check(pstEnv, pstPlayer, pstNpc))
				{
					return -1;
				}
				*/
				
				snprintf(pszOptStr, iOptStr, "(%s)[%s] %s", LJY_TASK35, szBuff,pstTaskDef->Name);
			}
			else
			{
				return -1;
			}
		}
		else
		{
			snprintf(pszOptStr, iOptStr, "(%s)[%s] %s", LJY_TASK6, szBuff,pstTaskDef->Name);
		}
	}
	else
	{
		if (0 > task_check_npc(pstEnv, pstPlayer, pstNpc->stMon.iID, iTaskID, 1))
		{
			return -1;
		}
		
		if (task_check_fini(pstEnv, pstPlayer, pstTaskRunOne, pstTaskDef))
		{
			snprintf(pszOptStr, iOptStr, "(%s)[%s] %s", LJY_TASK4, szBuff,pstTaskDef->Name);
		}
		else
		{
			snprintf(pszOptStr, iOptStr, "(%s)[%s] %s", LJY_TASK5, szBuff,pstTaskDef->Name);
		}
	}

	pszOptStr[iOptStr - 1] = 0;
		
	return 0;
}


int pet_task_condition_check( FINISPECIALTASK *pstSpecialTask, ROLEPET *pstRolePet)
{
	int i;
	PETSKILLGRID *pstPetSkillOne;
	
	switch(pstSpecialTask->FiniObjType)
	{
		case TASK_OBJ_PET:

			if(pstSpecialTask->FiniObjVal != 0 && pstRolePet->PetID != pstSpecialTask->FiniObjVal)
			{
				return -1;
			}
			
			if (pstSpecialTask->FiniConditionType == TASK_CONDITION_LEVEL &&
				pstRolePet->Level >= pstSpecialTask->FiniConditionVal )
			{
				return 1;
			}
			break;
		case TASK_OBJ_PET_SKILL:
			for (i = 0 ; i<pstRolePet->PetSkills.SkillGridNum; i++)
			{
				pstPetSkillOne = &pstRolePet->PetSkills.SkillGrid[i];

				if (pstPetSkillOne->SkillID == pstSpecialTask->FiniObjVal &&
					pstSpecialTask->FiniConditionType == TASK_CONDITION_LEVEL && 
					pstRolePet->Level >= pstSpecialTask->FiniConditionVal )
				{
					return 1;
				}
			}
			break;
		case TASK_OBJ_CATCH_PET:
			if (pstRolePet->PetID == pstSpecialTask->FiniObjVal )
			{
				return 1;
			}
			break;
		case TASK_OBJ_PET_WASH:
			return 1;
			break;
		case TASK_OBJ_PET_STRENGTHEN:
			return 1;
			break;
		case TASK_OBJ_PET_RESET_STRENGTHEN:
			return 1;
			break;
		default:
			break;
	}

	return 0;
}

int pets_task_special_fini(ZONESVRENV *pstEnv, Player *pstPlayer, FINISPECIALTASK *pstSpecialTask, TASKRUNONE *pstTaskRunOne)
{
	int i;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;

	for (i = 0; i < pstPetPackage->Num; i++)
	{
		if ( 1 ==pet_task_condition_check( pstSpecialTask, &pstPetPackage->RolePets[i]))
		{
			goto fini;
		}
	}		

	return 0;

fini:
	pstTaskRunOne->TaskStat = TASK_STAT_FINI;
	task_chg(pstEnv, pstPlayer, pstTaskRunOne);
	return 0;
}

int z_find_wear_by_itemid(ZONESVRENV* pstEnv, ROLEWEAR *pstRoleWear, int iItemID)
{
	int i;
	UNUSED(pstEnv);
	
	for (i=0; i<pstRoleWear->ArmNum; i++)
	{
		if ( pstRoleWear->RoleArms[i].DefID == iItemID )
		{
			return i;
		}
	}

	return -1;
}

int task_wear_arm(ZONESVRENV *pstEnv, Player *pstPlayer, FINISPECIALTASK *pstSpecialTask,
						TASKRUNONE *pstTaskRunOne)
{
	int iItemID = pstSpecialTask->FiniObjVal;
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	
	if ( z_find_wear_by_itemid(pstEnv, pstRoleWear, iItemID) >= 0)
	{
		pstTaskRunOne->TaskStat = TASK_STAT_FINI;
		task_chg(pstEnv, pstPlayer, pstTaskRunOne);
	}

	return 0;
}

int task_player_map(ZONESVRENV *pstEnv, Player *pstPlayer, FINISPECIALTASK *pstSpecialTask,
						TASKRUNONE *pstTaskRunOne)
{
	int iMapID =  pstSpecialTask->FiniObjVal;

	if (iMapID == GET_MAPID(pstPlayer->stRoleData.Map))
	{
		pstTaskRunOne->TaskStat = TASK_STAT_FINI;
		task_chg(pstEnv, pstPlayer, pstTaskRunOne);
	}

	return 0;
}


int task_player_skill(ZONESVRENV *pstEnv, Player *pstPlayer, FINISPECIALTASK *pstSpecialTask,
						TASKRUNONE *pstTaskRunOne)
{
	int i;
	ZoneSkill *pstZoneSkill;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;

	for (i=0; i< pstZoneSkillList->nSkillNum; i++)
	{
		pstZoneSkill = &pstZoneSkillList->astZoneSkills[i];

		if (pstZoneSkill->stSkillData.SkillID == pstSpecialTask->FiniObjVal)
		{
			if ( pstSpecialTask->FiniConditionType == TASK_CONDITION_LEVEL &&
				pstZoneSkill->stSkillData.SkillLevel >= pstSpecialTask->FiniConditionVal)
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
			}
			
			break;
		}
	}

	return 0;
}

int task_player_diathesis(ZONESVRENV *pstEnv, Player *pstPlayer, FINISPECIALTASK *pstSpecialTask,
						TASKRUNONE *pstTaskRunOne)
{
	int i;
	DIATHESISDATA *pstDiathesisData;
	DIATHESISINFO *pstDiathesisInfo = &pstPlayer->stRoleData.DiathesisInfo;

	for (i=0; i<(int)pstDiathesisInfo->DiathesisNum; i++)
	{
		pstDiathesisData = &pstDiathesisInfo->DiathesisList[i];
		if (pstDiathesisData->DiathesisID == pstSpecialTask->FiniObjVal)
		{
			if ( pstSpecialTask->FiniConditionType == TASK_CONDITION_LEVEL &&
				pstDiathesisData->DiathesisID >= pstSpecialTask->FiniConditionVal)
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
			}

			break;
		}
	}

	return 0;
}

int task_player_tmpitem_patrol_end(ZONESVRENV *pstEnv, Player *pstPlayer, FINISPECIALTASK *pstSpecialTask,
						TASKRUNONE *pstTaskRunOne)
{
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;

	if(pstRoleTmpSys->PatrolID <= 0 || 
		pstRoleTmpSys->DelPos.X != pstPlayer->stRoleData.Pos.X ||
		pstRoleTmpSys->DelPos.Y != pstPlayer->stRoleData.Pos.Y)
	{
		return -1;
	}

	if ( pstSpecialTask->FiniObjVal == pstRoleTmpSys->TmpItemID)
	{
		pstTaskRunOne->TaskStat = TASK_STAT_FINI;
		task_chg(pstEnv, pstPlayer, pstTaskRunOne);
	}

	return 0;
}

int task_player_clan(ZONESVRENV *pstEnv, Player *pstPlayer,
                     FINISPECIALTASK *pstSpecialTask, TASKRUNONE *pstTaskRunOne)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);

	if(!pstZoneClan)
	{
		return 0;
	}

	if(pstSpecialTask->FiniObjType == TASK_OBJ_JOIN_CLAN)
	{
		// 加入一个公会的目标已达成
		goto fini;
	}
	else if(pstSpecialTask->FiniObjType == TASK_OBJ_CLAN_CONTRIBUTE)
	{
		if(pstPlayer->stRoleData.MiscInfo.ClanInfo.Contribute >=
			 pstSpecialTask->FiniObjVal)
		{
		// 公会贡献度的目标已达成
			goto fini;
		}
	}
	return 0;

fini:
	pstTaskRunOne->TaskStat = TASK_STAT_FINI;
	task_chg(pstEnv, pstPlayer, pstTaskRunOne);
	return 0;	
}

int task_player_respact(ZONESVRENV *pstEnv, Player *pstPlayer,
                     FINISPECIALTASK *pstSpecialTask, TASKRUNONE *pstTaskRunOne)
{
	const RESPACTENTRY *pstEntry = player_find_respact(pstEnv, pstPlayer,
	                                                pstSpecialTask->FiniObjVal);
	const RESPACTPHASEDEF *pstPhaseDef = z_respact_phase_def(pstEnv,
	                                          pstSpecialTask->FiniConditionVal);

	if(!pstEntry || !pstPhaseDef)
	{
		return 0;
	}

	// 检查是否达到某个声望等级
	if(pstEntry->Value >= pstPhaseDef->NeedRespact)
	{
		pstTaskRunOne->TaskStat = TASK_STAT_FINI;
		task_chg(pstEnv, pstPlayer, pstTaskRunOne);
	}
	return 0;
}

int task_arm_condition_check(FINISPECIALTASK *pstSpecialTask, ROLEGRID *pstArmGrid)
{
	switch (pstSpecialTask->FiniObjType)
	{
		case TASK_OBJ_ARM_STAR:
			return 1;
			break;
		case TASK_OBJ_WEAR_ARM:
			if (pstArmGrid->Type == ITEM_TYPE_ARM && pstArmGrid->GridData.RoleArm.DefID == (int )pstSpecialTask->FiniObjVal)
			{
				return 1;
			}
			break;
		default:
			return 0;
			break;
	}

	return 0;
}

int task_arm_trace(ZONESVRENV *pstEnv, Player *pstPlayer, ROLEGRID *pstArmGrid, int iTraceType)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i=pstTaskRun->TaskRunNum-1; i>=0; i--)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if (TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j=0; j<MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType == 0)
			{
				break;
			}
			
			if (pstTaskDef->SpecialTask[j].FiniObjType != iTraceType)
			{
				continue;
			}

			if ( 1 == task_arm_condition_check(&pstTaskDef->SpecialTask[j], pstArmGrid) )
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
			}
		}
	}

	return 0;
}

int task_sjstar_trace(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i=pstTaskRun->TaskRunNum-1; i>=0; i--)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if (TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j=0; j<MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType == 0)
			{
				break;
			}
			
			if (pstTaskDef->SpecialTask[j].FiniObjType != TASK_OBJ_REFRESH_SJSTAR)
			{
				continue;
			}

			pstTaskRunOne->TaskStat = TASK_STAT_FINI;
			task_chg(pstEnv, pstPlayer, pstTaskRunOne);
		}
	}

	return 0;
}

int task_npcbuy_trace(ZONESVRENV *pstEnv, Player *pstPlayer, ITEMDEF *pstItemDef)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i=pstTaskRun->TaskRunNum-1; i>=0; i--)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if (TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j=0; j<MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType == 0)
			{
				break;
			}
			
			if (pstTaskDef->SpecialTask[j].FiniObjType != TASK_OBJ_NPCBUY)
			{
				continue;
			}

			if (pstTaskDef->SpecialTask[j].FiniObjVal == pstItemDef->ItemID)
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
			}
		}
	}

	return 0;
}


int task_fini_trace(ZONESVRENV *pstEnv, Player *pstPlayer, unsigned int uiTaskID)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i=pstTaskRun->TaskRunNum-1; i>=0; i--)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if (TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j=0; j<MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType == 0)
			{
				break;
			}

			if (pstTaskDef->SpecialTask[j].FiniObjType == TASK_OBJ_TASK)
			{
				if ( pstTaskDef->SpecialTask[j].FiniObjVal == uiTaskID )
				{
					pstTaskRunOne->TaskStat = TASK_STAT_FINI;
					task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				}
			}
			else if (pstTaskDef->SpecialTask[j].FiniObjType == TASK_OBJ_FINI_SJ)
			{
				TASKDEF *pstTaskFini;

				pstTaskFini = task_def_find(pstEnv, uiTaskID, NULL);
				if (pstTaskFini && (pstTaskFini->Type == TASK_TYPE_SJ_MON ||
					pstTaskFini->Type == TASK_TYPE_SJ_ITEM || pstTaskFini->Type == TASK_TYPE_SJ_BOSS))
				{
					pstTaskRunOne->TaskStat = TASK_STAT_FINI;
					task_chg(pstEnv, pstPlayer, pstTaskRunOne);	
				}
			}
		}
	}

	return 0;
}

int taskcycle_fini_trace(ZONESVRENV *pstEnv, Player *pstPlayer, unsigned int iTaskCycleID)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i=pstTaskRun->TaskRunNum-1; i>=0; i--)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if (TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j=0; j<MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType == 0)
			{
				break;
			}
			
			if (pstTaskDef->SpecialTask[j].FiniObjType != TASK_OBJ_TASKCYCLE)
			{
				continue;
			}

			if ( pstTaskDef->SpecialTask[j].FiniObjVal == iTaskCycleID )
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
			}
		}
	}

	return 0;
}

int task_player_fairy_lv(ZONESVRENV *pstEnv, Player *pstPlayer, FINISPECIALTASK *pstSpecialTask, TASKRUNONE *pstTaskRunOne)
{
	int i;
	ROLEFAIRY *pstRoleFairy;
	FAIRYPACKAGE *pstFairyPkg = &pstPlayer->stRoleData.Package.FairyPackage;

	for (i=0; i<pstFairyPkg->FairyNum; i++)
	{
		pstRoleFairy = &pstFairyPkg->RoleFairys[i];
		if (	pstSpecialTask->FiniObjVal == pstRoleFairy->FairyID &&
			pstSpecialTask->FiniConditionType == TASK_CONDITION_LEVEL &&
			pstRoleFairy->FairyLevel >= pstSpecialTask->FiniConditionVal)
		{
			pstTaskRunOne->TaskStat = TASK_STAT_FINI;
			task_chg(pstEnv, pstPlayer, pstTaskRunOne);
			break;
		}
	}
	return 0;
}

int task_player_owned_tmp_item(ZONESVRENV *pstEnv, Player *pstPlayer,
                               FINISPECIALTASK *pstSpecialTask,
                               TASKRUNONE *pstTaskRunOne)
{
	// 有指定的临时物品，任务就完成，否则就未完成
	if(pstPlayer->stRoleData.MiscInfo.TmpSys.TmpItemID ==
	   pstSpecialTask->FiniObjVal)
	{
		pstTaskRunOne->TaskStat = TASK_STAT_FINI;
		task_chg(pstEnv, pstPlayer, pstTaskRunOne);
	}
	else
	{
		pstTaskRunOne->TaskStat = TASK_STAT_RUN;
		task_chg(pstEnv, pstPlayer, pstTaskRunOne);
	}
	return 0;
}

int task_player_fairy_camp(ZONESVRENV *pstEnv, Player *pstPlayer, FINISPECIALTASK *pstSpecialTask, TASKRUNONE *pstTaskRunOne)
{
	int i;
	ROLEFAIRY *pstRoleFairy;
	FAIRYPACKAGE *pstFairyPkg = &pstPlayer->stRoleData.Package.FairyPackage;

	for (i=0; i<pstFairyPkg->FairyNum; i++)
	{
		pstRoleFairy = &pstFairyPkg->RoleFairys[i];
		if (	(pstRoleFairy->Stat & FAIRY_STAT_CAMP) &&
			pstSpecialTask->FiniObjVal == pstRoleFairy->FairyID)
		{
			pstTaskRunOne->TaskStat = TASK_STAT_FINI;
			task_chg(pstEnv, pstPlayer, pstTaskRunOne);
			break;
		}
	}
	return 0;
}

int task_player_ride(ZONESVRENV *pstEnv, Player *pstPlayer, FINISPECIALTASK *pstSpecialTask,
						TASKRUNONE *pstTaskRunOne)
{
	int i;
	ROLERIDELIST *psRideList = &pstPlayer->stRoleData.MiscInfo.RideList;

	for (i=0; i<(int)psRideList->Num; i++)
	{
		if (pstSpecialTask->FiniObjVal == 0 || psRideList->RideGrids[i].ItemID == pstSpecialTask->FiniObjVal)
		{
			if ( pstSpecialTask->FiniConditionType == TASK_CONDITION_LEVEL )
			{
				if(pstTaskRunOne->TaskStat != TASK_STAT_FINI &&
					psRideList->RideGrids[i].Level >= pstSpecialTask->FiniConditionVal)
				{
					pstTaskRunOne->TaskStat = TASK_STAT_FINI;
					task_chg(pstEnv, pstPlayer, pstTaskRunOne);
					break;
				}
			}
		}
	}

	return 0;
}

int task_player_unreal_save(ZONESVRENV *pstEnv, Player *pstPlayer, FINISPECIALTASK *pstSpecialTask,
						TASKRUNONE *pstTaskRunOne)
{
	if (pstPlayer->stRoleData.MiscInfo.Unreal.Save > 0)
	{
		pstTaskRunOne->TaskStat = TASK_STAT_FINI;
		task_chg(pstEnv, pstPlayer, pstTaskRunOne);	
	}
	return 0;
	UNUSED(pstSpecialTask);	//add by paraunused.pl
}

//指定地图存活一定时间
int task_player_life_time(ZONESVRENV *pstEnv, Player *pstPlayer,  FINISPECIALTASK *pstSpecialTask,
						TASKRUNONE *pstTaskRunOne)
{
	MapIndex *pstIdx = NULL;
	
	if(pstSpecialTask->FiniConditionType == TASK_CONDITION_MAP)
	{
		pstIdx = z_mappos_index(pstEnv->pstMapObj, NULL, GET_MAPID(pstPlayer->stRoleData.Map));
		if( NULL == pstIdx )
		{
			return 0;
		}
	}

	if (pstTaskRunOne->TaskFiniTime)
	{
		if (pstPlayer->stOnline.State & CS_STAT_DEAD)
		{
			pstTaskRunOne->TaskStat = TASK_STAT_FAIL;
			task_chg(pstEnv, pstPlayer, pstTaskRunOne);

			return 0;
		}
		
		if(pstIdx && 
			pstSpecialTask->FiniConditionVal != pstIdx->stMapDef.MapID && 
			pstSpecialTask->FiniConditionVal != pstIdx->stMapDef.ReferMapID)
		{
			pstTaskRunOne->TaskStat = TASK_STAT_FAIL;
			task_chg(pstEnv, pstPlayer, pstTaskRunOne);
		}

		if((int)pstTaskRunOne->TaskFiniTime > pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			return 0;
		}
		
		pstTaskRunOne->TaskStat = TASK_STAT_FINI;
		task_chg(pstEnv, pstPlayer, pstTaskRunOne);
	}
	else
	{
		CSPKG stPkg;
		CSTASKSVR *pstTaskSvr = &stPkg.Body.TaskSvr;
	
		if(pstIdx &&
			pstSpecialTask->FiniConditionVal != pstIdx->stMapDef.MapID && 
			pstSpecialTask->FiniConditionVal != pstIdx->stMapDef.ReferMapID)
		{
			return 0;
		}

		pstTaskRunOne->TaskFiniTime = pstEnv->pstAppCtx->stCurr.tv_sec + pstSpecialTask->FiniObjVal;

		pstTaskSvr->OpType = TASK_TIMER;
		pstTaskSvr->TaskSvrData.TaskTimer.TaskID = pstTaskRunOne->TaskID;
		pstTaskSvr->TaskSvrData.TaskTimer.TimerMode = 0;
		pstTaskSvr->TaskSvrData.TaskTimer.Time = pstSpecialTask->FiniObjVal;

		Z_CSHEAD_INIT(&stPkg.Head, TASK_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	
	return 0;
}

int task_beatk_player_trace(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i=pstTaskRun->TaskRunNum-1; i>=0; i--)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if(TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j=0; j<MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType == 0)
			{
				break;
			}
			
			if (pstTaskDef->SpecialTask[j].FiniObjType == TASK_OBJ_BEATK_PLAYER_FAIL)
			{
				if(pstTaskRunOne->TaskGenericProgress.Progress + 1 >= pstTaskRunOne->TaskGenericProgress.Max)
				{
					pstTaskRunOne->TaskStat = TASK_STAT_FAIL;
					task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				}
				else
				{
					task_generic_progress_run(pstEnv, pstPlayer, pstTaskDef->ID, 1);
				}
				break;
			}
		}
	}

	return 0;
}

int task_player_sparta_fini_trace(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i=pstTaskRun->TaskRunNum-1; i>=0; i--)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if(TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j=0; j<MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType == 0)
			{
				break;
			}
			
			if(pstTaskDef->SpecialTask[j].FiniObjType == TASK_OBJ_SPARTA_FINI)
			{
				task_generic_progress_run(pstEnv, pstPlayer, pstTaskDef->ID, 1);
				break;
			}
		}
	}

	return 0;
}
int task_special_one_fini(ZONESVRENV *pstEnv, Player *pstPlayer,  FINISPECIALTASK *pstSpecialTask, TASKRUNONE *pstTaskRunOne)
{
	switch (pstSpecialTask->FiniObjType)
	{
		case TASK_OBJ_PLAYER:
			if (pstSpecialTask->FiniConditionType == TASK_CONDITION_LEVEL 
				&& pstPlayer->stRoleData.Level >=  pstSpecialTask->FiniConditionVal)
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
			}
			return 0;
			break;
		case TASK_OBJ_PET:
		case TASK_OBJ_PET_SKILL:
			pets_task_special_fini(pstEnv, pstPlayer, pstSpecialTask, pstTaskRunOne);
			break;
		case TASK_OBJ_WEAR_ARM:
			task_wear_arm(pstEnv, pstPlayer, pstSpecialTask, pstTaskRunOne);
			break;
		case TASK_OBJ_PLAYER_SKILL:
			task_player_skill( pstEnv, pstPlayer, pstSpecialTask, pstTaskRunOne);
			break;
		case TASK_OBJ_PLAYER_DIATHESIS:
			task_player_diathesis(pstEnv, pstPlayer, pstSpecialTask, pstTaskRunOne);
			break;
		case TASK_OBJ_JOIN_CLAN:
		case TASK_OBJ_CLAN_CONTRIBUTE:
			task_player_clan(pstEnv, pstPlayer, pstSpecialTask, pstTaskRunOne);
			break;
		case TASK_OBJ_RESPACT:
			task_player_respact(pstEnv, pstPlayer, pstSpecialTask, pstTaskRunOne);
			break;
		case TASK_OBJ_MAP: 
			task_player_map(pstEnv, pstPlayer, pstSpecialTask, pstTaskRunOne);
			break;
		case TASK_OBJ_FAIRY_CAMP:
			task_player_fairy_camp(pstEnv, pstPlayer, pstSpecialTask, pstTaskRunOne);
			break;
		case TASK_OBJ_FAIRY_LV:
			task_player_fairy_lv(pstEnv, pstPlayer, pstSpecialTask, pstTaskRunOne);
			break;
		case TASK_OBJ_OWNED_TMP_ITEM:
			task_player_owned_tmp_item(pstEnv, pstPlayer, pstSpecialTask,
			                           pstTaskRunOne);
			break;
		case TASK_OBJ_RIDE:
			task_player_ride(pstEnv, pstPlayer, pstSpecialTask, pstTaskRunOne);
			break;
		case TASK_OBJ_PLAYER_LIFE_TIME:
			task_player_life_time(pstEnv, pstPlayer, pstSpecialTask, pstTaskRunOne);
			break;
		case TASK_OBJ_TASK:
			if (0 == task_find_fini(pstPlayer, pstSpecialTask->FiniObjVal))
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
			}
			break;
		case TASK_OBJ_TMPITEM_PATROL_END:
			task_player_tmpitem_patrol_end(pstEnv, pstPlayer, pstSpecialTask, pstTaskRunOne);
			break;
		case TASK_OBJ_FAIRY_STAR:
			{
				int i;	
				FAIRYPACKAGE *pstFairyPkg = &pstPlayer->stRoleData.Package.FairyPackage;

				for (i=0; i<pstFairyPkg->FairyNum; i++)
				{
					if ( (int)pstSpecialTask->FiniObjVal <= pstFairyPkg->RoleFairys[i].StarLvl )
					{
						pstTaskRunOne->TaskStat = TASK_STAT_FINI;
						task_chg(pstEnv, pstPlayer, pstTaskRunOne);
						break;
					}
				}
			}
			break;
		case TASK_OBJ_UNREAL_SAVE:
			task_player_unreal_save(pstEnv, pstPlayer, pstSpecialTask, pstTaskRunOne);
			break;
		case TASK_OBJ_STATUS_OVERLAY:
			{
				ZoneStatus *pstStatus = z_player_find_status(pstPlayer, pstSpecialTask->FiniObjVal);
				if(!pstStatus)
				{
					break;
				}

				if(pstStatus->unOverlay >= pstSpecialTask->FiniConditionVal)
				{
					pstTaskRunOne->TaskStat = TASK_STAT_FINI;
					task_chg(pstEnv, pstPlayer, pstTaskRunOne);
					break;
				}
			}
			break;
		case TASK_OBJ_PERFECT_ARM:
			if (z_wear_get_star_perfect_num(pstEnv, pstPlayer) >= (int)pstSpecialTask->FiniObjVal)
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				break;
			}
			break;
		default:
			return 0;
			break;
	}

	return 0;
}

int task_special_fini(ZONESVRENV *pstEnv, Player *pstPlayer, TASKDEF *pstTaskDef, TASKRUNONE *pstTaskRunOne)
{
	int i;

	for (i=0; i<MAX_SPECIAL_TASK; i++)
	{
		if (pstTaskDef->SpecialTask[i].FiniObjType == 0)
		{
			break;
		}
		
		task_special_one_fini(pstEnv, pstPlayer, &pstTaskDef->SpecialTask[i], pstTaskRunOne);
	}
	
	return 0;
}

int task_hit_monster_handler(ZONESVRENV* pstEnv, Player *pstPlayer,
                             ZoneSkill *pstZoneSkill, Monster *pstMonster)
{
	TASKDEF *pstTaskDef = NULL;
	TASKRUNONE *pstTaskRunOne = NULL;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;
	int i = 0, j = 0;

	i = pstTaskRun->TaskRunNum;
	while(i--)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if(pstTaskRunOne->TaskStat != TASK_STAT_RUN)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID,
		                           &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for(j = 0; j < MAX_SPECIAL_TASK; j++)
		{
			if(!pstTaskDef->SpecialTask[j].FiniObjType)
			{
				break;
			}

			switch(pstTaskDef->SpecialTask[j].FiniObjType)
			{
			case TASK_OBJ_SPECIFY_STRIKE:
				if(pstZoneSkill->stSkillData.SkillID ==
				   pstTaskDef->SpecialTask[j].FiniConditionVal &&
				   (unsigned)pstMonster->iDefIdx == pstTaskDef->SpecialTask[j].FiniObjVal)
				{
					pstTaskRunOne->TaskStat = TASK_STAT_FINI;
					task_chg(pstEnv, pstPlayer, pstTaskRunOne);
					return 0;
				}
				break;
			}
		}
	}
	return 0;
}

int task_player_trace(ZONESVRENV *pstEnv, Player *pstPlayer, int iTraceType)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i=pstTaskRun->TaskRunNum-1; i>=0; i--)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if(TASK_STAT_DROP == pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		if(TASK_STAT_RUN != pstTaskRunOne->TaskStat &&
		   iTraceType != TASK_OBJ_OWNED_TMP_ITEM) // 例外
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j=0; j<MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType == 0)
			{
				break;
			}
			
			if (pstTaskDef->SpecialTask[j].FiniObjType != iTraceType)
			{
				continue;
			}
			
			if(pstTaskDef->SpecialTask[j].FiniObjType == TASK_OBJ_FENJIE ||
				pstTaskDef->SpecialTask[j].FiniObjType == TASK_OBJ_INLAY_SPAR ||
				pstTaskDef->SpecialTask[j].FiniObjType == TASK_OBJ_ARM_RECAST ||
				pstTaskDef->SpecialTask[j].FiniObjType == TASK_OBJ_ARM_WASH ||
				pstTaskDef->SpecialTask[j].FiniObjType == TASK_OBJ_INLAY_GEM)
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				break;
			}

			if(pstTaskDef->SpecialTask[j].FiniObjType == TASK_OBJ_LVZHOU_WIN)
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				break;
			}

			if(pstTaskDef->SpecialTask[j].FiniObjType == TASK_OBJ_KUANGSHA_WIN)
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				break;
			}

			if(pstTaskDef->SpecialTask[j].FiniObjType == TASK_OBJ_SPAR_ADD_EXP)
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				break;
			}

			if(pstTaskDef->SpecialTask[j].FiniObjType == TASK_OBJ_RIDE_STRENGTHEN)
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				break;
			}

			if(pstTaskDef->SpecialTask[j].FiniObjType == TASK_OBJ_BEACH_TIME_OVER)
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				break;
			}	

			if(pstTaskDef->SpecialTask[j].FiniObjType == TASK_OBJ_SET_SAFE_PWD)
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				break;
			}	

			if(pstTaskDef->SpecialTask[j].FiniObjType == TASK_OBJ_BIND_SAFE_EMAIL)
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				break;
			}	

			if(pstTaskDef->SpecialTask[j].FiniObjType == TASK_OBJ_SPARTA_WIN)
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				break;
			}

			if(pstTaskDef->SpecialTask[j].FiniObjType == TASK_OBJ_ADD_FRIEND)
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				break;
			}
			task_special_one_fini(pstEnv, pstPlayer, &pstTaskDef->SpecialTask[j], pstTaskRunOne);
		}
	}

	return 0;
}

int task_player_repair_trace(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i=pstTaskRun->TaskRunNum-1; i>=0; i--)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if(TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j=0; j<MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType == 0)
			{
				break;
			}
			
			if (pstTaskDef->SpecialTask[j].FiniObjType != TASK_OBJ_REPAIR)
			{
				continue;
			}
			
			pstTaskRunOne->TaskStat = TASK_STAT_FINI;
			task_chg(pstEnv, pstPlayer, pstTaskRunOne);
			break;
		}
	}

	return 0;
}


int task_hit_player_trace(ZONESVRENV *pstEnv, Player *pstPlayer, Player *pstAtkee)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i=pstTaskRun->TaskRunNum-1; i>=0; i--)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if(TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j=0; j<MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType == 0)
			{
				break;
			}
			
			if (pstTaskDef->SpecialTask[j].FiniObjType == TASK_OBJ_KILL_PLAYER)
			{
				if(pstTaskDef->SpecialTask[j].FiniConditionType == TASK_CONDITION_CAREER &&
					(pstAtkee->stRoleData.Career&0x0F) != (pstTaskDef->SpecialTask[j].FiniConditionVal&0x0F) )
				{
					break;
				}

				if(pstTaskDef->SpecialTask[j].FiniConditionType == TASK_CONDITION_MAP)
				{
					MapIndex *pstIdx = NULL;
					
					pstIdx = z_mappos_index(pstEnv->pstMapObj, NULL, GET_MAPID(pstPlayer->stRoleData.Map));
					if( NULL == pstIdx )
					{
						return 0;
					}

					if(pstIdx && 
						pstTaskDef->SpecialTask[j].FiniConditionVal != pstIdx->stMapDef.MapID && 
						pstTaskDef->SpecialTask[j].FiniConditionVal != pstIdx->stMapDef.ReferMapID)
					{
						return 0;
					}
				}
				
				task_generic_progress_run(pstEnv, pstPlayer, pstTaskDef->ID, 1);
				break;
			}
		}
	}

	return 0;
}

int task_pet_trace(ZONESVRENV *pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, int iTraceType)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i=pstTaskRun->TaskRunNum-1; i>=0; i--)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if (TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j=0; j<MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType == 0)
			{
				break;
			}
			
			if (pstTaskDef->SpecialTask[j].FiniObjType != iTraceType)
			{
				continue;
			}

			if ( 1 ==pet_task_condition_check( &pstTaskDef->SpecialTask[j], pstRolePet))
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
			}
		}
	}

	return 0;
}

int fairy_task_cond_check(FINISPECIALTASK *pstSpecialTask, ROLEFAIRY *pstRoleFairy)
{
	switch(pstSpecialTask->FiniObjType)
	{
		case TASK_OBJ_FAIRY_CAMP:
			if (pstSpecialTask->FiniObjVal == pstRoleFairy->FairyID)
			{
				return 1;
			}
			
			break;
		case TASK_OBJ_FAIRY_LV:
			if (	pstSpecialTask->FiniObjVal == pstRoleFairy->FairyID &&
				pstSpecialTask->FiniConditionType == TASK_CONDITION_LEVEL &&
				pstRoleFairy->FairyLevel >= pstSpecialTask->FiniConditionVal)
			{
				return 1;
			}
			
			break;
		
		default:
			break;
	}

	return 0;
}

int task_fairy_trace(ZONESVRENV *pstEnv, Player *pstPlayer, ROLEFAIRY *pstRoleFairy, int iTraceType)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i=pstTaskRun->TaskRunNum-1; i>=0; i--)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if (TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j=0; j<MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType == 0)
			{
				break;
			}
			
			if (pstTaskDef->SpecialTask[j].FiniObjType != iTraceType)
			{
				continue;
			}

			if (fairy_task_cond_check(&pstTaskDef->SpecialTask[j], pstRoleFairy))
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
			}
		}
	}

	return 0;
}

int z_check_task_script(ZONESVRENV* pstEnv, TASKDEF  *pstTaskDef)
{
	char szScript[32];
	ScriptIndex *pstScriptInd;
	UNUSED(pstEnv);
	
	snprintf(szScript, sizeof(szScript),"task%d.mac", pstTaskDef->ID);
	szScript[sizeof(szScript) - 1] = 0;
	
	pstScriptInd = get_script_ind(szScript);
	if (NULL == pstScriptInd)
	{
		return 0;
	}

	if (0 == check_script_proc(szScript, pstScriptInd, TASK_SCRIPT_PROC_ACCEPT))
	{
		pstTaskDef->TaskScript |= TASK_SCRIPT_PROC_ACCEPT_MAP;
	}

	if (0 == check_script_proc(szScript, pstScriptInd, TASK_SCRIPT_PROC_FINI))
	{
		pstTaskDef->TaskScript |= TASK_SCRIPT_PROC_FINI_MAP;
	}

	if (0 == check_script_proc(szScript, pstScriptInd, TASK_SCRIPT_PROC_AWARD))
	{
		pstTaskDef->TaskScript |= TASK_SCRIPT_PROC_AWARD_MAP;
	}

	if (0 == check_script_proc(szScript, pstScriptInd, TASK_SCRIPT_PROC_FAIL))
	{
		pstTaskDef->TaskScript |= TASK_SCRIPT_PROC_FAIL_MAP;
	}

	if (0 == check_script_proc(szScript, pstScriptInd, TASK_SCRIPT_PROC_DROP))
	{
		pstTaskDef->TaskScript |= TASK_SCRIPT_PROC_DROP_MAP;
	}

	if (0 == check_script_proc(szScript, pstScriptInd, TASK_SCRIPT_PROC_TIMER))
	{
		pstTaskDef->TaskScript |= TASK_SCRIPT_PROC_TIMER_MAP;
	}

	if (0 == check_script_proc(szScript, pstScriptInd, TASK_SCRIPT_PROC_TIMER2))
	{
		pstTaskDef->TaskScript |= TASK_SCRIPT_PROC_TIMER2_MAP;
	}

	if (0 == check_script_proc(szScript, pstScriptInd, TASK_SCRIPT_PROC_MOTION))
	{
		pstTaskDef->TaskScript |= TASK_SCRIPT_PROC_MOTION_MAP;
	}
	
	return 0;
}

int task_generic_progress_run(ZONESVRENV *pstEnv, Player *pstPlayer,
                              int iTaskID, int iValueAdd)
{
	TASKRUNONE *pstRun = task_run_find(pstPlayer, iTaskID);
	if(!pstRun || !iValueAdd ||
	   pstRun->TaskGenericProgress.Max <= 0 ||
	   pstRun->TaskStat == TASK_STAT_FAIL)
	{
		return -1;
	}

	if(iValueAdd < 0)
	{
		if(pstRun->TaskGenericProgress.Progress + iValueAdd < 0)
		{
			return -1;
		}
	}
	else //iValueAdd > 0
	{
		if(pstRun->TaskGenericProgress.Progress + iValueAdd >
		   pstRun->TaskGenericProgress.Max)
		{
			return -1;
		}
	}

	pstRun->TaskGenericProgress.Progress += iValueAdd;

	if(pstRun->TaskGenericProgress.Progress == pstRun->TaskGenericProgress.Max)
	{
		pstRun->TaskStat = TASK_STAT_FINI;
		task_chg(pstEnv, pstPlayer, pstRun);
	}
	else
	{
		pstRun->TaskStat = TASK_STAT_RUN;
		task_chg(pstEnv, pstPlayer, pstRun);
	}
	return 0;
}


int del_secondaryskill_task(ZONESVRENV *pstEnv, Player *pstPlayer, int iSkillType)
{
	int i;
	TASKINFO *pstTaskInfo =  &pstPlayer->stRoleData.MiscInfo.TaskInfo;
	TASKDEF *pstTaskDef;
	int iCycleID;

	for (i=pstTaskInfo->TaskRun.TaskRunNum-1;i>=0; i--)
	{
		pstTaskDef = task_def_find(pstEnv, pstTaskInfo->TaskRun.TaskRuns[i].TaskID, 
								&pstTaskInfo->TaskRun.TaskRuns[i].TaskIdx);
		if (NULL == pstTaskDef || pstTaskDef->Type != TASK_TYPE_SECONDARYSKILL)
		{
			continue;
		}

		if(pstTaskDef->TaskGetLimit.SecondarySkillType != iSkillType)
		{
			continue;
		}
		iCycleID = pstTaskInfo->TaskRun.TaskRuns[i].CycleID;
		
		pstTaskInfo->TaskRun.TaskRuns[i].TaskStat = TASK_STAT_DROP;
		task_chg(pstEnv, pstPlayer, &pstTaskInfo->TaskRun.TaskRuns[i]);
		task_run_del(pstEnv, pstPlayer, pstTaskDef->ID);	

		if (iCycleID > 0)
		{
			TASKRECYCLEONE *pstCycleOne = NULL;
			TASKRECYCLEDEF *pstTaskRecycle = NULL;

			pstCycleOne = taskcycle_run_find(pstPlayer, iCycleID);
			if (pstCycleOne)
			{
				pstTaskRecycle = taskcycle_def_find(pstEnv, iCycleID, NULL);
				if (pstTaskRecycle)
				{
					taskcycle_run_del(pstPlayer, iCycleID);
				}
			}
		}
	}

	return 0;
}


int del_task_clan(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i;
	TASKINFO *pstTaskInfo =  &pstPlayer->stRoleData.MiscInfo.TaskInfo;
	TASKDEF *pstTaskDef;
	int iDel = 0;
	int iTaskRecycleID;
	int iNotify = 0;

	for (i=pstTaskInfo->TaskRun.TaskRunNum-1;i>=0; i--)
	{
		iDel = 0;
		pstTaskDef = task_def_find(pstEnv, pstTaskInfo->TaskRun.TaskRuns[i].TaskID, 
								&pstTaskInfo->TaskRun.TaskRuns[i].TaskIdx);
		if (!pstTaskDef )
		{
			continue;
		}

		// 删除单个公会任务
		if (pstTaskDef->Type == TASK_TYPE_CLAN)
		{
			iDel = 1;
		}

		iTaskRecycleID = pstTaskInfo->TaskRun.TaskRuns[i].CycleID;
		
		if (iTaskRecycleID > 0)
		{
			TASKRECYCLEONE *pstCycleOne = NULL;
			TASKRECYCLEDEF *pstTaskRecycle = NULL;

			pstCycleOne = taskcycle_run_find(pstPlayer, iTaskRecycleID);
			if (pstCycleOne)
			{
				pstTaskRecycle = taskcycle_def_find(pstEnv, iTaskRecycleID, NULL);
				if (pstTaskRecycle && pstTaskRecycle->CycType == CYC_TYPE_CLAN)
				{
					iDel = 1;
				}
			}
		}

		if (iDel)
		{
			task_drop_check(pstEnv, pstPlayer,pstTaskDef);
			pstTaskInfo->TaskRun.TaskRuns[i].TaskStat = TASK_STAT_DROP;
			task_chg(pstEnv, pstPlayer, &pstTaskInfo->TaskRun.TaskRuns[i]);
			task_run_del(pstEnv, pstPlayer, pstTaskDef->ID);	
			iNotify = 1;
		}
	}

	for (i=pstTaskInfo->TaskRecycle.TaskRecycleNum-1; i>=0; i--)
	{
		TASKRECYCLEDEF *pstTaskRecycle = NULL;
		iTaskRecycleID = pstTaskInfo->TaskRecycle.TaskRecycles[i].TaskRecycleID;

		pstTaskRecycle = taskcycle_def_find(pstEnv, iTaskRecycleID, NULL);
		if (pstTaskRecycle && pstTaskRecycle->CycType == CYC_TYPE_CLAN)
		{
			// 环组删了 并且oneday++
			taskcycle_run_del(pstPlayer, iTaskRecycleID);
			taskcycle_add_oneday(pstEnv, pstPlayer, iTaskRecycleID);
			iNotify = 1;
		}
	}

	if (iNotify)
	{
		task_info_all(pstEnv, pstPlayer);
	}
	return 0;
}


// 升级时删除   等级不满足的任务环
int del_taskrecycle_lvlup(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i,j;
	TASKINFO *pstTaskInfo =  &pstPlayer->stRoleData.MiscInfo.TaskInfo;
	TASKRECYCLEDEF *pstTaskRecycleDef = NULL;
	int iTaskRecycleID;
	int iNotify = 0;

	for (i=pstTaskInfo->TaskRecycle.TaskRecycleNum -1; i>=0; i--)
	{
		iTaskRecycleID = pstTaskInfo->TaskRecycle.TaskRecycles[i].TaskRecycleID;
		pstTaskRecycleDef = taskcycle_def_find(pstEnv, iTaskRecycleID, NULL);
		if (!pstTaskRecycleDef)
		{
			continue;
		}

		// 等级不匹配了
		if ((pstTaskRecycleDef->MinLevel > 0 && pstPlayer->stRoleData.Level < pstTaskRecycleDef->MinLevel) ||
			(pstTaskRecycleDef->MaxLevel > 0 && pstPlayer->stRoleData.Level > pstTaskRecycleDef->MaxLevel))
		{
			for (j=0; j<pstTaskInfo->TaskRun.TaskRunNum; j++)
			{
				if (	pstTaskInfo->TaskRun.TaskRuns[j].CycleID == pstTaskInfo->TaskRecycle.TaskRecycles[i].TaskRecycleID)
				{
					break;
				}
			}

			// 当没有单个环组时  删除整个环阻任务
			if (j >= pstTaskInfo->TaskRun.TaskRunNum)
			{
				// 环组删了 并且oneday++
				taskcycle_run_del(pstPlayer, iTaskRecycleID);
				taskcycle_add_oneday(pstEnv, pstPlayer, iTaskRecycleID);
				iNotify = 1;
			}
		}
	}

	if (iNotify)
	{
		task_info_all(pstEnv, pstPlayer);
	}
	return 0;
}

/*	-lxk
	功能:完成副本任务
*/
int task_pworld_fini_trace(ZONESVRENV *pstEnv, Player *pstPlayer, unsigned int iPworldID)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i = 0; i < pstTaskRun->TaskRunNum; i++)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if (TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j = 0; j < MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType != TASK_OBJ_PWORLD)
			{
				break;
			}

			if ( pstTaskDef->SpecialTask[j].FiniObjVal == iPworldID )
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				break;
			}
		}
	}

	return 0;
}

/*	-lxk
	功能:使用XX物品后，完成任务
	注意:先检查任务完成条件[道具效果 ResultID]，否则检查任务完成对象值[ItemID]
*/
int task_use_item_fini_trace(ZONESVRENV *pstEnv, Player *pstPlayer,ITEMDEF *pstItemDef)
{
	int i,j,m,iSucc;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;
	
	for (i = 0; i < pstTaskRun->TaskRunNum; i++)
	{
		iSucc = 0;
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if (TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j = 0; j < MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType != TASK_OBJ_USE_ITEM)
			{
				break;
			}
			if(pstTaskDef->SpecialTask[j].FiniConditionType == TASK_CONDITION_ITEM_RESULTID)
			{
				/*根据任务完成条件:使用道具的效果ID*/
				for(m = 0; m < MAX_ITEM_RESULT; m++)
				{
					if(pstItemDef->Result[m].ResultID == 0)
					{
						break;
					}
					if(pstTaskDef->SpecialTask[j].FiniConditionVal == pstItemDef->Result[m].ResultID)
					{
						iSucc = 1;
						break;
					}
				}
				
			}
			else if ( pstTaskDef->SpecialTask[j].FiniObjVal == pstItemDef->ItemID)
			{
				iSucc = 1;
			}
			if(iSucc)
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				break;
			}
		}
	}

	return 0;
}

/*	-lxk
	功能:完成竞技任务
*/
int task_bat_fini_trace(ZONESVRENV *pstEnv, Player *pstPlayer,unsigned int iBattleID)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i = 0; i < pstTaskRun->TaskRunNum; i++)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if (TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j = 0; j < MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType != TASK_OBJ_FINI_BAT)
			{
				break;
			}

			if ( pstTaskDef->SpecialTask[j].FiniObjVal == iBattleID )
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				break;
			}
		}
	}

	return 0;
}

/*	-lxk
	功能:完成一天在线时长任务
	参数:
		iOnlineTime:在线时长，单位s
*/
int task_online_time_trace(ZONESVRENV *pstEnv, Player *pstPlayer,unsigned int iOnlineTime)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i = 0; i < pstTaskRun->TaskRunNum; i++)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if (TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j = 0; j < MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType != TASK_OBJ_ONLINE_TIME)
			{
				break;
			}

			if ( pstTaskDef->SpecialTask[j].FiniObjVal <= iOnlineTime )
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				break;
			}
		}
	}

	return 0;
}

/*	-lxk
	功能:当精灵星级大于等于任务条件精灵星级时，完成任务
	参数:
		iStar:精灵星级
*/
int task_fairy_star_trace(ZONESVRENV *pstEnv, Player *pstPlayer,unsigned int iStar)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i = 0; i < pstTaskRun->TaskRunNum; i++)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if (TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j = 0; j < MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType != TASK_OBJ_FAIRY_STAR)
			{
				break;
			}

			if ( pstTaskDef->SpecialTask[j].FiniObjVal <= iStar )
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				break;
			}
		}
	}

	return 0;
}

/*	-lxk
	功能:过大地之心完成任务
	参数:
		iLayer:过第XX层时完成任务[注:当iLayer=0时，表示任意层]
*/
int task_bct_layer_trace(ZONESVRENV *pstEnv, Player *pstPlayer,unsigned int iLayer)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i = 0; i < pstTaskRun->TaskRunNum; i++)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if (TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j = 0; j < MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType != TASK_OBJ_BCT_LAYER)
			{
				break;
			}

			if ( (pstTaskDef->SpecialTask[j].FiniObjVal == 0)
				|| (pstTaskDef->SpecialTask[j].FiniObjVal == iLayer) )
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				break;
			}
		}
	}

	return 0;
}

/*	-lxk
	功能:公会礼包任务
*/
int task_clan_gift_trace(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i = 0; i < pstTaskRun->TaskRunNum; i++)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if (TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j = 0; j < MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType != TASK_OBJ_CLAN_GIFT)
			{
				break;
			}

			pstTaskRunOne->TaskStat = TASK_STAT_FINI;
			task_chg(pstEnv, pstPlayer, pstTaskRunOne);
			break;
		}
	}

	return 0;
}

/*	-lxk
	功能:使用藏宝图任务
*/
int task_cangbaotu_fini_trace(ZONESVRENV *pstEnv, Player *pstPlayer, unsigned int iItemID)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i = 0; i < pstTaskRun->TaskRunNum; i++)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if (TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j = 0; j < MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType != TASK_OBJ_USE_ITEM_CANGBAOTU)
			{
				break;
			}

			pstTaskRunOne->TaskStat = TASK_STAT_FINI;
			task_chg(pstEnv, pstPlayer, pstTaskRunOne);
			break;
		}
	}
	UNUSED(iItemID);
	return 0;
}

/*	-lxk
	功能:公会驱逐任务 - 提交一次50积分就完成任务
*/
int task_clan_everyday_trace(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i = 0; i < pstTaskRun->TaskRunNum; i++)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if (TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j = 0; j < MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType != TASK_OBJ_CLAN_EVERYDAY)
			{
				break;
			}

			pstTaskRunOne->TaskStat = TASK_STAT_FINI;
			task_chg(pstEnv, pstPlayer, pstTaskRunOne);
			break;
		}
	}

	return 0;
}
/*	-lxk
	功能:完成六一任务
*/
int task_liuyi_fini_trace(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	for (i = 0; i < pstTaskRun->TaskRunNum; i++)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if (TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j = 0; j < MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType != TASK_OBJ_LIUYI)
			{
				break;
			}
			pstTaskRunOne->TaskStat = TASK_STAT_FINI;
			task_chg(pstEnv, pstPlayer, pstTaskRunOne);
			break;
		}
	}

	return 0;
}


//lzk  假设银票大于规定值。则完成任务跑商
int task_paoshang_fini(ZONESVRENV *pstEnv, Player *pstPlayer,int YinPiao)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;
	unsigned int uiYinPiao = (unsigned int)YinPiao;

	for (i = 0; i < pstTaskRun->TaskRunNum; i++)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if (TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		if(TASK_TYPE_PAOSHANG != pstTaskDef->Type)
		{
			continue;
		}

		for (j = 0; j < MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType != TASK_OBJ_PAOSHANG)
			{
				continue;
			}

			if ( pstTaskDef->SpecialTask[j].FiniObjVal <= uiYinPiao )
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				
				break;
			}
		}
	}

	return 0;
}

// lzk  砸鸡蛋鲜花等完成特殊任务
//SpecialTask1_FiniObjType	         TASK_OBJ_ZA_CHARM
//SpecialTask1_FiniObjVal            送花朵数 最少填1 ，  ≥该数就可完成任务
//SpecialTask1_FiniConditionType   0是不限制，1是加魅力的，2是减魅力的
int task_za_charm_fini(ZONESVRENV *pstEnv, Player *pstPlayer,CHARMREQ *pstCharmReq)
{
	int i,j;
	TASKDEF *pstTaskDef;
	TASKRUNONE *pstTaskRunOne;
	TASKRUN *pstTaskRun = &pstPlayer->stRoleData.MiscInfo.TaskInfo.TaskRun;

	unsigned int uiCharmNumm = (unsigned int)pstCharmReq->Num;
	
	for (i = 0; i < pstTaskRun->TaskRunNum; i++)
	{
		pstTaskRunOne = &pstTaskRun->TaskRuns[i];
		if (TASK_STAT_RUN != pstTaskRunOne->TaskStat)
		{
			continue;
		}
		
		pstTaskDef = task_def_find(pstEnv, pstTaskRunOne->TaskID, &pstTaskRunOne->TaskIdx);
		if (NULL == pstTaskDef)
		{
			continue;
		}

		for (j = 0; j < MAX_SPECIAL_TASK; j++)
		{
			if (pstTaskDef->SpecialTask[j].FiniObjType != TASK_OBJ_ZA_CHARM)
			{
				continue;
			}

			if(pstTaskDef->SpecialTask[j].FiniConditionType == TASK_CONDITION_UP_CHARM)
			{
				//加魅力类型的。就不能是砸鸡蛋等减魅力的
				if(1 == pstCharmReq->Flag)
				{
					continue;
				}
			}

			if(pstTaskDef->SpecialTask[j].FiniConditionType == TASK_CONDITION_DEC_CHARM)
			{
				if(0 == pstCharmReq->Flag)
				{
					continue;
				}
			}

			if(pstTaskDef->SpecialTask[j].FiniObjVal <= uiCharmNumm)
			{
				pstTaskRunOne->TaskStat = TASK_STAT_FINI;
				task_chg(pstEnv, pstPlayer, pstTaskRunOne);
				
				break;
			}
		}
	}

	return 0;
}

