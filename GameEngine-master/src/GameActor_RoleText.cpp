#include "GameActor_RoleText.h"
#include "OgreResourceManager.h"
#include "OgreUIRenderer.h"
#include "GameActor.h"
#include "GameNpc.h"
#include "GameMainPlayer.h"
#include "ShowActor.h"
#include "GameCamera.h"
#include "OgreCamera.h"
#include "OgreLog.h"
#include "OgreXMLData.h"
#include "GameMap.h"
#include "GameActor_PVP.h"
#include <Windows.h>
#include <math.h>
#include <algorithm>
#include "OtherplayerTitleDrawer.h"
#include "WorldCity.h"
#include "ClanInfo.h"
#include <time.h>

#include "../UILib/ui_gameui.h"
#include "../UILib/ui_common.h"
#include "../UILib/ui_framemgr.h"

using namespace Ogre;
using namespace std;

enum  
{
	CLIENT_RELATION_TYPE_NONE = 0, 
	CLIENT_RELATION_TYPE_FRIEND = 1,		// 好友
	CLIENT_RELATION_TYPE_MASTER = 2,		// 师傅
	CLIENT_RELATION_TYPE_PRENTICE = 4,		// 徒弟
	CLIENT_RELATION_TYPE_SPOUSE = 8,		// 夫妻
	CLIENT_RELATION_TYPE_BROTHER = 16,		// 结拜
	CLIENT_RELATION_TYPE_SU_DI = 32,	// 追杀
	CLIENT_RELATION_TYPE_BLACK = 64,		// 黑名单
	CLIENT_RELATION_TYPE_TMPFRIEND = 128,	// 临时
	CLIENT_RELATION_TYPE_XIONG_SHOU = 256,		// 仇人	
};

Ogre::HUIRES s_hFontTexture[FONT_END];

Ogre::HUIRES GetTitleFontRes( int nFontType )
{
	assert( nFontType >= 0 && nFontType < FONT_END );
	return s_hFontTexture[nFontType];
}

const std::string strFontPath[] = 
{
	"uires\\fonts\\simhei.ttf",

	"uires\\ShuZiXiangGuanZiYuan\\new\\PuTongShangHaiZheng.xml",
	"uires\\ShuZiXiangGuanZiYuan\\new\\ChongWuShangHaiZheng.xml",
	"uires\\ShuZiXiangGuanZiYuan\\new\\JiNengShangHaiZheng.xml",
	"uires\\ShuZiXiangGuanZiYuan\\new\\ZiShenShangHaiZheng.xml",
	"uires\\ShuZiXiangGuanZiYuan\\new\\ZiShenShangHaiZheng.xml",

	"uires\\ShuZiXiangGuanZiYuan\\new\\PuTongShangHaiBaoJi.xml",
	"uires\\ShuZiXiangGuanZiYuan\\new\\ChongWuShangHaiBaoJi.xml",
	"uires\\ShuZiXiangGuanZiYuan\\new\\JiNengShangHaiBaoJi.xml",
	"uires\\ShuZiXiangGuanZiYuan\\new\\ZiShenShangHaiBaoJi.xml",
	"uires\\ShuZiXiangGuanZiYuan\\new\\ZiShenShangHaiBaoJi.xml",

	"uires\\ShuZiXiangGuanZiYuan\\new\\hpHuiFu.xml",
	"uires\\ShuZiXiangGuanZiYuan\\new\\mpHuiFu.xml",
	"uires\\ShuZiXiangGuanZiYuan\\new\\ShanBiMianYiXiShou.xml",
	"uires\\ShuZiXiangGuanZiYuan\\new\\JiaJingYan.xml",
	"uires\\ShuZiXiangGuanZiYuan\\new\\JiaYinBi.xml",

	"uires\\ShuZiXiangGuanZiYuan\\new\\JiaJinBi.xml",
	"uires\\TongYongLei\\XueTiao.tga",
	"uires\\TongYongLei\\XueTiaoKuang.tga",
	"uires\\ShuZiXiangGuanZiYuan\\new\\CaiJiShuiJing.xml",
	"uires\\ShuZiXiangGuanZiYuan\\new\\CaiJiKuangShi.xml",

	"uires\\ui\\Za\\ShangYunHuoChe.tga",
	"uires\\TongYongLei\\DuiZhangBiaoShi.tga",
	"uires\\TongYongLei\\DuiYuanBiaoShi.tga",
	"uires\\ShuZiXiangGuanZiYuan\\new\\CaiJiNengLiang.xml",
	"uires\\ShuZiXiangGuanZiYuan\\new\\CaiJiYouKuang.xml",

	"uires\\ui\\Za\\GongHuiGongFang-Gong.tga",
	"uires\\ui\\Za\\GongHuiGongFang-Fang.tga",
	"uires\\ui\\Za\\ZhanYun.tga",
	"uires\\ui\\Za\\ZiYuanXianShi.tga",
	"uires\\fonts\\simhei.ttf",

	"uires\\ShuZiXiangGuanZiYuan\\new\\JiaZhengYi.xml",
	"uires\\ShuZiXiangGuanZiYuan\\new\\JiaShengCunJiangZhang.xml",
}; 

enum
{
	PET_CHENG_HAO_XU_LING,
	PET_CHENG_HAO_QIANG_HUA,
	PET_CHENG_HAO_CHAO_NENG,
	PET_CHENG_HAO_LIN_HUN,

	MAX_PET_CHENG_HAO_NUM
};

static std::map<int,Ogre::HUIRES> gs_mapDesignationTex;

const char* getDesignationTexturePath( int nDesignationID )
{
	const tagDesignationDef* DesignationDef = &DBDataManager::getSingleton().getDesignationDef( nDesignationID );
	if ( DesignationDef == NULL )
	{
		return "";
	}
	return DesignationDef->EffectPath;
}

Ogre::HUIRES getDesignationTexRes( int nDesignationID )
{
	std::map<int,Ogre::HUIRES>::iterator it = gs_mapDesignationTex.find( nDesignationID );
	if( it != gs_mapDesignationTex.end() )
	{
		return it->second;
	}
	const tagDesignationDef* DesignationDef = &DBDataManager::getSingleton().getDesignationDef( nDesignationID );
	if ( DesignationDef == NULL )
	{
		return NULL;
	}
	Ogre::UIRenderer *prenderer = Ogre::UIRenderer::getSingletonPtr();
	Ogre::HUIRES  h = prenderer->CreateTexture( DesignationDef->EffectPath, Ogre::BLEND_ALPHABLEND );
	gs_mapDesignationTex[nDesignationID] = h;
	return h;
}

Ogre::HUIRES RoleText::getVipTexRes( int vipID )
{
	std::map<int,Ogre::HUIRES>::iterator it = m_sMapVipTex.find( vipID );
	if( it != m_sMapVipTex.end() )
	{
		return it->second;
	}
	const tagStarStoneDef* pDef = DBDataManager::getSingleton().getStarStoneDef(vipID);
	if( pDef == NULL )
	{
		m_sMapVipTex[vipID] = NULL;
		return NULL;
	}
	char buf[MAX_PATH];
	sprintf( buf, "uires\\ui\\Za\\TeQuanHuiZhang\\%d.tga",vipID );
	Ogre::HUIRES h = Ogre::UIRenderer::getSingletonPtr()->CreateTexture( buf, Ogre::BLEND_ALPHABLEND );
	gs_mapDesignationTex[vipID] = h;
	return h;
}

const std::string strFontConfigPath = "db/skill/font.xml";

const float TITLE_FOOT_LINE			= 8.0f;
const float TITLE_FIRST_LINE		= -1.0f;
const float TITLE_SECOND_LINE		= -1.0f;
const float TITLE_THIRD_LINE		= -2.0f;
const float TITLE_FORTH_LINE		= -3.2f;

const float MONSTER_FIRST_LINE		= -1.0f;
const float MONSTER_SECOND_LINE		= -1.2f;
const float MONSTER_THIRD_LINE		= -2.3f;
const float MONSTER_FORTH_LINE		= -3.3f;

const int POP_WINDOW_THIRD_LINE		= 22;
const int POP_WINDOW_FORTH_LINE		= 32;
const int POP_WINDOW_XVALUE_ADJUST	= 14;

const int PLAYER_GENDER_MALE = 1;
const int PLAYER_GENDER_FEMALE = 2;

const unsigned int MIN_FADE_TIME = 200;
static unsigned int MIN_DELAY_TIME = 100;
static unsigned int MAX_CHANNELS_SAME_TIME = 4;

const int MONSTER_TITLE_COLORLIST[]	= {
	0xffc6c11a,		// 不攻击
	0xffff0000,		// 主动攻击
	0xfff6ff00,		// 被动攻击
};/*  Ver.10 怪物攻击类型 Bind Macrosgroup:MON_ATK_TYPE,*/

vector<IDrawTitleIcon *> RoleText::m_lstTitleDrawer;
bool RoleText::s_mIsLoadingMap = false;

std::multimap<int, tagFont>	RoleText::m_mapFontType;	// 字体类型表

std::map<int,Ogre::HUIRES> RoleText::m_sMapVipTex;

void AddFlyFont(int fontType, int fontTypeTex, const char *text, int color/* = 0xffffff*/)
{
	MainGamePlayer *pActor = ActorManager::getSingleton().getMainPlayer();
	if (pActor != NULL)
	{
		RoleText::tagFlyFont	font;
		tagFlyText				flyText;
		font.m_pFontType		= RoleText::getFontType(fontType);
		font.m_vectorText.clear();

		flyText.m_Red		= (color >> 16);
		flyText.m_Green		= ((color >> 8) & 0xff);
		flyText.m_Blue		= (color & 0xff);
		flyText.m_Text		= text;
		flyText.m_nFontType	= fontTypeTex;
		font.m_vectorText.push_back(flyText);
		font.m_uStartTime	= timeGetTime() + font.m_pFontType->m_nDelayTime;
		pActor->getRoleText().AddFlyFont(font);
	}
}

IActorTitleDrawer* CreateActorTitleDrawer( GameActor* pActor )
{
	if ( pActor->getType() == GAT_PLAYER )
	{
		return new OtherplayerTitleDrawer( pActor );
	}

	return NULL;
}

RoleText::RoleText(void): m_bDrawTitle( false ), m_pOwner( NULL ), m_pActorTitleDrawControl( NULL )
{
	m_pActorTitleDrawControl = NULL;
}

RoleText::~RoleText(void)
{
	OGRE_DELETE( m_pActorTitleDrawControl );
}

void RoleText::Update( float dtick )
{
	if ( m_pActorTitleDrawControl == NULL )
	{
		return;
	}

	m_pActorTitleDrawControl->Update( dtick );
}

void RoleText::loadFontConfig()
{
	Ogre::XMLData xmldata;
	if( !xmldata.loadFile( strFontConfigPath ) )
	{
		LOG_SEVERE("load font config error: %s", strFontConfigPath.c_str());
		return;
	}
	
	XMLNode	pRootNode = xmldata.getRootNode();
	XMLNode	pChildNode = pRootNode.iterateChild();
	while ( !pChildNode.isNull() )
	{
		if( stricmp( pChildNode.getName(), "fonts" ) == 0 )
		{
			XMLNode child = pChildNode.iterateChild();
			while(!child.isNull())
			{
				tagFont font;
				font.m_nFontFlyId			= child.attribToInt("id");			
				font.m_nFontLife			= child.attribToInt("life");;	
				font.m_nSpeed		 		= child.attribToInt("speed");	
				font.m_fOffX				= child.attribToFloat("offx");
				font.m_fOffY				= child.attribToFloat("offy");
				font.m_fScal				= child.attribToFloat("scal");
				font.m_nScalTime			= child.attribToInt("scaltime");
				font.m_nScalOverTime		= child.attribToInt("scalovertime");
				font.m_nFlyType				= child.attribToInt("flytype");
				font.m_fInitOffX			= child.attribToFloat("initOffx");
				font.m_fInitOffY			= child.attribToFloat("initOffy");
				font.m_fDeltaDistY			= child.attribToFloat("distOffy");
				font.m_bIsHeavyAttack		= child.attribToBool("heavyAttack");

				if ( child.hasAttrib("delayTime") )
				{
					float fDelaySecond			= child.attribToFloat("delayTime");
					font.m_nDelayTime			= fDelaySecond * 1000;
				}
				else
				{
					font.m_nDelayTime			= 0;
				}
			
				
				if ( stricmp( child.attribToString( "reltive" ), "foot" ) == 0 )
				{
					font.m_nReltivePoint = RELATIVE_TO_FOOT;
				}
				else
				{
					font.m_nReltivePoint = RELATIVE_TO_HEAD;
				}
				font.m_nReltiveOffset		= child.attribToInt("relOffset");
				
				m_mapFontType.insert( std::make_pair( font.m_nFontFlyId, font ) );
				child = pChildNode.iterateChild(child);
			}
		}

		if( stricmp( pChildNode.getName(), "ExtraControl" ) == 0 )
		{
			MIN_DELAY_TIME			= pChildNode.attribToInt("delayTime");
			MAX_CHANNELS_SAME_TIME	= pChildNode.attribToInt("maxChannels");
		}

		pChildNode = pRootNode.iterateChild(pChildNode);
	}
}

void RoleText::initFont()
{
	Ogre::UIRenderer *prenderer = Ogre::UIRenderer::getSingletonPtr();
	
	s_hFontTexture[NORMAL_MAIN_ATTACK_FONT]		= prenderer->CreateBitmapFont( strFontPath[NORMAL_MAIN_ATTACK_FONT].c_str(), 
		Ogre::CCODING_GBK );

	s_hFontTexture[NAME_FONT]					= prenderer->CreateTrueTypeFont( 14, 14, strFontPath[NAME_FONT].c_str(), 
		Ogre::CCODING_GBK );

	s_hFontTexture[NORMAL_MAIN_PET_ATTACK_FONT]	= prenderer->CreateBitmapFont( strFontPath[NORMAL_MAIN_PET_ATTACK_FONT].c_str(), 
		Ogre::CCODING_GBK );

	s_hFontTexture[SKILL_MAIN_NORMAL_ATTACK_FONT] = prenderer->CreateBitmapFont( strFontPath[SKILL_MAIN_NORMAL_ATTACK_FONT].c_str(), 
		Ogre::CCODING_GBK );
	s_hFontTexture[NORMAL_MAIN_DAMAGED_FONT]	= prenderer->CreateBitmapFont( strFontPath[NORMAL_MAIN_DAMAGED_FONT].c_str(), 
		Ogre::CCODING_GBK );
	s_hFontTexture[NORMAL_MAINPET_DAMAGED_FONT]	= prenderer->CreateBitmapFont( strFontPath[NORMAL_MAINPET_DAMAGED_FONT].c_str(), 
		Ogre::CCODING_GBK );
	s_hFontTexture[NORMAL_MAIN_HEAVY_ATTACK_FONT]	= prenderer->CreateBitmapFont( strFontPath[NORMAL_MAIN_HEAVY_ATTACK_FONT].c_str(), 
		Ogre::CCODING_GBK );
	s_hFontTexture[HEAVY_MAINPET_ATTACK_FONT]		= prenderer->CreateBitmapFont( strFontPath[HEAVY_MAINPET_ATTACK_FONT].c_str(), 
		Ogre::CCODING_GBK );
	s_hFontTexture[SKILL_MAIN_HEAVY_ATTACK_FONT]	= prenderer->CreateBitmapFont( strFontPath[SKILL_MAIN_HEAVY_ATTACK_FONT].c_str(), 
		Ogre::CCODING_GBK );
	s_hFontTexture[HEAVY_MAIN_DAMAGED_FONT]		= prenderer->CreateBitmapFont( strFontPath[HEAVY_MAIN_DAMAGED_FONT].c_str(), 
		Ogre::CCODING_GBK );
	s_hFontTexture[HEAVY_MAINPET_DAMAGED_FONT]		= prenderer->CreateBitmapFont( strFontPath[HEAVY_MAINPET_DAMAGED_FONT].c_str(), 
		Ogre::CCODING_GBK );
	s_hFontTexture[HP_ADD_FONT]				= prenderer->CreateBitmapFont( strFontPath[HP_ADD_FONT].c_str(), 
		Ogre::CCODING_GBK );
	s_hFontTexture[MP_ADD_FONT]				= prenderer->CreateBitmapFont( strFontPath[MP_ADD_FONT].c_str(), 
		Ogre::CCODING_GBK );
	s_hFontTexture[MISS_IMMUNE_FONT]		= prenderer->CreateBitmapFont( strFontPath[MISS_IMMUNE_FONT].c_str(), 
		Ogre::CCODING_GBK );
	s_hFontTexture[EXP_ADD_FONT]			= prenderer->CreateBitmapFont( strFontPath[EXP_ADD_FONT].c_str(), 
		Ogre::CCODING_GBK );
	s_hFontTexture[MONEY_ADD_FONT]			= prenderer->CreateBitmapFont( strFontPath[MONEY_ADD_FONT].c_str(), 
		Ogre::CCODING_GBK );
	s_hFontTexture[BINDGOLD_ADD_FONT]		= prenderer->CreateBitmapFont( strFontPath[BINDGOLD_ADD_FONT].c_str(), 
		Ogre::CCODING_GBK );
	s_hFontTexture[HPBAR]					= prenderer->CreateTexture( strFontPath[HPBAR].c_str(), 
		Ogre::BLEND_ALPHABLEND );
	s_hFontTexture[HPBAR_BACKGROUP]			= prenderer->CreateTexture( strFontPath[HPBAR_BACKGROUP].c_str(), 
		Ogre::BLEND_ALPHABLEND );
	s_hFontTexture[TEAM_LEADER]				= prenderer->CreateTexture( strFontPath[TEAM_LEADER].c_str(), 
		Ogre::BLEND_ALPHABLEND );
	s_hFontTexture[TEAM_MEMBER]				= prenderer->CreateTexture( strFontPath[TEAM_MEMBER].c_str(), 
		Ogre::BLEND_ALPHABLEND );

	s_hFontTexture[TASK_SPY_GONG]			= prenderer->CreateTexture( strFontPath[TASK_SPY_GONG].c_str(),
		Ogre::BLEND_ALPHABLEND );
	s_hFontTexture[TASK_SPY_FANG]			= prenderer->CreateTexture( strFontPath[TASK_SPY_FANG].c_str(),
		Ogre::BLEND_ALPHABLEND );
	s_hFontTexture[STRONG_GLORY]			= prenderer->CreateTexture( strFontPath[STRONG_GLORY].c_str(),
		Ogre::BLEND_ALPHABLEND );
	s_hFontTexture[MACHINE_COLLECT]			= prenderer->CreateTexture( strFontPath[MACHINE_COLLECT].c_str(),
		Ogre::BLEND_ALPHABLEND );

	/*s_hFontTexture[MONSTER_HPBAR]			= prenderer->CreateTexture( strFontPath[MONSTER_HPBAR].c_str(), 
		Ogre::BLEND_ALPHABLEND );
	s_hFontTexture[MONSTER_HPBAR_BACKGROUP]	= prenderer->CreateTexture( strFontPath[MONSTER_HPBAR_BACKGROUP].c_str(), 
		Ogre::BLEND_ALPHABLEND );*/

	s_hFontTexture[CRYSTAL_FONT]			= prenderer->CreateBitmapFont( strFontPath[CRYSTAL_FONT].c_str(), 
		Ogre::CCODING_GBK );
	s_hFontTexture[ORE_FONT]				= prenderer->CreateBitmapFont( strFontPath[ORE_FONT].c_str(), 
		Ogre::CCODING_GBK );

	s_hFontTexture[OWN_MONSTER_FLAG]		= prenderer->CreateTexture( strFontPath[OWN_MONSTER_FLAG].c_str(), 
		Ogre::BLEND_ALPHABLEND );

	s_hFontTexture[MACHINE_ENERGY]			= prenderer->CreateBitmapFont( strFontPath[MACHINE_ENERGY].c_str(), 
		Ogre::CCODING_GBK );
	s_hFontTexture[MACHINE_RESOURCE]		= prenderer->CreateBitmapFont( strFontPath[MACHINE_RESOURCE].c_str(), 
		Ogre::CCODING_GBK );
	s_hFontTexture[RUNE_EX_DO]				= prenderer->CreateTrueTypeFont(24, 24, strFontPath[RUNE_EX_DO].c_str(), 
		Ogre::CCODING_GBK);	
	s_hFontTexture[ZHENGYI_ADD_FONT]		= prenderer->CreateBitmapFont( strFontPath[ZHENGYI_ADD_FONT].c_str(), 
		Ogre::CCODING_GBK );
	s_hFontTexture[LIFTBRAND_ADD_FONT]		= prenderer->CreateBitmapFont( strFontPath[LIFTBRAND_ADD_FONT].c_str(),
		Ogre::CCODING_GBK );
	loadFontConfig();
}

void RoleText::releaseFont()
{
	Ogre::UIRenderer *prenderer = Ogre::UIRenderer::getSingletonPtr();
	/*
	!!!字体文件不需要释放
	prenderer->ReleaseTrueTypeFont(s_hFontTexture[NAME_FONT]);
	prenderer->ReleaseUIRes(s_hFontTexture[NORMAL_HIT_FONT]);
	prenderer->ReleaseUIRes(s_hFontTexture[HEAVY_HIT_FONT]);
	prenderer->ReleaseUIRes(s_hFontTexture[HP_ADD_FONT]);
	prenderer->ReleaseUIRes(s_hFontTexture[MP_ADD_FONT]);
	prenderer->ReleaseUIRes(s_hFontTexture[MISS_IMMUNE_FONT]);
	*/
	prenderer->ReleaseUIRes(s_hFontTexture[HPBAR]);
	prenderer->ReleaseUIRes(s_hFontTexture[HPBAR_BACKGROUP]);
	prenderer->ReleaseUIRes(s_hFontTexture[OWN_MONSTER_FLAG]);
	prenderer->ReleaseUIRes(s_hFontTexture[TEAM_LEADER]);
	prenderer->ReleaseUIRes(s_hFontTexture[TEAM_MEMBER]);
	prenderer->ReleaseUIRes(s_hFontTexture[TASK_SPY_GONG]);
	prenderer->ReleaseUIRes(s_hFontTexture[TASK_SPY_FANG]);
	prenderer->ReleaseUIRes(s_hFontTexture[STRONG_GLORY]);
	prenderer->ReleaseUIRes(s_hFontTexture[MACHINE_COLLECT]);
	std::map<int,Ogre::HUIRES>::iterator it = gs_mapDesignationTex.begin();
	for( ; it != gs_mapDesignationTex.end(); ++it )
	{
		prenderer->ReleaseUIRes(it->second);
	}
	gs_mapDesignationTex.clear();
	it = m_sMapVipTex.begin();
	for( ; it != m_sMapVipTex.end(); ++it )
	{
		prenderer->ReleaseUIRes( it->second );
	}
	m_sMapVipTex.clear();
}

bool RoleText::hasFontType( int id )
{
	std::multimap<int, tagFont>::iterator iter = m_mapFontType.find( id );
	if( iter != m_mapFontType.end() )
	{
		return true;
	}
	return false; 
}

tagFont* RoleText::getFontType( int id, bool isHeavyAttack )
{
	FLY_FONT_ITER beg	= m_mapFontType.lower_bound( id );
	FLY_FONT_ITER end	= m_mapFontType.upper_bound( id );
	FLY_FONT_ITER iter	= beg;
	
	tagFont* pFont = NULL;
	if ( beg != end )
	{
		tagFont& oneFontConfig = beg->second;
		pFont = &oneFontConfig;
	}

	while ( iter != end )
	{
		tagFont& oneFontConfig = iter->second;
		if ( oneFontConfig.m_bIsHeavyAttack == isHeavyAttack )
		{
			pFont = &oneFontConfig;
			break;
		}
		++iter;
	}

	return pFont; 
}

void RoleText::CreateActorTitleControl()
{
	m_pActorTitleDrawControl = CreateActorTitleDrawer( m_pOwner );
}

void RoleText::setOwner( GameActor* pActor )
{
	m_pOwner = pActor;
}

void RoleText::setEnableDrawTitle( bool flag )
{
	//在特殊的副本不能显示
	if (m_pOwner->IsInSpecialPworldNotShowTitle())
	{
		return;
	}
	m_bDrawTitle = flag;
	if ( m_pActorTitleDrawControl == NULL )
	{
		return;
	}

	m_pActorTitleDrawControl->SetForceDrawFlag( flag );
}

void RoleText::AddFlyFont( tagFlyFont& t )
{
	m_vecFont.push_back( t );
}

void RoleText::BuildTitle()
{
	if ( m_pActorTitleDrawControl == NULL )
	{
		return;
	}

	m_pActorTitleDrawControl->BuildTitle();
}

void RoleText::UpdateTextColor()
{
	if ( m_pActorTitleDrawControl == NULL )
	{
		return;
	}

	m_pActorTitleDrawControl->UpdateTextColor();
}

void RoleText::UpdateTitleHp()
{
	if ( m_pActorTitleDrawControl == NULL )
	{
		return;
	}

	m_pActorTitleDrawControl->UpdateTitleHp();
}


Ogre::Vector3 RoleText::GetTopTitlePos( bool bFiexedPos )
{
	Ogre::ShowActor* pShowActor = m_pOwner->getShowActor();

	Ogre::Vector3 pos = pShowActor->getCurEntity()->getWorldPosition();
	if ( m_pOwner->getType() == GAT_MAINPLAYER || m_pOwner->getType() == GAT_PLAYER )
	{
		if ( m_pOwner->m_Rider.isOnHorse() )
		{
			pos = pShowActor->getCurEntity()->getAnchorWorldPos( 106 );
		}
		else if( m_pOwner->canActWingFloat() )
		{
			pos = pShowActor->getCurEntity()->getAnchorWorldPos( 106 );
		}
		else
		{
			if ( !bFiexedPos && m_pOwner->isJumping() )
			{
				//TODO HERE 在变身的时候还是需要改变高度的。。。。
				pos.y = pShowActor->getCurEntity()->getAnchorWorldPos( 106 ).y;
			}
			else
			{
				pos.y += pShowActor->getActorHeight() * pShowActor->getCurEntity()->getScale().y;
			}
		}
	}
	else
	{
		if ( !bFiexedPos && pShowActor->getCurEntity()->hasAnimPlaying( 10133 ) || 
				pShowActor->getCurEntity()->hasAnimPlaying( 10134 ) )
		{
			pos.y = pShowActor->getCurEntity()->getAnchorWorldPos( 106 ).y;
		}
		else
		{
			pos.y += pShowActor->getActorHeight();
		}
	}
	
	return pos;
}

void RoleText::OnDrawPlayerTitles( Ogre::Vector3& screenPos, Ogre::Vector3& worldPos, FollowCamera *pcamera )
{
	Ogre::ShowActor* pShowActor = m_pOwner->getShowActor();
	// TODO: 临时加来测试的
	if ( m_pActorTitleDrawControl == NULL )
	{
		drawPlayerTitle( pcamera );
	}
	else
	{
		m_pActorTitleDrawControl->DrawTitle( pcamera->GetCameraNode() );
	}
	if( m_pOwner->isInStall() )
	{
		drawPlayerStallTextTitle( pcamera );
	}
	if ( !pShowActor->isInMap() )
	{
		return;
	}
	
	if( m_pActorTitleDrawControl != NULL )
	{
		float yoffset = m_pActorTitleDrawControl->GetNameDrawYOffset();
		drawPlayerVip( pcamera, m_pOwner, yoffset );
	}
	if ( m_pOwner->m_Rider.isOnHorse() )
	{
		worldPos = pShowActor->getCurEntity()->getAnchorWorldPos( 106 );
		worldPos.y += 150; 
		pcamera->GetCameraNode()->pointWorldToWindow( screenPos, worldPos );
		drawPopWindow( screenPos );
	}
	else
	{
		drawPopWindow( screenPos );
	}		
}

// 绘制角色头顶信息
void RoleText::drawTitles( FollowCamera* pcamera )
{	
	if( m_pOwner == NULL )
	{
		return;
	}
	if (!m_pOwner->isShow() || m_pOwner->isOptimizeClear())
	{
		if( m_pOwner->isInStall() )
		{
			char strUIName[256];
			sprintf( strUIName,"%s%d的摊位的头顶UI", m_pOwner->getName(),m_pOwner->GetID() );
			g_pFrameMgr->hideFrame( strUIName );
		}
		return;
	}
	
	Ogre::ShowActor* pShowActor = m_pOwner->getShowActor();
	if ( pShowActor == NULL || pcamera == NULL )
	{
		return;
	}

	Ogre::Vector3 pos = pShowActor->getCurEntity()->getWorldPosition();
	pos.y += pShowActor->getActorHeight() * pShowActor->getCurEntity()->getScale().y + 100;
	Ogre::Vector3 scrpos;
	bool bFront = pcamera->GetCameraNode()->pointWorldToWindow( scrpos, pos );

	if ( bFront )
	{
		switch( m_pOwner->getType() )
		{
		case GAT_MAINPLAYER:
			{
				if (m_pOwner->IsInSpecialPworldNotShowTitle())
				{
					return;
				}
				drawMainPlayerTitle( pcamera );
				m_pOwner->m_Pet.UpdateFreedomPetPopRich( pcamera );
				if ( m_pOwner->m_Rider.isOnHorse())
				{
					MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
					assert( pMainPlayer->getRider()->getHorseId() != 0 );

					pos = pShowActor->getCurEntity()->getAnchorWorldPos( 106 );
					pos.y += 150;
					pcamera->GetCameraNode()->pointWorldToWindow( scrpos, pos );

					drawPopWindow( scrpos );
				}else{
					drawPopWindow( scrpos );
				}
			}
			break;
		case GAT_PLAYER:
			{
				if (m_pOwner->IsInSpecialPworldNotShowTitle())
				{
					return;
				}
				OnDrawPlayerTitles( scrpos, pos, pcamera );
			}
			break;
		case GAT_NPC:
			{
				drawNpcTitle( pcamera );
				int nOffsetY = 0;
				int nOffsetX = 0;

				// 取出NPC泡泡框的高度偏移

				char szPopName[256];
				sprintf( szPopName, "%d",m_pOwner->GetID() );

				std::map<std::string, tagPopWin>::iterator iter = g_pFrameMgr->m_PopWinMap.find( szPopName );
				if( iter != g_pFrameMgr->m_PopWinMap.end() )
				{
					Ogre::Vector3 pos = pShowActor->getCurEntity()->getWorldPosition();
					pos.y += pShowActor->getActorHeight() + 100;
					pcamera->GetCameraNode()->pointWorldToWindow( scrpos, pos );
					scrpos.x += iter->second.m_PopOffset.x;
					scrpos.y += iter->second.m_PopOffset.y; 
				}
				
				GameNPC* pNpc = (GameNPC*)m_pOwner;
				if ( pNpc->m_strNpcFunctionDes.empty() )
				{
					g_pFrameMgr->updatePopWinPos( szPopName, scrpos.x + POP_WINDOW_XVALUE_ADJUST, scrpos.y - POP_WINDOW_THIRD_LINE );
				}else{
					g_pFrameMgr->updatePopWinPos( szPopName, scrpos.x + POP_WINDOW_XVALUE_ADJUST, scrpos.y );
				}			
			}
			break;
		case GAT_PET:
			{
				if (m_pOwner->IsInSpecialPworldNotShowTitle())
				{
					return;
				}
				drawPetTitle( pcamera );
			}
			break;
		case GAT_MONSTER:
			{
				if (m_pOwner->IsInSpecialPworldNotShowTitle())
				{
					return;
				}
				if( m_pOwner->getSubType() == MON_SUB_TYPE_MACHINE )
				{
					OnDrawPlayerTitles( scrpos, pos, pcamera );
				}else{
					drawMonsterTitle( pcamera );

					char szFlyActorName[256];
					int nActorID = m_pOwner->GetID();
					sprintf( szFlyActorName, "%s%d",  m_pOwner->getName(), nActorID );
					std::map<std::string, tagPopWin>::iterator iter = g_pFrameMgr->m_PopWinMap.find( szFlyActorName );

					if( iter != g_pFrameMgr->m_PopWinMap.end() )
					{
						Ogre::Vector3 pos = pShowActor->getCurEntity()->getWorldPosition();
						pos.y += pShowActor->getActorHeight() * pShowActor->getCurEntity()->getScale().y + 100;
						pcamera->GetCameraNode()->pointWorldToWindow( scrpos, pos );
						scrpos.x += iter->second.m_PopOffset.x;
						scrpos.y += iter->second.m_PopOffset.y; 
					}
					g_pFrameMgr->updatePopWinPos( szFlyActorName, scrpos.x + POP_WINDOW_XVALUE_ADJUST, 
						scrpos.y - POP_WINDOW_THIRD_LINE );
				}
			}
			break;
		case GAT_ITEM:
			{
				drawTmpItemTitle( pcamera );
				// TODO: 位置要调节是错误的
				char szFlyActorName[256];
				sprintf( szFlyActorName, "%s%d",  m_pOwner->getName(), m_pOwner->GetID() );
				std::map<std::string, tagPopWin>::iterator iter = g_pFrameMgr->m_PopWinMap.find( szFlyActorName );
				if( iter != g_pFrameMgr->m_PopWinMap.end() )
				{
					Ogre::Vector3 pos = pShowActor->getCurEntity()->getWorldPosition();
					pos.y += pShowActor->getActorHeight() * pShowActor->getCurEntity()->getScale().y + 100;
					pcamera->GetCameraNode()->pointWorldToWindow( scrpos, pos );
					scrpos.x += iter->second.m_PopOffset.x;
					scrpos.y += iter->second.m_PopOffset.y; 
				}
				g_pFrameMgr->updatePopWinPos( szFlyActorName, scrpos.x + POP_WINDOW_XVALUE_ADJUST, 
												scrpos.y - POP_WINDOW_THIRD_LINE );
			}
			break;
		case GAT_EXTRA_NPC:
			{
				drawNpcTitle( pcamera );
				int nOffsetY = 0;
				int nOffsetX = 0;

				// 取出NPC泡泡框的高度偏移

				char szPopName[256];
				sprintf( szPopName, "%d%s",m_pOwner->GetID(),m_pOwner->getName() );

				std::map<std::string, tagPopWin>::iterator iter = g_pFrameMgr->m_PopWinMap.find( szPopName );
				if( iter != g_pFrameMgr->m_PopWinMap.end() )
				{
					Ogre::Vector3 pos = pShowActor->getCurEntity()->getWorldPosition();
					pos.y += pShowActor->getActorHeight() + 100;
					pcamera->GetCameraNode()->pointWorldToWindow( scrpos, pos );
					scrpos.x += iter->second.m_PopOffset.x;
					scrpos.y += iter->second.m_PopOffset.y; 
				}

				GameNPC* pNpc = (GameNPC*)m_pOwner;
				if ( pNpc->m_strNpcFunctionDes.empty() )
				{
					g_pFrameMgr->updatePopWinPos( szPopName, scrpos.x + POP_WINDOW_XVALUE_ADJUST, scrpos.y - POP_WINDOW_THIRD_LINE );
				}else{
					g_pFrameMgr->updatePopWinPos( szPopName, scrpos.x + POP_WINDOW_XVALUE_ADJUST, scrpos.y );
				}			
			}
			break;
		}
	}
	else
	{
		char szPopName[256];
		if ( m_pOwner->getType() == GAT_NPC || m_pOwner->getType() == GAT_MONSTER )
		{
			sprintf( szPopName, "%d", m_pOwner->GetID() );
		}
		else if ( m_pOwner->getType() == GAT_EXTRA_NPC)
		{
			sprintf( szPopName, "%d%s", m_pOwner->GetID(),m_pOwner->getName() );
		}
		else
		{
			sprintf( szPopName, "%s", m_pOwner->getName() );
		}
		g_pFrameMgr->hidePopWin( szPopName );
	}
}

void RoleText::drawItemUITitle( FollowCamera* pcamera )
{
	char szUIName[256];
	sprintf( szUIName, "%s%d的UI名字", m_pOwner->getName(), m_pOwner->GetID() );
	if ( isLoadingMap() )
	{
		g_pFrameMgr->hideFrame( szUIName );
		return;
	}

	float fWidth = 0.0f;
	float fHeight = 0.0f;
	if (m_pOwner->getItemNum() > 1)
	{
		char name[256];
		sprintf(name, "%sx%d", m_pOwner->getName(), m_pOwner->getItemNum());
		g_pDisplay->GetTextExtent( g_pFrameMgr->getFontInfoByFontType( "TeenyBlackFont" ), name, 
			fWidth, fHeight);
	}
	else
	{
		g_pDisplay->GetTextExtent( g_pFrameMgr->getFontInfoByFontType( "TeenyBlackFont" ), m_pOwner->getName(), 
			fWidth, fHeight);
	}

	const Ogre::Vector3& pos = GetTopTitlePos();
	Ogre::Vector3 scrpos;
	pcamera->GetCameraNode()->pointWorldToWindow( scrpos, pos );
	
	float fScale = g_pFrameMgr->GetScreenScaleX() < g_pFrameMgr->GetScreenScaleY() ?
							g_pFrameMgr->GetScreenScaleX() : g_pFrameMgr->GetScreenScaleY();
	int x = (int)( scrpos.x - fWidth * 0.5f - 20 * fScale );
	int y = (int)( scrpos.y + TITLE_THIRD_LINE * fHeight - 40 * fScale );

	g_pFrameMgr->updateHeadBindingFrame( szUIName, x,  y, (int)( fWidth + 40 * fScale ), 
											(int)( fHeight + 10 * fScale ) );
}

void RoleText::drawPlayerStallTextTitle( FollowCamera* pcamera )
{
	ActorManager& actorMgr		= ActorManager::getSingleton();
	char strUIName[256];
	sprintf( strUIName,"%s%d的摊位的头顶UI", m_pOwner->getName(),m_pOwner->GetID() );
	if ( isLoadingMap() || ( HIDE_OTHER_PLAYER_AND_PET_AND_STALL == actorMgr.getShowOtherPlayerModelOption() && !actorMgr.isShowOtherPlayerTitle() ) )
	{
		g_pFrameMgr->hideFrame( strUIName );
		return;
	}
	float fWidth	= 0.0f;
	float fHeight	= 0.0f;
	g_pDisplay->GetTextExtent( g_pFrameMgr->getFontInfoByFontType( "FZYBlackFont14" ) ,m_pOwner->getStallName(),
					fWidth, fHeight );
	const Ogre::Vector3& pos = GetTopTitlePos();
	Ogre::Vector3 scrPos;
	pcamera->GetCameraNode()->pointWorldToWindow( scrPos, pos );
	float fScale = g_pFrameMgr->GetScreenScaleX() < g_pFrameMgr->GetScreenScaleY() ? 
							g_pFrameMgr->GetScreenScaleX() : g_pFrameMgr->GetScreenScaleY();
	int x = ( int )( scrPos.x - fWidth * 0.5f - 10 * fScale );
	int y = ( int )( scrPos.y - 30 * fScale);
	g_pFrameMgr->updateHeadBindingFrame( strUIName, x, y, ( int )( fWidth + 20 * fScale ), 
													( int )( fHeight + 10 * fScale ) );
}

void RoleText::drawItemTextTitle( FollowCamera* pcamera )
{
	const char* pOwnerName = m_pOwner->getOwnerName();
	if ( pOwnerName[0] == 0 )
	{
		return;
	}

	int color = MONSTER_TITLE_COLORLIST[ATK_TYPE_PASSIVE];
	drawTopTitles( pcamera->GetCameraNode(), m_pOwner->getName(), color, TITLE_THIRD_LINE );
}

void RoleText::drawIDTextTitle(FollowCamera* pcamera)
{
	MainGamePlayer *player = ActorManager::getSingleton().getMainPlayer();
	if (player == NULL)
	{
		return;
	}
	char szIDText[128];
	sprintf(szIDText,"ID %d  距离 %.1f",m_pOwner->GetID(), m_pOwner->GetPosition().subtract(player->GetPosition()).length());
	drawTopTitles( pcamera->GetCameraNode(),szIDText,SELF_TITLE_COLOR,TITLE_FORTH_LINE );
}

// 绘制临时物品头顶信息
void RoleText::drawTmpItemTitle( FollowCamera* pcamera )
{
	if ( !m_pOwner->isInMap() )
	{
		return;
	}

	if ( m_pOwner->needAddButtonName() )
	{
		drawItemUITitle( pcamera );
	}
	else
	{
		drawItemTextTitle( pcamera );
	}
}

static float s_monsterTitleHeight[] = { 0.0f, -1.1f, -2.3f, -3.4f };

void RoleText::drawMonsterCommonTitle( FollowCamera* pcamera )
{
	
}

void RoleText::drawMonsterChengHao( FollowCamera* pcamera )
{
	const char* pszFuncDesc = ( (GameNPC*)m_pOwner )->getNpcFunDes();
	// TODO
	//const char* pszFuncDesc = "测试功能";
	if ( pszFuncDesc[0] != 0 )
	{
		drawTopTitles( pcamera->GetCameraNode(), pszFuncDesc, NPCFUNDE_TILTLE_COLOR, 
			MONSTER_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + 
			s_monsterTitleHeight[m_posInfo.m_nTitleYIndex++] );
		Ogre::UIRenderer *prenderer = Ogre::UIRenderer::getSingletonPtr();
		m_posInfo.m_fHeightInHpAndFirstText -= 0.5f;
	}
}

void RoleText::drawMonsterOwnerName( FollowCamera* pcamera )
{
	string strOwerDesc;
	const char* pszOwerName = m_pOwner->getOwnerName();
	
	if ( pszOwerName[0] != 0 )
	{
		strOwerDesc = string( pszOwerName ) + "的";
		
	}
	
	if ( strOwerDesc.empty() )
	{
		return;
	}

	drawTopTitles( pcamera->GetCameraNode(), strOwerDesc.c_str(), TEAM_TITLE_COLOR, 
		TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + s_monsterTitleHeight[m_posInfo.m_nTitleYIndex++] );
}

void RoleText::drawMonsterName( FollowCamera* pcamera )
{
	int color = MONSTER_TITLE_COLORLIST[m_pOwner->getAtkType()];
	if (m_pOwner->isCamp(SKILL_TARGET_FRIEND_MON))
	{
		color = MONSTER_TITLE_COLORLIST[0];
	}
	if ( m_pOwner->getType() == GAT_MONSTER)
	{
		if (m_pOwner->getSubType() == MON_SUB_TYPE_COLLECT)
		{
			const tagNpcDisplayInfo& npcDisplayInfo = DBDataManager::getSingleton().GetNpcDisplayInfo( 
				m_pOwner->getResId() );
			if ( npcDisplayInfo.race & MONSTER_PET )
			{
				color = 0xffD96D08;
			}
		}
		else if (m_pOwner->getSubType() == MON_SUB_TYPE_TOTEM)
		{
			if (m_pOwner->m_ObjCamp & SKILL_TARGET_ENEMY_MON)
			{
				color = TMPENEMY_TITLE_COLOR;
			}
			else
			{
				color = PLAYER_TITLE_COLOR;
			}
		}
		else if (m_pOwner->getCurMap()->getRealId() / 100 == 300)// 百层塔的怪都是主动攻击
		{
			color = MONSTER_TITLE_COLORLIST[1];
		}		
	}

	GameActor_PVP& actorPvpMgr = GameActor_PVP::getSingleton();
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
			color = AMOK_TITLE_COLOR;
		}
	}

	int amok = actorPvpMgr.getAmok( m_pOwner );
	if (amok == 1)
	{
		color = PKER_TITLE_COLOR;
	}
	else if (amok == 2)
	{
		color = PKEST_TITLE_COLOR;
	}
	vector<void *> icons;
	if ( m_pOwner->getSubType() == MON_SUB_TYPE_SINGLE_SAFECAR ||
		( m_pOwner->m_nHasOwner == 1 && m_pOwner->m_DartCarType > 0 ) )
	{
		icons.push_back( s_hFontTexture[OWN_MONSTER_FLAG] );
		m_posInfo.m_fHeightInHpAndFirstText -= 0.5f;
	}
	//跨服中乘骑机甲兽的玩家名字处理
	std::string ownerName = m_pOwner->getName();
	ownerName = ActorManager::getSingleton().replaceSpecialCode(ownerName);
	if( m_pOwner->getServerId() != -1 )
	{
		ownerName = string("[") + m_pOwner->getActorWorldName() + string("]") + ownerName;
	}

	float yoffset = MONSTER_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + s_monsterTitleHeight[m_posInfo.m_nTitleYIndex++];
	if( m_pOwner->getMonsterMachine()->getPassengerNum() > 0 )
	{
		GameActor* pActor = ActorManager::getSingleton().FindActor( m_pOwner->getMonsterMachine()->getPassenger(0) );
		if( pActor != NULL )
		{
			drawPlayerVip(pcamera,pActor,yoffset);
		}
	}
	ownerName = string( GetSpecialActivityAddInfo(m_pOwner) ) + ownerName;
	
	drawTopTitles( pcamera->GetCameraNode(), ownerName.c_str(), color, 
		yoffset, 0.0f, icons.size() > 0 ? &icons[0] : NULL,icons.size());
}

void RoleText::drawMonsterHP( FollowCamera* pcamera )
{
	float pre = (float)m_pOwner->getHP() / m_pOwner->getMaxHP();
	if ( m_pOwner->getMaxHP() <= 0 )
	{
// 		LOG_WARNING( "ERROR:Monster's maxHp is smaller then 0!" );
		assert( false );
	}

	if ( m_pOwner->getHP() <= 0 || m_pOwner->isDead() )
	{
		pre = 0;
	}

	if ( pre > 1)
	{
		pre = 1;
	}

	drawHp( pcamera->GetCameraNode(), pre, MONSTER_FIRST_LINE );
	m_posInfo.m_fHeightInHpAndFirstText = 0.0f;
}

void RoleText::drawMonsterOre( FollowCamera* pcamera )
{
	if ( m_pOwner->m_MonsterMachine.m_monsterInfo.m_nCollectType > 0 )
	{
		char szNumText[10];
		sprintf(szNumText,"%d", m_pOwner->m_MonsterMachine.getTotalOre());
		drawTopTitles( pcamera->GetCameraNode(),szNumText,SELF_TITLE_COLOR,MONSTER_FIRST_LINE + 1.6f );

		vector<void *> icons;
		icons.push_back( s_hFontTexture[MACHINE_COLLECT] );
		drawTopTexture( pcamera->GetCameraNode(), MONSTER_FIRST_LINE + 2.4f, 18.0f
			, &icons[0], icons.size() );
	}
}

bool RoleText::canDrawMonsterTitle()
{
	if ( m_pOwner == NULL )
	{
		return false;
	}
	
	const tagMonsterDef& monsterDef = DBDataManager::getSingleton().getMonsterDef( m_pOwner->getResId() );
	if( ( monsterDef.MiscType & MON_MISC_TYPE_WORD_PWD ) || ( monsterDef.MiscType & MON_MISC_TYPE_JIAJU ) )
	{
		return true;
	}

	if ( m_bDrawTitle )
	{
		return true;
	}
	
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer->GetSelectTarget() == m_pOwner->GetID() ) 
	{
		return true;
	}
	
	if( GetKeyState(VK_MENU) & 0xff00 )
	{
		return true;
	}
	
	if ( m_pOwner->m_nHasOwner == 1 && m_pOwner->GetID() == pMainPlayer->getOwnMonster() )
	{
		return true;
	}

	MonsterMachine *monsterMachine = pMainPlayer->getMonsterMachine();
	if (monsterMachine->m_playerInfo.m_nMonsterMemID == m_pOwner->GetID())
	{
		return true;
	}

 	if( m_pOwner->getSubType() == MON_SUB_TYPE_RONGLU )
 	{
 		return true;
 	}
	if ( m_pOwner->getSubType() == MON_SUB_TYPE_LOST_TOTEM )
	{
		return true;
	}

	return false;	
}

void RoleText::drawMonsterTime(FollowCamera* pcamera)
{
	char szTime[128];
	time_t t = (time_t)(m_pOwner->getMonsterLiveTime()/1000);
	struct tm *time = gmtime( &t );
	strftime(szTime,128,"剩余时间:%H:%M:%S",time);
	drawTopTitles( pcamera->GetCameraNode(), szTime, SELF_TITLE_COLOR, 
		MONSTER_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + 
		s_monsterTitleHeight[m_posInfo.m_nTitleYIndex++] );
	m_posInfo.m_fHeightInHpAndFirstText -= 0.5f;
}

void RoleText::drawRongluCount( FollowCamera* pcamera )
{
	if ( m_pOwner->getSubType() == MON_SUB_TYPE_RONGLU )
	{
		char szNumText[128];
		sprintf(szNumText,"收集能量：[%d/%d]", m_pOwner->m_nKillNum, m_pOwner->m_nKillMax);
		drawTopTitles( pcamera->GetCameraNode(),szNumText,NPCFUNDE_TILTLE_COLOR,MONSTER_FORTH_LINE - 1.2f );
	}	
}

void RoleText::drawLostTotem( FollowCamera* pcamera )
{
	if ( m_pOwner->getSubType() == MON_SUB_TYPE_LOST_TOTEM )
	{
		char szNumText[128];
		int val = m_pOwner->m_nLostTotemExpNum*100/m_pOwner->m_nLostTotemExpMax;
		sprintf(szNumText,"收集经验：%d%%[%d/%d]", val, m_pOwner->m_nLostTotemExpNum , m_pOwner->m_nLostTotemExpMax);
		drawTopTitles( pcamera->GetCameraNode(),szNumText,NPCFUNDE_TILTLE_COLOR,MONSTER_FORTH_LINE - 1.2f );
	}	
}

void RoleText::drawResMonsterOwner( FollowCamera* pcamera )
{
	if ( m_pOwner->IsBaseWarResMon() )
	{
		tdr_ulonglong clanWID = m_pOwner->getBaseWarClanWID() ;
		if ( clanWID != 0 )
		{
			const char *clanName = ClanInfo::getSingleton().getClanName(clanWID);
			char OwnerInfo[256];
			sprintf(OwnerInfo, "[%s]占领中", clanName);
			drawTopTitles( pcamera->GetCameraNode(), OwnerInfo, SELF_TITLE_COLOR, MONSTER_THIRD_LINE );
		}
		else
		{
			drawTopTitles( pcamera->GetCameraNode(), "无人占领", SELF_TITLE_COLOR, MONSTER_THIRD_LINE );
		}
	}
}

// 绘制怪物全部头顶信息
void RoleText::drawMonsterAllTitle( FollowCamera* pcamera )
{
	m_posInfo.m_nTitleYIndex = 0;
	drawMonsterHP( pcamera );

	drawMonsterName( pcamera );
	drawMonsterOwnerName( pcamera );
	drawMonsterChengHao( pcamera );
	drawResMonsterOwner( pcamera );
	if( m_pOwner->getMonsterLiveTime() > 0 )
	{
		drawMonsterTime(pcamera);
	}
	if( m_pOwner->getStickyFrame() ) {
		drawMonsterStickyFrame( pcamera );
	}
	drawRongluCount( pcamera );
	drawLostTotem(pcamera);
}

// 绘制怪物部分头顶信息
void RoleText::drawMonsterPartialTitle( FollowCamera* pcamera )
{

	if( ActorManager::getSingleton().getGameOpt( GAME_OPT_SHOW_MONSTER_HP ) )
	{
		drawMonsterHP( pcamera );
	}
	else
	{
		m_posInfo.m_fHeightInHpAndFirstText = 0.5f;
	}

	if ( ActorManager::getSingleton().getGameOpt( GAME_OPT_SHOW_MONSTER_NAME ) )
	{
		drawMonsterName( pcamera );
		drawMonsterChengHao( pcamera );
	}
}

//绘制怪物头顶信息
void RoleText::drawMonsterTitle( FollowCamera* pcamera )
{
	if ( m_pOwner->getSubType() == MON_SUB_TYPE_BUILD )
	{
		return;
	}

	if ( m_pOwner->getSubType() == MON_SUB_TYPE_COLLECT && !m_pOwner->canCollect() )
	{
		return;
	}
	
	ActorManager& actorMgr		= ActorManager::getSingleton();
	MainGamePlayer* pMainPlayer = actorMgr.getMainPlayer();
	MonsterMachine *monsterMachine = pMainPlayer->getMonsterMachine();
	if (monsterMachine->m_playerInfo.m_nMonsterMemID != m_pOwner->GetID())
	{
		const char* pszOwnerName = m_pOwner->getOwnerName();
		if ( pszOwnerName[0] != 0 && !actorMgr.isShowOtherPlayerTitle() )
		{
			tdr_ulonglong uOwnerTeamWID = m_pOwner->getOwnerTeamWID();
			if ( uOwnerTeamWID == 0 && strcmp( pszOwnerName, pMainPlayer->getName() ) != 0 )
			{
				return;
			}

			if ( uOwnerTeamWID != 0 && !pMainPlayer->isInSameTeam( uOwnerTeamWID ) )
			{
				return;
			}
		}
	}

	// 怪物头顶从下到上依次显示的内容
	// 1.怪物的功能描述；2.怪物的名称
	
	assert( m_pOwner->getAtkType() <= 2 );
	
	m_posInfo.m_nTitleYIndex = 0;
	//drawMonsterCommonTitle( pcamera );
	if( canDrawMonsterTitle() )
	{
		drawMonsterAllTitle( pcamera );
	}
	else
	{
		drawMonsterPartialTitle( pcamera );
	}
}

void RoleText::drawMonsterStickyFrame( FollowCamera* pcamera ) {
	ActorManager& actorMgr		= ActorManager::getSingleton();
	UIObject *pStickyFrame = 0;
	if( isLoadingMap() ) {
		return;
	}
	if( !( pStickyFrame = m_pOwner->getStickyFrame() ) ) {
		return;
	}
	float fWidth	= 132.0f;
	float fHeight	= 30.0f;
	const Ogre::Vector3& pos = GetTopTitlePos();
	Ogre::Vector3 scrPos;
	pcamera->GetCameraNode()->pointWorldToWindow( scrPos, pos );
	float fScale = g_pFrameMgr->GetScreenScaleX() < g_pFrameMgr->GetScreenScaleY() ? 
		g_pFrameMgr->GetScreenScaleX() : g_pFrameMgr->GetScreenScaleY();
	int x = ( int )( scrPos.x - fWidth * 0.5f - 10 * fScale );
	int y = ( int )( scrPos.y - 30 * fScale );
	std::string str = pStickyFrame->GetName();
	g_pFrameMgr->updateHeadBindingFrame( str.c_str(), x, y, ( int )( fWidth + 20 * fScale ), 
		( int )( fHeight + 10 * fScale ) );
}

// 绘制NPC头顶信息
void RoleText::drawNpcTitle( FollowCamera* pcamera )
{
	// NPC头顶从下到上依次显示的内容
	// 1.NPC的名称；2. NPC的功能描述3.	据点地图NPC头顶显示占领公会
	char buffer[256];
	int color = MONSTER_TITLE_COLORLIST[ATK_TYPE_PASSIVE];
	
	sprintf( buffer, "%s", m_pOwner->getName() );
	drawTopTitles( pcamera->GetCameraNode(), buffer, color, TITLE_FIRST_LINE );

	static std::string s_actorTypeDesc[] = { "NPC", "MONSTER", "ITEM", "PLAYER", "MAINPLAYER", "PET", "FAIRY" };
	
	GameNPC* pNpc = (GameNPC*)m_pOwner;
	if ( !pNpc->m_strNpcFunctionDes.empty() )
	{
		if (pNpc->getClanWID() == 0)
		{
			drawTopTitles( pcamera->GetCameraNode(), pNpc->getNpcFunDes(), 
				NPCFUNDE_TILTLE_COLOR, TITLE_THIRD_LINE, -1.0000000, NULL, 0, false);
		}
		else
		{
			const char *clanName = ClanInfo::getSingleton().getClanName(pNpc->getClanWID());
			if (clanName[0] == '\0')
			{
				drawTopTitles( pcamera->GetCameraNode(), pNpc->getNpcFunDes(), 
					NPCFUNDE_TILTLE_COLOR, TITLE_THIRD_LINE, -1.0000000, NULL, 0, false);
			}
			else
			{
				char NpcDes[256];
				sprintf(NpcDes, "[%s]", clanName);
				strcat(NpcDes, pNpc->getNpcFunDes());
				drawTopTitles( pcamera->GetCameraNode(), NpcDes, 
					NPCFUNDE_TILTLE_COLOR, TITLE_THIRD_LINE, -1.0000000, NULL, 0, false);
			}
		}
	}
	int mapID = m_pOwner->getMapId();
	if ( m_pOwner->IsCurMapHasStrongPoint( mapID ) )
	{
		tdr_ulonglong clanWid = m_pOwner->getCurMapStrongOwnerWID( mapID );
		float titlePos = TITLE_THIRD_LINE;
		if ( !pNpc->m_strNpcFunctionDes.empty() )
		{
			titlePos = TITLE_FORTH_LINE;
		}
		if (clanWid != 0  )
		{
			const char *clanName = ClanInfo::getSingleton().getClanName(clanWid);
			char OwnerInfo[256];
			sprintf(OwnerInfo, "[%s]占领中", clanName);
			drawTopTitles( pcamera->GetCameraNode(), OwnerInfo, color, titlePos );
		}
	}
}

float s_playerTitleHeight[] = { -0.2f, -1.2f, -2.3f, -3.3f };

void RoleText::drawPlayerCommonTitle( FollowCamera* pcamera )
{
	vector<void *> icons;
	if ( m_pOwner->m_ViewFlag & VIEW_FLAG_SPY )
	{
		icons.push_back(s_hFontTexture[TASK_SPY_GONG]);
		float m_offsety = 0.0;
		const char* pszDesignation = m_pOwner->getDesignation();
		if ( pszDesignation[0] != 0 )
		{
			m_offsety += -0.8;
		}
		if ( m_pOwner->isInTeam() )
		{
			m_offsety += -0.8;
		}
		drawTopTexture( pcamera->GetCameraNode(), TITLE_FORTH_LINE + m_offsety, 0, &icons[0], icons.size() );
		//drawTopTexture( pcamera->GetCameraNode(), TITLE_FORTH_LINE + m_posInfo.m_fHeightInHpAndFirstText + s_playerTitleHeight[m_posInfo.m_nTitleYIndex++], 0, &icons[0], icons.size() );
	}
	else if ( m_pOwner->m_ViewFlag & VIEW_FLAG_ANTI_SPY )
	{
		icons.push_back(s_hFontTexture[TASK_SPY_FANG]);
		float m_offsety = 0.0;
		const char* pszDesignation = m_pOwner->getDesignation();
		if ( pszDesignation[0] != 0 )
		{
			m_offsety += -0.8;
		}
		if ( m_pOwner->isInTeam() )
		{
			m_offsety += -0.8;
		}
		drawTopTexture( pcamera->GetCameraNode(), TITLE_FORTH_LINE + m_offsety, 0, &icons[0], icons.size() );
		//drawTopTexture( pcamera->GetCameraNode(), TITLE_FORTH_LINE + m_posInfo.m_fHeightInHpAndFirstText + s_playerTitleHeight[m_posInfo.m_nTitleYIndex++], 0, &icons[0], icons.size() );
	}
}

Ogre::HUIRES RoleText::getBattleIcon( GameActor* pOneActor )
{
	for (vector<IDrawTitleIcon *>::iterator iter = m_lstTitleDrawer.begin(), end = m_lstTitleDrawer.end(); 
			iter != end; ++iter)
	{
		IDrawTitleIcon* pOneDrawTitleIconControl = *iter;
		void *p	= pOneDrawTitleIconControl->GetTitleIcon( pOneActor );
		if ( p != NULL )
		{
			return ( Ogre::HUIRES )p;
		}
	}
	return NULL;
}

void RoleText::drawPlayerName( FollowCamera* pcamera )
{
	int color = PLAYER_TITLE_COLOR;

	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();

	switch ( m_pOwner->m_nRelation )
	{
	case RELATION_TYPE_FRIEND:
		{
			color = FRIEND_TITLE_COLOR;
		}
		break;
// 	case CLIENT_RELATION_TYPE_XIONG_SHOU:
// 		{
// 			color = ENEMY_TITLE_COLOR;
// 		}
// 		break;
// 	case CLIENT_RELATION_TYPE_BLACK:
// 		{
// 			color = BLACK_TITLE_COLOR;
// 		}
// 		break;
	default:
		break;
	}

	if ( pMainPlayer->isInSameTeam( m_pOwner->getTeamWID() ) )
	{
		color = TEAMMEMBER_TITLE_COLOR;
	}
	
	// TODO: 图片类添加重构
	vector<void *> icons;
	for (vector<IDrawTitleIcon *>::iterator i = m_lstTitleDrawer.begin(); i != m_lstTitleDrawer.end(); ++i)
	{
		void *p = (*i)->GetTitleIcon(m_pOwner);
		if ( p != NULL )
		{
			icons.push_back(p);
			break;
		}
	}
	if (ActorManager::getSingleton().getGameOpt( GAME_OPT_SHOW_ALLPLAYER_TEAM_DESIGNATION_NAME ))
	{
		const char* pszTeamName = m_pOwner->getTeamName();
		if (pszTeamName[0] != 0)
		{
			if (m_pOwner->m_bCaptain)
			{
				icons.push_back(s_hFontTexture[TEAM_LEADER]);
			}
			else
			{
				icons.push_back(s_hFontTexture[TEAM_MEMBER]);
			}
		}
	}
	if (icons.size() > 0)
	{
		m_posInfo.m_fHeightInHpAndFirstText -= 0.5f;
	}

	int amok = GameActor_PVP::getSingleton().getAmok(m_pOwner);
	if (amok == 1)
	{
		color = PKER_TITLE_COLOR;
	}
	else if (amok == 2)
	{
		color = PKEST_TITLE_COLOR;
	}

	if (GameActor_PVP::getSingleton().hasCrime(m_pOwner))
	{
		unsigned int now = timeGetTime();
		if (now > m_pOwner->m_lastChangeNameColorTime)
		{
			m_pOwner->m_lastChangeNameColorTime = now + 250;
			m_pOwner->m_isLastChangeNameColor = !m_pOwner->m_isLastChangeNameColor;
		}
		if (m_pOwner->m_isLastChangeNameColor)
		{
			color = AMOK_TITLE_COLOR;
		}
	}

	if (GameActor_PVP::getSingleton().isForcePkMode() && GameActor_PVP::getSingleton().getPkMode() != PK_MODE_NORMAL)
	{
		if (m_pOwner->m_ObjCamp & SKILL_TARGET_ENEMY_PLAYER)
		{
			color = BATTLE_ENEMY;
		}
		else if (m_pOwner->m_ObjCamp & SKILL_TARGET_FRIEND_PLAYER)
		{
			color = BATTLE_ALLY;
		}
	}
	std::string name = m_pOwner->getName();
	name = ActorManager::getSingleton().replaceSpecialCode(name);
	if( m_pOwner->getServerId() != -1 )
	{
		name = string("[") + m_pOwner->getActorWorldName() + string("]") + name;
	}
	drawTopTitles( pcamera->GetCameraNode(), name.c_str(), color, 
		TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + s_playerTitleHeight[m_posInfo.m_nTitleYIndex++], 
		0.0f, icons.size() > 0 ? &icons[0] : NULL,
		icons.size());
}

void RoleText::drawPlayerTeamName( FollowCamera* pcamera )
{
	ActorManager& actorManager = ActorManager::getSingleton();
	char szTeamName[256];
	MainGamePlayer* pMainPlayer = actorManager.getMainPlayer();
	const char* pszTeamName = m_pOwner->getTeamName();
	if ( false && pszTeamName[0] != 0 )
	{
		if ( m_pOwner->m_bCaptain )
		{
			if ( std::string( pMainPlayer->getTeamName() ).empty() )
			{
				sprintf( szTeamName,"队长 %s %d/%d",m_pOwner->getTeamName(), m_pOwner->m_nTeamNum, MAX_TEAM_MEMBER);
			}
			else
			{
				sprintf( szTeamName, "队长 %s", m_pOwner->getTeamName() );
			}		
		}
		else
		{
			if ( std::string( pMainPlayer->getTeamName() ).empty() )
			{
				sprintf( szTeamName,"%s %d/%d",m_pOwner->getTeamName(), m_pOwner->m_nTeamNum, MAX_TEAM_MEMBER);
			}
			else
			{
				sprintf( szTeamName, "%s", m_pOwner->getTeamName() );
			}	
		}

		if( pMainPlayer->getCurMap() != NULL )
		{
			drawTopTitles( pcamera->GetCameraNode(), szTeamName, TEAM_TITLE_COLOR, 
							TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + 
							s_playerTitleHeight[m_posInfo.m_nTitleYIndex++] );
		}
	}
	else
	{
		const char* pszDesignation = m_pOwner->getDesignation();
		if( pszDesignation[0] == 0 )
		{
			return;
		}
		// TODO: 加入贴图分支路径
		const char* pszDesignationTexPath = getDesignationTexturePath( m_pOwner->getDesignationID() );
		if ( pszDesignationTexPath[0] == '\0' )
		{
			// TITLE_SECOND_LINE
			const tagDesignationDef* DesignationDef = &DBDataManager::getSingleton().getDesignationDef( m_pOwner->getDesignationID() );
			if ( DesignationDef == NULL )
			{
				drawTopTitles( pcamera->GetCameraNode(), pszDesignation, TEAM_TITLE_COLOR, 
					TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + 
					s_playerTitleHeight[m_posInfo.m_nTitleYIndex++] );
				return;
			}
			const unsigned int nColor = DesignationDef->RGB;
			if ( nColor == 0 )
			{
				drawTopTitles( pcamera->GetCameraNode(), pszDesignation, TEAM_TITLE_COLOR, 
					TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + 
					s_playerTitleHeight[m_posInfo.m_nTitleYIndex++] );
			}
			else
			{
				drawTopTitles( pcamera->GetCameraNode(), pszDesignation, nColor + 0xff000000, 
					TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + 
					s_playerTitleHeight[m_posInfo.m_nTitleYIndex++] );
			}
		}
		else
		{
			vector<void *> icons;
			Ogre::HUIRES hDesignationRes = getDesignationTexRes( m_pOwner->getDesignationID() );
			if ( hDesignationRes == NULL )
			{
				drawTopTitles( pcamera->GetCameraNode(), pszDesignation, TEAM_TITLE_COLOR, 
					TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + 
					s_playerTitleHeight[m_posInfo.m_nTitleYIndex++] );
				return;
			}
			icons.push_back( hDesignationRes );
			drawTopTexture( pcamera->GetCameraNode(), 
				TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + s_playerTitleHeight[m_posInfo.m_nTitleYIndex++] 
				, 0, &icons[0], icons.size() );
		}
	}
}

void RoleText::drawPlayerHP( FollowCamera* pcamera )
{
	if ( m_pOwner->getMaxHP() <= 0 )
	{
		LOG_WARNING( "ERROR:%s's maxHp is smaller then 0!",m_pOwner->getName() );
		assert( 0 );
	}

	float pre = (float)m_pOwner->getHP() / m_pOwner->getMaxHP();

	if ( m_pOwner->getHP() <= 0 || m_pOwner->isDead() )
	{
		pre = 0;
	}

	if ( pre > 1)
	{
		pre = 1;
	}
	
	m_posInfo.m_fHeightInHpAndFirstText = 0.0f;
	float offsetscale = TITLE_FIRST_LINE;
	if( m_pOwner->getGender() == CLIENT_GENDER_FEMALE )
	{
		offsetscale = offsetscale + 0.5f;
		m_posInfo.m_fHeightInHpAndFirstText = 0.5f;
	}

	drawHp( pcamera->GetCameraNode(), pre, offsetscale );
}
const char* RoleText::GetSpecialActivityAddInfo( GameActor* pActor )
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

bool RoleText::canDrawPlayerTitle()
{
	if ( m_pOwner == NULL )
	{
		return false;
	}

	if ( m_bDrawTitle )
	{
		return true;
	}

	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer->GetSelectTarget() == m_pOwner->GetID() ) 
	{
		return true;
	}
	short keyState = ( GetKeyState( VK_MENU ) & 0xff00 );
	if( GetKeyState(VK_MENU) & 0xff00 )
	{
		return true;
	}

	return false;	
}

// 绘制怪物全部头顶信息
void RoleText::drawPlayerAllTitle( FollowCamera* pcamera )
{
	drawPlayerHP( pcamera );

	if( m_pOwner->getType() == GAT_MONSTER )
	{
		drawMonsterOre( pcamera );
		drawMonsterName( pcamera );
	}else{
		drawPlayerName( pcamera );
	}

	drawClanName( pcamera );

	if( m_pOwner->getType() == GAT_MONSTER )
	{
		drawMonsterOwnerClanName( pcamera );
		if( m_pOwner->getMonsterLiveTime() > 0 )
		{
			drawMonsterTime(pcamera);
		}
	}

	drawPlayerTeamName( pcamera );
}

// 绘制玩家部分头顶信息
void RoleText::drawPlayerPartialTitle( FollowCamera* pcamera )
{
	ActorManager& actorManager = ActorManager::getSingleton();
	if ( actorManager.getGameOpt( GAME_OPT_SHOW_OTHERPLAYER_HP ) )
	{
		drawPlayerHP( pcamera );
		drawMonsterOre( pcamera );
	}
	else
	{
		m_posInfo.m_fHeightInHpAndFirstText = 0.5f;
	}

	if ( actorManager.getGameOpt( GAME_OPT_SHOW_ALLPLAYER_AND_PET_NAME ) )
	{
		if( m_pOwner->getType() == GAT_MONSTER )
		{
			drawMonsterName( pcamera );
			if( m_pOwner->getMonsterLiveTime() > 0 )
			{
				drawMonsterTime( pcamera );
			}
		}else{
			drawPlayerName( pcamera );
		}
	}

	if ( actorManager.getGameOpt( GAME_OPT_SHOW_OTHERPLAYER_CLAN_NAME ) )
	{
		drawClanName( pcamera );
	}
	if( m_pOwner->getType() == GAT_MONSTER )
	{
		if( m_pOwner->m_MonsterMachine.isMainPlayerDriver() )
		{
			if ( ActorManager::getSingleton().getGameOpt( GAME_OPT_SHOW_MAIN_CLAN_NAME ) )
			{
				drawMonsterOwnerClanName( pcamera );
			}
		}else{
			if ( ActorManager::getSingleton().getGameOpt( GAME_OPT_SHOW_OTHERPLAYER_CLAN_NAME ) )
			{
				drawMonsterOwnerClanName( pcamera );
			}
		}
	}
	if ( actorManager.getGameOpt( GAME_OPT_SHOW_ALLPLAYER_TEAM_DESIGNATION_NAME ) )
	{
		drawPlayerTeamName( pcamera );
	}
}

// 绘制玩家头顶信息
void RoleText::drawPlayerTitle( FollowCamera* pcamera )
{
	m_posInfo.m_nTitleYIndex = 0;
	// TODO: 隐藏模型修改点
	/*if ( !ActorManager::getSingleton().isShowOtherPlayerModel() )
	{
		return;
	}*/
	ActorManager& actorMgr			= ActorManager::getSingleton();
	MainGamePlayer* pMainplayer		= actorMgr.getMainPlayer();
	if ( !actorMgr.isShowOtherPlayerTitle() )
	{
		if ( pMainplayer == NULL )
		{
			return;
		}
		
		tdr_ulonglong mainplayerTeamWID = pMainplayer->getTeamWID();
		if ( mainplayerTeamWID == 0  )
		{
			return;
		}

		if ( m_pOwner->getTeamWID() != mainplayerTeamWID )
		{
			return;
		}
	}

	if( m_pOwner->isOnMonsterMachine() )
	{
		return;
	}
	
	const char* pszStallName = m_pOwner->getStallName();
	if ( pszStallName[0] != '\0' )
	{
		if ( m_pOwner->isInStall() )
		{
			//drawPlayerStallTitle( pcamera );
			drawTopTitles( pcamera->GetCameraNode(), pszStallName, PLAYER_TITLE_COLOR, TITLE_FIRST_LINE );
			return;
		}
		else
		{
			drawPlayerLoveSignTitle( pcamera );
		}
	}

	drawPlayerCommonTitle( pcamera );

	if ( canDrawPlayerTitle() )
	{
		drawPlayerAllTitle( pcamera );
	}
	else
	{
		drawPlayerPartialTitle( pcamera );
	}
}

void RoleText::drawMainPlayerCommonTitle( FollowCamera* pcamera )
{
	ActorManager& actorManager = ActorManager::getSingleton();
	assert( m_pOwner->getStallName() != NULL );
	int color = SELF_TITLE_COLOR;

	MainGamePlayer* pMainPlayer = static_cast<MainGamePlayer*>( m_pOwner );
	vector<void *> icons;
	if ( m_pOwner->m_ViewFlag & VIEW_FLAG_SPY )
	{
		icons.push_back(s_hFontTexture[TASK_SPY_GONG]);
		float m_offsety = 0.0;
		const char* pszDesignation = m_pOwner->getDesignation();
		if ( pszDesignation[0] != 0 )
		{
			m_offsety += -0.8;
		}
		if ( m_pOwner->isInTeam() )
		{
			m_offsety += -0.8;
		}
		drawTopTexture( pcamera->GetCameraNode(), TITLE_FORTH_LINE + m_offsety, 0, &icons[0], icons.size() );
		//drawTopTexture( pcamera->GetCameraNode(), TITLE_FORTH_LINE + m_posInfo.m_fHeightInHpAndFirstText + s_playerTitleHeight[m_posInfo.m_nTitleYIndex++], 0, &icons[0], icons.size() );
	}
	else if ( m_pOwner->m_ViewFlag & VIEW_FLAG_ANTI_SPY )
	{
		icons.push_back(s_hFontTexture[TASK_SPY_FANG]);
		float m_offsety = 0.0;
		const char* pszDesignation = m_pOwner->getDesignation();
		if ( pszDesignation[0] != 0 )
		{
			m_offsety += -0.8;
		}
		if ( m_pOwner->isInTeam() )
		{
			m_offsety += -0.8;
		}
		drawTopTexture( pcamera->GetCameraNode(), TITLE_FORTH_LINE + m_offsety, 0, &icons[0], icons.size() );
		//drawTopTexture( pcamera->GetCameraNode(), TITLE_FORTH_LINE + m_posInfo.m_fHeightInHpAndFirstText + s_playerTitleHeight[m_posInfo.m_nTitleYIndex++], 0, &icons[0], icons.size() );
	}
}

void RoleText::drawMainPlayerName( FollowCamera* pcamera )
{
	vector<void *> icons;
	for (vector<IDrawTitleIcon *>::iterator i = m_lstTitleDrawer.begin(); i != m_lstTitleDrawer.end(); ++i)
	{
		void *p = (*i)->GetTitleIcon(m_pOwner);
		if (p)
		{
			icons.push_back(p);
			break;
		}
	}
	const char* pszTeamName = m_pOwner->getTeamName();
	if (pszTeamName[0] != 0)
	{
		if (m_pOwner->m_bCaptain)
		{
			icons.push_back(s_hFontTexture[TEAM_LEADER]);
		}
		else
		{
			icons.push_back(s_hFontTexture[TEAM_MEMBER]);
		}
	}

	if (icons.size() > 0)
	{
		m_posInfo.m_fHeightInHpAndFirstText -= 0.5f;
	}

	int amok = GameActor_PVP::getSingleton().getAmok(m_pOwner);
	unsigned int color = SELF_TITLE_COLOR;
	if (m_pOwner->m_uTeamWID != 0)
	{
		color = TEAMMEMBER_TITLE_COLOR;
	}
	if (amok == 1)
	{
		color = PKER_TITLE_COLOR;
	}
	else if (amok == 2)
	{
		color = PKEST_TITLE_COLOR;
	}

	if (GameActor_PVP::getSingleton().hasCrime(m_pOwner))
	{
		unsigned int now = timeGetTime();
		if (now > m_pOwner->m_lastChangeNameColorTime)
		{
			m_pOwner->m_lastChangeNameColorTime = now + 250;
			m_pOwner->m_isLastChangeNameColor = !m_pOwner->m_isLastChangeNameColor;
		}
		if (m_pOwner->m_isLastChangeNameColor)
		{
			color = AMOK_TITLE_COLOR;
		}
	}

	if (GameActor_PVP::getSingleton().isForcePkMode() && GameActor_PVP::getSingleton().getPkMode() != PK_MODE_NORMAL)
	{
		color = BATTLE_ALLY;
	}

	float yoffset = TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + s_playerTitleHeight[m_posInfo.m_nTitleYIndex++];
	drawPlayerVip( pcamera, m_pOwner, yoffset );
	drawTopTitles(pcamera->GetCameraNode(), m_pOwner->getName(), color, 
					yoffset, 0.0f, icons.size() > 0 ? &icons[0] : NULL,
					icons.size());
	
}

void RoleText::drawPlayerVip( FollowCamera* pcamera, GameActor* pActor, float yoffset )
{
	if ( false && !ActorManager::getSingleton().getGameOpt( GAME_OPT_SHOW_STAR_STONE ) )
	{
		int flag = pActor->getVipFlag();
		const Ogre::Vector3& pos = GetTopTitlePos( false );
		Ogre::Vector3 scrpos;
		pcamera->GetCameraNode()->pointWorldToWindow(scrpos, pos);

		float fScale = g_pFrameMgr->GetScreenScaleX()<g_pFrameMgr->GetScreenScaleY()?g_pFrameMgr->GetScreenScaleX():g_pFrameMgr->GetScreenScaleY();

		float fWidth = 0, fHeight = 0;
		Ogre::UIRenderer::getSingletonPtr()->GetTextExtent(s_hFontTexture[NAME_FONT], pActor->getName(), fWidth, fHeight);

		float xoffset = scrpos.x + fWidth * 0.5f * fScale;
		yoffset = scrpos.y + ( yoffset*fHeight-14 ) * fScale;

		for( int i = 1 ; i< 6 ; ++i )
		{
			if( flag &(1<<i) )
			{
				HUIRES h = getVipTexRes( i );
				if( h != NULL )
				{
					int nWidth = 0, nHeight = 0;
					Ogre::UIRenderer::getSingletonPtr()->GetTextureSize(h, nWidth, nHeight);
					Ogre::UIRenderer::getSingletonPtr()->BeginDraw(h);
					if ( fScale < SCALE_CHG_LIMIT_VALUE )
					{
						Ogre::UIRenderer::getSingletonPtr()->StretchRect( xoffset,
							yoffset,nWidth*fScale, nHeight*fScale, 0xffffffff );
					}
					else
					{
						Ogre::UIRenderer::getSingletonPtr()->DrawRect( (int)( xoffset + 0.5f ), 
							(int)( yoffset + 0.5f ),
							nWidth, nHeight, 0xffffffff );
					}				
					Ogre::UIRenderer::getSingletonPtr()->EndDraw();
					xoffset += nWidth;
				}
			}
		}
	}	
}

void RoleText::drawMainPlayerTeamName( FollowCamera* pcamera )
{
	// 绘制队伍头顶信息
	const char* pszTeamName = m_pOwner->getTeamName();
	if ( false && pszTeamName[0] != 0 )
	{
		string strTeamInfo = string( "队长" ) + m_pOwner->getTeamName();
		if ( m_pOwner->m_bCaptain )
		{
			strTeamInfo = string( "队长" ) + m_pOwner->getTeamName();
		}
		else
		{
			strTeamInfo = m_pOwner->getTeamName();
		}
		drawTopTitles( pcamera->GetCameraNode(), strTeamInfo.c_str(), TEAM_TITLE_COLOR, 
						TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + 
						s_playerTitleHeight[m_posInfo.m_nTitleYIndex++] );
	}
	else
	{
		MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
		//在末世殿堂副本显示竞技场等级，原位置上的称号不显示
		if ( pMain->getMapRealId() == SPARTA_MAP_ID )
		{
			drawTopTitles( pcamera->GetCameraNode(), ActorManager::getSingleton().GetSpartaLevelName( m_pOwner->getSpartaFightVal()), 0xff00ff00, 
				TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + 
				s_playerTitleHeight[m_posInfo.m_nTitleYIndex++] );
			return;
		}
		const char* pszDesignation = m_pOwner->getDesignation();
		if( pszDesignation[0] == 0 )
		{
			return;
		}

		const char* pszDesignationTexPath = getDesignationTexturePath( m_pOwner->getDesignationID() );
		if ( pszDesignationTexPath[0] == '\0' )
		{
			// TITLE_SECOND_LINE
			const tagDesignationDef* DesignationDef = &DBDataManager::getSingleton().getDesignationDef( m_pOwner->getDesignationID() );
			if ( DesignationDef == NULL )
			{
				drawTopTitles( pcamera->GetCameraNode(), pszDesignation, TEAM_TITLE_COLOR, 
					TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + 
					s_playerTitleHeight[m_posInfo.m_nTitleYIndex++] );
				return;
			}
			const unsigned int nColor = DesignationDef->RGB;
			if ( nColor == 0 )
			{
				drawTopTitles( pcamera->GetCameraNode(), pszDesignation, TEAM_TITLE_COLOR, 
					TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + 
					s_playerTitleHeight[m_posInfo.m_nTitleYIndex++] );
			}
			else
			{
				drawTopTitles( pcamera->GetCameraNode(), pszDesignation, nColor + 0xff000000, 
					TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + 
					s_playerTitleHeight[m_posInfo.m_nTitleYIndex++] );
			}	
		}
		else
		{
			vector<void *> icons;
			Ogre::HUIRES hDesignationRes = getDesignationTexRes( m_pOwner->getDesignationID() );
			if ( hDesignationRes == NULL )
			{
				drawTopTitles( pcamera->GetCameraNode(), pszDesignation, TEAM_TITLE_COLOR, 
					TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + 
					s_playerTitleHeight[m_posInfo.m_nTitleYIndex++] );
				return;
			}
			icons.push_back( hDesignationRes );
			drawTopTexture( pcamera->GetCameraNode(), 
						TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + s_playerTitleHeight[m_posInfo.m_nTitleYIndex++], 
							0, &icons[0], icons.size() );
		}
	}
}

void RoleText::drawMainPlayerHP( FollowCamera* pcamera )
{
	if ( m_pOwner->getMaxHP() <= 0 )
	{
		LOG_WARNING( "ERROR:%s's maxHp is smaller then 0!",m_pOwner->getName() );
		assert( 0 );
		return;
	}

	float pre = (float)m_pOwner->getHP() / m_pOwner->getMaxHP();
	if ( m_pOwner->getMaxHP() <= 0 )
	{
		LOG_WARNING( "ERROR:Player's maxHp is smaller then 0!" );
		assert( 0 );
	}

	if ( m_pOwner->getHP() <= 0 || m_pOwner->isDead() )
	{
		pre = 0;
	}
	if ( pre > 1)
	{
		pre = 1;
	}

	float offsetscale = TITLE_FIRST_LINE;
	m_posInfo.m_fHeightInHpAndFirstText = 0.0f;
	if( m_pOwner->getGender() == PLAYER_GENDER_FEMALE )
	{
		offsetscale = offsetscale + 0.5f;
		m_posInfo.m_fHeightInHpAndFirstText = 0.5f;
	}

	drawHp( pcamera->GetCameraNode(), pre, offsetscale );
}

bool RoleText::canDrawMainPlayerTitle()
{
	if ( m_pOwner == NULL )
	{
		return false;
	}

	/*if ( m_bDrawTitle )
	{
		return true;
	}*/

	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer->GetSelectTarget() == m_pOwner->GetID() ) 
	{
		return true;
	}
	
	bool isCurMenuDown = ( ( GetKeyState( VK_MENU ) & 0xff00 ) != 0 );
	short keyState = ( GetKeyState( VK_MENU ) & 0xff00 );
	if( GetKeyState(VK_MENU) & 0xff00 )
	{
		return true;
	}

	return false;	
}

// 绘制主角全部头顶信息
void RoleText::drawMainPlayerAllTitle( FollowCamera* pcamera )
{
	drawMainPlayerHP( pcamera );

	drawMainPlayerName( pcamera );

	drawClanName( pcamera );

	drawMainPlayerTeamName( pcamera );

	drawPlayerLoveSignTitle( pcamera );
}

// 绘制主角部分头顶信息
void RoleText::drawMainPlayerPartialTitle( FollowCamera* pcamera )
{
	// TODO: 可以在这配置动态位置
	ActorManager& actorManager = ActorManager::getSingleton();
	
	if( actorManager.getGameOpt( GAME_OPT_SHOW_MAINPLAYER_AND_TEAM_HP ) )
	{
		drawMainPlayerHP( pcamera );
	}
	else
	{
		m_posInfo.m_fHeightInHpAndFirstText = 0.5f;
	}


	if ( actorManager.getGameOpt( GAME_OPT_SHOW_ALLPLAYER_AND_PET_NAME ) )
	{
		drawMainPlayerName( pcamera );
	}

	if ( actorManager.getGameOpt( GAME_OPT_SHOW_MAIN_CLAN_NAME ) )
	{
		drawClanName( pcamera );
	}
	
	if( actorManager.getGameOpt( GAME_OPT_SHOW_ALLPLAYER_TEAM_DESIGNATION_NAME ) )
	{
		drawMainPlayerTeamName( pcamera );
	}

	drawPlayerLoveSignTitle( pcamera );
}

// 绘制主角头顶信息
void RoleText::drawMainPlayerTitle( FollowCamera* pcamera )
{
	int color = SELF_TITLE_COLOR;
	const char* pszStallName = m_pOwner->getStallName();
	m_posInfo.m_nTitleYIndex = 0;
	if( pszStallName[0] != 0 )
	{
		drawTopTitles( pcamera->GetCameraNode(), pszStallName, color, TITLE_FIRST_LINE );
		//drawMainPlayerStallTitle( pcamera );
		return;
	}

	drawMainPlayerCommonTitle( pcamera );
	if ( canDrawMainPlayerTitle() )
	{
		drawMainPlayerAllTitle( pcamera );
	}
	else
	{
		drawMainPlayerPartialTitle( pcamera );
	}
}

// 绘制玩家摊位头顶信息
void RoleText::drawMainPlayerStallTitle( FollowCamera* pcamera )
{
	/*const char* pszStallName = m_pOwner->getStallName();
	if ( pszStallName[0] == 0 )
	{
		return;
	}
	
	char buffer[256];
	sprintf_s( buffer, sizeof( buffer ), "%d", m_pOwner->GetID() );
	string strStallBtnName( m_pOwner->getName() );
	strStallBtnName = strStallBtnName + buffer + "的摊位的头顶UI";

	float fWidth = 0.0f;
	float fHeight = 0.0f;
	g_pDisplay->GetTextExtent( g_pFrameMgr->getFontInfoByFontType( "TeenyBlackFont" ), pszStallName, 
		fWidth, fHeight);
	const Ogre::Vector3& pos = GetTopTitlePos();
	Ogre::Vector3 scrpos;
	pcamera->GetCameraNode()->pointWorldToWindow( scrpos, pos );

	float fScale = g_pFrameMgr->GetScreenScaleX() < g_pFrameMgr->GetScreenScaleY() ?
		g_pFrameMgr->GetScreenScaleX() : g_pFrameMgr->GetScreenScaleY();
	int x = (int)( scrpos.x - fWidth * 0.5f - 20 * fScale );
	int y = (int)( scrpos.y + TITLE_THIRD_LINE * fHeight - 40 * fScale );

	g_pFrameMgr->updateHeadBindingFrame( strStallBtnName.c_str(), x,  y, (int)( fWidth + 30 * fScale ), 
		(int)( fHeight + 20 * fScale ) );*/

}

static float s_petTitleHeight[] = { 0.0f, -1.0f, -2.0 };
const int MAX_PET_GENIUS_TYPE		= 5;
const int MAX_STHRENGTHEN_LEVEL = 10;
void RoleText::drawPetChengHao( FollowCamera* pcamera )
{
	static const int PET_CHENG_HAO_COLORLIST[MAX_PET_GENIUS_TYPE]	= 
	{
		0xffffffff,		// 普通
		0xff00DC05,		// 良好
		0xff00DCCD,		// 优秀
		0xffCD00FF,		// 杰出
		0xffFB9400,		// 完美
	};

	GameActor*	pPetOwener	= ActorManager::getSingleton().FindActor( m_pOwner->getOwnerId() );
	GamePet*	pPetPackage	= pPetOwener->getPet();
	if ( pPetOwener == NULL )
	{
		return;
	}
	
	string strChengHao;
	if ( m_pOwner->m_nPetStrengthenLevel <= 0 || m_pOwner->m_nPetGenius < PET_GENIUS_D ||  
			m_pOwner->m_nPetGenius > PET_GENIUS_S )
	{
		return;
	}

	const tagPetStrengthenDef* pStrengthenDef = DBDataManager::getSingleton().getPetStrengthenDef( m_pOwner->m_nPetStrengthenLevel );
	if ( pStrengthenDef == NULL )
	{
		return;
	}

	int color = PET_CHENG_HAO_COLORLIST[m_pOwner->m_nPetGenius];

	drawTopTitles( pcamera->GetCameraNode(), pStrengthenDef->PetGeniusName[m_pOwner->m_nPetGenius], color, 
					TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + s_petTitleHeight[m_posInfo.m_nTitleYIndex++] );
}

void RoleText::drawPetName( FollowCamera* pcamera )
{
	int color = MONSTER_TITLE_COLORLIST[m_pOwner->getAtkType()];
	drawTopTitles( pcamera->GetCameraNode(), m_pOwner->getName(), color, 
					TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + s_petTitleHeight[m_posInfo.m_nTitleYIndex++] );
}

void RoleText::drawPetHP( FollowCamera* pcamera )
{
	float pre = (float)m_pOwner->getHP() / m_pOwner->getMaxHP();
	if ( m_pOwner->getMaxHP() <= 0 )
	{
		LOG_WARNING( "ERROR:%s's maxHp is smaller then 0!",m_pOwner->getName() );
		assert( 0 );
	}

	if ( m_pOwner->getHP() <= 0 || m_pOwner->isDead() )
	{
		pre = 0;
	}

	if ( pre > 1 )
	{
		pre = 1;
	}

	drawHp( pcamera->GetCameraNode(), pre, TITLE_FIRST_LINE );
	m_posInfo.m_fHeightInHpAndFirstText = 0.0f;
}

void RoleText::drawPetOwnerName( FollowCamera* pcamera )
{
	ActorManager& actorManager = ActorManager::getSingleton();
	GameActor* pPetOwner = actorManager.FindActor( m_pOwner->getOwnerId() );

	//跨服中名字处理
	std::string petOwnerName = pPetOwner->getName();
	petOwnerName = ActorManager::getSingleton().replaceSpecialCode(petOwnerName);
	if( pPetOwner->getServerId() != -1 )
	{
		petOwnerName = string("[") + pPetOwner->getActorWorldName() + string("]") + petOwnerName;
	}

	string strTopTitle;
	if ( pPetOwner != NULL )
	{
		strTopTitle = string( petOwnerName ) + "的";
	}

	if ( !strTopTitle.empty() )
	{
		drawTopTitles( pcamera->GetCameraNode(), strTopTitle.c_str(), TEAM_TITLE_COLOR, 
				TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + s_petTitleHeight[m_posInfo.m_nTitleYIndex++] );
	}
}

bool RoleText::canDrawPetTitle()
{
	if ( m_pOwner == NULL )
	{
		return false;
	}

	ActorManager& actorManager = ActorManager::getSingleton();
	GameActor *pHost = actorManager.FindActor( m_pOwner->getOwnerId() );
	if ( pHost != NULL && pHost->isDead())
	{
		return false;
	}

	if ( m_bDrawTitle )
	{
		return true;
	}

	MainGamePlayer* pMainPlayer = actorManager.getMainPlayer();
	if ( pMainPlayer->GetSelectTarget() == m_pOwner->GetID() ) 
	{
		return true;
	}

	if ( GetKeyState(VK_MENU) & 0xff00 )
	{
		return true;
	}

	return false;	
}

// 绘制宠物全部头顶信息
void RoleText::drawPetAllTitle( FollowCamera* pcamera )
{
	drawPetHP( pcamera );
	drawPetName( pcamera );
	drawPetChengHao( pcamera );
	drawPetOwnerName( pcamera );
}

// 绘制宠物部分头顶信息
void RoleText::drawPetPartialTitle( FollowCamera* pcamera )
{
	m_posInfo.m_fHeightInHpAndFirstText = 0.5f;
	ActorManager& actorManager = ActorManager::getSingleton();
	if ( actorManager.getGameOpt( GAME_OPT_SHOW_ALLPLAYER_AND_PET_NAME ) )
	{
		drawPetName( pcamera );
		drawPetChengHao( pcamera );
	}
}

// 绘制宠物头顶信息
void RoleText::drawPetTitle( FollowCamera* pcamera )
{
	ActorManager& actorManager	= ActorManager::getSingleton();
	GameActor* pPetOwner		= actorManager.FindActor( m_pOwner->getOwnerId() );
	if ( pPetOwner->isDead() || m_pOwner->isDead() || pPetOwner->isInFly() || m_pOwner->m_isHeti )
	{
		return;
	}
	
	MainGamePlayer* pMainPlayer = actorManager.getMainPlayer();
	if ( !actorManager.isShowOtherPlayerTitle() && pMainPlayer->GetID() != pPetOwner->GetID() /*&& 
			!pMainPlayer->isInSameTeam( pPetOwner->getTeamWID() )*/ )
	{
		return;
	}

	m_posInfo.m_nTitleYIndex = 0;
	if ( canDrawPetTitle() )
	{
		drawPetAllTitle( pcamera );
	}
	else
	{
		drawPetPartialTitle( pcamera );
	}
}

void RoleText::drawFlyTitles( FollowCamera* pcamera )
{
	unsigned int uCurTime = timeGetTime();
	updateAllPlayingText( uCurTime );

	for ( std::map<int, deque<tagCurPlayText> >::iterator dequeIter = m_curPlayingTextCollection.begin(), 
			dequeEnd = m_curPlayingTextCollection.end(); dequeIter != dequeEnd; ++dequeIter )
	{
		deque<tagCurPlayText>& oneTypePlayingText = dequeIter->second;
		for ( std::deque<tagCurPlayText>::iterator eleIter = oneTypePlayingText.begin(), 
				eleEnd = oneTypePlayingText.end(); eleIter != eleEnd; ++eleIter )
		{
			drawFlyTitles( pcamera->GetCameraNode(), *eleIter, uCurTime );
		}
	}
}

// 绘制怪物喊话的泡泡窗口
void RoleText::drawMonsterPopWin( FollowCamera* pcamera )
{
	Ogre::ShowActor* pShowActor = m_pOwner->getShowActor();
	if ( pShowActor == NULL || m_pOwner->getType() != GAT_MONSTER )
	{
		return;
	}
	Ogre::Vector3 pos = pShowActor->getCurEntity()->getWorldPosition();
	pos.y += pShowActor->getActorHeight() + 100;
	Ogre::Vector3 scrpos;
	pcamera->GetCameraNode()->pointWorldToWindow( scrpos, pos );
	char szPopName[256];
	sprintf( szPopName, "%d", m_pOwner->GetID() );
	g_pFrameMgr->updatePopWinPos( szPopName, scrpos.x + POP_WINDOW_XVALUE_ADJUST, scrpos.y );
}

// 绘制宠物喊话的泡泡窗口
void RoleText::drawPetPopWin( FollowCamera* pcamera )
{
	Ogre::ShowActor* pShowActor = m_pOwner->getShowActor();
	if ( pShowActor == NULL || m_pOwner->getType() != GAT_PET || !pShowActor->isInMap() )
	{
		return;
	}
	Ogre::Vector3 pos = pShowActor->getCurEntity()->getWorldPosition();
	pos.y += pShowActor->getActorHeight() + 100;
	Ogre::Vector3 scrpos;
	pcamera->GetCameraNode()->pointWorldToWindow( scrpos, pos );
	char szPopName[256];
	sprintf( szPopName, "%d", m_pOwner->GetID() );
	g_pFrameMgr->updatePopWinPos( szPopName, scrpos.x, scrpos.y );
}

bool RoleText::canDrawTitle()
{
	if( m_pOwner == NULL ) return false;

	if( m_pOwner->getType() == GAT_NPC )
	{
		return true;
	}

	//按下alt键 显示血条名字
	if( !(m_pOwner->getType() == GAT_MONSTER && m_pOwner->getSubType() == MON_SUB_TYPE_COLLECT) )
	{
		if( GetKeyState(VK_MENU) & 0xff00 )
		{
			return true;
		}
	}

	if( !m_bDrawTitle ) 
	{
		MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
		if( pMainPlayer != NULL )
		{
			if( pMainPlayer->GetSelectTarget() != m_pOwner->GetID() ) 
			{
				return false;
			}
		}
	}

	return true;	
}

void RoleText::drawTopTitles(Ogre::Camera *pcamera, const char *ptitle, Ogre::uint color, float h, 
							 float fOffsetY, void **icon, size_t count, bool bFiexedPos, Ogre::uint scolor )
{
	int resid = m_pOwner->getResId();
	const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resid);
	if (info.MiscType & NPCDISPLAYINFO_MISC_TYPE_NO_NAME)
	{
		return;
	}
	const Ogre::Vector3& pos = GetTopTitlePos( bFiexedPos );
	Ogre::Vector3 scrpos;
	pcamera->pointWorldToWindow(scrpos, pos);

	Ogre::UIRenderer *prenderer = Ogre::UIRenderer::getSingletonPtr();
	float width, height;
	float fScale = g_pFrameMgr->GetScreenScaleX()<g_pFrameMgr->GetScreenScaleY()?g_pFrameMgr->GetScreenScaleX():g_pFrameMgr->GetScreenScaleY();
	if( m_pOwner->getType() == GAT_NPC || m_pOwner->getType() == GAT_PLAYER || m_pOwner->getType() == GAT_MAINPLAYER )
	{
		//s_hFontTexture[NAME_FONT] = prenderer->CreateTrueTypeFont( 14, 14, strFontPath[NAME_FONT].c_str(), Ogre::CCODING_GBK );
	}else{
		//s_hFontTexture[NAME_FONT] = prenderer->CreateTrueTypeFont( 14, 14, strFontPath[NAME_FONT].c_str(), Ogre::CCODING_GBK );
	}
	prenderer->GetTextExtent(s_hFontTexture[NAME_FONT], ptitle, width, height);
	prenderer->renderText(s_hFontTexture[NAME_FONT], FONTSYTLE_BORDER, ptitle, scrpos.x - (width*0.5f+fOffsetY) * fScale,
		scrpos.y + (h*height-10) * fScale, Ogre::ColorQuad(color), fScale, false, /*scrpos.z*/0, Ogre::ColorQuad(scolor) );	

	int offset = 0;
	for (size_t i = count; i != 0; --i)
	{
		int nWidth = 0, nHeight = 0;
		prenderer->GetTextureSize(icon[i - 1], nWidth, nHeight);
		offset += nWidth;
		prenderer->BeginDraw(icon[i - 1]);
		float fAdjustX = (width * 0.5f) + offset;
		float fAdjustY = h * height - 10 - 5;
		prenderer->StretchRect( scrpos.x - fAdjustX*fScale
			, scrpos.y + fAdjustY*fScale,
			nWidth*fScale, nHeight*fScale, 0xffffffff );
		prenderer->EndDraw();
	}
}

void RoleText::drawTopTexture( Ogre::Camera *pcamera, float fOffsetY,  float fOffsetX, 
								void **icon, size_t count, bool bFiexedPos )
{
	float fScale = g_pFrameMgr->GetScreenScaleX()<g_pFrameMgr->GetScreenScaleY()?
					g_pFrameMgr->GetScreenScaleX():g_pFrameMgr->GetScreenScaleY();
	Ogre::UIRenderer *prenderer = Ogre::UIRenderer::getSingletonPtr();
	const Ogre::Vector3& pos = GetTopTitlePos( bFiexedPos );
	Ogre::Vector3 scrpos;
	pcamera->pointWorldToWindow(scrpos, pos);
	scrpos.x = floor( scrpos.x + 0.5f );
	scrpos.y = floor( scrpos.y + 0.5f );
	
	static const char* s_pszTitle = "零纪元开拓者";
	float fFontWidth = 0.0F, fFontHeight = 0.0F;
	prenderer->GetTextExtent(s_hFontTexture[NAME_FONT], s_pszTitle, fFontWidth, fFontHeight);

	float nAccumulateHeight = fOffsetY * fFontHeight + 5;
	for (size_t i = 0; i < count; ++i)
	{
		int nWidth = 0, nHeight = 0;
		prenderer->GetTextureSize(icon[i], nWidth, nHeight);
		nAccumulateHeight -= nHeight;
		prenderer->BeginDraw(icon[i]);
		if ( fScale < SCALE_CHG_LIMIT_VALUE )
		{
			prenderer->StretchRect( scrpos.x - (nWidth*0.5f+fOffsetX) * fScale, 
				scrpos.y + ( nAccumulateHeight ) * fScale,
				nWidth*fScale, 
				nHeight*fScale, 0xffffffff );
		}
		else
		{
			prenderer->DrawRect( (int)(scrpos.x - (nWidth*0.5f+fOffsetX) + 0.5f ), 
				(int)(scrpos.y + ( nAccumulateHeight ) + 0.5f ),
				nWidth, nHeight, 0xffffffff );
		}		
		prenderer->EndDraw();

		nAccumulateHeight += nHeight;
	}
}

void RoleText::drawHp( Ogre::Camera *pcamera, float fpre, float h )
{
	int resid = m_pOwner->getResId();
	const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resid);
	if (info.MiscType & NPCDISPLAYINFO_MISC_TYPE_NO_SELECT)
	{
		return;
	}
	const Ogre::Vector3& pos = GetTopTitlePos();
	Ogre::Vector3 scrpos;
	pcamera->pointWorldToWindow( scrpos, pos );
	float fScale = g_pFrameMgr->GetScreenScaleX()<g_pFrameMgr->GetScreenScaleY()?g_pFrameMgr->GetScreenScaleX():g_pFrameMgr->GetScreenScaleY();
	Ogre::UIRenderer *prenderer = Ogre::UIRenderer::getSingletonPtr();
	prenderer->BeginDraw( s_hFontTexture[HPBAR_BACKGROUP] );

	prenderer->StretchRect( scrpos.x - 40 * fScale + 6,
		scrpos.y + h * 10 * fScale,
		66 * fScale, 
		8 * fScale, 0xffffffff );
	prenderer->EndDraw();
	prenderer->BeginDraw( s_hFontTexture[HPBAR] );
	prenderer->StretchRect( scrpos.x - 39 * fScale + 6,
		scrpos.y + h * 10 * fScale, 
		(66 * fpre) * fScale,
		8 * fScale, 0xffffffff );
	prenderer->EndDraw();
}

// 绘制各角色头顶的公会信息
void RoleText::drawClanName( FollowCamera* pcamera )
{
	ActorManager& actorManager = ActorManager::getSingleton();
	// TODO: 屏蔽模型头顶信息修改点
	/*if ( !actorManager.isShowOtherPlayerTitle() )
	{
		return;
	}*/

	char strClanName[256];
	strClanName[0] = '\0';
	float clanNamPos = 0.0f ;
	strcat(strClanName, ActorManager::getSingleton().getClanFengHao(m_pOwner->getClanWID()));
	if ( !m_pOwner->m_strClanName.empty() )
	{
		switch( m_pOwner->m_nClanPosition )
		{
		case CLAN_POS_NOT_MEMBER:
			{
				strcat( strClanName, m_pOwner->m_strClanName.c_str() );
			}
			break;
		case CLAN_POS_NORMAL:
			{
				strcat( strClanName, m_pOwner->m_strClanName.c_str() );
				if ( m_pOwner->isInClanGroup() )
				{
					const char *szGroupName[] = { " 弑杀战团"," 暗杀战团"," 狂杀战团"," 灭杀战团"," 绝杀战团"," 黄金战团" };
					strcat( strClanName, szGroupName[m_pOwner->getClanGroupID()-1] );
				}
			}
			break;
		case CLAN_POS_TECH_OFFICER:
			{
				strcat( strClanName, m_pOwner->m_strClanName.c_str() );
				strcat( strClanName, " 科技官员" );
			}
			break;
		case CLAN_POS_COLLECTION_OFFICER:
			{
				strcat( strClanName, m_pOwner->m_strClanName.c_str() );
				strcat( strClanName, " 采集官员" );
			}
			break;
		case CLAN_POS_FACTORY_OFFICER:
			{
				strcat( strClanName, m_pOwner->m_strClanName.c_str() );
				strcat( strClanName, " 工厂官员" );
			}
			break;
		case CLAN_POS_PET_OFFICER:
			{
				strcat( strClanName, m_pOwner->m_strClanName.c_str() );
				strcat( strClanName, " 宠物官员" );
			}
			break;
		case CLAN_POS_WALL_OFFICER:
			{
				strcat( strClanName, m_pOwner->m_strClanName.c_str() );
				strcat( strClanName, " 郊区官员" );
			}
			break;		
		case CLAN_POS_VICE_PRESIDENT:
			{
				strcat( strClanName, m_pOwner->m_strClanName.c_str() );
				strcat( strClanName, " 副会长" );
				if ( m_pOwner->isInClanGroup() )
				{
					const char *szGroupName[] = { "(弑杀)","(暗杀)","(狂杀)","(灭杀)","(绝杀)","(黄金)" };
					strcat( strClanName, szGroupName[m_pOwner->getClanGroupID()-1] );
				}
			}
			break;
		case CLAN_POS_PRESIDENT:
			{
				strcat( strClanName, m_pOwner->m_strClanName.c_str() );
				strcat( strClanName, " 会长" );
				if ( m_pOwner->isInClanGroup() )
				{
					const char *szGroupName[] = { "(弑杀)","(暗杀)","(狂杀)","(灭杀)","(绝杀)","(黄金)" };
					strcat( strClanName, szGroupName[m_pOwner->getClanGroupID()-1] );
				}
			}
			break;
		case CLAN_POS_GROUP_1:
			{
				strcat( strClanName, m_pOwner->m_strClanName.c_str() );
				strcat( strClanName, " 弑杀战团长" );
			}
			break;
		case CLAN_POS_GROUP_2:
			{
				strcat( strClanName, m_pOwner->m_strClanName.c_str() );
				strcat( strClanName, " 暗杀战团长" );
			}
			break;
		case CLAN_POS_GROUP_3:
			{
				strcat( strClanName, m_pOwner->m_strClanName.c_str() );
				strcat( strClanName, " 狂杀战团长" );
			}
			break;
		case CLAN_POS_GROUP_4:
			{
				strcat( strClanName, m_pOwner->m_strClanName.c_str() );
				strcat( strClanName, " 灭杀战团长" );
			}
			break;
		case CLAN_POS_GROUP_5:
			{
				strcat( strClanName, m_pOwner->m_strClanName.c_str() );
				strcat( strClanName, " 绝杀战团长" );
			}
			break;
		case CLAN_POS_GROUP_6:
			{
				strcat( strClanName, m_pOwner->m_strClanName.c_str() );
				strcat( strClanName, " 黄金战团长" );
			}
			break;
		}
		clanNamPos = TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + 
			s_playerTitleHeight[m_posInfo.m_nTitleYIndex++];
		drawTopTitles( pcamera->GetCameraNode(), strClanName, m_pOwner->getClanColor(), 
						clanNamPos);
	}
	if( m_pOwner->getDaoBaClanWid() != 0 &&  !m_pOwner->isInStall() )
	{
		if ( clanNamPos == 0.0f )
		{
			clanNamPos = TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + 
				s_playerTitleHeight[m_posInfo.m_nTitleYIndex++];
		}
		vector<void *> icons;
		icons.push_back(s_hFontTexture[STRONG_GLORY]);
		float m_offsety = 0.6f;
		float m_offsetx = 0.0f;
		float width, height;
		Ogre::UIRenderer::getSingletonPtr()->GetTextExtent(s_hFontTexture[NAME_FONT], strClanName, width, height);
		int iconWidth,iconHeight;
		Ogre::UIRenderer::getSingletonPtr()->GetTextureSize( s_hFontTexture[STRONG_GLORY], iconWidth, iconHeight );
		if ( width == 0 )
		{
			m_offsetx += 0.0;
		}
		else
		{
			m_offsetx += -width/2 - iconWidth/2 ;
		}
		drawTopTexture( pcamera->GetCameraNode(), clanNamPos+m_offsety, m_offsetx, &icons[0], icons.size() );
	}
}

void RoleText::drawPopWindow( Ogre::Vector3 scrpos )
{
	if( m_pOwner->getCurMap() == NULL )
	{
		return;
	}

	int nMapID = m_pOwner->getCurMap()->getMapId();

	if ( ( nMapID / GAME_PWORLD_MAPID >= 1 ) && ( nMapID / GAME_PWORLD_MAPID < 10 ) )
	{
		if ( !std::string(m_pOwner->getTeamName() ).empty() && canDrawTitle() )
		{
			if ( !m_pOwner->m_strClanName.empty() )
			{	
				g_pFrameMgr->updatePopWinPos( m_pOwner->getName(), scrpos.x + POP_WINDOW_XVALUE_ADJUST, 
												scrpos.y - POP_WINDOW_FORTH_LINE );
			}
			else
			{
				g_pFrameMgr->updatePopWinPos( m_pOwner->getName(), scrpos.x + POP_WINDOW_XVALUE_ADJUST, 
												scrpos.y - POP_WINDOW_THIRD_LINE );
			}
		}
		else
		{
			if ( !m_pOwner->m_strClanName.empty() && canDrawTitle() )
			{
				g_pFrameMgr->updatePopWinPos( m_pOwner->getName(), scrpos.x + POP_WINDOW_XVALUE_ADJUST, 
												scrpos.y - POP_WINDOW_THIRD_LINE );
			}
			else
			{
				g_pFrameMgr->updatePopWinPos( m_pOwner->getName(), scrpos.x + POP_WINDOW_XVALUE_ADJUST, scrpos.y );
			}
		}
	}
	else
	{
		if ( !std::string(m_pOwner->getTeamName() ).empty() )
		{
			if ( !m_pOwner->m_strClanName.empty() && canDrawTitle() )
			{
				g_pFrameMgr->updatePopWinPos( m_pOwner->getName(), scrpos.x + POP_WINDOW_XVALUE_ADJUST, scrpos.y - POP_WINDOW_FORTH_LINE );
			}else{
				g_pFrameMgr->updatePopWinPos( m_pOwner->getName(), scrpos.x + POP_WINDOW_XVALUE_ADJUST, scrpos.y - POP_WINDOW_THIRD_LINE );
			}
		}
		else
		{
			if ( !m_pOwner->m_strClanName.empty() && canDrawTitle() )
			{
				g_pFrameMgr->updatePopWinPos( m_pOwner->getName(), scrpos.x + POP_WINDOW_XVALUE_ADJUST, scrpos.y - POP_WINDOW_THIRD_LINE );
			}else{
				g_pFrameMgr->updatePopWinPos( m_pOwner->getName(), scrpos.x + POP_WINDOW_XVALUE_ADJUST, scrpos.y );
			}
		}
	}

}

void RoleText::drawFlyTitles( Ogre::Camera *pcamera, tagCurPlayText& playingTextInfo, unsigned int uCurTime )
{
	if( playingTextInfo.m_pFontType->m_nFlyType == FONT_FLY_LINE )
	{
		flyLine( pcamera, playingTextInfo, uCurTime );
	}else if( playingTextInfo.m_pFontType->m_nFlyType == FONT_FLY_DISPER )
	{
		flyDispersedly( pcamera, playingTextInfo, uCurTime );
	}
}

// 直线飞行
void RoleText::flyLine( Ogre::Camera *pcamera, tagCurPlayText& playingTextInfo, unsigned int uCurTime )
{
	// 其次再进行绘制操作
	Ogre::ShowActor*		pShowActor		= m_pOwner->getShowActor();
	// 获取脚底世界坐标
	const Ogre::Vector3&	footPos			= pShowActor->getCurEntity()->getWorldPosition();
	Ogre::UIRenderer*		prenderer		= Ogre::UIRenderer::getSingletonPtr();
	float width, height;
	
	unsigned int textStartTime = playingTextInfo.m_uStartTime;
	// 若已经超过有效显示时间，则略过显示这个冒血伤害数字

	unsigned int uAlpha		= 255;
	unsigned int uDeltaTime = uCurTime - textStartTime;

	if ( textStartTime <= uCurTime )
	{
		tagFont* pFontType		= playingTextInfo.m_pFontType;
		tagFlyText& flyTextInfo	= playingTextInfo.m_curTextInfo;
		int nType = flyTextInfo.m_nFontType;
		int passTime = uCurTime - textStartTime;

		flyTextInfo.m_nCurOffY = (int)( pFontType->m_fInitOffY );
		flyTextInfo.m_nCurOffX = (int)( pFontType->m_fInitOffX );

		// 字体都是要先进行放大，然后才会开始缩小
		// 若字体处于放大阶段
		if ( passTime <= pFontType->m_nScalTime )
		{
			flyTextInfo.m_fCurScal = 1.0f + passTime * pFontType->m_fScal / 
								pFontType->m_nScalTime;
		}
		else
		{
			// 字体在放大后会立刻变为缩小状态，
			// 若字体现在处于缩小阶段
			if ( passTime <= pFontType->m_nScalTime + pFontType->m_nScalOverTime )
			{
				flyTextInfo.m_fCurScal = pFontType->m_fScal + 1.0f - 
									(passTime - pFontType->m_nScalTime) * 
									pFontType->m_fScal / pFontType->m_nScalOverTime;

			}
			else
			{
				flyTextInfo.m_fCurScal = 1.0f;
				unsigned int nTotalTime		= pFontType->m_nFontLife - pFontType->m_nScalTime - pFontType->m_nScalOverTime;
				unsigned int nChgAlphaTime	= passTime - pFontType->m_nScalTime - pFontType->m_nScalOverTime;
				uAlpha -= (unsigned int)( ( static_cast<float>(  nChgAlphaTime ) / nTotalTime ) * 255 );
			}

			if( flyTextInfo.m_fCurScal < 1.0f ) 
			{
				flyTextInfo.m_fCurScal = 1.0f;
			}
		}

		Ogre::Vector3 scrpos;
		Ogre::Vector3 drawPos( footPos.x, footPos.y + pFontType->m_nReltiveOffset, footPos.z );

		if ( pFontType->m_nReltivePoint == RELATIVE_TO_HEAD )
		{
			drawPos.y += pShowActor->getActorHeight();
		}
		
		pcamera->pointWorldToWindow( scrpos, drawPos );
		float xOffset = scrpos.x + pFontType->m_fOffX * passTime / 100 + pFontType->m_fInitOffX;
		float yOffset = scrpos.y + pFontType->m_fOffY * passTime / 100 /*+ playingTextInfo.m_nIndex * pFontType->m_fDeltaDistY*/ + pFontType->m_fInitOffY;
		int nFontType = flyTextInfo.m_nFontType;
		Ogre::ColorQuad shadowColor(0,0,0,uAlpha );
		prenderer->GetTextExtent( s_hFontTexture[nFontType], flyTextInfo.m_Text.c_str(), width, height );
		prenderer->renderText( s_hFontTexture[nFontType], FONTSYTLE_NORMAL, flyTextInfo.m_Text.c_str(), 
			xOffset - width * 0.5f, yOffset, Ogre::ColorQuad( flyTextInfo.m_Red, flyTextInfo.m_Green, flyTextInfo.m_Blue, uAlpha ), 
			flyTextInfo.m_fCurScal, false, 0, Ogre::ColorQuad(shadowColor) );
	}
}

// 散弹式飞行
void RoleText::flyDispersedly( Ogre::Camera *pcamera, tagCurPlayText& playingTextInfo, unsigned int uCurTime )
{

}

bool RoleText::isOutOfTimePlayingText( const tagCurPlayText& playingTextInfo, unsigned int uCurTime )
{
	if ( ( playingTextInfo.m_uStartTime + playingTextInfo.m_pFontType->m_nFontLife ) <= uCurTime )
	{
		return true;
	}

	return false;
}

void RoleText::clearAllOutOfTimePlayingText( unsigned int uCurTime )
{
	for ( std::map<int, deque<tagCurPlayText> >::iterator dequeIter = m_curPlayingTextCollection.begin(); 
			dequeIter != m_curPlayingTextCollection.end(); ++dequeIter )
	{
		deque<tagCurPlayText>& oneTypePlayingText = dequeIter->second;
		for ( std::deque<tagCurPlayText>::iterator eleIter = oneTypePlayingText.begin(); 
				eleIter !=  oneTypePlayingText.end(); )
		{
			if ( isOutOfTimePlayingText( *eleIter, uCurTime ) )
			{
				/*char buffer[256];
				sprintf( buffer, "删除的伤害显示数据是%s", eleIter->m_curTextInfo.m_Text.c_str() );
				LOG_PRIVATE( buffer );*/
				eleIter = oneTypePlayingText.erase( eleIter );
			}
			else
			{
				++eleIter;
			}
		}
	}
}

void RoleText::updateAllPlayingText( unsigned int uCurTime )
{
	clearAllOutOfTimePlayingText( uCurTime );
	memset( m_szCurHaveUpdated, 0, sizeof( m_szCurHaveUpdated) );
	
	for ( std::vector<tagFlyFont>::iterator iter = m_vecFont.begin(); iter != m_vecFont.end(); )
	{
		if ( iter->m_vectorText.empty() )
		{
			iter = m_vecFont.erase( iter );
			continue;
		}

		updatePlayingTextCollection( *iter, uCurTime );
		++iter;
	}
}

void RoleText::updatePlayingTextCollection( tagFlyFont& t, unsigned int uStartTime )
{
	unsigned int curTime = uStartTime;
	//int nCount = 0;
	
	for ( std::list<tagFlyText>::iterator iter = t.m_vectorText.begin(); iter != t.m_vectorText.end(); )
	{
		unsigned int textStartTime = t.m_uStartTime;
		
		// 若已经超过有效显示时间，则略过显示这个冒血伤害数字
		if ( ( textStartTime + t.m_pFontType->m_nFontLife ) <= curTime )
		{
			iter = t.m_vectorText.erase( iter );
			continue;
		}

		if ( textStartTime <= curTime )
		{
			/*if ( m_szCurHaveUpdated[iter->m_nFontType] == 1 )
			{
				t.m_uStartTime += MIN_DELAY_TIME;
			}*/

			if ( canUpdatePlayingTextCollection( iter->m_nFontType, t.m_pFontType->m_nFontFlyId, curTime ) )
			{
				/*char buffer[256];
				sprintf( buffer, "先加入的伤害显示数据是%s", iter->m_Text.c_str() );
				LOG_PRIVATE( buffer );*/
				addPlayingText( iter->m_nFontType, textStartTime, *iter, t.m_pFontType );
				iter = t.m_vectorText.erase( iter );
				continue;
			}
		}

		++iter;
	}
}

void RoleText::addPlayingText( int nType, unsigned uStartTime, const tagFlyText& flyTextInfo, 
								 tagFont* pFontType )
{
	deque<tagCurPlayText>& oneTypeTextCollection = m_curPlayingTextCollection[pFontType->m_nFontFlyId];
	/*assert( oneTypeTextCollection.size() <= MAX_CHANNELS_SAME_TIME );
	if ( oneTypeTextCollection.size() == MAX_CHANNELS_SAME_TIME )
	{
		oneTypeTextCollection.pop_front();
	}*/
	
	// TODO
	if ( !oneTypeTextCollection.empty() )
	{
		tagCurPlayText& lastTexElem = oneTypeTextCollection.back();
		uStartTime = lastTexElem.m_uStartTime + pFontType->m_nSpeed; 
	}

	tagCurPlayText addCurPlayTextInfo;
	addCurPlayTextInfo.m_uStartTime		= uStartTime;
	addCurPlayTextInfo.m_pFontType		= pFontType;
	addCurPlayTextInfo.m_curTextInfo	= flyTextInfo;
	addCurPlayTextInfo.m_nIndex			= (int)oneTypeTextCollection.size();
	oneTypeTextCollection.push_back( addCurPlayTextInfo );

	m_szCurHaveUpdated[nType]			= 1;
}

void RoleText::SetLoadMap( bool isLoading )
{
	s_mIsLoadingMap = isLoading;
}

bool RoleText::isLoadingMap()
{
	return s_mIsLoadingMap;
}

bool RoleText::canUpdatePlayingTextCollection( int nType, int nID, unsigned int uCurTime )
{
	// 若在当前帧已经更新过一次，则不能再更新一次
	if ( m_szCurHaveUpdated[nType] == 1 )
	{
		return false;
	}
	
	const deque<tagCurPlayText>& oneTypeTextCollection = m_curPlayingTextCollection[nID];
	if ( oneTypeTextCollection.empty() )
	{
		return true;
	}

	for ( deque<tagCurPlayText>::const_iterator iter = oneTypeTextCollection.begin(), end = oneTypeTextCollection.end();
			iter != end; ++iter )
	{
		int passTime = uCurTime - iter->m_uStartTime;
		if ( passTime > iter->m_pFontType->m_nScalTime )
		{
			return true;
		}
	}

	return false;
}

static bool sortTitleDrawer(IDrawTitleIcon *a, IDrawTitleIcon* b)
{
	return a->GetPriority() < b->GetPriority();
}

void RoleText::registTitleDrawer(IDrawTitleIcon *drawer)
{
	m_lstTitleDrawer.push_back(drawer);
	std::push_heap( m_lstTitleDrawer.begin(), m_lstTitleDrawer.end(), sortTitleDrawer );
}

void RoleText::drawMonsterIcon(Ogre::Camera *pCamera, void *i, int offest)
{
	Ogre::UIRenderer *prenderer = Ogre::UIRenderer::getSingletonPtr();
	int width = 0, height = 0;
	prenderer->GetTextureSize( i, width, height );
	const Ogre::Vector3& pos = GetTopTitlePos();
	Ogre::Vector3 scrpos;
	pCamera->pointWorldToWindow( scrpos, pos );

	float fScale = g_pFrameMgr->GetScreenScaleX()<g_pFrameMgr->GetScreenScaleY()?g_pFrameMgr->GetScreenScaleX():g_pFrameMgr->GetScreenScaleY();
	prenderer->BeginDraw( i );

	prenderer->StretchRect( scrpos.x - (width * 0.5f)*fScale, scrpos.y + ( offest - height )* fScale,
								width*fScale, height*fScale, 0xffffffff );

	//prenderer->DrawRect( (int)(scrpos.x - int(width / 2)), (int)(scrpos.y - height - offest), width, height, 0xffffffff );
	prenderer->EndDraw();
}

void RoleText::drawMonsterOwnerClanName( FollowCamera* pCamera )
{
	ActorManager& actorManager = ActorManager::getSingleton();
	if( m_pOwner->getSubType() != MON_SUB_TYPE_MACHINE )
	{
		return;
	}
	if( m_pOwner->getMonsterMachine()->getPassengerNum() == 0 )
	{
		return;
	}
	GameActor* pActor = ActorManager::getSingleton().FindActor( m_pOwner->getMonsterMachine()->getPassenger(0) );
	if( pActor == NULL )
	{
		return;
	}
	char strClanName[256];
	strClanName[0] = '\0';
	//if (WorldCityManager::getSingleton().IsWorldCityKeeper(pActor))
	//{
	//	strcat(strClanName, "[时间城] ");
	//}
	strcat(strClanName, ActorManager::getSingleton().getClanFengHao(pActor->getClanWID()));
	if ( !pActor->m_strClanName.empty() )
	{
		switch( pActor->m_nClanPosition )
		{
		case CLAN_POS_NOT_MEMBER:
			{
				strcat( strClanName, pActor->m_strClanName.c_str() );
			}
			break;
		case CLAN_POS_NORMAL:
			{
				strcat( strClanName, pActor->m_strClanName.c_str() );
			}
			break;
		case CLAN_POS_TECH_OFFICER:
			{
				strcat( strClanName, pActor->m_strClanName.c_str() );
				strcat( strClanName, " 科技官员" );
			}
			break;
		case CLAN_POS_COLLECTION_OFFICER:
			{
				strcat( strClanName, pActor->m_strClanName.c_str() );
				strcat( strClanName, " 采集官员" );
			}
			break;
		case CLAN_POS_FACTORY_OFFICER:
			{
				strcat( strClanName, pActor->m_strClanName.c_str() );
				strcat( strClanName, " 工厂官员" );
			}
			break;
		case CLAN_POS_PET_OFFICER:
			{
				strcat( strClanName, pActor->m_strClanName.c_str() );
				strcat( strClanName, " 宠物官员" );
			}
			break;
		case CLAN_POS_WALL_OFFICER:
			{
				strcat( strClanName, pActor->m_strClanName.c_str() );
				strcat( strClanName, " 郊区官员" );
			}
			break;		
		case CLAN_POS_VICE_PRESIDENT:
			{
				strcat( strClanName, pActor->m_strClanName.c_str() );
				strcat( strClanName, " 副会长" );
			}
			break;
		case CLAN_POS_PRESIDENT:
			{
				strcat( strClanName, pActor->m_strClanName.c_str() );
				strcat( strClanName, " 会长" );
			}
			break;
		}
		
		drawTopTitles( pCamera->GetCameraNode(), strClanName, pActor->getClanColor(), 
						TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + 
						s_playerTitleHeight[m_posInfo.m_nTitleYIndex++] );
	}
}

void RoleText::drawPlayerLoveSignTitle( FollowCamera* pcamera )
{
	if( m_pOwner->isShowLove() )
	{
		// 绘制情侣或者夫妻关系
		MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
		if( m_pOwner->getType() == GAT_MAINPLAYER && pMain->hasLover() )
		{
			char strLoveSign[256];
			strcpy( strLoveSign, "☆" );
			strcat( strLoveSign, pMain->getLoverName() );
			if ( pMain->getLoveColorType() == INTIMATE_COLOR_LOVERS )
			{
				strcat( strLoveSign, "的情侣☆" );
			}else
			{
				strcat( strLoveSign, "的伴侣☆" );
			}
			drawTopTitles( pcamera->GetCameraNode(), strLoveSign, LOVE_SIGN_COLOR, 
				TITLE_FIRST_LINE + m_posInfo.m_fHeightInHpAndFirstText + 
				s_playerTitleHeight[m_posInfo.m_nTitleYIndex++] );
		}
	}
}