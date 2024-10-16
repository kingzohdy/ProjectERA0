#include "StdAfx.h"
#include "XMLData.h"
#include "tinyxml/tinyxml.h"
// #include "util/utils.h"
#include <Windows.h>
#include <list>
using namespace std;

template<typename T>
void SafeDelete( T*& p )
{
	if ( p != NULL )
	{
		delete p;
		p = NULL;
	}
}


	const int LIBMPQ_TOOLS_BUFSIZE = 0x500;	/* buffer size for the decryption engine */
	static unsigned int	mpq_buffer[LIBMPQ_TOOLS_BUFSIZE];

	//------------------------------------------------------------------------------------------------------------------
	/*XMLNode::~XMLNode()
	{
		if ( m_pElement != NULL )
		{
			delete m_pElement;
			m_pElement = NULL;
		}
	}*/

	const char* XMLNode::getName()
	{
		return m_pElement->Value();
	}

	char* XMLNode::getFullName( char* buffer, size_t maxbuf )
	{
		std::list<string>	strs;
		string s;

		TiXmlNode* pcur = m_pElement;

		while ( pcur != NULL )
		{
			s = pcur->Value();
			strs.push_front( s );

			pcur = pcur->Parent();
		}

		s = "";
		std::list<string>::iterator iter = strs.begin();
		for ( ; iter != strs.end(); ++iter )
		{
			if ( !s.empty() ) 
			{
				s += ".";
			}

			s += *iter;
		}

		strncpy( buffer, s.c_str(), maxbuf );
		buffer[maxbuf-1] = 0;
		return buffer;
	}

	const char* XMLNode::getText()
	{
		const TiXmlNode* child = m_pElement->FirstChild();
		while ( child )
		{
			const TiXmlText* childText = child->ToText();
			if(childText) return childText->Value();

			child = child->NextSibling();
		}
		return NULL;
	}

	bool XMLNode::hasChild( const char* pszName )
	{
		return ( m_pElement->FirstChildElement( pszName ) != NULL );
	}

	XMLNode XMLNode::getChild( const char* pszName )
	{
		return XMLNode( m_pElement->FirstChildElement( pszName ) );
	}

	XMLNode XMLNode::iterateChild( XMLNode previous )
	{
		if ( previous.isNull( ) )
		{
			return XMLNode( m_pElement->FirstChildElement( ) );
		}
		else
		{
			return XMLNode( previous.m_pElement->NextSiblingElement( ) );
		}
	}

	XMLNode XMLNode::iterateChild()
	{
		return XMLNode( m_pElement->FirstChildElement( ) );
	}

	bool XMLNode::hasAttrib( const char* pszName )
	{
		return m_pElement->Attribute( pszName ) != NULL;
	}

	const char* XMLNode::attribToString( const char* pszName )
	{
		return m_pElement->Attribute( pszName );
	}

	int XMLNode::attribToInt( const char* pszName )
	{
		int v = 0;
		if ( m_pElement->QueryIntAttribute( pszName, &v ) != TIXML_SUCCESS )
		{
			assert( !"查询的值不为int类型,此为错误的查询方式!" );
		}

		return v;
	}

	float XMLNode::attribToFloat( const char* pszName )
	{
		float v = 0.0f;
		if ( m_pElement->QueryFloatAttribute( pszName, &v ) != TIXML_SUCCESS )
		{
			assert( !"查询的值不为float类型,此为错误的查询方式!" );
		}

		return v;
	}

	bool XMLNode::attribToBool( const char* pszName )
	{
		const char* pstr = attribToString( pszName );
		if ( pstr == NULL )
		{
			return false;
		}
		else 
		{
			return _stricmp( pstr, "true" ) == 0;
		}
	}

	bool XMLNode::attribToInt( const char* pszName, int& v )
	{
		if ( m_pElement->QueryIntAttribute( pszName, &v ) != TIXML_SUCCESS )
		{
			return false;
		}

		return true;
	}

	bool XMLNode::attribToFloat( const char* pszName, float& v )
	{
		if ( m_pElement->QueryFloatAttribute( pszName, &v ) != TIXML_SUCCESS )
		{
			return false;
		}

		return true;
	}

	void XMLNode::setAttribInt( const char* pszName, int v )
	{
		m_pElement->SetAttribute( pszName, v );
	}

	void XMLNode::setAttribFloat( const char* pszName, float v )
	{
		char buffer[256] = { '\0' };
		sprintf_s( buffer, "%f", v );
		m_pElement->SetAttribute( pszName, buffer );
	}

	void XMLNode::setAttribBool( const char* pszName, bool b )
	{
		m_pElement->SetAttribute( pszName, b ? "true" : "false" );
	}

	void XMLNode::setAttribStr(const char* pszName, const char* pstr )
	{
		m_pElement->SetAttribute( pszName, pstr );
	}


	XMLData::XMLData(void) : m_pDoc( NULL ), m_pFileHandle( NULL ), m_pMemImage( NULL ), m_strName(), m_nSize( 0 )
	{
	}

	XMLData::~XMLData( void )
	{
		if ( m_pFileHandle != NULL )
		{
			fclose( m_pFileHandle );
			m_pFileHandle = NULL;
		}

		if ( m_pMemImage )
		{
			free( m_pMemImage );
			m_pMemImage = NULL;
		}

		if ( m_pDoc != NULL )
		{
			delete m_pDoc;
			m_pDoc = NULL;
		}
	}	

	bool XMLData::loadBuffer( const void *pbuf, size_t bufsize )
	{
		SafeDelete( m_pDoc );
		m_pDoc = new TiXmlDocument;
		if ( !m_pDoc->LoadBuffer( ( const char* )pbuf, bufsize ) )
		{
			return false;
		}

		return true;
	}

	bool XMLData::loadStream( )
	{
		return loadBuffer( getMemoryImage(), m_nSize );
	}

	bool XMLData::loadFile(const string& strFilename)
	{
		openFileStream( strFilename, true );
		if( m_pFileHandle != NULL)
		{
			bool b = loadStream( );
			fclose( m_pFileHandle );
			m_pFileHandle = NULL;
			return b;
		}
		else 
		{
			return false;
		}
	}

	bool XMLData::loadRawFile(const string& strFilename)
	{
		SafeDelete( m_pDoc );
		m_pDoc = new TiXmlDocument;
		return m_pDoc->LoadFile( strFilename.c_str( ) );
	}

	bool XMLData::saveFile(const string& strFilename)
	{
		return m_pDoc->SaveFile( strFilename.c_str( ) );
	}

	XMLNode XMLData::getRootNode()
	{
		return XMLNode( m_pDoc->RootElement( ) );
	}

	XMLNode XMLData::getNodeByPath(const char* path)
	{
		char name[256] = { '\0' };
		const char *pcur = path;

		XMLNode node = getRootNode( );
		while ( pcur != NULL && *pcur != 0 )
		{
			const char* pdot = strchr( pcur, '.' );
			if ( pdot != NULL )
			{
				size_t len = pdot - pcur;
				memcpy( name, pcur, len );
				name[len] = 0;
				pcur = pdot + 1;
			}
			else
			{
				strcpy_s( name, pcur );
				pcur = NULL;
			}

			node = node.getChild( name );
			if( node.isNull( ) )
			{
				return node;
			}
		}

		return node;
	}

	void XMLData::validateFileName( char *buffer, size_t maxbuf, const char* psrc )
	{
		int			state	= 0; //0:正常， 1:前一个是'\'
		size_t		destlen	= 0;
		const char* pcur	= psrc;

		while ( *pcur != 0 )
		{
			if ( *pcur == '/' || *pcur == '\\' )
			{
				if ( state == 0 )
				{
					buffer[destlen++] = '\\';
					state = 1;
				}
				else
				{
					int a = 0;
				}
			}
			else
			{
				buffer[destlen++] = *pcur;
				state = 0;
			}

			assert( destlen < maxbuf );
			++pcur;
		}

		buffer[destlen] = 0;
	}


	void XMLData::openFileStream( const string& strFilename, bool bIsRead )
	{
		if ( strFilename.empty( ) ) 
		{
			return;
		}

		string	strFullPath;
		bool bIsRelPath = ( strFilename[1] != ':' );
		if ( bIsRead && bIsRelPath )
		{
			//strFullPath = strRootName + strFilename;
			strFullPath = strFilename;
		}
		else
		{
			strFullPath = strFilename;
		}

		FILE* pFile = fopen( strFullPath.c_str(), bIsRead ? "rb" : "wb" );
		if ( pFile == NULL )
		{
			if ( !bIsRead )
			{
				pFile = fopen( strFullPath.c_str(), "wb" );
			}

			if ( pFile == NULL )
				{
					return;
				}
		}

		m_pFileHandle	= pFile;
		m_pMemImage		= NULL;
		// Determine size
		fseek( m_pFileHandle, 0, SEEK_END );
		m_nSize = ftell( m_pFileHandle );
		fseek( m_pFileHandle, 0, SEEK_SET );
	}

	void* XMLData::getMemoryImage()
	{
		if ( m_pMemImage == NULL )
		{
			m_pMemImage = malloc( m_nSize );
			fseek( m_pFileHandle, 0, SEEK_SET );
			fread( m_pMemImage, m_nSize, 1, m_pFileHandle );
		}

		return m_pMemImage;
	}
