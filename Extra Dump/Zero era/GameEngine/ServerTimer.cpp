
#include "ServerTimer.h"
#include "GameMsgPrint.h"
#include <WinSock2.h>
#include <time.h>

extern "C"
{
	extern int gettimeofday(struct timeval *a_pstTv, struct timezone *a_pstTz);
}

ServerTime_ToLua getServerTime()
{
	int sec, usec;
	ServerTimer::getSingleton().getServerTime(sec, usec);
	ServerTime_ToLua time;
	time.sec = sec;
	time.usec = usec / 1000;
	return time;
}

bool isSameDay( unsigned int time )
{
	struct tm curr1;
	struct tm curr2;

	time_t t1 = getServerTime().sec;
	time_t t2 = time;

	//凌晨6点切换天,
	t1 += 2*3600;
	t2 += 2*3600;
	curr1 = *(gmtime(&t1));
	curr2 = *(gmtime(&t2));

	if( curr1.tm_year==curr2.tm_year && curr1.tm_yday==curr2.tm_yday )
	{
		return true;
	}else{
		return false;
	}
}
//获取当前是星期几
int getWeekDay()
{
	struct tm curr1;
	time_t t1 = getServerTime().sec;
	curr1 = *(gmtime(&t1));
	int weekDay = 0;
	if ( curr1.tm_wday == 0 )
	{
		return 7;
	}
	return curr1.tm_wday;
}

static ServerTimer s_STimer;

void ServerTimer::beginBench()
{
	timeval tv;
	gettimeofday(&tv, NULL);

	m_BenchClientTime.sec = tv.tv_sec;
	m_BenchClientTime.usec = tv.tv_usec;
	m_BenchTick = timeGetTime();
}

void ServerTimer::updateServerBenchTime(int sec, int usec)
{
	assert(usec >= 0);
	m_BenchServerTime.sec = sec;
	m_BenchServerTime.usec = usec;
	m_BenchTick = timeGetTime();
}

void ServerTimer::updateServerBenchTime(int ms)
{
	getServerTime(m_BenchServerTime.sec, m_BenchServerTime.usec);
	int usec = ms*1000 + m_BenchServerTime.usec;
	int sec = usec/1000000;
	usec -= sec*1000000;
	
	if(usec < 0)
	{
		sec--;
		usec += 1000000;
	}
	m_BenchServerTime.sec += sec;
	m_BenchServerTime.usec = usec;
	m_BenchTick = timeGetTime();
}

void ServerTimer::getServerTime(int &sec, int &usec)
{
	unsigned int curtick = timeGetTime();
	assert(curtick >= m_BenchTick);
	unsigned int dtick = (curtick - m_BenchTick);

	usec = m_BenchServerTime.usec + (dtick%1000)*1000;
	sec = m_BenchServerTime.sec + (dtick/1000) + (usec/1000000);
	usec = (usec%1000000);
}
