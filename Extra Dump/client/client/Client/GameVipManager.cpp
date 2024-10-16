#include "GameVipManager.h"
#include "NetMessage.h"
#include "GameDBData.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "GameClientOpt.h"
#include "GameQuest.h"

GameVipManager::GameVipManager()
{
	reset();
	GameClientOpt::getSingleton().setLevelSize( SAVE_OPT_AUTO_QUEST, sizeof(tagAutoQuestData) );
	GameClientOpt::getSingleton().setLevelSize( SAVE_OPT_AUTO_RECOVERY, sizeof(tagAutoRecoveryData) );
	INetworkManager::getSingleton().RegisterMsgHandler( STARDATA_SVR, this );
}

GameVipManager::~GameVipManager()
{

}

const tagStarStoneDef_ToLua* GameVipManager::getStarStoneDefByIdx( int nIdx )
{
	static_assert( sizeof(tagStarStoneDef) == sizeof(tagStarStoneDef_ToLua) );
	return (const tagStarStoneDef_ToLua*) DBDataManager::getSingleton().getStarStoneDefByIdx( nIdx );
}

const tagStarStoneDef_ToLua* GameVipManager::getStarStoneDef( int nID )
{
	static_assert( sizeof(tagStarStoneDef) == sizeof(tagStarStoneDef_ToLua) );
	return (const tagStarStoneDef_ToLua*) DBDataManager::getSingleton().getStarStoneDef( nID );
}

const tagStarFuncDef_ToLua* GameVipManager::getStarFuncDef( int nID )
{
	static_assert( sizeof(tagStarFuncDef) == sizeof(tagStarFuncDef_ToLua) );
	return (const tagStarFuncDef_ToLua*) DBDataManager::getSingleton().getStarFuncDef( nID );
}

void GameVipManager::reset()
{
	memset( &m_vipData, 0, sizeof(VIPDATA) );
	m_vEnableFunc.clear();
	m_bRequestServer = false;
}

bool GameVipManager::haveOpenVip()
{
	return m_vipData.VipFlag > 0;
}

int GameVipManager::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if( msgid == STARDATA_SVR )
	{
		switch( msgbody.StarDataSvr.Type )
		{
		case STARDATA_OPEN_STONE_RES:
			{
				m_bRequestServer = false;
				const CSSTARSTONEOPENRES& res = msgbody.StarDataSvr.StarDataSvr.StarStoneOpenRes;
				char buf[128];
				sprintf( buf, "OnOpenStoneRes( %d,%d )", res.Result, res.StarStoneID );
				GameManager::getSingleton().getScriptVM()->callString( buf );
				m_vipData = res.VipData;
				resetFuncData();
				GameManager::getSingleton().SendUIEvent( "GE_VIP_INFO_UPDATE" );				
				//刷新npc任务信息
				GameQuest::getSingleton().updateCanAcceptQuest();
				GameQuest::getSingleton().updateNpcQuestTitle();
			}
			break;
		case STARDATA_SYNC_RES:
			{
				m_vipData = msgbody.StarDataSvr.StarDataSvr.StarDataRes.VipData;
				MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
				if( pMain != NULL )
				{
					pMain->setVipFlag( m_vipData.VipFlag );
				}
				resetFuncData();
				GameManager::getSingleton().SendUIEvent( "GE_VIP_INFO_UPDATE" );
				//刷新npc任务信息
				GameQuest::getSingleton().updateCanAcceptQuest();
				GameQuest::getSingleton().updateNpcQuestTitle();
			}
			break;
		}
	}
	return 0;
}

bool GameVipManager::isEnableVip( int nID )
{
	if( m_vipData.VipFlag & (1<<nID) )
	{
		return true;
	}
	return false;
}

int GameVipManager::getStarFunResultVal1( int nResultID )
{
	int num = 0;
	for ( size_t i = 0; i < DBDataManager::getSingleton().getStarFuncNum(); ++i )
	{
		const tagStarFuncDef* pDef = DBDataManager::getSingleton().getStarFuncDefByIdx( i );
		if ( pDef != NULL && pDef->StarResult.ResultID == nResultID )
		{
			num += pDef->StarResult.ResultVal1;
			break;
		}
	}
	return num;
}

int GameVipManager::getCangBaoTuVipNum()
{
	return getStarFunResultVal1( STAR_RESULT_CANGBAOTU );
}

int GameVipManager::getBCTVipNum()
{
	return getStarFunResultVal1( STAR_RESULT_DADI );
}

int GameVipManager::getShangShanNum()
{
	return getStarFunResultVal1( STAR_RESULT_SHANGSAN );
}

int GameVipManager::getRongLuNum()
{
	return getStarFunResultVal1( STAR_RESULT_RONGLU );
}

int GameVipManager::getShangJinNum()
{
	return getStarFunResultVal1( STAR_RESULT_SHANGJIN );
}

int GameVipManager::getVipEndTime( int nID )
{
	for( int i = 0 ; i < m_vipData.StoneNum; ++i )
	{
		if( m_vipData.StarStoneInfo[i].ID == nID )
		{
			return m_vipData.StarStoneInfo[i].EndTime;
		}
	}
	return 0;
}

void GameVipManager::requestOpenVip( int nType , int nID )
{
	if( !m_bRequestServer )
	{
		tagCSPkgBody msgbody;
		memset( &msgbody, 0, sizeof(tagCSPkgBody) );
		msgbody.StarDataClt.Type = STARDATA_OPEN_STONE_REQ;
		msgbody.StarDataClt.StarDataClt.StarStoneOpenReq.OpenTimeType = nType;
		msgbody.StarDataClt.StarDataClt.StarStoneOpenReq.StarStoneID = nID;
		SendGameServerMsg( STARDATA_CLT,msgbody );
		m_bRequestServer = true;
	}
}

void GameVipManager::openVipShop()
{
	tagCSPkgBody msgbody;
	memset( &msgbody, 0, sizeof(tagCSPkgBody) );
	msgbody.AdjustC.Type = ADJUST_TYPE_UI;
	msgbody.AdjustC.AdjustData.UI.UIType = UI_TYPE_NPCSHOP;
	SendGameServerMsg( ADJUST_CLT, msgbody );
}

void GameVipManager::resetFuncData()
{
	m_vEnableFunc.clear();
	for( int i = 0 ;i < m_vipData.StoneNum; ++i )
	{
		const tagStarStoneDef* stoneDef = DBDataManager::getSingleton().getStarStoneDef( m_vipData.StarStoneInfo[i].ID );
		if( stoneDef != NULL )
		{
			for( int idx = 0; idx < MAX_STONE_FUNC; ++ idx )
			{
				if( stoneDef->StarFuncList[idx] > 0 )
				{
					m_vEnableFunc.push_back( stoneDef->StarFuncList[idx] );
				}
			}
		}
	}
}

tagAutoQuestData* GameVipManager::getAutoQuestOpt()
{
	return &m_autoQuestData;
}

void GameVipManager::saveAutoQuestOpt()
{
	GameClientOpt::getSingleton().setClientOpt( SAVE_OPT_AUTO_QUEST, (char*)& m_autoQuestData );
}

tagAutoRecoveryData* GameVipManager::getAutoRecoveryOpt()
{
	return &m_autoRecoveryData;
}

void GameVipManager::saveAutoRecoveryOpt()
{
	GameClientOpt::getSingleton().setClientOpt( SAVE_OPT_AUTO_RECOVERY, (char*)& m_autoRecoveryData );
}

bool GameVipManager::isOwnVipFunc( int nResultID )
{
	for ( size_t i = 0; i < m_vEnableFunc.size(); ++i )
	{
		const tagStarFuncDef* pDef = DBDataManager::getSingleton().getStarFuncDef( m_vEnableFunc[i] );
		if ( pDef != NULL && pDef->StarResult.ResultID == nResultID )
		{
			return true;
		}
	}
	return false;
}