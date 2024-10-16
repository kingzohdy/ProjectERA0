#pragma once
//////////////////////////////////////////////////////////////////////////
// ��ɫ����Ի�ϵͳ
//////////////////////////////////////////////////////////////////////////
#include "OgreSingleton.h"
#include "GameMsgPrint.h"
#include "NetMessage.h"
#include "cs_net.h"
#include "Item.h"
#include <map>

#pragma pack(1)
//tolua_begin
enum LINK_TYPE 
{
	SERVER_LINK			= 1, 
	ITEM_LINK			= 2, 
	ARM_LINK			= 3, 
	MISSION_LINK		= 4, 
	POSTION_LINK		= 5, 
	SECONDARY_SKILL_LINK= 6, 
	PET_LINK			= 7,
};

enum CHANNEL_TYPE
{
	ZONGHE_CHANNEL=0,
	CHATROOM_CHANNEL=1,
	WORLD_CHANNEL=2,
	CLAN_CHANNEL=3,
	PRIVATE_CHANNEL=4,
};

enum tagCL_CHAT_TYPE 
{
	CL_CHAT_SAY		= 0, 		/* ���� */
	CL_CHAT_TEAM	= 1, 		/* ���� */
	CL_CHAT_PRIVATE = 2, 		/* ˽�� */
	CL_CHAT_CLAN	= 3, 		/* ���� */
	CL_CHAT_WORLD	= 4, 		/* ���� */
	CL_CHAT_ITEM	= 5, 		/* С���� */
	CL_CHAT_FRIEND  = 6,		/* �������� */
	CL_CHAT_BATTLE	= 7, 	/* ս������ */
	CL_CHAT_MUD_ROOM	= 8,		/* ��������Ϣ */
};

enum
{
	CL_CHAT_WORLD_LEVEL = CHAT_WORLD_LEVEL,  /* �������緢�Ե���С�ȼ� */
	CL_CHAT_WORLD_COOL  = CHAT_WORLD_COOL,   /* �����������ȴ */
};

// ��Ʒ������
struct tagItemLink_ToLua
{
	int m_nItemId;
	int m_nItemDur;
	int InstVal1;                        	/*  Ver.1322  */
	int InstVal2;                        	/*  Ver.1322  */
	int InstVal3;                        	/*  Ver.1322  */
	int Flag;                            	/*  Ver.1324  */
};
// װ��������
struct tagArmLink_ToLua
{
	tagRoleArm_ToLua m_arm;
};

// ��������
struct tagMissionLink_ToLua
{
	int m_MissionId;
};

// λ����������
struct tagPostionLink_ToLua
{
	int m_nMapId;
	int m_nX;
	int m_nY;
};

struct tagSecondarySkillLink_ToLua
{
	short SkillType;                       	/*   ��������: �ɿ��ҩ�ȵ� Bind Macrosgroup:SECONDARY_SKILL_TYPE,*/
	short SkillLevel;                      	/*   ���ܵȼ� */
	int SkillVal;                        	/*   ���������� */
	char RecipeInfo[MAX_SECONDARY_SKILL_RECIPE];
};

struct tagPetLink_ToLua
{
	tdr_longlong	PetWid;
	char			PetName[NAME_LEN];
	char			OwnerName[ROLE_NAME_LEN]; 
};

union tagLinkData_ToLua
{
	tagPostionLink_ToLua		m_SystemLink;		// ϵͳ������
	tagItemLink_ToLua			m_ItemLink;			// ��Ʒ������
	tagArmLink_ToLua			m_RoleArmLink;		// װ��������
	tagMissionLink_ToLua		m_MissionLink;		// ��������
	tagPostionLink_ToLua		m_PostionLink;		// ��ͼ���곬����
	tagSecondarySkillLink_ToLua	m_SecondarySkillLink;//����ܳ�����
	tagPetLink_ToLua			m_PetLink;			//���ﳬ����
};

struct tagChatLinkData_ToLua
{
 	int Type;                            	/*    Bind Macrosgroup:CHAT_LINK_TYPE,*/
	tagLinkData_ToLua Data;
};

struct tagChatLink_ToLua
{
	int							m_nLinkNum;			// ����������
	tagChatLinkData_ToLua		m_LinkData[MAX_CHAT_LINK_NUM];
};

//tolua_end
#pragma pack()

class ChatManager //tolua_export
	: public NetMsgHandler, public Ogre::Singleton<ChatManager>
{//tolua_export
public:
	ChatManager	( void );
	~ChatManager( void );

	static ChatManager*		Create();	

	virtual int				HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

	//tolua_begin
	tagItemLink_ToLua&			getItemLink( int linkId);
	const tagArmLink_ToLua&		getArmLink( int linkId );
	
	std::string&			strReplace( std::string& szText, const char* src, const char* des );
	
	Item*					getItem(const tagRoleArm_ToLua* arm);

	int	getChatmsgtype()
	{
		return (int)m_ChatRes.Type;
	}

	const char*  getChatmsgrolename()
	{
		return m_ChatRes.From;
	}

	const char*  getChatmsgContent()
	{
		return m_ChatRes.Content;
	}

	const char* getChatreqrolename()
	{
		return m_ChatReq.ToName;
	}

	const char* getChatreqContent()
	{
		return m_ChatReq.Content;
	}

	int getChatreqType()
	{
		return m_ChatReq.Type;
	}

	//tolua_end
	
	int getChatType( const char* szChatText );

	int getLinkContentLength( const tagChatLinkData_ToLua& data );
	void InitChatLinkData( tagChatLink_ToLua& chatLink, const unsigned char* LinkContent, unsigned short LinkLen );

private:
	std::map<int, tagChatLink_ToLua>    m_ChatLinkMap;
	int									m_nLinkCount;

	Item								m_LinkItem;
	CSCHATRES							m_ChatRes;
	CSCHATREQ							m_ChatReq;
};//tolua_export

