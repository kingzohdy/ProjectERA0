/*
**  @file $RCSfile: zone_respact.c,v $
**  general description of this module
**  $Id: zone_respact.c,v 1.23 2013/06/26 03:20:33 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2013/06/26 03:20:33 $
**  @version $Revision: 1.23 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "proto_comm.h"
#include "proto_cs.h"
#include "role_db_meta.h"
#include "zone_clt.h"
#include "zone_svr.h"
#include "zone_task.h"
#include "zone_oplog.h"
#include "zone_err.h"
#include "zone_zh_name.h"

const RESPACTPARTYDEF* z_respact_party_def(ZONESVRENV *pstEnv, unsigned int nID)
{
	RESPACTPARTYDEF  stDummy;
	int iIndex, iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDummy.ID = nID;
	iIndex = bsearch_int(&stDummy, pstObj->sRespactPartyDef,
	                     pstObj->iRespactPartyDef, RESPACT_PARTY_DEF_DATA,
	                     &iEqu);
	return iEqu ? (RESPACTPARTYDEF *)pstObj->sRespactPartyDef[iIndex] : NULL;
}

const RESPACTPHASEDEF* z_respact_phase_def(ZONESVRENV *pstEnv,
                                           unsigned int nPhase)
{
	RESPACTPHASEDEF  stDummy;
	int iIndex, iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDummy.Phase = nPhase;
	iIndex = bsearch_int(&stDummy, pstObj->sRespactPhaseDef,
	                     pstObj->iRespactPhaseDef, RESPACT_PHASE_DEF_DATA,
	                     &iEqu);
	return iEqu ? (RESPACTPHASEDEF *)pstObj->sRespactPhaseDef[iIndex] : NULL;
}

// ��������ֱֵ�ӵõ���ǰ�������׶ζ���
const RESPACTPHASEDEF* z_respact_phase_refer_value(ZONESVRENV *pstEnv,
                                                   unsigned int nRespactValue)
{
	const ZoneObj *pstObj = pstEnv->pstZoneObj;
	int i = pstObj->iRespactPhaseDef;

	while(i--)
	{
		const RESPACTPHASEDEF* pstDef = 
			(RESPACTPHASEDEF *)pstObj->sRespactPhaseDef[i];
		if(nRespactValue >= pstDef->NeedRespact)
		{
			return pstDef;
		}
	}

	return NULL;
}

// ���ҽ�ɫ��������Ϣ
RESPACTENTRY* player_find_respact(ZONESVRENV *pstEnv, Player *pstPlayer,
                                  unsigned int nID)
{
	RESPACTINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.RespactInfo;
	int i = pstInfo->RespactPartyNum;
	UNUSED(pstEnv);

	while(i--)
	{
		if(pstInfo->Respact[i].ID == nID)
		{
			return (pstInfo->Respact + i);
		}
	}
	return NULL;
}

// ���ҽ�ɫ������Ϣ���Ҳ������½�һ��
RESPACTENTRY* player_find_or_new_respact(ZONESVRENV *pstEnv, Player *pstPlayer,
                                         unsigned int nID)
{
	RESPACTINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.RespactInfo;
	RESPACTENTRY *pstEntry = player_find_respact(pstEnv, pstPlayer, nID);

	if(!pstEntry)
	{
		if(pstInfo->RespactPartyNum >= MAX_RESPACT_PARTY_NUM)
		{
			return NULL;
		}
		pstEntry = &pstInfo->Respact[pstInfo->RespactPartyNum++];
		pstEntry->ID = nID;
		pstEntry->Value = 0;
	}
	return pstEntry;
}

// ��¼ʱ���͸��ͻ��˵�������Ϣ����
int player_respact_info(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSRESPACTSVR *pstSvr = &stPkg.Body.RespactSvr;

	pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_RESPACT_SNAP;

	pstSvr->Operation = RESPACT_SVR_INFO;
	pstSvr->RespactSvrData.RespactInfo =
		pstPlayer->stRoleData.MiscInfo.RespactInfo;

	Z_CSHEAD_INIT(&stPkg.Head, RESPACT_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

// ��ɫ�������仯��֪ͨ
int player_respact_chg(ZONESVRENV *pstEnv, Player *pstPlayer, unsigned int nID)
{
	CSPKG stPkg;
	CSRESPACTSVR *pstSvr = &stPkg.Body.RespactSvr;
	const RESPACTENTRY *pstEntry = player_find_respact(pstEnv, pstPlayer, nID);

	if(!pstEntry)
	{
		return -1;
	}

	pstSvr->Operation = RESPACT_SVR_CHG;
	pstSvr->RespactSvrData.RespactChg = *pstEntry;

	Z_CSHEAD_INIT(&stPkg.Head, RESPACT_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

// ɾ��һ����ɫ������Ϣ
int player_respact_del(ZONESVRENV *pstEnv, Player *pstPlayer, unsigned int nID)
{
	int iIdx = 0;
	RESPACTINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.RespactInfo;
	RESPACTENTRY *pstEntry = player_find_respact(pstEnv, pstPlayer, nID);

	if(!pstEntry)
	{
		return -1;
	}

	iIdx = (int)(pstEntry - pstInfo->Respact);
	if(iIdx < pstInfo->RespactPartyNum - 1)
	{
		memcpy(pstEntry, &pstInfo->Respact[pstInfo->RespactPartyNum - 1],
		       sizeof(*pstEntry));
	}

	pstInfo->RespactPartyNum--;
	return 0;
}

// ��ɫ����������
int player_respact_inc(ZONESVRENV *pstEnv, Player *pstPlayer, unsigned int nID,
                       unsigned int nValue, int fInCombat)
{
	const RESPACTPHASEDEF *pstLast = NULL, *pstCurr = NULL;
	RESPACTENTRY *pstEntry = NULL;
	const RESPACTPARTYDEF *pstPartyDef = NULL;
	unsigned int nIncreasedValue = 0;

	//ս����õķ�����
	if(fInCombat)
	{
		nValue = nValue *(1-pstPlayer->bAasIncomeLimt/100.0);
	}

	pstPartyDef = z_respact_party_def(pstEnv, nID);
	if(!pstPartyDef)
	{
		return -1; // ��Ϸ�и�����û�������Ӫ��
	}

	pstEntry = player_find_or_new_respact(pstEnv, pstPlayer, nID);
	if(!pstEntry)
	{
		return -1;
	}
	pstLast = z_respact_phase_refer_value(pstEnv, pstEntry->Value);
	if(!pstLast)
	{
		return -1;
	}
	if(pstEntry->Value + nValue >= MAX_RESPACT_VALUE)
	{
		if(pstEntry->Value > MAX_RESPACT_VALUE)
		{
			pstEntry->Value = MAX_RESPACT_VALUE;
		}
		nIncreasedValue = MAX_RESPACT_VALUE - pstEntry->Value;
		pstEntry->Value += nIncreasedValue;
	}
	else
	{
		nIncreasedValue = nValue;
		pstEntry->Value += nIncreasedValue;
	}

	pstCurr = z_respact_phase_refer_value(pstEnv, pstEntry->Value);
	if(!pstCurr)
	{
		return -1;
	}

	// �������
	if(pstPlayer->stOnline.cMiscFlag & MISC_FLAG_RESPACT_TRACE)
	{
		task_player_trace(pstEnv, pstPlayer, TASK_OBJ_RESPACT);
	}

	// �����仯֪ͨ
	if(nIncreasedValue)
	{
		player_respact_chg(pstEnv, pstPlayer, nID);

		if(pstPlayer->bAasIncomeLimt > 0 && fInCombat)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_RESPACT2, pstPartyDef->PartyName, nIncreasedValue);
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_RESPACT2, pstPartyDef->PartyName, nIncreasedValue);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_COMBAT, ZONEERR_RESPACT1, pstPartyDef->PartyName, nIncreasedValue);
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_RESPACT1, pstPartyDef->PartyName, nIncreasedValue);
		}
	}

	// �����ǲ�������������һ/N���׶�
	if(pstCurr->NeedRespact <= pstLast->NeedRespact)
	{
		return 0;
	}
	
	// ֪ͨ
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SHOW, 
						ZONEERR_RESPACT3, pstPartyDef->PartyName, pstCurr->PhaseText);

	// �㲥֪ͨȫ��
	if(pstCurr->NotifyWorld)
	{
		CSSYSMSG *pstMsg = NULL;
		CSPKG *pstCSRes = &pstEnv->stCSRes;
	
		Z_CSHEAD_INIT(&pstCSRes->Head, SYS_MSG);
		pstMsg = &pstCSRes->Body.SysMsg;
		pstMsg->Type = SYS_SHOW;
		pstMsg->Times = 0;
		pstMsg->Interval = 0;
		pstMsg->LinkLen = 0;
		snprintf(pstMsg->Msg, CCH(pstMsg->Msg), LJY_SYSMSG19,
		         pstPlayer->stRoleData.RoleName, pstPartyDef->PartyName,
		         pstCurr->PhaseText);
		pstMsg->Msg[sizeof(pstMsg->Msg) - 1] = 0;
		z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, pstCSRes);
	}

	z_role_respact_oplog(pstEnv, pstPlayer, pstEntry, OP_RESPACT_PHASE);
	return 0;
}

// ����ֵ���
int player_respact_value_check(ZONESVRENV *pstEnv, Player *pstPlayer,
                               const RESPACTENTRY *pstNeed)
{
	const RESPACTENTRY *pstEntry = NULL;

	if(!pstNeed->ID)
	{
		return 0; // û������Ҫ��
	}

	pstEntry = player_find_respact(pstEnv, pstPlayer, pstNeed->ID);
	if(!pstEntry)
	{
		return -1;
	}
	if(pstEntry->Value < pstNeed->Value)
	{
		return -1;
	}	
	return 0;
}
