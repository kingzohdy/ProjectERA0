

#include <stdio.h>
#include "unite_svr_desc.h"
#include "tdr/tdr.h"
#include "cs_net.h"
#include "comm_misc.h"
#include "apps/tormapi/torm_protocol_api.h"
#include "role_db_meta.h"
#include "comm/shtable.h"
#include "unite_svr_zh.h"

#define DEST_DB 0
#define SRC_DB 1

#define ACCOUNT_ARGS 2
#define WORLD_ARGS 1

#define NEED_UP_UIN 1
#define NOT_UP_UIN 2

#define FLAG_TRADEBUY 0
#define FLAG_TRADESELL 1

#define DB_FLAG_WORLD 1
#define DB_FLAG_ACCOUNT 2
#define DB_FLAG_BILL 3
#define DB_FLAG_REGION 4

typedef struct AccData
{
	char szPass9[ACCOUNT_LOGIN_NAME_LEN];
	int iUin;
}ACCDATA;

typedef struct AccGold
{
	int iUin;
	unsigned long long ullGold;
}ACCGOLD;

typedef struct UinMudExp
{
	int Uin;                             	/*   �ʺ�ID */
	tdr_longlong MudExp;                          	/*   �һ���õľ����ܺ� */
	int TakeTime;                        	/*   ͵�����ʱ�� */
	int DayTimes;                        	/*   ����͵����Ĵ��� */
	
}UINMUDEXP;

typedef struct BillData
{
	int iUin;
	unsigned long long ullAmount;
	unsigned long long ullPoints;
	unsigned long long ullBonus;
	unsigned long long ullCashTotalPoints;
	unsigned long long UsedTotalPoints;
}UBILLDATA;

typedef struct UinteClanData
{
	char cDBFlag;				// 0-src 1-dst  ��ʾ���ĸ����Ĺ���
	unsigned short usLvl;			// ����ȼ�
	unsigned int uiActiveVal;		// ��Ծ��
	unsigned long long ullWID;		// ΨһID

}UINTECLANDATA;

typedef struct UinteStoreData
{
	char cDBFlag;				// 0-src 1-dst  ��ʾ���ĸ���
	char szName[32];
	unsigned long long ullExchg;	// ���׶��
	unsigned long long ullTax;		// ˰��
	int iID;
	int iMoney;
	int iNum;
	STOREGRID stGrid[40];

}UINTESTOREDATA;


struct AccUin
{
	int iOldUin;
	int iNewUin;
};

typedef struct AccUin ACCUIN;

struct ShadowRoleName
{
	char szName[32];	
};
typedef struct ShadowRoleName SNAME;

struct uniteEnv
{
	UNITE_SVR_CONF stConf;
	
	LPTDRMETALIB pstMetaLib;
	TDRDBHANDLE hSrcDBHandle;			
	TDRDBHANDLE hSrcUpDBHandle;		
	TDRDBHANDLE hDestDBHandle;
	TDRDBHANDLE hDestUpDBHandle;

	TDRDBHANDLE hSrcAccHandle;
	TDRDBHANDLE hDstAccHandle;
	TDRDBHANDLE hDstUpAccHandle;
	TDRDBHANDLE hSrcUpAccHandle;

	TDRDBHANDLE hSrcBillHandle;
	TDRDBHANDLE hDstBillHandle;
	TDRDBHANDLE hDstUpBillHandle;

	TDRDBHANDLE hDstRegionHandle;
	TDRDBHANDLE hDstUpRegionHandle;
	
	TDRDBOBJECT stDestDBObj;

	int iDestClanLastIdx;
	int iSrcClanLastIdx;

	int iDstClanCount;
	int iDstMaxUin;
	FILE *fp;
	char cArgsFlag;
	char cUpUinFalg;

	// Ŀ�����ݿ��������
	LPSHTABLE pstNameDest; 

	// �������
	LPSHTABLE pstOverlapName;
	
	// Ŀ�����ݿ⹤��
	LPSHTABLE pstClanDest;

	// ��������
	LPSHTABLE pstOverlapClan;

	LPSHTABLE pstDstAccSht; 		
	LPSHTABLE pstOverlapAccSht;

	LPSHTABLE pstSrcAccUinSht;  // ����uin����pass9
	LPSHTABLE pstDstAccUinSht;
	LPSHTABLE pstUinMudExpSht;

	LPSHTABLE pstWorldAccSht;
};

typedef struct uniteEnv UNITEENV;

extern unsigned char g_szMetalib_unite_svr[];

static int updata_src_db(UNITEENV* pstEnv, TDRDBOBJECT *pstDBObj);
static int updata_dest_db(UNITEENV* pstEnv, TDRDBOBJECT *pstDBObj);
static int insert_dest_db(UNITEENV* pstEnv, TDRDBOBJECT *pstDBObj,int iDBFlag);
static int store_mail(UNITEENV* pstEnv, UINTESTOREDATA *pstOne);
static int up_trade_insert_mailentry(UNITEENV* pstEnv, MAILENTRY *pstMailEntry);
int update_account_db(UNITEENV* pstEnv, TDRDBOBJECT *pstDBObj, int iFlag);
static int UniteHashCmp( const void * pv1 , const void *pv2 )
{
	return strcmp((char*)pv1, (char*)pv2);
}

static unsigned int UniteIDHashCode(const void* pvData)
{	
	return sht_get_code((char*)pvData);
}

static int AccDataCmp( const void * pv1 , const void *pv2 )
{
	ACCDATA *p1 = (ACCDATA *)pv1;
	ACCDATA *p2 = (ACCDATA *)pv2;
	return strcmp(p1->szPass9, p2->szPass9);
}

static  unsigned int AccDataCode(const void* pvData)
{
	ACCDATA *p = (ACCDATA *)pvData;
	return sht_get_code(p->szPass9);
}

static int AccDataUinCmp(const void * pv1 , const void *pv2)
{
	ACCDATA *p1 = (ACCDATA *)pv1;
	ACCDATA *p2 = (ACCDATA *)pv2;
	return p1->iUin -p2->iUin;
}

static unsigned int AccDataUinCode(const void* pvData)
{
	ACCDATA *p = (ACCDATA *)pvData;
	return p->iUin;
}

static int UinMudExpCmp(const void * pv1 , const void *pv2)
{
	SHADOWACCDATA*p1 = (SHADOWACCDATA *)pv1;
	SHADOWACCDATA *p2 = (SHADOWACCDATA *)pv2;
	return p1->Uin -p2->Uin;
}

static unsigned int UinMudExpCode(const void* pvData)
{
	SHADOWACCDATA *p = (SHADOWACCDATA *)pvData;
	return p->Uin;
}

static int WorldAccCmp(const void * pv1 , const void *pv2)
{
	ACCSHARE*p1 = (ACCSHARE *)pv1;
	ACCSHARE *p2 = (ACCSHARE *)pv2;
	return p1->Uin -p2->Uin;
}

static unsigned int WorldAccCode(const void* pvData)
{
	ACCSHARE *p = (ACCSHARE *)pvData;
	return p->Uin;
}

static int AccUinCmp( const void * pv1 , const void *pv2 )
{
	ACCUIN *p1 = (ACCUIN *)pv1;
	ACCUIN *p2 = (ACCUIN *)pv2;
	return p1->iOldUin - p2->iOldUin;
}

static  unsigned int AccUinCode(const void* pvData)
{
	ACCUIN*p1 = (ACCUIN *)pvData;
	return (unsigned int)p1->iOldUin;
}


static int AccGoldCmp( const void * pv1 , const void *pv2 )
{
	ACCGOLD *p1 = (ACCGOLD *)pv1;
	ACCGOLD *p2 = (ACCGOLD *)pv2;
	return p1->iUin - p2->iUin;
}

static  unsigned int AccGoldCode(const void* pvData)
{
	ACCGOLD *p = (ACCGOLD *)pvData;
	return p->iUin;
}

static int BillDataCmp( const void * pv1 , const void *pv2 )
{
	UBILLDATA*p1 = (UBILLDATA *)pv1;
	UBILLDATA *p2 = (UBILLDATA *)pv2;
	return p1->iUin - p2->iUin;
}

static  unsigned int BillDataCode(const void* pvData)
{
	UBILLDATA *p = (UBILLDATA *)pvData;
	return p->iUin;
}

static int modify_role_name(UNITEENV* pstEnv,char *pszName, int iNameLen, int *iRet)
{
	char szName[32];

	szName[0] = '*';
	int iLen = 1;
	
	if (!pstEnv->pstOverlapName)
	{
		return 0;
	}

	if (sht_find(pstEnv->pstOverlapName, pszName, UniteHashCmp, UniteIDHashCode))
	{
		if (pstEnv->stConf.iAccFlag)
		{
			snprintf(szName+iLen, sizeof(szName)-iLen, "%d_", pstEnv->stConf.iSrcRegionID);
			iLen = strlen(szName);
		}

		if (pstEnv->stConf.iWorldFlag)
		{
			snprintf(szName+iLen, sizeof(szName)-iLen, "%d_", pstEnv->stConf.iSrcWorldID);
			iLen = strlen(szName);
		}

		snprintf(szName+iLen, sizeof(szName)-iLen, "%s", pszName);
		iLen = strlen(szName);
		
		STRNCPY(pszName, szName, iNameLen);
		if (iRet)
		{
			*iRet = 1;
		}
	}
	
	return 0;
}

static int modify_role_uin(UNITEENV* pstEnv,int *piUin)
{
	ACCUIN stAccUin;
	ACCUIN *pstAccUin;
	int iUin = *piUin;
	

	// ����Ŀ�����ݿ����Uin
	if (pstEnv->cUpUinFalg == NEED_UP_UIN)
	{
		*piUin += pstEnv->iDstMaxUin;
	}

	// ����˺���ͬ uin��ͬ����� ,��Ŀ�����ݿ�uinΪ׼
	if (!pstEnv->pstOverlapAccSht)
	{
		return 0;
	}

	stAccUin.iOldUin = iUin;
	pstAccUin = sht_find(pstEnv->pstOverlapAccSht, &stAccUin, AccUinCmp, AccUinCode);
	if (pstAccUin)
	{
		*piUin = pstAccUin->iNewUin;
	}
	return 0;
}

static int modify_clan_name(UNITEENV* pstEnv,char *pszName, int iNameLen, int *iRet)
{
	char szName[32];

	szName[0] = '*';
	int iLen = 1;
	
	if (!pstEnv->pstOverlapClan)
	{
		return 0;
	}
	
	if (sht_find(pstEnv->pstOverlapClan, pszName, UniteHashCmp, UniteIDHashCode))
	{
		if (pstEnv->stConf.iAccFlag)
		{
			snprintf(szName+iLen, sizeof(szName)-iLen, "%d_", pstEnv->stConf.iSrcRegionID);
			iLen = strlen(szName);
		}

		if (pstEnv->stConf.iWorldFlag)
		{
			snprintf(szName+iLen, sizeof(szName)-iLen, "%d_", pstEnv->stConf.iSrcWorldID);
			iLen = strlen(szName);
		}

		snprintf(szName+iLen, sizeof(szName)-iLen, "%s", pszName);
		iLen = strlen(szName);
		
		STRNCPY(pszName, szName, iNameLen);
		if (iRet)
		{
			*iRet = 1;
		}
	}
	
	return 0;
}

static int unite_error_log(FILE *fp,const char *pstLog)
{
	time_t t_Now;
	struct tm *pstNow;
	char szLog[1024];

	time(&t_Now);
	pstNow = localtime(&t_Now);
	
	snprintf(szLog,sizeof(szLog),"[%d %d %d  %d:%d:%d]:%s\n",pstNow->tm_year+1900,pstNow->tm_mon+1,pstNow->tm_mday,
			pstNow->tm_hour, pstNow->tm_min, pstNow->tm_sec, pstLog);
	fwrite(szLog, strlen(szLog), 1, fp);
	return 0;
}

static int init_unite_dbhandle(UNITEENV *pstEnv)
{
	TDRDBMS stDBMS;
	char szErrorMsg[2048]={0};
	UNITE_SVR_CONF *pstConf = &pstEnv->stConf;
	
	memset(&stDBMS, 0, sizeof(stDBMS));
	STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
	STRNCPY(stDBMS.szDBMSCurDatabaseName, pstConf->szSrcRoleDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
	STRNCPY(stDBMS.szDBMSConnectionInfo, pstConf->szSrcRoleDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
	STRNCPY(stDBMS.szDBMSUser, pstConf->szSrcRoleDBUser, sizeof(stDBMS.szDBMSUser));
	STRNCPY(stDBMS.szDBMSPassword, pstConf->szSrcRoleDBPasswd, sizeof(stDBMS.szDBMSPassword));
	stDBMS.iReconnectOpt = 1;	
	
	if (0 > tdr_open_dbhanlde(&pstEnv->hSrcDBHandle, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
		return -1;
	}

	if (0 > tdr_open_dbhanlde(&pstEnv->hSrcUpDBHandle, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
		return -1;
	}

	memset(&stDBMS, 0, sizeof(stDBMS));
	STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
	STRNCPY(stDBMS.szDBMSCurDatabaseName, pstConf->szDestRoleDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
	STRNCPY(stDBMS.szDBMSConnectionInfo, pstConf->szDestRoleDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
	STRNCPY(stDBMS.szDBMSUser, pstConf->szDestRoleDBUser, sizeof(stDBMS.szDBMSUser));
	STRNCPY(stDBMS.szDBMSPassword, pstConf->szDestRoleDBPasswd, sizeof(stDBMS.szDBMSPassword));
	stDBMS.iReconnectOpt = 1;	
	
	if (0 > tdr_open_dbhanlde(&pstEnv->hDestDBHandle, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
		return -1;
	}

	if (0 > tdr_open_dbhanlde(&pstEnv->hDestUpDBHandle, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
		return -1;
	}

	memset(&stDBMS, 0, sizeof(stDBMS));
	STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
	STRNCPY(stDBMS.szDBMSCurDatabaseName, pstConf->szSrcAccDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
	STRNCPY(stDBMS.szDBMSConnectionInfo, pstConf->szSrcAccDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
	STRNCPY(stDBMS.szDBMSUser, pstConf->szSrcAccDBUser, sizeof(stDBMS.szDBMSUser));
	STRNCPY(stDBMS.szDBMSPassword, pstConf->szSrcAccDBPasswd, sizeof(stDBMS.szDBMSPassword));
	stDBMS.iReconnectOpt = 1;	
	if (0 > tdr_open_dbhanlde(&pstEnv->hSrcAccHandle, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
		return -1;
	}

	if (0 > tdr_open_dbhanlde(&pstEnv->hSrcUpAccHandle, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
		return -1;
	}

	memset(&stDBMS, 0, sizeof(stDBMS));
	STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
	STRNCPY(stDBMS.szDBMSCurDatabaseName, pstConf->szDstAccDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
	STRNCPY(stDBMS.szDBMSConnectionInfo, pstConf->szDstAccDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
	STRNCPY(stDBMS.szDBMSUser, pstConf->szDstAccDBUser, sizeof(stDBMS.szDBMSUser));
	STRNCPY(stDBMS.szDBMSPassword, pstConf->szDstAccDBPasswd, sizeof(stDBMS.szDBMSPassword));
	stDBMS.iReconnectOpt = 1;	
	if (0 > tdr_open_dbhanlde(&pstEnv->hDstAccHandle, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
		return -1;
	}

	if (0 > tdr_open_dbhanlde(&pstEnv->hDstUpAccHandle, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
		return -1;
	}



	memset(&stDBMS, 0, sizeof(stDBMS));
	STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
	STRNCPY(stDBMS.szDBMSCurDatabaseName, pstConf->szSrcBillDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
	STRNCPY(stDBMS.szDBMSConnectionInfo, pstConf->szSrcBillDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
	STRNCPY(stDBMS.szDBMSUser, pstConf->szSrcBillDBUser, sizeof(stDBMS.szDBMSUser));
	STRNCPY(stDBMS.szDBMSPassword, pstConf->szSrcBillDBPasswd, sizeof(stDBMS.szDBMSPassword));
	stDBMS.iReconnectOpt = 1;	
	if (0 > tdr_open_dbhanlde(&pstEnv->hSrcBillHandle, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
		return -1;
	}

	memset(&stDBMS, 0, sizeof(stDBMS));
	STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
	STRNCPY(stDBMS.szDBMSCurDatabaseName, pstConf->szDstBillDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
	STRNCPY(stDBMS.szDBMSConnectionInfo, pstConf->szDstBillDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
	STRNCPY(stDBMS.szDBMSUser, pstConf->szDstBillDBUser, sizeof(stDBMS.szDBMSUser));
	STRNCPY(stDBMS.szDBMSPassword, pstConf->szDstBillDBPasswd, sizeof(stDBMS.szDBMSPassword));
	stDBMS.iReconnectOpt = 1;	
	if (0 > tdr_open_dbhanlde(&pstEnv->hDstBillHandle, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
		return -1;
	}

	if (0 > tdr_open_dbhanlde(&pstEnv->hDstUpBillHandle, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
		return -1;
	}

	memset(&stDBMS, 0, sizeof(stDBMS));
	STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
	STRNCPY(stDBMS.szDBMSCurDatabaseName, pstConf->szDstRegionDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
	STRNCPY(stDBMS.szDBMSConnectionInfo, pstConf->szDstRegionDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
	STRNCPY(stDBMS.szDBMSUser, pstConf->szDstRegionDBUser, sizeof(stDBMS.szDBMSUser));
	STRNCPY(stDBMS.szDBMSPassword, pstConf->szDstRegionDBPasswd, sizeof(stDBMS.szDBMSPassword));
	stDBMS.iReconnectOpt = 1;

	if (0 > tdr_open_dbhanlde(&pstEnv->hDstRegionHandle, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
		return -1;
	}

	if (0 > tdr_open_dbhanlde(&pstEnv->hDstUpRegionHandle, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
		return -1;
	}
	return 0;
}

static int get_conf(UNITE_SVR_CONF* pstConf)
{
	TDRDATA stHost;
	LPTDRMETALIB pstMetaLib;
	LPTDRMETA pstMeta = NULL;

	stHost.iBuff = sizeof(*pstConf);
	stHost.pszBuff = (char *)pstConf;
	
	pstMetaLib = (LPTDRMETALIB)&g_szMetalib_unite_svr[0];
	pstMeta = tdr_get_meta_by_name(pstMetaLib, "unite_svr_conf");
	if(NULL == pstMeta)
	{
		return -1;
	}
	
	if( tdr_input_file(pstMeta, &stHost, "unite_svr.xml", 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME) < 0)
	{
		return -1;
	}

	return 0;
}


static int init_nameid_sht(UNITEENV* pstEnv)
{
	int iRet = 0;
	unsigned int iCount;
	
	iRet = tdr_get_records_count(pstEnv->hDestDBHandle, "NameID",NULL, &iCount);
	if (iRet < 0)
	{
		return -1;
	}

	if (iCount > 0)
	{
		pstEnv->pstNameDest = sht_create(iCount*3, iCount, NAME_LEN, NULL);
		if (NULL == pstEnv->pstNameDest)
		{
			printf("init_sht error \n");
			return -1;
		}

		pstEnv->pstOverlapName = sht_create(iCount*3, iCount,NAME_LEN, NULL);
		if (NULL == pstEnv->pstOverlapName)
		{
			printf("init_sht error \n");
			return -1;
		}
	}
	return 0;
}

static int init_uinmudexp_sht(UNITEENV* pstEnv)
{
	int iRet = 0;
	unsigned int iCount;
	
	iRet = tdr_get_records_count(pstEnv->hDestDBHandle, "ShadowAccData",NULL, &iCount);
	if (iRet < 0)
	{
		return 0;
	}

	if (iCount > 0)
	{
		pstEnv->pstUinMudExpSht = sht_create(iCount*3, iCount, sizeof(SHADOWACCDATA), NULL);
		if (NULL == pstEnv->pstUinMudExpSht)
		{
			printf("init_sht:pstUinMudExpSht error \n");
			return -1;
		}

		SHADOWACCDATA stData;
		SHADOWACCDATA* pstData;
		char szSql[10240];
		char szBuff[10240];
		TDRDATA stSql;
		int iCount = 0;
		int iRet;
		TDRDBRESULTHANDLE hDBResult = 0;
		int iNumRows = 0;
		int i;
	
		pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "ShadowAccData");
		pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
		pstEnv->stDestDBObj.iObjSize = sizeof(stData);
		pstEnv->stDestDBObj.pszObj = (char*)&stData;

	
		stSql.pszBuff = &szSql[0];
		snprintf(szSql, sizeof(szSql),"SELECT * FROM ShadowAccData");
		stSql.iBuff = strlen(stSql.pszBuff);
	
		iRet = tdr_query(&hDBResult, pstEnv->hDestDBHandle, &stSql);
		if (0 != iRet)
		{
			printf("execute sql sentence:SELECT * FROM ShadowAccData error\n");
			return -1;
		}
		iNumRows = tdr_num_rows(hDBResult);
		if(iNumRows > 0)
		{
			for (i=0; i<iNumRows; i++)
			{
				pstData = NULL;
				iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
				if (0 != iRet)
				{	
					printf("ShadowAccData table:tdr_fetch_row error\n");
					return -1;
				}
				
				pstData = sht_insert_unique(pstEnv->pstUinMudExpSht,(SHADOWACCDATA*)pstEnv->stDestDBObj.pszObj, 
										UinMudExpCmp, UinMudExpCode);
				if (!pstData)
				{
					printf("sht_insert_unique:init_uinmudexp_sht  error\n");
					return -1;
				}
				else
				{
					*pstData = *(SHADOWACCDATA*)pstEnv->stDestDBObj.pszObj;
				}
			}
		}
	}
	return 0;
}

static int init_clan_sht(UNITEENV* pstEnv)
{
	int iRet = 0;
	unsigned int iCount;
	
	iRet = tdr_get_records_count(pstEnv->hDestDBHandle, "Clan",NULL, &iCount);
	if (iRet < 0)
	{
		return -1;
	}
	pstEnv->iDstClanCount = iCount;	// Ŀ�깫����
	if (iCount > 0)
	{
		pstEnv->pstClanDest = sht_create(iCount*3, iCount,NAME_LEN, NULL);
		if (NULL == pstEnv->pstClanDest)
		{
			printf("init_sht error \n");
			return -1;
		}

		pstEnv->pstOverlapClan = sht_create(iCount*3, iCount,NAME_LEN, NULL);
		if (NULL == pstEnv->pstOverlapClan)
		{
			printf("init_sht error \n");
			return -1;
		}
	}
	return 0;
}

static int init_account_sht(UNITEENV* pstEnv)
{
	unsigned int iCount = 0;

	if (tdr_get_records_count(pstEnv->hDstAccHandle, "Account",NULL, &iCount) < 0)
	{
		return -1;
	}

	if (iCount <= 0) return -1;

	pstEnv->pstDstAccSht = sht_create(iCount*3, iCount,sizeof(ACCDATA), NULL);
	if (NULL == pstEnv->pstDstAccSht)
	{
		printf("init_sht error \n");
		return -1;
	}

	pstEnv->pstOverlapAccSht = sht_create(iCount*3, iCount,sizeof(ACCUIN), NULL);
	if (NULL == pstEnv->pstOverlapAccSht)
	{
		printf("init_sht error \n");
		return -1;
	}
	return 0;
}

/*static int get_acc_max_uin(UNITEENV* pstEnv)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	int err;
	
	MYSQL *conn = mysql_init(NULL);
	if(!conn)
		return -1;

	if(!mysql_real_connect(
		conn,
		pstEnv->stConf.szDstAccDBConnect,
		pstEnv->stConf.szDstAccDBUser,
		pstEnv->stConf.szDstAccDBPasswd,
		pstEnv->stConf.szDstAccDBName,
		0,
		NULL,
		CLIENT_COMPRESS))
	{
		goto end;
	}

	err = mysql_query(conn, "SELECT max(Uin) FROM Account");
	if(err)
		goto end;

	res = mysql_store_result(conn);
	if(!res)
		goto end;

	row = mysql_fetch_row(res);
	if (row == NULL)
		goto end;

	pstEnv->iDstMaxUin = atoi(row[0]);
	printf("Dst max Uin = %d\n", pstEnv->iDstMaxUin);
	if (conn)
		mysql_close(conn);
	return 0;

end:
	if (conn)
		mysql_close(conn);
	return -1;
}*/

// ɾ����˵Ŀո�
int account_del_space(char *pszSrc)
{
	char *p;
	int i;	
	int iLen;
	int iMdf = 0;
	char szTmp[51];

	strcpy(szTmp, pszSrc);
	
	p  = szTmp;
	/*iLen = strlen(p);
	for (i=0; i<iLen; i++)
	{
		if ((*p) != ' ')
		{
			break;
		}

		p++;
		iMdf = 1;
	}*/

	iLen = strlen(p);
	for (i=iLen-1; i>=0; i--)
	{
		if ((*(p+i)) != ' ')
		{
			break;	
		}	

		*(p+i) = 0;
		iMdf = 1;
	}

	if (iMdf)
	{
		iLen = strlen(p);
		strcpy(pszSrc, p);
		pszSrc[iLen] = 0;
	}
	return iMdf;
}

static int update_db_of_account_pre(UNITEENV* pstEnv)
{
	ACCOUNT stAcc;
	TDRDATA stSql;
	int iRet;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iNumRows = 0;
	int i,j;
	ACCOUNT *pstAcc;
	char szSql[102400];

	stSql.iBuff = sizeof(szSql);
	stSql.pszBuff = &szSql[0];
	
	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "Account");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stAcc);
	pstEnv->stDestDBObj.pszObj = (char*)&stAcc;
	
	iRet = tdr_obj2sql(pstEnv->hDstAccHandle, &stSql, TDR_DBOP_SELECT, 
					&pstEnv->stDestDBObj,"where Pass9Account like '% %'");
	if (0 != iRet)
	{
		printf("���ɲ�ѯ��Դ��¼��SQL���ʧ��: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(pstEnv->hDestDBHandle));
		return iRet;
	}

	if(tdr_query(&hDBResult, pstEnv->hDstAccHandle, &stSql) != 0)
	{
		printf("ִ��SQL���ʧ��!\n");
		return -1;
	}
	

	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows < 1)
	{
		goto src;
	}

	for(i=0; i<iNumRows; i++ )
	{
		iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
		if (0 != iRet)
		{	
			printf("tdr_fetch_row error\n");
			return -1;
		}
		
		pstAcc = (ACCOUNT*)pstEnv->stDestDBObj.pszObj;
		if (account_del_space(pstAcc->Pass9Account))
		{
			if (update_account_db(pstEnv, &pstEnv->stDestDBObj,1) < 0)
			{
				return -1;
			}
		}
	}
src:

	stSql.iBuff = sizeof(szSql);
	stSql.pszBuff = &szSql[0];

	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "Account");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stAcc);
	pstEnv->stDestDBObj.pszObj = (char*)&stAcc;
	
	iRet = tdr_obj2sql(pstEnv->hSrcAccHandle, &stSql, TDR_DBOP_SELECT, 
					&pstEnv->stDestDBObj,"where Pass9Account like '% %'");
	if (0 != iRet)
	{
		printf("���ɲ�ѯ��Դ��¼��SQL���ʧ��: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(pstEnv->hSrcDBHandle));
		return iRet;
	}

	if(tdr_query(&hDBResult, pstEnv->hSrcAccHandle, &stSql) != 0)
	{
		printf("ִ��SQL���ʧ��!\n");
		return -1;
	}
	
	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows < 1)
	{
		return 0;
	}

	for(i=0; i<iNumRows; i++ )
	{
		iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
		if (0 != iRet)
		{	
			printf("tdr_fetch_row error\n");
			return -1;
		}
		
		pstAcc = (ACCOUNT*)pstEnv->stDestDBObj.pszObj;
		if (account_del_space(pstAcc->Pass9Account))
		{
			if (update_account_db(pstEnv, &pstEnv->stDestDBObj,0) < 0)
			{
				return -1;
			}
		}
	}
	return 0;
}

static int update_db_of_account(UNITEENV* pstEnv)
{
	/*
		1.	��ȡĿ��account��,�Ȱ�Ŀ��account���˺ź�uin���浽��ϣ��(pstEnv->pstDstAccSht)
		2.	��ȡԴaccount ,   ����pstEnv->pstDstAccSht,�����ǲ������ظ����˺�
		3.	������ظ����˺�,��Դaccount�е�uin��ΪĿ��account�е�uin
			���µ�uin��ԭ����uin��������ϣ��(pstEnv->pstOverlapAccSht),
			����������ʱ,����pstEnv->pstOverlapAccSht
		4.	Դaccount����Ŀ��account��
	*/
	
	ACCOUNT stAcc;
	ACCOUNT *pstAcc;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	int i;
	char szSql[102400];
	ACCDATA stAccData;
	ACCDATA *pstAccData;
	ACCUIN stAccUin;
	ACCUIN *pstAccUin;
	int iMaxUin = 0;

	stSql.iBuff = sizeof(szSql);
	stSql.pszBuff = &szSql[0];
	
	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "Account");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stAcc);
	pstEnv->stDestDBObj.pszObj = (char*)&stAcc;
	
	iRet = tdr_obj2sql(pstEnv->hDstAccHandle, &stSql, TDR_DBOP_SELECT, &pstEnv->stDestDBObj,"where Uin!='0'");
	if (0 != iRet)
	{
		printf("���ɲ�ѯ��Դ��¼��SQL���ʧ��: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(pstEnv->hDestDBHandle));
		return iRet;
	}

	if(tdr_query(&hDBResult, pstEnv->hDstAccHandle, &stSql) != 0)
	{
		printf("ִ��SQL���ʧ��!\n");
		return -1;
	}
	

	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows < 1)
	{
		return 0;
	}

	for(i=0; i<iNumRows; i++ )
	{
		iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
		if (0 != iRet)
		{	
			printf("tdr_fetch_row error\n");
			return -1;
		}
		
		pstAcc = (ACCOUNT*)pstEnv->stDestDBObj.pszObj;
		if (iMaxUin < pstAcc->Uin)
			iMaxUin = pstAcc->Uin;
		
		strncpy(stAccData.szPass9,pstAcc->Pass9Account,sizeof(stAccData.szPass9));
		// keyΪPass9Account
		pstAccData = sht_insert_unique(pstEnv->pstDstAccSht,&stAccData, AccDataCmp, AccDataCode);
		if (pstAccData)
		{
			pstAccData->iUin = pstAcc->Uin;
			strncpy(pstAccData->szPass9,pstAcc->Pass9Account,sizeof(pstAccData->szPass9));
		}
		else
		{
			return -1;
		}
	}

	pstEnv->iDstMaxUin = iMaxUin;
	printf("dst Account max uin = %d \n", iMaxUin);


	stSql.iBuff = sizeof(szSql);
	stSql.pszBuff = &szSql[0];
	iRet = tdr_obj2sql(pstEnv->hSrcAccHandle, &stSql, TDR_DBOP_SELECT, &pstEnv->stDestDBObj,"where Uin!='0'");
	if (0 != iRet)
	{
		printf("���ɲ�ѯ��Դ��¼��SQL���ʧ��: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(pstEnv->hDestDBHandle));
		return iRet;
	}

	if(tdr_query(&hDBResult, pstEnv->hSrcAccHandle, &stSql) != 0)
	{
		printf("ִ��SQL���ʧ��!\n");
		return -1;
	}
	
	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows < 1)
	{
		return 0;
	}

	for(i=0; i<iNumRows; i++ )
	{
		iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
		if (0 != iRet)
		{	
			printf("tdr_fetch_row error\n");
			return -1;
		}
		
		pstAcc = (ACCOUNT*)pstEnv->stDestDBObj.pszObj;
		strncpy(stAccData.szPass9,pstAcc->Pass9Account,sizeof(stAccData.szPass9));
		pstAccData = NULL;

		// ���ظ���Pass9Account ,������,��Ŀ�����ݿ�Ϊ׼
		if (pstEnv->pstDstAccSht)
		{
			pstAccData = sht_find(pstEnv->pstDstAccSht, &stAccData, AccDataCmp, AccDataCode);
		}
		
		if (pstAccData && pstEnv->pstOverlapAccSht)
		{
			stAccUin.iOldUin = pstAcc->Uin;
			
			// key Ϊ�ɵ�Uin
			pstAccUin = sht_insert_unique(pstEnv->pstOverlapAccSht,&stAccUin, AccUinCmp, AccUinCode);
			if (pstAccUin)
			{
				pstAccUin->iOldUin = pstAcc->Uin;
				pstAccUin->iNewUin = pstAccData->iUin;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			pstAcc->Uin += pstEnv->iDstMaxUin;
			if (insert_dest_db(pstEnv, &pstEnv->stDestDBObj, DB_FLAG_ACCOUNT) < 0)
			{
				printf("Insert Account Error\n");
				return -1;
			}
		}
	}

	// û���� ɾ��
	if (pstEnv->pstDstAccSht)
	{
		sht_destroy(&pstEnv->pstDstAccSht);
		pstEnv->pstDstAccSht = NULL;
	}
		
	return 0;
}

static int unite_init(UNITEENV* pstEnv)
{
	if (0 > tdr_load_metalib(&pstEnv->pstMetaLib, pstEnv->stConf.szMetaFile))
	{
		printf("load meta lib role_db_meta fail\n");
		return -1;
	}

	if (init_unite_dbhandle(pstEnv) < 0)
	{
		printf("init_unite_dbhandle error\n");
		return -1;
	}
	return 0;
}



static int insert_sht(UNITEENV* pstEnv, NAMEID *pstNameID, int iFlag)
{
	char *pszName=NULL;
	if (iFlag == DEST_DB)
	{
		if (pstEnv->pstNameDest)
			pszName = sht_insert_unique(pstEnv->pstNameDest,pstNameID->Name, UniteHashCmp, UniteIDHashCode);
		
	}
	else if(iFlag == SRC_DB)
	{
		if (pstEnv->pstOverlapName)
			pszName = sht_insert_unique(pstEnv->pstOverlapName,pstNameID->Name, UniteHashCmp, UniteIDHashCode);
	}
	else
	{
		return -1;
	}

	if (!pszName)
	{
		printf("sht_insert_unique error\n");
		return -1;
	}
	else
	{
		strncpy(pszName, pstNameID->Name,sizeof(pstNameID->Name));
	}
	return 0;
}

static int load_nameid_db(UNITEENV* pstEnv, int iFlag)
{
	NAMEID stNameID,stNameID1;
	NAMEID *pstNameID;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	int i;
	char szSql[10240];
	char *pszName;
	int iCount = 0;
	TDRDBHANDLE hDBHandle;
	
	stSql.iBuff = sizeof(szSql);
	stSql.pszBuff = &szSql[0];
	
	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "NameID");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stNameID);
	pstEnv->stDestDBObj.pszObj = (char*)&stNameID;

	if (iFlag == DEST_DB)
	{
		hDBHandle = pstEnv->hDestDBHandle;
	}
	else if (iFlag == SRC_DB)
	{
		hDBHandle = pstEnv->hSrcDBHandle;
	}
	else
	{
		return -1;
	}

	iRet = tdr_obj2sql(hDBHandle, &stSql, TDR_DBOP_SELECT, &pstEnv->stDestDBObj, "WHERE GID!='0'");
	if (0 != iRet)
	{
		printf("���ɲ�ѯ��Դ��¼��SQL���ʧ��: %s dbms error:%s\n",
					tdr_error_string(iRet), tdr_dbms_error(hDBHandle));
		return iRet;
	}

	if(tdr_query(&hDBResult, hDBHandle, &stSql) != 0)
	{
		printf("ִ��SQL���ʧ��!\n");
		return -1;
	}

	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows < 1)
	{
		return -1;
	}

	for(i=0; i<iNumRows; i++ )
	{
		iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
		if (0 != iRet)
		{	
			printf("tdr_fetch_row error\n");
			return -1;
		}
		
		pstNameID = (NAMEID*)pstEnv->stDestDBObj.pszObj;
		//stNameID1 = *pstNameID;
		//strlwr(stNameID1.Name); // ��ĸת��Сд
		
		if (iFlag == DEST_DB)
		{
			if (insert_sht(pstEnv, pstNameID, iFlag) < 0)
			{
				return -1;
			}
		}
		else if(iFlag == SRC_DB)
		{
			iRet = insert_dest_db(pstEnv, &pstEnv->stDestDBObj, DB_FLAG_WORLD);
			if (iRet < 0)
			{
				if (iRet == -2)
				{
					printf("��ɫ��:%s  �ظ�.....\n",pstNameID->Name);
					//��������
					if (insert_sht(pstEnv, pstNameID, iFlag) < 0)
					{
						return -1;
					}

					// �޸�NameID��
					modify_role_name(pstEnv,pstNameID->Name, sizeof(pstNameID->Name), NULL);
					if (insert_dest_db(pstEnv, &pstEnv->stDestDBObj, DB_FLAG_WORLD) < 0)
					{
						return -1;
					}
				}
				else
				{
					printf("Insert NameID Error\n");
					return -1;
				}
			}
			/*if (pstEnv->pstNameDest)
			{
				pszName = sht_find(pstEnv->pstNameDest, stNameID1.Name, UniteHashCmp, UniteIDHashCode);
				if (pszName)
				{
					//��������
					if (insert_sht(pstEnv, &stNameID1, iFlag) < 0)
					{
						return -1;
					}

					// �޸�NameID��
					modify_role_name(pstEnv,pstNameID->Name, sizeof(pstNameID->Name), NULL);
				}
			}
			
			// ��ɫ���޸�֮��  ,����Ŀ�����ݿ�
			if (insert_dest_db(pstEnv, &pstEnv->stDestDBObj, DB_FLAG_WORLD) < 0)
			{
				printf("Insert NameID Error\n");
				return -1;
			}*/
		}
		
		iCount ++;
		if (iCount % 500 == 0)
		{
			printf("���ڴ���NameID��!!\n");
		}
	}
	
	return 0;
}

static int load_clan_db(UNITEENV* pstEnv, int iFlag)
{
	CLAN stClan;
	CLAN *pstClan;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	int i;
	char szSql[409600];
	char *pszName = NULL;
	
	stSql.iBuff = sizeof(szSql);
	stSql.pszBuff = &szSql[0];
	
	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "Clan");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stClan);
	pstEnv->stDestDBObj.pszObj = (char*)&stClan;

	if (iFlag == DEST_DB)
	{
		iRet = tdr_obj2sql(pstEnv->hDestDBHandle, &stSql, TDR_DBOP_SELECT, &pstEnv->stDestDBObj,"where GID!='0'");
		if (0 != iRet)
		{
			printf("���ɲ�ѯ��Դ��¼��SQL���ʧ��: %s dbms error:%s\n",
				tdr_error_string(iRet), tdr_dbms_error(pstEnv->hDestDBHandle));
			return iRet;
		}

		if(tdr_query(&hDBResult, pstEnv->hDestDBHandle, &stSql) != 0)
		{
			printf("ִ��SQL���ʧ��!\n");
			return -1;
		}
	}
	else if(iFlag == SRC_DB)
	{
		iRet = tdr_obj2sql(pstEnv->hSrcDBHandle, &stSql, TDR_DBOP_SELECT, &pstEnv->stDestDBObj, "where GID!='0'");
		if (0 != iRet)
		{
			printf("���ɲ�ѯ��Դ��¼��SQL���ʧ��: %s dbms error:%s\n",
				tdr_error_string(iRet), tdr_dbms_error(pstEnv->hDestDBHandle));
			return iRet;
		}

		if(tdr_query(&hDBResult, pstEnv->hSrcDBHandle, &stSql) != 0)
		{
			printf("ִ��SQL���ʧ��!\n");
			return -1;
		}
	}
	else
	{
		return -1;
	}

	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows < 1)
	{
		return 0;
	}

	for(i=0; i<iNumRows; i++ )
	{
		iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
		if (0 != iRet)
		{	
			printf("tdr_fetch_row error\n");
			return -1;
		}
		
		pstClan = (CLAN*)pstEnv->stDestDBObj.pszObj;

		if (iFlag == DEST_DB)
		{
			if (!pstEnv->pstClanDest)
				return -1;
			
			pszName = sht_insert_unique(pstEnv->pstClanDest,pstClan->ClanProf.Name, UniteHashCmp, UniteIDHashCode);
			if (!pszName)
			{
				printf("sht_insert_unique error\n");
				return -1;
			}
			else
			{
				strncpy(pszName, pstClan->ClanProf.Name,sizeof(pstClan->ClanProf.Name));
			}
			
		}
		else if(iFlag == SRC_DB)
		{
			if (pstEnv->pstClanDest)
			{
				pszName = sht_find(pstEnv->pstClanDest, pstClan->ClanProf.Name, UniteHashCmp, UniteIDHashCode);
				if (pszName)
				{
					if (!pstEnv->pstOverlapClan)
						return -1;
					
					pszName = sht_insert_unique(pstEnv->pstOverlapClan,pstClan->ClanProf.Name, UniteHashCmp, UniteIDHashCode);
					if (!pszName)
					{
						printf("sht_insert_unique error\n");
						return -1;
					}
					else
					{
						strncpy(pszName, pstClan->ClanProf.Name,sizeof(pstClan->ClanProf.Name));
					}
				}
			}
			
		}
		else
		{
			return -1;
		}
			
	}

	return 0;
}


static int delete_db_data(UNITEENV* pstEnv)
{
	NAMEID stNameID;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	char szSql[10240];
	
	stSql.iBuff = sizeof(szSql);
	stSql.pszBuff = &szSql[0];
	
	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "NameID");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stNameID);
	pstEnv->stDestDBObj.pszObj = (char*)&stNameID;
	
	iRet = tdr_obj2sql(pstEnv->hDestDBHandle, &stSql, TDR_DBOP_DELETE, &pstEnv->stDestDBObj, "WHERE NameType='99'");
	if (0 != iRet)
	{
		printf("create delete sql error\n");
		return iRet;
	}
	
	if(tdr_query(&hDBResult, pstEnv->hDestDBHandle, &stSql) != 0)
	{
		printf("TDR_DBOP_DELETE error!\n");
		return -1;
	}

	iRet = tdr_obj2sql(pstEnv->hSrcDBHandle, &stSql, TDR_DBOP_DELETE, &pstEnv->stDestDBObj, "WHERE NameType='99'");
	if (0 != iRet)
	{
		printf("create delete sql error\n");
		return iRet;
	}
	
	if(tdr_query(&hDBResult, pstEnv->hSrcDBHandle, &stSql) != 0)
	{
		printf("TDR_DBOP_DELETE error!\n");
		return -1;
	}
	return 0;
}

static int shttab_print(FILE* fp, const void* pvData)
{
	fwrite(pvData, 32, 1, fp);
	return 0;
}

static int sht_list(LPSHTABLE pstSht)
{
	FILE *fp;

	fp = fopen("./test.txt","w");
	sht_dump_all(pstSht, fp, shttab_print);		
	fclose(fp);
	return 0;
}

static int display_sht(LPSHTABLE pstSht)
{
	int i;
	int iValid;
	char *pszName;

	for (i=0; i<pstSht->iItem; i++)
	{
		pszName = sht_pos(pstSht, i, &iValid);
		if (iValid == 0)
		{
			printf("display_sht error\n");
			return -1;
		}
		printf("%s\n",pszName);
	}
	return 0;
}


static void unite_destory_db_handle(UNITEENV* pstEnv)
{
	if (pstEnv->hDestDBHandle)
	{
		tdr_close_dbhanlde(pstEnv->hDestDBHandle);
	}

	if (pstEnv->hSrcDBHandle)
	{
		tdr_close_dbhanlde(pstEnv->hSrcDBHandle);
	}

	if (pstEnv->hSrcUpDBHandle)
	{
		tdr_close_dbhanlde(pstEnv->hSrcUpDBHandle);
	}

	if (pstEnv->fp)
	{
		fclose(pstEnv->fp);
	}
}

static void unite_destory_sht(UNITEENV* pstEnv)
{
	if (pstEnv->pstNameDest)
	{
		sht_destroy(&pstEnv->pstNameDest);
		pstEnv->pstNameDest = NULL;
	}

	if (pstEnv->pstOverlapName)
	{
		sht_destroy(&pstEnv->pstOverlapName);
		pstEnv->pstOverlapName = NULL;
	}

	if (pstEnv->pstOverlapClan)
	{
		sht_destroy(&pstEnv->pstOverlapClan);
		pstEnv->pstOverlapClan = NULL;
	}

	if (pstEnv->pstClanDest)
	{
		sht_destroy(&pstEnv->pstClanDest);
		pstEnv->pstClanDest = NULL;
	}

	if (pstEnv->pstDstAccSht)
	{
		sht_destroy(&pstEnv->pstDstAccSht);
		pstEnv->pstDstAccSht = NULL;
	}

	if (pstEnv->pstOverlapAccSht)
	{
		sht_destroy(&pstEnv->pstOverlapAccSht);
		pstEnv->pstOverlapAccSht = NULL;
	}

	if (pstEnv->pstWorldAccSht)
	{
		sht_destroy(&pstEnv->pstWorldAccSht);
		pstEnv->pstWorldAccSht = NULL;
	}

	if (pstEnv->pstUinMudExpSht)
	{
		sht_destroy(&pstEnv->pstUinMudExpSht);
		pstEnv->pstUinMudExpSht = NULL;
	}
}

static void unite_destory_sht_acc_uin(UNITEENV* pstEnv)
{
	if (pstEnv->pstSrcAccUinSht)
	{
		sht_destroy(&pstEnv->pstSrcAccUinSht);
		pstEnv->pstSrcAccUinSht = NULL;
	}

	if (pstEnv->pstDstAccUinSht)
	{
		sht_destroy(&pstEnv->pstDstAccUinSht);
		pstEnv->pstDstAccUinSht = NULL;
	}
}



static int updata_roledata(UNITEENV* pstEnv, ROLEDATA *pstRoleData)
{
	int i;
	ROLEMISCINFO *pstMisc = &pstRoleData->MiscInfo;
	ROLEENEMYMEMBER *pstBlackMember = pstMisc->BlackMember;
	ROLEBUDDY *pstBuddy =  &pstRoleData->Buddy;
	CLANID *pstClanID = &pstMisc->ClanInfo.ClanId;
	int iRet = 0;
	ROLESCHOOL *pstSchoool = &pstRoleData->SyncRelation.School;

	modify_role_name(pstEnv,pstRoleData->RoleName, sizeof(pstRoleData->RoleName), &iRet);
	modify_role_uin(pstEnv, &pstRoleData->Uin);

	// ����ID����,��¼����
	pstRoleData->MiscInfo.Store.ID = 0;

	// ��԰ID��0,��¼ʱ���
	pstRoleData->MiscInfo.Home.ID = 0;
	
	// �޸�worldID
	pstRoleData->WorldID = pstEnv->stConf.iDestWorldID;
	
	for (i=0; i<pstMisc->BlackNum; i++)
	{
		modify_role_name(pstEnv,pstBlackMember[i].RoleName, sizeof(pstBlackMember[i].RoleName), &iRet);
	}

	for (i=0; i<pstBuddy->BrotherNum; i++)
	{
		modify_role_name(pstEnv,pstBuddy->Brothers[i].RoleName, sizeof(pstBuddy->Brothers[i].RoleName), &iRet);
	}

	for (i=0; i<pstBuddy->FriendNum; i++)
	{
		modify_role_name(pstEnv,pstBuddy->Friends[i].RoleName, sizeof(pstBuddy->Friends[i].RoleName), &iRet);
	}

	for (i=0; i<pstBuddy->MasterNum; i++)
	{
		modify_role_name(pstEnv,pstBuddy->Master[i].RoleName, sizeof(pstBuddy->Master[i].RoleName), &iRet);
	}

	for (i=0; i<pstBuddy->SpouseNum; i++)
	{
		modify_role_name(pstEnv,pstBuddy->Spouse[i].RoleName, sizeof(pstBuddy->Spouse[i].RoleName), &iRet);
	}

	for (i=0; i<pstBuddy->PrenticeNum; i++)
	{
		modify_role_name(pstEnv,pstBuddy->Prentices[i].RoleName, sizeof(pstBuddy->Prentices[i].RoleName), &iRet);
	}

	for (i=0; i<pstMisc->Enemy.EnemyNum; i++)
	{
		modify_role_name(pstEnv,pstMisc->Enemy.Enemys[i].RoleName, sizeof(pstMisc->Enemy.Enemys[i].RoleName), &iRet);
	}

	if (ROLE_SCHOOL_TYPE_TEACHER == pstSchoool->JobType)
	{
		for (i=0; i<pstSchoool->Data.RoleTeacher.Num; i++)
		{
			modify_role_name(pstEnv,pstSchoool->Data.RoleTeacher.Students[i].RoleName,
								sizeof(pstSchoool->Data.RoleTeacher.Students[i].RoleName), &iRet);
			modify_clan_name(pstEnv,pstSchoool->Data.RoleTeacher.Students[i].ClanName,
								sizeof(pstSchoool->Data.RoleTeacher.Students[i].ClanName), &iRet);
		}
		
	}
	else if (ROLE_SCHOOL_TYPE_STUDENT== pstSchoool->JobType)
	{
		modify_role_name(pstEnv,pstSchoool->Data.RoleStudent.Teacher.RoleName,
								sizeof(pstSchoool->Data.RoleStudent.Teacher.RoleName), &iRet);
		modify_clan_name(pstEnv,pstSchoool->Data.RoleStudent.Teacher.ClanName,
								sizeof(pstSchoool->Data.RoleStudent.Teacher.ClanName), &iRet);
	}
	

	// �޸Ľ�ɫ������Ϣ
	if (pstClanID->GID > 0)
	{
		pstClanID->Id += pstEnv->iDestClanLastIdx;
		modify_clan_name(pstEnv,pstClanID->Name, sizeof(pstClanID->Name), &iRet);
		iRet = 1;
	}
	return iRet;
}

static int acc_updata_src_db(UNITEENV* pstEnv, TDRDBOBJECT *pstDBObj, TDRDBHANDLE hDBHandle)
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
		printf("���ɸ�����Դ��¼��SQL���ʧ��: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(hDBHandle));
		return iRet;
	}

	iRet = tdr_query(&hDBUpResult, hDBHandle, &stSql);
	if (0 != iRet)
	{
		printf("ִ�и�����Դ��¼��SQL���ʧ��: %s dbms error:%s\n",
				tdr_error_string(iRet), tdr_dbms_error(hDBHandle));		
	}

	if (hDBUpResult)
	{
		tdr_free_dbresult(&hDBUpResult);
	}
	return iRet;
}

static int updata_src_db(UNITEENV* pstEnv, TDRDBOBJECT *pstDBObj)
{
//	TDRDBOBJECT stDBObj;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBUpResult = 0;
	int iRet = 0;
	char szSql[409600];
	
	stSql.pszBuff = &szSql[0];
	stSql.iBuff = sizeof(szSql);
	
	iRet = tdr_obj2sql(pstEnv->hSrcUpDBHandle, &stSql, TDR_DBOP_UPDATE, pstDBObj, NULL);
	
	if (0 != iRet)
	{
		printf("���ɸ�����Դ��¼��SQL���ʧ��: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(pstEnv->hSrcUpDBHandle));
		return iRet;
	}

	iRet = tdr_query(&hDBUpResult, pstEnv->hSrcUpDBHandle, &stSql);
	//tdr_free_dbresult(&hDBUpResult);
	if (0 != iRet)
	{
		printf("ִ�и�����Դ��¼��SQL���ʧ��: %s dbms error:%s\n",
				tdr_error_string(iRet), tdr_dbms_error(pstEnv->hSrcUpDBHandle));		
	}

	return 0;
}

int update_account_db(UNITEENV* pstEnv, TDRDBOBJECT *pstDBObj, int iFlag)
{
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBUpResult = 0;
	int iRet = 0;
	char szSql[409600];
	TDRDBHANDLE hDestUpDBHandle;
	
	stSql.pszBuff = &szSql[0];
	stSql.iBuff = sizeof(szSql);

	if (iFlag)
	{
		hDestUpDBHandle = pstEnv->hDstUpAccHandle;
	}
	else
	{
		hDestUpDBHandle = pstEnv->hSrcUpAccHandle;	
	}
	
	iRet = tdr_obj2sql(hDestUpDBHandle, &stSql, TDR_DBOP_UPDATE, pstDBObj, NULL);
	if (0 != iRet)
	{
		printf("���ɸ�����Դ��¼��SQL���ʧ��: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(hDestUpDBHandle));
		return iRet;
	}
	stSql.iBuff = strlen(szSql);

	iRet = tdr_query(&hDBUpResult, hDestUpDBHandle, &stSql);
	if (0 != iRet)
	{
		printf("ִ�и�����Դ��¼��SQL���ʧ��: %s dbms error:%s\n",
				tdr_error_string(iRet), tdr_dbms_error(hDestUpDBHandle));	
		return iRet;
	}

	return 0;
}

static int updata_dest_db(UNITEENV* pstEnv, TDRDBOBJECT *pstDBObj)
{
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBUpResult = 0;
	int iRet = 0;
	char szSql[409600];
	
	stSql.pszBuff = &szSql[0];
	stSql.iBuff = sizeof(szSql);
	
	iRet = tdr_obj2sql(pstEnv->hDestUpDBHandle, &stSql, TDR_DBOP_UPDATE, pstDBObj, NULL);
	
	if (0 != iRet)
	{
		printf("���ɸ�����Դ��¼��SQL���ʧ��: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(pstEnv->hDestUpDBHandle));
		return iRet;
	}
	stSql.iBuff = strlen(szSql);

	iRet = tdr_query(&hDBUpResult, pstEnv->hDestUpDBHandle, &stSql);
	//tdr_free_dbresult(&hDBUpResult);
	if (0 != iRet)
	{
		printf("ִ�и�����Դ��¼��SQL���ʧ��: %s dbms error:%s\n",
				tdr_error_string(iRet), tdr_dbms_error(pstEnv->hDestUpDBHandle));	
		return iRet;
	}

	return 0;
}



static int insert_dest_db(UNITEENV* pstEnv, TDRDBOBJECT *pstDBObj, int iDBFlag)
{
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBUpResult = 0;
	int iRet = 0;
	char *pszSql = malloc(409600);
	TDRDBHANDLE hDBHandle;

	if (!pszSql)
		return -1;
	
	stSql.pszBuff = pszSql;
	stSql.iBuff = 409600;

	if (iDBFlag == DB_FLAG_WORLD) hDBHandle = pstEnv->hDestUpDBHandle;
	else if (iDBFlag == DB_FLAG_ACCOUNT) hDBHandle = pstEnv->hDstUpAccHandle;
	else if (iDBFlag == DB_FLAG_BILL) hDBHandle = pstEnv->hDstBillHandle;
	else return -1;
		
	iRet = tdr_obj2sql(hDBHandle, &stSql, TDR_DBOP_INSERT, pstDBObj, NULL);

	if (0 != iRet)
	{
		printf("���ɲ����¼��SQL���ʧ��: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(hDBHandle));
		free(pszSql);
		return iRet;
	}

	iRet = tdr_query(&hDBUpResult, hDBHandle, &stSql);
	tdr_free_dbresult(&hDBUpResult);
	if (0 != iRet)
	{
		int iDbmsErrNo = tdr_dbms_errno(hDBHandle);	
		free(pszSql);

		//printf("ִ�в����¼��SQL���ʧ��: %s dbms error:%s\n",
		//		tdr_error_string(iRet), tdr_dbms_error(hDBHandle));	
		if (1022 == iDbmsErrNo || 1062  == iDbmsErrNo || 1169 == iDbmsErrNo)
		{
			return -2;
		}
		else
		{
			return -1;
		}
	}

	free(pszSql);
	return 0;
}

static int update_dst_db(UNITEENV* pstEnv, TDRDBOBJECT *pstDBObj, int iDBFlag)
{
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBUpResult = 0;
	int iRet = 0;
	char *pszSql = malloc(409600);
	TDRDBHANDLE hDBHandle;

	if (!pszSql)
		return -1;
	
	stSql.pszBuff = pszSql;
	stSql.iBuff = 409600;

	if (iDBFlag == DB_FLAG_WORLD) hDBHandle = pstEnv->hDestUpDBHandle;
	else if(iDBFlag == DB_FLAG_ACCOUNT) hDBHandle = pstEnv->hDstUpAccHandle;
	else if (iDBFlag == DB_FLAG_BILL) hDBHandle = pstEnv->hDstUpBillHandle;
	else if (DB_FLAG_REGION == iDBFlag)
	{
		hDBHandle = pstEnv->hDstUpRegionHandle;
	}
	else return -1;
		
	iRet = tdr_obj2sql(hDBHandle, &stSql, TDR_DBOP_UPDATE, pstDBObj, NULL);
	if (0 != iRet)
	{
		printf("���ɸ��¼�¼��SQL���ʧ��: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(hDBHandle));
		free(pszSql);
		return iRet;
	}

	iRet = tdr_query(&hDBUpResult, hDBHandle, &stSql);
	tdr_free_dbresult(&hDBUpResult);
	if (0 != iRet)
	{
		printf("ִ�и��¼�¼��SQL���ʧ��: %s dbms error:%s\n",
				tdr_error_string(iRet), tdr_dbms_error(hDBHandle));	
		free(pszSql);
		return iRet;
	}

	free(pszSql);
	return 0;
}

static int update_relation(UNITEENV* pstEnv, RELATION *pstRelation)
{
	int i;
	int iRet = 0;

	for (i=0; i<pstRelation->Buddy.BuddyNum; i++)
	{
		pstRelation->Buddy.BuddyInfos[i].ClanID += pstEnv->iDestClanLastIdx;
		modify_clan_name(pstEnv,pstRelation->Buddy.BuddyInfos[i].ClanName, 
							sizeof(pstRelation->Buddy.BuddyInfos[i].ClanName), &iRet);
	}

	return iRet;
}

static int update_db_of_relation(UNITEENV* pstEnv)
{
	int i;
	RELATION stRelation;
	RELATION *pstRelation;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	char szSql[102400];
	char szBuff[102400];
	int iCount = 0;

	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "Relation");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stRelation);
	pstEnv->stDestDBObj.pszObj = (char*)&stRelation;

	printf("���ڴ���Relation��\n");
	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM Relation");
	stSql.iBuff = strlen(stSql.pszBuff);

	iRet = tdr_query(&hDBResult, pstEnv->hSrcDBHandle, &stSql);
	if (0 != iRet)
	{
		unite_error_log(pstEnv->fp,"src db execute sql sentence:SELECT * FROM Relation error\n");
		return -1;
	}
	
	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows < 1)
	{
		return 0;
	}

	for (i=0; i<iNumRows; i++)
	{
		memset (&stRelation, 0, sizeof(stRelation));
		iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
		if (0 != iRet)
		{	
			unite_error_log(pstEnv->fp,"src Relation table:tdr_fetch_row error\n");
			continue;
		}

		pstRelation = (RELATION*)pstEnv->stDestDBObj.pszObj;
		if (update_relation(pstEnv, pstRelation))
		{
			//if (updata_src_db(pstEnv, &pstEnv->stDestDBObj)< 0)
			//{
			//	return -1;
			//}
		}

		if (insert_dest_db(pstEnv, &pstEnv->stDestDBObj,DB_FLAG_WORLD) < 0)
		{
			printf("Insert Relation Error\n");
			snprintf(szBuff,sizeof(szBuff),"dest Relation table: insert error! GID=%lld\n", pstRelation->GID);
			unite_error_log(pstEnv->fp,szBuff);
			continue;
		}
		
		iCount ++;
		if (iCount %500 == 0)
		{
			printf("���ڴ���Relation��\n");
		}
	}

	printf("Relation�����%d����¼\n",iCount);
	return 0;
}

static int update_clan(UNITEENV* pstEnv, CLAN*pstClan)
{
	int iRet = 0;
	int i;

	CLANBRIEF *pstClanBrief = &pstClan->ClanBrief;
	CLANINNER *pstClanInner = &pstClan->ClanInner;

	pstClan->ClanProf.Id += pstEnv->iDestClanLastIdx;
	modify_clan_name(pstEnv,pstClan->ClanProf.Name,sizeof(pstClan->ClanProf.Name), &iRet);

	modify_role_name(pstEnv, pstClanBrief->Leader.RoleName, sizeof(pstClanBrief->Leader.RoleName), &iRet);

	for (i=0; i<pstClanBrief->AssistNum; i++)
	{
		modify_role_name(pstEnv, pstClanBrief->Assists[i].RoleName, sizeof(pstClanBrief->Assists[i].RoleName), &iRet);
	}

	/*for (i=0; i<pstClanBrief->OfficerNum; i++)
	{
		modify_role_name(pstEnv, pstClanBrief->Officers[i].RoleName, strlen(pstClanBrief->Officers[i].RoleName), &iRet);
	}

	for (i=0; i<pstClanInner->ClanEnemyNum; i++)
	{
		modify_clan_name(pstEnv,pstClanInner->ClanEnemys[i].ClanId.Name,
							sizeof(pstClanInner->ClanEnemys[i].ClanId.Name), &iRet);
	}

	for (i=0; i<pstClanInner->ClanFriendNum; i++)
	{
		modify_clan_name(pstEnv,pstClanInner->ClanFriends[i].ClanId.Name,
							sizeof(pstClanInner->ClanFriends[i].ClanId.Name), &iRet);
	}*/
	pstClanInner->ClanApplyEnemyNum = pstClanInner->ClanApplyFriendNum = pstClanInner->ClanApplyNum = 0;

	for (i=0; i<(int)pstClan->ClanCore.MemNum; i++)
	{
		modify_role_name(pstEnv, pstClan->ClanCore.ClanMem[i].RoleMember.RoleName, 
							sizeof(pstClan->ClanCore.ClanMem[i].RoleMember.RoleName), &iRet);
	}

	for (i=0; i<(int)pstClan->ClanPet.Num; i++)
	{
		modify_role_name(pstEnv, pstClan->ClanPet.ClanPets[i].RoleName, 
							sizeof(pstClan->ClanPet.ClanPets[i].RoleName), &iRet);
	}
	return 1;
}

static int unite_clan_cmp(const void *p1, const void*p2)
{
	UINTECLANDATA *pstClan1 = (UINTECLANDATA *)p1;
	UINTECLANDATA *pstClan2 =  (UINTECLANDATA *)p2;

	// ����| �ȼ��ߵ���ǰ�� 
	if (pstClan1->usLvl != pstClan2->usLvl)
	{
		return pstClan2->usLvl - pstClan1->usLvl;
	}
	else
	{
		// �ȼ���ͬ����Ծ�Ƚ�������
		return pstClan2->uiActiveVal - pstClan1->uiActiveVal;
	}
}

static int store_store_cmp(const void *p1, const void*p2)
{
	UINTESTOREDATA *pstStore1 = (UINTESTOREDATA *)p1;
	UINTESTOREDATA *pstStore2 =  (UINTESTOREDATA *)p2;

	// ����| �ȼ��ߵ���ǰ�� 
	if (pstStore1->ullTax!= pstStore2->ullTax)
	{
		return pstStore2->ullTax - pstStore1->ullTax;
	}
	else
	{
		// �ȼ���ͬ����Ծ�Ƚ�������
		return pstStore2->ullExchg - pstStore1->ullExchg;
	}
}


tdr_ulonglong unite_get_gid(UNITEENV* pstEnv)
{
	unsigned short unTmp;
	tdr_ulonglong ullGid = 0;
	int t = time(NULL);
	static unsigned short unSeq = 1;

	memcpy(&ullGid, &t, sizeof(int));
	unTmp = (pstEnv->stConf.iDestRegionID << 10) + pstEnv->stConf.iDestWorldID;
	memcpy( ((char *)(&ullGid)+sizeof(int)), &unTmp, sizeof(unTmp)); 
	
	memcpy( ((char *)(&ullGid)+sizeof(ullGid)-sizeof(unSeq)), &unSeq, sizeof(unSeq)); 
	unSeq ++;
	
	return ullGid;
}

static int up_trade_make_mail(UNITEENV* pstEnv, char *pData, MAILENTRY *pstMailEntry,int iFlag)
{
	TRADEBUY *pstBuy;
	TRADESELL *pstSell;

	memset(pstMailEntry, 0, sizeof(*pstMailEntry));
	
	if (FLAG_TRADEBUY == iFlag)
	{
		pstBuy = (TRADEBUY *)pData;

		modify_role_name(pstEnv,pstBuy->RoleName, sizeof(pstBuy->RoleName), NULL);
		strncpy(pstMailEntry->Mail.Head.Recv, pstBuy->RoleName, sizeof(pstMailEntry->Mail.Head.Recv));
		strncpy(pstMailEntry->Mail.Head.Title, UNITE_TRADE1, sizeof(pstMailEntry->Mail.Head.Title));
		snprintf(pstMailEntry->Mail.Detail.Text, sizeof(pstMailEntry->Mail.Detail.Text), 
				UNITE_TRADE2,
				pstBuy->TradeID,pstBuy->Moeny, UNITE_TRADE3);
		pstMailEntry->Mail.Detail.Gold = pstBuy->Moeny;
		strncpy(pstMailEntry->Mail.Head.Send, UNITE_TRADE4, sizeof(pstMailEntry->Mail.Head.Send));
	}
	else if (FLAG_TRADESELL== iFlag)
	{
		pstMailEntry->Mail.Head.Flags |= MAIL_FLAG_GOLD;
		pstSell = (TRADESELL *)pData;

		modify_role_name(pstEnv,pstSell->RoleName, sizeof(pstSell->RoleName), NULL);
		strncpy(pstMailEntry->Mail.Head.Recv, pstSell->RoleName, sizeof(pstMailEntry->Mail.Head.Recv));
		strncpy(pstMailEntry->Mail.Head.Title, UNITE_TRADE5, sizeof(pstMailEntry->Mail.Head.Title));
		snprintf(pstMailEntry->Mail.Detail.Text,sizeof(pstMailEntry->Mail.Detail.Text), 
				UNITE_TRADE6,
				pstSell->TradeID,pstSell->Gold, UNITE_TRADE7);
		pstMailEntry->Mail.Detail.Gold = pstSell->Gold;
		strncpy(pstMailEntry->Mail.Head.Send, UNITE_TRADE8, sizeof(pstMailEntry->Mail.Head.Send));
	}
	else
	{
		return -1;
	}

	pstMailEntry->Mail.Head.Flags |= MAIL_FLAG_SYSTEM;
	pstMailEntry->Mail.Head.Time = time(NULL);
	pstMailEntry->Mail.Head.WID = unite_get_gid(pstEnv);
	pstMailEntry->Mail.Detail.ItemNum = 0;
		
	return 0;
}

static int up_trade_insert_mailentry(UNITEENV* pstEnv, MAILENTRY *pstMailEntry)
{	
	TDRDBOBJECT stDBObj;

	stDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "MailEntry");
	stDBObj.iVersion = tdr_get_meta_current_version(stDBObj.pstMeta);
	stDBObj.iObjSize = sizeof(*pstMailEntry);
	stDBObj.pszObj = (char*)pstMailEntry;

	if (insert_dest_db(pstEnv, &stDBObj,DB_FLAG_WORLD) < 0)
	{
		return -1;
	}
	return 0;
}

static int update_db_of_tradebuy(UNITEENV* pstEnv)
{
	TRADEBUY stBuy;
	//TRADEBUY *pstBuy;
	char szSql[1024];
	char szBuff[1024];
	TDRDATA stSql;
	int iCount = 0;
	int iRet;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iNumRows = 0;
	int i;
	MAILENTRY stMailEntry;

	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "TradeBuy");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stBuy);
	pstEnv->stDestDBObj.pszObj = (char*)&stBuy;

	printf("���ڴ���TradeBuy��\n");

	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM TradeBuy");
	stSql.iBuff = strlen(stSql.pszBuff);

	iRet = tdr_query(&hDBResult, pstEnv->hSrcDBHandle, &stSql);
	if (0 != iRet)
	{
		unite_error_log(pstEnv->fp,"src db execute sql sentence:SELECT * FROM TradeBuy error\n");
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
			unite_error_log(pstEnv->fp,"src TradeBuy table:tdr_fetch_row error\n");
			return -1;
		}

		if (up_trade_make_mail(pstEnv, pstEnv->stDestDBObj.pszObj, &stMailEntry, FLAG_TRADEBUY) < 0)
		{
			continue;
		}
		
		if (up_trade_insert_mailentry(pstEnv, &stMailEntry) < 0)
		{	
			return -1;
		}

		iCount ++;
		if (iCount %500 == 0)
		{
			printf("���ڴ���TradeBuy��\n");
		}
	}
	
	return 0;
}


static int update_db_of_tradesell(UNITEENV* pstEnv)
{
	TRADESELL stSell;
	//TRADEBUY *pstBuy;
	char szSql[1024];
	char szBuff[1024];
	TDRDATA stSql;
	int iCount = 0;
	int iRet;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iNumRows = 0;
	int i;
	MAILENTRY stMailEntry;

	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "TradeSell");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stSell);
	pstEnv->stDestDBObj.pszObj = (char*)&stSell;

	printf("���ڴ���TradeSell��\n");

	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM TradeSell");
	stSql.iBuff = strlen(stSql.pszBuff);

	iRet = tdr_query(&hDBResult, pstEnv->hSrcDBHandle, &stSql);
	if (0 != iRet)
	{
		unite_error_log(pstEnv->fp,"src db execute sql sentence:SELECT * FROM TradeSell error\n");
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
			unite_error_log(pstEnv->fp,"src TradeSell table:tdr_fetch_row error\n");
			return -1;
		}

		if (up_trade_make_mail(pstEnv, pstEnv->stDestDBObj.pszObj, &stMailEntry, FLAG_TRADESELL) < 0)
		{
			continue;
		}
		
		if (up_trade_insert_mailentry(pstEnv, &stMailEntry) < 0)
		{	
			return -1;
		}

		iCount ++;
		if (iCount %500 == 0)
		{
			printf("���ڴ���TradeSell��\n");
		}
	}
	
	return 0;
}

static int unite_find_shadow_lap_name(SNAME *pastSname, int iNum,char *pszName)
{
	int i;
	SNAME *pstSname;

	for (i=0; i<iNum; i++)
	{
		pstSname = pastSname+i;
		if (strcmp(pstSname->szName, pszName)==0)
		{
			return 1;
		}
	}
	return 0;
}

int init_accshare_sht(UNITEENV* pstEnv)
{
	int iRet = 0;
	unsigned int iCount;
	
	iRet = tdr_get_records_count(pstEnv->hDestDBHandle, "AccShare",NULL, &iCount);
	if (iRet < 0)
	{
		return 0;
	}

	if (iCount > 0)
	{
		pstEnv->pstWorldAccSht = sht_create(iCount*3, iCount, sizeof(ACCSHARE), NULL);
		if (NULL == pstEnv->pstWorldAccSht)
		{
			printf("init_sht:pstWorldAccSht error \n");
			return -1;
		}

		ACCSHARE stData;
		ACCSHARE* pstData;
		char szSql[1024];
		char szBuff[1024];
		TDRDATA stSql;
		int iCount = 0;
		int iRet;
		TDRDBRESULTHANDLE hDBResult = 0;
		int iNumRows = 0;
		int i;
	
		pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "AccShare");
		pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
		pstEnv->stDestDBObj.iObjSize = sizeof(stData);
		pstEnv->stDestDBObj.pszObj = (char*)&stData;

	
		stSql.pszBuff = &szSql[0];
		snprintf(szSql, sizeof(szSql),"SELECT * FROM AccShare");
		stSql.iBuff = strlen(stSql.pszBuff);
	
		iRet = tdr_query(&hDBResult, pstEnv->hDestDBHandle, &stSql);
		if (0 != iRet)
		{
			printf("execute sql sentence:SELECT * FROM AccShare error\n");
			return -1;
		}
		
		iNumRows = tdr_num_rows(hDBResult);
		if (iNumRows <= 0)
		{
			return 0;
		}
		
	
		for (i=0; i<iNumRows; i++)
		{
			pstData = NULL;
			iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
			if (0 != iRet)
			{	
				printf("init_accshare_sht table:tdr_fetch_row error i=%d \n", i);
				return -1;
			}
			
			pstData = sht_insert_unique(pstEnv->pstWorldAccSht,(ACCSHARE*)pstEnv->stDestDBObj.pszObj, 
									WorldAccCmp, WorldAccCode);
			if (!pstData)
			{
				printf("sht_insert_unique:pstWorldAccSht  error\n");
				return -1;
			}
			else
			{
				*pstData = *(ACCSHARE*)pstEnv->stDestDBObj.pszObj;
			}
		}
		
	}
	return 0;
}

int update_db_of_accshare(UNITEENV* pstEnv)
{
	ACCSHARE stAcc;
	ACCSHARE *pstAcc;
	ACCSHARE *pstAccSrc;
	char szSql[1024];
	char szBuff[1024];
	TDRDATA stSql;
	int iCount = 0;
	int iRet;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iNumRows = 0;
	int i;
	int iChg;
	int iSucc = -1;

	printf("���ڴ���AccShare��\n");
	if (init_accshare_sht(pstEnv) < 0)
	{
		printf("init_accshare_sht  error!\n");
		return -1;
	}

	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "AccShare");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stAcc);
	pstEnv->stDestDBObj.pszObj = (char*)&stAcc;

	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM AccShare;");
	stSql.iBuff = strlen(stSql.pszBuff);
	
	iRet = tdr_query(&hDBResult, pstEnv->hSrcDBHandle, &stSql);
	if (0 != iRet)
	{
		printf("execute sql sentence:SELECT * FROM AccShare error \n");
		goto end;
	}
	
	iNumRows = tdr_num_rows(hDBResult);
	if (iNumRows <= 0)
		goto end;

	for (i=0; i<iNumRows; i++)
	{
		iChg = 0;
		iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
		if (0 != iRet)
		{	
			goto end;
		}

		pstAccSrc = (ACCSHARE*)pstEnv->stDestDBObj.pszObj;
		
		pstAcc = sht_find(pstEnv->pstWorldAccSht, pstAccSrc, WorldAccCmp, WorldAccCode);
		if (pstAcc)
		{
			if (pstAcc->LastLogout == 0)
			{
				pstAcc->LastLogout =  time(NULL);
				iChg = 1;
			}
			else
			{
				if (pstAcc->LastLogout > pstAccSrc->LastLogout)
				{
					pstAcc->LastLogout = pstAccSrc->LastLogout;	
					iChg = 1;
				}	
			}

			if (pstAcc->GrabTime > pstAccSrc->GrabTime)
			{
				pstAcc->GrabTime = pstAccSrc->GrabTime;
				iChg = 1;
			}

			if (pstAcc->OfflineExp >  pstAccSrc->OfflineExp)
			{
				pstAcc->OfflineExp = pstAccSrc->OfflineExp;
				iChg = 1;
			}

			if (pstAcc->TakeMudExp > pstAccSrc->TakeMudExp)
			{
				pstAcc->TakeMudExp = pstAccSrc->TakeMudExp;	
				iChg = 1;
			}
			
			if (iChg)
			{
				if (update_dst_db(pstEnv,  &pstEnv->stDestDBObj,DB_FLAG_WORLD) < 0)
				{
					printf("Update AccShare Error\n");
					goto end;
				}
			}
		}
		else
		{
			if (insert_dest_db(pstEnv, &pstEnv->stDestDBObj,DB_FLAG_WORLD) < 0)
			{
				printf("Insert AccShare Error\n");
				goto end;
			}
		}
	}

	iSucc = 0;

end:

	if (pstEnv->pstWorldAccSht)
	{
		sht_destroy(&pstEnv->pstWorldAccSht);
		pstEnv->pstWorldAccSht = NULL;
	}
	return iSucc;
}

static int update_db_of_shadowroledata(UNITEENV* pstEnv)
{
	SHADOWACCDATA stData;
	SHADOWACCDATA* pstData;
	SHADOWACCDATA* pstDstData;
	char szSql[1024];
	char szBuff[1024];
	TDRDATA stSql;
	int iCount = 0;
	int iRet;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iNumRows = 0;
	int i;
	int iSucc=-1;

	printf("���ڴ���ShadowRoleData��\n");
	if (init_uinmudexp_sht(pstEnv) < 0)
	{
		printf("err:init_uinmudexp_sht \n");
		goto end;
	}
	
	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "ShadowAccData");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stData);
	pstEnv->stDestDBObj.pszObj = (char*)&stData;

	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM ShadowAccData");
	stSql.iBuff = strlen(stSql.pszBuff);
	
	iRet = tdr_query(&hDBResult, pstEnv->hSrcDBHandle, &stSql);
	if (0 != iRet)
	{
		printf("execute sql sentence:SELECT * FROM ShadowAccData error \n");
		goto end;
	}
	
	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows > 0)
	{
		for (i=0; i<iNumRows; i++)
		{
			iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
			if (0 != iRet)
			{	
				printf("tdr_fetch_row Error\n");
				goto end;
			}
			pstData = (SHADOWACCDATA*)pstEnv->stDestDBObj.pszObj;

			pstDstData= NULL;
			if (pstEnv->pstUinMudExpSht)
			{
				pstDstData = sht_find(pstEnv->pstUinMudExpSht, 
								(SHADOWACCDATA*)pstEnv->stDestDBObj.pszObj, 
								UinMudExpCmp, UinMudExpCode);
			}
			
			if (pstDstData)
			{
				if (pstData->MudExp > pstDstData->MudExp)
				{
					if (update_dst_db(pstEnv,  &pstEnv->stDestDBObj,DB_FLAG_WORLD) < 0)
					{
						printf("Update shadowroledata Error\n");
						goto end;
					}
				}
			}
			else
			{
				if (insert_dest_db(pstEnv, &pstEnv->stDestDBObj,DB_FLAG_WORLD) < 0)
				{
					printf("Insert shadowroledata Error\n");
					goto end;
				}
			}
		}
	}

	iSucc = 0;
	
end:
	if (pstEnv->pstUinMudExpSht)
	{
		sht_destroy(&pstEnv->pstUinMudExpSht);
		pstEnv->pstUinMudExpSht = NULL;
	}
	return iSucc;
}

static int update_db_of_toprange(UNITEENV* pstEnv)
{
	TOPRANGE stTop;
	TOPRANGE *pstTop;
	char szSql[1024];
	char szBuff[1024];
	TDRDATA stSql;
	int iCount = 0;
	int iRet;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iNumRows = 0;
	int i;

	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "TopRange");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stTop);
	pstEnv->stDestDBObj.pszObj = (char*)&stTop;

	printf("���ڴ���TopRange��\n");

	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM TopRange");
	stSql.iBuff = strlen(stSql.pszBuff);

	iRet = tdr_query(&hDBResult, pstEnv->hSrcDBHandle, &stSql);
	if (0 != iRet)
	{
		unite_error_log(pstEnv->fp,"src db execute sql sentence:SELECT * FROM TopRange error\n");
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
			unite_error_log(pstEnv->fp,"src TopRange table:tdr_fetch_row error\n");
			return -1;
		}

		pstTop =  (TOPRANGE*)pstEnv->stDestDBObj.pszObj;
		pstTop->TopRange += 400;

		/*   FirstName       ���ˡ�ְҵ��ս����ʾ��ɫ����
						�����ʾ�������������ʾ������ */
						
		/*   SecondName	���ˡ�ְҵ��ʾ�������������ʾ�᳤����
						ս����ʾ�������������ʾ������ */
		if (	pstTop->MainType == RANGE_MAIN_SINGLE ||
			pstTop->MainType == RANGE_MAIN_CAREER ||
			pstTop->MainType == RANGE_MAIN_COMBAT ||
			pstTop->MainType == RANGE_MAIN_BATTLE ||
			pstTop->MainType == RANGE_MAIN_SPARTA ||
			pstTop->MainType == RANGE_MAIN_ATTR)
		{
			modify_role_name(pstEnv,pstTop->RangeData.FirstName, sizeof(pstTop->RangeData.FirstName), NULL);
		}
		else if (pstTop->MainType == RANGE_MAIN_CLAN)
		{
			modify_clan_name(pstEnv,pstTop->RangeData.FirstName, sizeof(pstTop->RangeData.FirstName), NULL);
		}
		else if (pstTop->MainType == RANGE_MAIN_PET)
		{
			// ����������
		}
	
		if (pstTop->MainType == RANGE_MAIN_SINGLE ||
			pstTop->MainType == RANGE_MAIN_CAREER||
			pstTop->MainType == RANGE_MAIN_ATTR)
		{
			modify_clan_name(pstEnv,pstTop->RangeData.SecondName, sizeof(pstTop->RangeData.SecondName), NULL);
		}
		else if (pstTop->MainType == RANGE_MAIN_CLAN ||
				pstTop->MainType == RANGE_MAIN_PET)
		{
			modify_role_name(pstEnv,pstTop->RangeData.SecondName, sizeof(pstTop->RangeData.SecondName), NULL);
		}
		else if (pstTop->MainType == RANGE_MAIN_COMBAT || pstTop->MainType == RANGE_MAIN_BATTLE)
		{
			// ս������
		}
		
		if (insert_dest_db(pstEnv, &pstEnv->stDestDBObj,DB_FLAG_WORLD) < 0)
		{
			printf("Insert TopRange Error\n");
			snprintf(szBuff,sizeof(szBuff),"dest TopRange table: insert error! \n");
			unite_error_log(pstEnv->fp,szBuff);
			return -1;
		}

		iCount ++;
		if (iCount %500 == 0)
		{
			printf("���ڴ���TopRange��\n");
		}
	}
	return 0;
}


static int update_db_of_home(UNITEENV* pstEnv)
{
	int i;
	HOME stHome;
	HOME *pstHome;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	char szSql[1024];
	char szBuff[1024];
	int iCount = 0;

	printf("���ڴ���Home��\n");
	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "Home");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stHome);
	pstEnv->stDestDBObj.pszObj = (char*)&stHome;

	
	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM Home");
	stSql.iBuff = strlen(stSql.pszBuff);

	iRet = tdr_query(&hDBResult, pstEnv->hSrcDBHandle, &stSql);
	if (0 != iRet)
	{
		printf("tdr_query Error\n");
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
			printf("tdr_fetch_row Error\n");
			return -1;
		}

		pstHome = (HOME*)pstEnv->stDestDBObj.pszObj;

		// ������ID
		pstHome->HomeID = 0;
		modify_role_name(pstEnv, pstHome->RoleName, sizeof(pstHome->RoleName), NULL);
		if (insert_dest_db(pstEnv, &pstEnv->stDestDBObj,DB_FLAG_WORLD) < 0)
		{
			printf("Insert Home Error\n");
			return -1;
		}

		iCount ++;
	}

	printf("Home�����%d����¼\n",iCount);

	return 0;
}

static int update_db_of_store(UNITEENV* pstEnv)
{
	int i;
	STORE stStore;
	STORE *pstStore;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	char szSql[1024];
	char szBuff[1024];
	int iCount = 0;

	printf("���ڴ���Store��\n");
	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "Store");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stStore);
	pstEnv->stDestDBObj.pszObj = (char*)&stStore;

	
	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM Store");
	stSql.iBuff = strlen(stSql.pszBuff);

	iRet = tdr_query(&hDBResult, pstEnv->hSrcDBHandle, &stSql);
	if (0 != iRet)
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

		pstStore = (STORE*)pstEnv->stDestDBObj.pszObj;

		// ������ID
		pstStore->StoreID = 0;
		modify_role_name(pstEnv, pstStore->Name, sizeof(pstStore->Name), NULL);
		if (insert_dest_db(pstEnv, &pstEnv->stDestDBObj,DB_FLAG_WORLD) < 0)
		{
			printf("Insert Store Error\n");
			return -1;
		}

		iCount ++;
	}

	printf("Store�����%d����¼\n",iCount);
	return 0;
}

static int updata_db_of_clan(UNITEENV* pstEnv)
{
	int i;
	CLAN stClan;
	CLAN *pstClan;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	char szSql[1024];
	char szBuff[1024];
	int iCount = 0;
	CLAN *pstClanList = NULL;

	// ����������
	printf("���ڴ���Clan��\n");
	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "Clan");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stClan);
	pstEnv->stDestDBObj.pszObj = (char*)&stClan;
	
	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM Clan");
	stSql.iBuff = strlen(stSql.pszBuff);
	iRet = tdr_query(&hDBResult, pstEnv->hDestDBHandle, &stSql);
	if (0 != iRet)
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

		pstClan = (CLAN*)pstEnv->stDestDBObj.pszObj;
		pstClan->ClanInner.ClanApplyEnemyNum = 0;
		pstClan->ClanInner.ClanApplyFriendNum = 0;
		pstClan->ClanInner.ClanApplyNum = 0;
		pstClan->ClanInner.ClanEnemyNum = 0;
		pstClan->ClanInner.ClanFriendNum = 0;
		pstClan->ClanCore.NextAddCityResource = 0;
		pstClan->ClanCore.NextAddStrongPointResource = 0;
		memset(&pstClan->ClanInner.StrongInfo, 0, sizeof(pstClan->ClanInner.StrongInfo));
		memset(&pstClan->ClanAdvance.ClanDonateLogInfo, 0, sizeof(pstClan->ClanAdvance.ClanDonateLogInfo));
		
		if (update_dst_db(pstEnv,  &pstEnv->stDestDBObj,DB_FLAG_WORLD) < 0)
		{
			printf("Update Clan Error\n");
			return -1;	
		}
	}
	

	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "Clan");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stClan);
	pstEnv->stDestDBObj.pszObj = (char*)&stClan;

	
	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM Clan");
	stSql.iBuff = strlen(stSql.pszBuff);

	iRet = tdr_query(&hDBResult, pstEnv->hSrcDBHandle, &stSql);
	if (0 != iRet)
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

		pstClan = (CLAN*)pstEnv->stDestDBObj.pszObj;
		pstClan->ClanInner.ClanApplyEnemyNum = 0;
		pstClan->ClanInner.ClanApplyFriendNum = 0;
		pstClan->ClanInner.ClanApplyNum = 0;
		pstClan->ClanInner.ClanEnemyNum = 0;
		pstClan->ClanInner.ClanFriendNum = 0;
		pstClan->ClanCore.NextAddCityResource = 0;
		pstClan->ClanCore.NextAddStrongPointResource = 0;
		memset(&pstClan->ClanInner.StrongInfo, 0, sizeof(pstClan->ClanInner.StrongInfo));
		memset(&pstClan->ClanAdvance.ClanDonateLogInfo, 0, sizeof(pstClan->ClanAdvance.ClanDonateLogInfo));
		update_clan(pstEnv, pstClan);
		if (insert_dest_db(pstEnv, &pstEnv->stDestDBObj,DB_FLAG_WORLD) < 0)
		{
			printf("Insert Clan Error\n");
			return -1;
		}

		iCount ++;
		if (iCount %500 == 0)
		{
			printf("���ڴ���Clan��\n");
		}
	}

#if 0
	// �������Ĺ��ᳬ��2000
	if (pstEnv->iDstClanCount + iNumRows > 2000)
	{
		pstClanList = (CLAN*)malloc(iNumRows *sizeof(CLAN));
		if (!pstClanList)
			return -1;

		for (i=0; i<iNumRows; i++)
		{
			iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
			if (0 != iRet)
			{	
				unite_error_log(pstEnv->fp,"src Clan table:tdr_fetch_row error\n");
				free(pstClanList);
				return -1;
			}
			memcpy(pstClanList+i, pstEnv->stDestDBObj.pszObj, sizeof(CLAN));
		}
		qsort(pstClanList, iNumRows, sizeof(CLAN), unite_clan_cmp);

		for (i=0; i<2000-pstEnv->iDstClanCount; i++)
		{
			pstClan = pstClanList +i;
			update_clan(pstEnv, pstClan);
			if (insert_dest_db(pstEnv, &pstEnv->stDestDBObj,DB_FLAG_WORLD) < 0)
			{
				printf("Insert Clan Error\n");
				snprintf(szBuff,sizeof(szBuff),"dest Clan table: insert error! Name=%s\n", pstClan->ClanProf.Name);
				unite_error_log(pstEnv->fp,szBuff);
				free(pstClanList);
				return -1;
			}

			iCount ++;
			if (iCount %500 == 0)
			{
				printf("���ڴ���Clan��\n");
			}
		}
	}
	else
	{
		for (i=0; i<iNumRows; i++)
		{
			iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
			if (0 != iRet)
			{	
				unite_error_log(pstEnv->fp,"src Clan table:tdr_fetch_row error\n");
				return -1;
			}

			pstClan = (CLAN*)pstEnv->stDestDBObj.pszObj;
			update_clan(pstEnv, pstClan);
			if (insert_dest_db(pstEnv, &pstEnv->stDestDBObj,DB_FLAG_WORLD) < 0)
			{
				printf("Insert Clan Error\n");
				snprintf(szBuff,sizeof(szBuff),"dest Clan table: insert error! Name=%s\n", pstClan->ClanProf.Name);
				unite_error_log(pstEnv->fp,szBuff);
				return -1;
			}

			iCount ++;
			if (iCount %500 == 0)
			{
				printf("���ڴ���Clan��\n");
			}
		}
	}
#endif

	printf("Clan�����%d����¼\n",iCount);
	return 0;
}

static int updata_mail(UNITEENV *pstEnv, MAILENTRY *pstMail)
{
	int iRet = 0;

	// ϵͳ�ʼ�Send����Ҫ�޸�
	if (!(pstMail->Mail.Head.Flags & 1))
	{
		modify_role_name(pstEnv,pstMail->Mail.Head.Send, sizeof(pstMail->Mail.Head.Send), &iRet);
	}
	modify_role_name(pstEnv,pstMail->Mail.Head.Recv, sizeof(pstMail->Mail.Head.Recv), &iRet);

	return iRet;
}

static int update_db_of_mailentry(UNITEENV *pstEnv)
{
	int i;
	MAILENTRY stMail;
	MAILENTRY *pstMail;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	char szSql[1024];
	int iCount = 0;
	char szBuff[1024];
	
	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "MailEntry");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stMail);
	pstEnv->stDestDBObj.pszObj = (char*)&stMail;

	printf("���ڴ���MailEntry��\n");
	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM MailEntry");
	stSql.iBuff = strlen(stSql.pszBuff);

	iRet = tdr_query(&hDBResult, pstEnv->hSrcDBHandle, &stSql);
	if (0 != iRet)
	{
		unite_error_log(pstEnv->fp,"src db execute sql sentence:SELECT * FROM MailEntry error\n");
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
			unite_error_log(pstEnv->fp,"src MailEntry table:tdr_fetch_row error\n");
			return -1;
		}

		pstMail = (MAILENTRY*)pstEnv->stDestDBObj.pszObj;
		if (updata_mail(pstEnv, pstMail))
		{
			//  �����ʼ����ݿ�
			//updata_src_db(pstEnv,  &pstEnv->stDestDBObj);
		}

		if (insert_dest_db(pstEnv, &pstEnv->stDestDBObj,DB_FLAG_WORLD) < 0)
		{
			printf("insert MailEntry table error\n");
			snprintf(szBuff,sizeof(szBuff),"dest MailEntry table: insert error! WID=%lld\n", pstMail->Mail.Head.WID);
			unite_error_log(pstEnv->fp,szBuff);	
			continue;
		}

		iCount ++;
		if (iCount %500 == 0)
		{
			printf("���ڴ���MailEntry��\n");
		}
	}

	printf("MailEntry�����%d����¼\n",iCount);
	return 0;
}


int update_db_of_accountrole1(UNITEENV* pstEnv)
{
	int i,j;
	ACCOUNTROLE stAccRole;
	ACCOUNTROLE *pstAccRole;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	char szSql[512];
	int iCount = 0;
	char szBuff[1024];
	int iDup = 0;
	
	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "AccountRole");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stAccRole);
	pstEnv->stDestDBObj.pszObj = (char*)&stAccRole;

	printf("���ڴ���AccountRole��\n");

	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM AccountRole;");
	stSql.iBuff = strlen(stSql.pszBuff);
	
	iRet = tdr_query(&hDBResult, pstEnv->hSrcAccHandle, &stSql);
	if (iRet < 0)
	{
		snprintf(szBuff,sizeof(szBuff),"src db execute sql sentence:%s error\n", szSql);
		unite_error_log(pstEnv->fp,szBuff);
		return -1;	
	}
	
	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows < 1)
	{
		return 0;
	}

	for(j=0; j<iNumRows; j++ )
	{
		iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
		if (0 != iRet)
		{	
			unite_error_log(pstEnv->fp,"src AccountRole table:tdr_fetch_row error\n");
			return -1;
		}

		pstAccRole = (ACCOUNTROLE*)pstEnv->stDestDBObj.pszObj;
		modify_role_uin(pstEnv, &pstAccRole->Uin);
		pstAccRole->RegionID = pstEnv->stConf.iDestRegionID;
		for (i=0; i<pstEnv->stConf.iWorldNum; i++)
		{
			if (pstAccRole->ServerID == pstEnv->stConf.astWorldList[i].iSrcWorldID)
			{
				pstAccRole->ServerID = pstEnv->stConf.astWorldList[i].iDstWorldID;	
			}
		}


		iRet = insert_dest_db(pstEnv, &pstEnv->stDestDBObj,DB_FLAG_ACCOUNT);
		if (iRet < 0)
		{
			if (iRet == -2)
			{
				iDup ++;	
			}
			else
			{
				printf("Insert AccountRole table error\n");
				return -1;
			}	
		}
		else
		{
			iCount ++;
			if (iCount % 500 == 0)
			{
				printf("�����޸�AccountRole��\n");
			}
		}
	}
	
	printf("AccountRole���޸���%d����¼\n",iCount);

	if (iDup > 0)
	{
		printf("����: ��%d����¼�����ظ�!!\n",iDup);
	}
	
	return 0;
}


int update_db_of_accountrole(UNITEENV* pstEnv)
{
	int i,j;
	ACCOUNTROLE stAccRole;
	ACCOUNTROLE *pstAccRole;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	char szSql[512];
	int iCount = 0;
	char szBuff[1024];
	
	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "AccountRole");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stAccRole);
	pstEnv->stDestDBObj.pszObj = (char*)&stAccRole;

	printf("���ڴ���AccountRole��\n");

	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM AccountRole;");
	stSql.iBuff = strlen(stSql.pszBuff);
	
	iRet = tdr_query(&hDBResult, pstEnv->hSrcAccHandle, &stSql);
	if (iRet < 0)
	{
		snprintf(szBuff,sizeof(szBuff),"src db execute sql sentence:%s error\n", szSql);
		unite_error_log(pstEnv->fp,szBuff);
		return -1;	
	}
	
	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows < 1)
	{
		return 0;
	}

	for(j=0; j<iNumRows; j++ )
	{
		iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
		if (0 != iRet)
		{	
			unite_error_log(pstEnv->fp,"src AccountRole table:tdr_fetch_row error\n");
			return -1;
		}

		pstAccRole = (ACCOUNTROLE*)pstEnv->stDestDBObj.pszObj;

		if (pstAccRole->ServerID != pstEnv->stConf.iSrcWorldID)
		{
			continue;
		}
		
		modify_role_name(pstEnv,pstAccRole->RoleName,sizeof(pstAccRole->RoleName), NULL);
		pstAccRole->ServerID = pstEnv->stConf.iDestWorldID;
		pstAccRole->RegionID = pstEnv->stConf.iDestRegionID;
		modify_role_uin(pstEnv, &pstAccRole->Uin);

		if ( update_dst_db(pstEnv, &pstEnv->stDestDBObj,DB_FLAG_ACCOUNT) < 0)
		{
			printf("update AccountRole table error\n");
			return -1;
		}
		
		/*if (insert_dest_db(pstEnv, &pstEnv->stDestDBObj,DB_FLAG_ACCOUNT) < 0)
		{
			printf("insert AccountRole table error\n");
			snprintf(szBuff,sizeof(szBuff),"dest AccountRole table: insert error! RoleNane=%s\n", pstAccRole->RoleName);
			unite_error_log(pstEnv->fp,szBuff);	
			return -1;;
		}*/

		iCount ++;
		if (iCount % 500 == 0)
		{
			printf("�����޸�AccountRole��\n");
		}
		
	}
	
	printf("AccountRole���޸���%d����¼\n",iCount);
	return 0;
}

static int update_db_of_roledata(UNITEENV* pstEnv)
{
	int i,j;
//	char *pszName;	
	ROLEDATA stRoleData;
	ROLEDATA *pstRoleData;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	char szSql[512];
	int iCount = 0;
	char szBuff[1024];
	
	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "RoleData");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stRoleData);
	pstEnv->stDestDBObj.pszObj = (char*)&stRoleData;

	printf("���ڴ���RoleData��\n");

	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM RoleData");
	stSql.iBuff = strlen(stSql.pszBuff);
	
	iRet = tdr_query(&hDBResult, pstEnv->hSrcDBHandle, &stSql);
	if (iRet < 0)
	{
		snprintf(szBuff,sizeof(szBuff),"src db execute sql sentence:%s error\n", szSql);
		unite_error_log(pstEnv->fp,szBuff);
		return -1;	
	}
	
	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows < 1)
	{
		return 0;
	}

	for(j=0; j<iNumRows; j++ )
	{
		iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
		if (0 != iRet)
		{	
			unite_error_log(pstEnv->fp,"src RoleData table:tdr_fetch_row error\n");
			return -1;
		}

		pstRoleData = (ROLEDATA*)pstEnv->stDestDBObj.pszObj;
		memset(&pstRoleData->MiscInfo.RoleTradeRecordInfo, 0, sizeof(pstRoleData->MiscInfo.RoleTradeRecordInfo));
		pstRoleData->MiscInfo.NameChangeTimes = 0;
	
		if (updata_roledata(pstEnv, pstRoleData))
		{
			//updata_src_db(pstEnv,  &pstEnv->stDestDBObj);
		}

		if (insert_dest_db(pstEnv, &pstEnv->stDestDBObj,DB_FLAG_WORLD) < 0)
		{
			printf("insert RoleData table error\n");
			snprintf(szBuff,sizeof(szBuff),"dest RoleData table: insert error! RoleNane=%s\n", pstRoleData->RoleName);
			unite_error_log(pstEnv->fp,szBuff);	
			continue;
		}

		iCount ++;
		if (iCount % 500 == 0)
		{
			printf("���ڲ���RoleData��\n");
		}
		
	}
	
	printf("RoleData�����%d����¼\n",iCount);
	return 0;
}

static int update_db_of_global(UNITEENV* pstEnv)
{
//	int iDestIdx,iSrcIdx;
	GLOBAL *pstGlobal;
	GLOBAL stGlobal;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	TDRDBRESULTHANDLE hDestDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	char szSql[10240];
	int i;
	int iMapID, iIsOpen;
	
	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "Global");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stGlobal);
	pstEnv->stDestDBObj.pszObj = (char*)&stGlobal;

	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM Global");
	stSql.iBuff = strlen(stSql.pszBuff);

	printf("���ڴ���Global��\n");
	iRet = tdr_query(&hDBResult, pstEnv->hSrcDBHandle, &stSql);
	if (0 != iRet)
	{
		unite_error_log(pstEnv->fp,"src db execute sql sentence:SELECT * FROM Global error\n");
		return -1;
	}
	
	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows < 1)
	{
		return -1;
	}

	iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
	if (0 != iRet)
	{	
		unite_error_log(pstEnv->fp,"src db: tdr_fetch_row error\n");
		return -1;
	}
	pstGlobal = (GLOBAL*)pstEnv->stDestDBObj.pszObj;
	pstEnv->iSrcClanLastIdx = pstGlobal->ClanLastIdx;
	
	iRet = tdr_query(&hDestDBResult, pstEnv->hDestDBHandle, &stSql);
	if (0 != iRet)
	{
		unite_error_log(pstEnv->fp,"dest db execute sql sentence:SELECT * FROM Global error\n");
		return -1;
	}
	
	iNumRows = tdr_num_rows(hDestDBResult);
	if(iNumRows < 1)
	{
		return -1;
	}

	iRet = tdr_fetch_row(hDestDBResult, &pstEnv->stDestDBObj);
	if (0 != iRet)
	{	
		unite_error_log(pstEnv->fp,"dest db: tdr_fetch_row error\n");
		return -1;
	}

	// ����id
	pstGlobal = (GLOBAL*)pstEnv->stDestDBObj.pszObj;
	pstEnv->iDestClanLastIdx = pstGlobal->ClanLastIdx;
	pstGlobal->ClanLastIdx += pstEnv->iSrcClanLastIdx;

	// �ݵ���Ϣֻ������û�п���
	for (i=0; i<pstGlobal->Misc.StrongPointInfo.Num; i++)
	{
		iMapID = pstGlobal->Misc.StrongPointInfo.StrongPoints[i].MapID;
		iIsOpen = pstGlobal->Misc.StrongPointInfo.StrongPoints[i].IsOpen;

		memset(&pstGlobal->Misc.StrongPointInfo.StrongPoints[i], 0, sizeof(pstGlobal->Misc.StrongPointInfo.StrongPoints[i]));
		pstGlobal->Misc.StrongPointInfo.StrongPoints[i].MapID = iMapID;
		pstGlobal->Misc.StrongPointInfo.StrongPoints[i].IsOpen = iIsOpen;
	}

	// ʱ�������Ĩ��
	memset(&pstGlobal->Misc.WorldCity, 0, sizeof(pstGlobal->Misc.WorldCity));

	// bct����Ĩ��
	memset(&pstGlobal->Misc.BaiCengHistory, 0, sizeof(pstGlobal->Misc.BaiCengHistory));


	
	
	if (updata_dest_db(pstEnv, &pstEnv->stDestDBObj) < 0)
	{
		unite_error_log(pstEnv->fp,"dest db: update Global table error\n");
		return -1;
	}
	return 0;
}

static int fill_sht(UNITEENV* pstEnv)
{
	if (pstEnv->pstNameDest)
	{
		if (load_nameid_db(pstEnv, DEST_DB) < 0)
		{
			return -1;
		}
	}

	if (pstEnv->pstOverlapName)
	{
		if (load_nameid_db(pstEnv, SRC_DB) < 0)
		{
			return -1;
		}	
	}
	
	if (pstEnv->pstClanDest)
	{
		if (load_clan_db(pstEnv, DEST_DB) < 0)
		{
			return -1;
		}
	}
	
	if (pstEnv->pstOverlapClan)
	{
		if (load_clan_db(pstEnv, SRC_DB) < 0)
		{
			return -1;
		}
	}
	
	return 0;
}

int unite_check_args(int argc, char* argv[])
{
	char c;
	if (	argc<2 ||
		(strcmp(argv[1], "account") &&
		strcmp(argv[1], "world") &&
		strcmp(argv[1], "nameid")) )
	{
		printf("��������!\n");
		printf("����account:ֻ��account����кϲ�\n");
		printf("����world:��account���worldһ���ı�ϲ�\n");
		printf("����nameid:��ֹ��ע\n");
		return -1;
	}

	printf("�Ϸ�֮ǰ��ȷ���Ѿ����ݺ�������ݿ�!\n");
	printf("ȷ���밴 y ��Y ��,���������ȡ�����β���!\n");
	scanf("%c",&c);
	if (	c != 'Y' &&
		c != 'y')
	{
		return -1;
	}
	printf("..............start..............\n");
	return 0;
}


int unite_account_modufy_shadowroledata1(UNITEENV* pstEnv)
{
	TDRDBMS stDBMS;
	char szErrorMsg[2048]={0};
	UNITE_SVR_CONF *pstConf = &pstEnv->stConf;
	int i;
	TDRDBHANDLE hSrcDBHandle;	
	TDRDBHANDLE hSrcUpDBHandle;	
	int j;
	SHADOWACCDATA stData;
	SHADOWACCDATA* pstData;
	char szSql[1024];
	TDRDATA stSql;
	int iCount = 0;
	int iRet;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iNumRows = 0;
	int iUin;
	
	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM ShadowAccData");
	stSql.iBuff = strlen(stSql.pszBuff);
	
	for (i=0; i<pstConf->iWorldNum; i++)
	{
		memset(&stDBMS, 0, sizeof(stDBMS));
		STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
		STRNCPY(stDBMS.szDBMSCurDatabaseName, pstConf->astWorldList[i].szSrcDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
		STRNCPY(stDBMS.szDBMSConnectionInfo,pstConf->astWorldList[i].szSrcDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
		STRNCPY(stDBMS.szDBMSUser, pstConf->astWorldList[i].szSrcDBUser, sizeof(stDBMS.szDBMSUser));
		STRNCPY(stDBMS.szDBMSPassword, pstConf->astWorldList[i].szSrcDBPasswd, sizeof(stDBMS.szDBMSPassword));
		stDBMS.iReconnectOpt = 1;	
		
		if (0 > tdr_open_dbhanlde(&hSrcDBHandle, &stDBMS, &szErrorMsg[0]))
		{
			printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
			return -1;
		}	

		if (0 > tdr_open_dbhanlde(&hSrcUpDBHandle, &stDBMS, &szErrorMsg[0]))
		{
			printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
			return -1;
		}	

		pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "ShadowAccData");
		pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
		pstEnv->stDestDBObj.iObjSize = sizeof(stData);
		pstEnv->stDestDBObj.pszObj = (char*)&stData;

		printf("���ڴ���ShadowRoleData��:%s\n", pstConf->astWorldList[i].szSrcDBName);

		iRet = tdr_query(&hDBResult, hSrcDBHandle, &stSql);
		if (0 != iRet)
		{
			unite_error_log(pstEnv->fp,"src db execute sql sentence:SELECT * FROM ShadowAccData error\n");
			return -1;
		}
		
		iNumRows = tdr_num_rows(hDBResult);
		if(iNumRows < 1)
		{
			return 0;
		}

		for (j=0; j<iNumRows; j++)
		{
			iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
			if (0 != iRet)
			{	
				unite_error_log(pstEnv->fp,"src ShadowAccData table:tdr_fetch_row error\n");
				return -1;
			}

			pstData =  (SHADOWACCDATA*)pstEnv->stDestDBObj.pszObj;
			iUin = pstData->Uin;
			
			modify_role_uin(pstEnv, &pstData->Uin);
			if(iUin != pstData->Uin)
			{
				acc_updata_src_db(pstEnv, &pstEnv->stDestDBObj, hSrcUpDBHandle);
				//updata_src_db(pstEnv,  &pstEnv->stDestDBObj);
				iCount ++;
			}
		}

		printf("ShadowAccData��:%s����%d����¼���޸�\n",pstConf->astWorldList[i].szSrcDBName,iCount);

	}
	return 0;
}

int unite_account_modify_shadowroledata(UNITEENV* pstEnv)
{
	SHADOWACCDATA stData;
	SHADOWACCDATA* pstData;
	char szSql[1024];
	TDRDATA stSql;
	int iCount = 0;
	int iRet;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iNumRows = 0;
	int i;
	int iUin;

	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "ShadowAccData");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stData);
	pstEnv->stDestDBObj.pszObj = (char*)&stData;

	printf("���ڴ���ShadowRoleData��\n");

	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM ShadowAccData");
	stSql.iBuff = strlen(stSql.pszBuff);

	iRet = tdr_query(&hDBResult, pstEnv->hSrcDBHandle, &stSql);
	if (0 != iRet)
	{
		unite_error_log(pstEnv->fp,"src db execute sql sentence:SELECT * FROM ShadowAccData error\n");
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
			unite_error_log(pstEnv->fp,"src ShadowAccData table:tdr_fetch_row error\n");
			return -1;
		}

		pstData =  (SHADOWACCDATA*)pstEnv->stDestDBObj.pszObj;
		iUin = pstData->Uin;
		
		modify_role_uin(pstEnv, &pstData->Uin);
		if(iUin != pstData->Uin)
		{
			updata_src_db(pstEnv,  &pstEnv->stDestDBObj);
			iCount ++;
		}

	}

	printf("ShadowAccData������%d����¼���޸�\n",iCount);
	return 0;
}

int unite_account_modify_accshare1(UNITEENV* pstEnv)
{
	TDRDBMS stDBMS;
	char szErrorMsg[2048]={0};
	UNITE_SVR_CONF *pstConf = &pstEnv->stConf;
	int i;
	TDRDBHANDLE hSrcDBHandle;	
	int j;
	TDRDBHANDLE hSrcUpDBHandle;	
	ACCSHARE stAccShare;
	ACCSHARE *pstAccShare;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	char szSql[10240];
	int iCount = 0;
	char szBuff[512];
	int iUin;
	unsigned int iNum = 0;
	int iRead = 0;
	int iCurr = 0;

	for (i=0; i<pstConf->iWorldNum; i++)
	{
		memset(&stDBMS, 0, sizeof(stDBMS));
		STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
		STRNCPY(stDBMS.szDBMSCurDatabaseName, pstConf->astWorldList[i].szSrcDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
		STRNCPY(stDBMS.szDBMSConnectionInfo,pstConf->astWorldList[i].szSrcDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
		STRNCPY(stDBMS.szDBMSUser, pstConf->astWorldList[i].szSrcDBUser, sizeof(stDBMS.szDBMSUser));
		STRNCPY(stDBMS.szDBMSPassword, pstConf->astWorldList[i].szSrcDBPasswd, sizeof(stDBMS.szDBMSPassword));
		stDBMS.iReconnectOpt = 1;	
		
		if (0 > tdr_open_dbhanlde(&hSrcDBHandle, &stDBMS, &szErrorMsg[0]))
		{
			printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
			return -1;
		}	

		if (0 > tdr_open_dbhanlde(&hSrcUpDBHandle, &stDBMS, &szErrorMsg[0]))
		{
			printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
			return -1;
		}	

		pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "AccShare");
		pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
		pstEnv->stDestDBObj.iObjSize = sizeof(stAccShare);
		pstEnv->stDestDBObj.pszObj = (char*)&stAccShare;

		printf("���ڴ���AccShare��:%s\n", pstConf->astWorldList[i].szSrcDBName);

		if (tdr_get_records_count(hSrcDBHandle, "AccShare",NULL, &iNum) < 0)
		{
			printf("tdr_get_records_count fail: %s\n", szErrorMsg);
			return -1;
		}

		if (iNum <= 0)
		{
			continue;
		}

		iCount = iNum;
		iCurr = 0;
		while(iNum > 0)
		{
			if (iNum > 10000)
			{
				iRead = 10000;
				stSql.pszBuff = &szSql[0];
				snprintf(szSql, sizeof(szSql),"SELECT * FROM AccShare limit %d,10000", iCurr);
				stSql.iBuff = strlen(stSql.pszBuff);	
			}
			else
			{
				iRead = iNum;
				stSql.pszBuff = &szSql[0];
				snprintf(szSql, sizeof(szSql),"SELECT * FROM AccShare limit %d,%d", iCurr, iNum);
				stSql.iBuff = strlen(stSql.pszBuff);
			}

			iNum -= iRead;
			iCurr += iRead;
			
			iRet = tdr_query(&hDBResult, hSrcDBHandle, &stSql);
			if (iRet < 0)
			{
				printf("tdr_query fail!\n");
				return -1;	
			}

			iNumRows = tdr_num_rows(hDBResult);
			if(iNumRows != iRead)
			{
				printf("iNumRows != iRead!\n");
				return -1;
			}

			for(j=0; j<iNumRows; j++ )
			{
				iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
				if (0 != iRet)
				{	
					printf("tdr_fetch_row fail!\n");
					return -1;
				}

				pstAccShare = (ACCSHARE*)pstEnv->stDestDBObj.pszObj;
				iUin = pstAccShare->Uin;
				modify_role_uin(pstEnv, &pstAccShare->Uin);
				if (iUin != pstAccShare->Uin)
				{
					//updata_src_db(pstEnv,  &pstEnv->stDestDBObj);
					acc_updata_src_db(pstEnv, &pstEnv->stDestDBObj, hSrcUpDBHandle);
				}
			}

			if (hDBResult)
			{
				tdr_free_dbresult(&hDBResult);
				hDBResult = 0;
			}
			printf("AccShare table:Count=%d, process=%d\n", iCount, iCurr);
		}
	}
	return 0;
}


int unite_roledata_up_in(UNITEENV* pstEnv, ROLEDATA *pstRoleData, TDRDBHANDLE hDBHandle)
{
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBUpResult = 0;
	int iRet = 0;
	char szSql[512];
	
	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"update RoleData set Uin=%d,WorldID=%d where RoleID=%llu", 
							pstRoleData->Uin, pstRoleData->WorldID, pstRoleData->RoleID);
	stSql.iBuff = strlen(szSql);
	
	iRet = tdr_query(&hDBUpResult, hDBHandle, &stSql);
	if (0 != iRet)
	{
		printf("ִ�и�����Դ��¼��SQL���ʧ��: %s dbms error:%s\n",
				tdr_error_string(iRet), tdr_dbms_error(hDBHandle));		
	}

	if (hDBUpResult)
		tdr_free_dbresult(&hDBUpResult);
	
	return iRet;
}

int unite_account_modify_roledata1(UNITEENV* pstEnv)
{
	TDRDBMS stDBMS;
	char szErrorMsg[2048]={0};
	UNITE_SVR_CONF *pstConf = &pstEnv->stConf;
	int i;
	TDRDBHANDLE hSrcDBHandle;	
	TDRDBHANDLE hSrcUpDBHandle;	
	int j;
	ROLEDATA stRoleData;
	ROLEDATA *pstRoleData;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	char szSql[10240];
	int iCount = 0;
	char szBuff[512];
	int iUin;
	unsigned int iNum = 0;
	int iCurr = 0;
	int iRead;


	for (i=0; i<pstConf->iWorldNum; i++)
	{
		memset(&stDBMS, 0, sizeof(stDBMS));
		STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
		STRNCPY(stDBMS.szDBMSCurDatabaseName, pstConf->astWorldList[i].szSrcDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
		STRNCPY(stDBMS.szDBMSConnectionInfo,pstConf->astWorldList[i].szSrcDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
		STRNCPY(stDBMS.szDBMSUser, pstConf->astWorldList[i].szSrcDBUser, sizeof(stDBMS.szDBMSUser));
		STRNCPY(stDBMS.szDBMSPassword, pstConf->astWorldList[i].szSrcDBPasswd, sizeof(stDBMS.szDBMSPassword));
		stDBMS.iReconnectOpt = 1;	
		
		if (0 > tdr_open_dbhanlde(&hSrcDBHandle, &stDBMS, &szErrorMsg[0]))
		{
			printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
			return -1;
		}


		if (0 > tdr_open_dbhanlde(&hSrcUpDBHandle, &stDBMS, &szErrorMsg[0]))
		{
			printf("tdr_open_dbhanlde fail: %s\n", szErrorMsg);
			return -1;
		}	

		printf("���ڴ���RoleData��:%s\n", pstConf->astWorldList[i].szSrcDBName);
		if (tdr_get_records_count(hSrcDBHandle, "RoleData",NULL, &iNum) < 0)
		{
			return -1;
		}

		if (iNum <= 0)
		{
			continue;
		}

		iCount = iNum;
		iCurr = 0;
		pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "RoleData");
		pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
		pstEnv->stDestDBObj.iObjSize = sizeof(stRoleData);
		pstEnv->stDestDBObj.pszObj = (char*)&stRoleData;

		while(iNum > 0)
		{
			if (iNum > 10000)
			{
				iRead = 10000;
				stSql.pszBuff = &szSql[0];
				snprintf(szSql, sizeof(szSql),"SELECT * FROM RoleData limit %d,10000", iCurr);
				stSql.iBuff = strlen(stSql.pszBuff);	
				
				
			}
			else
			{
				iRead = iNum;
				stSql.pszBuff = &szSql[0];
				snprintf(szSql, sizeof(szSql),"SELECT * FROM RoleData limit %d,%d", iCurr, iNum);
				stSql.iBuff = strlen(stSql.pszBuff);
			}

			iNum -= iRead;
			iCurr+=iRead;
			
			iRet = tdr_query(&hDBResult, hSrcDBHandle, &stSql);
			if (iRet < 0)
			{
				printf("tdr_query fail!\n");
				return -1;	
			}

			iNumRows = tdr_num_rows(hDBResult);
			if(iNumRows != iRead)
			{
				printf("iNumRows != iRead!\n");
				return -1;
			}

			for(j=0; j<iNumRows; j++ )
			{
				iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
				if (0 != iRet)
				{	
					printf("tdr_fetch_row fail!\n");
					return -1;
				}

				pstRoleData = (ROLEDATA*)pstEnv->stDestDBObj.pszObj;
				iUin = pstRoleData->Uin;
				modify_role_uin(pstEnv, &pstRoleData->Uin);
				pstRoleData->WorldID = pstConf->astWorldList[i].iDstWorldID;
				
				//updata_src_db(pstEnv,  &pstEnv->stDestDBObj);
				unite_roledata_up_in(pstEnv, pstRoleData, hSrcUpDBHandle);
			}

			if (hDBResult)
			{
				tdr_free_dbresult(&hDBResult);
				hDBResult = 0;
			}
			printf("RoleData table:count=%d, process=%d\n", iCount, iCurr);
		}
	}
	
	
	return 0;
}

int unite_account_modify_roledata(UNITEENV* pstEnv)
{
	int j;
	ROLEDATA stRoleData;
	ROLEDATA *pstRoleData;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	char szSql[10240];
	int iCount = 0;
	char szBuff[512];
	int iUin;
	
	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "RoleData");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stRoleData);
	pstEnv->stDestDBObj.pszObj = (char*)&stRoleData;

	printf("���ڴ���RoleData��\n");

	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM RoleData");
	stSql.iBuff = strlen(stSql.pszBuff);
	
	iRet = tdr_query(&hDBResult, pstEnv->hSrcDBHandle, &stSql);
	if (iRet < 0)
	{
		snprintf(szBuff,sizeof(szBuff),"src db execute sql sentence:%s error\n", szSql);
		unite_error_log(pstEnv->fp,szBuff);
		return -1;	
	}
	
	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows < 1)
	{
		return 0;
	}

	for(j=0; j<iNumRows; j++ )
	{
		iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
		if (0 != iRet)
		{	
			unite_error_log(pstEnv->fp,"src RoleData table:tdr_fetch_row error\n");
			return -1;
		}

		pstRoleData = (ROLEDATA*)pstEnv->stDestDBObj.pszObj;
		iUin = pstRoleData->Uin;
		modify_role_uin(pstEnv, &pstRoleData->Uin);

		if (iUin !=  pstRoleData->Uin)
		{
			updata_src_db(pstEnv,  &pstEnv->stDestDBObj);
			iCount ++;
		}
	}
	printf("RoleData������%d����¼���޸�\n",iCount);
	return 0;
}


int unite_save_gold_coin(	UNITEENV* pstEnv)
{
	int iRet = 0;
//	#if 0
	int i;
	ROLEDATA stRoleData;
	ROLEDATA *pstRoleData;
	TDRDBRESULTHANDLE hDBResult = 0;
	
	int iNumRows = 0;
	char szSql[1024];
	char szBuff[1024];
	TDRDATA stSql;
	LPSHTABLE pstSht = NULL;
	ACCGOLD *pstAccGold;
	ACCGOLD stAccGold;

	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "RoleData");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stRoleData);
	pstEnv->stDestDBObj.pszObj = (char*)&stRoleData;

	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT Count(*) FROM RoleData Group by Uin");
	stSql.iBuff = strlen(stSql.pszBuff);

	iRet = tdr_query(&hDBResult, pstEnv->hDestDBHandle, &stSql);
	if (0 != iRet)
	{
		return -1;
	}

	// �ж��ٸ�Uin
	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows < 1)
	{
		return 0;
	}

	pstSht = sht_create(iNumRows*3, iNumRows,sizeof(ACCGOLD), NULL);
	if (NULL == pstSht)
	{
		return -1;
	}

	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM RoleData where GodCoin > 0");
	stSql.iBuff = strlen(stSql.pszBuff);

	iRet = tdr_query(&hDBResult, pstEnv->hDestDBHandle, &stSql);
	if (0 != iRet)
	{
		goto end;
	}

	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows < 1)
	{
		iRet = 0;
		goto end;
	}

	for(i=0; i<iNumRows; i++ )
	{
		iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
		if (0 != iRet)
		{	
			goto end;
		}

		pstRoleData = (ROLEDATA*)pstEnv->stDestDBObj.pszObj;
		stAccGold.iUin = pstRoleData->Uin;
			
		if (pstRoleData->RoleDetail.GodCoin > 0)
		{
			pstAccGold = sht_find(pstSht, &stAccGold, AccGoldCmp, AccGoldCode);
			if (pstAccGold)
			{
				pstAccGold->ullGold += pstRoleData->RoleDetail.GodCoin;
			}
			else
			{
				pstAccGold = sht_insert_unique(pstSht, &stAccGold, AccGoldCmp, AccGoldCode);
				if (pstAccGold)
				{
					pstAccGold->ullGold = pstRoleData->RoleDetail.GodCoin;
					pstAccGold->iUin =  pstRoleData->Uin;
				}
				else
				{
					iRet = -1;
					goto end;
				}
			}

			pstRoleData->RoleDetail.GodCoin = 0;
			if (update_dst_db( pstEnv, &pstEnv->stDestDBObj, DB_FLAG_WORLD) < 0)
			{
				iRet = -1;
				goto end;
			}
			
		}
	}

	stSql.pszBuff = &szSql[0];

	// ����Bill��
	int iValid;
	for (i=0; i<pstSht->iMax; i++)
	{
		pstAccGold = sht_pos(pstSht, i, &iValid);
		if (iValid == 0)
		{
			continue;
		}

		snprintf(szSql, sizeof(szSql),"update Bill set Points=Points+%llu where Uin=%d", pstAccGold->ullGold, pstAccGold->iUin);
		stSql.iBuff = strlen(stSql.pszBuff);

		iRet = tdr_query(&hDBResult, pstEnv->hDstUpBillHandle, &stSql);
		if (0 != iRet)
		{
			goto end;
		}
	}

	iRet = 0;
end:

	if (pstSht)
	{
		sht_destroy(&pstSht);
		pstSht = NULL;	
	}

	if (iRet != 0)
		iRet = -1;
//	#endif
	
	return iRet;
}


int pass9_find_uin(UNITEENV* pstEnv, int iUin)
{
	ACCDATA stAccData;
	ACCDATA *pstAccDataSrc, *pstAccDataDst;
	int i;
	int iValid;
	
	stAccData.iUin = iUin;
	pstAccDataSrc= sht_find(pstEnv->pstSrcAccUinSht, &stAccData,AccDataUinCmp, AccDataUinCode);	
	if (!pstAccDataSrc)
		return -1;

	for (i=0; i<pstEnv->pstDstAccUinSht->iMax; i++)
	{
		pstAccDataDst = sht_pos(pstEnv->pstDstAccUinSht, i, &iValid);
		if (iValid == 0)
		{
			continue;
		}

		if (strcmp(pstAccDataSrc->szPass9, pstAccDataDst->szPass9) == 0)
		{
			if (pstAccDataDst->iUin > 0)
			{
				return pstAccDataDst->iUin; // ��������˺�����һ��ȥ��UIN
			}
			return -1;
		}
	}
	return 0;
}


/*int update_range_modify_name(UNITEENV* pstEnv,TOPRANGE *pstRange)
{

	if (pstRange->MainType == RANGE_MAIN_SINGLE)
	{
		modify_role_name(pstEnv,pstRange->RangeData.FirstName, sizeof(pstRange->RangeData.FirstName),NULL);
		modify_clan_name(pstEnv,pstRange->RangeData.SecondName, sizeof(pstRange->RangeData.SecondName),NULL)
	}
	else if (pstRange->MainType == RANGE_MAIN_CAREER)
	{
		modify_role_name(pstEnv,pstRange->RangeData.FirstName, sizeof(pstRange->RangeData.FirstName),NULL);
		modify_clan_name(pstEnv,pstRange->RangeData.SecondName, sizeof(pstRange->RangeData.SecondName),NULL)
	}
	else if (pstRange->MainType == RANGE_MAIN_CLAN)
	{
		modify_role_name(pstEnv,pstRange->RangeData.SecondName, sizeof(pstRange->RangeData.SecondName),NULL);
		modify_clan_name(pstEnv,pstRange->RangeData.FirstName, sizeof(pstRange->RangeData.FirstName),NULL)
	}
	else if (pstRange->MainType == RANGE_MAIN_PET)
	{
		modify_role_name(pstEnv,pstRange->RangeData.SecondName, sizeof(pstRange->RangeData.SecondName),NULL);
	}
	else if (pstRange->MainType == RANGE_MAIN_COMBAT)
	{
		modify_role_name(pstEnv,pstRange->RangeData.FirstName, sizeof(pstRange->RangeData.FirstName),NULL);
	}
	else if (pstRange->MainType == RANGE_MAIN_BATTLE)
	{
		modify_role_name(pstEnv,pstRange->RangeData.FirstName, sizeof(pstRange->RangeData.FirstName),NULL);
	}
	return 0;
}

int update_range(UNITEENV* pstEnv)
{
	TOPRANGE stRange;
	TOPRANGE *pstRange;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	int i;
	char szSql[10240];
	TDRDATA stSql;
	
	stSql.iBuff = sizeof(szSql);
	stSql.pszBuff = &szSql[0];

	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "TopRange");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stRange);
	pstEnv->stDestDBObj.pszObj = (char*)&stRange;
	
	iRet = tdr_obj2sql(pstEnv->hSrcDBHandle, &stSql, TDR_DBOP_SELECT, &pstEnv->stDestDBObj,NULL);
	if (0 != iRet)
	{
		printf("���ɲ�ѯ��Դ��¼��SQL���ʧ��: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(pstEnv->hSrcDBHandle));
		return iRet;
	}

	if(tdr_query(&hDBResult, pstEnv->hSrcDBHandle, &stSql) != 0)
	{
		printf("ִ��SQL���ʧ��!\n");
		return -1;
	}

	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows<= 0)
	{
		return -1;
	}

	for(i=0; i<iNumRows; i++ )
	{
		iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
		if (0 != iRet)
		{	
			goto end;
		}
		pstRange = (TOPRANGE*)pstEnv->stDestDBObj.pszObj;
		
	}
	return 0;
}*/

int update_db_of_bill(UNITEENV* pstEnv)
{
	BILL stBill;
	BILL *pstBill;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	int i;
	char szSql[10240];
	LPSHTABLE pstSht = NULL;
	UBILLDATA *pstBillData;
	UBILLDATA stBillData;
	int iUin;
	ACCUIN stAccUin;
	ACCUIN *pstAccUin;
	
	stSql.iBuff = sizeof(szSql);
	stSql.pszBuff = &szSql[0];
	
	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "Bill");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stBill);
	pstEnv->stDestDBObj.pszObj = (char*)&stBill;
	
	iRet = tdr_obj2sql(pstEnv->hDstBillHandle, &stSql, TDR_DBOP_SELECT, &pstEnv->stDestDBObj,"where Uin!='0'");
	if (0 != iRet)
	{
		printf("���ɲ�ѯ��Դ��¼��SQL���ʧ��: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(pstEnv->hDstBillHandle));
		return iRet;
	}

	if(tdr_query(&hDBResult, pstEnv->hDstBillHandle, &stSql) != 0)
	{
		printf("ִ��SQL���ʧ��!\n");
		return -1;
	}
	

	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows> 0)
	{
		pstSht = sht_create(iNumRows*3, iNumRows,sizeof(UBILLDATA), NULL);
		if (NULL == pstSht)
		{
			printf("sht_create error!\n");
			return -1;
		}

		for(i=0; i<iNumRows; i++ )
		{
			iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
			if (0 != iRet)
			{	
				goto end;
			}
			pstBill = (BILL*)pstEnv->stDestDBObj.pszObj;
			stBillData.iUin = pstBill->Uin;
			pstBillData = sht_insert_unique(pstSht, &stBillData, BillDataCmp, BillDataCode);
			if (pstBillData)
			{
				pstBillData->iUin = pstBill->Uin;
				pstBillData->ullAmount = pstBill->Amount;
				pstBillData->ullBonus = pstBill->Bonus;
				pstBillData->ullCashTotalPoints = pstBill->CashTotalPoints;
				pstBillData->ullPoints = pstBill->Points;
				pstBillData->UsedTotalPoints = pstBill->UsedTotalPoints;
			}
		}
	}

	iRet = tdr_obj2sql(pstEnv->hSrcBillHandle, &stSql, TDR_DBOP_SELECT, &pstEnv->stDestDBObj,"where Uin!='0'");
	if (0 != iRet)
	{
		printf("���ɲ�ѯ��Դ��¼��SQL���ʧ��: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(pstEnv->hSrcBillHandle));
		goto end;
	}

	if(tdr_query(&hDBResult, pstEnv->hSrcBillHandle, &stSql) != 0)
	{
		printf("ִ��SQL���ʧ��!\n");
		goto end;
	}
	
	iNumRows = tdr_num_rows(hDBResult);
	if (iNumRows > 0)
	{
		for(i=0; i<iNumRows; i++ )
		{
			iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
			if (0 != iRet)
			{	
				printf("tdr_fetch_row error!\n");
				goto end;
			}
			pstBill = (BILL*)pstEnv->stDestDBObj.pszObj;

			stAccUin.iOldUin = pstBill->Uin;
			pstAccUin = sht_find(pstEnv->pstOverlapAccSht,&stAccUin, AccUinCmp, AccUinCode);
			
			// Ŀ�������ظ��˺� ,uinΪĿ������Uin
			if (pstAccUin)
			{
				stBillData.iUin = pstAccUin->iNewUin;
				pstBillData = NULL;
				
				if (pstSht)
					pstBillData = sht_find(pstSht, &stBillData, BillDataCmp, BillDataCode);
				
				if (pstBillData)
				{
					pstBill->Uin = pstBillData->iUin;
					pstBill->Amount += pstBillData->ullAmount;
					pstBill->Points += pstBillData->ullPoints;
					pstBill->CashTotalPoints += pstBillData->ullCashTotalPoints;
					pstBill->UsedTotalPoints += pstBillData->UsedTotalPoints;
					pstBill->Bonus += pstBillData->ullBonus;
					// update 
					if ((iRet= update_dst_db(pstEnv, &pstEnv->stDestDBObj, DB_FLAG_BILL)) < 0)
					{
						printf("update_dst_db error!\n");
						goto end;
					}
				}
				else
				{
					modify_role_uin(pstEnv, &pstBill->Uin);
					if ((iRet = insert_dest_db(pstEnv, &pstEnv->stDestDBObj, DB_FLAG_BILL)) < 0)
					{
						printf("insert_dest_db error!\n");
						goto end;	
					}
						
				}
			}

			// Ŀ����û���ظ��˺�,Ҫ�޸�Uin
			else/* if (iUin == 0) */
			{
				modify_role_uin(pstEnv, &pstBill->Uin);
				if ((iRet = insert_dest_db(pstEnv, &pstEnv->stDestDBObj, DB_FLAG_BILL)) < 0)
				{
					printf("insert_dest_db error!\n");
					goto end;	
				}
			}
		}
	}

	iRet = 0;
end:

	if (pstSht)
	{
		sht_destroy(&pstSht);
		pstSht = NULL;
	}

	if (iRet != 0)
		iRet = -1;
	
	return iRet;
}


// ɾ����������
int delete_store_record(UNITEENV* pstEnv)
{
	int iRet;
	unsigned int iSrcNum = 0;
	unsigned int iDstNum = 0;
	UINTESTOREDATA *pstData = NULL;
	UINTESTOREDATA *pstOne = NULL;

	iRet = tdr_get_records_count(pstEnv->hDestDBHandle, "Store",NULL, &iDstNum);
	if (iRet < 0)
	{
		printf("tdr_get_records_count 1 error \n");
		return -1;
	}

	iRet = tdr_get_records_count(pstEnv->hSrcDBHandle, "Store",NULL, &iSrcNum);
	if (iRet < 0)
	{
		printf("tdr_get_records_count 2 error \n");
		return -1;
	}

	if (iSrcNum + iDstNum <= 500)
		return 0;

	pstData = malloc((iSrcNum + iDstNum)*sizeof(UINTESTOREDATA));
	if (!pstData)
	{
		printf("malloc  error \n");
		return -1;
	}		

	int i;
	STORE stStore;
	STORE *pstStore;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iNumRows = 0;
	char szSql[1024];
	char szBuff[1024];

	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "Store");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stStore);
	pstEnv->stDestDBObj.pszObj = (char*)&stStore;

	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM Store");
	stSql.iBuff = strlen(stSql.pszBuff);

	iRet = tdr_query(&hDBResult, pstEnv->hSrcDBHandle, &stSql);
	if (0 != iRet)
	{
		printf("tdr_query  error 1\n");
		goto end;
	}
	
	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows != (int)iSrcNum)
	{
		iRet = -1;
		printf("iNumRows != (int)iSrcNum  error \n");
		goto end;
	}

	for (i=0; i<iNumRows; i++)
	{
		iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
		if (0 != iRet)
		{	
			printf("tdr_fetch_row  error 1\n");
			goto end;
		}

		pstStore = (STORE*)pstEnv->stDestDBObj.pszObj;
		pstOne = pstData+i;
		pstOne->cDBFlag = 0;
		pstOne->iID = pstStore->StoreID;
		pstOne->ullExchg = pstStore->StoreInfo.Exchg;
		pstOne->ullTax = pstStore->StoreInfo.Tax; 
		pstOne->iMoney = pstStore->StoreInfo.Money;
		pstOne->iNum = pstStore->StoreInfo.Num;
		memcpy(&pstOne->stGrid[0],&pstStore->StoreInfo.Grid[0], sizeof(pstOne->stGrid[0])*pstStore->StoreInfo.Num);
		strncpy(pstOne->szName, pstStore->Name, sizeof(pstOne->szName));
	}

	iRet = tdr_query(&hDBResult, pstEnv->hDestDBHandle, &stSql);
	if (0 != iRet)
	{
		printf("tdr_query  error 2\n");
		goto end;
	}

	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows != (int)iDstNum)
	{
		iRet = -1;
		printf("iNumRows != (int)iDstNum  error \n");
		goto end;
	}

	for (i=0; i<iNumRows; i++)
	{
		iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
		if (0 != iRet)
		{	
			printf("tdr_fetch_row  error 2\n");
			goto end;
		}

		pstStore = (STORE*)pstEnv->stDestDBObj.pszObj;
		pstOne = pstData+i+iSrcNum;
		pstOne->cDBFlag = 1;
		pstOne->iID = pstStore->StoreID;
		pstOne->ullExchg = pstStore->StoreInfo.Exchg;
		pstOne->ullTax = pstStore->StoreInfo.Tax; 
		pstOne->iMoney = pstStore->StoreInfo.Money;
		pstOne->iNum = pstStore->StoreInfo.Num;
		memcpy(&pstOne->stGrid[0],&pstStore->StoreInfo.Grid[0], sizeof(pstOne->stGrid[0])*pstStore->StoreInfo.Num);
		strncpy(pstOne->szName, pstStore->Name, sizeof(pstOne->szName));
	}
	
	qsort(pstData, iSrcNum + iDstNum, sizeof(UINTESTOREDATA), store_store_cmp);

	stSql.pszBuff = &szSql[0];
	for (i=iSrcNum + iDstNum-1; i>=500; i--)
	{
		pstOne = pstData +i;
		
		snprintf(szSql, sizeof(szSql),"DELETE FROM Store where StoreID=%d", pstOne->iID);
		stSql.iBuff = strlen(stSql.pszBuff);

		if (pstOne->cDBFlag == 0)
		{
			iRet = tdr_query(&hDBResult, pstEnv->hSrcDBHandle, &stSql);
		}
		else
		{
			iRet = tdr_query(&hDBResult, pstEnv->hDestDBHandle, &stSql);
		}

		// �����ʼ��黹�����Ʒ todo:
		if (pstOne->iMoney > 0 || pstOne->iNum > 0)
		{
			if (store_mail(pstEnv, pstOne) < 0)
			{
				printf("store_mail  error 3\n");
				goto end;
			}	
		}

		if (0 != iRet)
		{
			printf("tdr_query  error 3\n");
			goto end;
		}
	}
	
	iRet = 0;
	
end:

	if (pstData)
		free(pstData);
	if (iRet != 0)
		iRet = -1;
	return iRet;
}

static int store_mail(UNITEENV* pstEnv, UINTESTOREDATA *pstOne)
{
	MAILENTRY stMailEntry;
	int i,j;
	int iMail = pstOne->iNum/MAIL_MAX_ITEM +(pstOne->iNum%MAIL_MAX_ITEM?1:0);

	if (iMail == 0)
	{
		memset(&stMailEntry, 0, sizeof(stMailEntry));
		strncpy(stMailEntry.Mail.Head.Recv, pstOne->szName, sizeof(stMailEntry.Mail.Head.Recv));
		strncpy(stMailEntry.Mail.Head.Title, UNITE_STORE1, sizeof(stMailEntry.Mail.Head.Title));
		strncpy(stMailEntry.Mail.Head.Send, UNITE_STORE2, sizeof(stMailEntry.Mail.Head.Send));
		stMailEntry.Mail.Head.Flags |= MAIL_FLAG_SYSTEM;
		stMailEntry.Mail.Head.Time = time(NULL);
		stMailEntry.Mail.Head.WID = unite_get_gid(pstEnv);
		stMailEntry.Mail.Detail.ItemNum = 0;
		stMailEntry.Mail.Detail.Gold = pstOne->iMoney;

		if (up_trade_insert_mailentry(pstEnv, &stMailEntry) < 0)
		{
			return -1;
		}
	}
	else
	{
		for (i=0; i<iMail; i++)
		{
			memset(&stMailEntry, 0, sizeof(stMailEntry));
			strncpy(stMailEntry.Mail.Head.Recv, pstOne->szName, sizeof(stMailEntry.Mail.Head.Recv));
			strncpy(stMailEntry.Mail.Head.Title, UNITE_STORE1, sizeof(stMailEntry.Mail.Head.Title));
			strncpy(stMailEntry.Mail.Head.Send, UNITE_STORE2, sizeof(stMailEntry.Mail.Head.Send));
			stMailEntry.Mail.Head.Flags |= MAIL_FLAG_SYSTEM;
			stMailEntry.Mail.Head.Time = time(NULL);
			stMailEntry.Mail.Head.WID = unite_get_gid(pstEnv);
			stMailEntry.Mail.Detail.ItemNum = 0;

			for (j=i*MAIL_MAX_ITEM; j<pstOne->iNum; j++)
			{
				stMailEntry.Mail.Detail.Items[j-i*MAIL_MAX_ITEM] = pstOne->stGrid[j].Grid;
				stMailEntry.Mail.Detail.ItemNum ++;
				if (stMailEntry.Mail.Detail.ItemNum >= MAIL_MAX_ITEM)
				{
					break;
				}
			}

			if (pstOne->iMoney> 0 && i==0)
			{
				stMailEntry.Mail.Detail.Gold = pstOne->iMoney;
			}

			if (up_trade_insert_mailentry(pstEnv, &stMailEntry)< 0)
			{
				return -1;
			}
		}
	}
	
	
	return 0; 
}


// ɾ����������  ��֤�Ϸ��󹤻����������1000
int delete_clan_record(UNITEENV* pstEnv)
{
	int iRet;
	unsigned int iSrcNum = 0;
	unsigned int iDstNum = 0;
	UINTECLANDATA *pstClanData=NULL;
	UINTECLANDATA *pstClanOne;
	
	iRet = tdr_get_records_count(pstEnv->hDestDBHandle, "Clan",NULL, &iDstNum);
	if (iRet < 0)
	{
		return -1;
	}

	iRet = tdr_get_records_count(pstEnv->hSrcDBHandle, "Clan",NULL, &iSrcNum);
	if (iRet < 0)
	{
		return -1;
	}

	if (iSrcNum + iDstNum <= 1000)
		return 0;

	pstClanData = malloc((iSrcNum + iDstNum)*sizeof(UINTECLANDATA));
	if (!pstClanData)
		return -1;

	int i;
	CLAN stClan;
	CLAN *pstClan;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iNumRows = 0;
	char szSql[1024];
	char szBuff[1024];

	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "Clan");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stClan);
	pstEnv->stDestDBObj.pszObj = (char*)&stClan;


	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM Clan");
	stSql.iBuff = strlen(stSql.pszBuff);

	iRet = tdr_query(&hDBResult, pstEnv->hSrcDBHandle, &stSql);
	if (0 != iRet)
	{
		goto end;
	}
	
	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows != (int)iSrcNum)
	{
		iRet = -1;
		goto end;
	}

	for (i=0; i<iNumRows; i++)
	{
		iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
		if (0 != iRet)
		{	
			goto end;
		}

		pstClan = (CLAN*)pstEnv->stDestDBObj.pszObj;
		pstClanOne = pstClanData+i;
		pstClanOne->cDBFlag = 0;
		pstClanOne->uiActiveVal = pstClan->ClanProf.ActiveValue;
		pstClanOne->usLvl = pstClan->ClanProf.Level;
		pstClanOne->ullWID = pstClan->ClanProf.GID;
	}

	iRet = tdr_query(&hDBResult, pstEnv->hDestDBHandle, &stSql);
	if (0 != iRet)
	{
		goto end;
	}

	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows != (int)iDstNum)
	{
		iRet = -1;
		goto end;
	}

	for (i=0; i<iNumRows; i++)
	{
		iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
		if (0 != iRet)
		{	
			goto end;
		}

		pstClan = (CLAN*)pstEnv->stDestDBObj.pszObj;
		pstClanOne = pstClanData+i+iSrcNum;
		pstClanOne->cDBFlag = 1;
		pstClanOne->uiActiveVal = pstClan->ClanProf.ActiveValue;
		pstClanOne->usLvl = pstClan->ClanProf.Level;
		pstClanOne->ullWID = pstClan->ClanProf.GID;
	}
	
	qsort(pstClanData, iSrcNum + iDstNum, sizeof(UINTECLANDATA), unite_clan_cmp);

	stSql.pszBuff = &szSql[0];
	for (i=iSrcNum + iDstNum-1; i>=1000; i--)
	{
		pstClanOne = pstClanData +i;
		
		snprintf(szSql, sizeof(szSql),"DELETE FROM Clan where GID=%llu", pstClanOne->ullWID);
		stSql.iBuff = strlen(stSql.pszBuff);

		if (pstClanOne->cDBFlag == 0)
		{
			iRet = tdr_query(&hDBResult, pstEnv->hSrcDBHandle, &stSql);
		}
		else
		{
			iRet = tdr_query(&hDBResult, pstEnv->hDestDBHandle, &stSql);
		}
		if (0 != iRet)
		{
			goto end;
		}
	}
	
	iRet = 0;
end:

	if (pstClanData)
		free(pstClanData);
	if (iRet != 0)
		iRet = -1;
	return iRet;
}


int init_acc_uin_sht(UNITEENV* pstEnv)
{
	unsigned int iCount = 0;

	// init sht
	if (tdr_get_records_count(pstEnv->hDstAccHandle, "Account",NULL, &iCount) < 0)
		return -1;

	if (iCount <= 0) 
		return -1;

	pstEnv->pstDstAccUinSht = sht_create(iCount*3, iCount,sizeof(ACCDATA), NULL);
	if (NULL == pstEnv->pstDstAccUinSht)
	{
		printf("init_sht error \n");
		return -1;
	}

	iCount = 0;
	if (tdr_get_records_count(pstEnv->hSrcAccHandle, "Account",NULL, &iCount) < 0)
		return -1;

	if (iCount <= 0) 
		return -1;

	pstEnv->pstSrcAccUinSht = sht_create(iCount*3, iCount,sizeof(ACCDATA), NULL);
	if (NULL == pstEnv->pstSrcAccUinSht)
	{
		printf("init_sht error \n");
		return -1;
	}

	// fill sht
	ACCOUNT stAcc;
	ACCOUNT *pstAcc;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	int i;
	char szSql[10240];
	ACCDATA stAccData;
	ACCDATA *pstAccData;
	ACCUIN stAccUin;
	ACCUIN *pstAccUin;

	stSql.iBuff = sizeof(szSql);
	stSql.pszBuff = &szSql[0];
	
	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "Account");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stAcc);
	pstEnv->stDestDBObj.pszObj = (char*)&stAcc;
	
	iRet = tdr_obj2sql(pstEnv->hDstAccHandle, &stSql, TDR_DBOP_SELECT, &pstEnv->stDestDBObj,"where Uin!='0'");
	if (0 != iRet)
	{
		printf("���ɲ�ѯ��Դ��¼��SQL���ʧ��: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(pstEnv->hDestDBHandle));
		return iRet;
	}

	if(tdr_query(&hDBResult, pstEnv->hDstAccHandle, &stSql) != 0)
	{
		printf("ִ��SQL���ʧ��!\n");
		return -1;
	}
	
	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows < 1)
	{
		return -1;
	}

	for(i=0; i<iNumRows; i++ )
	{
		iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
		if (0 != iRet)
		{	
			printf("tdr_fetch_row error\n");
			return -1;
		}
		
		pstAcc = (ACCOUNT*)pstEnv->stDestDBObj.pszObj;
		stAccData.iUin = pstAcc->Uin;

		// keyΪuin
		pstAccData = sht_insert_unique(pstEnv->pstDstAccUinSht,&stAccData, AccDataUinCmp, AccDataUinCode);
		if (pstAccData)
		{
			pstAccData->iUin = pstAcc->Uin;
			strncpy(pstAccData->szPass9,pstAcc->Pass9Account,sizeof(pstAccData->szPass9));
		}
		else
		{
			return -1;
		}
	}


	iRet = tdr_obj2sql(pstEnv->hSrcAccHandle, &stSql, TDR_DBOP_SELECT, &pstEnv->stDestDBObj,"where Uin!='0'");
	if (0 != iRet)
	{
		printf("���ɲ�ѯ��Դ��¼��SQL���ʧ��: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(pstEnv->hDestDBHandle));
		return iRet;
	}

	if(tdr_query(&hDBResult, pstEnv->hSrcAccHandle, &stSql) != 0)
	{
		printf("ִ��SQL���ʧ��!\n");
		return -1;
	}
	
	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows < 1)
	{
		return -1;
	}

	for(i=0; i<iNumRows; i++ )
	{
		iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
		if (0 != iRet)
		{	
			printf("tdr_fetch_row error\n");
			return -1;
		}
		
		pstAcc = (ACCOUNT*)pstEnv->stDestDBObj.pszObj;
		stAccData.iUin = pstAcc->Uin;

		// keyΪuin
		pstAccData = sht_insert_unique(pstEnv->pstSrcAccUinSht,&stAccData,  AccDataUinCmp, AccDataUinCode);
		if (pstAccData)
		{
			pstAccData->iUin = pstAcc->Uin;
			strncpy(pstAccData->szPass9,pstAcc->Pass9Account,sizeof(pstAccData->szPass9));
		}
		else
		{
			return -1;
		}
	}
	return 0;
}

/*
	��ȡĿ������RegionGlobal, �޸ķ�ID�͹�����
*/
static update_db_of_region_global(UNITEENV* pstEnv)
{
	REGIONGLOBAL stGlobal;
	REGIONGLOBAL *pstGlobal;
	int i,j;

	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "RegionGlobal");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stGlobal);
	pstEnv->stDestDBObj.pszObj = (char*)&stGlobal;

	char szSql[256];
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	int iSrcSvrID = pstEnv->stConf.iSrcWorldID;
	int iDstSvrID = pstEnv->stConf.iDestWorldID;
	int iChg = 0;

	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM RegionGlobal");
	stSql.iBuff = strlen(stSql.pszBuff);
	
	iRet = tdr_query(&hDBResult, pstEnv->hDstRegionHandle, &stSql);
	if (iRet < 0)
	{
		printf("tdr_query is error!\n");
		return -1;	
	}

	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows != 1)
	{
		printf("iNumRows != 1 \n");
		return -1;
	}

	iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
	if (0 != iRet)
	{	
		printf("tdr_fetch_row is error ! \n");
		return -1;
	}

	pstGlobal = (REGIONGLOBAL*)pstEnv->stDestDBObj.pszObj;
	RSTRONGINFO *pstInfo = &pstGlobal->Misc.RStrongInfo;

	for (i=0; i<pstInfo->Num; i++)
	{
		if (pstInfo->Strong[i].Owner.SvrID == iSrcSvrID)
		{
			pstInfo->Strong[i].Owner.ClanID+=pstEnv->iDestClanLastIdx;
			modify_clan_name( pstEnv,pstInfo->Strong[i].Owner.ClanName, sizeof(pstInfo->Strong[i].Owner.ClanName), NULL);
			pstInfo->Strong[i].Owner.SvrID = iDstSvrID;
			iChg=1;
		}

		
		if (pstInfo->Strong[i].Atk.SvrID == iSrcSvrID)
		{
			pstInfo->Strong[i].Atk.ClanID += pstEnv->iDestClanLastIdx;
			modify_clan_name( pstEnv,pstInfo->Strong[i].Atk.ClanName, sizeof(pstInfo->Strong[i].Atk.ClanName), NULL);
			pstInfo->Strong[i].Atk.SvrID = iDstSvrID;
			iChg=1;
		}

		for (j=0; j<pstInfo->Strong[i].Sale.iSale; j++)
		{
			if (pstInfo->Strong[i].Sale.RSale[j].SvrID == iSrcSvrID)
			{
				pstInfo->Strong[i].Sale.RSale[j].SvrID = iDstSvrID;
				pstInfo->Strong[i].Sale.RSale[j].ClanID += pstEnv->iDestClanLastIdx;
				modify_clan_name( pstEnv,pstInfo->Strong[i].Sale.RSale[j].ClanName, 
					sizeof(pstInfo->Strong[i].Sale.RSale[j].ClanName), NULL);
				iChg=1;
			}
		}

		for (j=0; j<pstInfo->Strong[i].Sale.iPlayer; j++)
		{
			if (pstInfo->Strong[i].Sale.RPlayer[j].SvrID == iSrcSvrID)
			{
				pstInfo->Strong[i].Sale.RPlayer[j].SvrID = iDstSvrID;
				pstInfo->Strong[i].Sale.RPlayer[j].ClanID += pstEnv->iDestClanLastIdx;
				modify_role_name( pstEnv, pstInfo->Strong[i].Sale.RPlayer[j].RoleName,
							sizeof(pstInfo->Strong[i].Sale.RPlayer[j].RoleName), NULL);
				iChg=1;
			}
		}
	}

	if (!iChg)
		return 0;

	
	if (update_dst_db(pstEnv, &pstEnv->stDestDBObj, DB_FLAG_REGION) < 0)
	{
		printf("update_dst_db is error ! \n");
		return -1;
	}
	
	return 0;
}

int insert_nameid_by_level(UNITEENV* pstEnv)
{
	int i,j;
	ROLEDATA stRoleData;
	ROLEDATA *pstRoleData;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;
	int iNumRows = 0;
	char szSql[512];
	int iCount = 0;
	char szBuff[1024];
	NAMEID stNameID;
	TDRDBOBJECT stDBObj;
	//int iCount = 0;
	
	
	pstEnv->stDestDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "RoleData");
	pstEnv->stDestDBObj.iVersion = tdr_get_meta_current_version(pstEnv->stDestDBObj.pstMeta);
	pstEnv->stDestDBObj.iObjSize = sizeof(stRoleData);
	pstEnv->stDestDBObj.pszObj = (char*)&stRoleData;


	stSql.pszBuff = &szSql[0];
	snprintf(szSql, sizeof(szSql),"SELECT * FROM RoleData");
	stSql.iBuff = strlen(stSql.pszBuff);
	
	iRet = tdr_query(&hDBResult, pstEnv->hSrcDBHandle, &stSql);
	if (iRet < 0)
	{
		snprintf(szBuff,sizeof(szBuff),"src db execute sql sentence:%s error\n", szSql);
		unite_error_log(pstEnv->fp,szBuff);
		return -1;	
	}
	
	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows < 1)
	{
		return 0;
	}

	stDBObj.pstMeta = tdr_get_meta_by_name(pstEnv->pstMetaLib, "NameID");
	stDBObj.iVersion = tdr_get_meta_current_version(stDBObj.pstMeta);
	stDBObj.iObjSize = sizeof(stNameID);
	stDBObj.pszObj = (char*)&stNameID;

	for(j=0; j<iNumRows; j++ )
	{
		iRet = tdr_fetch_row(hDBResult, &pstEnv->stDestDBObj);
		if (0 != iRet)
		{	
			unite_error_log(pstEnv->fp,"src RoleData table:tdr_fetch_row error\n");
			return -1;
		}

		pstRoleData = (ROLEDATA*)pstEnv->stDestDBObj.pszObj;

		if (pstRoleData->Level < 65)
		{
			continue;
		}

		stNameID.GID = pstRoleData->RoleID;
		stNameID.NameType = 99;
		strncpy(stNameID.Name,pstRoleData->RoleName, sizeof(stNameID.Name));
		stNameID.NewTime = pstRoleData->CreateTime;

		if (insert_dest_db(pstEnv, &stDBObj, DB_FLAG_WORLD) < 0)
		{
			printf("%s insert fail!\n", stNameID.Name);	
		}
		else
		{
			printf("%s insert succ!\n", stNameID.Name);
			iCount++;
		}
	}

	printf("Count=%d\n", iCount);
	return 0;
}

int main(int argc, char* argv[])
{
	UNITEENV stEnv;
	memset (&stEnv, 0, sizeof(stEnv));

	if (unite_check_args(argc, argv) < 0)
	{
		return -1;
	}

	stEnv.fp = fopen("./unite.error","w");
	if (stEnv.fp == NULL)
	{
		printf("fopen error!\n");
		return -1;
	}
		
	if (get_conf(&stEnv.stConf) < 0)
	{
		printf("load config file error!\n");
		return -1;
	}

	if (unite_init(&stEnv) < 0)
	{
		printf("unite_init error!\n");
		goto error;
	}

	// ����
	if (strcmp(argv[1], "account") == 0)
	{
		// ������ID��ͬ
		if (stEnv.stConf.iDestRegionID == stEnv.stConf.iSrcRegionID)
		{
			goto error;
		}
		
		stEnv.cArgsFlag = ACCOUNT_ARGS;
		stEnv.cUpUinFalg = NEED_UP_UIN;

		if (update_db_of_account_pre(&stEnv) < 0)
		{
			printf("update_db_of_account_pre error!\n");
			goto error;
		}

		// uin->pass9
		/*if ( init_acc_uin_sht(&stEnv) < 0)
		{
			printf("init_acc_uin_sht error!\n");
			goto error;
		}*/

		// pass9->uin
		if (init_account_sht(&stEnv) < 0)
		{
			printf("init_account_sht error!\n");
			goto error;
		}

		// ����account��
		if (update_db_of_account(&stEnv) < 0)
		{
			printf("update_db_of_account error!\n");
			goto error;	
		}

		// �޸�Bill��
		if (update_db_of_bill(&stEnv) < 0)
		{
			printf("update_db_of_bill error!\n");
			goto error;
		}

		// �޸�accountrole
		if ( update_db_of_accountrole1(&stEnv) < 0)
		{
			printf("update_db_of_accountrole error!\n");
			goto error;
		}
		
		/*---------------�޸Ľ�ɫ���ݿ�todo---*/
		if ( unite_account_modify_roledata1(&stEnv) < 0)
		{
			printf("unite_account_modify_roledata1 error!\n");
			goto error;	
		}

		if ( unite_account_modufy_shadowroledata1(&stEnv) < 0)
		{
			printf("unite_account_modufy_shadowroledata1 error!\n");
			goto error;	
		}

		if (unite_account_modify_accshare1(&stEnv) < 0)
		{
			printf("unite_account_modify_accshare1 error!\n");
			goto error;
		}

		
	}
	// �Ϸ�
	else if(strcmp(argv[1],"world") == 0)
	{
		stEnv.cArgsFlag = WORLD_ARGS;
		stEnv.cUpUinFalg = ((stEnv.stConf.iDestRegionID != stEnv.stConf.iSrcRegionID) ?
						NEED_UP_UIN : NOT_UP_UIN);

		// ͬһ������Ҫ�ϲ�account
		if (stEnv.stConf.iDestRegionID != stEnv.stConf.iSrcRegionID)
		{	
			// uin->pass9
			if ( init_acc_uin_sht(&stEnv) < 0)
			{
				printf("init_acc_uin_sht error!\n");
				goto error;
			}
			
			// �޸�Bill��
			if (update_db_of_bill(&stEnv) < 0)
			{
				printf("update_db_of_bill error!\n");
				goto error;
			}

			if (init_account_sht(&stEnv) < 0)
			{
				printf("init_account_sht error!\n");
				goto error;
			}
			
			// ����account��
			if (update_db_of_account(&stEnv) < 0)
			{
				printf("update_db_of_account error!\n");
				goto error;	
			}
		}

		// ɾ����������
		if (delete_store_record(&stEnv) < 0)
		{
			printf("delete_store_record error!\n");
			goto error;
		}

		// ɾ����������
		if (delete_clan_record(&stEnv) < 0)
		{
			printf("delete_clan_record error!\n");
			goto error;
		}

		if (delete_db_data(&stEnv) < 0)
		{
			printf("delete_db_data error!\n");
			goto error;
		}

		if (init_nameid_sht(&stEnv) < 0)
		{
			printf("init_nameid_sht error!\n");
			goto error;
		}

		if (init_clan_sht(&stEnv) < 0)
		{
			printf("init_clan_sht error!\n");
			goto error;
		}

		if (fill_sht(&stEnv) < 0)
		{
			printf("fill_sht error\n");
			goto error;
		}

		if (update_db_of_global(&stEnv)<0)
		{
			printf("update_db_of_global error!\n");
			goto error;
		}

		if ( update_db_of_accountrole(&stEnv) < 0)
		{
			printf("update_db_of_accountrole error!\n");
			goto error;
		}

		if (update_db_of_roledata(&stEnv)<0)
		{
			printf("update_db_of_roledata error!\n");
			goto error;
		}

		if (update_db_of_mailentry(&stEnv) < 0)
		{
			printf("update_db_of_mailentry error!\n");
			goto error;
		}

		if (update_db_of_relation(&stEnv) < 0)
		{
			printf("update_db_of_relation error!\n");
			goto error;
		}

		if (updata_db_of_clan(&stEnv) < 0)
		{
			printf("updata_db_of_clan error!\n");
			goto error;
		}

		if (update_db_of_store(&stEnv) < 0)
		{
			printf("update_db_of_store error!\n");
			goto error;
		}

		if (update_db_of_home(&stEnv) < 0)
		{
			printf("update_db_of_home error!\n");
			goto error;
		}

		if ( update_db_of_region_global(&stEnv) < 0)
		{
			printf("update_db_of_region_global error!\n");
			goto error;
		}

		if (update_db_of_toprange(&stEnv) < 0)
		{
			printf("update_db_of_toprange error!\n");
			goto error;
		}

		if (update_db_of_shadowroledata(&stEnv) < 0)
		{
			printf("update_db_of_shadowroledata error!\n");
			goto error;
		}

		if (update_db_of_tradebuy(&stEnv) < 0)
		{
			printf("update_db_of_tradebuy error!\n");
			goto error;
		}

		if (update_db_of_tradesell(&stEnv) < 0)
		{
			printf("update_db_of_tradesell error!\n");
			goto error;
		}

		if (update_db_of_accshare(&stEnv) < 0)
		{
			printf("update_db_of_accshare error!\n");
			goto error;	
		}

		unite_destory_sht(&stEnv);

		// todo: �����ɫ���Ͻ��
		/*if (unite_save_gold_coin(	&stEnv) < 0)
		{
			printf("unite_save_gold_coin error!\n");
			goto error;
		}*/

		unite_destory_sht_acc_uin(&stEnv);
		unite_destory_db_handle(&stEnv);

	}
	else if (strcmp(argv[1],"nameid") == 0)
	{
		insert_nameid_by_level(&stEnv);
	}
	else
	{
		goto error;
	}
	
	printf("execute succ!\n");
	return 0;
	
error:
	unite_destory_sht(&stEnv);
	unite_destory_sht_acc_uin(&stEnv);
	unite_destory_db_handle(&stEnv);
	printf("execute fail!\n");
	return -1;
}
