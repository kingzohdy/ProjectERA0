#include "ui_texture.h"
#include "ui_framemgr.h"
#include "OgreResourceManager.h"
#include "OgreRoot.h"
#include "OgreLog.h"

//-----------------------------------------------------------------------------
// Texture
//-----------------------------------------------------------------------------
extern FrameManager *g_pFrameMgr;

IMPLEMENT_UIOBJECT_CLONE(Texture)

Texture::Texture()
{
	m_hTex		= g_pDisplay->GetNullTexture();
	m_hLastTex	= NULL;
	m_hMaskTex	= Ogre::HUIRES(0);
	m_hTexRes	= NULL;
	m_hMaskTexRes = NULL;
	m_bUVAnim = m_bUIAnimLoop = m_bScanl = false;
	m_iTexX = m_iTexY = 0;
	m_iTexWidth = m_iTexHeight = 0;
	m_fTexRelX = m_fTexRelY = m_fTexRelWidth = m_fTexRelHeight = 0;
	m_iTimeSlot = m_iCurTime = 0;
	m_iTexRows = m_iTexCols = 0;
	m_fOffsetX = m_fOffsetY = 0.0f;

	m_iRelWidth = m_iRealHeight =0;
	
	m_fAngle = 0.0f;

	m_BlendMod = Ogre::BLEND_ALPHABLEND;

	m_Color = Ogre::ColorQuad(255, 255, 255,255);
	m_bGray = false;

	m_nNineSquareOffsetX = 0;
	m_nNineSquareOffsetY = 0;
	m_iTileSize = 0;
	m_BackgroundInsets = Ogre::RectInt(0, 0, 0, 0);

	m_DrawType = eDraw_Normal;
	m_UvType = Ogre::UI_UVT_NORMAL;

	m_bRotateTopLeft = false;
	m_bAlphaAmin = false;
	m_fSpeed = 0.0f;
}

Texture::~Texture()
{
	g_pDisplay->ReleaseUIRes(m_hTex);
	g_pDisplay->ReleaseUIRes(m_hLastTex);
	g_pDisplay->ReleaseUIRes(m_hMaskTex);
	if (m_hTexRes)
	{
		breakLoad(m_hTexRes);
	}
	if (m_hMaskTexRes)
	{
		breakLoad(m_hMaskTexRes);
	}
}

void Texture::CopyMembers(Texture *pdest)
{
	if (pdest)
	{
		LayoutFrame::CopyMembers(pdest);

		pdest->m_hTex = m_hTex;
		g_pDisplay->AddRef(pdest->m_hTex);
		pdest->m_BlendMod = m_BlendMod;
		pdest->m_TexturePath = m_TexturePath;

		pdest->m_Color = m_Color;
		pdest->m_bGray = m_bGray;

		pdest->m_iRelWidth = m_iRelWidth;
		pdest->m_iRealHeight = m_iRealHeight;
		pdest->m_iTexRows = m_iTexRows;
		pdest->m_iTexCols = m_iTexCols;

		pdest->m_fAngle = m_fAngle;

		pdest->m_bUVAnim = m_bUVAnim;
		pdest->m_bUIAnimLoop = m_bUIAnimLoop;

		pdest->m_iTimeSlot = m_iTimeSlot;
		pdest->m_iTexX = m_iTexX;
		pdest->m_iTexY = m_iTexY;
		pdest->m_iTexWidth = m_iTexWidth;
		pdest->m_iTexHeight = m_iTexHeight;
		pdest->m_fOffsetY = m_fOffsetY;
		pdest->m_fOffsetX = m_fOffsetX;

		pdest->m_iCurTime = m_iCurTime;
		pdest->m_iTileSize = m_iTileSize;
		pdest->m_BackgroundInsets = m_BackgroundInsets;
		pdest->m_TopLeftUV.set( m_TopLeftUV );
		pdest->m_TopUV.set( m_TopUV );
		pdest->m_TopRightUV.set( m_TopRightUV );
		pdest->m_LeftUV.set( m_LeftUV );
		pdest->m_CenterUV.set( m_CenterUV );
		pdest->m_RightUV.set( m_RightUV );
		pdest->m_BottomLeftUV.set( m_BottomLeftUV );
		pdest->m_BottomUV.set( m_BottomUV );
		pdest->m_BottomRightUV.set( m_BottomRightUV );
		pdest->m_nNineSquareOffsetX = m_nNineSquareOffsetX;
		pdest->m_nNineSquareOffsetY = m_nNineSquareOffsetY;

		pdest->m_DrawType = m_DrawType;
		pdest->m_UvType = m_UvType;

		pdest->m_bRotateTopLeft = m_bRotateTopLeft;
		pdest->m_bAlphaAmin = m_bAlphaAmin;
		pdest->m_fSpeed = m_fSpeed;
	}
}

void Texture::SetBlendMode(Ogre::UIBlendMode blendmode)
{
	m_BlendMod = blendmode;
}

void Texture::SetUVAnimation(int timeslot, bool loopmode)
{
	m_bShow = m_bUVAnim = true;
	m_bUIAnimLoop = loopmode;
	m_iTimeSlot = timeslot;
	m_iCurTime = 0;
}

void Texture::SetTextureHuires(Ogre::HUIRES hTex)
{
	Ogre::HUIRES hTmp = m_hTex;
	m_hTex = hTex;
	g_pDisplay->AddRef(m_hTex);
	g_pDisplay->ReleaseUIRes(hTmp);
}

Ogre::HUIRES Texture::GetTextureHuires()
{
	return m_hTex;
}

void Texture::SetColor(int r, int g, int b)
{
	m_Color = Ogre::ColorQuad(r, g, b, 255);
}

void Texture::SetGray (bool bGray)
{
	m_bGray = bGray;
}

bool Texture::IsGray ()
{
	return m_bGray;
}

void Texture::SetTexture(const char *path)
{
	if (path == NULL || path[0] == 0)
	{
		return;
	}
	if (m_TexturePath == path)
	{
		return;
	}
	m_TexturePath = path;
	SetTexture(path, false);
}

void Texture::SetTexture(const char *path, bool block)
{
	if (m_hTexRes)
	{
		breakLoad(m_hTexRes);
		m_hTexRes = NULL;
	}
	if (block)
	{
		Ogre::HUIRES h = m_hTex;
		m_hTex = g_pDisplay->CreateTexture(path, Ogre::BLEND_ALPHABLEND, &m_iRelWidth, &m_iRealHeight);
		g_pDisplay->ReleaseUIRes(m_hLastTex);
		m_hLastTex = h;
		if (m_hTex == NULL)
		{
			m_hTex = g_pDisplay->GetNullTexture();
		}

		if (m_fTexRelX != 0 || m_fTexRelY != 0 || m_fTexRelWidth != 0 || m_fTexRelHeight != 0)
		{
			m_iTexX = FloatToInt(m_fTexRelX * m_iRelWidth);
			m_iTexY = FloatToInt(m_fTexRelY * m_iRealHeight);
			m_iTexWidth = FloatToInt(m_fTexRelWidth * m_iRelWidth);
			m_iTexHeight = FloatToInt(m_fTexRelHeight * m_iRealHeight);
		}
	}
	else
	{
		if (g_pDisplay->GetTexture(path))
		{
			SetTexture(path, true);
		}
		else
		{
			if (m_hTex != NULL)
			{
				Ogre::HUIRES h = m_hTex;
				m_hTex = NULL;
				g_pDisplay->ReleaseUIRes(m_hLastTex);
				m_hLastTex = h;
			}
			char buffer[256];
			Ogre::ValidateFileName(buffer, 256, path);
			m_hTexRes = backgroundLoad(buffer,
				Ogre::ResourceManager::DP_TEXTURE);
		}
	}
}

void Texture::SetTexture(Ogre::Resource *pres)
{
	Ogre::HUIRES h = m_hTex;
	m_hTex = g_pDisplay->CreateTexture(pres, Ogre::BLEND_ALPHABLEND, &m_iRelWidth, &m_iRealHeight);
	g_pDisplay->ReleaseUIRes(m_hLastTex);
	m_hLastTex = h;
	if (m_hTex == NULL)
	{
		m_hTex = g_pDisplay->GetNullTexture();
	}

	if (m_fTexRelX != 0 || m_fTexRelY != 0 || m_fTexRelWidth != 0 || m_fTexRelHeight != 0)
	{
		m_iTexX = FloatToInt(m_fTexRelX * m_iRelWidth);
		m_iTexY = FloatToInt(m_fTexRelY * m_iRealHeight);
		m_iTexWidth = FloatToInt(m_fTexRelWidth * m_iRelWidth);
		m_iTexHeight = FloatToInt(m_fTexRelHeight * m_iRealHeight);
	}
}

const char *Texture::GetTexture()
{
	return m_TexturePath.c_str();
}

void Texture::SetAngle(float fAngle)
{
	m_fAngle = fAngle;
}

float Texture::GetAngle()
{
	return m_fAngle;
}

//设置贴图的UV坐标，x，y表示起始的像素，width和height是像素宽度和高度
void Texture::SetTexUV(int x, int y, int width, int height)
{
	m_iTexX = x;
	m_iTexY = y;
	m_iTexWidth = width;
	m_iTexHeight = height;
	m_fTexRelX = m_fTexRelY = m_fTexRelWidth = m_fTexRelHeight = 0;
}

void Texture::SetTexRelUV(float width, float height)
{
	SetTexRelUV(0, 0, width, height);
}

void Texture::SetTexRelUV(float x, float y, float width, float height)
{
	m_fTexRelX = x;
	m_fTexRelY = y;
	m_fTexRelWidth = width;
	m_fTexRelHeight = width;

	m_iTexX = FloatToInt(x * m_iRelWidth);
	m_iTexY = FloatToInt(y * m_iRealHeight);
	m_iTexWidth = FloatToInt(width * m_iRelWidth);
	m_iTexHeight = FloatToInt(width * m_iRealHeight);
}

int Texture::getRelWidth()
{
	while (m_hTexRes != NULL)
	{
		::Sleep(0);
		Ogre::ResourceManager::getSingleton().loadedCallback();
	}
	return m_iRelWidth;
}

int Texture::getRelHeight()
{
	while (m_hTexRes != NULL)
	{
		::Sleep(0);
		Ogre::ResourceManager::getSingleton().loadedCallback();
	}
	return m_iRealHeight;
}

void Texture::setTextOffsetY( float person )
{
	m_fOffsetY = person;
}

void Texture::setTextOffsetX( float person )
{
	m_fOffsetX = person;
}

float Texture::getTextOffsetY()
{
	return m_fOffsetY;
}

float Texture::getTextOffsetX()
{
	return m_fOffsetX;
}

void Texture::SetBlendAlpha(float alpha)
{
	assert((alpha >= 0.0f) && (alpha <= 1.0f));

	int iAlpha = FloatToInt(alpha * 255);

	m_Color.a = iAlpha;

}

float Texture::GetBlendAlpha()
{
	return static_cast<float>( m_Color.a ) / 255 ;
}

void Texture::setMask( const char* szmaskpath )
{
	if ( szmaskpath == NULL || szmaskpath[0] == 0 || m_MaskTexturePath == szmaskpath )
	{
		return;
	}
	m_MaskTexturePath = szmaskpath;
	setMask(szmaskpath, false);
}

void Texture::setMask(const char *path, bool block)
{
	if (m_hMaskTexRes)
	{
		breakLoad(m_hMaskTexRes);
		m_hMaskTexRes = NULL;
	}
	if (block)
	{
		Ogre::HUIRES h = m_hMaskTex;
		int width, height;
		m_hMaskTex = g_pDisplay->CreateTexture(path, Ogre::BLEND_ALPHABLEND, &width, &height);
		g_pDisplay->ReleaseUIRes(h);
	}
	else
	{
		if (g_pDisplay->GetTexture(path))
		{
			setMask(path, true);
		}
		else
		{
			if (m_hTex != NULL)
			{
				Ogre::HUIRES h = m_hMaskTex;
				m_hMaskTex = NULL;
				g_pDisplay->ReleaseUIRes(h);
			}
			char buffer[256];
			Ogre::ValidateFileName(buffer, 256, path);
			m_hMaskTexRes = backgroundLoad(buffer,
				Ogre::ResourceManager::DP_TEXTURE);
		}
	}
}

void Texture::setMask(Ogre::Resource *pres)
{
	Ogre::HUIRES h = m_hMaskTex;
	int width, height;
	m_hMaskTex = g_pDisplay->CreateTexture(pres, Ogre::BLEND_ALPHABLEND, &width, &height);
	g_pDisplay->ReleaseUIRes(h);
}

void Texture::UpdateSelf(float deltatime)
{
	if (!m_bShow)
	{
		return;
	}
	AdjustFrameByViewPoint();

// 	static int nNowScal = 0;
// 	static int bSmall = false;
// 	if( m_bScanl )
// 	{
// 		nNowScal++;
// 		m_AbsRect.m_Top -= (int)(nNowScal*0.1);
// 		m_AbsRect.m_Left -= (int)(nNowScal*0.1);
// 		m_AbsRect.m_Bottom += (int)(nNowScal*0.1);
// 		m_AbsRect.m_Right += (int)(nNowScal*0.1);
// 		if( nNowScal >= 20 )
// 		{
// 			bSmall = true;
// 			nNowScal++;
// 			nNowScal = -nNowScal;
// 		}
// 
// 		if( bSmall && nNowScal == 0 )
// 		{
// 			m_bScanl = false;
// 			bSmall = false;
// 		}
// 	}

	if (m_bUVAnim && m_hTex != (Ogre::HUIRES)0)
	{
		assert((m_iTexRows>0) && (m_iTexCols>0) && (m_iTimeSlot>0));

		m_iCurTime += (int)(deltatime * 1000.0f);

		int iCurFrame = m_iCurTime/m_iTimeSlot;

		m_iTexX = (int)(FloatToInt((float)m_iRelWidth)/m_iTexCols) * ((int)(iCurFrame - m_iTexCols * (iCurFrame/m_iTexCols)));
		m_iTexY = (int)(FloatToInt((float)m_iRealHeight)/m_iTexRows) * ((int)(iCurFrame/m_iTexCols));

		m_iTexWidth = (int)(FloatToInt((float)m_iRelWidth)/m_iTexCols);
		m_iTexHeight = (int)(FloatToInt((float)m_iRealHeight)/m_iTexRows);


		if (m_iCurTime > m_iTimeSlot * m_iTexRows * m_iTexCols)
		{
			m_iCurTime = 0;
			if (!m_bUIAnimLoop)
			{
				m_bUVAnim = m_bShow = false;
			}
		}
	}

	if( m_bAlphaAmin && m_hTex != (Ogre::HUIRES)0 )
	{
		float addAlpha = m_fSpeed * deltatime;		
		float alpha = GetBlendAlpha();
		alpha = alpha - addAlpha;
		if( alpha <= 0 )
		{
			m_bScanl = false;
			alpha = 1.0f;
		}
		SetBlendAlpha( alpha );
		if( m_bScanl )
		{
			SetSize( getRelWidth()*alpha*3, getRelHeight()*alpha*3 );
		}else{
			SetSize( getRelWidth()*alpha, getRelHeight()*alpha );
		}
		
	}
}

void Texture::Draw()
{
	if (m_hMaskTexRes != NULL)
	{
		return;
	}
	if ( m_hTex == ( Ogre::HUIRES )0 )
	{
		return;
	}
	else
	{
		if ( m_hTex != ( Ogre::HUIRES )1 )
		{
			g_pDisplay->SetUiTextureBlendModel(m_hTex, m_BlendMod);
		}
	}
	//如果是透明图的话，不绘制
	if( m_hTex == g_pDisplay->GetTextureRes("uires\\ui\\mask\\TouMingTongDao.tga") )
	{
		return;
	}

	//assert((m_hTex != (Ogre::HUIRES)0) && "Texture Ogre::HUIRES is NULL");

	//((ScreenTexture*)g_pDisplay->GetUiObject(m_hTex))->SetBlendMode(m_BlendMod);
	
	switch(m_DrawType)
	{
	case eDraw_Normal:
		{
			g_pDisplay->BeginDraw( m_hTex,m_hMaskTex,0,m_bGray );

			g_pDisplay->StretchRect((float)m_AbsRect.m_Left, (float)m_AbsRect.m_Top,
				(float)m_AbsRect.getWidth(), (float)m_AbsRect.getHeight(), m_Color.c, m_bGray,
				m_iTexX, m_iTexY, m_iTexWidth, m_iTexHeight, (Ogre::UiUvType)m_UvType, m_fAngle, m_fOffsetX, m_fOffsetY,m_AbsRect,m_bRotateTopLeft );

			g_pDisplay->EndDraw();
		}
		break;
	case eDraw_Tile:
		if( m_iTileSize > 0 )
		{
			DrawAsBackDrop();
		}		
		break;
	case eDraw_NineSquare:
		DrawAsNineSquare();
		break;
	case eDraw_Center:
		DrawAsCenter();
		break;
	case eDraw_Height:
		DrawAsHeight();
		break;
	default:
		break;
	}
}

TiXmlElement *Texture::Save(TiXmlElement *pElement)
{
	TiXmlElement *pTexture = LayoutFrame::Save(pElement);

	if (!pTexture)
	{
		return NULL;
	}
	
	//filepath
	if (!m_TexturePath.empty())
	{
		pTexture->SetAttribute("file", m_TexturePath.c_str());
	}

	//color
	if (!( m_Color == Ogre::ColorQuad(255, 255, 255,255)))
	{
		TiXmlElement *pTextColor = new TiXmlElement("Color");
		pTexture->LinkEndChild(pTextColor);

		pTextColor->SetAttribute("r", m_Color.r);
		pTextColor->SetAttribute("g", m_Color.g);
		pTextColor->SetAttribute("b", m_Color.b);
	}

	return pTexture;
}

void Texture::DrawAsBackDrop()
{
	float fScaleY = g_pFrameMgr->GetScreenScaleY() ;
	int nEdgeSize = FloatToInt( m_iTileSize * fScaleY );
	g_pDisplay->BeginDraw(m_hTex,0,0,m_bGray);
	TextureUV uv;
	uv.texHeight = uv.texWidth = m_iTileSize;
	//topleft
	uv.texX = m_iTexX + 4*m_iTileSize;
	uv.texY = m_iTexY;
	AddDrawRect( (float)m_AbsRect.m_Left,
		(float)m_AbsRect.m_Top, 
		(float)nEdgeSize,
		(float)nEdgeSize,
		m_Color.c,false,
		uv );
	//top
	uv.texX = m_iTexX+2*m_iTileSize;
	AddDrawRect( (float)m_AbsRect.m_Left + nEdgeSize,
		(float)m_AbsRect.m_Top,
		(float)m_AbsRect.getWidth() - 2 * nEdgeSize,
		(float)nEdgeSize,
		m_Color.c,false,
		uv );
	//topright
	uv.texX = m_iTexX+5*m_iTileSize;
	AddDrawRect( (float)m_AbsRect.m_Right - nEdgeSize,
		(float)m_AbsRect.m_Top, 
		(float)nEdgeSize,
		(float)nEdgeSize,
		m_Color.c,false,
		uv );
	//left
	uv.texX = m_iTexX;
	AddDrawRect( (float)m_AbsRect.m_Left,
		(float)m_AbsRect.m_Top + nEdgeSize, 
		(float)nEdgeSize,
		(float)m_AbsRect.getHeight() - 2 * nEdgeSize,
		m_Color.c,false,
		uv );
	//center
	uv.texX = m_iTexX+8*m_iTileSize;
	AddDrawRect( (float)m_AbsRect.m_Left + nEdgeSize,
		(float)m_AbsRect.m_Top + nEdgeSize, 
		(float)m_AbsRect.getWidth() - 2 * nEdgeSize,
		(float)m_AbsRect.getHeight() - 2 * nEdgeSize,
		m_Color.c,false,
		uv );
	//right
	uv.texX = m_iTexX+m_iTileSize;
	AddDrawRect( (float)m_AbsRect.m_Right - nEdgeSize,
		(float)m_AbsRect.m_Top + nEdgeSize, 
		(float)nEdgeSize,
		(float)m_AbsRect.getHeight() - 2 * nEdgeSize,
		m_Color.c,false,
		uv );
	//bottomleft
	uv.texX = m_iTexX+6*m_iTileSize;
	AddDrawRect( (float)m_AbsRect.m_Left,
		(float)m_AbsRect.m_Bottom - nEdgeSize,
		(float)nEdgeSize,
		(float)nEdgeSize,
		m_Color.c,false,
		uv );
	//bottom
	uv.texX = m_iTexX+3*m_iTileSize;
	AddDrawRect( (float)m_AbsRect.m_Left + nEdgeSize,
		(float)m_AbsRect.m_Bottom - nEdgeSize,
		(float)m_AbsRect.getWidth() - 2 * nEdgeSize,
		(float)nEdgeSize,
		m_Color.c,false,
		uv );
	//bottomright
	uv.texX = m_iTexX+7*m_iTileSize;
	AddDrawRect( (float)m_AbsRect.m_Right - nEdgeSize,
		(float)m_AbsRect.m_Bottom - nEdgeSize,
		(float)nEdgeSize,
		(float)nEdgeSize,
		m_Color.c,false,
		uv );
	g_pDisplay->EndDraw();
}

void Texture::AddDrawRect(float left,float top,float width,float height,unsigned int color,bool bGray,TextureUV uv)
{
	if( width <= uv.texWidth && height <= uv.texHeight )
	{
		g_pDisplay->StretchRect( left,top,width,height,color,bGray,uv.texX,uv.texY,uv.texWidth,uv.texHeight,(Ogre::UiUvType)m_UvType );
	}else{
		int nWidthNum = (int)(width / uv.texWidth);
		int nWidthOffset = FloatToInt(width - nWidthNum * uv.texWidth);
		int nHeightNum = (int)(height / uv.texHeight);
		int nHeightOffset = FloatToInt(height - nHeightNum * uv.texHeight);
		if( nWidthNum > 4000|| nHeightNum > 4000 )
		{
			LOG_INFO( "ui_texture %s size error left %f,top %f, width %f, height %f, uv.x %d, uv.y %d uv.width %d, uv.height %d",
						GetName(),left,top,width,height,uv.texX,uv.texY,uv.texWidth,uv.texHeight );
			return;
		}

		for(int i = 0 ; i < nWidthNum ; ++ i)
		{
			for( int j = 0 ; j < nHeightNum ; ++j )
			{
				g_pDisplay->StretchRect( left + i * uv.texWidth,
					top + j * uv.texHeight,
					(float)uv.texWidth,
					(float)uv.texHeight,
					color,bGray,uv.texX,uv.texY,uv.texWidth,uv.texHeight,(Ogre::UiUvType)m_UvType );
			}
		}
		if( nWidthOffset > 0  )
		{
			for( int i = 0 ; i < nHeightNum ; i++ )
			{
				g_pDisplay->StretchRect( left + nWidthNum * uv.texWidth,
					top + i * uv.texHeight,
					(float)nWidthOffset,
					(float)uv.texHeight,
					color,bGray,uv.texX,uv.texY,uv.texWidth,uv.texHeight,(Ogre::UiUvType)m_UvType );
			}
		}
		if( nHeightOffset > 0 )
		{
			for( int i = 0 ; i < nWidthNum ; i++ )
			{
				g_pDisplay->StretchRect( left + i * uv.texWidth,
					top + nHeightNum * uv.texHeight,
					(float)uv.texWidth,
					(float)nHeightOffset,
					color,bGray,uv.texX,uv.texY,uv.texWidth,uv.texHeight,(Ogre::UiUvType)m_UvType );
			}
		}
		if( nWidthOffset > 0 && nHeightOffset > 0 )
		{
			g_pDisplay->StretchRect( left + nWidthNum * uv.texWidth,
				top + nHeightNum * uv.texHeight,
				(float)nWidthOffset,
				(float)nHeightOffset,
				color,bGray,uv.texX,uv.texY,uv.texWidth,uv.texHeight,(Ogre::UiUvType)m_UvType );
		}
	}
}

void Texture::DrawAsNineSquare()
{
	float fScaleY   = g_pFrameMgr->GetScreenScaleY() ;
	int nOffsetX = FloatToInt( m_nNineSquareOffsetX * fScaleY );
	int nOffsetY = FloatToInt( m_nNineSquareOffsetY * fScaleY );
	g_pDisplay->BeginDraw(m_hTex,0,0,m_bGray);
	//topleft	
	AddDrawRect( (float)m_AbsRect.m_Left,
		(float)m_AbsRect.m_Top, 
		(float)nOffsetX,
		(float)nOffsetX,
		m_Color.c,false,
		m_TopLeftUV );
	//top
	AddDrawRect( (float)m_AbsRect.m_Left + nOffsetX,
		(float)m_AbsRect.m_Top,
		(float)m_AbsRect.getWidth() - 2 * nOffsetY,
		(float)nOffsetY,
		m_Color.c,false,
		m_TopUV );
	//topright
	AddDrawRect( (float)m_AbsRect.m_Right - nOffsetX,
		(float)m_AbsRect.m_Top, 
		(float)nOffsetX,
		(float)nOffsetY,
		m_Color.c,false,
		m_TopRightUV );
	//left
	AddDrawRect( (float)m_AbsRect.m_Left,
		(float)m_AbsRect.m_Top + nOffsetY, 
		(float)nOffsetX,
		(float)m_AbsRect.getHeight() - 2 * nOffsetY,
		m_Color.c,false,
		m_LeftUV );
	//center
	AddDrawRect( (float)m_AbsRect.m_Left + nOffsetX,
		(float)m_AbsRect.m_Top + nOffsetY, 
		(float)m_AbsRect.getWidth() - 2 * nOffsetX,
		(float)m_AbsRect.getHeight() - 2 * nOffsetY,
		m_Color.c,false,
		m_CenterUV );
	//right
	AddDrawRect( (float)m_AbsRect.m_Right - nOffsetX,
		(float)m_AbsRect.m_Top + nOffsetY, 
		(float)nOffsetX,
		(float)m_AbsRect.getHeight() - 2 * nOffsetY,
		m_Color.c,false,
		m_RightUV );
	//bottomleft
	AddDrawRect( (float)m_AbsRect.m_Left,
		(float)m_AbsRect.m_Bottom - nOffsetY,
		(float)nOffsetX,
		(float)nOffsetY,
		m_Color.c,false,
		m_BottomLeftUV );
	//bottom
	AddDrawRect( (float)m_AbsRect.m_Left + nOffsetX,
		(float)m_AbsRect.m_Bottom - nOffsetY,
		(float)m_AbsRect.getWidth() - 2 * nOffsetX,
		(float)nOffsetY,
		m_Color.c,false,
		m_BottomUV );
	//bottomright
	AddDrawRect( (float)m_AbsRect.m_Right - nOffsetX,
		(float)m_AbsRect.m_Bottom - nOffsetY,
		(float)nOffsetX,
		(float)nOffsetY,
		m_Color.c,false,
		m_BottomRightUV );
	g_pDisplay->EndDraw();
}

void Texture::DrawAsCenter()
{
	float fScaleY = g_pFrameMgr->GetScreenScaleY() ;
	int nLeftSize = FloatToInt(m_LeftUV.texWidth*fScaleY);
	int nCenterSize = FloatToInt(m_CenterUV.texWidth*fScaleY);
	int nRightSize = FloatToInt(m_RightUV.texWidth*fScaleY);
	int nTexWidth = m_AbsRect.getWidth();
	float fPos = 0.0f;
	if( nCenterSize == 0 )
	{
		return;
	}
	g_pDisplay->BeginDraw(m_hTex,0,0,m_bGray);
	if( nTexWidth < nLeftSize )
	{
		//只需要画左边
		fPos = (float)nTexWidth/nLeftSize;
		g_pDisplay->StretchRect( (float)m_AbsRect.m_Left,
			(float)m_AbsRect.m_Top,
			nTexWidth,
			(float)m_AbsRect.getHeight(),
			m_Color.c,false,
			m_LeftUV.texX,m_LeftUV.texY,m_LeftUV.texWidth*fPos,m_LeftUV.texHeight,(Ogre::UiUvType)m_UvType );
	}else{
		g_pDisplay->StretchRect( (float)m_AbsRect.m_Left,
			(float)m_AbsRect.m_Top,
			FloatToInt(m_LeftUV.texWidth*fScaleY),
			(float)m_AbsRect.getHeight(),
			m_Color.c,false,
			m_LeftUV.texX,m_LeftUV.texY,m_LeftUV.texWidth,m_LeftUV.texHeight,(Ogre::UiUvType)m_UvType );		
		int nWidth = nTexWidth - nLeftSize;
		if( nWidth < nRightSize )
		{
			//右半部分
			fPos = (float)nWidth/nRightSize;
			g_pDisplay->StretchRect( (float)m_AbsRect.m_Left+nLeftSize,
				(float)m_AbsRect.m_Top,
				nWidth,
				(float)m_AbsRect.getHeight(),
				m_Color.c,false,
				m_RightUV.texX,m_RightUV.texY,m_RightUV.texWidth*fPos,m_RightUV.texHeight,(Ogre::UiUvType)m_UvType );
		}else{
			nWidth = nWidth - nRightSize;
			int num = int(nWidth / nCenterSize );
			int offset = (nWidth - num * nCenterSize );

			for (int i=0; i<num; ++i)
			{   
				g_pDisplay->StretchRect((float)m_AbsRect.m_Left + nLeftSize + i * nCenterSize,
					(float)m_AbsRect.m_Top, 
					(float)nCenterSize, 
					(float)m_AbsRect.getHeight(), 
					m_Color.c,false, m_CenterUV.texX, m_CenterUV.texY, m_CenterUV.texWidth, m_CenterUV.texHeight,(Ogre::UiUvType)m_UvType );
			}
			if ( offset > 0 )
			{
				fPos = (float)offset/nCenterSize;
				g_pDisplay->StretchRect( (float)m_AbsRect.m_Left + nLeftSize + num * nCenterSize, 
					(float)m_AbsRect.m_Top, 
					(float)offset, 
					(float)m_AbsRect.getHeight(), 
					m_Color.c, false,m_CenterUV.texX, m_CenterUV.texY, FloatToInt(m_CenterUV.texWidth*fPos), m_CenterUV.texHeight,(Ogre::UiUvType)m_UvType );
			}
			g_pDisplay->StretchRect( (float)m_AbsRect.m_Right-nRightSize,
				(float)m_AbsRect.m_Top,
				nRightSize,
				(float)m_AbsRect.getHeight(),
				m_Color.c,false,
				m_RightUV.texX,m_RightUV.texY,m_RightUV.texWidth,m_RightUV.texHeight,(Ogre::UiUvType)m_UvType );
		}
	}
	g_pDisplay->EndDraw();
}

void Texture::DrawAsHeight()
{
	float fScaleY = g_pFrameMgr->GetScreenScaleY() ;
	int nTopSize = FloatToInt(m_TopUV.texHeight*fScaleY);
	int nCenterSize = FloatToInt(m_CenterUV.texHeight*fScaleY);
	int nBottomSize = FloatToInt(m_BottomUV.texHeight*fScaleY);
	int nTexHeight = m_AbsRect.getHeight();
	float fPos = 0.0f;
	if( nCenterSize == 0 )
	{
		return;
	}
	g_pDisplay->BeginDraw(m_hTex,0,0,m_bGray);

	if (nTexHeight<=nTopSize+nBottomSize)
	{
		//所需长度不足上下纹理只画上面和下面部分
		fPos=(float)nTexHeight/(2.0*nTopSize);
		g_pDisplay->StretchRect( (float)m_AbsRect.m_Left,
			(float)m_AbsRect.m_Top,
			(float)m_AbsRect.getWidth(),
			(float)nTexHeight/2.0,
			m_Color.c,false,
			m_TopUV.texX,m_TopUV.texY,m_TopUV.texWidth,m_TopUV.texHeight*fPos,(Ogre::UiUvType)m_UvType );
		g_pDisplay->StretchRect( (float)m_AbsRect.m_Left,
			(float)m_AbsRect.m_Top+(float)nTexHeight/2.0,
			(float)m_AbsRect.getWidth(),
			(float)nTexHeight/2.0,
			m_Color.c,false,
			m_BottomUV.texX,m_BottomUV.texY+m_BottomUV.texHeight*(1-fPos),m_BottomUV.texWidth,m_BottomUV.texHeight*fPos,(Ogre::UiUvType)m_UvType );
	}
	else
	{
		int nTempTexHeight	=	nTexHeight-nTopSize-nBottomSize;
		int nNums			=	nTempTexHeight/nCenterSize;
		int offset			=	nTempTexHeight-nNums*nCenterSize;
		float fPosCenter	=	(float)offset/(float)nCenterSize;
		g_pDisplay->StretchRect( (float)m_AbsRect.m_Left,
			(float)m_AbsRect.m_Top,
			(float)m_AbsRect.getWidth(),
			nTopSize,
			m_Color.c,false,
			m_TopUV.texX,m_TopUV.texY,m_TopUV.texWidth,m_TopUV.texHeight,(Ogre::UiUvType)m_UvType );
		float fIncrease=nTopSize;
		for ( int i = 0;i < nNums;i++ )
		{
			g_pDisplay->StretchRect( (float)m_AbsRect.m_Left,
				(float)m_AbsRect.m_Top+fIncrease,
				(float)m_AbsRect.getWidth(),
				(float)nCenterSize,
				m_Color.c,false,
				m_CenterUV.texX,m_CenterUV.texY,m_CenterUV.texWidth,m_CenterUV.texHeight,(Ogre::UiUvType)m_UvType );
				fIncrease=fIncrease+(float)nCenterSize;
		}
		g_pDisplay->StretchRect( (float)m_AbsRect.m_Left,
			(float)m_AbsRect.m_Top+fIncrease,
			(float)m_AbsRect.getWidth(),
			(float)offset,
			m_Color.c,false,
			m_CenterUV.texX,m_CenterUV.texY,m_CenterUV.texWidth,m_CenterUV.texHeight*fPosCenter,(Ogre::UiUvType)m_UvType );
		fIncrease=fIncrease+offset;
		g_pDisplay->StretchRect( (float)m_AbsRect.m_Left,
			(float)m_AbsRect.m_Top+fIncrease,
			(float)m_AbsRect.getWidth(),
			(float)nBottomSize,
			m_Color.c,false,
			m_BottomUV.texX,m_BottomUV.texY,m_BottomUV.texWidth,m_BottomUV.texHeight,(Ogre::UiUvType)m_UvType );
	}
	g_pDisplay->EndDraw();
}

void Texture::ChangeTextureTemplate( const char* template_name )
{
	SetTextureTemplate(template_name);
}

void Texture::SetTextureTemplate( std::string template_name )
{
	UIObject* pObj = g_pFrameMgr->getTemplateObject(template_name);
	if( pObj != NULL && strcmp(pObj->GetTypeName(),"Texture") ==0 )
	{
		Texture* ptex = (Texture*)pObj;
		g_pDisplay->ReleaseUIRes(m_hTex);
		m_hTex = AssignHUIRes(ptex->m_hTex);
		m_BlendMod		= ptex->m_BlendMod;
		m_TexturePath	= ptex->m_TexturePath;
		m_Color			= ptex->m_Color;
		m_bGray			= ptex->m_bGray;
		m_iRelWidth		= ptex->m_iRelWidth;
		m_iRealHeight	= ptex->m_iRealHeight;
		m_iTexRows		= ptex->m_iTexRows;
		m_iTexCols		= ptex->m_iTexCols;
		m_fAngle		= ptex->m_fAngle;
		m_bUVAnim		= ptex->m_bUVAnim;
		m_bUIAnimLoop	= ptex->m_bUIAnimLoop;
		m_iTimeSlot		= ptex->m_iTimeSlot;
		m_iTexX			= ptex->m_iTexX;
		m_iTexY			= ptex->m_iTexY;
		m_iTexWidth		= ptex->m_iTexWidth;
		m_iTexHeight	= ptex->m_iTexHeight;
		m_fOffsetY		= ptex->m_fOffsetY;
		m_fOffsetX		= ptex->m_fOffsetX;
		m_iCurTime		= ptex->m_iCurTime;
		m_iTileSize		= ptex->m_iTileSize;
		m_DrawType		= ptex->m_DrawType;
		m_nNineSquareOffsetX = ptex->m_nNineSquareOffsetX;
		m_nNineSquareOffsetY = ptex->m_nNineSquareOffsetY;
		m_BackgroundInsets	= ptex->m_BackgroundInsets;
		m_TopLeftUV.set( ptex->m_TopLeftUV );
		m_TopUV.set( ptex->m_TopUV );
		m_TopRightUV.set( ptex->m_TopRightUV );
		m_LeftUV.set( ptex->m_LeftUV );
		m_CenterUV.set( ptex->m_CenterUV );
		m_RightUV.set( ptex->m_RightUV );
		m_BottomLeftUV.set( ptex->m_BottomLeftUV );
		m_BottomUV.set( ptex->m_BottomUV );
		m_BottomRightUV.set( ptex->m_BottomRightUV );
	}
}

void Texture::setUvType(int nType)
{
	m_UvType = nType;
}

void Texture::StopUVAnim()
{
	m_bUVAnim = false;
	DrawShow( false );
}

void Texture::StartAlphaAmin( float speed )
{
	SetBlendAlpha( 1.0f );
	m_bScanl = m_bShow = m_bAlphaAmin = true;
	m_fSpeed = speed;
}

void Texture::StopAlphaAmin()
{
	m_bAlphaAmin = false;
}

void Texture::ResourceLoaded(Ogre::Resource *rec, Ogre::ResourceHandle h)
{
	if (h == m_hTexRes)
	{
		if (rec)
		{
			SetTexture(rec);
		}
		m_hTexRes = NULL;
	}
	else if (h == m_hMaskTexRes)
	{
		if (rec)
		{
			setMask(rec);
		}
		m_hMaskTexRes = NULL;
	}
}

/*
//-----------------------------------------------------------------------------
// textureregion lua function
//-----------------------------------------------------------------------------
DECLARE_CLASS_TO_SCRIPT_EX(Texture, LayoutFrame, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_)

//Texture
DECLARE_MEM_FUNC_TO_SCRIPT(Texture, SetUVAnimation, SVT_EMPTY, SVT_INT, SVT_BOOL);
DECLARE_MEM_FUNC_TO_SCRIPT(Texture, SetTextureHuires, SVT_EMPTY, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(Texture, GetTextureHuires, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(Texture, SetColor, SVT_EMPTY, SVT_INT, SVT_INT, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(Texture, SetTexture, SVT_EMPTY, SVT_PTR_STRING, SVT_PTR_STRING);
DECLARE_MEM_FUNC_TO_SCRIPT(Texture, GetTexture, SVT_PTR_STRING);
DECLARE_MEM_FUNC_TO_SCRIPT(Texture, SetAngle, SVT_EMPTY, SVT_FLOAT);
DECLARE_MEM_FUNC_TO_SCRIPT(Texture, GetAngle, SVT_FLOAT);
DECLARE_MEM_FUNC_TO_SCRIPT(Texture, SetTexUV, SVT_EMPTY, SVT_INT, SVT_INT, SVT_INT, SVT_INT);

*/