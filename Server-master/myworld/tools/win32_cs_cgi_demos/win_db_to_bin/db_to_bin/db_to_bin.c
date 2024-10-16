// db_to_bin.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"


#include <stdio.h>
#include "tdr/tdr.h"
#include "pal/tstring.h"
#include "pal/tfile.h"
#include "mysql/mysql.h"
#include <assert.h>
#include <stdlib.h>
#include "ResConv.h"
#include "db_to_bin_desc.h"

extern unsigned char g_szMetalib_db_to_bin[];
unsigned char sKey[]={\
0xd6, 0x02, 0x08, 0x00, 0xff, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0xd0, 0xca, 0x01, 0x00};

int db_to_bin_comp(const void* pv1, const void* pv2)
{
	return *(int*)(pv1) - *(int*)(pv2);
}

int db_to_bin_comp_ll(const void* pv1, const void* pv2)
{
	long long diff = (*(long long *)pv1) - (*(long long *)pv2);

	if (diff > 0) return 1;
	else if (diff < 0) return -1;
	else return 0;
	//long long *p1 = (long long *)(pv1);
	//long long *p2 = (long long *)(pv2);

//	return *(long long *)(pv1) - *(long long *)(pv2);
}

int init_dbhandle(LPTDRMETALIB* pstResLib,TDRDBHANDLE *hResDBHandle, DB_TO_BIN_CONF *pstConf)
{
	TDRDBMS stDBMS;
	char szErrorMsg[2048]={0};
	DB_TO_BIN_CONF stConf;
	LPTDRMETALIB pstLib;
	TDRDATA stHost;
	LPTDRMETA pstMeta = NULL;

	stHost.iBuff = sizeof(stConf);
	stHost.pszBuff = (char *)&stConf;

	pstLib = (LPTDRMETALIB)&g_szMetalib_db_to_bin[0];

	pstMeta = tdr_get_meta_by_name(pstLib, "db_to_bin_conf");
	if(NULL == pstMeta)
	{
		return -1;
	}
	if( tdr_input_file(pstMeta, &stHost, "db_to_bin.xml", 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME) < 0)
	{
		printf("加载配置文件失败!!!\n");
		return -1;
	}

	if (0 > tdr_load_metalib(pstResLib, stConf.szMetaPath))
	{
		printf("load meta lib resdb_meta fail\n");
		return -1;
	}

	memset(&stDBMS, 0, sizeof(stDBMS));
	STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
	STRNCPY(stDBMS.szDBMSCurDatabaseName, stConf.szResDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
	STRNCPY(stDBMS.szDBMSConnectionInfo, stConf.szResDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
	STRNCPY(stDBMS.szDBMSUser, stConf.szResDBUser, sizeof(stDBMS.szDBMSUser));
	STRNCPY(stDBMS.szDBMSPassword, stConf.szResDBPasswd, sizeof(stDBMS.szDBMSPassword));
	stDBMS.iReconnectOpt = 1;	

	if (0 > tdr_open_dbhanlde(hResDBHandle, &stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s", szErrorMsg);
		return -1;
	}

	memcpy(pstConf, &stConf, sizeof(*pstConf));
	return 0;
}

int bin_confuse(char *pszFileName)
{

	unsigned char *pszLibC;
	struct stat stStat;
	FILE *fp;
	int i;

	if (0 != stat(pszFileName, &stStat))
	{
		return -1;
	}

	fp = fopen(pszFileName, "rb");
	if (NULL == fp)
	{
		return -1;
	}

	pszLibC = (unsigned char *)malloc(stStat.st_size);
	if (NULL == pszLibC)
	{
		return -1;
	}

	fread(pszLibC, stStat.st_size, 1, fp);
	fclose(fp);

	tfdelete(pszFileName);

	for (i=0; i<stStat.st_size; i++)
	{
		*(pszLibC+i) ^= sKey[i%16];
	}

	fp = fopen(pszFileName, "wb");
	if (NULL == fp)
	{
		return -1;
	}

	fwrite(pszLibC, stStat.st_size, 1, fp);

	fclose(fp);
	free(pszLibC);	

	return 0;
}


static int res_mem_to_bin(LPTDRMETA pstMeta, char *pData, int iUnit, int iCount, char *pszBinPath)
{
	char szBinFile[1024];
	char *pch;
	int iLen;
	FILE *fp;
	TRESHEAD stResHead;
	int iWrite;
	int i;

	STRNCPY(szBinFile, pszBinPath, sizeof(szBinFile));
	pch = strrchr(szBinFile, '.');
	if (NULL != pch)
	{
		*pch = '\0';
	}
	iLen = (int)strlen(szBinFile);
	if ((iLen + strlen(".bin")) >= sizeof(szBinFile))
	{
		printf("生成保存资源信息的bin文件名失败\n");
		return -1;
	}
	pch = &szBinFile[iLen];
	STRNCPY(pch, ".bin", (sizeof(szBinFile) - iLen));

	fp = fopen(szBinFile, "wb");
	if (NULL == fp)
	{
		printf("以‘写’的方式打开文件(%s)失败\n", szBinFile);
		return -1;
	}

	memset(&stResHead, 0, sizeof(stResHead));
	stResHead.iCount = iCount;
	stResHead.iUnit = tdr_get_meta_size(pstMeta);
	stResHead.iMagic = TRES_FILE_MAGIC;
	stResHead.iVersion = TDR_METALIB_RESCONV_VERSION;
	stResHead.iResVersion = tdr_get_meta_current_version(pstMeta);
	iWrite = fwrite(&stResHead, sizeof(stResHead), 1, fp);
	if (iWrite != 1)
	{
		printf("向资源文件(%s)中写信息失败\n",szBinFile);
		return -1;
	}

	for (i=0; i<iCount; i++)
	{

		iWrite = fwrite(pData+iUnit*i, stResHead.iUnit, 1, fp);
		if (iWrite != 1)
		{
			printf("向资源文件(%s)中写信息失败\n",szBinFile);
			return  -1;
		}
	}

	fclose(fp);

	return 0;
}

static int tdbms_fetch_all_res(TDRDBHANDLE a_hDBHandle, TDRDBRESULTHANDLE a_hDbResult,  LPTDRMETA a_pstMeta, 
							   int iUnitSize, int iMetaSize,  int a_iVersion, char *a_pszDataBuff,  unsigned int *a_pdwResCount)
{	int iRet = 0;	
unsigned int dwCount;
TDRDBOBJECT stDBObj;

assert(NULL != a_pszDataBuff);	
assert(NULL != a_pstMeta);
assert(NULL != a_pdwResCount);
assert(0 < *a_pdwResCount);
assert(0 != a_hDBHandle);	
assert(0 != a_hDbResult); 

dwCount = 0;
while (dwCount < *a_pdwResCount)	
{		
	stDBObj.iObjSize = iUnitSize;
	stDBObj.pszObj = a_pszDataBuff + iUnitSize * dwCount;
	memset(stDBObj.pszObj + iMetaSize, 0, iUnitSize-iMetaSize);

	stDBObj.pstMeta = a_pstMeta;
	stDBObj.iVersion = a_iVersion;
	iRet = tdr_fetch_row(a_hDbResult, &stDBObj);
	if (0 != iRet)
	{			
		if ((TDR_ERR_GET_ERROR_CODE(iRet) != TDR_ERROR_DB_NO_RECORD_IN_RESULTSET) ||	
			(0 != tdr_dbms_errno(a_hDBHandle)))			
		{				
			printf("从数据库表中读取数据记录失败:%s, dbms  error:%s\n",					
				tdr_error_string(iRet), tdr_dbms_error(a_hDBHandle));
		}
		else	
		{		
			iRet = 0; /*已经取出所有记录*/			
		}	
		break;		
	}/*if (0 != iRet)*/	

	dwCount++; /*取下一条记录*/	
}/*while (dwCount < *a_pdwResCount)*/	

*a_pdwResCount = dwCount;	
return iRet;
}

static int load_resdb(LPTDRMETALIB pstResLib, TDRDBHANDLE hResDBHandle,LPTDRMETA pstMeta,char *pszMetaName, 
					  char *pAddr, int iCount, int iMetaSize)
{
	unsigned int dwResCount = iCount;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iWriteLen;
	TDRDATA stSql;
	char szSql[102400];

	((void)pstResLib);

	if (iCount > 0)
	{
		iWriteLen = _snprintf(szSql, sizeof(szSql)-1, "SELECT * FROM %s WHERE 1", pszMetaName);	
		if ((0 > iWriteLen) || (iWriteLen >= (int)(sizeof(szSql)-1)))	
		{		
			return -1;	
		}	
		stSql.iBuff = iWriteLen;	
		stSql.pszBuff = &szSql[0];	
		if ( 0 > tdr_query_quick(&hDBResult, hResDBHandle, &stSql))
		{
			return -1;
		}

		tdbms_fetch_all_res(hResDBHandle, hDBResult, pstMeta,  iMetaSize, iMetaSize, 0, pAddr, &dwResCount);
		tdr_free_dbresult(&hDBResult);
	}

	if (dwResCount != (unsigned int)iCount)
	{
		return -1;
	}

	printf("load %s count %d\n", pszMetaName, dwResCount);

	return 0;
}

static int res_to_bin(LPTDRMETALIB pstResLib, TDRDBHANDLE hResDBHandle, char *pszMetaName)
{
	char *pAddr = NULL;
	int iMetaSize;
	unsigned int iResCount;
	LPTDRMETA pstMeta;
	char stPathName[256];
	char stPathName1[256];
	char szCmd[1024];

	if (strcmp(pszMetaName, "test_MonsterDef") == 0)
	{
		return 0;
	}

	pstMeta = tdr_get_meta_by_name(pstResLib, pszMetaName);
	if (NULL == pstMeta)
	{
		printf("get meta %s fail\n", pszMetaName);
		return -1;
	}

	iMetaSize = tdr_get_meta_size(pstMeta);
	if (iMetaSize <= 0)
	{
		return -1;
	}

	iResCount = 0;	
	if (0 > tdr_get_records_count(hResDBHandle, pszMetaName, "", &iResCount))
	{
		return -1;
	}
	if (iResCount == 0)
	{
		//	return 0;
	}

	pAddr = (char*)malloc(iMetaSize * iResCount );
	if (!pAddr)
	{
		return -1;
	}
	memset (pAddr, 0, iMetaSize * iResCount);

	if (load_resdb(pstResLib, hResDBHandle,pstMeta,pszMetaName, pAddr, iResCount, iMetaSize) < 0)
	{
		return -1;
	}

	if (iResCount > 1)
	{
		if (strcmp(pszMetaName, "DiffBootyDef") == 0)
		{
			qsort(pAddr,  iResCount, iMetaSize, db_to_bin_comp_ll);
		}
		else
		{
			qsort(pAddr,  iResCount, iMetaSize, db_to_bin_comp);
		}

	}

	tfmkdir("./svr_res");
	tfmkdir("./clt_res");

	sprintf(stPathName,"./svr_res/%s.bin",pszMetaName);
	if(res_mem_to_bin(pstMeta, pAddr, iMetaSize, iResCount, stPathName) < 0)
	{
		return -1;
	}

	sprintf(stPathName,"./clt_res/%s.bin",pszMetaName);
	if(res_mem_to_bin(pstMeta, pAddr, iMetaSize, iResCount, stPathName) < 0)
	{
		return -1;
	}

	bin_confuse(stPathName);

	free(pAddr);
	return 0;
}

/*void  free_addr(char **pAddr)
{
if (*pAddr)
{
free(*pAddr);
*pAddr = NULL;
}
}*/

/*int get_unit(LPTDRMETALIB pstResLib,  char *pszMetaName)
{
LPTDRMETA pstMeta;
int iMetaSize;

pstMeta = tdr_get_meta_by_name(pstResLib, pszMetaName);
if (NULL == pstMeta)
{
printf("get meta %s fail\n", pszMetaName);
return -1;
}

iMetaSize = tdr_get_meta_size(pstMeta);
if (iMetaSize <= 0)
{
return -1;
}

return iMetaSize;
}*/

int main(int argc, char* argv[])
{
	LPTDRMETALIB pstResLib;
	TDRDBHANDLE hResDBHandle;
	DB_TO_BIN_CONF stConf;
	MYSQL *conn;
	int err;
	MYSQL_RES *res;
	MYSQL_ROW row;

	if( init_dbhandle(&pstResLib, &hResDBHandle, &stConf) < 0)
	{
		return -1;
	}

	conn = mysql_init(NULL);
	if(!conn)
		return -1;

	if(!mysql_real_connect(
		conn,
		stConf.szResDBConnect,
		stConf.szResDBUser,
		stConf.szResDBPasswd,
		stConf.szResDBName,
		0,
		NULL,
		CLIENT_COMPRESS))
	{
		return -1;
	}

	err = mysql_query(conn, "SHOW TABLES");
	if(err)
		return -1;

	res = mysql_store_result(conn);
	if(!res)
		return -1;

	row = NULL;
	while((row = mysql_fetch_row(res)) != NULL)
	{
		if(!row[0])
			continue;

		res_to_bin(pstResLib, hResDBHandle, (char*)(row[0]));
	}

	tdr_close_dbhanlde(&hResDBHandle);

	printf("db_to_bin is OK\n");
	return 0;
}


