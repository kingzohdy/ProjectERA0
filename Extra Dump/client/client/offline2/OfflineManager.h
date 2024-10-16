#ifndef __OfflineManager_H__
#define __OfflineManager_H__
#include "OgreSingleton.h"
#include "GameEngineTypes.h"

#define MAX_NAME 64
#define LOGIN_WIN_WIDTH 249
#define LOGIN_WIN_HEIGHT 632
#define CHAT_WIN_WIDTH 677
#define CHAT_WIN_HEIGHT 471
class MudNetMgr;
class FlickerManager;
class OfflineWatchPlayer;
class RelationManager;
class ChatRoomManager;
class OfflineMainPlayer;
class OfflineFairy;
//tolua_begin
enum OnlineType
{
	OFFLINE=0,
	CLIENT_ONLINE=1,
	MUD_ONLINE=2,
};
//tolua_end

void  addmessage(const char* content,int style);

class OfflineManager//tolua_export
	: public Ogre::Singleton<OfflineManager>
{//tolua_export
public:
	OfflineManager();
	~OfflineManager();
	
	//tolua_begin
	bool bitAnd( int nLhs, int nRhs );
	void end1();
	void end();
	void setsize(int i);
	void MoveWindowDown();
	void MoveWindowUp();
	void SendUIEvent(const char *pevent);
	void MiniWindow();
	void saveusername(const char* name);
	void clearusername();
	const char* getlastname();
	int	  getlastusercheck();
	void OpenUrl( const char* url );
	//tolua_end
	bool init(const char *config_file, void *hwnd,void *hinstance,bool bdirect,void* hMenu,const char* pdirectstart);
	int  OnWindowMsg(void *hwnd, unsigned int msgid, unsigned int wparam, unsigned int lparam);
	int  getGameState();
	void update();
	void setUIsize();
	bool initDB();
	void LoadServerListConfig(const char* fileName);
	void initRgn();
	void Destroy();
	int getWindowtopleftx();
	int getWindowtoplefty();
	void CallString(const char* pszStr);
	
	Ogre::SceneRenderer* getMainRender()
	{
		return m_pMainRenderer;
	}

	GameUI*	getGameUI()
	{
		return m_pGameUI;
	}

	Ogre::ScriptVM*	getScriptVM()
	{
		return m_pScriptVM;
	}
	void LoadLastUserName();
	void addtips(std::string strTips);
protected:
	void*					m_hwnd;
	void*					m_hinstance;
	GameConfig				m_Config;
	DBDataManager*			m_pDBMgr;
	GameUI*					m_pGameUI;
	Ogre::ScriptVM*			m_pScriptVM;
	Ogre::InputManager*		m_pInputMgr;
	MudNetMgr*				m_pmudMgr;
	unsigned int			m_lparam;
	FrameTick*				m_pFrameTick;
	FlickerManager*			m_flickMgr;
	char					m_UserName[MAX_NAME];
	ChatManager*			m_ChatMgr;
	ActorManager*			m_ActorMgr;
	OfflineWatchPlayer*		m_WatchPlayer;
	RelationManager*		m_RelationMgr;
	ChatRoomManager*		m_ChatRoomMgr;
	OfflineMainPlayer*		m_MainPlayer;
	Ogre::SceneRenderer*	m_pMainRenderer;

public:
	int						m_nLastSelectWorldId;
	int						m_nSaveUserName;
	bool					m_isRender;
};//tolua_export
#endif