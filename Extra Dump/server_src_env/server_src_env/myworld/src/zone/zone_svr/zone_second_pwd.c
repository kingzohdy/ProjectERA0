/*
**  @file $RCSfile: zone_second_pwd.c,v $
**  general description of this module
**  $Id: zone_second_pwd.c,v 1.13 2014/03/12 06:02:54 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2014/03/12 06:02:54 $
**  @version $Revision: 1.13 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/


#include "zone_second_pwd.h"
#include "zone_db.h"
#include "zone_player.h"
#include "zone_err.h"
#include "zone_clt.h"



//�������ö������뵽���ݿ���
static int player_set_secondpwd_to_db(ZONESVRENV* pstEnv, Player *pstPlayer,SECONDPWDCLT *pstSecondPwdClt)
{
	TDRDATA stMetaData;
	TDRDATA stNetData;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq = &pstPkg->Body.UpdateReq;
	ROLESECONDPWD stRoleSecPwd;
	
	z_init_torm_head(pstEnv->pstAppCtx, pstEnv, &pstPkg->Head, TORM_UPDATE_REQ,
		             "RoleSecondPwd", pstPlayer->stRoleData.Uin, NULL);
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	sprintf(&pstUpdateReq->OperInfo.WhereDef[0], "where RoleID=%llu", pstPlayer->stOnline.iRoleID);
	pstUpdateReq->DoInsert = 0;

	stRoleSecPwd.RoleID = pstPlayer->stOnline.iRoleID;
	stRoleSecPwd.SecondPwd[0] = 0;
	if (pstSecondPwdClt->SecondPwd.SecondPwdSet.PassWDSet[0] != 0)
	{
		STRNCPY(stRoleSecPwd.SecondPwd, pstSecondPwdClt->SecondPwd.SecondPwdSet.PassWDSet, sizeof(stRoleSecPwd.SecondPwd));
	}
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRoleSecPwd;
	stMetaData.iBuff = sizeof(stRoleSecPwd);
	
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, pstPkg, &stMetaData, 
		                      pstEnv->iRoleDBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}
	
	if(0 > z_torm_send(pstEnv->pstAppCtx, pstEnv, pstPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}
	return 0;
}


//���ö������룬�����µ����ݿ���
int player_secondpwd_set(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer,SECONDPWDCLT *pstSecondPwdClt)
{
	if(0 == pstSecondPwdClt->SecondPwd.SecondPwdSet.PassWDSet[0])
	{
			//��ʾ"���벻��Ϊ��"
			player_second_pwd_info_to_clt(pstEnv,pstPlayer,SECOND_PWD_SET_SVR,SECOND_PWD_NO_NULL);
			return -1;
	}
	else
	{
		if(player_set_secondpwd_to_db(pstEnv,pstPlayer,pstSecondPwdClt) < 0)
		{
			//����ʧ��
			player_second_pwd_info_to_clt(pstEnv,pstPlayer,SECOND_PWD_SET_SVR,SECOND_PWD_SET_ERR);
			return -1;
		}
		
		//���óɹ��󣬵����ɹ��Ի���
		player_second_pwd_info_to_clt(pstEnv,pstPlayer,SECOND_PWD_SET_SUCC_SVR,SECOND_PWD_SUCC);

		CSROLELOGINREQ stRoleLoginReq;
		stRoleLoginReq.RoleID = pstPlayer->stOnline.iRoleID;
		
		if(z_db_role_select(pstAppCtx, pstEnv, pstPlayer, &stRoleLoginReq)< 0)
		{
			return -1;
		}
	}
	return 0;
}


//����������룬�ж��Ƿ���ȷ
int player_secondpwd_input(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer,SECONDPWDCLT *pstSecondPwdClt)
{
	int iLevel = 0;
	
	if(0 == pstSecondPwdClt->SecondPwd.SecondPwdInput.PassWDInput[0])
	{
		//��ʾ"���벻��Ϊ��"
		player_second_pwd_info_to_clt(pstEnv,pstPlayer,SECOND_PWD_INPUT_SVR,SECOND_PWD_NO_NULL);
		return -1;
	}
	else
	{
		if(0 != strncmp(pstSecondPwdClt->SecondPwd.SecondPwdInput.PassWDInput, pstPlayer->stOnline.cSecondPwd, strlen(pstSecondPwdClt->SecondPwd.SecondPwdInput.PassWDInput)))
		{
			//�����������
			player_second_pwd_info_to_clt(pstEnv,pstPlayer,SECOND_PWD_INPUT_SVR,SECOND_PWD_ERR);
			return -1;
		}
		else
		{
			//����������ȷ
			player_second_pwd_info_to_clt(pstEnv,pstPlayer,0,SECOND_PWD_SUCC);

			if(1 == pstPlayer->stOnline.iSecondPwdCmd)//��¼
			{
				CSROLELOGINREQ stRoleLoginReq;
				stRoleLoginReq.RoleID = pstPlayer->stOnline.iRoleID;
				
				if(z_db_role_select(pstAppCtx, pstEnv, pstPlayer, &stRoleLoginReq)< 0)
				{
					return -1;
				}
			}
			else if(2 == pstPlayer->stOnline.iSecondPwdCmd)//ɾ����ɫ
			{

				CSROLEDELREQ stRoleDelReq;
				stRoleDelReq.RoleID = pstPlayer->stOnline.iRoleID;
				
				iLevel = player_role_del_level(pstEnv,stRoleDelReq.RoleID,pstPlayer);
				if (iLevel <= 0) return -1;

				// ����Ҫɾ����ʱ��Ÿ��½��
				if (iLevel < DELETE_ROLE_LEVEL_LIMIT)
				{
					return z_db_get_roledetail_req(pstAppCtx, pstEnv, pstPlayer, &stRoleDelReq);
				}
				else
				{
					return z_role_del_respite(pstEnv, pstPlayer, &stRoleDelReq);
				}
			}
			else
			{
				return -1;
			}
		}
	}
	return 0;
}


//svr �յ�SECOND_PWD_CLT ���͵��øú���
int player_secondpwd(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	SECONDPWDCLT *pstSecondPwdClt = &pstCsPkg->Body.SecondPwdClt;
	UNUSED(pstFrameHead);

	if (NULL == pstPlayer)
	{
		return -1;
	}
	
	if(SECOND_PWD_SET == pstSecondPwdClt->Cmd)
	{
		return player_secondpwd_set(pstAppCtx,pstEnv,pstPlayer,pstSecondPwdClt);
	}
	else if(SECOND_PWD_INPUT == pstSecondPwdClt->Cmd)	
	{
		return player_secondpwd_input(pstAppCtx,pstEnv,pstPlayer,pstSecondPwdClt);
	}
	else
	{
		return -1;
	}

	return 0;
}


//svr  to client  ������Ϣ
int player_second_pwd_info_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer, int iFlag, int Result)
{
	CSPKG stPak;
	SECONDPWDSVR *pstSecondPwdSvr = &stPak.Body.SecondPwdSvr;
	
	pstSecondPwdSvr->Flag = iFlag;
	pstSecondPwdSvr->Result = Result;
		
	Z_CSHEAD_INIT(&stPak.Head,SECOND_PWD_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPak);
	return 0;
}

//svr  to client  �����Ի���:���û�������
int player_send_to_clt_popup_dialog(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer,tdr_ulonglong iResId)
{
	int iflag;
	int iLevel;
	
	if(pstPlayer->stOnline.iRoleID != iResId)
	{
		return -1;
	}
	
	switch(pstPlayer->stOnline.iSecondPwdCmd)
	{
		case 1:  //��¼����
		{
			if(0 != pstPlayer->stOnline.cSecondPwd[0])
			{
				//������������������
				iflag = SECOND_PWD_INPUT_SVR;
			}
			else
			{
				//�������ö����������
				iflag = SECOND_PWD_SET_SVR;	
			}
		
			player_second_pwd_info_to_clt(pstEnv,pstPlayer,iflag,SECOND_PWD_SUCC);
			break;
		}
		case 2:  // ɾ������
		{
			if(0 != pstPlayer->stOnline.cSecondPwd[0])
			{
				//������������������	
				player_second_pwd_info_to_clt(pstEnv,pstPlayer,SECOND_PWD_INPUT_SVR,SECOND_PWD_SUCC);
			}	
			else
			{
				CSROLEDELREQ stRoleDelReq;
				stRoleDelReq.RoleID = pstPlayer->stOnline.iRoleID;
				
				iLevel = player_role_del_level(pstEnv,stRoleDelReq.RoleID,pstPlayer);
				if (iLevel <= 0) return -1;

				// ����Ҫɾ����ʱ��Ÿ��½��
				if (iLevel < DELETE_ROLE_LEVEL_LIMIT)
				{
					return z_db_get_roledetail_req(pstAppCtx, pstEnv, pstPlayer, &stRoleDelReq);
				}
				else
				{
					return z_role_del_respite(pstEnv, pstPlayer, &stRoleDelReq);
				}
			
			}
			break;
		}
		default:
			return -1;
			break;
	}

	return 0;

}
