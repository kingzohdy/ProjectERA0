/*
**  @file $RCSfile: zone_designation.c,v $
**  general description of this module
**  $Id: zone_designation.c,v 1.29 2013/09/02 07:02:01 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2013/09/02 07:02:01 $
**  @version $Revision: 1.29 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "proto_comm.h"
#include "proto_cs.h"
#include "role_db_meta.h"
#include "zone_clt.h"
#include "zone_svr.h"
#include "zone_move.h"
#include "zone_status.h"
#include "zone_package.h"
#include "zone_designation.h"
#include "zone_oplog.h"
#include "zone_attr.h"
#include "zone_err.h"

static void player_cancel_designation(ZONESVRENV* pstEnv, Player *pstPlayer);


// 查询称号定义
const DESIGNATIONDEF* z_designation_def(ZONESVRENV* pstEnv, unsigned int iID)
{
	DESIGNATIONDEF  stDummy;
	int iIndex, iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDummy.ID = iID;
	iIndex = bsearch_int(&stDummy, pstObj->sDesignationDef,
	                     pstObj->iDesignationDef, DESIGNATION_DEF_DATA, &iEqu);
	return iEqu ? (DESIGNATIONDEF *)pstObj->sDesignationDef[iIndex] : NULL;
}

// 发送角色所有称号列表
int player_designation_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	const ROLEDESIGNATIONS *pstDsn =
		&pstPlayer->stRoleData.MiscInfo.DesignationInfo;

	stPkg.Body.DesignationSvr.DesignationOp = DESIGNATION_SVR_INFO;
	stPkg.Body.DesignationSvr.DesignationData.DesignationInfo = *pstDsn;

	Z_CSHEAD_INIT(&stPkg.Head, DESIGNATION_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_DESIGNATION_SNAP;
	return 0;
}

// 称号相关视野广播消息
int player_designation_action(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	const ROLEDESIGNATIONS *pstInfo =
		&pstPlayer->stRoleData.MiscInfo.DesignationInfo;

	stPkg.Body.ActionRes.ID = pstPlayer->iMemID;
	stPkg.Body.ActionRes.ActionID = ACTION_DESIGNATION_CHG;
	stPkg.Body.ActionRes.Data.DesignationChg.DesignationID =
		pstInfo->CurrentDesignationID;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	return z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg,
	                            0);
}

// 增加角色的称号
int player_designation_add_in(ZONESVRENV* pstEnv, Player *pstPlayer,
                           unsigned int iID)
{
	int iRet = 0;
	const DESIGNATIONDEF* pstDef = NULL;
	CSPKG stPkg;
	ROLEDESIGNATIONS *pstInfo = &pstPlayer->stRoleData.MiscInfo.DesignationInfo;
	DESIGNATIONENTRY *pstDsn = player_find_designation(pstEnv, pstPlayer, iID);

	if(pstDsn)
	{
		iRet = ADD_DESIG_EXIST;
		goto error;
	}

	pstDef = z_designation_def(pstEnv, iID);
	if(!pstDef)
	{
		iRet = ADD_DESIG_NO_DEF;
		goto error;
	}

	if(pstInfo->Num >= MAX_ROLE_DESIGNATION)
	{
		iRet = ADD_DESIG_NUM_LIMIT;
		goto error;
	}

	pstDsn = &pstInfo->Designations[pstInfo->Num++];
	pstDsn->ID = iID;
	pstDsn->LastUseTime = 0;

	// --by jerry--
	pstDsn->EndTime = 0;
	if (pstDef->DesTime > 0)
	{
		pstDsn->EndTime = pstEnv->pstAppCtx->stCurr.tv_sec + pstDef->DesTime;
	}
	// -- --

	// 通知客户端，角色获得了一个新称号
	stPkg.Body.DesignationSvr.DesignationData.DesignationAdd = *pstDsn;
	stPkg.Body.DesignationSvr.DesignationOp = DESIGNATION_SVR_ADD;
	Z_CSHEAD_INIT(&stPkg.Head, DESIGNATION_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_DESIGNATION101,
	           pstDef->Name);

	// 运营日志
	z_role_designation_oplog(pstEnv, pstPlayer, pstDsn->ID, OP_DESIGNATION_GET);

final:
	return iRet;

error:
	//iRet = -1;
	goto final;
	return 0;
}

int player_designation_add(ZONESVRENV* pstEnv, Player *pstPlayer,unsigned int iID)
{
	int iRet = player_designation_add_in(pstEnv, pstPlayer,iID);
	if (g_iDesigVar == 1)
	{
		return iRet;
	}
	
	if (iRet == ADD_DESIG_NUM_LIMIT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_DESIGNATION102);
	}
	else if(ADD_DESIG_EXIST == iRet)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_DESIGNATION103);
	}
	else if(ADD_DESIG_NO_DEF == iRet)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_DESIGNATION104);
	}
	return iRet;
}

// 删除一个角色已有的称号
int player_designation_del(ZONESVRENV* pstEnv, Player *pstPlayer,
                           unsigned int iID)
{
	int iIdx = 0;
	ROLEDESIGNATIONS *pstInfo = &pstPlayer->stRoleData.MiscInfo.DesignationInfo;
	DESIGNATIONENTRY* pstEntry = player_find_designation(pstEnv, pstPlayer,
	                                                     iID);

	if(!pstEntry)
	{
		return -1;
	}

	if(pstInfo->CurrentDesignationID == iID && iID != 0)
	{
		player_designation_set(pstEnv, pstPlayer, 0);
	}

	iIdx = (int)(pstEntry - pstInfo->Designations);
	if(iIdx < (int)pstInfo->Num - 1)
	{
		memcpy(pstEntry, &pstInfo->Designations[pstInfo->Num - 1],
		       sizeof(*pstEntry));
	}

	pstInfo->Num--;

	return 0;
}


// 删除角色到期的称号,每分钟检查一下--by jerry--
int player_des_del_by_time(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	char cChg = 0;
	const DESIGNATIONDEF* pstDef;

	ROLEDESIGNATIONS *pstInfo = &pstPlayer->stRoleData.MiscInfo.DesignationInfo;

	for (i=pstInfo->Num -1; i>=0; i--)
	{
		if (	pstInfo->Designations[i].EndTime >0 && 
			pstInfo->Designations[i].EndTime < iCurr)
		{
			cChg = 1;
			if(pstInfo->CurrentDesignationID == pstInfo->Designations[i].ID)
			{
				player_designation_set(pstEnv, pstPlayer, 0);
			}

			pstDef = z_designation_def(pstEnv, pstInfo->Designations[i].ID);
			if (pstDef)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE, 
					ZONEERR_DESIGNATION105, pstDef->Name);
			}

			pstInfo->Num --;
			if ((int)pstInfo->Num != i)
			{
				pstInfo->Designations[i] = pstInfo->Designations[pstInfo->Num];	
			}
		}	
	}

	if (cChg)
	{
		 player_designation_info(pstEnv, pstPlayer);	
	}
	return 0;

	// 删除角色到期的称号--by jerry-- end
}

static void player_cancel_designation(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLEDESIGNATIONS *pstInfo = &pstPlayer->stRoleData.MiscInfo.DesignationInfo;
	/*const DESIGNATIONDEF* pstCurrentDef =
		z_designation_def(pstEnv, pstInfo->CurrentDesignationID);*/

	/*if(pstCurrentDef && pstCurrentDef->StatusID > 0)
	{
		// Buff 效果 off
		z_player_del_status(pstEnv->pstAppCtx, pstEnv, pstPlayer,
		                    pstCurrentDef->StatusID, 1, 0);
	}*/
	pstInfo->CurrentDesignationID = 0;
}

// 设置角色的称号
int player_designation_set(ZONESVRENV* pstEnv, Player *pstPlayer,
                           unsigned int iID)
{
	int iRet = 0;
	CSPKG stPkg;
	PlayerChagAttrVal stAttr;
	DESIGNATIONENTRY *pstDsn = player_find_designation(pstEnv, pstPlayer, iID);
	ROLEDESIGNATIONS *pstInfo = &pstPlayer->stRoleData.MiscInfo.DesignationInfo;
	const DESIGNATIONDEF* pstDef = z_designation_def(pstEnv, iID);
	CSDESIGNATIONSETRES *pstRes = 
		&stPkg.Body.DesignationSvr.DesignationData.DesignationSetRes;

	if(!iID)
	{
		player_cancel_designation(pstEnv, pstPlayer);
		pstRes->Succeeded = 1;
		pstRes->LastUseTime = 0;
		pstRes->DesignationID = pstInfo->CurrentDesignationID;
		goto final; // ID == 0表示取消称号
	}

	if(!pstDsn || !pstDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_DESIGNATION106);
		goto error;
	}

	/*
	if(pstDsn->ID == pstInfo->CurrentDesignationID)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "你指定的称号现在正在使用");
		goto error;
	}
	*/

	if(pstPlayer->stRoleData.RoleDetail.VIMCurr < pstDef->VimCost)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,
		           ZONEERR_DESIGNATION107);
		goto error;
	}

	// 取消原来的称号
	player_cancel_designation(pstEnv, pstPlayer);

	// 消耗精力值
	stAttr.AttrNum = 1;
	stAttr.AttrList[0].AttrID = ATTR_ID_VIM;
	stAttr.AttrList[0].nFlag = 1;
	stAttr.AttrList[0].Value.Vim = pstDef->VimCost;
	z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stAttr, 1);

	// 设置称号
	pstInfo->CurrentDesignationID = iID;
	pstRes->Succeeded = 1;
	pstRes->DesignationID = iID;

#if 0

	// -- by jerry--
	if(pstDef->StatusID > 0)
	{
		// Buff 效果 on	
		z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstPlayer,
			                  pstDef->StatusID, pstDef->StatusLevel,
			                  pstDef->Time, 1, 0);
	}
#endif
	pstDsn->LastUseTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstRes->LastUseTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	//player_designation_action(pstEnv, pstPlayer);

	// 运营日志
	z_role_designation_oplog(pstEnv, pstPlayer, pstDsn->ID, OP_DESIGNATION_USE);

final:
	player_designation_action(pstEnv, pstPlayer);
	stPkg.Body.DesignationSvr.DesignationOp = DESIGNATION_SVR_SET_RES;
	Z_CSHEAD_INIT(&stPkg.Head, DESIGNATION_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return iRet;

error:
	iRet = -1;
	pstRes->Succeeded = 0;
	pstRes->LastUseTime = 0;
	pstRes->DesignationID = pstInfo->CurrentDesignationID;
	goto final;
	return 0;
}

// 查找角色已有的称号
DESIGNATIONENTRY* player_find_designation(ZONESVRENV* pstEnv, Player *pstPlayer,
                                          unsigned int iID)
{
	ROLEDESIGNATIONS *pstInfo = &pstPlayer->stRoleData.MiscInfo.DesignationInfo;
	unsigned int i = 0;

	UNUSED(pstEnv);
	if(!iID)
	{
		return NULL;
	}

	i = pstInfo->Num;
	while(i--)
	{
		if(pstInfo->Designations[i].ID == iID)
		{
			return pstInfo->Designations + i;
		}
	}
	return NULL;
}


// 处理客户端发送过来的关于角色称号的操作。
int player_designation(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                       TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	const CSDESIGNATIONCLT *pstReq = &pstCsPkg->Body.DesignationClt;

	UNUSED(pstAppCtx);
	UNUSED(pstFrameHead);
	switch(pstReq->DesignationOp)
	{
	case DESIGNATION_CLT_SET:
		player_designation_set(pstEnv, pstPlayer,
		                       pstReq->DesignationData.DesignationSetID);
		break;

	default:
		return -1;
	}
	return 0;
}

int player_desig_get(ZONESVRENV* pstEnv, Player *pstPlayer, int iType)
{
	CHGCAREER *pstChgCareer = &pstEnv->pstGlobalObj->stGlobal.Misc.ChgCareer;
	int iDesigID = 0;
		
	if (iType == GET_DESIG_CHG_CAREER)
	{
		switch(pstPlayer->stRoleData.Career)
		{
		case CAREER_WARRIOR_DOUBLEHAND:
			if (pstChgCareer->ChgZFZ == 0)
				iDesigID = 130;
			pstChgCareer->ChgZFZ ++;	// 记一下转职人数,以防万一
			break;
		case CAREER_WARRIOR_SINGLEHAND:
			if (pstChgCareer->ChgSHZ == 0)
				iDesigID = 131;
			pstChgCareer->ChgSHZ ++;
			break;
		case CAREER_MAGICOR_SINGLEHAND:
			if (pstChgCareer->ChgJNZ== 0)
				iDesigID = 137;
			pstChgCareer->ChgJNZ ++;
			break;
		case CAREER_MAGICOR_DOUBLEHAND:
			if (pstChgCareer->ChgHLS== 0)
				iDesigID = 136;
			pstChgCareer->ChgHLS ++;
			break;
		case CAREER_GUNER_SINGLEHAND:
			if (pstChgCareer->ChgLSZ== 0)
				iDesigID = 135;
			pstChgCareer->ChgLSZ ++;
			break;
		case CAREER_GUNER_DOUBLEHAND:
			if (pstChgCareer->ChgHMZ== 0)
				iDesigID = 134;
			pstChgCareer->ChgHMZ ++;
			break;
		case CAREER_CHURCH_SINGLEHAND:
			if (pstChgCareer->ChgSYZ== 0)
				iDesigID = 133;
			pstChgCareer->ChgSYZ ++;
			break;
		case CAREER_CHURCH_DOUBLEHAND:
			if (pstChgCareer->ChgYLS== 0)
				iDesigID = 132;
			pstChgCareer->ChgYLS ++;
			break;
		default:
			break;
		}		
	}
	else if (iType == GET_DESIG_MON_KILL)
	{
		pstPlayer->stRoleData.MiscInfo.DesigVar.MonKillNum ++;
		if (pstPlayer->stRoleData.MiscInfo.DesigVar.MonKillNum == 100)
		{
			iDesigID = 138;	
		}
		else if (pstPlayer->stRoleData.MiscInfo.DesigVar.MonKillNum == 200)
		{
			iDesigID = 150;	
			pstPlayer->stRoleData.MiscInfo.DesigVar.MonKillNum = 0;
		}
	}
	else if (iType == GET_DESIG_KILL_DUDUNIAO)
	{
		/*pstPlayer->stRoleData.MiscInfo.DesigVar.KillDuDuNiaoNum ++;
		if (pstPlayer->stRoleData.MiscInfo.DesigVar.KillDuDuNiaoNum == 5000 )
		{
			iDesigID = 147;	
			pstPlayer->stRoleData.MiscInfo.DesigVar.KillDuDuNiaoNum = 0;
		}*/
	}
	else if (iType == GET_DESIF_ENTER_JAIN_NUM)
	{
		pstPlayer->stRoleData.MiscInfo.DesigVar.EnterJailNum ++;
		if (pstPlayer->stRoleData.MiscInfo.DesigVar.EnterJailNum ==100)
		{
			iDesigID = 145;	
			pstPlayer->stRoleData.MiscInfo.DesigVar.EnterJailNum = 0;
		}
	}
	else if (iType == GET_DESIG_USE_FOOD)
	{
		pstPlayer->stRoleData.MiscInfo.DesigVar.UseFoodNum ++;
		if (pstPlayer->stRoleData.MiscInfo.DesigVar.UseFoodNum == 5000)
		{
			if (pstPlayer->stRoleData.Gender == GENDER_MAN)
			{
				iDesigID = 143;	
			}
			else
			{
				iDesigID = 144;	
			}
			pstPlayer->stRoleData.MiscInfo.DesigVar.UseFoodNum = 0;
		}
		
	}
	else if(GET_DESIG_FRIEND_NUM == iType)
	{
		if (pstPlayer->stRoleData.Buddy.FriendNum == 60)
		{
			iDesigID = 139;		
		}
	}
	else if (GET_DESIG_CUIDI_PAOYOU == iType)
	{
		pstPlayer->stRoleData.MiscInfo.DesigVar.Var1 ++;
		if (pstPlayer->stRoleData.MiscInfo.DesigVar.Var1 == 100)
		{
			iDesigID = 151;	
			pstPlayer->stRoleData.MiscInfo.DesigVar.Var1 = 0;
		}
	}
	else if (GET_DESIG_EXP_99999 == iType)
	{
		if (pstPlayer->stRoleData.RoleDetail.Exp % 100000 == 99999)
		{
			iDesigID = 146;			
		}
	}
	else if (GET_DESIG_RIDE_STREN == iType)
	{
		iDesigID = 148;		
	}
	else if (GET_DESIG_WATCH == iType)
	{
		pstPlayer->stRoleData.MiscInfo.DesigVar.Var2 ++;
		if (pstPlayer->stRoleData.MiscInfo.DesigVar.Var2 == 100)
		{
			iDesigID = 152;
			pstPlayer->stRoleData.MiscInfo.DesigVar.Var2 = 0;
		}
	}
	
	if (iDesigID > 0)
	{
		g_iDesigVar = 1;
		player_designation_add( pstEnv,pstPlayer,iDesigID)	;
		g_iDesigVar = 0;
	}

	
	return 0;
}


