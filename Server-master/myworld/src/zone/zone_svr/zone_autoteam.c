/*
**  @file $RCSfile: zone_autoteam.c,v $
**  general description of this module
**  $Id: zone_autoteam.c,v 1.228 2012/12/13 03:19:36 eric Exp $
**  @author $Author: eric $
**  @date $Date: 2012/12/13 03:19:36 $
**  @version $Revision: 1.228 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#if 0
#include "zone_autoteam.h"
#include "zone_team.h"
#include "zone_clt.h"
#include "zone_map.h"
#include "zone_combat.h"
#include "zone_status.h"
#include "zone_diffpworld.h"
#include "zone_player.h"
#include "zone_spansvr.h"
#include "zone_battle.h"

typedef struct
{
	int iNum; //可组的人数
	time_t tTime;
	int iIdx;
	short nLimitFlag;

	AutoTeamPre stTeamPre;
	AutoTeamCreated stTeamCreated;
}AutoTeamPreIdx;

#define AUTOTEAM_OP_AGREE 1
#define AUTOTEAM_OP_CREATE 2
#define AUTOTEAM_OP_CREATE_AGREE 4

int autoteam_team_del_id(AutoTeamOne *pstAutoTeamOne, int iTeamID);

int autoteam_pre_loose_limit(ZONESVRENV *pstEnv, AutoTeamOne *pstAutoTeamOne, AutoTeamPre *pstAutoTeamPre, int iAutoTeamID);

int autoteam_init(ZONESVRENV* pstEnv)
{
	int i;
	AutoTeam *pstAutoTeam = &pstEnv->pstZoneObj->stAutoTeam;

	for (i=0; i<AUTO_TEAM_ID_MAX; i++)
	{
		pstAutoTeam->astAutoTeams[i].iCreatedTeamNum = 0;
		pstAutoTeam->astAutoTeams[i].iPreTeamNum = 0;
		pstAutoTeam->astAutoTeams[i].iSingleNum = 0;
	}
	
	return 0;
}

int autoteam_career_co(short nLimitFlag, int iTeamMember, unsigned char *pbCareer)
{
	int iNoCoNum = 0; //未符合职业的人数
	int iCanFillNum = MAX_TEAM_MEMBER - iTeamMember;

	if ((AUTO_TEAM_LIMIT_WARRIOR & nLimitFlag) && 0 == pbCareer[CAREER_WARRIOR - 1])
	{
		iNoCoNum++;
	}

	if ((AUTO_TEAM_LIMIT_MAGICOR & nLimitFlag) && 0 == pbCareer[CAREER_MAGICOR - 1])
	{
		iNoCoNum++;
	}

	if ((AUTO_TEAM_LIMIT_GUNER & nLimitFlag) && 0 == pbCareer[CAREER_GUNER - 1])
	{
		iNoCoNum++;
	}

	if ((AUTO_TEAM_LIMIT_CHUNCH & nLimitFlag) && 0 == pbCareer[CAREER_CHURCH - 1])
	{
		iNoCoNum++;
	}

	if (iNoCoNum <= iCanFillNum)
	{
		return 1;
	}
	
	return 0;
}

int autoteam_level_team_player(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, 
									Player *pstPlayer,int AutoTeamID)
{
	 AutoTeamInfo *pstAutoTeamInfo;
	int i;
	int iDiffLevelLimit;
	int iMinLevel;
	int iMaxLevel;
	int iTmpMaxLevel;
	ACTIVEHELPER *pstActiveHelper = NULL;
	DIFFPWORLDDEF *pstDiffPworldDef = NULL;

	pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;
	pstActiveHelper = find_active_by_autoteam(pstEnv, AutoTeamID);
	if(pstActiveHelper)
	{
		pstDiffPworldDef = z_find_diff_pworld_def(pstEnv,pstActiveHelper->TaskPworldID);
	}
	if(pstDiffPworldDef && pstDiffPworldDef->PworldInfo[1].PworldID==0)
	{
		if (pstEnv->pstAppCtx->stCurr.tv_sec - pstAutoTeamInfo->tStart >= LIMIT_LOOSE_TIME)
		{
			iDiffLevelLimit = LIMIT_LEVEL_LOOSE_DIFF_PLOY;
		}
		else
		{
			iDiffLevelLimit = LIMIT_LEVEL_DIFF_PLOY;
		}
	}
	else
	{
		if (pstEnv->pstAppCtx->stCurr.tv_sec - pstAutoTeamInfo->tStart >= LIMIT_LOOSE_TIME)
		{
			iDiffLevelLimit = LIMIT_LEVEL_LOOSE_DIFF;
		}
		else
		{
			iDiffLevelLimit = LIMIT_LEVEL_DIFF;
		}
	}

	iMinLevel = MAX_LEVEL + 1;
	iMaxLevel = 0;
	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		if (abs((int)pstZoneTeam->stTeamInfo.TeamMember[i].Level - (int)pstPlayer->stRoleData.Level) >
			iDiffLevelLimit)
		{
			return 0;
		}

		if (pstZoneTeam->stTeamInfo.TeamMember[i].Level > iMaxLevel)
		{
			iMaxLevel = pstZoneTeam->stTeamInfo.TeamMember[i].Level;
		}

		if (pstZoneTeam->stTeamInfo.TeamMember[i].Level < iMinLevel)
		{
			iMinLevel = pstZoneTeam->stTeamInfo.TeamMember[i].Level;
		}
	}

	pstAutoTeamInfo = &pstZoneTeam->stAutoTeamInfo;
	if(pstDiffPworldDef && pstDiffPworldDef->PworldInfo[1].PworldID==0)
	{
		if (pstEnv->pstAppCtx->stCurr.tv_sec - pstAutoTeamInfo->tStart >= LIMIT_LOOSE_TIME)
		{
			iDiffLevelLimit = LIMIT_LEVEL_LOOSE_DIFF_PLOY;
		}
		else
		{
			iDiffLevelLimit = LIMIT_LEVEL_DIFF_PLOY;
		}
	}
	else
	{
		if (pstEnv->pstAppCtx->stCurr.tv_sec - pstAutoTeamInfo->tStart >= LIMIT_LOOSE_TIME)
		{
			iDiffLevelLimit = LIMIT_LEVEL_LOOSE_DIFF;
		}
		else
		{
			iDiffLevelLimit = LIMIT_LEVEL_DIFF;
		}
	}

	iTmpMaxLevel = iMaxLevel;
	if (iMinLevel+iDiffLevelLimit > iMaxLevel)
	{
		iMaxLevel = iMinLevel+iDiffLevelLimit;
	}

	if (iTmpMaxLevel - iDiffLevelLimit < iMinLevel)
	{
		iMinLevel = iTmpMaxLevel - iDiffLevelLimit;
	}

	if ((int)pstPlayer->stRoleData.Level < iMinLevel || 
		(int)pstPlayer->stRoleData.Level > iMaxLevel)
	{
		return 0;
	}
	
	return 1;
}

int autoteam_level_player(ZONESVRENV* pstEnv, Player *pstPlayer1, Player *pstPlayer2,int AutoTeamID)
{
	AutoTeamInfo *pstAutoTeamInfo;
	int iDiffLevelLimit;
	ACTIVEHELPER *pstActiveHelper = NULL;
	DIFFPWORLDDEF *pstDiffPworldDef = NULL;

	pstAutoTeamInfo = &pstPlayer1->stOnline.stAutoTeamInfo;
	pstActiveHelper = find_active_by_autoteam(pstEnv, AutoTeamID);
	if(pstActiveHelper)
	{
		pstDiffPworldDef = z_find_diff_pworld_def(pstEnv,pstActiveHelper->TaskPworldID);
	}
	if(pstDiffPworldDef && pstDiffPworldDef->PworldInfo[1].PworldID==0)
	{
		if (pstEnv->pstAppCtx->stCurr.tv_sec - pstAutoTeamInfo->tStart >= LIMIT_LOOSE_TIME)
		{
			iDiffLevelLimit = LIMIT_LEVEL_LOOSE_DIFF_PLOY;
		}
		else
		{
			iDiffLevelLimit = LIMIT_LEVEL_DIFF_PLOY;
		}
	}
	else
	{
		if (pstEnv->pstAppCtx->stCurr.tv_sec - pstAutoTeamInfo->tStart >= LIMIT_LOOSE_TIME)
		{
			iDiffLevelLimit = LIMIT_LEVEL_LOOSE_DIFF;
		}
		else
		{
			iDiffLevelLimit = LIMIT_LEVEL_DIFF;
		}
	}

	if (abs((int)pstPlayer1->stRoleData.Level - (int)pstPlayer2->stRoleData.Level) > iDiffLevelLimit)
	{
		return 0;
	}

	pstAutoTeamInfo = &pstPlayer2->stOnline.stAutoTeamInfo;
	if(pstDiffPworldDef && pstDiffPworldDef->PworldInfo[1].PworldID==0)
	{
		if (pstEnv->pstAppCtx->stCurr.tv_sec - pstAutoTeamInfo->tStart >= LIMIT_LOOSE_TIME)
		{
			iDiffLevelLimit = LIMIT_LEVEL_LOOSE_DIFF_PLOY;
		}
		else
		{
			iDiffLevelLimit = LIMIT_LEVEL_DIFF_PLOY;
		}
	}
	else
	{
		if (pstEnv->pstAppCtx->stCurr.tv_sec - pstAutoTeamInfo->tStart >= LIMIT_LOOSE_TIME)
		{
			iDiffLevelLimit = LIMIT_LEVEL_LOOSE_DIFF;
		}
		else
		{
			iDiffLevelLimit = LIMIT_LEVEL_DIFF;
		}
	}

	if (abs((int)pstPlayer1->stRoleData.Level - (int)pstPlayer2->stRoleData.Level) > iDiffLevelLimit)
	{
		return 0;
	}

	return 1;
}

int autoteam_created_limit_co(ZONESVRENV* pstEnv, Player *pstPlayer, short nLimitFlag,
									ZoneTeam *pstZoneTeam, AutoTeamCreated *pstAutoTeamCreated,int AutoTeamID)
{
	int i;
	short nRet;
	int iPreTeamNum;
	unsigned char abCareerNum[MAX_CAREER];
	Player *pstOne;

	if (AUTO_TEAM_LIMIT_LEADER & nLimitFlag)
	{
		return 0;
	}
	
	//判断副本难度
	nRet = (AUTO_TEAM_LIMIT_EASY|AUTO_TEAM_LIMIT_NORM|AUTO_TEAM_LIMIT_DIFF|AUTO_TEAM_LIMIT_NIGHTMARE);
	nRet = nRet & nLimitFlag & pstAutoTeamCreated->nLimitFlag;
		
	for (i=0; i<pstAutoTeamCreated->nSingleNum; i++)
	{
		nRet = nRet & pstAutoTeamCreated->astSingle[i].nLimitFlag;
	}
	
	if (0 == nRet)
	{
		return 0;
	}

	//判断职业
	iPreTeamNum = pstZoneTeam->stTeamInfo.MemberNum + pstAutoTeamCreated->nSingleNum + 1;

	//判断已建队伍的职业需求
	memset(abCareerNum, 0, sizeof(abCareerNum));
	abCareerNum[(pstPlayer->stRoleData.Career & 0x0f) - 1]++;
	for (i=0; i<pstAutoTeamCreated->nSingleNum; i++)
	{
		abCareerNum[(pstAutoTeamCreated->astSingle[i].bCareer & 0x0f) - 1]++;
	}

	if (0 == autoteam_career_co(pstAutoTeamCreated->nLimitFlag, iPreTeamNum, abCareerNum))
	{
		return 0;
	}

	//判断散人的职业需求
	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		abCareerNum[(pstZoneTeam->stTeamInfo.TeamMember[i].Career & 0xf) - 1]++;
	}

	//自己的
	abCareerNum[(pstPlayer->stRoleData.Career & 0x0f) - 1]--;
	if (0 == autoteam_career_co(nLimitFlag, iPreTeamNum, abCareerNum))
	{
		return 0;
	}
	abCareerNum[(pstPlayer->stRoleData.Career & 0x0f) - 1]++;

	//其他散人的
	for (i=0; i<pstAutoTeamCreated->nSingleNum; i++)
	{
		abCareerNum[(pstAutoTeamCreated->astSingle[i].bCareer & 0x0f) - 1]--;
		if (0 == autoteam_career_co(pstAutoTeamCreated->astSingle[i].nLimitFlag,  iPreTeamNum, abCareerNum))
		{
			return 0;
		}
		abCareerNum[(pstAutoTeamCreated->astSingle[i].bCareer & 0x0f) - 1]++;
	}

	//等级差判断
	if (0 == autoteam_level_team_player(pstEnv, pstZoneTeam, pstPlayer,AutoTeamID))
	{
		return 0;
	}

	for (i=0; i<pstAutoTeamCreated->nSingleNum; i++)
	{
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamCreated->astSingle[i].iPlayerID);
		if (pstOne)
		{
			if (0 == autoteam_level_player(pstEnv, pstOne, pstPlayer,AutoTeamID))
			{
				return 0;
			}
		}
	}

	return 1;
}

int autoteam_created_addsingle_pre(ZONESVRENV* pstEnv, Player *pstPlayer, short nLimitFlag, 
						AutoTeamOne *pstAutoTeamOne, AutoTeamPreIdx *pstPreIdx,int AutoTeamID)
{
	int i, iTeamMem;
	ZoneTeam *pstZoneTeam;
	TMEMBLOCK *pstMemBlock;
	AutoTeamCreated *pstAutoTeamCreated;
	int iUnValidTeam = -1;

	memset(pstPreIdx, 0, sizeof(*pstPreIdx));

	for (i=0; i<pstAutoTeamOne->iCreatedTeamNum; i++)
	{
		pstAutoTeamCreated = &pstAutoTeamOne->astAutoTeamCreated[i];
			
		if (pstAutoTeamCreated->nSingleNum >= AUTO_TEAM_NEED_MEMBER ||
			pstAutoTeamCreated->OpFlag)
		{
			continue;
		}

		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstTeamPool, pstAutoTeamCreated->iTeamID);
		if (0 == pstMemBlock->fValid)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "zone team null");

			iUnValidTeam = pstAutoTeamCreated->iTeamID;
			continue;
		}

		pstZoneTeam = (ZoneTeam *)pstMemBlock->szData;

		iTeamMem = pstZoneTeam->stTeamInfo.MemberNum + pstAutoTeamCreated->nSingleNum;
		if (iTeamMem >= MAX_TEAM_MEMBER)
		{
			continue;
		}

		if (0 == autoteam_created_limit_co(pstEnv, pstPlayer, nLimitFlag, pstZoneTeam, pstAutoTeamCreated,AutoTeamID))
		{
			continue;
		}

		iTeamMem += 1;

		if (iTeamMem > pstPreIdx->iNum)
		{
			pstPreIdx->iNum = iTeamMem;
			pstPreIdx->tTime = pstZoneTeam->stAutoTeamInfo.tStart;
			pstPreIdx->iIdx = i;
			pstPreIdx->nLimitFlag = nLimitFlag;
		}
		
		if (iTeamMem >= MAX_TEAM_MEMBER)
		{
			break;
		}
	}

	if (iUnValidTeam != -1)
	{
		autoteam_team_del_id(pstAutoTeamOne, iUnValidTeam);
	}
	
	return 0;
}

int autoteam_get_prememinfo_fromplayer(CSAUTOTEAMPREMEMINFO *pstInfo, Player *pstPlayer)
{
	AutoTeamInfo *pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;
			
	pstInfo->Career = pstPlayer->stRoleData.Career;
	pstInfo->Face = pstPlayer->stRoleData.Face;
	pstInfo->Gender = pstPlayer->stRoleData.Gender;
	pstInfo->Hair = pstPlayer->stRoleData.Hair;
	pstInfo->Head = pstPlayer->stRoleData.Head;
	pstInfo->Level = pstPlayer->stRoleData.Level;
	pstInfo->CreatedTeamMem = 0;
	if (AUTOTEAM_OP_AGREE & pstAutoTeamInfo->bOPFlag)
	{
		pstInfo->AgreeStat = 1;
	}
	else
	{
		pstInfo->AgreeStat = 0;
	}
	STRNCPY(pstInfo->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstInfo->RoleName));
	memcpy(&pstInfo->AutoTeamApplyInfo, pstAutoTeamInfo, sizeof(pstInfo->AutoTeamApplyInfo));
	
	return 0;
}

int autoteam_get_prememinfo_frommember(CSAUTOTEAMPREMEMINFO *pstInfo, TEAMMEMBER *pstTeamMem, ZoneTeam *pstZoneTeam)
{
	pstInfo->Career = pstTeamMem->Career;
	pstInfo->Face = pstTeamMem->Face;
	pstInfo->Gender = pstTeamMem->Gender;
	pstInfo->Hair = pstTeamMem->Hair;
	pstInfo->Head = pstTeamMem->Head;
	pstInfo->Level = pstTeamMem->Level;
	pstInfo->AgreeStat = 1;
	pstInfo->CreatedTeamMem = 1;
	STRNCPY(pstInfo->RoleName, pstTeamMem->MemberName, sizeof(pstInfo->RoleName));

	if (pstTeamMem->Position == TEAM_POSITION_CAPTAIN)
	{
		memcpy(&pstInfo->AutoTeamApplyInfo, &pstZoneTeam->stAutoTeamInfo.stApply, sizeof(pstInfo->AutoTeamApplyInfo));
	}
	else
	{
		memset(&pstInfo->AutoTeamApplyInfo, 0, sizeof(pstInfo->AutoTeamApplyInfo));
	}

	return 0;
}


int autoteam_created_mem_info(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneTeam *pstZoneTeam, AutoTeamCreated *pstAutoTeamCreated	)
{
	int i;
	Player *pstOne;
	CSPKG stPkg;
	CSAUTOTEAMSVR *pstAutoTeamSvr = &stPkg.Body.AutoTeamSvr;
	CSAUTOTEAMPREADD *pstAdd = &pstAutoTeamSvr->AutoTeamSvrData.PreAdd;

	pstAutoTeamSvr->AutoTeamSvrOp = AUTOTEAM_SVR_OP_PRE_ADD;
	pstAdd->AutoTeamID = pstZoneTeam->stAutoTeamInfo.AutoTeamID;
	pstAdd->CreatedTeam = 1;
	pstAdd->Replace = 1;
	pstAdd->Num = 0;

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		autoteam_get_prememinfo_frommember(&pstAdd->PreMemInfos[pstAdd->Num++], &pstZoneTeam->stTeamInfo.TeamMember[i], pstZoneTeam);
	}

	if (pstAutoTeamCreated)
	{
		for (i=0; i<pstAutoTeamCreated->nSingleNum; i++)
		{
			pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamCreated->astSingle[i].iPlayerID);
			if (pstOne)
			{
				autoteam_get_prememinfo_fromplayer(&pstAdd->PreMemInfos[pstAdd->Num++], pstOne);
			}
		}
	}

	Z_CSHEAD_INIT(&stPkg.Head, AUTOTEAM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

AutoTeamCreated *autoteam_find_created_byteam(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam)
{
	int i;
	AutoTeamOne *pstAutoTeamOne;
	
	if (0 == pstZoneTeam->stAutoTeamInfo.tStart || 0 > pstZoneTeam->stAutoTeamInfo.AutoTeamID)
	{
		return NULL;
	}

	pstAutoTeamOne = &pstEnv->pstZoneObj->stAutoTeam.astAutoTeams[(int)pstZoneTeam->stAutoTeamInfo.AutoTeamID];
	for (i=0; i<pstAutoTeamOne->iCreatedTeamNum; i++)
	{
		if (pstAutoTeamOne->astAutoTeamCreated[i].iTeamID == pstZoneTeam->iTeamIdx)
		{
			return &pstAutoTeamOne->astAutoTeamCreated[i];
		}
	}

	return NULL;
}

//需要再发一遍预组队伍成员信息
int autoteam_created_mem_online_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZoneTeam *pstZoneTeam;
	AutoTeamCreated *pstAutoTeamCreated;

	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if (NULL == pstZoneTeam)
	{
		return 0;
	}

	pstAutoTeamCreated = autoteam_find_created_byteam(pstEnv, pstZoneTeam);
	if (NULL == pstAutoTeamCreated)
	{
		return 0;
	}
	
	autoteam_created_mem_info(pstEnv, pstPlayer, pstZoneTeam, pstAutoTeamCreated);
								
	return 0;
}

int autoteam_team_del_id(AutoTeamOne *pstAutoTeamOne, int iTeamID)
{
	int i;

	for (i=pstAutoTeamOne->iCreatedTeamNum-1;i>=0;i-- )
	{
		if (pstAutoTeamOne->astAutoTeamCreated[i].iTeamID == iTeamID)
		{
			pstAutoTeamOne->iCreatedTeamNum--;
			if (i != pstAutoTeamOne->iCreatedTeamNum)
			{
				memmove(&pstAutoTeamOne->astAutoTeamCreated[i], 
						&pstAutoTeamOne->astAutoTeamCreated[i+1],
						(pstAutoTeamOne->iCreatedTeamNum-i)*sizeof(pstAutoTeamOne->astAutoTeamCreated[0]));
			}
		}
	}
	
	return 0;
}


int autoteam_team_del(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeam, int iAutoTeamID)
{
	int i;
	AUTOTEAMAPPLYINFO *pstApply;
	AutoTeam *pstAutoTeam = &pstEnv->pstZoneObj->stAutoTeam;

	pstApply = &pstZoneTeam->stAutoTeamInfo.stApply;

	for (i=0; i<pstApply->AutoTeamIDNum; i++)
	{
		if (iAutoTeamID != pstApply->AutoTeamIDInfos[i].AutoTeamID)
		{
			autoteam_team_del_id(&pstAutoTeam->astAutoTeams[pstApply->AutoTeamIDInfos[i].AutoTeamID], pstZoneTeam->iTeamIdx);
		}
	}

	return 0;
}

int autoteam_created_add(ZONESVRENV* pstEnv, Player *pstPlayer,  AutoTeamPreIdx *pstPreIdx, 
							AutoTeamOne *pstAutoTeamOne, AUTOTEAMAPPLYINFO *pstApply, int iAutoTeamID)
{
	int i;
	ZoneTeam *pstZoneTeam;
	Player *pstOne;
	TMEMBLOCK *pstMemBlock;
	CSPKG stPkg;
	CSAUTOTEAMSVR *pstAutoTeamSvr = &stPkg.Body.AutoTeamSvr;
	CSAUTOTEAMPREADD *pstAdd = &pstAutoTeamSvr->AutoTeamSvrData.PreAdd;	
	AutoTeamSingle *pstSingle;
	AutoTeamInfo *pstAutoTeamInfo;
	AutoTeamCreated *pstAutoTeamCreated = &pstAutoTeamOne->astAutoTeamCreated[pstPreIdx->iIdx];

	UNUSED(pstApply);

	pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstTeamPool, pstAutoTeamCreated->iTeamID);
	if (0 == pstMemBlock->fValid)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "zone team null");
		return -1;
	}

	pstZoneTeam = (ZoneTeam *)pstMemBlock->szData;

	autoteam_team_del(pstEnv, pstZoneTeam, iAutoTeamID);
	pstAutoTeamInfo = &pstZoneTeam->stAutoTeamInfo;
	pstAutoTeamInfo->AutoTeamID = iAutoTeamID;
	pstAutoTeamInfo->Stat = 1;

	pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;
	pstAutoTeamInfo->AutoTeamID = iAutoTeamID;
	pstAutoTeamInfo->Stat = 1;
	pstAutoTeamInfo->bOPFlag = 0;

	pstAutoTeamSvr->AutoTeamSvrOp = AUTOTEAM_SVR_OP_PRE_ADD;
	pstAdd->Num = 1;
	pstAdd->AutoTeamID = pstZoneTeam->stAutoTeamInfo.AutoTeamID;
	pstAdd->CreatedTeam = 1;
	pstAdd->Replace = 0;
	autoteam_get_prememinfo_fromplayer(&pstAdd->PreMemInfos[0], pstPlayer);
	
	Z_CSHEAD_INIT(&stPkg.Head, AUTOTEAM_SVR);

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (pstOne)
		{
			z_cltmsg_send(pstEnv, pstOne, &stPkg);
		}
	}

	for (i=0; i<pstAutoTeamCreated->nSingleNum; i++)
	{
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamCreated->astSingle[i].iPlayerID);
		if (pstOne)
		{
			z_cltmsg_send(pstEnv, pstOne, &stPkg);
		}
	}

	pstSingle = &pstAutoTeamCreated->astSingle[pstAutoTeamCreated->nSingleNum++];
	memset(pstSingle, 0, sizeof(*pstSingle));
	pstSingle->bCareer = pstPlayer->stRoleData.Career;
	pstSingle->nLimitFlag = pstPreIdx->nLimitFlag;
	pstSingle->iPlayerID = pstPlayer->iMemID;

	autoteam_created_mem_info(pstEnv, pstPlayer, pstZoneTeam, pstAutoTeamCreated);
	
	return 0;
}

int autoteam_pre_limit_co(ZONESVRENV* pstEnv, Player *pstPlayer, short nLimitFlag1,
							AutoTeamPre *pstAutoTeamPre,int AutoTeamID)
{
	int i;
	short nRet;
	int iPreTeamNum;
	unsigned char abCareerNum[MAX_CAREER];
	Player *pstOne;

	//判断副本难度
	nRet = (AUTO_TEAM_LIMIT_EASY|AUTO_TEAM_LIMIT_NORM|AUTO_TEAM_LIMIT_DIFF|AUTO_TEAM_LIMIT_NIGHTMARE);
	nRet = nRet & nLimitFlag1;
			
	for (i=0; i<pstAutoTeamPre->nSingleNum; i++)
	{
		nRet = nRet & pstAutoTeamPre->astSingle[i].nLimitFlag;

		if ((AUTO_TEAM_LIMIT_LEADER & nLimitFlag1) && (AUTO_TEAM_LIMIT_LEADER & pstAutoTeamPre->astSingle[i].nLimitFlag)) //队长
		{
			return 0;
		}

		if (0 == nRet)
		{
			return 0;
		}
	}
	
	//判断职业
	iPreTeamNum = pstAutoTeamPre->nSingleNum + 1;

	//判断自己的职业需求
	memset(abCareerNum, 0, sizeof(abCareerNum));
	
	for (i=0; i<pstAutoTeamPre->nSingleNum; i++)
	{
		abCareerNum[(pstAutoTeamPre->astSingle[i].bCareer & 0x0f) - 1]++;
	}

	if (0 == autoteam_career_co(nLimitFlag1, iPreTeamNum, abCareerNum))
	{
		return 0;
	}

	//判断其他人的职业需求
	abCareerNum[(pstPlayer->stRoleData.Career & 0x0f) - 1]++;
	
	for (i=0; i<pstAutoTeamPre->nSingleNum; i++)
	{
		abCareerNum[(pstAutoTeamPre->astSingle[i].bCareer & 0x0f) - 1]--;
		if (0 == autoteam_career_co(pstAutoTeamPre->astSingle[i].nLimitFlag,  iPreTeamNum, abCareerNum))
		{
			return 0;
		}
		abCareerNum[(pstAutoTeamPre->astSingle[i].bCareer & 0x0f) - 1]++;

		//判断等级
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[i].iPlayerID);
		if (pstOne)
		{
			if (0 == autoteam_level_player(pstEnv, pstOne, pstPlayer,AutoTeamID))
			{
				return 0;
			}
		}
	}
	
	return 1;
}

int autoteam_pre_addsingle_pre(ZONESVRENV* pstEnv, Player *pstPlayer, short nLimitFlag,
						AutoTeamOne *pstAutoTeamOne, AutoTeamPreIdx *pstPreIdx,int AutoTeamID)
{
	int i, iTeamMem;
	AutoTeamPre *pstAutoTeamPre;

	memset(pstPreIdx, 0, sizeof(*pstPreIdx));

	for (i=0; i<pstAutoTeamOne->iPreTeamNum; i++)
	{
		pstAutoTeamPre = &pstAutoTeamOne->astAutoTeamPre[i];
			
		if (pstAutoTeamPre->nSingleNum >= MAX_TEAM_MEMBER || pstAutoTeamPre->OpFlag)
		{
			continue;
		}

		if (0 == autoteam_pre_limit_co(pstEnv, pstPlayer, nLimitFlag, pstAutoTeamPre,AutoTeamID))
		{
			continue;
		}

		iTeamMem = pstAutoTeamPre->nSingleNum + 1;

		if (iTeamMem > pstPreIdx->iNum)
		{
			pstPreIdx->iNum = iTeamMem;
			pstPreIdx->tTime = pstAutoTeamPre->tTime;
			pstPreIdx->iIdx = i;
			pstPreIdx->nLimitFlag = nLimitFlag;
		}
		
		if (iTeamMem >= MAX_TEAM_MEMBER)
		{
			break;
		}
	}
	
	return 0;
}

int autoteam_pre_mem_info(ZONESVRENV* pstEnv, Player *pstPlayer, AutoTeamPre *pstAutoTeamPre)
{
	int i;
	Player *pstOne;
	CSPKG stPkg;
	CSAUTOTEAMSVR *pstAutoTeamSvr = &stPkg.Body.AutoTeamSvr;
	CSAUTOTEAMPREADD *pstAdd = &pstAutoTeamSvr->AutoTeamSvrData.PreAdd;

	pstAutoTeamSvr->AutoTeamSvrOp = AUTOTEAM_SVR_OP_PRE_ADD;
	pstAdd->Num = 0;
	pstAdd->AutoTeamID = pstPlayer->stOnline.stAutoTeamInfo.AutoTeamID;
	pstAdd->CreatedTeam = 0;
	pstAdd->Replace = 1;

	for (i=0; i<pstAutoTeamPre->nSingleNum; i++)
	{
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[i].iPlayerID);
		if (pstOne)
		{
			autoteam_get_prememinfo_fromplayer(&pstAdd->PreMemInfos[pstAdd->Num++], pstOne);
		}
	}

	Z_CSHEAD_INIT(&stPkg.Head, AUTOTEAM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

//来了个队长放第一位
int autoteam_pre_just_caption_pos(ZONESVRENV *pstEnv, AutoTeamPre *pstAutoTeamPre)
{
	Player *pstOne;
	int i=0;
	AutoTeamSingle stAutoTeamSingle;
	
	for (i=0; i<pstAutoTeamPre->nSingleNum; i++)
	{
		if(pstAutoTeamPre->astSingle[i].nLimitFlag&AUTO_TEAM_LIMIT_LEADER)
		{
			stAutoTeamSingle=pstAutoTeamPre->astSingle[i];
			pstAutoTeamPre->astSingle[i] = pstAutoTeamPre->astSingle[0];
			pstAutoTeamPre->astSingle[0] = stAutoTeamSingle;
		}
	}

	for (i=0; i<pstAutoTeamPre->nSingleNum; i++)
	{
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[i].iPlayerID);
		if (pstOne)
		{
			autoteam_pre_mem_info(pstEnv, pstOne, pstAutoTeamPre);
		}
	}

	return 0;
}


int autoteam_pre_add(ZONESVRENV* pstEnv, Player *pstPlayer,  AutoTeamPreIdx *pstPreIdx, 
							AutoTeamOne *pstAutoTeamOne, int iAutoTeamID)
{
	//int i;
	//Player *pstOne;
	CSPKG stPkg;
	CSAUTOTEAMSVR *pstAutoTeamSvr = &stPkg.Body.AutoTeamSvr;
	CSAUTOTEAMPREADD *pstAdd = &pstAutoTeamSvr->AutoTeamSvrData.PreAdd;
	
	AutoTeamSingle *pstSingle;
	AutoTeamInfo *pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;
	AutoTeamPre *pstAutoTeamPre = &pstAutoTeamOne->astAutoTeamPre[pstPreIdx->iIdx];

	pstAutoTeamInfo->AutoTeamID = iAutoTeamID;
	pstAutoTeamInfo->Stat = 2;
	pstAutoTeamInfo->bOPFlag = 0;

	pstAutoTeamSvr->AutoTeamSvrOp = AUTOTEAM_SVR_OP_PRE_ADD;
	pstAdd->Num = 1;
	pstAdd->AutoTeamID = iAutoTeamID;
	pstAdd->CreatedTeam = 0;
	pstAdd->Replace = 0;
	autoteam_get_prememinfo_fromplayer(&pstAdd->PreMemInfos[0], pstPlayer);
	
	Z_CSHEAD_INIT(&stPkg.Head, AUTOTEAM_SVR);

	/*for (i=0; i<pstAutoTeamPre->nSingleNum; i++)
	{
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[i].iPlayerID);
		if (pstOne)
		{
			z_cltmsg_send(pstEnv, pstOne, &stPkg);
		}
	}*/

	pstSingle = &pstAutoTeamPre->astSingle[pstAutoTeamPre->nSingleNum++];
	memset(pstSingle, 0, sizeof(*pstSingle));
	pstSingle->bCareer = pstPlayer->stRoleData.Career;
	pstSingle->nLimitFlag = pstPreIdx->nLimitFlag;
	pstSingle->iPlayerID = pstPlayer->iMemID;

	//autoteam_pre_mem_info(pstEnv, pstPlayer, pstAutoTeamPre);
	autoteam_pre_just_caption_pos(pstEnv, pstAutoTeamPre);
	
	return 0;
}

int autoteam_single_addsingle_pre(ZONESVRENV* pstEnv, Player *pstPlayer, short nLimitFlag,
									AutoTeamOne *pstAutoTeamOne, AutoTeamPreIdx *pstPreIdx,int AutoTeamID)
{
	int i;
	AutoTeamSingle *pstSingle;
	AutoTeamPre *pstAutoTeamPre;

	memset(pstPreIdx, 0, sizeof(*pstPreIdx));

	for (i=0; i<pstAutoTeamOne->iSingleNum; i++)
	{
		pstAutoTeamPre = &pstPreIdx->stTeamPre;
		pstSingle = &pstAutoTeamOne->astAutoTeamSingle[i];
		memcpy(&pstAutoTeamPre->astSingle[pstAutoTeamPre->nSingleNum++], pstSingle, sizeof(pstAutoTeamPre->astSingle[0]));

		if (0 == autoteam_pre_limit_co(pstEnv, pstPlayer, nLimitFlag, pstAutoTeamPre,AutoTeamID))
		{
			pstAutoTeamPre->nSingleNum--;
			continue;
		}

		pstPreIdx->iNum++;
		pstPreIdx->nLimitFlag = nLimitFlag;
		if (pstPreIdx->iNum >= MAX_TEAM_MEMBER - 1)
		{
			break;
		}
	}
		
	return 0;
}

int autoteam_single_del_id(AutoTeamOne *pstAutoTeamOne, int iPlayerID)
{
	int i;
	
	for (i=pstAutoTeamOne->iSingleNum-1; i>=0; i--)
	{
		if (pstAutoTeamOne->astAutoTeamSingle[i].iPlayerID == iPlayerID)
		{
			pstAutoTeamOne->iSingleNum--;

			if (i != pstAutoTeamOne->iSingleNum)
			{
				memmove(&pstAutoTeamOne->astAutoTeamSingle[i], &pstAutoTeamOne->astAutoTeamSingle[i+1],
							(pstAutoTeamOne->iSingleNum-i)*sizeof(pstAutoTeamOne->astAutoTeamSingle[0]));
			}
			
			return 0;
		}
	}

	return -1;
}

int autoteam_single_del(ZONESVRENV* pstEnv, int iPlayerID)
{
	int i;
	Player *pstPlayer;
	AutoTeamInfo *pstAutoTeamInfo;

	pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, iPlayerID);
	if (NULL == pstPlayer)
	{
		return -1;
	}

	pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;
	if (pstAutoTeamInfo->tStart == 0)
	{
		return -1;
	}

	for (i=0; i<pstAutoTeamInfo->stApply.AutoTeamIDNum; i++)
	{
		autoteam_single_del_id(&pstEnv->pstZoneObj->stAutoTeam.astAutoTeams[pstAutoTeamInfo->stApply.AutoTeamIDInfos[i].AutoTeamID],
								iPlayerID);
	}
	
	return 0;
}

int autoteam_single_add_player(ZONESVRENV* pstEnv, AutoTeamOne *pstAutoTeamOne, Player *pstPlayer, short nLimitFlag)
{
	AutoTeamSingle *pstSingle;
		
	if (pstAutoTeamOne->iSingleNum >= AUTO_TEAM_SINGLE_NUM)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "autoteamid full");
		return -1;
	}

	pstSingle = &pstAutoTeamOne->astAutoTeamSingle[pstAutoTeamOne->iSingleNum++];
	memset(pstSingle, 0, sizeof(*pstSingle));
	pstSingle->iPlayerID = pstPlayer->iMemID;
	pstSingle->bCareer = pstPlayer->stRoleData.Career;
	pstSingle->nLimitFlag = nLimitFlag;
	
	return 0;
}

int autoteam_pre_leader(ZONESVRENV* pstEnv, AutoTeamPre *pstAutoTeamPre, int iCreatedFlag)
{
	Player *pstOne;
	int i;
	AutoTeamSingle stTmp;

	for (i=0; i<pstAutoTeamPre->nSingleNum; i++)
	{
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[i].iPlayerID);
		if (pstOne)
		{
			if ((iCreatedFlag && (AUTOTEAM_OP_CREATE & pstOne->stOnline.stAutoTeamInfo.bOPFlag)) ||
				(0 == iCreatedFlag && (AUTO_TEAM_LIMIT_LEADER & pstOne->stOnline.stAutoTeamInfo.stApply.LimitFlag)))
			{
				if (i != 0)
				{
					memcpy(&stTmp, &pstAutoTeamPre->astSingle[0], sizeof(stTmp));
					memcpy(&pstAutoTeamPre->astSingle[0], &pstAutoTeamPre->astSingle[i],
							sizeof(pstAutoTeamPre->astSingle[0]));
					memcpy(&pstAutoTeamPre->astSingle[i], &stTmp, sizeof(pstAutoTeamPre->astSingle[0]));
				}
				
				return 0;
			}
		}
	}
	
	return 0;
}

int autoteam_single_add(ZONESVRENV* pstEnv, Player *pstPlayer,  AutoTeamPreIdx *pstPreIdx, 
							AutoTeamOne *pstAutoTeamOne, AUTOTEAMAPPLYINFO *pstApply, int iAutoTeamID)
{
	int i;
	Player *pstOne;
	AutoTeamSingle *pstSingle;
	AutoTeamInfo *pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;
	AutoTeamPre *pstAutoTeamPre = &pstPreIdx->stTeamPre;
	UNUSED(pstApply);

	if (pstAutoTeamOne->iPreTeamNum >= AUTO_TEAM_PRE_NUM)
	{
		return -1;
	}

	pstAutoTeamInfo->AutoTeamID = iAutoTeamID;
	pstAutoTeamInfo->Stat = 2;
	pstAutoTeamInfo->bOPFlag = 0;

	pstSingle = &pstAutoTeamPre->astSingle[pstAutoTeamPre->nSingleNum++];
	memset(pstSingle, 0, sizeof(*pstSingle));
	pstSingle->bCareer = pstPlayer->stRoleData.Career;
	pstSingle->nLimitFlag = pstPreIdx->nLimitFlag;
	pstSingle->iPlayerID = pstPlayer->iMemID;

	autoteam_pre_leader(pstEnv, pstAutoTeamPre, 0);

	for (i=0; i<pstAutoTeamPre->nSingleNum; i++)
	{
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[i].iPlayerID);
		if (pstOne)
		{
			pstOne->stOnline.stAutoTeamInfo.Stat = 2;
			pstOne->stOnline.stAutoTeamInfo.AutoTeamID = iAutoTeamID;
			autoteam_pre_mem_info(pstEnv, pstOne, pstAutoTeamPre);
		}
	}

	pstAutoTeamPre->tTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	memcpy(&pstAutoTeamOne->astAutoTeamPre[pstAutoTeamOne->iPreTeamNum++], pstAutoTeamPre,
			sizeof(pstAutoTeamOne->astAutoTeamPre[0]));

	for (i=0; i<pstAutoTeamPre->nSingleNum; i++)
	{
		autoteam_single_del(pstEnv, pstAutoTeamPre->astSingle[i].iPlayerID);
	}

	for (i=pstAutoTeamPre->nSingleNum-1;i>=0; i--)
	{
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[i].iPlayerID);
		if (!pstOne)
		{
			pstAutoTeamPre->nSingleNum--;
			if (i != pstAutoTeamPre->nSingleNum)
			{
				memmove(&pstAutoTeamPre->astSingle[i], 
						   &pstAutoTeamPre->astSingle[i+1],
						    (pstAutoTeamPre->nSingleNum-i)*sizeof(pstAutoTeamPre->astSingle[0]));
			}
		}
	}

	return 0;
}

//统一，难度一个都没选转成全选
short autoteam_limit_filter(short nLimitFlag)
{
	if (0 == ((AUTO_TEAM_LIMIT_EASY|AUTO_TEAM_LIMIT_NORM|AUTO_TEAM_LIMIT_DIFF|AUTO_TEAM_LIMIT_NIGHTMARE) & nLimitFlag))
	{
		return (nLimitFlag | (AUTO_TEAM_LIMIT_EASY|AUTO_TEAM_LIMIT_NORM|AUTO_TEAM_LIMIT_DIFF));
	}
	else
	{
		return nLimitFlag;
	}
}

int autoteam_insert_single_apply(ZONESVRENV* pstEnv, Player *pstPlayer, AUTOTEAMAPPLYINFO *pstApply)
{
	int i;
	CSPKG stPkg;
	CSAUTOTEAMSVR *pstAutoTeamSvr = &stPkg.Body.AutoTeamSvr;
	CSAUTOTEAMPREADD *pstAdd = &pstAutoTeamSvr->AutoTeamSvrData.PreAdd;
	AutoTeamInfo *pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;
	AutoTeam *pstAutoTeam = &pstEnv->pstZoneObj->stAutoTeam;

	for (i=0; i<pstApply->AutoTeamIDNum; i++)
	{
		autoteam_single_add_player(pstEnv, &pstAutoTeam->astAutoTeams[pstApply->AutoTeamIDInfos[i].AutoTeamID],
									pstPlayer, autoteam_limit_filter(pstApply->LimitFlag|pstApply->AutoTeamIDInfos[i].LimitFlag));
	}

	pstAutoTeamInfo->AutoTeamID = -1;
	pstAutoTeamInfo->Stat = 0;
	pstAutoTeamInfo->bOPFlag = 0;
	
	pstAutoTeamSvr->AutoTeamSvrOp = AUTOTEAM_SVR_OP_PRE_ADD;
	pstAdd->Num = 0;
	pstAdd->AutoTeamID = pstAutoTeamInfo->AutoTeamID;
	pstAdd->CreatedTeam = 0;
	pstAdd->Replace = 1;

	autoteam_get_prememinfo_fromplayer(&pstAdd->PreMemInfos[pstAdd->Num++], pstPlayer);

	Z_CSHEAD_INIT(&stPkg.Head, AUTOTEAM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int autoteam_single_apply(ZONESVRENV* pstEnv, Player *pstPlayer, AUTOTEAMAPPLYINFO *pstApply)
{
	int i=0, iIdx=0, iAutoTeamIdx=0;
	int iRand;
	AutoTeamPreIdx stPreIdx;
	AutoTeamPreIdx stLastPreIdx;
	AutoTeam *pstAutoTeam = &pstEnv->pstZoneObj->stAutoTeam;
	AUTOTEAMAPPLYIDINFO *pstApplyIDInfo;
	AutoTeamInfo *pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;
			
	if (0 >= pstApply->AutoTeamIDNum)
	{
		return -1;
	}

	if (pstAutoTeamInfo->tStart == 0)
	{
		memset(pstAutoTeamInfo, 0, sizeof(*pstAutoTeamInfo));
		pstAutoTeamInfo->tStart = pstEnv->pstAppCtx->stCurr.tv_sec;
		memcpy(&pstAutoTeamInfo->stApply, pstApply, sizeof(pstAutoTeamInfo->stApply));
	}

	iRand = RAND1(pstApply->AutoTeamIDNum);

	//轮一遍created队伍，人多者优先
	if (0 == (AUTO_TEAM_LIMIT_LEADER & pstApply->LimitFlag))
	{
		stLastPreIdx.iNum = 0;
		for (i=0; i<pstApply->AutoTeamIDNum; i++)
		{
			pstApplyIDInfo = &pstApply->AutoTeamIDInfos[(iRand+i)%pstApply->AutoTeamIDNum];
			iIdx = pstApplyIDInfo->AutoTeamID;

			if (0 > autoteam_created_addsingle_pre(pstEnv, pstPlayer, autoteam_limit_filter(pstApplyIDInfo->LimitFlag|pstApply->LimitFlag), 
												&pstAutoTeam->astAutoTeams[iIdx], &stPreIdx,pstApplyIDInfo->AutoTeamID))
			{
				continue;
			}
			
			if (stPreIdx.iNum > stLastPreIdx.iNum)
			{
				memcpy(&stLastPreIdx, &stPreIdx, sizeof(stLastPreIdx));
				iAutoTeamIdx = iIdx;

				if (stPreIdx.iNum >= MAX_TEAM_MEMBER)
				{
					break;
				}
			}
			else if (stPreIdx.iNum == stLastPreIdx.iNum)
			{
				if (stPreIdx.tTime < stLastPreIdx.tTime)
				{
					memcpy(&stLastPreIdx, &stPreIdx, sizeof(stLastPreIdx));
					iAutoTeamIdx = iIdx;
				}
			}
		}

		if (stLastPreIdx.iNum > 0)
		{
			return autoteam_created_add(pstEnv, pstPlayer, &stLastPreIdx,  &pstAutoTeam->astAutoTeams[iAutoTeamIdx], 
										pstApply, iAutoTeamIdx);
		}
	}

	//轮一遍pre队伍，人多者优先
	stLastPreIdx.iNum = 0;
	for (i=0; i<pstApply->AutoTeamIDNum; i++)
	{
		pstApplyIDInfo = &pstApply->AutoTeamIDInfos[(iRand+i)%pstApply->AutoTeamIDNum];
		iIdx = pstApplyIDInfo->AutoTeamID;

		if (0 > autoteam_pre_addsingle_pre(pstEnv, pstPlayer, autoteam_limit_filter(pstApply->LimitFlag|pstApplyIDInfo->LimitFlag), 
										&pstAutoTeam->astAutoTeams[iIdx], &stPreIdx,pstApplyIDInfo->AutoTeamID))
		{
			continue;
		}
		
		if (stPreIdx.iNum > stLastPreIdx.iNum)
		{
			memcpy(&stLastPreIdx, &stPreIdx, sizeof(stLastPreIdx));
			iAutoTeamIdx = iIdx;

			if (stPreIdx.iNum >= MAX_TEAM_MEMBER)
			{
				break;
			}
		}
		else if (stPreIdx.iNum == stLastPreIdx.iNum)
		{
			if (stPreIdx.tTime < stLastPreIdx.tTime)
			{
				memcpy(&stLastPreIdx, &stPreIdx, sizeof(stLastPreIdx));
				iAutoTeamIdx = iIdx;
			}
		}
	}

	if (stLastPreIdx.iNum > 0)
	{
		return autoteam_pre_add(pstEnv, pstPlayer, &stLastPreIdx,  &pstAutoTeam->astAutoTeams[iAutoTeamIdx], 
									 iAutoTeamIdx);
	}

	//轮一遍散人，人多者优先
	stLastPreIdx.iNum = 0;
	for (i=0; i<pstApply->AutoTeamIDNum; i++)
	{
		pstApplyIDInfo = &pstApply->AutoTeamIDInfos[(iRand+i)%pstApply->AutoTeamIDNum];
		iIdx = pstApplyIDInfo->AutoTeamID;

		if (pstAutoTeam->astAutoTeams[iIdx].iPreTeamNum >= AUTO_TEAM_PRE_NUM)
		{
			continue;
		}

		if (0 > autoteam_single_addsingle_pre(pstEnv, pstPlayer, autoteam_limit_filter(pstApply->LimitFlag|pstApplyIDInfo->LimitFlag),
											&pstAutoTeam->astAutoTeams[iIdx], &stPreIdx,pstApplyIDInfo->AutoTeamID))
		{
			continue;
		}
		
		if (stPreIdx.iNum > stLastPreIdx.iNum)
		{
			memcpy(&stLastPreIdx, &stPreIdx, sizeof(stLastPreIdx));
			iAutoTeamIdx = iIdx;

			if (stPreIdx.iNum >= MAX_TEAM_MEMBER - 1)
			{
				break;
			}
		}
	}

	if (stLastPreIdx.iNum > 0)
	{
		return autoteam_single_add(pstEnv, pstPlayer, &stLastPreIdx,  &pstAutoTeam->astAutoTeams[iAutoTeamIdx], 
									pstApply, iAutoTeamIdx);
	}

	//插入所有队列
	autoteam_insert_single_apply(pstEnv, pstPlayer, pstApply);
	
	return 0;
}

int autoteam_single_addteam_pre(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, short nLimitFlag, 
									AutoTeamOne *pstAutoTeamOne, AutoTeamPreIdx *pstPreIdx,int AutoTeamID)
{
	int i;
	Player *pstOne;
	AutoTeamSingle *pstSingle;
	AutoTeamCreated *pstAutoTeamCreated = &pstPreIdx->stTeamCreated;

	memset(pstPreIdx, 0, sizeof(*pstPreIdx));
	pstAutoTeamCreated->iTeamID = pstZoneTeam->iTeamIdx;
	pstAutoTeamCreated->nLimitFlag = nLimitFlag;

	for (i=0; i<pstAutoTeamOne->iSingleNum; i++)
	{
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamOne->astAutoTeamSingle[i].iPlayerID);	
		if (NULL == pstOne)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "id err");
			if(i+1<pstAutoTeamOne->iSingleNum)
			{
				pstAutoTeamOne->astAutoTeamSingle[i] = pstAutoTeamOne->astAutoTeamSingle[i+1];
			}
			pstAutoTeamOne->iSingleNum = pstAutoTeamOne->iSingleNum -1;
			return -1;
		}
		
		if (0 == autoteam_created_limit_co(pstEnv, pstOne, pstAutoTeamOne->astAutoTeamSingle[i].nLimitFlag, pstZoneTeam, pstAutoTeamCreated,AutoTeamID))
		{
			continue;
		}

		pstPreIdx->iNum++;
		pstSingle = &pstAutoTeamCreated->astSingle[pstAutoTeamCreated->nSingleNum++];
		memcpy(pstSingle, &pstAutoTeamOne->astAutoTeamSingle[i], sizeof(*pstSingle));

		if (pstZoneTeam->stTeamInfo.MemberNum + pstPreIdx->iNum >= MAX_TEAM_MEMBER)
		{
			break;
		}

	}

	return 0;
}

int autoteam_single_addteam(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, AutoTeamPreIdx *pstPreIdx, 
								AutoTeamOne *pstAutoTeamOne, int iAutoTeamIdx)
{
	int i;
	Player *pstOne;
	AutoTeamCreated *pstAutoTeamCreated;

	if (pstAutoTeamOne->iCreatedTeamNum >= AUTO_TEAM_CREATED_NUM)
	{
		return -1;
	}

	pstZoneTeam->stAutoTeamInfo.AutoTeamID = iAutoTeamIdx;
	pstZoneTeam->stAutoTeamInfo.Stat = 1;
	
	pstAutoTeamCreated = &pstAutoTeamOne->astAutoTeamCreated[pstAutoTeamOne->iCreatedTeamNum++];
	memcpy(pstAutoTeamCreated, &pstPreIdx->stTeamCreated, sizeof(*pstAutoTeamCreated));

	for (i=0; i<pstAutoTeamCreated->nSingleNum; i++)
	{
		autoteam_single_del(pstEnv, pstAutoTeamCreated->astSingle[i].iPlayerID);

		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamCreated->astSingle[i].iPlayerID);

		if (pstOne)
		{
			pstOne->stOnline.stAutoTeamInfo.Stat = 1;
			pstOne->stOnline.stAutoTeamInfo.AutoTeamID = iAutoTeamIdx;
			autoteam_created_mem_info(pstEnv, pstOne, pstZoneTeam, pstAutoTeamCreated);
		}
	}

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);

		if (pstOne)
		{
			autoteam_created_mem_info(pstEnv, pstOne, pstZoneTeam, pstAutoTeamCreated);
		}
	}
	
	return 0;
}

int autoteam_created_add_team(ZONESVRENV* pstEnv, AutoTeamOne *pstAutoTeamOne, ZoneTeam *pstZoneTeam, 
									short nLimitFlag)
{
	AutoTeamCreated *pstAutoTeamCreated;
	UNUSED(pstEnv);
		
	if (pstAutoTeamOne->iCreatedTeamNum >= AUTO_TEAM_CREATED_NUM)
	{
		return -1;
	}

	pstAutoTeamCreated = &pstAutoTeamOne->astAutoTeamCreated[pstAutoTeamOne->iCreatedTeamNum++];
	pstAutoTeamCreated->nSingleNum = 0;
	pstAutoTeamCreated->OpFlag = 0;
	pstAutoTeamCreated->iTeamID = pstZoneTeam->iTeamIdx;
	pstAutoTeamCreated->nLimitFlag = nLimitFlag;

	return 0;
}

int autoteam_insert_team_apply(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, AUTOTEAMAPPLYINFO *pstApply)
{
	int i;
	Player *pstOne;
	AutoTeamInfo *pstAutoTeamInfo = &pstZoneTeam->stAutoTeamInfo;
	AutoTeam *pstAutoTeam = &pstEnv->pstZoneObj->stAutoTeam;

	for (i=0; i<pstApply->AutoTeamIDNum; i++)
	{
		autoteam_created_add_team(pstEnv, &pstAutoTeam->astAutoTeams[pstApply->AutoTeamIDInfos[i].AutoTeamID],
									pstZoneTeam, autoteam_limit_filter(pstApply->LimitFlag|pstApply->AutoTeamIDInfos[i].LimitFlag));
	}

	pstAutoTeamInfo->AutoTeamID = -1;
	pstAutoTeamInfo->Stat = 1;
	pstAutoTeamInfo->bOPFlag = 0;
	
	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);

		if (pstOne)
		{
			autoteam_created_mem_info(pstEnv, pstOne, pstZoneTeam, NULL);
		}
	}
	
	return 0;
}

int autoteam_pre_addteam_pre(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, short nLimitFlag, 
									AutoTeamOne *pstAutoTeamOne, AutoTeamPreIdx *pstPreIdx,int AutoTeamID)
{
	int i, j;
	AutoTeamCreated *pstAutoTeamCreated = &pstPreIdx->stTeamCreated;
	AutoTeamPre *pstAutoTeamPre;
	Player *pstOne;
	AutoTeamSingle *pstSingle;

	memset(pstPreIdx, 0, sizeof(*pstPreIdx));
	pstAutoTeamCreated->iTeamID = pstZoneTeam->iTeamIdx;
	pstAutoTeamCreated->nLimitFlag = nLimitFlag;

	for (i=0; i<pstAutoTeamOne->iPreTeamNum; i++)
	{
		pstAutoTeamPre = &pstAutoTeamOne->astAutoTeamPre[i];
		if (pstAutoTeamPre->OpFlag || pstAutoTeamPre->nSingleNum + pstZoneTeam->stTeamInfo.MemberNum > MAX_TEAM_MEMBER)
		{
			continue;
		}

		pstAutoTeamCreated->nSingleNum = 0;

		for (j=0; j<pstAutoTeamPre->nSingleNum; j++)
		{
			pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[j].iPlayerID);	
			if (NULL == pstOne)
			{
				break;
			}

			if (0 == autoteam_created_limit_co(pstEnv, pstOne, pstAutoTeamPre->astSingle[j].nLimitFlag, pstZoneTeam, pstAutoTeamCreated,AutoTeamID))
			{
				break;
			}

			pstSingle = &pstAutoTeamCreated->astSingle[pstAutoTeamCreated->nSingleNum++];
			memcpy(pstSingle, &pstAutoTeamPre->astSingle[j], sizeof(*pstSingle));
		}

		if (j == pstAutoTeamPre->nSingleNum)
		{
			if (pstAutoTeamPre->nSingleNum > pstPreIdx->iNum)
			{
				pstPreIdx->iNum = pstAutoTeamPre->nSingleNum;
				pstPreIdx->tTime = pstAutoTeamPre->tTime;
				pstPreIdx->iIdx = i;
			}
			else if (pstAutoTeamPre->nSingleNum == pstPreIdx->iNum)
			{
				if (pstAutoTeamPre->tTime < pstPreIdx->tTime)
				{
					pstPreIdx->iNum = pstAutoTeamPre->nSingleNum;
					pstPreIdx->tTime = pstAutoTeamPre->tTime;
					pstPreIdx->iIdx = i;
				}
			}
		}
	}
	
	return 0;
}

int autoteam_pre_del(AutoTeamOne *pstAutoTeamOne, AutoTeamPre *pstAutoTeamPre)
{
	int i;

	for (i=pstAutoTeamOne->iPreTeamNum-1;i>=0; i--)
	{
		if (pstAutoTeamPre == &pstAutoTeamOne->astAutoTeamPre[i])
		{
			pstAutoTeamOne->iPreTeamNum--;
			if (i != pstAutoTeamOne->iPreTeamNum)
			{
				memmove(&pstAutoTeamOne->astAutoTeamPre[i], 
						   &pstAutoTeamOne->astAutoTeamPre[i+1],
						    (pstAutoTeamOne->iPreTeamNum-i)*sizeof(pstAutoTeamOne->astAutoTeamPre[0]));
			}

			return 0;
		}
	}

	return -1;
}

int autoteam_pre_addteam(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, AutoTeamPreIdx *pstPreIdx, 
								AutoTeamOne *pstAutoTeamOne, int iAutoTeamIdx)
{
	int i;
	AutoTeamPre *pstAutoTeamPre = &pstAutoTeamOne->astAutoTeamPre[pstPreIdx->iIdx];
	AutoTeamCreated *pstAutoTeamCreated;
	Player *pstOne;

	if (pstAutoTeamOne->iCreatedTeamNum >= AUTO_TEAM_CREATED_NUM)
	{
		return -1;
	}	

	pstZoneTeam->stAutoTeamInfo.AutoTeamID = iAutoTeamIdx;
	pstZoneTeam->stAutoTeamInfo.Stat = 1;
	
	pstAutoTeamCreated = &pstAutoTeamOne->astAutoTeamCreated[pstAutoTeamOne->iCreatedTeamNum++];
	
	memcpy(pstAutoTeamCreated, &pstPreIdx->stTeamCreated, sizeof(*pstAutoTeamCreated));

	for (i=0; i<pstAutoTeamCreated->nSingleNum; i++)
	{
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamCreated->astSingle[i].iPlayerID);

		if (pstOne)
		{
			pstOne->stOnline.stAutoTeamInfo.Stat = 1;
			pstOne->stOnline.stAutoTeamInfo.AutoTeamID = iAutoTeamIdx;
			autoteam_created_mem_info(pstEnv, pstOne, pstZoneTeam, pstAutoTeamCreated);
		}
	}

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);

		if (pstOne)
		{
			autoteam_created_mem_info(pstEnv, pstOne, pstZoneTeam, pstAutoTeamCreated);
		}
	}

	autoteam_pre_del(pstAutoTeamOne, pstAutoTeamPre);
		
	return 0;
}
//如果是6个人的队伍申请,直接下发可以进入副本的协议
int autoteam_team_apply_by_fullmember(ZONESVRENV* pstEnv,ZoneTeam *pstZoneTeam,AUTOTEAMAPPLYINFO *pstApply)
{
	AutoTeamCreated *pstAutoTeamCreated;
	AutoTeamInfo *pstAutoTeamInfo = &pstZoneTeam->stAutoTeamInfo;
	AutoTeam *pstAutoTeam = &pstEnv->pstZoneObj->stAutoTeam;
	short nDiffLimit;
	short nDiffLimitTmp;
	int i;
	Player *pstOne;
	
	//随机找一个副本
	pstZoneTeam->stAutoTeamInfo.stApply = *pstApply;
	 
	if(pstAutoTeamInfo->AutoTeamID > 0)
	{
		nDiffLimit = autoteam_limit_filter(autoteam_find_apply_limit(&pstZoneTeam->stAutoTeamInfo.stApply, pstAutoTeamInfo->AutoTeamID));
		nDiffLimitTmp = nDiffLimit;
		for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
		{
			pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
			if (pstOne)
			{
				nDiffLimit &= autoteam_limit_filter(autoteam_find_apply_limit(&pstOne->stOnline.stAutoTeamInfo.stApply, pstAutoTeamInfo->AutoTeamID));
			}
		}
		if(!nDiffLimit)
		{
			nDiffLimit = nDiffLimitTmp;
		}
		autoteam_created_add_team(pstEnv, &pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID],pstZoneTeam, nDiffLimit);
	}
	else
	{
		int iRand = 0;
		iRand = RAND1(pstApply->AutoTeamIDNum);
		autoteam_created_add_team(pstEnv, &pstAutoTeam->astAutoTeams[pstApply->AutoTeamIDInfos[iRand].AutoTeamID],
									pstZoneTeam, autoteam_limit_filter(pstApply->LimitFlag|pstApply->AutoTeamIDInfos[iRand].LimitFlag));
		pstAutoTeamInfo->AutoTeamID = pstApply->AutoTeamIDInfos[iRand].AutoTeamID;
	}

	pstAutoTeamCreated = autoteam_find_created_by_teamid(&pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID], 
														pstZoneTeam->iTeamIdx);
	if(!pstAutoTeamCreated)
	{
		return -1;
	}

	autoteam_created_succ(pstEnv, pstZoneTeam, pstAutoTeamCreated, pstAutoTeamInfo->AutoTeamID);
	
	return 0;
}

int autoteam_team_apply(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, AUTOTEAMAPPLYINFO *pstApply)
{
	int i=0, iIdx=0, iRand=0, iAutoTeamIdx=0;
	AutoTeamPreIdx stPreIdx;
	AutoTeamPreIdx stLastPreIdx;
	AUTOTEAMAPPLYIDINFO *pstApplyIDInfo;
	AutoTeam *pstAutoTeam = &pstEnv->pstZoneObj->stAutoTeam;
	AutoTeamInfo *pstAutoTeamInfo = &pstZoneTeam->stAutoTeamInfo;

	if (0 >= pstApply->AutoTeamIDNum)
	{
		return -1;
	}

	if (pstAutoTeamInfo->tStart == 0)
	{
		AutoTeamInfo stAutoTeamInfoTmp = pstZoneTeam->stAutoTeamInfo;
		memset(pstAutoTeamInfo, 0, sizeof(*pstAutoTeamInfo));
		pstAutoTeamInfo->TransAutoTeamID = stAutoTeamInfoTmp.TransAutoTeamID;
		pstAutoTeamInfo->iPworldGrade = stAutoTeamInfoTmp.iPworldGrade;
		
		pstAutoTeamInfo->tStart = pstEnv->pstAppCtx->stCurr.tv_sec;
		memcpy(&pstAutoTeamInfo->stApply, pstApply, sizeof(pstAutoTeamInfo->stApply));
	}

	iRand = RAND1(pstApply->AutoTeamIDNum);

	//轮一遍散人
	stLastPreIdx.iNum = 0;
	for (i=0; i<pstApply->AutoTeamIDNum; i++)
	{
		pstApplyIDInfo = &pstApply->AutoTeamIDInfos[(iRand+i)%pstApply->AutoTeamIDNum];
		iIdx = pstApplyIDInfo->AutoTeamID;

		if (pstAutoTeam->astAutoTeams[iIdx].iCreatedTeamNum >= AUTO_TEAM_CREATED_NUM)
		{
			continue;
		}

		if (0 > autoteam_single_addteam_pre(pstEnv, pstZoneTeam, autoteam_limit_filter(pstApply->LimitFlag|pstApplyIDInfo->LimitFlag), 
											&pstAutoTeam->astAutoTeams[iIdx], &stPreIdx,pstApplyIDInfo->AutoTeamID))
		{
			continue;
		}
		
		if (stPreIdx.iNum > stLastPreIdx.iNum)
		{
			memcpy(&stLastPreIdx, &stPreIdx, sizeof(stLastPreIdx));
			iAutoTeamIdx = iIdx;

			if (pstZoneTeam->stTeamInfo.MemberNum + stPreIdx.iNum >= MAX_TEAM_MEMBER)
			{
				break;
			}
		}
	}

	if (stLastPreIdx.iNum > 0)
	{
		return autoteam_single_addteam(pstEnv, pstZoneTeam, &stLastPreIdx,  &pstAutoTeam->astAutoTeams[iAutoTeamIdx], 
										iAutoTeamIdx);
	}

	//轮一遍预组队
	stLastPreIdx.iNum = 0;
	for (i=0; i<pstApply->AutoTeamIDNum; i++)
	{
		pstApplyIDInfo = &pstApply->AutoTeamIDInfos[(iRand+i)%pstApply->AutoTeamIDNum];
		iIdx = pstApplyIDInfo->AutoTeamID;

		if (pstAutoTeam->astAutoTeams[iIdx].iCreatedTeamNum >= AUTO_TEAM_CREATED_NUM)
		{
			continue;
		}

		if (0 > autoteam_pre_addteam_pre(pstEnv, pstZoneTeam, autoteam_limit_filter(pstApply->LimitFlag|pstApplyIDInfo->LimitFlag), 
										&pstAutoTeam->astAutoTeams[iIdx], &stPreIdx,pstApplyIDInfo->AutoTeamID))
		{
			continue;
		}
		
		if (stPreIdx.iNum > stLastPreIdx.iNum)
		{
			memcpy(&stLastPreIdx, &stPreIdx, sizeof(stLastPreIdx));
			iAutoTeamIdx = iIdx;

			if (pstZoneTeam->stTeamInfo.MemberNum + stPreIdx.iNum >= MAX_TEAM_MEMBER)
			{
				break;
			}
		}
		else if (stPreIdx.iNum == stLastPreIdx.iNum)
		{
			if (stPreIdx.tTime < stLastPreIdx.tTime)
			{
				memcpy(&stLastPreIdx, &stPreIdx, sizeof(stLastPreIdx));
				iAutoTeamIdx = iIdx;
			}
		}
	}

	if (stLastPreIdx.iNum > 0)
	{
		return autoteam_pre_addteam(pstEnv, pstZoneTeam, &stLastPreIdx,  &pstAutoTeam->astAutoTeams[iAutoTeamIdx], 
									 iAutoTeamIdx);
	}

	//插入所有队列
	autoteam_insert_team_apply(pstEnv, pstZoneTeam, pstApply);
	
	return 0;
}

ACTIVEHELPER *find_active_by_autoteam(ZONESVRENV* pstEnv, int iAutoTeamID)
{
	int i;
	ACTIVEHELPER *pstActiveHelper;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	if(iAutoTeamID == 0)
	{
		return NULL;
	}
	for (i=0; i<pstObj->iActiveHelp; i++)
	{
		pstActiveHelper = (ACTIVEHELPER *)pstObj->sActiveHelp[i];
		if (pstActiveHelper->AutoTeamID == iAutoTeamID)
		{
			return pstActiveHelper;
		}
	}

	return NULL;
}

int autoteam_check_pworldlevel(ZONESVRENV* pstEnv, Player *pstPlayer,PWORLDDEF *pstPworldDef)
{
	UNUSED(pstEnv);
	
	if (pstPworldDef->MinLv > 0 && pstPlayer->stRoleData.Level < pstPworldDef->MinLv)
	{
		return -1;
	}

	if (pstPworldDef->MaxLv > 0 && pstPlayer->stRoleData.Level > pstPworldDef->MaxLv)
	{
		return -2;
	}
	
	return 0;
}

//在副本中的队伍自动组队
int autoteam_check_inpworld_byteam(ZONESVRENV* pstEnv, ZONEPWORLD *pstZonePworld, 
	ACTIVEHELPER *pstActiveHelp,ZoneTeam *pstZoneTeam)
{
	int i;
	ZONEPWORLDINST *pstZonePworldInst = NULL;

	autoteam_team_info_sync(pstEnv, pstZoneTeam,0);
	
	//队伍成员在副本的情况
	for(i=0; i<pstZoneTeam->iPworldNum; i++)
	{
		pstZonePworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstZoneTeam->stPworldInfo[i].iPworldInstID);
		if (pstZonePworldInst && 
			pstZonePworld->stPworldDef.PworldID != (unsigned int)pstZonePworldInst->stPworldIdx.iID)
		{
			return -1;
		}
	}
	
	//看看副本是否存在
	pstZonePworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstZoneTeam->iPworldInstID);
	if (!pstZonePworldInst)
	{
		//不存在返回成功
		return 0;
	}

	//队伍在副本里面,继续自动组队,只能组和自己副本一致的活动
	if( pstActiveHelp->AutoTeamID != pstZoneTeam->stAutoTeamInfo.AutoTeamID)
	{
		return -3;
	}

	return 0;	
}

int autoteam_diff_pworld_filter(int iDiffLimit, int iGrade)
{
	switch(iGrade)
	{
		case DIFF_PWORLD_GRADE_D:
			return (AUTO_TEAM_LIMIT_EASY & iDiffLimit);
			break;
		case DIFF_PWORLD_GRADE_C:
			return (AUTO_TEAM_LIMIT_NORM& iDiffLimit);
			break;
		case DIFF_PWORLD_GRADE_B:
			return (AUTO_TEAM_LIMIT_DIFF & iDiffLimit);
			break;
		case DIFF_PWORLD_GRADE_A:
			return (AUTO_TEAM_LIMIT_NIGHTMARE & iDiffLimit);
			break;
		default:
			break;
	}

	return 0;
}

int autoteam_apply_pworld_team_check(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, 
	Player *pstPlayer, ZONEPWORLD *pstZoneWorld, ACTIVEHELPER *pstActiveHelp)
{
	int i, iRet = 0;
	Player *pstPlayerMember;
	ZONEPWORLDINST *pstZonePworldInst = NULL;
	ZONEPWORLD* pstZonePworldPlayer;
	
	iRet = autoteam_check_inpworld_byteam( pstEnv, pstZoneWorld, pstActiveHelp,pstZoneTeam);
	if( iRet == -1)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "队伍成员还有未完成的副本，暂时不能自动组队");
		return -1;
	}
	else if( iRet < 0 )
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, 
				"只能组当前队伍所在副本的活动");
		return -1;
	}
	
	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstPlayerMember = z_name_player(pstEnv->pstAppCtx, pstEnv, 
			pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);

		if( !pstPlayerMember)
		{
			z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, 
				"队伍成员 %s 已经离线,不能自动组队", 
				pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
			
			return -1;
		}
		pstZonePworldInst = z_find_pworldinst2(pstEnv, pstPlayerMember->stRoleData.Map);
		if( pstZonePworldInst && 
			(unsigned int)pstZonePworldInst->stPworldIdx.iID == pstZoneWorld->stPworldDef.PworldID)
		{
			iRet = 0;
		}
		else
		{
			if(pstZonePworldInst)
			{
				pstZonePworldPlayer = z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
				if (pstZonePworldPlayer)
				{	
					if (is_battle_pworld(pstZonePworldPlayer))
					{
						z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, 
								"队伍成员 %s 在战场,不能自动组队", 
									pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
						return -1;
					}
				}
			}
	
			iRet = z_check_single_pworld(pstEnv, pstPlayerMember, &pstZoneWorld->stPworldDef, 0, NULL);
		}
		if (0 > iRet)
		{
			return -1;
		}
	}

	return 0;
}

int autoteam_apply_pworld_check(ZONESVRENV* pstEnv, Player *pstPlayer, 
	ACTIVEHELPER *pstActiveHelp, ZoneTeam *pstZoneTeam, AUTOTEAMAPPLYIDINFO *pstAutoTeamApplyIDInfo)
{
	int i;
	DIFFPWORLDDEF *pstDiffPworldDef;
	ZONEPWORLD *pstZoneWorld;
	ZONEIDX stIdx;
	
	pstDiffPworldDef = z_find_diff_pworld_def(pstEnv, pstActiveHelp->TaskPworldID);
	if(NULL == pstDiffPworldDef)
	{
		return -1;
	}

	for(i=0; i< MAX_DIFF_PWORLD_LIST; i++)
	{
		if(0 >= autoteam_diff_pworld_filter(pstAutoTeamApplyIDInfo->LimitFlag, i))
		{
			continue;
		}

		stIdx.iID = pstDiffPworldDef->PworldInfo[i].PworldID;
		stIdx.iPos = -1;
		pstZoneWorld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
		if (NULL == pstZoneWorld)
		{
			return -1;;
		}

		if (pstZoneTeam) 
		{
			if(0 > autoteam_apply_pworld_team_check(pstEnv, pstZoneTeam, 
				pstPlayer, pstZoneWorld, pstActiveHelp))
			{
				return -1;
			}
		}
		else
		{
			if (0 > z_check_single_pworld(pstEnv, pstPlayer, &pstZoneWorld->stPworldDef, 0, NULL))
			{
				return -1;
			}
		}	
	}

	return 0;
}
int autoteam_battle_apply(ZONESVRENV* pstEnv, Player *pstPlayer,AUTOTEAMAPPLYINFO *pstApply)
{
	ZoneTeam *pstZoneTeam = NULL;
	Player *pstOne = NULL;
	int i = 0;

	CSPKG stPkg;
	CSPKG *pstPkg = &stPkg;
	CSAUTOTEAMCLT *pstClt = &pstPkg->Body.AutoTeamClt;
	
	if( !(INST_MODE_ZONE_SVR == pstEnv->iInstMode &&
		pstEnv->pstConf->BattleFlag ) )
	{
		return -1;
	}

	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if(pstZoneTeam)
	{
		for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
		{
			pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
			if(pstOne)
			{
				if(0>player_battle_enter_req(pstEnv->pstAppCtx,pstEnv,&pstOne->stOnline.stFrameHead,
									pstOne,PLAYER_STATUS_BATTLE_LOGIN_READY,pstEnv->iBattlePveAutoteamID))
				{
					return 0;
				}
			}
		}
		//把队伍也同步过去
		team_shadow_sync_zone(pstEnv, pstZoneTeam,PLAYER_STATUS_BATTLE_LOGIN_READY,pstEnv->iBattlePveAutoteamID);
	}
	else
	{
		if(0>player_battle_enter_req(pstEnv->pstAppCtx,pstEnv,&pstPlayer->stOnline.stFrameHead,
									pstPlayer,PLAYER_STATUS_BATTLE_LOGIN_READY,pstEnv->iBattlePveAutoteamID))
		{
			return 0;
		}
	}

	//转发消息
	pstClt->AutoTeamCltOp = AUTOTEAM_CLT_OP_APPLY;
	pstClt->AutoTeamCltData.Apply = *pstApply;
	Z_CSHEAD_INIT(&pstPkg->Head, AUTOTEAM_CLT);	
	zone_proxy_request(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}
int autoteam_apply(ZONESVRENV* pstEnv, Player *pstPlayer,AUTOTEAMAPPLYINFO *pstApply)
{
	int i=0;
	ZoneTeam *pstZoneTeam;
	short anAutoTeamIDs[AUTO_TEAM_ID_MAX];
	ACTIVEHELPER *pstActiveHelp;
	Player *pstOne = NULL;

	if (0 >= pstApply->AutoTeamIDNum)
	{
		return -1;
	}

	if (pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal >= PLAYER_PK_PUBLIC_ENEMY &&
		player_is_zuolao(pstPlayer))
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "思过厅中的黑名玩家 %s 不能自动组队",pstPlayer->stRoleData.RoleName);
		return -1;
	}
		
	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if(pstZoneTeam)
	{
		for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
		{
			pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
			if(pstOne)
			{
				if (pstOne->stOnline.stAutoTeamInfo.tStart)
				{
					autoteam_reject(pstEnv, pstOne,0);
					z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstOne, SYS_MIDDLE, "个人自动组队取消");
				}

				if (pstOne->stRoleData.MiscInfo.PkInfo.PkVal >= PLAYER_PK_PUBLIC_ENEMY &&
					player_is_zuolao(pstOne))
				{
					z_sys_strf(pstEnv, pstOne, SYS_MIDDLE, "思过厅中的黑名玩家 %s 不能自动组队",pstOne->stRoleData.RoleName);
					return -1;
				}

			}
			
		}
	}

	memset(anAutoTeamIDs, 0, sizeof(anAutoTeamIDs));
	
	for (i=0; i<pstApply->AutoTeamIDNum; i++)
	{
		if (pstApply->AutoTeamIDInfos[i].AutoTeamID >= AUTO_TEAM_ID_MAX || anAutoTeamIDs[pstApply->AutoTeamIDInfos[i].AutoTeamID])
		{
			return -1;
		}
		anAutoTeamIDs[pstApply->AutoTeamIDInfos[i].AutoTeamID] = 1;

		pstActiveHelp = find_active_by_autoteam(pstEnv, pstApply->AutoTeamIDInfos[i].AutoTeamID);
		if (NULL == pstActiveHelp)
		{
			return -1;
		}

		if (pstActiveHelp->LevelLimit && pstPlayer->stRoleData.Level < pstActiveHelp->LevelLimit)
		{
			return -1;
		}

		if (pstActiveHelp->Type) //检查副本冷却
		{
			//自动组队目前排队的都是难度副本
			if(0 > autoteam_apply_pworld_check(pstEnv, pstPlayer, 
					pstActiveHelp, pstZoneTeam, &pstApply->AutoTeamIDInfos[i]))
			{
				return -1;
			}
		}
	}
	 
	if (pstZoneTeam)
	{
		if (pstPlayer != player_team_get_captain_player(pstEnv, pstZoneTeam))
		{
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "队伍成员不能自动组队");
			return -1;
		}
		
		if (pstZoneTeam->stAutoTeamInfo.tStart != 0)
		{
			autoteam_team_reject(pstEnv, pstPlayer, pstZoneTeam, 0);
		}

		if(0 == autoteam_battle_apply(pstEnv, pstPlayer,pstApply))
		{
			return 0;
		}
		else
		{
			if (pstZoneTeam->stTeamInfo.MemberNum >= MAX_TEAM_MEMBER)
			{
				autoteam_team_apply_by_fullmember( pstEnv,pstZoneTeam,pstApply);
				return 0;
			}
			return autoteam_team_apply(pstEnv, pstZoneTeam, pstApply);
		}
	}
	else
	{
		if (pstPlayer->stRoleData.MiscInfo.PworldInstID != 0)
		{
			if (z_find_pworld_inst_bysingle(pstEnv, pstPlayer))
			{
				z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "你还有未完成的副本，暂时不能自动组队");
				return -1;
			}
		}
		
		if (pstPlayer->stOnline.stAutoTeamInfo.tStart != 0)
		{
			autoteam_reject(pstEnv, pstPlayer,1);
		}	

		if(0 == autoteam_battle_apply(pstEnv, pstPlayer,pstApply))
		{
			return 0;
		}
		else
		{
			return autoteam_single_apply(pstEnv, pstPlayer, pstApply);
		}
	}

	return 0;
}

AutoTeamCreated *autoteam_find_created_by_playerid(AutoTeamOne *pstAutoTeamOne, int iPlayerID,
														AutoTeamSingle **ppSingle)
{
	int i, j;
	AutoTeamCreated *pstAutoTeamCreated;

	for (i=0; i<pstAutoTeamOne->iCreatedTeamNum; i++)
	{
		pstAutoTeamCreated = &pstAutoTeamOne->astAutoTeamCreated[i];
		for (j=0; j<pstAutoTeamCreated->nSingleNum; j++)
		{
			if (pstAutoTeamCreated->astSingle[j].iPlayerID == iPlayerID)
			{
				*ppSingle = &pstAutoTeamCreated->astSingle[j];
				return pstAutoTeamCreated;
			}
		}
	}
	
	return NULL;
}

AutoTeamPre *autoteam_find_pre_by_playerid(AutoTeamOne *pstAutoTeamOne, int iPlayerID,
														AutoTeamSingle **ppSingle)
{
	int i, j;
	AutoTeamPre *pstAutoTeamPre;

	for (i=0; i<pstAutoTeamOne->iPreTeamNum; i++)
	{
		pstAutoTeamPre = &pstAutoTeamOne->astAutoTeamPre[i];
		for (j=0; j<pstAutoTeamPre->nSingleNum; j++)
		{
			if (pstAutoTeamPre->astSingle[j].iPlayerID == iPlayerID)
			{
				*ppSingle = &pstAutoTeamPre->astSingle[j];
				return pstAutoTeamPre;
			}
		}
	}
	
	return NULL;
}


int autoteam_player_agree_send(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstAgreeOne)
{
	CSPKG stPkg;
	CSAUTOTEAMSVR *pstAutoTeamSvr = &stPkg.Body.AutoTeamSvr;
	CSAUTOTEAMPREAGREE *pstAgree = &pstAutoTeamSvr->AutoTeamSvrData.PreAgree;

	pstAutoTeamSvr->AutoTeamSvrOp = AUTOTEAM_SVR_OP_PRE_AGREE;
	STRNCPY(pstAgree->RoleName, pstAgreeOne->stRoleData.RoleName, sizeof(pstAgree->RoleName));

	Z_CSHEAD_INIT(&stPkg.Head, AUTOTEAM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int autoteam_player_cancel_send(ZONESVRENV* pstEnv, Player *pstPlayer, int iCancelFlag)
{
	CSPKG stPkg;
	CSAUTOTEAMSVR *pstAutoTeamSvr = &stPkg.Body.AutoTeamSvr;
	CSAUTOTEAMCANCELSVR *pstCancel = &pstAutoTeamSvr->AutoTeamSvrData.CancelSvr;

	pstAutoTeamSvr->AutoTeamSvrOp = AUTOTEAM_SVR_OP_CANCEL;
	pstCancel->CancelFlag = iCancelFlag;
	
	Z_CSHEAD_INIT(&stPkg.Head, AUTOTEAM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int autoteam_player_dec_send(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstDec)
{
	CSPKG stPkg;
	CSAUTOTEAMSVR *pstAutoTeamSvr = &stPkg.Body.AutoTeamSvr;
	CSAUTOTEAMPREDEC *pstPreDec = &pstAutoTeamSvr->AutoTeamSvrData.PreDec;

	pstAutoTeamSvr->AutoTeamSvrOp = AUTOTEAM_SVR_OP_PRE_DEC;
	STRNCPY(pstPreDec->RoleName, pstDec->stRoleData.RoleName, sizeof(pstPreDec->RoleName));
	
	Z_CSHEAD_INIT(&stPkg.Head, AUTOTEAM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}	

int autoteam_player_create_send(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstCreateOne)
{
	CSPKG stPkg;
	CSAUTOTEAMSVR *pstAutoTeamSvr = &stPkg.Body.AutoTeamSvr;
	CSAUTOTEAMCREATESVR *pstCreate = &pstAutoTeamSvr->AutoTeamSvrData.Create;

	pstAutoTeamSvr->AutoTeamSvrOp = AUTOTEAM_SVR_OP_CREATE;
	STRNCPY(pstCreate->RoleName, pstCreateOne->stRoleData.RoleName, sizeof(pstCreate->RoleName));
	
	Z_CSHEAD_INIT(&stPkg.Head, AUTOTEAM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int autoteam_player_create_reject_send(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstCancelOne)
{
	CSPKG stPkg;
	CSAUTOTEAMSVR *pstAutoTeamSvr = &stPkg.Body.AutoTeamSvr;
	CSAUTOTEAMCREATECANCELSVR *pstCreateCancel = &pstAutoTeamSvr->AutoTeamSvrData.CreateCancel;

	pstAutoTeamSvr->AutoTeamSvrOp = AUTOTEAM_SVR_OP_CREATE_CANCEL;
	STRNCPY(pstCreateCancel->RoleName, pstCancelOne->stRoleData.RoleName, sizeof(pstCreateCancel->RoleName));
	
	Z_CSHEAD_INIT(&stPkg.Head, AUTOTEAM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

short autoteam_find_apply_limit(AUTOTEAMAPPLYINFO *pstApply, int iAutoTeamID)
{
	int i;

	for (i=0; i<pstApply->AutoTeamIDNum; i++)
	{
		if (pstApply->AutoTeamIDInfos[i].AutoTeamID == iAutoTeamID)
		{
			return pstApply->AutoTeamIDInfos[i].LimitFlag;
		}
	}

	return 0;
}

int autoteam_pworld_grade(short nDiffLimit)
{
	int iPworldGrade;
	
	if (0 == nDiffLimit)
	{
		iPworldGrade = DIFF_PWORLD_GRADE_C;
	}
	else if(AUTO_TEAM_LIMIT_NIGHTMARE & nDiffLimit)
	{
		iPworldGrade = DIFF_PWORLD_GRADE_A;
	}
	else if (AUTO_TEAM_LIMIT_DIFF & nDiffLimit)
	{
		iPworldGrade = DIFF_PWORLD_GRADE_B;
	}
	else if (AUTO_TEAM_LIMIT_NORM & nDiffLimit)
	{
		iPworldGrade = DIFF_PWORLD_GRADE_C;
	}
	else
	{
		iPworldGrade = DIFF_PWORLD_GRADE_D;
	}

	return iPworldGrade;
}

int autoteam_diffpworld_grade_id_get(ZONESVRENV* pstEnv, int iDiffPworldID, int iLimit)
{
	int iGrade;
	DIFFPWORLDDEF *pstDiffPworldDef = z_find_diff_pworld_def(pstEnv, iDiffPworldID);

	if(NULL == pstDiffPworldDef)
	{
		return -1;
	}
	
	iGrade = autoteam_pworld_grade(iLimit);
	if(iGrade < 0 || iGrade >= MAX_DIFF_PWORLD_LIST)
	{
		return -1;
	}

	return pstDiffPworldDef->PworldInfo[iGrade].PworldID;
}

//自动组队成功之后,改变了人,都发一边
int autoteam_team_successed_chg_member(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam)
{
	int i;
	CSPKG stPkg;
	CSAUTOTEAMSVR *pstAutoTeamSvr = &stPkg.Body.AutoTeamSvr;
	CSAUTOTEAMSUCC *pstSucc = &pstAutoTeamSvr->AutoTeamSvrData.Succ;
	ACTIVEHELPER *pstActiveHelp = NULL;
	int iAutoTeamID = 0;
	Player *pstOne;

	if( pstZoneTeam->stAutoTeamInfo.TransAutoTeamID <=0 )
	{
		return -1;
	}
	
	iAutoTeamID = pstZoneTeam->stAutoTeamInfo.TransAutoTeamID;
	pstActiveHelp = find_active_by_autoteam(pstEnv, iAutoTeamID);
	if (NULL == pstActiveHelp)
	{
		return -1;
	}

	pstAutoTeamSvr->AutoTeamSvrOp = AUTOTEAM_SVR_OP_SUCC;
	pstSucc->OperType = 1;
	pstSucc->AutoTeamID = iAutoTeamID;
	pstSucc->PworldGrade = pstZoneTeam->stAutoTeamInfo.iPworldGrade;
	pstSucc->Num = 0;
	Z_CSHEAD_INIT(&stPkg.Head, AUTOTEAM_SVR);

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		autoteam_get_prememinfo_frommember(&pstSucc->PreMemInfos[pstSucc->Num++],
											   &pstZoneTeam->stTeamInfo.TeamMember[i],
											   pstZoneTeam);
	}
	for(i=0;i<pstZoneTeam->stTeamInfo.MemberNum;i++)
	{
		pstOne = z_name_player(pstEnv->pstAppCtx,pstEnv,pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if(pstOne)
		{
			if( pstOne == player_team_get_captain_player(pstEnv,pstZoneTeam) &&
				pstZoneTeam->stAutoTeamInfo.tStart > 0)
			{
				continue;
			}
			//保存只能传一次
			pstOne->stOnline.stAutoTeamInfo.Stat = 1;
			pstOne->stOnline.stAutoTeamInfo.AutoTeamID = iAutoTeamID;
			z_cltmsg_send(pstEnv, pstOne, &stPkg);
		}
	}
	

	return 0;
}

//自动组队成功之后,还加了人
int autoteam_team_successed_add_member(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, Player *pstOne)
{
	int i;
	CSPKG stPkg;
	CSAUTOTEAMSVR *pstAutoTeamSvr = &stPkg.Body.AutoTeamSvr;
	CSAUTOTEAMSUCC *pstSucc = &pstAutoTeamSvr->AutoTeamSvrData.Succ;
	ACTIVEHELPER *pstActiveHelp = NULL;
	int iAutoTeamID = 0;

	if( pstZoneTeam->stAutoTeamInfo.AutoTeamID <=0 ||
		pstZoneTeam->stAutoTeamInfo.tStart > 0)
	{
		return -1;
	}
	
	iAutoTeamID = pstZoneTeam->stAutoTeamInfo.AutoTeamID;
	pstActiveHelp = find_active_by_autoteam(pstEnv, iAutoTeamID);
	if (NULL == pstActiveHelp)
	{
		return -1;
	}

	pstAutoTeamSvr->AutoTeamSvrOp = AUTOTEAM_SVR_OP_SUCC;
	pstSucc->OperType = 0;
	pstSucc->AutoTeamID = iAutoTeamID;
	pstSucc->PworldGrade = pstZoneTeam->stAutoTeamInfo.bOPFlag;
	pstSucc->Num = 0;
	Z_CSHEAD_INIT(&stPkg.Head, AUTOTEAM_SVR);

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		autoteam_get_prememinfo_frommember(&pstSucc->PreMemInfos[pstSucc->Num++],
											   &pstZoneTeam->stTeamInfo.TeamMember[i],
											   pstZoneTeam);
	}

	z_cltmsg_send(pstEnv, pstOne, &stPkg);
	//保存只能传一次
	pstOne->stOnline.stAutoTeamInfo.Stat = 1;
	pstOne->stOnline.stAutoTeamInfo.AutoTeamID = iAutoTeamID;

	return 0;
}

int autoteam_team_succ(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, int iAutoTeamID, short nDiffLimit)
{
	Player *pstOne;
	int i;
	CSPKG stPkg;
	CSAUTOTEAMSVR *pstAutoTeamSvr = &stPkg.Body.AutoTeamSvr;
	CSAUTOTEAMSUCC *pstSucc = &pstAutoTeamSvr->AutoTeamSvrData.Succ;
	ACTIVEHELPER *pstActiveHelp = NULL;

	//保存一下难度
	pstZoneTeam->stAutoTeamInfo.bOPFlag = autoteam_pworld_grade(nDiffLimit);
	pstZoneTeam->stAutoTeamInfo.AutoTeamID = iAutoTeamID;
	
	pstActiveHelp = find_active_by_autoteam(pstEnv, iAutoTeamID);
	if (NULL == pstActiveHelp)
	{
		return -1;
	}

	pstZoneTeam->stAutoTeamInfo.TransAutoTeamID = iAutoTeamID;
	pstZoneTeam->stAutoTeamInfo.iPworldGrade = pstZoneTeam->stAutoTeamInfo.bOPFlag;

	pstAutoTeamSvr->AutoTeamSvrOp = AUTOTEAM_SVR_OP_SUCC;
	pstSucc->OperType = 0;
	pstSucc->AutoTeamID = iAutoTeamID;
	pstSucc->PworldGrade = pstZoneTeam->stAutoTeamInfo.bOPFlag;
	pstSucc->Num = 0;
	Z_CSHEAD_INIT(&stPkg.Head, AUTOTEAM_SVR);

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		autoteam_get_prememinfo_frommember(&pstSucc->PreMemInfos[pstSucc->Num++],
											   &pstZoneTeam->stTeamInfo.TeamMember[i],
											   pstZoneTeam);
	}

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (pstOne)
		{
			z_cltmsg_send(pstEnv, pstOne, &stPkg);

			//保存只能传一次
			pstOne->stOnline.stAutoTeamInfo.Stat = 1;
			pstOne->stOnline.stAutoTeamInfo.AutoTeamID = iAutoTeamID;
		}
	}

	team_shadow_chg_true_team(pstEnv, pstZoneTeam);

	return 0;
}

int autoteam_created_succ(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam,
								AutoTeamCreated *pstAutoTeamCreated, int iAutoTeamID)
{
	int i;
	TEAMMEMBER *pstTeamMember;
	Player *pstOne;
	short nDiffLimit;
	short nDiffLimitTmp = 0;
	
	if (pstZoneTeam->stTeamInfo.MemberNum + pstAutoTeamCreated->nSingleNum > MAX_TEAM_MEMBER)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "created team num err");
		return -1;
	}

	for (i=0; i<pstAutoTeamCreated->nSingleNum; i++)
	{
		pstTeamMember = &pstZoneTeam->stTeamInfo.TeamMember[pstZoneTeam->stTeamInfo.MemberNum];

		//找到这人,并且保证这个家伙不再队伍里
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamCreated->astSingle[i].iPlayerID);
		if (pstOne && 
			!player_team_get_member(pstZoneTeam, pstOne->stRoleData.RoleName, NULL) )
		{
			player_team_sync_member_info(pstEnv, pstTeamMember, pstOne);
			pstZoneTeam->stTeamInfo.MemberNum++;
			pstOne->stRoleData.MiscInfo.TeamID = pstZoneTeam->stTeamInfo.TeamID;
			pstOne->stRoleData.MiscInfo.TeamIdx = pstZoneTeam->iTeamIdx;

			//设置下玩家的进入副本的时候可以有10秒的倒计时
			pstOne->stRoleData.MiscInfo.InPworldSafeTime = MAX_ENTER_PWORLD_SAFE_TIME;

			//战场排队
			flush_battle_queue_team_info(pstEnv, pstOne, pstZoneTeam, 1);
			flush_battle_pworld_queue(pstEnv, pstOne, pstZoneTeam);
		}
	}

	player_team_sync(pstEnv, pstZoneTeam);
	player_team_all_action(pstEnv, pstZoneTeam);

	nDiffLimit = autoteam_limit_filter(autoteam_find_apply_limit(&pstZoneTeam->stAutoTeamInfo.stApply, iAutoTeamID));
	nDiffLimitTmp = nDiffLimit;
	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (pstOne)
		{
			nDiffLimit &= autoteam_limit_filter(autoteam_find_apply_limit(&pstOne->stOnline.stAutoTeamInfo.stApply, iAutoTeamID));
			memset(&pstOne->stOnline.stAutoTeamInfo, 0, sizeof(pstOne->stOnline.stAutoTeamInfo));
		}
	}

	//清除
	autoteam_team_del(pstEnv, pstZoneTeam, -1);
	memset(&pstZoneTeam->stAutoTeamInfo, 0, sizeof(pstZoneTeam->stAutoTeamInfo));

	if(!nDiffLimit)
	{
		nDiffLimit = nDiffLimitTmp;
	}
	autoteam_team_succ(pstEnv, pstZoneTeam, iAutoTeamID, nDiffLimit);

	return 0;
}

int autoteam_created_agree(ZONESVRENV* pstEnv, Player *pstPlayer, AutoTeamCreated *pstAutoTeamCreated, int iAutoTeamID)
{
	int i;
	Player *pstOne;
	ZoneTeam *pstZoneTeam;
	TMEMBLOCK *pstMemBlock;
	int iFlag = 0;

	pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstTeamPool, pstAutoTeamCreated->iTeamID);
	if (0 == pstMemBlock->fValid)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "zone team null");
		return -1;
	}

	pstZoneTeam = (ZoneTeam *)pstMemBlock->szData;

	if (pstZoneTeam->stTeamInfo.MemberNum + pstAutoTeamCreated->nSingleNum > MAX_TEAM_MEMBER)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "created team num err");
		return -1;
	}
	else if (pstZoneTeam->stTeamInfo.MemberNum + pstAutoTeamCreated->nSingleNum == MAX_TEAM_MEMBER)
	{
		for (i=0; i<pstAutoTeamCreated->nSingleNum; i++)
		{
			pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamCreated->astSingle[i].iPlayerID);
			if (pstOne)
			{
				/*if (0 == (AUTOTEAM_OP_AGREE & pstOne->stOnline.stAutoTeamInfo.bOPFlag))
				{
					break;
				}*/
			}
			else
			{
				break;
			}
		}

		if (i == pstAutoTeamCreated->nSingleNum)
		{
			iFlag = 1;
		}
	}

	if (iFlag) //全同意
	{
		autoteam_created_succ(pstEnv, pstZoneTeam, pstAutoTeamCreated, iAutoTeamID);
	}
	else
	{
		for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
		{
			pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
			if (pstOne)
			{
				autoteam_player_agree_send(pstEnv, pstOne, pstPlayer);
			}
		}
		
		for (i=0; i<pstAutoTeamCreated->nSingleNum; i++)
		{
			pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamCreated->astSingle[i].iPlayerID);
			if (pstOne)
			{
				autoteam_player_agree_send(pstEnv, pstOne, pstPlayer);
			}
		}
	}
	
	return 0;
}

int autoteam_pre_succ(ZONESVRENV* pstEnv, AutoTeamPre *pstAutoTeamPre, int iAutoTeamID)
{
	int i;
	Player *pstOne;
	ZoneTeam *pstZoneTeam;
	TEAMMEMBER *pstTeamMember;
	AutoTeamOne *pstAutoTeamOne = &pstEnv->pstZoneObj->stAutoTeam.astAutoTeams[iAutoTeamID];
	short nDiffLimit;
	short nDiffLimitTmp;

	if (pstAutoTeamPre->nSingleNum > MAX_TEAM_MEMBER)
	{
		return -1;
	}

	pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[0].iPlayerID);
	if (NULL == pstOne)
	{
		return -1;
	}

	pstZoneTeam = player_team_create(pstEnv, pstOne);
	if (NULL == pstZoneTeam)
	{
		return -1;
	}
	
	nDiffLimit = autoteam_limit_filter(autoteam_find_apply_limit(&pstOne->stOnline.stAutoTeamInfo.stApply, iAutoTeamID));
	nDiffLimitTmp = nDiffLimit;
	memset(&pstOne->stOnline.stAutoTeamInfo, 0, sizeof(pstOne->stOnline.stAutoTeamInfo));
	
	for (i=1; i<pstAutoTeamPre->nSingleNum; i++)
	{
		pstTeamMember = &pstZoneTeam->stTeamInfo.TeamMember[pstZoneTeam->stTeamInfo.MemberNum];

		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[i].iPlayerID);
		if (pstOne)
		{
			player_team_sync_member_info(pstEnv, pstTeamMember, pstOne);
			pstZoneTeam->stTeamInfo.MemberNum++;
			pstOne->stRoleData.MiscInfo.TeamID = pstZoneTeam->stTeamInfo.TeamID;
			pstOne->stRoleData.MiscInfo.TeamIdx = pstZoneTeam->iTeamIdx;

			nDiffLimit &= autoteam_limit_filter(autoteam_find_apply_limit(&pstOne->stOnline.stAutoTeamInfo.stApply, iAutoTeamID));
			memset(&pstOne->stOnline.stAutoTeamInfo, 0, sizeof(pstOne->stOnline.stAutoTeamInfo));

			//战场排队
			flush_battle_queue_team_info(pstEnv, pstOne, pstZoneTeam, 1);
			flush_battle_pworld_queue(pstEnv, pstOne, pstZoneTeam);
		}
	}

	//清除
	autoteam_pre_del(pstAutoTeamOne, pstAutoTeamPre);
	memset(&pstZoneTeam->stAutoTeamInfo, 0, sizeof(pstZoneTeam->stAutoTeamInfo));

	if(!nDiffLimit)
	{
		nDiffLimit = nDiffLimitTmp;
	}
	autoteam_team_succ(pstEnv, pstZoneTeam, iAutoTeamID, nDiffLimit);

	player_team_sync(pstEnv, pstZoneTeam);
	player_team_all_action(pstEnv, pstZoneTeam);


	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (pstOne)
		{
			team_create_succ_req(pstEnv,pstZoneTeam,pstOne);
		}
	}
	return 0;
}

int autoteam_pre_agree(ZONESVRENV* pstEnv, Player *pstPlayer, AutoTeamPre *pstAutoTeamPre, int iAutoTeamID)
{
	int i;
	Player *pstOne;
	int iFlag = 0;

	if (pstAutoTeamPre->nSingleNum > MAX_TEAM_MEMBER)
	{
		return -1;
	}
	else if (pstAutoTeamPre->nSingleNum == MAX_TEAM_MEMBER)
	{
		for (i=0; i<pstAutoTeamPre->nSingleNum; i++)
		{
			pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[i].iPlayerID);
			if (pstOne)
			{
				/*if (0 == (AUTOTEAM_OP_AGREE & pstOne->stOnline.stAutoTeamInfo.bOPFlag))
				{
					break;
				}*/
			}
			else
			{
				break;
			}
		}

		if (i == pstAutoTeamPre->nSingleNum)
		{
			iFlag = 1;
		}
	}
	
	if (iFlag) //全同意
	{
		autoteam_pre_leader(pstEnv, pstAutoTeamPre, 0);
		autoteam_pre_succ(pstEnv, pstAutoTeamPre, iAutoTeamID);
	}
	else
	{
		for (i=0; i<pstAutoTeamPre->nSingleNum; i++)
		{
			pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[i].iPlayerID);
			if (pstOne)
			{
				autoteam_player_agree_send(pstEnv, pstOne, pstPlayer);
			}
		}
	}
	
	return 0;
}


int autoteam_agree(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	AutoTeamInfo *pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;
	AutoTeamSingle *pstSingle;
	AutoTeamCreated *pstAutoTeamCreated;
	AutoTeamPre *pstAutoTeamPre;
	AutoTeam *pstAutoTeam = &pstEnv->pstZoneObj->stAutoTeam;

	if (AUTOTEAM_OP_AGREE & pstAutoTeamInfo->bOPFlag)
	{
		return -1;
	}

	pstAutoTeamInfo->bOPFlag |= AUTOTEAM_OP_AGREE;

	switch(pstAutoTeamInfo->Stat)
	{
		case 1:
			pstAutoTeamCreated = autoteam_find_created_by_playerid(&pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID], 
																	pstPlayer->iMemID, &pstSingle);
			if (pstAutoTeamCreated)
			{
				return autoteam_created_agree(pstEnv, pstPlayer, pstAutoTeamCreated, pstAutoTeamInfo->AutoTeamID);
			}
			break;
		case 2:
			pstAutoTeamPre = autoteam_find_pre_by_playerid(&pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID], 
																	pstPlayer->iMemID, &pstSingle);
			if (pstAutoTeamPre)
			{
				return autoteam_pre_agree(pstEnv, pstPlayer, pstAutoTeamPre, pstAutoTeamInfo->AutoTeamID);
			}
			break;
		default:
			break;
	}

	//给自己转发一下
	autoteam_player_agree_send(pstEnv, pstPlayer, pstPlayer);

	return 0;
}

int autoteam_single_addcreated_pre(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeam, AutoTeamCreated *pstAutoTeamCreatedOri,
											AutoTeamOne *pstAutoTeamOne, AutoTeamPreIdx *pstPreIdx,int AutoTeamID)
{
	int i;
	Player *pstOne;
	AutoTeamSingle *pstSingle;
	AutoTeamCreated *pstAutoTeamCreated = &pstPreIdx->stTeamCreated;
	AutoTeamPre *pstAutoTeamPre = &pstPreIdx->stTeamPre;
	
	memset(pstPreIdx, 0, sizeof(*pstPreIdx));
	
	memcpy(pstAutoTeamCreated, pstAutoTeamCreatedOri, sizeof(*pstAutoTeamCreated));

	for (i=0; i<pstAutoTeamOne->iSingleNum; i++)
	{
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamOne->astAutoTeamSingle[i].iPlayerID);	
		if (NULL == pstOne)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "id err");
			continue;
		}
		
		if (0 == autoteam_created_limit_co(pstEnv, pstOne, pstAutoTeamOne->astAutoTeamSingle[i].nLimitFlag, 
										pstZoneTeam, pstAutoTeamCreated,AutoTeamID))
		{
			continue;
		}

		pstPreIdx->iNum++;
				
		pstSingle = &pstAutoTeamCreated->astSingle[pstAutoTeamCreated->nSingleNum++];
		memcpy(pstSingle, &pstAutoTeamOne->astAutoTeamSingle[i], sizeof(*pstSingle));

		//记录下新加的人
		memcpy(&pstAutoTeamPre->astSingle[pstAutoTeamPre->nSingleNum++],
				pstSingle, sizeof(pstAutoTeamPre->astSingle[0]));

		if (pstZoneTeam->stTeamInfo.MemberNum + pstAutoTeamCreated->nSingleNum >= MAX_TEAM_MEMBER)
		{
			break;
		}

	}

	
	return 0;
}

int autoteam_single_addcreated(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, AutoTeamCreated *pstAutoTeamCreated,
	                                  AutoTeamPreIdx *pstPreIdx, AutoTeamOne *pstAutoTeamOne, int iAutoTeamIdx)
{
	int i;
	Player *pstOne;
	AutoTeamPre *pstAutoTeamPre;
	UNUSED(pstAutoTeamOne);

	autoteam_team_del(pstEnv, pstZoneTeam, iAutoTeamIdx);

	pstZoneTeam->stAutoTeamInfo.AutoTeamID = iAutoTeamIdx;
	pstZoneTeam->stAutoTeamInfo.Stat = 1;

	pstAutoTeamPre = &pstPreIdx->stTeamPre;
	memcpy(pstAutoTeamCreated, &pstPreIdx->stTeamCreated, sizeof(*pstAutoTeamCreated));

	for (i=0; i<pstAutoTeamPre->nSingleNum; i++)
	{
		autoteam_single_del(pstEnv, pstAutoTeamPre->astSingle[i].iPlayerID);

		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[i].iPlayerID);

		if (pstOne)
		{
			pstOne->stOnline.stAutoTeamInfo.Stat = 1;
			pstOne->stOnline.stAutoTeamInfo.AutoTeamID = iAutoTeamIdx;
			autoteam_created_mem_info(pstEnv, pstOne, pstZoneTeam, pstAutoTeamCreated);
		}
	}

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);

		if (pstOne)
		{
			autoteam_pre_mem_info(pstEnv, pstOne, pstAutoTeamPre);
		}
	}
	
	return 0;
}

int autoteam_pre_addcreated_pre(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeam, AutoTeamCreated *pstAutoTeamCreatedOri,
											AutoTeamOne *pstAutoTeamOne, AutoTeamPreIdx *pstPreIdx,int AutoTeamID)
{
	int i, j;
	AutoTeamCreated *pstAutoTeamCreated = &pstPreIdx->stTeamCreated;
	AutoTeamPre *pstAutoTeamPre;
	Player *pstOne;
	AutoTeamSingle *pstSingle;

	memset(pstPreIdx, 0, sizeof(*pstPreIdx));
	
	for (i=0; i<pstAutoTeamOne->iPreTeamNum; i++)
	{
		memcpy(pstAutoTeamCreated, pstAutoTeamCreatedOri, sizeof(*pstAutoTeamCreated));
		
		pstAutoTeamPre = &pstAutoTeamOne->astAutoTeamPre[i];
		if (pstAutoTeamPre->OpFlag || pstAutoTeamPre->nSingleNum + pstZoneTeam->stTeamInfo.MemberNum + pstAutoTeamCreated->nSingleNum > MAX_TEAM_MEMBER)
		{
			continue;
		}

		for (j=0; j<pstAutoTeamPre->nSingleNum; j++)
		{
			pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[j].iPlayerID);	
			if (NULL == pstOne)
			{
				break;
			}

			if (0 == autoteam_created_limit_co(pstEnv, pstOne, pstAutoTeamPre->astSingle[j].nLimitFlag, pstZoneTeam, pstAutoTeamCreated,AutoTeamID))
			{
				break;
			}

			pstSingle = &pstAutoTeamCreated->astSingle[pstAutoTeamCreated->nSingleNum++];
			memcpy(pstSingle, &pstAutoTeamPre->astSingle[j], sizeof(*pstSingle));
		}

		if (j == pstAutoTeamPre->nSingleNum)
		{
			if (pstAutoTeamPre->nSingleNum > pstPreIdx->iNum)
			{
				pstPreIdx->iNum = pstAutoTeamPre->nSingleNum;
				pstPreIdx->tTime = pstAutoTeamPre->tTime;
				pstPreIdx->iIdx = i;
			}
			else if (pstAutoTeamPre->nSingleNum == pstPreIdx->iNum)
			{
				if (pstAutoTeamPre->tTime < pstPreIdx->tTime)
				{
					pstPreIdx->iNum = pstAutoTeamPre->nSingleNum;
					pstPreIdx->tTime = pstAutoTeamPre->tTime;
					pstPreIdx->iIdx = i;
				}
			}
		}
	}
	
	return 0;
}

int autoteam_pre_addcreated(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeam, AutoTeamCreated *pstAutoTeamCreated,
						AutoTeamPreIdx *pstPreIdx, AutoTeamOne *pstAutoTeamOne, int iAutoTeamIdx)
{
	int i;
	AutoTeamPre *pstAutoTeamPre = &pstAutoTeamOne->astAutoTeamPre[pstPreIdx->iIdx];
	Player *pstOne;

	memcpy(pstAutoTeamCreated, &pstPreIdx->stTeamCreated, sizeof(*pstAutoTeamCreated));

	for (i=0; i<pstAutoTeamCreated->nSingleNum; i++)
	{
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamCreated->astSingle[i].iPlayerID);

		if (pstOne)
		{
			pstOne->stOnline.stAutoTeamInfo.Stat = 1;
			pstOne->stOnline.stAutoTeamInfo.AutoTeamID = iAutoTeamIdx;
			autoteam_created_mem_info(pstEnv, pstOne, pstZoneTeam, pstAutoTeamCreated);
		}
	}

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);

		if (pstOne)
		{
			autoteam_created_mem_info(pstEnv, pstOne, pstZoneTeam, pstAutoTeamCreated);
		}
	}

	autoteam_pre_del(pstAutoTeamOne, pstAutoTeamPre);

	//测试日志,为了查问题
	for(i=0;i<pstAutoTeamOne->iCreatedTeamNum ;i++)
	{
		tlog_info(pstEnv->pstLogCat, 0, 0, "nSingleNum:%d, iTeamID:%d,AutoTeamID:%d,Stat:%d",
			pstAutoTeamOne->astAutoTeamCreated[i].nSingleNum,
			pstAutoTeamOne->astAutoTeamCreated[i].iTeamID,
			pstZoneTeam->stAutoTeamInfo.AutoTeamID,
			pstZoneTeam->stAutoTeamInfo.Stat);	
	}
	return 0;
}
	

int autoteam_created_loose_limit(ZONESVRENV *pstEnv, AutoTeamOne *pstAutoTeamOne,
										AutoTeamCreated *pstAutoTeamCreated, int iAutoTeamIdx)
{
	TMEMBLOCK *pstMemBlock;
	ZoneTeam *pstZoneTeam;
	AutoTeamPreIdx stPreIdx;

	if (pstAutoTeamCreated->OpFlag)
	{
		return 0;
	}

	pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstTeamPool, pstAutoTeamCreated->iTeamID);
	if (0 == pstMemBlock->fValid)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "zone team null");
		return -1;
	}

	pstZoneTeam = (ZoneTeam *)pstMemBlock->szData;
	if (pstZoneTeam->stTeamInfo.MemberNum + pstAutoTeamCreated->nSingleNum >= MAX_TEAM_MEMBER)
	{
		return 0;	
	}

	autoteam_single_addcreated_pre(pstEnv, pstZoneTeam, pstAutoTeamCreated, pstAutoTeamOne, &stPreIdx,iAutoTeamIdx);

	if (stPreIdx.iNum > 0)
	{
		return autoteam_single_addcreated(pstEnv, pstZoneTeam, pstAutoTeamCreated, &stPreIdx,  pstAutoTeamOne, iAutoTeamIdx);
	}

	autoteam_pre_addcreated_pre(pstEnv, pstZoneTeam, pstAutoTeamCreated, pstAutoTeamOne, &stPreIdx,iAutoTeamIdx);

	if (stPreIdx.iNum > 0)
	{
		return autoteam_pre_addcreated(pstEnv, pstZoneTeam, pstAutoTeamCreated, &stPreIdx,  pstAutoTeamOne, iAutoTeamIdx);
	}

	return 0;
}


int autoteam_created_reject(ZONESVRENV* pstEnv, Player *pstPlayer, 
								AutoTeamCreated *pstAutoTeamCreated, int iAutoTeamID)
{
	int i;
	Player *pstOne;
	ZoneTeam *pstZoneTeam;
	TMEMBLOCK *pstMemBlock;
	AutoTeam *pstAutoTeam = &pstEnv->pstZoneObj->stAutoTeam;
	int iFlag = 0;

	if (pstAutoTeamCreated->OpFlag)
	{
		iFlag = 1;
		pstAutoTeamCreated->OpFlag = 0;
	}

	for (i=pstAutoTeamCreated->nSingleNum-1; i>=0; i--)
	{
		if (pstAutoTeamCreated->astSingle[i].iPlayerID == pstPlayer->iMemID)
		{
			pstAutoTeamCreated->nSingleNum--;
			if (i != pstAutoTeamCreated->nSingleNum)
			{
				memcpy(&pstAutoTeamCreated->astSingle[i], 
					&pstAutoTeamCreated->astSingle[pstAutoTeamCreated->nSingleNum],
					sizeof(pstAutoTeamCreated->astSingle[0]));
			}
		}
		else
		{
			pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamCreated->astSingle[i].iPlayerID);

			if (pstOne)
			{
				autoteam_player_dec_send(pstEnv, pstOne, pstPlayer);

				pstOne->stOnline.stAutoTeamInfo.bOPFlag &= ~(AUTOTEAM_OP_CREATE|AUTOTEAM_OP_CREATE_AGREE);
				if (iFlag)
				{
					autoteam_player_create_reject_send(pstEnv, pstOne, pstPlayer);
				}
			}
		}
	}

	autoteam_player_cancel_send(pstEnv, pstPlayer, 0);	
	
	autoteam_single_del(pstEnv, pstPlayer->iMemID);
	memset(&pstPlayer->stOnline.stAutoTeamInfo, 0, sizeof(pstPlayer->stOnline.stAutoTeamInfo));

	pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstTeamPool, pstAutoTeamCreated->iTeamID);
	if (0 == pstMemBlock->fValid)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "zone team null");
		return -1;
	}
	
	pstZoneTeam = (ZoneTeam *)pstMemBlock->szData;

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (pstOne)
		{
			autoteam_player_dec_send(pstEnv, pstOne, pstPlayer);

			pstOne->stOnline.stAutoTeamInfo.bOPFlag &= ~(AUTOTEAM_OP_CREATE|AUTOTEAM_OP_CREATE_AGREE);
			if (iFlag && pstZoneTeam->stTeamInfo.TeamMember[i].Position == TEAM_POSITION_CAPTAIN)
			{
				autoteam_player_create_reject_send(pstEnv, pstOne, pstPlayer);
			}
		}
	}

	autoteam_created_loose_limit(pstEnv, &pstAutoTeam->astAutoTeams[iAutoTeamID], pstAutoTeamCreated, iAutoTeamID);
	
	return 0;
}

int autoteam_pre_reject(ZONESVRENV* pstEnv, Player *pstPlayer, 
								AutoTeamPre *pstAutoTeamPre, int iAutoTeamID)
{
	int i;
	Player *pstOne;
	int iFlag = 0;
	AutoTeam *pstAutoTeam = &pstEnv->pstZoneObj->stAutoTeam;

	if (pstAutoTeamPre->OpFlag)
	{
		iFlag = 1;
		pstAutoTeamPre->OpFlag = 0;
	}

	for (i=pstAutoTeamPre->nSingleNum-1; i>=0; i--)
	{
		if (pstAutoTeamPre->astSingle[i].iPlayerID == pstPlayer->iMemID)
		{
			pstAutoTeamPre->nSingleNum--;
			if (i != pstAutoTeamPre->nSingleNum)
			{
				memmove(&pstAutoTeamPre->astSingle[i], 
					&pstAutoTeamPre->astSingle[i+1],
					sizeof(pstAutoTeamPre->astSingle[0])*pstAutoTeamPre->nSingleNum );
			}
		}
		else
		{
			pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[i].iPlayerID);
			if (pstOne)
			{
				autoteam_player_dec_send(pstEnv, pstOne, pstPlayer);

				pstOne->stOnline.stAutoTeamInfo.bOPFlag &= ~(AUTOTEAM_OP_CREATE|AUTOTEAM_OP_CREATE_AGREE);
				if (iFlag)
				{
					autoteam_player_create_reject_send(pstEnv, pstOne, pstPlayer);
				}
			}
			else
			{
				pstAutoTeamPre->nSingleNum--;
				if (i != pstAutoTeamPre->nSingleNum)
				{
					memmove(&pstAutoTeamPre->astSingle[i], 
						&pstAutoTeamPre->astSingle[i+1],
						sizeof(pstAutoTeamPre->astSingle[0])*pstAutoTeamPre->nSingleNum );
				}
			}
		}
	}

	autoteam_player_cancel_send(pstEnv, pstPlayer, 0);	
	
	autoteam_single_del(pstEnv, pstPlayer->iMemID);
	memset(&pstPlayer->stOnline.stAutoTeamInfo, 0, sizeof(pstPlayer->stOnline.stAutoTeamInfo));

	if (0 == pstAutoTeamPre->nSingleNum)
	{
		autoteam_pre_del(&pstAutoTeam->astAutoTeams[iAutoTeamID],
							pstAutoTeamPre);
	}
	else
	{
		autoteam_pre_loose_limit(pstEnv, &pstAutoTeam->astAutoTeams[iAutoTeamID], pstAutoTeamPre, iAutoTeamID);
	}

	if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode &&
		!(pstPlayer->stOnline.eStatusAssistant&PLAYER_STATUS_BATTLE_TEAM_SYNC) )
	{
		pstPlayer->stOnline.eStatusAssistant = 0;
		player_do_statusassitant_sync(pstEnv,pstPlayer);
	}
	
	return 0;
}

AutoTeamCreated *autoteam_find_created_by_teamid(AutoTeamOne *pstAutoTeamOne, int iTeamID)
{
	int i;

	for (i=0; i<pstAutoTeamOne->iCreatedTeamNum; i++)
	{
		if (pstAutoTeamOne->astAutoTeamCreated[i].iTeamID == iTeamID)
		{
			return &pstAutoTeamOne->astAutoTeamCreated[i];
		}
	}
	
	return NULL;
}

	
int autoteam_team_reject(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneTeam *pstZoneTeam, int iCancelFlag)
{
	int i;
	Player *pstOne;
	AutoTeamCreated *pstAutoTeamCreated;
	AutoTeam *pstAutoTeam = &pstEnv->pstZoneObj->stAutoTeam;
	AutoTeamInfo *pstAutoTeamInfo = &pstZoneTeam->stAutoTeamInfo;
	char szMsg[128];
	AutoTeamCreated stTmp;
	AutoTeamInfo stAutoTeamInfoTmp;

	szMsg[0] = 0;
	switch (iCancelFlag)
	{
		case 0:
			strcpy(szMsg, "自动组队取消了");
			break;
		case 1:
			strcpy(szMsg, "队伍成员变化，自动组队取消");
			break;
		case 2:
			if (pstPlayer)
			{
				snprintf(szMsg, sizeof(szMsg), "%s提前下副本了，自动组队取消", pstPlayer->stRoleData.RoleName);
			}
			break;
		case 3:
			strcpy(szMsg, "队伍成员变化，自动组队取消");
			break;
		default:
			break;
	}

	if (0 > pstAutoTeamInfo->AutoTeamID)
	{
		for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
		{
			pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
			if (pstOne)
			{
				if (2 == iCancelFlag) //下副本导致
				{
					autoteam_player_cancel_send(pstEnv, pstOne, 0);
					z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstOne, SYS_MIDDLE2, szMsg);
				}
				else
				{
					if (pstOne != pstPlayer)
					{
						if(pstAutoTeamInfo->TransAutoTeamID <= 0)
						{
							autoteam_player_cancel_send(pstEnv, pstOne, 0);
							if( (iCancelFlag != 3) || ((i+1) != pstZoneTeam->stTeamInfo.MemberNum) )
							{
								z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstOne, SYS_MIDDLE2, szMsg);
							}
						}
					}
					else
					{
						if (iCancelFlag)
						{
							autoteam_player_cancel_send(pstEnv, pstOne, 1);
							z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstOne, SYS_MIDDLE2, szMsg);
						}
						else
						{
							autoteam_player_cancel_send(pstEnv, pstOne, 0);
						}
					}
				}
				if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode &&
					!(pstZoneTeam->eStatusAssistant&PLAYER_STATUS_BATTLE_TEAM_SYNC) )
				{
					pstOne->stOnline.eStatusAssistant = 0;
					player_do_statusassitant_sync(pstEnv,pstOne);
				}
			}
		}

		autoteam_team_del(pstEnv, pstZoneTeam, -1);
		//memset(&pstZoneTeam->stAutoTeamInfo, 0, sizeof(pstZoneTeam->stAutoTeamInfo));
		stAutoTeamInfoTmp = pstZoneTeam->stAutoTeamInfo;
		memset(&pstZoneTeam->stAutoTeamInfo, 0, sizeof(pstZoneTeam->stAutoTeamInfo));
		pstAutoTeamInfo->TransAutoTeamID = stAutoTeamInfoTmp.TransAutoTeamID;
		pstAutoTeamInfo->iPworldGrade = stAutoTeamInfoTmp.iPworldGrade;
		
		return 0;
	}
	
	pstAutoTeamCreated = autoteam_find_created_by_teamid(&pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID], pstZoneTeam->iTeamIdx);
	if (NULL == pstAutoTeamCreated)
	{
		return -1;
	}

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (pstOne)
		{
			if (2 == iCancelFlag) //下副本导致
			{
				autoteam_player_cancel_send(pstEnv, pstOne, 0);
				z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstOne, SYS_MIDDLE2, szMsg);
			}
			else
			{
				if (pstOne != pstPlayer)
				{
					if(pstAutoTeamInfo->TransAutoTeamID <= 0)
					{
						autoteam_player_cancel_send(pstEnv, pstOne, 0);
						if( (iCancelFlag != 3) || ((i+1) != pstZoneTeam->stTeamInfo.MemberNum) )
						{
							z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstOne, SYS_MIDDLE2, szMsg);
						}
					}
				}
				else 
				{
					if (iCancelFlag )
					{
						autoteam_player_cancel_send(pstEnv, pstOne, 1);
						z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstOne, SYS_MIDDLE2, szMsg);
					}
					else
					{
						autoteam_player_cancel_send(pstEnv, pstOne, 0);
					}
				}
			}
			if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode &&
				!(pstZoneTeam->eStatusAssistant&PLAYER_STATUS_BATTLE_TEAM_SYNC) )
			{
				pstOne->stOnline.eStatusAssistant = 0;
				player_do_statusassitant_sync(pstEnv,pstOne);
			}
		}
	}

	memcpy(&stTmp, pstAutoTeamCreated, sizeof(stTmp));

	autoteam_team_del(pstEnv, pstZoneTeam, -1);
	//memset(&pstZoneTeam->stAutoTeamInfo, 0, sizeof(pstZoneTeam->stAutoTeamInfo));
	stAutoTeamInfoTmp = pstZoneTeam->stAutoTeamInfo;
	memset(&pstZoneTeam->stAutoTeamInfo, 0, sizeof(pstZoneTeam->stAutoTeamInfo));
	pstAutoTeamInfo->TransAutoTeamID = stAutoTeamInfoTmp.TransAutoTeamID;
	pstAutoTeamInfo->iPworldGrade = stAutoTeamInfoTmp.iPworldGrade;

	for (i=0; i<stTmp.nSingleNum; i++)
	{
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, stTmp.astSingle[i].iPlayerID);
		if (pstOne)
		{
			AUTOTEAMAPPLYINFO stApply;

			memcpy(&stApply, &pstOne->stOnline.stAutoTeamInfo.stApply, sizeof(stApply));
			autoteam_apply(pstEnv, pstOne, &stApply);
		}
	}
	
	return 0;
}
int autoteam_reject_send_to_battle(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSAUTOTEAMCLT *pstClt = &stPkg.Body.AutoTeamClt;

	if( INST_MODE_ZONE_SVR == pstEnv->iInstMode &&
		pstEnv->pstConf->BattleFlag)
	{
	
		pstClt->AutoTeamCltOp = AUTOTEAM_CLT_OP_UI;
		pstClt->AutoTeamCltData.UIOp.UIOp = AUTOTEAM_UI_REJECT;
		Z_CSHEAD_INIT(&stPkg.Head, AUTOTEAM_CLT);
	

		zone_proxy_request(pstEnv, pstPlayer, &stPkg);
	}

	return 0;
}
int autoteam_reject(ZONESVRENV* pstEnv, Player *pstPlayer,int iContinueFlag)
{
	AutoTeamSingle *pstSingle;
	AutoTeamCreated *pstAutoTeamCreated;
	AutoTeamPre *pstAutoTeamPre;
	AutoTeamInfo *pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;
	AutoTeam *pstAutoTeam = &pstEnv->pstZoneObj->stAutoTeam;
	ZoneTeam *pstZoneTeam;


	autoteam_reject_send_to_battle(pstEnv, pstPlayer);
	
	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if (pstZoneTeam)
	{
		if (pstZoneTeam->stAutoTeamInfo.tStart && 
			(pstPlayer == player_team_get_captain_player(pstEnv, pstZoneTeam)))
		{
			autoteam_team_reject(pstEnv, pstPlayer, pstZoneTeam, 0);
			return 0;
		}
		else
		{
			return 0;
		}
	}
		
	switch(pstAutoTeamInfo->Stat)
	{
		case 1:
			pstAutoTeamCreated = autoteam_find_created_by_playerid(&pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID], 
																	pstPlayer->iMemID, &pstSingle);
			if (pstAutoTeamCreated)
			{
				return autoteam_created_reject(pstEnv, pstPlayer, pstAutoTeamCreated, pstAutoTeamInfo->AutoTeamID);
			}
			break;
		case 2:
			pstAutoTeamPre = autoteam_find_pre_by_playerid(&pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID], 
																	pstPlayer->iMemID, &pstSingle);
			if (pstAutoTeamPre)
			{
				return autoteam_pre_reject(pstEnv, pstPlayer, pstAutoTeamPre, pstAutoTeamInfo->AutoTeamID);
			}
			break;
		default:
			break;
	}

	//给自己转发一下
	autoteam_player_cancel_send(pstEnv, pstPlayer, 0);		
	autoteam_single_del(pstEnv, pstPlayer->iMemID);
	memset(&pstPlayer->stOnline.stAutoTeamInfo, 0, sizeof(pstPlayer->stOnline.stAutoTeamInfo));

	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode && 
		!iContinueFlag )
	{
		pstPlayer->stOnline.eStatusAssistant = 0;
		player_do_statusassitant_sync(pstEnv,pstPlayer);
	}
	
	return 0;
}

int autoteam_created_create(ZONESVRENV* pstEnv, Player *pstPlayer, AutoTeamCreated *pstAutoTeamCreated)
{
	//int i;
	//Player *pstOne;
	
	if (pstAutoTeamCreated->OpFlag || 0 == pstAutoTeamCreated->nSingleNum)
	{
		return -1;
	}

	pstAutoTeamCreated->OpFlag = 1;
	pstPlayer->stOnline.stAutoTeamInfo.bOPFlag |= AUTOTEAM_OP_CREATE;

	//转发给散人
	/*for (i=0; i<pstAutoTeamCreated->nSingleNum; i++)
	{
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamCreated->astSingle[i].iPlayerID);

		if (pstOne)
		{
			autoteam_player_create_send(pstEnv, pstOne, pstPlayer);
		}
	}*/

	return 0;
	UNUSED(pstEnv);
}

int autoteam_pre_create(ZONESVRENV* pstEnv, Player *pstPlayer, AutoTeamPre *pstAutoTeamPre)
{
	//int i;
	//Player *pstOne;
	
	if (pstAutoTeamPre->OpFlag || 2 > pstAutoTeamPre->nSingleNum)
	{
		return -1;
	}

	pstAutoTeamPre->OpFlag = 1;
	pstPlayer->stOnline.stAutoTeamInfo.bOPFlag |= AUTOTEAM_OP_CREATE;

	//转发给散人
	/*for (i=0; i<pstAutoTeamPre->nSingleNum; i++)
	{
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[i].iPlayerID);

		if (pstOne && (pstOne != pstPlayer))
		{
			autoteam_player_create_send(pstEnv, pstOne, pstPlayer);
		}
	}*/

	return 0;
	UNUSED(pstEnv);
}
int autoteam_create_check(ZONESVRENV* pstEnv, Player *pstPlayer,int iAutoTeamID)
{
	ZONEPWORLDINST *pstZonePworldInst = NULL;
	int iPworldID = 0;
	ACTIVEHELPER *pstActiveHelp = NULL;
	AutoTeamInfo *pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;
	AutoTeam *pstAutoTeam = &pstEnv->pstZoneObj->stAutoTeam;
	AutoTeamSingle *pstSingle;
	AutoTeamCreated *pstAutoTeamCreated;
	AutoTeamPre *pstAutoTeamPre;
	
	pstActiveHelp = find_active_by_autoteam(pstEnv, iAutoTeamID);
	if (NULL == pstActiveHelp)
	{
		return -1;
	}
	
	pstZonePworldInst = z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);
	if (!pstZonePworldInst )
	{
		return 0;
	}	
	
	switch(pstAutoTeamInfo->Stat)
	{
		case 0:
			break;
		case 1:
			{
				pstAutoTeamCreated = autoteam_find_created_by_playerid(&pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID], 
																	pstPlayer->iMemID, &pstSingle);
				if (pstAutoTeamCreated)
				{
					iPworldID = autoteam_diffpworld_grade_id_get(pstEnv, pstActiveHelp->TaskPworldID, pstAutoTeamCreated->nLimitFlag);
					if (pstZonePworldInst->stPworldIdx.iID != iPworldID )
					{
						z_sys_strf(pstEnv,  pstPlayer, SYS_MIDDLE2, "请退出副本再操作,自动组队失败!");
						return -1;
					}
				}
			}
			break;
		case 2:
			{
				

				pstAutoTeamPre = autoteam_find_pre_by_playerid(&pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID], 
															pstPlayer->iMemID, &pstSingle);
				if (pstAutoTeamPre)
				{
					iPworldID = autoteam_diffpworld_grade_id_get(pstEnv, pstActiveHelp->TaskPworldID, pstSingle->nLimitFlag);
					if (pstZonePworldInst->stPworldIdx.iID != iPworldID )
					{
						z_sys_strf(pstEnv,  pstPlayer, SYS_MIDDLE2, "请退出副本再操作,自动组队失败!");
						return -1;
					}
				}
			}
			break;
		default:
			break;
	}
	return 0;
}
int autoteam_create(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i = 0;
	ZoneTeam *pstZoneTeam;
	AutoTeamCreated *pstAutoTeamCreated;
	AutoTeamInfo *pstAutoTeamInfo;
	AutoTeamSingle *pstSingle;
	AutoTeamPre *pstAutoTeamPre;
	AutoTeam *pstAutoTeam = &pstEnv->pstZoneObj->stAutoTeam;

	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if (pstZoneTeam)
	{
		if (pstPlayer != player_team_get_captain_player(pstEnv, pstZoneTeam))
		{
			return -1;
		}

		pstAutoTeamInfo = &pstZoneTeam->stAutoTeamInfo;
		if (0 > pstAutoTeamInfo->AutoTeamID)
		{
			//自己就可以进了
			autoteam_team_apply_by_fullmember( pstEnv,pstZoneTeam,&pstAutoTeamInfo->stApply);
			return -1;
		}

		pstAutoTeamCreated = autoteam_find_created_by_teamid(&pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID], 
														pstZoneTeam->iTeamIdx);
		if (NULL == pstAutoTeamCreated)
		{
			return -1;
		}
		if(0 > autoteam_create_check(pstEnv, pstPlayer,pstAutoTeamInfo->AutoTeamID))
		{
			return -1;
		}
		if(0 == pstAutoTeamCreated->nSingleNum)
		{
			//自己就可以进了
			autoteam_team_apply_by_fullmember( pstEnv,pstZoneTeam,&pstAutoTeamInfo->stApply);
			return -1;
		}
		
		autoteam_created_create(pstEnv, pstPlayer, pstAutoTeamCreated);

		//其他人同意	
		for (i=0; i<pstAutoTeamCreated->nSingleNum; i++)
		{
			Player *pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamCreated->astSingle[i].iPlayerID);
			if (pstOne && pstOne!=pstPlayer)
			{
				autoteam_create_agree(pstEnv, pstOne);
			}
		}
		return 0;
	}
	
	pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;

	if (AUTOTEAM_OP_CREATE & pstAutoTeamInfo->bOPFlag)
	{
		return -1;
	}

	if (0 > pstAutoTeamInfo->AutoTeamID)
	{
		return -1;
	}
	
	switch(pstAutoTeamInfo->Stat)
	{
		case 2:
			pstAutoTeamPre = autoteam_find_pre_by_playerid(&pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID], 
															pstPlayer->iMemID, &pstSingle);

			if (NULL == pstAutoTeamPre)
			{
				return -1;
			}
			
			if (pstAutoTeamPre->astSingle[0].iPlayerID != pstPlayer->iMemID)
			{
				return -1;
			}
 
			if(0 > autoteam_create_check(pstEnv, pstPlayer,pstAutoTeamInfo->AutoTeamID))
			{
				return -1;
			}

			autoteam_pre_create(pstEnv, pstPlayer, pstAutoTeamPre);

			//其他人同意		
			for (i=0; i<pstAutoTeamPre->nSingleNum; i++)
			{
				Player *pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[i].iPlayerID);
				if (pstOne && pstOne!=pstPlayer)
				{
					autoteam_create_agree(pstEnv, pstOne);
				}
			}
			
			break;
		default:
			return -1;
			break;
	}
	
	return 0;
}

int autoteam_created_create_agree(ZONESVRENV* pstEnv, Player *pstPlayer, 
										AutoTeamCreated *pstAutoTeamCreated, int iAutoTeamID)
{
	int i;
	Player *pstOne;
	ZoneTeam *pstZoneTeam;
	TMEMBLOCK *pstMemBlock;
	AutoTeamInfo *pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;
	int iFlag = 0;
	
	if (0 == pstAutoTeamCreated->OpFlag)
	{
		return -1;
	}

	pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstTeamPool, pstAutoTeamCreated->iTeamID);
	if (0 == pstMemBlock->fValid)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "zone team null");
		return -1;
	}

	pstZoneTeam = (ZoneTeam *)pstMemBlock->szData;

	pstAutoTeamInfo->bOPFlag |= AUTOTEAM_OP_CREATE_AGREE;

	for (i=0; i<pstAutoTeamCreated->nSingleNum; i++)
	{
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamCreated->astSingle[i].iPlayerID);
		if (pstOne)
		{
			if (0 == (AUTOTEAM_OP_CREATE_AGREE & pstOne->stOnline.stAutoTeamInfo.bOPFlag) &&
				0 == (AUTOTEAM_OP_CREATE & pstOne->stOnline.stAutoTeamInfo.bOPFlag))
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	if (i == pstAutoTeamCreated->nSingleNum)
	{
		iFlag = 1;
	}

	if (iFlag)
	{
		autoteam_created_succ(pstEnv, pstZoneTeam, pstAutoTeamCreated, iAutoTeamID);
	}
		
	return 0;
}

int autoteam_created_create_reject(ZONESVRENV* pstEnv, Player *pstPlayer, 
										AutoTeamCreated *pstAutoTeamCreated, int iAutoTeamID)
{
	int i;
	Player *pstOne;
	ZoneTeam *pstZoneTeam;
	TMEMBLOCK *pstMemBlock;
	AutoTeamInfo *pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;
	UNUSED(&iAutoTeamID);
		
	if (0 == pstAutoTeamCreated->OpFlag)
	{
		return -1;
	}

	pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstTeamPool, pstAutoTeamCreated->iTeamID);
	if (0 == pstMemBlock->fValid)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "zone team null");
		return -1;
	}

	pstZoneTeam = (ZoneTeam *)pstMemBlock->szData;

	pstAutoTeamInfo->bOPFlag &= ~AUTOTEAM_OP_CREATE_AGREE;
	pstAutoTeamInfo->bOPFlag &= ~AUTOTEAM_OP_CREATE;
	pstAutoTeamCreated->OpFlag = 0;

	for (i=0; i<pstAutoTeamCreated->nSingleNum; i++)
	{
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamCreated->astSingle[i].iPlayerID);
		if (pstOne && (pstOne != pstPlayer))
		{
			pstOne->stOnline.stAutoTeamInfo.bOPFlag &= ~AUTOTEAM_OP_CREATE_AGREE;
			pstOne->stOnline.stAutoTeamInfo.bOPFlag &= ~AUTOTEAM_OP_CREATE;
				
			autoteam_player_create_reject_send(pstEnv, pstOne, pstPlayer);
		}
	}

	pstOne = player_team_get_captain_player(pstEnv, pstZoneTeam);
	if (pstOne)
	{
		pstOne->stOnline.stAutoTeamInfo.bOPFlag &= ~AUTOTEAM_OP_CREATE_AGREE;
		pstOne->stOnline.stAutoTeamInfo.bOPFlag &= ~AUTOTEAM_OP_CREATE;

		autoteam_player_create_reject_send(pstEnv, pstOne, pstPlayer);
	}

	return 0;
}

int autoteam_pre_create_reject(ZONESVRENV* pstEnv, Player *pstPlayer, 
										AutoTeamPre *pstAutoTeamPre, int iAutoTeamID)
{
	int i;
	Player *pstOne;
	AutoTeamInfo *pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;
	UNUSED(&iAutoTeamID);
		
	/*if (0 == pstAutoTeamPre->OpFlag)
	{
		return -1;
	}*/

	pstAutoTeamInfo->bOPFlag &= ~AUTOTEAM_OP_CREATE_AGREE;
	pstAutoTeamInfo->bOPFlag &= ~AUTOTEAM_OP_CREATE;
	
	pstAutoTeamPre->OpFlag = 0;

	for (i=0; i<pstAutoTeamPre->nSingleNum; i++)
	{
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[i].iPlayerID);
		if (pstOne && (pstOne != pstPlayer) )
		{
			pstOne->stOnline.stAutoTeamInfo.bOPFlag &= ~AUTOTEAM_OP_CREATE_AGREE;
			pstOne->stOnline.stAutoTeamInfo.bOPFlag &= ~AUTOTEAM_OP_CREATE;

			autoteam_player_create_reject_send(pstEnv, pstOne, pstPlayer);
		}
	}

	return 0;
}
	

int autoteam_pre_create_agree(ZONESVRENV* pstEnv, Player *pstPlayer, AutoTeamPre *pstAutoTeamPre,
									int iAutoTeamID)
{
	int i;
	Player *pstOne;
	int iFlag = 0;
	AutoTeamInfo *pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;

	if (0 == pstAutoTeamPre->OpFlag)
	{
		return -1;
	}

	pstAutoTeamInfo->bOPFlag |= AUTOTEAM_OP_CREATE_AGREE;

	for (i=0; i<pstAutoTeamPre->nSingleNum; i++)
	{
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[i].iPlayerID);
		if (pstOne)
		{
			if (0 == (AUTOTEAM_OP_CREATE_AGREE & pstOne->stOnline.stAutoTeamInfo.bOPFlag) &&
				0 == (AUTOTEAM_OP_CREATE & pstOne->stOnline.stAutoTeamInfo.bOPFlag))
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	if (i == pstAutoTeamPre->nSingleNum)
	{
		iFlag = 1;
	}

	if (iFlag)
	{
		autoteam_pre_leader(pstEnv, pstAutoTeamPre, 1);
		autoteam_pre_succ(pstEnv, pstAutoTeamPre, iAutoTeamID);
	}

	return 0;
}

int autoteam_create_agree(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	AutoTeamCreated *pstAutoTeamCreated;
	AutoTeamPre *pstAutoTeamPre;
	AutoTeamSingle *pstSingle;
	AutoTeamInfo *pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;
	AutoTeam *pstAutoTeam = &pstEnv->pstZoneObj->stAutoTeam;
	
	if (0 > pstAutoTeamInfo->AutoTeamID)
	{
		return -1;
	}

	if (AUTOTEAM_OP_CREATE_AGREE & pstAutoTeamInfo->bOPFlag)
	{
		return -1;
	}

	switch(pstAutoTeamInfo->Stat)
	{
		case 1:
			pstAutoTeamCreated = autoteam_find_created_by_playerid(&pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID], 
														pstPlayer->iMemID, &pstSingle);

			if (NULL == pstAutoTeamCreated)
			{
				return -1;
			}

			autoteam_created_create_agree(pstEnv, pstPlayer, pstAutoTeamCreated, pstAutoTeamInfo->AutoTeamID);
			break;
		case 2:
			pstAutoTeamPre = autoteam_find_pre_by_playerid(&pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID], 
															pstPlayer->iMemID, &pstSingle);

			if (NULL == pstAutoTeamPre)
			{
				return -1;
			}
			
			autoteam_pre_create_agree(pstEnv, pstPlayer, pstAutoTeamPre, pstAutoTeamInfo->AutoTeamID);
			break;
		default:
			return -1;
			break;
	}
	
	return 0;
}

int autoteam_create_reject(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	AutoTeamCreated *pstAutoTeamCreated;
	AutoTeamPre *pstAutoTeamPre;
	AutoTeamSingle *pstSingle;
	AutoTeamInfo *pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;
	AutoTeam *pstAutoTeam = &pstEnv->pstZoneObj->stAutoTeam;

	if (0 > pstAutoTeamInfo->AutoTeamID)
	{
		return -1;
	}

	switch(pstAutoTeamInfo->Stat)
	{
		case 1:
			pstAutoTeamCreated = autoteam_find_created_by_playerid(&pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID], 
														pstPlayer->iMemID, &pstSingle);

			if (NULL == pstAutoTeamCreated)
			{
				return -1;
			}

			autoteam_created_create_reject(pstEnv, pstPlayer, pstAutoTeamCreated, pstAutoTeamInfo->AutoTeamID);
			break;
		case 2:
			pstAutoTeamPre = autoteam_find_pre_by_playerid(&pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID], 
															pstPlayer->iMemID, &pstSingle);

			if (NULL == pstAutoTeamPre)
			{
				return -1;
			}
			
			autoteam_pre_create_reject(pstEnv, pstPlayer, pstAutoTeamPre, pstAutoTeamInfo->AutoTeamID);
			break;
		default:
			return -1;
			break;
	}
	
	return 0;
}

int autoteam_exit_trans(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZoneTeam *pstZoneTeam;
	
	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if (pstZoneTeam)
	{
		player_team_exit(pstEnv, pstPlayer,pstZoneTeam);
	}

	if (pstPlayer->stOnline.stAutoTeamInfo.tStart)
	{
		autoteam_reject(pstEnv, pstPlayer,0);
	}

	autoteam_notify_clt_clean(pstEnv, pstPlayer);

	if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode)
	{
		pstPlayer->stOnline.eStatusAssistant = 0;
		player_do_statusassitant_sync(pstEnv,pstPlayer);
	}
	return 0;
}
int autoteam_op_ui(ZONESVRENV* pstEnv, Player *pstPlayer, AUTOTEAMCLTUI *pstUI)
{
	AutoTeamInfo *pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;
	ZoneTeam *pstZoneTeam = NULL;

	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if (pstZoneTeam)
	{
		if (0 == pstZoneTeam->stAutoTeamInfo.tStart && 
		     pstUI->UIOp != AUTOTEAM_UI_EXIT_TRANS)
		{
			autoteam_notify_clt_clean(pstEnv, pstPlayer);
			return -1;
		}
	}
	else if (0 == pstAutoTeamInfo->tStart && 
		     pstUI->UIOp != AUTOTEAM_UI_EXIT_TRANS )
	{
		autoteam_notify_clt_clean(pstEnv, pstPlayer);
		return -1;
	}
	
	switch(pstUI->UIOp)
	{
		case AUTOTEAM_UI_AGREE:
			autoteam_agree(pstEnv, pstPlayer);
			break;
		case AUTOTEAM_UI_REJECT:
			autoteam_reject(pstEnv, pstPlayer,0);
			break;
		case AUTOTEAM_UI_CREATE:
			autoteam_create(pstEnv, pstPlayer);
			break;
		case AUTOTEAM_UI_CREATE_AGREE:
			autoteam_create_agree(pstEnv, pstPlayer);
			break;
		case AUTOTEAM_UI_CREATE_REJECT:
			autoteam_create_reject(pstEnv, pstPlayer);
			break;
		case AUTOTEAM_UI_EXIT_TRANS:
			autoteam_exit_trans(pstEnv, pstPlayer);
			break;
		default:
			break;
	}
	
	return 0;
}

int autoteam_check_active_running(ZONESVRENV* pstEnv, ACTIVEHELPER *pstActiveHelper)
{
	int i, iWeekFlag, iLen,iLen2;
	struct tm stCurr;
	int iCurrDate, iCurrTime;
	char sTmp[20];
	int iTimeFlag;
	int iStartTime, iEndTime;

	localtime_r(&pstEnv->pstAppCtx->stCurr.tv_sec, &stCurr);
	strftime( sTmp, sizeof(sTmp), "%Y%m%d", &stCurr);
	iCurrDate = atoi(sTmp);
	
	if (8 == strlen(pstActiveHelper->StartDate) && iCurrDate < atoi(pstActiveHelper->StartDate))
	{
		return -1;
	}

	if (8 == strlen(pstActiveHelper->EndDate) && iCurrDate > atoi(pstActiveHelper->EndDate))
	{
		return -1;
	}

	if (pstActiveHelper->ReCycleType)
	{
		iWeekFlag = 0;
		iLen = strlen(pstActiveHelper->WeekDay);
		for (i=0; i<iLen; i++)
		{
			if (pstActiveHelper->WeekDay[i] == '1')
			{
				iWeekFlag |= (1 << (iLen - 1 - i));
			}
		}

		if( !(iWeekFlag & (1 << (6 - stCurr.tm_wday))) )
		{
			return -1;
		}
	}

	strftime( sTmp, sizeof(sTmp), "%H0%M", &stCurr);
	iCurrTime = atoi(sTmp);

	iTimeFlag = 0;
	for (i=0; i<MAX_ACTIVE_DAY_TIME; i++)
	{
		if (pstActiveHelper->ActiveDayTime[i].StartTime[0] == '-')
		{
			break;
		}

		iStartTime = 0;
		iEndTime = 24000;	
		iLen2 = strlen(pstActiveHelper->ActiveDayTime[i].StartTime);
		if (iLen2 == 4 || iLen2 == 5)
		{
			STRNCPY(sTmp, pstActiveHelper->ActiveDayTime[i].StartTime, sizeof(sTmp));
			if (iLen2 == 4)
			{
				sTmp[1] = '0';
			}
			else
			{
				sTmp[2] = '0';
			}
			iStartTime = atoi(sTmp);
		}
		iLen2 = strlen(pstActiveHelper->ActiveDayTime[i].EndTime);
		if (iLen2 == 4 || iLen2 == 5)
		{
			STRNCPY(sTmp, pstActiveHelper->ActiveDayTime[i].EndTime, sizeof(sTmp));
			if (iLen2 == 4)
			{
				sTmp[1] = '0';
			}
			else
			{
				sTmp[2] = '0';
			}
			iEndTime = atoi(sTmp);
		}
		if (iCurrTime >= iStartTime && iCurrTime < iEndTime)
		{
			iTimeFlag = 1;
			break;
		}
	}

	if (0 == iTimeFlag)
	{
		return -1;
	}
	
	return iStartTime;
}

int autoteam_check_trans(ZONESVRENV* pstEnv, Player *pstPlayer, AUTOTEAMCLTTRANS *pstTrans)
{
	ACTIVEHELPER *pstActiveHelper;
	AutoTeamInfo *pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;

	if (0 > z_player_goto_check(pstEnv, pstPlayer, 0, NULL))
	{
		return -1;
	}
	
	/*if (0 == pstAutoTeamInfo->Stat)
	{
		return -1;
	}*/

	//pstTrans->PworldID 是难度副本ID
	pstActiveHelper = find_active_by_autoteam(pstEnv, pstAutoTeamInfo->AutoTeamID);
	if (NULL == pstActiveHelper || pstActiveHelper->TaskPworldID != pstTrans->PworldID)
	{
		return -1;
	}

	//检查时间过期，提示一下
	if (0 > autoteam_check_active_running(pstEnv, pstActiveHelper))
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, "活动时间已过，不能传送到副本中");
		return -1;
	}


	z_ride_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	
	return 0;
}

int autoteam_op_trans(ZONESVRENV* pstEnv, Player *pstPlayer, int PworldID)
{
	ZONEPWORLD *pstZonePworld;
	PWORLDSELECT stPworldSelect;
	ZONEIDX stIdx;
	ZoneTeam *pstZoneTeam;
	//AutoTeamInfo *pstAutoTeamInfo;
	DIFFPWORLDDEF *pstDiffPworldDef;
	int iGrade;
	ACTIVEHELPER *pstActiveHelp;

	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if (NULL == pstZoneTeam)
	{
		return -1;
	}

	pstActiveHelp = find_active_by_autoteam(pstEnv, pstZoneTeam->stAutoTeamInfo.TransAutoTeamID);
	if (NULL == pstActiveHelp)
	{
		return -1;
	}
	//pstDiffPworldDef = z_find_diff_pworld_def(pstEnv, PworldID);
	pstDiffPworldDef = z_find_diff_pworld_def(pstEnv,pstActiveHelp->TaskPworldID);
	if(NULL == pstDiffPworldDef)
	{
		return -1;
	}

	//iGrade = pstZoneTeam->stAutoTeamInfo.bOPFlag;
	iGrade = pstZoneTeam->stAutoTeamInfo.iPworldGrade;
	if(iGrade < 0 || iGrade >= MAX_DIFF_PWORLD_LIST)
	{
		return -1;
	}

	stIdx.iID = pstDiffPworldDef->PworldInfo[iGrade].PworldID;
	stIdx.iPos = -1;
	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (NULL == pstZonePworld)
	{
		return -1;
	}

	
	/*
	pstAutoTeamInfo = &pstZoneTeam->stAutoTeamInfo;
	if (0 != pstAutoTeamInfo->tStart)
	{
		return -1;
	}

	if(pstAutoTeamInfo->AutoTeamID <=0)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, "传送失败");
		return -1;
	}*/

	memset(&stPworldSelect, 0, sizeof(stPworldSelect));

	pstPlayer->stOnline.stAutoTeamInfo.Stat = 0;
	if (z_player_enter_pworld(pstEnv, pstPlayer, pstZonePworld->stPworldDef.PworldID, &stPworldSelect, 1) < 0)
	{
		return -1;
	}
	//player_team_follow_enter_pworld(pstEnv, pstPlayer, pstZonePworld->stPworldDef.PworldID,&stPworldSelect);

	return 0;
	UNUSED(&PworldID);
}

int player_auto_team_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSAUTOTEAMCLT *pstClt = &pstCsPkg->Body.AutoTeamClt;
	SSBATTLECAMPINFO *pstBattleCampInfo = &pstPlayer->stOnline.stPworldQueue.stBattleCampInfo;
	ZoneTeam *pstZoneTeam;
	int i = 0;
	Player *pstOne = NULL;

	if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode && 
		!is_battle_pve_autoteam_svr(pstEnv))
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "当前处于跨服状态，不允许该操作");
		return -1;
	}

 	if( pstEnv->pstConf->BattleFlag && 
		INST_MODE_ZONE_SVR == pstEnv->iInstMode && 
		pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, 
					"跨服自动组队已经开启,请离开副本再操作!");
		return 0;
	}

	//自动组组队有操作就把跨服给退了
	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if(pstZoneTeam)
	{
		for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
		{
			pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
			if(pstOne)
			{
				z_exit_pworld_queue(pstEnv, pstOne, pstOne->stOnline.stPworldQueue.iPworldID, 1);
			}
			
		}
	}
	
	
	switch(pstClt->AutoTeamCltOp)
	{
		case AUTOTEAM_CLT_OP_APPLY:
			autoteam_apply(pstEnv, pstPlayer, &pstClt->AutoTeamCltData.Apply);
			break;
		case AUTOTEAM_CLT_OP_UI:
			if( INST_MODE_ZONE_SVR == pstEnv->iInstMode &&
				pstEnv->pstConf->BattleFlag)
			{
				if(pstPlayer->stOnline.stPworldQueue.stBattleCampInfo.BattleBusID>0)
				{
					zone_proxy_request(pstEnv, pstPlayer, pstCsPkg);
				}
				else if(pstClt->AutoTeamCltData.UIOp.UIOp == AUTOTEAM_UI_REJECT)
				{
					autoteam_notify_clt_clean(pstEnv, pstPlayer);
				}
			}
			else
			{
				autoteam_op_ui(pstEnv, pstPlayer, &pstClt->AutoTeamCltData.UIOp);
			}
			break;
		case AUTOTEAM_CLT_OP_TRANS:		
			if( INST_MODE_ZONE_SVR == pstEnv->iInstMode &&
				pstBattleCampInfo->BattleBusID == pstEnv->iBattlePveAutoteamID &&
				pstEnv->pstConf->BattleFlag )
			{
				if (pstPlayer->stOnline.State & CS_STAT_FIGHT)
				{
					z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "战斗状态下不能使用该功能");
					return -1;
				}
				
				pstBattleCampInfo->PworldDefID = pstClt->AutoTeamCltData.Trans.PworldID;
				player_battle_enter_req(pstAppCtx,pstEnv,pstFrameHead,
							pstPlayer,PLAYER_STATUS_BATTLE_LOGIN_ON,pstBattleCampInfo->BattleBusID);
			}
			else
			{
				if (pstPlayer->stOnline.State & CS_STAT_FIGHT)
				{
					z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "战斗状态下不能使用该功能");
					return -1;
				}
				if (0 > autoteam_check_trans(pstEnv, pstPlayer, &pstClt->AutoTeamCltData.Trans))
				{
					//z_send_sys_str(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, "自动组队传送失败,请重新自动组队");
					//autoteam_notify_clt_clean(pstEnv, pstPlayer);
					return -1;
				}
				if(0 != autoteam_op_trans(pstEnv, pstPlayer, pstClt->AutoTeamCltData.Trans.PworldID))
				{
					z_send_sys_str(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, "自动组队传送失败,请重新自动组队");
					autoteam_notify_clt_clean(pstEnv, pstPlayer);
				}
			}
			break;
		default:
			return -1;
			break;
	}
	
	return 0;
}

int autoteam_apply_timeout(AUTOTEAMAPPLYINFO *pstApply)
{
	int i;
	
	pstApply->LimitFlag = 0;
	for (i=0; i<pstApply->AutoTeamIDNum; i++)
	{
		//超时也不全选，始终组建已经选择的难度副本
		//pstApply->AutoTeamIDInfos[i].LimitFlag = AUTO_TEAM_LIMIT_EASY|AUTO_TEAM_LIMIT_NORM|AUTO_TEAM_LIMIT_DIFF;
	}

	return 0;
}

int autoteam_create_addpre_pre(ZONESVRENV *pstEnv, AutoTeamOne *pstAutoTeamOne, 
					AutoTeamPre *pstAutoTeamPre, AutoTeamPreIdx *pstPreIdx,int AutoTeamID)
{
	int i, j;
	AutoTeamCreated *pstAutoTeamCreatedOri;
	AutoTeamCreated *pstAutoTeamCreated = &pstPreIdx->stTeamCreated;
	ZoneTeam *pstZoneTeam;
	TMEMBLOCK *pstMemBlock;
	Player *pstOne;
	AutoTeamSingle *pstSingle;
	int iUnValidTeam = -1;
	
	memset(pstPreIdx, 0, sizeof(*pstPreIdx));

	for (i=0; i<pstAutoTeamOne->iCreatedTeamNum; i++)
	{
		pstAutoTeamCreatedOri = &pstAutoTeamOne->astAutoTeamCreated[i];
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstTeamPool, pstAutoTeamCreatedOri->iTeamID);
		if (0 == pstMemBlock->fValid)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "zone team null");
			iUnValidTeam = pstAutoTeamCreatedOri->iTeamID;
			continue;
		}
		
		pstZoneTeam = (ZoneTeam *)pstMemBlock->szData;
		
		if (pstAutoTeamCreatedOri->OpFlag || 
			pstAutoTeamCreatedOri->nSingleNum+pstZoneTeam->stTeamInfo.MemberNum+pstAutoTeamCreatedOri->nSingleNum > MAX_TEAM_MEMBER)
		{
			continue;
		}

		memcpy(pstAutoTeamCreated, pstAutoTeamCreatedOri, sizeof(*pstAutoTeamCreated));

		for (j=0; j<pstAutoTeamPre->nSingleNum; j++)
		{
			pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[j].iPlayerID);	
			if (NULL == pstOne)
			{
				break;
			}

			if (0 == autoteam_created_limit_co(pstEnv, pstOne, pstAutoTeamPre->astSingle[j].nLimitFlag, pstZoneTeam, pstAutoTeamCreated,AutoTeamID))
			{
				break;
			}

			pstSingle = &pstAutoTeamCreated->astSingle[pstAutoTeamCreated->nSingleNum++];
			memcpy(pstSingle, &pstAutoTeamPre->astSingle[j], sizeof(*pstSingle));
		}

		if (j == pstAutoTeamPre->nSingleNum)
		{
			if (pstAutoTeamCreated->nSingleNum + pstZoneTeam->stTeamInfo.MemberNum > pstPreIdx->iNum)
			{
				pstPreIdx->iNum = pstAutoTeamCreated->nSingleNum + pstZoneTeam->stTeamInfo.MemberNum;
				pstPreIdx->tTime = pstZoneTeam->stAutoTeamInfo.tStart;
				pstPreIdx->iIdx = i;
			}
			else if (pstAutoTeamPre->nSingleNum == pstPreIdx->iNum)
			{
				if (pstZoneTeam->stAutoTeamInfo.tStart < pstPreIdx->tTime)
				{
					pstPreIdx->iNum = pstAutoTeamCreated->nSingleNum + pstZoneTeam->stTeamInfo.MemberNum;
					pstPreIdx->tTime = pstZoneTeam->stAutoTeamInfo.tStart;
					pstPreIdx->iIdx = i;
				}
			}
		}

	}

	if (iUnValidTeam != -1)
	{
		autoteam_team_del_id(pstAutoTeamOne, iUnValidTeam);
	}
	
	return 0;
}

int autoteam_pre_addpre_pre(ZONESVRENV *pstEnv, AutoTeamOne *pstAutoTeamOne, AutoTeamPre *pstAutoTeamPreOri, 
					AutoTeamPreIdx *pstPreIdx,int AutoTeamID)
{
	int i, j;
	Player *pstOne;
	AutoTeamSingle *pstSingle;
	AutoTeamPre *pstAutoTeamPre;
	AutoTeamPre *pstAutoTeamPreTmp = &pstPreIdx->stTeamPre;
	
	memset(pstPreIdx, 0, sizeof(*pstPreIdx));

	for (i=0; i<pstAutoTeamOne->iPreTeamNum; i++)
	{
		pstAutoTeamPre = &pstAutoTeamOne->astAutoTeamPre[i];
		if (pstAutoTeamPre == pstAutoTeamPreOri)
		{
			continue;
		}
		
		if (pstAutoTeamPre->OpFlag || 
			pstAutoTeamPre->nSingleNum+pstAutoTeamPreOri->nSingleNum > MAX_TEAM_MEMBER)
		{
			continue;
		}

		memcpy(pstAutoTeamPreTmp, pstAutoTeamPreOri, sizeof(*pstAutoTeamPreTmp));

		for (j=0; j<pstAutoTeamPre->nSingleNum; j++)
		{
			pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[j].iPlayerID);	
			if (NULL == pstOne)
			{
				break;
			}

			if (0 == autoteam_pre_limit_co(pstEnv, pstOne, pstAutoTeamPre->astSingle[j].nLimitFlag, pstAutoTeamPreTmp,AutoTeamID))
			{
				break;
			}

			pstSingle = &pstAutoTeamPreTmp->astSingle[pstAutoTeamPreTmp->nSingleNum++];
			memcpy(pstSingle, &pstAutoTeamPre->astSingle[j], sizeof(*pstSingle));
		}

		if (j == pstAutoTeamPre->nSingleNum)
		{
			if (pstAutoTeamPre->nSingleNum > pstPreIdx->iNum)
			{
				pstPreIdx->iNum = pstAutoTeamPre->nSingleNum;
				pstPreIdx->tTime = pstAutoTeamPre->tTime;
				pstPreIdx->iIdx = i;
			}
			else if (pstAutoTeamPre->nSingleNum == pstPreIdx->iNum)
			{
				if (pstAutoTeamPre->tTime < pstPreIdx->tTime)
				{
					pstPreIdx->iNum = pstAutoTeamPre->nSingleNum;
					pstPreIdx->tTime = pstAutoTeamPre->tTime;
					pstPreIdx->iIdx = i;
				}
			}
		}

	}
	
	return 0;
}


int autoteam_created_addpre(ZONESVRENV *pstEnv, AutoTeamOne *pstAutoTeamOne, AutoTeamPre *pstAutoTeamPre, 
								AutoTeamPreIdx *pstPreIdx, int iAutoTeamID)
{
	int i;
	Player *pstOne;
	ZoneTeam *pstZoneTeam;
	TMEMBLOCK *pstMemBlock;
	AutoTeamCreated *pstAutoTeamCreated= &pstAutoTeamOne->astAutoTeamCreated[pstPreIdx->iIdx];

	memcpy(pstAutoTeamCreated, &pstPreIdx->stTeamCreated, sizeof(*pstAutoTeamCreated));
	pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstTeamPool, pstAutoTeamCreated->iTeamID);
	if (0 == pstMemBlock->fValid)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "zone team null");
		return -1;
	}
	
	pstZoneTeam = (ZoneTeam *)pstMemBlock->szData;

	for (i=0; i<pstAutoTeamCreated->nSingleNum; i++)
	{
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamCreated->astSingle[i].iPlayerID);

		if (pstOne)
		{
			pstOne->stOnline.stAutoTeamInfo.Stat = 1;
			pstOne->stOnline.stAutoTeamInfo.AutoTeamID = iAutoTeamID;
			autoteam_created_mem_info(pstEnv, pstOne, pstZoneTeam, pstAutoTeamCreated);
		}
	}

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);

		if (pstOne)
		{
			autoteam_created_mem_info(pstEnv, pstOne, pstZoneTeam, pstAutoTeamCreated);
		}
	}

	autoteam_pre_del(pstAutoTeamOne, pstAutoTeamPre);
	
	return 0;
}

int autoteam_pre_addpre(ZONESVRENV *pstEnv, AutoTeamOne *pstAutoTeamOne, AutoTeamPre *pstAutoTeamPreOri, 
								AutoTeamPreIdx *pstPreIdx, int iAutoTeamID)
{
	int i;
	Player *pstOne;
	AutoTeamPre *pstAutoTeamPre = &pstPreIdx->stTeamPre;
	UNUSED(&iAutoTeamID);

	memcpy(pstAutoTeamPreOri, pstAutoTeamPre, sizeof(*pstAutoTeamPreOri));

	for (i=0; i<pstAutoTeamPre->nSingleNum; i++)
	{
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[i].iPlayerID);
		if (pstOne)
		{
			autoteam_pre_mem_info(pstEnv, pstOne, pstAutoTeamPre);
		}
	}

	autoteam_pre_del(pstAutoTeamOne, &pstAutoTeamOne->astAutoTeamPre[pstPreIdx->iIdx]);
		
	return 0;
}

int autoteam_pre_loose_limit(ZONESVRENV *pstEnv, AutoTeamOne *pstAutoTeamOne, AutoTeamPre *pstAutoTeamPre, int iAutoTeamID)
{
	int i,j;
	AutoTeamPreIdx stPreIdx;
	AutoTeamSingle *pstSingle;
	Player *pstOne, *pstPrePlayer;
	CSPKG stPkg;
	CSAUTOTEAMSVR *pstAutoTeamSvr = &stPkg.Body.AutoTeamSvr;
	CSAUTOTEAMPREADD *pstAdd = &pstAutoTeamSvr->AutoTeamSvrData.PreAdd;
	AutoTeamInfo *pstAutoTeamInfo ;

	if (pstAutoTeamPre->nSingleNum >= MAX_TEAM_MEMBER || pstAutoTeamPre->OpFlag)
	{
		return 0;
	}
	
	//轮一遍created队伍，人多者优先
	autoteam_create_addpre_pre(pstEnv, pstAutoTeamOne,pstAutoTeamPre, &stPreIdx,iAutoTeamID);
	if (stPreIdx.iNum > 0)
	{
		return autoteam_created_addpre(pstEnv, pstAutoTeamOne, pstAutoTeamPre, &stPreIdx, iAutoTeamID);
	}

	//轮一遍pre队伍，人多者优先
	autoteam_pre_addpre_pre(pstEnv, pstAutoTeamOne,pstAutoTeamPre, &stPreIdx,iAutoTeamID);
	if (stPreIdx.iNum > 0)
	{
		return autoteam_pre_addpre(pstEnv, pstAutoTeamOne, pstAutoTeamPre, &stPreIdx, iAutoTeamID);
	}
	
	pstAutoTeamSvr->AutoTeamSvrOp = AUTOTEAM_SVR_OP_PRE_ADD;
	pstAdd->Num = 1;
	pstAdd->AutoTeamID = iAutoTeamID;
	pstAdd->CreatedTeam = 0;
	pstAdd->Replace = 0;
	Z_CSHEAD_INIT(&stPkg.Head, AUTOTEAM_SVR);

	for (i=0; i<pstAutoTeamOne->iSingleNum; i++)
	{
		pstSingle = &pstAutoTeamOne->astAutoTeamSingle[i];
		pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstSingle->iPlayerID);
		if (NULL == pstOne)
		{
			continue;
		}
		
		if (0 == autoteam_pre_limit_co(pstEnv, pstOne, pstSingle->nLimitFlag, pstAutoTeamPre,iAutoTeamID))
		{
			continue;
		}

		pstAutoTeamInfo = &pstOne->stOnline.stAutoTeamInfo;
		pstAutoTeamInfo->Stat = 2;
		pstAutoTeamInfo->AutoTeamID = iAutoTeamID;
		
		autoteam_get_prememinfo_fromplayer(&pstAdd->PreMemInfos[0], pstOne);

		for (j=0; j<pstAutoTeamPre->nSingleNum; j++)
		{
			pstPrePlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[j].iPlayerID);
			if (pstPrePlayer)	
			{
				z_cltmsg_send(pstEnv, pstPrePlayer, &stPkg);
			}
		}

		memcpy(&pstAutoTeamPre->astSingle[pstAutoTeamPre->nSingleNum++], pstSingle, sizeof(pstAutoTeamPre->astSingle[0]));
		//autoteam_pre_mem_info(pstEnv, pstOne, pstAutoTeamPre);

		autoteam_single_del(pstEnv, pstOne->iMemID);

		if (pstAutoTeamPre->nSingleNum >= MAX_TEAM_MEMBER)
		{
			break;
		}
	}
	autoteam_pre_just_caption_pos(pstEnv, pstAutoTeamPre);
	return 0;
}

int autoteam_tick_player(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	AUTOTEAMAPPLYINFO stApply;
	AutoTeamInfo *pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;
	AutoTeam *pstAutoTeam = &pstEnv->pstZoneObj->stAutoTeam;
	AutoTeamSingle *pstSingle;
	
	if (pstAutoTeamInfo->LimitTrigFlag || 
		pstEnv->pstAppCtx->stCurr.tv_sec - pstAutoTeamInfo->tStart < LIMIT_LOOSE_TIME)
	{
		return 0;
	}

	pstAutoTeamInfo->LimitTrigFlag = 1;
	pstAutoTeamInfo->stApply.LimitFlag &= ~AUTO_TEAM_LIMIT_LEADER;
	memcpy(&stApply, &pstAutoTeamInfo->stApply, sizeof(stApply));
	autoteam_apply_timeout(&stApply);
			
	switch(pstAutoTeamInfo->Stat)
	{
		case 0:
			autoteam_single_del(pstEnv, pstPlayer->iMemID);
			autoteam_single_apply(pstEnv, pstPlayer, &stApply);
			break;
		case 1:
			{
				AutoTeamCreated *pstAutoTeamCreated;
				
				pstAutoTeamCreated = autoteam_find_created_by_playerid(&pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID], 
																	pstPlayer->iMemID, &pstSingle);
				if (pstAutoTeamCreated)
				{
					pstSingle->nLimitFlag = pstSingle->nLimitFlag & (AUTO_TEAM_LIMIT_EASY|AUTO_TEAM_LIMIT_NORM|AUTO_TEAM_LIMIT_DIFF|AUTO_TEAM_LIMIT_NIGHTMARE);
					autoteam_created_loose_limit(pstEnv, &pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID],
												pstAutoTeamCreated, pstAutoTeamInfo->AutoTeamID);
				}
			}
			break;
		case 2:
			{
				AutoTeamPre *pstAutoTeamPre;

				pstAutoTeamPre = autoteam_find_pre_by_playerid(&pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID], 
															pstPlayer->iMemID, &pstSingle);

				if (pstAutoTeamPre)
				{
					pstSingle->nLimitFlag = pstSingle->nLimitFlag & (AUTO_TEAM_LIMIT_EASY|AUTO_TEAM_LIMIT_NORM|AUTO_TEAM_LIMIT_DIFF|AUTO_TEAM_LIMIT_NIGHTMARE);
					autoteam_pre_loose_limit(pstEnv, &pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID],
											pstAutoTeamPre, pstAutoTeamInfo->AutoTeamID);
				}
			}
			break;
		default:
			break;
	}
	
	return 0;

}

int autoteam_team_info_sync(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeam,int OperType)
{
	int i = 0;
	CSPKG stPkg;
	CSAUTOTEAMSVR *pstAutoTeamSvr = &stPkg.Body.AutoTeamSvr;
	CSAUTOTEAMENDSYNC *pstAutoTeamEndSync = &pstAutoTeamSvr->AutoTeamSvrData.AutoTeamEndSync;
	AutoTeamInfo *pstAutoTeamInfo = &pstZoneTeam->stAutoTeamInfo;
	//ZONEPWORLDINST *pstZonePworldInst = NULL;
	ACTIVEHELPER *pstActiveHelp = NULL;
	Player	*pstOne = NULL;
	
	
	if (0 != pstAutoTeamInfo->tStart ||
		0 > pstAutoTeamInfo->AutoTeamID ||
		0 >= pstAutoTeamInfo->TransAutoTeamID )
	{
		if( OperType==1 && pstZoneTeam->stAutoTeamInfo.iPworldGrade == DIFF_PWORLD_GRADE_A)
		{
			
		}
		else
		{
			return 0;
		}
	}

	pstActiveHelp = find_active_by_autoteam(pstEnv, pstAutoTeamInfo->AutoTeamID);
	if (NULL == pstActiveHelp)
	{
		pstAutoTeamInfo->AutoTeamID = 0;
	}
		

	if( pstActiveHelp && pstZoneTeam->iPworldNum > 0)
	{
		//队伍成员在副本的情况
		for(i=0; i<pstZoneTeam->iPworldNum; i++)
		{
			/*pstZonePworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstZoneTeam->stPworldInfo[i].iPworldInstID);
			if (pstZonePworldInst && 
				pstActiveHelp->TaskPworldID != pstZonePworldInst->stPworldIdx.iID)
			{
				pstAutoTeamInfo->AutoTeamID = 0;
				pstAutoTeamInfo->tAutoTeamValidTime = 0;
			}*/
		}
	}
	if(OperType == 1)
	{
		for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
		{
			pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
			if (pstOne)
			{
				if( OperType == 1 &&
					pstZoneTeam->stAutoTeamInfo.iPworldGrade == DIFF_PWORLD_GRADE_A)
				{
					autoteam_notify_clt_clean(pstEnv, pstOne);
				}
			}
		}
		memset(&pstZoneTeam->stAutoTeamInfo,0,sizeof(pstZoneTeam->stAutoTeamInfo));
		//player_team_broadcast_tip(pstEnv, pstZoneTeam,"队伍副本关闭了!");
	}

	//确保后加入的也能快速进来
	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (pstOne)
		{
			pstOne->stOnline.stAutoTeamInfo.AutoTeamID = pstAutoTeamInfo->AutoTeamID;
		}
	}


	pstAutoTeamEndSync->iTeamIdx = pstZoneTeam->iTeamIdx;
	//pstAutoTeamEndSync->AutoTeamID = pstAutoTeamInfo->AutoTeamID;
	//pstAutoTeamEndSync->PworldGrade = pstAutoTeamInfo->bOPFlag;
	pstAutoTeamEndSync->AutoTeamID = pstAutoTeamInfo->TransAutoTeamID;
	pstAutoTeamEndSync->PworldGrade = pstAutoTeamInfo->iPworldGrade;
	pstAutoTeamSvr->AutoTeamSvrOp = AUTOTEAM_SVR_OP_INFO_SYNC;
	Z_CSHEAD_INIT(&stPkg.Head, AUTOTEAM_SVR);
	player_team_broadcast( pstEnv, pstZoneTeam, &stPkg);

	return 0;
}
int autoteam_refresh_team(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeam,int OperType)
{	
	AUTOTEAMAPPLYINFO stApply;
	AutoTeam *pstAutoTeam = &pstEnv->pstZoneObj->stAutoTeam;
	AutoTeamInfo *pstAutoTeamInfo = &pstZoneTeam->stAutoTeamInfo;

	if(OperType == 0)
	{
		//tick
		if (pstAutoTeamInfo->LimitTrigFlag ||
			pstEnv->pstAppCtx->stCurr.tv_sec - pstAutoTeamInfo->tStart < LIMIT_LOOSE_TIME )
		{
			return 0;
		}
		pstAutoTeamInfo->LimitTrigFlag = 1;
	}	
	
	memcpy(&stApply, &pstAutoTeamInfo->stApply, sizeof(stApply));
	if(pstEnv->pstAppCtx->stCurr.tv_sec - pstAutoTeamInfo->tStart >= LIMIT_LOOSE_TIME)
	{
		autoteam_apply_timeout(&stApply);
	}

	if (0 > pstAutoTeamInfo->AutoTeamID)
	{
		autoteam_team_del(pstEnv, pstZoneTeam, -1);
						
		autoteam_team_apply(pstEnv, pstZoneTeam, &stApply);
	}
	else
	{
		AutoTeamCreated *pstAutoTeamCreated;

		pstAutoTeamCreated = autoteam_find_created_by_teamid(&pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID], 
														pstZoneTeam->iTeamIdx);
		if (pstAutoTeamCreated)
		{
			pstAutoTeamCreated->nLimitFlag = pstAutoTeamCreated->nLimitFlag & (AUTO_TEAM_LIMIT_EASY|AUTO_TEAM_LIMIT_NORM|AUTO_TEAM_LIMIT_DIFF|AUTO_TEAM_LIMIT_NIGHTMARE);
			autoteam_created_loose_limit(pstEnv, &pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID],
										pstAutoTeamCreated, pstAutoTeamInfo->AutoTeamID);
		}
	}
	
	return 0;
	
}

int autoteam_team_chg_reject(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeam, int iCancelFlag)
{
	Player *pstPlayer;

	pstPlayer = player_team_get_captain_player(pstEnv, pstZoneTeam);

	//if (pstPlayer)
	{
		autoteam_team_reject(pstEnv, pstPlayer, pstZoneTeam, iCancelFlag);
	}

	return 0;
}

int autoteam_team_chg_allow_check(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeam)
{
	AutoTeamCreated *pstAutoTeamCreated = NULL;

	pstAutoTeamCreated = autoteam_find_created_byteam(pstEnv, pstZoneTeam);
	if(!pstAutoTeamCreated )
	{
		return 0;
	}
	
	if (pstZoneTeam->stTeamInfo.MemberNum + pstAutoTeamCreated->nSingleNum >= MAX_TEAM_MEMBER)
	{
		return -1;	
	}
	
	return 0;
}
//允许队伍改变,自动组队继续
int autoteam_team_chg_allow(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeam)
{
	AutoTeamCreated *pstAutoTeamCreated = NULL;
	TEAMMEMBER *pstMember = NULL;
	Player *pstPlayer = NULL;
	int i=0;

	pstAutoTeamCreated = autoteam_find_created_byteam(pstEnv, pstZoneTeam);
				
	for(i=0;i<pstZoneTeam->stTeamInfo.MemberNum;i++)
	{
		pstMember = &pstZoneTeam->stTeamInfo.TeamMember[i];
		pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstMember->MemberName);
		if (!pstPlayer)
		{
			continue;
		}

		autoteam_created_mem_info(pstEnv, pstPlayer, pstZoneTeam, pstAutoTeamCreated);

	}

	if (pstAutoTeamCreated)
	{
		for (i=0; i<pstAutoTeamCreated->nSingleNum; i++)
		{
			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamCreated->astSingle[i].iPlayerID);
			if (!pstPlayer)
			{
				continue;
			}
			autoteam_created_mem_info(pstEnv, pstPlayer, pstZoneTeam, pstAutoTeamCreated);
		}
	}

	autoteam_refresh_team(pstEnv, pstZoneTeam,1);

	return 0;
}

int autoteam_player_offline(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	/*ZoneTeam *pstZoneTeam;
	
	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if (pstZoneTeam)
	{
		if (0 == pstZoneTeam->stAutoTeamInfo.tStart)
		{
			return 0;
		}
		
		if (pstPlayer == player_team_get_captain_player(pstEnv, pstZoneTeam))
		{
			autoteam_team_chg_reject(pstEnv, pstZoneTeam, 0);
		}
	}
	else
	{
		if (0 == pstPlayer->stOnline.stAutoTeamInfo.tStart)
		{
			return 0;
		}*/
		
		autoteam_reject(pstEnv, pstPlayer,0);
	//}
	
	return 0;
}

int autoteam_info_check_pworld(ZONESVRENV* pstEnv, AutoTeamInfo *pstAutoTeamInfo, int iPworldID)
{
	int i;
	ACTIVEHELPER *pstActiveHelp;
	UNUSED(&iPworldID);
	
	if (pstAutoTeamInfo->AutoTeamID >= 0)
	{
		pstActiveHelp = find_active_by_autoteam(pstEnv, pstAutoTeamInfo->AutoTeamID);
		if (pstActiveHelp && pstActiveHelp->Type)
		{
			return -1;
		}
	}
	else
	{
		for (i=0; i<pstAutoTeamInfo->stApply.AutoTeamIDNum; i++)
		{
			pstActiveHelp = find_active_by_autoteam(pstEnv, pstAutoTeamInfo->stApply.AutoTeamIDInfos[i].AutoTeamID);
			if (pstActiveHelp && pstActiveHelp->Type)
			{
				return -1;
			}
		}
	}
	
	return 0;
}

int autoteam_check_pworld(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, 
								Player *pstPlayer, int iPworldID)
{
	if (pstZoneTeam)
	{
		if (pstZoneTeam->stAutoTeamInfo.tStart && 
			0 > autoteam_info_check_pworld(pstEnv, &pstZoneTeam->stAutoTeamInfo, iPworldID))
		{
			autoteam_team_reject(pstEnv, pstPlayer, pstZoneTeam, 2);
		}
	}
	else
	{
		if (pstPlayer->stOnline.stAutoTeamInfo.tStart &&
			0 > autoteam_info_check_pworld(pstEnv, &pstPlayer->stOnline.stAutoTeamInfo, iPworldID))
		{
			autoteam_reject(pstEnv, pstPlayer,0);
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer,
				SYS_MIDDLE2, "你已进入副本，自动组队取消");
		}
	}
	
	return 0;
}

int autoteam_enter_pworld_safe_time_action(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	pstActionRes->ActionID = ACTION_INPWORD_SAFETIME_DOWN;
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->Data.InPworldSafeTimeDown = pstPlayer->stRoleData.MiscInfo.InPworldSafeTime;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);

	return 0;
}

//改变进入地图的位置
int autoteam_chg_pworld_mappos_byteam(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,
								ZONEPWORLDINST* pstPworldInst,RESPOS *pstPos)
{
	ZoneTeam *pstZoneTeam;
	int iRandom = 0;
	Player* pstPlayerTmp = NULL; 
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;

	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if(!pstZoneTeam)
	{
		pstMiscInfo->InPworldSafeTime = 0;
		return -1;
	}

	if( !pstPworldInst )
	{
		pstMiscInfo->InPworldSafeTime = 0;
		return -1;
	}

	if( pstPworldInst->iPlayer<=0 )
	{
		pstMiscInfo->InPworldSafeTime = 0;
		return -1;
	}
	iRandom = RAND1(pstPworldInst->iPlayer);
	pstPlayerTmp = z_id_player(pstAppCtx, pstEnv, pstPworldInst->aiPlayerID[iRandom]);
	if(!pstPlayerTmp)
	{
		pstMiscInfo->InPworldSafeTime = 0;
		return -1;
	}

	// 直接到那个人的身边去
	if( pstPlayer->stRoleData.MiscInfo.InPworldSafeTime <= 0)
	{
		pstMiscInfo->InPworldSafeTime = 0;
		return -1;
	}	
	if(pstPos)
	{
		pstPos->Y = pstPlayerTmp->stRoleData.Pos.Y;
		pstPos->X= pstPlayerTmp->stRoleData.Pos.X;
	}
		
	return 0;
}

int autoteam_in_pworld_add_safe_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEPWORLDINST* pstPworldInst,Player* pstPlayer)
{
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;

	//tlog_info(pstEnv->pstLogCat, 0, 0, "InPworldSafeTime:%d",pstMiscInfo->InPworldSafeTime);
	if( !pstPworldInst ||
		pstMiscInfo->InPworldSafeTime <= 0)
	{
		z_player_del_status(pstAppCtx, pstEnv, pstPlayer, ENTER_PWORLD_SAFE_STATUSID, 1, 0);
		return -1;
	}

	//tlog_info(pstEnv->pstLogCat, 0, 0, "InPworldSafeTime:%d,chLimitFlag:%d",pstMiscInfo->InPworldSafeTime,pstPworldInst->chLimitFlag);
	if( !pstPworldInst->chLimitFlag )
	{
		//副本还没开始加每日次数的话,不加这个状态
		pstMiscInfo->InPworldSafeTime = 0;
		z_player_del_status(pstAppCtx, pstEnv, pstPlayer, ENTER_PWORLD_SAFE_STATUSID, 1, 0);
		return -1;
	}
	
	//加保护状态buff
	z_player_apply_status(pstAppCtx,pstEnv,pstPlayer, ENTER_PWORLD_SAFE_STATUSID,
								1, pstMiscInfo->InPworldSafeTime,
								1, 0); 

	//发倒计时action
	autoteam_enter_pworld_safe_time_action(pstEnv, pstPlayer);

	return 0;
}

int autoteam_in_pworld_del_saft_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer)
{
	ROLEMISCINFO *pstMiscInfo = &pstPlayer->stRoleData.MiscInfo;

	pstMiscInfo->InPworldSafeTime = 0;

	z_player_del_status(pstAppCtx, pstEnv, pstPlayer, ENTER_PWORLD_SAFE_STATUSID, 1, 0);
	
	return 0;
}

//自动组对的队伍加一个没有在自动组队的人
int autoteam_team_add_player_check(ZONESVRENV* pstEnv, ZoneTeam *pstZoneTeam, Player* pstPlayer)
{
	int i=0;
	AutoTeamInfo *pstAutoTeamInfo = &pstZoneTeam->stAutoTeamInfo;
	AUTOTEAMAPPLYINFO *pstApply = &pstAutoTeamInfo->stApply;
	Player  *pstCaptain = NULL;
	short anAutoTeamIDs[AUTO_TEAM_ID_MAX];
	ACTIVEHELPER *pstActiveHelp;
	AutoTeamCreated *pstAutoTeamCreated = NULL;


	memset(anAutoTeamIDs, 0, sizeof(anAutoTeamIDs));
	if (0 >= pstApply->AutoTeamIDNum)
	{
		return -1;
	}
	pstCaptain = player_team_get_captain_player(pstEnv,pstZoneTeam);
	if(	!pstCaptain )
	{
		return -1;
	}

	pstAutoTeamCreated = autoteam_find_created_byteam(pstEnv, pstZoneTeam);	
	if(pstAutoTeamCreated)
	{
		if (pstZoneTeam->stTeamInfo.MemberNum + pstAutoTeamCreated->nSingleNum > MAX_TEAM_MEMBER)
		{
			return -1;
		}
	}
	
	for (i=0; i<pstApply->AutoTeamIDNum; i++)
	{
		if (pstApply->AutoTeamIDInfos[i].AutoTeamID >= AUTO_TEAM_ID_MAX || anAutoTeamIDs[pstApply->AutoTeamIDInfos[i].AutoTeamID])
		{
			return -1;
		}
		anAutoTeamIDs[pstApply->AutoTeamIDInfos[i].AutoTeamID] = 1;

		pstActiveHelp = find_active_by_autoteam(pstEnv, pstApply->AutoTeamIDInfos[i].AutoTeamID);
		if (NULL == pstActiveHelp)
		{
			return -1;
		}

		if (pstActiveHelp->LevelLimit && pstPlayer->stRoleData.Level < pstActiveHelp->LevelLimit)
		{
			return -1;
		}

		if (pstActiveHelp->Type) //检查副本冷却
		{
			ZONEPWORLD *pstZoneWorld;
			ZONEIDX stIdx;
			int iPworldID;

			iPworldID = autoteam_diffpworld_grade_id_get(pstEnv, pstActiveHelp->TaskPworldID, pstApply->AutoTeamIDInfos[i].LimitFlag);

			stIdx.iID = iPworldID;
			stIdx.iPos = -1;
			pstZoneWorld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
			if (NULL == pstZoneWorld)
			{
				return -1;
			}
			
			if (0 > player_limit_time_pworld_check(pstEnv, pstPlayer, &pstZoneWorld->stPworldDef, 0))
			{
				z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, 
						"你在%d秒内进入过一次%s活动的副本了,已不能再进入", 
					pstZoneWorld->stPworldDef.RedoLimitTime, pstActiveHelp->ActiveName);

				z_sys_strf(pstEnv, pstCaptain, SYS_MIDDLE2, 
						"不能加入新成员 %s 玩家在%d秒内进入过一次%s活动的副本了,已不能再进入", 
						pstPlayer->stRoleData.RoleName,
						pstZoneWorld->stPworldDef.RedoLimitTime, pstActiveHelp->ActiveName);
				return -2;
			}
			 
			if ( 0 > autoteam_check_pworldlevel(pstEnv, pstPlayer,&pstZoneWorld->stPworldDef))
			{
				z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, 
						"此副本要求等级%d-%d, 你的等级不符合,不能进入此副本", 
						pstZoneWorld->stPworldDef.MinLv, pstZoneWorld->stPworldDef.MaxLv);

				z_sys_strf(pstEnv, pstCaptain, SYS_MIDDLE2, 
						"此副本要求等级%d-%d, 玩家 %s 的等级不符合,不能进入此副本", 
						pstZoneWorld->stPworldDef.MinLv, pstZoneWorld->stPworldDef.MaxLv,
						pstPlayer->stRoleData.RoleName);
				return -2;
			}
			
		}
		
	}

	return 0;
}
 

int autoteam_notify_clt_clean(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSAUTOTEAMSVR *pstAutoTeamSvr = &stPkg.Body.AutoTeamSvr;
	CSAUTOTEAMCLEAN *pstAutoTeamClean = &pstAutoTeamSvr->AutoTeamSvrData.AutoTeamClean;	

	if(!pstPlayer)
	{
		return 0;
	}
	pstAutoTeamSvr->AutoTeamSvrOp = AUTOTEAM_SVR_OP_CLEAN;
	pstAutoTeamClean->Uin = pstPlayer->stRoleData.Uin;
		
	Z_CSHEAD_INIT(&stPkg.Head, AUTOTEAM_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}
int autoteam_get_createdteam_num(ZONESVRENV* pstEnv,AutoTeamCreated *pstAutoTeamCreated)
{
	int iTeamMem = 0;
	ZoneTeam *pstZoneTeam;
	TMEMBLOCK *pstMemBlock;
	
	pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstTeamPool, pstAutoTeamCreated->iTeamID);
	if (0 == pstMemBlock->fValid)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "zone team null");
		return 0;
	}

	pstZoneTeam = (ZoneTeam *)pstMemBlock->szData;
	iTeamMem = pstZoneTeam->stTeamInfo.MemberNum + pstAutoTeamCreated->nSingleNum;
	
	return iTeamMem;
}
//自动组队 队伍合并
int autoteam_createdteam_add_createdteam_pre(ZONESVRENV* pstEnv,AutoTeamOne *pstAutoTeamOne,
						AutoTeamCreated *pstAutoTeamCreatedCurr,AutoTeamPreIdx *pstPreIdx,int AutoTeamID )
{
	int i, iTeamMem,j;
	ZoneTeam *pstZoneTeam;
	ZoneTeam *pstZoneTeamCurr;
	TMEMBLOCK *pstMemBlock;
	AutoTeamCreated *pstAutoTeamCreatedTick;
	int iUnValidTeam = -1;
	Player *pstPlayer = NULL;
	int nLimitFlag = 0;

	memset(pstPreIdx,0,sizeof(AutoTeamPreIdx));	
	if (pstAutoTeamCreatedCurr->nSingleNum >= AUTO_TEAM_NEED_MEMBER ||
		pstAutoTeamCreatedCurr->OpFlag )
	{
		return 0;
	}
	
	pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstTeamPool, pstAutoTeamCreatedCurr->iTeamID);
	if (0 == pstMemBlock->fValid)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "zone team null");
		return -1;
	}
	pstZoneTeamCurr = (ZoneTeam *)pstMemBlock->szData; 
	nLimitFlag = pstAutoTeamCreatedCurr->nLimitFlag;
	
	for (i=0; i<pstAutoTeamOne->iCreatedTeamNum; i++)
	{
		pstAutoTeamCreatedTick = &pstAutoTeamOne->astAutoTeamCreated[i];
			
		if (pstAutoTeamCreatedTick->nSingleNum >= AUTO_TEAM_NEED_MEMBER ||
			pstAutoTeamCreatedTick->OpFlag ||
			pstAutoTeamCreatedTick == pstAutoTeamCreatedCurr )
		{
			continue;
		}

		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstTeamPool, pstAutoTeamCreatedTick->iTeamID);
		if (0 == pstMemBlock->fValid)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "zone team null");

			iUnValidTeam = pstAutoTeamCreatedTick->iTeamID;
			continue;
		}
		pstZoneTeam = (ZoneTeam *)pstMemBlock->szData;

		if (pstEnv->pstAppCtx->stCurr.tv_sec - pstZoneTeam->stAutoTeamInfo.tStart < LIMIT_LOOSE_TIME ||
			pstZoneTeam->stAutoTeamInfo.AutoTeamID == 0 )
		{
			continue;
		}
		
		iTeamMem = autoteam_get_createdteam_num(pstEnv,pstAutoTeamCreatedTick);
		if (iTeamMem >= MAX_TEAM_MEMBER)
		{
			continue;
		}
		iTeamMem += autoteam_get_createdteam_num(pstEnv,pstAutoTeamCreatedCurr);
		if (iTeamMem > MAX_TEAM_MEMBER)
		{
			continue;
		}

		//判断队伍的人
		for(j=0;j<pstZoneTeamCurr->stTeamInfo.MemberNum;j++)
		{
			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeamCurr->stTeamInfo.TeamMember[j].MemID);
			if(!pstPlayer)
			{
				break;
			}
			if (0 == autoteam_created_limit_co(pstEnv, pstPlayer, nLimitFlag, pstZoneTeam, pstAutoTeamCreatedTick,AutoTeamID))
			{
				break;
			}

			if(pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE)
			{
				break;
			}
		}
		if(j<pstZoneTeamCurr->stTeamInfo.MemberNum)
		{
			continue;
		}

		for(j=0;j<pstAutoTeamCreatedCurr->nSingleNum;j++)
		{
			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamCreatedCurr->astSingle[j].iPlayerID);
			if(!pstPlayer)
			{
				break;
			}
			if (0 == autoteam_created_limit_co(pstEnv, pstPlayer, nLimitFlag, pstZoneTeam, pstAutoTeamCreatedTick,AutoTeamID))
			{
				break;
			}
			if(pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE)
			{
				break;
			}
		}
		if(j<pstAutoTeamCreatedCurr->nSingleNum)
		{
			continue;
		}
		
		if (iTeamMem > pstPreIdx->iNum)
		{
			pstPreIdx->iNum = iTeamMem;
			pstPreIdx->tTime = pstZoneTeam->stAutoTeamInfo.tStart;
			pstPreIdx->iIdx = i;
			pstPreIdx->nLimitFlag = nLimitFlag;
		}
		
		if (iTeamMem >= MAX_TEAM_MEMBER)
		{
			break;
		}
	}

	if (iUnValidTeam != -1)
	{
		autoteam_team_del_id(pstAutoTeamOne, iUnValidTeam);
	}
	
	return 0;
}

int autoteam_merge_createdteam(ZONESVRENV *pstEnv,ZoneTeam *pstZoneTeamDec,ZoneTeam *pstZoneTeamAdd,Player **pstPlayerSingle,int iSingleNum)
{
	int i = 0;
	int iMember=0;
	Player *pstPlayer[MAX_TEAM_MEMBER];
	TEAMMEMBER *pstTeamMember;
	
	for(i=0;i<pstZoneTeamDec->stTeamInfo.MemberNum;i++)
	{
		pstPlayer[iMember] = z_id_player( pstEnv->pstAppCtx,pstEnv, pstZoneTeamDec->stTeamInfo.TeamMember[i].MemID);
		if(!pstPlayer[iMember])
		{
			continue;
		}
		iMember++;
	}

	team_shadow_chg_true_team(pstEnv, pstZoneTeamDec);
	player_team_destroy_in(pstEnv, pstZoneTeamDec, 2);

	for(i=0;i<iMember;i++)
	{
		pstPlayer[i]->stRoleData.MiscInfo.TeamID = pstZoneTeamAdd->stTeamInfo.TeamID;
		pstPlayer[i]->stRoleData.MiscInfo.TeamIdx = pstZoneTeamAdd->iTeamIdx;
		pstTeamMember = &pstZoneTeamAdd->stTeamInfo.TeamMember[pstZoneTeamAdd->stTeamInfo.MemberNum];
		player_team_sync_member_info(pstEnv, pstTeamMember, pstPlayer[i]);
		pstZoneTeamAdd->stTeamInfo.MemberNum++;
	}

	for(i=0;i<iSingleNum;i++)
	{
		pstPlayerSingle[i]->stRoleData.MiscInfo.TeamID = pstZoneTeamAdd->stTeamInfo.TeamID;
		pstPlayerSingle[i]->stRoleData.MiscInfo.TeamIdx = pstZoneTeamAdd->iTeamIdx;
		pstTeamMember = &pstZoneTeamAdd->stTeamInfo.TeamMember[pstZoneTeamAdd->stTeamInfo.MemberNum];
		player_team_sync_member_info(pstEnv, pstTeamMember, pstPlayerSingle[i]);
		pstZoneTeamAdd->stTeamInfo.MemberNum++;
	}


	team_shadow_chg_true_team(pstEnv, pstZoneTeamAdd);
	player_team_sync(pstEnv, pstZoneTeamAdd);
	autoteam_team_chg_allow(pstEnv, pstZoneTeamAdd);

	if( pstZoneTeamAdd->stAutoTeamInfo.TransAutoTeamID >0)
	{
		autoteam_team_successed_chg_member(pstEnv, pstZoneTeamAdd);
	}
	return 0;
}
////这个pstTeam在里面有可能被消除,放在最后调用
int autoteam_createdteam_add_createdteam(ZONESVRENV *pstEnv, ZoneTeam *pstZoneTeamDec)
{	
	AUTOTEAMAPPLYINFO stApply;
	AutoTeam *pstAutoTeam = &pstEnv->pstZoneObj->stAutoTeam;
	AutoTeamInfo *pstAutoTeamInfo = &pstZoneTeamDec->stAutoTeamInfo;
	AutoTeamOne *pstAutoTeamOne;
	AutoTeamCreated *pstAutoTeamCreated;
	AutoTeamCreated *pstAutoTeamCreatedAdd;
	AutoTeamPreIdx stPreIdx;
	int i = 0;
	int j = 0;
	TMEMBLOCK *pstMemBlock;
	ZoneTeam *pstZoneTeamAdd;
	int iMember=0;
	Player *pstPlayer[MAX_TEAM_MEMBER];
	AutoTeamInfo *pstAutoTeamInfoPlayer;
	int iAutoTeamID = 0;
	
	memset(&stPreIdx,0,sizeof(stPreIdx));
	if (pstEnv->pstAppCtx->stCurr.tv_sec - pstAutoTeamInfo->tStart < LIMIT_LOOSE_TIME ||
		pstAutoTeamInfo->AutoTeamID == 0 )
	{
		return 0;
	}
	
	memcpy(&stApply, &pstAutoTeamInfo->stApply, sizeof(stApply));
	if (0 > pstAutoTeamInfo->AutoTeamID)
	{
		//autoteam_team_del(pstEnv, pstZoneTeam, -1);
		//autoteam_team_apply(pstEnv, pstZoneTeam, &stApply);
		for (i=0; i<stApply.AutoTeamIDNum; i++)
		{
			AUTOTEAMAPPLYIDINFO *pstApplyIDInfo;
			pstApplyIDInfo = &stApply.AutoTeamIDInfos[i];
			pstAutoTeamOne = &pstAutoTeam->astAutoTeams[(int)pstApplyIDInfo->AutoTeamID];
			pstAutoTeamCreated = autoteam_find_created_by_teamid(pstAutoTeamOne, pstZoneTeamDec->iTeamIdx);
			if (pstAutoTeamCreated)
			{
				autoteam_createdteam_add_createdteam_pre(pstEnv,pstAutoTeamOne,pstAutoTeamCreated,&stPreIdx,pstApplyIDInfo->AutoTeamID);
				if(stPreIdx.iNum == 0)
				{
					continue;
				}	

				autoteam_team_del(pstEnv, pstZoneTeamDec, -1);
				
				//加入对方的队伍
				pstAutoTeamCreatedAdd = &pstAutoTeamOne->astAutoTeamCreated[stPreIdx.iIdx];
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstTeamPool, pstAutoTeamCreatedAdd->iTeamID);
				if (0 == pstMemBlock->fValid)
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "zone team null");
					return -1;
				}
				pstZoneTeamAdd = (ZoneTeam *)pstMemBlock->szData;
				iAutoTeamID = pstApplyIDInfo->AutoTeamID;
				autoteam_team_del(pstEnv, pstZoneTeamAdd, iAutoTeamID);
				pstAutoTeamInfo = &pstZoneTeamAdd->stAutoTeamInfo;
				pstAutoTeamInfo->AutoTeamID = iAutoTeamID;
				pstAutoTeamInfo->Stat = 1;
				autoteam_merge_createdteam(pstEnv,pstZoneTeamDec,pstZoneTeamAdd,NULL,0);
			}
			
		}
	}
	else
	{
		pstAutoTeamOne = &pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID];
		pstAutoTeamCreated = autoteam_find_created_by_teamid(pstAutoTeamOne, pstZoneTeamDec->iTeamIdx);
		if (pstAutoTeamCreated)
		{
			autoteam_createdteam_add_createdteam_pre(pstEnv,pstAutoTeamOne,pstAutoTeamCreated,&stPreIdx,pstAutoTeamInfo->AutoTeamID);
			if(stPreIdx.iNum == 0)
			{
				return 0;
			}
			for(j=0;j<pstAutoTeamCreated->nSingleNum;j++)
			{
				pstPlayer[iMember] = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamCreated->astSingle[j].iPlayerID);
				if(!pstPlayer[iMember])
				{
					continue;
				}
				pstAutoTeamInfoPlayer = &pstPlayer[iMember]->stOnline.stAutoTeamInfo;
				pstAutoTeamInfoPlayer->AutoTeamID = pstAutoTeamInfo->AutoTeamID;
				pstAutoTeamInfoPlayer->Stat = 1;
				pstAutoTeamInfoPlayer->bOPFlag = 0;
				iMember++;
				
			}
					
			autoteam_team_del(pstEnv, pstZoneTeamDec, pstAutoTeamInfo->AutoTeamID);	
			//加入对方的队伍
			pstAutoTeamCreatedAdd = &pstAutoTeamOne->astAutoTeamCreated[stPreIdx.iIdx];
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstTeamPool, pstAutoTeamCreatedAdd->iTeamID);
			if (0 == pstMemBlock->fValid)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "zone team null");
				return -1;
			}
			pstZoneTeamAdd = (ZoneTeam *)pstMemBlock->szData;

			iAutoTeamID = pstAutoTeamInfo->AutoTeamID;
			autoteam_team_del(pstEnv, pstZoneTeamAdd, iAutoTeamID);
			pstAutoTeamInfo = &pstZoneTeamAdd->stAutoTeamInfo;
			pstAutoTeamInfo->AutoTeamID = iAutoTeamID;
			pstAutoTeamInfo->Stat = 1;
			autoteam_merge_createdteam(pstEnv,pstZoneTeamDec,pstZoneTeamAdd,pstPlayer,iMember);
			
		}
	}
	return 0;
}

//自动组队说话频道
int autoteam_team_chat(ZONESVRENV* pstEnv, Player *pstPlayer, CSPKG *pstPkg,int iSelf)
{
	AutoTeamCreated *pstAutoTeamCreated = NULL;
	TEAMMEMBER *pstMember = NULL;
	int i=0;
	ZoneTeam *pstZoneTeam = NULL;
	Player *pstPlayerMember = NULL;
	AutoTeamInfo *pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;
	AutoTeam *pstAutoTeam = &pstEnv->pstZoneObj->stAutoTeam;
	AutoTeamSingle *pstSingle;
	AutoTeamPre *pstAutoTeamPre;

	if(iSelf) 
	{
		z_cltmsg_send(pstEnv, pstPlayer, pstPkg);
	}
	
	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if(pstZoneTeam)
	{
		pstAutoTeamCreated = autoteam_find_created_byteam(pstEnv, pstZoneTeam);
					
		for(i=0;i<pstZoneTeam->stTeamInfo.MemberNum;i++)
		{
			pstMember = &pstZoneTeam->stTeamInfo.TeamMember[i];
			pstPlayerMember = z_name_player(pstEnv->pstAppCtx, pstEnv, pstMember->MemberName);
			if (!pstPlayerMember)
			{
				continue;
			}
			if(pstPlayer != pstPlayerMember)
			{
				z_cltmsg_send(pstEnv, pstPlayerMember, pstPkg);
			}
		}

		if (pstAutoTeamCreated)
		{
			for (i=0; i<pstAutoTeamCreated->nSingleNum; i++)
			{
				pstPlayerMember = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamCreated->astSingle[i].iPlayerID);
				if (!pstPlayerMember || pstPlayerMember->eStatus != PLAYER_STATUS_ROLE_LOGIN)
				{
					continue;
				}
				if(pstPlayer != pstPlayerMember)
				{
					z_cltmsg_send(pstEnv, pstPlayerMember, pstPkg);
				}
			}
		}

		return 0;
	}
	
	switch(pstAutoTeamInfo->Stat)
	{
		case 1:
			pstAutoTeamCreated = autoteam_find_created_by_playerid(&pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID], 
																	pstPlayer->iMemID, &pstSingle);
			if (pstAutoTeamCreated)
			{
				TMEMBLOCK *pstMemBlock;
				for (i=0; i<pstAutoTeamCreated->nSingleNum; i++)
				{
					pstPlayerMember = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamCreated->astSingle[i].iPlayerID);
					if (!pstPlayerMember || pstPlayerMember->eStatus != PLAYER_STATUS_ROLE_LOGIN)
					{
						continue;
					}
					if(pstPlayer != pstPlayerMember)
					{
						z_cltmsg_send(pstEnv, pstPlayerMember, pstPkg);
					}
				}
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstTeamPool, pstAutoTeamCreated->iTeamID);
				if (0 == pstMemBlock->fValid)
				{
					tlog_error(pstEnv->pstLogCat, 0, 0, "zone team null");
					return -1;
				}
				pstZoneTeam = (ZoneTeam *)pstMemBlock->szData;
				for(i=0;i<pstZoneTeam->stTeamInfo.MemberNum;i++)
				{
					pstMember = &pstZoneTeam->stTeamInfo.TeamMember[i];
					pstPlayerMember = z_name_player(pstEnv->pstAppCtx, pstEnv, pstMember->MemberName);
					if (!pstPlayerMember)
					{
						continue;
					}
					if(pstPlayer != pstPlayerMember)
					{
						z_cltmsg_send(pstEnv, pstPlayerMember, pstPkg);
					}
				}
			}
			
			break;
		case 2:
			pstAutoTeamPre = autoteam_find_pre_by_playerid(&pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID], 
																	pstPlayer->iMemID, &pstSingle);
			if (pstAutoTeamPre)
			{
				for (i=pstAutoTeamPre->nSingleNum-1; i>=0; i--)
				{					
					pstPlayerMember = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[i].iPlayerID);
					if (!pstPlayerMember || pstPlayerMember->eStatus != PLAYER_STATUS_ROLE_LOGIN)
					{
						continue;
					}
					if(pstPlayer != pstPlayerMember)
					{
						z_cltmsg_send(pstEnv, pstPlayerMember, pstPkg);
					}
					
				}

			}
			break;
		default:
			break;
	}
	
	return 0;
}

/*
int autoteam_create_pworld_vig_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i = 0;
	int nDiffLimit =0, nDiffLimitTmp = 0;
	ZoneTeam *pstZoneTeam;
	AutoTeamCreated *pstAutoTeamCreated;
	AutoTeamInfo *pstAutoTeamInfo;
	AutoTeamSingle *pstSingle;
	AutoTeamPre *pstAutoTeamPre;
	Player *pstOne;
	AutoTeam *pstAutoTeam = &pstEnv->pstZoneObj->stAutoTeam;

	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if (pstZoneTeam)
	{
		if (pstPlayer != player_team_get_captain_player(pstEnv, pstZoneTeam))
		{
			return -1;
		}

		pstAutoTeamInfo = &pstZoneTeam->stAutoTeamInfo;
		if (0 > pstAutoTeamInfo->AutoTeamID)
		{
			return 0;
		}

		pstAutoTeamCreated = autoteam_find_created_by_teamid(&pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID], 
														pstZoneTeam->iTeamIdx);
		if (NULL == pstAutoTeamCreated)
		{
			return -1;
		}

		nDiffLimit = autoteam_limit_filter(autoteam_find_apply_limit(&pstZoneTeam->stAutoTeamInfo.stApply, pstAutoTeamInfo->AutoTeamID));
		nDiffLimitTmp = nDiffLimit;
		for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
		{
			pstOne = z_name_player(pstEnv->pstAppCtx, pstEnv, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
			if (pstOne)
			{
				nDiffLimit &= autoteam_limit_filter(autoteam_find_apply_limit(&pstOne->stOnline.stAutoTeamInfo.stApply, pstAutoTeamInfo->AutoTeamID));
				memset(&pstOne->stOnline.stAutoTeamInfo, 0, sizeof(pstOne->stOnline.stAutoTeamInfo));
			}
		}
		
		return 0;
	}
	
	pstAutoTeamInfo = &pstPlayer->stOnline.stAutoTeamInfo;
	if (AUTOTEAM_OP_CREATE & pstAutoTeamInfo->bOPFlag)
	{
		return -1;
	}

	if (0 > pstAutoTeamInfo->AutoTeamID)
	{
		return -1;
	}
	
	switch(pstAutoTeamInfo->Stat)
	{
		case 2:
			pstAutoTeamPre = autoteam_find_pre_by_playerid(&pstAutoTeam->astAutoTeams[(int)pstAutoTeamInfo->AutoTeamID], 
															pstPlayer->iMemID, &pstSingle);

			if (NULL == pstAutoTeamPre)
			{
				return -1;
			}
			
			if (pstAutoTeamPre->astSingle[0].iPlayerID != pstPlayer->iMemID)
			{
				return -1;
			}


			//其他人同意		
			for (i=0; i<pstAutoTeamPre->nSingleNum; i++)
			{
				Player *pstOne = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAutoTeamPre->astSingle[i].iPlayerID);
				if (pstOne)
				{
					//难度
				}
			}
			
			break;
		default:
			return -1;
			break;
	}
	
	return 0;
}
*/
#endif
