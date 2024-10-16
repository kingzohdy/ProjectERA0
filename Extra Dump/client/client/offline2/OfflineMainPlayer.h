#ifndef __OFFLINEMAINPALYER_H__
#define __OFFLINEMAINPLAYER_H__
#include <map>
#include <string>
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include <winsock2.h>
#define EXPLENGTH 110
//tolua_begin
enum OFFLINESTATE
{
	LOGIN_STATE=1,
	ROOMLIST_STATE=2,
	CHATROOM_STATE=3,
	ROLELIST_STATE=4,
};
//tolua_end
class OfflineMainPlayer//tolua_export
	: public NetMsgHandler,public Ogre::Singleton<OfflineMainPlayer>
{//tolua_export
public:
	OfflineMainPlayer();
	~OfflineMainPlayer();
	virtual int		HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	void			onRoleOffexpChangeRespose(tdr_ulonglong RoleID,int offexp);
	void			setEmotion(const char* szEmotion);
	void			EnterWorldFromZone();
	//tolua_begin
	const char*		getName();
	int				getLevel();
	int				getCareer();
	int				getGender();
	int				getOfflineExp();
	int				getMaxOfflineExp();
	const char*		getEmotion();
	int				getHead();
	int				getHair();
	int				getFace();
	tdr_longlong	getRoleId();
	int				getRoleListNums();
	const char*		getRoleName(int i);
	int				getRoleLevel(int i);
	int				getRoleCareer(int i);
	int				getOffExplength();
	
	void			setSelectRoleIndex(int i);
	int				getSelectRoleIndex();

	void			requestEnterGameMudWorld();
	void			requestRoomList();
	void			requestEmotion(const char* szEmotion);
	void			requestRoleLogout();
	void			requestStealexp(tdr_ulonglong RoleID,const char* szName);
	void			setGameState(int i);
	int				getGameState();
	bool			IsSamePlayer(int i);

	void			requestLeaveChatRoom();
	//tolua_end
	const CSMUDROLEDETAIL& getRoleDetail(){ return m_RoleDetail; }
private:
	CSROLELIST						m_RoleList;		//玩家角色列表
	CSMUDROLEDETAIL					m_RoleDetail;                      	/*  Ver.164  */
	int								m_OfflineExp;                      	/*   离线经验 */
	int								m_MaxOfflineExp;                   	/*   角色当前等级对应的最大离线经验 */
	char							m_eMotion[EMOTION_LEN];
	int								m_SelectRoleindex;
	int								m_GameState;
};//tolua_export
#endif