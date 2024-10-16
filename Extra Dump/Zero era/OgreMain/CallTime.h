#pragma once
#include <windows.h>
#include <string>
#include <list>
#include <map>
#include "OgrePrerequisites.H"

using namespace std;

namespace Ogre{

	enum  ColEnum
	{
		eColEnum_Name,
		eColEnum_Counts,
		eColEnum_TotalTimes,
		eColEnum_Average,
		eColEnum_MinTimes,
		eColEnum_MaxTimes
		
	};

	typedef struct TimeNode
	{
		string				name;
		bool				show;
		LARGE_INTEGER		fBeginTime;
		LARGE_INTEGER		fEndTime;
		LARGE_INTEGER		fFrequency;
		double				CastTime;

		struct TimeNode* pParent;
		struct TimeNode* pChild;
		struct TimeNode* pPrev;
		struct TimeNode* pNext;
	}TIMENODE,*pTIMENODE;

	typedef struct CSVData
	{
		string			str_Name;
		int				i_Counts;
		double			d_TotalTime;
		double			d_AverageTime;
		double			d_MinTime;
		double			d_MaxTime;
		CSVData() : i_Counts(1), d_TotalTime(0), d_AverageTime(0), d_MinTime(0),d_MaxTime(0) { }
	}CSVDATA;

	class CallTime
	{
	public:
		void	BeginCalTime(const char* pszname,float functionTime,bool showTime);
		void	EndCalTime();
	};

	class _OgreExport TraceTime
	{	
	public:
		TraceTime(const char* pszname);
		~TraceTime();
		bool	IsFirstTime();
		bool	IsLastTime();
		bool	IsInMainThread();
		
		static bool			FrameEnd();
		static TimeNode*	GetRootNodes();
		static void			SetTraceTime(float frameTime,float minFunctionTime);//ʱ�䵥λ��Ϊ����
		static bool			BeginTraceTime(float frameTime);//�ж��Ƿ��ڹ涨��֡ʱ����
		static void			IsShowTime();//�ж��Ƿ�����С����ʱ����
		static void EndTraceTime();
		static void	SetMainThreadID(int mainThreadID);
		static bool			IsNodeListNull();
		static bool			SetCSVData();
		static void			WirteToCSV();
		static void			Shutdown();
	public:	
		CallTime* m_pcaltime;
	};
	static bool		 g_isWriteToCSV;
	static bool      g_bShow;
	static TimeNode* g_pCurrentRootNode;
	static int		 g_iMainThreadID;

	class _OgreExport LogTime
	{
	public:
		LogTime();
		~LogTime();

		void LogThis();

	private:
		LARGE_INTEGER frequency;
		std::list<LARGE_INTEGER> logs;
	};
}
//ò����Ϊ��������һ����vc�Ͳ������¹��졣Ϊ�˷���ͳ�ƼӶ༸��
#define TRACETIME(x) Ogre::TraceTime tt(##x)
#define TRACETIME1(x) Ogre::TraceTime tt1(##x)
#define TRACETIME2(x) Ogre::TraceTime tt2(##x)
#define TRACETIME3(x) Ogre::TraceTime tt3(##x)
#define TRACETIME4(x) Ogre::TraceTime tt4(##x)
#define TRACETIME5(x) Ogre::TraceTime tt5(##x)
#define TRACETIME6(x) Ogre::TraceTime tt6(##x)

#define STARTLOGTIME()	Ogre::LogTime lt
#define LOGTHISTIME()	lt.LogThis()
