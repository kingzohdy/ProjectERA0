/*
**  @file $RCSfile: zone_oplog.c,v $
**  general description of this module
**  $Id: zone_oplog.c,v 1.203 2014/06/24 09:55:14 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2014/06/24 09:55:14 $
**  @version $Revision: 1.203 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_oplog.h"
#include "zone_package.h"
#include "zone_npc.h"
#include "zone_skill.h"
#include "zone_clt.h"
#include "zone_db.h"
#include "zone_rune.h"
#include "zone_span.h"
#include "zone_clan.h"
#include "zone_map.h"
#include "zone_strongpoint.h"
#include "zone_span_strong.h"

extern int tlog_init_cfg_from_file(TLOGCONF* a_pstConf, const char *a_pszPath);
extern unsigned char g_szMetalib_op_log_meta[];

void z_oplog_event_begin(ZONESVRENV* pstEnv)
{
	pstEnv->ullEventLogWID = z_get_gid(pstEnv->pstAppCtx, pstEnv);
	return ;
}

tdr_ulonglong z_get_logwid(ZONESVRENV* pstEnv)
{
	return pstEnv->ullEventLogWID;
}

void z_set_logwid(ZONESVRENV* pstEnv, tdr_ulonglong ullWID)
{
	pstEnv->ullEventLogWID = ullWID;
	return ;
}

 ROLEMEMBER * z_rolemem_warp( const Player * pstPlayer, ROLEMEMBER * pstRoleMember )
{
	bzero( pstRoleMember, sizeof(*pstRoleMember) );
	if( !pstPlayer )
	{
		return pstRoleMember;
	}
	
	pstRoleMember->RoleID = pstPlayer->stRoleData.RoleID;
	STRNCPY( pstRoleMember->RoleName, 
		pstPlayer->stRoleData.RoleName, CCH(pstRoleMember->RoleName) );
	return pstRoleMember;
}


int z_oplog_init(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	char szPath[PATH_MAX];

	snprintf(szPath, sizeof(szPath), "../cfg/op_log_%s_66.xml", tbus_addr_ntoa( (TBUSADDR) pstAppCtx->iId) );
	szPath[sizeof(szPath) - 1] = 0;		
	
	if (0 > tlog_init_cfg_from_file(&pstEnv->stOpLogConf, szPath))	
	{		
		printf("load op log cfg file fail\n");
		return -1;	
	}	

	if (0 > tlog_init(&pstEnv->stOpLogCtx, &pstEnv->stOpLogConf))
	{		
		printf("init op log cfg file fail\n");
		return -1;
	}

	pstEnv->pstOpLogCat = tlog_get_category(&pstEnv->stOpLogCtx, "oplog_net");
	if (NULL == pstEnv->pstOpLogCat)
	{		
		printf("load op log cfg filefail\n");
		return -1;
	}

	pstEnv->pstLogicLogMeta = tdr_get_meta_by_name((LPTDRMETALIB)g_szMetalib_op_log_meta, "op_log_meta");	
	if (NULL == pstEnv->pstLogicLogMeta)
	{
		return -1;
	}

	return 0;
}

int z_oplog_fini(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	UNUSED(pstAppCtx);
	tlog_fini(&pstEnv->stOpLogCtx);

	return 0;
}

int z_oplog_proxy_send(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, OP_LOG_META *pstOpLog,int iFrom)
{
	TDRDATA stHost;	
	TDRDATA stNet;
	char sBuff[2*sizeof(OP_LOG_META)];
	UNUSED(pstAppCtx);

	pstOpLog->OpLogFrom = iFrom;

	stNet.iBuff = sizeof(sBuff);
	stNet.pszBuff = sBuff;
	stHost.iBuff = sizeof(OP_LOG_META);
	stHost.pszBuff = (char *)pstOpLog;	

	if (0 > tdr_hton(pstEnv->pstLogicLogMeta, &stNet, &stHost, TDR_METALIB_OP_LOG_META_VERSION))
	{
		return -1;
	}	

	tlog_info_bin(pstEnv->pstOpLogCat, 0, 0, 0, TDR_METALIB_OP_LOG_META_VERSION, stNet.pszBuff, stNet.iBuff);
	
	return 0;
}

int z_oplog_send(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, OP_LOG_META *pstOpLog)
{
	z_oplog_proxy_send(pstAppCtx,pstEnv,pstOpLog,OP_LOG_FROM_LOCAL);
	return 0;
}


int z_role_oplog_proxy_send(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, OP_LOG_META *pstOpLog,Player* pstPlayer)
{
	// 如果是battle_svr,转发给zone_svr处理
	if(pstEnv->iInstMode == INST_MODE_BATTLE_SVR && pstPlayer)
	{
		SSPKG stSSPkg;
		SSCMDREQ* pstReq = &stSSPkg.Body.CmdReq;
		int iBusID = player_master_busid_get(pstEnv, pstPlayer);
		unsigned char ucInstance = 0;
			
		GET_INSTANCE_ENTITY(ucInstance,pstEnv->pstAppCtx->iId);
		pstOpLog->OpLogFrom = ucInstance;

		memset(pstReq,0,sizeof(*pstReq));
		pstReq->Cmd = SS_CMD_OPLOG_PROXY;
		pstReq->Data.OplogProxy = *pstOpLog;		
		
		Z_SSHEAD_INIT(&stSSPkg.Head, SS_CMD_REQ);
		z_ssmsg_send(pstEnv, &stSSPkg, iBusID);
	}
	else
	{
		return z_oplog_send(pstAppCtx, pstEnv, pstOpLog);
	}

	return 0;

}


int z_role_loging_oplog(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	OP_LOG_META stOpLog;
	OPROLELOGIN *pstRoleLogin = &stOpLog.OpLog.RoleLogin;

	stOpLog.OpLogType = OP_LOG_ROLELOGIN;

	pstRoleLogin->Uin = pstPlayer->stRoleData.Uin;
	pstRoleLogin->RoleID = pstPlayer->stRoleData.RoleID;
	STRNCPY(pstRoleLogin->RoleName, pstPlayer->stRoleData.RoleName, CCH(pstRoleLogin->RoleName));
	STRNCPY(pstRoleLogin->AccountLoginName, pstPlayer->szAccount, CCH(pstRoleLogin->AccountLoginName));
	STRNCPY(pstRoleLogin->MacAddr, pstPlayer->aMacAdd, CCH(pstRoleLogin->MacAddr));
	STRNCPY(pstRoleLogin->IP, inet_ntoa(*(struct in_addr *)&pstPlayer->stConnectInfo.stExtraInfo.IPInfo.ip), CCH(pstRoleLogin->IP));
	
	return z_oplog_send(pstAppCtx, pstEnv, &stOpLog);
}

int z_rolenew_oplog(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,
						tdr_ulonglong ullRoleID, int iCareer, int iGender,char *pszName)
{
	OP_LOG_META stOpLog;
	OPROLENEW *pstRoleNew = &stOpLog.OpLog.RoleNew;

	stOpLog.OpLogType = OP_LOG_ROLENEW;
	pstRoleNew->Uin = pstPlayer->stRoleData.Uin;
	pstRoleNew->RoleID = ullRoleID; // 角色ID
	pstRoleNew->Career = iCareer; // 角色职业
	pstRoleNew->Gender = iGender;
	STRNCPY(pstRoleNew->RoleName, pszName, sizeof(pstRoleNew->RoleName));
	STRNCPY(pstRoleNew->AccountLoginName, pstPlayer->szAccount, CCH(pstRoleNew->AccountLoginName));
	
	return z_oplog_send(pstAppCtx, pstEnv, &stOpLog);
}

int z_rolelogout_oplog(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	OP_LOG_META stOpLog;
	OPROLELOGOUT * pstRoleLogOut = &stOpLog.OpLog.RoleLogout;

	stOpLog.OpLogType = OP_LOG_ROLELOGOUT;

	pstRoleLogOut->Uin = pstPlayer->stRoleData.Uin;
	pstRoleLogOut->RoleID = pstPlayer->stRoleData.RoleID;
	STRNCPY(pstRoleLogOut->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstRoleLogOut->RoleName));
	STRNCPY(pstRoleLogOut->AccountLoginName, pstPlayer->szAccount, CCH(pstRoleLogOut->AccountLoginName));
	STRNCPY(pstRoleLogOut->IP, inet_ntoa(*(struct in_addr *)&pstPlayer->stConnectInfo.stExtraInfo.IPInfo.ip), CCH(pstRoleLogOut->IP));
	pstRoleLogOut->Reason = pstPlayer->ucRoleLogoutReason;
	pstRoleLogOut->Duration = pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.tRoleLogin;
	
	return z_oplog_send(pstAppCtx, pstEnv, &stOpLog);
}

int z_roleshop_oplog(ZONESVRENV* pstEnv, Player *pstPlayer ,ITEMDEF *pstItemDef , OPSHOPDATA*OldShopData 
	, int iNum , tdr_ulonglong llWid, int ShopAct, ITEMDEC *pstItemDec, int iClanConDec)
{
	OP_LOG_META stOpLog;
	OPROLESHOP *pstRoleShop = &stOpLog.OpLog.RoleShop;

	stOpLog.OpLogType = OP_LOG_ROLESHOP;

	memset(&stOpLog.OpLog.RoleShop ,0 ,sizeof(OPROLESHOP));
		
	stOpLog.OpLog.RoleShop.ShopItemData.ItemType = SHOP_ITEM_ITEM;

	pstRoleShop->Uin = pstPlayer->stRoleData.Uin;
	pstRoleShop->RoleID = pstPlayer->stRoleData.RoleID;
	STRNCPY(pstRoleShop->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstRoleShop->RoleName));
	STRNCPY(pstRoleShop->AccountLoginName, pstPlayer->szAccount, CCH(pstRoleShop->AccountLoginName));
	pstRoleShop->ShopAct = ShopAct; 

	
	pstRoleShop->NowShopData.Money = pstPlayer->stRoleData.RoleDetail.Money;
	pstRoleShop->NowShopData.Bull = pstPlayer->stRoleData.RoleDetail.Bull;

	pstRoleShop->OldShopData.Money = OldShopData->Money;
	pstRoleShop->OldShopData.Bull= OldShopData->Bull;
	if (pstItemDec)
		pstRoleShop->ItemDec = *pstItemDec;
	pstRoleShop->ClanConDec = iClanConDec;

	switch (ShopAct)
	{
		case OP_SHOP_SELL:
		case OP_SHOP_BUY:
		case OP_SHOP_BUYSELL:	
		case OP_SHOP_REPONE:
			if ( ShopAct == OP_SHOP_REPONE)
			{
				pstRoleShop->OldShopData.GoodsDur= OldShopData->GoodsDur;
				pstRoleShop->NowShopData.GoodsDur= iNum;
			}
			else
			{
				pstRoleShop->OldShopData.GoodsNum= OldShopData->GoodsNum;
				pstRoleShop->NowShopData.GoodsNum= z_get_pak_item_num(pstEnv, &pstPlayer->stRoleData.Package, pstItemDef->ItemID, -1,0,PACKAGE_DEC_NONE);
				pstRoleShop->ShopItemData.ItemData.GoodsData.Num = iNum;
			}

			pstRoleShop->ShopItemData.ItemData.GoodsData.GoodsId = pstItemDef->ItemID;
			pstRoleShop->ShopItemData.ItemData.GoodsData.GoodsMoney1= pstItemDef->BuyPrice;
			pstRoleShop->ShopItemData.ItemData.GoodsData.GoodsMoneyType1 = pstItemDef->MoneyType;
			pstRoleShop->ShopItemData.ItemData.GoodsData.GoodsWid = llWid;

			
			if (ShopAct == OP_SHOP_BUY)
			{
				if (pstItemDef->MoneyUses[0].ID != 0 &&
					pstItemDef->MoneyUses[0].Val != 0)
				{
					pstRoleShop->ShopItemData.ItemData.GoodsData.GoodsMoney2 = pstItemDef->MoneyUses[0].Val;
					pstRoleShop->ShopItemData.ItemData.GoodsData.GoodsMoneyType2 = pstItemDef->MoneyUses[0].ID;
				}

				if (pstItemDef->MoneyUses[1].ID != 0 &&
					pstItemDef->MoneyUses[1].Val != 0)
				{
					pstRoleShop->ShopItemData.ItemData.GoodsData.GoodsMoney3 = pstItemDef->MoneyUses[1].Val;
					pstRoleShop->ShopItemData.ItemData.GoodsData.GoodsMoneyType3 = pstItemDef->MoneyUses[1].ID;
				}

				if (pstItemDef->MoneyUses[2].ID != 0 &&
					pstItemDef->MoneyUses[2].Val != 0)
				{
					pstRoleShop->ShopItemData.ItemData.GoodsData.GoodsMoney4 = pstItemDef->MoneyUses[2].Val;
					pstRoleShop->ShopItemData.ItemData.GoodsData.GoodsMoneyType4 = pstItemDef->MoneyUses[2].ID;
				}
			}
			break;
		case OP_SHOP_REPALL:
			break;
			

	}
	//return z_oplog_send(NULL, pstEnv, &stOpLog);

	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_rolemail_oplog( ZONESVRENV* pstEnv, Player *pstPlayer, int iOperation, const MAIL * pstMail )
{
	unsigned int i;
	OP_LOG_META stOpLog;
	OPROLEMAIL * pstOpMail = &stOpLog.OpLog.RoleMail;

	bzero(pstOpMail, sizeof(*pstOpMail));
	
	pstOpMail->Operation = (unsigned char)iOperation;
	if (pstPlayer)
	{
		pstOpMail->Uin = pstPlayer->stRoleData.Uin;
		pstOpMail->RoleID = pstPlayer->stRoleData.RoleID;
		STRNCPY( pstOpMail->RoleName, pstPlayer->stRoleData.RoleName, CCH(pstOpMail->RoleName) );
		STRNCPY(pstOpMail->AccountLoginName, pstPlayer->szAccount, CCH(pstOpMail->AccountLoginName));
	}

	pstOpMail->RoleMailData.WID = pstMail->Head.WID;
	pstOpMail->RoleMailData.Flags = pstMail->Head.Flags;
	pstOpMail->RoleMailData.ItemNum = pstMail->Detail.ItemNum;
	pstOpMail->RoleMailData.Money = pstMail->Detail.Gold;
	pstOpMail->RoleMailData.Time = pstMail->Head.Time;

	i = pstMail->Detail.ItemNum;
	while(i--)
	{
		pstOpMail->RoleMailData.Items[i] = pstMail->Detail.Items[i];
	}

	STRNCPY( pstOpMail->RoleMailData.Send, pstMail->Head.Send, CCH(pstOpMail->RoleMailData.Send) );
	STRNCPY( pstOpMail->RoleMailData.Title, pstMail->Head.Title, CCH(pstOpMail->RoleMailData.Title) );
	STRNCPY( pstOpMail->RoleMailData.To, pstMail->Head.Recv, CCH(pstOpMail->RoleMailData.To) );
	

	stOpLog.OpLogType = OP_LOG_ROLEMAIL;
	//return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog );

	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_role_get_mailgoods_oplog(ZONESVRENV *pstEnv, Player *pstPlayer, 
	                             const OPROLEGETMAILGOODS *pstGoods)
{
	unsigned int i;
	OP_LOG_META stOpLog;
	OPROLEGETMAILGOODS * pstOpGoods = &stOpLog.OpLog.GetMailGoods;

	pstOpGoods->Uin = pstPlayer->stRoleData.Uin;
	STRNCPY( pstOpGoods->AccountLoginName, pstPlayer->szAccount, CCH( pstOpGoods->AccountLoginName) );
	z_rolemem_warp(pstPlayer, &pstOpGoods->RoleInfo);
	pstOpGoods->MailWID = pstGoods->MailWID;
	pstOpGoods->Money = pstGoods->Money;
	pstOpGoods->ItemNum = pstGoods->ItemNum;
	i = pstGoods->ItemNum;
	while(i--)
	{
		pstOpGoods->Items[i] = pstGoods->Items[i];
	}
	
	stOpLog.OpLogType = OP_LOG_GET_MAILGOODS;
	//return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog );

	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_role_exchg_oplog(ZONESVRENV* pstEnv, Player *pstPlayer,Player *pstExchgObj,EXCHGENTIY *pstIn, EXCHGENTIY *pstOut)
{
	int i;
	OP_LOG_META stOpLog;
	OPROLEEXCHG *pstRoleExchg = &stOpLog.OpLog.RoleExchg;
	ROLEGRID *pstRoleGrid;
	ROLEPET *pstRolePet;

	memset (pstRoleExchg, 0, sizeof(*pstRoleExchg));
	stOpLog.OpLogType = OP_LOG_ROLE_EXCHG;

	pstRoleExchg->EventLogWID = z_get_logwid(pstEnv);
	pstRoleExchg->Uin = pstPlayer->stRoleData.Uin;
	pstRoleExchg->ExchgObjUin = pstExchgObj->stRoleData.Uin;
	
	pstRoleExchg->RoleID =  pstPlayer->stRoleData.RoleID;
	pstRoleExchg->ExchgObjRoleID = pstExchgObj->stRoleData.RoleID;
	
	strncpy(pstRoleExchg->RoleName,pstPlayer->stRoleData.RoleName, sizeof(pstRoleExchg->RoleName));
	strncpy(pstRoleExchg->ExchgObjName,pstExchgObj->stRoleData.RoleName, sizeof(pstRoleExchg->RoleName));

	STRNCPY( pstRoleExchg->LoginName, pstPlayer->szAccount, CCH( pstRoleExchg->LoginName) );
	STRNCPY( pstRoleExchg->ExchgObjLoginName, pstExchgObj->szAccount, CCH( pstRoleExchg->ExchgObjLoginName) );
	
	pstRoleExchg->Time = pstEnv->pstAppCtx->stCurr.tv_sec;

	/*in*/
	pstRoleExchg->ExchgIn.ItemNum = pstIn->ItemNum;
	for (i=0; i<pstIn->ItemNum; i++)
	{
		pstRoleGrid = &pstIn->ItemEntiy[i].RoleGrid;
		memcpy(&pstRoleExchg->ExchgIn.RoleGrid[i], pstRoleGrid, sizeof(ROLEGRID));
	}

	pstRoleExchg->ExchgIn.PetNum = pstIn->PetNum;
	for (i=0; i<pstIn->PetNum; i++)
	{
		pstRolePet = &pstIn->PetEntiy[i].RolePet;
		memcpy(&pstRoleExchg->ExchgIn.RolePet[i], pstRolePet, sizeof(ROLEPET));
	}
	
	pstRoleExchg->ExchgIn.Money = pstIn->Money;
	/*in*/


	/*out*/
	pstRoleExchg->ExchgOut.ItemNum = pstOut->ItemNum;
	for (i=0; i<pstOut->ItemNum; i++)
	{
		pstRoleExchg->ExchgOut.RoleGrid[i]= pstOut->ItemEntiy[i].RoleGrid;
	}

	pstRoleExchg->ExchgOut.PetNum = pstOut->PetNum;
	for (i=0; i<pstOut->PetNum; i++)
	{
		pstRoleExchg->ExchgOut.RolePet[i] = pstOut->PetEntiy[i].RolePet;
	}
	
	pstRoleExchg->ExchgOut.Money = pstOut->Money;
	/*out*/

	//return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog );
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}


int z_role_stall_oplog(ZONESVRENV* pstEnv, Player *pstPlayer,STALLSEERES *pstStallSee,int iFlag)
{
	int i;
	OP_LOG_META stOpLog;
	OPROLESTALL *pstRoleStall = &stOpLog.OpLog.RoleStall;
	ROLEGRID *pstRoleGrid;

	memset (pstRoleStall, 0, sizeof(*pstRoleStall));
	stOpLog.OpLogType = OP_LOG_ROLE_STALL;

	pstRoleStall->Uin = pstPlayer->stRoleData.Uin;
	pstRoleStall->StallRoleID =  pstPlayer->stRoleData.RoleID;
	STRNCPY( pstRoleStall->AccountLoginName, pstPlayer->szAccount, CCH( pstRoleStall->AccountLoginName) );
	strncpy(pstRoleStall->StallOwner, pstPlayer->stRoleData.RoleName, sizeof(pstRoleStall->StallOwner));
	pstRoleStall->Time = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstRoleStall->StallFlag = iFlag;

	if (pstStallSee)
	{
		pstRoleStall->ItemNum = pstStallSee->ItemNum;
		for (i=0; i<pstStallSee->ItemNum; i++)
		{
			pstRoleGrid = &pstStallSee->ItemEntity[i].RoleGrid;
			pstRoleStall->StallItem[i].Price = pstStallSee->ItemEntity[i].Price;
			memcpy (&pstRoleStall->StallItem[i].RoleGrid, pstRoleGrid, sizeof(ROLEGRID));
		}

		pstRoleStall->PetNum = pstStallSee->PetNum;
		for (i=0; i<pstStallSee->PetNum; i++)
		{
			pstRoleStall->StallPet[i].Price = pstStallSee->PetEntity[i].Price;
			pstRoleStall->StallPet[i].WID = pstStallSee->PetEntity[i].RolePet.WID;
			pstRoleStall->StallPet[i].PetID = pstStallSee->PetEntity[i].RolePet.PetID;
			pstRoleStall->StallPet[i].PetLv = pstStallSee->PetEntity[i].RolePet.Level;
		}
	}
	
	
	
	//return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog );
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_role_fairy_oplog(ZONESVRENV* pstEnv, Player *pstPlayer,ROLEFAIRY *pstRoleFairy, int iOpType,
							int iPreStar,ITEMDEC *pstItemDec,int iMoneyDec)
{
	OP_LOG_META stOpLog;
	OPFAIRY *pstRoleFairyChg = &stOpLog.OpLog.RoleFairyChg;
	
	memset (pstRoleFairyChg, 0, sizeof(*pstRoleFairyChg));
	stOpLog.OpLogType = OP_LOG_FAIRY;

	pstRoleFairyChg->Uin = pstPlayer->stRoleData.Uin;
	pstRoleFairyChg->RoleID = pstPlayer->stRoleData.RoleID;
	pstRoleFairyChg->FairyID = pstRoleFairy->FairyID;
	pstRoleFairyChg->FairyLV = pstRoleFairy->FairyLevel;
	pstRoleFairyChg->WID = pstRoleFairy->WID;
	pstRoleFairyChg->OpType = iOpType;
	pstRoleFairyChg->PreStar = iPreStar;
	pstRoleFairyChg->CurrStar = pstRoleFairy->StarLvl;
	STRNCPY( pstRoleFairyChg->LoginName, pstPlayer->szAccount, CCH(  pstRoleFairyChg->LoginName) );
	strncpy(pstRoleFairyChg->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstRoleFairyChg->RoleName));
	if (pstItemDec)
		pstRoleFairyChg->ItemDec = *pstItemDec;
	pstRoleFairyChg->MoneyDec = iMoneyDec;
	
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);

}

int z_role_stall_buy_oplog(ZONESVRENV* pstEnv, Player *pstPlayer,Player *pstStallOwner,
							ROLEGRID*pstRoleGrid,ROLEPET *pstRolePet,int iType,unsigned int iPrice)
{
	OP_LOG_META stOpLog;
	OPROLESTALLBUY *pstRoleStallBuy = &stOpLog.OpLog.RoleStallBuy;

	memset(pstRoleStallBuy, 0, sizeof(*pstRoleStallBuy));
	stOpLog.OpLogType = OP_LOG_ROLE_STALL_BUY;

	pstRoleStallBuy->StallUin = pstStallOwner->stRoleData.Uin;
	pstRoleStallBuy->Uin = pstPlayer->stRoleData.Uin;
	
	pstRoleStallBuy->StallRoleID =  pstStallOwner->stRoleData.RoleID;
	pstRoleStallBuy->RoleID =  pstPlayer->stRoleData.RoleID;

	strncpy(pstRoleStallBuy->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstRoleStallBuy->RoleName));
	strncpy(pstRoleStallBuy->StallOwner, pstStallOwner->stRoleData.RoleName, sizeof(pstRoleStallBuy->StallOwner));

	STRNCPY( pstRoleStallBuy->OwnerLoginName, pstStallOwner->szAccount, CCH(  pstRoleStallBuy->OwnerLoginName) );
	STRNCPY( pstRoleStallBuy->BuyLoginName, pstPlayer->szAccount, CCH(  pstRoleStallBuy->BuyLoginName) );

	pstRoleStallBuy->Time = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstRoleStallBuy->Price = iPrice;
	pstRoleStallBuy->Type = iType;
	if (iType == STALL_TYPE_ITEM)
	{
		if (!pstRoleGrid)
		{
			return -1;
		}
		memcpy(&pstRoleStallBuy->BuyData.RoleGrid, pstRoleGrid, sizeof(ROLEGRID));
	}
	else if(iType == STALL_TYPE_PET)
	{
		if (!pstRolePet)
		{
			return -1;
		}
		memcpy(&pstRoleStallBuy->BuyData.RolePet, pstRolePet, sizeof(ROLEPET));
	}
	else
	{
		return -1;
	}
	
	
	//return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog );
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_role_machining_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEARM *pstOldRoelArm, 
								ROLEARM *pstRoleArm, MACHINNGEXPENDDATA *pstExpendData, 
								int iActType, int iSuccFlag, ITEMDEC *pstObtainItem, int iGetNum)
{
	OP_LOG_META stOpLog;
	OPROLEARMMACHINING *pstMachining = &stOpLog.OpLog.RoleArmMachining;
	int i;

	memset(pstMachining, 0, sizeof(*pstMachining));
	stOpLog.OpLogType = OP_LOG_ROLE_ARM_MACHINING;

	pstMachining->Uin = pstPlayer->stRoleData.Uin;
	pstMachining->RoleID =  pstPlayer->stRoleData.RoleID;
	strncpy(pstMachining->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstMachining->RoleName));
	STRNCPY( pstMachining->AccountLoginName, pstPlayer->szAccount, CCH(  pstMachining->AccountLoginName) );

	pstMachining->MachiningActType = iActType;
	pstMachining->Time = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstMachining->SuccFlag = iSuccFlag;
	
	if (pstObtainItem && iGetNum > 0)
	{
		for (i=0; i<iGetNum; i++)
		{
			if (	i>=MAX_OBTAIN )
				break;
			
			pstMachining->ObtainItem[i] = *(pstObtainItem+i);
		}	
	}

	if(pstExpendData)
	{
		memcpy(&pstMachining->ExpendData, pstExpendData, sizeof(pstMachining->ExpendData));
	}

	if (pstOldRoelArm)
	{
		memcpy(&pstMachining->ArmData.OldRoleArm, pstOldRoelArm, sizeof(pstMachining->ArmData.OldRoleArm));
	}

	if (pstRoleArm)
	{
		memcpy(&pstMachining->ArmData.NewRoleArm, pstRoleArm, sizeof(pstMachining->ArmData.NewRoleArm));
	}
	
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

// 怪物死亡记录
int z_monster_die_oplog(ZONESVRENV* pstEnv, MONSTERDEF *pstMonDef, Monster* pstMon, ZoneAni *pstAtker )
{
	OP_LOG_META stOpLog;
	OPMOSTERDIE * pstDieInfo = &stOpLog.OpLog.MonsterDie;

	//bzero( pstDieInfo, sizeof(*pstDieInfo) );
	stOpLog.OpLogType = OP_LOG_ROLE_MONSTER_DIE;

	pstDieInfo->EventLogWID = z_get_logwid(pstEnv);
	pstDieInfo->MosterID = pstMonDef->MonsterID;
	pstDieInfo->KillerType = KILLER_UNKNOW;
	pstDieInfo->PetID = 0;
	
	switch( pstAtker->iType )
	{
	case OBJ_PLAYER:
		if( pstAtker->stObj.pstPlayer )
		{
			pstDieInfo->KillerType = KILLER_PLAYER;
			z_rolemem_warp( pstAtker->stObj.pstPlayer, 
				&pstDieInfo->KillerInfo.PlayerInfo);
			pstDieInfo->PlayerUin = pstAtker->stObj.pstPlayer->stRoleData.Uin;
			STRNCPY( pstDieInfo->PlayerLoginName, pstAtker->stObj.pstPlayer->szAccount, 
						CCH( pstDieInfo->PlayerLoginName) );
		}
		break;

	case OBJ_MONSTER:
		if( pstAtker->stObj.pstMon )
		{
			pstDieInfo->KillerType = KILLER_MONSTER;
			pstDieInfo->KillerInfo.MonsterInfo.ID = pstAtker->stObj.pstMon->iDefIdx;
			pstDieInfo->PlayerUin = 0;
			pstDieInfo->PlayerLoginName[0] = 0;
		}
		break;

	case OBJ_PET:
		if( pstAtker->stObj.stZonePet.pstOwner )
		{
			z_rolemem_warp( pstAtker->stObj.stZonePet.pstOwner, 
				&pstDieInfo->KillerInfo.PetInfo);

			pstDieInfo->PlayerUin = pstAtker->stObj.stZonePet.pstOwner->stRoleData.Uin;
			STRNCPY( pstDieInfo->PlayerLoginName, pstAtker->stObj.stZonePet.pstOwner->szAccount, 
						CCH( pstDieInfo->PlayerLoginName) );
			if (pstAtker->stObj.stZonePet.pstRolePet)
				pstDieInfo->PetID = pstAtker->stObj.stZonePet.pstRolePet->PetID;
		}
		
		pstDieInfo->KillerType = KILLER_PET;
		break;

	default:
		return -1;
		break;
	}

	pstDieInfo->MapID  = GET_MAPID(pstMon->stMap.iID);
	pstDieInfo->Pos = pstMon->stCurrPos;
	pstDieInfo->SubType = pstMon->bSubType;
	pstDieInfo->LifeTime = 0;
	if(pstMon->tBirthEnd < pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		pstDieInfo->LifeTime = pstEnv->pstAppCtx->stCurr.tv_sec - pstMon->tBirthEnd;
	}
	

	return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog);
}


// 记录玩家拾取的物品
int z_role_pickitem_oplog( ZONESVRENV* pstEnv, Player * pstPlayer, 
	const ROLEGRID * pstGrids, int iGridNum, int iHowToPick )
{
	ITEMDEF * pstItemDef = NULL;
	OP_LOG_META stOpLog;
	OPROLEPICKITEM * pstPickItem = &stOpLog.OpLog.RolePickItem;

	if( iGridNum != 1 )
	{
		return -1;
	}

	stOpLog.OpLogType = OP_LOG_ROLE_PICK_ITEM;
	pstPickItem->Uin = pstPlayer->stRoleData.Uin;
	pstPickItem->How = (unsigned char)iHowToPick;
	pstPickItem->RoleCareer = pstPlayer->stRoleData.Career;
	pstPickItem->X = pstPlayer->stRoleData.Pos.X;
	pstPickItem->Y = pstPlayer->stRoleData.Pos.Y;
	pstPickItem->Map = GET_MAPID(pstPlayer->stRoleData.Map);
	z_rolemem_warp( pstPlayer, &pstPickItem->RoleInfo );
	STRNCPY( pstPickItem->LoginName, pstPlayer->szAccount, CCH(  pstPickItem->LoginName) );
	
	pstItemDef = z_find_itemdef( pstEnv, pstGrids[0].GridData.RoleItem.DefID );
	if( !pstItemDef )
	{
		return -1;
	}

	if (ITEM_TYPE_ARM == pstItemDef->ItemType)
	{
		//蓝装以下不记录了
		if (pstItemDef->Important < 2 && (0 == (ITEM_IMP_LOGTRACE & pstItemDef->CtrlFlag))) 
		{
			return 0;
		}
	}
	else if (pstItemDef->Important == 0 && (0 == (ITEM_IMP_LOGTRACE & pstItemDef->CtrlFlag)))
	{
		return 0;
	}
	
	pstPickItem->CareerLimit = pstItemDef->WearPre.Career;
	pstPickItem->Items = pstGrids[0];

	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}


// 记录玩家的任务记录
int z_role_task_oplog( ZONESVRENV* pstEnv, Player * pstPlayer, 
	unsigned short iTaskID, int iState , ROLEGRID *pstRoleGrid, int iRoleGrid,
							int iMoney, int iMoneyType, int iExp, int iDecMoney)
{
	OP_LOG_META stOpLog;
	OPROLETASK * pstRoleTask = &stOpLog.OpLog.RoleTask;

	memset(pstRoleTask,0,sizeof(OPROLETASK));
	//bzero( pstRoleTask, sizeof(*pstRoleTask) );
	
	stOpLog.OpLogType = OP_LOG_ROLE_TASK;

	pstRoleTask->EventLogWID = z_get_logwid(pstEnv);
	pstRoleTask->Uin = pstPlayer->stRoleData.Uin;
	z_rolemem_warp( pstPlayer, &pstRoleTask->RoleInfo );
	STRNCPY( pstRoleTask->LoginName, pstPlayer->szAccount, CCH(  pstRoleTask->LoginName) );

	pstRoleTask->State = (unsigned char)iState;
	pstRoleTask->TaskID = iTaskID;
	pstRoleTask->DecMoney = iDecMoney;
	pstRoleTask->AwardMoney = iMoney;
	pstRoleTask->AwardMoneyType = iMoneyType;
	pstRoleTask->AwardExp = iExp;
	
	//return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
	UNUSED(pstRoleGrid);	//add by paraunused.pl
	UNUSED(iRoleGrid);	//add by paraunused.pl
}

static void z_role_info_fill(Player * pstPlayer, OPROLEINFO *pstOpRoleInfo)
{
	pstOpRoleInfo->Uin = pstPlayer->stRoleData.Uin;
	pstOpRoleInfo->RoleID = pstPlayer->stRoleData.RoleID;
	STRNCPY(pstOpRoleInfo->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstOpRoleInfo->RoleName));
	STRNCPY(pstOpRoleInfo->LoginName, pstPlayer->szAccount, sizeof(pstOpRoleInfo->LoginName));
}

// 玩家任务道具奖励记录
int z_role_task_item_award(ZONESVRENV* pstEnv, Player * pstPlayer, 
								unsigned short iTaskID, ROLEGRID *pstRoleGrid, int iRoleGrid)
{
	//int i = 0;
	OP_LOG_META stOpLog;
	OPTASKITEMAWARD *pstOpTaskItemAward = &stOpLog.OpLog.OpTaskItemAward;
	memset(pstOpTaskItemAward,0,sizeof(OPTASKITEMAWARD));

	stOpLog.OpLogType = OP_LOG_TASK_AWARD_ITEM;
	
	pstOpTaskItemAward->EventLogWID = z_get_logwid(pstEnv);
	z_role_info_fill(pstPlayer, &pstOpTaskItemAward->RoleInfo);
	pstOpTaskItemAward->TaskID = iTaskID;
	
	if (iRoleGrid <= OP_MAX_TASK_AWARD_ITEM && iRoleGrid > 0)
	{
		memcpy(pstOpTaskItemAward->AwardItem,pstRoleGrid,sizeof(pstOpTaskItemAward->AwardItem[0])*iRoleGrid);
		pstOpTaskItemAward->AwardNum = iRoleGrid;
		//return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog);
		return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
	}

	return -1;
}

// 记录角色的聊天信息
int z_role_chat_oplog( ZONESVRENV* pstEnv, Player * pstPlayer,
	CSCHATREQ * pstChatReq )
{
	OP_LOG_META stOpLog;
	OPROLECHAT * pstRoleChat = &stOpLog.OpLog.RoleChat;

	//bzero( pstRoleChat, sizeof(*pstRoleChat) );
	stOpLog.OpLogType = OP_LOG_ROLE_CHAT;
	pstRoleChat->Uin = pstPlayer->stRoleData.Uin;
	z_rolemem_warp( pstPlayer, &pstRoleChat->RoleInfo );
	STRNCPY( pstRoleChat->LoginName, pstPlayer->szAccount, CCH(  pstRoleChat->LoginName) );

	pstRoleChat->ChatType = pstChatReq->Type;
	STRNCPY( pstRoleChat->ToName, pstChatReq->ToName, CCH(pstRoleChat->ToName) );
	STRNCPY( pstRoleChat->Content, pstChatReq->Content, CCH(pstRoleChat->Content) );

	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

// 记录角色的公会操作
int z_role_clan_oplog(ZONESVRENV *pstEnv, char * pszRoleName,
                      const CLANID *pstClanIdent, int iOperation,OPCLANDATA* pstData)
{
	OP_LOG_META stOpLog;
	OPROLECLAN *pstRoleClan = &stOpLog.OpLog.RoleClan;

	bzero( pstRoleClan, sizeof(*pstRoleClan) );
	stOpLog.OpLogType = OP_LOG_ROLE_CLAN;
	
	if (pszRoleName)
	{
		STRNCPY(pstRoleClan->RoleName, pszRoleName,
	        	CCH(pstRoleClan->RoleName));
	}

	pstRoleClan->Operation = (unsigned char)iOperation;
	pstRoleClan->ClanIdent = *pstClanIdent;
	if (pstData)
	{
		switch (iOperation)
		{
		case OP_CLAN_CREATE:
		case OP_CLAN_DESTROY:
		case OP_CLAN_JOIN:
		case OP_CLAN_EXIT:
		case OP_CLAN_DESTROY_MEMBER:
		case OP_CLAN_DESTROY_ACTIVE:
		case OP_CLAN_DESTROY_GM:
			break;
			
		case OP_CLAN_LEVEL_CH:
			pstRoleClan->ClanLevelCh = pstData->ClanLevelCh;
			break;
		
		case OP_CLAN_CITY_LEVEL_CH:
			pstRoleClan->ClanCityLevelCh = pstData->ClanCityLevelCh;
			break;
		
		case OP_CLAN_BUILDING_LEVEL_CH:
			pstRoleClan->ClanBuildingCh = pstData->ClanBuildingCh;
			break;
		
		case OP_CLAN_MAKE:
			pstRoleClan->ClanMake = pstData->ClanMake;
			break;
		
		case OP_CLAN_TECH_CH:
			pstRoleClan->ClanTechCh = pstData->ClanTechCh;
			break;

		case OP_CLAN_RESOURCE_PLAYER_COMMIT:
			pstRoleClan->ClanResource = pstData->ClanResourceCommit;
			break;

		case OP_CLAN_RESOURCE_TASK:
			pstRoleClan->ClanResource = pstData->ClanResourceTask;
			break;
			
		case OP_CLAN_RESOURCE_GATHER:
			pstRoleClan->ClanResource = pstData->ClanResourceGather;
			break;	

		case OP_CLAN_GRAB:
			pstRoleClan->ClanGrab = pstData->ClanGrab;
			break;
		
		case OP_CLAN_SKILL_RESET_RETURN_CONTRI:
			pstRoleClan->ClanSkillResetReturnContri = pstData->ClanSkillResetReturnContri;
			break;
			
		case OP_CLAN_SKILL_RESET_RETURN_MONEY:
			pstRoleClan->ClanSkillResetReturnMoney = pstData->ClanSkillResetReturnMoney;
			break;
			
		default:
			break;
		}
	}

	return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog);
}

int z_role_pet_oplog( ZONESVRENV* pstEnv, Player * pstPlayer, 
	const ROLEPET * pstPet, const ROLEPET * pstForwardPet, int iEvent, ITEMDEC *pstGetItem, 
	ITEMDEC *pstItemDec, USEMONEY *pstUse, PETSTRENINFO *pstStrenInfo)
{
	OP_LOG_META stOpLog;
	OPROLEPET * pstOpRolePet = &stOpLog.OpLog.RolePet;

	bzero(pstOpRolePet, sizeof(*pstOpRolePet) );
	stOpLog.OpLogType = OP_LOG_ROLE_PET;

	pstOpRolePet->EventLogWID = z_get_logwid(pstEnv);
	pstOpRolePet->Uin = pstPlayer->stRoleData.Uin;
	z_rolemem_warp( pstPlayer, &pstOpRolePet->RoleInfo );
	STRNCPY( pstOpRolePet->LoginName, pstPlayer->szAccount, CCH(  pstOpRolePet->LoginName) );

	pstOpRolePet->Event = (unsigned char)iEvent;
	if (pstUse)
	{
		//120919，策划把金币和金券名称互换但没动表格
		pstOpRolePet->Money = pstUse->UseMoney_Money;
		pstOpRolePet->JinBi = pstUse->UseMoney_JinQuan;
		pstOpRolePet->JinQaun = pstUse->UseMoney_JinBi;
	}
	
	if(pstGetItem)
	{
		pstOpRolePet->GetItem = *pstGetItem;
	}
	if(pstItemDec)
	{
		pstOpRolePet->ItemDec = *pstItemDec;
	}
	
	switch( pstOpRolePet->Event )
	{
		case OP_PET_BORN:
	  	case OP_PET_RELEASE:
		case OP_PET_STRENGTHEN:
		case OP_PET_ADD_ATTR:
		case OP_PET_FUSE:
		case OP_PET_WASH:
		case OP_PET_FUSE_DEL:
		case OP_PET_STRENGTHEN_CONSUME:
		case OP_PET_STRENGTHEN_REDO:
		case OP_PET_LOCK_SKILL:
		case OP_PET_WASH_SAVE:
		case OP_PET_LOVE_LEVEL_UP:
		case OP_PET_MOVE_STAR:
		case OP_PET_EAT:
		case OP_PET_EAT_DEL:
		case OP_PET_STEP_UP:
		case OP_PET_UNBIND:
			if (pstPet)
			{
				pstOpRolePet->PetInfo.FreePet = *pstPet;
			}
			break;

		case OP_PET_LVUP:
		case OP_PET_RESET_SKILL:
			if( pstForwardPet )
			{
				pstOpRolePet->PetInfo.LvUpPet.ForwardPet = *pstForwardPet;
			}

			if (pstPet)
			{
				pstOpRolePet->PetInfo.LvUpPet.CurrentPet = *pstPet;
			}
			
			break;
		default:
			return -1;
			break;
	}

	if (pstStrenInfo)
	{	
		pstOpRolePet->PetStrenInfo = *pstStrenInfo;
	}

	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_role_del_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, OPROLEDEL *pstOpRoleDel)
{
	OP_LOG_META stOpLog;
	OPROLEDEL *pstRoleDel = &stOpLog.OpLog.RoleDel;

	//bzero( pstRoleDel, sizeof(*pstRoleDel) );
	stOpLog.OpLogType = OP_LOG_ROLE_DEL;

	pstRoleDel->Uin = pstPlayer->stRoleData.Uin;
	pstRoleDel->RoleID = pstOpRoleDel->RoleID;
	pstRoleDel->Career = pstOpRoleDel->Career;
	pstRoleDel->Level = pstOpRoleDel->Level;
	STRNCPY( pstRoleDel->RoleName, pstOpRoleDel->RoleName, CCH(pstRoleDel->RoleName) );
	STRNCPY(pstRoleDel->AccountLoginName, pstPlayer->szAccount, CCH(pstRoleDel->AccountLoginName));
	
	return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog);
}

int z_pworld_time_oplog(ZONESVRENV* pstEnv, int iPworldID,int iTime)
{
	OP_LOG_META stOpLog;
	OPPWORLDTIME *pstPworldTm = &stOpLog.OpLog.OpPworldTime;

	stOpLog.OpLogType = OP_LOG_PWORLD_TIME;

	pstPworldTm->PworldID = iPworldID;
	pstPworldTm->Time = iTime;
	return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog);
}


int z_strong_oplog(ZONESVRENV* pstEnv, unsigned int uiClanID, int iStrongMapID)
{
	OP_LOG_META stOpLog;
	OPSTRONGINFO *pstOpLog = &stOpLog.OpLog.OpStrongInfo;

	stOpLog.OpLogType = OP_LOG_STRONG;

	pstOpLog->ClanID = uiClanID;
	pstOpLog->StrongMapID = iStrongMapID;
	return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog);
}

int z_city_fini_oplog(ZONESVRENV* pstEnv, WORLDCITYCLANFIGHTINFO *pstList, int iIdx)
{
	OP_LOG_META stOpLog;
	OPCITYFINI *pstOpLog = &stOpLog.OpLog.OpCityFini;	

	stOpLog.OpLogType = OP_LOG_CITY_FINI;
	
	pstOpLog->Idx = iIdx+1;
	pstOpLog->ClanId = pstList->ClanId;
	pstOpLog->BuffBuild = pstList->BuffBuild;
	pstOpLog->ControlBuild = pstList->ControlBuild;
	pstOpLog->DefBuild = pstList->DefBuild;
	pstOpLog->DoorBuild = pstList->DoorBuild;
	pstOpLog->ReviveBuild = pstList->ReviveBuild;
	pstOpLog->Total = pstList->Total;

	return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog);
}

int z_strong_fini_oplog(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	OP_LOG_META stOpLog;
	OPSTRONGFINI *pstOpLog = &stOpLog.OpLog.OpStrongFini;
	ZoneClan *pstClan;
	WarFieldOL *pstWarOL = &pstPlayer->stOnline.stWarOL;
	pstClan = player_get_clan(pstEnv, pstPlayer);

	if (!pstClan)
	{
		return 0;
	}
	
	stOpLog.OpLogType = OP_LOG_STRONG_FINI;

	pstOpLog->Assit = pstPlayer->stRoleData.MiscInfo.MiscWar.Assit;
	pstOpLog->Career = pstPlayer->stRoleData.Career;
	pstOpLog->CountKill = pstPlayer->stRoleData.MiscInfo.MiscWar.Kill;
	pstOpLog->CurrContinueKill = pstWarOL->iContinueKill;
	pstOpLog->Die = pstPlayer->stRoleData.MiscInfo.MiscWar.Die;
	pstOpLog->EnterTime = pstPlayer->stRoleData.MiscInfo.MiscWar.FightTime;
//	pstOpLog->MapID = GET_MAPID(pstPlayer->stRoleData.Map);
	pstOpLog->MaxContinueKill = pstWarOL->iKillNum;
	pstOpLog->ResGet = pstWarOL->iOre;
	pstOpLog->RoleInfo.RoleID = pstPlayer->stRoleData.RoleID;
	strncpy(pstOpLog->RoleInfo.RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstOpLog->RoleInfo.RoleName));

	
	pstOpLog->ClanID = pstClan->stClan.ClanProf.Id;
	pstOpLog->ClanWid = pstClan->stClan.ClanProf.GID;
	strncpy(pstOpLog->ClanName, pstClan->stClan.ClanProf.Name, sizeof(pstOpLog->ClanName));
	

	ZONEPWORLD *pstZonePworld;
	STRONGPOINTDEF *pstDef = NULL;
	STRONGPOINTONE *pstOne;
	
	pstZonePworld = z_find_pworld2(pstEnv, pstPlayer->stRoleData.Map);
	if (pstZonePworld && pstZonePworld->stPworldDef.StrongMapID > 0)
	{
		pstDef = strong_def_find( pstEnv,  pstZonePworld->stPworldDef.StrongMapID);
	}

	if (!pstDef)
		return 0;

	pstOpLog->StrongID = pstDef->MapID;
	strncpy(pstOpLog->StrongName, pstDef->Name, sizeof(pstOpLog->StrongName));

	pstOne= strong_point_one_get(pstEnv, pstDef->MapID);
	if (!pstOne)
		return 0;

	pstOpLog->Win = (pstClan->stClan.ClanProf.Id == pstOne->OwerID)?1:0;
	
	return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog);
}

int z_role_die_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneAni *pstAtker )
{
	OP_LOG_META stOpLog;
	OPPLAYERDIE * pstDieInfo = &stOpLog.OpLog.RoleDie;

	stOpLog.OpLogType = OP_LOG_ROLE_DIE;
	memset(pstDieInfo, 0, sizeof(*pstDieInfo));

	pstDieInfo->Career = pstPlayer->stRoleData.Career;
	pstDieInfo->EventLogWID = z_get_logwid(pstEnv);
	pstDieInfo->PlayerUin = pstPlayer->stRoleData.Uin;
	z_rolemem_warp(pstPlayer, &pstDieInfo->PlayerInfo);
	STRNCPY( pstDieInfo->PlayerLoginName, pstPlayer->szAccount, CCH(pstDieInfo->PlayerLoginName) );
	pstDieInfo->KillerType = KILLER_UNKNOW;

	pstDieInfo->PetID = 0;
	pstDieInfo->KillerLv = 0;
	switch( pstAtker->iType )
	{
	case OBJ_PLAYER:
		if( pstAtker->stObj.pstPlayer )
		{
			pstDieInfo->KillerType = KILLER_PLAYER;
			z_rolemem_warp( pstAtker->stObj.pstPlayer, 
				&pstDieInfo->KillerInfo.PlayerInfo);
			pstDieInfo->KillerUin = pstAtker->stObj.pstPlayer->stRoleData.Uin;
			STRNCPY(pstDieInfo->KillerLoginName, pstAtker->stObj.pstPlayer->szAccount,
						CCH(pstDieInfo->KillerLoginName));
			pstDieInfo->KillerLv = pstAtker->stObj.pstPlayer->stRoleData.Level;
			pstDieInfo->KillerCareer = pstAtker->stObj.pstPlayer->stRoleData.Career;
		}
		break;

	case OBJ_MONSTER:
		if( pstAtker->stObj.pstMon )
		{
			pstDieInfo->KillerType = KILLER_MONSTER;
			pstDieInfo->KillerInfo.MonsterInfo.ID = pstAtker->stObj.pstMon->iDefIdx;
			pstDieInfo->KillerUin = 0;
			pstDieInfo->KillerLoginName[0] = 0;
		}
		break;

	case OBJ_PET:
		if( pstAtker->stObj.stZonePet.pstOwner )
		{
			z_rolemem_warp( pstAtker->stObj.stZonePet.pstOwner, 
				&pstDieInfo->KillerInfo.PetInfo);
			pstDieInfo->KillerUin = pstAtker->stObj.stZonePet.pstOwner->stRoleData.Uin;
			pstDieInfo->KillerCareer = pstAtker->stObj.stZonePet.pstOwner->stRoleData.Career;
			STRNCPY(pstDieInfo->KillerLoginName, pstAtker->stObj.stZonePet.pstOwner->szAccount,
						CCH(pstDieInfo->KillerLoginName));
			pstDieInfo->KillerLv = pstAtker->stObj.stZonePet.pstOwner->stRoleData.Level;
			if (pstAtker->stObj.stZonePet.pstRolePet)
				pstDieInfo->PetID = pstAtker->stObj.stZonePet.pstRolePet->PetID;
		}
		
		pstDieInfo->KillerType = KILLER_PET;
		break;
	}

	pstDieInfo->MapID  = GET_MAPID(pstPlayer->stRoleData.Map);
	pstDieInfo->Pos = pstPlayer->stRoleData.Pos;
	pstDieInfo->PlayerLevel = pstPlayer->stRoleData.Level;

	pstDieInfo->Money = pstPlayer->stRoleData.MiscInfo.DieInfo.DieDrop.Money;
	pstDieInfo->Exp = pstPlayer->stRoleData.MiscInfo.DieInfo.DieDrop.Exp;

	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_pet_attr_chg_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, 
							unsigned char ucActType,  unsigned short unObjID, ITEMDEC *pstDec,
							USEMONEY *pstUseMoney)
{
	OP_LOG_META stOpLog;
	OPPETATTRCHG *pstPetChg = &stOpLog.OpLog.PetAttrChg;

	stOpLog.OpLogType = OP_LOG_PET_ATTR_CHG;

	pstPetChg->OwnerUin = pstPlayer->stRoleData.Uin;
	z_rolemem_warp(pstPlayer, &pstPetChg->OwnerInfo);
	STRNCPY( pstPetChg->OwnerLoginName, pstPlayer->szAccount, CCH(pstPetChg->OwnerLoginName) );
	pstPetChg->PetActType = ucActType;
	pstPetChg->ActObjID = unObjID;

	if (pstDec)
	{
		pstPetChg->UseItem = *pstDec;
	}
	
	if (pstUseMoney)
	{
		pstPetChg->UseMoney = *pstUseMoney;
	}
	
	memcpy(&pstPetChg->Pet, pstRolePet, sizeof(ROLEPET));
	
	//return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_role_chg_map_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int iPreMapID, RESPOS *pstPrePos)
{
	OP_LOG_META stOpLog;
	OPROLEMAPCHG *pstMapChg = &stOpLog.OpLog.RoleMap;

	stOpLog.OpLogType = OP_LOG_ROLE_MAP_CHG;

	z_rolemem_warp(pstPlayer, &pstMapChg->RoleInfo);
	STRNCPY( pstMapChg->LoginName, pstPlayer->szAccount, CCH(pstMapChg->LoginName) );
	pstMapChg->Uin = pstPlayer->stRoleData.Uin;
	pstMapChg->PreMapID = GET_MAPID(iPreMapID);
	pstMapChg->PrePos.X = pstPrePos->X;
	pstMapChg->PrePos.Y = pstPrePos->Y;
	pstMapChg->MapID =  GET_MAPID(pstPlayer->stRoleData.Map);
	pstMapChg->Pos = pstPlayer->stRoleData.Pos;
	pstMapChg->Lvl = pstPlayer->stRoleData.Level;
	
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_role_enter_pworld_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID)
{
	OP_LOG_META stOpLog;
	OPROLEENTERPWORLD *pstEnterPworld = &stOpLog.OpLog.RoleEnterPworld;

	stOpLog.OpLogType = OP_LOG_ROLE_ENTER_PWORLD;

	z_rolemem_warp(pstPlayer, &pstEnterPworld->RoleInfo);
	STRNCPY( pstEnterPworld->LoginName, pstPlayer->szAccount, CCH(pstEnterPworld->LoginName) );
	pstEnterPworld->Uin = pstPlayer->stRoleData.Uin;
	pstEnterPworld->RoleLevel = pstPlayer->stRoleData.Level;
	pstEnterPworld->PworldID = iPworldID;
	
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_role_diathesis_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int iLearnType, int iDiathesisID, int iLevel, int iPreLevel)
{
	OP_LOG_META stOpLog;
	OPROLEDIATHESISCHG *pstDiathesisChg = &stOpLog.OpLog.RoleDiathesis;

	stOpLog.OpLogType = OP_LOG_ROLE_DIATHESIS;

	z_rolemem_warp(pstPlayer, &pstDiathesisChg->RoleInfo);
	STRNCPY( pstDiathesisChg->LoginName, pstPlayer->szAccount, CCH(pstDiathesisChg->LoginName) );
	pstDiathesisChg->Uin = pstPlayer->stRoleData.Uin;
	pstDiathesisChg->LearnType = iLearnType;
	pstDiathesisChg->DiathesisID = iDiathesisID;
	pstDiathesisChg->Level = iLevel;
	pstDiathesisChg->PreLevel = iPreLevel;

	//return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_role_skill_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int iLearnType, 
							int iSkillID, int iLevel, int iPreLevel, int iSkillExp, tdr_longlong iPreSkillExp, SKILLLEARN *pstLearn,
							OPROLESKILLRUNECHG* pstRuneBefore,int iUseItemID,OPROLESKILLRUNECOST *pstCost)
{
	OP_LOG_META stOpLog;
	OPROLESKILLCHG *pstRoleSkill = &stOpLog.OpLog.RoleSkill;
	ZoneSkill* pstZoneSkill = NULL;

	bzero( pstRoleSkill, sizeof(*pstRoleSkill) );
	stOpLog.OpLogType = OP_LOG_ROLE_SKILL;

	z_rolemem_warp(pstPlayer, &pstRoleSkill->RoleInfo);
	STRNCPY( pstRoleSkill->LoginName, pstPlayer->szAccount, CCH(pstRoleSkill->LoginName) );
	pstRoleSkill->Uin = pstPlayer->stRoleData.Uin;
	pstRoleSkill->RoleExp = iSkillExp;
	pstRoleSkill->RolePreExp =  iPreSkillExp;
	pstRoleSkill->LearnType = iLearnType;
	pstRoleSkill->SkillID = iSkillID;
	pstRoleSkill->Level = iLevel;
	pstRoleSkill->PreLevel = iPreLevel;
	pstRoleSkill->UseItemID = iUseItemID;

	if (pstLearn)
	{
		pstRoleSkill->ItemID = pstLearn->ItemID;
		pstRoleSkill->UseSkillExp = pstLearn->Exp;
		pstRoleSkill->MoneyAttrID = pstLearn->MoneyAttrID;
		pstRoleSkill->MoneyVal = pstLearn->MoneyVal;
	}

	pstZoneSkill = z_get_player_zoneskill(pstPlayer, iSkillID);
	if (pstZoneSkill)
	{
		rune_op_info(pstZoneSkill,&pstRoleSkill->RuneInfoNow,RUNE_EMPTY,pstPlayer);
		
		if (pstRuneBefore)
		{
			pstRoleSkill->RuneInfoBefore = *pstRuneBefore;
		}
		else
		{
			pstRoleSkill->RuneInfoBefore = pstRoleSkill->RuneInfoNow;
		}
	}
	else
	{
		if (pstRuneBefore)
		{
			rune_op_info(NULL,&pstRoleSkill->RuneInfoNow,pstRuneBefore->RuneInfo[0].RuneType,pstPlayer);
			pstRoleSkill->RuneInfoBefore = *pstRuneBefore;
		}
	}

	if (pstCost)
	{
		pstRoleSkill->RuneCost = *pstCost;
	}
	

	//return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

// 记录GM指令
int z_gm_cmd_oplog(ZONESVRENV *pstEnv, Player *pstPlayer, const char * pszCmdLine)
{
	OP_LOG_META stOpLog;
	OPGMCMD *pstGMCmd = &stOpLog.OpLog.GMCmd;

	stOpLog.OpLogType = OP_LOG_GM_CMD;

	pstGMCmd->Uin = pstPlayer->stRoleData.Uin;
	STRNCPY(pstGMCmd->Account, pstPlayer->szAccount, CCH(pstGMCmd->Account));
	z_rolemem_warp(pstPlayer, &pstGMCmd->RoleInfo);
	STRNCPY(pstGMCmd->CmdLine, pszCmdLine, CCH(pstGMCmd->CmdLine));
	
	//return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_role_qiyue_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int iQiYueID, int Operation)
{	
	OP_LOG_META stOpLog;
	OPROLEQIYUECHG *pstOpRoleQiyueChg = &stOpLog.OpLog.RoleQiYueChg;
	UNUSED(pstEnv);

	stOpLog.OpLogType = OP_LOG_ROLE_QIYUE;

	z_rolemem_warp(pstPlayer, &pstOpRoleQiyueChg->RoleInfo);
	STRNCPY( pstOpRoleQiyueChg->LoginName, pstPlayer->szAccount, CCH(pstOpRoleQiyueChg->LoginName) );
	pstOpRoleQiyueChg->Uin = pstPlayer->stRoleData.Uin;
	pstOpRoleQiyueChg->Operation = Operation;
	pstOpRoleQiyueChg->QiYueID = iQiYueID;
	pstOpRoleQiyueChg->QiYueInfo = pstPlayer->stRoleData.Package.QiYueInfo;
	pstOpRoleQiyueChg->LogTime = 0; // 日志入库的时候会填

	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_role_designation_oplog(ZONESVRENV* pstEnv, Player *pstPlayer,
                             int iDesignationID, int iEvent)
{	
	OP_LOG_META stOpLog;
	OPROLEDESIGNATION *pstLog = &stOpLog.OpLog.RoleDesignationChg;
	UNUSED(pstEnv);	

	stOpLog.OpLogType = OP_LOG_DESIGNATION;

	z_rolemem_warp(pstPlayer, &pstLog->RoleInfo);
	STRNCPY( pstLog->LoginName, pstPlayer->szAccount, CCH(pstLog->LoginName) );
	pstLog->Uin = pstPlayer->stRoleData.Uin;
	pstLog->Event = (char)iEvent;
	pstLog->DesignationID = iDesignationID;

	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_role_respact_oplog(ZONESVRENV* pstEnv, Player *pstPlayer,
                         const RESPACTENTRY *pstRespact, int iEvent)
{	
	OP_LOG_META stOpLog;
	OPROLERESPACT *pstLog = &stOpLog.OpLog.RoleRespactChg;
	UNUSED(pstEnv);
	
	stOpLog.OpLogType = OP_LOG_RESPACT;

	z_rolemem_warp(pstPlayer, &pstLog->RoleInfo);
	STRNCPY( pstLog->LoginName, pstPlayer->szAccount, CCH(pstLog->LoginName) );
	pstLog->Uin = pstPlayer->stRoleData.Uin;
	pstLog->Event = (char)iEvent;
	pstLog->RespactInfo = *pstRespact;

	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_role_ride_oplog(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iRideID, int iLevel, int iOpType)
{
	OP_LOG_META stOpLog;
	OPROLERIDE *pstRoleRide = &stOpLog.OpLog.RoleRideChg;

	stOpLog.OpLogType = OP_LOG_ROLE_RIDE;
	pstRoleRide->Uin = pstPlayer->stRoleData.Uin;
	pstRoleRide->RoleInfo.RoleID = pstPlayer->stRoleData.RoleID;
	strncpy(pstRoleRide->RoleInfo.RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstRoleRide->RoleInfo.RoleName));
	STRNCPY( pstRoleRide->LoginName, pstPlayer->szAccount, CCH(pstRoleRide->LoginName) );
	pstRoleRide->RideID = iRideID; 
	pstRoleRide->RideLevel = iLevel; 
	pstRoleRide->OpType = iOpType;
	
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int  z_role_booty_drop_from_info(ZONESVRENV* pstEnv, OPBOOTYDROP *pstBootyDrop, int iFromID)
{
	int iIndex;
	char cType;
	TMEMBLOCK *pstMemBlock;
	Monster *pstMon;
	MONSTERDEF *pstMonsterDef;
	Player *pstFromPlayer;

	if(iFromID <= 0)
		return 0;

	iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, iFromID, &cType);
	if (iIndex < 0) 
	{
		return -1;
	}

	switch (cType) 
	{
		case OBJ_MONSTER:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				break;
			}

			pstMon = (Monster *)pstMemBlock->szData;

			pstBootyDrop->FromType = OP_DROP_FROM_MONSTER;
			pstBootyDrop->FromID = pstMon->iDefIdx;
			
			pstMonsterDef = z_get_mon_def(pstEnv,  pstMon);
			if(NULL == pstMonsterDef)
			{
				break;
			}
			
			STRNCPY(pstBootyDrop->FromName, pstMonsterDef->Name, CCH(pstBootyDrop->FromName));
			break;
		case OBJ_PLAYER:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				break;
			}
			
			pstFromPlayer = (Player *)pstMemBlock->szData;

			pstBootyDrop->FromType = OP_DROP_FROM_PLAYER;
			pstBootyDrop->FromID = pstFromPlayer->stRoleData.RoleID;
			STRNCPY(pstBootyDrop->FromName, pstFromPlayer->stRoleData.RoleName, CCH(pstBootyDrop->FromName));
			break;
	}

	return 0;
}

int z_role_booty_drop_oplog(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,
	int iMap,RESPOS *pstPos,ROLEGRID *pstGrid,int iDropType, int iFromID)
{
	OP_LOG_META stOpLog;
	OPBOOTYDROP *pstBootyDrop = &stOpLog.OpLog.RoleBootyDrop;
	ITEMDEF *pstItemDef = z_get_itemdef_roleitem(pstEnv, &pstGrid->GridData.RoleItem);
	
	if(NULL == pstItemDef)
	{
		return -1;
	}

	if (ITEM_TYPE_ARM == pstItemDef->ItemType)
	{
		//蓝装以下不记录了
		if (pstItemDef->Important < 2 && (0 == (ITEM_IMP_LOGTRACE & pstItemDef->CtrlFlag))) 
		{
			return 0;
		}
	}
	else if (pstItemDef->Important == 0 && (0 == (ITEM_IMP_LOGTRACE & pstItemDef->CtrlFlag)))
	{
		return 0;
	}

	stOpLog.OpLogType = OP_LOG_BOOTY_DROP;

	memset(pstBootyDrop, 0, sizeof(*pstBootyDrop));

	pstBootyDrop->EventLogWID = z_get_logwid(pstEnv);
	pstBootyDrop->DropType = iDropType;
	pstBootyDrop->MapID = GET_MAPID(iMap);
	pstBootyDrop->Pos = *pstPos;
	pstBootyDrop->RoleGrid = *pstGrid;
	pstBootyDrop->ItemImportant = pstItemDef->Important;

	z_role_booty_drop_from_info(pstEnv, pstBootyDrop, iFromID);
	
	return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_role_die_drop_oplog(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,Player *pstPlayer, OPROLEDIEDROP *pstOpDieDrop)
{
	OP_LOG_META stOpLog;
	OPROLEDIEDROP *pstDrop = &stOpLog.OpLog.OpRoleDieDrop;

	stOpLog.OpLogType = OP_LOG_ROLE_DIE_DROP;
	memset(pstDrop, 0, sizeof(*pstDrop));

	*pstDrop = *pstOpDieDrop;
	pstDrop->EventLogWID = z_get_logwid(pstEnv);

	//return z_oplog_send(pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_role_snapshot_oplog(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	OP_LOG_META stOpLog;
	OPROLESNAPSHOT *pstLog = &stOpLog.OpLog.OpRoleSnapshot;

	stOpLog.OpLogType = OP_LOG_ROLE_SNAPSHOT;
	pstLog->Uin = pstPlayer->stRoleData.Uin;
	pstLog->Exp = pstPlayer->stRoleData.RoleDetail.Exp;
	pstLog->Level = pstPlayer->stRoleData.Level;
	pstLog->SkillNum = pstPlayer->stRoleData.SkillInfo.SkillNum;
	pstLog->DiathsisNum = pstPlayer->stRoleData.DiathesisInfo.DiathesisNum;
	pstLog->StashItemNum = pstPlayer->stRoleData.Package.StashPackage.Num;
	pstLog->StuffItemNum = pstPlayer->stRoleData.Package.StuffPackage.Num;
	pstLog->SundriesItemNum = pstPlayer->stRoleData.Package.SundriesPackage.Num;
	pstLog->TaskItemNum = pstPlayer->stRoleData.Package.TaskPackage.Num;
	pstLog->Bull = pstPlayer->stRoleData.RoleDetail.Bull;
	pstLog->Money = pstPlayer->stRoleData.RoleDetail.Money;
	pstLog->StashBull = pstPlayer->stRoleData.Package.StashPackage.Bull;
	pstLog->StashMoney = pstPlayer->stRoleData.Package.StashPackage.Money;
	pstLog->Gold = pstPlayer->stRoleData.RoleDetail.GodCoin;
	pstLog->BindGold = pstPlayer->stRoleData.RoleDetail.BindGodCoin;
	pstLog->WearItemNum = pstPlayer->stRoleData.Wear.ArmNum;
	pstLog->Map = GET_MAPID(pstPlayer->stRoleData.Map);
	pstLog->Pos_x =  pstPlayer->stRoleData.Pos.X;
	pstLog->Pos_y =  pstPlayer->stRoleData.Pos.Y;
	STRNCPY( pstLog->LoginName, pstPlayer->szAccount, CCH( pstLog->LoginName) );
	
	//memcpy(&pstLog->LogRoleData.AvatarProf, &pstPlayer->stRoleData.AvatarProf, sizeof(pstLog->LogRoleData.AvatarProf));
	//memcpy(&pstLog->LogRoleData.DiathesisInfo, &pstPlayer->stRoleData.DiathesisInfo, sizeof(pstLog->LogRoleData.DiathesisInfo));
	//memcpy(&pstLog->LogRoleData.MiscInfo, &pstPlayer->stRoleData.MiscInfo, sizeof(pstLog->LogRoleData.MiscInfo));
	//memcpy(&pstLog->LogRoleData.Package, &pstPlayer->stRoleData.Package, sizeof(pstLog->LogRoleData.Package));
	//memcpy(&pstLog->LogRoleData.RoleDetail, &pstPlayer->stRoleData.RoleDetail, sizeof(pstLog->LogRoleData.RoleDetail));
	//memcpy(&pstLog->LogRoleData.SkillInfo, &pstPlayer->stRoleData.SkillInfo, sizeof(pstLog->LogRoleData.SkillInfo));
	//memcpy(&pstLog->LogRoleData.StatusInfo, &pstPlayer->stRoleData.StatusInfo, sizeof(pstLog->LogRoleData.StatusInfo));
	//memcpy(&pstLog->LogRoleData.Wear, &pstPlayer->stRoleData.Wear, sizeof(pstLog->LogRoleData.Wear));
	//memcpy(&pstLog->RoleDetail, &pstPlayer->stRoleData.RoleDetail, sizeof(pstLog->RoleDetail));
	pstLog->LogRoleData = pstPlayer->stRoleData;
	z_rolemem_warp(pstPlayer, &pstLog->RoleInfo);
	
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_role_godcoin_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int iOperate,
								int iTianYuan, CSSHOPPINGBUYREQ *pstShoppingBuyReq, SHOPDEF *pstShopDef)
{	
	int i;
	OP_LOG_META stOpLog;
	OPBUYSHOPPINGCHG *pstOpBuyShoppingChg = &stOpLog.OpLog.OpBuyShoppingChg;
	UNUSED(pstEnv);
	
	stOpLog.OpLogType = OP_LOG_USE_BANGDINGTIANYUAN;

	memset(pstOpBuyShoppingChg, 0, sizeof(*pstOpBuyShoppingChg));
	z_rolemem_warp(pstPlayer, &pstOpBuyShoppingChg->RoleInfo);
	STRNCPY(pstOpBuyShoppingChg->Account,pstPlayer->szAccount,
		CCH(pstOpBuyShoppingChg->Account));
	if (pstPlayer)
	{
		pstOpBuyShoppingChg->RoleLevel = pstPlayer->stRoleData.Level;
	}
	else
	{
		pstOpBuyShoppingChg->RoleLevel = 0;
	}
	pstOpBuyShoppingChg->Operate = iOperate;
	pstOpBuyShoppingChg->MoneyNum= iTianYuan;
	pstOpBuyShoppingChg->ShopID = pstShoppingBuyReq->ShoppingID;
	pstOpBuyShoppingChg->ShopNum = pstShoppingBuyReq->ShoppingNum;
	pstOpBuyShoppingChg->GiftFlag = pstShoppingBuyReq->GiftFlag;
	pstOpBuyShoppingChg->GiftToRoleID = pstShoppingBuyReq->GiftToRoleID;
	pstOpBuyShoppingChg->LogTime = 0;

	if (iOperate) //金券
	{
		pstOpBuyShoppingChg->MoneyLeft = pstPlayer->stRoleData.RoleDetail.BindGodCoin;
	}
	else
	{
		pstOpBuyShoppingChg->MoneyLeft = pstPlayer->stRoleData.RoleDetail.GodCoin;
	}

	for (i=0; i<SHOP_ITEM_COUNT; i++)
	{
		if (pstShopDef->Items[i].ItemID == 0)
		{
			break;
		}

		pstOpBuyShoppingChg->ShopItem[i].ItemID = pstShopDef->Items[i].ItemID;
		pstOpBuyShoppingChg->ShopItem[i].ItemNum = pstShopDef->Items[i].ItemNum*pstOpBuyShoppingChg->ShopNum;
	}
	
	//return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_role_shopping_exchange_oplog(ZONESVRENV* pstEnv, unsigned long long ullRolelID, char *RoleName,
								char *szAccount,int Uin,int iLevel, int PointNum,int GodCoin,int UserGodCoin,char Operate)
{	
	OP_LOG_META stOpLog;
	OPSHOPINGEXCHANGE *pstOpShopingExchange = &stOpLog.OpLog.OpShopingExchange;
	
	UNUSED(pstEnv);

	memset(pstOpShopingExchange,0,sizeof(OPSHOPINGEXCHANGE));
	stOpLog.OpLogType = OP_LOG_SHOPPING_EXCHANGE;

	pstOpShopingExchange->RoleInfo.RoleID = ullRolelID;
	STRNCPY(pstOpShopingExchange->RoleInfo.RoleName,RoleName,CCH(pstOpShopingExchange->RoleInfo.RoleName));
	STRNCPY(pstOpShopingExchange->Account,szAccount,
		CCH(pstOpShopingExchange->Account));
	
	pstOpShopingExchange->RoleLevel = iLevel;
		
	//pstOpShopingExchange->Uin = Uin;
	pstOpShopingExchange->GodCoin = GodCoin;
	pstOpShopingExchange->UserGodCoin = UserGodCoin;
	pstOpShopingExchange->PointNum = PointNum;
	pstOpShopingExchange->Operate = Operate;
	pstOpShopingExchange->LogTime = 0;
	
	return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog);
	UNUSED(Uin);	//add by paraunused.pl
}

int z_bindgold_consume_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int OpType, 
								int OpSubType, unsigned int BindGoldNum, char *pszDesc)
{
	OP_LOG_META stOpLog;
	stOpLog.OpLogType = OP_LOG_BINDGOLD_CONSUME;
	OPBINDGOLDCONSUME *pstOpConsume = &stOpLog.OpLog.OpBindGoldConsume;

	if (BindGoldNum == 0)
	{
		return 0;
	}

	memset(pstOpConsume, 0, sizeof(*pstOpConsume));

	if (pstPlayer)
	{
		pstOpConsume->Uin = pstPlayer->stRoleData.Uin;
		pstOpConsume->RoleID = pstPlayer->stRoleData.RoleID;
		pstOpConsume->RoleLevel = pstPlayer->stRoleData.Level;
	}
	
	pstOpConsume->OpType = OpType;
	pstOpConsume->OpSubType = OpSubType;
	pstOpConsume->BindGoldNum = BindGoldNum;
	STRNCPY(pstOpConsume->OpDesc, pszDesc, sizeof(pstOpConsume->OpDesc));
	pstOpConsume->LogTime = 0;

	//return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_bindgold_create_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int OpType, int OpSubType, unsigned int BindGoldNum, char *pszDesc)
{
	OP_LOG_META stOpLog;
	stOpLog.OpLogType = OP_LOG_BINDGOLD_CREATE;
	OPBINDGOLDCREATE *pstOpCreate = &stOpLog.OpLog.OpBindGoldCreate;

	memset(pstOpCreate, 0, sizeof(*pstOpCreate));
	if (pstPlayer)
	{
		pstOpCreate->Uin = pstPlayer->stRoleData.Uin;
		pstOpCreate->RoleID = pstPlayer->stRoleData.RoleID;
		pstOpCreate->RoleLevel = pstPlayer->stRoleData.Level;
	}
	
	pstOpCreate->OpType = OpType;
	pstOpCreate->OpSubType = OpSubType;
	pstOpCreate->BindGoldNum = BindGoldNum;
	pstOpCreate->LogTime = 0;
	STRNCPY(pstOpCreate->OpDesc, pszDesc, sizeof(pstOpCreate->OpDesc));
	
	//return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_money_consume_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int OpType, int OpSubType, unsigned int BindMoneyNum, unsigned int MoneyNum, char *pszDesc)
{
	OP_LOG_META stOpLog;
	stOpLog.OpLogType = OP_LOG_MONEY_CONSUME;
	OPMONEYCONSUME *pstOpMoney = &stOpLog.OpLog.OpMoneyConsume;

	memset(pstOpMoney, 0, sizeof(*pstOpMoney));
	if (pstPlayer)
	{
		pstOpMoney->Uin = pstPlayer->stRoleData.Uin;
		pstOpMoney->RoleID = pstPlayer->stRoleData.RoleID;
		pstOpMoney->RoleLevel = pstPlayer->stRoleData.Level;
	}
	
	pstOpMoney->OpType = OpType;
	pstOpMoney->OpSubType = OpSubType;
	pstOpMoney->BindMoneyNum = BindMoneyNum;
	pstOpMoney->MoneyNum = MoneyNum;
	pstOpMoney->LogTime = 0;
	STRNCPY(pstOpMoney->OpDesc, pszDesc, sizeof(pstOpMoney->OpDesc));
	
	//return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_battle_money_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int iType, unsigned int MoneyNum, char *pszDesc)
{
	OP_LOG_META stOpLog;
	stOpLog.OpLogType = OP_LOG_BATTLE_MONEY_USE;
	OPBATTLEMONEYUSE *pstOp = &stOpLog.OpLog.OpBattleMoneyUse;
	
	memset(pstOp, 0, sizeof(*pstOp));
	if (pstPlayer)
	{
		pstOp->Uin = pstPlayer->stRoleData.Uin;
		pstOp->RoleID = pstPlayer->stRoleData.RoleID;
		pstOp->RoleLevel = pstPlayer->stRoleData.Level;
	}

	pstOp->MoneyType = iType;
	pstOp->MoneyNum = MoneyNum;
	STRNCPY(pstOp->OpDesc, pszDesc, sizeof(pstOp->OpDesc));
	
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_money_create_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int OpType, int OpSubType, unsigned int BindMoneyNum, unsigned int MoneyNum, char *pszDesc)
{
	OP_LOG_META stOpLog;
	stOpLog.OpLogType = OP_LOG_MONEY_CREATE;
	OPMONEYCREATE *pstOpMoney = &stOpLog.OpLog.OpMoneyCreate;

	memset(pstOpMoney, 0, sizeof(*pstOpMoney));
	if (pstPlayer)
	{
		pstOpMoney->Uin = pstPlayer->stRoleData.Uin;
		pstOpMoney->RoleID = pstPlayer->stRoleData.RoleID;
		pstOpMoney->RoleLevel = pstPlayer->stRoleData.Level;
	}
	
	pstOpMoney->OpType = OpType;
	pstOpMoney->OpSubType = OpSubType;
	pstOpMoney->BindMoneyNum = BindMoneyNum;
	pstOpMoney->MoneyNum = MoneyNum;
	pstOpMoney->LogTime = 0;
	STRNCPY(pstOpMoney->OpDesc, pszDesc, sizeof(pstOpMoney->OpDesc));
	
	//return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_impitem_consume_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int iItemID, int OpType, 
									unsigned int iNum, int iBindNum)
{
	OP_LOG_META stOpLog;
	stOpLog.OpLogType = OP_LOG_IMPITEM_CONSUME;
	OPIMPITEMCONSUME *pstOpImpItem = &stOpLog.OpLog.OpImpItemConsume;

	memset(pstOpImpItem, 0, sizeof(*pstOpImpItem));
	if (pstPlayer)
	{
		pstOpImpItem->Uin = pstPlayer->stRoleData.Uin;
		pstOpImpItem->RoleID = pstPlayer->stRoleData.RoleID;
		pstOpImpItem->RoleLevel = pstPlayer->stRoleData.Level;
	}

	pstOpImpItem->ItemID = iItemID;
	pstOpImpItem->OpType = OpType;
	pstOpImpItem->Num = iNum;
	pstOpImpItem->BindNum = iBindNum;
	pstOpImpItem->LogTime = 0;
	pstOpImpItem->OpDesc[0] = 0;

	switch(OpType)
	{
	case IMPITEM_OPTYPE_ARMWASH:
		STRNCPY(pstOpImpItem->OpDesc, "装备洗炼道具", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_ARMSTAR:
		STRNCPY(pstOpImpItem->OpDesc, "装备强化道具", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_GEMLEVUP:
		STRNCPY(pstOpImpItem->OpDesc, "宝石升级材料", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_CLAN_RESOURCE:
		STRNCPY(pstOpImpItem->OpDesc, "公会资源", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_GEMIN:
		STRNCPY(pstOpImpItem->OpDesc, "宝石镶嵌", sizeof(pstOpImpItem->OpDesc));
		break;

	case IMPITEM_OPTYPE_GEMPICK:
		STRNCPY(pstOpImpItem->OpDesc, "宝石摘取符", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_ARMSTARMOV:
		STRNCPY(pstOpImpItem->OpDesc, "装备转星", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_ARMUNBIND:
		STRNCPY(pstOpImpItem->OpDesc, "装备解绑", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_ARMFENJIE:
		STRNCPY(pstOpImpItem->OpDesc, "装备分解", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_GEMADDPOWER:
		STRNCPY(pstOpImpItem->OpDesc, "宝石充能", sizeof(pstOpImpItem->OpDesc));
		break;	
		
	case IMPITEM_OPTYPE_ARMRECAST:
		STRNCPY(pstOpImpItem->OpDesc, "装备重铸", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_SPARIN:
		STRNCPY(pstOpImpItem->OpDesc, "晶石吸收", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_COLLECT:
		STRNCPY(pstOpImpItem->OpDesc, "采集", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_SKILL:
		STRNCPY(pstOpImpItem->OpDesc, "技能", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_FAIRYEVOLVE:
		STRNCPY(pstOpImpItem->OpDesc, "精灵进化", sizeof(pstOpImpItem->OpDesc));
		break;	
		
	case IMPITEM_OPTYPE_FAIRYSTAR:
		STRNCPY(pstOpImpItem->OpDesc, "精灵强化", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_FAIRYKEYIN:
		STRNCPY(pstOpImpItem->OpDesc, "精灵刻印", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_DART:
		STRNCPY(pstOpImpItem->OpDesc, "镖车", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_MAIL:
		STRNCPY(pstOpImpItem->OpDesc, "邮件", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_BCT:
		STRNCPY(pstOpImpItem->OpDesc, "大地之心", sizeof(pstOpImpItem->OpDesc));
		break;	

	case IMPITEM_OPTYPE_SHOP:
		STRNCPY(pstOpImpItem->OpDesc, "商店兑换", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_PAK_EX:
		STRNCPY(pstOpImpItem->OpDesc, "背包扩展", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_RIDELEV:
		STRNCPY(pstOpImpItem->OpDesc, "坐骑升级", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_ARMKEYIN:
		STRNCPY(pstOpImpItem->OpDesc, "装备刻印", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_ARM_FUMO:
		STRNCPY(pstOpImpItem->OpDesc, "装备附魔", sizeof(pstOpImpItem->OpDesc));
		break;	

	case IMPITEM_OPTYPE_PETSKILL:
		STRNCPY(pstOpImpItem->OpDesc, "宠物技能", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_PETSTRENG:
		STRNCPY(pstOpImpItem->OpDesc, "宠物强化", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_PETSKILLGRID:
		STRNCPY(pstOpImpItem->OpDesc, "宠物技能栏", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_PET_RESETSKILL:
		STRNCPY(pstOpImpItem->OpDesc, "宠物重置技能", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_PET_RESWASH:
		STRNCPY(pstOpImpItem->OpDesc, "宠物幻化", sizeof(pstOpImpItem->OpDesc));
		break;	
		
	case IMPITEM_OPTYPE_CHAT:
		STRNCPY(pstOpImpItem->OpDesc, "聊天频道", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_REVIVE:
		STRNCPY(pstOpImpItem->OpDesc, "复活", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_CAREER:
		STRNCPY(pstOpImpItem->OpDesc, "转职", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_GPRS:
		STRNCPY(pstOpImpItem->OpDesc, "定位", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_RUNE:
		STRNCPY(pstOpImpItem->OpDesc, "符文", sizeof(pstOpImpItem->OpDesc));
		break;

	case IMPITEM_OPTYPE_SCRIPT:
		STRNCPY(pstOpImpItem->OpDesc, "脚本", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_RECIPE:
		STRNCPY(pstOpImpItem->OpDesc, "生活技能配方", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_SJ:
		STRNCPY(pstOpImpItem->OpDesc, "赏金", sizeof(pstOpImpItem->OpDesc));
		break;
	
		break;
	case IMPITEM_OPTYPE_TASK:
		STRNCPY(pstOpImpItem->OpDesc, "任务", sizeof(pstOpImpItem->OpDesc));
		break;	
		

	case IMPITEM_OPTYPE_PICK:
		STRNCPY(pstOpImpItem->OpDesc, "拾取", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_CARD:
		STRNCPY(pstOpImpItem->OpDesc, "翻牌", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_CLAN_RESOURCE_MACHINE:
		STRNCPY(pstOpImpItem->OpDesc, "矿车油车", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_RAFFLE:
		STRNCPY(pstOpImpItem->OpDesc, "抽奖", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_GIFT:
		STRNCPY(pstOpImpItem->OpDesc, "礼包", sizeof(pstOpImpItem->OpDesc));
		break;	
		
	case IMPITEM_OPTYPE_PETCATCH:
		STRNCPY(pstOpImpItem->OpDesc, "宠物捕捉", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_PET_RESOLVE:
		STRNCPY(pstOpImpItem->OpDesc, "宠物放生", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_DAY:
		STRNCPY(pstOpImpItem->OpDesc, "今日目标", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_SHOPPING:
		STRNCPY(pstOpImpItem->OpDesc, "商城", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_USE:
		STRNCPY(pstOpImpItem->OpDesc, "道具使用", sizeof(pstOpImpItem->OpDesc));
		break;	
	case IMPITEM_OPTYPE_BAOXIANG:
		STRNCPY(pstOpImpItem->OpDesc, "宝箱", sizeof(pstOpImpItem->OpDesc));
		break;	
	case IMPITEM_OPTYPE_GM:
		STRNCPY(pstOpImpItem->OpDesc, "GM", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_SHUNIU:
		STRNCPY(pstOpImpItem->OpDesc, "枢纽", sizeof(pstOpImpItem->OpDesc));
		break;	
	case IMPITEM_OPTYPE_WEDDING:
		STRNCPY(pstOpImpItem->OpDesc, "结婚系统", sizeof(pstOpImpItem->OpDesc));
		break;	
	case IMPITEM_OPTYPE_GEM_PAG:
		STRNCPY(pstOpImpItem->OpDesc, "宝石包", sizeof(pstOpImpItem->OpDesc));
		break;	
	case IMPITEM_OPTYPE_PET_UNBIND:
		STRNCPY(pstOpImpItem->OpDesc, "宠物解绑", sizeof(pstOpImpItem->OpDesc));
		break;	
	case IMPITEM_OPTYPE_HOME:
		STRNCPY(pstOpImpItem->OpDesc, "家园", sizeof(pstOpImpItem->OpDesc));
		break;	
	case IMPITEM_OPTYPE_SUBJECT:
		STRNCPY(pstOpImpItem->OpDesc, "智力竞赛", sizeof(pstOpImpItem->OpDesc));
		break;	
	}
	
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

	
int z_impitem_create_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int iItemID, int OpType, unsigned int iNum, int iBindNum)
{
	OP_LOG_META stOpLog;
	stOpLog.OpLogType = OP_LOG_IMPITEM_CREATE;
	OPIMPITEMCREATE *pstOpImpItem = &stOpLog.OpLog.OpImpItemCreate;

	memset(pstOpImpItem, 0, sizeof(*pstOpImpItem));
	if (pstPlayer)
	{
		pstOpImpItem->Uin = pstPlayer->stRoleData.Uin;
		pstOpImpItem->RoleID = pstPlayer->stRoleData.RoleID;
		pstOpImpItem->RoleLevel = pstPlayer->stRoleData.Level;
	}
	
	pstOpImpItem->ItemID = iItemID;
	pstOpImpItem->OpType = OpType;
	pstOpImpItem->Num = iNum;	
	pstOpImpItem->BindNum = iBindNum;
	pstOpImpItem->LogTime = 0;
	pstOpImpItem->OpDesc[0] = 0;
	
	switch(OpType)
	{
	case IMPITEM_OPTYPE_ARMWASH:
		STRNCPY(pstOpImpItem->OpDesc, "装备洗炼道具", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_ARMSTAR:
		STRNCPY(pstOpImpItem->OpDesc, "装备强化道具", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_GEMLEVUP:
		STRNCPY(pstOpImpItem->OpDesc, "宝石升级材料", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_CLAN_RESOURCE:
		STRNCPY(pstOpImpItem->OpDesc, "公会资源", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_GEMIN:
		STRNCPY(pstOpImpItem->OpDesc, "宝石镶嵌", sizeof(pstOpImpItem->OpDesc));
		break;

	case IMPITEM_OPTYPE_GEMPICK:
		STRNCPY(pstOpImpItem->OpDesc, "宝石摘取符", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_ARMSTARMOV:
		STRNCPY(pstOpImpItem->OpDesc, "装备转星", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_ARMUNBIND:
		STRNCPY(pstOpImpItem->OpDesc, "装备解绑", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_ARMFENJIE:
		STRNCPY(pstOpImpItem->OpDesc, "装备分解", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_GEMADDPOWER:
		STRNCPY(pstOpImpItem->OpDesc, "宝石充能", sizeof(pstOpImpItem->OpDesc));
		break;	
		
	case IMPITEM_OPTYPE_ARMRECAST:
		STRNCPY(pstOpImpItem->OpDesc, "装备重铸", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_SPARIN:
		STRNCPY(pstOpImpItem->OpDesc, "晶石吸收", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_COLLECT:
		STRNCPY(pstOpImpItem->OpDesc, "采集", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_SKILL:
		STRNCPY(pstOpImpItem->OpDesc, "技能", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_FAIRYEVOLVE:
		STRNCPY(pstOpImpItem->OpDesc, "精灵进化", sizeof(pstOpImpItem->OpDesc));
		break;	
		
	case IMPITEM_OPTYPE_FAIRYSTAR:
		STRNCPY(pstOpImpItem->OpDesc, "精灵强化", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_FAIRYKEYIN:
		STRNCPY(pstOpImpItem->OpDesc, "精灵刻印", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_DART:
		STRNCPY(pstOpImpItem->OpDesc, "镖车", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_MAIL:
		STRNCPY(pstOpImpItem->OpDesc, "邮件", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_BCT:
		STRNCPY(pstOpImpItem->OpDesc, "大地之心", sizeof(pstOpImpItem->OpDesc));
		break;	

	case IMPITEM_OPTYPE_SHOP:
		STRNCPY(pstOpImpItem->OpDesc, "商店兑换", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_PAK_EX:
		STRNCPY(pstOpImpItem->OpDesc, "背包扩展", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_RIDELEV:
		STRNCPY(pstOpImpItem->OpDesc, "坐骑升级", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_ARMKEYIN:
		STRNCPY(pstOpImpItem->OpDesc, "装备刻印", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_ARM_FUMO:
		STRNCPY(pstOpImpItem->OpDesc, "装备附魔", sizeof(pstOpImpItem->OpDesc));
		break;	

	case IMPITEM_OPTYPE_PETSKILL:
		STRNCPY(pstOpImpItem->OpDesc, "宠物技能", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_PETSTRENG:
		STRNCPY(pstOpImpItem->OpDesc, "宠物强化", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_PETSKILLGRID:
		STRNCPY(pstOpImpItem->OpDesc, "宠物技能栏", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_PET_RESETSKILL:
		STRNCPY(pstOpImpItem->OpDesc, "宠物重置技能", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_PET_RESWASH:
		STRNCPY(pstOpImpItem->OpDesc, "宠物幻化", sizeof(pstOpImpItem->OpDesc));
		break;	
		
	case IMPITEM_OPTYPE_CHAT:
		STRNCPY(pstOpImpItem->OpDesc, "聊天频道", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_REVIVE:
		STRNCPY(pstOpImpItem->OpDesc, "复活", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_CAREER:
		STRNCPY(pstOpImpItem->OpDesc, "转职", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_GPRS:
		STRNCPY(pstOpImpItem->OpDesc, "定位", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_RUNE:
		STRNCPY(pstOpImpItem->OpDesc, "符文", sizeof(pstOpImpItem->OpDesc));
		break;

	case IMPITEM_OPTYPE_SCRIPT:
		STRNCPY(pstOpImpItem->OpDesc, "脚本", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_RECIPE:
		STRNCPY(pstOpImpItem->OpDesc, "生活技能配方", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_SJ:
		STRNCPY(pstOpImpItem->OpDesc, "赏金", sizeof(pstOpImpItem->OpDesc));
		break;
	
		break;
	case IMPITEM_OPTYPE_TASK:
		STRNCPY(pstOpImpItem->OpDesc, "任务", sizeof(pstOpImpItem->OpDesc));
		break;	
		

	case IMPITEM_OPTYPE_PICK:
		STRNCPY(pstOpImpItem->OpDesc, "拾取", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_CARD:
		STRNCPY(pstOpImpItem->OpDesc, "翻牌", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_CLAN_RESOURCE_MACHINE:
		STRNCPY(pstOpImpItem->OpDesc, "矿车油车", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_RAFFLE:
		STRNCPY(pstOpImpItem->OpDesc, "抽奖", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_GIFT:
		STRNCPY(pstOpImpItem->OpDesc, "礼包", sizeof(pstOpImpItem->OpDesc));
		break;	
		
	case IMPITEM_OPTYPE_PETCATCH:
		STRNCPY(pstOpImpItem->OpDesc, "宠物捕捉", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_PET_RESOLVE:
		STRNCPY(pstOpImpItem->OpDesc, "宠物放生", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_DAY:
		STRNCPY(pstOpImpItem->OpDesc, "今日目标", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_SHOPPING:
		STRNCPY(pstOpImpItem->OpDesc, "商城", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_USE:
		STRNCPY(pstOpImpItem->OpDesc, "道具使用", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_BAOXIANG:
		STRNCPY(pstOpImpItem->OpDesc, "宝箱", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_GM:
		STRNCPY(pstOpImpItem->OpDesc, "GM", sizeof(pstOpImpItem->OpDesc));
		break;
	case IMPITEM_OPTYPE_SHUNIU:
		STRNCPY(pstOpImpItem->OpDesc, "枢纽", sizeof(pstOpImpItem->OpDesc));
		break;	
	case IMPITEM_OPTYPE_WEDDING:
		STRNCPY(pstOpImpItem->OpDesc, "结婚系统", sizeof(pstOpImpItem->OpDesc));
		break;	
	case IMPITEM_OPTYPE_GEM_PAG:
		STRNCPY(pstOpImpItem->OpDesc, "宝石包", sizeof(pstOpImpItem->OpDesc));
		break;	
	case IMPITEM_OPTYPE_PET_UNBIND:
		STRNCPY(pstOpImpItem->OpDesc, "宠物解绑", sizeof(pstOpImpItem->OpDesc));
		break;	
	case IMPITEM_OPTYPE_HOME:
		STRNCPY(pstOpImpItem->OpDesc, "家园", sizeof(pstOpImpItem->OpDesc));
		break;		
	case IMPITEM_OPTYPE_SUBJECT:
		STRNCPY(pstOpImpItem->OpDesc, "智力竞赛", sizeof(pstOpImpItem->OpDesc));
		break;	
	}
	
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_clanprop_consume_oplog(ZONESVRENV* pstEnv, ZoneClan *pstClan, int iPropID, int OpType, int OpSubType, unsigned int iNum)
{
	OP_LOG_META stOpLog;
	stOpLog.OpLogType = OP_LOG_CLANPROP_CONSUME;
	OPCLANPROPERTYCONSUME *pstOp = &stOpLog.OpLog.OpClanPropertyConsume;

	memset(pstOp, 0, sizeof(*pstOp));

	if (iNum == 0)
	{
		return 0;
	}

	pstOp->ClanId = pstClan->stClan.ClanProf.Id;
	pstOp->ClanWID = pstClan->stClan.ClanProf.GID;
	pstOp->PropertyID = iPropID;
	pstOp->OpType = OpType;
	pstOp->OpSubType = OpSubType;
	pstOp->Num = iNum;	
	pstOp->LogTime = 0;
	pstOp->OpDesc[0] = 0;
	
	switch(OpType)
	{
	case OP_OPTYPE_TIMER:
		STRNCPY(pstOp->OpDesc, "定时消耗", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_DAYPWORLD:
		STRNCPY(pstOp->OpDesc, "公会每日活动", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_TASK:
		STRNCPY(pstOp->OpDesc, "任务", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_MAKE:
		STRNCPY(pstOp->OpDesc, "公会制造", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_ROLLBACK:
		STRNCPY(pstOp->OpDesc, "取消公会制造", sizeof(pstOp->OpDesc));
		break;	

		
	case OP_OPTYPE_RESEARCH:
		STRNCPY(pstOp->OpDesc, "公会科技研究", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_RESEARCH_CANCEL:
		STRNCPY(pstOp->OpDesc, "取消公会科技研究", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_RESEARCH_SPEED:
		STRNCPY(pstOp->OpDesc, "加速公会科技研究", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_MAKE_SPEED:
		STRNCPY(pstOp->OpDesc, "加速公会制造", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_UPGRADE:
		STRNCPY(pstOp->OpDesc, "公会升级", sizeof(pstOp->OpDesc));
		break;	
	case OP_OPTYPE_GM:
		STRNCPY(pstOp->OpDesc, "GM", sizeof(pstOp->OpDesc));
		break;	
	case OP_OPTYPE_CITAN:
		STRNCPY(pstOp->OpDesc, "刺探", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_GRAB:
		STRNCPY(pstOp->OpDesc, "兑换公会资源", sizeof(pstOp->OpDesc));
		break;	
	case OP_OPTYPE_SHOP:
		STRNCPY(pstOp->OpDesc, "商店消耗", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_GIFT:
		STRNCPY(pstOp->OpDesc, "礼包", sizeof(pstOp->OpDesc));
		break;	
	case OP_OPTYPE_STRONG_SALE:
		STRNCPY(pstOp->OpDesc, "据点竞拍", sizeof(pstOp->OpDesc));
		break;	
	}

	return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
}

int z_clanprop_create_oplog(ZONESVRENV* pstEnv, ZoneClan *pstClan, int iPropID, int OpType, int OpSubType, unsigned int iNum)
{
	OP_LOG_META stOpLog;
	stOpLog.OpLogType = OP_LOG_CLANPROP_CREATE;
	OPCLANPROPERTYCREATE *pstOp = &stOpLog.OpLog.OpClanPropertyCreate;

	memset(pstOp, 0, sizeof(*pstOp));

	pstOp->ClanId = pstClan->stClan.ClanProf.Id;
	pstOp->ClanWID = pstClan->stClan.ClanProf.GID;
	pstOp->PropertyID = iPropID;
	pstOp->OpType = OpType;
	pstOp->OpSubType = OpSubType;
	pstOp->Num = iNum;	
	pstOp->LogTime = 0;
	pstOp->OpDesc[0] = 0;
	
	switch(OpType)
	{
	case OP_OPTYPE_TIMER:
		STRNCPY(pstOp->OpDesc, "定时消耗", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_DAYPWORLD:
		STRNCPY(pstOp->OpDesc, "公会每日活动", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_TASK:
		STRNCPY(pstOp->OpDesc, "任务", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_MAKE:
		STRNCPY(pstOp->OpDesc, "公会制造", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_ROLLBACK:
		STRNCPY(pstOp->OpDesc, "取消公会制造", sizeof(pstOp->OpDesc));
		break;	

		
	case OP_OPTYPE_RESEARCH:
		STRNCPY(pstOp->OpDesc, "公会科技研究", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_RESEARCH_CANCEL:
		STRNCPY(pstOp->OpDesc, "取消公会科技研究", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_RESEARCH_SPEED:
		STRNCPY(pstOp->OpDesc, "加速公会科技研究", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_MAKE_SPEED:
		STRNCPY(pstOp->OpDesc, "加速公会制造", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_UPGRADE:
		STRNCPY(pstOp->OpDesc, "公会升级", sizeof(pstOp->OpDesc));
		break;	
	case OP_OPTYPE_GM:
		STRNCPY(pstOp->OpDesc, "GM", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_CITAN:
		STRNCPY(pstOp->OpDesc, "刺探", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_GRAB:
		STRNCPY(pstOp->OpDesc, "兑换公会资源", sizeof(pstOp->OpDesc));
		break;	
	case OP_OPTYPE_SHOP:
		STRNCPY(pstOp->OpDesc, "商店消耗", sizeof(pstOp->OpDesc));
		break;	
	case OP_OPTYPE_GIFT:
		STRNCPY(pstOp->OpDesc, "礼包", sizeof(pstOp->OpDesc));
		break;	
	case OP_OPTYPE_STRONG_SALE:
		STRNCPY(pstOp->OpDesc, "据点竞拍", sizeof(pstOp->OpDesc));
		break;	
	}

	return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
}

int z_contri_consume_oplog(ZONESVRENV* pstEnv, ZoneClan *pstClan, int OpType, int OpSubType, unsigned int iNum)
{
	OP_LOG_META stOpLog;
	stOpLog.OpLogType = OP_LOG_CONTRI_CONSUME;
	OPCONTRICONSUME *pstOp = &stOpLog.OpLog.OpContriConsume;

	memset(pstOp, 0, sizeof(*pstOp));

	pstOp->ClanId = pstClan->stClan.ClanProf.Id;
	pstOp->ClanWID = pstClan->stClan.ClanProf.GID;
	pstOp->OpType = OpType;
	pstOp->OpSubType = OpSubType;
	pstOp->Num = iNum;	
	pstOp->LogTime = 0;
	pstOp->OpDesc[0] = 0;
	
	switch(OpType)
	{
	case OP_OPTYPE_TIMER:
		STRNCPY(pstOp->OpDesc, "定时消耗", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_DAYPWORLD:
		STRNCPY(pstOp->OpDesc, "公会每日活动", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_TASK:
		STRNCPY(pstOp->OpDesc, "任务", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_MAKE:
		STRNCPY(pstOp->OpDesc, "公会制造", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_ROLLBACK:
		STRNCPY(pstOp->OpDesc, "取消公会制造", sizeof(pstOp->OpDesc));
		break;	

		
	case OP_OPTYPE_RESEARCH:
		STRNCPY(pstOp->OpDesc, "公会科技研究", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_RESEARCH_CANCEL:
		STRNCPY(pstOp->OpDesc, "取消公会科技研究", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_RESEARCH_SPEED:
		STRNCPY(pstOp->OpDesc, "加速公会科技研究", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_MAKE_SPEED:
		STRNCPY(pstOp->OpDesc, "加速公会制造", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_UPGRADE:
		STRNCPY(pstOp->OpDesc, "公会升级", sizeof(pstOp->OpDesc));
		break;	
	case OP_OPTYPE_GM:
		STRNCPY(pstOp->OpDesc, "GM", sizeof(pstOp->OpDesc));
		break;	
	case OP_OPTYPE_CITAN:
		STRNCPY(pstOp->OpDesc, "刺探", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_GRAB:
		STRNCPY(pstOp->OpDesc, "兑换公会资源", sizeof(pstOp->OpDesc));
		break;	
	case OP_OPTYPE_SHOP:
		STRNCPY(pstOp->OpDesc, "商店消耗", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_GIFT:
		STRNCPY(pstOp->OpDesc, "礼包", sizeof(pstOp->OpDesc));
		break;	
	}

	return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
}

int z_contri_create_oplog(ZONESVRENV* pstEnv, ZoneClan *pstClan, int OpType, int OpSubType, unsigned int iNum)
{
	OP_LOG_META stOpLog;
	stOpLog.OpLogType = OP_LOG_CONTRI_CREATE;
	OPCONTRICREATE *pstOp = &stOpLog.OpLog.OpContriCreate;

	memset(pstOp, 0, sizeof(*pstOp));

	pstOp->ClanId = pstClan->stClan.ClanProf.Id;
	pstOp->ClanWID = pstClan->stClan.ClanProf.GID;
	pstOp->OpType = OpType;
	pstOp->OpSubType = OpSubType;
	pstOp->Num = iNum;	
	pstOp->LogTime = 0;
	pstOp->OpDesc[0] = 0;
	
	switch(OpType)
	{
	case OP_OPTYPE_TIMER:
		STRNCPY(pstOp->OpDesc, "定时消耗", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_DAYPWORLD:
		STRNCPY(pstOp->OpDesc, "公会每日活动", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_TASK:
		STRNCPY(pstOp->OpDesc, "任务", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_MAKE:
		STRNCPY(pstOp->OpDesc, "公会制造", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_ROLLBACK:
		STRNCPY(pstOp->OpDesc, "取消公会制造", sizeof(pstOp->OpDesc));
		break;	

		
	case OP_OPTYPE_RESEARCH:
		STRNCPY(pstOp->OpDesc, "公会科技研究", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_RESEARCH_CANCEL:
		STRNCPY(pstOp->OpDesc, "取消公会科技研究", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_RESEARCH_SPEED:
		STRNCPY(pstOp->OpDesc, "加速公会科技研究", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_MAKE_SPEED:
		STRNCPY(pstOp->OpDesc, "加速公会制造", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_UPGRADE:
		STRNCPY(pstOp->OpDesc, "公会升级", sizeof(pstOp->OpDesc));
		break;	
	case OP_OPTYPE_GM:
		STRNCPY(pstOp->OpDesc, "GM", sizeof(pstOp->OpDesc));
		break;	
	case OP_OPTYPE_COMMIT:
		STRNCPY(pstOp->OpDesc, "提交公会资源", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_CITAN:
		STRNCPY(pstOp->OpDesc, "刺探", sizeof(pstOp->OpDesc));
		break;
	case OP_OPTYPE_GRAB:
		STRNCPY(pstOp->OpDesc, "兑换公会资源", sizeof(pstOp->OpDesc));
		break;	
	case OP_OPTYPE_SHOP:
		STRNCPY(pstOp->OpDesc, "商店消耗", sizeof(pstOp->OpDesc));
		break;	
	case OP_OPTYPE_GIFT:
		STRNCPY(pstOp->OpDesc, "礼包", sizeof(pstOp->OpDesc));
		break;
	}

	return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
}

int z_role_trade_oplog(ZONESVRENV* pstEnv, OPTRADEINFO *pstOpTradeInfo)
{	
	OP_LOG_META stOpLog;
	stOpLog.OpLogType = OP_TRADE_INFO;

	pstOpTradeInfo->OperDesc[sizeof(pstOpTradeInfo->OperDesc) - 1] = 0;
	memcpy(&stOpLog.OpLog.OpTradeInfo,pstOpTradeInfo,sizeof(OPTRADEINFO));
	
	return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog);
}

int z_role_lvup_snapshot_oplog(ZONESVRENV* pstEnv, Player *pstPlayer,
                               int iForwardLv, tdr_longlong iForwardExp,char CareerUpFlag)
{
	OP_LOG_META stOpLog;
	OPROLELVUPSNAPSHOT *pstLog = &stOpLog.OpLog.RoleLvUpSnapshot;
	
	stOpLog.OpLogType = OP_LOG_ROLE_LVUP_SNAPSHOT;
	
	z_rolemem_warp(pstPlayer, &pstLog->RoleInfo);
	STRNCPY(pstLog->LoginName, pstPlayer->szAccount, CCH(pstLog->LoginName));
	pstLog->Uin = pstPlayer->stRoleData.Uin;
	pstLog->Career = pstPlayer->stRoleData.Career;
	pstLog->CareerUpFlag = CareerUpFlag;
	pstLog->ForwardExp = iForwardExp;
	pstLog->CurrentExp = pstPlayer->stRoleData.RoleDetail.Exp;
	pstLog->ForwardLevel = iForwardLv;
	pstLog->CurrentLevel = pstPlayer->stRoleData.Level;
	pstLog->Bull = pstPlayer->stRoleData.RoleDetail.Bull;
	pstLog->Money = pstPlayer->stRoleData.RoleDetail.Money;
	pstLog->StashBull = pstPlayer->stRoleData.Package.StashPackage.Bull;
	pstLog->StashMoney = pstPlayer->stRoleData.Package.StashPackage.Money;
	pstLog->Map = GET_MAPID(pstPlayer->stRoleData.Map);
	pstLog->Pos_x =  pstPlayer->stRoleData.Pos.X;
	pstLog->Pos_y =  pstPlayer->stRoleData.Pos.Y;
	
	//return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_role_use_xp_skill_log(ZONESVRENV* pstEnv, Player *pstPlayer, int iSkillID, int iSkillLv,int iUseXp)
{
	OP_LOG_META stOpLog;

	OPUSEXPSKILL *pstLog = &stOpLog.OpLog.OpUseXpSKill;
	stOpLog.OpLogType = OP_LOG_USE_XP_SKILL;

	z_rolemem_warp(pstPlayer, &pstLog->PlayerInfo);
	STRNCPY(pstLog->PlayerLoginName, pstPlayer->szAccount, CCH(pstLog->PlayerLoginName));
	pstLog->PlayerUin = pstPlayer->stRoleData.Uin;
	pstLog->PlayerLevel = pstPlayer->stRoleData.Level;
	
	pstLog->XpSkillID = iSkillID;
	pstLog->XpSkillLevel = iSkillLv;
	pstLog->UseXp = iUseXp;

	
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
    	//OPGIFT OpGift;                          
}

int z_role_gift_log1(ZONESVRENV* pstEnv,tdr_ulonglong ullRoleID,const char*pszRoleName,
					const char*pszAcc,int iUin,int iLvl,int iGiftType, char *szGiftName)
{
	OP_LOG_META stOpLog;
	OPGIFT *pstLog = &stOpLog.OpLog.OpGift;
	memset(pstLog, 0, sizeof(*pstLog));
	
	stOpLog.OpLogType = OP_LOG_GIFT;

	if (ullRoleID > 0)
		pstLog->PlayerInfo.RoleID = ullRoleID;
	if (pszRoleName)
		STRNCPY(pstLog->PlayerInfo.RoleName, pszRoleName, sizeof(pstLog->PlayerInfo.RoleName));
	if (pszAcc)
		STRNCPY(pstLog->PlayerLoginName, pszAcc, CCH(pstLog->PlayerLoginName));
	if (iUin > 0)
		pstLog->PlayerUin = iUin;
	if (iLvl)
		pstLog->PlayerLevel = iLvl;
	if (szGiftName)
		STRNCPY(pstLog->GiftName, szGiftName, CCH(pstLog->GiftName));
	if (iGiftType > 0)
		pstLog->GiftType = iGiftType;	
	return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);

}

int z_role_gift_log(ZONESVRENV* pstEnv, Player *pstPlayer, char *pszRoleName, int iGiftType, char *szGiftName, int iGiftID,tdr_ulonglong CashUp)
{
	OP_LOG_META stOpLog;

	OPGIFT *pstLog = &stOpLog.OpLog.OpGift;

	memset(pstLog, 0, sizeof(*pstLog));
	
	stOpLog.OpLogType = OP_LOG_GIFT;

	if (pstPlayer)
	{
		z_rolemem_warp(pstPlayer, &pstLog->PlayerInfo);
		STRNCPY(pstLog->PlayerLoginName, pstPlayer->szAccount, CCH(pstLog->PlayerLoginName));
		pstLog->PlayerUin = pstPlayer->stRoleData.Uin;
		pstLog->PlayerLevel = pstPlayer->stRoleData.Level;
	}
	else
	{
		STRNCPY(pstLog->PlayerInfo.RoleName, pszRoleName, sizeof(pstLog->PlayerInfo.RoleName));
	}
	
	STRNCPY(pstLog->GiftName, szGiftName, CCH(pstLog->GiftName));
	pstLog->GiftType = iGiftType;	
	pstLog->GiftID = iGiftID;
	pstLog->GiftCashUp = CashUp;
	
	//return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_role_media_gift_log(ZONESVRENV* pstEnv, Player *pstPlayer,  char *szGiftName, int iUseItemID,GIFTITEM * pstSelOneGiftItem,GIFTITEM * pstSelAllGiftItem,int iSelAllGiftItemsLen)
{
	OP_LOG_META stOpLog;

	OPMEDIAGIFT * pstLog = &stOpLog.OpLog.OpMediaGift;

	memset(pstLog, 0, sizeof(*pstLog));
	
	stOpLog.OpLogType = OP_LOG_MEDIA_GIFT;

	z_rolemem_warp(pstPlayer, &pstLog->PlayerInfo);
	STRNCPY(pstLog->PlayerLoginName, pstPlayer->szAccount, CCH(pstLog->PlayerLoginName));
	pstLog->PlayerUin = pstPlayer->stRoleData.Uin;
	pstLog->PlayerLevel = pstPlayer->stRoleData.Level;
	
	STRNCPY(pstLog->GiftName, szGiftName, CCH(pstLog->GiftName));
	pstLog->UseItemID = iUseItemID;
	if(pstSelOneGiftItem)
	{
		memcpy(	&pstLog->GiftSelOneData,pstSelOneGiftItem,sizeof(pstLog->GiftSelOneData));
	}
	memcpy(&pstLog->GiftSelAllData,pstSelAllGiftItem,iSelAllGiftItemsLen* sizeof(pstLog->GiftSelAllData[0]));
	//return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_role_ltr_gift_log(ZONESVRENV* pstEnv, Player *pstPlayer,  char *szGiftName, int iUseItemID,int iUseNum,GIFTITEM * pstGiftItem)
{
	OP_LOG_META stOpLog;

	OPLTRGIFT * pstLog = &stOpLog.OpLog.OpLtrGift;

	memset(pstLog, 0, sizeof(*pstLog));
	
	stOpLog.OpLogType = OP_LOG_LTR_GIFT;

	z_rolemem_warp(pstPlayer, &pstLog->PlayerInfo);
	STRNCPY(pstLog->PlayerLoginName, pstPlayer->szAccount, CCH(pstLog->PlayerLoginName));
	pstLog->PlayerUin = pstPlayer->stRoleData.Uin;
	pstLog->PlayerLevel = pstPlayer->stRoleData.Level;
	
	STRNCPY(pstLog->GiftName, szGiftName, CCH(pstLog->GiftName));
	pstLog->UseItemID = iUseItemID;
	pstLog->UseNum = iUseNum;
	memcpy(	&pstLog->GiftData,pstGiftItem,sizeof(pstLog->GiftData));
	//return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_map_role_num_log(ZONESVRENV* pstEnv)
{
	OP_LOG_META stOpLog;
	OPMAPROLENUM *pstLog = &stOpLog.OpLog.OpMapRoleNum;
	int iMap;
	TMEMBLOCK *pstMemBlock;
	int i,j;
	Player *pstPlayer;
	int iFind;

	memset(pstLog, 0,sizeof(*pstLog));
	stOpLog.OpLogType = OP_LOG_MAP_ROLE_NUM;
	
	for(i = 0; i < MAX_PLAYER; i++)
	{
		iFind = 0;
		pstMemBlock = TMEMPOOL_GET_PTR_BYPOS(pstEnv->pstPlayerPool, i);
		if (!pstMemBlock->fValid)
		{
			continue;
		}

		pstPlayer = (Player *)pstMemBlock->szData;
		if (PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
		{
			continue;
		}

		if (pstPlayer->stRoleData.Map < PWORLD_MAPID_BASE)
		{
			iMap = pstPlayer->stRoleData.Map;
		}
		else
		{
			iMap = GET_PWORLDID(pstPlayer->stRoleData.Map);
		}

		for (j=0; j<LOG_MAX_MAP; j++)
		{
			if (pstLog->Maps[j].MapID == iMap)
			{
				iFind = 1;
				break;
			}	
		}

		if (iFind)
		{
			pstLog->Maps[j].RoleNum ++;
		}
		else
		{
			if (pstLog->Count >= LOG_MAX_MAP)
			{
				continue;
			}
			
			pstLog->Maps[pstLog->Count].MapID = iMap;
			pstLog->Maps[pstLog->Count].RoleNum = 1;
			pstLog->Count ++;
		}
	}

	return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
}

int z_role_secondary_skill_oplog(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
	int iActType, SECONDARYSKILLINFO *pstInfo)
{
	OP_LOG_META stOpLog;
	OPROLESECONDARYSKILL *pstRoleSecSkill = &stOpLog.OpLog.RoleSecondarySkill;

	stOpLog.OpLogType = OP_LOG_SECONDARY_SKILL;
	pstRoleSecSkill->Uin = pstPlayer->stRoleData.Uin;
	pstRoleSecSkill->RoleID = pstPlayer->stRoleData.RoleID;
	strncpy(pstRoleSecSkill->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstRoleSecSkill->RoleName));
	STRNCPY( pstRoleSecSkill->AccountLoginName, pstPlayer->szAccount, CCH(pstRoleSecSkill->AccountLoginName) );
	pstRoleSecSkill->ActType = iActType; 
	pstRoleSecSkill->SkillType = pstInfo->SkillType; 
	pstRoleSecSkill->SkillLevel = pstInfo->SkillLevel;
	
	//return z_oplog_send(pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
	UNUSED(pstAppCtx);	//add by paraunused.pl
}
int z_role_secskill_recipe_oplog(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
	SECONDARYSKILLINFO *pstInfo, int iRecipeID)
{
	OP_LOG_META stOpLog;
	OPROLESECSKILLRECIPE *pstRoleRecipe = &stOpLog.OpLog.RoleSecSkillRecipe;

	stOpLog.OpLogType = OP_LOG_SECONDARY_SKILL_RECIPE;
	pstRoleRecipe->Uin = pstPlayer->stRoleData.Uin;
	pstRoleRecipe->RoleID = pstPlayer->stRoleData.RoleID;
	strncpy(pstRoleRecipe->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstRoleRecipe->RoleName));
	STRNCPY( pstRoleRecipe->AccountLoginName, pstPlayer->szAccount, CCH(pstRoleRecipe->AccountLoginName) );
	
	pstRoleRecipe->SkillType = pstInfo->SkillType; 
	pstRoleRecipe->SkillLevel = pstInfo->SkillLevel;
	pstRoleRecipe->RecipeID = iRecipeID; 
	
	//return z_oplog_send(pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int z_acc_login_oplog(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,int iUin, const char* pszAccName, int iFlag,Player* pstPlayer)
{
	OP_LOG_META stOpLog;
	OPACCLOGIN *pstAccLogin;
	OPACCLOGOUT *pstAccLogout;

	// login
	if (iFlag)
	{
		stOpLog.OpLogType = OP_LOG_ACC_LOGIN;
		pstAccLogin = &stOpLog.OpLog.OpAccLogin;
		memset(pstAccLogin,0,sizeof(*pstAccLogin));

		
		pstAccLogin->Uin = iUin;
		STRNCPY(pstAccLogin->AccountLoginName, pszAccName, sizeof(pstAccLogin->AccountLoginName));

		if (pstPlayer)
		{
			pstAccLogin->CltType = pstPlayer->iCltType;
			pstAccLogin->UinFlag = pstPlayer->UinFlag;
			STRNCPY(pstAccLogin->MacAddr, pstPlayer->aMacAdd, CCH(pstAccLogin->MacAddr));
			STRNCPY(pstAccLogin->IP, inet_ntoa(*(struct in_addr *)&pstPlayer->stConnectInfo.stExtraInfo.IPInfo.ip), CCH(pstAccLogin->IP));
		}
	}
	// logout
	else
	{
		stOpLog.OpLogType = OP_LOG_ACC_LOGOUT;
		pstAccLogout = &stOpLog.OpLog.OpAccLogout;
		pstAccLogout->Uin = iUin;
		STRNCPY(pstAccLogout->AccountLoginName, pszAccName, sizeof(pstAccLogout->AccountLoginName));
	}
	return z_oplog_send(pstAppCtx, pstEnv, &stOpLog);
}

int z_role_item_dec_oplog(ZONESVRENV* pstEnv,Player* pstPlayer, ITEMDEC *pstItemDec,int iOpType)
{
	OP_LOG_META stOpLog;
	OPROLEITEMDEC *pstRoleItemDec = &stOpLog.OpLog.OpRoleItemDec;

	stOpLog.OpLogType = OP_LOG_ROLE_ITEM_DEC;
	pstRoleItemDec->Uin = pstPlayer->stRoleData.Uin;
	pstRoleItemDec->RoleInfo.RoleID = pstPlayer->stRoleData.RoleID;
	strncpy(pstRoleItemDec->RoleInfo.RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstRoleItemDec->RoleInfo.RoleName));
	STRNCPY( pstRoleItemDec->Pass9Account, pstPlayer->szAccount, CCH(pstRoleItemDec->Pass9Account) );

	pstRoleItemDec->OpType = (char)iOpType;
	pstRoleItemDec->LogTime = 0;
	
	pstRoleItemDec->ItemDec = *pstItemDec;

	//return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_genius_oplog(ZONESVRENV* pstEnv,Player* pstPlayer, int iOp, int iMoney, int iBindMoney, int iExp, int iSub, int iType)
{
	OP_LOG_META stOpLog;
	OPGENUIS *pstOp = &stOpLog.OpLog.OpGenuis;

	stOpLog.OpLogType = OP_LOG_GENIUS;
	pstOp->Career = pstPlayer->stRoleData.Career;
	pstOp->BindMoney = iBindMoney;
	pstOp->Exp = iExp;
	pstOp->Level = pstPlayer->stRoleData.Level;
	pstOp->LogTime = 0;
	pstOp->Money = iMoney;
	pstOp->Op = iOp;
	pstOp->RoleInfo.RoleID = pstPlayer->stRoleData.RoleID;
	strncpy(pstOp->RoleInfo.RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstOp->RoleInfo.RoleName));
	pstOp->SubType = iSub;
	pstOp->Type = iType;
	pstOp->Uin = pstPlayer->stRoleData.Uin;
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int spar_item_op(ROLEITEM *pstSparItem, OPSPARDETAIL *pstSparDetail)
{
	pstSparDetail->SparID = pstSparItem->DefID;
	pstSparDetail->SparLevel = pstSparItem->InstVal1;
	pstSparDetail->SparExp = pstSparItem->InstVal3;
	
	return 0;
}

int z_spar_oplog(ZONESVRENV* pstEnv,Player* pstPlayer, int iOp, ROLEITEM *pstOldSparItem, 
								ROLEITEM *pstNewSparItem, MACHINNGEXPENDDATA *pstExpendData)
{
	OP_LOG_META stOpLog;
	OPSPAR*pstOp = &stOpLog.OpLog.OpSpar;

	stOpLog.OpLogType = OP_LOG_SPAR;

	memset(pstOp, 0, sizeof(*pstOp));
	pstOp->Uin = pstPlayer->stRoleData.Uin;
	pstOp->RoleInfo.RoleID = pstPlayer->stRoleData.RoleID;
	strncpy(pstOp->RoleInfo.RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstOp->RoleInfo.RoleName));
	STRNCPY(pstOp->Pass9Account, pstPlayer->szAccount, CCH(pstOp->Pass9Account) );
	pstOp->Op = iOp;
	if (pstOldSparItem)
	{
		spar_item_op(pstOldSparItem , &pstOp->OldSpar);
	}

	if (pstNewSparItem)
	{
		spar_item_op(pstNewSparItem , &pstOp->NewSpar);
	}

	if (pstExpendData)
	{
		pstOp->Expend = *pstExpendData;
	}
	
	pstOp->LogTime = 0;
		
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_clan_boon_oplog(ZONESVRENV* pstEnv,Player* pstPlayer,int iMoneyType, int iMoney)
{
	OP_LOG_META stOpLog;
	OPBOON*pstOp = &stOpLog.OpLog.OpBoon;
	stOpLog.OpLogType = OP_LOG_CLANBOON;

	memset(pstOp, 0, sizeof(*pstOp));
	pstOp->Uin = pstPlayer->stRoleData.Uin;
	pstOp->RoleInfo.RoleID = pstPlayer->stRoleData.RoleID;
	pstOp->ClanID = pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id;
	strncpy(pstOp->RoleInfo.RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstOp->RoleInfo.RoleName));
	STRNCPY(pstOp->Pass9Account, pstPlayer->szAccount, CCH(pstOp->Pass9Account) );

	pstOp->MoneyType = iMoneyType;
	pstOp->Money = iMoney;
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_clan_boon_cond_oplog(ZONESVRENV* pstEnv,Player* pstPlayer, CLAN*pstClan)
{
	OP_LOG_META stOpLog;
	OPBOONCOND*pstOp = &stOpLog.OpLog.OpBoonCond;
	stOpLog.OpLogType = OP_LOG_BOON_COND;

	memset(pstOp, 0, sizeof(*pstOp));
	pstOp->ClanID = pstClan->ClanProf.Id;
	strncpy(pstOp->ClanName, pstClan->ClanProf.Name, sizeof(pstOp->ClanName));
	pstOp->ClanLvl = pstClan->ClanProf.Level;
	pstOp->Gold = pstClan->ClanBoon.Cond.Gold;
	if (pstClan->ClanBoon.Cond.AveNum)
		pstOp->Ave = pstClan->ClanBoon.Cond.Ave/pstClan->ClanBoon.Cond.AveNum;
	pstOp->Contri = pstClan->ClanBoon.Cond.Contri;
	pstOp->Tech = pstClan->ClanBoon.Cond.Tech;
	pstOp->UpAve = CLAN_ONLINE_AVE;
	pstOp->UpContri = CLAN_CONTRI_NUM;
	pstOp->UpGold = CLAN_GOLD_NUM1;
	pstOp->UpTech = CLAN_TECH_NUM;
	
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);

}

int z_role_open_box(ZONESVRENV* pstEnv,Player* pstPlayer,int iType, int iDec, int iAdd, 
						GIFTITEM *pstDec, GIFTITEM *pstAdd)
{
	OP_LOG_META stOpLog;	
	OPOPENBOX *pstOp = &stOpLog.OpLog.OpOpenBox;
	int i;

	stOpLog.OpLogType = OP_LOG_OPEN_BOX;
	if (pstPlayer)
	{
		pstOp->Uin= pstPlayer->stRoleData.Uin;
		pstOp->RoleInfo.RoleID = pstPlayer->stRoleData.RoleID;
		strncpy(pstOp->RoleInfo.RoleName, pstPlayer->stRoleData.RoleName, 
				sizeof(pstOp->RoleInfo.RoleName));
	}

	pstOp->AddItemNum = iAdd;
	pstOp->DecItemNum = iDec;
	if (pstDec)
	{
		for (i=0; i<iDec; i++)
		{
			pstOp->DecItem[i] = *(pstDec+i);
		}
	}

	if (iAdd)
	{
		for (i=0; i<iAdd; i++)
		{
			pstOp->AddItem[i] = *(pstAdd+i);
		}
	}

	pstOp->OpType = iType;
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_store_buy_oplog(ZONESVRENV* pstEnv,Player* pstPlayer, ZoneStore *pstStore, 
									ROLEGRID *pstGrid, int iMoney, int iNum, int iType, int iPreMoney)
{
	OP_LOG_META stOpLog;	
	OPSTORE *pstOp = &stOpLog.OpLog.OpStore;
	stOpLog.OpLogType = OP_LOG_STORE;

	memset(pstOp, 0, sizeof(*pstOp));
	pstOp->Type = iType;

	if (iType == 3)  
	{
		if (pstPlayer)
		{
			pstOp->BuyRoleID = pstPlayer->stRoleData.RoleID;
			strncpy(pstOp->BuyName, pstPlayer->stRoleData.RoleName,  sizeof(pstOp->BuyName));
		}
	}
	
	pstOp->StoreOwnerRoleID = pstStore->stStore.RoleID;
	pstOp->Money = iMoney;
	pstOp->Num = iNum;
	pstOp->PreMoney = iPreMoney;
	pstOp->CurrMoney = pstStore->stStore.StoreInfo.Money;

	if (pstGrid)
	{
		pstOp->RoleGrid = *pstGrid;
	}
	
	strncpy(pstOp->StoreName, pstStore->stStore.StoreInfo.StoreName,  sizeof(pstOp->StoreName));
	strncpy(pstOp->StoreOwnerName, pstStore->stStore.Name,  sizeof(pstOp->StoreOwnerName));
	pstOp->StoreID = pstStore->stStore.StoreID;
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_script_add_exp_oplog(ZONESVRENV* pstEnv,Player* pstPlayer,unsigned long long iExp, const char *pszScript)
{
	OP_LOG_META stOpLog;
	OPSCRIPTADDEXP*pstOp = &stOpLog.OpLog.OpScriptAddExp;
	
	stOpLog.OpLogType = OP_LOG_SCRIPT_ADD_EXP;
	memset(pstOp, 0, sizeof(*pstOp));

	pstOp->Value = iExp;
	if (pstPlayer)
	{
		pstOp->Uin= pstPlayer->stRoleData.Uin;
		pstOp->RoleInfo.RoleID = pstPlayer->stRoleData.RoleID;
		strncpy(pstOp->RoleInfo.RoleName, pstPlayer->stRoleData.RoleName, 
				sizeof(pstOp->RoleInfo.RoleName));
		STRNCPY(pstOp->Pass9Account, pstPlayer->szAccount, CCH(pstOp->Pass9Account));
	}

	if (pszScript[0] != 0)
	{
		strncpy(pstOp->ScriptName, pszScript, 
				sizeof(pstOp->RoleInfo.RoleName));
	}
	
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}


int z_role_item_drop_oplog(ZONESVRENV* pstEnv,Player* pstPlayer,ROLEGRID* pstGrid)
{
	OP_LOG_META stOpLog;
	OPROLEITEMDROP *pstRoleItemDrop = &stOpLog.OpLog.OpRoleItemDrop;

	stOpLog.OpLogType = OP_LOG_ROLE_ITEM_DROP;
	pstRoleItemDrop->Uin = pstPlayer->stRoleData.Uin;
	pstRoleItemDrop->RoleInfo.RoleID = pstPlayer->stRoleData.RoleID;
	strncpy(pstRoleItemDrop->RoleInfo.RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstRoleItemDrop->RoleInfo.RoleName));
	STRNCPY(pstRoleItemDrop->Pass9Account, pstPlayer->szAccount, CCH(pstRoleItemDrop->Pass9Account) );

	pstRoleItemDrop->GridData = *pstGrid;
	pstRoleItemDrop->LogTime = 0;

	//return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_role_pworld_award_oplog(ZONESVRENV* pstEnv,Player* pstPlayer,int iPworldID, int iExp, int iMoney)
{
	OP_LOG_META stOpLog;
	OPROLEPWORLDAWARD *pstRolePworldAward = &stOpLog.OpLog.OpRolePworldAward;

	stOpLog.OpLogType = OP_LOG_ROLE_PWORLD_AWARD;
	pstRolePworldAward->EventLogWID = z_get_logwid(pstEnv);
	pstRolePworldAward->Uin = pstPlayer->stRoleData.Uin;
	pstRolePworldAward->RoleInfo.RoleID = pstPlayer->stRoleData.RoleID;
	strncpy(pstRolePworldAward->RoleInfo.RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstRolePworldAward->RoleInfo.RoleName));
	STRNCPY(pstRolePworldAward->LoginName, pstPlayer->szAccount, CCH(pstRolePworldAward->LoginName) );

	pstRolePworldAward->PworldID = iPworldID;
	pstRolePworldAward->Money = iMoney;
	pstRolePworldAward->Exp = iExp;

	//return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_role_pworld_card_oplog(ZONESVRENV* pstEnv,Player* pstPlayer, int iPworldID, int iExp, int iMoney, ROLEGRID* pstGrid)
{
	OP_LOG_META stOpLog;
	OPROLEPWORLDCARD *pstRolePworldCard = &stOpLog.OpLog.OpRolePworldCard;

	memset(&stOpLog, 0,sizeof(stOpLog));
	stOpLog.OpLogType = OP_LOG_ROLE_PWORLD_CARD;
	pstRolePworldCard->EventLogWID = z_get_logwid(pstEnv);
	pstRolePworldCard->Uin = pstPlayer->stRoleData.Uin;
	pstRolePworldCard->RoleInfo.RoleID = pstPlayer->stRoleData.RoleID;
	strncpy(pstRolePworldCard->RoleInfo.RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstRolePworldCard->RoleInfo.RoleName));
	STRNCPY(pstRolePworldCard->LoginName, pstPlayer->szAccount, CCH(pstRolePworldCard->LoginName) );

	pstRolePworldCard->PworldID = iPworldID;
	pstRolePworldCard->Money = iMoney;
	pstRolePworldCard->Exp = iExp;
	if(pstGrid)
	{
		pstRolePworldCard->Item = *pstGrid;
	}
	
	//return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_role_battle_award_oplog(ZONESVRENV* pstEnv,Player* pstPlayer, int iBattleID, OPROLEBATTLEAWARDINFO *pstAwardInfo)
{
	OP_LOG_META stOpLog;
	OPROLEBATTLEAWARD *pstRoleBattleAward = &stOpLog.OpLog.OpRoleBattleAward;

	memset(&stOpLog, 0,sizeof(stOpLog));
	stOpLog.OpLogType = OP_LOG_ROLE_BATTLE_AWARD;
	pstRoleBattleAward->EventLogWID = z_get_logwid(pstEnv);
	pstRoleBattleAward->Uin = pstPlayer->stRoleData.Uin;
	pstRoleBattleAward->RoleInfo.RoleID = pstPlayer->stRoleData.RoleID;
	strncpy(pstRoleBattleAward->RoleInfo.RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstRoleBattleAward->RoleInfo.RoleName));
	STRNCPY(pstRoleBattleAward->Pass9Account, pstPlayer->szAccount, CCH(pstRoleBattleAward->Pass9Account) );

	pstRoleBattleAward->BattleID = iBattleID;
	pstRoleBattleAward->Award = *pstAwardInfo;
	
	//return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_role_recipe_make_oplog(ZONESVRENV* pstEnv,Player* pstPlayer, 
	int iSkillType, int iSkillLevel, int iRecipeID, OPROLERECIPEMAKEINFO *pstMakeInfo)
{
	OP_LOG_META stOpLog;
	OPROLERECIPEMAKE *pstRoleRecipeMake = &stOpLog.OpLog.OpRoleRecipeMake;

	memset(&stOpLog, 0,sizeof(stOpLog));
	stOpLog.OpLogType = OP_LOG_ROLE_RECIPE_MAKE;
	pstRoleRecipeMake->Uin = pstPlayer->stRoleData.Uin;
	pstRoleRecipeMake->RoleInfo.RoleID = pstPlayer->stRoleData.RoleID;
	strncpy(pstRoleRecipeMake->RoleInfo.RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstRoleRecipeMake->RoleInfo.RoleName));
	STRNCPY(pstRoleRecipeMake->Pass9Account, pstPlayer->szAccount, CCH(pstRoleRecipeMake->Pass9Account) );

	pstRoleRecipeMake->SkillType = iSkillType;
	pstRoleRecipeMake->SkillLevel = iSkillLevel;
	pstRoleRecipeMake->RecipeID = iRecipeID;
	pstRoleRecipeMake->MakeInfo = *pstMakeInfo;
	
	//return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_role_pet_catch_oplog(ZONESVRENV* pstEnv,Player* pstPlayer, 
	int iUseItemID, int iTargetID, ITEMDEC *pstGetItem)
{
	OP_LOG_META stOpLog;
	OPROLEPETCATCH *pstRolePetCatch = &stOpLog.OpLog.OpRolePetCatch;

	memset(&stOpLog, 0,sizeof(stOpLog));
	stOpLog.OpLogType = OP_LOG_ROLE_PET_CATCH;
	pstRolePetCatch->Uin = pstPlayer->stRoleData.Uin;
	pstRolePetCatch->RoleInfo.RoleID = pstPlayer->stRoleData.RoleID;
	strncpy(pstRolePetCatch->RoleInfo.RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstRolePetCatch->RoleInfo.RoleName));
	STRNCPY(pstRolePetCatch->Pass9Account, pstPlayer->szAccount, CCH(pstRolePetCatch->Pass9Account) );

	pstRolePetCatch->CatchItemID = iUseItemID;
	pstRolePetCatch->TargetID  = iTargetID;
	if(pstGetItem)
	{
		pstRolePetCatch->GetItem = *pstGetItem;
	}
	
	//return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_map_trans_oplog(ZONESVRENV* pstEnv,Player* pstPlayer,OPMAPTRANSINFO* pstData)
{
	OP_LOG_META stOpLog;
	OPMAPTRANS *pstMapTrans = &stOpLog.OpLog.OpMapTrans;

	stOpLog.OpLogType = OP_LOG_MAP_TRANS;
	pstMapTrans->Uin = pstPlayer->stRoleData.Uin;
	pstMapTrans->RoleInfo.RoleID = pstPlayer->stRoleData.RoleID;
	strncpy(pstMapTrans->RoleInfo.RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstMapTrans->RoleInfo.RoleName));
	STRNCPY(pstMapTrans->Pass9Account, pstPlayer->szAccount, CCH(pstMapTrans->Pass9Account) );

	pstMapTrans->TransInfo = *pstData;
	pstMapTrans->LogTime = 0;

	//return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

void z_role_oplog_info_fill(OPROLEINFO *pstRoleInfo, Player* pstPlayer)
{
	if (pstPlayer)
	{
		pstRoleInfo->Uin = pstPlayer->stRoleData.Uin;
		pstRoleInfo->RoleID = pstPlayer->stRoleData.RoleID;
		STRNCPY(pstRoleInfo->LoginName, pstPlayer->szAccount, sizeof(pstRoleInfo->LoginName));
		STRNCPY(pstRoleInfo->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstRoleInfo->RoleName));
	}
	else
	{
		memset(pstRoleInfo,0, sizeof(*pstRoleInfo));
	}

}

int z_role_fengyin_arm_oplog(ZONESVRENV* pstEnv,Player* pstPlayer,Player* pstFengyin,
							int iType,int iUse,ROLEARM *pstArm)
{
	OP_LOG_META stOpLog;
	OPROLEARMFENGYIN *pstOpRoleArmFengyin = &stOpLog.OpLog.OpRoleArmFengyin;
	
	stOpLog.OpLogType = OP_LOG_FENGYIN;
	pstOpRoleArmFengyin->Use = iUse;
	pstOpRoleArmFengyin->Type = iType;
	z_role_oplog_info_fill(&pstOpRoleArmFengyin->RoleInfo, pstPlayer);
	z_role_oplog_info_fill(&pstOpRoleArmFengyin->FengyinInfo, pstFengyin);
	pstOpRoleArmFengyin->RoleArm = *pstArm;
	
	//return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_lucky_oplog(ZONESVRENV* pstEnv,Player* pstPlayer,int iOpType,long long llValue)
{
	OP_LOG_META stOpLog;
	OPLUCKY *pstLucky = &stOpLog.OpLog.OpLucky;

	stOpLog.OpLogType = OP_LOG_LUCKY;
	pstLucky->Uin = pstPlayer->stRoleData.Uin;
	pstLucky->RoleInfo.RoleID = pstPlayer->stRoleData.RoleID;
	strncpy(pstLucky->RoleInfo.RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstLucky->RoleInfo.RoleName));
	STRNCPY(pstLucky->Pass9Account, pstPlayer->szAccount, CCH(pstLucky->Pass9Account) );

	pstLucky->OpType = iOpType;
	pstLucky->Value = llValue;
	
	pstLucky->LogTime = 0;
	

	//return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_world_city_siege_oplog(ZONESVRENV* pstEnv, OPWORLDCITYINFO *pstPreWorldCityInfo)
{
	OP_LOG_META stOpLog;
	OPWORLDCITYSIEGE  *pstOpWorldCitySiege = &stOpLog.OpLog.OpWorldCitySiege;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;
	
	stOpLog.OpLogType = OP_LOG_WORLD_CITY_SIEGE;
	pstOpWorldCitySiege->Owner.ClanGID = pstWorldCity->CityCore.ClanGID;
	pstOpWorldCitySiege->Owner.ClanId = pstWorldCity->CityCore.ClanId;
	pstOpWorldCitySiege->Owner.ClanKeepNum = pstWorldCity->CityCore.ClanKeepNum;
	STRNCPY(pstOpWorldCitySiege->Owner.ClanName, 
		pstWorldCity->CityCore.ClanName, CCH(pstOpWorldCitySiege->Owner.ClanName));

	memset(&pstOpWorldCitySiege->PreInfo, 0, sizeof(pstOpWorldCitySiege->PreInfo));
	if(pstPreWorldCityInfo)
	{
		pstOpWorldCitySiege->PreInfo = *pstPreWorldCityInfo;
	}
	
	return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
}

int z_clan_active_oplog(ZONESVRENV* pstEnv,Player* pstPlayer,int iActiveID,int iStatus)
{
	OP_LOG_META stOpLog;
	OPCLANACTIVE *pstClanActive = &stOpLog.OpLog.OpClanActive;

	stOpLog.OpLogType = OP_LOG_CLAN_ACTIVE;

	memset(pstClanActive,0,sizeof(*pstClanActive));
	
	if (pstPlayer)
	{
		pstClanActive->Uin = pstPlayer->stRoleData.Uin;
		pstClanActive->RoleInfo.RoleID = pstPlayer->stRoleData.RoleID;
		strncpy(pstClanActive->RoleInfo.RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstClanActive->RoleInfo.RoleName));
		STRNCPY(pstClanActive->Pass9Account, pstPlayer->szAccount, CCH(pstClanActive->Pass9Account) );
	}

	pstClanActive->ActiveID = iActiveID;
	pstClanActive->Status = iStatus;
	
	pstClanActive->LogTime = 0;

	//return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_daily_goal_award_oplog(ZONESVRENV* pstEnv,Player* pstPlayer,int iAwardID)
{
	OP_LOG_META stOpLog;
	OPDAILYGOALAWARD *pstDailyGoalAward = &stOpLog.OpLog.OpDailyGoalAward;

	stOpLog.OpLogType = OP_LOG_DAILY_GOAL_AWARD;
	pstDailyGoalAward->Uin = pstPlayer->stRoleData.Uin;
	pstDailyGoalAward->RoleInfo.RoleID = pstPlayer->stRoleData.RoleID;
	strncpy(pstDailyGoalAward->RoleInfo.RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstDailyGoalAward->RoleInfo.RoleName));
	STRNCPY(pstDailyGoalAward->Pass9Account, pstPlayer->szAccount, CCH(pstDailyGoalAward->Pass9Account) );

	pstDailyGoalAward->AwardID = iAwardID;
	pstDailyGoalAward->LogTime = 0;

	//return z_oplog_send(pstEnv->pstAppCtx, pstEnv, &stOpLog);
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

int z_spanstrong_oplog(ZONESVRENV* pstEnv, Player* pstPlayer, int iStrongPoint, int iEnd)
{
	OP_LOG_META stOpLog;
	OPSPANSTRONG *pstOpLog = &stOpLog.OpLog.OpSpanStrong;
	WarFieldOL *pstWarOL = &pstPlayer->stOnline.stWarOL;
	RSTRONGONE *pstOne;
	SPANSTRONGDEF *pstDef;

	memset(pstOpLog, 0, sizeof(*pstOpLog));

	pstOne =  span_strong_one_find(pstEnv, iStrongPoint);
	pstDef = span_strong_def_find(pstEnv,  iStrongPoint);
		
	stOpLog.OpLogType = OP_LOG_SPANSTRONG;

	if (pstDef)
	{
		STRNCPY(pstOpLog->StrongName, pstDef->Name, sizeof(pstOpLog->StrongName));
	}

	pstOpLog->StrongID = iStrongPoint;
	
	if (pstWarOL->cCampIdx == iEnd)
	{
		pstOpLog->Win = 1;
	}

	if (pstOne)
	{
		if (pstWarOL->cCampIdx == 1)
		{
			pstOpLog->ResPoint = pstOne->Pworld.DefRes;
		}
		else
		{
			pstOpLog->ResPoint = pstOne->Pworld.AtkRes;
		}
	}

	pstOpLog->WorldID = role_world_id_get(pstPlayer);
	pstOpLog->Assit = pstPlayer->stRoleData.MiscInfo.MiscWar.Assit;
	pstOpLog->Career = pstPlayer->stRoleData.Career;
	pstOpLog->CountKill = pstPlayer->stRoleData.MiscInfo.MiscWar.Kill;
	pstOpLog->CurrContinueKill = pstWarOL->iContinueKill;
	pstOpLog->Die = pstPlayer->stRoleData.MiscInfo.MiscWar.Die;
	pstOpLog->EnterTime = pstPlayer->stRoleData.MiscInfo.MiscWar.FightTime;
	pstOpLog->MaxContinueKill = pstWarOL->iKillNum;
	pstOpLog->ResGet = pstWarOL->iOre;
	pstOpLog->RoleInfo.RoleID = pstPlayer->stRoleData.RoleID;
	STRNCPY(pstOpLog->RoleInfo.RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstOpLog->RoleInfo.RoleName));

	STRNCPY(pstOpLog->ClanName, pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Name, sizeof(pstOpLog->ClanName));
	
	return z_oplog_send( pstEnv->pstAppCtx, pstEnv, &stOpLog);
}

int z_home_oplog(ZONESVRENV* pstEnv,Player* pstPlayer,Player* pstPlayerB,unsigned int HomeID,
					  char Lvl,int UseMoney,int UseItem,unsigned int Time,char *Desc)
{
	OP_LOG_META stOpLog;
	OPHOME *pstOpHome = &stOpLog.OpLog.OpHome;
	
	memset(pstOpHome,0,sizeof(*pstOpHome));
	stOpLog.OpLogType = OP_LOG_HOME;

	if(pstPlayer)
	{
		pstOpHome->Uin = pstPlayer->stRoleData.Uin;
		pstOpHome->RoleID = pstPlayer->stRoleData.RoleID;
		STRNCPY(pstOpHome->RoleName,pstPlayer->stRoleData.RoleName,CCH(pstOpHome->RoleName));
	}

	if(pstPlayerB)
	{
		pstOpHome->RoleIDB = pstPlayerB->stRoleData.RoleID;
		STRNCPY(pstOpHome->RoleNameB,pstPlayerB->stRoleData.RoleName,CCH(pstOpHome->RoleNameB));
	}

	pstOpHome->HomeID = HomeID;
	pstOpHome->HomeLvl = Lvl;
	pstOpHome->OpTime = Time;
	pstOpHome->UseMoney = UseMoney;
	pstOpHome->UseItem = UseItem;
	STRNCPY(pstOpHome->OpDesc,Desc,CCH(pstOpHome->OpDesc));

	pstOpHome->LogTime = 0;
	
	return z_role_oplog_proxy_send(pstEnv->pstAppCtx, pstEnv, &stOpLog,pstPlayer);
}

