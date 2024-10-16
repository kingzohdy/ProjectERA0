#include "mysql/mysql.h"
#include <stdio.h>
#include <time.h>
 #include <string.h>
 #include <stdlib.h>
 #include "time.h"


#define MAX_CACL_DATA 65535

struct CaclData
{
	char cLogin; 	// 1-登录 0-退出
	time_t t;		// 时间
};
typedef struct CaclData CACLDATA;

struct CaclEnv
{
	int iNum;
	CACLDATA stData[MAX_CACL_DATA]; // 一共500k
};

typedef struct CaclEnv CACLENV;
CACLENV stEnv;


// 2012-07-21 12:12:30->sec
time_t caclonline_strtime1(char *szTime)
{
	time_t tTime;
	struct tm stTm;
	
	if (NULL == strptime(szTime, "%Y-%m-%d %H:%M:%S", &stTm))
	{
		return -1;
	}

	tTime = mktime(&stTm);
	return tTime;
}

// 120721->sec
time_t caclonline_strtime2(char *szTime)
{
	char szBuff[64]={0};
	struct tm tmTime; 

	szBuff[0] = '2';
	szBuff[1] = '0';
	strcat(szBuff,szTime);
	strcat(szBuff,"000000");

	sscanf(szBuff,"%4d%2d%2d%2d%2d%2d",
		                  &tmTime.tm_year,
		                  &tmTime.tm_mon,
		                  &tmTime.tm_mday,
		                  &tmTime.tm_hour,
		                  &tmTime.tm_min,
		                  &tmTime.tm_sec);
	tmTime.tm_year -= 1900;
	tmTime.tm_mon --;
	tmTime.tm_isdst=-1;
		
	return mktime(&tmTime);
}


// 2012-07-21 12:12:30->120812
int caclonline_strtime3(const char *szTime)
{
	int iTime;
	struct tm stTm;
	
	if (NULL == strptime(szTime, "%Y-%m-%d %H:%M:%S", &stTm))
	{
		return -1;
	}

	iTime = (stTm.tm_year+1900)%100*10000 + (stTm.tm_mon+1)*100 +stTm.tm_mday;
	return iTime;
}

void caclonline_make_sql(char *szIn, int iSizeIn,int iStart, const char *pszStart, const char *pszEnd, 
							const char* pszTable,unsigned long long ullRoleID)
{
	// 90815 年是小于10的
	if (iStart < 100000)
	{
		snprintf(szIn, iSizeIn, 
			"select LogTime from %s_0%d where RoleID=%llu && LogTime>='%s' &&LogTime<='%s'", 
			pszTable,iStart, ullRoleID,pszStart,pszEnd);
	}
	else
	{
		snprintf(szIn, iSizeIn, 
			"select LogTime from %s_%d where RoleID=%llu && LogTime>='%s' &&LogTime<='%s'", 
			pszTable,iStart, ullRoleID,pszStart,pszEnd);
	}
}

int cacl_up_start_time(int *piStart, char *pszStart,int iLen)
{
	time_t iStart = caclonline_strtime1(pszStart);
	
	iStart += 24*60*60;
	memset(pszStart,0, iLen);
	strftime(pszStart,iLen,"%Y-%m-%d %H:%M:%S",localtime(&iStart));

	*piStart = caclonline_strtime3(pszStart);
	return 0;

}

int caclonline_get_table(CACLENV *pstEnv, const char *pszStart, const char *pszEnd, MYSQL *conn,unsigned long long ullRoleID)
{
	int iStart = caclonline_strtime3(pszStart);
	int iEnd = caclonline_strtime3(pszEnd);
	int iErr;
	int iYs,iMs,iDs;
	char szSql[256];
	MYSQL_RES *res;
	MYSQL_ROW row;
	time_t t;
	char szSTime[64];

	if (iEnd <iStart )
		return -1;

	strncpy(szSTime, pszStart, sizeof(szSTime));
	while(iStart <= iEnd)
	{
		caclonline_make_sql(szSql, sizeof(szSql),iStart, pszStart,pszEnd, "LogRoleLogin",ullRoleID);
		//snprintf(szSql, sizeof(szSql), "select LogTime from LogRoleLogin_%d where RoleID=%llu", iStart, ullRoleID);
		iErr = mysql_query(conn, szSql);
		if(!iErr)
		{
			res = mysql_store_result(conn);
			if(res)
			{
				while((row = mysql_fetch_row(res)) != NULL)
				{
					if(!row[0])
						continue;

					if (pstEnv->iNum >= MAX_CACL_DATA)
					{
						return -1;
					}
					t = caclonline_strtime1(row[0]);
					if (t > 0)
					{
						pstEnv->stData[pstEnv->iNum].t = t;
						pstEnv->stData[pstEnv->iNum].cLogin = 1;
						pstEnv->iNum ++;
					}
				}
			}
		}

		caclonline_make_sql(szSql, sizeof(szSql),iStart, pszStart,pszEnd, "LogRoleLogout",ullRoleID);
		//snprintf(szSql, sizeof(szSql), "select LogTime from LogRoleLogout_%d where RoleID=%llu", iStart, ullRoleID);
		iErr = mysql_query(conn, szSql);
		if(!iErr)
		{
			res = mysql_store_result(conn);
			if(res)
			{
				while((row = mysql_fetch_row(res)) != NULL)
				{
					if(!row[0])
						continue;

					if (pstEnv->iNum >= MAX_CACL_DATA)
					{
						return -1;
					}
					t = caclonline_strtime1(row[0]);
					if (t > 0)
					{
						pstEnv->stData[pstEnv->iNum].t = t;
						pstEnv->stData[pstEnv->iNum].cLogin = 0;
						pstEnv->iNum ++;
					}
				}
			}
		}

		cacl_up_start_time(&iStart, szSTime, sizeof(szSTime));

		/*iStart ++;
		iYs = iStart/10000;
		iMs = iStart/100%100;
		iDs = iStart%100;
		if (iDs > 31)
		{
			iDs = 1;
			iMs ++;
		}

		if (iMs > 12)
		{
			iMs = 1;
			iYs ++;
		}
		
		iStart = 	iYs*10000 +iMs *100 +iDs;*/
	}
	return 0;	
}

static unsigned long long my_atoull(const char *p)
{
	int n = !p ? 0 : strlen(p);
	unsigned long long ret = 0ULL, exp = 1ULL;

	while(n--)
	{
		if(p[n] < '0' || p[n] > '9')
		{
			ret = 0ULL;
			goto out;
		}
		ret += (p[n] - '0') * exp;
		exp *= 10;
	}
	
out:
	return ret;
}

static int caclonline_cmp(const void *p1, const void *p2)
{
	CACLDATA *pstData1;
	CACLDATA *pstData2;

	pstData1  = (CACLDATA *)p1;
	pstData2  = (CACLDATA *)p2;
	
	return pstData1->t - pstData2->t;
}


time_t caclonline_in(CACLENV *pstEnv,time_t tStart, time_t tEnd)
{
	time_t tOnline = 0;
	int i;
	char cPreFlag = 0x7F;

	for (i=0; i<pstEnv->iNum; i++)
	{
		if (i==0)
		{
			if (pstEnv->stData[i].cLogin == 0)
			{
				tOnline += pstEnv->stData[i].t - tStart;
				continue;
			}
		}	

		if (i==pstEnv->iNum-1)
		{
			if (pstEnv->stData[i].cLogin == 1)
			{
				tOnline += tEnd - pstEnv->stData[i].t;
			}
		}

		// 前一个是登陆, 后一个是退出才计算时间
		if (cPreFlag == 1 && pstEnv->stData[i].cLogin == 0)
		{
			tOnline += pstEnv->stData[i].t - pstEnv->stData[i-1].t;
		}
		
		cPreFlag = pstEnv->stData[i].cLogin;
	}

	return tOnline;
}

void caclonline_help()
{
	printf("参数错误!\n");
	printf("./caclonline world_log_3_5 localhost mmog mmog 120815 120830 897986512123\n");
}

// caclonline world_log_3_5 localhost mmog mmog '2012-08-15 20:20:20' '2012-09-15 20:20:20' 897986512123
int  main (int argc, char* argv[])
{
	memset(&stEnv, 0, sizeof(stEnv));

	MYSQL_RES *res;
	MYSQL_ROW row;
	int err;
	int iRet = -1;
	unsigned long long ullRoleID;
	int i;
	unsigned int  uiOnline = 0;
	time_t tStart,tEnd;

	if (argc != 8)
	{
		caclonline_help();
		return -1;
	}
	
	MYSQL *conn = mysql_init(NULL);
	if(!conn)
	{	
		printf("mysql_init fail\n");
		return -1;
	}
		

	if(!(mysql_real_connect(
		conn,
		argv[2],
		argv[3],
		argv[4],
		argv[1],
		0,
		NULL,
		CLIENT_COMPRESS)))
	{
		printf("mysql_real_connect fail\n");
		goto end;
	}

	ullRoleID = my_atoull(argv[7]);
	if (ullRoleID <= 0)
	{
		printf("my_atoull fail\n");
		goto end;
	}
		

	if (caclonline_get_table(&stEnv, argv[5], argv[6], conn, ullRoleID))
	{
		printf("caclonline_get_table fail\n");
		goto end;
	}

	qsort(&stEnv.stData[0],stEnv.iNum, sizeof(CACLDATA), caclonline_cmp);
	
	tStart = caclonline_strtime1(argv[5]);
	if (tStart <= 0)
	{
		printf("tStart fail\n");
		goto end;
	}
		
	tEnd = caclonline_strtime1(argv[6]);
	if (tEnd <= 0)
	{
		printf("tEnd fail\n");
		goto end;
	}

	uiOnline = caclonline_in(&stEnv,tStart, tEnd);
	printf("在线时间为:%u秒\n",uiOnline);
	iRet = 0;
	
end:
	if (conn)
		mysql_close(conn);
	
	return iRet;
}

