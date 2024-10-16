#include "zone_db.h"
#include "zone_clt.h"
#include "zone_attr.h"
#include "zone_mail.h"
#include "zone_package.h"
#include "zone_diffpworld.h"
#include "zone_team.h"
#include "zone_move.h"
#include "zone_syncrelation.h"
#include "zone_player.h"
#include "zone_err.h"
#include "zone_map.h"
#include "zone_zh_name.h"
#include "zone_shop.h"

#define MAX_FINI_SCHOOL 3

/* 对称的关系，不存在单方有 信息，另一方没有*/

int school_role_login(ZONESVRENV* pstEnv, Player *pstPlayer);

int school_del_mem(ROLESCHOOL *pstRoleSchool, tdr_ulonglong ullRoleID);

int school_student_add_xinde(ROLESCHOOL *pstRoleSchool, int iExp, int iSchoolVal);

int school_teacher_add_val(ROLESCHOOL *pstRoleSchool, tdr_ulonglong ullRoleID, int iTotalVal);


int player_school_student_mem_info(ZONESVRENV* pstEnv, Player *pstPlayer, ROLESTUDENTINFO *pstRoleStudentInfo);

int player_school_teacher_mem_info(ZONESVRENV* pstEnv, Player *pstPlayer, ROLETEACHERINFO *pstRoleTeacherInfo);


SCHOOLLEVELDEF * z_find_school_level_def(ZONESVRENV *pstEnv, int iLevel)
{
	int iIndex;
	int iEqu;
	SCHOOLLEVELDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.Level = iLevel;
	iIndex = bsearch_int(&stDef, pstObj->sSchoolLevelDef, pstObj->iSchoolLevelDef, SCHOOL_LEVEL_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (SCHOOLLEVELDEF*)pstObj->sSchoolLevelDef[iIndex];
		
	}
	return NULL;
}

SCHOOLPWORLDDEF * z_find_school_pworld_def(ZONESVRENV *pstEnv, int iPworldID)
{
	int iIndex;
	int iEqu;
	SCHOOLPWORLDDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.PworldID = iPworldID;
	iIndex = bsearch_int(&stDef, pstObj->sSchoolPworldDef, pstObj->iSchoolPworldDef, SCHOOL_PWORLD_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (SCHOOLPWORLDDEF*)pstObj->sSchoolPworldDef[iIndex];
		
	}
	return NULL;
}

SCHOOLXINDEDEF * z_find_school_xinde_def(ZONESVRENV *pstEnv, int iLevel)
{
	int iIndex;
	int iEqu;
	SCHOOLXINDEDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	stDef.Level = iLevel;
	iIndex = bsearch_int(&stDef, pstObj->sSchoolXinDeDef, pstObj->iSchoolXinDeDef, SCHOOL_XINDE_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (SCHOOLXINDEDEF*)pstObj->sSchoolXinDeDef[iIndex];
		
	}
	return NULL;
}

int role_base_db_res(ZONESVRENV* pstEnv, Player *pstPlayer, TORMSELECTRES *pstSelectRes,DBAsyn *pstDbsyn)
{
	int i;
	ROLEBASEINFO stRoleBase;
	ROLESCHOOL *pstRoleSchool = &pstPlayer->stRoleData.SyncRelation.School;
	RoleBaseSelect *pstRoleBaseSelect = &pstDbsyn->AsynCmd.stRoleBaseSelect;

	if (1 != pstSelectRes->SelectResult.ResultNum)
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
					"role_base_db_res RoleID =%llu ", pstRoleBaseSelect->ullRoleID);
		return -1;
	}

	if(pstPlayer->stRoleData.RoleID != pstRoleBaseSelect->ullRoleID)
	{
		return -1;
	}

	if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stRoleBase, sizeof(stRoleBase)))
	{
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0,
					"RoleID=%llu torm_fetch_one_meta fail", pstRoleBaseSelect->ullRoleID);
		return -1;
	}

	if(pstRoleSchool->JobType == ROLE_SCHOOL_TYPE_TEACHER)
	{
		ROLETEACHER *pstRoleTeacher = &pstRoleSchool->Data.RoleTeacher;

		for(i = 0; i < pstRoleTeacher->Num; i++)
		{
			if(pstRoleTeacher->Students[i].RoleID != stRoleBase.RoleID)
			{
				continue;
			}

			pstRoleTeacher->Students[i].Level = stRoleBase.Level;
			pstRoleTeacher->Students[i].Head = stRoleBase.Head;
			pstRoleTeacher->Students[i].Face = stRoleBase.Face;
			pstRoleTeacher->Students[i].Hair = stRoleBase.Hair;
			pstRoleTeacher->Students[i].Gender = stRoleBase.Gender;
			pstRoleTeacher->Students[i].Career = stRoleBase.Career;
			pstRoleTeacher->Students[i].MeiLi = stRoleBase.RoleDetail.WhiteJade;
			pstRoleTeacher->Students[i].DBFlag  = 1;
			
			player_school_student_mem_info(pstEnv, pstPlayer, &pstRoleTeacher->Students[i]);
			break;
		}
	}
	else if(pstRoleSchool->JobType == ROLE_SCHOOL_TYPE_STUDENT)
	{
		ROLESTUDENT *pstRoleStudent = &pstRoleSchool->Data.RoleStudent;

		if(pstRoleStudent->Teacher.RoleID != stRoleBase.RoleID)
		{
			return -1;
		}

		pstRoleStudent->Teacher.Level = stRoleBase.Level;
		pstRoleStudent->Teacher.Head = stRoleBase.Head;
		pstRoleStudent->Teacher.Face = stRoleBase.Face;
		pstRoleStudent->Teacher.Hair = stRoleBase.Hair;
		pstRoleStudent->Teacher.Gender = stRoleBase.Gender;
		pstRoleStudent->Teacher.Career = stRoleBase.Career;
		pstRoleStudent->Teacher.MeiLi = stRoleBase.RoleDetail.WhiteJade;
		pstRoleStudent->Teacher.DBFlag  = 1;

		player_school_teacher_mem_info(pstEnv, pstPlayer, &pstRoleStudent->Teacher);
	}
	else
	{
		return 0;
	}
	
	return 0;
}

int role_base_db_get(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong ullRoleID)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	ROLEBASEINFO stRoleBase;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	DBAsyn stDBAsyn;
		
	stDBAsyn.iCmd = DB_ROLE_BASE_SELECT;
	stDBAsyn.AsynCmd.stRoleBaseSelect.Uin = pstPlayer->stRoleData.Uin;
	stDBAsyn.AsynCmd.stRoleBaseSelect.ullRoleID = pstPlayer->stRoleData.RoleID;
	
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, "RoleBaseInfo", pstPlayer->stRoleData.Uin, &stDBAsyn);
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	//sprintf(pstSelectReq->OperInfo.WhereDef, "where RoleID=%llu and CtrlFlag!=%d", ullRoleID, ROLE_DEL_FLAG);
	sprintf(pstSelectReq->OperInfo.WhereDef, "where RoleID=%llu", ullRoleID);
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 0;

	memset(&stRoleBase, 0, sizeof(stRoleBase));
	stRoleBase.RoleID = ullRoleID;
		
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRoleBase;
	stMetaData.iBuff = sizeof(stRoleBase);

	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}
		
	return 0;
}

int role_sync_relation_update( ZONESVRENV * pstEnv, const CHGSYNCRELATION *pstRoleSyncRelation)
{
	DBAsyn stDBAsyn;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;

	stDBAsyn.iCmd = DB_ROLE_SYNC_RELATION_UPDATE;
	z_init_torm_head( pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ, 
		"ChgSyncRelation", -1, &stDBAsyn );
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)pstRoleSyncRelation;
	stMetaData.iBuff = sizeof(*pstRoleSyncRelation);
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}

	return 0;
}

int role_sync_relation_select_res(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes,DBAsyn *pstDbsyn)
{
	CHGSYNCRELATION stSyncRelation;
	RoleSyncRelationSelect  *pstSyncRelationSelect = &pstDbsyn->AsynCmd.stSyncRelationSelect;
	int i;

	if (1 != pstSelectRes->SelectResult.ResultNum)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
					"role_base_db_res RoleID =%llu ", pstSyncRelationSelect->ullRoleID);
		return -1;
	}

	if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stSyncRelation, sizeof(stSyncRelation)))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
					"RoleID=%llu torm_fetch_one_meta fail", pstSyncRelationSelect->ullRoleID);
		return -1;
	}

	if(pstSyncRelationSelect->iType == 0)
	{
		school_teacher_add_val(&stSyncRelation.SyncRelation.School, 
			pstSyncRelationSelect->ullRoleID, pstSyncRelationSelect->stSelectInfo.stTeacherAdd.iTotalVal);
	}
	else if(pstSyncRelationSelect->iType == 1)
	{
		school_student_add_xinde(&stSyncRelation.SyncRelation.School, 
			pstSyncRelationSelect->stSelectInfo.stStudentAdd.iExp, pstSyncRelationSelect->stSelectInfo.stStudentAdd.iVal);
	}
	else if(pstSyncRelationSelect->iType == 2)
	{
		school_del_mem(&stSyncRelation.SyncRelation.School, pstSyncRelationSelect->ullRoleID);
	}
	else if (pstSyncRelationSelect->iType == 3) // 老师改名
	{
		strncpy(stSyncRelation.SyncRelation.School.Data.RoleStudent.Teacher.RoleName, pstSyncRelationSelect->szNewName,
				sizeof(stSyncRelation.SyncRelation.School.Data.RoleStudent.Teacher.RoleName));
	}
	else if (pstSyncRelationSelect->iType == 4) // 学生改名
	{
		for (i=0; i<stSyncRelation.SyncRelation.School.Data.RoleTeacher.Num; i++)
		{
			if (stSyncRelation.SyncRelation.School.Data.RoleTeacher.Students[i].RoleID == pstSyncRelationSelect->ullRoleID)
			{
				strncpy(stSyncRelation.SyncRelation.School.Data.RoleTeacher.Students[i].RoleName, pstSyncRelationSelect->szNewName,
					sizeof(stSyncRelation.SyncRelation.School.Data.RoleTeacher.Students[i].RoleName));
				break;
			}
		}
	}

	role_sync_relation_update(pstEnv, &stSyncRelation);
	
	return 0;
}

int role_sync_relation_select_req(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong ullRoleID, DBAsyn *pstDBAsyn)
{
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	CHGSYNCRELATION stChgSyncRelation;
	TDRDATA stNetData;
	TDRDATA stMetaData;
		
	pstDBAsyn->iCmd = DB_ROLE_SYNC_RELATION_SELECT;
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_SELECT_REQ, "ChgSyncRelation", pstPlayer->stRoleData.Uin, pstDBAsyn);
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	sprintf(pstSelectReq->OperInfo.WhereDef, "where RoleID=%llu and CtrlFlag!=%d", ullRoleID, ROLE_DEL_FLAG);
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 0;

	memset(&stChgSyncRelation, 0, sizeof(stChgSyncRelation));
	stChgSyncRelation.RoleID = ullRoleID;
		
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stChgSyncRelation;
	stMetaData.iBuff = sizeof(stChgSyncRelation);

	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if (0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}
		
	return 0;
}

int sync_relation_login(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	school_role_login(pstEnv, pstPlayer);
	
	return 0;
}

int school_student_info_update(ROLESTUDENTINFO *pstRoleStudentInfo, Player *pstPlayer)
{
	pstRoleStudentInfo->Level = pstPlayer->stRoleData.Level;
	pstRoleStudentInfo->Head = pstPlayer->stRoleData.Head;
	pstRoleStudentInfo->Face = pstPlayer->stRoleData.Face;
	pstRoleStudentInfo->Hair = pstPlayer->stRoleData.Hair;
	pstRoleStudentInfo->Gender = pstPlayer->stRoleData.Gender;
	pstRoleStudentInfo->Career = pstPlayer->stRoleData.Career;
	pstRoleStudentInfo->MeiLi = pstPlayer->stRoleData.RoleDetail.WhiteJade;
	STRNCPY(pstRoleStudentInfo->ClanName, 
				pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Name, CCH(pstRoleStudentInfo->ClanName));
	pstRoleStudentInfo->DBFlag = 1;
	return 0;
}

int school_teacher_info_update(ROLETEACHERINFO *pstRoleTeacherInfo, Player *pstPlayer)
{
	pstRoleTeacherInfo->Level = pstPlayer->stRoleData.Level;
	pstRoleTeacherInfo->Head = pstPlayer->stRoleData.Head;
	pstRoleTeacherInfo->Face = pstPlayer->stRoleData.Face;
	pstRoleTeacherInfo->Hair = pstPlayer->stRoleData.Hair;
	pstRoleTeacherInfo->Gender = pstPlayer->stRoleData.Gender;
	pstRoleTeacherInfo->Career = pstPlayer->stRoleData.Career;
	pstRoleTeacherInfo->MeiLi = pstPlayer->stRoleData.RoleDetail.WhiteJade;
	STRNCPY(pstRoleTeacherInfo->ClanName, 
				pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Name, CCH(pstRoleTeacherInfo->ClanName));
	pstRoleTeacherInfo->DBFlag = 1;
	
			
	return 0;
}

int school_student_info2cs(ZONESVRENV* pstEnv, CSROLESTUDENTINFO *pstCSRoleStudentInfo, ROLESTUDENTINFO *pstRoleStudentInfo)
{
	Player *pstTmpPlayer;
	ZoneTeam *pstTeam;

	pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstRoleStudentInfo->RoleName);
	if(pstTmpPlayer)
	{
		//目标在线，先更新
		school_student_info_update(pstRoleStudentInfo, pstTmpPlayer);
	}
	
	STRNCPY(pstCSRoleStudentInfo->RoleName, 
				pstRoleStudentInfo->RoleName, CCH(pstCSRoleStudentInfo->RoleName));

	STRNCPY(pstCSRoleStudentInfo->ClanName, 
				pstRoleStudentInfo->ClanName, CCH(pstCSRoleStudentInfo->ClanName));
			
	pstCSRoleStudentInfo->RoleID = pstRoleStudentInfo->RoleID;
	pstCSRoleStudentInfo->Level = pstRoleStudentInfo->Level;
	pstCSRoleStudentInfo->Head = pstRoleStudentInfo->Head;
	pstCSRoleStudentInfo->Face = pstRoleStudentInfo->Face;
	pstCSRoleStudentInfo->Hair = pstRoleStudentInfo->Hair;
	pstCSRoleStudentInfo->Gender = pstRoleStudentInfo->Gender;
	pstCSRoleStudentInfo->Career = pstRoleStudentInfo->Career;
	pstCSRoleStudentInfo->DayTime = pstRoleStudentInfo->DayTime;
	pstCSRoleStudentInfo->JoinTime = pstRoleStudentInfo->JoinTime;
	pstCSRoleStudentInfo->JoinLevel = pstRoleStudentInfo->JoinLevel;
	pstCSRoleStudentInfo->MeiLi = pstRoleStudentInfo->MeiLi;
	pstCSRoleStudentInfo->Map = 0;
	pstCSRoleStudentInfo->TeamNum = 0;
	pstCSRoleStudentInfo->Online = 0;
	
	if(pstTmpPlayer)
	{
		pstCSRoleStudentInfo->Online = 1;
		pstCSRoleStudentInfo->Map = GET_MAPID(pstTmpPlayer->stRoleData.Map);
		pstTeam = player_team_get(pstEnv, pstTmpPlayer);
		if (pstTeam)
		{
			pstCSRoleStudentInfo->TeamNum = pstTeam->stTeamInfo.MemberNum;
		}
	}
	
	return 0;
}

int school_teacher_info2cs(ZONESVRENV* pstEnv, CSROLETEACHERINFO *pstCSRoleTeacherInfo, ROLETEACHERINFO *pstRoleTeacherInfo)
{
	Player *pstTmpPlayer;
	ZoneTeam *pstTeam;

	pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstRoleTeacherInfo->RoleName);
	if(pstTmpPlayer)
	{
		//目标在线，先更新
		school_teacher_info_update(pstRoleTeacherInfo, pstTmpPlayer);
	}
	
	STRNCPY(pstCSRoleTeacherInfo->RoleName, 
				pstRoleTeacherInfo->RoleName, CCH(pstCSRoleTeacherInfo->RoleName));

	STRNCPY(pstCSRoleTeacherInfo->ClanName, 
				pstRoleTeacherInfo->ClanName, CCH(pstCSRoleTeacherInfo->ClanName));

	pstCSRoleTeacherInfo->RoleID = pstRoleTeacherInfo->RoleID;
	pstCSRoleTeacherInfo->Level = pstRoleTeacherInfo->Level;
	pstCSRoleTeacherInfo->Head = pstRoleTeacherInfo->Head;
	pstCSRoleTeacherInfo->Face = pstRoleTeacherInfo->Face;
	pstCSRoleTeacherInfo->Hair = pstRoleTeacherInfo->Hair;
	pstCSRoleTeacherInfo->Gender = pstRoleTeacherInfo->Gender;
	pstCSRoleTeacherInfo->Career = pstRoleTeacherInfo->Career;
	pstCSRoleTeacherInfo->JoinLevel = pstRoleTeacherInfo->JoinLevel;
	pstCSRoleTeacherInfo->JoinTime = pstRoleTeacherInfo->JoinTime;
	pstCSRoleTeacherInfo->MeiLi = pstRoleTeacherInfo->MeiLi;
	pstCSRoleTeacherInfo->Map = 0;
	pstCSRoleTeacherInfo->TeamNum = 0;
	pstCSRoleTeacherInfo->Online = 0;
		
	if(pstTmpPlayer)
	{
		pstCSRoleTeacherInfo->Online = 1;
		pstCSRoleTeacherInfo->Map = GET_MAPID(pstTmpPlayer->stRoleData.Map);
		pstTeam = player_team_get(pstEnv, pstTmpPlayer);
		if (pstTeam)
		{
			pstCSRoleTeacherInfo->TeamNum = pstTeam->stTeamInfo.MemberNum;
		}
	}

	return 0;
}

int school_all_info_action(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	CSPKG stPkg;
	CSSCHOOLSVR *pstCSSchoolSvr = &stPkg.Body.SchoolSvr;
	ROLESCHOOL *pstRoleSchool = &pstPlayer->stRoleData.SyncRelation.School;

	pstCSSchoolSvr->Type = SCHOOL_SVR_ALL_INFO;
	pstCSSchoolSvr->Data.AllInfo.JobType = pstRoleSchool->JobType;
	if(pstRoleSchool->JobType == ROLE_SCHOOL_TYPE_TEACHER)
	{
		CSROLETEACHER *pstCSRoleTeacher = &pstCSSchoolSvr->Data.AllInfo.Data.Teacher;
		ROLETEACHER *pstRoleTeacher = &pstRoleSchool->Data.RoleTeacher;

		pstCSRoleTeacher->Num = pstRoleTeacher->Num;
		for(i = 0; i < pstRoleTeacher->Num; i++)
		{
			school_student_info2cs(pstEnv, &pstCSRoleTeacher->Students[i], &pstRoleTeacher->Students[i]);
		}
	}
	else if(pstRoleSchool->JobType == ROLE_SCHOOL_TYPE_STUDENT)
	{
		CSROLESTUDENT *pstCSRoleStudent = &pstCSSchoolSvr->Data.AllInfo.Data.Student;
		ROLESTUDENT *pstRoleStudent = &pstRoleSchool->Data.RoleStudent;

		pstCSRoleStudent->Exp = pstRoleStudent->Exp;
		pstCSRoleStudent->Val = pstRoleStudent->Val;
		pstCSRoleStudent->AwardCount = pstRoleStudent->AwardCount;
		pstCSRoleStudent->TotalCount = pstRoleStudent->TotalCount;
		
		school_teacher_info2cs(pstEnv, &pstCSRoleStudent->Teacher, &pstRoleStudent->Teacher);
	}

	Z_CSHEAD_INIT(&stPkg.Head, SCHOOL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int school_role_login_schoolval_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	ROLESCHOOL *pstRoleSchool = &pstPlayer->stRoleData.SyncRelation.School;
	ROLETEACHER *pstRoleTeacher = &pstRoleSchool->Data.RoleTeacher;

	if(pstRoleSchool->JobType != ROLE_SCHOOL_TYPE_TEACHER)
	{
		return -1;
	}

	for(i = 0; i < pstRoleTeacher->Num; i++)
	{
		if(pstRoleTeacher->Students[i].TotalVal > pstRoleTeacher->Students[i].AwardVal)
		{
			player_add_school_val(pstEnv, pstPlayer, pstRoleTeacher->Students[i].TotalVal-pstRoleTeacher->Students[i].AwardVal);
			pstRoleTeacher->Students[i].AwardVal = pstRoleTeacher->Students[i].TotalVal;
		}
	}

	return 0;
}

int school_role_online_action(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstOther, int fOnline)
{
	CSPKG stPkg;
	CSSCHOOLSVR *pstCSSchoolSvr = &stPkg.Body.SchoolSvr;

	pstCSSchoolSvr->Type = SCHOOL_SVR_ONLINE_UPDATE;
	
	pstCSSchoolSvr->Data.Online.RoleID = pstPlayer->stRoleData.RoleID;
	STRNCPY(pstCSSchoolSvr->Data.Online.RoleName, 
		pstPlayer->stRoleData.RoleName, CCH(pstCSSchoolSvr->Data.Online.RoleName));
	
	pstCSSchoolSvr->Data.Online.Online = fOnline;

	Z_CSHEAD_INIT(&stPkg.Head, SCHOOL_SVR);
	z_cltmsg_send(pstEnv, pstOther, &stPkg);
	
	return 0;
}

int school_role_logout(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	Player *pstTmpPlayer;
	ROLESCHOOL *pstRoleSchool = &pstPlayer->stRoleData.SyncRelation.School;

	if(pstRoleSchool->JobType == ROLE_SCHOOL_TYPE_TEACHER)
	{
		ROLETEACHER *pstRoleTeacher = &pstRoleSchool->Data.RoleTeacher;

		for(i = 0; i < pstRoleTeacher->Num; i++)
		{
			pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstRoleTeacher->Students[i].RoleName);
			if(pstTmpPlayer)
			{
				ROLESTUDENT *pstRoleStudent = &pstTmpPlayer->stRoleData.SyncRelation.School.Data.RoleStudent;
				
				school_teacher_info_update(&pstRoleStudent->Teacher, pstPlayer);
				school_role_online_action(pstEnv, pstPlayer, pstTmpPlayer, 0);
			}
		}
	}
	else if(pstRoleSchool->JobType == ROLE_SCHOOL_TYPE_STUDENT)
	{
		ROLESTUDENT *pstRoleStudent = &pstRoleSchool->Data.RoleStudent;
		
		pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstRoleStudent->Teacher.RoleName);
		if(pstTmpPlayer)
		{
			ROLESTUDENTINFO *pstRoleStudentInfo;
			
			pstRoleStudentInfo = school_teacher_get_stuinfo(pstEnv, pstTmpPlayer, pstPlayer->stRoleData.RoleID);
			if(pstRoleStudentInfo)
			{
				school_student_info_update(pstRoleStudentInfo, pstPlayer);
			}
			school_role_online_action(pstEnv, pstPlayer, pstTmpPlayer, 0);
		}
	}

	return 0;
}

//DBFlag == 1 数据一定是最新，无需从DB获取(角色下线前还会更新一次到所有的在线角色)
int school_role_login(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	Player *pstTmpPlayer;
	ROLESCHOOL *pstRoleSchool = &pstPlayer->stRoleData.SyncRelation.School;

	//更新在线的师徒关系，未在线的当发生观察信息的时候在判断
	if(pstRoleSchool->JobType == ROLE_SCHOOL_TYPE_TEACHER)
	{
		ROLETEACHER *pstRoleTeacher = &pstRoleSchool->Data.RoleTeacher;

		for(i = 0; i < pstRoleTeacher->Num; i++)
		{
			pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstRoleTeacher->Students[i].RoleName);
			if(pstTmpPlayer)
			{
				school_student_info_update(&pstRoleTeacher->Students[i], pstTmpPlayer);
				school_role_online_action(pstEnv, pstPlayer, pstTmpPlayer, 1);
			}
			else
			{
				pstRoleTeacher->Students[i].DBFlag = 0;
			}
		}
	}
	else if(pstRoleSchool->JobType == ROLE_SCHOOL_TYPE_STUDENT)
	{
		ROLESTUDENT *pstRoleStudent = &pstRoleSchool->Data.RoleStudent;
		
		pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstRoleStudent->Teacher.RoleName);
		if(pstTmpPlayer)
		{
			school_teacher_info_update(&pstRoleStudent->Teacher, pstTmpPlayer);
			school_role_online_action(pstEnv, pstPlayer, pstTmpPlayer, 1);
		}
		else
		{
			pstRoleStudent->Teacher.DBFlag = 0;
		}
	}

	school_role_login_schoolval_check(pstEnv, pstPlayer);

	school_all_info_action(pstEnv, pstPlayer);
	return 0;
}

ROLESTUDENTINFO* school_teacher_get_stuinfo(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong ullRoleID)
{
	int i;
	ROLESCHOOL *pstSchool = &pstPlayer->stRoleData.SyncRelation.School;

	if(pstSchool->JobType != ROLE_SCHOOL_TYPE_TEACHER)
	{
		return NULL;
	}
	
	for(i=0; i<pstSchool->Data.RoleTeacher.Num; i++)
	{
		if(pstSchool->Data.RoleTeacher.Students[i].RoleID == ullRoleID)
		{
			return  &pstSchool->Data.RoleTeacher.Students[i];
		}
	}
	
	return NULL;
}

int school_del_mem(ROLESCHOOL *pstRoleSchool, tdr_ulonglong ullRoleID)
{
	int i ;
	
	if(pstRoleSchool->JobType == ROLE_SCHOOL_TYPE_TEACHER)
	{
		ROLETEACHER *pstRoleTeacher = &pstRoleSchool->Data.RoleTeacher;

		for(i = 0; i < pstRoleTeacher->Num; i++)
		{
			if(pstRoleTeacher->Students[i].RoleID == ullRoleID)
			{
				break;
			}
		}

		if(i >= pstRoleTeacher->Num)
		{
			return -1;
		}

		pstRoleTeacher->Num--;
		if(i != pstRoleTeacher->Num)
		{
			memmove(&pstRoleTeacher->Students[i], 
				&pstRoleTeacher->Students[i+1], sizeof(pstRoleTeacher->Students[0])*(pstRoleTeacher->Num-i));
		}
	}
	else if(pstRoleSchool->JobType == ROLE_SCHOOL_TYPE_STUDENT)
	{
		ROLESTUDENT *pstRoleStudent = &pstRoleSchool->Data.RoleStudent;

		if(pstRoleStudent->Teacher.RoleID != ullRoleID)
		{
			return -1;
		}

		memset(pstRoleStudent, 0, sizeof(*pstRoleStudent));
		pstRoleSchool->JobType = ROLE_SCHOOL_TYPE_NONE;
	}
	
	return 0;
}

int school_student_add_xinde(ROLESCHOOL *pstRoleSchool, int iExp, int iSchoolVal)
{
	if(pstRoleSchool->JobType != ROLE_SCHOOL_TYPE_STUDENT)
	{
		return -1;
	}

	pstRoleSchool->Data.RoleStudent.TotalCount++;
	pstRoleSchool->Data.RoleStudent.Exp += iExp*0.6;
	pstRoleSchool->Data.RoleStudent.Val += iSchoolVal;
	
	return 0;
}

int school_teacher_add_val(ROLESCHOOL *pstRoleSchool, tdr_ulonglong ullRoleID, int iTotalVal)
{
	int i ;
	ROLETEACHER *pstRoleTeacher = &pstRoleSchool->Data.RoleTeacher;

	
	if(pstRoleSchool->JobType != ROLE_SCHOOL_TYPE_TEACHER)
	{
		return -1;
	}
		
	for(i = 0; i < pstRoleTeacher->Num; i++)
	{
		if(pstRoleTeacher->Students[i].RoleID == ullRoleID)
		{
			pstRoleTeacher->Students[i].TotalVal = iTotalVal;
			break;
		}
	}

	return 0;
}

//出师导师奖励
int school_teacher_fini_award(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstStudent, SCHOOLLEVELDEF *pstSchoolLevelDef)
{
	int i, iGridNum = 0;
	char szMsg[1024];
	ROLEGRID astGrid[MAX_SCHOOL_LEVEL_AWARD];

	for(i=0; i<=MAX_SCHOOL_LEVEL_AWARD-1; i++)
	{
		if(pstSchoolLevelDef->FiniTeacherAward[i].ItemID == 0)
		{
			continue;
		}

		if(0 > item_create(pstEnv->pstAppCtx, pstEnv, &astGrid[iGridNum], pstSchoolLevelDef->FiniTeacherAward[i].ItemID))
		{
			continue;
		}

		if(pstSchoolLevelDef->FiniTeacherAward[i].Bind)
		{
			astGrid[iGridNum].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
		}

		if(astGrid[iGridNum].Type == GRID_ITEM_ITEM)
		{
			astGrid[iGridNum].GridData.RoleItem.Num = pstSchoolLevelDef->FiniTeacherAward[i].Num;
		}

		iGridNum++;
	}

	if(iGridNum <= 0)
	{
		return 0;
	}

	snprintf(szMsg, sizeof(szMsg), LJY_MAIL301, pstStudent->stRoleData.RoleName);
	
	player_sysmail_fast_send(pstEnv, pstPlayer->stRoleData.RoleName, szMsg, 
		LJY_MAIL302, astGrid, iGridNum, 0, NULL, 0);
	return 0;
}

//出师学生奖励
int school_student_fini_award(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstTeacher, SCHOOLLEVELDEF *pstSchoolLevelDef)
{
	int i, iGridNum = 0;
	char szMsg[1024];
	ROLEGRID astGrid[MAX_SCHOOL_LEVEL_AWARD];

	for(i=0; i<=MAX_SCHOOL_LEVEL_AWARD-1; i++)
	{
		if(pstSchoolLevelDef->FiniStudentAward[i].ItemID == 0)
		{
			continue;
		}

		if(0 > item_create(pstEnv->pstAppCtx, pstEnv, &astGrid[iGridNum], pstSchoolLevelDef->FiniStudentAward[i].ItemID))
		{
			continue;
		}

		if(pstSchoolLevelDef->FiniStudentAward[i].Bind)
		{
			astGrid[iGridNum].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
		}

		if(astGrid[iGridNum].Type == GRID_ITEM_ITEM)
		{
			astGrid[iGridNum].GridData.RoleItem.Num = pstSchoolLevelDef->FiniStudentAward[i].Num;
		}

		iGridNum++;
	}

	if(iGridNum <= 0)
	{
		return 0;
	}

	snprintf(szMsg, sizeof(szMsg), LJY_MAIL301, pstPlayer->stRoleData.RoleName);
	player_sysmail_fast_send(pstEnv, pstPlayer->stRoleData.RoleName, szMsg, 
		LJY_MAIL302, astGrid, iGridNum, 0, NULL, 0);
	return 0;
}

int school_student_levelup(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i, iGridNum = 0;
	char szTitle[256];
	char szMsg[1024];
	ROLEGRID astGrid[MAX_SCHOOL_LEVEL_AWARD];
	SCHOOLLEVELDEF *pstSchoolLevelDef;
	ROLESCHOOL *pstRoleSchool = &pstPlayer->stRoleData.SyncRelation.School;
	
	if(pstRoleSchool->JobType != ROLE_SCHOOL_TYPE_STUDENT)
	{
		return -1;
	}

	if(pstPlayer->stRoleData.Level -pstRoleSchool->Data.RoleStudent.Teacher.JoinLevel <= 10)
	{
		return -1;
	}
	
	pstSchoolLevelDef = z_find_school_level_def(pstEnv, pstPlayer->stRoleData.Level);
	if(!pstSchoolLevelDef)
	{
		return -1;
	}

	for(i=0; i<=MAX_SCHOOL_LEVEL_AWARD-1; i++)
	{
		if(pstSchoolLevelDef->LevelAward[i].ItemID == 0)
		{
			continue;
		}

		if(0 > item_create(pstEnv->pstAppCtx, pstEnv, &astGrid[iGridNum], pstSchoolLevelDef->LevelAward[i].ItemID))
		{
			continue;
		}

		if(pstSchoolLevelDef->LevelAward[i].Bind)
		{
			astGrid[iGridNum].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
		}

		if(astGrid[iGridNum].Type == GRID_ITEM_ITEM)
		{
			astGrid[iGridNum].GridData.RoleItem.Num = pstSchoolLevelDef->LevelAward[i].Num;
		}

		iGridNum++;
	}

	if(iGridNum <= 0)
	{
		return 0;
	}

	//snprintf(szTitle, sizeof(szTitle), 
	//	"恭喜，您的学生%s等级提升到%d", 
	//	pstPlayer->stRoleData.RoleName, pstPlayer->stRoleData.Level);

	snprintf(szTitle, sizeof(szTitle), LJY_MAIL303);
	snprintf(szMsg, sizeof(szMsg), 
		LJY_MAIL304, 
		pstPlayer->stRoleData.RoleName, pstPlayer->stRoleData.Level);
	
	player_sysmail_fast_send(pstEnv, pstRoleSchool->Data.RoleStudent.Teacher.RoleName, szTitle, 
		szMsg, astGrid, iGridNum, 0, NULL, 0);
	
	return 0;
}

int school_teachr_add_student(ZONESVRENV* pstEnv, Player *pstTeacher, Player *pstStudent)
{
	ROLESTUDENTINFO *pstStudentInfo;
	ROLESTUDENT *pstStudentData;
	ROLESCHOOL *pstStuSchool = &pstStudent->stRoleData.SyncRelation.School;
	ROLESCHOOL *pstTeacherSchool = &pstTeacher->stRoleData.SyncRelation.School;

	if(pstTeacherSchool->JobType != ROLE_SCHOOL_TYPE_TEACHER)
	{
		pstTeacherSchool->JobType = ROLE_SCHOOL_TYPE_TEACHER;
		pstTeacherSchool->Data.RoleTeacher.Num = 0;
	}

	pstStudentInfo = &pstTeacherSchool->Data.RoleTeacher.Students[pstTeacherSchool->Data.RoleTeacher.Num++];
	memset(pstStudentInfo, 0, sizeof(*pstStudentInfo));
	
	pstStudentInfo->RoleID = pstStudent->stRoleData.RoleID;
	STRNCPY(pstStudentInfo->RoleName, pstStudent->stRoleData.RoleName, CCH(pstStudentInfo->RoleName));
	pstStudentInfo->Level = pstStudent->stRoleData.Level;
	pstStudentInfo->Head = pstStudent->stRoleData.Head;
	pstStudentInfo->Hair = pstStudent->stRoleData.Hair;
	pstStudentInfo->Gender = pstStudent->stRoleData.Gender;
	pstStudentInfo->Career = pstStudent->stRoleData.Career;
	pstStudentInfo->JoinLevel = pstStudent->stRoleData.Level;
	pstStudentInfo->JoinTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstStudentInfo->DBFlag = 1;


	pstStuSchool->JobType = ROLE_SCHOOL_TYPE_STUDENT;
	pstStudentData = &pstStuSchool->Data.RoleStudent;
	memset(pstStudentData, 0, sizeof(*pstStudentData));

	pstStudentData->Teacher.RoleID = pstTeacher->stRoleData.RoleID;
	STRNCPY(pstStudentData->Teacher.RoleName, pstTeacher->stRoleData.RoleName, CCH(pstStudentData->Teacher.RoleName));
	pstStudentData->Teacher.Level = pstStudent->stRoleData.Level;
	pstStudentData->Teacher.Head = pstStudent->stRoleData.Head;
	pstStudentData->Teacher.Hair = pstStudent->stRoleData.Hair;
	pstStudentData->Teacher.Gender = pstStudent->stRoleData.Gender;
	pstStudentData->Teacher.Career = pstStudent->stRoleData.Career;
	pstStudentData->Teacher.JoinTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstStudentData->Teacher.JoinLevel = pstStudent->stRoleData.Level;
	pstStudentData->Teacher.DBFlag = 1;
	return 0;
}

int player_school_teacher_mem_info(ZONESVRENV* pstEnv, Player *pstPlayer, ROLETEACHERINFO *pstRoleTeacherInfo)
{
	CSPKG stPkg;
	CSSCHOOLSVR *pstCSSchoolSvr = &stPkg.Body.SchoolSvr;
	CSROLETEACHERINFO *pstCSTeacherInfo = &pstCSSchoolSvr->Data.MemInfo.Data.Teacher;

	pstCSSchoolSvr->Type = SCHOOL_SVR_MEM_INFO;
	pstCSSchoolSvr->Data.MemInfo.Type = SCHOOL_MEM_TYPE_TEACHER;
	school_teacher_info2cs(pstEnv, pstCSTeacherInfo, pstRoleTeacherInfo);
	
	Z_CSHEAD_INIT(&stPkg.Head, SCHOOL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		
	return 0;
}

int player_school_student_mem_info(ZONESVRENV* pstEnv, Player *pstPlayer, ROLESTUDENTINFO *pstRoleStudentInfo)
{
	CSPKG stPkg;
	CSSCHOOLSVR *pstCSSchoolSvr = &stPkg.Body.SchoolSvr;
	CSROLESTUDENTINFO *pstCSRoleStudent = &pstCSSchoolSvr->Data.MemInfo.Data.Student;
	
	pstCSSchoolSvr->Type = SCHOOL_SVR_MEM_INFO;
	pstCSSchoolSvr->Data.MemInfo.Type = SCHOOL_MEM_TYPE_STUDENT;
	school_student_info2cs(pstEnv, pstCSRoleStudent, pstRoleStudentInfo);
	
	Z_CSHEAD_INIT(&stPkg.Head, SCHOOL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
			
	return 0;
}

int player_school_mem_info_req(ZONESVRENV* pstEnv, Player *pstPlayer, CSSCHOOLMEMINFOREQ *pstReq)
{
	int i;
	Player *pstTmpPlayer;
	ROLESCHOOL *pstRoleSchool = &pstPlayer->stRoleData.SyncRelation.School;
	
	if(pstRoleSchool->JobType == ROLE_SCHOOL_TYPE_TEACHER)
	{
		ROLETEACHER *pstRoleTeacher = &pstRoleSchool->Data.RoleTeacher;

		for(i = 0; i < pstRoleTeacher->Num; i++)
		{
			if(0 != strcmp(pstRoleTeacher->Students[i].RoleName, pstReq->RoleName))
			{
				continue;
			}

			pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstRoleTeacher->Students[i].RoleName);
			if(pstTmpPlayer == NULL && pstRoleTeacher->Students[i].DBFlag == 0)
			{
				role_base_db_get(pstEnv, pstPlayer, pstRoleTeacher->Students[i].RoleID);
			}
			else
			{
				player_school_student_mem_info(pstEnv, pstPlayer, &pstRoleTeacher->Students[i]);
			}
			
			break;
		}
	}
	else if(pstRoleSchool->JobType == ROLE_SCHOOL_TYPE_STUDENT)
	{
		ROLESTUDENT *pstRoleStudent = &pstRoleSchool->Data.RoleStudent;

		if(0 != strcmp(pstRoleStudent->Teacher.RoleName, pstReq->RoleName))
		{
			return -1;
		}

		pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstRoleStudent->Teacher.RoleName);
		if(pstTmpPlayer == NULL && pstRoleStudent->Teacher.DBFlag == 0)
		{
			role_base_db_get(pstEnv, pstPlayer, pstRoleStudent->Teacher.RoleID);
		}
		else
		{
			player_school_teacher_mem_info(pstEnv, pstPlayer, &pstRoleStudent->Teacher);
		}
	}
	else
	{
		return 0;
	}
		
	return 0;
}

int player_school_invite(ZONESVRENV* pstEnv, Player *pstPlayer, CSSCHOOLINVITE *pstReq)
{
	RESPOS stPos;
	CSPKG stPkg;
	CSSCHOOLSVR *pstCSSchoolSvr = &stPkg.Body.SchoolSvr;
	CSSCHOOLJOINUI *pstCSJoinUI = &pstCSSchoolSvr->Data.JoinUI;
	Player *pstStudentPlayer;
	ROLESCHOOL *pstStuSchool;
	ROLESCHOOL *pstTeacherSchool = &pstPlayer->stRoleData.SyncRelation.School;

	if(pstPlayer->stRoleData.Level < 60)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SYNCRELATION301);
		return -1;
	}
	
	if(pstTeacherSchool->JobType == ROLE_SCHOOL_TYPE_STUDENT)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SYNCRELATION301);
		return -1;
	}
	else if(pstTeacherSchool->JobType == ROLE_SCHOOL_TYPE_TEACHER)
	{
		if(pstTeacherSchool->Data.RoleTeacher.Num >= MAX_ROLE_STUDENT)
		{
			z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SYNCRELATION302);
			return -1;
		}
	}

	pstStudentPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstReq->RoleName);
	if(!pstStudentPlayer)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SYNCRELATION303);
		return -1;
	}

	if(pstStudentPlayer->stRoleData.Level < 30 ||
		pstStudentPlayer->stRoleData.Level >= 60)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SYNCRELATION304);
		return -1;
	}

	stPos.X = 28900;
	stPos.Y = 26400;

	if(z_distance(&stPos, &pstStudentPlayer->stRoleData.Pos) > 1500)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SYNCRELATION305);
		return -1;
	}

	if(z_distance(&stPos, &pstPlayer->stRoleData.Pos) > 1500)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SYNCRELATION306);
		return -1;
	}

	pstStuSchool = &pstStudentPlayer->stRoleData.SyncRelation.School;
	if(pstStuSchool->JobType == ROLE_SCHOOL_TYPE_STUDENT)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SYNCRELATION307);
		return -1;
	}
	else if(pstStuSchool->JobType == ROLE_SCHOOL_TYPE_TEACHER)
	{
		if(pstTeacherSchool->Data.RoleTeacher.Num > 0)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SYNCRELATION307);
			return -1;
		}
	}

	pstCSSchoolSvr->Type = SCHOOL_SVR_JOIN_UI;
	STRNCPY(pstCSJoinUI->RoleName, pstPlayer->stRoleData.RoleName, CCH(pstCSJoinUI->RoleName));
	
	Z_CSHEAD_INIT(&stPkg.Head, SCHOOL_SVR);
	z_cltmsg_send(pstEnv, pstStudentPlayer, &stPkg);

	z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SYNCRELATION308);
	return 0;
}

int player_school_accept(ZONESVRENV* pstEnv, Player *pstPlayer, CSSCHOOLACCEPT *pstReq)
{
	RESPOS stPos;
	Player *pstTeacher;
	ROLESCHOOL *pstTeacherSchool;
	ROLESCHOOL *pstStuSchool = &pstPlayer->stRoleData.SyncRelation.School;
	CSPKG stPkg;
	CSSCHOOLSVR *pstCSSchoolSvr = &stPkg.Body.SchoolSvr;

	if(pstPlayer->stRoleData.Level < 30 || pstPlayer->stRoleData.Level > 60)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SYNCRELATION309);
		return -1;
	}
	
	if(pstStuSchool->JobType == ROLE_SCHOOL_TYPE_STUDENT)
	{
		return -1;
	}
	else if(pstStuSchool->JobType == ROLE_SCHOOL_TYPE_TEACHER)
	{
		if(pstStuSchool->Data.RoleTeacher.Num > 0)
		{
			return -1;
		}
	}
	
	pstTeacher = z_name_player(pstEnv->pstAppCtx, pstEnv, pstReq->RoleName);
	if(!pstTeacher)
	{
		return -1;
	}

	if(pstTeacher->stRoleData.Level < 60)
	{
		return -1;
	}

	pstTeacherSchool = &pstTeacher->stRoleData.SyncRelation.School;
	if(pstTeacherSchool->JobType == ROLE_SCHOOL_TYPE_STUDENT)
	{
		return -1;
	}
	else if(pstTeacherSchool->JobType == ROLE_SCHOOL_TYPE_TEACHER)
	{
		if(pstTeacherSchool->Data.RoleTeacher.Num >= MAX_ROLE_STUDENT)
		{
			return -1;
		}
	}

	stPos.X = 28900;
	stPos.Y = 26400;

	if(z_distance(&stPos, &pstTeacher->stRoleData.Pos) > 1500)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SYNCRELATION310);
		return -1;
	}

	if(z_distance(&stPos, &pstPlayer->stRoleData.Pos) > 1500)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SYNCRELATION306);
		return -1;
	}

	player_reg_delete(pstEnv, pstPlayer);
	player_reg_delete(pstEnv, pstTeacher);

	school_teachr_add_student(pstEnv, pstTeacher, pstPlayer);
	school_all_info_action(pstEnv, pstPlayer);
	school_all_info_action(pstEnv, pstTeacher);

	pstCSSchoolSvr->Type = SCHOOL_SVR_ADD_STUDENT_ACTION;
	pstCSSchoolSvr->Data.AddStudent.TeacherRoleID = pstTeacher->stRoleData.RoleID;
	STRNCPY(pstCSSchoolSvr->Data.AddStudent.TeacherRoleName, 
		pstTeacher->stRoleData.RoleName, CCH(pstCSSchoolSvr->Data.AddStudent.TeacherRoleName));
	
	pstCSSchoolSvr->Data.AddStudent.RoleID = pstPlayer->stRoleData.RoleID;
	STRNCPY(pstCSSchoolSvr->Data.AddStudent.RoleName, 
		pstPlayer->stRoleData.RoleName, CCH(pstCSSchoolSvr->Data.AddStudent.RoleName));

	Z_CSHEAD_INIT(&stPkg.Head, SCHOOL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	z_cltmsg_send(pstEnv, pstTeacher, &stPkg);
	
	return 0;
}

int player_school_del_mail(ZONESVRENV* pstEnv, Player *pstPlayer, CSSCHOOLDEL *pstReq)
{
	char szTmp[128];
	char szMsg[512];
	time_t now;
	struct tm *pstnow;
	ROLESCHOOL *pstSchool = &pstPlayer->stRoleData.SyncRelation.School;

	now = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstnow = localtime(&now);
	strftime(szTmp, sizeof(szTmp), "%Y-%m-%d %H:%M:%S", pstnow);

	if(pstSchool->JobType == ROLE_SCHOOL_TYPE_TEACHER)
	{
		snprintf(szMsg, sizeof(szMsg), 
			LJY_MAIL305, 
			pstReq->RoleName, szTmp);
		player_sysmail_fast_send(pstEnv, pstPlayer->stRoleData.RoleName, LJY_MAIL306, szMsg, NULL, 0, 0, NULL, 0);

		snprintf(szMsg, sizeof(szMsg), 
			LJY_MAIL307, 
			pstPlayer->stRoleData.RoleName, szTmp);
		player_sysmail_fast_send(pstEnv, pstReq->RoleName, LJY_MAIL306, szMsg, NULL, 0, 0, NULL, 0);
	}
	else
	{
		snprintf(szMsg, sizeof(szMsg), 
			LJY_MAIL307, 
			pstReq->RoleName, szTmp);
		player_sysmail_fast_send(pstEnv, pstPlayer->stRoleData.RoleName, LJY_MAIL306, szMsg, NULL, 0, 0, NULL, 0);

		snprintf(szMsg, sizeof(szMsg), 
			LJY_MAIL305, 
			pstPlayer->stRoleData.RoleName, szTmp);
		player_sysmail_fast_send(pstEnv, pstReq->RoleName, LJY_MAIL306, szMsg, NULL, 0, 0, NULL, 0);
	}	

	return 0;
}

int player_school_reject(ZONESVRENV* pstEnv, Player *pstPlayer, CSSCHOOLREJECT *pstReq)
{
	Player *pstTmpPlayer;

	pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstReq->RoleName);
	if(!pstTmpPlayer)
	{
		return 0;
	}

	if(pstReq->Flag)
	{
		//超时提示
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstTmpPlayer, SYS_MIDDLE2, ZONEERR_SYNCRELATION311);
	}
	else
	{
		 //拒绝提示
		 z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstTmpPlayer, SYS_MIDDLE2, ZONEERR_SYNCRELATION312, pstPlayer->stRoleData.RoleName);
	}
	
	return 0;
}

int player_school_del(ZONESVRENV* pstEnv, Player *pstPlayer, CSSCHOOLDEL *pstReq)
{
	//char szMsg[256];
	Player *pstTmpPlayer;
	//ROLESCHOOL *pstSchool = &pstPlayer->stRoleData.SyncRelation.School;

	if(pstPlayer->stRoleData.RoleDetail.GreenJade < 500)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SYNCRELATION313);
		return -1;
	}
	
	if(0 > school_del_mem(&pstPlayer->stRoleData.SyncRelation.School, pstReq->RoleID))
	{
		return -1;
	}
	
	school_all_info_action(pstEnv, pstPlayer);
	player_add_vig(pstEnv, pstPlayer, pstPlayer->stRoleData.RoleDetail.GreenJade, 1, 1);

	player_school_del_mail(pstEnv, pstPlayer, pstReq);

	pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstReq->RoleName);
	if(pstTmpPlayer)
	{
		school_del_mem(&pstTmpPlayer->stRoleData.SyncRelation.School, pstPlayer->stRoleData.RoleID);
		school_all_info_action(pstEnv, pstTmpPlayer);
	}
	else
	{
		DBAsyn stDBAsyn;

		stDBAsyn.AsynCmd.stSyncRelationSelect.Uin = pstPlayer->stRoleData.Uin;
		stDBAsyn.AsynCmd.stSyncRelationSelect.ullRoleID = pstPlayer->stRoleData.RoleID;
		stDBAsyn.AsynCmd.stSyncRelationSelect.iType =  2;

		role_sync_relation_select_req(pstEnv, pstPlayer, pstReq->RoleID, &stDBAsyn);
	}
	
	return 0;
}

int player_school_xinde(ZONESVRENV* pstEnv, Player *pstPlayer, CSSCHOOLXINDE *pstReq)
{
	int i;
	Player *pstStudent;
	ROLESTUDENTINFO *pstStudentInfo;
	SCHOOLXINDEDEF *pstSchoolXinDeDef;
	ROLESCHOOL *pstSchool = &pstPlayer->stRoleData.SyncRelation.School;

	if(pstSchool->JobType != ROLE_SCHOOL_TYPE_TEACHER)
	{
		return -1;
	}

	pstStudentInfo = NULL;
	for(i=0; i<pstSchool->Data.RoleTeacher.Num; i++)
	{
		if(pstSchool->Data.RoleTeacher.Students[i].RoleID == pstReq->RoleID)
		{
			pstStudentInfo = &pstSchool->Data.RoleTeacher.Students[i];
			break;
		}
	}

	if(pstStudentInfo == NULL)
	{
		return -1;
	}

	if(IsSameDay(pstStudentInfo->DayTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{	
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SYNCRELATION314);
		return -1;
	}

	pstSchoolXinDeDef = z_find_school_xinde_def(pstEnv, pstPlayer->stRoleData.Level);
	if(!pstSchoolXinDeDef)
	{
		return -1;
	}

	if(pstPlayer->stRoleData.RoleDetail.Exp < pstSchoolXinDeDef->AddExp)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SYNCRELATION315);
		return -1;
	}

	pstStudentInfo->DayTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	player_dec_exp(pstEnv, pstPlayer, pstSchoolXinDeDef->AddExp);
	school_all_info_action(pstEnv, pstPlayer);

	pstStudent = z_name_player(pstEnv->pstAppCtx, pstEnv, pstStudentInfo->RoleName);
	if(!pstStudent)
	{
		DBAsyn stDBAsyn;

		stDBAsyn.iCmd = DB_ROLE_SYNC_RELATION_SELECT;

		stDBAsyn.AsynCmd.stSyncRelationSelect.Uin = pstPlayer->stRoleData.Uin;
		stDBAsyn.AsynCmd.stSyncRelationSelect.ullRoleID = pstPlayer->stRoleData.RoleID;
		stDBAsyn.AsynCmd.stSyncRelationSelect.iType = 1;
		stDBAsyn.AsynCmd.stSyncRelationSelect.stSelectInfo.stStudentAdd.iExp = pstSchoolXinDeDef->AddExp;
		stDBAsyn.AsynCmd.stSyncRelationSelect.stSelectInfo.stStudentAdd.iVal =  pstSchoolXinDeDef->SchoolVal;
			
		role_sync_relation_select_req(pstEnv, pstPlayer, pstStudentInfo->RoleID, &stDBAsyn);
	}
	else
	{
		school_student_add_xinde(&pstStudent->stRoleData.SyncRelation.School, 
			pstSchoolXinDeDef->AddExp, pstSchoolXinDeDef->SchoolVal);
		
		school_all_info_action(pstEnv, pstStudent);
	}

	z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
		ZONEERR_SYNCRELATION316);
	
	return 0;
}

int player_school_award_xinde(ZONESVRENV* pstEnv, Player *pstPlayer, CSSCHOOLAWARDXINDE *pstReq)
{
	Player *pstTeacher;
	ROLESCHOOL *pstSchool = &pstPlayer->stRoleData.SyncRelation.School;
	
	if(pstSchool->JobType != ROLE_SCHOOL_TYPE_STUDENT)
	{
		return -1;
	}

	if(pstSchool->Data.RoleStudent.Teacher.RoleID != pstReq->RoleID)
	{
		return -1;
	}

	if(pstSchool->Data.RoleStudent.Exp <= 0)
	{
		return -1;
	}

	if(0 > player_school_daylimit_check(pstEnv, pstPlayer))
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, 
				ZONEERR_SYNCRELATION317);
		return -1;
	}
	
	pworld_player_add_exp(pstEnv, pstPlayer, pstSchool->Data.RoleStudent.Exp);

	player_school_daylimit_add(pstEnv, pstPlayer);

	pstSchool->Data.RoleStudent.TotalVal += pstSchool->Data.RoleStudent.Val;
	pstSchool->Data.RoleStudent.Exp = 0;
	pstSchool->Data.RoleStudent.Val = 0;
	pstSchool->Data.RoleStudent.AwardCount++;
	school_all_info_action(pstEnv, pstPlayer);
	
	pstTeacher = z_name_player(pstEnv->pstAppCtx, pstEnv, pstSchool->Data.RoleStudent.Teacher.RoleName);
	if(!pstTeacher)
	{
		DBAsyn stDBAsyn;

		stDBAsyn.iCmd = DB_ROLE_SYNC_RELATION_SELECT;

		stDBAsyn.AsynCmd.stSyncRelationSelect.Uin = pstPlayer->stRoleData.Uin;
		stDBAsyn.AsynCmd.stSyncRelationSelect.ullRoleID = pstPlayer->stRoleData.RoleID;
		stDBAsyn.AsynCmd.stSyncRelationSelect.iType = 0;
		stDBAsyn.AsynCmd.stSyncRelationSelect.stSelectInfo.stTeacherAdd.iTotalVal = pstSchool->Data.RoleStudent.TotalVal;
			
		role_sync_relation_select_req(pstEnv, pstPlayer, pstSchool->Data.RoleStudent.Teacher.RoleID, &stDBAsyn);
	}
	else
	{
		school_teacher_add_val(&pstTeacher->stRoleData.SyncRelation.School, 
			pstPlayer->stRoleData.RoleID, pstSchool->Data.RoleStudent.TotalVal);

		school_role_login_schoolval_check(pstEnv, pstTeacher);
	}
	
	return 0;
}

int player_school_fini(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	RESPOS stPos;
	Player *pstTeacher;
	SCHOOLLEVELDEF *pstSchoolLevelDef;
	ROLESCHOOL *pstSchool = &pstPlayer->stRoleData.SyncRelation.School;
	FINISCHOOLINFO *pstFini;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;

	if(pstSchool->JobType != ROLE_SCHOOL_TYPE_STUDENT)
	{
		return -1;
	}

	if(pstPlayer->stRoleData.Level < 65)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, 	ZONEERR_SYNCRELATION318);
		return -1;
	}

	pstTeacher = z_name_player(pstEnv->pstAppCtx, pstEnv, pstSchool->Data.RoleStudent.Teacher.RoleName);
	if(!pstTeacher)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, 	
								ZONEERR_SYNCRELATION322);
		return -1;
	}

	pstFini = &pstTeacher->stRoleData.MiscInfo.FiniSchoolInfo;
	if (!IsSameWeek(iCurr, pstFini->Time))
	{
		pstFini->Time = iCurr;
		pstFini->Num = 0;
	}

	if (pstFini->Num >= MAX_FINI_SCHOOL)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, 	
								ZONEERR_SYNCRELATION321, MAX_FINI_SCHOOL);
		return -1;	
	}

	//距离检查
	stPos.X = 28900;
	stPos.Y = 26400;

	if(z_distance(&stPos, &pstTeacher->stRoleData.Pos) > 1500)
	{
		z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 	ZONEERR_SYNCRELATION319);
		return -1;
	}

	if(z_distance(&stPos, &pstPlayer->stRoleData.Pos) > 1500)
	{
		return -1;
	}
	
	pstSchoolLevelDef = z_find_school_level_def(pstEnv, pstPlayer->stRoleData.Level);
	if(!pstSchoolLevelDef)
	{
		return -1;
	}

	if(0 > school_del_mem(&pstPlayer->stRoleData.SyncRelation.School, pstSchool->Data.RoleStudent.Teacher.RoleID))
	{
		return -1;
	}

	pstFini->Num ++;
	
	school_all_info_action(pstEnv, pstPlayer);

	school_del_mem(&pstTeacher->stRoleData.SyncRelation.School, pstPlayer->stRoleData.RoleID);
	school_all_info_action(pstEnv, pstTeacher);

	school_teacher_fini_award(pstEnv, pstTeacher, pstPlayer, pstSchoolLevelDef);
	school_student_fini_award(pstEnv, pstPlayer, pstTeacher, pstSchoolLevelDef);
	return 0;
}

int player_school_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSSCHOOLCLT *pstSchoolClt = &pstCsPkg->Body.SchoolClt;

	if (pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
		return -1;
	}
	
	switch(pstSchoolClt->Type)
	{
		case SCHOOL_CLT_INVITE:
			return player_school_invite(pstEnv, pstPlayer, &pstSchoolClt->Data.Invite);
			break;
		case SCHOOL_CLT_ACCEPT:
			return player_school_accept(pstEnv, pstPlayer, &pstSchoolClt->Data.Accept);
			break;
		case SCHOOL_CLT_MEM_INFO_REQ:
			return player_school_mem_info_req(pstEnv, pstPlayer, &pstSchoolClt->Data.MemInfoReq);
			break;
		case SCHOOL_CLT_REJECT:
			return player_school_reject(pstEnv, pstPlayer, &pstSchoolClt->Data.Reject);
			break;
		case SCHOOL_CLT_DEL:
			return player_school_del(pstEnv, pstPlayer, &pstSchoolClt->Data.Del);
			break;
		case SCHOOL_CLT_XINDE:
			return player_school_xinde(pstEnv, pstPlayer, &pstSchoolClt->Data.XinDe);
			break;
		case SCHOOL_CLT_AWARD_XINDE:
			return player_school_award_xinde(pstEnv, pstPlayer, &pstSchoolClt->Data.AwardXinDe);
			break;
		default:
			break;
	}

	UNUSED(pstAppCtx);
	UNUSED(pstFrameHead);
	return 0;
}

int is_valid_pworld_teacher(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	ZoneTeam *pstTeam;
	Player *pstTmpPlayer;
	ROLESTUDENTINFO *pstRoleStudentInfo;
	ROLESCHOOL *pstSchool = &pstPlayer->stRoleData.SyncRelation.School;
	ZONEPWORLDINST *pstPInst;// = z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);

	if(pstSchool->JobType != ROLE_SCHOOL_TYPE_TEACHER)
	{
		return 0;
	}

	pstPInst = z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);
	if (!pstPInst)
	{
		return 0;
	}

	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(!pstTeam)
	{
		return 0;
	}

	for(i = 0; i<pstTeam->stTeamInfo.MemberNum; i++)
	{
		if(pstTeam->stTeamInfo.TeamMember[i].RoleID == pstPlayer->stRoleData.RoleID)
		{
			continue;
		}

		pstRoleStudentInfo = school_teacher_get_stuinfo(pstEnv, pstPlayer, pstTeam->stTeamInfo.TeamMember[i].RoleID);
		if(!pstRoleStudentInfo)
		{
			continue;
		}

		pstTmpPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
		if(!pstTmpPlayer)
		{
			continue;
		}

		if (pstPInst == z_find_pworldinst2(pstEnv, pstTmpPlayer->stRoleData.Map))
		{
			return 1;
		}

		/*if(pstTmpPlayer->stRoleData.Map == pstPlayer->stRoleData.Map)
		{
			return 1;
		}*/
	}

	return 0;
}

int is_school_student(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLESCHOOL *pstSchool = &pstPlayer->stRoleData.SyncRelation.School;
	
	if(pstSchool->JobType != ROLE_SCHOOL_TYPE_STUDENT)
	{
		return 0;
	}
	
	return 1;
}

int player_chuancheng_notify(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstRes = &stPkg.Body.PlayerMiscSvr;
	CHUANCHENGPOINT *pstPoint = &pstRes->PlayerMiscSvrData.ChuanChengPoint;
	pstRes->PlayerMiscType = CHUAN_CHENG_CHG;

	pstPoint->Point = pstPlayer->stRoleData.MiscInfo.SchoolInfo.DaySchoolVal;
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_add_school_val(ZONESVRENV* pstEnv, Player *pstPlayer, int iAddVal)
{
	ROLESCHOOLINFO *pstDayInfo = &pstPlayer->stRoleData.MiscInfo.SchoolInfo;

	if(!IsSameDay(pstEnv->pstAppCtx->stCurr.tv_sec, pstDayInfo->DayTime))
	{
		pstDayInfo->DayTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstDayInfo->DaySchoolVal = 0;
		player_chuancheng_notify(pstEnv, pstPlayer);
	}

	if(pstDayInfo->DaySchoolVal + iAddVal > MAX_DAY_SCHOOL_VAL)
	{
		iAddVal = MAX_DAY_SCHOOL_VAL - pstDayInfo->DaySchoolVal;
	}

	package_add_money(pstEnv, pstPlayer, ATTR_ID_SCHOOL_VAL, iAddVal);
	pstDayInfo->DaySchoolVal += iAddVal;
	return 0;
}

int player_pworld_award_schoolval(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	SCHOOLPWORLDDEF *pstDef;
	
	if(pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
	{
		return -1;
	}

	pstDef = z_find_school_pworld_def(pstEnv, GET_PWORLDID(pstPlayer->stRoleData.Map));
	if(!pstDef)
	{
		return -1;
	}

	if(!is_valid_pworld_teacher(pstEnv, pstPlayer))
	{
		return -1;
	}

	player_add_school_val(pstEnv, pstPlayer, pstDef->SchoolVal);
	return 0;
}

int player_open_school_invite_ui(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSSCHOOLSVR *pstCSSchoolSvr = &stPkg.Body.SchoolSvr;
	ROLESCHOOL *pstTeacherSchool = &pstPlayer->stRoleData.SyncRelation.School;

	if(pstPlayer->stRoleData.Level < 60)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SYNCRELATION320);
		return -1;
	}
	
	if(pstTeacherSchool->JobType == ROLE_SCHOOL_TYPE_STUDENT)
	{
		z_send_sysmsg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SYNCRELATION320);
		return -1;
	}
	else if(pstTeacherSchool->JobType == ROLE_SCHOOL_TYPE_TEACHER)
	{
		if(pstTeacherSchool->Data.RoleTeacher.Num >= MAX_ROLE_STUDENT)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SYNCRELATION302);
			return -1;
		}
	}

	pstCSSchoolSvr->Type = SCHOOL_SVR_INVITE_UI;
	Z_CSHEAD_INIT(&stPkg.Head, SCHOOL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_school_daylimit_update(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLESCHOOLLIMIT *pstSchoolLimit = &pstPlayer->stRoleData.SyncRelation.SchoolLimit;
	
	if(!IsSameDay(pstSchoolLimit->DayTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		pstSchoolLimit->DayTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstSchoolLimit->AwardNum = 0;
	}
	
	return 0;
}

int player_school_daylimit_add(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLESCHOOLLIMIT *pstSchoolLimit = &pstPlayer->stRoleData.SyncRelation.SchoolLimit;
	
	player_school_daylimit_update(pstEnv, pstPlayer);

	pstSchoolLimit->AwardNum++;
	return 0;
}

int player_school_daylimit_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLESCHOOLLIMIT *pstSchoolLimit = &pstPlayer->stRoleData.SyncRelation.SchoolLimit;

	player_school_daylimit_update(pstEnv, pstPlayer);

	if(pstSchoolLimit->AwardNum >= SCHOOL_DAY_AWARD_XINDE_NUM)
	{
		return -1;
	}
	
	return 0;
}

static int gender_asc_cmp(const void *p1, const void*p2)
{
	Player **pstPlayer1 = (Player **)p1;
	Player **pstPlayer2 =  (Player **)p2;

	if((*pstPlayer1) && (*pstPlayer2))
	{
		if((*pstPlayer1)->stRoleData.Gender > (*pstPlayer2)->stRoleData.Gender)
		{
			return 1;
		}
		else if((*pstPlayer1)->stRoleData.Gender < (*pstPlayer2)->stRoleData.Gender)
		{
			return -1;
		}
	}
	
	return 0;
}

static int gender_des_cmp(const void *p1, const void*p2)
{
	Player **pstPlayer1 = (Player **)p1;
	Player **pstPlayer2 =  (Player **)p2;

	if((*pstPlayer1) && (*pstPlayer2))
	{
		if((*pstPlayer2)->stRoleData.Gender > (*pstPlayer1)->stRoleData.Gender)
		{
			return 1;
		}
		else if((*pstPlayer2)->stRoleData.Gender < (*pstPlayer1)->stRoleData.Gender)
		{
			return -1;
		}
	}
	
	return 0;
}

//新人栏信息请求
int player_new_people_column_clt(ZONESVRENV *pstEnv,Player* pstPlayer,CSNEWPEOPLECLT *pReq)
{
	all_new_people(pstEnv,1,pstPlayer);
	return 0;
	UNUSED(pReq);
}

//type 等于1表示随机返还新人列表给客户端，其余的为返回总新人数
int all_new_people(ZONESVRENV* pstEnv,int type,Player* pstPlayer)
{
	int i = 0;
	int iCount = 0;
	Player *apstPlayers[MAX_PLAYER];
	TMEMBLOCK* pstMemBlock = NULL;
	Player * pstElement = NULL;
	ROLESCHOOL *pstSchool = NULL;
	
	for (i=0; i<MAX_PLAYER; i++)
	{
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstElement = (Player *)pstMemBlock->szData;		
		if(PLAYER_STATUS_ROLE_LOGIN != pstElement->eStatus)
		{
			continue;
		}

		if(pstElement->stRoleData.Level < 30 || pstElement->stRoleData.Level > 59)
		{
			continue;
		}

		pstSchool = &pstElement->stRoleData.SyncRelation.School;
		if(pstSchool->JobType != ROLE_SCHOOL_TYPE_NONE)
		{
			continue;
		}
		
		apstPlayers[iCount++] = pstElement;	
		
	}

	if(type == 1)
	{
		player_obtian_new_people_to_clt(pstEnv, apstPlayers,pstPlayer,iCount);
	}
	
	return iCount;
}

//异性优先
int player_obtian_new_people_to_clt(ZONESVRENV* pstEnv, Player **apstPlayers, Player *pstPlayer,int iCount)
{
	if(iCount <= 0)
	{
		return 0;
	}

	/*
	int i = 0;
	int iCountNum = iCount;

	//新人栏不要看到自己
	for(i=iCountNum-1;i>=0;i--)
	{
		if(apstPlayers[i]->stRoleData.RoleID == pstPlayer->stRoleData.RoleID)
		{
			iCount--;
			if(i != iCount)
			{
				apstPlayers[i] = apstPlayers[iCount];
			}
			break;
		}
	}
	*/
	
	if(pstPlayer->stRoleData.Gender == GENDER_MAN)
	{
		//男前女后
		qsort(&apstPlayers[0], iCount, sizeof(apstPlayers[0]), gender_asc_cmp);
	}
	else if(pstPlayer->stRoleData.Gender == GENDER_WOMAN)
	{
		//女前男后
		qsort(&apstPlayers[0], iCount, sizeof(apstPlayers[0]), gender_des_cmp);
	}
	else
	{
		return 0;
	}
	
	int j = 0;
	int iDiffIdx = 0;  // 男女性别分割点下标
	int k = 0;
	int iNum = 0;
	int iRand = RAND1(iCount);
	int iTempRand = iRand;
	int iRound = 0;
	int ifristScan = 0; // 开始扫描的下标
	CSNEWPEOPLEINFO stNewPeopleInfo;
	memset(&stNewPeopleInfo,0,sizeof(stNewPeopleInfo));

	//先找到男女性别分割点
	for(j=0;j<iCount-1;j++)
	{
		if(apstPlayers[j]->stRoleData.Gender != apstPlayers[j+1]->stRoleData.Gender)
		{
			iDiffIdx = j+1;
			break;
		}
	}
	
	if(iDiffIdx >= iRand)
	{
		//直接从分割点开始
		iTempRand = iDiffIdx;
	}

	ifristScan = iTempRand;

	for(k=iTempRand;k<iCount;k++)
	{
		iNum = stNewPeopleInfo.Num;
		
		if(iNum >= MAX_NEW_PEOPLE_NUM)
		{
			break;
		}

		if(iRound == 1)
		{
			if(k >= ifristScan)
			{
				//异性都扫过了，扫同性的
				k=-1;
				iRound++;
				continue;
			}
		}

		if(iRound == 2)
		{
			if(k >= iDiffIdx)
			{
				//全扫遍了
				break;
			}
		}
		
		STRNCPY(stNewPeopleInfo.PeopleInfo[iNum].RoleName,apstPlayers[k]->stRoleData.RoleName,sizeof(stNewPeopleInfo.PeopleInfo[iNum].RoleName));
		stNewPeopleInfo.PeopleInfo[iNum].Gender = apstPlayers[k]->stRoleData.Gender;
		stNewPeopleInfo.PeopleInfo[iNum].Level = apstPlayers[k]->stRoleData.Level;
		stNewPeopleInfo.Num++;		
		
		if(k == iCount-1)
		{
			//再扫一下异性的
			k = iDiffIdx-1;

			if(iRound == 0)
			{
				iRound++;
			}
		}
	}

	if(stNewPeopleInfo.Num > 0)
	{
		player_new_people_column_svr(pstEnv, pstPlayer,&stNewPeopleInfo);
	}

	return 0;
}

//新人栏的信息
int player_new_people_column_svr(ZONESVRENV* pstEnv, Player *pstPlayer,CSNEWPEOPLEINFO *pstNewPeopleInfo)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	CSNEWPEOPLEINFO *pstNewPeopleInfoSvr = &pstMiscSvr->PlayerMiscSvrData.NewPeopleInfo;
	
	pstMiscSvr->PlayerMiscType = PLAYER_NEW_PEOPLE_INFO;
	memcpy(pstNewPeopleInfoSvr,pstNewPeopleInfo,sizeof(*pstNewPeopleInfoSvr));

	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

//新人定时提示
int new_people_tick(ZONESVRENV* pstEnv)
{
	ZoneObj *pstZoneObj = pstEnv->pstZoneObj;
	int iCount = 0;
	if (!pstZoneObj)
	{
		return -1;
	}

	if(pstZoneObj->NewPeoRefTime/7200 != pstEnv->pstAppCtx->stCurr.tv_sec/7200)
	{
		pstZoneObj->NewPeoRefTime = pstEnv->pstAppCtx->stCurr.tv_sec;
		iCount = all_new_people(pstEnv,0,NULL);

		if(iCount > 0)
		{
			char szText[512] = {0};
			snprintf(szText, sizeof(szText),LJY_SYSMSG28,iCount);
			shopping_add_bulletin(pstEnv->pstAppCtx,pstEnv,20,10, szText);
		}
	}
	
	return 0;
}

int school_check_data(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i = 0;
	TMEMBLOCK* pstMemBlock = NULL;
	Player * pstElement = NULL;
	ROLESCHOOL *pstSchool = NULL;
	ROLESCHOOL *pstSchool2 = NULL;
	int flag = 0;
	int j = 0;

	pstSchool2 = &pstPlayer->stRoleData.SyncRelation.School;
	if(pstSchool2->JobType == ROLE_SCHOOL_TYPE_TEACHER)
	{
		flag = 1;
	}
	else if(pstSchool2->JobType == ROLE_SCHOOL_TYPE_STUDENT)
	{
		flag = 2;
	}
	else
	{
		return 0;
	}

	for (i=0; i<MAX_PLAYER; i++)
	{
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstElement = (Player *)pstMemBlock->szData; 	
		if(PLAYER_STATUS_ROLE_LOGIN != pstElement->eStatus || pstElement == pstPlayer)
		{
			continue;
		}

		pstSchool = &pstElement->stRoleData.SyncRelation.School;

		if(flag == 1)
		{
			if(pstSchool->JobType == ROLE_SCHOOL_TYPE_STUDENT)
			{
				//正向
				for(j=0;j<pstSchool2->Data.RoleTeacher.Num;j++)
				{
					if(pstSchool2->Data.RoleTeacher.Students[j].RoleID == pstElement->stRoleData.RoleID)
					{
						if(0 != strcmp(pstSchool2->Data.RoleTeacher.Students[j].RoleName,pstElement->stRoleData.RoleName))
						{
							STRNCPY(pstSchool2->Data.RoleTeacher.Students[j].RoleName,pstElement->stRoleData.RoleName, CCH(pstSchool2->Data.RoleTeacher.Students[j].RoleName));
							break;
						}
					}
				}

				//反向
				if(pstSchool->Data.RoleStudent.Teacher.RoleID == pstPlayer->stRoleData.RoleID)
				{
					if(0 != strcmp(pstSchool->Data.RoleStudent.Teacher.RoleName,pstPlayer->stRoleData.RoleName))
					{
						chg_role_name_notify_player(pstEnv, pstElement,pstPlayer->stRoleData.RoleID, pstPlayer->stRoleData.RoleName, pstSchool->Data.RoleStudent.Teacher.RoleName);
						STRNCPY(pstSchool->Data.RoleStudent.Teacher.RoleName,pstPlayer->stRoleData.RoleName, CCH(pstSchool->Data.RoleStudent.Teacher.RoleName));
					}
				}
			}
		}
		else 
		{
			if(pstSchool->JobType == ROLE_SCHOOL_TYPE_TEACHER)
			{
				//正向
				if(pstSchool2->Data.RoleStudent.Teacher.RoleID == pstElement->stRoleData.RoleID)
				{
					if(0 != strcmp(pstSchool2->Data.RoleStudent.Teacher.RoleName,pstElement->stRoleData.RoleName))
					{
						STRNCPY(pstSchool2->Data.RoleStudent.Teacher.RoleName,pstElement->stRoleData.RoleName, CCH(pstSchool2->Data.RoleStudent.Teacher.RoleName));
					}
				}

				//反向
				for(j=0;j<pstSchool->Data.RoleTeacher.Num;j++)
				{
					if(pstSchool->Data.RoleTeacher.Students[j].RoleID == pstPlayer->stRoleData.RoleID)
					{
						if(0 != strcmp(pstSchool->Data.RoleTeacher.Students[j].RoleName,pstPlayer->stRoleData.RoleName))
						{
							chg_role_name_notify_player(pstEnv, pstElement,pstPlayer->stRoleData.RoleID, pstPlayer->stRoleData.RoleName, pstSchool->Data.RoleTeacher.Students[j].RoleName);
							STRNCPY(pstSchool->Data.RoleTeacher.Students[j].RoleName,pstPlayer->stRoleData.RoleName, CCH(pstSchool->Data.RoleTeacher.Students[j].RoleName));
							break;
						}
					}
				}
			}
		}	
	}
	
	return 0;
}

int chg_role_name_notify_player(ZONESVRENV* pstEnv, Player *pstPlayer,tdr_ulonglong ullRoleID, const char* pszNew, const char* pszOld)
{
	CSPKG stPkg;
	CSCHGNAMESVR *pstChgNameSvr = &stPkg.Body.ChgNameSvr;

	pstChgNameSvr->Flag = CHG_ROLE_NAME;
	pstChgNameSvr->WID = ullRoleID;
	pstChgNameSvr->ID = 0;
	STRNCPY(pstChgNameSvr->NewName, pszNew, sizeof(pstChgNameSvr->NewName));
	STRNCPY(pstChgNameSvr->OldName, pszOld, sizeof(pstChgNameSvr->OldName));
	
	Z_CSHEAD_INIT(&stPkg.Head, CHGNAME_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

