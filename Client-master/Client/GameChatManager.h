#pragma once
//////////////////////////////////////////////////////////////////////////
// 角色聊天对话系统
//////////////////////////////////////////////////////////////////////////
#include "OgreSingleton.h"
#include "GameMsgPrint.h"
#include "NetMessage.h"
#include "cs_net.h"
#include "GameUIMsg.h"
#include "Item.h"
#include <map>

#pragma pack(1)
//tolua_begin
enum LINK_TYPE 
{
	NO_LINK				= 0,
	SERVER_LINK			= 1, 
	ITEM_LINK			= 2, 
	ARM_LINK			= 3, 
	MISSION_LINK		= 4, 
	POSTION_LINK		= 5, 
	SECONDARY_SKILL_LINK= 6, 
	PET_LINK			= 7,
	STORE_LINK			= 9,
};

enum tagCL_CHAT_TYPE 
{
	CL_CHAT_SAY		= CHAT_SAY, 		/* 附近 */
	CL_CHAT_TEAM	= CHAT_TEAM, 		/* 队伍 */
	CL_CHAT_PRIVATE = CHAT_PRIVATE, 	/* 私聊 */
	CL_CHAT_CLAN	= CHAT_CLAN, 		/* 公会 */
	CL_CHAT_WORLD	= CHAT_WORLD, 		/* 世界 */
	CL_CHAT_ITEM	= CHAT_ITEM, 		/* 小喇叭 */
	CL_CHAT_FRIEND  = CHAT_FRIEND,		/* 好友聊天 */
	CL_CHAT_BATTLE	= CHAT_BATTLE,		/* 战场聊天 */
	CL_CHAT_MSG		= CHAT_MUD_ROOM,	/* 信息 */
	CL_CHAT_AUTOTEAM = CHAT_AUTOTEAM,	/* 自动组队聊天 */
	CL_CHAT_SPAN	= CHAT_SPAN,		/* 跨服喇叭 */
	CL_CHAT_CLAN_GROUP = CHAT_CLAN_GROUP,/* 公会战团聊天 */
	CL_CHAT_WINDOW = CHAT_WINDOW,		/* 窗口聊天 */
	CL_CHAT_SPAN_MAP = CHAT_SPAN_MAP,	/*跨服聊天*/
	CL_CHAT_SPAN_MAP_WORLD = CHAT_SPAN_MAP_WORLD,	/*本服聊天*/

	CL_CHAT_SYS_MSG,					/* 系统信息 */
	CL_CHAT_ZONG_HE_MSG,				/* 综合信息 */
	CL_CHAT_XIN_XI_MSG,					/* 信息频道 */
	CL_CHAT_END		= 20,

	CL_CHAT_MONSTER				= CL_CHAT_END - 1,	// 怪物发言
	CL_CHAT_WARN_NEW_MESSAGE	= CL_CHAT_MONSTER - 1,
};

enum
{
	CL_CHAT_WORLD_LEVEL = CHAT_WORLD_LEVEL,  /* 可以世界发言的最小等级 */
	CL_CHAT_WORLD_COOL  = CHAT_WORLD_COOL,   /* 世界聊天的冷却 */
};

union tagLinkData_ToLua
{
	tagPostionLink_ToLua		m_SystemLink;		// 系统超链接
	tagItemLink_ToLua			m_ItemLink;			// 物品超链接
	tagArmLink_ToLua			m_RoleArmLink;		// 装备超链接
	tagMissionLink_ToLua		m_MissionLink;		// 任务超链接
	tagPostionLink_ToLua		m_PostionLink;		// 地图坐标超链接
	tagSecondarySkillLink_ToLua	m_SecondarySkillLink;//生活技能超链接
	tagPetLink_ToLua			m_PetLink;			// 宠物超链接
	tagStoreLink_ToLua			m_StoreLink;		// 店铺超链接
};

struct tagChatLinkData_ToLua
{
	int Type;                            	/*    Bind Macrosgroup:CHAT_LINK_TYPE,*/
	tagLinkData_ToLua Data;
};

struct tagChatLink_ToLua
{
	int							m_nLinkNum;			// 超链接数量
	tagChatLinkData_ToLua		m_LinkData[MAX_CHAT_LINK_NUM];
};

struct tagBulletinEntry_ToLua
{
	int ID;                              	/*   公告唯一ID */
	char Text[BULLETIN_MSG_LEN];          	/*   公告文本内容 */
	int RollInterval;                    	/*   滚动显示间隔，单位：秒 */
	int IconID;                          	/*  Ver.1124 Type为2时有效 */
	int Type;                            	/*  Ver.1124 0-正常公告 1-强制消息框 2-强制图标 */
	unsigned int TStart;                          	/*  Ver.1135 开始时间 */
};

struct tagCSChatReq_ToLua
{
	char Type;                            	/*    Bind Macrosgroup:CHAT_TYPE,*/
	char Res;                             	/*   客户端使用了此字段,判断是否需要回显 */
	unsigned short LinkLen;                        
	char ToName[NAME_LEN];               
	char Content[CLT_MSG_LEN];           
	char LinkContent[MAX_LINK];          
	short ListType;                        	/*  Ver.275 消耗道具的背包栏位 Bind Macrosgroup:LIST_TYPE,*/
	unsigned short GridIdx;                         	/*  Ver.275 消耗道具的栏位格子 */
	unsigned char OnlineType;                      	/*  Ver.578 接收者的在线类型 Bind Macrosgroup:ONLINE_TYPE,*/
	int Vip;                             	/*  Ver.898  */
	int IsCluster;                       	/*  Ver.1418 1-群发 */
};

struct tagFriendMsgGroup_ToLua
{
	int iGroup;                          	/*   多少个组 */
	int iMud;                            	/*  Ver.1489 有多少人在MudSvr */
	tagLong_ToLua Muds[MAX_BUDDY_FRIEND];          	/*  Ver.1489  */
	int Groups[MAX_FIREND_GROUP];       
	tagCSChatReq_ToLua ChatReq;                        
};


// 怪物发言类型
enum  
{
	CLIENT_SPEECH_MON_NONE = 0, 	/* 无发言信息 */
	CLIENT_SPEECH_MON_REFRESH = 1, 	/* 怪物刷新发言 */
	CLIENT_SPEECH_MON_DEAD = 2, 	/* 怪物死亡发言 */
	CLIENT_SPEECH_MON_HP = 3, 		/* 怪物减血发言 */
	CLIENT_SPEECH_MON_ATTACK = 4, 	/* 怪物攻击发言 */
	CLIENT_SPEECH_MON_BEHIT = 5, 	/* 怪物被击发言 */
};

enum
{
	CLIENT_CHAT_NOT_SHOW_IN_CHAT_FRAME	= 0,
	CLIENT_CHAT_SHOW_IN_CHAT_FRAME		= 1,
};

enum 
{
	CLIENT_CHAT_LINK_NONE		= 0,
	CLIENT_CHAT_LINK_SYSTEM		= CHAT_LINK_SYSTEM , 
	CLIENT_CHAT_LINK_ITEM		= CHAT_LINK_ITEM, 
	CLIENT_CHAT_LINK_ARM		= CHAT_LINK_ARM, 
	CLIENT_CHAT_LINK_MISSION	= CHAT_LINK_MISSION, 
	CLIENT_CHAT_LINK_POSTION	= CHAT_LINK_POSTION, 
	CLIENT_CHAT_LINK_SECONDARY_SKILL = CHAT_LINK_SECONDARY_SKILL, 
	CLIENT_CHAT_LINK_PET		= CHAT_LINK_PET, 
	CLIENT_CHAT_LINK_FAIRY		= CHAT_LINK_FAIRY,
	CLIENT_CHAT_LINK_STORE		= CHAT_LINK_STORE,
};

struct tagCacheDisplayTextInfo
{
	std::string		m_strLinkName;
	int				m_nLinkType;
};

#define CLIENT_SHOP_BULLETIN_TIME SHOP_BULLETIN_TIME
//tolua_end
#pragma pack()

struct GmBulletinData
{
	BULLETINENTRY bullet;
	int ShowTime;
};

class ChatManager //tolua_export
	: public NetMsgHandler, public Ogre::Singleton<ChatManager>, IGameMsgPrint
{//tolua_export
public:
	ChatManager	( void );
	~ChatManager( void );

	static ChatManager*		Create();	

	void parseTextLink( tagChatLink_ToLua& chatLink, char* szContent, int nLen );
	virtual int		HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

	virtual void setItemLink( Item& itemInfo );
	virtual void setPositionLink( int mapId, int x, int y );
	virtual void setMissionLink( int missionId, int nRecycleID );

	//tolua_begin
	void					setItemLink( tagItemLink_ToLua& itemLink );
	void					setArmLink( const tagArmLink_ToLua& armLink );
	void					setMissionLink( tagMissionLink_ToLua& missionLink );
	void					setPositionLink( tagPostionLink_ToLua& postionLink );
	void					setSecondarySkillLink( tagSecondarySkillLink_ToLua& secondarySkillLink );
	void					setPetLink( int nLow, int nHeight,const char* petName,const char* ownerName );
	void					setPetLink( tagPetLink_ToLua& petLink );
	void					setStoreLink( int storeID );
	tagItemLink_ToLua&			getItemLink( int linkId);
	const tagArmLink_ToLua&			getArmLink( int linkId );
	tagMissionLink_ToLua&			getMissionLink( int linkId );
	tagPostionLink_ToLua&			getPostionLink( int linkId );
	tagPostionLink_ToLua&			getSystemLink( int linkId );
	tagSecondarySkillLink_ToLua&	getSecondarySkillLink( int linkId );
	tagPetLink_ToLua&				getPetLink( int linkId );
	tagStoreLink_ToLua&				getStoreLink( int linkId );
	void					deleteLink( int type );
	void					deleteMapLinkInfo( int type );
	const	char*			deleteLink( char* szContext, const char* szlink );

	int						getLinkCount() { return m_nLinkCount; }
	int						getNotUsedLinkCount();
	int						getSelfLinkCount();

	void					requestChat( int channel, const char* szName, const char* szContent, 
		int nListType = -1, int nGrid = 0, char nShowUIType = CLIENT_CHAT_SHOW_IN_CHAT_FRAME, int online = 1);

	// 群发消息
	void requestMutilChat( tagFriendMsgGroup_ToLua msg, const char* szContent );
	tagFriendMsgGroup_ToLua getGroupMsg();

	virtual void			ShowSysMsg( const char* szText );
	std::string&			strReplace( std::string& szText, const char* src, const char* des );

	int						getNumCacheDisplayText();
	const tagCacheDisplayTextInfo& getCacheDisplayText( int i );
	bool					deleteLinkName( std::string strText, int nCurSorPos, int nSelSorPos, int nOpCode );
	bool					validateLinkName( std::string& strText );
	std::string&			strTrim( std::string& strTex );
	std::string&            linkName2ItemName( const char *szlinkName );

	virtual void			showMidTip( EMID_MESSAGE type ); 
	// 设置怪物技能发言
	void					setMonsterSkillSpeech( int nMonsterResId, int nMonsterMemId, int nConditionType, int nSkillId );

	// 设置怪物死亡发言
	void					setMonsterDeadSpeech( int pObjResId, int pObjId, const char* szAttackName, int nAttackRace, int nAttackGender );
	// 设置怪物攻击发言
	void					setMonsterSkillAttackSpeech( int pObjResId, int pObjId, const char* szAttackName, int nConditionType, int nSkillId );

	void					resetChatManager();

	// 获取公告的数量
	int						getBulletinNum()
	{ 
		return (int)m_vectBulletin.size(); 
	}
	// 获取指定下标的公告数据
	tagBulletinEntry_ToLua* getBulletinEntry( int nIndex );

	Item*					getItem(const tagRoleArm_ToLua* arm);
	Item*					getSparItem(const tagItemLink_ToLua* item);


	void					setLinkedItemLink(int nLinkId,int nLinkCountId);

	void					setChatConfigData(char* data,unsigned int len);
	char*					getChatConfigData();

	void					requestSendActionChat(int channel, const char* szName, const char* szContent);

	void					removeBulletinEntry( int nID );

	void					ClearLinkData();

	// 设置综合频道
	void					savePingBiZongHeChannelsOp();
	void					setZongHeChannelPingBiOp( int nChannelType, bool isPingBi );
	bool					isZongHeChannelPingBi( int nChannelType );

	// 设置世界频道
	void					savePingBiWorldChannelsOp();
	void					setWorldChannelPingBiOp( int nChannelType, bool isPingBi );
	bool					isWorldChannelPingBi( int nChannelType );

	// 设置公会频道
	void					savePingBiClanChannelsOp();
	void					setClanChannelPingBiOp( int nChannelType, bool isPingBi );
	bool					isClanChannelPingBi( int nChannelType );

	// 设置队伍频道
	void					savePingBiTeamChannelsOp();
	void					setTeamChannelPingBiOp( int nChannelType, bool isPingBi );
	bool					isTeamChannelPingBi( int nChannelType );

	// 设置附近频道
	void					savePingBiNearChannelsOp();
	void					setNearChannelPingBiOp( int nChannelType, bool isPingBi );
	bool					isNearChannelPingBi( int nChannelType );

	// 设置私聊频道
	void					savePingBiPrivateChannelsOp();
	void					setPrivateChannelPingBiOp( int nChannelType, bool isPingBi );
	bool					isPrivateChannelPingBi( int nChannelType );
	//tolua_end
	bool					isContentHaveChatType( int nChatType, const char* szChatText  );
	int						getChatType( const char* szChatText );

	void					addMakeSucessMsg(int itemID,const char* msg);
	void					parseItemLink( char* szContent );

	char*					getLastPingBiZongHeChannelsOp();
	char*					getLastPingBiWorldChannelsOp();
	char*					getLastPingBiClanChannelsOp();
	char*					getLastPingBiTeamChannelsOp();
	char*					getLastPingBiNearChannelsOp();
	char*					getLastPingBiPrivateChannelsOp();

	void					Update();
private:
	void					OnGetChatReq( const CSCHATREQ& svrChatReqData );

	bool	isLinkTypeInPrepareState( int nDestType );
	void	removeCacheDisplayText( int nType );


	void					updateNotUsedLinkCount();
	int						getLinkContentLength( const tagChatLinkData_ToLua& data );
	void					InitChatLinkData( tagChatLink_ToLua& chatLink, const unsigned char* LinkContent, unsigned short LinkLen );

private:
	tagChatLink_ToLua					m_ChatLink;
	std::string							m_szItemName;  

	std::vector<tagCacheDisplayTextInfo>	m_allCacheDisplayText;
	std::map<int, tagChatLink_ToLua>    m_ChatLinkMap;
	int									m_nLinkCount;
	int									m_nSelfPrepareLinkCount;
	tagFriendMsgGroup_ToLua				m_Group_msg;

	std::vector<BULLETINENTRY>			m_vectBulletin;

	Item								m_LinkItem;

	char								m_szLasetPingBiZongHeChatChannels[CL_CHAT_END];
	char								m_szLasetPingBiWorldChatChannels[CL_CHAT_END];
	char								m_szLasetPingBiClanChatChannels[CL_CHAT_END];
	char								m_szLasetPingBiTeamChatChannels[CL_CHAT_END];
	char								m_szLasetPingBiNearChatChannels[CL_CHAT_END];
	char								m_szLasetPingBiPrivateChatChannels[CL_CHAT_END];

	std::vector<GmBulletinData>			m_vGmBulletin;
};//tolua_export

