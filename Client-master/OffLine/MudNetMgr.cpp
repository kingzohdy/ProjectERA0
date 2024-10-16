#include "stdafx.h"
#include "MudNetMgr.h"
#include "cs_net.h"
#include "tinyxml.h"
#include "OgreXMLData.h"
#include "ServerTimer.h"
#include <MMSystem.h>

using namespace std;
using namespace Ogre;

//TODO
#define DIRCONFIG_PATH		"UserInfo/DirSvr.xml"
#define CONNECT_SERTIME		6000

const int DIR_DISCONNECT_FLAG	= -1;
const int GAME_DISCONNECT_FLAG	= -1;



const char *DirBigWorldInfo::getName()
{
	return m_Name;
}

int DirBigWorldInfo::getNumWorld()
{
	return m_nWorldNum;
}

DirWorldInfo &DirBigWorldInfo::getIthWorld(int i)
{
	return m_Worlds[i];
}

void DirBigWorldInfo::addWorldInfo(const CSDIRWORLDINFO &src)
{
	DirWorldInfo& info = m_Worlds[m_nWorldNum++];
	strcpy_s(info.name, src.WorldName);
	char *ipstr		= inet_ntoa(*(struct in_addr *)&src.IP);
	strcpy_s(info.ip, ipstr);
	info.recommend	= (src.MudStatFlag>>4)!=0;	
	info.busy		= (src.MudStatFlag&0xf);
	info.port		= NTOHS(src.Port);
	info.worldid	= src.WorldID;
	info.RegionID	= src.RegionID;
	ipstr			= inet_ntoa(*(struct in_addr *)&src.AuthIP);
	strcpy_s(info.AuthIP, ipstr);
	info.AuthPort	= NTOHS(src.AuthPort);
	ipstr			= inet_ntoa(*(struct in_addr *)&src.MudIP);
	strcpy_s(info.mudIp,ipstr);
	info.mudPort	= NTOHS(src.MudPort);
}

void DirBigWorldInfo::addWorldInfo( const DirWorldInfo& src )
{
	m_Worlds[m_nWorldNum++] = src;
}

void DirBigWorldInfo::updateWorldInfo( const CSDIRLATESTINFO& src )
{
	for( int i = 0; i < m_nWorldNum; i++ )
	{
		if( m_Worlds[i].worldid == src.WorldID )
		{
			m_Worlds[i].recommend	= (src.MudStatFlag>>4)!=0;	
			m_Worlds[i].busy		= (src.MudStatFlag&0xf);
			char *ipstr				= inet_ntoa(*(struct in_addr *)&src.IP);
			strcpy_s(m_Worlds[i].ip, ipstr);
			m_Worlds[i].port		= NTOHS(src.Port);
			m_Worlds[i].RegionID	= src.RegionID;
			ipstr					= inet_ntoa(*(struct in_addr *)&src.AuthIP);
			strcpy_s(m_Worlds[i].AuthIP, ipstr);
			m_Worlds[i].AuthPort	= NTOHS(src.AuthPort);
			ipstr					= inet_ntoa(*(struct in_addr *)&src.MudIP);
			strcpy_s(m_Worlds[i].mudIp,ipstr);
			m_Worlds[i].mudPort		= NTOHS(src.MudPort);
			return;
		}
	}
}

std::string DirBigWorldInfo::getUrl()
{
	return m_strUrl;
}

MudNetMgr::MudNetMgr(void)
{
	m_nBigWorldNum = 0;
	m_hThread = NULL;
	m_DirNetCL = -1;
	m_GameNetCL = -1;
	m_loginRoleID = -1;
	m_UserName[0] = '\0';

	m_attrNameMap[ATTR_ID_LILIANG] = "力量\b";
	m_attrNameMap[ATTR_ID_ZHILI] = "智力\b";
	m_attrNameMap[ATTR_ID_TILI] = "体质\b";
	m_attrNameMap[ATTR_ID_JINGSHEN] = "精神\b";
	m_attrNameMap[ATTR_ID_FANGYU] = "防护\b";
	//m_attrNameMap[ATTR_ID_SHANBI] = "闪躲\b";
	m_attrNameMap[ATTR_ID_MINGZHONG] = "精准\b";
	m_attrNameMap[ATTR_ID_PHSIC_ATK] = "物理攻击:\b";
	m_attrNameMap[ATTR_ID_ICE_ATK] = "魔法攻击:\b";
	m_attrNameMap[ATTR_ID_PHSIC_DEF] = "物理防御:\b";
	m_attrNameMap[ATTR_ID_ICE_DEF] = "魔法防御:\b";
	m_attrNameMap[ATTR_ID_HIT] = "命中:\b";
	m_attrNameMap[ATTR_ID_FLEE] = "闪避:\b";
	m_attrNameMap[ATTR_ID_DAMAGE] = "攻击强度:\b";
	m_attrNameMap[ATTR_ID_HEAVY] = "暴击率:\b";
	m_attrNameMap[ATTR_ID_DEF_PHSIC_HURT] = "物理减伤:\b";
	m_attrNameMap[ATTR_ID_DEF_ICE_HURT] = "物理减伤:\b";
	m_attrNameMap[ATTR_ID_DEF_PHSIC_HURT] = "魔法减伤:\b";
	m_attrNameMap[ATTR_ID_HPADD] = "物理减伤:\b";
	m_attrNameMap[ATTR_ID_DEF_PHSIC_HURT] = "HP每秒回复:\b";
	m_attrNameMap[ATTR_ID_MPADD] = "MP每秒回复:\b";

	InitializeCriticalSection(&m_CriticalSection);
}

MudNetMgr::~MudNetMgr(void)
{
	TerminateThread(m_hThread,0);
	DeleteCriticalSection(&m_CriticalSection);
}

MudNetMgr*	MudNetMgr::Create(XMLData* pData)
{
	MudNetMgr* pReturn = new MudNetMgr;
	LoadGameConfigFromXML(pReturn->m_config,pData);
	pReturn->initNetMgr();

	for( int i = 0 ; i < MAX_CHAT_ROOM_PLAYER ; i ++ )
	{
		pReturn->m_OnlineInfo[i].RoleID = -1;
	}
	return pReturn;
}

DWORD WINAPI workFunction(LPVOID lpvoid)
{
	while(1)
	{
		MudNetMgr* netMgr = (MudNetMgr*)lpvoid;
	
		if( netMgr != NULL  )
		{
			AutoLock( &netMgr->m_CriticalSection );
			netMgr->Dispatch();
		}
		Sleep(1);
	}
	return 0;
}
void MudNetMgr::Dispatch()
{
	if( m_pNetMgr != NULL )
	{
		m_pNetMgr->Dispatch();
	}
}

void MudNetMgr::startDispatch()
{
	DWORD dwThreadID;
	m_hThread = CreateThread(NULL,0,workFunction,this,0,&dwThreadID);
	if( m_hThread == NULL )
	{
		//TODO HERE
	}
}

void MudNetMgr::initNetMgr()
{
	m_pNetMgr = INetworkManager::Create();
	//注册消息
// 	m_pNetMgr->RegisterMsgHandler( DIR_RES,this );
// 	m_pNetMgr->RegisterMsgHandler( ACC_LOGIN_RES,this );
// 	m_pNetMgr->RegisterMsgHandler( ROLE_LIST,this );
// 	m_pNetMgr->RegisterMsgHandler( MUD_ROLE_LOGIN_RES,this);
// 	m_pNetMgr->RegisterMsgHandler(ONLINE_INFO,this);
// 	m_pNetMgr->RegisterMsgHandler(RELATION_SVR,this);
// 	m_pNetMgr->RegisterMsgHandler(CHAT_RES,this);
// 	m_pNetMgr->RegisterMsgHandler(ROLE_ENTER_CHATROOM,this);
// 	m_pNetMgr->RegisterMsgHandler(ROLE_LEAVE_CHATROOM,this);
// 	m_pNetMgr->RegisterMsgHandler(KICKOFF_INFO,this);
}

int MudNetMgr::HandleNetMsg( int msgid, const tagCSPkgBody& msgbody )
{
// 	switch( msgid )
// 	{
// 	case DIR_RES:
// 		{
// 			const CSDIRRES& res = msgbody.DirRes;
// 			onDirInfoResponse(res);
// 		}
// 		break;
// 	case ACC_LOGIN_RES:
// 		{
// 			if( msgbody.AccLoginRes.Result != 0 )
// 			{
// 				m_GameNetCL = GAME_DISCONNECT_FLAG;
// 				notifyGui(eLoginRet,msgbody.AccLoginRes.Result);		//登陆失败
// 				return -1;
// 			}
// 			notifyGui(eLoginRet,0);		//登陆成功
// 		}
// 		break;
// 	case ROLE_LIST:
// 		{
// 			m_RoleList = msgbody.RoleList;
// 			notifyGui(eRoleList,0);
// 		}
// 		break;
// 	case MUD_ROLE_LOGIN_RES:
// 		{
// 			m_loginRoleID = msgbody.MudRoleLoginRes.RoleDetail.RoleID;
// 			m_RoleDetail = msgbody.MudRoleLoginRes.RoleDetail;
// 			m_ChatRoomInfo = msgbody.MudRoleLoginRes.ChatRoomInfo;
// 			notifyGui(eRoleLogin,0);
// 		}
// 		break;
// 	case ONLINE_INFO:
// 		{
// 			const CSONLINESVR& online = msgbody.OnlineInfo;
// 			onOnlineResponse(online);
// 		}
// 		break;
// 	case RELATION_SVR:
// 		{
// 			const CSRELATIONSVR& relation = msgbody.RelationSvr;
// 			onRelationResponse( relation );
// 		}
// 		break;
// 	case CHAT_RES:
// 		{
// 			const CSCHATRES& chat = msgbody.ChatRes;
// 			onChatResponse(chat);
// 		}
// 		break;
// 	case ROLE_ENTER_CHATROOM:
// 		{
// 			char* playerName = NULL;
// 			m_ChatRoomInfo.ChatRoomRoleInfo[m_ChatRoomInfo.PlayerNum] = msgbody.EnterChatRoom.RoleInfo;
// 			m_ChatRoomInfo.PlayerNum ++;
// 			playerName = new char[NAME_LEN];
// 			strcpy_s(playerName,NAME_LEN,msgbody.EnterChatRoom.RoleInfo.RoleName);
// 			notifyGui(eEnterRoom,(LPARAM)playerName);
// 			break;
// 		}
// 		break;
// 	case ROLE_LEAVE_CHATROOM:
// 		{
// 			char* playerName = NULL;
// 			for( int i = 0 ; i < m_ChatRoomInfo.PlayerNum;i++ )
// 			{
// 				if( msgbody.LeaveChatRoom.RoleID == m_ChatRoomInfo.ChatRoomRoleInfo[i].RoleID )
// 				{
// 					m_ChatRoomInfo.ChatRoomRoleInfo[m_ChatRoomInfo.PlayerNum-1] = m_ChatRoomInfo.ChatRoomRoleInfo[i];
// 					m_ChatRoomInfo.PlayerNum --;
// 					playerName = new char[NAME_LEN];
// 					strcpy_s( playerName,NAME_LEN,msgbody.LeaveChatRoom.RoleName );
// 					break;
// 				}				
// 			}
// 			if( playerName != NULL )
// 			{
// 				notifyGui(eLeaveRoom,(LPARAM)playerName);
// 			}
// 		}
// 		break;
// 	case KICKOFF_INFO:
// 		{
// 			notifyGui(eError,eErrorKickOff);
// 			m_DirNetCL  = DIR_DISCONNECT_FLAG;
// 			m_GameNetCL = GAME_DISCONNECT_FLAG;
// 
// 			return -1;
// 
// 		}
// 		break;
// 	default:
// 		break;
// 	}
	return 0;
}

bool MudNetMgr::LoadLocalSvr()
{
	XMLData xmldata;

	if( !xmldata.loadFile( DIRCONFIG_PATH ) )
	{
		return false;
	}
	
	m_nBigWorldNum = 0;

	XMLNode pRootNode = xmldata.getRootNode();

	XMLNode pChildNode = pRootNode.iterateChild();
	int i = 0;
	while( !pChildNode.isNull() && i < MAX_BIGWORLD_NUM )
	{
		if( stricmp( pChildNode.getName(), "Version" ) == 0 )
		{
			m_nVersion = pChildNode.attribToInt("version");
		}

		if( stricmp( pChildNode.getName(), "BigWorld" ) == 0 )
		{
			strncpy_s( m_BigWorlds[i].m_Name, pChildNode.attribToString("name"), 64 );
			// TODO:
			if ( pChildNode.attribToString( "url" ) == NULL )
			{
				continue;
			}

			m_BigWorlds[i].m_strUrl = pChildNode.attribToString( "url" );

			XMLNode pBigWorldChild = pChildNode.iterateChild();
			int num = pChildNode.attribToInt("count");
			m_BigWorlds[i].m_nWorldNum = num;
			for ( int j = 0; j < num; j++ )
			{
				if ( stricmp( pBigWorldChild.getName(), "DirWorld" ) == 0 )
				{
					DirWorldInfo& worldInfo = m_BigWorlds[i].m_Worlds[j];
					worldInfo.busy		= pBigWorldChild.attribToInt("busy");
					worldInfo.port		= pBigWorldChild.attribToInt("port");
					worldInfo.recommend	= pBigWorldChild.attribToInt("recommend")==0?false:true;
					worldInfo.worldid	= pBigWorldChild.attribToInt("worldid");
					strncpy_s( worldInfo.name, pBigWorldChild.attribToString("name"), WORLD_NAME_LEN );
					strncpy_s( worldInfo.ip, pBigWorldChild.attribToString("ip"), 32 );
				}
				pBigWorldChild = pChildNode.iterateChild(pBigWorldChild);
			}
			i++;
		}
		pChildNode = pRootNode.iterateChild(pChildNode);
	}
	m_nBigWorldNum = i;
	return true;
}

void MudNetMgr::SaveLocalSvr()
{
	TiXmlDocument doc;
	std::list< TiXmlElement* > ListElemnt;

	TiXmlDeclaration *decl = new TiXmlDeclaration( "1.0", "utf-8", "yes" );
	doc.LinkEndChild( decl );

	TiXmlElement* root = new TiXmlElement( "LoclDirSvr" );
	doc.LinkEndChild( root );
	ListElemnt.push_back( root );

	TiXmlElement* pVersionElement = new TiXmlElement( "Version" );
	root->LinkEndChild( pVersionElement );
	pVersionElement->SetAttribute( "version", m_nVersion );
	ListElemnt.push_back( pVersionElement );

	for( int i = 0; i < m_nBigWorldNum; i++ )
	{
		TiXmlElement* pBigWorldElement = new TiXmlElement( "BigWorld");
		root->LinkEndChild(pBigWorldElement);
		pBigWorldElement->SetAttribute( "name",		m_BigWorlds[i].getName() );
		pBigWorldElement->SetAttribute( "count",	m_BigWorlds[i].getNumWorld() );
		pBigWorldElement->SetAttribute( "url",		m_BigWorlds[i].getUrl().c_str() );
		ListElemnt.push_back( pBigWorldElement );

		for( int j = 0; j < m_BigWorlds[i].getNumWorld(); j++ )
		{
			DirWorldInfo& DirWorld = m_BigWorlds[i].getIthWorld( j );
			TiXmlElement* pDirWorldElement = new TiXmlElement( "DirWorld");
			pBigWorldElement->LinkEndChild(pDirWorldElement);

			pDirWorldElement->SetAttribute( "name",			string( DirWorld.name ).c_str() );
			pDirWorldElement->SetAttribute( "recommend",	DirWorld.recommend );
			pDirWorldElement->SetAttribute( "busy",			DirWorld.busy );
			pDirWorldElement->SetAttribute( "ip",			string( DirWorld.ip ).c_str() );
			pDirWorldElement->SetAttribute( "port",			DirWorld.port );
			pDirWorldElement->SetAttribute( "worldid",		DirWorld.worldid );
			ListElemnt.push_back( pDirWorldElement );
		}
	}
	doc.SaveFile( DIRCONFIG_PATH );
}

void MudNetMgr::requestDirInfo()
{
// 	AutoLock lock(&m_CriticalSection);
// 	const char *ip;
// 	unsigned short port;
// 	hostent* remoteHost;
// 	remoteHost = gethostbyname(m_config.dir_server);
// 	if(remoteHost != NULL )
// 	{
// 		ip = inet_ntoa(*(struct in_addr *)*remoteHost->h_addr_list);
// 		if( m_config.dir_port != 0 )
// 		{
// 			port =m_config.dir_port;
// 		}
// 		if( m_DirNetCL < 0 )
// 		{
// 			m_pNetMgr->Release();
// 			m_pNetMgr->ReleaseDirServer();
// 			m_DirNetCL = m_pNetMgr->ConnectDirServer( ip, port );
// 			if(m_DirNetCL < 0) 
// 			{
// 				notifyGui(eError,eErrorGetDirError);
// 				return;
// 			}
// 		}
// 	}else if(m_config.dir_num != 0){		
// 		int idx = rand()%m_config.dir_num;
// 		for( int i = idx;i<m_config.dir_num;i++ )
// 		{
// 			ip = m_config.dir_iplist[i].dir_ip;
// 			port = m_config.dir_iplist[i].dir_port;
// 			if( m_DirNetCL < 0 )
// 			{
// 				m_pNetMgr->Release();
// 				m_DirNetCL = m_pNetMgr->ConnectDirServer( ip, port );
// 				if(m_DirNetCL > 0) 
// 				{
// 					break;
// 				}
// 			}
// 		}
// 		if( m_DirNetCL < 0 )
// 		{
// 			notifyGui(eError,eErrorGetDirError);
// 			return;
// 		}
// 	}	
// 	// 获取本地目录信息
// 	if( !LoadLocalSvr() )
// 	{
// 		m_nVersion = 0;
// 	}
// 	tagCSPkgBody body;
// 	body.DirReq.LastVersion = m_nVersion;
// 	SendDirNetMessage( DIR_REQ, body );
// 	m_DirNetCL = DIR_DISCONNECT_FLAG;
}

void MudNetMgr::initDirInfo(const CSDIRINFO& dirinfo)
{
	m_nBigWorldNum = dirinfo.WorldList.BigWorldNum;
	m_nVersion = dirinfo.WorldList.LastVersion;
	for(int i=0; i<m_nBigWorldNum; i++)
	{
		DirBigWorldInfo& biginfo = m_BigWorlds[i] ;
		strcpy_s(biginfo.m_Name, dirinfo.WorldList.BigWorldList[i].BigWorldName);
		biginfo.m_nWorldNum = 0;
		for(int j=0; j<dirinfo.WorldList.WorldNum; j++)
		{
			const CSDIRWORLDINFO &src = dirinfo.WorldList.ListInfoList[j];
			if(src.BigWorldIdx == i) biginfo.addWorldInfo(src);
		}
	}
	SaveLocalSvr();
}

void MudNetMgr::onDirInfoResponse(const CSDIRRES& res)
{
	if( res.Result == DIR_SUCC )
	{
		initDirInfo( res.DirInfo );
		notifyGui(eUpdateDirList,0);
	}else if( res.Result == DIR_LATEST )
	{
		updateDirInfo(res.DirInfo.DirLatest);
		notifyGui(eUpdateDirList,0);
	}else if( res.Result == DIR_ERR )
	{	
		notifyGui(eError,eErrorGetDirError);
	}
}

void MudNetMgr::updateDirInfo(const CSDIRLATEST& latestinfo)
{
	for( int i = 0; i < latestinfo.WorldNum; i++ )
	{
		for( int j = 0; j < m_nBigWorldNum; j++ )
		{
			m_BigWorlds[j].updateWorldInfo( latestinfo.LatestList[i] );
		}
	}
	SaveLocalSvr();
}

void MudNetMgr::onRelationResponse( const CSRELATIONSVR& relation )
{
	switch( relation.OpType )
	{
	case RELATION_OP_TYPE_CHGLIST:
		{
			for( int i = 0 ; i < relation.RelationSvrData.RelationChgList.Num ; i++ )
			{
				bool bFound = false;
				if( relation.RelationSvrData.RelationChgList.ChgList[i].RelationType == 1 )	//好友
				{
					for( size_t idx = 0 ; idx < m_vFriends.size(); idx++ )
					{
						if( relation.RelationSvrData.RelationChgList.ChgList[i].RoleID == m_vFriends[idx].RoleID )
						{
							for( int j = 0 ; j < relation.RelationSvrData.RelationChgList.ChgList[i].RelationAttrNum ; j++ )
							{
								if( relation.RelationSvrData.RelationChgList.ChgList[i].RelationAttrs[j].RelationAttrID == RELATION_ATTR_ONLINE )
								{
									m_vFriends[idx].iOnline = relation.RelationSvrData.RelationChgList.ChgList[i].RelationAttrs[j].RelationAttrData.IsOnline;
									break;
								}
							}
							bFound = true;
							break;
						}
					}
					if( !bFound )
					{
						FriendInfo info;
						info.RoleID = relation.RelationSvrData.RelationChgList.ChgList[i].RoleID;
						for( int j = 0 ; j < relation.RelationSvrData.RelationChgList.ChgList[i].RelationAttrNum ; j++ )
						{
							if( relation.RelationSvrData.RelationChgList.ChgList[i].RelationAttrs[j].RelationAttrID == RELATION_ATTR_ONLINE )
							{
								info.iOnline = relation.RelationSvrData.RelationChgList.ChgList[i].RelationAttrs[j].RelationAttrData.IsOnline;
							}						
							if( relation.RelationSvrData.RelationChgList.ChgList[i].RelationAttrs[j].RelationAttrID == RELATION_ATTR_NAME )
							{
								strcpy_s( info.roleName,
									relation.RelationSvrData.RelationChgList.ChgList[i].RelationAttrs[j].RelationAttrData.Name );
							}
							if( relation.RelationSvrData.RelationChgList.ChgList[i].RelationAttrs[j].RelationAttrID == RELATION_ATTR_EMOTION )
							{
								strcpy_s( info.Emotion,
									relation.RelationSvrData.RelationChgList.ChgList[i].RelationAttrs[j].RelationAttrData.Emotion );
							}
						}
						m_vFriends.push_back( info );
					}
				}
			}
			notifyGui(eRelation,0);
		}
		break;
	case RELATION_OP_TYPE_CHG:
		{
			vector<FriendInfo>::iterator it = m_vFriends.begin();
			for( ; it != m_vFriends.end(); ++it )
			{
				if( it->RoleID == relation.RelationSvrData.RelationChg.RoleID )
				{
					if( relation.RelationSvrData.RelationChg.RelationType != 1 )
					{
						m_vFriends.erase(it);
					}else{
						for( int i = 0; i < relation.RelationSvrData.RelationChg.RelationAttrNum ; i++ )
						{
							it->iOnline = relation.RelationSvrData.RelationChg.RelationAttrs[i].RelationAttrData.IsOnline;
							strcpy_s(it->roleName,
								relation.RelationSvrData.RelationChg.RelationAttrs[i].RelationAttrData.Name);
							strcpy_s(it->Emotion,
								relation.RelationSvrData.RelationChg.RelationAttrs[i].RelationAttrData.Emotion);
						}
					}
					break;
				}
			}
			notifyGui(eRelation,0);
		}
		break;
	}
}

void MudNetMgr::onOnlineResponse( const CSONLINESVR& online )
{
	bool bFound = false;
	if( online.Online == 0 )
	{
		for( size_t i = 0 ; i < m_vFriends.size() ; i++ )
		{
			if( m_vFriends[i].RoleID == online.RoleID )
			{
				bFound = true;
				m_vFriends[i].iOnline = 0;
				break;
			}
		}
	}else if( online.Online == 2 )
	{
		for( size_t i = 0 ; i < m_vFriends.size() ; i++ )
		{
			if( m_vFriends[i].RoleID == online.RoleID )
			{
				bFound = true;
				m_vFriends[i].iOnline = 2;
				break;
			}
		}

	}else if( online.Online == 1 )
	{
		for( size_t i = 0 ; i < m_vFriends.size() ; i++ )
		{
			if( m_vFriends[i].RoleID == online.RoleID )
			{
				bFound = true;
				m_vFriends[i].iOnline = 1;
				break;
			}
		}
	}
	//通知界面
	if( bFound )
	{
		for( int i = 0 ; i < 200 ; i ++ )
		{
			if( m_OnlineInfo[i].RoleID == -1 )
			{
				m_OnlineInfo[i] = online;
				notifyGui(eOnline,i);
				break;
			}
		}
	}
}

void MudNetMgr::onChatResponse(const CSCHATRES& chat)
{
	CSCHATRES* pRes = new CSCHATRES;
	*pRes = chat;
	//m_vChatRes.push_back(chat);
	notifyGui(eChatMsg,(LPARAM) pRes );
}

CSONLINESVR MudNetMgr::getOnlineInfo(int nIndex)
{
	assert( nIndex < MAX_CHAT_ROOM_PLAYER );
	CSONLINESVR info = m_OnlineInfo[nIndex];
	m_OnlineInfo[nIndex].RoleID = -1;
	return info;
}

void MudNetMgr::requestSendChat(const char* toName,const char* chatMsg,bool bIsFriend)
{
	tagCSPkgBody msgbody;
	strcpy_s(msgbody.ChatReq.Content,chatMsg);
	strcpy_s(msgbody.ChatReq.ToName, toName);
	if( strlen(toName) > 0 )
	{
		if(bIsFriend )
		{
			msgbody.ChatReq.Type = CHAT_FRIEND;
		}else{
			msgbody.ChatReq.Type = CHAT_PRIVATE;
		}
	}else{
		msgbody.ChatReq.Type = CHAT_SAY;
	}
	msgbody.ChatReq.LinkLen = 0;
	SendGameServerMsg(CHAT_REQ,msgbody);
}

const char* MudNetMgr::getAttrName(int attrID)
{
	map<int,string>::iterator it = m_attrNameMap.find(attrID);
	if( it != m_attrNameMap.end() )
	{
		return it->second.c_str();
	}
	return "";
}

int MudNetMgr::getFriendNum()
{
	return (int)m_vFriends.size();
}

FriendInfo& MudNetMgr::getFriendInfo(int nIndex)
{
	return m_vFriends[nIndex];
}

//////////////////////////////////////////////////////////////////////////
bool MudNetMgr::requestLoginGameServer( const char *ip, unsigned short port, const char *username, 
								   const char *passwd, int worldId, const char* worldName)
{
	AutoLock lock(&m_CriticalSection);
	// 同上次链接时间间隔需超过6秒
	static int nStartTime = 0;
	if( timeGetTime() - nStartTime < CONNECT_SERTIME )
	{
		notifyGui(eError,eErrorLoginRepeat);
		return false;
	}else{
		nStartTime = timeGetTime();
	}

	if( m_GameNetCL < 0 )
	{	
		m_pNetMgr->ReleaseDirServer();
		m_pNetMgr->Release();

		m_DirNetCL  = DIR_DISCONNECT_FLAG;
		strcpy_s(m_UserName, username);
		int nRet = 0;

		for( int i = 0; i < m_nBigWorldNum; i++ )
		{
			for( int j = 0; j < m_BigWorlds[i].getNumWorld(); j++ )
			{
				if( stricmp( m_BigWorlds[i].getIthWorld( j ).mudIp, ip ) == 0 && 
					m_BigWorlds[i].getIthWorld( j ).mudPort == port )
				{
					m_GameNetCL = m_pNetMgr->ConnectServer( m_BigWorlds[i].getIthWorld( j ).AuthIP, 
						m_BigWorlds[i].getIthWorld( j ).AuthPort,
						ip, 
						port,
						m_UserName, 
						const_cast<char*>( passwd ),
						&nRet );
					if( nRet != 0 )
					{
						notifyGui(eError,eErrorLoginError);
						return false;
					}
					if( m_GameNetCL >= 0 )
					{
						break;
					}
				}
			}
			if( m_GameNetCL >= 0 )
			{
				break;
			}
		}
		if(m_GameNetCL < 0 ) 
		{
			notifyGui(eError,eErrorLoginError);
			return false;
		}		
	}
	return true;
}

struct MudData
{
	NetCltAuthInfo authInfo;
	char		   ip[64];
	unsigned short port;
};

void MudNetMgr::requestLoginServerDirect()
{
	MudData data;
	CFile f;
	if( !f.Open("mudTmp",CFile::modeRead) )
	{
		notifyGui(eError,eErrorDirectLoginError);
		DeleteFile("mudTmp");
		return;
	}
	if( f.Read(&data,sizeof(MudData)) != sizeof(MudData) )
	{
		f.Close();
		DeleteFile("mudTmp");
		notifyGui(eError,eErrorDirectLoginError);
		return;
	}
	
	f.Close();
	AutoLock lock(&m_CriticalSection);
	int nRet = 0;
	m_GameNetCL = m_pNetMgr->DirectConnectServer( (char*)&data.authInfo,data.ip,data.port,&nRet );
 	DeleteFile("mudTmp");
	if( nRet != 0 || m_GameNetCL < 0 )
	{
		notifyGui(eError,eErrorDirectLoginError);
	}
}

void MudNetMgr::requestEnterGameWorld(tdr_longlong RoleID)
{
	if( m_loginRoleID != -1 )
	{
		notifyGui(eError,eErrorAlreadyLogin);
		return;
	}
	tagCSPkgBody msgbody;
	msgbody.MudRoleLoginReq.RoleID = RoleID;
	SendGameServerMsg(MUD_ROLE_LOGIN_REQ,msgbody);
}

void MudNetMgr::requestRoleLogout()
{
	tagCSPkgBody msgbody;
	msgbody.MudRoleLogoutReq.RoleID = m_loginRoleID;
	SendGameServerMsg(MUD_ROLE_LOGOUT_REQ,msgbody);
	m_loginRoleID = -1;
	m_vFriends.clear();
	//
	for( int i = 0 ; i < MAX_CHAT_ROOM_PLAYER ; i ++ )
	{
		m_OnlineInfo[i].RoleID = -1;
	}
	m_ChatRoomInfo.PlayerNum = 0;
}

void MudNetMgr::requestLogout()
{
	tagCSPkgBody msgbody;
	msgbody.AccLogoutReq.Uin = atoi(m_UserName);
	SendNetMessage(m_GameNetCL, ACC_LOGOUT_REQ, msgbody);
	m_GameNetCL = -1;
	m_loginRoleID = 1;
	m_UserName[0] = '\n';
}

void MudNetMgr::addMessageHwnd(HWND hWnd)
{
	for( size_t i = 0 ; i < m_DialogHwnd.size(); i++ )
	{
		if( m_DialogHwnd[i] == hWnd )
		{
			return;
		}
	}
	m_DialogHwnd.push_back(hWnd);
}

void MudNetMgr::notifyGui(eNotifyType type,LPARAM lParam)
{
	for( size_t i = 0 ; i < m_DialogHwnd.size(); i++ )
	{
		PostMessage(m_DialogHwnd[i],WM_NET_NOTIFY,type,lParam);
	}
}

DirBigWorldInfo& MudNetMgr::getBidWorldInfo(int nIndex)
{
	return m_BigWorlds[nIndex];
}

void MudNetMgr::requestHeartHeat()
{
	tagCSPkgBody body;
	CSTIME& serveTime = body.HeartBeatReq.SvrTime;
	ServerTimer::getSingleton().getServerTime( serveTime.BenchTimeSec, serveTime.BenchTimeUSec );
	int nResult = SendNetMessage(m_GameNetCL, HEARTBEAT_REQ, body);
}