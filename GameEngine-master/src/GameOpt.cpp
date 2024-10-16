#include "GameOpt.h"
#include <memory.h>
#include <assert.h>

GameSettingConfig::GameSettingConfig()
{
	memset( m_szSettingData, 0, MAX_GAME_SET_OPT * sizeof( char ) );
}

void GameSettingConfig::setGameOpt( int nIndex, char chValue )
{
	assert( nIndex < MAX_GAME_SET_OPT );
	m_szSettingData[nIndex] = chValue;
}

bool GameSettingConfig::getGameOpt( int nIndex )
{
	assert( nIndex < MAX_GAME_SET_OPT );
	return m_szSettingData[nIndex] == 0 ?false:true;
}

char GameSettingConfig::getGameOptValue( int nIndex )
{
	assert( nIndex < MAX_GAME_SET_OPT );
	return m_szSettingData[nIndex];
}
