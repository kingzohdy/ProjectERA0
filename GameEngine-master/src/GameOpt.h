#pragma once
//tolua_begin
enum
{
	GAME_OPT_SHOW_ALLPLAYER_AND_PET_NAME = 0,		// 显示所有玩家包括主角名字和宠物的名字
	GAME_OPT_SHOW_ALLPLAYER_TEAM_DESIGNATION_NAME,	// 显示所有玩家包括主角队伍和称号
	//GAME_OPT_SHOW_MAINPET_NAME,					// 显示主角的宠物名字
	GAME_OPT_SHOW_MAIN_CLAN_NAME,					// 显示主角工会名字
	GAME_OPT_SHOW_MONSTER_NAME,						// 显示怪物名字
	GAME_OPT_SHOW_MONSTER_HP,						// 显示怪物血量
	GAME_OPT_SHOW_OTHERPLAYER_HP,					// 显示其他玩家血量
	//GAME_OPT_SHOW_OTHERPLAYER_PET_HP,				// 显示其他玩家宠物血量
	GAME_OPT_SHOW_OTHERPLAYER_CLAN_NAME,			// 显示主角工会名字
	GAME_OPT_SHOW_MAINPLAYER_AND_TEAM_HP,					// 显示主角血量
	GAME_OPT_SHOW_OTHERPLAYER_TALK_POP,				// 显示主角聊天泡泡
	GAME_OPT_SHOW_MAINPLAYER_DAMAGE_SHOW,			// 显示冒血伤害
	GAME_OPT_SHOW_EQUIP_SHOW,						// 显示装备比较
	GAME_OPT_REFUSE_RIDE_INVITE,					// 拒绝骑乘邀请
	GAME_OPT_REFUSE_ADD_FRIEND_REQ,					// 拒绝好友申请
	GAME_OPT_REFUSE_CLAN_INVITE,					// 拒绝公会邀请
	GAME_OPT_REFUSE_TRADE_REQ,						// 拒绝交易申请
	GAME_OPT_REFUSE_TEAM_INVITE,					// 拒绝组队邀请
	GAME_OPT_REFUSE_SEE_EQUIP,						// 拒绝查看装备
	GAME_OPT_REFUSE_REFUSE_PK,						// 拒绝切磋邀请
	GAME_OPT_SWAP_MOUSE,							// 交换左右键
	GAME_OPT_SWAP_MOUSE_DEFAULT_SHOW_HAVE_SELECT,	// 下次登录自动默认已选模式
	GAME_OPT_LAST_PET_SELECT,						// 上次选中的宠物
	GAME_OPT_LOCK_SHORTCUT,							// 锁定快捷栏
	GAME_OPT_FORBIT_SELECT_PET,						// 禁止选中宠物
	GAME_OPT_LAST_PET_FIGHT,						// 上次出战的宠物
	GAME_OPT_AUTO_SKILL_RELEASE,                    // 技能自动释放
	GAME_OPT_AUTO_ACCEPT_TEAM_FOLLOW,				// 自动接受组队跟随要求
	GAME_OPT_SHOW_PERFECT_STAR_EFFECT,				// 显示完美星特效
	GAME_OPT_SHOW_STAR_STONE,						// 显示特权徽章
	GAME_OPT_CAMERA_3D,						// 显示特权徽章
	//*******************/
	MAX_GAME_SET_OPT = 32,
};

//tolua_end

// 游戏设置共享buffer
class GameSettingConfig
{
public:
	GameSettingConfig();
	void	setGameOpt( int nIndex, char chValue );
	bool	getGameOpt( int nIndex );
	char	getGameOptValue( int nIndex );
	unsigned int getMaxSize()
	{
		return MAX_GAME_SET_OPT;
	}
	char*	getRawBuffer()
	{
		return m_szSettingData;
	}

public:
	char	m_szSettingData[MAX_GAME_SET_OPT];
};