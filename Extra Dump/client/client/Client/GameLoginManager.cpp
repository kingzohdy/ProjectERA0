#include "GameLoginManager.h"
#include "GameManager.h"
#include "GamePlayManager.h"
#include "ServerTimer.h"
#include "GameActor.h"
#include "ShowActor.h"
#include "OgreLog.h"
#include "cs_net.h"
#include <memory.h>
#include "OgreXMLData.h"
#include "GameSkill_Mgr.h"
#include "GameSkill_Control.h"
#include "GameShortCut.h"
#include "GameMap.h"
#include "ShowGameMap.h"
#include "GameMaxMap.h"
#include "tinyxml.h"
#include "GameStateManager.h"
#include "Equip_Common.h"
#include "GameQuest.h"
#include "GameClientOpt.h"
#include "GameWizard.h"
#include "GameActor_Rider.h"
#include "GameMainPlayer.h"
#include "GameClanManager.h"
#include "GameTrust.h"
#include "GameStallManager.h"
#include "GameBackgroundScene.h"
#include "GameCamera.h"
#include "GameSkill_DefToLua.h"
#include "GameDiathesis.h"
#include "GameFriendManager.h"
#include "GameTeamManager.h"
#include "GameMarketManager.h"
#include "GameActor_PVP.h"
#include "GameUserConfig.h"
#include "GameFollowManager.h"
#include "AutoTeam.h"
#include "GameWheelLottery.h"
#include "PworldLimitInfo.h"
#include "MapLineSelectManager.h"
#include "GameBattle.h"
#include "OgreRoot.h"
#include "ogrebloom.h"
#include "GameChatManager.h"
#include "GameDBData.h"
#include "GameKeyMoveOperate.h"
#include "parseXML.h"
#include "OgreResourceManager.h"
#include <WinInet.h>
#include "GameAutoAssist.h"
#include "GamePromotion.h"
#include "Ping.h"
#include "GameGetAccountAllRole.h"
#include "MagicSmelter.h"
#include "GameVipManager.h"
#include "GameSchool.h"
#include "GameUIMsg.h"
#include "GameTopCareer.h"
#include "GamePoly.h"
#include "DreamLand.h"

#pragma comment( lib, "Wininet.lib" )

const string CLIENT_CFG_PATH = "client.cfg";
extern "C"
{
	extern int gettimeofday(struct timeval *a_pstTv, struct timezone *a_pstTz);
}

#define DIRCONFIG_PATH		"DirSvr.xml"
#define CONNECT_SERTIME		500

const int GAME_DISCONNECT_FLAG	= -1;
using namespace Ogre;

static void ResourceManagerKeepAlive(void *userdata)
{
	((LoginManager *)userdata)->keepAlive();
}

LoginManager *LoginManager::Create()
{
	LoginManager *p = new LoginManager;
	INetworkManager::getSingleton().RegisterMsgHandler(ACC_LOGIN_RES, p);
	INetworkManager::getSingleton().RegisterMsgHandler(ROLE_LIST, p);
	INetworkManager::getSingleton().RegisterMsgHandler(ROLE_LOGIN_RES, p);
	INetworkManager::getSingleton().RegisterMsgHandler(ROLE_NEW_RES, p);
	INetworkManager::getSingleton().RegisterMsgHandler(ROLE_DEL_RES, p);
	INetworkManager::getSingleton().RegisterMsgHandler(ACC_LOGOUT_RES, p);
	INetworkManager::getSingleton().RegisterMsgHandler(SELECT_BIRTH_LOCATION_SVR, p);
	INetworkManager::getSingleton().RegisterMsgHandler(SYS_MSG, p);
	INetworkManager::getSingleton().RegisterMsgHandler(ADJUST_TIME, p);
	INetworkManager::getSingleton().RegisterMsgHandler(ENTER_MAP, p);
	INetworkManager::getSingleton().RegisterMsgHandler(KICKOFF_INFO, p);
	INetworkManager::getSingleton().RegisterMsgHandler(QUE_INFO, p);
	INetworkManager::getSingleton().RegisterMsgHandler(HEARTBEAT_RES, p);
	INetworkManager::getSingleton().RegisterMsgHandler(PLAYER_MISC_SVR,p);
	INetworkManager::getSingleton().RegisterMsgHandler(AAS_SVR,p);
	return p;
}

LoginManager::LoginManager() : 
	m_GameNetCL( GAME_DISCONNECT_FLAG ), 
	m_HeartBeatTick( 0 ), 
	m_SelectRoleIndex( -1 ),
	m_nLoginResult( LOGIN_SUCC ), 
	m_nRoleNewResult( NEW_SUCC ),
	m_nDelResult( DEL_SUCC ),
	m_isFirstLoad(true),
	m_bStartfmud(false),
	m_bCloseLine(false),
	m_nLoginTime(0),
	m_bIsAlive(false),
	m_LastThread(0),
	m_nPing(0),
	m_bWaitPassCard(false),
	m_bPingTest(false),
	m_nPingCheck( 1000 ),
	m_nIpMode(0)
{
	m_dwMainThreadId = GetCurrentThreadId();
	m_CurTick = timeGetTime();
	m_bOffLine = false;
	m_uLastTime = 0;
	m_UserName[0] = '\0';
	for( int i = 0 ; i < MAX_SAVE_NAME_NUM ; i ++ )
	{
		m_SaveUserName[i][0] = '\0';
	}
	m_nLastSelectWorldId = -1;
	m_nLastSelectRegionId = -1;
	m_nSaveUserName = -1;
	m_ServerName[0] = '\0';
	m_SelfIp[0]		= '\0';
	m_DesIp[0]		= '\0';
	m_BossInfo.Num = 0;
	m_strLastWorldName = "";

	DBDataManager::getSingleton().loadBirthItemDef( m_vecBirthItemDef );
	Ogre::ResourceManager::getSingleton().setKeepAliveHandle(ResourceManagerKeepAlive, this);
	memset(&m_NewPeopleInfo, 0, sizeof(tagCSNewPeopleInfo));
	createPingThread();
}

LoginManager::~LoginManager()
{
	//结束进程
	for( size_t i = 0 ; i < m_vecDir.size() ; i++ )
	{
		m_vecDir[i].Clear();
	}
	Ogre::ResourceManager::getSingleton().setKeepAliveHandle(NULL, NULL);
}

void LoginManager::update(unsigned int dtick)
{
	keepAlive();

	for( int i = 0 ; i < (int)m_vecDir.size() ; i++ )
	{	
		if( m_vecDir[i].m_DirServerNet != NULL )
		{
			dispatchDirNetCL( m_vecDir[i].m_DirServerNet, i );
		}		
	}
}

static void ConvertRoleInfo(LoginRole &dest, const ROLEINFO &src)
{
	static_assert( sizeof(tagRoleAvatarProf_ToLua) == sizeof(ROLEAVATARPROF) );
	strncpy(dest.name, src.RoleName,32);
	dest.gender	= src.Gender;
	dest.roleid	= src.RoleID;
	dest.race	= src.Career;
	dest.level	= src.Level;
	dest.face	= src.Face;
	dest.hair	= src.Hair;
	dest.map	= src.Map;
	dest.head	= src.Head;
    dest.lastlogin = src.LastLogin;
	dest.delTime = src.DelTime;
	memcpy( &dest.armInfo, &src.Avatar, sizeof(ROLEAVATARPROF) );
}

int LoginManager::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	// 账号请求登入返回信息
	if( ACC_LOGIN_RES == msgid )
	{
		m_bWaitPassCard = false;
		if( msgbody.AccLoginRes.Result != 0 )
		{
			m_nLoginResult  = msgbody.AccLoginRes.Result;
			m_GameNetCL		= GAME_DISCONNECT_FLAG;
            GameManager::getSingleton().SendUIEvent("GE_UPDATE_LOGINRESULT");
			GameStateManager::changeGameState( GameStateManager::STATE_DIRLIST );
			return -1;
		}
	}
	else if ( QUE_INFO == msgid )
	{
		//
		const CSQUEINFO& queueInfo = msgbody.QueInfo;
		char buf[50] = { '\0' };
		sprintf( buf, "UpdateQueueInfo(%d,%d)", queueInfo.Max, queueInfo.Pos );
		GameManager::getSingleton().getScriptVM()->callString( buf );
	}
	else if( ROLE_LIST == msgid )
	{
		TryGetOtherWorldData();
		m_Roles.resize(msgbody.RoleList.RoleNum);
		for(unsigned int i=0; i<msgbody.RoleList.RoleNum; i++)
		{
			const ROLEINFO &src = msgbody.RoleList.RoleList[i];
			ConvertRoleInfo(m_Roles[i], src);
		}
		if( m_bOffLine )
		{
			m_bOffLine = false;
		}
        selectLastLoginRole();
		ServerTimer::getSingleton().updateServerBenchTime( msgbody.RoleList.SvrTime.BenchTimeSec,msgbody.RoleList.SvrTime.BenchTimeUSec );

		GameManager::getSingleton().SendUIEvent("GE_UPDATE_ROLELIST");
		GameManager::getSingleton().SendUIEvent("GE_UPDATE_GETROLELIST");
		GameStateManager::changeGameState( GameStateManager::STATE_ROLELIST );
		if ( m_bStartfmud )
		{
			EnterWorldDirect();
			m_bStartfmud = false;
		}
	}
	// 创建角色返回信息
	else if(ROLE_NEW_RES == msgid)
	{
		if(msgbody.RoleNewRes.Result != 0)
		{
			m_nRoleNewResult = msgbody.RoleNewRes.Result;		
		    GameManager::getSingleton().SendUIEvent("GE_UPDATE_ROLENEW");
			LOG_WARNING("new role failed");
		}
		else
		{
			LoginRole role;
			ConvertRoleInfo(role, msgbody.RoleNewRes.RoleInfo);
			m_Roles.push_back(role);

            m_SelectRoleIndex = ( (int)m_Roles.size() - 1 );

			GameManager::getSingleton().SendUIEvent("GE_UPDATE_ROLELIST");
            m_nRoleNewResult = NEW_SUCC;
			GameManager::getSingleton().SendUIEvent("GE_UPDATE_ROLENEW");
		}
	}
	// 删除角色返回信息
	else if(ROLE_DEL_RES == msgid)
	{
		if(msgbody.RoleDelRes.Result != 0)
		{
			m_nDelResult = msgbody.RoleDelRes.Result; 
			GameManager::getSingleton().SendUIEvent("GE_ROLE_DELFAILRESULT");
			LOG_WARNING("del role failed");
		}
		else
		{
			std::vector<LoginRole>::iterator iter = m_Roles.begin();
			for(; iter!=m_Roles.end(); iter++)
			{
				if(iter->roleid == msgbody.RoleDelRes.RoleID)
				{
					m_Roles.erase(iter);
					break;
				}
			}

            selectLastLoginRole();

			GameManager::getSingleton().SendUIEvent("GE_UPDATE_ROLELIST");
			GameManager::getSingleton().SendUIEvent("GE_UPDATE_ROLEDEL");
		}
	}
	else if(ACC_LOGOUT_RES == msgid)
	{
		if( msgbody.AccLogoutRes.Result == LOGOUT_FOR_MUD_SUCC )
		{
			startMud();
		}else{
			GameStateManager::changeGameState( GameStateManager::STATE_DIRLIST );
		}		
	}
	else if(SELECT_BIRTH_LOCATION_SVR == msgid)
	{
		const CSELECTBIRTHLOCS &loc = msgbody.SelectBirthLocS;
		m_SelectBirth.BirthMapNum	= loc.BirthMapNum;
		for( int i = 0; i < m_SelectBirth.BirthMapNum; i++ )
		{
			m_SelectBirth.Busy[i]			= loc.Busy[i];
			m_SelectBirth.MapIDList[i]		= loc.MapIDList[i];
		} 
		GameManager::getSingleton().SendUIEvent("GE_SELECTBIRTH_SHOW");
	}
	// 角色请求登入返回信息
	else if(ROLE_LOGIN_RES == msgid)
	{
		if( msgbody.RoleLoginRes.Result != LOGIN_SUCC )
		{
			m_nLoginResult  = msgbody.AccLoginRes.Result;
			GameManager::getSingleton().SendUIEvent("GE_UPDATE_LOGINRESULT");
			GameStateManager::changeGameState( GameStateManager::STATE_DIRLIST );
			return 0;
		}
		m_nLoginTime = getServerTime().sec;
		if( msgbody.RoleLoginRes.LoginFlag == 1 )
		{
			if(msgbody.RoleLoginRes.EnterFlag == 0)
			{
				GameManager::getSingleton().SendUIEvent("GE_ROLE_LOGIN");	
				LOG_WARNING("role login failed");
			}
			else
			{
				GameBackground::getSingleton().hide();
				GameWheelLottery::getSingleton().reset();

				const CSENTERWORLD &enterworld = msgbody.RoleLoginRes.Enter;
				const CSENTERMAP &entermap = enterworld.EnterMap;
				if( enterworld.SvrConfig & SVR_CONFIG_CLOSE_LINE )
				{
					m_bCloseLine = true;
				}else 
				{
					m_bCloseLine = false;
				}
				m_MapID = entermap.Map;
				char szBuf[256];
				// shortcut
				if( enterworld.ClintOptLen != 0 )
				{
					GameClientOpt::getSingleton().setOptContent( enterworld.ClintOptContent );
					// 读新手指引进度
					GameClientOpt::getSingleton().loadClientOpt( SAVE_OPT_DIRECT, *(short*)enterworld.ClintOptContent, 
						GameWizard::getSingleton().getSetp() );
					GameWizard::getSingleton().undoStep(GAMEWIZARD_FIRST_LOGINTIMECITY);
				}else{
					GameWizard::getSingleton().zeroSetp();
				}
				sprintf( szBuf,"BeginLoading(%d)",m_MapID );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
				//这里不是服务器当前时间，
// 				ServerTimer::getSingleton().updateServerBenchTime(msgbody.RoleLoginRes.BenchTime.BenchTimeSec, 
// 					msgbody.RoleLoginRes.BenchTime.BenchTimeUSec);

				int nMoodelId = getPlayerDisplayId( msgbody.RoleLoginRes.Enter.RoleDetail.Gender, msgbody.RoleLoginRes.Enter.RoleDetail.Career );

				MainGamePlayer *player = GamePlayManager::getSingleton().enterWorld(entermap.Map, entermap.x, entermap.y, msgbody.RoleLoginRes.RoleMemID, nMoodelId );
				player->m_Gender	= msgbody.RoleLoginRes.Enter.RoleDetail.Gender;
				player->m_Race		= msgbody.RoleLoginRes.Enter.RoleDetail.Career;
				player->m_Face		= msgbody.RoleLoginRes.Enter.RoleDetail.Face;
				player->m_Head		= msgbody.RoleLoginRes.Enter.RoleDetail.Head;
				player->m_Hair		= msgbody.RoleLoginRes.Enter.RoleDetail.Hair;
				player->SetWorldID( msgbody.RoleLoginRes.WorldID );

				if (player->getAvatar())
				{
					player->getAvatar()->equipItem( POS_ID_HAIR, msgbody.RoleLoginRes.Enter.RoleDetail.Hair + 1, 0 );	
					player->getAvatar()->equipItem( POS_ID_FACE, msgbody.RoleLoginRes.Enter.RoleDetail.Face, 0 );	
					player->getAvatar()->equipItem( POS_ID_HEAD, msgbody.RoleLoginRes.Enter.RoleDetail.Head, 0 );
				}

				for( size_t i = 0 ; i < m_Roles.size() ; i++ )
				{
					if( m_Roles[i].roleid == msgbody.RoleLoginRes.Enter.RoleDetail.RoleID )
					{
						player->setName( m_Roles[i].name );
					}
				}

				player->setSpeed(enterworld.MoveSpeed);
				/*RoleDetail*/
				const CSROLEDETAIL& roleattr = enterworld.RoleDetail;
				player->setLv( roleattr.Level );

				const CSSKILLLIST& skilllist = roleattr.SkillList;

				for( int i = 0; i < enterworld.RoleDetail.PlayerAttrVal.AttrNum; i++ )
				{
					player->setAttriValue(enterworld.RoleDetail.PlayerAttrVal.AttrList[i].AttrID, 
						enterworld.RoleDetail.PlayerAttrVal.AttrList[i].Value );
				}

				player->setType(GAT_MAINPLAYER);
				player->SetFaceTo( entermap.Dir*2 + 90 );

				// Wear
				const ROLEWEAR& Role_Wear	= enterworld.RoleDetail.Wear;
				player->m_nShowFlag			= enterworld.RoleDetail.ShowFlag;
				if( enterworld.RoleDetail.ShowFlag & 4 )
				{
					player->m_nShowFlag |= SHOW_BACK_FLAG;
				}
				if( enterworld.RoleDetail.ShowFlag & 8 )
				{
					player->m_nShowFlag |= SHOW_FACE_FLAG;
				}
				if( enterworld.RoleDetail.ShowFlag & 16 )
				{
					player->m_nShowFlag |= SHOW_WEDDING_DSN_FLAG;
				}
				player->m_roleWID = enterworld.RoleDetail.RoleID;				

				for( int i = 0; i < Role_Wear.ArmNum; i++ )
				{
					player->m_Equip.changEquip( Role_Wear.RoleArms[i].GridIdx, Role_Wear.RoleArms[i] );
					//
					if( !IsFashionArm(Role_Wear.RoleArms[i]) || ( player->isShowFashion() ) )
					{
						player->m_Equip.changEquip(Role_Wear.RoleArms[i].GridIdx,Role_Wear.RoleArms[i].DefID,Role_Wear.RoleArms[i].StarLv);
					}
				}
				if (player->getAvatar())
				{
					player->getAvatar()->updateAttrib();
				}
				player->m_Equip.changAmm( AMM_POS_LEFT, enterworld.RoleDetail.Wear.AmmMap.AmmLeft );
				player->m_Equip.changAmm( AMM_POS_RIGHT, enterworld.RoleDetail.Wear.AmmMap.AmmRight );
				player->m_Equip.changAmm( AMM_POS_SHOULDER, enterworld.RoleDetail.Wear.AmmMap.AmmShoulder );

				// status list
				player->m_BufStatus.setBufStatus( enterworld.RoleDetail.StatusList );
				player->getDiathesisManager().setDiathesisInfo( enterworld.RoleDetail.DiathesisList );
				player->getShowActor()->getCurEntity()->setPostSceneRenderer( GameManager::getSingleton().GetMainSceneRenderer() );
				player->resetLoveInfo();
				ActorManager::getSingleton().setPostSceneRenderer( GameManager::getSingleton().GetMainSceneRenderer() );
				ActorManager::getSingleton().resetShowModelAndTitleOption();

				SkillManager::getSingleton().reSetSkilList();
				// skill list
				for( int i = 0; i < skilllist.SkillNum; i++ )
				{
					tagSkillDyAttrib* pAttrib = new tagSkillDyAttrib;
					pAttrib->SkillLevel			= skilllist.Skills[i].SkillLevel;
					pAttrib->CollectNum			= skilllist.Skills[i].CollectNum;		
					pAttrib->CollectNumMax		= skilllist.Skills[i].CollectNumMax;
					pAttrib->RuneSlotEnabled	= skilllist.Skills[i].RuneSlotEnabled;
					pAttrib->RuneSlot			= skilllist.Skills[i].RuneSlot;
					pAttrib->RuneItemID			= skilllist.Skills[i].RuneItemID;
					pAttrib->RuneItemIDs.resize(skilllist.Skills[i].RuneNum);
					for (int n = 0; n < skilllist.Skills[i].RuneNum; ++n)
					{
						pAttrib->RuneItemIDs[n] = skilllist.Skills[i].RuneInfo[n].RuneItemID;
					}
					SkillManager::getSingleton().setSkillDyAttrib( skilllist.Skills[i].SkillID, pAttrib );
				}

				// 重置任务信息
				GameQuest::getSingleton().clearQuest();

				if( enterworld.ClintOptLen != 0 )
				{
					// 读快捷键
					tagShortCutSave SaveShorCut[MAX_SHORCUT];
					if( GameClientOpt::getSingleton().loadClientOpt( SAVE_OPT_SHORCUT, 
						*(short*)enterworld.ClintOptContent, (char*)&SaveShorCut ) )
					{
						for( int i = 0; i < MAX_SHORCUT; i++ )
						{
							//如果有转职后清空技能标记，且是技能，则清空此快捷栏
							if((enterworld.RoleDetail.ShowFlag & 32) && ( SaveShorCut[i].nType == SCT_SKILL || SaveShorCut[i].nType == SCT_FAIRYSKILL ) )
							{
								GameShortCut::getSingleton().delShortCut( i );
							}
							else
							{
								GameShortCut::getSingleton().setShortCut2UI( SaveShorCut[i].nType, SaveShorCut[i].Id, i, 
									SaveShorCut[i].Fitting, SaveShorCut[i].nLv );
							}
						}
					}
					//发消息给服务器，通知已清楚技能栏
					if(enterworld.RoleDetail.ShowFlag & 32)
					{
						ActorManager::getSingleton().getScriptVM()->callString("CareerChangeClearSkill()");
						requestShortCutSkillClear();
					}
					// 读组合键
					GameClientOpt::getSingleton().loadClientOpt( SAVE_OPT_SPSHORCT, *(short*)enterworld.ClintOptContent, 
						(char*)GameShortCut::getSingleton().m_SPShortCutSkill );
					//读取获得技能后快捷键索引
					GameClientOpt::getSingleton().loadClientOpt( SAVE_OPT_SHORTCUT_FULL_SKILL_INDEX, *(short*)enterworld.ClintOptContent, 
						(char*)&GameShortCut::getSingleton().m_cShortcutIndex );
					// 读游戏设置选项信息
					GameClientOpt::getSingleton().loadClientOpt( SAVE_OPT_GAMESETTING, *(short*)enterworld.ClintOptContent, 
						GameManager::getSingleton().getUserSystemConfig().getGameOptBuffer() );

					// 读取主角朝向
					GameClientOpt::getSingleton().loadClientOpt( SAVE_OPT_CAMERADIR, *(short*)enterworld.ClintOptContent,
						(char*)GamePlayManager::getSingleton().getPlayerCameraDir() );

					// 读取任务追踪数据
					GameClientOpt::getSingleton().loadClientOpt( SAVE_OPT_TASKTRACE, *(short*)enterworld.ClintOptContent,
						(char*)GameQuest::getSingleton().getTaskTraceData() );

					// 读取宠物自动使用药品设置数据
					GameClientOpt::getSingleton().loadClientOpt( SAVE_OPT_PET_AUTO_USE_ITEM, *(short*)enterworld.ClintOptContent,
						(char*)GameUserConfig::getSingleton().getPetUseItemConfig() );

					GameClientOpt::getSingleton().loadClientOpt( SAVE_OPT_LAST_COMMIT_MAIL_TIME, *(short*)enterworld.ClintOptContent,
						(char*)GameUserConfig::getSingleton().getLastCommitMailTime() );

					GameClientOpt::getSingleton().loadClientOpt( SAVE_OPT_LAST_PING_BI_WORLD_PIN_DAO, *(short*)enterworld.ClintOptContent,
							ChatManager::getSingleton().getLastPingBiWorldChannelsOp() );
					GameClientOpt::getSingleton().loadClientOpt( SAVE_OPT_LAST_PING_BI_CLAN_PIN_DAO, *(short*)enterworld.ClintOptContent,
						ChatManager::getSingleton().getLastPingBiClanChannelsOp() );
					GameClientOpt::getSingleton().loadClientOpt( SAVE_OPT_LAST_PING_BI_ZONG_HE_PIN_DAO, *(short*)enterworld.ClintOptContent,
						ChatManager::getSingleton().getLastPingBiZongHeChannelsOp() );
					GameClientOpt::getSingleton().loadClientOpt( SAVE_OPT_LAST_PING_BI_TEAM_PIN_DAO, *(short*)enterworld.ClintOptContent,
						ChatManager::getSingleton().getLastPingBiTeamChannelsOp() );
					GameClientOpt::getSingleton().loadClientOpt( SAVE_OPT_LAST_PING_BI_NEAR_PIN_DAO, *(short*)enterworld.ClintOptContent,
						ChatManager::getSingleton().getLastPingBiNearChannelsOp() );
					GameClientOpt::getSingleton().loadClientOpt( SAVE_OPT_LAST_PING_BI_PRIVATE_PIN_DAO, *(short*)enterworld.ClintOptContent,
						ChatManager::getSingleton().getLastPingBiPrivateChannelsOp() );
				}

				//读取用户的配置文件
				GameUserConfig::getSingleton().loadData();

				for( int i = 0; i < skilllist.SkillNum; i++ )
				{
					const tagSkillDef& SkillDef	= DBDataManager::getSingleton().GetSkillDef( skilllist.Skills[i].SkillID, 
						skilllist.Skills[i].SkillLevel );
					SkillManager::getSingleton().setSkillCDTime(skilllist.Skills[i].SkillID, skilllist.Skills[i].CoolLeft*100);
				}

				if( player->getHP() == 0 )
				{
					player->ChangeActState( GSACT_DEAD );
					//GameMsgPrint::getSingleton().SendUIEvent( "GE_MESSAGE_DEAD" );
				}else{
					if( msgbody.RoleLoginRes.Enter.RoleDetail.State & CS_STAT_FLY )
					{
						player->getActState().SetState( GSACT_STAND );
					}else{
						player->ChangeActState( GSACT_STAND );
					}
				}

				player->m_Container.send_GetInfo();

				GameStateManager::changeGameState( GameStateManager::STATE_INWORLD );
				player->m_pCamera = GamePlayManager::getSingleton().getMainCamera();

				player->m_bPworldInfoFlag = false;
				if ( entermap.PworldInfoFlag )
				{
					MainGamePlayer *player = ActorManager::getSingleton().getMainPlayer();
					player->m_bPworldInfoFlag = true;
					player->m_nPworldLeaveTime = entermap.PworldInfo.LeftTime;
					player->m_nPworldStartTime = entermap.PworldInfo.StartTime;
					GameManager::getSingleton().SendUIEvent("GE_ENTER_PWORLD");
				}

				// 不用设置登陆时的临时机械
				player->getEquip().setTmpArm( 0 );
				player->setEnerge( msgbody.RoleLoginRes.Enter.RoleDetail.TmpSys.TmpEnerge );

				tagRideInfo RideGrids[MAX_RIDE_GRID]; 
				for( int i = 0; i < enterworld.RoleDetail.RideList.Num; i++ )
				{
					RideGrids[i].RideID		= enterworld.RoleDetail.RideList.RideGrids[i].ItemID;
					RideGrids[i].Endtime	= enterworld.RoleDetail.RideList.RideGrids[i].Endtime;
					RideGrids[i].nLevel		= enterworld.RoleDetail.RideList.RideGrids[i].Level;
				}
				player->m_Rider.setRideInfo( enterworld.RoleDetail.RideList.Num, enterworld.RoleDetail.RideList.MaxNum, 
					RideGrids, true );
				player->m_pAutoLine = GameManager::getSingleton().getGameMapShower()->getAutoLine();
				// 请求任务信息
				GameQuest::getSingleton().getQuest();
				// 重置好友信息
				GameFriendManager::getSingleton().reSetFriendList( "ROLE_LOGIN_RES--466" );

				// 重置公会信息
				GameClanManager::getSingleton().resetClanInfo();
				// 重置地图信息
				GameManager::getSingleton().getGameMapShower()->GetMaxMapContal()->resetMapControl();
				GameManager::getSingleton().getGameMapShower()->GetMinMapContal()->resetMapControl();
				GameMapShower* pMapShower = GameManager::getSingleton().getGameMapShower();
				pMapShower->getMultiMapFindControl().reset();
				// 重置摆摊信息
				GameStallManager::getSingleton().resetAllStallInfo();
				// 重置好友列表中要情况的列表
				GameFriendManager::getSingleton().resetFriendList();
				// 重置队伍信息
				GameTeamManager::getSingleton().releaseTeamInfo();
				// 重置PVP公会敌对信息
				GameActor_PVP::getSingleton().resetEnemyClan();
				// 重置自动组队信息
				AutoTeamManager::getSingleton().releaseAutoTeamInfo();
				// 重置组队跟随信息
				GameFollowManager::getSingleton().resetFollowInfo();
				// 重置战场信息
				GameBattle::getSingleton().reset();
				GameBattle::getSingleton().clearWaitings();
				//重置活动引导信息
				GamePromotion::getSingleton().resetActivityGuidInfo();
				//初始化商城相关数据
				CMarketManager::getSingleton().resetMarketInfo();
				//初始化熔炉信息
				MagicSmelterManager::getSingleton().initRongluInfo();
				AutoAssist::getSingleton().resetAssistInfo();
				if( enterworld.ClintOptLen > 0 )
				{
					GameClientOpt::getSingleton().loadClientOpt( SAVE_OPT_MARKET_LATEST_GOLD_COIN_BUY, *(short*)enterworld.ClintOptContent,
						CMarketManager::getSingleton().getLatestGoldCoinBuy() );
					GameClientOpt::getSingleton().loadClientOpt( SAVE_OPT_MARKET_LATEST_TICK_BUY, *(short*)enterworld.ClintOptContent,
						CMarketManager::getSingleton().getLatestTicketBuy() );
					//自动辅助选项内容
					GameClientOpt::getSingleton().loadClientOpt( SAVE_OPT_AUTO_ASSIST, *(short*)enterworld.ClintOptContent,
						(char*)AutoAssist::getSingleton().getAutoAssistOpt() );
					//VIP高级辅助选项
					GameClientOpt::getSingleton().loadClientOpt( SAVE_OPT_VIP_AUTO_ASSIST, *(short*)enterworld.ClintOptContent,
						(char*)AutoAssist::getSingleton().getVIPAutoAssistOpt() );
					GameClientOpt::getSingleton().loadClientOpt( SAVE_OPT_AUTO_QUEST, *(short*)enterworld.ClintOptContent,
						(char*)GameVipManager::getSingleton().getAutoQuestOpt() );
					GameClientOpt::getSingleton().loadClientOpt( SAVE_OPT_AUTO_RECOVERY, *(short*)enterworld.ClintOptContent,
						(char*)GameVipManager::getSingleton().getAutoRecoveryOpt() );
				}				
				
				char buff[256];
				sprintf( buff,"AutoAssistLoadSevData()");
				ActorManager::getSingleton().getScriptVM()->callString(buff);
				sprintf( buff,"AutoRecoveryLoadSevData()");
				ActorManager::getSingleton().getScriptVM()->callString(buff);

				//读自己PK模式
				player->m_nPkMode = msgbody.RoleLoginRes.Enter.RoleDetail.PkInfo.PkMode;
				player->m_nPkValue = msgbody.RoleLoginRes.Enter.RoleDetail.PkInfo.PkVal;
				GameActor_PVP::getSingleton().onPkModeChanged(player->m_nPkMode);
				GameActor_PVP::getSingleton().onPkValueChanged(player->m_nPkValue);
				// 记录玩家登陆时间
				int sec, usec;
				ServerTimer::getSingleton().getServerTime( sec, usec ); 
				GameWizard::getSingleton().setRoleLoginTm( sec );
				//reqireHeartBeat();

				LoginManager::getSingleton().setFirstLoad(entermap.Map);
				GamePlayManager::getSingleton().preLoadMap(entermap);

				// 初始化键盘
				GameManager::getSingleton().getScriptVM()->callString("LoadKeyMap()");
				GameKeyMoveOperate::getSingleton().reset();

				{
					static const char *race[] = {
						"",
						"战士",
						"法师",
						"枪手",
						"祭祀",
					};
					LOG_INFO("角色【%s】进阶%d %d级", race[msgbody.RoleLoginRes.Enter.RoleDetail.Career & 0x0f],
						msgbody.RoleLoginRes.Enter.RoleDetail.Career / 0x10, msgbody.RoleLoginRes.Enter.RoleDetail.Level);
				}

				if( enterworld.SvrConfig & SVR_CONFIG_UNFY_USE_JINQUAN )
				{
					GameManager::getSingleton().getScriptVM()->callString( "ChangeCostMoneyType(1)" );
				}else{
					GameManager::getSingleton().getScriptVM()->callString( "ChangeCostMoneyType(0)" );
				}
			}
		}
		else if( msgbody.RoleLoginRes.LoginFlag == 2 )
		{
			MainGamePlayer *player = GamePlayManager::getSingleton().getMainPlayer();
			if (player)
			{
				ActorManager::getSingleton().changeMainPlayerID(msgbody.RoleLoginRes.RoleMemID);
			}
			else
			{
				int nMoodelId = getPlayerDisplayId(msgbody.RoleLoginRes.Enter.RoleDetail.Gender, msgbody.RoleLoginRes.Enter.RoleDetail.Career);
				ActorManager::getSingleton().CreateActor(msgbody.RoleLoginRes.RoleMemID, GAT_MAINPLAYER, nMoodelId);
			}
			//GamePlayManager::getSingleton().preLoadMap(msgbody.RoleLoginRes.Enter.EnterMap);
			player->setTeamName( "" );
			//player->setTeamWID( 0 );
			GameTeamManager::getSingleton().releaseTeamInfo();
			GameManager::getSingleton().SendUIEvent( "GE_BATTLE_TO_WORLD" );
		}
		else
		{
			MainGamePlayer *player = GamePlayManager::getSingleton().getMainPlayer();
			if (player)
			{
				ActorManager::getSingleton().changeMainPlayerID(msgbody.RoleLoginRes.RoleMemID);
				player->setTeamName( "" );
				player->setTeamWID( 0 );
			}
			else
			{
				int nMoodelId = getPlayerDisplayId(msgbody.RoleLoginRes.Enter.RoleDetail.Gender, msgbody.RoleLoginRes.Enter.RoleDetail.Career);
				ActorManager::getSingleton().CreateActor(msgbody.RoleLoginRes.RoleMemID, GAT_MAINPLAYER, nMoodelId);
			}
			
			GamePlayManager::getSingleton().preLoadMap(msgbody.RoleLoginRes.Enter.EnterMap);
			if ( msgbody.RoleLoginRes.Enter.EnterMap.RealMap == 1000 )
			{
				char szBuf[256];
				sprintf( szBuf,"BeginEnterMap(%d)", msgbody.RoleLoginRes.Enter.EnterMap.RealMap );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			else
			{
				GamePlayManager::getSingleton().startFade();
			}
			// 重置队伍信息
			GameTeamManager::getSingleton().releaseTeamInfo();
			GameManager::getSingleton().SendUIEvent( "GE_BATTLE_TO_WORLD" );
		}
		MainGamePlayer *pMainPlayer = GamePlayManager::getSingleton().getMainPlayer();
		if ( pMainPlayer != NULL )
		{
			RoleText& actorTitleMgr = pMainPlayer->getRoleText();
			actorTitleMgr.BuildTitle();
			pMainPlayer->setGMPermFlag( msgbody.RoleLoginRes.Perm );
			pMainPlayer->m_bIsOutSide = ( msgbody.RoleLoginRes.LoginFlag == 2 );
		}
		GameManager::getSingleton().SetMaxLevel( msgbody.RoleLoginRes.MaxLevel );
		GameManager::getSingleton().SetLevelFeng( msgbody.RoleLoginRes.LevelFeng.LevelFeng );
		char szBuf[128];
		sprintf( szBuf, "OnGetLevelFeng( %d,%d,%d )",msgbody.RoleLoginRes.LevelFeng.LevelFeng, msgbody.RoleLoginRes.LevelFeng.LevelFengEnd,msgbody.RoleLoginRes.LevelFengChg );
		GameManager::getSingleton().getScriptVM()->callString( szBuf );
		//pingCheck测试
		if ( msgbody.RoleLoginRes.PingCheck > 0 )
		{
			m_bPingTest		= true;
			m_nPingCheck	= msgbody.RoleLoginRes.PingCheck;
			strncpy( m_SelfIp,inet_ntoa(*(struct in_addr *)&msgbody.RoleLoginRes.IP ),48);
		}
		else
		{
			m_bPingTest		= false;
		}
		
	}
	else if(ADJUST_TIME == msgid)
	{
		ServerTimer::getSingleton().updateServerBenchTime(msgbody.AdjustTime.BenchTimeMs);
	}
	else if(SYS_MSG == msgid)
	{
		LOG_INFO(msgbody.SysMsg.Msg);
	}
	else if( ENTER_MAP == msgid )
	{
		GameMap *pCurMap = GamePlayManager::getSingleton().getCurMap();
		MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
		if ( pMainplayer != NULL )
		{
			pMainplayer->breakBTypePre();
		}

		if (/*true || */pCurMap == NULL || msgbody.EnterMap.RealMap != pCurMap->getRealId())
		{
			GamePlayManager::getSingleton().preLoadMap(msgbody.EnterMap);
			if (pCurMap == NULL)
			{
				GameManager::getSingleton().mapLoadFaded();
			}
			else
			{
				if ( msgbody.EnterMap.RealMap == 1000 )
				{
					char szBuf[256];
					sprintf( szBuf,"BeginEnterMap(%d)", msgbody.EnterMap.RealMap );
					GameManager::getSingleton().getScriptVM()->callString( szBuf );
				}
				else
				{
					GamePlayManager::getSingleton().startFade();
				}
			}
		}
		else
		{
			LoginManager::getSingleton().onMapPreLoaded(pCurMap, msgbody.EnterMap);
		}
		if ( msgbody.EnterMap.Span == MAP_SPAN_NONE )
		{
			pMainplayer->m_bIsOutSide = false;
		}
		else if ( msgbody.EnterMap.Span == MAP_SPAN_BATTLE )
		{
			pMainplayer->m_bIsOutSide = true;
		}
		DBDataManager& db = DBDataManager::getSingleton();
		tagMapDef const& mapDef = db.getMapDef(msgbody.EnterMap.RealMap);
		if(mapDef.CtrlFlag & MAP_CTRL_PVP_LOAD_MAP)
		{
			GameManager::getSingleton().SendUIEvent("GE_SPAN_MAP_ENTER");
		}
		else
		{
			GameManager::getSingleton().SendUIEvent("GE_SPAN_MAP_LEAVE");
		}
	}
	// 账号已在别处登录，被逼下线
	else if( KICKOFF_INFO == msgid )
	{   
		m_szOffLineText = msgbody.MaxBody;
// 		m_DirNetCL  = DIR_DISCONNECT_FLAG;
		m_GameNetCL = GAME_DISCONNECT_FLAG;
		if( GameManager::getSingleton().isInPlayCamerAnim() )
		{
			GameManager::getSingleton().stopPlayCameraAnim();
		}
		SkillManager::getSingleton().onStop();
		GameShortCut::getSingleton().clearSpringSpell();

		GamePlayManager::getSingleton().leaveMap();
		ActorManager::getSingleton().DestroyAllActor( true );		
		
		INetworkManager::getSingleton().Release();
		// 
		GameStateManager::changeGameState( GameStateManager::STATE_INIT );
		GameStateManager::changeGameState( GameStateManager::STATE_DIRLIST );
		GameManager::getSingleton().SendUIEvent("GE_KICKOFF_INFO");
		m_ServerName[0] = '\0';
		return -1;
	}
	else if(HEARTBEAT_RES == msgid)
	{
		//刷新 活动助手
		GameManager::getSingleton().SendUIEvent("GE_PROMOTION_UPDATE");
		//ServerTimer::getSingleton().updateServerBenchTime(heartBeatRes.ConnectTime.BenchTimeSec, heartBeatRes.ConnectTime.BenchTimeUSec);

	}else if( PLAYER_MISC_SVR == msgid )
	{
		switch (msgbody.PlayerMiscSvr.PlayerMiscType)
		{
		case PLAYER_CHOOSE_SVR:
			{
				char szBuf[256];
				sprintf( szBuf,"ShowServerMessageBoxForGame(%d,%d,\"%s\",%d)",msgbody.PlayerMiscSvr.PlayerMiscSvrData.ChooseS.BindType,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.ChooseS.MemID,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.ChooseS.Tips,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.ChooseS.Timer);
				GameManager::getSingleton().getScriptVM()->callString(szBuf);
			}
			break;
		case PLAYER_MOVIE_START_SVR:
			{
				GameManager::getSingleton().playVideo(msgbody.PlayerMiscSvr.PlayerMiscSvrData.MovieStartS.MovieFilePath);
			}
			break;
		case PLAYER_OPEN_ITEM_MAP:
			{
				char szBuf[256];
				sprintf( szBuf,"OpenItemMap( %d,%d,%d,%d )",msgbody.PlayerMiscSvr.PlayerMiscSvrData.OpenItemMap.ItemID,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.OpenItemMap.MapID,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.OpenItemMap.Pos.X,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.OpenItemMap.Pos.Y );
				GameManager::getSingleton().getScriptVM()->callString(szBuf);
			}
			break;
		case PLAYER_EXP_BUFF_INFO:
			{
				char szBuff[200];
				sprintf( szBuff,"OpenOfflineExpBuffFrame( %d, %d )",msgbody.PlayerMiscSvr.PlayerMiscSvrData.ExpBuffInfo.Exp, msgbody.PlayerMiscSvr.PlayerMiscSvrData.ExpBuffInfo.Day );
				GameManager::getSingleton().getScriptVM()->callString(szBuff);
				MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
				pMainPlayer->setOfflineDay( msgbody.PlayerMiscSvr.PlayerMiscSvrData.ExpBuffInfo.Day );
			}
			break;
		case PLAYER_WORLD_MAX_LEVEL:
			{
				char szBuff[200];
				sprintf( szBuff,"SetTopOneLv( %d )",msgbody.PlayerMiscSvr.PlayerMiscSvrData.MaxLevel.Level );
				GameManager::getSingleton().getScriptVM()->callString(szBuff);
			}
			break;
		case PLAYER_BAOXIANG_AWARD_INFO:
			{
				if( msgbody.PlayerMiscSvr.PlayerMiscSvrData.BaoXiangAwardInfo.Succ == 1 )
				{
					static_assert( sizeof(tagCSBaoXiangAwardInfo_ToLua) == sizeof(tagCSBaoXiangAwardInfo) );
					tagUIMsgBody UI_Msg;
					memcpy( &UI_Msg.BaoXiangAwardInfo, &msgbody.PlayerMiscSvr.PlayerMiscSvrData.BaoXiangAwardInfo, sizeof(tagCSBaoXiangAwardInfo) );
					GameUIMsg::getSingleton().setUIMsg( UI_Msg );
					GameManager::getSingleton().SendUIEvent( "GE_BAOXIANG_OPEN" );
				}
			}
			break;
		case PLAYER_BOSS_INFO_RES:
			{
				m_BossInfo.Num = msgbody.PlayerMiscSvr.PlayerMiscSvrData.BossInfoRes.Num;
				for ( int i=0; i < m_BossInfo.Num; i++ )
				{
					m_BossInfo.One[i].MonID		= msgbody.PlayerMiscSvr.PlayerMiscSvrData.BossInfoRes.One[i].MonID;
					m_BossInfo.One[i].Stat		= msgbody.PlayerMiscSvr.PlayerMiscSvrData.BossInfoRes.One[i].Stat;
					m_BossInfo.One[i].FreshTime = msgbody.PlayerMiscSvr.PlayerMiscSvrData.BossInfoRes.One[i].FreshTime;
				}
				GameManager::getSingleton().getScriptVM()->callString( "UpdateBossInfo()" );
			}
			break;
		case WEATHER_CHG_NOTIFY:
			{
				msgbody.PlayerMiscSvr.PlayerMiscSvrData.Weather.Master;
				msgbody.PlayerMiscSvr.PlayerMiscSvrData.Weather.WeatherType;
				char szBuf[128];
				sprintf( szBuf,"OnWeatherChange( %d,%d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.Weather.WeatherType, msgbody.PlayerMiscSvr.PlayerMiscSvrData.Weather.Master );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case PLAYER_SVR_BUY_HETONG:
			{
				if ( msgbody.PlayerMiscSvr.PlayerMiscSvrData.BuyHetong.HetongID > 1 )
				{
					char szBuf[128];
					sprintf( szBuf,"SetMessageBoxForItem( %d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.BuyHetong.HetongID );
					GameManager::getSingleton().getScriptVM()->callString( szBuf );
				}
				else
				{
					GameManager::getSingleton().getScriptVM()->callString( "ShowBuyMoneyFrame()" );
				}
				
			}
			break;
		case PLAYER_CHARM_RES:
			{
				char szBuf[128];
				sprintf( szBuf,"ReceiveFlower( %d,%d,\"%s\",\"%s\",%d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.CharmRes.Flag, msgbody.PlayerMiscSvr.PlayerMiscSvrData.CharmRes.Type,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.CharmRes.Accept, msgbody.PlayerMiscSvr.PlayerMiscSvrData.CharmRes.Send,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.CharmRes.Num );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case PLAYER_REG_RES:
			{					
				char szBuf[128];
				sprintf( szBuf,"StudentRegistMsgResult( %d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.RegRes.Succ );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case PLAYER_SVR_REG_INFO:
			{
				GameSchool::getSingleton().SetRegInfo( msgbody.PlayerMiscSvr.PlayerMiscSvrData.SvrRegInfo );
				char szBuf[128];
				sprintf( szBuf,"OpenRegistStudentsFrame( %d )",msgbody.PlayerMiscSvr.PlayerMiscSvrData.SvrRegInfo.NpcID );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case PLAYER_SVR_CONT_COVER_INFO:
			{
				char szBuf[128];
				sprintf( szBuf,"GiftCenterGuideContinueCheck( %d,%d,%d,%d,%d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.ContCoverGiftInfo.ItemID,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.ContCoverGiftInfo.ItemNum,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.ContCoverGiftInfo.ContCoverGiftInfo.Num,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.ContCoverGiftInfo.ContCoverGiftInfo.PreTime,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.ContCoverGiftInfo.ContCoverGiftInfo.PreItemID);
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case PLAYER_SVR_NEW_GIFT_INFO:
			{
				char szBuf[128];
				sprintf( szBuf,"SetNewGiftInfo( %d,%d,%d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.NewGiftInfo.NewRoleGiftInfo.NewRole,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.NewGiftInfo.NewRoleGiftInfo.End,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.NewGiftInfo.NewRoleGiftInfo.ID);
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case PLAYER_MT_GIFT_INFO:
			{
				char szBuf[128];
				sprintf( szBuf,"SetAwardMTGiftInfo( %d,%d,%d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.MTGiftInfo.AwardTime,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.MTGiftInfo.EndTime,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.MTGiftInfo.AwardMoneyVal);
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case PLAYER_BUCHANG_RES:
			{
				char szBuf[128];
				sprintf( szBuf,"ReturnGiftGetResult( %d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.BuChangRes.Succ);
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case PLAYER_YANGHAIBANG_UI_INFO:
			{
				char szBuf[128];
				sprintf( szBuf,"SetYangHaiBangUIInfo( %d,%d,%d,%d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.YangHaiBangUIInfo.EndTime,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.YangHaiBangUIInfo.Level,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.YangHaiBangUIInfo.Val,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.YangHaiBangUIInfo.tLifeEnd);
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}	
			break;
		case PLAYER_TOTEM_ACTIVE:
			{
				MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
				Item* item = pMain->getContainer().getItem(msgbody.PlayerMiscSvr.PlayerMiscSvrData.TotemActive.WID);
				item->m_nLostTotemState = msgbody.PlayerMiscSvr.PlayerMiscSvrData.TotemActive.Active;
				GameManager::getSingleton().SendUIEvent( "GE_CONTAINER_UPDATE" );
			}
			break;
		case PLAYER_GOLD_RATE:
			{
				char szBuf[128];
				sprintf( szBuf,"SetGoldRate( %d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.GoldRateRes.GoldRate );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case PLAYER_PINJIU_SCREEN_VAGUE:
			{
				unsigned int nCurtime = timeGetTime();
				GamePromotion::getSingleton().setPinJiuBlurTime( nCurtime );
				Ogre::BloomEffect::getSingleton().setGaussBlur( true );
			}
			break;
		case PLAYER_RONGYAN_MOVE_UI:
			{
				char szBuf[128];
				sprintf( szBuf,"OpenRongYanMonsterUI( %d, %d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.RongYanUIInfo.CurLayer,
										msgbody.PlayerMiscSvr.PlayerMiscSvrData.RongYanUIInfo.BeginSelectMul );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case PLAYER_RONGYAN_BOSS_TRUE_FALSE_UI:
			{
				char szBuf[128];
				sprintf( szBuf,"OpenRongYanMonsterGuessUI( %d, %d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.RongYanBossInfo.CurLayer,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.RongYanBossInfo.BeginSelectMul );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case ZHENGYI_LIMIT_CHG:
			{
				char szBuf[128];
				sprintf( szBuf,"UpdateZhengYiPoint( %d, %d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.ZhengyiChg.DayCount,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.ZhengyiChg.DayCountLimit );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case CHUAN_CHENG_CHG:
			{
				char szBuf[128];
				sprintf( szBuf,"UpdateChuanChengPerDay( %d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.ChuanChengPoint.Point );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case PLAYER_OPEN_PAOSHANG_UI_SVR:
			{
				static_assert( sizeof(tagCSPaoShangUIInfo_ToLua) == sizeof(tagCSPaoShangUIInfo) );
				tagUIMsgBody UI_Msg;
				memcpy( &UI_Msg.PaoShangData, &msgbody.PlayerMiscSvr.PlayerMiscSvrData.PaoShangUIInfo, sizeof(tagCSPaoShangUIInfo_ToLua) );
				GameUIMsg::getSingleton().setUIMsg( UI_Msg );
				GameManager::getSingleton().SendUIEvent( "GE_PAOSHANG_INFO" );
			}
			break;
		case PLAYER_DAQIAGNDAO_FENSHU:
			{
				char szBuf[128];
				sprintf( szBuf,"SetPworldCountNum( %d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.DaQiangDaoFenShu.iFenShu );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case PLAYER_WEEK_CHARM:
			{
				MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
				pMain->m_WeekCharm = msgbody.PlayerMiscSvr.PlayerMiscSvrData.WeekCharm.Charm;
			}
			break;
		case PLAYER_DIANFENG_FIRST_INFO:
			{
				GameTopCareer::getSingleton().SetTopCareerActiveTime( msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.ActiveTimeS );
				if( msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.Type == FIRST_INFO )
				{
					//保存八位职业冠军信息,按照DIANFENG_JOB_TYPE的顺序来
					GameTopCareer::getSingleton().SetCareerChampionInfo( DIANFENG_JOB_ZFZ, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFZFZFirstInfo.DFChampionInfo );
					GameTopCareer::getSingleton().SetCareerChampionInfo( DIANFENG_JOB_SHZ, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFSHZFirstInfo.DFChampionInfo );
					GameTopCareer::getSingleton().SetCareerChampionInfo( DIANFENG_JOB_YLS, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFYLSFirstInfo.DFChampionInfo );
					GameTopCareer::getSingleton().SetCareerChampionInfo( DIANFENG_JOB_SYZ, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFSYZFirstInfo.DFChampionInfo );
					GameTopCareer::getSingleton().SetCareerChampionInfo( DIANFENG_JOB_LSZ, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFLSZFirstInfo.DFChampionInfo );
					GameTopCareer::getSingleton().SetCareerChampionInfo( DIANFENG_JOB_HMZ, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFHMZFirstInfo.DFChampionInfo );
					GameTopCareer::getSingleton().SetCareerChampionInfo( DIANFENG_JOB_LHS, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFLHSFirstInfo.DFChampionInfo );
					GameTopCareer::getSingleton().SetCareerChampionInfo( DIANFENG_JOB_JNZ, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFJNZFirstInfo.DFChampionInfo );
					char szBuf[128];
					sprintf( szBuf,"OpenTopCareerFrame( %d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.Status );
					GameManager::getSingleton().getScriptVM()->callString( szBuf );
				}
				else if( msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.Type == FIRST_EFFIGY )
				{
					if( msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.WhichOne == 0 )
					{
						//保存八位职业冠军的雕像信息,按照DIANFENG_JOB_TYPE的顺序来
						GameTopCareer::getSingleton().SetEffigyCareerInfo( DIANFENG_JOB_ZFZ, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFZFZFirstInfo.DFEffigyInfo );
						GameTopCareer::getSingleton().SetEffigyCareerInfo( DIANFENG_JOB_SHZ, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFSHZFirstInfo.DFEffigyInfo );
						GameTopCareer::getSingleton().SetEffigyCareerInfo( DIANFENG_JOB_YLS, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFYLSFirstInfo.DFEffigyInfo );
						GameTopCareer::getSingleton().SetEffigyCareerInfo( DIANFENG_JOB_SYZ, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFSYZFirstInfo.DFEffigyInfo );
						GameTopCareer::getSingleton().SetEffigyCareerInfo( DIANFENG_JOB_LSZ, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFLSZFirstInfo.DFEffigyInfo );
						GameTopCareer::getSingleton().SetEffigyCareerInfo( DIANFENG_JOB_HMZ, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFHMZFirstInfo.DFEffigyInfo );
						GameTopCareer::getSingleton().SetEffigyCareerInfo( DIANFENG_JOB_LHS, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFLHSFirstInfo.DFEffigyInfo );
						GameTopCareer::getSingleton().SetEffigyCareerInfo( DIANFENG_JOB_JNZ, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFJNZFirstInfo.DFEffigyInfo );
					}
					else
					{
						switch( msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.WhichOne )
						{
						case DIANFENG_JOB_ZFZ:
							GameTopCareer::getSingleton().SetEffigyCareerInfo( DIANFENG_JOB_ZFZ, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFZFZFirstInfo.DFEffigyInfo );
							break;
						case DIANFENG_JOB_SHZ:
							GameTopCareer::getSingleton().SetEffigyCareerInfo( DIANFENG_JOB_SHZ, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFSHZFirstInfo.DFEffigyInfo );
							break;
						case DIANFENG_JOB_YLS:
							GameTopCareer::getSingleton().SetEffigyCareerInfo( DIANFENG_JOB_YLS, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFYLSFirstInfo.DFEffigyInfo );
							break;
						case DIANFENG_JOB_SYZ:
							GameTopCareer::getSingleton().SetEffigyCareerInfo( DIANFENG_JOB_SYZ, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFSYZFirstInfo.DFEffigyInfo );
							break;
						case DIANFENG_JOB_LSZ:
							GameTopCareer::getSingleton().SetEffigyCareerInfo( DIANFENG_JOB_LSZ, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFLSZFirstInfo.DFEffigyInfo );
							break;
						case DIANFENG_JOB_HMZ:
							GameTopCareer::getSingleton().SetEffigyCareerInfo( DIANFENG_JOB_HMZ, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFHMZFirstInfo.DFEffigyInfo );
							break;
						case DIANFENG_JOB_LHS:
							GameTopCareer::getSingleton().SetEffigyCareerInfo( DIANFENG_JOB_LHS, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFLHSFirstInfo.DFEffigyInfo );
							break;
						case DIANFENG_JOB_JNZ:
							GameTopCareer::getSingleton().SetEffigyCareerInfo( DIANFENG_JOB_JNZ, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengFirstInfo.DFJNZFirstInfo.DFEffigyInfo );
							break;
						}
					}
					GameTopCareer::getSingleton().ClearCareerEffigy();
					GameTopCareer::getSingleton().UpdateCareerEffigy();
				}
			}
			break;
		case PLAYER_DIANFENG_GROUP_UI_INFO:
			{
				GameTopCareer::getSingleton().SetCurCareerInfo( msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengGroupUIInfo.DFGroupInfo );
				char szBuf[128];
				sprintf( szBuf,"OpenGueesInfoFrame( %d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengGroupUIInfo.WhichOne );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case PLAYER_DIANFENG_PIPEI_SUCCEED_UI:
			{
				char szBuf[128];
				sprintf( szBuf,"SetTopCareerSucMatchState( %d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengPiPeiSucceedUI.res );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case PLAYER_DIANFENG_WIN_LOSS_UI:
			{
				msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengWinLossUI.WinLossInfo[0].Info;
				char szBuf[128];
				sprintf( szBuf,"SetTopCareerWinInfo( %d,%d,%d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengWinLossUI.WinLossInfo[0].Info,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengWinLossUI.WinLossInfo[1].Info, msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengWinLossUI.WinLossInfo[2].Info);
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case PLAYER_DIANFENG_BAOMING_UI_SVR:
			{
				if ( msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengBaoMingUI.Res == 0 )
				{
					GameManager::getSingleton().getScriptVM()->callString( "AddTopCareerInviteJoinTips()" );
				}
				else
				{
					char szBuf[128];
					sprintf( szBuf,"TopCareerInviteJoinResult( %d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengBaoMingUI.Res );
					GameManager::getSingleton().getScriptVM()->callString( szBuf );
				}
			}
			break;
		case PLAYER_DIANFENG_TRANSMIT_UI_SVR:
			{
				if ( msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengTransmitUI.Res == 0 )
				{
					GameManager::getSingleton().getScriptVM()->callString( "AddTopCareerTransJoinTips()" );
				}
				else
				{
					char szBuf[128];
					sprintf( szBuf,"TopCareerTransResult( %d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengTransmitUI.Res );
					GameManager::getSingleton().getScriptVM()->callString( szBuf );
				}
			}
			break;
		case PLAYER_DIANFENG_GUESS_RES_SVR:
			{
				char szBuf[128];
				sprintf( szBuf,"SetTopCareerGuessShow( %d,%d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengGuessResSvr.AllMoneyNum,
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengGuessResSvr.Res );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case PLAYER_DIANFENG_GUESS_MINGDAN:
			{
				GameTopCareer::getSingleton().SetGuessMingDan( msgbody.PlayerMiscSvr.PlayerMiscSvrData.DianFengGuessMingDan );
				GameManager::getSingleton().getScriptVM()->callString( "SetSelctWiners()" );
			}
			break;
		case CONTINUE_KILL_TANK:
			{
				GamePoly::getSingleton().setTankContineKill( msgbody.PlayerMiscSvr.PlayerMiscSvrData.ContinueKill.Continue );
			}
			break;
		case PLAYER_CLAN_PET_NOTIFY:
			{
				char szBuf[128];
				sprintf( szBuf,"setClanPetTouNum( %d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.ClanPetNotify.Notify.TouNum );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case PLAYER_BEACH_LOW_EXP_SVR:
			{
				char szBuf[128];
				sprintf( szBuf,"askIfBeginBeachLowExp()");
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case PLAYER_TEAM_MZT_LAYER_NOTIFY:
			{
				for ( int i = 0; i < msgbody.PlayerMiscSvr.PlayerMiscSvrData.MztLayerInfo.Num; i++ )
				{
					char szBuf[128];
					sprintf( szBuf,"AddTeamMember(%d,\"%s\",%d)", i+1, msgbody.PlayerMiscSvr.PlayerMiscSvrData.MztLayerInfo.Ones[i].Name,
						msgbody.PlayerMiscSvr.PlayerMiscSvrData.MztLayerInfo.Ones[i].Layer);
					GameManager::getSingleton().getScriptVM()->callString( szBuf );
				}
			}
			break;
		case PLAYER_UNREAL_EXP_LAYER_NOTIFY:
			{
				DreamLandManager::getSingleton().SetBufferLayer( msgbody.PlayerMiscSvr.PlayerMiscSvrData.UnrealExpLayerInfo.Layer );
			}
			break;
		case PLAYER_ZJ_RONGYAN_SVR:
			{
				if ( msgbody.PlayerMiscSvr.PlayerMiscSvrData.ZJRongYanInfo.Type == 2 )
				{
					GameManager::getSingleton().getScriptVM()->callString( "CloseHighLvRongYanStartUI()" );
				}else{
					char szBuf[128];
					sprintf( szBuf,"OpenHighLvRongYanStartUI(%d,%d,%d,%d,%d)",msgbody.PlayerMiscSvr.PlayerMiscSvrData.ZJRongYanInfo.DaoJiShi,
						msgbody.PlayerMiscSvr.PlayerMiscSvrData.ZJRongYanInfo.ExpMul,msgbody.PlayerMiscSvr.PlayerMiscSvrData.ZJRongYanInfo.Level,
						msgbody.PlayerMiscSvr.PlayerMiscSvrData.ZJRongYanInfo.NextExpMul,msgbody.PlayerMiscSvr.PlayerMiscSvrData.ZJRongYanInfo.CurExp);
					GameManager::getSingleton().getScriptVM()->callString( szBuf );
				}
			}
			break;
		case PLAYER_ZJ_RONGYAN_TRUE_FALSE_UI:
			{
				char szBuf[128];
				sprintf( szBuf,"OpenHighLvRongYanGuess(%d,%d)", msgbody.PlayerMiscSvr.PlayerMiscSvrData.ZJRongYanBossInfo.Type, msgbody.PlayerMiscSvr.PlayerMiscSvrData.ZJRongYanBossInfo.NextExpMul );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case PLAYER_WORD_PWD_UI:
			{
				char szBuf[256];
				sprintf( szBuf, "UpdateIterationTimeShow( %d,%d,%d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.WordPwdUI.Type, msgbody.PlayerMiscSvr.PlayerMiscSvrData.WordPwdUI.DaoJiShi, 
					msgbody.PlayerMiscSvr.PlayerMiscSvrData.WordPwdUI.Num );
				ActorManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case PLAYER_SCULPT_CHG_SVR:
			{
				char szBuf[256];
				sprintf( szBuf, "ShowFashionChangeResult( %d )", msgbody.PlayerMiscSvr.PlayerMiscSvrData.SculptChgSvr.Res );
				ActorManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case PLAYER_ENTER_SPAN_MAP_NOTIFY:
			{
				MainGamePlayer *mainplayer = ActorManager::getSingleton().getMainPlayer();
				if(mainplayer)
				{
					mainplayer->m_nCampIdx = msgbody.PlayerMiscSvr.PlayerMiscSvrData.EnterSpanMapNotify.CampIdx;
					GameManager::getSingleton().getScriptVM()->callString( "ClearPVPMapNPCWithoutID()" );
				}
			}
			break;
		case PLAYER_NEW_PEOPLE_INFO:
			{
				memcpy(&m_NewPeopleInfo, &msgbody.PlayerMiscSvr.PlayerMiscSvrData.NewPeopleInfo, sizeof(CSNEWPEOPLEINFO));
			}
			break;
		}
	}else if( AAS_SVR == msgid )
	{
		switch (msgbody.AasSvr.Type)
		{
		case AAS_SVR_UI:
			{
				GameManager::getSingleton().getScriptVM()->callString( "ShowContinueRegisterFrame()" );
			}
			break;
		case AAS_SVR_SUCC:
			{
				GameManager::getSingleton().getScriptVM()->callString( "HideContinueRegisterFrame()" );
			}
			break;
		}
	}
	return 0;
}

void LoginManager::reqireHeartBeat()
{
	tagCSPkgBody body;
	CSTIME& serveTime = body.HeartBeatReq.SvrTime;
	ServerTimer::getSingleton().getServerTime( serveTime.BenchTimeSec, serveTime.BenchTimeUSec );
	int nResult = SendNetMessage(m_GameNetCL, HEARTBEAT_REQ, body);
	// 若在不处于掉线的情况下，网络连接出错，则返回登录界面
	if ( !m_bOffLine && nResult < 0 )
	{
		GameManager::getSingleton().SendUIEvent( "GE_BREAK_NETWORK" );
	}

	// 若已经处于掉线状态，若收到重新连接的信号，则提示网络又重新连接上了
	if ( m_bOffLine && nResult == 0 )
	{
		m_bOffLine = false;
	}
}

void LoginManager::releaseLoginGameServer()
{
	if ( m_GameNetCL >= 0 )
	{
		INetworkManager::getSingleton().Release();
		m_GameNetCL		= GAME_DISCONNECT_FLAG;
	}
}

bool LoginManager::requestLoginGameServer( int RegionIdx, int WorldIdx, const char *username, const char *passwd )
{
	for( size_t i = 0 ; i < m_vecDir.size() ; i++ )
	{
		m_vecDir[i].Clear();
	}
	GameGetAccountAllRole::getSingleton().Reset();
	GameConfig &cfg = GameManager::getSingleton().GetConfig();
	if( m_GameNetCL < 0 )
	{	
		INetworkManager::getSingleton().Release();
		strncpy(m_UserName, username,64);
		int nRet = AUTH_ERR_NET;
		DirWorldInfo& world = m_BigWorlds[RegionIdx].getIthWorld(WorldIdx);

		SYSTEMTIME st;
		GetLocalTime( &st );
		LOG_INFO("connect server start at %d 时-%d 分-%d 秒", st.wHour, st.wMinute, st.wSecond);

		std::string authIp(GetSelectWorldAuthIp(world));
		std::string ip(GetSelectWorldIp(world));
		m_GameNetCL = INetworkManager::getSingleton().ConnectServer( authIp.c_str(), world.AuthPort,
			 				ip.c_str(), 
			 				world.port,
			 				m_UserName, 
			 				const_cast<char*>( passwd ),
			 				&nRet,getClientType(),0,NULL,NULL,NULL,NULL,
			 				GameManager::getSingleton().getMachineID() );

		// 若需要激活账户
		if ( nRet == AUTH_IFM_NEED_REGIST_CODE_ERROR )
		{
			GameManager::getSingleton().SendUIEvent("GE_NEED_REGIST_CODE");
			return false;
		}

		SetLastSelectData( RegionIdx, WorldIdx );

		// 需要密保卡
		if( nRet == AUTH_REASON_NEED_PASSWD_CARD_ERROR )
		{
			m_bWaitPassCard = true;
			memset( &m_stPasswdCardInfo, 0, sizeof(PASSWDCARDAUTHINFO) );
			GameManager::getSingleton().SendUIEvent( "GE_NEED_PASSWD_CARD" );
			return false;
		}
		if(m_GameNetCL < 0 ) 
		{
			char szScript[256];
			sprintf( szScript, "LoginErrorMes(%d)", nRet );
			GameManager::getSingleton().getScriptVM()->callString( szScript );
			return false;
		}
	}
	return true;
}

int LoginManager::UseBackUpIP( const DirWorldInfo &IthWorld, int nAuth_Flag, const char *passwd, const char *szMac, int nRet )
{
	char authIP_Bak[48];
	int	 nAuthIpNum;
	char IP_Bak[48];
	int	 nIpNum;
	if ( ( IthWorld.authIP_Bak1 == 0 || IthWorld.authIP_Bak1 == -1 ) && 
		 ( IthWorld.authIP_Bak2 == 0 || IthWorld.authIP_Bak2 == -1 ) && 
		 ( IthWorld.Ip_Bak1 == 0 || IthWorld.Ip_Bak1 == -1 ) &&
		 ( IthWorld.Ip_Bak2 == 0 || IthWorld.Ip_Bak2 == -1 ) )
	{
		return -1;
	}
	//选取较好的authIP_Bak
	if ( ( IthWorld.authIP_Bak1 == 0 || IthWorld.authIP_Bak1 == -1 ) && 
		 ( IthWorld.authIP_Bak2 == 0 || IthWorld.authIP_Bak2 == -1 )  )
	{
		strncpy(authIP_Bak, IthWorld.AuthIP, 48);
	}else
	{
		nAuthIpNum = PickBackUpSvrIP( IthWorld.authIP_Bak1 ,IthWorld.authIP_Bak2 );
		if ( nAuthIpNum == 1 )
		{
			strncpy(authIP_Bak,inet_ntoa(*(struct in_addr *)&IthWorld.authIP_Bak1 ),48);
		}else{
			strncpy(authIP_Bak,inet_ntoa(*(struct in_addr *)&IthWorld.authIP_Bak2 ),48);
		}
	}
	//选取较好的Ip_Bak
	if ( ( IthWorld.Ip_Bak1 == 0 || IthWorld.Ip_Bak1 == -1 ) &&
		 ( IthWorld.Ip_Bak2 == 0 || IthWorld.Ip_Bak2 == -1 ) )
	{
		strncpy(IP_Bak, IthWorld.ip, 48);
	}else
	{
		nIpNum = PickBackUpSvrIP( IthWorld.Ip_Bak1 ,IthWorld.Ip_Bak2 );
		if ( nIpNum == 1 )
		{
			strncpy(IP_Bak,inet_ntoa(*(struct in_addr *)&IthWorld.Ip_Bak1 ),48);
		}else {
			strncpy(IP_Bak,inet_ntoa(*(struct in_addr *)&IthWorld.Ip_Bak2 ),48);
		}
	}

	int nGameNetCL = INetworkManager::getSingleton().ConnectServer( authIP_Bak, 
													IthWorld.AuthPort,
													IP_Bak, 
													IthWorld.port,
													m_UserName, 
													const_cast<char*>( passwd ),
													&nRet,getClientType(),nAuth_Flag,NULL,NULL,NULL,NULL,szMac );
	SYSTEMTIME st;
	GetLocalTime( &st );
	LOG_INFO("connect server %s instead %s at %d 时-%d 分-%d 秒",IP_Bak,IthWorld.ip,st.wHour, st.wMinute, st.wSecond);
	strncpy( ( char* )&IthWorld.ip, IP_Bak, 48 );
	strncpy( ( char* )&IthWorld.AuthIP, authIP_Bak, 48 );

	char buff[256];
	sprintf( buff,"RefreshIthWorldInfo()");
	ActorManager::getSingleton().getScriptVM()->callString(buff);
	
	return nGameNetCL;
}

int LoginManager::PickBackUpSvrIP( int bak1 ,int bak2 )
{
	char Bak1[48];
	char Bak2[48];
	int	 Interval1;
	int  Interval2;
	CPing BakIP1;
	CPing BakIP2 ;
	if ( bak1 != 0 && bak1 != -1 )
	{
		strncpy(Bak1,inet_ntoa(*(struct in_addr *)&bak1),48);
		BakIP1.Ping( Bak1);
		
		Interval1 = BakIP1.nReceivedTime ;

		if ( (bak2 == 0 || bak2 == -1) && Interval1 < 1000  )
		{
			return 1;
		}
	}
	if ( bak2 != 0 && bak2 != -1 )
	{
		strncpy(Bak2,inet_ntoa(*(struct in_addr *)&bak2 ),48);
		BakIP2.Ping( Bak2 );

		Interval2 = BakIP2.nReceivedTime;

		if ( (Interval1 >= 1000 && Interval2 >= 1000 ) || abs(Interval1 - Interval2) < 100 )
		{
			return -1;
		}
		if ( bak1 != 0 && bak1 != -1 )
		{
			return Interval1 >= Interval2 ? 2 : 1;
		}else{
			return 2;
		}
	}
	return 1;
}
DWORD WINAPI TestServerBackUpIp( void* data )
{
	DirWorldInfo * IthWorld =  ( DirWorldInfo * )data;
	char IP_Bak[48];
	int	 nIpNum;
	
	if ( IthWorld->Ip_Bak1 != 0 && IthWorld->Ip_Bak1 != -1  &&
		 IthWorld->Ip_Bak2 != 0 && IthWorld->Ip_Bak2 != -1  )
	{
		//选取较好的Ip_Bak
		nIpNum = LoginManager::getSingleton().PickBackUpSvrIP( IthWorld->Ip_Bak1 ,IthWorld->Ip_Bak2 );
		if ( nIpNum == -1 )
		{
			strncpy( LoginManager::getSingleton().m_IpBak, IthWorld->ip, 48 );
			LoginManager::getSingleton().m_bIsAlive = true;
			return 0;
		}
		if ( nIpNum == 1 )
		{
			strncpy(IP_Bak,inet_ntoa(*(struct in_addr *)&IthWorld->Ip_Bak1 ),48);
		}else if ( nIpNum == 2 )
		{
			strncpy(IP_Bak,inet_ntoa(*(struct in_addr *)&IthWorld->Ip_Bak2 ),48);
		}
		strncpy( LoginManager::getSingleton().m_IpBak, IP_Bak, 48 );
		LoginManager::getSingleton().m_bIsAlive = true;
		return 0;
	}
	LoginManager::getSingleton().m_bIsAlive = false;
	return 0;
}

void LoginManager::getOptimalBakIp( DirWorldInfo * IthWorld )
{
	return;
	m_bIsAlive = false;
	strncpy( m_IpBak, IthWorld->ip, 48 );
	if ( m_LastThread != 0 )
	{
		TerminateThread( m_LastThread, 0);
		CloseHandle( m_LastThread );
	}

	DWORD threadid;
	HANDLE threadHandle = CreateThread( NULL, 0, TestServerBackUpIp, IthWorld, 0, &threadid );
	m_LastThread = threadHandle;
	//CloseHandle( threadHandle );
}

bool LoginManager::requestActiveAccount( int RegionIdx, int WorldIdx , const char *pszActiveCode )
{
	// 同上次链接时间间隔需超过6秒
	static int nStartTime = 0;
	if ( timeGetTime() - nStartTime < CONNECT_SERTIME )
	{
		return false;
	}else{
		nStartTime = timeGetTime();
	}

	if ( m_GameNetCL < 0 )
	{
		INetworkManager::getSingleton().Release();
		int nRet = AUTH_ERR_NET;
		DirWorldInfo& world = m_BigWorlds[RegionIdx].getIthWorld(WorldIdx);

		std::string authIp(GetSelectWorldAuthIp(world));
		std::string ip(GetSelectWorldIp(world));
		m_GameNetCL = INetworkManager::getSingleton().ActiveAccount( 
			authIp.c_str(), 
			world.AuthPort,
			ip.c_str(), 
			world.port,
			const_cast<char*>( pszActiveCode ),
			GameManager::getSingleton().getMachineID(),
			getClientType(),
			&nRet );

		if ( m_GameNetCL < 0) 
		{
			char szScript[256];
			sprintf( szScript, "LoginErrorMes(%d)", nRet );
			GameManager::getSingleton().getScriptVM()->callString( szScript );
			return false;
		}
		if( nRet == AUTH_IFM_ACCOUNT_REGISTER_SUCCES )
		{
			char szScript[256];
			sprintf( szScript, "LoginErrorMes(%d)", nRet );
			GameManager::getSingleton().getScriptVM()->callString( szScript );
		}
		SetLastSelectData( RegionIdx, WorldIdx );
	}
	return true;
}

void LoginManager::requestCreateRole( const char* szNickName,int Gender,int Race, int Face, int Head, int Hair, int nClothes )
{
	tagCSPkgBody body;
	CSROLENEWREQ& roleNewReqData	= body.RoleNewReq;
	roleNewReqData.Career			= Race;
	roleNewReqData.Gender			= Gender;
	roleNewReqData.Face				= Face;
	roleNewReqData.Head				= Head;
	roleNewReqData.Hair				= Hair;
	roleNewReqData.ApperalSelect	= nClothes;
	size_t nSrcStrLen = strlen( szNickName );
	strncpy( roleNewReqData.RoleName, szNickName, nSrcStrLen );
	roleNewReqData.RoleName[nSrcStrLen] = '\0';

	SendNetMessage( m_GameNetCL, ROLE_NEW_REQ, body );
}

void LoginManager::requestDeleteSelectedRole()
{
	if(m_SelectRoleIndex < 0) return;

	tagCSPkgBody body;
	body.RoleDelReq.RoleID = m_Roles[m_SelectRoleIndex].roleid;

	SendNetMessage(m_GameNetCL, ROLE_DEL_REQ, body);
}

void LoginManager::requestEnterWorld()
{
	assert(size_t(m_SelectRoleIndex) < m_Roles.size());

// 	ServerTimer::getSingleton().beginBench();
	tagCSPkgBody body;
	body.RoleLoginReq.RoleID = m_Roles[m_SelectRoleIndex].roleid;
	SendNetMessage(m_GameNetCL, ROLE_LOGIN_REQ, body);
}

void LoginManager::requestRoleList()
{
	tagCSPkgBody msgbody;
	msgbody.RoleLogoutReq.RoleID = ActorManager::getSingleton().getMainPlayer()->getRoleWId();
	SendGameServerMsg( ROLE_LOGOUT_REQ, msgbody );
}

void LoginManager::requestLogout()
{
	//在有连接的时候，才请求退出
	if( m_GameNetCL >= 0 )
	{
		tagCSPkgBody body;
		body.AccLogoutReq.Uin = atoi(m_UserName);
		body.AccLogoutReq.LogoutFor = LOGOUT_FOR_NORMAL;
		SendNetMessage(m_GameNetCL, ACC_LOGOUT_REQ, body);
		GameStateManager::changeGameState(GameStateManager::STATE_DIRLIST);
		m_GameNetCL = -1;
	}
	m_bOffLine = true;
}

void LoginManager::requestCancelLogout()
{
	if( ActorManager::getSingleton().getMainPlayer() != NULL )
	{
		tagCSPkgBody msgbody;
		msgbody.RoleLogoutReq.RoleID = ActorManager::getSingleton().getMainPlayer()->getRoleWId();
		SendGameServerMsg( CANCEL_ROLE_LOGOUT, msgbody );
	}
}

void LoginManager::requestSelectBirthLocation( int index )
{
	assert( index < m_SelectBirth.BirthMapNum );
	tagCSPkgBody body;
	body.SelectBirthLocC.MapID = m_SelectBirth.MapIDList[index];
	SendNetMessage( m_GameNetCL, SELECT_BIRTH_LOCATION_CLT, body );
}

void LoginManager::requestCancelSelect()
{
	tagCSPkgBody body;
	body.SelectBirthLocC.MapID = 0;
	SendNetMessage( m_GameNetCL, SELECT_BIRTH_LOCATION_CLT, body );
}

tagClientSelectBirth& LoginManager::getSelectBirthLocation()
{
	return m_SelectBirth;
}

int LoginManager::getSelectBirthIndex()
{
	return m_SelectBirthIndex;
}

const char* LoginManager::getMapName(int mapId)
{
	const tagMapDef* pmapdef = &DBDataManager::getSingleton().getMapDef( mapId );
	//assert( pmapdef != NULL );
	if ( pmapdef == NULL )
	{
		return NULL;
	}
	return pmapdef->Name;
}

int LoginManager::getNumRole()
{
	return (int)m_Roles.size();
}

LoginRole &LoginManager::getIthRole(int i)
{
	assert(size_t(i) < m_Roles.size());
	return m_Roles[i];
}

void LoginManager::setAvatarAttrib(int i)
{
	clearEquipIdList();
	LoginRole role = getIthRole(i);
	for( int i = 0; i < role.armInfo.Num; i++ )
	{
		setArmEquipIdList( role.armInfo.Arms[i].GridIdx, role.armInfo.Arms[i].ArmDefID,role.armInfo.Arms[i].StarLv );
		//接着处理下背饰信息
		if( role.armInfo.BackModelID > 0 && role.armInfo.Arms[i].GridIdx == EQUIP_POS_FASHION_BACK  )
		{
			setArmEquipIdList( role.armInfo.Arms[i].GridIdx, role.armInfo.BackModelID,role.armInfo.Arms[i].StarLv );
		}
	}
	setAvatarSelectSuit( role.armInfo.SelectFashion );
	g_AvatarChange(); 
	::setAvatarAttrib( 0, role.race );
	::setAvatarAttrib( 1, role.gender-1 );
	::setAvatarAttrib( POS_ID_HAIR, role.hair + 1 );
	::setAvatarAttrib( POS_ID_FACE, role.face );
	::setAvatarAttrib( POS_ID_HEAD, role.head );
	if( role.map == BEACH_MAP_ID )
	{
		::setAvatarAttrib( POS_ID_FASHION_HAIR,1 );
	}
	g_ConfigFashionFair();
}

void LoginManager::selectRole(int i)
{
	m_SelectRoleIndex = i;
}

void LoginManager::beginRotatePreview(int speed)
{
}

void LoginManager::endRotatePreview()
{
}

void LoginManager::onEscPress()
{
}

bool LoginManager::isRoleNumFull()
{
     if (m_Roles.size() < ROLE_USER_NUM )
        return false;  
	 else
        return true;

	// return m_Roles.size() == ROLE_USER_NUM;
}

void LoginManager::selectLastLoginRole()
{
    int nSelectIndex    = -1;
    int nLastTime       = 0;

    for( int i = 0; i < (int)m_Roles.size(); i++ )
    {
        if( m_Roles[i].lastlogin >= nLastTime )
        {
            nSelectIndex = i;
            nLastTime = m_Roles[i].lastlogin;
        }
    }
  
    selectRole( nSelectIndex );
}

bool LoginManager::isBackFromInWorld()
{
	if(GameStateManager::getLastGameState()==GameStateManager::STATE_INWORLD
		&&GameStateManager::getGameState() == GameStateManager::STATE_ROLELIST )
		return true;
	
	return false;
}

void LoginManager::requestStartMud()
{
	//在有连接的时候，才请求退出
	if( m_GameNetCL >= 0 )
	{
		tagCSPkgBody body;
		body.AccLogoutReq.Uin = atoi(m_UserName);
		body.AccLogoutReq.LogoutFor = LOGOUT_FOR_MUD;
		SendNetMessage(m_GameNetCL, ACC_LOGOUT_REQ, body);
// 		GameStateManager::changeGameState(GameStateManager::STATE_DIRLIST);
// 		m_GameNetCL = -1;
	}
// 	m_bOffLine = true;
}

struct MudData
{
	NetCltAuthInfo authInfo;
	char		   ip[64];
	unsigned short port;
	unsigned short mudport;
	tdr_ulonglong RoleID;
};
void LoginManager::startMud()
{
	MudData data;
	get_clt_auth_han_info(&data.authInfo);
	strncpy(data.ip,m_szLastLoginIp,32);
	data.mudport	= m_uLastLoginMudPort;
	data.port		= m_uLastLoginPort;
	data.RoleID	= m_Roles[m_SelectRoleIndex].roleid;

	char pCtemp[100];
	strcpy(pCtemp,"directzone");
	strcat(pCtemp,data.authInfo.AccountLoginName);
	FILE* f = NULL;
	if (_access(pCtemp,0) == 0)
	{
		DeleteFile(pCtemp);
	}
	f = fopen(pCtemp,"wb");
	fwrite(&data,sizeof(MudData),1,f);
	fclose(f);
	//requestLogout();
	SHELLEXECUTEINFO info;
	memset(&info,0,sizeof(SHELLEXECUTEINFO));
	/*info.lpVerb = "runas";
	
	info.nShow = SW_SHOW;*/
	info.cbSize = sizeof(SHELLEXECUTEINFO);
	info.lpParameters = (LPCSTR)pCtemp;
	info.lpFile = "offline2.exe";
	
	if( ShellExecuteEx(&info) )
	{
		GameManager::getSingleton().QuitGame();
	}else
	{
		DeleteFile(pCtemp);
	}
}


bool LoginManager::requestLoginGameServerDirect(const char *pdirectmud)
{
	LoadLocalSvr();
	tryRequestDirInfo();
	MudData data;
	FILE* f = NULL;
	if(_access(pdirectmud,0) != 0)
	{
		return false;
	}
	f = fopen(pdirectmud,"rb");
	fread(&data,sizeof(MudData),1,f);
	fclose(f);
	int nRet = AUTH_ERR_NET;
	
	int i = 0, j = 0;
	const DirWorldInfo*pWorld = NULL;
	for( i = 0 ; i < (int)m_BigWorlds.size(); ++i )
	{
		for( j = 0 ; j < m_BigWorlds[i].getNumWorld(); ++j )
		{
			if( strcmp(m_BigWorlds[i].getIthWorld(j).ip,data.ip) == 0 
				&& m_BigWorlds[i].getIthWorld(j).port == data.port )
			{
				pWorld = &(m_BigWorlds[i].getIthWorld(j));
				break;
			}
		}
	}
	if( pWorld == NULL )
	{
		return false;
	}

	std::string ip(GetSelectWorldIp(*pWorld));
	m_GameNetCL = INetworkManager::getSingleton().DirectConnectServer( (char*)&data.authInfo,
														ip.c_str(),
														pWorld->port,
														GameManager::getSingleton().getMachineID(),
														getClientType(),
														&nRet );
	if( nRet != 0 || m_GameNetCL < 0 )
	{
		return false;
	}
	m_DirectloginRoleID	=	data.RoleID;
	DeleteFile(pdirectmud);

	SetLastSelectData( i, j );
	m_bStartfmud=true;
	return true;
}

tagBirthItemDef_ToLua* LoginManager::getBirthItemDef( int nCareer, int nGender )
{
	for ( std::vector<tagBirthItemDef*>::iterator iter = m_vecBirthItemDef.begin(), end = m_vecBirthItemDef.end();
			iter != end; ++iter )
	{
		tagBirthItemDef* pOneBirthItemDef = *iter;
		if ( pOneBirthItemDef->Career == nCareer && pOneBirthItemDef->Gender == nGender )
		{
			return (tagBirthItemDef_ToLua*)pOneBirthItemDef;
		}
	}

	return NULL;
}

tagCSNewPeopleInfo_ToLua* LoginManager::getNewPeopleInfo()
{
	assert(sizeof(tagCSNewPeopleInfo_ToLua) == sizeof(CSNEWPEOPLEINFO));
	return (tagCSNewPeopleInfo_ToLua*)&m_NewPeopleInfo;
}

void LoginManager::EnterWorldDirect()
{
	for (int i = 0;i < (int)m_Roles.size();i++)
	{
		if ( m_DirectloginRoleID == m_Roles[i].roleid )
		{
			m_SelectRoleIndex	= i;
			requestEnterWorld();
			break;
		}
	}
}

bool LoginManager::isInBattleWorld()
{
	int mapid = GameManager::getSingleton().getGameMapShower()->getRealMapId();
	if( mapid >= GAME_PWORLD_MAPID )
	{
		int nPworldId = mapid/100;
		const tagPworldDef* pPworldDef = DBDataManager::getSingleton().getPworldDef( nPworldId );
		if( pPworldDef != NULL && pPworldDef->PolyType == PWORLD_POLY_SPAN_WORLD )
		{
			return true;
		}
	}
	return false;
}

void LoginManager::saveLastUserName()
{
	int nIdx = -1;
	for( int i = 0 ; i < MAX_SAVE_NAME_NUM ; i++ )
	{
		if( strcmp( m_SaveUserName[i],m_UserName ) == 0 )
		{
			nIdx = i;			
		}
		if( nIdx == -1 && strlen( m_SaveUserName[i] ) == 0 )
		{
			nIdx = i;
		}
	}if( nIdx == -1 )
	{
		nIdx = 2;
	}
	for( int i = nIdx ; i > 0 ; i -- )
	{
		char tmp[MAX_USER_NAME_LENGTH];
		strcpy( tmp,m_SaveUserName[i-1] );
		strcpy( m_SaveUserName[i],m_SaveUserName[i-1]);
		strcpy( m_SaveUserName[i],tmp);
	}
	strcpy( m_SaveUserName[0],m_UserName );
	m_nSaveUserName = 1;
	saveLastUserData();
}

void LoginManager::clearLastUserName()
{
	for( int i = 0 ; i < MAX_SAVE_NAME_NUM ; i++ )
	{
		m_SaveUserName[i][0] = '\0';
	}
	m_nSaveUserName = 0;
	saveLastUserData();
}

int	LoginManager::getSaveUserCheck()
{
	return m_nSaveUserName;
}

const char* LoginManager::getLastUserName()
{
	return m_SaveUserName[0];
}

const char* LoginManager::getUserNameByIdx( int idx )
{
	if( idx < MAX_SAVE_NAME_NUM )
	{
		return m_SaveUserName[idx];
	}
	return "";
}

void LoginManager::LoadLastUserName()
{
	XMLData xmldata;
	if( xmldata.loadFile( "UserInfoLoginRecord.xml" ) )
	{
		XMLNode pRootNode = xmldata.getRootNode();
		XMLNode pChildNode = pRootNode.iterateChild();
		while( !pChildNode.isNull() )
		{
			if( stricmp(pChildNode.getName(),"UserName") == 0 )
			{
				if( pChildNode.hasAttrib("saveuser") )
				{
					m_nSaveUserName = pChildNode.attribToInt( "saveuser" );
				}
				if( pChildNode.hasAttrib("username") )
				{
					strcpy( m_SaveUserName[0], pChildNode.attribToString("username") );
				}
				if( pChildNode.hasAttrib("username2") )
				{
					strcpy( m_SaveUserName[1], pChildNode.attribToString("username2") );
				}
				if( pChildNode.hasAttrib("username3") )
				{
					strcpy( m_SaveUserName[2], pChildNode.attribToString("username3") );
				}
			}else if( stricmp(pChildNode.getName(),"selectWorld") == 0 )
			{
				m_nLastSelectWorldId = pChildNode.attribToInt( "worldId" );
				if( pChildNode.hasAttrib("worldName") )
				{
					m_strLastWorldName = pChildNode.attribToString("worldName");
				}
			}else if( stricmp(pChildNode.getName(),"IpMode") == 0 )
			{
				m_nIpMode = pChildNode.attribToInt( "IpMode" );
			}
			pChildNode = pRootNode.iterateChild(pChildNode);
		}
	}
}

void LoginManager::saveLastUserData()
{
	TiXmlDocument doc;
	TiXmlElement* root = new TiXmlElement( "SaveUser" );
	doc.LinkEndChild( root );
	TiXmlElement* pUserElement = new TiXmlElement( "UserName");
	root->LinkEndChild(pUserElement);
	pUserElement->SetAttribute( "saveuser", m_nSaveUserName );
	if( strlen( m_SaveUserName[0] ) > 0 )
	{
		pUserElement->SetAttribute( "username", m_SaveUserName[0] );
	}
	if( strlen( m_SaveUserName[1] ) > 0 )
	{
		pUserElement->SetAttribute( "username2", m_SaveUserName[1] );
	}
	if( strlen( m_SaveUserName[2] ) > 0 )
	{
		pUserElement->SetAttribute( "username3", m_SaveUserName[2] );
	}
	TiXmlElement* pWorldElement = new TiXmlElement( "selectWorld");
	root->LinkEndChild(pWorldElement);
	pWorldElement->SetAttribute( "worldId", m_nLastSelectWorldId );

	pWorldElement->SetAttribute( "worldName",m_strLastWorldName.c_str() );

	TiXmlElement* pIpModeElement = new TiXmlElement( "IpMode" );
	root->LinkEndChild(pIpModeElement);
	pIpModeElement->SetAttribute( "IpMode", m_nIpMode );

	doc.SaveFile( "UserInfoLoginRecord.xml" );
}

bool LoginManager::requestCreateUser( int RegionIdx, 
									 int WorldIdx, 
									 const char *username, 
									 const char *passwd,
									 const char* szCertID,
									 const char* szRealName,
									 const char* szEmail,
									 const char* szMobile )
{
	for( size_t i = 0 ; i < m_vecDir.size() ; i++ )
	{
		m_vecDir[i].Clear();
	}
	GameGetAccountAllRole::getSingleton().Reset();
	static int nStartTime = 0;
	if( timeGetTime() - nStartTime < CONNECT_SERTIME )
	{
		return false;
	}else{
		nStartTime = timeGetTime();
	}
	GameConfig &cfg = GameManager::getSingleton().GetConfig();
	if( m_GameNetCL < 0 )
	{	
		INetworkManager::getSingleton().Release();

		strncpy(m_UserName, username,64);
		int nRet = AUTH_ERR_NET;
		DirWorldInfo& world = m_BigWorlds[RegionIdx].getIthWorld(WorldIdx);

		std::string authIp(GetSelectWorldAuthIp(world));
		std::string ip(GetSelectWorldIp(world));
		m_GameNetCL = INetworkManager::getSingleton().ConnectServer( authIp.c_str(), 
			world.AuthPort,
			ip.c_str(), 
			world.port,
			m_UserName, 
			const_cast<char*>( passwd ),
			&nRet,getClientType(),AUTH_FLAG_REG,
			szCertID,
			szRealName,
			"user@wanyouyl.com",
			szMobile,
			GameManager::getSingleton().getMachineID() );

// 		//连接失败使用备用ip
// 		if ( m_GameNetCL < 0 && nRet == AUTH_ERR_NET )
// 		{
// 			m_GameNetCL = UseBackUpIP( m_BigWorlds[i].getIthWorld(j), AUTH_FLAG_REG, passwd, GameManager::getSingleton().getMachineID(), nRet );
// 		}
		// 若需要激活账户
		if ( nRet == AUTH_IFM_NEED_REGIST_CODE_ERROR )
		{
			GameManager::getSingleton().SendUIEvent("GE_NEED_REGIST_CODE");
			return false;
		}

		SetLastSelectData( RegionIdx, WorldIdx );

		if( m_GameNetCL < 0 )
		{
			char szScript[256];
			sprintf( szScript, "LoginErrorMes(%d)", nRet );
			GameManager::getSingleton().getScriptVM()->callString( szScript );
			return false;
		}
		if( nRet == AUTH_IFM_ACCOUNT_REGISTER_SUCCES )
		{
			char szScript[256];
			sprintf( szScript, "LoginErrorMes(%d)", nRet );
			GameManager::getSingleton().getScriptVM()->callString( szScript );
			return false;
		}
	}
	return true;
}

void LoginManager::continueRegisterUserInfo( const char* szCertID, const char* szRealName, const char* szEmail )
{
	tagCSPkgBody msgbody;
	msgbody.AasClt.Type = AAS_CLT_ADD_ID_INFO;
	tagCSAasIDInfo& IDinfo = msgbody.AasClt.Data.IDInfo;
	strncpy( IDinfo.IDCard, szCertID, sizeof( IDinfo.IDCard ) );
	strncpy( IDinfo.Name, szRealName, sizeof( IDinfo.Name ) );
	strncpy( IDinfo.eMail, szEmail, sizeof( IDinfo.eMail ) );
	SendGameServerMsg( AAS_CLT, msgbody );
}

int LoginManager::getLastSelectArea()
{
	if( m_nLastSelectWorldId != 0 )
	{
		for( int i = 0; i < (int)m_BigWorlds.size(); i++ )
		{
			for( int j = 0; j < m_BigWorlds[i].getNumWorld(); j++ )
			{
				if( m_BigWorlds[i].getIthWorld(j).worldid == m_nLastSelectWorldId )
				{
					if( m_strLastWorldName.empty() || m_strLastWorldName == m_BigWorlds[i].getIthWorld(j).name )
					{
						return i;
					}
				}
			}
		}
	}
	return -1;
}

int LoginManager::getLastSelectWorld()
{
	if( m_nLastSelectWorldId != 0 )
	{
		for( size_t i = 0; i < m_BigWorlds.size(); i++ )
		{
			for( int j = 0; j < m_BigWorlds[i].getNumWorld(); j++ )
			{
				if( m_BigWorlds[i].getIthWorld(j).worldid == m_nLastSelectWorldId )
				{
					if( m_strLastWorldName.empty() || m_strLastWorldName == m_BigWorlds[i].getIthWorld(j).name )
					{
						return j;
					}
				}
			}
		}
	}
	return -1;
}

bool LoginManager::getReadUserProtocol()
{
	XMLData& xmlData = Root::getSingleton().m_Config;

	XMLNode loginReadProtocol = xmlData.getRootNode().getChild("Login");

	if ( !loginReadProtocol.isNull() && loginReadProtocol.hasAttrib("accept") )
	{
		return loginReadProtocol.attribToBool("accept");
	}
	else
	{
		return true;
	}

}

void LoginManager::setReadUserProtocol( bool bAccept )
{
	XMLData& xmlData = Root::getSingleton().m_Config;
	if ( !xmlData.loadRawFile( CLIENT_CFG_PATH ) )
	{
		return;
	}

	XMLNode	rootNode = xmlData.getRootNode();
	XMLNode loginReadProtocolNode	= rootNode.getChild("Login");
	if ( loginReadProtocolNode.isNull() )
	{
		rootNode.m_pElement->LinkEndChild( new TiXmlElement( "Login") );
		loginReadProtocolNode = rootNode.getChild("Login");
	}

	loginReadProtocolNode.setAttribBool( "accept",	bAccept	);
	xmlData.saveFile( CLIENT_CFG_PATH );
}

void LoginManager::replyPlayerMisc( int nBindType,int nId,int nChooseType )
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_CHOOSE_CLT;
	msgbody.PlayerMiscClt.PlayerMiscCltData.ChooseC.BindType = nBindType;
	msgbody.PlayerMiscClt.PlayerMiscCltData.ChooseC.MemID = nId;
	msgbody.PlayerMiscClt.PlayerMiscCltData.ChooseC.ChooseType = nChooseType;
	SendGameServerMsg( PLAYER_MISC_CLT,msgbody );
}

void LoginManager::breakNetConnect()
{
	if( m_GameNetCL >= 0 )
	{
		INetworkManager::getSingleton().Release();
		GameStateManager::changeGameState(GameStateManager::STATE_DIRLIST);
		m_GameNetCL = -1;
	}
	m_bOffLine = true;
	m_bWaitPassCard = false;

	SkillManager::getSingleton().onStop();
	GameShortCut::getSingleton().clearSpringSpell();
	m_ServerName[0] = '\0';
}

void LoginManager::onMapPreLoaded(GameMap *map, const CSENTERMAP& entermap)
{
	GameStateManager::changeGameState( GameStateManager::STATE_INWORLD );
	MainGamePlayer *player = ActorManager::getSingleton().getMainPlayer();
	m_MapID = entermap.Map;
	if( player != NULL )
	{
		bool isRepeatEnter = false;
		if( entermap.PworldInfoFlag == 0 )
		{
			player->m_bPworldInfoFlag = false;
			player->m_nPworldLeaveTime = player->m_nPworldStartTime = 0;
			player->m_nPworldInstID = 0;
		}else{
			player->m_bPworldInfoFlag = true; 
			player->m_nPworldLeaveTime = entermap.PworldInfo.LeftTime;
			player->m_nPworldStartTime = entermap.PworldInfo.StartTime;
			if (player->m_nPworldInstID == entermap.PworldInfo.PworldInstID)
			{
				isRepeatEnter = true;
			}
			else
			{
				player->m_nPworldInstID = entermap.PworldInfo.PworldInstID;
			}
		}
		player->setMoveSeq( entermap.InitMoveSeq );
		player->getSpecialMove()->setType(MOVETYPE_NOMAL);
		int nPreMapId = 0;
		if (player->getCurMap())
		{
			nPreMapId = player->getCurMap()->getRealId();
		}

		int nCurRealMapId = GameManager::getSingleton().getGameMapShower()->getRealMapId();
		if( nCurRealMapId < GAME_PWORLD_MAPID && entermap.RealMap >= GAME_PWORLD_MAPID )
		{
			GamePlayManager::getSingleton().setCameraAngle( entermap.RealMap );
		}

		GameManager::getSingleton().getGameMapShower()->setRealMapId( entermap.RealMap );
		MapLineSelectManager::getSingleton().requestMapLineInfo();
		PworldLimitInfo::getSingleton().onEnterMap(entermap.RealMap);
		GamePlayManager::getSingleton().enterMap(map, entermap.x, entermap.y);

		GameManager::getSingleton().SendUIEvent("GE_ENTER_MAP");

		int nCameraAngle = entermap.CameraDir*2;

		// 如果是从副本出来且设置过相机转向 则将相机转向设置为进副本前的转向
		tagPlayerCameraDir* playerCameraDir = GamePlayManager::getSingleton().getPlayerCameraDir();
		if( entermap.RealMap < GAME_PWORLD_MAPID && playerCameraDir->m_nMapId >= GAME_PWORLD_MAPID ){
			nCameraAngle = GamePlayManager::getSingleton().getCameraAngle();
		}

		GamePlayManager::getSingleton().getMainCamera()->SetAngle( nCameraAngle );
		player->SetFaceTo( entermap.Dir*2 + 90, true );

		if (m_isFirstLoad)
		{
			m_isFirstLoad = false;
			GameManager::getSingleton().SendUIEvent("GE_ENTER_PLAYERLOGIN");
			GameMsgPrint::getSingleton().SendUIEvent( "GE_LOGIN_GAME_SETTING" );
		}
		else
		{
			//Ogre::BloomEffect::getSingleton().playRadialBloom(1500, true);
		}

		if( player->m_bPworldInfoFlag )
		{
			if (nPreMapId != entermap.RealMap)
			{
				GamePlayManager::getSingleton().leavePwolrd( nPreMapId );
			}
			GamePlayManager::getSingleton().enterPworld( entermap.RealMap );
			tagUIMsgBody UIMsg;
			const tagPworldDef *pworldDef = DBDataManager::getSingleton().getPworldDef(entermap.RealMap / 100);
			if (pworldDef && pworldDef->DynType)
			{
				UIMsg.PworldInfo.PworldIdx = entermap.PworldInfo.SelectGrade + 1;
			}
			else
			{
				UIMsg.PworldInfo.PworldIdx = 0;
			}
			GameUIMsg::getSingleton().setUIMsg(UIMsg);
			//是同一个FB
			if( isRepeatEnter )
			{
				GameManager::getSingleton().SendUIEvent("GE_REENTER_PWORLD");
			}
			else
			{
				GameManager::getSingleton().SendUIEvent("GE_ENTER_PWORLD");
			}
		}else{
			GamePlayManager::getSingleton().leavePwolrd( nPreMapId );
		}
		char buf[256];
		sprintf( buf,"ShowMapEndTime( %d )",entermap.MapEndTime );
		GameManager::getSingleton().getScriptVM()->callString( buf );

		if ( entermap.Span == MAP_SPAN_NONE )
		{
			player->m_bIsOutSide = false;
		}
		else if ( entermap.Span == MAP_SPAN_BATTLE )
		{
			player->m_bIsOutSide = true;
		}
	} 
}

void LoginManager::setFirstLoad(int mapid)
{
	m_isFirstLoad = true;
	char szBuf[256];
	sprintf( szBuf,"BeginEnterMap(%d)", mapid );
	GameManager::getSingleton().getScriptVM()->callString( szBuf );
}

bool LoginManager::isFirstLoad()
{
	return m_isFirstLoad;
}


// 获取 dir 列表
DirBigWorldInfo::DirBigWorldInfo()
{
	m_Name[0]		= 0;
	m_uStatusFlag	= 0;
	m_nDirIdx		= -1;
}

const char *DirBigWorldInfo::getName()
{
	return m_Name;
}

int DirBigWorldInfo::getNumWorld()
{
	return (int)m_Worlds.size();
}

DirWorldInfo &DirBigWorldInfo::getIthWorld(int i)
{
	if( i < m_Worlds.size() )
	{
		return m_Worlds[i];
	}
	else
	{
		return m_Worlds[0];
	}
}

bool DirBigWorldInfo::isDianXinArea()
{
	return ( m_uStatusFlag & REGIONSTATUSBIT_DIANXIN ) != 0;
}

bool DirBigWorldInfo::isWangTongArea()
{
	return ( m_uStatusFlag & REGIONSTATUSBIT_WANGTONG ) != 0;
}

bool DirBigWorldInfo::isShuangXianArea()
{
	return ( m_uStatusFlag & REGIONSTATUSBIT_SHUANGXIAN ) != 0;
}

bool DirBigWorldInfo::isNewBigWorld()
{
	return ( m_uStatusFlag & REGIONSTATUSBIT_NEW ) != 0;
}

bool DirBigWorldInfo::isRecommendBigWorld()
{
	return ( m_uStatusFlag & REGIONSTATUSBIT_TUIJIE ) != 0;
}

unsigned int DirBigWorldInfo::getFlag()
{
	return m_uStatusFlag;
}

void DirBigWorldInfo::addWorldInfo(const CSDIRWORLDINFO &src)
{
	DirWorldInfo info;
	strncpy(info.name, src.WorldName,48);
	strncpy(info.ip, src.IP,32);
	info.Ip_Bak1	= src.IP_Bak1;
	info.Ip_Bak2	= src.IP_Bak2;
	info.recommend	= (src.StatFlag>>4)!=0;	
	info.busy		= (src.StatFlag&0xf);
	info.port		= NTOHS(src.Port);
	info.worldid	= src.WorldID;
	info.RegionID	= src.RegionID;
	strncpy(info.AuthIP, src.AuthIP,48);
	info.AuthPort	= NTOHS(src.AuthPort);
	info.authIP_Bak1= src.AuthIP_Bak1;
	info.authIP_Bak2= src.AuthIP_Bak2;
	strncpy(info.mudIp,src.MudIP,48);
	info.mudPort	= NTOHS(src.MudPort);
	info.MudIp_Bak1	= src.MudIP_Bak1;
	info.MudIp_Bak2	= src.MudIP_Bak2;
	m_Worlds.push_back(info);
}

void DirBigWorldInfo::clearWorldInfo()
{
	m_Worlds.clear();
}

void DirBigWorldInfo::addWorldInfo( const DirWorldInfo& src )
{
	m_Worlds.push_back( src );
}

void DirBigWorldInfo::updateWorldInfo( const CSDIRLATESTINFO& src )
{
	for( size_t i = 0; i < m_Worlds.size(); i++ )
	{
		if( m_Worlds[i].worldid == src.WorldID )
		{
			m_Worlds[i].recommend	= (src.StatFlag>>4)!=0;	
			m_Worlds[i].busy		= (src.StatFlag&0xf);
			strncpy(m_Worlds[i].ip, src.IP,48);
			strncpy(m_Worlds[i].mudIp, src.MudIP,48);
			m_Worlds[i].port		= NTOHS(src.Port);
			m_Worlds[i].mudPort		= NTOHS(src.MudPort);
			m_Worlds[i].RegionID	= src.RegionID;
			strncpy(m_Worlds[i].AuthIP, src.AuthIP,48);
			m_Worlds[i].AuthPort	= NTOHS(src.AuthPort);

			m_Worlds[i].Ip_Bak1		= src.IP_Bak1;
			m_Worlds[i].Ip_Bak2		= src.IP_Bak2;
			m_Worlds[i].authIP_Bak1	= src.AuthIP_Bak1;
			m_Worlds[i].authIP_Bak2	= src.AuthIP_Bak2;
			m_Worlds[i].MudIp_Bak1	= src.MudIP_Bak1;
			m_Worlds[i].MudIp_Bak2	= src.MudIP_Bak2;
			return;
		}
	}
}

std::string DirBigWorldInfo::getUrl()
{
	return m_strUrl;
}

int LoginManager::getNumBigWorld()
{
	return (int)m_BigWorlds.size();
}

DirBigWorldInfo *LoginManager::getBigWorld(int i)
{
	assert(size_t(i)<m_BigWorlds.size());

	return &m_BigWorlds[i];
}

bool LoginManager::LoadLocalSvr()
{
	LoadLastUserName();
	if( !m_vecDir.empty() )
	{
		for( size_t i = 0 ; i < m_vecDir.size() ; i++ )
		{
			m_vecDir[i].Clear();			
		}
		m_vecDir.clear();
	}

	GameConfig& cfg = GameManager::getSingleton().GetConfig();
	for( int i=0;i<cfg.dir_num;i++ )
	{
		DirConnectData data;
		data.m_DirServerNet = NULL;
		data.m_nConfigIdx = i;
		m_vecDir.push_back( data );
	}

	XMLData xmldata;
	if( !xmldata.loadFile( DIRCONFIG_PATH ) )
	{
		return false;
	}
	// 将旧的内容清空 Add by Kevin [11/24/2009]
	if (m_BigWorlds.size() != 0)
	{
		m_BigWorlds.clear();
	}

	XMLNode pRootNode = xmldata.getRootNode();

	XMLNode pChildNode = pRootNode.iterateChild();
	while( !pChildNode.isNull() )
	{
		if( stricmp( pChildNode.getName(), "BigWorld" ) == 0 )
		{
			DirBigWorldInfo pbiginfo;
			strncpy( pbiginfo.m_Name, pChildNode.attribToString("name"), 64 );
			pbiginfo.m_uStatusFlag = pChildNode.attribToInt("flag");			
			if ( pChildNode.attribToString( "url" ) == NULL )
			{
				return false;
			}
			pbiginfo.m_strUrl = pChildNode.attribToString( "url" );
			XMLNode pBigWorldChild = pChildNode.iterateChild();
			int num = pChildNode.attribToInt("count");
			for ( int i = 0; i < num; i++ )
			{
				if( stricmp( pBigWorldChild.getName(), "DirWorld" ) == 0 )
				{
					DirWorldInfo worldInfo;
					worldInfo.busy		= 0;
					worldInfo.port		= pBigWorldChild.attribToInt("port");
					worldInfo.recommend	= pBigWorldChild.attribToInt("recommend") == 0 ? false:true;
					worldInfo.worldid	= pBigWorldChild.attribToInt("worldid");
					strncpy( worldInfo.name, pBigWorldChild.attribToString("name"), WORLD_NAME_LEN );
					strncpy( worldInfo.ip, pBigWorldChild.attribToString("ip"), 32 );
					pbiginfo.addWorldInfo( worldInfo );
				}
				pBigWorldChild = pChildNode.iterateChild(pBigWorldChild);
			}
			m_BigWorlds.push_back(pbiginfo);				
		}		
		pChildNode = pRootNode.iterateChild(pChildNode);
	}

	GameManager::getSingleton().SendUIEvent( "GE_UPDATE_DIRINFO" );
	return true;
}

void LoginManager::SaveLocalSvr()
{
	TiXmlDocument doc;
	std::list< TiXmlElement* > ListElemnt;

	// 	TiXmlDeclaration *decl = new TiXmlDeclaration( "1.0", "utf-8", "yes" );
	// 	doc.LinkEndChild( decl );

	TiXmlElement* root = new TiXmlElement( "LoclDirSvr" );
	doc.LinkEndChild( root );
	ListElemnt.push_back( root );

	for( size_t i = 0; i < m_BigWorlds.size(); i++ )
	{
		TiXmlElement* pBigWorldElement = new TiXmlElement( "BigWorld");
		root->LinkEndChild(pBigWorldElement);
		pBigWorldElement->SetAttribute( "flag",		m_BigWorlds[i].getFlag() );
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

void LoginManager::initDirInfo( const CSDIRWORLDLIST &dirinfo )
{
	if (m_BigWorlds.size() != 0)
	{
		m_BigWorlds.clear();
	}
	for(int i=0; i<dirinfo.BigWorldNum; i++)
	{
		DirBigWorldInfo pbiginfo;
		const CSDIRBIGWORLD& svrBigWorldInfo = dirinfo.BigWorldList[i];
		strncpy(pbiginfo.m_Name, svrBigWorldInfo.BigWorldName,64);
		pbiginfo.m_strUrl = svrBigWorldInfo.URL;
		pbiginfo.m_uStatusFlag = svrBigWorldInfo.StatFlag;
		for(int j=0; j<dirinfo.WorldNum; j++)
		{
			const CSDIRWORLDINFO &src = dirinfo.ListInfoList[j];
			if(src.BigWorldIdx == i)
			{
				pbiginfo.addWorldInfo(src);
			}
		}
		m_BigWorlds.push_back(pbiginfo);
	}
	SaveLocalSvr();
}
bool DirBigWorldInfoCompare( const DirBigWorldInfo& lhs, const DirBigWorldInfo& rhs )
{
	return lhs.m_nDirIdx < rhs.m_nDirIdx;
}

void LoginManager::initDirInfo( const CSDIRINFO& dirinfo, int nIdx )
{
	for(int i=0; i<dirinfo.WorldList.BigWorldNum; i++)
	{
		const CSDIRBIGWORLD& svrBigWorldInfo = dirinfo.WorldList.BigWorldList[i];
		bool bFound = false;
		for( size_t j = 0 ; j < m_BigWorlds.size() ; j++ )
		{
			if( strncmp( m_BigWorlds[j].m_Name,svrBigWorldInfo.BigWorldName,64 )  == 0 )
			{
				m_BigWorlds[j].m_strUrl = svrBigWorldInfo.URL;
				m_BigWorlds[j].m_uStatusFlag = svrBigWorldInfo.StatFlag;
				m_BigWorlds[j].clearWorldInfo();
				m_BigWorlds[j].m_nDirIdx = nIdx;
				for(int idx=0; idx<dirinfo.WorldList.WorldNum; idx++)
				{
					const CSDIRWORLDINFO &src = dirinfo.WorldList.ListInfoList[idx];
					if(src.BigWorldIdx == i) m_BigWorlds[j].addWorldInfo(src);
				}
				bFound = true;
				break;
			}
		}
		if( !bFound )
		{
			DirBigWorldInfo pbiginfo;			
			strncpy(pbiginfo.m_Name, svrBigWorldInfo.BigWorldName,64);
			pbiginfo.m_strUrl = svrBigWorldInfo.URL;
			pbiginfo.m_uStatusFlag = svrBigWorldInfo.StatFlag;
			pbiginfo.m_nDirIdx = nIdx;

			for(int j=0; j<dirinfo.WorldList.WorldNum; j++)
			{
				const CSDIRWORLDINFO &src = dirinfo.WorldList.ListInfoList[j];
				if(src.BigWorldIdx == i) pbiginfo.addWorldInfo(src);
			}
			m_BigWorlds.push_back(pbiginfo);
		}
	}
	std::stable_sort( m_BigWorlds.begin(),m_BigWorlds.end(),DirBigWorldInfoCompare );
	SaveLocalSvr();
}

int SendDirNetMessageEx( NETCLTHANDLE *pnetcl,int msgid, const tagCSPkgBody &msgbody )
{
	CSPKG pkg;
	pkg.Head.Magic = PROTOCOL_MAGIC;
	pkg.Head.Cmd = msgid;
	pkg.Body = msgbody;

	if( pnetcl == NULL )
	{
		LOG_WARNING("dir_net_clt_send failed");
		return -1;
	}

	// 打印发送消息日志
#ifndef BUILD_DEPLOY
	unsigned char kbuf[256];
	GetKeyboardState(kbuf);
	if( kbuf[VK_NUMLOCK] & 1)
	{
		static char szBuf[1024*4];
		static char buf[256];
		ZeroMemory( szBuf, 1024*4 );
		ZeroMemory( buf, 256 );
		if( net_clt_sprintf( pnetcl, &pkg, szBuf, 1024*4 ) >= 0 )
		{
			SYSTEMTIME st;
			GetLocalTime( &st );
			sprintf( buf, "c->s: %d-%d-%d %d:%d:%d", 
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
			LOG_SEVEREMSG(buf);
			LOG_SEVEREMSG(szBuf);
		}
	}
#endif

	if( 0 > net_clt_send( pnetcl, &pkg, 10 ) )
	{
		LOG_WARNING("net_clt_send failed");
		return -1;
	}
	return 0;
}

void LoginManager::LoadDirFromDirSever()
{
	for( size_t i = 0 ; i < m_vecDir.size() ; i++ )
	{
		m_vecDir[i].Clear();			
	}
	GameConfig& cfg = GameManager::getSingleton().GetConfig();
	for( size_t i = 0 ; i < m_vecDir.size() ; i++ )
	{
		NetConfigData& configData = cfg.dir_iplist[m_vecDir[i].m_nConfigIdx];
		if( net_clt_han_create_mem( INetworkManager::getSingleton().getMetaBuf(),
			configData.dir_server,
			configData.dir_port, &m_vecDir[i].m_DirServerNet ) < 0 )
		{
			for( int j = 0 ; j < configData.num ; j ++ )
			{
				if( net_clt_han_create_mem( INetworkManager::getSingleton().getMetaBuf(),
					configData.dir_data[j].dir_ip,
					configData.dir_data[j].dir_port, &m_vecDir[i].m_DirServerNet ) >= 0 )
				{
					LOG_INFO("connected dir server %s:%d\n", configData.dir_data[j].dir_ip, configData.dir_data[j].dir_port );
					break;
				}
			}
		}else{
			LOG_INFO("connected dir server %s:%d\n", configData.dir_server, configData.dir_port );
		}
		if( m_vecDir[i].m_DirServerNet != NULL )
		{
			tagCSPkgBody body;
			body.DirReq.LastVersion = 0;
			SendDirNetMessageEx( m_vecDir[i].m_DirServerNet,DIR_REQ, body );
		}
	}
}

void LoginManager::tryRequestDirInfo()
{
	GameStateManager::changeGameState( GameStateManager::STATE_DIRLIST );
	if( !LoadDirFromUrl( GameManager::getSingleton().GetConfig().dir_url ) )
	{
 		LoadDirFromDirSever();
	}
}

bool LoginManager::requestDirInfo(const char *ip, unsigned short port)
{
	m_BigWorlds.clear();
	LoadLocalSvr();
	tryRequestDirInfo();
	return true;
}

bool LoginManager::dirRefurbish(const char *ip, unsigned short port)
{	
	tryRequestDirInfo();
	return true;
}

void LoginManager::dispatchDirNetCL( NETCLTHANDLE *pnetcl, int nIdx )
{
	CSPKG	pkg;
	int ret = net_clt_recv(pnetcl, &pkg, 0);
	if(ret <= 0)
	{
		return;
	}
	// 打印消息日志
#ifndef BUILD_DEPLOY
	unsigned char kbuf[256];
	GetKeyboardState(kbuf);
	if( kbuf[VK_NUMLOCK] & 1)
	{
		static char szBuf[1024*6];
		static char buf[256];
		ZeroMemory( szBuf, 1024*6 );
		ZeroMemory( buf, 256 );
		if( net_clt_sprintf( pnetcl, &pkg, szBuf, 1024*6 ) == 0 )
		{
			SYSTEMTIME st;
			GetLocalTime( &st );
			sprintf( buf, "s->c: %d-%d-%d %d:%d:%d", 
				st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
			LOG_SEVEREMSG(buf);
			LOG_SEVEREMSG(szBuf);
		}
	}
#endif
	HandleDirServerMsg( pkg.Head.Cmd, pkg.Body, nIdx );
}

void LoginManager::HandleDirServerMsg( int msgid, const tagCSPkgBody& msgbody, int nIdx )
{
 	if(DIR_RES == msgid)
 	{
 		switch( msgbody.DirRes.Result )
 		{
 		case DIR_SUCC:
 			{
 				initDirInfo( msgbody.DirRes.DirInfo, nIdx );
 				GameManager::getSingleton().SendUIEvent( "GE_UPDATE_DIRINFO" );
 				GameStateManager::changeGameState( GameStateManager::STATE_DIRLIST );
 			}
 			break;
//  		case DIR_LATEST:
//  			{
//  				updateDirInfo( msgbody.DirRes.DirInfo.DirLatest );
//  				GameManager::getSingleton().SendUIEvent( "GE_UPDATE_DIRINFO" );
//  				GameStateManager::changeGameState( GameStateManager::STATE_DIRLIST );
//  			}
//  			break;
 		case DIR_ERR:
 			{
 				LOG_WARNING("failed to get dir");
 			}
 		}
		m_vecDir[nIdx].Clear();
 	}
}

bool LoginManager::LoadDirFromUrl( const char* szUrl )
{
	HINTERNET it = InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);
	if( it == NULL )
	{
		return false;
	}
	HINTERNET it2 = InternetOpenUrl( it,szUrl,NULL,0,INTERNET_FLAG_NO_CACHE_WRITE,NULL );
	if( it2 == NULL )
	{
		InternetCloseHandle(it);
		return false;
	}
	DWORD nTotalSize = 0;
	DWORD nLength = sizeof(nTotalSize);
	if( !HttpQueryInfo(it2, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &nTotalSize,
		&nLength, NULL) )
	{
		InternetCloseHandle(it);
		InternetCloseHandle(it2);
		return false;
	}
	char* buf = new char[ nTotalSize+1 ];
	buf[nTotalSize] = '\0';
	if( !InternetReadFile(it2,buf,nTotalSize,&nLength) || nTotalSize != nLength )
	{
		delete buf;
		InternetCloseHandle(it);
		InternetCloseHandle(it2);
		return false;
	}

	CSDIRWORLDLIST* dirInfo = new CSDIRWORLDLIST;
	if( !parseXmlData( (char*)INetworkManager::getSingleton().getMetaBuf(),buf,(char*)dirInfo,sizeof(CSDIRWORLDLIST) ) )
	{
		delete buf;
		delete dirInfo;
		InternetCloseHandle(it);
		InternetCloseHandle(it2);
		return false;
	}
	initDirInfo( *dirInfo );
	GameManager::getSingleton().SendUIEvent( "GE_UPDATE_DIRINFO" );

	delete buf;
	delete dirInfo;
	InternetCloseHandle(it);
	InternetCloseHandle(it2);
	return true;
}

void LoginManager::requestUnDelRole( int nIdx)
{
	if(nIdx < 0) return;

	tagCSPkgBody body;
	body.CsUnDel.RoleID = m_Roles[nIdx].roleid;

	SendNetMessage(m_GameNetCL, ROLE_UNDEL_REQ, body);
}

const char* LoginManager::getWorldNameById( int nId )
{
	for( size_t i = 0 ; i < m_BigWorlds.size() ; i++ )
	{		
		DirBigWorldInfo& bigWorld = m_BigWorlds[i];
		for( int j = 0 ; j < bigWorld.getNumWorld() ; j++ )
		{
			if( bigWorld.getIthWorld(j).worldid == nId )
			{
				return bigWorld.getIthWorld(j).name;
			}
		}
	}
	return "";
}

const char* LoginManager::getWorldName( unsigned short RegionId, unsigned short WorldId )
{
	for( size_t i = 0 ; i < m_BigWorlds.size() ; i++ )
	{
		DirBigWorldInfo& bigWorld = m_BigWorlds[i];
		for( int j = 0 ; j < bigWorld.getNumWorld() ; j++ )
		{
			if( bigWorld.getIthWorld(j).worldid == WorldId && bigWorld.getIthWorld(j).RegionID == RegionId )
			{
				return bigWorld.getIthWorld(j).name;
			}
		}
	}
	return "";
}

int LoginManager::getTodayOnTime()
{
	struct tm curr1;
	struct tm curr2;

	time_t t1 = getServerTime().sec;
	time_t t2 = m_nLoginTime;
	//凌晨6点切换天,
	t1 += 2*3600;
	t2 += 2*3600;
	curr1 = *(gmtime(&t1));
	curr2 = *(gmtime(&t2));
	if( curr1.tm_year==curr2.tm_year && curr1.tm_yday==curr2.tm_yday )
	{
		return (int)(t1 - t2);
	}else{
		return curr1.tm_hour*3600 + curr1.tm_min*60 + curr1.tm_sec;
	}
}

void LoginManager::requestEnterLoginWithoutUserName()
{
	for( size_t i = 0 ; i < m_vecDir.size() ; i++ )
	{
		m_vecDir[i].Clear();		
	}
	GameGetAccountAllRole::getSingleton().Reset();
	GameConfig &cfg = GameManager::getSingleton().GetConfig();
	int worldid = 0;

	if( m_GameNetCL < 0 )
	{	
		INetworkManager::getSingleton().Release();
		sprintf( m_UserName,"*%s",GameManager::getSingleton().getMachineID() );
		int nRet = AUTH_ERR_NET;
		int nCurSelectArea =-1,nCurSelectWorld =-1,nSelectRegionId = -1;

		for( size_t i = 0; i < m_BigWorlds.size(); i++ )
		{
			DirBigWorldInfo& worldInfo = m_BigWorlds[i];
			for( int j = 0; j < worldInfo.getNumWorld(); j++ )
			{
				if( worldInfo.getIthWorld(j).recommend && worldInfo.getIthWorld(j).busy != CLIENT_DIR_NOT_WORK )
				{
					nCurSelectArea = (int)i;
					nCurSelectWorld = j;
					nSelectRegionId = worldInfo.getIthWorld( j ).RegionID;
					worldid = worldInfo.getIthWorld(j).worldid;

					std::string authIp(GetSelectWorldAuthIp(worldInfo.getIthWorld( j )));
					std::string ip(GetSelectWorldIp(worldInfo.getIthWorld( j )));
					m_GameNetCL = INetworkManager::getSingleton().ConnectServer( authIp.c_str(), 
						worldInfo.getIthWorld( j ).AuthPort,
						ip.c_str(), 
						worldInfo.getIthWorld( j ).port,
						m_UserName, 
						"",
						&nRet,getClientType(),AUTH_FLAG_QUICK,NULL,NULL,NULL,NULL,
						GameManager::getSingleton().getMachineID() );
					if ( nRet == AUTH_IFM_NEED_REGIST_CODE_ERROR )
 					{
 						GameManager::getSingleton().SendUIEvent("GE_NEED_REGIST_CODE");
 						return;
 					}
					if( m_GameNetCL >= 0 )
 					{
 						break;
 					}
					break;
				}
			}
			if( nCurSelectArea >=0 && nCurSelectWorld >=0 )
			{
				break;
			}
		}

		if( nCurSelectArea>=0 && nCurSelectWorld>=0 )
		{
			if(m_GameNetCL < 0 ) 
			{
				char szScript[256];
				sprintf( szScript, "LoginErrorMes(%d)", nRet );
				GameManager::getSingleton().getScriptVM()->callString( szScript );
				return;
			}
			SetLastSelectData( nCurSelectArea, nCurSelectWorld );
		}
	}
}

bool LoginManager::isQuickStart()
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if( pMain && pMain->getGMPermFlag() & UIN_FLAG_QUICK )
	{
		return true;
	}
	return false;
}

void LoginManager::requestCreateUserInGame( char* szUserName, char* szPwd, char* szName, char* szCert, char* szMobile, char* szMail )
{
	if( m_GameNetCL >= 0 && isQuickStart() )
	{
		INetworkManager::getSingleton().CreateUserInGame(szUserName,szPwd,szCert,szName,szUserName,szMobile,getClientType());
		strcpy( m_UserName,szUserName );
	}
}

void LoginManager::keepAlive()
{
	if (m_dwMainThreadId == GetCurrentThreadId())
	{
		unsigned int now = timeGetTime();
		unsigned int dtick = now - m_CurTick;
		m_CurTick = now;
		if( GameStateManager::getGameState() == GameStateManager::STATE_ROLELIST ||
			GameStateManager::getGameState() == GameStateManager::STATE_MAPLOADING ||
			GameStateManager::getGameState() == GameStateManager::STATE_INWORLD )
		{
			m_HeartBeatTick += dtick;
			if(m_HeartBeatTick >= 30000)
			{
				m_HeartBeatTick = 0;
				reqireHeartBeat();
			}
		}else if( m_bWaitPassCard )
		{
			m_HeartBeatTick += dtick;
			if(m_HeartBeatTick >= 30000)
			{
				m_HeartBeatTick = 0;
				int ret = INetworkManager::getSingleton().AuthHeartHeat();
			}
		}
	}
}

bool LoginManager::isTodayFirstLogin()
{
	return (!isSameDay( m_Roles[m_SelectRoleIndex].lastlogin ));
}

int LoginManager::getClientType()
{
	if( GameManager::getSingleton().GetConfig().clt_type > 0 )
	{
		return GameManager::getSingleton().GetConfig().clt_type;
	}
	if( Root::getSingleton().IsSmartClient() )
	{
		return CLT_MICRO;
	}
	return CLT_PC;
}
void LoginManager::PingCheckStartFunction( int nRecTime, int nLostPack,int nMax, int nMin )
{
	CPing PTest(1000);
	//const char* addr = "60.213.179.23";

	const char* addr = "www.sina.com.cn";

	SOCKADDR_IN addrDsc = {0};
	hostent* pHostent = gethostbyname(addr);
	if (pHostent == NULL)
	{
		return ;
	}
	addrDsc.sin_family = pHostent->h_addrtype;
	memcpy(&(addrDsc.sin_addr), pHostent->h_addr, pHostent->h_length);
	PTest.Ping( addrDsc, 5, 20 );
	LOG_INFO("Ping Test Starts And Time Is %d and %d ",nRecTime,PTest.nReceivedTime );
	if ( PTest.nReceivedTime < m_nPingCheck )
	{
		char url[256];
		sprintf( url,"http://ping.wanyouyl.com/getPingData.php?u=%s&f=%s&t=%s&max=%d&min=%d&avg=%d&l=%d",
			m_UserName,
			m_SelfIp,
			m_DesIp,
			nMax,
			nMin,
			nRecTime,
			nLostPack);
		GameManager::getSingleton().SendHttpRequest( url );
		LOG_INFO( "Ping Test Ends." );
		setPingCheckFlag( false );
	}
}

int LoginManager::getPingCheckValue()
{
	return m_nPingCheck;
}

bool LoginManager::getPingCheckFlag()
{
	return m_bPingTest;
}

void LoginManager::setPingCheckFlag( bool bPingTest )
{
	m_bPingTest = bPingTest;
}

DWORD WINAPI PingThreadWorkFunction( void* data )
{
	while( true )
	{
		if( GameStateManager::getGameState() == GameStateManager::STATE_INWORLD )
		{
			CPing ping(1000);
			SOCKADDR_IN addr;
			if( INetworkManager::getSingleton().getSockAddr( &addr ) )
			{
				ping.Ping( addr, 5, 20 );
				strncpy( LoginManager::getSingleton().getDesIP(),inet_ntoa(addr.sin_addr) ,48 );
			}else{
				ping.Ping( INetworkManager::getSingleton().getConnectWorldIp(), 5 , 20 );
				strncpy( LoginManager::getSingleton().getDesIP(),INetworkManager::getSingleton().getConnectWorldIp(), 48 );
			}
			//Ping异常记录
			if ( ping.nReceivedTime > LoginManager::getSingleton().getPingCheckValue() && LoginManager::getSingleton().getPingCheckFlag() )
			{
				int nRecPack = ( 5 - ping.nReceivedNum ) * 100 / 5;
				LoginManager::getSingleton().PingCheckStartFunction( ping.nReceivedTime, nRecPack,ping.m_nMaxTime, ping.m_nMinTime );
			}
			if( ping.nReceivedTime < 0 )
			{
				LoginManager::getSingleton().SetPing( 5 );
			}else{
				LoginManager::getSingleton().SetPing( ping.nReceivedTime );
			}
			::Sleep( 60*1000 );
		}
		::Sleep( 2*1000 );
	}
	return 0;
}

void LoginManager::createPingThread()
{
	DWORD threadid;
	HANDLE threadHandle = CreateThread( NULL, 0, PingThreadWorkFunction, 0, 0, &threadid );
	CloseHandle( threadHandle );
}

void LoginManager::SetPing( int nPing )
{
	m_nPing = nPing;
}

int LoginManager::GetPing()
{
	return m_nPing;
}

void LoginManager::TryGetOtherWorldData()
{
	for( size_t i = 0; i < m_BigWorlds.size(); i++ )
	{
		if( m_BigWorlds[i].getNumWorld() > 0 )
		{
			DirWorldInfo& info = m_BigWorlds[i].getIthWorld(0);
			GameGetAccountAllRole::getSingleton().AddWorldData( info.worldid, info.RegionID, GetSelectWorldAuthIp(info), info.AuthPort, m_UserName );
		}
	}
	GameGetAccountAllRole::getSingleton().StartGetRoleData();
}

bool LoginManager::QuickLoginServer( unsigned short regionId, unsigned short worldId )
{
	for( size_t i = 0 ; i < m_BigWorlds.size() ; i++ )
	{
		for( int j = 0 ; j < m_BigWorlds[i].getNumWorld() ; j++ )
		{
			m_nLastSelectWorldId = worldId;
			m_nLastSelectRegionId = regionId;
			DirWorldInfo& info = m_BigWorlds[i].getIthWorld(j);
			if( info.worldid == worldId && info.RegionID == regionId )
			{
				INetworkManager::getSingleton().Release();
				GameStateManager::changeGameState( GameStateManager::STATE_DIRLIST );
				sprintf( m_ServerName,"[%s:%s]", m_BigWorlds[i].getName(), info.name );

				int nRet = 0;
				std::string ip(GetSelectWorldIp(info));
				m_GameNetCL = INetworkManager::getSingleton().QuickLoginServer( ip.c_str(), info.port, GameManager::getSingleton().getMachineID(),getClientType(),&nRet );
				if( m_GameNetCL >= 0 && nRet == 0 )
				{
					return true;
				}
				return false;
			}
		}
	}
	return false;
}

const char* LoginManager::getPasswdCardPath( int nIdx )
{
	static char path[MAX_PATH];
	GetTempPath( MAX_PATH, path );
	GetTempFileName( path,NULL,0,path );
	sprintf( path, "%s.png",path );
	INetworkManager::getSingleton().SaveTempImage( nIdx, path );
	return path;
}

void LoginManager::SetPasswdCardInfo( int nIdx, const char* szData )
{
	m_stPasswdCardInfo.Num = 3;
	m_stPasswdCardInfo.Data[nIdx].CellLen = (int)strlen(szData);
	strcpy(m_stPasswdCardInfo.Data[nIdx].Cell,szData);
}

void LoginManager::sendFashionChangeMessage(unsigned char head, unsigned char face, unsigned char hair)
{
	tagCSPkgBody msgbody;
	memset(&msgbody,0,sizeof(tagCSPkgBody));
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_SCULPT_CHG_CLT;
	msgbody.PlayerMiscClt.PlayerMiscCltData.SculptChgClt.Head = head;
	msgbody.PlayerMiscClt.PlayerMiscCltData.SculptChgClt.Face = face;
	msgbody.PlayerMiscClt.PlayerMiscCltData.SculptChgClt.Hair = hair;
	SendGameServerMsg(PLAYER_MISC_CLT, msgbody);
}

void LoginManager::sendNewPeopleRequest()
{
	tagCSPkgBody msgbody;
	memset(&msgbody,0,sizeof(tagCSPkgBody));
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_NEW_PEOPLE_CLT;
	SendGameServerMsg(PLAYER_MISC_CLT, msgbody);
}

void LoginManager::clearNewPeopleInfo()
{
	//memset(&m_NewPeopleInfo, 0, sizeof(tagCSNewPeopleInfo));
	//do something lazy
	m_NewPeopleInfo.Num = 0;
}

bool LoginManager::requestAuthPasswdCard()
{
	int nRet = 0;
	m_GameNetCL = INetworkManager::getSingleton().AuthPasswdCard( 
							GameManager::getSingleton().getMachineID(),
							getClientType(), &m_stPasswdCardInfo,&nRet );
	// 需要密保卡
	if( nRet == AUTH_REASON_NEED_PASSWD_CARD_ERROR )
	{
		m_bWaitPassCard = true;
		memset( &m_stPasswdCardInfo, 0, sizeof(PASSWDCARDAUTHINFO) );
		GameManager::getSingleton().SendUIEvent( "GE_NEED_PASSWD_CARD" );
		return false;
	}
	if ( m_GameNetCL < 0) 
	{
		char szScript[256];
		sprintf( szScript, "LoginErrorMes(%d)", nRet );
		GameManager::getSingleton().getScriptVM()->callString( szScript );
		return false;
	}

	//SetLastSelectData( m_nLastSelectRegionId, m_nLastSelectWorldId );
	return true;
}

void LoginManager::SetLastSelectData( int RegionId, int WorldId )
{
	if( RegionId >= 0 && RegionId < (int)m_BigWorlds.size() )
	{
		DirBigWorldInfo& BigWorld = m_BigWorlds[RegionId];
		if(WorldId >= BigWorld.getNumWorld())
		{
			return;
		}
		DirWorldInfo& world = BigWorld.getIthWorld( WorldId);
		m_nLastSelectRegionId = world.RegionID;
		m_nLastSelectWorldId = world.worldid;
		sprintf( m_ServerName,"[%s:%s]", BigWorld.getName(), world.name );
		sprintf(m_szLastLoginIp,world.mudIp);
		m_strLastWorldName = world.name;
		m_uLastLoginPort	= world.port;
		m_uLastLoginMudPort = world.mudPort;
		saveLastUserData();
	}
}

const char* LoginManager::GetSelectWorldIp( const DirWorldInfo& world )
{
	if( GetIpMode() == 1 && !(world.Ip_Bak1 == 0 || world.Ip_Bak1 == -1) > 0 )
	{
		return inet_ntoa(*(struct in_addr *)&world.Ip_Bak1 );
	}
	if( GetIpMode() == 2 && !(world.Ip_Bak2 == 0 || world.Ip_Bak2 == -1) > 0 )
	{
		return inet_ntoa(*(struct in_addr *)&world.Ip_Bak2);
	}
	return world.ip;
}

const char* LoginManager::GetSelectWorldAuthIp( const DirWorldInfo& world )
{
	if( GetIpMode() == 1 && !(world.authIP_Bak1 == 0 || world.authIP_Bak1 == -1) > 0 )
	{
		return inet_ntoa(*(struct in_addr *)&world.authIP_Bak1);
	}
	if( GetIpMode() == 2 && !(world.authIP_Bak2 == 0 || world.authIP_Bak2 == -1) > 0 )
	{
		return inet_ntoa(*(struct in_addr *)&world.authIP_Bak2);
	}
	return world.AuthIP;
}

int LoginManager::GetIpMode()
{
	return m_nIpMode;
}

void LoginManager::SetIpMode( int nMode )
{
	m_nIpMode = nMode;
	saveLastUserData();
}

void LoginManager::requestGetSMS( const char* szPhone )
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_PRECREATE_GIFT_VERIFY;
	msgbody.PlayerMiscClt.PlayerMiscCltData.PrecreateGiftVerify.OpType = PRECREATE_GIFT_VERIFY_OP_GET_SMS;
	strncpy( msgbody.PlayerMiscClt.PlayerMiscCltData.PrecreateGiftVerify.Data.GetSmsMobile, szPhone, NAME_LEN );
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}

void LoginManager::requsetVerifySMS( int nCode )
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_PRECREATE_GIFT_VERIFY;
	msgbody.PlayerMiscClt.PlayerMiscCltData.PrecreateGiftVerify.OpType = PRECREATE_GIFT_VERIFY_OP_VERIFY;
	msgbody.PlayerMiscClt.PlayerMiscCltData.PrecreateGiftVerify.Data.Verify = nCode;
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}
void LoginManager::requestShortCutSkillClear()
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_SHORTCUT_SKILL_CLEAR;
	msgbody.PlayerMiscClt.PlayerMiscCltData.ClearShortCutSkill.res = 1;
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}