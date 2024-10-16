#include "AutoTalkNpc.h"

AutoTalkNpc *AutoTalkNpc::m_instant = 0;

AutoTalkNpc::AutoTalkNpc()
{
	m_instant = this;
}

AutoTalkNpc * AutoTalkNpc::getInstant()
{
	return m_instant;
}
