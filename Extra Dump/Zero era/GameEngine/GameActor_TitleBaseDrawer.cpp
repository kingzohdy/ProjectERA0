#include "GameActor_TitleBaseDrawer.h"
#include "OgreCamera.h"
#include "OgreUIRenderer.h"
#include "GameActor_PVP.h"
#include "../UILib/ui_framemgr.h"
#include "ShowActor.h"
#include "OgreEntity.h"

extern Ogre::HUIRES GetTitleFontRes( int nFontType );

tagAdjustPosInfo IActorTitleDrawer::m_posInfo;
Ogre::Vector3 IActorTitleDrawer::s_curScreenPos;
IActorTitleDrawer::IActorTitleDrawer( GameActor* pOwner ) : m_pOwner( pOwner ), m_isForceDrawTitle( false ),
															m_bIsLastMenuDown( false ), m_Name(pOwner->getName())
{
}

IActorTitleDrawer::~IActorTitleDrawer()
{}

void IActorTitleDrawer::Update( float dtick )
{}

void IActorTitleDrawer::UpdateTextColor()
{
}

void IActorTitleDrawer::DrawAllIconTitle( Ogre::Camera *pCamera )
{
}

void IActorTitleDrawer::DrawOneTopTextTitles( Ogre::Camera *pCamera, const tagDrawTextTitleInfo& oneDrawTextInfo,
												Ogre::uint shadowColor )
{
	const std::string& strText = oneDrawTextInfo.m_strTitle;
	if ( strText.empty() )
	{
		return;
	}

	DrawOneTopTextTitles( pCamera, strText.c_str(), oneDrawTextInfo.m_color, oneDrawTextInfo.m_fOffsetX, 
							oneDrawTextInfo.m_fOffsetY );
}

void IActorTitleDrawer::DrawOneTopTextTitles( Ogre::Camera *pCamera, const char *pszText, const Ogre::ColorQuad& color, 
												float fOffsetX, float fOffsetY, Ogre::uint shadowColor )
{
	Ogre::UIRenderer *prenderer = Ogre::UIRenderer::getSingletonPtr();
	float fTextWidth			= 0.0f; 
	float fTextHeight			= 0.0f;
	float fScale				= g_pFrameMgr->GetScreenScaleY();
	
	Ogre::HUIRES hFont = GetTitleFontRes( NAME_FONT );
	prenderer->GetTextExtent( hFont, pszText, fTextWidth, fTextHeight );

	float fTextStartScreenX = s_curScreenPos.x - ( fTextWidth * 0.5f + fOffsetX )  * fScale;
	float fTextStartScreenY	= s_curScreenPos.y + ( fOffsetY * fTextHeight - 10 )	* fScale;
	prenderer->renderText( hFont, FONTSYTLE_BORDER, pszText, fTextStartScreenX, fTextStartScreenY, color, fScale, 
							false, /*s_curScreenPos.z*/0 );
}

void IActorTitleDrawer::DrawOneIconTitle( Ogre::Camera *pCamera, const tagDrawTitleIconInfo& oneDrawIconInfo )
{
	if ( oneDrawIconInfo.m_hUiRes == NULL )
	{
		return;
	}

	DrawOneIconTitle( pCamera, oneDrawIconInfo.m_hUiRes, oneDrawIconInfo.m_fOffsetX, oneDrawIconInfo.m_fOffsetY, 
						oneDrawIconInfo.m_fWidth, oneDrawIconInfo.m_fHeight );
}

void IActorTitleDrawer::DrawOneIconTitle( Ogre::Camera *pCamera, Ogre::HUIRES hUiRes, float fScreenStartX, float fScreenStartY, 
											 float fWidth, float fHeight )
{
	Ogre::UIRenderer *pUIRenderer	= Ogre::UIRenderer::getSingletonPtr();
	float fScale					= g_pFrameMgr->GetScreenScaleY();
	pUIRenderer->BeginDraw( hUiRes );
	float posx = floor( s_curScreenPos.x - fScreenStartX * fScale + 0.5f );
	float posy = floor( s_curScreenPos.y + fScreenStartY * fScale + 0.5f );
	if ( fScale < SCALE_CHG_LIMIT_VALUE )
	{
		pUIRenderer->StretchRect( posx, posy,
			fWidth * fScale, fHeight * fScale, 0xffffffff );
	}
	else
	{
		pUIRenderer->DrawRect( (int)( posx + 0.5f ), (int)( posy + 0.5f ),
								fWidth , fHeight , 0xffffffff );
	}
	pUIRenderer->EndDraw();
}

const Ogre::Vector3& IActorTitleDrawer::GetTopTitlePos()
{
	static Ogre::Vector3 s_worldPos;
	Ogre::ShowActor*	pOneShowActor	= m_pOwner->getShowActor();
	Ogre::Entity*		pOneEntity		= pOneShowActor->getCurEntity();
	if ( m_pOwner->getType() == GAT_MAINPLAYER || m_pOwner->getType() == GAT_PLAYER )
	{
		Rider& riderMgr = m_pOwner->m_Rider;
		if ( riderMgr.isOnHorse() )
		{
			s_worldPos = pOneEntity->getAnchorWorldPos( 106 );
			return s_worldPos;
		}
		else if( m_pOwner->canActWingFloat() )
		{
			s_worldPos = pOneEntity->getAnchorWorldPos( 106 );
			return s_worldPos;
		}
	}
	
	s_worldPos	= pOneEntity->getWorldPosition();
	if ( m_pOwner->isJumping() )
	{
		//TODO HERE 在变身的时候还是需要改变高度的。。。。
		s_worldPos.y = pOneEntity->getAnchorWorldPos( 106 ).y;
	}
	else
	{
		float fCurActorHeight = pOneShowActor->getActorHeight() * pOneEntity->getScale().y;
		s_worldPos.y += fCurActorHeight;
	}

	return s_worldPos;
}

bool IActorTitleDrawer::isInStall()
{
	return m_pOwner->isInStall();
}

float IActorTitleDrawer::GetNameOffsetX()
{
	return 0.0f;
}

float IActorTitleDrawer::GetNameOffsetY()
{
	return 0.0f;
}

const char* IActorTitleDrawer::GetName()
{
	return m_Name.c_str();
}

Ogre::ColorQuad IActorTitleDrawer::GetNameColor()
{
	return Ogre::ColorQuad( 255, 255, 255 );
}

float IActorTitleDrawer::GetClanNameOffsetX()
{
	return 0.0f;
}

float IActorTitleDrawer::GetClanNameOffsetY()
{
	return 0.0f;
}

const char* IActorTitleDrawer::GetClanName()
{
	return "";
}

Ogre::ColorQuad IActorTitleDrawer::GetClanNameColor()
{
	return Ogre::ColorQuad( 255, 255, 255 ); 
}

float IActorTitleDrawer::GetTemaOrChengHaoNameOffsetX()
{
	return 0.0f;
}

float IActorTitleDrawer::GetTemaOrChengHaoNameOffsetY()
{
	return 0.0f;
}

const char* IActorTitleDrawer::GetTemaOrChengHaoName()
{
	return "";
}

Ogre::ColorQuad IActorTitleDrawer::GetTemaOrChengHaoNameColor()
{
	return Ogre::ColorQuad( 255, 255, 255 ); 
}

float IActorTitleDrawer::GetOwnerNameOffsetX()
{
	return 0.0f;
}

float IActorTitleDrawer::GetOwnerNameOffsetY()
{
	return 0.0f;
}

const char* IActorTitleDrawer::GetOwnerName()
{
	return "";
}

Ogre::ColorQuad IActorTitleDrawer::GetOwnerNameColor()
{
	return Ogre::ColorQuad( 255, 255, 255 ); 
}

float IActorTitleDrawer::GetStallNameOffsetX()
{
	return 0.0f;
}

float IActorTitleDrawer::GetStallNameOffsetY()
{
	return 0.0f;
}

const char* IActorTitleDrawer::GetStallName()
{
	return "";
}

Ogre::ColorQuad IActorTitleDrawer::GetStallNameColor()
{
	return Ogre::ColorQuad( 255, 255, 255 ); 
}

float IActorTitleDrawer::GetFunctionDescOffsetX()
{
	return 0.0f;
}

float IActorTitleDrawer::GetFunctionDescOffsetY()
{
	return 0.0f;
}

const char* IActorTitleDrawer::GetFunctionDesc()
{
	return "";
}

Ogre::ColorQuad IActorTitleDrawer::GetFunctionDescColor()
{
	return Ogre::ColorQuad( 255, 255, 255 ); 
}

float IActorTitleDrawer::GetLoveSignOffsetX()
{
	return 0.0f;
}

float IActorTitleDrawer::GetLoveSignOffsetY()
{
	return 0.0f;
}

const char* IActorTitleDrawer::GetLoveSign()
{
	return "";
}

Ogre::ColorQuad IActorTitleDrawer::GetLoveSignColor()
{
	return Ogre::ColorQuad( 255, 255, 255 ); 
}

float IActorTitleDrawer::GetHpOffsetX()
{
	return 0.0f;
}

float IActorTitleDrawer::GetHpOffsetY()
{
	return 0.0f;
}

float IActorTitleDrawer::GetHpWidth()
{
	return 0.0f;
}

float IActorTitleDrawer::GetHpHeight()
{
	return 0.0f;
}

float IActorTitleDrawer::GetHpBackOffsetX()
{
	return 0.0f;
}

float IActorTitleDrawer::GetHpBackOffsetY()
{
	return 0.0f;
}

float IActorTitleDrawer::GetHpBackWidth()
{
	return 0.0f;
}

float IActorTitleDrawer::GetHpBackHeight()
{
	return 0.0f;
}

float IActorTitleDrawer::GetChengHaoNameIconOffsetX()
{
	return 0.0f;
}

float IActorTitleDrawer::GetChengHaoNameIconOffsetY()
{
	return 0.0f;
}

float IActorTitleDrawer::GetChengHaoNameIconWidth()
{
	return 0.0f;
}

float IActorTitleDrawer::GetChengHaoNameIconHeight()
{
	return 0.0f;
}

float IActorTitleDrawer::GetNameIconOffsetX()
{
	return 0.0f;
}

float IActorTitleDrawer::GetNameIconOffsetY()
{
	return 0.0f;
}

float IActorTitleDrawer::GetNameIconWidth()
{
	return 0.0f;
}

float IActorTitleDrawer::GetNameIconHeight()
{
	return 0.0f;
}
float IActorTitleDrawer::GetNameDrawYOffset()
{
	return 0.0;
}
