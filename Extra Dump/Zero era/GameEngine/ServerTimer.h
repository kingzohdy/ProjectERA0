
#ifndef __SERVERTIMER_H__
#define __SERVERTIMER_H__

#include "OgreSingleton.h"

class ServerTimer : public Ogre::Singleton<ServerTimer>
{
public:
	void beginBench();
	void updateServerBenchTime(int sec, int usec);
	void updateServerBenchTime(int ms);
	void getServerTime(int &sec, int &usec);

private:
	struct AbsTime
	{
		int sec;
		int usec;
	};

	AbsTime m_BenchClientTime;
	AbsTime m_BenchServerTime;
	unsigned int m_BenchTick;
};

//tolua_begin
struct ServerTime_ToLua
{
	unsigned int sec;
	unsigned int usec;
};
ServerTime_ToLua getServerTime();
bool isSameDay( unsigned int time );
int			getWeekDay();						//获取当前是星期几
//tolua_end
#endif