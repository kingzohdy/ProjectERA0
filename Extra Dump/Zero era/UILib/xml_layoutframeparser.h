#pragma once
#include "xml_uimgr.h"
#include "ui_layoutframe.h"


class XMLLayoutFrameParser : public XMLUIObjectParser
{
public:
	XMLLayoutFrameParser();
	~XMLLayoutFrameParser();

	virtual bool LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate = false);

	void FrameStrataParser(const char *pstr);

	LayoutDim SizeParser(Ogre::XMLNode pNode);
	LayoutAnchor AnchorsParser(Ogre::XMLNode pNode, const std::string nodename, UIObject *pdestobj, bool bTemplate);

private:
	LayoutFrame *m_pDestLayoutFrame;
};
