#include <Windows.h>
#include "OtherplayerTitleDrawer.h"
#include "GameMainPlayer.h"
#include "GameActorManager.h"
#include "GameActor_PVP.h"
#include "OgreUIRenderer.h"
#include "WorldCity.h"

// 其他玩家
// 其他玩家绘制名字
// TODO: 这些高度值和颜色值以后统一放到XML配置文件，给美术策划调
const float TITLE_FIRST_LINE				= -1.0f;
const float TITLE_FORTH_LINE				= -3.2f;

//const unsigned int PLAYER_TITLE_COLOR		= 0xfffffebd; // 普通玩家名字颜色
//const unsigned int FRIEND_TITLE_COLOR		= 0xff93BCE5; // 好友名字颜色
//const unsigned int BATTLE_ENEMY			= 0xffff8040;
//const unsigned int BATTLE_ALLY			= 0xff80ff80;
//const unsigned int AMOK_TITLE_COLOR		= 0xffff0000;
//const unsigned int PKER_TITLE_COLOR		= 0xffff0000;
//const unsigned int PKEST_TITLE_COLOR		= 0xff910000;
//const unsigned int TEAMMEMBER_TITLE_COLOR	= 0xff0080ff; // 队友名字颜色
//const unsigned int TEAM_TITLE_COLOR		= 0xffcdb25f; // 组队名字颜色

static float s_playerTitleHeight[]			= { -0.2f, -1.2f, -2.3f, -3.3f };

extern const char* getDesignationTexturePath( int nDesignationID );
extern Ogre::HUIRES GetTitleFontRes( int nFontType );
extern Ogre::HUIRES getDesignationTexRes( int nDesignationID );

OtherplayerTitleDrawer::OtherplayerTitleDrawer( GameActor* pOwner ) : IActorTitleDrawer( pOwner )
{
}

OtherplayerTitleDrawer::~OtherplayerTitleDrawer()
{
}

void OtherplayerTitleDrawer::Update( float dtick )
{
	UpdateCrimeNameColor();
	ValidateVkMenuState();
}

void OtherplayerTitleDrawer::UpdateTextColor()
{
	UpdateNameColor();
	UpdateClanNameColor();
}

void OtherplayerTitleDrawer::UpdateTitleHp()
{
	tagDrawTitleIconInfo& hpTexturetInfo = m_allDrawIcons[OTHER_PLAYER_TITLE_ICON_HP];
	if ( hpTexturetInfo.m_hUiRes == NULL )
	{
		return;
	}

	BuildHp();
}

void OtherplayerTitleDrawer::ValidateVkMenuState()
{

	//在特殊的副本不能显示
	if (m_pOwner->IsInSpecialPworldNotShowTitle())
	{
		return;
	}
	// 如果上次没显示全部头顶信息，而这次alt键按下，则要更新
	// 若上次已经显示所有头顶信息，则不管是否按下ALT键 
	short keyState		= ( GetKeyState( VK_MENU ) & 0xff00 );
	bool isCurMenuDown	= keyState != 0;
	if ( isCurMenuDown == m_bIsLastMenuDown )
	{
		return;
	}
	
	m_bIsLastMenuDown = isCurMenuDown;
	
	if ( IsOwnerSelectedOrTouchByMouse() )
	{
		return;
	}

	BuildTitle();
}

void OtherplayerTitleDrawer::ClearAllTitle()
{
	for ( int i = 0; i < OTHER_PLAYER_TITLE_END; ++i )
	{
		tagDrawTextTitleInfo& oneDrawTextInfo = m_allTextTitles[i];
		std::string& strText = oneDrawTextInfo.m_strTitle;
		strText.clear();
	}
}

void OtherplayerTitleDrawer::ClearAllIconTitle()
{
	for ( int i = 0; i < OTHER_PLAYER_TITLE_ICON_END; ++i )
	{
		tagDrawTitleIconInfo& oneDrawIconInfo = m_allDrawIcons[i];
		oneDrawIconInfo.m_hUiRes = NULL;
	}
}

bool OtherplayerTitleDrawer::IsShowName()
{
	/*if ( IsForbidShowAllTitle() )
	{
		return false;
	}*/
	
	if ( isInStall() )
	{
		return false;
	}

	if ( IsForceShowHPNameClanTeam() )
	{
		return true;
	}

	ActorManager& actorMgr = ActorManager::getSingleton();
	if ( !actorMgr.isShowOtherPlayerTitle() )
	{
		return false;
	}

	if( m_pOwner->isOnMonsterMachine() )
	{
		return true;
	}

	MainGamePlayer* pMainplayer		= actorMgr.getMainPlayer();

	if ( !actorMgr.getGameOpt( GAME_OPT_SHOW_ALLPLAYER_AND_PET_NAME ) )
	{
		return false;
	}

	return true;
}

const char* OtherplayerTitleDrawer::GetSpecialActivityAddInfo( GameActor* pActor )
{
	//是不是在光影之战的副本
	int mapid = pActor->getMapRealId();
	if( mapid >= GAME_PWORLD_MAPID )
	{
		int nPworldId = mapid/100;
		const tagPworldDef* pPworldDef = DBDataManager::getSingleton().getPworldDef( nPworldId );
		if( pPworldDef != NULL && pPworldDef->PolyType == PWORLD_POLY_DOUBLE_MONSTER )
		{
			if ( pActor->getCampIdx() == 1 )
			{
				return "[光明]";
			}
			else if ( pActor->getCampIdx() == 2 )
			{
				return "[暗影]";
			}
		}else if( pPworldDef != NULL && pPworldDef->PolyType == PWORLD_POLY_TANK )
		{
			if ( pActor->getCampIdx() == 1 )
			{
				return "[联邦军]";
			}
			else if ( pActor->getCampIdx() == 2 )
			{
				return "[帝国军]";
			}
		}
	}
	return "";
}

void OtherplayerTitleDrawer::BuildNameTtile()
{
	if ( !IsShowName() )
	{
		return;
	}
	m_Name = m_pOwner->getName();
	m_Name = ActorManager::getSingleton().replaceSpecialCode(m_Name);
	if( m_pOwner->getServerId() != -1 )
	{
		m_Name = string("[") + m_pOwner->getActorWorldName() + string("]") + m_Name;
	}
	m_Name = string(GetSpecialActivityAddInfo(m_pOwner)) + m_Name;

	
	tagDrawTextTitleInfo& nameDrawTextInfo = m_allTextTitles[OTHER_PLAYER_TITLE_NAME];

	nameDrawTextInfo.m_fOffsetX = 0.0f;
	nameDrawTextInfo.m_fOffsetY = GetNameOffsetY();
	nameDrawTextInfo.m_color	= GetNameColor();
	nameDrawTextInfo.m_strTitle	= GetName();
}

float OtherplayerTitleDrawer::GetNameOffsetY()
{
	return TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + s_playerTitleHeight[m_posInfo.m_nTitleYIndex++];
}

const char* OtherplayerTitleDrawer::GetName()
{
	return m_Name.c_str();
}


Ogre::ColorQuad OtherplayerTitleDrawer::GetNameColor()
{
	GameActor_PVP& actorPvpMgr = GameActor_PVP::getSingleton();
	// TODO: 这个没加
	if ( actorPvpMgr.isForcePkMode() && actorPvpMgr.getPkMode() != PK_MODE_NORMAL )
	{
		if ( ( m_pOwner->m_ObjCamp & SKILL_TARGET_ENEMY_PLAYER ) != 0 )
		{
			return BATTLE_ENEMY;
		}
		else if ( ( m_pOwner->m_ObjCamp & SKILL_TARGET_FRIEND_PLAYER ) != 0 )
		{
			return BATTLE_ALLY;
		}
	}
	
	if ( actorPvpMgr.hasCrime( m_pOwner ) )
	{
		unsigned int now = timeGetTime();
		if (now > m_pOwner->m_lastChangeNameColorTime)
		{
			m_pOwner->m_lastChangeNameColorTime = now + 250;
			m_pOwner->m_isLastChangeNameColor = !m_pOwner->m_isLastChangeNameColor;
		}

		if (m_pOwner->m_isLastChangeNameColor)
		{
			return AMOK_TITLE_COLOR;
		}
	}

	int amok = actorPvpMgr.getAmok( m_pOwner );
	if (amok == 1)
	{
		return PKER_TITLE_COLOR;
	}
	else if (amok == 2)
	{
		return PKEST_TITLE_COLOR;
	}

	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer->isInSameTeam( m_pOwner->getTeamWID() ) )
	{
		return TEAMMEMBER_TITLE_COLOR;
	}

	Ogre::uint color = PLAYER_TITLE_COLOR;
	switch ( m_pOwner->m_nRelation )
	{
	case RELATION_TYPE_FRIEND:
		{
			return FRIEND_TITLE_COLOR;
		}
		break;
	default:
		break;
	}

	if( pMainPlayer->isInPVPMap() && pMainPlayer->m_nCampIdx != m_pOwner->m_nCampIdx )
	{
		return SPAN_MAP_DIFF_COLOR;
	}

	return PLAYER_TITLE_COLOR;
}	

// 其他玩家绘制公会名字
bool OtherplayerTitleDrawer::IsShowClanName()
{
	if ( !m_pOwner->isInClan() )
	{
		return false;
	}
	
	if ( isInStall() )
	{
		return false;
	}

	if ( IsForceShowHPNameClanTeam() )
	{
		return true;
	}

	if ( IsForbidShowAllTitle() )
	{
		return false;
	}

	ActorManager& actorMgr			= ActorManager::getSingleton();
	MainGamePlayer* pMainplayer		= actorMgr.getMainPlayer();


	if ( actorMgr.getGameOpt( GAME_OPT_SHOW_MAIN_CLAN_NAME ) && pMainplayer->isInSameClan( m_pOwner->getClanWID() ) )
	{
		return true;
	}

	if ( actorMgr.getGameOpt( GAME_OPT_SHOW_OTHERPLAYER_CLAN_NAME ) && !pMainplayer->isInSameClan( m_pOwner->getClanWID() ) )
	{
		return true;
	}

	return false;
}
bool OtherplayerTitleDrawer::IsShowDaoBa()
{
	if ( m_pOwner->getDaoBaClanWid() == 0 )
	{
		return false;
	}
	if ( isInStall() )
	{
		return false;
	}

	ActorManager& actorMgr			= ActorManager::getSingleton();
	MainGamePlayer* pMainplayer		= actorMgr.getMainPlayer();

	//跨服战场不显示刀疤
	if ( pMainplayer->isOutSide() )
	{
		return false;
	}

	if ( IsForceShowHPNameClanTeam() )
	{
		return true;
	}

	if ( IsForbidShowAllTitle() )
	{
		return false;
	}

	if ( actorMgr.getGameOpt( GAME_OPT_SHOW_MAIN_CLAN_NAME ) && pMainplayer->isInSameClan( m_pOwner->getClanWID() ) )
	{
		return true;
	}

	if ( actorMgr.getGameOpt( GAME_OPT_SHOW_OTHERPLAYER_CLAN_NAME ) && !pMainplayer->isInSameClan( m_pOwner->getClanWID() ) )
	{
		return true;
	}
	return false;
}

void OtherplayerTitleDrawer::BuildClanNameTtile()
{
	float clanNamePos = 0.0f;
	if ( IsShowDaoBa() )
	{
		clanNamePos = GetClanNameOffsetY();
		tagDrawTitleIconInfo& mClanTaskInfo = m_allDrawIcons[OTHER_PLAYER_TITLE_ICON_CLAN_GLORY];
		mClanTaskInfo.m_hUiRes = GetTitleFontRes( STRONG_GLORY );
		float width, height;
		Ogre::UIRenderer::getSingletonPtr()->GetTextExtent( GetTitleFontRes( NAME_FONT ), GetClanName(), width, height );
		if (  width == 0 )
		{
			mClanTaskInfo.m_fOffsetX = 10.0;
			height = 15.0f;
		}
		else
		{
			mClanTaskInfo.m_fOffsetX = - width/2;
		}
		mClanTaskInfo.m_fOffsetY = clanNamePos*height -14.0f;
		mClanTaskInfo.m_fWidth = 18.0;
		mClanTaskInfo.m_fHeight = 21.0;
	}
	tagDrawTextTitleInfo& clanNameDrawTextInfo = m_allTextTitles[OTHER_PLAYER_TITLE_CLAN_NAME];
	if ( !IsShowClanName() )
	{
		return;
	}
	if ( clanNamePos == 0.0f )
	{
		clanNamePos = GetClanNameOffsetY();
	}
	clanNameDrawTextInfo.m_fOffsetX = 0.0f;
	clanNameDrawTextInfo.m_fOffsetY = clanNamePos;
	clanNameDrawTextInfo.m_color	= GetClanNameColor();
	clanNameDrawTextInfo.m_strTitle	= GetClanName();

	if ( m_pOwner->m_ViewFlag & VIEW_FLAG_SPY )
	{
		tagDrawTitleIconInfo& mClanTaskInfo = m_allDrawIcons[OTHER_PLAYER_TITLE_ICON_CLAN_SPY];
		mClanTaskInfo.m_hUiRes = GetTitleFontRes( TASK_SPY_GONG );
		mClanTaskInfo.m_fOffsetX = 10.0;
		float m_offsety = 0;
		if ( IsShowTemaOrChengHaoName() )
		{
			m_offsety = -20.0;
		}
		mClanTaskInfo.m_fOffsetY = -70.0 + m_offsety;
		mClanTaskInfo.m_fWidth = 25.0;
		mClanTaskInfo.m_fHeight = 31.0;
	}
	else if ( m_pOwner->m_ViewFlag & VIEW_FLAG_ANTI_SPY )
	{
		tagDrawTitleIconInfo& mClanTaskInfo = m_allDrawIcons[OTHER_PLAYER_TITLE_ICON_CLAN_SPY];
		mClanTaskInfo.m_hUiRes = GetTitleFontRes( TASK_SPY_FANG );
		mClanTaskInfo.m_fOffsetX = 10.0;
		float m_offsety = 0;
		if ( IsShowTemaOrChengHaoName() )
		{
			m_offsety = -20.0;
		}
		mClanTaskInfo.m_fOffsetY = -70.0 + m_offsety;
		mClanTaskInfo.m_fWidth = 25.0;
		mClanTaskInfo.m_fHeight = 31.0;
	}
}

float OtherplayerTitleDrawer::GetClanNameOffsetY()
{
	return TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + s_playerTitleHeight[m_posInfo.m_nTitleYIndex++];
}
float OtherplayerTitleDrawer::GetClanSPYOffsetY()
{
	return TITLE_FORTH_LINE + m_posInfo.m_fHeightInHpAndFirstText + s_playerTitleHeight[m_posInfo.m_nTitleYIndex++];
}

const char* OtherplayerTitleDrawer::GetClanName()
{
	static std::string s_strClanPositionDesc;

	const char* pszClanName	= m_pOwner->getClanName();
	s_strClanPositionDesc.clear();
	//if (WorldCityManager::getSingleton().IsWorldCityKeeper(m_pOwner))
	//{
	//	s_strClanPositionDesc += "[时间城] ";
	//}
	s_strClanPositionDesc += ActorManager::getSingleton().getClanFengHao(m_pOwner->getClanWID());

	switch( m_pOwner->m_nClanPosition )
	{
	case CLAN_POS_NOT_MEMBER:
		{
			s_strClanPositionDesc += pszClanName;
		}
		break;

	case CLAN_POS_NORMAL:
		{
			s_strClanPositionDesc += pszClanName;
			if ( m_pOwner->isInClanGroup() )
			{
				const char *szGroupName[] = { " 弑杀战团"," 暗杀战团"," 狂杀战团"," 灭杀战团"," 绝杀战团"," 黄金战团" };
				s_strClanPositionDesc += szGroupName[m_pOwner->getClanGroupID()- 1];
			}
		}
		break;

	case CLAN_POS_TECH_OFFICER:
		{
			s_strClanPositionDesc += pszClanName;
			s_strClanPositionDesc += " 科技官员";
		}
		break;

	case CLAN_POS_COLLECTION_OFFICER:
		{
			s_strClanPositionDesc += pszClanName;
			s_strClanPositionDesc += " 采集官员";
		}
		break;

	case CLAN_POS_FACTORY_OFFICER:
		{
			s_strClanPositionDesc += pszClanName;
			s_strClanPositionDesc += " 工厂官员";
		}
		break;

	case CLAN_POS_PET_OFFICER:
		{
			s_strClanPositionDesc += pszClanName;
			s_strClanPositionDesc += " 宠物官员";
		}
		break;

	case CLAN_POS_WALL_OFFICER:
		{
			s_strClanPositionDesc += pszClanName;
			s_strClanPositionDesc += " 郊区官员";
		}
		break;	

	case CLAN_POS_VICE_PRESIDENT:
		{
			s_strClanPositionDesc += pszClanName;
			s_strClanPositionDesc += " 副会长";
			if ( m_pOwner->isInClanGroup() )
			{
				const char *szGroupName[] = { "(弑杀)","(暗杀)","(狂杀)","(灭杀)","(绝杀)","(黄金)" };
				s_strClanPositionDesc += szGroupName[m_pOwner->getClanGroupID()- 1];
			}
		}
		break;
	case CLAN_POS_PRESIDENT:
		{
			s_strClanPositionDesc += pszClanName;
			s_strClanPositionDesc += " 会长";
			if ( m_pOwner->isInClanGroup() )
			{
				const char *szGroupName[] = { "(弑杀)","(暗杀)","(狂杀)","(灭杀)","(绝杀)","(黄金)" };
				s_strClanPositionDesc += szGroupName[m_pOwner->getClanGroupID()- 1];
			}
		}
		break;
	case CLAN_POS_GROUP_1:
		{
			s_strClanPositionDesc += pszClanName;
			s_strClanPositionDesc += " 弑杀战团长";
		}
		break;
	case CLAN_POS_GROUP_2:
		{
			s_strClanPositionDesc += pszClanName;
			s_strClanPositionDesc += " 暗杀战团长";
		}
		break;
	case CLAN_POS_GROUP_3:
		{
			s_strClanPositionDesc += pszClanName;
			s_strClanPositionDesc += " 狂杀战团长";
		}
		break;
	case CLAN_POS_GROUP_4:
		{
			s_strClanPositionDesc += pszClanName;
			s_strClanPositionDesc += " 灭杀战团长";
		}
		break;
	case CLAN_POS_GROUP_5:
		{
			s_strClanPositionDesc += pszClanName;
			s_strClanPositionDesc += " 绝杀战团长";
		}
		break;
	case CLAN_POS_GROUP_6:
		{
			s_strClanPositionDesc += pszClanName;
			s_strClanPositionDesc += " 黄金战团长";
		}
		break;
	
	default:
		break;
	}

	return s_strClanPositionDesc.c_str();
}

Ogre::ColorQuad OtherplayerTitleDrawer::GetClanNameColor()
{
	return m_pOwner->getClanColor();
}

// 其他玩家绘制队伍或者称号名字
bool OtherplayerTitleDrawer::IsShowTemaOrChengHaoName()
{
	const char* pszDesignation = m_pOwner->getDesignation();
	if( pszDesignation[0] == '\0' )
	{
		return false;
	}

	const char* pszDesignationTexPath = getDesignationTexturePath( m_pOwner->getDesignationID() );
	if ( pszDesignationTexPath[0] != '\0' && getDesignationTexRes( m_pOwner->getDesignationID() ) != NULL )
	{
		return false;
	}
	
	if ( isInStall() )
	{
		return false;
	}

	if ( IsForceShowHPNameClanTeam() )
	{
		return true;
	}

	if ( IsForbidShowAllTitle() )
	{
		return false;
	}

	ActorManager& actorMgr			= ActorManager::getSingleton();
	MainGamePlayer* pMainplayer		= actorMgr.getMainPlayer();

	if ( !actorMgr.getGameOpt( GAME_OPT_SHOW_ALLPLAYER_TEAM_DESIGNATION_NAME ) )
	{
		return false;
	}

	return true;
}

void OtherplayerTitleDrawer::BuildTemaOrChengHaoNameTtile()
{
	tagDrawTextTitleInfo& temaOrChengHaoNameDrawTextInfo = m_allTextTitles[OTHER_PLAYER_TITLE_TEAM_OR_CHENG_HAO_NAME];
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	//在末世殿堂副本显示竞技场等级，原位置上的称号不显示
	if ( pMain->getMapRealId() == SPARTA_MAP_ID )
	{
		temaOrChengHaoNameDrawTextInfo.m_fOffsetX	= 0.0f;
		temaOrChengHaoNameDrawTextInfo.m_fOffsetY	= GetTemaOrChengHaoNameOffsetY();
		temaOrChengHaoNameDrawTextInfo.m_color		= 0xff00ff00;
		temaOrChengHaoNameDrawTextInfo.m_strTitle	= ActorManager::getSingleton().GetSpartaLevelName( m_pOwner->getSpartaFightVal() );
		return;
	}
	if ( !IsShowTemaOrChengHaoName() )
	{
		return;
	}
	
	temaOrChengHaoNameDrawTextInfo.m_fOffsetX	= 0.0f;
	temaOrChengHaoNameDrawTextInfo.m_fOffsetY	= GetTemaOrChengHaoNameOffsetY();
	temaOrChengHaoNameDrawTextInfo.m_color		= GetTemaOrChengHaoNameColor();
	temaOrChengHaoNameDrawTextInfo.m_strTitle	= GetTemaOrChengHaoName();
}

float OtherplayerTitleDrawer::GetTemaOrChengHaoNameOffsetY()
{
	return TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + s_playerTitleHeight[m_posInfo.m_nTitleYIndex++];
}

const char* OtherplayerTitleDrawer::GetTemaOrChengHaoName()
{
	return m_pOwner->getDesignation();
}

Ogre::ColorQuad OtherplayerTitleDrawer::GetTemaOrChengHaoNameColor()
{
	const tagDesignationDef* DesignationDef = &DBDataManager::getSingleton().getDesignationDef( m_pOwner->getDesignationID() );
	if ( !DesignationDef )
	{		
		return TEAM_TITLE_COLOR;
	}
	const unsigned int nColor = DesignationDef->RGB;
	if ( nColor == 0 )
	{
		return TEAM_TITLE_COLOR;
	}
	else
	{
		return nColor + 0xff000000;
	}
}

// 其他玩家绘制摊位名字
bool OtherplayerTitleDrawer::IsShowStallName()
{
	/*if ( IsForbidShowAllTitle() )
	{
		return false;
	}*/
	
	
	if ( isInStall() )
	{
		if ( m_pOwner->isInScene() )
		{
			return true;
		}

		ActorManager& actorMgr = ActorManager::getSingleton();
		if ( !actorMgr.isShowOtherPlayerTitle() )
		{
			return false;
		}

		MainGamePlayer* pMainplayer		= actorMgr.getMainPlayer();

		if ( IsForceShowHPNameClanTeam() )
		{
			return true;
		}

		if ( !actorMgr.getGameOpt( GAME_OPT_SHOW_ALLPLAYER_AND_PET_NAME ) )
		{
			return false;
		}

		return true;
	}
	

	return false;
}

void OtherplayerTitleDrawer::BuildStallNameTitle()
{
	tagDrawTextTitleInfo& stallNameDrawTextInfo = m_allTextTitles[OTHER_PLAYER_TITLE_STALL_NAME];
	if ( !IsShowStallName() )
	{
		return;
	}
	
	stallNameDrawTextInfo.m_fOffsetX	= 0.0f;
	stallNameDrawTextInfo.m_fOffsetY	= GetStallNameOffsetY();
	stallNameDrawTextInfo.m_color		= GetStallNameColor();
	stallNameDrawTextInfo.m_strTitle	= GetStallName();
}

float OtherplayerTitleDrawer::GetStallNameOffsetY()
{
	return TITLE_FIRST_LINE;
}

const char* OtherplayerTitleDrawer::GetStallName()
{
	return m_pOwner->getStallName();
}

Ogre::ColorQuad OtherplayerTitleDrawer::GetStallNameColor()
{
	return PLAYER_TITLE_COLOR;
}

// 玩家伴侣信息
bool OtherplayerTitleDrawer::IsShowLoveSign()
{
	if ( IsForbidShowAllTitle() )
	{
		return false;
	}

	if ( !m_pOwner->isShowLove() )
	{
		return false;
	}
	

	const char* pszStallName = m_pOwner->getStallName();
	if ( !m_pOwner->isInStall() && pszStallName[0] != '\0' )
	{
		if ( m_pOwner->isInScene() )
		{
			return true;
		}

		ActorManager& actorMgr = ActorManager::getSingleton();
		if ( !actorMgr.isShowOtherPlayerTitle() )
		{
			return false;
		}

		MainGamePlayer* pMainplayer		= actorMgr.getMainPlayer();

		if ( IsForceShowHPNameClanTeam() )
		{
			return true;
		}

		if ( !actorMgr.getGameOpt( GAME_OPT_SHOW_ALLPLAYER_AND_PET_NAME ) )
		{
			return false;
		}

		return true;
	}
	

	return false;
}

void OtherplayerTitleDrawer::BuildLoveSign()
{
	tagDrawTextTitleInfo& loveSignDrawTextInfo = m_allTextTitles[OTHER_PLAYER_TITLE_LOVE_SIGN];
	if ( !IsShowLoveSign() )
	{
		return;
	}
	
	loveSignDrawTextInfo.m_fOffsetX	= 0.0f;
	loveSignDrawTextInfo.m_fOffsetY	= GetLoveSignOffsetY();
	loveSignDrawTextInfo.m_color		= GetLoveSignColor();
	loveSignDrawTextInfo.m_strTitle	+= "☆";
	loveSignDrawTextInfo.m_strTitle	+= GetLoveSign();
	loveSignDrawTextInfo.m_strTitle	+= "☆";
}

float OtherplayerTitleDrawer::GetLoveSignOffsetY()
{
	return TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + s_playerTitleHeight[m_posInfo.m_nTitleYIndex++];
}

const char* OtherplayerTitleDrawer::GetLoveSign()
{
	return m_pOwner->getStallName();
}

Ogre::ColorQuad OtherplayerTitleDrawer::GetLoveSignColor()
{
	return LOVE_SIGN_COLOR;
}


void OtherplayerTitleDrawer::DrawAllTopTextTitles( Ogre::Camera *pcamera )
{
	for ( int i = 0; i < OTHER_PLAYER_TITLE_END; ++i )
	{
		const tagDrawTextTitleInfo& oneDrawTextInfo = m_allTextTitles[i];
		DrawOneTopTextTitles( pcamera, oneDrawTextInfo );
	}
}

void OtherplayerTitleDrawer::DrawAllIconTitle( Ogre::Camera *pCamera )
{
	for ( int i = 0; i < OTHER_PLAYER_TITLE_ICON_END; ++i )
	{
		const tagDrawTitleIconInfo& oneDrawIconInfo = m_allDrawIcons[i];
		DrawOneIconTitle( pCamera, oneDrawIconInfo );
	}
}

float OtherplayerTitleDrawer::GetBattleIconOffsetX()
{
	Ogre::UIRenderer *pUIRender = Ogre::UIRenderer::getSingletonPtr();
	float fTextWidth		= 0;
	float fTextHeight		= 0;
	const char* pszTitle	= GetName();
	pUIRender->GetTextExtent( GetTitleFontRes( NAME_FONT ), pszTitle, fTextWidth, fTextHeight);

	float fAdjustX = 0;
	Ogre::HUIRES hTeamIcon	= GetTeamIcon();
	if ( hTeamIcon != NULL )
	{
		fAdjustX = GetTeamIconWidth();
	}
	return fTextWidth * 0.5f + GetBattleIconWidth() + fAdjustX;
}

float OtherplayerTitleDrawer::GetBattleIconOffsetY()
{
	Ogre::UIRenderer *pUIRender = Ogre::UIRenderer::getSingletonPtr();
	float fTextWidth		= 0;
	float fTextHeight		= 0;
	const char* pszTitle	= GetName();
	pUIRender->GetTextExtent( GetTitleFontRes( NAME_FONT ), pszTitle, fTextWidth, fTextHeight);
	float fAdjustY = TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + s_playerTitleHeight[m_posInfo.m_nTitleYIndex];
	return fAdjustY * fTextHeight - 17;
}

float OtherplayerTitleDrawer::GetBattleIconWidth()
{
	Ogre::UIRenderer *pUIRender = Ogre::UIRenderer::getSingletonPtr();
	int nWidth	= 0;
	int nHeight = 0;
	Ogre::HUIRES hBattleIcon = RoleText::getBattleIcon( m_pOwner );
	pUIRender->GetTextureSize( hBattleIcon, nWidth, nHeight );
	return nWidth;
}

float OtherplayerTitleDrawer::GetBattleIconHeight()
{
	Ogre::UIRenderer *pUIRender = Ogre::UIRenderer::getSingletonPtr();
	int nWidth	= 0;
	int nHeight = 0;
	Ogre::HUIRES hBattleIcon = RoleText::getBattleIcon( m_pOwner );
	pUIRender->GetTextureSize( hBattleIcon, nWidth, nHeight );
	return nHeight;
}

float OtherplayerTitleDrawer::GetTeamIconOffsetX()
{
	Ogre::UIRenderer *pUIRender = Ogre::UIRenderer::getSingletonPtr();
	float fTextWidth		= 0;
	float fTextHeight		= 0;
	const char* pszTitle	= GetName();
	pUIRender->GetTextExtent( GetTitleFontRes( NAME_FONT ), pszTitle, fTextWidth, fTextHeight);

	return fTextWidth * 0.5f + GetTeamIconWidth();
}

float OtherplayerTitleDrawer::GetTeamIconOffsetY()
{
	return GetBattleIconOffsetY();
}

float OtherplayerTitleDrawer::GetTeamIconWidth()
{
	Ogre::UIRenderer *pUIRender = Ogre::UIRenderer::getSingletonPtr();
	int nWidth	= 0;
	int nHeight = 0;
	Ogre::HUIRES hTeamIcon = GetTeamIcon();
	pUIRender->GetTextureSize( hTeamIcon, nWidth, nHeight );
	return nWidth;
}

float OtherplayerTitleDrawer::GetTeamIconHeight()
{
	Ogre::UIRenderer *pUIRender = Ogre::UIRenderer::getSingletonPtr();
	int nWidth	= 0;
	int nHeight = 0;
	Ogre::HUIRES hTeamIcon = GetTeamIcon();
	pUIRender->GetTextureSize( hTeamIcon, nWidth, nHeight );
	return nHeight;
}

bool OtherplayerTitleDrawer::IsForbidShowAllTitle()
{
	if ( m_pOwner == NULL )
	{
		return true;
	}

	ActorManager& actorMgr			= ActorManager::getSingleton();
	MainGamePlayer* pMainplayer		= actorMgr.getMainPlayer();
	if ( !actorMgr.isShowOtherPlayerTitle() )
	{
		if ( pMainplayer == NULL )
		{
			return true;
		}

		if ( !pMainplayer->isInSameTeam( m_pOwner->getTeamWID() ) )
		{
			return true;
		}
	}

	if( m_pOwner->isOnMonsterMachine() )
	{
		return true;
	}

	return false;
}

bool OtherplayerTitleDrawer::IsOwnerSelectedOrTouchByMouse()
{
	ActorManager& actorMgr			= ActorManager::getSingleton();
	MainGamePlayer* pMainplayer		= actorMgr.getMainPlayer();
	if ( m_isForceDrawTitle )
	{
		return true;
	}

	if ( pMainplayer->GetSelectTarget() == m_pOwner->GetID() ) 
	{
		return true;
	}

	return false;
}

bool OtherplayerTitleDrawer::IsForceShowHPNameClanTeam()
{
	if ( IsOwnerSelectedOrTouchByMouse() )
	{
		return true;
	}

	short keyState		= ( GetKeyState( VK_MENU ) & 0xff00 );
	bool isCurMenuDown	= keyState != 0;
	if ( isCurMenuDown )
	{
		return true;
	}

	return false;
}

bool OtherplayerTitleDrawer::IsShowHp()
{
	ActorManager& actorMgr			= ActorManager::getSingleton();
	MainGamePlayer* pMainplayer		= actorMgr.getMainPlayer();

	if ( isInStall() )
	{
		return false;
	}

	if ( IsForceShowHPNameClanTeam() )
	{
		return true;
	}

	if ( IsForbidShowAllTitle() )
	{
		return false;
	}

	if ( actorMgr.getGameOpt( GAME_OPT_SHOW_MAINPLAYER_AND_TEAM_HP ) && pMainplayer->isInSameTeam( m_pOwner->getTeamWID() ) )
	{
		return true;
	}

	if ( !actorMgr.getGameOpt( GAME_OPT_SHOW_OTHERPLAYER_HP ) )
	{
		return false;
	}

	return true;
}

void OtherplayerTitleDrawer::BuildHp()
{
	AdjustInitHpHeight();
	if ( !IsShowHp() )
	{
		return;
	}

	tagDrawTitleIconInfo& hpBackTexturetInfo = m_allDrawIcons[OTHER_PLAYER_TITLE_ICON_HP_BACK];
	hpBackTexturetInfo.m_hUiRes		= GetTitleFontRes( HPBAR_BACKGROUP );
	hpBackTexturetInfo.m_fOffsetX	= GetHpBackOffsetX();
	hpBackTexturetInfo.m_fOffsetY	= GetHpBackOffsetY();
	hpBackTexturetInfo.m_fWidth		= GetHpBackWidth();
	hpBackTexturetInfo.m_fHeight	= GetHpBackHeight();

	tagDrawTitleIconInfo& hpTexturetInfo = m_allDrawIcons[OTHER_PLAYER_TITLE_ICON_HP];
	hpTexturetInfo.m_hUiRes				= GetTitleFontRes( HPBAR );
	hpTexturetInfo.m_fOffsetX			= GetHpOffsetX();
	hpTexturetInfo.m_fOffsetY			= GetHpOffsetY();
	hpTexturetInfo.m_fWidth				= GetHpWidth();
	hpTexturetInfo.m_fHeight			= GetHpHeight();
}

float OtherplayerTitleDrawer::GetHpOffsetX()
{
	return GetHpBackOffsetX() - 1.0f;
}

float OtherplayerTitleDrawer::GetHpOffsetY()
{
	return GetHpBackOffsetY();
}

float OtherplayerTitleDrawer::GetHpWidth()
{
	float fScale = (float)m_pOwner->getHP() / m_pOwner->getMaxHP();

	if ( m_pOwner->getHP() <= 0 || m_pOwner->isDead() )
	{
		fScale = 0;
	}

	if ( fScale > 1)
	{
		fScale = 1;
	}
	return GetHpBackWidth() * fScale;
}

float OtherplayerTitleDrawer::GetHpHeight()
{
	return GetHpBackHeight();
}

float OtherplayerTitleDrawer::GetHpBackOffsetX()
{
	return 35.0f;
}

float OtherplayerTitleDrawer::GetHpBackOffsetY()
{
	float fOffsetY = TITLE_FIRST_LINE;
	if( m_pOwner->getGender() == CLIENT_GENDER_FEMALE )
	{
		fOffsetY += 0.5f;
	}

	return fOffsetY * 10;
}

float OtherplayerTitleDrawer::GetHpBackWidth()
{
	return 66.0f;
}

float OtherplayerTitleDrawer::GetHpBackHeight()
{
	return 8.0f;
}

bool OtherplayerTitleDrawer::IsShowChengHaoNameIcon()
{
	const char* pszDesignation = m_pOwner->getDesignation();
	if( pszDesignation[0] == '\0' )
	{
		return false;
	}

	const char* pszDesignationTexPath = getDesignationTexturePath( m_pOwner->getDesignationID() );
	if ( pszDesignationTexPath[0] == '\0' )
	{
		return false;
	}

	if ( getDesignationTexRes( m_pOwner->getDesignationID() ) == NULL )
	{
		return false;
	}

	ActorManager& actorMgr			= ActorManager::getSingleton();
	MainGamePlayer* pMainplayer		= actorMgr.getMainPlayer();

	if ( isInStall() )
	{
		return false;
	}

	if ( IsForceShowHPNameClanTeam() )
	{
		return true;
	}

	if ( IsForbidShowAllTitle() )
	{
		return false;
	}

	if ( !actorMgr.getGameOpt( GAME_OPT_SHOW_ALLPLAYER_TEAM_DESIGNATION_NAME ) )
	{
		return false;
	}

	return true;
}

void OtherplayerTitleDrawer::BuildChengHaoNameIcon()
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	//在末世殿堂副本显示竞技场等级，原位置上的称号图片不显示
	if ( pMain->getMapRealId() == SPARTA_MAP_ID )
	{
		return;
	}
	if ( !IsShowChengHaoNameIcon() )
	{		
		return;
	}

	tagDrawTitleIconInfo& chengHaoTexturetInfo = m_allDrawIcons[OTHER_PLAYER_TITLE_ICON_CHENG_HAO];
	chengHaoTexturetInfo.m_hUiRes		= getDesignationTexRes( m_pOwner->getDesignationID() );
	chengHaoTexturetInfo.m_fOffsetX		= GetChengHaoNameIconOffsetX();
	chengHaoTexturetInfo.m_fOffsetY		= GetChengHaoNameIconOffsetY();
	chengHaoTexturetInfo.m_fWidth		= GetChengHaoNameIconWidth();
	chengHaoTexturetInfo.m_fHeight		= GetChengHaoNameIconHeight();
}

float OtherplayerTitleDrawer::GetChengHaoNameIconOffsetX()
{
	int nWidth			= 0; 
	int nHeight			= 0;
	Ogre::HUIRES hUiRes	= getDesignationTexRes( m_pOwner->getDesignationID() );
	Ogre::UIRenderer *pUIRenderer = Ogre::UIRenderer::getSingletonPtr();
	pUIRenderer->GetTextureSize( hUiRes, nWidth, nHeight);
	return nWidth * 0.5f;
}

float OtherplayerTitleDrawer::GetChengHaoNameIconOffsetY()
{
	float fFontWidth		= 0.0F; 
	float fFontHeight		= 0.0F;
	Ogre::HUIRES hFontRes	= GetTitleFontRes( NAME_FONT );
	const char* pszTitle	= "零纪元开拓者";
	Ogre::UIRenderer *pUIRenderer = Ogre::UIRenderer::getSingletonPtr();
	pUIRenderer->GetTextExtent( hFontRes, pszTitle, fFontWidth, fFontHeight );
	float fOffsetY			= TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + s_playerTitleHeight[m_posInfo.m_nTitleYIndex++];
	float fAccumulateHeight = fOffsetY * fFontHeight - 23;
	return fAccumulateHeight;
}

float OtherplayerTitleDrawer::GetChengHaoNameIconWidth()
{
	int nWidth			= 0; 
	int nHeight			= 0;
	Ogre::HUIRES hUiRes	= getDesignationTexRes( m_pOwner->getDesignationID() );
	Ogre::UIRenderer *pUIRenderer = Ogre::UIRenderer::getSingletonPtr();
	pUIRenderer->GetTextureSize( hUiRes, nWidth, nHeight);
	return nWidth;
}

float OtherplayerTitleDrawer::GetChengHaoNameIconHeight()
{
	int nWidth			= 0; 
	int nHeight			= 0;
	Ogre::HUIRES hUiRes	= getDesignationTexRes( m_pOwner->getDesignationID() );
	Ogre::UIRenderer *pUIRenderer = Ogre::UIRenderer::getSingletonPtr();
	pUIRenderer->GetTextureSize( hUiRes, nWidth, nHeight);
	return nHeight;
}

Ogre::HUIRES OtherplayerTitleDrawer::GetTeamIcon()
{
	ActorManager& actorMgr = ActorManager::getSingleton();
	/*if ( !actorMgr.getGameOpt( GAME_OPT_SHOW_ALLPLAYER_TEAM_DESIGNATION_NAME ) )
	{
		return NULL;
	}*/

	if ( !m_pOwner->isInTeam() )
	{
		return NULL;
	}

	Ogre::HUIRES hTeamIcon	= NULL;
	if (m_pOwner->m_bCaptain)
	{
		hTeamIcon = GetTitleFontRes( TEAM_LEADER );
	}
	else
	{
		hTeamIcon = GetTitleFontRes( TEAM_MEMBER );
	}

	return hTeamIcon;
}

bool OtherplayerTitleDrawer::IsShowNameIcon()
{
	Ogre::HUIRES hBattleIcon	= RoleText::getBattleIcon( m_pOwner );
	Ogre::HUIRES hTeamIcon		= GetTeamIcon();
	return hBattleIcon != NULL || hTeamIcon != NULL;
}

void OtherplayerTitleDrawer::BuildNameIcon()
{
	if ( !IsShowName() )
	{
		return;
	}

	if ( !IsShowNameIcon() )
	{
		return;
	}
	
	m_posInfo.m_fHeightInHpAndFirstText -= 0.5f;
	Ogre::HUIRES hTeamIcon	= GetTeamIcon();
	if ( hTeamIcon != NULL )
	{
		tagDrawTitleIconInfo& teamTexturetInfo	= m_allDrawIcons[OTHER_PLAYER_TITLE_ICON_TEAM];
		teamTexturetInfo.m_hUiRes				= hTeamIcon;
		teamTexturetInfo.m_fOffsetX				= GetTeamIconOffsetX();
		teamTexturetInfo.m_fOffsetY				= GetTeamIconOffsetY();
		teamTexturetInfo.m_fWidth				= GetTeamIconWidth();
		teamTexturetInfo.m_fHeight				= GetTeamIconHeight();
	}

	Ogre::HUIRES hBattleIcon = RoleText::getBattleIcon( m_pOwner );
	if ( hBattleIcon != NULL )
	{
		tagDrawTitleIconInfo& battleTexturetInfo	= m_allDrawIcons[OTHER_PLAYER_TITLE_ICON_BATTLE];
		battleTexturetInfo.m_hUiRes					= hBattleIcon;
		battleTexturetInfo.m_fOffsetX				= GetBattleIconOffsetX();
		battleTexturetInfo.m_fOffsetY				= GetBattleIconOffsetY();
		battleTexturetInfo.m_fWidth					= GetBattleIconWidth();
		battleTexturetInfo.m_fHeight				= GetBattleIconHeight();
	}
}

void OtherplayerTitleDrawer::AdjustInitHpHeight()
{
	if ( IsShowHp() )
	{
		if( m_pOwner->getGender() == CLIENT_GENDER_FEMALE )
		{
			m_posInfo.m_fHeightInHpAndFirstText = 0.5f;
		}
		else
		{
			m_posInfo.m_fHeightInHpAndFirstText = 0.0f;
		}
	}
	else
	{
		m_posInfo.m_fHeightInHpAndFirstText = 0.5f;
	}
}

void OtherplayerTitleDrawer::UpdateCrimeNameColor()
{
	tagDrawTextTitleInfo& nameTitleConfig = m_allTextTitles[OTHER_PLAYER_TITLE_NAME];
	if ( nameTitleConfig.m_strTitle.empty() )
	{
		return;
	}
	unsigned int now = timeGetTime();
	GameActor_PVP& actorPvpMgr = GameActor_PVP::getSingleton();
	if (actorPvpMgr.hasCrime( m_pOwner ) || m_pOwner->m_isLastChangeNameColor)
	{
		nameTitleConfig.m_color = GetNameColor();
	}
}
void OtherplayerTitleDrawer::UpdateNameColor()
{
	if ( !IsShowName() )
	{
		return;
	}

	tagDrawTextTitleInfo& nameDrawTextInfo = m_allTextTitles[OTHER_PLAYER_TITLE_NAME];
	nameDrawTextInfo.m_color = GetNameColor();
}

void OtherplayerTitleDrawer::UpdateClanNameColor()
{
	if ( !IsShowClanName() )
	{
		return;
	}

	tagDrawTextTitleInfo& clanNameDrawTextInfo = m_allTextTitles[OTHER_PLAYER_TITLE_CLAN_NAME];
	clanNameDrawTextInfo.m_color = GetClanNameColor();
}

float OtherplayerTitleDrawer::GetNameDrawYOffset()
{
	tagDrawTextTitleInfo& nameDrawTextInfo = m_allTextTitles[OTHER_PLAYER_TITLE_NAME];
	if( !nameDrawTextInfo.m_strTitle.empty() )
	{
		return nameDrawTextInfo.m_fOffsetY;
	}
	return IActorTitleDrawer::GetNameDrawYOffset();
}