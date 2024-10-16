
#include <stdio.h>
#include "db_mem_xml_desc.h"
#include "tdr/tdr.h"
#include "cs_net.h"
#include "comm_misc.h"
#include "apps/tormapi/torm_protocol_api.h"
#include "role_db_meta.h"

#define META_TDR_DIR  "../../cfg/db_mate" 
#define MAX_ROLE_DEF 10

extern unsigned char g_szMetalib_db_mem_xml[];

char szRoleName[32];


int init_dbhandle(LPTDRMETALIB* pstResLib,TDRDBHANDLE *hResDBHandle,const char* pszDbName)
{
	TDRDATA stHost;
	TDRDBMS stDBMS;
	char szErrorMsg[2048]={0};
	DB_MEM_XML_CONF stConf;
	LPTDRMETALIB pstLib;
	LPTDRMETA pstMeta = NULL;

	stHost.iBuff = sizeof(stConf);
	stHost.pszBuff = (char *)&stConf;
	
	pstLib = (LPTDRMETALIB)&g_szMetalib_db_mem_xml[0];
	pstMeta = tdr_get_meta_by_name(pstLib, "db_mem_xml_conf");
	if(NULL == pstMeta)
	{
		return -1;
	}
	if( tdr_input_file(pstMeta, &stHost, "db_mem_xml.xml", 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME) < 0)
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
	//STRNCPY(stDBMS.szDBMSCurDatabaseName, pszDbName, sizeof(stDBMS.szDBMSCurDatabaseName));
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


int select_global_to_xml(TDRDBHANDLE hDBHandle, TDRDBOBJECT *pstDBObj, char*pszName)
{
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	int i;
	char szSql[102400];
	
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

	if(res_mem_to_xml(pstDBObj->pstMeta, pszName, pstDBObj->pszObj, pstDBObj->iObjSize) < 0)
	{
		return -1;
	}
	
	
	tdr_free_dbresult(&hDBResult);
	if(iRet != 0)
	{
		return -1;
	}
	return 0;
}

int select_student(TDRDBHANDLE hDBHandle, TDRDBOBJECT *pstDBObj, FILE *fp)
{
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	int i;
	char szSql[102400];
	char szXmlName[128];
	ROLEDATA *pstRoleData;
		
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

		if (pstRoleData->SyncRelation.School.JobType == ROLE_SCHOOL_TYPE_STUDENT)
		{
			snprintf(szSql, sizeof(szSql), "%llu\n", pstRoleData->RoleID);
			fputs(szSql, fp);
		}
	}
	
	tdr_free_dbresult(&hDBResult);
	
	return 0;
}

int select_clan_to_xml(TDRDBHANDLE hDBHandle, TDRDBOBJECT *pstDBObj,
							unsigned int  uiClanID)
{
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	int i;
	char szSql[102400];
	char szXmlName[128];
	CLAN* pstClan;
	
	snprintf(szSql, sizeof(szSql),"select * from Clan;");
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

		pstClan = (CLAN*)(pstDBObj->pszObj);
		
		sprintf(szXmlName, "./%s.xml", pstClan->ClanProf.Name);
		if (uiClanID == 0 || uiClanID == pstClan->ClanProf.Id)
		{
			if(res_mem_to_xml(pstDBObj->pstMeta, szXmlName, pstDBObj->pszObj, pstDBObj->iObjSize) < 0)
			{
				return -1;
			}
		}
	}
	
	tdr_free_dbresult(&hDBResult);
	if(iRet != 0)
	{
		return -1;
	}
	return 0;
}

int select_db_to_xml(TDRDBHANDLE hDBHandle, TDRDBOBJECT *pstDBObj, int  uin)
{
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	char stWhere[128];
	char szXmlName[128];
	int iNumRows = 0;
	int i;
	char szSql[102400];
	
	stSql.iBuff = sizeof(szSql);
	stSql.pszBuff = &szSql[0];

	sprintf(stWhere,"WHERE Uin = '%d'",uin);
	iRet = tdr_obj2sql(hDBHandle, &stSql, TDR_DBOP_SELECT, pstDBObj, stWhere);
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
	if(iNumRows < 1)
	{
		return -1;
	}
	
	for(i=0; i<iNumRows; i++ )
	{
		iRet = tdr_fetch_row(hDBResult, pstDBObj);
		if (szRoleName[0] != 0 && strcmp(szRoleName,((ROLEDATA*)(pstDBObj->pszObj))->RoleName) != 0)
		{
			continue;
		}

		if (((ROLEDATA*)(pstDBObj->pszObj))->DelTime > 0)
		{
			continue;
		}
		
		if (0 != iRet)
		{			
			if ((TDR_ERR_GET_ERROR_CODE(iRet) != TDR_ERROR_DB_NO_RECORD_IN_RESULTSET) ||	
				(0 != tdr_dbms_errno(hDBResult)))			
			{				
				printf("从数据库表中读取数据记录失败:%s, dbms  error:%s\n",					
				tdr_error_string(iRet), tdr_dbms_error(hDBResult));
			}
			else	
			{		
				iRet = 0; 
			}	
			break;		
		}
		
		sprintf(szXmlName, "./%s.xml", ((ROLEDATA*)(pstDBObj->pszObj))->RoleName);
		if(res_mem_to_xml(pstDBObj->pstMeta, szXmlName, pstDBObj->pszObj, pstDBObj->iObjSize) < 0)
		{
			return -1;
		}
	}
	
	tdr_free_dbresult(&hDBResult);
	if(iRet != 0)
	{
		return -1;
	}
	return 0;
}



/* 1. db_mem_xml 12333 0  world_1	
12333表示uin       0表示导出为xml文件	world_1 表示从哪个数据库导出



    2. dn_mem_xml  123.xml   1 world_2
123.xml文件名    1 表示导入到数据库    可以是非0数字	world_2表示导入到哪个数据库	
*/

int main(int argc, char* argv[])
{
	LPTDRMETALIB pstResLib;
	TDRDBHANDLE hResDBHandle;
	TDRDBOBJECT stDBObj;
	ROLEDATA stRoleData;
	char szXmlName[128];
	LPTDRMETA pstMeta = NULL;
	TDRDATA stHost;
	char *pCh;

	/*if(argc != 3)
	{
		printf("参数错误\n");
		return -1;
	}*/
	
	if(argc < 2) 
	{
		printf("参数错误,参考一下示例:\n");
		printf("./db_mem_xml 12333  0   ---导出数据库\n");
		printf("./db_mem_xml 123.xml 1  ---导入数据库\n");
		printf("./db_mem_xml 2-----导出Global表\n");
		printf("./db_mem_xml 3 clanid-----导出Clan表\n");
		printf("./db_mem_xml 4 filename-----导出徒弟的RoleID\n");
		return -1;
	}

	szRoleName[0] = 0;
	if (argc == 5)
	{
		STRNCPY(szRoleName, argv[4], sizeof(szRoleName));
	}

	if( init_dbhandle(&pstResLib, &hResDBHandle,argv[3]) < 0)
	{
		printf("init_dbhandle fail:\n");
		return -1;
	}

	if (argc == 2)
	{
		if (atoi(argv[1]) == 2)
		{
			GLOBAL stGlobal;
			stDBObj.pstMeta = tdr_get_meta_by_name(pstResLib, "Global");
			stDBObj.iVersion = tdr_get_meta_current_version(stDBObj.pstMeta);
			stDBObj.iObjSize = sizeof(stGlobal);
			stDBObj.pszObj = (char*)&stGlobal;
			select_global_to_xml(hResDBHandle,&stDBObj,"Global.xml");
		}
	}
	else
	{
		if (atoi(argv[1]) == 3)
		{
			CLAN stClan;
			stDBObj.pstMeta = tdr_get_meta_by_name(pstResLib, "Clan");
			stDBObj.iVersion = tdr_get_meta_current_version(stDBObj.pstMeta);
			stDBObj.iObjSize = sizeof(stClan);
			stDBObj.pszObj = (char*)&stClan;
			select_clan_to_xml(hResDBHandle,&stDBObj, atoi(argv[2]));
		}
		else if (atoi(argv[1]) == 4)
		{
			FILE *fp;

			fp = fopen(argv[2], "w+");
			if (NULL == fp)
			{
				printf("open file %s fail\n", argv[2]);
				return -1;
			}
			
			stDBObj.pstMeta = tdr_get_meta_by_name(pstResLib, "RoleData");
			stDBObj.iVersion = tdr_get_meta_current_version(stDBObj.pstMeta);
			stDBObj.iObjSize = sizeof(stRoleData);
			stDBObj.pszObj = (char*)&stRoleData;
			
			select_student(hResDBHandle, &stDBObj, fp);
			fclose(fp);
		}
		else if(atoi(argv[2]) == 0)
		{
			stRoleData.Uin = atoi(argv[1]);

			stDBObj.pstMeta = tdr_get_meta_by_name(pstResLib, "RoleData");
			stDBObj.iVersion = tdr_get_meta_current_version(stDBObj.pstMeta);
			stDBObj.iObjSize = sizeof(stRoleData);
			stDBObj.pszObj = (char*)&stRoleData;
			
			 if(select_db_to_xml (hResDBHandle, &stDBObj, stRoleData.Uin) < 0)
			 {
				return -1;
			 }
		}
		else if (atoi(argv[2]) == 1)
		{
			pCh = strrchr(argv[1], '.');
			if(pCh == NULL)
			{
				snprintf(szXmlName, sizeof(szXmlName), "./%s.xml", argv[1]);
				szXmlName[sizeof(szXmlName) - 1] = 0;
			}
			else
			{	
				STRNCPY(szXmlName, argv[1], sizeof(szXmlName));
			}
			
			pstMeta = tdr_get_meta_by_name(pstResLib, "RoleData");
			
			stHost.pszBuff = (char*)&stRoleData;
			stHost.iBuff = sizeof(stRoleData);

			if( tdr_input_file(pstMeta, &stHost, szXmlName, 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME) < 0)
			{
				printf("load file is fail\n");
				return -1;
			}

			if(res_mem_to_db(hResDBHandle, pstMeta, stHost.pszBuff, stHost.iBuff) < 0)
			{
				printf("res_mem_to_db is fail\n");
				return -1;
			}
		}
	}
	
	 printf("end\n");
	 return 0;
}
