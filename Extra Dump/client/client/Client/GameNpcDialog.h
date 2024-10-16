#pragma once
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "cs_net.h"
#include "AutoTalkNpc.h"
#include <vector>
#include <utility>

class GameNpcDialog//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<GameNpcDialog>, public AutoTalkNpc
{//tolua_export
public:
	GameNpcDialog(void);
	~GameNpcDialog(void);

protected:
	void		send_NpcTalk(int npcMemId, tagCSPkgBody msgbody);

public:
	//tolua_begin
	void			commitTaskItem(int list, int grid);
	void			commitTaskItem(int list1, int grid1, int list2, int grid2);
	void			commitTaskItem(int list1, int grid1, int list2, int grid2, int list3, int grid3);
	void			commitTaskItem(int list1, int grid1, int list2, int grid2, int list3, int grid3, int list4, int grid4);
	void			commitTaskItem(int list1, int grid1, int list2, int grid2, int list3, int grid3, int list4, int grid4, int list5, int grid5);
	void			commitTaskItem(int list1, int grid1, int list2, int grid2, int list3, int grid3, int list4, int grid4, int list5, int grid5, int list6, int grid6);
	void			commitTaskItem(int list1, int grid1, int list2, int grid2, int list3, int grid3, int list4, int grid4, int list5, int grid5, int list6, int grid6, int list7, int grid7);
	void			commitTaskItem(int list1, int grid1, int list2, int grid2, int list3, int grid3, int list4, int grid4, int list5, int grid5, int list6, int grid6, int list7, int grid7, int list8, int grid8);
	void			commitTaskItem(int list1, int grid1, int list2, int grid2, int list3, int grid3, int list4, int grid4, int list5, int grid5, int list6, int grid6, int list7, int grid7, int list8, int grid8, int list9, int grid9);
	void			commitTaskItem(int list1, int grid1, int list2, int grid2, int list3, int grid3, int list4, int grid4, int list5, int grid5, int list6, int grid6, int list7, int grid7, int list8, int grid8, int list9, int grid9, int list10, int grid10);
	void			selectOpt( int nSelectOpt, bool isLastOpt );
	void			send_CloseTalk();
	void			send_NextTalk();
	void			endTalk();
	void			endCurTalk();
	virtual int		talkByMemId( int npcMemId );
	virtual int		talkByResId( int npcResId );
	unsigned int	getMaxNpcTalkDist( GameActor* pNpc );
	unsigned int	getMaxNpcTalkDist( int nResId );
	int				getLastNpcResId();
	int				getTalkNpcMemID();
	//tolua_end	
	void			send_NpcTalk(int npcMemId);
	void			send_NpcTalk(int npcMemId, int nSelectOpt);
	void			send_NpcTalk(int npcMemId, const std::vector<std::pair<int, int>> &lstItem);
	virtual int		HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	virtual void	update(unsigned int dtick);
	void			clearNpcData();

private:
	int  m_nNpcResId;
	int  m_lastNpcId;
	int	 m_nNpcMemId;
	bool m_bCath;
	bool m_PushCamera;
};//tolua_export

inline int GameNpcDialog::getLastNpcResId()
{
	return m_lastNpcId;
}