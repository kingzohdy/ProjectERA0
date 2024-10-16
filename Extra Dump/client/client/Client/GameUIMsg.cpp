#include "GameUIMsg.h"

GameUIMsg::GameUIMsg(void)
{
}

GameUIMsg::~GameUIMsg(void)
{
}

GameUIMsg* GameUIMsg::Create()
{
	GameUIMsg* p = new GameUIMsg;
	return p;	
}

void GameUIMsg::setUIMsg( const tagUIMsgBody& body )
{
	m_UIMsg = body;
}

const tagUIMsgBody* GameUIMsg::getUIMsg()const
{
	return &m_UIMsg;
}
