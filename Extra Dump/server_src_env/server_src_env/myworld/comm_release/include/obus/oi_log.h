
#ifndef _OI_LOG_H
#define _OI_LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	FILE	*pLogFile;
	char	sBaseFileName[80];
	char	sLogFileName[80];
	int	iShiftType;		// 0 -> shift by size,  1 -> shift by LogCount, 2 -> shift by interval, 3 ->shift by day, 4 -> shift by hour, 5 -> shift by min
	int	iMaxLogNum;
	long	lMaxSize;
	long	lMaxCount;
	long	lLogCount;
	time_t	lLastShiftTime;
} LogFile;

long InitLogFile(LogFile* pstLogFile, char* sLogBaseName, long iShiftType, long iMaxLogNum, long iMAX);
int Log(LogFile* pstLogFile, int iLogTime, char* sFormat, ...);
int Logv(LogFile* pstLogFile, char* sFormat, va_list ap);
int Logv2(LogFile* pstLogFile, int iLogTime, char* sFormat, va_list ap);

#ifdef __cplusplus
}
#endif

#endif
