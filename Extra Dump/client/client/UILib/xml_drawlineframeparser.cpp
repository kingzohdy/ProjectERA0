#pragma warning( disable : 4996 )

#include "xml_drawlineframeparser.h"
#include "ui_drawlineframe.h"
#include "ui_framemgr.h"
#include "OgreStringUtil.h"

using namespace Ogre;

extern FrameManager *g_pFrameMgr;
//-----------------------------------------------------------------------------
// UI XML DrawLineFrame
//-----------------------------------------------------------------------------
XMLDrawLineFrameParser::XMLDrawLineFrameParser()
{
}

XMLDrawLineFrameParser::~XMLDrawLineFrameParser()
{
}

bool XMLDrawLineFrameParser::LoadUIObjectParam(UIObject *pdestobj, XMLNode pNode, bool bTemplate)
{
	XMLLayoutFrameParser::LoadUIObjectParam(pdestobj, pNode, bTemplate);
	XMLNode pChildNode	= pNode.iterateChild();
	m_pDestLineFrame	= static_cast<DrawLineFrame*>(pdestobj);
	m_pDestLineFrame->m_iTexX = 0;
	m_pDestLineFrame->m_iTexY = 0;
	m_pDestLineFrame->m_iTexWidth = 0;
	m_pDestLineFrame->m_iTexHeight = 0;

	if ( pNode.attribToString( "file" ))
	{
		wchar_t	buf[1024];
		StringUtil::UTF8ToUnicode( buf, 1024, pNode.attribToString("file") );
		char	buf2[1024];
		StringUtil::UnicodeToAnsi( buf2, 1024, buf );
		m_pDestLineFrame->m_hTex = g_pDisplay->CreateTexture( buf2, BLEND_ALPHABLEND, &m_pDestLineFrame->m_iTexWidth, &m_pDestLineFrame->m_iTexHeight );
	}
	if( pNode.hasAttrib( "x" ) )
	{
		m_pDestLineFrame->m_iTexX = pNode.attribToInt("x");
	}
	if( pNode.hasAttrib( "y" ) )
	{
		m_pDestLineFrame->m_iTexY = pNode.attribToInt("y");
	}
	if( pNode.hasAttrib( "w" ) )
	{
		m_pDestLineFrame->m_iTexWidth = pNode.attribToInt("w");
	}
	if( pNode.hasAttrib("h") )
	{
		m_pDestLineFrame->m_iTexHeight = pNode.attribToInt("h");
	}

	while( !pChildNode.isNull() )
	{
		if ( stricmp(pChildNode.getName(), "Color") == 0 )
		{
			if ( pChildNode.attribToString( "r" ) )
			{
				m_pDestLineFrame->m_LineColor.r = pChildNode.attribToInt( "r" );
			}

			if ( pChildNode.attribToString( "g" ) )
			{
				m_pDestLineFrame->m_LineColor.g = pChildNode.attribToInt( "g" );
			}

			if ( pChildNode.attribToString( "b" ) )
			{
				m_pDestLineFrame->m_LineColor.b = pChildNode.attribToInt( "b" );
			}
		}

		pChildNode = pNode.iterateChild(pChildNode);
	}

	return true;
}