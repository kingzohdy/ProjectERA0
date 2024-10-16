#pragma once
#include "EditEventHandler.h"
#include <map>

namespace TE
{
	//�༭�������¼��๤��
	class EditHandlerManager
	{
	public:
		EditHandlerManager(void);
		~EditHandlerManager(void);

		//�õ���Ӧ������;����( ���������� )
		BaseEditHandler*   GetEventProcess( int nTypeProcess );
		//�ͷ�����
		void    Destory();
	private:
		//�¼�������ӦMAP 
		typedef std::map<int , BaseEditHandler*>   EVENT_PROCESS_MAP; 
		EVENT_PROCESS_MAP                        m_EventProcessMap;

	};

	extern EditHandlerManager g_EventProcessFactory;
}
