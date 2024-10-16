#include "GameClan_SpanBaseWar.h"
#include "GameManager.h"
#include "GameDBData.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "GameLoginManager.h"
#include "OgreScriptLuaVM.h"

GameClan_SpanBaseWar::GameClan_SpanBaseWar(void)
{
	INetworkManager::getSingleton().RegisterMsgHandler( SPAN_STRONG_SVR, this );
}

GameClan_SpanBaseWar::~GameClan_SpanBaseWar(void)
{
}

int GameClan_SpanBaseWar::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if ( msgid == SPAN_STRONG_SVR )
	{
		const CSSPANSTRONGSVR& Msg = msgbody.SpanStrongSvr;
		switch( Msg.Cmd )
		{
		case SPAN_STRONG_SVR_ALL_INFO:
			{
				mAllStrongInfo = Msg.Data.AllInfo.All;
				GameManager::getSingleton().SendUIEvent("GE_SPAN_STRONG_ALL_INFO");
			}
			break;
		case SPAN_STRONG_SVR_INFO_ONE:
			{
				for( int i = 0;  i < mAllStrongInfo.Num;  i++ )
				{
					if( mAllStrongInfo.Strong[i].StrongID == Msg.Data.OneInfo.One.StrongID )
					{
						mAllStrongInfo.Strong[i] = Msg.Data.OneInfo.One;
					}
				}
				GameManager::getSingleton().SendUIEvent("GE_SPAN_STRONG_CHANGE_INFO");
			}
			break;
		case SPAN_STRONG_SVR_PWORLD_INFO:
			{
				mSpanStrongPwolrd = Msg.Data.Pworld;
			}
			break;
		case SPAN_STRONG_SVR_WAR_ONE:
			{
				MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
				for( int i = 0; i < mWarAllPlayerInfo.Num; i++ )
				{
					if( strcmp( Msg.Data.WarOne.RoleName, mWarAllPlayerInfo.Ones[i].RoleName ) == 0 )
					{
						mWarAllPlayerInfo.Ones[i] = Msg.Data.WarOne;
						GameManager::getSingleton().SendUIEvent("GE_SPAN_STRONG_WAR_INFO_UPDATE");
					}
				}
				pMain->m_nCampIdx = Msg.Data.WarOne.Camp;
				mSpanStrongSelfInfo = Msg.Data.WarOne;
			}
			break;
		case SPAN_STRONG_SVR_WAR_ALL:
			{
				MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
				mWarAllPlayerInfo = Msg.Data.WarALL;
				if ( Msg.Data.WarALL.End > 0 )
				{
					GameManager::getSingleton().SendUIEvent("GE_SPAN_STRONG_WAR_END");
					int nWinFlag = 0;
					if ( Msg.Data.WarALL.WinCamp == pMain->m_nCampIdx )
					{
						nWinFlag = 1;
					}
					char buf[256];
					sprintf( buf, "SetSpanStrongWarResult(%d)",nWinFlag );
					GameManager::getSingleton().getScriptVM()->callString( buf );
					pMain->m_nCampIdx = 0;
				}
				GameManager::getSingleton().SendUIEvent("GE_SPAN_STRONG_WAR_INFO_UPDATE");
			}
			break;
		case SPAN_STRONG_SVR_OCC_INFO:
			{
				m_SpanStrongOccInfo = Msg.Data.Occ;
				//将据点的即时变化同步到所有据点信息里
				for( int i = 0; i < Msg.Data.Occ.Num; i++ )
				{
					mAllStrongInfo.Strong[i].StrongID = Msg.Data.Occ.Occ[i].StrongID;
					mAllStrongInfo.Strong[i].Owner = Msg.Data.Occ.Occ[i].Owner;
					mAllStrongInfo.Strong[i].StrongID = Msg.Data.Occ.Sale[i].StrongID;
					mAllStrongInfo.Strong[i].Sale = Msg.Data.Occ.Sale[i].Sale;
				}
				GameManager::getSingleton().SendUIEvent("GE_SPAN_STRONG_CHANGE_INFO");
			}
			break;
		default:
			break;
		}
	}
	return 0;
}
void GameClan_SpanBaseWar::SpanStrongSale( int nID, int nMoney )
{
	tagCSPkgBody msgbody;
	msgbody.SpanStrongClt.Cmd = SPAN_STRONG_CLT_OP_SALE;
	msgbody.SpanStrongClt.Data.Sale.Money = nMoney;
	msgbody.SpanStrongClt.Data.Sale.StrongID = nID;
	SendGameServerMsg( SPAN_STRONG_CLT, msgbody );
}
void GameClan_SpanBaseWar::SpanStrongEnter( int nID )
{
	tagCSPkgBody msgbody;
	msgbody.SpanStrongClt.Cmd = SPAN_STRONG_CLT_OP_ENTER;
	msgbody.SpanStrongClt.Data.Enter.StrongID = nID;
	SendGameServerMsg( SPAN_STRONG_CLT, msgbody );
}
void GameClan_SpanBaseWar::ReqSpanStrongInfo()
{
	tagCSPkgBody msgbody;
	msgbody.SpanStrongClt.Cmd = SPAN_STRONG_CLT_OP_INFO_REQ;
	SendGameServerMsg( SPAN_STRONG_CLT, msgbody );
}
void GameClan_SpanBaseWar::ReqSelectGroup( int nStrongID, int nGroupID )
{
	tagCSPkgBody msgbody;
	msgbody.SpanStrongClt.Cmd = SPAN_STRONG_CLT_OP_GROUP;
	msgbody.SpanStrongClt.Data.Group.Group = nGroupID;
	msgbody.SpanStrongClt.Data.Group.StrongID = nStrongID;
	SendGameServerMsg( SPAN_STRONG_CLT, msgbody );
}
void GameClan_SpanBaseWar::ReqSpanStrongAward( int nStrongID )
{
	tagCSPkgBody msgbody;
	msgbody.SpanStrongClt.Cmd = SPAN_STRONG_CLT_OP_AWARD;
	msgbody.SpanStrongClt.Data.Award.StrongID = nStrongID;
	SendGameServerMsg( SPAN_STRONG_CLT, msgbody );
}
tagRStrongOne_ToLua GameClan_SpanBaseWar::getSpanStrongInfoByIndex( int nIdx )
{
	static_assert(sizeof(tagRStrongOne) == sizeof(tagRStrongOne_ToLua)); 
	tagRStrongOne_ToLua result;
	memset( &result, 0, sizeof(tagRStrongOne_ToLua) );
	if( nIdx <= mAllStrongInfo.Num && nIdx > 0 )
	{
		memcpy( &result, &mAllStrongInfo.Strong[nIdx-1], sizeof(tagRStrongOne_ToLua) );
	}
	return result;
}
tagSpanStrongDef_ToLua GameClan_SpanBaseWar::getSpanStrongDef( int ID )
{
	static_assert(sizeof(tagSpanStrongDef) == sizeof(tagSpanStrongDef_ToLua)); 
	tagSpanStrongDef_ToLua def;
	memset( &def, 0, sizeof(tagSpanStrongDef_ToLua) );
	if ( ID <= 0 || ID >  MAX_SPAN_STRONG_NUM )
	{
		return def;
	}
	memcpy( &def, DBDataManager::getSingleton().getSpanStrongDef(ID), sizeof(tagSpanStrongDef)  );
	return def;
}
tagCsSpanStrongWarOne_ToLua	GameClan_SpanBaseWar::getSpanStrongPlayerInfo( int nIdx )
{
	static_assert(sizeof(tagCsSpanStrongWarOne) == sizeof(tagCsSpanStrongWarOne_ToLua)); 
	tagCsSpanStrongWarOne_ToLua info;
	memset( &info, 0, sizeof(tagCsSpanStrongWarOne_ToLua) );
	if( nIdx > mWarAllPlayerInfo.Num || nIdx <= 0 )
	{
		return info;
	}
	memcpy( &info, &mWarAllPlayerInfo.Ones[nIdx-1], sizeof(tagCsSpanStrongWarOne)  );
	return info;
}
int GameClan_SpanBaseWar::getSpanStrongPlayerNum()
{
	return mWarAllPlayerInfo.Num;
}
tagCsSpanStrongPworld_ToLua GameClan_SpanBaseWar::getSpanStrongWarPworldInfo()
{
	tagCsSpanStrongPworld_ToLua pworldInfo;
	memcpy( &pworldInfo, &mSpanStrongPwolrd, sizeof(tagCsSpanStrongPworld_ToLua) );
	return pworldInfo;
}
int	GameClan_SpanBaseWar::getSpanStrongSelfContinueKill()
{
	return mSpanStrongSelfInfo.CurrContinueKill;
}
int	GameClan_SpanBaseWar::getSpanStrongOwnerExpVal()
{
	return m_SpanStrongOccInfo.MaxExp;
}
int	GameClan_SpanBaseWar::getSpanStrongOwnerBindGold()
{
	return m_SpanStrongOccInfo.MaxBindGold;
}
int	GameClan_SpanBaseWar::getSpanStrongOwnerBull()
{
	return m_SpanStrongOccInfo.MaxBull;
}
void GameClan_SpanBaseWar::objEnterView(GameActor *actor)
{
	if ( !isInSpanStrongPworld() )
	{
		return;
	}
	if( actor->getServerId() != -1 )
	{
		actor->setActorWorldName( LoginManager::getSingleton().getWorldNameById(actor->getServerId()) );
	}
}
void GameClan_SpanBaseWar::loadSpanStrongPworldID()
{
	static_assert(sizeof(tagSpanStrongDef) == sizeof(tagSpanStrongDef_ToLua)); 
	for( int i = 0; i <MAX_SPAN_STRONG_NUM; i++ )
	{
		const tagSpanStrongDef* pDef =DBDataManager::getSingleton().getSpanStrongDef(i+1);
		m_nSpanPworldID[i] = pDef->PworldID;
	}
}
bool GameClan_SpanBaseWar::isInSpanStrongPworld()
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	int mapid = pMain->getMapRealId();
	int nPworldId = mapid/100;
	if( mapid >= GAME_PWORLD_MAPID )
	{
		for( int i = 0; i <MAX_SPAN_STRONG_NUM; i++ )
		{
			if( m_nSpanPworldID[i] == nPworldId )
			{
				return true;
			}
		}
	}
	return false;
}