#pragma once
#include "xml_layoutframeparser.h"
#include "ui_frame.h"


class XMLFrameParser : public XMLLayoutFrameParser
{
public:
	XMLFrameParser();
	~XMLFrameParser();

	virtual bool LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate = false);

	bool LayersParser(Ogre::XMLNode pNode, bool bTemplate);
	bool BackDropParser(Frame *pFrame, Ogre::XMLNode pNode);

	bool FrameParserRecursive(Frame *pFrame, Ogre::XMLNode pNode);

private:
	Frame *m_pDestFrame;
};

