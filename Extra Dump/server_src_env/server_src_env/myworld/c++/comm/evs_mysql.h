/**
	@file	MySQL.hpp
*/

#ifndef	_MYSQL_HPP_
#define	_MYSQL_HPP_

namespace evslib
{

#include	<mysql.h>
#include	<stdlib.h>

typedef struct tagMySqlInfo{
	char sHost[100];
	char sUser[100];
	char sPasswd[100];
	char sDB[100];
	unsigned int nPort;
	char sTableName[100];
}MYSQLINFO;

typedef int (ProcessBatchRecord)(MYSQL_ROW lRow);//<, char *lsBuffer, int nBuffSize)

class clsMySQL
{

public:
	clsMySQL ();
	~clsMySQL ();

	int Connect (char *sDBHost, char *sDBUser, char *sDBPasswd, char *sDBName=NULL, unsigned int nPort=3306);

	int UseDB (char *sDBName);

	int Query (char *sSQL, int nSQLLen);

	int GetRecordNum();

	int IsEnd();
	MYSQL_ROW GetFirst();
	MYSQL_ROW GetNext();

	void GetErrMsg (char *sErrMsg, int nMsgSize);
	char * GetErrMsg ();

	int Disconnect();

	//<int FetchRecord (int *nCount);

	int Query (char *sSQL, int nSQLLen, int *nCount);
	MYSQL_ROW FetchRecord ();
	int ReleaseRes();

	MYSQL_ROW FetchLocateRecord (int *nCount, int nLocate=0);

	int FetchBatchRecord (int *nRealCount, int nCount, ProcessBatchRecord *pFun);

	void SetAutoCommit( int nAutoCommit=1/* 0/1 */);
	int GetAutoCommit () { return m_nAutoCommit; }

	char *GetDB() { return m_sDBName; }

	//< return 0 when valid .
	int IsValid ();

	//<
	int EscapeString (char *sTo, char *sFrom, int nFromLen);

private:
	void 	SetErrMsg(const char *sErrMsg=NULL);

protected:
	char m_sErrMsg[200];
public:
	MYSQL m_MYSQL;
private:	
	MYSQL_RES *m_pRes;

	int m_nAutoCommit; //< 0/1

	int m_bDestroy;//< 0/1.
	char	m_sDBName[50];

	//< auto reconncect.
	int m_nReConnect;//<0/1
	MYSQLINFO m_MySQLInfo;
};

}
#endif
