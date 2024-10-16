
#include "wid_check_desc.h"
#include <stdio.h>
#include "tdr/tdr.h"
#include "cs_net.h"
#include "comm_misc.h"
#include "apps/tormapi/torm_protocol_api.h"
#include "comm/shtable.h"

#define WID_OP_ALL 0
#define WID_OP_ITEM 1
#define WID_OP_PET 2
#define WID_OP_RIDE 3
#define WID_OP_MAIL 4

#define MAX_WID 100000

struct widcheckEnv
{
	WID_CHECK_CONF stConf;
	LPTDRMETALIB pstMetaLib;
	TDRDBHANDLE hDBHandle;			
	TDRDBHANDLE hDBHandleDel;		
	TDRDBHANDLE hDBHandleUp;	
	FILE *fp;
	int iNumRows;
	TDRDBOBJECT stDBObj;
	TDRDBRESULTHANDLE hDBResult;
	LPSHTABLE pstWid;
};

typedef struct widcheckEnv WIDCHECKENV;

extern unsigned char g_szMetalib_wid_check[];

int wid_check_same(WIDCHECKENV *pstEnv, unsigned long long ullWid);

static int widcheckcmp(const void * pv1 , const void *pv2)
{
	unsigned long long  *p1 = (unsigned long long *)pv1;
	unsigned long long  *p2 = (unsigned long long *)pv2;

	if (*p1  >  *p2)
		return 1;
	if (*p1  <  *p2)
		return -1;
		
	return 0;
}

static unsigned int widcheckcode(const void* pvData)
{
	unsigned long long  *p1 = (unsigned long long *)pvData;
	
	return *p1;
}

int wid_get(WIDCHECKENV *pstEnv)
{
	char szWid[24];
	int iLen;
	unsigned long long *p;
	unsigned long long u;
	
	memset(szWid, 0, 24);
	while ( fgets(szWid, 24, pstEnv->fp) != NULL)
	{		
		p = NULL;
		iLen = strlen(szWid);
		if (iLen >= 1)
		{
			if (	szWid[iLen-1]=='\n' ||
				szWid[iLen-1]=='\r')
			{
				szWid[iLen-1] = '\0';
			}
		}

		if (iLen >= 2)
		{
			if (	szWid[iLen-2]=='\n' ||
				szWid[iLen-2]=='\r')
			{
				szWid[iLen-2] = '\0';
			}
		}

		u = atoull(szWid);
		if (u == 0)
		{
			printf("atoull fail\n");
			return -1;
		}
		
		p = sht_insert_unique(pstEnv->pstWid ,&u, widcheckcmp, widcheckcode);
		if (p)
			*p = u;
		else
			return -1;
			
		memset(szWid, 0, 24);
	}
	return 0;
}

int wid_init(WIDCHECKENV *pstEnv)
{
	memset(pstEnv, 0, sizeof(*pstEnv));

	TDRDATA stHost;
	LPTDRMETALIB pstMetaLib;
	LPTDRMETA pstMeta = NULL;

	stHost.iBuff = sizeof(pstEnv->stConf);
	stHost.pszBuff = (char *)&pstEnv->stConf;
	
	pstMetaLib = (LPTDRMETALIB)&g_szMetalib_wid_check[0];
	pstMeta = tdr_get_meta_by_name(pstMetaLib, "wid_check_conf");
	if(NULL == pstMeta)
	{
		return -1;
	}
	
	if( tdr_input_file(pstMeta, &stHost, "wid_check.xml", 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME) < 0)
	{
		return -1;
	}

	TDRDBMS stDBMS;
	char szErrorMsg[2048]={0};
	WID_CHECK_CONF *pstConf = &pstEnv->stConf;
	
	memset(&stDBMS, 0, sizeof(stDBMS));
	STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
	STRNCPY(stDBMS.szDBMSCurDatabaseName, pstConf->szDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
	STRNCPY(stDBMS.szDBMSConnectionInfo, pstConf->szDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
	STRNCPY(stDBMS.szDBMSUser, pstConf->szDBUser, sizeof(stDBMS.szDBMSUser));
	STRNCPY(stDBMS.szDBMSPassword, pstConf->szDBPasswd, sizeof(stDBMS.szDBMSPassword));
	stDBMS.iReconnectOpt = 1;	
	
	if (0 > tdr_open_dbhanlde(&pstEnv->hDBHandle, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
		return -1;
	}

	if (0 > tdr_open_dbhanlde(&pstEnv->hDBHandleDel, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
		return -1;
	}

	if (0 > tdr_open_dbhanlde(&pstEnv->hDBHandleUp, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
		return -1;
	}

	pstEnv->fp = fopen(pstEnv->stConf.szFileName,"r");
	if (pstEnv->fp == NULL)
	{
		printf("fopen error!\n");
		return -1;
	}

	if (0 > tdr_load_metalib(&pstEnv->pstMetaLib, pstEnv->stConf.szMetaFile))
	{
		printf("load meta lib role_db_meta fail\n");
		return -1;
	}

	pstEnv->pstWid = sht_create(MAX_WID*3, MAX_WID, sizeof(long long), NULL);
	if (NULL == pstEnv->pstWid)
	{
		printf("init_sht error \n");
		return -1;
	}

	if ( wid_get(pstEnv) < 0)
	{
		printf("wid_get fail\n");
		return -1;
	}
	return 0;
}


int wid_check_mail_in(WIDCHECKENV *pstEnv, MAILENTRY *pstMail)
{
	int i;
	int iChg=0;

	for (i=pstMail->Mail.Detail.ItemNum-1; i>=0; i--)
	{
		if (wid_check_same(pstEnv,pstMail->Mail.Detail.Items[i].GridData.RoleItem.WID))
		{
			pstMail->Mail.Detail.ItemNum --;
			if (i != pstMail->Mail.Detail.ItemNum)
			{
				pstMail->Mail.Detail.Items[i] = pstMail->Mail.Detail.Items[pstMail->Mail.Detail.ItemNum];
			}
	
			iChg = 1;
		}
	}
	return iChg;
}

int wid_check_same(WIDCHECKENV *pstEnv, unsigned long long ullWid)
{
	unsigned long long *p = NULL;
	
	p = sht_find(pstEnv->pstWid, &ullWid, widcheckcmp, widcheckcode);
	if (p)
		return 1;
			
	return 0;
}

int wid_check_item(WIDCHECKENV *pstEnv, ROLEDATA *pstRoleData)
{
	ROLEWEAR *pstWear;
	SUNDRIESPACKAGE *pstSundpack;
	STASHPACKAGE *pstStashpack;
	int j;
	int iChg=0;
	
	pstWear = &pstRoleData->Wear;
	for (j=pstWear->ArmNum-1; j>=0; j--)
	{
		if (wid_check_same(pstEnv, pstWear->RoleArms[j].WID))
		{
			pstWear->ArmNum --;
			if (j != pstWear->ArmNum)
			{
				pstWear->RoleArms[j] = pstWear->RoleArms[pstWear->ArmNum];
			}

			iChg = 1;
		}
	}
	
	pstStashpack = &pstRoleData->Package.StashPackage;
	for (j=pstStashpack->Num-1; j>=0; j--)
	{
		if (wid_check_same(pstEnv, pstStashpack->RoleGrids[j].GridData.RoleItem.WID))
		{
			pstStashpack->Num --;
			if (j != pstStashpack->Num)
			{
				pstStashpack->RoleGrids[j] = pstStashpack->RoleGrids[pstStashpack->Num];
			}
			iChg = 1;
		}
	}
	
	pstSundpack = &pstRoleData->Package.SundriesPackage;
	for (j=pstSundpack->Num-1; j>=0; j--)
	{
		if (wid_check_same(pstEnv, pstSundpack->RoleGrids[j].GridData.RoleItem.WID))
		{
			pstSundpack->Num --;
			if (j != pstSundpack->Num)
			{
				pstSundpack->RoleGrids[j] = pstSundpack->RoleGrids[pstSundpack->Num];
			}
			iChg = 1;
		}
	}
	
	return iChg;
}

int wid_check_ride(WIDCHECKENV *pstEnv, ROLEDATA *pstRoleData)
{
	
	return 0;
}

int wid_check_mail(WIDCHECKENV *pstEnv)
{
	TDRDBOBJECT stDBObj;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	TDRDBRESULTHANDLE hDBResult1 = 0;
	int iRet = 0;
	char szSql[512];
	char szSql1[102400];
	MAILENTRY stMail;
	MAILENTRY *pstMail;
	int iNumRows;
	int i;
	
	stDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "MailEntry");
	stDBObj.iVersion = tdr_get_meta_current_version(stDBObj.pstMeta);
	stDBObj.iObjSize = sizeof(stMail);
	stDBObj.pszObj = (char*)&stMail;

	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM MailEntry");
	stSql.iBuff = strlen(stSql.pszBuff);
	
	iRet = tdr_query(&hDBResult, pstEnv->hDBHandle, &stSql);
	if (iRet < 0)
	{
		printf("wid_check_mail:tdr_query fail\n");
		return -1;	
	}
	
	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows < 1)
	{
		printf("wid_check_mail:tdr_num_rows fail\n");
		return -1;
	}

	for(i=0; i<iNumRows; i++ )
	{
		iRet = tdr_fetch_row(hDBResult, &stDBObj);
		if (0 != iRet)
		{	
			printf("wid_check_mail:tdr_num_rows:%d fail\n", i);
			return -1;
		}

		pstMail = (MAILENTRY*)stDBObj.pszObj;
		if (wid_check_mail_in(pstEnv, pstMail))
		{
			/*stSql.pszBuff = &szSql[0];
			snprintf(szSql, sizeof(szSql),"DELETE FROM MailEntry WHERE WID=%llu%c", pstMail->Mail.Head.WID,'\0');
			stSql.iBuff = strlen(stSql.pszBuff);
			hDBResult1 = 0;
			tdr_query(&hDBResult1, pstEnv->hDBHandleDel, &stSql);	*/

			stSql.iBuff = sizeof(szSql1);
			stSql.pszBuff = &szSql1[0];
			iRet = tdr_obj2sql(pstEnv->hDBHandleUp, &stSql, TDR_DBOP_UPDATE, &stDBObj, NULL);
			if (0 != iRet)
			{
				printf("生成插入资源记录的SQL语句失败: %s dbms error:%s\n",
					tdr_error_string(iRet), tdr_dbms_error(pstEnv->hDBHandleUp));
				return -1;
			}

			tdr_query(&hDBResult1, pstEnv->hDBHandleUp, &stSql);	
		}
	}

	if (hDBResult)
	{
		tdr_free_dbresult(&hDBResult);
	}

	if (hDBResult1)
	{
		tdr_free_dbresult(&hDBResult1);
	}
	return 0;
}

int wid_check_pet(WIDCHECKENV *pstEnv, ROLEDATA *pstRoleData)
{
	PETPACKAGE *pstPetPack = &pstRoleData->Package.PetPackage;
	int i;
	int iChg=0;

	for (i=pstPetPack->Num-1; i>=0; i--)
	{
		if (wid_check_same(pstEnv, pstPetPack->RolePets[i].WID))
		{
			pstPetPack->Num --;
			if (i!= pstPetPack->Num)
			{
				 pstPetPack->RolePets[i] =  pstPetPack->RolePets[pstPetPack->Num];
			}
			iChg = 1;
		}
	}
	return iChg;
}

int wid_check_roledate(WIDCHECKENV *pstEnv, int iOp)
{
	int i;
	ROLEDATA *pstRoleData;
	ROLEDATA stRoleData;
	TDRDATA stSql;
	int iRet = 0;
	char szSql[512];
	int iChg=0;
	char szSql1[102400];
	TDRDBRESULTHANDLE hDBResult = 0;
	
	pstEnv->stDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "RoleData");
	pstEnv->stDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDBObj.pstMeta);
	pstEnv->stDBObj.iObjSize = sizeof(stRoleData);
	pstEnv->stDBObj.pszObj = (char*)&stRoleData;

	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM RoleData");
	stSql.iBuff = strlen(stSql.pszBuff);
	
	iRet = tdr_query(&pstEnv->hDBResult, pstEnv->hDBHandle, &stSql);
	if (iRet < 0)
	{
		printf("从数据库表中读取数据记录失败:%s, dbms  error:%s\n",					
						tdr_error_string(iRet), tdr_dbms_error(pstEnv->hDBHandle));

		printf("tdr_query fail\n");
		return -1;	
	}
	
	pstEnv->iNumRows = tdr_num_rows(pstEnv->hDBResult);
	if(pstEnv->iNumRows < 1)
	{
		printf("tdr_num_rows fail\n");
		return -1;
	}
	
	for(i=0; i<pstEnv->iNumRows; i++ )
	{
		iRet = tdr_fetch_row(pstEnv->hDBResult, &pstEnv->stDBObj);
		if (0 != iRet)
		{	
			printf("tdr_num_rows:%d fail\n", i);
			return -1;
		}

		pstRoleData = (ROLEDATA*)pstEnv->stDBObj.pszObj;
		if (iOp == WID_OP_ITEM)
		{
			iChg = wid_check_item(pstEnv, pstRoleData);
		}
		else if (iOp == WID_OP_RIDE)
		{

		}
		else if (iOp == WID_OP_PET)
		{
			iChg = wid_check_pet(pstEnv,pstRoleData);	
		}
		else if (iOp == WID_OP_ALL)
		{
			iChg = wid_check_item(pstEnv, pstRoleData)+wid_check_pet(pstEnv,pstRoleData);
		}

		if (iChg)
		{
			stSql.iBuff = sizeof(szSql1);
			stSql.pszBuff = &szSql1[0];
			iRet = tdr_obj2sql(pstEnv->hDBHandleUp, &stSql, TDR_DBOP_UPDATE, &pstEnv->stDBObj, NULL);
			if (0 != iRet)
			{
				printf("生成插入资源记录的SQL语句失败: %s dbms error:%s\n",
					tdr_error_string(iRet), tdr_dbms_error(pstEnv->hDBHandleUp));
				return -1;
			}

			tdr_query(&hDBResult, pstEnv->hDBHandleUp, &stSql);	
		}
	}

	if (hDBResult)
	{
		tdr_free_dbresult(&hDBResult);
	}
	return 0;
}

int wid_end(WIDCHECKENV *pstEnv)
{
	if (pstEnv->hDBHandle)
	{
		tdr_close_dbhanlde(pstEnv->hDBHandle);
	}

	if (pstEnv->hDBHandleDel)
	{
		tdr_close_dbhanlde(pstEnv->hDBHandleDel);
	}

	if (pstEnv->hDBHandleUp)
	{
		tdr_close_dbhanlde(pstEnv->hDBHandleUp);
	}

	if (pstEnv->hDBResult)
	{
		tdr_free_dbresult(&pstEnv->hDBResult);
	}

	if (pstEnv->pstWid)
	{
		sht_destroy(&pstEnv->pstWid);
	}
	
	if (pstEnv->fp)
	{
		fclose(pstEnv->fp);
	}
	return 0;
}

int main(int iargc, char* argv[])
{
	int iOp;
	WIDCHECKENV stEnv;
	
	if (iargc != 2)
	{
		return -1;
	}

	iOp = atoi(argv[1]);
	if (iOp < 0)
	{
		return -1;
	}

	if ( wid_init(&stEnv) < 0)
		goto end;


	if (iOp == WID_OP_ALL)
	{
		if (wid_check_roledate(&stEnv, WID_OP_ALL) < 0)
		{
			goto end;
		}

		if (wid_check_mail(&stEnv) < 0)
		{
			goto end;
		}
	
	}
	else if (	iOp == WID_OP_ITEM ||
			iOp == WID_OP_PET ||
			iOp == WID_OP_RIDE)
	{
		if (wid_check_roledate(&stEnv, iOp) < 0)
		{
			goto end;
		}
	}
	else if (iOp == WID_OP_MAIL)
	{
		if (wid_check_mail(&stEnv) < 0)
		{
			goto end;
		}
	}
	else
	{
		goto end;
	}

	printf("exe succ!!\n");
end:
	wid_end(&stEnv);
	return 0;
}
