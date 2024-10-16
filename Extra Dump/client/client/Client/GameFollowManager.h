#pragma once
#include "OgreSingleton.h"
#include "GameEngineTypes.h"
#include <string>

//tolua_begin 
enum
{
	CLIENT_NORMAL_FOLLOW	= 0,
	CLIENT_CAPTAIN_FOLLOW	= 1,
	CLIENT_CONVOY_FOLLOW	= 2,
};

enum
{
	CLIENT_MAX_FOLLOW_MICROSECOND_INTERVAL = 4000,
};
//tolua_end

class FollowCamera;
class GameFollowManager//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<GameFollowManager>
{//tolua_export
public:
	GameFollowManager(void);
	~GameFollowManager(void);
	// c->s

	//tolua_begin 
	// 开始跟随某个玩家
	void	beginWeiXing( const char* pszName, int nMemId, int nFollowType = CLIENT_NORMAL_FOLLOW );
	// 取消跟随某个玩家
	virtual void	cancelWeiXing( );

	// 呼吁队员跟随对着那个
	void	appealWeiXing();

	bool	isWeiXing();

	bool	canStartFollow();

	bool	isWeiXingTarget( const char* pszName, int nMemId );

	const char*	weiXingTarget();
	//tolua_end
	void	resetFollowInfo();

	void	drawFollowFrame( );

	bool	needCancelFollow();

	// s->c
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	virtual void update(unsigned int dtick);
protected:
private:
	int				m_nMemID;		// 所跟随的玩家的内存ID
	std::string		m_strPlayerName;
	int				m_nPreMapID;	// 所跟随玩家在上一次跟随操作原来所处的地图ID 
	unsigned int	m_uLastTime;
};//tolua_export