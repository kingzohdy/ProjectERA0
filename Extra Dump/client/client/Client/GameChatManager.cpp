#include "GameChatManager.h"
#include "GameMsgPrint.h"
#include "GameManager.h"
#include "OgreLog.h"
#include "cs_net.h"
#include "GameDBData.h"
#include "GameUIMsg.h"
#include "GameMainPlayer.h"
#include "Item.h"
#include "GameQuest.h"
#include "OgreScriptLuaVM.h"
#include "GameUserConfig.h"
#include "GameFriendManager.h"
#include <algorithm>
#include <Windows.h>
#include <utility>
#include <string>
#include "../UILib/ui_gameui.h"
#include "../UILib/ui_common.h"
#include "../UILib/ui_framemgr.h"

#include "ResourceCheck.h"
#include "GameClientOpt.h"
#include "GameClanManager.h"
#include "ServerTimer.h"
#include <iostream>
using namespace std;

const std::string strkeyWorld = "@@%d#n";
const std::string strItemLinkKeyWorld = "@@0#n";
const std::string strTaskLinkColor = "FEBC42";
const std::string STR_PARSE_LINK = "@ITEMLINK@";
const std::string STR_POS_PARSE_LINK = "@POSLINK@";

ChatManager* ChatManager::Create()
{
	ChatManager* p = new ChatManager;
	INetworkManager::getSingleton().RegisterMsgHandler( CHAT_RES, p );
	INetworkManager::getSingleton().RegisterMsgHandler( SYS_MSG,  p );
	INetworkManager::getSingleton().RegisterMsgHandler( MON_SPEECH, p );
	INetworkManager::getSingleton().RegisterMsgHandler( MON_SPEECH_EX, p );
	INetworkManager::getSingleton().RegisterMsgHandler( BULLETIN_SVR, p );
	INetworkManager::getSingleton().RegisterMsgHandler( CHAT_REQ, p );

	GameMsgPrint::getSingleton().registGameMsgHandler( p );
	return p;
}

ChatManager::ChatManager()
{
	resetChatManager();
	ZeroMemory( &m_ChatLink, sizeof(tagChatLink_ToLua) );
	m_ChatLink.m_nLinkNum = 0;
	m_nLinkCount = 0;
	ZeroMemory( m_szLasetPingBiWorldChatChannels,	sizeof( m_szLasetPingBiWorldChatChannels ) );
	ZeroMemory( m_szLasetPingBiClanChatChannels,		sizeof( m_szLasetPingBiClanChatChannels ) );

	static_assert( sizeof(tagRoleArm_ToLua) == sizeof(tagRoleArm) );
	static_assert( sizeof(tagChatLink_ToLua) == sizeof(tagChatLink) );
	static_assert( sizeof(tagPostionLink_ToLua) == sizeof(tagPostionLink) );
	static_assert( sizeof(tagItemLink_ToLua) == sizeof(tagItemLink) );
	//static_assert( sizeof(tagMissionLink_ToLua) == sizeof(tagMissionLink) );
	static_assert( sizeof(tagSecondarySkillLink_ToLua) == sizeof(tagSecondarySkillLink) );

	GameClientOpt::getSingleton().setLevelSize( SAVE_OPT_LAST_PING_BI_ZONG_HE_PIN_DAO, CL_CHAT_END );
	GameClientOpt::getSingleton().setLevelSize( SAVE_OPT_LAST_PING_BI_WORLD_PIN_DAO, CL_CHAT_END );
	GameClientOpt::getSingleton().setLevelSize( SAVE_OPT_LAST_PING_BI_CLAN_PIN_DAO, CL_CHAT_END );
	GameClientOpt::getSingleton().setLevelSize( SAVE_OPT_LAST_PING_BI_TEAM_PIN_DAO, CL_CHAT_END );
	GameClientOpt::getSingleton().setLevelSize( SAVE_OPT_LAST_PING_BI_NEAR_PIN_DAO, CL_CHAT_END );
	GameClientOpt::getSingleton().setLevelSize( SAVE_OPT_LAST_PING_BI_PRIVATE_PIN_DAO, CL_CHAT_END );
	// 	static_assert( sizeof(tagPetLink_ToLua) == sizeof(tagPetLink) );
}

ChatManager::~ChatManager()
{
	GameMsgPrint::getSingleton().unRegistGameMsgHandler( this );
}

void ChatManager::parseTextLink( tagChatLink_ToLua& chatLink, char* szContent, int nLen )
{
	int nLinkType = getChatType( szContent );
	char bufLinkMsgOld[CLT_MSG_LEN] = { '\0' };
	char bufLinkMsgNew[CLT_MSG_LEN] = { '\0' }; 
	bool bLink = false;

	for( int i = 0 ; i < MAX_CHAT_LINK_NUM ; i ++ )
	{
		tagChatLinkData_ToLua& oneChatLinkData = chatLink.m_LinkData[i];
		bool isHaveChatTypeSymbol = isContentHaveChatType( oneChatLinkData.Type, szContent );
		switch( oneChatLinkData.Type )
		{
		case CHAT_LINK_SYSTEM:
			// 系统指定位置
			{
				tagPostionLink_ToLua& pos = oneChatLinkData.Data.m_SystemLink;
				if( isHaveChatTypeSymbol && pos.m_nMapId != 0 )
				{
					const tagMapDef& mapDef = DBDataManager::getSingleton().getMapDef( pos.m_nMapId );
					memset(bufLinkMsgOld, 0, sizeof(bufLinkMsgOld));
					memset(bufLinkMsgNew, 0, sizeof(bufLinkMsgNew));
					int nLinkId = m_nLinkCount * 10 + SERVER_LINK;
					sprintf( bufLinkMsgNew, "#L<%s(%d,%d)>@@%d#n", mapDef.Name, pos.m_nX, pos.m_nY, nLinkId );
					sprintf( bufLinkMsgOld, "#L<%s(%d,%d)>@@%d#n", mapDef.Name, pos.m_nX, pos.m_nY, SERVER_LINK );

					std::string szText = szContent;
					strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
					memset(  szContent, 0, CLT_MSG_LEN );
					strncpy( szContent, szText.c_str(), szText.length()+1 );
					m_ChatLinkMap[m_nLinkCount++]	= chatLink; 
					return;
				}
			}
			break;
		case CHAT_LINK_ITEM:
			{
				tagItemLink_ToLua& item = oneChatLinkData.Data.m_ItemLink;
				//物品
				if( isHaveChatTypeSymbol && item.m_nItemId != 0 )
				{
					ITEMDEF itemDefine = DBDataManager::getSingleton().getItemDef( item.m_nItemId );

					memset(bufLinkMsgOld, 0, sizeof(bufLinkMsgOld));
					memset(bufLinkMsgNew, 0, sizeof(bufLinkMsgNew));
					int nLinkId = m_nLinkCount * 10 + ITEM_LINK;

					sprintf( bufLinkMsgNew, "@@%d#n",  nLinkId );
					sprintf( bufLinkMsgOld, "@@%d#n", ITEM_LINK );

					std::string szText = szContent;
					strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
					memset(  szContent, 0, CLT_MSG_LEN );
					strncpy( szContent, szText.c_str(), szText.length()+1 );
					bLink = true;
				}
			}
			break;
		case CHAT_LINK_ARM:
			{
				tagArmLink_ToLua& arm = chatLink.m_LinkData[i].Data.m_RoleArmLink;
				//物品
				if( isHaveChatTypeSymbol && arm.m_arm.DefID != 0 )
				{
					ITEMDEF itemDefine = DBDataManager::getSingleton().getItemDef( arm.m_arm.DefID );

					memset(bufLinkMsgOld, 0, sizeof(bufLinkMsgOld));
					memset(bufLinkMsgNew, 0, sizeof(bufLinkMsgNew));
					int nLinkId = m_nLinkCount * 10 + ARM_LINK;

					sprintf( bufLinkMsgNew, "@@%d#n",  nLinkId );
					sprintf( bufLinkMsgOld, "@@%d#n", ARM_LINK );

					std::string szText = szContent;
					strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
					memset(  szContent, 0, CLT_MSG_LEN );
					strncpy( szContent, szText.c_str(), szText.length()+1 );
					bLink = true;
				}
			}
			break;
		case CHAT_LINK_MISSION:
			{
				//任务
				tagMissionLink_ToLua& mission = chatLink.m_LinkData[i].Data.m_MissionLink;
				if( isHaveChatTypeSymbol && mission.m_MissionId != 0 )
				{
					const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( mission.m_MissionId );
					const char* szQuestType = GameQuest::getSingleton().getQuestTypeName( taskDef.Type );
					memset( bufLinkMsgOld, 0, sizeof(bufLinkMsgOld) );
					memset( bufLinkMsgNew, 0, sizeof(bufLinkMsgNew) );
					int nLinkId = m_nLinkCount * 10 + MISSION_LINK;
					assert( taskDef.Type <= TASK_TYPE_DISCOVER );
					if( taskDef.TaskGetLimit.GetLevelMin > 0 )
					{
						sprintf( bufLinkMsgNew, "#L#c%s[%d级]<%s %s>@@%d#n", strTaskLinkColor.c_str(),taskDef.TaskGetLimit.GetLevelMin, szQuestType, taskDef.Name, nLinkId );
						sprintf( bufLinkMsgOld, "#L#c%s[%d级]<%s %s>@@%d#n", strTaskLinkColor.c_str(),taskDef.TaskGetLimit.GetLevelMin, szQuestType, taskDef.Name, MISSION_LINK );
					}else
					{
						sprintf( bufLinkMsgNew, "#L#c%s<%s %s>@@%d#n", strTaskLinkColor.c_str(), szQuestType, taskDef.Name, nLinkId );
						sprintf( bufLinkMsgOld, "#L#c%s<%s %s>@@%d#n", strTaskLinkColor.c_str(), szQuestType, taskDef.Name, MISSION_LINK );
					}

					std::string szText = szContent;
					strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
					memset( szContent, 0, CLT_MSG_LEN );
					strncpy( szContent, szText.c_str(), szText.length()+1 );
					bLink = true;
				}
			}
			break;
		case CHAT_LINK_POSTION:
			{
				tagPostionLink_ToLua& pos = oneChatLinkData.Data.m_PostionLink;
				//位置
				if( isHaveChatTypeSymbol && pos.m_nMapId != 0)
				{
					const tagMapDef& mapDef = DBDataManager::getSingleton().getMapDef( pos.m_nMapId );

					memset(bufLinkMsgOld, 0, sizeof(bufLinkMsgOld));
					memset(bufLinkMsgNew, 0, sizeof(bufLinkMsgNew));
					int nLinkId = m_nLinkCount * 10 + POSTION_LINK;
					sprintf( bufLinkMsgNew, "#L<%s(%d,%d)>@@%d#n", mapDef.Name, pos.m_nX/100, pos.m_nY/100, nLinkId );
					sprintf( bufLinkMsgOld, "#L<%s(%d,%d)>@@%d#n", mapDef.Name, pos.m_nX/100, pos.m_nY/100, POSTION_LINK );

					std::string szText = szContent;
					strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
					memset(  szContent, 0, CLT_MSG_LEN );
					strncpy( szContent, szText.c_str(), szText.length()+1 );
					bLink = true;
				}
			}
			break;
		case CHAT_LINK_SECONDARY_SKILL:
			{
				tagSecondarySkillLink_ToLua& skill = oneChatLinkData.Data.m_SecondarySkillLink;
				//生活技能
				if( isHaveChatTypeSymbol && skill.SkillType != 0)
				{
					const SECONDARYSKILLDEF* pSkillDef = DBDataManager::getSingleton().getSecondarySkillDef(skill.SkillType,skill.SkillLevel);
					if( pSkillDef == NULL )
					{
						break;
					}
					memset(bufLinkMsgOld, 0, sizeof(bufLinkMsgOld));
					memset(bufLinkMsgNew, 0, sizeof(bufLinkMsgNew));
					int nLinkId = m_nLinkCount * 10 + SECONDARY_SKILL_LINK;
					sprintf( bufLinkMsgNew, "#L#cDCD27D[%s]@@%d#n", pSkillDef->NameDesc,nLinkId );
					sprintf( bufLinkMsgOld, "#L#cDCD27D[%s]@@%d#n", pSkillDef->NameDesc,SECONDARY_SKILL_LINK );

					std::string szText = szContent;
					strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
					memset(  szContent, 0, CLT_MSG_LEN );
					strncpy( szContent, szText.c_str(), szText.length()+1 );
					bLink = true;
				}
			}
			break;
		case CHAT_LINK_PET:
			{
				tagPetLink_ToLua& pet = oneChatLinkData.Data.m_PetLink;
				int nLinkId = m_nLinkCount * 10 + PET_LINK;
				sprintf( bufLinkMsgNew, "#L<%s>@@%d#n", pet.PetName,nLinkId );
				sprintf( bufLinkMsgOld, "#L<%s>@@%d#n", pet.PetName,PET_LINK );

				std::string szText = szContent;
				strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
				memset(  szContent, 0, CLT_MSG_LEN );
				strncpy( szContent, szText.c_str(), szText.length()+1 );
				bLink = true;
			}
			break;
		default:
			break;
		}
	}	

	if ( bLink )
	{
		m_ChatLinkMap[m_nLinkCount++]	= chatLink; 
	}
}

int ChatManager::HandleNetMsg( int msgid, const tagCSPkgBody& msgbody )
{
	switch( msgid )
	{
	case CHAT_RES:
		{
		const CSCHATRES& Msg	= msgbody.ChatRes;
// 		if( ActorManager::getSingleton().getMainPlayer() != NULL &&
// 			ActorManager::getSingleton().getMainPlayer()->isHaveGMPerm() )
// 		{
// 			char buf[256];
// 			sprintf(buf,"RobotTestGMProc( \"%s\",\"%s\" )",Msg.Content,Msg.From);;
// 			GameManager::getSingleton().getScriptVM()->callString(buf);
// 		}

			GameFriendManager::getSingleton().setOnlinePlayer( Msg.From, Msg.OnlineType);
			tagUIMsgBody UI_Msg;
			UI_Msg.ChatMsg.Type		= Msg.Type;
			const char* pszFromSpeakerName = Msg.From;
			if( pszFromSpeakerName[0] != 0 )
			{
				strncpy( UI_Msg.ChatMsg.Speaker, pszFromSpeakerName, ROLE_NAME_LEN );
				//如果在战场黑名单中的话，不显示
				GameFriendManager& friendMgr = GameFriendManager::getSingleton();
				if( friendMgr.isInBattleBlack(Msg.From) )
				{
					return 0;
				}

				if ( ( Msg.Type == CHAT_WORLD || Msg.Type == CHAT_SAY ) && 
					friendMgr.isPlayerRelation( CLIENT_RELATION_TYPE_BLACK, pszFromSpeakerName ) )
				{
					return 0;
				}
			}

			if( Msg.Type == CHAT_CLAN || Msg.Type == CHAT_WORLD )
			{
				MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
				if( pMainPlayer != NULL && strcmp( Msg.From, pMainPlayer->getName() ) == 0 )
				{
					return 0;
				}
			}
			strncpy( UI_Msg.ChatMsg.Content, Msg.Content, CLT_MSG_LEN );
			UI_Msg.ChatMsg.isVip = ( ( Msg.Vip & ROLE_VIP_FLAG ) > 0 );
			UI_Msg.ChatMsg.IsCluster = Msg.IsCluster;
			UI_Msg.ChatMsg.WorldID = Msg.WorldID;
			tagChatLink_ToLua chatLink ;
			memset( &chatLink, 0, sizeof(tagChatLink_ToLua) );
			if( Msg.LinkLen > 0 )
			{
				InitChatLinkData( chatLink, Msg.LinkContent, Msg.LinkLen );
			}
			//如果有链接
			if( chatLink.m_nLinkNum > 0 )
			{
				char bufLinkMsgOld[CLT_MSG_LEN];
				char bufLinkMsgNew[CLT_MSG_LEN]; 
				for( int i = 0 ; i < chatLink.m_nLinkNum ; i ++ )
				{
					const tagChatLinkData_ToLua& oneChatLinkData = chatLink.m_LinkData[i];
					bool isHaveChatTypeSymbol = isContentHaveChatType( oneChatLinkData.Type, Msg.Content );
					switch ( oneChatLinkData.Type )
					{
					case CHAT_LINK_SYSTEM:
						// 系统指定位置
						{
							tagPostionLink_ToLua& pos = chatLink.m_LinkData[i].Data.m_SystemLink;
							if( isHaveChatTypeSymbol && pos.m_nMapId != 0 )
							{
								const tagMapDef& mapDef = DBDataManager::getSingleton().getMapDef( pos.m_nMapId );
								memset(bufLinkMsgOld, 0, sizeof(bufLinkMsgOld));
								memset(bufLinkMsgNew, 0, sizeof(bufLinkMsgNew));
								int nLinkId = m_nLinkCount * 10 + SERVER_LINK;
								sprintf( bufLinkMsgNew, "#L<%s(%d,%d)>@@%d#n", mapDef.Name, pos.m_nX, pos.m_nY, nLinkId );
								sprintf( bufLinkMsgOld, "#L<%s(%d,%d)>@@%d#n", mapDef.Name, pos.m_nX, pos.m_nY, SERVER_LINK );

								std::string szText = UI_Msg.ChatMsg.Content;
								strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
								memset(  UI_Msg.ChatMsg.Content, 0, CLT_MSG_LEN );
								strncpy( UI_Msg.ChatMsg.Content, szText.c_str(), szText.length()+1 );
								m_ChatLinkMap[m_nLinkCount++]	= chatLink; 

								GameUIMsg::getSingleton().setUIMsg( UI_Msg );
								GameManager::getSingleton().SendUIEvent( "GE_UPDATE_CHATMSG" );
								return 0;
							}
						}
						break;
					case CHAT_LINK_ITEM:
						{
							tagItemLink_ToLua& item = chatLink.m_LinkData[i].Data.m_ItemLink;
							//物品
							if( isHaveChatTypeSymbol && item.m_nItemId != 0 )
							{
								ITEMDEF itemDefine = DBDataManager::getSingleton().getItemDef( item.m_nItemId );

								memset(bufLinkMsgOld, 0, sizeof(bufLinkMsgOld));
								memset(bufLinkMsgNew, 0, sizeof(bufLinkMsgNew));
								int nLinkId = m_nLinkCount * 10 + ITEM_LINK;

								sprintf( bufLinkMsgNew, "@@%d#n",  nLinkId );
								sprintf( bufLinkMsgOld, "@@%d#n", ITEM_LINK );

								std::string szText = UI_Msg.ChatMsg.Content;
								strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
								memset(  UI_Msg.ChatMsg.Content, 0, CLT_MSG_LEN );
								strncpy( UI_Msg.ChatMsg.Content, szText.c_str(), szText.length()+1 );
							}
						}
						break;
					case CHAT_LINK_ARM:
						{
							tagArmLink_ToLua& arm = chatLink.m_LinkData[i].Data.m_RoleArmLink;
							//物品
							if( isHaveChatTypeSymbol && arm.m_arm.DefID != 0 )
							{
								ITEMDEF itemDefine = DBDataManager::getSingleton().getItemDef( arm.m_arm.DefID );

								memset(bufLinkMsgOld, 0, sizeof(bufLinkMsgOld));
								memset(bufLinkMsgNew, 0, sizeof(bufLinkMsgNew));
								int nLinkId = m_nLinkCount * 10 + ARM_LINK;

								sprintf( bufLinkMsgNew, "@@%d#n",  nLinkId );
								sprintf( bufLinkMsgOld, "@@%d#n", ARM_LINK );

								std::string szText = UI_Msg.ChatMsg.Content;
								strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
								memset(  UI_Msg.ChatMsg.Content, 0, CLT_MSG_LEN );
								strncpy( UI_Msg.ChatMsg.Content, szText.c_str(), szText.length()+1 );
							}
						}
						break;
					case CHAT_LINK_MISSION:
						{
							//任务
							tagMissionLink_ToLua& mission = chatLink.m_LinkData[i].Data.m_MissionLink;
							if( isHaveChatTypeSymbol && mission.m_MissionId != 0 )
							{
								const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( mission.m_MissionId );
								const char* szQuestType = GameQuest::getSingleton().getQuestTypeName( taskDef.Type );
								memset( bufLinkMsgOld, 0, sizeof(bufLinkMsgOld) );
								memset( bufLinkMsgNew, 0, sizeof(bufLinkMsgNew) );
								int nLinkId = m_nLinkCount * 10 + MISSION_LINK;
								assert( taskDef.Type <= TASK_TYPE_DISCOVER );
								if( taskDef.TaskGetLimit.GetLevelMin > 0 )
								{
									sprintf( bufLinkMsgNew, "#L#c%s[%d级]<%s %s>@@%d#n", strTaskLinkColor.c_str(),taskDef.TaskGetLimit.GetLevelMin, szQuestType, taskDef.Name, nLinkId );
									sprintf( bufLinkMsgOld, "#L#c%s[%d级]<%s %s>@@%d#n", strTaskLinkColor.c_str(),taskDef.TaskGetLimit.GetLevelMin, szQuestType, taskDef.Name, MISSION_LINK );
								}else
								{
									sprintf( bufLinkMsgNew, "#L#c%s<%s %s>@@%d#n", strTaskLinkColor.c_str(), szQuestType, taskDef.Name, nLinkId );
									sprintf( bufLinkMsgOld, "#L#c%s<%s %s>@@%d#n", strTaskLinkColor.c_str(), szQuestType, taskDef.Name, MISSION_LINK );
								}

								std::string szText = UI_Msg.ChatMsg.Content;
								strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
								memset( UI_Msg.ChatMsg.Content, 0, CLT_MSG_LEN );
								strncpy( UI_Msg.ChatMsg.Content, szText.c_str(), szText.length()+1 );
							}
						}
						break;
					case CHAT_LINK_POSTION:
						{
							tagPostionLink_ToLua& pos = chatLink.m_LinkData[i].Data.m_PostionLink;
							//位置
							if( isHaveChatTypeSymbol && pos.m_nMapId != 0)
							{
								const tagMapDef& mapDef = DBDataManager::getSingleton().getMapDef( pos.m_nMapId );

								memset(bufLinkMsgOld, 0, sizeof(bufLinkMsgOld));
								memset(bufLinkMsgNew, 0, sizeof(bufLinkMsgNew));
								int nLinkId = m_nLinkCount * 10 + POSTION_LINK;
								sprintf( bufLinkMsgNew, "#L<%s(%d,%d)>@@%d#n", mapDef.Name, pos.m_nX/100, pos.m_nY/100, nLinkId );
								sprintf( bufLinkMsgOld, "#L<%s(%d,%d)>@@%d#n", mapDef.Name, pos.m_nX/100, pos.m_nY/100, POSTION_LINK );

								std::string szText = UI_Msg.ChatMsg.Content;
								strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
								memset(  UI_Msg.ChatMsg.Content, 0, CLT_MSG_LEN );
								strncpy( UI_Msg.ChatMsg.Content, szText.c_str(), szText.length()+1 );
							}
						}
						break;
					case CHAT_LINK_SECONDARY_SKILL:
						{
							tagSecondarySkillLink_ToLua& skill = chatLink.m_LinkData[i].Data.m_SecondarySkillLink;
							//生活技能
							if( isHaveChatTypeSymbol && skill.SkillType != 0)
							{
								const SECONDARYSKILLDEF* pSkillDef = DBDataManager::getSingleton().getSecondarySkillDef(skill.SkillType,skill.SkillLevel);
								if( pSkillDef == NULL )
								{
									break;
								}
								memset(bufLinkMsgOld, 0, sizeof(bufLinkMsgOld));
								memset(bufLinkMsgNew, 0, sizeof(bufLinkMsgNew));
								int nLinkId = m_nLinkCount * 10 + SECONDARY_SKILL_LINK;
								sprintf( bufLinkMsgNew, "#L#cDCD27D[%s]@@%d#n", pSkillDef->NameDesc,nLinkId );
								sprintf( bufLinkMsgOld, "#L#cDCD27D[%s]@@%d#n", pSkillDef->NameDesc,SECONDARY_SKILL_LINK );

								std::string szText = UI_Msg.ChatMsg.Content;
								strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
								memset(  UI_Msg.ChatMsg.Content, 0, CLT_MSG_LEN );
								strncpy( UI_Msg.ChatMsg.Content, szText.c_str(), szText.length()+1 );
							}
						}
						break;
					case CHAT_LINK_PET:
						{
							tagPetLink_ToLua& pet = chatLink.m_LinkData[i].Data.m_PetLink;
							int nLinkId = m_nLinkCount * 10 + PET_LINK;
							sprintf( bufLinkMsgNew, "#L<%s>@@%d#n", pet.PetName,nLinkId );
							sprintf( bufLinkMsgOld, "#L<%s>@@%d#n", pet.PetName,PET_LINK );

							std::string szText = UI_Msg.ChatMsg.Content;
							strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
							memset(  UI_Msg.ChatMsg.Content, 0, CLT_MSG_LEN );
							strncpy( UI_Msg.ChatMsg.Content, szText.c_str(), szText.length()+1 );
						}
						break;
					case CHAT_LINK_STORE:
						{
							tagStoreLink_ToLua& store = chatLink.m_LinkData[i].Data.m_StoreLink;
							int nLinkId = m_nLinkCount * 10 + STORE_LINK;
							sprintf( bufLinkMsgNew, "#L[%s]@@%d#n", "查看店铺", nLinkId );
							sprintf( bufLinkMsgOld, "#L[%s]@@%d#n", "查看店铺", STORE_LINK );

							std::string szText = UI_Msg.ChatMsg.Content;
							strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
							memset(  UI_Msg.ChatMsg.Content, 0, CLT_MSG_LEN );
							strncpy( UI_Msg.ChatMsg.Content, szText.c_str(), szText.length()+1 );
						}
						break;
					default:
						break;
					}
				}				
			}
			HWND hWnd = FindWindow(NULL,"ZeroChatWin");
			if ( hWnd != NULL )
			{
				COPYDATASTRUCT cpd;
				cpd.dwData = 0;
				cpd.cbData = strlen(UI_Msg.ChatMsg.Content);
				cpd.lpData=(void*)UI_Msg.ChatMsg.Content;
				SendMessage( hWnd, WM_COPYDATA,0,(LPARAM)&cpd);
			}
			m_ChatLinkMap[m_nLinkCount++]	= chatLink; 

			GameUIMsg::getSingleton().setUIMsg( UI_Msg );

			if( UI_Msg.ChatMsg.Type == CL_CHAT_FRIEND )
			{					
				GameManager::getSingleton().SendUIEvent( "GE_FRIEND_GETMSG" );
				GameManager::getSingleton().FlashWindow();
			}else if( UI_Msg.ChatMsg.Type == CL_CHAT_AUTOTEAM )
			{
				GameManager::getSingleton().SendUIEvent( "GE_AUTOTEAM_GETMSG" );
			}else if ( UI_Msg.ChatMsg.Type == CL_CHAT_WINDOW )
			{
				GameManager::getSingleton().SendUIEvent( "GE_WINDOW_GETMSG" );
			}else{
				GameManager::getSingleton().SendUIEvent( "GE_UPDATE_CHATMSG" );
			}

			if( Msg.Type == CHAT_SAY && ActorManager::getSingleton().getGameOpt( GAME_OPT_SHOW_OTHERPLAYER_TALK_POP )  )
			{
				//伤害统计不显示泡泡
				if( !strstr(UI_Msg.ChatMsg.Content,"#@!") && !strstr(UI_Msg.ChatMsg.Content,"@@#") )
				{
					g_pFrameMgr->addPopWindow( "ChatPop", Msg.From, UI_Msg.ChatMsg.Content, -100, -100, 8, 
						"PopBackArrowTexture", true, 0, 0 );
				}
			}
		}
		break;
	case SYS_MSG:
		{			
			const CSSYSMSG& SysMsg	= msgbody.SysMsg;
			tagUIMsgBody	UI_Msg;
			UI_Msg.SysMsg.Type		= SysMsg.Type;
			UI_Msg.SysMsg.Times		= SysMsg.Times;
			UI_Msg.SysMsg.Interval	= SysMsg.Interval;
			strncpy( UI_Msg.SysMsg.Msg, SysMsg.Msg, SYS_MSG_LEN );
			if( SysMsg.Type == SYS_BULLETIN )
			{
				for( size_t i =0; i < sizeof(UI_Msg.SysMsg.Msg); i++ )
				{
					if( UI_Msg.SysMsg.Msg[i] == 10 )
					{
						UI_Msg.SysMsg.Msg[i] = 0;
					}
				}
			}

			tagChatLink_ToLua chatLink = *(tagChatLink_ToLua*)SysMsg.LinkContent;
			//如果有链接
			if( SysMsg.LinkLen > 0 && chatLink.m_nLinkNum > 0 )
			{
				char bufLinkMsgOld[CLT_MSG_LEN];
				char bufLinkMsgNew[CLT_MSG_LEN];

				for( int i = 0 ; i < MAX_CHAT_LINK_NUM ; i ++ )
				{
					switch( chatLink.m_LinkData[i].Type )
					{
					case CHAT_LINK_SYSTEM:
						// 系统指定位置
						{
							tagPostionLink_ToLua& pos = chatLink.m_LinkData[i].Data.m_SystemLink;
							if( pos.m_nMapId != 0 )
							{
								const tagMapDef& mapDef = DBDataManager::getSingleton().getMapDef( pos.m_nMapId );
								memset(bufLinkMsgOld, 0, sizeof(bufLinkMsgOld));
								memset(bufLinkMsgNew, 0, sizeof(bufLinkMsgNew));
								int nLinkId = m_nLinkCount * 10 + SERVER_LINK;
								sprintf( bufLinkMsgNew, "#L<%s(%d,%d)>@@%d#n", mapDef.Name, pos.m_nX, pos.m_nY, nLinkId );
								sprintf( bufLinkMsgOld, "#L<%s(%d,%d)>@@%d#n", mapDef.Name, pos.m_nX, pos.m_nY, SERVER_LINK );

								std::string szText = UI_Msg.SysMsg.Msg;
								strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
								memset(  UI_Msg.SysMsg.Msg, 0, CLT_MSG_LEN );
								strncpy( UI_Msg.SysMsg.Msg, szText.c_str(), szText.length()+1 );
								m_ChatLinkMap[m_nLinkCount++]	= chatLink; 

								GameUIMsg::getSingleton().setUIMsg( UI_Msg );
								GameManager::getSingleton().SendUIEvent( "GE_UPDATE_CHATMSG" );
								return 0;
							}
						}
						break;
					case CHAT_LINK_ITEM:
						{
							tagItemLink_ToLua& item = chatLink.m_LinkData[i].Data.m_ItemLink;
							//物品
							if( item.m_nItemId != 0 )
							{
								ITEMDEF itemDefine = DBDataManager::getSingleton().getItemDef( item.m_nItemId );
								std::string szItemName = itemDefine.Name;
								memset(bufLinkMsgNew, 0, sizeof(bufLinkMsgNew));
								int nLinkId = m_nLinkCount * 10 + ITEM_LINK;
								sprintf( bufLinkMsgNew, "#L#c%s<%s>@@%d#n", getItemColor(itemDefine.Important,itemDefine.SuitSetID).c_str(), szItemName.c_str(), nLinkId );

								std::string szText = UI_Msg.SysMsg.Msg;
								strReplace( szText, STR_PARSE_LINK.c_str(), bufLinkMsgNew );
								memset(  UI_Msg.SysMsg.Msg, 0, CLT_MSG_LEN );
								strncpy( UI_Msg.SysMsg.Msg, szText.c_str(), szText.length()+1 );
							}
						}
						break;
					case CHAT_LINK_ARM:
						{
							tagArmLink_ToLua& arm = chatLink.m_LinkData[i].Data.m_RoleArmLink;
							//物品
							if( arm.m_arm.DefID != 0 )
							{
								ITEMDEF itemDefine = DBDataManager::getSingleton().getItemDef( arm.m_arm.DefID );
								std::string szItemName = getItem( (tagRoleArm_ToLua*)&arm.m_arm )->getDisplayName();
								memset(bufLinkMsgNew, 0, sizeof(bufLinkMsgNew));
								int nLinkId = m_nLinkCount * 10 + ARM_LINK;
								sprintf( bufLinkMsgNew, "#L#c%s<%s>@@%d#n", getItemColor(itemDefine.Important,itemDefine.SuitSetID).c_str(), szItemName.c_str(), nLinkId );

								std::string szText = UI_Msg.SysMsg.Msg;
								strReplace( szText, STR_PARSE_LINK.c_str(), bufLinkMsgNew );
								memset(  UI_Msg.SysMsg.Msg, 0, CLT_MSG_LEN );
								strncpy( UI_Msg.SysMsg.Msg, szText.c_str(), szText.length()+1 );
							}
						}
						break;
					case CHAT_LINK_PET:
						{
							tagPetLink_ToLua& pet = chatLink.m_LinkData[i].Data.m_PetLink;
							int nLinkId = m_nLinkCount * 10 + PET_LINK;
							sprintf( bufLinkMsgNew, "#L<%s>@@%d#n", pet.PetName,nLinkId );
							sprintf( bufLinkMsgOld, "#L<%s>@@%d#n", pet.PetName,PET_LINK );

							std::string szText = UI_Msg.ChatMsg.Content;
							strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
							memset(  UI_Msg.ChatMsg.Content, 0, CLT_MSG_LEN );
							strncpy( UI_Msg.ChatMsg.Content, szText.c_str(), szText.length()+1 );
						}
						break;
					case CHAT_LINK_POSTION:
						{
							tagPostionLink_ToLua& pos = chatLink.m_LinkData[i].Data.m_PostionLink;
							int nLinkId = m_nLinkCount * 10 + POSTION_LINK;
							const tagMapDef& mapDef = DBDataManager::getSingleton().getMapDef( pos.m_nMapId );
							sprintf( bufLinkMsgNew, "#L<%s>@@%d#n", mapDef.Name, nLinkId );
							std::string szText = UI_Msg.SysMsg.Msg;
							strReplace( szText, STR_POS_PARSE_LINK.c_str(), bufLinkMsgNew );
							memset(  UI_Msg.SysMsg.Msg, 0, CLT_MSG_LEN );
							strncpy( UI_Msg.SysMsg.Msg, szText.c_str(), szText.length()+1 );
						}
						break;
					default:
						break;
					}
				}
				m_ChatLinkMap[m_nLinkCount++]	= chatLink;
			}
			GameUIMsg::getSingleton().setUIMsg( UI_Msg );
			GameManager::getSingleton().SendUIEvent( "GE_UPDATE_SYSMSG" );
		}
		break;
	case MON_SPEECH:
		{
			const CSMONSPEECH& monSpeech					= msgbody.MonSpeech;
			int nSpeakIndex									= monSpeech.SpeechIdx;
			const tagMonsterSpeechDef* pMonsterSpeechDef	= DBDataManager::getSingleton().getMonsterSpeechDef( monSpeech.MonsterID );
			if( pMonsterSpeechDef != NULL && nSpeakIndex < MAX_MON_ACT_SPEECH )
			{
				// SYS_WHOOP
				char szScriptFun[1024];
				sprintf( szScriptFun, 
					"MonsterSpeak( %d, %d, [===[%s ]===], %d, [===[%s]===], %d, %d )", 
					pMonsterSpeechDef->MonsterID, 
					monSpeech.MemID, 
					pMonsterSpeechDef->MonActSpeech[nSpeakIndex].Desc,
					SYS_CHAT, 
					monSpeech.RoleName,
					monSpeech.Career,
					monSpeech.Gender
					);

				GameManager::getSingleton().getScriptVM()->callString( szScriptFun );
			}
		}
		break;
	case BULLETIN_SVR:
		{
			CSBULLETINSVRDATA bulletinSvr = msgbody.BulletinSvr.BulletinSvrData;		
			for( int i = 0 ; i < bulletinSvr.BulletinSnapshot.Num ; i ++ )
			{					
				//bulletinSvr.BulletinSnapshot.BulletinArray[i].Text;
			}
			switch( msgbody.BulletinSvr.Operation )
			{
			case BULLETIN_SVR_SNAPSHOT: 
				{
					int nMemNum = bulletinSvr.BulletinSnapshot.Num;
					for( int i = 0; i < nMemNum; i++ )
					{
						if( bulletinSvr.BulletinSnapshot.BulletinArray[i].Type == 0 )
						{
							parseItemLink( bulletinSvr.BulletinSnapshot.BulletinArray[i].Text );
							m_vectBulletin.push_back( bulletinSvr.BulletinSnapshot.BulletinArray[i] );
						}else{
							GmBulletinData data;
							data.bullet = bulletinSvr.BulletinSnapshot.BulletinArray[i];
							if( data.bullet.TStart == 0 )
							{
								data.bullet.TStart = getServerTime().sec;
							}
							if( data.bullet.TStart > getServerTime().sec )
							{
								data.ShowTime = data.bullet.RollInterval + data.bullet.TStart;
							}							
							m_vGmBulletin.push_back( data );
						}
					}
				}
				break;
			case BULLETIN_SVR_CHG_ADD:
				{
					// 这个新插入的放到第二条，立刻显示
					if( bulletinSvr.BulletinAdded.Type == 0 )
					{
						parseItemLink( bulletinSvr.BulletinAdded.Text );
						if ( m_vectBulletin.empty() )
						{
							m_vectBulletin.push_back( bulletinSvr.BulletinAdded );
						}
						else{
							m_vectBulletin.insert( m_vectBulletin.begin() + 1, bulletinSvr.BulletinAdded );
						}
					}else{
						GmBulletinData data;
						data.bullet = bulletinSvr.BulletinAdded;
						if( data.bullet.TStart == 0 )
						{
							data.bullet.TStart = getServerTime().sec;
						}
						if( data.bullet.TStart > getServerTime().sec )
						{
							data.ShowTime = data.bullet.RollInterval + data.bullet.TStart;
						}						
						m_vGmBulletin.push_back( data );
					}
				}
				break;
			case BULLETIN_SVR_CHG_DEL:
				{
					std::vector<BULLETINENTRY>::iterator iter = m_vectBulletin.begin();
					while( iter != m_vectBulletin.end() )
					{
						if( iter->ID == bulletinSvr.BulletinDeletedID && iter->RollInterval != SHOP_BULLETIN_TIME )
						{
							m_vectBulletin.erase( iter );
							break;
						}
						iter++;
					}
					std::vector<GmBulletinData>::iterator it = m_vGmBulletin.begin();
					while(it != m_vGmBulletin.end())
					{
						if( it->bullet.ID == bulletinSvr.BulletinDeletedID )
						{
							m_vGmBulletin.erase( it );
							break;
						}
						it ++;
					}
				}
				break;
			}
			GameManager::getSingleton().SendUIEvent( "GE_UPDATE_SYS_BULLETIN" );
		}
		break;
	case MON_SPEECH_EX:
		{
			const CSMONSPEECHEX&  monspeechex = msgbody.MonSpeechEx;
			char szScriptFun[1024];
			// SYS_WHOOP
			sprintf( szScriptFun, 
				"MonsterSpeak( %d, %d, [===[%s ]===], %d, [===[%s]===],%d, %d )", 
				monspeechex.MonsterID,
				monspeechex.MemID,
				monspeechex.Content,
				SYS_CHAT,
				monspeechex.RoleName,
				monspeechex.Career,
				monspeechex.Gender
				);

			GameManager::getSingleton().getScriptVM()->callString( szScriptFun );
			break;
		}
		break;

	case CHAT_REQ:
		{
			const CSCHATREQ& svrChatReqData = msgbody.ChatReq;
			OnGetChatReq( svrChatReqData );
			HWND hWnd = FindWindow(NULL,"ZeroChatWin");
			if ( hWnd != NULL )
			{
				COPYDATASTRUCT cpd;
				cpd.dwData = 0;
				cpd.cbData = strlen(svrChatReqData.Content)+1;
				cpd.lpData=malloc(cpd.cbData);
				memcpy(cpd.lpData,svrChatReqData.Content,cpd.cbData);
				SendMessage( hWnd, WM_COPYDATA,0,(LPARAM)&cpd);
			}
		}
		break;
	}
	return 0;
}

void ChatManager::setItemLink( Item& itemInfo )
{
	static_assert(sizeof( tagRoleArm_ToLua ) == sizeof( tagRoleArm ));
	m_szItemName = itemInfo.getDisplayName();
	if( itemInfo.getItemType() == ITEM_SLOTTYPE_AMM )
	{
		tagArmLink_ToLua armLink;
		memcpy(&armLink.m_arm,&itemInfo.m_ItemData.m_Arm,sizeof(tagRoleArm_ToLua));
		setArmLink( armLink );
	}else if( itemInfo.getItemType() == ITEM_SLOTTYPE_ITEM )
	{
		tagItemLink_ToLua itemLink;
		itemLink.m_nItemId = itemInfo.getItemId();
		itemLink.m_nItemDur = itemInfo.getItemDur();
		itemLink.InstVal1 = itemInfo.getItemInstVal1();
		itemLink.InstVal2 = itemInfo.getItemInstVal2();
		itemLink.InstVal3 = itemInfo.getItemInstVal3();
		itemLink.Flag	  = itemInfo.getItemFlag();
		setItemLink( itemLink );
	}
}

void ChatManager::setPositionLink( int mapId, int x, int y )
{
	tagPostionLink_ToLua postionLink;
	postionLink.m_nMapId = mapId;
	postionLink.m_nX	 = x;
	postionLink.m_nY	 = y;
	setPositionLink( postionLink );
}

void ChatManager::setMissionLink( int missionId, int nRecycleID )
{
	tagMissionLink_ToLua missionLink;
	missionLink.m_MissionId = missionId;
	missionLink.m_nRecycleID= nRecycleID;
	setMissionLink( missionLink );
}

void ChatManager::setItemLink( tagItemLink_ToLua& itemLink )
{
	if( itemLink.m_nItemId == 0 ) return;

	if ( isLinkTypeInPrepareState( CHAT_LINK_ITEM ) || isLinkTypeInPrepareState( CHAT_LINK_ARM ) )
	{
		return;
	}

	tagChatLinkData_ToLua& oneChatLinkData = m_ChatLink.m_LinkData[m_ChatLink.m_nLinkNum];
	if( oneChatLinkData.Type == NO_LINK )
	{
		oneChatLinkData.Type = CHAT_LINK_ITEM;
		oneChatLinkData.Data.m_ItemLink = itemLink;
		tagUIMsgBody UI_Msg;
		UI_Msg.ChatSetLink.m_nLinkType	= ITEM_LINK;
		UI_Msg.ChatSetLink.m_itemLink	= oneChatLinkData.Data.m_ItemLink;

		tagItemDef_ToLua* itemDefine = getItemDef( oneChatLinkData.Data.m_RoleArmLink.m_arm.DefID );
		if( itemDefine == NULL )
		{
			return;
		}
		m_szItemName = itemDefine->Name;
		char szItemLinkName[MAX_CLI_LINKNAME];

		sprintf( szItemLinkName, "<%s>", m_szItemName.c_str());
		strncpy( UI_Msg.ChatSetLink.m_szLinkName, szItemLinkName,  MAX_CLI_LINKNAME );
		tagCacheDisplayTextInfo oneCacheDisplayText;
		oneCacheDisplayText.m_strLinkName	= szItemLinkName;
		oneCacheDisplayText.m_nLinkType		= oneChatLinkData.Type;
		m_allCacheDisplayText.push_back( oneCacheDisplayText );

		//向服务端发送的格式
		sprintf( szItemLinkName, "#L#c%s<%s>@@%d#n", getItemColor(itemDefine->Important,itemDefine->SuitSetID).c_str(), 
			m_szItemName.c_str(), ITEM_LINK );
		strncpy( UI_Msg.ChatSetLink.m_szLinkName2ServerRich, szItemLinkName, MAX_CLI_LINKNAME );

		//向聊天窗口发送的格式
		int nLinkId = m_nLinkCount * 10 + ITEM_LINK;
		sprintf( szItemLinkName, "#L#c%s<%s>@@%d#n", getItemColor(itemDefine->Important,itemDefine->SuitSetID).c_str(), 
			m_szItemName.c_str(), 
			nLinkId );
		memset( UI_Msg.ChatSetLink.m_szLinkName2ClientRich, 0, MAX_CLI_LINKNAME );
		strncpy( UI_Msg.ChatSetLink.m_szLinkName2ClientRich, szItemLinkName, strlen(szItemLinkName)+1 );

		GameUIMsg::getSingleton().setUIMsg( UI_Msg );
		GameManager::getSingleton().SendUIEvent( "GE_SET_CHATLINK" );
		updateNotUsedLinkCount();
		++m_ChatLink.m_nLinkNum;
	}
}

void ChatManager::setArmLink(const tagArmLink_ToLua& armLink)
{
	if( armLink.m_arm.DefID == 0 ) return;
	if ( isLinkTypeInPrepareState( CHAT_LINK_ITEM ) || isLinkTypeInPrepareState( CHAT_LINK_ARM ) )
	{
		return;
	}

	tagChatLinkData_ToLua& oneChatLinkData = m_ChatLink.m_LinkData[m_ChatLink.m_nLinkNum];
	if( oneChatLinkData.Type == NO_LINK )
	{
		oneChatLinkData.Type = CHAT_LINK_ARM;
		oneChatLinkData.Data.m_RoleArmLink = armLink;
		tagUIMsgBody UI_Msg;
		UI_Msg.ChatSetLink.m_nLinkType	= ARM_LINK;
		UI_Msg.ChatSetLink.m_armLink	= oneChatLinkData.Data.m_RoleArmLink;

		tagItemDef_ToLua* itemDefine = getItemDef( oneChatLinkData.Data.m_RoleArmLink.m_arm.DefID );
		if( itemDefine == NULL )
		{
			return;
		}
		char szItemLinkName[MAX_CLI_LINKNAME];

		sprintf( szItemLinkName, "<%s>", m_szItemName.c_str());
		strncpy( UI_Msg.ChatSetLink.m_szLinkName, szItemLinkName,  MAX_CLI_LINKNAME );
		tagCacheDisplayTextInfo oneCacheDisplayText;
		oneCacheDisplayText.m_strLinkName	= szItemLinkName;
		oneCacheDisplayText.m_nLinkType		= oneChatLinkData.Type;
		m_allCacheDisplayText.push_back( oneCacheDisplayText );

		//向服务端发送的格式
		sprintf( szItemLinkName, "#L#c%s<%s>@@%d#n", getItemColor(itemDefine->Important,itemDefine->SuitSetID).c_str(), 
			m_szItemName.c_str(), ARM_LINK );
		strncpy( UI_Msg.ChatSetLink.m_szLinkName2ServerRich, szItemLinkName, MAX_CLI_LINKNAME );

		//向聊天窗口发送的格式
		int nLinkId = getSelfLinkCount() * 10 + ARM_LINK;
		sprintf( szItemLinkName, "#L#c%s<%s>@@%d#n", getItemColor(itemDefine->Important,itemDefine->SuitSetID).c_str(), 
			m_szItemName.c_str(), nLinkId );
		memset( UI_Msg.ChatSetLink.m_szLinkName2ClientRich, 0, MAX_CLI_LINKNAME );
		strncpy( UI_Msg.ChatSetLink.m_szLinkName2ClientRich, szItemLinkName, strlen(szItemLinkName)+1 );

		GameUIMsg::getSingleton().setUIMsg( UI_Msg );
		GameManager::getSingleton().SendUIEvent( "GE_SET_CHATLINK" );
		updateNotUsedLinkCount();
		++m_ChatLink.m_nLinkNum;
	}
}

void ChatManager::setMissionLink( tagMissionLink_ToLua& missionLink )
{
	if( missionLink.m_MissionId == 0 ) return;

	if( isLinkTypeInPrepareState( CHAT_LINK_MISSION ) )
	{
		return;
	}
	tagChatLinkData_ToLua& oneChatLinkData = m_ChatLink.m_LinkData[m_ChatLink.m_nLinkNum];

	if( oneChatLinkData.Type == NO_LINK )
	{
		oneChatLinkData.Data.m_MissionLink  = missionLink;
		oneChatLinkData.Type = CHAT_LINK_MISSION;
		tagUIMsgBody UI_Msg;
		UI_Msg.ChatSetLink.m_nLinkType	 = MISSION_LINK;
		UI_Msg.ChatSetLink.m_missionLink = oneChatLinkData.Data.m_MissionLink;

		const TASKDEF& taskDefine = DBDataManager::getSingleton().getTaskDef( oneChatLinkData.Data.m_MissionLink.m_MissionId );
		const char* szQuestType = GameQuest::getSingleton().getQuestTypeName( taskDefine.Type );
		char szTaskLinkName[MAX_CLI_LINKNAME];
		assert( taskDefine.Type <= TASK_TYPE_DISCOVER );
		if( taskDefine.TaskGetLimit.GetLevelMin > 0 )
		{
			sprintf( szTaskLinkName, "[%d级]<%s %s>",taskDefine.TaskGetLimit.GetLevelMin,szQuestType, taskDefine.Name );
		}else{
			sprintf( szTaskLinkName, "<%s %s>",szQuestType, taskDefine.Name );
		}

		strncpy( UI_Msg.ChatSetLink.m_szLinkName, szTaskLinkName,  MAX_CLI_LINKNAME );
		tagCacheDisplayTextInfo oneCacheDisplayText;
		oneCacheDisplayText.m_strLinkName	= szTaskLinkName;
		oneCacheDisplayText.m_nLinkType		= oneChatLinkData.Type;
		m_allCacheDisplayText.push_back( oneCacheDisplayText );

		//向服务端发送的格式
		if( taskDefine.TaskGetLimit.GetLevelMin > 0  )
		{
			sprintf( szTaskLinkName, "#L#c%s[%d级]<%s %s>@@%d#n", strTaskLinkColor.c_str(),
				taskDefine.TaskGetLimit.GetLevelMin, 
				szQuestType, taskDefine.Name, MISSION_LINK );
		}else{
			sprintf( szTaskLinkName, "#L#c%s<%s %s>@@%d#n", strTaskLinkColor.c_str(), 
				szQuestType, taskDefine.Name, MISSION_LINK );
		}
		strncpy( UI_Msg.ChatSetLink.m_szLinkName2ServerRich, szTaskLinkName, MAX_CLI_LINKNAME );

		//向聊天窗口发送的格式
		int nLinkId = getSelfLinkCount() * 10 + MISSION_LINK;
		if( taskDefine.TaskGetLimit.GetLevelMin > 0 )
		{
			sprintf( szTaskLinkName, "#L#c%s[%d级]<%s %s>@@%d#n", strTaskLinkColor.c_str(),taskDefine.TaskGetLimit.GetLevelMin, szQuestType, taskDefine.Name, nLinkId );
		}else{
			sprintf( szTaskLinkName, "#L#c%s<%s %s>@@%d#n", strTaskLinkColor.c_str(), szQuestType, taskDefine.Name, nLinkId );
		}
		memset( UI_Msg.ChatSetLink.m_szLinkName2ClientRich, 0, MAX_CLI_LINKNAME );
		strncpy( UI_Msg.ChatSetLink.m_szLinkName2ClientRich, szTaskLinkName, strlen(szTaskLinkName)+1 );

		GameUIMsg::getSingleton().setUIMsg( UI_Msg );
		GameManager::getSingleton().SendUIEvent( "GE_SET_CHATLINK" );
		updateNotUsedLinkCount();
		++m_ChatLink.m_nLinkNum;
	}
}

void ChatManager::setPositionLink( tagPostionLink_ToLua& postionLink )
{	
	if( isLinkTypeInPrepareState( CHAT_LINK_POSTION ) )
	{
		return;
	}
	tagChatLinkData_ToLua& oneChatLinkData = m_ChatLink.m_LinkData[m_ChatLink.m_nLinkNum];

	if( oneChatLinkData.Type == NO_LINK )
	{
		oneChatLinkData.Type		= CHAT_LINK_POSTION;
		tagPostionLink_ToLua& curCachedPositionLinkData = oneChatLinkData.Data.m_PostionLink;
		curCachedPositionLinkData	= postionLink;

		tagUIMsgBody UI_Msg;
		tagSetLink& curUILinkData	= UI_Msg.ChatSetLink;
		curUILinkData.m_nLinkType	= POSTION_LINK;
		curUILinkData.m_postionLink	= curCachedPositionLinkData;

		const tagMapDef& mapDef = DBDataManager::getSingleton().getMapDef( curCachedPositionLinkData.m_nMapId );
		const char* mapName = GameManager::getSingleton().getGameMapShower()->GetMinMapContal()->getMapName( curCachedPositionLinkData.m_nMapId );

		char szFormatPositionLinkBuffer[MAX_CLI_LINKNAME] = { '\0' };

		sprintf( szFormatPositionLinkBuffer, "<%s(%d,%d)>", mapName, curCachedPositionLinkData.m_nX / 100, 
			curCachedPositionLinkData.m_nY / 100 );
		strncpy( curUILinkData.m_szLinkName, szFormatPositionLinkBuffer, MAX_CLI_LINKNAME );
		tagCacheDisplayTextInfo oneCacheDisplayText;
		oneCacheDisplayText.m_strLinkName	= szFormatPositionLinkBuffer;
		oneCacheDisplayText.m_nLinkType		= oneChatLinkData.Type;
		m_allCacheDisplayText.push_back( oneCacheDisplayText );

		//向服务端发送的格式
		sprintf( szFormatPositionLinkBuffer, "#L<%s(%d,%d)>@@%d#n", mapName, curCachedPositionLinkData.m_nX/100, 
			curCachedPositionLinkData.m_nY/100, POSTION_LINK );
		strncpy( curUILinkData.m_szLinkName2ServerRich, szFormatPositionLinkBuffer, MAX_CLI_LINKNAME );

		//向聊天窗口发送的格式
		int nLinkId = getSelfLinkCount() * 10 + POSTION_LINK;
		sprintf( szFormatPositionLinkBuffer, "#L<%s(%d,%d)>@@%d#n", mapName, curCachedPositionLinkData.m_nX/100, 
			curCachedPositionLinkData.m_nY/100, nLinkId);
		memset( curUILinkData.m_szLinkName2ClientRich, 0, MAX_CLI_LINKNAME );
		strncpy( curUILinkData.m_szLinkName2ClientRich, szFormatPositionLinkBuffer, strlen(szFormatPositionLinkBuffer)+1 );

		GameUIMsg::getSingleton().setUIMsg( UI_Msg );
		GameManager::getSingleton().SendUIEvent( "GE_SET_CHATLINK" );
		updateNotUsedLinkCount();
		++m_ChatLink.m_nLinkNum;
	}
}

void ChatManager::setSecondarySkillLink( tagSecondarySkillLink_ToLua& secondarySkillLink )
{
	if( isLinkTypeInPrepareState( CHAT_LINK_SECONDARY_SKILL ) )
	{
		return;
	}
	tagChatLinkData_ToLua& oneChatLinkData = m_ChatLink.m_LinkData[m_ChatLink.m_nLinkNum];

	if( oneChatLinkData.Type == NO_LINK )
	{
		oneChatLinkData.Data.m_SecondarySkillLink = secondarySkillLink;
		oneChatLinkData.Type = CHAT_LINK_SECONDARY_SKILL;

		tagUIMsgBody UI_Msg;
		UI_Msg.ChatSetLink.m_nLinkType	= SECONDARY_SKILL_LINK;
		UI_Msg.ChatSetLink.m_SecondarySkillLink	= oneChatLinkData.Data.m_SecondarySkillLink;

		char szItemLinkName[MAX_CLI_LINKNAME];
		const SECONDARYSKILLDEF* pSkillDef = DBDataManager::getSingleton().getSecondarySkillDef(secondarySkillLink.SkillType,secondarySkillLink.SkillLevel);
		if( pSkillDef == NULL )
		{
			return;
		}
		sprintf( szItemLinkName, "[%s]",pSkillDef->NameDesc );
		strncpy( UI_Msg.ChatSetLink.m_szLinkName, szItemLinkName, MAX_CLI_LINKNAME );
		tagCacheDisplayTextInfo oneCacheDisplayText;
		oneCacheDisplayText.m_strLinkName	= szItemLinkName;
		oneCacheDisplayText.m_nLinkType		= oneChatLinkData.Type;
		m_allCacheDisplayText.push_back( oneCacheDisplayText );

		//向服务端发送的格式
		sprintf( szItemLinkName, "#L#cDCD27D[%s]@@%d#n", pSkillDef->NameDesc, SECONDARY_SKILL_LINK );
		strncpy( UI_Msg.ChatSetLink.m_szLinkName2ServerRich, szItemLinkName, MAX_CLI_LINKNAME );

		//向聊天窗口发送的格式
		int nLinkId = getSelfLinkCount() * 10 + SECONDARY_SKILL_LINK;
		sprintf( szItemLinkName, "#L#cDCD27D[%s]@@%d#n",  pSkillDef->NameDesc, nLinkId);
		memset( UI_Msg.ChatSetLink.m_szLinkName2ClientRich, 0, MAX_CLI_LINKNAME );
		strncpy( UI_Msg.ChatSetLink.m_szLinkName2ClientRich, szItemLinkName, strlen(szItemLinkName)+1 );

		GameUIMsg::getSingleton().setUIMsg( UI_Msg );
		GameManager::getSingleton().SendUIEvent( "GE_SET_CHATLINK" );
		updateNotUsedLinkCount();
		++m_ChatLink.m_nLinkNum;
	}
}

void ChatManager::setStoreLink( int storeID )
{
	if( isLinkTypeInPrepareState( CHAT_LINK_STORE ) )
	{
		return;
	}
	tagChatLinkData_ToLua& oneChatLinkData = m_ChatLink.m_LinkData[m_ChatLink.m_nLinkNum];

	if( oneChatLinkData.Type == NO_LINK )
	{
		oneChatLinkData.Data.m_StoreLink.StoreID = storeID;
		oneChatLinkData.Type = CHAT_LINK_STORE;

		tagUIMsgBody UI_Msg;
		UI_Msg.ChatSetLink.m_nLinkType	= STORE_LINK;
		UI_Msg.ChatSetLink.m_StoreLink	= oneChatLinkData.Data.m_StoreLink;

		char szItemLinkName[MAX_CLI_LINKNAME];
		sprintf( szItemLinkName, "[%s]", "查看店铺" );
		strncpy( UI_Msg.ChatSetLink.m_szLinkName, szItemLinkName, MAX_CLI_LINKNAME );
		tagCacheDisplayTextInfo oneCacheDisplayText;
		oneCacheDisplayText.m_strLinkName	= szItemLinkName;
		oneCacheDisplayText.m_nLinkType		= oneChatLinkData.Type;
		m_allCacheDisplayText.push_back( oneCacheDisplayText );

		//向服务端发送的格式
		sprintf( szItemLinkName, "#L[%s]@@%d#n", "查看店铺", STORE_LINK );
		strncpy( UI_Msg.ChatSetLink.m_szLinkName2ServerRich, szItemLinkName, MAX_CLI_LINKNAME );

		int nLinkId = getSelfLinkCount() * 10 + STORE_LINK;
		sprintf( szItemLinkName, "#L[%s]@@%d#n", "查看店铺", nLinkId );
		memset( UI_Msg.ChatSetLink.m_szLinkName2ClientRich, 0, MAX_CLI_LINKNAME );
		strncpy( UI_Msg.ChatSetLink.m_szLinkName2ClientRich, szItemLinkName, strlen(szItemLinkName)+1 );

		GameUIMsg::getSingleton().setUIMsg( UI_Msg );
		GameManager::getSingleton().SendUIEvent( "GE_SET_CHATLINK" );
		updateNotUsedLinkCount();
		++m_ChatLink.m_nLinkNum;
	}
}

void ChatManager::setPetLink( tagPetLink_ToLua& petLink )
{
	if( isLinkTypeInPrepareState( CHAT_LINK_PET ) )
	{
		return;
	}
	tagChatLinkData_ToLua& oneChatLinkData = m_ChatLink.m_LinkData[m_ChatLink.m_nLinkNum];

	if( oneChatLinkData.Type == NO_LINK )
	{
		oneChatLinkData.Data.m_PetLink = petLink;
		oneChatLinkData.Type = CHAT_LINK_PET;

		tagUIMsgBody UI_Msg;
		UI_Msg.ChatSetLink.m_nLinkType	= PET_LINK;
		UI_Msg.ChatSetLink.m_PetLink	= oneChatLinkData.Data.m_PetLink;

		char szItemLinkName[MAX_CLI_LINKNAME];
		sprintf( szItemLinkName, "<%s>",petLink.PetName );
		strncpy( UI_Msg.ChatSetLink.m_szLinkName, szItemLinkName, MAX_CLI_LINKNAME );
		tagCacheDisplayTextInfo oneCacheDisplayText;
		oneCacheDisplayText.m_strLinkName	= szItemLinkName;
		oneCacheDisplayText.m_nLinkType		= oneChatLinkData.Type;
		m_allCacheDisplayText.push_back( oneCacheDisplayText );

		//向服务端发送的格式
		sprintf( szItemLinkName, "#L<%s>@@%d#n", petLink.PetName, PET_LINK );
		strncpy( UI_Msg.ChatSetLink.m_szLinkName2ServerRich, szItemLinkName, MAX_CLI_LINKNAME );

		int nLinkId = getSelfLinkCount() * 10 + PET_LINK;
		sprintf( szItemLinkName, "#L<%s>@@%d#n",  petLink.PetName, nLinkId);
		memset( UI_Msg.ChatSetLink.m_szLinkName2ClientRich, 0, MAX_CLI_LINKNAME );
		strncpy( UI_Msg.ChatSetLink.m_szLinkName2ClientRich, szItemLinkName, strlen(szItemLinkName)+1 );

		GameUIMsg::getSingleton().setUIMsg( UI_Msg );
		GameManager::getSingleton().SendUIEvent( "GE_SET_CHATLINK" );
		updateNotUsedLinkCount();
		++m_ChatLink.m_nLinkNum;
	}
}

void ChatManager::setPetLink( int nLow, int nHeight,const char* petName,const char* ownerName )
{
	if( isLinkTypeInPrepareState( CHAT_LINK_PET ) )
	{
		return;
	}
	tagChatLinkData_ToLua& oneChatLinkData = m_ChatLink.m_LinkData[m_ChatLink.m_nLinkNum];
	if( oneChatLinkData.Type == NO_LINK )
	{
		tagLong_ToLua petWid;
		petWid.nLow = nLow;
		petWid.nHeight = nHeight;
		oneChatLinkData.Data.m_PetLink.PetWid = petWid;
		strcpy(oneChatLinkData.Data.m_PetLink.PetName,petName);
		strcpy(oneChatLinkData.Data.m_PetLink.OwnerName,ownerName);
		oneChatLinkData.Type = CHAT_LINK_PET;

		tagUIMsgBody UI_Msg;
		UI_Msg.ChatSetLink.m_nLinkType	= CHAT_LINK_PET;
		UI_Msg.ChatSetLink.m_SecondarySkillLink	= oneChatLinkData.Data.m_SecondarySkillLink;

		char szItemLinkName[MAX_CLI_LINKNAME];
		sprintf( szItemLinkName, "<%s>",petName );
		strncpy( UI_Msg.ChatSetLink.m_szLinkName, szItemLinkName, MAX_CLI_LINKNAME );
		tagCacheDisplayTextInfo oneCacheDisplayText;
		oneCacheDisplayText.m_strLinkName	= szItemLinkName;
		oneCacheDisplayText.m_nLinkType		= oneChatLinkData.Type;
		m_allCacheDisplayText.push_back( oneCacheDisplayText );

		//向服务端发送的格式
		sprintf( szItemLinkName, "#L<%s>@@%d#n", petName, PET_LINK );
		strncpy( UI_Msg.ChatSetLink.m_szLinkName2ServerRich, szItemLinkName, MAX_CLI_LINKNAME );

		//向聊天窗口发送的格式
		int nLinkId = getSelfLinkCount() * 10 + PET_LINK;
		sprintf( szItemLinkName, "#L<%s>@@%d#n",  petName, nLinkId);
		memset( UI_Msg.ChatSetLink.m_szLinkName2ClientRich, 0, MAX_CLI_LINKNAME );
		strncpy( UI_Msg.ChatSetLink.m_szLinkName2ClientRich, szItemLinkName, strlen(szItemLinkName)+1 );

		GameUIMsg::getSingleton().setUIMsg( UI_Msg );
		GameManager::getSingleton().SendUIEvent( "GE_SET_CHATLINK" );
		updateNotUsedLinkCount();
		++m_ChatLink.m_nLinkNum;
	}
}

tagItemLink_ToLua& ChatManager::getItemLink( int linkId )
{
	static tagItemLink_ToLua itemLink;
	itemLink.m_nItemId = 0;
	std::map<int, tagChatLink_ToLua>::iterator iter = m_ChatLinkMap.find( linkId );
	if( iter != m_ChatLinkMap.end() )
	{
		for( int i = 0 ; i < MAX_CHAT_LINK_NUM ; i++ )
		{
			if( iter->second.m_LinkData[i].Type == CHAT_LINK_ITEM )
			{
				return iter->second.m_LinkData[i].Data.m_ItemLink;
			}
		}
	}
	return itemLink;
}

const tagArmLink_ToLua& ChatManager::getArmLink( int linkId )
{
	static tagArmLink_ToLua armLink;
	armLink.m_arm.DefID = 0;
	std::map<int, tagChatLink_ToLua>::iterator iter = m_ChatLinkMap.find( linkId );
	if( iter != m_ChatLinkMap.end() )
	{
		for( int i = 0 ; i < MAX_CHAT_LINK_NUM ; i++ )
		{
			if( iter->second.m_LinkData[i].Type == CHAT_LINK_ARM )
			{
				return iter->second.m_LinkData[i].Data.m_RoleArmLink;
			}
		}
	}
	return armLink;

}

tagMissionLink_ToLua& ChatManager::getMissionLink( int linkId )
{
	static tagMissionLink_ToLua missionLink;
	missionLink.m_MissionId = 0;
	missionLink.m_nRecycleID= 0;

	std::map<int, tagChatLink_ToLua>::iterator iter = m_ChatLinkMap.find( linkId );
	if( iter != m_ChatLinkMap.end() )
	{
		tagChatLink_ToLua& oneChatLinkData = iter->second;
		for( int i = 0 ; i < MAX_CHAT_LINK_NUM; i ++ )
		{
			tagChatLinkData_ToLua& oneLinkData = oneChatLinkData.m_LinkData[i];
			if( oneLinkData.Type == CHAT_LINK_MISSION )
			{
				return oneLinkData.Data.m_MissionLink;
			}
		}
	}

	return missionLink;
}

tagPostionLink_ToLua& ChatManager::getPostionLink( int linkId )
{
	static tagPostionLink_ToLua postionLink;
	postionLink.m_nMapId = 0;

	std::map<int, tagChatLink_ToLua>::iterator iter = m_ChatLinkMap.find( linkId );
	if( iter == m_ChatLinkMap.end() )
	{
		return postionLink;
	}

	tagChatLink_ToLua& oneChatLinkDataWrap = iter->second;
	for( int i = 0 ; i < MAX_CHAT_LINK_NUM ; i ++ )
	{
		tagChatLinkData_ToLua& oneChatLinkData = oneChatLinkDataWrap.m_LinkData[i];
		if( oneChatLinkData.Type == CHAT_LINK_POSTION )
		{
			return oneChatLinkData.Data.m_PostionLink;
		}
	}

	return postionLink;
}

tagPostionLink_ToLua& ChatManager::getSystemLink( int linkId )
{
	static tagPostionLink_ToLua postionLink;
	postionLink.m_nMapId = 0;

	std::map<int, tagChatLink_ToLua>::iterator iter = m_ChatLinkMap.find( linkId );
	if( iter != m_ChatLinkMap.end() )
	{
		for( int i = 0 ; i < MAX_CHAT_LINK_NUM; i ++ )
		{
			if( iter->second.m_LinkData[i].Type == CHAT_LINK_SYSTEM )
			{
				return iter->second.m_LinkData[i].Data.m_SystemLink;
			}
		}
	}

	return postionLink;
}

tagSecondarySkillLink_ToLua& ChatManager::getSecondarySkillLink(int linkId)
{
	static tagSecondarySkillLink_ToLua secondaryLink;
	secondaryLink.SkillType = 0;

	std::map<int, tagChatLink_ToLua>::iterator iter = m_ChatLinkMap.find( linkId );
	if( iter != m_ChatLinkMap.end() )
	{
		for( int i = 0 ; i < MAX_CHAT_LINK_NUM; i ++ )
		{
			if( iter->second.m_LinkData[i].Type == CHAT_LINK_SECONDARY_SKILL )
			{
				return iter->second.m_LinkData[i].Data.m_SecondarySkillLink;
			}
		}
	}
	return secondaryLink;
}

tagPetLink_ToLua& ChatManager::getPetLink( int linkId )
{
	static tagPetLink_ToLua petLink;
	memset( &petLink.PetWid,0,sizeof(tagLong_ToLua) );
	std::map<int, tagChatLink_ToLua>::iterator iter = m_ChatLinkMap.find( linkId );
	if( iter != m_ChatLinkMap.end() )
	{
		for( int i = 0 ; i < MAX_CHAT_LINK_NUM; i ++ )
		{
			if( iter->second.m_LinkData[i].Type == CHAT_LINK_PET )
			{
				return iter->second.m_LinkData[i].Data.m_PetLink;
			}
		}
	}
	return petLink;
}

tagStoreLink_ToLua& ChatManager::getStoreLink( int linkId )
{
	static tagStoreLink_ToLua storeLink;
	storeLink.StoreID = 0;

	std::map<int, tagChatLink_ToLua>::iterator iter = m_ChatLinkMap.find( linkId );
	if( iter != m_ChatLinkMap.end() )
	{
		for( int i = 0 ; i < MAX_CHAT_LINK_NUM; i ++ )
		{
			if( iter->second.m_LinkData[i].Type == CHAT_LINK_STORE )
			{
				return iter->second.m_LinkData[i].Data.m_StoreLink;
			}
		}
	}
	return storeLink;
}

void ChatManager::deleteLink( int type )
{
	if( m_ChatLink.m_nLinkNum == 0 )
	{
		return;
	}

	for( int i = 0; i < MAX_CHAT_LINK_NUM;  ++i )
	{
		tagChatLinkData_ToLua& oneChatLinkData = m_ChatLink.m_LinkData[i];
		if( oneChatLinkData.Type == type )
		{
			for( int j = i; j < m_ChatLink.m_nLinkNum - 1; j++ )
			{
				m_ChatLink.m_LinkData[j] = m_ChatLink.m_LinkData[j+1]; 
			}
			ZeroMemory( &m_ChatLink.m_LinkData[--m_ChatLink.m_nLinkNum],sizeof(tagChatLinkData_ToLua) );
		}
	}
	removeCacheDisplayText( type );
}

void ChatManager::deleteMapLinkInfo( int type )
{
	for( int i = 0 ; i < m_ChatLink.m_nLinkNum ; ++i )
	{
		tagChatLinkData_ToLua& oneChatLinkData = m_ChatLink.m_LinkData[i];
		if( oneChatLinkData.Type == type )
		{
			for( int j = i; j < m_ChatLink.m_nLinkNum - 1; j++ )
			{
				m_ChatLink.m_LinkData[j] = m_ChatLink.m_LinkData[j+1]; 
			}
			ZeroMemory( &m_ChatLink.m_LinkData[--m_ChatLink.m_nLinkNum],sizeof(tagChatLinkData_ToLua) );
		}
	}
}

void ChatManager::removeCacheDisplayText( int nType )
{
	for ( std::vector<tagCacheDisplayTextInfo>::iterator iter = m_allCacheDisplayText.begin();
		iter != m_allCacheDisplayText.end(); ++iter )
	{
		const tagCacheDisplayTextInfo& oneCacheTextInfo = *iter;
		if ( oneCacheTextInfo.m_nLinkType == nType )
		{
			m_allCacheDisplayText.erase( iter );
			break;
		}
	}

}

const char* ChatManager::deleteLink( char* szContext, const char* szlink  )
{
	if( strlen(szContext) < strlen(szlink) ) return NULL;

	std::string szText = szContext;
	szContext = szContext + ( sizeof(szContext) - sizeof( szlink ) );
	if( strcmp( szContext, szlink ) == 0 )
	{
		strReplace( szText, szlink, ""  );
		return szText.c_str();
	}
	return NULL;
}

int	ChatManager::getNotUsedLinkCount()
{
	if ( m_ChatLink.m_nLinkNum > 0 )
	{
		++m_nLinkCount;
	}

	return m_nLinkCount;
}

void ChatManager::updateNotUsedLinkCount()
{
	if ( m_ChatLink.m_nLinkNum == 0 )
	{
		m_nSelfPrepareLinkCount = m_nLinkCount;
		++m_nLinkCount;
	}
}

int	ChatManager::getSelfLinkCount()
{
	if ( m_ChatLink.m_nLinkNum > 0 )
	{
		return m_nSelfPrepareLinkCount;
	}

	return m_nLinkCount;
}

void ChatManager::requestChat( int channel, const char* szName, const char* szContent, int nListType, 
							  int nGrid, char nShowUIType ,int online)
{
	// 执行GM指令
	if( ActorManager::getSingleton().getMainPlayer() != NULL &&
		ActorManager::getSingleton().getMainPlayer()->isHaveGMPerm() )
	{
		if( strstr( szContent, "//cmdpg" ) != NULL )
		{
			char szBuf[256];
			sprintf( szBuf, "CallGMPackage(\"%s\")", szContent );
			GameManager::getSingleton().getScriptVM()->callString( szBuf );	
			return;		
		}
		if( strstr( szContent, "//cmdAuto" ) != NULL )
		{
			char szBuf[256];
			sprintf( szBuf, "CallGMAutoQuest(\"%s\")", szContent );
			GameManager::getSingleton().getScriptVM()->callString( szBuf );	
			return;		
		}
		//add for resource
		if(strcmp(szContent,"//checkdb") == 0 )
		{
			CResourceCheck::getSingleton().loadDB();
			CResourceCheck::getSingleton().startCheck();
		}
	}
	tagCSPkgBody msgbody;
	memset( &msgbody, 0 ,sizeof(tagCSPkgBody) );
	CSCHATREQ& chatReqData	= msgbody.ChatReq;
	chatReqData.Type	= channel;
	chatReqData.Res		= nShowUIType;
	chatReqData.OnlineType = online;
	if ( channel == CL_CHAT_ITEM || channel == CL_CHAT_SPAN )
	{
		chatReqData.ListType	= nListType;
		chatReqData.GridIdx		= nGrid;
	}

	if( chatReqData.Type == CL_CHAT_FRIEND || chatReqData.Type == CL_CHAT_PRIVATE )
	{
		strncpy( chatReqData.ToName, szName, sizeof(chatReqData.ToName) );

		chatReqData.OnlineType =GameFriendManager::getSingleton().getPlayerOnlineTypeByName( chatReqData.ToName );

	}
	else if ( chatReqData.Type == CL_CHAT_WINDOW )
	{
		strncpy( chatReqData.ToName, szName, sizeof(chatReqData.ToName) );
	}
	else
	{
		chatReqData.ToName[0] = 0;
	}
	strncpy( chatReqData.Content, szContent, sizeof(chatReqData.Content) );

	if( m_ChatLink.m_nLinkNum > 0 )
	{
		int nLinkLen = 0;
		int nSize = sizeof(m_ChatLink.m_nLinkNum);
		memcpy( chatReqData.LinkContent, &m_ChatLink.m_nLinkNum, nSize );
		nLinkLen += nSize;
		for( int i = 0 ; i < m_ChatLink.m_nLinkNum ; ++i )
		{
			nSize = getLinkContentLength( m_ChatLink.m_LinkData[i] );
			memcpy( chatReqData.LinkContent + nLinkLen, &m_ChatLink.m_LinkData[i], nSize );
			nLinkLen += nSize;
		}
		chatReqData.LinkLen = nLinkLen;
		m_ChatLinkMap[m_nSelfPrepareLinkCount] = m_ChatLink;
	}
	else
	{
		chatReqData.LinkLen = 0;
		m_ChatLinkMap[m_nLinkCount++] = m_ChatLink;
	}
	SendGameServerMsg( CHAT_REQ, msgbody );

	ZeroMemory( &m_ChatLink, sizeof(tagChatLink_ToLua) );
	m_allCacheDisplayText.clear();
}

tagFriendMsgGroup_ToLua ChatManager::getGroupMsg()
{
	static_assert( sizeof( tagFriendMsgGroup_ToLua ) == sizeof( tagFriendMsgGroup ) );
	memset( &m_Group_msg, 0, sizeof( tagFriendMsgGroup_ToLua ) );
	return m_Group_msg;
}

//群发消息
void ChatManager::requestMutilChat( tagFriendMsgGroup_ToLua msg, const char* szContent )
{
	tagCSPkgBody msgbody;
	memset( &msgbody, 0, sizeof(tagCSPkgBody) );	
	msgbody.RelationClt.OpType = RELATION_OP_TYPE_MSG_GROUP;
	msgbody.RelationClt.RelationCltData.FriendMsgGroup.iGroup = msg.iGroup;
	for ( int i = 0; i < msg.iGroup; i++ )
	{
		msgbody.RelationClt.RelationCltData.FriendMsgGroup.Groups[i] = msg.Groups[i];
	}
	msgbody.RelationClt.RelationCltData.FriendMsgGroup.iMud = msg.iMud;
	for ( int i = 0; i < msg.iMud; i++ )
	{
		memcpy( &msgbody.RelationClt.RelationCltData.FriendMsgGroup.Muds[i], &msg.Muds[i], sizeof( tagLong_ToLua ) );
	}
	CSCHATREQ& chatReq = msgbody.RelationClt.RelationCltData.FriendMsgGroup.ChatReq;
	chatReq.Type = CL_CHAT_FRIEND;
	strncpy( chatReq.Content, szContent , sizeof(chatReq.Content) );
	if( m_ChatLink.m_nLinkNum > 0 )
	{
		int nLinkLen = 0;
		int nSize = sizeof(m_ChatLink.m_nLinkNum);
		memcpy( chatReq.LinkContent, &m_ChatLink.m_nLinkNum, nSize );
		nLinkLen += nSize;
		for( int i = 0 ; i < m_ChatLink.m_nLinkNum ; ++i )
		{
			nSize = getLinkContentLength( m_ChatLink.m_LinkData[i] );
			memcpy( chatReq.LinkContent + nLinkLen, &m_ChatLink.m_LinkData[i], nSize );
			nLinkLen += nSize;
		}
		chatReq.LinkLen = nLinkLen;
		m_ChatLinkMap[m_nSelfPrepareLinkCount] = m_ChatLink;
	}
	else
	{
		chatReq.LinkLen = 0;
		m_ChatLinkMap[m_nLinkCount++] = m_ChatLink;
	}
	SendGameServerMsg( RELATION_CLT, msgbody );

	ZeroMemory( &m_ChatLink, sizeof(tagChatLink_ToLua) );
	m_allCacheDisplayText.clear();
}

void ChatManager::ShowSysMsg( const char* szText )
{
	tagUIMsgBody UI_Msg;
	strncpy( UI_Msg.SysMsg.Msg, szText, SYS_MSG_LEN );
	UI_Msg.SysMsg.Type = SYS_TIP;
	GameUIMsg::getSingleton().setUIMsg( UI_Msg );
	GameManager::getSingleton().SendUIEvent( "GE_UPDATE_SYSMSG" );
}

std::string& ChatManager::strReplace( std::string& szText, const char* src, const char* des  )
{
	size_t nPos = szText.find(src);
	if ( nPos != -1)
	{
		szText.replace(nPos, strlen(src), des);
	}
	return szText;
}

int ChatManager::getNumCacheDisplayText()
{
	return m_allCacheDisplayText.size();
}

const tagCacheDisplayTextInfo& ChatManager::getCacheDisplayText( int i )
{
	return m_allCacheDisplayText[i];
}

bool ChatManager::deleteLinkName( std::string strText, int nCurSorPos, int nSelSorPos, int nOpCode )
{
	if ( !m_ChatLink.m_nLinkNum )
	{
		return false;
	}

	std::string::size_type selBegin	= 0;
	std::string::size_type selEnd	= 0;
	if ( nCurSorPos == nSelSorPos )
	{
		selBegin = selEnd = nCurSorPos;
	}
	else
	{
		selBegin	= min( nCurSorPos, nSelSorPos );
		selEnd		= max( nCurSorPos, nSelSorPos );
	}

	std::string strSelectText = strText.substr( selBegin, max( 1, selBegin - selEnd ) );
	cout << "strSelectText = " << strSelectText << endl;
	for ( std::vector<tagCacheDisplayTextInfo>::iterator iter = m_allCacheDisplayText.begin();
		iter != m_allCacheDisplayText.end(); )
	{
		const tagCacheDisplayTextInfo& oneCacheTextInfo = *iter;
		std::string::size_type nCurLinkBeginPos = strText.find( oneCacheTextInfo.m_strLinkName, 0 );
		if ( nCurLinkBeginPos == string::npos )
		{
			++iter;
			continue;
		}

		std::string::size_type nCurLinkEndPos = nCurLinkBeginPos + oneCacheTextInfo.m_strLinkName.length();
		string strCurLinkText = strText.substr( nCurLinkBeginPos, nCurLinkEndPos - nCurLinkBeginPos );
		cout << "strCurLinkText = " << strCurLinkText << endl;
		if (  ( selBegin >= nCurLinkBeginPos && selBegin <= nCurLinkEndPos ) || 
			( selEnd >= nCurLinkBeginPos && selEnd <= nCurLinkEndPos ) )
		{
			cout << "oneCacheTextInfo.m_nLinkType = " << oneCacheTextInfo.m_nLinkType << endl;
			int nDeleteLinkType = oneCacheTextInfo.m_nLinkType;
			deleteMapLinkInfo( oneCacheTextInfo.m_nLinkType );
			iter = m_allCacheDisplayText.erase( iter );

			char buffer[256];
			sprintf( buffer, "DeletChatInputLinkType(%d,%d)", nDeleteLinkType, nOpCode );
			GameManager::getSingleton().getScriptVM()->callString( buffer );
			continue;
		}

		++iter;
	}

	return false;
}

bool ChatManager::validateLinkName( std::string& strText )
{
	bool isChangeLink = false;
	for ( std::vector<tagCacheDisplayTextInfo>::iterator iter = m_allCacheDisplayText.begin();
		iter != m_allCacheDisplayText.end(); )
	{
		const tagCacheDisplayTextInfo& oneCacheTextInfo = *iter;
		std::string::size_type nCurLinkBeginPos = strText.find( oneCacheTextInfo.m_strLinkName, 0 );

		if ( nCurLinkBeginPos != string::npos )
		{
			++iter;
			continue;
		}

		//strReplace( strText, oneCacheTextInfo.m_strLinkName.c_str(), "" );
		deleteMapLinkInfo( oneCacheTextInfo.m_nLinkType );
		iter = m_allCacheDisplayText.erase( iter );
		isChangeLink = true;
	}

	return isChangeLink;
}

std::string& ChatManager::strTrim( std::string& strTex )
{
	const std::string& strSpace = " ";
	strTex.erase( strTex.find_last_not_of( strSpace ) + 1 );
	strTex.erase( 0, strTex.find_first_not_of( strSpace ) );      
	return strTex;   
}

std::string&  ChatManager::linkName2ItemName( const char *szlinkName )
{
	if( std::string( szlinkName ).empty() )
	{
		m_szItemName = "";
	}else{
		m_szItemName = std::string( szlinkName ).substr( 1, strlen(szlinkName) - 2 );
	}

	return m_szItemName;
}

void ChatManager::showMidTip( EMID_MESSAGE type )
{
	const unsigned int INT = 1000;
	static unsigned int lastTime = 0;
	static int lastType = 0;
	unsigned int now = timeGetTime();
	if (lastType == type && lastTime + INT > now)
	{
		return;
	}
	lastTime = now;
	lastType = type;
	tagUIMsgBody UI_Msg;
	UI_Msg.MidTipsMsg.m_nMsgType = type;
	GameUIMsg::getSingleton().setUIMsg( UI_Msg );
	GameManager::getSingleton().SendUIEvent( "GE_MIDTIPS_MSG" );
}

// 获取指定下标的公告数据
tagBulletinEntry_ToLua* ChatManager::getBulletinEntry( int nIndex )
{
	static_assert( sizeof(tagBulletinEntry_ToLua) == sizeof(tagBulletinEntry) );
	assert( nIndex < (int)m_vectBulletin.size() );
	return (tagBulletinEntry_ToLua*)&m_vectBulletin[nIndex];
}

void ChatManager::removeBulletinEntry( int nID )
{
	std::vector<BULLETINENTRY>::iterator iter = m_vectBulletin.begin();
	while( iter != m_vectBulletin.end() )
	{
		if( iter->ID == nID )
		{
			m_vectBulletin.erase( iter );
			return;
		}
		iter ++;
	}
}

void ChatManager::resetChatManager()
{
	m_vectBulletin.clear();
	m_vGmBulletin.clear();
	m_ChatLinkMap.clear();
	m_nLinkCount = 0;
	m_nSelfPrepareLinkCount = 0;
}

// 设置怪物技能发言
void ChatManager::setMonsterSkillSpeech( int nMonsterResId, int nMonsterMemId, int nConditionType, int nSkillId )
{
	const tagMonsterSpeechDef* pMonsterSpeechDef = DBDataManager::getSingleton().getMonsterSpeechDef( nMonsterResId);
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();

	if( pMainPlayer != NULL && pMonsterSpeechDef != NULL )
	{
		for( int i = 0; i < MAX_MON_SKILL_SPEECH; i++ )
		{
			if( pMonsterSpeechDef->MonSkillSpeech[i].SkillID == nSkillId 
				&& pMonsterSpeechDef->MonSkillSpeech[i].ConditionType == nConditionType  )
			{
				char szScriptFun[1024];
				sprintf( szScriptFun, 
					"MonsterSpeak( %d, %d, [===[%s ]===], %d, [===[%s]===],%d, %d )", 
					nMonsterResId, 
					nMonsterMemId, 
					pMonsterSpeechDef->MonSkillSpeech[i].Desc,
					pMonsterSpeechDef->MonSkillSpeech[i].MsgType,
					pMainPlayer->getName(),
					pMainPlayer->getRace(),
					pMainPlayer->getGender()
					);

				GameManager::getSingleton().getScriptVM()->callString( szScriptFun );
				break;
			}
		}
	}
}

// 设置怪物死亡发言
void ChatManager::setMonsterDeadSpeech( int pObjResId, int pObjId, const char* szAttackName, int nAttackRace, int nAttackGender )
{
	const tagMonsterSpeechDef* pMonsterSpeechDef = DBDataManager::getSingleton().getMonsterSpeechDef( pObjResId );
	if( pMonsterSpeechDef != NULL )
	{
		for( int i = 0; i < MAX_MON_ACT_SPEECH; i++  )
		{
			int nRate = rand()%100;
			if( pMonsterSpeechDef->MonActSpeech[i].ConditionVal > nRate 
				&& pMonsterSpeechDef->MonActSpeech[i].ConditionType == SPEECH_MON_DEAD )
			{
				char szScriptFun[1024];
				// SYS_WHOOP
				sprintf( szScriptFun, 
					"MonsterSpeak( %d, %d, [===[%s ]===], %d, [===[%s]===],%d, %d )", 
					pObjResId, 
					pObjId, 
					pMonsterSpeechDef->MonActSpeech[i].Desc,
					SYS_CHAT,
					szAttackName,
					nAttackRace,
					nAttackGender
					);

				ActorManager::getSingleton().getScriptVM()->callString( szScriptFun );
			}
		}
	}
}

void ChatManager::setMonsterSkillAttackSpeech( int pObjResId, int pObjId, const char* szAttackName, int nConditionType, int nSkillId )
{
	const tagMonsterSpeechDef* pMonsterSpeechDef = DBDataManager::getSingleton().getMonsterSpeechDef( pObjResId );
	if(  pMonsterSpeechDef != NULL )
	{
		for( int i = 0; i < MAX_MON_SKILL_SPEECH; i++ )
		{
			if( pMonsterSpeechDef->MonSkillSpeech[i].SkillID == nSkillId 
				&& pMonsterSpeechDef->MonSkillSpeech[i].ConditionType == nConditionType  )
			{
				char szScriptFun[1024];
				sprintf( szScriptFun, 
					"MonsterSpeak( %d, %d, [===[%s ]===], %d, [===[%s]===],%d, %d )", 
					pObjResId, 
					pObjId, 
					pMonsterSpeechDef->MonSkillSpeech[i].Desc,
					pMonsterSpeechDef->MonSkillSpeech[i].MsgType,
					szAttackName,
					0,
					0);

				GameManager::getSingleton().getScriptVM()->callString( szScriptFun );
				break;
			}
		}
	}
}


Item* ChatManager::getItem(const tagRoleArm_ToLua* arm)
{
	static_assert(sizeof( tagRoleArm_ToLua ) == sizeof( tagRoleArm ));
	if (arm == NULL )
	{
		return NULL;
	}
	m_LinkItem.releaseItem();
	m_LinkItem.m_ItemType = ITEM_SLOTTYPE_AMM;
	m_LinkItem.m_ItemData.m_Arm = *(ROLEARM*)arm;
	m_LinkItem.InitItemName();
	return & m_LinkItem;
}

Item* ChatManager::getSparItem(const tagItemLink_ToLua* item)
{
	if (item == NULL )
	{
		return NULL;
	}
	m_LinkItem.releaseItem();
	m_LinkItem.m_ItemType = ITEM_SLOTTYPE_ITEM;
	m_LinkItem.m_ItemData.m_Item.DefID = item->m_nItemId;
	m_LinkItem.m_ItemData.m_Item.InstVal1 = item->InstVal1;
	m_LinkItem.m_ItemData.m_Item.InstVal2 = item->InstVal2;
	m_LinkItem.m_ItemData.m_Item.InstVal3 = item->InstVal3;
	m_LinkItem.m_ItemData.m_Item.Flag = item->Flag;
	m_LinkItem.InitItemName();
	return & m_LinkItem;
}

bool ChatManager::isContentHaveChatType( int nChatType, const char* szChatText  )
{
	std::string strTmpLinkText	= szChatText;
	std::string::size_type nPos = strTmpLinkText.find("@@");
	int nLinkType = -1;
	while ( nPos != std::string::npos )
	{
		std::string strOneLinkSymbol	= strTmpLinkText.substr( nPos+2,1);
		nLinkType						= atoi( strOneLinkSymbol.c_str() );
		if ( nLinkType == nChatType )
		{
			return true;
		}

		nPos = strTmpLinkText.find("@@", nPos + 2 );
	}

	return false;
}

int ChatManager::getChatType( const char* szChatText )
{
	std::string szTmp = szChatText;
	size_t nPos = szTmp.find("@@");
	int nLinkType = -1;
	if ( nPos != -1)
	{
		szTmp = szTmp.substr( nPos+2,1);
		return atoi( szTmp.c_str() );
	}
	return -1;
}

void ChatManager::setLinkedItemLink(int nLinkId,int nLinkCountId)
{
	if (nLinkId == ITEM_LINK )
	{
		tagItemLink_ToLua& itemLink = getItemLink(nLinkCountId);
		if (getItemType(itemLink.m_nItemId) == ITEM_SLOTTYPE_ITEM )
		{
			setItemLink(itemLink);
		}
	}else if( nLinkId == ARM_LINK )
	{
		const tagArmLink_ToLua& armLink = getArmLink(nLinkCountId);
		if( getItemType(armLink.m_arm.DefID) == ITEM_SLOTTYPE_AMM )
		{
			Item* itemInfo = getItem(&armLink.m_arm);
			setItemLink(*itemInfo);
		}
	}else if( nLinkId == PET_LINK )
	{
		tagPetLink_ToLua& petLink = getPetLink( nLinkCountId );
		setPetLink(petLink.PetWid.nLow,petLink.PetWid.nHeight,petLink.PetName,petLink.OwnerName);
	}else if( nLinkId == SECONDARY_SKILL_LINK )
	{
		tagSecondarySkillLink_ToLua& secondaryLink = getSecondarySkillLink( nLinkCountId );
		setSecondarySkillLink(secondaryLink);
	}
}

void ChatManager::setChatConfigData(char* data,unsigned int len)
{
	GameUserConfig::getSingleton().setConfigData(CONFIGTYPE_CHAT,data,len + 1 );
}
char* ChatManager::getChatConfigData()
{
	return (char*) GameUserConfig::getSingleton().getConfigData(CONFIGTYPE_CHAT);
}

void ChatManager::addMakeSucessMsg(int itemID,const char* msg)
{
	tagUIMsgBody UI_Msg;
	strcpy(UI_Msg.SysMsg.Msg,msg);
	UI_Msg.SysMsg.Type = SYS_MSG_CHAT_TIP;
	if( itemID != 0 )
	{
		char bufLinkMsgNew[CLT_MSG_LEN];
		tagChatLink_ToLua chatLink;
		chatLink.m_nLinkNum = 1;
		chatLink.m_LinkData[0].Type = CHAT_LINK_ITEM;
		chatLink.m_LinkData[0].Data.m_ItemLink.m_nItemId = itemID;

		ITEMDEF itemDefine = DBDataManager::getSingleton().getItemDef( itemID );
		std::string szItemName = itemDefine.Name;
		memset(bufLinkMsgNew, 0, sizeof(bufLinkMsgNew));
		int nLinkId = m_nLinkCount * 10 + ITEM_LINK;
		sprintf( bufLinkMsgNew, "#L#c%s<%s>@@%d#n", getItemColor(itemDefine.Important,itemDefine.SuitSetID).c_str(), szItemName.c_str(), nLinkId );
		std::string szText = UI_Msg.SysMsg.Msg;
		strReplace( szText, STR_PARSE_LINK.c_str(), bufLinkMsgNew );
		memset(  UI_Msg.SysMsg.Msg, 0, CLT_MSG_LEN );
		strncpy( UI_Msg.SysMsg.Msg, szText.c_str(), szText.length()+1 );
		m_ChatLinkMap[m_nLinkCount++]	= chatLink;
	}
	GameUIMsg::getSingleton().setUIMsg( UI_Msg );
	GameManager::getSingleton().SendUIEvent( "GE_UPDATE_SYSMSG" );
}

void ChatManager::requestSendActionChat(int channel, const char* szName, const char* szContent)
{
	tagCSPkgBody msgbody;
	memset(&msgbody,0,sizeof(tagCSPkgBody));
	CSCHATREQ& chatReqData	= msgbody.ChatReq;
	chatReqData.Type = channel;
	strncpy( chatReqData.Content, szContent, sizeof(chatReqData.Content) );
	chatReqData.LinkLen = 0;
	chatReqData.Res		= CLIENT_CHAT_NOT_SHOW_IN_CHAT_FRAME;
	memset( chatReqData.LinkContent, 0, sizeof( tagChatLink_ToLua ) );
	SendGameServerMsg( CHAT_REQ, msgbody );
}

void ChatManager::parseItemLink( char* szContext )
{
	char buf[BULLETIN_MSG_LEN];
	int itemID = 0;
	sscanf(szContext,"%[^[][#L<%[^>]>@@%d#n]",buf,buf,&itemID);
	std::string szText( szContext );

	if( itemID > 0 )
	{
		tagChatLink_ToLua chatLink;
		chatLink.m_nLinkNum = 1;
		chatLink.m_LinkData[0].Type = ITEM_LINK;
		chatLink.m_LinkData[0].Data.m_ItemLink.m_nItemId = itemID;
		chatLink.m_LinkData[0].Data.m_ItemLink.m_nItemDur = 0;
		int nLinkId = m_nLinkCount * 10 + ITEM_LINK;
		char bufLinkMsgNew[256];
		char bufLinkMsgOld[256];
		sprintf( bufLinkMsgOld,strkeyWorld.c_str(),itemID );
		sprintf( bufLinkMsgNew, strkeyWorld.c_str(), nLinkId );
		strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
		strncpy( szContext, szText.c_str(), szText.length()+1 );		
		m_ChatLinkMap[m_nLinkCount++] = chatLink;
	}	
}

void ChatManager::ClearLinkData()
{
	ZeroMemory( &m_ChatLink, sizeof(tagChatLink_ToLua) );
	m_ChatLink.m_nLinkNum = 0;
	m_allCacheDisplayText.clear();
}

// 设置综合频道
void ChatManager::savePingBiZongHeChannelsOp()
{
	GameClientOpt::getSingleton().setClientOpt( SAVE_OPT_LAST_PING_BI_ZONG_HE_PIN_DAO, m_szLasetPingBiZongHeChatChannels );
}

void ChatManager::setZongHeChannelPingBiOp( int nChannelType, bool isPingBi )
{
	assert( nChannelType < CL_CHAT_END );
	m_szLasetPingBiZongHeChatChannels[nChannelType] = ( isPingBi ? 1 : 0 );
}

bool ChatManager::isZongHeChannelPingBi( int nChannelType )
{
	assert( nChannelType < CL_CHAT_END );
	return m_szLasetPingBiZongHeChatChannels[nChannelType] == 1;
}

// 设置世界频道
void ChatManager::savePingBiWorldChannelsOp()
{
	GameClientOpt::getSingleton().setClientOpt( SAVE_OPT_LAST_PING_BI_WORLD_PIN_DAO, m_szLasetPingBiWorldChatChannels );
}

void ChatManager::setWorldChannelPingBiOp( int nChannelType, bool isPingBi )
{
	assert( nChannelType < CL_CHAT_END );
	m_szLasetPingBiWorldChatChannels[nChannelType] = ( isPingBi ? 1 : 0 );
}

bool ChatManager::isWorldChannelPingBi( int nChannelType )
{
	assert( nChannelType < CL_CHAT_END );
	return m_szLasetPingBiWorldChatChannels[nChannelType] == 1;
}

// 设置公会频道
void ChatManager::savePingBiClanChannelsOp()
{
	GameClientOpt::getSingleton().setClientOpt( SAVE_OPT_LAST_PING_BI_CLAN_PIN_DAO, m_szLasetPingBiClanChatChannels );
}

void ChatManager::setClanChannelPingBiOp( int nChannelType, bool isPingBi )
{
	assert( nChannelType < CL_CHAT_END );
	m_szLasetPingBiClanChatChannels[nChannelType] = ( isPingBi ? 1 : 0 );
}

bool ChatManager::isClanChannelPingBi( int nChannelType )
{
	assert( nChannelType < CL_CHAT_END );
	return m_szLasetPingBiClanChatChannels[nChannelType] == 1;
}

// 设置队伍频道
void ChatManager::savePingBiTeamChannelsOp()
{
	GameClientOpt::getSingleton().setClientOpt( SAVE_OPT_LAST_PING_BI_TEAM_PIN_DAO, m_szLasetPingBiTeamChatChannels );
}

void ChatManager::setTeamChannelPingBiOp( int nChannelType, bool isPingBi )
{
	assert( nChannelType < CL_CHAT_END );
	m_szLasetPingBiTeamChatChannels[nChannelType] = ( isPingBi ? 1 : 0 );
}

bool ChatManager::isTeamChannelPingBi( int nChannelType )
{
	assert( nChannelType < CL_CHAT_END );
	return m_szLasetPingBiTeamChatChannels[nChannelType] == 1;
}

// 设置附近频道
void ChatManager::savePingBiNearChannelsOp()
{
	GameClientOpt::getSingleton().setClientOpt( SAVE_OPT_LAST_PING_BI_NEAR_PIN_DAO, m_szLasetPingBiNearChatChannels );
}

void ChatManager::setNearChannelPingBiOp( int nChannelType, bool isPingBi )
{
	assert( nChannelType < CL_CHAT_END );
	m_szLasetPingBiNearChatChannels[nChannelType] = ( isPingBi ? 1 : 0 );
}

bool ChatManager::isNearChannelPingBi( int nChannelType )
{
	assert( nChannelType < CL_CHAT_END );
	return m_szLasetPingBiNearChatChannels[nChannelType] == 1;
}

// 设置私聊频道
void ChatManager::savePingBiPrivateChannelsOp()
{
	GameClientOpt::getSingleton().setClientOpt( SAVE_OPT_LAST_PING_BI_PRIVATE_PIN_DAO, m_szLasetPingBiPrivateChatChannels );
}

void ChatManager::setPrivateChannelPingBiOp( int nChannelType, bool isPingBi )
{
	assert( nChannelType < CL_CHAT_END );
	m_szLasetPingBiPrivateChatChannels[nChannelType] = ( isPingBi ? 1 : 0 );
}

bool ChatManager::isPrivateChannelPingBi( int nChannelType )
{
	assert( nChannelType < CL_CHAT_END );
	return m_szLasetPingBiPrivateChatChannels[nChannelType] == 1;
}

char* ChatManager::getLastPingBiZongHeChannelsOp()
{
	return m_szLasetPingBiZongHeChatChannels;
}

char* ChatManager::getLastPingBiWorldChannelsOp()
{
	return m_szLasetPingBiWorldChatChannels;
}

char* ChatManager::getLastPingBiClanChannelsOp()
{
	return m_szLasetPingBiClanChatChannels;
}

char* ChatManager::getLastPingBiTeamChannelsOp()
{
	return m_szLasetPingBiTeamChatChannels;
}

char* ChatManager::getLastPingBiNearChannelsOp()
{
	return m_szLasetPingBiNearChatChannels;
}

char* ChatManager::getLastPingBiPrivateChannelsOp()
{
	return m_szLasetPingBiPrivateChatChannels;
}

void ChatManager::OnGetChatReq( const CSCHATREQ& svrChatReqData )
{
	//static_assert( sizeof( tagChatReq ) == sizeof( CSCHATREQ ) );

	ActorManager& actorMgr		= ActorManager::getSingleton();
	MainGamePlayer* pMainPlayer = actorMgr.getMainPlayer();
	if( svrChatReqData.Res != CLIENT_CHAT_NOT_SHOW_IN_CHAT_FRAME && svrChatReqData.Type == CHAT_SAY && 
		actorMgr.getGameOpt( GAME_OPT_SHOW_OTHERPLAYER_TALK_POP ) )
	{
		if( !strstr(svrChatReqData.Content,"#@!") )
		{
			//如果是发送伤害统计就不显示pop
			//需要在pop 中添加超链接信息			

			g_pFrameMgr->addPopWindow( "ChatPop", pMainPlayer->getName(), 
				svrChatReqData.Content, -100, -100, 8, "PopBackArrowTexture", true, 0, 0 );
		}
	}

	tagUIMsgBody	UI_Msg;
	tagChatReq& chatReqData = UI_Msg.ChatReq;
	strncpy( chatReqData.Content, svrChatReqData.Content, CLT_MSG_LEN );
	chatReqData.GridIdx	= svrChatReqData.GridIdx;

	memcpy( chatReqData.LinkContent, svrChatReqData.LinkContent, MAX_LINK );
	chatReqData.LinkLen		= svrChatReqData.LinkLen;
	chatReqData.ListType	= svrChatReqData.ListType;
	chatReqData.OnlineType	= svrChatReqData.OnlineType;
	chatReqData.Res			= svrChatReqData.Res;
	strncpy( chatReqData.ToName, svrChatReqData.ToName, NAME_LEN );
	chatReqData.Type		= svrChatReqData.Type;
	chatReqData.isVip		= ( ( svrChatReqData.Vip & ROLE_VIP_FLAG ) > 0 );
	tagChatLink_ToLua chatLink ;
	memset( &chatLink, 0, sizeof(tagChatLink_ToLua) );
	if( svrChatReqData.LinkLen > 0 )
	{
		InitChatLinkData( chatLink, svrChatReqData.LinkContent, svrChatReqData.LinkLen );
	}
	parseTextLink( chatLink, chatReqData.Content, CLT_MSG_LEN );
	GameUIMsg::getSingleton().setUIMsg( UI_Msg );
	if ( svrChatReqData.Type == CL_CHAT_FRIEND )
	{
		GameManager::getSingleton().SendUIEvent( "GE_UPDATE_FRIENDCHATFRAME_BY_CLIENT" );
	}else if ( svrChatReqData.Type == CL_CHAT_WINDOW )
	{
		GameManager::getSingleton().SendUIEvent( "GE_UPDATE_WINDOWCHATFRAME_BY_CLIENT" );
	}else if( svrChatReqData.Type == CL_CHAT_AUTOTEAM )
	{
		GameManager::getSingleton().SendUIEvent( "GE_AUTOTEAM_GETMSG_BY_CLIENT" );
	}else
	{
		if ( svrChatReqData.Res != CLIENT_CHAT_NOT_SHOW_IN_CHAT_FRAME )
		{
			GameManager::getSingleton().SendUIEvent( "GE_UPDATE_CHATFRAME_BY_CLIENT" );
		}
	}
}

bool ChatManager::isLinkTypeInPrepareState( int nDestType )
{
	if( m_ChatLink.m_nLinkNum >= MAX_CHAT_LINK_NUM )
	{
		showMidTip( MID_CHATLINK_MAX_NUM );
		return true;
	}

	for ( int i = 0; i < m_ChatLink.m_nLinkNum; ++i )
	{
		tagChatLinkData_ToLua& oneChatLinkData = m_ChatLink.m_LinkData[i];
		if( oneChatLinkData.Type == nDestType )
		{
			if( nDestType > 0 && nDestType <= CHAT_LINK_PET  )
			{
				showMidTip( MID_CHATLINK_SAME_EXIST );
			}
			return true;
		}
	}

	return false;
}

void ChatManager::Update()
{
	int sec = getServerTime().sec;
	std::vector<GmBulletinData>::iterator it = m_vGmBulletin.begin();
	for( ; it != m_vGmBulletin.end() ; it ++ )
	{
		if( sec >= it->ShowTime )
		{
			GameManager::getSingleton().getScriptVM()->callFunction("ShowGMMessagBox","u[tagBulletinEntry_ToLua]",
				&(it->bullet) );
			it->ShowTime = sec + it->bullet.RollInterval;
		}
	}
}

void ChatManager::InitChatLinkData( tagChatLink_ToLua& chatLink,const unsigned char* LinkContent, unsigned short LinkLen )
{
	int nOffset = 0;
	int nNum = *(int*)LinkContent;
	nOffset += 4;
	for( int i = 0; i < nNum; ++i )
	{
		chatLink.m_LinkData[i].Type = *(int*)( LinkContent + nOffset );
		int nSize = getLinkContentLength( chatLink.m_LinkData[i] );
		memcpy( &chatLink.m_LinkData[i], (LinkContent + nOffset),nSize );
		nOffset += nSize;
	}
	chatLink.m_nLinkNum = nNum;
}

int ChatManager::getLinkContentLength( const tagChatLinkData_ToLua& data )
{
	int size = sizeof(data.Type);
	switch( data.Type )
	{
	case CHAT_LINK_SYSTEM:
		{
			size += sizeof(tagPostionLink_ToLua);
		}
		break;
	case CHAT_LINK_ITEM:
		{
			size += sizeof(tagItemLink_ToLua);
		}
		break;
	case CHAT_LINK_ARM:
		{
			size += sizeof(tagArmLink_ToLua);
		}
		break;
	case CHAT_LINK_MISSION:
		{
			size += sizeof(tagMissionLink_ToLua);
		}
		break;
	case CHAT_LINK_POSTION:
		{
			size += sizeof(tagPostionLink_ToLua);
		}
		break;
	case CHAT_LINK_SECONDARY_SKILL:
		{
			size += sizeof(tagSecondarySkillLink_ToLua);
		}
		break;
	case CHAT_LINK_PET:
		{
			size += sizeof(tagPetLink_ToLua);
		}
		break;
	case CHAT_LINK_STORE:
		{
			size += sizeof(tagStoreLink_ToLua);
		}
		break;
	}
	return size;
}