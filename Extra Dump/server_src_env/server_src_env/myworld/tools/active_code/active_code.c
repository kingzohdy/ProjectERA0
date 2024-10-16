

#include "active_code_desc.h"
#include <stdio.h>
#include "tdr/tdr.h"
#include "cs_net.h"
#include "comm_misc.h"
#include "apps/tormapi/torm_protocol_api.h"

extern unsigned char g_szMetalib_active_code[];

struct ActiveEnv
{
	ACTIVE_CODE_CONF stConf;
	LPTDRMETALIB pstMetaLib;
	TDRDBHANDLE hHandle;			
	TDRDBOBJECT stDBObj;
	FILE *fp; 
};

typedef struct ActiveEnv ACTIVEENV;
ACTIVEENV stEnv;

char gszChar[]={'2','3','4','5','6','7','8','9',
				'a','b','c','d','e','f','g','h','i','j','k','m','n','p','q','r','s','t','u','v','w','x','y','z',
				'A','B','C','D','E','F','G','H','I','J','K','L','M','N','P','Q','R','S','T','U','V','W','X','Y','Z'};

int active_code_init(ACTIVEENV *pstEnv)
{
	TDRDATA stHost;
	LPTDRMETALIB pstMetaLib;
	LPTDRMETA pstMeta = NULL;
	TDRDBMS stDBMS;
	char szErrorMsg[2048]={0};
	ACTIVE_CODE_CONF *pstConf;

	memset(pstEnv, 0, sizeof(*pstEnv));
	
	pstEnv->fp = fopen("./active.txt","w");
	if (pstEnv->fp == NULL)
	{
		printf("fopen error!\n");
		return -1;
	}

	stHost.iBuff = sizeof(pstEnv->stConf);
	stHost.pszBuff = (char *)&pstEnv->stConf;	
	
	pstMetaLib = (LPTDRMETALIB)&g_szMetalib_active_code[0];
	pstMeta = tdr_get_meta_by_name(pstMetaLib, "active_code_conf");
	if(NULL == pstMeta)
	{
		return -1;
	}
	
	if( tdr_input_file(pstMeta, &stHost, "active_code.xml", 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME) < 0)
	{
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
	STRNCPY(stDBMS.szDBMSCurDatabaseName, pstConf->szDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
	STRNCPY(stDBMS.szDBMSConnectionInfo, pstConf->szDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
	STRNCPY(stDBMS.szDBMSUser, pstConf->szDBUser, sizeof(stDBMS.szDBMSUser));
	STRNCPY(stDBMS.szDBMSPassword, pstConf->szDBPasswd, sizeof(stDBMS.szDBMSPassword));
	stDBMS.iReconnectOpt = 1;	
	
	if (0 > tdr_open_dbhanlde(&pstEnv->hHandle, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
		return -1;
	}

	srand(time(NULL));
	return 0;
}

int active_code_in(ACTIVEENV *pstEnv)
{
	int i,j;
	int iLen;
	char *p;
	int iCharLen = sizeof(gszChar);
	char c='\n';
	ACTIVE stActive;
	int iRet;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	char szSql[10240];
	int iCount = 0;
	
	pstEnv->stDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "Active");
	pstEnv->stDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDBObj.pstMeta);
	pstEnv->stDBObj.iObjSize = sizeof(stActive);
	pstEnv->stDBObj.pszObj = (char*)&stActive;

	for (i=0; i<pstEnv->stConf.iMakeNum; i++)
	{
		if (iCount > pstEnv->stConf.iMakeNum*10)
		{
			printf("something err\n");			
			break;
		}
		iCount++;
		
		memset(&stActive,0,sizeof(stActive));
		
		snprintf(stActive.ActiveCode, sizeof(stActive.ActiveCode),"%s", pstEnv->stConf.szPrefix);
		iLen = strlen(stActive.ActiveCode);
		
		if (pstEnv->stConf.iBit + iLen > 256)
			break;
		
		p = stActive.ActiveCode+iLen;

		for (j=0; j<pstEnv->stConf.iBit; j++)
		{
			*p = gszChar[rand()%iCharLen];	
			p++;
		}

		// todo insert db
		stSql.iBuff = sizeof(szSql);
		stSql.pszBuff = &szSql[0];
		iRet = tdr_obj2sql(pstEnv->hHandle, &stSql, TDR_DBOP_INSERT, &pstEnv->stDBObj, NULL);
		if (iRet < 0)
		{
			printf("生成插入记录的SQL语句失败: %s dbms error:%s\n",
					tdr_error_string(iRet), tdr_dbms_error(pstEnv->hHandle));
			return iRet;
		}
		
		iRet = tdr_query(&hDBResult, pstEnv->hHandle, &stSql);
		if (iRet < 0 )
		{
			i--;
			continue;
		}

		fwrite(stActive.ActiveCode, 1, strlen(stActive.ActiveCode), pstEnv->fp);
		fwrite(&c, 1, 1, pstEnv->fp);
	}
	return 0;
}

int active_code_end(ACTIVEENV *pstEnv)
{
	if (pstEnv->fp)
	{
		fclose(pstEnv->fp);
		pstEnv->fp = NULL;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	if (active_code_init(&stEnv) < 0)
	{
		goto end;
	}

	if (active_code_in(&stEnv) < 0)
	{
		goto end;
	}

end:
	active_code_end(&stEnv);
	return 0;
}
