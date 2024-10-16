
#include "account_dyn_desc.h"
#include <stdio.h>
#include "tdr/tdr.h"
#include "cs_net.h"
#include "comm_misc.h"
#include "apps/tormapi/torm_protocol_api.h"
#include "role_db_meta.h"


struct DynEnv
{
	ACCOUNT_DYN_CONF stConf;
	
	LPTDRMETALIB pstMetaLib;
	TDRDBHANDLE hSrcHandle;			
	TDRDBHANDLE hDscHandle;
	TDRDBOBJECT stDBObj;
};

typedef struct DynEnv DYNENV;

extern unsigned char g_szMetalib_account_dyn[];

static int account_syn_conf(ACCOUNT_DYN_CONF* pstConf)
{
	TDRDATA stHost;
	LPTDRMETALIB pstMetaLib;
	LPTDRMETA pstMeta = NULL;

	stHost.iBuff = sizeof(*pstConf);
	stHost.pszBuff = (char *)pstConf;
	
	pstMetaLib = (LPTDRMETALIB)&g_szMetalib_account_dyn[0];
	pstMeta = tdr_get_meta_by_name(pstMetaLib, "account_dyn_conf");
	if(NULL == pstMeta)
	{
		return -1;
	}
	
	if( tdr_input_file(pstMeta, &stHost, "account_dyn.xml", 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME) < 0)
	{
		return -1;
	}

	return 0;
}

static int account_syn_init(DYNENV* pstEnv)
{
	TDRDBMS stDBMS;
	char szErrorMsg[2048]={0};
	ACCOUNT_DYN_CONF *pstConf;

	memset(pstEnv, 0, sizeof(*pstEnv));
	
	if (account_syn_conf(&pstEnv->stConf) < 0)
	{
		printf("account_dyn_conf fail\n");
		return -1;
	}
	pstConf = &pstEnv->stConf;
	
	if (0 > tdr_load_metalib(&pstEnv->pstMetaLib, pstEnv->stConf.szMetaFile))
	{
		printf("load meta lib role_db_meta fail\n");
		return -1;
	}

	memset(&stDBMS, 0, sizeof(stDBMS));
	STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
	STRNCPY(stDBMS.szDBMSCurDatabaseName, pstConf->szSrcDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
	STRNCPY(stDBMS.szDBMSConnectionInfo, pstConf->szSrcDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
	STRNCPY(stDBMS.szDBMSUser, pstConf->szSrcDBUser, sizeof(stDBMS.szDBMSUser));
	STRNCPY(stDBMS.szDBMSPassword, pstConf->szSrcDBPasswd, sizeof(stDBMS.szDBMSPassword));
	stDBMS.iReconnectOpt = 1;	
	
	if (0 > tdr_open_dbhanlde(&pstEnv->hSrcHandle, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
		return -1;
	}

	memset(&stDBMS, 0, sizeof(stDBMS));
	STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
	STRNCPY(stDBMS.szDBMSCurDatabaseName, pstConf->szDscDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
	STRNCPY(stDBMS.szDBMSConnectionInfo, pstConf->szDscDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
	STRNCPY(stDBMS.szDBMSUser, pstConf->szDscDBUser, sizeof(stDBMS.szDBMSUser));
	STRNCPY(stDBMS.szDBMSPassword, pstConf->szDscDBPasswd, sizeof(stDBMS.szDBMSPassword));
	stDBMS.iReconnectOpt = 1;	
	
	if (0 > tdr_open_dbhanlde(&pstEnv->hDscHandle, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
		return -1;
	}
	return 0;
}


int account_syn_in(DYNENV* pstEnv)
{
	ACCOUNT stAccount;
	ACCOUNT stAccountTmp;
	ACCOUNT *pstAcc;
	int iRet;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	TDRDBRESULTHANDLE hDBResult1 = 0;
	int iNum, i;
	char szSql[10240];
	TDRDBOBJECT stDBObj;
	int iCount = 0;
		
	stSql.iBuff = sizeof(szSql);
	stSql.pszBuff = &szSql[0];
	
	pstEnv->stDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "Account");
	pstEnv->stDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDBObj.pstMeta);
	pstEnv->stDBObj.iObjSize = sizeof(stAccount);
	pstEnv->stDBObj.pszObj = (char*)&stAccount;
	
	iRet = tdr_obj2sql(pstEnv->hSrcHandle, &stSql, TDR_DBOP_SELECT, &pstEnv->stDBObj,"where Uin != 0");
	if (0 != iRet)
	{
		printf("生成查询资源记录的SQL语句失败: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(pstEnv->hSrcHandle));
		return iRet;
	}

	if(tdr_query(&hDBResult, pstEnv->hSrcHandle, &stSql) != 0)
	{
		printf("执行SQL语句失败!\n");
		return -1;
	}
	

	iNum = tdr_num_rows(hDBResult);
	if(iNum> 0)
	{
		printf("src num=%d\n", iNum);		

		stSql.iBuff = sizeof(szSql);
		for(i=0; i<iNum; i++ )
		{
			iRet = tdr_fetch_row(hDBResult, &pstEnv->stDBObj);
			if (0 != iRet)
			{	
				printf("tdr_fetch_row error!!!\n");
				return -1;
			}
			pstAcc = (ACCOUNT*)pstEnv->stDBObj.pszObj;
			
			memcpy(&stAccountTmp, pstAcc, sizeof(stAccountTmp));

			memset(pstAcc, 0, sizeof(*pstAcc));

			STRNCPY(pstAcc->Pass9Account, stAccountTmp.Pass9Account, sizeof(pstAcc->Pass9Account));
			memcpy(pstAcc->PasswdHash, stAccountTmp.PasswdHash, sizeof(pstAcc->PasswdHash));
			STRNCPY(pstAcc->BirthDay, stAccountTmp.BirthDay, sizeof(pstAcc->BirthDay));
			pstAcc->Gender = stAccountTmp.Gender;
			STRNCPY(pstAcc->RegisterTs, stAccountTmp.RegisterTs, sizeof(pstAcc->RegisterTs));
			pstAcc->AccountCreateTime = stAccountTmp.AccountCreateTime;
			pstAcc->ActiveKeyStatus = stAccountTmp.ActiveKeyStatus;
			pstAcc->PasswdCard = stAccountTmp.PasswdCard;

			if (stAccountTmp.UinFlag &UIN_FLAG_PASSWD_CARD)
			{
				pstAcc->UinFlag |= UIN_FLAG_PASSWD_CARD;
			}

			if (stAccountTmp.UinFlag &UIN_FLAG_SAFE_EMAIL)
			{
				pstAcc->UinFlag |= UIN_FLAG_SAFE_EMAIL;
			}
			
			stDBObj.pstMeta = pstEnv->stDBObj.pstMeta;
			stDBObj.iVersion =pstEnv->stDBObj.iVersion;
			stDBObj.iObjSize = sizeof(ACCOUNT);
			stDBObj.pszObj = (char*)pstAcc;
			stSql.iBuff = sizeof(szSql);
			iRet = tdr_obj2sql(pstEnv->hDscHandle, &stSql, TDR_DBOP_INSERT, &stDBObj, NULL);
			if (iRet < 0)
			{
				printf("account %s 生成插入记录的SQL语句失败: %s dbms error:%s\n",
						pstAcc->Pass9Account, tdr_error_string(iRet), tdr_dbms_error(pstEnv->hDscHandle));
				return iRet;
			}
			//snprintf(stSql.pszBuff, stSql.iBuff,
			//		"insert into Account(Uin,Pass9Account,BirthDay,Gender,RegisterTs,AccountCreateTime,PasswdHash) VALUES (%d,%s,%s,%d,%s,%d,%s)",
			//		pstAcc->Uin,pstAcc->Pass9Account,pstAcc->BirthDay,
			//		pstAcc->Gender,pstAcc->RegisterTs,pstAcc->AccountCreateTime,pstAcc->PasswdHash);
			
			iRet = tdr_query(&hDBResult1, pstEnv->hDscHandle, &stSql);
			if (iRet < 0 )
			{
				int iDbmsErrNo = tdr_dbms_errno(pstEnv->hDscHandle);		
				//if (1022 != iDbmsErrNo && 1062 != iDbmsErrNo && 1169 != iDbmsErrNo)
				{
					printf("account %s insert fail: %s dbms error:%s\n",
							pstAcc->Pass9Account, tdr_error_string(iRet), tdr_dbms_error(pstEnv->hDscHandle));
					return -1;
				}
			
			}
			else
			{
				iCount ++;
			}
		}
	}

	printf("插入%d条记录\n", iCount);
	return 0;
}

int account_syn_end(DYNENV* pstEnv)
{
	if (pstEnv->hDscHandle)
	{
		tdr_close_dbhanlde(pstEnv->hDscHandle);
	}

	if (pstEnv->hSrcHandle)
	{
		tdr_close_dbhanlde(pstEnv->hSrcHandle);
	}

	return 0;
}

DYNENV stDynEnv;

int main()
{
	printf("...start...\n");
	if (account_syn_init(&stDynEnv) < 0)
	{
		goto end;
	}

	if ( account_syn_in(&stDynEnv) < 0)
	{
		goto end;
	}
	printf("...end...\n");
end:
	account_syn_end(&stDynEnv);
	return 0;
}
