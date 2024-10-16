#ifndef __CHATROOMANAGER_H__
#define __CHATROOMANAGER_H__
#include "RelationManager.h"
#include <map>
#include <vector>
#include <string>
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#define MAX_ROOMS 10
class ChatRoomManager//tolua_export
	: public NetMsgHandler,public Ogre::Singleton<ChatRoomManager>
{//tolua_export
public:
	ChatRoomManager();
	~ChatRoomManager();
	virtual int		HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	void			initChatRoom(CSMUDENTERCHATROOMRES MudEnterChatRoomRes);
	void			PlayerEnterRoom(CSROOMROLEINFO t_ChatRoominfo);//玩家进入房间
	void			PlayerLeaveRoom(const char* szName);
	//tolua_begin
	void			requestLeaveRoom();
	void			requestEnterRoom(int index);
	OfflinePlayer*	getPlayerInfo(int index);
	int				getCurrentRoomrolenums();
	int				getCurrentChatRoomIndex();
	int				getChatRoomNums();

	int				getRoomPlayerNums(int i);
	void			clear();
	bool			getCanConnnect()
	{
		return		m_bIsConnnectChatroom;
	}
	//tolua_end
	void			update(unsigned int dtick);
	void			setCurrentChatroomindex(int i)
	{
		m_nCurrenRoomIndex = i;
	}
	bool				IsInChatRoom(std::string szName);

	void				PlayerEnterRoom( const CSMUDROLEDETAIL& roleDetail,const char* name );
private:
	int							m_nCurrenRoomIndex;
	int							m_nRoomNums;
	std::vector<OfflinePlayer>	m_ChatRoomPlayerList;
	int							m_nRoomPlayerNums[MAX_ROOMS];
	CSMUDCHATROOMLISTRES        m_ChatRoomList;
	int							m_nupdatetime;
	bool						m_bIsConnnectChatroom;//判断当前是否在连接聊天室用于当网络很繁忙的时候
};//tolua_export
#endif