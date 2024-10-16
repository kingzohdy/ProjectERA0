#include "pal/pal.h"
#include "tdr/tdr.h"
#include "proto_comm.h"
#include "role_db_meta.h"
#include "op_log_meta.h"

extern int tdr_dbms_fetch_field(OUT char **a_ppszFieldVal, OUT unsigned int *a_pulLen, IN TDRDBRESULTHANDLE a_hDBResult, IN const char *a_szName);

char g_szBuff[1024000]; //注意这里最好用全局静态变量或动态分配，有可能堆栈溢出
char g_szTxtBuff[1024000]; //注意这里最好用全局静态变量或动态分配，有可能堆栈溢出
char g_szBuff1[1024000];
char g_szTxtBuff1[1024000];

/*环境: mysql 5.0,5.1, 32位操作系统*/
int main(int argc, char* argv[])
{
	int i;
	int iRet;
	int iNumRows;
	TDRDBMS stDBMS;
	TDRDBOBJECT stDBObj;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult;
	LPTDRMETALIB pstMetaLib;
	TDRDBHANDLE hDBHandle;
	TDRDBHANDLE hDBHandle1;
	TDRDBRESULTHANDLE hDBResult1;
	char szErrorMsg[2048];
	
	//装载数据结构描述的meta 库，role_db_meta.tdr 每个版本都会更新
	if (0 > tdr_load_metalib(&pstMetaLib, "./role_db_meta.tdr"))
	{
		printf("load meta lib  fail\n");
		return -1;
	}

	/*设置连接数据库参数，工具可以放入到配置文件中
	*/
	memset(&stDBMS, 0, sizeof(stDBMS));
	STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
	STRNCPY(stDBMS.szDBMSCurDatabaseName, "world_3", sizeof(stDBMS.szDBMSCurDatabaseName));
	STRNCPY(stDBMS.szDBMSConnectionInfo, "192.168.0.77", sizeof(stDBMS.szDBMSConnectionInfo));	
	STRNCPY(stDBMS.szDBMSUser, "mmog", sizeof(stDBMS.szDBMSUser));
	STRNCPY(stDBMS.szDBMSPassword, "mmog", sizeof(stDBMS.szDBMSPassword));	

	if (0 > tdr_open_dbhanlde(&hDBHandle, &stDBMS, szErrorMsg))
	{
		printf("tdr_open_dbhanlde fail: %s", szErrorMsg);
		return -1;
	}

	//由于需要演示xml 格式数据导入到数据库，需要另一个db handle
	if (0 > tdr_open_dbhanlde(&hDBHandle1, &stDBMS, szErrorMsg))
	{
		printf("tdr_open_dbhanlde fail: %s", szErrorMsg);
		return -1;
	}

	/*获取某些数据记录
	*/
	stSql.pszBuff = g_szBuff;
	//stSql.iBuff是sql语句的真正长度
	stSql.iBuff = snprintf(stSql.pszBuff, sizeof(g_szBuff), 
		"select * from RoleData where RoleName='kent99' or RoleName='kent100' ");
       if(tdr_query(&hDBResult, hDBHandle, &stSql) != 0)
	{
		printf("执行SQL语句失败!\n");
		return -1;
	}
	iNumRows = tdr_num_rows(hDBResult);

	stDBObj.pstMeta = tdr_get_meta_by_name(pstMetaLib, "RoleData");
	stDBObj.iVersion = tdr_get_meta_current_version(stDBObj.pstMeta);
	for (i=0; i<iNumRows; i++)
	{
		stDBObj.iObjSize = sizeof(g_szBuff);
		stDBObj.pszObj = g_szBuff;
		iRet = tdr_fetch_row(hDBResult, &stDBObj);
		if (0 > iRet)
		{
			printf ("fetch err\n");
			return -1;
		}

		//------展示如何解析某一行数据库记录----
		
		/*----内存格式解析----
		   优点: 解析速度最快，使用方式最灵活
		   缺点: 每次更新，不仅要更新role_db_meta.tdr，而且还需要使用最新的.h头文件重新编译工具
		*/
		{
			ROLEDATA stRoleData;

			assert(sizeof(stRoleData) == stDBObj.iObjSize);

			/*上面解析时可以直接
			stDBObj.iObjSize = sizeof(stRoleData);
			stDBObj.pszObj = (char *)&stRoleData;
			避免多一次拷贝
			*/
			memcpy(&stRoleData, stDBObj.pszObj, stDBObj.iObjSize);

			printf("RoleName=%s, wear arm num=%d\n", stRoleData.RoleName,
				stRoleData.Wear.ArmNum);
		}
		
		/*----xml格式解析----
		   优点: 每次只要更新role_db_meta.tdr文件，不需要重新编译工具，
		   缺点: 需要对xml格式再次解析
		*/
		{
			TDRDATA stXml;
			TDRDATA stHost;

			stXml.iBuff = sizeof(g_szTxtBuff);
			stXml.pszBuff = g_szTxtBuff;
			stHost.iBuff = stDBObj.iObjSize;
			stHost.pszBuff = stDBObj.pszObj;
			if (0 > tdr_output(stDBObj.pstMeta, &stXml, &stHost, 0, TDR_IO_NEW_XML_VERSION))
			{
				printf("tdr_output fail\n");
			}
			else
			{
				printf("xml =%s\n", stXml.pszBuff);
			}

			/* 可以使用接口将xml 格式的数据写回数据库
			    注意，不要改变主键，另外，由于需要执行
			    tdr_query，所以需要另一个hDBHandle1 和hDBResult1
			*/
			{
				TDRDATA stHost1;
				TDRDBOBJECT stDBObj1;
				TDRDATA stSql1;

				stHost1.iBuff = sizeof(g_szBuff1);
				stHost1.pszBuff = g_szBuff1;

				if (0 > tdr_input(stDBObj.pstMeta, &stHost1, &stXml, 0, TDR_IO_NEW_XML_VERSION))
				{
					printf("tdr_input fail\n");
				}
				else
				{
					stDBObj1.pstMeta = stDBObj.pstMeta;
					stDBObj1.iVersion = stDBObj.iVersion;
					stDBObj1.iObjSize = stHost1.iBuff;
					stDBObj1.pszObj = stHost1.pszBuff;
					stSql1.iBuff = sizeof(g_szTxtBuff1);
					stSql1.pszBuff = g_szTxtBuff1;

					if (0 > tdr_obj2sql(hDBHandle1, &stSql1, TDR_DBOP_UPDATE, &stDBObj1, NULL))
					{
						printf("tdr_obj2sql fail\n");
					}
					else
					{
						if (0 > tdr_query(&hDBResult1, hDBHandle1, &stSql1))
						{
							printf("tdr_query fail\n");
						}
						
						tdr_free_dbresult(&hDBResult1);
					}
				}
			}
		}

		//----txt格式解析----
		/*----xml格式解析----
		   优点: 每次只要更新role_db_meta.tdr文件，不需要重新编译工具，
		   缺点: 比xml格式更难解析
		*/
		
		{
			TDRDATA stTxt;
			TDRDATA stHost;

			stTxt.iBuff = sizeof(g_szTxtBuff);
			stTxt.pszBuff = g_szTxtBuff;
			stHost.iBuff = stDBObj.iObjSize;
			stHost.pszBuff = stDBObj.pszObj;
			if (0 > tdr_sprintf(stDBObj.pstMeta, &stTxt, &stHost, 0))
			{
				printf("tdr_sprintf fail\n");
			}
			else
			{
				printf("txt =%s\n", stTxt.pszBuff);
			}
		}

		//----展示如何解析某个记录的某个blob字段----
		{
			char *pszFieldVal;
			unsigned int dwFieldLen;
			int iVersion;
			int iBlobBuff;
			LPTDRMETA pstMeta;
			TDRDATA stData;
			TDRDATA stHost;
	
			/* 可以不用下面这个函数，而使用mysql接口把blob字段读出来
			*/
			if (0 > tdr_dbms_fetch_field(&pszFieldVal, &dwFieldLen, hDBResult, "Package"))
			{
				continue;
			}

			/*blob的格式:
			   开始4个字节是这个blob的数据版本Version;
			   接着的4个字节是这个blob的长度；
			   后面的就是打包的内容
			*/
			iVersion = ntohl(*((long*)pszFieldVal));
			pszFieldVal += sizeof(long);

			iBlobBuff = ntohl(*((long*)pszFieldVal));
			pszFieldVal += sizeof(long);

			//如果知道某个blob字段的meta name: 需要参考相关的xml文件
			pstMeta = tdr_get_meta_by_name(pstMetaLib, "RolePackage");
			if (NULL == pstMeta)
			{
				continue;
			}

			stData.iBuff = iBlobBuff;
			stData.pszBuff = pszFieldVal;	
			stHost.iBuff = sizeof(g_szBuff);
			stHost.pszBuff = g_szBuff;
			if (0 > tdr_ntoh(pstMeta, &stHost, &stData, iVersion))
			{
				continue;
			}
			
			//----内存格式解析，优缺点见前述----
			{
				ROLEPACKAGE stPackage;

				//可以stHost.iBuff=sizeof(stPackage) stHost.pszBuff=&stPackage减少一次拷贝
				memcpy(&stPackage, stHost.pszBuff, sizeof(stPackage));
				printf("SundriesPackage num=%d\n", stPackage.SundriesPackage.Num);
			}

			//----xml格式解析，优缺点见前述----
			{
				TDRDATA stXml;
			
				stXml.iBuff = sizeof(g_szTxtBuff);
				stXml.pszBuff = g_szTxtBuff;
				
				if (0 > tdr_output(pstMeta, &stXml, &stHost, 0, TDR_IO_NEW_XML_VERSION))
				{
					printf("tdr_output fail\n");
				}
				else
				{
					printf("xml =%s\n", stXml.pszBuff);
				}
			}
			
			//----txt格式解析，优缺点见前述----
			{
				TDRDATA stTxt;
				
				stTxt.iBuff = sizeof(g_szTxtBuff);
				stTxt.pszBuff = g_szTxtBuff;
				if (0 > tdr_sprintf(pstMeta, &stTxt, &stHost, 0))
				{
					printf("tdr_sprintf fail\n");
				}
				else
				{
					printf("txt =%s\n", stTxt.pszBuff);
				}
			}
			
			//-----------------------------------------------------------
		}
	}

	//对应tdr_query
	tdr_free_dbresult(&hDBResult);

	//对应tdr_open_dbhanlde
	tdr_close_dbhanlde(hDBHandle);
	tdr_close_dbhanlde(hDBHandle1);
	
	return 0;
}
