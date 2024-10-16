
#include "xml_modelviewparser.h"
#include "ui_framemgr.h"
#include "OgreStringUtil.h"

using namespace Ogre;

//-----------------------------------------------------------------------------
// UI XML Button Parser
//-----------------------------------------------------------------------------
extern FrameManager *g_pFrameMgr;
XMLModelViewParser::XMLModelViewParser()
{
}

XMLModelViewParser::~XMLModelViewParser()
{
}

bool XMLModelViewParser::LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate)
{
	XMLLayoutFrameParser::LoadUIObjectParam(pdestobj, pNode, bTemplate);

	m_pDestView = static_cast<ModelView *>(pdestobj);

	if(pNode.hasAttrib("background"))
	{
		m_pDestView->setBackground(pNode.attribToString("background"));
	}

	if(pNode.hasChild("Background"))
	{
		XMLNode bg = pNode.getChild("Background");
		XMLNode model = bg.iterateChild();
		while(!model.isNull())
		{
			m_pDestView->addBackground(model.attribToString("file"));
			model = bg.iterateChild(model);
		}
	}

	if(pNode.hasAttrib("camera"))
	{
		m_pDestView->setCameraAnim(pNode.attribToString("camera"));
		if(pNode.hasAttrib("cam_seq"))
		{
			m_pDestView->playCameraAnim(pNode.attribToInt("cam_seq"), 0);
		}
	}

	return true;
}