#include "pal/pal.h"
#include "tdr/tdr.h"
#include "proto_comm.h"
#include "role_db_meta.h"
#include "op_log_meta.h"

extern int tdr_dbms_fetch_field(OUT char **a_ppszFieldVal, OUT unsigned int *a_pulLen, IN TDRDBRESULTHANDLE a_hDBResult, IN const char *a_szName);

char g_szBuff[1024000]; //ע�����������ȫ�־�̬������̬���䣬�п��ܶ�ջ���
char g_szTxtBuff[1024000]; //ע�����������ȫ�־�̬������̬���䣬�п��ܶ�ջ���
char g_szBuff1[1024000];
char g_szTxtBuff1[1024000];

/*����: mysql 5.0,5.1, 32λ����ϵͳ*/
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
	
	//װ�����ݽṹ������meta �⣬role_db_meta.tdr ÿ���汾�������
	if (0 > tdr_load_metalib(&pstMetaLib, "./role_db_meta.tdr"))
	{
		printf("load meta lib  fail\n");
		return -1;
	}

	/*�����������ݿ���������߿��Է��뵽�����ļ���
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

	//������Ҫ��ʾxml ��ʽ���ݵ��뵽���ݿ⣬��Ҫ��һ��db handle
	if (0 > tdr_open_dbhanlde(&hDBHandle1, &stDBMS, szErrorMsg))
	{
		printf("tdr_open_dbhanlde fail: %s", szErrorMsg);
		return -1;
	}

	/*��ȡĳЩ���ݼ�¼
	*/
	stSql.pszBuff = g_szBuff;
	//stSql.iBuff��sql������������
	stSql.iBuff = snprintf(stSql.pszBuff, sizeof(g_szBuff), 
		"select * from RoleData where RoleName='kent99' or RoleName='kent100' ");
       if(tdr_query(&hDBResult, hDBHandle, &stSql) != 0)
	{
		printf("ִ��SQL���ʧ��!\n");
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

		//------չʾ��ν���ĳһ�����ݿ��¼----
		
		/*----�ڴ��ʽ����----
		   �ŵ�: �����ٶ���죬ʹ�÷�ʽ�����
		   ȱ��: ÿ�θ��£�����Ҫ����role_db_meta.tdr�����һ���Ҫʹ�����µ�.hͷ�ļ����±��빤��
		*/
		{
			ROLEDATA stRoleData;

			assert(sizeof(stRoleData) == stDBObj.iObjSize);

			/*�������ʱ����ֱ��
			stDBObj.iObjSize = sizeof(stRoleData);
			stDBObj.pszObj = (char *)&stRoleData;
			�����һ�ο���
			*/
			memcpy(&stRoleData, stDBObj.pszObj, stDBObj.iObjSize);

			printf("RoleName=%s, wear arm num=%d\n", stRoleData.RoleName,
				stRoleData.Wear.ArmNum);
		}
		
		/*----xml��ʽ����----
		   �ŵ�: ÿ��ֻҪ����role_db_meta.tdr�ļ�������Ҫ���±��빤�ߣ�
		   ȱ��: ��Ҫ��xml��ʽ�ٴν���
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

			/* ����ʹ�ýӿڽ�xml ��ʽ������д�����ݿ�
			    ע�⣬��Ҫ�ı����������⣬������Ҫִ��
			    tdr_query��������Ҫ��һ��hDBHandle1 ��hDBResult1
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

		//----txt��ʽ����----
		/*----xml��ʽ����----
		   �ŵ�: ÿ��ֻҪ����role_db_meta.tdr�ļ�������Ҫ���±��빤�ߣ�
		   ȱ��: ��xml��ʽ���ѽ���
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

		//----չʾ��ν���ĳ����¼��ĳ��blob�ֶ�----
		{
			char *pszFieldVal;
			unsigned int dwFieldLen;
			int iVersion;
			int iBlobBuff;
			LPTDRMETA pstMeta;
			TDRDATA stData;
			TDRDATA stHost;
	
			/* ���Բ������������������ʹ��mysql�ӿڰ�blob�ֶζ�����
			*/
			if (0 > tdr_dbms_fetch_field(&pszFieldVal, &dwFieldLen, hDBResult, "Package"))
			{
				continue;
			}

			/*blob�ĸ�ʽ:
			   ��ʼ4���ֽ������blob�����ݰ汾Version;
			   ���ŵ�4���ֽ������blob�ĳ��ȣ�
			   ����ľ��Ǵ��������
			*/
			iVersion = ntohl(*((long*)pszFieldVal));
			pszFieldVal += sizeof(long);

			iBlobBuff = ntohl(*((long*)pszFieldVal));
			pszFieldVal += sizeof(long);

			//���֪��ĳ��blob�ֶε�meta name: ��Ҫ�ο���ص�xml�ļ�
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
			
			//----�ڴ��ʽ��������ȱ���ǰ��----
			{
				ROLEPACKAGE stPackage;

				//����stHost.iBuff=sizeof(stPackage) stHost.pszBuff=&stPackage����һ�ο���
				memcpy(&stPackage, stHost.pszBuff, sizeof(stPackage));
				printf("SundriesPackage num=%d\n", stPackage.SundriesPackage.Num);
			}

			//----xml��ʽ��������ȱ���ǰ��----
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
			
			//----txt��ʽ��������ȱ���ǰ��----
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

	//��Ӧtdr_query
	tdr_free_dbresult(&hDBResult);

	//��Ӧtdr_open_dbhanlde
	tdr_close_dbhanlde(hDBHandle);
	tdr_close_dbhanlde(hDBHandle1);
	
	return 0;
}
