#pragma once
#include "GameEngineTypes.h"
#include "OgreBaseObject.h"
#include "OgreFixedString.h"

#include <map>
#include <deque>
#include <list>
#include "OgreUIRenderTypes.h"

const unsigned int NPCFUNDE_TILTLE_COLOR	= 0xffd986ff; // NPC功能颜色
const unsigned int TMPENEMY_TITLE_COLOR		= 0xffff0000; // 敌对怪物名字颜色

const unsigned int SELF_TITLE_COLOR			= 0Xfffffebd; // 玩家自已名字颜色
const unsigned int PLAYER_TITLE_COLOR		= 0xfffffebd; // 普通玩家名字颜色
const unsigned int FRIEND_TITLE_COLOR		= 0xff93BCE5; // 好友名字颜色
const unsigned int TEAMMEMBER_TITLE_COLOR	= 0xff0080ff; // 队友名字颜色

const unsigned int SPAN_MAP_DIFF_COLOR = 0xffff0000;//跨服不同服角色名字颜色

const unsigned int CLAN_TITLE_MY_COLOR		= 0xff80ff80; // 自己行会名字颜色
const unsigned int CLAN_TITLE_ALLY_COLOR	= 0xff80ffff; // 同盟行会名字颜色
const unsigned int CLAN_TITLE_ENEMY_COLOR	= 0xffff8040; // 敌对行会名字颜色
const unsigned int CLAN_TITLE_OTHER_COLOR	= 0xffffffff; // 其他行会名字颜色

const unsigned int PKER_TITLE_COLOR			= 0xffff0000;
const unsigned int PKEST_TITLE_COLOR		= 0xff910000;
const unsigned int AMOK_TITLE_COLOR			= 0xffff0000;
const unsigned int BATTLE_ALLY				= 0xff80ff80;
const unsigned int BATTLE_ENEMY				= 0xffff8040;

const unsigned int TEAM_TITLE_COLOR			= 0xffcdb25f; // 组队名字颜色

const unsigned int CLAN_ANTI_SPY_COLOR		= 0xff00ff00;//刺探活动中的探标记颜色
const unsigned int CLAN_SPY_COLOR			= 0xffff0000;//刺探活动中的防标记颜色

const unsigned int LOVE_SIGN_COLOR			= 0xffff08ff;//情侣，夫妻关系颜色

// 重构基本思路
// 1.外部使用者仅仅是设置数据（如：高度，文字，图标）；2.下一帧将这些数据绘制出来
// FONT_TYPE
//tolua_begin
enum
{
	NAME_FONT = 0,					// 人物名字字体

	NORMAL_MAIN_ATTACK_FONT,		// 主角普通技能攻击造成对方所受的伤害
	NORMAL_MAIN_PET_ATTACK_FONT,	// 宠物普通技能攻击造成对方所受的伤害
	SKILL_MAIN_NORMAL_ATTACK_FONT,	// 主角以技能攻击造成对方所受的伤害
	NORMAL_MAIN_DAMAGED_FONT,		// 主角受到的普通伤害
	NORMAL_MAINPET_DAMAGED_FONT,	// 主角宠物受到的普通伤害
	
	NORMAL_MAIN_HEAVY_ATTACK_FONT,	// 主角普通暴击造成对方所受的伤害
	HEAVY_MAINPET_ATTACK_FONT,		// 主角宠物暴击造成对方所受的伤害
	SKILL_MAIN_HEAVY_ATTACK_FONT,	// 主角技能暴击造成对方所受的伤害
	HEAVY_MAIN_DAMAGED_FONT,		// 主角受暴击伤害字体
	HEAVY_MAINPET_DAMAGED_FONT,		// 宠物受暴击伤害字体

	HP_ADD_FONT,					// 加血字体
	MP_ADD_FONT,					// 加蓝字体

	MISS_IMMUNE_FONT,				// 闪避中文字体
	
	EXP_ADD_FONT,					// 增加经验字体
	MONEY_ADD_FONT,					// 增加银币字体
	BINDGOLD_ADD_FONT,				// 增加绑定金币字体

	HPBAR,							// 血条面板
	HPBAR_BACKGROUP,				// 血条面板底板

	CRYSTAL_FONT,					// 水晶资源字体
	ORE_FONT,						// 矿石资源字体

	OWN_MONSTER_FLAG,				// 拥有的任务怪标记

	TEAM_LEADER,					// 队长
	TEAM_MEMBER,					// 队员

	MACHINE_ENERGY,					// 采集车能量回复
	MACHINE_RESOURCE,				// 采集车资源增加

	TASK_SPY_GONG,					//情报攻防的攻图标
	TASK_SPY_FANG,					//情报攻防的防图标
	STRONG_GLORY,					//战旗荣耀
	MACHINE_COLLECT,				// 采集车资源图标

	RUNE_EX_DO,						// 发动特殊效果
	ZHENGYI_ADD_FONT,				// 增加正义点
	LIFTBRAND_ADD_FONT,				//增加生存奖章

	FONT_END,
};

void AddFlyFont(int fontType, int fontTypeTex, const char *text, int color = 0xffffff);
//tolua_end

extern Ogre::HUIRES s_hFontTexture[FONT_END];

// FLY_TYPE
enum
{
	FONT_FLY_LINE,			// 垂直纵向
	FONT_FLY_LINE_RAND,		// 随机点纵向
	FONT_FLY_DISPER,		// 散弹
};

enum
{
	RELATIVE_TO_HEAD,	// 相对于头顶
	RELATIVE_TO_FOOT,	// 相对于脚底
};

struct tagFont
{
	int					m_nFontLife;		// 标准文字生命
	int					m_nSpeed;			// 显现速度ms,间隔多久出现一个字
	float				m_fOffX;			// X飘移速度方向
	float				m_fOffY;			// Y飘移速度方向
	float				m_fScal;			// 增大倍数
	int					m_nScalTime;		// 放大持续时间
	int					m_nScalOverTime;	// 放大缩回时间
	int					m_nFlyType;			// 飞行方式
	int					m_nFontFlyId;		// ID(技能特殊飞行方式直接用技能ID)
	float				m_fInitOffX;		// 初始X偏移值
	float				m_fInitOffY;		// 初始Y偏移值
	float				m_fDeltaDistY;		// 高频率每字Y间隔
	int					m_nReltivePoint;	// 冒出的相对点
	int					m_nReltiveOffset;	// 相对冒出点的偏移
	bool				m_bIsHeavyAttack;	// 是否是针对暴击的配置
	unsigned int		m_nDelayTime;		// 延迟时间
};

struct tagFlyText
{
	int				m_nFontType;	// 字体类型
	int				m_nCurOffX;		// 当前文字的Y偏移值
	int				m_nCurOffY;		// 当前文字的X偏移值
	unsigned char	m_Red;
	unsigned char	m_Green;
	unsigned char	m_Blue;
	float			m_fCurScal;		// 当前文字的缩放比例
	std::string		m_Text;
	tagFlyText()
	{
		reset();
	};

	void reset()
	{
		m_nFontType = -1;
		m_nCurOffX	= 0;
		m_nCurOffY	= 0;
		m_Red = 255;
		m_Green = 255;
		m_Blue = 255;
		m_fCurScal	= 1.0f;
	}
};

struct tagRoleTitleInfo
{
	std::string		m_strContent;
	unsigned int	m_uHeight;
};

class IDrawTitleIcon
{
public:
	virtual void *	GetTitleIcon(GameActor *)	= 0;
	virtual int		GetPriority(){return 0;}
};

struct tagCurPlayText
{
	unsigned int	m_uStartTime;
	tagFlyText		m_curTextInfo;
	tagFont*		m_pFontType;
	int				m_nIndex;
};

typedef std::multimap<int, tagFont>::iterator FLY_FONT_ITER;

class IActorTitleDrawer;
class RoleText
{
public:
	RoleText(void);
	~RoleText(void);

public:
	struct tagFlyFont
	{
		tagFlyFont() : m_uStartTime( 0 ), m_pFontType( NULL ) {}
		unsigned int			m_uStartTime;
		int						m_nAttackTime;
		tagFont*				m_pFontType;
		std::list<tagFlyText>	m_vectorText;
	};

	void Update( float dtick );
	
	static void loadFontConfig();
	static void initFont();
	static void releaseFont();
	static bool hasFontType( int id );
	static tagFont* getFontType( int id, bool isHeavyAttack = false );
	
	void CreateActorTitleControl();
	void setOwner( GameActor* pActor );
	void setEnableDrawTitle( bool flag );

	const tagFont* getFontType( const char* szFontType ) const;
	void AddFlyFont( tagFlyFont& t );
	Ogre::Vector3 GetTopTitlePos( bool bFixedPos = false );
	void BuildTitle();
	void UpdateTextColor();
	void UpdateTitleHp();
	void OnDrawPlayerTitles( Ogre::Vector3& screenPos, Ogre::Vector3& worldPos, FollowCamera *pcamera );
	void drawTitles( FollowCamera *pcamera );
	// 绘制临时物品头顶信息
	void drawItemUITitle( FollowCamera* pcamera );
	void drawItemTextTitle( FollowCamera* pcamera );
	void drawTmpItemTitle( FollowCamera* pcamera );

	
	void drawMonsterCommonTitle( FollowCamera* pcamera );
	void drawMonsterChengHao( FollowCamera* pcamera );
	void drawMonsterOwnerName( FollowCamera* pcamera );
	void drawMonsterName( FollowCamera* pcamera );
	void drawMonsterHP( FollowCamera* pcamera );
	void drawMonsterOre( FollowCamera* pcamera );
	bool canDrawMonsterTitle();
	// 绘制怪物全部头顶信息
	void drawMonsterAllTitle( FollowCamera* pcamera );
	// 绘制怪物部分头顶信息
	void drawMonsterPartialTitle( FollowCamera* pcamera );
	// 绘制怪物头顶信息
	void drawMonsterTitle( FollowCamera* pcamera );
	void drawMonsterOwnerClanName( FollowCamera* pcamera );
	void drawMonsterStickyFrame( FollowCamera* pcamera );

	// 绘制NPC头顶信息
	void drawNpcTitle( FollowCamera* pcamera );
	
	void drawPlayerCommonTitle( FollowCamera* pcamera );
	void drawPlayerName( FollowCamera* pcamera );
	void drawPlayerTeamName( FollowCamera* pcamera );
	void drawPlayerHP( FollowCamera* pcamera );
	bool canDrawPlayerTitle();
	// 绘制玩家全部头顶信息
	void drawPlayerAllTitle( FollowCamera* pcamera );
	// 绘制玩家部分头顶信息
	void drawPlayerPartialTitle( FollowCamera* pcamera );
	// 绘制玩家头顶信息
	void drawPlayerTitle( FollowCamera* pcamera );

	void drawMainPlayerCommonTitle( FollowCamera* pcamera );
	void drawMainPlayerName( FollowCamera* pcamera );
	void drawMainPlayerTeamName( FollowCamera* pcamera );
	void drawMainPlayerHP( FollowCamera* pcamera );

	const char* GetSpecialActivityAddInfo( GameActor* pActor );

	bool canDrawMainPlayerTitle();
	// 绘制主角全部头顶信息
	void drawMainPlayerAllTitle( FollowCamera* pcamera );
	// 绘制主角部分头顶信息
	void drawMainPlayerPartialTitle( FollowCamera* pcamera );
	// 绘制主角头顶信息
	void drawMainPlayerTitle( FollowCamera* pcamera );
	// 绘制玩家摊位头顶信息
	void drawMainPlayerStallTitle( FollowCamera* pcamera );
	//绘制其他玩家摊位头顶信息
	void drawPlayerStallTextTitle( FollowCamera* pcamera );
	//绘制玩家情侣，夫妻信息
	void drawPlayerLoveSignTitle( FollowCamera* pcamera );

	void drawPetChengHao( FollowCamera* pcamera );
	void drawPetName( FollowCamera* pcamera );
	void drawPetHP( FollowCamera* pcamera );
	void drawPetOwnerName( FollowCamera* pcamera );
	bool canDrawPetTitle();
	// 绘制宠物全部头顶信息
	void drawPetAllTitle( FollowCamera* pcamera );
	// 绘制宠物部分头顶信息
	void drawPetPartialTitle( FollowCamera* pcamera );
	// 绘制宠物头顶信息
	void drawPetTitle( FollowCamera* pcamera );

	// 获取头顶颜色信息
	void drawFlyTitles( FollowCamera* pcamera );
	// 绘制怪物喊话的泡泡窗口
	void drawMonsterPopWin( FollowCamera* pcamera );
	// 绘制宠物喊话的泡泡窗口
	void drawPetPopWin( FollowCamera* pcamera );
	// 绘制 id
	void RoleText::drawIDTextTitle(FollowCamera* pcamera);
	// 绘制 time
	void drawMonsterTime( FollowCamera* pcamera);
	// 绘制熔炉杀怪计数
	void drawRongluCount( FollowCamera* pcamera );
	//绘制失落图腾收集经验数目
	void drawLostTotem( FollowCamera* pcamera );
	//绘制场景资源怪归属
	void drawResMonsterOwner( FollowCamera* pcamera);
	
	void drawPlayerVip( FollowCamera* pcamera, GameActor* pActor, float yoffset );

	static void SetLoadMap( bool isLoading );
	static bool isLoadingMap();
private:
	bool canDrawTitle();
	// 绘制头顶显示的文本信息
	
	void drawTopTitles( Ogre::Camera *pcamera, const char *ptitle, Ogre::uint color, float h, 
		float fOffsetY = 0, void **icon = NULL, size_t count = 0, bool bFiexedPos = false, Ogre::uint scolor = 0xff000000  );
	// 
	void drawTopTexture( Ogre::Camera *pcamera, float fOffsetY,  float fOffsetX = 0, 
						void **icon = NULL, size_t count = 0, bool bFiexedPos = false );

	void drawFlyTitles( Ogre::Camera *pcamera, tagCurPlayText& playingTextInfo, unsigned int uCurTime );
	// 绘制怪物头顶标记
	void drawMonsterIcon(Ogre::Camera *pCamera, void *i, int offest);
	// 绘制头顶血条
	void drawHp( Ogre::Camera *pcamera, float fpre, float h );
	// 绘制人物头顶公会名字信息
	void drawClanName( FollowCamera* pcamera );
	// 绘制角色头顶泡泡框信息
	void drawPopWindow( Ogre::Vector3 scrpos );
	// 直线飞行
	void flyLine( Ogre::Camera *pcamera, tagCurPlayText& playingTextInfo, unsigned int uCurTime );	
		
	// 散弹式飞行
	void flyDispersedly( Ogre::Camera *pcamera, tagCurPlayText& playingTextInfo, unsigned int uCurTime );
	
	bool isOutOfTimePlayingText( const tagCurPlayText& playingTextInfo, unsigned int uCurTime );
	void clearAllOutOfTimePlayingText( unsigned int uCurTime );
	void updateAllPlayingText( unsigned int uCurTime );
	void updatePlayingTextCollection( tagFlyFont& t, unsigned int uStartTime );
	void addPlayingText( int nType, unsigned int uStartTime, const tagFlyText& flyTextInfo, 
										tagFont* pFontType );
	
	bool canUpdatePlayingTextCollection( int nType, int nID, unsigned int uCurTime );

	Ogre::HUIRES getVipTexRes( int vipID );

private:	
	static std::multimap<int, tagFont>	m_mapFontType;	// 字体类型表
	std::vector<tagFlyFont>			m_vecFont;		// 显示数字列表
	//std::map<int, std::vector<tagFlyFont> >	m_allShowFont;	// 所有要显示的数字列表
	bool							m_bDrawTitle;	// 是否显示称呼	
	GameActor*						m_pOwner;		// 所属对象
	//std::vector<tagRoleTitleInfo>	m_titles;		// 要绘制头顶信息内容

	std::map<int, std::deque<tagCurPlayText> >	m_curPlayingTextCollection;
	char	m_szCurHaveUpdated[FONT_END];	// 当前帧已经更新过的某种类型冒血伤害
	
	struct tagAdjustPosInfo
	{
		int		m_nTitleYIndex;
		float	m_fHeightInHpAndFirstText;
	};
	tagAdjustPosInfo				m_posInfo;

private:
	static std::vector<IDrawTitleIcon *>	m_lstTitleDrawer;
	static bool	s_mIsLoadingMap;

	static std::map<int,Ogre::HUIRES>		m_sMapVipTex;

	IActorTitleDrawer*	m_pActorTitleDrawControl;

public:
	static void registTitleDrawer(IDrawTitleIcon *drawer);
	static Ogre::HUIRES getBattleIcon( GameActor* pOneActor );
};
