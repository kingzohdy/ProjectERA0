#pragma once
#include "GameEngineTypes.h"
#include "OgreBaseObject.h"
#include "OgreFixedString.h"
#include <map>

enum
{
	NAME_FONT			= 0,		// 人物名字字体
	NORMAL_HIT_FONT,				// 普通攻击字体
	HEAVY_HIT_FONT,					// 暴击字体
	HP_ADD_FONT,					// 加血字体
	MP_ADD_FONT,					// 加蓝字体
	CHINESE_FONT,					// 中文字体
	HPBAR,							// 血条面板
	HPBAR_BACKGROUP,				// 血条面板底板

	FONT_END,
};

enum
{
	FONT_FLY_LINE,			// 垂直纵向
	FONT_FLY_LINE_RAND,		// 随机点纵向
	FONT_FLY_DISPER,		// 散弹
};

struct tagFont
{
	int					m_nFontLife;		// 文字生命
	int					m_nSpeed;			// 显现速度ms,间隔多久出现一个字
	float				m_fOffX;			// X方向偏移ms
	float				m_fOffY;			// Y方向偏移ms
	float				m_fScal;			// 增大倍数
	int					m_nScalTime;		// 放大持续时间
	int					m_nScalOverTime;	// 放大缩回时间
	int					m_nFlyType;			// 飞行方式
	int					m_nFontFlyId;		// ID(技能特殊飞行方式直接用技能ID)
};

struct tagFlyText
{
	int				m_nFontType;
	int				m_nCurOffX;
	int				m_nCurOffY;
	float			m_fCurScal;
	std::string		m_Text;
	tagFlyText(){
		m_nCurOffX = 0;
		m_nCurOffY = 0;
		m_fCurScal = 1.0f;
	};
};

class RoleText
{
public:
	RoleText(void);
	~RoleText(void);

public:
	struct tagFlyFont
	{
		unsigned int			m_uStartTime;
		tagFont*				m_pFontType;
		std::list<tagFlyText>	m_vectorText;
	};
	
	static void loadFontConfig();
	static void initFont();
	static void releaseFont();
	static bool hasFontType( int id );
	static tagFont* getFontType( int id );

	void setOwner( GameActor* pActor );
	void setEnableDrawTitle( bool flag );

	const tagFont* getFontType( const char* szFontType ) const;
	void AddFlyFont( tagFlyFont& t );

	void drawTitles( FollowCamera *pcamera );
	void drawFlyTitles( FollowCamera* pcamera );
private:
	bool canDrawTitle();

	void drawTopTitles( Ogre::Camera *pcamera, const char *ptitle, Ogre::uint color, int h );
	void drawFlyTitles( Ogre::Camera *pcamera, tagFlyFont& t );
	void drawHp( Ogre::Camera *pcamera, float fpre, int h );
	
	// 直线飞行
	void flyLine( Ogre::Camera *pcamera, tagFlyFont& t );		
	// 散弹式飞行
	void flyDispersedly( Ogre::Camera *pcamera, tagFlyFont& t );				
private:
	static std::map<int, tagFont>	m_mapFontType;	// 字体类型表
	std::vector<tagFlyFont>			m_vecFFont;		// 显示数字列表
	bool							m_bDrawTitle;	// 是否显示称呼	
	GameActor*						m_pOwner;		// 所属对象
};
