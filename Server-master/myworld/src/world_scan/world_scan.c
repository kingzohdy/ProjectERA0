/*
* world_scan.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cs_cgi.h"
#include "mysql/mysql.h"
#include <time.h>

#define LOGIN_IP_NUM 10
#define LOGIN_TIMES    30
#define EXCHG_TIMES   50
#define MAIL_TIMES      50
#define CLIENTNUM_TIMES      3

#define FREEZE_TIME  3*86400
#define TIMEOUT         10000

#define TEST     1
#define DO        2
#define FREEZE 3

#define LOGIN_CHECK          10
#define EXCHANGE_CHECK   20
#define MAIL_CHECK            30
#define CLIENTNUM_CHECK            40
#define CLIENTIP_CHECK            50
//#define FREEZE_ACC_NAME  40 

/*
* 过虑字符串 pstr 中的 字符 c 
* @IN    pstr             源字符串
*@IN    c                  要过虑的字符
* return :                  过虑掉 字符 c 后的字符串首地址
*/
char *filter_char(const char *pstr, char c)
{
	int i = 0;
	int iLen = 0;
	char *ptmp = NULL;
	char *pDest = NULL;
	
	if(!pstr)
	{
		return NULL;
	}
	
	iLen = strlen(pstr);
	ptmp = (char *)pstr;
	pDest = (char *)malloc(iLen+1);
	
	if(!pDest)
	{
		printf("malloc fail\n");
		return NULL;
	}
	{
		char *p = pDest;
		memset(p, 0, iLen+1);
		while(i <iLen)
		{
			ptmp = (char*)pstr + i;
			if( *ptmp != c )
				*p++ = *ptmp;
			i++;
		}
	}
	if(!pDest[0])
	{
		free(pDest);
		return NULL;
	}
	return pDest;
}

int scan_set_acc_freeze_by_name(int iRegionID, const char *pszAccountName, int iFreezeTime)
{
	int iErr = 0;
	int iRet = 0;
	int iTotal = 0;
	int iSucceed = 0;
	FILE *pf = NULL;
	struct {
		char szDate[40];
		char szLoginName[40];
		//int iFreezeTime;
	}stAccountErr;
	
	int *pHandle = NULL;
	
	if(init_interface("cfg/proto_ss.tdr", &pHandle, "cfg/cs_cgi.xml"))
	{
		printf("初始化失败!\n");
		iErr = -1;
		goto final;
	}
	
	if (!pHandle)
	{
		printf("无效的句柄!\n");
		iErr = -1;
		goto final;
	}
	
	//封一个指定的账号
	
	if(pszAccountName )
	{
		iTotal++;
		//printf("name:%s\n",pszAccountName);
		iRet = cs_set_account_freeze(pHandle, iRegionID, pszAccountName, 1, iFreezeTime,"7", TIMEOUT);
		if(!iRet)
		{
			iSucceed++;
			printf("异常账号:%s  成功封号%d秒!\n", pszAccountName, iFreezeTime);
		}
		else
		{
			printf("异常账号:%s  封号失败!\n", pszAccountName);
		}
		goto final;
	}
	
	//封文件中的账号
	pf = fopen("cfg/exception_account.txt", "r");
	if(!pf)
	{
		iErr = -2;
		printf("打开文件 cfg/exception_account.txt 失败\n");
		goto final;
	}
	
	while(fscanf(pf, "%s%s", stAccountErr.szDate, stAccountErr.szLoginName) != EOF)
	{
		iTotal++;
		//printf("Date:%s  AccName:%s\n", stAccountErr.szDate, stAccountErr.szLoginName);
		
		iRet = cs_set_account_freeze(pHandle, iRegionID, stAccountErr.szLoginName, 1, iFreezeTime,"7", TIMEOUT);
		if(!iRet)
		{
			iSucceed++;
			printf("异常账号:%s  成功封号%d秒!\n", stAccountErr.szLoginName, iFreezeTime);
		}
		else
		{
			printf("异常账号:%s  封号失败!\n", stAccountErr.szLoginName);
		}
		
	}
	
	printf("一共%d个账号异常,封号成功%d个,失败%d个\n", iTotal, iSucceed, iTotal-iSucceed);
final:
	if(pHandle)
		destory_interface(pHandle);
	if(pf)
		fclose(pf);
	return iErr;
}

int scan_set_account_freeze(int * pHandle, int iRegionID, const char * pszLoginName, 
                                                            int iEnable, int iFreezeTime, int iTimeOut)
{
	int iErr = 0;

	iErr = cs_set_account_freeze(pHandle, iRegionID, pszLoginName, iEnable, iFreezeTime,"7", iTimeOut);
	
	return iErr;
}

static int get_loginname_by_rolename(MYSQL *pstMySql, const char *pszRoleName, char *pszLoginName, int iLoginName)
{
	int Ret = 0;
	char szSql[1024];
	//char szDataBase[40];
	MYSQL_RES * res;
	MYSQL_ROW row;
	
	if(NULL == pszLoginName)
	{
		printf("参数错误\n");
		Ret = -1;
		goto final;
	}
	/*数据库连接部分放在函数外*/
	/*
	snprintf(szDataBase, sizeof(szDataBase),  "account_%d", iRegionID);
	MYSQL *conn = mysql_init(NULL);
	if(!conn)
	{
		printf("mysql_init fail\n");
		Ret = -2;
		goto final;
	}
	
	if(!mysql_real_connect(
		conn,
		MySqlIP,
		szUserName,
		szPasswd,
		szDataBase,
		MySqlPort,
		NULL,
		CLIENT_COMPRESS|CLIENT_MULTI_STATEMENTS))
	{
		printf("mysql_real_connect fail\n");
		Ret = -3;
		goto final;
	}
	*/
	snprintf(szSql, sizeof(szSql), "SELECT  Uin, Pass9Account FROM `AccountRole` where RoleName='%s';",pszRoleName);
	
	if(mysql_query(pstMySql, szSql))
	{
		fprintf(stderr, "mysql_query \"%s\" fail\nError:%s\n", szSql, mysql_error(pstMySql));
		Ret = -2;
		goto final;
	}

	res = mysql_store_result(pstMySql);
	if(res)
	{
		row=mysql_fetch_row(res);
		if(NULL != row && NULL != row[0] )
		{
			strncpy(pszLoginName ,row[1], iLoginName); pszLoginName[iLoginName-1] = '\0';
			Ret = 0;
			goto final;
		}
		else
		{
			printf("fetch none\n");
			Ret = -3;
		}
	}
final:
	if (res)
	{
		mysql_free_result(res);
		res = NULL;
	}
	return Ret;
}

int world_scan(const char * MySqlIP, int MySqlPort, const char *szUserName, const char * szPasswd,
	           int region_id, int world_id , short nType, short nCheckType, const char *pszCheckDate, 
	           int iLimit, int iFreezeTime, int iLimit1)
{
	int *pHandle = NULL;
	int ret = 0;
	int iErr = 0;
	MYSQL_RES *res=NULL;
	MYSQL_RES *restbl=NULL;
	MYSQL_ROW row;
	MYSQL *conn=NULL;
	MYSQL *conn1=NULL;
	MYSQL *AccConn=NULL;
	char szSql[1024] = {0};
	char MyDB[256] = {0};
	//char szDate[30] = {0};
	char szLoginName[256] = {0};
	//char *pszLoginName = szLoginName;

	int iTotal = 0;
	int iFreezed = 0;
	
	FILE *fp = NULL;
	if(nType == TEST && !(fp=fopen("cfg/exception_account.txt", "w")))
	{
		printf("打开文件cfg/exception_account.txt失败\n");
		iErr = -1;
		goto error;
	}
	


	if(init_interface("cfg/proto_ss.tdr", &pHandle, "cfg/cs_cgi.xml"))
	{
		printf("初始化Handle 失败!\n");
		iErr = -1;
		goto error;
	}

	if(!pHandle)
	{
		printf("初始化Handle 失败!\n");
		iErr = -1;
		goto error;
	}
	
	//根据区ID 与服ID 确定要用的数据库名称
	snprintf(MyDB, sizeof(MyDB), "world_log_%d_%d", region_id, world_id);
	if (nCheckType == CLIENTNUM_CHECK || nCheckType == CLIENTIP_CHECK)
	{
		snprintf(MyDB, sizeof(MyDB), "account_%d", region_id);
	}
	
	conn = mysql_init(NULL);
	if(!conn)
	{
		printf("mysql_init fail\n");
		iErr = -1;
		goto error;
	}

	if(!mysql_real_connect(
		conn,
		MySqlIP,
		szUserName,
		szPasswd,
		MyDB,
		MySqlPort,
		NULL,
		CLIENT_COMPRESS|CLIENT_MULTI_STATEMENTS))
	{
		fprintf(stderr, "mysql_real_connect %s fail, Error:%s \n",MyDB, mysql_error(conn));
		iErr = -1;
		goto error;
	}

	conn1 = mysql_init(NULL);
	if(!conn1)
	{
		printf("mysql_init fail\n");
		iErr = -1;
		goto error;
	}

	if(!mysql_real_connect(
		conn1,
		MySqlIP,
		szUserName,
		szPasswd,
		MyDB,
		MySqlPort,
		NULL,
		CLIENT_COMPRESS|CLIENT_MULTI_STATEMENTS))
	{
		fprintf(stderr, "mysql_real_connect %s fail, Error:%s \n",MyDB, mysql_error(conn1));
		iErr = -1;
		goto error;
	}
	
	//初始化并连接 Account 数据库,以供mail 检测时通过RoleName 查找LoginName
	snprintf(MyDB, sizeof(MyDB),  "account_%d", region_id);
	AccConn = mysql_init(NULL);
	if(!AccConn)
	{
		printf("mysql_init fail\n");
		iErr = -1;
		goto error;
	}
	
	if(nCheckType == MAIL_CHECK && !mysql_real_connect(
		AccConn,
		MySqlIP,
		szUserName,
		szPasswd,
		MyDB,
		MySqlPort,
		NULL,
		CLIENT_COMPRESS|CLIENT_MULTI_STATEMENTS))
	{
		fprintf(stderr, "mysql_real_connect %s fail, Error:%s \n",MyDB, mysql_error(AccConn));
		iErr = -1;
		goto error;
	}

	switch(nCheckType)
	{
	case LOGIN_CHECK:
		//if(NULL != filter_char(pszCheckDate, '-'))
		//	snprintf(szSql,sizeof(szSql),"show tables like 'LogRoleLogin_%s';",filter_char(pszCheckDate, '-'));
		snprintf(szSql,sizeof(szSql),"show tables like 'LogRoleLogin_%s';",pszCheckDate);
		
		if(nType == TEST)
			printf("异常 Mac Check:\n");
		break;
	case EXCHANGE_CHECK:
		snprintf(szSql,sizeof(szSql),"show tables like 'LogRoleExchg_%s';",pszCheckDate);
			
		if(nType == TEST)
			printf("异常交易 Check:\n");
		break;
	case MAIL_CHECK:
		snprintf(szSql,sizeof(szSql),"show tables like 'LogRoleMail_%s';",pszCheckDate);
			
		if(nType == TEST)
			printf("异常接收邮件 Check:\n");
		break;

	case CLIENTNUM_CHECK:
	case CLIENTIP_CHECK:	
		snprintf(szSql,sizeof(szSql),"show tables like 'AccountRole';");
			
		if(nType == TEST)
		{
			printf("异常客户端数量 Check:\n");
		}
		break;
	}

	if(nType == DO)
	{
		printf("将异常账号发送给服务器进行封号:\n");
	}

	
	if(mysql_query(conn1, szSql))
	{
		fprintf(stderr, "mysql_query \"%s\" fail\nError:%s\n", szSql, mysql_error(conn1));
		iErr = -1;
		goto error;
	}

	res = mysql_store_result(conn1);
	if(res)
	{
		while( (row=mysql_fetch_row(res)) != NULL)
		{
			//snprintf(szDate, sizeof(szDate), "%s", strchr(row[0], '_')+1);
			switch(nCheckType)
			{
			case LOGIN_CHECK:
				snprintf(szSql,sizeof(szSql), "SELECT DISTINCT Uin, AccountLoginName, t1.MacAddr, t2.LoginCount, "
				"str_to_date(t1.LogTime,'%%Y-%%m-%%d') as LogDate from %s as t1 inner join " 
				"( SELECT Macaddr, COUNT(DISTINCT Uin) as LoginCount  FROM %s  WHERE MacAddr<>'' and MacAddr <> '00-00-00-00-00-00'  "
				"group by MacAddr having count(distinct Uin)>=%d ) as t2 on (t1.MacAddr=t2.MacAddr);", row[0], row[0], iLimit);
				break;
			case EXCHANGE_CHECK:
				snprintf(szSql,sizeof(szSql), "SELECT LoginName, RoleName, count(*) as ExchgTimes, str_to_date(LogTime,'%%Y-%%m-%%d') as LogDate,  "
				        "sum(ExchgIn_Money) as total FROM  %s where ExchgOut_Money=0 and ExchgOut_ItemNum=0 and ExchgOut_PetNum = 0 "
			            "GROUP BY RoleID having ExchgTimes >= %d;",  row[0], iLimit);
				break;
			case MAIL_CHECK:
				snprintf(szSql,sizeof(szSql), "SELECT RoleMailData_To, count(*) as count, str_to_date(LogTime,'%%Y-%%m-%%d') as LogDate  FROM  %s  "
			            "where RoleMailData_Flags = 0 and (RoleMailData_ItemNum >= 1 or RoleMailData_Money >=1)  "
			            "GROUP BY RoleMailData_To having count(*) >= %d;",  row[0], iLimit);
				break;

			case CLIENTNUM_CHECK:
				snprintf(szSql,sizeof(szSql), "SELECT b.MacAddr,a.Pass9Account,a.Uin,b.ClientNum,a.OnlineFlag from AccountRole a right join (SELECT Uin,Pass9Account,OnlineFlag,MacAddr,count(RoleWID) as ClientNum from AccountRole  where OnlineFlag > 0 and MacAddr != 'cpuid is none' and MacAddr not like 'SN%%' and MacAddr != '(null)' "
					" and MacAddr not like 'WD%%'  and MacAddr not like '02-00-4c-4f-4f-50' and MacAddr not like 'IC%%' and MacAddr not like 'ST%%' and MacAddr not like '534F49443859383800' and MacAddr not like '00-53-45-00-00-00' GROUP BY MacAddr HAVING count(RoleWID) > %d)	b on a.MacAddr = b.MacAddr and a.OnlineFlag = 1 GROUP BY Pass9Account ORDER BY b.MacAddr;",iLimit);
				break;

			case CLIENTIP_CHECK:
				snprintf(szSql,sizeof(szSql), "SELECT b.LastLoginIP,a.Pass9Account,a.Uin,b.ClientNum,a.OnlineFlag from AccountRole a right join (SELECT Uin,Pass9Account,OnlineFlag,LastLoginIP,count(RoleWID) as ClientNum from AccountRole  where OnlineFlag > 0 and LastLoginIP != 0 "
					" GROUP BY LastLoginIP HAVING count(RoleWID) > %d)	b on a.LastLoginIP = b.LastLoginIP and a.OnlineFlag = 1 GROUP BY Pass9Account ORDER BY b.LastLoginIP;",iLimit);
				break;
			default:
				printf("检测类型参数错误\n");
				iErr = -1;
				goto error;
				break;
			}
			

			if(mysql_query(conn, szSql))
			{
				fprintf(stderr, "mysql_query \"%s\" fail\nError:%s\n", szSql, mysql_error(conn));
				iErr = -1;
				goto error;
			}
			
			
			restbl = mysql_store_result(conn);
			
			if(restbl)
			{
				while((row=mysql_fetch_row(restbl)) != NULL)
				{
					if(row[0] != NULL)
					{
						++iTotal;
						if(TEST == nType && fp)
						{
							switch(nCheckType)
							{
							case LOGIN_CHECK:
								printf("日期:%s\tAccountName=%s  的账号异常登录  MAC=%s  登录账号数:%s \n",
								row[4], row[1], row[2], row[3]);
								//snprintf(pszLoginName, sizeof(szLoginName), "%s", row[1]); pszLoginName[sizeof(szLoginName)-1] = '\0';
								//写入文件
								fprintf(fp,"LogTime:%s\t%s\n",row[4], row[1]);
								break;
							case EXCHANGE_CHECK:
								printf("日期:%s\tAccountName=%s  RoleName=%s  Exchange Times=%s  TotalInMoney=%s 交易异常\n",
								row[3],row[0], row[1], row[2],row[4]);
								//snprintf(pszLoginName, sizeof(szLoginName), "%s", row[0]); pszLoginName[sizeof(szLoginName)-1] = '\0';
								fprintf(fp,"LogTime:%s\t%s\n",row[3], row[0]);
								break;
							case MAIL_CHECK:
								if(get_loginname_by_rolename(AccConn, row[0], szLoginName, sizeof(szLoginName))<0)
								{	
									printf("找不到RoleName=%s 对应的账号名\n",row[0]);
									continue;
								}
								printf("日期:%s\tAccountName=%s   RoleName=%s   Times=%s  接收邮件异常\n", row[2], szLoginName, row[0], row[1]);
								//get_loginname_by_rolename(MySqlIP, MySqlPort, szUserName, szPasswd, row[0], region_id, szLoginName);
								//pszLoginName[sizeof(szLoginName)-1] = '\0';
								fprintf(fp,"LogTime:%s\t%s\n",row[2], szLoginName);
								break;

							case CLIENTNUM_CHECK:
								
								printf("MacAddr:%s\tAccountName:%s\tUin:%d\tNum:%d  客户端数量异常\n", 
									row[0], row[1], atoi(row[2]), atoi(row[3]));								
								fprintf(fp,"LogTime:%s\t%s\n","0", row[1]);
								break;
							case CLIENTIP_CHECK:
								
								printf("IP:%s\tAccountName:%s\tUin:%d\tNum:%d  客户端ip 数量异常\n", 
									row[0], row[1], atoi(row[2]), atoi(row[3]));								
								fprintf(fp,"LogTime:%s\t%s\n","0", row[1]);
								break;	
							default:
								printf("检测类型参数错误\n");
								iErr = -1;
								goto error;
								break;
							}
						}
						else if(DO == nType)
						{
							switch(nCheckType)
							{
							case LOGIN_CHECK:
								snprintf(szLoginName, sizeof(szLoginName), "%s", row[1]); szLoginName[sizeof(szLoginName)-1] = '\0';
								break;
							case EXCHANGE_CHECK:
								snprintf(szLoginName, sizeof(szLoginName), "%s", row[0]); szLoginName[sizeof(szLoginName)-1] = '\0';
								break;
							case MAIL_CHECK:
								if(get_loginname_by_rolename(AccConn, row[0], szLoginName, sizeof(szLoginName))<0)
								{	
									printf("找不到RoleName=%s 对应的账号名\n",row[0]);
									continue;
								}
								break;

							case CLIENTNUM_CHECK:
								snprintf(szLoginName, sizeof(szLoginName), "%s", row[1]); szLoginName[sizeof(szLoginName)-1] = '\0';
								break;
							case CLIENTIP_CHECK:
								snprintf(szLoginName, sizeof(szLoginName), "%s", row[1]); szLoginName[sizeof(szLoginName)-1] = '\0';
								break;	
							default:
								printf("检测类型参数错误\n");
								iErr = -1;
								goto error;
								break;
							}
							
							ret = scan_set_account_freeze(pHandle, region_id, szLoginName, 1, iFreezeTime, TIMEOUT);
		
							if(!ret)
							{
								++iFreezed;
								printf("异常账号:%s  成功封号%d秒!\n", szLoginName, iFreezeTime);
							}
							else
							{
								printf("异常账号:%s  封号失败!err:%d\n", szLoginName,ret);
							}
						}
					}
				}
			}

			if(restbl)
			{
				mysql_free_result(restbl);
			}
		}
	}
	
	switch(nType)
	{
	case TEST:
		printf("发现%d个账号异常\n", iTotal);
		break;
	case DO:
		printf("发现%d个账号异常,封号成功%d个,失败%d个\n", iTotal, iFreezed, iTotal-iFreezed);
		break;
	}

	printf("end with success\n");////////

error:
	if(fp)
		fclose(fp);
	if(res)
		mysql_free_result(res);
	if(pHandle)
		destory_interface(pHandle);
	
	return iErr;
}

int main(int argc, char *argv[])
{
	time_t tTime;
	struct tm *ptmNow;
	char szSuffix[16];
	int MySqlPort = 3306;
	int RegionID = 0;
	int WorldID = 0 ;
	
	short nType = TEST ;  // 0  test,  1  do, 2 freeze
	short nCheck = 0;  //10  登录异常, 20  交易异常,  30 邮件异常,40 封指定账号
	char *pszCheckDate = "%";
	char *pszLoginName = NULL;
	int iFreezeTime = FREEZE_TIME;
	int iLimit = 0;
	int iLimit1 = 0;
	if(argc<2 || argc >13)
	{
		printf("错误参数, 请用./world_scan  help 查看用法\n");
		return -1;
	}
	
	if(argc == 2)
	{	
		if(0 != strncmp(argv[1],"help",strlen("help")))
		{
			printf("错误参数, 请用./world_scan  help 查看用法\n");
			return -1;
		}
		else
		{
			printf("\n功能:检测异常账号或异常角色,若检测到, 则封账号(默认3 天),异常包括:"
				"\n\t同一MAC地址一天内有超过30个不同账号登录"
				"\n\t通过交易获得物品或金钱50次"
				"\n\t同一MAC地址同时在线数量超过3个"
				"\n    使用前需要配置好 cfg/cs_cgi.xml 文件\n");
			printf("\n用法: ./world_scan  command args ... \n");
			printf("example:   ./world_scan  freeze   RegionID  [FreezeTime  [LoginName]]\n");
			printf("           ./world_scan  test|do  MySqlIP  MySqlPort  usename  password  RegionID  WorldID  CheckType [Limit [Date [FreezeTime]]]\n");
			printf("command:    freeze     直接指定账号进行封号\n");
			printf("            test       打印异常用户数据, 并将异常账号的登录日期、账号名输出到文件 cfg/exception_account.txt\n");
			printf("            do         扫描异常用户数据并发送到account_cs_svr进行封帐号(默认3天)\n");
			printf("RegionID:              账号所在大区ID\n");
			printf("FreezeTime:            表示封号时间(单位秒), 默认为 3 天\n");
			printf("LoginName:             指定封号的登录账户名,默认从cfg/exception_account.txt(由test 生成)中获取\n");
			printf("MySqlIP:               MySql 的IP 地址\n");
			printf("MySqlPort:             MySql 的端口号\n");
			printf("usename:               MySql 的用户名\n");
			printf("password:              MySql 的密码\n");
			printf("WorldID:               World ID\n");
			printf("CheckType:             异常行为\n");
			printf("             login     登录异常  \n");
			printf("             exchg     交易异常 \n");
			printf("             mail      邮件异常  \n");
			printf("             clientnum      客户端数量   \n");
			printf("Limit:                 行为次数上限,大于等于此数值则认为该行为异常,login 默认为30, exchg, mail 默认为50\n");
			printf("Date:                  使用此参数需要参数 Limit. 表示日志文件的日期, 如 121105 表示 2012-11-05, "
			                            "如果该参数为 0 或不指定,则扫描数据库中所有相关类型的日志表\n");
			
			return 0;
		}
	}
	else if(0 == strncmp(argv[1], "freeze", strlen("freeze")))
	{
		nType = FREEZE;
	}
	else if(0 == strncmp(argv[1], "test", strlen("test")))
	{
		nType = TEST;
	}
	else if(0 == strncmp(argv[1], "do", strlen("do")))
	{
		nType = DO;
	}
	else
	{
		printf("错误参数, 请用./world_scan  help 查看用法\n");
		return -1;
	}
	
	if(nType == FREEZE)
	{
		if(argc == 3)
			RegionID = atoi(argv[2]);
		else if(argc == 4)
		{
			RegionID = atoi(argv[2]);
			iFreezeTime = atoi(argv[3]);
		}
		else if(argc == 5)
		{
			RegionID = atoi(argv[2]);
			iFreezeTime = atoi(argv[3]);
			pszLoginName = argv[4];
		}
		else
		{
			printf("参数太多, 请用./world_scan  help 查看用法\n");
			return -1;
		}
	}
	else
	{
		if(argc <9)
		{
			printf("参数太少, 请用./world_scan  help 查看用法\n");
			return -1;
		}
		MySqlPort = atoi(argv[3]);
		RegionID = atoi(argv[6]);
		WorldID = atoi(argv[7]);
		if(0 == strncmp(argv[8], "login", strlen("login")))
		{
			nCheck = LOGIN_CHECK;
			iLimit = LOGIN_TIMES;
		}
		else if(0 == strncmp(argv[8], "exchg", strlen("exchg")))
		{
			nCheck = EXCHANGE_CHECK;
			iLimit = EXCHG_TIMES;
			iLimit1 = LOGIN_IP_NUM;
		}
		else if(0 == strncmp(argv[8], "mail", strlen("mail")))
		{
			nCheck = MAIL_CHECK;
			iLimit = MAIL_TIMES;
		}
		else if(0 == strncmp(argv[8], "clientnum", strlen("clientnum")))
		{
			nCheck = CLIENTNUM_CHECK;
			iLimit = CLIENTNUM_TIMES;
		}
		else if(0 == strncmp(argv[8], "clientip", strlen("clientip")))
		{
			nCheck = CLIENTIP_CHECK;
			iLimit = LOGIN_IP_NUM;
		}
		else
		{
			printf("CheckType error\n");
			return -1;
		}
		
	}


	if(argc >= 10)
	{
		iLimit= atoi(argv[9]);
	}
	
	if(argc >= 12)
	{
		iFreezeTime = atoi(argv[11]);
	}

	if(argc >= 13)
	{
		iLimit1 = atoi(argv[12]);
	}

	if(argc >= 11)
	{
		if (0 != strncmp(argv[10], "0", strlen("0")))
		{
			pszCheckDate = argv[10];	
		}
		else
		{
			tTime = time(NULL);
			ptmNow = localtime( &tTime );
			strftime(szSuffix, sizeof(szSuffix), "%y%m%d", ptmNow);
			pszCheckDate = szSuffix;
		}
	}
	
	//封指定的账号
	if(nType== FREEZE && pszLoginName)
	{
		scan_set_acc_freeze_by_name(RegionID, pszLoginName,iFreezeTime);

		return 0;
	}
	
	if (RegionID && WorldID && nType && nCheck)
	{
		world_scan(argv[2], MySqlPort, argv[4], argv[5], RegionID, WorldID, nType, nCheck, pszCheckDate, iLimit, iFreezeTime, iLimit1);
	}

	return 0;
}



