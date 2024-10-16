#include "RelationManager.h"
#include "MudNetMgr.h"
#include <algorithm>
#include "OfflineManager.h"
#include "OfflineMainPlayer.h"
#include "ChatRoomManager.h"
using namespace std;
bool operator== ( const OfflinePlayer& friendInfo, const tdr_ulonglong& roleID )
{
	return friendInfo.m_RoleID == roleID;
}
RelationManager::RelationManager()
{
	INetworkManager::getSingleton().RegisterMsgHandler( ONLINE_INFO, this );
	INetworkManager::getSingleton().RegisterMsgHandler( RELATION_SVR, this );
	INetworkManager::getSingleton().RegisterMsgHandler( CLAN_SVR, this );
	m_FriendList.clear();
	m_ClanMemberList.clear();
	m_OnlinePlayer.clear();
}

RelationManager::~RelationManager()
{

}

int RelationManager::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	switch (msgid)
	{
		case RELATION_SVR:
			{
				const CSRELATIONSVR& relation = msgbody.RelationSvr;
				OnResposeRelationSvr( relation );
			}
			break;
		case ONLINE_INFO:
			{
				OnResposeOnlineChange( msgbody.OnlineInfo );
			}
			break;
		case CLAN_SVR:
			{
				OnResponseClanSvr( msgbody.ClanSvr);
			}
			break;
	}
	return 0;
}

void RelationManager::dealOnlineplayer()
{
	for (vector<OfflinePlayer>::iterator fri_itr =m_FriendList.begin();fri_itr!=m_FriendList.end();fri_itr++ ) 
	if ( fri_itr->m_Onlinetype != 0 )
	{
		tagCSPkgBody msgbody;
		msgbody.RelationClt.OpType = RELATION_OP_TYPE_INFO_REQ;
		CSBUDDYINFOREQ&  addReq = msgbody.RelationClt.RelationCltData.BuddyInfoReq;
		strcpy( addReq.ReqName,fri_itr->m_RoleName );
		addReq.OnlineType = fri_itr->m_Onlinetype;
		OfflineSendMessage( RELATION_CLT, msgbody );
	}
}

void RelationManager::updateRelation( const CSRELATIONCHG tRelationChg )
{
	if (find( m_FriendList.begin(), m_FriendList.end(), tRelationChg.RoleID ) == m_FriendList.end() )
	{
		OfflinePlayer tOffPlayer;
		tOffPlayer.m_RoleID = tRelationChg.RoleID;
		m_FriendList.push_back(tOffPlayer);
	}
	vector<OfflinePlayer>::iterator fri_itr = find( m_FriendList.begin(), m_FriendList.end(), tRelationChg.RoleID );
	fri_itr->m_Relationtype = tRelationChg.RelationType;
	for ( int j = 0; j < tRelationChg.RelationAttrNum; j++ )
	{
		setRoleAttr( *fri_itr, tRelationChg.RelationAttrs[j].RelationAttrID, tRelationChg.RelationAttrs[j].RelationAttrData);
	}

}

void RelationManager::setRoleAttr( OfflinePlayer &Roleinfo, int attrid,  CSRELATIONATTRDATA RelationAttrData)
{
	switch (attrid)
	{
	case RELATION_ATTR_ONLINE:
		Roleinfo.m_Onlinetype = RelationAttrData.IsOnline;
		break;
	case RELATION_ATTR_NAME:
		strcpy( Roleinfo.m_RoleName, RelationAttrData.Name );
		break;
	case RELATION_ATTR_EMOTION:
		strcpy( Roleinfo.m_Emotion, RelationAttrData.Emotion );
		break;
	case RELATION_ATTR_CAREER:
		Roleinfo.m_uCareer = RelationAttrData.Career;
		break;
	case RELATION_ATTR_GENDER:
		Roleinfo.m_uGender = RelationAttrData.Gender;
		break;
	case RELATION_ATTR_LEVEL:
		Roleinfo.m_uLevel = RelationAttrData.Level;
		break;
	case  RELATION_ATTR_HEAD:
		Roleinfo.m_nhead = RelationAttrData.Head >> 16;
		Roleinfo.m_nhair = ( RelationAttrData.Head >> 8 ) & 0xff;
		Roleinfo.m_nface =  RelationAttrData.Head &0X000000FF;
		break;
	case  RELATION_ATTR_MEMNAME:
	case  RELATION_ATTR_BUDDYVAL:
		break; 
	}
}


void RelationManager::requestAddFriend(const char* szName,const char* Reqinfo)
{
	int statetype = getOnlineinfo( szName );
	if ( statetype == CLIENT_ONLINE || statetype == MUD_ONLINE)
	{
		tagCSPkgBody msgbody;
		msgbody.RelationClt.OpType = RELATION_OP_TYPE_ADDRELATION_REQ;
		CSADDRELATION& addReq = msgbody.RelationClt.RelationCltData.AddRelationReq;
		addReq.Type = RELATION_TYPE_FRIEND;
		ADDFRIENDREQ& addFriendInfo = addReq.AddRelationData.AddFriend;
		strncpy( addFriendInfo.Name, szName, strlen( szName ) + 1 );
		strncpy( addFriendInfo.ReqInfo, Reqinfo, strlen( Reqinfo ) + 1 );
		addFriendInfo.OnlineType = statetype;
		OfflineSendMessage( RELATION_CLT, msgbody );
	}
	else
	{
		MudNetMgr::getSingleton().addMessagestr("对方不在线!",0);
	}
}

void RelationManager::requestDeleteFriend(tdr_ulonglong LongId)
{
	tagCSPkgBody msgbody;
	msgbody.RelationClt.OpType = RELATION_OP_TYPE_DELRELATION_REQ;
	CSDELRELATIONREQ& addReq = msgbody.RelationClt.RelationCltData.DelRelationReq;
	addReq.Type = RELATION_TYPE_FRIEND;
	addReq.RoleID=LongId;
	OfflineSendMessage( RELATION_CLT, msgbody );
}


void RelationManager::responseAddFriend(const char* szName,int isaccept,int Onlinetype)
{
	tagCSPkgBody msgbody;
	msgbody.RelationClt.OpType = RELATION_OP_TYPE_ADDFRIEND_RES;
	CSADDFRIENDRES& addRes = msgbody.RelationClt.RelationCltData.AddFriendRes;
	addRes.Accept		=	isaccept;
	addRes.OnlineType	=	Onlinetype;
	addRes.AddOther		=	0;
	strncpy( addRes.ReqName, szName, strlen( szName ) + 1 );
	OfflineSendMessage( RELATION_CLT, msgbody );
}

void RelationManager::OnResposeRelationSvr(const CSRELATIONSVR relation)
{
	switch( relation.OpType )
	{
	case RELATION_OP_TYPE_CHGLIST:
		{
			for( int i = 0 ; i < relation.RelationSvrData.RelationChgList.Num ; i++ )
			{
				updateRelation( relation.RelationSvrData.RelationChgList.ChgList[i] );
			}
			dealOnlineplayer();
			OfflineManager::getSingleton().SendUIEvent("GE_RELATION_UPDATE");
		}
		break;
	case RELATION_OP_TYPE_CHG:
		{
			updateRelation( relation.RelationSvrData.RelationChg );
			OfflineManager::getSingleton().SendUIEvent("GE_RELATION_UPDATE");
		}
		break;
	
	case RELATION_OP_TYPE_ADDFRIEND_INFO:
		{
			m_reqname = relation.RelationSvrData.RelationAddFriendInfo.ReqName;
			m_reqonlinetype = relation.RelationSvrData.RelationAddFriendInfo.OnlineType;
			m_reqinfo = relation.RelationSvrData.RelationAddFriendInfo.ReqInfo;
			OfflineManager::getSingleton().SendUIEvent("GE_REQUEST_FRIEND");
		}
		break;
	case RELATION_OP_TYPE_DELRELATION_RES:
		{
			if (relation.RelationSvrData.RelationDelRes.Type==RELATION_TYPE_FRIEND)
			{
				vector<OfflinePlayer>::iterator it = find( m_FriendList.begin(), m_FriendList.end(), relation.RelationSvrData.RelationDelRes.RoleID );
				if ( it != m_FriendList.end() )
				{
					m_FriendList.erase( it );
					OfflineManager::getSingleton().SendUIEvent("GE_RELATION_UPDATE");
				}
			}		
		}
		break;
	case RELATION_OP_TYPE_EMOTION_RES:
		{
			OfflineMainPlayer::getSingleton().setEmotion(relation.RelationSvrData.EmotionRes.Emotion);
			OfflineManager::getSingleton().SendUIEvent("GE_ENMOTION_UPADATE");
		}
		break;
	}
}

const char* RelationManager::getReqName()
{
	return m_reqname.c_str();
}

int	RelationManager::getReqOnlinetype()
{
	return m_reqonlinetype;
}

const char* RelationManager::getReqInfo()
{
	return m_reqinfo.c_str();
}

void RelationManager::OnResposeOnlineChange( const CSONLINESVR& online )
{
	vector<OfflinePlayer>::iterator fri_temp = find( m_FriendList.begin(), m_FriendList.end(), online.RoleID );
	setOnlinePlayer( online.RoleName, online.Online);
	if (fri_temp != m_FriendList.end() )
	{
		string t_tips = online.RoleName;
		if ( online.Online == 0)
		{
			t_tips = t_tips+"下线了！";
		}
		else if ( online.Online == 1 )
		{
			t_tips = t_tips+"从客户端登录了！";
			requestgetFriendinfo(online.RoleName,online.Online);
		}
		else
		{
			t_tips = t_tips+"上线了！";
			requestgetFriendinfo(online.RoleName,online.Online);
		}
		fri_temp->m_Onlinetype = online.Online;
		MudNetMgr::getSingleton().m_tiplist.push_back( t_tips );
		OfflineManager::getSingleton().SendUIEvent("GE_RELATION_UPDATE");
	}
	if ( online.Online == 0 )
	{
		char pszStr[50];
		sprintf(pszStr,"DeleteChatObject( \"%s\")",online.RoleName);
		OfflineManager::getSingleton().CallString(pszStr);
		vector<OfflinePlayer>::iterator fri_temp = find( m_ClanMemberList.begin(),m_ClanMemberList.end(), online.RoleID );
		if (fri_temp != m_ClanMemberList.end() )
		{
			m_ClanMemberList.erase(fri_temp);
			string t_tips = online.RoleName;
			t_tips = "公会成员"+t_tips+"下线了！";
			MudNetMgr::getSingleton().m_tiplist.push_back( t_tips );
			OfflineManager::getSingleton().SendUIEvent("GE_RELATION_UPDATE");
		}
	}
}

OfflinePlayer*	RelationManager::getFriendInfo(int index)
{
	if ( m_FriendList.size() > 0 && index > 0  ) 
	{
		return &m_FriendList[index-1];
	}
	else
	{
		return NULL;
	}
}
int RelationManager::getFriendNum()
{
	return m_FriendList.size();
}

void RelationManager::clear()
{
	m_FriendList.clear();
	m_ClanMemberList.clear();
}

int	RelationManager::getClanMemeberNum()
{
	return m_ClanMemberList.size();	
}

OfflinePlayer* RelationManager::getClanMemberInfo(int index)
{
	if ( m_ClanMemberList.size() > 0 && index > 0 )
	{
		return &m_ClanMemberList[index - 1];
	}
	return NULL;
}

void RelationManager::updateClanMember( CLANMEM Members )
{
	if ( find( m_ClanMemberList.begin(), m_ClanMemberList.end(), Members.RoleMember.RoleID ) == m_ClanMemberList.end() )
	{
		OfflinePlayer t_Offplayer;
		t_Offplayer.m_RoleID = Members.RoleMember.RoleID;
		m_ClanMemberList.push_back( t_Offplayer );
	}
	vector<OfflinePlayer>::iterator Clan_itr = find( m_ClanMemberList.begin(), m_ClanMemberList.end(), Members.RoleMember.RoleID );
	Clan_itr->m_uCareer = Members.Career;
	strcpy(Clan_itr->m_RoleName, Members.RoleMember.RoleName);
	Clan_itr->m_uLevel = Members.Level;
	Clan_itr->m_Onlinetype = Members.Online;
}

void RelationManager::OnResponseClanSvr( CSCLANSVR ClanSvr )
{
	switch ( ClanSvr.Operation )
	{
	case CLAN_SVR_MEMBER_SNAPSHOT:
		{
			m_ClanID = ClanSvr.ClanSvrData.ClanMemberSnapshot.ID;
			for (int i = 0; i < ClanSvr.ClanSvrData.ClanMemberSnapshot.MemNum; i++)
			{
				updateClanMember( ClanSvr.ClanSvrData.ClanMemberSnapshot.Members[i] );
			}
		}
		break;
	case CLAN_SVR_MEMBER_CHG:
		{
			updateClanMember( ClanSvr.ClanSvrData.ClanMemberChg.MemInfo );
		}
		break;
	}
}

bool RelationManager::IsFriend(const char* szName)
{
	for (int i = 0; i < m_FriendList.size(); i++)
	{
		if ( strcmp( m_FriendList[i].m_RoleName, szName ) == 0 )
		{
			return true;
		}
	}
	return false;
}

void RelationManager::setOnlinePlayer(string szName,int OnlineType)
{
	if ( OnlineType == 1 || OnlineType == 2 )
	{
		map<string,int>::iterator olpliter=m_OnlinePlayer.find(szName);
		if ( olpliter != m_OnlinePlayer.end() )
		{
			olpliter->second = OnlineType;
		}
		else
		{
			m_OnlinePlayer[szName] = OnlineType;
		}
	}

	if ( OnlineType == 0 )
	{
		map<string,int>::iterator olpliter=m_OnlinePlayer.find(szName);
		if ( olpliter != m_OnlinePlayer.end() )
		{
			m_OnlinePlayer.erase( olpliter );
		}
	}
}

int RelationManager::getOnlineinfo(string szName)//通过名字获取玩家的在线状态
{
	map<string,int>::iterator olpliter = m_OnlinePlayer.find(szName);
	if ( olpliter != m_OnlinePlayer.end() )
	{
		return olpliter->second;
	}

	for ( vector<OfflinePlayer>::iterator iter = m_FriendList.begin(); iter != m_FriendList.end(); iter++)
	{
		if ( strcmp( iter->m_RoleName, szName.c_str() ) == 0 )
		{
			return iter->m_Onlinetype;
		}
	}

	for ( vector<OfflinePlayer>::iterator iter = m_ClanMemberList.begin(); iter != m_ClanMemberList.end(); iter++)
	{
		if ( strcmp( iter->m_RoleName, szName.c_str() ) == 0 )
		{
			return iter->m_Onlinetype;
		}
	}
	
	if ( ChatRoomManager::getSingleton().IsInChatRoom( szName ) || strcmp(szName.c_str(),OfflineMainPlayer::getSingleton().getName()) == 0 )
	{
		return MUD_ONLINE;
	}
	return -1;
}

OfflinePlayer* RelationManager::getFriendInfoByName(const char* szName)
{
	for ( vector<OfflinePlayer>::iterator iter = m_FriendList.begin(); iter != m_FriendList.end(); iter++)
	{
		if ( strcmp( iter->m_RoleName, szName ) == 0 )
		{
			return &(*iter);
		}
	}
	return NULL;
}

void RelationManager::requestgetFriendinfo(const char* szName, int Onlinetype)
{
	tagCSPkgBody msgbody;
	msgbody.RelationClt.OpType	= RELATION_OP_TYPE_INFO_REQ;
	CSBUDDYINFOREQ& requireInfo = msgbody.RelationClt.RelationCltData.BuddyInfoReq;
	strcpy( requireInfo.ReqName, szName);
	requireInfo.OnlineType = Onlinetype;
	OfflineSendMessage( RELATION_CLT, msgbody );
}