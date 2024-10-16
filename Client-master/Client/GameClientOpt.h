#pragma once
#include "OgreSingleton.h"
#include <map>
#include "proto_comm.h"

enum
{
	SAVE_OPT_DIRECT		= 0,	// 新手指引
	SAVE_OPT_SHORCUT,			// 快捷栏
	SAVE_OPT_SPSHORCT,			// 组合键
	SAVE_OPT_GAMESETTING,		// 游戏设置 
	SAVE_OPT_CAMERADIR,			// 进副本前的镜头朝向 
	SAVE_OPT_TASKTRACE,			// 任务追踪设置 
	SAVE_OPT_PET_AUTO_USE_ITEM,	// 宠物自动使用药品
	SAVE_OPT_LAST_COMMIT_MAIL_TIME,	// 上次提交邮件的时间

	SAVE_OPT_LAST_PING_BI_WORLD_PIN_DAO,		// 上次所选择屏蔽的世界频道
	SAVE_OPT_LAST_PING_BI_CLAN_PIN_DAO,			// 上次所选择屏蔽的公会频道
	SAVE_OPT_LAST_PING_BI_ZONG_HE_PIN_DAO,		// 上次所选择屏蔽的综合频道
	SAVE_OPT_LAST_PING_BI_TEAM_PIN_DAO,			// 上次所选择屏蔽的队伍频道
	SAVE_OPT_LAST_PING_BI_NEAR_PIN_DAO,			// 上次所选择屏蔽的附近频道
	SAVE_OPT_LAST_PING_BI_PRIVATE_PIN_DAO,		// 上次所选择屏蔽的私聊频道
	SAVE_OPT_MARKET_LATEST_GOLD_COIN_BUY,		// 最近金币商城购买列表 
	SAVE_OPT_MARKET_LATEST_TICK_BUY,			// 最近金券商城购买列表
	SAVE_OPT_AUTO_ASSIST,						// 自动辅助功能
	SAVE_OPT_SHORTCUT_FULL_SKILL_INDEX,			// 快捷栏Full后，获得技能拖拉索引
	SAVE_OPT_VIP_AUTO_ASSIST,					// VIP高级辅助
	SAVE_OPT_AUTO_QUEST,						// 自动任务设置
	SAVE_OPT_AUTO_RECOVERY,						// 自动回复设置
};

class GameClientOpt
	: public Ogre::Singleton<GameClientOpt>
{
public:
	GameClientOpt(void);
	~GameClientOpt(void);

public:
	// 设置保存数据的大小
	void setLevelSize( int nLevel, int nSize );
	// 设置保存到服务器上的信息
	void setClientOpt( int nLevel, char* szData, bool bDelaySave = true );
	// 读取保存到服务器上的配置信息
	bool loadClientOpt( int nLevel, int nVersion, char* szOutData );
	// 设置配置信息
	void setOptContent( const char* szData );

	void update( unsigned int dtick );
private:
	std::map<int,int>		m_mapLevel; 
	int						m_OptLen;                         
	char					m_OptContent[MAX_OPT_LEN]; 
	unsigned int			m_nLastSend;
	bool					m_bNeedSend;
};
