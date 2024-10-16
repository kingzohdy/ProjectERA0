/*
**  @file $RCSfile: zone_coroutine.c,v $
**  general description of this module
**  $Id: zone_coroutine.c,v 1.3 2013/09/06 03:50:37 tjw Exp $
**  @author $Author: tjw $
**  @date $Date: 2013/09/06 03:50:37 $
**  @version $Revision: 1.3 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "zone_player.h"
#include "tsec/tsecbasedef.h"
#include "tconnapi.h"
#include "comm/shtable.h"
#include "zone_clt.h"
#include "zone_db.h"
#include "zone_map.h"
#include "zone_move.h"
#include "mapmask.h"
#include "zone_act_status.h"
#include "libdirty.h"
#include "comm_misc.h"
#include "zone_skill.h"
#include "zone_status.h"
#include "zone_attr.h"
#include "zone_span.h"
#include "zone_team.h"
#include "zone_clan.h"
#include "zone_clt.h"
#include "zone_oplog.h"
#include "zone_mail.h"
#include "zone_machine.h"
#include "zone_gm.h"
#include "zone_coroutine.h"

/*
**   pstPlayer 可能是伪造的， 如上线时访问中立国进程时那边就没有Player, 
**    但回来时RoleName 依然要带回来， 所以就先伪造了一个
*/
int player_rpc_call(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	COROUTINERPC *pstRpc = &pstEnv->stCoroutineRPC;
	int iOldFromID = pstRpc->FromID;
	//SSPKG stPkg;	//comment by paraunused.pl
	
	pstRpc->Step++; 
	pstRpc->FromID = pstEnv->pstAppCtx->iId;
	if (pstPlayer)
	{
		STRNCPY(pstRpc->RoleName,pstPlayer->stRoleData.RoleName,sizeof(pstRpc->RoleName));
	}
	else
	{
		pstRpc->RoleName[0] = 0;
	}

	if (pstRpc->Flag & CO_FLAG_RET)
	{
		pstRpc->DstID = iOldFromID;
		pstRpc->Flag &= ~CO_FLAG_RET;
	}
	else
	{
		pstRpc->Flag |= CO_FLAG_RET;
	}

	if (iOldFromID)
	{
		pstRpc->DstID = iOldFromID;
	}

	if (0 == pstRpc->DstID)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "DstID err:%d",pstRpc->DstID);
		return -1;
	}

	//发协议
	z_ss_coroutine_rpc_req(pstEnv->pstAppCtx,pstEnv,pstRpc);
	
	return 0;
}

int co_init(COROUTINERPC *pstRpc)
{
	pstRpc->Type = 0;
	pstRpc->Step = 0;
	pstRpc->RoleName[0] = 0;
	pstRpc->FromID = 0;
	pstRpc->DstID = 0;
	pstRpc->Flag = 0;
	return 0;
}

int db_co_reset(ZONESVRENV* pstEnv)
{
	DBMetaInfo *pstMetaInfo = &pstEnv->stDBMetaInfo;
	DBCOROUTINERPC *pstRpc = &pstEnv->stDBCoroutineRPC;
	
	pstRpc->Type = 0;
	pstRpc->TormCmd = 0;
	pstRpc->Flag |= DB_CO_FLAG_REQ;
	
	pstMetaInfo->szMetaName[0] = 0;
	pstMetaInfo->iDoInsert = 0;
	pstMetaInfo->iObjSize = 0;
	pstMetaInfo->pszObj = NULL;
	pstMetaInfo->szWhere[0] = 0;
	pstMetaInfo->iUin = -1;
	pstMetaInfo->iDBType = 0;
	
	return 0;
}

int db_co_init(ZONESVRENV* pstEnv)
{
	DBCOROUTINERPC *pstRpc = &pstEnv->stDBCoroutineRPC;

	db_co_reset(pstEnv);

	pstRpc->Flag |= DB_CO_FLAG_REQ;
	
	return 0;
}



int db_rpc_call_in(ZONESVRENV* pstEnv,DBCOROUTINERPC *pstRpc, DBMetaInfo *pstMetaInfo)
{
	DBAsyn stDBAsyn;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	TORMINSERTREQ *pstInsertReq = &pstPkg->Body.InsertReq;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	TORMDELETEREQ *pstDeleteReq = &pstPkg->Body.DeleteReq;

	int iDBID = 0;
	int iDBProtoVer = 0;

	if (!pstMetaInfo->szMetaName[0] || !pstRpc->TormCmd ||
		!pstMetaInfo->pszObj || !pstMetaInfo->iObjSize)
	{
		return -1;
	}

	switch(pstMetaInfo->iDBType)
	{
	case DB_CO_DB_TYPE_MISC:
		iDBID = pstEnv->iMiscDBID;
		iDBProtoVer = pstEnv->iMiscDBProtoVer;
		break;
	case DB_CO_DB_TYPE_ROLE:
		iDBID = pstEnv->iRoleDBID;
		iDBProtoVer = pstEnv->iRoleDBProtoVer;
		break;
	case DB_CO_DB_TYPE_ACCOUNT:
		iDBID = pstEnv->iAccountDBID;
		iDBProtoVer = pstEnv->iAccountDBProtoVer;
		break;
	case DB_CO_DB_TYPE_REGION:
		iDBID = pstEnv->iRegionDBID;
		iDBProtoVer = pstEnv->iRegionDBProtoVer;
		break;
	default:
		tlog_error(pstEnv->pstLogCat, 0, 0,"db_rpc_call_in DBType error DBType=%d", pstMetaInfo->iDBType);
		return -1;
		break;
	}

	stDBAsyn.iCmd = pstRpc->Type;
	//todo:
	//stDBAsyn.OpType = CO_OP;
	stDBAsyn.AsynCmd.stDBCoroutineRpc = *pstRpc;
	
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, pstRpc->TormCmd,
	                 pstMetaInfo->szMetaName, pstMetaInfo->iUin, &stDBAsyn);

	if ( TORM_SELECT_REQ == pstRpc->TormCmd)
	{
		pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
		if (pstMetaInfo->szWhere[0])
		{
			STRNCPY(pstSelectReq->OperInfo.WhereDef,pstMetaInfo->szWhere,sizeof(pstSelectReq->OperInfo.WhereDef));
		}
		pstSelectReq->OffSet = 0;
		pstSelectReq->Limit = 0;
	}
	else if (TORM_UPDATE_REQ == pstRpc->TormCmd)
	{
		pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;  /*离线更换国王时需要update 返回*/
		pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
		pstUpdateReq->OperInfo.WhereDef[0] = 0;
		pstUpdateReq->DoInsert = pstMetaInfo->iDoInsert; 
	}
	else if (TORM_INSERT_REQ == pstRpc->TormCmd)
	{
		pstInsertReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstInsertReq->OperInfo.EntryNameList.EntryNameNum = 0;
		pstInsertReq->OperInfo.WhereDef[0] = 0;
	}
	else if (TORM_DELETE_REQ == pstRpc->TormCmd)
	{
		pstDeleteReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
		pstDeleteReq->OperInfo.EntryNameList.EntryNameNum = 0;
		pstDeleteReq->OperInfo.WhereDef[0] = 0;
	}
	else
	{
		return -1;
	}


	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = pstMetaInfo->pszObj;
	stMetaData.iBuff = pstMetaInfo->iObjSize;
	if (0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, iDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, iDBID))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_torm_send fail");
		return -1;
	}
	
	return 0;
}

int db_rpc_call(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	DBCOROUTINERPC *pstRpc = &pstEnv->stDBCoroutineRPC;
	DBMetaInfo *pstMetaInfo = &pstEnv->stDBMetaInfo;

	if (! (pstRpc->Flag & DB_CO_FLAG_REQ))
	{
		return 0;
	}

	if (pstPlayer)
	{
		pstMetaInfo->iUin = pstPlayer->stRoleData.Uin;
	}
	else
	{
		pstMetaInfo->iUin = -1;
	}

	//发协议
	db_rpc_call_in(pstEnv,pstRpc,pstMetaInfo);
	
	return 0;
}




//异步踢分线并处理返回的消息
int player_kickoff_co(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	COROUTINERPC *pstRpc = &pstEnv->stCoroutineRPC;
	
	if (!pstPlayer)
	{
		return 0;
	}
	CO_BEGIN
	{
		//主线代码
		
		if (pstPlayer)
		{
			SpanOL *pstSpan = &pstPlayer->stOnline.stSpanOnline;
			pstRpc->Type = CO_TYPE_KICKOFF;
			if (pstSpan->iStat != SPAN_STAT_CONNECT)
			{
				return -1;
			}
			pstRpc->DstID = pstSpan->stInfo.stConnect.iDstID;
		}
	}
	CO_YIELD(1)
	{
		//分线代码
		COROUTINERPC *pstRpc = &pstEnv->stCoroutineRPC;
		if (pstPlayer)
		{
			pstRpc->Ret = 0;//player_kickoff(pstEnv->pstAppCtx,pstEnv,pstPlayer,NULL);
		}
		else
		{
			pstRpc->Ret = -1;
		}
		
	}
	CO_YIELD(2)
	{
		//主线代码
		
	}
	CO_END

	return 0;
}




int z_ss_coroutine_rpc(ZONESVRENV * pstEnv,COROUTINERPC *pstCoroutineRpc)
{
	COROUTINERPC *pstRpc = &pstEnv->stCoroutineRPC;
	Player * pstPlayer = NULL;
	 
	(*pstRpc) = (*pstCoroutineRpc);

	if (pstRpc->RoleName[0])
	{
		pstPlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,pstRpc->RoleName);
	}

	switch(pstRpc->Type)
	{
	case CO_TYPE_NULL:
		tlog_error(pstEnv->pstLogCat, 0, 0, "type is null");
		break;
	case CO_TYPE_KICKOFF:
		break;
	case CO_TYPE_CLAN_BATTLE_AWARD:
		clan_battle_award_co(pstEnv,NULL, NULL, 0);
		break;
	default:
		break;
	}

	co_init(pstRpc);
	
	return 0;
}


int z_db_coroutine_rpc(ZONESVRENV * pstEnv,DBCOROUTINERPC *pstDBCoroutineRpc,TORMPKG *pstPkg,Player * pstPlayer)
{
	DBCOROUTINERPC *pstRpc = &pstEnv->stDBCoroutineRPC;
	 
	(*pstRpc) = (*pstDBCoroutineRpc);

	pstRpc->TormCmd = pstPkg->Head.HeadCmd.Cmd;


	switch(pstRpc->Type)
	{
	case DB_CO_TYPE_NULL:
		tlog_error(pstEnv->pstLogCat, 0, 0, "type is null");
		break;
		

	default:
		break;
	}

	db_co_reset(pstEnv);
	
	return 0;
	UNUSED(pstPlayer);	//add by paraunused.pl
}
		
