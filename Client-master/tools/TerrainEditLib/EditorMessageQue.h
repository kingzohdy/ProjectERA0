
#pragma once
#include <deque>

namespace TE
{
	enum
	{
		EDMSG_SELECTLIGHT = 0, //ѡ�еƹ�ģ��
		EDMSG_UPDATELIGHTS, //�ƹ��б����
		EDMSG_DELETELIGHT,
		EDMSG_SELECTNPC, //ѡ��npcģ��
		EDMSG_UPDATENPC, //npc�б����
		EDMSG_DELETENPC,
	};

	struct EditorMsg
	{
		int type;
		unsigned int param[2];
	};

	class EditorMsgQue
	{
	public:
		EditorMsgQue();
		
		void postMessage(int type, unsigned int param1=0, unsigned int param2=0);
		bool popMessage(EditorMsg &msg);

	private:
		std::deque<EditorMsg>m_MsgQue;
	};

	extern EditorMsgQue g_EditorMsgQue;
}