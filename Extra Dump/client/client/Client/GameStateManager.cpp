#include "GameStateManager.h"
#include "OgreRoot.h"

int getGameState()
{
	return GameStateManager::getGameState();
}

int GameStateManager::getGameState()
{
	return g_GameState;
}
int GameStateManager::getLastGameState()
{
	return g_LastGameState;
}
void GameStateManager::changeGameState( int state )
{
	if (g_GameState != state )
	{
		g_LastGameState = g_GameState;
		g_GameState = state;
	}

	if (state == STATE_INWORLD)
	{
		Ogre::Root::getSingleton().setInWorld(true);
	}
	else 
		Ogre::Root::getSingleton().setInWorld(false);
}
