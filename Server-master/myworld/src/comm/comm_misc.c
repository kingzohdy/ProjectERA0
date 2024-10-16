/*
**  @file $RCSfile: comm_misc.c,v $
**  general description of this module
**  $Id: comm_misc.c,v 1.93 2013/02/27 03:17:30 tjw Exp $
**  @author $Author: tjw $
**  @date $Date: 2013/02/27 03:17:30 $
**  @version $Revision: 1.93 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "comm_misc.h"
#include "pal/tresource.h"
#include <mysqld_error.h>
#include "tresloader.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "math.h"
#include "tbus/tbus.h"

/* 常量定义 */
const double INF = 1E200;
const double EP = 1E-10;
const int MAXV = 300;
const double PI = 3.14159265;

static char gs_szSql[1024000];

static unsigned char upcase[256] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
   16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
  ' ','!','"','#','$','%','&', 39,'(',')','*','+',',','-','.','/',
  '0','1','2','3','4','5','6','7','8','9',':',';','<','=','>','?',
  '@','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O',
  'P','Q','R','S','T','U','V','W','X','Y','Z','[', 92,']','^','_',
  '`','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O',
  'P','Q','R','S','T','U','V','W','X','Y','Z','{','|','}','~',127,
  128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
  144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
  160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
  176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
  192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
  208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
  224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
  240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255};
#define UPC(x) (upcase[(unsigned char)(x)])

int set_rlimit(unsigned int uiMaxConn)
{
	struct rlimit rlim;

	rlim.rlim_cur = rlim.rlim_max = uiMaxConn + 100; 
	if (setrlimit(RLIMIT_NOFILE, &rlim) < 0) {
		perror("setlimit");
		return -1;
	}
	getrlimit(RLIMIT_NOFILE, &rlim);
	printf("Max open file: %ld\n", rlim.rlim_cur);
	return 0;
}

int ss_han_create(unsigned char *pszLib, LPSSHANDLE *ppSSHandle)
{
	int iRet = 0;
	SSHANDLE *pstSSHandle = NULL;

	pstSSHandle = (SSHANDLE *)malloc(sizeof(SSHANDLE));
	if (!pstSSHandle)
	{
		return -1;
	}

	pstSSHandle->pstSSProtoLib = (LPTDRMETALIB)pszLib;
	
	if (0 == iRet)
	{
		pstSSHandle->pstSSProtoPkgMeta = tdr_get_meta_by_name(pstSSHandle->pstSSProtoLib, "SSPkg");
		if (!pstSSHandle->pstSSProtoPkgMeta)
		{
			iRet = -1;
		}
	}

	if (0 > iRet)
	{
		free(pstSSHandle);
	}
	else
	{
		*ppSSHandle = pstSSHandle;
	}
	
	return iRet;
}

int ss_han_destroy(LPSSHANDLE *ppSSHandle)
{
	free(*ppSSHandle);
	*ppSSHandle = NULL;
	
	return 0;
}

int ss_unpack(SSHANDLE  *pstSSHandler, char *pszBuf, int iBuf, SSPKG *pstPkg, int iVersion)
{
	TDRDATA stHost;
	TDRDATA stNet;
	
		
	stNet.pszBuff = pszBuf;
	stNet.iBuff = iBuf;
	stHost.pszBuff = (char *)pstPkg;
	stHost.iBuff = sizeof(SSPKG);

	return tdr_ntoh(pstSSHandler->pstSSProtoPkgMeta, &stHost, &stNet, iVersion);
}

int ss_pack(SSHANDLE  *pstSSHandler, TDRDATA *pstNet, SSPKG *pstPkg, int iVersion)
{
	TDRDATA stHost;

	stHost.iBuff = sizeof(SSPKG);
	stHost.pszBuff = (char *)pstPkg;

	pstPkg->Head.Ver = TDR_METALIB_PROTO_SS_VERSION;

	return tdr_hton(pstSSHandler->pstSSProtoPkgMeta,  pstNet, &stHost, iVersion);
}

int cmp_modify_time(char *sFile, time_t *plMTime)
{
	struct stat stStat;

	if	(stat(sFile, &stStat) < 0) return -1;
	if (*plMTime == stStat.st_mtime) return 0;
	*plMTime = stStat.st_mtime;
	
	return 1;
}

int bsearch_int (const void *key, const void *base, int nmemb, int size, int *piEqual)
{
	size_t l, u, idx;
	const void *p, *p2;
	int comparison, comparison2;

	*piEqual = 0;
	if (nmemb < 0) return -1;
	if (!nmemb) return 0;
	l = 0;
	u = nmemb;
	
	while (l < u)
	{
		idx = (l + u) / 2;
		p = (void *) (((const char *) base) + (idx * size));
		comparison = *(int *)key - *(int *)p ;
		
		if (comparison == 0)
		{
			*piEqual = 1;
			return idx;
		}		
		else if (comparison < 0)
		{
			if (idx == 0) return idx;
			
			p2 = (void *) (((const char *) base) + ((idx - 1) * size));
			comparison2 = *(int *)key - *(int *)p2 ;
			
			if (comparison2 > 0) return idx;
			
			u = idx;
		}
		else /*if (comparison > 0)*/ 
		{
			l = idx + 1;
		}		
	}
	
	return u;
}

int binsert_int (const void *key, const void *base, int *pnmemb, int size, int iUnique)
{
	int i, iInsert, iEqu;
	
	iInsert = bsearch_int(key, base, *pnmemb, size, &iEqu);
	if (iInsert < 0) return 0;
	if (iEqu && iUnique) return 0;
	if (iInsert < *pnmemb)
	{
		//for (i = *pnmemb; i > iInsert; i--) memcpy((char *)base + i * size, (char *)base + (i - 1)*size, size);
		i = *pnmemb;
		//安全拷贝下
		memmove((char *)base + (iInsert+1) * size, (char *)base + (iInsert)*size, size*(i-iInsert));
	}
	memcpy((char *)base + iInsert*size, key, size);
	(*pnmemb)++;
	return 1;
}

int bdelete_int (const void *key, const void *base, int *pnmemb, int size)
{
	int i, iDelete, iEqu;

	iDelete = bsearch_int(key, base, *pnmemb, size, &iEqu);
	if (!iEqu) return 0;
	if (iDelete < *pnmemb)
	{
		//for (i = iDelete; i < (*pnmemb) - 1; i++) memcpy((char *)base + i * size, (char *)base + (i + 1)*size, size);
		i = *pnmemb;
		memmove((char *)base + (iDelete) * size, (char *)base + (iDelete+1)*size, size*(i-iDelete-1));
	}
	(*pnmemb)--;
	return 1;
}

int my_bsearch (const void *key, const void *base, size_t nmemb, size_t size, int *piEqual, int (*compar) (const void *, const void *))
{
	size_t l, u, idx;
	const void *p, *p2;
	int comparison, comparison2;
	
	*piEqual = 0;
	if (!nmemb) return 0;
	l = 0;
	u = nmemb;
	
	while (l < u)
	{
		idx = (l + u) / 2;
		p = (void *) (((const char *) base) + (idx * size));
		comparison = (*compar) (key, p);
		
		if (comparison == 0)
		{
			*piEqual = 1;
			return idx;
		}		
		else if (comparison < 0)
		{
			if (idx == 0) return idx;
			
			p2 = (void *) (((const char *) base) + ((idx - 1) * size));
			comparison2 = (*compar) (key, p2);
			
			if (comparison2 > 0) return idx;
			
			u = idx;
		}
		else /*if (comparison > 0)*/ 
		{
			l = idx + 1;
		}		
	}
	
	return u;
}

int my_bdelete(const void *key, const void *base, size_t *pnmemb, size_t size, int (*compar) (const void *, const void *))
{
int i, iDelete, iEqu;

	iDelete = my_bsearch(key, base, *pnmemb, size, &iEqu, compar);
	if (!iEqu) return 0;
	if (iDelete < (int)*pnmemb)
	{
		//for (i = iDelete; i < (int)(*pnmemb) - 1; i++) memcpy((char *)base + i * size, (char *)base + (i + 1)*size, size);
		i = *pnmemb;
		memmove((char *)base + (iDelete) * size, (char *)base + (iDelete+1)*size, size*(i-iDelete-1));
	}
	(*pnmemb)--;
	return 1;
}

int my_binsert(const void *key, const void *base, size_t *pnmemb, size_t size, int iUnique, int (*compar) (const void *, const void *))
{
int i, iInsert, iEqu;

	iInsert = my_bsearch(key, base, *pnmemb, size, &iEqu, compar);
	if (iEqu && iUnique) return 0;
	if (iInsert < (int)*pnmemb)
	{
		//for (i = *pnmemb; i > iInsert; i--) memcpy((char *)base + i * size, (char *)base + (i - 1)*size, size);
		i = *pnmemb;
		memmove((char *)base + (iInsert+1) * size, (char *)base + (iInsert)*size, size*(i-iInsert));
	}
	memcpy((char *)base + iInsert*size, key, size);
	(*pnmemb)++;
	return 1;
}

char* cistrstr(char *s1, char *s2)
{
	 register char *p1,*p2;
	 p1 = s1;
	 p2 = s2;
	 while (*s1 && *s2) {
		if (UPC(*s1) == UPC(*s2)) {
			s1++;
			s2++;
		} else {
			s1 = ++p1;
			s2 = p2;
		}
	}
	if (!*s2) return p1;
	return NULL;		
}

int cistrcmp(char *s1, char *s2)
{
    register char c1;
    while ((c1 = *s1) && UPC(c1) == UPC(*s2)) {
	s1++;
	s2++;
    }
    return UPC(c1) - UPC(*s2);
}

int res_mem_to_xml(LPTDRMETA pstMeta, char *pszOutFile, char *pData, int iUnit)
{
	char szXmlFile[1024];
	char *pch;
	int iLen;
	FILE *fp;
	int iRet;
	TDRDATA stHost;
	
	STRNCPY(szXmlFile, pszOutFile, sizeof(szXmlFile));
	pch = strrchr(szXmlFile, '.');
	if (NULL != pch)
	{
		*pch = '\0';
	}
	iLen = (int)strlen(szXmlFile);
	if ((iLen + strlen(".xml")) >= sizeof(szXmlFile))
	{
		printf("生成保存资源信息的xml文件名失败\n");
		return -1;
	}
	pch = &szXmlFile[iLen];
	STRNCPY(pch, ".xml", (sizeof(szXmlFile) - iLen));

	fp = fopen(szXmlFile, "w");
	if (NULL == fp)
	{
		printf("以‘写’的方式打开文件(%s)失败\n", szXmlFile);
		return -1;
	}

	stHost.iBuff = iUnit;
	stHost.pszBuff = pData;
	iRet = tdr_output_fp(pstMeta, fp, &stHost, 0, TDR_IO_NEW_XML_VERSION);
	if (0 != iRet)
	{
		printf(" 将结构体数据输出到XML文件中失败：%s\n", 
				tdr_error_string(iRet));
	}

	fclose(fp);

	return iRet;
}

int res_update_db(TDRDBHANDLE hDBHandle, TDRDBOBJECT *pstDBObj)
{
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iRet = 0;


	/*生成插入数据的sql语句*/
	stSql.iBuff = sizeof(gs_szSql);
	stSql.pszBuff = &gs_szSql[0];
	iRet = tdr_obj2sql(hDBHandle, &stSql, TDR_DBOP_UPDATE, pstDBObj, NULL);
	if (0 != iRet)
	{
		printf("生成更新资源记录的SQL语句失败: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(hDBHandle));
		return iRet;
	}

	/*执行sql语句*/
	iRet = tdr_query(&hDBResult, hDBHandle, &stSql);
	tdr_free_dbresult(&hDBResult);
	if (0 != iRet)
	{
		printf("执行更新资源记录的SQL语句失败: %s dbms error:%s\n",
				tdr_error_string(iRet), tdr_dbms_error(hDBHandle));		
	}/*if (0 != iRet)*/

	return iRet;
}

int res_mem_to_db(TDRDBHANDLE hDBHandle, LPTDRMETA pstMeta, char* pszData, int iUnit)
{
	int iRet = 0;
	TDRDBOBJECT stDBObj;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;

	stDBObj.pstMeta = pstMeta;
	stDBObj.iVersion = tdr_get_meta_current_version(pstMeta);
	stDBObj.iObjSize = iUnit;
	stDBObj.pszObj = pszData;
	stSql.iBuff = sizeof(gs_szSql);
	stSql.pszBuff = &gs_szSql[0];

	iRet = tdr_obj2sql(hDBHandle, &stSql, TDR_DBOP_INSERT, &stDBObj, NULL);
	if (0 != iRet)
	{
		printf("生成插入资源记录的SQL语句失败: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(hDBHandle));
		return -1;
	}

	/*执行sql语句*/
	iRet = tdr_query(&hDBResult, hDBHandle, &stSql);
	tdr_free_dbresult(&hDBResult);
	if (0 != iRet)
	{
		//如果是主键冲突，则以更新记录的方式导入
		int iDbmsErrNo = tdr_dbms_errno(hDBHandle);		
		if (ER_DUP_KEY == iDbmsErrNo || ER_DUP_ENTRY == iDbmsErrNo || ER_DUP_UNIQUE == iDbmsErrNo)
		{
			iRet = res_update_db(hDBHandle, &stDBObj);
			if (0 != iRet)
			{
				return -1;
			}
		}else
		{
			printf("执行插入资源记录的SQL语句失败: %s dbms error:%s\n",
				tdr_error_string(iRet), tdr_dbms_error(hDBHandle));
			return -1;
		}
	}
	
	return 0;
}

int load_resfile_one(char *pszPath, char *pszMetaName, char *pAddr, int *piCount, 
							time_t *ptTime, int iUnitSize, int iUnitMax, int *piReload)
{
	char szFile[64];
	int iCount;
	int iBuffLen;
	int iUnit;
	char *pszDataBuff=NULL;
	int i;
	int iRet;
	
	snprintf(szFile, sizeof(szFile), "%s/%s.bin", pszPath, pszMetaName);
	iRet = cmp_modify_time(szFile, ptTime);
	if (0 > iRet)
	{
		printf( "load res file %s fail", szFile);
		return -1;
	}
	else if (1 == iRet)
	{
		*piReload = 1;
		
		iBuffLen = 0;
		iUnit = 0;
		iCount = trl_load(&pszDataBuff, &iBuffLen, &iUnit, szFile, NULL, RL_FLAG_INGORE_DIFF_HASH);

		if (0 > iCount)
		{
			*ptTime = 0;
			printf("trl_load %s fail, ret=%d", szFile, iCount);
			return -1;
		}

		if (iUnit > iUnitSize || iCount > iUnitMax)
		{
			*ptTime = 0;
			printf("trl_load %s fail, unit=%d, count=%d", szFile, iUnit, iCount);
			return -1;
		}

		for (i=0; i<iCount; i++)
		{
			memcpy(pAddr+i*iUnitSize, pszDataBuff+iUnit*i, iUnit);
			memset(pAddr+i*iUnitSize + iUnit, 0, iUnitSize - iUnit);
		}

		*piCount = iCount;

		trl_unload(&pszDataBuff);

		printf("load %s count %d\n", pszMetaName, iCount);
	}
	else
	{
		*piReload = 0;
	}
	
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

int load_resdb_one(LPTDRMETALIB pstResLib, TDRDBHANDLE hResDBHandle, char *pszMetaName, char *pAddr, int *piCount, int iUintSize, int iUnitMax)
{
	unsigned int dwResCount = 0;
	TDRDBRESULTHANDLE hDBResult = 0;
	int iWriteLen;
	TDRDATA stSql;
	char szSql[102400];
	LPTDRMETA pstMeta;
	int iMetaSize;

	pstMeta = tdr_get_meta_by_name(pstResLib, pszMetaName);
	if (NULL == pstMeta)
	{
		printf("get meta %s fail\n", pszMetaName);
		return -1;
	}

	iMetaSize = tdr_get_meta_size(pstMeta);

	if (iUintSize < iMetaSize)
	{
		printf("meta %s unitsize too small %d<%d\n", pszMetaName, iUintSize, tdr_get_meta_size(pstMeta));
		return -1;
	}
	
	dwResCount = 0;	
	if (0 > tdr_get_records_count(hResDBHandle, pszMetaName, "", &dwResCount))
	{
		return -1;
	}
	if ((int)dwResCount > iUnitMax)
	{
		printf( "load %s count %d > %d\n", pszMetaName, dwResCount, iUnitMax);
		return -1;
	}
	
	if (dwResCount > 0)
	{
		iWriteLen = snprintf(szSql, sizeof(szSql)-1, "SELECT * FROM %s WHERE 1", pszMetaName);	
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

		tdbms_fetch_all_res(hResDBHandle, hDBResult, pstMeta,  iUintSize, iMetaSize, 0, pAddr, &dwResCount);
		tdr_free_dbresult(&hDBResult);
	}

	*piCount = dwResCount;

	printf("load %s count %d\n", pszMetaName, dwResCount);

	return 0;
}

/*int load_resdb(LPTDRMETALIB pstResLib, TDRDBHANDLE hResDBHandle,LPTDRMETA pstMeta,char *pszMetaName, 
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
		iWriteLen = snprintf(szSql, sizeof(szSql)-1, "SELECT * FROM %s WHERE 1", pszMetaName);	
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
}*/

SUITSET *find_suitset(char *pAddr, int iCount,int iUnit, int iSuitSetID)
{
	SUITSET  stSuitSet;
	int iIndex;
	int iEqu;
	
	stSuitSet.SuitSetID = iSuitSetID;
	iIndex = bsearch_int(&stSuitSet, pAddr, iCount, iUnit, &iEqu);
	if (iEqu)
	{
		return (SUITSET *)(pAddr+iIndex*iUnit);
	}
	
	return NULL;	
}

/*
int z_check_pinzhidef(char *pAddrPinZhiDef, int iCount, int iUnit)
{
	int i;
	PETPINZHIDEF *pstPinZhiDef;

	for (i=0; i<iCount; i++)
	{
		pstPinZhiDef =  (PETPINZHIDEF *)(pAddrPinZhiDef + i * iUnit);

		if(pstPinZhiDef->MaxVal <= pstPinZhiDef->MinVal)
		{
			printf("pstPinZhiDef->MaxVal - pstPinZhiDef->MinVal errer\n");
			return -1;
		}

		if (pstPinZhiDef->ZhuanShenQianLiMax <= pstPinZhiDef->ZhuanShenQianLiMin)
		{
			printf("pstPinZhiDef->ZhuanShenQianLiMax - pstPinZhiDef->ZhuanShenQianLiMin errer\n");
			return -1;
		}

		if (pstPinZhiDef->ZhuanShenNengLiMax <=  pstPinZhiDef->ZhuanShenNengLiMin)
		{
			printf("pstPinZhiDef->ZhuanShenNengLiMax - pstPinZhiDef->ZhuanShenNengLiMin errer\n");
			return -1;
		}
	}
	return 0;
}

int z_check_petsubracedef(char *pAddrPetSubRace, int iCountPetSubRace, int iPetSubRaceUnit,
							char *pAddrPetDef, int iCountPetDef, int iPetDefUnit)
{	
	int i,j,k;
	int iCount;
	char *p;
	int iSkillMin = SHORT_MAX;
	int iSkillMax = -1;
	int iRaceMin = SHORT_MAX;
	int iRaceMax = -1;
	int iSubRaceMin = SHORT_MAX;
	int iSubRaceMax = -1;
	PETSUBRACEDEF *pstPetSubRaceDef;
	PETDEF *pstPetDef;

	for (i=0; i<iCountPetSubRace; i++)
	{
		pstPetSubRaceDef = (PETSUBRACEDEF *)(pAddrPetSubRace + i * iPetSubRaceUnit);

		if (pstPetSubRaceDef->Race < iRaceMin)
		{
			iRaceMin = pstPetSubRaceDef->Race;
		}

		if (pstPetSubRaceDef->Race > iRaceMax)
		{
			iRaceMax = pstPetSubRaceDef->Race;
		}

		if (pstPetSubRaceDef->SubRace < iSubRaceMin)
		{
			iSubRaceMin = pstPetSubRaceDef->SubRace;
		}

		if (pstPetSubRaceDef->SubRace > iSubRaceMax)
		{
			iSubRaceMax = pstPetSubRaceDef->SubRace;
		}

		for (j=0;j<MAX_PET_ADDED_SKILL;j++)
		{

			if (pstPetSubRaceDef->AddedSkills[j] == 0)
			{
				break;
			}
			
			if ((int)pstPetSubRaceDef->AddedSkills[j] < iSkillMin)
			{
				iSkillMin = pstPetSubRaceDef->AddedSkills[j];
			}

			if ((int)pstPetSubRaceDef->AddedSkills[j] > iSkillMax)
			{
				iSkillMax = pstPetSubRaceDef->AddedSkills[j];
			}
		}
	}

	for (i=0; i<iCountPetDef; i++)
	{
		pstPetDef =  (PETDEF *)(pAddrPetDef + i * iPetDefUnit);
		if (pstPetDef->Race < iRaceMin)
		{
			iRaceMin = pstPetDef->Race;
		}

		if (pstPetDef->Race > iRaceMax)
		{
			iRaceMax = pstPetDef->Race;
		}

		if (pstPetDef->SubRace < iSubRaceMin)
		{
			iSubRaceMin = pstPetDef->SubRace;
		}

		if (pstPetDef->SubRace > iSubRaceMax)
		{
			iSubRaceMax = pstPetDef->SubRace;
		}
		
		for (j=0; j<MAX_PET_HIDE_SKILL; j++)
		{
			if (pstPetDef->HideSkills[j] == 0)
			{
				break;
			}

			if ((int)pstPetDef->HideSkills[j] < iSkillMin)
			{
				iSkillMin = pstPetDef->HideSkills[j];
			}

			if ((int)pstPetDef->HideSkills[j] > iSkillMax)
			{
				iSkillMax = pstPetDef->HideSkills[j];
			}
		}

		for (j=0; j<MAX_PET_BASIC_SKILL; j++)
		{
			if (pstPetDef->BasicSkills[j] == 0)
			{
				break;
			}

			if ((int)pstPetDef->BasicSkills[j] < iSkillMin)
			{
				iSkillMin = pstPetDef->BasicSkills[j];
			}

			if ((int)pstPetDef->BasicSkills[j] > iSkillMax)
			{
				iSkillMax = pstPetDef->BasicSkills[j];
			}
		}
	}

	if (iSkillMin > iSkillMax || iRaceMin > iRaceMax || iSubRaceMin > iSubRaceMax)
	{
		return 0;
	}

	p = calloc(1, (iSkillMax-iSkillMin+1)*(iRaceMax-iRaceMin+1)*(iSubRaceMax-iSubRaceMin+1));
	
	for (i=0; i<iCountPetSubRace; i++)
	{	
		pstPetSubRaceDef = (PETSUBRACEDEF *)(pAddrPetSubRace + i * iPetSubRaceUnit);

		for (j=0;j<MAX_PET_ADDED_SKILL;j++)
		{

			if (pstPetSubRaceDef->AddedSkills[j] == 0)
			{
				break;
			}
			
			*(p + (pstPetSubRaceDef->AddedSkills[j]-iSkillMin)*(iRaceMax-iRaceMin+1)*(iSubRaceMax-iSubRaceMin+1) +
				(pstPetSubRaceDef->Race-iRaceMin)*(iSubRaceMax-iSubRaceMin+1) + (pstPetSubRaceDef->SubRace-iSubRaceMin)) = 1;
		}
	}

	for (i=0; i<iCountPetDef; i++)
	{
		pstPetDef =  (PETDEF *)(pAddrPetDef + i * iPetDefUnit);
		for (j=0; j<MAX_PET_HIDE_SKILL; j++)
		{
			if (pstPetDef->HideSkills[j] == 0)
			{
				break;
			}
			
			*(p + (pstPetDef->HideSkills[j]-iSkillMin)*(iRaceMax-iRaceMin+1)*(iSubRaceMax-iSubRaceMin+1) +
				(pstPetDef->Race-iRaceMin)*(iSubRaceMax-iSubRaceMin+1) + (pstPetDef->SubRace-iSubRaceMin)) = 1;
			
		}

		for (j=0; j<MAX_PET_BASIC_SKILL; j++)
		{
			if (pstPetDef->BasicSkills[j] == 0)
			{
				break;
			}
			*(p + (pstPetDef->BasicSkills[j]-iSkillMin)*(iRaceMax-iRaceMin+1)*(iSubRaceMax-iSubRaceMin+1) +
				(pstPetDef->Race-iRaceMin)*(iSubRaceMax-iSubRaceMin+1) + (pstPetDef->SubRace-iSubRaceMin)) = 1;

		}
	}

	for (i=0; i<iSkillMax-iSkillMin+1; i++)
	{
		iCount = 0;
		for (j=0; j<iRaceMax-iRaceMin+1; j++)
		{
			for (k=0; k<iSubRaceMax-iSubRaceMin+1; k++)
			{
				iCount += *(p + i*(iRaceMax-iRaceMin+1)*(iSubRaceMax-iSubRaceMin+1)+j*(iSubRaceMax-iSubRaceMin+1)+k);
			}
		}

		if (iCount > 1)
		{
			printf("pet skill %d fail\n", i+iSkillMin);
			
			free(p);
			return -1;
		}
	}

	free(p);
	
	return 0;
}
*/
int check_item_rnd_hole(ITEMDEF *pstItemDef)
{
	// 第一个孔不需要判断
	int i;
	RNDHOLE *pstHole;
	int iRnd;

	for (i=1; i<MAX_ARM_HOLE; i++)
	{
		pstHole = &pstItemDef->Hole[i];
		iRnd = pstHole->RndBlue + pstHole->RndColor + pstHole->RndGreen + pstHole->RndRed;
		if (iRnd != 0 && iRnd != 100)
		{
			return -1;
		}
		
	}
	return 0;
}

int check_item(char *pAddrItem, int iCountItem, int iUnitItem, char *pAddrSuit, int iCountSuit, int iUnitSuit)
{
	int i;
	ITEMDEF *pstItemDef;
	SUITSET *pstSuitSet;

	for (i=0; i<iCountItem; i++)
	{
		pstItemDef = (ITEMDEF *)(pAddrItem + i * iUnitItem);
		if (pstItemDef->ItemType == ITEM_TYPE_ARM)
		{
			if (pstItemDef->MaxPile > 1)
			{
				printf("item sec err ItemType=%d  id:%d\n", pstItemDef->ItemType,pstItemDef->ItemID);
				return -1;
			}
			if(pstItemDef->ItemTmp.Type == 0) // 非临时物品
			{
				if (pstItemDef->WearPosNum == 0 || 3 < pstItemDef->WearPosNum || pstItemDef->WearPos1 == 0 || pstItemDef->WearPos1 > WEAR_POS_MAX || pstItemDef->WearPos2 > WEAR_POS_MAX || pstItemDef->WearPos3 > WEAR_POS_MAX)
				{
					printf("item %d wear pos err\n", pstItemDef->ItemID);
					return -1;
				}
			}
			

			if ((1 == pstItemDef->WearPosNum && 0 != pstItemDef->WearPos2) || 
					(2 == pstItemDef->WearPosNum && 0 != pstItemDef->WearPos3) ||
					(3 == pstItemDef->WearPosNum && 0 == pstItemDef->WearPos2))
			{
				printf("item %d wearpos num=%d\n", pstItemDef->ItemID, pstItemDef->WearPosNum);
				return -1;
			}
		}

		if (pstItemDef->SuitSetID > 0)
		{
			pstSuitSet = find_suitset(pAddrSuit, iCountSuit,iUnitSuit, pstItemDef->SuitSetID);
			if (NULL == pstSuitSet)
			{
				printf("item %d no suitid %d\n", pstItemDef->ItemID, pstItemDef->SuitSetID);
				return -1;
			}
		}

		//if (pstItemDef->NoDrop != pstItemDef->NoNpcSell)
		//{
		//	printf("item %d nodrop=%d nonpcsell=%d\n", pstItemDef->ItemID, pstItemDef->NoDrop, pstItemDef->NoNpcSell);
		//	return -1;
		//}

		//if (pstItemDef->MapItem && (0 == pstItemDef->NoDeal    || pstItemDef->MaxPile > 1))
		//{
		//	printf("item %d map item nodeal=%d maxpile=%d\n", pstItemDef->ItemID, pstItemDef->NoDeal, pstItemDef->MaxPile);
		//	return -1;
		//}

		if ( 0 != pstItemDef->ValidTime && pstItemDef->MaxPile > 1)
		{
			printf("item %d  item validtime =%d maxpile=%d\n", pstItemDef->ItemID, pstItemDef->ValidTime, pstItemDef->MaxPile);
			return -1;
		}

		if (check_item_rnd_hole(pstItemDef) < 0)
		{
			printf("item %d  rand hole error", pstItemDef->ItemID);
			return -1;
		}
	}

	return 0;
}

int check_skilldef(char *pAddr, int iCount, int iUint)
{
	int i;
	SKILLDEF *pstSkillDef;

	for (i=0; i<iCount; i++)
	{
		pstSkillDef = (SKILLDEF *)(pAddr + i*iUint);

		if (pstSkillDef->SkillID < 10000 && pstSkillDef->TargetNum > MAX_SKILL_TARGET)
		{
			printf("skill %d TargetNum=%d\n", pstSkillDef->SkillID, pstSkillDef->TargetNum);
			return -1;
		}

		/*
		if (pstSkillDef->FittingsGridRowNum > MAX_FITTINGS_ROW)
		{
			printf("skill %d FittingsGridRowNum=%d\n", pstSkillDef->SkillID, pstSkillDef->FittingsGridRowNum);
			return -1;
		}
		*/

		if (pstSkillDef->MainRange.RangeVal1 > 20000 || pstSkillDef->MainRange.RangeVal2 > 20000)
		{
			printf("skill %d RangeVal1=%d, RangeVal2=%d\n", pstSkillDef->SkillID, pstSkillDef->MainRange.RangeVal1,
					pstSkillDef->MainRange.RangeVal2);
			return -1;
		}
	}

	return 0;
}


int check_diathesis(char *pAddr, int iCount ,int iUint)
{
	int i;
	DIATHESISDEF *pstDiathesisDef;

	for (i=0; i<iCount; i++)
	{
		pstDiathesisDef = (DIATHESISDEF *)(pAddr + i*iUint);
		if (pstDiathesisDef->DiathesisID < 1 || pstDiathesisDef->Level < 1)
		{
			printf("DiathesisDef %d level %d error, please check database DiathesisDef tab", pstDiathesisDef->DiathesisID, pstDiathesisDef->Level);
			return -1;
		}
	}
	
	return 0;
}

int check_fittings(char *pAddr, int iCount, int iUint)
{
	int i;
	FITTINGSDEF *pstFittingsDef;

	for (i=0; i<iCount; i++)
	{
		pstFittingsDef = (FITTINGSDEF *)(pAddr + i*iUint);
		
		if (pstFittingsDef->FittingsID < 1 || pstFittingsDef->Level < 1 ||
			pstFittingsDef->FittingsResult[0].SkillResultID == 0 || 
			pstFittingsDef->Type >= MAX_FITTINGS_TYPE )
		{
			printf("FittingsDef %d level %d error, please check database FittingsDef tab\n", pstFittingsDef->FittingsID, pstFittingsDef->Level);
			return -1;
		}
	}
	
	return 0;
}

int check_patrol(char *pAddr, int iCount, int iUint)
{
	int i;
	PATROLDEF *pstPatrolDef;

	for (i=0; i<iCount; i++)
	{
		pstPatrolDef = (PATROLDEF *)(pAddr + i*iUint);

		if (0 == pstPatrolDef->PatrolPos[0].X || 0 == pstPatrolDef->PatrolPos[1].X)
		{
			printf ("patrol def %d error\n", pstPatrolDef->PatrolID);
			return -1;
		}
	}
	
	return 0;
}

int pre_item(char *pAddr, int iCount, int iUnit)
{
	int i;
	ITEMDEF *pstItemDef;

	for (i=0; i<iCount; i++)
	{
		pstItemDef = (ITEMDEF *)(pAddr + i*iUnit);
		if(0 == pstItemDef->MaxPile )
		{
			pstItemDef->MaxPile = 1;
		}

		/*if (pstItemDef->BuyPrice <= 0)
		{
			printf("item %d def err: BuyPrice is not zero\n", pstItemDef->ItemID);
			return -1;
		}*/

		if (0 > itemid_num(pstItemDef))
		{
			printf("item %d def err\n", pstItemDef->ItemID);
			return -1;
		}
	}

	return 0;
}

int itemid_num(ITEMDEF *pstItemDef)
{
	int iType;
	int iTypeNum;
	int iAmmType;
	char szItemID[16];

	memset(szItemID, 0, sizeof(szItemID));
	sprintf(szItemID, "%u", pstItemDef->ItemID);

	iAmmType = szItemID[3] - '0';

	//取前3 位
	szItemID[3] = 0;
	iTypeNum = atoi(szItemID);

	pstItemDef->NumType = iTypeNum;
	pstItemDef->AmmType = 0;

	if (pstItemDef->ItemID >= ITEM_NUM_ARM_MIN && pstItemDef->ItemID <= ITEM_NUM_ARM_MAX)
	{
		iType =  ITEM_TYPE_ARM;
	}
	else if (pstItemDef->ItemID >= ITEM_NUM_ITEM_MIN && pstItemDef->ItemID <= ITEM_NUM_ITEM_MAX)
	{
		if (pstItemDef->ItemID >= ITEM_NUM_AMM_MIN && pstItemDef->ItemID <= ITEM_NUM_AMM_MAX)
		{
			iType = ITEM_TYPE_AMM;
			pstItemDef->AmmType = iAmmType;
		}
		else if (pstItemDef->ItemID >= ITEM_NUM_STUFF_MIN && pstItemDef->ItemID <= ITEM_NUM_STUFF_MAX)
		{
			iType = ITEM_TYPE_STUFF;
		}
		else if (pstItemDef->ItemID >= ITEM_NUM_TASK_MIN && pstItemDef->ItemID <= ITEM_NUM_TASK_MAX)
		{
			iType = ITEM_TYPE_TASK;
		}
		else
		{
			iType = ITEM_TYPE_ITEM;
		}
	}
	else
	{
		iType = -1;
	}

	pstItemDef->ItemType = iType;

	return 0;
}

int IsPointInProcPoly(PROCPOLY *poly, int x, int y)
{
	int flag,flag1, i;
	int data;

	if(x<=poly->MinX || x>=poly->MaxX || y<=poly->MinY || y>=poly->MaxY) return 0;
	if(poly->Count < 3) return 0;

	flag1=-2;
	for(i=0; i<poly->Count-1; i++)
	{
		data = (poly->PointList[i].X-x) * (poly->PointList[i+1].Y-y) - (poly->PointList[i+1].X-x) * (poly->PointList[i].Y-y);
		flag = (data>0) ? 1 : ( (data < 0) ? -1 : 0 );
		if(flag)
		{
			if(flag1 == -2) flag1=flag;
			else if(flag != flag1) return 0;
		}
	}
	return 1;
}

int IsSameDay(time_t t1, time_t t2)
{
	struct tm curr1;
	struct tm curr2;

	//凌晨6点切换天
	t1 -= 6*3600;
	t2 -= 6*3600;
	
	localtime_r(&t1, &curr1);
	localtime_r(&t2, &curr2);

	if( curr1.tm_year==curr2.tm_year && curr1.tm_yday==curr2.tm_yday )
		return 1;

	return 0;
}

int IsSameWeek(time_t t1, time_t t2)
{
	int iNowWeekDay;
	int iStartWeekDay;
	struct tm stNow;
	struct tm stStart;
	time_t tNow;
	time_t tNowTmp, tStartTmp;
	time_t iStartTime = t1;

	tNow = t2;

	//凌晨6点切换天
	iStartTime -= 6*3600;
	tNow -= 6*3600;
	
	localtime_r(&tNow, &stNow);
	localtime_r(&iStartTime, &stStart);

	iStartWeekDay = stStart.tm_wday;
	if ( stStart.tm_wday == 0 )
	{
		iStartWeekDay = 7;
	}
	
	//一周的开始那一天
	tStartTmp = iStartTime - (iStartWeekDay-1)*24*60*60;

	iNowWeekDay = stNow.tm_wday;
	if ( stNow.tm_wday == 0 )
	{
		iNowWeekDay = 7;
	}
	
	//一周的开始那一天
	tNowTmp = tNow - (iNowWeekDay-1)*24*60*60;
	
	//同一天则同一个星期内
	tNowTmp += 6*3600;
	tStartTmp += 6*3600;
	
	if (IsSameDay(tNowTmp, tStartTmp))
	{		
		return 1;
	}

	return 0;
}


char * time2string(time_t tTime)
{
	static char szTime[25];
	memset(szTime,0,sizeof(szTime));
	strftime(szTime,sizeof(szTime),"%Y%m%d%H%M%S",localtime(&tTime));
	
	return szTime;
}
/* e.g  20100810120000 */
time_t string2time(const char * szTime)
{
	struct tm tmTime;
	time_t   tTime;

	sscanf(szTime,"%4d%2d%2d%2d%2d%2d",
		                  &tmTime.tm_year,
		                  &tmTime.tm_mon,
		                  &tmTime.tm_mday,
		                  &tmTime.tm_hour,
		                  &tmTime.tm_min,
		                  &tmTime.tm_sec);
	tmTime.tm_year -= 1900;
	tmTime.tm_mon --;
	tmTime.tm_isdst=-1;

	tTime = mktime(&tmTime);
	
	return tTime;
}

unsigned long long atoull(const char *p)
{
	int n = !p ? 0 : strlen(p);
	unsigned long long ret = 0ULL, exp = 1ULL;

	while(n--)
	{
		if(p[n] < '0' || p[n] > '9')
		{
			ret = 0ULL;
			return ret;
		}
		ret += (p[n] - '0') * exp;
		exp *= 10;
	}
	
	return ret;
}

char *str2hex(const void *pvBuff, int iSize)
{
	int i = 0; 
	int iBuff = 0;
	static char  szStr2HexBuff[102400];
	
	memset(szStr2HexBuff,0,sizeof(szStr2HexBuff));
	iBuff = sizeof(szStr2HexBuff);
	
	if (iSize > iBuff)
	{
		iSize = iBuff;
	}

	for (i=0; i<iSize; i++)
	{
		if((i+1)%2 == 0 )
		{
			snprintf(szStr2HexBuff + strlen(szStr2HexBuff),sizeof(szStr2HexBuff)-strlen(szStr2HexBuff), "%02X ", ((unsigned char*)pvBuff)[i]);
		}
		else
		{
			snprintf(szStr2HexBuff + strlen(szStr2HexBuff),sizeof(szStr2HexBuff)-strlen(szStr2HexBuff), "%02X", ((unsigned char*)pvBuff)[i]);
		}
	}

	if (iSize == 0)
	{
		szStr2HexBuff[0] = 0;
	}

	return szStr2HexBuff;
}

////////////////////////////////////////////////////////////////////////////////
// grav_slab.c
// #include "grav_slab.h"
#include <string.h>

#define SLAB_POOL_MAGIC   0x51ab9001U
#define SLAB_POOL_MIN_SIZE_SHIFT 3

#define SLAB_INIT_MEMORY  0xdeaddeadU
#define SLAB_FREED_MEMORY 0xfeedfeedU


#define SLAB_PAGE_MASK   3
#define SLAB_PAGE        0
#define SLAB_BIG         1
#define SLAB_EXACT       2
#define SLAB_SMALL       3

#if defined(_M_X64) || defined(__x86_64__)

#define SLAB_PAGE_FREE   0
#define SLAB_PAGE_BUSY   0xffffffffffffffff
#define SLAB_PAGE_START  0x8000000000000000

#define SLAB_SHIFT_MASK  0x000000000000000f
#define SLAB_MAP_MASK    0xffffffff00000000
#define SLAB_MAP_SHIFT   32

#define SLAB_BUSY        0xffffffffffffffff

#else // x86_32

#define SLAB_PAGE_FREE   0
#define SLAB_PAGE_BUSY   0xffffffff
#define SLAB_PAGE_START  0x80000000

#define SLAB_SHIFT_MASK  0x0000000f
#define SLAB_MAP_MASK    0xffff0000
#define SLAB_MAP_SHIFT   16

#define SLAB_BUSY        0xffffffff

#endif

#define slab_ptr(pool, off) ((void *)(((char *)(pool)) + (uintptr_t)(off)))

#define slab_off(pool, ptr) ((uintptr_t)((char*)(ptr) - (char*)(pool)))

#define slab_obj(pool, off, type) \
	((type *)(((char *)(pool)) + (uintptr_t)(off)))


static void slab_init_memory(char *raw, size_t n)
{
	uint32_t *p = (uint32_t*)raw;
	n /= sizeof(*p);
	while(n--)
		*p++ = SLAB_INIT_MEMORY;
}

struct grav_slab_pool* grav_slab_pool_init(char *raw, size_t len)
{
	struct grav_slab_pool *pool = NULL;
	struct grav_slab_page *slots = NULL;
	size_t i, m, n, size, n_pages;
	char *p = NULL;
	
	if(!raw)
		return NULL;
	
	if((uintptr_t)raw % 4)
		return NULL;
	
	if(len < sizeof(struct grav_slab_pool) + (1 << (GRAV_PAGE_SIZE_SHIFT)))
		return NULL;
	
	pool = (struct grav_slab_pool*)raw;
	memset(pool, 0, sizeof(*pool));

	pool->magic = SLAB_POOL_MAGIC;
	pool->page_shift = GRAV_PAGE_SIZE_SHIFT;
	pool->page_size  = 1 << GRAV_PAGE_SIZE_SHIFT;
	
	pool->min_shift = SLAB_POOL_MIN_SIZE_SHIFT;
	pool->min_size  = 1 << SLAB_POOL_MIN_SIZE_SHIFT;
	
	pool->slab_max_size = pool->page_size / 2;
	
	pool->slab_exact_size = pool->page_size / (8 * sizeof(uintptr_t));
	for(i = pool->slab_exact_size; i >>= 1; ++pool->slab_exact_shift) {}
	
	pool->end = raw + len;
	p = slab_ptr(pool, sizeof(*pool));
	size = pool->end - p;
	slab_init_memory(p, size);

	slots = (struct grav_slab_page *)p;
	n = pool->page_shift - pool->min_shift;
	
	for(i = 0; i < n; ++i)
	{
		slots[i].slab = 0;
		slots[i].next = slab_off(pool, slots + i);
		slots[i].prev = 0;
	}

	p += n * sizeof(struct grav_slab_page);
	
	n_pages = (size_t)(size / (pool->page_size + sizeof(struct grav_slab_page)));
	
	memset(p, 0, n_pages * sizeof(struct grav_slab_page));
	
	pool->pages = (struct grav_slab_page *)p;
	
	pool->free.prev = 0;
	pool->free.next = slab_off(pool, p);
	
	pool->pages->slab = n_pages;
	pool->pages->next = slab_off(pool, &pool->free);
	pool->pages->prev = slab_off(pool, &pool->free);
	
	pool->start = grav_align_ptr(
		(uintptr_t)p + n_pages * sizeof(struct grav_slab_page),
		pool->page_size);
	
	m = n_pages - (pool->end - pool->start) / pool->page_size;
	if(m > 0)
	{
		n_pages -= m;
		pool->pages->slab = n_pages;
	}
	return pool;
}

struct grav_slab_pool* grav_slab_pool_attach(char *raw, size_t len)
{
	struct grav_slab_pool *pool = NULL;
	size_t n, size, n_pages;
	char *p = NULL;
	
	if(!raw)
		return NULL;
	
	pool = (struct grav_slab_pool*)raw;
	if(pool->magic != SLAB_POOL_MAGIC)
		return NULL;

	pool->end = raw + len;
	p = slab_ptr(pool, sizeof(*pool));
	size = pool->end - p;
	
	n = pool->page_shift - pool->min_shift;
	p += n * sizeof(struct grav_slab_page);
	n_pages = (size_t)(size / (pool->page_size + sizeof(struct grav_slab_page)));
	pool->pages = (struct grav_slab_page *)p;
	pool->start = grav_align_ptr(
		(uintptr_t)p + n_pages * sizeof(struct grav_slab_page),
		pool->page_size);
	
	return pool;
}

static struct grav_slab_page* slab_alloc_pages(struct grav_slab_pool *pool,
                                               size_t n_pages)
{
	struct grav_slab_page *page = NULL, *p = NULL;
	
	for(page = slab_ptr(pool, pool->free.next);
	    page != &pool->free;
	    page = slab_ptr(pool, page->next))
	{
		if(page->slab < n_pages)
			continue;
		
		if(page->slab > n_pages)
		{
			page[n_pages].slab = page->slab - n_pages;
			page[n_pages].next = page->next;
			page[n_pages].prev = page->prev;
			
			p = slab_ptr(pool, page->prev);
			p->next = slab_off(pool, page + n_pages);
			slab_obj(pool, page->next, struct grav_slab_page)->prev = p->next;
		}
		else // page->slab == n_pages
		{
			p = slab_ptr(pool, page->prev);
			p->next = page->next;
			slab_obj(pool, page->next,
			         struct grav_slab_page)->prev = page->prev;
		}
		
		page->slab = n_pages | SLAB_PAGE_START;
		page->next = 0;
		page->prev = SLAB_PAGE;
		
		if(--n_pages == 0)
			return page;
		
		for(p = page + 1; n_pages; --n_pages)
		{
			p->slab = SLAB_PAGE_BUSY;
			p->next = 0;
			p->prev = SLAB_PAGE;
			++p;
		}
		return page;
	}
	return NULL;
}

static void slab_free_pages(struct grav_slab_pool *pool,
							struct grav_slab_page *page,
							size_t n_pages)
{
	struct grav_slab_page *prev = NULL;

	page->slab = n_pages--;

	if(n_pages)
		memset(page + 1, 0, n_pages * sizeof(struct grav_slab_page));

	if(page->next)
	{
		prev = slab_ptr(pool, page->prev & ~SLAB_PAGE_MASK);
		prev->next = page->next;
		slab_obj(pool, page->next, struct grav_slab_page)->prev = page->prev;
	}

	page->prev = slab_off(pool, &pool->free);
	page->next = pool->free.next;

	slab_obj(pool, page->next, struct grav_slab_page)->prev
		= slab_off(pool, page);

	pool->free.next = slab_off(pool, page);
}

void* grav_slab_alloc_locked(struct grav_slab_pool *pool, size_t size)
{
	size_t s;
	uintptr_t p, n, m, mask, *bitmap;
	uintptr_t i, slot, shift, map;
	struct grav_slab_page *page, *prev, *slots;
	
	if(size >= pool->slab_max_size)
	{
		page = slab_alloc_pages(pool,
			(size + pool->page_size - 1) >> pool->page_shift);
		if(page)
		{
			p = (page - pool->pages) << pool->page_shift;
			p += (uintptr_t)pool->start;
		}
		else
			p = 0;
		
		goto final;
	}
	
	if(size > pool->min_size)
	{
		shift = 1;
		for(s = size - 1; s >>= 1; ++shift) {}
		slot = shift - pool->min_shift;
	}
	else
	{
		size = pool->min_size;
		shift = pool->min_shift;
		slot = 0;
	}
	
	slots = slab_ptr(pool, sizeof(struct grav_slab_pool));
	page = slab_ptr(pool, slots[slot].next);
	
	if(slab_ptr(pool, page->next) == page)
		goto first_alloc;
	
	if(shift < pool->slab_exact_shift)
	{
		do
		{
			p = (page - pool->pages) << pool->page_shift;
			bitmap = (uintptr_t *)(pool->start + p);
			
			map = (1 << (pool->page_shift - shift)) /
			      (sizeof(uintptr_t) * 8);
			
			for(n = 0; n < map; ++n)
			{
				if(bitmap[n] != SLAB_BUSY)
				{
					for(m = 1, i = 0; m; m <<= 1, ++i)
					{
						if(bitmap[n] & m)
							continue;
						
						bitmap[n] |= m;
						
						i = ((n * sizeof(uintptr_t) * 8) << shift) +
						    (i << shift);
						
						if(bitmap[n] == SLAB_BUSY)
						{
							for(n = n + 1; n < map; ++n)
							{
								if(bitmap[n] != SLAB_BUSY)
								{
									p = (uintptr_t)bitmap + i;
									goto final;
								}
							}
							
							prev = slab_ptr(pool,
							                page->prev & ~SLAB_PAGE_MASK);
							prev->next = page->next;
							slab_obj(pool, page->next,
							         struct grav_slab_page)->prev = page->prev;
							
							page->next = 0;
							page->prev = SLAB_SMALL;
						}
						
						p = (uintptr_t)bitmap + i;
						goto final;
					}
				}
			}
			
			page = slab_ptr(pool, page->next);
			
		}while(page);
		
	}
	else if(shift == pool->slab_exact_shift)
	{
		do
		{
			if(page->slab != SLAB_BUSY)
			{
				for(m = 1, i = 0; m; m <<= 1, ++i)
				{
					if(page->slab & m)
						continue;
					
					page->slab |= m;
					
					if(page->slab == SLAB_BUSY)
					{
						prev = slab_ptr(pool, page->prev & ~SLAB_PAGE_MASK);
						prev->next = page->next;
						slab_obj(pool, page->next,
						         struct grav_slab_page)->prev = page->prev;
						
						page->next = 0;
						page->prev = SLAB_EXACT;
					}
					
					p = (page - pool->pages) << pool->page_shift;
					p += i << shift;
					p += (uintptr_t)pool->start;
					
					goto final;
				}
			}
			
			page = slab_ptr(pool, page->next);
			
		}while(page);
	}
	else // (shift > pool->slab_exact_shift)
	{
		n = pool->page_shift - (page->slab & SLAB_SHIFT_MASK);
		n = 1 << n;
		n = ((uintptr_t)1 << n) - 1;
		mask = n << SLAB_MAP_SHIFT;
		
		do
		{
			if((page->slab & SLAB_MAP_MASK) != mask)
			{
				for(m = (uintptr_t)1 << SLAB_MAP_SHIFT, i = 0;
					m & mask;
					m <<= 1, ++i)
				{
					if(page->slab & m)
						continue;
					
					page->slab |= m;
					
					if((page->slab & SLAB_MAP_MASK) == mask)
					{
						prev = slab_ptr(pool, page->prev & ~SLAB_PAGE_MASK);
						prev->next = page->next;
						slab_obj(pool, page->next,
						         struct grav_slab_page)->prev = page->prev;
						
						page->next = 0;
						page->prev = SLAB_BIG;
					}
					
					p = (page - pool->pages) << pool->page_shift;
					p += i << shift;
					p += (uintptr_t)pool->start;
					
					goto final;
				}
			}
			
			page = slab_ptr(pool, page->next);
			
		}while(page);
	}
	
first_alloc:
	page = slab_alloc_pages(pool, 1);
	
	if(!page)
		return NULL;
	
	if(shift < pool->slab_exact_shift)
	{
		p = (page - pool->pages) << pool->page_shift;
		bitmap = (uintptr_t *)(pool->start + p);
		
		s = 1 << shift;
		n = (1 << (pool->page_shift - shift)) / 8 / s;
		
		if(!n)
			n = 1;
		
		bitmap[0] = (2 << n) - 1;
		
		map = (1 << (pool->page_shift - shift)) / (sizeof(uintptr_t) * 8);
		
		for(i = 1; i < map; ++i)
			bitmap[i] = 0;
		
		page->slab = shift;
		page->next = slab_off(pool, slots + slot);
		page->prev = slab_off(pool, slots + slot) | SLAB_SMALL;
		
		slots[slot].next = slab_off(pool, page);
		
		p = ((page - pool->pages) << pool->page_shift) + s * n;
		p += (uintptr_t)pool->start;
		
		goto final;
	}
	else if(shift == pool->slab_exact_shift)
	{
		page->slab = 1;
		page->next = slab_off(pool, slots + slot);
		page->prev = slab_off(pool, slots + slot) | SLAB_EXACT;
		
		slots[slot].next = slab_off(pool, page);
		
		p = ((page - pool->pages) << pool->page_shift);
		p += (uintptr_t)pool->start;
		
		goto final;
	}
	else // shift > pool->slab_exact_shift
	{
		page->slab = ((uintptr_t)1 << SLAB_MAP_SHIFT) | shift;
		page->next = slab_off(pool, slots + slot);
		page->prev = slab_off(pool, slots + slot) | SLAB_BIG;
		
		slots[slot].next = slab_off(pool, page);
		
		p = ((page - pool->pages) << pool->page_shift);
		p += (uintptr_t)pool->start;
		
		goto final;
	}
	
	p = 0;
final:
	return (void *)p;
}

void grav_slab_free_locked(struct grav_slab_pool *pool, void *p)
{
	size_t size;
	uintptr_t slab, m, *bitmap;
	uintptr_t n, type, slot, shift, map;
	struct grav_slab_page *slots, *page;

	if((char *)p < pool->start || (char *)p > pool->end)
		goto fail;

	n = ((char *)p - pool->start) >> pool->page_shift;
	page = pool->pages + n;
	slab = page->slab;
	type = page->prev & SLAB_PAGE_MASK;

	switch(type)
	{
	case SLAB_SMALL:
		shift = slab & SLAB_SHIFT_MASK;
		size = 1 << shift;

		if((uintptr_t)p & (size - 1))
			goto wrong_chunk;

		n = ((uintptr_t)p & (pool->page_size - 1)) >> shift;
		m = (uintptr_t)1 << (n & (sizeof(uintptr_t) * 8 - 1));
		n /= (sizeof(uintptr_t) * 8);
		bitmap = (uintptr_t *)((uintptr_t)p & ~(pool->page_size - 1));
		
		if(bitmap[n] & m)
		{
			if(page->next == 0)
			{
				slots = slab_ptr(pool, sizeof(struct grav_slab_pool));
				slot = shift - pool->min_shift;

				page->next = slots[slot].next;
				slots[slot].next = slab_off(pool, page);

				page->prev = slab_off(pool, slots + slot) | SLAB_SMALL;
				slab_obj(pool, page->next,
				         struct grav_slab_page)->prev
					= slab_off(pool, page) | SLAB_SMALL;
			}

			bitmap[n] &= ~m;

			n = (1 << (pool->page_shift - shift)) / 8 / (1 << shift);

			if(!n)
				n = 1;

			if(bitmap[0] & ~(((uintptr_t)1 << n) - 1))
				goto final;

			map = (1 << (pool->page_shift - shift)) / 
				  (sizeof(uintptr_t) * 8);

			for(n = 1; n < map; ++n)
				if(bitmap[n])
					goto final;

			slab_free_pages(pool, page, 1);
			goto final;
		}
		goto chunk_already_free;
		
	case SLAB_EXACT:
		m = (uintptr_t)1 << (((uintptr_t)p & (pool->page_size - 1))
			 >> pool->slab_exact_shift);
		size = pool->slab_exact_size;

		if((uintptr_t)p & (size - 1))
			goto wrong_chunk;

		if(slab & m)
		{
			if(slab == SLAB_BUSY)
			{
				slots = slab_ptr(pool, sizeof(struct grav_slab_pool));
				slot = pool->slab_exact_size - pool->min_shift;

				page->next = slots[slot].next;
				slots[slot].next = slab_off(pool, page);

				page->prev = slab_off(pool, slots + slot) | SLAB_EXACT;
				slab_obj(pool, page->next,
				         struct grav_slab_page)->prev
					= slab_off(pool, page) | SLAB_EXACT;
			}

			page->slab &= ~m;

			if(page->slab)
				goto final;

			slab_free_pages(pool, page, 1);

			goto final;
		}
		goto chunk_already_free;
		
	case SLAB_BIG:
		shift = slab & SLAB_SHIFT_MASK;
		size = 1 << shift;

		if((uintptr_t)p & (size - 1))
			goto wrong_chunk;

		m = (uintptr_t)1 << ((((uintptr_t)p & (pool->page_size - 1))
			>> shift) + SLAB_MAP_SHIFT);

		if(slab & m)
		{
			if(page->next == 0)
			{
				slots = slab_ptr(pool, sizeof(struct grav_slab_pool));
				slot = shift - pool->min_shift;

				page->next = slots[slot].next;
				slots[slot].next = slab_off(pool, page);

				page->prev = slab_off(pool, slots + slot) | SLAB_BIG;
				slab_obj(pool, page->next,
				         struct grav_slab_page)->prev
					= slab_off(pool, page) | SLAB_BIG;
			}

			page->slab &= ~m;

			if(page->slab & SLAB_MAP_MASK)
				goto final;

			slab_free_pages(pool, page, 1);

			goto final;
		}
		goto chunk_already_free;
		
	case SLAB_PAGE:
		if((uintptr_t)p & (pool->page_size - 1))
			goto wrong_chunk;
		
		if(slab == SLAB_PAGE_FREE)
			goto fail;

		if(slab == SLAB_PAGE_BUSY)
			goto fail;

		n = ((char *)p - pool->start) >> pool->page_shift;
		size = slab &= ~SLAB_PAGE_START;

		slab_free_pages(pool, pool->pages + n, size);
		slab_init_memory(p, size << pool->page_shift);
		return;
	}
	return; // NO reachec!

final:
	slab_init_memory(p, size);
	return;

wrong_chunk:
	goto fail;

chunk_already_free:
	goto fail;
	
fail:
	return;
}


/*(sp-op)*(ep-op)的叉积
r=multiply(sp,ep,op),得到(sp-op)*(ep-op)的叉积
r>0:sp在矢量op ep的顺时针方向；
r=0：op sp ep三点共线；
r<0: sp在矢量op ep的逆时针方向 */
double multiply(DPOINT sp,DPOINT ep,DPOINT op)
{	
	return((sp.X-op.X)*(ep.Y-op.Y) - (ep.X-op.X)*(sp.Y-op.Y));
}

//判断点q在凸多边形polygon内
// 点q是凸多边形polygon内[包括边上]时，返回true
// 注意：多边形polygon一定要是凸多边形
int InsideConvexPolygon(int vcount, RESPOS polygon[], RESPOS q1)
{
	DPOINT p;
	DPOINT q;
	LINESEG l;
	int i;
	
	q.X= q1.X;
	q.Y= q1.Y;
	
	p.X=0; p.Y=0;
	for(i=0;i<vcount;i++) // 寻找一个肯定在多边形polygon内的点p：多边形顶点平均值
	{
		p.X+=polygon[i].X;
		p.Y+=polygon[i].Y;
	}
	
	p.X /= vcount;
	p.Y /= vcount;
	for(i=0;i<vcount;i++)
	{
		l.s.X=polygon[i].X;
		l.s.Y=polygon[i].Y;
		l.e.X=polygon[(i+1)%vcount].X;
		l.e.Y=polygon[(i+1)%vcount].Y;
		if(multiply(p,l.e,l.s)*multiply(q,l.e,l.s)<0)
		{
			/* 点p和点q在边l的两侧，说明点q肯定在多边形外 */
			return 0;
		}
	}
	return 1;
}

/* 判断点p是否在线段l上
条件：(p在线段l所在的直线上)&& (点p在以线段l为对角线的矩形内) */
int online(LINESEG l, RESPOS p1)
{
	DPOINT p;
	p.X = p1.X;
	p.Y = p1.Y;
	
	return ((multiply(p,l.e,l.s)==0)
		&& ( ( (p.X-l.s.X) * (p.X-l.e.X) <=0 ) && ( (p.Y-l.s.Y)*(p.Y-l.e.Y) <=0 ) ) );
}

// 根据已知两点坐标，求过这两点的直线解析方程： a*x+b*y+c = 0 (a >= 0)
LINE makeline(DPOINT p1, DPOINT p2)
{
	LINE tl;
	int sign=1;
	tl.a=p2.Y-p1.Y;
	if(tl.a<0)
	{
		sign=-1;
		tl.a=sign*tl.a;
	}
	tl.b=sign*(p1.X-p2.X);
	tl.c=sign*(p1.Y*p2.X-p1.X*p2.Y);
	return tl;
}

// 如果两条直线 l1(a1*x+b1*y+c1 = 0), l2(a2*x+b2*y+c2 = 0)相交，返回true，且返回交点p
int lineintersect(LINE l1,LINE l2,RESPOS *p) // 是 L1，L2
{
	double d=l1.a*l2.b-l2.a*l1.b;
	if(abs(d)<EP) // 不相交
	{
		return 0;
	}
	p->X = (l2.c*l1.b-l1.c*l2.b)/d;
	p->Y = (l2.a*l1.c-l1.a*l2.c)/d;
	
	return 1;
}
// 如果线段l1和l2相交，返回true且交点由(inter)返回，否则返回false
int intersection(LINESEG l1,LINESEG l2,RESPOS *inter)
{
	LINE ll1,ll2;
	ll1=makeline(l1.s,l1.e);
	ll2=makeline(l2.s,l2.e);
	if(lineintersect(ll1,ll2,inter)) 
	{
		//return online(l1,*inter);
		if(inter->X<=0 || inter->X <=0)
		{
			return 0;
		}
		return ( ((inter->X-l1.s.X)*(inter->X-l1.e.X)<=0) && ((inter->Y-l1.s.Y)*(inter->Y-l1.e.Y)<=0) &&
				 ((inter->X-l2.s.X)*(inter->X-l2.e.X)<=0) && ((inter->Y-l2.s.Y)*(inter->Y-l2.e.Y)<=0) );
	}
	else 
	{
		return 0;
	}
	return 0;
}

// 返回两点之间欧氏距离
double dist(DPOINT p1,DPOINT p2)
{
	return( sqrt( (p1.X-p2.X)*(p1.X-p2.X)+(p1.Y-p2.Y)*(p1.Y-p2.Y) ) );
}
/*矢量(p1-op)和(p2-op)的点积
r=dotmultiply(p1,p2,op),得到矢量(p1-op)和(p2-op)的点积如果两个矢量都非零矢量
r < 0: 两矢量夹角为锐角；
r = 0：两矢量夹角为直角；
r > 0: 两矢量夹角为钝角 */
double dotmultiply(DPOINT p1,DPOINT p2,DPOINT p0)
{
	return ((p1.X-p0.X)*(p2.X-p0.X) + (p1.Y-p0.Y)*(p2.Y-p0.Y));
}
/* 判断点C在线段AB所在的直线l上垂足P的与线段AB的关系
本函数是根据下面的公式写的，P是点C到线段AB所在直线的垂足
AC dot AB
r = ----------------------
||AB||^2
(Cx-Ax)(Bx-Ax) + (Cy-Ay)(By-Ay)
= ----------------------------------------------------
L^2
r has the following meaning:
r=0 P = A
r=1 P = B
r<0 P is on the backward extension of AB
r>1 P is on the forward extension of AB
0<r<1 P is interior to AB
*/
double relation(DPOINT c,LINESEG l)
{
	LINESEG tl;
	tl.s=l.s;
	tl.e=c;
	return dotmultiply(tl.e,l.e,l.s)/(dist(l.s,l.e)*dist(l.s,l.e));
}
// 求点C到线段AB所在直线的垂足 P
DPOINT perpendicular(DPOINT p,LINESEG l)
{
	double r=relation(p,l);
	DPOINT tp;
	tp.X=l.s.X+r*(l.e.X-l.s.X);
	tp.Y=l.s.Y+r*(l.e.Y-l.s.Y);
	return tp;
}
/* 求点p到线段l的最短距离
返回线段上距该点最近的点np 注意：np是线段l上到点p最近的点，不一定是垂足 */
double ptolinesegdist(DPOINT p,LINESEG l,DPOINT *np)
{
	double r=relation(p,l);
	if(r<0)
	{
		*np=l.s;
		return dist(p,l.s);
	}
	if(r>1)
	{
		*np=l.e;
		return dist(p,l.e);
	}
	*np=perpendicular(p,l);
	return dist(p,*np);
}
//点p3到线段p1,p2的最短距离
double ptolinesegdist_ex(RESPOS p1,RESPOS p2,RESPOS p3)
{
	DPOINT p;
	LINESEG l;
	DPOINT np;
	double dLen;

	p.X = p3.X;
	p.Y = p3.Y;

	l.s.X = p1.X;
	l.s.Y = p1.Y;
	l.e.X = p2.X;
	l.e.Y = p2.Y;

	dLen = ptolinesegdist(p,l,&np);
	
	return dLen;
}

int IsTransPkgCmd(int iCmd)
{
	if (BULLETIN_SVR == iCmd ||
		SYS_MSG == iCmd || 
		CHAT_RES == iCmd || 
		ONLINE_INFO == iCmd || 
		RELATION_CLT == iCmd || 
		RELATION_SVR == iCmd ||
		CLAN_SVR == iCmd ||
		CHAT_REQ == iCmd)
	{
		return 1;
	}
	return 0;
}

int region_world_func_inst_aton_busid(int iRegion, int iWorldID, int iFuncID, int iInst)
{
	int iBusID = 0;
	char szAddr[64];
	
	sprintf(szAddr, "%d.%d.%d.%d", iRegion, iWorldID, iFuncID, iInst);
	tbus_addr_aton(szAddr, (TBUSADDR *)&iBusID);
	return iBusID;
}

int str_to_passwd_card(char* pszStr,int iXLen,int iYLen,PASSWDCARD *pstCard)
{
	int iRet = 0;
	char *p = NULL;
	PASSWDCARDCELL* pstCell = NULL;
	PASSWDCARDROW* pstRow = NULL;
	
	if (iXLen < 0 || iYLen < 0 ||
		iXLen > PASSWD_CARD_MAX_COLUMN ||
		iYLen > PASSWD_CARD_MAX_ROW)
	{
		return -1;
	}

	memset(pstCard,0,sizeof(*pstCard));
		
	p = pszStr;
	
	pstCard->RowNum = 0;
	pstRow = &pstCard->Row[pstCard->RowNum];
	pstCard->RowNum++;
	
	pstRow->ColumnNum = 0;		
	pstCell = &pstRow->Column[pstRow->ColumnNum];	
	pstRow->ColumnNum++;
	
	pstCell->CellLen = 0;

	while(*p)
	{			
		if (*p == ',')
		{
			
			if (pstRow->ColumnNum >= iXLen)
			{
				if (pstCard->RowNum < PASSWD_CARD_MAX_ROW)
				{
					
					pstRow = &pstCard->Row[pstCard->RowNum];
					pstCard->RowNum++;

					pstRow->ColumnNum = 0;		
					pstCell = &pstRow->Column[pstRow->ColumnNum];
					pstRow->ColumnNum++;
					
					pstCell->CellLen = 0;
				}
				else
				{
					iRet = -2;
					break;
				}
			}
			else
			{
				pstCell = &pstRow->Column[pstRow->ColumnNum];
				pstRow->ColumnNum++;
				pstCell->CellLen = 0;
			}
		}
		else
		{
			if (pstCell->CellLen < PASSWD_CARD_MAX_CELL_LEN - 1)
			{
				pstCell->Cell[pstCell->CellLen] = *p;
				pstCell->CellLen++;
			}
			else
			{
				iRet = -3;
				break;
			}
		}			
		
		p++;
	}

	
	if(iRet != 0)
	{
		memset(pstCard,0,sizeof(*pstCard));
	}
	
	return iRet;
}

