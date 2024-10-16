#pragma once

#include <stdio.h>
#include <stddef.h>
#include <string>

class TiXmlElement;
class TiXmlDocument;

class XMLNode
{
public:
	XMLNode() : m_pElement( NULL )
	{
	}

	XMLNode( TiXmlElement* pelement ) : m_pElement( pelement )
	{
	}

	/*~XMLNode();*/

	bool isNull();

	const char* getName();
	char* getFullName( char* buffer, size_t maxbuf ); //from parent --->  child
	const char* getText();

	bool hasChild( const char* pszName );
	XMLNode getChild( const char* pszName );
	XMLNode iterateChild( XMLNode previous );
	XMLNode iterateChild( );

	bool hasAttrib( const char* pszName );
	const char* attribToString( const char* pszName );
	int attribToInt( const char* pszName );
	float attribToFloat( const char* pszName );
	bool attribToBool( const char* pszName );

	bool attribToInt( const char* pszName, int& value );
	bool attribToFloat( const char* pszName, float& value );

	void setAttribInt( const char* pszName, int v );
	void setAttribFloat( const char* pszName, float v );
	void setAttribBool( const char* pszName, bool b );
	void setAttribStr( const char* pszName, const char* pstr );

public:
	TiXmlElement *m_pElement;
};

inline bool XMLNode::isNull()
{
	return m_pElement == NULL;
}

class XMLData
{
public:
	XMLData();
	~XMLData();

	bool loadBuffer(const void *pbuf, size_t bufsize);
	bool loadStream();
	bool loadFile(const std::string &filename);
	bool loadRawFile(const std::string &filename);

	bool saveFile(const std::string &filename);

	XMLNode getRootNode();
	XMLNode getNodeByPath(const char *path); //path="Config.RenderSystem.FXSetting"， 如果中间任何一个节点不存在， 返回空节点

private:

	void validateFileName( char* buffer, size_t maxbuf, const char* psrc );
	void CreateFilePathDir(const char *path);
	void openFileStream( const std::string& strFilename, bool bIsRead );
	void* getMemoryImage();

private:
	TiXmlDocument*	m_pDoc;
	FILE*			m_pFileHandle;
	void*			m_pMemImage;
	/// The name (e.g. resource name) that can be used to identify the source fot his data (optional)
	std::string		m_strName;		
	/// Size of the data in the stream (may be 0 if size cannot be determined)
	size_t			m_nSize;
};
