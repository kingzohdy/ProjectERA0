#pragma once
#include "GameEngineTypes.h"
#include "OgreBaseObject.h"
#include "OgreFixedString.h"
#include <map>

enum
{
	NAME_FONT			= 0,		// ������������
	NORMAL_HIT_FONT,				// ��ͨ��������
	HEAVY_HIT_FONT,					// ��������
	HP_ADD_FONT,					// ��Ѫ����
	MP_ADD_FONT,					// ��������
	CHINESE_FONT,					// ��������
	HPBAR,							// Ѫ�����
	HPBAR_BACKGROUP,				// Ѫ�����װ�

	FONT_END,
};

enum
{
	FONT_FLY_LINE,			// ��ֱ����
	FONT_FLY_LINE_RAND,		// ���������
	FONT_FLY_DISPER,		// ɢ��
};

struct tagFont
{
	int					m_nFontLife;		// ��������
	int					m_nSpeed;			// �����ٶ�ms,�����ó���һ����
	float				m_fOffX;			// X����ƫ��ms
	float				m_fOffY;			// Y����ƫ��ms
	float				m_fScal;			// ������
	int					m_nScalTime;		// �Ŵ����ʱ��
	int					m_nScalOverTime;	// �Ŵ�����ʱ��
	int					m_nFlyType;			// ���з�ʽ
	int					m_nFontFlyId;		// ID(����������з�ʽֱ���ü���ID)
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
	
	// ֱ�߷���
	void flyLine( Ogre::Camera *pcamera, tagFlyFont& t );		
	// ɢ��ʽ����
	void flyDispersedly( Ogre::Camera *pcamera, tagFlyFont& t );				
private:
	static std::map<int, tagFont>	m_mapFontType;	// �������ͱ�
	std::vector<tagFlyFont>			m_vecFFont;		// ��ʾ�����б�
	bool							m_bDrawTitle;	// �Ƿ���ʾ�ƺ�	
	GameActor*						m_pOwner;		// ��������
};
