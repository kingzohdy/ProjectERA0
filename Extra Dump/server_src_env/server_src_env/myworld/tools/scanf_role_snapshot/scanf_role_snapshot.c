
#include <stdio.h>
#include "scanf_role_snapshot_desc.h"
#include "tdr/tdr.h"
#include "cs_net.h"
#include "comm_misc.h"
#include "apps/tormapi/torm_protocol_api.h"
#include "role_db_meta.h"


extern unsigned char g_szMetalib_scanf_role_snapshot[];


int scanf_init_dbhandle(LPTDRMETALIB* pstResLib,TDRDBHANDLE *hResDBHandle)
{
	TDRDATA stHost;
	TDRDBMS stDBMS;
	char szErrorMsg[2048]={0};
	SCANF_ROLE_SNAPSHOTCONF stConf;
	LPTDRMETALIB pstLib;
	LPTDRMETA pstMeta = NULL;

	stHost.iBuff = sizeof(stConf);
	stHost.pszBuff = (char *)&stConf;
	
	pstLib = (LPTDRMETALIB)&g_szMetalib_scanf_role_snapshot[0];
	pstMeta = tdr_get_meta_by_name(pstLib, "scanf_role_snapshotconf");
	if(NULL == pstMeta)
	{
		return -1;
	}
	
	if( tdr_input_file(pstMeta, &stHost, "scanf_role_snapshot.xml", 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME) < 0)
	{
		return -1;
	}
	
	if (0 > tdr_load_metalib(pstResLib, "../../cfg/db_meta/role_db_meta.tdr") && 
		0 > tdr_load_metalib(pstResLib, "role_db_meta.tdr"))
	{
		printf("load meta lib resdb_meta fail\n");
		return -1;
	}

	memset(&stDBMS, 0, sizeof(stDBMS));
	STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
	STRNCPY(stDBMS.szDBMSCurDatabaseName, stConf.szRoleDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
	STRNCPY(stDBMS.szDBMSConnectionInfo, stConf.szRoleDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
	STRNCPY(stDBMS.szDBMSUser, stConf.szRoleDBUser, sizeof(stDBMS.szDBMSUser));
	STRNCPY(stDBMS.szDBMSPassword, stConf.szRoleDBPasswd, sizeof(stDBMS.szDBMSPassword));
	stDBMS.iReconnectOpt = 1;	
	
	if (0 > tdr_open_dbhanlde(hResDBHandle, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s", szErrorMsg);
		return -1;
	}
	return 0;

}


int select_log_db(TDRDBHANDLE hDBHandle, TDRDBOBJECT *pstDBObj, LOGROLEDATA *pstLogRoledata)
{
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	char stWhere[256];
	int iNumRows = 0;
	char szSql[102400];
	TDRDATA stHost;
	
	stSql.iBuff = sizeof(szSql);
	stSql.pszBuff = &szSql[0];

	sprintf(stWhere," Limit 1");
	if (tdr_obj2sql(hDBHandle, &stSql, TDR_DBOP_SELECT, pstDBObj, stWhere) != 0)
	{
		printf("生成查询资源记录的SQL语句失败\n");
		return -1;
	}

	if(tdr_query(&hDBResult, hDBHandle, &stSql) != 0)
	{
		printf("执行SQL语句失败!\n");
		return -1;
	}
	
	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows != 1)
	{
		return -1;
	}
	
	
	iRet = tdr_fetch_row(hDBResult, pstDBObj);
	if (0 != iRet)
	{			
		if ((TDR_ERR_GET_ERROR_CODE(iRet) != TDR_ERROR_DB_NO_RECORD_IN_RESULTSET) ||	
			(0 != tdr_dbms_errno(hDBResult)))			
		{				
			printf("从数据库表中读取数据记录失败:%s, dbms  error:%s\n",					
			tdr_error_string(iRet), tdr_dbms_error(hDBResult));
		}
		
		return -1;
	}

	

	if(pstLogRoledata)
	{	
		LOGROLESNAPSHOT *pstRoleSnapShot;
		
		pstRoleSnapShot = (LOGROLESNAPSHOT *)pstDBObj->pszObj;
		memcpy(pstLogRoledata, &pstRoleSnapShot->LogData.LogRoleData, sizeof(*pstLogRoledata));
	}
	

	stHost.pszBuff = (char *)pstDBObj->pszObj;
	stHost.iBuff = pstDBObj->iObjSize;
	tdr_fprintf(pstDBObj->pstMeta,stdout, &stHost, 0);
	
	
	
	return 0;
}


int main(int argc, char* argv[])
{
	LPTDRMETALIB pstResLib;
	TDRDBHANDLE hResDBHandle;
	TDRDBOBJECT stDBObj;
	LOGROLESNAPSHOT stSanpshot;
	LOGROLEDATA stLogRoledata;
	
	if (scanf_init_dbhandle(&pstResLib,&hResDBHandle) < 0)
	{
		printf("scanf_init_dbhandle error!\n");
		return -1;
	}

	memset (&stDBObj, 0, sizeof(stDBObj));
	stDBObj.pstMeta = tdr_get_meta_by_name(pstResLib, "LogRoleSnapshot");
	if (!stDBObj.pstMeta)
	{
		printf("tdr_get_meta_by_name error!\n");
		return -1;
	}
	
	stDBObj.iVersion = tdr_get_meta_current_version(stDBObj.pstMeta);
	stDBObj.iObjSize = sizeof(stSanpshot);
	stDBObj.pszObj = (char*)&stSanpshot;

	select_log_db(hResDBHandle, &stDBObj, &stLogRoledata);


	tdr_free_dbresult(&hResDBHandle);
	return 0;
}
