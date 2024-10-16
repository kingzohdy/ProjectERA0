#pragma once

class GameStateManager
{
public:
	enum
	{
		STATE_INIT = 0,
		STATE_DIRLIST,
		STATE_ROLELIST,
		STATE_MAPLOADING,
		STATE_INWORLD
	};
	
	static int	getGameState();
	static int	getLastGameState();
	static void changeGameState( int state );
};

static int g_GameState = GameStateManager::STATE_INIT;
static int g_LastGameState = GameStateManager::STATE_INIT;

//tolua_begin
enum
{
	CLIENT_STATE_INIT		= GameStateManager::STATE_INIT,
	CLIENT_STATE_DIRLIST	= GameStateManager::STATE_DIRLIST,
	CLIENT_STATE_ROLELIST	= GameStateManager::STATE_ROLELIST,
	CLIENT_STATE_INWORLD	= GameStateManager::STATE_INWORLD
};
extern int getGameState();
//tolua_end