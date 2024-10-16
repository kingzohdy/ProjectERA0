#include "GameQiYue.h"
#include <assert.h>
#include <memory.h>
#include "GameEngineTypes.h"

GameQiYue::GameQiYue(void)
{
}

GameQiYue::~GameQiYue(void)
{
}

void GameQiYue::setQiYueList( const tagQiYueInfo& qiYueList )
{
	static_assert( sizeof( tagQiYueInfo ) == sizeof( tagQiYueInfo_ToLua ) );
	memcpy( &m_qiYueList, &qiYueList, sizeof( tagQiYueInfo_ToLua ) );
}

const tagQiYueInfo_ToLua& GameQiYue::getQiYueList( ) const
{
	return m_qiYueList;
}

const tagQiYueInfo_ToLua& GameQiYue::getQiYueList( )
{
	return m_qiYueList;
}
