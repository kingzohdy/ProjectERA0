#include "GameCardBox.h"
#include "proto_cs.h"
#include "GameEngineTypes.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include "GameMsgPrint.h"
#include "GameDBData.h"
#include "GameUIMsg.h"

GameCardBox::GameCardBox(void)
{
	INetworkManager::getSingleton().RegisterMsgHandler( CARD_BOX_SVR, this );
}

GameCardBox::~GameCardBox(void)
{

}

int GameCardBox::HandleNetMsg( int msgid, const tagCSPkgBody& msgbody )
{
	if ( msgid != CARD_BOX_SVR )
	{
		return 0;
	}
	switch( msgbody.CardBoxSvr.Type )
	{
	case CARDBOX_SVR_TYPE_BOX_INFO:
		{
			static( sizeof(tagCSCardBoxInfo_ToLua) == sizeof(tagCSCardBoxInfo) );
			tagUIMsgBody UI_Msg;
			memcpy( &UI_Msg.CardBoxInfo, &msgbody.CardBoxSvr.Data.CardBoxInfo, sizeof(tagCSCardBoxInfo_ToLua) );
			GameUIMsg::getSingleton().setUIMsg( UI_Msg );
			GameManager::getSingleton().SendUIEvent( "GE_CARDBOX_INFO" );
		}
		break;
	case CARDBOX_SVR_TYPE_SELECT_INFO:
		{
			static( sizeof(tagCSCardBoxSelectInfo_ToLua) == sizeof(tagCSCardBoxSelectInfo) );
			tagUIMsgBody UI_Msg;
			memcpy( &UI_Msg.CardBoxSelectInfo, &msgbody.CardBoxSvr.Data.SelectInfo, sizeof(tagCSCardBoxSelectInfo_ToLua) );
			GameUIMsg::getSingleton().setUIMsg( UI_Msg );
			GameManager::getSingleton().SendUIEvent( "GE_CARDBOX_SELECT" );
		}
		break;
	case CARDBOX_SVR_TYPE_CLOSE:
		{
			GameManager::getSingleton().getScriptVM()->callString( "CloseCardBoxFrame()" );
		}
		break;
	default:
		break;
	}
	return 0;
}

void GameCardBox::requestOpenCardBox( int nGridIdx, int nListType )
{
	tagCSPkgBody msgbody;
	msgbody.CardBoxClt.Type = CARDBOX_CLT_TYPE_OPEN;
	msgbody.CardBoxClt.Data.Open.GridIdx = nGridIdx;
	msgbody.CardBoxClt.Data.Open.ListType = nListType;
	SendGameServerMsg( CARD_BOX_CLT, msgbody );
}

void GameCardBox::requestCloseCardBox()
{
	tagCSPkgBody msgbody;
	msgbody.CardBoxClt.Type = CARDBOX_CLT_TYPE_CLOSE;
	SendGameServerMsg( CARD_BOX_CLT, msgbody );
}

void GameCardBox::requestWashCard()
{
	tagCSPkgBody msgbody;
	msgbody.CardBoxClt.Type = CARDBOX_CLT_TYPE_WASH;
	SendGameServerMsg( CARD_BOX_CLT, msgbody );
}

void GameCardBox::requestSelectCard( int nGridIdx )
{
	tagCSPkgBody msgbody;
	msgbody.CardBoxClt.Type = CARDBOX_CLT_TYPE_SELECT;
	msgbody.CardBoxClt.Data.Select.GridIdx = nGridIdx;
	SendGameServerMsg( CARD_BOX_CLT, msgbody );
}

void GameCardBox::requestNextCard()
{
	tagCSPkgBody msgbody;
	msgbody.CardBoxClt.Type = CARDBOX_CLT_TYPE_NEXT;
	SendGameServerMsg( CARD_BOX_CLT, msgbody );
}

const tagCardBoxDef_ToLua* GameCardBox::getCardBoxDef( int id )
{
	static_assert( sizeof(tagCardBoxDef_ToLua) == sizeof(tagCardBoxDef) );
	return (const tagCardBoxDef_ToLua*)DBDataManager::getSingleton().getCardBoxDef( id );
}