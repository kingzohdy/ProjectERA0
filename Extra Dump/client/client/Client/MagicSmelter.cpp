#include "MagicSmelter.h"
#include "proto_cs.h"
#include "GameEngineTypes.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include "GameMsgPrint.h"
#include "GameDBData.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "EffectXPPlay.h"
#include "OgreSoundSystem.h"

MagicSmelterManager::MagicSmelterManager(void)
{
	INetworkManager::getSingleton().RegisterMsgHandler( RONGLU_SVR, this );
	DBDataManager::getSingleton().loadRongluDef( m_vecRongluData );
}

MagicSmelterManager::~MagicSmelterManager(void)
{
}

void MagicSmelterManager::initRongluInfo()
{
	m_nSmelterID = 0;
	m_nResultID = 0;
	m_nGradeLv = 0;
}

void MagicSmelterManager::requestRecastSmelter( int nGridIdx )
{
	tagCSPkgBody msgbody;
	msgbody.RongluClt.Cmd = RONGLU_CLT_OP_RESET_START;
	msgbody.RongluClt.Data.Start.GridIdx = nGridIdx;
	SendGameServerMsg( RONGLU_CLT, msgbody );
}

void MagicSmelterManager::endRecastSmelter( int nGridIdx, int id )
{
	tagCSPkgBody msgbody;
	msgbody.RongluClt.Cmd = RONGLU_CLT_OP_RESET_END;
	msgbody.RongluClt.Data.End.GridIdx = nGridIdx;
	if ( m_nSmelterID != id )
	{
		msgbody.RongluClt.Data.End.RongluID = m_nSmelterID;
	}
	else
	{
		msgbody.RongluClt.Data.End.RongluID = id;
	}	
	SendGameServerMsg( RONGLU_CLT, msgbody );
}

void MagicSmelterManager::breakRecastSmelter()
{
	tagCSPkgBody msgbody;
	msgbody.RongluClt.Cmd = RONGLU_CLT_OP_RESET_BREAK;
	SendGameServerMsg( RONGLU_CLT, msgbody );
}

tagRongluDef_ToLua* MagicSmelterManager::getRongluDef( int id )
{
	static_assert( sizeof(tagRongluDef_ToLua) == sizeof(tagRongluDef) );
	const tagRongluDef* rongluDef = DBDataManager::getSingleton().getRongLuDef( id );
	if ( rongluDef != NULL)
	{
		return (tagRongluDef_ToLua*)rongluDef;
	}
	return NULL;
}

int MagicSmelterManager::getRongluIdByPinZhi( int nIndex )
{
	assert( nIndex < m_vecRongluData.size() );
	std::vector<tagRongluDef*>::iterator iter = m_vecRongluData.begin();
	for (; iter != m_vecRongluData.end(); ++iter)
	{
		if ( (*iter)->Pinzhi == nIndex )
		{
			return (*iter)->ItemID;
		}
	}
	return 0;
}

void MagicSmelterManager::playRongluRecastSound()
{
	Ogre::SoundSystem::getSingleton().playSound2D("sounds\\sounds_1\\t电火花.wma", 1);
}

int MagicSmelterManager::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if ( msgid != RONGLU_SVR )
	{
		return 0;
	}
	const RONGLUSVR& rongLuSvr = msgbody.RongluSvr;
	switch( rongLuSvr.Cmd )
	{
	case RONGLU_SVR_OP_START_RES:
		{
			m_nSmelterID = rongLuSvr.Data.Start.ItemID;
			char buffer[256];
			sprintf( buffer, "SetRecastResult(%d)", rongLuSvr.Data.Start.ItemID );
			GameManager::getSingleton().getScriptVM()->callString( buffer );			
		}
		break;
	case RONGLU_SVR_OP_ONE:
		{
			m_nResultID = rongLuSvr.Data.One.ItemID;
			m_nGradeLv = rongLuSvr.Data.One.LvlGrade;
			char buffer[256];
			sprintf( buffer, "UpdateRecastResult(%d,%d)", rongLuSvr.Data.One.ItemID,rongLuSvr.Data.One.LvlGrade );
			GameManager::getSingleton().getScriptVM()->callString( buffer );
			GameMsgPrint::getSingleton().SendUIEvent("GE_QUEST_ITEM_CHANGE");
		}
		break;
	case RONGLU_SVR_OP_KILL_NUM_CHG:
		{
			//失落图腾播放灵魂特效协议借用了原来的熔炉协议，如果类型为1就表示是失落图腾的消息，0表示熔炉
			if (  rongLuSvr.Data.Kill.Type == 1 )
			{
				GameActor* pOwner = ActorManager::getSingleton().FindActor( rongLuSvr.Data.Kill.MonMemID );
				if ( pOwner != NULL )
				{
					pOwner->setLostTotemExpNum( rongLuSvr.Data.Kill.Num );
					pOwner->setLostTotemEffect();
					MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
					//只显示属于自己的图腾吸魂特效并刷新计数器
					if ( pMainPlayer != NULL && strcmp( pOwner->getOwnerName(), pMainPlayer->getName()) == 0 )
					{	
						GameActor* pMonster = ActorManager::getSingleton().FindActor( rongLuSvr.Data.Kill.KillMonMemID );
						if ( pMonster != NULL )
						{
							pMonster->setDeathAction(new EffectXPPlay(pMonster, pOwner, 1));
						}
					}
				}
			}
			else
			{
				GameActor* pOwner = ActorManager::getSingleton().FindActor( rongLuSvr.Data.Kill.MonMemID );
				if ( pOwner != NULL )
				{
					pOwner->setKillCount( rongLuSvr.Data.Kill.Num );
					MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
					//只显示属于自己的熔炉吸魂特效并刷新计数器
					if ( pMainPlayer != NULL && strcmp( pOwner->getOwnerName(), pMainPlayer->getName()) == 0 )
					{	
						GameActor* pMonster = ActorManager::getSingleton().FindActor( rongLuSvr.Data.Kill.KillMonMemID );
						if ( pMonster != NULL )
						{
							pMonster->setDeathAction(new EffectXPPlay(pMonster, pOwner, 1));
						}
					}
				}
			}
		}
	default:
		break;
	}
	return 0;
}