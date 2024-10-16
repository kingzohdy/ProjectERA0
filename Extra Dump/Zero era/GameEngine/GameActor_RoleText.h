#pragma once
#include "GameEngineTypes.h"
#include "OgreBaseObject.h"
#include "OgreFixedString.h"

#include <map>
#include <deque>
#include <list>
#include "OgreUIRenderTypes.h"

const unsigned int NPCFUNDE_TILTLE_COLOR	= 0xffd986ff; // NPC������ɫ
const unsigned int TMPENEMY_TITLE_COLOR		= 0xffff0000; // �жԹ���������ɫ

const unsigned int SELF_TITLE_COLOR			= 0Xfffffebd; // �������������ɫ
const unsigned int PLAYER_TITLE_COLOR		= 0xfffffebd; // ��ͨ���������ɫ
const unsigned int FRIEND_TITLE_COLOR		= 0xff93BCE5; // ����������ɫ
const unsigned int TEAMMEMBER_TITLE_COLOR	= 0xff0080ff; // ����������ɫ

const unsigned int SPAN_MAP_DIFF_COLOR = 0xffff0000;//�����ͬ����ɫ������ɫ

const unsigned int CLAN_TITLE_MY_COLOR		= 0xff80ff80; // �Լ��л�������ɫ
const unsigned int CLAN_TITLE_ALLY_COLOR	= 0xff80ffff; // ͬ���л�������ɫ
const unsigned int CLAN_TITLE_ENEMY_COLOR	= 0xffff8040; // �ж��л�������ɫ
const unsigned int CLAN_TITLE_OTHER_COLOR	= 0xffffffff; // �����л�������ɫ

const unsigned int PKER_TITLE_COLOR			= 0xffff0000;
const unsigned int PKEST_TITLE_COLOR		= 0xff910000;
const unsigned int AMOK_TITLE_COLOR			= 0xffff0000;
const unsigned int BATTLE_ALLY				= 0xff80ff80;
const unsigned int BATTLE_ENEMY				= 0xffff8040;

const unsigned int TEAM_TITLE_COLOR			= 0xffcdb25f; // ���������ɫ

const unsigned int CLAN_ANTI_SPY_COLOR		= 0xff00ff00;//��̽��е�̽�����ɫ
const unsigned int CLAN_SPY_COLOR			= 0xffff0000;//��̽��еķ������ɫ

const unsigned int LOVE_SIGN_COLOR			= 0xffff08ff;//���£����޹�ϵ��ɫ

// �ع�����˼·
// 1.�ⲿʹ���߽������������ݣ��磺�߶ȣ����֣�ͼ�꣩��2.��һ֡����Щ���ݻ��Ƴ���
// FONT_TYPE
//tolua_begin
enum
{
	NAME_FONT = 0,					// ������������

	NORMAL_MAIN_ATTACK_FONT,		// ������ͨ���ܹ�����ɶԷ����ܵ��˺�
	NORMAL_MAIN_PET_ATTACK_FONT,	// ������ͨ���ܹ�����ɶԷ����ܵ��˺�
	SKILL_MAIN_NORMAL_ATTACK_FONT,	// �����Լ��ܹ�����ɶԷ����ܵ��˺�
	NORMAL_MAIN_DAMAGED_FONT,		// �����ܵ�����ͨ�˺�
	NORMAL_MAINPET_DAMAGED_FONT,	// ���ǳ����ܵ�����ͨ�˺�
	
	NORMAL_MAIN_HEAVY_ATTACK_FONT,	// ������ͨ������ɶԷ����ܵ��˺�
	HEAVY_MAINPET_ATTACK_FONT,		// ���ǳ��ﱩ����ɶԷ����ܵ��˺�
	SKILL_MAIN_HEAVY_ATTACK_FONT,	// ���Ǽ��ܱ�����ɶԷ����ܵ��˺�
	HEAVY_MAIN_DAMAGED_FONT,		// �����ܱ����˺�����
	HEAVY_MAINPET_DAMAGED_FONT,		// �����ܱ����˺�����

	HP_ADD_FONT,					// ��Ѫ����
	MP_ADD_FONT,					// ��������

	MISS_IMMUNE_FONT,				// ������������
	
	EXP_ADD_FONT,					// ���Ӿ�������
	MONEY_ADD_FONT,					// ������������
	BINDGOLD_ADD_FONT,				// ���Ӱ󶨽������

	HPBAR,							// Ѫ�����
	HPBAR_BACKGROUP,				// Ѫ�����װ�

	CRYSTAL_FONT,					// ˮ����Դ����
	ORE_FONT,						// ��ʯ��Դ����

	OWN_MONSTER_FLAG,				// ӵ�е�����ֱ��

	TEAM_LEADER,					// �ӳ�
	TEAM_MEMBER,					// ��Ա

	MACHINE_ENERGY,					// �ɼ��������ظ�
	MACHINE_RESOURCE,				// �ɼ�����Դ����

	TASK_SPY_GONG,					//�鱨�����Ĺ�ͼ��
	TASK_SPY_FANG,					//�鱨�����ķ�ͼ��
	STRONG_GLORY,					//ս����ҫ
	MACHINE_COLLECT,				// �ɼ�����Դͼ��

	RUNE_EX_DO,						// ��������Ч��
	ZHENGYI_ADD_FONT,				// ���������
	LIFTBRAND_ADD_FONT,				//�������潱��

	FONT_END,
};

void AddFlyFont(int fontType, int fontTypeTex, const char *text, int color = 0xffffff);
//tolua_end

extern Ogre::HUIRES s_hFontTexture[FONT_END];

// FLY_TYPE
enum
{
	FONT_FLY_LINE,			// ��ֱ����
	FONT_FLY_LINE_RAND,		// ���������
	FONT_FLY_DISPER,		// ɢ��
};

enum
{
	RELATIVE_TO_HEAD,	// �����ͷ��
	RELATIVE_TO_FOOT,	// ����ڽŵ�
};

struct tagFont
{
	int					m_nFontLife;		// ��׼��������
	int					m_nSpeed;			// �����ٶ�ms,�����ó���һ����
	float				m_fOffX;			// XƮ���ٶȷ���
	float				m_fOffY;			// YƮ���ٶȷ���
	float				m_fScal;			// ������
	int					m_nScalTime;		// �Ŵ����ʱ��
	int					m_nScalOverTime;	// �Ŵ�����ʱ��
	int					m_nFlyType;			// ���з�ʽ
	int					m_nFontFlyId;		// ID(����������з�ʽֱ���ü���ID)
	float				m_fInitOffX;		// ��ʼXƫ��ֵ
	float				m_fInitOffY;		// ��ʼYƫ��ֵ
	float				m_fDeltaDistY;		// ��Ƶ��ÿ��Y���
	int					m_nReltivePoint;	// ð������Ե�
	int					m_nReltiveOffset;	// ���ð�����ƫ��
	bool				m_bIsHeavyAttack;	// �Ƿ�����Ա���������
	unsigned int		m_nDelayTime;		// �ӳ�ʱ��
};

struct tagFlyText
{
	int				m_nFontType;	// ��������
	int				m_nCurOffX;		// ��ǰ���ֵ�Yƫ��ֵ
	int				m_nCurOffY;		// ��ǰ���ֵ�Xƫ��ֵ
	unsigned char	m_Red;
	unsigned char	m_Green;
	unsigned char	m_Blue;
	float			m_fCurScal;		// ��ǰ���ֵ����ű���
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
	// ������ʱ��Ʒͷ����Ϣ
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
	// ���ƹ���ȫ��ͷ����Ϣ
	void drawMonsterAllTitle( FollowCamera* pcamera );
	// ���ƹ��ﲿ��ͷ����Ϣ
	void drawMonsterPartialTitle( FollowCamera* pcamera );
	// ���ƹ���ͷ����Ϣ
	void drawMonsterTitle( FollowCamera* pcamera );
	void drawMonsterOwnerClanName( FollowCamera* pcamera );
	void drawMonsterStickyFrame( FollowCamera* pcamera );

	// ����NPCͷ����Ϣ
	void drawNpcTitle( FollowCamera* pcamera );
	
	void drawPlayerCommonTitle( FollowCamera* pcamera );
	void drawPlayerName( FollowCamera* pcamera );
	void drawPlayerTeamName( FollowCamera* pcamera );
	void drawPlayerHP( FollowCamera* pcamera );
	bool canDrawPlayerTitle();
	// �������ȫ��ͷ����Ϣ
	void drawPlayerAllTitle( FollowCamera* pcamera );
	// ������Ҳ���ͷ����Ϣ
	void drawPlayerPartialTitle( FollowCamera* pcamera );
	// �������ͷ����Ϣ
	void drawPlayerTitle( FollowCamera* pcamera );

	void drawMainPlayerCommonTitle( FollowCamera* pcamera );
	void drawMainPlayerName( FollowCamera* pcamera );
	void drawMainPlayerTeamName( FollowCamera* pcamera );
	void drawMainPlayerHP( FollowCamera* pcamera );

	const char* GetSpecialActivityAddInfo( GameActor* pActor );

	bool canDrawMainPlayerTitle();
	// ��������ȫ��ͷ����Ϣ
	void drawMainPlayerAllTitle( FollowCamera* pcamera );
	// �������ǲ���ͷ����Ϣ
	void drawMainPlayerPartialTitle( FollowCamera* pcamera );
	// ��������ͷ����Ϣ
	void drawMainPlayerTitle( FollowCamera* pcamera );
	// �������̯λͷ����Ϣ
	void drawMainPlayerStallTitle( FollowCamera* pcamera );
	//�����������̯λͷ����Ϣ
	void drawPlayerStallTextTitle( FollowCamera* pcamera );
	//����������£�������Ϣ
	void drawPlayerLoveSignTitle( FollowCamera* pcamera );

	void drawPetChengHao( FollowCamera* pcamera );
	void drawPetName( FollowCamera* pcamera );
	void drawPetHP( FollowCamera* pcamera );
	void drawPetOwnerName( FollowCamera* pcamera );
	bool canDrawPetTitle();
	// ���Ƴ���ȫ��ͷ����Ϣ
	void drawPetAllTitle( FollowCamera* pcamera );
	// ���Ƴ��ﲿ��ͷ����Ϣ
	void drawPetPartialTitle( FollowCamera* pcamera );
	// ���Ƴ���ͷ����Ϣ
	void drawPetTitle( FollowCamera* pcamera );

	// ��ȡͷ����ɫ��Ϣ
	void drawFlyTitles( FollowCamera* pcamera );
	// ���ƹ��ﺰ�������ݴ���
	void drawMonsterPopWin( FollowCamera* pcamera );
	// ���Ƴ��ﺰ�������ݴ���
	void drawPetPopWin( FollowCamera* pcamera );
	// ���� id
	void RoleText::drawIDTextTitle(FollowCamera* pcamera);
	// ���� time
	void drawMonsterTime( FollowCamera* pcamera);
	// ������¯ɱ�ּ���
	void drawRongluCount( FollowCamera* pcamera );
	//����ʧ��ͼ���ռ�������Ŀ
	void drawLostTotem( FollowCamera* pcamera );
	//���Ƴ�����Դ�ֹ���
	void drawResMonsterOwner( FollowCamera* pcamera);
	
	void drawPlayerVip( FollowCamera* pcamera, GameActor* pActor, float yoffset );

	static void SetLoadMap( bool isLoading );
	static bool isLoadingMap();
private:
	bool canDrawTitle();
	// ����ͷ����ʾ���ı���Ϣ
	
	void drawTopTitles( Ogre::Camera *pcamera, const char *ptitle, Ogre::uint color, float h, 
		float fOffsetY = 0, void **icon = NULL, size_t count = 0, bool bFiexedPos = false, Ogre::uint scolor = 0xff000000  );
	// 
	void drawTopTexture( Ogre::Camera *pcamera, float fOffsetY,  float fOffsetX = 0, 
						void **icon = NULL, size_t count = 0, bool bFiexedPos = false );

	void drawFlyTitles( Ogre::Camera *pcamera, tagCurPlayText& playingTextInfo, unsigned int uCurTime );
	// ���ƹ���ͷ�����
	void drawMonsterIcon(Ogre::Camera *pCamera, void *i, int offest);
	// ����ͷ��Ѫ��
	void drawHp( Ogre::Camera *pcamera, float fpre, float h );
	// ��������ͷ������������Ϣ
	void drawClanName( FollowCamera* pcamera );
	// ���ƽ�ɫͷ�����ݿ���Ϣ
	void drawPopWindow( Ogre::Vector3 scrpos );
	// ֱ�߷���
	void flyLine( Ogre::Camera *pcamera, tagCurPlayText& playingTextInfo, unsigned int uCurTime );	
		
	// ɢ��ʽ����
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
	static std::multimap<int, tagFont>	m_mapFontType;	// �������ͱ�
	std::vector<tagFlyFont>			m_vecFont;		// ��ʾ�����б�
	//std::map<int, std::vector<tagFlyFont> >	m_allShowFont;	// ����Ҫ��ʾ�������б�
	bool							m_bDrawTitle;	// �Ƿ���ʾ�ƺ�	
	GameActor*						m_pOwner;		// ��������
	//std::vector<tagRoleTitleInfo>	m_titles;		// Ҫ����ͷ����Ϣ����

	std::map<int, std::deque<tagCurPlayText> >	m_curPlayingTextCollection;
	char	m_szCurHaveUpdated[FONT_END];	// ��ǰ֡�Ѿ����¹���ĳ������ðѪ�˺�
	
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
