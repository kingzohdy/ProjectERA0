#ifndef __RELATIONMANAGER_H__
#define __RELATIONMANAGER_H__

#include <map>
#include <string>
#include <vector>
#include "GameEngineTypes.h"
#include "OgreSingleton.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"

//tolua_begin
enum RelationSelecttype
{
	CHATROOMTYPE=1,//聊天室
	CLANTYPE=2,//公会
	FRIENDTYPE=3//好友
};
struct OfflinePlayer
{
	tdr_ulonglong	m_RoleID;                          	/*   角色id */
	char			m_RoleName[NAME_LEN];
	char			m_Emotion[EMOTION_LEN];
	int				m_uCareer;                         
	int				m_uLevel;                          
	int				m_uGender;
	int				m_Onlinetype;
	int				m_nhead;
	int				m_nface;
	int				m_nhair;
	int				m_Relationtype;
};

//tolua_end

bool operator== ( const OfflinePlayer& friendInfo, const tdr_ulonglong& roleID );
class RelationManager//tolua_export
	: public NetMsgHandler,public Ogre::Singleton<RelationManager>
{//tolua_export
public:
	RelationManager();
	~RelationManager();
	//tolua_begin
	OfflinePlayer*	getFriendInfo(int index);
	OfflinePlayer*	getFriendInfoByName(const char* szName);
	OfflinePlayer*	getClanMemberInfo(int index);

	void			requestAddFriend(const char* szName,const char* Reqinfo);
	void			requestDeleteFriend(tdr_ulonglong LongId);
	void			responseAddFriend(const char* szName,int isaccept,int Onlinetype);
	int				getFriendNum();
	int				getClanMemeberNum();
	const char*		getReqName();
	const char*		getReqInfo();
	int				getReqOnlinetype();
	bool			IsFriend(const char* szName);
	void			requestgetFriendinfo(const char* szName, int Onlinetype);

	//tolua_end
	void			OnResposeRelationSvr(const CSRELATIONSVR relation);//关系变化信息
	void			OnResposeOnlineChange( const CSONLINESVR& online );
	void            OnResponseClanSvr(  CSCLANSVR ClanSvr );
	void			updateRelation( const CSRELATIONCHG tRelationChg );
	void			dealOnlineplayer();
	void			updateClanMember( CLANMEM Members );
	void			setRoleAttr( OfflinePlayer &Roleinfo, int attrid,  CSRELATIONATTRDATA RelationAttrData);
	virtual int		HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	void			clear();
	void			setOnlinePlayer(std::string szName,int OnlineType);
	int				getOnlineinfo(std::string szName);
	std::map<std::string,int>		m_OnlinePlayer;
private:
	std::vector<OfflinePlayer>		m_FriendList;
	std::vector<OfflinePlayer>		m_ClanMemberList;
	std::string						m_reqname;//申请加好友的名字
	int								m_reqonlinetype;
	std::string						m_reqinfo;
	CLANID							m_ClanID; 
};//tolua_export
#endif