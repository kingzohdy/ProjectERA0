#pragma once
#include "EditEventHandler.h"
#include <map>

namespace TE
{
	//编辑器处理事件类工厂
	class EditHandlerManager
	{
	public:
		EditHandlerManager(void);
		~EditHandlerManager(void);

		//得到对应操作类;参数( 操作的类型 )
		BaseEditHandler*   GetEventProcess( int nTypeProcess );
		//释放数据
		void    Destory();
	private:
		//事件操作对应MAP 
		typedef std::map<int , BaseEditHandler*>   EVENT_PROCESS_MAP; 
		EVENT_PROCESS_MAP                        m_EventProcessMap;

	};

	extern EditHandlerManager g_EventProcessFactory;
}
