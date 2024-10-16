


#include "zone_clanboon.h"
#include "zone_clan.h"
#include "zone_clt.h"
#include "zone_package.h"
#include "zone_oplog.h"
#include "zone_err.h"

// 福利条件变化，即时通知
int clan_boon_condition_chg(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan)
{
	CSPKG stPkg;
	CSBOONSVR *pstBoonSvr = &stPkg.Body.BoonSvr;
	
	pstBoonSvr->Cmd = BOON_SVR_OP_CHG;
	pstBoonSvr->Data.Chg.Cond = pstZoneClan->stClan.ClanBoon.Cond;
	
	Z_CSHEAD_INIT(&stPkg.Head, BOON_SVR);
	player_clan_broadcast(pstEnv, pstZoneClan, &stPkg, 0);
	return 0;
}

// 领取和分配变化
int clan_boon_assign_chg(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan)
{
	CSPKG stPkg;
	CSBOONSVR *pstBoonSvr = &stPkg.Body.BoonSvr;
	
	pstBoonSvr->Cmd = BOON_SVR_OP_ASSIGN;
	pstBoonSvr->Data.Assign.Assign = pstZoneClan->stClan.ClanBoon.Assign;
	
	Z_CSHEAD_INIT(&stPkg.Head, BOON_SVR);
	player_clan_broadcast(pstEnv, pstZoneClan, &stPkg, 0);
	return 0;
}

// 通知全部信息
int clan_boon_all_notify(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, Player *pstPlayer)
{
	CSPKG stPkg;
	CSBOONSVR *pstBoonSvr = &stPkg.Body.BoonSvr;
	
	pstBoonSvr->Cmd = BOON_SVR_OP_ALL;
	pstBoonSvr->Data.All.ClanBoon = pstZoneClan->stClan.ClanBoon;
	
	Z_CSHEAD_INIT(&stPkg.Head, BOON_SVR);
	if (pstPlayer)
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	else
		player_clan_broadcast(pstEnv, pstZoneClan, &stPkg, 0);
	return 0;
}

// 角色上线
int clan_boon_player_login(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	ZoneClan *pstClan = z_id_clan(pstEnv, pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id);
	if (pstClan)
	{
		clan_boon_all_notify(pstEnv, pstClan, pstPlayer);	
	}
	return 0;
}

// 公会12:00-24:00采集在线人数
int clan_boon_get_online(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan)
{
	CLANBOON *pstClanBoon = &pstZoneClan->stClan.ClanBoon;
	time_t tCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	struct tm stNow;
	int iOnline = 0;
	int i;
	
	localtime_r(&tCurr, &stNow);
	
	if (stNow.tm_hour < 12 )
	{
		return 0;
	}

	if (tCurr - pstClanBoon->Cond.AveTime < 60*60)
	{
		return 0;	
	}

	for (i=0; i<(int)pstZoneClan->stClan.ClanCore.MemNum; i++)
	{
		if (z_name_player(pstEnv->pstAppCtx,  pstEnv, pstZoneClan->stClan.ClanCore.ClanMem[i].RoleMember.RoleName))
		{
			iOnline ++;
		}
	}

	pstClanBoon->Cond.Ave += iOnline;//pstZoneClan->stZoneClanInfo.iOnlineCount;
	pstClanBoon->Cond.AveNum ++;

	stNow.tm_min = 0;
	stNow.tm_sec = 0;
	pstClanBoon->Cond.AveTime = mktime(&stNow); // 只记录正点时间

	// 通知客户端条件变化 todo:
	clan_boon_condition_chg(pstEnv, pstZoneClan);
	return 0;
}

//iType: 0-防守成功 1-防守失败 2-进攻成功
int clan_boon_strong_chg_update(ZONESVRENV *pstEnv,int iMapID, ZoneClan *pstZoneClan, int iType)
{
	CLANBOON *pstBoon = &pstZoneClan->stClan.ClanBoon;
	
	switch(iMapID)
	{
	case 2200:
		if (iType == 0)
		{
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].ID = BOON_ADD_TYPE_XUEYUAN_DEF_SUCC;
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].Per = 9;
			
		}
		else if (iType == 1)
		{
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].ID = BOON_ADD_TYPE_XUEYUAN_DEF_FAIL;
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].Per = -9;
		}
		else if(iType == 2)
		{
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].ID = BOON_ADD_TYPE_XUEYUAN_ATK_SUCC;
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].Per = 9;
		}
		else
		{
			return 0;
		}
		break;
	case 2202:
		if (iType == 0)
		{
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].ID = BOON_ADD_TYPE_BINGDAO_DEF_SUCC;
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].Per = 9;
			
		}
		else if (iType == 1)
		{
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].ID = BOON_ADD_TYPE_BINGDAO_DEF_FAIL;
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].Per = -9;
		}
		else if(iType == 2)
		{
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].ID = BOON_ADD_TYPE_BINGDAO_ATK_SUCC;
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].Per = 9;
		}
		else
		{
			return 0;
		}
		break;
	case 2300:
		if (iType == 0)
		{
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].ID = BOON_ADD_TYPE_XUEYUAN_DEF_SUCC;
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].Per = 9;
			
		}
		else if (iType == 1)
		{
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].ID = BOON_ADD_TYPE_XUEYUAN_DEF_FAIL;
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].Per = -9;
		}
		else if(iType == 2)
		{
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].ID = BOON_ADD_TYPE_XUEYUAN_ATK_SUCC;
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].Per = 9;
		}
		else
		{
			return 0;
		}
		break;
	case 2400:
		if (iType == 0)
		{
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].ID = BOON_ADD_TYPE_LUORI_DEF_SUCC;
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].Per = 10;
			
		}
		else if (iType == 1)
		{
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].ID = BOON_ADD_TYPE_LUORI_DEF_FAIL;
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].Per = -10;
		}
		else if(iType == 2)
		{
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].ID = BOON_ADD_TYPE_LUORI_ATK_SUCC;
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].Per = 10;
		}
		else
		{
			return 0;
		}
		break;
	case 2500:
		if (iType == 0)
		{
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].ID = BOON_ADD_TYPE_YULIN_DEF_SUCC;
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].Per = 10;
			
		}
		else if (iType == 1)
		{
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].ID = BOON_ADD_TYPE_YULIN_DEF_FAIL;
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].Per = -10;
		}
		else if(iType == 2)
		{
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].ID = BOON_ADD_TYPE_YULIN_ATK_SUCC;
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].Per = 10;
		}
		else
		{
			return 0;
		}
		break;
	case 3404:
		if (iType == 0)
		{
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].ID = BOON_ADD_TYPE_MINGSHA_DEF_SUCC;
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].Per = 12;
			
		}
		else if (iType == 1)
		{
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].ID = BOON_ADD_TYPE_MINGSHA_DEF_FAIL;
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].Per = -12;
		}
		else if(iType == 2)
		{
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].ID = BOON_ADD_TYPE_MINGSHA_ATK_SUCC;
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].Per = 12;
		}
		else
		{
			return 0;
		}
		break;
	case 4404:
		if (iType == 0)
		{
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].ID = BOON_ADD_TYPE_SHIJIAN_DEF_SUCC;
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].Per = 16;
			
		}
		else if (iType == 1)
		{
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].ID = BOON_ADD_TYPE_SHIJIAN_DEF_FAIL;
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].Per = -16;
		}
		else if(iType == 2)
		{
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].ID = BOON_ADD_TYPE_SHIJIAN_ATK_SUCC;
			pstBoon->Cond.Event[pstBoon->Cond.EventNum].Per = 16;
		}
		else
		{
			return 0;
		}
		break;

	}
	pstBoon->Cond.Event[pstBoon->Cond.EventNum].Time = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstBoon->Cond.EventNum ++;

	// todo 公会广播
	clan_boon_condition_chg(pstEnv, pstZoneClan);
	return 0;
}

//  	每周日早6点更新达成福利的条件
int clan_boon_clear(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan)
{
	CLANBOON *pstClanBoon = &pstZoneClan->stClan.ClanBoon;
	time_t tCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	struct tm stNow;
	int iPer = 100;
	int i;

	localtime_r(&tCurr, &stNow);

	if (stNow.tm_wday != 0 ||stNow.tm_hour != 6)
	{
		return 0;
	}

	if (tCurr - pstClanBoon->Cond.ClearTime < 7*24*60*60)
	{
		return 0;	
	}

	memset(&pstClanBoon->Assign, 0, sizeof(pstClanBoon->Assign));
	z_clan_boon_cond_oplog(pstEnv,NULL, &pstZoneClan->stClan);

	// 不是3级公会没有奖励
	if (pstZoneClan->stClan.ClanProf.Level < 3)
	{
		goto end;
	}
	
	// 检查条件
	if (pstClanBoon->Cond.Contri < CLAN_CONTRI_NUM)
	{
		tlog_info(pstEnv->pstLogCat, 0, 0, "clan_boon_clear:clanid=%d, contri=%d", 
			pstZoneClan->stClan.ClanProf.Id, pstClanBoon->Cond.Contri );
		goto end;
	}

	if (pstClanBoon->Cond.Tech < CLAN_TECH_NUM)
	{
		tlog_info(pstEnv->pstLogCat, 0, 0, "clan_boon_clear:clanid=%d, tech=%d", 
			pstZoneClan->stClan.ClanProf.Id, pstClanBoon->Cond.Tech );
		goto end;
	}

	if ( pstEnv->pstConf->CountryVer != COUNTRY_VER_KOREA)
	{
		if (pstClanBoon->Cond.Gold < CLAN_GOLD_NUM1)
		{
			tlog_info(pstEnv->pstLogCat, 0, 0, "clan_boon_clear:clanid=%d, gold=%d", 
				pstZoneClan->stClan.ClanProf.Id, pstClanBoon->Cond.Gold );
			goto end;
		}
	}

	if (pstClanBoon->Cond.AveNum <= 0)
	{
		goto end;
	}
	
	if ((int)(pstClanBoon->Cond.Ave/pstClanBoon->Cond.AveNum) <CLAN_ONLINE_AVE )
	{
		tlog_info(pstEnv->pstLogCat, 0, 0, "clan_boon_clear:clanid=%d, online=%d", 
			pstZoneClan->stClan.ClanProf.Id, (int)(pstClanBoon->Cond.Ave/pstClanBoon->Cond.AveNum) );
		goto end;
	}

	for (i=0; i<pstClanBoon->Cond.EventNum; i++)
	{
		iPer += pstClanBoon->Cond.Event[i].Per;
	}

	if (iPer < 100)
	{
		iPer = 100;
	}

	if (iPer > 250)
	{
		iPer = 250;
	}

	pstClanBoon->Assign.BindGlod = CLAN_BOON_BINDGOLD*(iPer/100.0f);

	//if (pstClanBoon->Cond.Gold >= CLAN_GOLD_NUM2)
	{
		pstClanBoon->Assign.Glod = CLAN_BOON_GOLD;
		pstClanBoon->Assign.AssignGlod.Money = CLAN_BOON_GOLD;
		pstClanBoon->Assign.AssignGlod.Pos = CLAN_POS_PRESIDENT;

		// 只记录谁能领金币,就算以后换了会长也只有记录的玩家可以领
		strncpy(pstClanBoon->Assign.AssignGlod.Name, pstZoneClan->stClan.ClanBrief.Leader.RoleName, 
				sizeof(pstClanBoon->Assign.AssignGlod.Name));	
	}
	

end:
	memset(&pstClanBoon->Cond, 0, sizeof(pstClanBoon->Cond));
	stNow.tm_min = 0;
	stNow.tm_sec = 0;
	pstClanBoon->Cond.ClearTime = mktime(&stNow); // 只记录正点时间

	// 全公会广播todo:
	clan_boon_all_notify(pstEnv, pstZoneClan, NULL);
	return 0;

}

int clan_boon_get(ZONESVRENV* pstEnv, Player *pstPlayer, CSBOONGET *pstGet, ZoneClan *pstClan)
{
	CLANBOON *pstBoon;
	int i;
	int iType;
	BOONASSIGNPOS *pstOne = NULL;
	int iPosition;
	char *pstName;

	pstBoon = &pstClan->stClan.ClanBoon;

	// 策划确定会长给500金券
	if (pstGet->Flag == 0)
	{
		iType = ATTR_ID_BINDGODCOIN;
		pstOne = &pstBoon->Assign.AssignGlod;
		pstName = MONEY_TYPE_BIND_GODCOIN_NAME;
	}
	else
	{
		iType = ATTR_ID_BINDGODCOIN;
		pstName = MONEY_TYPE_BIND_GODCOIN_NAME;
		for (i=0; i<pstBoon->Assign.Office; i++)
		{
			if (strcmp(pstPlayer->stRoleData.RoleName, pstBoon->Assign.AssignBindGlod[i].Name) == 0)
			{
				pstOne = &pstBoon->Assign.AssignBindGlod[i];
				break;
			}
		}
	}

	if (pstOne == NULL)
	{
		return -1;
	}

	if (pstOne->Money <= 0 || pstGet->Money > pstOne->Money)
	{
		return -1;
	}

	if (pstOne->Flag)
	{
		return -1;
	}

	if (package_add_money_test(pstEnv, pstPlayer, iType, pstOne->Money) < 0)
	{
		return -1;
	}

	// 条件满足
	pstOne->Flag = 1;
	pstClan->stZoneClanInfo.iDirty = CACHE_FLAG_IMM;
	package_add_money( pstEnv, pstPlayer, iType, pstOne->Money);
	if (ATTR_ID_BINDGODCOIN == iType)
	{
		z_bindgold_create_oplog(pstEnv, pstPlayer, BINDGOLD_OPTYPE_CLANBOON, 0, 
						pstOne->Money, "公会管理奖励");
	}

	iPosition = player_clan_position(pstEnv, pstPlayer);
	player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN,
		ZONEERR_CLANBROAD278,
		kClanPosName[iPosition],pstPlayer->stRoleData.RoleName, pstOne->Money, pstName);

	z_clan_boon_oplog(pstEnv,pstPlayer,iType, pstOne->Money);

	// 公会通知一下
	clan_boon_assign_chg(pstEnv, pstClan);
	return 0;
}

CLANMEM *clan_get_mem_by_name(ZoneClan *pstClan, char *pstName)
{
	int i;
	CLANCORE *pstCore = &pstClan->stClan.ClanCore;

	for (i=0; i<(int)pstCore->MemNum; i++)
	{
		if (strcmp(pstCore->ClanMem[i].RoleMember.RoleName, pstName) == 0)
		{
			return &pstCore->ClanMem[i];
		}
	}	
	return NULL;
}

int clan_boon_assign(ZONESVRENV* pstEnv, Player *pstPlayer, CSBOONASSIGN*pstAssign, ZoneClan *pstClan)
{
	CLANBOON *pstBoon;
	int i,j;
	int iCount = 0;
	CLANMEM *pstMem;

	pstBoon = &pstClan->stClan.ClanBoon;

	if (player_clan_position(pstEnv, pstPlayer) != CLAN_POS_PRESIDENT)
	{
		return -1;
	}

	// 分配过了
	if (pstBoon->Assign.Office > 0)
	{
		return -1;
	}

	// 没有金券分配
	if (pstBoon->Assign.BindGlod <= 0)
	{
		return -1;
	}

	if (pstAssign->Office <= 0 || pstAssign->Office > BOON_GET_MAX)
	{
		return -1;
	}

	for (i=0; i<pstAssign->Office; i++)
	{
		if (pstAssign->AssignBindGlod[i].Pos < CLAN_POS_VICE_PRESIDENT ||
			pstAssign->AssignBindGlod[i].Pos  == CLAN_POS_PRESIDENT)
		{
			return -1;
		}

		if (pstAssign->AssignBindGlod[i].Name[0] == 0)
		{
			return -1;
		}

		// 检查重复职位
		for (j=i+1; j<pstAssign->Office; j++)
		{
			if (pstAssign->AssignBindGlod[i].Pos == pstAssign->AssignBindGlod[j].Pos)
			{
				return -1;
			}
		}

		// 检查重复名字
		for (j=i+1; j<pstAssign->Office; j++)
		{
			if (strcmp(pstAssign->AssignBindGlod[i].Name, pstAssign->AssignBindGlod[j].Name) == 0)
			{
				return -1;
			}
		}

		// 检查职位和名字能不能对上
		pstMem = clan_get_mem_by_name(pstClan, pstAssign->AssignBindGlod[i].Name);
		if (!pstMem ||pstMem->Position != pstAssign->AssignBindGlod[i].Pos )
			return -1;
		
		iCount += pstAssign->AssignBindGlod[i].Money;
		pstAssign->AssignBindGlod[i].Flag = 0;
	}

	if (iCount > pstBoon->Assign.BindGlod)
	{
		return -1;
	}

	pstBoon->Assign.Office = pstAssign->Office;
	memcpy(&pstBoon->Assign.AssignBindGlod, &pstAssign->AssignBindGlod, 
			sizeof(pstBoon->Assign.AssignBindGlod[0])*pstAssign->Office);
	pstClan->stZoneClanInfo.iDirty = CACHE_FLAG_IMM;

	// 公会通知一下
	clan_boon_assign_chg(pstEnv, pstClan);
	return 0;
}

int clan_boon_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                        TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	ZoneClan *pstClan; 
	CSBOONCLT *pstBoonClt = &pstCsPkg->Body.BoonClt;

	if (pstEnv->pstClanObj->stClanObjInfo.iClanStat != CLAN_STAT_FINI)
	{
		return -1;
	}

	pstClan = z_id_clan( pstEnv, pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id);
	if (!pstClan )
	{
		return -1;
	}
	
	if (pstBoonClt->Cmd == BOON_CLT_OP_GET)
	{
		return clan_boon_get(pstEnv, pstPlayer, &pstBoonClt->Data.Get, pstClan);
	}
	else if (pstBoonClt->Cmd == BOON_CLT_OP_ASSIGN)
	{
		return clan_boon_assign(pstEnv, pstPlayer, &pstBoonClt->Data.Assign, pstClan);
	}
	
	return 0;
}
