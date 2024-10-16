#include "RoleText.h"
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
#include <Windows.h>

using namespace Ogre;
using namespace std;

static Ogre::HUIRES s_hFontTexture[FONT_END];

const std::string strFontPath[] = {
	"uires\\fonts\\secondary.ttf",
	"uires\\数字相关资源\\人物伤害.xml",
	"uires\\数字相关资源\\人物暴击.xml",
	"uires\\数字相关资源\\加血.xml",
	"uires\\数字相关资源\\加蓝.xml",
	"uires\\数字相关资源\\中文字体.xml",

	"uires\\通用类\\血条.tga",
	"uires\\通用类\\血条框.tga",
}; 

const std::string strFontConfigPath = "db/skill/font.xml";
const int NOMAL_TITLE_COLOR			= 0xffffffff;
const int PLAYER_TITLE_COLOR		= 0xff00ffff;
const int MAINPLAER_TITLE_COLOR		= 0xffc48f68;
const int NPCFUNDE_TILTLE_COLOR		= 0Xffff00ff;

const int MONSTER_TITLE_COLORLIST[]	= {
	0xffc6c11a,		// 不攻击
	0xffff0000,		// 主动攻击
	0xffc6c11a,		// 被动攻击
};/*  Ver.10 怪物攻击类型 Bind Macrosgroup:MON_ATK_TYPE,*/

std::map<int, tagFont>	RoleText::m_mapFontType;	// 字体类型表

RoleText::RoleText(void): m_bDrawTitle( false ), m_pOwner( NULL ) 
{
}

RoleText::~RoleText(void)
{
}

void RoleText::loadFontConfig()
{
	Ogre::XMLData xmldata;
	if( !xmldata.loadFile( strFontConfigPath ) )
	{
		LOG_SEVERE("load font config error: %s", strFontConfigPath.c_str());
		return;
	}

	XMLNode	root = xmldata.getRootNode();
	XMLNode	node = root.iterateChild();
	if( stricmp( node.getName(), "fonts" ) == 0 )
	{
		XMLNode child = node.iterateChild();
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

			m_mapFontType[font.m_nFontFlyId] = font;
			child = node.iterateChild(child);
		}
		node = root.iterateChild(node);
	}
}

void RoleText::initFont()
{
	Ogre::UIRenderer *prenderer = Ogre::UIRenderer::getSingletonPtr();
	s_hFontTexture[NAME_FONT]		= prenderer->CreateTrueTypeFont( 15, 15, strFontPath[NAME_FONT].c_str(), Ogre::CCODING_GBK );
	s_hFontTexture[NORMAL_HIT_FONT] = prenderer->CreateBitmapFont( strFontPath[NORMAL_HIT_FONT].c_str(), Ogre::CCODING_GBK );
	s_hFontTexture[HEAVY_HIT_FONT]  = prenderer->CreateBitmapFont( strFontPath[HEAVY_HIT_FONT].c_str(), Ogre::CCODING_GBK );
	s_hFontTexture[HP_ADD_FONT]		= prenderer->CreateBitmapFont( strFontPath[HP_ADD_FONT].c_str(), Ogre::CCODING_GBK );
	s_hFontTexture[MP_ADD_FONT]		= prenderer->CreateBitmapFont( strFontPath[MP_ADD_FONT].c_str(), Ogre::CCODING_GBK );
	s_hFontTexture[CHINESE_FONT]	= prenderer->CreateBitmapFont( strFontPath[CHINESE_FONT].c_str(), Ogre::CCODING_GBK );
	s_hFontTexture[HPBAR]			= prenderer->CreateTexture( strFontPath[HPBAR].c_str(), Ogre::BLEND_ALPHABLEND );
	s_hFontTexture[HPBAR_BACKGROUP]	= prenderer->CreateTexture( strFontPath[HPBAR_BACKGROUP].c_str(), Ogre::BLEND_ALPHABLEND );

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
	prenderer->ReleaseUIRes(s_hFontTexture[CHINESE_FONT]);
	*/
	prenderer->ReleaseUIRes(s_hFontTexture[HPBAR]);
	prenderer->ReleaseUIRes(s_hFontTexture[HPBAR_BACKGROUP]);
}

bool RoleText::hasFontType( int id )
{
	std::map<int, tagFont>::iterator iter = m_mapFontType.find( id );
	if( iter != m_mapFontType.end() )
	{
		return true;
	}
	return false; 
}

tagFont* RoleText::getFontType( int id )
{
	std::map<int, tagFont>::iterator iter = m_mapFontType.find( id );
	if( iter != m_mapFontType.end() )
	{
		return &m_mapFontType[id];
	}
	return NULL; 
}

void RoleText::setOwner( GameActor* pActor )
{
	m_pOwner = pActor;
}

void RoleText::setEnableDrawTitle( bool flag )
{
	m_bDrawTitle = flag;
}

void RoleText::AddFlyFont( tagFlyFont& t )
{
	m_vecFFont.push_back( t );
}

void RoleText::drawTitles( FollowCamera *pcamera )
{	
	if( !canDrawTitle() ) 
	{
		return;
	}
	Ogre::ShowActor* pShowActor = m_pOwner->getShowActor();

	char buffer[256];
	sprintf( buffer, 
			 "%s  %s",
			 m_pOwner->getTeamName(), 
			 m_pOwner->getName() );
	// 采集
	if( m_pOwner->getSubType() != 0 && m_pOwner->getSubType() == MON_SUB_TYPE_COLLECT )
	{
		double fhealth	= (float)m_pOwner->getHealth()/100;
		int	   nHp		= fhealth*m_pOwner->getMaxHP();
		sprintf( buffer, 
				 "%s  %s(%d)(%d/%d)", 
				 m_pOwner->getTeamName(), 
				 m_pOwner->getName(), 
				 m_pOwner->GetID(), 
				 nHp, 
				 m_pOwner->getMaxHP() );
	}
	int color = NOMAL_TITLE_COLOR;
	switch( m_pOwner->getType() )
	{
	case GAT_MAINPLAYER:
		{
			color = MAINPLAER_TITLE_COLOR;
		}
		break;
	case GAT_MONSTER:
		{
			assert( m_pOwner->getAtkType() <= 2 );
			color = MONSTER_TITLE_COLORLIST[m_pOwner->getAtkType()];
		}
		break;
	case GAT_NPC:
		{
			color = MONSTER_TITLE_COLORLIST[ATK_TYPE_PASSIVE];
		}
		break;
	case GAT_PLAYER:
		{
			color = PLAYER_TITLE_COLOR;
		}
		break;
	};
	drawTopTitles( pcamera->GetCameraNode(), buffer, color, -1.2 );
	// NPC需要显示功能
	if( m_pOwner->getType() == GAT_NPC )
	{
		drawTopTitles( pcamera->GetCameraNode(), ((GameNPC*)m_pOwner)->getNpcFunDes(), NPCFUNDE_TILTLE_COLOR, -2.2 );
	}

	if( m_pOwner->getType() == GAT_MONSTER || m_pOwner->getType() == GAT_PLAYER || m_pOwner->getType() == GAT_PET )
	{
		float pre = (float)m_pOwner->getHealth()/100;
		drawHp( pcamera->GetCameraNode(), pre, -1.2 );
	}
}

void RoleText::drawFlyTitles( FollowCamera* pcamera )
{
	for( std::vector<tagFlyFont>::iterator iter = m_vecFFont.begin(); iter != m_vecFFont.end(); )
	{
		if( iter->m_vectorText.empty() )
		{
			iter = m_vecFFont.erase( iter );
			continue;
		}
		drawFlyTitles( pcamera->GetCameraNode(), *iter );
		++iter;
	}
}

bool RoleText::canDrawTitle()
{
	if( m_pOwner == NULL ) return false;
	
	if( m_pOwner->getType() == GAT_NPC )
	{
		return true;
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

void RoleText::drawTopTitles(Ogre::Camera *pcamera, const char *ptitle, Ogre::uint color, int h)
{
	Ogre::ShowActor* pShowActor = m_pOwner->getShowActor();

	Ogre::Vector3 pos = pShowActor->getCurEntity()->getWorldPosition();
	pos.y += pShowActor->getActorHeight();

	Ogre::Vector3 scrpos;
	pcamera->pointWorldToWindow(scrpos, pos);

	Ogre::UIRenderer *prenderer = Ogre::UIRenderer::getSingletonPtr();
	float width, height;
	prenderer->GetTextExtent(s_hFontTexture[NAME_FONT], ptitle, width, height);
	prenderer->renderText(s_hFontTexture[NAME_FONT], FONTSYTLE_BORDER, ptitle, scrpos.x-width*0.5f, 
		scrpos.y+h*height-10, Ogre::ColorQuad(color), 1.0f, false, /*scrpos.z*/0 );
}

void RoleText::drawHp( Ogre::Camera *pcamera, float fpre, int h )
{
	Ogre::ShowActor* pShowActor = m_pOwner->getShowActor();

	Ogre::Vector3 pos = pShowActor->getCurEntity()->getWorldPosition();
	pos.y += pShowActor->getActorHeight();

	Ogre::Vector3 scrpos;
	pcamera->pointWorldToWindow(scrpos, pos);

	Ogre::UIRenderer *prenderer = Ogre::UIRenderer::getSingletonPtr();
	prenderer->BeginDraw(s_hFontTexture[HPBAR_BACKGROUP]);
	prenderer->DrawRect( scrpos.x - 40, scrpos.y + h*10-2, 90, 9, 0xffffffff );
	prenderer->EndDraw();
	prenderer->BeginDraw(s_hFontTexture[HPBAR]);
	prenderer->DrawRect( scrpos.x - 38, scrpos.y + h*10, 86*fpre, 5, 0xffffffff );
	prenderer->EndDraw();
}

void RoleText::drawFlyTitles( Ogre::Camera *pcamera, tagFlyFont& t )
{
	if( t.m_pFontType->m_nFlyType == FONT_FLY_LINE )
	{
		flyLine( pcamera, t );
	}else if( t.m_pFontType->m_nFlyType == FONT_FLY_DISPER )
	{
		flyDispersedly( pcamera, t );
	}
}

// 直线飞行
void RoleText::flyLine( Ogre::Camera *pcamera, tagFlyFont& t )
{
	Ogre::ShowActor* pShowActor = m_pOwner->getShowActor();
	Ogre::Vector3 pos = pShowActor->getCurEntity()->getWorldPosition();
	pos.y += ( pShowActor->getActorHeight() + 100 );

	Ogre::Vector3 scrpos;
	pcamera->pointWorldToWindow(scrpos, pos);

	Ogre::UIRenderer *prenderer = Ogre::UIRenderer::getSingletonPtr();
	float width, height;

	bool bDel				= true; 
	int nCount				= 0;
	unsigned int curTime	= timeGetTime();
	for( std::list<tagFlyText>::iterator iter = t.m_vectorText.begin(); iter != t.m_vectorText.end(); ++iter )
	{
		int textStartTime = t.m_uStartTime + nCount * t.m_pFontType->m_nSpeed;
		if( ( textStartTime + t.m_pFontType->m_nFontLife ) <= curTime )
		{
			nCount++;
			continue;
		}

		if( textStartTime <= curTime )
		{
			int passTime = curTime - textStartTime;
			iter->m_nCurOffY = t.m_pFontType->m_fOffY * passTime/100;
			if( t.m_pFontType->m_fOffX != 0 )
			{
				iter->m_nCurOffX = t.m_pFontType->m_fOffX * passTime/100;
			}
			if( passTime <= t.m_pFontType->m_nScalTime )
			{
				iter->m_fCurScal = 1.0f + passTime * t.m_pFontType->m_fScal/t.m_pFontType->m_nScalTime;
			}else{
				if( passTime <= t.m_pFontType->m_nScalTime + t.m_pFontType->m_nScalOverTime )
				{
					iter->m_fCurScal = t.m_pFontType->m_fScal + 1.0f - (passTime-t.m_pFontType->m_nScalTime) * t.m_pFontType->m_fScal/t.m_pFontType->m_nScalOverTime;
				}else{
					iter->m_fCurScal = 1.0f;
				}
				if( iter->m_fCurScal < 1.0f ) iter->m_fCurScal = 1.0f;
			}

			int nFontType = iter->m_nFontType;
			prenderer->GetTextExtent( s_hFontTexture[nFontType], iter->m_Text.c_str(), width, height );
			prenderer->renderText( s_hFontTexture[nFontType], FONTSYTLE_NORMAL, iter->m_Text.c_str(), 
				scrpos.x + iter->m_nCurOffX - width*0.5f, scrpos.y + iter->m_nCurOffY, Ogre::ColorQuad(0xffffffff), iter->m_fCurScal );
		}
		nCount++;
		bDel = false;
	}
	if( bDel ) t.m_vectorText.clear();
}

// 散弹式飞行
void RoleText::flyDispersedly( Ogre::Camera *pcamera, tagFlyFont& t )
{

}
