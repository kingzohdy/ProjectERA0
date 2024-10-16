/*
**  @file RCSfile
**  general description of this module
**  Id
**  @author Author : lzk
**  @date Date
**  @version Revision
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "tdr/tdr.h"
#include <stdio.h>
#include "global_var_svr_conf_desc.h"
#include "cs_net.h"
#include "comm_misc.h"
#include "apps/tormapi/torm_protocol_api.h"
#include "role_db_meta.h"
#include "proto_comm.h"

extern unsigned char g_szMetalib_global_var_svr[];

#define ROLE_DB_META_TDR_DIR_1 "../../cfg/db_meta/role_db_meta.tdr"
#define ROLE_DB_META_TDR_DIR_2 "role_db_meta.tdr"

char g_DBName[GLOBAL_VAR_SVR_URL_LEN]; 

int init_dbhandle(LPTDRMETALIB* pstResLib,TDRDBHANDLE *hResDBHandle)
{
	TDRDBMS stDBMS;
	char szErrorMsg[2048]={0};
	GLOBAL_VAR_SVRCONF stConf;
	LPTDRMETALIB pstLib;
	TDRDATA stHost;
	LPTDRMETA pstMeta = NULL;

	stHost.iBuff = sizeof(stConf);
	stHost.pszBuff = (char *)&stConf;

	pstLib = (LPTDRMETALIB)&g_szMetalib_global_var_svr[0];

	pstMeta = tdr_get_meta_by_name(pstLib, "global_var_svrconf");
	if(NULL == pstMeta)
	{
		printf("tdr_get_meta_by_name fail;\n");
		return -1;
	}
	
	if( tdr_input_file(pstMeta, &stHost, "global_var_svr.xml", 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME) < 0)
	{
		printf("加载配置文件失败!!!\n");
		return -1;
	}

	if (0 > tdr_load_metalib(pstResLib, ROLE_DB_META_TDR_DIR_1) && 
		0 > tdr_load_metalib(pstResLib,ROLE_DB_META_TDR_DIR_2))
	{
		printf("load meta lib resdb_meta fail\n");
		return -1;
	}

	memset(&stDBMS, 0, sizeof(stDBMS));
	STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
	STRNCPY(stDBMS.szDBMSCurDatabaseName, stConf.szDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
	STRNCPY(stDBMS.szDBMSConnectionInfo, stConf.szDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
	STRNCPY(stDBMS.szDBMSUser, stConf.szDBUser, sizeof(stDBMS.szDBMSUser));
	STRNCPY(stDBMS.szDBMSPassword, stConf.szDBPasswd, sizeof(stDBMS.szDBMSPassword));
	stDBMS.iReconnectOpt = 1;	

	if (0 > tdr_open_dbhanlde(hResDBHandle, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s", szErrorMsg);
		return -1;
	}

	STRNCPY(g_DBName,stConf.szDBName, GLOBAL_VAR_SVR_URL_LEN);

	return 0;
}

int select_player_var_value(TDRDBHANDLE hDBHandle, TDRDBOBJECT *pstDBObj, FILE *fp,int iIdx,int iBit)
{
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	int i;
	char szSql[102400];
	ROLEDATA *pstRoleData;
	int iValue = 0;
	char cFlag = 0;

	if ((iBit>=0 && iBit<=7) || 8==iBit || 16==iBit || 32==iBit)
	{
		
	}
	else
	{
		return -1;
	}

	if( iIdx<0 || iIdx>=MAX_ROLE_SCRIPT_VAR || (16==iBit && iIdx>=MAX_ROLE_SCRIPT_VAR-1) || (32==iBit && iIdx>=MAX_ROLE_SCRIPT_VAR-3))
	{
		return -1;
	}
		
	snprintf(szSql, sizeof(szSql),"select * from RoleData;");
	stSql.iBuff = strlen(szSql);
	stSql.pszBuff = &szSql[0];

	if(tdr_query(&hDBResult, hDBHandle, &stSql) != 0)
	{
		printf("执行SQL语句失败!\n");
		return -1;
	}

	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows < 1)
	{
		return -1;
	}

	printf("RoleData表总共有%d条数据\n",iNumRows);

	for(i=0; i<iNumRows; i++ )
	{
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
		
		pstRoleData = (ROLEDATA *)pstDBObj->pszObj;
		
		if( 8 == iBit )
		{
			iValue = pstRoleData->MiscInfo.GlobalVar[iIdx];
		}
		else if (16 == iBit)
		{
			iValue = *(short*)(pstRoleData->MiscInfo.GlobalVar+iIdx);
		}
		else if (32 == iBit)
		{
			iValue = *(int*)(pstRoleData->MiscInfo.GlobalVar+iIdx);
		}
		else
		{
			cFlag	=	1 << iBit;

			if( pstRoleData->MiscInfo.GlobalVar[iIdx] & cFlag )
				iValue	=	1;
			else
				iValue	=	0;
		}
		
		if(iValue > 0)
		{
			snprintf(szSql, sizeof(szSql), "%d,%llu,%d\n", pstRoleData->Uin,pstRoleData->RoleID,iValue);
			fputs(szSql, fp);
		}
	}
	
	tdr_free_dbresult(&hDBResult);
	
	return 0;
}

static int updata_db(TDRDBHANDLE hDBHandle, TDRDBOBJECT *pstDBObj)
{
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBUpResult = 0;
	int iRet = 0;
	char szSql[409600];
	
	stSql.pszBuff = &szSql[0];
	stSql.iBuff = sizeof(szSql);
	
	iRet = tdr_obj2sql(hDBHandle, &stSql, TDR_DBOP_UPDATE, pstDBObj, NULL);
	
	if (0 != iRet)
	{
		printf("生成更新资源记录的SQL语句失败: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(hDBHandle));
		return iRet;
	}
	stSql.iBuff = strlen(szSql);

	iRet = tdr_query(&hDBUpResult, hDBHandle, &stSql);
	if (0 != iRet)
	{
		printf("执行更新资源记录的SQL语句失败: %s dbms error:%s\n",
				tdr_error_string(iRet), tdr_dbms_error(hDBHandle));	
		return iRet;
	}

	return 0;
}

int select_global_to_clear_var(TDRDBHANDLE hDBHandle, TDRDBOBJECT *pstDBObj)
{
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	char szSql[102400];
	GLOBAL *pstGlobal;
	
	stSql.iBuff = sizeof(szSql);
	stSql.pszBuff = &szSql[0];

	iRet = tdr_obj2sql(hDBHandle, &stSql, TDR_DBOP_SELECT, pstDBObj, NULL);
	if (0 != iRet)
	{
		printf("生成查询资源记录的SQL语句失败: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(hDBHandle));
		return iRet;
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

	pstGlobal = (GLOBAL*)pstDBObj->pszObj;
	memset(&pstGlobal->Misc.GlobalVar,0,sizeof(pstGlobal->Misc.GlobalVar));

	//更新回数据库
	iRet = updata_db(hDBHandle, pstDBObj);
	
	tdr_free_dbresult(&hDBResult);
	if(iRet != 0)
	{
		return -1;
	}
	return 0;
}

int help(void)
{
	printf("参数错误，请参考一下示例：\n");
	printf("./global_var_svr 1 45 32-----1表示导出表格，45表示从角色身上的GlobalVar45开始[单位32个字节]有值时的表格\n");
	printf("./global_var_svr 1 45 16-----1表示导出表格，45表示从角色身上的GlobalVar45开始[单位16个字节]有值时的表格\n");
	printf("./global_var_svr 1 45 8-----1表示导出表格，45表示从角色身上的GlobalVar45开始[单位8个字节]有值时的表格\n");
	printf("./global_var_svr 2   -----2表示删除global内的global_var信息\n");
	return 0;
}

int main(int argc, char* argv[])
{
	LPTDRMETALIB pstResLib;
	TDRDBHANDLE hResDBHandle;
	TDRDBOBJECT stDBObj;
	ROLEDATA stRoleData;
	char szFileName[128];
		
	if(argc < 2)
	{
		help();
		return -1;
	}

	if(init_dbhandle(&pstResLib,&hResDBHandle) < 0)
	{
		printf("init_dbhandle fail;\n");
		return -1;
	}
	
	if(argc == 4)
	{
		if(atoi(argv[1]) == 1 && atoi(argv[2]) >= 0 && atoi(argv[3]) >= 0)
		{
			FILE *fp;

			snprintf(szFileName,sizeof(szFileName),"global_var_%s.csv",g_DBName);
			fp = fopen(szFileName, "w+");
			if (NULL == fp)
			{
				printf("open file %s fail\n", szFileName);
				return -1;
			}
			
			stDBObj.pstMeta = tdr_get_meta_by_name(pstResLib, "RoleData");
			stDBObj.iVersion = tdr_get_meta_current_version(stDBObj.pstMeta);
			stDBObj.iObjSize = sizeof(stRoleData);
			stDBObj.pszObj = (char*)&stRoleData;
	
			fputs("Uin,RoleID,TeamNum\r\n" , fp);
			
			select_player_var_value(hResDBHandle, &stDBObj, fp,atoi(argv[2]),atoi(argv[3]));
			fclose(fp);
		}
		else
		{
			help();
			return -1;
		}
	}
	else if(argc == 2)
	{
		if(atoi(argv[1]) == 2)
		{
		 	GLOBAL stGlobal;
			stDBObj.pstMeta = tdr_get_meta_by_name(pstResLib, "Global");
			stDBObj.iVersion = tdr_get_meta_current_version(stDBObj.pstMeta);
			stDBObj.iObjSize = sizeof(stGlobal);
			stDBObj.pszObj = (char*)&stGlobal;
			select_global_to_clear_var(hResDBHandle,&stDBObj);
		}
		else
		{
			help();
			return -1;
		}
	}
	else
	{
		help();
		return -1;
	}
	
	return 0;
}


