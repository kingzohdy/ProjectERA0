/*
**  @file $RCSfile: zone_act_status.c,v $
**  general description of this module
**  $Id: zone_act_status.c,v 1.24 2013/05/22 10:12:19 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2013/05/22 10:12:19 $
**  @version $Revision: 1.24 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_act_status.h"
#include "zone_err.h"
#include "zone_clt.h"
#include "zone_zh_name.h"

char* s_action[]	=
{
 LJY_ACT_MOVE,         
 LJY_ACT_SIT,	        
 LJY_ACT_PREPARE, 
 LJY_ACT_ATK,	        
 LJY_ACT_NPCTALK,	 
 LJY_ACT_WEAR,         
 LJY_ACT_TRANS,	 
 LJY_ACT_USEITEM,     
 LJY_ACT_EXCHG,	 
 LJY_ACT_RIDE ,   	 
 LJY_ACT_STALL	,        
 LJY_ACT_ARM ,       	 
 LJY_ACT_RIDE_MACHINE, 
};
#define ACTION_MAP_SIZE			(sizeof(s_action)/sizeof(s_action[0]))

typedef struct tagActionMap
{
	int  iStatusValNo; //动作对应的不允许状态	 
	int  iStatusValClear; //动作对应的清除状态	
}ActionMap;

static ActionMap g_sastActionMap[RES_STATUS_MAX];

int z_player_act_test(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, int iAction)
{
	if (RES_ACTION_MIN > iAction || RES_ACTION_MAX < iAction)
	{
		return -1;
	}

	if (pstPlayer->stOnline.State & g_sastActionMap[iAction].iStatusValNo)
	{
		//tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player action=%d test status fail", pstPlayer->stRoleData.Uin, iAction);
		return 0;
	}
	
	return 1;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
}

int z_player_act_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer, int iAction)
{
	if (RES_ACTION_MIN > iAction || RES_ACTION_MAX < iAction)
	{
		return 0;
	}

	if (pstPlayer->stOnline.State & g_sastActionMap[iAction].iStatusValNo)
	{
		//tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player action=%d check status fail", pstPlayer->stRoleData.Uin, iAction);

		if (iAction >= 0 && iAction < (int)ACTION_MAP_SIZE)
		{
			//死亡就不提示了。交给客户端提示，因为死亡客户端表现有可能延时。
			if(!(pstPlayer->stOnline.State & CS_STAT_DEAD))
			{
				z_send_sysmsg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_ACTION_NOT, s_action[iAction]);
			}
		}
		
		return 0;
	}

	pstPlayer->stOnline.State &= ~g_sastActionMap[iAction].iStatusValClear;
	
	return 1;
}

int z_act_status_init(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	int i;
	ACTIONSTATUS *pstActionStatus;
	int iStatus;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	memset(g_sastActionMap, 0, sizeof(g_sastActionMap));

	for (i=0; i<pstObj->iActionStatusDef; i++)
	{
		if (pstObj->iActionStatusDef >= RES_STATUS_MAX)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "pstObj->iActionStatusDef=%d", pstObj->iActionStatusDef );
			return -1;
		}

		pstActionStatus = (ACTIONSTATUS *)pstObj->sActionStatusDef[i];
		iStatus = 1<<pstActionStatus->StatusIdx;

		if (ACTION_STATUS_NO == pstActionStatus->Walk)
		{
			g_sastActionMap[RES_ACTION_WALK].iStatusValNo |= iStatus;
		}
		else if (ACTION_STATUS_YES_CLEAR == pstActionStatus->Walk)
		{
			g_sastActionMap[RES_ACTION_WALK].iStatusValClear |= iStatus;
		}

		if (ACTION_STATUS_NO == pstActionStatus->Sit)
		{
			g_sastActionMap[RES_ACTION_SIT].iStatusValNo |= iStatus;
		}
		else if (ACTION_STATUS_YES_CLEAR == pstActionStatus->Sit)
		{
			g_sastActionMap[RES_ACTION_SIT].iStatusValClear |= iStatus;
		}

		if (ACTION_STATUS_NO == pstActionStatus->Prepare)
		{
			g_sastActionMap[RES_ACTION_PREPARE].iStatusValNo |= iStatus;
		}
		else if (ACTION_STATUS_YES_CLEAR == pstActionStatus->Prepare)
		{
			g_sastActionMap[RES_ACTION_PREPARE].iStatusValClear |= iStatus;
		}

		if (ACTION_STATUS_NO == pstActionStatus->Atk)
		{
			g_sastActionMap[RES_ACTION_ATK].iStatusValNo |= iStatus;
		}
		else if (ACTION_STATUS_YES_CLEAR == pstActionStatus->Atk)
		{
			g_sastActionMap[RES_ACTION_ATK].iStatusValClear |= iStatus;
		}

		if (ACTION_STATUS_NO == pstActionStatus->Dialog)
		{
			g_sastActionMap[RES_ACTION_DIALOG].iStatusValNo |= iStatus;
		}
		else if (ACTION_STATUS_YES_CLEAR == pstActionStatus->Dialog)
		{
			g_sastActionMap[RES_ACTION_DIALOG].iStatusValClear |= iStatus;
		}

		if (ACTION_STATUS_NO == pstActionStatus->Wear)
		{
			g_sastActionMap[RES_ACTION_WEAR].iStatusValNo |= iStatus;
		}
		else if (ACTION_STATUS_YES_CLEAR == pstActionStatus->Wear)
		{
			g_sastActionMap[RES_ACTION_WEAR].iStatusValClear |= iStatus;
		}

		if (ACTION_STATUS_NO == pstActionStatus->Trans)
		{
			g_sastActionMap[RES_ACTION_TRANS].iStatusValNo |= iStatus;
		}
		else if (ACTION_STATUS_YES_CLEAR == pstActionStatus->Trans)
		{
			g_sastActionMap[RES_ACTION_TRANS].iStatusValClear |= iStatus;
		}

		if (ACTION_STATUS_NO == pstActionStatus->UseItem)
		{
			g_sastActionMap[RES_ACTION_USEITEM].iStatusValNo |= iStatus;
		}
		else if (ACTION_STATUS_YES_CLEAR == pstActionStatus->UseItem)
		{
			g_sastActionMap[RES_ACTION_USEITEM].iStatusValClear |= iStatus;
		}

		if (ACTION_STATUS_NO == pstActionStatus->Exchg)
		{
			g_sastActionMap[RES_ACTION_EXCHG].iStatusValNo |= iStatus;
		}
		else if (ACTION_STATUS_YES_CLEAR == pstActionStatus->Exchg)
		{
			g_sastActionMap[RES_ACTION_EXCHG].iStatusValClear |= iStatus;
		}

		if (ACTION_STATUS_NO == pstActionStatus->Ride)
		{
			g_sastActionMap[RES_ACTION_RIDE].iStatusValNo |= iStatus;
		}
		else if (ACTION_STATUS_YES_CLEAR == pstActionStatus->Ride)
		{
			g_sastActionMap[RES_ACTION_RIDE].iStatusValClear |= iStatus;
		}

		if (ACTION_STATUS_NO == pstActionStatus->Stall)
		{
			g_sastActionMap[RES_ACTION_STALL].iStatusValNo |= iStatus;
		}
		else if (ACTION_STATUS_YES_CLEAR == pstActionStatus->Stall)
		{
			g_sastActionMap[RES_ACTION_STALL].iStatusValClear |= iStatus;
		}

		if (ACTION_STATUS_NO == pstActionStatus->Machining)
		{
			g_sastActionMap[RES_ACTION_MACHINING].iStatusValNo |= iStatus;
		}
		else if (ACTION_STATUS_YES_CLEAR == pstActionStatus->Machining)
		{
			g_sastActionMap[RES_ACTION_MACHINING].iStatusValClear |= iStatus;
		}

		if (ACTION_STATUS_NO == pstActionStatus->RideMachine)
		{
			g_sastActionMap[RES_ACTION_RIDE_MACHINE].iStatusValNo |= iStatus;
		}
		else if (ACTION_STATUS_YES_CLEAR == pstActionStatus->RideMachine)
		{
			g_sastActionMap[RES_ACTION_RIDE_MACHINE].iStatusValClear |= iStatus;
		}
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

