#pragma once


#include "EditEventHandler.h"

#include <map>

namespace TE
{
//�༭�������¼��๤��
class MEventProcessFactory
{
public:
	MEventProcessFactory(void);
	~MEventProcessFactory(void);

	//�õ���Ӧ������;����( ���������� )
	IEventProcess*   GetEventProcess( int nTypeProcess );
    //�ͷ�����
	void    Destory();
private:
	//�¼�������ӦMAP 
	typedef std::map<int , IEventProcess*>   EVENT_PROCESS_MAP; 
	EVENT_PROCESS_MAP                        m_EventProcessMap;

};

};

extern TE::MEventProcessFactory g_EventProcessFactory;
