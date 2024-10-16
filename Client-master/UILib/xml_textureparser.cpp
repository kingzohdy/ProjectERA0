#pragma warning( disable : 4996 )

#include "xml_textureparser.h"
#include "ui_framemgr.h"

#include "OgreStringUtil.h"

using namespace Ogre;

extern FrameManager *g_pFrameMgr;
//-----------------------------------------------------------------------------
// UI XML Texture
//-----------------------------------------------------------------------------
XMLTextureParser::XMLTextureParser()
{

}

XMLTextureParser::~XMLTextureParser()
{
}

bool XMLTextureParser::LoadUIObjectParam(UIObject *pdestobj, XMLNode pNode, bool bTemplate)
{
	XMLLayoutFrameParser::LoadUIObjectParam(pdestobj, pNode, bTemplate);

	m_pDestTexture = static_cast<::Texture *>(pdestobj);

	if (pNode.attribToString("file"))
	{	
		if (pNode.attribToString("name"))
			m_pDestTexture->SetName(pNode.attribToString("name"));
		
        wchar_t buff[1024];
		StringUtil::UTF8ToUnicode(buff , 1024 , pNode.attribToString("file"));
        char buff2[1024];
        StringUtil::UnicodeToAnsi(buff2 , 1024 , buff);
        
        m_pDestTexture->m_TexturePath = buff2;//pNode.attribToString("file");

		UIBlendMode drawmode = BLEND_ALPHABLEND;
		if (pNode.attribToString("alphamode"))
		{
			if (stricmp(pNode.attribToString("alphamode"), "opaque") == 0)
			{
				drawmode = BLEND_OPAQUE;
			}

			if (stricmp(pNode.attribToString("alphamode"), "add") == 0)
			{
				drawmode = BLEND_ADD;
			}
			if( stricmp(pNode.attribToString("alphamode"), "alphablend") == 0 )
			{
				drawmode = BLEND_ALPHABLEND;
			}
			if( stricmp(pNode.attribToString("alphamode"), "addblend") == 0 )
			{
				drawmode = BLEND_ADDBLEND;
			}
			if( stricmp(pNode.attribToString("alphamode"), "modulate") == 0 )
			{
				drawmode = BLEND_MODULATE;
			}			
		}
		if( pNode.hasAttrib("DrawType") )
		{
			if( stricmp(pNode.attribToString("DrawType"),"normal") == 0 )
			{
				m_pDestTexture->m_DrawType = eDraw_Normal;
			}
			if( stricmp(pNode.attribToString("DrawType"),"tile") == 0 )
			{
				m_pDestTexture->m_DrawType = eDraw_Tile;
			}
			if( stricmp(pNode.attribToString("DrawType"),"ninesquare") == 0 )
			{
				m_pDestTexture->m_DrawType = eDraw_NineSquare;
			}
			if( stricmp(pNode.attribToString("DrawType"),"center") == 0 )
			{
				m_pDestTexture->m_DrawType = eDraw_Center;
			}
			if( stricmp(pNode.attribToString("DrawType"),"height") == 0 )
			{
				m_pDestTexture->m_DrawType = eDraw_Height;
			}
		}
		if( pNode.hasAttrib("UVType") )
		{
			if( stricmp(pNode.attribToString("UVType"),"normal") == 0 )
			{
				m_pDestTexture->m_UvType = Ogre::UI_UVT_NORMAL;
			}
			if( stricmp(pNode.attribToString("UVType"),"turn180") == 0 )
			{
				m_pDestTexture->m_UvType = Ogre::UI_UVT_TURN180;
			}
			if( stricmp(pNode.attribToString("UVType"),"mirroeu") == 0 )
			{
				m_pDestTexture->m_UvType = Ogre::UI_UVT_MIRROEU;
			}
			if( stricmp(pNode.attribToString("UVType"),"mirroev") == 0 )
			{
				m_pDestTexture->m_UvType = Ogre::UI_UVT_MIRROEV;
			}
		}
		
		HUIRES htex = g_pDisplay->CreateTexture(buff2 , drawmode, &m_pDestTexture->m_iRelWidth, &m_pDestTexture->m_iRealHeight);
		m_pDestTexture->SetTextureHuires(htex);
		g_pDisplay->ReleaseUIRes(htex);

		m_pDestTexture->m_BlendMod = drawmode;
		if( pNode.attribToString("x") )
		{
			m_pDestTexture->SetTexUV( pNode.attribToInt("x"), pNode.attribToInt("y"), 
				pNode.attribToInt("w"), pNode.attribToInt("h") );
		}
	}

	XMLNode pChildNode = pNode.iterateChild();
	while(!pChildNode.isNull())
	{
		if (stricmp(pChildNode.getName(), "Angle") == 0)
		{
			if (pChildNode.attribToString("value"))
			{
				m_pDestTexture->SetAngle( pChildNode.attribToFloat("value") );
			}
		}

		if (stricmp(pChildNode.getName(), "UVAnimation") == 0)
		{
			//if (pChildNode.attribToString("realwidth"))
			//{
			//	m_pDestTexture->m_iRelWidth = pChildNode.attribToInt("realwidth");
			//}

			//if (pChildNode.attribToString("realheight"))
			//{
			//	m_pDestTexture->m_iRealHeight = pChildNode.attribToInt("realheight");
			//}

			if (pChildNode.attribToString("texrows"))
			{
				m_pDestTexture->m_iTexRows = pChildNode.attribToInt("texrows");
			}

			if (pChildNode.attribToString("texcols"))
			{
				m_pDestTexture->m_iTexCols = pChildNode.attribToInt("texcols");
			}
		}

		if (stricmp(pChildNode.getName(), "TexCoords") == 0)
		{
			int texX, texY, width, height;
			texX = texY = width = height = 0;
	
			if((pChildNode.attribToString("left") != NULL) && (pChildNode.attribToString("right") != NULL)
					&& (pChildNode.attribToString("realwidth") != NULL))
			{
				width = FloatToInt((pChildNode.attribToFloat("right")
					- pChildNode.attribToFloat("left")) * pChildNode.attribToInt("realwidth"));
				texX = FloatToInt(pChildNode.attribToFloat("left") * pChildNode.attribToInt("realwidth"));
			}

			if((pChildNode.attribToString("bottom") != NULL) && (pChildNode.attribToString("top") != NULL)
					&& (pChildNode.attribToString("realheight") != NULL))
			{
				height = FloatToInt((pChildNode.attribToFloat("bottom")
					- pChildNode.attribToFloat("top")) * pChildNode.attribToInt("realheight"));
				texY = FloatToInt(pChildNode.attribToFloat("top") * pChildNode.attribToInt("realheight"));
			}

			m_pDestTexture->m_iTexX = texX;
			m_pDestTexture->m_iTexY = texY;
			m_pDestTexture->m_iTexWidth = width;
			m_pDestTexture->m_iTexHeight = height;
		}
		if( m_pDestTexture->m_DrawType == eDraw_Tile && stricmp( pChildNode.getName(),"Tile" )==0 )
		{
			BackDropParser(pChildNode);
		}
		if( m_pDestTexture->m_DrawType == eDraw_NineSquare && stricmp( pChildNode.getName(),"NineSquare" )==0 )
		{
			NineSquareParser( pChildNode );
		}
		if( m_pDestTexture->m_DrawType == eDraw_Center && stricmp(pChildNode.getName(),"Center")==0 )
		{
			NineSquareParser(pChildNode);
		}
		if( m_pDestTexture->m_DrawType == eDraw_Height && stricmp(pChildNode.getName(),"Height")==0 )
		{
			NineSquareParser(pChildNode);
		}
		pChildNode = pNode.iterateChild(pChildNode);
	}
	return true;
}

void XMLTextureParser::BackDropParser( Ogre::XMLNode pNode)
{
	XMLNode pChildNode = pNode.iterateChild();
	while(!pChildNode.isNull())
	{
		if (stricmp(pChildNode.getName(), "TileSize") == 0)
		{
			XMLNode pGrandChild = pChildNode.iterateChild();
			while(!pGrandChild.isNull())
			{
				if (stricmp(pGrandChild.getName(), "AbsValue") == 0)
				{
					if (pGrandChild.attribToString("val"))
					{
						m_pDestTexture->m_iTileSize = pGrandChild.attribToInt("val");
					}
				}
				pGrandChild = pChildNode.iterateChild(pGrandChild);
			}
		}
		if (stricmp(pChildNode.getName(), "BackgroundInsets") == 0)
		{
			XMLNode pGrandChild = pChildNode.iterateChild();
			while(!pGrandChild.isNull())
			{
				if (stricmp(pGrandChild.getName(), "AbsInset") == 0)
				{
					m_pDestTexture->m_BackgroundInsets = RectInt(5, 6, 6, 5);//给个默认值避免出错
					if (pGrandChild.attribToString("left"))
					{
						m_pDestTexture->m_BackgroundInsets.m_Left = pGrandChild.attribToInt("left");
					}
					if (pGrandChild.attribToString("right"))
					{
						m_pDestTexture->m_BackgroundInsets.m_Right = pGrandChild.attribToInt("right");
					}
					if (pGrandChild.attribToString("top"))
					{
						m_pDestTexture->m_BackgroundInsets.m_Top = pGrandChild.attribToInt("top");
					}
					if (pGrandChild.attribToString("bottom"))
					{
						m_pDestTexture->m_BackgroundInsets.m_Bottom = pGrandChild.attribToInt("bottom");
					}
				}
				pGrandChild = pChildNode.iterateChild(pGrandChild);
			}
		}
		pChildNode = pNode.iterateChild(pChildNode);
	}
}
#define PARSER_SQUARE(uv,name)	\
	if( stricmp(pChildNode.getName(),name) == 0 )	\
	{	\
	if( pChildNode.hasAttrib("x") )	\
		m_pDestTexture->##uv.texX = pChildNode.attribToInt("x");	\
	if( pChildNode.hasAttrib("y") )	\
		m_pDestTexture->##uv.texY = pChildNode.attribToInt("y");	\
	if( pChildNode.hasAttrib("w") )	\
		m_pDestTexture->##uv.texWidth = pChildNode.attribToInt("w");	\
	if( pChildNode.hasAttrib("h") )	\
		m_pDestTexture->##uv.texHeight = pChildNode.attribToInt("h");	\
	}


void XMLTextureParser::NineSquareParser( Ogre::XMLNode pNode)
{
	XMLNode pChildNode = pNode.iterateChild();
	while(!pChildNode.isNull())
	{
		PARSER_SQUARE(m_TopLeftUV,"Topleft");
		PARSER_SQUARE(m_TopUV,"Top");
		PARSER_SQUARE(m_TopRightUV,"Topright");
		PARSER_SQUARE(m_LeftUV,"Left");
		PARSER_SQUARE(m_CenterUV,"Center");
		PARSER_SQUARE(m_RightUV,"Right");
		PARSER_SQUARE(m_BottomLeftUV,"Bottomleft");
		PARSER_SQUARE(m_BottomUV,"Bottom");
		PARSER_SQUARE(m_BottomRightUV,"Bottomright");
		if( stricmp(pChildNode.getName(),"OffSet") == 0 )
		{
			if( pChildNode.hasAttrib("x") )
			{
				m_pDestTexture->m_nNineSquareOffsetX = pChildNode.attribToInt("x");
			}
			if( pChildNode.hasAttrib("y") )
			{
				m_pDestTexture->m_nNineSquareOffsetY = pChildNode.attribToInt("y");
			}
		}
		pChildNode = pNode.iterateChild(pChildNode);
	}
}
