

#include "mysql/mysql.h"
#include <stdio.h>
#include <time.h>
 #include <string.h>
 #include <stdlib.h>

// 删除iDay天以前的表
int clean_world_log_db(const char *pszTable, int iDay, time_t t, MYSQL *conn)
{
	time_t   tTime;
	struct tm tmTime;
	int err;
	char szSql[128] = {0};
	char szTime[128] = {0};
	int iLen = strlen(pszTable);
	const char *p = pszTable +(iLen-6);

	szTime[0] = '2';
	szTime[1] = '0';
	strcat(szTime,p);
	strcat(szTime,"000000");
	
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

	if (t-tTime >= iDay*24*60*60)
	{
		snprintf(szSql,sizeof(szSql), "DROP TABLE %s", pszTable);
		err = mysql_query(conn,szSql);
		if (err)
		{
			printf("clean_world_log_db:mysql_query is fail!\n");
		}
	}
	return 0;
}

void clean_help()
{
	printf("参数不正确!\n");
	printf("./cleanworldlog world_log_3_10 localhost mmog mmog 60\n");
	
}

//   ./cleanworldlog world_log_3_10 localhost mmog mmog 60
int  main (int argc, char* argv[])
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	int err;
	int iRet = -1;
	time_t t;

	if (argc != 6)
	{
		clean_help();
		return -1;
	}
	
	MYSQL *conn = mysql_init(NULL);
	if(!conn)
		return -1;

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
		goto end;
	}

	err = mysql_query(conn, "SHOW TABLES");
	if(err)
		goto end;
	
	res = mysql_store_result(conn);
	if(!res)
		goto end;

	t = time(NULL);
	row = NULL;
	while((row = mysql_fetch_row(res)) != NULL)
	{
		if(!row[0])
			continue;
		clean_world_log_db((char*)(row[0]), atoi(argv[5]),t,conn);
	}

	iRet = 0;

end:
	if (conn)
		mysql_close(conn);
	
	return iRet;
}
