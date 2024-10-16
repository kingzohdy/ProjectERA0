
#pragma once
#include <deque>

namespace TE
{
	enum
	{
		EDMSG_SELECTLIGHT = 0, //选中灯光模型
		EDMSG_UPDATELIGHTS, //灯光列表更新
		EDMSG_DELETELIGHT,
		EDMSG_SELECTNPC, //选中npc模型
		EDMSG_UPDATENPC, //npc列表更新
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