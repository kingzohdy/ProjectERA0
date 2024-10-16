
#include "EditorMessageQue.h"

namespace TE
{
	EditorMsgQue g_EditorMsgQue;

	EditorMsgQue::EditorMsgQue()
	{
	}

	void EditorMsgQue::postMessage(int type, unsigned int param1, unsigned int param2)
	{
		EditorMsg msg;
		msg.type = type;
		msg.param[0] = param1;
		msg.param[1] = param2;

		m_MsgQue.push_back(msg);
	}

	bool EditorMsgQue::popMessage(EditorMsg &msg)
	{
		if(m_MsgQue.empty()) return false;

		msg = m_MsgQue.front();
		m_MsgQue.pop_front();

		return true;
	}
}