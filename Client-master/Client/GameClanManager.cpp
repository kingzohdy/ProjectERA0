#include "GameClanManager.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "SystemSettingConfig.h"
#include "ServerTimer.h"
#include <algorithm>
#include "GameMap.h"
#include "ShowGameMap.h"
#include "ClanInfo.h"
#include "GameQuest.h"
#include "GameNpc.h"
#include "ShowActor.h"
#include "GameActor_PVP.h"
#include "OgreRoot.h"
#include "GameCamera.h"
#include "OgreCamera.h"
#include "GameExActorMgr.h"
#include "GameNpcDialog.h"

int comp_func(const void * v1, const void *v2)
{
	return *((int *)v1) - *((int *)v2);
}

tagClanLevelDef_ToLua*	getClanLevelDef( int nLevel )
{
	static_assert( sizeof(tagClanLevelDef_ToLua) == sizeof(tagClanLevelDef) );
	return (tagClanLevelDef_ToLua*)DBDataManager::getSingleton().getClanLevelDef( nLevel );
}

const int NONE_CLAN_FIND = 0XFFFF;

bool operator< ( const tagClanMem_ToLua& lhs, const tagClanMem_ToLua& rhs )
{
	static_assert( sizeof( tagClanMem_ToLua ) == sizeof( tagClanMem ) );
	if ( lhs.Online == rhs.Online )
	{
		return strcmp( lhs.RoleMember.m_szRoleName, rhs.RoleMember.m_szRoleName ) < 0;
	}

	return lhs.Online > rhs.Online;
}

bool operator== ( const tagOtherClanInfo& clanInfo,const unsigned int clanId )
{
	if( clanInfo.m_ClanInfo.m_uClanId == clanId )
	{
		return true;
	}
	return false;
}

GameClanManager::GameClanManager(void)
{
	m_CurClanCityId = 0;
	m_ClanCityEntity = NULL;
	for( int i = 0 ; i < MAX_CLAN_BUILDING ; i++ )
	{
		m_CityEntities[i] = NULL;
	}

	for ( int i = 0 ; i < CLAN_PET_MAX_JIYANG; i++ )
	{
		m_ClanPetEntities[i] = NULL;
	}

	resetClanInfo();
	INetworkManager::getSingleton().RegisterMsgHandler( CLAN_SVR, this );
	INetworkManager::getSingleton().RegisterMsgHandler( ACTIVE_GAME_SVR,this );
	INetworkManager::getSingleton().RegisterMsgHandler( CLAN_PET_SVR,this );

	DBDataManager::getSingleton().loadClanSkill( m_vClanSkill );

	m_ClanPetHighLvLastTime = 0;
}

GameClanManager::~GameClanManager(void)
{
}

int GameClanManager::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if( msgid == CLAN_SVR )
	{
		const CSCLANSVRDATA& clanSvr = msgbody.ClanSvr.ClanSvrData;
		MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
		if( msgbody.ClanClt.Operation == CLAN_SVR_INFO && pMainPlayer != NULL )
		{
			pMainPlayer->m_nClanID		= clanSvr.ClanInfo.ProfInfo.Id;
			pMainPlayer->setClanWID( clanSvr.ClanInfo.ProfInfo.GID );
		}
		switch( msgbody.ClanSvr.Operation )
		{
		case CLAN_SVR_FIND_RES:
			{
				const CSCLANFINDRES& clanFindRes = clanSvr.ClanFindRes;
				//char szBuf[256];
				// 若是查询操作，则只刷新查询信息
				tagOtherClanInfo clanInfo;
				for ( int i = 0; i < clanFindRes.Num; ++i )
				{
					setClanInfo( &clanInfo, &clanFindRes.ClanView[i] );
					m_QueryClanInfoList.push_back( clanInfo );
				}
				GameManager::getSingleton().getScriptVM()->callString( "updataSearchClan()" );
				break;
			}
			break;
		case CLAN_SVR_DIPLOMATISM_CHG:
			{
				GameActor_PVP::getSingleton().resetEnemyClan();
				m_ClanDipomatism.m_nFriendNum		= clanSvr.ClanDiplomatismSnapshot.FriendNum;
				m_ClanDipomatism.m_nEnemyNum		= clanSvr.ClanDiplomatismSnapshot.EnemyNum;
				for( int i = 0; i < m_ClanDipomatism.m_nFriendNum; i++ )
				{
					m_ClanDipomatism.m_Friends[i].m_uClanId = clanSvr.ClanDiplomatismSnapshot.Friends[i].ClanID.Id;
					memcpy( &m_ClanDipomatism.m_Friends[i].m_clanLongId, 
						&clanSvr.ClanDiplomatismSnapshot.Friends[i].ClanID.GID, sizeof(tdr_ulonglong) );
					strncpy( m_ClanDipomatism.m_Friends[i].m_szClanName, 
						clanSvr.ClanDiplomatismSnapshot.Friends[i].ClanID.Name, strlen(clanSvr.ClanDiplomatismSnapshot.Friends[i].ClanID.Name)+1 );
					strcpy(m_ClanDipomatism.m_Friends[i].m_szCaptionName,clanSvr.ClanDiplomatismSnapshot.Friends[i].Leader);
					m_ClanDipomatism.m_Friends[i].m_nTime = clanSvr.ClanDiplomatismSnapshot.Friends[i].Time;
				}
				for( int i = 0; i < m_ClanDipomatism.m_nEnemyNum; i++ )
				{
					m_ClanDipomatism.m_Enemys[i].m_uClanId = clanSvr.ClanDiplomatismSnapshot.Enemys[i].ClanID.Id;
					memcpy( &m_ClanDipomatism.m_Enemys[i].m_clanLongId, 
						&clanSvr.ClanDiplomatismSnapshot.Enemys[i].ClanID.GID, sizeof(tdr_ulonglong) );
					strncpy( m_ClanDipomatism.m_Enemys[i].m_szClanName, 
						clanSvr.ClanDiplomatismSnapshot.Enemys[i].ClanID.Name, strlen(clanSvr.ClanDiplomatismSnapshot.Enemys[i].ClanID.Name)+1 );
					strcpy(m_ClanDipomatism.m_Enemys[i].m_szCaptionName,clanSvr.ClanDiplomatismSnapshot.Enemys[i].Leader);
					m_ClanDipomatism.m_Enemys[i].m_nTime = clanSvr.ClanDiplomatismSnapshot.Enemys[i].Time;

					GameActor_PVP::getSingleton().setEnemyClan(clanSvr.ClanDiplomatismSnapshot.Enemys[i].ClanID);
				}				
				updateAllClan();
				GameManager::getSingleton().SendUIEvent( "GE_CLAN_DIPLOMATISM" );
			}
			break;
		case CLAN_SVR_CREATE:
			{
				strncpy( m_ClanViewer.m_clanInfo.m_szClanName, clanSvr.ClanName, NAME_LEN );
			}
			break;

		case CLAN_SVR_ASK_JOIN_CLAN: 
			/* 询问是否入会 */
			{
				SystemSettingConfig& systemConfig = GameManager::getSingleton().getUserSystemConfig();
				const CSCLANASK& clanInviteInfo = clanSvr.ClanAskJoinClan;
				tdr_ulonglong uClanId =	clanSvr.ClanAskJoinClan.ID.GID;
				if ( systemConfig.getGameOpt( GAME_OPT_REFUSE_CLAN_INVITE ) )
				{
					// 若设置决绝被加，直接回复拒绝
					clan_Opt_AnswerJoinClan( ((tagLong_ToLua*)&uClanId)->nHeight, ((tagLong_ToLua*)&uClanId)->nLow, 
											clanInviteInfo.ID.Id, clanInviteInfo.ID.Name, clanInviteInfo.HostName, 0 );
				}
				else
				{
					char szBuf[256];

					// 参数( 公会名称，询问人名字 )
					sprintf( szBuf, "askJoinClan(%d,%d,%d,\"%s\",\"%s\")",  ((tagLong_ToLua*)&uClanId)->nHeight, 
						((tagLong_ToLua*)&uClanId)->nLow, 
						clanInviteInfo.ID.Id, 
						clanInviteInfo.ID.Name, 
						clanInviteInfo.HostName );
					GameManager::getSingleton().getScriptVM()->callString( szBuf );
				}
			}
			break;
		case CLAN_SVR_CITY_INFO:
			{
				m_clanCity.setCityInfo( clanSvr.ClanCityInfo );
				GameManager::getSingleton().SendUIEvent( "GE_CLAN_CITY_SVR_INFO" );
				//checkClanBuildChange();
			}
			break;

		case CLAN_SVR_ASK_JOIN_UNION:
			/* 询问是否入盟,这个消息会被发送给被邀请公会的会长或副会长 */
			{
				char szBuf[256];
				tdr_ulonglong uClanId =	clanSvr.ClanAskJoinUnion.ID.GID;
				// 参数( 公会名称，询问人名字 )
				sprintf( szBuf, "askJoinUnion(%d,%d,%d,\"%s\",\"%s\")", ((tagLong_ToLua*)&uClanId)->nHeight, 
																		((tagLong_ToLua*)&uClanId)->nLow, 
																		clanSvr.ClanAskJoinUnion.ID.Id, 
																		clanSvr.ClanAskJoinUnion.ID.Name, 
																		clanSvr.ClanAskJoinUnion.HostName );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;

		case CLAN_SVR_ASK_JOIN_ENEMY:
			/* 询问是进入行敌视状态 */
			{
				
			}
			break;

		case CLAN_SVR_INFO:
			/* 公会信息 */
			{
				m_ClanViewer.m_clanInfo.m_uClanId			= clanSvr.ClanInfo.ProfInfo.Id;
				m_ClanViewer.m_clanInfo.m_nClanLevel		= clanSvr.ClanInfo.ProfInfo.Level;
				m_ClanViewer.m_clanInfo.m_nClanMemNum		= clanSvr.ClanInfo.ProfInfo.MemNum;
				m_ClanViewer.m_clanInfo.m_nMoney			= clanSvr.ClanInfo.ProfInfo.Money;
				m_ClanViewer.m_clanInfo.m_nBuildingValue	= clanSvr.ClanInfo.ProfInfo.StableValue;
				m_ClanViewer.m_clanInfo.m_nActiveValue		= clanSvr.ClanInfo.ProfInfo.ActiveValue;
				m_ClanViewer.m_clanInfo.m_nSkillPoint		= clanSvr.ClanInfo.ProfInfo.TechPoint;
				m_ClanViewer.m_clanInfo.m_BulletinTextLastEditTime = clanSvr.ClanInfo.BulletinTextLastEditTime;
				m_ClanViewer.m_clanInfo.m_Flag				= clanSvr.ClanInfo.ProfInfo.Flag;
				memcpy( &m_ClanViewer.m_clanInfo.m_clanLongId, &clanSvr.ClanInfo.ProfInfo.GID, 
							sizeof( clanSvr.ClanInfo.ProfInfo.GID ) );
				memcpy( &m_ClanViewer.m_clanInfo.m_szClanName, &clanSvr.ClanInfo.ProfInfo.Name, 
							strlen( clanSvr.ClanInfo.ProfInfo.Name ) + 1 );
				memcpy( &m_ClanViewer.m_clanInfo.m_szBulletinText, &clanSvr.ClanInfo.BulletinText, 
							strlen( clanSvr.ClanInfo.BulletinText ) + 1 );
				memcpy( &m_ClanViewer.m_PresidentInfo.m_RoleLongId, &clanSvr.ClanInfo.PresidentID.RoleID, 
							sizeof( tdr_ulonglong ) );
				memcpy( &m_ClanViewer.m_PresidentInfo.m_szRoleName, &clanSvr.ClanInfo.PresidentID.RoleName, 
							strlen( clanSvr.ClanInfo.PresidentID.RoleName ) + 1 );

				GameManager::getSingleton().SendUIEvent( "GE_CLAN_SVR_INFO" );
				//checkClanBuildChange();
				GameQuest::getSingleton().updateNpcQuestTitle();

				//处理登录时公会情报npc标识的显示
				if ( pMainPlayer->getMapId() == CLAN_CITY_MAP_ID )
				{
					const int id = ActorManager::getSingleton().getActorIdByResId( CLAN_INTELLINGENCE_NPC_ID );
					GameNPC* pNpc = (GameNPC*)ActorManager::getSingleton().FindActor( id );
					if ( pNpc != NULL )
					{
						pNpc->getShowActor()->getCurEntity()->stopMotion( 9541 );
						if ( clanSvr.ClanInfo.ProfInfo.Id == getCurClanCityId() )
						{							
							pNpc->getShowActor()->getCurEntity()->playMotion( "9541_16", false, 9541 );
						}				
					}					
				}
			}
			break;

		case CLAN_SVR_MEMBER_SNAPSHOT:
			/* 全部会员信息 */
			{
				static_assert( sizeof( tagClanMem_ToLua ) == sizeof( CLANMEM ) );
				const CSCLANMEMBERS& memberRes = clanSvr.ClanMemberSnapshot;

				for( int i = 0; i < memberRes.MemNum; i++ )
				{
					memcpy( &m_ClanViewer.m_Members[i], &memberRes.Members[i], sizeof( CLANMEM ) );
					if( std::string( m_ClanViewer.m_Members[i].RoleMember.m_szRoleName ) == 
						std::string( pMainPlayer->getName() ) )
					{
						pMainPlayer->setClanName( clanSvr.ClanInfo.ProfInfo.Name );
						pMainPlayer->m_nClanID			= clanSvr.ClanInfo.ProfInfo.Id;
						pMainPlayer->m_nClanPosition	= m_ClanViewer.m_Members[i].Position;
						pMainPlayer->m_nClanGroup		= m_ClanViewer.m_Members[i].GroupID;
						pMainPlayer->setClanWID( clanSvr.ClanInfo.ProfInfo.GID );
					}						
				}

				memcpy( &m_ClanViewer.m_clanInfo.m_clanLongId, &memberRes.ID.GID, sizeof(tdr_ulonglong) );
				m_ClanViewer.m_clanInfo.m_uClanId = memberRes.ID.Id;
				m_ClanViewer.m_clanInfo.m_nClanMemNum = memberRes.MemNum;
				strncpy( m_ClanViewer.m_clanInfo.m_szClanName, memberRes.ID.Name, NAME_LEN );

				sortMemberList( m_nSortType );
				updateClanDonateInfo();
			}
			break;

		case CLAN_SVR_MEMBER_CHG:
			/* 增量会员信息 */
			{
				int num = m_ClanViewer.m_clanInfo.m_nClanMemNum;
				static_assert( sizeof( tagClanMem_ToLua ) == sizeof( tagClanMem ) );
				if( clanSvr.ClanMemberChg.Code == CLAN_MEMBER_CHG_ADD )
				{
					memcpy( &m_ClanViewer.m_Members[num], &clanSvr.ClanMemberChg.MemInfo, sizeof( CLANMEM ) );
					m_ClanViewer.m_clanInfo.m_nClanMemNum++;
				}

				if( clanSvr.ClanMemberChg.Code == CLAN_MEMBER_CHG_MOD )
				{
					//看看是否是主角
// 					if( clanSvr.ClanMemberChg.MemInfo.RoleMember.RoleID == ActorManager::getSingleton().getMainPlayer()->getRoleWId() )
// 					{
// 						ActorManager::getSingleton().getMainPlayer()->m_nClanPosition = clanSvr.ClanMemberChg.MemInfo.Position;
// 					}
					if( clanSvr.ClanMemberChg.MemInfo.Online == ROLE_MUD_ONLINE )
					{
						for( int i = 0; i < num; i++ )
						{
							if( *(tdr_ulonglong*)&m_ClanViewer.m_Members[i].RoleMember.m_RoleLongId.nHeight == 
								clanSvr.ClanMemberChg.MemInfo.RoleMember.RoleID )
							{
								//memcpy( &m_ClanViewer.m_Members[i], &clanSvr.ClanMemberChg.MemInfo, sizeof( CLANMEM ) );
								m_ClanViewer.m_Members[i].Online = clanSvr.ClanMemberChg.MemInfo.Online;
								m_ClanViewer.m_Members[i].Career = clanSvr.ClanMemberChg.MemInfo.Career;
								m_ClanViewer.m_Members[i].Level = clanSvr.ClanMemberChg.MemInfo.Level;
							}
						}
					}else{
						GameActor* pactor = ActorManager::getSingleton().findActorByName( clanSvr.ClanMemberChg.MemInfo.RoleMember.RoleName );
						if(pactor != NULL )
						{
							pactor->m_nClanPosition = clanSvr.ClanMemberChg.MemInfo.Position;
							pactor->m_nClanGroup = clanSvr.ClanMemberChg.MemInfo.GroupID;
							RoleText& actorTitleMgr = pactor->getRoleText();
							actorTitleMgr.BuildTitle();
						}
						for( int i = 0; i < num; i++ )
						{
							if( *(tdr_ulonglong*)&m_ClanViewer.m_Members[i].RoleMember.m_RoleLongId.nHeight == 
								clanSvr.ClanMemberChg.MemInfo.RoleMember.RoleID )
							{
								if ( *(unsigned char*)&m_ClanViewer.m_Members[i].Position != clanSvr.ClanMemberChg.MemInfo.Position )
								{
									memcpy( &m_ClanViewer.m_Members[i], &clanSvr.ClanMemberChg.MemInfo, sizeof( CLANMEM ) );
									GameManager::getSingleton().SendUIEvent("GE_CLAN_POSITION_CHANGE");
								}
								else
								{
									memcpy( &m_ClanViewer.m_Members[i], &clanSvr.ClanMemberChg.MemInfo, sizeof( CLANMEM ) );
								}						
							}
						}
					}					 
					GameManager::getSingleton().SendUIEvent("GE_CLAN_CONTRIBUTE_CHANGE");
				}

				if( clanSvr.ClanMemberChg.Code == CLAN_MEMBER_CHG_DEL )
				{
					for( int i = 0; i < num; i++ )
					{
						if( *(tdr_ulonglong*)&m_ClanViewer.m_Members[i].RoleMember.m_RoleLongId.nHeight == 
							clanSvr.ClanMemberChg.MemInfo.RoleMember.RoleID && i != num-1 )
						{
							std::swap( m_ClanViewer.m_Members[num-1], m_ClanViewer.m_Members[i] );
						}
					}
					m_ClanViewer.m_clanInfo.m_nClanMemNum--;
					GameManager::getSingleton().SendUIEvent("GE_CLAN_MEMBER_DEL");
				}			
				
				sortMemberList( m_nSortType );
				updateClanDonateInfo();
				GameManager::getSingleton().SendUIEvent( "GE_CLAN_UPDATEMEMBER" );
			}
			break;

		case CLAN_SVR_CLAN_VIEW_PAGE:
			/* 一页公会信息 */
			{
			}
			break;
		case CLAN_SVR_APPLY_JOIN_LIST:
			/* 申请列表 */
			{
				static_assert( sizeof( tagClanApply_ToLua ) == sizeof( tagClanApply ) );
				memset( &m_ApplyJoinList.m_ApplyRoles, 0, sizeof(tagClanApply_ToLua)*CLAN_MAX_APPLY );
				m_ApplyJoinList.m_nRoleNum = clanSvr.ClanApplyJoinList.RoleNum;
				for( int i = 0; i < m_ApplyJoinList.m_nRoleNum; i++ )
				{
					memcpy( &m_ApplyJoinList.m_ApplyRoles[i], &clanSvr.ClanApplyJoinList.ApplyRoles[i], sizeof( tagClanApply ) );
				}
				// 刷新申请列表
				GameManager::getSingleton().SendUIEvent( "GE_CLAN_JOINLIST" );
			}
			break;

		case CLAN_SVR_DIPLOMATISM_STATUS:
			/* 公会的外交状况 */
			{
				m_ClanDipomatism.m_nFriendNum		= clanSvr.ClanDiplomatismStatus.FriendNum;
				m_ClanDipomatism.m_nEnemyNum		= clanSvr.ClanDiplomatismStatus.EnemyNum;
				m_ClanDipomatism.m_nApplyFriendNum	= clanSvr.ClanDiplomatismStatus.ApplyFriendNum;
				m_ClanDipomatism.m_nApplyEnemyNum	= clanSvr.ClanDiplomatismStatus.ApplyEnemyNum;
				for( int i = 0; i < m_ClanDipomatism.m_nFriendNum; i++ )
				{
					m_ClanDipomatism.m_Friends[i].m_uClanId = clanSvr.ClanDiplomatismStatus.Friends[i].ClanID.Id;
					memcpy( &m_ClanDipomatism.m_Friends[i].m_clanLongId, 
						&clanSvr.ClanDiplomatismStatus.Friends[i].ClanID.GID, sizeof(tdr_ulonglong) );
					strncpy( m_ClanDipomatism.m_Friends[i].m_szClanName, 
						clanSvr.ClanDiplomatismStatus.Friends[i].ClanID.Name, strlen(clanSvr.ClanDiplomatismStatus.Friends[i].ClanID.Name)+1 );
					strcpy(m_ClanDipomatism.m_Friends[i].m_szCaptionName,clanSvr.ClanDiplomatismStatus.Friends[i].Leader);
					m_ClanDipomatism.m_Friends[i].m_nTime = clanSvr.ClanDiplomatismStatus.Friends[i].Time;
				}
				for( int i = 0; i < m_ClanDipomatism.m_nEnemyNum; i++ )
				{
					m_ClanDipomatism.m_Enemys[i].m_uClanId = clanSvr.ClanDiplomatismStatus.Enemys[i].ClanID.Id;
					memcpy( &m_ClanDipomatism.m_Enemys[i].m_clanLongId, 
						&clanSvr.ClanDiplomatismStatus.Enemys[i].ClanID.GID, sizeof(tdr_ulonglong) );
					strncpy( m_ClanDipomatism.m_Enemys[i].m_szClanName, 
						clanSvr.ClanDiplomatismStatus.Enemys[i].ClanID.Name, strlen(clanSvr.ClanDiplomatismStatus.Enemys[i].ClanID.Name)+1 );
					strcpy(m_ClanDipomatism.m_Enemys[i].m_szCaptionName,clanSvr.ClanDiplomatismStatus.Enemys[i].Leader);
					m_ClanDipomatism.m_Friends[i].m_nTime = clanSvr.ClanDiplomatismStatus.Enemys[i].Time;
				}
				for( int i = 0; i < m_ClanDipomatism.m_nApplyFriendNum; i++ )
				{
					m_ClanDipomatism.m_ApplyFriends[i].m_ClanInfo.m_uClanId = clanSvr.ClanDiplomatismStatus.ApplyFriends[i].ClanID.Id;
					memcpy( &m_ClanDipomatism.m_ApplyFriends[i].m_ClanInfo.m_clanLongId, 
						&clanSvr.ClanDiplomatismStatus.ApplyFriends[i].ClanID.GID, sizeof(tdr_ulonglong) );
					strncpy( m_ClanDipomatism.m_ApplyFriends[i].m_ClanInfo.m_szClanName, 
						clanSvr.ClanDiplomatismStatus.ApplyFriends[i].ClanID.Name, strlen(clanSvr.ClanDiplomatismStatus.ApplyFriends[i].ClanID.Name)+1 );
					strcpy(m_ClanDipomatism.m_ApplyFriends[i].m_ClanInfo.m_szCaptionName,clanSvr.ClanDiplomatismStatus.ApplyFriends[i].Leader);
					m_ClanDipomatism.m_ApplyFriends[i].m_nApplyTime = clanSvr.ClanDiplomatismStatus.ApplyFriends[i].ApplyTime;
				}
				for( int i = 0; i < m_ClanDipomatism.m_nApplyEnemyNum; i++ )
				{
					m_ClanDipomatism.m_ApplyEnemys[i].m_ClanInfo.m_uClanId = clanSvr.ClanDiplomatismStatus.ApplyEnemys[i].ClanID.Id;
					memcpy( &m_ClanDipomatism.m_ApplyEnemys[i].m_ClanInfo.m_clanLongId, 
						&clanSvr.ClanDiplomatismStatus.ApplyEnemys[i].ClanID.GID, sizeof(tdr_ulonglong) );
					strncpy( m_ClanDipomatism.m_ApplyEnemys[i].m_ClanInfo.m_szClanName, 
						clanSvr.ClanDiplomatismStatus.ApplyEnemys[i].ClanID.Name, strlen(clanSvr.ClanDiplomatismStatus.ApplyEnemys[i].ClanID.Name)+1 );
					strcpy(m_ClanDipomatism.m_ApplyEnemys[i].m_ClanInfo.m_szCaptionName,clanSvr.ClanDiplomatismStatus.ApplyEnemys[i].Leader);
					m_ClanDipomatism.m_ApplyEnemys[i].m_nApplyTime = clanSvr.ClanDiplomatismStatus.ApplyEnemys[i].ApplyTime;
				}
				updateAllClan();
				GameManager::getSingleton().SendUIEvent( "GE_CLAN_DIPLOMATISM" );
			}
			break;

		case CLAN_SVR_MSG:
			/* 公会文本消息 */
			{
				// TODO::
			}
			break;
// 		case CLAN_SVR_MANIFESTO:
// 			/* 公会宣言 */	
// 			{
// 				char szScript[256];
// 				// 刷新公会查询页面，参数( 当前页数 )
// 				sprintf( szScript, "showManifesto(%d)", msgbody.ClanClt.ClanCltData.ClanManifestoText );
// 				GameManager::getSingleton().getScriptVM()->callString( szScript );
// 			}
// 			break;
		case CLAN_SVR_BULLETIN:
			/* 公会宣言 */	
			{
				char szScript[256];
				// 刷新公会查询页面，参数( 当前页数 )
				strncpy(m_ClanViewer.m_clanInfo.m_szBulletinText,msgbody.ClanSvr.ClanSvrData.ClanBulletin.BulletinText,SHORT_DESC_LEN);
				m_ClanViewer.m_clanInfo.m_BulletinTextLastEditTime = msgbody.ClanSvr.ClanSvrData.ClanBulletin.BulletinLastWrite;
				sprintf( szScript, "updateClanBulletin([===[%s]===],%d)", m_ClanViewer.m_clanInfo.m_szBulletinText,m_ClanViewer.m_clanInfo.m_BulletinTextLastEditTime );
				GameManager::getSingleton().getScriptVM()->callString( szScript );
			}
			break;
		case CLAN_SVR_DISABLE:
			/* 清空公会快照 */	
			{
				memset( &m_ClanViewer, 0, sizeof(tagClanViewer) );
				pMainPlayer->m_nClanID = 0;
				pMainPlayer->setClanWID( 0 );
				GameQuest::getSingleton().updateNpcQuestTitle();
				GameActor_PVP::getSingleton().resetEnemyClan();
				GameManager::getSingleton().SendUIEvent( "GE_CLAN_DISABLEFRAME" );
			}
			break;
		case CLAN_SVR_NOTIFY:
			{
				if( msgbody.ClanSvr.ClanSvrData.ClanNotify.Code == CLAN_NOTIFY_MEMBER )
				{
					GameManager::getSingleton().SendUIEvent("GE_CLAN_NOTIFY_MEMBER");
				}else if( msgbody.ClanSvr.ClanSvrData.ClanNotify.Code == CLAN_NOTIFY_DIPLOMATISM )
				{
					GameManager::getSingleton().SendUIEvent("GE_CLAN_NOTIFY_DIPLOMATISM");
				}
				break;
			}
		case CLAN_SVR_ID_SNAPSHOT:
			{
				m_otherClanIDVec.clear();
				for( int i = 0 ; i < msgbody.ClanSvr.ClanSvrData.ClanIDSnapshot.Num; i ++ )
				{
					m_otherClanIDVec.push_back(msgbody.ClanSvr.ClanSvrData.ClanIDSnapshot.IDs[i]);
				}
			}
			break;
		case CLAN_SVR_VIEW_PAGE:
			{
				for( int i = 0 ; i < msgbody.ClanSvr.ClanSvrData.ClanViewOfPage.Num; i ++ )
				{
					modifyOtherClanData( msgbody.ClanSvr.ClanSvrData.ClanViewOfPage.View[i] );
				}
				std::stable_sort( m_otherClanVec.begin(),m_otherClanVec.end(),otherClanCompare );
				GameManager::getSingleton().SendUIEvent( "GE_CLANLIST_UPDATE" );
			}
			break;
		case CLAN_SVR_VIEW_CHG:
			{
				switch( msgbody.ClanSvr.ClanSvrData.ClanViewChg.What )
				{
				case CLAN_VIEW_CHG_ADD:
					modifyOtherClanData( msgbody.ClanSvr.ClanSvrData.ClanViewChg.Data.Added );
					break;
				case CLAN_VIEW_CHG_DEL:
					deleteOtherClanData( msgbody.ClanSvr.ClanSvrData.ClanViewChg.Data.Deleted );
					break;
				case CLAN_VIEW_CHG_MOD:
					modifyOtherClanData( msgbody.ClanSvr.ClanSvrData.ClanViewChg.Data.Modified );
					break;
				}
				std::stable_sort( m_otherClanVec.begin(),m_otherClanVec.end(),otherClanCompare );
				GameManager::getSingleton().SendUIEvent( "GE_CLANLIST_UPDATE" );
			}
			break;
		case CLAN_SVR_ADVANCE_SNAPSHOT:
			{
				static_assert(sizeof(tagClanAdvance_ToLua) == sizeof( CLANADVANCE ) );
				memcpy( &m_ClanAdvace,&msgbody.ClanSvr.ClanSvrData.ClanAdvanceSnapshot.Snapshot,sizeof(CLANADVANCE) );
				memcpy( &m_ClanSkill,&msgbody.ClanSvr.ClanSvrData.ClanAdvanceSnapshot.Snapshot.ClanSkillInfo,sizeof(CLANSKILLINFO) );
				GameManager::getSingleton().SendUIEvent("GE_CLAN_ADVANCE_UPDATE");
				GameManager::getSingleton().SendUIEvent("GE_CLAN_MAKING_UPDATE");
				GameManager::getSingleton().SendUIEvent("GE_CLAN_STORAGE_UPDATE");
			}
			break;
		case CLAN_SVR_RESEARCH_NOTIFY:
			{
				const CSCLANPROGRESSNOTIFY& notify = msgbody.ClanSvr.ClanSvrData.ClanResearchNotify;
				switch( notify.What )
				{
				case CLAN_PROGRESS_STARTED:
					{
						if( m_ClanAdvace.Tech.TechResearchNum < MAX_CLAN_TECH_CATEGORY )
						{
							static_assert( sizeof(tagClanRunningProgress_ToLua) == sizeof(tagClanRunningProgress) );
							memcpy( &m_ClanAdvace.Tech.TechResearchProgress[m_ClanAdvace.Tech.TechResearchNum++],&notify.Data.Started,sizeof(tagClanRunningProgress) );
						}
					}
					break;
				case CLAN_PROGRESS_CANCELED:
				case CLAN_PROGRESS_FINISHED:
					{
						if( notify.What == CLAN_PROGRESS_FINISHED )
						{
							m_ClanAdvace.Tech.TechLevel[ m_ClanAdvace.Tech.TechResearchProgress[notify.Data.Canceled.Pos].Category ] ++;
						}
						for( int i = notify.Data.Canceled.Pos;i < m_ClanAdvace.Tech.TechResearchNum - 1 ; i++ )
						{
							m_ClanAdvace.Tech.TechResearchProgress[i] = m_ClanAdvace.Tech.TechResearchProgress[i+1];
						}
						m_ClanAdvace.Tech.TechResearchNum --;
						GameManager::getSingleton().SendUIEvent("GE_CLAN_RESEARCH_COMPLATE");
					}
					break;
				case CLAN_PROGRESS_SHIFTED:
					{
						m_ClanAdvace.Tech.TechResearchProgress[notify.Data.Shifted.Identifier.Pos].BeginTime = notify.Data.Shifted.BeginTime;
						m_ClanAdvace.Tech.TechResearchProgress[notify.Data.Shifted.Identifier.Pos].EndTime = notify.Data.Shifted.EndTime;
					}
					break;
				}
				GameManager::getSingleton().SendUIEvent("GE_CLAN_ADVANCE_UPDATE");				
			}
			break;
		case CLAN_SVR_MAKING_NOTIFY:
			{
				const CSCLANPROGRESSNOTIFY& notify = msgbody.ClanSvr.ClanSvrData.ClanMakingNotify;
				switch( notify.What )
				{
				case CLAN_PROGRESS_STARTED:
					{
						if( notify.Data.Started.Category == CLAN_MAKE_FOR_COLLECTION )
						{
							if( m_ClanAdvace.Produce.CollectionMakingNum < MAX_COLLECTION_MAKING_NUM )
							{
								static_assert( sizeof(tagClanRunningProgress_ToLua) == sizeof(tagClanRunningProgress) );
								memcpy( &m_ClanAdvace.Produce.CollectionMakingProgress[m_ClanAdvace.Produce.CollectionMakingNum++],&notify.Data.Started,sizeof(tagClanRunningProgress) );
							}
						}else if( notify.Data.Started.Category == CLAN_MAKE_FOR_ARM )
						{
							if( m_ClanAdvace.Produce.ArmMakingNum < MAX_ARM_MAKING_NUM )
							{
								static_assert( sizeof(tagClanRunningProgress_ToLua) == sizeof(tagClanRunningProgress) );
								memcpy(&m_ClanAdvace.Produce.ArmMakingProgress[m_ClanAdvace.Produce.ArmMakingNum++],&notify.Data.Started,sizeof(tagClanRunningProgress));
							}
						}
					}
					break;
				case CLAN_PROGRESS_FINISHED:
					{
						if( notify.Data.Canceled.Category == CLAN_MAKE_FOR_COLLECTION )
						{
							m_ClanAdvace.Produce.CollectionMakingProgress[notify.Data.Finished.Pos].Value --;
							if( m_ClanAdvace.Produce.CollectionMakingProgress[notify.Data.Finished.Pos].Value <= 0 )
							{
								for( int i=notify.Data.Canceled.Pos;i<m_ClanAdvace.Produce.CollectionMakingNum-1;i++ )
								{
									m_ClanAdvace.Produce.CollectionMakingProgress[i] = m_ClanAdvace.Produce.CollectionMakingProgress[i+1];
								}
								m_ClanAdvace.Produce.CollectionMakingNum --;
							}else{
								tagClanProductDef_ToLua* productDef = getProductDefByID( m_ClanAdvace.Produce.CollectionMakingProgress[notify.Data.Finished.Pos].ID );
								if( productDef != NULL )
								{
									m_ClanAdvace.Produce.CollectionMakingProgress[notify.Data.Finished.Pos].BeginTime = getServerTime();
									m_ClanAdvace.Produce.CollectionMakingProgress[notify.Data.Finished.Pos].EndTime = getServerTime() + productDef->TimeCost;
								}
							}							
						}else if( notify.Data.Canceled.Category == CLAN_MAKE_FOR_ARM )
						{
							m_ClanAdvace.Produce.ArmMakingProgress[notify.Data.Finished.Pos].Value --;
							if( m_ClanAdvace.Produce.ArmMakingProgress[notify.Data.Finished.Pos].Value <= 0 )
							{
								for( int i=notify.Data.Canceled.Pos;i<m_ClanAdvace.Produce.ArmMakingNum-1;i++ )
								{
									m_ClanAdvace.Produce.ArmMakingProgress[i] = m_ClanAdvace.Produce.ArmMakingProgress[i+1];
								}
								m_ClanAdvace.Produce.ArmMakingNum --;
							}else{
								tagClanProductDef_ToLua* productDef = getProductDefByID( m_ClanAdvace.Produce.ArmMakingProgress[notify.Data.Finished.Pos].ID );
								if( productDef != NULL )
								{
									m_ClanAdvace.Produce.ArmMakingProgress[notify.Data.Finished.Pos].BeginTime = getServerTime();
									m_ClanAdvace.Produce.ArmMakingProgress[notify.Data.Finished.Pos].EndTime = getServerTime() + productDef->TimeCost;
								}
							}						
						}
					}
					break;
				case CLAN_PROGRESS_CANCELED:
					{						
						if( notify.Data.Canceled.Category == CLAN_MAKE_FOR_COLLECTION )
						{
							for( int i=notify.Data.Canceled.Pos;i<m_ClanAdvace.Produce.CollectionMakingNum-1;i++ )
							{
								m_ClanAdvace.Produce.CollectionMakingProgress[i] = m_ClanAdvace.Produce.CollectionMakingProgress[i+1];
							}
							m_ClanAdvace.Produce.CollectionMakingNum --;
						}else if( notify.Data.Canceled.Category == CLAN_MAKE_FOR_ARM )
						{
							for( int i=notify.Data.Canceled.Pos;i<m_ClanAdvace.Produce.ArmMakingNum-1;i++ )
							{
								m_ClanAdvace.Produce.ArmMakingProgress[i] = m_ClanAdvace.Produce.ArmMakingProgress[i+1];
							}
							m_ClanAdvace.Produce.ArmMakingNum --;
						}						
					}
					break;
				case CLAN_PROGRESS_SHIFTED:
					{
						if( notify.Data.Shifted.Identifier.Category == CLAN_MAKE_FOR_COLLECTION )
						{
							m_ClanAdvace.Produce.CollectionMakingProgress[notify.Data.Shifted.Identifier.Pos].BeginTime = notify.Data.Shifted.BeginTime;
							m_ClanAdvace.Produce.CollectionMakingProgress[notify.Data.Shifted.Identifier.Pos].EndTime = notify.Data.Shifted.EndTime;
						}else if( notify.Data.Shifted.Identifier.Category == CLAN_MAKE_FOR_ARM )
						{
							m_ClanAdvace.Produce.ArmMakingProgress[notify.Data.Shifted.Identifier.Pos].BeginTime = notify.Data.Shifted.BeginTime;
							m_ClanAdvace.Produce.ArmMakingProgress[notify.Data.Shifted.Identifier.Pos].EndTime = notify.Data.Shifted.EndTime;
						}
					}
					break;
				}
				GameManager::getSingleton().SendUIEvent("GE_CLAN_MAKING_UPDATE");
			}
			break;
		case CLAN_SVR_PROPERTY_NOTIFY:
			{
				const CSCLANPROPERTYNOTIFY& notify = msgbody.ClanSvr.ClanSvrData.ClanPropertyNotify;
				if( notify.Which == CLAN_PROPERTY_MONEY )
				{
					m_ClanViewer.m_clanInfo.m_nMoney = notify.Value;
					GameManager::getSingleton().SendUIEvent( "GE_CLAN_MONEY_UPDATE" );
				}
				if( notify.Which == CLAN_PROPERTY_STABLE_VALUE )
				{
					m_ClanViewer.m_clanInfo.m_nBuildingValue = notify.Value;
				}
				if( notify.Which == CLAN_PROPERTY_ACTIVE_VALUE )
				{
					m_ClanViewer.m_clanInfo.m_nActiveValue = notify.Value;
				}
				if( notify.Which == CLAN_PROPERTY_TECH_POINT )
				{
					m_ClanViewer.m_clanInfo.m_nSkillPoint = notify.Value;
				}
				if( notify.Which == CLAN_PROPERTY_USED_TECH_POINT )
				{
					m_ClanAdvace.UsedTechPoint = notify.Value;
				}
				if( notify.Which == CLAN_PROPERTY_MAX_COLLECTION_MAKING_NUM )
				{
					m_ClanAdvace.Produce.MaxCollectionMakingNum = notify.Value;
				}
				if( notify.Which == CLAN_PROPERTY_MAX_ARM_MAKING_NUM )
				{
					m_ClanAdvace.Produce.MaxArmMakingNum = notify.Value;
				}
				if( notify.Which == CLAN_PROPERTY_LEVEL )
				{
					m_ClanViewer.m_clanInfo.m_nClanLevel	= notify.Value;
					GameQuest::getSingleton().updateNpcQuestTitle();
					GameManager::getSingleton().SendUIEvent( "GE_CLAN_LEVEL_UPDATE" );
				}
				if( notify.Which == CLAN_PROPERTY_CITY_LEVEL )
				{
					m_clanCity.m_clanCityInfo.m_nLevel	= notify.Value;
					//checkClanBuildChange();
				}
				if ( notify.Which == CLAN_PROPERTY_FLAG )
				{
					m_ClanViewer.m_clanInfo.m_Flag = notify.Value;
				}
				GameManager::getSingleton().SendUIEvent( "GE_CLAN_SVR_INFO" );
				GameManager::getSingleton().SendUIEvent("GE_CLAN_ADVANCE_UPDATE");
			}
			break;
		case CLAN_SVR_BUILDING_NOTIFY:
			{
				const CSCLANBUILDINGNOTIFY& notify = msgbody.ClanSvr.ClanSvrData.ClanBuildingNotify;
				switch( notify.What )
				{
				case CLAN_BUILDING_BEGIN:
					{
// 						notify.Data.Begin.BeginTime;
// 						notify.Data.Begin.EndTime;
// 						notify.Data.Begin.Idx;
					}
					break;
				case CLAN_BUILDING_COMPLETED:
					m_clanCity.m_clanCityInfo.m_buildings[notify.Data.Completed].Level++;
					break;
				default:
					break;
				}
				//checkClanBuildChange();
				GameManager::getSingleton().SendUIEvent("GE_CLAN_BUILDING_COMPLETE");
			}
			break;
		case CLAN_SVR_ASK_EXIT_ENEMY:
			{
				memcpy(&m_EnemyID,&msgbody.ClanSvr.ClanSvrData.ClanAskExitEnemy.ID,sizeof(CLANID));
				strcpy( m_szEnemyHostName,msgbody.ClanClt.ClanCltData.ClanAnswerExitEnemy.HostName );
				char szBuf[256];
				sprintf(szBuf,"ClanAskExitEnemy(\"%s\",\"%s\")",m_szEnemyHostName,m_EnemyID.Name);
				GameManager::getSingleton().getScriptVM()->callString(szBuf);				
			}
			break;
			//4.6响应服务器消息
		case CLAN_SVR_COLOR_NOTIFY:
			{
				m_ClanColor.clear();
				const tagSetClanColor &info = msgbody.ClanSvr.ClanSvrData.ClanColorNotify.Set;
				for (int i = 0; i < info.Num; i++)
				{
					if (info.ClanColor[i].Color != 0)
					{
						m_ClanColor[info.ClanColor[i].ClanID] = info.ClanColor[i].Color;
					}
					m_ClanIDMap[info.ClanColor[i].ClanGID] = info.ClanColor[i].ClanID;
					m_ClanIDMapReverse[info.ClanColor[i].ClanID] = info.ClanColor[i].ClanGID;
				}
				updateAllClan();
				GameManager::getSingleton().SendUIEvent( "GE_CLANLIST_UPDATE" );//调用lua的更新函数
			}
			break;
		case CLAN_SVR_RESOURCE_NOTIFY:
			{
				const tagClanProductDef *clanProductDef = DBDataManager::getSingleton().getClanProductDef(clanSvr.ClanResourceNotify.ResourceID);
				if (clanProductDef && clanProductDef->ResourceID != 0)
				{
					bool bFind = false;
					for (int i = 0; i < m_ClanAdvace.Resource.ResourceNum; ++i)
					{
						if (m_ClanAdvace.Resource.ResourceInfo[i].ResourceID == clanProductDef->ResourceID)
						{
							bFind = true;
							m_ClanAdvace.Resource.ResourceInfo[i].Num = clanSvr.ClanResourceNotify.Num;
							m_ClanAdvace.Resource.ResourceInfo[i].State = clanSvr.ClanResourceNotify.State;
							break;
						}
					}
					if (!bFind)
					{
						m_ClanAdvace.Resource.ResourceInfo[m_ClanAdvace.Resource.ResourceNum].ResourceID = clanProductDef->ResourceID;
						m_ClanAdvace.Resource.ResourceInfo[m_ClanAdvace.Resource.ResourceNum].Num = clanSvr.ClanResourceNotify.Num;
						m_ClanAdvace.Resource.ResourceInfo[m_ClanAdvace.Resource.ResourceNum].State = clanSvr.ClanResourceNotify.State;
						++m_ClanAdvace.Resource.ResourceNum;
					}
				}
				else
				{
					bool bFind = false;
					for (int i = 0; i < m_ClanAdvace.Resource.ItemNum; ++i)
					{
						if (m_ClanAdvace.Resource.ItemInfo[i].Pos == clanSvr.ClanResourceNotify.Pos)
						{
							bFind = true;
							m_ClanAdvace.Resource.ItemInfo[i].ResourceID = clanSvr.ClanResourceNotify.ResourceID;
							m_ClanAdvace.Resource.ItemInfo[i].Num = clanSvr.ClanResourceNotify.Num;
							m_ClanAdvace.Resource.ItemInfo[i].State = clanSvr.ClanResourceNotify.State;
							break;
						}
					}
					if (!bFind)
					{
						m_ClanAdvace.Resource.ItemInfo[m_ClanAdvace.Resource.ItemNum].ResourceID = clanSvr.ClanResourceNotify.ResourceID;
						m_ClanAdvace.Resource.ItemInfo[m_ClanAdvace.Resource.ItemNum].Num = clanSvr.ClanResourceNotify.Num;
						m_ClanAdvace.Resource.ItemInfo[m_ClanAdvace.Resource.ItemNum].State = clanSvr.ClanResourceNotify.State;
						m_ClanAdvace.Resource.ItemInfo[m_ClanAdvace.Resource.ItemNum].Pos = clanSvr.ClanResourceNotify.Pos;
						++m_ClanAdvace.Resource.ItemNum;
					}
				}
				GameManager::getSingleton().SendUIEvent("GE_CLAN_STORAGE_UPDATE");
			}
			break;
		case CLAN_SVR_MAP_POS_NOTIFY:
			{
				m_ClanMapPos = msgbody.ClanSvr.ClanSvrData.MapPosInfo.Info;
				GameManager::getSingleton().getScriptVM()->callString("UpdateWorldMapClanMapPos()");
			}
			break;
		case CLAN_SVR_YESTERDAY_CLAN_TOUCH_NOTIFY:
			{
				m_nClanYesterdayTouchNum = msgbody.ClanSvr.ClanSvrData.YesterdayClanTouchNotify.Num;
				GameQuest::getSingleton().updateNpcQuestTitle();
			}
			break;
		case CLAN_SVR_GETRES_INFO:
			{
				static_assert(sizeof(tagClanViewGetRes_ToLua) == sizeof( CSCLANVIEWGETRESSVR ) );
				memcpy( &m_ClanViewGetRes,&msgbody.ClanSvr.ClanSvrData.ViewGetResSvr,sizeof(CSCLANVIEWGETRESSVR) );
				GameManager::getSingleton().getScriptVM()->callString("SetGetResInfo()");
			}
			break;
		case CLAN_SVR_SUMMON_ASK:
			{
				MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
				m_ClanSummonMemId = msgbody.ClanSvr.ClanSvrData.SummonAsk.SummonMemID;
				if ( pMainPlayer == NULL || pMainPlayer->GetID() == m_ClanSummonMemId )
				{
					break;
				}
				int nOverTime		=	msgbody.ClanSvr.ClanSvrData.SummonAsk.OverTime;
				char buff[256];
				sprintf( buff, "ClanMemberSummon( %d, %d, \"%s\" )",nOverTime, msgbody.ClanSvr.ClanSvrData.SummonAsk.Postion, 
							msgbody.ClanSvr.ClanSvrData.SummonAsk.RoleName );
				GameManager::getSingleton().getScriptVM()->callString( buff );
			}
			break;
		case CLAN_SVR_SUMMON_UI_CLOSE:
			{
				char buff[256];
				sprintf( buff, "ClanMemberSummonFrameClose()");
				GameManager::getSingleton().getScriptVM()->callString( buff );
			}
			break;
		case CLAN_SVR_GROUP_INFO:
			{
				static_assert(sizeof(tagClanGroupInfo_ToLua) == sizeof( CLANGROUPINFO ) );
				memcpy( &m_ClanGroupInfo,&msgbody.ClanSvr.ClanSvrData.ClanGroupInfo,sizeof(CLANGROUPINFO) );
			}
			break;
		case CLAN_SVR_DART_INFO:
			{
				char buff[256];
				sprintf( buff, "SetClanDartInfo(%d)",msgbody.ClanSvr.ClanSvrData.ClanDart.EndTime );
				GameManager::getSingleton().getScriptVM()->callString( buff );
			}
			break;
		case CLAN_SVR_SPEND_INFO:
			{
				m_ClanCostLastTime = msgbody.ClanSvr.ClanSvrData.ClanSpend.LastTime;
				m_ClanCostNextTime = msgbody.ClanSvr.ClanSvrData.ClanSpend.NextTime;
				GameManager::getSingleton().getScriptVM()->callString( "CheckMoneyForNextCost()" );
			}
			break;
		}
	}
	else if ( ACTIVE_GAME_SVR == msgid )
	{
		const CSACTIVEGAMESVR& Msg = msgbody.ActiveGameSvr;
		const CSACTIVEGAMEUIINFO& activeGameUIInfo = Msg.Data.UIInfo;
		switch( Msg.Type )
		{
		case ACTIVE_GAME_SVR_TYPE_UI_INFO:
			switch( activeGameUIInfo.UIType )
			{
			case ACTIVE_UI_TYPE_CLAN_PWORLD:
				{
					char szBuf[256];
					const CSACTIVEGAMECLANPWORLD& clanPworld = activeGameUIInfo.Data.ClanPworld;
					sprintf( szBuf, "ClanPworldGameBegin(%d,%d,%d,%d,%d,%d,%d,%d,%d)", clanPworld.ClanContri, clanPworld.ClanMaxVal,
						clanPworld.ClanMoney, clanPworld.ClanVal, clanPworld.Val, clanPworld.EndTime, clanPworld.PlayerMoney ,clanPworld.PlayerExp, clanPworld.BossLife );
					GameManager::getSingleton().getScriptVM()->callString( szBuf );
				}
				break;
			case ACTIVE_UI_TYPE_WORLD_CITY_OPEN:
				{
					GameManager::getSingleton().getScriptVM()->callString("WolrdCityBattleOpen()");
				}
				break;
			case ACTIVE_UI_TYPE_CLAN_TOUCH:
				{
					GameManager::getSingleton().getScriptVM()->callString( "SetClanOpenTouchTips()" );
				}
				break;
			case ACTIVE_UI_TYPE_CLAN_SPY:
				{
					GameManager::getSingleton().getScriptVM()->callString( "ClanSPYGameBegin()" );
					GameManager::getSingleton().SendUIEvent("GE_CLAN_ACTIVE_UPDATE");
				}
				break;
			case ACTIVE_UI_TYPE_CLAN_MACHINE_PWORLD:
				{
					MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
					//如果是联合模式
					if ( activeGameUIInfo.Data.ClanMachine.Flag == 1 )
					{
						pMainPlayer->m_nUnionMachinePworldClanID = activeGameUIInfo.Data.ClanMachine.ClanID;
					}else{
						pMainPlayer->m_nNormalMachinePworldClanID = activeGameUIInfo.Data.ClanMachine.ClanID;
					}
					char szBuf[256];
					sprintf( szBuf, "SetClanMachineWavesInfo(%d,%d)", activeGameUIInfo.Data.ClanMachine.Flag, activeGameUIInfo.Data.ClanMachine.Wave );
					GameManager::getSingleton().getScriptVM()->callString( szBuf );
					GameManager::getSingleton().getScriptVM()->callString( "ClanMachinePworldBegin()" );
					GameManager::getSingleton().SendUIEvent("GE_CLAN_ACTIVE_UPDATE");
				}
				break;
			/*case ACTIVE_UI_TYPE_LUCKY:
				{
					GameManager::getSingleton().getScriptVM()->callString( "HeavenPieActiveBegin()" );
				}
				break;*/
			case ACTIVE_UI_TYPE_GUCHENG:
				{
					char szBuf[256];
					sprintf( szBuf, "GuChengActiveBegin(%d,%d)",activeGameUIInfo.Data.GuCheng.Invaild,activeGameUIInfo.Data.GuCheng.Award );
					GameManager::getSingleton().getScriptVM()->callString( szBuf );
				}
				break;
			}
			break;
		case ACTIVE_GAME_SVR_TYPE_UI_CLOSE:
			switch(Msg.Data.UIClose.UIType)
			{
			case ACTIVE_UI_TYPE_CLAN_PWORLD:
				{
					GameManager::getSingleton().getScriptVM()->callString( "ClanPworldGameEnd()" );
				}
				break;
			case ACTIVE_UI_TYPE_CLAN_TOUCH:
				{
					GameManager::getSingleton().getScriptVM()->callString( "ClanOpenTouchGameEnd()" );
				}
				break;
			case ACTIVE_UI_TYPE_CLAN_SPY:
				{
					GameManager::getSingleton().getScriptVM()->callString( "ClanSPYGameEnd()" );
					GameManager::getSingleton().SendUIEvent("GE_CLAN_ACTIVE_UPDATE");
				}
				break;
			case ACTIVE_UI_TYPE_CLAN_MACHINE_PWORLD:
				{
					MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
					//如果是联合模式
					if ( activeGameUIInfo.Data.ClanMachine.Flag == 1 )
					{
						pMainPlayer->m_nUnionMachinePworldClanID = 0;
					}else{
						pMainPlayer->m_nNormalMachinePworldClanID = 0;
					}
					GameManager::getSingleton().getScriptVM()->callString( "ClanMachinePworldEnd()" );
					GameManager::getSingleton().SendUIEvent("GE_CLAN_ACTIVE_UPDATE");
				}
				break;
			case ACTIVE_UI_TYPE_GUCHENG:
				{
					GameManager::getSingleton().getScriptVM()->callString( "GuChengActiveEnd()" );
				}
				break;
			}
			break;
		}
	}
	else if ( CLAN_PET_SVR == msgid )
	{
		const CSCLANPETSVR& Msg = msgbody.ClanPetSvr;
		switch( Msg.Cmd )
		{
		case CLAN_PET_SVR_OP_ALL:
			{
				static_assert( sizeof(tagClanPetOne_ToLua) == sizeof(tagClanPetOne) );
				m_ClanPetAll.clear();
				const CLANPET& petAll = Msg.Data.All.All;
				m_ClanPetMax	= petAll.MaxNum;
				m_ClanPetFree	= petAll.FreeNum;
				int nClanPetNum	= petAll.Num;
				for( int i=0; i < nClanPetNum; i++ )
				{
					tagClanPetOne_ToLua stClanPetOne;
					memcpy(&stClanPetOne,&petAll.ClanPets[i],sizeof(tagClanPetOne));
					int idx = petAll.ClanPets[i].Idx;
					m_ClanPetAll[idx] = stClanPetOne;
				}
				cleanClanPetInfo();
				char szBuf[256];
				sprintf( szBuf, "SetClanPetAllRailing( %d )",m_ClanPetMax );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case CLAN_PET_SVR_OP_ONE:
			{
				static_assert( sizeof(tagClanPetOne_ToLua) == sizeof(tagClanPetOne) );
				const CLANPETONE& PetOneInfo = Msg.Data.One.One;
				tagClanPetOne_ToLua stClanPetOne;
				memcpy(&stClanPetOne,&Msg.Data.One.One,sizeof(tagClanPetOne));

				std::map<int,tagClanPetOne_ToLua>::iterator it = m_ClanPetAll.find( PetOneInfo.Idx );
				if ( it != m_ClanPetAll.end() && PetOneInfo.PetID == 0 )
				{
					m_ClanPetAll.erase( PetOneInfo.Idx );
					if ( m_ClanPetEntities[PetOneInfo.Idx] )
					{
						m_ClanPetEntities[PetOneInfo.Idx]->detachFromScene();
						OGRE_RELEASE(m_ClanPetEntities[PetOneInfo.Idx]);
					}
				}
				else if ( it != m_ClanPetAll.end() )
				{			
					it->second = stClanPetOne;
				}
				else if ( it == m_ClanPetAll.end() )
				{
					m_ClanPetAll[PetOneInfo.Idx] = stClanPetOne;
					char szBuf[256];
					sprintf( szBuf, "SetClanPetAllRailing( %d )",m_ClanPetMax );
					GameManager::getSingleton().getScriptVM()->callString( szBuf );
				}
				GameManager::getSingleton().SendUIEvent("GE_CLAN_PET_UPDATE");
			}
			break;
		case CLAN_PET_SVR_OP_GRID:
			{
				if( m_ClanPetMax > Msg.Data.Grid.MaxNum )
				{
					cleanClanPetInfo();
				}
				m_ClanPetMax	= Msg.Data.Grid.MaxNum;
				m_ClanPetFree	= Msg.Data.Grid.FreeNum;
				char szBuf[256];
				sprintf( szBuf, "SetClanPetAllRailing( %d )",m_ClanPetMax );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case CLAN_PET_SVR_OP_BOX:
			{
				char szBuf[256];
				sprintf( szBuf, "GetClanPetBoxAward(%d)",Msg.Data.Box.Type );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case CLAN_PET_SVR_OP_GAOJI_SEC:
			{
				m_ClanPetHighLvLastTime = Msg.Data.Sec.Sec;
			}
			break;
		}
	}

	return 0;
}

// 设置公会快照
void GameClanManager::setClanViewer( const tagClanProf& clanProf )
{
	m_ClanViewer.m_clanInfo.m_uClanId			= clanProf.Id;
	m_ClanViewer.m_clanInfo.m_nClanLevel		= clanProf.Level;
	m_ClanViewer.m_clanInfo.m_nClanMemNum		= clanProf.MemNum;
	m_ClanViewer.m_clanInfo.m_nMoney			= clanProf.Money;
	m_ClanViewer.m_clanInfo.m_nBuildingValue	= clanProf.StableValue;
	m_ClanViewer.m_clanInfo.m_nActiveValue		= clanProf.ActiveValue;
	m_ClanViewer.m_clanInfo.m_nSkillPoint		= clanProf.TechPoint;
	m_ClanViewer.m_clanInfo.m_Flag				= clanProf.Flag;
	memcpy( &m_ClanViewer.m_clanInfo.m_clanLongId, &clanProf.GID, sizeof( clanProf.GID ) );
	memcpy( &m_ClanViewer.m_clanInfo.m_szClanName, &clanProf.Name, strlen( clanProf.Name ) + 1 );
}

// 设置工会专属城市信息
void GameClanManager::setClanCity( const tagClanCity& cityInfo )
{
	m_clanCity.setCityInfo( cityInfo );
}

// 获取公会快照
const tagClanViewer& GameClanManager::getClanViewer()const
{
	return m_ClanViewer;
}

// 获取申请入会列表
tagApplyJoinList& GameClanManager::getApplyJoinList()
{
	return m_ApplyJoinList;
}

// 获取公会关系状况
const tagClanDiplomatism& GameClanManager::getClanDipLomatism()const
{
	return m_ClanDipomatism;
}

// 公会成员排序
void GameClanManager::sortMemberList( int nType, int nFlag )
{
	m_nSortType = nType;
	for( int i = 0; i < m_ClanViewer.m_clanInfo.m_nClanMemNum - 1; i++ )
	{
		if( m_ClanViewer.m_clanInfo.m_nClanMemNum <= 1 )
		{
			break;
		}
		for( int j = i; j < m_ClanViewer.m_clanInfo.m_nClanMemNum ; j++ )
		{
			if( cmpMember( m_ClanViewer.m_Members[i], m_ClanViewer.m_Members[j], m_nSortType,nFlag ) < 0 )
			{
				std::swap( m_ClanViewer.m_Members[i], m_ClanViewer.m_Members[j] );
			}
		}
	}
}

// 按在线情况对会员进行排序
void GameClanManager::sortMemberByOnline()
{
	std::sort( m_ClanViewer.m_Members, m_ClanViewer.m_Members + m_ClanViewer.m_clanInfo.m_nClanMemNum );
}

// 获取服务器公会数量
int GameClanManager::getClanTotalNum()
{
	return (int)m_otherClanVec.size();
}

// 获取服务器指定页公会信息
const tagOtherClanInfo* GameClanManager::getOneClanInfoList( int nPage, int nRow, bool isSearch )const
{
	size_t nIndex = nPage*MAXCLANLISTROW + nRow ;
	if( isSearch )
	{
		if( nIndex < m_QueryClanInfoList.size() )
		{ 
			return &m_QueryClanInfoList[nIndex];
		}
	}
	else
	{
		if( nIndex < m_otherClanVec.size() )
		{
			return &m_otherClanVec[nIndex];
		}
	}	
	return NULL;
}

// 获取所查询到的公会数量
int GameClanManager::getQueryClanTotalNum()
{
	return (int)m_QueryClanInfoList.size();
}

// 获取所查询到的公会信息
const tagOtherClanInfo* GameClanManager::getQueryClanInfoList( int nRow ) const
{
	int nCount = 0;
	for( std::vector<tagOtherClanInfo>::const_iterator iter = m_QueryClanInfoList.begin(), 
			end = m_QueryClanInfoList.end(); iter != end; ++iter )
	{
		if (nRow == nCount )
		{
			return &(*iter);
		}
		++nCount;
	}
	return NULL;
}

// 查询某个人是否是工会成员
bool GameClanManager::isClanMember( int nHeight, int nLow )
{
	for ( int i = 0; i < m_ClanViewer.m_clanInfo.m_nClanMemNum; ++i )
	{
		const tagMemberInfo& member = m_ClanViewer.m_Members[i].RoleMember;
		if ( member.m_RoleLongId.nHeight == nHeight && member.m_RoleLongId.nLow == nLow )
		{
			return true;
		}
	}

	return false;
}
// 查询某个人是否是工会成员
bool GameClanManager::isClanMember( const char* pszName )
{
	for ( int i = 0; i < m_ClanViewer.m_clanInfo.m_nClanMemNum; ++i )
	{
		const tagMemberInfo& member = m_ClanViewer.m_Members[i].RoleMember;
		if ( strcmp( member.m_szRoleName, pszName ) == 0 )
		{
			return true;
		}
	}

	return false;
}

//是否是公会官员
bool GameClanManager::isClanOfficer( const char* pszName )
{
	for ( int i = 0; i < m_ClanViewer.m_clanInfo.m_nClanMemNum; ++i )
	{
		const tagMemberInfo& member = m_ClanViewer.m_Members[i].RoleMember;
		if ( strcmp( member.m_szRoleName, pszName ) == 0 && m_ClanViewer.m_Members[i].Position > 0 )
		{
			return true;
		}
	}
	return false;
}

// 重置公会信息
void GameClanManager::resetClanInfo()
{
	m_nSortType = QUICKSORT_BY_ONLINE;
	memset( &m_ClanViewer,		0, sizeof( tagClanViewer ) );
	memset( &m_ApplyJoinList,	0, sizeof( tagApplyJoinList ) );
	memset( &m_ClanDipomatism,	0, sizeof( tagClanDiplomatism ) );
	memset( &m_clanCity,		0, sizeof( m_clanCity ) );
	memset( &m_ClanAdvace,		0, sizeof(tagClanAdvance_ToLua));
	memset( &m_ClanMapPos,		0, sizeof(tagClanMapPos));

	m_ClanCostLastTime = 0;
	m_ClanCostNextTime = 0;

	m_otherClanIDVec.clear();
	m_otherClanVec.clear();
	m_ClanSortType = OTHER_CLAN_SORT_ID;
	m_ClanSortFlag = 1;
	m_uLastRequestTick = 0;
	m_nClanYesterdayTouchNum = 0;
	ClanInfo::getSingleton().resetClanInfo();
	m_ClanPetAll.clear();
	m_ClanPetMax	= 0;
	m_ClanPetFree	= 0;
}
const tagOtherClanInfo* GameClanManager::getOneClanInfoByIndex( int nIdx )const
{
	if( nIdx < m_otherClanVec.size() )
	{
		return &m_otherClanVec[nIdx];
	}
	return NULL;
}

// 设置成员信息
void GameClanManager::setClanInfo( tagOtherClanInfo* pClanInfo, const void* pInfo )
{
	static_assert( sizeof( tagClanCityProf_ToLua ) == sizeof( tagClanCityProf ) );
	tagCSClanView* pClanView = (tagCSClanView*)pInfo;
	static_assert( sizeof( tagClanBrief_ToLua ) == sizeof( CLANBRIEF ) );

	pClanInfo->m_ClanInfo.m_uClanId			= pClanView->ProfInfo.Id;
	pClanInfo->m_ClanInfo.m_nClanLevel		= pClanView->ProfInfo.Level;
	pClanInfo->m_ClanInfo.m_nClanMemNum		= pClanView->ProfInfo.MemNum;
	pClanInfo->m_ClanInfo.m_nMoney			= pClanView->ProfInfo.Money;
	pClanInfo->m_ClanInfo.m_nBuildingValue	= pClanView->ProfInfo.StableValue;
	pClanInfo->m_ClanInfo.m_nActiveValue	= pClanView->ProfInfo.ActiveValue;
	pClanInfo->m_ClanInfo.m_nSkillPoint		= pClanView->ProfInfo.TechPoint;
	pClanInfo->m_ClanInfo.m_Flag			= pClanView->ProfInfo.Flag;
	sprintf( pClanInfo->m_ClanInfo.m_szCaptionName, "%s", pClanView->BriefInfo.Leader.RoleName );
	memcpy( &pClanInfo->m_ClanInfo.m_clanLongId, &pClanView->ProfInfo.GID, sizeof( tdr_ulonglong ) );
	strncpy( pClanInfo->m_ClanInfo.m_szClanName, pClanView->ProfInfo.Name, strlen( pClanView->ProfInfo.Name )+1 );
	pClanInfo->m_ClanBrief = *(tagClanBrief_ToLua*)&pClanView->BriefInfo;
	memcpy( &pClanInfo->m_cityInfo, &pClanView->CityInfo, sizeof( tagClanCityProf_ToLua ) );
}

int GameClanManager::cmpMember( const tagClanMem_ToLua& src, const tagClanMem_ToLua& des, int nType ,int nFlag )
{
	switch( nType )
	{
	case QUICKSORT_BY_POSITION:
		{
			return (nRegulateClanPos[src.Position] - nRegulateClanPos[des.Position] )*nFlag;			
		}
		break;
	case QUICKSORT_BY_LEVEL:
		{
			return (src.Level - des.Level )*nFlag;		
		}
		break;
	case QUICKSORT_BY_CAREER:
		{
			int srcVal = src.Career < nFlag ? (src.Career + MAX_CAREER_NUM ) : src.Career;
			int desVal = des.Career < nFlag ? (des.Career + MAX_CAREER_NUM ) : des.Career;
			return (srcVal - desVal);			
		}
		break;
	case QUICKSORT_BY_ONLINE:
		{
			if( src.Online == 0 && des.Online == 0 )
			{
				return (src.LastOnline - des.LastOnline)*nFlag;
			}else{
				return ( src.Online - des.Online )*nFlag;
			}
		}
		break;
	case QUICKSORT_BY_CONTRIBUTE:
		{
			return ( src.TotalContribute - des.TotalContribute )*nFlag;
		}
		break;
	case QUICKSORT_BY_NAME:
		{
			return strcoll( src.RoleMember.m_szRoleName, des.RoleMember.m_szRoleName )*nFlag;
		}
		break;
	case QUICKSORT_BY_NICKNAME:
		{
			return strcoll( src.NickName, des.NickName )*nFlag;
		}
		break;
	case QUICKSORT_BY_GROUP:
		{
			if ( src.Position == CLAN_POS_NORMAL && des.Position == CLAN_POS_NORMAL )
			{
				return ( src.GroupID - des.GroupID )*nFlag;
			}
			else
			{
				return -1;
			}
		}
		break;
	}
	return -1;
}

int GameClanManager::getServerTime()
{
	int sec, usec;
	ServerTimer::getSingleton().getServerTime( sec, usec );
	return sec;
}

void GameClanManager::requestClanViewInfo()
{
	tagCSPkgBody msgbody;
	if( m_otherClanIDVec.empty() )
	{
		return;
	}
	msgbody.ClanClt.Operation = CLAN_OP_VIEW_PAGE_BY_ID;
	msgbody.ClanClt.ClanCltData.ClanViewPageByID.Num = 0;
	for( size_t i = 0 ; i < MAXCLANLISTROW; i ++ )
	{
		if( i < m_otherClanIDVec.size() )
		{
			msgbody.ClanClt.ClanCltData.ClanViewPageByID.IDs[i] = m_otherClanIDVec[i];
			msgbody.ClanClt.ClanCltData.ClanViewPageByID.Num ++;
		}
	}
	SendGameServerMsg(CLAN_CLT,msgbody);
}

void GameClanManager::modifyOtherClanData(tagCSClanView view )
{
	std::vector<tagOtherClanInfo>::iterator it = std::find(m_otherClanVec.begin(),m_otherClanVec.end(),view.ProfInfo.Id);
	if( it != m_otherClanVec.end() )
	{
		setClanInfo(&(*it),&view);
	}else{
		tagOtherClanInfo clanInfo;
		setClanInfo(&clanInfo,&view);
		m_otherClanVec.push_back( clanInfo );		
	}
	std::vector<unsigned int>::iterator idIt = std::find( m_otherClanIDVec.begin(),m_otherClanIDVec.end(),view.ProfInfo.Id );
	if( idIt != m_otherClanIDVec.end() )
	{
		m_otherClanIDVec.erase(idIt);
	}
	ClanInfo::getSingleton().setClanName(view.ProfInfo.GID, view.ProfInfo.Id, view.ProfInfo.Name);
}

void GameClanManager::deleteOtherClanData( unsigned int clanId )
{
	std::vector<tagOtherClanInfo>::iterator it = std::find(m_otherClanVec.begin(),m_otherClanVec.end(),clanId);
	if( it != m_otherClanVec.end() )
	{
		m_otherClanVec.erase(it);
	}
	it = std::find( m_QueryClanInfoList.begin(),m_QueryClanInfoList.end(),clanId );
	if( it != m_QueryClanInfoList.end() )
	{
		m_QueryClanInfoList.erase(it);
	}
	std::vector<unsigned int>::iterator idIt = std::find( m_otherClanIDVec.begin(),m_otherClanIDVec.end(),clanId );
	if( idIt != m_otherClanIDVec.end() )
	{
		m_otherClanIDVec.erase(idIt);
	}
}

bool otherClanCompare(const tagOtherClanInfo& lhs,const tagOtherClanInfo& rhs )
{
	int nType = GameClanManager::getSingleton().getClanSortType();
	int nFlag = GameClanManager::getSingleton().getClanSortFlag();
	switch( nType )
	{
	case OTHER_CLAN_SORT_ID:
		{
			return ((int)( lhs.m_ClanInfo.m_uClanId - rhs.m_ClanInfo.m_uClanId ))*nFlag > 0?true:false;
		}
		break;
	case OTHER_CLAN_SORT_NAME:
		{
			return strcoll(lhs.m_ClanInfo.m_szClanName,rhs.m_ClanInfo.m_szClanName )*nFlag> 0?true:false;		
		}
		break;
	case OTHER_CLAN_SORT_NUM:
		{
			return ( lhs.m_ClanInfo.m_nClanMemNum - rhs.m_ClanInfo.m_nClanMemNum )*nFlag> 0?true:false;
		}
		break;
	case OTHER_CLAN_SORT_LEVEL:
		{
			return ( lhs.m_ClanInfo.m_nClanLevel - rhs.m_ClanInfo.m_nClanLevel )*nFlag> 0?true:false;
		}
		break;
	case OTHER_CLAN_SORT_COLOR:
		{
			bool result = false;
			do 
			{
				if (GameClanManager::getSingleton().hasClanColor(rhs.m_ClanInfo.m_uClanId))
				{
					result = true;
					break;
				}
				if (GameClanManager::getSingleton().hasClanColor(lhs.m_ClanInfo.m_uClanId))
				{
					result = false;
					break;
				}
				result = GameClanManager::getSingleton().getClanColor(lhs.m_ClanInfo.m_uClanId) <
					GameClanManager::getSingleton().getClanColor(rhs.m_ClanInfo.m_uClanId);
			} while (false);

			if (nFlag < 0)
			{
				result = !result;
			}
			return result;
		}
		break;
	}
	return true;
}

void GameClanManager::changeClanMemberName( const char* pszOldName, const char* pszNewName )
{
	bool bChangeName = false;
	for ( int i = 0; i < m_ClanViewer.m_clanInfo.m_nClanMemNum; ++i )
	{
		tagClanMem_ToLua& oneClanMemberInfo = m_ClanViewer.m_Members[i];
		if ( strncmp( oneClanMemberInfo.RoleMember.m_szRoleName, pszOldName, ROLE_NAME_LEN_SAVE ) == 0 )
		{
			strncpy( oneClanMemberInfo.RoleMember.m_szRoleName, pszNewName, ROLE_NAME_LEN_SAVE );
			bChangeName = true;
			break;
		}
	}
	if ( strncmp( m_ClanViewer.m_PresidentInfo.m_szRoleName, pszOldName, ROLE_NAME_LEN_SAVE ) == 0 )
	{
		strncpy( m_ClanViewer.m_PresidentInfo.m_szRoleName, pszNewName, ROLE_NAME_LEN_SAVE );
		bChangeName = true;
	}
	if( strncmp( m_ClanViewer.m_clanInfo.m_szCaptionName, pszOldName, ROLE_NAME_LEN_SAVE ) == 0 )
	{
		strncpy( m_ClanViewer.m_clanInfo.m_szCaptionName, pszNewName, ROLE_NAME_LEN_SAVE );
		bChangeName = true;
	}

	for( int i = 0 ; i < m_QueryClanInfoList.size() ; i++ )
	{
		if( strncmp( m_QueryClanInfoList[i].m_ClanInfo.m_szCaptionName, pszOldName, ROLE_NAME_LEN_SAVE ) == 0 )
		{
			strncpy( m_QueryClanInfoList[i].m_ClanInfo.m_szCaptionName, pszNewName, ROLE_NAME_LEN_SAVE );
			bChangeName = true;
		}
		if( strncmp( m_QueryClanInfoList[i].m_ClanBrief.Leader.m_szRoleName, pszOldName, ROLE_NAME_LEN_SAVE ) == 0 )
		{
			strncpy( m_QueryClanInfoList[i].m_ClanBrief.Leader.m_szRoleName, pszNewName, ROLE_NAME_LEN_SAVE );
			bChangeName = true;
		}
		for( int j = 0 ; j < m_QueryClanInfoList[i].m_ClanBrief.AssistNum ; j++ )
		{
			if( strncmp( m_QueryClanInfoList[i].m_ClanBrief.Assists[j].m_szRoleName, pszOldName, ROLE_NAME_LEN_SAVE ) == 0 )
			{
				strncpy( m_QueryClanInfoList[i].m_ClanBrief.Assists[j].m_szRoleName, pszNewName, ROLE_NAME_LEN_SAVE );
				bChangeName = true;
			}
		}
	}
	
	for( int i = 0 ; i < m_ApplyJoinList.m_nRoleNum ; i ++ )
	{
		if( strncmp( m_ApplyJoinList.m_ApplyRoles[i].RoleMember.m_szRoleName, pszOldName, ROLE_NAME_LEN_SAVE ) == 0 )
		{
			strncpy( m_ApplyJoinList.m_ApplyRoles[i].RoleMember.m_szRoleName, pszNewName, ROLE_NAME_LEN_SAVE );
			bChangeName = true;
			break;
		}
	}
	for( int i = 0 ; i < m_ClanDipomatism.m_nFriendNum ; i++ )
	{
		if( strncmp( m_ClanDipomatism.m_Friends[i].m_szCaptionName, pszOldName, ROLE_NAME_LEN_SAVE ) == 0 )
		{
			strncpy( m_ClanDipomatism.m_Friends[i].m_szCaptionName, pszNewName, ROLE_NAME_LEN_SAVE );
			bChangeName = true;
			break;
		}
	}
	for( int i = 0 ; i < m_ClanDipomatism.m_nEnemyNum ; i++ )
	{
		if( strncmp( m_ClanDipomatism.m_Enemys[i].m_szCaptionName, pszOldName, ROLE_NAME_LEN_SAVE ) == 0 )
		{
			strncpy( m_ClanDipomatism.m_Enemys[i].m_szCaptionName, pszNewName, ROLE_NAME_LEN_SAVE );
			bChangeName = true;
			break;
		}
	}
	for( int i = 0 ; i < m_ClanDipomatism.m_nApplyEnemyNum ; i++ )
	{
		if( strncmp( m_ClanDipomatism.m_ApplyEnemys[i].m_ClanInfo.m_szCaptionName, pszOldName, ROLE_NAME_LEN_SAVE ) == 0 )
		{
			strncpy( m_ClanDipomatism.m_ApplyEnemys[i].m_ClanInfo.m_szCaptionName, pszNewName, ROLE_NAME_LEN_SAVE );
			bChangeName = true;
			break;
		}
	}
	for( int i = 0 ; i < m_ClanDipomatism.m_nApplyFriendNum ; i++ )
	{
		if( strncmp( m_ClanDipomatism.m_ApplyFriends[i].m_ClanInfo.m_szCaptionName, pszOldName, ROLE_NAME_LEN_SAVE ) == 0 )
		{
			strncpy( m_ClanDipomatism.m_ApplyFriends[i].m_ClanInfo.m_szCaptionName, pszNewName, ROLE_NAME_LEN_SAVE );
			bChangeName = true;
			break;
		}
	}
	for( int i = 0; i < m_vecSortDonateInfo.size(); ++i )
	{
		if( strncmp( m_vecSortDonateInfo[i].RoleMember.m_szRoleName, pszOldName, ROLE_NAME_LEN_SAVE ) == 0 )
		{
			strncpy( m_vecSortDonateInfo[i].RoleMember.m_szRoleName, pszNewName, ROLE_NAME_LEN_SAVE );
			bChangeName = true;
			break;
		}
	}
	for( int i = 0 ; i < m_ClanViewGetRes.InfoNum; ++i )
	{
		if( strncmp( m_ClanViewGetRes.GetResInfos[i].Name, pszOldName, ROLE_NAME_LEN ) == 0 )
		{
			strncpy( m_ClanViewGetRes.GetResInfos[i].Name, pszNewName, ROLE_NAME_LEN );
			bChangeName = true;
			break;
		}
	}
	if( bChangeName )
	{
		GameManager::getSingleton().SendUIEvent( "GE_CHG_CLAN_MEMBER_NAME" );
	}
}

void GameClanManager::sortOtherClan( int nType, int nFlag,bool bSearch )
{
	m_ClanSortType = nType;
	m_ClanSortFlag = nFlag;
	if( bSearch )
	{
		std::stable_sort( m_QueryClanInfoList.begin(),m_QueryClanInfoList.end(),otherClanCompare );
	}else{
		std::stable_sort( m_otherClanVec.begin(),m_otherClanVec.end(),otherClanCompare );
	}
	
}
void GameClanManager::UpdateClanPetNpcTalk()
{
	int nNpcMemId = ExtraActorMgr::getSingleton().getNpcTalkID();
	if ( nNpcMemId == 0 )
	{
		return;
	}
	int nLastNpc	= ExtraActorMgr::getSingleton().getLastNpcTalkID();
	if ( nLastNpc == nNpcMemId )
	{
		return;
	}
	MainGamePlayer*		pMainPlayer	= ActorManager::getSingleton().getMainPlayer();
	GameActor *			pNpc		= ExtraActorMgr::getSingleton().FindActor( nNpcMemId );
	if ( pNpc == NULL )
	{
		return;
	}
	GameXYZ pos2(0,0,0);
	GameXYZ pos	= pMainPlayer->GetPosition();
	pos.y	= 0;
	pos2	= pNpc->GetPosition();
	pos2.y	= 0;

	unsigned int Dis = (unsigned int)pos2.subtract(pos).length();
	const tagMonsterDef& monsterDef = DBDataManager::getSingleton().getMonsterDef( pNpc->getResId() );
	int nMaxTalkDist = GameNpcDialog::getSingleton().getMaxNpcTalkDist( pNpc->getResId() ) - 20;

	int nTalkResult = MOVE_TIME_INTERVAL_SMALL;

	if ( Dis <= nMaxTalkDist )
	{
		ExtraActorMgr::getSingleton().setLastNpcTalkID( nNpcMemId );
		char buff[256];
		sprintf( buff, "OpenClanPetRailing(%d)",pNpc->GetID() );
		GameManager::getSingleton().getScriptVM()->callString( buff );
	}
}
void GameClanManager::update()
{
	if( !m_otherClanIDVec.empty() )
	{
		if( GameManager::getSingleton().getTickTime() - m_uLastRequestTick >=1*1000 )
		{
			requestClanViewInfo();
			m_uLastRequestTick = GameManager::getSingleton().getTickTime();
		}
	}
	
	UpdateClanPetNpcTalk();
}

tagClanTechDef_ToLua* GameClanManager::getClanTechDef( int nCat,int nLevel )
{
	static_assert( sizeof( tagClanTechDef_ToLua ) == sizeof( tagClanTechDef ) );
	return (tagClanTechDef_ToLua*) DBDataManager::getSingleton().getClanTechDef( nCat,nLevel );
}

int GameClanManager::getMaxSkillLevel( int nCat )
{
	int nLevel = 1;
	const tagClanTechDef* pDef = DBDataManager::getSingleton().getClanTechDef( nCat,nLevel );
	if( pDef == NULL )
	{
		return 0;
	}
	while( pDef != NULL )
	{
		pDef = DBDataManager::getSingleton().getClanTechDef( nCat,nLevel );
		if( pDef != NULL )
		{
			nLevel++;
		}
	}
	return --nLevel;
}

tagClanRunningProgress_ToLua* GameClanManager::getTechProgress( int nCat )
{
	for( int i = 0 ; i < m_ClanAdvace.Tech.TechResearchNum ; i ++ )
	{
		if( m_ClanAdvace.Tech.TechResearchProgress[i].Category == nCat )
		{
			return ( tagClanRunningProgress_ToLua* )( &m_ClanAdvace.Tech.TechResearchProgress[i] );
		}
	}
	return NULL;
}

tagClanRunningProgress_ToLua* GameClanManager::getProductProgress(int nID)
{
	for( int i = 0 ; i < m_ClanAdvace.Produce.ArmMakingNum ; i++ )
	{
		if( m_ClanAdvace.Produce.ArmMakingProgress[i].ID == nID )
		{
			return &m_ClanAdvace.Produce.ArmMakingProgress[i];
		}
	}
	return NULL;
}
tagClanRunningProgress_ToLua* GameClanManager::getProductProgressByIdx(int nIdx )
{
	if( m_ClanAdvace.Produce.ArmMakingNum > nIdx )
	{
		return &m_ClanAdvace.Produce.ArmMakingProgress[nIdx];
	}
	return NULL;
}


tagClanAdvance_ToLua* GameClanManager::getAdvanceInfo()
{
	return &m_ClanAdvace;
}

int GameClanManager::getProductNum()
{
	return DBDataManager::getSingleton().getClanProductDefNum();
}

tagClanProductDef_ToLua* GameClanManager::getProductDefByID( int nID )
{
	static_assert( sizeof(tagClanProductDef_ToLua) == sizeof(tagClanProductDef) );
	return (tagClanProductDef_ToLua*)DBDataManager::getSingleton().getClanProductDef(nID);
}

tagClanProductDef_ToLua* GameClanManager::getProductDefByIndex(int nIndex)
{
	return (tagClanProductDef_ToLua*)DBDataManager::getSingleton().getClanProductDefByIndex( nIndex );
}

tagClanMachineProp_ToLua* GameClanManager::getMachineProp( int nMonsterID )
{
	static tagClanMachineProp_ToLua machineProp;
	memset(&machineProp,0,sizeof(tagClanMachineProp_ToLua));
	const MONSTERDEF& pMonsterDef = DBDataManager::getSingleton().getMonsterDef( nMonsterID );
	const MONMACHINEDEF* pMachineDef = DBDataManager::getSingleton().getMonMachineDef(nMonsterID);
	if( pMachineDef != NULL )
	{
		machineProp.MaxHp = pMonsterDef.HP;
		machineProp.Fuel = pMachineDef->MaxEnergy;
		machineProp.CollectSpd = pMachineDef->CollcetTime;
		machineProp.MoveSpd = pMonsterDef.MoveSpd;
		machineProp.Atk = pMonsterDef.PhsicAtk;
		machineProp.Def = pMonsterDef.PhsicDef;
		machineProp.CollectCap = pMachineDef->MaxPackOre;
		for( int i = 0 ; i < MAX_CLAN_TECH_CATEGORY ; i++ )
		{
			if( m_ClanAdvace.Tech.TechLevel[i] > 0 )
			{
				for( int lv=1;lv<=m_ClanAdvace.Tech.TechLevel[i];lv++ )
				{
					const CLANTECHDEF* pTechDef = DBDataManager::getSingleton().getClanTechDef( i,lv );
					for( int j = 0 ; j < MAX_CLAN_TECH_RESULT; j ++ )
					{
						if( pTechDef->Result[j].ID == CLAN_TECH_COLLECTOR_ADD_HP && pTechDef->Result[j].Value1 == nMonsterID )
						{
							machineProp.MaxHp += pTechDef->Result[j].Value2;
						}
						if( pTechDef->Result[j].ID == CLAN_TECH_COLLECTOR_ADD_DEF && pTechDef->Result[j].Value1 == nMonsterID )
						{
							machineProp.Def += pTechDef->Result[j].Value2;
						}
						if( pTechDef->Result[j].ID == CLAN_TECH_COLLECTOR_ADD_ATK && pTechDef->Result[j].Value1 == nMonsterID )
						{
							machineProp.Atk += pTechDef->Result[j].Value2;
						}
						if( pTechDef->Result[j].ID == CLAN_TECH_COLLECTOR_ADD_MOVE_SPEED && pTechDef->Result[j].Value1 == nMonsterID )
						{
							machineProp.MoveSpd += pTechDef->Result[j].Value2;
						}
						if( pTechDef->Result[j].ID == CLAN_TECH_COLLECTOR_ADD_COLLECT_SPEED && pTechDef->Result[j].Value1 == nMonsterID )
						{
							machineProp.CollectSpd += pTechDef->Result[j].Value2;
						}
						if( pTechDef->Result[j].ID == CLAN_TECH_COLLECTOR_ADD_CAP && pTechDef->Result[j].Value1 == nMonsterID )
						{
							machineProp.CollectCap += pTechDef->Result[j].Value2;
						}
					}
				}
			}
		}
	}
	return &machineProp;
}

const tagClanMem_ToLua* GameClanManager::getMemberInfo(const char* szName)
{
	for ( int i = 0; i < m_ClanViewer.m_clanInfo.m_nClanMemNum; ++i )
	{
		if ( strcmp( m_ClanViewer.m_Members[i].RoleMember.m_szRoleName, szName ) == 0 )
		{
			return &m_ClanViewer.m_Members[i];
		}
	}
	return NULL;
}

void GameClanManager::onLeaveClanMap()
{
	if( m_ClanCityEntity )
	{
		m_ClanCityEntity->detachFromScene();
		OGRE_RELEASE(m_ClanCityEntity);
	}
	for( int i = 0 ; i < MAX_CLAN_BUILDING ; i++ )
	{
		if( m_CityEntities[i] )
		{
			m_CityEntities[i]->detachFromScene();
			OGRE_RELEASE(m_CityEntities[i]);
		}
	}
	cleanClanPetInfo();
	m_CurClanCityId = 0;
	memset(&m_ClanCityInfo, 0, sizeof(m_ClanCityInfo));
	m_vClanNpcs.clear();
	GameManager::getSingleton().SendUIEvent("GE_UPDATE_NPC");
	GameManager::getSingleton().SendUIEvent("GE_CLAN_PET_LEAVE");
}

void GameClanManager::onEnterClanMap()
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if( m_ClanCityInfo.Level > 0 )
	{
		for( int i = 0 ; i < MAX_CLAN_BUILDING ; i++ )
		{
			const tagClanBuildingDisplayInfo* pInfo = DBDataManager::getSingleton().getBuildingDisplayInfo( i );
			int nLevel = m_ClanCityInfo.Buildings[i].Level;
			if( pInfo )
			{
				const CLANBUILDINGENTITY& entityInfo = pInfo->Entities[nLevel];
				int y=0;
				pMain->getCurMap()->getTerrainH( entityInfo.X*10,entityInfo.Y*10,y );
				Ogre::WorldPos pos( Ogre::Vector3((float)entityInfo.X*10,(float)y,(float)entityInfo.Y*10) );
				pos = pos/10;
				if( m_CityEntities[i] )
				{
					m_CityEntities[i]->detachFromScene();
					OGRE_RELEASE(m_CityEntities[i]);
				}				
				m_CityEntities[i] = new Ogre::Entity;
				m_CityEntities[i]->load( entityInfo.Model,true );
				m_CityEntities[i]->setRotation( entityInfo.Pitch,entityInfo.Yaw,entityInfo.Roll );
				m_CityEntities[i]->setPosition( pos );
				m_CityEntities[i]->setScale( Ogre::Vector3((float)entityInfo.Scale,(float)entityInfo.Scale,(float)entityInfo.Scale) );
				m_CityEntities[i]->update(0);
				m_CityEntities[i]->attachToScene( pMain->getCurMap()->getShowGameMap()->getGameScene() );
			}
		}
	}

	//公会城市的模型
	const tagClanBuildingDisplayInfo* pInfo = DBDataManager::getSingleton().getBuildingDisplayInfo( 5 );
	if( pInfo )
	{
		const CLANBUILDINGENTITY& entityInfo = pInfo->Entities[m_ClanCityInfo.Level];
		if( m_ClanCityEntity )
		{
			m_ClanCityEntity->detachFromScene();
			OGRE_RELEASE(m_ClanCityEntity);
		}
		OGRE_RELEASE(m_ClanCityEntity);
		m_ClanCityEntity = new Ogre::Entity;
		int y=0;
		pMain->getCurMap()->getTerrainH( entityInfo.X*10,entityInfo.Y*10,y );
		Ogre::WorldPos pos( Ogre::Vector3((float)entityInfo.X*10,(float)y,(float)entityInfo.Y*10) );
		pos = pos/10;
		m_ClanCityEntity->load( entityInfo.Model,true );
		m_ClanCityEntity->setPosition(pos);
		m_ClanCityEntity->setRotation( entityInfo.Pitch,entityInfo.Yaw,entityInfo.Roll );
		m_ClanCityEntity->setScale(Ogre::Vector3((float)entityInfo.Scale,(float)entityInfo.Scale,(float)entityInfo.Scale));
		m_ClanCityEntity->update( 0 );	
		m_ClanCityEntity->attachToScene( pMain->getCurMap()->getShowGameMap()->getGameScene() );
	}
	m_vClanNpcs.clear();
	const tagClanCityLevelDef *ClanCityLevelDef = DBDataManager::getSingleton().getClanCityLevelDef(m_ClanCityInfo.Level);
	if (ClanCityLevelDef != NULL)
	{
		for (size_t i = 0; i < CLAN_MAX_NPC_NUM; ++i)
		{
			if (ClanCityLevelDef->ExtraNpcs[i].Show)
			{
				const tagClanNpc &npc = ClanCityLevelDef->ExtraNpcs[i];
				if (npc.NpcID != 0)
				{
					tagMapActorInfo npcInfo;
					npcInfo.m_nId		= npc.NpcID;	
					npcInfo.m_nMapId	= CLAN_CITY_MAP_ID;
					npcInfo.m_nRealX	= npc.Pos.X;
					npcInfo.m_nRealY	= npc.Pos.Y;
					npcInfo.m_bShow		= true;
					strncpy(npcInfo.m_szName, DBDataManager::getSingleton().getMonsterDef(npcInfo.m_nId).Name, NAME_LEN);
					m_vClanNpcs.push_back(npcInfo);
				}
			}
		}
	}
	GameManager::getSingleton().SendUIEvent( "GE_UPDATE_NPC" );
}

void GameClanManager::checkClanBuildChange()
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if( pMain&& pMain->getCurMap() && pMain->getCurMap()->getMapId() == CLAN_CITY_MAP_ID )
	{
		onEnterClanMap();
	}
}
bool GameClanManager::IsInClanPworldIsland()
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if ( pMain && pMain->getCurMap() 
		&& pMain->getCurMap()->getMapId() == CLAN_EVERYDAY_PWORLD_MAP_ID )
	{
		return true;
	}
	return false;
}
bool GameClanManager::IsInClanMachinePworld()
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if ( pMain && pMain->getCurMap() 
		&& pMain->getCurMap()->getMapId() == CLAN_MACHINE_MAP_ID )
	{
		return true;
	}
	return false;
}
bool GameClanManager::isFriendClan( tagLong_ToLua clanWid )
{
	for( int i = 0 ; i < m_ClanDipomatism.m_nFriendNum ; i ++ )
	{
		if( memcmp( &m_ClanDipomatism.m_Friends[i].m_clanLongId,&clanWid,sizeof(tagLong_ToLua) ) == 0 )
		{
			return true;
		}
	}
	return false;
}

bool GameClanManager::isEnemyClan( tagLong_ToLua clanWid )
{
	for( int i = 0 ; i < m_ClanDipomatism.m_nEnemyNum ; i++ )
	{
		if( memcmp( &m_ClanDipomatism.m_Enemys[i].m_clanLongId,&clanWid,sizeof(tagLong_ToLua) ) == 0 )
		{
			return true;
		}
	}
	return false;
}

void GameClanManager::updateAllClan()
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer == NULL)
	{
		return;
	}
	tdr_ulonglong wId = pMainPlayer->getClanWID();
	ACTOR_ITER begin, end;
	begin = ActorManager::getSingleton().getActorBegin();
	end = ActorManager::getSingleton().getActorEnd();
	while (begin != end)
	{
		GameActor *pActor = begin->second;
		++begin;

		if (pActor->getType() != GAT_PLAYER && pActor->getType() != GAT_MONSTER && pActor->getType() != GAT_NPC && pActor->getType() != GAT_MAINPLAYER)
		{
			continue;
		}
		pActor->resetClan();
		tdr_ulonglong k = pActor->getClanWID();
		tagLong_ToLua tolua;
		memcpy(&tolua, &k, sizeof(tagLong_ToLua));
		if (pMainPlayer->isInClan())
		{
			if (wId == k)
			{
				pActor->setClanSame();
			}
			else if (isFriendClan(tolua))
			{
				pActor->setClanAlly();
			}
			else if (isEnemyClan(tolua))
			{
				pActor->setClanEnemy();
			}
		}
		if (hasClanColor(tolua))
		{
			pActor->setClanColor(getClanColor(tolua));
		}
	}
}

void GameClanManager::objEnterView(GameActor *actor)
{
	if (actor->getType() != GAT_PLAYER && actor->getType() != GAT_MONSTER && actor->getType() != GAT_NPC)
	{
		return;
	}
	updateClan(actor);
}

void GameClanManager::updateClan(GameActor *pActor)
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer == NULL)
	{
		return;
	}
	pActor->resetClan();
	tdr_ulonglong wId = pActor->getClanWID();
	tagLong_ToLua tolua;
	memcpy(&tolua, &wId, sizeof(tagLong_ToLua));
	if (pMainPlayer->isInClan())
	{
		if (pMainPlayer->getClanWID() == wId)
		{
			pActor->setClanSame();
		}
		else if (isFriendClan(tolua))
		{
			pActor->setClanAlly();
		}
		else if (isEnemyClan(tolua))
		{
			pActor->setClanEnemy();
		}
	}
	if (hasClanColor(tolua))
	{
		pActor->setClanColor(getClanColor(tolua));
	}
}

int GameClanManager::CLanMenmberOnlineinfo( const char* pszName )
{
	for ( int i = 0; i < m_ClanViewer.m_clanInfo.m_nClanMemNum; ++i )
	{
		const tagMemberInfo& member = m_ClanViewer.m_Members[i].RoleMember;
		if ( strcmp( member.m_szRoleName, pszName ) == 0  )
		{
			return m_ClanViewer.m_Members[i].Online;
		}
	}
	return 0;
}

void GameClanManager::changeClanName( tdr_ulonglong wid, const char* szNewName )
{
	if( memcmp( &m_ClanViewer.m_clanInfo.m_clanLongId, &wid,sizeof(tdr_ulonglong)) == 0 )
	{
		strncpy( m_ClanViewer.m_clanInfo.m_szClanName,szNewName,NAME_LEN-1 );
		GameManager::getSingleton().SendUIEvent( "GE_CLAN_SVR_INFO" );
	}
	for( size_t i = 0 ; i < m_otherClanVec.size() ; i++ )
	{
		if( memcmp( &m_otherClanVec[i].m_ClanInfo.m_clanLongId, &wid,sizeof(tdr_ulonglong)) == 0 )
		{
			strncpy( m_otherClanVec[i].m_ClanInfo.m_szClanName,szNewName,NAME_LEN-1 );
			GameManager::getSingleton().SendUIEvent( "GE_CLANLIST_UPDATE" );
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////

int GameClanManager::getStorageItemNum()
{
	return m_ClanAdvace.Resource.ItemNum;
}

tagClanResourceInfo_ToLua * GameClanManager::getStorageItemInfo(int idx)
{
	return &m_ClanAdvace.Resource.ItemInfo[idx];
}

int GameClanManager::getStorageResourceNum(int resouceId)
{
	for (int i = 0; i < m_ClanAdvace.Resource.ResourceNum; ++i)
	{
		if (m_ClanAdvace.Resource.ResourceInfo[i].ResourceID == resouceId)
		{
			return m_ClanAdvace.Resource.ResourceInfo[i].Num;
		}
	}

	return 0;
}

void GameClanManager::lockSwitchStorage(int resourceId, int state)
{
	if( resourceId < m_ClanAdvace.Resource.ItemNum )
	{
		tagCSPkgBody msgbody;
		msgbody.ClanClt.Operation = CLAN_OP_RESOURCE_LOCK_SWITCH;
		msgbody.ClanClt.ClanCltData.ClanResourceLockSwitch.ResourceID = m_ClanAdvace.Resource.ItemInfo[resourceId].Pos;
		msgbody.ClanClt.ClanCltData.ClanResourceLockSwitch.LockState = state;
		SendGameServerMsg(CLAN_CLT, msgbody);
	}
}

void GameClanManager::lockSwitchStorageByIdx(int idx, int state)
{
	lockSwitchStorage(getStorageItemInfo(idx)->ResourceID, state);
}

void GameClanManager::exchangeStorageItem(int resourceId, int num)
{
	if( resourceId < m_ClanAdvace.Resource.ItemNum )
	{
		tagCSPkgBody msgbody;
		msgbody.ClanClt.Operation = CLAN_OP_RESOURCE_GRAB_ITEM;
		msgbody.ClanClt.ClanCltData.ClanResourceGrabItem.ResourceID = m_ClanAdvace.Resource.ItemInfo[resourceId].Pos;
		msgbody.ClanClt.ClanCltData.ClanResourceGrabItem.Num = num;
		SendGameServerMsg(CLAN_CLT, msgbody);
	}
}

void GameClanManager::exchangeStorageItemByIdx(int idx, int num)
{
	exchangeStorageItem(getStorageItemInfo(idx)->ResourceID, num);
}

unsigned int GameClanManager::getDefaultClanColor(const tagLong_ToLua &clanWid)
{
	if (memcmp(&m_ClanViewer.m_clanInfo.m_clanLongId, &clanWid, sizeof(tagLong_ToLua)) == 0)
	{
		return CLAN_TITLE_MY_COLOR;
	}
	if (isFriendClan(clanWid))
	{
		return CLAN_TITLE_ALLY_COLOR;
	}
	if (isEnemyClan(clanWid))
	{
		return CLAN_TITLE_ENEMY_COLOR;
	}
	return CLAN_TITLE_OTHER_COLOR;
}

unsigned int GameClanManager::getDefaultClanColor(unsigned int id)
{
	std::map<unsigned int, tdr_ulonglong>::iterator i = m_ClanIDMapReverse.find(id);
	if (i != m_ClanIDMapReverse.end())
	{
		return getDefaultClanColor(*((tagLong_ToLua *)(&i->second)));
	}
	return CLAN_TITLE_OTHER_COLOR;
}

unsigned int GameClanManager::getClanColor(const tagLong_ToLua &clanWid)
{
	std::map<tdr_ulonglong, unsigned int>::iterator i = m_ClanIDMap.find(*((tdr_ulonglong *)(&clanWid)));
	if (i == m_ClanIDMap.end())
	{
		return getDefaultClanColor(clanWid);
	}
	return getClanColor(i->second);
}

unsigned int GameClanManager::getClanColor(unsigned int id)
{
	std::map<unsigned int, unsigned int>::iterator i = m_ClanColor.find(id);
	if (i == m_ClanColor.end())
	{
		return getDefaultClanColor(id);
	}
	return i->second;
}

bool GameClanManager::hasClanColor(const tagLong_ToLua &clanWid)
{
	std::map<tdr_ulonglong, unsigned int>::iterator i = m_ClanIDMap.find(*((tdr_ulonglong *)(&clanWid)));
	if (i == m_ClanIDMap.end())
	{
		return false;
	}
	return hasClanColor(i->second);
}

bool GameClanManager::hasClanColor(unsigned int id)
{
	std::map<unsigned int, unsigned int>::iterator i = m_ClanColor.find(id);
	if (i == m_ClanColor.end())
	{
		return false;
	}
	return i->second != getDefaultClanColor(id);
}

tagClanMapPos_ToLua* GameClanManager::getClanMapPos()
{
	static_assert( sizeof(tagClanMapPos) == sizeof(tagClanMapPos_ToLua) );
	return (tagClanMapPos_ToLua*)&m_ClanMapPos;
}

void GameClanManager::SetClanCityInfo(int id, const tagClanCity &info)
{
	m_CurClanCityId = id;
	m_ClanCityInfo = info;
}

int GameClanManager::getClanNpcsCount()
{
	return m_vClanNpcs.size();
}

const tagMapActorInfo & GameClanManager::getClanNpcInfo(int index)
{
	return m_vClanNpcs[index];
}

const tagMapActorInfo * GameClanManager::getClanNpcInfoById(int id)
{
	for (int i = 0, j = m_vClanNpcs.size(); i < j; ++i)
	{
		if (m_vClanNpcs[i].m_nId == id)
		{
			return &(m_vClanNpcs[i]);
		}
	}
	return NULL;
}

const tagClanSpyTransDef_ToLua*	GameClanManager::getClanSpyTransDef( int id )
{
	static_assert( sizeof(tagClanSpyTransDef_ToLua) == sizeof(tagClanSpyTransDef) );
	return (tagClanSpyTransDef_ToLua*)DBDataManager::getSingleton().getClanSpyTransDef( id );
}

int GameClanManager::GetClanDonateLogNum()
{
	return (int)m_vecSortDonateInfo.size();
}

void GameClanManager::updateClanDonateInfo()
{
	m_vecSortDonateInfo.clear();
	for( int i=0; i < m_ClanViewer.m_clanInfo.m_nClanMemNum; i++ )
	{
		if( m_ClanViewer.m_Members[i].DonateLastTime > 0 )
		{
			m_vecSortDonateInfo.push_back( m_ClanViewer.m_Members[i] );
		}
	}
	GameManager::getSingleton().SendUIEvent("GE_CLAN_DONATE_LOG_UPDATE");
}

const tagClanMem_ToLua* GameClanManager::GetClanDonateLog( int nIndex )
{
	assert( nIndex < (int)m_vecSortDonateInfo.size() );
	return &m_vecSortDonateInfo[nIndex];
}

void GameClanManager::SortDonateLog( int nType, bool bFlag )
{
	m_nDonateSortType = nType;
	m_bFlag = bFlag;
	std::stable_sort( m_vecSortDonateInfo.begin(), m_vecSortDonateInfo.end(), compareLogNum );
}

bool compareLogNum( const tagClanMem_ToLua& lhs, const tagClanMem_ToLua& rhs )
{	
	int nType = GameClanManager::getSingleton().getDonateSortType();
	bool bFlag = GameClanManager::getSingleton().getSortFlag();
	switch( nType )
	{
	case TIME_SORT:
		{
			return lhs.DonateLastTime > rhs.DonateLastTime;
		}
		break;
	case MONEY_SORT:
		{
			if( !bFlag )
			{
				return lhs.DonateNum < rhs.DonateNum;
			}
			return lhs.DonateNum > rhs.DonateNum;
		}
		break;
	case CLAN_SORT:
		{	
			if( !bFlag )
			{
				return lhs.DonateClanMoney < rhs.DonateClanMoney;
			}
			return lhs.DonateClanMoney > rhs.DonateClanMoney;
		}
		break;
	case CONTRI_SORT:
		{
			if( !bFlag )
			{
				return lhs.DonateContri < rhs.DonateContri;
			}
			return lhs.DonateContri > rhs.DonateContri;
		}
		break;
	default:
		break;
	}
}

const tagClanSkillDef_ToLua* GameClanManager::getClanSkillDef( int id, int lv )
{
	static_assert( sizeof(tagClanSkillDef) == sizeof(tagClanSkillDef_ToLua) );
	return ((const tagClanSkillDef_ToLua *)&DBDataManager::getSingleton().getClanSkillDef( id, lv ));
}

const tagClanSkillDef_ToLua* GameClanManager::getClanSkillDefByIndex( int nIndex )
{
	assert( nIndex < (int)m_vClanSkill.size() );
	return (const tagClanSkillDef_ToLua *)(m_vClanSkill[nIndex]);
}

const tagClanSkillInfo_ToLua* GameClanManager::getClanSkillInfo()
{
	return &m_ClanSkill;
}

const tagPlayerClanSkillInfo_ToLua* GameClanManager::getPlayerClanSkillInfo()
{
	return &m_PlayerCLanSkill;
}

void GameClanManager::updateClanSkillInfo( const tagClanSkillInfo &info )
{
	memcpy( &m_ClanSkill, &info, sizeof(tagClanSkillInfo) );
	GameManager::getSingleton().SendUIEvent("GE_CLAN_SKILL_UPDATE");
}

void GameClanManager::updatePlayerClanSkillInfo( const tagPlayerClanSkillInfo &info )
{
	memcpy( &m_PlayerCLanSkill, &info, sizeof(tagPlayerClanSkillInfo) );
	GameManager::getSingleton().SendUIEvent("GE_PLAYER_CLAN_SKILL_UPDATE");
}

void GameClanManager::Clan_Opt_SummonRequest( bool bAgree )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_CLAN_SUMMON_ANSWER;
	if (bAgree)
	{
		msgbody.ClanClt.ClanCltData.SummonAnswer.SummonMemID	= m_ClanSummonMemId;
		msgbody.ClanClt.ClanCltData.SummonAnswer.Yes			= 1;
	}
	else
	{
		msgbody.ClanClt.ClanCltData.SummonAnswer.SummonMemID	= m_ClanSummonMemId;
		msgbody.ClanClt.ClanCltData.SummonAnswer.Yes			= 0;
	}
	SendGameServerMsg( CLAN_CLT,msgbody );
}
void GameClanManager::Clan_Opt_SetGroup( int roleIdx, int groupIdx, int set )
{
	tagClanViewer clanviewer = getClanViewer();
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_SET_GROUP;
	msgbody.ClanClt.ClanCltData.ClanSetGroup.Group = groupIdx;
	memcpy( &msgbody.ClanClt.ClanCltData.ClanSetGroup.RoleID, &clanviewer.m_Members[roleIdx].RoleMember.m_RoleLongId, sizeof(tagLong_ToLua) );
	msgbody.ClanClt.ClanCltData.ClanSetGroup.Set = set;
	SendGameServerMsg( CLAN_CLT, msgbody );
}
void GameClanManager::Clan_Opt_SetGroupStrongMap( int groupIdx, int map )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_SET_GROUP_STRONG_MAP;
	msgbody.ClanClt.ClanCltData.ClanSetGroupStrongMap.Group = groupIdx;
	msgbody.ClanClt.ClanCltData.ClanSetGroupStrongMap.Map = map;
	SendGameServerMsg( CLAN_CLT, msgbody );
}
void GameClanManager::Clan_Opt_OpenGroup( int groupIdx )
{
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation = CLAN_OP_OPEN_GROUP;
	msgbody.ClanClt.ClanCltData.ClanOpenGroup.Group = groupIdx;
	SendGameServerMsg( CLAN_CLT, msgbody );
}

const char* GameClanManager::getMemberNameByPos( int pos )
{
	for ( int i = 0; i < m_ClanViewer.m_clanInfo.m_nClanMemNum; ++i )
	{
		if ( m_ClanViewer.m_Members[i].Position == pos )
		{
			return m_ClanViewer.m_Members[i].RoleMember.m_szRoleName;
		}
	}
	return "";
}
// 公会宠物寄养部分 

void GameClanManager::Clan_Pet_JiYang( int nIndex, int nHour, int nHeight, int nLow, int nFlag )
{
	tagCSPkgBody msgbody;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;

	msgbody.ClanPetClt.Cmd = CLAN_PET_CLT_OP_JIYANG;
	msgbody.ClanPetClt.Data.JiYang.Flag	= nFlag;
	msgbody.ClanPetClt.Data.JiYang.Idx	= nIndex;
	msgbody.ClanPetClt.Data.JiYang.Hour = nHour;
	msgbody.ClanPetClt.Data.JiYang.WID	= *((tdr_ulonglong*)&ID);

	SendGameServerMsg( CLAN_PET_CLT, msgbody );
}

void GameClanManager::Clan_Pet_JiYangEnd( int nIndex, int nHeight, int nLow )
{
	tagCSPkgBody msgbody;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;

	msgbody.ClanPetClt.Cmd = CLAN_PET_CLT_OP_JIYANG_END;
	msgbody.ClanPetClt.Data.JiYangEnd.Idx	= nIndex;
	msgbody.ClanPetClt.Data.JiYangEnd.WID		= *((tdr_ulonglong*)&ID);

	SendGameServerMsg( CLAN_PET_CLT, msgbody );
}

void GameClanManager::Clan_Pet_WanShua( int nIndex, int nGrid, int nType, int nHeight, int nLow )
{
	tagCSPkgBody msgbody;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;

	msgbody.ClanPetClt.Cmd = CLAN_PET_CLT_OP_WANSHUA;
	msgbody.ClanPetClt.Data.Wanshua.Idx		= nIndex;
	msgbody.ClanPetClt.Data.Wanshua.GridIdx = nGrid;
	msgbody.ClanPetClt.Data.Wanshua.Type	= nType;
	msgbody.ClanPetClt.Data.Wanshua.WID		= *((tdr_ulonglong*)&ID);

	SendGameServerMsg( CLAN_PET_CLT, msgbody );
}

void GameClanManager::Clan_Pet_Seduce( int nIndex, int nHeight, int nLow )
{
	tagCSPkgBody msgbody;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;

	msgbody.ClanPetClt.Cmd = CLAN_PET_CLT_OP_TOU;
	msgbody.ClanPetClt.Data.Tou.Idx		= nIndex;
	msgbody.ClanPetClt.Data.Tou.WID		= *((tdr_ulonglong*)&ID);

	SendGameServerMsg( CLAN_PET_CLT, msgbody );
}

void GameClanManager::Clan_Pet_Eat( int nIndex, int nGrid, int nHeight, int nLow )
{
	tagCSPkgBody msgbody;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;

	msgbody.ClanPetClt.Cmd = CLAN_PET_CLT_OP_EAT;
	msgbody.ClanPetClt.Data.Eat.Idx		= nIndex;
	msgbody.ClanPetClt.Data.Eat.GridIdx	= nGrid;
	msgbody.ClanPetClt.Data.Eat.WID		= *((tdr_ulonglong*)&ID);

	SendGameServerMsg( CLAN_PET_CLT, msgbody );
}

void GameClanManager::Clan_Pet_Work( int nIndex, int nHeight, int nLow )
{
	tagCSPkgBody msgbody;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;

	msgbody.ClanPetClt.Cmd = CLAN_PET_CLT_OP_WORK;
	msgbody.ClanPetClt.Data.Work.Idx		= nIndex;
	msgbody.ClanPetClt.Data.Work.WID		= *((tdr_ulonglong*)&ID);

	SendGameServerMsg( CLAN_PET_CLT, msgbody );
}

void GameClanManager::Clan_Pet_WorkAward( int nIndex, int nHeight, int nLow )
{
	tagCSPkgBody msgbody;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;

	msgbody.ClanPetClt.Cmd = CLAN_PET_CLT_OP_WORK_AWARD;
	msgbody.ClanPetClt.Data.WorkAward.Idx	= nIndex;
	msgbody.ClanPetClt.Data.WorkAward.WID	= *((tdr_ulonglong*)&ID);

	SendGameServerMsg( CLAN_PET_CLT, msgbody );
}

void GameClanManager::Clan_Pet_Clear( int nIndex, int nHeight, int nLow )
{
	tagCSPkgBody msgbody;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;

	msgbody.ClanPetClt.Cmd = CLAN_PET_CLT_OP_CLEAR;
	msgbody.ClanPetClt.Data.Clear.Idx	= nIndex;
	msgbody.ClanPetClt.Data.Clear.WID	= *((tdr_ulonglong*)&ID);

	SendGameServerMsg( CLAN_PET_CLT, msgbody );
}

int GameClanManager::getClanPetNum()
{
	if ( m_ClanPetAll.empty() )
	{
		return 0;
	}
	return m_ClanPetAll.size();
}
int	GameClanManager::getClanPetFreeNum()
{
	return m_ClanPetFree;
}
tagClanPetOne_ToLua* GameClanManager::getClanPetData( int nIdx )
{
	std::map<int, tagClanPetOne_ToLua>::iterator iter = m_ClanPetAll.find( nIdx );
	if ( iter == m_ClanPetAll.end() )
	{
		return NULL;
	}
	return &iter->second;
}

void GameClanManager::updateClanPetRailing( int nPosX, int nPosZ, int nIdx )
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();	

	if ( m_ClanPetEntities[nIdx] )
	{
		m_ClanPetEntities[nIdx]->detachFromScene();
		OGRE_RELEASE(m_ClanPetEntities[nIdx]);
	}
	if ( nIdx >= m_ClanPetMax )
	{
		return ;
	}

	int y=0;
	pMain->getCurMap()->getTerrainH( nPosX*10,nPosZ*10,y );
	Ogre::WorldPos pos( Ogre::Vector3((float)nPosX*10,(float)y,(float)nPosZ*10) );
	pos = pos/10;

	// 宠物寄养栏位npc模拟
	GameActor *pactor = NULL;
	pactor = ExtraActorMgr::getSingleton().FindActor( nIdx + 1 );
	if ( pactor == NULL )
	{
		pactor = ExtraActorMgr::getSingleton().CreateActor( nIdx + 1, GAT_EXTRA_NPC, 304147, true );

		const tagMonsterDef& monsterDef = DBDataManager::getSingleton().getMonsterDef( pactor->getResId() );
		pactor->setMaxHP( monsterDef.HP );
		pactor->reSetHp( monsterDef.HP );
		pactor->setLv( monsterDef.Level );
		pactor->setName( monsterDef.Name );
		pactor->OnEnterMap( pMain->getCurMap() );
		pactor->SetPosition( pos.x/10, pos.z/10 );
		pactor->SetFaceTo( 90, true );
		RoleText& actorTitleMgr = pactor->getRoleText();
		actorTitleMgr.BuildTitle();
	}

	std::map<int,tagClanPetOne_ToLua>::iterator iter = m_ClanPetAll.find( nIdx );
	if ( iter != m_ClanPetAll.end() )
	{
		const tagPetDef& petDef	= DBDataManager::getSingleton().getPetDef( iter->second.PetID );
		const tagNpcDisplayInfo &petInfo = DBDataManager::getSingleton().GetNpcDisplayInfo( petDef.MouldID );
		m_ClanPetEntities[nIdx] = new Ogre::AvatarEntity;
		m_ClanPetEntities[nIdx]->load( petInfo.model,false );
		m_ClanPetEntities[nIdx]->setPosition( pos );
		m_ClanPetEntities[nIdx]->setScale( Ogre::Vector3((float)petInfo.scale,(float)petInfo.scale,(float)petInfo.scale) );
		m_ClanPetEntities[nIdx]->setRotation(90 - 90.0f, 0, 0);
		m_ClanPetEntities[nIdx]->playAnim( 10100 );
		m_ClanPetEntities[nIdx]->update(60000);	
		m_ClanPetEntities[nIdx]->attachToScene( pMain->getCurMap()->getShowGameMap()->getGameScene() );
	}	
}

void GameClanManager::cleanClanPetInfo()
{
	for ( int i = 0 ; i < CLAN_PET_MAX_JIYANG; i++ )
	{
		if ( m_ClanPetEntities[i] )
		{
			m_ClanPetEntities[i]->detachFromScene();
			OGRE_RELEASE(m_ClanPetEntities[i]);
		}
	}
	ExtraActorMgr::getSingleton().DestroyAllActor();
}
