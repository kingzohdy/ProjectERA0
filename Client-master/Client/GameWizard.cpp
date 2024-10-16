#include "GameWizard.h"
#include "GameClientOpt.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include "GameActor.h"

#include "GameMainPlayer.h"
#include "GameActorManager.h"
#include "GameSkill_Mgr.h"
#include "GameSkill_Control.h"
#include "GameShortCut.h"
#include "OgreLog.h"
#include "GameDBData.h"
#include "GameClanManager.h"
#include "GamePlayManager.h"
#include "ShowActor.h"
#include "GameCamera.h"
#include "OgreCamera.h"
#include "GameMap.h"
#include "ShowGameMap.h"
#include "ServerTimer.h"

#include "../UILib/ui_framemgr.h"

enum
{
	FIRST_PETID			= 20000,
	FIRST_PETSKILL		= 6001,
	FIRST_SHORCUTINDEX	= 12,
	FIRST_PETEGGID		= 5620000
};

GameWizard::GameWizard(void)
{
	GameClientOpt::getSingleton().setLevelSize( SAVE_OPT_DIRECT, sizeof(m_szStep) );
	INetworkManager::getSingleton().RegisterMsgHandler( GIFT_SVR, this );
	INetworkManager::getSingleton().RegisterMsgHandler( CONTI_GIFT_SVR, this );
	INetworkManager::getSingleton().RegisterMsgHandler( ACCGIFT_SVR,this );
	INetworkManager::getSingleton().RegisterMsgHandler( PROPS_SVR,this );
	INetworkManager::getSingleton().RegisterMsgHandler( ONLINEGIFT_GROUP_SVR,this );
	INetworkManager::getSingleton().RegisterMsgHandler( CDKEY_SVR,this );
	INetworkManager::getSingleton().RegisterMsgHandler( LTRINFO_SVR,this );
	//DBDataManager::getSingleton().loadOneDayGiftDef( m_vectOneDayGift );
	DBDataManager::getSingleton().loadLVGiftDef( m_vectLVGift );
	memset(&m_CountOnlineInfo,0,sizeof(tagMiscContOnlineInfo));
	memset( &m_RoleGiftInfo, 0, sizeof( tagRoleGiftInfo ) );
	memset( &m_AccGiftInfo,0,sizeof( tagAccGiftInfo ));
	memset( &m_RoleLtrInfo,0,sizeof(tagRoleLtrInfo));
	LoadOneDayGift();
	m_PreCreateGift = 0;
	m_bShowPreCreate = false;
	m_nLastRequestTime = 0;
	m_UserType = 0;
	m_pDirEntity = NULL;
}

GameWizard::~GameWizard(void)
{
	OGRE_RELEASE( m_pDirEntity );
}

void GameWizard::LoadOneDayGift()
{
	int nNum = DBDataManager::getSingleton().getOneDayGiftNum();
	for( int i = 0 ; i < nNum ; i++ )
	{
		tagOneDayGift* pGift = DBDataManager::getSingleton().getOneDayGift(i);
		if( pGift != NULL )
		{
			if( (pGift->GiftCond.UserType & USER_TYPE_INTERNET_BAR ) )
			{
				m_vVipBarGift.push_back( pGift );
			}else if( pGift->GiftCond.UserType & USER_TYPE_CLAN )
			{
				m_vectClanGift.push_back( pGift );
			}else{
				m_vectOneDayGift.push_back( pGift );
			}
		}
	}
}

// 向服务器领取奖励请求
void GameWizard::sendAwardRequest( int nAwardID )
{
	tagCSPkgBody msgbody;
	msgbody.OnlineAwardClt.AwardItemID	= nAwardID;
	SendGameServerMsg( ONLINE_AWARD_CLT, msgbody ); 
}

// 向服务器发送领取礼包请求
void GameWizard::sendGiftRequest( int nLv, int nGiftId, int nType )
{
	if( GameManager::getSingleton().getTickTime() - m_nLastRequestTime < 500 )
	{
		return;
	}
	m_nLastRequestTime = GameManager::getSingleton().getTickTime();
	tagCSPkgBody msgbody;
	memset( &msgbody,0,sizeof(tagCSPkgBody) );
	msgbody.GiftClt.GiftType        = nType;
	if( nType == GIFT_TYPE_LV )
	{
		msgbody.GiftClt.CltData.LV      = nLv;
	}else if( nType == GIFT_TYPE_ONEDAY ){
		msgbody.GiftClt.CltData.GiftID  = nGiftId;
	}else if( nType == GIFT_TYPE_CONTI )
	{
		msgbody.GiftClt.CltData.ContGiftID = nGiftId;
	}else if( nType == GIFT_TYPE_ACCGIFT )
	{
		msgbody.GiftClt.CltData.AccGiftID = nGiftId;
	}else if( nType == GIFT_TYPE_PROPGIFT )
	{
		//msgbody.GiftClt.CltData.PropGiftID = nGiftId;
	}else if( nType == GIFT_TYPE_CONTI_GROUP )
	{
		msgbody.GiftClt.CltData.ContiGroup.ID = nLv;
		msgbody.GiftClt.CltData.ContiGroup.Day = nGiftId;
	}
	SendGameServerMsg( GIFT_CLT, msgbody );
}

void GameWizard::sendCDKeyGiftRequest( const char* szCDKey )
{
	if( GameManager::getSingleton().getTickTime() - m_nLastRequestTime < 500 )
	{
		return;
	}
	m_nLastRequestTime = GameManager::getSingleton().getTickTime();
	tagCSPkgBody msgbody;
	memset( &msgbody,0,sizeof(tagCSPkgBody) );
	msgbody.GiftClt.GiftType = GIFT_TYPE_CDKEY;
	strncpy( msgbody.GiftClt.CltData.CDkey,szCDKey,CDKEY_LEN );
	SendGameServerMsg( GIFT_CLT,msgbody );
}

// 处理服务器发回的消息
int GameWizard::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if ( msgid == GIFT_SVR )
	{
		memset( &m_RoleGiftInfo, 0, sizeof( tagRoleGiftInfo ) );
		m_RoleGiftInfo = msgbody.GiftSvr.GiftInfo;
		m_PreCreateGift = msgbody.GiftSvr.PreCreateGift;
		if( m_PreCreateGift == ROLE_PRE_FLAG )
		{
			m_bShowPreCreate = true;
		}
		m_UserType = msgbody.GiftSvr.UserType;
	}else if( msgid == CONTI_GIFT_SVR )
	{
		memset(&m_CountOnlineInfo,0,sizeof(tagMiscContOnlineInfo));
		m_CountOnlineInfo = msgbody.OnlineGiftSvr.ContOnlineInfo;
	}else if( msgid == ACCGIFT_SVR )
	{
		if( msgbody.AccGiftSvr.Type == SVR_ACC_GIFT_TYPE_ALL )
		{
			m_AccGiftInfo = msgbody.AccGiftSvr.AccGiftData.AccGiftInfo;
		}else if( msgbody.AccGiftSvr.Type == SVR_ACC_GIFT_TYPE_ONE )
		{
			for( int i = 0; i < m_AccGiftInfo.Count ; i++ )
			{
				if( msgbody.AccGiftSvr.AccGiftData.AccGiftRes.ID == m_AccGiftInfo.AccGifts[i].ID )
				{
					m_AccGiftInfo.AccGifts[i].Flag = ACCGIFT_CANNT_GET_TOLUA;
					break;
				}
			}
		}		
	}
	else if( msgid == PROPS_SVR )
	{
		if( msgbody.PropGiftSvr.Type == SVR_PROP_GIFT_TYPE_ALL )
		{
			m_PropGiftInfo = msgbody.PropGiftSvr.PropGiftData.GiftInfo;
		}else if( msgbody.PropGiftSvr.Type == SVR_PROP_GIFT_TYPE_ONE )
		{
			for( int i = 0 ; i < m_PropGiftInfo.Count; i++ )
			{
				if( msgbody.PropGiftSvr.PropGiftData.GiftRes.GroupID == m_PropGiftInfo.PropsGift[i].GroupID )
				{
					m_PropGiftInfo.PropsGift[i].GiftGetNum = msgbody.PropGiftSvr.PropGiftData.GiftRes.GiftGetNum;
					for( int j = 0 ; j < m_PropGiftInfo.PropsGift[i].GiftGetNum; j++ )
					{
						m_PropGiftInfo.PropsGift[i].GiftGetInfo[j].CashUp = msgbody.PropGiftSvr.PropGiftData.GiftRes.GiftGetInfo[j].CashUp;
					}
				}
			}		
		}else if( msgbody.PropGiftSvr.Type == SVR_PROP_GIFT_TYPE_ADD )
		{
			bool bFound = false;
			for( int i = 0 ; i < m_PropGiftInfo.Count; i++ )
			{
				if( msgbody.PropGiftSvr.PropGiftData.PropsGiftAddCash.GroupID == m_PropGiftInfo.PropsGift[i].GroupID )
				{
					m_PropGiftInfo.PropsGift[i].CashNow = m_PropGiftInfo.PropsGift[i].CashNow + msgbody.PropGiftSvr.PropGiftData.PropsGiftAddCash.AddCash;
					bFound = true;
				}
			}
			if ( !bFound )
			{
				m_PropGiftInfo.PropsGift[m_PropGiftInfo.Count].GroupID = msgbody.PropGiftSvr.PropGiftData.PropsGiftAddCash.GroupID;
				m_PropGiftInfo.PropsGift[m_PropGiftInfo.Count].CashNow = msgbody.PropGiftSvr.PropGiftData.PropsGiftAddCash.AddCash;
				m_PropGiftInfo.PropsGift[m_PropGiftInfo.Count].GiftGetNum = 0;
				m_PropGiftInfo.Count = m_PropGiftInfo.Count + 1;
			}
		}else if ( msgbody.PropGiftSvr.Type == SVR_PROP_GIFT_TYPE_NOTICE )
		{
			for( int i = 0 ; i < m_PropGiftInfo.Count; i++ )
			{
				if( msgbody.PropGiftSvr.PropGiftData.GiftNotice.GroupID == m_PropGiftInfo.PropsGift[i].GroupID )
				{
					m_PropGiftInfo.PropsGift[i].GiftGetNum = msgbody.PropGiftSvr.PropGiftData.GiftNotice.GiftGetNum;
					for( int j = 0 ; j < m_PropGiftInfo.PropsGift[i].GiftGetNum; j++ )
					{
						m_PropGiftInfo.PropsGift[i].GiftGetInfo[j].CashUp = msgbody.PropGiftSvr.PropGiftData.GiftNotice.GiftGetInfo[j].CashUp;
					}
				}
			}
		}
	}else if( msgid == ONLINEGIFT_GROUP_SVR )
	{
		if( msgbody.OnlineGiftGroupSvr.Type == SVR_GIFT_GROUP_TYPE_ONE )
		{
			tagContiGroup group;
			bool bFound = false;
			for( size_t i = 0 ; i < m_vContiGroup.size(); i++ )
			{
				if( m_vContiGroup[i].ID == msgbody.OnlineGiftGroupSvr.GiftGroupData.GiftGroupOne.ID)
				{
					m_vContiGroup[i].Day = msgbody.OnlineGiftGroupSvr.GiftGroupData.GiftGroupOne.Day;
					bFound = true;
					break;
				}
			}
			if( !bFound )
			{
				group.ID = msgbody.OnlineGiftGroupSvr.GiftGroupData.GiftGroupOne.ID;
				group.Day = msgbody.OnlineGiftGroupSvr.GiftGroupData.GiftGroupOne.Day;
				m_vContiGroup.push_back( group );
			}
		}else if( msgbody.OnlineGiftGroupSvr.Type == SVR_GIFT_GROUP_TYPE_RES )
		{
			std::vector<tagContiGroup>::iterator it = m_vContiGroup.begin();
			for( ; it != m_vContiGroup.end() ; ++it )
			{
				if( it->ID == msgbody.OnlineGiftGroupSvr.GiftGroupData.GiftGroupOne.ID
					&& it->Day == msgbody.OnlineGiftGroupSvr.GiftGroupData.GiftGroupOne.Day )
				{
					m_vContiGroup.erase( it );
					break;
				}
			}
		}
	}else if( msgid == CDKEY_SVR )
	{
		char buf[256];
		sprintf( buf,"GetCDKeyResponse( %d )",msgbody.CDKeySvr.Succ );
		GameManager::getSingleton().getScriptVM()->callString( buf );
		return 0;
	}else if( msgid == LTRINFO_SVR )
	{
		m_RoleLtrInfo = msgbody.LtrInfoSvr.RoleLtrData;
	}
	GameManager::getSingleton().SendUIEvent("GE_GIFT_UPDATE");
	return 0;	
}

void GameWizard::useContainerItem( int itemId, int listType, int GridIdx )
{
	char tmp[256];
	sprintf( tmp, "SpringUseContainerItem(%d,%d,%d)", itemId, listType, GridIdx );
	GameManager::getSingleton().getScriptVM()->callString( tmp );
}

// 对象进入视野
void GameWizard::objEnterView(GameActor *actor)
{
	springByObjectEnter(actor->getType(), actor->getResId(), actor->GetID(), 100);
}

// 对象进入视野
void GameWizard::objLeaveView(GameActor *actor)
{
	char szPopName[256];
	if( actor->getType() == GAT_NPC || actor->getType() == GAT_MONSTER )
	{
		sprintf( szPopName, "%d", actor->GetID() );
	}else{
		sprintf( szPopName, "%s", actor->getName() );
	}
	g_pFrameMgr->hidePopWin( szPopName );
}

// 更新任务NPC
void GameWizard::updateQuestNpc( int nQuestState, int nQuestId, int nRecycleID )
{
	if( nQuestState == TASK_STAT_RUN )
	{
		GameWizard::getSingleton().springByGetQuest( nQuestId, nRecycleID );
	}
	else if( nQuestState == TASK_STAT_FINI )
	{
		GameWizard::getSingleton().springByFinishQuest( nQuestId );
	}
	else if( nQuestState == TASK_STAT_COMMIT )
	{
		GameWizard::getSingleton().springByQuestCommit( nQuestId );
	}
	else if( nQuestState == TASK_STAT_DROP )
	{
		GameWizard::getSingleton().springByQuestDrop( nQuestId );
	}
}
// 任务完成触发
void GameWizard::springByFinishQuest( int nQuestId )
{
	char tmp[256];
	sprintf( tmp, "SpringByFinishQuest(%d)", nQuestId );
	GameManager::getSingleton().getScriptVM()->callString( tmp );
}

// 任务获取触发
void GameWizard::springByGetQuest( int nQuestId, int nRecycleID )
{
	char tmp[256];
	sprintf( tmp, "SpringByGetQuest(%d,%d)", nQuestId, nRecycleID );
	GameManager::getSingleton().getScriptVM()->callString( tmp );
}

// 任务提交触发
void GameWizard::springByQuestCommit( int nQuestId )
{
	char tmp[256];
	sprintf( tmp, "SpringByQuestCommit(%d)", nQuestId );
	GameManager::getSingleton().getScriptVM()->callString( tmp );
}

// 任务丢弃触发
void GameWizard::springByQuestDrop( int nQuestId )
{
	char tmp[256];
	sprintf( tmp, "SpringByQuestDrop(%d)", nQuestId );
	GameManager::getSingleton().getScriptVM()->callString( tmp );
}

// 升级触发
void GameWizard::springByLevelUp( int nLv )
{
	char tmp[256];
	sprintf( tmp, "SpringByLevelUp(%d)", nLv );
	GameManager::getSingleton().getScriptVM()->callString( tmp );
}

// 对象进入视野触发
void GameWizard::springByObjectEnter( int objType, int objResId, int actorId, int dis )
{
	char tmp[256];
	sprintf( tmp, "SpringByObjectEnter(%d,%d,%d,%d)", objType, objResId, actorId, dis );
	GameManager::getSingleton().getScriptVM()->callString( tmp );
}

// 进入场景触发
void GameWizard::springByEnterMap( int mapId )
{
	char tmp[256];
	sprintf( tmp, "SpringByEnterMap(%d)", mapId );
	GameManager::getSingleton().getScriptVM()->callString( tmp );
}

// 学习技能触发
void GameWizard::springByStudySkill( int skillId )
{
	char tmp[256];
	sprintf( tmp, "SpringByStudySkill(%d)", skillId );
	GameManager::getSingleton().getScriptVM()->callString( tmp );
}

// 获取服务器上配置信息
void GameWizard::setSetp( const char* szStep )
{
	memcpy( m_szStep, szStep, sizeof(m_szStep) );
}

// 获取配置信息
char* GameWizard::getSetp()
{
	return m_szStep;
}

// 清空配置信息
void GameWizard::zeroSetp()
{
	memset( m_szStep, 0, sizeof(m_szStep) );
}

// 处理步骤
void GameWizard::doStep( int nStep )
{
	if( !isDo( nStep ) )
	{
		int nIndex		= nStep/1000;
		int nIndexStep	= nStep%1000;
		m_szStep[nIndex] |= nIndexStep;

		GameClientOpt::getSingleton().setClientOpt( SAVE_OPT_DIRECT, m_szStep );
	}
}

void GameWizard::undoStep( int nStep )
{
	if( isDo(nStep) )
	{
		int nIndex		= nStep/1000;
		int nIndexStep	= nStep%1000;
		m_szStep[nIndex] ^= nIndexStep;

		GameClientOpt::getSingleton().setClientOpt( SAVE_OPT_DIRECT, m_szStep );
	}
}

// 步骤是否已经处理过
bool GameWizard::isDo( int nStep )
{
	int nIndex		= nStep/1000;
	int nIndexStep	= nStep%1000;
	bool bResult = (m_szStep[nIndex] & nIndexStep) == 0 ?false:true;
	return (m_szStep[nIndex] & nIndexStep) == 0 ?false:true;
}

// 关闭NPC对话泡泡
void GameWizard::hideNpcTalkByResId( int nResId )
{
	GameActor* pActor = ActorManager::getSingleton().FindActor( ActorManager::getSingleton().getActorIdByResId( nResId ) );
	if( pActor != NULL )
	{
		delNpcTalkByActorId( pActor->GetID() );
	}
}
// 显示NPC对话泡泡
void GameWizard::showNpcTalkByResId( int id )
{
	GameActor* pActor = ActorManager::getSingleton().FindActor( ActorManager::getSingleton().getActorIdByResId(id) );
	if( pActor != NULL )
	{
		char szPopName[256];
		sprintf( szPopName, "%d", pActor->GetID() );
		g_pFrameMgr->showPopWin( szPopName );
	}
}

// 删除NPC对话泡泡
void GameWizard::delNpcTalkByActorId( int id )
{
	char szPopName[256];
	sprintf( szPopName, "%d", id );
	g_pFrameMgr->delPopWindow( szPopName ); 
}

// 设置NPC对话
void GameWizard::setNpcTalkByActorId( int nId, const char* szTalk, const char* szDrawBack, 
									 bool bShowOnce, int nOffsetX, int nOffsetY, const char* szTexPath, 
									 bool bShowTex, int nTexOffsetX, int nTexOffsetY, int nDoStep, int nDuration )
{
	GameActor* pActor = ActorManager::getSingleton().FindActor( nId );
	if( pActor != NULL )
	{
		if ( bShowOnce && isDo( nDoStep ) )
		{
			return;
		}
		char szPopName[256];
		sprintf( szPopName, "%d", pActor->GetID() );
		g_pFrameMgr->addPopWindow(	szDrawBack, szPopName, szTalk, nOffsetX, nOffsetY, nDuration, szTexPath, 
			bShowTex, nTexOffsetX, nTexOffsetY );
		pActor->addPopRich( szPopName );
		g_pFrameMgr->updatePopWinPos(szPopName,600,-400);
	}
}

// 删除怪物的对话泡泡
void GameWizard::delActorTalkByActorId( int nActorId )
{
	GameActor* pActor = ActorManager::getSingleton().FindActor( nActorId );
	if ( pActor != NULL )
	{
		delActorTalkByActorId( pActor );
	}
}

// 删除怪物的对话泡泡
void GameWizard::delActorTalkByActorId( GameActor* pActor )
{
	if ( pActor != NULL )
	{
		char buffer[256];
		sprintf( buffer, "%s%d",  pActor->getName(), pActor->GetID() );
		g_pFrameMgr->delPopWindow(	buffer );
	}
}

// 设置怪物的对话泡泡
void GameWizard::setActorTalkByActorId( int nActorId, const char* szTalk, const char* szDrawBack, bool bShowOnce, 
									   int nOffsetX, int nOffsetY, const char* szTexPath, bool bShowTex, 
									   int nTexOffsetX, int nTexOffsetY, int nDoStep, int nDuration )
{
	GameActor* pActor = ActorManager::getSingleton().FindActor( nActorId );
	if ( pActor != NULL )
	{
		setActorTalkByActorId( pActor, szTalk, szDrawBack, bShowOnce, nOffsetX, nOffsetY, szTexPath, bShowTex, 
			nTexOffsetX, nTexOffsetY, nDoStep, nDuration );
	}
}

// 设置怪物的对话泡泡
void GameWizard::setActorTalkByActorId( GameActor* pActor, const char* szTalk, const char* szDrawBack, bool bShowOnce, 
									   int nOffsetX, int nOffsetY, const char* szTexPath, bool bShowTex, int nTexOffsetX, 
									   int nTexOffsetY, int nDoStep, int nDuration )
{
	if ( bShowOnce && isDo( nDoStep ) )
	{
		return;
	}
	char szPopName[256];
	if ( pActor->getType() == GAT_NPC )
	{
		sprintf( szPopName, "%d",pActor->GetID() );
	}
	else
	{
		sprintf( szPopName, "%s%d",  pActor->getName(), pActor->GetID() );
	}

	g_pFrameMgr->addPopWindow(	szDrawBack, szPopName, szTalk, nOffsetX, nOffsetY, nDuration, szTexPath, 
		bShowTex, nTexOffsetX, nTexOffsetY );
	pActor->addPopRich( szPopName );
}

// 设置某些类型怪物对话泡泡
void GameWizard::setGroupActorsByResId( int nResId,const char* szTalk, const char* szDrawBack, bool bShowOnce, 
									   int nOffsetX, int nOffsetY, const char* szTexPath, bool bShowTex, 
									   int nTexOffsetX, int nTexOffsetY, int nDoStep, int nDuration  )
{
	for( ACTOR_ITER iter  = ActorManager::getSingleton().getActorBegin(), end = ActorManager::getSingleton().getActorEnd(); 
		iter != end; ++iter )
	{
		GameActor* pActor = iter->second;
		if ( pActor->getResId() == nResId )
		{
			setActorTalkByActorId( pActor, szTalk, szDrawBack, bShowOnce, nOffsetX, nOffsetY, szTexPath, bShowTex, 
				nTexOffsetX, nTexOffsetY, nDoStep, nDuration );
		}
	}
}

// 删除某些类型怪物对话泡泡
void GameWizard::delGroupActorsByResId( int nResId )
{
	for( ACTOR_ITER iter  = ActorManager::getSingleton().getActorBegin(), end = ActorManager::getSingleton().getActorEnd(); 
		iter != end; ++iter )
	{
		GameActor* pActor = iter->second;
		if ( pActor->getResId() == nResId )
		{
			delActorTalkByActorId( pActor );
		}
	}
}

// 删除所有掉落包裹
void GameWizard::delAllDropItemPop( )
{
	for( ACTOR_ITER iter  = ActorManager::getSingleton().getActorBegin(), end = ActorManager::getSingleton().getActorEnd(); 
		iter != end; ++iter )
	{
		GameActor* pActor = iter->second;
		if ( pActor->getType() == GAT_ITEM  )
		{
			const tagItemDef& itemDef = DBDataManager::getSingleton().getItemDef( pActor->getResId() );
			if ( itemDef.ItemTmp.Type == ITEM_TMP_NO )
			{
				delActorTalkByActorId( pActor );
			}
		}
	}
}

// 设置飞龙对话泡泡
void GameWizard::setFlyMonsterTalk( int nActorId, const char* szTalk, 
								   const char* szDrawBack, const char* szTexPath )
{
	GameActor* pActor			= ActorManager::getSingleton().FindActor( nActorId );
	MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pActor != NULL && pActor == pMainPlayer && pMainPlayer->getRider()->getHorseId() != 0 )
	{
		char szFlyActorName[256];
		const tagNpcDisplayInfo& npcDisplayInfo = DBDataManager::getSingleton().GetNpcDisplayInfo( pMainPlayer->getRider()->getHorseId() );
		sprintf( szFlyActorName, "%s%s",pActor->getName(), npcDisplayInfo.name );
		g_pFrameMgr->addPopWindow(	szDrawBack, szFlyActorName, szTalk, -50, 270, 10, szTexPath, true, 0, 0 );
		pActor->addPopRich( szFlyActorName );
	}
}

// 获得每日奖励信息
tagOneDayGift_ToLua* GameWizard::getOneDayGiftDef( int nIndex )
{
	static_assert( sizeof( tagOneDayGift_ToLua ) == sizeof( tagOneDayGift ) );
	assert( nIndex < (int)m_vectOneDayGift.size() || nIndex >= 0 );
	return (tagOneDayGift_ToLua*)m_vectOneDayGift[nIndex];
}

// 获得等级奖励信息
tagLVGift_ToLua* GameWizard::getLVGiftDef( int nIndex )
{
	static_assert( sizeof( tagLVGift_ToLua ) == sizeof( tagLVGift ) );
	assert( nIndex < (int)m_vectLVGift.size() || nIndex >= 0 );
	return (tagLVGift_ToLua*)m_vectLVGift[nIndex];    
}

// 获得玩家已领取奖励信息
tagRoleGiftInfo_ToLua* GameWizard::getRoleGiftInfo()
{ 
	static_assert( sizeof( tagRoleGiftInfo_ToLua ) == sizeof( tagRoleGiftInfo ) );
	return (tagRoleGiftInfo_ToLua*)&m_RoleGiftInfo; 
}

tagMiscContOnlineInfo_ToLua* GameWizard::getContOnlineInfo()
{
	static_assert(sizeof(tagMiscContOnlineInfo_ToLua) == sizeof(tagMiscContOnlineInfo) );
	return (tagMiscContOnlineInfo_ToLua*)&m_CountOnlineInfo;
}
// 获得剩余的等待时间
int GameWizard::getLeavingTime( int nTotalTime )
{
	int nOneDayOnLineTm = getRoleGiftInfo()->OneDayOnlineTM;
	int sec, usec;
	ServerTimer::getSingleton().getServerTime( sec, usec ); 
	int nIntervalTm = sec - getRoleGiftInfo()->OnlineTMUpTM;
	int nLeavingTm = nTotalTime - nIntervalTm - nOneDayOnLineTm; 
	return ( nLeavingTm < 0 ? 0 : nLeavingTm );
}

// 能否领取奖励
bool GameWizard::canGetAwardGift()
{
	//     std::vector<tagOneDayGift*>::iterator iter = m_vectOneDayGift.begin();
	//     while( iter != m_vectOneDayGift.end() )
	//     {        
	//         if( (*iter)->GiftType == CLIENT_GIFT_TYPE_LV 
	//              && (*iter)->GiftCond.OnlineTM != 0 
	//              && getLeavingTime( (*iter)->GiftCond.OnlineTM ) == 0)
	//         {
	//             return true;
	//         }
	//         iter++;
	//     }
	return false;
}

bool GameWizard::isToday( int nTime )
{
	int sec, usec;
	ServerTimer::getSingleton().getServerTime( sec, usec );
	time_t t = (time_t)sec;
	struct tm *curtime = localtime( &t );
	int nSerTmDay	= curtime->tm_mday;
	int nSerTmMon	= curtime->tm_mon;
	int nSerTmYear	= curtime->tm_year;

	time_t t1 = (time_t)nTime;
	struct tm* time = localtime( &t1 );

	if( time->tm_year == nSerTmYear && time->tm_mon == nSerTmMon && time->tm_wday == nSerTmDay )
	{
		return true;
	}
	return false;
}

const tagContOnlineGiftDef_ToLua* GameWizard::getContOnlineGiftDef( int nID )
{
	return (const tagContOnlineGiftDef_ToLua*)(DBDataManager::getSingleton().getContOnlineGiftDef(nID));
}
tagAccGiftInfo_ToLua* GameWizard::getAccGiftInfo()
{
	static_assert( sizeof(tagAccGiftInfo_ToLua) == sizeof(tagAccGiftInfo) );
	return (tagAccGiftInfo_ToLua*)&m_AccGiftInfo;
}

void GameWizard::InitGiftData()
{
	memset(&m_CountOnlineInfo,0,sizeof(tagMiscContOnlineInfo));
	memset( &m_RoleGiftInfo, 0, sizeof( tagRoleGiftInfo ) );
	memset( &m_AccGiftInfo,0,sizeof( tagAccGiftInfo ));
	memset( &m_PropGiftInfo,0,sizeof( tagPropGiftInfo_ToLua ));
	memset( &m_RoleLtrInfo,0,sizeof(tagRoleLtrInfo));
	m_PreCreateGift = 0;
	m_bShowPreCreate = false;
	m_vContiGroup.clear();
	m_UserType = 0;
}

tagPropGiftInfo_ToLua* GameWizard::getPropGiftInfo()
{
	static_assert( sizeof(tagPropGiftInfo_ToLua) == sizeof(tagPropGiftInfo) );
	return (tagPropGiftInfo_ToLua*)(&m_PropGiftInfo);
}

int GameWizard::getContiNum()
{
	return (int)m_vContiGroup.size();
}

tagContiGroup_ToLua* GameWizard::getContiData( int nIdx )
{
	if( nIdx < getContiNum() )
	{
		return(tagContiGroup_ToLua*)&m_vContiGroup[nIdx];
	}
	return NULL;
}

const tagOnlineGiftGroup_ToLua* GameWizard::getOnlineGiftGroupDef( int nID )
{
	return (const tagOnlineGiftGroup_ToLua*)DBDataManager::getSingleton().getOnlineGiftGroup( nID );
}

tagGiftOne_ToLua* GameWizard::getGiftInfo( int nID,int nDay )
{
	const tagOnlineGiftGroup* pGiftGroupDef = DBDataManager::getSingleton().getOnlineGiftGroup( nID );
	if( pGiftGroupDef != NULL )
	{
		for( int i = 0 ; i < pGiftGroupDef->OneNum ; i ++ )
		{
			if( pGiftGroupDef->GiftOnes[i].Day == nDay )
			{
				return (tagGiftOne_ToLua*)(&pGiftGroupDef->GiftOnes[i]);
			}
		}
	}
	return NULL;
}

bool GameWizard::CanShowPreCreateGift()
{
	return m_bShowPreCreate;
}

bool GameWizard::CanGetPreCreateGift()
{
	return m_PreCreateGift == ROLE_PRE_FLAG;
}

bool GameWizard::IsVipNetBarUser()
{
	return ( m_UserType & USER_TYPE_INTERNET_BAR );
}

int GameWizard::GetVipNetBarGiftNum()
{
	return m_vVipBarGift.size();
}

tagOneDayGift_ToLua* GameWizard::GetVipNetBarGift(int nIdx)
{
	if( nIdx < m_vVipBarGift.size() )
	{
		return (tagOneDayGift_ToLua*)m_vVipBarGift[nIdx];
	}
	return NULL;
}

int GameWizard::GetClanOneDayGift()
{
	return m_vectClanGift.size();
}

tagOneDayGift_ToLua* GameWizard::GetClanOneDayGift( int nIdx )
{
	if( nIdx < m_vectClanGift.size() )
	{
		return ( tagOneDayGift_ToLua* ) m_vectClanGift[nIdx];
	}
	return NULL;
}

int GameWizard::getLtrGiftCount(int itemId)
{
	for( int i = 0 ; i < m_RoleLtrInfo.Count ; i++ )
	{
		if( m_RoleLtrInfo.RoleLtrInfoDetail[i].ItemID == itemId )
		{
			return m_RoleLtrInfo.RoleLtrInfoDetail[i].Num;
		}
	}
	return 0;
}

tagMediaGiftDef_ToLua* GetMediaGiftDef( int nGiftId )
{
	static_assert(sizeof(tagMediaGiftDef_ToLua) == sizeof(tagMediaGiftDef));
	return(tagMediaGiftDef_ToLua*)(DBDataManager::getSingleton().getMediaGiftDef(nGiftId));
}

tagLtrGiftDef_ToLua* GetLtrGiftDef( int nGiftId )
{
	static_assert(sizeof(tagLtrGiftDef_ToLua) == sizeof(tagLtrGiftDef));
	return(tagLtrGiftDef_ToLua*)(DBDataManager::getSingleton().getLtrGiftDef(nGiftId));
}

tagBuChangDef_ToLua* GetBuChangDef( int nDay )
{
	static_assert(sizeof(tagBuChangDef_ToLua) == sizeof(tagBuChangDef));
	return(tagBuChangDef_ToLua*)(DBDataManager::getSingleton().getBuChangDef(nDay));
}

tagArmBuChangDef_TuLua* GetArmBuChangDef( int nLvl, int nCareer )
{
	static_assert(sizeof(tagArmBuChangDef_TuLua) == sizeof(tagArmBuChangDef));
	return(tagArmBuChangDef_TuLua*)(DBDataManager::getSingleton().getArmBuChangDef(nLvl,nCareer));
}

bool GameWizard::IsAlreadyGetOneDayGift( int giftId )
{
	for( int i = 0 ; i < m_RoleGiftInfo.OneDayNum; i++ )
	{
		if( m_RoleGiftInfo.GiftInfoOneDay[i].GiftID == giftId )
		{
			return true;
		}
	}
	return false;
}

bool GameWizard::IsAlreadGetClanDayGift( int nIdx )
{
	if( IsAlreadyGetOneDayGift( m_vectClanGift[nIdx]->GiftID ) )
	{
		return true;
	}

	if( m_vectClanGift[nIdx]->GiftCond.ClanLevel > 0 )
	{
		for( size_t i = 0 ; i < m_vectClanGift.size() ; i++ )
		{
			if( m_vectClanGift[i]->GiftCond.ClanLevel > 0 && IsAlreadyGetOneDayGift(m_vectClanGift[i]->GiftID) )
			{
				return true;
			}
		}
	}
	return false;	 
}

bool GameWizard::isCanGetClanGift()
{
	if ( !ActorManager::getSingleton().getMainPlayer()->isInClan() )
	{
		return false;
	}
	for( size_t i = 0 ; i < m_vectClanGift.size() ; i++ )
	{		
		if( !IsAlreadGetClanDayGift( i ) )
		{
			if( m_vectClanGift[i]->GiftCond.ClanTouch > 0 )
			{
				if( ActorManager::getSingleton().getMainPlayer()->getLv() >= CLAN_GIFT_TOUCH_EXP_LEVEL )
				{
					return true;
				}
			}
			return true;
		}
	}
	return false;
}

tagPosition_ToLua GameWizard::getPlayTopWindowPos( int actorId )
{
	GameActor* pActor = ActorManager::getSingleton().FindActor( actorId );
	FollowCamera *pcamera = GamePlayManager::getSingleton().getMainCamera();
	if( pActor == NULL || pcamera == NULL )
	{
		return tagPosition_ToLua( 0,0 );
	}

	Ogre::Vector3 pos = pActor->getShowActor()->getCurEntity()->getWorldPosition();
	pos.y += pActor->getShowActor()->getActorHeight() * pActor->getShowActor()->getCurEntity()->getScale().y;
	Ogre::Vector3 scrpos;
	pcamera->GetCameraNode()->pointWorldToWindow( scrpos, pos );

	int x = (int)( scrpos.x );
	int y = (int)( scrpos.y );
	return tagPosition_ToLua( x,y );
}

void GameWizard::UpdateDirEntity( int nId )
{
	GameActor* pActor = ActorManager::getSingleton().FindActor( nId );
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if( pActor == NULL || pMain == NULL )
	{
		HideDirEntity();
		return;
	}
	if( GamePlayManager::getSingleton().getCurMap() == NULL ||
		GamePlayManager::getSingleton().getCurMap()->getShowGameMap()->getGameScene() == NULL )
	{
		return;
	}
	if( m_pDirEntity == NULL )
	{
		m_pDirEntity = new Ogre::Entity;
		m_pDirEntity->load( "effect\\entity\\9578_16.ent", false );
		m_pDirEntity->attachToScene( GamePlayManager::getSingleton().getCurMap()->getShowGameMap()->getGameScene() );
	}
	Ogre::WorldPos pos = pMain->GetPosition();
	GamePlayManager::getSingleton().getCurMap()->getTerrainH(pos.x, pos.z, pos.y);
	pos += Ogre::Vector3( 0, 20, 0 );
	m_pDirEntity->setPosition( pos );

	int angle = FaceAngleBetween( pMain->GetPosition().x,pMain->GetPosition().z,pActor->GetPosition().x,pActor->GetPosition().z );
	m_pDirEntity->setRotation( 90.0f + angle,0,0 );
	m_pDirEntity->update(30);	
}

void GameWizard::HideDirEntity()
{
	if( m_pDirEntity != NULL )
	{
		if( GamePlayManager::getSingleton().getCurMap() != NULL )
		{
			m_pDirEntity->detachFromScene();
		}		
		OGRE_RELEASE(m_pDirEntity);
	}
}

void GameWizard::RequestReloadAccgift()
{
	tagCSPkgBody msgbody;
	msgbody.GiftClt.GiftType = GIFT_TYPE_ACCGIFT_RELOAD;
	SendGameServerMsg( GIFT_CLT, msgbody );
}

void GameWizard::requestPropsGift( int id, tdr_ulonglong cash )
{
	if( GameManager::getSingleton().getTickTime() - m_nLastRequestTime < 500 )
	{
		return;
	}
	m_nLastRequestTime = GameManager::getSingleton().getTickTime();
	tagCSPkgBody msgbody;
	memset( &msgbody,0,sizeof(tagCSPkgBody) );
	msgbody.GiftClt.GiftType = GIFT_TYPE_PROPGIFT;
	msgbody.GiftClt.CltData.PropGiftIDInfo.GroupID = id;
	msgbody.GiftClt.CltData.PropGiftIDInfo.CashUp = cash;
	SendGameServerMsg( GIFT_CLT, msgbody );
}

int GameWizard::compareValue( tdr_ulonglong Val1, tdr_ulonglong Val2 )
{
	if ( Val1 < Val2 )
	{
		return -1;
	}
	else if ( Val1 > Val2 )
	{
		return 1;
	}
	return 0;
}

const tagContCoverGift_ToLua* GameWizard::getContCoverGiftDef( int nDays )
{
	static_assert( sizeof(tagContCoverGift_ToLua) == sizeof(tagContCoverGift) );
	return (const tagContCoverGift_ToLua*) DBDataManager::getSingleton().getContCoverGiftDef( nDays );
}

int GameWizard::getContCoverMaxDays()
{
	std::vector<CONTCOVERGIFT> ContCoverGift;
	DBDataManager::getSingleton().loadConCoverGift(ContCoverGift);
	int nrecord = (int)ContCoverGift.size();
	return nrecord;
}

void GameWizard::contGiftCheckIN( int nDays )
{
	tagCSPkgBody msgbody;
	memset( &msgbody,0,sizeof(tagCSPkgBody) );
	msgbody.GiftClt.GiftType = GIFT_TYPE_CONT_COVER;
	msgbody.GiftClt.CltData.ContinueDay = nDays;
	SendGameServerMsg( GIFT_CLT, msgbody );
}
const tagNewRoleGift_ToLua* GameWizard::getNewRoleGiftDef( int nID )
{
	static_assert( sizeof(tagNewRoleGift_ToLua) == sizeof(tagNewRoleGift) );
	return (const tagNewRoleGift_ToLua*) DBDataManager::getSingleton().getNewRoleGiftDef( nID );
}
void GameWizard::getNewRoleGift( int nID )
{
	tagCSPkgBody msgbody;
	memset( &msgbody,0,sizeof(tagCSPkgBody) );
	msgbody.GiftClt.GiftType = GIFT_TYPE_NEW_ROLE;
	msgbody.GiftClt.CltData.ID = nID;
	SendGameServerMsg( GIFT_CLT, msgbody );
}
// 发送MT礼包
void GameWizard::sendAwardMTGift()
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_MT_GIFT_AWARD;
	msgbody.PlayerMiscClt.PlayerMiscCltData.AwardMTGift = PLAYER_MT_GIFT_AWARD;
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}