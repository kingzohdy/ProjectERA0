#include "GameArmProce.h"
#include "GameDBData.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "GameExchange.h"
#include "GameActorManager.h"
#include "ShowActor.h"
#include "GameChatManager.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include "GameWizard.h"
#include "math.h"

const tagGemDef_ToLua* getGemDef( int nItemID )
{
	static_assert( sizeof( tagGemDef_ToLua ) == sizeof( tagGemDef ) );
	const tagGemDef* pGemDef = &DBDataManager::getSingleton().getGemDef( nItemID );
	return ( const tagGemDef_ToLua* ) pGemDef;
}

const tagArmLevelUPDef_ToLua* getArmLevelUPDef( int nID )
{
	static_assert( sizeof(tagArmLevelUPDef_ToLua) == sizeof(tagArmLevelUPDef) );
	return (const tagArmLevelUPDef_ToLua*) DBDataManager::getSingleton().getArmLevelUPDef(nID);
}

int getArmMoveMoney( int nType )
{
	int nID = ( nType == 1 ) ? USE_ID_ARM_SKILL_MOVE : USE_ID_ATTR_MOVE;
	const tagUseDef *UseDef = DBDataManager::getSingleton().getUseDef( nID );
	if (UseDef == NULL)
	{
		return 0;
	}
	return UseDef->UseMoney;
}

int getArmResetMoney()
{
	const tagUseDef *UseDef = DBDataManager::getSingleton().getUseDef(USE_ID_ATTR_RESET);
	if (UseDef == NULL)
	{
		return 0;
	}
	return UseDef->UseMoney;

}

GameArmProce::GameArmProce(void)
{
	INetworkManager::getSingleton().RegisterMsgHandler( ARM_SVR, this );
	INetworkManager::getSingleton().RegisterMsgHandler( BACK_SVR, this );
	DBDataManager::getSingleton().loadGemDef( m_gemDef ); 
	for ( std::vector<tagGemDef*>::iterator iter = m_gemDef.begin(), end = m_gemDef.end(); iter != end; ++iter )
	{
		tagGemDef* pOneGemDef = *iter;
		if ( pOneGemDef == NULL )
		{
			continue;
		}

		m_gemTypePartion[pOneGemDef->GemType].push_back( pOneGemDef );
	}
	DBDataManager::getSingleton().loadGemNewDef( m_gemNewDef );
	for ( std::vector<tagGemNewDef*>::iterator iter = m_gemNewDef.begin(), end = m_gemNewDef.end(); iter != end; ++iter )
	{
		tagGemNewDef* pOneGemNewDef = *iter;
		if ( pOneGemNewDef == NULL )
		{
			continue;
		}
	}
	m_bCanOpenHole = true;
	std::vector<tagLevelUP*> verLevelUpDef;
	DBDataManager::getSingleton().loadLevelUp( verLevelUpDef );
	for( size_t i = 0 ; i < verLevelUpDef.size() ; i++ )
	{
		if( verLevelUpDef[i]->AddSparPkg > 0 )
		{
			m_vSparAddLv.push_back( verLevelUpDef[i]->Level );
		}
	}
}

GameArmProce::~GameArmProce(void)
{
}

// 装备品质提升
void GameArmProce::armAddStar( int nListType, int nGridIdx, int nSelectStar, int nBind, int nMust )
{
	if ( GameExchange::getSingleton().isInExchange() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_EXCHANGE_ACTIONFORBID );
		return;
	}

	tagCSPkgBody msgbody;
	memset( &msgbody,0,sizeof(tagCSPkgBody) );
	msgbody.ArmClt.ArmCltType = ARM_ACT_STAR;
	msgbody.ArmClt.ArmCltData.ArmStar.ArmData.ArmListType	= nListType;
	msgbody.ArmClt.ArmCltData.ArmStar.ArmData.ArmGridIdx	= nGridIdx;
	msgbody.ArmClt.ArmCltData.ArmStar.SelectStar = nSelectStar;
	msgbody.ArmClt.ArmCltData.ArmStar.Bind = nBind;
	msgbody.ArmClt.ArmCltData.ArmStar.Must = nMust;

	SendGameServerMsg( ARM_CLT, msgbody );
}

//保存强化
void GameArmProce::armAddSave( int nArmListType, int nArmGridIdx, int nSelectStarIdx, int nRateIdx )
{
	tagCSPkgBody msgbody;
	memset( &msgbody, 0, sizeof(tagCSPkgBody) );
	msgbody.ArmClt.ArmCltType = ARM_STAR_SAVE;
	msgbody.ArmClt.ArmCltData.ArmStarSave.ArmData.ArmGridIdx = nArmGridIdx;
	msgbody.ArmClt.ArmCltData.ArmStarSave.ArmData.ArmListType = nArmListType;
	msgbody.ArmClt.ArmCltData.ArmStarSave.MulIdx = nRateIdx;
	msgbody.ArmClt.ArmCltData.ArmStarSave.SelectStar = nSelectStarIdx;

	SendGameServerMsg( ARM_CLT, msgbody );
}

void GameArmProce::bindArm( int nListType, int nGridIdx )
{
	tagCSPkgBody msgbody;
	memset( &msgbody,0,sizeof(tagCSPkgBody) );
	msgbody.ArmClt.ArmCltType = ARM_BIND;
	ARMBINDDATA& armBind = msgbody.ArmClt.ArmCltData.ArmBind;
	armBind.ArmData.ArmListType	= nListType;
	armBind.ArmData.ArmGridIdx	= nGridIdx;

	SendGameServerMsg( ARM_CLT, msgbody );
}

//装备解绑
void GameArmProce::unBindArm( int nListType,int nGridIdx,int nStuffID,int nStuffList,int nStuffGridIdx )
{
	tagCSPkgBody msgbody;
	memset( &msgbody,0,sizeof(tagCSPkgBody) );
	msgbody.ArmClt.ArmCltType = ARM_UNBIND;
	ARMUNBINDDATA& armStar = msgbody.ArmClt.ArmCltData.ArmUnBind;
	armStar.ArmData.ArmListType	= nListType;
	armStar.ArmData.ArmGridIdx	= nGridIdx;
	armStar.StuffData.ItemID = nStuffID;
	armStar.StuffData.ListType = nStuffList;
	armStar.StuffData.GridIdx = nStuffGridIdx;
	SendGameServerMsg( ARM_CLT, msgbody );
}

//转移品质
void GameArmProce::moveArmStar( int nSrcList,int nSrcGrid,int nDestList,int nDestGrid, int nBind )
{
	if ( GameExchange::getSingleton().isInExchange() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_EXCHANGE_ACTIONFORBID );
		return;
	}
	tagCSPkgBody msgbody;
	memset( &msgbody,0,sizeof(tagCSPkgBody) );
	msgbody.ArmClt.ArmCltType = ARM_MOVE_STAR;
	ARMMOVESTARDATA& armMoveStar = msgbody.ArmClt.ArmCltData.ArmMoveStar;
	armMoveStar.Bind = nBind;
	armMoveStar.DstArmData.ArmListType = nDestList;
	armMoveStar.DstArmData.ArmGridIdx = nDestGrid;
	armMoveStar.SrcArmData.ArmListType = nSrcList;
	armMoveStar.SrcArmData.ArmGridIdx = nSrcGrid;
	SendGameServerMsg( ARM_CLT,msgbody );
}

// 装备等级提升
void GameArmProce::armAddLevel( int nListType, int nGridIdx )
{
	tagCSPkgBody msgbody;
	memset( &msgbody, 0, sizeof(tagCSPkgBody) );
	msgbody.ArmClt.ArmCltType = ARM_LEVEL_UP;
	msgbody.ArmClt.ArmCltData.LevelUP.ArmData.ArmListType = nListType;
	msgbody.ArmClt.ArmCltData.LevelUP.ArmData.ArmGridIdx = nGridIdx;
	SendGameServerMsg( ARM_CLT, msgbody );
}

// 装备战斗力提升
void GameArmProce::armAddFight( int nListType, int nGridIdx, Item& safeItem )
{
	tagCSPkgBody msgbody;
	msgbody.ArmClt.ArmCltType = ARM_ACT_FIGHT;
	ARMUPDATA& armFight = msgbody.ArmClt.ArmCltData.ArmFight;
	armFight.ArmData.ArmListType	= nListType;
	armFight.ArmData.ArmGridIdx	= nGridIdx;
	//armFight.SafeFlag				= nSafeFlag;
	armFight.SafeData.ItemID		= safeItem.getItemId();
	armFight.SafeData.ListType	= safeItem.getListType();
	armFight.SafeData.GridIdx	= safeItem.getGridIdx();
	int nNum = 0;
	unsigned int nNeedMoney	= 0;
	for( int i = 0; i < MAX_STUFF_ARMPROCE; i++ )
	{
		if( m_Stuff[i].getItemId() != 0 )
		{
			armFight.StuffData[nNum].ItemID		= m_Stuff[i].getItemId();
			armFight.StuffData[nNum].ListType	= m_Stuff[i].getListType();
			armFight.StuffData[nNum].GridIdx	= m_Stuff[i].getGridIdx();
			//msgbody.ArmClt.ArmCltData.ArmStar.StuffID[nNum] = m_Stuff[i].getItemId();
			nNeedMoney += getGemCostMoney( m_SrcItem.getFight(), m_Stuff[i].getItemId() );
			nNum++;
		}
	}
	if( ActorManager::getSingleton().getMainPlayer()->getMoney() < nNeedMoney )
	{
		ChatManager::getSingleton().showMidTip( MID_ARMPROCET_NOTENOUGHBULL );
		return;
	}
	msgbody.ArmClt.ArmCltData.ArmFight.StuffNum = nNum;
	SendGameServerMsg( ARM_CLT, msgbody );
}

// 装备鉴定
void GameArmProce::armIdentifiability( int nListType, int nGridIdx )
{
	tagCSPkgBody msgbody;
	msgbody.ArmClt.ArmCltType = ARM_ACT_IDENTIFIABILITY;
	msgbody.ArmClt.ArmCltData.ArmIdentifiability.ArmGridIdx  = nGridIdx;
	msgbody.ArmClt.ArmCltData.ArmIdentifiability.ArmListType = nListType;
	
	if( ActorManager::getSingleton().getMainPlayer()->getMoney() < m_SrcItem.getItemDef()->IdentifiabilityMoney )
	{
		ChatManager::getSingleton().showMidTip( MID_ARMPROCET_NOTENOUGHBULL );
		return;
	}

	SendGameServerMsg( ARM_CLT, msgbody );
}

//装备鉴定（鉴定仪）
void GameArmProce::armIdentifyStart( int nSrcType,int nSrcGrid,int nDestType,int nDestGrid )
{
	tagCSPkgBody msgbody;
	msgbody.ArmClt.ArmCltType = ARM_ACT_CHECK;
	msgbody.ArmClt.ArmCltData.ArmCheck.Arm.ArmListType		= nSrcType;
	msgbody.ArmClt.ArmCltData.ArmCheck.Arm.ArmGridIdx		= nSrcGrid;
	msgbody.ArmClt.ArmCltData.ArmCheck.UseItem.ArmListType	= nDestType;
	msgbody.ArmClt.ArmCltData.ArmCheck.UseItem.ArmGridIdx	= nDestGrid;

	SendGameServerMsg( ARM_CLT, msgbody );
}
// 宝石升级
void GameArmProce::gemLevelUp( Item& stuffItem,int nType )
{
	tagCSPkgBody msgbody;
	msgbody.ArmClt.ArmCltType = GEM_ACT_LEVEL_UP;
	GEMLEVELUPDATA&	gemLevelUpData = msgbody.ArmClt.ArmCltData.GemLevelUp;
	//gemLevelUpData.StuffID = nStuffId;

	gemLevelUpData.StuffData.ItemID		= stuffItem.getItemId();
	gemLevelUpData.StuffData.ListType	= stuffItem.getListType();
	gemLevelUpData.StuffData.GridIdx	= stuffItem.getGridIdx();

	int nNum = 0;
	unsigned int nNeedMoney	= 0;
	for( int i = 0; i < MAX_STUFF_ARMPROCE; i++ )
	{
		if( m_Stuff[i].getItemId() != 0 )
		{
			//msgbody.ArmClt.ArmCltData.GemLevelUp.GemID[nNum] = m_Stuff[i].getItemId();
	
			gemLevelUpData.GemData[nNum].ItemID		= m_Stuff[i].getItemId();
			gemLevelUpData.GemData[nNum].ListType	= m_Stuff[i].getListType();
			gemLevelUpData.GemData[nNum].GridIdx	= m_Stuff[i].getGridIdx();
			
			nNeedMoney += getGemCostMoney( 0, m_Stuff[i].getItemId() );
			nNum++;
		}
	}

	if( ActorManager::getSingleton().getMainPlayer()->getMoney() < nNeedMoney )
	{
		ChatManager::getSingleton().showMidTip( MID_ARMPROCET_NOTENOUGHBULL );
		return;
	}

	if( nNum == 0 )
	{
		ChatManager::getSingleton().showMidTip( MID_ARMPROCE_NOUPDATEGEM );
	}

	msgbody.ArmClt.ArmCltData.GemLevelUp.GemNum = nNum;
	msgbody.ArmClt.ArmCltData.GemLevelUp.IfBaoXian = nType;
	SendGameServerMsg( ARM_CLT, msgbody );
}

// 宝石开孔
void GameArmProce::openGemHole( int nHoleIdx, int nListType )
{
	if( !m_bCanOpenHole )
	{
		return;
	}
	tagCSPkgBody msgbody;
	GEMOPENHOLEDATA& openHoleData	= msgbody.ArmClt.ArmCltData.GemOpenHole;
	msgbody.ArmClt.ArmCltType		= GEM_ACT_OPEN_HOLE;
	openHoleData.ArmHoleIdx				= nHoleIdx;
	openHoleData.ArmData.ArmGridIdx		= m_SrcItem.getGridIdx();
	openHoleData.ArmData.ArmListType	= nListType/*m_SrcItem.getListType()*/;
	SendGameServerMsg( ARM_CLT, msgbody );
	m_bCanOpenHole = false;
}

void GameArmProce::requireFengYinPlayerInfo( int nListType, int nGridIdx )
{
	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainplayer == NULL )
	{
		return;
	}

	Container& containerMgr = pMainplayer->getContainer();
	Item& requireAram = containerMgr.getItem( nListType, nGridIdx );
	if ( requireAram.getItemId() == 0 )
	{
		return;
	}

	tagCSPkgBody msgbody;
	CSARMCLT& armCltData	= msgbody.ArmClt;
	armCltData.ArmCltType	= ARM_UNFENGYIN_PRE;

	ARMUNFENGYINPRE& unFengyinPreData = armCltData.ArmCltData.ArmUnFengYinPre;

	ARMIDXDATA& requireArmData	= unFengyinPreData.ArmData;
	requireArmData.ArmListType	= nListType;
	requireArmData.ArmGridIdx	= nGridIdx;

	SendGameServerMsg( ARM_CLT, msgbody );
}

void GameArmProce::requireUnFengYin( int nListType, int nGridIdx )
{
	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainplayer == NULL )
	{
		return;
	}

	Container& containerMgr = pMainplayer->getContainer();
	Item& requireAram = containerMgr.getItem( nListType, nGridIdx );
	if ( requireAram.getItemId() == 0 )
	{
		return;
	}

	tagCSPkgBody msgbody;
	CSARMCLT& armCltData	= msgbody.ArmClt;
	armCltData.ArmCltType	= ARM_UNFENGYIN;

	ARMUNFENGYIN& unFengyinPreData = armCltData.ArmCltData.ArmUnFengYin;

	ARMIDXDATA& requireArmData	= unFengyinPreData.ArmData;
	requireArmData.ArmListType	= nListType;
	requireArmData.ArmGridIdx	= nGridIdx;

	SendGameServerMsg( ARM_CLT, msgbody );
}

// 宝石镶嵌
void GameArmProce::gemInlay( int nHoleIdx, Item& gemItem, int nListType/*, Item& safeItem*/ )
{
	tagCSPkgBody msgbody;
	msgbody.ArmClt.ArmCltType								= GEM_ACT_INLAY;
	GEMINLAYDATA& gemInLay = msgbody.ArmClt.ArmCltData.GemInlay;
	gemInLay.ArmHoleIdx				= nHoleIdx;
	gemInLay.ArmData.ArmGridIdx		= m_SrcItem.getGridIdx();
	//gemInLay.ArmData.ArmListType	= CONTAINER_TYPE_WEAR/*m_SrcItem.getListType()*/;
	gemInLay.ArmData.ArmListType	= nListType;
	//msgbody.ArmClt.ArmCltData.GemInlay.GemID			= nGemId;

	gemInLay.GemData.ItemID		= gemItem.getItemId();
	gemInLay.GemData.ListType	= gemItem.getListType();
	gemInLay.GemData.GridIdx	= gemItem.getGridIdx();

	//msgbody.ArmClt.ArmCltData.GemInlay.SafeFlag		= nSafeFlag;
	/*gemInLay.SafeData.ItemID	= safeItem.getItemId();
	gemInLay.SafeData.ListType	= safeItem.getListType();
	gemInLay.SafeData.GridIdx	= safeItem.getGridIdx();*/
	gemInLay.SafeData.ItemID	= 0;
	gemInLay.SafeData.ListType	= 0;
	gemInLay.SafeData.GridIdx	= 0;

	SendGameServerMsg( ARM_CLT, msgbody );
}

// 宝石摘取
void GameArmProce::gemPick( int nHoleIdx, int nList,int nGrid,int nStuffID,int nStuffList,int nStuffGrid )
{
// 	int nId = getGemPickUpUsedItem( m_SrcItem.getHoleGemId( nHoleIdx ) );
// 	if( nId != 0 )
// 	{
// 		const tagGemDef* pGemDef = &DBDataManager::getSingleton().getGemDef( m_SrcItem.getHoleGemId( nHoleIdx ) );
// 		if( ActorManager::getSingleton().getMainPlayer()->getMoney() < pGemDef->GemAttr->GemMoney )
// 		{
// 			ChatManager::getSingleton().showMidTip( MID_ARMPROCET_NOTENOUGHBULL );
// 			return;
// 		}
// 	}
	tagCSPkgBody msgbody;
	msgbody.ArmClt.ArmCltType = GEM_ACT_PICK;
	GEMPICKDATA& gemPickData = msgbody.ArmClt.ArmCltData.GemPick;
	gemPickData.ArmHoleIdx			=  nHoleIdx ;
	gemPickData.ArmData.ArmGridIdx	= nGrid;
	gemPickData.ArmData.ArmListType	= nList;

 	gemPickData.StuffData.ItemID	= nStuffID;
 	gemPickData.StuffData.ListType	= nStuffList;
 	gemPickData.StuffData.GridIdx	= nStuffGrid;
	SendGameServerMsg( ARM_CLT, msgbody );
}

// 是否在待装备列表中的物品
bool GameArmProce::isInStuffItem( int nListType, int nGrid )
{
	for( int i = 0; i < MAX_STUFF_ARMPROCE; i++ )
	{
		if( m_Stuff[i].getItemId() != 0 
			&& m_Stuff[i].getListType() == nListType && m_Stuff[i].getGridIdx() == nGrid )
		{
			return true;
		}
	}

	return false;
}

int GameArmProce::HandleNetMsg( int msgid, const tagCSPkgBody& msgbody )
{
	if( msgid == ARM_SVR )
	{
		switch( msgbody.ArmSvr.ArmSvrType )
		{
		case ARM_ACT_STAR:
			{
				/* 装备品质提升 */
				if( msgbody.ArmSvr.Flag == 1 )
				{
					ChatManager::getSingleton().showMidTip( MID_ARMPROCE_ARMSTAR );
					GameManager::getSingleton().SendUIEvent("GE_ARMSTAR_SUCESS");
				}else if( msgbody.ArmSvr.Flag == 0 )
				{
					GameManager::getSingleton().SendUIEvent("GE_ARMSTAR_FAILED");
				}
			}
			break;
		case ARM_ACT_LEVEL:
			{
				/* 装备等级提升 */
				if( msgbody.ArmSvr.Flag == 1 )
				{
					ChatManager::getSingleton().showMidTip( MID_ARMPROCE_LEVEL );
				}else{
					ChatManager::getSingleton().showMidTip( MID_ARMPROCE_LEVEL_FAIL );
				}
			}
			break;
		case ARM_ACT_FIGHT:
			{
				/* 装备战斗力提升 */
				if( msgbody.ArmSvr.Flag == 1 )
				{
					ChatManager::getSingleton().showMidTip( MID_ARMPROCE_FIGHT );
				}else{
					ChatManager::getSingleton().showMidTip( MID_ARMPROCE_FIGHT_FAIL );
				}
			}
			break;
		case ARM_ACT_IDENTIFIABILITY:
			{
				/* 装备鉴定 */
				if( msgbody.ArmSvr.Flag == 1 )
				{
					ChatManager::getSingleton().showMidTip( MID_ARMPROCE_IDENTIFIABILITY );
				}else{
					ChatManager::getSingleton().showMidTip( MID_ARMPROCE_IDENTIFIABILITY_FAIL );
				}
			}
			break;
		case ARM_FENJIE:
			{
			}
			break;
		case GEM_ACT_LEVEL_UP:
			{
				/* 宝石升级 */
				if( msgbody.ArmSvr.Flag == 1 )
				{
					//ChatManager::getSingleton().showMidTip( MID_ARMPROCE_GEMLEVELUP );
				}else{
					//ChatManager::getSingleton().showMidTip( MID_ARMPROCE_GEMLEVELUP_FAIL );
				}

				int nListType = 0;
				int nGridIdx = 0;
				char buffer[256];

				Item* resultItem = Container::getSingletonPtr()->getItem( msgbody.ArmSvr.WID );
				if ( resultItem != NULL )
				{
					nListType = resultItem->getListType();
					nGridIdx = resultItem->getGridIdx();
				}
				sprintf( buffer, "GemLevelUp(%d,%d,%d,%d)", msgbody.ArmSvr.Flag, msgbody.ArmSvr.IndexID, nListType, nGridIdx );
				GameManager::getSingleton().getScriptVM()->callString( buffer );
			}
			break;

		case GEM_ACT_OPEN_HOLE:
			{
				char buffer[256];
				sprintf( buffer, "GemOpenHole(%d)", msgbody.ArmSvr.Flag );
				GameManager::getSingleton().getScriptVM()->callString( buffer );
				m_bCanOpenHole = true;
			}
			break;

		case GEM_ACT_INLAY:
			{
				char buffer[256];
				sprintf( buffer, "GemInLay(%d,%d)", msgbody.ArmSvr.Flag, msgbody.ArmSvr.IndexID );
				GameManager::getSingleton().getScriptVM()->callString( buffer );
			}
			break;
		case GEM_ACT_PICK:
			{
				/* 宝石摘取 */
				if( msgbody.ArmSvr.Flag == 1 )
				{
					//m_SrcItem.updateItemData();
					GameManager::getSingleton().SendUIEvent( "GE_GEM_PICK_SUCESS" );
					ChatManager::getSingleton().showMidTip( MID_ARMPROCE_GEMPICK );
				}else{
					ChatManager::getSingleton().showMidTip( MID_ARMPROCE_GEMPICK_FAIL );
				}
			}
			break;
		case GEM_ADD_POWER:
			{
				char buffer[256];
				sprintf( buffer, "GemChargeResult(%d)", msgbody.ArmSvr.Flag );
				GameManager::getSingleton().getScriptVM()->callString( buffer );
			}
			break;
		case GEM_RESET_ATTR:
			{
				char buffer[256];
				sprintf( buffer, "GemResetResult(%d,%d,%d,%d)",msgbody.ArmSvr.Flag, msgbody.ArmSvr.ValIdx, msgbody.ArmSvr.AttrIdx,msgbody.ArmSvr.Mul );
				GameManager::getSingleton().getScriptVM()->callString( buffer );
			}
			break;
		case GEM_RESET_ATTR_SAVE:
			{
				char buffer[256];
				sprintf( buffer, "GemSaveResetResult(%d)", msgbody.ArmSvr.Flag );
				GameManager::getSingleton().getScriptVM()->callString( buffer );
			}
			break;
		case ARM_BIND:
			{
				if( msgbody.ArmSvr.Flag == 1 )
				{
					GameManager::getSingleton().SendUIEvent("GE_ARMBIND_SUCESS");
				}
			}
			break;
		case ARM_UNBIND:
			{
				if( msgbody.ArmSvr.Flag == 1 )
				{
					GameManager::getSingleton().SendUIEvent("GE_ARMUNBIND_SUCESS");
				}
			}
			break;
		case ARM_MOVE_STAR:
			{
				if( msgbody.ArmSvr.Flag == 1 )
				{
					GameManager::getSingleton().SendUIEvent("GE_ARMMOVESTAR_SUCESS");
				}
			}
			break;

		case ARM_UNFENGYIN_PRE:
			{
				char szScripts[256];
				sprintf( szScripts, "GetFengYinEquiPlayerInfo(\"%s\")", msgbody.ArmSvr.RoleName );
				GameManager::getSingleton().getScriptVM()->callString( szScripts );
			}
			break;

		case ARM_UNFENGYIN:
			{
				char buffer[256];
				sprintf( buffer, "ArmUnFengYinResult( %d )", msgbody.ArmSvr.Flag );
				GameManager::getSingleton().getScriptVM()->callString( buffer );
			}
			break;
		case SPAR_SORB:
			{
				char buffer[256];
				sprintf( buffer, "ArmSparSorResult( %d )", msgbody.ArmSvr.Flag );
				GameManager::getSingleton().getScriptVM()->callString( buffer );
			}
			break;
		case RUNE_UP_SVR_RES:
			{
				char buffer[256];
				sprintf( buffer, "SkillRuneCombineResult( %d,%d )", msgbody.ArmSvr.Flag, msgbody.ArmSvr.IndexID );
				GameManager::getSingleton().getScriptVM()->callString( buffer );
			}
			break;
		case ARM_ADDATTR_MOVE:
			{
				char buf[256];
				sprintf( buf, "ArmAttrMoveSuccess(%d,%d,%d)",msgbody.ArmSvr.Flag,msgbody.ArmSvr.IndexID,msgbody.ArmSvr.WID );
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;
		case ARM_ATTR_RESET:
			{
				char buf[256];
				sprintf( buf, "ArmAttrResetSuccess(%d)", msgbody.ArmSvr.Flag );
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;
		case ARM_ACT_CHECK:
			{
				char buf[256];
				sprintf( buf, "ArmIdentifySuccess(%d)", msgbody.ArmSvr.Flag );
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;
		case ARM_ACT_STEP_UP:
			{
				char buf[256];
				sprintf( buf, "ArmStepUpResult(%d)", msgbody.ArmSvr.Flag );
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;
		case ARM_BASE_ATTR_RESET:
			{
				if( msgbody.ArmSvr.Flag == 1 )
				{
					static_assert( sizeof(tagAddAttrInst_ToLua) == sizeof(tagAddAttrInst) );
					for ( int i=0; i < msgbody.ArmSvr.AttrNum; ++i )
					{
						m_ArmBaseResetResult[i].AddAttrID	= msgbody.ArmSvr.AddAttr[i].AddAttrID;
						m_ArmBaseResetResult[i].Val			= msgbody.ArmSvr.AddAttr[i].Val;
					}					
				}
				char buf[256];
				sprintf( buf, "BaseAttrResetResult(%d,%d)", msgbody.ArmSvr.Flag, msgbody.ArmSvr.AttrNum );
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;
		case ARM_BASE_ATTR_RESET_SAVE:
			{
				char buf[256];
				sprintf( buf, "BaseAttrSaveResult(%d)", msgbody.ArmSvr.Flag );
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;
		case ARM_SKILL_ACTIVE:
			{
				char buf[256];
				sprintf( buf, "ArmSkillActiveResult(%d)", msgbody.ArmSvr.Flag );
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break; 
		/*case ARM_LEGEND_UP:
			{
				char buf[256];
				sprintf( buf, "ArmLegendUpResult(%d)", msgbody.ArmSvr.Flag );
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;*/
		case ARM_BACK_UNREAL:
			{
				char buf[256];
				sprintf( buf, "UnRealBackResult(%d)", msgbody.ArmSvr.Flag );
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;
		case  ARM_SKILL_RECAST_ACTIVE:
			{
				char buf[256];
				sprintf( buf, "ArmSKillRecreateResult(%d)", msgbody.ArmSvr.Flag );
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;
		case  ARM_SKILL_RECAST_LOCK:
			{
				char buf[256];
				sprintf( buf, "ArmSKillRecreateResult(%d)", msgbody.ArmSvr.Flag );
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;
		case  ARM_SKILL_RECAST_RESET:
			{
				char buf[256];
				sprintf( buf, "ArmSKillRecreateResult(%d)", msgbody.ArmSvr.Flag );
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;
		}
	}
	else if( msgid == BACK_SVR ) 
	{
		switch( msgbody.BackSvr.Type )
		{
		case BACK_SVR_LEVELUP_SUCC:
			{
				char buf[256];
				sprintf( buf, "BackLevelUpSuccess(%d)", msgbody.BackSvr.Data.LevelUPSucc.Succ );
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;
		case BACK_SVR_CHGTYPE_SUCC:
			{
				char buf[256];
				sprintf( buf, "BackChangeTypeSuccess(%d)", msgbody.BackSvr.Data.ChgTypeSucc.Succ );
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;
		}
	}
	return 0;
}	

// 添加目标物品
int GameArmProce::addSrcItem( Item& item, int nType )
{
	// 物品不是装备类型
	if( item.getItemType() != ITEM_SLOTTYPE_AMM )
	{
		return MID_ARMPROCE_NOTEQUIPITEM;
	}
	
	switch( nType )
	{
	case STUFF_NEEDHOLE:
		{
			// 是否是有孔装备
			if( item.getHoleNum() == 0 )
			{
				return MID_ARMPROCE_NOTHOLDEQUIP;
			}

			// 需要有可镶嵌孔
			int nInLayNum	= 0;
			int nHoleNum = item.getHoleNum();
			for( int i = 0; i < nHoleNum; i++ )
			{
				if( item.getHoleGemId( i ) != 0 )
				{
					nInLayNum++;
				}
			}

			if( ( nHoleNum - nInLayNum ) <= 0  )
			{
				return MID_ARMPROCET_NOTHOLEINLAY;
			}
		}
		break;
	case STUFF_NEEDINLAYGEM:
		{
			// 是否是有孔装备
			if( item.getHoleNum() == 0 )
			{
				return MID_ARMPROCE_NOTHOLDEQUIP;
			}

			// 装备需要镶嵌宝石
			int nHoleNum = item.getHoleNum();
			bool bFound = false;
			for( int i = 0; i < nHoleNum; i++ )
			{
				if( item.getHoleGemId( i ) != 0 )
				{
					bFound = true;
				}
			}
			if( !bFound )
			{
				return MID_ARMPROCET_NOTINLAYGEM;
			}
		}
		break;
	case STUFF_INDENTITY:
		{
			// 装备不可鉴定
			if( item.getItemDef()->IdentifiabilityType == 0 )
			{
				return MID_ARMPROCET_CANNOTINDENTITY;
			}
		}
	    break;
	case STUFF_EQUIP:
		{
			// 装备未鉴定
			tagItemDef_ToLua* pItemDef = item.getItemDef();
			if( pItemDef->IdentifiabilityType > 0 )
			{
				return MID_ARMPROCET_NOTINDENTITYEQUIP ;
			}
		}
	    break;
	case STUFF_CANLEVEL:
		{
			// 装备未鉴定
			if( item.getItemDef()->IdentifiabilityType > 0 )
			{
				return MID_ARMPROCET_NOTINDENTITYEQUIP ;
			}
			if( item.getItemDef()->NextItemLevel == 0  )
			{
				return MID_ARMPROCET_NOTNEXTITEMLEVEL ;
			}
		}
		break;
	default:
	    break;
	}
	m_SrcItem = item;
	return 0;
}

// 添加装备提升所需要材料
int GameArmProce::addStuff( Item& item, int nIndex, int nType )
{
	const tagGemDef* pGemDef = &(DBDataManager::getSingleton().getGemDef( item.getItemId() ));
	switch( nType )
	{
	case STUFF_INLAYGEM:
		{
			if( m_SrcItem.getItemId() == 0)
			{
				// 提示没有放入装备
				return MID_ARMPROCE_NOTEQUIP;
			}

			if( pGemDef == NULL )
			{
				// 提示不是宝石
				return MID_ARMPROCE_NOTGEM;
			}

			if( pGemDef->GemEffectType != GEM_EFFECT_INLAY )
			{
				// 提示不是镶嵌宝石
				return MID_ARMPROCE_NOTGEMEFFECTINLAY;
			}

			if( m_SrcItem.getHoleGemColor( nIndex ) != pGemDef->GemColor )
			{
				// 提示该孔颜色跟宝石颜色不匹配
				return MID_ARMPROCET_HOLECOLORDIFWITHGEM;
			}

			if ((m_SrcItem.getItemDef()->WearPre.RoleLevelMin < 30 && pGemDef->GemLevel > 1) ||
				(m_SrcItem.getItemDef()->WearPre.RoleLevelMin < 50 && pGemDef->GemLevel > 2) ||
				(m_SrcItem.getItemDef()->WearPre.RoleLevelMin < 70 && pGemDef->GemLevel > 3) ||
				(m_SrcItem.getItemDef()->WearPre.RoleLevelMin < 80 && pGemDef->GemLevel > 4) ||
				(m_SrcItem.getItemDef()->WearPre.RoleLevelMin < 90 && pGemDef->GemLevel > 5) )
			{
				// 宝石等级与装备等级不符
				return MID_ARMPROCET_INLAY_LEVELLIMIT;
			}

			int nHoleNum = m_SrcItem.getHoleNum();
			for( int i = 0; i < nHoleNum; i++ )
			{
				const tagGemDef* pSrcGemDef = &DBDataManager::getSingleton().getGemDef( m_SrcItem.getHoleGemId(i) );
				if( i != nIndex 
					&& m_SrcItem.getHoleGemId( i ) != 0 
					&& ( m_SrcItem.getHoleGemId( i ) == item.getItemId() || pSrcGemDef->GemType == pGemDef->GemType ) )
				{
					// 提示该装备已镶嵌过这种宝石
					return MID_ARMPROCET_HOLEHASSAMEGEM;
				}
			}
			
			for( int i = 0; i < MAX_STUFF_ARMPROCE; i++ )
			{
				const tagGemDef* pStuffGemDef = &DBDataManager::getSingleton().getGemDef( m_Stuff[i].getItemId() );
				if( pStuffGemDef != NULL && pGemDef->GemType == pStuffGemDef->GemType )
				{
					// 提示待镶嵌列表已有同类型宝石
					return MID_ARMPROCE_SAMEGEMTYPEINLAY;
				}
			}
		}
		break;
	case STUFF_LEVELGEM:
		{
			//if( pGemDef == NULL )
			//{
			//	// 提示不是宝石
			//	return MID_ARMPROCE_NOTGEM;
			//}

			//if( getGemType( item.getItemId() ) != GEM_EFFECT_INLAY )
			//{
			//	// 非镶嵌类宝石不可升级
			//	return MID_ARMPROCE_NOTINLAYGEM;
			//}
		
			//int nLv	  = -1;
			//int nType = -1;
			//getGemTypeLv(  &nType, &nLv );
			//if( nType != -1 || nLv != -1 )
			//{
			//	if( nLv != pGemDef->GemLevel )
			//	{
			//		// 提示宝石等级不一致
			//		return MID_ARMPROCE_NOTSAMELEVEL;
			//	}
			//}
		}
		break;
	case STUFF_STARITEM:
		{
			if( m_SrcItem.getItemId() == 0)
			{
				// 提示没有放入装备
				return MID_ARMPROCE_NOTEQUIP;
			}

			if( getGemType( item.getItemId() ) != GEM_EFFECT_STAR )
			{
				// 提示不是品质升级材料
				return MID_ARMPROCET_NOTSTARSTUFF;
			}
		}
	    break;
	case STUFF_FIGHTITEM:
		{
			if( m_SrcItem.getItemId() == 0)
			{
				// 提示没有放入装备
				return MID_ARMPROCE_NOTEQUIP;
			}

			if( getGemType( item.getItemId() ) != GEM_EFFECT_FIGHT )
			{
				// 提示不是战斗力升级材料
				return MID_ARMPROCET_NOTFIGHTSTUFF;
			}
		}
		break;
	case STUFF_LEVELITEM:
		{
			if( m_SrcItem.getItemId() == 0)
			{
				// 提示没有放入装备
				return MID_ARMPROCE_NOTEQUIP;
			}

			if( getGemType( item.getItemId() ) != GEM_EFFECT_LEVEL )
			{
				// 提示不是等级升级材料
				return MID_ARMPROCET_NOTEQUIPLEVELSTUFF;
			}
		}
		break;
	default:
	    break;
	}
	m_Stuff[nIndex] = item;
	return 0;
}

// 删除装备提升所需要材料
void GameArmProce::delStuff( int nIndex )
{
	m_Stuff[nIndex].releaseItem();
}

// 删除待提升装备
void GameArmProce::delSrcItem()
{
	m_SrcItem.releaseItem();
}

// 清空材料栏
void GameArmProce::clear()
{
	for( int i = 0; i < MAX_STUFF_ARMPROCE; i++ )
	{
		m_Stuff[i].releaseItem();
	}
	m_SrcItem.releaseItem();
}

// 获取目标物品
Item& GameArmProce::getSrcItem()
{
	return m_SrcItem;
}

// 材料是否为空
bool GameArmProce::isStuffItemEmpty()
{
	for ( int i = 0; i < MAX_STUFF_ARMPROCE; ++i )
	{
		if ( m_Stuff[i].getItemId() != 0 )
		{
			return false;
		}
	}

	return true;
}

// 获取添加的材料
Item& GameArmProce::getStuffItem( int nIndex )
{
	return m_Stuff[nIndex];
}

// 获取所需宝石升级符ID
int GameArmProce::getGemUpdateId()
{
	for( int i = 0; i < MAX_STUFF_ARMPROCE; i++ )
	{
		if( m_Stuff[i].getItemId() != 0 )
		{
			const tagGemDef* pGemDef = &DBDataManager::getSingleton().getGemDef( m_Stuff[i].getItemId() );
			if( pGemDef != NULL )
			{
				return pGemDef->LevelUpItemID;
			}
		}
	}

	return 0;
}

// 获取宝石摘取所需物品ID
int GameArmProce::getGemPickUpUsedItem( int nGemId )
{
	const tagGemDef* pGemDef = &DBDataManager::getSingleton().getGemDef( nGemId );
	if( pGemDef != NULL )
	{
		return pGemDef->PickItemID;
	}

	return 0;
}

// 获取宝石的升级概率
int GameArmProce::getGemSuccessRate(  int index, int nGemId )
{
	const tagGemDef* pGemDef = &DBDataManager::getSingleton().getGemDef( nGemId );
	if( pGemDef != NULL )
	{
		assert( index <= MAX_GEM_ATTR_LEVEL && index >= 0 );
		return pGemDef->GemAttr[index].GemRate;
	}

	return 0;
}

// 获取宝石的升级花费
unsigned int GameArmProce::getGemCostMoney(  int index, int nGemId )
{
	const tagGemDef* pGemDef = &DBDataManager::getSingleton().getGemDef( nGemId );
	if( pGemDef != NULL )
	{
		assert( index <= MAX_GEM_ATTR_LEVEL && index >= 0 );
		return pGemDef->GemAttr[index].GemMoney;
	}

	return 0;
}

// 获取放入宝石的类型和等级
void GameArmProce::getGemTypeLv( int* nType, int* nLv )
{
	for( int i = 0; i < MAX_STUFF_ARMPROCE; i++ )
	{
		const tagGemDef* pGemDef = &DBDataManager::getSingleton().getGemDef( m_Stuff[i].getItemId() );
		if( pGemDef != NULL )
		{
			*nType	= pGemDef->GemType;
			*nLv	= pGemDef->GemLevel;
			return;
		}
	}
}

// 获取宝石的类型
int GameArmProce::getGemType( int nGemId )
{
	const tagGemDef* pGemDef = &DBDataManager::getSingleton().getGemDef( nGemId );
	if( pGemDef != NULL )
	{
		return pGemDef->GemEffectType;
	}

	return -1;
}

// 获取宝石的颜色
int GameArmProce::getGemColor( int nGemId )
{
	const tagGemDef* pGemDef = &DBDataManager::getSingleton().getGemDef( nGemId );
	if( pGemDef != NULL )
	{
		return pGemDef->GemColor;
	}

	return -1;
}

// 获取宝石定义
const tagGemDef_ToLua* GameArmProce::getGemDef( int nItemID )
{
	for ( std::vector<tagGemDef*>::iterator iter = m_gemDef.begin(), end = m_gemDef.end(); iter != end; ++iter )
	{
		tagGemDef* pGemDef = *iter;
		if ( pGemDef->ItemID == nItemID )
		{
			return (tagGemDef_ToLua*)pGemDef;
		}
	}

	return NULL;
}
//获取新宝石定义
const tagGemNewDef_ToLua* GameArmProce::getGemNewDef( int nItemID )
{
	static_assert( sizeof(tagGemNewDef) == sizeof(tagGemNewDef_ToLua) );
	for ( std::vector<tagGemNewDef*>::iterator iter = m_gemNewDef.begin(),end = m_gemNewDef.end(); iter != end; ++iter )
	{
		tagGemNewDef* pGemNewDef = *iter;
		if ( pGemNewDef->ItemID == nItemID )
		{
			return (tagGemNewDef_ToLua*)pGemNewDef;
		}
	}
	return NULL;
}
const tagGemAttrStarDef_ToLua* GameArmProce::getGemAttrStarDef( int nAttrID, int nLevel, int nStar )
{
	return (tagGemAttrStarDef_ToLua*) DBDataManager::getSingleton().getGemAttrStarDef( nAttrID, nLevel, nStar );
}
const tagGemLevelStarDef_ToLua* GameArmProce::getGemLevelStarDef( int nLevel, int nStar )
{
	return (tagGemLevelStarDef_ToLua*) DBDataManager::getSingleton().getGemLevelStarDef( nLevel, nStar );
}
const tagUseDef_ToLua* GameArmProce::getUseDef( int nID )
{
	static_assert( sizeof(tagUseDef_ToLua) == sizeof(tagUseDef) );
	return (tagUseDef_ToLua*) DBDataManager::getSingleton().getUseDef( nID );
}
// 是否是宝石
bool GameArmProce::isGem( int nItemID )
{
	for ( std::vector<tagGemDef*>::iterator iter = m_gemDef.begin(), end = m_gemDef.end(); iter != end; ++iter )
	{
		tagGemDef* pGemDef = *iter;
		if ( pGemDef->ItemID == nItemID )
		{
			return true;
		}
	}

	return false;
}

int GameArmProce::getStarNeedStuffNum( int nStar )
{
	const tagArmRandDef* pArmRandDef = DBDataManager::getSingleton().getArmRandDef( nStar,1 );
	if( pArmRandDef == NULL )
	{
		return -1;
	}
	return pArmRandDef->ArmStarDelGem;
}

int GameArmProce::getStarNeedMoney( int nStar )
{
	const tagArmRandDef* pArmRandDef = DBDataManager::getSingleton().getArmRandDef( nStar,1 );
	if( pArmRandDef == NULL )
	{
		return -1;
	}
	return pArmRandDef->ArmStarMoney;
}

int GameArmProce::getBindNeedMoney( int nStar )
{
	const tagArmRandDef* pArmRandDef = DBDataManager::getSingleton().getArmRandDef( nStar,2 );
	if( pArmRandDef == NULL )
	{
		return -1;
	}
	return pArmRandDef->ArmBindMoney;
}

int GameArmProce::getMoveStarNeedStuffNum( int nStar )
{
	const tagArmRandDef* pArmRandDef = DBDataManager::getSingleton().getArmRandDef( nStar,3 );
	if( pArmRandDef == NULL )
	{
		return -1;
	}
	return pArmRandDef->ArmMoveStarDelGem;
}

int GameArmProce::getUnBindNeedGold( int nStar )
{
	const tagArmRandDef* pArmRandDef = DBDataManager::getSingleton().getArmRandDef( nStar,4 );
	if( pArmRandDef == NULL )
	{
		return -1;
	}
	return pArmRandDef->ArmUnBindGodcoin;
}

int GameArmProce::getBackStar( int nStar )
{
	const tagArmRandDef* pArmRandDef = DBDataManager::getSingleton().getArmRandDef( nStar,1 );
	if( pArmRandDef == NULL )
	{
		return -1;
	}
	return pArmRandDef->ArmStarBackLevel;
}

const tagGemDef_ToLua* GameArmProce::getNextHeChengGemDef( const tagGemDef_ToLua* pCurGemDef )
{
	const std::vector<tagGemDef*> oneTypeGemVec = m_gemTypePartion[pCurGemDef->GemType];
	for ( std::vector<tagGemDef*>::const_iterator citer = oneTypeGemVec.begin(), cend = oneTypeGemVec.end();
			citer != cend; ++citer )
	{
		const tagGemDef* pOneGemDef = *citer;
		if ( pOneGemDef->GemLevel == pCurGemDef->GemLevel + 1 )
		{
			return (tagGemDef_ToLua*)pOneGemDef;
		}
	}

	return NULL;
}

const tagArmFenJieDef_ToLua* GameArmProce::getArmFenJieDef( int nLevel, int nImportant )
{
	static_assert( sizeof(tagArmFenJieDef_ToLua) == sizeof(tagArmFenJieDef) );
	const tagArmFenJieDef* mArmFenJieDef =  DBDataManager::getSingleton().getArmFenJieDef( nLevel, nImportant );
	return (tagArmFenJieDef_ToLua*)mArmFenJieDef;
}

void GameArmProce::requestContainerResolvePre( tdr_ulonglong WId, char ListType, unsigned short GirdId, int nType  )
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if( !(pMain->getActState().GetState() == GSACT_STAND || pMain->getActState().GetState() == GSACT_MOVE) 
		|| pMain->m_Equip.isUsedTmp()|| pMain->isInMorph() ||pMain->getActState().getInState(GSACT_IN_USEDMACHINE)
		||pMain->getActState().getInState(GSACT_IN_CARRY) || !pMain->getBufStatus()->canUsedSpell() )
	{
		GameMsgPrint::getSingleton().showMidTip(MID_RESOLVE_WRONG_STATE);
		return;
	}else if( pMain->isInSkillTetanic()
		||pMain->getActState().getInState(GSACT_IN_CAST) )
	{
		GameMsgPrint::getSingleton().showMidTip(MID_RESOLVE_FAILED_INCASTING);
		return;
	}
	if( !pMain->canControl() )
	{
		GameMsgPrint::getSingleton().showMidTip(MID_RESOLVE_FAILED_CANNTCONTROL);
		return;
	}else if( pMain->isInRide() )
	{
		if( !pMain->m_Rider.isOwnRide() )
		{
			GameMsgPrint::getSingleton().showMidTip(MID_RESOLVE_WRONG_STATE);
			return;
		}
		pMain->m_Rider.requestRideDown();
	}
	if( !pMain->canStartChant( eResolve_Chant ) )
	{
		return;
	}
	pMain->CmdMoveStop();
	if ( pMain->getContainer().getContainerLockState() == CONTAINER_LOCK_STATE_LOCK )
	{
		GameMsgPrint::getSingleton().showMidTip(MID_RESOLVE_CONTAINER_LOCK);
		return;
	}

	tagCSPkgBody msgbody;
	msgbody.ArmClt.ArmCltType = ARM_FENJIE_PRE;
	msgbody.ArmClt.ArmCltData.ArmFenJiePre.ArmData.ArmIdxData.ArmListType = ListType;
	msgbody.ArmClt.ArmCltData.ArmFenJiePre.ArmData.ArmIdxData.ArmGridIdx = GirdId;
	msgbody.ArmClt.ArmCltData.ArmFenJiePre.ArmData.WId = WId;
	msgbody.ArmClt.ArmCltData.ArmFenJiePre.Type = nType;
	SendGameServerMsg(ARM_CLT,msgbody);
	pMain->startResolveChant( ListType, GirdId, WId, nType );

}

void GameArmProce::requestSparEpurate( tdr_ulonglong armWId, char armListType, unsigned short armGirdId, 
										   tdr_ulonglong itemWId, char itemListType, unsigned short itemGirdId )
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if( !(pMain->getActState().GetState() == GSACT_STAND || pMain->getActState().GetState() == GSACT_MOVE) 
		|| pMain->m_Equip.isUsedTmp()|| pMain->isInMorph() ||pMain->getActState().getInState(GSACT_IN_USEDMACHINE)
		||pMain->getActState().getInState(GSACT_IN_CARRY) || !pMain->getBufStatus()->canUsedSpell() )
	{
		GameMsgPrint::getSingleton().showMidTip(MID_RESOLVE_WRONG_STATE);
		return;
	}else if( pMain->isInSkillTetanic()
		||pMain->getActState().getInState(GSACT_IN_CAST) )
	{
		GameMsgPrint::getSingleton().showMidTip(MID_RESOLVE_FAILED_INCASTING);
		return;
	}
	if( !pMain->canControl() )
	{
		GameMsgPrint::getSingleton().showMidTip(MID_RESOLVE_FAILED_CANNTCONTROL);
		return;
	}else if( pMain->isInRide() )
	{
		if( !pMain->m_Rider.isOwnRide() )
		{
			GameMsgPrint::getSingleton().showMidTip(MID_RESOLVE_WRONG_STATE);
			return;
		}
		pMain->m_Rider.requestRideDown();
	}
	if( !pMain->canStartChant( eResolve_Chant ) )
	{
		return;
	}
	pMain->CmdMoveStop();
	if ( pMain->getContainer().getContainerLockState() == CONTAINER_LOCK_STATE_LOCK )
	{
		GameMsgPrint::getSingleton().showMidTip(MID_RESOLVE_CONTAINER_LOCK);
		return;
	}

	tagCSPkgBody msgbody;
	msgbody.ArmClt.ArmCltType = ARM_FENJIE;
	msgbody.ArmClt.ArmCltData.ArmFenJie.ArmData.ArmIdxData.ArmListType = armListType;
	msgbody.ArmClt.ArmCltData.ArmFenJie.ArmData.ArmIdxData.ArmGridIdx = armGirdId;
	msgbody.ArmClt.ArmCltData.ArmFenJie.ArmData.WId = armWId;
	msgbody.ArmClt.ArmCltData.ArmFenJie.TiLianData.ArmIdxData.ArmListType = itemListType;
	msgbody.ArmClt.ArmCltData.ArmFenJie.TiLianData.ArmIdxData.ArmGridIdx = itemGirdId;
	msgbody.ArmClt.ArmCltData.ArmFenJie.TiLianData.WId = itemWId;
	msgbody.ArmClt.ArmCltData.ArmFenJie.Type = 2;
	SendGameServerMsg(ARM_CLT,msgbody);
}

void GameArmProce::requestBreakContainerResolvePre()
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if( isResolvePre() )
	{
		pMain->requestBreakChant( eResolve_Chant );
	}
}

bool GameArmProce::isResolvePre()
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	return pMain->isInPre( eResolve_Chant );
}

void GameArmProce::stopContainerResolve()
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	pMain->onBreakChant( eResolve_Chant );
}
void GameArmProce::resetGem( int nHoleIdx, int nUseMoneyType, int nListType, int nGirdId )
{
	tagCSPkgBody msgbody;
	msgbody.ArmClt.ArmCltType = GEM_RESET_ATTR;
	msgbody.ArmClt.ArmCltData.GemReSet.ArmHoleIdx = nHoleIdx;
	msgbody.ArmClt.ArmCltData.GemReSet.UseType = nUseMoneyType;
	msgbody.ArmClt.ArmCltData.GemReSet.ArmData.ArmGridIdx = nGirdId;
	msgbody.ArmClt.ArmCltData.GemReSet.ArmData.ArmListType = nListType;
	SendGameServerMsg( ARM_CLT, msgbody );
}
void GameArmProce::saveResetGem( int nListType, int nGridIdx )
{
	tagCSPkgBody msgbody;
	msgbody.ArmClt.ArmCltType = GEM_RESET_ATTR_SAVE;
	msgbody.ArmClt.ArmCltData.GemReSetSave.ArmData.ArmGridIdx = nGridIdx;
	msgbody.ArmClt.ArmCltData.GemReSetSave.ArmData.ArmListType = nListType;
	SendGameServerMsg( ARM_CLT, msgbody );
}
tagGemAddPower_ToLua GameArmProce::getChargeGemAddPowerInfo()
{
	static_assert( sizeof(tagGemAddPower_ToLua) == sizeof(tagGemAddPower) );
	memset( &m_GemAddPowerInfo,0,sizeof( tagGemAddPower_ToLua ) );
	return m_GemAddPowerInfo;
}
int GameArmProce::getChargePerPowerUseMoney()
{
	const tagUseDef *UseDef = DBDataManager::getSingleton().getUseDef(USE_ID_POWER);
	if (UseDef == NULL)
	{
		return 0;
	}
	return UseDef->UseMoney;
}
void GameArmProce::chargeGem( tagGemAddPower_ToLua Info )
{
	tagCSPkgBody msgbody;
	memset( &msgbody,0,sizeof( tagCSPkgBody ) );
	msgbody.ArmClt.ArmCltType = GEM_ADD_POWER;
	msgbody.ArmClt.ArmCltData.GemAddPower.OpType = Info.OpType;
	msgbody.ArmClt.ArmCltData.GemAddPower.UseType = Info.UseType;
	msgbody.ArmClt.ArmCltData.GemAddPower.ArmData.ArmGridIdx = Info.ArmData.ArmGridIdx;
	msgbody.ArmClt.ArmCltData.GemAddPower.ArmData.ArmListType = Info.ArmData.ArmListType;
	msgbody.ArmClt.ArmCltData.GemAddPower.ArmHoleIdx = Info.ArmHoleIdx;
	msgbody.ArmClt.ArmCltData.GemAddPower.StuffNum = Info.StuffNum;
	for ( int i = 0; i < Info.StuffNum; i++ )
	{
		msgbody.ArmClt.ArmCltData.GemAddPower.StuffData[i].GridIdx = Info.StuffData[i].GridIdx;
		msgbody.ArmClt.ArmCltData.GemAddPower.StuffData[i].ItemID = Info.StuffData[i].ItemID;
		msgbody.ArmClt.ArmCltData.GemAddPower.StuffData[i].ItemNum = Info.StuffData[i].ItemNum;
		msgbody.ArmClt.ArmCltData.GemAddPower.StuffData[i].ListType = Info.StuffData[i].ListType;
	}
	SendGameServerMsg( ARM_CLT, msgbody );
}

void GameArmProce::normalRecastArm( int nItemId, int nListType, int nGridIndex, int nBind, int nMust )
{
	tagCSPkgBody msgbody;
	memset( &msgbody,0,sizeof(tagCSPkgBody) );
	msgbody.ArmClt.ArmCltType = ARM_RECAST;
	msgbody.ArmClt.ArmCltData.ArmRecast.OperateType = OPERATE_TYPE_ONCE;
	msgbody.ArmClt.ArmCltData.ArmRecast.ItemID = nItemId;
	msgbody.ArmClt.ArmCltData.ArmRecast.ArmInfo.ListType = nListType;
	msgbody.ArmClt.ArmCltData.ArmRecast.ArmInfo.Idx = nGridIndex;
	msgbody.ArmClt.ArmCltData.ArmRecast.Bind = nBind;
	msgbody.ArmClt.ArmCltData.ArmRecast.Must = nMust;
	SendGameServerMsg( ARM_CLT, msgbody ) ;
}

void GameArmProce::advanceRecastArm( int nType, int nListType, int nGridIndex )
{
	tagCSPkgBody msgbody;
	memset( &msgbody,0,sizeof(tagCSPkgBody) );
	msgbody.ArmClt.ArmCltType = ARM_RECAST;
	msgbody.ArmClt.ArmCltData.ArmRecast.OperateType = OPERATE_TYPE_ONCE_ADVANCE;
	msgbody.ArmClt.ArmCltData.ArmRecast.ConsumeType = nType;
	msgbody.ArmClt.ArmCltData.ArmRecast.ArmInfo.ListType = nListType;
	msgbody.ArmClt.ArmCltData.ArmRecast.ArmInfo.Idx = nGridIndex;
	SendGameServerMsg( ARM_CLT,msgbody );
}

void GameArmProce::batchRecastArm( int nType, int nListType, int nGridIndex )
{
	tagCSPkgBody msgbody;
	memset( &msgbody,0,sizeof(tagCSPkgBody) );
	msgbody.ArmClt.ArmCltType = ARM_RECAST;
	msgbody.ArmClt.ArmCltData.ArmRecast.OperateType = OPERATE_TYPE_BATCH;
	msgbody.ArmClt.ArmCltData.ArmRecast.ConsumeType = nType;
	msgbody.ArmClt.ArmCltData.ArmRecast.ArmInfo.ListType = nListType;
	msgbody.ArmClt.ArmCltData.ArmRecast.ArmInfo.Idx = nGridIndex;
	SendGameServerMsg( ARM_CLT,msgbody );

}

void GameArmProce::saveRecastArm( int nSelectIdx, int nListType, int nGridIdex )
{
	tagCSPkgBody msgbody;
	memset( &msgbody,0,sizeof(tagCSPkgBody) );
	msgbody.ArmClt.ArmCltType = ARM_RECAST_SAVE;
	msgbody.ArmClt.ArmCltData.ArmRecastSave.ArmInfo.ListType = nListType;
	msgbody.ArmClt.ArmCltData.ArmRecastSave.ArmInfo.Idx = nGridIdex;
	msgbody.ArmClt.ArmCltData.ArmRecastSave.selectIdx = nSelectIdx;
	SendGameServerMsg( ARM_CLT, msgbody );
}

int GameArmProce::getArmRecastArea( int resultID, int mul )
{
	const tagArmRecastDef* pDef =  DBDataManager::getSingleton().getArmRecastDef( resultID );
	if( pDef != NULL )
	{
		for( int i = 0 ; i < MAX_ARM_RECAST_LIST ; i++ )
		{
			if( mul <= pDef->RecastInfo[i].MaxnMul && mul >= pDef->RecastInfo[i].MinMul )
			{
				return i;
			}
		}
	}
	return 0;
}

void GameArmProce::requestArmWash( int stuffItemId, int armList, int armGrid, int nBind, int nLockFlag, int nSafeFlag, int nMust )
{
	tagCSPkgBody msgbody;
	memset( &msgbody, 0, sizeof( tagCSPkgBody ) );
	msgbody.ArmClt.ArmCltType = ARM_WASH_ADDATTR;
	msgbody.ArmClt.ArmCltData.ArmWash.ItemID = stuffItemId;
	msgbody.ArmClt.ArmCltData.ArmWash.ArmData.ArmListType = armList;
	msgbody.ArmClt.ArmCltData.ArmWash.ArmData.ArmGridIdx = armGrid;
	msgbody.ArmClt.ArmCltData.ArmWash.Bind = nBind;
	msgbody.ArmClt.ArmCltData.ArmWash.LockFlag = nLockFlag;
	msgbody.ArmClt.ArmCltData.ArmWash.SafeFlag = nSafeFlag;
	msgbody.ArmClt.ArmCltData.ArmWash.Must = nMust;
	SendGameServerMsg( ARM_CLT, msgbody );
}

void GameArmProce::requestArmWashExtent( int armList, int armGrid, int idx )
{
	tagCSPkgBody msgbody;
	memset( &msgbody, 0, sizeof( tagCSPkgBody ) );
	msgbody.ArmClt.ArmCltType = ARM_ATTR_ACTIVE;
	msgbody.ArmClt.ArmCltData.AttrActive.ArmData.ArmListType = armList;
	msgbody.ArmClt.ArmCltData.AttrActive.ArmData.ArmGridIdx = armGrid;
	msgbody.ArmClt.ArmCltData.AttrActive.Idx = idx;
	SendGameServerMsg( ARM_CLT, msgbody );
}

void GameArmProce::saveWashArm( int armList, int armGrid )
{
	tagCSPkgBody msgbody;
	memset( &msgbody, 0 , sizeof( tagCSPkgBody ) );
	msgbody.ArmClt.ArmCltType = ARM_WASH_ADDATTR_SAVE;
	msgbody.ArmClt.ArmCltData.ArmWashSave.ArmData.ArmListType = armList;
	msgbody.ArmClt.ArmCltData.ArmWashSave.ArmData.ArmGridIdx = armGrid;
	SendGameServerMsg( ARM_CLT, msgbody );
}

const tagSparDef_ToLua* GameArmProce::getSparDef( int id )
{
	static_assert( sizeof(tagSparDef_ToLua) == sizeof(tagSparDef) );
	return (const tagSparDef_ToLua*) DBDataManager::getSingleton().getSparDef( id );
}

bool GameArmProce::requestSparInlay( int nSrcList, int nSrcGrid, int nDestList, int nDestGrid )
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if( pMain == NULL )
	{
		return false;
	}
	Item& srcItem = pMain->getContainer().getItem( nSrcList, nSrcGrid );
	Item& destItem = pMain->getContainer().getItem( nDestList, nDestGrid );
	if( (srcItem.getItemId() > 0 && srcItem.getItemDef()->WearPre.RoleLevelMin > pMain->getLv()) ||
		( destItem.getItemId() > 0 && destItem.getItemDef()->WearPre.RoleLevelMin > pMain->getLv() ) )
	{
		GameMsgPrint::getSingleton().showMidTip(MID_USE_ITEM_FAILE_LVENOUGH);
		return false;
	}
	if( !GameWizard::getSingleton().isDo( GAMEWIZARD_FIRST_BLINK_GET_SPAR_CONTAINER_BTN ) )
	{
		GameWizard::getSingleton().doStep( GAMEWIZARD_FIRST_BLINK_GET_SPAR_CONTAINER_BTN );
	}
	tagCSPkgBody msgbody;
	memset( &msgbody, 0, sizeof( tagCSPkgBody ) );
	msgbody.ArmClt.ArmCltType = SPAR_INLAY;
	msgbody.ArmClt.ArmCltData.SparInlay.Src.ArmListType = nSrcList;
	msgbody.ArmClt.ArmCltData.SparInlay.Src.ArmGridIdx = nSrcGrid;
	msgbody.ArmClt.ArmCltData.SparInlay.Dsc.ArmListType = nDestList;
	msgbody.ArmClt.ArmCltData.SparInlay.Dsc.ArmGridIdx = nDestGrid;
	SendGameServerMsg( ARM_CLT, msgbody );
	return true;
}

tagSparSorbData_ToLua* GameArmProce::getSparSorbData()
{
	static_assert( sizeof(tagSparSorbData_ToLua) == sizeof(tagSparSorbData) );
	memset( &m_SparSorbData, 0 ,sizeof(SPARSORBDATA) );
	return ( tagSparSorbData_ToLua* )&m_SparSorbData;
}

void GameArmProce::requestSparSorb()
{
	tagCSPkgBody msgbody;
	memset( &msgbody, 0, sizeof( tagCSPkgBody ) );
	msgbody.ArmClt.ArmCltType = SPAR_SORB;
	memcpy( &msgbody.ArmClt.ArmCltData.SparSorb, &m_SparSorbData, sizeof(SPARSORBDATA) );
	SendGameServerMsg( ARM_CLT, msgbody );
}

int GameArmProce::getSparOpenLv( int nIdx )
{
	if( nIdx < (int)m_vSparAddLv.size() )
	{
		return m_vSparAddLv[nIdx];
	}
	return 0;
}

int GameArmProce::getSparSorbCostMoney()
{
	const tagUseDef *UseDef = DBDataManager::getSingleton().getUseDef(USE_ID_SPAR_SORB);
	if (UseDef == NULL)
	{
		return 0;
	}
	return UseDef->UseMoney;
}

const tagHighWashUseDef_ToLua* GameArmProce::getHighWashDef( int nNum )
{
	static_assert( sizeof(tagHighWashUseDef_ToLua) == sizeof(tagHighWashUseDef) );
	return ( const tagHighWashUseDef_ToLua* ) DBDataManager::getSingleton().getHighWashUseDef( nNum );
}

const tagBackLevelDef_ToLua* GameArmProce::getBackLevelDef( int id )
{
	static_assert( sizeof(tagBackLevelDef_ToLua) == sizeof(tagBackLevelDef) );
	return (const tagBackLevelDef_ToLua*) DBDataManager::getSingleton().getBackLevelDef( id );
}
int GameArmProce::getBackLevelID( int nType, int nLevel )
{
	std::vector<BACKLEVELDEF> backInfo;
	DBDataManager::getSingleton().loadBackLevelDef(backInfo);
	int nrecord = (int)backInfo.size();

	int count = 0;
	for(int i=0; i<nrecord; i++)
	{
		const BACKLEVELDEF &info = backInfo[i];
		if ( info.SubType == nType && info.Level == nLevel )
		{
			return info.ItemID;
		}
	}
	return 0;
}
void GameArmProce::requestArmAttrMove( int nSrcList, int nSrcGrid, int nDstList, int nDstGrid, int nUseList, int nUseGrid, int nType )
{
	tagCSPkgBody msgbody;
	memset( &msgbody, 0, sizeof(tagCSPkgBody) );
	msgbody.ArmClt.ArmCltType = ARM_ADDATTR_MOVE;
	msgbody.ArmClt.ArmCltData.AttrMove.SrcData.ArmGridIdx = nSrcGrid;
	msgbody.ArmClt.ArmCltData.AttrMove.SrcData.ArmListType = nSrcList;
	msgbody.ArmClt.ArmCltData.AttrMove.DstData.ArmGridIdx = nDstGrid;
	msgbody.ArmClt.ArmCltData.AttrMove.DstData.ArmListType = nDstList;
	msgbody.ArmClt.ArmCltData.AttrMove.UseData.ArmGridIdx = nUseGrid;
	msgbody.ArmClt.ArmCltData.AttrMove.UseData.ArmListType = nUseList;
	msgbody.ArmClt.ArmCltData.AttrMove.Type = nType;
	SendGameServerMsg( ARM_CLT, msgbody );
}

void GameArmProce::requestArmAttrReset( int nList, int nGrid, int nItemID, int nBind, int nIdx )
{
	tagCSPkgBody msgbody;
	memset( &msgbody, 0, sizeof(tagCSPkgBody) );
	msgbody.ArmClt.ArmCltType = ARM_ATTR_RESET;
	msgbody.ArmClt.ArmCltData.AttrReSet.ArmData.ArmGridIdx = nGrid;
	msgbody.ArmClt.ArmCltData.AttrReSet.ArmData.ArmListType = nList;
	msgbody.ArmClt.ArmCltData.AttrReSet.Idx = nIdx;
	msgbody.ArmClt.ArmCltData.AttrReSet.UseItemID = nItemID;
	msgbody.ArmClt.ArmCltData.AttrReSet.Bind = nBind;
	SendGameServerMsg( ARM_CLT, msgbody );
}

void GameArmProce::requestBackLevelUp( int nBackList, int nBackGrid,tdr_ulonglong backWId, int nItemList, int nItemGrid,tdr_ulonglong itemWId )
{
	tagCSPkgBody msgbody;
	memset( &msgbody, 0, sizeof(tagCSPkgBody) );
	msgbody.BackClt.Type = BACK_CLT_LEVELUP;
	msgbody.BackClt.Data.LevelUP.Back.ListType	= nBackList;
	msgbody.BackClt.Data.LevelUP.Back.GridIdx	= nBackGrid;
	msgbody.BackClt.Data.LevelUP.Back.WID		= backWId;

	msgbody.BackClt.Data.LevelUP.BeUsed.ListType	= nItemList;
	msgbody.BackClt.Data.LevelUP.BeUsed.GridIdx		= nItemGrid;
	msgbody.BackClt.Data.LevelUP.BeUsed.WID			= itemWId;

	SendGameServerMsg( BACK_CLT, msgbody );
}

void GameArmProce::requestBackChange( int nBackList, int nBackGrid, tdr_ulonglong backWId, int nItemList, int nItemGrid,tdr_ulonglong itemWId )
{
	tagCSPkgBody msgbody;
	memset( &msgbody, 0, sizeof(tagCSPkgBody) );
	msgbody.BackClt.Type = BACK_CLT_CHG_TYPE;
	msgbody.BackClt.Data.ChgType.Back.ListType	= nBackList;
	msgbody.BackClt.Data.ChgType.Back.GridIdx	= nBackGrid;
	msgbody.BackClt.Data.ChgType.Back.WID		= backWId;

	msgbody.BackClt.Data.ChgType.BeUsed.ListType	= nItemList;
	msgbody.BackClt.Data.ChgType.BeUsed.GridIdx		= nItemGrid;
	msgbody.BackClt.Data.ChgType.BeUsed.WID			= itemWId;

	SendGameServerMsg( BACK_CLT, msgbody );
}

void GameArmProce::requestArmStepUp( tagArmStepUp_ToLua Data )
{
	tagCSPkgBody msgbody;
	msgbody.ArmClt.ArmCltType = ARM_ACT_STEP_UP;
	msgbody.ArmClt.ArmCltData.StepUp.ArmNum = Data.ArmNum;
	msgbody.ArmClt.ArmCltData.StepUp.Src.ArmGridIdx = Data.Src.ArmGridIdx;
	msgbody.ArmClt.ArmCltData.StepUp.Src.ArmListType = Data.Src.ArmListType;
	for ( int i=0; i < Data.ArmNum; ++i )
	{
		msgbody.ArmClt.ArmCltData.StepUp.ArmStuffs[i].ArmGridIdx = Data.ArmStuffs[i].ArmGridIdx;
		msgbody.ArmClt.ArmCltData.StepUp.ArmStuffs[i].ArmListType = Data.ArmStuffs[i].ArmListType;
	}
	SendGameServerMsg( ARM_CLT, msgbody );
}

tagArmStepUp_ToLua GameArmProce::getArmStepUpData()
{
	static_assert( sizeof(tagArmStepUp_ToLua) == sizeof(tagArmStepUp) );
	memset( &m_ArmStepUpData, 0, sizeof(tagArmStepUp_ToLua) );
	return m_ArmStepUpData;
}

const tagArmStepUpDef_ToLua* GameArmProce::getArmStepUpDef( int nLv, int nImp, int nPos )
{
	static_assert( sizeof(tagArmStepUpDef_ToLua) == sizeof(tagArmStepUpDef) );
	return (const tagArmStepUpDef_ToLua*)DBDataManager::getSingleton().getArmStepUpDef( nLv, nImp, nPos );
}

void GameArmProce::requestArmBaseReset( tagArmBaseReSet_ToLua Data )
{
	tagCSPkgBody msgbody;
	msgbody.ArmClt.ArmCltType = ARM_BASE_ATTR_RESET;
	msgbody.ArmClt.ArmCltData.BaseReSet.ArmNum = Data.ArmNum;
	msgbody.ArmClt.ArmCltData.BaseReSet.Src.ArmGridIdx = Data.Src.ArmGridIdx;
	msgbody.ArmClt.ArmCltData.BaseReSet.Src.ArmListType = Data.Src.ArmListType;
	msgbody.ArmClt.ArmCltData.BaseReSet.UseItem.ArmGridIdx = Data.UseItem.ArmGridIdx;
	msgbody.ArmClt.ArmCltData.BaseReSet.UseItem.ArmListType = Data.UseItem.ArmListType;
	for ( int i=0; i< Data.ArmNum; ++i )
	{
		msgbody.ArmClt.ArmCltData.BaseReSet.ArmStuffs[i].ArmGridIdx = Data.ArmStuffs[i].ArmGridIdx;
		msgbody.ArmClt.ArmCltData.BaseReSet.ArmStuffs[i].ArmListType = Data.ArmStuffs[i].ArmListType;
	}
	SendGameServerMsg( ARM_CLT, msgbody );
}

void GameArmProce::requestArmBaseResetSave( int nList, int nGrid )
{
	tagCSPkgBody msgbody;
	msgbody.ArmClt.ArmCltType = ARM_BASE_ATTR_RESET_SAVE;
	msgbody.ArmClt.ArmCltData.BaseReSetSave.Src.ArmGridIdx = nGrid;
	msgbody.ArmClt.ArmCltData.BaseReSetSave.Src.ArmListType = nList;
	SendGameServerMsg( ARM_CLT, msgbody );
}

tagArmBaseReSet_ToLua GameArmProce::getArmBaseResetData()
{
	static_assert( sizeof(tagArmBaseReSet_ToLua) == sizeof(tagArmBaseReSet) );
	memset( &m_ArmBaseResetData, 0, sizeof(tagArmBaseReSet_ToLua) );
	return m_ArmBaseResetData;
}

const tagArmGradeValDef_ToLua* GameArmProce::getArmGradeValDef( int nImp )
{
	static_assert( sizeof(tagArmGradeValDef_ToLua) == sizeof(tagArmGradeValDef) );
	return (const tagArmGradeValDef_ToLua*)DBDataManager::getSingleton().getArmGradeValDef( nImp );
}

const tagArmReSetDef_ToLua* GameArmProce::getArmResetDef( int nLv )
{
	static_assert( sizeof(tagArmReSetDef_ToLua) == sizeof(tagArmReSetDef) );
	return (const tagArmReSetDef_ToLua*)DBDataManager::getSingleton().getArmResetDef( nLv );
}

tagAddAttrInst_ToLua& GameArmProce::getArmResetResult( int idx )
{
	assert( idx < MAX_ARM_ADD_ATTR );
	return m_ArmBaseResetResult[idx];
}

const tagArmSkillNumDef_ToLua* GameArmProce::getArmSkillNumDef( int nLv, int nImp, int nPos )
{
	static_assert( sizeof(tagArmSkillNumDef_ToLua) == sizeof(tagArmSkillNumDef) );
	return (const tagArmSkillNumDef_ToLua*)DBDataManager::getSingleton().getArmSkillNumDef( nLv, nImp, nPos );
}

const tagArmSkillAttrDef_ToLua* GameArmProce::getArmSkillAttrDef( int nId )
{
	static_assert( sizeof(tagArmSkillAttrDef_ToLua) == sizeof(tagArmSkillAttrDef) );
	return (const tagArmSkillAttrDef_ToLua*)DBDataManager::getSingleton().getArmSkillAttrDef( nId );
}

void GameArmProce::requestArmSkillActive( int idx, int nArmList, int nArmGird, int nUseList, int nUseGrid )
{
	tagCSPkgBody msgbody;
	msgbody.ArmClt.ArmCltType = ARM_SKILL_ACTIVE;
	msgbody.ArmClt.ArmCltData.SkillActive.Idx = idx;
	msgbody.ArmClt.ArmCltData.SkillActive.Src.ArmGridIdx = nArmGird;
	msgbody.ArmClt.ArmCltData.SkillActive.Src.ArmListType = nArmList;
	msgbody.ArmClt.ArmCltData.SkillActive.Use.ArmGridIdx = nUseGrid;
	msgbody.ArmClt.ArmCltData.SkillActive.Use.ArmListType = nUseList;
	SendGameServerMsg( ARM_CLT, msgbody );
}
void GameArmProce::requestArmUpOrange( int nUseList, int nUseGrid, int nArmList, int nArmGird, int nNum, int nOption )
{
	tagCSPkgBody msgbody;
	msgbody.ArmClt.ArmCltType = ARM_LEGEND_UP;
	msgbody.ArmClt.ArmCltData.LegendUp.Num = nNum;
	msgbody.ArmClt.ArmCltData.LegendUp.SrcScroll.ArmListType	= nUseList;
	msgbody.ArmClt.ArmCltData.LegendUp.SrcScroll.ArmGridIdx		= nUseGrid;
	msgbody.ArmClt.ArmCltData.LegendUp.SrcArm.ArmListType		= nArmList;
	msgbody.ArmClt.ArmCltData.LegendUp.SrcArm.ArmGridIdx		= nArmGird;
	int nIndex = 0;
	for ( int i=0; i < 4; i++ )
	{
		if ( nIndex >= ARM_LEGEND_MAX )
		{
			break;
		}
		if( bitAnd( nOption, (int) pow(2.0,i) ) )
		{
			msgbody.ArmClt.ArmCltData.LegendUp.Idxs[nIndex] = i;   // 索引值从0开始  0-3
			nIndex++;
		}	
	}
	SendGameServerMsg( ARM_CLT, msgbody );
}
void GameArmProce::requestUnRealBack( int nSrcList, int nSrcGrid, int nDesList, int nDesGrid )
{
	tagCSPkgBody msgbody;
	msgbody.ArmClt.ArmCltType = ARM_BACK_UNREAL;
	msgbody.ArmClt.ArmCltData.BackUnreal.Src.ArmListType = nSrcList;
	msgbody.ArmClt.ArmCltData.BackUnreal.Src.ArmGridIdx = nSrcGrid;
	msgbody.ArmClt.ArmCltData.BackUnreal.Dsc.ArmListType = nDesList;
	msgbody.ArmClt.ArmCltData.BackUnreal.Dsc.ArmGridIdx = nDesGrid;
	SendGameServerMsg( ARM_CLT, msgbody );
}
void GameArmProce::requestArmSkillActive( int srcGridIdx, int srcListType )
{
	tagCSPkgBody msgbody;
	msgbody.ArmClt.ArmCltType = ARM_SKILL_RECAST_ACTIVE;
	msgbody.ArmClt.ArmCltData.RecastActive.Src.ArmGridIdx = srcGridIdx;
	msgbody.ArmClt.ArmCltData.RecastActive.Src.ArmListType = srcListType;
	SendGameServerMsg( ARM_CLT, msgbody );
}
void GameArmProce::requestArmSkillLock( int nIdx, int srcGridIdx, int srcListType )
{
	tagCSPkgBody msgbody;
	msgbody.ArmClt.ArmCltType = ARM_SKILL_RECAST_LOCK;
	msgbody.ArmClt.ArmCltData.RecastLock.Idx = nIdx;
	msgbody.ArmClt.ArmCltData.RecastLock.Src.ArmGridIdx = srcGridIdx;
	msgbody.ArmClt.ArmCltData.RecastLock.Src.ArmListType = srcListType;
	SendGameServerMsg( ARM_CLT, msgbody );
}
void GameArmProce::requestArmSkillReset( int srcGridIdx, int srcListType, int stuffGridIdx, int stuffListType )	
{
	tagCSPkgBody msgbody;
	msgbody.ArmClt.ArmCltType = ARM_SKILL_RECAST_RESET;
	msgbody.ArmClt.ArmCltData.RecastReset.Src.ArmGridIdx = srcGridIdx;
	msgbody.ArmClt.ArmCltData.RecastReset.Src.ArmListType = srcListType;
	msgbody.ArmClt.ArmCltData.RecastReset.Stuff.ArmGridIdx = stuffGridIdx;
	msgbody.ArmClt.ArmCltData.RecastReset.Stuff.ArmListType = stuffListType;
	SendGameServerMsg( ARM_CLT, msgbody );
}
const tagArmSkillRecastDef_ToLua* GameArmProce::getArmSkillRecastDef( int nLv )
{
	static_assert( sizeof(tagArmSkillRecastDef_ToLua) == sizeof(tagArmSkillRecastDef) );
	return (const tagArmSkillRecastDef_ToLua*)DBDataManager::getSingleton().getArmSkillRecastDef( nLv );
}