

#include "zone_dianfeng.h"
#include "zone_player.h"
#include "zone_err.h"
#include "zone_package.h"
#include "zone_clt.h"
#include "zone_map.h"
#include "zone_itemlimit.h"
#include "zone_mail.h"
#include "zone_move.h"
#include "zone_combat.h"
#include "zone_designation.h"
#include "zone_sparta.h"
#include "zone_status.h"
#include "zone_script.h"
#include "zone_shop.h"
#include "zone_attr.h"


#define DINGSHNE_BUFF 1316

int g_EnterDianFeng = 0;

/*末世巅峰*/


//末世巅峰报名检查
int player_dianfeng_baoming_check(ZONESVRENV* pstEnv, Player *pstPlayer,DIANFENGGLOBAL *pstDianFengGlobal,DIANFENGINFO *pstDianFengInfo)
{
	if(pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE , ZONEERR_DIANFENG19);
		return -1;
	}	
	
	if(pstDianFengInfo->JoinTime != 0 && pstDianFengInfo->JoinTime != pstDianFengGlobal->ActiveTimeS)
	{
		pstDianFengInfo->LoginOutNum = 0;
		pstDianFengInfo->DianFengFlag = 0;
		pstDianFengInfo->PkOutNum = 0;
	}
	
	//   不是报名时间
	if(pstDianFengGlobal->Status == ACTIVE_PK)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG28);
		return -1;
	}

	if(pstDianFengGlobal->Status == ACTIVE_CLOSE || pstDianFengGlobal->Status == ACTIVE_CLEAN_DATA)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG29);
		return -1;
	}
	
	if(pstDianFengInfo->DianFengFlag & IF_DIANFENG_BAOMING)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE , ZONEERR_DIANFENG9);
		return -1;
	}

	if(pstPlayer->stRoleData.Level < 65)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE , ZONEERR_DIANFENG1);
		return -1;
	}

	if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, MOSHI_DIANFENG_BAOMING_MONEY))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE,ZONEERR_DIANFENG3,MOSHI_DIANFENG_BAOMING_MONEY);
		return -1;
	}

	return 0;
}

int player_dianfeng_baoming_data_new(ZONESVRENV* pstEnv, Player *pstPlayer,DIANFENGJOININFO *pstDianFengJoinInfo,unsigned int uiCID)
{
	if(0 > player_dianfeng_if_in_baoming_pipei(pstEnv,pstPlayer->stRoleData.RoleName,pstDianFengJoinInfo))
	{
		return -1;
	}
	
	int iMemNum = 0;
	iMemNum = pstDianFengJoinInfo->MemNum;
	if(iMemNum == 0)
	{
		pstDianFengJoinInfo->DFversion = pstEnv->pstAppCtx->stCurr.tv_sec;
	}
	else if(iMemNum > 0)
	{
		pstDianFengJoinInfo->DFversion++;
	}
	else
	{
		iMemNum = pstDianFengJoinInfo->MemNum = 0;
	}
	
	pstDianFengJoinInfo->GroupInfo[iMemNum].RoleName[0] = 0;
	if (pstPlayer->stRoleData.RoleName[0] != 0)
	{
		STRNCPY(pstDianFengJoinInfo->GroupInfo[iMemNum].RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstDianFengJoinInfo->GroupInfo[iMemNum].RoleName));
	}

	pstDianFengJoinInfo->GroupInfo[iMemNum].ClanID = uiCID;
	pstDianFengJoinInfo->GroupInfo[iMemNum].YiSupportNum = 0;
	pstDianFengJoinInfo->GroupInfo[iMemNum].RoleID = pstPlayer->stRoleData.RoleID;
	pstDianFengJoinInfo->GroupInfo[iMemNum].LoginOutTime = 0;

	pstDianFengJoinInfo->MemNum++;
	return 0;
}

//末世巅峰报名 
int player_dianfeng_baoming(ZONESVRENV* pstEnv, Player *pstPlayer,CSDIANFENGBAOMING *pstCSDianFengBaoMing)
{
	UNUSED(pstCSDianFengBaoMing);
	DIANFENGINFO *pstDianFengInfo = &pstPlayer->stRoleData.MiscInfo.DianFengInfo;
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;
	 
	DIANFENGJOININFO *pstDianFengJoinInfo = NULL;	
	unsigned int uiCID = 0;
	
	if(0 > player_dianfeng_baoming_check(pstEnv,pstPlayer,pstDianFengGlobal,pstDianFengInfo))
	{
		goto final;
	}
			
	//按照职业添加参加人数和数据
	if(0 > dianfeng_get_joininfo_by_career(pstEnv,&pstDianFengJoinInfo,pstDianFengGlobal,pstPlayer->stRoleData.Career))
		return -1;

	if(pstDianFengJoinInfo->MemNum >= MAX_DIANFENG_EVERY_GROUP_MEM_NUM)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG10);
		goto final;
	}

	if (0 > package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY, MOSHI_DIANFENG_BAOMING_MONEY))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG3,MOSHI_DIANFENG_BAOMING_MONEY);
		goto final;
	}

	CLANINFO *pstClanInfo = &pstPlayer->stRoleData.MiscInfo.ClanInfo;
	if(NULL != pstClanInfo)
	{
		uiCID = pstClanInfo->ClanId.Id;
	}

	pstDianFengInfo->JoinTime = pstDianFengGlobal->ActiveTimeS;
	pstDianFengInfo->DianFengFlag |= IF_DIANFENG_BAOMING;
	
	player_dianfeng_baoming_data_new(pstEnv, pstPlayer,pstDianFengJoinInfo,uiCID);
	
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG4);
	player_baoming_res_to_clt(pstEnv, pstPlayer,1);

	//更新一下报名界面
	player_first_or_effigy_info_to_clt(pstEnv, pstPlayer,FIRST_INFO);

	if(pstDianFengGlobal->Status == ACTIVE_READY)
	{
		//如果在这个阶段报名的，直接出现传送框
		player_transmit_res_to_clt(pstEnv, pstPlayer,0);
	}

	return 0;
	
final:
	player_baoming_res_to_clt(pstEnv, pstPlayer,2);
	return -1;
}


int player_dianfeng_transmit_check(ZONESVRENV* pstEnv, Player *pstPlayer,DIANFENGINFO *pstDianFengInfo)
{	
	if (pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN ||
		pstPlayer->stOnline.stSpanOnline.stApply.iBattleApply == SPAN_APPLY_ING ||
		pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG23);
		return -1;
	}
	
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst = NULL;
	MapIndex* pstMapIdx = NULL;
	
	if(pstPlayer->stRoleData.Map > PWORLD_MAPID_BASE)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG18);
		return -1;
	}

	stIdx.iID	=	pstPlayer->stRoleData.Map;
	stIdx.iPos	=	pstPlayer->stOnline.iMapPos;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (!pstMapInst)
		return -1;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	if (pstMapIdx->stMapDef.CtrlFlag &MAP_CTRL_LOGOUT_TO_REVIVE_POINT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG17);
		return -1;
	}

	if (0 > z_player_goto_check(pstEnv, pstPlayer, 0, NULL))
	{
		return -1;
	}

	//未报名
	if(!(pstDianFengInfo->DianFengFlag & IF_DIANFENG_BAOMING))
	{ 
		return -1;
	}

	if(pstPlayer->stOnline.State & CS_STAT_FIGHT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG36);
		return -1;
	}

	if(0 < if_player_has_dingshen_status(pstEnv, pstPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG37);
		return -1;
	}
	
	return 0;
}

int player_dianfeng_transmit(ZONESVRENV* pstEnv, Player *pstPlayer,CSDIANFENGTRANSMIT *pstCSDianFengTransmit)
{
	UNUSED(pstCSDianFengTransmit);
	DIANFENGINFO *pstDianFengInfo = &pstPlayer->stRoleData.MiscInfo.DianFengInfo;
	
	if (0 > player_dianfeng_transmit_check(pstEnv, pstPlayer,pstDianFengInfo))
	{
		goto final;
	}
	
	RESPOS stResPos;
	int iMapIndex = 0;

	//传送到职业组相应的准备地图上 
	if(0 > dianfeng_get_ready_map_by_career(pstEnv,pstPlayer->stRoleData.Career,&iMapIndex))
	{
		goto final;
	}
	
	stResPos.X = 6600;
	stResPos.Y = 6000;

	if(0 > z_dianfeng_change_map(pstEnv, pstPlayer,iMapIndex,stResPos,1))
	{
		goto final;
	}
	
	pstDianFengInfo->DianFengFlag |= IF_DIANFENG_TRANSMIT_SUC;
	
	player_transmit_res_to_clt(pstEnv, pstPlayer,1);
	return 0;

final:
	player_transmit_res_to_clt(pstEnv, pstPlayer,2);
	return -1;
}

int player_support_val_chg(ZONESVRENV *pstEnv,DIANFENGJOININFO *pstDianFengJoinInfo,tdr_ulonglong RoleID)
{
	if(NULL == pstDianFengJoinInfo)
	{
		return -1;
	}

	int flag = 0;
	int iMemNum = min(pstDianFengJoinInfo->MemNum,MAX_DIANFENG_EVERY_GROUP_MEM_NUM);
	int i;
	GROUPINFO *pstGroupInfo = NULL;
	
	if(iMemNum <= 0)
	{
		return -1;
	}

	for(i=0;i<iMemNum;i++)
	{
		pstGroupInfo = &pstDianFengJoinInfo->GroupInfo[i];
		if(NULL == pstGroupInfo)
		{
			continue;
		}
		
		if( pstGroupInfo->RoleID != RoleID)
		{
			continue;
		}

		pstGroupInfo->YiSupportNum++;

		flag = 1;
	}

	if (1 == flag)
	{
		pstDianFengJoinInfo->DFversion ++;
	}
	return 0;
}

//修改支持量和保存竞猜者信息
int player_dianfeng_guessinfos_and_support_val_chg(ZONESVRENV* pstEnv, Player *pstPlayer,GUESSINFOS *pstGuessInfos,DIANFENGGLOBAL *pstDianFengGlobal)
{
	int i;
	GUESSINFOS *pstGuess = &pstDianFengGlobal->GuessInfos[pstDianFengGlobal->GuessMemNum];
	
	DIANFENGJOININFO *pstDianFengJoinInfo = NULL;
	pstGuess->Num = MAX_SUPPORT_MEM_NUM;
	
	for(i=0;i<pstGuessInfos->Num;i++)
	{
		if(pstGuessInfos->SupportMem[i].RoleID == 0)
		{
			continue;
		}
			
		//增加支持量
		if(i == DIANFENG_JOB_ZFZ-1)
		{
			pstDianFengJoinInfo = &pstDianFengGlobal->DianFengZFZJoinInfo;
		}
		else if(i == DIANFENG_JOB_SHZ-1)
		{
			pstDianFengJoinInfo = &pstDianFengGlobal->DianFengSHZJoinInfo;
		}
		else if(i == DIANFENG_JOB_YLS-1)
		{
			pstDianFengJoinInfo = &pstDianFengGlobal->DianFengYLSJoinInfo;
		}
		else if(i == DIANFENG_JOB_SYZ-1)
		{
			pstDianFengJoinInfo = &pstDianFengGlobal->DianFengSYZJoinInfo;
		}
		else if (i == DIANFENG_JOB_LSZ-1)
		{
			pstDianFengJoinInfo = &pstDianFengGlobal->DianFengLSZJoinInfo;
		}
		else if(i == DIANFENG_JOB_HMZ-1)
		{
			pstDianFengJoinInfo = &pstDianFengGlobal->DianFengHMZJoinInfo;
		}
		else if(i == DIANFENG_JOB_LHS-1)
		{
			pstDianFengJoinInfo = &pstDianFengGlobal->DianFengLHSJoinInfo;
		}
		else if(i == DIANFENG_JOB_JNZ-1)
		{
			pstDianFengJoinInfo = &pstDianFengGlobal->DianFengJNZJoinInfo;
		}
		else 
		{
			return -1;
		}

		player_support_val_chg(pstEnv,pstDianFengJoinInfo,pstGuessInfos->SupportMem[i].RoleID);
		
		//保存被支持者 信息
		pstGuess->SupportMem[i].RoleID = pstGuessInfos->SupportMem[i].RoleID;
	}

	STRNCPY(pstGuess->GuessRoleName,pstPlayer->stRoleData.RoleName,sizeof(pstGuess->GuessRoleName));
	pstDianFengGlobal->GuessMemNum++;
	
	return 0;
}

//末世巅峰竞猜检查
int player_dianfeng_guess_check(ZONESVRENV* pstEnv, Player *pstPlayer,GUESSINFOS *pstGuessInfos,DIANFENGGLOBAL *pstDianFengGlobal)
{
	if(pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE , ZONEERR_DIANFENG19);
		return -1;
	}	
	
	DIANFENGINFO *pstDianFengInfo = &pstPlayer->stRoleData.MiscInfo.DianFengInfo;
	int i;
	
	DIANFENGJOININFO *pstDianFengZFZJoinInfo = &pstDianFengGlobal->DianFengZFZJoinInfo;
	DIANFENGJOININFO *pstDianFengSHZJoinInfo = &pstDianFengGlobal->DianFengSHZJoinInfo;
	DIANFENGJOININFO *pstDianFengYLSJoinInfo = &pstDianFengGlobal->DianFengYLSJoinInfo;
	DIANFENGJOININFO *pstDianFengSYZJoinInfo = &pstDianFengGlobal->DianFengSYZJoinInfo;
	DIANFENGJOININFO *pstDianFengLSZJoinInfo = &pstDianFengGlobal->DianFengLSZJoinInfo;
	DIANFENGJOININFO *pstDianFengHMZJoinInfo = &pstDianFengGlobal->DianFengHMZJoinInfo;
	DIANFENGJOININFO *pstDianFengLHSJoinInfo = &pstDianFengGlobal->DianFengLHSJoinInfo;
	DIANFENGJOININFO *pstDianFengJNZJoinInfo = &pstDianFengGlobal->DianFengJNZJoinInfo;

	if(pstDianFengGlobal->Status == ACTIVE_PK)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG26);
		return -1;
	}

	if(pstDianFengGlobal->Status == ACTIVE_CLOSE || pstDianFengGlobal->Status == ACTIVE_CLEAN_DATA)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG27);
		return -1;
	}
	
	//有一组报名人数少于3人时不能进行冠军竞猜
	if(pstDianFengZFZJoinInfo->MemNum < MIN_GUESS_BAOMING_MEM_NUM || pstDianFengSHZJoinInfo->MemNum < MIN_GUESS_BAOMING_MEM_NUM ||
	   pstDianFengYLSJoinInfo->MemNum < MIN_GUESS_BAOMING_MEM_NUM || pstDianFengSYZJoinInfo->MemNum < MIN_GUESS_BAOMING_MEM_NUM ||
	   pstDianFengLSZJoinInfo->MemNum < MIN_GUESS_BAOMING_MEM_NUM || pstDianFengHMZJoinInfo->MemNum < MIN_GUESS_BAOMING_MEM_NUM ||
	   pstDianFengLHSJoinInfo->MemNum < MIN_GUESS_BAOMING_MEM_NUM || pstDianFengJNZJoinInfo->MemNum < MIN_GUESS_BAOMING_MEM_NUM )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG11,MIN_GUESS_BAOMING_MEM_NUM);
		return -1;
	}

	if(pstDianFengInfo->JoinTime != 0 && pstDianFengInfo->JoinTime != pstDianFengGlobal->ActiveTimeS)
	{
		pstDianFengInfo->LoginOutNum = 0;
		pstDianFengInfo->DianFengFlag = 0;
		pstDianFengInfo->PkOutNum = 0;
	}

	//巅峰竞猜只能竞猜1次
	if(pstDianFengInfo->DianFengFlag & IF_DIANFENG_GUESS)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG13);
		return -1;
	}

	//最大的竞猜者数
	if(pstDianFengGlobal->GuessMemNum >= MAX_GUESS_PLAYER_NUM)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG12);
		return -1;
	}
	
	//玩家需要对8个职业组一个个选择，全部选择完成后才能竞猜
	if(pstGuessInfos->Num != MAX_SUPPORT_MEM_NUM)
	{
		return -1;
	}

	for(i=0;i<pstGuessInfos->Num;i++)
	{
		if(pstGuessInfos->SupportMem[i].RoleID == 0)
		{
			return -1;
		}
	}
	
	if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, MOSHI_DIANFENG_JINGCAI_MONEY))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG5 ,MOSHI_DIANFENG_JINGCAI_MONEY);
		return -1;
	}
	
	return 0;
}

//点击确认竞猜按钮发送的消息。末世巅峰竞猜
int player_dianfeng_guess(ZONESVRENV* pstEnv, Player *pstPlayer,GUESSINFOS *pstGuessInfos)
{
	DIANFENGINFO *pstDianFengInfo = &pstPlayer->stRoleData.MiscInfo.DianFengInfo;
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;
	GUESSINFOS stGuessInfos;
	memset(&stGuessInfos,0,sizeof(stGuessInfos));
	
	if(0 > player_dianfeng_guess_check(pstEnv, pstPlayer,pstGuessInfos,pstDianFengGlobal))
	{
		goto final;
	}

	if (0 > package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY, MOSHI_DIANFENG_JINGCAI_MONEY))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG5 ,MOSHI_DIANFENG_JINGCAI_MONEY);
		goto final;
	}

	//本届竞猜奖池
	pstDianFengGlobal->AllMoneyNum += MOSHI_DIANFENG_JINGCAI_MONEY/2;
		
	//修改支持量和保存竞猜者信息
	player_dianfeng_guessinfos_and_support_val_chg(pstEnv,pstPlayer,pstGuessInfos,pstDianFengGlobal);

	pstDianFengInfo->DianFengFlag |= IF_DIANFENG_GUESS;
	pstDianFengInfo->JoinTime = pstDianFengGlobal->ActiveTimeS;

	memcpy(&stGuessInfos,pstGuessInfos,sizeof(stGuessInfos));
	
	//成功竞猜
	player_guess_res_to_clt(pstEnv,pstPlayer,1,stGuessInfos,pstDianFengGlobal);

	//发竞猜名单邮件和给客户端
	dianfeng_get_guess_mingdan(pstEnv,pstPlayer,pstDianFengGlobal,1);

	return 0;

final:
	player_guess_res_to_clt(pstEnv,pstPlayer,2,stGuessInfos,pstDianFengGlobal);
	return -1;
}

//竞猜参与者邮件
int player_moshi_dianfeng_join_award(ZONESVRENV *pstEnv,char *pName,int iNum,tdr_ulonglong iMoney)
{
	char szText[512];

	if(NULL == pName)
	{
		return -1;
	}
	
	if(iNum < 0)
	{
		iNum = 0;
	}

	snprintf(szText, sizeof(szText), LJY_MAIL60 ,pName,iNum,iMoney);
				   
	player_sysmail_fast_send(pstEnv, pName, LJY_MAIL59 ,
		szText, NULL, 0, 0,LJY_MAIL63, 0);

	return 0;
}

//竞猜获奖者邮件
int player_moshi_dianfeng_first_award(ZONESVRENV *pstEnv,char *pName,int iNum,tdr_ulonglong iMoney)
{
	char szText[512];

	if(NULL == pName)
	{
		return -1;
	}
	
	if(iNum < 0)
	{
		iNum = 0;
	}
	
	snprintf(szText, sizeof(szText), LJY_MAIL62 ,pName,iNum,iMoney);
				   
	player_sysmail_fast_send(pstEnv, pName, LJY_MAIL61 ,
		szText, NULL, 0, iMoney,LJY_MAIL63, 0);

	return 0;
}

int player_moshi_dianfeng_money_pool_award(ZONESVRENV *pstEnv,char *pName,tdr_ulonglong iMoney)
{
	char szText[512];

	if(NULL == pName)
	{
		return -1;
	}

	snprintf(szText, sizeof(szText), LJY_MAIL64 ,pName,iMoney);
				   
	player_sysmail_fast_send(pstEnv, pName, LJY_MAIL59 ,
		szText, NULL, 0, 0,LJY_MAIL63, 0);
	return 0;
}

int player_dianfeng_guess_mingdan_mail(ZONESVRENV *pstEnv,CSDIANFENGGUESSMINGDAN *pstCSDianFengGuessMingDan)
{
	char szText[512];

	if(NULL == pstCSDianFengGuessMingDan)
	{
		return -1;
	}

	snprintf(szText, sizeof(szText), LJY_MAIL69 ,pstCSDianFengGuessMingDan->GuessRoleName,
		pstCSDianFengGuessMingDan->SupportMingDan[0].Name,
		pstCSDianFengGuessMingDan->SupportMingDan[1].Name,
		pstCSDianFengGuessMingDan->SupportMingDan[2].Name,
		pstCSDianFengGuessMingDan->SupportMingDan[3].Name,
		pstCSDianFengGuessMingDan->SupportMingDan[4].Name,
		pstCSDianFengGuessMingDan->SupportMingDan[5].Name,
		pstCSDianFengGuessMingDan->SupportMingDan[6].Name,
		pstCSDianFengGuessMingDan->SupportMingDan[7].Name
		);
				   
	player_sysmail_fast_send(pstEnv, pstCSDianFengGuessMingDan->GuessRoleName, LJY_MAIL70 ,
		szText, NULL, 0, 0,LJY_MAIL63, 0);
	return 0;
}


//客户端点击竞猜职业选择某一组
int player_clt_need_which_one_group_info(ZONESVRENV *pstEnv,Player *pstPlayer,CSDIANFENGGROUPINFOC *pstCSDianFengGroupInfoC)
{
	if(pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE , ZONEERR_DIANFENG19);
		return -1;
	}	
	
	int iversion = pstCSDianFengGroupInfoC->DFversion;
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;

	if(pstDianFengGlobal->Status == ACTIVE_PK)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG26);
		return -1;
	}

	if(pstDianFengGlobal->Status == ACTIVE_CLOSE || pstDianFengGlobal->Status == ACTIVE_CLEAN_DATA)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG27);
		return -1;
	}
	
	DIANFENGJOININFO *pstDianFengJoinInfo = NULL;
	int Career = 0;

	switch(pstCSDianFengGroupInfoC->WhichOne)
	{
		case DIANFENG_JOB_ZFZ:
			Career = CAREER_WARRIOR_DOUBLEHAND;
			break;
		case DIANFENG_JOB_SHZ:
			Career = CAREER_WARRIOR_SINGLEHAND;
			break;
		case DIANFENG_JOB_YLS:
			Career = CAREER_CHURCH_DOUBLEHAND;
			break;
		case DIANFENG_JOB_SYZ:
			Career = CAREER_CHURCH_SINGLEHAND;
			break;
		case DIANFENG_JOB_LSZ:
			Career = CAREER_GUNER_SINGLEHAND;
			break;
		case DIANFENG_JOB_HMZ:
			Career = CAREER_GUNER_DOUBLEHAND;
			break;
		case DIANFENG_JOB_LHS:
			Career = CAREER_MAGICOR_DOUBLEHAND;
			break;
		case DIANFENG_JOB_JNZ:
			Career = CAREER_MAGICOR_SINGLEHAND;
			break;
		default:
			return -1;
			break;
	}

	if(0 > dianfeng_get_joininfo_by_career(pstEnv,&pstDianFengJoinInfo,pstDianFengGlobal,Career))
		return -1;

	if(pstDianFengJoinInfo->DFversion == iversion)
	{	
		if(pstDianFengJoinInfo->DFversion == 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG30);
		}
		return -1;
	}
	
	player_group_info(pstEnv,pstPlayer,pstDianFengJoinInfo,pstCSDianFengGroupInfoC->WhichOne);
	
	return 0;
}

int player_group_info(ZONESVRENV *pstEnv,Player *pstPlayer,DIANFENGJOININFO *pstDianFengJoinInfo,int iWitchOne)
{
	if(NULL == pstDianFengJoinInfo)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG30);
		return -1;
	}

	int iMemNum = min(pstDianFengJoinInfo->MemNum,MAX_DIANFENG_EVERY_GROUP_MEM_NUM);
	int i;
	GROUPINFO *pstGroupInfo = NULL;
	DFGROUPINFO stDFGroup;
	memset(&stDFGroup,0,sizeof(stDFGroup));
	
	if(iMemNum <= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG30);
		return -1;
	}

	stDFGroup.MemNum = iMemNum;

	for(i=0;i<iMemNum;i++)
	{
		pstGroupInfo = &pstDianFengJoinInfo->GroupInfo[i];
		if(NULL == pstGroupInfo)
		{
			continue;
		}
		stDFGroup.GroupMemInfo[i].ClanID = pstGroupInfo->ClanID;
		stDFGroup.GroupMemInfo[i].YiSupportNum = pstGroupInfo->YiSupportNum;
		stDFGroup.GroupMemInfo[i].RoleID = pstGroupInfo->RoleID;
		STRNCPY(stDFGroup.GroupMemInfo[i].RoleName, pstGroupInfo->RoleName, CCH(stDFGroup.GroupMemInfo[i].RoleName));
	}

	player_group_info_to_clt(pstEnv,pstPlayer,iWitchOne,pstDianFengJoinInfo->DFversion,&stDFGroup);
	return 0;
}

//下发组信息给客户端
int player_group_info_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,int iWhichOne,int iDFversion,DFGROUPINFO *pstDFGroup)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	CSDIANFENGGROUPUIINFO *pstDianFengUIInfo = &pstMiscSvr->PlayerMiscSvrData.DianFengGroupUIInfo;

	pstMiscSvr->PlayerMiscType = PLAYER_DIANFENG_GROUP_UI_INFO;
	pstDianFengUIInfo->WhichOne = iWhichOne;
	pstDianFengUIInfo->DFversion = iDFversion;
	memcpy(&pstDianFengUIInfo->DFGroupInfo,pstDFGroup,sizeof(pstDianFengUIInfo->DFGroupInfo));
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

//对战情况
int player_win_loss_ui_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,DIANFENGWINLOSSUI *pstDianFengWinLoss)
{
	if(NULL == pstDianFengWinLoss)
	{
		return -1;
	}
	
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	DIANFENGWINLOSSUI *pstDianFengWinLossUI = &pstMiscSvr->PlayerMiscSvrData.DianFengWinLossUI;

	pstMiscSvr->PlayerMiscType = PLAYER_DIANFENG_WIN_LOSS_UI;
	pstDianFengWinLossUI->FieldNum = MAX_DIANFENG_PK_FIELD_NUM;
	int i;
	
	for(i=0;i<MAX_DIANFENG_PK_FIELD_NUM;i++)
	{
		/*0代表未开始打，1代表赢了，2代表输了, 3 表示平*/
		if(pstDianFengWinLoss->WinLossInfo[i].Info == 3)
		{
			// 平了也是显示输了
			pstDianFengWinLossUI->WinLossInfo[i].Info = 2;
		}
		else
		{
			pstDianFengWinLossUI->WinLossInfo[i].Info = pstDianFengWinLoss->WinLossInfo[i].Info;
		}
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

//传送情况结果告知客户端
int player_transmit_res_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,int iRes)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	DIANFENGTRANSMITUI *pstDianFengTransmitUI = &pstMiscSvr->PlayerMiscSvrData.DianFengTransmitUI;

	pstMiscSvr->PlayerMiscType = PLAYER_DIANFENG_TRANSMIT_UI_SVR;
	
	/*0的话表示弹出传送提示框，1表示传送成功，2表示传送失败*/
	/*3表示关闭*/
	pstDianFengTransmitUI->Res = iRes;
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

//报名情况告知客户端
int player_baoming_res_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,int iRes)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	DIANFENGBAOMINGUI *pstDianFengBaoMingUI = &pstMiscSvr->PlayerMiscSvrData.DianFengBaoMingUI;

	pstMiscSvr->PlayerMiscType = PLAYER_DIANFENG_BAOMING_UI_SVR;
	
	/*0的话打开报名提示框，1的话报名成功，2的话报名失败,3表示关闭*/
	pstDianFengBaoMingUI->Res = iRes;
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int player_guess_res_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,int iRes,GUESSINFOS stGuessInfos,DIANFENGGLOBAL *pstDianFengGlobal)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	CSDIANFENGGUESSRESSVR *pstDianFengGuessResSvr = &pstMiscSvr->PlayerMiscSvrData.DianFengGuessResSvr;

	pstMiscSvr->PlayerMiscType = PLAYER_DIANFENG_GUESS_RES_SVR;
	
	/*1表示竞猜成功，2表示竞猜失败,3表示已经竞猜过了，4表示没有竞猜过*/
	pstDianFengGuessResSvr->Res = iRes;
	pstDianFengGuessResSvr->AllMoneyNum = pstDianFengGlobal->AllMoneyNum;

	//  当iRes 为1  或者3  的时候，以下才有值
	memcpy(&pstDianFengGuessResSvr->DianFengJingCai,&stGuessInfos,sizeof(pstDianFengGuessResSvr->DianFengJingCai));
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

//巅峰之战匹配成功后含有倒计时的二级提示框
int player_pipei_succeed_ui_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,int res)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	DIANFENGPIPEISUCCEEDUI *pstDianFengPiPeiSucceedUI = &pstMiscSvr->PlayerMiscSvrData.DianFengPiPeiSucceedUI;

	pstMiscSvr->PlayerMiscType = PLAYER_DIANFENG_PIPEI_SUCCEED_UI;
	//res 1表示打开倒计时提示框，2表示关闭
	pstDianFengPiPeiSucceedUI->res = res;
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

//s->c:末世巅峰上一届冠军信息或雕像信息
int player_first_or_effigy_info_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,int type)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	CSDIANFENGFIRSTINFO *pstDianFengFirstInfo = &pstMiscSvr->PlayerMiscSvrData.DianFengFirstInfo;
	memset(pstDianFengFirstInfo,0,sizeof(*pstDianFengFirstInfo));

	pstMiscSvr->PlayerMiscType = PLAYER_DIANFENG_FIRST_INFO;
	
	DIANFENGINFO *pstDianFengInfo = &pstPlayer->stRoleData.MiscInfo.DianFengInfo;
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;
		
	DFFIRST *pstDFZFZFirstInfo = &pstDianFengGlobal->DFZFZFirstInfo;
	DFFIRST *pstDFSHZFirstInfo = &pstDianFengGlobal->DFSHZFirstInfo;
	DFFIRST *pstDFYLSFirstInfo = &pstDianFengGlobal->DFYLSFirstInfo;
	DFFIRST *pstDFSYZFirstInfo = &pstDianFengGlobal->DFSYZFirstInfo;
	DFFIRST *pstDFLSZFirstInfo = &pstDianFengGlobal->DFLSZFirstInfo;
	DFFIRST *pstDFHMZFirstInfo = &pstDianFengGlobal->DFHMZFirstInfo;
	DFFIRST *pstDFLHSFirstInfo = &pstDianFengGlobal->DFLHSFirstInfo;
	DFFIRST *pstDFJNZFirstInfo = &pstDianFengGlobal->DFJNZFirstInfo;

	switch(type)
	{
		case FIRST_INFO:
			pstDianFengFirstInfo->Type = type;
			pstDianFengFirstInfo->ActiveTimeS = pstDianFengGlobal->ActiveTimeS;

			if(pstDianFengGlobal->Status == ACTIVE_OPEN || pstDianFengGlobal->Status == ACTIVE_READY)
			{
				if(pstDianFengInfo->DianFengFlag & IF_DIANFENG_BAOMING)
				{
					pstDianFengFirstInfo->IfBaoMing = 1;
					pstDianFengFirstInfo->Status = BAOMING_YI_BAO;
				}
				else
				{
					pstDianFengFirstInfo->IfBaoMing = 0;
					pstDianFengFirstInfo->Status = BAOMING_XIANSHI;
				}
			}
			else
			{
				pstDianFengFirstInfo->Status = BAOMING_NO_IN_TIME;
			}

			memcpy(&pstDianFengFirstInfo->DFZFZFirstInfo.DFChampionInfo,&pstDFZFZFirstInfo->DFChampionInfo,sizeof(pstDianFengFirstInfo->DFZFZFirstInfo.DFChampionInfo));
			memcpy(&pstDianFengFirstInfo->DFSHZFirstInfo.DFChampionInfo,&pstDFSHZFirstInfo->DFChampionInfo,sizeof(pstDianFengFirstInfo->DFSHZFirstInfo.DFChampionInfo));
			memcpy(&pstDianFengFirstInfo->DFYLSFirstInfo.DFChampionInfo,&pstDFYLSFirstInfo->DFChampionInfo,sizeof(pstDianFengFirstInfo->DFYLSFirstInfo.DFChampionInfo));
			memcpy(&pstDianFengFirstInfo->DFSYZFirstInfo.DFChampionInfo,&pstDFSYZFirstInfo->DFChampionInfo,sizeof(pstDianFengFirstInfo->DFSYZFirstInfo.DFChampionInfo));
			memcpy(&pstDianFengFirstInfo->DFLSZFirstInfo.DFChampionInfo,&pstDFLSZFirstInfo->DFChampionInfo,sizeof(pstDianFengFirstInfo->DFLSZFirstInfo.DFChampionInfo));
			memcpy(&pstDianFengFirstInfo->DFHMZFirstInfo.DFChampionInfo,&pstDFHMZFirstInfo->DFChampionInfo,sizeof(pstDianFengFirstInfo->DFHMZFirstInfo.DFChampionInfo));
			memcpy(&pstDianFengFirstInfo->DFLHSFirstInfo.DFChampionInfo,&pstDFLHSFirstInfo->DFChampionInfo,sizeof(pstDianFengFirstInfo->DFLHSFirstInfo.DFChampionInfo));
			memcpy(&pstDianFengFirstInfo->DFJNZFirstInfo.DFChampionInfo,&pstDFJNZFirstInfo->DFChampionInfo,sizeof(pstDianFengFirstInfo->DFJNZFirstInfo.DFChampionInfo));
			break;
		case FIRST_EFFIGY:
			pstDianFengFirstInfo->Type = type;
			pstDianFengFirstInfo->WhichOne = 0;
			memcpy(&pstDianFengFirstInfo->DFZFZFirstInfo.DFEffigyInfo,&pstDFZFZFirstInfo->DFEffigyInfo,sizeof(pstDianFengFirstInfo->DFZFZFirstInfo.DFEffigyInfo));
			memcpy(&pstDianFengFirstInfo->DFSHZFirstInfo.DFEffigyInfo,&pstDFSHZFirstInfo->DFEffigyInfo,sizeof(pstDianFengFirstInfo->DFSHZFirstInfo.DFEffigyInfo));
			memcpy(&pstDianFengFirstInfo->DFYLSFirstInfo.DFEffigyInfo,&pstDFYLSFirstInfo->DFEffigyInfo,sizeof(pstDianFengFirstInfo->DFYLSFirstInfo.DFEffigyInfo));
			memcpy(&pstDianFengFirstInfo->DFSYZFirstInfo.DFEffigyInfo,&pstDFSYZFirstInfo->DFEffigyInfo,sizeof(pstDianFengFirstInfo->DFSYZFirstInfo.DFEffigyInfo));
			memcpy(&pstDianFengFirstInfo->DFLSZFirstInfo.DFEffigyInfo,&pstDFLSZFirstInfo->DFEffigyInfo,sizeof(pstDianFengFirstInfo->DFLSZFirstInfo.DFEffigyInfo));
			memcpy(&pstDianFengFirstInfo->DFHMZFirstInfo.DFEffigyInfo,&pstDFHMZFirstInfo->DFEffigyInfo,sizeof(pstDianFengFirstInfo->DFHMZFirstInfo.DFEffigyInfo));
			memcpy(&pstDianFengFirstInfo->DFLHSFirstInfo.DFEffigyInfo,&pstDFLHSFirstInfo->DFEffigyInfo,sizeof(pstDianFengFirstInfo->DFLHSFirstInfo.DFEffigyInfo));
			memcpy(&pstDianFengFirstInfo->DFJNZFirstInfo.DFEffigyInfo,&pstDFJNZFirstInfo->DFEffigyInfo,sizeof(pstDianFengFirstInfo->DFJNZFirstInfo.DFEffigyInfo));
	
			break;
		default:
			return -1;
			break;

	}
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

//把时间回退到周一零点零分零秒
time_t time_to_mon(time_t t1)
{
	struct tm *pstTemps;
	int weekday;
	time_t tStartTmp;
	
	pstTemps = localtime(&t1);
	weekday = pstTemps->tm_wday;
	if(weekday == 0)
	{
		weekday = 7;
	}
		
	tStartTmp = t1 - (weekday-1)*24*60*60;
	pstTemps = localtime(&tStartTmp);
	pstTemps->tm_hour = 0;
	pstTemps->tm_min = 0;
	pstTemps->tm_sec = 0;
	
	return mktime(pstTemps);
}

int dianfeng_set_open(ZONESVRENV* pstEnv, time_t tTime)
{
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;

	pstDianFengGlobal->ActiveTimeS = tTime;

	dianfeng_clean_some_data(pstEnv,pstDianFengGlobal,1);
	
	return 0;
}

int player_dianfeng_tick(ZONESVRENV* pstEnv)
{
	if(pstEnv->pstConf->DianFengFlag == 0 || (is_fini_global(pstEnv) == 0) || get_curr_max_level(pstEnv) < 65)
	{
		return 0;
	}
	
	time_t  t = pstEnv->pstAppCtx->stCurr.tv_sec;
	struct tm *pstTm;
	pstTm = localtime(&t);	
	int weekday;
	time_t temps;
	struct tm *pstTemps;

	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;
	
	if(0 == pstDianFengGlobal->ActiveTimeS)
	{
		weekday = pstTm->tm_wday;
		if(weekday == 0)
		{
			weekday = 7;
		}

		if(weekday >= 6)
		{
			if(weekday == 7)
			{
				temps =  pstEnv->pstAppCtx->stCurr.tv_sec + 24*60*60;
			}
			else 
			{
				temps =  pstEnv->pstAppCtx->stCurr.tv_sec + 2*24*60*60;
			}

			//下周一
			pstTemps = localtime(&temps);
			pstTemps->tm_hour = 0;
			pstTemps->tm_min = 0;
			pstTemps->tm_sec = 0;

			dianfeng_set_open(pstEnv, mktime(pstTemps));
			
			dianfeng_clear(pstEnv,1);
			dianfeng_clear(pstEnv,0);
		}
		else
		{
			//这一周就开始
			pstDianFengGlobal->ActiveTimeS = time_to_mon(pstEnv->pstAppCtx->stCurr.tv_sec);
			dianfeng_set_open(pstEnv,pstDianFengGlobal->ActiveTimeS);
			dianfeng_clear(pstEnv,1);
			dianfeng_clear(pstEnv,0);
		}
	}
	else
	{
		if(pstEnv->pstAppCtx->stCurr.tv_sec - pstDianFengGlobal->ActiveTimeS >= 2*7*24*60*60)
		{
			pstDianFengGlobal->ActiveTimeS = time_to_mon(pstEnv->pstAppCtx->stCurr.tv_sec);
			dianfeng_set_open(pstEnv,pstDianFengGlobal->ActiveTimeS);
		}
	}

	if(pstEnv->pstAppCtx->stCurr.tv_sec - pstDianFengGlobal->ActiveTimeS >= 0)
	{
		if(!(pstDianFengGlobal->DFFlag & DIANFENG_OPEN_FLAG))
		{
			pstDianFengGlobal->DFFlag |= DIANFENG_OPEN_FLAG;
			pstDianFengGlobal->Status = ACTIVE_OPEN;
			pstDianFengGlobal->RunTimeS = time_to_mon(pstEnv->pstAppCtx->stCurr.tv_sec);
		}
	}

	if(pstDianFengGlobal->DFFlag & DIANFENG_OPEN_FLAG)
	{
		//周六零点的时候，未报名并且在线的玩家，发送巅峰活动开启提示窗
		if(pstEnv->pstAppCtx->stCurr.tv_sec - pstDianFengGlobal->RunTimeS >= (5*24*60*60))
		{
			if(!(pstDianFengGlobal->DFFlag & SHOW_DIANFENG_OPEN_LING))
			{
				all_login_player_dianfeng_baoming_dlg(pstEnv);
				pstDianFengGlobal->DFFlag |= SHOW_DIANFENG_OPEN_LING;
			}
		}
	}

	if(pstDianFengGlobal->DFFlag & SHOW_DIANFENG_OPEN_LING)
	{
		//"周六19：10时提示" 
		if(pstEnv->pstAppCtx->stCurr.tv_sec - pstDianFengGlobal->RunTimeS >= (5*24*60*60 + 19*60*60+10*60))
		{
			if(!(pstDianFengGlobal->DFFlag & SHOW_DIANFENG_TIS))
			{
				pstDianFengGlobal->RunTimeS = pstEnv->pstAppCtx->stCurr.tv_sec;
				all_login_player_dianfeng_baoming_msg(pstEnv);
				pstDianFengGlobal->DFFlag |= SHOW_DIANFENG_TIS;
			}
		}
	}

	if(pstDianFengGlobal->DFFlag & SHOW_DIANFENG_TIS)
	{
		 //"周六19：20时所有已报名快捷栏上方飘出传送提示框" 
		if(pstEnv->pstAppCtx->stCurr.tv_sec - pstDianFengGlobal->RunTimeS >= (10*60))
		{
			if(!(pstDianFengGlobal->DFFlag & SHOW_DIANFENG_OPEN_TWENTY))
			{
				pstDianFengGlobal->RunTimeS = pstEnv->pstAppCtx->stCurr.tv_sec;
				pstDianFengGlobal->DFFlag |= SHOW_DIANFENG_OPEN_TWENTY;
				pstDianFengGlobal->Status = ACTIVE_READY;
		 		all_player_dianfeng_transmit_dlg(pstEnv,pstDianFengGlobal);
			}
		}
	}

	if(pstDianFengGlobal->DFFlag & SHOW_DIANFENG_OPEN_TWENTY)
	{
		 //"周六19:30未进入准备地方的玩家进行报名队列剔除并开始匹配比赛" 
		if(pstEnv->pstAppCtx->stCurr.tv_sec - pstDianFengGlobal->RunTimeS >= (10*60))
		{
			if(!(pstDianFengGlobal->DFFlag & DIANFENG_DELETE_NO_READY))
			{
				pstDianFengGlobal->RunTimeS = pstEnv->pstAppCtx->stCurr.tv_sec;
				pstDianFengGlobal->DFFlag |= DIANFENG_DELETE_NO_READY;
				pstDianFengGlobal->Status = ACTIVE_PK;
				player_dianfeng_no_reday_del(pstEnv,pstDianFengGlobal);
				all_player_dianfeng_baoming_and_transmit_dlg_close(pstEnv);
			}
		}
	}
	
	if(pstDianFengGlobal->Status == ACTIVE_PK)
	{
		player_dianfeng_pipei(pstEnv,pstDianFengGlobal);
		
		dianfeng_yanchi_tick(pstEnv,pstDianFengGlobal);
	}

	//如果不在报名和匹配中，却在准备和pk中，踢出去
	dianfeng_check_tick(pstEnv,pstDianFengGlobal);
	
	//检查是否结束
	 dianfeng_close_tick(pstEnv,pstDianFengGlobal);

	if(ACTIVE_CLEAN_DATA == pstDianFengGlobal->Status)
	{
		//清空一些数据
		dianfeng_clean_some_data(pstEnv,pstDianFengGlobal,0);
	}

	return 0;
}

int dianfeng_clean_some_data(ZONESVRENV *pstEnv,DIANFENGGLOBAL *pstDianFengGlobal,int type)
{
	DIANFENGJOININFO *pstDianFengZFZJoinInfo = &pstDianFengGlobal->DianFengZFZJoinInfo;
	DIANFENGJOININFO *pstDianFengSHZJoinInfo = &pstDianFengGlobal->DianFengSHZJoinInfo;
	DIANFENGJOININFO *pstDianFengYLSJoinInfo = &pstDianFengGlobal->DianFengYLSJoinInfo;
	DIANFENGJOININFO *pstDianFengSYZJoinInfo = &pstDianFengGlobal->DianFengSYZJoinInfo;
	DIANFENGJOININFO *pstDianFengLSZJoinInfo = &pstDianFengGlobal->DianFengLSZJoinInfo;
	DIANFENGJOININFO *pstDianFengHMZJoinInfo = &pstDianFengGlobal->DianFengHMZJoinInfo;
	DIANFENGJOININFO *pstDianFengLHSJoinInfo = &pstDianFengGlobal->DianFengLHSJoinInfo;
	DIANFENGJOININFO *pstDianFengJNZJoinInfo = &pstDianFengGlobal->DianFengJNZJoinInfo;

	DFFIRSTINFO *pstZFZChampionInfo = &pstDianFengGlobal->DFZFZFirstInfo.DFChampionInfo;             	
	DFFIRSTINFO *pstSHZChampionInfo = &pstDianFengGlobal->DFSHZFirstInfo.DFChampionInfo;  
	DFFIRSTINFO *pstYLSChampionInfo = &pstDianFengGlobal->DFYLSFirstInfo.DFChampionInfo;  
	DFFIRSTINFO *pstSYZChampionInfo = &pstDianFengGlobal->DFSYZFirstInfo.DFChampionInfo;  
	DFFIRSTINFO *pstLSZChampionInfo = &pstDianFengGlobal->DFLSZFirstInfo.DFChampionInfo;  
	DFFIRSTINFO *pstHMZChampionInfo = &pstDianFengGlobal->DFHMZFirstInfo.DFChampionInfo;  
	DFFIRSTINFO *pstLHSChampionInfo = &pstDianFengGlobal->DFLHSFirstInfo.DFChampionInfo;  
	DFFIRSTINFO *pstJNZChampionInfo = &pstDianFengGlobal->DFJNZFirstInfo.DFChampionInfo;  
	
	int i;
	int GMemNum = 0;
	int k;

	memset(pstDianFengZFZJoinInfo,0,sizeof(*pstDianFengZFZJoinInfo));
	memset(pstDianFengSHZJoinInfo,0,sizeof(*pstDianFengSHZJoinInfo));
	memset(pstDianFengYLSJoinInfo,0,sizeof(*pstDianFengYLSJoinInfo));
	memset(pstDianFengSYZJoinInfo,0,sizeof(*pstDianFengSYZJoinInfo));

	memset(pstDianFengLSZJoinInfo,0,sizeof(*pstDianFengLSZJoinInfo));
	memset(pstDianFengHMZJoinInfo,0,sizeof(*pstDianFengHMZJoinInfo));
	memset(pstDianFengLHSJoinInfo,0,sizeof(*pstDianFengLHSJoinInfo));
	memset(pstDianFengJNZJoinInfo,0,sizeof(*pstDianFengJNZJoinInfo));

	GMemNum = pstDianFengGlobal->GuessMemNum;
	for(i=0;i<GMemNum;i++)
	{
		pstDianFengGlobal->GuessInfos[i].GuessRoleName[0] = 0;
		for(k=0;k<MAX_SUPPORT_MEM_NUM;k++)
		{
			pstDianFengGlobal->GuessInfos[i].SupportMem[k].RoleID = 0; 
		}
		pstDianFengGlobal->GuessInfos[i].Num = 0;
	}

	pstDianFengGlobal->GuessMemNum = 0;

	if(type == 1)
	{
		//其余时候不能取消，否则pk完后，又马上开始新一轮了
		pstDianFengGlobal->DFFlag &= ~DIANFENG_OPEN_FLAG;
	}

	pstDianFengGlobal->DFFlag &= ~SHOW_DIANFENG_OPEN_LING;                  	
	pstDianFengGlobal->DFFlag &= ~SHOW_DIANFENG_OPEN_FIRST;                 	
	pstDianFengGlobal->DFFlag &= ~SHOW_DIANFENG_TIS;                        	
	pstDianFengGlobal->DFFlag &= ~SHOW_DIANFENG_OPEN_TWENTY;                
	pstDianFengGlobal->DFFlag &= ~DIANFENG_DELETE_NO_READY;  

	pstDianFengGlobal->Status = ACTIVE_CLOSE;

	pstZFZChampionInfo->RoleID = 0;
	pstSHZChampionInfo->RoleID = 0;
	pstYLSChampionInfo->RoleID = 0;
	pstSYZChampionInfo->RoleID = 0;
	pstLSZChampionInfo->RoleID = 0;
	pstHMZChampionInfo->RoleID = 0;
	pstLHSChampionInfo->RoleID = 0;
	pstJNZChampionInfo->RoleID = 0;
	return 0;
}

int dianfeng_group_champion_record(ZONESVRENV *pstEnv,char *pName,DFFIRSTINFO *pstDFChampionInfo)
{
	Player *pstPlayer = NULL;
	int iMapIndex;
	DIANFENGINFO *pstDianFengInfo = NULL;
	RESPOS stResPos;
	char szText[512];
	char sVar[DLG_STR_LEN];
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;
	DIANFENGJOININFO *pstDianFengJoinInfo = NULL;
	
	pstPlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,pName);
	if(NULL == pstPlayer)
	{
		return -1;
	}
		
	unsigned int uiCID = 0;
	CLANINFO *pstClanInfo = &pstPlayer->stRoleData.MiscInfo.ClanInfo;
	
	if(NULL != pstClanInfo)
	{
		uiCID = pstClanInfo->ClanId.Id;
	}

	//在DianFengGlobal  记住
	pstDFChampionInfo->ClanId = uiCID;
	pstDFChampionInfo->RoleID = pstPlayer->stRoleData.RoleID;
	pstDFChampionInfo->RoleLevel = pstPlayer->stRoleData.Level;
	pstDFChampionInfo->RoleGender = pstPlayer->stRoleData.Gender;
	pstDFChampionInfo->RoleCareer = pstPlayer->stRoleData.Career;
	pstDFChampionInfo->RoleHair = pstPlayer->stRoleData.Hair;
	pstDFChampionInfo->RoleHead = pstPlayer->stRoleData.Head;
	pstDFChampionInfo->RoleFace = pstPlayer->stRoleData.Face;
	STRNCPY(pstDFChampionInfo->RoleName, pstPlayer->stRoleData.RoleName,
		      CCH(pstDFChampionInfo->RoleName));
	memcpy (&pstDFChampionInfo->RoleAvatar, &pstPlayer->stRoleData.AvatarProf, sizeof(pstDFChampionInfo->RoleAvatar));
	pstDFChampionInfo->MapID = 0;
	pstDFChampionInfo->Pos.X = 0;
	pstDFChampionInfo->Pos.Y = 0;
	
	char *pszCareer;
		
	switch(pstPlayer->stRoleData.Career)
	{
		case CAREER_WARRIOR_SINGLEHAND:
			pszCareer = CAREER_NAME_WARRIOR_SINGLEHAND;
			//零之守护者
			player_designation_add(pstEnv, pstPlayer, 131);
			break;

		case CAREER_WARRIOR_DOUBLEHAND:
			pszCareer = CAREER_NAME_WARRIOR_DOUBLEHAND;
			//零之征服者
			player_designation_add(pstEnv, pstPlayer, 130);
			break;

		case CAREER_MAGICOR_SINGLEHAND:
			pszCareer = CAREER_NAME_MAGICOR_SINGLEHAND;
			//零之聚能者
			player_designation_add(pstEnv, pstPlayer, 137);
			break;

		case CAREER_MAGICOR_DOUBLEHAND:
			pszCareer = CAREER_NAME_MAGICOR_DOUBLEHAND;
			//零之炼魂师
			player_designation_add(pstEnv, pstPlayer, 136);
			break;

		case CAREER_GUNER_SINGLEHAND:
			pszCareer = CAREER_NAME_GUNER_SINGLEHAND;
			//零之猎杀者
			player_designation_add(pstEnv, pstPlayer, 135);
			break;

		case CAREER_GUNER_DOUBLEHAND:
			pszCareer = CAREER_NAME_GUNER_DOUBLEHAND;
			//零之毁灭者
			player_designation_add(pstEnv, pstPlayer, 134);
			break;

		case CAREER_CHURCH_SINGLEHAND:
			pszCareer = CAREER_NAME_CHURCH_SINGLEHAND;
			//零之圣佑者
			player_designation_add(pstEnv, pstPlayer, 133);
			break;

		case CAREER_CHURCH_DOUBLEHAND:
			pszCareer = CAREER_NAME_CHURCH_DOUBLEHAND;
			//零之驭雷师
			player_designation_add(pstEnv, pstPlayer, 132);
			break;

		default:
			pszCareer = NULL;
			break;
	}
	
	if (pszCareer)
	{
		STRNCPY(sVar, pszCareer, sizeof(sVar));
	}
	else
	{
		sVar[0] = 0;
	}

	snprintf(szText, sizeof(szText),LJY_SYSMSG24,pstPlayer->stRoleData.RoleName,sVar);
	
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SHOW, ZONEERR_DIANFENG16,szText);

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR ,ZONEERR_DIANFENG15,pstPlayer->stRoleData.RoleName);
	
	shopping_add_bulletin(pstEnv->pstAppCtx,pstEnv,300,60, szText);
	
	pstDianFengInfo = &pstPlayer->stRoleData.MiscInfo.DianFengInfo;

	iMapIndex = pstDianFengInfo->MapId;
	stResPos.X = pstDianFengInfo->ResPos.X;
	stResPos.Y = pstDianFengInfo->ResPos.Y;

	pstDianFengInfo->DianFengFlag |= IF_DIANFENG_ONE_PK;
	
	if(0 > dianfeng_get_joininfo_by_career(pstEnv,&pstDianFengJoinInfo,pstDianFengGlobal,pstPlayer->stRoleData.Career))
		return -1;
	
	//把报名队列清零，防止踢出时,判断是自动放弃的
	pstDianFengJoinInfo->MemNum = 0;
	
	//踢出
	z_dianfeng_change_map(pstEnv, pstPlayer,iMapIndex, stResPos ,0);

	dianfeng_revive_sth(pstEnv,pstPlayer);
 
	return 0;
}

//记录冠军信息
int	dianfeng_champion_record(ZONESVRENV *pstEnv,char *pName,int iWhichJob)
{
	DFFIRSTINFO *pstDFChampionInfo = NULL;
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;

	switch(iWhichJob)
	{
		case DIANFENG_JOB_ZFZ:
			pstDFChampionInfo = &pstDianFengGlobal->DFZFZFirstInfo.DFChampionInfo;
			break;
		case DIANFENG_JOB_SHZ:
			pstDFChampionInfo = &pstDianFengGlobal->DFSHZFirstInfo.DFChampionInfo;
			break;
		case DIANFENG_JOB_YLS:
			pstDFChampionInfo = &pstDianFengGlobal->DFYLSFirstInfo.DFChampionInfo;
			break;
		case DIANFENG_JOB_SYZ:
			pstDFChampionInfo = &pstDianFengGlobal->DFSYZFirstInfo.DFChampionInfo;
			break;
		case DIANFENG_JOB_LSZ:
			pstDFChampionInfo = &pstDianFengGlobal->DFLSZFirstInfo.DFChampionInfo;
			break;
		case DIANFENG_JOB_HMZ:
			pstDFChampionInfo = &pstDianFengGlobal->DFHMZFirstInfo.DFChampionInfo;
			break;
		case DIANFENG_JOB_LHS:
			pstDFChampionInfo = &pstDianFengGlobal->DFLHSFirstInfo.DFChampionInfo;
			break;
		case DIANFENG_JOB_JNZ:
			pstDFChampionInfo = &pstDianFengGlobal->DFJNZFirstInfo.DFChampionInfo;
			break;
		default:
			return -1;
			break;
			
	}

	if(NULL == pstDFChampionInfo)
	{
		return -1;
	}
	
	if(pstDFChampionInfo->RoleID == 0)
	{
		if(0 > dianfeng_group_champion_record(pstEnv,pName,pstDFChampionInfo))
		{
			return -1;
		}
	}
	
	return 0;
}

//0  表示该组产生冠军，-1  表示没
int dianfeng_one_group_first_judge(ZONESVRENV *pstEnv,DIANFENGJOININFO *pstDianFengJoinInfo,int iWhichJob)
{
	if(NULL == pstDianFengJoinInfo)
	{
		return -1;
	}
	
	DIANFENGPIPEI *pstDianFengPiPei = &pstDianFengJoinInfo->DianFengPiPei;
	
	int MemNum = pstDianFengJoinInfo->MemNum;

	if(MemNum == 1 && pstDianFengPiPei->PiPeiNum == 0)
	{		
		//记录冠军信息
		if(0 == dianfeng_champion_record(pstEnv,pstDianFengJoinInfo->GroupInfo[MemNum-1].RoleName,iWhichJob))
		{
			return 0;
		}
	}

	//该组没人参加 或者都掉线了
	if(MemNum == 0 && pstDianFengPiPei->PiPeiNum == 0)
	{
		return 0;
	}

	return -1;
}

//0 表示参加的组都产生冠军。-1 表示还没全部产生
int dianfeng_group_first_if_all_produce(ZONESVRENV *pstEnv,DIANFENGGLOBAL *pstDianFengGlobal)
{
	DIANFENGJOININFO *pstDianFengZFZJoinInfo = &pstDianFengGlobal->DianFengZFZJoinInfo;
	DIANFENGJOININFO *pstDianFengSHZJoinInfo = &pstDianFengGlobal->DianFengSHZJoinInfo;
	DIANFENGJOININFO *pstDianFengYLSJoinInfo = &pstDianFengGlobal->DianFengYLSJoinInfo;
	DIANFENGJOININFO *pstDianFengSYZJoinInfo = &pstDianFengGlobal->DianFengSYZJoinInfo;
	DIANFENGJOININFO *pstDianFengLSZJoinInfo = &pstDianFengGlobal->DianFengLSZJoinInfo;
	DIANFENGJOININFO *pstDianFengHMZJoinInfo = &pstDianFengGlobal->DianFengHMZJoinInfo;
	DIANFENGJOININFO *pstDianFengLHSJoinInfo = &pstDianFengGlobal->DianFengLHSJoinInfo;
	DIANFENGJOININFO *pstDianFengJNZJoinInfo = &pstDianFengGlobal->DianFengJNZJoinInfo;

	int res = 0;
	
	if(0 > dianfeng_one_group_first_judge(pstEnv,pstDianFengZFZJoinInfo,DIANFENG_JOB_ZFZ))
	{
		res = -1;
	}
	
	if(0 > dianfeng_one_group_first_judge(pstEnv,pstDianFengSHZJoinInfo,DIANFENG_JOB_SHZ))
	{
		res = -1;
	}
	
	if(0 > dianfeng_one_group_first_judge(pstEnv,pstDianFengYLSJoinInfo,DIANFENG_JOB_YLS))
	{
		res = -1;
	}
	
	if(0 > dianfeng_one_group_first_judge(pstEnv,pstDianFengSYZJoinInfo,DIANFENG_JOB_SYZ))
	{
		res = -1;
	}
	
	if(0 > dianfeng_one_group_first_judge(pstEnv,pstDianFengLSZJoinInfo,DIANFENG_JOB_LSZ))
	{
		res = -1;
	}
	
	if(0 > dianfeng_one_group_first_judge(pstEnv,pstDianFengHMZJoinInfo,DIANFENG_JOB_HMZ))
	{
		res = -1;
	}
	
	if(0 > dianfeng_one_group_first_judge(pstEnv,pstDianFengLHSJoinInfo,DIANFENG_JOB_LHS))	
	{
		res = -1;
	}
	
	if(0 > dianfeng_one_group_first_judge(pstEnv,pstDianFengJNZJoinInfo,DIANFENG_JOB_JNZ))
	{
		res = -1;
	}
	
	return res;
}

int dianfeng_close_tick(ZONESVRENV *pstEnv,DIANFENGGLOBAL *pstDianFengGlobal)
{
	int num = 0;
	tdr_ulonglong ullMoney = 0;

	if(pstDianFengGlobal->Status == ACTIVE_PK)
	{
		//每一个参赛的组都决出冠军的时候
		if(0 == dianfeng_group_first_if_all_produce(pstEnv,pstDianFengGlobal))
		{
			//中奖人数
			num = dianfeng_ret_guess_win_num(pstEnv,pstDianFengGlobal);
			if(num > 0)
			{
				//中者奖励
				ullMoney = MOSHI_DIANFENG_JINGCAI_MONEY+(pstDianFengGlobal->AllMoneyNum)/num; 
			}
			else if(num == 0)
			{
				ullMoney = pstDianFengGlobal->AllMoneyNum;
			}
			else
			{
				return -1;
			}

			//给参与竞猜的发邮件奖励
			player_dianfeng_send_mail_award(pstEnv,num,ullMoney,pstDianFengGlobal);

			if(num > 0)
			{
				pstDianFengGlobal->AllMoneyNum = 0;
			}
			pstDianFengGlobal->Status = ACTIVE_CLEAN_DATA;
		}
	}
	
	return 0;
}

//判断当前玩家是否在准备地图里面
int player_dianfeng_curr_map_same_as_reday_map(ZONESVRENV *pstEnv,Player *pstMemberPlayer)
{
	int currmap = 0;
	int iMapIndex = 0;

	currmap = pstMemberPlayer->stRoleData.Map;

	if(0 > dianfeng_get_ready_map_by_career(pstEnv,pstMemberPlayer->stRoleData.Career,&iMapIndex))
	{
		return -1;
	}
	
	if(iMapIndex != currmap)
	{
		return -1;
	}
	return 0;
}

int player_dianfeng_no_ready_del_group(ZONESVRENV *pstEnv,DIANFENGJOININFO *pstDianFengJoinInfo)
{	
	int i;
	int iMemNum = 0;
	GROUPINFO *pstGroupInfo = NULL;
	Player *pstMemberPlayer = NULL;	

	if(NULL == pstDianFengJoinInfo)
	{
		return 0;
	}

	if(pstDianFengJoinInfo->MemNum <= 0)
	{
		return 0;
	}
	
	iMemNum = min(pstDianFengJoinInfo->MemNum,MAX_DIANFENG_EVERY_GROUP_MEM_NUM);
	
	for(i=iMemNum-1;i>=0;i--)
	{
		pstGroupInfo = &pstDianFengJoinInfo->GroupInfo[i];
		if(NULL == pstGroupInfo)
		{
			continue;
		}

		pstMemberPlayer =  z_name_player(pstEnv->pstAppCtx, pstEnv,pstGroupInfo->RoleName);
		if(NULL == pstMemberPlayer)
		{
			//不在线的，直接去除
			pstDianFengJoinInfo->MemNum--;
			if(i!=pstDianFengJoinInfo->MemNum)
			{
				memcpy(&pstDianFengJoinInfo->GroupInfo[i],&pstDianFengJoinInfo->GroupInfo[pstDianFengJoinInfo->MemNum],sizeof(pstDianFengJoinInfo->GroupInfo[i]));
			}
			continue;
		}
		
		if(0 > player_dianfeng_curr_map_same_as_reday_map(pstEnv,pstMemberPlayer))
		{
			//在线的，但传送不成功的，去除
			pstDianFengJoinInfo->MemNum--;
			if(i!=pstDianFengJoinInfo->MemNum)
			{
				memcpy(&pstDianFengJoinInfo->GroupInfo[i],&pstDianFengJoinInfo->GroupInfo[pstDianFengJoinInfo->MemNum],sizeof(pstDianFengJoinInfo->GroupInfo[i]));
			}

			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstMemberPlayer, SYS_MIDDLE , ZONEERR_DIANFENG6);
		}

	}
	
	return 0;
}

int player_dianfeng_no_reday_del(ZONESVRENV *pstEnv,DIANFENGGLOBAL *pstDianFengGlobal)
{
	DIANFENGJOININFO *pstDianFengZFZJoinInfo = &pstDianFengGlobal->DianFengZFZJoinInfo;
	DIANFENGJOININFO *pstDianFengSHZJoinInfo = &pstDianFengGlobal->DianFengSHZJoinInfo;
	DIANFENGJOININFO *pstDianFengYLSJoinInfo = &pstDianFengGlobal->DianFengYLSJoinInfo;
	DIANFENGJOININFO *pstDianFengSYZJoinInfo = &pstDianFengGlobal->DianFengSYZJoinInfo;
	DIANFENGJOININFO *pstDianFengLSZJoinInfo = &pstDianFengGlobal->DianFengLSZJoinInfo;
	DIANFENGJOININFO *pstDianFengHMZJoinInfo = &pstDianFengGlobal->DianFengHMZJoinInfo;
	DIANFENGJOININFO *pstDianFengLHSJoinInfo = &pstDianFengGlobal->DianFengLHSJoinInfo;
	DIANFENGJOININFO *pstDianFengJNZJoinInfo = &pstDianFengGlobal->DianFengJNZJoinInfo;

	player_dianfeng_no_ready_del_group(pstEnv,pstDianFengZFZJoinInfo);
	player_dianfeng_no_ready_del_group(pstEnv,pstDianFengSHZJoinInfo);
	player_dianfeng_no_ready_del_group(pstEnv,pstDianFengYLSJoinInfo);
	player_dianfeng_no_ready_del_group(pstEnv,pstDianFengSYZJoinInfo);
	player_dianfeng_no_ready_del_group(pstEnv,pstDianFengLSZJoinInfo);
	player_dianfeng_no_ready_del_group(pstEnv,pstDianFengHMZJoinInfo);
	player_dianfeng_no_ready_del_group(pstEnv,pstDianFengLHSJoinInfo);
	player_dianfeng_no_ready_del_group(pstEnv,pstDianFengJNZJoinInfo);
	
	return 0;
}

int player_dianfeng_pipei(ZONESVRENV *pstEnv,DIANFENGGLOBAL *pstDianFengGlobal)
{
	DIANFENGJOININFO *pstDianFengZFZJoinInfo = &pstDianFengGlobal->DianFengZFZJoinInfo;
	DIANFENGJOININFO *pstDianFengSHZJoinInfo = &pstDianFengGlobal->DianFengSHZJoinInfo;
	DIANFENGJOININFO *pstDianFengYLSJoinInfo = &pstDianFengGlobal->DianFengYLSJoinInfo;
	DIANFENGJOININFO *pstDianFengSYZJoinInfo = &pstDianFengGlobal->DianFengSYZJoinInfo;
	DIANFENGJOININFO *pstDianFengLSZJoinInfo = &pstDianFengGlobal->DianFengLSZJoinInfo;
	DIANFENGJOININFO *pstDianFengHMZJoinInfo = &pstDianFengGlobal->DianFengHMZJoinInfo;
	DIANFENGJOININFO *pstDianFengLHSJoinInfo = &pstDianFengGlobal->DianFengLHSJoinInfo;
	DIANFENGJOININFO *pstDianFengJNZJoinInfo = &pstDianFengGlobal->DianFengJNZJoinInfo;

	int i;
	for(i=0;i<10;i++)
	{
		player_dianfeng_group_pipei(pstEnv,pstDianFengZFZJoinInfo);
		player_dianfeng_group_pipei(pstEnv,pstDianFengSHZJoinInfo);
		player_dianfeng_group_pipei(pstEnv,pstDianFengYLSJoinInfo);
		player_dianfeng_group_pipei(pstEnv,pstDianFengSYZJoinInfo);
		player_dianfeng_group_pipei(pstEnv,pstDianFengLSZJoinInfo);
		player_dianfeng_group_pipei(pstEnv,pstDianFengHMZJoinInfo);
		player_dianfeng_group_pipei(pstEnv,pstDianFengLHSJoinInfo);
		player_dianfeng_group_pipei(pstEnv,pstDianFengJNZJoinInfo);
	}
	
	return 0;
}


int all_login_player_dianfeng_baoming_dlg(ZONESVRENV *pstEnv)
{
	int i = 0;
	for (i=0; i<MAX_PLAYER; i++)
	{
		TMEMBLOCK* pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		Player * pstPlayer = NULL;
		
		DIANFENGINFO *pstDianFengInfo = NULL;
		
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayer = (Player *)pstMemBlock->szData;		
		
		if(PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus ) 
		{
			pstDianFengInfo = &pstPlayer->stRoleData.MiscInfo.DianFengInfo;
			if(!(pstDianFengInfo->DianFengFlag & IF_DIANFENG_BAOMING))
			{
				player_baoming_res_to_clt(pstEnv, pstPlayer,0);
			}
		}
	}
	return 0;
}

int all_player_dianfeng_baoming_and_transmit_dlg_close(ZONESVRENV *pstEnv)
{
	int i = 0;
	for (i=0; i<MAX_PLAYER; i++)
	{
		TMEMBLOCK* pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		Player * pstPlayer = NULL;
			
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayer = (Player *)pstMemBlock->szData;		
		
		if((PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus) ||(PLAYER_STATUS_ROLE_NOTHING == pstPlayer->eStatus)) 
		{
			//通知客户端关闭报名框了，假如还有打开的话
			player_baoming_res_to_clt(pstEnv, pstPlayer,3);
			//关闭传送框
			player_transmit_res_to_clt(pstEnv, pstPlayer,3);
		}
	}
	return 0;
}


int all_login_player_dianfeng_baoming_msg(ZONESVRENV *pstEnv)
{
	int i = 0;
	for (i=0; i<MAX_PLAYER; i++)
	{
		TMEMBLOCK* pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		Player * pstPlayer = NULL;
		
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayer = (Player *)pstMemBlock->szData;		
		
		if((PLAYER_STATUS_ROLE_LOGIN == pstPlayer->eStatus) ||(PLAYER_STATUS_ROLE_NOTHING == pstPlayer->eStatus)) 
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP , ZONEERR_DIANFENG7);
		}
	}
	return 0;
}


int all_player_dianfeng_transmit_dlg(ZONESVRENV *pstEnv,DIANFENGGLOBAL *pstDianFengGlobal)
{
	if(pstDianFengGlobal->Status != ACTIVE_READY)
	{
		return -1;
	}
	
	DIANFENGJOININFO *pstDianFengZFZJoinInfo = &pstDianFengGlobal->DianFengZFZJoinInfo;
	DIANFENGJOININFO *pstDianFengSHZJoinInfo = &pstDianFengGlobal->DianFengSHZJoinInfo;
	DIANFENGJOININFO *pstDianFengYLSJoinInfo = &pstDianFengGlobal->DianFengYLSJoinInfo;
	DIANFENGJOININFO *pstDianFengSYZJoinInfo = &pstDianFengGlobal->DianFengSYZJoinInfo;
	DIANFENGJOININFO *pstDianFengLSZJoinInfo = &pstDianFengGlobal->DianFengLSZJoinInfo;
	DIANFENGJOININFO *pstDianFengHMZJoinInfo = &pstDianFengGlobal->DianFengHMZJoinInfo;
	DIANFENGJOININFO *pstDianFengLHSJoinInfo = &pstDianFengGlobal->DianFengLHSJoinInfo;
	DIANFENGJOININFO *pstDianFengJNZJoinInfo = &pstDianFengGlobal->DianFengJNZJoinInfo;

	player_dianfeng_join_transmit_dlg(pstEnv,pstDianFengZFZJoinInfo);
	player_dianfeng_join_transmit_dlg(pstEnv,pstDianFengSHZJoinInfo);
	player_dianfeng_join_transmit_dlg(pstEnv,pstDianFengYLSJoinInfo);
	player_dianfeng_join_transmit_dlg(pstEnv,pstDianFengSYZJoinInfo);
	player_dianfeng_join_transmit_dlg(pstEnv,pstDianFengLSZJoinInfo);
	player_dianfeng_join_transmit_dlg(pstEnv,pstDianFengHMZJoinInfo);
	player_dianfeng_join_transmit_dlg(pstEnv,pstDianFengLHSJoinInfo);
	player_dianfeng_join_transmit_dlg(pstEnv,pstDianFengJNZJoinInfo);
	return 0;
}


int player_dianfeng_join_transmit_dlg(ZONESVRENV *pstEnv,DIANFENGJOININFO *pstDianFengJoinInfo)
{	
	int i;
	int iMemNum = 0;
	GROUPINFO *pstGroupInfo = NULL;
	Player *pstMemberPlayer = NULL;

	if(NULL == pstDianFengJoinInfo)
	{
		return 0;
	}

	if(pstDianFengJoinInfo->MemNum <= 0)
	{
		return 0;
	}
	
	iMemNum = min(pstDianFengJoinInfo->MemNum,MAX_DIANFENG_EVERY_GROUP_MEM_NUM);
	
	for(i=0;i<iMemNum;i++)
	{
		pstGroupInfo = &pstDianFengJoinInfo->GroupInfo[i];
		if(NULL == pstGroupInfo)
		{
			continue;
		}

		pstMemberPlayer =  z_name_player(pstEnv->pstAppCtx, pstEnv,pstGroupInfo->RoleName);
		if(NULL == pstMemberPlayer)
		{
			continue;
		}

		player_transmit_res_to_clt(pstEnv, pstMemberPlayer,0);
	}
	
	return 0;
}

int player_dianfeng_login_transmit_dlg(ZONESVRENV *pstEnv,Player *pstPlayer,DIANFENGGLOBAL *pstDianFengGlobal)
{
	if(pstDianFengGlobal->Status != ACTIVE_READY)
	{
		return -1;
	}

	DIANFENGJOININFO *pstDianFengJoinInfo = NULL;	
	int i;
	int iMemNum = 0;
	GROUPINFO *pstGroupInfo = NULL;
	
	if(0 > dianfeng_get_joininfo_by_career(pstEnv,&pstDianFengJoinInfo,pstDianFengGlobal,pstPlayer->stRoleData.Career))
		return -1;

	if(NULL == pstDianFengJoinInfo)
	{
		return 0;
	}

	if(pstDianFengJoinInfo->MemNum <= 0)
	{
		return 0;
	}
	
	iMemNum = min(pstDianFengJoinInfo->MemNum,MAX_DIANFENG_EVERY_GROUP_MEM_NUM);
	
	for(i=0;i<iMemNum;i++)
	{
		pstGroupInfo = &pstDianFengJoinInfo->GroupInfo[i];
		if(NULL == pstGroupInfo)
		{
			continue;
		}

		if(0 == strcmp(pstPlayer->stRoleData.RoleName,pstGroupInfo->RoleName))
		{
			player_transmit_res_to_clt(pstEnv, pstPlayer,0);
			break;
		}
	}
	
	return 0;
}

//设置默认的雕像地图和坐标
int dianfeng_effigy_login_set(ZONESVRENV *pstEnv,DFFIRSTINFO *pstDFEffigyInfo,int iWhichJob)
{	
	if(pstDFEffigyInfo == NULL)
	{
		return 0;
	}
	
	if(0 == pstDFEffigyInfo->RoleID)
	{
		RESPOS stPos;
		//默认的
		pstDFEffigyInfo->MapID = 1000;
		
		switch(iWhichJob)
		{
		case DIANFENG_JOB_ZFZ:
			stPos.X = 27660;
			stPos.Y = 20909;
			break;
		case DIANFENG_JOB_SHZ:
			stPos.X = 25482;
			stPos.Y = 20909;
			break;
		case DIANFENG_JOB_YLS:
			stPos.X = 27662;
			stPos.Y = 17818;
			break;
		case DIANFENG_JOB_SYZ:
			stPos.X = 25482;
			stPos.Y = 17813;
			break;
		case DIANFENG_JOB_LSZ:
			stPos.X = 27658;
			stPos.Y = 19859;
			break;
		case DIANFENG_JOB_HMZ:
			stPos.X = 25482;
			stPos.Y = 19858;
			break;
		case DIANFENG_JOB_LHS:
			stPos.X = 27660;
			stPos.Y = 18868;
			break;
		case DIANFENG_JOB_JNZ:
			stPos.X = 25482;
			stPos.Y = 18867;
			break;
		default:
			return -1;
			break;
		}

		memcpy(&pstDFEffigyInfo->Pos,&stPos,sizeof(pstDFEffigyInfo->Pos));
	}
	
	return 0;
}

// 登录的时候。发雕塑信息给clt	，默认的也要发
int player_login_effigy_send_to_clt(ZONESVRENV* pstEnv,Player *pstPlayer,DIANFENGGLOBAL *pstDianFengGlobal)
{		
	DFFIRSTINFO *pstDFZFZEffigyInfo = &pstDianFengGlobal->DFZFZFirstInfo.DFEffigyInfo;
	DFFIRSTINFO *pstDFSHZEffigyInfo = &pstDianFengGlobal->DFSHZFirstInfo.DFEffigyInfo;
	DFFIRSTINFO *pstDFYLSEffigyInfo = &pstDianFengGlobal->DFYLSFirstInfo.DFEffigyInfo;
	DFFIRSTINFO *pstDFSYZEffigyInfo = &pstDianFengGlobal->DFSYZFirstInfo.DFEffigyInfo;
	DFFIRSTINFO *pstDFLSZEffigyInfo = &pstDianFengGlobal->DFLSZFirstInfo.DFEffigyInfo;
	DFFIRSTINFO *pstDFHMZEffigyInfo = &pstDianFengGlobal->DFHMZFirstInfo.DFEffigyInfo;
	DFFIRSTINFO *pstDFLHSEffigyInfo = &pstDianFengGlobal->DFLHSFirstInfo.DFEffigyInfo;
	DFFIRSTINFO *pstDFJNZEffigyInfo = &pstDianFengGlobal->DFJNZFirstInfo.DFEffigyInfo;
		
	dianfeng_effigy_login_set(pstEnv,pstDFZFZEffigyInfo,DIANFENG_JOB_ZFZ);
	dianfeng_effigy_login_set(pstEnv,pstDFSHZEffigyInfo,DIANFENG_JOB_SHZ);
	dianfeng_effigy_login_set(pstEnv,pstDFYLSEffigyInfo,DIANFENG_JOB_YLS);
	dianfeng_effigy_login_set(pstEnv,pstDFSYZEffigyInfo,DIANFENG_JOB_SYZ);
	dianfeng_effigy_login_set(pstEnv,pstDFLSZEffigyInfo,DIANFENG_JOB_LSZ);
	dianfeng_effigy_login_set(pstEnv,pstDFHMZEffigyInfo,DIANFENG_JOB_HMZ);
	dianfeng_effigy_login_set(pstEnv,pstDFLHSEffigyInfo,DIANFENG_JOB_LHS);
	dianfeng_effigy_login_set(pstEnv,pstDFJNZEffigyInfo,DIANFENG_JOB_JNZ);
	
	player_first_or_effigy_info_to_clt(pstEnv, pstPlayer,FIRST_EFFIGY);
	return 0;
}

//	周六当天第一次上线的时候，发送巅峰开启提示窗
int player_dianfeng_login_check(ZONESVRENV* pstEnv,Player *pstPlayer)
{	
	if(pstEnv->pstConf->DianFengFlag == 0 || (is_fini_global(pstEnv) == 0) || get_curr_max_level(pstEnv) < 65)
	{
		return 0;
	}
	
	time_t	t = pstEnv->pstAppCtx->stCurr.tv_sec;
	struct tm *pstTm;
	pstTm = localtime(&t);	
	DIANFENGINFO *pstDianFengInfo = &pstPlayer->stRoleData.MiscInfo.DianFengInfo;
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;
	
	if(pstDianFengGlobal->Status == ACTIVE_OPEN || pstDianFengGlobal->Status == ACTIVE_READY)
	{
		if(pstDianFengInfo->JoinTime != 0 && pstDianFengInfo->JoinTime != pstDianFengGlobal->ActiveTimeS)
		{
			pstDianFengInfo->LoginOutNum = 0;
			pstDianFengInfo->DianFengFlag = 0;
			pstDianFengInfo->PkOutNum = 0;
			pstDianFengInfo->JoinTime = pstDianFengGlobal->ActiveTimeS;
		}

		if(pstTm->tm_wday == 6)
		{
			if(!(pstDianFengInfo->DianFengFlag & IF_DIANFENG_BAOMING))
			{
				player_baoming_res_to_clt(pstEnv, pstPlayer,0);
			}
		}
	}

	//周六19：20   ~ 19:30     已报名快捷栏上方飘出传送提示框
	player_dianfeng_login_transmit_dlg(pstEnv,pstPlayer,pstDianFengGlobal);

	// 登录的时候。发雕塑信息给clt  ，默认的也要发
	player_login_effigy_send_to_clt(pstEnv, pstPlayer,pstDianFengGlobal);

	//登录的时候，检查是否还在报名和匹配的队列中
	player_login_baoming_pipei_queue_check(pstEnv,pstPlayer,pstDianFengGlobal);
	
	return 0;
}

//设置冠军   雕像
int dianfeng_effigy_set(ZONESVRENV *pstEnv, Player *pstPlayer, int iMap, RESPOS *pstPos,int WhichOne)
{
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;
	DFFIRSTINFO stDFEffigyInfo;
	memset(&stDFEffigyInfo,0,sizeof(stDFEffigyInfo));
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	CSDIANFENGFIRSTINFO *pstDianFengFirstInfo = &pstMiscSvr->PlayerMiscSvrData.DianFengFirstInfo;
	memset(pstDianFengFirstInfo,0,sizeof(*pstDianFengFirstInfo));
	
	DFFIRSTINFO stCSDFEffigyInfo;
	memset(&stCSDFEffigyInfo,0,sizeof(stCSDFEffigyInfo));

	pstMiscSvr->PlayerMiscType = PLAYER_DIANFENG_FIRST_INFO;

	pstDianFengFirstInfo->Type = FIRST_EFFIGY;
	pstDianFengFirstInfo->WhichOne = WhichOne;

	player_dianfeng_effigy_set_data(pstEnv, pstPlayer,&stDFEffigyInfo,&stCSDFEffigyInfo,iMap, pstPos);

	switch(WhichOne)
	{
		case DIANFENG_JOB_ZFZ:
			memcpy(&pstDianFengGlobal->DFZFZFirstInfo.DFEffigyInfo,&stDFEffigyInfo,sizeof(pstDianFengGlobal->DFZFZFirstInfo.DFEffigyInfo));
			memcpy(&pstDianFengFirstInfo->DFZFZFirstInfo.DFEffigyInfo,&stCSDFEffigyInfo,sizeof(pstDianFengFirstInfo->DFZFZFirstInfo.DFEffigyInfo));
			break;
		case DIANFENG_JOB_SHZ:	
			memcpy(&pstDianFengGlobal->DFSHZFirstInfo.DFEffigyInfo,&stDFEffigyInfo,sizeof(pstDianFengGlobal->DFSHZFirstInfo.DFEffigyInfo));
			memcpy(&pstDianFengFirstInfo->DFSHZFirstInfo.DFEffigyInfo,&stCSDFEffigyInfo,sizeof(pstDianFengFirstInfo->DFSHZFirstInfo.DFEffigyInfo));
			break;
		case DIANFENG_JOB_YLS:
			memcpy(&pstDianFengGlobal->DFYLSFirstInfo.DFEffigyInfo,&stDFEffigyInfo,sizeof(pstDianFengGlobal->DFYLSFirstInfo.DFEffigyInfo));
			memcpy(&pstDianFengFirstInfo->DFYLSFirstInfo.DFEffigyInfo,&stCSDFEffigyInfo,sizeof(pstDianFengFirstInfo->DFYLSFirstInfo.DFEffigyInfo));
			break;
		case DIANFENG_JOB_SYZ:	
			memcpy(&pstDianFengGlobal->DFSYZFirstInfo.DFEffigyInfo,&stDFEffigyInfo,sizeof(pstDianFengGlobal->DFSYZFirstInfo.DFEffigyInfo));
			memcpy(&pstDianFengFirstInfo->DFSYZFirstInfo.DFEffigyInfo,&stCSDFEffigyInfo,sizeof(pstDianFengFirstInfo->DFSYZFirstInfo.DFEffigyInfo));
			break;
		case DIANFENG_JOB_LSZ:
			memcpy(&pstDianFengGlobal->DFLSZFirstInfo.DFEffigyInfo,&stDFEffigyInfo,sizeof(pstDianFengGlobal->DFLSZFirstInfo.DFEffigyInfo));
			memcpy(&pstDianFengFirstInfo->DFLSZFirstInfo.DFEffigyInfo,&stCSDFEffigyInfo,sizeof(pstDianFengFirstInfo->DFLSZFirstInfo.DFEffigyInfo));
			break;
		case DIANFENG_JOB_HMZ:
			memcpy(&pstDianFengGlobal->DFHMZFirstInfo.DFEffigyInfo,&stDFEffigyInfo,sizeof(pstDianFengGlobal->DFHMZFirstInfo.DFEffigyInfo));
			memcpy(&pstDianFengFirstInfo->DFHMZFirstInfo.DFEffigyInfo,&stCSDFEffigyInfo,sizeof(pstDianFengFirstInfo->DFHMZFirstInfo.DFEffigyInfo));
			break;
		case DIANFENG_JOB_LHS:
			memcpy(&pstDianFengGlobal->DFLHSFirstInfo.DFEffigyInfo,&stDFEffigyInfo,sizeof(pstDianFengGlobal->DFLHSFirstInfo.DFEffigyInfo));
			memcpy(&pstDianFengFirstInfo->DFLHSFirstInfo.DFEffigyInfo,&stCSDFEffigyInfo,sizeof(pstDianFengFirstInfo->DFLHSFirstInfo.DFEffigyInfo));
			break;
		case DIANFENG_JOB_JNZ:	
			memcpy(&pstDianFengGlobal->DFJNZFirstInfo.DFEffigyInfo,&stDFEffigyInfo,sizeof(pstDianFengGlobal->DFJNZFirstInfo.DFEffigyInfo));
			memcpy(&pstDianFengFirstInfo->DFJNZFirstInfo.DFEffigyInfo,&stCSDFEffigyInfo,sizeof(pstDianFengFirstInfo->DFJNZFirstInfo.DFEffigyInfo));
			break;	
		default:
			return -1;
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	return z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);
}

int player_dianfeng_effigy_set_data(ZONESVRENV *pstEnv, Player *pstPlayer,DFFIRSTINFO *pstDFEffigyInfo,DFFIRSTINFO *pstCSDFEffigyInfo,int iMap, RESPOS *pstPos)
{
	unsigned int uiCID = 0;
	CLANINFO *pstClanInfo = &pstPlayer->stRoleData.MiscInfo.ClanInfo;
	
	if(NULL != pstClanInfo)
	{
		uiCID = pstClanInfo->ClanId.Id;
	}

	//在DianFengGlobal  记住
	pstDFEffigyInfo->ClanId = uiCID;
	pstDFEffigyInfo->RoleID = pstPlayer->stRoleData.RoleID;
	pstDFEffigyInfo->RoleLevel = pstPlayer->stRoleData.Level;
	pstDFEffigyInfo->RoleGender = pstPlayer->stRoleData.Gender;
	pstDFEffigyInfo->RoleCareer = pstPlayer->stRoleData.Career;
	pstDFEffigyInfo->RoleHair = pstPlayer->stRoleData.Hair;
	pstDFEffigyInfo->RoleHead = pstPlayer->stRoleData.Head;
	pstDFEffigyInfo->RoleFace = pstPlayer->stRoleData.Face;
	STRNCPY(pstDFEffigyInfo->RoleName, pstPlayer->stRoleData.RoleName,
		      CCH(pstDFEffigyInfo->RoleName));
	memcpy (&pstDFEffigyInfo->RoleAvatar, &pstPlayer->stRoleData.AvatarProf, sizeof(pstDFEffigyInfo->RoleAvatar));
	pstDFEffigyInfo->MapID = iMap;
	pstDFEffigyInfo->Pos = *pstPos;

	//需要通知客户端的
	pstCSDFEffigyInfo->ClanId = uiCID;
	pstCSDFEffigyInfo->RoleID = pstPlayer->stRoleData.RoleID;
	pstCSDFEffigyInfo->RoleLevel = pstPlayer->stRoleData.Level;
	pstCSDFEffigyInfo->RoleGender = pstPlayer->stRoleData.Gender;
	pstCSDFEffigyInfo->RoleCareer = pstPlayer->stRoleData.Career;
	pstCSDFEffigyInfo->RoleHair = pstPlayer->stRoleData.Hair;
	pstCSDFEffigyInfo->RoleHead = pstPlayer->stRoleData.Head;
	pstCSDFEffigyInfo->RoleFace = pstPlayer->stRoleData.Face;
	STRNCPY(pstCSDFEffigyInfo->RoleName, pstPlayer->stRoleData.RoleName,
		      CCH(pstCSDFEffigyInfo->RoleName));
	memcpy (&pstCSDFEffigyInfo->RoleAvatar, &pstPlayer->stRoleData.AvatarProf, sizeof(pstCSDFEffigyInfo->RoleAvatar));
	pstCSDFEffigyInfo->MapID = iMap;
	pstCSDFEffigyInfo->Pos = *pstPos;
		
	return 0;
}

//返回竞猜中，中奖的人数
int dianfeng_ret_guess_win_num(ZONESVRENV *pstEnv,DIANFENGGLOBAL *pstDianFengGlobal)
{
	int iRet = 0;
	int iGuessNum = pstDianFengGlobal->GuessMemNum;
	GUESSINFOS *pstGuessInfos = NULL;
	DFFIRSTINFO *pstDFChampionInfo = NULL;
	int i;
	int k;
	int w = 0;
		
	for(i=0;i<iGuessNum;i++)
	{
		pstGuessInfos = &pstDianFengGlobal->GuessInfos[i];
		if(pstGuessInfos == NULL)
		{
			continue;
		}
		
		for(k=0;k<MAX_SUPPORT_MEM_NUM;k++)
		{
			w = k+1;
			//客户端发给服务器存储时，必须按这样的顺序来，少一些比较
			if(w == DIANFENG_JOB_ZFZ)
			{
				pstDFChampionInfo = &pstDianFengGlobal->DFZFZFirstInfo.DFChampionInfo;
			}
			else if(w == DIANFENG_JOB_SHZ)
			{
				pstDFChampionInfo = &pstDianFengGlobal->DFSHZFirstInfo.DFChampionInfo;
			}
			else if(w == DIANFENG_JOB_YLS)
			{
                pstDFChampionInfo = &pstDianFengGlobal->DFYLSFirstInfo.DFChampionInfo;
			}
			else if(w == DIANFENG_JOB_SYZ)
			{
				pstDFChampionInfo = &pstDianFengGlobal->DFSYZFirstInfo.DFChampionInfo;
			}
			else if(w == DIANFENG_JOB_LSZ)
			{
				pstDFChampionInfo = &pstDianFengGlobal->DFLSZFirstInfo.DFChampionInfo;
			}
			else if(w == DIANFENG_JOB_HMZ)
			{
				pstDFChampionInfo = &pstDianFengGlobal->DFHMZFirstInfo.DFChampionInfo;
			}
			else if(w == DIANFENG_JOB_LHS)
			{
				pstDFChampionInfo = &pstDianFengGlobal->DFLHSFirstInfo.DFChampionInfo;
			}
			else if(w == DIANFENG_JOB_JNZ)
			{
				pstDFChampionInfo = &pstDianFengGlobal->DFJNZFirstInfo.DFChampionInfo;
			}
			else
			{
				break;
			}

			if(pstDFChampionInfo == NULL)
			{
				continue;
			}

			if(pstGuessInfos->SupportMem[k].RoleID != pstDFChampionInfo->RoleID)
			{
				break;
			}
		}

		if(k>=MAX_SUPPORT_MEM_NUM)
		{
			iRet++; 
		}
	}
	
	return iRet;
}

int player_dianfeng_send_mail_award(ZONESVRENV *pstEnv,int num,tdr_ulonglong ullMoney,DIANFENGGLOBAL *pstDianFengGlobal)
{
	int iGuessNum = pstDianFengGlobal->GuessMemNum;
	int i;
	GUESSINFOS *pstGuessInfos = NULL;
	int k;
	DFFIRSTINFO *pstDFChampionInfo = NULL;
	int w = 0;
		
	for(i=0;i<iGuessNum;i++)
	{
		pstGuessInfos = &pstDianFengGlobal->GuessInfos[i];
		if(NULL == pstGuessInfos)
		{
			continue;
		}

		if(num == 0)
		{
			//告诉奖池多少钱的邮件
			player_moshi_dianfeng_money_pool_award(pstEnv,pstGuessInfos->GuessRoleName,ullMoney);
			continue;
		}

		for(k=0;k<MAX_SUPPORT_MEM_NUM;k++)
		{
			w = k+1;
			//客户端发给服务器存储时，必须按这样的顺序来，少一些比较
			if(w == DIANFENG_JOB_ZFZ)
			{
				pstDFChampionInfo = &pstDianFengGlobal->DFZFZFirstInfo.DFChampionInfo;
			}
			else if(w == DIANFENG_JOB_SHZ)
			{
				pstDFChampionInfo = &pstDianFengGlobal->DFSHZFirstInfo.DFChampionInfo;
			}
			else if(w == DIANFENG_JOB_YLS)
			{
                pstDFChampionInfo = &pstDianFengGlobal->DFYLSFirstInfo.DFChampionInfo;
			}
			else if(w == DIANFENG_JOB_SYZ)
			{
				pstDFChampionInfo = &pstDianFengGlobal->DFSYZFirstInfo.DFChampionInfo;
			}
			else if(w == DIANFENG_JOB_LSZ)
			{
				pstDFChampionInfo = &pstDianFengGlobal->DFLSZFirstInfo.DFChampionInfo;
			}
			else if(w == DIANFENG_JOB_HMZ)
			{
				pstDFChampionInfo = &pstDianFengGlobal->DFHMZFirstInfo.DFChampionInfo;
			}
			else if(w == DIANFENG_JOB_LHS)
			{
				pstDFChampionInfo = &pstDianFengGlobal->DFLHSFirstInfo.DFChampionInfo;
			}
			else if(w == DIANFENG_JOB_JNZ)
			{
				pstDFChampionInfo = &pstDianFengGlobal->DFJNZFirstInfo.DFChampionInfo;
			}
			else
			{
				break;
			}

			if(pstDFChampionInfo == NULL)
			{
				continue;
			}

			if(pstGuessInfos->SupportMem[k].RoleID != pstDFChampionInfo->RoleID)
			{
				break;
			}
		}

		if(k>=MAX_SUPPORT_MEM_NUM)
		{
			//猜中的
			player_moshi_dianfeng_first_award(pstEnv,pstGuessInfos->GuessRoleName,num,ullMoney);
		}
		else
		{
			//安慰奖
			player_moshi_dianfeng_join_award(pstEnv,pstGuessInfos->GuessRoleName,num,ullMoney);
		}
	}
	
	return 0;
}

int player_dianfeng_group_pipei(ZONESVRENV *pstEnv,DIANFENGJOININFO *pstDianFengJoinInfo)
{
	if(NULL == pstDianFengJoinInfo)
	{
		return -1;
	}
	
	int iMemNum = pstDianFengJoinInfo->MemNum;
	GROUPINFO *pstGroupInfo = NULL;
	Player *pstpstPlayer = NULL;
	if(iMemNum < 1)
	{
		goto _end;
	}

	if(iMemNum == 1)
	{
		pstGroupInfo = &pstDianFengJoinInfo->GroupInfo[0];
		
		if(NULL == pstGroupInfo)
		{
			goto _end;
		}

		pstpstPlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,pstGroupInfo->RoleName);
		if(NULL == pstpstPlayer)
		{
			if(pstGroupInfo->LoginOutTime == 0)
			{
				pstGroupInfo->LoginOutTime = pstEnv->pstAppCtx->stCurr.tv_sec + 180;
			}

			if(pstGroupInfo->LoginOutTime <= pstEnv->pstAppCtx->stCurr.tv_sec)
			{
				pstDianFengJoinInfo->MemNum--;
			}
		}
		else
		{
			pstGroupInfo->LoginOutTime = 0;
		}
		
		goto _end;
	}
	
	int i = 0;
	int j = 0;
	GROUPINFO *pstGroupInfo1 = NULL;
	GROUPINFO *pstGroupInfo2 = NULL;
	int iPos1 = 0;
	int iPos2 = 0;
	DIANFENGPIPEI *pstDianFengPiPei = NULL;
	int iPiPeiNum = 0;
	Player *pstpstPlayer1 = NULL;
	Player *pstpstPlayer2 = NULL;

	for(i=iMemNum-1;i>=1;i--)
	{
		pstGroupInfo1 = &pstDianFengJoinInfo->GroupInfo[i];
		if(NULL == pstGroupInfo1)
		{
			continue;
		}

		iPos1 = i;

		pstpstPlayer1 = z_name_player(pstEnv->pstAppCtx,pstEnv,pstGroupInfo1->RoleName);
		if(NULL == pstpstPlayer1)
		{
			if(pstGroupInfo1->LoginOutTime == 0)
			{
				pstGroupInfo1->LoginOutTime = pstEnv->pstAppCtx->stCurr.tv_sec + 180;
			}

			if(pstGroupInfo1->LoginOutTime <= pstEnv->pstAppCtx->stCurr.tv_sec)
			{
				pstDianFengJoinInfo->MemNum--;
				if(iPos1 != pstDianFengJoinInfo->MemNum)
				{
					memcpy(pstGroupInfo1,&pstDianFengJoinInfo->GroupInfo[pstDianFengJoinInfo->MemNum],sizeof(*pstGroupInfo1));
				}
			}
			
			continue;
		}
		else
		{
			pstGroupInfo1->LoginOutTime = 0;
		}
		
		for(j=i-1;j>=0;j--)
		{
			pstGroupInfo2 = &pstDianFengJoinInfo->GroupInfo[j];
			if(NULL == pstGroupInfo2)
			{
				continue;
			}
			
			iPos2 = j;
			
			pstpstPlayer2 = z_name_player(pstEnv->pstAppCtx,pstEnv,pstGroupInfo2->RoleName);
			if(NULL == pstpstPlayer2)
			{
				if(pstGroupInfo2->LoginOutTime == 0)
				{
					pstGroupInfo2->LoginOutTime = pstEnv->pstAppCtx->stCurr.tv_sec + 180;
				}
				
				if(pstGroupInfo2->LoginOutTime <= pstEnv->pstAppCtx->stCurr.tv_sec)
				{
					pstDianFengJoinInfo->MemNum--;
					if(iPos2 != pstDianFengJoinInfo->MemNum)
					{
						memcpy(pstGroupInfo2,&pstDianFengJoinInfo->GroupInfo[pstDianFengJoinInfo->MemNum],sizeof(*pstGroupInfo2));
						if (iPos1 == pstDianFengJoinInfo->MemNum)
						{
							iPos1 = j;
							pstGroupInfo1 = &pstDianFengJoinInfo->GroupInfo[iPos1];
						}
					}
				}
				
				continue;
			}
			else
			{
				pstGroupInfo2->LoginOutTime = 0;
			}

			pstDianFengPiPei = &pstDianFengJoinInfo->DianFengPiPei;

			iPiPeiNum = pstDianFengPiPei->PiPeiNum;
			if (iPiPeiNum >= MAX_DIANFENG_EVERY_GROUP_PIPEI_NUM)
			{
				goto _end;
			}

			player_pipei_succeed_ui_to_clt(pstEnv, pstpstPlayer1,1);
			player_pipei_succeed_ui_to_clt(pstEnv, pstpstPlayer2,1);
			
			STRNCPY(pstDianFengPiPei->PiPeiInfos[iPiPeiNum].RedRoleName,pstGroupInfo1->RoleName,sizeof(pstDianFengPiPei->PiPeiInfos[iPiPeiNum].RedRoleName));
			STRNCPY(pstDianFengPiPei->PiPeiInfos[iPiPeiNum].BuleRoleName,pstGroupInfo2->RoleName,sizeof(pstDianFengPiPei->PiPeiInfos[iPiPeiNum].BuleRoleName));
			
			pstDianFengPiPei->PiPeiInfos[iPiPeiNum].PiPeiTime = pstEnv->pstAppCtx->stCurr.tv_sec + 20;
	
			memset(&pstDianFengPiPei->PiPeiInfos[iPiPeiNum].DianFengWinLoss,0,sizeof(pstDianFengPiPei->PiPeiInfos[iPiPeiNum].DianFengWinLoss));
			pstDianFengPiPei->PiPeiInfos[iPiPeiNum].BuleLoginOutTime = 0;
			pstDianFengPiPei->PiPeiInfos[iPiPeiNum].RedLoginOutTime = 0;
			pstDianFengPiPei->PiPeiInfos[iPiPeiNum].PworldInstID = 0;
			pstDianFengPiPei->PiPeiInfos[iPiPeiNum].Wid = 0;
			
			pstDianFengPiPei->PiPeiNum++;

			pstDianFengJoinInfo->MemNum--;
			if(iPos1 != pstDianFengJoinInfo->MemNum)
			{
				memcpy(pstGroupInfo1,&pstDianFengJoinInfo->GroupInfo[pstDianFengJoinInfo->MemNum],sizeof(*pstGroupInfo1));
			}

			pstDianFengJoinInfo->MemNum--;
			if(iPos2 != pstDianFengJoinInfo->MemNum)
			{
				memcpy(pstGroupInfo2,&pstDianFengJoinInfo->GroupInfo[pstDianFengJoinInfo->MemNum],sizeof(*pstGroupInfo2));
			}

			goto _end;
		}
		
	}

_end:
	dianfeng_enter_pworld_tick(pstEnv,pstDianFengJoinInfo);
	
	return 0;
}

//匹配成功后，20  s 后 进入比赛场地
int dianfeng_enter_pworld_tick(ZONESVRENV *pstEnv,DIANFENGJOININFO *pstDianFengJoinInfo)
{
	ZONEIDX stIdx;
	PWORLDSELECT stPworldSelect;
	ZONEPWORLD *pstZonePworld;
	ZONEPWORLDINST *pstZonePworldInst;
	Player *pstpstPlayer1 = NULL;
	Player *pstpstPlayer2 = NULL;
	int iPiPei = 0;
	DIANFENGPIPEI *pstDianFengPiPei = NULL;
	int k = 0;
	PIPEIINFOS *pstPiPei = NULL;
	
	DIANFENGWINLOSSUI stDianFengWinLoss;
	
	pstDianFengPiPei = &pstDianFengJoinInfo->DianFengPiPei;
	iPiPei = pstDianFengPiPei->PiPeiNum;
	if(iPiPei <= 0)
	{
		return 0;
	}
	
	for(k=iPiPei-1;k>=0;k--)
	{
		pstPiPei = &pstDianFengPiPei->PiPeiInfos[k];
		if(pstPiPei == NULL)
		{
			continue;
		}
		
		if(pstPiPei->PiPeiTime != 0 && pstPiPei->PiPeiTime <= pstEnv->pstAppCtx->stCurr.tv_sec)
		{  
			pstpstPlayer1 = z_name_player(pstEnv->pstAppCtx,pstEnv,pstPiPei->RedRoleName);

			pstpstPlayer2 = z_name_player(pstEnv->pstAppCtx,pstEnv,pstPiPei->BuleRoleName);

			if(NULL != pstpstPlayer1 && NULL != pstpstPlayer2)
			{
				stIdx.iID = DIANFENG_PWORLD_ID;
				stIdx.iPos = -1;
			
				pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
				if (NULL == pstZonePworld)
				{
					return -1;
				}

				bzero(&stPworldSelect, sizeof(stPworldSelect));
				
				pstZonePworldInst = z_in_new_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstZonePworld, &stPworldSelect,  0, pstZonePworld->stPworldDef.Timeout*60);
				if (NULL == pstZonePworldInst)
				{
					continue;
				}
				
				pstZonePworldInst->iDownTm = pstEnv->pstAppCtx->stCurr.tv_sec+300;
				pstZonePworldInst->iDianFengRound = 0;

				pstPiPei->Wid =  pstZonePworldInst->ullPworldWID;
				pstPiPei->PworldInstID = pstZonePworldInst->iPworldInstID;

				pstPiPei->PiPeiTime = 0;

				DIANFENGINFO *pstDianFengInfo1 = &pstpstPlayer1->stRoleData.MiscInfo.DianFengInfo;
				DIANFENGINFO *pstDianFengInfo2 = &pstpstPlayer2->stRoleData.MiscInfo.DianFengInfo;

				//必须至少打过一场，才能开始领取奖励
				pstDianFengInfo1->DianFengFlag |= IF_DIANFENG_ONE_PK;
				pstDianFengInfo2->DianFengFlag |= IF_DIANFENG_ONE_PK;
				
				z_player_enter_pworld_map(pstEnv, pstpstPlayer1, pstZonePworldInst, pstZonePworld);
				z_player_enter_pworld_map(pstEnv, pstpstPlayer2, pstZonePworldInst, pstZonePworld);

				dianfeng_add_ready_buff_and_effect(pstEnv,pstpstPlayer1,pstpstPlayer2,0);

				memset(&stDianFengWinLoss,0,sizeof(stDianFengWinLoss));
				player_win_loss_ui_to_clt(pstEnv, pstpstPlayer1,&stDianFengWinLoss);
				player_win_loss_ui_to_clt(pstEnv, pstpstPlayer2,&stDianFengWinLoss);
				continue;
			}
			else if(NULL == pstpstPlayer1 && NULL != pstpstPlayer2)
			{
  				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstpstPlayer2, SYS_MIDDLE , ZONEERR_DIANFENG8);
				player_pipei_succeed_ui_to_clt(pstEnv, pstpstPlayer2,2);
			}
			else if(NULL != pstpstPlayer1 && NULL == pstpstPlayer2)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstpstPlayer1, SYS_MIDDLE , ZONEERR_DIANFENG8);
				player_pipei_succeed_ui_to_clt(pstEnv, pstpstPlayer1,2);
			}
			else
			{
				
			}

			player_pipei_bad(pstDianFengJoinInfo,pstDianFengPiPei,k);
			
		}
	}
	return 0;
}

//匹配失败，有至少一方下线
int player_pipei_bad(DIANFENGJOININFO *pstDianFengJoinInfo,DIANFENGPIPEI *pstDianFengPiPei,int k)
{
	int memNum = pstDianFengJoinInfo->MemNum;

	if (memNum > MAX_DIANFENG_EVERY_GROUP_MEM_NUM - 2)
	{
		return -1;
	}
	
	memset(&pstDianFengJoinInfo->GroupInfo[memNum],0,sizeof(pstDianFengJoinInfo->GroupInfo[memNum]));
	STRNCPY(pstDianFengJoinInfo->GroupInfo[memNum].RoleName,pstDianFengPiPei->PiPeiInfos[k].RedRoleName,sizeof(pstDianFengJoinInfo->GroupInfo[memNum].RoleName));
	pstDianFengJoinInfo->MemNum++;

	memNum = pstDianFengJoinInfo->MemNum;
	memset(&pstDianFengJoinInfo->GroupInfo[memNum],0,sizeof(pstDianFengJoinInfo->GroupInfo[memNum]));
	STRNCPY(pstDianFengJoinInfo->GroupInfo[memNum].RoleName,pstDianFengPiPei->PiPeiInfos[k].BuleRoleName,sizeof(pstDianFengJoinInfo->GroupInfo[memNum].RoleName));
	pstDianFengJoinInfo->MemNum++;

	pstDianFengPiPei->PiPeiNum--;
	if(k != pstDianFengPiPei->PiPeiNum)
	{
		memcpy(&pstDianFengPiPei->PiPeiInfos[k],&pstDianFengPiPei->PiPeiInfos[pstDianFengPiPei->PiPeiNum],sizeof(pstDianFengPiPei->PiPeiInfos[k]));
	}
				
	return 0;
}

int dianfeng_get_map_pos(ZONESVRENV* pstEnv,  MapIndex *pstMapIdx,
							RESPOS *pstPos, Player *pstPlayer)
{
	int iPiPei = 0;
	DIANFENGPIPEI *pstDianFengPiPei = NULL;
	int k = 0;
	PIPEIINFOS *pstPiPei = NULL;

	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;

	DIANFENGJOININFO *pstDianFengJoinInfo = NULL;

	if(0 > dianfeng_get_joininfo_by_career(pstEnv,&pstDianFengJoinInfo,pstDianFengGlobal,pstPlayer->stRoleData.Career))
		return -1;
	
	pstDianFengPiPei = &pstDianFengJoinInfo->DianFengPiPei;
	if(pstDianFengPiPei == NULL)
	{
		return -1;
	}
	iPiPei = pstDianFengPiPei->PiPeiNum;
	if(iPiPei <= 0)
	{
		return -1;
	}
	
	for(k=0;k<iPiPei;k++)
	{
		pstPiPei = &pstDianFengPiPei->PiPeiInfos[k];
		if(pstPiPei == NULL)
		{
			continue;
		}

		if(0 == strcmp(pstPiPei->RedRoleName,pstPlayer->stRoleData.RoleName))
		{
			*pstPos = pstMapIdx->stMapDef.RevivePs[0];
			break;
		}

		if(0 == strcmp(pstPiPei->BuleRoleName,pstPlayer->stRoleData.RoleName))
		{
			*pstPos = pstMapIdx->stMapDef.RevivePs[1];
			break;
		}
	}

	if(k>=iPiPei)
	{
		return -1;
	}
	
	return 0;
}

//巅峰副本中，玩家死亡处理
int dianfeng_player_pworld_die_dispose(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	ZONEPWORLDINST *pstZonePworldInst;
	ZONEPWORLD *pstPworld = NULL;
	int iPiPei = 0;
	DIANFENGPIPEI *pstDianFengPiPei = NULL;
	int k = 0;
	PIPEIINFOS *pstPiPei = NULL;
	PIPEIINFOS *pstPiPei2 = NULL;
	int flag = 0;
	int iRound = 0;
	Player *pstPlayerWin = NULL; 
	int i=0;
	
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;
	DIANFENGJOININFO *pstDianFengJoinInfo = NULL;

	tlog_debug(pstEnv->pstLogCat, 0, 0, "dianfeng:dieName : %s", pstPlayer->stRoleData.RoleName);
	
	if(0 > dianfeng_get_joininfo_by_career(pstEnv,&pstDianFengJoinInfo,pstDianFengGlobal,pstPlayer->stRoleData.Career))
		return -1;
	
	pstDianFengPiPei = &pstDianFengJoinInfo->DianFengPiPei;
	if(pstDianFengPiPei == NULL)
	{
		return -1;
	}
	iPiPei = pstDianFengPiPei->PiPeiNum;
	if(iPiPei <= 0)
	{
		return 0;
	}
	
	for(k=iPiPei-1;k>=0;k--)
	{
		pstPiPei = &pstDianFengPiPei->PiPeiInfos[k];
		if(pstPiPei == NULL)
		{
			continue;
		}
		
		if(0 == strcmp(pstPiPei->RedRoleName,pstPlayer->stRoleData.RoleName))
		{
			flag = 2;//死亡的玩家为红方,红方输
			break;
		}

		if(0 == strcmp(pstPiPei->BuleRoleName,pstPlayer->stRoleData.RoleName))
		{
			flag = 1;//死亡的玩家为蓝方，红方赢
			break;	
		}
		
	}

	if(k<0)
	{
		return -1;
	}

	pstZonePworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstPiPei->PworldInstID);
	if (NULL == pstZonePworldInst)
	{
		return -1;
	}
	
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
	if (NULL == pstPworld)
	{
		return -1;
	}

	if(pstZonePworldInst->tYanChiTime != 0)
	{
		return -1;
	}
	
	for(i=0;i<iPiPei;i++)
	{
		pstPiPei2 = &pstDianFengPiPei->PiPeiInfos[i];
		if(pstPiPei2 == NULL)
		{
			continue;
		}
		
		tlog_debug(pstEnv->pstLogCat, 0, 0, "dianfeng:all PiPeiInfos [%d] :(redrolename:%s, bluerolename:%s,wid:%llu,w1:%d,w2:%d,w3:%d)",i, pstPiPei2->RedRoleName, pstPiPei2->BuleRoleName, pstPiPei2->Wid,
		pstPiPei2->DianFengWinLoss.WinLossInfo[0], pstPiPei2->DianFengWinLoss.WinLossInfo[1],pstPiPei2->DianFengWinLoss.WinLossInfo[2]);
	}
	
	tlog_debug(pstEnv->pstLogCat, 0, 0, "dianfeng: die : pipei:(redrolename:%s, bluerolename:%s,wid:%llu,w1:%d,w2:%d,w3:%d),  k %d, pworldwid %llu, round %d", pstPiPei->RedRoleName, pstPiPei->BuleRoleName, pstPiPei->Wid,
		pstPiPei->DianFengWinLoss.WinLossInfo[0], pstPiPei->DianFengWinLoss.WinLossInfo[1],pstPiPei->DianFengWinLoss.WinLossInfo[2],
		k, pstZonePworldInst->ullPworldWID, pstZonePworldInst->iDianFengRound);
	
	//0、1、2  才正常
	iRound = pstZonePworldInst->iDianFengRound;
	if(iRound >= MAX_DIANFENG_PK_FIELD_NUM)
	{
		return 0;
	}
		
	pstPiPei->DianFengWinLoss.FieldNum = MAX_DIANFENG_PK_FIELD_NUM;
	
	//红方输
	if(flag == 2)
	{
		//红方pstPlayer ， 蓝方pstPlayerWin
		pstPiPei->DianFengWinLoss.WinLossInfo[iRound].Info = 2;
		pstPlayerWin = z_name_player(pstEnv->pstAppCtx,pstEnv,pstPiPei->BuleRoleName);
		if(NULL != pstPlayerWin)
		{
			//播放胜利失败特效
			dianfeng_bofang_shengli_effect(pstEnv,pstPlayerWin);
			dianfeng_bofang_loss_effect(pstEnv,pstPlayer);

			tlog_debug(pstEnv->pstLogCat, 0, 0, "dianfeng: winName (%s)", pstPlayerWin->stRoleData.RoleName);
		}
		else
		{
			dianfeng_bofang_loss_effect(pstEnv,pstPlayer);
		}
	}
	//红方赢
	else 
	{
		// 蓝方pstPlayer ，红 方pstPlayerWin
		pstPiPei->DianFengWinLoss.WinLossInfo[iRound].Info = 1;
		pstPlayerWin = z_name_player(pstEnv->pstAppCtx,pstEnv,pstPiPei->RedRoleName);
		if(NULL != pstPlayerWin)
		{
			dianfeng_bofang_shengli_effect(pstEnv,pstPlayerWin);
			dianfeng_bofang_loss_effect(pstEnv,pstPlayer);
			
			tlog_debug(pstEnv->pstLogCat, 0, 0, "dianfeng : winName (%s)", pstPlayerWin->stRoleData.RoleName);
		}
		else
		{
			dianfeng_bofang_loss_effect(pstEnv,pstPlayer);
		}
		
	}

	dianfeng_yanchi_dispose_tm(pstEnv,pstZonePworldInst);

	return 0;
}

//红方输赢平场次
int dianfeng_red_pk_info(ZONESVRENV* pstEnv,PIPEIINFOS *pstPiPei,int iRound,int *iPingNum,int *iRedWinNum,int *iRedLossNum)
{
	int i;
	int iRedWin = 0;
	int iNum = 0;
	int iRedLoss = 0 ;
	
	for(i=0;i<=iRound;i++)
	{
		if(pstPiPei->DianFengWinLoss.WinLossInfo[i].Info == 1)
		{
			iRedWin ++;
		}
		else if(pstPiPei->DianFengWinLoss.WinLossInfo[i].Info == 2)
		{
			iRedLoss ++;
		}
		else if(pstPiPei->DianFengWinLoss.WinLossInfo[i].Info == 3)
		{
			//平局
			iNum ++;
		}
		else
		{}
	}

	*iPingNum = iNum;
	*iRedWinNum = iRedWin;
	*iRedLossNum = iRedLoss;
	
	return 0;
}

//   1  表示红方胜，-1 表示没，2表示蓝方胜
int dianfeng_if_pk_win(ZONESVRENV* pstEnv,PIPEIINFOS *pstPiPei,int iRound)
{
	int iRedWinNum = 0;//红方赢的次数
	int iPingNum = 0;//平的次数
	int iRedLossNum = 0;//红方输的次数

	if(iRound >= MAX_DIANFENG_PK_FIELD_NUM || iRound <= 0)
	{
		return -1;		
	}

	int iTemp2 = MAX_DIANFENG_PK_FIELD_NUM - 2;
	int iTemp1 = MAX_DIANFENG_PK_FIELD_NUM - 1;

	dianfeng_red_pk_info(pstEnv,pstPiPei,iRound,&iPingNum,&iRedWinNum,&iRedLossNum);

	// iRound 1   表示打了两场
	if(iRound == iTemp2)
	{
		//红方:  胜2  或 胜1平1 或平2
		if((iRedWinNum ==  iTemp1) ||
			(iRedWinNum == iTemp2 && iPingNum == iTemp2)||
			(iPingNum == iTemp1))
		{
			return 1;
		}

		//蓝方胜:  红方输2 或红方输1平1
		if((iRedLossNum == iTemp1)||
			(iPingNum == iTemp2 && iRedLossNum == iTemp2))
		{
			return 2;
		}
	}

	//iRound 2  表示3场 才决出胜负
	if(iRound == iTemp1)
	{
		//红方胜:   胜2负1  ，胜1  负1 平1
		if((iRedWinNum == iTemp1 && iRedLossNum == iTemp2)||
			(iRedWinNum == iTemp2 && iRedLossNum == iTemp2 && iPingNum == iTemp2))
		{
			return 1;
		}
		else
		{
			// 蓝方胜:   红方赢1  输2
			return 2;
		}
	}	
	
	return -1;
}

//假如有一方胜出，进行处理
int dianfeng_pk_end_dispose(ZONESVRENV* pstEnv,PIPEIINFOS *pstPiPei,Player *pstRedPlayer,Player *pstBulePlayer,int iRound,int k,ZONEPWORLDINST *pstPworldInst,ZONEPWORLD *pstPworld,int whichJob)
{
	DIANFENGINFO *pstDianFengInfo = NULL;
	RESPOS stResPos;
	int iMapIndex;

	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;
	MapIndex* pstIdx;
	RESPOS stPos;	
	int res = dianfeng_if_pk_win(pstEnv,pstPiPei,iRound);
	
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;

	tlog_debug(pstEnv->pstLogCat, 0, 0, "dianfeng : pipei:(redrolename:%s, bluerolename:%s,wid:%llu,w1:%d,w2:%d,w3:%d), iRound %d, k %d, pworldwid %llu, round %d, whichJob %d, res %d", pstPiPei->RedRoleName, pstPiPei->BuleRoleName, pstPiPei->Wid,
		pstPiPei->DianFengWinLoss.WinLossInfo[0], pstPiPei->DianFengWinLoss.WinLossInfo[1],pstPiPei->DianFengWinLoss.WinLossInfo[2],
		iRound, k, pstPworldInst->ullPworldWID, pstPworldInst->iDianFengRound, whichJob, res);
	if (pstRedPlayer)
	{
		tlog_debug(pstEnv->pstLogCat, 0, 0, "dianfeng : redplayer:%s", pstRedPlayer->stRoleData.RoleName);
	}

	if (pstBulePlayer)
	{
		tlog_debug(pstEnv->pstLogCat, 0, 0, "dianfeng : blueplayer:%s", pstBulePlayer->stRoleData.RoleName);
	}

	//红胜
	if(1 == res)
	{
		//红方再添加到报名队列,并把匹配队列该双人去除
		player_career_pipei_del_baoming_add(pstEnv,pstPiPei->RedRoleName,k,whichJob);
		
		if(NULL != pstBulePlayer)
		{
  			pstDianFengInfo = &pstBulePlayer->stRoleData.MiscInfo.DianFengInfo;

			iMapIndex = pstDianFengInfo->MapId;
			stResPos.X = pstDianFengInfo->ResPos.X;
			stResPos.Y = pstDianFengInfo->ResPos.Y;

			//踢出蓝方
			z_dianfeng_change_map(pstEnv, pstBulePlayer,iMapIndex, stResPos ,0);

			dianfeng_revive_sth(pstEnv,pstBulePlayer);

			//先清掉在副本中加的所有状态
			player_status_clean_all(pstEnv->pstAppCtx, pstEnv, pstBulePlayer);

			dianfeng_restore_status(pstEnv,pstPworld,pstBulePlayer);
			
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstBulePlayer, SYS_SVR , ZONEERR_DIANFENG14);
		}

		if(NULL != pstRedPlayer)
		{
			if(0 > dianfeng_is_first_msg(pstEnv,pstRedPlayer->stRoleData.Career,pstDianFengGlobal))
			{
				dianfeng_guang_bo(pstEnv,pstRedPlayer);
			}
			else
			{
				player_status_clean_all(pstEnv->pstAppCtx, pstEnv, pstRedPlayer);
				//为冠军时，还原状态
				dianfeng_restore_status(pstEnv,pstPworld,pstRedPlayer);
			}
		}
		
		//销毁副本		
		z_in_destroy_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstPworldInst);
	}
	//蓝方胜
	else if(2 == res)
	{
		player_career_pipei_del_baoming_add(pstEnv, pstPiPei->BuleRoleName,k,whichJob);
			
		if(NULL != pstRedPlayer)
		{
			pstDianFengInfo = &pstRedPlayer->stRoleData.MiscInfo.DianFengInfo;

			iMapIndex = pstDianFengInfo->MapId;
			stResPos.X = pstDianFengInfo->ResPos.X;
			stResPos.Y = pstDianFengInfo->ResPos.Y;

			//踢出红方
			z_dianfeng_change_map(pstEnv, pstRedPlayer,iMapIndex, stResPos ,0);

			dianfeng_revive_sth(pstEnv,pstRedPlayer);

			player_status_clean_all(pstEnv->pstAppCtx, pstEnv, pstRedPlayer);

			dianfeng_restore_status(pstEnv,pstPworld,pstRedPlayer);
		
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstRedPlayer, SYS_SVR , ZONEERR_DIANFENG14);
		}
		
		if(NULL != pstBulePlayer)
		{
			if(0 > dianfeng_is_first_msg(pstEnv,pstBulePlayer->stRoleData.Career,pstDianFengGlobal))
			{
				dianfeng_guang_bo(pstEnv,pstBulePlayer);
			}
			else
			{
				player_status_clean_all(pstEnv->pstAppCtx, pstEnv, pstBulePlayer);
				//为冠军时，还原状态
				dianfeng_restore_status(pstEnv,pstPworld,pstBulePlayer);
			}
		}
		
		//销毁副本		
		z_in_destroy_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstPworldInst);
	}
	//还没决出胜负
	else
	{
		iMapIndex = (pstPworldInst->iPworldInstID & 0xffff)<<16 | (pstPworld->stPworldDef.InitMapID & 0xffff);
		
		stIdx.iID	=	iMapIndex;
		stIdx.iPos	=	-1;
		pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
		if(!pstMapInst)
			return -1;
	
		pstIdx	= z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, stIdx.iID);
		if( !pstIdx )
			return -1;

		stPos.X = pstIdx->stMapDef.RevivePs[0].X;
		stPos.Y = pstIdx->stMapDef.RevivePs[0].Y;
		
		pstPworldInst->iDianFengRound++;
		tlog_debug(pstEnv->pstLogCat, 0, 0, "dianfeng : pworldwid:%llu, round:%d", pstPworldInst->ullPworldWID, pstPworldInst->iDianFengRound);

		pstPworldInst->iDownTm = pstEnv->pstAppCtx->stCurr.tv_sec+300;
		
		if(pstPworldInst->iDianFengRound < MAX_DIANFENG_PK_FIELD_NUM)
		{
			pworld_again(pstEnv,pstPworld,pstPworldInst,iMapIndex,&stPos);

			if(pstRedPlayer != NULL && pstBulePlayer != NULL)
			{
				tlog_debug(pstEnv->pstLogCat, 0, 0, "dianfeng : redplayer :%s, blueplayer:%s", pstRedPlayer->stRoleData.RoleName, pstBulePlayer->stRoleData.RoleName);
			    dianfeng_add_ready_buff_and_effect(pstEnv,pstRedPlayer,pstBulePlayer,pstPworldInst->iDianFengRound);
			}
			return -1;
		}
	}
	
	return 0;
}

int z_dianfeng_change_map(ZONESVRENV* pstEnv, Player *pstPlayer,int iMapIndex, RESPOS stResPos,int Type)
{
	RESPOS stResPos1;
 	MapIndex *pstMapIdx;
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;
	DIANFENGINFO *pstDianFengInfo = NULL;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, iMapIndex);
	if(NULL == pstMapIdx)
	{
		return -1;
	}

	stIdx.iID	=	iMapIndex;
	stIdx.iPos	=	-1;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &stResPos, &stResPos, 
						300, 0, &stResPos1);

	if(Type == 1)
	{
		//切换之前，先记录下来
		pstDianFengInfo = &pstPlayer->stRoleData.MiscInfo.DianFengInfo;
		pstDianFengInfo->MapId = pstPlayer->stRoleData.Map;
		pstDianFengInfo->ResPos.X = pstPlayer->stRoleData.Pos.X;
		pstDianFengInfo->ResPos.Y =  pstPlayer->stRoleData.Pos.Y;
	}
	
	z_player_change_map(pstEnv->pstAppCtx, pstEnv, pstPlayer, iMapIndex, &stResPos1, 0, 0);

	return 0;
}

int player_career_pipei_del_baoming_add(ZONESVRENV* pstEnv,char *pName,int k,int whichJob)
{	
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;
	DIANFENGJOININFO *pstDianFengJoinInfo = NULL;
	DIANFENGPIPEI *pstDianFengPiPei = NULL;
	RESPOS stResPos;
	int iMapIndex = 0;
	Player *pstPlayer = NULL;
	char TempRoleName[ROLE_NAME_LEN];  

	if(0 > dianfeng_get_joininfo_by_career(pstEnv,&pstDianFengJoinInfo,pstDianFengGlobal,whichJob))
	{
		return -1;
	}
	
	if(0 > dianfeng_get_ready_map_by_career(pstEnv,whichJob,&iMapIndex))
	{
		return -1;
	}

	pstDianFengPiPei = &pstDianFengJoinInfo->DianFengPiPei;
	if(pstDianFengPiPei == NULL)
	{
		return -1;
	}
	
	int memNum = pstDianFengJoinInfo->MemNum;

	if (memNum > MAX_DIANFENG_EVERY_GROUP_MEM_NUM - 1)
	{
		return -1;
	}

	STRNCPY(TempRoleName,pName,sizeof(TempRoleName));
	
	memset(&pstDianFengJoinInfo->GroupInfo[memNum],0,sizeof(pstDianFengJoinInfo->GroupInfo[memNum]));
	STRNCPY(pstDianFengJoinInfo->GroupInfo[memNum].RoleName,pName,sizeof(pstDianFengJoinInfo->GroupInfo[memNum].RoleName));
	pstDianFengJoinInfo->MemNum++;

	pstDianFengPiPei->PiPeiNum--;
	if(k != pstDianFengPiPei->PiPeiNum)
	{
		memcpy(&pstDianFengPiPei->PiPeiInfos[k],&pstDianFengPiPei->PiPeiInfos[pstDianFengPiPei->PiPeiNum],sizeof(pstDianFengPiPei->PiPeiInfos[k]));
	}
	
	pstPlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,TempRoleName);
	if(pstPlayer != NULL)
	{
		if(0 > dianfeng_is_first_msg(pstEnv,pstPlayer->stRoleData.Career,pstDianFengGlobal))
		{
			stResPos.X = 6600;
			stResPos.Y = 6000;
			z_dianfeng_change_map(pstEnv, pstPlayer,iMapIndex, stResPos ,0);
		}
	}	
	
	return 0;
}

//副本倒计时结束
int z_dianfeng_pworld_down_tm_end(ZONESVRENV* pstEnv,ZONEPWORLDINST *pstPworldInst,ZONEPWORLD *pstPworld)
{
	int iRound = pstPworldInst->iDianFengRound;
	Player *pstRedPlayer = NULL;
	Player *pstBulePlayer = NULL;
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;

	PIPEIINFOS *pstPiPei = NULL;
	int iCarceer = 0;
	int iWhere = 0;

	if(0 > dianfeng_by_wid_get_pipei(pstEnv,&pstPiPei,pstDianFengGlobal,pstPworldInst,&iCarceer,&iWhere))
	{
		return -1;
	}

	pstPiPei->DianFengWinLoss.FieldNum = MAX_DIANFENG_PK_FIELD_NUM;

	pstBulePlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,pstPiPei->BuleRoleName);
	pstRedPlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,pstPiPei->RedRoleName);
	
	if(pstBulePlayer != NULL && pstRedPlayer != NULL)
	{
		pstPiPei->DianFengWinLoss.WinLossInfo[iRound].Info = 3; 
		dianfeng_bofang_loss_effect(pstEnv,pstBulePlayer);
		dianfeng_bofang_loss_effect(pstEnv,pstRedPlayer);
	}
	else if(pstBulePlayer == NULL && pstRedPlayer != NULL)
	{
		pstPiPei->DianFengWinLoss.WinLossInfo[iRound].Info = 1; 
		dianfeng_bofang_shengli_effect(pstEnv,pstRedPlayer);
	}
	else if(pstBulePlayer != NULL && pstRedPlayer == NULL)
	{
		pstPiPei->DianFengWinLoss.WinLossInfo[iRound].Info = 2; 
		dianfeng_bofang_shengli_effect(pstEnv,pstBulePlayer);
	}
	else
	{
		pstPiPei->DianFengWinLoss.WinLossInfo[iRound].Info = 3; 
	}
	
	dianfeng_yanchi_dispose_tm(pstEnv,pstPworldInst);

	return 0;
}

int dianfeng_get_joininfo_by_career(ZONESVRENV* pstEnv,DIANFENGJOININFO **ppstDianFengJoin,DIANFENGGLOBAL *pstDianFengGlobal,int career)
{
	switch(career)
	{
		case CAREER_WARRIOR_SINGLEHAND:
			*ppstDianFengJoin = &pstDianFengGlobal->DianFengSHZJoinInfo;
			break;
		case CAREER_WARRIOR_DOUBLEHAND:	
			*ppstDianFengJoin = &pstDianFengGlobal->DianFengZFZJoinInfo;
			break;
		case CAREER_MAGICOR_SINGLEHAND:
			*ppstDianFengJoin = &pstDianFengGlobal->DianFengJNZJoinInfo;
			break;
		case CAREER_MAGICOR_DOUBLEHAND:	
			*ppstDianFengJoin = &pstDianFengGlobal->DianFengLHSJoinInfo;
			break;
		case CAREER_GUNER_SINGLEHAND:
			*ppstDianFengJoin = &pstDianFengGlobal->DianFengLSZJoinInfo;
			break;
		case CAREER_GUNER_DOUBLEHAND:
			*ppstDianFengJoin = &pstDianFengGlobal->DianFengHMZJoinInfo;
			break;
		case CAREER_CHURCH_SINGLEHAND:
			*ppstDianFengJoin = &pstDianFengGlobal->DianFengSYZJoinInfo;
			break;
		case CAREER_CHURCH_DOUBLEHAND:	
			*ppstDianFengJoin = &pstDianFengGlobal->DianFengYLSJoinInfo;
			break;	
		default:
			return -1;
			break;
	}
	
	return 0;
}

int dianfeng_pworld_tick(ZONESVRENV * pstEnv, ZONEPWORLD *pstPworld, ZONEPWORLDINST *pstPworldInst, int iSec)
{
	UNUSED(pstPworld);
	UNUSED(iSec);
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;
	PIPEIINFOS *pstPiPei = NULL;
	Player *pstTempPlayer =NULL;
	Player *pstTempPlayer2 =NULL;
	int iCarceer = 0;
	int iWhere = 0;

	if(0 > dianfeng_by_wid_get_pipei(pstEnv,&pstPiPei,pstDianFengGlobal,pstPworldInst,&iCarceer,&iWhere))
	{
		return -1;
	}

	if(pstPiPei == NULL)
	{
		return -1;
	}

	pstTempPlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,pstPiPei->BuleRoleName);
	pstTempPlayer2 = z_name_player(pstEnv->pstAppCtx,pstEnv,pstPiPei->RedRoleName);

	if(NULL ==  pstTempPlayer)
	{
		if(pstPiPei->BuleLoginOutTime == 0)
		{
			pstPiPei->BuleLoginOutTime = pstEnv->pstAppCtx->stCurr.tv_sec + 120;
		}

		if(pstPiPei->BuleLoginOutTime <= pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			//已经在这个函数中，传送到准备地图
			player_career_pipei_del_baoming_add(pstEnv, pstPiPei->RedRoleName,iWhere,iCarceer);

			if(pstTempPlayer2 != NULL)
			{
				if(0 > dianfeng_is_first_msg(pstEnv,pstTempPlayer2->stRoleData.Career,pstDianFengGlobal))
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTempPlayer2, SYS_MIDDLE , ZONEERR_DIANFENG21);
				}
				else
				{
					player_status_clean_all(pstEnv->pstAppCtx, pstEnv, pstTempPlayer2);
					//为冠军时，还原状态
					dianfeng_restore_status(pstEnv,pstPworld,pstTempPlayer2);
				}
			}
			
			//销毁副本		
			z_in_destroy_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstPworldInst);

			return 0;
		}
	}
	else
	{
		pstPiPei->BuleLoginOutTime = 0;
	}

	pstTempPlayer2 = z_name_player(pstEnv->pstAppCtx,pstEnv,pstPiPei->RedRoleName);
	if(NULL ==  pstTempPlayer2)
	{
		if(pstPiPei->RedLoginOutTime == 0)
		{
			pstPiPei->RedLoginOutTime = pstEnv->pstAppCtx->stCurr.tv_sec + 120;
		}

		if(pstPiPei->RedLoginOutTime <= pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			player_career_pipei_del_baoming_add(pstEnv,pstPiPei->BuleRoleName ,iWhere,iCarceer);

			if(pstTempPlayer != NULL)
			{
				//如果是冠军了，就别提示这一句了
				if(0 > dianfeng_is_first_msg(pstEnv,pstTempPlayer->stRoleData.Career,pstDianFengGlobal))
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTempPlayer, SYS_MIDDLE , ZONEERR_DIANFENG21);
				}
				else
				{
					player_status_clean_all(pstEnv->pstAppCtx, pstEnv, pstTempPlayer);
					//为冠军时，还原状态
					dianfeng_restore_status(pstEnv,pstPworld,pstTempPlayer);
				}
			}
			
			//销毁副本		
			z_in_destroy_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstPworldInst);
		}
	}
	else
	{
		pstPiPei->RedLoginOutTime = 0;
	}
	
	return 0;
}


//登录的时候，检查是否还在报名和匹配的队列中
int player_login_baoming_pipei_queue_check(ZONESVRENV* pstEnv, Player *pstPlayer,DIANFENGGLOBAL *pstDianFengGlobal)
{
	DIANFENGINFO *pstDianFengInfo = NULL;
	pstDianFengInfo = &pstPlayer->stRoleData.MiscInfo.DianFengInfo;
	
	if(pstDianFengInfo->JoinTime != 0 && pstDianFengInfo->JoinTime != pstDianFengGlobal->ActiveTimeS)
	{
		pstDianFengInfo->LoginOutNum = 0;
		pstDianFengInfo->DianFengFlag = 0;
		pstDianFengInfo->PkOutNum = 0;
		pstDianFengInfo->JoinTime = pstDianFengGlobal->ActiveTimeS;
	}

	//没报名的，下面不用看了
	if(!(pstDianFengInfo->DianFengFlag & IF_DIANFENG_BAOMING))
	{
		return -1;
	}

	DIANFENGINFO *pstDianFengInfo1 = NULL;
	int i;
	DIANFENGJOININFO *pstDianFengJoinInfo = NULL;
	int iPiPei = 0;
	DIANFENGPIPEI *pstDianFengPiPei = NULL;
	int k = 0;
	PIPEIINFOS *pstPiPei = NULL;
	int flag = 0;
	RESPOS ResPos;
	int iMapIndex;
	int ColorFlag = 0;
	ZONEPWORLDINST *pstZonePworldInst = NULL;
	ZONEPWORLD *pstPworld = NULL;
	Player *pstTempPlayer = NULL;

	int MemNum;
	int iEnd = 0;
	
	if(0 > dianfeng_get_joininfo_by_career(pstEnv,&pstDianFengJoinInfo,pstDianFengGlobal,pstPlayer->stRoleData.Career))
		return -1;

	pstDianFengPiPei = &pstDianFengJoinInfo->DianFengPiPei;
	if(pstDianFengPiPei == NULL)
	{
		return -1;
	}
	iPiPei = pstDianFengPiPei->PiPeiNum;
	if(iPiPei <= 0)
	{
		flag = 1;
	}
	
	for(k=iPiPei-1;k>=0;k--)
	{
		pstPiPei = &pstDianFengPiPei->PiPeiInfos[k];
		if(pstPiPei == NULL)
		{
			continue;
		}
		
		if(0 == strcmp(pstPiPei->RedRoleName,pstPlayer->stRoleData.RoleName))
		{
			ColorFlag = 1;
		}

		if(0 == strcmp(pstPiPei->BuleRoleName,pstPlayer->stRoleData.RoleName))
		{
			ColorFlag = 2;
		}

		if(ColorFlag == 1 || ColorFlag == 2)
		{
			if(pstDianFengGlobal->Status == ACTIVE_PK)
			{
				if(pstDianFengInfo->JoinTime == pstDianFengGlobal->ActiveTimeS && pstDianFengInfo->JoinTime != 0)
				{
					//pk  时的离线次数不能超过3次
					pstDianFengInfo->PkOutNum++;
				}
			}

			//先找出来
			pstZonePworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstPiPei->PworldInstID);
			if(pstZonePworldInst != NULL)
			{
				pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
			}
		
			if(pstDianFengInfo->PkOutNum >= 3)
			{
				if(ColorFlag == 1)
				{
					pstTempPlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,pstPiPei->BuleRoleName);
					//蓝方再添加到报名队列,并把匹配队列该双人去除
					player_career_pipei_del_baoming_add(pstEnv,pstPiPei->BuleRoleName,k,pstPlayer->stRoleData.Career);
				}
				else
				{
					pstTempPlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,pstPiPei->RedRoleName);
					//红方再添加到报名队列,并把匹配队列该双人去除
					player_career_pipei_del_baoming_add(pstEnv,pstPiPei->RedRoleName,k,pstPlayer->stRoleData.Career);
				}
				
				pstDianFengInfo->PkOutNum = 0;	
				
				if(NULL != pstTempPlayer)
				{
					if(0 > dianfeng_is_first_msg(pstEnv,pstTempPlayer->stRoleData.Career,pstDianFengGlobal))
					{
						z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTempPlayer, SYS_MIDDLE2 , ZONEERR_DIANFENG21);
					}
					else
					{
						player_status_clean_all(pstEnv->pstAppCtx, pstEnv, pstTempPlayer);
						//为冠军时，还原状态
						dianfeng_restore_status(pstEnv,pstPworld,pstTempPlayer);
					}
				}

				if (NULL != pstZonePworldInst && NULL != pstPworld)
				{
					iEnd = 1;
				}
				
				goto end;
			}
			else
			{
				break;	
			}
		}
	}

	//不在匹配中
	if(k<0)
	{
		flag = 1;
	}

	if(flag == 1)
	{
		MemNum = pstDianFengJoinInfo->MemNum;
		if(MemNum <= 0)
		{
			if(0 == player_dianfeng_curr_map_same_as_reday_map(pstEnv,pstPlayer))
			{
				goto end;
			}
		}
		for(i=MemNum-1;i>=0;i--)
		{
			if(0 == strcmp(pstDianFengJoinInfo->GroupInfo[i].RoleName,pstPlayer->stRoleData.RoleName))
			{
				if(pstDianFengGlobal->Status == ACTIVE_PK)
				{
					if(pstDianFengInfo->JoinTime == pstDianFengGlobal->ActiveTimeS && pstDianFengInfo->JoinTime != 0)
					{
						//报名队列的离线次数不能超过3次
						pstDianFengInfo->LoginOutNum++;
					}
				}

				if(pstDianFengInfo->LoginOutNum >= 3)
				{
					pstDianFengJoinInfo->MemNum--;
					if(i != pstDianFengJoinInfo->MemNum)
					{
						memcpy(&pstDianFengJoinInfo->GroupInfo[i],&pstDianFengJoinInfo->GroupInfo[pstDianFengJoinInfo->MemNum],sizeof(pstDianFengJoinInfo->GroupInfo[i]));
					}

					pstDianFengInfo->LoginOutNum = 0;
					goto end;
				}
				else
				{
					break;
				}
			}
		}

		//不在报名队列中
		if(i<0)
		{
			if(0 == player_dianfeng_curr_map_same_as_reday_map(pstEnv,pstPlayer))
			{
				goto end;
			}
		}
		
	}
	//在匹配中
	else
	{
		if (NULL == pstZonePworldInst || NULL == pstPworld)
		{
			//基本不会出现这种情况
			if(ColorFlag == 1)
			{
				pstTempPlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,pstPiPei->BuleRoleName);
			}
			else 
			{
				pstTempPlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,pstPiPei->RedRoleName);
			}

			pstDianFengPiPei->PiPeiNum--;
			if(k != pstDianFengPiPei->PiPeiNum)
			{
				memcpy(&pstDianFengPiPei->PiPeiInfos[k],&pstDianFengPiPei->PiPeiInfos[pstDianFengPiPei->PiPeiNum],sizeof(pstDianFengPiPei->PiPeiInfos[k]));
			}
			
			if(NULL != pstTempPlayer)
			{
				pstDianFengInfo1 = &pstTempPlayer->stRoleData.MiscInfo.DianFengInfo;
				iMapIndex = pstDianFengInfo1->MapId;
				ResPos.X = pstDianFengInfo1->ResPos.X;
				ResPos.Y = pstDianFengInfo1->ResPos.Y;

				//踢出去最开始的地方
				z_dianfeng_change_map(pstEnv, pstTempPlayer,iMapIndex, ResPos ,0);
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTempPlayer, SYS_MIDDLE , ZONEERR_DIANFENG20);
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTempPlayer, SYS_SVR , ZONEERR_DIANFENG14);
			}

			goto end;
		}

		z_player_enter_pworld_map(pstEnv, pstPlayer, pstZonePworldInst, pstPworld);

		//红方
		if(ColorFlag == 1)
		{
			player_win_loss_ui_to_clt(pstEnv, pstPlayer,&pstPiPei->DianFengWinLoss);
		}
		//蓝方
		else
		{		
			DIANFENGWINLOSSUI stDianFengWinLoss;
			memset(&stDianFengWinLoss,0,sizeof(stDianFengWinLoss));
			stDianFengWinLoss.FieldNum = MAX_DIANFENG_PK_FIELD_NUM;
			
			//根据红方情况推出蓝方情况
			for(i=0;i<MAX_DIANFENG_PK_FIELD_NUM;i++)
			{
				if(pstPiPei->DianFengWinLoss.WinLossInfo[i].Info == 2)
				{
					stDianFengWinLoss.WinLossInfo[i].Info = 1;
				}
				else if(pstPiPei->DianFengWinLoss.WinLossInfo[i].Info == 1)
				{
					stDianFengWinLoss.WinLossInfo[i].Info  = 2;
				}
				else if(pstPiPei->DianFengWinLoss.WinLossInfo[i].Info == 3)
				{
					stDianFengWinLoss.WinLossInfo[i].Info  = 3;
				}
				else
				{
					stDianFengWinLoss.WinLossInfo[i].Info = 0;
				}
			}
		
			player_win_loss_ui_to_clt(pstEnv, pstPlayer,&stDianFengWinLoss);
		}
	}

	return 0;

end:
	iMapIndex = pstDianFengInfo->MapId;
	ResPos.X = pstDianFengInfo->ResPos.X;
	ResPos.Y = pstDianFengInfo->ResPos.Y;

	//踢出去最开始的地方
	z_dianfeng_change_map(pstEnv, pstPlayer,iMapIndex, ResPos ,0);
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE , ZONEERR_DIANFENG20);
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR , ZONEERR_DIANFENG14);

	if(iEnd == 1)
	{
		z_in_destroy_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstZonePworldInst);
	}
	return 0;
}	

int dianfeng_by_wid_get_pipei(ZONESVRENV* pstEnv,PIPEIINFOS **ppstPiPei,DIANFENGGLOBAL *pstDianFengGlobal,ZONEPWORLDINST *pstPworldInst,int *iCarceer,int *iWhere)
{
	if(NULL == pstPworldInst)
	{
		return -1;
	}
	
	DIANFENGJOININFO *pstDianFengZFZJoinInfo = &pstDianFengGlobal->DianFengZFZJoinInfo;
	DIANFENGJOININFO *pstDianFengSHZJoinInfo = &pstDianFengGlobal->DianFengSHZJoinInfo;
	DIANFENGJOININFO *pstDianFengYLSJoinInfo = &pstDianFengGlobal->DianFengYLSJoinInfo;
	DIANFENGJOININFO *pstDianFengSYZJoinInfo = &pstDianFengGlobal->DianFengSYZJoinInfo;
	DIANFENGJOININFO *pstDianFengLSZJoinInfo = &pstDianFengGlobal->DianFengLSZJoinInfo;
	DIANFENGJOININFO *pstDianFengHMZJoinInfo = &pstDianFengGlobal->DianFengHMZJoinInfo;
	DIANFENGJOININFO *pstDianFengLHSJoinInfo = &pstDianFengGlobal->DianFengLHSJoinInfo;
	DIANFENGJOININFO *pstDianFengJNZJoinInfo = &pstDianFengGlobal->DianFengJNZJoinInfo;
	int index = 0;
	
	if(0 == dianfeng_get_info_by_wid(pstEnv,&index,pstDianFengZFZJoinInfo,pstPworldInst))
	{
		*iCarceer = CAREER_WARRIOR_DOUBLEHAND;
		*ppstPiPei = &pstDianFengZFZJoinInfo->DianFengPiPei.PiPeiInfos[index];
		goto final;
	}

	if(0 == dianfeng_get_info_by_wid(pstEnv,&index,pstDianFengSHZJoinInfo,pstPworldInst))
	{
		*iCarceer = CAREER_WARRIOR_SINGLEHAND;
		*ppstPiPei = &pstDianFengSHZJoinInfo->DianFengPiPei.PiPeiInfos[index];
		goto final;
	}

	if(0 == dianfeng_get_info_by_wid(pstEnv,&index,pstDianFengYLSJoinInfo,pstPworldInst))
	{
		*iCarceer = CAREER_CHURCH_DOUBLEHAND;
		*ppstPiPei = &pstDianFengYLSJoinInfo->DianFengPiPei.PiPeiInfos[index];
		goto final;
	}

	if(0 == dianfeng_get_info_by_wid(pstEnv,&index,pstDianFengSYZJoinInfo,pstPworldInst))
	{
		*iCarceer = CAREER_CHURCH_SINGLEHAND;
		*ppstPiPei = &pstDianFengSYZJoinInfo->DianFengPiPei.PiPeiInfos[index];
		goto final;
	}

	if(0 == dianfeng_get_info_by_wid(pstEnv,&index,pstDianFengLSZJoinInfo,pstPworldInst))
	{
		*iCarceer = CAREER_GUNER_SINGLEHAND;
		*ppstPiPei = &pstDianFengLSZJoinInfo->DianFengPiPei.PiPeiInfos[index];
		goto final;
	}

	if(0 == dianfeng_get_info_by_wid(pstEnv,&index,pstDianFengHMZJoinInfo,pstPworldInst))
	{
		*iCarceer = CAREER_GUNER_DOUBLEHAND;
		*ppstPiPei = &pstDianFengHMZJoinInfo->DianFengPiPei.PiPeiInfos[index];
		goto final;
	}

	if(0 == dianfeng_get_info_by_wid(pstEnv,&index,pstDianFengLHSJoinInfo,pstPworldInst))
	{
		*iCarceer = CAREER_MAGICOR_DOUBLEHAND;
		*ppstPiPei = &pstDianFengLHSJoinInfo->DianFengPiPei.PiPeiInfos[index];
		goto final;
	}

	if(0 == dianfeng_get_info_by_wid(pstEnv,&index,pstDianFengJNZJoinInfo,pstPworldInst))
	{
		*iCarceer = CAREER_MAGICOR_SINGLEHAND;
		*ppstPiPei = &pstDianFengJNZJoinInfo->DianFengPiPei.PiPeiInfos[index];
		goto final;
	}

	return -1;
	
final:	
	
	*iWhere = index;
	return 0;
}

int dianfeng_get_info_by_wid(ZONESVRENV* pstEnv,int *index,DIANFENGJOININFO *pstDianFengJoin,ZONEPWORLDINST *pstPworldInst)
{
	int iPiPei;
	int k;
	DIANFENGPIPEI *pstDianFengPiPei = NULL;
	if(pstDianFengJoin == NULL)
	{
		return -1;
	}
		
	pstDianFengPiPei = &pstDianFengJoin->DianFengPiPei;
	if(pstDianFengPiPei == NULL)
	{
		return -1;
	}
	iPiPei = pstDianFengPiPei->PiPeiNum;
	if(iPiPei <= 0)
	{
		return -1;
	}
	
	for(k=iPiPei-1;k>=0;k--)
	{
		if(pstDianFengPiPei->PiPeiInfos[k].Wid == pstPworldInst->ullPworldWID)
			break;
	}

	if(k<0)
	{
		return -1;
	}

	*index = k;
	
	return 0;
}

int dianfeng_get_ready_map_by_career(ZONESVRENV* pstEnv,int career,int *iMapIndex)
{
	switch(career)
	{
		case CAREER_WARRIOR_SINGLEHAND:
			*iMapIndex = SHZ_READY_MAP_ID;
			break;
		case CAREER_WARRIOR_DOUBLEHAND:	
			*iMapIndex = ZHZ_READY_MAP_ID;
			break;
		case CAREER_MAGICOR_SINGLEHAND:
			*iMapIndex = JNZ_READY_MAP_ID;
			break;
		case CAREER_MAGICOR_DOUBLEHAND:	
			*iMapIndex = LHS_READY_MAP_ID;
			break;
		case CAREER_GUNER_SINGLEHAND:
			*iMapIndex = LSZ_READY_MAP_ID;
			break;
		case CAREER_GUNER_DOUBLEHAND:
			*iMapIndex = HMZ_READY_MAP_ID;
			break;
		case CAREER_CHURCH_SINGLEHAND:
			*iMapIndex = SYZ_READY_MAP_ID;
			break;
		case CAREER_CHURCH_DOUBLEHAND:	
			*iMapIndex = YLS_READY_MAP_ID;
			break;	
		default:
			return -1;
			break;
	}

	return 0;
}

int player_dianfeng_if_chamption(ZONESVRENV* pstEnv,DIANFENGGLOBAL *pstDianFengGlobal,Player *pstPlayer)
{
	DFFIRSTINFO *pstChampionInfo = NULL;
	
	switch(pstPlayer->stRoleData.Career)
	{
		case CAREER_WARRIOR_SINGLEHAND:
			pstChampionInfo = &pstDianFengGlobal->DFSHZFirstInfo.DFChampionInfo;  
			break;
		case CAREER_WARRIOR_DOUBLEHAND: 
			pstChampionInfo = &pstDianFengGlobal->DFZFZFirstInfo.DFChampionInfo;
			break;
		case CAREER_MAGICOR_SINGLEHAND:
			pstChampionInfo = &pstDianFengGlobal->DFJNZFirstInfo.DFChampionInfo; 
			break;
		case CAREER_MAGICOR_DOUBLEHAND: 
			pstChampionInfo = &pstDianFengGlobal->DFLHSFirstInfo.DFChampionInfo; 
			break;
		case CAREER_GUNER_SINGLEHAND:
			pstChampionInfo = &pstDianFengGlobal->DFLSZFirstInfo.DFChampionInfo; 
			break;
		case CAREER_GUNER_DOUBLEHAND:
			pstChampionInfo = &pstDianFengGlobal->DFHMZFirstInfo.DFChampionInfo; 
			break;
		case CAREER_CHURCH_SINGLEHAND:
			pstChampionInfo = &pstDianFengGlobal->DFSYZFirstInfo.DFChampionInfo;  
			break;
		case CAREER_CHURCH_DOUBLEHAND:	
			pstChampionInfo = &pstDianFengGlobal->DFYLSFirstInfo.DFChampionInfo; 
			break;	
		default:
			return -1;
			break;
	}

	if(pstChampionInfo == NULL)
	{
		return -1;
	}
	
	if(0 == strcmp(pstPlayer->stRoleData.RoleName,pstChampionInfo->RoleName))
	{
		//是冠军
		return 1;
	}
	
    return -1;
}


int dianfeng_chg_baoming_and_pipei_name(ZONESVRENV* pstEnv, const char* pszOldName, const char* pszNewName,DIANFENGJOININFO *pstDianFengJoinInfo)
{
	int MemNum,PiPeiNum;
	int i,j;

	GROUPINFO *pstGroupInfo = NULL;
	PIPEIINFOS *pstPiPeiInfos = NULL;

	if(pstDianFengJoinInfo == NULL)
	{
		return 0;
	}
 	DIANFENGPIPEI *pstDianFengPiPei = &pstDianFengJoinInfo->DianFengPiPei;
	
	MemNum = min(pstDianFengJoinInfo->MemNum,MAX_DIANFENG_EVERY_GROUP_MEM_NUM);

	//要么在报名队列，要么在匹配队列
	
	if(MemNum > 0)
	{
		for (i=0; i<MemNum; i++)
		{
			pstGroupInfo = &pstDianFengJoinInfo->GroupInfo[i];
			if(pstGroupInfo == NULL)
			{
				continue;
			}
		
			if (!pstGroupInfo->RoleName)
			{
				continue;
			}
			
			if (strcmp(pstGroupInfo->RoleName, pszOldName)  == 0)
			{
				STRNCPY(pstGroupInfo->RoleName,pszNewName, ROLE_NAME_LEN);
				return 1;
			}
		}
	}

	if(pstDianFengPiPei == NULL)
	{
		return 0;
	}
	PiPeiNum = min(pstDianFengPiPei->PiPeiNum,MAX_DIANFENG_EVERY_GROUP_PIPEI_NUM);

	if(PiPeiNum > 0)
	{
		for(j=0;j<PiPeiNum;j++)
		{
			pstPiPeiInfos = &pstDianFengPiPei->PiPeiInfos[j];
			if(pstPiPeiInfos == NULL)
			{
				continue;
			}

			if((!pstPiPeiInfos->RedRoleName)|| (!pstPiPeiInfos->BuleRoleName))
			{
				continue;
			}

			if (strcmp(pstPiPeiInfos->RedRoleName, pszOldName)  == 0)
			{
				STRNCPY(pstPiPeiInfos->RedRoleName,pszNewName, ROLE_NAME_LEN);
				return 1;
			}

			if (strcmp(pstPiPeiInfos->BuleRoleName, pszOldName)  == 0)
			{
				STRNCPY(pstPiPeiInfos->BuleRoleName,pszNewName, ROLE_NAME_LEN);
				return 1;
			}
		}
	}
	
	return 0;
}

int dianfeng_chg_champion_and_effigy_name(ZONESVRENV* pstEnv, const char* pszOldName, const char* pszNewName,DFFIRST *pstDFFirst)
{
	if(NULL == pstDFFirst)
	{
		return 0;
	}
	DFFIRSTINFO *pstDFChampionInfo = &pstDFFirst->DFChampionInfo;
	DFFIRSTINFO *pstDFEffigyInfo = &pstDFFirst->DFEffigyInfo;
	int flag = 0;

	if(pstDFChampionInfo == NULL)
	{
		return 0;
	}

	if(pstDFEffigyInfo == NULL)
	{
		return 0;
	}

	if (pstDFChampionInfo->RoleName)
	{
		if (strcmp(pstDFChampionInfo->RoleName, pszOldName)  == 0)
		{
			STRNCPY(pstDFChampionInfo->RoleName,pszNewName, ROLE_NAME_LEN);
			flag = 1;
		}
	}

	if (pstDFEffigyInfo->RoleName)
	{
		if (strcmp(pstDFEffigyInfo->RoleName, pszOldName)  == 0)
		{
			STRNCPY(pstDFEffigyInfo->RoleName,pszNewName, ROLE_NAME_LEN);
			flag = 1;
		}
	}

	if(flag == 1)
	{
		return 1;
	}
	
	return 0;
}

int dianfeng_update_name(ZONESVRENV* pstEnv, const char* pszOldName, const char* pszNewName)
{
	if(pstEnv->pstConf->DianFengFlag == 0 || (is_fini_global(pstEnv) == 0) || get_curr_max_level(pstEnv) < 65)
	{
		return 0;
	}
	
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;
	DIANFENGJOININFO *pstDianFengZFZJoinInfo = &pstDianFengGlobal->DianFengZFZJoinInfo;
	DIANFENGJOININFO *pstDianFengSHZJoinInfo = &pstDianFengGlobal->DianFengSHZJoinInfo;
	DIANFENGJOININFO *pstDianFengYLSJoinInfo = &pstDianFengGlobal->DianFengYLSJoinInfo;
	DIANFENGJOININFO *pstDianFengSYZJoinInfo = &pstDianFengGlobal->DianFengSYZJoinInfo;
	DIANFENGJOININFO *pstDianFengLSZJoinInfo = &pstDianFengGlobal->DianFengLSZJoinInfo;
	DIANFENGJOININFO *pstDianFengHMZJoinInfo = &pstDianFengGlobal->DianFengHMZJoinInfo;
	DIANFENGJOININFO *pstDianFengLHSJoinInfo = &pstDianFengGlobal->DianFengLHSJoinInfo;
	DIANFENGJOININFO *pstDianFengJNZJoinInfo = &pstDianFengGlobal->DianFengJNZJoinInfo;

	DFFIRST *pstDFZFZFirst = &pstDianFengGlobal->DFZFZFirstInfo;
	DFFIRST *pstDFHMZFirst = &pstDianFengGlobal->DFHMZFirstInfo;
	DFFIRST *pstDFJNZFirst = &pstDianFengGlobal->DFJNZFirstInfo;
	DFFIRST *pstDFLHSFirst = &pstDianFengGlobal->DFLHSFirstInfo;
	DFFIRST *pstDFSHZFirst = &pstDianFengGlobal->DFSHZFirstInfo;
	DFFIRST *pstDFSYZFirst = &pstDianFengGlobal->DFSYZFirstInfo;
	DFFIRST *pstDFYLSFirst = &pstDianFengGlobal->DFYLSFirstInfo;
	DFFIRST *pstDFLSZFirst = &pstDianFengGlobal->DFLSZFirstInfo;

	int i;
	int GuessMemNum = pstDianFengGlobal->GuessMemNum;
	GUESSINFOS *pstGuessInfos = NULL;
	int flag = 0;
	int flag2 = 0;

	//改竞猜名
	if(GuessMemNum > 0)
	{
		for(i=0;i<GuessMemNum;i++)
		{
			pstGuessInfos = &pstDianFengGlobal->GuessInfos[i];
			if(pstGuessInfos == NULL)
			{
				continue;
			}

			if(!pstGuessInfos->GuessRoleName)
			{
				continue;
			}

			if (strcmp(pstGuessInfos->GuessRoleName, pszOldName)  == 0)
			{
				STRNCPY(pstGuessInfos->GuessRoleName,pszNewName, ROLE_NAME_LEN);
				break;
			}
		}
	}

	//改报名队列和匹配队列的
	if(1 == dianfeng_chg_baoming_and_pipei_name(pstEnv,pszOldName,pszNewName,pstDianFengZFZJoinInfo))
	{
		flag = 1;
	}

	if(0 == flag)
	{
		if(1 == dianfeng_chg_baoming_and_pipei_name(pstEnv,pszOldName,pszNewName,pstDianFengSHZJoinInfo))
		{
			flag = 1;
		}
	}

	if(0 == flag)
	{
		if(1 == dianfeng_chg_baoming_and_pipei_name(pstEnv,pszOldName,pszNewName,pstDianFengYLSJoinInfo))
		{
			flag = 1;
		}
	}

	if(0 == flag)
	{
		if(1 == dianfeng_chg_baoming_and_pipei_name(pstEnv,pszOldName,pszNewName,pstDianFengSYZJoinInfo))
		{
			flag = 1;
		}
	}

	if(0 == flag)
	{
		if(1 == dianfeng_chg_baoming_and_pipei_name(pstEnv,pszOldName,pszNewName,pstDianFengLSZJoinInfo))
		{
			flag = 1;
		}
	}

	if(0 == flag)
	{
		if(1 == dianfeng_chg_baoming_and_pipei_name(pstEnv,pszOldName,pszNewName,pstDianFengHMZJoinInfo))
		{
			flag = 1;
		}
	}

	if(0 == flag)
	{
		if(1 == dianfeng_chg_baoming_and_pipei_name(pstEnv,pszOldName,pszNewName,pstDianFengLHSJoinInfo))
		{
			flag = 1;
		}
	}

	if(0 == flag)
	{
		if(1 == dianfeng_chg_baoming_and_pipei_name(pstEnv,pszOldName,pszNewName,pstDianFengJNZJoinInfo))
		{
			flag = 1;
		}
	}

	//改冠军名和雕像名
	if(0 == flag2)
	{
		if(1 == dianfeng_chg_champion_and_effigy_name(pstEnv,pszOldName,pszNewName,pstDFZFZFirst))
		{
			flag2 = 1;
		}
	}

	if(0 == flag2)
	{
		if(1 == dianfeng_chg_champion_and_effigy_name(pstEnv,pszOldName,pszNewName,pstDFHMZFirst))
		{
			flag2 = 1;
		}
	}

	if(0 == flag2)
	{
		if(1 == dianfeng_chg_champion_and_effigy_name(pstEnv,pszOldName,pszNewName,pstDFJNZFirst))
		{
			flag2 = 1;
		}
	}

	if(0 == flag2)
	{
		if(1 == dianfeng_chg_champion_and_effigy_name(pstEnv,pszOldName,pszNewName,pstDFLHSFirst))
		{
			flag2 = 1;
		}
	}

	if(0 == flag2)
	{
		if(1 == dianfeng_chg_champion_and_effigy_name(pstEnv,pszOldName,pszNewName,pstDFSHZFirst))
		{
			flag2 = 1;
		}
	}

	if(0 == flag2)
	{
		if(1 == dianfeng_chg_champion_and_effigy_name(pstEnv,pszOldName,pszNewName,pstDFSYZFirst))
		{
			flag2 = 1;
		}
	}

	if(0 == flag2)
	{
		if(1 == dianfeng_chg_champion_and_effigy_name(pstEnv,pszOldName,pszNewName,pstDFYLSFirst))
		{
			flag2 = 1;
		}
	}

	if(0 == flag2)
	{
		if(1 == dianfeng_chg_champion_and_effigy_name(pstEnv,pszOldName,pszNewName,pstDFLSZFirst))
		{
			flag2 = 1;
		}
	}
	
	return 0;
}

//脚本中给巅峰 竞技 奖励
int dianfeng_give_award(ZONESVRENV* pstEnv,int iAwardType,Player *pstPlayer)
{
	int i, iGridNum = 0;
	char szMsg[1024];
	ROLEGRID astGrid[MAX_SPARTA_AWARD_ITEM];
	SPARTAAWARDDEF *pstAwardDef;
	int iRange = 0;

	if(iAwardType == 1)
	{
		//冠军奖励
		iRange = 1000;
	}
	else
	{
		//参与奖励
		iRange = 1001;
	}
	
	pstAwardDef = z_find_sparta_award_def(pstEnv, iRange);
	if(!pstAwardDef)
	{
		return -1;
	}

	for(i=0; i<=MAX_SPARTA_AWARD_ITEM-1; i++)
	{
		if(pstAwardDef->AwardItem[i].ItemID == 0)
		{
			continue;
		}

		if(0 > item_create(pstEnv->pstAppCtx, pstEnv, &astGrid[iGridNum], pstAwardDef->AwardItem[i].ItemID))
		{
			continue;
		}

		if(pstAwardDef->AwardItem[i].Bind)
		{
			astGrid[iGridNum].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
		}

		if(astGrid[iGridNum].Type == GRID_ITEM_ITEM)
		{
			astGrid[iGridNum].GridData.RoleItem.Num = pstAwardDef->AwardItem[i].Num;
		}

		iGridNum++;
	}

	if(iGridNum <= 0)
	{
		return 0;
	}

	//背包不足，用邮件
	if(0 > package_add_test(pstEnv->pstAppCtx , pstEnv , pstPlayer, astGrid, iGridNum,0))
	{
		if(iAwardType == 1)
		{
			snprintf(szMsg, sizeof(szMsg), LJY_MAIL68, pstPlayer->stRoleData.RoleName);
			player_sysmail_fast_send(pstEnv, pstPlayer->stRoleData.RoleName, LJY_MAIL61, szMsg, astGrid, iGridNum, 0, LJY_MAIL63, 0);
		}
		else
		{
			snprintf(szMsg, sizeof(szMsg), LJY_MAIL67, pstPlayer->stRoleData.RoleName);
			player_sysmail_fast_send(pstEnv, pstPlayer->stRoleData.RoleName, LJY_MAIL59, szMsg, astGrid, iGridNum, 0, LJY_MAIL63, 0);
		}

	}
	else
	{
		 package_add(pstEnv->pstAppCtx , pstEnv , pstPlayer, astGrid, iGridNum,0, IMPITEM_OPTYPE_SCRIPT);
	}
	
	return 0;
}

//判断玩家是否已经竞猜过了，有的话，要发回竞猜消息给客户端
int player_dianfeng_if_guess(ZONESVRENV* pstEnv,Player *pstPlayer, CSDIANFENGIFGUESS *pstCSDianFengIfGuess)
{
	UNUSED(pstCSDianFengIfGuess);
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;

	if(pstDianFengGlobal->Status == ACTIVE_PK)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG26);
		return -1;
	}

	if(pstDianFengGlobal->Status == ACTIVE_CLOSE || pstDianFengGlobal->Status == ACTIVE_CLEAN_DATA)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG27);
		return -1;
	}
	
	GUESSINFOS stGuessInfos;
	memset(&stGuessInfos,0,sizeof(stGuessInfos));

	int GuessMemNum = pstDianFengGlobal->GuessMemNum;
	GUESSINFOS *pstGuessInfos = NULL;
	int i;
	int flag = 0;
	
	if(GuessMemNum > 0)
	{
		for(i=0;i<GuessMemNum;i++)
		{
			pstGuessInfos = &pstDianFengGlobal->GuessInfos[i];
			if(pstGuessInfos == NULL)
			{
				continue;
			}

			if(!pstGuessInfos->GuessRoleName)
			{
				continue;
			}

			if (strcmp(pstGuessInfos->GuessRoleName, pstPlayer->stRoleData.RoleName)  == 0)
			{
				memcpy(&stGuessInfos,pstGuessInfos,sizeof(stGuessInfos));
					
				//竞猜过
				player_guess_res_to_clt(pstEnv,pstPlayer,3,stGuessInfos,pstDianFengGlobal);
					
				dianfeng_get_guess_mingdan(pstEnv,pstPlayer,pstDianFengGlobal,0);
					
				break;
			}
		}

		if(i>=GuessMemNum)
		{
			flag = 1;
		}
	}
	else
	{
		flag = 1;
	}
	
	if(1 == flag)
	{
		//没有
		player_guess_res_to_clt(pstEnv,pstPlayer,4,stGuessInfos,pstDianFengGlobal);
	}

	return 0;
}

int dianfeng_add_ready_buff_and_effect(ZONESVRENV* pstEnv,Player *pstRedPlayer,Player *pstBulePlayer,int iRound)
{
	//加定身buff
	z_player_apply_status( pstEnv->pstAppCtx, pstEnv, pstRedPlayer, 
					DINGSHNE_BUFF, 1, 9, 1, 0);

	z_player_apply_status( pstEnv->pstAppCtx, pstEnv, pstBulePlayer, 
					DINGSHNE_BUFF, 1, 9, 1, 0);

	if(iRound == 0)
	{
		//倒计时特效第一局
		play_effect(pstEnv, pstRedPlayer, 2, 0, "playUiEffect(0,0,0,\"effect\\\\entity\\\\JJCdaojishi1.ent\")", NULL);
		play_effect(pstEnv, pstBulePlayer, 2, 0, "playUiEffect(0,0,0,\"effect\\\\entity\\\\JJCdaojishi1.ent\")", NULL);
	}
	else if(iRound == 1)
	{
		//倒计时特效第二局
		play_effect(pstEnv, pstRedPlayer, 2, 0, "playUiEffect(0,0,0,\"effect\\\\entity\\\\JJCdaojishi2.ent\")", NULL);
		play_effect(pstEnv, pstBulePlayer, 2, 0, "playUiEffect(0,0,0,\"effect\\\\entity\\\\JJCdaojishi2.ent\")", NULL);
	}
	else
	{
		//倒计时特效第三局
		play_effect(pstEnv, pstRedPlayer, 2, 0, "playUiEffect(0,0,0,\"effect\\\\entity\\\\JJCdaojishi3.ent\")", NULL);
		play_effect(pstEnv, pstBulePlayer, 2, 0, "playUiEffect(0,0,0,\"effect\\\\entity\\\\JJCdaojishi3.ent\")", NULL);
	}
	
	return 0;
}

int dianfeng_bofang_shengli_effect(ZONESVRENV* pstEnv,Player *pstWinPlayer)
{
	//胜利特效
	play_effect(pstEnv, pstWinPlayer, 2, 0, "playUiEffect(0,0,0,\"effect\\\\entity\\\\JJCshengli.ent\")", NULL);
	
	return 0;
}

int dianfeng_bofang_loss_effect(ZONESVRENV* pstEnv,Player *pstLossPlayer)
{
	//失败特效
	play_effect(pstEnv, pstLossPlayer, 2, 0, "playUiEffect(0,0,0,\"effect\\\\entity\\\\JJCshibai.ent\")", NULL);

	return 0;
}

//获取红蓝方进入比赛副本中的面向
int dianfeng_get_dir(ZONESVRENV* pstEnv,Player *pstPlayer,int *iDir)
{
	int iPiPei = 0;
	DIANFENGPIPEI *pstDianFengPiPei = NULL;
	int k = 0;
	PIPEIINFOS *pstPiPei = NULL;

	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;

	DIANFENGJOININFO *pstDianFengJoinInfo = NULL;

	if(0 > dianfeng_get_joininfo_by_career(pstEnv,&pstDianFengJoinInfo,pstDianFengGlobal,pstPlayer->stRoleData.Career))
		return -1;
	
	pstDianFengPiPei = &pstDianFengJoinInfo->DianFengPiPei;
	if(pstDianFengPiPei == NULL)
	{
		return -1;
	}
	iPiPei = pstDianFengPiPei->PiPeiNum;
	if(iPiPei <= 0)
	{
		return -1;
	}
	
	for(k=0;k<iPiPei;k++)
	{
		pstPiPei = &pstDianFengPiPei->PiPeiInfos[k];
		if(pstPiPei == NULL)
		{
			continue;
		}

		if(0 == strcmp(pstPiPei->RedRoleName,pstPlayer->stRoleData.RoleName))
		{
			*iDir = 0;
			break;
		}

		if(0 == strcmp(pstPiPei->BuleRoleName,pstPlayer->stRoleData.RoleName))
		{
			*iDir = 90;
			break;
		}
	}

	if(k>=iPiPei)
	{
		return -1;
	}
	
	return 0;
}

int dianfeng_gm_clean(ZONESVRENV* pstEnv,DIANFENGJOININFO *pstDianFengJoinInfo)
{
	int i;
	int iPiPei = 0;
	DIANFENGPIPEI *pstDianFengPiPei = NULL;
	int k = 0;
	PIPEIINFOS *pstPiPei = NULL;
	RESPOS ResPos;
	int iMapIndex;
	ZONEPWORLDINST *pstZonePworldInst = NULL;
	ZONEPWORLD *pstPworld = NULL;
	Player *pstTempPlayer2 = NULL;
	int MemNum;
	int iEnd = 0;
	Player *pstTempPlayer1 = NULL;
	DIANFENGINFO *pstDianFengInfo = NULL;
	DIANFENGINFO *pstDianFengInfo2 = NULL;
	Player *pstTempPlayer3 = NULL;

	if(pstDianFengJoinInfo == NULL)
	{
		return -1;
	}
	
	pstDianFengPiPei = &pstDianFengJoinInfo->DianFengPiPei;
	if(pstDianFengPiPei == NULL)
	{
		goto _baoming;
	}
	
	iPiPei = pstDianFengPiPei->PiPeiNum;
	if(iPiPei <= 0)
	{
		goto _baoming;
	}
	
	for(k=iPiPei-1;k>=0;k--)
	{
		pstPiPei = &pstDianFengPiPei->PiPeiInfos[k];
		if(pstPiPei == NULL)
		{
			continue;
		}
		
		pstTempPlayer1 = z_name_player(pstEnv->pstAppCtx,pstEnv,pstPiPei->RedRoleName);
		if(pstTempPlayer1 != NULL)
		{
			iEnd = 1;
			pstDianFengInfo = &pstTempPlayer1->stRoleData.MiscInfo.DianFengInfo;
			iMapIndex = pstDianFengInfo->MapId;
			ResPos.X = pstDianFengInfo->ResPos.X;
			ResPos.Y = pstDianFengInfo->ResPos.Y;
			z_dianfeng_change_map(pstEnv, pstTempPlayer1,iMapIndex, ResPos ,0);
		}

		pstTempPlayer2 = z_name_player(pstEnv->pstAppCtx,pstEnv,pstPiPei->BuleRoleName);
		if(pstTempPlayer2 != NULL)
		{
			iEnd = 1;
			pstDianFengInfo2 = &pstTempPlayer2->stRoleData.MiscInfo.DianFengInfo;
			iMapIndex = pstDianFengInfo2->MapId;
			ResPos.X = pstDianFengInfo2->ResPos.X;
			ResPos.Y = pstDianFengInfo2->ResPos.Y;
			z_dianfeng_change_map(pstEnv, pstTempPlayer2,iMapIndex, ResPos ,0);
		}

		if(iEnd == 1)
		{
			pstZonePworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstPiPei->PworldInstID);
			if(pstZonePworldInst != NULL)
			{
				pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
				if(pstPworld != NULL)
				{
					z_in_destroy_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstZonePworldInst);
				}
			}
			
		}
	}

_baoming:

	MemNum = pstDianFengJoinInfo->MemNum;
	if(MemNum <= 0)
	{
		return -1;
	}
	
	for(i=MemNum-1;i>=0;i--)
	{
		pstTempPlayer3 = z_name_player(pstEnv->pstAppCtx,pstEnv,pstDianFengJoinInfo->GroupInfo[i].RoleName);
		
		if(pstTempPlayer3 != NULL)
		{
			if(0 == player_dianfeng_curr_map_same_as_reday_map(pstEnv,pstTempPlayer3))
			{
				pstDianFengInfo2 = &pstTempPlayer3->stRoleData.MiscInfo.DianFengInfo;
				iMapIndex = pstDianFengInfo2->MapId;
				ResPos.X = pstDianFengInfo2->ResPos.X;
				ResPos.Y = pstDianFengInfo2->ResPos.Y;
				z_dianfeng_change_map(pstEnv, pstTempPlayer3,iMapIndex, ResPos ,0);
			}
		}
	}
	
	return 0;
}

int dianfeng_chamption_clear(ZONESVRENV* pstEnv,DFFIRSTINFO *pstDFChampionInfo)
{
	memset(pstDFChampionInfo,0,sizeof(*pstDFChampionInfo));
	return 0;
}

int dianfeng_effigy_clear(ZONESVRENV* pstEnv,DFFIRSTINFO *pstDFEffigyInfo)
{
	memset(pstDFEffigyInfo,0,sizeof(*pstDFEffigyInfo));
	return 0;
}

int dianfeng_get_guess_mingdan(ZONESVRENV* pstEnv,Player *pstPlayer,DIANFENGGLOBAL *pstDianFengGlobal,int iType)
{
	int iGuess = pstDianFengGlobal->GuessMemNum;
	int i;
	GUESSINFOS *pstGuess = NULL;
	CSDIANFENGGUESSMINGDAN stGuessMingDan;
	memset(&stGuessMingDan,0,sizeof(stGuessMingDan));
	int res = 0;
	
	if(iGuess <= 0)
	{
		return -1;
	}

	for(i=0;i<iGuess;i++)
	{
		 pstGuess = &pstDianFengGlobal->GuessInfos[i];
		 if(pstGuess == NULL)
		 {
			continue;
		 }

		 if(0 == strcmp(pstGuess->GuessRoleName,pstPlayer->stRoleData.RoleName))
		 {
			break;
		 }
	}

	if(i >= iGuess)
	{
		return -1;
	}

	STRNCPY(stGuessMingDan.GuessRoleName,pstPlayer->stRoleData.RoleName,sizeof(stGuessMingDan.GuessRoleName));
	
	DIANFENGJOININFO *pstDianFengZFZJoinInfo = &pstDianFengGlobal->DianFengZFZJoinInfo;
	DIANFENGJOININFO *pstDianFengSHZJoinInfo = &pstDianFengGlobal->DianFengSHZJoinInfo;
	DIANFENGJOININFO *pstDianFengYLSJoinInfo = &pstDianFengGlobal->DianFengYLSJoinInfo;
	DIANFENGJOININFO *pstDianFengSYZJoinInfo = &pstDianFengGlobal->DianFengSYZJoinInfo;
	DIANFENGJOININFO *pstDianFengLSZJoinInfo = &pstDianFengGlobal->DianFengLSZJoinInfo;
	DIANFENGJOININFO *pstDianFengHMZJoinInfo = &pstDianFengGlobal->DianFengHMZJoinInfo;
	DIANFENGJOININFO *pstDianFengLHSJoinInfo = &pstDianFengGlobal->DianFengLHSJoinInfo;
	DIANFENGJOININFO *pstDianFengJNZJoinInfo = &pstDianFengGlobal->DianFengJNZJoinInfo;

	stGuessMingDan.Num = MAX_SUPPORT_MEM_NUM;

	res = dianfeng_guess_mingdan(pstEnv,pstDianFengZFZJoinInfo,pstGuess->SupportMem[0].RoleID);
	
	if(res >= 0 )
	{
		STRNCPY(stGuessMingDan.SupportMingDan[0].Name,pstDianFengZFZJoinInfo->GroupInfo[res].RoleName,sizeof(stGuessMingDan.SupportMingDan[0].Name));
	}
	else
	{
		return -1;
	}

	res = dianfeng_guess_mingdan(pstEnv,pstDianFengSHZJoinInfo,pstGuess->SupportMem[1].RoleID);
	if(res >= 0 )
	{
		STRNCPY(stGuessMingDan.SupportMingDan[1].Name,pstDianFengSHZJoinInfo->GroupInfo[res].RoleName,sizeof(stGuessMingDan.SupportMingDan[1].Name));
	}
	else
	{
		return -1;
	}
	
	res = dianfeng_guess_mingdan(pstEnv,pstDianFengYLSJoinInfo,pstGuess->SupportMem[2].RoleID);
	if(res >= 0 )
	{
		STRNCPY(stGuessMingDan.SupportMingDan[2].Name,pstDianFengYLSJoinInfo->GroupInfo[res].RoleName,sizeof(stGuessMingDan.SupportMingDan[2].Name));
	}
	else
	{
		return -1;
	}
	
	res = dianfeng_guess_mingdan(pstEnv,pstDianFengSYZJoinInfo,pstGuess->SupportMem[3].RoleID);
	if(res >= 0 )
	{
		STRNCPY(stGuessMingDan.SupportMingDan[3].Name,pstDianFengSYZJoinInfo->GroupInfo[res].RoleName,sizeof(stGuessMingDan.SupportMingDan[3].Name));
	}
	else
	{
		return -1;
	}
	
	res = dianfeng_guess_mingdan(pstEnv,pstDianFengLSZJoinInfo,pstGuess->SupportMem[4].RoleID);
	if(res >= 0 )
	{
		STRNCPY(stGuessMingDan.SupportMingDan[4].Name,pstDianFengLSZJoinInfo->GroupInfo[res].RoleName,sizeof(stGuessMingDan.SupportMingDan[4].Name));
	}
	else
	{
		return -1;
	}
	
	res = dianfeng_guess_mingdan(pstEnv,pstDianFengHMZJoinInfo,pstGuess->SupportMem[5].RoleID);
	if(res >= 0 )
	{
		STRNCPY(stGuessMingDan.SupportMingDan[5].Name,pstDianFengHMZJoinInfo->GroupInfo[res].RoleName,sizeof(stGuessMingDan.SupportMingDan[5].Name));
	}
	else
	{
		return -1;
	}
	
	res = dianfeng_guess_mingdan(pstEnv,pstDianFengLHSJoinInfo,pstGuess->SupportMem[6].RoleID);
	if(res >= 0 )
	{
		STRNCPY(stGuessMingDan.SupportMingDan[6].Name,pstDianFengLHSJoinInfo->GroupInfo[res].RoleName,sizeof(stGuessMingDan.SupportMingDan[6].Name));
	}
	else
	{
		return -1;
	}
	
	res = dianfeng_guess_mingdan(pstEnv,pstDianFengJNZJoinInfo,pstGuess->SupportMem[7].RoleID);
	if(res >= 0 )
	{
		STRNCPY(stGuessMingDan.SupportMingDan[7].Name,pstDianFengJNZJoinInfo->GroupInfo[res].RoleName,sizeof(stGuessMingDan.SupportMingDan[7].Name));
	}
	else
	{
		return -1;
	}

	//发邮件
	if(iType == 1)
	{
		//发竞猜名单邮件
		player_dianfeng_guess_mingdan_mail(pstEnv,&stGuessMingDan);
	}

	dianfeng_guess_mingdan_to_clt(pstEnv, pstPlayer,&stGuessMingDan);
	
	return 0;
}

int dianfeng_guess_mingdan(ZONESVRENV* pstEnv,DIANFENGJOININFO *pstDianFengJoinInfo,tdr_ulonglong RoleID)
{
	int MemNum;
	GROUPINFO *pstGroupInfo = NULL;
	int i;

	if(NULL == pstDianFengJoinInfo)
	{
		return -1;
	}

	MemNum = pstDianFengJoinInfo->MemNum;
	if(MemNum <= 0)
	{
		return -1;
	}
	
	for(i=MemNum-1;i>=0;i--)
	{
		pstGroupInfo = &pstDianFengJoinInfo->GroupInfo[i];
		if(NULL == pstGroupInfo)
		{
			continue;
		}

		if(pstGroupInfo->RoleID == RoleID)
		{
			return i;
		}
		
	}
	
	return -1;
}

//发竞猜名单。有名字的
int dianfeng_guess_mingdan_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,CSDIANFENGGUESSMINGDAN *pstGuessMingDan)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	CSDIANFENGGUESSMINGDAN *pstDianFengGuessMingDan = &pstMiscSvr->PlayerMiscSvrData.DianFengGuessMingDan;

	pstMiscSvr->PlayerMiscType = PLAYER_DIANFENG_GUESS_MINGDAN;
	
	memcpy(pstDianFengGuessMingDan,pstGuessMingDan,sizeof(*pstDianFengGuessMingDan));
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int dianfeng_clear(ZONESVRENV* pstEnv,int iflag)
{
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;

	DFFIRST *pstDFZFZFirstInfo = &pstDianFengGlobal->DFZFZFirstInfo;
	DFFIRST *pstDFSHZFirstInfo = &pstDianFengGlobal->DFSHZFirstInfo;
	DFFIRST *pstDFYLSFirstInfo = &pstDianFengGlobal->DFYLSFirstInfo;
	DFFIRST *pstDFSYZFirstInfo = &pstDianFengGlobal->DFSYZFirstInfo;
	DFFIRST *pstDFLSZFirstInfo = &pstDianFengGlobal->DFLSZFirstInfo;
	DFFIRST *pstDFHMZFirstInfo = &pstDianFengGlobal->DFHMZFirstInfo;
	DFFIRST *pstDFLHSFirstInfo = &pstDianFengGlobal->DFLHSFirstInfo;
	DFFIRST *pstDFJNZFirstInfo = &pstDianFengGlobal->DFJNZFirstInfo;
	
	//清除冠军信息
	if(1 == iflag)
	{
		dianfeng_chamption_clear(pstEnv,&pstDFZFZFirstInfo->DFChampionInfo);
		dianfeng_chamption_clear(pstEnv,&pstDFSHZFirstInfo->DFChampionInfo);
		dianfeng_chamption_clear(pstEnv,&pstDFYLSFirstInfo->DFChampionInfo);
		dianfeng_chamption_clear(pstEnv,&pstDFSYZFirstInfo->DFChampionInfo);
		dianfeng_chamption_clear(pstEnv,&pstDFLSZFirstInfo->DFChampionInfo);
		dianfeng_chamption_clear(pstEnv,&pstDFHMZFirstInfo->DFChampionInfo);
		dianfeng_chamption_clear(pstEnv,&pstDFLHSFirstInfo->DFChampionInfo);
		dianfeng_chamption_clear(pstEnv,&pstDFJNZFirstInfo->DFChampionInfo);
	}
	//清除雕像信息
	else if(0 == iflag)
	{
		dianfeng_effigy_clear(pstEnv,&pstDFZFZFirstInfo->DFEffigyInfo);
		dianfeng_effigy_clear(pstEnv,&pstDFSHZFirstInfo->DFEffigyInfo);
		dianfeng_effigy_clear(pstEnv,&pstDFYLSFirstInfo->DFEffigyInfo);
		dianfeng_effigy_clear(pstEnv,&pstDFSYZFirstInfo->DFEffigyInfo);
		dianfeng_effigy_clear(pstEnv,&pstDFLSZFirstInfo->DFEffigyInfo);
		dianfeng_effigy_clear(pstEnv,&pstDFHMZFirstInfo->DFEffigyInfo);
		dianfeng_effigy_clear(pstEnv,&pstDFLHSFirstInfo->DFEffigyInfo);
		dianfeng_effigy_clear(pstEnv,&pstDFJNZFirstInfo->DFEffigyInfo);

		dianfeng_effigy_login_set(pstEnv,&pstDFZFZFirstInfo->DFEffigyInfo,DIANFENG_JOB_ZFZ);
		dianfeng_effigy_login_set(pstEnv,&pstDFSHZFirstInfo->DFEffigyInfo,DIANFENG_JOB_SHZ);
		dianfeng_effigy_login_set(pstEnv,&pstDFYLSFirstInfo->DFEffigyInfo,DIANFENG_JOB_YLS);
		dianfeng_effigy_login_set(pstEnv,&pstDFSYZFirstInfo->DFEffigyInfo,DIANFENG_JOB_SYZ);
		dianfeng_effigy_login_set(pstEnv,&pstDFLSZFirstInfo->DFEffigyInfo,DIANFENG_JOB_LSZ);
		dianfeng_effigy_login_set(pstEnv,&pstDFHMZFirstInfo->DFEffigyInfo,DIANFENG_JOB_HMZ);
		dianfeng_effigy_login_set(pstEnv,&pstDFLHSFirstInfo->DFEffigyInfo,DIANFENG_JOB_LHS);
		dianfeng_effigy_login_set(pstEnv,&pstDFJNZFirstInfo->DFEffigyInfo,DIANFENG_JOB_JNZ);
	}
	else
	{
		return -1;
	}
	
	return 0;
}

//在增加报名玩家时，检查是否已经在报名和匹配队伍中了
int player_dianfeng_if_in_baoming_pipei(ZONESVRENV* pstEnv,char *pName,DIANFENGJOININFO *pstDianFengJoinInfo)
{
	int i;
	int iPiPei = 0;
	DIANFENGPIPEI *pstDianFengPiPei = NULL;
	int k = 0;
	PIPEIINFOS *pstPiPei = NULL;
	int MemNum;
	GROUPINFO *pstGroup = NULL;

	if(pstDianFengJoinInfo == NULL || pName == NULL)
	{
		return 0;
	}

	MemNum = pstDianFengJoinInfo->MemNum;
	if(MemNum <= 0)
	{
		goto _pipei;
	}
	
	for(i=0;i<MemNum;i++)
	{
		pstGroup = &pstDianFengJoinInfo->GroupInfo[i];
		if(pstGroup == NULL)
		{
			continue;
		}
		
		//已经在报名队列中了
		if(0 == strcmp(pName,pstGroup->RoleName))
		{
			break;
		}
	}

	if(i>=MemNum)
	{
		goto _pipei;
	}

	return -1;
	
_pipei:
	pstDianFengPiPei = &pstDianFengJoinInfo->DianFengPiPei;
	if(pstDianFengPiPei == NULL)
	{
		return 0;		
	}
	iPiPei = pstDianFengPiPei->PiPeiNum;
	if(iPiPei <= 0)
	{
		return 0;
	}
	
	for(k=0;k<iPiPei;k++)
	{
		pstPiPei = &pstDianFengPiPei->PiPeiInfos[k];
		if(pstPiPei == NULL)
		{
			continue;
		}

		//该玩家已经在匹配中了
		if(0 == strcmp(pstPiPei->RedRoleName,pName))
		{
			break;
		}

		if(0 == strcmp(pstPiPei->BuleRoleName,pName))
		{
			break;
		}
	}

	if(k>=iPiPei)
	{
		return 0;
	}

	return -1;
}

int dianfeng_yanchi_dispose_tm(ZONESVRENV* pstEnv,ZONEPWORLDINST *pstPworldInst)
{
	if(pstPworldInst == NULL)
	{
		return 0;
	}
	
	pstPworldInst->tYanChiTime = pstEnv->pstAppCtx->stCurr.tv_sec + 5;
	
	return 0;
}

//巅峰延迟处理
int dianfeng_yanchi_group_tick(ZONESVRENV* pstEnv,DIANFENGJOININFO *pstDianFengJoinInfo,int iCarceer)
{
	PIPEIINFOS *pstPiPei = NULL;
	Player *pstRedPlayer = NULL;
	Player *pstBulePlayer = NULL;
	int iRound = 0;
	ZONEPWORLDINST *pstPworldInst = NULL;
	ZONEPWORLD *pstPworld = NULL;
	int iPiPei = 0;
	DIANFENGPIPEI *pstDianFengPiPei = NULL;
	int k = 0;
	DIANFENGWINLOSSUI stDianFengWinLoss;
	int i;

	if(pstDianFengJoinInfo == NULL)
	{
		return -1;
	}
	
	pstDianFengPiPei = &pstDianFengJoinInfo->DianFengPiPei;
	if(pstDianFengPiPei == NULL)
	{
		return -1;
	}
	iPiPei = pstDianFengPiPei->PiPeiNum;
	if(iPiPei <= 0)
	{
		return -1;
	}
	
	for(k=iPiPei-1;k>=0;k--)
	{
		pstPiPei = &pstDianFengPiPei->PiPeiInfos[k];
		if(pstPiPei == NULL)
		{
			continue;
		}

		pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstPiPei->PworldInstID);
		if (NULL == pstPworldInst)
		{
			continue;
		}
		
		pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
		if (NULL == pstPworld)
		{
			continue;
		}

		if(pstPworldInst->tYanChiTime < pstEnv->pstAppCtx->stCurr.tv_sec && pstPworldInst->tYanChiTime != 0)
		{
			pstPworldInst->tYanChiTime = 0;

			pstRedPlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,pstPiPei->RedRoleName);
			pstBulePlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,pstPiPei->BuleRoleName);

			iRound = pstPworldInst->iDianFengRound;

			if(0 > dianfeng_pk_end_dispose(pstEnv,pstPiPei,pstRedPlayer,pstBulePlayer,iRound,k,pstPworldInst,pstPworld,iCarceer))
			{
				iRound = pstPworldInst->iDianFengRound;
							
				//0、1、2  才正常
				if(iRound >= MAX_DIANFENG_PK_FIELD_NUM)
				{
					continue;
				}

				if(pstRedPlayer != NULL)
				{
					pstPiPei->DianFengWinLoss.FieldNum = MAX_DIANFENG_PK_FIELD_NUM;
					player_win_loss_ui_to_clt(pstEnv, pstRedPlayer,&pstPiPei->DianFengWinLoss);
				}

				if(pstBulePlayer != NULL)
				{
					memset(&stDianFengWinLoss,0,sizeof(stDianFengWinLoss));
						
					stDianFengWinLoss.FieldNum = MAX_DIANFENG_PK_FIELD_NUM;
			
					//根据红方情况推出蓝方情况
					for(i=0;i<iRound;i++)
					{
						if(pstPiPei->DianFengWinLoss.WinLossInfo[i].Info == 2)
						{
							stDianFengWinLoss.WinLossInfo[i].Info = 1;
						}
						else if(pstPiPei->DianFengWinLoss.WinLossInfo[i].Info == 1)
						{
							stDianFengWinLoss.WinLossInfo[i].Info  = 2;
						}
						else if(pstPiPei->DianFengWinLoss.WinLossInfo[i].Info == 3)
						{
							stDianFengWinLoss.WinLossInfo[i].Info  = 3;
						}
						else
						{
							stDianFengWinLoss.WinLossInfo[i].Info = 0;
						}	
					}

					player_win_loss_ui_to_clt(pstEnv, pstBulePlayer,&stDianFengWinLoss);
				}
			}		
		}
		
	}

	return 0;
}

int dianfeng_yanchi_tick(ZONESVRENV *pstEnv,DIANFENGGLOBAL *pstDianFengGlobal)
{
	DIANFENGJOININFO *pstDianFengZFZJoinInfo = &pstDianFengGlobal->DianFengZFZJoinInfo;
	DIANFENGJOININFO *pstDianFengSHZJoinInfo = &pstDianFengGlobal->DianFengSHZJoinInfo;
	DIANFENGJOININFO *pstDianFengYLSJoinInfo = &pstDianFengGlobal->DianFengYLSJoinInfo;
	DIANFENGJOININFO *pstDianFengSYZJoinInfo = &pstDianFengGlobal->DianFengSYZJoinInfo;
	DIANFENGJOININFO *pstDianFengLSZJoinInfo = &pstDianFengGlobal->DianFengLSZJoinInfo;
	DIANFENGJOININFO *pstDianFengHMZJoinInfo = &pstDianFengGlobal->DianFengHMZJoinInfo;
	DIANFENGJOININFO *pstDianFengLHSJoinInfo = &pstDianFengGlobal->DianFengLHSJoinInfo;
	DIANFENGJOININFO *pstDianFengJNZJoinInfo = &pstDianFengGlobal->DianFengJNZJoinInfo;

	dianfeng_yanchi_group_tick(pstEnv,pstDianFengZFZJoinInfo,CAREER_WARRIOR_DOUBLEHAND);
	dianfeng_yanchi_group_tick(pstEnv,pstDianFengSHZJoinInfo,CAREER_WARRIOR_SINGLEHAND);
	dianfeng_yanchi_group_tick(pstEnv,pstDianFengYLSJoinInfo,CAREER_CHURCH_DOUBLEHAND);
	dianfeng_yanchi_group_tick(pstEnv,pstDianFengSYZJoinInfo,CAREER_CHURCH_SINGLEHAND);
	dianfeng_yanchi_group_tick(pstEnv,pstDianFengLSZJoinInfo,CAREER_GUNER_SINGLEHAND);
	dianfeng_yanchi_group_tick(pstEnv,pstDianFengHMZJoinInfo,CAREER_GUNER_DOUBLEHAND);
	dianfeng_yanchi_group_tick(pstEnv,pstDianFengLHSJoinInfo,CAREER_MAGICOR_DOUBLEHAND);
	dianfeng_yanchi_group_tick(pstEnv,pstDianFengJNZJoinInfo,CAREER_MAGICOR_SINGLEHAND);

	return 0;
}

//回血，回魔法
int dianfeng_revive_sth(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	if(pstPlayer == NULL)
	{
		return 0;
	}
	
	pstPlayer->stOnline.iReviverID = 0;
	pstPlayer->stOnline.State &= ~CS_STAT_DEAD;

	player_revive_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
					pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax, pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax);
	
	player_revive_action(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	return 0;
}

//0 表示他就是冠军了，-1不是,如果是冠军，就不要提示进入下一轮了
int dianfeng_is_first_msg(ZONESVRENV *pstEnv,int iWhichJob,DIANFENGGLOBAL *pstDianFengGlobal)
{
	DIANFENGJOININFO *pstDianFengJoinInfo = NULL;	
			
	//按照职业添加参加人数和数据
	if(0 > dianfeng_get_joininfo_by_career(pstEnv,&pstDianFengJoinInfo,pstDianFengGlobal,iWhichJob))
		return -1;

	if(NULL == pstDianFengJoinInfo)
	{
		return -1;
	}
	
	DIANFENGPIPEI *pstDianFengPiPei = &pstDianFengJoinInfo->DianFengPiPei;
	
	int MemNum = pstDianFengJoinInfo->MemNum;

	if(MemNum == 1 && pstDianFengPiPei->PiPeiNum == 0)
	{		
		return 0;
	}

	return -1;
}

//冠军界面公会信息变动时。要同步更新
int dianfeng_chamption_clan_info_update(ZONESVRENV *pstEnv,Player *pstPlayer,unsigned int ClanId)
{
	if(pstEnv->pstConf->DianFengFlag == 0 || (is_fini_global(pstEnv) == 0) || get_curr_max_level(pstEnv) < 65)
	{
		return 0;
	}
	
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;
	DFFIRSTINFO *pstChampionInfo = NULL;

	DIANFENGJOININFO *pstDianFengJoinInfo = NULL;
	int i;
	GROUPINFO *pstGroupInfo = NULL;
	int iMemNum = 0;
	
	if(pstPlayer == NULL)
	{
		return -1;
	}
	
	switch(pstPlayer->stRoleData.Career)
	{
		case CAREER_WARRIOR_SINGLEHAND:
			pstChampionInfo = &pstDianFengGlobal->DFSHZFirstInfo.DFChampionInfo;
			pstDianFengJoinInfo = &pstDianFengGlobal->DianFengSHZJoinInfo;
			break;
		case CAREER_WARRIOR_DOUBLEHAND: 
			pstChampionInfo = &pstDianFengGlobal->DFZFZFirstInfo.DFChampionInfo;
			pstDianFengJoinInfo = &pstDianFengGlobal->DianFengZFZJoinInfo;
			break;
		case CAREER_MAGICOR_SINGLEHAND:
			pstChampionInfo = &pstDianFengGlobal->DFJNZFirstInfo.DFChampionInfo; 
			pstDianFengJoinInfo = &pstDianFengGlobal->DianFengJNZJoinInfo;
			break;
		case CAREER_MAGICOR_DOUBLEHAND: 
			pstChampionInfo = &pstDianFengGlobal->DFLHSFirstInfo.DFChampionInfo; 
			pstDianFengJoinInfo = &pstDianFengGlobal->DianFengLHSJoinInfo;
			break;
		case CAREER_GUNER_SINGLEHAND:
			pstChampionInfo = &pstDianFengGlobal->DFLSZFirstInfo.DFChampionInfo; 
			pstDianFengJoinInfo = &pstDianFengGlobal->DianFengLSZJoinInfo;
			break;
		case CAREER_GUNER_DOUBLEHAND:
			pstChampionInfo = &pstDianFengGlobal->DFHMZFirstInfo.DFChampionInfo; 
			pstDianFengJoinInfo = &pstDianFengGlobal->DianFengHMZJoinInfo;
			break;
		case CAREER_CHURCH_SINGLEHAND:
			pstChampionInfo = &pstDianFengGlobal->DFSYZFirstInfo.DFChampionInfo;  
			pstDianFengJoinInfo = &pstDianFengGlobal->DianFengSYZJoinInfo;
			break;
		case CAREER_CHURCH_DOUBLEHAND:	
			pstChampionInfo = &pstDianFengGlobal->DFYLSFirstInfo.DFChampionInfo; 
			pstDianFengJoinInfo = &pstDianFengGlobal->DianFengYLSJoinInfo;
			break;	
		default:
			return -1;
			break;
	}

	if(pstChampionInfo == NULL)
	{
		return -1;
	}
	
	if(0 == strcmp(pstPlayer->stRoleData.RoleName,pstChampionInfo->RoleName))
	{
		pstChampionInfo->ClanId = ClanId;
	}

	if(pstDianFengJoinInfo == NULL)
	{
		return -1;
	}

	iMemNum = min(pstDianFengJoinInfo->MemNum,MAX_DIANFENG_EVERY_GROUP_MEM_NUM);
	
	if(iMemNum <= 0)
	{
		return -1;
	}

	for(i=0;i<iMemNum;i++)
	{
		pstGroupInfo = &pstDianFengJoinInfo->GroupInfo[i];
		if(NULL == pstGroupInfo)
		{
			continue;
		}

		if(0 == strcmp(pstGroupInfo->RoleName,pstPlayer->stRoleData.RoleName))
		{
			pstGroupInfo->ClanID = ClanId;
			break;
		}
	}
	
	return 0;
}

int dianfeng_guang_bo(ZONESVRENV *pstEnv,Player *pstPlayer)
{
	CSSYSMSG *pstMsg = NULL;
	CSPKG *pstCSRes = &pstEnv->stCSRes;

	Z_CSHEAD_INIT(&pstCSRes->Head, SYS_MSG);
	pstMsg = &pstCSRes->Body.SysMsg;
	pstMsg->Type = SYS_MIDDLE2;
	pstMsg->Times = 0;
	pstMsg->Interval = 0;
	pstMsg->LinkLen = 0;
	snprintf(pstMsg->Msg, CCH(pstMsg->Msg), LJY_SYSMSG25,pstPlayer->stRoleData.RoleName);
	pstMsg->Msg[sizeof(pstMsg->Msg) - 1] = 0;
	z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, pstCSRes);

	return 0;
}

//看看是否有定身buff
int  if_player_has_dingshen_status(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	ZoneStatus *pstZoneStatus;
	STATUSDEF *pstStatusDef;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;

	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		
		if(pstStatusDef->StatusID == DINGSHNE_BUFF)
		{
			return 1;
		}
	}

	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		
		if(pstStatusDef->StatusID == DINGSHNE_BUFF)
		{
			return 1;
		}
	}

	for (i=0; i<pstStatusList->bNoLimitStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		
		if(pstStatusDef->StatusID == DINGSHNE_BUFF)
		{
			return 1;
		}
	}

	return -1;
}

//离开巅峰就视为自动放弃比赛
int player_leave_dianfeng_repose(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if(NULL == pstPlayer)
	{
		return -1;
	}
	
	if (pstPlayer->stRoleData.Map == SHZ_READY_MAP_ID || pstPlayer->stRoleData.Map == ZHZ_READY_MAP_ID ||
		pstPlayer->stRoleData.Map == JNZ_READY_MAP_ID || pstPlayer->stRoleData.Map == LHS_READY_MAP_ID ||
		pstPlayer->stRoleData.Map == LSZ_READY_MAP_ID || pstPlayer->stRoleData.Map == HMZ_READY_MAP_ID ||
		pstPlayer->stRoleData.Map == SYZ_READY_MAP_ID || pstPlayer->stRoleData.Map == YLS_READY_MAP_ID ||
		(GET_PWORLDID(pstPlayer->stRoleData.Map) == DIANFENG_PWORLD_ID))
	{
		DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;
		int i;
		DIANFENGJOININFO *pstDianFengJoinInfo = NULL;
		int iPiPei = 0;
		DIANFENGPIPEI *pstDianFengPiPei = NULL;
		int k = 0;
		PIPEIINFOS *pstPiPei = NULL;
		int flag = 0;
		int ColorFlag = 0;
		ZONEPWORLDINST *pstZonePworldInst = NULL;
		ZONEPWORLD *pstPworld = NULL;
		Player *pstTempPlayer = NULL;
		int MemNum;
		DIANFENGINFO *pstDianFengInfo = NULL;
		
		if(0 > dianfeng_get_joininfo_by_career(pstEnv,&pstDianFengJoinInfo,pstDianFengGlobal,pstPlayer->stRoleData.Career))
			return -1;

		pstDianFengPiPei = &pstDianFengJoinInfo->DianFengPiPei;
		if(pstDianFengPiPei == NULL)
		{
			return -1;
		}
		iPiPei = pstDianFengPiPei->PiPeiNum;
		if(iPiPei <= 0)
		{
			flag = 1;
		}
		
		for(k=iPiPei-1;k>=0;k--)
		{
			pstPiPei = &pstDianFengPiPei->PiPeiInfos[k];
			if(pstPiPei == NULL)
			{
				continue;
			}
			
			if(0 == strcmp(pstPiPei->RedRoleName,pstPlayer->stRoleData.RoleName))
			{
				ColorFlag = 1;
			}

			if(0 == strcmp(pstPiPei->BuleRoleName,pstPlayer->stRoleData.RoleName))
			{
				ColorFlag = 2;
			}

			if(ColorFlag == 1 || ColorFlag == 2)
			{
				//先找出来
				pstZonePworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstPiPei->PworldInstID);
				if(pstZonePworldInst != NULL)
				{
					pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
				}
			
				if(ColorFlag == 1)
				{
					pstTempPlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,pstPiPei->BuleRoleName);
					//蓝方再添加到报名队列,并把匹配队列该双人去除
					player_career_pipei_del_baoming_add(pstEnv,pstPiPei->BuleRoleName,k,pstPlayer->stRoleData.Career);
				}
				else
				{
					pstTempPlayer = z_name_player(pstEnv->pstAppCtx,pstEnv,pstPiPei->RedRoleName);
					//红方再添加到报名队列,并把匹配队列该双人去除
					player_career_pipei_del_baoming_add(pstEnv,pstPiPei->RedRoleName,k,pstPlayer->stRoleData.Career);
				}
									
				if (NULL != pstZonePworldInst && NULL != pstPworld)
				{
					if(NULL != pstTempPlayer)
					{
						if(0 > dianfeng_is_first_msg(pstEnv,pstTempPlayer->stRoleData.Career,pstDianFengGlobal))
						{
							z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTempPlayer, SYS_MIDDLE2 , ZONEERR_DIANFENG39);
						}
						else
						{
							player_status_clean_all(pstEnv->pstAppCtx, pstEnv, pstTempPlayer);
							//为冠军时，还原状态
							dianfeng_restore_status(pstEnv,pstPworld,pstTempPlayer);
						}
					}
					
					z_in_destroy_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstZonePworldInst);
				}
				else
				{
					if(NULL != pstTempPlayer)
					{
						if(0 > dianfeng_is_first_msg(pstEnv,pstTempPlayer->stRoleData.Career,pstDianFengGlobal))
						{
							z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstTempPlayer, SYS_MIDDLE2 , ZONEERR_DIANFENG40);
							
							player_pipei_succeed_ui_to_clt(pstEnv, pstTempPlayer,2);
						}
					}

					player_pipei_succeed_ui_to_clt(pstEnv, pstPlayer,2);
				}

				//这里其实没啥用了，为了防止重新报名出现"你已报名"的提示
				pstDianFengInfo = &pstPlayer->stRoleData.MiscInfo.DianFengInfo;
				pstDianFengInfo->DianFengFlag &= ~IF_DIANFENG_BAOMING;
					
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP , ZONEERR_DIANFENG41);
				
				break;
			}
		}

		//不在匹配中
		if(k<0)
		{
			flag = 1;
		}

		if(flag == 1)
		{
			//在报名队列中
			MemNum = pstDianFengJoinInfo->MemNum;
			if(MemNum <= 0)
			{
				return -1;
			}
			
			for(i=MemNum-1;i>=0;i--)
			{
				if(0 == strcmp(pstDianFengJoinInfo->GroupInfo[i].RoleName,pstPlayer->stRoleData.RoleName))
				{
					pstDianFengJoinInfo->MemNum--;
					if(i != pstDianFengJoinInfo->MemNum)
					{
						memcpy(&pstDianFengJoinInfo->GroupInfo[i],&pstDianFengJoinInfo->GroupInfo[pstDianFengJoinInfo->MemNum],sizeof(pstDianFengJoinInfo->GroupInfo[i]));
					}

					//假设是在ACTIVE_READY  时自动放弃，还可以重新报名
					pstDianFengInfo = &pstPlayer->stRoleData.MiscInfo.DianFengInfo;
					pstDianFengInfo->DianFengFlag &= ~IF_DIANFENG_BAOMING;
	
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP , ZONEERR_DIANFENG41);
					break;
				}
			}

			//不在报名队列中
			if(i<0)
			{
				return -1;
			}
		}
	}
	
	return 0;
}

int dianfeng_check_tick(ZONESVRENV* pstEnv,DIANFENGGLOBAL *pstDianFengGlobal)
{
	DIANFENGJOININFO *pstDianFengZFZJoinInfo = &pstDianFengGlobal->DianFengZFZJoinInfo;
	DIANFENGJOININFO *pstDianFengSHZJoinInfo = &pstDianFengGlobal->DianFengSHZJoinInfo;
	DIANFENGJOININFO *pstDianFengYLSJoinInfo = &pstDianFengGlobal->DianFengYLSJoinInfo;
	DIANFENGJOININFO *pstDianFengSYZJoinInfo = &pstDianFengGlobal->DianFengSYZJoinInfo;
	DIANFENGJOININFO *pstDianFengLSZJoinInfo = &pstDianFengGlobal->DianFengLSZJoinInfo;
	DIANFENGJOININFO *pstDianFengHMZJoinInfo = &pstDianFengGlobal->DianFengHMZJoinInfo;
	DIANFENGJOININFO *pstDianFengLHSJoinInfo = &pstDianFengGlobal->DianFengLHSJoinInfo;
	DIANFENGJOININFO *pstDianFengJNZJoinInfo = &pstDianFengGlobal->DianFengJNZJoinInfo;

	dianfeng_group_check_tick(pstEnv,pstDianFengZFZJoinInfo,ZHZ_READY_MAP_ID);
	dianfeng_group_check_tick(pstEnv,pstDianFengSHZJoinInfo,SHZ_READY_MAP_ID);
	dianfeng_group_check_tick(pstEnv,pstDianFengYLSJoinInfo,YLS_READY_MAP_ID);
	dianfeng_group_check_tick(pstEnv,pstDianFengSYZJoinInfo,SYZ_READY_MAP_ID);
	dianfeng_group_check_tick(pstEnv,pstDianFengLSZJoinInfo,LSZ_READY_MAP_ID);
	dianfeng_group_check_tick(pstEnv,pstDianFengHMZJoinInfo,HMZ_READY_MAP_ID);
	dianfeng_group_check_tick(pstEnv,pstDianFengLHSJoinInfo,LHS_READY_MAP_ID);
	dianfeng_group_check_tick(pstEnv,pstDianFengJNZJoinInfo,JNZ_READY_MAP_ID);
	
	return 0;
}

int dianfeng_group_check_tick(ZONESVRENV* pstEnv,DIANFENGJOININFO *pstDianFengJoinInfo,int iMap)
{
	int i;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	AREADYN *pstArea;
	Player *pstPlayer;
	int iIndex;
	int j;
	RESPOS ResPos;
	int iMapIndex;
	DIANFENGINFO *pstDianFengInfo = NULL;

	stIdx.iID = iMap;
	stIdx.iPos = -1;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	
	for (i = 0; i < pstMapInst->iAreaDyn; i++) 
	{
		iIndex	=	pstMapInst->aiAreaDyn[i];

		if( iIndex<0)
		{
			continue;
		}

		pstArea	=	(AREADYN*)tmempool_get(pstEnv->pstAreaDynPool, iIndex);
		if (NULL == pstArea)
		{
			continue;
		}
		
		for (j = 0; j < pstArea->iAnimate; j++) 
		{
			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstArea->aiAnimate[j]);
			if(NULL == pstPlayer || PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
			{
				continue;
			}

			if(0 == player_dianfeng_if_in_baoming_pipei(pstEnv,pstPlayer->stRoleData.RoleName,pstDianFengJoinInfo))
			{
				//不在报名和匹配队列中，一律踢出该地图
				pstDianFengInfo = &pstPlayer->stRoleData.MiscInfo.DianFengInfo;
				iMapIndex = pstDianFengInfo->MapId;
				ResPos.X = pstDianFengInfo->ResPos.X;
				ResPos.Y = pstDianFengInfo->ResPos.Y;
				z_dianfeng_change_map(pstEnv, pstPlayer,iMapIndex, ResPos ,0);
			}

		}
	}

	return 0;
}

//  踢出时，还原状态    
int dianfeng_restore_status(ZONESVRENV* pstEnv,ZONEPWORLD *pstPworld,Player *pstPlayer)
{
	if (pstPworld && pstPworld->stPworldDef.CtrlFlag & PWORLD_CLEAN_STATUS)
	{
		if(pstPlayer)
		{
			z_status_online(pstEnv->pstAppCtx, pstEnv, pstPlayer , 1);
			z_player_send_status(pstEnv->pstAppCtx, pstEnv, pstPlayer );
			z_player_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer);
			z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		}
	}
	
	return 0;		
}

int dianfeng_clear_champion_by_career(ZONESVRENV* pstEnv,Player *pstPlayer,int Career)
{
	DFFIRSTINFO *pstDFChampionInfo = NULL;
	DIANFENGGLOBAL *pstDianFengGlobal = &pstEnv->pstGlobalObj->stGlobal.Misc.DianFengGlobal;

	switch(Career)
	{
		case CAREER_WARRIOR_SINGLEHAND:
			pstDFChampionInfo = &pstDianFengGlobal->DFSHZFirstInfo.DFChampionInfo;
			break;
		case CAREER_WARRIOR_DOUBLEHAND: 
			pstDFChampionInfo = &pstDianFengGlobal->DFZFZFirstInfo.DFChampionInfo;
			break;
		case CAREER_MAGICOR_SINGLEHAND:
			pstDFChampionInfo = &pstDianFengGlobal->DFJNZFirstInfo.DFChampionInfo;
			break;
		case CAREER_MAGICOR_DOUBLEHAND: 
			pstDFChampionInfo = &pstDianFengGlobal->DFLHSFirstInfo.DFChampionInfo;
			break;
		case CAREER_GUNER_SINGLEHAND:
			pstDFChampionInfo = &pstDianFengGlobal->DFLSZFirstInfo.DFChampionInfo;
			break;
		case CAREER_GUNER_DOUBLEHAND:
			pstDFChampionInfo = &pstDianFengGlobal->DFHMZFirstInfo.DFChampionInfo;
			break;
		case CAREER_CHURCH_SINGLEHAND:
			pstDFChampionInfo = &pstDianFengGlobal->DFSYZFirstInfo.DFChampionInfo;
			break;
		case CAREER_CHURCH_DOUBLEHAND:	
			pstDFChampionInfo = &pstDianFengGlobal->DFYLSFirstInfo.DFChampionInfo;
			break;	
		default:
			return -1;
			break;
	}

	if(NULL == pstDFChampionInfo)
	{
		return -1;
	}

	if(0 == strcmp(pstPlayer->stRoleData.RoleName,pstDFChampionInfo->RoleName))
	{
		//是冠军
		memset(pstDFChampionInfo,0,sizeof(*pstDFChampionInfo));
	}

	return 0;
}

