#include "xml_multieditboxparser.h"
#include "ui_framemgr.h"
#include "xml_fontstringparser.h"

using namespace Ogre;

//-----------------------------------------------------------------------------
// UI XML MultiEditBox Parser
//-----------------------------------------------------------------------------
XMLMultiEditBoxParser::XMLMultiEditBoxParser()
{
}

XMLMultiEditBoxParser::~XMLMultiEditBoxParser()
{
}

bool XMLMultiEditBoxParser::LoadUIObjectParam( UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate )
{
	XMLFrameParser::LoadUIObjectParam( pdestobj, pNode, bTemplate );

	m_pDestMultiEditBox = static_cast<MultiEditBox *>(pdestobj);
	
	if ( pNode.attribToString( "slidername" ) )
	{
		m_pDestMultiEditBox->m_SliderName = pNode.attribToString("slidername");
	}

	if ( pNode.attribToString( "letters" ) )
	{
		m_pDestMultiEditBox->m_MaxChar = pNode.attribToInt( "letters" );
	}

	if ( pNode.attribToString( "lineInterval" ) )
	{
		m_pDestMultiEditBox->m_RowSpace = pNode.attribToInt( "lineInterval" );
	}

	bool bFound = false;
	if ( pNode.attribToString( "fonttype" ) )
	{
		for( int i = 0; i < ( int )g_pFrameMgr->m_HMAYFONT.size(); i++ )
		{
			if( pNode.attribToString("fonttype") == g_pFrameMgr->m_HMAYFONT[i].name )
			{
				m_pDestMultiEditBox->m_FontIndex = i;
				bFound = true;
				break;
			}
		}
	}

	if ( !bFound )
	{
		assert(0 && "wrong Font Type");
	}

	XMLNode pChildNode = pNode.iterateChild();
	while(!pChildNode.isNull())
	{
		if( stricmp( pChildNode.getName(), "editselcolor" ) == 0 )
		{
			if( pChildNode.attribToString( "r" ) )
			{
				m_pDestMultiEditBox->m_EditSelColor.r = pChildNode.attribToInt( "r" );
			}

			if( pChildNode.attribToString( "g" ) )
			{
				m_pDestMultiEditBox->m_EditSelColor.g = pChildNode.attribToInt( "g" );
			}

			if( pChildNode.attribToString( "b" ) )
			{
				m_pDestMultiEditBox->m_EditSelColor.b = pChildNode.attribToInt( "b" );
			}
		}
		
		if( stricmp( pChildNode.getName(), "cursorcolor" ) == 0 )
		{
			if( pChildNode.attribToString( "r" ) )
			{
				m_pDestMultiEditBox->m_CurSorColor.r = pChildNode.attribToInt( "r" );
			}

			if( pChildNode.attribToString( "g" ) )
			{
				m_pDestMultiEditBox->m_CurSorColor.g = pChildNode.attribToInt( "g" );
			}

			if( pChildNode.attribToString( "b" ) )
			{
				m_pDestMultiEditBox->m_CurSorColor.b = pChildNode.attribToInt( "b" );
			}
		}

		if( stricmp( pChildNode.getName(), "textcolor" ) == 0 )
		{
			if( pChildNode.attribToString( "r" ) )
			{
				m_pDestMultiEditBox->m_DrawColor.r = pChildNode.attribToInt( "r" );
			}

			if( pChildNode.attribToString( "g" ) )
			{
				m_pDestMultiEditBox->m_DrawColor.g = pChildNode.attribToInt( "g" );
			}

			if( pChildNode.attribToString( "b" ) )
			{
				m_pDestMultiEditBox->m_DrawColor.b = pChildNode.attribToInt( "b" );
			}
		}
		pChildNode = pNode.iterateChild( pChildNode );
	}

	return true;
}