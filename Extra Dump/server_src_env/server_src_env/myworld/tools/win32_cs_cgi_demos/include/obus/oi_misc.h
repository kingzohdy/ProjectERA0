
#ifndef _OI_MISC_H
#define _OI_MISC_H

#include <stdio.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <stdarg.h>
#include <sys/param.h>
#include <sys/procfs.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int g_iTerminate ;

typedef  void (*SA_HANDLE)(int);

int WritePid(char* sPidFilePath);

void DaemonInit();

void DaemonInit1();

char *ShortDateTimeStr(time_t *mytime);

char *CompactDateTimeStr(time_t mytime);

char *CompactDateStr(time_t mytime);

char *DateTimeStr(time_t *mytime);

char *DayStr(time_t mytime);

char *CurrDayStr(void);

char *CurrDateTimeStr(void);

char *CurrShortDateTimeStr(void);

time_t GetUnixTime(char *sTimeStr);

char* MarkTime(int iFlag, char* sFormat, ...);

int GetPrpsinfo(pid_t pr_pid, prpsinfo_t* prpsinfo);

int WaitProcExit(int iPid);

int KillProcByPid(int iPid);

int KillPrevious(char* sPidFilePath);

int UinToTable(unsigned long lUin);

char* HashToIndexTable(char* sBaseTableName, unsigned char c);

int MsSleep(unsigned long lMs);

int FileCopy(char* sFile1, char* sFile2);

unsigned long GetPrimeNumber(unsigned long iNum);

int CheckCharacter ( char *pszStr );
int CheckCharacter2(char * pszStr) ;


int IPCmp(unsigned long* alIP1, unsigned long* alIP2);

int IPSCmp(unsigned long* lIP, unsigned long* alIP);

int InitAreaIP();

int GetAreaID(char* sIP);

int IsChinaIP(char* sIP);

int DailyLog(char* sPrefix, char* sFormat, ...);

void *my_bsearch (const void *key, const void *base, size_t nmemb, size_t size, int *piEqual, 
	 int (*compar) (const void *, const void *));

int MyBSearch (const void *key, const void *base, size_t nmemb, size_t size, int *piEqual, int (*compar) (const void *, const void *));

int MyBInsert (const void *key, const void *base, size_t *pnmemb, size_t size, int iUnique, int (*compar) (const void *, const void *));

int MyBDelete (const void *key, const void *base, size_t *pnmemb, size_t size, int (*compar) (const void *, const void *));

int MyIDelete (const void *base, size_t *pnmemb, size_t size, int index);

int CmpModifyTime(char *sFile, time_t *plMTime);

int IsSameDay(time_t t1, time_t t2);

void sigterm_handle( int isignal) ;
void MySigaction(  int sig ,SA_HANDLE sighandle) ; 

#ifdef __cplusplus
}
#endif

#endif

