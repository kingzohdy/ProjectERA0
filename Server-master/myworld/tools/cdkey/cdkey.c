#include "cdkey_desc.h"
#include <stdio.h>
#include "tdr/tdr.h"
#include "cs_net.h"
#include "comm_misc.h"
#include "apps/tormapi/torm_protocol_api.h"

extern unsigned char g_szMetalib_cdkey[];

struct CdkeyEnv
{
	CDKEY_CONF stConf;
	LPTDRMETALIB pstMetaLib;
	TDRDBHANDLE hHandle;			
	TDRDBOBJECT stDBObj;
	FILE *fp; 
};

typedef struct CdkeyEnv CDKEYENV;
CDKEYENV stEnv;

char gszCdKeyChar[]={'2','3','4','5','6','7','8','9',
				'a','b','c','d','e','f','g','h','i','j','k','m','n','p','q','r','s','t','u','v','w','x','y','z',
				'A','B','C','D','E','F','G','H','I','J','K','L','M','N','P','Q','R','S','T','U','V','W','X','Y','Z'};

int cdkey_init(CDKEYENV *pstEnv)
{
	TDRDATA stHost;
	LPTDRMETALIB pstMetaLib;
	LPTDRMETA pstMeta = NULL;
	TDRDBMS stDBMS;
	char szErrorMsg[2048]={0};
	CDKEY_CONF *pstConf;

	memset(pstEnv, 0, sizeof(*pstEnv));
	
	pstEnv->fp = fopen("./cdkey.txt","w");
	if (pstEnv->fp == NULL)
	{
		printf("fopen error!\n");
		return -1;
	}

	stHost.iBuff = sizeof(pstEnv->stConf);
	stHost.pszBuff = (char *)&pstEnv->stConf;	
	
	pstMetaLib = (LPTDRMETALIB)&g_szMetalib_cdkey[0];
	pstMeta = tdr_get_meta_by_name(pstMetaLib, "cdkey_conf");
	if(NULL == pstMeta)
	{
		return -1;
	}
	
	if( tdr_input_file(pstMeta, &stHost, "cdkey.xml", 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME) < 0)
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


int cdkey_make_one(char *p, int iBit, int iRand)
{
	int i;
	for (i=0; i<iBit; i++)
	{
		*p = gszCdKeyChar[rand()%iRand];	
		p++;	
	}
	return 0;
}

int cdkey_make_in(CDKEYENV *pstEnv)
{
	int i,j;
	CDKEYDEF stDef;
	int iRet;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	char szSql[10240];
	char c='\n';
	char *p;
	int iLen;
	int iCharLen = sizeof(gszCdKeyChar);
	int iCount;
	
	pstEnv->stDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "CDKeyDef");
	pstEnv->stDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDBObj.pstMeta);
	pstEnv->stDBObj.iObjSize = sizeof(stDef);
	pstEnv->stDBObj.pszObj = (char*)&stDef;

	for (i=0; i<pstEnv->stConf.iCdkeyNum; i++)
	{
		iCount = 0;
		for (j=0; j<pstEnv->stConf.astMakeCdkey[i].iMakeNum; j++)
		{
			if (iCount > pstEnv->stConf.astMakeCdkey[i].iMakeNum*10)
			{
				printf("something err\n");			
				break;
			}
			iCount++;
			
			memset(&stDef,0,sizeof(stDef));
			snprintf(stDef.CDKey, sizeof(stDef.CDKey),"%s",pstEnv->stConf.astMakeCdkey[i].szType);
			iLen = strlen(stDef.CDKey);
			p = stDef.CDKey+iLen;
			
			cdkey_make_one(p, pstEnv->stConf.astMakeCdkey[i].iLen, iCharLen);

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
				j--;
				continue;
			}

			fwrite(stDef.CDKey, 1, strlen(stDef.CDKey), pstEnv->fp);
			fwrite(&c, 1, 1, pstEnv->fp);
			
		}	
	}
	return 0;
}

int cdkey_end(CDKEYENV *pstEnv)
{
	if (pstEnv->fp)
	{
		fclose(pstEnv->fp);
		pstEnv->fp = NULL;
	}

	if (pstEnv->hHandle)
	{
		tdr_close_dbhanlde(pstEnv->hHandle);
	}
	return 0;
}

int main(int argc, char* argv[])
{
	if (cdkey_init(&stEnv) < 0)
	{
		goto end;
	}

	if (cdkey_make_in(&stEnv) < 0)
	{
		goto end;
	}
end:
	cdkey_end(&stEnv);
	return 0;
}

