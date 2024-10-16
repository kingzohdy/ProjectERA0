#pragma once
#include "xml_layoutframeparser.h"
#include "ui_texture.h"

class XMLTextureParser : public XMLLayoutFrameParser
{
public:
	XMLTextureParser();
	~XMLTextureParser();

	virtual bool LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate = false);
private:

	Texture *m_pDestTexture;

	void BackDropParser( Ogre::XMLNode pNode );
	void NineSquareParser( Ogre::XMLNode pNode );

};

