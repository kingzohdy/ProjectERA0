#include <stdio.h>
#include <string.h>
#include <time.h>
#include <evs_mysql.h>
#include <evs_inifile.h>
#include "../../../src/comm/md5.h"

using namespace evslib;

typedef struct tagAccountInfo{
	char  account_prefix[128];
	int    account_start_num;
	int    account_total_num;
	char  account_birthday[20];
	int    account_gm_uinflag;
	int    passwd_length;
	char  passwd[32];
}ACCOUNTINFO;

static char gs_szCharList[]	=	"abcdefghijkmnpqrstuvwxyz23456789";
char * rand_str(char* pszBuff, int iLen)
{
	char *pszChars=gs_szCharList;

	int	i, iChars;

	iChars	=	sizeof(gs_szCharList) - 1;

	for ( i=0; i<iLen;i++)
	{	
		pszBuff[i]	=	pszChars[ (int)( (float)iChars*rand()/(RAND_MAX+1.0) ) ];
	}

	return pszBuff;
}

MYSQLINFO 			m_stMySQL; 
ACCOUNTINFO		m_stAccountInfo;
clsMySQL 			m_clsMySQL;

int ReadIni (const char *apszConfig)
{
	int liRet = 0;

	if( apszConfig == NULL )
	{
		printf("conf file is NULL!\n");
		return -1;
	}
	memset(&m_stAccountInfo,0,sizeof(m_stAccountInfo));
	memset(&m_stMySQL,0,sizeof(m_stMySQL));
	
	CIniFile *lpoIni = new CIniFile (apszConfig);
	if (lpoIni == NULL && !lpoIni->IsValid())
	{
		printf("IniFile failed con=%s",apszConfig);
		return -1;
	}

#define		_MYSQL_SEC_  "MYSQL"
	lpoIni->GetString (_MYSQL_SEC_, "IP", 		"localhost", m_stMySQL.sHost, sizeof(m_stMySQL.sHost));
	lpoIni->GetString (_MYSQL_SEC_, "User", 		"root", m_stMySQL.sUser, sizeof(m_stMySQL.sUser));
	lpoIni->GetString (_MYSQL_SEC_, "Passwd", 	"", m_stMySQL.sPasswd, sizeof(m_stMySQL.sPasswd));
	lpoIni->GetString (_MYSQL_SEC_, "DB", 		"", m_stMySQL.sDB, sizeof(m_stMySQL.sDB));

#define		_ACCOUNT_SEC_  "ACCOUNT"
	lpoIni->GetString (_ACCOUNT_SEC_, "account_prefix", "test_", m_stAccountInfo.account_prefix, sizeof(m_stAccountInfo.account_prefix));
	lpoIni->GetInt (_ACCOUNT_SEC_, "account_start_num", 0, &m_stAccountInfo.account_start_num);
	lpoIni->GetInt (_ACCOUNT_SEC_, "account_total_num", 1, &m_stAccountInfo.account_total_num);
	lpoIni->GetString (_ACCOUNT_SEC_, "account_birthday", "19811023", m_stAccountInfo.account_birthday, sizeof(m_stAccountInfo.account_birthday));
	lpoIni->GetInt (_ACCOUNT_SEC_, "account_gm_uinflag", 0, &m_stAccountInfo.account_gm_uinflag);
	lpoIni->GetInt (_ACCOUNT_SEC_, "passwd_length", 0, &m_stAccountInfo.passwd_length);
	lpoIni->GetString (_ACCOUNT_SEC_, "passwd", "", m_stAccountInfo.passwd,sizeof(m_stAccountInfo.passwd));
	if(strlen(m_stAccountInfo.passwd) != 0)
	{
		m_stAccountInfo.passwd_length = strlen(m_stAccountInfo.passwd);
	}

	
	delete lpoIni;
	return liRet;

}
int Query_test()
{
	int iRet = 0;
	int liLen =0, liCount = 0 ;
	char lszSQL[128] = {0};

	liLen = snprintf (lszSQL, sizeof(lszSQL), "select Pass9Account from Account");	

	iRet = m_clsMySQL.Query (lszSQL, liLen, &liCount);
	if ( iRet < 0 )
	{
		printf( "ERR|query fail.sql=[%s],err=[%s]\n", lszSQL, m_clsMySQL.GetErrMsg());
		return -1;
	}

	if (liCount == 0)
	{
		printf( "WARN|no conf record.sql=[%s]\n", lszSQL);
		m_clsMySQL.ReleaseRes ();
		return 0;
	}
	
	MYSQL_ROW lRow;
	for (int i=0; i < liCount; i++)
	{
		if ( (lRow = m_clsMySQL.FetchRecord ()) != NULL)
		{
			// get it.
			printf("%s\n",lRow[0]);
			//printf("%s\n",lRow[1]);
		}
		
	}	
	return 0;
}
// 生成密钥
static inline void auth_gen_key(char *psKey, const char * pszPasswd)
{
	char szTmp[16];

	Md5HashBuffer(szTmp, pszPasswd, strlen(pszPasswd));
	Md5HashBuffer(psKey, szTmp, sizeof(szTmp));
}

int Insert_blob()
{
	int iRet=0;
	char sql[256] = {0};//最终的sql语句
	unsigned short current_length = 0;
	char  head[1024];
	const char * tail= "\")";
	char Md5[16];
	char AccountLoginPasswd[17];
	char AccountLogin[51];
	int i = 0;
	int iTmp;
	
	for( i=0;i<m_stAccountInfo.account_total_num;i++ )
	{
		current_length = 0;
		memset(head,0,sizeof(head));
		memset(Md5,0,sizeof(Md5));
		memset(AccountLoginPasswd,0,sizeof(AccountLoginPasswd));
		memset(sql,0,sizeof(sql));
		memset(AccountLogin,0,sizeof(AccountLogin));

		snprintf(AccountLogin,sizeof(AccountLogin),"%s%d",
			m_stAccountInfo.account_prefix,
			m_stAccountInfo.account_start_num+i);
		
		snprintf(head,sizeof(head),"INSERT INTO Account (UinFlag,Pass9Account,BirthDay,PasswdHash) VALUES(%d,\"%s\",\"%s\",\"",
			m_stAccountInfo.account_gm_uinflag,
			AccountLogin,
			m_stAccountInfo.account_birthday
			);
		if(m_stAccountInfo.passwd_length >16)
		{
			m_stAccountInfo.passwd_length = 16;
		}

		if(strlen(m_stAccountInfo.passwd) != 0)
		{
			memcpy(AccountLoginPasswd,m_stAccountInfo.passwd,m_stAccountInfo.passwd_length);
		}
		else
		{
			rand_str(AccountLoginPasswd,m_stAccountInfo.passwd_length);
		}

		AccountLoginPasswd[m_stAccountInfo.passwd_length]='\0';
		
		auth_gen_key(Md5,AccountLoginPasswd);

		memcpy(sql, head, strlen(head));
		current_length += strlen(head); 

		iTmp = mysql_real_escape_string(&m_clsMySQL.m_MYSQL, sql+current_length, Md5, sizeof(Md5)); 
		current_length += iTmp;

		memcpy(sql+current_length, tail, strlen(tail)); 
		current_length += strlen(tail); 
		
		iRet = m_clsMySQL.Query (sql, current_length);
		if ( iRet < 0 )
		{
			//printf( "ERR|query fail.sql=[%s],err=[%s]\n", sql, m_clsMySQL.GetErrMsg());
			//return -1;
			fprintf(stderr, "ERR|query fail.sql=[%s],err=[%s]\n", sql, m_clsMySQL.GetErrMsg());
		}
		else
		{
			//printf("login: %s,passwd: %s\n",AccountLogin,AccountLoginPasswd);
			fprintf(stdout, "login: %s,passwd: %s\n",AccountLogin,AccountLoginPasswd);
		}
	}

	return 0;
}
//插入传入的帐号和密码
int Insert_account(char *Account,char* Passwd)
{
	int iRet=0;
	char sql[256] = {0};//最终的sql语句
	unsigned short current_length = 0;
	char  head[1024];
	const char * tail= "\")";
	char Md5[16];
	char AccountLoginPasswd[17];
	char AccountLogin[51];
	//int i = 0;
	int iTmp;
	
	current_length = 0;
	memset(head,0,sizeof(head));
	memset(Md5,0,sizeof(Md5));
	memset(AccountLoginPasswd,0,sizeof(AccountLoginPasswd));
	memset(sql,0,sizeof(sql));
	memset(AccountLogin,0,sizeof(AccountLogin));

	snprintf(AccountLogin,sizeof(AccountLogin),"%s",Account);
	
	snprintf(head,sizeof(head),"INSERT INTO Account (UinFlag,Pass9Account,BirthDay,PasswdHash) VALUES(%d,\"%s\",\"%s\",\"",
		m_stAccountInfo.account_gm_uinflag,
		AccountLogin,
		m_stAccountInfo.account_birthday
		);

	m_stAccountInfo.passwd_length = strlen(Passwd);
	if(m_stAccountInfo.passwd_length >16)
	{
		m_stAccountInfo.passwd_length = 16;
	}

	
	memcpy(AccountLoginPasswd,Passwd,m_stAccountInfo.passwd_length);
	
	AccountLoginPasswd[m_stAccountInfo.passwd_length]='\0';
		
	auth_gen_key(Md5,AccountLoginPasswd);

	memcpy(sql, head, strlen(head));
	current_length += strlen(head); 

	iTmp = mysql_real_escape_string(&m_clsMySQL.m_MYSQL, sql+current_length, Md5, sizeof(Md5)); 
	current_length += iTmp;

	memcpy(sql+current_length, tail, strlen(tail)); 
	current_length += strlen(tail); 
	
	iRet = m_clsMySQL.Query (sql, current_length);
	if ( iRet < 0 )
	{
		//printf( "ERR|query fail.sql=[%s],err=[%s]\n", sql, m_clsMySQL.GetErrMsg());
		//return -1;
		fprintf(stderr, "ERR|query fail.sql=[%s],err=[%s]\n", sql, m_clsMySQL.GetErrMsg());
	}
	else
	{
		//printf("login: %s,passwd: %s\n",AccountLogin,AccountLoginPasswd);
		fprintf(stdout, "login: %s,passwd: %s\n",AccountLogin,AccountLoginPasswd);
	}
	
	return 0;
}
int main(int argc , char **argv)
{
	int iRet = 0;

	srandom(time(NULL));    
	ReadIni("./account_tool.ini");
	   
    	iRet = m_clsMySQL.Connect ( m_stMySQL.sHost, m_stMySQL.sUser, m_stMySQL.sPasswd, m_stMySQL.sDB);
	if ( iRet < 0)
	{
	   	printf("ERR|connect mysql fail. err=%s\n", m_clsMySQL.GetErrMsg());
	    	return -1;
	}

	if(argc ==3)
	{
		Insert_account(argv[1],argv[2]);
	}
	else 
	{
		Insert_blob();
	}
	
	m_clsMySQL.ReleaseRes ();
	m_clsMySQL.Disconnect();
	return 0;
}
