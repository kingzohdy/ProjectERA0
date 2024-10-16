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
* �����ַ��� pstr �е� �ַ� c 
* @IN    pstr             Դ�ַ���
*@IN    c                  Ҫ���ǵ��ַ�
* return :                  ���ǵ� �ַ� c ����ַ����׵�ַ
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
		printf("��ʼ��ʧ��!\n");
		iErr = -1;
		goto final;
	}
	
	if (!pHandle)
	{
		printf("��Ч�ľ��!\n");
		iErr = -1;
		goto final;
	}
	
	//��һ��ָ�����˺�
	
	if(pszAccountName )
	{
		iTotal++;
		//printf("name:%s\n",pszAccountName);
		iRet = cs_set_account_freeze(pHandle, iRegionID, pszAccountName, 1, iFreezeTime,"7", TIMEOUT);
		if(!iRet)
		{
			iSucceed++;
			printf("�쳣�˺�:%s  �ɹ����%d��!\n", pszAccountName, iFreezeTime);
		}
		else
		{
			printf("�쳣�˺�:%s  ���ʧ��!\n", pszAccountName);
		}
		goto final;
	}
	
	//���ļ��е��˺�
	pf = fopen("cfg/exception_account.txt", "r");
	if(!pf)
	{
		iErr = -2;
		printf("���ļ� cfg/exception_account.txt ʧ��\n");
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
			printf("�쳣�˺�:%s  �ɹ����%d��!\n", stAccountErr.szLoginName, iFreezeTime);
		}
		else
		{
			printf("�쳣�˺�:%s  ���ʧ��!\n", stAccountErr.szLoginName);
		}
		
	}
	
	printf("һ��%d���˺��쳣,��ųɹ�%d��,ʧ��%d��\n", iTotal, iSucceed, iTotal-iSucceed);
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
		printf("��������\n");
		Ret = -1;
		goto final;
	}
	/*���ݿ����Ӳ��ַ��ں�����*/
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
		printf("���ļ�cfg/exception_account.txtʧ��\n");
		iErr = -1;
		goto error;
	}
	


	if(init_interface("cfg/proto_ss.tdr", &pHandle, "cfg/cs_cgi.xml"))
	{
		printf("��ʼ��Handle ʧ��!\n");
		iErr = -1;
		goto error;
	}

	if(!pHandle)
	{
		printf("��ʼ��Handle ʧ��!\n");
		iErr = -1;
		goto error;
	}
	
	//������ID ���ID ȷ��Ҫ�õ����ݿ�����
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
	
	//��ʼ�������� Account ���ݿ�,�Թ�mail ���ʱͨ��RoleName ����LoginName
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
			printf("�쳣 Mac Check:\n");
		break;
	case EXCHANGE_CHECK:
		snprintf(szSql,sizeof(szSql),"show tables like 'LogRoleExchg_%s';",pszCheckDate);
			
		if(nType == TEST)
			printf("�쳣���� Check:\n");
		break;
	case MAIL_CHECK:
		snprintf(szSql,sizeof(szSql),"show tables like 'LogRoleMail_%s';",pszCheckDate);
			
		if(nType == TEST)
			printf("�쳣�����ʼ� Check:\n");
		break;

	case CLIENTNUM_CHECK:
	case CLIENTIP_CHECK:	
		snprintf(szSql,sizeof(szSql),"show tables like 'AccountRole';");
			
		if(nType == TEST)
		{
			printf("�쳣�ͻ������� Check:\n");
		}
		break;
	}

	if(nType == DO)
	{
		printf("���쳣�˺ŷ��͸����������з��:\n");
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
				printf("������Ͳ�������\n");
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
								printf("����:%s\tAccountName=%s  ���˺��쳣��¼  MAC=%s  ��¼�˺���:%s \n",
								row[4], row[1], row[2], row[3]);
								//snprintf(pszLoginName, sizeof(szLoginName), "%s", row[1]); pszLoginName[sizeof(szLoginName)-1] = '\0';
								//д���ļ�
								fprintf(fp,"LogTime:%s\t%s\n",row[4], row[1]);
								break;
							case EXCHANGE_CHECK:
								printf("����:%s\tAccountName=%s  RoleName=%s  Exchange Times=%s  TotalInMoney=%s �����쳣\n",
								row[3],row[0], row[1], row[2],row[4]);
								//snprintf(pszLoginName, sizeof(szLoginName), "%s", row[0]); pszLoginName[sizeof(szLoginName)-1] = '\0';
								fprintf(fp,"LogTime:%s\t%s\n",row[3], row[0]);
								break;
							case MAIL_CHECK:
								if(get_loginname_by_rolename(AccConn, row[0], szLoginName, sizeof(szLoginName))<0)
								{	
									printf("�Ҳ���RoleName=%s ��Ӧ���˺���\n",row[0]);
									continue;
								}
								printf("����:%s\tAccountName=%s   RoleName=%s   Times=%s  �����ʼ��쳣\n", row[2], szLoginName, row[0], row[1]);
								//get_loginname_by_rolename(MySqlIP, MySqlPort, szUserName, szPasswd, row[0], region_id, szLoginName);
								//pszLoginName[sizeof(szLoginName)-1] = '\0';
								fprintf(fp,"LogTime:%s\t%s\n",row[2], szLoginName);
								break;

							case CLIENTNUM_CHECK:
								
								printf("MacAddr:%s\tAccountName:%s\tUin:%d\tNum:%d  �ͻ��������쳣\n", 
									row[0], row[1], atoi(row[2]), atoi(row[3]));								
								fprintf(fp,"LogTime:%s\t%s\n","0", row[1]);
								break;
							case CLIENTIP_CHECK:
								
								printf("IP:%s\tAccountName:%s\tUin:%d\tNum:%d  �ͻ���ip �����쳣\n", 
									row[0], row[1], atoi(row[2]), atoi(row[3]));								
								fprintf(fp,"LogTime:%s\t%s\n","0", row[1]);
								break;	
							default:
								printf("������Ͳ�������\n");
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
									printf("�Ҳ���RoleName=%s ��Ӧ���˺���\n",row[0]);
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
								printf("������Ͳ�������\n");
								iErr = -1;
								goto error;
								break;
							}
							
							ret = scan_set_account_freeze(pHandle, region_id, szLoginName, 1, iFreezeTime, TIMEOUT);
		
							if(!ret)
							{
								++iFreezed;
								printf("�쳣�˺�:%s  �ɹ����%d��!\n", szLoginName, iFreezeTime);
							}
							else
							{
								printf("�쳣�˺�:%s  ���ʧ��!err:%d\n", szLoginName,ret);
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
		printf("����%d���˺��쳣\n", iTotal);
		break;
	case DO:
		printf("����%d���˺��쳣,��ųɹ�%d��,ʧ��%d��\n", iTotal, iFreezed, iTotal-iFreezed);
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
	short nCheck = 0;  //10  ��¼�쳣, 20  �����쳣,  30 �ʼ��쳣,40 ��ָ���˺�
	char *pszCheckDate = "%";
	char *pszLoginName = NULL;
	int iFreezeTime = FREEZE_TIME;
	int iLimit = 0;
	int iLimit1 = 0;
	if(argc<2 || argc >13)
	{
		printf("�������, ����./world_scan  help �鿴�÷�\n");
		return -1;
	}
	
	if(argc == 2)
	{	
		if(0 != strncmp(argv[1],"help",strlen("help")))
		{
			printf("�������, ����./world_scan  help �鿴�÷�\n");
			return -1;
		}
		else
		{
			printf("\n����:����쳣�˺Ż��쳣��ɫ,����⵽, ����˺�(Ĭ��3 ��),�쳣����:"
				"\n\tͬһMAC��ַһ�����г���30����ͬ�˺ŵ�¼"
				"\n\tͨ�����׻����Ʒ���Ǯ50��"
				"\n\tͬһMAC��ַͬʱ������������3��"
				"\n    ʹ��ǰ��Ҫ���ú� cfg/cs_cgi.xml �ļ�\n");
			printf("\n�÷�: ./world_scan  command args ... \n");
			printf("example:   ./world_scan  freeze   RegionID  [FreezeTime  [LoginName]]\n");
			printf("           ./world_scan  test|do  MySqlIP  MySqlPort  usename  password  RegionID  WorldID  CheckType [Limit [Date [FreezeTime]]]\n");
			printf("command:    freeze     ֱ��ָ���˺Ž��з��\n");
			printf("            test       ��ӡ�쳣�û�����, �����쳣�˺ŵĵ�¼���ڡ��˺���������ļ� cfg/exception_account.txt\n");
			printf("            do         ɨ���쳣�û����ݲ����͵�account_cs_svr���з��ʺ�(Ĭ��3��)\n");
			printf("RegionID:              �˺����ڴ���ID\n");
			printf("FreezeTime:            ��ʾ���ʱ��(��λ��), Ĭ��Ϊ 3 ��\n");
			printf("LoginName:             ָ����ŵĵ�¼�˻���,Ĭ�ϴ�cfg/exception_account.txt(��test ����)�л�ȡ\n");
			printf("MySqlIP:               MySql ��IP ��ַ\n");
			printf("MySqlPort:             MySql �Ķ˿ں�\n");
			printf("usename:               MySql ���û���\n");
			printf("password:              MySql ������\n");
			printf("WorldID:               World ID\n");
			printf("CheckType:             �쳣��Ϊ\n");
			printf("             login     ��¼�쳣  \n");
			printf("             exchg     �����쳣 \n");
			printf("             mail      �ʼ��쳣  \n");
			printf("             clientnum      �ͻ�������   \n");
			printf("Limit:                 ��Ϊ��������,���ڵ��ڴ���ֵ����Ϊ����Ϊ�쳣,login Ĭ��Ϊ30, exchg, mail Ĭ��Ϊ50\n");
			printf("Date:                  ʹ�ô˲�����Ҫ���� Limit. ��ʾ��־�ļ�������, �� 121105 ��ʾ 2012-11-05, "
			                            "����ò���Ϊ 0 ��ָ��,��ɨ�����ݿ�������������͵���־��\n");
			
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
		printf("�������, ����./world_scan  help �鿴�÷�\n");
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
			printf("����̫��, ����./world_scan  help �鿴�÷�\n");
			return -1;
		}
	}
	else
	{
		if(argc <9)
		{
			printf("����̫��, ����./world_scan  help �鿴�÷�\n");
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
	
	//��ָ�����˺�
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



