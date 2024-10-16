#include <stdio.h>
#include "genius_rectify_desc.h"
#include "tdr/tdr.h"
#include "cs_net.h"
#include "comm_misc.h"
#include "apps/tormapi/torm_protocol_api.h"
#include "role_db_meta.h"
#include "resdb_meta.h"

#define MAX_DEF 6784
#define BIN_SIZE 256

struct geniusEnv
{
	GENIUS_RECTIFY_CONF stConf;
	int iOld;
	int iNew;
	
	char sOld[MAX_DEF][BIN_SIZE];
	char sNew[MAX_DEF][BIN_SIZE];

	LPTDRMETALIB pstMetaLib;
	TDRDBHANDLE hDBHandle;	
	TDRDBHANDLE hUpDBHandle;	

	TDRDBOBJECT stDestDBObj;
};

typedef struct geniusEnv GENIUSENV;

extern unsigned char g_szMetalib_genius_rectify[];

int genius_load_res(GENIUSENV *pstEnv)
{
	time_t iTmp = 0;
	int iReLoad = 0;
	
	if (0 > load_resfile_one("./",  pstEnv->stConf.szOldBinName, pstEnv->sOld[0], &pstEnv->iOld,
								&iTmp, BIN_SIZE, MAX_DEF, &iReLoad))
	{
		printf("load_resfile_one 1 fail!\n");
		return -1;
	}

	iTmp = 0;
	iReLoad = 0;
	if (0 > load_resfile_one("./",  pstEnv->stConf.szNewBinName, pstEnv->sNew[0], &pstEnv->iNew,
								&iTmp, BIN_SIZE, MAX_DEF, &iReLoad))
	{
		printf("load_resfile_one 2 fail!\n");
		return -1;
	}
	return 0;
}

int genius_init(GENIUSENV *pstEnv)
{
	TDRDATA stHost;
	LPTDRMETALIB pstMetaLib;
	LPTDRMETA pstMeta = NULL;
	GENIUS_RECTIFY_CONF *pstConf = &pstEnv->stConf;

	stHost.iBuff = sizeof(*pstConf);
	stHost.pszBuff = (char *)pstConf;
	
	pstMetaLib = (LPTDRMETALIB)&g_szMetalib_genius_rectify[0];
	pstMeta = tdr_get_meta_by_name(pstMetaLib, "genius_rectify_conf");
	if(NULL == pstMeta)
	{
		return -1;
	}
	
	if( tdr_input_file(pstMeta, &stHost, "genius_rectify.xml", 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME) < 0)
	{
		return -1;
	}


	if (0 > tdr_load_metalib(&pstEnv->pstMetaLib, pstEnv->stConf.szMetaFile))
	{
		printf("load meta lib role_db_meta fail\n");
		return -1;
	}

	TDRDBMS stDBMS;
	char szErrorMsg[2048]={0};
	
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

	
	if (0 > tdr_open_dbhanlde(&pstEnv->hUpDBHandle, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
		return -1;
	}
	return 0;
}


GENIUSATTRDEF *genius_def_find(GENIUSENV *pstEnv, int iCareer, int iAttrID, int iLvl, int iOld)
{
	GENIUSATTRDEF stDef;
	int iEqu, iIdx;

	stDef.Career = iCareer;
	stDef.AttrID = iAttrID;
	stDef.AttrLvl = iLvl;

	if (iOld)
	{
		iIdx = bsearch_int(&stDef, pstEnv->sOld, pstEnv->iOld, BIN_SIZE, &iEqu);
		if (iEqu)
		{
			return (GENIUSATTRDEF *)&pstEnv->sOld[iIdx];
		}
	}
	else
	{
		iIdx = bsearch_int(&stDef, pstEnv->sNew , pstEnv->iNew, BIN_SIZE, &iEqu);
		if (iEqu)
		{
			return (GENIUSATTRDEF *)&pstEnv->sNew[iIdx];
		}
	}
	
	return NULL;
}

int genius_rectify_val(GENIUSENV *pstEnv, ROLEDATA *pstRoleData)
{
	int iVal = 0;
	GENIUSATTRDEF *pstOld;
	GENIUSATTRDEF *pstNew;
	GENIUSINFO *pstGeniusInfo = &pstRoleData->MiscInfo.GeniusInfo;
	int i, j, k;
	
	for (i=0; i<pstGeniusInfo->Num; i++)
	{
		for (j=0; j<pstGeniusInfo->One[i].Num; j++)
		{
			for (k=1; k<=pstGeniusInfo->One[i].SubOne[j].Lvl; k++)
			{
				pstOld = genius_def_find(pstEnv, pstRoleData->Career, 
									pstGeniusInfo->One[i].SubOne[j].AttrID, k, 1);
				if (!pstOld)
				{
					return -1;
				}

				pstNew = genius_def_find(pstEnv, pstRoleData->Career, 
										pstGeniusInfo->One[i].SubOne[j].AttrID, k, 0);
				if (!pstNew)
				{
					return -1;
				}

				if (pstOld->UpLimit.UpUseMoney > pstNew->UpLimit.UpUseMoney)
				{
					iVal += (pstOld->UpLimit.UpUseMoney - pstNew->UpLimit.UpUseMoney);
				}
			}
		}	
	}

	return iVal;
}


static int genius_updata_db(GENIUSENV* pstEnv, ROLEDATA *pstRoleData, int iVal)
{
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBUpResult = 0;
	int iRet = 0;
	char szSql[1024];
	
	stSql.pszBuff = &szSql[0];
	//stSql.iBuff = sizeof(szSql);

	snprintf(szSql,sizeof(szSql), "UPDATE RoleData SET Bull=Bull+%d where RoleID=%llu;", iVal , pstRoleData->RoleID);
	stSql.iBuff = strlen(szSql);
	
	iRet = tdr_query(&hDBUpResult, pstEnv->hUpDBHandle, &stSql);
	if (0 != iRet)
	{
		printf("执行更新资源记录的SQL语句失败: %s dbms error:%s\n",
				tdr_error_string(iRet), tdr_dbms_error(pstEnv->hUpDBHandle));	
		return -1;
	}

	printf("%s 增加绑银%d\n", pstRoleData->RoleName, iVal);

	return 0;
}


int genius_rectify(GENIUSENV *pstEnv)
{
	ROLEDATA stRoleData;
	ROLEDATA *pstRoleData;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	char szSql[512];
	int iCount = 0;
	char szBuff[1024];
	int i;
	int iVal = 0;
	
	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "RoleData");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stRoleData);
	pstEnv->stDestDBObj.pszObj = (char*)&stRoleData;

	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM RoleData");
	stSql.iBuff = strlen(stSql.pszBuff);
	
	iRet = tdr_query(&hDBResult, pstEnv->hDBHandle, &stSql);
	if (iRet < 0)
	{
		return -1;	
	}
	
	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows < 1)
	{
		return 0;
	}

	for (i=0; i<iNumRows; i++)
	{
		iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
		if (0 != iRet)
		{	
			return -1;
		}

		pstRoleData = (ROLEDATA*)pstEnv->stDestDBObj.pszObj;
		if (pstRoleData->Level < 60)
		{
			continue;
		}

		if (pstRoleData->MiscInfo.GeniusInfo.Num <= 0 || pstRoleData->MiscInfo.GeniusInfo.One[0].Num <= 0)
		{
			continue;
		}
		
		iVal = genius_rectify_val(pstEnv, pstRoleData);
		if (iVal < 0)
		{
			return -1;
		}

		if (iVal > 0)
		{
			if (genius_updata_db(pstEnv, pstRoleData, iVal) < 0)
			{
				return -1;
			}	
		}
		
	}

	return 0;
}

int main()
{
	GENIUSENV stEnv;
	memset (&stEnv, 0, sizeof(stEnv));

	if (genius_init(&stEnv) < 0)
	{
		printf("init fail!\n");
		return -1;
	}

	if (genius_load_res(&stEnv) < 0)
	{
		return -1;
	}

	if (genius_rectify(&stEnv) < 0)
	{
		printf("genius_rectify fail!\n");
		return -1;
	}
	return 0;
}


