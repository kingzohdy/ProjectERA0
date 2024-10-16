#include "OfflineChatManager.h"
#include "GameMsgPrint.h"
#include "GameDBData.h"
#include "Item.h"
#include "OgreScriptLuaVM.h"
#include "OfflineManager.h"
#include "RelationManager.h"

#include "../UILib/ui_gameui.h"
#include "../UILib/ui_common.h"
#include "../UILib/ui_framemgr.h"
using namespace std;

const std::string strkeyWorld = "@@%d#n";
const std::string strItemLinkKeyWorld = "@@0#n";
const std::string strTaskLinkColor = "FEBC42";
const std::string STR_PARSE_LINK = "@ITEMLINK@";

const std::string& getOfflineItemColor( int important )
{
	assert( important < ITEM_IMPORTANT_END );
	static std::string strImportantColor[ITEM_IMPORTANT_END] ={
		"9b9b9b",	 // 灰色(普通品质)
		"42a83d",	 // 绿色(优秀品质)
		"3d6da8",	 // 蓝色(精良品质)
		"972df9",	 // 紫色(极品品质)
	};
	return strImportantColor[important];
}


ChatManager* ChatManager::Create()
{
	ChatManager* p = new ChatManager;
	INetworkManager::getSingleton().RegisterMsgHandler( CHAT_RES, p );
	INetworkManager::getSingleton().RegisterMsgHandler( CHAT_REQ, p );
	INetworkManager::getSingleton().RegisterMsgHandler( SYS_MSG, p );
	return p;
}

ChatManager::ChatManager()
{
	m_nLinkCount = 0;

	static_assert( sizeof(tagRoleArm_ToLua) == sizeof(tagRoleArm) );
 	static_assert( sizeof(tagChatLink_ToLua) == sizeof(tagChatLink) );
	static_assert( sizeof(tagPostionLink_ToLua) == sizeof(tagPostionLink) );
 	static_assert( sizeof(tagItemLink_ToLua) == sizeof(tagItemLink) );
	static_assert( sizeof(tagMissionLink_ToLua) == sizeof(tagMissionLink) );
	static_assert( sizeof(tagSecondarySkillLink_ToLua) == sizeof(tagSecondarySkillLink) );
}

ChatManager::~ChatManager()
{
}

int ChatManager::HandleNetMsg( int msgid, const tagCSPkgBody& msgbody )
{
	switch( msgid )
	{
	case CHAT_RES:
		{
			const CSCHATRES& Msg	= msgbody.ChatRes;
			m_ChatRes				= msgbody.ChatRes;
			RelationManager::getSingleton().setOnlinePlayer( m_ChatRes.From, m_ChatRes.OnlineType);

			int nLinkType = getChatType( Msg.Content );
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
				for( int i = 0 ; i < MAX_CHAT_LINK_NUM ; i ++ )
				{
					switch( chatLink.m_LinkData[i].Type )
					{
					case CHAT_LINK_SYSTEM:
						// 系统指定位置
						{
							tagPostionLink_ToLua& pos = chatLink.m_LinkData[i].Data.m_SystemLink;
							if( nLinkType == SERVER_LINK && pos.m_nMapId != 0 )
							{
								const tagMapDef& mapDef = DBDataManager::getSingleton().getMapDef( pos.m_nMapId );
								memset(bufLinkMsgOld, 0, sizeof(bufLinkMsgOld));
								memset(bufLinkMsgNew, 0, sizeof(bufLinkMsgNew));
								int nLinkId = m_nLinkCount * 10 + SERVER_LINK;
								sprintf( bufLinkMsgNew, "#L<%s(%d,%d)>@@%d#n", mapDef.Name, pos.m_nX, pos.m_nY, nLinkId );
								sprintf( bufLinkMsgOld, "#L<%s(%d,%d)>@@%d#n", mapDef.Name, pos.m_nX, pos.m_nY, SERVER_LINK );

								std::string szText = Msg.Content;
								strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
								m_ChatLinkMap[m_nLinkCount++]	= chatLink; 
								return 0;
							}
						}
						break;
					case CHAT_LINK_ITEM:
						{
							tagItemLink_ToLua& item = chatLink.m_LinkData[i].Data.m_ItemLink;
							//物品
							if( nLinkType == ITEM_LINK && item.m_nItemId != 0 )
							{
								ITEMDEF itemDefine = DBDataManager::getSingleton().getItemDef( item.m_nItemId );

								memset(bufLinkMsgOld, 0, sizeof(bufLinkMsgOld));
								memset(bufLinkMsgNew, 0, sizeof(bufLinkMsgNew));
								int nLinkId = m_nLinkCount * 10 + ITEM_LINK;

								sprintf( bufLinkMsgNew, "@@%d#n",  nLinkId );
								sprintf( bufLinkMsgOld, "@@%d#n", ITEM_LINK );

								std::string szText = Msg.Content;
								strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
								strReplace( szText,getItemColor(itemDefine.Important,itemDefine.SuitSetID).c_str(),getOfflineItemColor(itemDefine.Important).c_str());
								memset(  m_ChatRes.Content, 0, CLT_MSG_LEN );
								strncpy( m_ChatRes.Content, szText.c_str(), szText.length()+1 );
							}
						}
						break;
					case CHAT_LINK_ARM:
						{
							tagArmLink_ToLua& arm = chatLink.m_LinkData[i].Data.m_RoleArmLink;
							//物品
							if( nLinkType == ARM_LINK && arm.m_arm.DefID != 0 )
							{
								ITEMDEF itemDefine = DBDataManager::getSingleton().getItemDef( arm.m_arm.DefID );

								memset(bufLinkMsgOld, 0, sizeof(bufLinkMsgOld));
								memset(bufLinkMsgNew, 0, sizeof(bufLinkMsgNew));
								int nLinkId = m_nLinkCount * 10 + ARM_LINK;

								sprintf( bufLinkMsgNew, "@@%d#n",  nLinkId );
								sprintf( bufLinkMsgOld, "@@%d#n", ARM_LINK );

								std::string szText = Msg.Content;
								strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
								strReplace( szText,getItemColor(itemDefine.Important,itemDefine.SuitSetID).c_str(),getOfflineItemColor(itemDefine.Important).c_str());
								memset(  m_ChatRes.Content, 0, CLT_MSG_LEN );
								strncpy( m_ChatRes.Content, szText.c_str(), szText.length()+1 );
							}
						}
						break;
					case CHAT_LINK_MISSION:
						{
							//任务
							tagMissionLink_ToLua& mission = chatLink.m_LinkData[i].Data.m_MissionLink;
							if( nLinkType == MISSION_LINK && mission.m_MissionId != 0 )
							{
								const tagTaskDef& taskDef = DBDataManager::getSingleton().getTaskDef( mission.m_MissionId );
								//TODO
								const char* szQuestType = "新手";
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

								std::string szText = Msg.Content;
								strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
							}
						}
						break;
					case CHAT_LINK_POSTION:
						{
							tagPostionLink_ToLua& pos = chatLink.m_LinkData[i].Data.m_PostionLink;
							//位置
							if( nLinkType == POSTION_LINK && pos.m_nMapId != 0)
							{
								const tagMapDef& mapDef = DBDataManager::getSingleton().getMapDef( pos.m_nMapId );

								memset(bufLinkMsgOld, 0, sizeof(bufLinkMsgOld));
								memset(bufLinkMsgNew, 0, sizeof(bufLinkMsgNew));
								int nLinkId = m_nLinkCount * 10 + POSTION_LINK;
								sprintf( bufLinkMsgNew, "#L<%s(%d,%d)>@@%d#n", mapDef.Name, pos.m_nX/10, pos.m_nY/10, nLinkId );
								sprintf( bufLinkMsgOld, "#L<%s(%d,%d)>@@%d#n", mapDef.Name, pos.m_nX/10, pos.m_nY/10, POSTION_LINK );

								std::string szText = Msg.Content;
								strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
							}
						}
						break;
					case CHAT_LINK_SECONDARY_SKILL:
						{
							tagSecondarySkillLink_ToLua& skill = chatLink.m_LinkData[i].Data.m_SecondarySkillLink;
							//生活技能
							if( nLinkType == SECONDARY_SKILL_LINK && skill.SkillType != 0)
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

								std::string szText = Msg.Content;
								strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
							}
						}
						break;
					case CHAT_LINK_PET:
						{
							tagPetLink_ToLua& pet = chatLink.m_LinkData[i].Data.m_PetLink;
							int nLinkId = m_nLinkCount * 10 + PET_LINK;
							sprintf( bufLinkMsgNew, "#L[%s]@@%d#n", pet.PetName,nLinkId );
							sprintf( bufLinkMsgOld, "#L[%s]@@%d#n", pet.PetName,PET_LINK );

							std::string szText = Msg.Content;
							strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
						}
						break;
					default:
						break;
					}
				}				
			}
			m_ChatLinkMap[m_nLinkCount++]	= chatLink; 
			OfflineManager::getSingleton().SendUIEvent("GE_CHATMSG");
		}
		break;
	case SYS_MSG:
		{			
			const CSSYSMSG& SysMsg	= msgbody.SysMsg;

			string strtemp = SysMsg.Msg;

			tagChatLink_ToLua chatLink = *(tagChatLink_ToLua*)SysMsg.LinkContent;
			//如果有链接
			if( SysMsg.LinkLen > 0 && chatLink.m_nLinkNum )
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

								std::string szText = SysMsg.Msg;
								strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
								m_ChatLinkMap[m_nLinkCount++]	= chatLink; 
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
								sprintf( bufLinkMsgNew, "#L#c%s<%s>@@%d#n", getOfflineItemColor(itemDefine.Important).c_str(), szItemName.c_str(), nLinkId );

								std::string szText = SysMsg.Msg;
								strReplace( szText, STR_PARSE_LINK.c_str(), bufLinkMsgNew );
								strtemp =  szText;
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
								sprintf( bufLinkMsgNew, "#L#c%s<%s>@@%d#n", getOfflineItemColor(itemDefine.Important).c_str(), szItemName.c_str(), nLinkId );

								std::string szText = SysMsg.Msg;
								strReplace( szText, STR_PARSE_LINK.c_str(), bufLinkMsgNew );
								strtemp =  szText;;
							}
						}
						break;
					case CHAT_LINK_PET:
						{
							tagPetLink_ToLua& pet = chatLink.m_LinkData[i].Data.m_PetLink;
							int nLinkId = m_nLinkCount * 10 + PET_LINK;
							sprintf( bufLinkMsgNew, "#L[%s]@@%d#n", pet.PetName,nLinkId );
							sprintf( bufLinkMsgOld, "#L[%s]@@%d#n", pet.PetName,PET_LINK );

							std::string szText = SysMsg.Msg;
							strReplace( szText, bufLinkMsgOld, bufLinkMsgNew );
						}
						break;
					default:
						break;
					}
				}
				m_ChatLinkMap[m_nLinkCount++]	= chatLink;
			}
			if (OfflineManager::getSingleton().getGameState() == 3)
			{
				OfflineManager::getSingleton().addtips(strtemp);
			}
			else
			{
				addmessage(strtemp.c_str(),1);
			}
		}
		break;
	case CHAT_REQ:
		{
			m_ChatReq = msgbody.ChatReq;
			OfflineManager::getSingleton().SendUIEvent("GE_CHATREQ");
		}
		break;
	}
	return 0;
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

std::string& ChatManager::strReplace( std::string& szText, const char* src, const char* des  )
{
	size_t nPos = szText.find(src);
	if ( nPos != -1)
	{
		szText.replace(nPos, strlen(src), des);
	}
	return szText;
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
	}
	return size;
}